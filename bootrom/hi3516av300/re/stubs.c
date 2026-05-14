/*
 * Link-only stubs for transitive callees of the reverse-engineered
 * functions in bootloader.c. The frontier has now pushed down to the
 * deep crypto/protocol drivers — what remains here is the genuinely
 * big stuff that warrants dedicated review when reversed.
 *
 * Each name reflects role-from-context. Signatures match the bootrom
 * call sites (return types and argument counts), not necessarily the
 * full semantic.
 */

typedef unsigned char  u8;
typedef unsigned int   u32;
typedef unsigned int   size_t;

/* Single-byte UART0 TX leaf — used by uart0_send_status. */
void uart0_write(int byte)                          { (void)byte; }        /* 0x400514c */

/* UART secure-load helpers (each 200-350 instructions; bulk of the
 * fastboot serial protocol). */
int  uart0_wait_start_frame(void)                   { return 0; }          /* 0x4003410 */
int  uart0_recv_payload(void)                       { return 0; }          /* 0x4003354 */
int  uart0_proto_handshake(void)                    { return 0; }          /* 0x4003580 */
int  uart0_recv_signed_image(void *dst, unsigned size) { (void)dst; (void)size; return 0; }  /* 0x40034e0 */

/* Media-path helpers — bulk SDIO / eMMC programmer state machines.
 * Each runs 200-500+ instructions and lives in its own subsystem. */
int  media_init_d(void)                             { return 0; }          /* 0x400422c */
int  media_program_b(void *ctx)                     { (void)ctx; return 0; }  /* 0x4004d8c */
int  media_finalize_b(void *dst, void *src, unsigned len)
    { (void)dst; (void)src; (void)len; return 0; }                          /* 0x4004900 */
int  media_sub_a(void)                              { return 0; }          /* 0x40077a8 */
unsigned media_sub_b(void)                          { return 0; }          /* 0x40077b8 */
void media_sub_c(void)                              {}                     /* 0x400795c */

/*
 * KLAD / RSA / SPACC / TRNG internal helpers — these are the actual
 * crypto drivers. Each is large (130-700 instructions) and merits its
 * own dedicated PR for proper auditing of the RSA signature check,
 * OTP key fetch, and hash-chain primitives.
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
