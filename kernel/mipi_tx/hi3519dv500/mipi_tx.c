/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "mipi_tx.h"
#include "mipi_tx_hal.h"
#include "mipi_tx_reg.h"
#include "autoconf.h"
#include "ot_mipi_tx_mod_init.h"
#include "ot_osal.h"
#include "type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define MIPI_TX_DEV_NAME "ot_mipi_tx"
#define MIPI_TX_PROC_NAME "mipi_tx"

#define OT_MEDIA_DYNAMIC_MINOR 255

#define mipi_tx_down_sem_return()                              \
    do {                                                       \
        if (osal_sem_down_interruptible(&g_mipi_tx_dev_sem)) { \
            return (-1);                                       \
        }                                                      \
    } while (0)

#define mipi_tx_up_sem() osal_sem_up(&g_mipi_tx_dev_sem)

#if mipi_tx_desc("pub")

static int g_en_dev = FALSE;
static int g_en_dev_cfg = FALSE;

static osal_dev *g_mipi_tx_dev;
static osal_semaphore g_mipi_tx_dev_sem;

static mipi_tx_dev_ctx_t g_mipi_tx_dev_ctx;

void mipi_tx_set_irq_num(unsigned int irq_num)
{
    mipi_tx_drv_set_irq_num(irq_num);
}

void mipi_tx_set_regs(const void *regs)
{
    mipi_tx_drv_set_regs((const mipi_tx_regs_type_t *)regs);
}

void mipi_tx_set_work_mode(mipi_tx_work_mode_t work_mode)
{
    g_mipi_tx_dev_ctx.work_param.work_mode = work_mode;
}

mipi_tx_work_mode_t mipi_tx_get_work_mode(void)
{
    return g_mipi_tx_dev_ctx.work_param.work_mode;
}

void mipi_tx_set_lp_clk_en(unsigned char lp_clk_en)
{
    g_mipi_tx_dev_ctx.work_param.lp_clk_en = lp_clk_en;
}

unsigned char mipi_tx_get_lp_clk_en(void)
{
    return g_mipi_tx_dev_ctx.work_param.lp_clk_en;
}

int mipi_tx_get_lane_num(const short lane_id[], int lane_id_len)
{
    int lane_num = 0;
    int i;

    for (i = 0; i < lane_id_len; i++) {
        if (lane_id[i] != MIPI_TX_DISABLE_LANE_ID) {
            lane_num++;
        }
    }
    return lane_num;
}

static int mipi_tx_check_temp_lane_id(int lane_index, int valid_lane_num, const short lane_id[], int lane_id_len)
{
    int j;
    int temp_id;
    temp_id = lane_id[lane_index];

    if ((temp_id < MIPI_TX_DISABLE_LANE_ID) || (temp_id >= LANE_MAX_NUM)) {
        mipi_tx_err("lane_id[%d] is invalid value %d.\n", lane_index, temp_id);
        return -1;
    }

    if (temp_id == MIPI_TX_DISABLE_LANE_ID) {
        return 0;
    }

    /*
     * lanex_id and lane num:
     * 1 lane: lane0_id, lane1/2/3_id must be different from lane0_id
     * 2 lane: lane0/1_id, lane2/3_id must be different from lane0/1_id
     * 3 lane: lane0/1/2_id, lane3_id must be different from lane0/1/2_id
     */
    if (temp_id > valid_lane_num) {
        mipi_tx_err("lane_id[%d]=%d is illegal, lane%d_id can't be set in %d lane mode\n", lane_index, temp_id,
            temp_id, valid_lane_num);
        return -1;
    }

    /* valid lane id must be different. */
    for (j = lane_index + 1; j < lane_id_len; j++) {
        if (temp_id == lane_id[j]) {
            mipi_tx_err("lane_id[%d] can't be same value %d as lane_id[%d]\n", lane_index, temp_id, j);
            return -1;
        }
    }

    return 0;
}

static int mipi_tx_check_dev_cfg_lane_id(const combo_dev_cfg_t *dev_cfg)
{
    int i;
    const int max_lane_num = LANE_MAX_NUM;
    int lane_num;

    lane_num = mipi_tx_get_lane_num(dev_cfg->lane_id, LANE_MAX_NUM);
    if (lane_num == 0) {
        mipi_tx_err("all lane_id is invalid!\n");
        return -1;
    }

    for (i = 0; i < max_lane_num; i++) {
        if (mipi_tx_check_temp_lane_id(i, lane_num, dev_cfg->lane_id, LANE_MAX_NUM) != 0) {
            return -1;
        }
    }

    return 0;
}

