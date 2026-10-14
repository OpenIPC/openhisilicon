#ifndef __JXF37P_H__
#define __JXF37P_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define JXF37P_FULL_LINES_MAX           0xFFFF
#define JXF37P_VMAX_2M_1920x1080_LINEAR 0x465

#define JXF37P_EXP_OFFSET_LINEAR 0 // TODO: 待确认

#define JXF37P_EXP_SHIFT 0

#define JXF37P_REG_ADDR_EXP_H  0x02
#define JXF37P_REG_ADDR_EXP_L  0x01
#define JXF37P_REG_ADDR_GAIN   0x00
#define JXF37P_REG_ADDR_VMAX_H 0x23
#define JXF37P_REG_ADDR_VMAX_L 0x22

typedef enum {
    JXF37P_REG_EXP_H = 0,
    JXF37P_REG_EXP_L,
    JXF37P_REG_GAIN,
    JXF37P_REG_VMAX_H,
    JXF37P_REG_VMAX_L,
    JXF37P_REG_MAX_NUM
} jxf37p_reg_info;

#ifdef __cplusplus
}
#endif

#endif // __JXF37P_H__
