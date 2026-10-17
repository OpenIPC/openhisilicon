/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#include "sensor_common.h"
#include "imx335_cfg.h"
#include "imx335_cmos.h"

static void imx335_default_reg_init(cis_info *cis)
{
    td_u32 i;
    td_s32 ret = TD_SUCCESS;
    ot_isp_sns_state *past_sensor = TD_NULL;

    past_sensor = cis->sns_state;
    if (past_sensor == TD_NULL) {
        return;
    }

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

/* SensorReset(): reset the sensor into standby. */
static td_s32 imx335_reset(cis_info *cis)
{
    td_s32 ret = TD_SUCCESS;

    ret += cis_write_reg(&cis->i2c, 0x3000, 0x01);
    ret += cis_write_reg(&cis->i2c, 0x3001, 0x00);
    cis_delay_ms(18);
    ret += cis_write_reg(&cis->i2c, 0x3002, 0x01);
    ret += cis_write_reg(&cis->i2c, 0x3003, 0x00);
    cis_delay_ms(18);
    ret += cis_write_reg(&cis->i2c, 0x3004, 0x04);
    cis_delay_ms(18);
    ret += cis_write_reg(&cis->i2c, 0x3004, 0x00);

    if (ret != TD_SUCCESS) {
        isp_err_trace("imx335 sensor reset failed!\n");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

/* Write the AE-owned registers (SHR0/gain/VMAX) from regs_info and release
 * standby into streaming. */
static td_s32 imx335_start(cis_info *cis)
{
    imx335_default_reg_init(cis);

    sns_check_return(cis_write_reg(&cis->i2c, 0x3000, 0x00));
    cis_delay_ms(18);
    sns_check_return(cis_write_reg(&cis->i2c, 0x3002, 0x00));

    return TD_SUCCESS;
}

static td_s32 imx335_reg_init(cis_info *cis, cis_reg_cfg *cfg, td_u32 len)
{
    td_u32 i;

    if (imx335_reset(cis) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    for (i = 0; i < len; i++) {
        if (cis_write_reg(&cis->i2c, cfg->addr, cfg->data) != TD_SUCCESS) {
            return TD_FAILURE;
        }
        cfg++;
    }

    return imx335_start(cis);
}

/* Flexible window-crop init (IMX335_CROP_FLEX_LINEAR_MODE). The crop-specific
 * registers are computed at runtime from the per-pipe crop window set by
 * cmos_isp_set_image_mode() from Isp_W/Isp_H; the fixed readout registers are
 * the working 1080p crop table reused as the base, then overridden where the
 * window enters. Ported from the OpenIPC ev200 driver's
 * IMX335_cropped_flex_init().
 *
 * Datasheet constraints (IMX335LQN-C p55):
 *   HNUM = crop_w + 24 (24-pixel colour-processing margin), multiple of 24
 *   HTRIMMING_START = 48 + N*12, 48 <= HTRIMMING_START + HNUM <= 2664
 *   AREA3_WIDTH_1 = Y_OUT_SIZE*2, multiple of 4 (V units, lines*2)
 *   AREA3_ST_ADR_1 = 176 + (1944 - crop_h)
 *   VTTL (VMAX) >= AREA3_WIDTH_1 + 96
 * crop_w is snapped to 24 and crop_h to 4, giving the effective crop limits
 * 312..2592 by 372..1944. */
static td_s32 imx335_flex_reg_init(cis_info *cis)
{
    td_u32 i;
    td_u32 crop_w;
    td_u32 crop_h;
    td_u32 hnum;
    td_u32 hslack;
    td_u32 hstart;
    td_u32 y_out_size;
    td_u32 area3_width;
    td_u32 area3_st;
    td_u32 vmax;
    td_u32 hmax;
    td_float fps;

    imx335_get_crop(cis->pipe, &crop_w, &crop_h);

    crop_w = (crop_w / 24) * 24;
    crop_w = clip3(crop_w, 312, 2592);
    crop_h = (crop_h / 4) * 4;
    crop_h = clip3(crop_h, 372, 1944);

    hnum = crop_w + 24;
    hslack = (2616 > hnum) ? (2616 - hnum) : 0;
    hstart = 48 + ((hslack / 2) / 12) * 12;

    y_out_size = crop_h + 20;
    area3_width = y_out_size * 2;
    area3_st = 176 + (1944 - crop_h);

    vmax = IMX335_FLEX_VMAX(crop_h);

    /* T6: keep VMAX at the crop length and carry the frame rate with HMAX. */
    fps = imx335_get_flex_fps(cis->pipe);
    hmax = (td_u32)((td_float)IMX335_FLEX_CLK / (fps * (td_float)vmax) + 0.5f);
    hmax = clip3(hmax, IMX335_FLEX_HMAX_MIN, IMX335_FLEX_HMAX_MAX);

    if (imx335_reset(cis) != TD_SUCCESS) {
        return TD_FAILURE;
    }

    for (i = 0; i < (td_u32)(sizeof(imx335_1080p_crop_10bit) / sizeof(imx335_1080p_crop_10bit[0])); i++) {
        if (cis_write_reg(&cis->i2c, imx335_1080p_crop_10bit[i].addr,
                          imx335_1080p_crop_10bit[i].data) != TD_SUCCESS) {
            return TD_FAILURE;
        }
    }

    {
        cis_reg_cfg crop_regs[] = {
            {0x302C, hstart & 0xFF},
            {0x302D, (hstart >> 8) & 0x3F},
            {0x302E, hnum & 0xFF},
            {0x302F, (hnum >> 8) & 0x1F},
            {IMX335_VMAX_ADDR_L, vmax & 0xFF},
            {IMX335_VMAX_ADDR_M, (vmax >> 8) & 0xFF},
            {IMX335_VMAX_ADDR_H, (vmax >> 16) & 0x0F},
            {IMX335_HMAX_ADDR_L, hmax & 0xFF},
            {IMX335_HMAX_ADDR_H, (hmax >> 8) & 0x1F},
            {0x3056, y_out_size & 0xFF},
            {0x3057, (y_out_size >> 8) & 0x1F},
            {0x3074, area3_st & 0xFF},
            {0x3075, (area3_st >> 8) & 0x1F},
            {0x3076, area3_width & 0xFF},
            {0x3077, (area3_width >> 8) & 0x1F},
        };

        for (i = 0; i < (td_u32)(sizeof(crop_regs) / sizeof(crop_regs[0])); i++) {
            if (cis_write_reg(&cis->i2c, crop_regs[i].addr, crop_regs[i].data) != TD_SUCCESS) {
                isp_err_trace("imx335 flex: crop write 0x%04x failed\n", crop_regs[i].addr);
                return TD_FAILURE;
            }
        }
    }

    printf("IMX335 CROP_FLEX: crop=%ux%u (HNUM=%u HSTART=%u Y_OUT=%u AREA3_ST=%u AREA3_W=%u VMAX=%u HMAX=%u fps=%.2f)\n",
        crop_w, crop_h, hnum, hstart, y_out_size, area3_st, area3_width, vmax, hmax, fps);

    return imx335_start(cis);
}

td_s32 imx335_init(cis_info *cis)
{
    td_s32 ret;
    td_u32 len;
    cis_reg_cfg *cfg = TD_NULL;
    ot_isp_sns_state *sns_state = TD_NULL;
    const char *mode_name = TD_NULL;

    sns_check_pointer_return(cis);
    sns_state = cis->sns_state;
    sns_check_pointer_return(sns_state);

    if (sns_state->img_mode == IMX335_CROP_FLEX_LINEAR_MODE) {
        ret = imx335_flex_reg_init(cis);
        mode_name = "IMX335_CROP_FLEX_LINEAR_MODE";
        goto init_done;
    }

    switch (sns_state->img_mode) {
        case IMX335_1520P_10BIT_MODE:
            cfg = imx335_1520p_10bit;
            len = (td_u32)(sizeof(imx335_1520p_10bit) / sizeof(imx335_1520p_10bit[0]));
            mode_name = "IMX335_1520P_10BIT_MODE";
            break;
        case IMX335_60FPS_CROPPED_1080P_MODE:
            cfg = imx335_1080p_crop_10bit;
            len = (td_u32)(sizeof(imx335_1080p_crop_10bit) / sizeof(imx335_1080p_crop_10bit[0]));
            mode_name = "IMX335_60FPS_CROPPED_1080P_MODE";
            break;
        case IMX335_60FPS_BINNING_MODE:
            cfg = imx335_binning_60fps;
            len = (td_u32)(sizeof(imx335_binning_60fps) / sizeof(imx335_binning_60fps[0]));
            mode_name = "IMX335_60FPS_BINNING_MODE";
            break;
        case IMX335_60FPS_FULL_1944P_MODE:
            cfg = imx335_full_5m_10bit_40fps;
            len = (td_u32)(sizeof(imx335_full_5m_10bit_40fps) / sizeof(imx335_full_5m_10bit_40fps[0]));
            mode_name = "IMX335_60FPS_FULL_1944P_MODE";
            break;
        case IMX335_5M_30FPS_12BIT_LINEAR_MODE:
        default:
            cfg = imx335_linear_5m30_12bit;
            len = (td_u32)(sizeof(imx335_linear_5m30_12bit) / sizeof(imx335_linear_5m30_12bit[0]));
            mode_name = "IMX335_5M_30FPS_12BIT_LINEAR";
            break;
    }

    ret = imx335_reg_init(cis, cfg, len);

init_done:
    if (ret != TD_SUCCESS) {
        isp_err_trace("imx335_reg_init failed!\n");
        return ret;
    }

    printf("===================================================================================\n");
    printf("vi_pipe:%d,== %s Init OK! ==\n", cis->pipe, mode_name);
    printf("===================================================================================\n");

    return TD_SUCCESS;
}

td_s32 imx335_get_standby_cfg(ot_isp_sns_regs_info *standby_cfg)
{
    td_u32 i;

    sns_check_pointer_return(standby_cfg);

    standby_cfg->reg_num = (td_u32)(sizeof(imx335_standby_cfg) / sizeof(imx335_standby_cfg[0]));

    for (i = 0; i < standby_cfg->reg_num; i++) {
        standby_cfg->i2c_data[i].dev_addr = IMX335_I2C_ADDR;
        standby_cfg->i2c_data[i].addr_byte_num = IMX335_ADDR_BYTE;
        standby_cfg->i2c_data[i].data_byte_num = IMX335_DATA_BYTE;
        standby_cfg->i2c_data[i].reg_addr = imx335_standby_cfg[i].addr;
        standby_cfg->i2c_data[i].data = imx335_standby_cfg[i].data;
    }

    return TD_SUCCESS;
}
