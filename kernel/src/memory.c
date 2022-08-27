
#include <kernel.h>

#include "vmm.h"
#define KMEM_MAGIC_NUMBER 0x426187a862714697

struct kmem_free_header {
    size_t len;
    struct kmem_free_header* next;
    struct kmem_free_header* prev;
};

struct kmem_used_header {
    size_t len;
    uint64_t magic;
};

#define HEAD_USED sizeof(struct kmem_used_header)
#define HEAD_FREE sizeof(struct kmem_free_header)

struct kmem_free_header* kmem_root;

void kmem_init(uintptr_t start, uintptr_t end)
{
    kmem_root = (struct kmem_free_header*) start;
    kmem_root->prev = NULL;
    kmem_root->next = NULL;
    kmem_root->len = (end - start) - HEAD_FREE;
}

void* kmalloc(size_t size)
{
    if (!kmem_root || !size)
        return NULL;

    struct kmem_free_header* cur = kmem_root;
    // 4 byte padding
    size += 4;

    /**
     * if size + used header + free header < length + free header, split node
     * if size + used header <= length + free header, convert node
     * else, move to next free node
     * if all else fails, return NULL for now
     */
    while (cur)
    {
        if ((size + HEAD_USED) < cur->len)
        {
            // split free block with remaining size
            struct kmem_free_header* split;
            split = (char*)cur + size + HEAD_USED;
            split->prev = cur->prev;
            split->next = cur->next;
            if (cur->prev) cur->prev->next = split;
            if (cur->next) cur->next->prev = split;
            split->len = cur->len - size - HEAD_USED;

            if (cur == kmem_root)
                kmem_root = split;

            // turn free block into allocated block
            struct kmem_used_header* alloc = cur;
            alloc->magic = KMEM_MAGIC_NUMBER;
            alloc->len = size;
            return ((char*)alloc + HEAD_USED);
        }
        else if (size + HEAD_USED <= cur->len + HEAD_FREE)
        {
            // directly turn free block into allocated block
            struct kmem_free_header *p, *n;
            p = cur->prev;
            n = cur->next;
            if (p) p->next = n;
            if (n) n->prev = p;

            if (cur == kmem_root)
                kmem_root = n;

            size += (cur->len + HEAD_FREE) - (size + HEAD_USED);

            struct kmem_used_header* alloc = cur;
            alloc->magic = KMEM_MAGIC_NUMBER;
            alloc->len = size;
            return ((char*)alloc + HEAD_USED);
        }
        else if (cur->next)
        {
            cur = cur->next;
        }
        else
        {
            // no valid blocks were found
            // TODO: dynamically adjust heap size using VMM?
            return NULL;
        }
    }
    return NULL;
}

void kfree(void* mem)
{
    struct kmem_used_header* mem_block = (char*)mem - HEAD_USED;
    struct kmem_free_header* cur = kmem_root;

    if (mem_block->magic != KMEM_MAGIC_NUMBER)
    {
        return;
    }

    /**
     * if mem_block > cur
     *      if cur->next, NULL, add to end of free list
     *      else next node
     * if mem_block < cur, link mem_block to free list in middle
     */
    while (cur)
    {
        if (mem_block > cur)
        {
            if (cur->next == NULL)
            {
                size_t l = (mem_block->len + HEAD_USED) - HEAD_FREE;
                cur->next = mem_block;
                cur->next->len = l;
                cur->next->prev = cur;
                cur->next->next = NULL;
            }
            else
            {
                cur = cur->next;
            }
        }
        else if (mem_block < cur)
        {
            struct kmem_free_header* mem_free = mem_block;
            size_t l = (mem_block->len + HEAD_USED) - HEAD_FREE;
            mem_free->len = l;
            if (cur->prev) cur->prev->next = mem_free;
            mem_free->prev = cur->prev;
            cur->prev = mem_free;
            mem_free->next = cur;

            if (cur == kmem_root)
                kmem_root = mem_free;
            
            break;
        }
    }
    // coalesce free blocks
    cur = kmem_root;
    while (cur)
    {
        if ((char*)cur + cur->len + HEAD_FREE == cur->next)
        {
            struct kmem_free_header* d_block = cur->next;
            cur->next = d_block->next;
            d_block->next = NULL;
            d_block->prev = NULL;
            cur->len += d_block->len + HEAD_FREE;
            d_block->len = 0;
        }
        else
        {
            cur = cur->next;
        }
    }
}
