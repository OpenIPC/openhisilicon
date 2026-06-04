/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#include "drv_mipirx_comm.h"
#include "hal_mipirx_ctrl_reg.h"
#include "hal_mipirx_global_reg.h"
#include "hal_mipirx_sys_reg.h"
#include "hal_mipirx_comm.h"
#include "hal_mipirx_custom.h"
#include "hal_mipirx_lvds_ctrl.h"

#define LANE_OFFSET       0x20
#define DOL_LINE_INFO_LEF_N   0x0201
#define DOL_LINE_INFO_LEF_N_1 0x0211
#define DOL_LINE_INFO_SEF_N   0x0202
#define DOL_LINE_INFO_SEF_N_1 0x0212
#define LVDS_FRAME_CNT 10

static mipirx_lvds_err_cnt lvds_err_cnt[MIPIRX_PORT_NUM_MAX];

mipirx_lvds_err_cnt *hal_mipi_get_lvds_err_cnt(td_u8 port_id)
{
    if (port_id >= MIPIRX_PORT_NUM_MAX) {
        return TD_NULL;
    }
    return &lvds_err_cnt[port_id];
}

td_void hal_mipirx_lvds_set_cropping_area(td_u8 port_id, mipirx_img_rect *img_size,
    td_s32 enable, td_u8 total_lane_num)
{
    td_u32 *ctrl_addr = hal_mipirx_get_ctrl_addr(port_id);
    td_u32 width_per_lane, x_per_lane;
    if (total_lane_num == 0) {
        mipirx_log_err("lvds none lane\n");
        return;
    }
    hal_mipirx_reg_writeb(ctrl_addr, REG_LVDS_CTRL, CFG_LVDS_CROP_EN_M, enable);
    if (enable) {
        width_per_lane = osal_div_u64(img_size->width, total_lane_num);
        x_per_lane = osal_div_u64(img_size->x, total_lane_num);

        hal_mipirx_reg_writeb(ctrl_addr, REG_LVDS_CTRL, CFG_LVDS_CROP_EN_M, enable);

        hal_mipirx_reg_writeb(ctrl_addr, REG_LVDS_CROP_START0, CFG_LVDS_START_X0_LANE_M, x_per_lane);
        hal_mipirx_reg_writeb(ctrl_addr, REG_LVDS_CROP_START0, CFG_LVDS_START_Y0_M, img_size->y);

        hal_mipirx_reg_writeb(ctrl_addr, REG_LVDS_CROP_START1, CFG_LVDS_START_X1_LANE_M, x_per_lane);
        hal_mipirx_reg_writeb(ctrl_addr, REG_LVDS_CROP_START1, CFG_LVDS_START_Y1_M, img_size->y);

        hal_mipirx_reg_writeb(ctrl_addr, REG_LVDS_CROP_START2, CFG_LVDS_START_X0_LANE_M, x_per_lane);
        hal_mipirx_reg_writeb(ctrl_addr, REG_LVDS_CROP_START2, CFG_LVDS_START_Y0_M, img_size->y);

        hal_mipirx_reg_writeb(ctrl_addr, REG_LVDS_CROP_START3, CFG_LVDS_START_X0_LANE_M, x_per_lane);
        hal_mipirx_reg_writeb(ctrl_addr, REG_LVDS_CROP_START3, CFG_LVDS_START_Y0_M, img_size->y);

        hal_mipirx_reg_writeb(ctrl_addr, REG_LVDS_IMGSIZE, CFG_LVDS_IMGHEIGHT_M, img_size->height - 1);
        hal_mipirx_reg_writeb(ctrl_addr, REG_LVDS_IMGSIZE, CFG_LVDS_IMGWIDTH_LANE_M, width_per_lane - 1);
    }
}

static td_void mipirx_set_lvds_wdr_num(const td_u32 *ctrl_addr, mipirx_wdr_num_type wdr_num)
{
    switch (wdr_num) {
        case LVDS_WDR_NUM_NONE:
            hal_mipirx_reg_writeb(ctrl_addr, REG_LVDS_WDR, CFG_LVDS_WDR_NUM_M, 0);
            break;
        case LVDS_WDR_2F:
            hal_mipirx_reg_writeb(ctrl_addr, REG_LVDS_WDR, CFG_LVDS_WDR_NUM_M, 1);
            break;
        default:
            mipirx_log_err("invalid wdr num:%d.\n", wdr_num);
            break;
    }
}

