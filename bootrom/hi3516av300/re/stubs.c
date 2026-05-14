/*
 * Link-only stubs for transitive callees of the reverse-engineered
 * functions in bootloader.c. Kept in a separate translation unit so
 * -O2 in bootloader.c cannot fold their constant returns and erase
 * the conditional structure of larger callers.
 *
 * The frontier moves one layer deeper with this PR — the four giants
 * (initialize_emmc, klad_load_keys, klad_verify, klad_alt_verify) are
 * now reversed in bootloader.c. The stubs below are the next layer
 * down: KLAD/RSA register-level helpers, eMMC command primitives,
 * SDIO0 init, header-validation chain, and a few more leaves.
 */

typedef unsigned char  u8;
typedef unsigned int   u32;
typedef unsigned int   size_t;

/* UART0 PL011 primitives */
void initialize_uart0(void)                         {}                     /* 0x40050c0 */
void disable_uart0(void)                            {}                     /* 0x400512c */
int  uart0_read(void)                               { return 0; }          /* 0x4005170 */
int  uart0_has_rx_data(void)                        { return 0; }          /* 0x40051a0 */
int  uart0_wait_start_frame(void)                   { return 0; }          /* 0x4003410 */
int  uart0_recv_payload(void)                       { return 0; }          /* 0x4003354 */
int  uart0_proto_handshake(void)                    { return 0; }          /* 0x4003580 */
int  uart0_recv_signed_image(void *dst, unsigned size) { (void)dst; (void)size; return 0; }  /* 0x40034e0 */
void uart0_send_status(int a, int b)                { (void)a; (void)b; }  /* 0x4000320 */

/* Timer leaves */
void wait_long_timer_0(unsigned ms)                 { (void)ms; }          /* 0x4002c84 */
unsigned timer_get_value(void)                      { return 0; }          /* 0x4005234 */
unsigned get_wait_ticks(unsigned ms)                { (void)ms; return 0; }/* 0x4005244 */
void update_timer_0_value(void)                     {}                     /* 0x400524c */

/* Chunk allocator + vendor "no-op marker" */
unsigned alloc_chunk(unsigned size)                 { (void)size; return 0; } /* 0x4002b78 */
void free_chunk(unsigned addr)                      { (void)addr; }        /* 0x4002c1c */
void nop_2d74(void)                                 {}                     /* 0x4002d74 */

/* Media-path helpers still pending */
int  media_init_d(void)                             { return 0; }          /* 0x400422c */
int  media_program_b(void *ctx)                     { (void)ctx; return 0; }  /* 0x4004d8c */
int  media_finalize_b(void *dst, void *src, unsigned len)
    { (void)dst; (void)src; (void)len; return 0; }                          /* 0x4004900 */
int  media_sub_a(void)                              { return 0; }          /* 0x40077a8 */
unsigned media_sub_b(void)                          { return 0; }          /* 0x40077b8 */
void media_sub_c(void)                              {}                     /* 0x400795c */

/*
 * KLAD / RSA / SPACC / TRNG internal helpers — each lives as a real
 * function in the av300 mask-ROM at the address noted. Names reflect
 * usage context in the four reversed giants; semantic responsibilities
 * (which crypto primitive, which OTP slot, which key) await
 * cross-reference with the av300 vendor documentation.
 */
int  klad_validate_header(void *ctx)                { (void)ctx; return 0; }  /* 0x4001150 */
int  klad_init_check(int slot, int boot_pin)        { (void)slot; (void)boot_pin; return 0; }  /* 0x40010ac */
int  klad_dispatch_sig(void *ctx, unsigned size, unsigned arg2, unsigned arg3, unsigned arg4)
    { (void)ctx; (void)size; (void)arg2; (void)arg3; (void)arg4; return 0; }  /* 0x4000868 */
int  klad_post_unlock(int slot, int boot_pin)       { (void)slot; (void)boot_pin; return 0; }  /* 0x4000568 */
int  klad_check_a(void)                             { return 0; }          /* 0x40018b8 */
int  klad_check_b(void)                             { return 0; }          /* 0x40016ac — actually entry of klad_load_keys-shape function */
int  klad_finalize(void *ctx)                       { (void)ctx; return 0; }  /* 0x4001470 fragment 0x400136c */
int  klad_decode_chunk(unsigned a, unsigned b)      { (void)a; (void)b; return 0; }  /* 0x4000358 */

/* eMMC command primitives + low-level helpers */
void configure_emmc_pins(void)                      {}                     /* 0x4003d68 */
int  send_command_emmc(void)                        { return 0; }          /* 0x4003c30 */

/* RSA dispatch entry from klad_verify */
int  klad_verify_rsa(void)                          { return 0; }          /* 0x4005468 */

/* External funcs that klad_alt_verify references */
int  klad_alt_dispatch(void *ctx)                   { (void)ctx; return 0; }  /* 0x4001df8 */
int  klad_alt_e88(void)                             { return 0; }          /* 0x4001e88 */
