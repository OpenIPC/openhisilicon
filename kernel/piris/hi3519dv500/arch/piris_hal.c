/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */
#include "ot_type.h"
#include "piris_hal.h"

/* PIRIS_DRV_Write REG value */
/* gpio5_0 AIN2 */
#define PIRIS_B_CASE0_REG0   0x0

#define PIRIS_B_CASE1_REG0   0x1

#define PIRIS_B_CASE2_REG0   0x1

#define PIRIS_B_CASE3_REG0   0x0

/* PIRIS_DRV_Write REG value */
/* gpio4_5(AIN1) gpio4_7(BIN1) gpio4_6(BIN2) */
#define PIRIS_A_CASE0_REG0   0xa0

#define PIRIS_A_CASE1_REG0   0x80

#define PIRIS_A_CASE2_REG0   0x40

#define PIRIS_A_CASE3_REG0   0x60

#define ot_io_pirisi_address(piris_ibase, x)  ((td_uintptr_t)(piris_ibase) + ((x) - (BASE_ADDR)))

#define piris_a_cfg_reg0(piris_ibase)        ot_io_pirisi_address((piris_ibase), BASE_ADDR + 0x43c0)
#define piris_a_ctrl_reg0(piris_ibase)       ot_io_pirisi_address((piris_ibase), BASE_ADDR + 0x4000)

#define piris_b_cfg_reg0(piris_ibase)        ot_io_pirisi_address((piris_ibase), BASE_ADDR + 0x5004)
#define piris_b_ctrl_reg0(piris_ibase)       ot_io_pirisi_address((piris_ibase), BASE_ADDR + 0x5000)

#define  piris_write_reg(addr, value) ((*(volatile unsigned int *)(addr)) = (value))
#define  piris_read_reg(addr)         (*(volatile unsigned int *)(addr))

int piris_drv_write(void *piris_ibase, unsigned char bits)
{
    switch (bits) {
        case 0: /* case 0 */
            piris_write_reg(piris_a_ctrl_reg0(piris_ibase), 0xE0);        // GPIO4_7, GPIO4_6, GPIO4_5
            piris_write_reg(piris_b_ctrl_reg0(piris_ibase), 0x1);         // GPIO5_0
            piris_write_reg(piris_a_cfg_reg0(piris_ibase), PIRIS_A_CASE0_REG0);
            piris_write_reg(piris_b_cfg_reg0(piris_ibase), PIRIS_B_CASE0_REG0);
            break;

        case 1: /* case 1 */
            piris_write_reg(piris_a_ctrl_reg0(piris_ibase), 0xE0);
            piris_write_reg(piris_b_ctrl_reg0(piris_ibase), 0x1);
            piris_write_reg(piris_a_cfg_reg0(piris_ibase), PIRIS_A_CASE1_REG0);
            piris_write_reg(piris_b_cfg_reg0(piris_ibase), PIRIS_B_CASE1_REG0);
            break;

        case 2: /* case 2 */
            piris_write_reg(piris_a_ctrl_reg0(piris_ibase), 0xE0);
            piris_write_reg(piris_b_ctrl_reg0(piris_ibase), 0x1);
            piris_write_reg(piris_a_cfg_reg0(piris_ibase), PIRIS_A_CASE2_REG0);
            piris_write_reg(piris_b_cfg_reg0(piris_ibase), PIRIS_B_CASE2_REG0);
            break;

        case 3: /* case 3 */
            piris_write_reg(piris_a_ctrl_reg0(piris_ibase), 0xE0);
            piris_write_reg(piris_b_ctrl_reg0(piris_ibase), 0x1);
            piris_write_reg(piris_a_cfg_reg0(piris_ibase), PIRIS_A_CASE3_REG0);
            piris_write_reg(piris_b_cfg_reg0(piris_ibase), PIRIS_B_CASE3_REG0);
            break;

        default:
            break;
    }
    return 0;
}