static int mipi_tx_check_dev_cfg_out_mode(const combo_dev_cfg_t *dev_cfg)
{
    if (dev_cfg->out_mode >= OUT_MODE_BUTT) {
        mipi_tx_err("mipi_tx dev output_mode(%d) err!\n", dev_cfg->out_mode);
        return -1;
    }
    return 0;
}

static int mipi_tx_check_dev_cfg_video_mode(const combo_dev_cfg_t *dev_cfg)
{
    if (dev_cfg->video_mode >= VIDEO_DATA_MODE_BUTT) {
        mipi_tx_err("mipi_tx dev video_mode(%d) err!\n", dev_cfg->video_mode);
        return -1;
    }
    return 0;
}

static int mipi_tx_check_dev_cfg_out_format(const combo_dev_cfg_t *dev_cfg)
{
    if (dev_cfg->out_format >= OUT_FORMAT_BUTT) {
        mipi_tx_err("mipi_tx dev format(%d) err!\n", dev_cfg->out_format);
        return -1;
    }
    return 0;
}

static int mipi_tx_check_dev_vertical_sync_info(const sync_info_t *sync_info)
{
    if ((sync_info->vact < MIPI_TX_MIN_SYNC_VACT) || (sync_info->vact > MIPI_TX_MAX_SYNC_VACT)) {
        mipi_tx_err("mipi tx sync's vact(%u) should be [%u, %u]!\n", sync_info->vact, MIPI_TX_MIN_SYNC_VACT,
            MIPI_TX_MAX_SYNC_VACT);
        return -1;
    }

    if ((sync_info->vbp < MIPI_TX_MIN_SYNC_VBP) || (sync_info->vbp > MIPI_TX_MAX_SYNC_VBP)) {
        mipi_tx_err("mipi tx sync's vbb(%u) should be [%u, %u]!\n", sync_info->vbp, MIPI_TX_MIN_SYNC_VBP,
            MIPI_TX_MAX_SYNC_VBP);
        return -1;
    }

    if ((sync_info->vfp < MIPI_TX_MIN_SYNC_VFP) || (sync_info->vfp > MIPI_TX_MAX_SYNC_VFP)) {
        mipi_tx_err("mipi tx sync's vfb(%u) should be [%u, %u]!\n",  sync_info->vfp, MIPI_TX_MIN_SYNC_VFP,
            MIPI_TX_MAX_SYNC_VFP);
        return -1;
    }

    return 0;
}

static int mipi_tx_check_dev_horizontal_sync_info(const sync_info_t *sync_info)
{
    if ((sync_info->hact < MIPI_TX_MIN_SYNC_HACT) || (sync_info->hact > MIPI_TX_MAX_SYNC_HACT)) {
        mipi_tx_err("mipi tx sync's hact(%u) should be [%u, %u]!\n", sync_info->hact, MIPI_TX_MIN_SYNC_HACT,
            MIPI_TX_MAX_SYNC_HACT);
        return -1;
    }

    if (sync_info->hbp < MIPI_TX_MIN_SYNC_HBP) {
        mipi_tx_err("mipi tx sync's hbb(%u) should be [%u, %u]!\n", sync_info->hbp, MIPI_TX_MIN_SYNC_HBP,
            MIPI_TX_MAX_SYNC_HBP);
        return -1;
    }

    if (sync_info->hfp < MIPI_TX_MIN_SYNC_HFP) {
        mipi_tx_err("mipi tx sync's hfb(%u) should be [%u, %u]!\n", sync_info->hfp, MIPI_TX_MIN_SYNC_HFP,
            MIPI_TX_MAX_SYNC_HFP);
        return -1;
    }

    return 0;
}

static int mipi_tx_check_dev_pulse_sync_info(const sync_info_t *sync_info)
{
    if (sync_info->hpw < MIPI_TX_MIN_SYNC_HPW) {
        mipi_tx_err("mipi tx sync's hpw(%u) should be [%u, %u]!\n", sync_info->hpw, MIPI_TX_MIN_SYNC_HPW,
            MIPI_TX_MAX_SYNC_HPW);
        return -1;
    }

    if ((sync_info->vpw < MIPI_TX_MIN_SYNC_VPW) || (sync_info->vpw > MIPI_TX_MAX_SYNC_VPW)) {
        mipi_tx_err("mipi tx sync's vpw(%u) should be [%u, %u]!\n", sync_info->vpw, MIPI_TX_MIN_SYNC_VPW,
            MIPI_TX_MAX_SYNC_VPW);
        return -1;
    }

    return 0;
}

static int mipi_tx_check_dev_cfg_sync_info(const combo_dev_cfg_t *dev_cfg)
{
    int ret;

    ret = mipi_tx_check_dev_vertical_sync_info(&dev_cfg->sync_info);
    if (ret != 0) {
        return ret;
    }

    ret = mipi_tx_check_dev_horizontal_sync_info(&dev_cfg->sync_info);
    if (ret != 0) {
        return ret;
    }

    return mipi_tx_check_dev_pulse_sync_info(&dev_cfg->sync_info);
}

