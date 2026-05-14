#include "hi3516av300-regmap.h"

typedef unsigned char  u8;
typedef unsigned short u16;
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

/* Functions reverse-engineered in this file (below); forward-declared so
 * callers above the definitions still resolve. */
extern void wait_timer_0(unsigned ticks);
extern void organize_memory_chunks(void);
extern int  burn_bootloader(void);
extern int  set_boot_target_addr(int boot_pin);
extern void reset_emmc(void);
extern void media_init_a(void);
extern void media_init_b(void);
extern int  media_init_c(void);
extern int  media_program_a(void);
extern int  media_finalize_a(void);
extern void close_sdio0(void);
extern int  media_init_alt(void);
extern void media_init_alt_finalize(void);
extern int  parse_image_header(void *dst);
extern void invoke_foreign_code(void *fn);
extern void memcpy_emmc(void *dst, unsigned off, unsigned len);
extern void initialize_uart0_hdwr(void);
extern void disable_uart0_hdwr(void);
extern void uart0_flush_rx_fifo(void);
extern int  klad_unlock(void);

/* Forward declarations — these four are defined below in this file. */
extern int  klad_load_keys(void);                  /* 0x4001470 */
extern int  klad_verify(void);                     /* 0x4001f0c */
extern int  klad_alt_verify(void);                 /* 0x4001bdc */
extern int  initialize_emmc(void);                 /* 0x4003de0 */

/* Stubs in stubs.c — the new frontier after this PR. */
extern int  klad_check_a(void);                    /* 0x40018b8 */
extern int  klad_check_b(void);                    /* 0x40016ac */

extern void initialize_uart0(void);                /* 0x40050c0 */
extern void disable_uart0(void);                   /* 0x400512c */
extern int  uart0_read(void);                      /* 0x4005170 */
extern int  uart0_has_rx_data(void);               /* 0x40051a0 */
extern int  uart0_wait_start_frame(void);          /* 0x4003410 */
extern int  uart0_recv_payload(void);              /* 0x4003354 */
extern void uart0_proto_handshake(void);           /* 0x4003580 */
extern int  uart0_recv_signed_image(void *dst, unsigned size);  /* 0x40034e0 */
extern void uart0_send_status(int a, int b);      /* 0x4000320 */

extern void wait_long_timer_0(unsigned ms);        /* 0x4002c84 */
extern unsigned timer_get_value(void);             /* 0x4005234 */
extern unsigned get_wait_ticks(unsigned ms);       /* 0x4005244 */
extern void update_timer_0_value(void);            /* 0x400524c */

extern unsigned alloc_chunk(unsigned size);        /* 0x4002b78 */
extern void free_chunk(unsigned addr);             /* 0x4002c1c */
extern void nop_2d74(void);                        /* 0x4002d74 */

extern int  media_init_d(void);                    /* 0x400422c */
extern int  media_program_b(void *ctx);            /* 0x4004d8c */
extern int  media_finalize_b(void *dst, void *src, unsigned len);  /* 0x4004900 */
extern int  media_sub_a(void);                     /* 0x40077a8 */
extern unsigned media_sub_b(void);                 /* 0x40077b8 */
extern void media_sub_c(void);                     /* 0x400795c */

extern int  klad_validate_header(void *ctx);       /* 0x4001150 */
extern int  reset_secure_io_peripherals(unsigned slot, int boot_pin);  /* 0x40010ac */
extern int  klad_dispatch_sig(void *ctx, unsigned msg_size, unsigned msg_off, void *sig_buf, void *modulus_buf);  /* 0x4000868 */
extern int  klad_post_unlock(unsigned boot_type, int target_size_class, unsigned size);  /* 0x4000568 */
extern int  klad_finalize(void *ctx);              /* 0x400136c */
extern int  refresh_ddr(unsigned mctl_mode);       /* 0x4000358 */
extern void configure_emmc_pins(void);             /* 0x4003d68 */
extern int  send_command_emmc(void);               /* 0x4003c30 */
extern int  klad_verify_rsa(void *src, unsigned len);  /* 0x4005468 */
extern int  klad_alt_dispatch(void *ctx);          /* 0x4001df8 */
extern int  klad_alt_e88(int boot_pin, int err);   /* 0x4001e88 */

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

/* ===== UART0 hardware bring-up ===== */

void initialize_uart0_hdwr(void)  /* 0x4003318 */
{
    volatile u32 *iocfg1 = (volatile u32 *)IO_CTRL1_START;
    iocfg1[0x10 / 4] = 0x531;       /* UART0_RX pinmux */
    iocfg1[0x14 / 4] = 0x431;       /* UART0_TX pinmux */
    initialize_uart0();
}

void disable_uart0_hdwr(void)  /* 0x4003334 */
{
    disable_uart0();
}

void uart0_flush_rx_fifo(void)  /* 0x4003338 */
{
    while (uart0_has_rx_data())
        (void)uart0_read();
}

int burn_bootloader(void)  /* 0x40034ac */
{
    volatile u32 *sysctrl = (volatile u32 *)SYSCTRL_START;

    if (uart0_wait_start_frame() == 0)
        return -1;
    sysctrl[0x154 / 4] = 1;
    uart0_recv_payload();
    return 0;
}

/* ===== eMMC / SDIO0 management ===== */

void reset_emmc(void)  /* 0x4004060 */
{
    volatile u32 *emmc = (volatile u32 *)EMMC_START;
    emmc[MMC_CMD / 4] = 0x84000000u;          /* abort + use_hold_reg */
    emmc[MMC_CARD_RSTN / 4] = 0;              /* assert reset */
    wait_long_timer_0(100);
    emmc[MMC_CARD_RSTN / 4] = 1;              /* release */
    wait_long_timer_0(1000);
    emmc[MMC_CTRL / 4] = 7;                   /* reset DMA + FIFO + INT */
}

void close_sdio0(void)  /* 0x400445c */
{
    volatile u32 *sdio0 = (volatile u32 *)SDIO0_START;
    volatile u32 *chunk_slot = (volatile u32 *)(SDRAM_START + 0x164);

    if (chunk_slot[0] != 0) {
        free_chunk(chunk_slot[0]);
        chunk_slot[0] = 0;
    }
    sdio0[MMC_CTRL / 4]  = 7;
    sdio0[MMC_PWREN / 4] = 0;
}

void memcpy_emmc(void *dst, unsigned off, unsigned len)  /* 0x40040a0 */
{
    volatile u32 *emmc = (volatile u32 *)EMMC_START;
    u32 words = (len + 3) >> 2;
    u32 *out = (u32 *)dst;

    (void)off;   /* offset is consumed by the surrounding caller via separate
                    CMD setup — within this routine we just drain DATA */
    if (words == 0)
        return;

    while (words) {
        u32 stat;
        do {
            stat = emmc[MMC_RINTSTS / 4];
            emmc[MMC_RINTSTS / 4] = stat;
            if (stat & 0xba80u)
                return;
        } while ((stat & 0x28) == 0);

        u32 fifo_words = (emmc[MMC_STATUS / 4] >> 17) & 0x1fff;
        u32 take = words < fifo_words ? words : fifo_words;
        for (u32 i = 0; i < take; i++)
            *out++ = emmc[MMC_DATA / 4];
        words -= take;
        if (stat & 0x8)
            break;
    }
    (void)dst;
}

/* ===== Timer leaves (TIMER0 ticks @ ~3 MHz vendor-clock) ===== */

void wait_timer_0(unsigned ticks)  /* 0x40052b4 */
{
    /* For ticks < 1000 the function takes a fast path that scales by
     * a vendor-supplied factor (0x2dc6c0 / 1) — equivalent to "ticks
     * are already in target units". For >= 1000 it converts and adds
     * the snapshot delta. Either way the final wait loop spins on
     * timer_get_value() until it crosses the computed target. */
    unsigned scaled;
    unsigned target;
    unsigned start;

    if (ticks < 1000) {
        unsigned t1 = ticks * 0x2dc6c0u;
        unsigned long long m = (unsigned long long)t1 * 0x431b1de83ull;
        scaled = (unsigned)(m >> 50);
    } else {
        unsigned long long m1 = (unsigned long long)ticks * 0x10624dd3ull;
        unsigned milli = (unsigned)(m1 >> 38);
        unsigned t1 = milli * 0x2dc6c0u;
        unsigned long long m2 = (unsigned long long)t1 * 0x10624dd3ull;
        scaled = (unsigned)(m2 >> 38);
    }

    update_timer_0_value();
    start = scaled + 0;        /* placeholder for caller-saved r5 */
    target = start + 1;
    if (target < start) {
        volatile u32 *timer0 = (volatile u32 *)TIMER0_START;
        volatile u32 *state  = (volatile u32 *)(SDRAM_START + SRAM_TIMER0_VALUE_OFF);
        u32 cur = timer0[TIMER_VALUE / 4];
        state[1] = 0;
        state[0] = cur;
        start = scaled;
    }

    do {
        update_timer_0_value();
    } while (timer_get_value() < start);
}

/* ===== Foreign code trampoline ===== */

void invoke_foreign_code(void *fn)  /* 0x4005348 */
{
    /* Saves the all-callee-regs frame, stashes a return-fixup pointer
     * at *(SRAM_START+0x150), invokes fn, then sets the "stage-2
     * loaded" flag at SRAM[0x188] = 1 before returning. */
    volatile u32 *sysctrl_save_slot = (volatile u32 *)(SYSCTRL_START + 0x150);
    typedef void (*foreign_fn)(void);

    (void)sysctrl_save_slot;  /* the bootrom writes a pc-relative
                                  return-trampoline ptr here; harmless
                                  to omit in the freestanding compile */
    ((foreign_fn)fn)();
    *(volatile u32 *)(SDRAM_START + 0x188) = 1;
}

/* ===== Image header parser ===== */

int parse_image_header(void *dst)  /* 0x40001dc */
{
    volatile u32 *buf = (volatile u32 *)(SDRAM_START + SRAM_LOAD_TARGET_OFF);
    u32 *out = (u32 *)dst;
    u32 magic = buf[0];
    u32 size  = buf[1];
    u32 mode  = buf[2];
    u32 mode2 = buf[3];

    out[0] = magic;
    out[1] = size;
    if (size > 0x100000u)
        return -1;

    out[2] = mode;
    /* mode must be either == 0x200 or have bit 7 = 0 in the high
     * nibble check — original uses bics #0x80 + cmp #0x200 to permit
     * mode==0x200 or "(mode-0x80)&~0x80 == 0". */
    {
        u32 m1 = (mode - 0x80) & ~0x80u;
        int allowed = (m1 == 0) || (mode == 0x200);
        if (!allowed)
            return -1;
    }
    if (mode != mode2)
        return -1;
    out[3] = mode2;

    {
        u32 base = SDRAM_START + 0x510;
        u32 *desc = (u32 *)(SDRAM_START + 0x520);
        u32 mode_x2 = mode << 1;
        u32 entry;

        out[4] = base;
        out[5] = mode_x2 + (u32)desc;
        entry = *(volatile u32 *)((SDRAM_START + 0x520) + mode_x2);
        if (entry & 3)
            return -1;
        out[6] = entry;

        {
            u32 hdr_off = mode_x2 + (SDRAM_START + 0x524);
            u32 abs_addr = entry + hdr_off;
            u32 abs_minus = abs_addr - 0x6000u;
            out[7] = hdr_off;
            out[8] = abs_addr;
            if (abs_minus > 0x6000u)
                return -1;
            out[9] = mode;
            {
                u32 next = *(volatile u32 *)(abs_addr + mode);
                out[10] = next;
                if (size < next)
                    return -1;
                if (next & 3)
                    return -1;
                return 0;
            }
        }
    }
}

/* ===== Boot-target dispatcher ===== */

int set_boot_target_addr(int boot_pin)  /* 0x40004b8 */
{
    volatile u32 *load_target_slot = (volatile u32 *)SDRAM_START;
    u32 r0, r1, r2, r3;
    int hit;

    if (boot_pin == 1) {
        initialize_emmc();
        memcpy_emmc((void *)(SDRAM_START + SRAM_LOAD_TARGET_OFF), 0, 0x6000);
        load_target_slot[0] = SDRAM_START + SRAM_LOAD_TARGET_OFF;
        /* Fall through to the four-word cross-check below, reading
         * from SDRAM_START+0x500..0x50c (which we just populated). */
        r0 = *(volatile u32 *)(SDRAM_START + 0x500);
        r3 = *(volatile u32 *)(SDRAM_START + 0x504);
        r2 = *(volatile u32 *)(SDRAM_START + 0x508);
        r1 = *(volatile u32 *)(SDRAM_START + 0x50c);
    } else {
        load_target_slot[0] = FLASH_START;
        r0 = *(volatile u32 *)FLASH_START;
        r3 = *(volatile u32 *)(FLASH_START + 4);
        r2 = *(volatile u32 *)(FLASH_START + 8);
        r1 = *(volatile u32 *)(FLASH_START + 12);
    }

    hit = (r3 == r2 && r0 == r3);
    if (r2 != r1)
        return 0;
    return hit ? -1 : 0;
}

/* ===== Media bring-up sequence (fastboot path) ===== */

void media_init_a(void)  /* 0x400414c */
{
    volatile u32 *iocfg0  = (volatile u32 *)IO_CTRL0_START;
    volatile u32 *misc    = (volatile u32 *)MISC_START;
    u32 sysstat;
    u32 *slot;
    u32 pad_val;

    iocfg0[0x1c / 4] = 0x6f1;
    iocfg0[0x20 / 4] = 0x5f1;
    iocfg0[0x24 / 4] = 0x6c1;

    misc[0x4 / 4] = 5;

    sysstat = *(volatile u32 *)(SYSCTRL_START + 0x30);
    pad_val = (sysstat & 0x1000u) ? 0x4f1 : 0x5f1;

    /* Six consecutive IOCFG slots starting at IO_CTRL0+0x28. */
    for (slot = (u32 *)(IO_CTRL0_START + 0x28);
         slot != (u32 *)(IO_CTRL0_START + 0x3c);
         slot++)
        *slot = pad_val;
}

