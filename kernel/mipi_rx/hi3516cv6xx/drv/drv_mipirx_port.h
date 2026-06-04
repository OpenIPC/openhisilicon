/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */
#ifndef DRV_MIPIRX_PORT_H
#define DRV_MIPIRX_PORT_H

#include "hal_mipirx_comm.h"
#include "drv_mipirx_kapi_struct.h"

typedef struct {
    mipirx_lane_mode lane_mode;
    mipirx_port_cfg mipirx_port_cfg[MIPIRX_PORT_NUM_MAX];
    mipirx_lane_bit_map lane_bit_map[MIPIRX_PORT_NUM_MAX];
} mipirx_port_ctx;

#endif