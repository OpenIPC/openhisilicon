/* IMX662 bring-up (xipc sequence: standby, config, AE regs, cancel, master-start). */
#include <pthread.h>
#include "sensor_common.h"
#include "imx662_cmos.h"
#include "imx662_cfg.h"
#include "imx662_sensor_ctrl.h"

/* The orientation the caller asked for, per pipe. pfn_mirror_flip arrives on
 * the caller's thread and the bring-up runs on the ISP thread, so both hold
 * cis->mutex while they read this and write the sensor; nothing else in the
 * common layer holds that lock across a sensor call. Cleared at unregister;
 * the caller re-issues it from its own configuration. */
static td_bool g_imx662_mirror[OT_ISP_MAX_PIPE_NUM];
static td_bool g_imx662_flip[OT_ISP_MAX_PIPE_NUM];

static td_void imx662_lock(cis_info *cis)
{
    if (pthread_mutex_lock(&cis->mutex) != 0) {
        isp_err_trace("imx662: mutex lock failed!\n");
    }
}

static td_void imx662_unlock(cis_info *cis)
{
    if (pthread_mutex_unlock(&cis->mutex) != 0) {
        isp_err_trace("imx662: mutex unlock failed!\n");
    }
}

/* HREVERSE and VREVERSE are all it takes on this sensor: the readout window
 * and its Bayer phase follow them. Both are always written, so an orientation
 * a previous session left behind cannot survive a clear. */
static td_s32 imx662_write_orientation_locked(cis_info *cis)
{
    ot_vi_pipe pipe = cis->pipe;
    td_s32 ret = TD_SUCCESS;

    if (pipe < 0 || pipe >= OT_ISP_MAX_PIPE_NUM) {
        return TD_FAILURE;
    }
    ret += cis_write_reg(&cis->i2c, IMX662_HREVERSE_ADDR, g_imx662_mirror[pipe] ? 0x01 : 0x00);
    ret += cis_write_reg(&cis->i2c, IMX662_VREVERSE_ADDR, g_imx662_flip[pipe] ? 0x01 : 0x00);
    return (ret == TD_SUCCESS) ? TD_SUCCESS : TD_FAILURE;
}

td_s32 imx662_set_orientation(cis_info *cis, td_bool mirror, td_bool flip)
{
    td_s32 ret = TD_SUCCESS;

    sns_check_pointer_return(cis);
    if (cis->pipe < 0 || cis->pipe >= OT_ISP_MAX_PIPE_NUM) {
        return TD_FAILURE;
    }
    imx662_lock(cis);
    g_imx662_mirror[cis->pipe] = mirror;
    g_imx662_flip[cis->pipe] = flip;
    /* Before the first bring-up there is no bus yet; the bring-up applies it. */
    if (cis->i2c.fd >= 0) {
        ret = imx662_write_orientation_locked(cis);
    }
    imx662_unlock(cis);
    return ret;
}

td_void imx662_clear_orientation(ot_vi_pipe vi_pipe)
{
    if (vi_pipe < 0 || vi_pipe >= OT_ISP_MAX_PIPE_NUM) {
        return;
    }
    g_imx662_mirror[vi_pipe] = TD_FALSE;
    g_imx662_flip[vi_pipe] = TD_FALSE;
}

static void imx662_default_reg_init(cis_info *cis)
{
    td_u32 i;
    td_s32 ret = TD_SUCCESS;
    ot_isp_sns_state *s = cis->sns_state;
    for (i = 0; i < s->regs_info[0].reg_num; i++) {
        ret += cis_write_reg(&cis->i2c, s->regs_info[0].i2c_data[i].reg_addr,
                             s->regs_info[0].i2c_data[i].data);
    }
    if (ret != TD_SUCCESS) {
        isp_err_trace("write register failed!\n");
    }
}

static td_s32 imx662_reg_init(cis_info *cis, cis_reg_cfg *cfg, td_u32 len, const char *banner)
{
    td_u32 i;
    td_s32 ret = TD_FAILURE;

    imx662_lock(cis);
    if (cis_write_reg(&cis->i2c, 0x3000, 0x01) != TD_SUCCESS) { /* standby */
        goto out;
    }
    for (i = 0; i < len; i++) {
        if (cis_write_reg(&cis->i2c, cfg->addr, cfg->data) != TD_SUCCESS) {
            goto out;
        }
        cfg++;
    }
    imx662_default_reg_init(cis);
    /* In standby, so the first frame already comes out the way it was asked. */
    if (imx662_write_orientation_locked(cis) != TD_SUCCESS) {
        isp_err_trace("imx662: writing the orientation failed!\n");
        goto out;
    }
    if (cis_write_reg(&cis->i2c, 0x3000, 0x00) != TD_SUCCESS) { /* standby cancel */
        goto out;
    }
    cis_delay_ms(20);                                           /* 20ms */
    if (cis_write_reg(&cis->i2c, 0x3002, 0x00) != TD_SUCCESS) { /* master mode start */
        goto out;
    }
    printf("===%s Init OK!===\n", banner);
    ret = TD_SUCCESS;
out:
    imx662_unlock(cis);
    return ret;
}

td_s32 imx662_linear_2m30_12bit_init(cis_info *cis)
{
    sns_check_pointer_return(cis);
    return imx662_reg_init(cis, imx662_linear_2m30_12bit,
        sizeof(imx662_linear_2m30_12bit) / sizeof(imx662_linear_2m30_12bit[0]),
        "IMX662 2M 30fps 12bit LINE");
}
td_s32 imx662_linear_2m60_12bit_init(cis_info *cis)
{
    sns_check_pointer_return(cis);
    return imx662_reg_init(cis, imx662_linear_2m60_12bit,
        sizeof(imx662_linear_2m60_12bit) / sizeof(imx662_linear_2m60_12bit[0]),
        "IMX662 2M 60fps 12bit LINE");
}
td_s32 imx662_linear_2m90_10bit_init(cis_info *cis)
{
    sns_check_pointer_return(cis);
    return imx662_reg_init(cis, imx662_linear_2m90_10bit,
        sizeof(imx662_linear_2m90_10bit) / sizeof(imx662_linear_2m90_10bit[0]),
        "IMX662 2M 90fps 10bit LINE");
}
