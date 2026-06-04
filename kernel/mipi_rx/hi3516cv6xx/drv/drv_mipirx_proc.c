/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#include "drv_mipirx_proc.h"
#include "drv_mipirx_comm.h"
#include "hal_mipirx_phy.h"
#include "drv_mipirx_port.h"
#include "hal_mipirx_custom.h"
#include "hal_mipirx_mipi_ctrl.h"
#include "hal_mipirx_crg.h"
#include "hal_mipirx_sensor.h"
#include "hal_mipirx_comm.h"
#ifdef CONFIG_OT_MIPIRX_LVDS_SUPPORT
#include "hal_mipirx_lvds_ctrl.h"
#endif

#define MIPIRX_PROC_LINE_LENGTH    120
#define MIPIRX_PROC_LINE_INDENT_LENGTH    50
#define MIPIRX_PROC_EMPTY_ELEM "                             |"
#define MIPIRX_PROC_DIAGNOSIS   "%d:port%d %-111s|\n"
#define MIPIRX_PROC_LINE        "%-119s|\n"
#define MIPIRX_ECHO_0_PARAM_FMT "echo %-31s > /proc/umap/mipi_rx   %-75s\n"
#define MIPIRX_ECHO_1_PARAM_FMT "echo %-12s %-17d  > /proc/umap/mipi_rx   %-30s %-75s\n"
#define MIPIRX_ECHO_2_PARAM_FMT "echo %-12s %-5d %-11d  > /proc/umap/mipi_rx   %-30s %-75s\n"
#define MIPIRX_ECHO_3_PARAM_FMT "echo %-12s %-5d %-5d %-5d  > /proc/umap/mipi_rx   %-30s %-75s\n"
#define MIPIRX_PHY_DATA_DELAY 100
#define MIPIRX_INT_DELAY 1

#define mipirx_array_size(x) (sizeof(x) / sizeof((x)[0]))

static td_u32 mipirx_proc_check_max(td_u32 in, td_u32 max)
{
    td_u32 out;

    if (max == 0) {
        mipirx_log_err("array max size error!\n");
        return 0;
    }

    out = (in >= (max - 1)) ? (max - 1) : in;

    return out;
}

static td_void mipirx_proc_print_wrap(td_void *s, td_u8 *proc_num)
{
    td_u32 i;
    for (i = 0; i < SIZE_4 - *proc_num; ++i) {
        osal_seq_printf(s, "%s", MIPIRX_PROC_EMPTY_ELEM);
    }
    osal_seq_printf(s, "\n");
    *proc_num = 0;
}

static td_void mipirx_proc_print_title(td_void *s, td_u8 *proc_num, const char *title)
{
    td_u32 i;

    if ((s == TD_NULL) || (title == TD_NULL) || (strlen(title) > MIPIRX_PROC_LINE_LENGTH)) {
        return;
    }

    if (*proc_num != 0) {
        mipirx_proc_print_wrap(s, proc_num);
        *proc_num = 0;
    }

    for (i = 0; i < MIPIRX_PROC_LINE_INDENT_LENGTH; i++) { /* 2:half */
        osal_seq_printf(s, "-");
    }

    osal_seq_printf(s, "%s", title);
    for (i = 0; i < MIPIRX_PROC_LINE_LENGTH - strlen(title) - MIPIRX_PROC_LINE_INDENT_LENGTH; i++) { /* 2:half */
        osal_seq_printf(s, "-");
    }
    osal_seq_printf(s, "\n");
}

static td_void mipirx_proc_print_audo_wrap(td_void *s, td_u8 *proc_num)
{
    ++*proc_num;
    if (*proc_num == SIZE_4) {
        mipirx_proc_print_wrap(s, proc_num);
    }
}

static td_void mipirx_proc_print_hex(td_void *s, td_u8 *proc_num, td_char *key, td_u32 value)
{
    if (key == TD_NULL) {
        return;
    }

    osal_seq_printf(s, "%-17s:0x%-9x|", key, value);
    mipirx_proc_print_audo_wrap(s, proc_num);
}

static td_void mipirx_proc_print_u32(td_void *s, td_u8 *proc_num, td_char *key, td_u32 value)
{
    if (key == TD_NULL) {
        return;
    }

    osal_seq_printf(s, "%-17s:%-11u|", key, value);
    mipirx_proc_print_audo_wrap(s, proc_num);
}

static td_void mipirx_proc_print_str(td_void *s, td_u8 *proc_num, td_char *key, const td_char *value)
{
    if (key == TD_NULL || value == TD_NULL) {
        return;
    }

    osal_seq_printf(s, "%-17s:%-11s|", key, value);
    mipirx_proc_print_audo_wrap(s, proc_num);
}

#ifdef CONFIG_OT_MIPIRX_DEBUG_CMD_SUPPORT
static td_u32 mipirx_drv_proc_str_check(const td_char *src)
{
    td_u32 len;
    if (src == NULL) {
        return 0;
    }
    len = strlen(src);
    if (len == 0) {
        return 0;
    }
    if (src[len - 1] == '\n') {
        len--;
    }
    return len;
}

static td_u32 mipirx_drv_proc_trans_hex_str_to_dec(const td_char *src)
{
    td_u32 sum = 0;
    td_u32 i = 0;
    td_u32 len;
    td_u32 base = 10; /* 10: num*10 */
    len = mipirx_drv_proc_str_check(src);
    if (len == 0) {
        return 0;
    }
    if ((len >= 2) && src[i] == '0' && src[i + 1] == 'x') { /* 2: skip "0x" 2 words */
        i += 2; /* 2: move ahead 2 bytes */
        base = 0x10; /* 0x10: num*0x10 */
    }
    if (len > 10) { /* 10: at most 10 words, like: 0xffffffff */
        len = 10; /* 10: at most handle front 19 words */
    }
    for (; i < len; ++i) {
        if (src[i] >= '0' && src[i] <= '9') {
            sum = sum * base + (src[i] - '0');        /* 10: times 10, means carry forword */
        } else if (src[i] >= 'a' && src[i] <= 'f') {
            sum = sum * 0x10 + (src[i] - 'a' + 10); /* 10: 'a' value equals 10 */
        } else if (src[i] >= 'A' && src[i] <= 'F') {
            sum = sum * 0x10 + (src[i] - 'A' + 10); /* 10: 'A' regard as 'a' */
        } else {
            return 0;
        }
    }
    return sum;
}
#endif

static td_void mipirx_proc_print_version(td_void *s)
{
    osal_seq_printf(s, "\nModule: [MIPI_RX], Build Time["__DATE__
                                ", "__TIME__
                                "]\n\n");
}

static td_void mipirx_proc_print_lane_mode(td_void *s, td_u8 *proc_num)
{
    mipirx_proc_print_title(s, proc_num, "lane_mode");
    mipirx_proc_print_str(s, proc_num, "lane_mode", hal_mipirx_phy_get_lane_mode());
    mipirx_proc_print_wrap(s, proc_num);
}

static td_void mipirx_proc_print_work_mode(td_void *s, td_u8 *proc_num, mipirx_work_mode mode)
{
    td_char *mode_s[MIPIRX_INPUT_MODE_MAX + 1] = {"mipi", "sublvds", "lvds", "hispi", "Na"};
    mode = mipirx_proc_check_max(mode, mipirx_array_size(mode_s));
    mipirx_proc_print_str(s, proc_num, "work_mode", mode_s[mode]);
}

static td_void mipirx_proc_print_data_rate(td_void *s, td_u8 *proc_num, mipirx_data_rate rate)
{
    td_char *rate_s[MIPIRX_DATA_RATE_MAX + 1] = {"x1", "x2", "Na"};
    rate = mipirx_proc_check_max(rate, mipirx_array_size(rate_s));
    mipirx_proc_print_str(s, proc_num, "data_rate", rate_s[rate]);
}

static void mipirx_proc_get_data_type(td_void *s, td_u8 *proc_num, mipirx_data_type data_type)
{
    td_char *type_s[MIPIRX_DATA_TYPE_MAX + 1] = {
        "raw_8", "raw_10", "raw_12", "raw_14", "420_8_N", "420_8_L", "422_8", "422_P", "Na"
    };
    data_type = mipirx_proc_check_max(data_type, mipirx_array_size(type_s));
    mipirx_proc_print_str(s, proc_num, "data_type", type_s[data_type]);
}

