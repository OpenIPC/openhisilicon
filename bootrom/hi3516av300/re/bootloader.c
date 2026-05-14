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

extern void enable_instruction_cache(void);    /* defined in start.S */
extern void disable_instruction_cache(void);   /* defined in start.S */

/*
 * Functions defined elsewhere or below — minimal forward decls so the
 * 11 reverse-engineered bodies below can reference each other in any
 * order. Functions still living as link-only stubs are in stubs.c.
 */
extern int  initial_memory_setup(void);        /* 0x4001128 — below */
extern void timer_start(void);                 /* 0x40051bc — below */
extern void timer_stop(void);                  /* 0x40051e8 — below */
extern void timer_reset_counter(void);         /* 0x40051f8 — below */
extern void timer_wait_seconds(unsigned n);    /* 0x4002cb4 — below */
extern int  is_update_mode(void);              /* 0x400105c — below */
extern int  fastboot_update(void);             /* 0x4002368 — below */
extern int  media_update(void);                /* 0x40023c4 — below */
extern int  config_use_flash(void);            /* 0x400249c — below */
extern int  bootrom_secure_check(void);        /* 0x4002284 — below */
extern int  bootrom_alt_media(void);           /* 0x40025d4 — below */

/* Transitive callees not yet reverse-engineered — stubbed in stubs.c. */
extern void wait_timer_0(unsigned ticks);          /* 0x40052b4 */
extern void organize_memory_chunks(void);          /* 0x4002a80 */
extern int  burn_bootloader(void);                 /* 0x40034ac */
extern int  set_boot_target_addr(int boot_pin);    /* 0x40004b8 */
extern void reset_emmc(void);                      /* 0x4004060 */
extern void media_init_a(void);                    /* 0x400414c */
extern void media_init_b(void);                    /* 0x40041c0 */
extern int  media_init_c(void);                    /* 0x40041f0 */
extern int  media_program_a(void);                 /* 0x40046c8 */
extern int  media_finalize_a(void);                /* 0x4005384 */
extern void close_sdio0(void);                     /* 0x400445c */
extern int  media_init_alt(void);                  /* 0x4005670 */
extern void media_init_alt_finalize(void);         /* 0x40056c0 */
extern int  parse_image_header(void *dst);         /* 0x40001dc */
extern void invoke_foreign_code(void *fn);         /* 0x4005348 */
extern int  initialize_emmc(void);                 /* 0x4003de0 */
extern void memcpy_emmc(void *dst, unsigned off, unsigned len); /* 0x40040a0 */
extern void initialize_uart0_hdwr(void);           /* 0x4003318 */
extern void disable_uart0_hdwr(void);              /* 0x4003334 */
extern void uart0_flush_rx_fifo(void);             /* 0x4003338 */
extern int  klad_load_keys(void);                  /* 0x4001470 */
extern int  klad_verify(void);                     /* 0x4001f0c */
extern int  klad_unlock(void);                     /* 0x4001af4 */
extern int  klad_alt_verify(void);                 /* 0x4001bdc */

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

/*
 * Timer subsystem — TIMER0 (PL031 PrimeCell at 0x12000000).
 * Used as the bootrom's free-running cycle counter and the source
 * for timer_wait_seconds + the load-path delays.
 */

void timer_stop(void)  /* 0x40051e8 */
{
    *(volatile u32 *)(TIMER0_START + TIMER_CONTROL) = 0;
}

void timer_start(void)  /* 0x40051bc */
{
    volatile u32 *sysctrl = (volatile u32 *)SYSCTRL_START;
    volatile u32 *timer0  = (volatile u32 *)TIMER0_START;
    u32 ctrl;

    ctrl = sysctrl[SC_CTRL / 4];
    ctrl &= ~0x10000u;             /* clear bit 16 — ungate timer clock */
    sysctrl[SC_CTRL / 4] = ctrl;
    timer0[TIMER_LOAD / 4] = 0;
    timer0[TIMER_CONTROL / 4] = 0xc2;   /* enable + periodic + 32-bit (vendor) */
}

