/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#include "sensor_common.h"
#include "os04d10_cfg.h"

static void os04d10_default_reg_init(cis_info *cis)
{
    td_u32 i;
    td_s32 ret = TD_SUCCESS;
    ot_isp_sns_state *past_sensor = TD_NULL;

    past_sensor = cis->sns_state;
    for (i = 0; i < past_sensor->regs_info[0].reg_num; i++) {
        ret += cis_write_reg(&cis->i2c,
            past_sensor->regs_info[0].i2c_data[i].reg_addr,
            past_sensor->regs_info[0].i2c_data[i].data);
    }

    if (ret != TD_SUCCESS) {
        isp_err_trace("write register failed!\n");
    }
    return;
}

static td_s32 os04d10_reg_init(cis_info *cis, cis_reg_cfg *cfg, td_u32 len)
{
    td_u32 i;

    for (i = 0; i < len; i++) {
        sns_check_return(cis_write_reg(&cis->i2c, cfg->addr, cfg->data));
        cfg++;
    }

    os04d10_default_reg_init(cis);

    return TD_SUCCESS;
}

td_s32 os04d10_linear_4m30_10bit_init(cis_info *cis)
{
    td_s32 ret;
    td_u32 len;
    cis_reg_cfg *cfg = os04d10_linear_4m30_10bit;

    sns_check_pointer_return(cis);

    len = (td_u32)(sizeof(os04d10_linear_4m30_10bit) / sizeof(os04d10_linear_4m30_10bit[0]));
    ret = os04d10_reg_init(cis, cfg, len);
    if (ret != TD_SUCCESS) {
        isp_err_trace("os04d10_reg_init failed!\n");
        return ret;
    }

    printf("===================================================================================\n");
    printf("vi_pipe:%d,== os04d10 24Mclk 4M30fps(MIPI) 2LANE_720Mbps 10bit linear Init OK! ==\n", cis->pipe);
    printf("===================================================================================\n");

    return TD_SUCCESS;
}
