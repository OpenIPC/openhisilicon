/*
 * Copyright (c); XMEDIA. All rights reserved.
 */

#include "msensor_chip_proc.h"
#include <linux/kernel.h>
#include "osal.h"
#include "msensor.h"
#include "icm42670.h"

#define MSENSOR_CHIP_INFO         "msensor_chip"
#define MSENSOR_CHIP_VERSION_INFO "msensor_chip debug 0.0.0.1"

static xmedia_char *msensor_triger_mode2_str(msensor_triger_mode mode)
{
    switch (mode) {
        case TRIGER_TIMER:
            return "TIMER";
        case TRIGER_EXTERN_INTERRUPT:
            return "GPIO_INTERRUPT";
        default:
            return "error";
    }
    return "0";
}

static xmedia_void msensor_print_proc_title(osal_proc_entry_t *s, xmedia_char *title)
{
    xmedia_s32 i;

    for (i = 0; i < 50; i++) {
        osal_seq_printf(s, "-");
    }

    osal_seq_printf(s, title);

    for (i = 0; i < 50; i++) {
        osal_seq_printf(s, "-");
    }
    osal_seq_printf(s, "\n");
}

static xmedia_s32 msensor_chip_proc_show(osal_proc_entry_t *s)
{
    xmedia_s32 i;
    xmedia_msensor_param *msensor_param = XMEDIA_NULL;
    icm42670_dev_info *dev_info = XMEDIA_NULL;


    osal_seq_printf(s,
                    "[msensor] version:[" MSENSOR_CHIP_VERSION_INFO "], build time["__DATE__
                    ", "__TIME__
                    "]\n");

    for (i = 0; i < XMEDIA_MSENSOR_MAX_DEV_NUM; i++) {
        if(msensor_chip_get_status(i) == XMEDIA_FALSE) {
            continue;
        }
        msensor_param = msensor_chip_get_param(i);
        dev_info = chip_get_dev_info(i);
        msensor_print_proc_title(s, "common parameter");
        osal_seq_printf(s, "%24s %24s %24s\n", "dev_no", "trigle_mode", "fifo_en");
        osal_seq_printf(s, "%24d %24s %24d\n", i, msensor_triger_mode2_str(dev_info->triger_data.triger_mode), dev_info->fifo_en);

        if (msensor_param->attr.device_mask & XMEDIA_MSENSOR_DEVICE_GYRO) {
            msensor_print_proc_title(s, "gyro parameter");
            osal_seq_printf(s, "%24s\n", "##ICM42670##");

            osal_seq_printf(s, "%24s %24s %24s %24s %24s\n", "sample_rate", "full-scale-range", "datawidth",
                            "max-chip-temperature", "min-chip-temperature");
            osal_seq_printf(s, "%24d %24d %24d %24d %24d\n", msensor_param->config.gyro_config.odr,
                            msensor_param->config.gyro_config.fsr, msensor_param->config.gyro_config.data_width,
                            msensor_param->config.gyro_config.temperature_max,
                            msensor_param->config.gyro_config.temperature_min);
        }
        if (msensor_param->attr.device_mask & XMEDIA_MSENSOR_DEVICE_ACC) {
            msensor_print_proc_title(s, "accelerometer parameter");

            osal_seq_printf(s, "%24s\n", "##ICM42670##");

            osal_seq_printf(s, "%24s %24s %24s %24s %24s\n", "sample_rate", "full-scale-range", "datawidth",
                            "max-chip-temperature", "min-chip-temperature");
            osal_seq_printf(s, "%24d %24d %24d %24d %24d\n", msensor_param->config.acc_config.odr,
                            msensor_param->config.acc_config.fsr, msensor_param->config.acc_config.data_width,
                            msensor_param->config.acc_config.temperature_max,
                            msensor_param->config.acc_config.temperature_min);
        }
    }

    return 0;
}

xmedia_s32 mpu_proc_init(xmedia_void)
{
    osal_proc_entry_t *msensor_chip_entry = XMEDIA_NULL;

    msensor_chip_entry = osal_create_proc_entry(MSENSOR_CHIP_INFO, XMEDIA_NULL);
    if (msensor_chip_entry == XMEDIA_NULL) {
        printk("osal_create_proc_entry failed!\n");
        return XMEDIA_FAILURE;
    }

    msensor_chip_entry->read = msensor_chip_proc_show;
    msensor_chip_entry->write = XMEDIA_NULL;
    return XMEDIA_SUCCESS;
}

void mpu_proc_exit(xmedia_void)
{
    osal_remove_proc_entry(MSENSOR_CHIP_INFO, 0);
    return;
}

