/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#include "drv_mipirx_comm.h"
#include "hal_mipirx_comm.h"
#include "hal_mipirx_custom.h"
#include "hal_mipirx_phy_reg.h"
#include "hal_mipirx_global_reg.h"
#include "hal_mipirx_phy.h"
#include "sys_ext.h"

#define MIPI_CIL_FREQ 150 /* MIPI CIL FREQ 150MHZ */
#define MIPIRX_DELAY 1000

#ifdef CONFIG_OT_MIPIRX_LVDS_SUPPORT
typedef struct {
    mipirx_data_type input_data_type;
    td_u32 data_bit_width_value;
} mipirx_lvds_data_bit_width_map;

static const mipirx_lvds_data_bit_width_map g_mipirx_lvds_data_bit_width_map[] = {
    {MIPIRX_DATA_TYPE_RAW_8BIT,  0x1},
    {MIPIRX_DATA_TYPE_RAW_10BIT, 0x2},
    {MIPIRX_DATA_TYPE_RAW_12BIT, 0x3},
    {MIPIRX_DATA_TYPE_RAW_14BIT, 0x4},
};
#endif

static mipirx_phy_err_cnt phy_err_cnt[MIPIRX_PHY_NUM_MAX];

mipirx_phy_err_cnt *hal_mipi_get_phy_err_cnt_ctx(td_u8 phy_id)
{
    if (phy_id >= MIPIRX_PHY_NUM_MAX) {
        return TD_NULL;
    }
    return &phy_err_cnt[phy_id];
}

static td_char *hal_mipirx_convert_fsm_stat(td_u32 sta)
{
    switch (sta) {
        case 0x0:
            return "IDLE";
        case 0x1:
            return "LP11";
        case 0x2:
            return "SOT_LP01";
        case 0x3:
            return "SOT_LP00";
        case 0x4:
            return "SOT_HS0";
        case 0x5:
            return "HS_VLD";
        case 0x6:
            return "HS_LP00";
        case 0x7:
            return "EOT_LP11";
        case 0x8:
            return "ESCAPE";
        default:
            return "Na";
    }
}

td_char *hal_mipirx_phy_get_lane_cur_fsm_stat(td_u32 phy_id, td_s16 lane_id)
{
    td_u32 sta;
    td_u32 *phy_addr = hal_mipirx_get_phy_addr(phy_id);

    switch (lane_id) {
        case 0x0:
            sta = hal_mipirx_reg_readb(phy_addr, REG_CIL_FSM_ST0_LINK, CFG_CIL_CUR_ST_D0_M);
            break;
        case 0x1:
            sta = hal_mipirx_reg_readb(phy_addr, REG_CIL_FSM_ST0_LINK, CFG_CIL_CUR_ST_D1_M);
            break;
        case 0x2:
            sta = hal_mipirx_reg_readb(phy_addr, REG_CIL_FSM_ST0_LINK, CFG_CIL_CUR_ST_D2_M);
            break;
        case 0x3:
            sta = hal_mipirx_reg_readb(phy_addr, REG_CIL_FSM_ST0_LINK, CFG_CIL_CUR_ST_D3_M);
            break;
        default:
            return "Na";
    }

    return hal_mipirx_convert_fsm_stat(sta);
}

td_char *hal_mipirx_phy_get_lane_next_fsm_stat(td_u8 phy_id, td_s16 lane_id)
{
    td_u32 sta;
    td_u32 *phy_addr = hal_mipirx_get_phy_addr(phy_id);

    switch (lane_id) {
        case 0x0:
            sta = hal_mipirx_reg_readb(phy_addr, REG_CIL_FSM_ST0_LINK, CFG_CIL_NXT_ST_D0_M);
            break;
        case 0x1:
            sta = hal_mipirx_reg_readb(phy_addr, REG_CIL_FSM_ST0_LINK, CFG_CIL_NXT_ST_D1_M);
            break;
        case 0x2:
            sta = hal_mipirx_reg_readb(phy_addr, REG_CIL_FSM_ST0_LINK, CFG_CIL_NXT_ST_D2_M);
            break;
        case 0x3:
            sta = hal_mipirx_reg_readb(phy_addr, REG_CIL_FSM_ST0_LINK, CFG_CIL_NXT_ST_D3_M);
            break;
        default:
            return "Na";
    }

    return hal_mipirx_convert_fsm_stat(sta);
}

