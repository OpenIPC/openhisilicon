/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */
#ifndef PLAT_MIPIRX_COMM_H
#define PLAT_MIPIRX_COMM_H

#include "drv_mipirx_osal.h"
#include "hal_mipirx_custom.h"

typedef struct {
    td_bool phy_rg_drveclk2_enz;
    td_bool phy_rg_drveclk_enz;
    td_bool phy_rg_extbus_en;
    td_bool phy_rg_ext_en;
    td_bool phy_rg_ext2_en;
    td_bool phy_rg_int_en;
} mipirx_phy_mode_link;

typedef struct {
    td_u8 phy_lane_bit[MIPIRX_PHY_NUM_MAX];
} mipirx_lane_bit_map;

td_s32 hal_mipirx_reg_base_init(td_void *pdev);

td_s32 hal_mipirx_reg_base_deinit(td_void);

td_u32 *hal_mipirx_get_global_addr(td_void);

td_u32 *hal_mipirx_get_pixel_crg_addr(td_void);

td_u32 *hal_mipirx_get_sys_addr(td_void);

td_u32 *hal_mipirx_get_fpga_phy_addr(td_void);

td_u32 *hal_mipirx_get_fpga_ctrl_addr(td_void);

td_u32 *hal_mipirx_get_sns_addr(td_u8 port_id);

td_u32 *hal_mipirx_get_ctrl_addr(td_u8 port_id);

td_u32 *hal_mipirx_get_phy_addr(td_u8 port_id);

td_u32 hal_mipirx_reg_read(const td_void *base, td_u32 offset);

td_void hal_mipirx_reg_write(const td_void *base, td_u32 offset, td_u32 value);

td_u32 hal_mipirx_reg_readb(const td_void *base, td_u32 offset, td_u32 mask);

td_void hal_mipirx_reg_writeb(const td_void *base, td_u32 offset, td_u32 mask, td_u32 val);

#endif