void media_init_b(void)  /* 0x40041c0 */
{
    volatile u32 *crg = (volatile u32 *)CRG_START;
    u32 v = crg[0x160 / 4];     /* SDIO0_CONFIG */
    crg[0x160 / 4] = v | 1;     /* assert reset */
    v &= ~0xdu;
    v |=  0xeu;
    wait_long_timer_0(1000);
    crg[0x160 / 4] = v;          /* deassert + new clock select */
}

int media_init_c(void)  /* 0x40041f0 */
{
    volatile u32 *sdio0 = (volatile u32 *)SDIO0_START;
    int r;

    if (sdio0[MMC_PWREN / 4] != 0)
        return -1;
    if (sdio0[MMC_PWREN / 4] != 0)
        return -1;
    r = (sdio0[MMC_PWREN / 4] != 0) ? 1 : 0;
    return -r;
}

int media_program_a(void)  /* 0x40046c8 */
{
    volatile u32 *chunk_slot = (volatile u32 *)(SDRAM_START + 0x168);
    unsigned ctx;
    int rc;

    media_init_d();
    ctx = alloc_chunk(28);
    chunk_slot[0] = ctx;
    if (ctx == 0) {
        close_sdio0();
        return 0;
    }

    memset((void *)ctx, 0, 28);

    {
        u8 *p = (u8 *)ctx;
        *(u32 *)(p + 0) = 0;
        p[4] = 0xff;
        *(u32 *)(p + 16) = 0x4680;
        *(u32 *)(p + 12) = 0x200;
        p[5] = 0;
        p[6] = 1;
    }

    rc = media_program_b((void *)ctx);
    if (rc == 3)
        return 1;
    close_sdio0();
    free_chunk(ctx);
    chunk_slot[0] = 0;
    return 0;
}

int media_finalize_a(void)  /* 0x4005384 */
{
    volatile u32 *sysctrl = (volatile u32 *)SYSCTRL_START;
    int rc;

    rc = media_finalize_b((void *)(SDRAM_START + SRAM_LOAD_TARGET_OFF),
                           (void *)0x00007fb0, 0x6000);
    if (rc <= 0)
        return -1;

    nop_2d74();
    invoke_foreign_code((void *)(SDRAM_START + SRAM_LOAD_TARGET_OFF));

    *(volatile u32 *)(SDRAM_START + 0x188) = 1;

    rc = media_finalize_b((void *)(DDR_START + 0x01000000),
                           (void *)0x00007fb0, 0x80000);
    if (rc <= 0)
        return -1;

    sysctrl[0x130 / 4] = 0x81000000u;
    sysctrl[0x148 / 4] = 0x444f574eu;   /* "DOWN" magic */
    nop_2d74();
    close_sdio0();
    invoke_foreign_code((void *)(DDR_START + 0x01000000));
    return 0;
}

int media_init_alt(void)  /* 0x4005670 */
{
    unsigned ticks = get_wait_ticks(3000);
    volatile u32 *flag = (volatile u32 *)(SDRAM_START + 0x18c);

    (void)media_sub_b();           /* harmless probe; result unused */
    timer_reset_counter();

    for (;;) {
        unsigned cur = timer_get_value();
        if (cur >= ticks)
            break;
        (void)media_sub_a();
        if (flag[0] == 1)
            return 0;              /* signaled OK */
    }
    media_sub_c();
    return 0;                       /* timed out — no error */
}

void media_init_alt_finalize(void)  /* 0x40056c0 */
{
    volatile u32 *flag     = (volatile u32 *)(SDRAM_START + 0x18c);
    volatile u32 *misc_150 = (volatile u32 *)(SYSCTRL_START + 0x150);
    u32 (*loaded)(void) = 0;
    u32 status;

    for (;;) {
        flag[0] = 0;
        do {
            media_sub_a();
            status = flag[0];
        } while (status == 0);

        if (status == 1) {
            loaded = (u32(*)(void))flag[1];
            continue;
        }
        if (status != 4)
            continue;

        loaded();
        if (*misc_150 == 0x434d454du) {   /* "CMEM" or similar */
            *misc_150 = 0;
            return;
        }
    }
}

/* ===== Memory chunk allocator setup ===== */

void organize_memory_chunks(void)  /* 0x4002a80 */
{
    /* Populates the chunk-allocator descriptor table at SRAM 0x04010010
     * from a packed (u16 size, u16 count) array at bootrom 0x7e5c.
     * Each descriptor in SRAM is {size, count, phy_addr, alloc_mask}.
     */
    const u16 *src;
    u32 *desc;
    u32 free_addr;

    memset((void *)(SDRAM_START + 0x10), 0, 0x100);

    src = (const u16 *)BOOTROM_INIT_REC_SRC;   /* 0x7f44 — alias for
                                                  the descriptor table
                                                  that lives just before
                                                  bootrom 0x7e5c */
    src = (const u16 *)0x00007e5c;
    desc = (u32 *)(SDRAM_START + 0x18);
    free_addr = SDRAM_START + 0x500;

    while (src[0] != 0) {
        u16 size  = src[0];
        u16 count = src[1];
        desc[-2] = free_addr;
        desc[-3] = 1;
        free_addr += (u32)count * size;
        desc[-4] = size;
        desc += 4;
        src += 2;
    }

    *(volatile u32 *)(SDRAM_START + 0x10) = 0x02000000u;

    /* Second pass: sort descriptors by base address (selection-sort). */
    {
        u32 *a = (u32 *)(SDRAM_START + 0x10);
        while (a[1] != 0) {
            u32 *b = a + 4;
            if (b[1] == 0)
                break;
            u32 *minp = a;
            while (b[1] != 0) {
                if (b[0] < minp[0])
                    minp = b;
                b += 4;
            }
            if (minp != a) {
                u32 t0 = minp[0], t1 = minp[1], t2 = minp[2];
                minp[0] = a[0];  minp[1] = a[1];  minp[2] = a[2];
                a[0] = t0;       a[1] = t1;       a[2] = t2;
            }
            a += 4;
        }
    }
}

/* ===== KLAD unlock (smallest of the four klad_* family) ===== */

int klad_unlock(void)  /* 0x4001af4 */
{
    volatile u32 *sysctrl = (volatile u32 *)SYSCTRL_START;
    u32 peristat = sysctrl[PERISTAT / 4];

    if (!(peristat & 0x10)) {
        if (klad_check_b() == 0)
            return 0;
    }
    /* Either peristat bit 4 set, or klad_check_b reported retry. */
    if (peristat & 0x20)
        return -1;
    return klad_check_a() != 0 ? -1 : 0;
}

/*
 * ============================================================================
 * The four giants — initialize_emmc, klad_load_keys, klad_verify, klad_alt_verify.
 *
 * These are structural reversals: the control-flow graph and identified
 * MMIO sequences match the original mask-ROM, but several deep helpers
 * (klad_validate_header, klad_dispatch_sig, send_command_emmc, etc.)
 * remain as link-only stubs in stubs.c. They become the next reversal
 * frontier. Names ending in _a/_b/_c or _sub are placeholders — the
 * actual responsibilities (which RSA mode, which OTP key slot, which
 * eMMC CMD index) await cross-reference with av300 vendor docs.
 * ============================================================================
 */

/*
 * initialize_emmc — bring up the eMMC host controller (Synopsys DW MMC
 * at EMMC_START=0x10100000).
 *
 * Phases:
 *   1. configure_emmc_pins (pinmux for CMD/CLK/DATA0..7).
 *   2. CRG[0x148] |= 1 (assert eMMC reset); wait 1ms.
 *   3. Inspect SYSCTRL.PERISTAT bits 9..8 (eMMC bus-width selector):
 *        0b01 → set SYSCTRL[0x14c]=1 and skip 4-bit setup
 *        0b10 → set SYSCTRL[0x14c]=1, OR clock mux bits, fall through
 *        else → leave CRG bits as configured for default mode
 *   4. CRG[0x148] |= 0xe (release reset + ungate variant clocks).
 *   5. EMMC_CTRL = 7 (reset DMA+FIFO+INT); poll until those three bits clear.
 *   6. Set MMC_RINTSTS=-1, MMC_TBBCNT (offset 0x8c) = -1, MMC_CTRL=16
 *      (PIO mode), then dispatch on PERISTAT bits 9..8 again to select
 *      either a 4-bit (CTYPE), 8-bit, or default-bus init sequence
 *      via three send_command_emmc rounds.
 *   7. CTYPE high byte 0xfff00 (drive strength?), CMD setup, CARD_RSTN
 *      reset cycle (0 → wait 100ms → 1 → wait 1000ms), BLKSIZ=0x200,
 *      BYTCNT=0x100000.
 *   8. UHS_REG_EXT clock-divider setup: mask out 0xff00000 + 0xf0000,
 *      OR in 0x7f0000 (clock div + phase), set CARDTHRCTL high.
 *   9. Mode-dependent CRG fixup for HS200/HS400-class buses.
 *   10. get_wait_ticks(1000) — compute 1-second timeout in ticks.
 *   11. timer_reset_counter; spin on RINTSTS bit 9 (CMD_DONE / DTO)
 *       up to timeout; on timeout return -1.
 *   12. Clear RINTSTS, return success (last good CMD index value).
 *
 * Returns 0 on success, -1 on timeout.
 */
int initialize_emmc(void)  /* 0x4003de0 */
{
    volatile u32 *emmc   = (volatile u32 *)EMMC_START;
    volatile u32 *crg    = (volatile u32 *)CRG_START;
    volatile u32 *sysctrl = (volatile u32 *)SYSCTRL_START;
    u32 crg148, peristat, mode_sel;
    u32 ctype_val;
    unsigned ticks;
    int rc;

    configure_emmc_pins();

    crg148 = crg[0x148 / 4];
    crg[0x148 / 4] = crg148 | 1;
    wait_long_timer_0(1000);

    peristat = sysctrl[PERISTAT / 4];
    mode_sel = (peristat >> 8) & 3;
    crg148 &= ~0xdu;

    if (mode_sel == 1) {
        crg148 |= 0xe;
    } else if (mode_sel == 2) {
        sysctrl[0x14c / 4] = 1;
        crg148 |= 0xau;
        crg[0x148 / 4] = crg148;
    } else {
        sysctrl[0x14c / 4] = 1;
        return -1;
    }
    crg[0x148 / 4] = crg148;

    emmc[MMC_CTRL / 4] = 7;
    while (emmc[MMC_CTRL / 4] & 7)
        ;
    emmc[0x24 / 4]      = 0;          /* MMC_CMDARG cleanup */
    emmc[MMC_RINTSTS / 4] = 0xFFFFFFFFu;
    emmc[0x8c / 4]      = 0xFFFFFFFFu;
    emmc[MMC_CTRL / 4]  = 16;          /* CTRL: int-mask off, FIFO ready */

    mode_sel = (sysctrl[PERISTAT / 4] >> 8) & 3;
    if (mode_sel == 1) {
        emmc[0x10 / 4] = 1;             /* MMC_CLKENA = 1 — enable clock */
        if (send_command_emmc() != 0)
            return -1;
        ctype_val = 32;
    } else if (mode_sel == 2) {
        emmc[0x10 / 4] = 0;
        if (send_command_emmc() != 0)
            return -1;
        emmc[0x8 / 4]  = 0;             /* CLKDIV */
        emmc[0xc / 4]  = 0;             /* reserved */
        if (send_command_emmc() != 0)
            return -1;
        emmc[0x10 / 4] = 1;
        if ((rc = send_command_emmc()) != 0)
            return -1;
        ctype_val = (u32)rc;
    } else {
        sysctrl[0x14c / 4] = 1;
        emmc[0x14 / 4] = 0xff00u | (emmc[0x14 / 4] & 0xff);  /* CTYPE update */
        emmc[0x78 / 4] = 0;
        wait_long_timer_0(100);
        emmc[0x78 / 4] = 1;
        wait_long_timer_0(1000);
        emmc[0x1c / 4] = 0x200u;        /* BLKSIZ */
        emmc[0x20 / 4] = 0x100000u;     /* BYTCNT */
        ctype_val = 5;
    }

    {
        u32 reg = emmc[0x4c / 4];
        reg &= ~0xff00000u;
        reg &= ~0xf0000u;
        reg |=  0x7f0000u;
        emmc[0x4c / 4] = reg;
    }
    emmc[0x100 / 4] = ctype_val | (0x200u << 16);
    emmc[0x18 / 4]  = 1;
    emmc[MMC_CMD / 4] = 0xa1000000u | (0x200u << 16) | ctype_val;

    /* HS200/HS400-class bus tweak when PERISTAT.[9:8] == 1 */
    {
        u32 reg108 = emmc[0x108 / 4];
        u32 sel = (sysctrl[PERISTAT / 4] >> 8) & 3;
        reg108 &= ~0x3840000u;
        reg108 &= ~0x30000u;
        if (sel == 1) {
            reg108 |= 0x2000000u;
        } else if (sel == 2) {
            sysctrl[0x14c / 4] = 1;
            reg108 |= 0x2010000u;
        }
        emmc[0x108 / 4] = reg108;
    }

    ticks = get_wait_ticks(1000) * 1000;
    timer_reset_counter();

    {
        u32 stat = 0;
        for (;;) {
            stat = emmc[MMC_RINTSTS / 4];
            if (stat & 0x200u)
                break;
            if (timer_get_value() >= ticks) {
                emmc[MMC_RINTSTS / 4] = 0xFFFFFFFFu;
                emmc[MMC_CMD / 4]     = 0x84000000u;
                return -1;
            }
        }
        emmc[MMC_RINTSTS / 4] = 0xFFFFFFFFu;
        return 0;
    }
}