static td_void mipirx_proc_print_img_rect(td_void *s, td_u8 *proc_num, mipirx_img_rect *img_conf)
{
    mipirx_proc_print_u32(s, proc_num, "img_rect_x", img_conf->x);
    mipirx_proc_print_u32(s, proc_num, "img_rect_y", img_conf->y);
    mipirx_proc_print_u32(s, proc_num, "img_width", img_conf->width);
    mipirx_proc_print_u32(s, proc_num, "img_height", img_conf->height);
}

static td_void mipirx_proc_print_lane_id(td_void *s, td_u8 *proc_num, td_s8 lane_id[], int lane_num)
{
    td_s32 ret;
    td_char lane_str[16] = {0}; // 16: max length

    if (lane_num == MIPIRX_LANE_NUM) {
        ret = snprintf_s(lane_str, sizeof(lane_str), sizeof(lane_str) - 1, "%d,%d,%d,%d",
            lane_id[SIZE_0], lane_id[SIZE_1], lane_id[SIZE_2], lane_id[SIZE_3]);
        if (ret >= 0) {
            mipirx_proc_print_str(s, proc_num, "lane_id", lane_str);
        }
    }
}

static td_void mipirx_proc_print_mipi_wdr(td_void *s, td_u8 *proc_num, mipirx_mipi_cfg *mipi_attr)
{
    td_char *wdr_mode_s[MIPI_WDR_MODE_MAX + 1] = {
        "none", "VC", "DT", "DOL", "Na"
    };

    mipi_attr->wdr_mode = mipirx_proc_check_max(mipi_attr->wdr_mode, mipirx_array_size(wdr_mode_s));
    mipirx_proc_print_str(s, proc_num, "wdr_mode", wdr_mode_s[mipi_attr->wdr_mode]);
    if (mipi_attr->wdr_mode == MIPI_WDR_MODE_DT) {
        mipirx_proc_print_u32(s, proc_num, "wdr_dt_1", mipi_attr->wdr_data_type[0]);
        mipirx_proc_print_u32(s, proc_num, "wdr_dt_2", mipi_attr->wdr_data_type[1]);
    }
}

static td_void mipirx_proc_print_ext_data(td_void *s, td_u8 *proc_num, mipirx_mipi_udf_dt *ext_data)
{
    td_u32 i;
    td_char *ext_data_type_s[MIPIRX_MAX_EXT_DATA_TYPE_NUM] = {"ext_dt_1", "ext_dt_2", "ext_dt_3"};
    td_char *ext_data_width[MIPIRX_MAX_EXT_DATA_TYPE_NUM] = {"ext_dt_width_1", "ext_dt_width_2", "ext_dt_width_3"};

    if (ext_data->num > MIPIRX_MAX_EXT_DATA_TYPE_NUM) {
        return;
    }

    for (i = 0; i < ext_data->num && i < MIPIRX_MAX_EXT_DATA_TYPE_NUM; ++i) {
        mipirx_proc_print_hex(s, proc_num, ext_data_type_s[i], ext_data->data_type[i]);
        mipirx_proc_print_u32(s, proc_num, ext_data_width[i], ext_data->data_bit_width[i]);
    }
}

static td_void mipirx_proc_print_mipi_attr(td_void *s, td_u8 *proc_num, mipirx_port_cfg *mipi)
{
    mipirx_proc_print_data_rate(s, proc_num, mipi->data_rate);
    mipirx_proc_get_data_type(s, proc_num, mipi->data_type);
    mipirx_proc_print_img_rect(s, proc_num, &mipi->img_rect);
    mipirx_proc_print_lane_id(s, proc_num, mipi->lane_id, MIPIRX_LANE_NUM);
    mipirx_proc_print_mipi_wdr(s, proc_num, &mipi->mipi_attr);
    mipirx_proc_print_ext_data(s, proc_num, &mipi->mipi_attr.udf_dt);
}

#ifdef CONFIG_OT_MIPIRX_LVDS_SUPPORT
static td_void mipirx_proc_print_endian(td_void *s, td_u8 *proc_num, mipirx_lvds_cfg *lvds_attr)
{
    td_char *endian_s[2] = {"LITTLE", "BIG"}; // 2: length
    lvds_attr->data_endian = mipirx_proc_check_max(lvds_attr->data_endian, mipirx_array_size(endian_s));
    lvds_attr->sync_code_endian = mipirx_proc_check_max(lvds_attr->sync_code_endian, mipirx_array_size(endian_s));
    mipirx_proc_print_str(s, proc_num, "data_endian", endian_s[lvds_attr->data_endian]);
    mipirx_proc_print_str(s, proc_num, "sync_code_endian", endian_s[lvds_attr->sync_code_endian]);
}

static td_void mipirx_proc_print_sync_mode(td_void *s, td_u8 *proc_num, mipirx_lvds_sync_mode sync_mode)
{
    td_char *sync_mode_s[MIPIRX_LVDS_SYNC_MODE_MAX] = {"SOF", "SAV"};

    sync_mode = mipirx_proc_check_max(sync_mode, mipirx_array_size(sync_mode_s));
    mipirx_proc_print_str(s, proc_num, "sync_mode", sync_mode_s[sync_mode]);
}

static td_void mipirx_proc_print_lvds_wdr(td_void *s, td_u8 *proc_num, mipirx_lvds_wdr_attr *wdr_attr)
{
    td_char *wdr_type_s[LVDS_WDR_TYPE_MAX] = {"none", "IND_SOF", "SHARE_SOF", "DOL_4TH", "DOL_HCONN", "DOL_5TH"};

    wdr_attr->wdr_type = mipirx_proc_check_max(wdr_attr->wdr_type, mipirx_array_size(wdr_type_s));
    mipirx_proc_print_str(s, proc_num, "wdr_type", wdr_type_s[wdr_attr->wdr_type]);
    if (wdr_attr->wdr_num == 0) {
        return;
    }

    mipirx_proc_print_u32(s, proc_num, "wdr_num", 2); // 2: wdr num
    if (wdr_attr->wdr_type == LVDS_WDR_DOL_HCONNECT) {
        mipirx_proc_print_u32(s, proc_num, "hblank1", wdr_attr->hblank1);
        mipirx_proc_print_u32(s, proc_num, "hblank2", wdr_attr->hblank2);
    }
}

static td_void mipirx_proc_print_sync_code(td_void *s, td_u8 *proc_num, mipirx_lvds_cfg *lvds_attr)
{
    td_char tmp_s[18] = {0}; // 18: length
    td_u32 lane_num, vc_num, code_num;
    td_s32 ret;
    mipirx_proc_print_wrap(s, proc_num);
    for (lane_num = 0; lane_num < MIPIRX_LANE_NUM; ++lane_num) {
        for (vc_num = 0; vc_num < MIPIRX_WDR_VC_NUM; ++vc_num) {
            for (code_num = 0; code_num < MIPIRX_SYNC_CODE_NUM; ++code_num) {
                ret = snprintf_s(tmp_s, sizeof(tmp_s), sizeof(tmp_s) - 1, "L%d_VC%d_scode_%d", lane_num, vc_num,
                    code_num);
                mipirx_proc_print_hex(s, proc_num, tmp_s, lvds_attr->sync_code[lane_num][vc_num][code_num]);
            }
        }
    }
    if (ret < 0) {
        mipirx_log_err("failed to print sync code.\n");
    }
}

static td_void mipirx_proc_print_lvds_attr(td_void *s, td_u8 *proc_num, mipirx_port_cfg *port_cfg)
{
    mipirx_proc_print_data_rate(s, proc_num, port_cfg->data_rate);
    mipirx_proc_get_data_type(s, proc_num, port_cfg->data_type);
    mipirx_proc_print_img_rect(s, proc_num, &port_cfg->img_rect);
    mipirx_proc_print_lane_id(s, proc_num, port_cfg->lane_id, MIPIRX_LANE_NUM);
    mipirx_proc_print_sync_mode(s, proc_num, port_cfg->lvds_attr.sync_mode);
    mipirx_proc_print_endian(s, proc_num, &port_cfg->lvds_attr);
    mipirx_proc_print_lvds_wdr(s, proc_num, &port_cfg->lvds_attr.wdr_attr);
    mipirx_proc_print_sync_code(s, proc_num, &port_cfg->lvds_attr);
}
#endif

