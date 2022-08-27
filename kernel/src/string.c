
#include <kernel.h>

void* kmemcpy(void* dest, const void* src, size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        ((uint8_t*)dest)[i] = ((uint8_t*)src)[i];
    }
    return dest;
}

size_t kstrlen(const char* str)
{
    size_t l = 0;
    while (str[l]) l++;
    return l;
}

int kstrncmp(const char* s1, const char* s2, size_t num)
{
    size_t i = 0;
    int r = 0;
    do {
        r = s1[i] - s2[i];
        i++;
    } while (!r && i < num);
    return r;
}

int kstrcmp(const char* s1, const char* s2)
{
    return kstrncmp(s1, s2, kstrlen(s1));
}
