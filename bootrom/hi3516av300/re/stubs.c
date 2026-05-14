/*
 * Link-only stubs for the final transitive frontier of the av300
 * bootrom reversal. After this PR, only two categories remain:
 *
 *   1. The CRC-checksummed UART frame parser (receive_frame, 269 instr).
 *   2. Inner crypto/SD primitives — the actual hardware drivers behind
 *      RSA0 (klad_setup, rsa0_kick, klad_rsa_chain, klad_sha_*),
 *      SDIO0 (send_command_sdio0, sdio_read_block/write_block,
 *      media_sub_a_inner, media_sub_b_setup), and a handful of
 *      smaller helpers.
 *
 * Each name reflects role-from-context. Signatures match call sites.
 * Internal RSA hardware register sequences and key material handling
 * are the next focused-audit target.
 */

typedef unsigned char  u8;
typedef unsigned int   u32;
typedef unsigned int   size_t;

/* UART frame parser — 269 instructions of CRC + state machine. */
int  receive_frame(void *frame)                     { (void)frame; return 0; }  /* 0x4002ee4 */

/* SDIO0/SD wire-protocol leaves. */
int  send_command_sdio0(unsigned cmd, unsigned arg, int sync)
    { (void)cmd; (void)arg; (void)sync; return 0; }                       /* 0x4003ce0 */
int  sdio_read_block(void *dst)                     { (void)dst; return 0; }  /* 0x4006cb8 */
int  sdio_write_block(void *src)                    { (void)src; return 0; }  /* 0x4006cc0 */
int  media_sub_a_inner(void *desc)                  { (void)desc; return 0; }  /* 0x4007ae8 */
int  media_sub_b_setup(void *desc)                  { (void)desc; return 0; }  /* 0x4007898 */

/* KLAD/RSA crypto-driver primitives — the deepest hardware-driver
 * layer. klad_dispatch_sig calls these in sequence; reversing them
 * means tracing the exact RSA0 register dance vs SPACC accelerator
 * register dance. */
int  klad_setup(void *ctx, unsigned arg, void *src)
    { (void)ctx; (void)arg; (void)src; return 0; }                        /* 0x400072c */
int  rsa0_kick(unsigned mode, unsigned ticks)
    { (void)mode; (void)ticks; return 0; }                                /* 0x40006a4 */
int  klad_rsa_chain(void *ctx, unsigned key_size, void *payload)
    { (void)ctx; (void)key_size; (void)payload; return 0; }               /* 0x4000aac */
int  klad_check_step(void)                          { return 0; }          /* 0x4001314 */
int  klad_alt_step(unsigned arg)                    { (void)arg; return 0; }  /* 0x4001b40 */

void klad_sha_init(void *a, void *b)                { (void)a; (void)b; }  /* 0x40036c4 */
void klad_sha_finalize(void)                        {}                     /* 0x4003858 */
void klad_load_const(void)                          {}                     /* 0x400122c */
void klad_sha_update(unsigned arg)                  { (void)arg; }         /* 0x40038dc */
void klad_sha_chunk(void *a, unsigned len)          { (void)a; (void)len; }  /* 0x400373c */
void klad_sha_chunk2(unsigned a, unsigned len)      { (void)a; (void)len; }  /* 0x40037e4 */
void klad_sha_close(void)                           {}                     /* 0x400390c */
int  klad_finalize_check(unsigned val, unsigned size, unsigned len)
    { (void)val; (void)size; (void)len; return 0; }                       /* 0x4000614 */

int  alt_media_program(void)                        { return 0; }          /* 0x40057dc */
int  alt_media_finalize_buf(void *dst, unsigned len)
    { (void)dst; (void)len; return 0; }                                   /* 0x4005740 */

/* media_init_alt is already reversed in bootloader.c — provide an
 * alias so the klad_check_a path's reference resolves to the same
 * symbol. (alt_media_init was the old name from klad_check_a's
 * comment text; the real function is media_init_alt.) */
extern int media_init_alt(void);
int alt_media_init(void) { return media_init_alt(); }