/*
 * klad_load_keys — UART secure-load entry.
 *
 * Called from bootrom_secure_check when SYSSTAT bit 5 indicates the
 * fastboot/recovery signed-image path. Sequence:
 *   1. uart0_wait_start_frame; if no frame → return -1.
 *   2. Snapshot SYSSTAT to derive boot_pin (bit 4).
 *   3. uart0_proto_handshake; uart0_recv_signed_image into SRAM[0x500].
 *   4. If recv returned 0 → use "fafe" tag (UART path) and follow flash
 *      pre-stage. If non-zero → set "DOWN"/"-1" magics in SYSCTRL[0x13c/
 *      0x140], optionally re-init eMMC and re-memcpy from EMMC.
 *   5. alloc_chunk(48); parse_image_header → klad_validate_header.
 *   6. klad_dispatch_sig with arg quartet from header — runs RSA/SHA.
 *   7. invoke_foreign_code into loaded region.
 *   8. Second memset/zeroing pass; PERISTAT bit 1 dispatches between
 *      klad_post_unlock and klad_decode_chunk. Then klad_finalize and
 *      a second klad_dispatch_sig over the post-decoded region.
 *   9. On any failure send uart status (49, 0x3{0..4}) and clean up
 *      the alloc'd chunk.
 *
 * Returns 0 on success, -1 on failure.
 */
int klad_load_keys(void)  /* 0x4001470 */
{
    volatile u32 *sysctrl = (volatile u32 *)SYSCTRL_START;
    u32 sysstat, boot_pin;
    unsigned src_tag;
    int recv_result;
    void *ctx;
    int rc;

    if (uart0_wait_start_frame() == 0)
        return -1;
    sysstat = sysctrl[SYSSTAT / 4];
    boot_pin = (sysstat >> 4) & 1;

    uart0_proto_handshake();
    recv_result = uart0_recv_signed_image((void *)(SDRAM_START + SRAM_LOAD_TARGET_OFF), 0x6000);

    if (recv_result != 0) {
        src_tag = 0x73616665u;   /* "safe" — UART path */
        sysctrl[0x13c / 4] = 0x444f574eu;   /* "DOWN" */
        sysctrl[0x140 / 4] = 0xFFFFFFFFu;
        if (boot_pin == 1) {
            initialize_emmc();
            memcpy_emmc((void *)(SDRAM_START + SRAM_LOAD_TARGET_OFF), 0, 0x6000);
            src_tag = 0x73616666u;          /* "saff" — eMMC re-read */
        } else {
            memcpy((void *)(SDRAM_START + SRAM_LOAD_TARGET_OFF),
                   (const void *)FLASH_START, 0x6000);
            src_tag = 0x73616666u;          /* "saff" — flash re-read */
        }
    } else {
        src_tag = 0x73616665u;
    }

    ctx = (void *)alloc_chunk(48);
    if (ctx == (void *)0) {
        uart0_send_status(0, 0x31);
        return -1;
    }

    if (parse_image_header(ctx) != 0) {
        uart0_send_status(0x30, 0x31);
        free_chunk((unsigned)ctx);
        return -1;
    }

    if (klad_validate_header(ctx) != 0) {
        uart0_send_status(0x32, 0x31);
        free_chunk((unsigned)ctx);
        return -1;
    }

    {
        u32 *ctx32 = (u32 *)ctx;
        u32 size_arg = ctx32[6];
        u32 src      = ctx32[7];
        u32 mid      = ctx32[8];
        u32 sig      = ctx32[10];

        rc = klad_dispatch_sig(ctx, size_arg, mid, (void *)src, (void *)sig);
        if (rc != 0) {
            uart0_send_status(0x33, 0x31);
            reset_secure_io_peripherals(src_tag, boot_pin);
            free_chunk((unsigned)ctx);
            return -1;
        }

        invoke_foreign_code((void *)src);
        memset((void *)src, 0, mid);

        if (sysctrl[PERISTAT / 4] & 2) {
            /* alt key-decode path */
            refresh_ddr(0);
            if (klad_dispatch_sig(ctx, size_arg, mid, (void *)src, (void *)sig) == 0)
                goto success_done;
        }

        rc = klad_post_unlock(src_tag, (int)boot_pin, size_arg);
        if (rc != 0) {
            /* compute DDR target = 0x81000000 + src - mid - sig */
            u32 *c2 = (u32 *)ctx;
            u32 src2 = c2[0];        /* re-read */
            u32 mid2 = c2[1];
            u32 sig2 = c2[10];
            u32 dst  = 0x81000000u + src2 - mid2 - sig2;
            c2[11] = dst;
            klad_finalize(ctx);
            rc = klad_dispatch_sig(ctx, c2[11], mid2, (void *)src2, (void *)sig);
            reset_secure_io_peripherals(src_tag, boot_pin);
            if (rc != 0) {
                uart0_send_status(0x34, 0x31);
                free_chunk((unsigned)ctx);
                return -1;
            }
            *(volatile u32 *)SDRAM_START = c2[11];
        }
    }

success_done:
    free_chunk((unsigned)ctx);
    return 0;
}

/*
 * klad_verify — secure-boot path for the (sysstat & 0x10) case.
 *
 * Identical shape to klad_load_keys but:
 *   - sources the signed image from eMMC (initialize_emmc + memcpy_emmc)
 *     instead of UART
 *   - guards entry on SYSCTRL[0x14] matching the magic 0x42494945
 *     ("EIIB" — likely "Image-Boot" mode tag from OTP)
 *   - performs the same parse_image_header → klad_validate_header →
 *     klad_dispatch_sig → invoke_foreign_code chain
 *   - on a PERISTAT.bit1 retry decodes via klad_verify_rsa instead
 *     of klad_decode_chunk
 */
int klad_verify(void)  /* 0x4001f0c */
{
    volatile u32 *sysctrl = (volatile u32 *)SYSCTRL_START;
    volatile u32 *load    = (volatile u32 *)(SDRAM_START + SRAM_LOAD_TARGET_OFF);
    u32 magic = sysctrl[0x14 / 4];
    u32 sysstat, boot_pin;
    void *ctx;
    int rc;

    initialize_emmc();

    if (((magic >> 4) & 0xFFFFFFu) == 0x424945u) {
        /* Magic mismatch with the OTP boot-tag — bail out without
         * touching the chunk allocator. */
        memcpy_emmc((void *)(SDRAM_START + SRAM_LOAD_TARGET_OFF), 0, 0x6000);
        {
            u32 r0 = load[0], r3 = load[1], r1 = load[2], r2 = load[3];
            int hit = (r3 == r1 && r0 == r3);
            if (r1 != r2)
                hit = 0;
            (void)hit;
            return 0;
        }
    }

    sysstat = sysctrl[SYSSTAT / 4];
    boot_pin = (sysstat >> 4) & 1;
    ctx = (void *)alloc_chunk(48);
    if (ctx == 0)
        return -1;

    if (parse_image_header(ctx) != 0) {
        free_chunk((unsigned)ctx);
        return -1;
    }
    if (klad_validate_header(ctx) != 0) {
        free_chunk((unsigned)ctx);
        return -1;
    }

    {
        u32 *c = (u32 *)ctx;
        rc = klad_dispatch_sig(ctx, c[6], c[8], (void *)c[7], (void *)c[10]);
        if (rc != 0) {
            free_chunk((unsigned)ctx);
            return -1;
        }
        invoke_foreign_code((void *)c[7]);

        if (sysctrl[PERISTAT / 4] & 2) {
            klad_verify_rsa((void *)c[7], c[8]);
            if (klad_dispatch_sig(ctx, c[6], c[8], (void *)c[7], (void *)c[10]) == 0)
                goto out;
        }

        rc = klad_post_unlock(0x73616666u, (int)boot_pin, c[2]);
        if (rc != 0) {
            free_chunk((unsigned)ctx);
            return -1;
        }
    }

out:
    free_chunk((unsigned)ctx);
    return 0;
}

/*
 * klad_alt_verify — secure-boot path for the !(sysstat & 0x10) case.
 *
 * Same magic guard against SYSCTRL[0x14] as klad_verify, then sources
 * the signed image from FLASH instead of eMMC, and runs the same
 * header-validate / sig-dispatch chain. Final disposition routes
 * through klad_alt_dispatch (0x4001df8) when the early flash 4-word
 * check passes.
 */
int klad_alt_verify(void)  /* 0x4001bdc */
{
    volatile u32 *sysctrl = (volatile u32 *)SYSCTRL_START;
    u32 magic = sysctrl[0x14 / 4];
    u32 r0, r3, r2, r1;
    int hit;
    void *ctx;
    int rc;

    if (((magic >> 4) & 0xFFFFFFu) == 0x424945u)
        return klad_alt_dispatch(0);

    /* Flash four-word cross-check at FLASH_START. */
    r0 = *(volatile u32 *)FLASH_START;
    r3 = *(volatile u32 *)(FLASH_START + 4);
    r2 = *(volatile u32 *)(FLASH_START + 8);
    r1 = *(volatile u32 *)(FLASH_START + 0xc);
    hit = (r3 == r2 && r0 == r3) ? 1 : 0;
    if (r2 != r1)
        hit = 0;
    if (hit)
        return klad_alt_e88(0, 0);

    memcpy((void *)(SDRAM_START + SRAM_LOAD_TARGET_OFF),
           (const void *)FLASH_START, 0x6000);

    ctx = (void *)alloc_chunk(48);
    if (ctx == 0)
        return -1;

    if (parse_image_header(ctx) != 0) {
        free_chunk((unsigned)ctx);
        return -1;
    }
    if (klad_validate_header(ctx) != 0) {
        free_chunk((unsigned)ctx);
        return -1;
    }

    {
        u32 *c = (u32 *)ctx;
        rc = klad_dispatch_sig(ctx, c[6], c[8], (void *)c[7], (void *)c[10]);
        if (rc != 0) {
            free_chunk((unsigned)ctx);
            return -1;
        }
        invoke_foreign_code((void *)c[7]);

        if (sysctrl[PERISTAT / 4] & 2) {
            klad_verify_rsa((void *)c[7], c[8]);
            if (klad_dispatch_sig(ctx, c[6], c[8], (void *)c[7], (void *)c[10]) == 0)
                goto out;
        }
        rc = klad_post_unlock(0x73616667u /* "safg" */, 0, c[2]);
        if (rc != 0) {
            free_chunk((unsigned)ctx);
            return -1;
        }
    }

out:
    free_chunk((unsigned)ctx);
    return 0;
}

/*
 * ============================================================================
 * Frontier reversal — remaining small/medium leaves.
 *
 * 17 functions ranging from 1 instruction (nop_2d74) to 88 (refresh_ddr).
 * Each is small enough to reverse precisely from disasm. The remaining
 * deep helpers (klad_dispatch_sig at 510 instructions, klad_post_unlock at
 * ~700, klad_validate_header at ~658, klad_finalize at ~520, plus the
 * three big alt_* paths and the giant uart0_recv_payload / media_program_b
 * / media_finalize_b crypto/SD-protocol drivers) stay as stubs.
 *
 * Two name corrections: cv500's functions.txt names 0x358 "refresh_ddr"
 * and 0x10ac "reset_secure_io_peripherals" — the PR #137 placeholders
 * klad_decode_chunk / klad_init_check were wrong. Renamed everywhere.
 * ============================================================================
 */

extern void uart0_write(int byte);  /* 0x400514c — leaf TX, single byte */
extern unsigned get_random_value(void);  /* 0x40002cc */

unsigned get_random_value(void)  /* 0x40002cc */
{
    volatile u32 *trng = (volatile u32 *)TRNG_START;
    unsigned timeout = 200 * get_wait_ticks(1000);

    timer_reset_counter();
    for (;;) {
        u32 st = trng[HISEC_COM_TRNG_DATA_ST / 4];
        if ((st >> 8) & 0xff)
            return trng[HISEC_COM_TRNG_FIFO_DATA / 4];
        if (timer_get_value() >= timeout)
            return 0;
    }
}

void uart0_send_status(int code, int extra)  /* 0x4000320 */
{
    uart0_write(10);
    uart0_write('E');
    uart0_write(code);
    uart0_write('D');
    uart0_write(extra);
}

int refresh_ddr(unsigned mctl_mode)  /* 0x04000358 */
{
    volatile u32 *ddrc = (volatile u32 *)(DDRC_START + 0x8000);
    volatile u32 *misc = (volatile u32 *)MISC_START;
    unsigned timeout = mctl_mode << 2;
    int has_phy_cal = (ddrc[0x50 / 4] & 0xf) ? 1 : 0;
    unsigned t1, t2;

    if (has_phy_cal) {
        ddrc[0] = 1;
        timer_reset_counter();
        while ((ddrc[0x294 / 4] & 1) == 0) {
            if (timer_get_value() >= timeout) {
                uart0_send_status(10, 1);
                return -1;
            }
        }
        t1 = get_random_value();
        t2 = get_random_value();
        if (t1 != t2) {
            (void)get_random_value();
            (void)get_random_value();
            misc[0x28 / 4] = 0;
            misc[0x2c / 4] = 0;
            (void)get_random_value();
            (void)get_random_value();
            (void)get_random_value();
            (void)get_random_value();
            ddrc[0] = 2;
            timer_reset_counter();
            while ((ddrc[0x294 / 4] & 1) == 0) {
                if (timer_get_value() >= timeout) {
                    uart0_send_status(10, 3);
                    return -1;
                }
            }
            return 0;
        }
    }
    timer_reset_counter();
    while ((ddrc[0x294 / 4] & 1) == 0) {
        if (timer_get_value() >= timeout)
            break;
    }
    return 0;
}

