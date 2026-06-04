/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#include "drv_mipirx_comm.h"

static td_u8 mpirx_get_phy_lane_bit_map(td_u8 phy_id, td_u16 lane_bit_map)
{
    return (lane_bit_map >> (MIPIRX_ONE_PHY_LANE_NUM * phy_id)) & 0xf;
}

td_bool mipirx_check_phy_link_port(td_u8 phy_id, mipirx_lane_bit_map *cur_lane)
{
    if (phy_id >= MIPIRX_PHY_NUM_MAX) {
        return TD_FALSE;
    }

    return cur_lane->phy_lane_bit[phy_id] > 0;
}

td_u16 mipirx_get_lane_bit_map(const td_s8 *lane_id, td_u8 max_lane_num)
{
    int i;
    td_u16 lane_bit_map = 0;
    for (i = 0; i < max_lane_num; ++i) {
        if (lane_id[i] >= 0) {
            lane_bit_map |= (0x1 << lane_id[i]);
        }
    }
    return lane_bit_map;
}

td_void mipirx_get_phy_lane_bit_map(td_s8 *lane_id, td_u8 lane_size,
    mipirx_lane_bit_map *bit_map, td_u8 max_phy_num)
{
    td_u8 phy_id;
    td_u8 phy_lane_bit;
    td_u16 lane_bit_map;

    lane_bit_map = mipirx_get_lane_bit_map(lane_id, lane_size);
    for (phy_id = 0; phy_id < max_phy_num; ++phy_id) {
        phy_lane_bit = mpirx_get_phy_lane_bit_map(phy_id, lane_bit_map);
        if (phy_lane_bit != 0) {
            bit_map->phy_lane_bit[phy_id] = phy_lane_bit;
        }
    }
}

td_u8 mipirx_get_phy_lane_bit(td_u8 phy_id, mipirx_lane_bit_map *lane_bit_map, td_u8 port_max)
{
    td_u8 i;
    td_u8 phy_bit_map = 0;
    for (i = 0; i < port_max; ++i) {
        phy_bit_map |= lane_bit_map[i].phy_lane_bit[phy_id];
    }
    return phy_bit_map;
}