static int mipi_tx_check_dev_cfg_phy_data_rate(const combo_dev_cfg_t *dev_cfg)
{
    if ((dev_cfg->phy_data_rate < MIPI_TX_MIN_PHY_DATA_RATE) ||
        (dev_cfg->phy_data_rate > MIPI_TX_MAX_PHY_DATA_RATE)) {
        mipi_tx_err("mipi_tx dev phy data rate(%u) should be in [%d, %d]\n", dev_cfg->phy_data_rate,
            MIPI_TX_MIN_PHY_DATA_RATE, MIPI_TX_MAX_PHY_DATA_RATE);
        return -1;
    }
    return 0;
}

static int mipi_tx_check_dev_cfg_pixel_clk(const combo_dev_cfg_t *dev_cfg)
{
    if ((dev_cfg->pixel_clk < MIPI_TX_MIN_PIXEL_CLK) ||
        (dev_cfg->pixel_clk > MIPI_TX_MAX_PIXEL_CLK)) {
        mipi_tx_err("mipi_tx dev pixel clk(%u) should be in [%d, %d]\n", dev_cfg->pixel_clk,
            MIPI_TX_MIN_PIXEL_CLK, MIPI_TX_MAX_PIXEL_CLK);
        return -1;
    }
    return 0;
}

static int mipi_tx_is_format_for_dsi_mode(out_format_t out_format)
{
    if ((out_format >= OUT_FORMAT_RGB_16BIT) && (out_format <= OUT_FORMAT_YUV422_16BIT)) {
        return 1;
    }
    return 0;
}

static int mipi_tx_is_format_for_csi_mode(out_format_t out_format)
{
    if ((out_format >= OUT_FORMAT_YUV420_8BIT_NORMAL) && (out_format <= OUT_FORMAT_RAW_16BIT)) {
        return 1;
    }
    return 0;
}

static int mipi_tx_is_dsi_mode(out_mode_t out_mode)
{
    if ((out_mode == OUT_MODE_DSI_VIDEO) || (out_mode == OUT_MODE_DSI_CMD)) {
        return 1;
    }
    return 0;
}

static int mipi_tx_is_csi_mode(out_mode_t out_mode)
{
    if (out_mode == OUT_MODE_CSI) {
        return 1;
    }
    return 0;
}

static int mipi_tx_check_dev_out_mode_and_out_format(const combo_dev_cfg_t *dev_cfg)
{
    if ((mipi_tx_is_dsi_mode(dev_cfg->out_mode) == 1) &&
        (mipi_tx_is_format_for_dsi_mode(dev_cfg->out_format) != 1)) {
        mipi_tx_err("mipi_tx dev out mode(%d) does not match the out data format(%d)\n",
            dev_cfg->out_mode, dev_cfg->out_format);
        return -1;
    }

    if ((mipi_tx_is_csi_mode(dev_cfg->out_mode) == 1) &&
        (mipi_tx_is_format_for_csi_mode(dev_cfg->out_format) != 1)) {
        mipi_tx_err("mipi_tx dev out mode(%d) does not match the out data format(%d)\n",
            dev_cfg->out_mode, dev_cfg->out_format);
        return -1;
    }
    return 0;
}

static int mipi_tx_check_dev_clklane_continue_mode(const combo_dev_cfg_t *dev_cfg)
{
    if ((dev_cfg->clklane_continue_mode < MIPI_TX_CLK_LANE_CONTINUE) ||
        (dev_cfg->clklane_continue_mode >= MIPI_TX_CLK_LANE_BUTT)) {
        mipi_tx_err("mipi_tx dev continue mode (%u) should be in [%d, %d]\n", dev_cfg->clklane_continue_mode,
            MIPI_TX_CLK_LANE_CONTINUE, MIPI_TX_CLK_LANE_NON_CONTINUE);
        return -1;
    }

    return 0;
}