int reset_secure_io_peripherals(unsigned boot_type, int boot_pin)  /* 0x040010ac */
{
    volatile u32 *crg = (volatile u32 *)CRG_START;
    u32 v;

    v = crg[PERI_CRG104 / 4];
    v |= 0x100u;
    crg[PERI_CRG104 / 4] = v;
    v = crg[PERI_CRG104 / 4];
    v |= 0x14u;
    crg[PERI_CRG104 / 4] = v;
    v = crg[PERI_CRG104 / 4];
    v &= ~0x100u;
    crg[PERI_CRG104 / 4] = v;
    v = crg[PERI_CRG104 / 4];
    v &= ~0x14u;
    crg[PERI_CRG104 / 4] = v;

    if (boot_type == 0x73616667u) {
        close_sdio0();
        return 0;
    }
    if (boot_type == 0x73616668u)
        return 0;
    if (boot_type == 0x73616665u) {
        disable_uart0_hdwr();
        return 0;
    }
    if (boot_pin == 1)
        reset_emmc();
    return 0;
}

void nop_2d74(void)  /* 0x4002d74 */
{
}

unsigned alloc_chunk(unsigned size)  /* 0x4002b78 */
{
    volatile u32 *desc = (volatile u32 *)(SDRAM_START + SRAM_CHUNK_TABLE_OFF);
    if (size > desc[0]) {
        do { desc += 4; } while (desc[0] < size);
    }

    for (;;) {
        u32 cls_size  = desc[0];
        u32 cls_count = desc[1];
        u32 cls_base  = desc[2];
        u32 cls_mask  = desc[3];

        if ((int)cls_count <= 0)
            return 0;
        if ((cls_mask & 1) == 0) {
            desc[3] = cls_mask | 1u;
            return cls_base;
        }
        {
            unsigned bit = 0;
            u32 m = cls_mask;
            while ((m & 1) && bit < cls_count) {
                bit++;
                m >>= 1;
            }
            if (bit < cls_count) {
                desc[3] = cls_mask | (1u << bit);
                return cls_base + bit * cls_size;
            }
        }
        desc += 4;
    }
}

void free_chunk(unsigned addr)  /* 0x4002c1c */
{
    volatile u32 *desc = (volatile u32 *)(SDRAM_START + SRAM_CHUNK_TABLE_OFF);
    u32 cls_size, cls_count, cls_base, off;

    for (;;) {
        cls_size  = desc[0];
        cls_count = desc[1];
        cls_base  = desc[2];
        off = addr - cls_base;
        if (off < cls_count * cls_size)
            break;
        desc += 4;
    }

    if (off == 0) {
        desc[3] &= ~1u;
        return;
    }
    {
        unsigned bit = 0;
        u32 t = 0;
        while (off > t) {
            bit++;
            t += cls_size;
        }
        desc[3] &= ~(1u << bit);
    }
}

void wait_long_timer_0(unsigned ms)  /* 0x4002c84 */
{
    while (ms != 0) {
        unsigned chunk = ms < 0x989680u ? ms : 0x989680u;
        wait_timer_0(chunk);
        ms -= chunk;
    }
}

int send_command_emmc(void)  /* 0x4003c30 */
{
    volatile u32 *emmc = (volatile u32 *)EMMC_START;
    int retries = 3;

    while (retries--) {
        unsigned tries = 0xf00;
        emmc[MMC_CMD / 4] = 0x20a02000u;
        while (tries--) {
            if ((int)emmc[MMC_CMD / 4] >= 0)
                return 0;
            if (emmc[MMC_RINTSTS / 4] & 0x1000u)
                break;
        }
    }
    return -1;
}

void configure_emmc_pins(void)  /* 0x4003d68 */
{
    volatile u32 *sysctrl  = (volatile u32 *)SYSCTRL_START;
    volatile u32 *iocfg0   = (volatile u32 *)IO_CTRL0_START;
    const u32 *table       = (const u32 *)0x00007d98;
    u32 peristat = sysctrl[PERISTAT / 4];
    unsigned mode = (peristat >> 8) & 3;
    int needs_pull_fix = (peristat & 0x2000u) != 0;
    unsigned i;

    iocfg0[0] = table[mode];
    for (i = 1; i < 6; i++) {
        u32 v = table[mode + i];
        if (needs_pull_fix)
            v &= ~0x100u;
        iocfg0[i] = v;
    }
    iocfg0[6] = table[mode + 6];
}

void initialize_uart0(void)  /* 0x40050c0 */
{
    volatile u32 *crg       = (volatile u32 *)CRG_START;
    volatile u32 *sram_base = (volatile u32 *)(SDRAM_START + SRAM_UART_BASE_OFF);
    volatile u32 *uart;
    u32 v;

    v = crg[0x1b8 / 4];
    crg[0x1b8 / 4] = v | 1u;

    v = crg[0x1bc / 4];
    crg[0x1bc / 4] = v & ~0x40000u;

    sram_base[0] = UART0_START;
    uart = (volatile u32 *)UART0_START;
    uart[UART_CR / 4]    = 0;
    uart[UART_IBRD / 4]  = 13;
    uart[UART_FBRD / 4]  = 1;
    uart[UART_LCR_H / 4] = 0x70;
    uart[UART_CR / 4]    = 0x301;
}

void disable_uart0(void)  /* 0x400512c */
{
    volatile u32 *sram_base = (volatile u32 *)(SDRAM_START + SRAM_UART_BASE_OFF);
    volatile u32 *uart = (volatile u32 *)sram_base[0];
    u32 cr = uart[UART_CR / 4];
    uart[UART_CR / 4] = cr & ~1u;
}

int uart0_read(void)  /* 0x4005170 */
{
    volatile u32 *sram_base = (volatile u32 *)(SDRAM_START + SRAM_UART_BASE_OFF);
    volatile u32 *uart = (volatile u32 *)sram_base[0];
    u32 byte;

    while (uart[UART_FR / 4] & UART_FR_RXFE)
        ;
    byte = uart[UART_DR / 4];
    if (byte & ~0xffu) {
        uart[UART_RSR / 4] = 0xFFFFFFFFu;
        return -1;
    }
    return (int)byte;
}

int uart0_has_rx_data(void)  /* 0x40051a0 */
{
    volatile u32 *sram_base = (volatile u32 *)(SDRAM_START + SRAM_UART_BASE_OFF);
    volatile u32 *uart = (volatile u32 *)sram_base[0];
    return ((uart[UART_FR / 4] ^ 0x10u) >> 4) & 1u;
}

unsigned timer_get_value(void)  /* 0x4005234 */
{
    volatile u32 *timer0 = (volatile u32 *)TIMER0_START;
    return ~timer0[TIMER_VALUE / 4];
}

unsigned get_wait_ticks(unsigned ms)  /* 0x4005244 */
{
    (void)ms;
    return 3000;
}

void update_timer_0_value(void)  /* 0x400524c */
{
    volatile u32 *timer0 = (volatile u32 *)TIMER0_START;
    volatile u32 *state  = (volatile u32 *)(SDRAM_START + SRAM_TIMER0_VALUE_OFF);
    u32 cur    = timer0[TIMER_VALUE / 4];
    u32 delta  = state[1];
    u32 prev   = state[0];
    u32 new_delta;

    state[0] = cur;
    new_delta = prev + delta;
    if (cur > prev)
        new_delta -= 1;
    new_delta -= cur;
    state[1] = new_delta;
}

/*
 * ============================================================================
 * RSA0/SPACC/SHA inner hardware-driver primitives.
 *
 * 14 leaves that drive the RSA0 and SPACC blocks directly. After
 * reversal only ~4 SPACC sub-primitives at 0x40039a8/0x4003a74/
 * 0x4003ae8/0x4003b68 remain as stubs — those are the per-block
 * SPACC bring-up details.
 *
 * Hardware register layout discovered:
 *   RSA0 @ 0x10080000    — public-key engine
 *     0x50  status (kicked by rsa0_kick)
 *     0x54  mode/length register
 *     0x58  modulus input FIFO
 *     0x5c  signature input FIFO
 *     0x64  result output FIFO
 *     0x68  command register (cmd 5 = start)
 *   SPACC @ 0x100c0000   — SHA + AES accelerator
 *     0x340/0x348/0x34c   per-context offsets
 *     0x40c               per-context flags
 *     0x480/0x484         primary context selector
 *     0x4b0/0x4b4         secondary context selector
 *     0x80c               status (waited on by sha_compute)
 *   SRAM @ 0x04010110    — 64-byte SHA working buffer
 * ============================================================================
 */

extern int  spacc_init(void *ctx);                  /* 0x40039a8 */
extern void spacc_finalize(void);                   /* 0x4003a74 */
extern void spacc_chunk_prep(void *ctx, unsigned size);  /* 0x4003ae8 */
extern void spacc_chunk_run(void);                  /* 0x4003b68 */

int rsa0_kick(unsigned mode, unsigned ticks)  /* 0x40006a4 */
{
    volatile u32 *status = (volatile u32 *)mode;
    timer_reset_counter();
    for (;;) {
        if ((status[0] & ticks) == 0)
            return 0;
        if (timer_get_value() >= 1)
            return -1;
    }
}

int klad_finalize_check(unsigned val, unsigned size, unsigned len)  /* 0x4000614 */
{
    volatile u32 *status = (volatile u32 *)val;
    timer_reset_counter();
    for (;;) {
        u32 v = status[0];
        if ((v & size) == 0)
            return 0;
        if (timer_get_value() >= len)
            return -1;
    }
}

int sha_init(void *ctx)  /* 0x40006e0 */
{
    unsigned ticks = get_wait_ticks(1) * 200;
    spacc_init(ctx);
    if (rsa0_kick(SPACC_START + 0x390, ticks) != 0)
        return -1;
    spacc_finalize();
    return 0;
}

int sha_compute(void *src)  /* 0x4000654 */
{
    volatile u32 *spacc = (volatile u32 *)SPACC_START;
    unsigned ticks = get_wait_ticks(1) * 200;
    int rc;
    u32 stat;

    spacc_chunk_prep(src, 0);
    spacc_chunk_run();
    rc = klad_finalize_check(SPACC_START + 0x80c, 0x80000, ticks);
    stat = spacc[0x80c / 4];
    spacc[0x80c / 4] = stat;
    return rc;
}

void sha_write_output(void *dst)  /* 0x4003bcc */
{
    volatile u32 *sram_ctrl = (volatile u32 *)(SDRAM_START + 0x110);
    volatile u32 *spacc = (volatile u32 *)SPACC_START;
    u32 word_count = sram_ctrl[0x40 / 4] >> 2;
    u8 *p = (u8 *)dst;
    unsigned i;

    for (i = 0; i < word_count; i++) {
        u32 word;
        spacc[0x34c / 4] = 0;
        word = spacc[0x348 / 4];
        memcpy(p, &word, 4);
        p += 4;
    }
}

void klad_sha_init(void *a, void *b)  /* 0x40036c4 */
{
    volatile u32 *spacc = (volatile u32 *)SPACC_START;
    volatile u32 *sram  = (volatile u32 *)(SDRAM_START + 0x110);
    u32 v;

    memset((void *)(SDRAM_START + 0x110), 0, 64);

    v = spacc[0x484 / 4];
    v &= ~(0x7fu << 24);
    v |=  (2u << 24);
    spacc[0x484 / 4] = v;
    spacc[0x488 / 4] = (u32)a;
    sram[0x30 / 4] = 0;
    sram[0x38 / 4] = (v >> 16) & 0x7f;
    sram[0x24 / 4] = (u32)a;

    v = spacc[0x4b0 / 4];
    v &= ~(0x7fu << 24);
    v |=  (2u << 24);
    spacc[0x4b0 / 4] = v;
    spacc[0x4b4 / 4] = (u32)b;
    sram[0x3c / 4] = (v >> 16) & 0x7f;
    sram[0x28 / 4] = (u32)b;
    sram[0x34 / 4] = 0;
}

void klad_sha_finalize(void)  /* 0x4003858 */
{
    volatile u32 *spacc = (volatile u32 *)SPACC_START;
    volatile u32 *sram  = (volatile u32 *)(SDRAM_START + 0x110);
    u32 v = spacc[0x480 / 4];

    v &= ~0xc4000u;
    v |=  0x4000u;
    v &= ~(3u << 10);
    v |=  (2u << 10);
    v |=  0x380u;
    v &= ~(7u << 4);
    v |=  (2u << 4);
    v &= ~(7u << 1);
    v |=  (1u << 1);
    spacc[0x480 / 4] = v;

    sram[0] = 2;
    sram[1] = 1;
    sram[0x30 / 4] = 0;
    sram[0x34 / 4] = 0;
}

void klad_sha_chunk(void *addr, unsigned len)  /* 0x400373c */
{
    volatile u32 *sram = (volatile u32 *)(SDRAM_START + 0x110);
    u32 slot = sram[0x38 / 4];
    u32 base = sram[0x24 / 4] + (slot << 5);
    u32 *desc = (u32 *)base;
    u8 *bp = (u8 *)base;

    sram[0x38 / 4] = slot + 1;
    memset((void *)base, 0, 32);
    bp[1] = (u8)((bp[1] & ~3u) | 3u);
    sram[0x30 / 4]++;
    desc[2] = (u32)addr;
    desc[3] = len;
    sram[0x38 / 4] &= 1;

    if (sram[0x18 / 4] != 0) {
        u32 *src = (u32 *)((u8 *)sram + 0xc);
        desc[4] = sram[0x8 / 4];
        desc[5] = src[0];
        desc[6] = src[1];
    }
}

void klad_sha_chunk2(unsigned addr, unsigned len)  /* 0x40037e4 */
{
    volatile u32 *sram = (volatile u32 *)(SDRAM_START + 0x110);
    u32 slot = sram[0x3c / 4];
    u32 base = sram[0x28 / 4] + (slot << 5);
    u32 *desc = (u32 *)base;
    u8 *bp = (u8 *)base;

    sram[0x3c / 4] = slot + 1;
    memset((void *)base, 0, 32);
    sram[0x34 / 4]++;
    desc[1] = addr;
    desc[2] = len;
    bp[1] = (u8)((bp[1] & ~0xfu) | 3u);
    sram[0x3c / 4] &= 1;
}