td_char *hal_mipirx_phy_get_clk_cur_fsm_stat(td_u8 phy_id, td_u32 clk_num)
{
    td_u32 sta;
    td_u32 *phy_addr = hal_mipirx_get_phy_addr(phy_id);

    switch (clk_num) {
        case 0x0:
            sta = hal_mipirx_reg_readb(phy_addr, REG_CIL_FSM_ST1_LINK, CFG_CIL_CUR_ST_CK_M);
            break;
        case 0x1:
            sta = hal_mipirx_reg_readb(phy_addr, REG_CIL_FSM_ST1_LINK, CFG_CIL_CUR_ST_CK2_M);
            break;
        default:
            return "Na";
    }

    return hal_mipirx_convert_fsm_stat(sta);
}

td_char *hal_mipirx_phy_get_clk_next_fsm_stat(td_u8 phy_id, td_u32 clk_num)
{
    td_u32 sta;
    td_u32 *phy_addr = hal_mipirx_get_phy_addr(phy_id);

    switch (clk_num) {
        case 0x0:
            sta = hal_mipirx_reg_readb(phy_addr, REG_CIL_FSM_ST1_LINK, CFG_CIL_NXT_ST_CK_M);
            break;
        case 0x1:
            sta = hal_mipirx_reg_readb(phy_addr, REG_CIL_FSM_ST1_LINK, CFG_CIL_NXT_ST_CK2_M);
            break;
        default:
            return "Na";
    }

    return hal_mipirx_convert_fsm_stat(sta);
}

#ifdef CONFIG_OT_MIPIRX_LVDS_SUPPORT
td_void hal_mipirx_phy_get_lvds_data(td_u8 phy_id, td_u32 *data, td_u32 size)
{
    td_u8 i;
    td_u32 *phy_addr = hal_mipirx_get_phy_addr(phy_id);

    for (i = 0; i < MIPIRX_LANE_MAX && i < size; i++) {
        switch (i) {
            case SIZE_0:
                data[i] = hal_mipirx_reg_readb(phy_addr, REG_PHY_DATA_LVDS_LINK, CFG_PHY_DATA0_LVDS_HS_M);
                break;
            case SIZE_1:
                data[i] = hal_mipirx_reg_readb(phy_addr, REG_PHY_DATA_LVDS_LINK, CFG_PHY_DATA1_LVDS_HS_M);
                break;
            case SIZE_2:
                data[i] = hal_mipirx_reg_readb(phy_addr, REG_PHY_DATA_LVDS_LINK, CFG_PHY_DATA2_LVDS_HS_M);
                break;
            case SIZE_3:
                data[i] = hal_mipirx_reg_readb(phy_addr, REG_PHY_DATA_LVDS_LINK, CFG_PHY_DATA3_LVDS_HS_M);
                break;
            default:
                mipirx_log_err("get lvds data with invalid lane id %u.", i);
                break;
        }
    }
}

td_void hal_mipirx_phy_get_lvds_pix_num(td_u8 phy_id, td_u32 data[], td_u32 size)
{
    td_u32 *phy_addr = hal_mipirx_get_phy_addr(phy_id);

    if (size < 0x2) {
        return;
    }

    data[0] = hal_mipirx_reg_readb(phy_addr, REG_PHY_PIX_PUM_LINK, CFG_PHY_LANE02_PIX_NUM_M);
    data[1] = hal_mipirx_reg_readb(phy_addr, REG_PHY_PIX_PUM_LINK, CFG_PHY_LANE13_PIX_NUM_M);
}
#endif

