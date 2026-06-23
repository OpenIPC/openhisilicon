/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef ISP_EXT_CONFIG_H
#define ISP_EXT_CONFIG_H

#include "isp_vreg.h"
#include "ot_common.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/* instance 'ext' of module 'ext_config' */
#define ot_ext_base_addr(vi_pipe) (isp_vir_reg_base(vi_pipe))

/* TOP: 0x0~0xFFF */
#define VREG_TOP_OFFSET     0x0
#define VREG_TOP_SIZE       0x1000
#define vreg_top_base(vi_pipe)  (ot_ext_base_addr(vi_pipe) + VREG_TOP_OFFSET)
/* SIZE < 0x100 */
/* BLC: 0x1000~0x107F */
#define VREG_BLC_OFFSET             (VREG_TOP_OFFSET + VREG_TOP_SIZE)
#define VREG_BLC_SIZE               0x80
#define vreg_blc_base(vi_pipe)      (ot_ext_base_addr(vi_pipe) + VREG_BLC_OFFSET)

/* RC: 0x1080~0x108F */
#define VREG_RC_OFFSET              (VREG_BLC_OFFSET + VREG_BLC_SIZE)
#define VREG_RC_SIZE                0x10
#define vreg_rc_base(vi_pipe)       (ot_ext_base_addr(vi_pipe) + VREG_RC_OFFSET)

/* CLUT:0x1090~0x10AF */
#define VREG_CLUT_OFFSET            (VREG_RC_OFFSET + VREG_RC_SIZE)
#define VREG_CLUT_SIZE              0x20
#define vreg_clut_base(vi_pipe)     (ot_ext_base_addr(vi_pipe) + VREG_CLUT_OFFSET)

/* FPN: 0x10B0~0x10DF */
#define VREG_FPN_OFFSET             (VREG_CLUT_OFFSET + VREG_CLUT_SIZE)
#define VREG_FPN_SIZE               0x30
#define vreg_fpn_base(vi_pipe)      (ot_ext_base_addr(vi_pipe) + VREG_FPN_OFFSET)

/* GCAC: 0x10E0~0x10FF */
#define VREG_GCAC_OFFSET            (VREG_FPN_OFFSET + VREG_FPN_SIZE)
#define VREG_GCAC_SIZE              0x20
#define vreg_gcac_base(vi_pipe)     (ot_ext_base_addr(vi_pipe) + VREG_GCAC_OFFSET)

/* FLICKER:  0x1100~0x111F */
#define VREG_FLICK_OFFSET           (VREG_GCAC_OFFSET + VREG_GCAC_SIZE)
#define VREG_FLICK_SIZE             0x20
#define vreg_flick_base(vi_pipe)    (ot_ext_base_addr(vi_pipe) + VREG_FLICK_OFFSET)

/* LCAC: 0x1120~0x117F */
#define VREG_LCAC_OFFSET            (VREG_FLICK_OFFSET + VREG_FLICK_SIZE)
#define VREG_LCAC_SIZE              0x60
#define vreg_lcac_base(vi_pipe)     (ot_ext_base_addr(vi_pipe) + VREG_LCAC_OFFSET)

/* FCR: 0x1180~0x11CF */
#define VREG_FCR_OFFSET             (VREG_LCAC_OFFSET + VREG_LCAC_SIZE)
#define VREG_FCR_SIZE               0x50
#define vreg_fcr_base(vi_pipe)      (ot_ext_base_addr(vi_pipe) + VREG_FCR_OFFSET)

/* DngInfo : 0x11D0 ~ 0x121F */
#define VREG_DNGINFO_OFFSET        (VREG_FCR_OFFSET + VREG_FCR_SIZE)
#define VREG_DNGINFO_SIZE          0x50
#define vreg_dnginfo_base(vi_pipe) (ot_ext_base_addr(vi_pipe) + VREG_DNGINFO_OFFSET)

/* GE:  0x1220~0x129F */
#define VREG_GE_OFFSET              (VREG_DNGINFO_OFFSET + VREG_DNGINFO_SIZE)
#define VREG_GE_SIZE                0x80
#define vreg_ge_base(vi_pipe)       (ot_ext_base_addr(vi_pipe) + VREG_GE_OFFSET)

/* csc: 0x12A0~0x12FF */
#define VREG_CSC_OFFSET             (VREG_GE_OFFSET + VREG_GE_SIZE)
#define VREG_CSC_SIZE               0x60
#define vreg_csc_base(vi_pipe)      (ot_ext_base_addr(vi_pipe) + VREG_CSC_OFFSET)

/* SIZE <:= 0x500 */
/* WDR: 0x1300~0x15FF */
#define VREG_WDR_OFFSET         (VREG_CSC_OFFSET + VREG_CSC_SIZE)
#define VREG_WDR_SIZE           0x300
#define vreg_wdr_base(vi_pipe)  (ot_ext_base_addr(vi_pipe) + VREG_WDR_OFFSET)

/* LDCI: 0x1600~0x16FF */
#define VREG_LDCI_OFFSET        (VREG_WDR_OFFSET + VREG_WDR_SIZE)
#define VREG_LDCI_SIZE          0x100
#define vreg_ldci_base(vi_pipe) (ot_ext_base_addr(vi_pipe) + VREG_LDCI_OFFSET)

/* DM: 0x1700~0x17FF */
#define VREG_DM_OFFSET          (VREG_LDCI_OFFSET + VREG_LDCI_SIZE)
#define VREG_DM_SIZE            0x100
#define vreg_dm_base(vi_pipe)   (ot_ext_base_addr(vi_pipe) + VREG_DM_OFFSET)

/* AWB:0x1800~0x18FF */
#define VREG_AWB_OFFSET         (VREG_DM_OFFSET + VREG_DM_SIZE)
#define VREG_AWB_SIZE           0x100
#define vreg_awb_base(vi_pipe)  (ot_ext_base_addr(vi_pipe) + VREG_AWB_OFFSET)

/* AF:0x1900~0x19FF */
#define VREG_AF_OFFSET          (VREG_AWB_OFFSET + VREG_AWB_SIZE)
#define VREG_AF_SIZE            0x100
#define vreg_af_base(vi_pipe)   (ot_ext_base_addr(vi_pipe) + VREG_AF_OFFSET)

/* AE:0x1A00~0x1BFF */
#define VREG_AE_OFFSET          (VREG_AF_OFFSET + VREG_AF_SIZE)
#define VREG_AE_SIZE            0x200
#define vreg_ae_base(vi_pipe)   (ot_ext_base_addr(vi_pipe) + VREG_AE_OFFSET)

/* ACAC:0x1C00~0x1DFF */
#define VREG_ACAC_OFFSET            (VREG_AE_OFFSET + VREG_AE_SIZE)
#define VREG_ACAC_SIZE              0x200
#define vreg_acac_base(vi_pipe)     (ot_ext_base_addr(vi_pipe) + VREG_ACAC_OFFSET)

/* BSHP:0x1E00~0x3DFF */
#define VREG_BSHP_OFFSET              (VREG_ACAC_OFFSET + VREG_ACAC_SIZE)
#define VREG_BSHP_SIZE                0x2000
#define vreg_bshp_base(vi_pipe)       (ot_ext_base_addr(vi_pipe) + VREG_BSHP_OFFSET)

/* DEHAZE:  0x3E00~0x3FFF */
#define VREG_DEHAZE_OFFSET          (VREG_BSHP_OFFSET + VREG_BSHP_SIZE)
#define VREG_DEHAZE_SIZE            0x200
#define vreg_dehaze_base(vi_pipe)   (ot_ext_base_addr(vi_pipe) + VREG_DEHAZE_OFFSET)

/* DRC: 0x4000~0x42FF */
#define VREG_DRC_OFFSET             (VREG_DEHAZE_OFFSET + VREG_DEHAZE_SIZE)
#define VREG_DRC_SIZE               0x300
#define vreg_drc_base(vi_pipe)      (ot_ext_base_addr(vi_pipe) + VREG_DRC_OFFSET)

/* BNR: 0x4300~0x47FF */
#define VREG_BNR_OFFSET             (VREG_DRC_OFFSET + VREG_DRC_SIZE)
#define VREG_BNR_SIZE               0x500
#define vreg_bnr_base(vi_pipe)      (ot_ext_base_addr(vi_pipe) + VREG_BNR_OFFSET)

/* PreGamma:0x4800~0x4CFF */
#define VREG_PREGAMMA_OFFSET        (VREG_BNR_OFFSET + VREG_BNR_SIZE)
#define VREG_PREGAMMA_SIZE          0x500
#define vreg_pregamma_base(vi_pipe) (ot_ext_base_addr(vi_pipe) + VREG_PREGAMMA_OFFSET)

/* SIZE < 0x1000 */
/* expander: 0x4D00 ~ 0x53FF */
#define VREG_EXPANDER_OFFSET        (VREG_PREGAMMA_OFFSET + VREG_PREGAMMA_SIZE)
#define VREG_EXPANDER_SIZE           0x700
#define vreg_expander_base(vi_pipe)  (ot_ext_base_addr(vi_pipe) + VREG_EXPANDER_OFFSET)

/* CA:0x5400~0x60FF */
#define VREG_CA_OFFSET              (VREG_EXPANDER_OFFSET + VREG_EXPANDER_SIZE)
#define VREG_CA_SIZE                0xD00
#define vreg_ca_base(vi_pipe)       (ot_ext_base_addr(vi_pipe) + VREG_CA_OFFSET)

/* Gamma:0x6100~0x69FF */
#define VREG_GAMMA_OFFSET           (VREG_CA_OFFSET + VREG_CA_SIZE)
#define VREG_GAMMA_SIZE             0x900
#define vreg_gamma_base(vi_pipe)    (ot_ext_base_addr(vi_pipe) + VREG_GAMMA_OFFSET)

/* vreg_crb_base */
/* CRB:0x6A00~0x72FF */
#define VREG_CRB_OFFSET              (VREG_GAMMA_OFFSET + VREG_GAMMA_SIZE)
#define VREG_CRB_SIZE                0x900
#define vreg_crb_base(vi_pipe)       (ot_ext_base_addr(vi_pipe) + VREG_CRB_OFFSET)

/* SIZE >:= 0x1000 */
/* SHARPEN: 0x7300~0x88FF */
#define VREG_SHARPEN_OFFSET         (VREG_CRB_OFFSET + VREG_CRB_SIZE)
#define VREG_SHARPEN_SIZE           0x1600
#define vreg_sharpen_base(vi_pipe)  (ot_ext_base_addr(vi_pipe) + VREG_SHARPEN_OFFSET)

/* LSC:  0x8900~0xD8FF */
#define VREG_LSC_OFFSET             (VREG_SHARPEN_OFFSET + VREG_SHARPEN_SIZE)
#define VREG_LSC_SIZE               0x5000
#define vreg_lsc_base(vi_pipe)      (ot_ext_base_addr(vi_pipe) + VREG_LSC_OFFSET)

/* SIZE >:= 0x10000 */
/* DPC:0xD900~0x199FF */
#define VREG_DPC_OFFSET             (VREG_LSC_OFFSET + VREG_LSC_SIZE)
#ifdef CONFIG_OT_ISP_DPC_STATIC_TABLE_SUPPORT
#define VREG_DPC_SIZE               0xC100
#else
#define VREG_DPC_SIZE               0x100
#endif
#define vreg_dpc_base(vi_pipe)       (ot_ext_base_addr(vi_pipe) + VREG_DPC_OFFSET)

/* RGBIR:0x19A00~0x19AFF */
#define VREG_RGBIR_OFFSET             (VREG_DPC_OFFSET + VREG_DPC_SIZE)
#define VREG_RGBIR_SIZE               0x100
#define vreg_rgbir_base(vi_pipe)      (ot_ext_base_addr(vi_pipe) + VREG_RGBIR_OFFSET)

/* AI Color:0x19B00~0x19B2F */
#define VREG_AI_COLOR_OFFSET          (VREG_RGBIR_OFFSET + VREG_RGBIR_SIZE)
#define VREG_AI_COLOR_SIZE            0x30
#define vreg_ai_color_base(vi_pipe)   (ot_ext_base_addr(vi_pipe) + VREG_AI_COLOR_OFFSET)

/* AI Sharpeness:0x19B30~0x19B7F */
#define VREG_AI_SHARPEN_OFFSET          (VREG_AI_COLOR_OFFSET + VREG_AI_COLOR_SIZE)
#define VREG_AI_SHARPEN_SIZE            0x50
#define vreg_ai_sharpen_base(vi_pipe)   (ot_ext_base_addr(vi_pipe) + VREG_AI_SHARPEN_OFFSET)

/* AI Noise:0x19B80~0x19B9F */
#define VREG_AI_NOISE_OFFSET          (VREG_AI_SHARPEN_OFFSET + VREG_AI_SHARPEN_SIZE)
#define VREG_AI_NOISE_SIZE            0x20
#define vreg_ai_noise_base(vi_pipe)   (ot_ext_base_addr(vi_pipe) + VREG_AI_NOISE_OFFSET)

/* AI Bright:0x19BA0~0x19BCF */
#define VREG_AI_BRIGHT_OFFSET          (VREG_AI_NOISE_OFFSET + VREG_AI_NOISE_SIZE)
#define VREG_AI_BRIGHT_SIZE            0x30
#define vreg_ai_bright_base(vi_pipe)   (ot_ext_base_addr(vi_pipe) + VREG_AI_BRIGHT_OFFSET)

/* AI Bright:0x19BD0~0x19BEF */
#define VREG_AI_CONTRAST_OFFSET          (VREG_AI_BRIGHT_OFFSET + VREG_AI_BRIGHT_SIZE)
#define VREG_AI_CONTRAST_SIZE            0x20
#define vreg_ai_contrast_base(vi_pipe)   (ot_ext_base_addr(vi_pipe) + VREG_AI_CONTRAST_OFFSET)

/* AI Group:0x19BF0~0x19BFF */
#define VREG_AI_GROUP_OFFSET          (VREG_AI_CONTRAST_OFFSET + VREG_AI_CONTRAST_SIZE)
#define VREG_AI_GROUP_SIZE            0x10
#define vreg_ai_group_base(vi_pipe)   (ot_ext_base_addr(vi_pipe) + VREG_AI_GROUP_OFFSET)

/* AGamma:0x19C00~0x19C1F */
#define VREG_AGAMMA_OFFSET          (VREG_AI_GROUP_OFFSET + VREG_AI_GROUP_SIZE)
#define VREG_AGAMMA_SIZE            0x20
#define vreg_agamma_base(vi_pipe)   (ot_ext_base_addr(vi_pipe) + VREG_AGAMMA_OFFSET)

/* AIISP:0x19C20~0x19C7F */
#define VREG_AIISP_OFFSET          (VREG_AGAMMA_OFFSET + VREG_AGAMMA_SIZE)
#define VREG_AIISP_SIZE            0x60
#define vreg_aiisp_base(vi_pipe)   (ot_ext_base_addr(vi_pipe) + VREG_AIISP_OFFSET)

/* Csp:0x19C80~0x1A57F */
#define VREG_CSP_OFFSET             (VREG_AIISP_OFFSET + VREG_AIISP_SIZE)
#define VREG_CSP_SIZE               0x900
#define vreg_csp_base(vi_pipe)      (ot_ext_base_addr(vi_pipe) + VREG_CSP_OFFSET)

/* LBLC:0x1A580~0x1C87F */
#define VREG_LBLC_OFFSET             (VREG_CSP_OFFSET + VREG_CSP_SIZE)
#define VREG_LBLC_SIZE               0x2300
#define vreg_lblc_base(vi_pipe)      (ot_ext_base_addr(vi_pipe) + VREG_LBLC_OFFSET)

/* CAC: 0x1C880~0x1CA7F */
#define VREG_CAC_OFFSET            (VREG_LBLC_OFFSET + VREG_LBLC_SIZE)
#define VREG_CAC_SIZE              0x200
#define vreg_cac_base(vi_pipe)     (ot_ext_base_addr(vi_pipe) + VREG_CAC_OFFSET)

/* BNR noise:0x1CA80~0x1CAFF */
#define VREG_SNS_OFFSET          (VREG_CAC_OFFSET + VREG_CAC_SIZE)
#define VREG_SNS_SIZE            0x80
#define vreg_sns_base(vi_pipe)   (ot_ext_base_addr(vi_pipe) + VREG_SNS_OFFSET)

/* group: top */
/* register: ot_ext_top_wdr_cfg */
/* TD_TRUE: the state of WDR config is initialized; */
/* TD_FALSE: the state of WDR config is NOT initialized */
/* args: data (1-bit) */
static __inline td_void ot_ext_top_wdr_cfg_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0x0000, (data & 0x01));
}
static __inline td_u8 ot_ext_top_wdr_cfg_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_top_base(vi_pipe) + 0x0000) & 0x01);
}

/* register: ot_ext_top_pub_attr_cfg */
/* TD_TRUE: the state of pub_attr config is initialized; */
/* TD_FALSE: the state of pub_attr config is NOT initialized */
/* args: data (1-bit) */
static __inline td_void ot_ext_top_pub_attr_cfg_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0x0002, (data & 0x01));
}
static __inline td_u8 ot_ext_top_pub_attr_cfg_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_top_base(vi_pipe) + 0x0002) & 0x01);
}

/* register: ot_ext_top_wdr_switch */
/* TD_TRUE: WDR switch is complete; */
/* TD_FALSE: WDR switch is NOT complete */
/* args: data (1-bit) */
static __inline td_void ot_ext_top_wdr_switch_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0x0004, (data & 0x01));
}
static __inline td_u8 ot_ext_top_wdr_switch_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_top_base(vi_pipe) + 0x0004) & 0x01);
}

/* register: ot_ext_top_res_switch */
/* TD_TRUE: resolution switch is complete; */
/* TD_FALSE: resolution switch is NOT complete */
/* args: data (1-bit) */
static __inline td_void ot_ext_top_res_switch_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0x0006, (data & 0x01));
}
static __inline td_u8 ot_ext_top_res_switch_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_top_base(vi_pipe) + 0x0006) & 0x01);
}

/* register: ot_ext_top_sensor_width */
/* args: data (32-bit) */
static __inline td_void ot_ext_top_sensor_width_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x0008, data);
}
static __inline td_u32 ot_ext_top_sensor_width_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x0008);
}

/* register: ot_ext_top_sensor_width */
/* args: data (32-bit) */
static __inline td_void ot_ext_top_sensor_height_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x000c, data);
}
static __inline td_u32 ot_ext_top_sensor_height_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x000c);
}

/* args: data (32-bit) */
static __inline td_void ot_ext_system_fps_base_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x0010, data);
}
static __inline td_u32 ot_ext_system_fps_base_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x0010);
}

/* register: ot_ext_system_sensor_max_resolution_width */
/* width of sensor resolution */
/* args: data (16-bit) */
static __inline td_void ot_ext_system_sensor_max_resolution_width_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x0014, data);
}

static __inline td_u16 ot_ext_system_sensor_max_resolution_width_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x0014);
}

/* register: ot_ext_system_sensor_max_resolution_height */
/* height of sensor resolution */
/* args: data (16-bit) */
static __inline td_void ot_ext_system_sensor_max_resolution_height_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x0016, data);
}

static __inline td_u16 ot_ext_system_sensor_max_resolution_height_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x0016);
}

/* register: ot_ext_system_sys_debug_enable */
/* 0:disable system debug information */
/* 1:enable system debug information */
#define OT_EXT_SYSTEM_SYS_DEBUG_ENABLE_DEFAULT  0x0

/* args: data (1-bit) */
static __inline td_void ot_ext_system_sys_debug_enable_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    td_u16 current = iord_16direct(vreg_top_base(vi_pipe) + 0x0018);
    iowr_16direct(vreg_top_base(vi_pipe) + 0x0018, (((td_u16)data) << 0) | (current & 0xFFFE));
}

static __inline td_u16 ot_ext_system_sys_debug_enable_read(ot_vi_pipe vi_pipe)
{
    return (td_u16)((iord_16direct(vreg_top_base(vi_pipe) + 0x0018) & 0x0001) >> 0);
}

/* register: ot_ext_system_sys_debug_high_addr */
/* address of system debug information */
#define OT_EXT_SYSTEM_SYS_DEBUG_HIGH_ADDR_DEFAULT  0x0

/* args: data (32-bit) */
static __inline td_void ot_ext_system_sys_debug_high_addr_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x001c, data);
}

static __inline td_u32 ot_ext_system_sys_debug_high_addr_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x001c);
}

/* register: ot_ext_system_sys_debug_low_addr */
/* address of system debug information */
#define OT_EXT_SYSTEM_SYS_DEBUG_LOW_ADDR_DEFAULT  0x0

/* args: data (32-bit) */
static __inline td_void ot_ext_system_sys_debug_low_addr_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x0020, data);
}

static __inline td_u32 ot_ext_system_sys_debug_low_addr_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x0020);
}

/* register: ot_ext_system_sys_debug_size */
/* size of sys debug struct */
#define OT_EXT_SYSTEM_SYS_DEBUG_SIZE_DEFAULT  0x0

/* args: data (32-bit) */
static __inline td_void ot_ext_system_sys_debug_size_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x0024, data);
}

static __inline td_u32 ot_ext_system_sys_debug_size_read(ot_vi_pipe vi_pipe)
{
    return (td_u32)iord_32direct(vreg_top_base(vi_pipe) + 0x0024);
}

/* register: ot_ext_system_debug_depth */
/* the depth of all debug information */
#define OT_EXT_SYSTEM_DEBUG_DEPTH_DEFAULT  0x0
static __inline td_void ot_ext_system_sys_debug_depth_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x0028, data);
}

static __inline td_u32 ot_ext_system_sys_debug_depth_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x0028);
}

/* register: ot_ext_system_statistics_ctrl_lowbit */
/* ISP statistics control low bit */
#define OT_EXT_SYSTEM_STATISTICS_LOWBIT_DEFAULT  (ISP_STATISTICS_KEY)

/* args: data (32-bit) */
static __inline td_u32 ot_ext_system_statistics_ctrl_lowbit_read(ot_vi_pipe vi_pipe)
{
    return  iord_32direct(vreg_top_base(vi_pipe) + 0x002c);
}

static __inline td_void ot_ext_system_statistics_ctrl_lowbit_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x002c, data);
}

/* register: ot_ext_system_statistics_ctrl_highbit */
/* ISP statistics control high bit */
#define OT_EXT_SYSTEM_STATISTICS_HIGHBIT_DEFAULT  (ISP_STATISTICS_KEY)

/* args: data (32-bit) */
static __inline td_u32 ot_ext_system_statistics_ctrl_highbit_read(ot_vi_pipe vi_pipe)
{
    return  iord_32direct(vreg_top_base(vi_pipe) + 0x0030);
}

static __inline td_void ot_ext_system_statistics_ctrl_highbit_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x0030, data);
}

/* register: ot_ext_system_bind_attr */
/* isp bind attr include ae_lib awb_lib sensor_id etc */
/* |----------------------------------------------------------------| */
/* |           RSV          |     AE_LIB_ID    |   AWB_LIB_ID       | */
/* |----------------------------------------------------------------| */
/* |<--------16bits--------><-------8bits------><------8bits------->| */
#define OT_EXT_SYSTEM_BIND_ATTR_DEFAULT  0x0

/* args: data (32-bit) */
static __inline td_u32 ot_ext_system_bind_attr_read(ot_vi_pipe vi_pipe)
{
    return  iord_32direct(vreg_top_base(vi_pipe) + 0x0034);
}
static __inline td_void ot_ext_system_bind_attr_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x0034, data);
}

/* register: ot_ext_system_freeze_firmware */
/* disables firmware and stops updating the ISP */
#define OT_EXT_SYSTEM_FREEZE_FIRMWARE_DEFAULT  0

/* args: data (1-bit) */
static __inline td_void ot_ext_system_freeze_firmware_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    td_u8 current = iord_8direct(vreg_top_base(vi_pipe) + 0x0038);
    iowr_8direct(vreg_top_base(vi_pipe) + 0x0038, (data & 0x01) | (current & 0xFE));
}
static __inline td_u8 ot_ext_system_freeze_firmware_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_top_base(vi_pipe) + 0x0038) & 0x01);
}

/* register: ot_ext_system_sensor_wdr_mode */
/* WDR mode: 0=linear 1=built-in wdr 2=2to1 full frame wdr  ... */
#define OT_EXT_SYSTEM_SENSOR_WDR_MODE_DEFAULT   0x0

/* args: data (6-bit) */
static __inline td_void ot_ext_system_sensor_wdr_mode_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    td_u16 current = iord_16direct(vreg_top_base(vi_pipe) + 0x003a);
    iowr_16direct(vreg_top_base(vi_pipe) + 0x003a, (((td_u16)(data & 0x3F)) << 4) | (current & 0xFC0F)); /* shift 4 */
}
static __inline td_u8 ot_ext_system_sensor_wdr_mode_read(ot_vi_pipe vi_pipe)
{
    return (td_u8)((iord_16direct(vreg_top_base(vi_pipe) + 0x003a) & 0x03F0) >> 4);    /* right shift 4 */
}

/* register: ot_ext_sync_total_width */
/* total frame width including horizontal blanking */
#define OT_EXT_SYNC_TOTAL_WIDTH_DEFAULT  0x672
/* args: data (16-bit) */
static __inline td_void ot_ext_sync_total_width_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x3c, data);
}
static __inline td_u16 ot_ext_sync_total_width_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x3c);
}

/* register: ot_ext_sync_total_height */
/* total frame height including vertical blanking */
#define OT_EXT_SYNC_TOTAL_HEIGHT_DEFAULT  0x2EE

/* args: data (16-bit) */
static __inline td_void ot_ext_sync_total_height_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x40, data);
}
static __inline td_u16 ot_ext_sync_total_height_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x40);
}

static __inline td_void ot_ext_system_be_total_width_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x44, data);
}
static __inline td_u16 ot_ext_system_be_total_width_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x44);
}

static __inline td_void ot_ext_system_be_total_height_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x46, data);
}
static __inline td_u16 ot_ext_system_be_total_height_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x46);
}

static __inline td_void ot_ext_system_sys_debug_type_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x48, data);
}

static __inline td_u32 ot_ext_system_sys_debug_type_read(ot_vi_pipe vi_pipe)
{
    return (td_u32)iord_32direct(vreg_top_base(vi_pipe) + 0x48);
}

/* register: ot_ext_system_sys_debug_enable */
/* 0:disable system ae debug information */
/* 1:enable system ae debug information */
#define OT_EXT_SYSTEM_SYS_AE_DEBUG_ENABLE_DEFAULT  0x0

/* args: data (1-bit) */
static __inline td_void ot_ext_system_sys_ae_debug_enable_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    td_u16 current = iord_16direct(vreg_top_base(vi_pipe) + 0x004c);
    iowr_16direct(vreg_top_base(vi_pipe) + 0x004c, (((td_u16)data) << 0) | (current & 0xFFFE));
}

static __inline td_u16 ot_ext_system_sys_ae_debug_enable_read(ot_vi_pipe vi_pipe)
{
    return (td_u16)((iord_16direct(vreg_top_base(vi_pipe) + 0x004c) & 0x0001) >> 0);
}

/* register: ot_ext_system_sys_debug_high_addr */
/* address of system ae debug information */
#define OT_EXT_SYSTEM_SYS_AE_DEBUG_HIGH_ADDR_DEFAULT  0x0

/* args: data (32-bit) */
static __inline td_void ot_ext_system_sys_ae_debug_high_addr_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x0050, data);
}

static __inline td_u32 ot_ext_system_sys_ae_debug_high_addr_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x0050);
}

/* register: ot_ext_system_sys_debug_low_addr */
/* address of system ae debug information */
#define OT_EXT_SYSTEM_SYS_AE_DEBUG_LOW_ADDR_DEFAULT  0x0

/* args: data (32-bit) */
static __inline td_void ot_ext_system_sys_ae_debug_low_addr_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x0054, data);
}

static __inline td_u32 ot_ext_system_sys_ae_debug_low_addr_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x0054);
}

/* register: ot_ext_system_debug_depth */
/* the depth of all ae debug information */
#define OT_EXT_SYSTEM_AE_DEBUG_DEPTH_DEFAULT  0x0
static __inline td_void ot_ext_system_sys_ae_debug_depth_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x0058, data);
}

static __inline td_u32 ot_ext_system_sys_ae_debug_depth_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x0058);
}

/* register: ot_ext_system_sys_debug_enable */
/* 0:disable system awb debug information */
/* 1:enable system awb debug information */
#define OT_EXT_SYSTEM_SYS_AWB_DEBUG_ENABLE_DEFAULT  0x0

/* args: data (1-bit) */
static __inline td_void ot_ext_system_sys_awb_debug_enable_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    td_u16 current = iord_16direct(vreg_top_base(vi_pipe) + 0x005c);
    iowr_16direct(vreg_top_base(vi_pipe) + 0x005c, (((td_u16)data) << 0) | (current & 0xFFFE));
}

static __inline td_u16 ot_ext_system_sys_awb_debug_enable_read(ot_vi_pipe vi_pipe)
{
    return (td_u16)((iord_16direct(vreg_top_base(vi_pipe) + 0x005c) & 0x0001) >> 0);
}

/* register: ot_ext_system_sys_debug_high_addr */
/* address of system awb debug information */
#define OT_EXT_SYSTEM_SYS_AWB_DEBUG_HIGH_ADDR_DEFAULT  0x0

/* args: data (32-bit) */
static __inline td_void ot_ext_system_sys_awb_debug_high_addr_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x0060, data);
}

static __inline td_u32 ot_ext_system_sys_awb_debug_high_addr_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x0060);
}

/* register: ot_ext_system_sys_debug_low_addr */
/* address of system awb debug information */
#define OT_EXT_SYSTEM_SYS_AWB_DEBUG_LOW_ADDR_DEFAULT  0x0

/* args: data (32-bit) */
static __inline td_void ot_ext_system_sys_awb_debug_low_addr_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x0064, data);
}

static __inline td_u32 ot_ext_system_sys_awb_debug_low_addr_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x0064);
}

/* register: ot_ext_system_debug_depth */
/* the depth of all awb debug information */
#define OT_EXT_SYSTEM_DEBUG_AWB_DEPTH_DEFAULT  0x0
static __inline td_void ot_ext_system_sys_awb_debug_depth_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x0068, data);
}

static __inline td_u32 ot_ext_system_sys_awb_debug_depth_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x0068);
}

/* register: ot_ext_system_sys_debug_enable */
/* 0:disable system dehaze debug information */
/* 1:enable system dehaze debug information */
#define OT_EXT_SYSTEM_SYS_DEHAZE_DEBUG_ENABLE_DEFAULT  0x0

/* args: data (1-bit) */
static __inline td_void ot_ext_system_sys_dehaze_debug_enable_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    td_u16 current = iord_16direct(vreg_top_base(vi_pipe) + 0x006c);
    iowr_16direct(vreg_top_base(vi_pipe) + 0x006c, (((td_u16)data) << 0) | (current & 0xFFFE));
}

static __inline td_u16 ot_ext_system_sys_dehaze_debug_enable_read(ot_vi_pipe vi_pipe)
{
    return (td_u16)((iord_16direct(vreg_top_base(vi_pipe) + 0x006c) & 0x0001) >> 0);
}

/* register: ot_ext_system_sys_debug_high_addr */
/* address of system dehaze debug information */
#define OT_EXT_SYSTEM_SYS_DEHAZE_DEBUG_HIGH_ADDR_DEFAULT  0x0

/* args: data (32-bit) */
static __inline td_void ot_ext_system_sys_dehaze_debug_high_addr_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x0070, data);
}

static __inline td_u32 ot_ext_system_sys_dehaze_debug_high_addr_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x0070);
}

/* register: ot_ext_system_sys_debug_low_addr */
/* address of system dehaze debug information */
#define OT_EXT_SYSTEM_SYS_DEHAZE_DEBUG_LOW_ADDR_DEFAULT  0x0

/* args: data (32-bit) */
static __inline td_void ot_ext_system_sys_dehaze_debug_low_addr_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x0074, data);
}

static __inline td_u32 ot_ext_system_sys_dehaze_debug_low_addr_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x0074);
}

/* register: ot_ext_system_debug_depth */
/* the depth of all dehaze debug information */
#define OT_EXT_SYSTEM_DEHAZE_DEBUG_DEPTH_DEFAULT  0x0
static __inline td_void ot_ext_system_sys_dehaze_debug_depth_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x0078, data);
}

static __inline td_u32 ot_ext_system_sys_dehaze_debug_depth_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x0078);
}

/* register: ot_ext_system_debug_update */
/* 0:disable system update debug information */
/* 1:enable system update debug information */
#define OT_EXT_SYSTEM_DEBUG_UPDATE_DEFAULT  0x0
static __inline td_void ot_ext_system_sys_debug_update_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x007c, data);
}

static __inline td_u32 ot_ext_system_sys_debug_update_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x007c);
}
#define OT_EXT_SYSTEM_BINARY_EN_DEFAULT  0x0
static __inline td_void ot_ext_system_sys_binary_en_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0x0080, data);
}

static __inline td_u8 ot_ext_system_sys_binary_en_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_top_base(vi_pipe) + 0x0080);
}
#define OT_EXT_SYSTEM_CHECK_SUM_EN_DEFAULT  0x0
static __inline td_void ot_ext_system_sys_check_sum_en_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0x0081, data);
}

static __inline td_u8 ot_ext_system_sys_check_sum_en_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_top_base(vi_pipe) + 0x0081);
}

static __inline td_void ot_ext_system_update_info_high_phyaddr_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct((vreg_top_base(vi_pipe) + 0xa4), data);
}

static __inline td_u32 ot_ext_system_update_info_high_phyaddr_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0xa4);
}

static __inline td_void ot_ext_system_update_info_low_phyaddr_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct((vreg_top_base(vi_pipe) + 0xa8), data);
}

static __inline td_u32 ot_ext_system_update_info_low_phyaddr_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0xa8);
}

static __inline td_void ot_ext_system_sys_iso_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct((vreg_top_base(vi_pipe) + 0xac), data);
}

static __inline td_u32 ot_ext_system_sys_iso_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0xac);
}
/* register: ot_ext_system_gamma_curve_type */
/* the type of gamma curve */
#define OT_EXT_SYSTEM_GAMMA_CURVE_TYPE_DEFAULT  0

/* args: data (8-bit) */
static __inline td_void ot_ext_system_gamma_curve_type_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0xc8, data);
}
static __inline td_u8 ot_ext_system_gamma_curve_type_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_top_base(vi_pipe) + 0xc8);
}

/* register: isp_crop_pos_x_write */
static __inline td_void ot_ext_system_corp_pos_x_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0xd4, data);
}
static __inline td_u16 ot_ext_system_corp_pos_x_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0xd4);
}

/* register: isp_crop_pos_y_write */
static __inline td_void ot_ext_system_corp_pos_y_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0xd6, data);
}
static __inline td_u16 ot_ext_system_corp_pos_y_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0xd6);
}

/* register: ot_ext_system_isp_bit_bypass */
#define OT_EXT_SYS_ISP_BYPASS_DEFAULT   0

static __inline td_void ot_ext_system_isp_bit_bypass_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0xd8, data);
}

static __inline td_u32 ot_ext_system_isp_bit_bypass_read(ot_vi_pipe vi_pipe)
{
    return  iord_32direct(vreg_top_base(vi_pipe) + 0xd8);
}

#define OT_EXT_SYSTEM_TOP_BIT_RESERVE0_WRITE_DEFAULT  0

static __inline td_void ot_ext_system_top_bit_reserve0_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0xe0, data & 0xF);
}

static __inline td_u8 ot_ext_system_top_bit_reserve0_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_top_base(vi_pipe) + 0xe0) & 0xF);
}

#define OT_EXT_SYSTEM_TOP_BIT_RESERVE1_WRITE_DEFAULT  0

static __inline td_void ot_ext_system_top_bit_reserve1_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0xe1, data & 0x3);
}

static __inline td_u8 ot_ext_system_top_bit_reserve1_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_top_base(vi_pipe) + 0xe1) & 0x3);
}

#define OT_EXT_SYSTEM_TOP_BIT_RESERVE2_WRITE_DEFAULT  0
static __inline td_void ot_ext_system_top_bit_reserve2_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0xe2, data & 0x3);
}

static __inline td_u8 ot_ext_system_top_bit_reserve2_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_top_base(vi_pipe) + 0xe2) & 0x3);
}

#define OT_EXT_SYSTEM_TOP_BIT_RESERVE3_WRITE_DEFAULT  0
static __inline td_void ot_ext_system_top_bit_reserve3_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0xe3, data & 0x3);
}

static __inline td_u8 ot_ext_system_top_bit_reserve3_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_top_base(vi_pipe) + 0xe3) & 0x3);
}

#define OT_EXT_SYSTEM_TOP_BIT_RESERVE4_WRITE_DEFAULT   0
static __inline td_void ot_ext_system_top_bit_reserve4_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0xe4, data & 0x7);
}

static __inline td_u8 ot_ext_system_top_bit_reserve4_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_top_base(vi_pipe) + 0xe4) & 0x7);
}

static __inline td_void ot_ext_system_yuv_format_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0xec, data);
}

static __inline td_u32 ot_ext_system_yuv_format_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0xec);
}

/* register: ot_ext_system_rggb_cfg_write */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_rggb_cfg_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0xf0, (data & 0xff));
}
static __inline td_u8 ot_ext_system_rggb_cfg_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_top_base(vi_pipe) + 0xf0) & 0xff);
}

/* register: ot_ext_system_sensor_mode */
/* used to distinguish the special sensor mode */
#define OT_EXT_SYSTEM_SENSOR_MODE_DEFAULT  0x0

/* args: data (1-bit) */
static __inline td_void ot_ext_system_sensor_mode_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0x00f2, (data & 0xff));
}
static __inline td_u8 ot_ext_system_sensor_mode_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_top_base(vi_pipe) + 0x00f2) & 0xff);
}

/* register: ot_ext_system_isp_dgain_bypass */
/*  */
#define OT_EXT_SYS_ISP_DGAIN_ENABLE_DEFAULT  0x0
#define OT_EXT_SYS_ISP_DGAIN_ENABLE_DATASIZE 1

static __inline td_void ot_ext_system_isp_dgain_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0x00f4, (data & 0x01));
}

static __inline td_u8 ot_ext_system_isp_dgain_enable_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_top_base(vi_pipe) + 0x00f4) & 0x01);
}

/* register: ot_ext_system_be_free_buffer_high_addr */
#define OT_EXT_SYSTEM_BE_FREE_BUFFER_HIGH_ADDR_DEFAULT  0x0

/* args: data (32-bit) */
static __inline td_void ot_ext_system_be_free_buffer_high_addr_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x0100, data);
}

static __inline td_u32 ot_ext_system_be_free_buffer_high_addr_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x0100);
}

/* register: ot_ext_system_be_free_buffer_low_addr */
#define OT_EXT_SYSTEM_BE_FREE_BUFFER_LOW_ADDR_DEFAULT   0x0

/* args: data (32-bit) */
static __inline td_void ot_ext_system_be_free_buffer_low_addr_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x0104, data);
}

static __inline td_u32 ot_ext_system_be_free_buffer_low_addr_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x0104);
}

/* register: ot_ext_system_manual_sharpen_overshoot_amt */
/* sharpening overshoot amt */
#define OT_EXT_SYSTEM_ACTUAL_SHARPEN_OVERSHOOT_DEFAULT                  150

/* args: data (8-bit) */
static __inline td_void ot_ext_system_actual_sharpen_overshoot_amt_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0x0108, data);
}
static __inline td_u8 ot_ext_system_actual_sharpen_overshoot_amt_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_top_base(vi_pipe) + 0x0108);
}

/* register: ot_ext_system_manual_sharpen_undershoot_amt_write */
/* sharpening undershoot amt */
#define OT_EXT_SYSTEM_ACTUAL_SHARPEN_UNDERSHOOT_DEFAULT            200
/* args: data (8-bit) */
static __inline td_void ot_ext_system_actual_sharpen_undershoot_amt_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0x010a, data);
}
static __inline td_u8 ot_ext_system_actual_sharpen_undershoot_amt_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_top_base(vi_pipe) + 0x010a);
}

#define OT_EXT_SYSTEM_ACTUAL_SHARPEN_SHOOTSUPST_DEFAULT            1

static __inline td_void ot_ext_system_actual_sharpen_shoot_sup_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0x010c, data);
}

static __inline td_u8 ot_ext_system_actual_sharpen_shoot_sup_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_top_base(vi_pipe) + 0x010c);
}

static  __inline td_void ot_ext_system_actual_sharpen_edge_frequence_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x010e, data);
}

static __inline td_u16 ot_ext_system_actual_sharpen_edge_frequence_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x010e);
}

static  __inline td_void ot_ext_system_actual_sharpen_texture_frequence_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x0110, data);
}

static __inline td_u16 ot_ext_system_actual_sharpen_texture_frequence_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x0110);
}
/* addr range:sharpen [0x120, 0x160), index:32(OT_ISP_SHARPEN_GAIN_NUM) */
static __inline td_void ot_ext_system_actual_sharpen_edge_str_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x0120 + index * sizeof(td_u16), data);
}

static __inline td_u16 ot_ext_system_actual_sharpen_edge_str_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x0120 + index * sizeof(td_u16));
}

/* addr range:sharpen [0x160, 0x180), index:32(OT_ISP_SHARPEN_GAIN_NUM) */
static __inline td_void ot_ext_system_actual_sharpen_texture_str_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x0160 + index * sizeof(td_u16), data);
}

static __inline td_u16 ot_ext_system_actual_sharpen_texture_str_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x0160 + index * sizeof(td_u16));
}

#define OT_EXT_SYSTEM_BAYERNR_ACTUAL_STRENGTH_DEFAULT  100

/* args: data (8-bit) */
static __inline td_void ot_ext_system_bayernr_actual_fine_strength_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0x01A0, data);
}
static __inline td_u8 ot_ext_system_bayernr_actual_fine_strength_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_top_base(vi_pipe) + 0x01A0);
}

#define  OT_EXT_SYSTEM_BAYERNR_ACTUAL_CORING_WEIGHT_DEFAULT 800
static __inline td_void ot_ext_system_bayernr_actual_coring_weight_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x01A2, data);
}

static __inline td_u16 ot_ext_system_bayernr_actual_coring_weight_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x01A2);
}

static __inline td_void ot_ext_system_bayernr_actual_nr_lsc_ratio_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0x01A4, data);
}
static __inline td_u8 ot_ext_system_bayernr_actual_nr_lsc_ratio_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_top_base(vi_pipe) + 0x01A4);
}

/* args: data (8-bit) */
static __inline td_void ot_ext_system_bayernr_actual_chroma_strength_write(ot_vi_pipe vi_pipe, td_u8 i, td_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0x01A8 + (i * sizeof(td_u8)), data);
}
static __inline td_u8 ot_ext_system_bayernr_actual_chroma_strength_read(ot_vi_pipe vi_pipe, td_u8 i)
{
    return iord_8direct(vreg_top_base(vi_pipe) + 0x01A8 + (i * sizeof(td_u8)));
}

/* args: data (8-bit) */
static __inline td_void ot_ext_system_bayernr_actual_wdr_frame_strength_write(ot_vi_pipe vi_pipe, td_u8 i, td_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0x01AC + (i * sizeof(td_u8)), data);
}
static __inline td_u8 ot_ext_system_bayernr_actual_wdr_frame_strength_read(ot_vi_pipe vi_pipe, td_u8 i)
{
    return iord_8direct(vreg_top_base(vi_pipe) + 0x01AC + (i * sizeof(td_u8)));
}

/* args: data (16-bit) */
static __inline td_void ot_ext_system_bayernr_actual_coarse_strength_write(ot_vi_pipe vi_pipe, td_u8 i, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x0200 + (i * sizeof(td_u16)), data);
}
static __inline td_u16 ot_ext_system_bayernr_actual_coarse_strength_read(ot_vi_pipe vi_pipe, td_u8 i)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x0200 + (i * sizeof(td_u16)));
}

static __inline td_void  ot_ext_system_dehaze_actual_strength_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x0208, data);
}
static __inline td_u16  ot_ext_system_dehaze_actual_strength_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x0208);
}

static __inline td_void  ot_ext_system_drc_actual_strength_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x020A, data);
}
static __inline td_u16  ot_ext_system_drc_actual_strength_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x020A);
}

static __inline td_void  ot_ext_system_actual_wdr_exposure_ratio_write(ot_vi_pipe vi_pipe, td_u8 i, td_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x0210 + (i * sizeof(td_u32)), data);
}
static __inline td_u32  ot_ext_system_actual_wdr_exposure_ratio_read(ot_vi_pipe vi_pipe, td_u8 i)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x0210 + (i * sizeof(td_u32)));
}

static __inline td_void ot_ext_system_block_num_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0x220, (data & 0xff));
}
static __inline td_u8 ot_ext_system_block_num_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_top_base(vi_pipe) + 0x220) & 0xff);
}

#define OT_EXT_SYSTEM_TOP_CHANNEL_SELECT_WRITE_DEFAULT  0

static __inline td_void ot_ext_system_top_channel_select_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0x224, data & 0x3);
}

static __inline td_u8 ot_ext_system_top_channel_select_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_top_base(vi_pipe) + 0x224) & 0x3);
}

#define OT_EXT_SYSTEM_TOP_CHANNEL_SELECT_PRE_WRITE_DEFAULT  0

static __inline td_void ot_ext_system_top_channel_select_pre_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0x226, data & 0x3);
}

static __inline td_u8 ot_ext_system_top_channel_select_pre_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_top_base(vi_pipe) + 0x226) & 0x3);
}
#define OT_EXT_SYSTEM_ISP_PIPE_DIFF_TYPE_DEFAULT  0x0 // 0: Auto 1:Manual

static __inline td_void ot_ext_system_isp_pipe_diff_type_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0x228, data & 0x1);
}

static __inline td_u8 ot_ext_system_isp_pipe_diff_type_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_top_base(vi_pipe) + 0x228) & 0x1);
}

#define OT_EXT_SYSTEM_ISP_PIPE_DIFF_OFFSET_DEFAULT  0x0

/* args: data (32-bit) */
/* addr range TOP: [0x230,0x240), index:4 */
static __inline td_void ot_ext_system_isp_pipe_diff_offset_write(ot_vi_pipe vi_pipe, td_u8 index, td_s32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x230 + (index * sizeof(td_s32)), data);
}
static __inline td_s32 ot_ext_system_isp_pipe_diff_offset_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x230 + (index * sizeof(td_s32)));
}

/* register: ot_ext_system_isp_pipe_diff_gain */
#define OT_EXT_SYSTEM_ISP_PIPE_DIFF_GAIN_DEFAULT  0x100

/* args: data (32-bit) */
/* addr range TOP: [0x240,0x250), index:4 */
static __inline td_void ot_ext_system_isp_pipe_diff_gain_write(ot_vi_pipe vi_pipe, td_u8 index, td_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x240 + (index * sizeof(td_u32)), data);
}
static __inline td_u32 ot_ext_system_isp_pipe_diff_gain_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x240 + (index * sizeof(td_u32)));
}

/* register: ot_ext_system_isp_matrix_coefft */
#define OT_EXT_SYSTEM_ISP_PIPE_DIFF_CCM_DEFAULT  0x100

/* args: data (16-bit) */
/* addr range TOP: [0x250,0x262), index:9 */
static __inline td_void ot_ext_system_isp_pipe_diff_ccm_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x250 + (index << 1), data);
}
static __inline td_u16 ot_ext_system_isp_pipe_diff_ccm_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x250 + (index << 1));
}

#define OT_EXT_SYSTEM_ISP_PIPE_DIFF_OVERLAP_DEFAULT  0x6 // Range:[0x1, 0x10]

/* args: data (u8-bit) */
/* addr range TOP: [0x1,0x10), index:9 */
static __inline td_void ot_ext_system_isp_pipe_diff_overlap_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0x264, data);
}

static __inline td_u8 ot_ext_system_isp_pipe_diff_overlap_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_top_base(vi_pipe) + 0x264);
}


/* register: ot_ext_system_black_level_query */
/* R channel black level */
static __inline td_void ot_ext_system_black_level_query_f0_r_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x270, data);
}

static __inline td_u16 ot_ext_system_black_level_query_f0_r_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x270);
}

/* gr channel black level */
static __inline td_void ot_ext_system_black_level_query_f0_gr_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x272, data);
}

static __inline td_u16 ot_ext_system_black_level_query_f0_gr_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x272);
}

/* gb channel black level */
static __inline td_void ot_ext_system_black_level_query_f0_gb_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x274, data);
}

static __inline td_u16 ot_ext_system_black_level_query_f0_gb_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x274);
}

/* B channel black level */
static __inline td_void ot_ext_system_black_level_query_f0_b_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x276, data);
}

static __inline td_u16 ot_ext_system_black_level_query_f0_b_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x276);
}

/* register: ot_ext_system_black_level_query */
/* R channel black level */
static __inline td_void ot_ext_system_black_level_query_f1_r_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x278, data);
}

static __inline td_u16 ot_ext_system_black_level_query_f1_r_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x278);
}
/* gr channel black level */
static __inline td_void ot_ext_system_black_level_query_f1_gr_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x27a, data);
}

static __inline td_u16 ot_ext_system_black_level_query_f1_gr_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x27a);
}

/* gb channel black level */
static __inline td_void ot_ext_system_black_level_query_f1_gb_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x27c, data);
}

static __inline td_u16 ot_ext_system_black_level_query_f1_gb_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x27c);
}

/* B channel black level */
static __inline td_void ot_ext_system_black_level_query_f1_b_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x27e, data);
}

static __inline td_u16 ot_ext_system_black_level_query_f1_b_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x27e);
}

/* R channel black level */
static __inline td_void ot_ext_system_black_level_query_f2_r_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x280, data);
}

static __inline td_u16 ot_ext_system_black_level_query_f2_r_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x280);
}
/* gr channel black level */
static __inline td_void ot_ext_system_black_level_query_f2_gr_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x282, data);
}

static __inline td_u16 ot_ext_system_black_level_query_f2_gr_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x282);
}

/* gb channel black level */
static __inline td_void ot_ext_system_black_level_query_f2_gb_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x284, data);
}

static __inline td_u16 ot_ext_system_black_level_query_f2_gb_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x284);
}

/* B channel black level */
static __inline td_void ot_ext_system_black_level_query_f2_b_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x286, data);
}

static __inline td_u16 ot_ext_system_black_level_query_f2_b_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x286);
}

/* register: ot_ext_system_black_level_query */
/* R channel black level */
static __inline td_void ot_ext_system_black_level_query_f3_r_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x288, data);
}

static __inline td_u16 ot_ext_system_black_level_query_f3_r_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x288);
}
/* gr channel black level */
static __inline td_void ot_ext_system_black_level_query_f3_gr_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x28a, data);
}

static __inline td_u16 ot_ext_system_black_level_query_f3_gr_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x28a);
}

/* gb channel black level */
static __inline td_void ot_ext_system_black_level_query_f3_gb_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x28c, data);
}

static __inline td_u16 ot_ext_system_black_level_query_f3_gb_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x28c);
}

/* B channel black level */
static __inline td_void ot_ext_system_black_level_query_f3_b_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x28e, data);
}

static __inline td_u16 ot_ext_system_black_level_query_f3_b_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x28e);
}

/* register: ot_ext_system_be_free_buffer_high_addr */
#define OT_EXT_SYSTEM_BE_LUT_STT_BUFFER_HIGH_ADDR_DEFAULT  0x0

/* args: data (32-bit) */
static __inline td_void ot_ext_system_be_lut_stt_buffer_high_addr_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x290, data);
}

static __inline td_u32 ot_ext_system_be_lut_stt_buffer_high_addr_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x290);
}

/* register: ot_ext_system_be_free_buffer_low_addr */
#define OT_EXT_SYSTEM_BE_LUT_STT_BUFFER_LOW_ADDR_DEFAULT  0x0

/* args: data (32-bit) */
static __inline td_void ot_ext_system_be_lut_stt_buffer_low_addr_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x294, data);
}

static __inline td_u32 ot_ext_system_be_lut_stt_buffer_low_addr_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x294);
}

/* register: ot_ext_system_be_buffer_address_high */
/* args: data (32-bit) */
static __inline td_void ot_ext_system_be_buffer_address_high_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x02a0, data);
}

static __inline td_u32 ot_ext_system_be_buffer_address_high_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x02a0);
}

/* register: ot_ext_system_be_buffer_address_low */
/* args: data (32-bit) */
static __inline td_void ot_ext_system_be_buffer_address_low_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x02a4, data);
}

static __inline td_u32 ot_ext_system_be_buffer_address_low_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x02a4);
}

/* register: ot_ext_system_bebuf_size */
/* args: data (32-bit) */
static __inline td_void ot_ext_system_be_buffer_size_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x02a8, data);
}

static __inline td_u32 ot_ext_system_be_buffer_size_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x02a8);
}

/* register: ot_ext_system_ldci_read_stt_buffer_high_addr */
#define OT_EXT_SYSTEM_LDCI_READ_STT_BUF_HIGH_ADDR_DEFAULT 0x0
/* args: data (32-bit) */
static __inline td_void ot_ext_system_ldci_read_stt_buffer_high_addr_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x2ac, data);
}

static __inline td_u32 ot_ext_system_ldci_read_stt_buffer_high_addr_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x2ac);
}

/* register: ot_ext_system_ldci_read_stt_buffer_low_addr */
#define OT_EXT_SYSTEM_LDCI_READ_STT_BUF_LOW_ADDR_DEFAULT 0x0

/* args: data (32-bit) */
static __inline td_void ot_ext_system_ldci_read_stt_buffer_low_addr_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x2b0, data);
}

static __inline td_u32 ot_ext_system_ldci_read_stt_buffer_low_addr_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x2b0);
}

static __inline td_void ot_ext_system_isp_dump_frame_pos_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x2b4, data);
}

static __inline td_u32 ot_ext_system_isp_dump_frame_pos_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x2b4);
}

/* register: sensor_flip_enable_write */
static __inline td_void ot_sensor_flip_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0x2b8, data);
}

static __inline td_u8 ot_sensor_flip_enable_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_top_base(vi_pipe) + 0x2b8);
}

/* register: sensor_mirror_enable_write */
static __inline td_void ot_sensor_mirror_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0x2ba, data);
}

static __inline td_u8 ot_sensor_mirror_enable_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_top_base(vi_pipe) + 0x2ba);
}

/* register: mipi_crop_enable_write */
static __inline td_void ot_mipi_crop_enable_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x2bc, data);
}

static __inline td_u16 ot_mipi_crop_enable_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x2bc);
}

/* register: mipi_crop_pos_x_write */
static __inline td_void ot_mipi_crop_pos_x_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x2be, data);
}

static __inline td_u16 ot_mipi_crop_pos_x_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x2be);
}

/* register: mipi_crop_pos_y_write */
static __inline td_void ot_mipi_crop_pos_y_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x2c0, data);
}
static __inline td_u16 ot_mipi_crop_pos_y_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x2c0);
}

/* register: mipi_crop_width_write */
static __inline td_void ot_mipi_crop_width_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x2c2, data);
}

static __inline td_u16 ot_mipi_crop_width_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x2c2);
}

/* register: mipi_crop_height_write */
static __inline td_void ot_mipi_crop_height_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x2c4, data);
}

static __inline td_u16 ot_mipi_crop_height_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x2c4);
}
/* addr range:[0x2c6, 0x2ce), index:4(OT_ISP_BAYER_CHN_NUM) */
static __inline td_void ot_ext_system_sns_black_level_query_f0_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct((vreg_top_base(vi_pipe) + 0x2c6 + sizeof(td_u16) * index), data);
}

static __inline td_u16 ot_ext_system_sns_black_level_query_f0_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_16direct((vreg_top_base(vi_pipe) + 0x2c6 + sizeof(td_u16) * index)));
}
/* addr range:[0x2ce, 0x2d6), index:4(OT_ISP_BAYER_CHN_NUM) */
static __inline td_void ot_ext_system_sns_black_level_query_f1_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct((vreg_top_base(vi_pipe) + 0x2ce + sizeof(td_u16) * index), data);
}

static __inline td_u16 ot_ext_system_sns_black_level_query_f1_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_16direct((vreg_top_base(vi_pipe) + 0x2ce + sizeof(td_u16) * index)));
}
/* addr range:[0x2d6, 0x2de), index:4(OT_ISP_BAYER_CHN_NUM) */
static __inline td_void ot_ext_system_sns_black_level_query_f2_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct((vreg_top_base(vi_pipe) + 0x2d6 + sizeof(td_u16) * index), data);
}

static __inline td_u16 ot_ext_system_sns_black_level_query_f2_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_16direct((vreg_top_base(vi_pipe) + 0x2d6 + sizeof(td_u16) * index)));
}

/* addr range:[0x2de, 0x2e6), index:4(OT_ISP_BAYER_CHN_NUM) */
static __inline td_void ot_ext_system_sns_black_level_query_f3_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct((vreg_top_base(vi_pipe) + 0x2de + sizeof(td_u16) * index), data);
}

static __inline td_u16 ot_ext_system_sns_black_level_query_f3_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_16direct((vreg_top_base(vi_pipe) + 0x2de + sizeof(td_u16) * index)));
}

/* addr range:[0x2e6, 0x2ee), index:4(OT_ISP_BAYER_CHN_NUM) */
static __inline td_void ot_ext_system_user_black_level_f0_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct((vreg_top_base(vi_pipe) + 0x2e6 + sizeof(td_u16) * index), data);
}

static __inline td_u16 ot_ext_system_user_black_level_f0_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_16direct((vreg_top_base(vi_pipe) + 0x2e6 + sizeof(td_u16) * index)));
}
/* addr range:[0x2ee, 0x2f6), index:4(OT_ISP_BAYER_CHN_NUM) */
static __inline td_void ot_ext_system_user_black_level_f1_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct((vreg_top_base(vi_pipe) + 0x2ee + sizeof(td_u16) * index), data);
}

static __inline td_u16 ot_ext_system_user_black_level_f1_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_16direct((vreg_top_base(vi_pipe) + 0x2ee + sizeof(td_u16) * index)));
}

/* addr range:[0x2f6, 0x2fe), index:4(OT_ISP_BAYER_CHN_NUM) */
static __inline td_void ot_ext_system_user_black_level_f2_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct((vreg_top_base(vi_pipe) + 0x2f6 + sizeof(td_u16) * index), data);
}

static __inline td_u16 ot_ext_system_user_black_level_f2_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_16direct((vreg_top_base(vi_pipe) + 0x2f6 + sizeof(td_u16) * index)));
}
/* addr range:[0x2fe, 0x306), index:4(OT_ISP_BAYER_CHN_NUM) */
static __inline td_void ot_ext_system_user_black_level_f3_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct((vreg_top_base(vi_pipe) + 0x2fe + sizeof(td_u16) * index), data);
}

static __inline td_u16 ot_ext_system_user_black_level_f3_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_16direct((vreg_top_base(vi_pipe) + 0x2fe + sizeof(td_u16) * index)));
}

/* fswdr mode */
static __inline td_void ot_ext_system_fswdr_mode_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x308, data);
}

static __inline td_u16 ot_ext_system_fswdr_mode_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_top_base(vi_pipe) + 0x308));
}

#ifdef CONFIG_OT_ISP_DUMP_DEBUG_SUPPORT
static __inline td_void ot_ext_system_isp_send_dbg_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x30a, data);
}

static __inline td_u16 ot_ext_system_isp_send_dbg_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x30a);
}
static __inline td_void ot_ext_system_isp_dump_dbg_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x30c, data);
}

static __inline td_u16 ot_ext_system_isp_dump_dbg_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x30c);
}
#endif
static __inline td_void ot_ext_system_fe_lut_stt_buffer_high_addr_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x310, data);
}

static __inline td_u32 ot_ext_system_fe_lut_stt_buffer_high_addr_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x310);
}

static __inline td_void ot_ext_system_fe_lut_stt_buffer_low_addr_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x314, data);
}

static __inline td_u32 ot_ext_system_fe_lut_stt_buffer_low_addr_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x314);
}

static __inline td_void ot_ext_system_fe_roi_en_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0x318, data);
}
static __inline td_u8 ot_ext_system_fe_roi_en_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_top_base(vi_pipe) + 0x318);
}

static __inline td_void ot_ext_system_fe_roi_x_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x31a, data);
}

static __inline td_u16 ot_ext_system_fe_roi_x_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_top_base(vi_pipe) + 0x31a));
}

static __inline td_void ot_ext_system_fe_roi_y_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x31c, data);
}

static __inline td_u16 ot_ext_system_fe_roi_y_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_top_base(vi_pipe) + 0x31c));
}

static __inline td_void ot_ext_system_fe_roi_width_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x31e, data);
}

static __inline td_u16 ot_ext_system_fe_roi_width_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_top_base(vi_pipe) + 0x31e));
}

static __inline td_void ot_ext_system_fe_roi_height_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x320, data);
}

static __inline td_u16 ot_ext_system_fe_roi_height_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_top_base(vi_pipe) + 0x320));
}

/* addr range:[0x322, 0x32A), index:4(OT_ISP_BAYER_CHN_NUM) */
static __inline td_void ot_ext_system_ori_black_level_query_f0_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct((vreg_top_base(vi_pipe) + 0x322 + sizeof(td_u16) * index), data);
}

static __inline td_u16 ot_ext_system_ori_black_level_query_f0_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_16direct((vreg_top_base(vi_pipe) + 0x322 + sizeof(td_u16) * index)));
}

/* addr range:[0x32A, 0x332), index:4(OT_ISP_BAYER_CHN_NUM) */
static __inline td_void ot_ext_system_ori_black_level_query_f1_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct((vreg_top_base(vi_pipe) + 0x32A + sizeof(td_u16) * index), data);
}

static __inline td_u16 ot_ext_system_ori_black_level_query_f1_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_16direct((vreg_top_base(vi_pipe) + 0x32A + sizeof(td_u16) * index)));
}

/* addr range:[0x332, 0x33A), index:4(OT_ISP_BAYER_CHN_NUM) */
static __inline td_void ot_ext_system_ori_black_level_query_f2_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct((vreg_top_base(vi_pipe) + 0x332 + sizeof(td_u16) * index), data);
}

static __inline td_u16 ot_ext_system_ori_black_level_query_f2_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_16direct((vreg_top_base(vi_pipe) + 0x332 + sizeof(td_u16) * index)));
}

/* addr range:[0x33A, 0x342), index:4(OT_ISP_BAYER_CHN_NUM) */
static __inline td_void ot_ext_system_ori_black_level_query_f3_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct((vreg_top_base(vi_pipe) + 0x33A + sizeof(td_u16) * index), data);
}

static __inline td_u16 ot_ext_system_ori_black_level_query_f3_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_16direct((vreg_top_base(vi_pipe) + 0x33A + sizeof(td_u16) * index)));
}

/* register: ot_ext_af_enable */
/* used to */
/* args: data (1-bit) */
static __inline td_void ot_ext_af_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0000, (data & 0x01));
}
static __inline td_u8 ot_ext_af_enable_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0000) & 0x01);
}

static __inline td_void ot_ext_af_iir0_enable0_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0001, (data & 0x01));
}
static __inline td_u8 ot_ext_af_iir0_enable0_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0001) & 0x01);
}

static __inline td_void ot_ext_af_iir0_enable1_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0002, (data & 0x01));
}
static __inline td_u8 ot_ext_af_iir0_enable1_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0002) & 0x01);
}

static __inline td_void ot_ext_af_iir0_enable2_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0003, (data & 0x01));
}
static __inline td_u8 ot_ext_af_iir0_enable2_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0003) & 0x01);
}

static __inline td_void ot_ext_af_iir1_enable0_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0004, (data & 0x01));
}
static __inline td_u8 ot_ext_af_iir1_enable0_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0004) & 0x01);
}

static __inline td_void ot_ext_af_iir1_enable1_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0005, (data & 0x01));
}
static __inline td_u8 ot_ext_af_iir1_enable1_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0005) & 0x01);
}

static __inline td_void ot_ext_af_iir1_enable2_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0006, (data & 0x01));
}
static __inline td_u8 ot_ext_af_iir1_enable2_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0006) & 0x01);
}

static __inline td_void ot_ext_af_peakmode_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0007, (data & 0x01));
}
static __inline td_u8 ot_ext_af_peakmode_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0007) & 0x01);
}

static __inline td_void ot_ext_af_squmode_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0008, (data & 0x01));
}
static __inline td_u8 ot_ext_af_squmode_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0008) & 0x01);
}

static __inline td_void ot_ext_af_offset_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0009, (data & 0x01));
}
static __inline td_u8 ot_ext_af_offset_enable_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0009) & 0x01);
}

static __inline td_void ot_ext_af_crop_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x000a, (data & 0x01));
}
static __inline td_u8 ot_ext_af_crop_enable_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x000a) & 0x01);
}

static __inline td_void ot_ext_af_lpf_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x000b, (data & 0x01));
}
static __inline td_u8 ot_ext_af_lpf_enable_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x000b) & 0x01);
}

static __inline td_void ot_ext_af_mean_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x000c, (data & 0x01));
}
static __inline td_u8 ot_ext_af_mean_enable_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x000c) & 0x01);
}

static __inline td_void ot_ext_af_rawmode_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x000d, (data & 0x01));
}
static __inline td_u8 ot_ext_af_rawmode_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x000d) & 0x01);
}

static __inline td_void ot_ext_af_iir0_ds_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x000e, (data & 0x01));
}
static __inline td_u8 ot_ext_af_iir0_ds_enable_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x000e) & 0x01);
}

static __inline td_void ot_ext_af_iir1_ds_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x000f, (data & 0x01));
}
static __inline td_u8 ot_ext_af_iir1_ds_enable_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x000f) & 0x01);
}

static __inline td_void ot_ext_af_fir0_lpf_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0010, (data & 0x01));
}
static __inline td_u8 ot_ext_af_fir0_lpf_enable_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0010) & 0x01);
}

static __inline td_void ot_ext_af_fir1_lpf_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0011, (data & 0x01));
}
static __inline td_u8 ot_ext_af_fir1_lpf_enable_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0011) & 0x01);
}

static __inline td_void ot_ext_af_iir0_ldg_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0012, (data & 0x01));
}
static __inline td_u8 ot_ext_af_iir0_ldg_enable_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0012) & 0x01);
}

static __inline td_void ot_ext_af_iir1_ldg_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0013, (data & 0x01));
}
static __inline td_u8 ot_ext_af_iir1_ldg_enable_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0013) & 0x01);
}

static __inline td_void ot_ext_af_fir0_ldg_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0014, (data & 0x01));
}
static __inline td_u8 ot_ext_af_fir0_ldg_enable_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0014) & 0x01);
}

static __inline td_void ot_ext_af_fir1_ldg_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0015, (data & 0x01));
}
static __inline td_u8 ot_ext_af_fir1_ldg_enable_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0015) & 0x01);
}

static __inline td_void ot_ext_af_updatemode_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0016, (data & 0x01));
}
static __inline td_u8 ot_ext_af_updatemode_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0016) & 0x01);
}

static __inline td_void ot_ext_af_update_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0017, (data & 0x01));
}
static __inline td_u8 ot_ext_af_update_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0017) & 0x01);
}

/* 8bit */
static __inline td_void ot_ext_af_bayermode_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0018, (data & 0xff));
}
static __inline td_u8 ot_ext_af_bayermode_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0018) & 0xff);
}

static __inline td_void ot_ext_af_gamma_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0019, (data & 0xff));
}
static __inline td_u8 ot_ext_af_gamma_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0019) & 0xff);
}

static __inline td_void ot_ext_af_gain_limit_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x001a, (data & 0xff));
}
static __inline td_u8 ot_ext_af_gain_limit_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x001a) & 0xff);
}

static __inline td_void ot_ext_af_iir_plg_group0_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x001b, (data & 0xff));
}
static __inline td_u8 ot_ext_af_iir_plg_group0_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x001b) & 0xff);
}

static __inline td_void ot_ext_af_iir_pls_group0_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x001c, (data & 0xff));
}
static __inline td_u8 ot_ext_af_iir_pls_group0_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x001c) & 0xff);
}

static __inline td_void ot_ext_af_iir_plg_group1_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x001d, (data & 0xff));
}
static __inline td_u8 ot_ext_af_iir_plg_group1_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x001d) & 0xff);
}

static __inline td_void ot_ext_af_iir_pls_group1_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x001e, (data & 0xff));
}
static __inline td_u8 ot_ext_af_iir_pls_group1_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x001e) & 0xff);
}

static __inline td_void ot_ext_af_cnt_shift0_h_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x001f, (data & 0xff));
}
static __inline td_u8 ot_ext_af_cnt_shift0_h_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x001f) & 0xff);
}

static __inline td_void ot_ext_af_cnt_shift1_h_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0020, (data & 0xff));
}
static __inline td_u8 ot_ext_af_cnt_shift1_h_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0020) & 0xff);
}

static __inline td_void ot_ext_af_cnt_shift0_v_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0021, (data & 0xff));
}
static __inline td_u8 ot_ext_af_cnt_shift0_v_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0021) & 0xff);
}

static __inline td_void ot_ext_af_cnt_shift_y_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0022, (data & 0xff));
}
static __inline td_u8 ot_ext_af_cnt_shift_y_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0022) & 0xff);
}

static __inline td_void ot_ext_af_iir_dilate0_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0023, (data & 0xff));
}
static __inline td_u8 ot_ext_af_iir_dilate0_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0023) & 0xff);
}

static __inline td_void ot_ext_af_iir_dilate1_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0024, (data & 0xff));
}
static __inline td_u8 ot_ext_af_iir_dilate1_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0024) & 0xff);
}

static __inline td_void ot_ext_af_highlight_thre_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0025, (data & 0xff));
}
static __inline td_u8 ot_ext_af_highlight_thre_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0025) & 0xff);
}

static __inline td_void ot_ext_af_pos_sel_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0026, (data & 0xff));
}
static __inline td_u8 ot_ext_af_pos_sel_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0026) & 0xff);
}

#define OT_EXT_AF_SET_FLAG_DISABLE 0x0
#define OT_EXT_AF_SET_FLAG_ENABLE  0x1
static __inline td_void ot_ext_af_set_flag_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0027, (data & 0xff));
}
static __inline td_u8 ot_ext_af_set_flag_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0027) & 0xff);
}

/* args: data (16-bit) */
static __inline td_void ot_ext_af_mean_thres_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0028), data);
}
static __inline td_u16 ot_ext_af_mean_thres_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0028));
}

static __inline td_void ot_ext_af_window_vnum_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x002a), data);
}
static __inline td_u16 ot_ext_af_window_vnum_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x002a));
}

static __inline td_void ot_ext_af_window_hnum_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x002c), data);
}
static __inline td_u16 ot_ext_af_window_hnum_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x002c));
}

static __inline td_void ot_ext_af_iir_thre0_coring_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x002e), data);
}
static __inline td_u16 ot_ext_af_iir_thre0_coring_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x002e));
}

static __inline td_void ot_ext_af_iir_thre1_coring_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0030), data);
}
static __inline td_u16 ot_ext_af_iir_thre1_coring_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0030));
}

static __inline td_void ot_ext_af_iir_peak0_coring_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0032), data);
}
static __inline td_u16 ot_ext_af_iir_peak0_coring_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0032));
}

static __inline td_void ot_ext_af_iir_peak1_coring_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0034), data);
}
static __inline td_u16 ot_ext_af_iir_peak1_coring_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0034));
}

static __inline td_void ot_ext_af_fir_thre0_coring_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0036), data);
}
static __inline td_u16 ot_ext_af_fir_thre0_coring_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0036));
}

static __inline td_void ot_ext_af_fir_thre1_coring_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0038), data);
}
static __inline td_u16 ot_ext_af_fir_thre1_coring_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0038));
}

static __inline td_void ot_ext_af_fir_peak0_coring_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x003a), data);
}
static __inline td_u16 ot_ext_af_fir_peak0_coring_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x003a));
}

static __inline td_void ot_ext_af_fir_peak1_coring_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x003c), data);
}
static __inline td_u16 ot_ext_af_fir_peak1_coring_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x003c));
}

static __inline td_void ot_ext_af_iir_slope0_coring_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x003e), data);
}
static __inline td_u16 ot_ext_af_iir_slope0_coring_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x003e));
}

static __inline td_void ot_ext_af_iir_slope1_coring_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0040), data);
}
static __inline td_u16 ot_ext_af_iir_slope1_coring_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0040));
}

static __inline td_void ot_ext_af_fir_slope0_coring_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0042), data);
}
static __inline td_u16 ot_ext_af_fir_slope0_coring_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0042));
}

static __inline td_void ot_ext_af_fir_slope1_coring_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0044), data);
}
static __inline td_u16 ot_ext_af_fir_slope1_coring_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0044));
}

static __inline td_void ot_ext_af_offset_gr_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0046), data);
}
static __inline td_u16 ot_ext_af_offset_gr_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0046));
}

static __inline td_void ot_ext_af_offset_gb_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0048), data);
}
static __inline td_u16 ot_ext_af_offset_gb_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0048));
}

static __inline td_void ot_ext_af_input_v_size_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x004a), data);
}
static __inline td_u16 ot_ext_af_input_v_size_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x004a));
}

static __inline td_void ot_ext_af_input_h_size_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x004c), data);
}
static __inline td_u16 ot_ext_af_input_h_size_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x004c));
}

static __inline td_void ot_ext_af_iir0_shift_group0_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x004e), data);
}
static __inline td_u16 ot_ext_af_iir0_shift_group0_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x004e));
}

static __inline td_void ot_ext_af_iir1_shift_group0_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0050), data);
}
static __inline td_u16 ot_ext_af_iir1_shift_group0_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0050));
}

static __inline td_void ot_ext_af_iir2_shift_group0_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0052), data);
}
static __inline td_u16 ot_ext_af_iir2_shift_group0_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0052));
}

static __inline td_void ot_ext_af_iir3_shift_group0_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0054), data);
}
static __inline td_u16 ot_ext_af_iir3_shift_group0_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0054));
}

static __inline td_void ot_ext_af_iir0_shift_group1_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0056), data);
}
static __inline td_u16 ot_ext_af_iir0_shift_group1_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0056));
}

static __inline td_void ot_ext_af_iir1_shift_group1_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0058), data);
}
static __inline td_u16 ot_ext_af_iir1_shift_group1_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0058));
}

static __inline td_void ot_ext_af_iir2_shift_group1_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x005a), data);
}
static __inline td_u16 ot_ext_af_iir2_shift_group1_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x005a));
}

static __inline td_void ot_ext_af_iir3_shift_group1_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x005c), data);
}
static __inline td_u16 ot_ext_af_iir3_shift_group1_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x005c));
}

static __inline td_void ot_ext_af_iir_thre0_low_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x005e), data);
}
static __inline td_u16 ot_ext_af_iir_thre0_low_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x005e));
}

static __inline td_void ot_ext_af_iir_thre0_high_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0060), data);
}
static __inline td_u16 ot_ext_af_iir_thre0_high_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0060));
}

static __inline td_void ot_ext_af_iir_thre1_low_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0062), data);
}
static __inline td_u16 ot_ext_af_iir_thre1_low_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0062));
}

static __inline td_void ot_ext_af_iir_thre1_high_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0064), data);
}
static __inline td_u16 ot_ext_af_iir_thre1_high_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0064));
}

static __inline td_void ot_ext_af_iir_gain0_low_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0066), data);
}
static __inline td_u16 ot_ext_af_iir_gain0_low_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0066));
}

static __inline td_void ot_ext_af_iir_gain0_high_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0068), data);
}
static __inline td_u16 ot_ext_af_iir_gain0_high_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0068));
}

static __inline td_void ot_ext_af_iir_gain1_low_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x006a), data);
}
static __inline td_u16 ot_ext_af_iir_gain1_low_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x006a));
}

static __inline td_void ot_ext_af_iir_gain1_high_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x006c), data);
}
static __inline td_u16 ot_ext_af_iir_gain1_high_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x006c));
}

static __inline td_void ot_ext_af_iir_slope0_low_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x006e), data);
}
static __inline td_u16 ot_ext_af_iir_slope0_low_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x006e));
}

static __inline td_void ot_ext_af_iir_slope0_high_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0070), data);
}
static __inline td_u16 ot_ext_af_iir_slope0_high_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0070));
}

static __inline td_void ot_ext_af_iir_slope1_low_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0072), data);
}
static __inline td_u16 ot_ext_af_iir_slope1_low_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0072));
}

static __inline td_void ot_ext_af_iir_slope1_high_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0074), data);
}
static __inline td_u16 ot_ext_af_iir_slope1_high_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0074));
}

static __inline td_void ot_ext_af_fir_thre0_low_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0076), data);
}
static __inline td_u16 ot_ext_af_fir_thre0_low_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0076));
}

static __inline td_void ot_ext_af_fir_thre0_high_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0078), data);
}
static __inline td_u16 ot_ext_af_fir_thre0_high_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0078));
}

static __inline td_void ot_ext_af_fir_thre1_low_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x007a), data);
}
static __inline td_u16 ot_ext_af_fir_thre1_low_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x007a));
}

static __inline td_void ot_ext_af_fir_thre1_high_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x007c), data);
}
static __inline td_u16 ot_ext_af_fir_thre1_high_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x007c));
}

static __inline td_void ot_ext_af_fir_gain0_low_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x007e), data);
}
static __inline td_u16 ot_ext_af_fir_gain0_low_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x007e));
}

static __inline td_void ot_ext_af_fir_gain0_high_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0080), data);
}
static __inline td_u16 ot_ext_af_fir_gain0_high_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0080));
}

static __inline td_void ot_ext_af_fir_gain1_low_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0082), data);
}
static __inline td_u16 ot_ext_af_fir_gain1_low_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0082));
}

static __inline td_void ot_ext_af_fir_gain1_high_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0084), data);
}
static __inline td_u16 ot_ext_af_fir_gain1_high_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0084));
}

static __inline td_void ot_ext_af_fir_slope0_low_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0086), data);
}
static __inline td_u16 ot_ext_af_fir_slope0_low_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0086));
}

static __inline td_void ot_ext_af_fir_slope0_high_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0088), data);
}
static __inline td_u16 ot_ext_af_fir_slope0_high_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0088));
}

static __inline td_void ot_ext_af_fir_slope1_low_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x008a), data);
}
static __inline td_u16 ot_ext_af_fir_slope1_low_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x008a));
}

static __inline td_void ot_ext_af_fir_slope1_high_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x008c), data);
}
static __inline td_u16 ot_ext_af_fir_slope1_high_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x008c));
}

static __inline td_void ot_ext_af_acc_shift0_h_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x008e), data);
}
static __inline td_u16 ot_ext_af_acc_shift0_h_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x008e));
}

static __inline td_void ot_ext_af_acc_shift1_h_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0090), data);
}
static __inline td_u16 ot_ext_af_acc_shift1_h_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0090));
}

static __inline td_void ot_ext_af_acc_shift0_v_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0092), data);
}
static __inline td_u16 ot_ext_af_acc_shift0_v_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0092));
}

static __inline td_void ot_ext_af_acc_shift1_v_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0094), data);
}
static __inline td_u16 ot_ext_af_acc_shift1_v_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0094));
}

static __inline td_void ot_ext_af_acc_shift_y_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0096), data);
}
static __inline td_u16 ot_ext_af_acc_shift_y_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0096));
}

static __inline td_void ot_ext_af_crop_pos_y_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0098), data);
}
static __inline td_u16 ot_ext_af_crop_pos_y_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0098));
}

static __inline td_void ot_ext_af_crop_pos_x_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x009a), data);
}
static __inline td_u16 ot_ext_af_crop_pos_x_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x009a));
}

static __inline td_void ot_ext_af_crop_v_size_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x009c), data);
}
static __inline td_u16 ot_ext_af_crop_v_size_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x009c));
}

static __inline td_void ot_ext_af_crop_h_size_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x009e), data);
}
static __inline td_u16 ot_ext_af_crop_h_size_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x009e));
}

static __inline td_void ot_ext_af_iir_gain0_group0_write(ot_vi_pipe vi_pipe, td_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00a0), data);
}
static __inline td_s16 ot_ext_af_iir_gain0_group0_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00a0));
}

static __inline td_void ot_ext_af_iir_gain0_group1_write(ot_vi_pipe vi_pipe, td_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00a2), data);
}
static __inline td_s16 ot_ext_af_iir_gain0_group1_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00a2));
}

static __inline td_void ot_ext_af_iir_gain1_group0_write(ot_vi_pipe vi_pipe, td_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00a4), data);
}
static __inline td_s16 ot_ext_af_iir_gain1_group0_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00a4));
}

static __inline td_void ot_ext_af_iir_gain1_group1_write(ot_vi_pipe vi_pipe, td_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00a6), data);
}
static __inline td_s16 ot_ext_af_iir_gain1_group1_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00a6));
}

static __inline td_void ot_ext_af_iir_gain2_group0_write(ot_vi_pipe vi_pipe, td_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00a8), data);
}
static __inline td_s16 ot_ext_af_iir_gain2_group0_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00a8));
}

static __inline td_void ot_ext_af_iir_gain2_group1_write(ot_vi_pipe vi_pipe, td_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00aa), data);
}
static __inline td_s16 ot_ext_af_iir_gain2_group1_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00aa));
}

static __inline td_void ot_ext_af_iir_gain3_group0_write(ot_vi_pipe vi_pipe, td_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00ac), data);
}
static __inline td_s16 ot_ext_af_iir_gain3_group0_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00ac));
}

static __inline td_void ot_ext_af_iir_gain3_group1_write(ot_vi_pipe vi_pipe, td_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00ae), data);
}
static __inline td_s16 ot_ext_af_iir_gain3_group1_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00ae));
}

static __inline td_void ot_ext_af_iir_gain4_group0_write(ot_vi_pipe vi_pipe, td_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00b0), data);
}
static __inline td_s16 ot_ext_af_iir_gain4_group0_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00b0));
}

static __inline td_void ot_ext_af_iir_gain4_group1_write(ot_vi_pipe vi_pipe, td_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00b2), data);
}
static __inline td_s16 ot_ext_af_iir_gain4_group1_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00b2));
}

static __inline td_void ot_ext_af_iir_gain5_group0_write(ot_vi_pipe vi_pipe, td_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00b4), data);
}
static __inline td_s16 ot_ext_af_iir_gain5_group0_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00b4));
}

static __inline td_void ot_ext_af_iir_gain5_group1_write(ot_vi_pipe vi_pipe, td_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00b6), data);
}
static __inline td_s16 ot_ext_af_iir_gain5_group1_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00b6));
}

static __inline td_void ot_ext_af_iir_gain6_group0_write(ot_vi_pipe vi_pipe, td_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00b8), data);
}
static __inline td_s16 ot_ext_af_iir_gain6_group0_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00b8));
}

static __inline td_void ot_ext_af_iir_gain6_group1_write(ot_vi_pipe vi_pipe, td_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00ba), data);
}
static __inline td_s16 ot_ext_af_iir_gain6_group1_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00ba));
}

static __inline td_void ot_ext_af_fir_h_gain0_group0_write(ot_vi_pipe vi_pipe, td_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00bc), data);
}
static __inline td_s16 ot_ext_af_fir_h_gain0_group0_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00bc));
}

static __inline td_void ot_ext_af_fir_h_gain0_group1_write(ot_vi_pipe vi_pipe, td_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00be), data);
}
static __inline td_s16 ot_ext_af_fir_h_gain0_group1_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00be));
}

static __inline td_void ot_ext_af_fir_h_gain1_group0_write(ot_vi_pipe vi_pipe, td_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00c0), data);
}
static __inline td_s16 ot_ext_af_fir_h_gain1_group0_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00c0));
}

static __inline td_void ot_ext_af_fir_h_gain1_group1_write(ot_vi_pipe vi_pipe, td_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00c2), data);
}
static __inline td_s16 ot_ext_af_fir_h_gain1_group1_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00c2));
}

static __inline td_void ot_ext_af_fir_h_gain2_group0_write(ot_vi_pipe vi_pipe, td_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00c4), data);
}
static __inline td_s16 ot_ext_af_fir_h_gain2_group0_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00c4));
}

static __inline td_void ot_ext_af_fir_h_gain2_group1_write(ot_vi_pipe vi_pipe, td_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00c6), data);
}
static __inline td_s16 ot_ext_af_fir_h_gain2_group1_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00c6));
}

static __inline td_void ot_ext_af_fir_h_gain3_group0_write(ot_vi_pipe vi_pipe, td_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00c8), data);
}
static __inline td_s16 ot_ext_af_fir_h_gain3_group0_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00c8));
}

static __inline td_void ot_ext_af_fir_h_gain3_group1_write(ot_vi_pipe vi_pipe, td_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00ca), data);
}
static __inline td_s16 ot_ext_af_fir_h_gain3_group1_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00ca));
}

static __inline td_void ot_ext_af_fir_h_gain4_group0_write(ot_vi_pipe vi_pipe, td_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00cc), data);
}
static __inline td_s16 ot_ext_af_fir_h_gain4_group0_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00cc));
}

static __inline td_void ot_ext_af_fir_h_gain4_group1_write(ot_vi_pipe vi_pipe, td_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00ce), data);
}
static __inline td_s16 ot_ext_af_fir_h_gain4_group1_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00ce));
}

static __inline td_void ot_ext_af_shift_count_y_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00d0), data);
}
static __inline td_u16 ot_ext_af_shift_count_y_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00d0));
}

/* 8bit */
static __inline td_void ot_ext_af_iir0_shift_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x00d2, (data & 0xff));
}
static __inline td_u8 ot_ext_af_iir0_shift_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x00d2) & 0xff);
}

static __inline td_void ot_ext_af_iir1_shift_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x00d3, (data & 0xff));
}
static __inline td_u8 ot_ext_af_iir1_shift_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x00d3) & 0xff);
}

/* args: data (2-bit) */
/*  AF sta info enable, [0] = FE sta,[1] = BE sta  */
static __inline td_void ot_ext_system_af_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x00e0, (data & OT_ISP_AF_ENABLE_MASK));
}
static __inline td_u8 ot_ext_system_af_enable_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x00e0) & OT_ISP_AF_ENABLE_MASK);
}


static __inline td_void ot_ext_af_fe_crop_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x00e4, (data & 0x01));
}
static __inline td_u8 ot_ext_af_fe_crop_enable_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x00e4) & 0x01);
}


static __inline td_void ot_ext_af_fe_crop_pos_y_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00e8), data);
}
static __inline td_u16 ot_ext_af_fe_crop_pos_y_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00e8));
}

static __inline td_void ot_ext_af_fe_crop_pos_x_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00ea), data);
}
static __inline td_u16 ot_ext_af_fe_crop_pos_x_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00ea));
}

static __inline td_void ot_ext_af_fe_crop_v_size_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00ec), data);
}
static __inline td_u16 ot_ext_af_fe_crop_v_size_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00ec));
}

static __inline td_void ot_ext_af_fe_crop_h_size_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00ee), data);
}
static __inline td_u16 ot_ext_af_fe_crop_h_size_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00ee));
}

/* ============================================================ */
/* DRC */
/* ============================================================ */
static __inline td_void ot_ext_system_drc_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x0), data);
}

static __inline td_u8 ot_ext_system_drc_enable_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x0));
}

static __inline td_void ot_ext_system_drc_param_updated_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x01), data);
}

static __inline td_u8 ot_ext_system_drc_param_updated_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x01));
}

static __inline td_void ot_ext_system_drc_curve_select_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x02), data);
}

static __inline td_u8 ot_ext_system_drc_curve_select_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x02));
}

static __inline td_void ot_ext_system_drc_bright_gain_lmt_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x03), data);
}

static __inline td_u8 ot_ext_system_drc_bright_gain_lmt_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x03));
}

static __inline td_void ot_ext_system_drc_bright_gain_lmt_step_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x04), data);
}

static __inline td_u8 ot_ext_system_drc_bright_gain_lmt_step_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x04));
}

static __inline td_void ot_ext_system_drc_cc_ctrl_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x05), data);
}


static __inline td_u8 ot_ext_system_drc_cc_ctrl_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x05));
}

static __inline td_void ot_ext_system_drc_tm_ctrl_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x06), data);
}

static __inline td_u8 ot_ext_system_drc_tm_ctrl_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x06));
}

static __inline td_void ot_ext_system_drc_dark_gain_lmt_y_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x07), data);
}


static __inline td_u8 ot_ext_system_drc_dark_gain_lmt_y_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x07));
}

static __inline td_void ot_ext_system_drc_dark_gain_lmt_c_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x08), data);
}


static __inline td_u8 ot_ext_system_drc_dark_gain_lmt_c_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x08));
}

static __inline td_void ot_ext_system_drc_contrast_ctrl_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x09), data);
}


static __inline td_u8 ot_ext_system_drc_contrast_ctrl_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x09));
}

static __inline td_void ot_ext_system_drc_manual_mode_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x0A), data);
}


static __inline td_u8 ot_ext_system_drc_manual_mode_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x0A));
}

static __inline td_void ot_ext_system_drc_asymmetry_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x0B), data);
}


static __inline td_u8 ot_ext_system_drc_asymmetry_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x0B));
}

static __inline td_void ot_ext_system_drc_secondpole_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x0C), data);
}


static __inline td_u8 ot_ext_system_drc_secondpole_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x0C));
}

static __inline td_void ot_ext_system_drc_compress_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x0D), data);
}


static __inline td_u8 ot_ext_system_drc_compress_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x0D));
}

static __inline td_void ot_ext_system_drc_stretch_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x0E), data);
}


static __inline td_u8 ot_ext_system_drc_stretch_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x0E));
}

static __inline td_void ot_ext_system_isp_drc_init_status_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_drc_base(vi_pipe) + 0xF, (data & 0x01));
}

static __inline td_u8 ot_ext_system_isp_drc_init_status_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0xF) & 0x01);
}

static __inline td_void ot_ext_system_drc_manual_strength_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_drc_base(vi_pipe) + 0x10), data);
}


static __inline td_u16 ot_ext_system_drc_manual_strength_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_drc_base(vi_pipe) + 0x10));
}

static __inline td_void ot_ext_system_drc_auto_strength_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_drc_base(vi_pipe) + 0x12), data);
}


static __inline td_u16 ot_ext_system_drc_auto_strength_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_drc_base(vi_pipe) + 0x12));
}

static __inline td_void ot_ext_system_drc_auto_strength_max_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_drc_base(vi_pipe) + 0x14), data);
}


static __inline td_u16 ot_ext_system_drc_auto_strength_max_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_drc_base(vi_pipe) + 0x14));
}

static __inline td_void ot_ext_system_drc_auto_strength_min_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_drc_base(vi_pipe) + 0x16), data);
}


static __inline td_u16 ot_ext_system_drc_auto_strength_min_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_drc_base(vi_pipe) + 0x16));
}

static __inline td_void ot_ext_system_drc_spa_flt_coef_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x18), data);
}

static __inline td_u8 ot_ext_system_drc_spa_flt_coef_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x18));
}

static __inline td_void ot_ext_system_drc_rng_flt_coef_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x19), data);
}

static __inline td_u8 ot_ext_system_drc_rng_flt_coef_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x19));
}
/* addr range DRC[0x1A, 0x3b) index:33 */
static __inline td_void ot_ext_system_drc_mixing_bright_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x1A + sizeof(td_u8) * index), data);
}

static __inline td_u8 ot_ext_system_drc_mixing_bright_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct((vreg_drc_base(vi_pipe) + 0x1A + sizeof(td_u8) * index)));
}

/* addr range DRC[0x3b, 0x5c) index:33 */
static __inline td_void ot_ext_system_drc_mixing_dark_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x3B + sizeof(td_u8) * index), data);
}

static __inline td_u8 ot_ext_system_drc_mixing_dark_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct((vreg_drc_base(vi_pipe) + 0x3B + sizeof(td_u8) * index)));
}


static __inline td_void ot_ext_system_drc_grad_rev_max_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x5C), data);
}

static __inline td_u8 ot_ext_system_drc_grad_rev_max_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x5C));
}

static __inline td_void ot_ext_system_drc_grad_rev_thr_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x5D), data);
}

static __inline td_u8 ot_ext_system_drc_grad_rev_thr_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x5D));
}

static __inline td_void ot_ext_system_drc_detail_gain_max_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x5E), data);
}

static __inline td_u8 ot_ext_system_drc_detail_gain_max_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x5E));
}

static __inline td_void ot_ext_system_drc_detail_gain_min_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x5F), data);
}

static __inline td_u8 ot_ext_system_drc_detail_gain_min_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x5F));
}

static __inline td_void ot_ext_system_drc_detail_gain_thr_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x60), data);
}

static __inline td_u8 ot_ext_system_drc_detail_gain_thr_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x60));
}

static __inline td_void ot_ext_system_drc_detail_gain_slo_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x61), data);
}

static __inline td_u8 ot_ext_system_drc_detail_gain_slo_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x61));
}

static __inline td_void ot_ext_system_drc_detail_adjust_coef_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x62), data);
}

static __inline td_u8 ot_ext_system_drc_detail_adjust_coef_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x62));
}

/* addr range DRC[0x64, 0x67) index:3 */
static __inline td_void ot_ext_system_drc_flt_coef_x_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x64 + sizeof(td_u8) * index), data);
}

static __inline td_u8 ot_ext_system_drc_flt_coef_x_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct((vreg_drc_base(vi_pipe) + 0x64 + sizeof(td_u8) * index)));
}

/* addr range DRC[0x68, 0x6b) index:3 */
static __inline td_void ot_ext_system_drc_flt_low_thr_x_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x68 + sizeof(td_u8) * index), data);
}

static __inline td_u8 ot_ext_system_drc_flt_low_thr_x_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct((vreg_drc_base(vi_pipe) + 0x68 + sizeof(td_u8) * index)));
}

/* addr range DRC[0x6c, 0x6f) index:3 */
static __inline td_void ot_ext_system_drc_flt_high_thr_x_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x6C + sizeof(td_u8) * index), data);
}

static __inline td_u8 ot_ext_system_drc_flt_high_thr_x_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct((vreg_drc_base(vi_pipe) + 0x6C + sizeof(td_u8) * index)));
}

/* addr range DRC[0x70, 0x73) index:3 */
static __inline td_void ot_ext_system_drc_mixing_bright_x_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x70 + sizeof(td_u8) * index), data);
}

static __inline td_u8 ot_ext_system_drc_mixing_bright_x_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct((vreg_drc_base(vi_pipe) + 0x70 + sizeof(td_u8) * index)));
}
/* addr range DRC[0x92, 0x95) index:3 */

static __inline td_void ot_ext_system_drc_mixing_dark_x_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x92 + sizeof(td_u8) * index), data);
}

static __inline td_u8 ot_ext_system_drc_mixing_dark_x_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct((vreg_drc_base(vi_pipe) + 0x92 + sizeof(td_u8) * index)));
}

static __inline td_void ot_ext_system_drc_detail_gain_x_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0xB4), data);
}

static __inline td_u8 ot_ext_system_drc_detail_gain_x_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0xB4));
}

static __inline td_void ot_ext_system_drc_detail_adjust_coef_x_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0xB5), data);
}

static __inline td_u8 ot_ext_system_drc_detail_adjust_coef_x_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0xB5));
}

/* addr range DRC[0xB6, 0xf8) index:33 */
static __inline td_void ot_ext_system_drc_cc_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct((vreg_drc_base(vi_pipe) + 0xB6 + sizeof(td_u16) * index), data);
}

static __inline td_u16 ot_ext_system_drc_cc_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_16direct((vreg_drc_base(vi_pipe) + 0xB6 + sizeof(td_u16) * index)));
}

/* addr range DRC[0xf8, 0x288) index:200 */
static __inline td_void ot_ext_system_drc_tm_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct((vreg_drc_base(vi_pipe) + 0xF8 + sizeof(td_u16) * index), data);
}

static __inline td_u16 ot_ext_system_drc_tm_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_16direct((vreg_drc_base(vi_pipe) + 0xF8 + sizeof(td_u16) * index)));
}

static __inline td_void ot_ext_system_drc_flt_bld_d_shift_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x288), data);
}

static __inline td_u8 ot_ext_system_drc_flt_bld_d_shift_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x288));
}

static __inline td_void ot_ext_system_drc_flt_bld_d_max_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x289), data);
}

static __inline td_u8 ot_ext_system_drc_flt_bld_d_max_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x289));
}

static __inline td_void ot_ext_system_drc_flt_bld_d_bright_min_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x28A), data);
}

static __inline td_u8 ot_ext_system_drc_flt_bld_d_bright_min_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x28A));
}

static __inline td_void ot_ext_system_drc_flt_bld_d_bright_thr_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x28B), data);
}

static __inline td_u8 ot_ext_system_drc_flt_bld_d_bright_thr_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x28B));
}

static __inline td_void ot_ext_system_drc_flt_bld_d_bright_slo_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x28C), data);
}

static __inline td_u8 ot_ext_system_drc_flt_bld_d_bright_slo_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x28C));
}

static __inline td_void ot_ext_system_drc_flt_bld_d_dark_min_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x28D), data);
}

static __inline td_u8 ot_ext_system_drc_flt_bld_d_dark_min_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x28D));
}

static __inline td_void ot_ext_system_drc_flt_bld_d_dark_thr_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x28E), data);
}

static __inline td_u8 ot_ext_system_drc_flt_bld_d_dark_thr_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x28E));
}

static __inline td_void ot_ext_system_drc_flt_bld_d_dark_slo_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x28F), data);
}

static __inline td_u8 ot_ext_system_drc_flt_bld_d_dark_slo_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x28F));
}

static __inline td_void ot_ext_system_drc_flt_bld_l_shift_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x290), data);
}

static __inline td_u8 ot_ext_system_drc_flt_bld_l_shift_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x290));
}

static __inline td_void ot_ext_system_drc_flt_bld_l_max_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x291), data);
}

static __inline td_u8 ot_ext_system_drc_flt_bld_l_max_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x291));
}

static __inline td_void ot_ext_system_drc_flt_bld_l_bright_min_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x292), data);
}

static __inline td_u8 ot_ext_system_drc_flt_bld_l_bright_min_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x292));
}

static __inline td_void ot_ext_system_drc_flt_bld_l_bright_thr_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x293), data);
}

static __inline td_u8 ot_ext_system_drc_flt_bld_l_bright_thr_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x293));
}

static __inline td_void ot_ext_system_drc_flt_bld_l_bright_slo_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x294), data);
}

static __inline td_u8 ot_ext_system_drc_flt_bld_l_bright_slo_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x294));
}

static __inline td_void ot_ext_system_drc_flt_bld_l_dark_min_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x295), data);
}

static __inline td_u8 ot_ext_system_drc_flt_bld_l_dark_min_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x295));
}

static __inline td_void ot_ext_system_drc_flt_bld_l_dark_thr_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x296), data);
}

static __inline td_u8 ot_ext_system_drc_flt_bld_l_dark_thr_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x296));
}

static __inline td_void ot_ext_system_drc_flt_bld_l_dark_slo_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x297), data);
}

static __inline td_u8 ot_ext_system_drc_flt_bld_l_dark_slo_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x297));
}

static __inline td_void ot_ext_system_drc_detail_gain_bld_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x298), data);
}

static __inline td_u8 ot_ext_system_drc_detail_gain_bld_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x298));
}

static __inline td_void ot_ext_system_drc_shoot_reduction_en_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x299), data);
}

static __inline td_u8 ot_ext_system_drc_shoot_reduction_en_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x299));
}

static __inline td_void ot_ext_system_drc_high_sat_ctrl_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x29A), data);
}

static __inline td_u8 ot_ext_system_drc_high_sat_ctrl_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x29A));
}

static __inline td_void ot_ext_system_drc_pd_strength_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x29B), data);
}

static __inline td_u8 ot_ext_system_drc_pd_strength_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x29B));
}

static __inline td_void ot_ext_system_drc_advanced_mode_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x29C), data);
}

static __inline td_u8 ot_ext_system_drc_advanced_mode_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x29C));
}

static __inline td_void ot_ext_system_drc_tone_mapping_wgt_x_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x29D), data);
}

static __inline td_u8 ot_ext_system_drc_tone_mapping_wgt_x_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x29D));
}

static __inline td_void ot_ext_system_drc_gtm_gain_coef_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x29E), data);
}

static __inline td_u8 ot_ext_system_drc_gtm_gain_coef_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x29E));
}

static __inline td_void ot_ext_system_drc_global_color_ctrl_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x29F), data);
}

static __inline td_u8 ot_ext_system_drc_global_color_ctrl_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x29F));
}

/* addr range DRC[0x2A0, 0x2B8) index:12 */
static __inline td_void ot_ext_system_drc_stat_coef_scale_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct((vreg_drc_base(vi_pipe) + 0x2A0 + sizeof(td_u16) * index), data);
}

static __inline td_u16 ot_ext_system_drc_stat_coef_scale_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_16direct((vreg_drc_base(vi_pipe) + 0x2A0 + sizeof(td_u16) * index)));
}

/* addr range DRC[0x2B8, 0x2C4) index:12 */
static __inline td_void ot_ext_system_drc_stat_coef_shift_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x2B8 + sizeof(td_u8) * index), data);
}

static __inline td_u8 ot_ext_system_drc_stat_coef_shift_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct((vreg_drc_base(vi_pipe) + 0x2B8 + sizeof(td_u8) * index)));
}

/* addr range DRC[0x2C4, 0x2DC) index:12 */
static __inline td_void ot_ext_system_drc_stat_coef_offset_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_s16 data)
{
    iowr_16direct((vreg_drc_base(vi_pipe) + 0x2C4 + sizeof(td_s16) * index), data);
}

static __inline td_s16 ot_ext_system_drc_stat_coef_offset_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_16direct((vreg_drc_base(vi_pipe) + 0x2C4 + sizeof(td_s16) * index)));
}
/* csp en */
static __inline td_void ot_ext_system_drc_csp_en_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x2DC), data);
}

static __inline td_u8 ot_ext_system_drc_csp_en_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x2DC));
}

/* ============================================================== */
/* dehaze */
/* ============================================================== */
/* register: ot_ext_system_dehaze_manu_mode */
/* enables manual dehaze: 0=auto 1=manual */
#define OT_EXT_SYSTEM_DEHAZE_MANU_MODE_DEFAULT  0
/* args: data (8-bit) */
static __inline void ot_ext_system_dehaze_manu_mode_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_dehaze_base(vi_pipe), data);
}
static __inline td_u32 ot_ext_system_dehaze_manu_mode_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dehaze_base(vi_pipe));
}

/* register: ot_ext_system_manual_dehaze_hblk */
/* manual dehaze hblk number */
/* args: data (8-bit) */
static __inline void ot_ext_system_manual_dehaze_hblk_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_dehaze_base(vi_pipe) + 0x1, data);
}
static __inline td_u8 ot_ext_system_manual_dehaze_hblk_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dehaze_base(vi_pipe) + 0x1);
}

/* register: ot_ext_system_manual_dehaze_vblk */
/* manual dehaze vblk number */
/* args: data (8-bit) */
static __inline void ot_ext_system_manual_dehaze_vblk_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_dehaze_base(vi_pipe) + 0x2, data);
}

static __inline td_u8 ot_ext_system_manual_dehaze_vblk_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dehaze_base(vi_pipe) + 0x2);
}

/* register: ot_ext_system_dehaze_ctp */
/* manual dehaze curve turn point */
#define OT_EXT_SYSTEM_DEHAZE_CTP_DEFAULT  0x80

/* args: data (8-bit) */
static __inline void ot_ext_system_dehaze_ctp_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_dehaze_base(vi_pipe) + 0x3, data);
}
static __inline td_u8 ot_ext_system_dehaze_ctp_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dehaze_base(vi_pipe) + 0x3);
}

/* register: ot_ext_system_manual_dehaze_mft */
/* manual dehaze minimum filter threshold */
#define OT_EXT_SYSTEM_DEHAZE_MFT_DEFAULT 0x200

/* args: data (10-bit) */
static __inline void ot_ext_system_dehaze_mft_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_dehaze_base(vi_pipe) + 0x4, data);
}
static __inline td_u16 ot_ext_system_dehaze_mft_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_dehaze_base(vi_pipe) + 0x4) & 0x3ff);
}

/* register: ot_ext_system_manual_dehaze_mfs */
/* manual dehaze minimum filter size */
#define OT_EXT_SYSTEM_DEHAZE_MFS_DEFAULT 0x7
/* args: data (8-bit) */
static __inline void ot_ext_system_dehaze_mfs_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_dehaze_base(vi_pipe) + 0x6, data);
}
static __inline td_u8 ot_ext_system_dehaze_mfs_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dehaze_base(vi_pipe) + 0x6);
}

/* register: ot_ext_system_manual_dehaze_ct */
/* manual dehaze change threshold */
#define OT_EXT_SYSTEM_DEHAZE_CT_DEFAULT 255
/* args: data (8-bit) */
static __inline void ot_ext_system_dehaze_ct_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_dehaze_base(vi_pipe) + 0x7, data);
}
static __inline td_u8 ot_ext_system_dehaze_ct_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dehaze_base(vi_pipe) + 0x7);
}

/* register: ot_ext_system_manual_dehaze_tfic */
/* manual dehaze temper filter increase ceofficient */
#define OT_EXT_SYSTEM_DEHAZE_TFIC_DEFAULT  0x8
/* args: data (16-bit) */
static __inline void ot_ext_system_dehaze_tfic_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_dehaze_base(vi_pipe) + 0x8, data);
}
static __inline td_u16 ot_ext_system_dehaze_tfic_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_dehaze_base(vi_pipe) + 0x8);
}

/* register: ot_ext_system_manual_dehaze_tfdc */
/* manual dehaze temper filter decrease ceofficient */
#define OT_EXT_SYSTEM_DEHAZE_TFDC_DEFAULT  64
/* args: data (16-bit) */
static __inline void ot_ext_system_dehaze_tfdc_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_dehaze_base(vi_pipe) + 0xa, data);
}
static __inline td_u16 ot_ext_system_dehaze_tfdc_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_dehaze_base(vi_pipe) + 0xa);
}

/* register: ot_ext_system_dehaze_debug_enable */
/* dehaze debug enable */
#define OT_EXT_SYSTEM_DEHAZE_DEBUGFILE_ENABLE_DEFAULT  0

/* args: data (8-bit) */
static __inline void ot_ext_system_dehaze_debugfile_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_dehaze_base(vi_pipe) + 0xc, data);
}
static __inline td_u8 ot_ext_system_dehaze_debugfile_enable_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dehaze_base(vi_pipe) + 0xc);
}

/* register: ot_ext_system_manual_dehaze_strength */
/* dehaze strength */
#define OT_EXT_SYSTEM_MANUAL_DEHAZE_STRENGTH_DEFAULT  0x80

/* args: data (8-bit) */
static __inline void ot_ext_system_manual_dehaze_strength_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_dehaze_base(vi_pipe) + 0xd, data);
}
static __inline td_u8 ot_ext_system_manual_dehaze_strength_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dehaze_base(vi_pipe) + 0xd);
}

/* register: isp_ext_system_manual_dehaze_autostrength */
/* dehaze strength */
#define OT_EXT_SYSTEM_MANUAL_DEHAZE_AUTOSTRENGTH_DEFAULT  0x80
/* args: data (8-bit) */
static __inline void ot_ext_system_manual_dehaze_autostrength_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_dehaze_base(vi_pipe) + 0xe, data);
}
static __inline td_u8 ot_ext_system_manual_dehaze_autostrength_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dehaze_base(vi_pipe) + 0xe);
}

static __inline td_void ot_ext_system_isp_dehaze_init_status_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_dehaze_base(vi_pipe) + 0xf, (data & 0x01));
}

static __inline td_u8 ot_ext_system_isp_dehaze_init_status_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_dehaze_base(vi_pipe) + 0xf) & 0x01);
}

#define OT_EXT_SYSTEM_DEHAZE_ENABLE_DEFAULT  0x0

/* args: data (8-bit) */
static __inline void ot_ext_system_dehaze_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_dehaze_base(vi_pipe) + 0x10, data);
}
static __inline td_u8 ot_ext_system_dehaze_enable_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dehaze_base(vi_pipe) + 0x10);
}

/* args: data (1-bit) */
#define OT_EXT_SYSTEM_DEHAZE_DEBUG_ENABLE_DEFAULT 0x0
static __inline td_void ot_ext_system_dehaze_debug_enable_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_dehaze_base(vi_pipe) + 0x14, data & 0x1);
}
static __inline td_u16 ot_ext_system_dehaze_debug_enable_read(ot_vi_pipe vi_pipe)
{
    return (td_u16)(iord_16direct(vreg_dehaze_base(vi_pipe) + 0x14) & 0x1);
}

/* register: ot_ext_system_dehaze_debug_addr */
/* address of dehaze debug information */
/* args: data (32-bit) */
static __inline td_void ot_ext_system_dehaze_debug_high_addr_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_dehaze_base(vi_pipe) + 0x18, data);
}

static __inline td_u32 ot_ext_system_dehaze_debug_high_addr_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_dehaze_base(vi_pipe) + 0x18);
}

/* args: data (32-bit) */
static __inline td_void ot_ext_system_dehaze_debug_low_addr_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_dehaze_base(vi_pipe) + 0x1c, data);
}

static __inline td_u32 ot_ext_system_dehaze_debug_low_addr_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_dehaze_base(vi_pipe) + 0x1c);
}

/* register: ot_ext_system_dehaze_debug_size */
/* size of dehaze debug struct */
/* args: data (32-bit) */
static __inline td_void ot_ext_system_dehaze_debug_size_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_dehaze_base(vi_pipe) + 0x20, data);
}

static __inline td_u32 ot_ext_system_dehaze_debug_size_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_dehaze_base(vi_pipe) + 0x20);
}

/* register: ot_ext_system_dehaze_debug_depth */
/* depth of dehaze debug struct */
/* args: data (32-bit) */
static __inline td_void ot_ext_system_dehaze_debug_depth_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_dehaze_base(vi_pipe) + 0x24, data);
}

static __inline td_u32 ot_ext_system_dehaze_debug_depth_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_dehaze_base(vi_pipe) + 0x24);
}

/* register: ot_ext_system_dehaze_debug_quit */
/* depth of dehaze debug struct */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_dehaze_debug_quit_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_dehaze_base(vi_pipe) + 0x28, data);
}

static __inline td_u8 ot_ext_system_dehaze_debug_quit_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dehaze_base(vi_pipe) + 0x28);
}

/* register: ot_ext_system_dehaze_debug_complete */
/* depth of dehaze debug struct */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_dehaze_debug_complete_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_dehaze_base(vi_pipe) + 0x29, data);
}

static __inline td_u8 ot_ext_system_dehaze_debug_complete_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dehaze_base(vi_pipe) + 0x29);
}

/* register: ot_ext_system_defog_lut_write len = 256 */
/* args: data (8-bit) */
/* addr range dehaze[0x30, 0x130) index:256 */
static __inline td_void ot_ext_system_dehaze_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct(vreg_dehaze_base(vi_pipe) + 0x30 + (index * sizeof(td_u8)), data);
}

static __inline td_u8 ot_ext_system_dehaze_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return iord_8direct(vreg_dehaze_base(vi_pipe) + 0x30 + (index * sizeof(td_u8)));
}

#define OT_EXT_SYSTEM_USER_DEHAZE_LUT_ENABLE_DEFAULT  0
static __inline td_void ot_ext_system_user_dehaze_lut_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_dehaze_base(vi_pipe) + 0x140, data);
}
static __inline td_u8 ot_ext_system_user_dehaze_lut_enable_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dehaze_base(vi_pipe) + 0x140);
}

#define OT_EXT_SYSTEM_USER_DEHAZE_LUT_UPDATE_DEFAULT  0
static __inline td_void ot_ext_system_user_dehaze_lut_update_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_dehaze_base(vi_pipe) + 0x142, data);
}
static __inline td_u8 ot_ext_system_user_dehaze_lut_update_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dehaze_base(vi_pipe) + 0x142);
}

/* args: data (8-bit) */
static __inline td_void ot_ext_system_fpn_cablibrate_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_fpn_base(vi_pipe), data);
}

static __inline td_u8 ot_ext_system_fpn_cablibrate_enable_read(ot_vi_pipe vi_pipe)
{
    return  iord_8direct(vreg_fpn_base(vi_pipe));
}

static __inline void ot_ext_system_fpn_pre_hnr_status_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_fpn_base(vi_pipe) + 0x1, data);
}

static __inline td_u8 ot_ext_system_fpn_pre_hnr_status_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_fpn_base(vi_pipe) + 0x1);
}
/* register: ot_ext_system_manual_fpn_strength */
#define OT_EXT_SYSTEM_FPN_STRENGTH_DEFAULT  0
/* args: data (16-bit) */
/* register: ot_ext_system_manual_fpn_opmode */
#define OT_EXT_SYSTEM_FPN_OPMODE_DEFAULT  0x0
/* args: data (8-bit) */
static __inline void ot_ext_system_manual_fpn_opmode_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_fpn_base(vi_pipe) + 0x2, data);
}

static __inline td_u8 ot_ext_system_manual_fpn_opmode_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_fpn_base(vi_pipe) + 0x2);
}

/* register: isp_ext_system_manual_fpn_update */
#define OT_EXT_SYSTEM_FPN_MANU_UPDATE_DEFAULT  0x0
/* args: data (8-bit) */
static __inline void ot_ext_system_manual_fpn_update_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_fpn_base(vi_pipe) + 0x3, data);
}

static __inline td_u8 ot_ext_system_manual_fpn_update_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_fpn_base(vi_pipe) + 0x3);
}

/* register: ot_ext_system_manual_fpn_corr_cfg */
#define OT_EXT_SYSTEM_FPN_MANU_ISO_DEFAULT  6375
/* args: data (32-bit) */
static __inline void ot_ext_system_manual_fpn_iso_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_fpn_base(vi_pipe) + 0x4, data);
}

static __inline td_u32 ot_ext_system_manual_fpn_iso_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_fpn_base(vi_pipe) + 0x4);
}

/* register: ot_ext_system_sensor_iso */
/* sensor ISO */
#define OT_EXT_SYSTEM_FPN_SENSOR_ISO_DEFAULT  0x0

/* args: data (32-bit) */
static __inline td_void ot_ext_system_fpn_sensor_iso_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_fpn_base(vi_pipe) + 0x8, data);
}

static __inline td_u32 ot_ext_system_fpn_sensor_iso_read(ot_vi_pipe vi_pipe)
{
    return  iord_32direct(vreg_fpn_base(vi_pipe) + 0x8);
}

/* register: isp_ext_system_manual_fpn_corr_cfg */
#define OT_EXT_SYSTEM_FPN_MANU_CORRCFG_DEFAULT  0x0
/* args: data (8-bit) */
static __inline void ot_ext_system_manual_fpn_corr_cfg_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_fpn_base(vi_pipe) + 0xc, data);
}

static __inline td_u8 isp_ext_system_manual_fpn_corr_cfg_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_fpn_base(vi_pipe) + 0xc);
}

static __inline void ot_ext_system_manual_fpn_type_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_fpn_base(vi_pipe) + 0xd, data);
}

static __inline td_u8 isp_ext_system_manual_fpn_type_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_fpn_base(vi_pipe) + 0xd);
}

static __inline void ot_ext_system_fpn_cor_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_fpn_base(vi_pipe) + 0xe, data);
}

static __inline td_u8 isp_ext_system_fpn_cor_enable_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_fpn_base(vi_pipe) + 0xe);
}

static __inline void ot_ext_system_manual_fpn_pixel_format_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_fpn_base(vi_pipe) + 0xf, data);
}

static __inline td_u8 isp_ext_system_manual_fpn_pixel_format_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_fpn_base(vi_pipe) + 0xf);
}

static __inline void ot_ext_system_manual_fpn_gain_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_fpn_base(vi_pipe) + 0x10, data);
}

static __inline td_u32 isp_ext_system_manual_fpn_gain_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_fpn_base(vi_pipe) + 0x10);
}

static __inline void ot_ext_system_manual_fpn_offset0_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_fpn_base(vi_pipe) + 0x14, data);
}

static __inline td_u32 isp_ext_system_manual_fpn_offset0_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_fpn_base(vi_pipe) + 0x14);
}

static __inline void ot_ext_system_manual_fpn_offset1_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_fpn_base(vi_pipe) + 0x18, data);
}

static __inline td_u32 isp_ext_system_manual_fpn_offset1_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_fpn_base(vi_pipe) + 0x18);
}

static __inline void ot_ext_system_manual_fpn_offset2_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_fpn_base(vi_pipe) + 0x1C, data);
}

static __inline td_u32 isp_ext_system_manual_fpn_offset2_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_fpn_base(vi_pipe) + 0x1C);
}

static __inline void ot_ext_system_fpn_offset_shift_en_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_fpn_base(vi_pipe) + 0x20, data);
}

static __inline td_u8 ot_ext_system_fpn_offset_shift_en_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_fpn_base(vi_pipe) + 0x20);
}

static __inline void ot_ext_system_fpn_aibnr_mode_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_fpn_base(vi_pipe) + 0x21, data);
}

static __inline td_u8 isp_ext_system_fpn_aibnr_mode_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_fpn_base(vi_pipe) + 0x21);
}

/* BLACK LEVEL */
static __inline td_void ot_ext_system_black_level_f0_r_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_blc_base(vi_pipe), data);
}

static __inline td_u16 ot_ext_system_black_level_f0_r_read(ot_vi_pipe vi_pipe)
{
    return  iord_16direct(vreg_blc_base(vi_pipe));
}

static __inline td_void ot_ext_system_black_level_f0_gr_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_blc_base(vi_pipe) + 0x2, data);
}

static __inline td_u16 ot_ext_system_black_level_f0_gr_read(ot_vi_pipe vi_pipe)
{
    return  iord_16direct(vreg_blc_base(vi_pipe) + 0x2);
}

static __inline td_void ot_ext_system_black_level_f0_gb_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_blc_base(vi_pipe) + 0x4, data);
}

static __inline td_u16 ot_ext_system_black_level_f0_gb_read(ot_vi_pipe vi_pipe)
{
    return  iord_16direct(vreg_blc_base(vi_pipe) + 0x4);
}

static __inline td_void ot_ext_system_black_level_f0_b_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_blc_base(vi_pipe) + 0x6, data);
}

static __inline td_u16 ot_ext_system_black_level_f0_b_read(ot_vi_pipe vi_pipe)
{
    return  iord_16direct(vreg_blc_base(vi_pipe) + 0x6);
}

/* register: ot_ext_system_black_level_change */
/* ISP black level change */
static __inline td_void ot_ext_system_black_level_change_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_blc_base(vi_pipe) + 0x8, data);
}

static __inline td_u8 ot_ext_system_black_level_change_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_blc_base(vi_pipe) + 0x8);
}

static __inline td_void ot_ext_system_black_level_mode_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_blc_base(vi_pipe) + 0x9, data);
}
static __inline td_u8 ot_ext_system_black_level_mode_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_blc_base(vi_pipe) + 0x9);
}

/* wdr chn1 blc */
static __inline td_void ot_ext_system_black_level_f1_r_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_blc_base(vi_pipe) + 0xa, data);
}

static __inline td_u16 ot_ext_system_black_level_f1_r_read(ot_vi_pipe vi_pipe)
{
    return  iord_16direct(vreg_blc_base(vi_pipe) + 0xa);
}

static __inline td_void ot_ext_system_black_level_f1_gr_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_blc_base(vi_pipe) + 0xc, data);
}

static __inline td_u16 ot_ext_system_black_level_f1_gr_read(ot_vi_pipe vi_pipe)
{
    return  iord_16direct(vreg_blc_base(vi_pipe) + 0xc);
}

static __inline td_void ot_ext_system_black_level_f1_gb_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_blc_base(vi_pipe) + 0xe, data);
}

static __inline td_u16 ot_ext_system_black_level_f1_gb_read(ot_vi_pipe vi_pipe)
{
    return  iord_16direct(vreg_blc_base(vi_pipe) + 0xe);
}

static __inline td_void ot_ext_system_black_level_f1_b_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_blc_base(vi_pipe) + 0x10, data);
}

static __inline td_u16 ot_ext_system_black_level_f1_b_read(ot_vi_pipe vi_pipe)
{
    return  iord_16direct(vreg_blc_base(vi_pipe) + 0x10);
}

/* wdr chn2 blc */
static __inline td_void ot_ext_system_black_level_f2_r_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_blc_base(vi_pipe) + 0x12, data);
}

static __inline td_u16 ot_ext_system_black_level_f2_r_read(ot_vi_pipe vi_pipe)
{
    return  iord_16direct(vreg_blc_base(vi_pipe) + 0x12);
}

static __inline td_void ot_ext_system_black_level_f2_gr_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_blc_base(vi_pipe) + 0x14, data);
}

static __inline td_u16 ot_ext_system_black_level_f2_gr_read(ot_vi_pipe vi_pipe)
{
    return  iord_16direct(vreg_blc_base(vi_pipe) + 0x14);
}

static __inline td_void ot_ext_system_black_level_f2_gb_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_blc_base(vi_pipe) + 0x16, data);
}

static __inline td_u16 ot_ext_system_black_level_f2_gb_read(ot_vi_pipe vi_pipe)
{
    return  iord_16direct(vreg_blc_base(vi_pipe) + 0x16);
}

static __inline td_void ot_ext_system_black_level_f2_b_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_blc_base(vi_pipe) + 0x18, data);
}

static __inline td_u16 ot_ext_system_black_level_f2_b_read(ot_vi_pipe vi_pipe)
{
    return  iord_16direct(vreg_blc_base(vi_pipe) + 0x18);
}

/* wdr chn3 blc */
static __inline td_void ot_ext_system_black_level_f3_r_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_blc_base(vi_pipe) + 0x1a, data);
}

static __inline td_u16 ot_ext_system_black_level_f3_r_read(ot_vi_pipe vi_pipe)
{
    return  iord_16direct(vreg_blc_base(vi_pipe) + 0x1a);
}

static __inline td_void ot_ext_system_black_level_f3_gr_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_blc_base(vi_pipe) + 0x1c, data);
}

static __inline td_u16 ot_ext_system_black_level_f3_gr_read(ot_vi_pipe vi_pipe)
{
    return  iord_16direct(vreg_blc_base(vi_pipe) + 0x1c);
}

static __inline td_void ot_ext_system_black_level_f3_gb_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_blc_base(vi_pipe) + 0x1e, data);
}

static __inline td_u16 ot_ext_system_black_level_f3_gb_read(ot_vi_pipe vi_pipe)
{
    return  iord_16direct(vreg_blc_base(vi_pipe) + 0x1e);
}

/* args: data (16-bit) */
static __inline td_void ot_ext_system_black_level_f3_b_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_blc_base(vi_pipe) + 0x20, data);
}

static __inline td_u16 ot_ext_system_black_level_f3_b_read(ot_vi_pipe vi_pipe)
{
    return  iord_16direct(vreg_blc_base(vi_pipe) + 0x20);
}

/* Register: ot_ext_system_isp_dynamic_black_level_change_enable */
/* args: data (16-bit) */
static __inline td_void ot_ext_system_isp_dynamic_blc_pattern_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_blc_base(vi_pipe) + 0x22, data);
}

static __inline td_u16 ot_ext_system_isp_dynamic_blc_pattern_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_blc_base(vi_pipe) + 0x22);
}

static __inline td_void ot_ext_system_isp_dynamic_blc_startxpos_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_blc_base(vi_pipe) + 0x24, data);
}

static __inline td_u16 ot_ext_system_isp_dynamic_blc_startxpos_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_blc_base(vi_pipe) + 0x24);
}

static __inline td_void ot_ext_system_isp_dynamic_blc_startypos_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_blc_base(vi_pipe) + 0x26, data);
}

static __inline td_u16 ot_ext_system_isp_dynamic_blc_startypos_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_blc_base(vi_pipe) + 0x26);
}

static __inline td_void ot_ext_system_isp_dynamic_blc_obwidth_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_blc_base(vi_pipe) + 0x28, data);
}

static __inline td_u16 ot_ext_system_isp_dynamic_blc_obwidth_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_blc_base(vi_pipe) + 0x28);
}

static __inline td_void ot_ext_system_isp_dynamic_blc_obheight_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_blc_base(vi_pipe) + 0x2a, data);
}

static __inline td_u16 ot_ext_system_isp_dynamic_blc_obheight_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_blc_base(vi_pipe) + 0x2a);
}

static __inline td_void ot_ext_system_isp_dynamic_blc_low_threshold_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_blc_base(vi_pipe) + 0x2c, data);
}

static __inline td_u16 ot_ext_system_isp_dynamic_blc_low_threshold_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_blc_base(vi_pipe) + 0x2c);
}

static __inline td_void ot_ext_system_isp_dynamic_blc_high_threshold_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_blc_base(vi_pipe) + 0x2e, data);
}

static __inline td_u16 ot_ext_system_isp_dynamic_blc_high_threshold_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_blc_base(vi_pipe) + 0x2e);
}

static __inline td_void ot_ext_system_isp_dynamic_blc_attr_updata_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_blc_base(vi_pipe) + 0x30, data);
}

static __inline td_u16 ot_ext_system_isp_dynamic_blc_attr_updata_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_blc_base(vi_pipe) + 0x30);
}

static __inline td_void ot_ext_system_isp_blc_init_status_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_blc_base(vi_pipe) + 0x32, (data & 0x01));
}

static __inline td_u8 ot_ext_system_isp_blc_init_status_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_blc_base(vi_pipe) + 0x32) & 0x01);
}

static __inline td_void ot_ext_system_isp_user_blc_en_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_blc_base(vi_pipe) + 0x33, (data & 0x01));
}

static __inline td_u8 ot_ext_system_isp_user_blc_en_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_blc_base(vi_pipe) + 0x33) & 0x01);
}

/* addr range BLC[0x34, 0x54) index:16 */
static __inline td_void ot_ext_system_isp_dynamic_blc_offset_updata_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct(vreg_blc_base(vi_pipe) + 0x34 + (index * sizeof(td_u16)), data);
}

static __inline td_s16 ot_ext_system_isp_dynamic_blc_offset_updata_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return iord_16direct(vreg_blc_base(vi_pipe) + 0x34 + (index * sizeof(td_u16)));
}

static __inline td_void ot_ext_system_isp_dynamic_blc_tolerance_updata_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_blc_base(vi_pipe) + 0x54, data);
}

static __inline td_u16 ot_ext_system_isp_dynamic_blc_tolerance_updata_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_blc_base(vi_pipe) + 0x54);
}

static __inline td_void ot_ext_system_isp_dynamic_blc_filter_strength_updata_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_blc_base(vi_pipe) + 0x56, data);
}

static __inline td_u16 ot_ext_system_isp_dynamic_blc_filter_strength_updata_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_blc_base(vi_pipe) + 0x56);
}

static __inline td_void ot_ext_system_isp_dynamic_blc_separate_en_updata_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_blc_base(vi_pipe) + 0x58, data);
}

static __inline td_u16 ot_ext_system_isp_dynamic_blc_separate_en_updata_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_blc_base(vi_pipe) + 0x58);
}

static __inline td_void ot_ext_system_isp_ag_cali_dynamic_blc_updata_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct(vreg_blc_base(vi_pipe) + 0x5a + (index * sizeof(td_u16)), data);
}

static __inline td_u16 ot_ext_system_isp_ag_cali_dynamic_blc_updata_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return iord_16direct(vreg_blc_base(vi_pipe) + 0x5a + (index * sizeof(td_u16)));
}

static __inline td_void ot_ext_system_isp_dynamic_blc_filter_thr_updata_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_blc_base(vi_pipe) + 0x7a, data);
}

static __inline td_u16 ot_ext_system_isp_dynamic_blc_filter_thr_updata_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_blc_base(vi_pipe) + 0x7a);
}

/* AE */
/* register: ot_ext_system_ae_sel */
/* AE sel */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_ae_be_sel_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe)), data);
}
static __inline td_u8 ot_ext_system_ae_be_sel_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_ae_base(vi_pipe));
}

/* register: ot_ext_system_ae_hist_offset_x */
/* AE hist offset X */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_ae_hist_offset_x_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0x1), data);
}
static __inline td_u8 ot_ext_system_ae_hist_offset_x_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0x1);
}

/* register: ot_ext_system_ae_hist_offset_y */
/* AE hist offset Y */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_ae_hist_offset_y_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0x2), data);
}
static __inline td_u8 ot_ext_system_ae_hist_offset_y_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0x2);
}

/* register: ot_ext_system_ae_hist_skip_x */
/* AE hist skip X */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_ae_hist_skip_x_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0x3), data);
}
static __inline td_u8 ot_ext_system_ae_hist_skip_x_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0x3);
}

/* register: ot_ext_system_ae_hist_skip_y */
/* AE hist skip Y */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_ae_hist_skip_y_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0x4), data);
}
static __inline td_u8 ot_ext_system_ae_hist_skip_y_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0x4);
}

/* register: ot_ext_system_ae_fourplanemode */
/* AE fourplanemode */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_ae_fourplanemode_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0x5), data);
}
static __inline td_u8 ot_ext_system_ae_fourplanemode_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0x5);
}

/* register: ot_ext_system_ae_histmode */
/* AE histmode */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_ae_histmode_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0x6), data);
}
static __inline td_u8 ot_ext_system_ae_histmode_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0x6);
}

/* register: ot_ext_system_ae_avermode */
/* AE avermode */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_ae_avermode_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0x7), data);
}
static __inline td_u8 ot_ext_system_ae_avermode_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0x7);
}

/* register: ot_ext_system_ae_maxgainmode */
/* AE maxgainmode */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_ae_maxgainmode_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0x8), data);
}
static __inline td_u8 ot_ext_system_ae_maxgainmode_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0x8);
}

/* register: ot_ext_system_ae_fe_en */
/* AE FE enable */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_ae_fe_en_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0xc), data);
}
static __inline td_u8 ot_ext_system_ae_fe_en_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0xc);
}

/* register: ot_ext_system_ae_be_en */
/* AE BE enable */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_ae_be_en_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0xd), data);
}
static __inline td_u8 ot_ext_system_ae_be_en_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0xd);
}

/* register: ot_ext_system_mg_en */
/* MG enable */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_mg_en_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0xe), data);
}
static __inline td_u8 ot_ext_system_mg_en_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0xe);
}

/* register: ot_ext_system_crop_en */
/* CROP enable */
#define OT_EXT_SYSTEM_CROP_EN_DEFAULT 0x0

/* args: data (8-bit) */
static __inline td_void ot_ext_system_ae_crop_en_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0xf), data);
}
static __inline td_u8 ot_ext_system_ae_crop_en_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0xf);
}

/* register: ot_ext_system_ae_crop_x */
/* AE CROP X */
#define OT_EXT_SYSTEM_AE_CROP_X_DEFAULT 0x0

/* args: data (16-bit) */
static __inline td_void ot_ext_system_ae_crop_x_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_ae_base(vi_pipe) + 0x10), data);
}
static __inline td_u32 ot_ext_system_ae_crop_x_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_ae_base(vi_pipe) + 0x10);
}

/* register: ot_ext_system_ae_crop_y */
/* AE CROP Y */
#define OT_EXT_SYSTEM_AE_CROP_Y_DEFAULT 0x0

/* args: data (16-bit) */
static __inline td_void ot_ext_system_ae_crop_y_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_ae_base(vi_pipe) + 0x14), data);
}
static __inline td_u16 ot_ext_system_ae_crop_y_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_ae_base(vi_pipe) + 0x14);
}

/* register: ot_ext_system_ae_crop_height */
/* AE CROP HEIGHT */
#define OT_EXT_SYSTEM_AE_CROP_HEIGHT_DEFAULT 0x0

/* args: data (16-bit) */
static __inline td_void ot_ext_system_ae_crop_height_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_ae_base(vi_pipe) + 0x18), data);
}
static __inline td_u16 ot_ext_system_ae_crop_height_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_ae_base(vi_pipe) + 0x18);
}

/* register: ot_ext_system_ae_crop_width */
/* AE CROP WIDTH */
#define OT_EXT_SYSTEM_AE_CROP_WIDTH_DEFAULT 0x0

/* args: data (16-bit) */
static __inline td_void ot_ext_system_ae_crop_width_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_ae_base(vi_pipe) + 0x1c), data);
}
static __inline td_u16 ot_ext_system_ae_crop_width_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_ae_base(vi_pipe) + 0x1c);
}

/* register: ot_ext_system_smart_enable */
/* smart enable */
#define OT_EXT_SYSTEM_SMART_ENABLE_DEFAULT 0x0

/* args: data (8-bit) */
/* addr range ae[0x20, 0x22) index:2 */
static __inline td_void ot_ext_system_smart_enable_write(ot_vi_pipe vi_pipe, td_u16 index, td_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0x20 + index), data);
}
static __inline td_u8 ot_ext_system_smart_enable_read(ot_vi_pipe vi_pipe, td_u16 index)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0x20 + index);
}

/* register: ot_ext_system_smart_available */
/* smart available */
#define OT_EXT_SYSTEM_SMART_AVAILABLE_DEFAULT 0x0

/* args: data (8-bit) */
/* addr range ae[0x30, 0x32) index:2 */
static __inline td_void ot_ext_system_smart_available_write(ot_vi_pipe vi_pipe, td_u16 index, td_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0x30 + index), data);
}
static __inline td_u8 ot_ext_system_smart_available_read(ot_vi_pipe vi_pipe, td_u16 index)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0x30 + index);
}

/* register: ot_ext_system_smart_luma */
/* smart luma */
#define OT_EXT_SYSTEM_SMART_LUMA_DEFAULT 0x0

/* args: data (8-bit) */
/* addr range ae[0x40, 0x42) index:2 */
static __inline td_void ot_ext_system_smart_luma_write(ot_vi_pipe vi_pipe, td_u16 index, td_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0x40 + index), data);
}
static __inline td_u8 ot_ext_system_smart_luma_read(ot_vi_pipe vi_pipe, td_u16 index)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0x40 + index);
}

/* register: ot_ext_system_smart_update */
/* smart update */
#define OT_EXT_SYSTEM_SMART_UPDATE_DEFAULT 0x0

/* args: data (8-bit) */
static __inline td_void ot_ext_system_smart_update_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0x50), data);
}
static __inline td_u8 ot_ext_system_smart_update_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0x50);
}

/* ------------------------------------------------------------------------------  */
/* register: ot_ext_system_tunnel_enable */
/* ------------------------------------------------------------------------------  */
/* tunnel enable */
/* ------------------------------------------------------------------------------  */
#define OT_EXT_SYSTEM_TUNNEL_ENABLE_DEFAULT 0x0

/* args: data (8-bit) */
/* addr range AE[0x60, 0x62) index:2 */
static __inline td_void ot_ext_system_tunnel_enable_write(ot_vi_pipe vi_pipe, td_u16 index, td_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0x60 + index), data);
}
static __inline td_u8 ot_ext_system_tunnel_enable_read(ot_vi_pipe vi_pipe, td_u16 index)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0x60 + index);
}

/* ------------------------------------------------------------------------------  */
/* register: ot_ext_system_tunnel_available */
/* ------------------------------------------------------------------------------  */
/* tunnel available */
/* ------------------------------------------------------------------------------  */
#define OT_EXT_SYSTEM_TUNNEL_AVAILABLE_DEFAULT 0x0

/* args: data (8-bit) */
/* addr range AE[0x64, 0x66) index:2 */
static __inline td_void ot_ext_system_tunnel_available_write(ot_vi_pipe vi_pipe, td_u16 index, td_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0x64 + index), data);
}
static __inline td_u8 ot_ext_system_tunnel_available_read(ot_vi_pipe vi_pipe, td_u16 index)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0x64 + index);
}

/* ------------------------------------------------------------------------------  */
/* register: ot_ext_system_tunnel_pix_ratio */
/* ------------------------------------------------------------------------------  */
/* tunnel pix ratio */
/* ------------------------------------------------------------------------------  */
#define OT_EXT_SYSTEM_TUNNEL_AREA_RATIO_DEFAULT 0x0

/* args: data (32-bit) */
/* addr range AE[0x70, 0x78) index:2 */
static __inline td_void ot_ext_system_tunnel_area_ratio_write(ot_vi_pipe vi_pipe, td_u16 index, td_u32 data)
{
    iowr_32direct((vreg_ae_base(vi_pipe) + 0x70 + (index << 0x2)), data);
}
static __inline td_u32 ot_ext_system_tunnel_area_ratio_read(ot_vi_pipe vi_pipe, td_u16 index)
{
    return iord_32direct(vreg_ae_base(vi_pipe) + 0x70 + (index << 0x2));
}

/* ------------------------------------------------------------------------------  */
/* register: ot_ext_system_tunnel_value */
/* ------------------------------------------------------------------------------  */
/* tunnel value */
/* ------------------------------------------------------------------------------  */
#define OT_EXT_SYSTEM_TUNNEL_EXP_PERF_DEFAULT 0x0

/* args: data (32-bit) */
/* addr range AE[0x80, 0x88) index:2 */
/* addr range AE[0x78, 0x80) index:2 */
static __inline td_void ot_ext_system_tunnel_exp_perf_write(ot_vi_pipe vi_pipe, td_u16 index, td_u32 data)
{
    iowr_32direct((vreg_ae_base(vi_pipe) + 0x78 + (index << 0x2)), data);
}
static __inline td_u32 ot_ext_system_tunnel_exp_perf_read(ot_vi_pipe vi_pipe, td_u16 index)
{
    return iord_32direct(vreg_ae_base(vi_pipe) + 0x78 + (index << 0x2));
}

/* ------------------------------------------------------------------------------  */
/* register: ot_ext_system_tunnel_update */
/* ------------------------------------------------------------------------------  */
/* tunnel update */
/* ------------------------------------------------------------------------------  */
#define OT_EXT_SYSTEM_TUNNEL_UPDATE_DEFAULT 0x0

/* args: data (8-bit) */
static __inline td_void ot_ext_system_tunnel_update_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0x80), data);
}
static __inline td_u8 ot_ext_system_tunnel_update_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0x80);
}

/* register: ot_ext_system_crop_en */
/* CROP enable */
#define OT_EXT_SYSTEM_FE_CROP_EN_DEFAULT 0x0

/* args: data (8-bit) */
static __inline td_void ot_ext_system_ae_fe_crop_en_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0x81), data);
}
static __inline td_u8 ot_ext_system_ae_fe_crop_en_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0x81);
}

/* register: ot_ext_system_ae_crop_x */
/* AE CROP X */
#define OT_EXT_SYSTEM_AE_FE_CROP_X_DEFAULT 0x0

/* args: data (16-bit) */
static __inline td_void ot_ext_system_ae_fe_crop_x_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_ae_base(vi_pipe) + 0x82), data);
}
static __inline td_u32 ot_ext_system_ae_fe_crop_x_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_ae_base(vi_pipe) + 0x82);
}

/* register: ot_ext_system_ae_crop_y */
/* AE CROP Y */
#define OT_EXT_SYSTEM_AE_FE_CROP_Y_DEFAULT 0x0

/* args: data (16-bit) */
static __inline td_void ot_ext_system_ae_fe_crop_y_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_ae_base(vi_pipe) + 0x84), data);
}
static __inline td_u16 ot_ext_system_ae_fe_crop_y_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_ae_base(vi_pipe) + 0x84);
}

/* register: ot_ext_system_ae_crop_height */
/* AE CROP HEIGHT */
#define OT_EXT_SYSTEM_AE_FE_CROP_HEIGHT_DEFAULT 0x0

/* args: data (16-bit) */
static __inline td_void ot_ext_system_ae_fe_crop_height_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_ae_base(vi_pipe) + 0x86), data);
}
static __inline td_u16 ot_ext_system_ae_fe_crop_height_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_ae_base(vi_pipe) + 0x86);
}

/* register: ot_ext_system_ae_crop_width */
/* AE CROP WIDTH */
#define OT_EXT_SYSTEM_AE_FE_CROP_WIDTH_DEFAULT 0x0

/* args: data (16-bit) */
static __inline td_void ot_ext_system_ae_fe_crop_width_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_ae_base(vi_pipe) + 0x88), data);
}
static __inline td_u16 ot_ext_system_ae_fe_crop_width_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_ae_base(vi_pipe) + 0x88);
}

/* register: ot_ext_system_fast_face_frame_pts */
/* fast face frame pts */
#define OT_EXT_SYSTEM_FACE_FRAME_PTS_DEFAULT 0x0
/* args: data (64-bit) */
static __inline td_void ot_ext_system_fast_face_frame_pts_write(ot_vi_pipe vi_pipe, td_u64 data)
{
    iowr_32direct((vreg_ae_base(vi_pipe) + 0x94), (td_u32)(data >> 0x20));
    iowr_32direct((vreg_ae_base(vi_pipe) + 0x98), (td_u32)((td_s64)data - ((data >> 0x20) << 0x20)));
}
static __inline td_u64 ot_ext_system_fast_face_frame_pts_read(ot_vi_pipe vi_pipe)
{
    return ((td_u64)iord_32direct(vreg_ae_base(vi_pipe) + 0x94) << 0x20) + iord_32direct(vreg_ae_base(vi_pipe) + 0x98);
}
/* register: ot_ext_system_smart_frame_pts */
/* smart frame pts */
#define OT_EXT_SYSTEM_FAST_FACE_ENABLE 0x0
/* args: data (8-bit) */
static __inline td_void ot_ext_system_fast_face_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0x9c), data);
}
static __inline td_u8 ot_ext_system_fast_face_enable_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0x9c);
}
/* register: ot_ext_system_fast_face_available */
#define OT_EXT_SYSTEM_FAST_FACE_AVAILABLE 0x0
/* fast face available */
static __inline td_void ot_ext_system_fast_face_available_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0x9d), data);
}
static __inline td_u8 ot_ext_system_fast_face_available_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0x9d);
}
/* register: ot_ext_system_fast_face_x */
#define OT_EXT_SYSTEM_FAST_FACE_X_DEFAULT 0x0
/* fast face x */
static __inline td_void ot_ext_system_fast_face_x_write(ot_vi_pipe vi_pipe, td_u16 index, td_u32 data)
{
    iowr_16direct((vreg_ae_base(vi_pipe) + 0x9e + (index << 1)), data & 0xFFFF);
}
static __inline td_s32 ot_ext_system_fast_face_x_read(ot_vi_pipe vi_pipe, td_u16 index)
{
    return (td_s32)iord_16direct(vreg_ae_base(vi_pipe) + 0x9e + (index << 1));
}
/* register: ot_ext_system_fast_face_y */
#define OT_EXT_SYSTEM_FAST_FACE_Y_DEFAULT 0x0
/* fast face y */
static __inline td_void ot_ext_system_fast_face_y_write(ot_vi_pipe vi_pipe, td_u16 index, td_u32 data)
{
    iowr_16direct((vreg_ae_base(vi_pipe) + 0xa8 + (index << 1)), data & 0xFFFF);
}
static __inline td_s32 ot_ext_system_fast_face_y_read(ot_vi_pipe vi_pipe, td_u16 index)
{
    return (td_s32)iord_16direct(vreg_ae_base(vi_pipe) + 0xa8 + (index << 1));
}
/* register: ot_ext_system_fast_face_width */
#define OT_EXT_SYSTEM_FAST_FACE_WIDTH_DEFAULT 0x0
/* fast face width */
static __inline td_void ot_ext_system_fast_face_width_write(ot_vi_pipe vi_pipe, td_u16 index, td_u32 data)
{
    iowr_16direct((vreg_ae_base(vi_pipe) + 0xb2 + (index << 1)), data & 0xFFFF);
}
static __inline td_u32 ot_ext_system_fast_face_width_read(ot_vi_pipe vi_pipe, td_u16 index)
{
    return (td_u32)iord_16direct(vreg_ae_base(vi_pipe) + 0xb2 + (index << 1));
}
/* register: ot_ext_system_smart_height */
#define OT_EXT_SYSTEM_FAST_FACE_HEIGHT_DEFAULT 0x0
/* fast face height */
static __inline td_void ot_ext_system_fast_face_height_write(ot_vi_pipe vi_pipe, td_u16 index, td_u32 data)
{
    iowr_16direct((vreg_ae_base(vi_pipe) + 0xbc + (index << 1)), data & 0xFFFF);
}
static __inline td_u32 ot_ext_system_fast_face_height_read(ot_vi_pipe vi_pipe, td_u32 index)
{
    return (td_u32)iord_16direct(vreg_ae_base(vi_pipe) + 0xbc + (index << 1));
}

/* register: ot_ext_system_ae_weight_table */
/* AE weight table */
#define OT_EXT_SYSTEM_AE_WEIGHT_TABLE_DEFAULT 0x0

/* args: data (8-bit), addr range: AE:[0x100,0x200),index: 256 */
static __inline td_void ot_ext_system_ae_weight_table_write(ot_vi_pipe vi_pipe, td_u16 index, td_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0x100 + index), data);
}
static __inline td_u8 ot_ext_system_ae_weight_table_read(ot_vi_pipe vi_pipe, td_u16 index)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0x100 + index);
}

/* DPC */
/* register: ot_ext_system_dpc_static_cor_enable */
/* defect pixel static correction enable */
#define OT_EXT_SYSTEM_DPC_STATIC_COR_ENABLE_DEFAULT 0x01

/* dpc_static_cor_enable */
static __inline td_void ot_ext_system_dpc_static_cor_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_dpc_base(vi_pipe), (data & 0x1));
}
static __inline td_u8 ot_ext_system_dpc_static_cor_enable_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_dpc_base(vi_pipe)) & 0x1);
}

/* register: ot_ext_system_dpc_static_calib_enable */
/* defect pixel static correction enable */
#define OT_EXT_SYSTEM_DPC_STATIC_CALIB_ENABLE_DEFAULT 0x0

/* dpc_static_calib_enable */
static __inline td_void ot_ext_system_dpc_static_calib_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_dpc_base(vi_pipe) + 1, (data & 0x1));
}
static __inline td_u8 ot_ext_system_dpc_static_calib_enable_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_dpc_base(vi_pipe) + 1) & 0x1);
}

/* register: ot_ext_system_dpc_defect_type */
/* ISP_STATIC_DP_BRIGHT:0x0, ISP_STATIC_DP_DARK:0x1 */
#define OT_EXT_SYSTEM_DPC_STATIC_DEFECT_TYPE_DEFAULT 0x0

/* static_dp_type */
static __inline td_void ot_ext_system_dpc_static_defect_type_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_dpc_base(vi_pipe) + 0x2, (data & 0x1));
}
static __inline td_u8 ot_ext_system_dpc_static_defect_type_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_dpc_base(vi_pipe) + 0x2) & 0x1);
}

/* register: ot_ext_system_dpc_start_thresh */
/* define the start thresh of defect pixel calibration */
#define OT_EXT_SYSTEM_DPC_START_THRESH_DEFAULT 0x3

/* start_thresh */
static __inline td_void ot_ext_system_dpc_start_thresh_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_dpc_base(vi_pipe) + 0x3), data);
}
static __inline td_u8 ot_ext_system_dpc_start_thresh_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dpc_base(vi_pipe) + 0x3);
}

/* register: ot_ext_system_dpc_count_max */
/* bad pixel calibration  the maxmum bad pixel count */
#define OT_EXT_SYSTEM_DPC_COUNT_MAX_DEFAULT 0x400

/* count_max */
static __inline td_void ot_ext_system_dpc_count_max_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_dpc_base(vi_pipe) + 0x4, data);
}
static __inline td_u16 ot_ext_system_dpc_count_max_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_dpc_base(vi_pipe) + 0x4);
}

/* register: ot_ext_system_dpc_count_min */
/* bad pixel calibration  the minimum bad pixel count */
#define OT_EXT_SYSTEM_DPC_COUNT_MIN_DEFAULT 0x1

/* count_min */
static __inline td_void ot_ext_system_dpc_count_min_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_dpc_base(vi_pipe) + 0x6, data);
}
static __inline td_u16 ot_ext_system_dpc_count_min_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_dpc_base(vi_pipe) + 0x6);
}

/* register: ot_ext_system_dpc_trigger_time */
/* the bad pixel trigger frame cnt of time out */
#define OT_EXT_SYSTEM_DPC_TRIGGER_TIME_DEFAULT 0x640
static __inline td_void ot_ext_system_dpc_trigger_time_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_dpc_base(vi_pipe) + 0x8, data);
}
static __inline td_u16 ot_ext_system_dpc_trigger_time_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_dpc_base(vi_pipe) + 0x8);
}

/* register: ot_ext_system_dpc_finish_thresh */
/* the finish thresh of defect pixel calibration */
/* finish_thresh */
static __inline td_void ot_ext_system_dpc_finish_thresh_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_dpc_base(vi_pipe) + 0xa, data);
}
static __inline td_u8 ot_ext_system_dpc_finish_thresh_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dpc_base(vi_pipe) + 0xa);
}

/* register: ot_ext_system_dpc_trigger_status */
/* the static bad pixel trigger status */
/* 0:initial status */
/* 1:finished */
/* 2:time out */
#define OT_EXT_SYSTEM_DPC_TRIGGER_STATUS_DEFAULT 0x0

static __inline td_void ot_ext_system_dpc_trigger_status_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_dpc_base(vi_pipe) + 0xb, (data & 0x3));
}
static __inline td_u8 ot_ext_system_dpc_trigger_status_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_dpc_base(vi_pipe) + 0xb) & 0x3);
}

/* register: ot_ext_system_dpc_static_dp_show */
/* highlight static defect pixel */
#define OT_EXT_SYSTEM_DPC_STATIC_DP_SHOW_DEFAULT 0x0

static __inline td_void ot_ext_system_dpc_static_dp_show_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_dpc_base(vi_pipe) + 0xc, (data & 0x1));
}
static __inline td_u8 ot_ext_system_dpc_static_dp_show_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_dpc_base(vi_pipe) + 0xc) & 0x1);
}

static __inline td_void ot_ext_system_isp_dpc_init_status_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_dpc_base(vi_pipe) + 0xd, (data & 0x1));
}

static __inline td_u8 ot_ext_system_isp_dpc_init_status_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_dpc_base(vi_pipe) + 0xd) & 0x1);
}

/* register: ot_ext_system_dpc_bpt_cor_number by user config */
/* ot_ext_system_dpc_bpt_cor_number */
#define OT_EXT_SYSTEM_DPC_BPT_COR_NUMBER_DEFAULT 0x0
static __inline td_void ot_ext_system_dpc_bpt_cor_number_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_dpc_base(vi_pipe) + 0xe, data);
}
static __inline td_u16 ot_ext_system_dpc_bpt_cor_number_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_dpc_base(vi_pipe) + 0xe));
}

/* register: ot_ext_system_dpc_bpt_calib_number */
/* ot_ext_system_dpc_bpt_calib_number */
#define OT_EXT_SYSTEM_DPC_BPT_CALIB_NUMBER_DEFAULT 0x0
static __inline td_void ot_ext_system_dpc_bpt_calib_number_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_dpc_base(vi_pipe) + 0x10, data);
}
static __inline td_u16 ot_ext_system_dpc_bpt_calib_number_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_dpc_base(vi_pipe) + 0x10));
}

/* register: ot_ext_system_dpc_static_attr_updata */
/* ot_ext_system_dpc_static_attr_updata */
#define OT_EXT_SYSTEM_DPC_STATIC_ATTR_UPDATE_DEFAULT 0x0
static __inline td_void ot_ext_system_dpc_static_attr_update_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_dpc_base(vi_pipe) + 0x12, (data & 0x1));
}
static __inline td_u8 ot_ext_system_dpc_static_attr_update_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_dpc_base(vi_pipe) + 0x12) & 0x1);
}

static __inline td_void ot_ext_system_dpc_dynamic_attr_update_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_dpc_base(vi_pipe) + 0x13, (data & 0x1));
}
static __inline td_u8 ot_ext_system_dpc_dynamic_attr_update_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_dpc_base(vi_pipe) + 0x13) & 0x1);
}

/* register: ot_ext_system_dpc_dynamic_cor_enable */
/* defect pixel dynamic correction enable */
#define OT_EXT_SYSTEM_DPC_DYNAMIC_COR_ENABLE_DEFAULT 0x01

/* dynamic_cor_enable */
static __inline td_void ot_ext_system_dpc_dynamic_cor_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_dpc_base(vi_pipe) + 0x14, (data & 0x1));
}
static __inline td_u8 ot_ext_system_dpc_dynamic_cor_enable_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_dpc_base(vi_pipe) + 0x14) & 0x1);
}

/* register: ot_ext_system_dpc_suppress_twinkle_enable_write */
/* ot_ext_system_dpc_suppress_twinkle_enable */
/* addr range dpc[0x16, 0x1a) index: OT_ISP_WDR_MAX_FRAME_NUM */
#define OT_EXT_SYSTEM_DPC_SUPPRESS_TWINKLE_ENABLE_DEFAULT 0x0
static __inline td_void ot_ext_system_dpc_suppress_twinkle_enable_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct(vreg_dpc_base(vi_pipe) + 0x16 + index * sizeof(td_u8), (data & 0x1));
}
static __inline td_u8 ot_ext_system_dpc_suppress_twinkle_enable_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct(vreg_dpc_base(vi_pipe) + 0x16 + index * sizeof(td_u8)) & 0x1);
}

/* register: ot_ext_system_dpc_suppress_twinkle_thr_write */
/* ot_ext_system_dpc_suppress_twinkle_thr */
/* addr range dpc[0x1a, 0x1e) index: OT_ISP_WDR_MAX_FRAME_NUM */
#define OT_EXT_SYSTEM_DPC_SUPPRESS_TWINKLE_THR_DEFAULT 0x6
static __inline td_void ot_ext_system_dpc_suppress_twinkle_thr_write(ot_vi_pipe vi_pipe, td_u8 index, td_s8 data)
{
    iowr_8direct(vreg_dpc_base(vi_pipe) + 0x1a + index * sizeof(td_u8), data);
}
static __inline td_s8 ot_ext_system_dpc_suppress_twinkle_thr_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return iord_8direct(vreg_dpc_base(vi_pipe) + 0x1a + index * sizeof(td_u8));
}

/* register: ot_ext_system_dpc_suppress_twinkle_slope_write */
/* ot_ext_system_dpc_suppress_twinkle_slope */
/* addr range dpc[0x1e, 0x22) index: OT_ISP_WDR_MAX_FRAME_NUM */
#define OT_EXT_SYSTEM_DPC_SUPPRESS_TWINKLE_SLOPE_DEFAULT 0x15
static __inline td_void ot_ext_system_dpc_suppress_twinkle_slope_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct(vreg_dpc_base(vi_pipe) + 0x1e + index * sizeof(td_u8), data);
}
static __inline td_u8 ot_ext_system_dpc_suppress_twinkle_slope_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return iord_8direct(vreg_dpc_base(vi_pipe) + 0x1e + index * sizeof(td_u8));
}

/* register: ot_ext_system_dpc_dynamic_manual_enable */
/* dynamic_manual_enable */
/* addr range dpc[0x22, 0x26) index: OT_ISP_WDR_MAX_FRAME_NUM */
#define OT_EXT_SYSTEM_DPC_DYNAMIC_MANUAL_ENABLE_DEFAULT 0x0
static __inline td_void ot_ext_system_dpc_dynamic_manual_enable_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct(vreg_dpc_base(vi_pipe) + 0x22 + index * sizeof(td_u8), (data & 0x1));
}
static __inline td_u8 ot_ext_system_dpc_dynamic_manual_enable_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct(vreg_dpc_base(vi_pipe) + 0x22 + index * sizeof(td_u8)) & 0x1);
}

/* register: ot_ext_system_dpc_dynamic_strength */
/* manual dpc_dynamic_strength */
/* addr range dpc[0x27, 0x2b) index: OT_ISP_WDR_MAX_FRAME_NUM */
#define OT_EXT_SYSTEM_DPC_DYNAMIC_STRENGTH_DEFAULT 0x0
static __inline td_void ot_ext_system_dpc_dynamic_strength_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct(vreg_dpc_base(vi_pipe) + 0x27 + index * sizeof(td_u8), data);
}

static __inline td_u8 ot_ext_system_dpc_dynamic_strength_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return iord_8direct(vreg_dpc_base(vi_pipe) + 0x27 + index * sizeof(td_u8));
}

/* register: ot_ext_system_dpc_dynamic_blend_ratio_table */
/* manual dpc_blend_ratio */
/* addr range dpc[0x2b, 0x2f) index: OT_ISP_WDR_MAX_FRAME_NUM */
#define OT_EXT_SYSTEM_DPC_DYNAMIC_BLEND_RATIO_DEFAULT 0x0
static __inline td_void ot_ext_system_dpc_dynamic_blend_ratio_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct(vreg_dpc_base(vi_pipe) + 0x2b + index * sizeof(td_u8), data);
}

static __inline td_u8 ot_ext_system_dpc_dynamic_blend_ratio_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return iord_8direct(vreg_dpc_base(vi_pipe) + 0x2b + index * sizeof(td_u8));
}

/* register: ot_ext_system_dpc_dynamic_strength_table */
/* dpc_dynamic_strength_table */
/* addr range dpc[0x30, 0x70), index: OT_ISP_WDR_MAX_FRAME_NUM * 16; index:0-15, short; index:16-31, long */
static __inline td_void ot_ext_system_dpc_dynamic_strength_table_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct(vreg_dpc_base(vi_pipe) + 0x30 + index * sizeof(td_u8), data);
}

static __inline td_u8 ot_ext_system_dpc_dynamic_strength_table_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return iord_8direct(vreg_dpc_base(vi_pipe) + 0x30 + index * sizeof(td_u8));
}

/* register: ot_ext_system_dpc_dynamic_blend_ratio_table */
/* dpc_blend_ratio_table */
/* addr range dpc[0x7a, 0xb0), index: OT_ISP_WDR_MAX_FRAME_NUM * 16; index:0-15, short; index:16-31, long */
static __inline td_void ot_ext_system_dpc_dynamic_blend_ratio_table_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct(vreg_dpc_base(vi_pipe) + 0x70 + index * sizeof(td_u8), data);
}

static __inline td_u8 ot_ext_system_dpc_dynamic_blend_ratio_table_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return iord_8direct(vreg_dpc_base(vi_pipe) + 0x70 + index * sizeof(td_u8));
}

/* addr range dpc[0xb0, 0xb4) index: OT_ISP_WDR_MAX_FRAME_NUM */
#define OT_EXT_SYSTEM_DPC_BRIGHT_STRENGTH_DEFAULT 0x60
static __inline td_void ot_ext_system_dpc_bright_strength_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct(vreg_dpc_base(vi_pipe) + 0xb0 + index * sizeof(td_u8), data);
}
static __inline td_u8 ot_ext_system_dpc_bright_strength_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return iord_8direct(vreg_dpc_base(vi_pipe) + 0xb0 + index * sizeof(td_u8));
}

/* addr range dpc[0xb4, 0xb8) index: OT_ISP_WDR_MAX_FRAME_NUM */
#define OT_EXT_SYSTEM_DPC_DARK_STRENGTH_DEFAULT 0x3F
static __inline td_void ot_ext_system_dpc_dark_strength_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct(vreg_dpc_base(vi_pipe) + 0xb4 + index * sizeof(td_u8), data);
}
static __inline td_u8 ot_ext_system_dpc_dark_strength_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return iord_8direct(vreg_dpc_base(vi_pipe) + 0xb4 + index * sizeof(td_u8));
}

#ifdef CONFIG_OT_ISP_DPC_STATIC_TABLE_SUPPORT
/* DPC table */
/* register: ot_ext_system_dpc_calib_bpt */
/* ot_ext_system_dpc_calib_bpt */
static __inline td_void ot_ext_system_dpc_calib_bpt_write(ot_vi_pipe vi_pipe, td_u16 index, td_u32 data)
{
    iowr_32direct(vreg_dpc_base(vi_pipe) + 0xC0 + index * sizeof(td_u32), data);
}

static __inline td_u32 ot_ext_system_dpc_calib_bpt_read(ot_vi_pipe vi_pipe,  td_u16 index)
{
    return iord_32direct(vreg_dpc_base(vi_pipe) + 0xC0 + index * sizeof(td_u32));
}

/* register: ot_ext_system_dpc_cor_bpt */
/* ot_ext_system_dpc_cor_bpt */
static __inline td_void ot_ext_system_dpc_cor_bpt_write(ot_vi_pipe vi_pipe, td_u16 index, td_u32 data)
{
    iowr_32direct(vreg_dpc_base(vi_pipe) + 0x60C0 + index * sizeof(td_u32), data);
}

static __inline td_u32 ot_ext_system_dpc_cor_bpt_read(ot_vi_pipe vi_pipe,  td_u16 index)
{
    return iord_32direct(vreg_dpc_base(vi_pipe) + 0x60C0 + index * sizeof(td_u32));
}
#endif

/* GE */
/* register: ot_ext_system_ge_npoffset */
/* ge npoffset table */
/* args: data (16-bit) */
/* addr range ge[0x0, 0x20) index:16 */
static __inline td_void ot_ext_system_ge_npoffset_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct(vreg_ge_base(vi_pipe)  + index * sizeof(td_u16), data);
}

static __inline td_u16 ot_ext_system_ge_npoffset_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return iord_16direct(vreg_ge_base(vi_pipe)  +  index * sizeof(td_u16));
}

/* register: ot_ext_system_ge_threshold_ */
/* ge threshold_ table */
#define OT_EXT_SYSTEM_GE_THRESHOLD_DEFAULT 0
/* args: data (16-bit) */
/* addr range ge[0x20, 0x40) index:16 */
static __inline td_void ot_ext_system_ge_threshold_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct(vreg_ge_base(vi_pipe)  +  0x20  + (index * sizeof(td_u16)), data);
}

static __inline td_u16 ot_ext_system_ge_threshold_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return iord_16direct(vreg_ge_base(vi_pipe)  +  0x20  + (index * sizeof(td_u16)));
}

/* register: ot_ext_system_ge_strength_ */
/* ge strength_ table */
/* addr range ge[0x40, 0x60) index:16 */
static __inline td_void ot_ext_system_ge_strength_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct(vreg_ge_base(vi_pipe)  +  0x40  + index * sizeof(td_u16), data);
}

static __inline td_u16 ot_ext_system_ge_strength_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return iord_16direct(vreg_ge_base(vi_pipe)  +  0x40   +  index * sizeof(td_u16));
}

/* ge_slope */
static __inline td_void ot_ext_system_ge_slope_write(ot_vi_pipe vi_pipe,  td_u16 data)
{
    iowr_16direct(vreg_ge_base(vi_pipe)  +  0x60, data);
}

static __inline td_u16 ot_ext_system_ge_slope_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_ge_base(vi_pipe)  +  0x60);
}

/* ge_sensitivity */
static __inline td_void ot_ext_system_ge_sensitivity_write(ot_vi_pipe vi_pipe,  td_u16 data)
{
    iowr_16direct(vreg_ge_base(vi_pipe)  +  0x62, data);
}

static __inline td_u16 ot_ext_system_ge_sensitivity_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_ge_base(vi_pipe)  +  0x62);
}

/* ge_sensithreshold */
static __inline td_void ot_ext_system_ge_sensithreshold_write(ot_vi_pipe vi_pipe,  td_u16 data)
{
    iowr_16direct(vreg_ge_base(vi_pipe)  +  0x64, data);
}

static __inline td_u16 ot_ext_system_ge_sensithreshold_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_ge_base(vi_pipe)  +  0x64);
}

/* ge_enable */
static __inline td_void ot_ext_system_ge_enable_write(ot_vi_pipe vi_pipe,  td_u8 data)
{
    iowr_8direct(vreg_ge_base(vi_pipe)  +  0x66, data & 0x1);
}

static __inline td_u8 ot_ext_system_ge_enable_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_ge_base(vi_pipe)  +  0x66) & 0x1);
}

static __inline td_void ot_ext_system_ge_coef_update_en_write(ot_vi_pipe vi_pipe,  td_u8 data)
{
    iowr_8direct(vreg_ge_base(vi_pipe)  +  0x67, data & 0x1);
}

static __inline td_u8 ot_ext_system_ge_coef_update_en_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_ge_base(vi_pipe)  +  0x67) & 0x1);
}

static __inline td_void ot_ext_system_isp_ge_init_status_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_ge_base(vi_pipe) + 0x68, (data & 0x01));
}

static __inline td_u8 ot_ext_system_isp_ge_init_status_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_ge_base(vi_pipe) + 0x68) & 0x01);
}

static __inline td_void ot_ext_system_ge_filter_mode_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct(vreg_ge_base(vi_pipe)  +  0x69  + index * sizeof(td_u8), data);
}

static __inline td_u8 ot_ext_system_ge_filter_mode_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return iord_8direct(vreg_ge_base(vi_pipe)  +  0x69   +  index * sizeof(td_u8));
}

static __inline td_void ot_ext_system_isp_demosaic_init_status_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_dm_base(vi_pipe), (data & 0x01));
}

static __inline td_u8 ot_ext_system_isp_demosaic_init_status_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_dm_base(vi_pipe)) & 0x01);
}

/* register: ot_ext_system_demosaic_manual_mode_write */
/* args: data (8-bit) */
#define OT_EXT_SYSTEM_DEMOSAIC_MANUAL_MODE_DEFAULT 0
static __inline td_void ot_ext_system_demosaic_manual_mode_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_dm_base(vi_pipe) + 0x10, data & 0x1);
}
static __inline td_u8 ot_ext_system_demosaic_manual_mode_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_dm_base(vi_pipe) + 0x10) & 0x1);
}

/* register: ot_ext_system_demosaic_enable_write */
/* args: data (8-bit) */
#define OT_EXT_SYSTEM_DEMOSAIC_ENABLE_DEFAULT 0x1
#define OT_EXT_SYSTEM_DEMOSAIC_ENABLE_DATASIZE 1
static __inline td_void ot_ext_system_demosaic_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_dm_base(vi_pipe) + 0x11, data & 0x1);
}
static __inline td_u8 ot_ext_system_demosaic_enable_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_dm_base(vi_pipe) + 0x11) & 0x1);
}

/* register: ot_ext_system_demosaic_attr_update_en_write */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_demosaic_attr_update_en_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_dm_base(vi_pipe) + 0x12, data & 0x1);
}
static __inline td_u8 ot_ext_system_demosaic_attr_update_en_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_dm_base(vi_pipe) + 0x12) & 0x1);
}

/* register: ot_ext_system_demosaic_manual_nondirection_midfreq_detailenhance_strength_write */
/* args: data (8-bit) */
#define OT_EXT_SYSTEM_DEMOSAIC_MANUAL_NDDM_MF_EHNC_STR_LINDFT  8
#define OT_EXT_SYSTEM_DEMOSAIC_MANUAL_NDDM_MF_EHNC_STR_WDRDFT  0
static __inline td_void ot_ext_system_demosaic_manual_nddm_mfehnc_str_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_dm_base(vi_pipe) + 0x13, data);
}
static __inline td_u8 ot_ext_system_demosaic_manual_nddm_mfehnc_str_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dm_base(vi_pipe) + 0x13);
}

/* register: ot_ext_system_demosaic_manual_nondirection_higfreq_detailenhance_strength_write */
/* args: data (8-bit) */
#define OT_EXT_SYSTEM_DEMOSAIC_MANUAL_NDDM_HF_EHNC_STR_LINDFT  3
#define OT_EXT_SYSTEM_DEMOSAIC_MANUAL_NDDM_HF_EHNC_STR_WDRDFT  0
static __inline td_void ot_ext_system_demosaic_manual_nddm_hfehnc_str_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_dm_base(vi_pipe) + 0x14, data);
}
static __inline td_u8 ot_ext_system_demosaic_manual_nddm_hfehnc_str_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dm_base(vi_pipe) + 0x14);
}

/* register: ot_ext_system_demosaic_manual_nondirection_strength_write */
/* args: data (8-bit) */
#define OT_EXT_SYSTEM_DEMOSAIC_MANUAL_NDDM_STR_LINDFT 64
#define OT_EXT_SYSTEM_DEMOSAIC_MANUAL_NDDM_STR_WDRDFT 64
static __inline td_void ot_ext_system_demosaic_manual_nddm_str_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_dm_base(vi_pipe) + 0x15, data);
}
static __inline td_u8 ot_ext_system_demosaic_manual_nddm_str_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dm_base(vi_pipe) + 0x15);
}

/* register: ot_ext_system_demosaic_manual_detail_smooth_range_write */
/* args: data (8-bit) */
#define OT_EXT_SYSTEM_DEMOSAIC_MANUAL_DTLSMOOTH_RANGE_LINDFT 2
#define OT_EXT_SYSTEM_DEMOSAIC_MANUAL_DTLSMOOTH_RANGE_WDRDFT 2
static __inline td_void ot_ext_system_demosaic_manual_dtlsmooth_range_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_dm_base(vi_pipe) + 0x16, data);
}
static __inline td_u8 ot_ext_system_demosaic_manual_dtlsmooth_range_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dm_base(vi_pipe) + 0x16);
}

/* register: ot_ext_system_demosaic_manual_detail_smooth_strength_write */
/* args: data (16-bit) */
#define OT_EXT_SYSTEM_DEMOSAIC_MANUAL_DTLSMOOTH_STR_LINDFT 256
#define OT_EXT_SYSTEM_DEMOSAIC_MANUAL_DTLSMOOTH_STR_WDRDFT 256
static __inline td_void ot_ext_system_demosaic_manual_dtlsmooth_str_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_dm_base(vi_pipe) + 0x18, data);
}
static __inline td_u16 ot_ext_system_demosaic_manual_dtlsmooth_str_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_dm_base(vi_pipe) + 0x18);
}

/* register: ot_ext_system_demosaic_auto_nondirection_midfreq_detailenhance_strength_write */
/* args: data (8-bit) */
/* addr range dm[0x20, 0x30) index:16 */
static __inline td_void ot_ext_system_demosaic_auto_nddm_mfehnc_str_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct(vreg_dm_base(vi_pipe) + 0x20 + (index * sizeof(td_u8)), data);
}
static __inline td_u8 ot_ext_system_demosaic_auto_nddm_mfehnc_str_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return iord_8direct(vreg_dm_base(vi_pipe) + 0x20 + (index * sizeof(td_u8)));
}

/* register: ot_ext_system_demosaic_auto_detail_smooth_range_write */
/* args: data (8-bit) */
/* addr range dm[0x30, 0x40) index:16 */
static __inline td_void ot_ext_system_demosaic_auto_nddm_hfehnc_str_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct(vreg_dm_base(vi_pipe) + 0x30 + (index * sizeof(td_u8)), data);
}
static __inline td_u8 ot_ext_system_demosaic_auto_nddm_hfehnc_str_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return iord_8direct(vreg_dm_base(vi_pipe) + 0x30 + (index * sizeof(td_u8)));
}

/* register: ot_ext_system_demosaic_auto_nondirection_strength_write */
/* args: data (8-bit) */
/* addr range dm[0x40, 0x50) index:16 */
static __inline td_void ot_ext_system_demosaic_auto_nddm_str_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct(vreg_dm_base(vi_pipe) + 0x40 + (index * sizeof(td_u8)), data);
}
static __inline td_u8 ot_ext_system_demosaic_auto_nddm_str_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return iord_8direct(vreg_dm_base(vi_pipe) + 0x40 + (index * sizeof(td_u8)));
}

/* register: ot_ext_system_demosaic_auto_detail_smooth_range_write */
/* args: data (8-bit) */
/* addr range dm[0x50, 0x60) index:16 */
static __inline td_void ot_ext_system_demosaic_auto_dtlsmooth_range_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct(vreg_dm_base(vi_pipe) + 0x50 + (index * sizeof(td_u8)), data);
}
static __inline td_u8 ot_ext_system_demosaic_auto_dtlsmooth_range_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return iord_8direct(vreg_dm_base(vi_pipe) + 0x50 + (index * sizeof(td_u8)));
}

/* register: ot_ext_system_demosaic_auto_detail_smooth_strength_write */
/* args: data (16-bit) */
#define OT_EXT_SYSTEM_DEMOSAIC_AUTO_DTLSMOOTH_STR_DEFAULT 256
/* addr range dm[0x60, 0x80) index:16 */
static __inline td_void ot_ext_system_demosaic_auto_dtlsmooth_str_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct(vreg_dm_base(vi_pipe) + 0x60 + (index * sizeof(td_u16)), data);
}
static __inline td_u16 ot_ext_system_demosaic_auto_dtlsmooth_str_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return iord_16direct(vreg_dm_base(vi_pipe) + 0x60 + (index * sizeof(td_u16)));
}

/* register: ot_ext_system_demosaic_manual_colornoise_thdf_write */
#define OT_EXT_SYSTEM_DEMOSAIC_COLORNOISE_CTRL_THDF_DEFAULT 0
static __inline td_void ot_ext_system_demosaic_manual_colornoise_thdf_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_dm_base(vi_pipe) + 0x80, data);
}
static __inline td_u8 ot_ext_system_demosaic_manual_colornoise_thdf_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dm_base(vi_pipe) + 0x80);
}

/* register: ot_ext_system_demosaic_manual_colornoise_strf_write */
#define OT_EXT_SYSTEM_DEMOSAIC_COLORNOISE_CTRL_STRF_DEFAULT 8
static __inline td_void ot_ext_system_demosaic_manual_colornoise_strf_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_dm_base(vi_pipe) + 0x81, data);
}
static __inline td_u8 ot_ext_system_demosaic_manual_colornoise_strf_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dm_base(vi_pipe) + 0x81);
}

/* register: ot_ext_system_demosaic_manual_desat_dark_range_write */
/* args: data (8-bit) */
#define OT_EXT_SYSTEM_DEMOSAIC_MANUAL_DESAT_DARK_RANGE_DEFAULT  0
#define OT_EXT_SYSTEM_DEMOSAIC_MANUAL_DESAT_DARK_RANGE_DATASIZE 1
static __inline td_void ot_ext_system_demosaic_manual_desat_dark_range_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_dm_base(vi_pipe) + 0x82, data);
}
static __inline td_u8 ot_ext_system_demosaic_manual_desat_dark_range_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dm_base(vi_pipe) + 0x82);
}

/* register: ot_ext_system_demosaic_manual_desat_dark_strength_write */
/* args: data (8-bit) */
#define OT_EXT_SYSTEM_DEMOSAIC_MANUAL_DESAT_DARK_STRENGTH_DEFAULT  0
#define OT_EXT_SYSTEM_DEMOSAIC_MANUAL_DESAT_DARK_STRENGTH_DATASIZE 1
static __inline td_void ot_ext_system_demosaic_manual_desat_dark_strength_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_dm_base(vi_pipe) + 0x83, data);
}
static __inline td_u8 ot_ext_system_demosaic_manual_desat_dark_strength_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dm_base(vi_pipe) + 0x83);
}
/* register: ot_ext_system_demosaic_auto_colornoise_thdf_write */
/* args: data (16-bit) */
#define OT_EXT_SYSTEM_DEMOSAIC_AUTO_COLORNOISE_THDF_DEFAULT 0
/* addr range dm[0x90, 0xa0) index:16 */
static __inline td_void ot_ext_system_demosaic_auto_colornoise_thdf_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct(vreg_dm_base(vi_pipe) + 0x90 + (index * sizeof(td_u8)), data);
}
static __inline td_u8 ot_ext_system_demosaic_auto_colornoise_thdf_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return iord_8direct(vreg_dm_base(vi_pipe) + 0x90 + (index * sizeof(td_u8)));
}

/* register: ot_ext_system_demosaic_auto_colornoise_strf_write */
/* args: data (16-bit) */
/* addr range dm[0xa0, 0xb0) index:16 */
#define OT_EXT_SYSTEM_DEMOSAIC_AUTO_COLORNOISE_STRF_DEFAULT 8
static __inline td_void ot_ext_system_demosaic_auto_colornoise_strf_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct(vreg_dm_base(vi_pipe) + 0xa0 + (index * sizeof(td_u8)), data);
}
static __inline td_u8 ot_ext_system_demosaic_auto_colornoise_strf_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return iord_8direct(vreg_dm_base(vi_pipe) + 0xa0 + (index * sizeof(td_u8)));
}

/* register: ot_ext_system_demosaic_auto_desat_dark_range */
/* demosaic desat dark range */
#define OT_EXT_SYSTEM_DEMOSAIC_AUTO_DESAT_DARK_RANGE_DEFAULT 30
#define OT_EXT_SYSTEM_DEMOSAIC_AUTO_DESAT_DARK_RANGE_DATASIZE 16
/* args: data (8-bit) */
/* addr range dm[0xb0, 0xc0) index:16 */
static __inline td_void ot_ext_system_demosaic_auto_desat_dark_range_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct(vreg_dm_base(vi_pipe) + 0xb0 + (index * sizeof(td_u8)), data);
}
static __inline td_u8 ot_ext_system_demosaic_auto_desat_dark_range_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return iord_8direct(vreg_dm_base(vi_pipe) + 0xb0 + (index * sizeof(td_u8)));
}

/* register: ot_ext_system_demosaic_auto_desat_dark_strength */
/* demosaic desat dark strength */
#define OT_EXT_SYSTEM_DEMOSAIC_AUTO_DESAT_DARK_STRENGTH_DEFAULT 0
#define OT_EXT_SYSTEM_DEMOSAIC_AUTO_DESAT_DARK_STRENGTH_DATASIZE 16
/* args: data (8-bit) */
/* addr range dm[0xc0, 0xd0) index:16 */
static __inline td_void ot_ext_system_demosaic_auto_desat_dark_strength_write(ot_vi_pipe vi_pipe,
                                                                              td_u8 index, td_u8 data)
{
    iowr_8direct(vreg_dm_base(vi_pipe) + 0xc0 + (index * sizeof(td_u8)), data);
}
static __inline td_u8 ot_ext_system_demosaic_auto_desat_dark_strength_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return iord_8direct(vreg_dm_base(vi_pipe) + 0xc0 + (index * sizeof(td_u8)));
}

/* register: ot_ext_system_demosaic_aidm_en */
/* demosaic aidm enable */
#define OT_EXT_SYSTEM_DEMOSAIC_AIDM_ENABLE_DEFAULT 0x0
/* args: data (8-bit) */
static __inline td_void ot_ext_system_demosaic_aidm_en_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_dm_base(vi_pipe) + 0xd0, (data & 0x1));
}
static __inline td_u8 ot_ext_system_demosaic_aidm_en_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dm_base(vi_pipe) + 0xd0);
}

/* register: ot_ext_system_demosaic_aidm_blend_ratio */
/* demosaic aidm blend ratio */
#define OT_EXT_SYSTEM_DEMOSAIC_AI_DETAIL_STRENGTH_DEFAULT 0x80
/* args: data (16-bit) */
static __inline td_void ot_ext_system_demosaic_ai_detail_strength_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_dm_base(vi_pipe) + 0xd2, data);
}
static __inline td_u16 ot_ext_system_demosaic_ai_detail_strength_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_dm_base(vi_pipe) + 0xd2);
}

/* register: ot_ext_system_antifalsecolor_enable_write */
/* args: data (8-bit) */
#define OT_EXT_SYSTEM_ANTIFALSECOLOR_ENABLE_DEFAULT 0x1
static __inline td_void ot_ext_system_afc_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_fcr_base(vi_pipe), (data & 0x1));
}
static __inline td_u8 ot_ext_system_afc_enable_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_fcr_base(vi_pipe));
}

/* register: ot_ext_system_fcr_manual_mode_write */
/* args: data (8-bit) */
#define OT_EXT_SYSTEM_ANTIFALSECOLOR_MANUAL_MODE_DEFAULT 0
static __inline td_void ot_ext_system_afc_manual_mode_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_fcr_base(vi_pipe) + 0x1, data);
}
static __inline td_u8 ot_ext_system_afc_manual_mode_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_fcr_base(vi_pipe) + 0x1);
}

/* register: ot_ext_system_antifalsecolor_auto_threshold_write */
/* args: data (8-bit) */
/* addr range fcr[0x2, 0x12) index:16 */
static __inline td_void ot_ext_system_afc_auto_thd_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct(vreg_fcr_base(vi_pipe) + 0x2 + (index * sizeof(td_u8)), data);
}
static __inline td_u8 ot_ext_system_afc_auto_thd_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return iord_8direct(vreg_fcr_base(vi_pipe) + 0x2 + (index * sizeof(td_u8)));
}

/* register: ot_ext_system_antifalsecolor_auto_strenght_write */
/* args: data (8-bit) */
/* addr range fcr[0x12, 0x22) index:16 */
static __inline td_void ot_ext_system_afc_auto_str_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct(vreg_fcr_base(vi_pipe) + 0x12 + (index * sizeof(td_u8)), data);
}
static __inline td_u8 ot_ext_system_afc_auto_str_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return iord_8direct(vreg_fcr_base(vi_pipe) + 0x12 + (index * sizeof(td_u8)));
}

/* register: ot_ext_system_antifalsecolor_manual_threshold_write */
/* args: data (8-bit) */
#define OT_EXT_SYSTEM_ANTIFALSECOLOR_MANUAL_THRESHOLD_DEFAULT 8
static __inline td_void ot_ext_system_afc_manual_thd_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_fcr_base(vi_pipe) + 0x22, data);
}
static __inline td_u8 ot_ext_system_afc_manual_thd_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_fcr_base(vi_pipe) + 0x22);
}

/* register: ot_ext_system_antifalsecolor_manual_strenght_write */
/* args: data (8-bit) */
#define OT_EXT_SYSTEM_ANTIFALSECOLOR_MANUAL_STRENGTH_DEFAULT 8
static __inline td_void ot_ext_system_afc_manual_str_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_fcr_base(vi_pipe) + 0x23, data);
}
static __inline td_u8 ot_ext_system_afc_manual_str_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_fcr_base(vi_pipe) + 0x23);
}

static __inline td_void ot_ext_system_isp_afc_init_status_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_fcr_base(vi_pipe) + 0x24, (data & 0x01));
}

static __inline td_u8 ot_ext_system_isp_afc_init_status_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_fcr_base(vi_pipe) + 0x24) & 0x01);
}

/* BAYER NR: vreg_bnr_base(vi_pipe) ~ vreg_bnr_base(vi_pipe)  + */
/* register: ot_ext_system_bayernr_enable_write */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_bayernr_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x0), data);
}

static __inline td_u8 ot_ext_system_bayernr_enable_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_bnr_base(vi_pipe) + 0x0));
}

static __inline td_void ot_ext_system_bayernr_op_type_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x01), data);
}

static __inline td_u8 ot_ext_system_bayernr_op_type_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_bnr_base(vi_pipe) + 0x01));
}

static __inline td_void ot_ext_system_bayernr_tnr_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x02), data);
}

static __inline td_u8 ot_ext_system_bayernr_tnr_enable_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_bnr_base(vi_pipe) + 0x02));
}

static __inline td_void ot_ext_system_bayernr_lsc_nr_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x03), data);
}

static __inline td_u8 ot_ext_system_bayernr_lsc_nr_enable_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_bnr_base(vi_pipe) + 0x03));
}

static __inline td_void ot_ext_system_bayernr_lsc_ratio1_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x04), data);
}

static __inline td_u8 ot_ext_system_bayernr_lsc_ratio1_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_bnr_base(vi_pipe) + 0x04));
}

static __inline td_void ot_ext_system_bayernr_coring_ratio_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct((vreg_bnr_base(vi_pipe) + 0x06 + sizeof(td_u16) * index), data);
}

static __inline td_u16 ot_ext_system_bayernr_coring_ratio_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_16direct((vreg_bnr_base(vi_pipe) + 0x06 + sizeof(td_u16) * index)));
}

static __inline td_void ot_ext_system_bayernr_mix_gain_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct((vreg_bnr_base(vi_pipe) + 0x48 + sizeof(td_u16) * index), data);
}

static __inline td_u16 ot_ext_system_bayernr_mix_gain_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_16direct((vreg_bnr_base(vi_pipe) + 0x48 + sizeof(td_u16) * index)));
}

static __inline td_void ot_ext_system_bayernr_auto_sfm0_coarse_strength_r_lut_write(ot_vi_pipe vi_pipe,
                                                                                    td_u8 index, td_u16 data)
{
    iowr_16direct((vreg_bnr_base(vi_pipe) + 0x88 + sizeof(td_u16) * index), data);
}

static __inline td_u16 ot_ext_system_bayernr_auto_sfm0_coarse_strength_r_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_16direct((vreg_bnr_base(vi_pipe) + 0x88 + sizeof(td_u16) * index)));
}

static __inline td_void ot_ext_system_bayernr_auto_sfm0_coarse_strength_gr_lut_write(ot_vi_pipe vi_pipe,
                                                                                     td_u8 index, td_u16 data)
{
    iowr_16direct((vreg_bnr_base(vi_pipe) + 0xA8 + sizeof(td_u16) * index), data);
}

static __inline td_u16 ot_ext_system_bayernr_auto_sfm0_coarse_strength_gr_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_16direct((vreg_bnr_base(vi_pipe) + 0xA8 + sizeof(td_u16) * index)));
}

static __inline td_void ot_ext_system_bayernr_auto_sfm0_coarse_strength_gb_lut_write(ot_vi_pipe vi_pipe,
                                                                                     td_u8 index, td_u16 data)
{
    iowr_16direct((vreg_bnr_base(vi_pipe) + 0xC8 + sizeof(td_u16) * index), data);
}

static __inline td_u16 ot_ext_system_bayernr_auto_sfm0_coarse_strength_gb_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_16direct((vreg_bnr_base(vi_pipe) + 0xC8 + sizeof(td_u16) * index)));
}

static __inline td_void ot_ext_system_bayernr_auto_sfm0_coarse_strength_b_lut_write(ot_vi_pipe vi_pipe,
                                                                                    td_u8 index, td_u16 data)
{
    iowr_16direct((vreg_bnr_base(vi_pipe) + 0xE8 + sizeof(td_u16) * index), data);
}

static __inline td_u16 ot_ext_system_bayernr_auto_sfm0_coarse_strength_b_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_16direct((vreg_bnr_base(vi_pipe) + 0xE8 + sizeof(td_u16) * index)));
}

static __inline td_void ot_ext_system_bayernr_auto_sfm6_strength_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x108 + sizeof(td_u8) * index), data);
}

static __inline td_u8 ot_ext_system_bayernr_auto_sfm6_strength_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct((vreg_bnr_base(vi_pipe) + 0x108 + sizeof(td_u8) * index)));
}

static __inline td_void ot_ext_system_bayernr_auto_sfm0_detail_prot_lut_write(ot_vi_pipe vi_pipe,
                                                                              td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x118 + sizeof(td_u8) * index), data);
}

static __inline td_u8 ot_ext_system_bayernr_auto_sfm0_detail_prot_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct((vreg_bnr_base(vi_pipe) + 0x118 + sizeof(td_u8) * index)));
}

static __inline td_void ot_ext_system_bayernr_auto_fine_strength_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x128 + sizeof(td_u8) * index), data);
}

static __inline td_u8 ot_ext_system_bayernr_auto_fine_strength_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct((vreg_bnr_base(vi_pipe) + 0x128 + sizeof(td_u8) * index)));
}

static __inline td_void ot_ext_system_bayernr_auto_coring_wgt_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct((vreg_bnr_base(vi_pipe) + 0x138 + sizeof(td_u16) * index), data);
}

static __inline td_u16 ot_ext_system_bayernr_auto_coring_wgt_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_16direct((vreg_bnr_base(vi_pipe) + 0x138 + sizeof(td_u16) * index)));
}

static __inline td_void ot_ext_system_bayernr_auto_coring_mot_ratio_lut_write(ot_vi_pipe vi_pipe,
                                                                              td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x158 + sizeof(td_u8) * index), data);
}

static __inline td_u8 ot_ext_system_bayernr_auto_coring_mot_ratio_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct((vreg_bnr_base(vi_pipe) + 0x158 + sizeof(td_u8) * index)));
}

static __inline td_void ot_ext_system_bayernr_manual_sfm0_coarse_strength_r_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_bnr_base(vi_pipe) + 0x168), data);
}

static __inline td_u16 ot_ext_system_bayernr_manual_sfm0_coarse_strength_r_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_bnr_base(vi_pipe) + 0x168));
}

static __inline td_void ot_ext_system_bayernr_manual_sfm0_coarse_strength_gr_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_bnr_base(vi_pipe) + 0x16A), data);
}

static __inline td_u16 ot_ext_system_bayernr_manual_sfm0_coarse_strength_gr_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_bnr_base(vi_pipe) + 0x16A));
}

static __inline td_void ot_ext_system_bayernr_manual_sfm0_coarse_strength_gb_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_bnr_base(vi_pipe) + 0x16C), data);
}

static __inline td_u16 ot_ext_system_bayernr_manual_sfm0_coarse_strength_gb_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_bnr_base(vi_pipe) + 0x16C));
}

static __inline td_void ot_ext_system_bayernr_manual_sfm0_coarse_strength_b_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_bnr_base(vi_pipe) + 0x16E), data);
}

static __inline td_u16 ot_ext_system_bayernr_manual_sfm0_coarse_strength_b_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_bnr_base(vi_pipe) + 0x16E));
}

static __inline td_void ot_ext_system_bayernr_manual_sfm6_strength_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x170), data);
}

static __inline td_u8 ot_ext_system_bayernr_manual_sfm6_strength_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_bnr_base(vi_pipe) + 0x170));
}

static __inline td_void ot_ext_system_bayernr_manual_sfm0_detail_prot_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x171), data);
}

static __inline td_u8 ot_ext_system_bayernr_manual_sfm0_detail_prot_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_bnr_base(vi_pipe) + 0x171));
}

static __inline td_void ot_ext_system_bayernr_manual_fine_strength_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x172), data);
}

static __inline td_u8 ot_ext_system_bayernr_manual_fine_strength_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_bnr_base(vi_pipe) + 0x172));
}

static __inline td_void ot_ext_system_bayernr_manual_coring_wgt_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_bnr_base(vi_pipe) + 0x174), data);
}

static __inline td_u16 ot_ext_system_bayernr_manual_coring_wgt_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_bnr_base(vi_pipe) + 0x174));
}

static __inline td_void ot_ext_system_bayernr_manual_coring_mot_ratio_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x176), data);
}

static __inline td_u8 ot_ext_system_bayernr_manual_coring_mot_ratio_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_bnr_base(vi_pipe) + 0x176));
}

static __inline td_void ot_ext_system_bayernr_auto_md_anti_flicker_strength_lut_write(ot_vi_pipe vi_pipe,
                                                                                      td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x177 + sizeof(td_u8) * index), data);
}

static __inline td_u8 ot_ext_system_bayernr_auto_md_anti_flicker_strength_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct((vreg_bnr_base(vi_pipe) + 0x177 + sizeof(td_u8) * index)));
}

static __inline td_void ot_ext_system_bayernr_auto_md_static_ratio_lut_write(ot_vi_pipe vi_pipe,
                                                                             td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x187 + sizeof(td_u8) * index), data);
}

static __inline td_u8 ot_ext_system_bayernr_auto_md_static_ratio_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct((vreg_bnr_base(vi_pipe) + 0x187 + sizeof(td_u8) * index)));
}

static __inline td_void ot_ext_system_bayernr_auto_md_motion_ratio_lut_write(ot_vi_pipe vi_pipe,
                                                                             td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x197 + sizeof(td_u8) * index), data);
}

static __inline td_u8 ot_ext_system_bayernr_auto_md_motion_ratio_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct((vreg_bnr_base(vi_pipe) + 0x197 + sizeof(td_u8) * index)));
}

static __inline td_void ot_ext_system_bayernr_auto_md_static_fine_strength_lut_write(ot_vi_pipe vi_pipe,
                                                                                     td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x1A7 + sizeof(td_u8) * index), data);
}

static __inline td_u8 ot_ext_system_bayernr_auto_md_static_fine_strength_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct((vreg_bnr_base(vi_pipe) + 0x1A7 + sizeof(td_u8) * index)));
}

static __inline td_void ot_ext_system_bayernr_auto_tfs_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x1B7 + sizeof(td_u8) * index), data);
}

static __inline td_u8 ot_ext_system_bayernr_auto_tfs_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct((vreg_bnr_base(vi_pipe) + 0x1B7 + sizeof(td_u8) * index)));
}

static __inline td_void ot_ext_system_bayernr_auto_user_define_md_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x1C7 + sizeof(td_u8) * index), data);
}

static __inline td_u8 ot_ext_system_bayernr_auto_user_define_md_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct((vreg_bnr_base(vi_pipe) + 0x1C7 + sizeof(td_u8) * index)));
}

static __inline td_void ot_ext_system_bayernr_auto_user_define_bri_thresh_lut_write(ot_vi_pipe vi_pipe,
                                                                                    td_u8 index, td_s16 data)
{
    iowr_16direct((vreg_bnr_base(vi_pipe) + 0x1D8 + sizeof(td_s16) * index), data);
}

static __inline td_s16 ot_ext_system_bayernr_auto_user_define_bri_thresh_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_16direct((vreg_bnr_base(vi_pipe) + 0x1D8 + sizeof(td_s16) * index)));
}

static __inline td_void ot_ext_system_bayernr_auto_user_define_dark_thresh_lut_write(ot_vi_pipe vi_pipe,
                                                                                     td_u8 index, td_u16 data)
{
    iowr_16direct((vreg_bnr_base(vi_pipe) + 0x1F8 + sizeof(td_u16) * index), data);
}

static __inline td_u16 ot_ext_system_bayernr_auto_user_define_dark_thresh_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_16direct((vreg_bnr_base(vi_pipe) + 0x1F8 + sizeof(td_u16) * index)));
}

static __inline td_void ot_ext_system_bayernr_auto_user_define_color_thresh_lut_write(ot_vi_pipe vi_pipe,
                                                                                      td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x218 + sizeof(td_u8) * index), data);
}

static __inline td_u8 ot_ext_system_bayernr_auto_user_define_color_thresh_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct((vreg_bnr_base(vi_pipe) + 0x218 + sizeof(td_u8) * index)));
}

static __inline td_void ot_ext_system_bayernr_auto_sfr_r_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x228 + sizeof(td_u8) * index), data);
}

static __inline td_u8 ot_ext_system_bayernr_auto_sfr_r_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct((vreg_bnr_base(vi_pipe) + 0x228 + sizeof(td_u8) * index)));
}

static __inline td_void ot_ext_system_bayernr_auto_sfr_g_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x238 + sizeof(td_u8) * index), data);
}

static __inline td_u8 ot_ext_system_bayernr_auto_sfr_g_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct((vreg_bnr_base(vi_pipe) + 0x238 + sizeof(td_u8) * index)));
}

static __inline td_void ot_ext_system_bayernr_auto_sfr_b_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x248 + sizeof(td_u8) * index), data);
}

static __inline td_u8 ot_ext_system_bayernr_auto_sfr_b_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct((vreg_bnr_base(vi_pipe) + 0x248 + sizeof(td_u8) * index)));
}

static __inline td_void ot_ext_system_bayernr_manual_md_anti_flicker_strength_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x258), data);
}

static __inline td_u8 ot_ext_system_bayernr_manual_md_anti_flicker_strength_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_bnr_base(vi_pipe) + 0x258));
}

static __inline td_void ot_ext_system_bayernr_manual_md_static_ratio_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x259), data);
}

static __inline td_u8 ot_ext_system_bayernr_manual_md_static_ratio_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_bnr_base(vi_pipe) + 0x259));
}

static __inline td_void ot_ext_system_bayernr_manual_md_motion_ratio_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x25A), data);
}

static __inline td_u8 ot_ext_system_bayernr_manual_md_motion_ratio_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_bnr_base(vi_pipe) + 0x25A));
}

static __inline td_void ot_ext_system_bayernr_manual_md_static_fine_strength_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x25B), data);
}

static __inline td_u8 ot_ext_system_bayernr_manual_md_static_fine_strength_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_bnr_base(vi_pipe) + 0x25B));
}

static __inline td_void ot_ext_system_bayernr_manual_tfs_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x25C), data);
}

static __inline td_u8 ot_ext_system_bayernr_manual_tfs_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_bnr_base(vi_pipe) + 0x25C));
}

static __inline td_void ot_ext_system_bayernr_manual_user_define_md_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x25D), data);
}

static __inline td_u8 ot_ext_system_bayernr_manual_user_define_md_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_bnr_base(vi_pipe) + 0x25D));
}

static __inline td_void ot_ext_system_bayernr_manual_user_define_bri_thresh_write(ot_vi_pipe vi_pipe, td_s16 data)
{
    iowr_16direct((vreg_bnr_base(vi_pipe) + 0x25E), data);
}

static __inline td_s16 ot_ext_system_bayernr_manual_user_define_bri_thresh_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_bnr_base(vi_pipe) + 0x25E));
}

static __inline td_void ot_ext_system_bayernr_manual_user_define_dark_thresh_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_bnr_base(vi_pipe) + 0x260), data);
}

static __inline td_u16 ot_ext_system_bayernr_manual_user_define_dark_thresh_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_bnr_base(vi_pipe) + 0x260));
}

static __inline td_void ot_ext_system_bayernr_manual_user_define_color_thresh_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x262), data);
}


static __inline td_u8 ot_ext_system_bayernr_manual_user_define_color_thresh_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_bnr_base(vi_pipe) + 0x262));
}

static __inline td_void ot_ext_system_bayernr_manual_sfr_r_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x263), data);
}

static __inline td_u8 ot_ext_system_bayernr_manual_sfr_r_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_bnr_base(vi_pipe) + 0x263));
}

static __inline td_void ot_ext_system_bayernr_manual_sfr_g_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x264), data);
}

static __inline td_u8 ot_ext_system_bayernr_manual_sfr_g_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_bnr_base(vi_pipe) + 0x264));
}

static __inline td_void ot_ext_system_bayernr_manual_sfr_b_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x265), data);
}

static __inline td_u8 ot_ext_system_bayernr_manual_sfr_b_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_bnr_base(vi_pipe) + 0x265));
}

static __inline td_void ot_ext_system_bayernr_snr_sfm0_wdr_strength_lut_write(ot_vi_pipe vi_pipe,
                                                                              td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x266 + sizeof(td_u8) * index), data);
}

static __inline td_u8 ot_ext_system_bayernr_snr_sfm0_wdr_strength_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct((vreg_bnr_base(vi_pipe) + 0x266 + sizeof(td_u8) * index)));
}

static __inline td_void ot_ext_system_bayernr_snr_sfm0_fusion_strength_lut_write(ot_vi_pipe vi_pipe,
                                                                                 td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x26A + sizeof(td_u8) * index), data);
}

static __inline td_u8 ot_ext_system_bayernr_snr_sfm0_fusion_strength_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct((vreg_bnr_base(vi_pipe) + 0x26A + sizeof(td_u8) * index)));
}

static __inline td_void ot_ext_system_bayernr_snr_wdr_sfm6_strength_lut_write(ot_vi_pipe vi_pipe,
                                                                              td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x26E + sizeof(td_u8) * index), data);
}

static __inline td_u8 ot_ext_system_bayernr_snr_wdr_sfm6_strength_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct((vreg_bnr_base(vi_pipe) + 0x26E + sizeof(td_u8) * index)));
}

static __inline td_void ot_ext_system_bayernr_md_wdr_strength_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x272 + sizeof(td_u8) * index), data);
}

static __inline td_u8 ot_ext_system_bayernr_md_wdr_strength_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct((vreg_bnr_base(vi_pipe) + 0x272 + sizeof(td_u8) * index)));
}

static __inline td_void ot_ext_system_bayernr_md_fusion_strength_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x276 + sizeof(td_u8) * index), data);
}

static __inline td_u8 ot_ext_system_bayernr_md_fusion_strength_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct((vreg_bnr_base(vi_pipe) + 0x276 + sizeof(td_u8) * index)));
}

static __inline td_void ot_ext_system_bayernr_denoise_y_alpha_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x27A), data);
}

static __inline td_u8 ot_ext_system_bayernr_denoise_y_alpha_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_bnr_base(vi_pipe) + 0x27A));
}

static __inline td_void ot_ext_system_bayernr_denoise_y_fg_str_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x27C + sizeof(td_u8) * index), data);
}

static __inline td_u8 ot_ext_system_bayernr_denoise_y_fg_str_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct((vreg_bnr_base(vi_pipe) + 0x27C + sizeof(td_u8) * index)));
}

static __inline td_void ot_ext_system_bayernr_denoise_y_bg_str_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x28C + sizeof(td_u8) * index), data);
}

static __inline td_u8 ot_ext_system_bayernr_denoise_y_bg_str_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct((vreg_bnr_base(vi_pipe) + 0x28C + sizeof(td_u8) * index)));
}

static __inline td_void ot_ext_system_bayernr_auto_dering_strength_lut_write(ot_vi_pipe vi_pipe,
                                                                             td_u8 index, td_u16 data)
{
    iowr_16direct((vreg_bnr_base(vi_pipe) + 0x2AC + sizeof(td_u16) * index), data);
}

static __inline td_u16 ot_ext_system_bayernr_auto_dering_strength_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_16direct((vreg_bnr_base(vi_pipe) + 0x2AC + sizeof(td_u16) * index)));
}

static __inline td_void ot_ext_system_bayernr_auto_dering_thresh_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x2DC + sizeof(td_u8) * index), data);
}

static __inline td_u8 ot_ext_system_bayernr_auto_dering_thresh_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct((vreg_bnr_base(vi_pipe) + 0x2DC + sizeof(td_u8) * index)));
}

static __inline td_void ot_ext_system_bayernr_auto_dering_static_strength_lut_write(ot_vi_pipe vi_pipe,
                                                                                    td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x2EC + sizeof(td_u8) * index), data);
}

static __inline td_u8 ot_ext_system_bayernr_auto_dering_static_strength_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct((vreg_bnr_base(vi_pipe) + 0x2EC + sizeof(td_u8) * index)));
}

static __inline td_void ot_ext_system_bayernr_auto_dering_motion_strength_lut_write(ot_vi_pipe vi_pipe,
                                                                                    td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x2FC + sizeof(td_u8) * index), data);
}

static __inline td_u8 ot_ext_system_bayernr_auto_dering_motion_strength_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct((vreg_bnr_base(vi_pipe) + 0x2FC + sizeof(td_u8) * index)));
}

static __inline td_void ot_ext_system_bayernr_manual_dering_strength_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_bnr_base(vi_pipe) + 0x30C), data);
}

static __inline td_u16 ot_ext_system_bayernr_manual_dering_strength_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_bnr_base(vi_pipe) + 0x30C));
}

static __inline td_void ot_ext_system_bayernr_manual_dering_thresh_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x30F), data);
}

static __inline td_u8 ot_ext_system_bayernr_manual_dering_thresh_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_bnr_base(vi_pipe) + 0x30F));
}

static __inline td_void ot_ext_system_bayernr_manual_dering_static_strength_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x310), data);
}

static __inline td_u8 ot_ext_system_bayernr_manual_dering_static_strength_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_bnr_base(vi_pipe) + 0x310));
}

static __inline td_void ot_ext_system_bayernr_manual_dering_motion_strength_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x311), data);
}

static __inline td_u8 ot_ext_system_bayernr_manual_dering_motion_strength_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_bnr_base(vi_pipe) + 0x311));
}

static __inline td_void ot_ext_system_bayernr_snr_fusion_sfm6_strength_lut_write(ot_vi_pipe vi_pipe,
                                                                                 td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x315 + sizeof(td_u8) * index), data);
}

static __inline td_u8 ot_ext_system_bayernr_snr_fusion_sfm6_strength_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct((vreg_bnr_base(vi_pipe) + 0x315 + sizeof(td_u8) * index)));
}

static __inline td_void ot_ext_system_isp_bayernr_init_status_write(ot_vi_pipe vi_pipe,  td_u8 data)
{
    iowr_8direct(vreg_bnr_base(vi_pipe) + 0x319, (data & 0x01));
}

static __inline td_u8 ot_ext_system_isp_bayernr_init_status_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_bnr_base(vi_pipe) + 0x319) & 0x01);
}

static __inline td_void ot_ext_system_bayernr_auto_sfm7_strength_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x31B + sizeof(td_u8) * index), data);
}

static __inline td_u8 ot_ext_system_bayernr_auto_sfm7_strength_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct((vreg_bnr_base(vi_pipe) + 0x31B + sizeof(td_u8) * index)));
}

static __inline td_void ot_ext_system_bayernr_manual_sfm7_strength_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x32B), data);
}

static __inline td_u8 ot_ext_system_bayernr_manual_sfm7_strength_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_bnr_base(vi_pipe) + 0x32B));
}

static __inline td_void ot_ext_system_bayernr_auto_sth_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x32C + sizeof(td_u8) * index), data);
}

static __inline td_u8 ot_ext_system_bayernr_auto_sth_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct((vreg_bnr_base(vi_pipe) + 0x32C + sizeof(td_u8) * index)));
}

static __inline td_void ot_ext_system_bayernr_manual_sth_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x33C), data);
}

static __inline td_u8 ot_ext_system_bayernr_manual_sth_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_bnr_base(vi_pipe) + 0x33C));
}

static __inline td_void ot_ext_system_bayernr_auto_sfm1_adp_strength_lut_write(ot_vi_pipe vi_pipe,
                                                                               td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x33D + sizeof(td_u8) * index), data);
}

static __inline td_u8 ot_ext_system_bayernr_auto_sfm1_adp_strength_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct((vreg_bnr_base(vi_pipe) + 0x33D + sizeof(td_u8) * index)));
}

static __inline td_void ot_ext_system_bayernr_manual_sfm1_adp_strength_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x34D), data);
}

static __inline td_u8 ot_ext_system_bayernr_manual_sfm1_adp_strength_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_bnr_base(vi_pipe) + 0x34D));
}

static __inline td_void ot_ext_system_bayernr_auto_sfm1_strength_lut_write(ot_vi_pipe vi_pipe,
                                                                           td_u8 index, td_u16 data)
{
    iowr_16direct((vreg_bnr_base(vi_pipe) + 0x34E + sizeof(td_u16) * index), data);
}

static __inline td_u16 ot_ext_system_bayernr_auto_sfm1_strength_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_16direct((vreg_bnr_base(vi_pipe) + 0x34E + sizeof(td_u16) * index)));
}

static __inline td_void ot_ext_system_bayernr_manual_sfm1_strength_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_bnr_base(vi_pipe) + 0x36E), data);
}

static __inline td_u16 ot_ext_system_bayernr_manual_sfm1_strength_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_bnr_base(vi_pipe) + 0x36E));
}

static __inline td_void ot_ext_system_bayernr_auto_md_mode_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x370 + sizeof(td_u8) * index), data);
}

static __inline td_u8 ot_ext_system_bayernr_auto_md_mode_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct((vreg_bnr_base(vi_pipe) + 0x370 + sizeof(td_u8) * index)));
}

static __inline td_void ot_ext_system_bayernr_manual_md_mode_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x380), data);
}

static __inline td_u8 ot_ext_system_bayernr_manual_md_mode_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_bnr_base(vi_pipe) + 0x380));
}

static __inline td_void ot_ext_system_bayernr_auto_md_size_ratio_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x381 + sizeof(td_u8) * index), data);
}

static __inline td_u8 ot_ext_system_bayernr_auto_md_size_ratio_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct((vreg_bnr_base(vi_pipe) + 0x381 + sizeof(td_u8) * index)));
}

static __inline td_void ot_ext_system_bayernr_manual_md_size_ratio_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x391), data);
}

static __inline td_u8 ot_ext_system_bayernr_manual_md_size_ratio_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_bnr_base(vi_pipe) + 0x391));
}

static __inline td_void ot_ext_system_bayernr_snr_wdr_sfm7_strength_lut_write(ot_vi_pipe vi_pipe,
                                                                              td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x392 + sizeof(td_u8) * index), data);
}

static __inline td_u8 ot_ext_system_bayernr_snr_wdr_sfm7_strength_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct((vreg_bnr_base(vi_pipe) + 0x392 + sizeof(td_u8) * index)));
}

static __inline td_void ot_ext_system_bayernr_snr_fusion_sfm7_strength_lut_write(ot_vi_pipe vi_pipe,
                                                                                 td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x396 + sizeof(td_u8) * index), data);
}

static __inline td_u8 ot_ext_system_bayernr_snr_fusion_sfm7_strength_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return (iord_8direct((vreg_bnr_base(vi_pipe) + 0x396 + sizeof(td_u8) * index)));
}

static __inline td_void ot_ext_system_bayernr_attr_update_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_bnr_base(vi_pipe) + 0x39A), data);
}

static __inline td_u8 ot_ext_system_bayernr_attr_update_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_bnr_base(vi_pipe) + 0x39A));
}

/* frame stitch WDR */
/* WDR---U8 --EXT_REGISTER */
/* register: ot_ext_wdr_manual_mode_enable */
/* 0: auto; 1: manual; */
/* td_bool */
static __inline td_void ot_ext_system_wdr_manual_mode_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe), data);
}
static __inline td_u8 ot_ext_system_wdr_manual_mode_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_wdr_base(vi_pipe));
}

static __inline td_void ot_ext_system_wdr_coef_update_en_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x1, data);
}
static __inline td_u8 ot_ext_system_wdr_coef_update_en_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_wdr_base(vi_pipe)  +  0x1);
}

/* wdr_en */
#define OT_EXT_SYSTEM_WDR_EN_DEFAULT 0
static __inline td_void ot_ext_system_wdr_en_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x2, data);
}
static __inline td_u8 ot_ext_system_wdr_en_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_wdr_base(vi_pipe)  +  0x2);
}

/* fusion_mode */
static __inline td_void ot_ext_system_fusion_mode_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x3, data);
}
static __inline td_u8 ot_ext_system_fusion_mode_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_wdr_base(vi_pipe)  +  0x3);
}

/* mdt_en */
#define OT_EXT_SYSTEM_MDT_EN_DEFAULT 1
static __inline td_void ot_ext_system_mdt_en_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x4, data);
}
static __inline td_u8 ot_ext_system_mdt_en_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_wdr_base(vi_pipe)  +  0x4);
}

static __inline td_void ot_ext_system_isp_fswdr_init_status_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x5, (data & 0x01));
}

static __inline td_u8 ot_ext_system_isp_fswdr_init_status_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0x5) & 0x01);
}

/* shortexpo_chk */
static __inline td_void ot_ext_system_wdr_shortexpo_chk_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x6, data);
}
static __inline td_u8 ot_ext_system_wdr_shortexpo_chk_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_wdr_base(vi_pipe)  +  0x6);
}

/* td_u8 */
/* mdt_l_bld */
#define OT_EXT_SYSTEM_WDR_MDT_LONG_BLEND_DEFAULT 0xFE

static __inline td_void ot_ext_system_wdr_mdt_long_blend_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x12, data);
}
static __inline td_u8 ot_ext_system_wdr_mdt_long_blend_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0x12));
}

/* noise_model_coef */
#define OT_EXT_SYSTEM_WDR_NOISE_MODEL_COEF_DEFAULT 0x50
static __inline td_void ot_ext_system_wdr_noise_model_coef_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x13, data);
}
static __inline td_u8 ot_ext_system_wdr_noise_model_coef_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0x13));
}

/* mdt_full_thr */
#define OT_EXT_SYSTEM_WDR_MDT_FULL_THR_DEFAULT 0xFE
static __inline td_void ot_ext_system_wdr_mdt_full_thr_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x1e, data);
}
static __inline td_u8 ot_ext_system_wdr_mdt_full_thr_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0x1e));
}

/* mdt_still_thr */
#define OT_EXT_SYSTEM_WDR_MDT_STILL_THR_DEFAULT 0x14
static __inline td_void ot_ext_system_wdr_mdt_still_thr_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x1f, data);
}
static __inline td_u8 ot_ext_system_wdr_mdt_still_thr_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0x1f));
}

#define OT_EXT_SYSTEM_FUSION_BLEND_EN_DEFAULT 1
static __inline td_void ot_ext_system_fusion_blend_en_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x20, data);
}
static __inline td_u8 ot_ext_system_fusion_blend_en_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0x20));
}

static __inline td_void ot_ext_system_wdr_manual_mdthr_low_gain_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x22, data);
}
static __inline td_u8 ot_ext_system_wdr_manual_mdthr_low_gain_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0x22));
}

static __inline td_void ot_ext_system_wdr_manual_mdthr_hig_gain_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x23, data);
}
static __inline td_u8 ot_ext_system_wdr_manual_mdthr_hig_gain_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0x23));
}

#define OT_EXT_SYSTEM_WDR_MDREF_FLICKER_DEFAULT 0x1
static __inline td_void ot_ext_system_wdr_mdref_flicker_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x24, data);
}
static __inline td_u8 ot_ext_system_wdr_mdref_flicker_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0x24));
}

static __inline td_void ot_ext_system_wdr_fusion_blend_wgt_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x25, data);
}
static __inline td_u8 ot_ext_system_wdr_fusion_blend_wgt_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0x25));
}

#define OT_EXT_SYSTEM_FUSION_FORCE_BLEND_THRESHOLD_DEFAULT 0xE
static __inline td_void ot_ext_system_fusion_force_blend_threshold_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x26, data);
}
static __inline td_u8 ot_ext_system_fusion_force_blend_threshold_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0x26));
}

/* fusion_force_gray_en */
#define OT_EXT_SYSTEM_FUSION_FORCE_GRAY_EN_DEFAULT 0x1
static __inline td_void ot_ext_system_fusion_force_gray_en_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x27, data);
}
static __inline td_u16 ot_ext_system_fusion_force_gray_en_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0x27));
}

/* td_u16 */
/* long thr */
#define OT_EXT_SYSTEM_WDR_LONGTHR_WRITE_DEFAULT 0xBC0
static __inline td_void ot_ext_system_wdr_longthr_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_wdr_base(vi_pipe) + 0x40, data);
}
static __inline td_u16 ot_ext_system_wdr_longthr_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_wdr_base(vi_pipe) + 0x40));
}

/* short thr */
#define OT_EXT_SYSTEM_WDR_SHORTTHR_WRITE_DEFAULT 0xFC0
static __inline td_void ot_ext_system_wdr_shortthr_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_wdr_base(vi_pipe) + 0x42, data);
}
static __inline td_u16 ot_ext_system_wdr_shortthr_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_wdr_base(vi_pipe) + 0x42));
}

static __inline td_void ot_ext_system_fusion_thr_write(ot_vi_pipe vi_pipe, td_u8 i, td_u16 data)
{
    iowr_16direct(vreg_wdr_base(vi_pipe) + 0x44 + (i * sizeof(td_u16)), data);
}
static __inline td_u16 ot_ext_system_fusion_thr_read(ot_vi_pipe vi_pipe,  td_u8 i)
{
    return iord_16direct(vreg_wdr_base(vi_pipe) + 0x44 + (i * sizeof(td_u16)));
}

#define OT_EXT_SYSTEM_WDR_WBGAIN_POSITION_DEFAULT 1
#define OT_EXT_SYSTEM_WDR_WBGAIN_IN_DG1 0
#define OT_EXT_SYSTEM_WDR_WBGAIN_IN_WB 1

/* args: data (2-bit) */
static __inline td_void ot_ext_system_wdr_wbgain_position_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_wdr_base(vi_pipe) + 0x4f), data);
}
static __inline td_u8 ot_ext_system_wdr_wbgain_position_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_wdr_base(vi_pipe) + 0x4f);
}

/* LUT */
/* isp_wdr_forcelong_en */
#define OT_EXT_SYSTEM_WDR_FORCELONG_EN 0x1
static __inline td_void ot_ext_system_wdr_forcelong_en_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0xe2, data);
}

static __inline td_u8 ot_ext_system_wdr_forcelong_en_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0xe2));
}

/* isp_wdr_forcelong_low_thd */
#define OT_EXT_SYSTEM_WDR_FORCELONG_LOW_THD 0x1F4
static __inline td_void ot_ext_system_wdr_forcelong_low_thd_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_wdr_base(vi_pipe) + 0xe4, data);
}

static __inline td_u16 ot_ext_system_wdr_forcelong_low_thd_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_wdr_base(vi_pipe) + 0xe4));
}

/* isp_wdr_forcelong_high_thd */
#define OT_EXT_SYSTEM_WDR_FORCELONG_HIGH_THD 0x2BC
static __inline td_void ot_ext_system_wdr_forcelong_high_thd_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_wdr_base(vi_pipe) + 0xe6, data);
}

static __inline td_u16 ot_ext_system_wdr_forcelong_high_thd_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_wdr_base(vi_pipe) + 0xe6));
}

/* isp_wdr_shortcheck_thd */
#define OT_EXT_SYSTEM_WDR_SHORTCHECK_THD 0xc
static __inline td_void ot_ext_system_wdr_shortcheck_thd_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_wdr_base(vi_pipe) + 0xe8, data);
}

static __inline td_u16 ot_ext_system_wdr_shortcheck_thd_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_wdr_base(vi_pipe) + 0xe8));
}

static __inline td_void ot_ext_system_wdr_auto_mdthr_low_gain_write(ot_vi_pipe vi_pipe, td_u8 i, td_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x110 + (i * sizeof(td_u8)), data);
}
static __inline td_u8 ot_ext_system_wdr_auto_mdthr_low_gain_read(ot_vi_pipe vi_pipe, td_u8 i)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0x110 + (i * sizeof(td_u8))));
}

static __inline td_void ot_ext_system_wdr_auto_mdthr_hig_gain_write(ot_vi_pipe vi_pipe, td_u8 i, td_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x1b0 + (i * sizeof(td_u8)), data);
}
static __inline td_u8 ot_ext_system_wdr_auto_mdthr_hig_gain_read(ot_vi_pipe vi_pipe, td_u8 i)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0x1b0 + (i * sizeof(td_u8))));
}

/* WDR---U32 ARRAY--EXT_REGISTER */
/* FLICKER */
/* register: ot_ext_system_flicker_min_band_num */
/* flicker_min_band_num */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_flicker_min_band_num_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_flick_base(vi_pipe)  +  0X0, data);
}
static __inline td_u8 ot_ext_system_flicker_min_band_num_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_flick_base(vi_pipe)  +  0X0);
}

/* register: ot_ext_system_flicker_min_valid_band_pcnt */
/* flicker_min_valid_band_pcnt */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_flicker_min_valid_band_pcnt_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_flick_base(vi_pipe)  +  0x1, data);
}
static __inline td_u8 ot_ext_system_flicker_min_valid_band_pcnt_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_flick_base(vi_pipe)  +  0x1);
}

/* register: ot_ext_system_flicker_wave_diff1 */
/* flicker_wave_diff1 */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_flicker_wave_diff1_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_flick_base(vi_pipe)  +  0x2, data);
}
static __inline td_u8 ot_ext_system_flicker_wave_diff1_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_flick_base(vi_pipe)  +  0x2);
}

/* register: ot_ext_system_flicker_wave_diff2 */
/* flicker_wave_diff2 */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_flicker_wave_diff2_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_flick_base(vi_pipe)  +  0x3, data);
}
static __inline td_u8 ot_ext_system_flicker_wave_diff2_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_flick_base(vi_pipe)  +  0x3);
}

/* register: ot_ext_system_flicker_period */
/* flicker_period */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_flicker_period_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_flick_base(vi_pipe)  +  0x4, data);
}
static __inline td_u8 ot_ext_system_flicker_period_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_flick_base(vi_pipe)  +  0x4);
}

/* register: ot_ext_system_flicker_gr_cnt */
/* flicker_gr_cnt */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_flicker_gr_cnt_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_flick_base(vi_pipe)  +  0x5, data);
}
static __inline td_u8 ot_ext_system_flicker_gr_cnt_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_flick_base(vi_pipe)  +  0x5);
}

/* register: ot_ext_system_flicker_gb_cnt */
/* flicker_gb_cnt */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_flicker_gb_cnt_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_flick_base(vi_pipe)  +  0x6, data);
}
static __inline td_u8 ot_ext_system_flicker_gb_cnt_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_flick_base(vi_pipe)  +  0x6);
}

/* register: ot_ext_system_flicker_result */
/* flicker_result */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_flicker_result_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_flick_base(vi_pipe)  +  0x7, data);
}
static __inline td_u8 ot_ext_system_flicker_result_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_flick_base(vi_pipe)  +  0x7);
}

/* register: ot_ext_system_freq_result */
/* freq_result */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_freq_result_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_flick_base(vi_pipe)  +  0x8, data);
}
static __inline td_u8 ot_ext_system_freq_result_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_flick_base(vi_pipe)  +  0x8);
}

static __inline td_void ot_ext_system_flicker_over_cnt_thr_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_flick_base(vi_pipe)  +  0xc, data);
}
static __inline td_u32 ot_ext_system_flicker_over_cnt_thr_read(ot_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_flick_base(vi_pipe)  +  0xc);
}

static __inline td_void ot_ext_system_flicker_over_thr_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_flick_base(vi_pipe)  +  0x10, data);
}
static __inline td_u16 ot_ext_system_flicker_over_thr_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_flick_base(vi_pipe)  +  0x10);
}

/* lSC */
static __inline td_void ot_ext_system_isp_mesh_shading_enable_write(ot_vi_pipe vi_pipe,  td_u8 data)
{
    iowr_8direct(vreg_lsc_base(vi_pipe), data & 0x1);
}

static __inline td_u8 ot_ext_system_isp_mesh_shading_enable_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_lsc_base(vi_pipe)) & 0x1);
}

/* args: data (8-bit) */
static __inline td_void ot_ext_system_isp_mesh_shading_lut_attr_updata_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_lsc_base(vi_pipe)  +  1, data);
}
static __inline td_u8 ot_ext_system_isp_mesh_shading_lut_attr_updata_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_lsc_base(vi_pipe)  +  1);
}

/* args: data (8-bit) */
static __inline td_void ot_ext_system_isp_mesh_shading_attr_updata_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_lsc_base(vi_pipe)  +  0x2, data);
}
static __inline td_u8 ot_ext_system_isp_mesh_shading_attr_updata_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_lsc_base(vi_pipe)  +  0x2);
}

static __inline td_void ot_ext_system_isp_mesh_shading_mesh_scale_write(ot_vi_pipe vi_pipe,  td_u8 data)
{
    iowr_8direct(vreg_lsc_base(vi_pipe)  +  0x3, data);
}

static __inline td_u8 ot_ext_system_isp_mesh_shading_mesh_scale_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_lsc_base(vi_pipe)  +  0x3));
}

static __inline td_void ot_ext_system_isp_mesh_shading_mesh_strength_write(ot_vi_pipe vi_pipe,  td_u16 data)
{
    iowr_16direct(vreg_lsc_base(vi_pipe)  +  0x4, data);
}

static __inline td_u16 ot_ext_system_isp_mesh_shading_mesh_strength_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_lsc_base(vi_pipe)  +  0x4));
}

static __inline td_void ot_ext_system_isp_lsc_init_status_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_lsc_base(vi_pipe) + 0x7, (data & 0x01));
}

static __inline td_u8 ot_ext_system_isp_lsc_init_status_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_lsc_base(vi_pipe) + 0x7) & 0x01);
}

/* length : 32;bytes: 16 * 2 */
static __inline td_void ot_ext_system_isp_mesh_shading_xgrid_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct(vreg_lsc_base(vi_pipe)  +  0x8  + (index * sizeof(td_u16)), data);
}

static __inline td_u16 ot_ext_system_isp_mesh_shading_xgrid_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return iord_16direct(vreg_lsc_base(vi_pipe)  +  0x8  + (index * sizeof(td_u16)));
}

/* length: 16;bytes: 16 * 2 */
static __inline td_void ot_ext_system_isp_mesh_shading_ygrid_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct(vreg_lsc_base(vi_pipe)  +  0x28  + (index * sizeof(td_u16)), data);
}

static __inline td_u16 ot_ext_system_isp_mesh_shading_ygrid_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return iord_16direct(vreg_lsc_base(vi_pipe)  +  0x28  + (index * sizeof(td_u16)));
}

/* bytes : 2 */
static __inline td_void ot_ext_system_isp_mesh_shading_blendratio_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_lsc_base(vi_pipe)  +  0x48, data);
}

static __inline td_u16 ot_ext_system_isp_mesh_shading_blendratio_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_lsc_base(vi_pipe)  +  0x48);
}

/* length:  1089;bytes: 1089 * 2 */
static __inline td_void ot_ext_system_isp_mesh_shading_r_gain0_write(ot_vi_pipe vi_pipe, td_u16 index, td_u16 data)
{
    iowr_16direct(vreg_lsc_base(vi_pipe) + 0x50 + (index * sizeof(td_u16)), data);
}

static __inline td_u16 ot_ext_system_isp_mesh_shading_r_gain0_read(ot_vi_pipe vi_pipe, td_u16 index)
{
    return iord_16direct(vreg_lsc_base(vi_pipe) + 0x50 + (index * sizeof(td_u16)));
}

/* length:  1089;bytes: 1089 * 2 */
static __inline td_void ot_ext_system_isp_mesh_shading_gr_gain0_write(ot_vi_pipe vi_pipe, td_u16 index, td_u16 data)
{
    iowr_16direct(vreg_lsc_base(vi_pipe) + 0x8D2 + (index * sizeof(td_u16)), data);
}

static __inline td_u16 ot_ext_system_isp_mesh_shading_gr_gain0_read(ot_vi_pipe vi_pipe, td_u16 index)
{
    return iord_16direct(vreg_lsc_base(vi_pipe) + 0x8D2 + (index * sizeof(td_u16)));
}

/* length: 1089;bytes: 1089 * 2 */
static __inline td_void ot_ext_system_isp_mesh_shading_gb_gain0_write(ot_vi_pipe vi_pipe, td_u16 index, td_u16 data)
{
    iowr_16direct(vreg_lsc_base(vi_pipe) + 0x1154 + (index * sizeof(td_u16)), data);
}

static __inline td_u16 ot_ext_system_isp_mesh_shading_gb_gain0_read(ot_vi_pipe vi_pipe, td_u16 index)
{
    return iord_16direct(vreg_lsc_base(vi_pipe) + 0x1154 + (index * sizeof(td_u16)));
}

/* length: 1089;bytes: 1089 * 2 */
static __inline td_void ot_ext_system_isp_mesh_shading_b_gain0_write(ot_vi_pipe vi_pipe, td_u16 index, td_u16 data)
{
    iowr_16direct(vreg_lsc_base(vi_pipe) + 0x19D6 + (index * sizeof(td_u16)), data);
}

static __inline td_u16 ot_ext_system_isp_mesh_shading_b_gain0_read(ot_vi_pipe vi_pipe, td_u16 index)
{
    return iord_16direct(vreg_lsc_base(vi_pipe) + 0x19D6 + (index * sizeof(td_u16)));
}

/* length: 1089;bytes: 1089 * 2 */
static __inline td_void ot_ext_system_isp_mesh_shading_r_gain1_write(ot_vi_pipe vi_pipe, td_u16 index, td_u16 data)
{
    iowr_16direct(vreg_lsc_base(vi_pipe) + 0x2258 + (index * sizeof(td_u16)), data);
}

static __inline td_u16 ot_ext_system_isp_mesh_shading_r_gain1_read(ot_vi_pipe vi_pipe, td_u16 index)
{
    return iord_16direct(vreg_lsc_base(vi_pipe) + 0x2258 + (index * sizeof(td_u16)));
}

/* length: 1089;bytes: 1089 * 2 */
static __inline td_void ot_ext_system_isp_mesh_shading_gr_gain1_write(ot_vi_pipe vi_pipe, td_u16 index, td_u16 data)
{
    iowr_16direct(vreg_lsc_base(vi_pipe) + 0x2ADA + (index * sizeof(td_u16)), data);
}

static __inline td_u16 ot_ext_system_isp_mesh_shading_gr_gain1_read(ot_vi_pipe vi_pipe, td_u16 index)
{
    return iord_16direct(vreg_lsc_base(vi_pipe) + 0x2ADA + (index * sizeof(td_u16)));
}

/* length: 1089;bytes: 1089 * 2 */
static __inline td_void ot_ext_system_isp_mesh_shading_gb_gain1_write(ot_vi_pipe vi_pipe, td_u16 index, td_u16 data)
{
    iowr_16direct(vreg_lsc_base(vi_pipe) + 0x335C + (index * sizeof(td_u16)), data);
}

static __inline td_u16 ot_ext_system_isp_mesh_shading_gb_gain1_read(ot_vi_pipe vi_pipe, td_u16 index)
{
    return iord_16direct(vreg_lsc_base(vi_pipe) + 0x335C + (index * sizeof(td_u16)));
}

/* length: 1089;bytes: 1089 * 2 */
static __inline td_void ot_ext_system_isp_mesh_shading_b_gain1_write(ot_vi_pipe vi_pipe, td_u16 index, td_u16 data)
{
    iowr_16direct(vreg_lsc_base(vi_pipe) + 0x3BDE + (index * sizeof(td_u16)), data);
}

static __inline td_u16 ot_ext_system_isp_mesh_shading_b_gain1_read(ot_vi_pipe vi_pipe, td_u16 index)
{
    return iord_16direct(vreg_lsc_base(vi_pipe) + 0x3BDE + (index * sizeof(td_u16)));
}

static __inline td_void ot_ext_system_isp_mesh_shading_fe_lut_attr_updata_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_lsc_base(vi_pipe)  +  0x4460, data);
}
static __inline td_u8 ot_ext_system_isp_mesh_shading_fe_lut_attr_updata_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_lsc_base(vi_pipe)  +  0x4460);
}

/* args: data (8-bit) */
static __inline td_void ot_ext_system_isp_mesh_shading_fe_attr_updata_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_lsc_base(vi_pipe)  +  0x4461, data);
}
static __inline td_u8 ot_ext_system_isp_mesh_shading_fe_attr_updata_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_lsc_base(vi_pipe)  +  0x4461);
}

static __inline td_void ot_ext_system_isp_mesh_shading_inner_update_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_lsc_base(vi_pipe)  +  0x4462, data);
}
static __inline td_u8 ot_ext_system_isp_mesh_shading_inner_update_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_lsc_base(vi_pipe)  +  0x4462);
}

static __inline td_void ot_ext_system_isp_mesh_shading_attr_inner_update_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_lsc_base(vi_pipe)  +  0x4463, data);
}
static __inline td_u8 ot_ext_system_isp_mesh_shading_attr_inner_update_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_lsc_base(vi_pipe)  +  0x4463);
}

/* ACS */
static __inline td_void ot_ext_system_isp_acs_enable_write(ot_vi_pipe vi_pipe,  td_u8 data)
{
    iowr_8direct(vreg_lsc_base(vi_pipe) + 0x4870, data & 0x1);
}

static __inline td_u8 ot_ext_system_isp_acs_enable_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_lsc_base(vi_pipe) + 0x4870) & 0x1);
}

static __inline td_void ot_ext_system_isp_acs_y_strength_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_lsc_base(vi_pipe) + 0x4872, data);
}

static __inline td_u16 ot_ext_system_isp_acs_y_strength_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_lsc_base(vi_pipe) + 0x4872);
}

static __inline td_void ot_ext_system_isp_acs_run_interval_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_lsc_base(vi_pipe) + 0x4874, data);
}

static __inline td_u16 ot_ext_system_isp_acs_run_interval_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_lsc_base(vi_pipe) + 0x4874);
}

static __inline td_void ot_ext_system_isp_acs_lock_enable_write(ot_vi_pipe vi_pipe,  td_u8 data)
{
    iowr_8direct(vreg_lsc_base(vi_pipe) + 0x4876, data & 0x1);
}

static __inline td_u8 ot_ext_system_isp_acs_lock_enable_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_lsc_base(vi_pipe) + 0x4876) & 0x1);
}

static __inline td_void ot_ext_system_isp_acs_attr_updata_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_lsc_base(vi_pipe) + 0x4878, data & 0x1);
}

static __inline td_u8 ot_ext_system_isp_acs_attr_updata_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_lsc_base(vi_pipe) + 0x4878) & 0x1);
}

static __inline td_void ot_ext_system_isp_acs_lsc_lut_attr_updata_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_lsc_base(vi_pipe) + 0x487A, data);
}

static __inline td_u8 ot_ext_system_isp_acs_lsc_lut_attr_updata_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_lsc_base(vi_pipe) + 0x487A);
}

static __inline td_void ot_ext_system_isp_acs_init_status_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_lsc_base(vi_pipe) + 0x487B, (data & 0x01));
}

static __inline td_u8 ot_ext_system_isp_acs_init_status_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_lsc_base(vi_pipe) + 0x487B) & 0x01);
}

/* YUV SHARPEN */
/* sharpening core */
#define OT_EXT_SYSTEM_SHARPEN_MANU_MODE_DEFAULT 0
/* args: data (8-bit) */
static __inline td_void ot_ext_system_sharpen_manu_mode_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe)  +  0x000, data & 0x01);
}
static __inline td_u8 ot_ext_system_sharpen_manu_mode_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe)  +  0x000) & 0x01);
}

static __inline td_void ot_ext_system_sharpen_motion_en_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe) + 0x001, data & 0x01);
}
static __inline td_u8 ot_ext_system_sharpen_motion_en_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe) + 0x001) & 0x01);
}

#define     OT_EXT_SYSTEM_MANUAL_SHARPEN_EN_DEFAULT     1
static __inline td_void ot_ext_system_manual_sharpen_en_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe)  +  0x002, data & 0x01);
}
static __inline td_u8 ot_ext_system_manual_sharpen_en_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe)  +  0x002) & 0x01);
}

#define     OT_EXT_SYSTEM_SHARPEN_MPI_UPDATE_EN_DEFAULT     0
static __inline td_void ot_ext_system_sharpen_mpi_update_en_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe)  +  0x003, data & 0x01);
}
static __inline td_u8 ot_ext_system_sharpen_mpi_update_en_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe)  +  0x003) & 0x01);
}

#define     OT_EXT_SYSTEM_MANUAL_SHARPEN_TEXTURESTR_DEF     300
static __inline td_void ot_ext_system_manual_sharpen_texture_str_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct(vreg_sharpen_base(vi_pipe)  +  0x004  + (index * sizeof(td_u16)), data & 0xFFF);
}
static __inline td_u16 ot_ext_system_manual_sharpen_texture_str_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return (iord_16direct(vreg_sharpen_base(vi_pipe)  +  0x004  + (index * sizeof(td_u16))) & 0xFFF);
}

#define     OT_EXT_SYSTEM_MANUAL_SHARPEN_EDGESTR_DEF        400
static __inline td_void ot_ext_system_manual_sharpen_edge_str_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct(vreg_sharpen_base(vi_pipe)  +  0x044  + (index * sizeof(td_u16)), data & 0xFFF);
}
static __inline td_u16 ot_ext_system_manual_sharpen_edge_str_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return (iord_16direct(vreg_sharpen_base(vi_pipe)  +  0x044  + (index * sizeof(td_u16))) & 0xFFF);
}

#define     OT_EXT_SYSTEM_MANUAL_SHARPEN_TEXTUREFREQ_DEF        128
static __inline td_void ot_ext_system_manual_sharpen_texture_freq_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_sharpen_base(vi_pipe)  +  0x084, data & 0xFFF);
}
static __inline td_u16 ot_ext_system_manual_sharpen_texture_freq_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_sharpen_base(vi_pipe)  +  0x084) & 0xFFF);
}

#define     OT_EXT_SYSTEM_MANUAL_SHARPEN_EDGEFREQ_DEF       96
static __inline td_void ot_ext_system_manual_sharpen_edge_freq_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_sharpen_base(vi_pipe)  +  0x086, data & 0xFFF);
}
static __inline td_u16 ot_ext_system_manual_sharpen_edge_freq_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_sharpen_base(vi_pipe)  +  0x086) & 0xFFF);
}

#define     OT_EXT_SYSTEM_MANUAL_SHARPEN_OVERSHOOT_DEF     100
static __inline td_void ot_ext_system_manual_sharpen_over_shoot_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe)  +  0x088, data & 0x7f);
}
static __inline td_u8 ot_ext_system_manual_sharpen_over_shoot_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe)  +  0x088) & 0x7f);
}

#define     OT_EXT_SYSTEM_MANUAL_SHARPEN_UNDERSHOOT_DEF     127
static __inline td_void ot_ext_system_manual_sharpen_under_shoot_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe)  +  0x089, data & 0x7f);
}
static __inline td_u8 ot_ext_system_manual_sharpen_under_shoot_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe)  +  0x089) & 0x7f);
}

#define     OT_EXT_SYSTEM_MANUAL_SHARPEN_SHOOTSUPSTR_DEF        60
static __inline td_void ot_ext_system_manual_sharpen_shoot_sup_str_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe)  +  0x08a, data);
}
static __inline td_u8 ot_ext_system_manual_sharpen_shoot_sup_str_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe)  +  0x08a));
}

#define     OT_EXT_SYSTEM_MANUAL_SHARPEN_DETAILCTRL_DEF     128
static __inline td_void ot_ext_system_manual_sharpen_detailctrl_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe)  +  0x08b, data);
}
static __inline td_u8 ot_ext_system_manual_sharpen_detailctrl_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe)  +  0x08b));
}

#define     OT_EXT_SYSTEM_MANUAL_SHARPEN_EDGEFILTSTR_DEF        53
static __inline td_void ot_ext_system_manual_sharpen_edge_filt_str_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe)  +  0x08c, data & 0x3f);
}
static __inline td_u8 ot_ext_system_manual_sharpen_edge_filt_str_read(ot_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_sharpen_base(vi_pipe)  +  0x08c)) & 0x3f);
}

/* register: ot_ext_system_manual_sharpen_luma_wgt */
/* sharpening manual luma_wgt table length:32*1 BYTE */
/* args: data (8-bit) */
/* 32 multi addrs */
#define     OT_EXT_SYSTEM_MANUAL_SHARPEN_LUMAWGT_DEF        (OT_ISP_SHARPEN_LUMAWGT)
static __inline td_void ot_ext_system_manual_sharpen_luma_wgt_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe)  +  0x08e  + (index * sizeof(td_u8)), data & OT_ISP_SHARPEN_LUMAWGT_BIT);
}
static __inline td_u8 ot_ext_system_manual_sharpen_luma_wgt_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe)  +  0x08e  + (index * sizeof(td_u8))) & OT_ISP_SHARPEN_LUMAWGT_BIT);
}

/* r_gain: data (8-bit) */
#define     OT_EXT_SYSTEM_MANUAL_SHARPEN_RGAIN_DEF      (OT_ISP_SHARPEN_RGAIN)
static __inline td_void ot_ext_system_manual_sharpen_r_gain_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe)  +  0xae, data & OT_ISP_SHARPEN_RGAIN_BIT);
}
static __inline td_u8 ot_ext_system_manual_sharpen_r_gain_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe)  +  0xae) & OT_ISP_SHARPEN_RGAIN_BIT);
}

/* b_gain: data (8-bit) */
#define     OT_EXT_SYSTEM_MANUAL_SHARPEN_BGAIN_DEF      (OT_ISP_SHARPEN_BGAIN)
static __inline td_void ot_ext_system_manual_sharpen_b_gain_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe)  +  0xaf, data & OT_ISP_SHARPEN_BGAIN_BIT);
}
static __inline td_u8 ot_ext_system_manual_sharpen_b_gain_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe)  +  0xaf) & OT_ISP_SHARPEN_BGAIN_BIT);
}

/* skin_gain: data (8-bit) */
#define     OT_EXT_SYSTEM_MANUAL_SHARPEN_SKINGAIN_DEF       31
static __inline td_void ot_ext_system_manual_sharpen_skin_gain_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe)  +  0xb0, data & 0x1f);
}
static __inline td_u8 ot_ext_system_manual_sharpen_skin_gain_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe)  +  0xb0) & 0x1f);
}

/* edge_filt_max_cap: data (8-bit) */
#define     OT_EXT_SYSTEM_MANUAL_SHARPEN_EDGEFILTMAXCAP_DEF        (OT_ISP_SHARPEN_EDGE_FILT_MAX_CAP)
static __inline td_void ot_ext_system_manual_sharpen_edge_filt_max_cap_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe)  +  0xb1, data & OT_ISP_SHARPEN_EDGE_FILT_MAX_CAP_BIT);
}
static __inline td_u8 ot_ext_system_manual_sharpen_edge_filt_max_cap_read(ot_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_sharpen_base(vi_pipe)  +  0xb1)) & OT_ISP_SHARPEN_EDGE_FILT_MAX_CAP_BIT);
}

/* auto 16 */
/* auoto 32 x 16 multi addrs */
static __inline td_void ot_ext_system_sharpen_texture_str_write(ot_vi_pipe vi_pipe, td_u16 index, td_u16 data)
{
    iowr_16direct((vreg_sharpen_base(vi_pipe)  +  0xb2  +  index * sizeof(td_u16)), data & 0xFFF);
}
static __inline td_u16 ot_ext_system_sharpen_texture_str_read(ot_vi_pipe vi_pipe,  td_u16 index)
{
    return ((iord_16direct((vreg_sharpen_base(vi_pipe)  +  0xb2  +  index * sizeof(td_u16)))) & 0xFFF);
}

/* auoto 32 x 16 multi addrs */
static __inline td_void ot_ext_system_sharpen_edge_str_write(ot_vi_pipe vi_pipe, td_u16 index, td_u16 data)
{
    iowr_16direct((vreg_sharpen_base(vi_pipe)  +  0x4b2  +  index * sizeof(td_u16)), data & 0xFFF);
}
static __inline td_u16 ot_ext_system_sharpen_edge_str_read(ot_vi_pipe vi_pipe,  td_u16 index)
{
    return ((iord_16direct((vreg_sharpen_base(vi_pipe)  +  0x4b2  +  index * sizeof(td_u16)))) & 0xFFF);
}

/* auoto 16 multi addrs */
static __inline td_void ot_ext_system_sharpen_texture_freq_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct((vreg_sharpen_base(vi_pipe)  +  0x8b2  +  index * sizeof(td_u16)), data & 0xFFF);
}
static __inline td_u16 ot_ext_system_sharpen_texture_freq_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return ((iord_16direct((vreg_sharpen_base(vi_pipe)  +  0x8b2  +  index * sizeof(td_u16)))) & 0xFFF);
}

/* auoto 16 multi addrs */
static __inline td_void ot_ext_system_sharpen_edge_freq_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct((vreg_sharpen_base(vi_pipe)  +  0x8d2  +  index * sizeof(td_u16)), data & 0xFFF);
}
static __inline td_u16 ot_ext_system_sharpen_edge_freq_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return ((iord_16direct((vreg_sharpen_base(vi_pipe)  +  0x8d2  +  index * sizeof(td_u16)))) & 0xFFF);
}

/* auoto 16 multi addrs */
static __inline td_void ot_ext_system_sharpen_over_shoot_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_sharpen_base(vi_pipe)  +  0x8f2  +  index * sizeof(td_u8)), data & 0x7f);
}
static __inline td_u8 ot_ext_system_sharpen_over_shoot_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return ((iord_8direct((vreg_sharpen_base(vi_pipe)  +  0x8f2  +  index * sizeof(td_u8)))) & 0x7f);
}

/* auoto 16 multi addrs */
static __inline td_void ot_ext_system_sharpen_under_shoot_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_sharpen_base(vi_pipe)  +  0x902  +  index * sizeof(td_u8)), data & 0x7f);
}
static __inline td_u8 ot_ext_system_sharpen_under_shoot_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return ((iord_8direct((vreg_sharpen_base(vi_pipe)  +  0x902  +  index * sizeof(td_u8)))) & 0x7f);
}

/* auoto 16 multi addrs */
static __inline td_void ot_ext_system_sharpen_shoot_sup_str_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_sharpen_base(vi_pipe)  +  0x912  +  index * sizeof(td_u8)), data);
}
static __inline td_u8 ot_ext_system_sharpen_shoot_sup_str_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return ((iord_8direct((vreg_sharpen_base(vi_pipe)  +  0x912  +  index * sizeof(td_u8)))));
}

/* auoto 16 multi addrs */
static __inline td_void ot_ext_system_sharpen_detailctrl_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_sharpen_base(vi_pipe)  +  0x922  +  index * sizeof(td_u8)), data);
}
static __inline td_u8 ot_ext_system_sharpen_detailctrl_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return ((iord_8direct((vreg_sharpen_base(vi_pipe)  +  0x922  +  index * sizeof(td_u8)))));
}

/* auoto 16 multi addrs */
static __inline td_void ot_ext_system_sharpen_r_gain_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_sharpen_base(vi_pipe)  +  0x932  +  index * sizeof(td_u8)), data & OT_ISP_SHARPEN_RGAIN_BIT);
}
static __inline td_u8 ot_ext_system_sharpen_r_gain_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return ((iord_8direct((vreg_sharpen_base(vi_pipe)  +  0x932  +  index * sizeof(td_u8)))) &
        OT_ISP_SHARPEN_RGAIN_BIT);
}

/* auoto 16 multi addrs */
static __inline td_void ot_ext_system_sharpen_b_gain_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_sharpen_base(vi_pipe)  +  0x942  +  index * sizeof(td_u8)), data & OT_ISP_SHARPEN_BGAIN_BIT);
}
static __inline td_u8 ot_ext_system_sharpen_b_gain_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return ((iord_8direct((vreg_sharpen_base(vi_pipe)  +  0x942  +  index * sizeof(td_u8)))) &
        OT_ISP_SHARPEN_BGAIN_BIT);
}

/* auoto 16 multi addrs */
static __inline td_void ot_ext_system_sharpen_skin_gain_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_sharpen_base(vi_pipe)  +  0x952  +  index * sizeof(td_u8)), data & 0x1f);
}
static __inline td_u8 ot_ext_system_sharpen_skin_gain_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return ((iord_8direct((vreg_sharpen_base(vi_pipe)  +  0x952  +  index * sizeof(td_u8)))) & 0x1f);
}

/* auoto 16 multi addrs */
static __inline td_void ot_ext_system_sharpen_edge_filt_str_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_sharpen_base(vi_pipe)  +  0x962  +  index * sizeof(td_u8)), data & 0x3f);
}
static __inline td_u8 ot_ext_system_sharpen_edge_filt_str_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return ((iord_8direct((vreg_sharpen_base(vi_pipe)  +  0x962  +  index * sizeof(td_u8)))) & 0x3f);
}

/* auoto 32 x 16 multi addrs */
static __inline td_void ot_ext_system_sharpen_luma_wgt_write(ot_vi_pipe vi_pipe, td_u16 index, td_u8 data)
{
    iowr_8direct((vreg_sharpen_base(vi_pipe) + 0x972 +  index * sizeof(td_u8)), data & OT_ISP_SHARPEN_LUMAWGT_BIT);
}
static __inline td_u16 ot_ext_system_sharpen_luma_wgt_read(ot_vi_pipe vi_pipe,  td_u16 index)
{
    return ((iord_8direct((vreg_sharpen_base(vi_pipe) + 0x972 + index * sizeof(td_u8)))) & OT_ISP_SHARPEN_LUMAWGT_BIT);
}

/* new add manual */
#define     OT_EXT_SYSTEM_MANUAL_SHARPEN_DETAILCTRLTHR_DEF     160

static __inline td_void ot_ext_system_manual_sharpen_detailctrl_thr_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe)  +  0xB80, data);
}
static __inline td_u8 ot_ext_system_manual_sharpen_detailctrl_thr_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe)  +  0xB80));
}

#define     OT_EXT_SYSTEM_MANUAL_SHARPEN_SHOOTSUPADJ_DEF     9

static __inline td_void ot_ext_system_manual_sharpen_shoot_sup_adj_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe)  +  0xB81, data & 0x0f);
}
static __inline td_u8 ot_ext_system_manual_sharpen_shoot_sup_adj_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe)  +  0xB81) & 0x0f);
}

#define     OT_EXT_SYSTEM_MANUAL_SHARPEN_MAXSHARPGAIN_DEF     160

static __inline td_void ot_ext_system_manual_sharpen_max_sharp_gain_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_sharpen_base(vi_pipe)  +  0xB82, data & 0x7FF);
}
static __inline td_u16 ot_ext_system_manual_sharpen_max_sharp_gain_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_sharpen_base(vi_pipe)  +  0xB82) & 0x7FF);
}

static __inline td_void ot_ext_system_isp_sharpen_init_status_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe) + 0xB84, (data & 0x01));
}

static __inline td_u8 ot_ext_system_isp_sharpen_init_status_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe) + 0xB84) & 0x01);
}

#define     OT_EXT_SYSTEM_MANUAL_SHARPEN_SKINUMIN_DEF     100

static __inline td_void ot_ext_system_manual_sharpen_skin_umin_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe)  +  0xB86, data);
}
static __inline td_u8 ot_ext_system_manual_sharpen_skin_umin_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe)  +  0xB86));
}

#define     OT_EXT_SYSTEM_MANUAL_SHARPEN_SKINVMIN_DEF     135

static __inline td_void ot_ext_system_manual_sharpen_skin_vmin_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe)  +  0xB87, data);
}
static __inline td_u8 ot_ext_system_manual_sharpen_skin_vmin_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe)  +  0xB87));
}

#define     OT_EXT_SYSTEM_MANUAL_SHARPEN_SKINUMAX_DEF     127

static __inline td_void ot_ext_system_manual_sharpen_skin_umax_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe)  +  0xB88, data);
}
static __inline td_u8 ot_ext_system_manual_sharpen_skin_umax_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe)  +  0xB88));
}

#define     OT_EXT_SYSTEM_MANUAL_SHARPEN_SKINVMAX_DEF     160

static __inline td_void ot_ext_system_manual_sharpen_skin_vmax_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe)  +  0xB89, data);
}
static __inline td_u8 ot_ext_system_manual_sharpen_skin_vmax_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe) + 0xB89));
}
/* g_gain: data (8-bit) */
#define     OT_EXT_SYSTEM_MANUAL_SHARPEN_GGAIN_DEF      (OT_ISP_SHARPEN_GGAIN)

static __inline td_void ot_ext_system_manual_sharpen_g_gain_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe) + 0xB8A, data & OT_ISP_SHARPEN_GGAIN_BIT);
}
static __inline td_u8 ot_ext_system_manual_sharpen_g_gain_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe) + 0xB8A) & OT_ISP_SHARPEN_GGAIN_BIT);
}
#define     OT_EXT_SYSTEM_MANUAL_SHARPEN_WEAKDETAILGAIN_DEF     0
static __inline td_void ot_ext_system_manual_sharpen_weak_detail_gain_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe)  +  0xB8b, data & 0x7F);
}
static __inline td_u8 ot_ext_system_manual_sharpen_weak_detail_gain_read(ot_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_sharpen_base(vi_pipe)  +  0xB8b)) & 0x7F);
}
/* new add auto */
static __inline td_void ot_ext_system_sharpen_detailctrl_thr_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_sharpen_base(vi_pipe)  +  0xB8C  +  index * sizeof(td_u8)), data);
}
static __inline td_u8 ot_ext_system_sharpen_detailctrl_thr_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return ((iord_8direct((vreg_sharpen_base(vi_pipe)  +  0xB8C  +  index * sizeof(td_u8)))));
}

static __inline td_void ot_ext_system_sharpen_max_sharp_gain_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct((vreg_sharpen_base(vi_pipe)  +  0xBaC  +  index * sizeof(td_u16)), data & 0x7ff);
}
static __inline td_u16 ot_ext_system_sharpen_max_sharp_gain_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return ((iord_16direct((vreg_sharpen_base(vi_pipe)  +  0xBaC  +  index * sizeof(td_u16)))) & 0x7ff);
}

static __inline td_void ot_ext_system_sharpen_shoot_sup_adj_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_sharpen_base(vi_pipe)  +  0xBCC  +  index * sizeof(td_u8)), data & 0x0f);
}
static __inline td_u8 ot_ext_system_sharpen_shoot_sup_adj_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return ((iord_8direct((vreg_sharpen_base(vi_pipe)  +  0xBCC  +  index * sizeof(td_u8)))) & 0x0f);
}

static __inline td_void ot_ext_system_sharpen_weak_detail_gain_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_sharpen_base(vi_pipe)  +  0xBDC  +  index * sizeof(td_u8)), data & 0x7f);
}
static __inline td_u8 ot_ext_system_sharpen_weak_detail_gain_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return ((iord_8direct((vreg_sharpen_base(vi_pipe)  +  0xBDC  +  index * sizeof(td_u8)))) & 0x7f);
}
static __inline td_void ot_ext_system_sharpen_g_gain_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_sharpen_base(vi_pipe)  +  0xBEC  +  index * sizeof(td_u8)), data & OT_ISP_SHARPEN_GGAIN_BIT);
}
static __inline td_u8 ot_ext_system_sharpen_g_gain_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return ((iord_8direct((vreg_sharpen_base(vi_pipe) + 0xBEC + index * sizeof(td_u8)))) & OT_ISP_SHARPEN_GGAIN_BIT);
}

/* auoto 16 multi addrs */
static __inline td_void ot_ext_system_sharpen_edge_filt_max_cap_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_sharpen_base(vi_pipe) + 0xBFC + index * sizeof(td_u8)),
                 data & OT_ISP_SHARPEN_EDGE_FILT_MAX_CAP_BIT);
}
static __inline td_u8 ot_ext_system_sharpen_edge_filt_max_cap_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe) + 0xBFC + index * sizeof(td_u8)) &
            OT_ISP_SHARPEN_EDGE_FILT_MAX_CAP_BIT);
}

static __inline td_void ot_ext_system_sharpen_detail_map_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe) + 0xC1C, data);
}
static __inline td_u8 ot_ext_system_sharpen_detail_map_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_sharpen_base(vi_pipe) + 0xC1C);
}

static __inline td_void ot_ext_system_sharpen_motion_thr0_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe) + 0xC1D, data & 0xF);
}
static __inline td_u8 ot_ext_system_sharpen_motion_thr0_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe) + 0xC1D)  & 0xF);
}

static __inline td_void ot_ext_system_sharpen_motion_thr1_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe) + 0xC1E, data & 0xF);
}
static __inline td_u8 ot_ext_system_sharpen_motion_thr1_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe) + 0xC1E)  & 0xF);
}

static __inline td_void ot_ext_system_sharpen_motion_gain0_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_sharpen_base(vi_pipe) + 0xC20, data);
}
static __inline td_u16 ot_ext_system_sharpen_motion_gain0_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_sharpen_base(vi_pipe) + 0xC20);
}

static __inline td_void ot_ext_system_sharpen_motion_gain1_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_sharpen_base(vi_pipe) + 0xC22, data);
}
static __inline td_u16 ot_ext_system_sharpen_motion_gain1_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_sharpen_base(vi_pipe) + 0xC22);
}

/* auto 16 */
/* auoto 32 x 16 multi addrs */
static __inline td_void ot_ext_system_sharpen_auto_mot_texture_str_write(ot_vi_pipe vi_pipe, td_u16 index, td_u16 data)
{
    iowr_16direct((vreg_sharpen_base(vi_pipe) + 0xC24 + index * sizeof(td_u16)), data & 0xFFF);
}
static __inline td_u16 ot_ext_system_sharpen_auto_mot_texture_str_read(ot_vi_pipe vi_pipe,  td_u16 index)
{
    return ((iord_16direct((vreg_sharpen_base(vi_pipe) + 0xC24 + index * sizeof(td_u16)))) & 0xFFF);
}

/* auoto 32 x 16 multi addrs */
static __inline td_void ot_ext_system_sharpen_auto_mot_edge_str_write(ot_vi_pipe vi_pipe, td_u16 index, td_u16 data)
{
    iowr_16direct((vreg_sharpen_base(vi_pipe) + 0x1024 + index * sizeof(td_u16)), data & 0xFFF);
}
static __inline td_u16 ot_ext_system_sharpen_auto_mot_edge_str_read(ot_vi_pipe vi_pipe,  td_u16 index)
{
    return ((iord_16direct((vreg_sharpen_base(vi_pipe) + 0x1024 + index * sizeof(td_u16)))) & 0xFFF);
}

/* auoto 16 multi addrs */
static __inline td_void ot_ext_system_sharpen_auto_mot_texture_freq_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct((vreg_sharpen_base(vi_pipe) + 0x1424 + index * sizeof(td_u16)), data & 0xFFF);
}
static __inline td_u16 ot_ext_system_sharpen_auto_mot_texture_freq_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return ((iord_16direct((vreg_sharpen_base(vi_pipe) + 0x1424 + index * sizeof(td_u16)))) & 0xFFF);
}

/* auoto 16 multi addrs */
static __inline td_void ot_ext_system_sharpen_auto_mot_edge_freq_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct((vreg_sharpen_base(vi_pipe) + 0x1444 + index * sizeof(td_u16)), data & 0xFFF);
}
static __inline td_u16 ot_ext_system_sharpen_auto_mot_edge_freq_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return ((iord_16direct((vreg_sharpen_base(vi_pipe) + 0x1444 + index * sizeof(td_u16)))) & 0xFFF);
}

/* auoto 16 multi addrs */
static __inline td_void ot_ext_system_sharpen_auto_mot_over_shoot_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_sharpen_base(vi_pipe) + 0x1464 + index * sizeof(td_u8)), data & 0x7f);
}
static __inline td_u8 ot_ext_system_sharpen_auto_mot_over_shoot_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return ((iord_8direct((vreg_sharpen_base(vi_pipe) + 0x1464 + index * sizeof(td_u8)))) & 0x7f);
}

/* auoto 16 multi addrs */
static __inline td_void ot_ext_system_sharpen_auto_mot_under_shoot_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_sharpen_base(vi_pipe) + 0x1474 + index * sizeof(td_u8)), data & 0x7f);
}
static __inline td_u8 ot_ext_system_sharpen_auto_mot_under_shoot_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return ((iord_8direct((vreg_sharpen_base(vi_pipe) + 0x1474 + index * sizeof(td_u8)))) & 0x7f);
}
/* manual 32 * 2 multi addrs */
static __inline td_void ot_ext_system_sharpen_manual_mot_texture_str_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct(vreg_sharpen_base(vi_pipe) + 0x1484 + (index * sizeof(td_u16)), data & 0xFFF);
}
static __inline td_u16 ot_ext_system_sharpen_manual_mot_texture_str_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return (iord_16direct(vreg_sharpen_base(vi_pipe) + 0x1484 + (index * sizeof(td_u16))) & 0xFFF);
}

static __inline td_void ot_ext_system_sharpen_manual_mot_edge_str_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct(vreg_sharpen_base(vi_pipe) + 0x14C4 + (index * sizeof(td_u16)), data & 0xFFF);
}
static __inline td_u16 ot_ext_system_sharpen_manual_mot_edge_str_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return (iord_16direct(vreg_sharpen_base(vi_pipe) + 0x14C4 + (index * sizeof(td_u16))) & 0xFFF);
}

static __inline td_void ot_ext_system_sharpen_manual_mot_texture_freq_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_sharpen_base(vi_pipe) + 0x1504, data & 0xFFF);
}
static __inline td_u16 ot_ext_system_sharpen_manual_mot_texture_freq_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_sharpen_base(vi_pipe) + 0x1504) & 0xFFF);
}

static __inline td_void ot_ext_system_sharpen_manual_mot_edge_freq_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_sharpen_base(vi_pipe) + 0x1506, data & 0xFFF);
}
static __inline td_u16 ot_ext_system_sharpen_manual_mot_edge_freq_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_sharpen_base(vi_pipe) + 0x1506) & 0xFFF);
}

static __inline td_void ot_ext_system_sharpen_manual_mot_over_shoot_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe) + 0x1508, data & 0x7f);
}
static __inline td_u8 ot_ext_system_sharpen_manual_mot_over_shoot_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe) + 0x1508) & 0x7f);
}

static __inline td_void ot_ext_system_sharpen_manual_mot_under_shoot_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe) + 0x1509, data & 0x7f);
}
static __inline td_u8 ot_ext_system_sharpen_manual_mot_under_shoot_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe) + 0x1509) & 0x7f);
}

/* CAC */
/* register: ot_ext_system_local_cac_enable */
/* local_cac_enable */
static __inline td_void ot_ext_system_local_cac_enable_write(ot_vi_pipe vi_pipe,  td_u8 data)
{
    iowr_8direct(vreg_lcac_base(vi_pipe), data & 0x1);
}

static __inline td_u16 ot_ext_system_local_cac_enable_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_lcac_base(vi_pipe)) & 0x1);
}
/* register: ot_ext_system_local_cac_manual_mode_enable */
/* local_cac_manual_mode_enable */
static __inline td_void ot_ext_system_local_cac_manual_mode_enable_write(ot_vi_pipe vi_pipe,  td_u8 data)
{
    iowr_8direct(vreg_lcac_base(vi_pipe)  +  1, data & 0x1);
}

static __inline td_u8 ot_ext_system_local_cac_manual_mode_enable_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_lcac_base(vi_pipe)  +  1) & 0x1);
}

/* register: ot_ext_system_local_cac_purple_det_range */
/* local_cac_purple_det_range */
static __inline td_void ot_ext_system_local_cac_purple_det_range_write(ot_vi_pipe vi_pipe,  td_u16 data)
{
    iowr_16direct(vreg_lcac_base(vi_pipe)  +  0x2, data);
}

static __inline td_u16 ot_ext_system_local_cac_purple_det_range_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_lcac_base(vi_pipe)  +  0x2);
}

/* register: ot_ext_system_local_cac_purple_var_thr */
/* local_cac_purple_var_thr */
static __inline td_void ot_ext_system_local_cac_purple_var_thr_write(ot_vi_pipe vi_pipe,  td_u16 data)
{
    iowr_16direct(vreg_lcac_base(vi_pipe)  +  0x4, data);
}

static __inline td_u16 ot_ext_system_local_cac_purple_var_thr_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_lcac_base(vi_pipe)  +  0x4);
}

/* register: ot_ext_system_local_cac_manual_cb_str */
/* local_cac_manual_cb */
static __inline td_void ot_ext_system_local_cac_manual_cb_str_write(ot_vi_pipe vi_pipe,  td_u8 data)
{
    iowr_8direct(vreg_lcac_base(vi_pipe)  +  0x6, data);
}

static __inline td_u8 ot_ext_system_local_cac_manual_cb_str_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_lcac_base(vi_pipe)  +  0x6);
}

/* register: ot_ext_system_local_cac_manual_cr_str */
/* local_cac_manual_cr */
static __inline td_void ot_ext_system_local_cac_manual_cr_str_write(ot_vi_pipe vi_pipe,  td_u8 data)
{
    iowr_8direct(vreg_lcac_base(vi_pipe)  +  0x7, data);
}

static __inline td_u8 ot_ext_system_local_cac_manual_cr_str_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_lcac_base(vi_pipe)  +  0x7);
}

/* register: ot_ext_system_local_cac_cb_str_table */
/* local_cac_cb_str_table */
static __inline td_void ot_ext_system_local_cac_auto_cb_str_table_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct(vreg_lcac_base(vi_pipe)  +  0x8  + (index * sizeof(td_u8)), data);
}

static __inline td_u8 ot_ext_system_local_cac_auto_cb_str_table_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return iord_8direct(vreg_lcac_base(vi_pipe)  +  0x8  + (index * sizeof(td_u8)));
}

/* register: ot_ext_system_local_cac_cr_str_table */
/* local_cac_cr_str_table */
static __inline td_void ot_ext_system_local_cac_auto_cr_str_table_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct(vreg_lcac_base(vi_pipe)  +  0x18  + (index * sizeof(td_u8)), data);
}

static __inline td_u8 ot_ext_system_local_cac_auto_cr_str_table_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return iord_8direct(vreg_lcac_base(vi_pipe)  +  0x18  + (index * sizeof(td_u8)));
}

/* register: ot_ext_system_local_cac_coef_update_en */
/* local_cac_coef_update_en */
static __inline td_void ot_ext_system_local_cac_coef_update_en_write(ot_vi_pipe vi_pipe,  td_u8 data)
{
    iowr_8direct(vreg_lcac_base(vi_pipe)  +  0x28, data & 0x1);
}

static __inline td_u8 ot_ext_system_local_cac_coef_update_en_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_lcac_base(vi_pipe)  +  0x28) & 0x1);
}

/* register: ot_ext_system_local_cac_luma_high_cnt_thr */
static __inline td_void ot_ext_system_local_cac_luma_high_cnt_thr_write(ot_vi_pipe vi_pipe,  td_u8 data)
{
    iowr_8direct(vreg_lcac_base(vi_pipe)  +  0x29, data);
}

static __inline td_u8 ot_ext_system_local_cac_luma_high_cnt_thr_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_lcac_base(vi_pipe)  +  0x29);
}

/* register: ot_ext_system_local_cac_cb_cnt_high_thr */
static __inline td_void ot_ext_system_local_cac_cb_cnt_high_thr_write(ot_vi_pipe vi_pipe,    td_u8 data)
{
    iowr_8direct(vreg_lcac_base(vi_pipe)  +  0x2a, data);
}

static __inline td_u8 ot_ext_system_local_cac_cb_cnt_high_thr_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_lcac_base(vi_pipe)  +  0x2a);
}
/* register: ot_ext_system_local_cac_cb_cnt_low_thr */
static __inline td_void ot_ext_system_local_cac_cb_cnt_low_thr_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_lcac_base(vi_pipe)  +  0x2b, data);
}

static __inline td_u8 ot_ext_system_local_cac_cb_cnt_low_thr_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_lcac_base(vi_pipe)  +  0x2b);
}

/* register: ot_ext_system_local_cac_luma_thr */
static __inline td_void ot_ext_system_local_cac_luma_thr_write(ot_vi_pipe vi_pipe,   td_u16 data)
{
    iowr_16direct(vreg_lcac_base(vi_pipe)  + 0x2c, data);
}

static __inline td_u16 ot_ext_system_local_cac_luma_thr_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_lcac_base(vi_pipe) +  0x2c);
}

/* register: ot_ext_system_local_cac_cb_thr */
static __inline td_void ot_ext_system_local_cac_cb_thr_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_lcac_base(vi_pipe)  + 0x2e, data);
}

static __inline td_u16 ot_ext_system_local_cac_cb_thr_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_lcac_base(vi_pipe) +  0x2e);
}

/* register: ot_ext_system_local_cac_bld_avg_cur */
static __inline td_void ot_ext_system_local_cac_bld_avg_cur_write(ot_vi_pipe vi_pipe,    td_u8 data)
{
    iowr_8direct(vreg_lcac_base(vi_pipe)  +  0x30, data);
}

static __inline td_u8 ot_ext_system_local_cac_bld_avg_cur_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_lcac_base(vi_pipe)  +  0x30);
}

/* register: ot_ext_system_local_cac_defcolor_cr */
static __inline td_void ot_ext_system_local_cac_defcolor_cr_write(ot_vi_pipe vi_pipe,    td_u8 data)
{
    iowr_8direct(vreg_lcac_base(vi_pipe)  +  0x31, data);
}

static __inline td_u8 ot_ext_system_local_cac_defcolor_cr_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_lcac_base(vi_pipe)  +  0x31);
}

/* register: ot_ext_system_local_cac_defcolor_cb */
static __inline td_void ot_ext_system_local_cac_defcolor_cb_write(ot_vi_pipe vi_pipe,    td_u8 data)
{
    iowr_8direct(vreg_lcac_base(vi_pipe)  +  0x32, data);
}

static __inline td_u8 ot_ext_system_local_cac_defcolor_cb_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_lcac_base(vi_pipe)  +  0x32);
}

static __inline td_void ot_ext_system_isp_lcac_init_status_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_lcac_base(vi_pipe) + 0x33, (data & 0x01));
}

static __inline td_u8 ot_ext_system_isp_lcac_init_status_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_lcac_base(vi_pipe) + 0x33) & 0x01);
}

/* Register: ot_ext_system_local_cac_r_thd_table */
static __inline td_void ot_ext_system_local_cac_r_thd_table_write(ot_vi_pipe vi_pipe, td_u8 i, td_u16 data)
{
    iowr_16direct(vreg_lcac_base(vi_pipe)  +  0x34  + (i * sizeof(td_u16)), data);
}

static __inline td_u16 ot_ext_system_local_cac_r_thd_table_read(ot_vi_pipe vi_pipe, td_u8 i)
{
    return iord_16direct(vreg_lcac_base(vi_pipe)  +  0x34  + (i * sizeof(td_u16)));
}


/* Register: ot_ext_system_local_cac_g_thd_table */
static __inline td_void ot_ext_system_local_cac_g_thd_table_write(ot_vi_pipe vi_pipe, td_u8 i, td_u16 data)
{
    iowr_16direct(vreg_lcac_base(vi_pipe)  +  0x3a  + (i * sizeof(td_u16)), data);
}

static __inline td_u16 ot_ext_system_local_cac_g_thd_table_read(ot_vi_pipe vi_pipe, td_u8 i)
{
    return iord_16direct(vreg_lcac_base(vi_pipe)  +  0x3a  + (i * sizeof(td_u16)));
}


/* Register: ot_ext_system_local_cac_b_thd_table */
static __inline td_void ot_ext_system_local_cac_b_thd_table_write(ot_vi_pipe vi_pipe, td_u8 i, td_u16 data)
{
    iowr_16direct(vreg_lcac_base(vi_pipe)  +  0x40  + (i * sizeof(td_u16)), data);
}

static __inline td_u16 ot_ext_system_local_cac_b_thd_table_read(ot_vi_pipe vi_pipe, td_u8 i)
{
    return iord_16direct(vreg_lcac_base(vi_pipe)  +  0x40  + (i * sizeof(td_u16)));
}


/* Register: ot_ext_system_local_cac_luma_thd_table */
static __inline td_void ot_ext_system_local_cac_luma_thd_table_write(ot_vi_pipe vi_pipe, td_u8 i, td_u16 data)
{
    iowr_16direct(vreg_lcac_base(vi_pipe)  +  0x46  + (i * sizeof(td_u16)), data);
}

static __inline td_u16 ot_ext_system_local_cac_luma_thd_table_read(ot_vi_pipe vi_pipe, td_u8 i)
{
    return iord_16direct(vreg_lcac_base(vi_pipe)  +  0x46  + (i * sizeof(td_u16)));
}


/* Register: ot_ext_system_local_cac_luma_thd_table */
static __inline td_void ot_ext_system_local_cac_cbcr_ratio_table_write(ot_vi_pipe vi_pipe, td_u8 i, td_s16 data)
{
    iowr_16direct(vreg_lcac_base(vi_pipe)  +  0x4c  + (i * sizeof(td_s16)), data);
}

static __inline td_s16 ot_ext_system_local_cac_cbcr_ratio_table_read(ot_vi_pipe vi_pipe, td_u8 i)
{
    return iord_16direct(vreg_lcac_base(vi_pipe)  +  0x4c  + (i * sizeof(td_s16)));
}


/* ACAC */
static __inline td_void ot_ext_system_acac_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_acac_base(vi_pipe), data);
}

static __inline td_u8 ot_ext_system_acac_enable_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_acac_base(vi_pipe));
}
/* det mode  */
static __inline td_void ot_ext_system_acac_det_mode_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_acac_base(vi_pipe) + 0x1, data);
}

static __inline td_u8 ot_ext_system_acac_det_mode_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_acac_base(vi_pipe) + 0x1);
}

static __inline td_void ot_ext_system_acac_edge_thd_write(ot_vi_pipe vi_pipe, td_u8 i, td_u16 data)
{
    iowr_16direct(vreg_acac_base(vi_pipe) + 0x2 + (i * sizeof(td_u16)), data);
}

static __inline td_u16 ot_ext_system_acac_edge_thd_read(ot_vi_pipe vi_pipe, td_u8 i)
{
    return iord_16direct(vreg_acac_base(vi_pipe) + 0x2 + (i * sizeof(td_u16)));
}

static __inline td_void ot_ext_system_acac_fcc_on_c_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_acac_base(vi_pipe) + 0x6, data);
}

static __inline td_u16 ot_ext_system_acac_fcc_on_c_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_acac_base(vi_pipe) + 0x6);
}

static __inline td_void ot_ext_system_acac_fcc_on_y_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_acac_base(vi_pipe) + 0x8, data);
}

static __inline td_u16 ot_ext_system_acac_fcc_on_y_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_acac_base(vi_pipe) + 0x8);
}

static __inline td_void ot_ext_system_acac_fcc_rb_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_acac_base(vi_pipe) + 0xa, data);
}

static __inline td_u16 ot_ext_system_acac_fcc_rb_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_acac_base(vi_pipe) + 0xa);
}

static __inline td_void ot_ext_system_acac_det_pur_limit_h_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_acac_base(vi_pipe) + 0xc, data);
}

static __inline td_u16 ot_ext_system_acac_det_pur_limit_h_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_acac_base(vi_pipe) + 0xc);
}

static __inline td_void ot_ext_system_acac_det_pur_limit_l_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_acac_base(vi_pipe) + 0xe, data);
}

static __inline td_u16 ot_ext_system_acac_det_pur_limit_l_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_acac_base(vi_pipe) + 0xe);
}

static __inline td_void ot_ext_system_acac_det_pur_sat_thd_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_acac_base(vi_pipe) + 0x10, data);
}

static __inline td_u16 ot_ext_system_acac_det_pur_sat_thd_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_acac_base(vi_pipe) + 0x10);
}

static __inline td_void ot_ext_system_acac_purple_alpha_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_acac_base(vi_pipe) + 0x12, data);
}

static __inline td_u16 ot_ext_system_acac_purple_alpha_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_acac_base(vi_pipe) + 0x12);
}

static __inline td_void ot_ext_system_acac_edge_alpha_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_acac_base(vi_pipe) + 0x14, data);
}

static __inline td_u16 ot_ext_system_acac_edge_alpha_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_acac_base(vi_pipe) + 0x14);
}

static __inline td_void ot_ext_system_acac_edge_gain_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_acac_base(vi_pipe) + 0x16, data);
}

static __inline td_u16 ot_ext_system_acac_edge_gain_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_acac_base(vi_pipe) + 0x16);
}

static __inline td_void ot_ext_system_acac_op_type_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_acac_base(vi_pipe) + 0x1a, data);
}

static __inline td_u8 ot_ext_system_acac_op_type_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_acac_base(vi_pipe) + 0x1a);
}

static __inline td_void ot_ext_system_acac_coef_update_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_acac_base(vi_pipe) + 0x1b, data);
}

static __inline td_u8 ot_ext_system_acac_coef_update_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_acac_base(vi_pipe) + 0x1b);
}

static __inline td_void ot_ext_system_acac_edge_thd_auto_write(ot_vi_pipe vi_pipe, td_u32 i, td_u16 data)
{
    iowr_16direct(vreg_acac_base(vi_pipe) + (i * sizeof(td_u16)) + 0x1c, data);
}

static __inline td_u16 ot_ext_system_acac_edge_thd_auto_read(ot_vi_pipe vi_pipe, td_u32 i)
{
    return iord_16direct(vreg_acac_base(vi_pipe) + (i * sizeof(td_u16)) + 0x1c);
}

static __inline td_void ot_ext_system_acac_fcc_on_c_auto_write(ot_vi_pipe vi_pipe, td_u8 i, td_u16 data)
{
    iowr_16direct(vreg_acac_base(vi_pipe) + (i * sizeof(td_u16)) + 0x5c, data);
}

static __inline td_u16 ot_ext_system_acac_fcc_on_c_auto_read(ot_vi_pipe vi_pipe, td_u8 i)
{
    return iord_16direct(vreg_acac_base(vi_pipe) + (i * sizeof(td_u16)) + 0x5c);
}

static __inline td_void ot_ext_system_acac_fcc_on_y_auto_write(ot_vi_pipe vi_pipe, td_u8 i, td_u16 data)
{
    iowr_16direct(vreg_acac_base(vi_pipe) + (i * sizeof(td_u16)) + 0x7c, data);
}

static __inline td_u16 ot_ext_system_acac_fcc_on_y_auto_read(ot_vi_pipe vi_pipe, td_u8 i)
{
    return iord_16direct(vreg_acac_base(vi_pipe) + (i * sizeof(td_u16)) + 0x7c);
}

static __inline td_void ot_ext_system_acac_fcc_rb_auto_write(ot_vi_pipe vi_pipe, td_u8 i, td_u16 data)
{
    iowr_16direct(vreg_acac_base(vi_pipe) + (i * sizeof(td_u16)) + 0x9c, data);
}

static __inline td_u16 ot_ext_system_acac_fcc_rb_auto_read(ot_vi_pipe vi_pipe, td_u8 i)
{
    return iord_16direct(vreg_acac_base(vi_pipe) + (i * sizeof(td_u16)) + 0x9c);
}

static __inline td_void ot_ext_system_acac_det_pur_limit_h_auto_write(ot_vi_pipe vi_pipe, td_u8 i, td_u16 data)
{
    iowr_16direct(vreg_acac_base(vi_pipe) + (i * sizeof(td_u16)) + 0xbc, data);
}

static __inline td_u16 ot_ext_system_acac_det_pur_limit_h_auto_read(ot_vi_pipe vi_pipe, td_u8 i)
{
    return iord_16direct(vreg_acac_base(vi_pipe) + (i * sizeof(td_u16)) + 0xbc);
}

static __inline td_void ot_ext_system_acac_det_pur_limit_l_auto_write(ot_vi_pipe vi_pipe, td_u8 i, td_u16 data)
{
    iowr_16direct(vreg_acac_base(vi_pipe) + (i * sizeof(td_u16)) + 0xdc, data);
}

static __inline td_u16 ot_ext_system_acac_det_pur_limit_l_auto_read(ot_vi_pipe vi_pipe, td_u8 i)
{
    return iord_16direct(vreg_acac_base(vi_pipe) + (i * sizeof(td_u16)) + 0xdc);
}

static __inline td_void ot_ext_system_acac_det_pur_sat_thd_auto_write(ot_vi_pipe vi_pipe, td_u8 i, td_u16 data)
{
    iowr_16direct(vreg_acac_base(vi_pipe) + (i * sizeof(td_u16)) + 0xfc, data);
}

static __inline td_u16 ot_ext_system_acac_det_pur_sat_thd_auto_read(ot_vi_pipe vi_pipe, td_u8 i)
{
    return iord_16direct(vreg_acac_base(vi_pipe) + (i * sizeof(td_u16)) + 0xfc);
}

static __inline td_void ot_ext_system_acac_purple_alpha_auto_write(ot_vi_pipe vi_pipe, td_u8 i, td_u16 data)
{
    iowr_16direct(vreg_acac_base(vi_pipe) + 0x11c + (i * sizeof(td_u16)), data);
}

static __inline td_u16 ot_ext_system_acac_purple_alpha_auto_read(ot_vi_pipe vi_pipe, td_u8 i)
{
    return iord_16direct(vreg_acac_base(vi_pipe) + 0x11c + (i * sizeof(td_u16)));
}

static __inline td_void ot_ext_system_acac_edge_alpha_auto_write(ot_vi_pipe vi_pipe, td_u8 i, td_u16 data)
{
    iowr_16direct(vreg_acac_base(vi_pipe) + 0x13c + (i * sizeof(td_u16)), data);
}

static __inline td_u16 ot_ext_system_acac_edge_alpha_auto_read(ot_vi_pipe vi_pipe, td_u8 i)
{
    return iord_16direct(vreg_acac_base(vi_pipe) + 0x13c + (i * sizeof(td_u16)));
}

static __inline td_void ot_ext_system_acac_edge_gain_auto_write(ot_vi_pipe vi_pipe, td_u8 i, td_u16 data)
{
    iowr_16direct(vreg_acac_base(vi_pipe) + 0x15c + (i * sizeof(td_u16)), data);
}

static __inline td_u16 ot_ext_system_acac_edge_gain_auto_read(ot_vi_pipe vi_pipe, td_u8 i)
{
    return iord_16direct(vreg_acac_base(vi_pipe) + 0x15c + (i * sizeof(td_u16)));
}

static __inline td_void ot_ext_system_isp_acac_init_status_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_acac_base(vi_pipe) + 0x17c, (data & 0x01));
}

static __inline td_u8 ot_ext_system_isp_acac_init_status_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_acac_base(vi_pipe) + 0x17c) & 0x01);
}


/* CAC ext config */
static __inline td_void ot_ext_system_cac_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_cac_base(vi_pipe), data);
}

static __inline td_u8 ot_ext_system_cac_enable_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_cac_base(vi_pipe));
}

static __inline td_void ot_ext_system_isp_cac_init_status_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_cac_base(vi_pipe) + 0x1, (data & 0x01));
}

static __inline td_u8 ot_ext_system_isp_cac_init_status_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_cac_base(vi_pipe) + 0x1) & 0x01);
}

static __inline td_void ot_ext_system_cac_op_type_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_cac_base(vi_pipe) + 0x2, data);
}

static __inline td_u8 ot_ext_system_cac_op_type_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_cac_base(vi_pipe) + 0x2);
}

static __inline td_void ot_ext_system_cac_det_mode_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_cac_base(vi_pipe) + 0x3, data);
}

static __inline td_u8 ot_ext_system_cac_det_mode_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_cac_base(vi_pipe) + 0x3);
}

static __inline td_void ot_ext_system_cac_purple_upper_limit_write(ot_vi_pipe vi_pipe, td_s16 data)
{
    iowr_16direct(vreg_cac_base(vi_pipe) + 0x4, data);
}

static __inline td_s16 ot_ext_system_cac_purple_upper_limit_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_cac_base(vi_pipe) + 0x4);
}

static __inline td_void ot_ext_system_cac_purple_lower_limit_write(ot_vi_pipe vi_pipe, td_s16 data)
{
    iowr_16direct(vreg_cac_base(vi_pipe) + 0x6, data);
}

static __inline td_s16 ot_ext_system_cac_purple_lower_limit_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_cac_base(vi_pipe) + 0x6);
}

static __inline td_void ot_ext_system_cac_purple_det_range_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_cac_base(vi_pipe) + 0x8, data);
}

static __inline td_u16 ot_ext_system_cac_purple_det_range_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_cac_base(vi_pipe) + 0x8);
}

static __inline td_void ot_ext_system_cac_var_threshold_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_cac_base(vi_pipe) + 0xA, data);
}

static __inline td_u16 ot_ext_system_cac_var_threshold_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_cac_base(vi_pipe) + 0xA);
}

static __inline td_void ot_ext_system_cac_r_thd_table_write(ot_vi_pipe vi_pipe, td_u8 i, td_u16 data)
{
    iowr_16direct(vreg_cac_base(vi_pipe) + 0xC + (i * sizeof(td_u16)), data);
}

static __inline td_u16 ot_ext_system_cac_r_thd_table_read(ot_vi_pipe vi_pipe, td_u8 i)
{
    return iord_16direct(vreg_cac_base(vi_pipe) + 0xC + (i * sizeof(td_u16)));
}

static __inline td_void ot_ext_system_cac_g_thd_table_write(ot_vi_pipe vi_pipe, td_u8 i, td_u16 data)
{
    iowr_16direct(vreg_cac_base(vi_pipe) + 0x12 + (i * sizeof(td_u16)), data);
}

static __inline td_u16 ot_ext_system_cac_g_thd_table_read(ot_vi_pipe vi_pipe, td_u8 i)
{
    return iord_16direct(vreg_cac_base(vi_pipe) + 0x12 + (i * sizeof(td_u16)));
}

static __inline td_void ot_ext_system_cac_b_thd_table_write(ot_vi_pipe vi_pipe, td_u8 i, td_u16 data)
{
    iowr_16direct(vreg_cac_base(vi_pipe) + 0x18 + (i * sizeof(td_u16)), data);
}

static __inline td_u16 ot_ext_system_cac_b_thd_table_read(ot_vi_pipe vi_pipe, td_u8 i)
{
    return iord_16direct(vreg_cac_base(vi_pipe) + 0x18 + (i * sizeof(td_u16)));
}

static __inline td_void ot_ext_system_cac_edge_gain_auto_write(ot_vi_pipe vi_pipe, td_u8 i, td_u16 data)
{
    iowr_16direct(vreg_cac_base(vi_pipe) + 0x1E + (i * sizeof(td_u16)), data);
}

static __inline td_u16 ot_ext_system_cac_edge_gain_auto_read(ot_vi_pipe vi_pipe, td_u8 i)
{
    return iord_16direct(vreg_cac_base(vi_pipe) + 0x1E + (i * sizeof(td_u16)));
}

static __inline td_void ot_ext_system_cac_rb_strength_auto_write(ot_vi_pipe vi_pipe, td_u8 i, td_u16 data)
{
    iowr_16direct(vreg_cac_base(vi_pipe) + 0x3E + (i * sizeof(td_u16)), data);
}

static __inline td_u16 ot_ext_system_cac_rb_strength_auto_read(ot_vi_pipe vi_pipe, td_u8 i)
{
    return iord_16direct(vreg_cac_base(vi_pipe) + 0x3E + (i * sizeof(td_u16)));
}

static __inline td_void ot_ext_system_cac_purple_alpha_auto_write(ot_vi_pipe vi_pipe, td_u8 i, td_u16 data)
{
    iowr_16direct(vreg_cac_base(vi_pipe) + 0x5E + (i * sizeof(td_u16)), data);
}

static __inline td_u16 ot_ext_system_cac_purple_alpha_auto_read(ot_vi_pipe vi_pipe, td_u8 i)
{
    return iord_16direct(vreg_cac_base(vi_pipe) + 0x5E + (i * sizeof(td_u16)));
}

static __inline td_void ot_ext_system_cac_edge_alpha_auto_write(ot_vi_pipe vi_pipe, td_u8 i, td_u16 data)
{
    iowr_16direct(vreg_cac_base(vi_pipe) + 0x7E + (i * sizeof(td_u16)), data);
}

static __inline td_u16 ot_ext_system_cac_edge_alpha_auto_read(ot_vi_pipe vi_pipe, td_u8 i)
{
    return iord_16direct(vreg_cac_base(vi_pipe) + 0x7E + (i * sizeof(td_u16)));
}

static __inline td_void ot_ext_system_cac_satu_low_thd_auto_write(ot_vi_pipe vi_pipe, td_u8 i, td_u16 data)
{
    iowr_16direct(vreg_cac_base(vi_pipe) + 0x9E + (i * sizeof(td_u16)), data);
}

static __inline td_u16 ot_ext_system_cac_satu_low_thd_auto_read(ot_vi_pipe vi_pipe, td_u8 i)
{
    return iord_16direct(vreg_cac_base(vi_pipe) + 0x9E + (i * sizeof(td_u16)));
}

static __inline td_void ot_ext_system_cac_edge_thd_auto_write(ot_vi_pipe vi_pipe, td_u8 i, td_u16 data)
{
    iowr_16direct(vreg_cac_base(vi_pipe) + 0xBE + (i * sizeof(td_u16)), data);
}

static __inline td_u16 ot_ext_system_cac_edge_thd_auto_read(ot_vi_pipe vi_pipe, td_u8 i)
{
    return iord_16direct(vreg_cac_base(vi_pipe) + 0xBE + (i * sizeof(td_u16)));
}

static __inline td_void ot_ext_system_cac_cr_str_table_write(ot_vi_pipe vi_pipe, td_u8 i, td_u8 data)
{
    iowr_8direct(vreg_cac_base(vi_pipe) + 0xFE + (i * sizeof(td_u8)), data);
}

static __inline td_u8 ot_ext_system_cac_cr_str_table_read(ot_vi_pipe vi_pipe, td_u8 i)
{
    return iord_8direct(vreg_cac_base(vi_pipe) + 0xFE + (i * sizeof(td_u8)));
}

static __inline td_void ot_ext_system_cac_cb_str_table_write(ot_vi_pipe vi_pipe, td_u8 i, td_u8 data)
{
    iowr_8direct(vreg_cac_base(vi_pipe) + 0x10E + (i * sizeof(td_u8)), data);
}

static __inline td_u8 ot_ext_system_cac_cb_str_table_read(ot_vi_pipe vi_pipe, td_u8 i)
{
    return iord_8direct(vreg_cac_base(vi_pipe) + 0x10E + (i * sizeof(td_u8)));
}

static __inline td_void ot_ext_system_cac_edge_gain_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_cac_base(vi_pipe) + 0x11E, data);
}

static __inline td_u16 ot_ext_system_cac_edge_gain_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_cac_base(vi_pipe) + 0x11E);
}

static __inline td_void ot_ext_system_cac_rb_strength_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_cac_base(vi_pipe) + 0x120, data);
}

static __inline td_u16 ot_ext_system_cac_rb_strength_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_cac_base(vi_pipe) + 0x120);
}

static __inline td_void ot_ext_system_cac_purple_alpha_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_cac_base(vi_pipe) + 0x122, data);
}

static __inline td_u16 ot_ext_system_cac_purple_alpha_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_cac_base(vi_pipe) + 0x122);
}

static __inline td_void ot_ext_system_cac_edge_alpha_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_cac_base(vi_pipe) + 0x124, data);
}

static __inline td_u16 ot_ext_system_cac_edge_alpha_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_cac_base(vi_pipe) + 0x124);
}

static __inline td_void ot_ext_system_cac_satu_low_thd_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_cac_base(vi_pipe) + 0x126, data);
}

static __inline td_u16 ot_ext_system_cac_satu_low_thd_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_cac_base(vi_pipe) + 0x126);
}

static __inline td_void ot_ext_system_cac_manual_cr_str_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_cac_base(vi_pipe) + 0x128, data);
}

static __inline td_u8 ot_ext_system_cac_manual_cr_str_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_cac_base(vi_pipe) + 0x128);
}

static __inline td_void ot_ext_system_cac_manual_cb_str_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_cac_base(vi_pipe) + 0x129, data);
}

static __inline td_u8 ot_ext_system_cac_manual_cb_str_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_cac_base(vi_pipe) + 0x129);
}

static __inline td_void ot_ext_system_cac_edge_thd_write(ot_vi_pipe vi_pipe, td_u8 i, td_u16 data)
{
    iowr_16direct(vreg_cac_base(vi_pipe) + 0x12A + (i * sizeof(td_u16)), data);
}

static __inline td_u16 ot_ext_system_cac_edge_thd_read(ot_vi_pipe vi_pipe, td_u8 i)
{
    return iord_16direct(vreg_cac_base(vi_pipe) + 0x12A + (i * sizeof(td_u16)));
}

static __inline td_void ot_ext_system_cac_satu_high_thd_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_cac_base(vi_pipe) + 0x12E, data);
}

static __inline td_u16 ot_ext_system_cac_satu_high_thd_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_cac_base(vi_pipe) + 0x12E);
}

static __inline td_void ot_ext_system_cac_satu_high_thd_auto_write(ot_vi_pipe vi_pipe, td_u8 i, td_u16 data)
{
    iowr_16direct(vreg_cac_base(vi_pipe) + 0x130 + (i * sizeof(td_u16)), data);
}

static __inline td_u16 ot_ext_system_cac_satu_high_thd_auto_read(ot_vi_pipe vi_pipe, td_u8 i)
{
    return iord_16direct(vreg_cac_base(vi_pipe) + 0x130 + (i * sizeof(td_u16)));
}

static __inline td_void ot_ext_system_cac_coef_update_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_cac_base(vi_pipe) + 0x150, data);
}

static __inline td_u8 ot_ext_system_cac_coef_update_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_cac_base(vi_pipe) + 0x150);
}


/* bayer sharpen ext config */
static __inline td_void ot_ext_system_bshp_manual_mode_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_bshp_base(vi_pipe) + 0x0), data);
}

static __inline td_u8 ot_ext_system_bshp_manual_mode_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_bshp_base(vi_pipe) + 0x0));
}

static __inline td_void ot_ext_system_bshp_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_bshp_base(vi_pipe) + 0x01), data);
}

static __inline td_u8 ot_ext_system_bshp_enable_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_bshp_base(vi_pipe) + 0x01));
}

#define OT_EXT_SYSTEM_BSHP_ATTR_UPDATE_DEFAULT 0
static __inline td_void ot_ext_system_bshp_attr_update_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_bshp_base(vi_pipe) + 0x02), data);
}

static __inline td_u8 ot_ext_system_bshp_attr_update_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_bshp_base(vi_pipe) + 0x02));
}

/* ***********************************mf_gain 8bit 0x03-0x04******************************************* */
#define OT_EXT_SYSTEM_BSHP_MF_GAIN_DEFAULT 3
static __inline td_void ot_ext_system_bshp_mf_gain_manual_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_bshp_base(vi_pipe) + 0x03), data);
}

static __inline td_u8 ot_ext_system_bshp_mf_gain_manual_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_bshp_base(vi_pipe) + 0x03));
}

/* ***********************************hf_gain 8bit 0x04-0x05******************************************* */
#define OT_EXT_SYSTEM_BSHP_HF_GAIN_DEFAULT 2
static __inline td_void ot_ext_system_bshp_hf_gain_manual_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_bshp_base(vi_pipe) + 0x04), data);
}

static __inline td_u8 ot_ext_system_bshp_hf_gain_manual_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_bshp_base(vi_pipe) + 0x04));
}

/* ***********************************dark_tex_gain 8bit 0x05-0x06******************************************* */
#define OT_EXT_SYSTEM_BSHP_DARK_GAIN_DEFAULT 1
static __inline td_void ot_ext_system_bshp_dark_gain_manual_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_bshp_base(vi_pipe) + 0x05), data);
}

static __inline td_u8 ot_ext_system_bshp_dark_gain_manual_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_bshp_base(vi_pipe) + 0x05));
}

/* ***********************************dark_theshold_low 16bit 0x06-0x08**************************************** */
#define OT_EXT_SYSTEM_BSHP_DARK_THD_LOW_DEFAULT 180
static __inline td_void ot_ext_system_bshp_dark_thd_low_manual_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_bshp_base(vi_pipe) + 0x06), data);
}

static __inline td_u16 ot_ext_system_bshp_dark_thd_low_manual_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_bshp_base(vi_pipe) + 0x06));
}

/* ***********************************dark_theshold_high 16bit 0x08-0x0A*************************************** */
#define OT_EXT_SYSTEM_BSHP_DARK_THD_HIGH_DEFAULT 220
static __inline td_void ot_ext_system_bshp_dark_thd_high_manual_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_bshp_base(vi_pipe) + 0x08), data);
}

static __inline td_u16 ot_ext_system_bshp_dark_thd_high_manual_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_bshp_base(vi_pipe) + 0x08));
}

/* ***********************************edge_theshold_low 16bit 0x0A-0x0C***************************************** */
#define OT_EXT_SYSTEM_BSHP_TEXTURE_THD_LOW_DEFAULT 200
static __inline td_void ot_ext_system_bshp_texture_thd_low_manual_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_bshp_base(vi_pipe) + 0x0A), data);
}

static __inline td_u16 ot_ext_system_bshp_texture_thd_low_manual_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_bshp_base(vi_pipe) + 0x0A));
}

/* ***********************************edge_theshold_high 16bit 0x0C-0x0E**************************************** */
#define OT_EXT_SYSTEM_BSHP_TEXTURE_THD_HIGH_DEFAULT 300
static __inline td_void ot_ext_system_bshp_texture_thd_high_manual_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_bshp_base(vi_pipe) + 0x0C), data);
}

static __inline td_u16 ot_ext_system_bshp_texture_thd_high_manual_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_bshp_base(vi_pipe) + 0x0C));
}

/* ***********************************over_shoot_amt 16bit 0x0E-0x10******************************************* */
#define OT_EXT_SYSTEM_BSHP_OVERSHOOT_DEFAULT 128
static __inline td_void ot_ext_system_bshp_overshoot_manual_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_bshp_base(vi_pipe) + 0x0E), data);
}

static __inline td_u16 ot_ext_system_bshp_overshoot_manual_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_bshp_base(vi_pipe) + 0x0E));
}

/* ***********************************under_shoot_amt 16bit 0x10-0x12******************************************* */
#define OT_EXT_SYSTEM_BSHP_UNDERSHOOT_DEFAULT 128
static __inline td_void ot_ext_system_bshp_undershoot_manual_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_bshp_base(vi_pipe) + 0x10), data);
}

static __inline td_u16 ot_ext_system_bshp_undershoot_manual_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_bshp_base(vi_pipe) + 0x10));
}

/* ***********************************mf_strength 8 level 8x8bit 0x12-0x1A**************************************** */
static __inline td_void ot_ext_system_bshp_mf_str_manual_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bshp_base(vi_pipe)  +  0x12  +  index * sizeof(td_u8)), data);
}

static __inline td_u8 ot_ext_system_bshp_mf_str_manual_lut_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return ((iord_8direct((vreg_bshp_base(vi_pipe)  +  0x12  +  index * sizeof(td_u8)))));
}

/* ***********************************hf_strength 8 level 8x8bit 0x1A-0x22**************************************** */
static __inline td_void ot_ext_system_bshp_hf_str_manual_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bshp_base(vi_pipe)  +  0x1A  +  index * sizeof(td_u8)), data);
}

static __inline td_u8 ot_ext_system_bshp_hf_str_manual_lut_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return ((iord_8direct((vreg_bshp_base(vi_pipe)  +  0x1A  +  index * sizeof(td_u8)))));
}

/* ***********************************dark_strength 8 level 8x8bit 0x22-0x2A*************************************** */
static __inline td_void ot_ext_system_bshp_dark_str_manual_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bshp_base(vi_pipe)  +  0x22  +  index * sizeof(td_u8)), data);
}

static __inline td_u8 ot_ext_system_bshp_dark_str_manual_lut_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return ((iord_8direct((vreg_bshp_base(vi_pipe)  +  0x22  +  index * sizeof(td_u8)))));
}

/* ***************************mf_gain auto 16 level**iso 16x8bit len:0x10 0x2A-0x3A************* */
static __inline td_void ot_ext_system_bshp_mf_gain_auto_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bshp_base(vi_pipe) + 0x2A + index * sizeof(td_u8)), data);
}
static __inline td_u8 ot_ext_system_bshp_mf_gain_auto_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return ((iord_8direct((vreg_bshp_base(vi_pipe) + 0x2A + index * sizeof(td_u8)))));
}

/* ***************************hf_gain auto 16 level**iso 16x8bit len:0x10 0x3A-0x4A************* */
static __inline td_void ot_ext_system_bshp_hf_gain_auto_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bshp_base(vi_pipe) + 0x3A + index * sizeof(td_u8)), data);
}
static __inline td_u8 ot_ext_system_bshp_hf_gain_auto_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return ((iord_8direct((vreg_bshp_base(vi_pipe) + 0x3A + index * sizeof(td_u8)))));
}

/* ***************************dark_gain auto 16 level**iso 16x8bit len:0x10 0x4A-0x5A************* */
static __inline td_void ot_ext_system_bshp_dark_gain_auto_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bshp_base(vi_pipe) + 0x4A + index * sizeof(td_u8)), data);
}
static __inline td_u8 ot_ext_system_bshp_dark_gain_auto_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return ((iord_8direct((vreg_bshp_base(vi_pipe) + 0x4A + index * sizeof(td_u8)))));
}

/* ***************************overshoot auto 16 level**iso 16x16bit len:0x20 0x5A-0x7A************* */
static __inline td_void ot_ext_system_bshp_overshoot_auto_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct((vreg_bshp_base(vi_pipe) + 0x5A + index * sizeof(td_u16)), data);
}
static __inline td_u16 ot_ext_system_bshp_overshoot_auto_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return ((iord_16direct((vreg_bshp_base(vi_pipe) + 0x5A + index * sizeof(td_u16)))));
}

/* ***************************undershoot auto 16 level**iso 16x16bit len:0x20 0x7A-0x9A************* */
static __inline td_void ot_ext_system_bshp_undershoot_auto_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct((vreg_bshp_base(vi_pipe) + 0x7A + index * sizeof(td_u16)), data);
}
static __inline td_u16 ot_ext_system_bshp_undershoot_auto_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return ((iord_16direct((vreg_bshp_base(vi_pipe) + 0x7A + index * sizeof(td_u16)))));
}

/* ***************************mf_str auto 16 level***iso 16x8lutx8bit len:0x80 0x9A-0x11A************ */
static __inline td_void ot_ext_system_bshp_mf_str_auto_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bshp_base(vi_pipe) + 0x9A + index * sizeof(td_u8)), data);
}
static __inline td_u8 ot_ext_system_bshp_mf_str_auto_lut_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return ((iord_8direct((vreg_bshp_base(vi_pipe) + 0x9A + index * sizeof(td_u8)))));
}

/* ***************************hf_str auto 16 level***iso 16x8lutx8bit len:0x80 0x11A-0x19A************ */
static __inline td_void ot_ext_system_bshp_hf_str_auto_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bshp_base(vi_pipe) + 0x11A + index * sizeof(td_u8)), data);
}
static __inline td_u8 ot_ext_system_bshp_hf_str_auto_lut_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return ((iord_8direct((vreg_bshp_base(vi_pipe) + 0x11A + index * sizeof(td_u8)))));
}

/* ***************************dark_str auto 16 level***iso 16x8lutx8bit len:0x80 0x19A-0x21A************ */
static __inline td_void ot_ext_system_bshp_dark_str_auto_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_bshp_base(vi_pipe) + 0x19A + index * sizeof(td_u8)), data);
}
static __inline td_u8 ot_ext_system_bshp_dark_str_auto_lut_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return ((iord_8direct((vreg_bshp_base(vi_pipe) + 0x19A + index * sizeof(td_u8)))));
}

static __inline td_void ot_ext_system_isp_bshp_init_status_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_bshp_base(vi_pipe) + 0x21A, (data & 0x01));
}

static __inline td_u8 ot_ext_system_isp_bshp_init_status_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_bshp_base(vi_pipe) + 0x21A) & 0x01);
}


/* CA */
/* register: ot_ext_system_ca_y_ratio_lut_write */
/* LEN =256 DATASIZE td_u32 */
static __inline td_void ot_ext_system_ca_y_ratio_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u32 data)
{
    iowr_32direct(vreg_ca_base(vi_pipe)  + (index * sizeof(td_u32)), data);
}

static __inline td_u32 ot_ext_system_ca_y_ratio_lut_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return iord_32direct(vreg_ca_base(vi_pipe)  + (index * sizeof(td_u32)));
}
/* register: ot_ext_system_ca_y_sat_lut_write */
/* LEN =256 DATASIZE td_u32 */
static __inline td_void ot_ext_system_ca_y_sat_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u32 data)
{
    iowr_32direct(vreg_ca_base(vi_pipe) + 0x400 + (index * sizeof(td_u32)), data);
}

static __inline td_u32 ot_ext_system_ca_y_sat_lut_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return iord_32direct(vreg_ca_base(vi_pipe) + 0x400 + (index * sizeof(td_u32)));
}
/* register: ot_ext_system_ca_iso_ratio_lut_write */
/* LEN =16 DATASIZE td_u16  16 * 4 */
static __inline td_void ot_ext_system_ca_iso_ratio_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u32 data)
{
    iowr_32direct(vreg_ca_base(vi_pipe)  +  0x800  + (index * sizeof(td_u32)), data);
}

static __inline td_u32 ot_ext_system_ca_iso_ratio_lut_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return iord_32direct(vreg_ca_base(vi_pipe)  +  0x800  + (index * sizeof(td_u32)));
}
/* register: ot_ext_system_ca_saturation_ratio_write */
#define OT_ISP_EXT_CA_SATURATION_RATIO_DEFAULT 1000
static __inline td_void ot_ext_system_ca_saturation_ratio_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_ca_base(vi_pipe) + 0x840, data);
}

static __inline td_u16 ot_ext_system_ca_saturation_ratio_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_ca_base(vi_pipe) + 0x840);
}
/* register: ot_ext_system_ca_en_write */
static __inline td_void ot_ext_system_ca_en_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_ca_base(vi_pipe) + 0x842, data & 0x1);
}

static __inline td_u8 ot_ext_system_ca_en_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_ca_base(vi_pipe)  +  0x842) & 0x1);
}
/* register: ot_ext_system_ca_cp_en_write */
static __inline td_void ot_ext_system_ca_cp_en_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_ca_base(vi_pipe) + 0x843, data & 0x1);
}

static __inline td_u8 ot_ext_system_ca_cp_en_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_ca_base(vi_pipe) + 0x843) & 0x1);
}

/* register: ot_ext_system_ca_cp_en_write */
static __inline td_void ot_ext_system_ca_coef_update_en_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_ca_base(vi_pipe) + 0x844, data & 0x1);
}

static __inline td_u8 ot_ext_system_ca_coef_update_en_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_ca_base(vi_pipe) + 0x844) & 0x1);
}

static __inline td_void ot_ext_system_isp_ca_init_status_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_ca_base(vi_pipe) + 0x845, (data & 0x01));
}

static __inline td_u8 ot_ext_system_isp_ca_init_status_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_ca_base(vi_pipe) + 0x845) & 0x01);
}

/* LEN =256 DATASIZE td_u32  256 * 4 */
static __inline td_void ot_ext_system_ca_cp_lut_write(ot_vi_pipe vi_pipe, td_u8 index, td_u32 data)
{
    iowr_32direct(vreg_ca_base(vi_pipe) + 0x848 + (index * sizeof(td_u32)), data);
}

static __inline td_u32 ot_ext_system_ca_cp_lut_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return iord_32direct(vreg_ca_base(vi_pipe) + 0x848 + (index * sizeof(td_u32)));
}

#ifdef CONFIG_SD3403V100
/* register: ot_ext_system_ca_luma_thd_high_write */
#define OT_ISP_EXT_CA_LUMA_THD_HIGH_DEFAULT 400
static __inline td_void ot_ext_system_ca_luma_thd_high_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_ca_base(vi_pipe)  +  0xC50, data);
}

static __inline td_u16 ot_ext_system_ca_luma_thd_high_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_ca_base(vi_pipe)  +  0xC50);
}
#endif
/* crb */
static __inline td_void ot_ext_system_crb_en_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_crb_base(vi_pipe) + 0x0), data);
}

static __inline td_u16 ot_ext_system_crb_en_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_crb_base(vi_pipe) + 0x0));
}

static __inline td_void ot_ext_system_crb_step_first_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_crb_base(vi_pipe) + 0x2), data);
}

static __inline td_u16 ot_ext_system_crb_step_first_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_crb_base(vi_pipe) + 0x2));
}

static __inline td_void ot_ext_system_crb_step_second_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_crb_base(vi_pipe) + 0x4), data);
}

static __inline td_u16 ot_ext_system_crb_step_second_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_crb_base(vi_pipe) + 0x4));
}

static __inline td_void ot_ext_system_crb_step_third_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_crb_base(vi_pipe) + 0x6), data);
}

static __inline td_u16 ot_ext_system_crb_step_third_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_crb_base(vi_pipe) + 0x6));
}

static __inline td_void ot_ext_system_crb_manual_rgain_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_crb_base(vi_pipe) + 0x8), data);
}

static __inline td_u16 ot_ext_system_crb_manual_rgain_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_crb_base(vi_pipe) + 0x8));
}

static __inline td_void ot_ext_system_crb_manual_bgain_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_crb_base(vi_pipe) + 0xa), data);
}

static __inline td_u16 ot_ext_system_crb_manual_bgain_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_crb_base(vi_pipe) + 0xa));
}

static __inline td_void ot_ext_system_crb_coef_update_en_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_crb_base(vi_pipe)  +  0xe, data & 0x1);
}

static __inline td_u8 ot_ext_system_crb_coef_update_en_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_crb_base(vi_pipe)  +  0xe) & 0x1);
}
static __inline td_void ot_ext_system_isp_crb_init_status_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_crb_base(vi_pipe) + 0xf, (data & 0x01));
}

static __inline td_u8 ot_ext_system_isp_crb_init_status_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_crb_base(vi_pipe) + 0xf) & 0x01);
}

static __inline td_void ot_ext_system_crb_auto_rgain_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct((vreg_crb_base(vi_pipe) + 0x10 + index * sizeof(td_u16)), data & 0x7ff);
}
static __inline td_u16 ot_ext_system_crb_auto_rgain_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return ((iord_16direct((vreg_crb_base(vi_pipe) + 0x10  +  index * sizeof(td_u16)))) & 0x7ff);
}

static __inline td_void ot_ext_system_crb_auto_bgain_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct((vreg_crb_base(vi_pipe)  +  0x30  +  index * sizeof(td_u16)), data & 0x7ff);
}
static __inline td_u16 ot_ext_system_crb_auto_bgain_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return ((iord_16direct((vreg_crb_base(vi_pipe)  +  0x30  +  index * sizeof(td_u16)))) & 0x7ff);
}

static __inline td_void ot_ext_system_crb_type_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_16direct(vreg_crb_base(vi_pipe)  +  0x50, data & 0x1);
}

static __inline td_u8 ot_ext_system_crb_type_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_crb_base(vi_pipe) + 0x50) & 0x1);
}

/* module: csc(original CSC) */
/* register: ot_ext_system_csc_coef len = 9 */
/* args: data (16-bit) */
static __inline td_void ot_ext_system_csc_coef_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct(vreg_csc_base(vi_pipe) + 0x0 + index * sizeof(td_u16), data);
}

static __inline td_u16 ot_ext_system_csc_coef_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return iord_16direct(vreg_csc_base(vi_pipe) + 0x0 + index * sizeof(td_u16));
}

/* register: ot_ext_system_csc_dcin len = 3 */
/* args: data (10-bit) */
static __inline td_void ot_ext_system_csc_dcin_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct(vreg_csc_base(vi_pipe) + 0x20 + index * sizeof(td_u16), data);
}

static __inline td_u16 ot_ext_system_csc_dcin_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return iord_16direct(vreg_csc_base(vi_pipe) + 0x20 +  index * sizeof(td_u16));
}

/* register: ot_ext_system_csc_dcout len = 3 */
/* args: data (10-bit) */
static __inline td_void ot_ext_system_csc_dcout_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct(vreg_csc_base(vi_pipe) + 0x30 + index * sizeof(td_u16), data);
}

static __inline td_u16 ot_ext_system_csc_dcout_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return iord_16direct(vreg_csc_base(vi_pipe) + 0x30 +  index * sizeof(td_u16));
}

/* register: ot_ext_system_csc_dcout len = 3 */
/* args: data (10-bit) */
static __inline td_void ot_ext_system_csc_gamuttype_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_csc_base(vi_pipe) + 0x40, data);
}

static __inline td_u8 ot_ext_system_csc_gamuttype_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_csc_base(vi_pipe) + 0x40);
}

/* register: ot_ext_system_csc_enable */
/* args: data (1-bit) */
static __inline td_void ot_ext_system_csc_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_csc_base(vi_pipe) + 0x50), (data & 0x1));
}

static __inline td_u8 ot_ext_system_csc_enable_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_csc_base(vi_pipe) + 0x50) & 0x1);
}

/* register: ot_ext_system_csc_enable */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_csc_contrast_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_csc_base(vi_pipe) + 0x51), data);
}

static __inline td_u8 ot_ext_system_csc_contrast_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_csc_base(vi_pipe) + 0x51));
}

/* register: ot_ext_system_csc_enable */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_csc_hue_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_csc_base(vi_pipe) + 0x52), data);
}

static __inline td_u8 ot_ext_system_csc_hue_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_csc_base(vi_pipe) + 0x52));
}

/* register: ot_ext_system_csc_enable */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_csc_sat_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_csc_base(vi_pipe) + 0x53), data);
}

static __inline td_u8 ot_ext_system_csc_sat_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_csc_base(vi_pipe) + 0x53));
}

/* register: ot_ext_system_csc_enable */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_csc_luma_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_csc_base(vi_pipe) + 0x54), data);
}

static __inline td_u8 ot_ext_system_csc_luma_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_csc_base(vi_pipe) + 0x54));
}

/* register: ot_ext_system_csc_enable */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_csc_limitrange_en_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_csc_base(vi_pipe) + 0x55), data);
}

static __inline td_u8 ot_ext_system_csc_limitrange_en_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_csc_base(vi_pipe) + 0x55));
}

/* register: ot_ext_system_csc_enable */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_csc_ext_en_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_csc_base(vi_pipe) + 0x56), data);
}

static __inline td_u8 ot_ext_system_csc_ext_en_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_csc_base(vi_pipe) + 0x56));
}

/* register: ot_ext_system_csc_enable */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_csc_ctmode_en_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_csc_base(vi_pipe) + 0x57), data);
}

static __inline td_u8 ot_ext_system_csc_ctmode_en_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_csc_base(vi_pipe) + 0x57));
}

/* register: ot_ext_system_csc_update */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_csc_attr_update_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_csc_base(vi_pipe) + 0x58), data);
}

static __inline td_u8 ot_ext_system_csc_attr_update_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_csc_base(vi_pipe) + 0x58));
}
/* LDCI */
/* ldci---U8--EXT_REGISTER */
#define OT_EXT_SYSTEM_LDCI_MANU_MODE_DEFAULT 0
/* args: data (8-bit) */
static __inline td_void ot_ext_system_ldci_manu_mode_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_ldci_base(vi_pipe), data & 0x1);
}
static __inline td_u8 ot_ext_system_ldci_manu_mode_read(ot_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_ldci_base(vi_pipe))) & 0x1);
}

#define OT_EXT_SYSTEM_LDCI_MANU_HEPOSWGT_DEFAULT 64
/* args: data (8-bit) */
static __inline td_void ot_ext_system_ldci_manu_he_poswgt_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_ldci_base(vi_pipe) + 0x1, data & 0xff);
}
static __inline td_u8 ot_ext_system_ldci_manu_he_pos_wgt_read(ot_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_ldci_base(vi_pipe) + 0x1)) & 0xff);
}

#define OT_EXT_SYSTEM_LDCI_MANU_HEPOSSIGMA_DEFAULT 80
/* args: data (8-bit) */
static __inline td_void ot_ext_system_ldci_manu_he_pos_sigma_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_ldci_base(vi_pipe) + 0x2, data & 0xff);
}
static __inline td_u8 ot_ext_system_ldci_manu_he_pos_sigma_read(ot_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_ldci_base(vi_pipe) + 0x2)) & 0xff);
}

#define OT_EXT_SYSTEM_LDCI_MANU_HEPOSMEAN_DEFAULT 0
/* args: data (8-bit) */
static __inline td_void ot_ext_system_ldci_manu_he_pos_mean_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_ldci_base(vi_pipe) + 0x3, data & 0xff);
}
static __inline td_u8 ot_ext_system_ldci_manu_he_pos_mean_read(ot_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_ldci_base(vi_pipe) + 0x3)) & 0xff);
}

#define OT_EXT_SYSTEM_LDCI_MANU_HENEGWGT_DEFAULT 50
/* args: data (8-bit) */
static __inline td_void ot_ext_system_ldci_manu_he_negwgt_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_ldci_base(vi_pipe) + 0x4, data & 0xff);
}
static __inline td_u8 ot_ext_system_ldci_manu_he_neg_wgt_read(ot_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_ldci_base(vi_pipe) + 0x4)) & 0xff);
}

#define OT_EXT_SYSTEM_LDCI_MANU_HENEGSIGMA_DEFAULT 80
/* args: data (8-bit) */
static __inline td_void ot_ext_system_ldci_manu_he_negsigma_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_ldci_base(vi_pipe) + 0x5, data & 0xff);
}
static __inline td_u8 ot_ext_system_ldci_manu_he_neg_sigma_read(ot_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_ldci_base(vi_pipe) + 0x5)) & 0xff);
}

#define OT_EXT_SYSTEM_LDCI_MANU_HENEGMEAN_DEFAULT 0
/* args: data (8-bit) */
static __inline td_void ot_ext_system_ldci_manu_he_negmean_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_ldci_base(vi_pipe) + 0x6, data & 0xff);
}
static __inline td_u8 ot_ext_system_ldci_manu_he_neg_mean_read(ot_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_ldci_base(vi_pipe) + 0x6)) & 0xff);
}

#define OT_EXT_SYSTEM_LDCI_ENABLE_DEFAULT 0
/* args: data (8-bit) */
static __inline td_void ot_ext_system_ldci_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_ldci_base(vi_pipe) + 0x7, data & 0x1);
}
static __inline td_u8 ot_ext_system_ldci_enable_read(ot_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_ldci_base(vi_pipe) + 0x7)) & 0x1);
}

#define OT_EXT_SYSTEM_LDCI_GAUSSLPFSIGMA_DEFAULT 36
/* args: data (8-bit) */
static __inline td_void ot_ext_system_ldci_gauss_lpfsigma_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_ldci_base(vi_pipe) + 0x8, data & 0xff);
}
static __inline td_u8 ot_ext_system_ldci_gauss_lpfsigma_read(ot_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_ldci_base(vi_pipe) + 0x8)) & 0xff);
}

static __inline td_void ot_ext_system_isp_ldci_init_status_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_ldci_base(vi_pipe) + 0x9, (data & 0x01));
}

static __inline td_u8 ot_ext_system_isp_ldci_init_status_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_ldci_base(vi_pipe) + 0x9) & 0x01);
}

/* ldci---U16--EXT_REGISTER */
#define OT_EXT_SYSTEM_LDCI_MANU_BLCCTRL_DEFAULT 5
/* args: data (16-bit) */
static __inline td_void ot_ext_system_ldci_manu_blc_ctrl_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_ldci_base(vi_pipe) + 0xa, data & 0x1ff);
}
static __inline td_u16 ot_ext_system_ldci_manu_blc_ctrl_read(ot_vi_pipe vi_pipe)
{
    return ((iord_16direct(vreg_ldci_base(vi_pipe) + 0xa)) & 0x1ff);
}

/* ldci---ARRAY--EXT_REGISTER */
/* auoto 16 multi addrs */
static __inline td_void ot_ext_system_ldci_he_pos_wgt_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_ldci_base(vi_pipe) + 0x10 + index * sizeof(td_u8)), data & 0xff);
}
static __inline td_u8 ot_ext_system_ldci_he_pos_wgt_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return ((iord_8direct((vreg_ldci_base(vi_pipe) + 0x10 + index * sizeof(td_u8)))) & 0xff);
}

/* auoto 16 multi addrs */
static __inline td_void ot_ext_system_ldci_he_pos_sigma_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_ldci_base(vi_pipe) + 0x20 + index * sizeof(td_u8)), data & 0xff);
}
static __inline td_u8 ot_ext_system_ldci_he_pos_sigma_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return ((iord_8direct((vreg_ldci_base(vi_pipe) + 0x20 + index * sizeof(td_u8)))) & 0xff);
}

/* auoto 16 multi addrs */
static __inline td_void ot_ext_system_ldci_he_pos_mean_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_ldci_base(vi_pipe) + 0x30 + index * sizeof(td_u8)), data & 0xff);
}
static __inline td_u8 ot_ext_system_ldci_he_pos_mean_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return ((iord_8direct((vreg_ldci_base(vi_pipe) + 0x30 + index * sizeof(td_u8)))) & 0xff);
}

/* auoto 16 multi addrs */
static __inline td_void ot_ext_system_ldci_he_neg_wgt_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_ldci_base(vi_pipe) + 0x40 + index * sizeof(td_u8)), data & 0xff);
}
static __inline td_u8 ot_ext_system_ldci_he_neg_wgt_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return ((iord_8direct((vreg_ldci_base(vi_pipe) + 0x40 + index * sizeof(td_u8)))) & 0xff);
}

/* auoto 16 multi addrs */
static __inline td_void ot_ext_system_ldci_he_neg_sigma_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_ldci_base(vi_pipe) + 0x50 + index * sizeof(td_u8)), data & 0xff);
}
static __inline td_u8 ot_ext_system_ldci_he_neg_sigma_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return ((iord_8direct((vreg_ldci_base(vi_pipe) + 0x50 + index * sizeof(td_u8)))) & 0xff);
}

/* auoto 16 multi addrs */
static __inline td_void ot_ext_system_ldci_he_neg_mean_write(ot_vi_pipe vi_pipe, td_u8 index, td_u8 data)
{
    iowr_8direct((vreg_ldci_base(vi_pipe) + 0x60 + index * sizeof(td_u8)), data & 0xff);
}
static __inline td_u8 ot_ext_system_ldci_he_neg_mean_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return ((iord_8direct((vreg_ldci_base(vi_pipe) + 0x60 + index * sizeof(td_u8)))) & 0xff);
}

/* auoto 16 multi addrs */
static __inline td_void ot_ext_system_ldci_blc_ctrl_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct((vreg_ldci_base(vi_pipe) + 0x70 + index * sizeof(td_u16)), data & 0x1ff);
}
static __inline td_u16 ot_ext_system_ldci_blc_ctrl_read(ot_vi_pipe vi_pipe,  td_u8 index)
{
    return ((iord_16direct((vreg_ldci_base(vi_pipe) + 0x70 + index * sizeof(td_u16)))) & 0x1ff);
}

#define OT_EXT_SYSTEM_LDCI_TPR_INCR_COEF_DEFAULT 8

static __inline td_void ot_ext_system_ldci_tpr_incr_coef_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_ldci_base(vi_pipe) + 0x90, data & 0x1ff);
}
static __inline td_u16 ot_ext_system_ldci_tpr_incr_coef_read(ot_vi_pipe vi_pipe)
{
    return ((iord_16direct(vreg_ldci_base(vi_pipe) + 0x90)) & 0x1ff);
}

#define OT_EXT_SYSTEM_LDCI_TPR_DECR_COEF_DEFAULT 8

static __inline td_void ot_ext_system_ldci_tpr_decr_coef_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_ldci_base(vi_pipe) + 0x92, data & 0x1ff);
}
static __inline td_u16 ot_ext_system_ldci_tpr_decr_coef_read(ot_vi_pipe vi_pipe)
{
    return ((iord_16direct(vreg_ldci_base(vi_pipe) + 0x92)) & 0x1ff);
}

/* register: DNG info */
/* args: data (32-bit) */
#define OT_EXT_SYSTEM_DNG_STATIC_INFO_VALID_DEFAULT 0x0

/* args: data (8-bit) */
static __inline td_void ot_ext_system_dng_static_info_valid_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_dnginfo_base(vi_pipe), data);
}

static __inline td_u8 ot_ext_system_dng_static_info_valid_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dnginfo_base(vi_pipe));
}

#define OT_EXT_SYSTEM_DNG_HIGH_WB_GAIN_R_DEFAULT 0x400
/* args: data (16-bit) */
static __inline td_void ot_ext_system_dng_high_wb_gain_r_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_dnginfo_base(vi_pipe) + 0x2, data);
}

static __inline td_u16 ot_ext_system_dng_high_wb_gain_r_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_dnginfo_base(vi_pipe) + 0x2);
}

#define OT_EXT_SYSTEM_DNG_HIGH_WB_GAIN_G_DEFAULT 0x400
/* args: data (16-bit) */
static __inline td_void ot_ext_system_dng_high_wb_gain_g_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_dnginfo_base(vi_pipe) + 0x4, data);
}

static __inline td_u16 ot_ext_system_dng_high_wb_gain_g_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_dnginfo_base(vi_pipe) + 0x4);
}

#define OT_EXT_SYSTEM_DNG_HIGH_WB_GAIN_B_DEFAULT 0x400
/* args: data (16-bit) */
static __inline td_void ot_ext_system_dng_high_wb_gain_b_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_dnginfo_base(vi_pipe) + 0x6, data);
}

static __inline td_u16 ot_ext_system_dng_high_wb_gain_b_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_dnginfo_base(vi_pipe) + 0x6);
}

#define OT_EXT_SYSTEM_DNG_LOW_WB_GAIN_R_DEFAULT 0x400
/* args: data (16-bit) */
static __inline td_void ot_ext_system_dng_low_wb_gain_r_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_dnginfo_base(vi_pipe) + 0x8, data);
}

static __inline td_u16 ot_ext_system_dng_low_wb_gain_r_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_dnginfo_base(vi_pipe) + 0x8);
}

#define OT_EXT_SYSTEM_DNG_LOW_WB_GAIN_G_DEFAULT 0x400
/* args: data (16-bit) */
static __inline td_void ot_ext_system_dng_low_wb_gain_g_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_dnginfo_base(vi_pipe) + 0xa, data);
}

static __inline td_u16 ot_ext_system_dng_low_wb_gain_g_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_dnginfo_base(vi_pipe) + 0xa);
}

#define OT_EXT_SYSTEM_DNG_LOW_WB_GAIN_B_DEFAULT 0x400
/* args: data (16-bit) */
static __inline td_void ot_ext_system_dng_low_wb_gain_b_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_dnginfo_base(vi_pipe) + 0xc, data);
}

static __inline td_u16 ot_ext_system_dng_low_wb_gain_b_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_dnginfo_base(vi_pipe) + 0xc);
}

#define OT_EXT_SYSTEM_DNG_HIGT_TEMP_DEFAULT 0x0
/* args: data (16-bit) */
static __inline td_void ot_ext_system_dng_high_temp_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_dnginfo_base(vi_pipe) + 0xe, data);
}

static __inline td_u16 ot_ext_system_dng_high_temp_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_dnginfo_base(vi_pipe) + 0xe);
}

#define OT_EXT_SYSTEM_DNG_HIGT_CCM_DEFAULT 0x0
/* args: data (16-bit) */
static __inline td_void ot_ext_system_dng_high_ccm_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct(vreg_dnginfo_base(vi_pipe) + 0x10 + sizeof(td_u16) * index, data);
}

static __inline td_u16 ot_ext_system_dng_high_ccm_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return iord_16direct(vreg_dnginfo_base(vi_pipe) + 0x10 + sizeof(td_u16) * index);
}

#define OT_EXT_SYSTEM_DNG_LOW_TEMP_DEFAULT 0x0
/* args: data (16-bit) */
static __inline td_void ot_ext_system_dng_low_temp_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_dnginfo_base(vi_pipe) + 0x22, data);
}

static __inline td_u16 ot_ext_system_dng_low_temp_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_dnginfo_base(vi_pipe) + 0x22);
}

#define OT_EXT_SYSTEM_DNG_LOW_CCM_DEFAULT 0x0
/* args: data (16-bit) */
static __inline td_void ot_ext_system_dng_low_ccm_write(ot_vi_pipe vi_pipe, td_u8 index, td_u16 data)
{
    iowr_16direct(vreg_dnginfo_base(vi_pipe) + 0x24 + sizeof(td_u16) * index, data);
}

static __inline td_u16 ot_ext_system_dng_low_ccm_read(ot_vi_pipe vi_pipe, td_u8 index)
{
    return iord_16direct(vreg_dnginfo_base(vi_pipe) + 0x24 + sizeof(td_u16) * index);
}

/* radial crop */
/* register: ot_ext_system_rc_en_write */
static __inline td_void ot_ext_system_rc_en_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_rc_base(vi_pipe)  +  0x0, data & 0x1);
}

static __inline td_u8 ot_ext_system_rc_en_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_rc_base(vi_pipe)  +  0x0) & 0x1);
}

static __inline td_void ot_ext_system_rc_coef_update_en_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_rc_base(vi_pipe)  +  0x1, data & 0x1);
}

static __inline td_u8 ot_ext_system_rc_coef_update_en_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_rc_base(vi_pipe)  +  0x1) & 0x1);
}

/* register: ot_ext_system_rc_center_hor_coord_write */
static __inline td_void ot_ext_system_rc_center_hor_coord_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_rc_base(vi_pipe)  +  0x2, data);
}

static __inline td_u16 ot_ext_system_rc_center_hor_coord_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_rc_base(vi_pipe)  +  0x2));
}

/* register: ot_ext_system_rc_center_ver_coord_write */
static __inline td_void ot_ext_system_rc_center_ver_coord_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_rc_base(vi_pipe)  +  0x4, data);
}

static __inline td_u16 ot_ext_system_rc_center_ver_coord_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_rc_base(vi_pipe)  +  0x4));
}

/* register: ot_ext_system_rc_radius_write */
static __inline td_void ot_ext_system_rc_radius_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_rc_base(vi_pipe)  +  0x6, data);
}

static __inline td_u16 ot_ext_system_rc_radius_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_rc_base(vi_pipe)  +  0x6));
}

/* radial crop */
/* register: ot_ext_system_gamma_lut_write */
static __inline td_void ot_ext_system_gamma_en_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_gamma_base(vi_pipe), data);
}

static __inline td_u8 ot_ext_system_gamma_en_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_gamma_base(vi_pipe));
}

static __inline td_void ot_ext_system_gamma_lut_update_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_gamma_base(vi_pipe) + 0x1, data);
}

static __inline td_u8 ot_ext_system_gamma_lut_update_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_gamma_base(vi_pipe) + 0x1);
}

static __inline td_void ot_ext_system_isp_gamma_init_status_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_gamma_base(vi_pipe) + 0x2, (data & 0x01));
}

static __inline td_u8 ot_ext_system_isp_gamma_init_status_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_gamma_base(vi_pipe) + 0x2) & 0x01);
}

static __inline td_void ot_ext_system_gamma_lut_write(ot_vi_pipe vi_pipe, td_u16 index, td_u16 data)
{
    iowr_16direct(vreg_gamma_base(vi_pipe) + 0x4 + (index * sizeof(td_u16)), data);
}

static __inline td_u16 ot_ext_system_gamma_lut_read(ot_vi_pipe vi_pipe,  td_u16 index)
{
    return (iord_16direct(vreg_gamma_base(vi_pipe) + 0x4 + (index * sizeof(td_u16))));
}

/* register: ot_ext_system_pregamma_lut_write */
static __inline td_void ot_ext_system_pregamma_en_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_pregamma_base(vi_pipe), data);
}

static __inline td_u8 ot_ext_system_pregamma_en_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_pregamma_base(vi_pipe));
}

static __inline td_void ot_ext_system_pregamma_lut_update_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_pregamma_base(vi_pipe) + 0x1, data);
}

static __inline td_u8 ot_ext_system_pregamma_lut_update_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_pregamma_base(vi_pipe) + 0x1);
}

static __inline td_void ot_ext_system_isp_pregamma_init_status_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_pregamma_base(vi_pipe) + 0x2, (data & 0x01));
}

static __inline td_u8 ot_ext_system_isp_pregamma_init_status_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_pregamma_base(vi_pipe) + 0x2) & 0x01);
}

/* depth : 257 * sizeof(td_u32)  = 0x404 */
static __inline td_void ot_ext_system_pregamma_lut_write(ot_vi_pipe vi_pipe, td_u16 index, td_u32 data)
{
    iowr_32direct(vreg_pregamma_base(vi_pipe) + 0x4 + (index * sizeof(td_u32)), data);
}

static __inline td_u32 ot_ext_system_pregamma_lut_read(ot_vi_pipe vi_pipe,  td_u32 index)
{
    return (iord_32direct(vreg_pregamma_base(vi_pipe) + 0x4 + (index * sizeof(td_u32))));
}

static __inline td_void ot_ext_system_expander_en_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_expander_base(vi_pipe), data);
}

static __inline td_u8 ot_ext_system_expander_en_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_expander_base(vi_pipe));
}

static __inline td_void ot_ext_system_expander_bit_depth_in_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_expander_base(vi_pipe) + 0x1, data);
}

static __inline td_u8 ot_ext_system_expander_bit_depth_in_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_expander_base(vi_pipe) + 0x1);
}

static __inline td_void ot_ext_system_expander_bit_depth_out_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_expander_base(vi_pipe) + 0x2, data);
}

static __inline td_u8 ot_ext_system_expander_bit_depth_out_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_expander_base(vi_pipe) + 0x2);
}

static __inline td_void ot_ext_system_expander_param_update_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_expander_base(vi_pipe) + 0x3, data);
}

static __inline td_u8 ot_ext_system_expander_param_update_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_expander_base(vi_pipe) + 0x3);
}

static __inline td_void ot_ext_system_isp_expander_init_status_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_expander_base(vi_pipe) + 0x4, (data & 0x01));
}

static __inline td_u8 ot_ext_system_isp_expander_init_status_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_expander_base(vi_pipe) + 0x4) & 0x01);
}

static __inline td_void ot_ext_system_expander_blc_param_update_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_expander_base(vi_pipe) + 0x5, data);
}

static __inline td_u8 ot_ext_system_expander_blc_param_update_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_expander_base(vi_pipe) + 0x5);
}

static __inline td_void ot_ext_system_expander_knee_point_num_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_expander_base(vi_pipe) + 0x6, data);
}

static __inline td_u16 ot_ext_system_expander_knee_point_num_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_expander_base(vi_pipe) + 0x6));
}
/* depth : 256 * sizeof(td_u16) = 0x200 */
static __inline td_void ot_ext_system_expander_knee_x_coord_write(ot_vi_pipe vi_pipe, td_u16 index, td_u16 data)
{
    iowr_16direct(vreg_expander_base(vi_pipe) + 0x8 + (index * sizeof(td_u16)), data);
}

static __inline td_u16 ot_ext_system_expander_knee_x_coord_read(ot_vi_pipe vi_pipe, td_u16 index)
{
    return (iord_16direct(vreg_expander_base(vi_pipe) + 0x8 + (index * sizeof(td_u16))));
}

/* depth : 256 * sizeof(td_u32)  = 0x400 */
static __inline td_void ot_ext_system_expander_knee_y_coord_write(ot_vi_pipe vi_pipe, td_u16 index, td_u32 data)
{
    iowr_32direct(vreg_expander_base(vi_pipe) + 0x208 + (index * sizeof(td_u32)), data);
}

static __inline td_u32 ot_ext_system_expander_knee_y_coord_read(ot_vi_pipe vi_pipe,  td_u32 index)
{
    return (iord_32direct(vreg_expander_base(vi_pipe) + 0x208 + (index * sizeof(td_u32))));
}

/* CLUT */
/* register: ot_ext_system_clut_en_write */
static __inline td_void ot_ext_system_clut_en_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_clut_base(vi_pipe)  +  0x0, data & 0x1);
}

static __inline td_u8 ot_ext_system_clut_en_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_clut_base(vi_pipe)  +  0x0) & 0x1);
}

static __inline td_void ot_ext_system_clut_lut_update_en_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_clut_base(vi_pipe)  +  0x1, data & 0x1);
}
static __inline td_u8 ot_ext_system_clut_lut_update_en_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_clut_base(vi_pipe)  +  0x1) & 0x1);
}

/* register: ot_ext_system_clut_gain_r_write */
static __inline td_void ot_ext_system_clut_gain_r_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_clut_base(vi_pipe) + 0x2), data);
}

static __inline td_u16 ot_ext_system_clut_gain_r_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_clut_base(vi_pipe) + 0x2));
}

/* register: ot_ext_system_clut_gain_g_write */
static __inline td_void ot_ext_system_clut_gain_g_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_clut_base(vi_pipe) + 0x4), data);
}

static __inline td_u16 ot_ext_system_clut_gain_g_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_clut_base(vi_pipe) + 0x4));
}

/* register: ot_ext_system_clut_gain_b_write */
static __inline td_void ot_ext_system_clut_gain_b_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_clut_base(vi_pipe) + 0x6), data);
}

static __inline td_u16 ot_ext_system_clut_gain_b_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_clut_base(vi_pipe) + 0x6));
}

/* register: ot_ext_system_clut_b_lut_write */
static __inline td_void ot_ext_system_clut_ctrl_update_en_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_clut_base(vi_pipe)  +  0x8, data & 0x1);
}
static __inline td_u8 ot_ext_system_clut_ctrl_update_en_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_clut_base(vi_pipe)  +  0x8) & 0x1);
}

static __inline td_void ot_ext_system_isp_clut_init_status_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_clut_base(vi_pipe) + 0x9, (data & 0x01));
}

static __inline td_u8 ot_ext_system_isp_clut_init_status_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_clut_base(vi_pipe) + 0x9) & 0x01);
}

/* register: ot_ext_system_zone_row_awb */
/* config information of awb statistics */
#define OT_EXT_SYSTEM_AWB_SWITCH_DEFAULT 0
#define OT_EXT_SYSTEM_AWB_SWITCH_AFTER_DG 0
#define OT_EXT_SYSTEM_AWB_SWITCH_AFTER_EXPANDER 1
#define OT_EXT_SYSTEM_AWB_SWITCH_AFTER_DRC 2

/* args: data (2-bit) */
static __inline td_void ot_ext_system_awb_switch_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0x2), data);
}
static __inline td_u8 ot_ext_system_awb_switch_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0x2);
}

#define OT_EXT_SYSTEM_AWB_HNUM_DEFAULT 0x20

/* args: data (16-bit) */
static __inline td_void ot_ext_system_awb_hnum_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0x4), data);
}

static __inline td_u16 ot_ext_system_awb_hnum_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0x4);
}

#define OT_EXT_SYSTEM_AWB_VNUM_DEFAULT 0x20

/* args: data (16-bit) */
static __inline td_void ot_ext_system_awb_vnum_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0x6), data);
}

static __inline td_u16 ot_ext_system_awb_vnum_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0x6);
}

#define OT_EXT_SYSTEM_AWB_ZONE_BIN_DEFAULT 0x1

/* args: data (16-bit) */
static __inline td_void ot_ext_system_awb_zone_bin_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0x8), data);
}

static __inline td_u16 ot_ext_system_awb_zone_bin_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0x8);
}

#define OT_EXT_SYSTEM_AWB_HIST_BIN_THRESH0_DEFAULT 0xFFFF

/* args: data (16-bit) */
static __inline td_void ot_ext_system_awb_hist_bin_thresh0_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0xa), data);
}

static __inline td_u16 ot_ext_system_awb_hist_bin_thresh0_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0xa);
}

#define OT_EXT_SYSTEM_AWB_HIST_BIN_THRESH1_DEFAULT 0xFFFF

/* args: data (16-bit) */
static __inline td_void ot_ext_system_awb_hist_bin_thresh1_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0xc), data);
}

static __inline td_u16 ot_ext_system_awb_hist_bin_thresh1_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0xc);
}

#define OT_EXT_SYSTEM_AWB_HIST_BIN_THRESH2_DEFAULT 0xFFFF

/* args: data (16-bit) */
static __inline td_void ot_ext_system_awb_hist_bin_thresh2_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0xe), data);
}

static __inline td_u16 ot_ext_system_awb_hist_bin_thresh2_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0xe);
}

#define OT_EXT_SYSTEM_AWB_HIST_BIN_THRESH3_DEFAULT 0xFFFF

/* args: data (16-bit) */
static __inline td_void ot_ext_system_awb_hist_bin_thresh3_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0x10), data);
}

static __inline td_u16 ot_ext_system_awb_hist_bin_thresh3_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0x10);
}

#define OT_EXT_SYSTEM_AWB_WHITE_LEVEL_DEFAULT 0xffff

/* args: data (16-bit) */
static __inline td_void ot_ext_system_awb_white_level_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0x12), data);
}

static __inline td_u16 ot_ext_system_awb_white_level_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0x12);
}

#define OT_EXT_SYSTEM_AWB_BLACK_LEVEL_DEFAULT 0x0

/* args: data (16-bit) */
static __inline td_void ot_ext_system_awb_black_level_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0x14), data);
}

static __inline td_u16 ot_ext_system_awb_black_level_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0x14);
}

#define OT_EXT_SYSTEM_AWB_CR_REF_MAX_DEFAULT 0x180

/* args: data (16-bit) */
static __inline td_void ot_ext_system_awb_cr_ref_max_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0x16), data);
}

static __inline td_u16 ot_ext_system_awb_cr_ref_max_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0x16);
}

#define OT_EXT_SYSTEM_AWB_CR_REF_MIN_DEFAULT 0x40

/* args: data (16-bit) */
static __inline td_void ot_ext_system_awb_cr_ref_min_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0x18), data);
}

static __inline td_u16 ot_ext_system_awb_cr_ref_min_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0x18);
}

#define OT_EXT_SYSTEM_AWB_CB_REF_MAX_DEFAULT 0x180

/* args: data (16-bit) */
static __inline td_void ot_ext_system_awb_cb_ref_max_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0x1a), data);
}

static __inline td_u16 ot_ext_system_awb_cb_ref_max_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0x1a);
}

#define OT_EXT_SYSTEM_AWB_CB_REF_MIN_DEFAULT 0x40

/* args: data (16-bit) */
static __inline td_void ot_ext_system_awb_cb_ref_min_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0x1c), data);
}

static __inline td_u16 ot_ext_system_awb_cb_ref_min_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0x1c);
}

#define OT_EXT_SYSTEM_AWB_GAIN_ENABLE_DEFAULT 0x1

/* args: data (8-bit) */
static __inline td_void ot_ext_system_awb_gain_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_awb_base(vi_pipe) + 0x1e), (data & 0x1));
}

static __inline td_u8 ot_ext_system_awb_gain_enable_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_awb_base(vi_pipe) + 0x1e) & 0x1);
}

/* args: data (16-bit) */
static __inline td_void ot_ext_system_cc_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_awb_base(vi_pipe) + 0x20), (data & 0x1));
}

static __inline td_u8 ot_ext_system_cc_enable_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_awb_base(vi_pipe) + 0x20) & 0x1);
}

/* register: ot_ext_system_cc_colortone_rgain */
/* cc colortone */
#define OT_EXT_SYSTEM_CCM_COLORTONE_RGAIN_DEFAULT 256

/* args: data (16-bit) */
static __inline td_void ot_ext_system_cc_colortone_rgain_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0x22), data);
}

static __inline td_u16 ot_ext_system_cc_colortone_rgain_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0x22);
}

#define OT_EXT_SYSTEM_CCM_COLORTONE_GGAIN_DEFAULT 256

/* args: data (16-bit) */
static __inline td_void ot_ext_system_cc_colortone_ggain_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0x24), data);
}

static __inline td_u16 ot_ext_system_cc_colortone_ggain_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0x24);
}

#define OT_EXT_SYSTEM_CCM_COLORTONE_BGAIN_DEFAULT 256

/* args: data (16-bit) */
static __inline td_void ot_ext_system_cc_colortone_bgain_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0x26), data);
}

static __inline td_u16 ot_ext_system_cc_colortone_bgain_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0x26);
}

#define OT_EXT_SYSTEM_WB_STATISTICS_MPI_UPDATE_EN_DEFAULT 0

/* args: data (8-bit) */
static __inline td_void ot_ext_system_wb_statistics_mpi_update_en_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_awb_base(vi_pipe) + 0x28), data);
}

static __inline td_u8 ot_ext_system_wb_statistics_mpi_update_en_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_awb_base(vi_pipe) + 0x28);
}

/* args: data (16-bit) */
static __inline td_void ot_ext_system_awb_sta_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_awb_base(vi_pipe) + 0x2a), (data & 0x1));
}

static __inline td_u8 ot_ext_system_awb_sta_enable_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_awb_base(vi_pipe) + 0x2a) & 0x1);
}

/* register: ot_ext_system_crop_en */
/* CROP enable */
#define OT_EXT_SYSTEM_AWB_CROP_EN_DEFAULT 0x0

/* args: data (8-bit) */
static __inline td_void ot_ext_system_awb_crop_en_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct((vreg_awb_base(vi_pipe) + 0x2c), data);
}
static __inline td_u8 ot_ext_system_awb_crop_en_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_awb_base(vi_pipe) + 0x2c);
}

/* register: ot_ext_system_awb_crop_x */
/* AE CROP X */
#define OT_EXT_SYSTEM_AWB_CROP_X_DEFAULT 0x0

/* args: data (16-bit) */
static __inline td_void ot_ext_system_awb_crop_x_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0x2e), data);
}
static __inline td_u16 ot_ext_system_awb_crop_x_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0x2e);
}

/* register: ot_ext_system_awb_crop_y */
/* AWB CROP Y */
#define OT_EXT_SYSTEM_AWB_CROP_Y_DEFAULT 0x0

/* args: data (16-bit) */
static __inline td_void ot_ext_system_awb_crop_y_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0x30), data);
}
static __inline td_u16 ot_ext_system_awb_crop_y_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0x30);
}

/* register: ot_ext_system_awb_crop_height */
/* AWB CROP HEIGHT */
#define OT_EXT_SYSTEM_AWB_CROP_HEIGHT_DEFAULT 0x0

/* args: data (16-bit) */
static __inline td_void ot_ext_system_awb_crop_height_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0x32), data);
}
static __inline td_u16 ot_ext_system_awb_crop_height_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0x32);
}

/* register: ot_ext_system_awb_crop_width */
/* AWB CROP WIDTH */
#define OT_EXT_SYSTEM_AWB_CROP_WIDTH_DEFAULT 0x0

/* args: data (16-bit) */
static __inline td_void ot_ext_system_awb_crop_width_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0x34), data);
}
static __inline td_u16 ot_ext_system_awb_crop_width_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0x34);
}

/* args: data (8-bit) */
static __inline td_void ot_ext_system_rgbir_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_rgbir_base(vi_pipe), (data & 0x1));
}
static __inline td_u8 ot_ext_system_rgbir_enable_read(ot_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_rgbir_base(vi_pipe))) & 0x1);
}

/* register: ot_ext_system_rgbir_th_write */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_rgbir_th_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_rgbir_base(vi_pipe) + 0x1, (data & 0xff));
}
static __inline td_u8 ot_ext_system_rgbir_th_read(ot_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_rgbir_base(vi_pipe) + 0x1)) & 0xff);
}

/* register: ot_ext_system_rgbir_tv_write */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_rgbir_tv_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_rgbir_base(vi_pipe) + 0x2, (data & 0xff));
}
static __inline td_u8 ot_ext_system_rgbir_tv_read(ot_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_rgbir_base(vi_pipe) + 0x2)) & 0xff);
}

/* register: ot_ext_system_rgbir_outpattern_write */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_rgbir_outpattern_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_rgbir_base(vi_pipe) + 0x3, (data & 0x3));
}
static __inline td_u8 ot_ext_system_rgbir_outpattern_read(ot_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_rgbir_base(vi_pipe) + 0x3)) & 0x3);
}

/* register: ot_ext_system_rgbir_outpattern_write */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_rgbir_inpattern_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_rgbir_base(vi_pipe) + 0x4, (data & 0x7));
}
static __inline td_u8 ot_ext_system_rgbir_inpattern_read(ot_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_rgbir_base(vi_pipe) + 0x4)) & 0x7);
}

static __inline td_void ot_ext_system_isp_rgbir_init_status_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_rgbir_base(vi_pipe) + 0x5, (data & 0x01));
}

static __inline td_u8 ot_ext_system_isp_rgbir_init_status_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_rgbir_base(vi_pipe) + 0x5) & 0x01);
}

/* register: ot_ext_system_rgbir_expctrl_write */
/* args: data (16-bit) */
static __inline td_void ot_ext_system_rgbir_expctrl_write(ot_vi_pipe vi_pipe, td_u16 data, td_u16 i)
{
    iowr_16direct(vreg_rgbir_base(vi_pipe) + 0x6 + i * sizeof(td_u16), (data & 0xffff));
}
static __inline td_u16 ot_ext_system_rgbir_expctrl_read(ot_vi_pipe vi_pipe, td_u16 i)
{
    return ((iord_16direct(vreg_rgbir_base(vi_pipe) + 0x6 + i * sizeof(td_u16))) & 0xffff);
}

/* register: ot_ext_system_rgbir_expctrl_write */
/* args: data (16-bit) */
static __inline td_void ot_ext_system_rgbir_gain_write(ot_vi_pipe vi_pipe, td_u16 data, td_u16 i)
{
    iowr_16direct(vreg_rgbir_base(vi_pipe) + 0xa + i * sizeof(td_u16), (data & 0xffff));
}
static __inline td_u16 ot_ext_system_rgbir_gain_read(ot_vi_pipe vi_pipe, td_u16 i)
{
    return ((iord_16direct(vreg_rgbir_base(vi_pipe) + 0xa + i * sizeof(td_u16))) & 0xffff);
}

/* register: ot_ext_system_rgbir_outpattern_write */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_rgbir_irstatus_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_rgbir_base(vi_pipe) + 0xe, (data & 0x3));
}
static __inline td_u8 ot_ext_system_rgbir_irstatus_read(ot_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_rgbir_base(vi_pipe) + 0xe)) & 0x3);
}

/* register: ot_ext_system_rgbir_cvtmatrix_write */
/* args: data (16-bit, array) */
#define ISP_RGBIR_CVTMATRIX_NUM 12
static __inline td_void ot_ext_system_rgbir_cvtmatrix_write(ot_vi_pipe vi_pipe, td_u16 data, td_u16 i)
{
    iowr_16direct(vreg_rgbir_base(vi_pipe) + 0x10 + i * sizeof(td_u16), (data & 0xffff));
}
static __inline td_u16 ot_ext_system_rgbir_cvtmatrix_read(ot_vi_pipe vi_pipe, td_u16 i)
{
    return ((iord_16direct(vreg_rgbir_base(vi_pipe) + 0x10 + i * sizeof(td_u16))) & 0xffff);
}

/* register: ot_ext_system_rgbir_attr_update_write */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_rgbir_attr_update_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_rgbir_base(vi_pipe) + 0x28, (data & 0x1));
}
static __inline td_u8 ot_ext_system_rgbir_attr_update_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_rgbir_base(vi_pipe) + 0x28) & 0x1);
}

/* register: ot_ext_system_rgbir_smooth_enable_write */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_rgbir_smooth_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_rgbir_base(vi_pipe) + 0x40, (data & 0x1));
}
static __inline td_u8 ot_ext_system_rgbir_smooth_enable_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_rgbir_base(vi_pipe) + 0x40) & 0x1);
}

/* register: ot_ext_system_rgbir_irremove_enable_write */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_rgbir_irremove_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_rgbir_base(vi_pipe) + 0x42, (data & 0x1));
}
static __inline td_u8 ot_ext_system_rgbir_irremove_enable_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_rgbir_base(vi_pipe) + 0x42) & 0x1);
}

/* register: ot_ext_system_rgbir_irremove_ratio_write */
/* args: data * 3 (16-bit, array) */
static __inline td_void ot_ext_system_rgbir_irremove_ratio_write(ot_vi_pipe vi_pipe, td_u16 data, td_u16 i)
{
    iowr_16direct(vreg_rgbir_base(vi_pipe) + 0x44 + i * sizeof(td_u16), (data & 0xffff));
}
static __inline td_u16 ot_ext_system_rgbir_irremove_ratio_read(ot_vi_pipe vi_pipe, td_u16 i)
{
    return ((iord_16direct(vreg_rgbir_base(vi_pipe) + 0x44 + i * sizeof(td_u16))) & 0xffff);
}

/* register: ot_ext_system_rgbir_wb_ctrl_write */
/* args: data * 2 (16-bit, array) */
static __inline td_void ot_ext_system_rgbir_wb_ctrl_write(ot_vi_pipe vi_pipe, td_u16 data, td_u16 i)
{
    iowr_16direct(vreg_rgbir_base(vi_pipe) + 0x50 + i * sizeof(td_u16), (data & 0xffff));
}
static __inline td_u16 ot_ext_system_rgbir_wb_ctrl_read(ot_vi_pipe vi_pipe, td_u16 i)
{
    return ((iord_16direct(vreg_rgbir_base(vi_pipe) + 0x50 + i * sizeof(td_u16))) & 0xffff);
}

/* register: ot_ext_system_rgbir_autogain_enable_write */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_rgbir_autogain_enable_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_rgbir_base(vi_pipe) + 0x54, (data & 0x1));
}
static __inline td_u8 ot_ext_system_rgbir_autogain_enable_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_rgbir_base(vi_pipe) + 0x54) & 0x1);
}

/* register: ot_ext_system_rgbir_auto_gain_write */
/* args: data (16-bit) */
static __inline td_void ot_ext_system_rgbir_auto_gain_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_rgbir_base(vi_pipe) + 0x56, (data & 0xffff));
}
static __inline td_u16 ot_ext_system_rgbir_auto_gain_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_rgbir_base(vi_pipe) + 0x56) & 0xffff);
}

/* register: ot_ext_system_rgbir_ir_sum_write */
/* args: data (32-bit) */
static __inline td_void ot_ext_system_rgbir_ir_sum_write(ot_vi_pipe vi_pipe, td_u32 data)
{
    iowr_32direct(vreg_rgbir_base(vi_pipe) + 0x58, (data & 0xffffffff));
}
static __inline td_u32 ot_ext_system_rgbir_ir_sum_read(ot_vi_pipe vi_pipe)
{
    return (iord_32direct(vreg_rgbir_base(vi_pipe) + 0x58) & 0xffffffff);
}

/* register: ot_ext_system_rgbir_is_irupscale_write */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_rgbir_is_irupscale_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_rgbir_base(vi_pipe) + 0x5e, (data & 0x1));
}
static __inline td_u8 ot_ext_system_rgbir_is_irupscale_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_rgbir_base(vi_pipe) + 0x5e) & 0x1);
}

/* register: ot_ext_system_rgbir_in_bayer_write */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_rgbir_in_bayer_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_rgbir_base(vi_pipe) + 0x60, (data & 0x3));
}
static __inline td_u8 ot_ext_system_rgbir_in_bayer_read(ot_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_rgbir_base(vi_pipe) + 0x60)) & 0x3);
}

/* register: ot_ext_system_rgbir_irremove_enable_write */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_rgbir_mode_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_rgbir_base(vi_pipe) + 0x62, (data & 0x1));
}
static __inline td_u8 ot_ext_system_rgbir_mode_read(ot_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_rgbir_base(vi_pipe) + 0x62)) & 0x1);
}

/* LBLC */
/* args: data (8-bit) */
static __inline td_void ot_ext_system_isp_lblc_enable_write(ot_vi_pipe vi_pipe,  td_u8 data)
{
    iowr_8direct(vreg_lblc_base(vi_pipe), data & 0x1);
}
static __inline td_u8 ot_ext_system_isp_lblc_enable_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_lblc_base(vi_pipe)) & 0x1);
}

/* args: data (8-bit) */
static __inline td_void ot_ext_system_isp_lblc_lut_attr_updata_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_lblc_base(vi_pipe) + 0x1, data);
}
static __inline td_u8 ot_ext_system_isp_lblc_lut_attr_updata_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_lblc_base(vi_pipe) + 0x1);
}

/* args: data (8-bit) */
static __inline td_void ot_ext_system_isp_lblc_attr_updata_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_lblc_base(vi_pipe) + 0x2, data);
}
static __inline td_u8 ot_ext_system_isp_lblc_attr_updata_read(ot_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_lblc_base(vi_pipe) + 0x2);
}

/* args: data (8-bit) */
static __inline td_void ot_ext_system_isp_lblc_init_status_write(ot_vi_pipe vi_pipe, td_u8 data)
{
    iowr_8direct(vreg_lblc_base(vi_pipe) + 0x3, (data & 0x01));
}
static __inline td_u8 ot_ext_system_isp_lblc_init_status_read(ot_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_lblc_base(vi_pipe) + 0x3) & 0x01);
}

/* args: data (16-bit) */
static __inline td_void ot_ext_system_isp_lblc_strength_write(ot_vi_pipe vi_pipe,  td_u16 data)
{
    iowr_16direct(vreg_lblc_base(vi_pipe) + 0x4, data);
}
static __inline td_u16 ot_ext_system_isp_lblc_strength_read(ot_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_lblc_base(vi_pipe) + 0x4));
}

/* bytes: 2 */
static __inline td_void ot_ext_system_isp_lblc_offset_r_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_lblc_base(vi_pipe) + 0x10, data);
}
static __inline td_u16 ot_ext_system_isp_lblc_offset_r_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_lblc_base(vi_pipe) + 0x10);
}

static __inline td_void ot_ext_system_isp_lblc_offset_gr_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_lblc_base(vi_pipe) + 0x12, data);
}
static __inline td_u16 ot_ext_system_isp_lblc_offset_gr_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_lblc_base(vi_pipe) + 0x12);
}

static __inline td_void ot_ext_system_isp_lblc_offset_gb_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_lblc_base(vi_pipe) + 0x14, data);
}
static __inline td_u16 ot_ext_system_isp_lblc_offset_gb_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_lblc_base(vi_pipe) + 0x14);
}

static __inline td_void ot_ext_system_isp_lblc_offset_b_write(ot_vi_pipe vi_pipe, td_u16 data)
{
    iowr_16direct(vreg_lblc_base(vi_pipe) + 0x16, data);
}
static __inline td_u16 ot_ext_system_isp_lblc_offset_b_read(ot_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_lblc_base(vi_pipe) + 0x16);
}

/* length: 1089; bytes: 1089*2 = 0x882 */
static __inline td_void ot_ext_system_isp_lblc_mesh_blc_r_write(ot_vi_pipe vi_pipe, td_u16 index, td_u16 data)
{
    iowr_16direct(vreg_lblc_base(vi_pipe) + 0x18 + (index * sizeof(td_u16)), data);
}
static __inline td_u16 ot_ext_system_isp_lblc_mesh_blc_r_read(ot_vi_pipe vi_pipe, td_u16 index)
{
    return iord_16direct(vreg_lblc_base(vi_pipe) + 0x18 + (index * sizeof(td_u16)));
}

static __inline td_void ot_ext_system_isp_lblc_mesh_blc_gr_write(ot_vi_pipe vi_pipe, td_u16 index, td_u16 data)
{
    iowr_16direct(vreg_lblc_base(vi_pipe) + 0x89A + (index * sizeof(td_u16)), data);
}
static __inline td_u16 ot_ext_system_isp_lblc_mesh_blc_gr_read(ot_vi_pipe vi_pipe, td_u16 index)
{
    return iord_16direct(vreg_lblc_base(vi_pipe) + 0x89A + (index * sizeof(td_u16)));
}

static __inline td_void ot_ext_system_isp_lblc_mesh_blc_gb_write(ot_vi_pipe vi_pipe, td_u16 index, td_u16 data)
{
    iowr_16direct(vreg_lblc_base(vi_pipe) + 0x111C + (index * sizeof(td_u16)), data);
}
static __inline td_u16 ot_ext_system_isp_lblc_mesh_blc_gb_read(ot_vi_pipe vi_pipe, td_u16 index)
{
    return iord_16direct(vreg_lblc_base(vi_pipe) + 0x111C + (index * sizeof(td_u16)));
}

static __inline td_void ot_ext_system_isp_lblc_mesh_blc_b_write(ot_vi_pipe vi_pipe, td_u16 index, td_u16 data)
{
    iowr_16direct(vreg_lblc_base(vi_pipe) + 0x199E + (index * sizeof(td_u16)), data);
}
static __inline td_u16 ot_ext_system_isp_lblc_mesh_blc_b_read(ot_vi_pipe vi_pipe, td_u16 index)
{
    return iord_16direct(vreg_lblc_base(vi_pipe) + 0x199E + (index * sizeof(td_u16)));
}

static __inline td_void ot_ext_system_sns_noise_calibration_lut_write(ot_vi_pipe vi_pipe, td_u32 index, td_double data)
{
    iowr_double_direct((vreg_sns_base(vi_pipe) + sizeof(td_double) * index), data);
}

static __inline td_double ot_ext_system_sns_noise_calibration_lut_read(ot_vi_pipe vi_pipe, td_u32 index)
{
    return (iord_double_direct((vreg_sns_base(vi_pipe) + sizeof(td_double) * index)));
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* ISP_EXT_CONFIG_H */
