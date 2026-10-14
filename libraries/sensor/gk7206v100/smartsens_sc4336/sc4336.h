#ifndef __SC4336_H__
#define __SC4336_H__

#include "sns_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

/* sensor fps mode */
#define SC4336_FULL_LINES_MAX                 0x7FFF
#define SC4336_INCREASE_LINES                 0 /* make real fps less than stand fps because NVR require */
#define SC4336_SLAVE_VMAX_4M_LINEAR           (3000 + SC4336_INCREASE_LINES)
#define SC4336_VMAX_4M_LINEAR                 (1500 + SC4336_INCREASE_LINES)
#define SC4336_FRAME_RATE_MIN                 0x34BC
#define SC4336_EXP_OFFSET_LINEAR                 10

#define SC4336_S_EXP_MAX_DEFAULT_1440P 0xcc

// the value of the sc4336 exposure register is in half line units, (<< 1) for AE calculation, (>> 1) for EXP reg
#define SC4336_EXP_SHIFT 1

#define SC4336_REG_ADDR_EXP_H       0x3e00
#define SC4336_REG_ADDR_EXP_M       0x3e01
#define SC4336_REG_ADDR_EXP_L       0x3e02
#define SC4336_REG_ADDR_AGAIN_H     0x3e08
#define SC4336_REG_ADDR_AGAIN_L     0x3e09
#define SC4336_REG_ADDR_DGAIN_H     0x3e06
#define SC4336_REG_ADDR_DGAIN_L     0x3e07
#define SC4336_REG_ADDR_VMAX_H      0x320e
#define SC4336_REG_ADDR_VMAX_L      0x320f

typedef enum {
    SC4336_REG_EXP_H = 0,
    SC4336_REG_EXP_M,
    SC4336_REG_EXP_L,
    SC4336_REG_AGAIN_H,
    SC4336_REG_AGAIN_L,
    SC4336_REG_DGAIN_H,
    SC4336_REG_DGAIN_L,
    SC4336_REG_VMAX_H,
    SC4336_REG_VMAX_L,
    SC4336_REG_L_MAX_NUM,
} sc4336_reg_info;

#define SC4336_SLAVE_LINEAR_REG_0X322E   SC4336_REG_L_MAX_NUM
#define SC4336_SLAVE_LINEAR_REG_0X322F   SC4336_REG_L_MAX_NUM + 1

#ifdef __cplusplus
}
#endif

#endif // __SC4336_H__