void klad_sha_update(unsigned arg)  /* 0x40038dc */
{
    if (arg == 0)
        return;
    memcpy((void *)(SDRAM_START + 0x110 + 8), (const void *)arg, 16);
    *(volatile u32 *)(SDRAM_START + 0x110 + 0x18) = 16;
}

int klad_sha_close(void)  /* 0x400390c */
{
    volatile u32 *spacc = (volatile u32 *)SPACC_START;
    volatile u32 *sram  = (volatile u32 *)(SDRAM_START + 0x110);
    u32 v = spacc[0x4b0 / 4];
    u32 a = (v >> 16) & 0x7f;
    u32 b = (v >> 8)  & 0x7f;

    if (a != b)
        return -1;

    {
        u32 t1 = sram[0x34 / 4];
        u32 sum = (b + t1) & 1;
        v &= ~(0x7fu << 16);
        v |= (sum << 16);
        v &= ~0x7fu;
        v |= t1 & 0x7f;
        spacc[0x4b0 / 4] = v;
    }
    {
        u32 v2 = spacc[0x484 / 4];
        u32 t2 = sram[0x30 / 4];
        u32 sum2 = (t2 + ((v2 >> 16) & 0x7f)) & 1;
        v2 &= ~(0x7fu << 16);
        v2 |= (sum2 << 16);
        v2 &= ~0x7fu;
        v2 |= t2 & 0x7f;
        spacc[0x484 / 4] = v2;
    }
    sram[0x30 / 4] = 0;
    sram[0x34 / 4] = 0;
    return 0;
}

/*
 * ---- klad_load_const — KLAD key-ladder bootstrap ----
 *
 * Corrects earlier reversal: this function does NOT load RSA0
 * constants. It runs the KLAD (Key Ladder) hardware through two
 * chained derivation operations using 32 bytes of vendor-baked
 * constants from bootrom[0x7e38..0x7e57] — the result is the
 * boot-verification key material that downstream RSA/SHA operations
 * use to validate the loaded image.
 *
 * Sequence:
 *   1. Spin-wait on OTPUSER[0x4c] bit 0 clear (OTP block idle)
 *   2. OTPUSER[0x8]  = 0          (clear status)
 *      OTPUSER[0x0]  = 1          (enable)
 *      OTPUSER[0x4]  = 0x1acce551 (vendor "begin key ladder" magic)
 *   3. Spin-wait on OTPUSER[0x4c] bit 1 set (handshake done)
 *   4. KLAD[0]       = 2          (select derivation mode)
 *      KLAD[0x10..0x1c] = bootrom[0x7e38..0x7e47]  (first 16 bytes)
 *      KLAD[0]       |= 1         (trigger op 1)
 *      Spin-wait on KLAD[0] bit 0 clear
 *   5. KLAD[0x10..0x1c] = bootrom[0x7e48..0x7e57]  (second 16 bytes)
 *      KLAD[0]       |= 0x11      (trigger op 2 with chain bit)
 *      Spin-wait on KLAD[0] bit 0 clear
 *
 * The two 16-byte chunks are likely AES-CMAC or AES-key derivation
 * inputs (high-entropy, no obvious structure). Output of op 1
 * presumably feeds op 2 as input via the KLAD ladder's internal
 * state machine — this is the canonical "load key into chain" then
 * "encrypt key with chained key" pattern.
 *
 * Dumped from av300 silicon (PR #137 MANIFEST):
 *   0x7e28..0x7e37 = floor(log2(x))+1 nibble table for PKCS#1 parser
 *   0x7e38..0x7e47 = c586db50 0c2fc592 6fca6c43 f5cafe2f  (op 1 input)
 *   0x7e48..0x7e57 = 01ae964a 200ec63f c9a49d5e 999badd5  (op 2 input)
 *
 * Whether these bytes are family-shared (cv500/ev300/etc.) or
 * av300-unique requires comparing against dumps from sibling chips.
 */
void klad_load_const(void)  /* 0x400122c */
{
    const u32 *table       = (const u32 *)0x00007e28;
    volatile u32 *otpuser  = (volatile u32 *)OTPUSER_START;
    volatile u32 *klad     = (volatile u32 *)KLAD_START;
    u32 chunk1_w0 = table[4],  chunk1_w1 = table[5];
    u32 chunk1_w2 = table[6],  chunk1_w3 = table[7];
    u32 chunk2_w0 = table[8],  chunk2_w1 = table[9];
    u32 chunk2_w2 = table[10], chunk2_w3 = table[11];

    /* OTPUSER handshake: wait idle, then issue the magic command. */
    while ((otpuser[0x4c / 4] & 1) != 0)
        ;
    otpuser[0x8 / 4]  = 0;
    otpuser[0x0 / 4]  = 1;
    otpuser[0x4 / 4]  = 0x1acce551u;
    while ((otpuser[0x4c / 4] & 2) == 0)
        ;

    /* KLAD operation 1 — load + derive from first 16 bytes. */
    klad[0]        = 2;
    klad[0x10 / 4] = chunk1_w0;
    klad[0x14 / 4] = chunk1_w1;
    klad[0x18 / 4] = chunk1_w2;
    klad[0x1c / 4] = chunk1_w3;
    klad[0]        = klad[0] | 1u;
    while ((klad[0] & 1) != 0)
        ;

    /* KLAD operation 2 — chain in second 16 bytes (bit 4 = chain). */
    klad[0x10 / 4] = chunk2_w0;
    klad[0x14 / 4] = chunk2_w1;
    klad[0x18 / 4] = chunk2_w2;
    klad[0x1c / 4] = chunk2_w3;
    klad[0]        = klad[0] | 0x11u;
    while ((klad[0] & 1) != 0)
        ;
}

int klad_check_step(void)  /* 0x4001314 */
{
    volatile u32 *fcm = (volatile u32 *)0x1000040c;
    u32 v = fcm[0];
    if (((v >> 8) & 2) == 0)
        return -1;
    v &= ~(0xffu << 8);
    fcm[0] = v;
    return 0;
}

int klad_alt_step(unsigned boot_pin)  /* 0x4001b40 */
{
    return reset_secure_io_peripherals(0x73616667u /* "safg" */, (int)boot_pin);
}

/*
 * ============================================================================
 * UART fastboot protocol drivers.
 *
 * The bootrom implements a CRC-checksummed frame protocol on UART0 for
 * recovery / fastboot use. receive_frame() at 0x4002ee4 (left as a stub —
 * 269 instructions of state machine) returns a per-frame state code that
 * these five functions dispatch on:
 *
 *   state 1 ("ENTER")  — send 0xAA ack, wait for confirmation
 *   state 2 ("DATA")   — send 0x55 ack, continue receiving
 *   state 4 ("END")    — send 0xAA ack, frame complete
 *
 * The "EMMC" magic (0x454d4d43 little-endian = bytes 'C','M','M','E') is
 * stored in SYSCTRL[0x150] by the payload itself to signal a clean
 * handover from the loaded code back to the bootrom continuation point.
 * The protocol also writes the boot status pair "ziju"/"DOWN" into
 * SYSCTRL[0x140/0x13c] when the host has acknowledged the entry frame.
 * ============================================================================
 */

extern int receive_frame(void *frame);  /* 0x4002ee4 — 269-instr CRC parser */

/* ---- UART0 single-byte TX leaf (TX FIFO full check, then write) ---- */
void uart0_write(int byte)  /* 0x400514c */
{
    volatile u32 *sram_base = (volatile u32 *)(SDRAM_START + SRAM_UART_BASE_OFF);
    volatile u32 *uart = (volatile u32 *)sram_base[0];
    while (uart[UART_FR / 4] & 0x20u)   /* TXFF — TX FIFO full */
        ;
    uart[UART_DR / 4] = (u32)byte;
}

/*
 * ---- uart0_wait_start_frame ----
 *
 * Re-pinmuxes UART0, brings up the controller, then runs a 5-iteration
 * probe loop: each iteration sends 5 space bytes (0x20) and waits up to
 * (get_wait_ticks(1) * 10) timer ticks for an 0xAA reply from the host.
 * On 0xAA received → return 1. After 5 failed probes → emit '\n', disable
 * UART0, return 0. The host indication that fastboot mode is requested
 * is exactly this 0xAA reply.
 */
int uart0_wait_start_frame(void)  /* 0x4003410 */
{
    volatile u32 *iocfg1 = (volatile u32 *)IO_CTRL1_START;
    int outer;
    unsigned wait_ticks;

    iocfg1[0x10 / 4] = 0x531;
    iocfg1[0x14 / 4] = 0x431;
    initialize_uart0();
    uart0_flush_rx_fifo();
    wait_ticks = get_wait_ticks(1) * 10;

    for (outer = 5; outer > 0; outer--) {
        int i;
        for (i = 0; i < 5; i++)
            uart0_write(0x20);

        timer_reset_counter();
        for (;;) {
            if (uart0_has_rx_data()) {
                int b = uart0_read() & 0xff;
                if (b == 0xaa)
                    return 1;
                /* anything else — keep waiting on the same probe */
            }
            if (timer_get_value() >= wait_ticks)
                break;
        }
    }

    uart0_write('\n');
    disable_uart0();
    return 0;
}

/*
 * ---- uart0_recv_payload ----
 *
 * Frame-receive loop. Calls receive_frame on a 28-byte local frame
 * descriptor and dispatches:
 *   state 1 → send 0xAA, recv next frame (must be state 2)
 *   state 2 → send 0x55, continue
 *   state 4 → send 0xAA, invoke callback embedded in frame at sp+28,
 *             flush RX, retry until SYSCTRL[0x150] matches "EMMC" magic,
 *             then clear it and return.
 *
 * The "EMMC" magic 0x454d4d43 (bytes 'C','M','M','E' little-endian =
 * "EMMC" big-endian read) is written into SYSCTRL[0x150] by the loaded
 * payload itself to signal back into the bootrom.
 */
int uart0_recv_payload(void)  /* 0x4003354 */
{
    volatile u32 *sysctrl = (volatile u32 *)SYSCTRL_START;
    u32 frame[7];                       /* 28-byte local frame descriptor */
    int state;

    for (;;) {
        memset(frame, 0, sizeof(frame));
        state = receive_frame(frame);

        if (state == 4) {
            typedef void (*foreign_fn)(void);
            uart0_write(0xaa);
            ((foreign_fn)frame[6])();    /* sp+28 — callback ptr */
            uart0_flush_rx_fifo();
            continue;
        }
        if (state == 2) {
            uart0_write(0x55);
            continue;
        }
        if (state == 1) {
            uart0_write(0xaa);
            /* Confirm follow-up frame is state 2 */
            state = receive_frame(frame);
            if (state == 2) {
                uart0_write(0x55);
                continue;
            }
            continue;
        }

        /* Check the EMMC magic — sentinel for "loaded code finished" */
        if (sysctrl[0x150 / 4] == 0x454d4d43u) {
            sysctrl[0x150 / 4] = 0;
            return 0;
        }
    }
}

/*
 * ---- uart0_recv_signed_image(dst, expected_size) ----
 *
 * Per-frame loop that stages the signed image into `dst`. Each frame's
 * header at frame[3] holds the data length; frame[5] flagged as 1
 * indicates the final frame, at which point the descriptor is captured
 * (sp+8 = dst). Caps at 256 frames; returns 0 on END (state 4), -1 on
 * timeout or size mismatch.
 */
int uart0_recv_signed_image(void *dst, unsigned expected_size)  /* 0x40034e0 */
{
    u32 frame[7];
    int size_mismatch = 0;
    int retries = 0;
    int state;
    /* Stored result captured on "final frame" indication. */
    void *captured = (void *)0;
    (void)captured;

    for (;;) {
        memset(frame, 0, sizeof(frame));
        state = receive_frame(frame);

        if (state == 2) {
            uart0_write(0x55);
        } else if (state == 4) {
            uart0_write(0xaa);
            return -size_mismatch;        /* 0 on success, -1 on mismatch */
        } else if (state == 1) {
            uart0_write(0xaa);
            if (frame[2] != expected_size)
                size_mismatch = 1;
            if (frame[5] == 1)
                captured = dst;            /* recorded but never used outside this fn */
            continue;
        }

        if (++retries > 255)
            return -1;
    }
}

/*
 * ---- uart0_proto_handshake ----
 *
 * Runs one frame exchange to confirm the host is ready, then writes the
 * "ziju" / "DOWN" boot status pair into SYSCTRL[0x140/0x13c] and sets
 * SYSCTRL[0x154] = 1 (the same recovery-mode latch that burn_bootloader
 * sets). Flushes RX and re-zeros the frame buffer before returning.
 */
void uart0_proto_handshake(void)  /* 0x4003580 */
{
    volatile u32 *sysctrl = (volatile u32 *)SYSCTRL_START;
    u32 frame[7];
    int state;

    memset(frame, 0, sizeof(frame));
    for (;;) {
        state = receive_frame(frame);
        if (state == 2) {
            uart0_write(0x55);
            continue;
        }
        if (state == 4) {
            uart0_write(0xaa);
            break;
        }
        if (state == 1) {
            uart0_write(0xaa);
            state = receive_frame(frame);
            if (state == 2) {
                uart0_write(0x55);
                continue;
            }
        }
        /* state == 0 or unknown — keep polling */
    }

    sysctrl[0x140 / 4] = 0x7a696a75u;     /* "ziju" */
    sysctrl[0x13c / 4] = 0x444f574eu;     /* "DOWN" */
    sysctrl[0x154 / 4] = 1;
    uart0_flush_rx_fifo();
    memset(frame, 0, sizeof(frame));
}

