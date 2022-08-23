/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#ifndef __PIRIS_HAL_H__
#define __PIRIS_HAL_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/***************base address***********************/
/* Piris use GPIO3_4, GPIO3_5, GPIO3_6, GPIO3_7 */
/* GPIO1's base address is 0x120D0000 */

#define PIRISI_ADRESS_BASE 0x120B3000

/***************PIRIS_DRV_Write REG value***********************/
#define PIRIS_CASE0_REG0 0x50

#define PIRIS_CASE1_REG0 0x60

#define PIRIS_CASE2_REG0 0xA0

#define PIRIS_CASE3_REG0 0x90

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __PIRIS_HAL_H__ */
