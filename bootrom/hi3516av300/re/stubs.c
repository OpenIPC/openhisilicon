/*
 * Link-only stubs for transitive callees of the reverse-engineered
 * functions in bootloader.c. The frontier is now down to two
 * subsystems plus a handful of new deep helpers surfaced by the
 * media-driver reversal:
 *
 *   - 9 KLAD/RSA crypto primitives (the actual security model)
 *   - 1 UART CRC frame parser
 *   - 4 SDIO0/SD wire-protocol leaves surfaced by media_init_d /
 *     media_program_b / media_sub_a (send_command_sdio0 +
 *     sdio_read_block/write_block + media_sub_a_inner +
 *     media_sub_b_setup)
 *
 * Each remaining stub is large (110-700 instructions) and warrants
 * its own dedicated PR for focused review.
 */

typedef unsigned char  u8;
typedef unsigned int   u32;
typedef unsigned int   size_t;

/*
 * UART frame parser — 269 instructions of CRC + state machine.
 * Drives all five UART fastboot dispatchers in bootloader.c.
 */
int  receive_frame(void *frame)                     { (void)frame; return 0; }  /* 0x4002ee4 */

/*
 * SDIO0/SD wire-protocol leaves surfaced by the newly reversed
 * media_init_d / media_program_b / media_sub_a / media_sub_b.
 */
int  send_command_sdio0(unsigned cmd, unsigned arg, int sync)
    { (void)cmd; (void)arg; (void)sync; return 0; }                       /* 0x4003ce0 */
int  sdio_read_block(void *dst)                     { (void)dst; return 0; }  /* 0x4006cb8 */
int  sdio_write_block(void *src)                    { (void)src; return 0; }  /* 0x4006cc0 */
int  media_sub_a_inner(void *desc)                  { (void)desc; return 0; }  /* 0x4007ae8 */
int  media_sub_b_setup(void *desc)                  { (void)desc; return 0; }  /* 0x4007898 */

/*
 * KLAD / RSA / SPACC / TRNG internal helpers — the actual crypto
 * drivers. Each is large (130-700 instructions) and merits its own
 * PR for proper auditing of the RSA signature check, OTP key fetch,
 * and hash-chain primitives.
 */
int  klad_validate_header(void *ctx)                { (void)ctx; return 0; }  /* 0x4001150 (658 instr) */
int  klad_dispatch_sig(void *ctx, unsigned size, unsigned arg2, unsigned arg3, unsigned arg4)
    { (void)ctx; (void)size; (void)arg2; (void)arg3; (void)arg4; return 0; }  /* 0x4000868 (510 instr) */
int  klad_post_unlock(int slot, int boot_pin)       { (void)slot; (void)boot_pin; return 0; }  /* 0x4000568 (695 instr) */
int  klad_finalize(void *ctx)                       { (void)ctx; return 0; }  /* 0x400136c (523 instr) */
int  klad_check_a(void)                             { return 0; }          /* 0x40018b8 (143 instr) */
int  klad_check_b(void)                             { return 0; }          /* 0x40016ac (131 instr) */
int  klad_verify_rsa(void)                          { return 0; }          /* 0x4005468 (128 instr) */
int  klad_alt_dispatch(void *ctx)                   { (void)ctx; return 0; }  /* 0x4001df8 (663 instr) */
int  klad_alt_e88(void)                             { return 0; }          /* 0x4001e88 (627 instr) */