static td_void hal_mipirx_lvds_set_dol_line_information(td_u32 *ctrl_addr, mipirx_wdr_num_type wdr_num)
{
    if (wdr_num == LVDS_WDR_2F) {
        hal_mipirx_reg_writeb(ctrl_addr, REG_LVDS_LI_WORD0, CFG_LI_WORD0_0_M, DOL_LINE_INFO_LEF_N);
        hal_mipirx_reg_writeb(ctrl_addr, REG_LVDS_LI_WORD0, CFG_LI_WORD0_1_M, DOL_LINE_INFO_LEF_N_1);
        hal_mipirx_reg_writeb(ctrl_addr, REG_LVDS_LI_WORD1, CFG_LI_WORD1_0_M, DOL_LINE_INFO_SEF_N);
        hal_mipirx_reg_writeb(ctrl_addr, REG_LVDS_LI_WORD1, CFG_LI_WORD1_1_M, DOL_LINE_INFO_SEF_N_1);
    } else {
        mipirx_log_err("mipirx lvds invalid wdr num.\n");
    }
}

td_void hal_mipirx_set_lvds_wdr_mode(td_u8 port_id, mipirx_lvds_wdr_attr *attr)
{
    td_u32 *ctrl_addr = hal_mipirx_get_ctrl_addr(port_id);

    mipirx_set_lvds_wdr_num(ctrl_addr, attr->wdr_num);

    if (attr->wdr_type == LVDS_WDR_NONE) {
        hal_mipirx_reg_writeb(ctrl_addr, REG_LVDS_WDR, CFG_LVDS_WDR_EN_M, 0);
    } else {
        hal_mipirx_reg_writeb(ctrl_addr, REG_LVDS_WDR, CFG_LVDS_WDR_EN_M, 1);
    }

    switch (attr->wdr_type) {
        case LVDS_WDR_INDEPENDENT_SOF:
            hal_mipirx_reg_writeb(ctrl_addr, REG_LVDS_WDR, CFG_LVDS_WDR_MODE_M, SIZE_0);
            break;
        case LVDS_WDR_SHARE_SOF:
            hal_mipirx_reg_writeb(ctrl_addr, REG_LVDS_WDR, CFG_LVDS_WDR_MODE_M, SIZE_2);
            break;
        case LVDS_WDR_DOL_4TH_CODE:
            // independent sync code
            hal_mipirx_reg_writeb(ctrl_addr, REG_LVDS_WDR, CFG_LVDS_WDR_MODE_M, SIZE_4);
            break;
        case LVDS_WDR_DOL_HCONNECT:
            // 4th code sync with shared sync code
            hal_mipirx_reg_writeb(ctrl_addr, REG_LVDS_WDR, CFG_LVDS_WDR_MODE_M, SIZE_5);
            hal_mipirx_reg_writeb(ctrl_addr, REG_LVDS_DOLSCD_HBLK, CFG_DOL_HBLANK2_M, attr->hblank2);
            hal_mipirx_reg_writeb(ctrl_addr, REG_LVDS_DOLSCD_HBLK, CFG_DOL_HBLANK1_M, attr->hblank1);
            break;
        case LVDS_WDR_DOL_5TH_CODE:
            hal_mipirx_reg_writeb(ctrl_addr, REG_LVDS_WDR, CFG_LVDS_WDR_MODE_M, SIZE_6);
            hal_mipirx_lvds_set_dol_line_information(ctrl_addr, attr->wdr_num);
            break;
        default:
            break;
    }
}

td_void hal_mipirx_set_lvds_ctrl_mode(td_u8 port_id, mipirx_lvds_cfg *lvds_attr,
    mipirx_data_type input_data_type)
{
    td_u32 *ctrl_addr = hal_mipirx_get_ctrl_addr(port_id);
    switch (input_data_type) {
        case MIPIRX_DATA_TYPE_RAW_8BIT:
            hal_mipirx_reg_writeb(ctrl_addr, REG_LVDS_CTRL, CFG_LVDS_RAW_TYPE_M, SIZE_1);
            break;
        case MIPIRX_DATA_TYPE_RAW_10BIT:
            hal_mipirx_reg_writeb(ctrl_addr, REG_LVDS_CTRL, CFG_LVDS_RAW_TYPE_M, SIZE_2);
            break;
        case MIPIRX_DATA_TYPE_RAW_12BIT:
            hal_mipirx_reg_writeb(ctrl_addr, REG_LVDS_CTRL, CFG_LVDS_RAW_TYPE_M, SIZE_3);
            break;
        case MIPIRX_DATA_TYPE_RAW_14BIT:
            hal_mipirx_reg_writeb(ctrl_addr, REG_LVDS_CTRL, CFG_LVDS_RAW_TYPE_M, SIZE_4);
            break;
        default:
            break;
    }
    hal_mipirx_reg_writeb(ctrl_addr, REG_LVDS_CTRL, CFG_LVDS_SYNC_MODE_M, lvds_attr->sync_mode);
    hal_mipirx_reg_writeb(ctrl_addr, REG_LVDS_CTRL, CFG_LVDS_PIX_BIG_ENDIAN_M, lvds_attr->data_endian);
    hal_mipirx_reg_writeb(ctrl_addr, REG_LVDS_CTRL, CFG_LVDS_CODE_BIG_ENDIAN_M, lvds_attr->sync_code_endian);
}