td_void hal_mipirx_phy_set_mode_link(td_u8 phy_id, mipirx_lane_mode lane_mode)
{
    td_u32 *phy_addr = hal_mipirx_get_phy_addr(phy_id);
    if (lane_mode == MODE_4_LANE) {
        hal_mipirx_reg_writeb(phy_addr, REG_PHY_MODE_LINK, CFG_PHY_RG_EN_CLK0_M, 1);
    } else if (lane_mode == MODE_2_PLUS_2_LANE) {
        hal_mipirx_reg_writeb(phy_addr, REG_PHY_MODE_LINK, CFG_PHY_RG_EN_2L2L_M, 1);
        hal_mipirx_reg_writeb(phy_addr, REG_PHY_MODE_LINK, CFG_PHY_RG_EN_CLK0_M, 1);
        hal_mipirx_reg_writeb(phy_addr, REG_PHY_MODE_LINK, CFG_PHY_RG_EN_CLK1_M, 1);
    }
}

td_void hal_mipirx_phy_set_skew(td_u8 phy_id)
{
    td_u32 *phy_addr = hal_mipirx_get_phy_addr(phy_id);
    if (ckfn_sys_entry() != TD_TRUE || ckfn_sys_get_mipirx_ext_cfg() != TD_TRUE) {
        mipirx_log_err("sys_entry or sys_get_mipirx_ext_cfg doesn't exist.\n");
        return;
    }
    /* if call_sys_get_mipirx_ext_cfg() return true is bga, else is qfn */
    if (call_sys_get_mipirx_ext_cfg()) {
        hal_mipirx_reg_write(phy_addr, REG_PHY_SKEW_LINK, 0x100000);
    } else {
        hal_mipirx_reg_write(phy_addr, REG_PHY_SKEW_LINK, 0x101100);
    }
}

td_void hal_mipirx_phy_set_lane_mode(mipirx_lane_mode lane_mode)
{
    td_u32 *global_addr = hal_mipirx_get_global_addr();

    if (lane_mode == MODE_4_LANE) {
        hal_mipirx_reg_writeb(global_addr, REG_HS_MODE_SELECT, CFG_HS_MODE_M, 0x7);
    } else if (lane_mode == MODE_2_PLUS_2_LANE) {
        hal_mipirx_reg_writeb(global_addr, REG_HS_MODE_SELECT, CFG_HS_MODE_M, 0xB);
    }
}

td_bool hal_mipirx_phy_get_enable_stat(td_u8 phy_id)
{
    td_u32 *global_addr = hal_mipirx_get_global_addr();
    return hal_mipirx_reg_readb(global_addr, REG_PHY_EN, CFG_PHY0_EN_M << phy_id) == 1;
}

td_bool hal_mipirx_phy_get_lane_term_on(td_u8 phy_id, td_s32 lane_id)
{
    td_u32 *phy_addr = hal_mipirx_get_phy_addr(phy_id);

    if (lane_id < 0) {
        return TD_FALSE;
    }

    return hal_mipirx_reg_readb(phy_addr, REG_PHY_EN_LINK, CFG_PHY_D0_TERM_EN_M << lane_id) == 1;
}

td_bool hal_mipirx_phy_get_lane_en(td_s32 lane_id)
{
    td_u32 *global_addr = hal_mipirx_get_global_addr();

    if (lane_id < 0) {
        return TD_FALSE;
    }

    return hal_mipirx_reg_readb(global_addr, REG_LANE_EN, CFG_LANE0_EN_M << lane_id) == 1;
}

static td_void hal_mipirx_phy_en_link(td_u32 *phy_addr, td_u32 phy_lane_bit_map)
{
    td_u32 phy_da_valid_mask =
        CFG_PHY_DA_D0_VALID_M | CFG_PHY_DA_D1_VALID_M | CFG_PHY_DA_D2_VALID_M | CFG_PHY_DA_D3_VALID_M;
    td_u32 phy_term_mask = CFG_PHY_D0_TERM_EN_M | CFG_PHY_D1_TERM_EN_M | CFG_PHY_D2_TERM_EN_M | CFG_PHY_D3_TERM_EN_M;

    hal_mipirx_reg_writeb(phy_addr, REG_PHY_EN_LINK, phy_da_valid_mask, phy_lane_bit_map);
    hal_mipirx_reg_writeb(phy_addr, REG_PHY_EN_LINK, phy_term_mask, phy_lane_bit_map);

    hal_mipirx_reg_writeb(phy_addr, REG_PHY_EN_LINK, CFG_PHY_CLK_TERM_EN_M, ((phy_lane_bit_map & 0x5) > 0));
    hal_mipirx_reg_writeb(phy_addr, REG_PHY_EN_LINK, CFG_PHY_CLK2_TERM_EN_M, ((phy_lane_bit_map & 0xa) > 0));
}

