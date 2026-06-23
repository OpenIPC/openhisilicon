/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef SC450AI_CMOS_H
#define SC450AI_CMOS_H

#include "ot_common_isp.h"
#include "ot_sns_ctrl.h"
#include "securec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifndef clip3
#define clip3(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
#endif


#define SC450AI_I2C_ADDR    0x60
#define SC450AI_ADDR_BYTE   2
#define SC450AI_DATA_BYTE   1
#define sc450ai_sensor_get_ctx(dev, ctx)   ((ctx) = sc450ai_get_ctx(dev))

#define SC450AI_FULL_LINES_MAX              0x4920
#define SC450AI_FULL_LINES_MAX_2TO1_WDR     0x2490

/******** SC450AI Register Address ********/
#define SC450AI_EXP_ADDR                    0x3E00
#define SC450AI_SEXP_ADDR                   0x3E04
#define SC450AI_AGAIN_ADDR                  0x3E08
#define SC450AI_SAGAIN_ADDR                 0x3E12
#define SC450AI_DGAIN_ADDR                  0x3E06
#define SC450AI_SDGAIN_ADDR                 0x3E10
#define SC450AI_VMAX_ADDR                   0x320E
#define SC450AI_SEXP_MAX_ADDR               0x3E23

#define SC450AI_INCREASE_LINES 0  /* make real fps less than stand fps because NVR require */
#define SC450AI_VMAX_4M_30FPS_12BIT_LINEAR  (1560 + SC450AI_INCREASE_LINES)
#define SC450AI_VMAX_4M_30FPS_2TO1_WDR      (3120 + SC450AI_INCREASE_LINES)

#define SC450AI_FRAME_RATE_MIN              0x9240 /* Min:2.5fps */
#define EXP_OFFSET_LINEAR                   10
#define EXP_OFFSET_WDR                      18
#define SEXP_OFFSET_WDR                     11
#define SC450AI_SEXP_MAX_DEFAULT            0x178

typedef enum {
    EXPO_L_IDX = 0,
    EXPO_M_IDX,
    EXPO_H_IDX,
    DGAIN_L_IDX,
    DGAIN_H_IDX,
    AGAIN_L_IDX,
    AGAIN_H_IDX,
    VMAX_L_IDX,
    VMAX_H_IDX,
    REG_REV1,
    SEXPO_L_IDX,
    SEXPO_H_IDX,
    SDGAIN_L_IDX,
    SDGAIN_H_IDX,
    SAGAIN_L_IDX,
    SAGAIN_H_IDX,
    SEXP_MAX_L_IDX,
    SEXP_MAX_H_IDX
}sc450ai_reg_index;

typedef enum {
    SC450AI_4M_30FPS_12BIT_LINEAR_MODE = 0,
    SC450AI_4M_30FPS_10BIT_2TO1_VC_MODE = 1,
    SC450AI_MODE_BUTT
} sc450ai_res_mode;

typedef struct {
    td_u32      ver_lines;
    td_u32      max_ver_lines;
    td_float    max_fps;
    td_float    min_fps;
    td_u32      width;
    td_u32      height;
    td_u8       sns_mode;
    ot_wdr_mode wdr_mode;
    const char *mode_name;
} sc450ai_video_mode_tbl;

ot_isp_sns_state *sc450ai_get_ctx(ot_vi_pipe vi_pipe);
ot_isp_sns_commbus *sc450ai_get_bus_info(ot_vi_pipe vi_pipe);

td_void sc450ai_init(ot_vi_pipe vi_pipe);
td_void sc450ai_exit(ot_vi_pipe vi_pipe);
td_void sc450ai_standby(ot_vi_pipe vi_pipe);
td_void sc450ai_restart(ot_vi_pipe vi_pipe);
td_s32  sc450ai_write_register(ot_vi_pipe vi_pipe, td_u32 addr, td_u32 data);
td_s32  sc450ai_read_register(ot_vi_pipe vi_pipe, td_u32 addr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif /* SC450AI_CMOS_H */
