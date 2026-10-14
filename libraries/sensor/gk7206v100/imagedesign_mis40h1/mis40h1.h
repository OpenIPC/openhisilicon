#ifndef __MIS40H1_H__
#define __MIS40H1_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

// sensor fps mode
#define MIS40H1_FULL_LINES_MAX      0xFFFF
#define MIS40H1_VMAX_4M_1520_LINEAR 1650

#define MIS40H1_EXP_OFFSET_LINEAR 2

#define MIS40H1_REG_ADDR_EXP_H       0x3100 // Shutter-time[13:8]
#define MIS40H1_REG_ADDR_EXP_L       0x3101 // Shutter-time[7:0]
#define MIS40H1_REG_ADDR_AGAIN_H     0x3106 // Analog-gain[1:0]
#define MIS40H1_REG_ADDR_AGAIN_L     0x3107 // Analog-gain[7:0]
#define MIS40H1_REG_ADDR_DGAIN_H     0x420b // digital-gain[11:6]
#define MIS40H1_REG_ADDR_DGAIN_L     0x420C // digital-gain[5:0]
#define MIS40H1_REG_ADDR_VMAX_H      0x310e // Vmax[13:8]
#define MIS40H1_REG_ADDR_VMAX_L      0x310f // Vmax[7:0]
#define MIS40H1_REG_ADDR_NUM1        0x3008
typedef enum {
    MIS40H1_REG_EXP_H = 0,
    MIS40H1_REG_EXP_L,
    MIS40H1_REG_AGAIN_H,
    MIS40H1_REG_AGAIN_L,
    MIS40H1_REG_VMAX_H,
    MIS40H1_REG_VMAX_L,
    MIS40H1_REG_NUM1,
    MIS40H1_REG_DGAIN_H,
    MIS40H1_REG_DGAIN_L,
    MIS40H1_REG_MAX_NUM,
} mis40h1_reg_info;

#ifdef __cplusplus
}
#endif

#endif // __MIS40H1_H__