static td_void hal_mipirx_phy_cfg_lane_en(td_u32 *global_addr, td_u32 phy_lane_bit_map)
{
    td_u32 lane_en_mask = CFG_LANE0_EN_M | CFG_LANE1_EN_M | CFG_LANE2_EN_M | CFG_LANE3_EN_M;
    hal_mipirx_reg_writeb(global_addr, REG_LANE_EN, lane_en_mask, phy_lane_bit_map);
}

static td_void hal_mipirx_phy_cfg_rg_en_clk(td_u32 *phy_addr, td_u8 phy_lane_bit)
{
    hal_mipirx_reg_writeb(phy_addr, REG_PHY_MODE_LINK, CFG_PHY_RG_EN_D_M, phy_lane_bit);
    hal_mipirx_reg_writeb(phy_addr, REG_PHY_MODE_LINK, CFG_PHY_RG_EN_CLK0_M, TD_TRUE);
    hal_mipirx_reg_writeb(phy_addr, REG_PHY_MODE_LINK, CFG_PHY_RG_EN_CLK1_M, (phy_lane_bit & 0xa) > 0);
}

static td_void hal_mipirx_phy_cfg_test(td_u32 *phy_addr, td_u8 phy_lane_bit)
{
    if (phy_lane_bit <= SIZE_3 && phy_lane_bit > 0) {
        hal_mipirx_reg_write(phy_addr, REG_PHY_TEST, 0x40);
    } else {
        hal_mipirx_reg_write(phy_addr, REG_PHY_TEST, 0x44);
    }
}

td_void hal_mipirx_phy_cfg_lanes(td_u8 phy_id, td_u8 phy_lane_bit)
{
    td_u32 *phy_addr = hal_mipirx_get_phy_addr(phy_id);
    td_u32 *global_addr = hal_mipirx_get_global_addr();

    hal_mipirx_phy_en_link(phy_addr, phy_lane_bit);
    hal_mipirx_phy_cfg_lane_en(global_addr, phy_lane_bit);
    hal_mipirx_phy_cfg_rg_en_clk(phy_addr, phy_lane_bit);

    hal_mipirx_phy_cfg_test(phy_addr, phy_lane_bit);
    hal_mipirx_reg_writeb(global_addr, REG_PHY_EN, CFG_PHY0_EN_M << phy_id, phy_lane_bit > 0);
    hal_mipirx_reg_writeb(global_addr, REG_PHY_CIL_CTRL, CFG_PHYCIL0_CKEN_M << phy_id, phy_lane_bit > 0);
    hal_mipirx_reg_writeb(global_addr, REG_PHYCFG_EN, CFG_PHYCIL0_CFG_EN_M << phy_id, TD_TRUE);
}