void timer_reset_counter(void)  /* 0x40051f8 */
{
    volatile u32 *timer0 = (volatile u32 *)TIMER0_START;
    volatile u32 *state  = (volatile u32 *)(SDRAM_START + SRAM_TIMER0_VALUE_OFF);
    u32 ctrl;

    ctrl = timer0[TIMER_CONTROL / 4];
    timer0[TIMER_CONTROL / 4] = ctrl & ~0x80u;   /* disable */
    timer0[TIMER_LOAD / 4] = 0xFFFFFFFFu;         /* counts down from max */
    timer0[TIMER_CONTROL / 4] = ctrl | 0x80u;    /* re-enable */
    state[1] = 0;                                 /* delta */
    state[0] = timer0[TIMER_VALUE / 4];           /* snapshot */
}

void timer_wait_seconds(unsigned n)  /* 0x4002cb4 */
{
    if (n == 0)
        return;
    do {
        wait_timer_0(1000);
    } while (--n);
}

/*
 * Polls the recovery button on GPIO0 pin 0. Must read LOW for ~20s
 * consecutively (two timer_wait_seconds(10) gaps) to register as
 * "update requested". The first non-zero sample exits with 0.
 * Returns 1 only if all three samples are LOW.
 */
int is_update_mode(void)  /* 0x400105c */
{
    volatile u32 *gpio0 = (volatile u32 *)GPIO0_START;
    u32 sample;

    gpio0[GPIO_DIR / 4] = 0;
    sample = *(volatile u32 *)(GPIO0_START + GPIO_DATA_BIT(0));
    if (sample != 0)
        return 0;

    timer_wait_seconds(10);
    sample = *(volatile u32 *)(GPIO0_START + GPIO_DATA_BIT(0));
    if (sample != 0)
        return 0;

    timer_wait_seconds(10);
    sample = *(volatile u32 *)(GPIO0_START + GPIO_DATA_BIT(0));
    return sample == 0;
}

/*
 * Earliest call from main(). Enables the I-cache, then copies a
 * 12-byte init record from bootrom 0x7f44 to SRAM 0x04010004, then
 * tail-calls organize_memory_chunks() to set up the chunk allocator
 * the rest of the bootloader uses for its working buffers.
 */
int initial_memory_setup(void)  /* 0x4001128 */
{
    enable_instruction_cache();
    memcpy((void *)(SDRAM_START + SRAM_INIT_RECORD_OFF),
           (const void *)BOOTROM_INIT_REC_SRC, 12);
    organize_memory_chunks();
    return 0;
}

/*
 * UART fastboot loader. Returns 0 on success, -1 on burn_bootloader
 * failure or set_boot_target_addr failure. When booting from eMMC
 * (SYSSTAT bit 4 == 1) and set_boot_target_addr reports success
 * non-zero, also reset the eMMC subsystem before returning.
 */
int fastboot_update(void)  /* 0x4002368 */
{
    volatile u32 *sysctrl = (volatile u32 *)SYSCTRL_START;
    int rc;
    u32 boot_pin;

    if (burn_bootloader() != 0)
        return -1;

    boot_pin = (sysctrl[SYSSTAT / 4] >> 4) & 1;
    rc = set_boot_target_addr(boot_pin);
    if (rc == 0 || boot_pin != 1)
        return rc;

    reset_emmc();
    return rc;
}

/*
 * Media (eMMC/SDIO) image loader. Dispatches on PERISTAT bits 4 + 5:
 *
 *   bit4=0           → fastboot media path (init_a/b/c, program_a,
 *                       finalize_a, write "ziju" magic into
 *                       SYSCTRL[0x140] on success)
 *   bit4=1, bit5=1   → return -1
 *   bit4=1, bit5=0   → alt path: init_alt, write 2 into SYSCTRL[0x154],
 *                      init_alt_finalize, then set_boot_target_addr
 *                      with the SYSSTAT-bit4 boot_pin selector
 *
 * Success returns 0; any path failure returns -1.
 */
int media_update(void)  /* 0x40023c4 */
{
    volatile u32 *sysctrl = (volatile u32 *)SYSCTRL_START;
    u32 peristat = sysctrl[PERISTAT / 4];
    int rc;
    u32 boot_pin;

    if (peristat & 0x10) {
        if (peristat & 0x20)
            return -1;
        /* alt media path */
        if (media_init_alt() == 0)
            return -1;
        sysctrl[0x154 / 4] = 2;
        media_init_alt_finalize();
        boot_pin = (sysctrl[SYSSTAT / 4] >> 4) & 1;
        rc = set_boot_target_addr(boot_pin);
        if (rc == 0)
            return 0;
        if (boot_pin != 1)
            return -1;
        reset_emmc();
        return rc;
    }

    /* fastboot media path */
    media_init_a();
    media_init_b();
    if (media_init_c() != 0)
        return -1;
    if (media_program_a() == 0)
        return -1;
    sysctrl[0x140 / 4] = MEDIA_OK_MAGIC;
    rc = media_finalize_a();
    close_sdio0();
    if (rc != 0) {
        sysctrl[0x140 / 4] = 0xFFFFFFFFu;
        return -1;
    }
    return 0;
}

