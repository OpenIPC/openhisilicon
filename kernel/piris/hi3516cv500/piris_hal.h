/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of piris_hal.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __PIRIS_HAL_H__
#define __PIRIS_HAL_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/* base address */
/* Piris use GPIO0_3, GPIO0_4, GPIO0_5, GPIO0_6 */
/* GPIO1's base address is 0x120D0000 */
#define PIRISI_ADRESS_BASE 0x120D0000

/* PIRIS_DRV_Write REG value */
#define PIRIS_CASE0_REG0   0x28

#define PIRIS_CASE1_REG0   0x30

#define PIRIS_CASE2_REG0   0x50

#define PIRIS_CASE3_REG0   0x48

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __PIRIS_HAL_H__ */