#ifdef CONFIG_OT_MIPIRX_LVDS_SUPPORT
td_void hal_mipirx_phy_set_work_mode(td_u8 phy_id, mipirx_work_mode work_mode)
{
    td_u32 *phy_addr = hal_mipirx_get_phy_addr(phy_id);

    if (work_mode == MIPIRX_INPUT_MODE_SUBLVDS) {
        hal_mipirx_reg_writeb(phy_addr, REG_PHY_MODE_LINK, CFG_PHY_RG_MIPI_MODE1_M, 0);
        hal_mipirx_reg_writeb(phy_addr, REG_PHY_MODE_LINK, CFG_PHY_RG_MIPI_MODE0_M, 1);
        hal_mipirx_reg_writeb(phy_addr, REG_PHY_MODE_LINK, CFG_PHY_RG_EN_LP1_M, 1);
        hal_mipirx_reg_writeb(phy_addr, REG_PHY_MODE_LINK, CFG_PHY_RG_EN_LP0_M, 1);
        hal_mipirx_reg_writeb(phy_addr, REG_PHY_RG_TOP_CFG_LINK, CFG_CIL_RG_TOP_CFG1_M, 0x15);
    } else if (work_mode == MIPIRX_INPUT_MODE_LVDS) {
        hal_mipirx_reg_writeb(phy_addr, REG_PHY_MODE_LINK, CFG_PHY_RG_MIPI_MODE1_M, 1);
        hal_mipirx_reg_writeb(phy_addr, REG_PHY_MODE_LINK, CFG_PHY_RG_MIPI_MODE0_M, 0);
        hal_mipirx_reg_writeb(phy_addr, REG_PHY_RG_TOP_CFG_LINK, CFG_CIL_RG_TOP_CFG1_M, 0x16);
    } else { // MIPI/HISPI
        hal_mipirx_reg_writeb(phy_addr, REG_PHY_MODE_LINK, CFG_PHY_RG_EN_LP1_M, 1);
        hal_mipirx_reg_writeb(phy_addr, REG_PHY_MODE_LINK, CFG_PHY_RG_EN_LP0_M, 1);
        hal_mipirx_reg_writeb(phy_addr, REG_PHY_RG_TOP_CFG_LINK, CFG_CIL_RG_TOP_CFG1_M, 0x14);
    }
}
#else
td_void hal_mipirx_phy_set_work_mode(td_u8 phy_id, mipirx_work_mode work_mode)
{
    td_u32 *phy_addr = hal_mipirx_get_phy_addr(phy_id);
    ot_unused(work_mode);

    hal_mipirx_reg_writeb(phy_addr, REG_PHY_MODE_LINK, CFG_PHY_RG_EN_LP1_M, 1);
    hal_mipirx_reg_writeb(phy_addr, REG_PHY_MODE_LINK, CFG_PHY_RG_EN_LP0_M, 1);
    hal_mipirx_reg_writeb(phy_addr, REG_PHY_RG_TOP_CFG_LINK, CFG_CIL_RG_TOP_CFG1_M, 0x14);
}
#endif

td_void hal_mipirx_phy_get_mipi_data(td_u8 phy_id, td_u32 *mipi_lane_data, td_u16 size)
{
    td_u8 i;
    td_u32 *phy_addr = hal_mipirx_get_phy_addr(phy_id);

    for (i = 0; i < MIPIRX_LANE_MAX && i < size; i++) {
        switch (i) {
            case SIZE_0:
                mipi_lane_data[i] = hal_mipirx_reg_readb(phy_addr, REG_PHY_DATA_MIPI_LINK, CFG_PHY_DATA0_MIPI_HS_M);
                break;
            case SIZE_1:
                mipi_lane_data[i] = hal_mipirx_reg_readb(phy_addr, REG_PHY_DATA_MIPI_LINK, CFG_PHY_DATA1_MIPI_HS_M);
                break;
            case SIZE_2:
                mipi_lane_data[i] = hal_mipirx_reg_readb(phy_addr, REG_PHY_DATA_MIPI_LINK, CFG_PHY_DATA2_MIPI_HS_M);
                break;
            case SIZE_3:
                mipi_lane_data[i] = hal_mipirx_reg_readb(phy_addr, REG_PHY_DATA_MIPI_LINK, CFG_PHY_DATA3_MIPI_HS_M);
                break;
            default:
                mipirx_log_err("get mipi data with invalid lane id %u.", i);
                break;
        }
    }
}

