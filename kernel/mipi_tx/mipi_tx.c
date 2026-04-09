/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2016-2019. All rights reserved.
 * Description: mipi tx file.
 * Author: Hisilicon multimedia software group
 * Create: 2016/06/28
 */

#include "mipi_tx.h"
#include "mipi_tx_hal.h"
#include "mipi_tx_reg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define MIPI_TX_DEV_NAME           "hi_mipi_tx"
#define MIPI_TX_PROC_NAME          "mipi_tx"

#define HIMEDIA_DYNAMIC_MINOR      255

#define NULL    0L

#define mipi_tx_down_sem_return()\
do {\
    if (osal_down_interruptible(&g_mipi_tx_dev_sem)) {\
        return -1;\
    }\
}while(0)\

#define mipi_tx_up_sem() osal_up(&g_mipi_tx_dev_sem)

static int g_en_dev = FALSE;
static int g_en_dev_cfg = FALSE;

static osal_dev_t* g_mipi_tx_dev;
struct osal_semaphore g_mipi_tx_dev_sem;

mipi_tx_dev_ctx_t g_mipi_tx_dev_ctx;

static int mipi_tx_check_comb_dev_cfg(const combo_dev_cfg_t *dev_cfg)
{
    int i;
    int valid_lane_id[LANE_MAX_NUM] = {0, 1, 2, 3};
    if (g_en_dev == TRUE) {
        hi_mipi_tx_err("mipi_tx dev has enable!\n");
        return -1;
    }

    if (dev_cfg->devno != 0) {
        hi_mipi_tx_err("mipi_tx dev devno err!\n");
        return -1;
    }

    for (i = 0; i < LANE_MAX_NUM; i++) {
        if ((dev_cfg->lane_id[i] != valid_lane_id[i]) && (dev_cfg->lane_id[i] != MIPI_TX_DISABLE_LANE_ID)) {
            hi_mipi_tx_err("mipi_tx dev lane_id0(%d) err!\n", dev_cfg->lane_id[i]);
            return -1;
        }
    }

    if ((dev_cfg->output_mode != OUTPUT_MODE_CSI) && (dev_cfg->output_mode != OUTPUT_MODE_DSI_VIDEO) &&
        (dev_cfg->output_mode != OUTPUT_MODE_DSI_CMD)) {
        hi_mipi_tx_err("mipi_tx dev output_mode(%d) err!\n", dev_cfg->output_mode);
        return -1;
    }

    if ((dev_cfg->video_mode != BURST_MODE) && (dev_cfg->video_mode != NON_BURST_MODE_SYNC_PULSES) &&
        (dev_cfg->video_mode != NON_BURST_MODE_SYNC_EVENTS)) {
        hi_mipi_tx_err("mipi_tx dev video_mode(%d) err!\n", dev_cfg->video_mode);
        return -1;
    }

    if ((dev_cfg->output_format != OUT_FORMAT_RGB_16_BIT) && (dev_cfg->output_format != OUT_FORMAT_RGB_18_BIT) &&
        (dev_cfg->output_format != OUT_FORMAT_RGB_24_BIT) && (dev_cfg->output_format !=
        OUT_FORMAT_YUV420_8_BIT_NORMAL) && (dev_cfg->output_format != OUT_FORMAT_YUV420_8_BIT_LEGACY) &&
        (dev_cfg->output_format != OUT_FORMAT_YUV422_8_BIT)) {
        hi_mipi_tx_err("mipi_tx dev output_format(%d) err!\n", dev_cfg->output_format);
        return -1;
    }

    return 0;
}

static int mipi_tx_set_combo_dev_cfg(const combo_dev_cfg_t *dev_cfg)
{
    int ret;

    ret = mipi_tx_check_comb_dev_cfg(dev_cfg);
    if (ret < 0) {
        hi_mipi_tx_err("mipi_tx check combo_dev config failed!\n");
        return ret;
    }

    /* set controler config */
    mipi_tx_drv_set_controller_cfg(dev_cfg);

    /* set phy config */
    mipi_tx_drv_set_phy_cfg(dev_cfg);

    osal_memcpy(&g_mipi_tx_dev_ctx.dev_cfg, dev_cfg, sizeof(combo_dev_cfg_t));

    g_en_dev_cfg = TRUE;

    return ret;
}

