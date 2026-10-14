#ifndef _IMX179_COMM_H_
#define _IMX179_COMM_H_

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Imx179 Register Address */
#define IMX179_REG_ADDR_GROUP_HOLD 0x0104 // grouped param hold
#define IMX179_REG_ADDR_EXP_H      0x0202
#define IMX179_REG_ADDR_EXP_L      0x0203
#define IMX179_REG_ADDR_AGAIN_H    0x0204
#define IMX179_REG_ADDR_AGAIN_L    0x0205
#define IMX179_REG_ADDR_DGAIN_GR_H 0x020e
#define IMX179_REG_ADDR_DGAIN_GR_L 0x020f
#define IMX179_REG_ADDR_DGAIN_R_H  0x0210
#define IMX179_REG_ADDR_DGAIN_R_L  0x0211
#define IMX179_REG_ADDR_DGAIN_B_H  0x0212
#define IMX179_REG_ADDR_DGAIN_B_L  0x0213
#define IMX179_REG_ADDR_DGAIN_GB_H 0x0214
#define IMX179_REG_ADDR_DGAIN_GB_L 0x0215
#define IMX179_REG_ADDR_VMAX_H     0x0340
#define IMX179_REG_ADDR_VMAX_L     0x0341

typedef enum {
    IMX179_REG_GROUP_HOLD_START = 0,
    IMX179_REG_EXP_H,
    IMX179_REG_EXP_L,
    IMX179_REG_AGAIN_H,
    IMX179_REG_AGAIN_L,
    IMX179_REG_DGAIN_GR_H,
    IMX179_REG_DGAIN_GR_L,
    IMX179_REG_DGAIN_R_H,
    IMX179_REG_DGAIN_R_L,
    IMX179_REG_DGAIN_B_H,
    IMX179_REG_DGAIN_B_L,
    IMX179_REG_DGAIN_GB_H,
    IMX179_REG_DGAIN_GB_L,
    IMX179_REG_VMAX_H,
    IMX179_REG_VMAX_L,
    IMX179_REG_GROUP_HOLD_END,
    IMX179_REG_MAX_NUM
}imx179_reg_info;

#define IMX179_EXP_OFFSET_LINEAR 4

/****************************************************************************
 * local variables                                                            *
 ****************************************************************************/
#define IMX179_FULL_LINES_MAX   0xFFFFF

#define IMX179_VMAX_8M_LINEAR        2481
#define IMX179_VMAX_640_480_LINEAR   0x26c

#ifdef __cplusplus
}
#endif

#endif
