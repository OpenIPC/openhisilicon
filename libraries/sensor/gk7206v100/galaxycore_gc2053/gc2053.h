#ifndef __GC2053_H__
#define __GC2053_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GC2053_RES_IS_1080P(w, h) ((w) == 1920 && (h) == 1080)

/*****SC2235 Register Address*****/
#define GC2053_REG_EXP_ADDR_L            0x04   // Shutter-time[7:0]
#define GC2053_REG_EXP_ADDR_H            0x03   // Shutter-time[13:8]
#define GC2053_REG_AGAIN_ADDR_L          0xb3   // Analog-gain[7:0]
#define GC2053_REG_AGAIN_ADDR_H          0xb4   // Analog-gain[9:8]
#define GC2053_REG_DGAIN_ADDR_L          0xb9   // digital-gain[5:0]
#define GC2053_REG_DGAIN_ADDR_H          0xb8   // digital-gain[11:6]
#define GC2053_REG_AUTO_PREGAIN_ADDR_L   0xb2   // auto-pregain-gain[5:0]
#define GC2053_REG_AUTO_PREGAIN_ADDR_H   0xb1   // auto-pregain-gain[9:6]
#define GC2053_REG_STATUS_ADDR           0x90   // slow shutter via framerate or exptime
#define GC2053_REG_VMAX_ADDR_L           0x42   // Vmax[7:0]
#define GC2053_REG_VMAX_ADDR_H           0x41   // Vmax[13:8]
#define GC2053_REG_PRBS_MODE_ADDR        0x38
#define GC2053_REG_OPTICAL_BLACK_ADDR    0x7c

#define GC2053_VMAX_1080P30_LINEAR       0x465

typedef enum{
    GC2053_REG_EXP_L = 0,
    GC2053_REG_EXP_H,
    GC2053_REG_AGC_L,
    GC2053_REG_AGC_H,
    GC2053_REG_DGC_L,
    GC2053_REG_DGC_H,
    GC2053_REG_PREGAIN_L,
    GC2053_REG_PREGAIN_H,
    GC2053_REG_STATUS,
    GC2053_REG_VMAX_H, // 必须得先配置 vmax 高位寄存器，然后再配置低位寄存器
    GC2053_REG_VMAX_L,
    GC2053_REG_L_MAX_NUM,

    //short frame reg info from here
    GC2053_S_REG_EXP_L = GC2053_REG_L_MAX_NUM,
    GC2053_S_REG_EXP_H,
    GC2053_REG_PRBS_MODE,
    GC2053_REG_OPTICAL_BLACK,
    GC2053_REG_MAX_NUM
}gc2053_reg_info;

#define GC2053_FULL_LINES_MAX                 0x3FFF

#ifdef __cplusplus
}
#endif

#endif //__GC2053_H__