#ifdef CONFIG_OT_MIPIRX_LVDS_SUPPORT
td_void hal_mipirx_phy_set_sync_dct(td_u8 phy_id, mipirx_data_type data_type,
    mipirx_lvds_endian data_endian, td_u32 lane_bitmap)
{
    td_u8 len;
    td_u8 i;
    td_u32 data_type_val;
    td_u32 *phy_addr = hal_mipirx_get_phy_addr(phy_id);
    len = mipirx_array_size(g_mipirx_lvds_data_bit_width_map);
    for (i = 0; i < len; i++) {
        if (data_type == g_mipirx_lvds_data_bit_width_map[i].input_data_type) {
            data_type_val = g_mipirx_lvds_data_bit_width_map[i].data_bit_width_value;
            break;
        }
    }

    if (lane_bitmap & 0x5) {
        hal_mipirx_reg_writeb(phy_addr, REG_PHY_SYNC_DCT_LINK, CFG_CIL_RAW_TYPE0_M, data_type_val);
        hal_mipirx_reg_writeb(phy_addr, REG_PHY_SYNC_DCT_LINK, CFG_CIL_CODE_BIG_ENDIAN0_M, data_endian);
    }

    if (lane_bitmap & 0xa) {
        hal_mipirx_reg_writeb(phy_addr, REG_PHY_SYNC_DCT_LINK, CFG_CIL_RAW_TYPE1_M, data_type_val);
        hal_mipirx_reg_writeb(phy_addr, REG_PHY_SYNC_DCT_LINK, CFG_CIL_CODE_BIG_ENDIAN1_M, data_endian);
    }
}

td_void hal_mipirx_phy_set_sync_code(td_u8 phy_id, td_s8 *lane_id,
    const td_u16 sync_code[][MIPIRX_WDR_VC_NUM][MIPIRX_SYNC_CODE_NUM], td_u8 lane_num, td_u32 shift_code)
{
    td_u8 i;
    td_u32 *phy_addr = hal_mipirx_get_phy_addr(phy_id);
    td_s8 *real_lane_id = lane_id + MIPIRX_ONE_PHY_LANE_NUM * phy_id;

    for (i = 0; i < lane_num; i++) {
        if (real_lane_id[i] != -1) {
            switch (real_lane_id[i]) {
                case SIZE_0:
                    hal_mipirx_reg_writeb(phy_addr, REG_PHY_SYNC_SOF0_LINK, CFG_CIL_SOF0_WORD4_0_M, sync_code[i][0][0]);
                    hal_mipirx_reg_writeb(phy_addr, REG_PHY_SYNC_SOF0_LINK, CFG_CIL_SOF1_WORD4_0_M,
                        sync_code[i][0][0] + shift_code);
                    break;
                case SIZE_1:
                    hal_mipirx_reg_writeb(phy_addr, REG_PHY_SYNC_SOF1_LINK, CFG_CIL_SOF0_WORD4_1_M, sync_code[i][0][0]);
                    hal_mipirx_reg_writeb(phy_addr, REG_PHY_SYNC_SOF1_LINK, CFG_CIL_SOF1_WORD4_1_M,
                        sync_code[i][0][0] + shift_code);
                    break;
                case SIZE_2:
                    hal_mipirx_reg_writeb(phy_addr, REG_PHY_SYNC_SOF2_LINK, CFG_CIL_SOF0_WORD4_2_M, sync_code[i][0][0]);
                    hal_mipirx_reg_writeb(phy_addr, REG_PHY_SYNC_SOF2_LINK, CFG_CIL_SOF1_WORD4_2_M,
                        sync_code[i][0][0] + shift_code);
                    break;
                case SIZE_3:
                    hal_mipirx_reg_writeb(phy_addr, REG_PHY_SYNC_SOF3_LINK, CFG_CIL_SOF0_WORD4_3_M, sync_code[i][0][0]);
                    hal_mipirx_reg_writeb(phy_addr, REG_PHY_SYNC_SOF3_LINK, CFG_CIL_SOF1_WORD4_3_M,
                        sync_code[i][0][0] + shift_code);
                    break;
                default:
                    mipirx_log_err("mipirx phy lvds invalid lane id %u.", real_lane_id[i]);
                    break;
            }
        }
    }
}
#endif