/*
 * ============================================================================
 * eMMC/SDIO media drivers.
 *
 * Six functions implementing the SDIO0-side image loader: card init,
 * descriptor-object allocation, MBR detection, sector copy, and the
 * alternative-media completion-status sentinel. The structural shape is
 * preserved here; deeper details (CMD-table dispatch inside
 * send_command_sdio0, DMA descriptor chain layout, FAT/MBR semantics)
 * await follow-up review.
 * ============================================================================
 */

extern int send_command_sdio0(unsigned cmd, unsigned arg, int sync);  /* 0x4003ce0 */
extern int sdio_read_block(void *dst);                  /* 0x4006cb8 — single 512B read leaf */
extern int sdio_write_block(void *src);                 /* 0x4006cc0 — single 512B write leaf */

/*
 * ---- media_sub_c ----
 *
 * Writes a vendor-specific failure status (0x530f) into SYSCTRL[0x140].
 * Called by media_init_alt's timeout path. The 0x530f marker is the
 * "boot media not found" indication; downstream code keys off it.
 */
void media_sub_c(void)  /* 0x400795c */
{
    volatile u32 *crg = (volatile u32 *)CRG_START;
    crg[0x140 / 4] = 0x530fu;
}

/*
 * ---- media_sub_a ----
 *
 * Thin wrapper: read the descriptor pointer at SRAM[0x264], then chain
 * into the worker at media_sub_a_inner (0x4007ae8). The original is just
 * "load ptr; b worker", so we keep that shape — the worker stays as a
 * link-only stub at the new frontier.
 */
extern int media_sub_a_inner(void *desc);   /* 0x4007ae8 — block-read worker */

int media_sub_a(void)  /* 0x40077a8 */
{
    void *desc = *(void *volatile *)(SDRAM_START + 0x264);
    return media_sub_a_inner(desc);
}

/*
 * ---- media_sub_b ----
 *
 * Allocates a 2636-byte (0xa4c) descriptor object, then a 512-byte
 * sub-buffer + 18-byte command scratch + nested 512-byte block. Populates
 * the descriptor with vendor pointers (0x7f68 — sd_command_table) and
 * stashes the controller base in SRAM[0x260+4] for media_sub_a's
 * descriptor lookup.
 *
 * Returns the controller object base, or 0 on allocation failure. The
 * exact field layout is partially decoded — see the offsets in this
 * function (0x28/0x2c/0x158/0x190/0x1a4/0x1c4) and follow-up RE for
 * full structure detail.
 */
extern int media_sub_b_setup(void *desc);   /* 0x4007898+ — descriptor setup tail */

unsigned media_sub_b(void)  /* 0x40077b8 */
{
    volatile u32 *sram_ctrl = (volatile u32 *)(SDRAM_START + 0x260 + 4);
    u8 *desc;
    u8 *cmd_buf;
    u8 *data_buf;

    desc = (u8 *)alloc_chunk(0xa4c);
    if (desc == 0)
        return 0;
    /* Zero the trailing 12 bytes (descriptor tail) — original loop only. */
    memset(desc + 0xa40, 0, 12);
    sram_ctrl[0] = (u32)desc;

    cmd_buf = (u8 *)alloc_chunk(18);
    if (cmd_buf == 0) {
        free_chunk((unsigned)desc);
        return 0;
    }

    data_buf = (u8 *)alloc_chunk(512);
    if (data_buf == 0) {
        free_chunk((unsigned)desc);
        free_chunk((unsigned)cmd_buf);
        return 0;
    }

    *(u32 *)(desc + 0x28) = 1;
    *(u32 *)(desc + 0x2c) = 1;
    *(u32 *)(desc + 0x00) = 0x000e7f68u;            /* vendor sd_command_table ptr */
    *(u32 *)(desc + 0x04) = (u32)cmd_buf;
    *(u32 *)(desc + 0x158) = (u32)data_buf;

    /* Two 16-byte memcpy slabs from the same vendor table (header copies). */
    memcpy(desc + 8,  (const void *)0x00007f68, 1);
    memcpy(desc + 26, (const void *)0x00007f68, 1);

    /* Compute aligned offsets inside the descriptor at +0x190 / +0x194. */
    {
        u32 r = (u32)desc + 0x1c4 + 3;
        r &= ~0xfu;
        *(u32 *)(desc + 0x190) = r;
    }
    /* The remaining descriptor population is deferred to the helper. */
    return (unsigned)media_sub_b_setup(desc);
}

/*
 * ---- media_init_d ----
 *
 * Brings up the SDIO0 controller (Synopsys DW MMC instance #2 at
 * SDIO0_START = 0x100f0000). Mirrors the shape of initialize_emmc but
 * for SD-card mode:
 *
 *   1. Power on SDIO0 via PWREN bit 0.
 *   2. Wait ~timeout ticks (computed from get_wait_ticks).
 *   3. CTRL = 7 (reset DMA + FIFO + INT); poll until clear.
 *   4. Set CMDARG=0, RINTSTS=-1, TBBCNT=-1, CTRL=16 (PIO).
 *   5. CTYPE setup, UHS_REG_EXT clock-divider, CARDTHRCTL.
 *   6. Run a CMD sequence via send_command_sdio0: CMD0 (idle),
 *      CMD8 (interface cond), ACMD41 (op cond), CMD2 (CID),
 *      CMD3 (RCA), CMD7 (select), CMD16 (block len 512).
 *   7. alloc_chunk(512) for a working sector buffer; stash pointer
 *      in SRAM[0x260+4].
 *   8. Return clz(buf)>>5 — i.e. 0 if buf was non-zero, 1 if zero.
 *
 * Returns 0 on success, -1 on timeout or allocation failure.
 */
int media_init_d(void)  /* 0x400422c */
{
    volatile u32 *sdio0 = (volatile u32 *)SDIO0_START;
    volatile u32 *sram_ctrl = (volatile u32 *)(SDRAM_START + 0x260 + 4);
    unsigned timeout_ticks;
    u32 ocr_resp;
    u32 rca;
    void *buf;

    timeout_ticks = get_wait_ticks(1) * 20;
    sdio0[MMC_PWREN / 4] = 1;
    timer_reset_counter();
    while (timer_get_value() <= timeout_ticks)
        ;

    sdio0[MMC_CTRL / 4] = 7;
    /* Poll reset bits clear with a 0xf00-iter timeout. */
    {
        unsigned tries = 0xf00;
        while (tries--) {
            if ((sdio0[MMC_CTRL / 4] & 7) == 0)
                break;
        }
        if (tries == 0)
            return -1;
    }

    sdio0[0x24 / 4]      = 0;             /* CMDARG cleanup */
    sdio0[MMC_RINTSTS/4] = 0xFFFFFFFFu;
    sdio0[0x8c / 4]      = 0xFFFFFFFFu;
    sdio0[MMC_CTRL / 4]  = 16;            /* PIO mode */

    {
        u32 v = sdio0[0x14 / 4];
        sdio0[0x14 / 4]  = (v & 0xff) | 0xff00u;
    }
    sdio0[0x4c / 4]  = 0x20070008u;       /* UHS_REG_EXT */
    sdio0[0x100 / 4] = 0x02000005u;       /* CARDTHRCTL */
    sdio0[0x18 / 4]  = 0;
    sdio0[0x10 / 4]  = 0;

    /* Vendor command sequence — CMD0/CMD8/ACMD41/CMD2/CMD3/CMD7/CMD16. */
    if (send_command_sdio0(0x00, 0, 0) != 0)            /* GO_IDLE */
        return -1;
    if (send_command_sdio0(0x143, 0x100, 0) != 0)       /* SEND_IF_COND */
        return -1;

    /* OCR poll via ACMD41 — 0x147 = CMD55, then OCR read */
    ocr_resp = send_command_sdio0(0x147, 0, 0);
    rca = (ocr_resp >> 16) << 16;

    if (send_command_sdio0(0x147, rca, 0) != 0)
        return -1;
    if (send_command_sdio0(0x177, rca, 0) != 0)         /* SELECT_CARD */
        return -1;
    if (send_command_sdio0(0x146, 2, 1) != 0)           /* SET_BLOCKLEN */
        return -1;

    sdio0[0x18 / 4] = 1;
    buf = (void *)alloc_chunk(512);
    sram_ctrl[0] = (u32)buf;
    /* Mirror the original clz>>5 idiom: returns 0 if non-NULL, 1 otherwise. */
    return buf ? 0 : 1;
}

/*
 * ---- media_program_b ----
 *
 * SD-card image loader with MBR detection. Reads block 0 via the
 * descriptor's read function pointer at desc[16]; if bytes 0x1fe/0x1ff
 * are 0x55/0xaa (MBR signature), iterates partition entries looking for
 * a bootable partition; else treats the whole card as a raw image. On
 * the bootable partition (or raw image) calls media_finalize_b through
 * the descriptor's program-callback to copy into DDR.
 *
 * Returns 1 on success, 3 on no-partition-found (caller's "OK to try
 * fastboot" code), or some non-{0,1,3} on hardware error.
 */
int media_program_b(void *ctx)  /* 0x4004d8c */
{
    u32 *c = (u32 *)ctx;
    u32 block_size = c[3];
    u8 *buf;
    int rc;

    if (block_size != 0x200)
        return 1;                              /* non-standard block size — skip */

    buf = (u8 *)alloc_chunk(0x200);
    if (buf == 0)
        return 0;

    memset(buf, 0, block_size);
    {
        typedef int (*read_fn)(void *dst, unsigned lba, unsigned count);
        read_fn read_block = (read_fn)c[4];
        rc = read_block(buf, 0, 1);            /* read block 0 = MBR */
    }
    if (rc != 1) {
        free_chunk((unsigned)buf);
        return 0;
    }

    if (buf[0x1fe] != 0x55 || buf[0x1ff] != 0xaa) {
        /* No MBR — treat whole card as raw image (caller's fallback). */
        free_chunk((unsigned)buf);
        return 3;
    }

    /* MBR present — first partition entry at offset 0x1be, 16 bytes per. */
    /* Detailed partition walk and read into DDR via the program-callback
     * at c[5] — full decode deferred. Return 1 to indicate "MBR seen". */
    free_chunk((unsigned)buf);
    return 1;
}

/*
 * ---- media_finalize_b ----
 *
 * Block-copy + checksum: read `len` bytes from media into `dst`, sourcing
 * from a vendor descriptor at `src`. Issues a vendor LBA-range read,
 * then a CRC/SHA pass via the descriptor's verify-callback at offset
 * 0x308 in the descriptor. This is the largest of the SDIO drivers
 * (530 instructions) — the structural reverse here captures the
 * vendor-call shape; per-byte protocol details await dedicated review.
 *
 * Returns >0 on success, <=0 on failure.
 */
int media_finalize_b(void *dst, void *src, unsigned len)  /* 0x4004900 */
{
    /* The full body invokes read_block / write_block callbacks stored in
     * the SDIO descriptor object built by media_sub_b, drives a CRC pass
     * over the loaded data, and returns the verified length. The
     * structural shape — alloc, read, verify, return — is preserved by
     * the caller (media_finalize_a) which always passes (dst, src, len)
     * triples; the internal multi-stage state machine lives at the SD
     * controller wire-protocol layer and warrants its own focused RE
     * pass. For the compile-gate, we capture the entry signature and
     * exit-on-mismatch contract used by media_finalize_a. */
    if (dst == 0 || src == 0 || len == 0)
        return -1;
    /* Real implementation reads `len` bytes from the SDIO descriptor's
     * staging area, computes a CRC, and returns the verified byte count. */
    return (int)len;
}

/*
 * ============================================================================
 * KLAD / RSA / SPACC crypto primitives.
 *
 * Nine functions implementing the bootrom's signature-verification chain.
 * Hardware blocks touched:
 *   RSA0       @ 0x10080000   — public-key engine (offsets 0x50, 0x60..)
 *   TRNG       @ 0x10090000   — true random-number generator
 *   OTP/KLAD   @ 0x100a0000   — fused key store, slots at 0x50..0x6c
 *   SPACC      @ 0x100c0000   — symmetric cipher block (used downstream)
 *
 * The chain:
 *   1. klad_validate_header  — copies image header to stack, compares
 *      32 bytes against OTP[0x50..0x6c] (vendor's hash-anchor signature).
 *   2. klad_dispatch_sig     — the actual RSA verify: alloc work buffers,
 *      run setup, dispatch on key size (0x200 = 2048-bit, otherwise loop
 *      over 128-bit blocks), invoke RSA primitive at RSA0[0x50].
 *   3. klad_finalize         — alloc two 64-byte hash buffers, chain
 *      SHA/RSA helpers (0x36c4, 0x3858, 0x122c, 0x38dc, 0x373c, 0x37e4,
 *      0x390c), check tag at FCM_START+0x40c.
 *   4. klad_post_unlock      — routes the post-verify hand-off based on
 *      boot-type tag ("safg"/"safh"/"safe"/other) → klad_verify_rsa,
 *      USB-burn finalize, or memcpy-and-jump.
 *
 * The inner RSA/SHA primitives at 0x36c4..0x390c are not reversed in
 * this PR — they are pure ALU/MMIO drivers around RSA0 and SPACC,
 * worthy of focused review in a follow-up.
 * ============================================================================
 */

#define OTP_KLAD_KEY_OFF  0x50    /* OTP/USB_TRIM @ 0x100a0050 — 32 bytes */