static int mipi_tx_check_set_cmd_info(const cmd_info_t *cmd_info)
{
    if (g_en_dev == TRUE) {
        hi_mipi_tx_err("mipi_tx dev has enable!\n");
        return -1;
    }

    if (g_en_dev_cfg != TRUE) {
        hi_mipi_tx_err("mipi_tx dev has not config!\n");
        return -1;
    }

    if (cmd_info->devno != 0) {
        hi_mipi_tx_err("mipi_tx devno(%d)err!\n", cmd_info->devno);
        return -1;
    }

    /* When cmd is not NULL, cmd_size means the length of cmd or it means cmd and addr */
    if (cmd_info->cmd != NULL) {
        if (cmd_info->cmd_size > MIPI_TX_SET_DATA_SIZE) {
            hi_mipi_tx_err("mipi_tx dev cmd_size(%d) err!\n", cmd_info->cmd_size);
            return -1;
        }
    }

    return 0;
}

static int mipi_tx_set_cmd(const cmd_info_t *cmd_info)
{
    int ret;

    ret = mipi_tx_check_set_cmd_info(cmd_info);
    if (ret < 0) {
        hi_mipi_tx_err("mipi_tx check combo_dev config failed!\n");
        return ret;
    }

    return mipi_tx_drv_set_cmd_info(cmd_info);
}

static int mipi_tx_check_get_cmd_info(const get_cmd_info_t *get_cmd_info)
{
    if (g_en_dev == TRUE) {
        hi_mipi_tx_err("mipi_tx dev has enable!\n");
        return -1;
    }

    if (g_en_dev_cfg != TRUE) {
        hi_mipi_tx_err("mipi_tx dev has not config!\n");
        return -1;
    }

    if (get_cmd_info->devno != 0) {
        hi_mipi_tx_err("mipi_tx dev devno(%d) err!\n", get_cmd_info->devno);
        return -1;
    }

    if ((get_cmd_info->get_data_size == 0) || (get_cmd_info->get_data_size > MIPI_TX_GET_DATA_SIZE)) {
        hi_mipi_tx_err("mipi_tx dev get_data_size(%d) err!\n", get_cmd_info->get_data_size);
        return -1;
    }

    if (get_cmd_info->get_data == NULL) {
        hi_mipi_tx_err("mipi_tx dev get_data is null!\n");
        return -1;
    }
    return 0;
}

static int mipi_tx_get_cmd(get_cmd_info_t *get_cmd_info)
{
    int ret;

    ret = mipi_tx_check_get_cmd_info(get_cmd_info);
    if (ret < 0) {
        hi_mipi_tx_err("mipi_tx check combo_dev config failed!\n");
        return ret;
    }

    return mipi_tx_drv_get_cmd_info(get_cmd_info);
}

static void mipi_tx_enable(void)
{
    output_mode_t output_mode;

    output_mode = g_mipi_tx_dev_ctx.dev_cfg.output_mode;

    mipi_tx_drv_enable_input(output_mode);

    g_en_dev = TRUE;
}

static void mipi_tx_disable(void)
{
    mipi_tx_drv_disable_input();

    g_en_dev = FALSE;
    g_en_dev_cfg = FALSE;
}

static long mipi_tx_ioctl(unsigned int cmd, unsigned long arg, void* private_data)
{
    int ret = 0;

    switch (cmd) {
        case HI_MIPI_TX_SET_DEV_CFG: {
            combo_dev_cfg_t *pstcombo_dev_cfg = NULL;

            pstcombo_dev_cfg = (combo_dev_cfg_t *)arg;

            mipi_tx_check_null_ptr_return(pstcombo_dev_cfg);

            mipi_tx_down_sem_return();
            ret = mipi_tx_set_combo_dev_cfg(pstcombo_dev_cfg);
            if (ret < 0) {
                hi_mipi_tx_err("mipi_tx set combo_dev config failed!\n");
                ret = -1;
            }
            mipi_tx_up_sem();
            break;
        }

        case HI_MIPI_TX_SET_CMD: {
            cmd_info_t *cmd_info = NULL;

            cmd_info = (cmd_info_t *)arg;

            mipi_tx_check_null_ptr_return(cmd_info);
            mipi_tx_down_sem_return();
            ret = mipi_tx_set_cmd(cmd_info);
            if (ret < 0) {
                hi_mipi_tx_err("mipi_tx set cmd failed!\n");
                ret = -1;
            }
            mipi_tx_up_sem();
            break;
        }

        case HI_MIPI_TX_GET_CMD: {
            get_cmd_info_t *get_cmd_info = NULL;
            get_cmd_info = (get_cmd_info_t *)arg;
            mipi_tx_check_null_ptr_return(get_cmd_info);
            mipi_tx_down_sem_return();
            ret = mipi_tx_get_cmd(get_cmd_info);
            if (ret < 0) {
                hi_mipi_tx_err("mipi_tx get cmd failed!\n");
                ret = -1;
            }
            mipi_tx_up_sem();
            break;
        }

        case HI_MIPI_TX_ENABLE: {
            mipi_tx_down_sem_return();
            mipi_tx_enable();
            mipi_tx_up_sem();
            break;
        }

        case HI_MIPI_TX_DISABLE: {
            mipi_tx_down_sem_return();
            mipi_tx_disable();
            mipi_tx_up_sem();
            break;
        }

        default: {
            hi_mipi_tx_err("invalid mipi_tx ioctl cmd\n");
            ret = -1;
            break;
        }
    }

    return ret;
}