td_void hal_mipirx_set_lvds_data_rate(td_u8 port_id, mipirx_data_rate data_rate)
{
    td_u32 *ctrl_addr = hal_mipirx_get_ctrl_addr(port_id);
    if (data_rate == MIPIRX_DATA_RATE_X1) {
        hal_mipirx_reg_writeb(ctrl_addr, REG_LVDS_OUTPUT_PIX_NUM, CFG_LVDS_DOUBLE_PIX_EN_M, 0);
    }
}

td_void hal_mipirx_lvds_ctrl_set_cli_mode(td_u16 lane_bit_map)
{
    td_u32 *global_addr = hal_mipirx_get_global_addr();

    if (lane_bit_map & 0x5) {
        hal_mipirx_reg_writeb(global_addr, REG_PHYCFG_MODE, CFG_PHYCIL0_0_CFG_MODE_M, 1);
    }
    if (lane_bit_map & 0xa) {
        hal_mipirx_reg_writeb(global_addr, REG_PHYCFG_MODE, CFG_PHYCIL0_1_CFG_MODE_M, 1);
    }
    hal_mipirx_reg_writeb(global_addr, REG_PHYCFG_MODE, CFG_PHYCIL0_CFG_MODE_SEL_M, 0);
}

td_void hal_mipirx_lvds_ctrl_set_work_mode(td_u8 port_id)
{
    td_u32 *sys_addr = hal_mipirx_get_sys_addr();

    switch (port_id) {
        case 0:
            hal_mipirx_reg_writeb(sys_addr, REG_MIPI_WORK_MODE, CFG_MIPI0_WORK_MODE_M, 0x1);
            break;
        case 1:
            hal_mipirx_reg_writeb(sys_addr, REG_MIPI_WORK_MODE, CFG_MIPI1_WORK_MODE_M, 0x1);
            break;
        default:
            break;
    }
}

td_void hal_mipirx_lvds_ctrl_enable_clr_clock(td_u8 port_id, td_bool enable)
{
    td_u32 *global_addr = hal_mipirx_get_global_addr();
    switch (port_id) {
        case 0:
            hal_mipirx_reg_writeb(global_addr, REG_CHN0_CLR_EN, CFG_CHN0_CLR_EN_ALIGN_M, enable);
            hal_mipirx_reg_writeb(global_addr, REG_CHN0_CLR_EN, CFG_CHN0_CLR_EN_LVDS_M, enable);
            break;
        case 1:
            hal_mipirx_reg_writeb(global_addr, REG_CHN1_CLR_EN, CFG_CHN1_CLR_EN_ALIGN_M, enable);
            hal_mipirx_reg_writeb(global_addr, REG_CHN1_CLR_EN, CFG_CHN1_CLR_EN_LVDS_M, enable);
            break;
        default:
            mipirx_log_err("mipirx lvds%u invalid port id.\n", port_id);
            break;
    }
}