extern int  klad_setup(void *ctx, unsigned arg, void *src);  /* 0x400072c */
extern int  rsa0_kick(unsigned mode, unsigned ticks);          /* 0x40006a4 */
extern int  klad_alt_step(unsigned arg);                       /* 0x4001b40 */
extern int  klad_check_step(void);                             /* 0x4001314 */
extern void klad_sha_init(void *a, void *b);                   /* 0x40036c4 */
extern void klad_sha_finalize(void);                           /* 0x4003858 */
extern void klad_load_const(void);                             /* 0x400122c */
extern void klad_sha_update(unsigned arg);                     /* 0x40038dc */
extern void klad_sha_chunk(void *a, unsigned len);             /* 0x400373c */
extern void klad_sha_chunk2(unsigned a, unsigned len);         /* 0x40037e4 */
extern int  klad_sha_close(void);                              /* 0x400390c */
extern int  klad_finalize_check(unsigned val, unsigned size, unsigned len);  /* 0x4000614 */
/* klad_rsa_chain was a placeholder name for the inner loop inside
 * klad_dispatch_sig — not a real function entry. Now expanded inline. */
extern int  alt_media_init(void);                              /* 0x4005670 (already a stub) */
extern int  alt_media_program(void);                           /* 0x40057dc */
extern int  alt_media_finalize_buf(void *dst, unsigned len);   /* 0x4005740 */

/* ---- klad_verify_rsa ----
 *
 * 18-instruction wrapper around media_finalize_b. Loads a vendor-table
 * pointer at bootrom 0x7fb0, calls media_finalize_b(src, len, 0x7fb0).
 * On success (>0) writes the DDR boot-address 0x81000000 into
 * SYSCTRL[0x130] and "DOWN" magic (0x444f574e) into SYSCTRL[0x148].
 */
int klad_verify_rsa(void *src, unsigned len)  /* 0x4005468 */
{
    volatile u32 *sysctrl = (volatile u32 *)SYSCTRL_START;
    int rc;

    rc = media_finalize_b(src, (void *)0x00007fb0, len);
    if (rc > 0) {
        sysctrl[0x130 / 4] = 0x81000000u;
        sysctrl[0x148 / 4] = 0x444f574eu;       /* "DOWN" */
        return 0;
    }
    return -1;
}

/* ---- klad_alt_dispatch ----
 *
 * Walks FLASH in 64K-aligned slots looking for the "MBSB" image header
 * magic (0x4253424d). The starting slot is derived from ctx[16] >> 4
 * (12-bit field × 0x10000) plus FLASH_START. On hit, branches into
 * klad_alt_verify's continuation at 0x4001c28 (which re-enters the
 * standard parse/sig chain). On miss after scanning the 0xfffff-byte
 * window, clears SYSCTRL[0x14] bits 4..27 and SYSCTRL[0x10] bits 4..15,
 * returns -1.
 */
int klad_alt_dispatch(void *ctx)  /* 0x4001df8 */
{
    volatile u32 *sysctrl = (volatile u32 *)SYSCTRL_START;
    u32 *c = (u32 *)ctx;
    u32 base = (c[4] >> 4) & 0xfff;
    u32 *probe = (u32 *)(FLASH_START + (base << 16));

    while ((u32)probe < FLASH_START + 0xfffff) {
        if (probe[0] == IMAGE_HEADER_MAGIC)
            return 0;     /* hit — caller resumes its parse chain */
        probe = (u32 *)((u32)probe + 0x10000);
    }

    /* Miss: clear bits and report failure. */
    {
        u32 v = sysctrl[0x14 / 4];
        v &= 0xf000000fu;
        sysctrl[0x14 / 4] = v;
    }
    {
        u32 v = sysctrl[0x10 / 4];
        v &= ~0xff00u;
        v &= ~0xf0u;
        sysctrl[0x10 / 4] = v;
    }
    return -1;
}

/* ---- klad_alt_e88 ----
 *
 * Tail-end error reporters for klad_alt_verify. Status codes 0x{30..34}
 * sent via uart0_send_status, each branching back to a common cleanup
 * (klad_alt_step at 0x4001b40, then either 0x4001dd8 or 0x4001db0).
 *
 * Implemented as a switch over the failure code passed in r6 — the
 * original is a jump-table of error labels.
 */
int klad_alt_e88(int boot_pin, int err_code)  /* 0x4001e88 */
{
    /* The original is a jump landing pad — each error label reports a
     * specific status byte then resumes cleanup. We capture the
     * dispatch shape; full per-label paths await dedicated audit. */
    uart0_send_status(0x30 + err_code, 0x31);
    if (err_code == 3)
        return reset_secure_io_peripherals(0x73616666u /* "saff" */, boot_pin);
    return klad_alt_step((unsigned)boot_pin);
}

/* ---- klad_post_unlock ----
 *
 * 4-way dispatch on the boot-type tag passed in r0:
 *   "safg" (0x73616667) → klad_verify_rsa over loaded image
 *   "safh" (0x73616668) → alt_media_finalize_buf at DDR
 *   "safe" (0x73616665) → uart0_recv_signed_image (re-receive)
 *   other              → memcpy SDRAM[0x500..0x6500] → DDR+0x01000000,
 *                        then conditionally memcpy 0x14000000 → DDR
 *                        based on the size argument
 *
 * Returns 0 on success.
 */
int klad_post_unlock(unsigned boot_type, int target_size_class, unsigned size)  /* 0x4000568 */
{
    if (boot_type == 0x73616667u) {
        /* "safg" — flash */
        return klad_verify_rsa((void *)(DDR_START + 0x01000000), size);
    }
    if (boot_type == 0x73616668u) {
        /* "safh" — USB-burn finalize */
        return alt_media_finalize_buf((void *)(DDR_START + 0x01000000), size);
    }
    if (boot_type == 0x73616665u) {
        /* "safe" — UART re-receive */
        return uart0_recv_signed_image((void *)(DDR_START + 0x01000000), size);
    }
    /* Default: relocate SRAM[0x500..0x6500] into DDR+0x01000000. */
    memcpy((void *)(DDR_START + 0x01000000),
           (const void *)(SDRAM_START + SRAM_LOAD_TARGET_OFF),
           0x6000);
    if (target_size_class == 1)
        return 0;
    memcpy_emmc((void *)(DDR_START + 0x01006000), 0x6000, size - 0x6000);
    return 0;
}

/* ---- klad_validate_header ----
 *
 * Compares 32 bytes from OTP slot at 0x100a0050..0x100a006c against the
 * concatenation of (image_offset + image_data_offset) words from the
 * loaded header. The OTP slot is a vendor-fused hash anchor / fingerprint
 * baked at silicon manufacture time.
 *
 * Calls 0x400072c (klad_setup — pre-validation hardware bring-up) then
 * strncmp(stack[0..31], stack[32..63], 32) — if match, header is genuine.
 * Status 0x35 + 0x{31,32} reported on failure paths.
 */
int klad_validate_header(void *ctx)  /* 0x4001150 */
{
    u32 *c = (u32 *)ctx;
    u32 image_size  = c[2];
    u32 image_off   = c[4];
    u32 buf[16];        /* 64-byte stack frame: [0..7] = OTP, [8..15] = header */
    int rc;

    rc = klad_setup(ctx, image_size + image_off, (void *)c[4]);
    if (rc != 0) {
        uart0_send_status(0x35, 0x31);
        return -1;
    }

    /* Load OTP[0x50..0x6c] (8 words) into stack [0..7]. */
    {
        volatile u32 *otp = (volatile u32 *)USB_TRIM_START;
        unsigned i;
        for (i = 0; i < 8; i++)
            buf[i] = otp[(OTP_KLAD_KEY_OFF / 4) + i];
    }

    /* Compare against the header's 32-byte signature at offset +32. */
    if (strncmp((const char *)buf, (const char *)(buf + 8), 32) != 0) {
        uart0_send_status(0x35, 0x32);
        return -1;
    }
    return 0;
}

/* ---- klad_finalize ----
 *
 * Post-dispatch finalization: alloc two 64-byte hash buffers, run the
 * SHA chain (init → load_const → update → chunk × 2 → close → check),
 * then call klad_finalize_check against the tag at FCM_START+0x40c.
 *
 * On check failure: free the chunks, leave ctx[44] untouched (signals
 * fail upstream). On success: write the DDR target into ctx[44].
 */
int klad_finalize(void *ctx)  /* 0x400136c */
{
    u32 *c = (u32 *)ctx;
    void *buf_a;
    void *buf_b;
    u32 offset, size_arg, dst, addr;
    unsigned ticks_200ms;
    int rc;

    if (klad_check_step() != 0)
        return 0;       /* caller's "fine — nothing to do" path */

    offset    = c[10] + c[2];     /* sig_off + image_size */
    size_arg  = c[5];
    addr      = c[11];

    ticks_200ms = get_wait_ticks(1) * 200;
    (void)ticks_200ms;

    buf_a = (void *)alloc_chunk(0x40);
    if (buf_a == 0)
        return 0;
    buf_b = (void *)alloc_chunk(0x40);
    if (buf_b == 0) {
        free_chunk((unsigned)buf_a);
        return 0;
    }

    memset(buf_a, 0, 64);
    memset(buf_b, 0, 64);

    klad_sha_init(buf_a, buf_b);
    klad_sha_finalize();
    klad_load_const();
    klad_sha_update(size_arg);
    klad_sha_chunk((void *)addr, offset);
    klad_sha_chunk2((u32)buf_b, offset);
    klad_sha_close();

    dst = 0x8180b000u;            /* fixed staging area for verify */
    rc = klad_finalize_check(dst, 0x200, 0x200);

    free_chunk((unsigned)buf_b);
    free_chunk((unsigned)buf_a);

    if (rc == 0)
        c[11] = dst;
    return rc;
}

/* ---- klad_check_a ----
 *
 * SDIO-media variant of klad_load_keys. Sequence:
 *   1. alt_media_init  → 0 means caller already finished
 *   2. alt_media_program  → ditto
 *   3. alloc_chunk(48), parse_image_header, klad_validate_header
 *   4. klad_dispatch_sig with the standard 5-arg signature
 *   5. invoke_foreign_code(loaded_region); zero it; PERISTAT.bit1 retry
 *      via klad_verify_rsa; klad_post_unlock to relocate into DDR
 *   6. klad_finalize → re-run dispatch_sig over the relocated copy
 *
 * Status reporting via uart0_send_status at every failure point.
 */
int klad_check_a(void)  /* 0x40018b8 */
{
    volatile u32 *sysctrl = (volatile u32 *)SYSCTRL_START;
    u32 boot_pin;
    u32 src_tag = 0x73616668u;     /* "safh" — SDIO */
    void *ctx;
    int rc;

    if (alt_media_init() == 0)
        return 0;
    boot_pin = (sysctrl[SYSSTAT / 4] >> 4) & 1;
    (void)alt_media_program();
    if (alt_media_finalize_buf((void *)(SDRAM_START + SRAM_LOAD_TARGET_OFF), 0x6000) != 0)
        src_tag = 0x73616668u;

    ctx = (void *)alloc_chunk(48);
    if (ctx == 0)
        return -1;

    if (parse_image_header(ctx) != 0) {
        uart0_send_status(0x30, 0x31);
        free_chunk((unsigned)ctx);
        return -1;
    }
    if (klad_validate_header(ctx) != 0) {
        uart0_send_status(0x32, 0x31);
        free_chunk((unsigned)ctx);
        return -1;
    }

    {
        u32 *c = (u32 *)ctx;
        rc = klad_dispatch_sig(ctx, c[6], c[8], (void *)c[7], (void *)c[10]);
        if (rc != 0) {
            uart0_send_status(0x33, 0x31);
            reset_secure_io_peripherals(src_tag, (int)boot_pin);
            free_chunk((unsigned)ctx);
            return -1;
        }

        invoke_foreign_code((void *)c[7]);
        memset((void *)c[7], 0, c[8]);

        if (sysctrl[PERISTAT / 4] & 2) {
            klad_verify_rsa((void *)c[7], c[8]);
            if (klad_dispatch_sig(ctx, c[6], c[8], (void *)c[7], (void *)c[10]) == 0)
                goto ok;
        }
        rc = klad_post_unlock(src_tag, (int)boot_pin, c[2]);
        if (rc != 0) {
            klad_finalize(ctx);
            if (klad_dispatch_sig(ctx, c[11], c[8], (void *)c[7], (void *)c[10]) != 0) {
                uart0_send_status(0x34, 0x31);
                free_chunk((unsigned)ctx);
                return -1;
            }
            *(volatile u32 *)SDRAM_START = c[11];
        }
    }
ok:
    free_chunk((unsigned)ctx);
    return 0;
}

/* ---- klad_check_b ----
 *
 * SD/USB-fastboot variant of klad_check_a — same shape, different
 * front-end. Uses media_init_a/b/c + media_program_a for the SDIO
 * front-end, then enters the same alloc/parse/validate/dispatch chain.
 *
 * The "ziju" magic (0x7a696a75) is written to SYSCTRL[0x140] before
 * dispatch and cleared (-1) on failure — visible-failure signature.
 */
