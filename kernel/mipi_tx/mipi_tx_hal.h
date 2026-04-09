/******************************************************************************
Copyright (C), 2016, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : mipi_tx_hal.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2016/11/15
Last Modified :
Description   :
Function List :
******************************************************************************/
#ifndef __MIPI_TX_HAL_H__
#define __MIPI_TX_HAL_H__

#include "hi_mipi_tx.h"

typedef struct {
    unsigned int vall_det;
    unsigned int vact_det;
    unsigned int hall_det;
    unsigned int hact_det;
    unsigned int hbp_det;
    unsigned int hsa_det;
    unsigned int vsa_det;
}mipi_tx_dev_phy_t;


void mipi_tx_drv_set_phy_cfg(combo_dev_cfg_t *dev_cfg);
void mipi_tx_drv_set_controller_cfg(combo_dev_cfg_t *dev_cfg);

int mipi_tx_drv_set_cmd_info(cmd_info_t *cmd_info);
int mipi_tx_drv_get_cmd_info(get_cmd_info_t *get_cmd_info);
void mipi_tx_drv_enable_input(output_mode_t output_mode);
void mipi_tx_drv_disable_input(void);

int mipi_tx_drv_init(int smooth);
void mipi_tx_drv_exit(void);

#endif
