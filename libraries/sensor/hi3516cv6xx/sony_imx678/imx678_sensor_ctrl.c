/* IMX678 bring-up: standby, the mode table, the AE slots, orientation, standby
 * cancel, 24 ms, master start. */
#include <pthread.h>
#include "sensor_common.h"
#include "imx678_cmos.h"
#include "imx678_cfg.h"
#include "imx678_sensor_ctrl.h"

/* The orientation the caller asked for, per pipe. pfn_mirror_flip arrives on
 * the caller's thread and the bring-up runs on the ISP thread, so both hold
 * cis->mutex while they read this and write the sensor; nothing else in the
 * common layer holds that lock across a sensor call. Cleared at unregister;
 * the caller re-issues it from its own configuration. */
static td_bool g_imx678_mirror[OT_ISP_MAX_PIPE_NUM];
static td_bool g_imx678_flip[OT_ISP_MAX_PIPE_NUM];

static td_void imx678_lock(cis_info *cis)
{
    if (pthread_mutex_lock(&cis->mutex) != 0) {
        isp_err_trace("imx678: mutex lock failed!\n");
    }
}

static td_void imx678_unlock(cis_info *cis)
{
    if (pthread_mutex_unlock(&cis->mutex) != 0) {
        isp_err_trace("imx678: mutex unlock failed!\n");
    }
}

/* HREVERSE and VREVERSE are all it takes on this sensor: the readout window
 * follows them and the Bayer phase stays RGGB (the upstream sony-starvis2
 * driver reports SRGGB12 in every orientation). Both are always written, so
 * an orientation a previous session left behind cannot survive a clear. */
static td_s32 imx678_write_orientation_locked(cis_info *cis)
{
    ot_vi_pipe pipe = cis->pipe;
    td_s32 ret = TD_SUCCESS;

    if (pipe < 0 || pipe >= OT_ISP_MAX_PIPE_NUM) {
        return TD_FAILURE;
    }
    ret += cis_write_reg(&cis->i2c, IMX678_HREVERSE_ADDR, g_imx678_mirror[pipe] ? 0x01 : 0x00);
    ret += cis_write_reg(&cis->i2c, IMX678_VREVERSE_ADDR, g_imx678_flip[pipe] ? 0x01 : 0x00);
    return (ret == TD_SUCCESS) ? TD_SUCCESS : TD_FAILURE;
}

td_s32 imx678_set_orientation(cis_info *cis, td_bool mirror, td_bool flip)
{
    td_s32 ret = TD_SUCCESS;

    sns_check_pointer_return(cis);
    if (cis->pipe < 0 || cis->pipe >= OT_ISP_MAX_PIPE_NUM) {
        return TD_FAILURE;
    }
    imx678_lock(cis);
    g_imx678_mirror[cis->pipe] = mirror;
    g_imx678_flip[cis->pipe] = flip;
    /* Before the first bring-up there is no bus yet; the bring-up applies it. */
    if (cis->i2c.fd >= 0) {
        ret = imx678_write_orientation_locked(cis);
    }
    imx678_unlock(cis);
    return ret;
}

td_void imx678_clear_orientation(ot_vi_pipe vi_pipe)
{
    if (vi_pipe < 0 || vi_pipe >= OT_ISP_MAX_PIPE_NUM) {
        return;
    }
    g_imx678_mirror[vi_pipe] = TD_FALSE;
    g_imx678_flip[vi_pipe] = TD_FALSE;
}

static td_s32 imx678_default_reg_init(cis_info *cis)
{
    td_u32 i;
    ot_isp_sns_state *s = cis->sns_state;
    for (i = 0; i < s->regs_info[0].reg_num; i++) {
        if (cis_write_reg(&cis->i2c, s->regs_info[0].i2c_data[i].reg_addr,
                          s->regs_info[0].i2c_data[i].data) != TD_SUCCESS) {
            isp_err_trace("imx678: write 0x%04x failed!\n", s->regs_info[0].i2c_data[i].reg_addr);
            return TD_FAILURE;
        }
    }
    return TD_SUCCESS;
}

static td_s32 imx678_reg_init(cis_info *cis, cis_reg_cfg *cfg, td_u32 len, const char *banner)
{
    td_u32 i;
    td_s32 ret = TD_FAILURE;

    imx678_lock(cis);
    if (cis_write_reg(&cis->i2c, 0x3000, 0x01) != TD_SUCCESS) { /* standby */
        goto out;
    }
    for (i = 0; i < len; i++) {
        if (cis_write_reg(&cis->i2c, cfg->addr, cfg->data) != TD_SUCCESS) {
            goto out;
        }
        cfg++;
    }
    if (imx678_default_reg_init(cis) != TD_SUCCESS) {
        goto out;
    }
    /* In standby, so the first frame already comes out the way it was asked. */
    if (imx678_write_orientation_locked(cis) != TD_SUCCESS) {
        isp_err_trace("imx678: writing the orientation failed!\n");
        goto out;
    }
    if (cis_write_reg(&cis->i2c, 0x3000, 0x00) != TD_SUCCESS) { /* standby cancel */
        goto out;
    }
    /* The datasheet's internal regulator stabilization before XMSTA = 0 is
     * 24 ms; the Hi3519DV500 driver waited 18. */
    cis_delay_ms(IMX678_REGULATOR_WAIT_MS);
    if (cis_write_reg(&cis->i2c, 0x3002, 0x00) != TD_SUCCESS) { /* master mode start */
        goto out;
    }
    printf("===%s Init OK!===\n", banner);
    ret = TD_SUCCESS;
out:
    imx678_unlock(cis);
    return ret;
}

td_s32 imx678_linear_4k30_12bit_init(cis_info *cis)
{
    sns_check_pointer_return(cis);
    return imx678_reg_init(cis, imx678_linear_4k30_12bit,
        sizeof(imx678_linear_4k30_12bit) / sizeof(imx678_linear_4k30_12bit[0]),
        "IMX678 8M 30fps 12bit LINE");
}

/* Standby and restart as the Hi3519DV500 driver does them: master stop with
 * standby, standby cancel then master start. Nothing to do before the first
 * bring-up opened the bus. */
td_s32 imx678_standby(cis_info *cis)
{
    td_s32 ret = TD_SUCCESS;

    sns_check_pointer_return(cis);
    imx678_lock(cis);
    if (cis->i2c.fd >= 0) {
        ret += cis_write_reg(&cis->i2c, 0x3000, 0x01); /* standby */
        ret += cis_write_reg(&cis->i2c, 0x3002, 0x01); /* master mode stop */
    }
    imx678_unlock(cis);
    return (ret == TD_SUCCESS) ? TD_SUCCESS : TD_FAILURE;
}

td_s32 imx678_restart(cis_info *cis)
{
    td_s32 ret = TD_SUCCESS;

    sns_check_pointer_return(cis);
    imx678_lock(cis);
    if (cis->i2c.fd >= 0) {
        ret += cis_write_reg(&cis->i2c, 0x3000, 0x00); /* standby cancel */
        cis_delay_ms(IMX678_REGULATOR_WAIT_MS);
        ret += cis_write_reg(&cis->i2c, 0x3002, 0x00); /* master mode start */
    }
    imx678_unlock(cis);
    return (ret == TD_SUCCESS) ? TD_SUCCESS : TD_FAILURE;
}