static int mipi_tx_check_comb_dev_cfg(const combo_dev_cfg_t *dev_cfg)
{
    int ret;
    if (g_en_dev == TRUE) {
        mipi_tx_err("mipi_tx dev has enable!\n");
        return -1;
    }

    if (dev_cfg->devno != 0) {
        mipi_tx_err("mipi_tx dev devno err!\n");
        return -1;
    }

    ret = mipi_tx_check_dev_cfg_lane_id(dev_cfg);
    if (ret != 0) {
        return ret;
    }

    ret = mipi_tx_check_dev_cfg_out_mode(dev_cfg);
    if (ret != 0) {
        return ret;
    }

    ret = mipi_tx_check_dev_cfg_video_mode(dev_cfg);
    if (ret != 0) {
        return ret;
    }

    ret = mipi_tx_check_dev_cfg_out_format(dev_cfg);
    if (ret != 0) {
        return ret;
    }

    ret = mipi_tx_check_dev_cfg_sync_info(dev_cfg);
    if (ret != 0) {
        return ret;
    }

    ret = mipi_tx_check_dev_cfg_phy_data_rate(dev_cfg);
    if (ret != 0) {
        return ret;
    }

    ret = mipi_tx_check_dev_cfg_pixel_clk(dev_cfg);
    if (ret != 0) {
        return ret;
    }

    ret = mipi_tx_check_dev_out_mode_and_out_format(dev_cfg);
    if (ret != 0) {
        return ret;
    }
    return mipi_tx_check_dev_clklane_continue_mode(dev_cfg);
}

static int mipi_tx_set_combo_dev_cfg(const combo_dev_cfg_t *dev_cfg)
{
    int ret;

    mipi_tx_check_null_ptr_return(dev_cfg);
    mipi_tx_down_sem_return();

    ret = mipi_tx_check_comb_dev_cfg(dev_cfg);
    if (ret < 0) {
        mipi_tx_up_sem();
        mipi_tx_err("mipi_tx check combo_dev config failed!\n");
        return ret;
    }

    /* set controller config */
    mipi_tx_drv_set_controller_cfg(dev_cfg);

    /* set phy config */
    mipi_tx_drv_set_phy_cfg();

    mipi_tx_set_work_mode(MIPI_TX_WORK_MODE_LP);
    mipi_tx_set_lp_clk_en(FALSE);

    (void)memcpy_s(&g_mipi_tx_dev_ctx.dev_cfg, sizeof(combo_dev_cfg_t), dev_cfg, sizeof(combo_dev_cfg_t));
    g_en_dev_cfg = TRUE;

    mipi_tx_up_sem();

    return ret;
}

#endif /* #if mipi_tx_desc("pub") */

#if mipi_tx_desc("set & get cmd")

static int mipi_tx_check_pub_info(const mipi_tx_pub_info *pub_info)
{
    if (g_en_dev_cfg != TRUE) {
        mipi_tx_err("mipi_tx dev has not configured!\n");
        return -1;
    }

    if (pub_info->devno != 0) {
        mipi_tx_err("mipi_tx devno(%u) illegal!\n", pub_info->devno);
        return -1;
    }

    if ((pub_info->work_param.work_mode != MIPI_TX_WORK_MODE_LP) &&
        (pub_info->work_param.work_mode != MIPI_TX_WORK_MODE_HS)) {
        mipi_tx_err("mipi_tx work_mode(%d) is illegal!\n", pub_info->work_param.work_mode);
        return -1;
    }

    if ((pub_info->work_param.lp_clk_en != 0) && (pub_info->work_param.lp_clk_en != 1)) {
        mipi_tx_err("mipi_tx lp_clk_en(%d) is illegal!\n", pub_info->work_param.lp_clk_en);
        return -1;
    }

    /* lp_clk_en must be 1 when work mode is hs mode */
    if ((pub_info->work_param.work_mode == MIPI_TX_WORK_MODE_HS) && (pub_info->work_param.lp_clk_en != 1)) {
        mipi_tx_err("mipi_tx lp_clk_en(%d) is not support in work_mode(%d)!\n",
            pub_info->work_param.lp_clk_en, pub_info->work_param.work_mode);
        return -1;
    }

    return 0;
}

static int mipi_tx_check_set_cmd_info(const cmd_info_t *cmd_info)
{
    mipi_tx_pub_info pub_info = { 0 };
    int ret;

    pub_info.devno = cmd_info->devno;
    pub_info.work_param.work_mode = cmd_info->work_mode;
    pub_info.work_param.lp_clk_en = cmd_info->lp_clk_en;

    ret = mipi_tx_check_pub_info(&pub_info);
    if (ret != 0) {
        return ret;
    }

    /* When cmd is not NULL,cmd_size means the length of cmd or it means cmd and addr */
    if (cmd_info->cmd != NULL) {
        if ((cmd_info->cmd_size > MIPI_TX_SET_DATA_SIZE) || (cmd_info->cmd_size == 0)) {
            mipi_tx_err("mipi_tx dev cmd_size(%d) is illegal, it should be in (%d, %d].\n", cmd_info->cmd_size,
                0, MIPI_TX_SET_DATA_SIZE);
            return -1;
        }
    }

    return 0;
}

