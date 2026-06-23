/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef GFBG_EXT_H
#define GFBG_EXT_H

#include "ot_type.h"
#include "ot_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef struct {
    td_bool (*fb_get_fb_open_state)(td_void);
    td_void (*fb_set_vdp_clk_state)(td_bool enable);
} ot_fb_export_func;

#define ckfn_fb_get_fb_open_state() \
    (func_entry(ot_fb_export_func, OT_ID_FB) != NULL) && \
    (func_entry(ot_fb_export_func, OT_ID_FB)->fb_get_fb_open_state != NULL)
#define call_fb_get_fb_open_state() \
    func_entry(ot_fb_export_func, OT_ID_FB)->fb_get_fb_open_state()

#define ckfn_fb_set_vdp_clk_state() \
    (func_entry(ot_fb_export_func, OT_ID_FB) != NULL) && \
    (func_entry(ot_fb_export_func, OT_ID_FB)->fb_set_vdp_clk_state != NULL)
#define call_fb_set_vdp_clk_state(enable) \
    func_entry(ot_fb_export_func, OT_ID_FB)->fb_set_vdp_clk_state(enable)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* GFBG_EXT_H */