/*
 * Link-only stubs for transitive callees of the reverse-engineered
 * functions in bootloader.c. The frontier is now down to two
 * subsystems: the eMMC/SDIO media drivers and the KLAD crypto
 * primitives, each warranting its own dedicated review PR.
 *
 * Signatures match the bootrom call sites; semantic responsibility
 * for the KLAD entry points awaits cross-reference with av300
 * vendor crypto documentation.
 */

typedef unsigned char  u8;
typedef unsigned int   u32;
typedef unsigned int   size_t;

/*
 * Frame parser shared by the four UART fastboot drivers above. CRC
 * checksumming + framing state machine — 269 instructions of its
 * own, deferred for a focused review.
 */
int  receive_frame(void *frame)                     { (void)frame; return 0; }  /* 0x4002ee4 */

/*
 * Media-path drivers — bulk SDIO / eMMC programmer state machines.
 * Each runs 200-500+ instructions and lives in its own subsystem.
 */
int  media_init_d(void)                             { return 0; }          /* 0x400422c */
int  media_program_b(void *ctx)                     { (void)ctx; return 0; }  /* 0x4004d8c */
int  media_finalize_b(void *dst, void *src, unsigned len)
    { (void)dst; (void)src; (void)len; return 0; }                          /* 0x4004900 */
int  media_sub_a(void)                              { return 0; }          /* 0x40077a8 */
unsigned media_sub_b(void)                          { return 0; }          /* 0x40077b8 */
void media_sub_c(void)                              {}                     /* 0x400795c */

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