static td_void mipirx_proc_print_port_attr(td_void *s, td_u8 *proc_num, mipirx_port_ctx *port_ctx, td_u32 port_id)
{
    mipirx_proc_print_title(s, proc_num, "port_attr");
    mipirx_proc_print_u32(s, proc_num, "port_id", port_id);
    mipirx_proc_print_work_mode(s, proc_num, port_ctx->mipirx_port_cfg[port_id].work_mode);
    if (port_ctx->mipirx_port_cfg[port_id].work_mode == MIPIRX_INPUT_MODE_MIPI) {
        mipirx_proc_print_mipi_attr(s, proc_num, &(port_ctx->mipirx_port_cfg[port_id]));
    } else {
#ifdef CONFIG_OT_MIPIRX_LVDS_SUPPORT
        mipirx_proc_print_lvds_attr(s, proc_num, &(port_ctx->mipirx_port_cfg[port_id]));
#endif
    }
}

static td_void mipirx_proc_print_clk_stat(td_void *s, td_u8 *proc_num, td_u32 port_id)
{
    td_bool sensor_clk, mipi_clk;
    sensor_clk = hal_mipirx_get_sensor_clk_stat(port_id);
    mipi_clk = hal_mipirx_crg_get_pix_clock_stat(port_id);
    mipirx_proc_print_title(s, proc_num, "clk_status");
    mipirx_proc_print_str(s, proc_num, "sensor_clk", sensor_clk ? "Y" : "N");
    mipirx_proc_print_str(s, proc_num, "mipi_clk", mipi_clk ? "Y" : "N");
}

#ifdef CONFIG_OT_MIPIRX_FULL_PROC
static td_void mipirx_proc_print_phy_fsm(td_void *s, td_u8 *proc_num, td_u8 phy_id)
{
    mipirx_proc_print_str(s, proc_num, "cil_d0_cur_stat", hal_mipirx_phy_get_lane_cur_fsm_stat(phy_id, 0x0));
    mipirx_proc_print_str(s, proc_num, "cil_d0_nxt_stat", hal_mipirx_phy_get_lane_next_fsm_stat(phy_id, 0x0));
    mipirx_proc_print_str(s, proc_num, "cil_d1_cur_stat", hal_mipirx_phy_get_lane_cur_fsm_stat(phy_id, 0x1));
    mipirx_proc_print_str(s, proc_num, "cil_d1_nxt_stat", hal_mipirx_phy_get_lane_next_fsm_stat(phy_id, 0x1));
    mipirx_proc_print_str(s, proc_num, "cil_d2_cur_stat", hal_mipirx_phy_get_lane_cur_fsm_stat(phy_id, 0x2));
    mipirx_proc_print_str(s, proc_num, "cil_d2_nxt_stat", hal_mipirx_phy_get_lane_next_fsm_stat(phy_id, 0x2));
    mipirx_proc_print_str(s, proc_num, "cil_d3_cur_stat", hal_mipirx_phy_get_lane_cur_fsm_stat(phy_id, 0x3));
    mipirx_proc_print_str(s, proc_num, "cil_d3_nxt_stat", hal_mipirx_phy_get_lane_next_fsm_stat(phy_id, 0x3));
    mipirx_proc_print_str(s, proc_num, "cil_clk_cur_stat", hal_mipirx_phy_get_clk_cur_fsm_stat(phy_id, 0x0));
    mipirx_proc_print_str(s, proc_num, "cil_clk_nxt_stat", hal_mipirx_phy_get_clk_next_fsm_stat(phy_id, 0x0));
    mipirx_proc_print_str(s, proc_num, "cil_clk2_cur_stat", hal_mipirx_phy_get_clk_cur_fsm_stat(phy_id, 0x1));
    mipirx_proc_print_str(s, proc_num, "cil_clk2_nxt_stat", hal_mipirx_phy_get_clk_next_fsm_stat(phy_id, 0x1));
}
#endif

static td_void mipirx_proc_print_phy_info(td_void *s, td_u8 *proc_num, mipirx_port_ctx *port_ctx, td_u8 port_id)
{
    td_u32 phy_id;
    for (phy_id = 0; phy_id < MIPIRX_PHY_NUM_MAX; ++phy_id) {
        if (!mipirx_check_phy_link_port(phy_id, &port_ctx->lane_bit_map[port_id])) {
            continue;
        }
        mipirx_proc_print_title(s, proc_num, "phy_info");
        mipirx_proc_print_u32(s, proc_num, "phy_id", phy_id);
        mipirx_proc_print_str(s, proc_num, "phy_en", hal_mipirx_phy_get_enable_stat(phy_id) ? "Y" : "N");
        mipirx_proc_print_wrap(s, proc_num);
        mipirx_proc_print_str(s, proc_num, "phy_d0_term_en", hal_mipirx_phy_get_lane_term_on(phy_id, 0x0) ? "Y" : "N");
        mipirx_proc_print_str(s, proc_num, "phy_d1_term_en", hal_mipirx_phy_get_lane_term_on(phy_id, 0x1) ? "Y" : "N");
        mipirx_proc_print_str(s, proc_num, "phy_d2_term_en", hal_mipirx_phy_get_lane_term_on(phy_id, 0x2) ? "Y" : "N");
        mipirx_proc_print_str(s, proc_num, "phy_d3_term_en", hal_mipirx_phy_get_lane_term_on(phy_id, 0x3) ? "Y" : "N");
        mipirx_proc_print_str(s, proc_num, "lane0_en", hal_mipirx_phy_get_lane_en(0x0) ? "Y" : "N");
        mipirx_proc_print_str(s, proc_num, "lane1_en", hal_mipirx_phy_get_lane_en(0x1) ? "Y" : "N");
        mipirx_proc_print_str(s, proc_num, "lane2_en", hal_mipirx_phy_get_lane_en(0x2) ? "Y" : "N");
        mipirx_proc_print_str(s, proc_num, "lane3_en", hal_mipirx_phy_get_lane_en(0x3) ? "Y" : "N");
#ifdef CONFIG_OT_MIPIRX_FULL_PROC
        mipirx_proc_print_hex(s, proc_num, "lane0_map", hal_mipirx_ctrl_get_lane_map(port_id, 0x0));
        mipirx_proc_print_hex(s, proc_num, "lane1_map", hal_mipirx_ctrl_get_lane_map(port_id, 0x1));
        mipirx_proc_print_hex(s, proc_num, "lane2_map", hal_mipirx_ctrl_get_lane_map(port_id, 0x2));
        mipirx_proc_print_hex(s, proc_num, "lane3_map", hal_mipirx_ctrl_get_lane_map(port_id, 0x3));
        mipirx_proc_print_phy_fsm(s, proc_num, phy_id);
#endif
    }
}

static td_void mipirx_porc_print_lane_data(td_void *s, td_u8 *proc_num, td_u8 phy_id)
{
    td_u32 lane_data[MIPIRX_TOTAL_LANE_NUM_MAX] = {0};
    hal_mipirx_phy_get_phy_data(phy_id, lane_data, MIPIRX_TOTAL_LANE_NUM_MAX);
    mipirx_proc_print_hex(s, proc_num, "lane0_data", lane_data[0x0]);
    mipirx_proc_print_hex(s, proc_num, "lane1_data", lane_data[0x1]);
    mipirx_proc_print_hex(s, proc_num, "lane2_data", lane_data[0x2]);
    mipirx_proc_print_hex(s, proc_num, "lane3_data", lane_data[0x3]);
}

static td_void mipirx_proc_print_mipi_ph_data(td_void *s, td_u8 *proc_num, td_u8 phy_id)
{
    td_u32 ph_data[MIPIRX_TOTAL_LANE_NUM_MAX] = {0};
    hal_mipirx_phy_get_mipi_ph_data(phy_id, ph_data, MIPIRX_TOTAL_LANE_NUM_MAX);
    mipirx_proc_print_hex(s, proc_num, "mipi_ph_d0", ph_data[0x0]);
    mipirx_proc_print_hex(s, proc_num, "mipi_ph_d1", ph_data[0x1]);
    mipirx_proc_print_hex(s, proc_num, "mipi_ph_d2", ph_data[0x2]);
    mipirx_proc_print_hex(s, proc_num, "mipi_ph_d3", ph_data[0x3]);
}

static td_void mipirx_proc_print_mipi_data(td_void *s, td_u8 *proc_num, td_u8 phy_id)
{
    td_u32 mipi_data[MIPIRX_TOTAL_LANE_NUM_MAX] = {0};
    hal_mipirx_phy_get_mipi_data(phy_id, mipi_data, MIPIRX_TOTAL_LANE_NUM_MAX);
#ifdef CONFIG_OT_MIPIRX_FULL_PROC
    mipirx_proc_print_hex(s, proc_num, "mipi_data0", mipi_data[0x0]);
    mipirx_proc_print_hex(s, proc_num, "mipi_data1", mipi_data[0x1]);
    mipirx_proc_print_hex(s, proc_num, "mipi_data2", mipi_data[0x2]);
    mipirx_proc_print_hex(s, proc_num, "mipi_data3", mipi_data[0x3]);
#endif
}