static int mipi_tx_set_cmd(const cmd_info_t *cmd_info)
{
    int ret;

    mipi_tx_check_null_ptr_return(cmd_info);
    mipi_tx_down_sem_return();

    ret = mipi_tx_check_set_cmd_info(cmd_info);
    if (ret < 0) {
        mipi_tx_up_sem();
        mipi_tx_err("mipi_tx check set cmd info failed!\n");
        return ret;
    }
    ret = mipi_tx_drv_set_cmd_info(cmd_info);
    mipi_tx_up_sem();
    return ret;
}

static int mipi_tx_check_get_cmd_info(const get_cmd_info_t *get_cmd_info)
{
    mipi_tx_pub_info pub_info = { 0 };
    int ret;

    pub_info.devno = get_cmd_info->devno;
    pub_info.work_param.work_mode = get_cmd_info->work_mode;
    pub_info.work_param.lp_clk_en = get_cmd_info->lp_clk_en;

    ret = mipi_tx_check_pub_info(&pub_info);
    if (ret != 0) {
        return ret;
    }

    if ((get_cmd_info->get_data_size == 0) || (get_cmd_info->get_data_size > MIPI_TX_GET_DATA_SIZE)) {
        mipi_tx_err("mipi_tx dev get_data_size(%d) illegal, it should be in (%d, %d].\n",
            get_cmd_info->get_data_size, 0, MIPI_TX_GET_DATA_SIZE);
        return -1;
    }

    if (get_cmd_info->get_data == NULL) {
        mipi_tx_err("mipi_tx dev get_data is null!\n");
        return -1;
    }

    return 0;
}

static int mipi_tx_get_cmd(const get_cmd_info_t *get_cmd_info)
{
    int ret;

    mipi_tx_check_null_ptr_return(get_cmd_info);
    mipi_tx_down_sem_return();
    ret = mipi_tx_check_get_cmd_info(get_cmd_info);
    if (ret < 0) {
        mipi_tx_up_sem();
        mipi_tx_err("mipi_tx check get cmd info failed!\n");
        return ret;
    }
    ret = mipi_tx_drv_get_cmd_info(get_cmd_info);
    mipi_tx_up_sem();

    return ret;
}

#endif /* #if mipi_tx_desc("set & get cmd") */

#ifndef OT_FPGA
#if mipi_tx_desc("pn swap")

static int mipi_tx_check_set_pn_swap(const combo_dev_pn_swap_t *tx_pn_swap)
{
    int i;

    if (g_en_dev_cfg != TRUE) {
        mipi_tx_err("mipi_tx dev has not configured!\n");
        return -1;
    }

    if (g_en_dev == TRUE) {
        mipi_tx_err("mipi_tx dev has enable!\n");
        return -1;
    }

    if (tx_pn_swap->devno != 0) {
        mipi_tx_err("mipi_tx devno(%u) illegal!\n", tx_pn_swap->devno);
        return -1;
    }

    for (i = 0; i < MIPI_LANE_NUM_PER_DEV; i++) {
        if ((tx_pn_swap->pn_swap[i] != TRUE) && (tx_pn_swap->pn_swap[i] != FALSE)) {
            mipi_tx_err("pn_swap (%u) is illegal!\n", tx_pn_swap->pn_swap[i]);
            return -1;
        }
    }

    return 0;
}

static int mipi_tx_set_pn_swap(const combo_dev_pn_swap_t *pn_swap)
{
    int ret;

    mipi_tx_check_null_ptr_return(pn_swap);
    mipi_tx_down_sem_return();

    ret = mipi_tx_check_set_pn_swap(pn_swap);
    if (ret < 0) {
        mipi_tx_up_sem();
        mipi_tx_err("mipi_tx check pn swap failed!\n");
        return ret;
    }
    mipi_tx_drv_set_pn_swap(pn_swap);
    mipi_tx_up_sem();

    return ret;
}

#endif /* #if mipi_tx_desc("pn swap") */
#endif

#if mipi_tx_desc("enable & disable")

static int mipi_tx_enable(void)
{
    out_mode_t output_mode;

    mipi_tx_down_sem_return();
    if (g_en_dev_cfg == FALSE) {
        mipi_tx_up_sem();
        return -1;
    }

    output_mode = g_mipi_tx_dev_ctx.dev_cfg.out_mode;
    mipi_tx_drv_enable_input(output_mode);
    if (output_mode == OUT_MODE_DSI_CMD) {
        mipi_tx_set_work_mode(MIPI_TX_WORK_MODE_HS);
    }
    mipi_tx_set_lp_clk_en(TRUE);
    g_en_dev = TRUE;
    mipi_tx_up_sem();

    return 0;
}

