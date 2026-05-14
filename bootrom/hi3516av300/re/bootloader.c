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
 * Wait for APLL+DPLL+EPLL to lock, then switch the SoC clock-mux to
 * the post-PLL frequencies. Empirically derived bit layout:
 *   PLL_LOCK_STAT bits 0,1,3 = lock indicators (mask 0xb).
 *   SOC_CONFIG bits 0-10 are clock-select fields (bit 9 preserved);
 *     written value 0x592 = bits 1,4,7,8,10. Semantic decoding of
 *     each subfield requires the av300 vendor "用户指南" — not yet
 *     cross-referenced. Bits >=11 are preserved.
 *
 * Both delay loops are timing-critical: the 6 NOPs between PLL_LOCK
 * polls space them out so the lock signal has time to propagate, and
 * the 10 NOPs after the clock switch let the new frequency settle
 * before _start touches anything else. Using asm volatile keeps gcc
 * from collapsing them under -O2.
 */
void configure_soc_clocks(void)
{
    volatile u32 *crg = (volatile u32 *)CRG_START;
    u32 v;
    int timeout = 0x1389;
    int settle;

    while (timeout--) {
        if ((crg[PLL_LOCK_STAT / 4] & 0xb) == 0xb)
            break;
        asm volatile("nop; nop; nop; nop; nop; nop");
    }

    v = crg[SOC_CONFIG / 4];
    v &= ~(0x5f0u | 0xfu);
    v |= 0x590u;
    v |= 0x2u;
    crg[SOC_CONFIG / 4] = v;

    for (settle = 0; settle < 10; settle++)
        asm volatile("nop");
}

/*
 * Stub for the bootloader main flow at 0x04002704. Reversed in a
 * later PR — exists here solely so start.S's `bl main` resolves.
 * Real bootrom main loads the next-stage image into SDRAM and writes
 * its entry point to *(u32 *)SDRAM_START, which the clear_remap thunk
 * reads after releasing the boot remap.
 */
int main(void)
{
    *(volatile u32 *)SDRAM_START = 0;
    return 0;
}