td_void hal_mipirx_lvds_ctrl_set_sync_code(td_u8 port_id, const td_s8 *lane_id,
    td_u16 sync_code[][MIPIRX_WDR_VC_NUM][MIPIRX_SYNC_CODE_NUM], td_u8 lane_num)
{
    td_u8 i;
    td_u32 *ctrl_addr = hal_mipirx_get_ctrl_addr(port_id);

    for (i = 0; i < lane_num; i++) {
        if (lane_id[i] != -1) {
            hal_mipirx_reg_writeb(ctrl_addr,
                REG_LVDS_LANE_SOF_01 + LANE_OFFSET * i, CFG_LANE_SOF_0_M, sync_code[i][SIZE_0][SIZE_0]);
            hal_mipirx_reg_writeb(ctrl_addr,
                REG_LVDS_LANE_SOF_01 + LANE_OFFSET * i, CFG_LANE_SOF_1_M, sync_code[i][SIZE_1][SIZE_0]);

            hal_mipirx_reg_writeb(ctrl_addr,
                REG_LVDS_LANE_EOF_01 + LANE_OFFSET * i, CFG_LANE_EOF_0_M, sync_code[i][SIZE_0][SIZE_1]);
            hal_mipirx_reg_writeb(ctrl_addr,
                REG_LVDS_LANE_EOF_01 + LANE_OFFSET * i, CFG_LANE_EOF_1_M, sync_code[i][SIZE_1][SIZE_1]);

            hal_mipirx_reg_writeb(ctrl_addr,
                REG_LVDS_LANE_SOL_01 + LANE_OFFSET * i, CFG_LANE_SOL_0_M, sync_code[i][SIZE_0][SIZE_2]);
            hal_mipirx_reg_writeb(ctrl_addr,
                REG_LVDS_LANE_SOL_01 + LANE_OFFSET * i, CFG_LANE_SOL_1_M, sync_code[i][SIZE_1][SIZE_2]);

            hal_mipirx_reg_writeb(ctrl_addr,
                REG_LVDS_LANE_EOL_01 + LANE_OFFSET * i, CFG_LANE_EOL_0_M, sync_code[i][SIZE_0][SIZE_3]);
            hal_mipirx_reg_writeb(ctrl_addr,
                REG_LVDS_LANE_EOL_01 + LANE_OFFSET * i, CFG_LANE_EOL_1_M, sync_code[i][SIZE_1][SIZE_3]);
        }
    }
}

td_void hal_mipirx_lvds_ctrl_set_nxt_sync_code(td_u8 port_id, const td_s8 *lane_id,
    const td_u16 nxt_sync_code[][MIPIRX_WDR_VC_NUM][MIPIRX_SYNC_CODE_NUM], td_u8 lane_num, td_u32 shift_code)
{
    td_u8 i;
    td_u32 *ctrl_addr = hal_mipirx_get_ctrl_addr(port_id);

    for (i = 0; i < lane_num; i++) {
        if (lane_id[i] != -1) {
            hal_mipirx_reg_writeb(ctrl_addr,
                REG_LVDS_LANE_NXT_SOF_01 + LANE_OFFSET * i, CFG_LANE_NXT_SOF_0_M,
                nxt_sync_code[i][SIZE_0][SIZE_0] + shift_code);
            hal_mipirx_reg_writeb(ctrl_addr,
                REG_LVDS_LANE_NXT_SOF_01 + LANE_OFFSET * i, CFG_LANE_NXT_SOF_1_M,
                nxt_sync_code[i][SIZE_1][SIZE_0] + shift_code);

            hal_mipirx_reg_writeb(ctrl_addr,
                REG_LVDS_LANE_NXT_EOF_01 + LANE_OFFSET * i, CFG_LANE_NXT_EOF_0_M,
                nxt_sync_code[i][SIZE_0][SIZE_1] + shift_code);
            hal_mipirx_reg_writeb(ctrl_addr,
                REG_LVDS_LANE_NXT_EOF_01 + LANE_OFFSET * i, CFG_LANE_NXT_EOF_1_M,
                nxt_sync_code[i][SIZE_1][SIZE_1] + shift_code);

            hal_mipirx_reg_writeb(ctrl_addr,
                REG_LVDS_LANE_NXT_SOL_01 + LANE_OFFSET * i, CFG_LANE_NXT_SOL_0_M,
                nxt_sync_code[i][SIZE_0][SIZE_2] + shift_code);
            hal_mipirx_reg_writeb(ctrl_addr,
                REG_LVDS_LANE_NXT_SOL_01 + LANE_OFFSET * i, CFG_LANE_NXT_SOL_1_M,
                nxt_sync_code[i][SIZE_1][SIZE_2] + shift_code);

            hal_mipirx_reg_writeb(ctrl_addr,
                REG_LVDS_LANE_NXT_EOL_01 + LANE_OFFSET * i, CFG_LANE_NXT_EOL_0_M,
                nxt_sync_code[i][SIZE_0][SIZE_3] + shift_code);
            hal_mipirx_reg_writeb(ctrl_addr,
                REG_LVDS_LANE_NXT_EOL_01 + LANE_OFFSET * i, CFG_LANE_NXT_EOL_1_M,
                nxt_sync_code[i][SIZE_1][SIZE_3] + shift_code);
        }
    }
}