static int mipi_tx_disable(void)
{
    mipi_tx_down_sem_return();
    mipi_tx_drv_disable_input();

    mipi_tx_drv_reset_pn_swap();
    mipi_tx_set_work_mode(MIPI_TX_WORK_MODE_LP);
    mipi_tx_set_lp_clk_en(FALSE);
    mipi_tx_drv_set_ctrl_clk(0);

    g_en_dev = FALSE;
    g_en_dev_cfg = FALSE;
    mipi_tx_up_sem();

    return 0;
}

#endif /* #if mipi_tx_desc("enable & disable") */

#if mipi_tx_desc("proc")

#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
static int mipi_tx_get_sync_same(sync_info_t *psync_info, mipi_tx_dev_phy_t mipi_tx_phy_ctx)
{
    if ((psync_info->hact != mipi_tx_phy_ctx.hact_det) || (psync_info->hpw != mipi_tx_phy_ctx.hsa_det) ||
        (psync_info->hbp != mipi_tx_phy_ctx.hbp_det) || (psync_info->vpw != mipi_tx_phy_ctx.vsa_det) ||
        (psync_info->vact != mipi_tx_phy_ctx.vact_det)) {
        return 0;
    }

    return 1;
}

static void mipi_tx_proc_module_show(osal_proc_entry *s)
{
    int smooth = mipi_tx_get_smooth();
    osal_seq_printf(s->seqfile, "----------mipi_tx module config------------------------\n");
    osal_seq_printf(s->seqfile, "%8s\n", "smooth");
    osal_seq_printf(s->seqfile, "%8d\n", smooth);
    osal_seq_printf(s->seqfile, "\r\n");
}

static char* mipi_tx_proc_continue(continue_mode_t clklane_continue_mode)
{
    if (clklane_continue_mode == MIPI_TX_CLK_LANE_CONTINUE) {
        return "CONTINUE";
    } else {
        return "NON-CONTINUE";
    }
}

static void mipi_tx_proc_dev_show(osal_proc_entry *s)
{
    combo_dev_cfg_t *pdev_cfg = NULL;
    sync_info_t *psync_info = NULL;
    pdev_cfg = &g_mipi_tx_dev_ctx.dev_cfg;
    psync_info = &g_mipi_tx_dev_ctx.dev_cfg.sync_info;

    /* mipi tx device config */
    osal_seq_printf(s->seqfile, "----------mipi_tx dev config---------------------------\n");

    osal_seq_printf(s->seqfile, "%8s%8s%8s%8s%8s%15s%15s%15s%15s%15s%26s\n",
        "devno", "lane0", "lane1", "lane2", "lane3", "output_mode", "phy_data_rate", "pixel_clk(KHz)",
        "video_mode", "output_fmt", "clklane_continue_mode");

    if (g_en_dev == TRUE) {
        osal_seq_printf(s->seqfile, "%8d%8d%8d%8d%8d%15d%15d%15d%15d%15d%26s\n",
            pdev_cfg->devno,
            pdev_cfg->lane_id[0],
            pdev_cfg->lane_id[1],
            pdev_cfg->lane_id[2], /* lane id 2 */
            pdev_cfg->lane_id[3], /* lane id 3 */
            pdev_cfg->out_mode,
            pdev_cfg->phy_data_rate,
            pdev_cfg->pixel_clk,
            pdev_cfg->video_mode,
            pdev_cfg->out_format,
            mipi_tx_proc_continue(pdev_cfg->clklane_continue_mode));
    }
    osal_seq_printf(s->seqfile, "\r\n");

    /* mipi tx device sync config */
    osal_seq_printf(s->seqfile, "----------mipi_tx sync config---------------------------\n");

    osal_seq_printf(s->seqfile, "%14s%14s%14s%14s%14s%14s%14s%14s\n",
        "hact", "hbp", "hfp", "hsa", "vact", "vbp", "vfp", "vsa");

    if (g_en_dev == TRUE) {
        osal_seq_printf(s->seqfile, "%14d%14d%14d%14d%14d%14d%14d%14d\n",
            psync_info->hact,
            psync_info->hbp,
            psync_info->hfp,
            psync_info->hpw,
            psync_info->vact,
            psync_info->vbp,
            psync_info->vfp,
            psync_info->vpw);
    }
    osal_seq_printf(s->seqfile, "\r\n");
}

static int mipi_tx_proc_dev_lane_pn_status_show(osal_proc_entry *s)
{
    combo_dev_pn_swap_t pn_status;

    /* mipi tx device pn_swap config */
    osal_seq_printf(s->seqfile, "----------mipi_tx pn_swap config------------------------\n");
    osal_seq_printf(s->seqfile, "%14s%14s%14s%14s\n", "lane0_pn", "lane1_pn", "lane2_pn", "lane3_pn");
    mipi_tx_drv_get_pn_swap(&pn_status);
    if (g_en_dev == TRUE) {
        osal_seq_printf(s->seqfile, "%14d%14d%14d%14d\n",
            pn_status.pn_swap[0], /* lane id 0 */
            pn_status.pn_swap[1], /* lane id 1 */
            pn_status.pn_swap[2], /* lane id 2 */
            pn_status.pn_swap[3]); /* lane id 3 */
    }

    osal_seq_printf(s->seqfile, "\r\n");
    return 0;
}

