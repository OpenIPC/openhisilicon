#ifndef _IMX415_COMM_H_
#define _IMX415_COMM_H_

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Imx415 Register Address */
#define IMX415_REG_ADDR_GAIN   (0x3090)
#define IMX415_REG_ADDR_VMAX_L (0x3024)
#define IMX415_REG_ADDR_VMAX_M (0x3025)
#define IMX415_REG_ADDR_VMAX_H (0x3026)
#define IMX415_REG_ADDR_SHR0_L (0x3050)
#define IMX415_REG_ADDR_SHR0_M (0x3051)
#define IMX415_REG_ADDR_SHR0_H (0x3052)
#define IMX415_REG_ADDR_SHR1_L (0x3054)
#define IMX415_REG_ADDR_SHR1_M (0x3055)
#define IMX415_REG_ADDR_SHR1_H (0x3056)
#define IMX415_REG_ADDR_RHS1_L (0x3060)
#define IMX415_REG_ADDR_RHS1_M (0x3061)
#define IMX415_REG_ADDR_RHS1_H (0x3062)

typedef enum {
    IMX415_REG_GAIN = 0,
    IMX415_REG_VMAX_L,
    IMX415_REG_VMAX_M,
    IMX415_REG_VMAX_H,
    IMX415_REG_SHR0_L,
    IMX415_REG_SHR0_M,
    IMX415_REG_SHR0_H,
    IMX415_REG_L_MAX_NUM,

    IMX415_REG_SHR1_L = IMX415_REG_L_MAX_NUM,
    IMX415_REG_SHR1_M,
    IMX415_REG_SHR1_H,
    IMX415_REG_RHS1_L,
    IMX415_REG_RHS1_M,
    IMX415_REG_RHS1_H,
    IMX415_REG_MAX_NUM
}imx415_reg_info;

#define IMX415_LINEAR_REG_INFO_MAX_NUM   IMX415_REG_L_MAX_NUM
#define IMX415_2TO1_WDR_REG_INFO_MAX_NUM IMX415_REG_MAX_NUM

/****************************************************************************
 * local variables                                                            *
 ****************************************************************************/
#define IMX415_FULL_LINES_MAX           0xFFFFF
#define IMX415_FULL_LINES_MAX_2TO1_WDR  9000

#define IMX415_VMAX_8M_LINEAR   2250
#define IMX415_VMAX_8M_WDR      4500

#ifdef __cplusplus
}
#endif

#endif
