#include "hi3516av300-regmap.h"

typedef unsigned char  u8;
typedef unsigned int   u32;
typedef unsigned int   size_t;

void *memset(void *dst, int val, size_t n)
{
    u8 *p = (u8 *)dst;
    u8 c = (u8)val;
    while (n--)
        *p++ = c;
    return dst;
}

void *memcpy(void *dst, const void *src, size_t n)
{
    u8 *d = (u8 *)dst;
    const u8 *s = (const u8 *)src;
    while (n--)
        *d++ = *s++;
    return dst;
}

size_t strlen(const char *s)
{
    const char *p = s;
    while (*p)
        p++;
    return (size_t)(p - s);
}

int strncmp(const char *a, const char *b, size_t n)
{
    while (n--) {
        unsigned char ca = (unsigned char)*a++;
        unsigned char cb = (unsigned char)*b++;
        if (ca != cb)
            return (int)ca - (int)cb;
        if (ca == 0)
            return 0;
    }
    return 0;
}

int strcmp(const char *a, const char *b)
{
    while (*a && *a == *b) {
        a++;
        b++;
    }
    return (int)(unsigned char)*a - (int)(unsigned char)*b;
}

/*
 * Stubs for callees of _start that live elsewhere in the mask-ROM and
 * are not yet reverse-engineered. The compiled re.elf is a parser/
 * link gate, not a flashable image — these stubs exist solely so the
 * cross-references in start.S resolve.
 */

void configure_soc_clocks(void) { }

int main(void)
{
    /* Real bootrom main loads the next-stage image into SDRAM at
     * SDRAM_START + offset and stores its entry point at *(u32*)
     * SDRAM_START. Reversed in a follow-up PR. */
    *(volatile u32 *)SDRAM_START = 0;
    return 0;
}