static td_void mipirx_proc_print_mipi_img_size(td_void *s, td_u8 *proc_num, td_u8 port_id)
{
    mipirx_img_rect img_size = {0};
    hal_mipirx_mipi_ctrl_get_mipi_imgsize(port_id, 0x0, &img_size);
    mipirx_proc_print_u32(s, proc_num, "mipi_vc0_w", img_size.width);
    mipirx_proc_print_u32(s, proc_num, "mipi_vc0_h", img_size.height);
    memset_s(&img_size, sizeof(img_size), 0, sizeof(img_size));
    hal_mipirx_mipi_ctrl_get_mipi_imgsize(port_id, 0x1, &img_size);
    mipirx_proc_print_u32(s, proc_num, "mipi_vc1_w", img_size.width);
    mipirx_proc_print_u32(s, proc_num, "mipi_vc1_h", img_size.height);
    memset_s(&img_size, sizeof(img_size), 0, sizeof(img_size));
    hal_mipirx_mipi_ctrl_get_mipi_imgsize(port_id, 0x2, &img_size);
    mipirx_proc_print_u32(s, proc_num, "mipi_vc2_w", img_size.width);
    mipirx_proc_print_u32(s, proc_num, "mipi_vc2_h", img_size.height);
    memset_s(&img_size, sizeof(img_size), 0, sizeof(img_size));
    hal_mipirx_mipi_ctrl_get_mipi_imgsize(port_id, 0x3, &img_size);
    mipirx_proc_print_u32(s, proc_num, "mipi_vc3_w", img_size.width);
    mipirx_proc_print_u32(s, proc_num, "mipi_vc3_h", img_size.height);
}

#ifdef CONFIG_OT_MIPIRX_LVDS_SUPPORT
static td_void mipirx_proc_print_lvds_data(td_void *s, td_u8 *proc_num, td_u8 phy_id)
{
    td_u32 data[MIPIRX_TOTAL_LANE_NUM_MAX] = {0};

    hal_mipirx_phy_get_lvds_data(phy_id, data, MIPIRX_TOTAL_LANE_NUM_MAX);
    mipirx_proc_print_hex(s, proc_num, "lvds_data0", data[0x0]);
    mipirx_proc_print_hex(s, proc_num, "lvds_data1", data[0x1]);
    mipirx_proc_print_hex(s, proc_num, "lvds_data2", data[0x2]);
    mipirx_proc_print_hex(s, proc_num, "lvds_data3", data[0x3]);
}

static td_void mipirx_proc_print_lvds_lane_size(td_void *s, td_u8 *proc_num, td_u8 port_id)
{
    td_u32 width = 0, height = 0;
    hal_mipirx_lvds_ctrl_get_lane_img_size(port_id, 0x0, &width, &height);
    mipirx_proc_print_u32(s, proc_num, "lvds_l0_width", width);
    mipirx_proc_print_u32(s, proc_num, "lvds_l0_height", height);
    hal_mipirx_lvds_ctrl_get_lane_img_size(port_id, 0x1, &width, &height);
    mipirx_proc_print_u32(s, proc_num, "lvds_l1_width", width);
    mipirx_proc_print_u32(s, proc_num, "lvds_l1_height", height);
    hal_mipirx_lvds_ctrl_get_lane_img_size(port_id, 0x2, &width, &height);
    mipirx_proc_print_u32(s, proc_num, "lvds_l2_width", width);
    mipirx_proc_print_u32(s, proc_num, "lvds_l2_height", height);
    hal_mipirx_lvds_ctrl_get_lane_img_size(port_id, 0x3, &width, &height);
    mipirx_proc_print_u32(s, proc_num, "lvds_l3_width", width);
    mipirx_proc_print_u32(s, proc_num, "lvds_l3_height", height);
}

static td_void mipirx_proc_print_lvds_vc_size(td_void *s, td_u8 *proc_num, td_u8 port_id)
{
    td_u32 width = 0, height = 0;
    hal_mipirx_lvds_ctrl_get_vc_img_size(port_id, 0x0, &width, &height);
    mipirx_proc_print_u32(s, proc_num, "lvds_vc0_w", width);
    mipirx_proc_print_u32(s, proc_num, "lvds_vc0_h", height);
    hal_mipirx_lvds_ctrl_get_vc_img_size(port_id, 0x1, &width, &height);
    mipirx_proc_print_u32(s, proc_num, "lvds_vc1_w", width);
    mipirx_proc_print_u32(s, proc_num, "lvds_vc1_h", height);
    hal_mipirx_lvds_ctrl_get_vc_img_size(port_id, 0x2, &width, &height);
    mipirx_proc_print_u32(s, proc_num, "lvds_vc2_w", width);
    mipirx_proc_print_u32(s, proc_num, "lvds_vc2_h", height);
    hal_mipirx_lvds_ctrl_get_vc_img_size(port_id, 0x3, &width, &height);
    mipirx_proc_print_u32(s, proc_num, "lvds_vc3_w", width);
    mipirx_proc_print_u32(s, proc_num, "lvds_vc3_h", height);
}

static td_void mipirx_proc_print_lvds_pix_num(td_void *s, td_u8 *proc_num, td_u8 port_id)
{
    td_u32 data[0x2] = {0};
    hal_mipirx_phy_get_lvds_pix_num(port_id, data, 0x2);
    mipirx_proc_print_u32(s, proc_num, "lvds_l02_pix_num", data[0]);
    mipirx_proc_print_u32(s, proc_num, "lvds_l13_pix_num", data[1]);
}
#endif

static td_void mipirx_proc_print_phy_freq(td_void *s, td_u8 *proc_num, td_u8 phy_id)
{
    td_u32 freq = hal_mipirx_phy_get_freq(phy_id);
    td_char tmp_s[18] = {0}; // 18: length
    td_s32 ret;
    if (freq == 0) {
        mipirx_proc_print_str(s, proc_num, "freq_measure", "n/a");
    } else {
        ret = snprintf_s(tmp_s, sizeof(tmp_s), sizeof(tmp_s) - 1, "%dMHz", freq);
        if (ret >= 0) {
            mipirx_proc_print_str(s, proc_num, "freq_measure", tmp_s);
        }
    }
}

static td_void mipirx_proc_print_detect_data(td_void *s, td_u8 *proc_num, mipirx_port_ctx *port_ctx, td_u8 port_id)
{
    td_u32 phy_id;
    for (phy_id = 0; phy_id < MIPIRX_PHY_NUM_MAX; ++phy_id) {
        if (!mipirx_check_phy_link_port(phy_id, &port_ctx->lane_bit_map[port_id])) {
            continue;
        }
        mipirx_proc_print_title(s, proc_num, "data_detect");
        mipirx_proc_print_u32(s, proc_num, "phy_id", phy_id);
        mipirx_proc_print_phy_freq(s, proc_num, phy_id);
        mipirx_proc_print_wrap(s, proc_num);
        mipirx_porc_print_lane_data(s, proc_num, phy_id);
        if (port_ctx->mipirx_port_cfg[port_id].work_mode == MIPIRX_INPUT_MODE_MIPI) {
            mipirx_proc_print_mipi_ph_data(s, proc_num, phy_id);
            mipirx_proc_print_mipi_data(s, proc_num, phy_id);
            mipirx_proc_print_mipi_img_size(s, proc_num, port_id);
        } else {
#ifdef CONFIG_OT_MIPIRX_LVDS_SUPPORT
            mipirx_proc_print_lvds_data(s, proc_num, phy_id);
            mipirx_proc_print_lvds_lane_size(s, proc_num, port_id);
            mipirx_proc_print_lvds_vc_size(s, proc_num, port_id);
            mipirx_proc_print_lvds_pix_num(s, proc_num, port_id);
#endif
        }
    }
}

td_void mipirx_proc_print_align_err(td_void *s, td_u8 *proc_num, td_u32 port_id)
{
    mipirx_align_err *cnt = hal_mipi_get_align_err_ctx(port_id);
    if (cnt == TD_NULL) {
        return;
    }
    mipirx_proc_print_title(s, proc_num, "align_err");
    mipirx_proc_print_u32(s, proc_num, "fifo_ovfl", cnt->fifo_ovfl);
    mipirx_proc_print_u32(s, proc_num, "lane0_fifo_ovfl", cnt->lane0_fifo_ovfl);
    mipirx_proc_print_u32(s, proc_num, "lane1_fifo_ovfl", cnt->lane1_fifo_ovfl);
    mipirx_proc_print_u32(s, proc_num, "lane2_fifo_ovfl", cnt->lane2_fifo_ovfl);
    mipirx_proc_print_u32(s, proc_num, "lane3_fifo_ovfl", cnt->lane3_fifo_ovfl);
    mipirx_proc_print_title(s, proc_num, "diagnosis");
}

