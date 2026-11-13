/* IMX415 bring-up -- the xipc sequence: standby + XMSTA stop, the mode table
 * (0x3031 ADBIT, 0x3032 MDBIT: RAW10), the AE registers, the orientation,
 * standby cancel, 24 ms, master start. */
#include <pthread.h>
#include "sensor_common.h"
#include "imx415_cmos.h"
#include "imx415_cfg.h"
#include "imx415_sensor_ctrl.h"

/* The orientation the caller asked for, per pipe. pfn_mirror_flip arrives on
 * the caller's thread and the bring-up runs on the ISP thread, so both hold
 * cis->mutex while they read this and write the sensor; nothing else in the
 * common layer holds that lock across a sensor call. Cleared at unregister;
 * the caller re-issues it from its own configuration. */
static td_bool g_imx415_mirror[OT_ISP_MAX_PIPE_NUM];
static td_bool g_imx415_flip[OT_ISP_MAX_PIPE_NUM];

static td_void imx415_lock(cis_info *cis)
{
    if (pthread_mutex_lock(&cis->mutex) != 0) {
        isp_err_trace("imx415: mutex lock failed!\n");
    }
}

static td_void imx415_unlock(cis_info *cis)
{
    if (pthread_mutex_unlock(&cis->mutex) != 0) {
        isp_err_trace("imx415: mutex unlock failed!\n");
    }
}

/* HREVERSE and VREVERSE (0x3030 bits 0 and 1) are all it takes: inverted
 * readout starts one pixel further on in each direction (datasheet, "Normal
 * Operation and Inverted Operation"), so the Bayer phase does not move, in
 * all-pixel and in 2/2 binning alike. The register is always written, so an
 * orientation a previous session left behind cannot survive a clear. */
static td_s32 imx415_write_orientation_locked(cis_info *cis)
{
    ot_vi_pipe pipe = cis->pipe;
    td_u32 val;

    if (pipe < 0 || pipe >= OT_ISP_MAX_PIPE_NUM) {
        return TD_FAILURE;
    }
    val = (g_imx415_mirror[pipe] ? IMX415_HREVERSE_BIT : 0) |
          (g_imx415_flip[pipe] ? IMX415_VREVERSE_BIT : 0);
    return cis_write_reg(&cis->i2c, IMX415_REVERSE_ADDR, val);
}

td_s32 imx415_set_orientation(cis_info *cis, td_bool mirror, td_bool flip)
{
    td_s32 ret = TD_SUCCESS;

    sns_check_pointer_return(cis);
    if (cis->pipe < 0 || cis->pipe >= OT_ISP_MAX_PIPE_NUM) {
        return TD_FAILURE;
    }
    imx415_lock(cis);
    g_imx415_mirror[cis->pipe] = mirror;
    g_imx415_flip[cis->pipe] = flip;
    /* Before the first bring-up there is no bus yet; the bring-up applies it. */
    if (cis->i2c.fd >= 0) {
        ret = imx415_write_orientation_locked(cis);
    }
    imx415_unlock(cis);
    return ret;
}

td_void imx415_clear_orientation(ot_vi_pipe vi_pipe)
{
    if (vi_pipe < 0 || vi_pipe >= OT_ISP_MAX_PIPE_NUM) {
        return;
    }
    g_imx415_mirror[vi_pipe] = TD_FALSE;
    g_imx415_flip[vi_pipe] = TD_FALSE;
}

static td_s32 imx415_default_reg_init(cis_info *cis)
{
    td_u32 i;
    ot_isp_sns_state *s = cis->sns_state;
    for (i = 0; i < s->regs_info[0].reg_num; i++) {
        if (cis_write_reg(&cis->i2c, s->regs_info[0].i2c_data[i].reg_addr,
                          s->regs_info[0].i2c_data[i].data) != TD_SUCCESS) {
            isp_err_trace("imx415: write 0x%04x failed!\n", s->regs_info[0].i2c_data[i].reg_addr);
            return TD_FAILURE;
        }
    }
    return TD_SUCCESS;
}

static td_s32 imx415_reg_init(cis_info *cis, cis_reg_cfg *cfg, td_u32 len, const char *banner)
{
    td_u32 i;
    td_s32 ret = TD_FAILURE;

    imx415_lock(cis);
    if (cis_write_reg(&cis->i2c, 0x3000, 0x01) != TD_SUCCESS) { /* standby */
        goto out;
    }
    if (cis_write_reg(&cis->i2c, 0x3002, 0x01) != TD_SUCCESS) { /* XMSTA: master stop */
        goto out;
    }
    for (i = 0; i < len; i++) {
        if (cis_write_reg(&cis->i2c, cfg->addr, cfg->data) != TD_SUCCESS) {
            goto out;
        }
        cfg++;
    }
    if (imx415_default_reg_init(cis) != TD_SUCCESS) {
        goto out;
    }
    /* In standby, so the first frame already comes out the way it was asked. */
    if (imx415_write_orientation_locked(cis) != TD_SUCCESS) {
        isp_err_trace("imx415: writing the orientation failed!\n");
        goto out;
    }
    if (cis_write_reg(&cis->i2c, 0x3000, 0x00) != TD_SUCCESS) { /* standby cancel */
        goto out;
    }
    cis_delay_ms(24);                                           /* 24ms */
    if (cis_write_reg(&cis->i2c, 0x3002, 0x00) != TD_SUCCESS) { /* master mode start */
        goto out;
    }
    printf("===%s Init OK!===\n", banner);
    ret = TD_SUCCESS;
out:
    imx415_unlock(cis);
    return ret;
}

td_s32 imx415_linear_8m30_10bit_init(cis_info *cis)
{
    sns_check_pointer_return(cis);
    return imx415_reg_init(cis, imx415_linear_8m30_10bit,
        sizeof(imx415_linear_8m30_10bit) / sizeof(imx415_linear_8m30_10bit[0]),
        "IMX415 8M 30fps 10bit LINE");
}

td_s32 imx415_linear_2m_binning_30_10bit_init(cis_info *cis)
{
    sns_check_pointer_return(cis);
    return imx415_reg_init(cis, imx415_linear_2m_binning_30_10bit,
        sizeof(imx415_linear_2m_binning_30_10bit) / sizeof(imx415_linear_2m_binning_30_10bit[0]),
        "IMX415 2M binning 30fps 10bit LINE");
}
