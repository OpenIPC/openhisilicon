/*
 * Final transitive frontier for the av300 bootrom reversal. After
 * this PR the only remaining stubs are 10 hardware-leaf primitives:
 *
 *   1. UART CRC frame parser (receive_frame, 269 instr)
 *   2. SDIO0/SD wire-protocol leaves (5)
 *   3. SPACC accelerator sub-primitives (4) — per-block bring-up
 *      details internal to sha_init / sha_compute
 *
 * Reversing these traces the per-cycle SPACC and SD-card hardware
 * register sequences; they are deep audit targets but not part of
 * the high-level boot logic.
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

/* SPACC accelerator sub-primitives — internal helpers used by
 * sha_init and sha_compute. Each is small (a handful of instructions
 * doing register reads/writes against SPACC_START); reversing them
 * means tracing the SPACC SHA-context bring-up wire layer. */
int  spacc_init(void *ctx)                          { (void)ctx; return 0; }  /* 0x40039a8 */
void spacc_finalize(void)                           {}                       /* 0x4003a74 */
void spacc_chunk_prep(void *ctx, unsigned size)     { (void)ctx; (void)size; }  /* 0x4003ae8 */
void spacc_chunk_run(void)                          {}                       /* 0x4003b68 */

int  alt_media_program(void)                        { return 0; }            /* 0x40057dc */
int  alt_media_finalize_buf(void *dst, unsigned len)
    { (void)dst; (void)len; return 0; }                                      /* 0x4005740 */

/* media_init_alt is already reversed in bootloader.c — provide an
 * alias so the klad_check_a path's reference resolves. */
extern int media_init_alt(void);
int alt_media_init(void) { return media_init_alt(); }
