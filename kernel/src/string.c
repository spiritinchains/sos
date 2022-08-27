
#include <kernel.h>

void* kmemcpy(void* dest, const void* src, size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        ((uint8_t*)dest)[i] = ((uint8_t*)src)[i];
    }
    return dest;
}