#ifdef CONFIG_OT_MIPIRX_LVDS_SUPPORT
td_void mipirx_proc_print_lvds_err_cnt(td_void *s, td_u8 *proc_num, td_u32 port_id)
{
    mipirx_lvds_err_cnt *cnt = hal_mipi_get_lvds_err_cnt(port_id);
    if (cnt == TD_NULL) {
        return;
    }
    mipirx_proc_print_title(s, proc_num, "lvds_err");
    mipirx_proc_print_u32(s, proc_num, "rd_fifo_err_lk0", cnt->link0_rd_err_st);
    mipirx_proc_print_u32(s, proc_num, "wr_fifo_err_lk0", cnt->link0_wr_err_st);
    mipirx_proc_print_u32(s, proc_num, "rd_fifo_err_lk1", cnt->link1_rd_err_st);
    mipirx_proc_print_u32(s, proc_num, "wr_fifo_err_lk1", cnt->link1_wr_err_st);
    mipirx_proc_print_u32(s, proc_num, "lane0_sync_err", cnt->lane0_sync_err_st);
    mipirx_proc_print_u32(s, proc_num, "lane1_sync_err", cnt->lane1_sync_err_st);
    mipirx_proc_print_u32(s, proc_num, "lane2_sync_err", cnt->lane2_sync_err_st);
    mipirx_proc_print_u32(s, proc_num, "lane3_sync_err", cnt->lane3_sync_err_st);
    mipirx_proc_print_u32(s, proc_num, "vsync_cnt", cnt->vsync_st);
}
#endif

td_void mipirx_proc_print_mipi_err_cnt(td_void *s, td_u8 *proc_num, td_u32 port_id)
{
    mipirx_mipi_err_cnt *cnt = hal_mipi_get_ctl_err_cnt_ctx(port_id);
    if (cnt == TD_NULL) {
        return;
    }
    mipirx_proc_print_title(s, proc_num, "mipi_err");
    mipirx_proc_print_u32(s, proc_num, "vc0_crc_err", cnt->vc0_err_crc_cnt);
    mipirx_proc_print_u32(s, proc_num, "vc1_crc_err", cnt->vc1_err_crc_cnt);
    mipirx_proc_print_u32(s, proc_num, "vc2_crc_err", cnt->vc2_err_crc_cnt);
    mipirx_proc_print_u32(s, proc_num, "vc3_crc_err", cnt->vc3_err_crc_cnt);
    mipirx_proc_print_u32(s, proc_num, "vc0_ecc_err", cnt->vc0_err_ecc_cnt);
    mipirx_proc_print_u32(s, proc_num, "vc1_ecc_err", cnt->vc1_err_ecc_cnt);
    mipirx_proc_print_u32(s, proc_num, "vc2_ecc_err", cnt->vc2_err_ecc_cnt);
    mipirx_proc_print_u32(s, proc_num, "vc3_ecc_err", cnt->vc3_err_ecc_cnt);
#ifdef CONFIG_OT_MIPIRX_FULL_PROC
    mipirx_proc_print_u32(s, proc_num, "vc0_frm_num_err", cnt->vc0_err_frame_num_cnt);
    mipirx_proc_print_u32(s, proc_num, "vc1_frm_num_err", cnt->vc1_err_frame_num_cnt);
    mipirx_proc_print_u32(s, proc_num, "vc2_frm_num_err", cnt->vc2_err_frame_num_cnt);
    mipirx_proc_print_u32(s, proc_num, "vc3_frm_num_err", cnt->vc3_err_frame_num_cnt);
    mipirx_proc_print_u32(s, proc_num, "vc0_frm_mismatch", cnt->vc0_err_frame_s_e_num_mismatch_cnt);
    mipirx_proc_print_u32(s, proc_num, "vc1_frm_mismatch", cnt->vc1_err_frame_s_e_num_mismatch_cnt);
    mipirx_proc_print_u32(s, proc_num, "vc2_frm_mismatch", cnt->vc2_err_frame_s_e_num_mismatch_cnt);
    mipirx_proc_print_u32(s, proc_num, "vc3_frm_mismatch", cnt->vc3_err_frame_s_e_num_mismatch_cnt);
#endif
    mipirx_proc_print_u32(s, proc_num, "vsync_cnt", cnt->vsync_cnt);
    mipirx_proc_print_u32(s, proc_num, "rd_data_fifo_err", cnt->data_fifo_rderr_cnt);
    mipirx_proc_print_u32(s, proc_num, "wr_data_fifo_err", cnt->data_fifo_wrerr_cnt);
    mipirx_proc_print_u32(s, proc_num, "rd_cmd_fifo_err", cnt->cmd_fifo_rderr_cnt);
    mipirx_proc_print_u32(s, proc_num, "wr_cmd_fifo_err", cnt->cmd_fifo_wrerr_cnt);
    mipirx_proc_print_u32(s, proc_num, "data_fifo_n_emt", cnt->data_fifo_not_empty_cnt);
    mipirx_proc_print_u32(s, proc_num, "cmd_fifo_n_emt", cnt->cmd_fifo_not_empty_cnt);
}

#ifdef CONFIG_OT_MIPIRX_FULL_PROC
td_void mipirx_proc_print_phy_err_irq(td_void *s, td_u8 *proc_num, mipirx_port_ctx *port_ctx, td_u32 port_id)
{
    mipirx_phy_err_cnt *cnt = TD_NULL;
    td_u32 phy_id;
    for (phy_id = 0; phy_id < MIPIRX_PHY_NUM_MAX; ++phy_id) {
        if (!mipirx_check_phy_link_port(phy_id, &port_ctx->lane_bit_map[port_id])) {
            continue;
        }
        cnt = hal_mipi_get_phy_err_cnt_ctx(phy_id);
        if (cnt == TD_NULL) {
            return;
        }
        mipirx_proc_print_title(s, proc_num, "phy_err_irq");
        mipirx_proc_print_u32(s, proc_num, "phy_id", phy_id);
        mipirx_proc_print_wrap(s, proc_num);
        mipirx_proc_print_u32(s, proc_num, "clk_fsm_tmout", cnt->clk_fsm_tmout_err_cnt);
        mipirx_proc_print_u32(s, proc_num, "clk_fsm_escape", cnt->clk_fsm_escape_err_cnt);
        mipirx_proc_print_u32(s, proc_num, "d0_fsm_tmout", cnt->d0_fsm_tmout_err_cnt);
        mipirx_proc_print_u32(s, proc_num, "d1_fsm_tmout", cnt->d1_fsm_tmout_err_cnt);
        mipirx_proc_print_u32(s, proc_num, "d2_fsm_tmout", cnt->d2_fsm_tmout_err_cnt);
        mipirx_proc_print_u32(s, proc_num, "d3_fsm_tmout", cnt->d3_fsm_tmout_err_cnt);
        mipirx_proc_print_u32(s, proc_num, "d0_err_escape", cnt->d0_fsm_escape_err_cnt);
        mipirx_proc_print_u32(s, proc_num, "d1_err_escape", cnt->d1_fsm_escape_err_cnt);
        mipirx_proc_print_u32(s, proc_num, "d2_err_escape", cnt->d2_fsm_escape_err_cnt);
        mipirx_proc_print_u32(s, proc_num, "d3_err_escape", cnt->d3_fsm_escape_err_cnt);
    }
}
#endif

static td_void drv_mipirx_diag_phy_data(td_void *s, td_u8 phy_id)
{
    td_u8 i;
    td_u32 phy_lane_data[MIPIRX_TOTAL_LANE_NUM_MAX] = {0};
    td_u32 phy_lane_data_nxt[MIPIRX_TOTAL_LANE_NUM_MAX] = {0};
    td_u32 phy_data = 0;
    td_u32 phy_data_nxt = 0;
    hal_mipirx_phy_get_phy_data(phy_id, phy_lane_data, MIPIRX_TOTAL_LANE_NUM_MAX);
    osal_msleep(MIPIRX_PHY_DATA_DELAY);
    hal_mipirx_phy_get_phy_data(phy_id, phy_lane_data_nxt, MIPIRX_TOTAL_LANE_NUM_MAX);
    for (i = 0; i < MIPIRX_TOTAL_LANE_NUM_MAX; i++) {
        phy_data = phy_data | (phy_lane_data[i] << (i * SIZE_8));
        phy_data_nxt = phy_data_nxt | (phy_lane_data_nxt[i] << (i * SIZE_8));
    }

    if (phy_data == 0 && phy_data_nxt == 0) {
        osal_seq_printf(s, "There is no data input for the phy module. Please check the phy configuration.\n");
    }
}

