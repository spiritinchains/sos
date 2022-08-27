
#include <kernel.h>

#define PAGE_SIZE 4096

#define pagealign(addr) (addr + 4096 - (addr % 4096))

struct page_table_entry
{
    unsigned P : 1;                 // present
    unsigned RW : 1;                // read/write
    unsigned US : 1;                // user/supervisor
    unsigned PWT : 1;               // write-through
    unsigned PCD : 1;               // cache disable
    unsigned A : 1;                 // accessed
    unsigned D : 1;                 // dirty
    unsigned PAT : 1;               // page attr table
    unsigned G : 1;                 // global
    unsigned unused : 3;
    unsigned frame_addr : 20;
}__attribute__((packed));

struct page_directory_entry
{
    unsigned P : 1;                 // present
    unsigned RW : 1;                // read/write
    unsigned US : 1;                // user/supervisor
    unsigned PWT : 1;               // write-through
    unsigned PCD : 1;               // cache disable
    unsigned A : 1;                 // accessed
    unsigned unused2 : 1;
    unsigned PS : 1;                // page size (1 if 4MB)
    unsigned unused : 4;
    unsigned phys_addr : 20;        // physical address of page table
}__attribute__((packed));

enum frame_status {
    FRAME_USED = 0,
    FRAME_FREE = 1
};

void vmm_init();

enum frame_status get_frame_status(uint32_t i);


void page_alloc(uint32_t virt);
void page_dealloc(uint32_t virt);

void page_bind(uint32_t virt, uint32_t phys);

struct page_table_entry* get_pte(uint32_t pt, uint32_t i);
struct page_table_entry* get_pte_virt(uintptr_t virt);

struct page_directory_entry* get_pde(uint32_t pdi);
struct page_directory_entry* get_pde_virt(uintptr_t virt);

uintptr_t get_physical_address(uintptr_t virt);