/*
 * Flash image loader. Copies the first 0x6000 bytes from FLASH_START
 * (0x14000000) into SRAM load buffer (0x04010500), parses the image
 * header. If the magic ("MBSB") matches: walk the header's segment
 * descriptor table at stack offsets {+4, +8, +12, +24} to compute the
 * second-stage source/length, memcpy that into DDR at 0x81000000, then
 * cross-check four control words.
 *
 * The four-word check at the end is the same pattern used by
 * bootrom_alt_media's verification — see comment there.
 */
int config_use_flash(void)  /* 0x400249c */
{
    volatile u32 *sysctrl = (volatile u32 *)SYSCTRL_START;
    volatile u32 *load_target_slot = (volatile u32 *)SDRAM_START;
    u32 header[12];                       /* 48-byte local frame */
    u32 *target;
    u32 *flash = (u32 *)FLASH_START;
    u32 r0, r1, r2, r3;
    int rc;

    memcpy((void *)(SDRAM_START + SRAM_LOAD_TARGET_OFF), flash, 0x6000);

    rc = parse_image_header(header);
    if (rc != 0 || header[0] != IMAGE_HEADER_MAGIC) {
        /* Verification failure path — compare four control cells
         * at IO_CTRL0+0 vs slot offsets 0x14000{004,008,00c}. */
        load_target_slot[0] = FLASH_START;
        r0 = *(volatile u32 *)FLASH_START;
        r3 = *(volatile u32 *)(FLASH_START + 0x4);
        r2 = *(volatile u32 *)(FLASH_START + 0x8);
        r1 = *(volatile u32 *)(FLASH_START + 0xc);
        rc = (r3 == r2 && r0 == r3) ? 1 : 0;
        if (r2 != r1)
            r0 = 0;
        else
            r0 = rc & 1;
        return -(int)r0;
    }

    /* Header parse succeeded; relocate the second-stage image. */
    {
        u32 seg_size    = header[2];
        u32 seg_off     = header[3];
        u32 seg_unknown = header[6];
        u32 src_offset  = (seg_size << 1) + 0x28 + seg_off;
        u32 dst_addr    = (DDR_START + 0x01000000);

        target = (u32 *)(FLASH_START + src_offset);
        load_target_slot[0] = (u32)target + seg_unknown;

        memcpy((void *)(SDRAM_START + SRAM_LOAD_TARGET_OFF), target, 0x6000);
        sysctrl[0x140 / 4] = MEDIA_OK_MAGIC;
        invoke_foreign_code((void *)(SDRAM_START + SRAM_LOAD_TARGET_OFF));
        sysctrl[0x140 / 4] = (u32)rc;                /* clear */

        memcpy((void *)dst_addr, (void *)load_target_slot[0], 0x80000);
        load_target_slot[0] = dst_addr;
    }

    /* Re-run the four-word check against the relocated copy. */
    r0 = *(volatile u32 *)(DDR_START + 0x01000000);
    r3 = *(volatile u32 *)(DDR_START + 0x01000004);
    r2 = *(volatile u32 *)(DDR_START + 0x01000008);
    r1 = *(volatile u32 *)(DDR_START + 0x0100000c);
    rc = (r3 == r2 && r0 == r3) ? 1 : 0;
    if (r2 != r1)
        r0 = 0;
    else
        r0 = rc & 1;
    return -(int)r0;
}

/*
 * Alternate-media (eMMC raw) image loader. Initializes eMMC, copies
 * the first 0x6000 bytes from eMMC into the SRAM load buffer, parses
 * the same "MBSB" header, then on success relocates a 0x6000 chunk
 * into DDR at 0x81000000 and validates with the same four-word
 * cross-check used by config_use_flash. Failure path resets eMMC and
 * returns -1.
 */
