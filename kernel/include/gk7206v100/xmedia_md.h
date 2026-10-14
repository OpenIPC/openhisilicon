/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef _XMEDIA_MD_H_
#define _XMEDIA_MD_H_

#include "xmedia_ive_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef enum xmedia_md_alg_mode_e {
    XMEDIA_MD_ALG_MODE_BG  = 0x0,
    XMEDIA_MD_ALG_MODE_REF = 0x1,

    XMEDIA_MD_ALG_MODE_BUTT
} xmedia_md_alg_mode_e;
typedef struct xmedia_md_attr_s {
    xmedia_md_alg_mode_e  en_alg_mode;
    xmedia_ive_sad_mode_e en_sad_mode;
    xmedia_ive_sad_out_ctrl_e en_sad_out_ctrl;
    xmedia_u32 u32_width;
    xmedia_u32 u32_height;
    xmedia_u16 u16_sad_thr;
    xmedia_ive_ccl_ctrl_s st_ccl_ctrl;
    xmedia_ive_add_ctrl_s st_add_ctrl;
} xmedia_md_attr_s;


xmedia_s32 xmedia_md_init(xmedia_void);


xmedia_s32 xmedia_md_exit(xmedia_void);


xmedia_s32 xmedia_md_create_chn(xmedia_s32 md_chn, xmedia_md_attr_s *pst_md_attr);


xmedia_s32 xmedia_md_destroy_chn(xmedia_s32 md_chn);


xmedia_s32 xmedia_md_set_chn_attr(xmedia_s32 md_chn, xmedia_md_attr_s *pst_md_attr);


xmedia_s32 xmedia_md_get_chn_attr(xmedia_s32 md_chn, xmedia_md_attr_s *pst_md_attr);


xmedia_s32 xmedia_md_get_bg(xmedia_s32 md_chn, xmedia_ive_dst_image_s *pst_bg);


xmedia_s32 xmedia_md_process(xmedia_s32 md_chn, xmedia_ive_src_image_s *pst_cur, xmedia_ive_src_image_s *pst_ref, xmedia_ive_dst_image_s *pst_sad, xmedia_ive_dst_mem_info_s *pst_blob);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif
