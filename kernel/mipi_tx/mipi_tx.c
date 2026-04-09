/******************************************************************************
Copyright (C), 2016, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : mipi_tx.c
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2016/11/8
Last Modified :
Description   : This program is free software; you can redistribute it and/or modify
              : it under the terms of the GNU General Public License as published by
              : the Free Software Foundation; either version 2 of the License, or
              : (at your option) any later version.
              : This program is distributed in the hope that it will be useful,
              : but WITHOUT ANY WARRANTY; without even the implied warranty of
              : MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
              : GNU General Public License for more details.
              : You should have received a copy of the GNU General Public License
              : along with this program;
Function List :
******************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#include "hi_osal.h"
#include "type.h"
#include "hi_mipi_tx.h"
#include "mipi_tx_hal.h"
#include "mipi_tx_reg.h"
/*
 * macro definition
 */
#define MIPI_TX_DEV_NAME           "hi_mipi_tx"
#define MIPI_TX_PROC_NAME          "mipi_tx"

#define HIMEDIA_DYNAMIC_MINOR      255

#define NULL    0L


typedef struct {
    combo_dev_cfg_t dev_cfg;
} mipi_tx_dev_ctx_t;
/*
 * global variables definition
 */
static osal_dev_t* mipi_tx_dev;
extern volatile mipi_tx_regs_type_t *mipi_tx_regs_va;

mipi_tx_dev_ctx_t mipi_tx_dev_ctx;
mipi_tx_dev_phy_t mipi_tx_phy_ctx;
/*
 * function definition
 */
static int mipi_tx_check_comb_dev_cfg(combo_dev_cfg_t *dev_cfg)
{
    return 0;
}

static int mipi_tx_set_combo_dev_cfg(combo_dev_cfg_t *dev_cfg)
{
    int ret;

    ret = mipi_tx_check_comb_dev_cfg(dev_cfg);
    if (ret < 0) {
        HI_MIPI_TX_ERR("mipi_tx check combo_dev config failed!\n");
        return -1;
    }

    /* set controler config */
    mipi_tx_drv_set_controller_cfg(dev_cfg);

    /* set phy config*/
    mipi_tx_drv_set_phy_cfg(dev_cfg);


    osal_memcpy(&mipi_tx_dev_ctx.dev_cfg, dev_cfg, sizeof(combo_dev_cfg_t));

    return ret;
}

static int mipi_tx_set_cmd(cmd_info_t *cmd_info)
{
    return mipi_tx_drv_set_cmd_info(cmd_info);
}

static int mipi_tx_get_cmd(get_cmd_info_t *get_cmd_info)
{
    return mipi_tx_drv_get_cmd_info(get_cmd_info);
}

static void mipi_tx_enable(void)
{
    output_mode_t output_mode;

    output_mode = mipi_tx_dev_ctx.dev_cfg.output_mode;

    mipi_tx_drv_enable_input(output_mode);
}

static void mipi_tx_disable(void)
{
    mipi_tx_drv_disable_input();
}


static long mipi_tx_ioctl(unsigned int cmd, unsigned long arg, void* private_data)
{
    int ret = 0;

    switch (cmd) {
        case HI_MIPI_TX_SET_DEV_CFG:
        {
            combo_dev_cfg_t *pstcombo_dev_cfg = NULL;

            pstcombo_dev_cfg = (combo_dev_cfg_t *)arg;

            MIPITX_CHECK_NULL_PTR(pstcombo_dev_cfg);

            ret = mipi_tx_set_combo_dev_cfg(pstcombo_dev_cfg);
            if (ret < 0) {
                HI_MIPI_TX_ERR("mipi_tx set combo_dev config failed!\n");
                ret = -1;
            }
            break;
        }

        case HI_MIPI_TX_SET_CMD:
        {
            cmd_info_t *cmd_info = NULL;

            cmd_info = (cmd_info_t *)arg;

            MIPITX_CHECK_NULL_PTR(cmd_info);

            ret = mipi_tx_set_cmd(cmd_info);
            if (ret < 0) {
                HI_MIPI_TX_ERR("mipi_tx set cmd failed!\n");
                ret = -1;
            }
            break;
        }

        case HI_MIPI_TX_GET_CMD:
        {
            get_cmd_info_t *get_cmd_info = NULL;
            get_cmd_info = (get_cmd_info_t *)arg;
            MIPITX_CHECK_NULL_PTR(get_cmd_info);
            ret = mipi_tx_get_cmd(get_cmd_info);
            if (ret < 0) {
                HI_MIPI_TX_ERR("mipi_tx get cmd failed!\n");
                ret = -1;
            }
            break;
        }

        case HI_MIPI_TX_ENABLE:
        {
            mipi_tx_enable();
            break;
        }

        case HI_MIPI_TX_DISABLE:
        {
            mipi_tx_disable();
            break;
        }

        default:
        {
            HI_MIPI_TX_ERR("invalid mipi_tx ioctl cmd\n");
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
    pdev_cfg = &mipi_tx_dev_ctx.dev_cfg;
    psync_info = &mipi_tx_dev_ctx.dev_cfg.sync_info;


    /* mipi tx device config */
    osal_seq_printf(s, "----------MIPI_Tx DEV CONFIG---------------------------\n");

    osal_seq_printf(s, "%8s%8s%8s%8s%8s%15s%15s%15s%15s%15s\n",
        "devno",
        "lane0",
        "lane1",
        "lane2",
        "lane3",
        "output_mode",
        "phy_data_rate",
        "pixel_clk(KHz)",
        "video_mode",
        "output_fmt");

    osal_seq_printf(s, "%8d%8d%8d%8d%8d%15d%15d%15d%15d%15d\n",
        pdev_cfg->devno,
        pdev_cfg->lane_id[0],
        pdev_cfg->lane_id[1],
        pdev_cfg->lane_id[2],
        pdev_cfg->lane_id[3],
        pdev_cfg->output_mode,
        pdev_cfg->phy_data_rate,
        pdev_cfg->pixel_clk,
        pdev_cfg->video_mode,
        pdev_cfg->output_format);

    osal_seq_printf(s, "\r\n");

    /* mipi tx device sync config */
    osal_seq_printf(s, "----------MIPI_Tx SYNC CONFIG---------------------------\n");

    osal_seq_printf(s, "%14s%14s%14s%14s%14s%14s%14s%14s%14s\n",
        "pkt_size",
        "hsa_pixels",
        "hbp_pixels",
        "hline_pixels",
        "vsa_lines",
        "vbp_lines",
        "vfp_lines",
        "active_lines",
        "edpi_cmd_size");

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
    mipi_tx_phy_ctx.hact_det = mipi_tx_regs_va->HORI0_DET.bits.hact_det;
    mipi_tx_phy_ctx.hall_det = mipi_tx_regs_va->HORI0_DET.bits.hline_det;
    mipi_tx_phy_ctx.hbp_det  = mipi_tx_regs_va->HORI1_DET.bits.hbp_det;
    mipi_tx_phy_ctx.hsa_det  = mipi_tx_regs_va->HORI1_DET.bits.hsa_det;

    mipi_tx_phy_ctx.vact_det = mipi_tx_regs_va->VERT_DET.bits.vact_det;
    mipi_tx_phy_ctx.vall_det = mipi_tx_regs_va->VERT_DET.bits.vall_det;
    mipi_tx_phy_ctx.vsa_det  = mipi_tx_regs_va->VSA_DET.bits.vsa_det;

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
    // osal_seq_printf(s, "\nModule: [MIPI_TX], Build Time["__DATE__", "__TIME__"]\n");
    osal_seq_printf(s, "\nModule: [MIPI_TX], Build Time[202009291535]\n");
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

int mipi_tx_module_init(int smooth)
{
    int ret;
    osal_proc_entry_t *mipi_tx_proc_entry;

    mipi_tx_dev = osal_createdev(MIPI_TX_DEV_NAME);
    if (mipi_tx_dev == NULL) {
        HI_MIPI_TX_ERR("create mipi_tx device failed!\n");
        goto fail0;
    }

    mipi_tx_dev->fops  = &mipi_tx_fops;
    mipi_tx_dev->minor = HIMEDIA_DYNAMIC_MINOR;
    mipi_tx_dev->osal_pmops = &mipi_tx_pmops;

    ret = osal_registerdevice(mipi_tx_dev);
    if (ret != 0) {
        HI_MIPI_TX_ERR("register mipi_tx device failed!\n");
        goto fail1;
    }

    mipi_tx_proc_entry = osal_create_proc_entry(MIPI_TX_PROC_NAME, NULL);
    if (mipi_tx_proc_entry == NULL) {
        HI_MIPI_TX_ERR("create mipi_tx proc(%s) failed!\n", MIPI_TX_PROC_NAME);
        goto fail2;
    }

    mipi_tx_proc_entry->read = mipi_tx_proc_show;
    mipi_tx_proc_entry->write = NULL;

    if ((smooth != TRUE) && (smooth != FALSE)) {
        HI_MIPI_TX_ERR("modual param smooth is illegal!\n");
        goto fail3;
    }

    ret = mipi_tx_init(smooth);
    if (ret != 0) {
        HI_MIPI_TX_ERR("hi_mipi_init failed!\n");
        goto fail3;
    }

    osal_printk("load mipi_tx driver successful!\n");
    return 0;

fail3:
    osal_remove_proc_entry(MIPI_TX_PROC_NAME, NULL);
fail2:
    osal_deregisterdevice(mipi_tx_dev);
fail1:
    osal_destroydev(mipi_tx_dev);
    mipi_tx_dev = NULL;
fail0:
    HI_MIPI_TX_ERR("load mipi_tx driver failed!\n");
    return -1;
}

void mipi_tx_module_exit(void)
{
    mipi_tx_exit();

    osal_remove_proc_entry(MIPI_TX_PROC_NAME, NULL);

    osal_deregisterdevice(mipi_tx_dev);

    osal_destroydev(mipi_tx_dev);
    mipi_tx_dev = NULL;

    osal_printk("unload mipi_tx driver ok!\n");
}

#ifdef __cplusplus
#if __cplusplus
}

#endif
#endif /* End of #ifdef __cplusplus */