static td_void drv_mipirx_diag_mipi_data(td_void *s, td_u8 phy_id)
{
    td_u8 i;
    td_u32 mipi_lane_data[MIPIRX_TOTAL_LANE_NUM_MAX] = {0};
    td_u32 mipi_lane_data_nxt[MIPIRX_TOTAL_LANE_NUM_MAX] = {0};
    td_u32 mipi_data = 0;
    td_u32 mipi_data_nxt = 0;
    hal_mipirx_phy_get_mipi_data(phy_id, mipi_lane_data, MIPIRX_TOTAL_LANE_NUM_MAX);
    osal_msleep(MIPIRX_PHY_DATA_DELAY);
    hal_mipirx_phy_get_mipi_data(phy_id, mipi_lane_data_nxt, MIPIRX_TOTAL_LANE_NUM_MAX);
    for (i = 0; i < MIPIRX_TOTAL_LANE_NUM_MAX; i++) {
        mipi_data = mipi_data | (mipi_lane_data[i] << (i * SIZE_8));
        mipi_data_nxt = mipi_data_nxt | (mipi_lane_data_nxt[i] << (i * SIZE_8));
    }

    if (mipi_data == 0 && mipi_data_nxt == 0) {
        osal_seq_printf(s, "There is no data input for the mipi module. Please check the mipi configuration.\n");
    }
}

#ifdef CONFIG_OT_MIPIRX_LVDS_SUPPORT
static td_void drv_mipirx_diag_lvds_data(td_void *s, td_u8 phy_id)
{
    td_u8 i;
    td_u32 lvds_lane_data[MIPIRX_TOTAL_LANE_NUM_MAX] = {0};
    td_u32 lvds_lane_data_nxt[MIPIRX_TOTAL_LANE_NUM_MAX] = {0};
    td_u32 lvds_data = 0;
    td_u32 lvds_data_nxt = 0;
    hal_mipirx_phy_get_lvds_data(phy_id, lvds_lane_data, MIPIRX_TOTAL_LANE_NUM_MAX);
    osal_msleep(MIPIRX_PHY_DATA_DELAY);
    hal_mipirx_phy_get_lvds_data(phy_id, lvds_lane_data_nxt, MIPIRX_TOTAL_LANE_NUM_MAX);
    for (i = 0; i < MIPIRX_TOTAL_LANE_NUM_MAX; i++) {
        lvds_data = lvds_data | (lvds_lane_data[i] << (i * SIZE_8));
        lvds_data_nxt = lvds_data_nxt | (lvds_lane_data_nxt[i] << (i * SIZE_8));
    }

    if (lvds_data == 0 && lvds_data_nxt == 0) {
        osal_seq_printf(s, "There is no data input for the lvds module. Please check the lvds configuration.\n");
    }
}
#endif

static td_void drv_mipirx_diag_lane_align(td_void *s, td_u8 port_id)
{
    mipirx_align_err *align_cnt;
    mipirx_align_err *align_cnt_nxt;

    align_cnt = hal_mipi_get_align_err_ctx(port_id);
    if (align_cnt == TD_NULL) {
        return;
    }
    osal_msleep(MIPIRX_INT_DELAY);
    align_cnt_nxt = hal_mipi_get_align_err_ctx(port_id);
    if (align_cnt_nxt == TD_NULL) {
        return;
    }
    if (align_cnt_nxt->fifo_ovfl > align_cnt->fifo_ovfl ||
        align_cnt_nxt->lane0_fifo_ovfl > align_cnt->lane0_fifo_ovfl ||
        align_cnt_nxt->lane1_fifo_ovfl > align_cnt->lane1_fifo_ovfl ||
        align_cnt_nxt->lane2_fifo_ovfl > align_cnt->lane2_fifo_ovfl ||
        align_cnt_nxt->lane3_fifo_ovfl > align_cnt->lane3_fifo_ovfl) {
        osal_seq_printf(s, "Some of the lanes may not have data flows. Please check whether"
            "receiver and transmitter have the correct number of the lanes enabled.\n");
    }
}
static td_void drv_mipirx_diag_crc(td_void *s, td_u8 port_id)
{
    mipirx_mipi_err_cnt *crc_cnt;
    mipirx_mipi_err_cnt *crc_cnt_nxt;

    crc_cnt = hal_mipi_get_ctl_err_cnt_ctx(port_id);
    if (crc_cnt == TD_NULL) {
        return;
    }
    osal_msleep(MIPIRX_INT_DELAY);
    crc_cnt_nxt = hal_mipi_get_ctl_err_cnt_ctx(port_id);
    if (crc_cnt_nxt->vc0_err_crc_cnt > crc_cnt->vc0_err_crc_cnt ||
        crc_cnt_nxt->vc1_err_crc_cnt > crc_cnt->vc1_err_crc_cnt ||
        crc_cnt_nxt->vc2_err_crc_cnt > crc_cnt->vc2_err_crc_cnt ||
        crc_cnt_nxt->vc3_err_crc_cnt > crc_cnt->vc3_err_crc_cnt ||
        crc_cnt_nxt->vc0_err_ecc_cnt > crc_cnt->vc0_err_ecc_cnt ||
        crc_cnt_nxt->vc1_err_ecc_cnt > crc_cnt->vc1_err_ecc_cnt ||
        crc_cnt_nxt->vc2_err_ecc_cnt > crc_cnt->vc2_err_ecc_cnt ||
        crc_cnt_nxt->vc3_err_ecc_cnt > crc_cnt->vc3_err_ecc_cnt) {
        osal_seq_printf(s, "The signal quality of the hardware may not be good. "
            "Please check the hardware design or the transmitter's parameter.\n");
    }

    if (crc_cnt_nxt->data_fifo_wrerr_cnt > crc_cnt->data_fifo_wrerr_cnt ||
        crc_cnt_nxt->cmd_fifo_wrerr_cnt > crc_cnt->cmd_fifo_wrerr_cnt) {
        osal_seq_printf(s, "The data flow is too fast to read. The data flow speed may exceed our capacity. "
            "Please reduce the data flow speed by using smaller resolution or smaller bit width.\n");
    }
}

static td_void mipirx_proc_print_port(td_void *s, td_u8 *proc_num, mipirx_port_ctx *port_ctx, td_u32 port_id)
{
    mipirx_proc_print_port_attr(s, proc_num, port_ctx, port_id);
    mipirx_proc_print_clk_stat(s, proc_num, port_id);
    mipirx_proc_print_phy_info(s, proc_num, port_ctx, port_id);
    mipirx_proc_print_detect_data(s, proc_num, port_ctx, port_id);
#ifdef CONFIG_OT_MIPIRX_FULL_PROC
    mipirx_proc_print_phy_err_irq(s, proc_num, port_ctx, port_id);
#endif
    if (port_ctx->mipirx_port_cfg[port_id].work_mode == MIPIRX_INPUT_MODE_MIPI) {
        mipirx_proc_print_mipi_err_cnt(s, proc_num, port_id);
    } else {
#ifdef CONFIG_OT_MIPIRX_LVDS_SUPPORT
        mipirx_proc_print_lvds_err_cnt(s, proc_num, port_id);
#endif
    }
    mipirx_proc_print_align_err(s, proc_num, port_id);
}

static td_void mipirx_diag_port(td_void *s, td_u8 *proc_num, mipirx_port_ctx *port_ctx, td_u32 port_id)
{
    td_u32 phy_id;
    for (phy_id = 0; phy_id < MIPIRX_PHY_NUM_MAX; ++phy_id) {
        if (!mipirx_check_phy_link_port(phy_id, &port_ctx->lane_bit_map[port_id])) {
            continue;
        }
        drv_mipirx_diag_phy_data(s, phy_id);
        if (port_ctx->mipirx_port_cfg[port_id].work_mode == MIPIRX_INPUT_MODE_MIPI) {
            drv_mipirx_diag_mipi_data(s, phy_id);
        } else {
#ifdef CONFIG_OT_MIPIRX_LVDS_SUPPORT
            drv_mipirx_diag_lvds_data(s, phy_id);
#endif
        }
        drv_mipirx_diag_lane_align(s, port_id);
        drv_mipirx_diag_crc(s, port_id);
    }
}

