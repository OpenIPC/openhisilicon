/*
 * SC850SL bring-up, as the vendor's Hi3516CV610 firmware (SIP-K662C6S, xipc)
 * does it: soft reset, the register table (sc850sl_cfg.h, recovered byte for
 * byte from xipc), the AE sync registers, stream on. The order is the one
 * smart_sc450ai uses, which is the vendor's own CV610 SmartSens driver.
 */
#include <pthread.h>
#include "sensor_common.h"
#include "sc850sl_cmos.h"
#include "sc850sl_cfg.h"
#include "sc850sl_sensor_ctrl.h"

/* The orientation the caller asked for, per pipe. pfn_mirror_flip arrives on
 * the caller's thread and the bring-up runs on the ISP thread, so both hold
 * cis->mutex while they read this and write the sensor. Cleared at
 * unregister; the caller re-issues it from its own configuration. */
static td_bool g_sc850sl_mirror[OT_ISP_MAX_PIPE_NUM];
static td_bool g_sc850sl_flip[OT_ISP_MAX_PIPE_NUM];

static td_void sc850sl_lock(cis_info *cis)
{
    if (pthread_mutex_lock(&cis->mutex) != 0) {
        isp_err_trace("sc850sl: mutex lock failed!\n");
    }
}

static td_void sc850sl_unlock(cis_info *cis)
{
    if (pthread_mutex_unlock(&cis->mutex) != 0) {
        isp_err_trace("sc850sl: mutex unlock failed!\n");
    }
}

/* Both tables leave 0x3221 at 0x66 -- mirrored and flipped -- which is how
 * the vendor's module is mounted, so that is "normal" here and the caller's
 * mirror and flip toggle away from it. The sensor moves its readout window
 * with these bits so the Bayer phase stays put: the XMedia SC850SL driver
 * reports BGGR in all four orientations. */
static td_s32 sc850sl_write_orientation_locked(cis_info *cis)
{
    ot_vi_pipe pipe = cis->pipe;
    td_u32 val = SC850SL_MIRROR_FLIP_DEFAULT;

    if (pipe < 0 || pipe >= OT_ISP_MAX_PIPE_NUM) {
        return TD_FAILURE;
    }
    if (g_sc850sl_mirror[pipe]) {
        val ^= SC850SL_MIRROR_BITS;
    }
    if (g_sc850sl_flip[pipe]) {
        val ^= SC850SL_FLIP_BITS;
    }
    return cis_write_reg(&cis->i2c, SC850SL_MIRROR_FLIP_ADDR, val);
}

td_s32 sc850sl_set_orientation(cis_info *cis, td_bool mirror, td_bool flip)
{
    td_s32 ret = TD_SUCCESS;

    sns_check_pointer_return(cis);
    if (cis->pipe < 0 || cis->pipe >= OT_ISP_MAX_PIPE_NUM) {
        return TD_FAILURE;
    }
    sc850sl_lock(cis);
    g_sc850sl_mirror[cis->pipe] = mirror;
    g_sc850sl_flip[cis->pipe] = flip;
    /* Before the first bring-up there is no bus yet; the bring-up applies it. */
    if (cis->i2c.fd >= 0) {
        ret = sc850sl_write_orientation_locked(cis);
    }
    sc850sl_unlock(cis);
    return ret;
}

td_void sc850sl_clear_orientation(ot_vi_pipe vi_pipe)
{
    if (vi_pipe < 0 || vi_pipe >= OT_ISP_MAX_PIPE_NUM) {
        return;
    }
    g_sc850sl_mirror[vi_pipe] = TD_FALSE;
    g_sc850sl_flip[vi_pipe] = TD_FALSE;
}

static td_s32 sc850sl_default_reg_init(cis_info *cis)
{
    td_u32 i;
    ot_isp_sns_state *s = cis->sns_state;

    for (i = 0; i < s->regs_info[0].reg_num; i++) {
        if (cis_write_reg(&cis->i2c, s->regs_info[0].i2c_data[i].reg_addr,
                          s->regs_info[0].i2c_data[i].data) != TD_SUCCESS) {
            isp_err_trace("sc850sl: write 0x%04x failed!\n", s->regs_info[0].i2c_data[i].reg_addr);
            return TD_FAILURE;
        }
    }
    return TD_SUCCESS;
}

static td_s32 sc850sl_reg_init(cis_info *cis, cis_reg_cfg *cfg, td_u32 len)
{
    td_u32 i;
    td_s32 ret = TD_FAILURE;

    sc850sl_lock(cis);
    if (cis_write_reg(&cis->i2c, 0x0103, 0x01) != TD_SUCCESS) { /* soft reset */
        goto out;
    }
    cis_delay_ms(1); /* 1ms */
    for (i = 0; i < len; i++) {
        if (cis_write_reg(&cis->i2c, cfg->addr, cfg->data) != TD_SUCCESS) {
            goto out;
        }
        cfg++;
    }
    if (sc850sl_default_reg_init(cis) != TD_SUCCESS) {
        goto out;
    }
    /* Before streaming, so the first frame already comes out the way it was asked. */
    if (sc850sl_write_orientation_locked(cis) != TD_SUCCESS) {
        isp_err_trace("sc850sl: writing the orientation failed!\n");
        goto out;
    }
    if (cis_write_reg(&cis->i2c, SC850SL_STREAM_ADDR, SC850SL_STREAM_ON) != TD_SUCCESS) {
        goto out;
    }
    ret = TD_SUCCESS;
out:
    sc850sl_unlock(cis);
    return ret;
}

td_s32 sc850sl_linear_8m30_12bit_init(cis_info *cis)
{
    td_s32 ret;
    td_u32 len;
    cis_reg_cfg *cfg = sc850sl_linear_8m30_12bit;

    sns_check_pointer_return(cis);

    len = (td_u32)(sizeof(sc850sl_linear_8m30_12bit) / sizeof(sc850sl_linear_8m30_12bit[0]));
    ret = sc850sl_reg_init(cis, cfg, len);
    if (ret != TD_SUCCESS) {
        isp_err_trace("sc850sl_reg_init failed!\n");
        return ret;
    }

    printf("========================================================================\n");
    printf("vi_pipe:%d,== SC850SL_MIPI_27MInput_4lane_1080Mbps_12bit_30fps_3840x2160 Init OK! ==\n", cis->pipe);
    printf("========================================================================\n");

    return TD_SUCCESS;
}

td_s32 sc850sl_vc_wdr_2t1_8m30_10bit_init(cis_info *cis)
{
    td_s32 ret;
    td_u32 len;
    cis_reg_cfg *cfg = sc850sl_wdr_2t1_8m30_10bit;

    sns_check_pointer_return(cis);

    len = (td_u32)(sizeof(sc850sl_wdr_2t1_8m30_10bit) / sizeof(sc850sl_wdr_2t1_8m30_10bit[0]));
    ret = sc850sl_reg_init(cis, cfg, len);
    if (ret != TD_SUCCESS) {
        isp_err_trace("sc850sl_reg_init failed!\n");
        return ret;
    }

    printf("============================================================================================\n");
    printf("vi_pipe:%d,== SC850SL_MIPI_27MInput_4lane_1458Mbps_10bit_WDR2T1_30fps_3840x2160 Init OK! ==\n", cis->pipe);
    printf("============================================================================================\n");

    return TD_SUCCESS;
}
