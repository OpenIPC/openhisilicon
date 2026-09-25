/*
 * Copyright (c) XMEDIA. All rights reserved.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/version.h>
#include <linux/of_platform.h>

#include "type.h"
#include "common.h"
#include "osal.h"

#ifndef MODULE

extern int __init osal_driver_init(void);
extern int __init sys_driver_init(void);
extern int __init adc_driver_init(void);
extern int __init vi_driver_init(void);
extern int __init isp_driver_init(void);
extern int __init sysconfig_driver_init(void);
extern int __init acodec_driver_init(void);
extern int __init adec_driver_init(void);
extern int __init aenc_driver_init(void);
extern int __init ai_driver_init(void);
extern int __init aio_driver_init(void);
extern int __init ao_driver_init(void);
extern int __init base_driver_init(void);
extern int __init chnl_driver_init(void);
extern int __init cipher_driver_init(void);
extern int __init gfbg_driver_init(void);          //only for v200 series
extern int __init h264e_driver_init(void);
extern int __init h2645_driver_init(void);
extern int __init piris_driver_init(void);
extern int __init pwm_driver_init(void);
extern int __init sample_ist_driver_init(void);
extern int __init sensor_i2c_driver_init(void);
extern int __init sensor_spi_driver_init(void);
extern int __init ive_driver_init(void);
extern int __init jpege_driver_init(void);
extern int __init mipi_rx_driver_init(void);
extern int __init pdm_driver_init(void);
extern int __init pm_driver_init(void);
extern int __init rc_driver_init(void);
extern int __init venc_driver_init(void);

extern int __init rgn_driver_init(void);
extern int __init ssp_lcd_ota5182_driver_init(void);
extern int __init ssp_lcd_st7789_driver_init(void);
extern int __init ssp_lcd_st7796_driver_init(void);
extern int __init tde_driver_init(void);
extern int __init vedu_driver_init(void);
extern int __init vgs_driver_init(void);
extern int __init vo_driver_init(void);            //only for v200 series
extern int __init vpss_driver_init(void);
extern int __init wdt_driver_init(void);
extern int __init npu_driver_init(void);           //only for v500 series

static int __init xmedia_driver_init(void)
{
    osal_driver_init();
    sysconfig_driver_init();
    base_driver_init();
    sys_driver_init();
    tde_driver_init();
    rgn_driver_init();

    vgs_driver_init();
    vi_driver_init();
    isp_driver_init();
    vpss_driver_init();

#if (defined xm72050200) || (defined xm72020300) || \
    (defined xm72050300) || (defined xm76050100)

    gfbg_driver_init();          //only for v200 series
    vo_driver_init();            //only for v200 series

#endif

    chnl_driver_init();
    vedu_driver_init();
    rc_driver_init();
    venc_driver_init();
    h264e_driver_init();
    h2645_driver_init();
    jpege_driver_init();

    aio_driver_init();
    ai_driver_init();
    ao_driver_init();
    aenc_driver_init();
    adec_driver_init();
    acodec_driver_init();

    piris_driver_init();
    pwm_driver_init();
    sensor_i2c_driver_init();
    sensor_spi_driver_init();
    mipi_rx_driver_init();

//following modules not loaded in quick start mode to save start time.
#ifndef XMEDIA_QUICK_START
    ive_driver_init();
    cipher_driver_init();
#else
    pdm_driver_init();
#endif

//following modules are only loaded if needed.
#if 0
    sample_ist_driver_init();
    ssp_lcd_ota5182_driver_init();
    ssp_lcd_st7789_driver_init();
    ssp_lcd_st7796_driver_init();
    adc_driver_init();
    wdt_driver_init();
#endif

#if (defined xm72050500) || (defined xm72050510) || (defined xm72050530) || (defined xm72050230) || (defined xm72020330)
    pm_driver_init();
    npu_driver_init();
#endif

    return 0;
}

module_init(xmedia_driver_init);
MODULE_LICENSE("GPL");

#endif