static void mipi_tx_proc_dev_status_show(osal_proc_entry *s)
{
    int check_sync_same;
    mipi_tx_dev_phy_t mipi_tx_phy_ctx;
    sync_info_t *psync_info = &g_mipi_tx_dev_ctx.dev_cfg.sync_info;

    mipi_tx_drv_get_dev_status(&mipi_tx_phy_ctx);

    /* mipi tx phy status */
    osal_seq_printf(s->seqfile, "----------mipi_tx dev status---------------------------\n");
    osal_seq_printf(s->seqfile, "%8s%8s%8s%8s%8s%8s%8s%12s\n",
        "width", "height", "horiall", "vertall", "hbp", "hsa", "vsa", "sync_check");

    if (g_en_dev == TRUE) {
        check_sync_same = mipi_tx_get_sync_same(psync_info, mipi_tx_phy_ctx);
        osal_seq_printf(s->seqfile, "%8u%8u%8u%8u%8u%8u%8u%12u\n",
            mipi_tx_phy_ctx.hact_det,
            mipi_tx_phy_ctx.vact_det,
            mipi_tx_phy_ctx.hall_det,
            mipi_tx_phy_ctx.vall_det,
            mipi_tx_phy_ctx.hbp_det,
            mipi_tx_phy_ctx.hsa_det,
            mipi_tx_phy_ctx.vsa_det,
            check_sync_same);
    }
    osal_seq_printf(s->seqfile, "\r\n");
}

static int mipi_tx_proc_show(osal_proc_entry *s)
{
    osal_seq_printf(s->seqfile, "\nModule: [MIPI_TX], Build Time["__DATE__", "__TIME__"]\n");
    mipi_tx_proc_module_show(s);
    mipi_tx_proc_dev_show(s);
    mipi_tx_proc_dev_lane_pn_status_show(s);
    mipi_tx_proc_dev_status_show(s);
    return 0;
}
#endif
#endif /* #if mipi_tx_desc("proc") */

#if mipi_tx_desc("init & ioctl")
static int ot_mipi_tx_set_dev_cfg_job(unsigned int cmd, void *arg, void *private_data)
{
    mipi_tx_unused(cmd);
    mipi_tx_unused(private_data);
    return mipi_tx_set_combo_dev_cfg((const combo_dev_cfg_t *)(uintptr_t)arg);
}

static int ot_mipi_tx_set_cmd_job(unsigned int cmd, void *arg, void *private_data)
{
    mipi_tx_unused(cmd);
    mipi_tx_unused(private_data);
    return mipi_tx_set_cmd((const cmd_info_t *)(uintptr_t)arg);
}

static int ot_mipi_tx_get_cmd_job(unsigned int cmd, void *arg, void *private_data)
{
    mipi_tx_unused(cmd);
    mipi_tx_unused(private_data);
    return mipi_tx_get_cmd((const get_cmd_info_t *)(uintptr_t)arg);
}

static int ot_mipi_tx_enable_job(unsigned int cmd, void *arg, void *private_data)
{
    mipi_tx_unused(cmd);
    mipi_tx_unused(arg);
    mipi_tx_unused(private_data);
    return mipi_tx_enable();
}

static int ot_mipi_tx_disable_job(unsigned int cmd, void *arg, void *private_data)
{
    mipi_tx_unused(cmd);
    mipi_tx_unused(arg);
    mipi_tx_unused(private_data);
    return mipi_tx_disable();
}

#ifndef OT_FPGA
static int ot_mipi_tx_set_pn_swap_job(unsigned int cmd, void *arg, void *private_data)
{
    mipi_tx_unused(cmd);
    mipi_tx_unused(private_data);
    return mipi_tx_set_pn_swap((const combo_dev_pn_swap_t *)(uintptr_t)arg);
}

#else
static int ot_mipi_tx_set_pn_swap_job(unsigned int cmd, void *arg, void *private_data)
{
    mipi_tx_unused(cmd);
    mipi_tx_unused(arg);
    mipi_tx_unused(private_data);
    mipi_tx_err("mipi_tx not support pn swap!\n");
    return -1;
}

#endif

