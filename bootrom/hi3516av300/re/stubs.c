/*
 * Link-only stubs for transitive callees of the reverse-engineered
 * functions in bootloader.c. Kept in a separate translation unit so
 * -O2 in bootloader.c cannot fold their constant returns and erase
 * the conditional structure of larger callers.
 */

typedef unsigned char  u8;
typedef unsigned int   u32;
typedef unsigned int   size_t;

/* The four biggest functions in the bootrom — saved for a dedicated
 * follow-up PR because each is 140+ instructions on its own. */
int  klad_load_keys(void)                           { return 0; }          /* 0x4001470 */
int  klad_verify(void)                              { return 0; }          /* 0x4001f0c */
int  klad_alt_verify(void)                          { return 0; }          /* 0x4001bdc */
int  initialize_emmc(void)                          { return 0; }          /* 0x4003de0 */

/* Leaves of the newly-reversed functions still to be done. */
int  klad_check_a(void)                             { return 0; }          /* 0x40018b8 */
int  klad_check_b(void)                             { return 0; }          /* 0x40016ac */

void initialize_uart0(void)                         {}                     /* 0x40050c0 */
void disable_uart0(void)                            {}                     /* 0x400512c */
int  uart0_read(void)                               { return 0; }          /* 0x4005170 */
int  uart0_has_rx_data(void)                        { return 0; }          /* 0x40051a0 */
int  uart0_wait_start_frame(void)                   { return 0; }          /* 0x4003410 */
int  uart0_recv_payload(void)                       { return 0; }          /* 0x4003354 */

void wait_long_timer_0(unsigned ms)                 { (void)ms; }          /* 0x4002c84 */
unsigned timer_get_value(void)                      { return 0; }          /* 0x4005234 */
unsigned get_wait_ticks(unsigned ms)                { (void)ms; return 0; }/* 0x4005244 */
void update_timer_0_value(void)                     {}                     /* 0x400524c */

unsigned alloc_chunk(unsigned size)                 { (void)size; return 0; } /* 0x4002b78 */
void free_chunk(unsigned addr)                      { (void)addr; }        /* 0x4002c1c */
void nop_2d74(void)                                 {}                     /* 0x4002d74 */

int  media_init_d(void)                             { return 0; }          /* 0x400422c */
int  media_program_b(void *ctx)                     { (void)ctx; return 0; }  /* 0x4004d8c */
int  media_finalize_b(void *dst, void *src, unsigned len)
    { (void)dst; (void)src; (void)len; return 0; }                          /* 0x4004900 */
int  media_sub_a(void)                              { return 0; }          /* 0x40077a8 */
unsigned media_sub_b(void)                          { return 0; }          /* 0x40077b8 */
void media_sub_c(void)                              {}                     /* 0x400795c */
