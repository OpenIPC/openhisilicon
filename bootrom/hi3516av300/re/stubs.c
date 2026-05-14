/*
 * Last-resort stubs — sub-leaves surfaced by the SPACC and SDIO
 * driver reversals. After this PR these are the only remaining
 * unreversed entries in the bootrom:
 *
 *   media_sub_a_decode_block    (0x4007c70, called from media_sub_a_inner)
 *   sdio_pio_loop               (0x4006c08, sdio_write_block tail)
 *   sdio_descriptor_init        (0x4006cd8, called from media_sub_b_setup)
 *   media_sub_b_init_substruct  (0x4007398, called from media_sub_b_setup)
 *   media_sub_b_calc_offsets    (0x4007688, called from media_sub_b_setup)
 *   media_sub_b_calc_tail       (0x40078a4, called from media_sub_b_setup)
 *
 * These are tail calls into block-level transfer state machines that
 * have no direct security relevance — they handle USB-style SDIO
 * descriptor walking and DMA queue management. Stubbed to keep the
 * compile-gate happy.
 */

typedef unsigned char  u8;
typedef unsigned int   u32;
typedef unsigned int   size_t;

int  media_sub_a_decode_block(void *ctx, unsigned tag)
    { (void)ctx; (void)tag; return 0; }                                    /* 0x4007c70 */
int  sdio_pio_loop(void *ctx)                       { (void)ctx; return 0; }   /* 0x4006c08 */
int  sdio_descriptor_init(void *ctx)                { (void)ctx; return 0; }   /* 0x4006cd8 */
int  media_sub_b_init_substruct(void *ctx)          { (void)ctx; return 0; }   /* 0x4007398 */
int  media_sub_b_calc_offsets(void *ctx)            { (void)ctx; return 0; }   /* 0x4007688 */
int  media_sub_b_calc_tail(void *ctx)               { (void)ctx; return 0; }   /* 0x40078a4 */

/* media_init_alt is already reversed in bootloader.c — provide an
 * alias so the klad_check_a path's reference resolves. */
extern int media_init_alt(void);
int alt_media_init(void) { return media_init_alt(); }