#ifdef CONFIG_COMPAT
static long mipi_tx_compat_ioctl(unsigned int cmd, unsigned long arg, void* private_data)
{
    switch (cmd) {
        default:
        {
            break;
        }
    }

    return mipi_tx_ioctl(cmd, arg, private_data);
}
#endif

static int mipi_tx_init(int smooth)
{
    return mipi_tx_drv_init(smooth);
}

static void mipi_tx_exit(void)
{
    mipi_tx_drv_exit();
}

static int mipi_tx_proc_dev_show(osal_proc_entry_t *s)
{
    combo_dev_cfg_t*  pdev_cfg    = NULL;
    sync_info_t*      psync_info  = NULL;
    pdev_cfg = &g_mipi_tx_dev_ctx.dev_cfg;
    psync_info = &g_mipi_tx_dev_ctx.dev_cfg.sync_info;

    /* mipi tx device config */
    osal_seq_printf(s, "----------MIPI_Tx DEV CONFIG---------------------------\n");

    osal_seq_printf(s, "%8s%8s%8s%8s%8s%15s%15s%15s%15s%15s\n",
        "devno", "lane0", "lane1", "lane2", "lane3", "output_mode", "phy_data_rate", "pixel_clk(KHz)",
        "video_mode", "output_fmt");

    osal_seq_printf(s, "%8d%8d%8d%8d%8d%15d%15d%15d%15d%15d\n",
        pdev_cfg->devno,
        pdev_cfg->lane_id[0],
        pdev_cfg->lane_id[1],
        pdev_cfg->lane_id[2], /* lina id 2 */
        pdev_cfg->lane_id[3], /* lina id 3 */
        pdev_cfg->output_mode,
        pdev_cfg->phy_data_rate,
        pdev_cfg->pixel_clk,
        pdev_cfg->video_mode,
        pdev_cfg->output_format);

    osal_seq_printf(s, "\r\n");

    /* mipi tx device sync config */
    osal_seq_printf(s, "----------MIPI_Tx SYNC CONFIG---------------------------\n");

    osal_seq_printf(s, "%14s%14s%14s%14s%14s%14s%14s%14s%14s\n",
        "pkt_size", "hsa_pixels", "hbp_pixels", "hline_pixels", "vsa_lines", "vbp_lines",
        "vfp_lines", "active_lines", "edpi_cmd_size");

    osal_seq_printf(s, "%14d%14d%14d%14d%14d%14d%14d%14d%14d\n",
        psync_info->vid_pkt_size,
        psync_info->vid_hsa_pixels,
        psync_info->vid_hbp_pixels,
        psync_info->vid_hline_pixels,
        psync_info->vid_vsa_lines,
        psync_info->vid_vbp_lines,
        psync_info->vid_vfp_lines,
        psync_info->vid_active_lines,
        psync_info->edpi_cmd_size);

    osal_seq_printf(s, "\r\n");
    return 0;
}

static int mipi_tx_proc_dev_status_show(osal_proc_entry_t *s)
{
    mipi_tx_dev_phy_t mipi_tx_phy_ctx;

    mipi_tx_drv_get_dev_status(&mipi_tx_phy_ctx);

    /* mipi tx phy status */
    osal_seq_printf(s, "----------MIPI_Tx DEV STATUS---------------------------\n");
    osal_seq_printf(s, "%8s%8s%8s%8s%8s%8s%8s\n",
        "width", "height", "HoriAll", "VertAll", "hbp", "hsa", "vsa");
    osal_seq_printf(s, "%8u%8u%8u%8u%8u%8u%8u\n",
        mipi_tx_phy_ctx.hact_det,
        mipi_tx_phy_ctx.vact_det,
        mipi_tx_phy_ctx.hall_det,
        mipi_tx_phy_ctx.vall_det,
        mipi_tx_phy_ctx.hbp_det,
        mipi_tx_phy_ctx.hsa_det,
        mipi_tx_phy_ctx.vsa_det);
    osal_seq_printf(s, "\r\n");
    return 0;
}
static int mipi_tx_proc_show(osal_proc_entry_t *s)
{
    osal_seq_printf(s, "\nModule: [MIPI_TX], Build Time["__DATE__", "__TIME__"]\n");
    mipi_tx_proc_dev_show(s);
    mipi_tx_proc_dev_status_show(s);
    return 0;
}

