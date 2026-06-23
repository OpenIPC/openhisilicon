/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef ISP_HNR_EXT_CONFIG_H
#define ISP_HNR_EXT_CONFIG_H

#include "isp_vreg.h"
#include "ot_common.h"
#include "isp_ext_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

static __inline td_void ot_ext_system_aibnr_en_write(ot_vi_pipe vi_pipe, td_bool data)
{
    iowr_8direct(vreg_aiisp_base(vi_pipe), data);
}

static __inline td_bool ot_ext_system_aibnr_en_read(ot_vi_pipe vi_pipe)
{
    return (td_bool)(iord_8direct(vreg_aiisp_base(vi_pipe)) & 0x1);
}

static __inline td_void ot_ext_system_aibnr_normal_blend_status_write(ot_vi_pipe vi_pipe, td_bool data)
{
    iowr_8direct(vreg_aiisp_base(vi_pipe) + 0x4, data);
}

static __inline td_bool ot_ext_system_aibnr_normal_blend_status_read(ot_vi_pipe vi_pipe)
{
    return (td_bool)(iord_8direct(vreg_aiisp_base(vi_pipe) + 0x4) & 0x1);
}

static __inline td_void ot_ext_system_aidrc_en_write(ot_vi_pipe vi_pipe, td_bool data)
{
    iowr_8direct(vreg_aiisp_base(vi_pipe) + 0xc, data);
}

static __inline td_bool ot_ext_system_aidrc_en_read(ot_vi_pipe vi_pipe)
{
    return (td_bool)(iord_8direct(vreg_aiisp_base(vi_pipe) + 0xc) & 0x1);
}

static __inline td_void ot_ext_system_aidrc_mode_write(ot_vi_pipe vi_pipe, td_bool data)
{
    iowr_8direct(vreg_aiisp_base(vi_pipe) + 0x10, data);
}

static __inline td_bool ot_ext_system_aidrc_mode_read(ot_vi_pipe vi_pipe)
{
    return (td_bool)(iord_8direct(vreg_aiisp_base(vi_pipe) + 0x10) & 0x1);
}

static __inline td_void ot_ext_system_aidrc_dataflow_status_write(ot_vi_pipe vi_pipe, td_bool data)
{
    iowr_8direct(vreg_aiisp_base(vi_pipe) + 0x14, data);
}

static __inline td_bool ot_ext_system_aidrc_dataflow_status_read(ot_vi_pipe vi_pipe)
{
    return (td_bool)(iord_8direct(vreg_aiisp_base(vi_pipe) + 0x14) & 0x1);
}

static __inline td_void ot_ext_system_aidm_en_write(ot_vi_pipe vi_pipe, td_bool data)
{
    iowr_8direct(vreg_aiisp_base(vi_pipe) + 0x18, data);
}

static __inline td_bool ot_ext_system_aidm_en_read(ot_vi_pipe vi_pipe)
{
    return (td_bool)(iord_8direct(vreg_aiisp_base(vi_pipe) + 0x18) & 0x1);
}

static __inline td_void ot_ext_system_ai3dnr_en_write(ot_vi_pipe vi_pipe, td_bool data)
{
    iowr_8direct(vreg_aiisp_base(vi_pipe) + 0x1c, data);
}

static __inline td_bool ot_ext_system_ai3dnr_en_read(ot_vi_pipe vi_pipe)
{
    return (td_bool)(iord_8direct(vreg_aiisp_base(vi_pipe) + 0x1c) & 0x1);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
