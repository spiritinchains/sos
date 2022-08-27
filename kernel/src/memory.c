
#include <kernel.h>

#include "vmm.h"
#define KMEM_MAGIC_NUMBER 0x426187a862714697

// kmem_free_header and kmem_used_header must be of same size

struct kmem_free_header {
    size_t len;
    struct kmem_free_header* next;
    struct kmem_free_header* prev;
};

struct kmem_used_header {
    size_t len;
    uint64_t magic;
};

struct kmem_free_header* kmem_root;

void kmem_init(uintptr_t start, uintptr_t end)
{
    kmem_root = (struct kmem_free_header*) start;
    kmem_root->prev = NULL;
    kmem_root->next = NULL;
    kmem_root->len = (end - start) - sizeof(struct kmem_free_header);
}

void* kmalloc(size_t size)
{
    struct kmem_free_header* cur = kmem_root;
    /**
     * if size <= free block size + header size, split node
     * if size is equal to free block size, don't split
     * else, move to next free node
     * if all else fails, return NULL for now
     */
    while (cur)
    {
        if (size <= (cur->len + sizeof(struct kmem_free_header)))
        {
            struct kmem_free_header* split;
            split = cur + sizeof(struct kmem_free_header) + size;
            split->prev = (cur->prev == NULL) ? NULL : cur->prev;
            split->next = (cur->next == NULL) ? NULL : cur->next;
            split->len = cur->len - (split - cur) - sizeof(struct kmem_free_header);

            struct kmem_used_header* alloc = (struct kmem_used_header*) cur;
            alloc->magic = KMEM_MAGIC_NUMBER;
            alloc->len = size;
            return (alloc + sizeof(struct kmem_used_header));
        }
        else if (size == cur->len)
        {
            struct kmem_free_header *p, *n;
            p = cur->prev;
            n = cur->next;
            if (p) { p->next = n; }
            if (n) { n->prev = p; }
            size_t l = cur->len;
            struct kmem_used_header* alloc;
            alloc = (struct kmem_used_header*) cur;
            alloc->magic = KMEM_MAGIC_NUMBER;
            alloc->len = l;
            return (alloc + sizeof(struct kmem_used_header));
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
    struct kmem_used_header* m = mem - sizeof(struct kmem_used_header);
    struct kmem_free_header* cur = kmem_root;

    if (m->magic != KMEM_MAGIC_NUMBER)
    {
        return;
    }

    /**
     * if m > cur
     *      if cur->next, NULL, add to end of free list
     *      else next node
     * if m < cur, link m to free list in middle
     */
    while (cur)
    {
        if (m > cur)
        {
            if (cur->next == NULL)
            {
                size_t l = m->len;
                cur->next = m;
                cur->next->len = l;
                cur->next->prev = cur;
                cur->next->next = NULL;
            }
            else
            {
                cur = cur->next;
            }
        }
        else if (m < cur)
        {
            struct kmem_free_header* m_free = (struct kmem_free_header*)m;
            size_t l = m->len;
            m_free->len = l;
            cur->prev->next = m_free;
            m_free->prev = cur->prev;
            cur->prev = m_free;
            m_free->next = cur;
        }
    }
    // coalesce free blocks
    cur = kmem_root;
    while (cur)
    {
        if (cur + cur->len + sizeof(struct kmem_free_header) == cur->next)
        {
            struct kmem_free_header* d_block = cur->next;
            cur->next = d_block->next;
            d_block->next = NULL;
            d_block->prev = NULL;
            cur->len += d_block->len + sizeof(struct kmem_free_header);
            d_block->len = 0;
        }
        else
        {
            cur = cur->next;
        }
    }
}