td_void hal_mipirx_phy_get_phy_data(td_u8 phy_id, td_u32 *phy_lane_data, td_u16 size)
{
    td_u8 i;
    td_u32 *phy_addr = hal_mipirx_get_phy_addr(phy_id);

    for (i = 0; i < MIPIRX_LANE_MAX && i < size; i++) {
        switch (i) {
            case SIZE_0:
                phy_lane_data[i] = hal_mipirx_reg_readb(phy_addr, REG_PHY_DATA_LINK, CFG_PHY_DATA0_MIPI_M);
                break;
            case SIZE_1:
                phy_lane_data[i] = hal_mipirx_reg_readb(phy_addr, REG_PHY_DATA_LINK, CFG_PHY_DATA1_MIPI_M);
                break;
            case SIZE_2:
                phy_lane_data[i] = hal_mipirx_reg_readb(phy_addr, REG_PHY_DATA_LINK, CFG_PHY_DATA2_MIPI_M);
                break;
            case SIZE_3:
                phy_lane_data[i] = hal_mipirx_reg_readb(phy_addr, REG_PHY_DATA_LINK, CFG_PHY_DATA3_MIPI_M);
                break;
            default:
                mipirx_log_err("get phy data with invalid lane id %u.", i);
                break;
        }
    }
}

td_void hal_mipirx_phy_set_eq(td_u8 phy_id, td_u32 eq)
{
    td_u32 *phy_addr = hal_mipirx_get_phy_addr(phy_id);
    td_u32 *global_addr = hal_mipirx_get_global_addr();
    if (eq > 0xF) {
        return;
    }
    hal_mipirx_reg_writeb(phy_addr, REG_PHY_EQ_LINK, CFG_PHY_RG_EQ_CLK_M, eq);
    hal_mipirx_reg_writeb(global_addr, REG_PHYCFG_EN, CFG_PHYCIL0_CFG_EN_M << phy_id, TD_TRUE);
}

td_void hal_mipirx_phy_get_mipi_ph_data(td_u8 phy_id, td_u32 *data, td_u16 size)
{
    td_u8 i;
    td_u32 *phy_addr = hal_mipirx_get_phy_addr(phy_id);

    for (i = 0; i < MIPIRX_LANE_MAX && i < size; i++) {
        switch (i) {
            case SIZE_0:
                data[i] = hal_mipirx_reg_readb(phy_addr, REG_PHY_PH_MIPI_LINK, CFG_PHY_PH0_MIPI_M);
                break;
            case SIZE_1:
                data[i] = hal_mipirx_reg_readb(phy_addr, REG_PHY_PH_MIPI_LINK, CFG_PHY_PH1_MIPI_M);
                break;
            case SIZE_2:
                data[i] = hal_mipirx_reg_readb(phy_addr, REG_PHY_PH_MIPI_LINK, CFG_PHY_PH2_MIPI_M);
                break;
            case SIZE_3:
                data[i] = hal_mipirx_reg_readb(phy_addr, REG_PHY_PH_MIPI_LINK, CFG_PHY_PH3_MIPI_M);
                break;
            default:
                mipirx_log_err("get phy data with invalid lane id %u.", i);
                break;
        }
    }
}

td_char *hal_mipirx_phy_get_lane_mode(td_void)
{
    td_u32 *global_addr = hal_mipirx_get_global_addr();
    td_u32 mode;

    mode = hal_mipirx_reg_readb(global_addr, REG_HS_MODE_SELECT, CFG_HS_MODE_M);
    if (mode == 0xB) {
        return "2+2";
    } else if (mode == 0x7 || mode == 0x0 || mode == 0xA) {
        return "4";
    } else {
        return "Na";
    }
}

