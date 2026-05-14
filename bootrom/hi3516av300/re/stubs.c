/*
 * Link-only stubs for transitive callees of the reverse-engineered
 * functions in bootloader.c — addresses live in the av300 mask-ROM but
 * aren't yet decoded. Kept in a separate translation unit so -O2 in
 * bootloader.c cannot see through them at call sites and dead-code-
 * eliminate the conditionals that test their return values.
 *
 * Each stub will be replaced with its reverse-engineered body in a
 * later PR; callers do not need to change when that happens.
 */

typedef unsigned int u32;
typedef unsigned int size_t;

/* wait/timer leaves */
void wait_timer_0(unsigned ticks)                   { (void)ticks; }       /* 0x40052b4 */

/* memory chunk allocator */
void organize_memory_chunks(void)                   {}                     /* 0x4002a80 */

/* UART fastboot helpers */
int  burn_bootloader(void)                          { return 0; }          /* 0x40034ac */
int  set_boot_target_addr(int boot_pin)             { (void)boot_pin; return 0; }  /* 0x40004b8 */

/* eMMC + SDIO0 helpers */
void reset_emmc(void)                               {}                     /* 0x4004060 */
int  initialize_emmc(void)                          { return 0; }          /* 0x4003de0 */
void memcpy_emmc(void *dst, unsigned off, unsigned len) { (void)dst; (void)off; (void)len; }  /* 0x40040a0 */
void close_sdio0(void)                              {}                     /* 0x400445c */

/* media-load helpers (fastboot + alt paths) */
void media_init_a(void)                             {}                     /* 0x400414c */
void media_init_b(void)                             {}                     /* 0x40041c0 */
int  media_init_c(void)                             { return 0; }          /* 0x40041f0 */
int  media_program_a(void)                          { return 0; }          /* 0x40046c8 */
int  media_finalize_a(void)                         { return 0; }          /* 0x4005384 */
int  media_init_alt(void)                           { return 0; }          /* 0x4005670 */
void media_init_alt_finalize(void)                  {}                     /* 0x40056c0 */

/* image header parser + foreign-code trampoline */
int  parse_image_header(void *dst)                  { (void)dst; return 0; }  /* 0x40001dc */
void invoke_foreign_code(void *fn)                  { (void)fn; }          /* 0x4005348 */

/* UART0 hardware bring-up */
void initialize_uart0_hdwr(void)                    {}                     /* 0x4003318 */
void disable_uart0_hdwr(void)                       {}                     /* 0x4003334 */
void uart0_flush_rx_fifo(void)                      {}                     /* 0x4003338 */

/* KLAD / RSA / SPACC / TRNG security-block helpers */
int  klad_load_keys(void)                           { return 0; }          /* 0x4001470 */
int  klad_verify(void)                              { return 0; }          /* 0x4001f0c */
int  klad_unlock(void)                              { return 0; }          /* 0x4001af4 */
int  klad_alt_verify(void)                          { return 0; }          /* 0x4001bdc */