td_s32 drv_mipirx_proc_read(osal_proc_entry *proc_entry)
{
    td_void *s;
    mipirx_port_ctx *mipi_ctx;
    td_u32 port_id;
    td_u8 proc_num = 0;

    if (proc_entry == TD_NULL || proc_entry->private_data == TD_NULL) {
        return TD_FAILURE;
    }

    s = proc_entry->seqfile;
    mipi_ctx = (mipirx_port_ctx *)proc_entry->private_data;

    mipirx_proc_print_version(s);
    mipirx_proc_print_lane_mode(s, &proc_num);
    if (mipi_ctx->lane_mode == LANE_DIVIDE_MODE_MAX) {
        return TD_SUCCESS;
    }
    for (port_id = 0; port_id < MIPIRX_PORT_NUM_MAX; ++port_id) {
        if (mipi_ctx->mipirx_port_cfg[port_id].work_mode == MIPIRX_INPUT_MODE_MAX) {
            continue;
        }
        mipirx_proc_print_port(s, &proc_num, mipi_ctx, port_id);
        mipirx_diag_port(s, &proc_num, mipi_ctx, port_id);
    }
    return TD_SUCCESS;
}

#ifdef CONFIG_OT_MIPIRX_DEBUG_CMD_SUPPORT
static td_bool drv_mipirx_debug_check_para_err(td_u32 argc, td_char (*argv)[PROC_CMD_SINGEL_LENGTH_MAX],
    td_void *private, td_u32 param_num)
{
    mipirx_port_ctx *mipi_ctx;
    td_u32 port_id;

    if (argv == TD_NULL || argc < param_num || private == TD_NULL) {
        return TD_TRUE;
    }

    mipi_ctx = (mipirx_port_ctx *) private;
    port_id = mipirx_drv_proc_trans_hex_str_to_dec(argv[SIZE_1]);
    if (port_id >= MIPIRX_PORT_NUM_MAX) {
        OT_PRINT("mipirx debug input port id error!\n");
        return TD_TRUE;
    }
    if (mipi_ctx->mipirx_port_cfg[port_id].work_mode == MIPIRX_INPUT_MODE_MAX) {
        OT_PRINT("work mode is not configured!\n");
        return TD_TRUE;
    }

    return TD_FALSE;
}

static td_void drv_mipirx_debug_cfg_lanes(mipirx_port_ctx *mipirx_ctx, td_u8 port_id)
{
    td_u8 phy_id;
    td_u8 all_port_lane_map;
    mipirx_lane_bit_map *cur_lane;
    td_s8 *lanes = TD_NULL;

    cur_lane = &mipirx_ctx->lane_bit_map[port_id];
    lanes = mipirx_ctx->mipirx_port_cfg[port_id].lane_id;
    mipirx_get_phy_lane_bit_map(lanes, MIPIRX_LANE_NUM, cur_lane, MIPIRX_PHY_NUM_MAX);

    for (phy_id = 0; phy_id < MIPIRX_PHY_NUM_MAX; ++phy_id) {
        all_port_lane_map = mipirx_get_phy_lane_bit(phy_id, mipirx_ctx->lane_bit_map, MIPIRX_PORT_NUM_MAX);
        hal_mipirx_phy_cfg_lanes(phy_id, all_port_lane_map);
    }

    hal_mipirx_mipi_ctrl_set_lane_id(port_id, lanes, MIPIRX_LANE_NUM);
    hal_mipirx_mipi_ctrl_set_cli_mode(mipirx_get_lane_bit_map(lanes, MIPIRX_LANE_NUM));
}

static td_s32 drv_mipirx_debug_open_lane(td_u32 argc, td_char (*argv)[PROC_CMD_SINGEL_LENGTH_MAX], td_void *private)
{
    mipirx_port_ctx *mipirx_ctx;
    td_u32 port_id;
    td_u8 lane_index;
    td_u8 lane_value;
    td_s8 *lanes = TD_NULL;

    if (drv_mipirx_debug_check_para_err(argc, argv, private, SIZE_4)) {
        return TD_FAILURE;
    }

    port_id = mipirx_drv_proc_trans_hex_str_to_dec(argv[SIZE_1]);
    lane_index = mipirx_drv_proc_trans_hex_str_to_dec(argv[SIZE_2]);
    lane_value = mipirx_drv_proc_trans_hex_str_to_dec(argv[SIZE_3]);
    if (lane_index >= MIPIRX_TOTAL_LANE_NUM_MAX || (td_s8) lane_value >= MIPIRX_TOTAL_LANE_NUM_MAX) {
        OT_PRINT("mipirx debug input para error!\n");
        return TD_FAILURE;
    }

    mipirx_ctx = (mipirx_port_ctx *) private;
    lanes = mipirx_ctx->mipirx_port_cfg[port_id].lane_id;
    lanes[lane_index] = (td_s8) lane_value;

    drv_mipirx_debug_cfg_lanes(mipirx_ctx, port_id);

    OT_PRINT("mipirx debug open lane success!\n");
    return TD_SUCCESS;
}

static td_s32 drv_mipirx_debug_close_lane(td_u32 argc, td_char (*argv)[PROC_CMD_SINGEL_LENGTH_MAX], td_void *private)
{
    mipirx_port_ctx *mipirx_ctx;
    td_u32 port_id;
    td_u8 lane_index;
    td_s8 *lanes = TD_NULL;

    if (drv_mipirx_debug_check_para_err(argc, argv, private, SIZE_3)) {
        return TD_FAILURE;
    }

    port_id = mipirx_drv_proc_trans_hex_str_to_dec(argv[SIZE_1]);
    lane_index = mipirx_drv_proc_trans_hex_str_to_dec(argv[SIZE_2]);
    if (lane_index >= MIPIRX_TOTAL_LANE_NUM_MAX) {
        OT_PRINT("mipirx debug input para error!\n");
        return TD_FAILURE;
    }

    mipirx_ctx = (mipirx_port_ctx *) private;
    lanes = mipirx_ctx->mipirx_port_cfg[port_id].lane_id;
    lanes[lane_index] = -1;
    drv_mipirx_debug_cfg_lanes(mipirx_ctx, port_id);

    OT_PRINT("mipirx debug close lane success!");
    return TD_SUCCESS;
}

static td_s32 drv_mipirx_debug_color_bar(td_u32 argc, td_char (*argv)[PROC_CMD_SINGEL_LENGTH_MAX], td_void *private)
{
    td_bool enable;
    td_bool stripe;
    td_bool mode;

    if (argv == TD_NULL || argc < SIZE_4 || private == TD_NULL) {
        return TD_TRUE;
    }
    enable = mipirx_drv_proc_trans_hex_str_to_dec(argv[SIZE_1]) != 0;
    stripe = mipirx_drv_proc_trans_hex_str_to_dec(argv[SIZE_2]) != 0;
    mode = mipirx_drv_proc_trans_hex_str_to_dec(argv[SIZE_3]) != 0;
    hal_mipirx_phy_color_bar(enable, mode, stripe);
    
    OT_PRINT("mipirx debug config color bar success!");
    return TD_SUCCESS;
}

#ifdef CONFIG_OT_MIPIRX_LVDS_SUPPORT
static td_void debug_crop_lvds_xy(mipirx_port_ctx *mipi_ctx, mipirx_img_rect *img_conf,
    td_u32 port_id, td_u32 x, td_u32 y)
{
    td_u32 lane_num = 0;
    td_u32 index;

    for (index = 0; index < MIPIRX_TOTAL_LANE_NUM_MAX; ++index) {
        if (mipi_ctx->mipirx_port_cfg[port_id].lane_id[index] >= 0) {
            ++lane_num;
        }
    }
    img_conf = &mipi_ctx->mipirx_port_cfg[port_id].img_rect;
    img_conf->x = x;
    img_conf->y = y;
    hal_mipirx_lvds_set_cropping_area(port_id, img_conf, TD_TRUE, lane_num);
}
#endif

