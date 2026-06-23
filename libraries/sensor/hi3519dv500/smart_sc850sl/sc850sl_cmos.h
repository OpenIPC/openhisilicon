/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef SC850SL_CMOS_H
#define SC850SL_CMOS_H

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

#define SC850SL_I2C_ADDR    0x60
#define SC850SL_ADDR_BYTE   2
#define SC850SL_DATA_BYTE   1
#define sc850sl_sensor_get_ctx(dev, ctx)   ((ctx) = sc850sl_get_ctx(dev))

#define SC850SL_FULL_LINES_MAX          0x6978 /* 0x6978 = 27000 = Min:2.5fps */
#define SC850SL_FULL_LINES_MAX_2TO1_WDR 0x34BC /* 13500 = Min:10fps */
#define SC850SL_INCREASE_LINES 0  /* make real fps less than stand fps because NVR require */
#define SC850SL_VMAX_8M_30FPS_12BIT_LINEAR     (2250 + SC850SL_INCREASE_LINES) /* 0x8CA */
#define SC850SL_VMAX_8M_30FPS_10BIT_2TO1_WDR   (4500 + SC850SL_INCREASE_LINES)   /* 0x1194 */

/******** SC850SL Register Address ********/
#define SC850SL_EXPO_H_ADDR          0x3E00
#define SC850SL_EXPO_M_ADDR          0x3E01
#define SC850SL_EXPO_L_ADDR          0x3E02

#define SC850SL_AGAIN_H_ADDR         0x3E08
#define SC850SL_AGAIN_L_ADDR         0x3E09

#define SC850SL_DGAIN_H_ADDR         0x3E06
#define SC850SL_DGAIN_L_ADDR         0x3E07

#define SC850SL_SHORT_EXPO_H_ADDR    0x3E22
#define SC850SL_SHORT_EXPO_M_ADDR    0x3E04
#define SC850SL_SHORT_EXPO_L_ADDR    0x3E05

#define SC850SL_SHORT_AGAIN_H_ADDR   0x3E12
#define SC850SL_SHORT_AGAIN_L_ADDR   0x3E13

#define SC850SL_SHORT_DGAIN_H_ADDR   0x3E10
#define SC850SL_SHORT_DGAIN_L_ADDR   0x3E11

#define SC850SL_VMAX_H_ADDR          0x320E
#define SC850SL_VMAX_L_ADDR          0x320F

#define SC850SL_WDR_LOGIC_H_ADDR          0x3230
#define SC850SL_WDR_LOGIC_L_ADDR          0x3231

#define SC850SL_SHORT_EXP_MAX_H_ADDR     0x3E23
#define SC850SL_SHORT_EXP_MAX_L_ADDR     0x3E24

#define SC850SL_LOGIC_ADDR_2     0x363c

#define SC850SL_FRAME_RATE_MIN       0xD2F0 /* Min:2.5fps */
#define EXP_OFFSET_LINEAR            4
#define EXP_OFFSET_WDR_LONG          8
#define EXP_OFFSET_WDR_SHORT         2
#define SC850SL_SEXP_MAX_DEFAULT     0x10A

typedef enum {
    SC850SL_8M_30FPS_12BIT_LINEAR_MODE = 0,
    SC850SL_8M_30FPS_10BIT_2TO1_VC_MODE,
    SC850SL_MODE_BUTT
} sc850sl_res_mode;

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
} sc850sl_video_mode_tbl;

typedef enum {
    EXPO_L_IDX = 0,
    EXPO_M_IDX,
    EXPO_H_IDX,
    AGAIN_L_IDX,
    AGAIN_H_IDX,
    DGAIN_L_IDX,
    DGAIN_H_IDX,
    VMAX_L_IDX,
    VMAX_H_IDX,
    SNS_LOGIC_REG363C,
    REG_MAX_IDX
} sc850sl_linear_reg_index;

typedef enum {
    WDR_EXPO_L_IDX = 0,
    WDR_EXPO_M_IDX,
    WDR_EXPO_H_IDX,
    WDR_AGAIN_L_IDX,
    WDR_AGAIN_H_IDX,
    WDR_DGAIN_L_IDX,
    WDR_DGAIN_H_IDX,
    WDR_VMAX_L_IDX,
    WDR_VMAX_H_IDX,
    WDR_SNS_LOGIC_REG363C,
    WDR_SHORT_EXPO_L_IDX,
    WDR_SHORT_EXPO_M_IDX,
    WDR_SHORT_AGAIN_L_IDX,
    WDR_SHORT_AGAIN_H_IDX,
    WDR_SHORT_DGAIN_L_IDX,
    WDR_SHORT_DGAIN_H_IDX,
    SHORT_EXP_MAX_L_ADDR_IDX,
    SHORT_EXP_MAX_H_ADDR_IDX,
    WDR_LOGIC_L_IDX,
    WDR_LOGIC_H_IDX,
    WDR_REG_MAX_IDX
}sc850sl_wdr_reg_index;


ot_isp_sns_state *sc850sl_get_ctx(ot_vi_pipe vi_pipe);
ot_isp_sns_commbus *sc850sl_get_bus_info(ot_vi_pipe vi_pipe);

td_void sc850sl_init(ot_vi_pipe vi_pipe);
td_void sc850sl_exit(ot_vi_pipe vi_pipe);
td_void sc850sl_standby(ot_vi_pipe vi_pipe);
td_void sc850sl_restart(ot_vi_pipe vi_pipe);


td_s32  sc850sl_write_register(ot_vi_pipe vi_pipe, td_u32 addr, td_u32 data);
td_s32  sc850sl_read_register(ot_vi_pipe vi_pipe, td_u32 addr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif /* SC850SL_CMOS_H */
