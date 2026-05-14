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
 * Inverse of configure_soc_clocks: clears bits 0-10 of SOC_CONFIG
 * (bit 9 preserved, bits ≥11 preserved), no new bits set back.
 * Called by main() before final return to put the clock-mux into a
 * known-quiet state on the success path. 10-NOP settle as before.
 */
void reset_soc_clocks(void)
{
    volatile u32 *crg = (volatile u32 *)CRG_START;
    u32 v;
    int settle;

    v = crg[SOC_CONFIG / 4];
    v &= ~(0x5f0u | 0xfu);
    crg[SOC_CONFIG / 4] = v;

    for (settle = 0; settle < 10; settle++)
        asm volatile("nop");
}

/*
 * Forward declarations for callees of main() — defined as link-only
 * stubs in stubs.c (separate TU so -O2 can't fold their constant
 * returns and erase main()'s real conditional structure).
 */
extern void disable_instruction_cache(void);   /* defined in start.S */
extern int  initial_memory_setup(void);        /* 0x4001128 */
extern void timer_start(void);                 /* 0x40051bc */
extern void timer_stop(void);                  /* 0x40051e8 */
extern void timer_reset_counter(void);         /* 0x40051f8 */
extern int  fastboot_update(void);             /* 0x4002368 */
extern int  media_update(void);                /* 0x40023c4 */
extern int  is_update_mode(void);              /* 0x400105c */
extern int  config_use_flash(void);            /* 0x400249c */
extern void timer_wait_seconds(unsigned n);    /* 0x4002cb4 */
extern int  bootrom_secure_check(void);        /* 0x4002284 — sig/secure dispatch, name TBD */
extern int  bootrom_alt_media(void);           /* 0x40025d4 — alt-boot dispatch, name TBD */

/*
 * Bootloader main flow at 0x04002704.
 *
 * Dispatches one of four boot paths based on two SYSCTRL state bits
 * read once at entry, plus an external "update mode" signal:
 *
 *   PERISTAT bit 0 set  → path_a (bootrom_secure_check)
 *   SYSSTAT  bit 5 set  → try fastboot_update first
 *   SYSSTAT  bit 4 set  → "boot_alt": skip pinmux, take alt media path
 *   else                → flash path (config_use_flash + media_update)
 *
 * is_update_mode() interposes between fastboot and flash/alt: if true,
 * media_update is tried directly before falling through to the
 * boot_alt-selected path.
 *
 * All paths converge on:
 *   success    → disable_instruction_cache, maybe reset_soc_clocks
 *                (only if *(u32*)(SDRAM+0x188) == 0), timer_stop,
 *                return 0
 *   erase/reset → zero SDRAM+0x500..+0x6500, wait 500 units,
 *                 timer_stop, write SC_SYSRES=1 to soft-reset.
 *                 Returns -1 in the fall-through (unreachable after
 *                 the reset takes effect).
 *
 * SYSSTAT bit 4/5 and PERISTAT bit 0 semantics ("boot_alt", "fastboot
 * hint", "secure-mode flag") are inferred from how main routes on
 * them, not yet cross-referenced against the av300 vendor user-guide.
 *
 * Pinmux IOCFG1[0..5] are programmed in the default-boot case with
 * vendor-specific values (0x6c1 / 0x4f1 × 4 / 0x5f1) — almost
 * certainly UART0 plus a couple of GPIO function selects for the
 * recovery console. Bit fields decode awaits docs.
 */
int main(void)
{
    volatile u32 *sysctrl = (volatile u32 *)SYSCTRL_START;
    volatile u32 *iocfg1  = (volatile u32 *)IO_CTRL0_START;
    u32 sysstat;
    u32 boot_alt;

    initial_memory_setup();
    timer_start();
    timer_reset_counter();

    sysstat = sysctrl[SYSSTAT / 4];
    boot_alt = sysstat & 0x10;

    if (!boot_alt) {
        iocfg1[0] = 0x6c1;
        iocfg1[1] = 0x4f1;
        iocfg1[2] = 0x4f1;
        iocfg1[3] = 0x4f1;
        iocfg1[4] = 0x5f1;
        iocfg1[5] = 0x4f1;
    }

    if (sysctrl[PERISTAT / 4] & 1) {
        /* path_a */
        if (bootrom_secure_check() != 0)
            goto erase_and_reset;
        goto success;
    }

    if (sysstat & 0x20) {
        if (fastboot_update() == 0)
            goto success;
    }

    if (is_update_mode()) {
        if (media_update() == 0)
            goto success;
    }

    if (boot_alt) {
        /* path_c — alternate media */
        if (bootrom_alt_media() == 0)
            goto success;
        if (media_update() == 0)
            goto success;
    } else {
        /* path_b — flash */
        if (config_use_flash() == 0)
            goto success;
        if (media_update() == 0)
            goto success;
    }

erase_and_reset:
    memset((void *)(SDRAM_START + 0x500), 0, 0x6000);
    timer_wait_seconds(500);
    timer_stop();
    sysctrl[SC_SYSRES / 4] = 1;
    return -1;

success:
    disable_instruction_cache();
    if (*(volatile u32 *)(SDRAM_START + 0x188) == 0)
        reset_soc_clocks();
    timer_stop();
    return 0;
}
