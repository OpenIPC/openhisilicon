/*
 * GC2063 1920x1080p30 linear RAW10 init, as the vendor's Hi3516CV610 firmware
 * (SIP-K662C6S, xipc) does it: the register table (gc2063_cfg.h), which ends
 * by turning the stream on, 20 ms, then the AE sync registers. The table is
 * byte for byte the Hi3516CV500 GC2053 driver's 1080p30 sequence.
 */
#include <pthread.h>
#include "sensor_common.h"
#include "gc2063_cmos.h"
#include "gc2063_cfg.h"
#include "gc2063_sensor_ctrl.h"

/* The orientation the caller asked for, per pipe. pfn_mirror_flip arrives on
 * the caller's thread and the bring-up runs on the ISP thread, so both hold
 * cis->mutex while they read this and write the sensor. Cleared at
 * unregister; the caller re-issues it from its own configuration. */
static td_bool g_gc2063_mirror[OT_ISP_MAX_PIPE_NUM];
static td_bool g_gc2063_flip[OT_ISP_MAX_PIPE_NUM];

static td_void gc2063_lock(cis_info *cis)
{
    if (pthread_mutex_lock(&cis->mutex) != 0) {
        isp_err_trace("gc2063: mutex lock failed!\n");
    }
}

static td_void gc2063_unlock(cis_info *cis)
{
    if (pthread_mutex_unlock(&cis->mutex) != 0) {
        isp_err_trace("gc2063: mutex unlock failed!\n");
    }
}

/* 0x17 on page 0, as both GC2053 references write it: the Hi3516CV500 driver
 * writes 0x80..0x83, the XMedia one sets bits 0 and 1. Neither moves the
 * window with it, and the XMedia driver reports one Bayer order for all four
 * orientations, so the phase is taken not to move. The page is selected first:
 * every AE register is on page 0 too, but nothing else here guarantees it. */
static td_s32 gc2063_write_orientation_locked(cis_info *cis)
{
    ot_vi_pipe pipe = cis->pipe;
    td_u32 val = GC2063_MIRROR_FLIP_BASE;
    td_s32 ret = TD_SUCCESS;

    if (pipe < 0 || pipe >= OT_ISP_MAX_PIPE_NUM) {
        return TD_FAILURE;
    }
    if (g_gc2063_mirror[pipe]) {
        val |= GC2063_MIRROR_BIT;
    }
    if (g_gc2063_flip[pipe]) {
        val |= GC2063_FLIP_BIT;
    }
    ret += cis_write_reg(&cis->i2c, GC2063_PAGE_ADDR, 0x00);
    ret += cis_write_reg(&cis->i2c, GC2063_MIRROR_FLIP_ADDR, val);
    return (ret == TD_SUCCESS) ? TD_SUCCESS : TD_FAILURE;
}

td_s32 gc2063_set_orientation(cis_info *cis, td_bool mirror, td_bool flip)
{
    td_s32 ret = TD_SUCCESS;

    sns_check_pointer_return(cis);
    if (cis->pipe < 0 || cis->pipe >= OT_ISP_MAX_PIPE_NUM) {
        return TD_FAILURE;
    }
    gc2063_lock(cis);
    g_gc2063_mirror[cis->pipe] = mirror;
    g_gc2063_flip[cis->pipe] = flip;
    /* Before the first bring-up there is no bus yet; the bring-up applies it. */
    if (cis->i2c.fd >= 0) {
        ret = gc2063_write_orientation_locked(cis);
    }
    gc2063_unlock(cis);
    return ret;
}

td_void gc2063_clear_orientation(ot_vi_pipe vi_pipe)
{
    if (vi_pipe < 0 || vi_pipe >= OT_ISP_MAX_PIPE_NUM) {
        return;
    }
    g_gc2063_mirror[vi_pipe] = TD_FALSE;
    g_gc2063_flip[vi_pipe] = TD_FALSE;
}

static td_s32 gc2063_default_reg_init(cis_info *cis)
{
    td_u32 i;
    ot_isp_sns_state *s = cis->sns_state;

    for (i = 0; i < s->regs_info[0].reg_num; i++) {
        if (cis_write_reg(&cis->i2c, s->regs_info[0].i2c_data[i].reg_addr,
                          s->regs_info[0].i2c_data[i].data) != TD_SUCCESS) {
            isp_err_trace("gc2063: write 0x%02x failed!\n", s->regs_info[0].i2c_data[i].reg_addr);
            return TD_FAILURE;
        }
    }
    return TD_SUCCESS;
}

td_s32 gc2063_linear_2m30_10bit_init(cis_info *cis)
{
    td_u32 i, len;
    td_s32 ret = TD_FAILURE;
    cis_reg_cfg *cfg = gc2063_linear_2m30_10bit;

    sns_check_pointer_return(cis);
    len = (td_u32)(sizeof(gc2063_linear_2m30_10bit) / sizeof(gc2063_linear_2m30_10bit[0]));

    gc2063_lock(cis);
    for (i = 0; i < len; i++) {
        if (cis_write_reg(&cis->i2c, cfg->addr, cfg->data) != TD_SUCCESS) {
            goto out;
        }
        cfg++;
    }
    cis_delay_ms(20); /* 20ms, as the vendor waits */
    /* The table leaves page 0 selected, where every sync register is. */
    if (gc2063_default_reg_init(cis) != TD_SUCCESS) {
        goto out;
    }
    if (gc2063_write_orientation_locked(cis) != TD_SUCCESS) {
        isp_err_trace("gc2063: writing the orientation failed!\n");
        goto out;
    }
    ret = TD_SUCCESS;
out:
    gc2063_unlock(cis);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    printf("=================================================================\n");
    printf("vi_pipe:%d,== gc2063 27Mclk 2M30fps(MIPI) 10bit linear init success! ==\n", cis->pipe);
    printf("=================================================================\n");
    return TD_SUCCESS;
}
