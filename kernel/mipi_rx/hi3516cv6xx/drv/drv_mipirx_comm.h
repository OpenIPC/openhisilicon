/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */
#ifndef DRV_MIPIRX_COMM_H
#define DRV_MIPIRX_COMM_H

#include "drv_mipirx_osal.h"
#include "drv_mipirx_kapi_struct.h"
#include "hal_mipirx_comm.h"

#define mipirx_abs(value) ((value) > 0 ? (value) : -(value))
#define mipirx_array_size(x) (sizeof(x) / sizeof((x)[0]))
#define mipirx_bit(x)             (1u << (x))
#define mipirx_bits(_start, _end) ((mipirx_bit(_end) - mipirx_bit(_start)) + mipirx_bit(_end))

#define MIPIRX_LANE_MAX 4
#define SIZE_0  0
#define SIZE_1  1
#define SIZE_2  2
#define SIZE_3  3
#define SIZE_4  4
#define SIZE_5  5
#define SIZE_6  6
#define SIZE_7  7
#define SIZE_8  8

td_bool mipirx_check_phy_link_port(td_u8 phy_id, mipirx_lane_bit_map *cur_lane);

td_u16 mipirx_get_lane_bit_map(const td_s8 *lane_id, td_u8 max_lane_num);

td_void mipirx_get_phy_lane_bit_map(td_s8 *lane_id, td_u8 lane_size,
    mipirx_lane_bit_map *bit_map, td_u8 max_phy_num);

td_u8 mipirx_get_phy_lane_bit(td_u8 phy_id, mipirx_lane_bit_map *lane_bit_map, td_u8 port_max);

#endif
