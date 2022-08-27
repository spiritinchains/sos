
#include <kernel.h>
#include <multiboot2.h>

#include "vmm.h"

// 4GB / 4KB = 1M bits = 128K bytes
#define FRAME_BITMAP_SIZE (128 * 1024)

#define CURRENT_PAGE_DIR 0xFFFFFFFC

static uint8_t frame_bitmap[FRAME_BITMAP_SIZE];

extern uintptr_t _kern_end;
extern uintptr_t _kern_end_load;
extern uintptr_t _kdata_end;

void kmem_init(uintptr_t start, uintptr_t end);

enum frame_status
get_frame_status(uint32_t i) {
    return (frame_bitmap[i / 8] >> (i % 8)) & 1;
}

void 
set_frame_status(uint32_t i, enum frame_status x) {
    if (x == FRAME_FREE)
        frame_bitmap[i / 8] |= 1 << (i % 8);
    else if (x == FRAME_USED)
        frame_bitmap[i / 8] &= ~(1 << (i % 8));
}

void 
vmm_init()
{
    extern struct multiboot_tag_mmap* tag_mmap;
    uint32_t vmem_free = 0;

    // iterate memory map and set free frames
    size_t len = tag_mmap->size / tag_mmap->entry_size;
    for (int i = 0; i < len; i++)
    {
        struct multiboot_mmap_entry* e = &(tag_mmap->entries[i]);

        // only register contiguous RAM blocks after 1M
        if (e->addr >= 0x100000 && e->type == MULTIBOOT_MEMORY_AVAILABLE)
        {
            uintptr_t start = e->addr;
            uintptr_t end = e->addr + e->len;
            vmem_free += e->len;

            // skip frames kernel is loaded in
            if (start < &_kern_end_load)
                start = &_kern_end_load;
            
            while (start < end)
            {
                set_frame_status(start >> 12, FRAME_FREE);
                start += 0x1000;
            }
        }
    }

    // set up heap
    for (uintptr_t i = &_kern_end; i < &_kern_end + (32*1024*1024); i += PAGE_SIZE)
    {
        page_alloc(i);
        get_pte_virt(i)->US = 1;
        get_pte_virt(i)->RW = 1;
    }
    kmem_init(&_kern_end, &_kern_end + (32*1024*1024));
}

uint32_t
get_free_frame()
{
    // iterate frame bitmap and return first free bit index
    for (int i = 0; i < FRAME_BITMAP_SIZE; i++)
    {
        if (frame_bitmap[i] != 0)
        {
            uint8_t a = frame_bitmap[i];
            uint32_t r = 0;
            while (a & 1 != 1)
            {
                a >>= 1;
                r++;
            }
            r += i * 8;
            return r;
        }
    }
    return 0;
}

/**
 * gets a pointer to the page table entry at index pti of the page table in 
 * index pdi of the current page directory
 */
struct page_table_entry*
get_pte(uint32_t pdi, uint32_t pti)
{
    if (pdi > 1022 || pti > 1023)
        return NULL;
    uintptr_t r = 0xFFC00000;
    r |= (pdi << 12);
    r |= (pti << 2);
    return r;
}

struct page_table_entry*
get_pte_virt(uintptr_t virt)
{
    uint32_t pdi, pti;
    pdi = (virt >> 22);
    pti = (virt >> 12) & 0x3FF;
    return get_pte(pdi, pti);
}

/**
 * gets a pointer to the page directory entry at index pdi of the
 * current page directory
 */
struct page_directory_entry*
get_pde(uint32_t pdi)
{
    if (pdi > 1022)
        return NULL;
    uintptr_t r = 0xFFFFF000;
    r |= (pdi << 2);
    return r;
}

struct page_directory_entry*
get_pde_virt(uintptr_t virt)
{
    return get_pde(virt >> 22);
}

uintptr_t
get_physical_address(uintptr_t virt)
{
    return (get_pte_virt(virt)->frame_addr << 12) | (virt & 0xFFF);
}

/**
 * Allocates a page table to the page directory entry pde
 * Returns pointer to the first pte in allocated page table
 */
struct page_table_entry*
page_table_alloc(struct page_directory_entry* pde)
{
    uint32_t frame = get_free_frame();
    pde->phys_addr = frame;
}

/**
 * bind specific virtual page to specific physical frame
 * meant for general use, hence this does not check whether the physical
 * frame is free, or even valid RAM
 */
void
page_bind(uint32_t virt, uint32_t phys)
{
    // get PDI and PTI
    struct page_directory_entry* pde;
    struct page_table_entry* pte;
    
    // TODO: support allocating page under multiple PIDs
    pde = get_pde_virt(virt);
    pte = get_pte_virt(virt);

    // _nothing_ could _possibly_ go wrong if I just assume that all 
    // the structures already exist in memory, right?
    pte->frame_addr = (phys >> 12);
    pte->P = 1;
}

void 
page_alloc(uint32_t virt)
{
    uint32_t frame = get_free_frame();
    
    // TODO: this means no free pages are left - do something useful
    if (frame == 0)
        return;
    
    page_bind(virt, (frame << 12));
    set_frame_status(frame, FRAME_USED);
}

void 
page_dealloc(uint32_t virt)
{
    uint32_t frame = get_physical_address(virt) >> 12;
    set_frame_status(frame, FRAME_FREE);
}
