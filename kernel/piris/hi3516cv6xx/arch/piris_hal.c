/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */
#include "ot_type.h"
#include "piris_hal.h"

/* PIRIS_DRV_Write REG value */
/* gpio10_0 (AIN1) gpio10_1 (BIN1) */
#define PIRIS_B_CASE0_REG0   0x3

#define PIRIS_B_CASE1_REG0   0x6

#define PIRIS_B_CASE2_REG0   0xc

#define PIRIS_B_CASE3_REG0   0x9

/* PIRIS_DRV_Write REG value */
/* gpio10_2(AIN2) gpio10_3(BIN2) */
#define PIRIS_A_CASE0_REG0   0x3

#define PIRIS_A_CASE1_REG0   0x6

#define PIRIS_A_CASE2_REG0   0xc

#define PIRIS_A_CASE3_REG0   0x9

#define ot_io_pirisi_address(piris_ibase, x)  ((td_uintptr_t)(piris_ibase) + ((x) - (BASE_ADDR)))

#define piris_a_cfg_reg0(piris_ibase)        ot_io_pirisi_address((piris_ibase), BASE_ADDR + 0xa030)
#define piris_a_ctrl_reg0(piris_ibase)       ot_io_pirisi_address((piris_ibase), BASE_ADDR + 0xa400)

#define piris_b_cfg_reg0(piris_ibase)        ot_io_pirisi_address((piris_ibase), BASE_ADDR + 0xa00c)
#define piris_b_ctrl_reg0(piris_ibase)       ot_io_pirisi_address((piris_ibase), BASE_ADDR + 0xa400)

#define  piris_write_reg(addr, value) ((*(volatile unsigned int *)(addr)) = (value))
#define  piris_read_reg(addr)         (*(volatile unsigned int *)(addr))

int piris_drv_write(void *piris_ibase, unsigned char bits)
{
    switch (bits) {
        case 0: /* case 0 */
            piris_write_reg(piris_a_ctrl_reg0(piris_ibase), 0xf);        // GPIO10_0, GPIO10_1, GPIO10_2, GPIO10_3
            piris_write_reg(piris_b_ctrl_reg0(piris_ibase), 0xf);        // GPIO10_0, GPIO10_1, GPIO10_2, GPIO10_3
            piris_write_reg(piris_a_cfg_reg0(piris_ibase), PIRIS_A_CASE0_REG0);
            piris_write_reg(piris_b_cfg_reg0(piris_ibase), PIRIS_B_CASE0_REG0);
            break;

        case 1: /* case 1 */
            piris_write_reg(piris_a_ctrl_reg0(piris_ibase), 0xf);
            piris_write_reg(piris_b_ctrl_reg0(piris_ibase), 0xf);
            piris_write_reg(piris_a_cfg_reg0(piris_ibase), PIRIS_A_CASE1_REG0);
            piris_write_reg(piris_b_cfg_reg0(piris_ibase), PIRIS_B_CASE1_REG0);
            break;

        case 2: /* case 2 */
            piris_write_reg(piris_a_ctrl_reg0(piris_ibase), 0xf);
            piris_write_reg(piris_b_ctrl_reg0(piris_ibase), 0xf);
            piris_write_reg(piris_a_cfg_reg0(piris_ibase), PIRIS_A_CASE2_REG0);
            piris_write_reg(piris_b_cfg_reg0(piris_ibase), PIRIS_B_CASE2_REG0);
            break;

        case 3: /* case 3 */
            piris_write_reg(piris_a_ctrl_reg0(piris_ibase), 0xf);
            piris_write_reg(piris_b_ctrl_reg0(piris_ibase), 0xf);
            piris_write_reg(piris_a_cfg_reg0(piris_ibase), PIRIS_A_CASE3_REG0);
            piris_write_reg(piris_b_cfg_reg0(piris_ibase), PIRIS_B_CASE3_REG0);
            break;

        default:
            break;
    }
    return 0;
}