static td_s32 drv_mipirx_debug_crop_xy(td_u32 argc, td_char (*argv)[PROC_CMD_SINGEL_LENGTH_MAX], td_void *private)
{
    mipirx_port_ctx *mipi_ctx;
    td_u32 port_id;
    td_u32 x;
    td_u32 y;
    mipirx_img_rect *img_conf;

    mipi_ctx = (mipirx_port_ctx *) private;
    if (drv_mipirx_debug_check_para_err(argc, argv, private, SIZE_4)) {
        return TD_FAILURE;
    }

    port_id = mipirx_drv_proc_trans_hex_str_to_dec(argv[SIZE_1]);
    x = mipirx_drv_proc_trans_hex_str_to_dec(argv[SIZE_2]);
    y = mipirx_drv_proc_trans_hex_str_to_dec(argv[SIZE_3]);
    if (mipi_ctx->mipirx_port_cfg[port_id].work_mode == MIPIRX_INPUT_MODE_MIPI) {
        img_conf = &mipi_ctx->mipirx_port_cfg[port_id].img_rect;
        img_conf->x = x;
        img_conf->y = y;
        hal_mipirx_mipi_ctrl_set_rect(port_id, img_conf, TD_TRUE);
    } else {
#ifdef CONFIG_OT_MIPIRX_LVDS_SUPPORT
        debug_crop_lvds_xy(mipi_ctx, img_conf, port_id, x, y);
#endif
    }

    OT_PRINT("mipirx debug set crop x y success!");
    return TD_SUCCESS;
}

#ifdef CONFIG_OT_MIPIRX_LVDS_SUPPORT
static td_void debug_crop_lvds_wh(mipirx_port_ctx *mipi_ctx, mipirx_img_rect *img_conf,
    td_u32 port_id, td_u32 width, td_u32 height)
{
    td_u32 lane_num = 0;
    td_u32 index;

    for (index = 0; index < MIPIRX_TOTAL_LANE_NUM_MAX; ++index) {
        if (mipi_ctx->mipirx_port_cfg[port_id].lane_id[index] >= 0) {
            ++lane_num;
        }
    }
    img_conf = &mipi_ctx->mipirx_port_cfg[port_id].img_rect;
    img_conf->width = width;
    img_conf->height = height;
    hal_mipirx_lvds_set_cropping_area(port_id, img_conf, TD_TRUE, lane_num);
}
#endif

static td_s32 drv_mipirx_debug_crop_wh(td_u32 argc, td_char (*argv)[PROC_CMD_SINGEL_LENGTH_MAX], td_void *private)
{
    mipirx_port_ctx *mipi_ctx;
    td_u32 port_id;
    td_u32 width;
    td_u32 height;
    mipirx_img_rect *img_conf;

    if (drv_mipirx_debug_check_para_err(argc, argv, private, SIZE_4)) {
        return TD_FAILURE;
    }
    mipi_ctx = (mipirx_port_ctx *) private;
    port_id = mipirx_drv_proc_trans_hex_str_to_dec(argv[SIZE_1]);
    width = mipirx_drv_proc_trans_hex_str_to_dec(argv[SIZE_2]);
    height = mipirx_drv_proc_trans_hex_str_to_dec(argv[SIZE_3]);
    if (mipi_ctx->mipirx_port_cfg[port_id].work_mode == MIPIRX_INPUT_MODE_MIPI) {
        img_conf = &mipi_ctx->mipirx_port_cfg[port_id].img_rect;
        img_conf->width = width;
        img_conf->height = height;
        hal_mipirx_mipi_ctrl_set_rect(port_id, img_conf, TD_TRUE);
    } else {
#ifdef CONFIG_OT_MIPIRX_LVDS_SUPPORT
        debug_crop_lvds_wh(mipi_ctx, img_conf, port_id, width, height);
#endif
    }

    OT_PRINT("mipirx debug set crop width height success!");
    return TD_SUCCESS;
}

static td_s32 drv_mipirx_debug_set_eq(td_u32 argc, td_char (*argv)[PROC_CMD_SINGEL_LENGTH_MAX], td_void *private)
{
    mipirx_port_ctx *mipi_ctx;
    td_u32 port_id;
    td_u32 eq;
    td_u8 phy_id;

    if (argv == TD_NULL || argc < SIZE_2 || private == TD_NULL) {
        return TD_TRUE;
    }

    mipi_ctx = (mipirx_port_ctx *) private;
    port_id = mipirx_drv_proc_trans_hex_str_to_dec(argv[SIZE_1]);
    
    for (phy_id = 0; phy_id < MIPIRX_PHY_NUM_MAX; ++phy_id) {
        if (mipi_ctx->lane_bit_map[port_id].phy_lane_bit[phy_id] != 0) {
            hal_mipirx_phy_set_eq(phy_id, eq);
        }
    }

    OT_PRINT("mipirx debug set eq success!");
    return TD_SUCCESS;
}

static td_s32 drv_mipirx_debug_clean_err_cnt(td_u32 argc, td_char (*argv)[PROC_CMD_SINGEL_LENGTH_MAX], td_void *private)
{
    ot_unused(argc);
    ot_unused(argv);
    ot_unused(private);
    hal_mipirx_phy_clean_err_cnt();
    hal_mipirx_mipi_ctrl_clean_err_cnt();
#ifdef CONFIG_OT_MIPIRX_LVDS_SUPPORT
    hal_mipirx_lvds_ctrl_clean_err_cnt();
#endif
    return TD_SUCCESS;
}

static td_s32 drv_mipirx_debug_help(td_u32 argc, td_char (*argv)[PROC_CMD_SINGEL_LENGTH_MAX], td_void *private)
{
    ot_unused(argc);
    ot_unused(argv);
    ot_unused(private);
    OT_PRINT("=================================================================================="
             "======================================\n");
    OT_PRINT("echo command      para1 para2 para3  path                   explaination"
             "                   para descrption\n");
    OT_PRINT(MIPIRX_ECHO_0_PARAM_FMT, "cleanerr", "clean err cnt");
    OT_PRINT(MIPIRX_ECHO_3_PARAM_FMT, "openlane", 0, 0, 0, "open lane", "p0(port id) p1(lane index) p2(lane value)");
    OT_PRINT(MIPIRX_ECHO_2_PARAM_FMT, "closelane", 0, 0, "close lane", "p0(port id) p1(lane index)");
    OT_PRINT(MIPIRX_ECHO_3_PARAM_FMT, "colorbar", 0, 1, 1, "color bar setting",
        "p0(0:disable, 1:enable) p1(1:stripe, 0:fade) p2(0:lvds 1:mipi)");
    OT_PRINT(MIPIRX_ECHO_3_PARAM_FMT, "cropxy", 0, 1, 1, "set crop coordinate",
        "p0(port id) p1(x-coordinate x) p2(y-coordinate)");
    OT_PRINT(MIPIRX_ECHO_3_PARAM_FMT, "cropwh", 0, 1, 1, "set crop size",
        "p0(port id) p1(width) p2(height)");
    OT_PRINT(MIPIRX_ECHO_2_PARAM_FMT, "seteq", 0, 0x8, "set eq",
        "p0(port id) eq(0-0xF)");
    return TD_SUCCESS;
}

static osal_proc_cmd mipirx_debug_cmd_list[] = {
    { "help",       drv_mipirx_debug_help },
    { "cleanerr",   drv_mipirx_debug_clean_err_cnt },
    { "openlane",   drv_mipirx_debug_open_lane },
    { "closelane",  drv_mipirx_debug_close_lane },
    { "colorbar",   drv_mipirx_debug_color_bar },
    { "cropxy",     drv_mipirx_debug_crop_xy },
    { "cropwh",     drv_mipirx_debug_crop_wh },
    { "seteq",      drv_mipirx_debug_set_eq },
};
#endif

td_s32 drv_mipirx_add_proc(mipirx_port_ctx *mipi_ctx)
{
    osal_proc_entry *mipi_rx_proc_entry = TD_NULL;

    mipi_rx_proc_entry = osal_create_proc_entry("mipi_rx", TD_NULL);
    if (mipi_rx_proc_entry == TD_NULL) {
        mipirx_log_err("mipirx add mipi_rx proc entry error!\n");
        return TD_FAILURE;
    }

    mipi_rx_proc_entry->read = drv_mipirx_proc_read;
#ifdef CONFIG_OT_MIPIRX_DEBUG_CMD_SUPPORT
    mipi_rx_proc_entry->cmd_cnt = sizeof(mipirx_debug_cmd_list) / sizeof(osal_proc_cmd);
    mipi_rx_proc_entry->cmd_list = mipirx_debug_cmd_list;
#else
    mipi_rx_proc_entry->cmd_cnt = 0;
    mipi_rx_proc_entry->cmd_list = TD_NULL;
#endif
    mipi_rx_proc_entry->write = TD_NULL;
    mipi_rx_proc_entry->private_data = (void *) mipi_ctx;
    return TD_SUCCESS;
}

td_void drv_mipirx_del_proc(td_void)
{
    osal_remove_proc_entry("mipi_rx", TD_NULL);
}