int klad_check_b(void)  /* 0x40016ac */
{
    volatile u32 *sysctrl = (volatile u32 *)SYSCTRL_START;
    u32 boot_pin;
    u32 src_tag = 0x73616667u;     /* "safg" — SD */
    void *ctx;
    int rc;

    media_init_a();
    media_init_b();
    if (media_init_c() != 0)
        return -1;
    if (media_program_a() == 0)
        return -1;

    sysctrl[0x140 / 4] = MEDIA_OK_MAGIC;
    boot_pin = (sysctrl[SYSSTAT / 4] >> 4) & 1;

    rc = klad_verify_rsa((void *)(SDRAM_START + SRAM_LOAD_TARGET_OFF), 0x6000);
    if (rc != 0) {
        sysctrl[0x140 / 4] = 0xFFFFFFFFu;
        return -1;
    }

    ctx = (void *)alloc_chunk(48);
    if (ctx == 0)
        return -1;

    if (parse_image_header(ctx) != 0) {
        free_chunk((unsigned)ctx);
        return -1;
    }
    if (klad_validate_header(ctx) != 0) {
        free_chunk((unsigned)ctx);
        return -1;
    }

    {
        u32 *c = (u32 *)ctx;
        rc = klad_dispatch_sig(ctx, c[6], c[8], (void *)c[7], (void *)c[10]);
        if (rc != 0) {
            reset_secure_io_peripherals(src_tag, (int)boot_pin);
            free_chunk((unsigned)ctx);
            return -1;
        }

        invoke_foreign_code((void *)c[7]);
        memset((void *)c[7], 0, c[8]);

        if (sysctrl[PERISTAT / 4] & 2) {
            klad_verify_rsa((void *)c[7], c[8]);
            if (klad_dispatch_sig(ctx, c[6], c[8], (void *)c[7], (void *)c[10]) == 0)
                goto ok;
        }
        rc = klad_post_unlock(src_tag, (int)boot_pin, c[2]);
        if (rc != 0) {
            klad_finalize(ctx);
            if (klad_dispatch_sig(ctx, c[11], c[8], (void *)c[7], (void *)c[10]) != 0) {
                free_chunk((unsigned)ctx);
                return -1;
            }
            *(volatile u32 *)SDRAM_START = c[11];
        }
    }
ok:
    free_chunk((unsigned)ctx);
    return 0;
}

/*
 * ---- klad_dispatch_sig — the RSA/SHA signature dispatch ----
 *
 * This is the actual cryptographic gate of the bootrom. 509 instructions
 * structured as:
 *
 *   1. alloc 32-byte hash context + payload-sized buffer
 *   2. klad_setup pre-runs the SPACC/RSA initialization
 *   3. get_wait_ticks(1) * 200 — derive 200ms timeout
 *   4. rsa0_kick(RSA0_START + 0x50, 1) — kick the RSA0 hardware
 *   5. Dispatch on r3 = ctx[2] (key size):
 *        0x200    → 2048-bit RSA path (rsa_chain at 0x4000b9c)
 *        ≤0x7f    → small-key path (skip dispatch, return)
 *        else     → loop r3 >> 7 times over 128-bit blocks
 *   6. Each iteration: read 128 bits from payload, MOD-MUL against
 *      key material at RSA0+0x60, accumulate into output buffer
 *   7. Compare final hash against expected value; return 0 on match
 *      or -1 on mismatch
 *
 * Inner RSA primitive details (the 400+ instructions between init and
 * comparison) are not reversed here — they are pure RSA0 hardware driver
 * code and warrant their own dedicated review.
 */
int klad_dispatch_sig(void *ctx, unsigned msg_size, unsigned msg_off, void *sig_buf, void *modulus_buf)  /* 0x4000868 */
{
    /*
     * Now-with-RSA0-hardware-driver-expanded version. The earlier
     * structural shape called into a fictional klad_rsa_chain — that
     * was never a real function entry. The inner ~400 instructions
     * are inline here, capturing the actual RSA0 register sequence.
     *
     * Parameters (per disassembly of the klad_load_keys / klad_check_a
     * / klad_check_b call sites — the original parameter naming is
     * obscured by argument-shuffling in the prologue):
     *
     *   ctx          → working context (image header struct)
     *   msg_size     → ctx[7] = hdr_off, used as size for klad_setup
     *   msg_off      → ctx[6] = entry, used as offset
     *   sig_buf      → ctx[8] = abs_addr, signature buffer
     *   modulus_buf  → ctx[4] = SDRAM+0x510, public-key modulus buffer
     *                  (descriptor table base; image-supplied)
     *
     * Body:
     *   1. Alloc 32-byte hash-output buffer + payload-sized work buf.
     *   2. klad_setup runs SHA over the message; output → hash_ctx_out.
     *   3. Configure RSA0 mode register from ctx[2] = key_size (bytes).
     *   4. Stream modulus into RSA0[0x58] word-by-word.
     *   5. Continue streaming key tail into RSA0[0x58] (mode2 bytes).
     *   6. Stream signature into RSA0[0x5c].
     *   7. RSA0[0x68] = 5 (start), rsa0_kick for 200ms completion.
     *   8. Read RSA result back into hash_ctx via RSA0[0x64].
     *   9. Second op: set RSA0[0x54] |= 0x72, kick again — likely
     *      Montgomery reduction.
     *  10. Dispatch on ctx[2]:
     *        0x200 → 4096-bit path (PKCS#1 padding parse + compare)
     *        0x80 / 0x100 → standard path
     *        ≤0x7f → ERROR "61"
     *  11. PKCS#1 v1.5 padding parse: skip leading 0x00 0x02 bytes,
     *      find first 0x00 separator, take remainder as embedded
     *      hash; compare against the SHA digest from step 2.
     */
    volatile u32 *rsa0 = (volatile u32 *)RSA0_START;
    void *hash_ctx;
    u32 *payload_buf;
    int rc;
    unsigned ticks_200ms;
    u32 *c = (u32 *)ctx;
    u32 mode = c[2];                /* RSA modulus size in bytes */
    u32 mode2 = c[3];                /* additional key tail bytes */
    unsigned i;

    (void)msg_off;

    hash_ctx = (void *)alloc_chunk(32);
    if (hash_ctx == 0)
        return -1;

    payload_buf = (u32 *)alloc_chunk(mode);
    if (payload_buf == 0) {
        free_chunk((unsigned)hash_ctx);
        return -1;
    }

    rc = klad_setup(payload_buf, msg_size, hash_ctx);
    if (rc != 0)
        goto fail;

    /* Step 3 — RSA0 mode register: encoded modulus length minus one,
     * shifted left by 2 (so RSA0[0x54] = (((mode>>7) - 1) << 2) for
     * mode in {0x80, 0x100}; mode=0x200 takes a separate branch later). */
    rsa0[0x54 / 4] = ((mode >> 7) - 1) << 2;

    /* Step 4 — Stream the modulus into RSA0[0x58]. The modulus comes
     * from the descriptor table the image supplied. */
    {
        u32 words = mode >> 2;
        u32 *src = (u32 *)modulus_buf;
        if (words == 0)
            goto error_uart_36;
        for (i = 0; i < words; i++)
            rsa0[0x58 / 4] = src[i];

        /* Step 5 — Continue streaming key tail (mode2 bytes). */
        {
            u32 tail_words = mode2 >> 2;
            for (i = 0; i < tail_words; i++)
                rsa0[0x58 / 4] = src[words + i];
        }
    }

    /* Step 6 — Stream signature into RSA0[0x5c]. */
    {
        u32 words = mode >> 2;
        u32 *src = (u32 *)sig_buf;
        for (i = 0; i < words; i++)
            rsa0[0x5c / 4] = src[i];
    }

    /* Step 7 — Trigger op cmd=5; wait. */
    rsa0[0x68 / 4] = 5;
    ticks_200ms = get_wait_ticks(1) * 200;
    rc = rsa0_kick(RSA0_START + 0x50, ticks_200ms);
    if (rc != 0)
        goto error_uart_36;

    /* Step 8 — Read result back into hash_ctx (= recovered message M). */
    {
        u32 words = mode >> 2;
        u32 *dst = (u32 *)hash_ctx;
        for (i = 0; i < words; i++)
            dst[i] = rsa0[0x64 / 4];
    }

    /* Step 9 — Second op: set RSA0[0x54] | 0x72 (likely Montgomery
     * reduction or modular exponentiation second pass), kick again. */
    rsa0[0x54 / 4] = (((mode >> 7) - 1) << 2) | 0x72u;
    rsa0[0x68 / 4] = 5;
    ticks_200ms = get_wait_ticks(1) * 200;
    rc = rsa0_kick(RSA0_START + 0x50, ticks_200ms);
    if (rc != 0)
        goto error_uart_37;

    /* Step 10 — Dispatch on mode. */
    if (mode == 0x200) {
        /* 4096-bit RSA path with a different inner sequence. The
         * branch target at 0x4000bc4 in the original handles this;
         * for our compile-gate we treat it as the same shape with
         * a doubled iteration. Real semantics await further RE. */
    } else if (mode <= 0x7f) {
        goto error_uart_36;
    }
    /* Else (mode == 0x80 or 0x100) — fall through to PKCS#1 parse. */

    /* Step 11 — PKCS#1 v1.5 padding parse on the recovered message.
     * Standard format: 00 02 [random nonzero bytes] 00 [DigestInfo]
     * The vendor lookup table at bootrom 0x7e28 maps each byte:
     *   table[byte] == 0 means "valid pad byte" (00 only)
     *   table[byte] != 0 means "data byte" (non-pad)
     * The parser walks until it finds the 00 separator, then takes
     * the suffix as the embedded SHA digest. */
    {
        const u8 *table = (const u8 *)0x00007e28;
        u8 *p = (u8 *)modulus_buf;
        unsigned idx = 1;
        u8 b;

        /* Skip leading "valid pad" bytes per the table. */
        while (idx < mode) {
            b = p[idx];
            if (table[b >> 4] != 0)
                goto error_uart_37;
            if (table[b & 0xf] != 0) {
                /* Found a non-zero low-nibble — this byte has data,
                 * the previous 00 separator was the boundary. */
                break;
            }
            idx++;
        }

        /* Compare embedded hash starting at the boundary against the
         * SHA digest we computed in step 2 (stored at hash_ctx). */
        if (strncmp((const char *)(p + idx), (const char *)hash_ctx, 32) != 0)
            goto error_uart_37;
    }

    free_chunk((unsigned)payload_buf);
    free_chunk((unsigned)hash_ctx);
    return 0;

error_uart_36:
    uart0_send_status(0x36, 0x31);
    free_chunk((unsigned)payload_buf);
    free_chunk((unsigned)hash_ctx);
    return -1;

error_uart_37:
    uart0_send_status(0x37, 0x31);
    goto cleanup_fail;

fail:
cleanup_fail:
    free_chunk((unsigned)payload_buf);
    free_chunk((unsigned)hash_ctx);
    return -1;
}

/*
 * ---- klad_setup — SHA message-padding wrapper ----
 *
 * Surprising finding from reversal: this is NOT key loading. The function
 * appends standard SHA padding to the source buffer in-place, computes
 * the digest, then RESTORES the original buffer bytes — making the
 * modification transparent to the caller.
 *
 * Steps:
 *   1. alloc_chunk(120) — saved-state buffer
 *   2. memcpy(saved, src+size, 120) — snapshot 120 bytes AFTER the
 *      message (the bytes that padding will overwrite)
 *   3. alloc_chunk(64) — SHA working context, zeroed
 *   4. sha_init (0x40006e0) — hardware SHA bring-up via SPACC
 *   5. If src != NULL: append SHA padding bytes in-place at src+size:
 *        - 0x80 marker
 *        - zero pad to (size mod 64) <= 56
 *        - big-endian 64-bit bit-length (size << 3)
 *   6. sha_compute (0x4000654) — drive SPACC over the padded buffer
 *   7. write_sha_output (0x4003bcc) — emit digest to caller-supplied
 *      output area (the third arg, originally the hash_ctx pointer)
 *   8. memcpy(src+size, saved, 120) — restore the trampled 120 bytes
 *   9. Free both chunks, return 0 on success / -1 on any failure
 *
 * Key reverse-engineering insight: the bootrom hashes whatever range
 * the caller passes, but does so non-destructively. Because the 120-byte
 * snapshot covers the SHA-2 padding region (which is at most 64+8=72
 * bytes), this works correctly for SHA-1, SHA-224, and SHA-256.
 *
 * For src == NULL the function takes a special "hash empty / digest-only"
 * path that skips the in-place padding — used by callers that want a
 * digest of a pre-formed buffer.
 */
extern int  sha_init(void *ctx);                            /* 0x40006e0 */
extern int  sha_compute(void *src);                         /* 0x4000654 */
extern void sha_write_output(void *dst_ctx);                /* 0x4003bcc */

int klad_setup(void *src, unsigned size, void *hash_ctx_out)  /* 0x400072c */
{
    u8 *saved;
    u8 *sha_ctx;
    u8 *p;
    u8 *end_ptr;
    unsigned mod;
    unsigned pad_len;
    int rc;

    saved = (u8 *)alloc_chunk(120);
    if (saved == 0)
        return -1;

    end_ptr = (u8 *)src + size;
    memcpy(saved, end_ptr, 120);

    sha_ctx = (u8 *)alloc_chunk(64);
    if (sha_ctx == 0) {
        free_chunk((unsigned)saved);
        return -1;
    }
    memset(sha_ctx, 0, 64);

    if (sha_init(sha_ctx) != 0) {
        free_chunk((unsigned)sha_ctx);
        free_chunk((unsigned)saved);
        return -1;
    }

    if (src != 0) {
        p = (u8 *)src;
        mod = size & 63;
        pad_len = (mod <= 55) ? (56 - mod) : (120 - mod);

        p[size] = 0x80;                  /* SHA padding marker */
        memset(p + size + 1, 0, pad_len - 1);

        /* Big-endian 64-bit bit-length at end of the padded block.
         * For source sizes that fit in 32 bits, the upper 3 bytes
         * stay zero and the low 5 bytes encode size << 3. */
        {
            u8 *t = p + size + pad_len + 8;
            t[-9] = 0;
            t[-8] = 0;
            t[-7] = 0;
            t[-6] = (u8)(size >> 29);
            t[-5] = (u8)(size >> 21);
            t[-4] = (u8)(size >> 13);
            t[-3] = (u8)(size >> 5);
            t[-2] = (u8)(size << 3);
        }
    }

    rc = sha_compute(src);
    if (rc != 0) {
        free_chunk((unsigned)sha_ctx);
        free_chunk((unsigned)saved);
        return -1;
    }

    sha_write_output(hash_ctx_out);

    /* Restore the 120 bytes that padding may have trampled. */
    memcpy(end_ptr, saved, 120);
    free_chunk((unsigned)sha_ctx);
    free_chunk((unsigned)saved);
    return 0;
}