static int mipi_tx_open(void* private_data)
{
    return 0;
}

static int mipi_tx_release(void *private_data)
{
    return 0;
}

static osal_fileops_t mipi_tx_fops = {
    .open           = mipi_tx_open,
    .release        = mipi_tx_release,
    .unlocked_ioctl = mipi_tx_ioctl,
#ifdef CONFIG_COMPAT
    .compat_ioctl   = mipi_tx_compat_ioctl,
#endif
};

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
static int mipi_tx_freeze(osal_dev_t *pdev)
{
    return 0;
}

static int mipi_tx_restore(osal_dev_t *pdev)
{
    return 0;
}
#else
static int mipi_tx_freeze(osal_dev_t *pdev)
{
    return 0;
}

static int mipi_tx_restore(osal_dev_t *pdev)
{
    return 0;
}
#endif

static osal_pmops_t mipi_tx_pmops = {
    .pm_freeze  = mipi_tx_freeze,
    .pm_restore = mipi_tx_restore,
};

int mipi_tx_register_device(void)
{
    int ret;
    g_mipi_tx_dev = osal_createdev(MIPI_TX_DEV_NAME);
    if (g_mipi_tx_dev == NULL) {
        hi_mipi_tx_err("create mipi_tx device failed!\n");
        return -1;
    }

    g_mipi_tx_dev->fops  = &mipi_tx_fops;
    g_mipi_tx_dev->minor = HIMEDIA_DYNAMIC_MINOR;
    g_mipi_tx_dev->osal_pmops = &mipi_tx_pmops;

    ret = osal_registerdevice(g_mipi_tx_dev);
    if (ret != 0) {
        osal_destroydev(g_mipi_tx_dev);
        g_mipi_tx_dev = NULL;
        hi_mipi_tx_err("register mipi_tx device failed!\n");
        return -1;
    }
    return 0;
}

void mipi_tx_unregister_device(void)
{
    osal_deregisterdevice(g_mipi_tx_dev);
    osal_destroydev(g_mipi_tx_dev);
    g_mipi_tx_dev = NULL;
    return;
}

int mipi_tx_module_init(int smooth)
{
    int ret;
    osal_proc_entry_t *mipi_tx_proc_entry = NULL;

    ret = mipi_tx_register_device();
    if (ret != 0) {
        goto fail0;
    }

    mipi_tx_proc_entry = osal_create_proc_entry(MIPI_TX_PROC_NAME, NULL);
    if (mipi_tx_proc_entry == NULL) {
        hi_mipi_tx_err("create mipi_tx proc(%s) failed!\n", MIPI_TX_PROC_NAME);
        goto fail1;
    }

    mipi_tx_proc_entry->read = mipi_tx_proc_show;
    mipi_tx_proc_entry->write = NULL;

    if ((smooth != TRUE) && (smooth != FALSE)) {
        hi_mipi_tx_err("modual param smooth is illegal!\n");
        goto fail2;
    }

    ret = mipi_tx_init(smooth);
    if (ret != 0) {
        hi_mipi_tx_err("hi_mipi_init failed!\n");
        goto fail2;
    }

    ret = osal_sema_init(&g_mipi_tx_dev_sem, 1);
    if (ret != 0) {
        hi_mipi_tx_err("init sema error!\n");
        goto fail3;
    }

    osal_printk("load mipi_tx driver successful!\n");
    return 0;

fail3:
    mipi_tx_exit();
fail2:
    osal_remove_proc_entry(MIPI_TX_PROC_NAME, NULL);
fail1:
    mipi_tx_unregister_device();
fail0:
    hi_mipi_tx_err("load mipi_tx driver failed!\n");
    return -1;
}

void mipi_tx_module_exit(void)
{
    osal_sema_destory(&g_mipi_tx_dev_sem);

    mipi_tx_exit();

    osal_remove_proc_entry(MIPI_TX_PROC_NAME, NULL);

    mipi_tx_unregister_device();

    osal_printk("unload mipi_tx driver ok!\n");
}

#ifdef __cplusplus
#if __cplusplus
}

#endif
#endif /* End of #ifdef __cplusplus */