static osal_ioctl_cmd g_mipi_tx_ioctl_cmd_list[] = {
    { OT_MIPI_TX_SET_DEV_CFG, ot_mipi_tx_set_dev_cfg_job },
    { OT_MIPI_TX_SET_CMD,     ot_mipi_tx_set_cmd_job },
    { OT_MIPI_TX_GET_CMD,     ot_mipi_tx_get_cmd_job },
    { OT_MIPI_TX_ENABLE,      ot_mipi_tx_enable_job },
    { OT_MIPI_TX_DISABLE,     ot_mipi_tx_disable_job },
    { OT_MIPI_TX_SET_PN_SWAP, ot_mipi_tx_set_pn_swap_job },
};

void mipi_tx_set_ioctl_cmd_list(osal_fileops *mipi_tx_fops)
{
    mipi_tx_fops->cmd_list = g_mipi_tx_ioctl_cmd_list;
    mipi_tx_fops->cmd_cnt = sizeof(g_mipi_tx_ioctl_cmd_list) / sizeof(g_mipi_tx_ioctl_cmd_list[0]);
}

static int mipi_tx_init(int smooth)
{
    return mipi_tx_drv_init(smooth);
}

static void mipi_tx_exit(void)
{
    mipi_tx_drv_exit();
}

static int mipi_tx_open(void *private_data)
{
    mipi_tx_unused(private_data);
    return 0;
}

static int mipi_tx_release(void *private_data)
{
    mipi_tx_unused(private_data);
    return 0;
}

static osal_fileops g_mipi_tx_fops = {
    .open = mipi_tx_open,
    .release = mipi_tx_release,
};

static int mipi_tx_register_device(void)
{
    int ret;
    g_mipi_tx_dev = osal_dev_create(MIPI_TX_DEV_NAME);
    if (g_mipi_tx_dev == NULL) {
        mipi_tx_err("create mipi_tx device failed!\n");
        return -1;
    }

    mipi_tx_set_ioctl_cmd_list(&g_mipi_tx_fops);
    g_mipi_tx_dev->fops = &g_mipi_tx_fops;
    g_mipi_tx_dev->minor = OT_MEDIA_DYNAMIC_MINOR;

    ret = osal_dev_register(g_mipi_tx_dev);
    if (ret != 0) {
        osal_dev_destroy(g_mipi_tx_dev);
        g_mipi_tx_dev = NULL;
        mipi_tx_err("register mipi_tx device failed!\n");
        return -1;
    }
    return 0;
}

static void mipi_tx_unregister_device(void)
{
    osal_dev_unregister(g_mipi_tx_dev);
    osal_dev_destroy(g_mipi_tx_dev);
    g_mipi_tx_dev = NULL;
    return;
}

static int mipi_tx_check_smooth(int smooth)
{
    if ((smooth != TRUE) && (smooth != FALSE)) {
        mipi_tx_err("module param smooth(%d) is illegal!\n", smooth);
        return -1;
    }
    return 0;
}

int mipi_tx_module_init(int smooth)
{
    int ret;
#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
    osal_proc_entry *mipi_tx_proc_entry = NULL;
#endif

    ret = mipi_tx_check_smooth(smooth);
    if (ret != 0) {
        goto fail0;
    }

    ret = mipi_tx_register_device();
    if (ret != 0) {
        goto fail0;
    }

#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
    mipi_tx_proc_entry = osal_create_proc_entry(MIPI_TX_PROC_NAME, NULL);
    if (mipi_tx_proc_entry == NULL) {
        mipi_tx_err("create mipi_tx proc(%s) failed!\n", MIPI_TX_PROC_NAME);
        goto fail1;
    }

    mipi_tx_proc_entry->read = mipi_tx_proc_show;
    mipi_tx_proc_entry->write = NULL;
#endif

    ret = mipi_tx_init(smooth);
    if (ret != 0) {
        mipi_tx_err("mipi init failed!\n");
        goto fail2;
    }

    ret = osal_sem_init(&g_mipi_tx_dev_sem, 1);
    if (ret != 0) {
        mipi_tx_err("init sema error!\n");
        goto fail3;
    }

    osal_printk("load mipi_tx.ko ....OK!\n");
    return 0;

fail3:
    mipi_tx_exit();
fail2:
#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
    osal_remove_proc_entry(MIPI_TX_PROC_NAME, NULL);
fail1:
#endif
    mipi_tx_unregister_device();
fail0:
    mipi_tx_err("load mipi_tx.ko ....failed!\n");
    return -1;
}

void mipi_tx_module_exit(void)
{
    osal_sem_destroy(&g_mipi_tx_dev_sem);
    mipi_tx_exit();
#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
    osal_remove_proc_entry(MIPI_TX_PROC_NAME, NULL);
#endif
    mipi_tx_unregister_device();
    osal_printk("unload mipi_tx.ko ....OK!\n");
}

#endif /* #if mipi_tx_desc("init & ioctl") */

#ifdef __cplusplus
#if __cplusplus
}

#endif
#endif
