/*
 * Final remaining stubs — 6 deep-leaf SDIO protocol helpers that no
 * higher-level boot logic depends on. Each runs inside the SDIO0
 * descriptor walk and handles per-step register sequencing against
 * the controller. None are reachable on the secure-boot path before
 * signature verification; they live below it.
 */

typedef unsigned char  u8;
typedef unsigned int   u32;
typedef unsigned int   size_t;

int  sdio_proto_step_a(void)                        { return 0; }            /* 0x4007578 */
int  sdio_proto_step_b(void)                        { return 0; }            /* 0x4007a5c */
int  sdio_proto_step_c(void)                        { return 0; }            /* 0x4005990 */
int  sdio_proto_step_d(void *ctx)                   { (void)ctx; return 0; } /* 0x40059bc */
int  sdio_proto_step_e(void *ctx, unsigned a, unsigned b, unsigned c)
    { (void)ctx; (void)a; (void)b; (void)c; return 0; }                     /* 0x4006fdc */
int  sdio_proto_step_f(void)                        { return 0; }            /* 0x40075a0 */

/* media_init_alt is already reversed in bootloader.c — provide an
 * alias so the klad_check_a path's reference resolves. */
extern int media_init_alt(void);
int alt_media_init(void) { return media_init_alt(); }