td_void hal_mipirx_lvds_ctrl_get_lane_img_size(td_u8 port_id, td_u8 lane_id, td_u32 *width, td_u32 *height)
{
    td_u32 *ctrl_addr = hal_mipirx_get_ctrl_addr(port_id);

    if (lane_id >= MIPIRX_TOTAL_LANE_NUM_MAX) {
        return;
    }

    *width = hal_mipirx_reg_readb(ctrl_addr, REG_LVDS_LANE_IMGSIZE_STATIS + 0x4 * lane_id, CFG_LANE_IMGWIDTH_M) + 1;
    *height = hal_mipirx_reg_readb(ctrl_addr, REG_LVDS_LANE_IMGSIZE_STATIS + 0x4 * lane_id, CFG_LANE_IMGHEIGHT_M);
}

td_void hal_mipirx_lvds_ctrl_get_vc_img_size(td_u8 port_id, td_u8 vc, td_u32 *width, td_u32 *height)
{
    td_u32 *ctrl_addr = hal_mipirx_get_ctrl_addr(port_id);

    if (vc >= MIPIRX_VC_NUM_MAX) {
        return;
    }

    *width = hal_mipirx_reg_readb(ctrl_addr, REG_LVDS_IMGSIZE0_STATIS + 0x4 * vc, CFG_LVDS_IMGWIDTH0_M);
    *height = hal_mipirx_reg_readb(ctrl_addr, REG_LVDS_IMGSIZE0_STATIS + 0x4 * vc, CFG_LVDS_IMGHEIGHT0_M);
}

static td_void hal_mipirx_lvds_count_sync_err(td_u32 err, td_u32 port_id)
{
    if (err & CFG_LANE0_SYNC_ERR_ST_M) {
        lvds_err_cnt[port_id].lane0_sync_err_st++;
    }
    if (err & CFG_LANE1_SYNC_ERR_ST_M) {
        lvds_err_cnt[port_id].lane1_sync_err_st++;
    }
    if (err & CFG_LANE2_SYNC_ERR_ST_M) {
        lvds_err_cnt[port_id].lane2_sync_err_st++;
    }
    if (err & CFG_LANE3_SYNC_ERR_ST_M) {
        lvds_err_cnt[port_id].lane3_sync_err_st++;
    }
}

static td_void hal_mipirx_lvds_fifo_wr_err(td_u32 err, td_u8 port_id)
{
    td_u32 *ctrl_addr = hal_mipirx_get_ctrl_addr(port_id);
    if (err & CFG_LINK0_WR_ERR_ST_M) {
        lvds_err_cnt[port_id].link0_wr_err_st++;
    }
    if (err & CFG_LINK1_WR_ERR_ST_M) {
        lvds_err_cnt[port_id].link1_wr_err_st++;
    }
    if (err & CFG_LINK0_RD_ERR_ST_M) {
        lvds_err_cnt[port_id].link0_rd_err_st++;
    }
    if (err & CFG_LINK1_RD_ERR_ST_M) {
        lvds_err_cnt[port_id].link1_rd_err_st++;
    }
    if (err & CFG_LVDS_VSYNC_ST_M) {
        lvds_err_cnt[port_id].vsync_st++;
        if (lvds_err_cnt[port_id].vsync_st >= LVDS_FRAME_CNT) {
            hal_mipirx_reg_writeb(ctrl_addr, REG_LVDS_CTRL_INT_MSK, CFG_LVDS_VSYNC_MSK_M, 0);
        }
    }
}

td_void hal_mipirx_lvds_ctrl_err_irq_open(td_u8 port_id)
{
    td_u32 *ctrl_addr = hal_mipirx_get_ctrl_addr(port_id);
    hal_mipirx_reg_write(ctrl_addr, REG_LVDS_CTRL_INT_MSK, 0x1033000f);
}

td_void hal_mipirx_lvds_ctrl_count_err(td_u8 port_id)
{
    td_u32 *ctrl_addr = hal_mipirx_get_ctrl_addr(port_id);
    td_u32 err;

    err = hal_mipirx_reg_read(ctrl_addr, REG_LVDS_CTRL_INT);
    if (err != 0) {
        hal_mipirx_reg_write(ctrl_addr, REG_LVDS_CTRL_INT_RAW, 0xFFFFFFFF);
        hal_mipirx_lvds_count_sync_err(err, port_id);
        hal_mipirx_lvds_fifo_wr_err(err, port_id);
    }
}

td_void hal_mipirx_lvds_ctrl_clean_err_cnt(td_void)
{
    memset_s(lvds_err_cnt, sizeof(lvds_err_cnt), 0, sizeof(lvds_err_cnt));
}