static td_void hal_mipirx_phy_count_escape_err(td_u32 err, td_u32 phy_id)
{
    if (err & CFG_ERR_ESCAPE_D0_ST_M) {
        phy_err_cnt[phy_id].d0_fsm_escape_err_cnt++;
    }
    if (err & CFG_ERR_ESCAPE_D1_ST_M) {
        phy_err_cnt[phy_id].d1_fsm_escape_err_cnt++;
    }
    if (err & CFG_ERR_ESCAPE_D2_ST_M) {
        phy_err_cnt[phy_id].d2_fsm_escape_err_cnt++;
    }
    if (err & CFG_ERR_ESCAPE_D3_ST_M) {
        phy_err_cnt[phy_id].d3_fsm_escape_err_cnt++;
    }
    if (err & CFG_ERR_ESCAPE_CK_ST_M) {
        phy_err_cnt[phy_id].clk_fsm_escape_err_cnt++;
    }
}

static td_void hal_mipirx_phy_count_tmout_err(td_u32 err, td_u32 phy_id)
{
    if (err & CFG_ERR_TIMEOUT_D0_ST_M) {
        phy_err_cnt[phy_id].d0_fsm_tmout_err_cnt++;
    }
    if (err & CFG_ERR_TIMEOUT_D1_ST_M) {
        phy_err_cnt[phy_id].d1_fsm_tmout_err_cnt++;
    }
    if (err & CFG_ERR_TIMEOUT_D2_ST_M) {
        phy_err_cnt[phy_id].d2_fsm_tmout_err_cnt++;
    }
    if (err & CFG_ERR_TIMEOUT_D3_ST_M) {
        phy_err_cnt[phy_id].d3_fsm_tmout_err_cnt++;
    }
    if (err & CFG_ERR_TIMEOUT_CK_ST_M) {
        phy_err_cnt[phy_id].clk_fsm_tmout_err_cnt++;
    }
}

td_void hal_mipirx_phy_count_err_irq(td_u8 phy_id)
{
    td_u32 *phy_addr = hal_mipirx_get_phy_addr(phy_id);
    td_u32 err;

    err = hal_mipirx_reg_read(phy_addr, REG_MIPI_CIL_INT_LINK);
    if (err == 0) {
        return;
    }

    hal_mipirx_reg_write(phy_addr, REG_MIPI_CIL_INT_RAW_LINK, 0xffffffff);
    hal_mipirx_phy_count_escape_err(err, phy_id);
    hal_mipirx_phy_count_tmout_err(err, phy_id);
}

td_void hal_mipirx_phy_clean_err_cnt(td_void)
{
    memset_s(phy_err_cnt, sizeof(phy_err_cnt), 0, sizeof(phy_err_cnt));
}

td_void hal_mipirx_phy_color_bar(td_bool enable, td_bool mipi_mode, td_bool is_stripe)
{
    td_u32 *global_addr = hal_mipirx_get_global_addr();
    hal_mipirx_reg_writeb(global_addr, REG_COLORBAR_CTRL_LINK0, CFG_COLORBAR_MODE_M, mipi_mode);
    hal_mipirx_reg_writeb(global_addr, REG_COLORBAR_CTRL_LINK0, CFG_COLORBAR_PATTERN_M, is_stripe);
    hal_mipirx_reg_writeb(global_addr, REG_COLORBAR_CTRL_LINK0, CFG_COLORBAR_EN_M, enable);
}

td_void hal_mipirx_phy_irq_open(td_u8 phy_id)
{
    td_u32 *phy_addr = hal_mipirx_get_phy_addr(phy_id);
    hal_mipirx_reg_write(phy_addr, REG_MIPI_CIL_INT_MSK_LINK, 0x1f1f);
}

td_u32 hal_mipirx_phy_get_freq(td_u8 phy_id)
{
    td_u32 *phy_addr = hal_mipirx_get_phy_addr(phy_id);
    td_u32 hs_cnt, cil_cnt;
    cil_cnt = hal_mipirx_reg_readb(phy_addr, REG_FREQ_MEASURE, CFG_CIL_CNT_M);
    hs_cnt = hal_mipirx_reg_readb(phy_addr, REG_FREQ_MEASURE, CFG_HS_CNT_M);
    if (hs_cnt == 0 || cil_cnt == 0) {
        return 0;
    }

    return hs_cnt * MIPI_CIL_FREQ / cil_cnt * SIZE_8; /* freq: MHZ >= 1MHZ */
}