int bootrom_alt_media(void)  /* 0x40025d4 */
{
    volatile u32 *load_target_slot = (volatile u32 *)SDRAM_START;
    u32 header[13];   /* 52-byte local frame */
    u32 r0, r1, r2, r3;
    int rc;

    initialize_emmc();
    memcpy_emmc((void *)(SDRAM_START + SRAM_LOAD_TARGET_OFF), 0, 0x6000);

    rc = parse_image_header(header);
    if (rc != 0 || header[0] != IMAGE_HEADER_MAGIC)
        goto verify;

    {
        u32 seg_off  = header[3];
        u32 seg_more = header[2];
        u32 *control = (u32 *)(SDRAM_START + SRAM_LOAD_TARGET_OFF + 0x024);

        invoke_foreign_code(control);
        memcpy_emmc((void *)(DDR_START + 0x01000000), seg_off, 0x6000);
        memcpy_emmc((void *)(DDR_START + 0x01006000),
                    header[1] - 0x6000,
                    (u32)(0x81000000 - header[1] + header[2] /* paranoia */));
        load_target_slot[0] = DDR_START + 0x01000000;
        (void)seg_more;
    }

verify:
    r0 = *(volatile u32 *)(SDRAM_START + SRAM_LOAD_TARGET_OFF);
    r3 = *(volatile u32 *)(SDRAM_START + SRAM_LOAD_TARGET_OFF + 0x4);
    r2 = *(volatile u32 *)(SDRAM_START + SRAM_LOAD_TARGET_OFF + 0x8);
    r1 = *(volatile u32 *)(SDRAM_START + SRAM_LOAD_TARGET_OFF + 0xc);
    rc = (r3 == r2 && r0 == r3) ? 1 : 0;
    if (r2 != r1)
        r0 = 0;
    else
        r0 = rc & 1;
    if (r0 != 0) {
        reset_emmc();
        return -1;
    }
    return 0;
}

/*
 * Secure-boot dispatch entered from main() when PERISTAT bit 0 is set.
 *
 * Sequence:
 *   1. Tweak CRG.PERI_CRG104 (0x1a0) to bring KLAD/RSA/TRNG/SPACC out
 *      of reset and ungate their clocks (bits 5/3/1 set, bits 8 clear).
 *   2. Kick the TRNG (write 10 to TRNG+0x200).
 *   3. Read SYSSTAT once. Bit 5 → call klad_load_keys; on success skip
 *      to cleanup. Bit 4 → call klad_verify; map its return into the
 *      final result via klad_unlock. Else → klad_alt_verify.
 *   4. Cleanup: disable UART0 (via 0x4003334), gate KLAD/etc back off.
 *
 * Function names for the four KLAD entry points are placeholders;
 * their actual signatures await further RE.
 */
int bootrom_secure_check(void)  /* 0x4002284 */
{
    volatile u32 *crg     = (volatile u32 *)CRG_START;
    volatile u32 *sysctrl = (volatile u32 *)SYSCTRL_START;
    volatile u32 *trng    = (volatile u32 *)TRNG_START;
    u32 sysstat;
    int result;
    u32 v;

    /* Bring security block out of reset, ungate clocks */
    v = crg[PERI_CRG104 / 4];
    v &= ~0x100u;
    v |=  0x200u;
    crg[PERI_CRG104 / 4] = v;

    v = crg[PERI_CRG104 / 4];
    v &= ~0x15u;
    v |=  0x2au;
    crg[PERI_CRG104 / 4] = v;

    trng[0x200 / 4] = 10;             /* kick TRNG */

    sysstat = sysctrl[SYSSTAT / 4];

    if (sysstat & 0x20) {
        if (klad_load_keys() == 0) {
            result = 0;
            goto cleanup;
        }
    }

    initialize_uart0_hdwr();
    uart0_flush_rx_fifo();

    /* While the recovery button is held, klad_unlock can decide the
     * secure check is done (returns 0). Otherwise we re-poll the
     * button and try again. */
    while (is_update_mode()) {
        if (klad_unlock() == 0) {
            result = 0;
            goto cleanup;
        }
    }

    {
        int sub = (sysstat & 0x10) ? klad_verify() : klad_alt_verify();
        if (sub == 0)
            result = 0;
        else if (sub == -2)
            result = klad_unlock();
        else
            result = sub;
    }

cleanup:
    disable_uart0_hdwr();

    v = crg[PERI_CRG104 / 4];
    v &= ~0x200u;
    crg[PERI_CRG104 / 4] = v;

    v = crg[PERI_CRG104 / 4];
    v &= ~0x2au;
    crg[PERI_CRG104 / 4] = v;

    return result;
}
