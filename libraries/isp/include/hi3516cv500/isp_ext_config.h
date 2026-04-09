/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_ext_config.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __ISP_EXT_CONFIG_H__
#define __ISP_EXT_CONFIG_H__

#include "isp_vreg.h"
#include "hi_common_adapt.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/* instance 'ext' of module 'ext_config' */
#define hi_ext_base_addr(vi_pipe) (isp_vir_reg_base(vi_pipe))

#ifdef CONFIG_HI_ISP_DPC_STATIC_TABLE_SUPPORT
#define HI_EXT_SIZE         0x20000
#else
#define HI_EXT_SIZE         0x10000
#endif
/* TOP: 0x10000~0x10FFF */
#define VREG_TOP_OFFSET     0x0
#define VREG_TOP_SIZE       0x1000
#define vreg_top_base(vi_pipe)  (hi_ext_base_addr(vi_pipe) + VREG_TOP_OFFSET)
/* SIZE < 0x100 */
/* SIZE < 0x100 */
/* BLC:   0x1000~0x100F */
#define VREG_BLC_OFFSET             (VREG_TOP_OFFSET + VREG_TOP_SIZE)
#define VREG_BLC_SIZE               0x10
#define vreg_blc_base(vi_pipe)      (hi_ext_base_addr(vi_pipe) + VREG_BLC_OFFSET)

/* FeLogLUT:0x1010~0x101f */
#define VREG_FELOGLUT_OFFSET        (VREG_BLC_OFFSET + VREG_BLC_SIZE)
#define VREG_FELOGLUT_SIZE          0x10
#define vreg_feloglut_base(vi_pipe) (hi_ext_base_addr(vi_pipe) + VREG_FELOGLUT_OFFSET)

/* BeLogLUT:0x1020~0x102f */
#define VREG_LOGLUT_OFFSET          (VREG_FELOGLUT_OFFSET + VREG_FELOGLUT_SIZE)
#define VREG_LOGLUT_SIZE            0x10
#define vreg_loglut_base(vi_pipe)   (hi_ext_base_addr(vi_pipe) + VREG_LOGLUT_OFFSET)

/* EdgeMark:0x1030~0x103f */
#define VREG_EDGEMARK_OFFSET        (VREG_LOGLUT_OFFSET + VREG_LOGLUT_SIZE)
#define VREG_EDGEMARK_SIZE          0x10
#define vreg_edgemark_base(vi_pipe) (hi_ext_base_addr(vi_pipe) + VREG_EDGEMARK_OFFSET)

/* Hlc:0x1040~0x104f */
#define VREG_HLC_OFFSET             (VREG_EDGEMARK_OFFSET + VREG_EDGEMARK_SIZE)
#define VREG_HLC_SIZE               0x10
#define vreg_hlc_base(vi_pipe)      (hi_ext_base_addr(vi_pipe) + VREG_HLC_OFFSET)

/* RC:   0x1050~0x105F */
#define VREG_RC_OFFSET              (VREG_HLC_OFFSET + VREG_HLC_SIZE)
#define VREG_RC_SIZE                0x10
#define vreg_rc_base(vi_pipe)       (hi_ext_base_addr(vi_pipe) + VREG_RC_OFFSET)

/* CLUT:0x1060~0x107F */
#define VREG_CLUT_OFFSET            (VREG_RC_OFFSET + VREG_RC_SIZE)
#define VREG_CLUT_SIZE              0x20
#define vreg_clut_base(vi_pipe)     (hi_ext_base_addr(vi_pipe) + VREG_CLUT_OFFSET)

/* FPN:   0x1080~0x109F */
#define VREG_FPN_OFFSET             (VREG_CLUT_OFFSET + VREG_CLUT_SIZE)
#define VREG_FPN_SIZE               0x20
#define vreg_fpn_base(vi_pipe)      (hi_ext_base_addr(vi_pipe) + VREG_FPN_OFFSET)

/* GCAC: 0x10A0~0x10BF */
#define VREG_GCAC_OFFSET            (VREG_FPN_OFFSET + VREG_FPN_SIZE)
#define VREG_GCAC_SIZE              0x20
#define vreg_gcac_base(vi_pipe)     (hi_ext_base_addr(vi_pipe) + VREG_GCAC_OFFSET)

/* FLICKER:  0x10C0~0x10DF */
#define VREG_FLICK_OFFSET           (VREG_GCAC_OFFSET + VREG_GCAC_SIZE)
#define VREG_FLICK_SIZE             0x20
#define vreg_flick_base(vi_pipe)    (hi_ext_base_addr(vi_pipe) + VREG_FLICK_OFFSET)

/* LCAC: 0x10E0~0x113F */
#define VREG_LCAC_OFFSET            (VREG_FLICK_OFFSET + VREG_FLICK_SIZE)
#define VREG_LCAC_SIZE              0x60
#define vreg_lcac_base(vi_pipe)     (hi_ext_base_addr(vi_pipe) + VREG_LCAC_OFFSET)

/* FCR: 0x1140~0x118F */
#define VREG_FCR_OFFSET             (VREG_LCAC_OFFSET + VREG_LCAC_SIZE)
#define VREG_FCR_SIZE               0x50
#define vreg_fcr_base(vi_pipe)      (hi_ext_base_addr(vi_pipe) + VREG_FCR_OFFSET)

/* DngInfo : 0x1190 ~ 0x11DF */
#define  VREG_DNGINFO_OFFSET        (VREG_FCR_OFFSET + VREG_FCR_SIZE)
#define  VREG_DNGINFO_SIZE          0x50
#define  vreg_dnginfo_base(vi_pipe) (hi_ext_base_addr(vi_pipe) + VREG_DNGINFO_OFFSET)

/* GE:     0x11E0~0x125F */
#define VREG_GE_OFFSET              (VREG_DNGINFO_OFFSET + VREG_DNGINFO_SIZE)
#define VREG_GE_SIZE                0x80
#define vreg_ge_base(vi_pipe)       (hi_ext_base_addr(vi_pipe) + VREG_GE_OFFSET)

/* csc: 0x1260~0x12FF */
#define VREG_CSC_OFFSET             (VREG_GE_OFFSET + VREG_GE_SIZE)
#define VREG_CSC_SIZE               0xA0 /* 0x50 reserved */
#define vreg_csc_base(vi_pipe)      (hi_ext_base_addr(vi_pipe) + VREG_CSC_OFFSET)

/* SIZE <:= 0x500 */
/* WDR: 0x1300~0x13FF */
#define VREG_WDR_OFFSET         (VREG_CSC_OFFSET + VREG_CSC_SIZE)
#define VREG_WDR_SIZE           0x100
#define vreg_wdr_base(vi_pipe)  (hi_ext_base_addr(vi_pipe) + VREG_WDR_OFFSET)

/* LDCI: 0x1400~0x14FF */
#define VREG_LDCI_OFFSET        (VREG_WDR_OFFSET + VREG_WDR_SIZE)
#define VREG_LDCI_SIZE          0x100
#define vreg_ldci_base(vi_pipe) (hi_ext_base_addr(vi_pipe) + VREG_LDCI_OFFSET)

/* DM: 0x1500~0x15FF */
#define VREG_DM_OFFSET          (VREG_LDCI_OFFSET + VREG_LDCI_SIZE)
#define VREG_DM_SIZE            0x100
#define vreg_dm_base(vi_pipe)   (hi_ext_base_addr(vi_pipe) + VREG_DM_OFFSET)

/* AWB:0x1600~0x16FF */
#define VREG_AWB_OFFSET         (VREG_DM_OFFSET + VREG_DM_SIZE)
#define VREG_AWB_SIZE           0x100
#define vreg_awb_base(vi_pipe)  (hi_ext_base_addr(vi_pipe) + VREG_AWB_OFFSET)

/* AF:0x1700~0x17FF */
#define VREG_AF_OFFSET          (VREG_AWB_OFFSET + VREG_AWB_SIZE)
#define VREG_AF_SIZE            0x100
#define vreg_af_base(vi_pipe)   (hi_ext_base_addr(vi_pipe) + VREG_AF_OFFSET)

/* AE:0x1800~0x19FF */
#define VREG_AE_OFFSET          (VREG_AF_OFFSET + VREG_AF_SIZE)
#define VREG_AE_SIZE            0x200
#define vreg_ae_base(vi_pipe)   (hi_ext_base_addr(vi_pipe) + VREG_AE_OFFSET)
/* DEHAZE:  0x1A00~0x1BFF */
#define VREG_DEHAZE_OFFSET          (VREG_AE_OFFSET + VREG_AE_SIZE)
#define VREG_DEHAZE_SIZE            0x200
#define vreg_dehaze_base(vi_pipe)   (hi_ext_base_addr(vi_pipe) + VREG_DEHAZE_OFFSET)

/* DRC: 0x1C00~0x1EFF */
#define VREG_DRC_OFFSET             (VREG_DEHAZE_OFFSET + VREG_DEHAZE_SIZE)
#define VREG_DRC_SIZE               0x300
#define vreg_drc_base(vi_pipe)      (hi_ext_base_addr(vi_pipe) + VREG_DRC_OFFSET)

/* BNR: 0x1F00~0x21FF */
#define VREG_BNR_OFFSET             (VREG_DRC_OFFSET + VREG_DRC_SIZE)
#define VREG_BNR_SIZE               0x300
#define vreg_bnr_base(vi_pipe)      (hi_ext_base_addr(vi_pipe) + VREG_BNR_OFFSET)

/* DE:0x2200~0x25FF */
#define VREG_DE_OFFSET              (VREG_BNR_OFFSET + VREG_BNR_SIZE)
#define VREG_DE_SIZE                0x400
#define vreg_de_base(vi_pipe)       (hi_ext_base_addr(vi_pipe) + VREG_DE_OFFSET)

/* PreGamma:0x2600~0x2AFF */
#define VREG_PREGAMMA_OFFSET        (VREG_DE_OFFSET + VREG_DE_SIZE)
#define VREG_PREGAMMA_SIZE          0x500
#define vreg_pregamma_base(vi_pipe) (hi_ext_base_addr(vi_pipe) + VREG_PREGAMMA_OFFSET)

/* SIZE < 0x1000 */
/* CA:  0x2B00~0x33FF */
#define VREG_CA_OFFSET              (VREG_PREGAMMA_OFFSET + VREG_PREGAMMA_SIZE)
#define VREG_CA_SIZE                0x900
#define vreg_ca_base(vi_pipe)       (hi_ext_base_addr(vi_pipe) + VREG_CA_OFFSET)

/* Gamma:0x3400~0x3CFF */
#define VREG_GAMMA_OFFSET           (VREG_CA_OFFSET + VREG_CA_SIZE)
#define VREG_GAMMA_SIZE             0x900
#define vreg_gamma_base(vi_pipe)    (hi_ext_base_addr(vi_pipe) + VREG_GAMMA_OFFSET)

/* SHARPEN: 0x3D00~0x49FF */
#define VREG_SHARPEN_OFFSET         (VREG_GAMMA_OFFSET + VREG_GAMMA_SIZE)
#define VREG_SHARPEN_SIZE           0xD00
#define vreg_sharpen_base(vi_pipe)  (hi_ext_base_addr(vi_pipe) + VREG_SHARPEN_OFFSET)

/* RLSC:0x4A00~0x57FF */
#define VREG_RLSC_OFFSET            (VREG_SHARPEN_OFFSET + VREG_SHARPEN_SIZE)
#define VREG_RLSC_SIZE              0xe00
#define vreg_rlsc_base(vi_pipe)     (hi_ext_base_addr(vi_pipe) + VREG_RLSC_OFFSET)

/* SIZE >:= 0x1000 */
/* LSC:  0x5800~0xA7FF */
#define VREG_LSC_OFFSET             (VREG_RLSC_OFFSET + VREG_RLSC_SIZE)
#define VREG_LSC_SIZE               0x5000
#define vreg_lsc_base(vi_pipe)      (hi_ext_base_addr(vi_pipe) + VREG_LSC_OFFSET)

/* SIZE >:= 0x10000 */
/* DPC:0xA800~0x1B7FF */
#define VREG_DPC_OFFSET             (VREG_LSC_OFFSET + VREG_LSC_SIZE)
#ifdef CONFIG_HI_ISP_DPC_STATIC_TABLE_SUPPORT
#define VREG_DPC_SIZE               0x11000
#else
#define VREG_DPC_SIZE               0x1000
#endif
#define vreg_dpc_base(vi_pipe)       (hi_ext_base_addr(vi_pipe) + VREG_DPC_OFFSET)

/* RGBIR:0x1B800~0x1B8FF */
#define VREG_RGBIR_OFFSET             (VREG_DPC_OFFSET + VREG_DPC_SIZE)
#define VREG_RGBIR_SIZE               0x100
#define vreg_rgbir_base(vi_pipe)      (hi_ext_base_addr(vi_pipe) + VREG_RGBIR_OFFSET)

/* 0x4700 reserved */
/* AI Color:0x1B900~0x1B92F */
#define VREG_AI_COLOR_OFFSET          (VREG_RGBIR_OFFSET + VREG_RGBIR_SIZE)
#define VREG_AI_COLOR_SIZE            0x30
#define vreg_ai_color_base(vi_pipe)   (hi_ext_base_addr(vi_pipe) + VREG_AI_COLOR_OFFSET)

/* AI Sharpeness:0x1B930~0x1B97F */
#define VREG_AI_SHARPEN_OFFSET          (VREG_AI_COLOR_OFFSET + VREG_AI_COLOR_SIZE)
#define VREG_AI_SHARPEN_SIZE            0x50
#define vreg_ai_sharpen_base(vi_pipe)   (hi_ext_base_addr(vi_pipe) + VREG_AI_SHARPEN_OFFSET)

/* AI Noise:0x1B980~0x1B99F */
#define VREG_AI_NOISE_OFFSET          (VREG_AI_SHARPEN_OFFSET + VREG_AI_SHARPEN_SIZE)
#define VREG_AI_NOISE_SIZE            0x20
#define vreg_ai_noise_base(vi_pipe)   (hi_ext_base_addr(vi_pipe) + VREG_AI_NOISE_OFFSET)

/* AI Bright:0x1B9A0~0x1B9CF */
#define VREG_AI_BRIGHT_OFFSET          (VREG_AI_NOISE_OFFSET + VREG_AI_NOISE_SIZE)
#define VREG_AI_BRIGHT_SIZE            0x30
#define vreg_ai_bright_base(vi_pipe)   (hi_ext_base_addr(vi_pipe) + VREG_AI_BRIGHT_OFFSET)

/* AI Bright:0x1B9D0~0x1B9EF */
#define VREG_AI_CONTRAST_OFFSET          (VREG_AI_BRIGHT_OFFSET + VREG_AI_BRIGHT_SIZE)
#define VREG_AI_CONTRAST_SIZE            0x20
#define vreg_ai_contrast_base(vi_pipe)   (hi_ext_base_addr(vi_pipe) + VREG_AI_CONTRAST_OFFSET)

/* AI Group:0x1B9F0~0x1B9EF */
#define VREG_AI_GROUP_OFFSET          (VREG_AI_CONTRAST_OFFSET + VREG_AI_CONTRAST_SIZE)
#define VREG_AI_GROUP_SIZE            0x10
#define vreg_ai_group_base(vi_pipe)   (hi_ext_base_addr(vi_pipe) + VREG_AI_GROUP_OFFSET)

/* group: top */
/* register: hi_ext_top_wdr_cfg */
/* HI_TRUE: the state of WDR config is initialized; */
/* HI_FALSE: the state of WDR config is NOT initialized */
/* args: data (1-bit) */
static __inline hi_void hi_ext_top_wdr_cfg_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0x0000, (data & 0x01));
}
static __inline hi_u8 hi_ext_top_wdr_cfg_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_top_base(vi_pipe) + 0x0000) & 0x01);
}

/* register: hi_ext_top_pub_attr_cfg */
/* HI_TRUE: the state of pub_attr config is initialized; */
/* HI_FALSE: the state of pub_attr config is NOT initialized */
/* args: data (1-bit) */
static __inline hi_void hi_ext_top_pub_attr_cfg_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0x0002, (data & 0x01));
}
static __inline hi_u8 hi_ext_top_pub_attr_cfg_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_top_base(vi_pipe) + 0x0002) & 0x01);
}

/* register: hi_ext_top_wdr_switch */
/* HI_TRUE: WDR switch is complete; */
/* HI_FALSE: WDR switch is NOT complete */
/* args: data (1-bit) */
static __inline hi_void hi_ext_top_wdr_switch_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0x0004, (data & 0x01));
}
static __inline hi_u8 hi_ext_top_wdr_switch_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_top_base(vi_pipe) + 0x0004) & 0x01);
}

/* register: hi_ext_top_res_switch */
/* HI_TRUE: resolution switch is complete; */
/* HI_FALSE: resolution switch is NOT complete */
/* args: data (1-bit) */
static __inline hi_void hi_ext_top_res_switch_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0x0006, (data & 0x01));
}
static __inline hi_u8 hi_ext_top_res_switch_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_top_base(vi_pipe) + 0x0006) & 0x01);
}

/* register: hi_ext_top_sensor_width */
/* args: data (32-bit) */
static __inline hi_void hi_ext_top_sensor_width_write(hi_vi_pipe vi_pipe, hi_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x0008, data);
}
static __inline hi_u32 hi_ext_top_sensor_width_read(hi_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x0008);
}

/* register: hi_ext_top_sensor_width */
/* args: data (32-bit) */
static __inline hi_void hi_ext_top_sensor_height_write(hi_vi_pipe vi_pipe, hi_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x000c, data);
}
static __inline hi_u32 hi_ext_top_sensor_height_read(hi_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x000c);
}

/* args: data (32-bit) */
static __inline hi_void hi_ext_system_fps_base_write(hi_vi_pipe vi_pipe, hi_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x0010, data);
}
static __inline hi_u32 hi_ext_system_fps_base_read(hi_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x0010);
}

/* register: hi_ext_system_sensor_max_resolution_width */
/* width of sensor resolution */
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_sensor_max_resolution_width_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x0014, data);
}

static __inline hi_u16 hi_ext_system_sensor_max_resolution_width_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x0014);
}

/* register: hi_ext_system_sensor_max_resolution_height */
/* height of sensor resolution */
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_sensor_max_resolution_height_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x0016, data);
}

static __inline hi_u16 hi_ext_system_sensor_max_resolution_height_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x0016);
}

/* register: hi_ext_system_sys_debug_enable */
/* 0:disable system debug information */
/* 1:enable system debug information */
#define HI_EXT_SYSTEM_SYS_DEBUG_ENABLE_DEFAULT  0x0

/* args: data (1-bit) */
static __inline hi_void hi_ext_system_sys_debug_enable_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    hi_u16 current = iord_16direct(vreg_top_base(vi_pipe) + 0x0018);
    iowr_16direct(vreg_top_base(vi_pipe) + 0x0018, (((hi_u16) data) << 0) | (current & 0xFFFE));
}

static __inline hi_u16 hi_ext_system_sys_debug_enable_read(hi_vi_pipe vi_pipe)
{
    return (hi_u16) ((iord_16direct(vreg_top_base(vi_pipe) + 0x0018) & 0x0001) >> 0);
}

/* register: hi_ext_system_sys_debug_high_addr */
/* address of system debug information */
#define HI_EXT_SYSTEM_SYS_DEBUG_HIGH_ADDR_DEFAULT  0x0

/* args: data (32-bit) */
static __inline hi_void hi_ext_system_sys_debug_high_addr_write(hi_vi_pipe vi_pipe, hi_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x001c, data);
}

static __inline hi_u32 hi_ext_system_sys_debug_high_addr_read(hi_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x001c);
}

/* register: hi_ext_system_sys_debug_low_addr */
/* address of system debug information */
#define HI_EXT_SYSTEM_SYS_DEBUG_LOW_ADDR_DEFAULT  0x0

/* args: data (32-bit) */
static __inline hi_void hi_ext_system_sys_debug_low_addr_write(hi_vi_pipe vi_pipe, hi_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x0020, data);
}

static __inline hi_u32 hi_ext_system_sys_debug_low_addr_read(hi_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x0020);
}

/* register: hi_ext_system_sys_debug_size */
/* size of sys debug struct */
#define HI_EXT_SYSTEM_SYS_DEBUG_SIZE_DEFAULT  0x0

/* args: data (32-bit) */
static __inline hi_void hi_ext_system_sys_debug_size_write(hi_vi_pipe vi_pipe, hi_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x0024, data) ;
}

static __inline hi_u32 hi_ext_system_sys_debug_size_read(hi_vi_pipe vi_pipe)
{
    return (hi_u32)iord_32direct(vreg_top_base(vi_pipe) + 0x0024);
}

/* register: hi_ext_system_debug_depth */
/* the depth of all debug information */
#define HI_EXT_SYSTEM_DEBUG_DEPTH_DEFAULT  0x0

/* args: data (16-bit) */
static __inline hi_void hi_ext_system_sys_debug_depth_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x0028, data);
}

static __inline hi_u16 hi_ext_system_sys_debug_depth_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x0028);
}

/* register: hi_ext_system_statistics_ctrl_lowbit */
/* ISP statistics control low bit */
#define HI_EXT_SYSTEM_STATISTICS_LOWBIT_DEFAULT  (ISP_STATISTICS_KEY)

/* args: data (32-bit) */
static __inline hi_u32 hi_ext_system_statistics_ctrl_lowbit_read(hi_vi_pipe vi_pipe)
{
    return  iord_32direct(vreg_top_base(vi_pipe) + 0x002c);
}

static __inline hi_void hi_ext_system_statistics_ctrl_lowbit_write(hi_vi_pipe vi_pipe, hi_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x002c, data);
}

/* register: hi_ext_system_statistics_ctrl_highbit */
/* ISP statistics control high bit */
#define HI_EXT_SYSTEM_STATISTICS_HIGHBIT_DEFAULT  (ISP_STATISTICS_KEY)

/* args: data (32-bit) */
static __inline hi_u32 hi_ext_system_statistics_ctrl_highbit_read(hi_vi_pipe vi_pipe)
{
    return  iord_32direct(vreg_top_base(vi_pipe) + 0x0030);
}

static __inline hi_void hi_ext_system_statistics_ctrl_highbit_write(hi_vi_pipe vi_pipe, hi_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x0030, data);
}

/* register: hi_ext_system_bind_attr */
/* isp bind attr include ae_lib awb_lib sensor_id etc */
/* |----------------------------------------------------------------| */
/* |           RSV          |     AE_LIB_ID    |   AWB_LIB_ID       | */
/* |----------------------------------------------------------------| */
/* |<--------16bits--------><-------8bits------><------8bits------->| */
#define HI_EXT_SYSTEM_BIND_ATTR_DEFAULT  0x0

/* args: data (32-bit) */
static __inline hi_u32 hi_ext_system_bind_attr_read(hi_vi_pipe vi_pipe)
{
    return  iord_32direct(vreg_top_base(vi_pipe) + 0x0034);
}
static __inline hi_void hi_ext_system_bind_attr_write(hi_vi_pipe vi_pipe, hi_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x0034, data);
}

/* register: hi_ext_system_freeze_firmware */
/* disables firmware and stops updating the ISP */
#define HI_EXT_SYSTEM_FREEZE_FIRMWARE_DEFAULT  0

/* args: data (1-bit) */
static __inline hi_void hi_ext_system_freeze_firmware_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    hi_u8 current = iord_8direct(vreg_top_base(vi_pipe) + 0x0038);
    iowr_8direct(vreg_top_base(vi_pipe) + 0x0038, (data & 0x01) | (current & 0xFE));
}
static __inline hi_u8 hi_ext_system_freeze_firmware_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_top_base(vi_pipe) + 0x0038) & 0x01);
}

/* register: hi_ext_system_sensor_wdr_mode */
/* WDR mode: 0=linear 1=built-in wdr 2=2to1 full frame wdr  ... */
#define HI_EXT_SYSTEM_SENSOR_WDR_MODE_DEFAULT   0x0

/* args: data (6-bit) */
static __inline hi_void hi_ext_system_sensor_wdr_mode_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    hi_u16 current = iord_16direct(vreg_top_base(vi_pipe) + 0x003a);
    iowr_16direct(vreg_top_base(vi_pipe) + 0x003a, (((hi_u16) (data & 0x3F)) << 4) | (current & 0xFC0F)); /* shift 4 */
}
static __inline hi_u8 hi_ext_system_sensor_wdr_mode_read(hi_vi_pipe vi_pipe)
{
    return (hi_u8)((iord_16direct(vreg_top_base(vi_pipe) + 0x003a) & 0x03F0) >> 4);    /* right shift 4 */
}

/* register: hi_ext_sync_total_width */
/* total frame width including horizontal blanking */
#define HI_EXT_SYNC_TOTAL_WIDTH_DEFAULT  0x672
/* args: data (16-bit) */
static __inline hi_void hi_ext_sync_total_width_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x3c, data);
}
static __inline hi_u16 hi_ext_sync_total_width_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x3c);
}

/* register: hi_ext_sync_total_height */
/* total frame height including vertical blanking */
#define HI_EXT_SYNC_TOTAL_HEIGHT_DEFAULT  0x2EE

/* args: data (16-bit) */
static __inline hi_void hi_ext_sync_total_height_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x40, data);
}
static __inline hi_u16 hi_ext_sync_total_height_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x40);
}

static __inline hi_void hi_ext_system_be_total_width_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x44, data);
}
static __inline hi_u16 hi_ext_system_be_total_width_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x44);
}

static __inline hi_void hi_ext_system_be_total_height_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x46, data);
}
static __inline hi_u16 hi_ext_system_be_total_height_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x46);
}

static __inline hi_void hi_ext_system_update_info_high_phyaddr_write(hi_vi_pipe vi_pipe, hi_u32 data)
{
    iowr_32direct((vreg_top_base(vi_pipe) + 0xa4), data);
}

static __inline hi_u32 hi_ext_system_update_info_high_phyaddr_read(hi_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0xa4);
}

static __inline hi_void hi_ext_system_update_info_low_phyaddr_write(hi_vi_pipe vi_pipe, hi_u32 data)
{
    iowr_32direct((vreg_top_base(vi_pipe) + 0xa8), data);
}

static __inline hi_u32 hi_ext_system_update_info_low_phyaddr_read(hi_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0xa8);
}

static __inline hi_void hi_ext_system_sys_iso_write(hi_vi_pipe vi_pipe, hi_u32 data)
{
    iowr_32direct((vreg_top_base(vi_pipe) + 0xac), data);
}

static __inline hi_u32 hi_ext_system_sys_iso_read(hi_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0xac);
}
/* register: hi_ext_system_gamma_curve_type */
/* the type of gamma curve */
#define HI_EXT_SYSTEM_GAMMA_CURVE_TYPE_DEFAULT  0

/* args: data (8-bit) */
static __inline hi_void hi_ext_system_gamma_curve_type_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0xc8, data);
}
static __inline hi_u8 hi_ext_system_gamma_curve_type_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_top_base(vi_pipe) + 0xc8);
}

/* register: isp_crop_pos_x_write */
static __inline hi_void hi_ext_system_corp_pos_x_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0xd4, data);
}
static __inline hi_u16 hi_ext_system_corp_pos_x_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0xd4);
}

/* register: isp_crop_pos_y_write */
static __inline hi_void hi_ext_system_corp_pos_y_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0xd6, data);
}
static __inline hi_u16 hi_ext_system_corp_pos_y_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0xd6);
}

/* register: hi_ext_system_isp_bit_bypass */
#define HI_EXT_SYS_ISP_BYPASS_DEFAULT   0

static __inline hi_void hi_ext_system_isp_bit_bypass_write(hi_vi_pipe vi_pipe, hi_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0xd8, data);
}

static __inline hi_u32 hi_ext_system_isp_bit_bypass_read(hi_vi_pipe vi_pipe)
{
    return  iord_32direct(vreg_top_base(vi_pipe) + 0xd8);
}

#define HI_EXT_SYSTEM_TOP_BIT_RESERVE0_WRITE_DEFAULT  0

static __inline hi_void hi_ext_system_top_bit_reserve0_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0xe0, data & 0xF);
}

static __inline hi_u8 hi_ext_system_top_bit_reserve0_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_top_base(vi_pipe) + 0xe0) & 0xF);
}

#define HI_EXT_SYSTEM_TOP_BIT_RESERVE1_WRITE_DEFAULT  0

static __inline hi_void hi_ext_system_top_bit_reserve1_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0xe1, data & 0x3);
}

static __inline hi_u8 hi_ext_system_top_bit_reserve1_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_top_base(vi_pipe) + 0xe1) & 0x3);
}

#define HI_EXT_SYSTEM_TOP_BIT_RESERVE2_WRITE_DEFAULT  0
static __inline hi_void hi_ext_system_top_bit_reserve2_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0xe2, data & 0x3);
}

static __inline hi_u8 hi_ext_system_top_bit_reserve2_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_top_base(vi_pipe) + 0xe2) & 0x3);
}

#define HI_EXT_SYSTEM_TOP_BIT_RESERVE3_WRITE_DEFAULT  0
static __inline hi_void hi_ext_system_top_bit_reserve3_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0xe3, data & 0x3);
}

static __inline hi_u8 hi_ext_system_top_bit_reserve3_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_top_base(vi_pipe) + 0xe3) & 0x3);
}

#define HI_EXT_SYSTEM_TOP_BIT_RESERVE4_WRITE_DEFAULT   0
static __inline hi_void hi_ext_system_top_bit_reserve4_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0xe4, data & 0x7);
}

static __inline hi_u8 hi_ext_system_top_bit_reserve4_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_top_base(vi_pipe) + 0xe4) & 0x7);
}

static __inline hi_void hi_ext_system_yuv_format_write(hi_vi_pipe vi_pipe, hi_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0xec, data);
}

static __inline hi_u32 hi_ext_system_yuv_format_read(hi_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0xec);
}

/* register: hi_ext_system_rggb_cfg_write */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_rggb_cfg_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0xf0, (data & 0xff));
}
static __inline hi_u8 hi_ext_system_rggb_cfg_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_top_base(vi_pipe) + 0xf0) & 0xff);
}

/* register: hi_ext_system_sensor_mode */
/* used to distinguish the special sensor mode */
#define HI_EXT_SYSTEM_SENSOR_MODE_DEFAULT  0x0

/* args: data (1-bit) */
static __inline hi_void hi_ext_system_sensor_mode_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0x00f2, (data & 0xff));
}
static __inline hi_u8 hi_ext_system_sensor_mode_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_top_base(vi_pipe) + 0x00f2) & 0xff);
}

/* register: hi_ext_system_isp_dgain_bypass */
/*  */
#define HI_EXT_SYS_ISP_DGAIN_ENABLE_DEFAULT  0x0
#define HI_EXT_SYS_ISP_DGAIN_ENABLE_DATASIZE 1

static __inline hi_void hi_ext_system_isp_dgain_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0x00f4, (data & 0x01));
}

static __inline hi_u8 hi_ext_system_isp_dgain_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_top_base(vi_pipe) + 0x00f4) & 0x01);
}

/* register: hi_ext_system_be_free_buffer_high_addr */
#define HI_EXT_SYSTEM_BE_FREE_BUFFER_HIGH_ADDR_DEFAULT  0x0

/* args: data (32-bit) */
static __inline hi_void hi_ext_system_be_free_buffer_high_addr_write(hi_vi_pipe vi_pipe, hi_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x0100, data);
}

static __inline hi_u32 hi_ext_system_be_free_buffer_high_addr_read(hi_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x0100);
}

/* register: hi_ext_system_be_free_buffer_low_addr */
#define HI_EXT_SYSTEM_BE_FREE_BUFFER_LOW_ADDR_DEFAULT   0x0

/* args: data (32-bit) */
static __inline hi_void hi_ext_system_be_free_buffer_low_addr_write(hi_vi_pipe vi_pipe, hi_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x0104, data);
}

static __inline hi_u32 hi_ext_system_be_free_buffer_low_addr_read(hi_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x0104);
}

/* register: hi_ext_system_manual_sharpen_overshoot_amt */
/* sharpening overshoot amt */
#define HI_EXT_SYSTEM_ACTUAL_SHARPEN_OVERSHOOT_DEFAULT                  150

/* args: data (8-bit) */
static __inline hi_void hi_ext_system_actual_sharpen_overshoot_amt_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0x0108, data);
}
static __inline hi_u8 hi_ext_system_actual_sharpen_overshoot_amt_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_top_base(vi_pipe) + 0x0108);
}

/* register: hi_ext_system_manual_sharpen_undershoot_amt_write */
/* sharpening undershoot amt */
#define HI_EXT_SYSTEM_ACTUAL_SHARPEN_UNDERSHOOT_DEFAULT            200
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_actual_sharpen_undershoot_amt_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0x010a, data);
}
static __inline hi_u8 hi_ext_system_actual_sharpen_undershoot_amt_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_top_base(vi_pipe) + 0x010a);
}

#define HI_EXT_SYSTEM_ACTUAL_SHARPEN_SHOOTSUPST_DEFAULT            1

static __inline hi_void hi_ext_system_actual_sharpen_shoot_sup_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0x010c, data);
}

static __inline hi_u8 hi_ext_system_actual_sharpen_shoot_sup_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_top_base(vi_pipe) + 0x010c);
}

static  __inline hi_void hi_ext_system_actual_sharpen_edge_frequence_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x010e, data);
}

static __inline hi_u16 hi_ext_system_actual_sharpen_edge_frequence_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x010e);
}

static  __inline hi_void hi_ext_system_actual_sharpen_texture_frequence_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x0110, data);
}

static __inline hi_u16 hi_ext_system_actual_sharpen_texture_frequence_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x0110);
}

static __inline hi_void hi_ext_system_actual_sharpen_edge_str_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x0120 + index * sizeof(hi_u16), data);
}

static __inline hi_u16 hi_ext_system_actual_sharpen_edge_str_read(hi_vi_pipe vi_pipe, hi_u8 index)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x0120 + index * sizeof(hi_u16));
}

static __inline hi_void hi_ext_system_actual_sharpen_texture_str_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x0160 + index * sizeof(hi_u16), data);
}

static __inline hi_u16 hi_ext_system_actual_sharpen_texture_str_read(hi_vi_pipe vi_pipe, hi_u8 index)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x0160 + index * sizeof(hi_u16));
}

#define HI_EXT_SYSTEM_BAYERNR_ACTUAL_STRENGTH_DEFAULT  100

/* args: data (8-bit) */
static __inline hi_void hi_ext_system_bayernr_actual_fine_strength_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0x01A0, data);
}
static __inline hi_u8 hi_ext_system_bayernr_actual_fine_strength_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_top_base(vi_pipe) + 0x01A0);
}

#define  HI_EXT_SYSTEM_BAYERNR_ACTUAL_CORING_WEIGHT_DEFAULT 800
static __inline hi_void hi_ext_system_bayernr_actual_coring_weight_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x01A2, data);
}

static __inline hi_u16 hi_ext_system_bayernr_actual_coring_weight_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x01A2);
}

static __inline hi_void hi_ext_system_bayernr_actual_nr_lsc_ratio_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0x01A4, data);
}
static __inline hi_u8 hi_ext_system_bayernr_actual_nr_lsc_ratio_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_top_base(vi_pipe) + 0x01A4);
}

/* args: data (8-bit) */
static __inline hi_void hi_ext_system_bayernr_actual_chroma_strength_write(hi_vi_pipe vi_pipe, hi_u8 i, hi_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0x01A8 + (i * sizeof(hi_u8)), data);
}
static __inline hi_u8 hi_ext_system_bayernr_actual_chroma_strength_read(hi_vi_pipe vi_pipe, hi_u8 i)
{
    return iord_8direct(vreg_top_base(vi_pipe) + 0x01A8 + (i * sizeof(hi_u8)));
}

/* args: data (8-bit) */
static __inline hi_void hi_ext_system_bayernr_actual_wdr_frame_strength_write(hi_vi_pipe vi_pipe, hi_u8 i, hi_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0x01AC + (i * sizeof(hi_u8)), data);
}
static __inline hi_u8 hi_ext_system_bayernr_actual_wdr_frame_strength_read(hi_vi_pipe vi_pipe, hi_u8 i)
{
    return iord_8direct(vreg_top_base(vi_pipe) + 0x01AC + (i * sizeof(hi_u8)));
}

/* args: data (16-bit) */
static __inline hi_void hi_ext_system_bayernr_actual_coarse_strength_write(hi_vi_pipe vi_pipe, hi_u8 i, hi_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x0200 + (i * sizeof(hi_u16)), data);
}
static __inline hi_u16 hi_ext_system_bayernr_actual_coarse_strength_read(hi_vi_pipe vi_pipe, hi_u8 i)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x0200 + (i * sizeof(hi_u16)));
}

static __inline hi_void  hi_ext_system_dehaze_actual_strength_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x0208, data);
}
static __inline hi_u16  hi_ext_system_dehaze_actual_strength_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x0208);
}

static __inline hi_void  hi_ext_system_drc_actual_strength_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x020A, data);
}
static __inline hi_u16  hi_ext_system_drc_actual_strength_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x020A);
}

static __inline hi_void  hi_ext_system_actual_wdr_exposure_ratio_write(hi_vi_pipe vi_pipe, hi_u8 i, hi_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x0210 + (i * sizeof(hi_u32)), data);
}
static __inline hi_u32  hi_ext_system_actual_wdr_exposure_ratio_read(hi_vi_pipe vi_pipe, hi_u8 i)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x0210 + (i * sizeof(hi_u32)));
}

static __inline hi_void hi_ext_system_block_num_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0x220, (data & 0xff));
}
static __inline hi_u8 hi_ext_system_block_num_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_top_base(vi_pipe) + 0x220) & 0xff);
}

#define HI_EXT_SYSTEM_TOP_CHANNEL_SELECT_WRITE_DEFAULT  0

static __inline hi_void hi_ext_system_top_channel_select_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0x224, data & 0x3);
}

static __inline hi_u8 hi_ext_system_top_channel_select_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_top_base(vi_pipe) + 0x224) & 0x3);
}

#define HI_EXT_SYSTEM_TOP_CHANNEL_SELECT_PRE_WRITE_DEFAULT  0

static __inline hi_void hi_ext_system_top_channel_select_pre_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0x226, data & 0x3);
}

static __inline hi_u8 hi_ext_system_top_channel_select_pre_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_top_base(vi_pipe) + 0x226) & 0x3);
}

/* ISP multi-pipe different config attr */
/* register: hi_ext_system_isp_pipe_diff_offset */
#define HI_EXT_SYSTEM_ISP_PIPE_DIFF_OFFSET_DEFAULT  0x0

/* args: data (32-bit) */
static __inline hi_void hi_ext_system_isp_pipe_diff_offset_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_s32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x230 + (index * sizeof(hi_s32)), data);
}
static __inline hi_s32 hi_ext_system_isp_pipe_diff_offset_read(hi_vi_pipe vi_pipe, hi_u8 index)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x230 + (index * sizeof(hi_s32)));
}

/* register: hi_ext_system_isp_pipe_diff_gain */
#define HI_EXT_SYSTEM_ISP_PIPE_DIFF_GAIN_DEFAULT  0x100

/* args: data (32-bit) */
static __inline hi_void hi_ext_system_isp_pipe_diff_gain_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x240 + (index * sizeof(hi_u32)), data);
}
static __inline hi_u32 hi_ext_system_isp_pipe_diff_gain_read(hi_vi_pipe vi_pipe, hi_u8 index)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x240 + (index * sizeof(hi_u32)));
}

/* register: hi_ext_system_isp_matrix_coefft */
#define HI_EXT_SYSTEM_ISP_PIPE_DIFF_CCM_DEFAULT  0x100

/* args: data (16-bit) */
static __inline hi_void hi_ext_system_isp_pipe_diff_ccm_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x250 + (index << 1), data);
}
static __inline hi_u16 hi_ext_system_isp_pipe_diff_ccm_read(hi_vi_pipe vi_pipe, hi_u8 index)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x250 + (index << 1));
}

/* register: hi_ext_system_black_level_query */
/* R channel black level */
static __inline hi_void hi_ext_system_black_level_query_00_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x270, data);
}

static __inline hi_u16 hi_ext_system_black_level_query_00_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x270);
}
/* gr channel black level */
static __inline hi_void hi_ext_system_black_level_query_01_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x272, data);
}

static __inline hi_u16 hi_ext_system_black_level_query_01_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x272);
}

/* gb channel black level */
static __inline hi_void hi_ext_system_black_level_query_10_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x274, data);
}

static __inline hi_u16 hi_ext_system_black_level_query_10_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x274);
}

/* B channel black level */
static __inline hi_void hi_ext_system_black_level_query_11_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_top_base(vi_pipe) + 0x276, data);
}

static __inline hi_u16 hi_ext_system_black_level_query_11_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_top_base(vi_pipe) + 0x276);
}

/* register: hi_ext_system_be_free_buffer_high_addr */
#define HI_EXT_SYSTEM_BE_LUT_STT_BUFFER_HIGH_ADDR_DEFAULT  0x0

/* args: data (32-bit) */
static __inline hi_void hi_ext_system_be_lut_stt_buffer_high_addr_write(hi_vi_pipe vi_pipe, hi_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x278, data);
}

static __inline hi_u32 hi_ext_system_be_lut_stt_buffer_high_addr_read(hi_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x278);
}

/* register: hi_ext_system_be_free_buffer_low_addr */
#define HI_EXT_SYSTEM_BE_LUT_STT_BUFFER_LOW_ADDR_DEFAULT  0x0

/* args: data (32-bit) */
static __inline hi_void hi_ext_system_be_lut_stt_buffer_low_addr_write(hi_vi_pipe vi_pipe, hi_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x27c, data);
}

static __inline hi_u32 hi_ext_system_be_lut_stt_buffer_low_addr_read(hi_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x27c);
}

/* register: hi_ext_system_be_buffer_address_high */
/* args: data (32-bit) */
static __inline hi_void hi_ext_system_be_buffer_address_high_write(hi_vi_pipe vi_pipe, hi_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x02a0, data);
}

static __inline hi_u32 hi_ext_system_be_buffer_address_high_read(hi_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x02a0);
}

/* register: hi_ext_system_be_buffer_address_low */
/* args: data (32-bit) */
static __inline hi_void hi_ext_system_be_buffer_address_low_write(hi_vi_pipe vi_pipe, hi_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x02a4, data);
}

static __inline hi_u32 hi_ext_system_be_buffer_address_low_read(hi_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x02a4);
}

/* register: hi_ext_system_bebuf_size */
/* args: data (32-bit) */
static __inline hi_void hi_ext_system_be_buffer_size_write(hi_vi_pipe vi_pipe, hi_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x02a8, data);
}

static __inline hi_u32 hi_ext_system_be_buffer_size_read(hi_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x02a8);
}

/* register: hi_ext_system_ldci_read_stt_buffer_high_addr */
#define HI_EXT_SYSTEM_LDCI_READ_STT_BUF_HIGH_ADDR_DEFAULT 0x0
/* args: data (32-bit) */
static __inline hi_void hi_ext_system_ldci_read_stt_buffer_high_addr_write(hi_vi_pipe vi_pipe, hi_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x2ac, data);
}

static __inline hi_u32 hi_ext_system_ldci_read_stt_buffer_high_addr_read(hi_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x2ac);
}

/* register: hi_ext_system_ldci_read_stt_buffer_low_addr */
#define HI_EXT_SYSTEM_LDCI_READ_STT_BUF_LOW_ADDR_DEFAULT 0x0

/* args: data (32-bit) */
static __inline hi_void hi_ext_system_ldci_read_stt_buffer_low_addr_write(hi_vi_pipe vi_pipe, hi_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x2b0, data);
}

static __inline hi_u32 hi_ext_system_ldci_read_stt_buffer_low_addr_read(hi_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x2b0);
}

static __inline hi_void hi_ext_system_isp_raw_position_write(hi_vi_pipe vi_pipe, hi_u32 data)
{
    iowr_32direct(vreg_top_base(vi_pipe) + 0x300, data);
}

static __inline hi_u32 hi_ext_system_isp_raw_position_read(hi_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_top_base(vi_pipe) + 0x300);
}

#define  HI_EXT_SYSTEM_ISP_RAW_POSITION_ENABLE_WRITE_DEFAULT  0
static __inline hi_void hi_ext_system_isp_raw_position_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_top_base(vi_pipe) + 0x304, (data & 0x01));
}

static __inline hi_u8 hi_ext_system_isp_raw_position_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_top_base(vi_pipe) + 0x304) & 0x01);
}

/* register: hi_ext_af_enable */
/* used to */
/* args: data (1-bit) */
static __inline hi_void hi_ext_af_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0000, (data & 0x01));
}
static __inline hi_u8 hi_ext_af_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0000) & 0x01);
}

static __inline hi_void hi_ext_af_iir0_enable0_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0001, (data & 0x01));
}
static __inline hi_u8 hi_ext_af_iir0_enable0_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0001) & 0x01);
}

static __inline hi_void hi_ext_af_iir0_enable1_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0002, (data & 0x01));
}
static __inline hi_u8 hi_ext_af_iir0_enable1_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0002) & 0x01);
}

static __inline hi_void hi_ext_af_iir0_enable2_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0003, (data & 0x01));
}
static __inline hi_u8 hi_ext_af_iir0_enable2_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0003) & 0x01);
}

static __inline hi_void hi_ext_af_iir1_enable0_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0004, (data & 0x01));
}
static __inline hi_u8 hi_ext_af_iir1_enable0_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0004) & 0x01);
}

static __inline hi_void hi_ext_af_iir1_enable1_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0005, (data & 0x01));
}
static __inline hi_u8 hi_ext_af_iir1_enable1_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0005) & 0x01);
}

static __inline hi_void hi_ext_af_iir1_enable2_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0006, (data & 0x01));
}
static __inline hi_u8 hi_ext_af_iir1_enable2_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0006) & 0x01);
}

static __inline hi_void hi_ext_af_peakmode_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0007, (data & 0x01));
}
static __inline hi_u8 hi_ext_af_peakmode_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0007) & 0x01);
}

static __inline hi_void hi_ext_af_squmode_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0008, (data & 0x01));
}
static __inline hi_u8 hi_ext_af_squmode_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0008) & 0x01);
}

static __inline hi_void hi_ext_af_offset_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0009, (data & 0x01));
}
static __inline hi_u8 hi_ext_af_offset_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0009) & 0x01);
}

static __inline hi_void hi_ext_af_crop_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x000a, (data & 0x01));
}
static __inline hi_u8 hi_ext_af_crop_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x000a) & 0x01);
}

static __inline hi_void hi_ext_af_lpf_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x000b, (data & 0x01));
}
static __inline hi_u8 hi_ext_af_lpf_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x000b) & 0x01);
}

static __inline hi_void hi_ext_af_mean_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x000c, (data & 0x01));
}
static __inline hi_u8 hi_ext_af_mean_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x000c) & 0x01);
}

static __inline hi_void hi_ext_af_rawmode_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x000d, (data & 0x01));
}
static __inline hi_u8 hi_ext_af_rawmode_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x000d) & 0x01);
}

static __inline hi_void hi_ext_af_iir0_ds_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x000e, (data & 0x01));
}
static __inline hi_u8 hi_ext_af_iir0_ds_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x000e) & 0x01);
}

static __inline hi_void hi_ext_af_iir1_ds_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x000f, (data & 0x01));
}
static __inline hi_u8 hi_ext_af_iir1_ds_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x000f) & 0x01);
}

static __inline hi_void hi_ext_af_fir0_lpf_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0010, (data & 0x01));
}
static __inline hi_u8 hi_ext_af_fir0_lpf_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0010) & 0x01);
}

static __inline hi_void hi_ext_af_fir1_lpf_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0011, (data & 0x01));
}
static __inline hi_u8 hi_ext_af_fir1_lpf_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0011) & 0x01);
}

static __inline hi_void hi_ext_af_iir0_ldg_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0012, (data & 0x01));
}
static __inline hi_u8 hi_ext_af_iir0_ldg_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0012) & 0x01);
}

static __inline hi_void hi_ext_af_iir1_ldg_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0013, (data & 0x01));
}
static __inline hi_u8 hi_ext_af_iir1_ldg_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0013) & 0x01);
}

static __inline hi_void hi_ext_af_fir0_ldg_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0014, (data & 0x01));
}
static __inline hi_u8 hi_ext_af_fir0_ldg_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0014) & 0x01);
}

static __inline hi_void hi_ext_af_fir1_ldg_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0015, (data & 0x01));
}
static __inline hi_u8 hi_ext_af_fir1_ldg_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0015) & 0x01);
}

static __inline hi_void hi_ext_af_updatemode_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0016, (data & 0x01));
}
static __inline hi_u8 hi_ext_af_updatemode_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0016) & 0x01);
}

static __inline hi_void hi_ext_af_update_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0017, (data & 0x01));
}
static __inline hi_u8 hi_ext_af_update_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0017) & 0x01);
}

/* 8bit */
static __inline hi_void hi_ext_af_bayermode_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0018, (data & 0xff));
}
static __inline hi_u8 hi_ext_af_bayermode_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0018) & 0xff);
}

static __inline hi_void hi_ext_af_gamma_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0019, (data & 0xff));
}
static __inline hi_u8 hi_ext_af_gamma_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0019) & 0xff);
}

static __inline hi_void hi_ext_af_gain_limit_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x001a, (data & 0xff));
}
static __inline hi_u8 hi_ext_af_gain_limit_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x001a) & 0xff);
}

static __inline hi_void hi_ext_af_iir_plg_group0_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x001b, (data & 0xff));
}
static __inline hi_u8 hi_ext_af_iir_plg_group0_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x001b) & 0xff);
}

static __inline hi_void hi_ext_af_iir_pls_group0_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x001c, (data & 0xff));
}
static __inline hi_u8 hi_ext_af_iir_pls_group0_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x001c) & 0xff);
}

static __inline hi_void hi_ext_af_iir_plg_group1_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x001d, (data & 0xff));
}
static __inline hi_u8 hi_ext_af_iir_plg_group1_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x001d) & 0xff);
}

static __inline hi_void hi_ext_af_iir_pls_group1_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x001e, (data & 0xff));
}
static __inline hi_u8 hi_ext_af_iir_pls_group1_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x001e) & 0xff);
}

static __inline hi_void hi_ext_af_cnt_shift0_h_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x001f, (data & 0xff));
}
static __inline hi_u8 hi_ext_af_cnt_shift0_h_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x001f) & 0xff);
}

static __inline hi_void hi_ext_af_cnt_shift1_h_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0020, (data & 0xff));
}
static __inline hi_u8 hi_ext_af_cnt_shift1_h_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0020) & 0xff);
}

static __inline hi_void hi_ext_af_cnt_shift0_v_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0021, (data & 0xff));
}
static __inline hi_u8 hi_ext_af_cnt_shift0_v_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0021) & 0xff);
}

static __inline hi_void hi_ext_af_cnt_shift_y_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0022, (data & 0xff));
}
static __inline hi_u8 hi_ext_af_cnt_shift_y_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0022) & 0xff);
}

static __inline hi_void hi_ext_af_iir_dilate0_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0023, (data & 0xff));
}
static __inline hi_u8 hi_ext_af_iir_dilate0_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0023) & 0xff);
}

static __inline hi_void hi_ext_af_iir_dilate1_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0024, (data & 0xff));
}
static __inline hi_u8 hi_ext_af_iir_dilate1_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0024) & 0xff);
}

static __inline hi_void hi_ext_af_hiligh_thre_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0025, (data & 0xff));
}
static __inline hi_u8 hi_ext_af_hiligh_thre_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0025) & 0xff);
}

static __inline hi_void hi_ext_af_pos_sel_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0026, (data & 0xff));
}
static __inline hi_u8 hi_ext_af_pos_sel_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0026) & 0xff);
}

#define HI_EXT_AF_SET_FLAG_DISABLE 0x0
#define HI_EXT_AF_SET_FLAG_ENABLE  0x1
static __inline hi_void hi_ext_af_set_flag_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x0027, (data & 0xff));
}
static __inline hi_u8 hi_ext_af_set_flag_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x0027) & 0xff);
}

/* args: data (16-bit) */
static __inline hi_void hi_ext_af_mean_thres_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0028), data);
}
static __inline hi_u16 hi_ext_af_mean_thres_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0028));
}

static __inline hi_void hi_ext_af_window_vnum_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x002a), data);
}
static __inline hi_u16 hi_ext_af_window_vnum_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x002a));
}

static __inline hi_void hi_ext_af_window_hnum_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x002c), data);
}
static __inline hi_u16 hi_ext_af_window_hnum_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x002c));
}

static __inline hi_void hi_ext_af_iir_thre0_coring_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x002e), data);
}
static __inline hi_u16 hi_ext_af_iir_thre0_coring_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x002e));
}

static __inline hi_void hi_ext_af_iir_thre1_coring_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0030), data);
}
static __inline hi_u16 hi_ext_af_iir_thre1_coring_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0030));
}

static __inline hi_void hi_ext_af_iir_peak0_coring_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0032), data);
}
static __inline hi_u16 hi_ext_af_iir_peak0_coring_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0032));
}

static __inline hi_void hi_ext_af_iir_peak1_coring_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0034), data);
}
static __inline hi_u16 hi_ext_af_iir_peak1_coring_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0034));
}

static __inline hi_void hi_ext_af_fir_thre0_coring_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0036), data);
}
static __inline hi_u16 hi_ext_af_fir_thre0_coring_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0036));
}

static __inline hi_void hi_ext_af_fir_thre1_coring_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0038), data);
}
static __inline hi_u16 hi_ext_af_fir_thre1_coring_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0038));
}

static __inline hi_void hi_ext_af_fir_peak0_coring_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x003a), data);
}
static __inline hi_u16 hi_ext_af_fir_peak0_coring_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x003a));
}

static __inline hi_void hi_ext_af_fir_peak1_coring_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x003c), data);
}
static __inline hi_u16 hi_ext_af_fir_peak1_coring_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x003c));
}

static __inline hi_void hi_ext_af_iir_slope0_coring_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x003e), data);
}
static __inline hi_u16 hi_ext_af_iir_slope0_coring_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x003e));
}

static __inline hi_void hi_ext_af_iir_slope1_coring_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0040), data);
}
static __inline hi_u16 hi_ext_af_iir_slope1_coring_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0040));
}

static __inline hi_void hi_ext_af_fir_slope0_coring_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0042), data);
}
static __inline hi_u16 hi_ext_af_fir_slope0_coring_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0042));
}

static __inline hi_void hi_ext_af_fir_slope1_coring_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0044), data);
}
static __inline hi_u16 hi_ext_af_fir_slope1_coring_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0044));
}

static __inline hi_void hi_ext_af_offset_gr_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0046), data);
}
static __inline hi_u16 hi_ext_af_offset_gr_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0046));
}

static __inline hi_void hi_ext_af_offset_gb_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0048), data);
}
static __inline hi_u16 hi_ext_af_offset_gb_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0048));
}

static __inline hi_void hi_ext_af_input_v_size_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x004a), data);
}
static __inline hi_u16 hi_ext_af_input_v_size_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x004a));
}

static __inline hi_void hi_ext_af_input_h_size_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x004c), data);
}
static __inline hi_u16 hi_ext_af_input_h_size_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x004c));
}

static __inline hi_void hi_ext_af_iir0_shift_group0_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x004e), data);
}
static __inline hi_u16 hi_ext_af_iir0_shift_group0_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x004e));
}

static __inline hi_void hi_ext_af_iir1_shift_group0_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0050), data);
}
static __inline hi_u16 hi_ext_af_iir1_shift_group0_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0050));
}

static __inline hi_void hi_ext_af_iir2_shift_group0_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0052), data);
}
static __inline hi_u16 hi_ext_af_iir2_shift_group0_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0052));
}

static __inline hi_void hi_ext_af_iir3_shift_group0_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0054), data);
}
static __inline hi_u16 hi_ext_af_iir3_shift_group0_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0054));
}

static __inline hi_void hi_ext_af_iir0_shift_group1_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0056), data);
}
static __inline hi_u16 hi_ext_af_iir0_shift_group1_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0056));
}

static __inline hi_void hi_ext_af_iir1_shift_group1_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0058), data);
}
static __inline hi_u16 hi_ext_af_iir1_shift_group1_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0058));
}

static __inline hi_void hi_ext_af_iir2_shift_group1_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x005a), data);
}
static __inline hi_u16 hi_ext_af_iir2_shift_group1_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x005a));
}

static __inline hi_void hi_ext_af_iir3_shift_group1_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x005c), data);
}
static __inline hi_u16 hi_ext_af_iir3_shift_group1_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x005c));
}

static __inline hi_void hi_ext_af_iir_thre0_low_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x005e), data);
}
static __inline hi_u16 hi_ext_af_iir_thre0_low_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x005e));
}

static __inline hi_void hi_ext_af_iir_thre0_high_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0060), data);
}
static __inline hi_u16 hi_ext_af_iir_thre0_high_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0060));
}

static __inline hi_void hi_ext_af_iir_thre1_low_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0062), data);
}
static __inline hi_u16 hi_ext_af_iir_thre1_low_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0062));
}

static __inline hi_void hi_ext_af_iir_thre1_high_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0064), data);
}
static __inline hi_u16 hi_ext_af_iir_thre1_high_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0064));
}

static __inline hi_void hi_ext_af_iir_gain0_low_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0066), data);
}
static __inline hi_u16 hi_ext_af_iir_gain0_low_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0066));
}

static __inline hi_void hi_ext_af_iir_gain0_high_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0068), data);
}
static __inline hi_u16 hi_ext_af_iir_gain0_high_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0068));
}

static __inline hi_void hi_ext_af_iir_gain1_low_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x006a), data);
}
static __inline hi_u16 hi_ext_af_iir_gain1_low_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x006a));
}

static __inline hi_void hi_ext_af_iir_gain1_high_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x006c), data);
}
static __inline hi_u16 hi_ext_af_iir_gain1_high_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x006c));
}

static __inline hi_void hi_ext_af_iir_slope0_low_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x006e), data);
}
static __inline hi_u16 hi_ext_af_iir_slope0_low_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x006e));
}

static __inline hi_void hi_ext_af_iir_slope0_high_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0070), data);
}
static __inline hi_u16 hi_ext_af_iir_slope0_high_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0070));
}

static __inline hi_void hi_ext_af_iir_slope1_low_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0072), data);
}
static __inline hi_u16 hi_ext_af_iir_slope1_low_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0072));
}

static __inline hi_void hi_ext_af_iir_slope1_high_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0074), data);
}
static __inline hi_u16 hi_ext_af_iir_slope1_high_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0074));
}

static __inline hi_void hi_ext_af_fir_thre0_low_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0076), data);
}
static __inline hi_u16 hi_ext_af_fir_thre0_low_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0076));
}

static __inline hi_void hi_ext_af_fir_thre0_high_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0078), data);
}
static __inline hi_u16 hi_ext_af_fir_thre0_high_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0078));
}

static __inline hi_void hi_ext_af_fir_thre1_low_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x007a), data);
}
static __inline hi_u16 hi_ext_af_fir_thre1_low_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x007a));
}

static __inline hi_void hi_ext_af_fir_thre1_high_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x007c), data);
}
static __inline hi_u16 hi_ext_af_fir_thre1_high_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x007c));
}

static __inline hi_void hi_ext_af_fir_gain0_low_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x007e), data);
}
static __inline hi_u16 hi_ext_af_fir_gain0_low_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x007e));
}

static __inline hi_void hi_ext_af_fir_gain0_high_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0080), data);
}
static __inline hi_u16 hi_ext_af_fir_gain0_high_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0080));
}

static __inline hi_void hi_ext_af_fir_gain1_low_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0082), data);
}
static __inline hi_u16 hi_ext_af_fir_gain1_low_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0082));
}

static __inline hi_void hi_ext_af_fir_gain1_high_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0084), data);
}
static __inline hi_u16 hi_ext_af_fir_gain1_high_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0084));
}

static __inline hi_void hi_ext_af_fir_slope0_low_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0086), data);
}
static __inline hi_u16 hi_ext_af_fir_slope0_low_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0086));
}

static __inline hi_void hi_ext_af_fir_slope0_high_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0088), data);
}
static __inline hi_u16 hi_ext_af_fir_slope0_high_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0088));
}

static __inline hi_void hi_ext_af_fir_slope1_low_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x008a), data);
}
static __inline hi_u16 hi_ext_af_fir_slope1_low_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x008a));
}

static __inline hi_void hi_ext_af_fir_slope1_high_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x008c), data);
}
static __inline hi_u16 hi_ext_af_fir_slope1_high_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x008c));
}

static __inline hi_void hi_ext_af_acc_shift0_h_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x008e), data);
}
static __inline hi_u16 hi_ext_af_acc_shift0_h_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x008e));
}

static __inline hi_void hi_ext_af_acc_shift1_h_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0090), data);
}
static __inline hi_u16 hi_ext_af_acc_shift1_h_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0090));
}

static __inline hi_void hi_ext_af_acc_shift0_v_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0092), data);
}
static __inline hi_u16 hi_ext_af_acc_shift0_v_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0092));
}

static __inline hi_void hi_ext_af_acc_shift1_v_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0094), data);
}
static __inline hi_u16 hi_ext_af_acc_shift1_v_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0094));
}

static __inline hi_void hi_ext_af_acc_shift_y_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0096), data);
}
static __inline hi_u16 hi_ext_af_acc_shift_y_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0096));
}

static __inline hi_void hi_ext_af_crop_pos_y_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x0098), data);
}
static __inline hi_u16 hi_ext_af_crop_pos_y_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x0098));
}

static __inline hi_void hi_ext_af_crop_pos_x_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x009a), data);
}
static __inline hi_u16 hi_ext_af_crop_pos_x_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x009a));
}

static __inline hi_void hi_ext_af_crop_v_size_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x009c), data);
}
static __inline hi_u16 hi_ext_af_crop_v_size_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x009c));
}

static __inline hi_void hi_ext_af_crop_h_size_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x009e), data);
}
static __inline hi_u16 hi_ext_af_crop_h_size_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x009e));
}

static __inline hi_void hi_ext_af_iir_gain0_group0_write(hi_vi_pipe vi_pipe, hi_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00a0), data);
}
static __inline hi_s16 hi_ext_af_iir_gain0_group0_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00a0));
}

static __inline hi_void hi_ext_af_iir_gain0_group1_write(hi_vi_pipe vi_pipe, hi_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00a2), data);
}
static __inline hi_s16 hi_ext_af_iir_gain0_group1_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00a2));
}

static __inline hi_void hi_ext_af_iir_gain1_group0_write(hi_vi_pipe vi_pipe, hi_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00a4), data);
}
static __inline hi_s16 hi_ext_af_iir_gain1_group0_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00a4));
}

static __inline hi_void hi_ext_af_iir_gain1_group1_write(hi_vi_pipe vi_pipe, hi_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00a6), data);
}
static __inline hi_s16 hi_ext_af_iir_gain1_group1_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00a6));
}

static __inline hi_void hi_ext_af_iir_gain2_group0_write(hi_vi_pipe vi_pipe, hi_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00a8), data);
}
static __inline hi_s16 hi_ext_af_iir_gain2_group0_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00a8));
}

static __inline hi_void hi_ext_af_iir_gain2_group1_write(hi_vi_pipe vi_pipe, hi_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00aa), data);
}
static __inline hi_s16 hi_ext_af_iir_gain2_group1_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00aa));
}

static __inline hi_void hi_ext_af_iir_gain3_group0_write(hi_vi_pipe vi_pipe, hi_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00ac), data);
}
static __inline hi_s16 hi_ext_af_iir_gain3_group0_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00ac));
}

static __inline hi_void hi_ext_af_iir_gain3_group1_write(hi_vi_pipe vi_pipe, hi_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00ae), data);
}
static __inline hi_s16 hi_ext_af_iir_gain3_group1_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00ae));
}

static __inline hi_void hi_ext_af_iir_gain4_group0_write(hi_vi_pipe vi_pipe, hi_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00b0), data);
}
static __inline hi_s16 hi_ext_af_iir_gain4_group0_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00b0));
}

static __inline hi_void hi_ext_af_iir_gain4_group1_write(hi_vi_pipe vi_pipe, hi_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00b2), data);
}
static __inline hi_s16 hi_ext_af_iir_gain4_group1_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00b2));
}

static __inline hi_void hi_ext_af_iir_gain5_group0_write(hi_vi_pipe vi_pipe, hi_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00b4), data);
}
static __inline hi_s16 hi_ext_af_iir_gain5_group0_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00b4));
}

static __inline hi_void hi_ext_af_iir_gain5_group1_write(hi_vi_pipe vi_pipe, hi_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00b6), data);
}
static __inline hi_s16 hi_ext_af_iir_gain5_group1_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00b6));
}

static __inline hi_void hi_ext_af_iir_gain6_group0_write(hi_vi_pipe vi_pipe, hi_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00b8), data);
}
static __inline hi_s16 hi_ext_af_iir_gain6_group0_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00b8));
}

static __inline hi_void hi_ext_af_iir_gain6_group1_write(hi_vi_pipe vi_pipe, hi_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00ba), data);
}
static __inline hi_s16 hi_ext_af_iir_gain6_group1_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00ba));
}

static __inline hi_void hi_ext_af_fir_h_gain0_group0_write(hi_vi_pipe vi_pipe, hi_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00bc), data);
}
static __inline hi_s16 hi_ext_af_fir_h_gain0_group0_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00bc));
}

static __inline hi_void hi_ext_af_fir_h_gain0_group1_write(hi_vi_pipe vi_pipe, hi_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00be), data);
}
static __inline hi_s16 hi_ext_af_fir_h_gain0_group1_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00be));
}

static __inline hi_void hi_ext_af_fir_h_gain1_group0_write(hi_vi_pipe vi_pipe, hi_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00c0), data);
}
static __inline hi_s16 hi_ext_af_fir_h_gain1_group0_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00c0));
}

static __inline hi_void hi_ext_af_fir_h_gain1_group1_write(hi_vi_pipe vi_pipe, hi_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00c2), data);
}
static __inline hi_s16 hi_ext_af_fir_h_gain1_group1_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00c2));
}

static __inline hi_void hi_ext_af_fir_h_gain2_group0_write(hi_vi_pipe vi_pipe, hi_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00c4), data);
}
static __inline hi_s16 hi_ext_af_fir_h_gain2_group0_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00c4));
}

static __inline hi_void hi_ext_af_fir_h_gain2_group1_write(hi_vi_pipe vi_pipe, hi_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00c6), data);
}
static __inline hi_s16 hi_ext_af_fir_h_gain2_group1_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00c6));
}

static __inline hi_void hi_ext_af_fir_h_gain3_group0_write(hi_vi_pipe vi_pipe, hi_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00c8), data);
}
static __inline hi_s16 hi_ext_af_fir_h_gain3_group0_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00c8));
}

static __inline hi_void hi_ext_af_fir_h_gain3_group1_write(hi_vi_pipe vi_pipe, hi_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00ca), data);
}
static __inline hi_s16 hi_ext_af_fir_h_gain3_group1_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00ca));
}

static __inline hi_void hi_ext_af_fir_h_gain4_group0_write(hi_vi_pipe vi_pipe, hi_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00cc), data);
}
static __inline hi_s16 hi_ext_af_fir_h_gain4_group0_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00cc));
}

static __inline hi_void hi_ext_af_fir_h_gain4_group1_write(hi_vi_pipe vi_pipe, hi_s16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00ce), data);
}
static __inline hi_s16 hi_ext_af_fir_h_gain4_group1_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00ce));
}

static __inline hi_void hi_ext_af_shift_count_y_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_af_base(vi_pipe) + 0x00d0), data);
}
static __inline hi_u16 hi_ext_af_shift_count_y_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_af_base(vi_pipe) + 0x00d0));
}

/* 8bit */
static __inline hi_void hi_ext_af_iir0_shift_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x00d2, (data & 0xff));
}
static __inline hi_u8 hi_ext_af_iir0_shift_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x00d2) & 0xff);
}

static __inline hi_void hi_ext_af_iir1_shift_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x00d3, (data & 0xff));
}
static __inline hi_u8 hi_ext_af_iir1_shift_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x00d3) & 0xff);
}

/* args: data (2-bit) */
/*  AF sta info enable, [0] = FE sta,[1] = BE sta  */
static __inline hi_void hi_ext_system_af_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_af_base(vi_pipe) + 0x00e0, (data & HI_ISP_AF_ENABLE_MASK));
}
static __inline hi_u8 hi_ext_system_af_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_af_base(vi_pipe) + 0x00e0) & HI_ISP_AF_ENABLE_MASK);
}

/* ============================================================ */
/* DRC */
/* ============================================================ */
/* register: hi_ext_system_drc_enable_ */
/* DRC module enable */
static __inline hi_void hi_ext_system_drc_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_drc_base(vi_pipe) + 0x0, data);
}
static __inline hi_u8 hi_ext_system_drc_enable_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_drc_base(vi_pipe) + 0x0);
}

/* register: hi_ext_system_drc_manual_mode */
/* manual mode enable of drc module */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_manual_mode_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x1), data);
}
static __inline hi_u8 hi_ext_system_drc_manual_mode_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x1));
}

/* register: hi_ext_system_drc_monochrome_mode */
/* monochrome mode enable of drc module */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_monochrome_mode_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x2), data);
}
static __inline hi_u8 hi_ext_system_drc_monochrome_mode_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x2));
}

/* register: hi_ext_system_drc_asymmetry */
/* asymmetry parameter of DRC asymmetry curve */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_asymmetry_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_drc_base(vi_pipe) + 0x5, data);
}
static __inline hi_u8 hi_ext_system_drc_asymmetry_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x5));
}

/* register: hi_ext_system_drc_secondpole */
/* secondpole parameter of DRC asymmetry curve */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_secondpole_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x6), data);
}
static __inline hi_u8 hi_ext_system_drc_secondpole_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x6));
}

/* register: hi_ext_system_drc_compress */
/* compress parameter of DRC asymmetry curve */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_compress_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x7), data);
}
static __inline hi_u8 hi_ext_system_drc_compress_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x7));
}

/* register: hi_ext_system_drc_stretch */
/* stretch parameter of DRC asymmetry curve */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_stretch_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x8), data);
}
static __inline hi_u8 hi_ext_system_drc_stretch_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x8));
}

/* register: hi_ext_system_drc_colorcc_lut_up_en */
/* colorcc_lut_up_en */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_colorcc_lut_up_en_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x9), data);
}
static __inline hi_u8 hi_ext_system_drc_colorcc_lut_up_en_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x9));
}
/* register: hi_ext_system_drc_bin_numz */
/* bin_num_z */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_bin_num_z_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0xa), data);
}
static __inline hi_u8 hi_ext_system_drc_bin_num_z_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0xa));
}

/* register: hi_ext_system_drc_mixing_coring */
/* drc_mixing_coring */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_mixing_coring_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0xb), data);
}
static __inline hi_u8 hi_ext_system_drc_mixing_coring_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0xb));
}
/* register: hi_ext_system_drc_mixing_dark_min */
/* mixing_dark_min */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_mixing_dark_min_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0xc), data);
}
static __inline hi_u8 hi_ext_system_drc_mixing_dark_min_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0xc));
}
/* register: hi_ext_system_drc_mixing_dark_max */
/* mixing_dark_max */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_mixing_dark_max_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0xd), data);
}
static __inline hi_u8 hi_ext_system_drc_mixing_dark_max_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0xd));
}
/* register: hi_ext_system_drc_mixing_dark_slo */
/* mixing_dark_slo */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_mixing_dark_slo_write(hi_vi_pipe vi_pipe, hi_s8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0xe), data);
}
static __inline hi_s8 hi_ext_system_drc_mixing_dark_slo_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0xe));
}
/* register: hi_ext_system_drc_mixing_dark_thr */
/* mixing_dark_thr */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_mixing_dark_thr_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0xf), data);
}
static __inline hi_u8 hi_ext_system_drc_mixing_dark_thr_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0xf));
}

/* register: hi_ext_system_drc_mixing_bright_min */
/* mixing_bright_min */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_mixing_bright_min_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x10), data);
}
static __inline hi_u8 hi_ext_system_drc_mixing_bright_min_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x10));
}
/* register: hi_ext_system_drc_mixing_bright_max */
/* mixing_bright_max */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_mixing_bright_max_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x11), data);
}
static __inline hi_u8 hi_ext_system_drc_mixing_bright_max_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x11));
}
/* register: hi_ext_system_drc_mixing_bright_slo */
/* mixing_bright_slo */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_mixing_bright_slo_write(hi_vi_pipe vi_pipe, hi_s8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x12), data);
}
static __inline hi_s8 hi_ext_system_drc_mixing_bright_slo_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x12));
}
/* register: hi_ext_system_drc_mixing_bright_thr */
/* mixing_bright_thr */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_mixing_bright_thr_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x13), data);
}
static __inline hi_u8 hi_ext_system_drc_mixing_bright_thr_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x13));
}
/* register: hi_ext_system_drc_gain_clip_knee */
/* mixing_gain_clip_knee */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_gain_clip_knee_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x14), data);
}
static __inline hi_u8 hi_ext_system_drc_gain_clip_knee_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x14));
}
/* register: hi_ext_system_drc_gain_clip_step */
/* mixing_gain_clip_step */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_gain_clip_step_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x15), data);
}
static __inline hi_u8 hi_ext_system_drc_gain_clip_step_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x15));
}
/* register: hi_ext_system_drc_purple_high_slo */
/* purple_high_slo */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_purple_high_slo_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x16), data);
}
static __inline hi_u8 hi_ext_system_drc_purple_high_slo_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x16));
}
/* register: hi_ext_system_drc_purple_high_thr */
/* purple_high_thr */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_purple_high_thr_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x17), data);
}
static __inline hi_u8 hi_ext_system_drc_purple_high_thr_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x17));
}
/* register: hi_ext_system_drc_purple_low_slo */
/* purple_low_slo */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_purple_low_slo_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x18), data);
}
static __inline hi_u8 hi_ext_system_drc_purple_low_slo_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x18));
}
/* register: hi_ext_system_drc_purple_low_thr */
/* purple_low_thr */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_purple_low_thr_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x19), data);
}
static __inline hi_u8 hi_ext_system_drc_purple_low_thr_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x19));
}
/* register: hi_ext_system_drc_grad_shift */
/* grad_shift */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_grad_shift_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x1a), data);
}
static __inline hi_u8 hi_ext_system_drc_grad_shift_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x1a));
}
/* register: hi_ext_system_drc_grad_slope */
/* grad_slope */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_grad_slope_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x1b), data);
}
static __inline hi_u8 hi_ext_system_drc_grad_slope_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x1b));
}
/* register: hi_ext_system_drc_grad_max */
/* grad_max */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_grad_max_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x1c), data);
}
static __inline hi_u8 hi_ext_system_drc_grad_max_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x1c));
}
/* register: hi_ext_system_drc_grad_thr */
/* grad_thr */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_grad_thr_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x1d), data);
}
static __inline hi_u8 hi_ext_system_drc_grad_thr_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x1d));
}
/* register: hi_ext_system_drc_spa_coarse */
/* spa_coarse */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_var_spa_coarse_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x1e), data);
}
static __inline hi_u8 hi_ext_system_drc_var_spa_coarse_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x1e));
}
/* register: hi_ext_system_drc_spa_median */
/* spa_median */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_var_spa_medium_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x1f), data);
}
static __inline hi_u8 hi_ext_system_drc_var_spa_medium_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x1f));
}
/* register: hi_ext_system_drc_spa_fine */
/* spa_fine */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_var_spa_fine_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x20), data);
}
static __inline hi_u8 hi_ext_system_drc_var_spa_fine_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x20));
}
/* register: hi_ext_system_drc_rng_coarse */
/* rng_coarse */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_var_rng_coarse_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x21), data);
}
static __inline hi_u8 hi_ext_system_drc_var_rng_coarse_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x21));
}
/* register: hi_ext_system_drc_rng_medium */
/* rng_median */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_var_rng_medium_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x22), data);
}
static __inline hi_u8 hi_ext_system_drc_var_rng_medium_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x22));
}
/* register: hi_ext_system_drc_rng_fine */
/* rng_fine */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_var_rng_fine_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x23), data);
}
static __inline hi_u8 hi_ext_system_drc_var_rng_fine_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x23));
}

/* register: hi_ext_system_drc_bin_mix_coarse_lut */
/* bin_mix_coarse_lut */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_bin_mix_coarse_lut_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x24  +  index * sizeof(hi_u8)), data);
}
static __inline hi_u8 hi_ext_system_drc_bin_mix_coarse_lut_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return (iord_8direct((vreg_drc_base(vi_pipe) + 0x24  +  index * sizeof(hi_u8))));
}

/* register: hi_ext_system_drc_bin_mix_medium_lut */
/* bin_mix_medium_lut */
static __inline hi_void hi_ext_system_drc_bin_mix_medium_lut_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x2c  +  index * sizeof(hi_u8)), data);
}
static __inline hi_u8 hi_ext_system_drc_bin_mix_medium_lut_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return (iord_8direct((vreg_drc_base(vi_pipe) + 0x2c  +  index * sizeof(hi_u8))));
}

/* register: hi_ext_system_drc_detail_coring */
/* drc_detail_coring */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_detail_coring_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x34), data);
}
static __inline hi_u8 hi_ext_system_drc_detail_coring_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x34));
}
/* register: hi_ext_system_drc_detail_dark_min */
/* detail_dark_min */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_detail_dark_min_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x35), data);
}
static __inline hi_u8 hi_ext_system_drc_detail_dark_min_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x35));
}
/* register: hi_ext_system_drc_detail_dark_max */
/* detail_dark_max */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_detail_dark_max_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x36), data);
}
static __inline hi_u8 hi_ext_system_drc_detail_dark_max_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x36));
}
/* register: hi_ext_system_drc_detail_dark_slo */
/* detail_dark_slo */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_detail_dark_slo_write(hi_vi_pipe vi_pipe, hi_s8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x37), data);
}
static __inline hi_s8 hi_ext_system_drc_detail_dark_slo_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x37));
}
/* register: hi_ext_system_drc_detail_dark_thr */
/* detail_dark_thr */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_detail_dark_thr_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x38), data);
}
static __inline hi_u8 hi_ext_system_drc_detail_dark_thr_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x38));
}

/* register: hi_ext_system_drc_detail_bright_min */
/* detail_bright_min */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_detail_bright_min_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x39), data);
}
static __inline hi_u8 hi_ext_system_drc_detail_bright_min_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x39));
}
/* register: hi_ext_system_drc_detail_bright_max */
/* detail_bright_max */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_detail_bright_max_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x3a), data);
}
static __inline hi_u8 hi_ext_system_drc_detail_bright_max_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x3a));
}
/* register: hi_ext_system_drc_detail_bright_slo */
/* detail_bright_slo */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_detail_bright_slo_write(hi_vi_pipe vi_pipe, hi_s8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x3b), data);
}
static __inline hi_s8 hi_ext_system_drc_detail_bright_slo_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x3b));
}
/* register: hi_ext_system_drc_detail_bright_thr */
/* detail_bright_thr */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_detail_bright_thr_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x3c), data);
}
static __inline hi_u8 hi_ext_system_drc_detail_bright_thr_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x3c));
}

static __inline hi_void hi_ext_system_drc_coef_update_en_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x3d), data);
}
static __inline hi_u8 hi_ext_system_drc_coef_update_en_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x3d));
}

static __inline hi_void hi_ext_system_drc_lut_update_en_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x3e), data);
}
static __inline hi_u8 hi_ext_system_drc_lut_update_en_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x3e));
}

/* register: hi_ext_system_drc_manual_strength */
/* manual setting for DRC strength (for manual DRC) */
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_drc_manual_strength_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_drc_base(vi_pipe) + 0x40, data);
}
static __inline hi_u16 hi_ext_system_drc_manual_strength_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_drc_base(vi_pipe) + 0x40);
}

/* register: hi_ext_system_drc_auto_strength */
/* auto setting for DRC strength (for auto DRC) */
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_drc_auto_strength_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_drc_base(vi_pipe) + 0x42, data);
}
static __inline hi_u16 hi_ext_system_drc_auto_strength_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_drc_base(vi_pipe) + 0x42);
}

/* register: hi_ext_system_drc_auto_strength_max */
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_drc_auto_strength_max_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_drc_base(vi_pipe) + 0x44), data);
}

static __inline hi_u16 hi_ext_system_drc_auto_strength_max_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_drc_base(vi_pipe) + 0x44));
}

/* register: hi_ext_system_drc_auto_strength_min */
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_drc_auto_strength_min_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_drc_base(vi_pipe) + 0x46), data);
}

static __inline hi_u16 hi_ext_system_drc_auto_strength_min_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_drc_base(vi_pipe) + 0x46));
}

/* register: hi_ext_system_drc_strength_target */
/* strength target of drc moudle */
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_drc_strength_target_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_drc_base(vi_pipe) + 0x48), data);
}
static __inline hi_u16 hi_ext_system_drc_strength_target_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_drc_base(vi_pipe) + 0x48));
}

/* register: hi_ext_system_drc_fs_ada_max */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_fs_ada_max_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x4a), data);
}
static __inline hi_u8 hi_ext_system_drc_fs_ada_max_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x4a));
}

/* register: hi_ext_system_drc_diff_thr_low */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_diff_thr_low_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x4b), data);
}
static __inline hi_u8 hi_ext_system_drc_diff_thr_low_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x4b));
}

/* register: hi_ext_system_drc_diff_thr_high */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_diff_thr_high_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x4c), data);
}
static __inline hi_u8 hi_ext_system_drc_diff_thr_high_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x4c));
}

/* register: hi_ext_system_drc_flt_scale_fine */
/* DRC filter scale fine control */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_flt_scale_fine_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x50), data);
}
static __inline hi_u8 hi_ext_system_drc_flt_scale_fine_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x50));
}

/* register: hi_ext_system_drc_flt_scale_coarse */
/* DRC filter scale coarse control */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_flt_scale_coarse_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x51), data);
}
static __inline hi_u8 hi_ext_system_drc_flt_scale_coarse_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x51));
}

/* register: hi_ext_system_drc_suppress_bright_min */
/* DRC detail suppression control */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_suppress_bright_min_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x52), data);
}
static __inline hi_u8 hi_ext_system_drc_suppress_bright_min_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x52));
}

/* register: hi_ext_system_drc_suppress_bright_thr */
/* DRC detail suppression control */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_suppress_bright_thr_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x53), data);
}
static __inline hi_u8 hi_ext_system_drc_suppress_bright_thr_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x53));
}

/* register: hi_ext_system_drc_suppress_bright_slo */
/* DRC detail suppression control */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_suppress_bright_slo_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x54), data);
}
static __inline hi_u8 hi_ext_system_drc_suppress_bright_slo_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x54));
}

/* register: hi_ext_system_drc_suppress_dark_min */
/* DRC detail suppression control */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_suppress_dark_min_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x55), data);
}
static __inline hi_u8 hi_ext_system_drc_suppress_dark_min_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x55));
}

/* register: hi_ext_system_drc_suppress_dark_thr */
/* DRC detail suppression control */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_suppress_dark_thr_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x56), data);
}
static __inline hi_u8 hi_ext_system_drc_suppress_dark_thr_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x56));
}

/* register: hi_ext_system_drc_suppress_dark_slo */
/* DRC detail suppression control */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_suppress_dark_slo_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x57), data);
}
static __inline hi_u8 hi_ext_system_drc_suppress_dark_slo_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x57));
}

/* register: hi_ext_system_drc_dp_detect_rb2rb */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_dp_detect_rb2rb_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x58), data);
}
static __inline hi_u8 hi_ext_system_drc_dp_detect_rb2rb_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x58));
}

/* register: hi_ext_system_drc_dp_detect_g2rb */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_dp_detect_g2rb_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x59), data);
}
static __inline hi_u8 hi_ext_system_drc_dp_detect_g2rb_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x59));
}

/* register: hi_ext_system_drc_dp_detect_repl_ctr */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_dp_detect_repl_ctr_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x5a), data);
}
static __inline hi_u8 hi_ext_system_drc_dp_detect_repl_ctr_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x5a));
}

/* register: hi_ext_system_drc_dp_detect_rng_ratio */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_dp_detect_rng_ratio_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x5b), data);
}
static __inline hi_u8 hi_ext_system_drc_dp_detect_rng_ratio_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x5b));
}

/* register: hi_ext_system_drc_dp_detect_thr_slo */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_dp_detect_thr_slo_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_16direct((vreg_drc_base(vi_pipe) + 0x5c), data);
}
static __inline hi_u8 hi_ext_system_drc_dp_detect_thr_slo_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x5c));
}

/* register: hi_ext_system_drc_dp_detect_thr_min */
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_drc_dp_detect_thr_min_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_drc_base(vi_pipe) + 0x5e), data);
}
static __inline hi_u16 hi_ext_system_drc_dp_detect_thr_min_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_drc_base(vi_pipe) + 0x5e));
}

/* register: hi_ext_system_drc_bright_gain_lmt */
/* bright gain limit */
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_drc_bright_gain_lmt_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_drc_base(vi_pipe) + 0x7e), data);
}
static __inline hi_u16 hi_ext_system_drc_bright_gain_lmt_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_drc_base(vi_pipe) + 0x7e));
}

/* register: hi_ext_system_drc_dark_gain_lmt_c */
/* bright gain limit */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_dark_gain_lmt_c_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x80), data);
}
static __inline hi_u8 hi_ext_system_drc_dark_gain_lmt_c_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x80));
}

/* register: hi_ext_system_drc_dark_gain_lmt_c */
/* bright gain limit */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_dark_gain_lmt_y_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x82), data);
}
static __inline hi_u8 hi_ext_system_drc_dark_gain_lmt_y_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x82));
}

/* register: hi_ext_system_drc_colorcc_lut_read */
/* colorcc lut read len = 33  ~0x11061 */
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_drc_colorcc_lut_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u16 data)
{
    iowr_16direct((vreg_drc_base(vi_pipe) + 0x84  +  index * sizeof(hi_u16)), data);
}
static __inline hi_u16 hi_ext_system_drc_colorcc_lut_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return (iord_16direct((vreg_drc_base(vi_pipe) + 0x84  +  index * sizeof(hi_u16))));
}

/* register: hi_ext_system_drc_cubic_ltmx0 */
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_drc_cubic_ltmx0_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_drc_base(vi_pipe) + 0xce), data);
}
static __inline hi_u16 hi_ext_system_drc_cubic_ltmx0_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_drc_base(vi_pipe) + 0xce));
}
/* register: hi_ext_system_drc_cubic_ltmx1 */
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_drc_cubic_ltmx1_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_drc_base(vi_pipe) + 0xd0), data);
}
static __inline hi_u16 hi_ext_system_drc_cubic_ltmx1_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_drc_base(vi_pipe) + 0xd0));
}
/* register: hi_ext_system_drc_cubic_ltmx2 */
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_drc_cubic_ltmx2_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_drc_base(vi_pipe) + 0xd2), data);
}
static __inline hi_u16 hi_ext_system_drc_cubic_ltmx2_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_drc_base(vi_pipe) + 0xd2));
}
/* register: hi_ext_system_drc_cubic_ltmx3 */
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_drc_cubic_ltmx3_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_drc_base(vi_pipe) + 0xd4), data);
}
static __inline hi_u16 hi_ext_system_drc_cubic_ltmx3_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_drc_base(vi_pipe) + 0xd4));
}
/* register: hi_ext_system_drc_cubic_ltmx4 */
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_drc_cubic_ltmx4_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_drc_base(vi_pipe) + 0xd6), data);
}
static __inline hi_u16 hi_ext_system_drc_cubic_ltmx4_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_drc_base(vi_pipe) + 0xd6));
}
/* register: hi_ext_system_drc_cubic_ltmx5 */
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_drc_cubic_ltmx5_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_drc_base(vi_pipe) + 0xd8), data);
}
static __inline hi_u16 hi_ext_system_drc_cubic_ltmx5_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_drc_base(vi_pipe) + 0xd8));
}
/* register: hi_ext_system_drc_cubic_ltmy0 */
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_drc_cubic_ltmy0_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_drc_base(vi_pipe) + 0xda), data);
}
static __inline hi_u16 hi_ext_system_drc_cubic_ltmy0_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_drc_base(vi_pipe) + 0xda));
}
/* register: hi_ext_system_drc_cubic_ltmy1 */
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_drc_cubic_ltmy1_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_drc_base(vi_pipe) + 0xdc), data);
}
static __inline hi_u16 hi_ext_system_drc_cubic_ltmy1_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_drc_base(vi_pipe) + 0xdc));
}
/* register: hi_ext_system_drc_cubic_ltmy2 */
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_drc_cubic_ltmy2_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_drc_base(vi_pipe) + 0xde), data);
}
static __inline hi_u16 hi_ext_system_drc_cubic_ltmy2_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_drc_base(vi_pipe) + 0xde));
}
/* register: hi_ext_system_drc_cubic_ltmy3 */
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_drc_cubic_ltmy3_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_drc_base(vi_pipe) + 0xe0), data);
}
static __inline hi_u16 hi_ext_system_drc_cubic_ltmy3_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_drc_base(vi_pipe) + 0xe0));
}
/* register: hi_ext_system_drc_cubic_ltmy4 */
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_drc_cubic_ltmy4_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_drc_base(vi_pipe) + 0xe2), data);
}
static __inline hi_u16 hi_ext_system_drc_cubic_ltmy4_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_drc_base(vi_pipe) + 0xe2));
}
/* register: hi_ext_system_drc_cubic_ltmy5 */
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_drc_cubic_ltmy5_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_drc_base(vi_pipe) + 0xe4), data);
}
static __inline hi_u16 hi_ext_system_drc_cubic_ltmy5_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_drc_base(vi_pipe) + 0xe4));
}

/* register: hi_ext_system_drc_cubic_mode */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_cubic_mode_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0xe6), data);
}
static __inline hi_u8 hi_ext_system_drc_cubic_mode_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0xe6));
}

/* register: hi_ext_system_drc_curve_sel */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_curve_sel_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0xe7), data);
}
static __inline hi_u8 hi_ext_system_drc_curve_sel_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0xe7));
}

/* register: hi_ext_system_drc_cubic_slp0 */
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_drc_cubic_slp0_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_drc_base(vi_pipe) + 0xe8), data);
}
static __inline hi_u16 hi_ext_system_drc_cubic_slp0_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_drc_base(vi_pipe) + 0xe8));
}
/* register: hi_ext_system_drc_cubic_slp1 */
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_drc_cubic_slp1_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_drc_base(vi_pipe) + 0xea), data);
}
static __inline hi_u16 hi_ext_system_drc_cubic_slp1_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_drc_base(vi_pipe) + 0xea));
}
/* register: hi_ext_system_drc_cubic_slp2 */
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_drc_cubic_slp2_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_drc_base(vi_pipe) + 0xec), data);
}
static __inline hi_u16 hi_ext_system_drc_cubic_slp2_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_drc_base(vi_pipe) + 0xec));
}
/* register: hi_ext_system_drc_cubic_slp3 */
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_drc_cubic_slp3_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_drc_base(vi_pipe) + 0xee), data);
}
static __inline hi_u16 hi_ext_system_drc_cubic_slp3_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_drc_base(vi_pipe) + 0xee));
}
/* register: hi_ext_system_drc_cubic_slp4 */
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_drc_cubic_slp4_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_drc_base(vi_pipe) + 0xf0), data);
}
static __inline hi_u16 hi_ext_system_drc_cubic_slp4_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_drc_base(vi_pipe) + 0xf0));
}

/* register: hi_ext_system_drc_darkstep */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_detail_darkstep_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0xf2), data);
}
static __inline hi_u8 hi_ext_system_drc_detail_darkstep_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0xf2));
}

/* register: hi_ext_system_drc_brightstep */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_detail_brightstep_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0xf3), data);
}
static __inline hi_u8 hi_ext_system_drc_detail_brightstep_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0xf3));
}

/* register: hi_ext_system_drc_shp_log */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_shp_log_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0xf4), data);
}
static __inline hi_u8 hi_ext_system_drc_shp_log_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0xf4));
}

/* register: hi_ext_system_drc_shp_exp */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_shp_exp_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0xf5), data);
}
static __inline hi_u8 hi_ext_system_drc_shp_exp_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0xf5));
}

/* register: hi_ext_system_drc_flt_spa_fine */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_flt_spa_fine_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0xf6), data);
}
static __inline hi_u8 hi_ext_system_drc_flt_spa_fine_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0xf6));
}

/* register: hi_ext_system_drc_flt_spa_medium */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_flt_spa_medium_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0xf7), data);
}
static __inline hi_u8 hi_ext_system_drc_flt_spa_medium_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0xf7));
}

/* register: hi_ext_system_drc_flt_spa_coarse */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_flt_spa_coarse_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0xf8), data);
}
static __inline hi_u8 hi_ext_system_drc_flt_spa_coarse_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0xf8));
}

/* register: hi_ext_system_drc_flt_rng_fine */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_flt_rng_fine_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0xf9), data);
}
static __inline hi_u8 hi_ext_system_drc_flt_rng_fine_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0xf9));
}

/* register: hi_ext_system_drc_flt_rng_medium */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_flt_rng_medium_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0xfa), data);
}
static __inline hi_u8 hi_ext_system_drc_flt_rng_medium_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0xfa));
}

/* register: hi_ext_system_drc_flt_rng_coarse */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_flt_rng_coarse_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0xfb), data);
}
static __inline hi_u8 hi_ext_system_drc_flt_rng_coarse_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0xfb));
}

/* register: hi_ext_system_drc_flt_rng_ada_max */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_flt_rng_ada_max_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0xfc), data);
}
static __inline hi_u8 hi_ext_system_drc_flt_rng_ada_max_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0xfc));
}

/* register: hi_ext_system_drc_dis_offset_coef */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_dis_offset_coef_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0xfd), data);
}
static __inline hi_u8 hi_ext_system_drc_dis_offset_coef_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0xfd));
}

/* register: hi_ext_system_drc_dis_thr_coef_low */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_dis_thr_coef_low_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0xfe), data);
}
static __inline hi_u8 hi_ext_system_drc_dis_thr_coef_low_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0xfe));
}

/* register: hi_ext_system_drc_dis_thr_coef_high */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_dis_thr_coef_high_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0xff), data);
}
static __inline hi_u8 hi_ext_system_drc_dis_thr_coef_high_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0xff));
}

/* register: hi_ext_system_drc_detail_sub_factor */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_detail_sub_factor_write(hi_vi_pipe vi_pipe, hi_s8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x100), data);
}
static __inline hi_s8 hi_ext_system_drc_detail_sub_factor_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x100));
}

/* register: hi_ext_system_drc_wgt_box_tri_sel */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_wgt_box_tri_sel_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x101), data);
}
static __inline hi_u8 hi_ext_system_drc_wgt_box_tri_sel_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x101));
}

/* register: hi_ext_system_drc_color_corr_enable */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_color_corr_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x102), data);
}
static __inline hi_u8 hi_ext_system_drc_color_corr_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x102));
}

/* register: hi_ext_system_drc_detail_boost_enable */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_detail_boost_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x103), data);
}
static __inline hi_u8 hi_ext_system_drc_detail_boost_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x103));
}

/* register: hi_ext_system_drc_pdw_sum_enable */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_drc_pdw_sum_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_drc_base(vi_pipe) + 0x104), data);
}
static __inline hi_u8 hi_ext_system_drc_pdw_sum_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_drc_base(vi_pipe) + 0x104));
}

/* register: hi_ext_system_drc_tm_lut_write len = 200 */
/* args: data (16-bit) */
/* 0x107 ~ 0x296 */
static __inline hi_void hi_ext_system_drc_tm_lut_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u16 data)
{
    iowr_16direct((vreg_drc_base(vi_pipe) + 0x106 + index * sizeof(hi_u16)), data);
}

static __inline hi_u16 hi_ext_system_drc_tm_lut_read(hi_vi_pipe vi_pipe, hi_u8 index)
{
    return iord_16direct((vreg_drc_base(vi_pipe) + 0x106 + index * sizeof(hi_u16)));
}

/* ============================================================== */
/* dehaze */
/* ============================================================== */
/* register: hi_ext_system_dehaze_manu_mode */
/* enables manual dehaze: 0=auto 1=manual */
#define HI_EXT_SYSTEM_DEHAZE_MANU_MODE_DEFAULT  0
/* args: data (8-bit) */
static __inline void hi_ext_system_dehaze_manu_mode_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dehaze_base(vi_pipe), data);
}
static __inline hi_u32 hi_ext_system_dehaze_manu_mode_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dehaze_base(vi_pipe));
}

/* register: hi_ext_system_manual_dehaze_hblk */
/* manual dehaze hblk number */
/* args: data (8-bit) */
static __inline void hi_ext_system_manual_dehaze_hblk_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dehaze_base(vi_pipe) + 0x1, data);
}
static __inline hi_u8 hi_ext_system_manual_dehaze_hblk_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dehaze_base(vi_pipe) + 0x1);
}

/* register: hi_ext_system_manual_dehaze_vblk */
/* manual dehaze vblk number */
/* args: data (8-bit) */
static __inline void hi_ext_system_manual_dehaze_vblk_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dehaze_base(vi_pipe) + 0x2, data);
}

static __inline hi_u8 hi_ext_system_manual_dehaze_vblk_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dehaze_base(vi_pipe) + 0x2);
}

/* register: hi_ext_system_dehaze_ctp */
/* manual dehaze curve turn point */
#define HI_EXT_SYSTEM_DEHAZE_CTP_DEFAULT  0x80

/* args: data (8-bit) */
static __inline void hi_ext_system_dehaze_ctp_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dehaze_base(vi_pipe) + 0x3, data);
}
static __inline hi_u8 hi_ext_system_dehaze_ctp_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dehaze_base(vi_pipe) + 0x3);
}

/* register: hi_ext_system_manual_dehaze_mft */
/* manual dehaze minumum filter threshold */
#define HI_EXT_SYSTEM_DEHAZE_MFT_DEFAULT 0x200

/* args: data (10-bit) */
static __inline void hi_ext_system_dehaze_mft_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_dehaze_base(vi_pipe) + 0x4, data);
}
static __inline hi_u16 hi_ext_system_dehaze_mft_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_dehaze_base(vi_pipe) + 0x4) & 0x3ff);
}

/* register: hi_ext_system_manual_dehaze_mfs */
/* manual dehaze minumum filter size */
#define HI_EXT_SYSTEM_DEHAZE_MFS_DEFAULT 0x7
/* args: data (8-bit) */
static __inline void hi_ext_system_dehaze_mfs_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dehaze_base(vi_pipe) + 0x6, data);
}
static __inline hi_u8 hi_ext_system_dehaze_mfs_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dehaze_base(vi_pipe) + 0x6);
}

/* register: hi_ext_system_manual_dehaze_ct */
/* manual dehaze change threshold */
#define HI_EXT_SYSTEM_DEHAZE_CT_DEFAULT 255
/* args: data (8-bit) */
static __inline void hi_ext_system_dehaze_ct_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dehaze_base(vi_pipe) + 0x7, data);
}
static __inline hi_u8 hi_ext_system_dehaze_ct_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dehaze_base(vi_pipe) + 0x7);
}

/* register: hi_ext_system_manual_dehaze_tfic */
/* manual dehaze temper filter increase ceofficient */
#define HI_EXT_SYSTEM_DEHAZE_TFIC_DEFAULT  0x8
/* args: data (16-bit) */
static __inline void hi_ext_system_dehaze_tfic_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_dehaze_base(vi_pipe) + 0x8, data);
}
static __inline hi_u16 hi_ext_system_dehaze_tfic_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_dehaze_base(vi_pipe) + 0x8);
}

/* register: hi_ext_system_manual_dehaze_tfdc */
/* manual dehaze temper filter decrease ceofficient */
#define HI_EXT_SYSTEM_DEHAZE_TFDC_DEFAULT  64
/* args: data (16-bit) */
static __inline void hi_ext_system_dehaze_tfdc_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_dehaze_base(vi_pipe) + 0xa, data);
}
static __inline hi_u16 hi_ext_system_dehaze_tfdc_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_dehaze_base(vi_pipe) + 0xa);
}

/* register: hi_ext_system_dehaze_debug_enable */
/* dehaze debug enable */
#define HI_EXT_SYSTEM_DEHAZE_DEBUGFILE_ENABLE_DEFAULT  0

/* args: data (8-bit) */
static __inline void hi_ext_system_dehaze_debugfile_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dehaze_base(vi_pipe) + 0xc, data);
}
static __inline hi_u8 hi_ext_system_dehaze_debugfile_enable_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dehaze_base(vi_pipe) + 0xc);
}

/* register: hi_ext_system_manual_dehaze_strength */
/* dehaze strength */
#define HI_EXT_SYSTEM_MANUAL_DEHAZE_STRENGTH_DEFAULT  0x80

/* args: data (8-bit) */
static __inline void hi_ext_system_manual_dehaze_strength_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dehaze_base(vi_pipe) + 0xd, data);
}
static __inline hi_u8 hi_ext_system_manual_dehaze_strength_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dehaze_base(vi_pipe) + 0xd);
}

/* register: isp_ext_system_manual_dehaze_autostrength */
/* dehaze strength */
#define HI_EXT_SYSTEM_MANUAL_DEHAZE_AUTOSTRENGTH_DEFAULT  0x80
/* args: data (8-bit) */
static __inline void hi_ext_system_manual_dehaze_autostrength_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dehaze_base(vi_pipe) + 0xe, data);
}
static __inline hi_u8 hi_ext_system_manual_dehaze_autostrength_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dehaze_base(vi_pipe) + 0xe);
}

#define HI_EXT_SYSTEM_DEHAZE_ENABLE_DEFAULT  0x0

/* args: data (8-bit) */
static __inline void hi_ext_system_dehaze_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dehaze_base(vi_pipe) + 0x10, data);
}
static __inline hi_u8 hi_ext_system_dehaze_enable_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dehaze_base(vi_pipe) + 0x10);
}

/* args: data (1-bit) */
#define HI_EXT_SYSTEM_DEHAZE_DEBUG_ENABLE_DEFAULT 0x0
static __inline hi_void hi_ext_system_dehaze_debug_enable_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_dehaze_base(vi_pipe) + 0x14, data & 0x1);
}
static __inline hi_u16 hi_ext_system_dehaze_debug_enable_read(hi_vi_pipe vi_pipe)
{
    return (hi_u16)(iord_16direct(vreg_dehaze_base(vi_pipe) + 0x14) & 0x1);
}

/* register: hi_ext_system_dehaze_debug_addr */
/* address of dehaze debug information */
/* args: data (32-bit) */
static __inline hi_void hi_ext_system_dehaze_debug_high_addr_write(hi_vi_pipe vi_pipe, hi_u32 data)
{
    iowr_32direct(vreg_dehaze_base(vi_pipe) + 0x18, data);
}

static __inline hi_u32 hi_ext_system_dehaze_debug_high_addr_read(hi_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_dehaze_base(vi_pipe) + 0x18);
}

/* args: data (32-bit) */
static __inline hi_void hi_ext_system_dehaze_debug_low_addr_write(hi_vi_pipe vi_pipe, hi_u32 data)
{
    iowr_32direct(vreg_dehaze_base(vi_pipe) + 0x1c, data);
}

static __inline hi_u32 hi_ext_system_dehaze_debug_low_addr_read(hi_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_dehaze_base(vi_pipe) + 0x1c);
}

/* register: hi_ext_system_dehaze_debug_size */
/* size of dehaze debug struct */
/* args: data (32-bit) */
static __inline hi_void hi_ext_system_dehaze_debug_size_write(hi_vi_pipe vi_pipe, hi_u32 data)
{
    iowr_32direct(vreg_dehaze_base(vi_pipe) + 0x20, data);
}

static __inline hi_u32 hi_ext_system_dehaze_debug_size_read(hi_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_dehaze_base(vi_pipe) + 0x20);
}

/* register: hi_ext_system_dehaze_debug_depth */
/* depth of dehaze debug struct */
/* args: data (32-bit) */
static __inline hi_void hi_ext_system_dehaze_debug_depth_write(hi_vi_pipe vi_pipe, hi_u32 data)
{
    iowr_32direct(vreg_dehaze_base(vi_pipe) + 0x24, data);
}

static __inline hi_u32 hi_ext_system_dehaze_debug_depth_read(hi_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_dehaze_base(vi_pipe) + 0x24);
}

/* register: hi_ext_system_dehaze_debug_quit */
/* depth of dehaze debug struct */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_dehaze_debug_quit_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dehaze_base(vi_pipe) + 0x28, data);
}

static __inline hi_u8 hi_ext_system_dehaze_debug_quit_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dehaze_base(vi_pipe) + 0x28);
}

/* register: hi_ext_system_dehaze_debug_complete */
/* depth of dehaze debug struct */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_dehaze_debug_complete_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dehaze_base(vi_pipe) + 0x29, data);
}

static __inline hi_u8 hi_ext_system_dehaze_debug_complete_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dehaze_base(vi_pipe) + 0x29);
}

/* register: hi_ext_system_defog_lut_write len = 256 */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_dehaze_lut_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u8 data)
{
    iowr_8direct(vreg_dehaze_base(vi_pipe) + 0x30 + (index * sizeof(hi_u8)), data);
}

static __inline hi_u8 hi_ext_system_dehaze_lut_read(hi_vi_pipe vi_pipe, hi_u8 index)
{
    return iord_8direct(vreg_dehaze_base(vi_pipe) + 0x30 + (index * sizeof(hi_u8)));
}

#define HI_EXT_SYSTEM_USER_DEHAZE_LUT_ENABLE_DEFAULT  0
static __inline hi_void hi_ext_system_user_dehaze_lut_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dehaze_base(vi_pipe) + 0x140, data);
}
static __inline hi_u8 hi_ext_system_user_dehaze_lut_enable_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dehaze_base(vi_pipe) + 0x140);
}

#define HI_EXT_SYSTEM_USER_DEHAZE_LUT_UPDATE_DEFAULT  0
static __inline hi_void hi_ext_system_user_dehaze_lut_update_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dehaze_base(vi_pipe) + 0x142, data);
}
static __inline hi_u8 hi_ext_system_user_dehaze_lut_update_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dehaze_base(vi_pipe) + 0x142);
}

/* register: hi_ext_system_manual_fpn_strength */
#define HI_EXT_SYSTEM_FPN_STRENGTH_DEFAULT  0
/* args: data (16-bit) */
/* register: hi_ext_system_manual_fpn_opmode */
#define HI_EXT_SYSTEM_FPN_OPMODE_DEFAULT  0x0
/* args: data (8-bit) */
static __inline void hi_ext_system_manual_fpn_opmode_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_fpn_base(vi_pipe) + 0x2, data);
}

static __inline hi_u8 hi_ext_system_manual_fpn_opmode_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_fpn_base(vi_pipe) + 0x2);
}

/* register: isp_ext_system_manual_fpn_update */
#define HI_EXT_SYSTEM_FPN_MANU_UPDATE_DEFAULT  0x0
/* args: data (8-bit) */
static __inline void hi_ext_system_manual_fpn_update_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_fpn_base(vi_pipe) + 0x3, data);
}

static __inline hi_u8 hi_ext_system_manual_fpn_update_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_fpn_base(vi_pipe) + 0x3);
}

/* register: hi_ext_system_manual_fpn_corr_cfg */
#define HI_EXT_SYSTEM_FPN_MANU_ISO_DEFAULT  6375
/* args: data (32-bit) */
static __inline void hi_ext_system_manual_fpn_iso_write(hi_vi_pipe vi_pipe, hi_u32 data)
{
    iowr_32direct(vreg_fpn_base(vi_pipe) + 0x4, data);
}

static __inline hi_u32 hi_ext_system_manual_fpn_iso_read(hi_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_fpn_base(vi_pipe) + 0x4);
}

/* register: hi_ext_system_sensor_iso */
/* sensor ISO */
#define HI_EXT_SYSTEM_FPN_SENSOR_ISO_DEFAULT  0x0

/* args: data (32-bit) */
static __inline hi_void hi_ext_system_fpn_sensor_iso_write(hi_vi_pipe vi_pipe, hi_u32 data)
{
    iowr_32direct(vreg_fpn_base(vi_pipe) + 0x8, data);
}

static __inline hi_u32 hi_ext_system_fpn_sensor_iso_read(hi_vi_pipe vi_pipe)
{
    return  iord_32direct(vreg_fpn_base(vi_pipe) + 0x8);
}

/* register: isp_ext_system_manual_fpn_corr_cfg */
#define HI_EXT_SYSTEM_FPN_MANU_CORRCFG_DEFAULT  0x0
/* args: data (8-bit) */
static __inline void hi_ext_system_manual_fpn_corr_cfg_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_fpn_base(vi_pipe) + 0xc, data);
}

static __inline hi_u8 isp_ext_system_manual_fpn_corr_cfg_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_fpn_base(vi_pipe) + 0xc);
}

static __inline void hi_ext_system_manual_fpn_type_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_fpn_base(vi_pipe) + 0xd, data);
}

static __inline hi_u8 isp_ext_system_manual_fpn_type_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_fpn_base(vi_pipe) + 0xd);
}

static __inline void hi_ext_system_manual_fpn_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_fpn_base(vi_pipe) + 0xe, data);
}

static __inline hi_u8 isp_ext_system_manual_fpn_enable_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_fpn_base(vi_pipe) + 0xe);
}

static __inline void hi_ext_system_manual_fpn_pixel_format_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_fpn_base(vi_pipe) + 0xf, data);
}

static __inline hi_u8 isp_ext_system_manual_fpn_pixel_format_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_fpn_base(vi_pipe) + 0xf);
}

static __inline void hi_ext_system_manual_fpn_gain_write(hi_vi_pipe vi_pipe, hi_u32 data)
{
    iowr_32direct(vreg_fpn_base(vi_pipe) + 0x10, data);
}

static __inline hi_u32 isp_ext_system_manual_fpn_gain_read(hi_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_fpn_base(vi_pipe) + 0x10);
}

static __inline void hi_ext_system_manual_fpn_offset_write(hi_vi_pipe vi_pipe, hi_u32 data)
{
    iowr_32direct(vreg_fpn_base(vi_pipe) + 0x14, data);
}

static __inline hi_u32 isp_ext_system_manual_fpn_offset_read(hi_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_fpn_base(vi_pipe) + 0x14);
}

#define HI_EXT_SYSTEM_FPN_SENSOR_ISO_DEFAULT  0x0

/* args: data (8-bit) */
static __inline hi_void hi_ext_system_fpn_cablibrate_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_fpn_base(vi_pipe) + 0x19, data);
}

static __inline hi_u8 hi_ext_system_fpn_cablibrate_enable_read(hi_vi_pipe vi_pipe)
{
    return  iord_8direct(vreg_fpn_base(vi_pipe) + 0x19);
}

/* BLACK LEVEL */
/* register: hi_ext_system_black_level_00 */
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_black_level_00_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_blc_base(vi_pipe), data);
}

static __inline hi_u16 hi_ext_system_black_level_00_read(hi_vi_pipe vi_pipe)
{
    return  iord_16direct(vreg_blc_base(vi_pipe));
}

/* register: hi_ext_system_black_level_01 */
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_black_level_01_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_blc_base(vi_pipe) + 0x2, data);
}

static __inline hi_u16 hi_ext_system_black_level_01_read(hi_vi_pipe vi_pipe)
{
    return  iord_16direct(vreg_blc_base(vi_pipe) + 0x2);
}

/* register: hi_ext_system_black_level_10 */
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_black_level_10_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_blc_base(vi_pipe) + 0x4, data);
}

static __inline hi_u16 hi_ext_system_black_level_10_read(hi_vi_pipe vi_pipe)
{
    return  iord_16direct(vreg_blc_base(vi_pipe) + 0x4);
}

/* register: hi_ext_system_black_level_11 */
/* ISP black level 11 */
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_black_level_11_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_blc_base(vi_pipe) + 0x6, data);
}

static __inline hi_u16 hi_ext_system_black_level_11_read(hi_vi_pipe vi_pipe)
{
    return  iord_16direct(vreg_blc_base(vi_pipe) + 0x6);
}

/* register: hi_ext_system_black_level_change */
/* ISP black level change */
#define HI_EXT_SYSTEM_BLACK_LEVEL_CHANGE_DEFAULT  0x0
/* args: data (1-bit) */
static __inline hi_void hi_ext_system_black_level_change_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_blc_base(vi_pipe) + 0x8, data);
}

static __inline hi_u8 hi_ext_system_black_level_change_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_blc_base(vi_pipe) + 0x8);
}

/* register: hi_ext_system_black_level_manual_mode */
/* args: data (8-bit) */
#define HI_EXT_SYSTEM_BLACK_LEVEL_MANUAL_MODE_DEFAULT 0
static __inline hi_void hi_ext_system_black_level_manual_mode_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_blc_base(vi_pipe) + 0xa, data & 0x1);
}
static __inline hi_u8 hi_ext_system_black_level_manual_mode_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_blc_base(vi_pipe) + 0xa) & 0x1);
}

/* AE */
/* register: hi_ext_system_ae_sel */
/* AE sel */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_ae_be_sel_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe)), data);
}
static __inline hi_u8 hi_ext_system_ae_be_sel_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_ae_base(vi_pipe));
}

/* register: hi_ext_system_ae_hist_offset_x */
/* AE hist offset X */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_ae_hist_offset_x_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0x1), data);
}
static __inline hi_u8 hi_ext_system_ae_hist_offset_x_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0x1);
}

/* register: hi_ext_system_ae_hist_offset_y */
/* AE hist offset Y */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_ae_hist_offset_y_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0x2), data);
}
static __inline hi_u8 hi_ext_system_ae_hist_offset_y_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0x2);
}

/* register: hi_ext_system_ae_hist_skip_x */
/* AE hist skip X */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_ae_hist_skip_x_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0x3), data);
}
static __inline hi_u8 hi_ext_system_ae_hist_skip_x_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0x3);
}

/* register: hi_ext_system_ae_hist_skip_y */
/* AE hist skip Y */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_ae_hist_skip_y_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0x4), data);
}
static __inline hi_u8 hi_ext_system_ae_hist_skip_y_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0x4);
}

/* register: hi_ext_system_ae_fourplanemode */
/* AE fourplanemode */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_ae_fourplanemode_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0x5), data);
}
static __inline hi_u8 hi_ext_system_ae_fourplanemode_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0x5);
}

/* register: hi_ext_system_ae_histmode */
/* AE histmode */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_ae_histmode_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0x6), data);
}
static __inline hi_u8 hi_ext_system_ae_histmode_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0x6);
}

/* register: hi_ext_system_ae_avermode */
/* AE avermode */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_ae_avermode_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0x7), data);
}
static __inline hi_u8 hi_ext_system_ae_avermode_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0x7);
}

/* register: hi_ext_system_ae_maxgainmode */
/* AE maxgainmode */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_ae_maxgainmode_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0x8), data);
}
static __inline hi_u8 hi_ext_system_ae_maxgainmode_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0x8);
}

/* register: hi_ext_system_ae_fe_en */
/* AE FE enable */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_ae_fe_en_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0xc), data);
}
static __inline hi_u8 hi_ext_system_ae_fe_en_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0xc);
}

/* register: hi_ext_system_ae_be_en */
/* AE BE enable */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_ae_be_en_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0xd), data);
}
static __inline hi_u8 hi_ext_system_ae_be_en_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0xd);
}

/* register: hi_ext_system_mg_en */
/* MG enable */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_mg_en_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0xe), data);
}
static __inline hi_u8 hi_ext_system_mg_en_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0xe);
}

/* register: hi_ext_system_crop_en */
/* CROP enable */
#define HI_EXT_SYSTEM_CROP_EN_DEFAULT 0x0

/* args: data (8-bit) */
static __inline hi_void hi_ext_system_ae_crop_en_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0xf), data);
}
static __inline hi_u8 hi_ext_system_ae_crop_en_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0xf);
}

/* register: hi_ext_system_ae_crop_x */
/* AE CROP X */
#define HI_EXT_SYSTEM_AE_CROP_X_DEFAULT 0x0

/* args: data (16-bit) */
static __inline hi_void hi_ext_system_ae_crop_x_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_ae_base(vi_pipe) + 0x10), data);
}
static __inline hi_u32 hi_ext_system_ae_crop_x_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_ae_base(vi_pipe) + 0x10);
}

/* register: hi_ext_system_ae_crop_y */
/* AE CROP Y */
#define HI_EXT_SYSTEM_AE_CROP_Y_DEFAULT 0x0

/* args: data (16-bit) */
static __inline hi_void hi_ext_system_ae_crop_y_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_ae_base(vi_pipe) + 0x14), data);
}
static __inline hi_u16 hi_ext_system_ae_crop_y_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_ae_base(vi_pipe) + 0x14);
}

/* register: hi_ext_system_ae_crop_height */
/* AE CROP HEIGHT */
#define HI_EXT_SYSTEM_AE_CROP_HEIGHT_DEFAULT 0x0

/* args: data (16-bit) */
static __inline hi_void hi_ext_system_ae_crop_height_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_ae_base(vi_pipe) + 0x18), data);
}
static __inline hi_u16 hi_ext_system_ae_crop_height_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_ae_base(vi_pipe) + 0x18);
}

/* register: hi_ext_system_ae_crop_width */
/* AE CROP WIDTH */
#define HI_EXT_SYSTEM_AE_CROP_WIDTH_DEFAULT 0x0

/* args: data (16-bit) */
static __inline hi_void hi_ext_system_ae_crop_width_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_ae_base(vi_pipe) + 0x1c), data);
}
static __inline hi_u16 hi_ext_system_ae_crop_width_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_ae_base(vi_pipe) + 0x1c);
}

/* register: hi_ext_system_smart_enable */
/* smart enable */
#define HI_EXT_SYSTEM_SMART_ENABLE_DEFAULT 0x0

/* args: data (8-bit) */
static __inline hi_void hi_ext_system_smart_enable_write(hi_vi_pipe vi_pipe, hi_u16 index, hi_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0x20 + index), data);
}
static __inline hi_u8 hi_ext_system_smart_enable_read(hi_vi_pipe vi_pipe, hi_u16 index)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0x20 + index);
}

/* register: hi_ext_system_smart_available */
/* smart available */
#define HI_EXT_SYSTEM_SMART_AVAILABLE_DEFAULT 0x0

/* args: data (8-bit) */
static __inline hi_void hi_ext_system_smart_available_write(hi_vi_pipe vi_pipe, hi_u16 index, hi_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0x30 + index), data);
}
static __inline hi_u8 hi_ext_system_smart_available_read(hi_vi_pipe vi_pipe, hi_u16 index)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0x30 + index);
}

/* register: hi_ext_system_smart_luma */
/* smart luma */
#define HI_EXT_SYSTEM_SMART_LUMA_DEFAULT 0x0

/* args: data (8-bit) */
static __inline hi_void hi_ext_system_smart_luma_write(hi_vi_pipe vi_pipe, hi_u16 index, hi_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0x40 + index), data);
}
static __inline hi_u8 hi_ext_system_smart_luma_read(hi_vi_pipe vi_pipe, hi_u16 index)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0x40 + index);
}

/* register: hi_ext_system_smart_update */
/* smart update */
#define HI_EXT_SYSTEM_SMART_UPDATE_DEFAULT 0x0

/* args: data (8-bit) */
static __inline hi_void hi_ext_system_smart_update_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0x50), data);
}
static __inline hi_u8 hi_ext_system_smart_update_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0x50);
}

/* ------------------------------------------------------------------------------  */
/* register: hi_ext_system_tunnel_enable */
/* ------------------------------------------------------------------------------  */
/* tunnel enable */
/* ------------------------------------------------------------------------------  */
#define HI_EXT_SYSTEM_TUNNEL_ENABLE_DEFAULT 0x0

/* args: data (8-bit) */
static __inline hi_void hi_ext_system_tunnel_enable_write(hi_vi_pipe vi_pipe, hi_u16 index, hi_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0x60 + index), data);
}
static __inline hi_u8 hi_ext_system_tunnel_enable_read(hi_vi_pipe vi_pipe, hi_u16 index)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0x60 + index);
}

/* ------------------------------------------------------------------------------  */
/* register: hi_ext_system_tunnel_available */
/* ------------------------------------------------------------------------------  */
/* tunnel available */
/* ------------------------------------------------------------------------------  */
#define HI_EXT_SYSTEM_TUNNEL_AVAILABLE_DEFAULT 0x0

/* args: data (8-bit) */
static __inline hi_void hi_ext_system_tunnel_available_write(hi_vi_pipe vi_pipe, hi_u16 index, hi_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0x64 + index), data);
}
static __inline hi_u8 hi_ext_system_tunnel_available_read(hi_vi_pipe vi_pipe, hi_u16 index)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0x64 + index);
}

/* ------------------------------------------------------------------------------  */
/* register: hi_ext_system_tunnel_pix_ratio */
/* ------------------------------------------------------------------------------  */
/* tunnel pix ratio */
/* ------------------------------------------------------------------------------  */
#define HI_EXT_SYSTEM_TUNNEL_PIX_RATIO_DEFAULT 0x0

/* args: data (32-bit) */
static __inline hi_void hi_ext_system_tunnel_area_ratio_write(hi_vi_pipe vi_pipe, hi_u16 index, hi_u32 data)
{
    iowr_32direct((vreg_ae_base(vi_pipe) + 0x70 + (index << 0x2)), data);
}
static __inline hi_u32 hi_ext_system_tunnel_area_ratio_read(hi_vi_pipe vi_pipe, hi_u16 index)
{
    return iord_32direct(vreg_ae_base(vi_pipe) + 0x70 + (index << 0x2));
}

/* ------------------------------------------------------------------------------  */
/* register: hi_ext_system_tunnel_value */
/* ------------------------------------------------------------------------------  */
/* tunnel value */
/* ------------------------------------------------------------------------------  */
#define HI_EXT_SYSTEM_TUNNEL_VALUE_DEFAULT 0x0

/* args: data (32-bit) */
static __inline hi_void hi_ext_system_tunnel_exp_perf_write(hi_vi_pipe vi_pipe, hi_u16 index, hi_u32 data)
{
    iowr_32direct((vreg_ae_base(vi_pipe) + 0x80 + (index << 0x2)), data);
}
static __inline hi_u32 hi_ext_system_tunnel_exp_perf_read(hi_vi_pipe vi_pipe, hi_u16 index)
{
    return iord_32direct(vreg_ae_base(vi_pipe) + 0x80 + (index << 0x2));
}

/* ------------------------------------------------------------------------------  */
/* register: hi_ext_system_tunnel_update */
/* ------------------------------------------------------------------------------  */
/* tunnel update */
/* ------------------------------------------------------------------------------  */
#define HI_EXT_SYSTEM_TUNNEL_UPDATE_DEFAULT 0x0

/* args: data (8-bit) */
static __inline hi_void hi_ext_system_tunnel_update_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0x90), data);
}
static __inline hi_u8 hi_ext_system_tunnel_update_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0x90);
}
/* register: hi_ext_system_ae_weight_table */
/* AE weight table */
#define HI_EXT_SYSTEM_AE_WEIGHT_TABLE_DEFAULT 0x0

/* args: data (8-bit) */
static __inline hi_void hi_ext_system_ae_weight_table_write(hi_vi_pipe vi_pipe, hi_u16 index, hi_u8 data)
{
    iowr_8direct((vreg_ae_base(vi_pipe) + 0x100 + index), data);
}
static __inline hi_u8 hi_ext_system_ae_weight_table_read(hi_vi_pipe vi_pipe, hi_u16 index)
{
    return iord_8direct(vreg_ae_base(vi_pipe) + 0x100 + index);
}

/* DPC */
/* register: hi_ext_system_dpc_static_cor_enable */
/* defect pixel static correction enable */
#define HI_EXT_SYSTEM_DPC_STATIC_COR_ENABLE_DEFAULT 0x01

/* dpc_static_cor_enable */
static __inline hi_void hi_ext_system_dpc_static_cor_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dpc_base(vi_pipe), data & 0x1);
}
static __inline hi_u8 hi_ext_system_dpc_static_cor_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_dpc_base(vi_pipe)) & 0x1);
}

/* register: hi_ext_system_dpc_static_calib_enable */
/* defect pixel static correction enable */
#define HI_EXT_SYSTEM_DPC_STATIC_CALIB_ENABLE_DEFAULT 0x0

/* dpc_static_calib_enable */
static __inline hi_void hi_ext_system_dpc_static_calib_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dpc_base(vi_pipe)  +  1, data & 0x1);
}
static __inline hi_u8 hi_ext_system_dpc_static_calib_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_dpc_base(vi_pipe)  +  1) & 0x1);
}

/* register: hi_ext_system_dpc_defect_type */
/* ISP_STATIC_DP_BRIGHT:0x0,ISP_STATIC_DP_DARK:0x1 */
#define HI_EXT_SYSTEM_DPC_STATIC_DEFECT_TYPE_DEFAULT 0x0

/* static_dp_type */
static __inline hi_void hi_ext_system_dpc_static_defect_type_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dpc_base(vi_pipe)  +  0x2, data & 0x1);
}
static __inline hi_u8 hi_ext_system_dpc_static_defect_type_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_dpc_base(vi_pipe)  +  0x2) & 0x1);
}

/* register: hi_ext_system_dpc_start_thresh */
/* define the start thresh of defect pixel calibration */
#define HI_EXT_SYSTEM_DPC_START_THRESH_DEFAULT 0x3

/* start_thresh */
static __inline hi_void hi_ext_system_dpc_start_thresh_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_dpc_base(vi_pipe)  +  0x3), data);
}
static __inline hi_u8 hi_ext_system_dpc_start_thresh_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dpc_base(vi_pipe)  +  0x3);
}

/* register: hi_ext_system_dpc_finish_thresh */
/* the finish thresh of defect pixel calibration */
/* finish_thresh */
static __inline hi_void hi_ext_system_dpc_finish_thresh_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dpc_base(vi_pipe)  +  0x4, data);
}
static __inline hi_u8 hi_ext_system_dpc_finish_thresh_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dpc_base(vi_pipe)  +  0x4);
}

/* register: hi_ext_system_dpc_trigger_status */
/* the static bad pixel trigger status */
/* 0:initial status */
/* 1:finished */
/* 2:time out */
#define HI_EXT_SYSTEM_DPC_TRIGGER_STATUS_DEFAULT 0x0

/* status */
static __inline hi_void hi_ext_system_dpc_trigger_status_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dpc_base(vi_pipe)  +  0x5, data);
}
static __inline hi_u8 hi_ext_system_dpc_trigger_status_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dpc_base(vi_pipe)  +  0x5);
}

/* register: hi_ext_system_dpc_static_dp_show */
/* highlight static defect pixel */
#define HI_EXT_SYSTEM_DPC_STATIC_DP_SHOW_DEFAULT 0x0

/* show */
static __inline hi_void hi_ext_system_dpc_static_dp_show_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dpc_base(vi_pipe)  +  0x6, data & 0x1);
}
static __inline hi_u8 hi_ext_system_dpc_static_dp_show_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_dpc_base(vi_pipe)  +  0x6) & 0x1);
}

/* register: hi_ext_system_dpc_dynamic_cor_enable */
/* defect pixel dynamic correction enable */
#define HI_EXT_SYSTEM_DPC_DYNAMIC_COR_ENABLE_DEFAULT 0x01

/* dynamic_cor_enable */
static __inline hi_void hi_ext_system_dpc_dynamic_cor_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dpc_base(vi_pipe)  +  0x7, data & 0x1);
}
static __inline hi_u8 hi_ext_system_dpc_dynamic_cor_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_dpc_base(vi_pipe)  +  0x7) & 0x1);
}

/* register: hi_ext_system_dpc_count_max */
/* bad pixel calibration  the maxmum bad pixel count */
#define HI_EXT_SYSTEM_DPC_COUNT_MAX_DEFAULT 0x400

/* count_max */
static __inline hi_void hi_ext_system_dpc_count_max_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_dpc_base(vi_pipe)  +  0x8, data);
}
static __inline hi_u16 hi_ext_system_dpc_count_max_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_dpc_base(vi_pipe)  +  0x8);
}

/* register: hi_ext_system_dpc_count_min */
/* bad pixel calibration  the minmum bad pixel count */
#define HI_EXT_SYSTEM_DPC_COUNT_MIN_DEFAULT 0x1

/* count_min */
static __inline hi_void hi_ext_system_dpc_count_min_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_dpc_base(vi_pipe)  +  0xa, data);
}
static __inline hi_u16 hi_ext_system_dpc_count_min_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_dpc_base(vi_pipe)  +  0xa);
}

/* register: hi_ext_system_dpc_trigger_time */
/* the bad pixel trigger frame cnt of time out */
#define HI_EXT_SYSTEM_DPC_TRIGGER_TIME_DEFAULT 0x640

/* status */
static __inline hi_void hi_ext_system_dpc_trigger_time_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_dpc_base(vi_pipe)  +  0xc, data);
}
static __inline hi_u16 hi_ext_system_dpc_trigger_time_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_dpc_base(vi_pipe)  +  0xc) ;
}

/* register: hi_ext_system_dpc_dynamic_manual_enable */
#define HI_EXT_SYSTEM_DPC_DYNAMIC_MANUAL_ENABLE_DEFAULT 0x0
/* dynamic_manual_enable */
static __inline hi_void hi_ext_system_dpc_dynamic_manual_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dpc_base(vi_pipe)  +  0xe, data & 0x1);
}
static __inline hi_u8 hi_ext_system_dpc_dynamic_manual_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_dpc_base(vi_pipe)  +  0xe) & 0x1);
}

/* register: hi_ext_system_dpc_dynamic_strength_table */
/* dpc_dynamic_strength_table */
static __inline hi_void hi_ext_system_dpc_dynamic_strength_table_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u16 data)
{
    iowr_16direct(vreg_dpc_base(vi_pipe)  +  0x10  + index * sizeof(hi_u16), data);
}

static __inline hi_u16 hi_ext_system_dpc_dynamic_strength_table_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return iord_16direct(vreg_dpc_base(vi_pipe)  +  0x10  +  index * sizeof(hi_u16));
}

/* register: hi_ext_system_dpc_dynamic_blend_ratio_table */
/* dpc_blend_ratio_table */
static __inline hi_void hi_ext_system_dpc_dynamic_blend_ratio_table_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u16 data)
{
    iowr_16direct(vreg_dpc_base(vi_pipe)  +  0x30  + index * sizeof(hi_u16), data);
}

static __inline hi_u16 hi_ext_system_dpc_dynamic_blend_ratio_table_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return iord_16direct(vreg_dpc_base(vi_pipe)  +  0x30   +  index * sizeof(hi_u16));
}

/* register: hi_ext_system_dpc_dynamic_strength */
/* dpc_dynamic_strength */
#define HI_EXT_SYSTEM_DPC_DYNAMIC_STRENGTH_DEFAULT 0x0
static __inline hi_void hi_ext_system_dpc_dynamic_strength_write(hi_vi_pipe vi_pipe,  hi_u16 data)
{
    iowr_16direct(vreg_dpc_base(vi_pipe)  +  0x50, data);
}

static __inline hi_u16 hi_ext_system_dpc_dynamic_strength_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_dpc_base(vi_pipe)  +  0x50);
}

/* register: hi_ext_system_dpc_dynamic_blend_ratio_table */
/* dpc_blend_ratio */
#define HI_EXT_SYSTEM_DPC_DYNAMIC_BLEND_RATIO_DEFAULT 0x0
static __inline hi_void hi_ext_system_dpc_dynamic_blend_ratio_write(hi_vi_pipe vi_pipe,  hi_u16 data)
{
    iowr_16direct(vreg_dpc_base(vi_pipe)  +  0x52, data);
}

static __inline hi_u16 hi_ext_system_dpc_dynamic_blend_ratio_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_dpc_base(vi_pipe)  +  0x52);
}

/* register: hi_ext_system_dpc_manual_mode */
#define HI_EXT_SYSTEM_DPC_MANU_MODE_DEFAULT 0x0
static __inline hi_void hi_ext_system_dpc_manual_mode_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dpc_base(vi_pipe)  +  0x54, data & 0x1);
}
static __inline hi_u8 hi_ext_system_dpc_manual_mode_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_dpc_base(vi_pipe)  +  0x54) & 0x1);
}

/* register: hi_ext_system_dpc_alpha0_rb_write */
/* hi_ext_system_dpc_alpha0_rb_write */
#define HI_EXT_SYSTEM_DPC_DYNAMIC_ALPHA0_RB_DEFAULT  0x0
static __inline hi_void hi_ext_system_dpc_alpha0_rb_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dpc_base(vi_pipe)  +  0x55, data);
}
static __inline hi_u8 hi_ext_system_dpc_alpha0_rb_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_dpc_base(vi_pipe)  +  0x55));
}

/* register: hi_ext_system_dpc_alpha0_rb_write */
/* hi_ext_system_dpc_alpha0_rb_write */
#define HI_EXT_SYSTEM_DPC_DYNAMIC_ALPHA0_G_DEFAULT 0x0
static __inline hi_void hi_ext_system_dpc_alpha0_g_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dpc_base(vi_pipe)  +  0x56, data);
}
static __inline hi_u8 hi_ext_system_dpc_alpha0_g_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_dpc_base(vi_pipe)  +  0x56));
}

/* register: hi_ext_system_dpc_suppress_twinkle_enable_write */
/* hi_ext_system_dpc_suppress_twinkle_enable */
#define HI_EXT_SYSTEM_DPC_SUPPRESS_TWINKLE_ENABLE_DEFAULT 0x0
static __inline hi_void hi_ext_system_dpc_suppress_twinkle_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dpc_base(vi_pipe)  +  0x57, data & 0x1);
}
static __inline hi_u8 hi_ext_system_dpc_suppress_twinkle_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_dpc_base(vi_pipe)  +  0x57) & 0x1);
}

/* register: hi_ext_system_dpc_suppress_twinkle_thr_write */
/* hi_ext_system_dpc_suppress_twinkle_thr */
#define HI_EXT_SYSTEM_DPC_SUPPRESS_TWINKLE_THR_DEFAULT 0x6
static __inline hi_void hi_ext_system_dpc_suppress_twinkle_thr_write(hi_vi_pipe vi_pipe, hi_s8 data)
{
    iowr_16direct(vreg_dpc_base(vi_pipe)  +  0x58, data);
}
static __inline hi_s8 hi_ext_system_dpc_suppress_twinkle_thr_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_dpc_base(vi_pipe)  +  0x58) & 0xFF);
}

/* register: hi_ext_system_dpc_suppress_twinkle_slope_write */
/* hi_ext_system_dpc_suppress_twinkle_slope */
#define HI_EXT_SYSTEM_DPC_SUPPRESS_TWINKLE_SLOPE_DEFAULT 0x15
static __inline hi_void hi_ext_system_dpc_suppress_twinkle_slope_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dpc_base(vi_pipe)  +  0x59, data & 0xFF);
}
static __inline hi_u8 hi_ext_system_dpc_suppress_twinkle_slope_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_dpc_base(vi_pipe)  +  0x59) & 0xFF);
}

/* register: hi_ext_system_dpc_bpt_calib_number */
/* hi_ext_system_dpc_bpt_calib_number */
#define HI_EXT_SYSTEM_DPC_BPT_CALIB_NUMBER_DEFAULT 0x0
static __inline hi_void hi_ext_system_dpc_bpt_calib_number_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_dpc_base(vi_pipe)  +  0x5a, data);
}
static __inline hi_u16 hi_ext_system_dpc_bpt_calib_number_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_dpc_base(vi_pipe)  +  0x5a));
}

/* register: hi_ext_system_dpc_bpt_cor_number */
/* hi_ext_system_dpc_bpt_cor_number */
#define HI_EXT_SYSTEM_DPC_BPT_COR_NUMBER_DEFAULT 0x0
static __inline hi_void hi_ext_system_dpc_bpt_cor_number_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_dpc_base(vi_pipe)  +  0x5c, data);
}
static __inline hi_u16 hi_ext_system_dpc_bpt_cor_number_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_dpc_base(vi_pipe)  +  0x5c));
}

/* register: hi_ext_system_dpc_static_attr_updata */
/* hi_ext_system_dpc_static_attr_updata */
#define HI_EXT_SYSTEM_DPC_STATIC_ATTR_UPDATE_DEFAULT 0x0
static __inline hi_void hi_ext_system_dpc_static_attr_update_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dpc_base(vi_pipe)  +  0x5e, data & 0x1);
}
static __inline hi_u8 hi_ext_system_dpc_static_attr_update_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_dpc_base(vi_pipe)  +  0x5e) & 0x1);
}

static __inline hi_void hi_ext_system_dpc_dynamic_attr_update_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dpc_base(vi_pipe)  +  0x5f, data & 0x1);
}
static __inline hi_u8 hi_ext_system_dpc_dynamic_attr_update_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_dpc_base(vi_pipe)  +  0x5f) & 0x1);
}

#ifdef CONFIG_HI_ISP_DPC_STATIC_TABLE_SUPPORT
/* DPC table */
/* register: hi_ext_system_dpc_calib_bpt */
/* hi_ext_system_dpc_calib_bpt */
static __inline hi_void hi_ext_system_dpc_calib_bpt_write(hi_vi_pipe vi_pipe, hi_u16 index, hi_u32 data)
{
    iowr_32direct(vreg_dpc_base(vi_pipe)  +  0x60  + index * sizeof(hi_u32), data);
}

static __inline hi_u32 hi_ext_system_dpc_calib_bpt_read(hi_vi_pipe vi_pipe,  hi_u16 index)
{
    return iord_32direct(vreg_dpc_base(vi_pipe)  +  0x60  +  index * sizeof(hi_u32));
}

/* register: hi_ext_system_dpc_cor_bpt */
/* hi_ext_system_dpc_cor_bpt */
static __inline hi_void hi_ext_system_dpc_cor_bpt_write(hi_vi_pipe vi_pipe, hi_u16 index, hi_u32 data)
{
    iowr_32direct(vreg_dpc_base(vi_pipe)  +  0x8060  + index * sizeof(hi_u32), data);
}

static __inline hi_u32 hi_ext_system_dpc_cor_bpt_read(hi_vi_pipe vi_pipe,  hi_u16 index)
{
    return iord_32direct(vreg_dpc_base(vi_pipe)  +  0x8060  +  index * sizeof(hi_u32));
}
#endif

/* GE */
/* register: hi_ext_system_ge_npoffset */
/* ge npoffset table */
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_ge_npoffset_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u16 data)
{
    iowr_16direct(vreg_ge_base(vi_pipe)  + index * sizeof(hi_u16), data);
}

static __inline hi_u16 hi_ext_system_ge_npoffset_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return iord_16direct(vreg_ge_base(vi_pipe)  +  index * sizeof(hi_u16));
}

/* register: hi_ext_system_ge_threshold_ */
/* ge threshold_ table */
#define HI_EXT_SYSTEM_GE_THRESHOLD_DEFAULT 0
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_ge_threshold_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u16 data)
{
    iowr_16direct(vreg_ge_base(vi_pipe)  +  0x20  + (index * sizeof(hi_u16)), data);
}

static __inline hi_u16 hi_ext_system_ge_threshold_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return iord_16direct(vreg_ge_base(vi_pipe)  +  0x20  + (index * sizeof(hi_u16)));
}

/* register: hi_ext_system_ge_strength_ */
/* ge strength_ table */
static __inline hi_void hi_ext_system_ge_strength_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u16 data)
{
    iowr_16direct(vreg_ge_base(vi_pipe)  +  0x40  + index * sizeof(hi_u16), data);
}

static __inline hi_u16 hi_ext_system_ge_strength_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return iord_16direct(vreg_ge_base(vi_pipe)  +  0x40   +  index * sizeof(hi_u16));
}

/* ge_slope */
static __inline hi_void hi_ext_system_ge_slope_write(hi_vi_pipe vi_pipe,  hi_u16 data)
{
    iowr_16direct(vreg_ge_base(vi_pipe)  +  0x60, data);
}

static __inline hi_u16 hi_ext_system_ge_slope_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_ge_base(vi_pipe)  +  0x60);
}

/* ge_sensitivity */
static __inline hi_void hi_ext_system_ge_sensitivity_write(hi_vi_pipe vi_pipe,  hi_u16 data)
{
    iowr_16direct(vreg_ge_base(vi_pipe)  +  0x62, data);
}

static __inline hi_u16 hi_ext_system_ge_sensitivity_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_ge_base(vi_pipe)  +  0x62);
}

/* ge_sensithreshold */
static __inline hi_void hi_ext_system_ge_sensithreshold_write(hi_vi_pipe vi_pipe,  hi_u16 data)
{
    iowr_16direct(vreg_ge_base(vi_pipe)  +  0x64, data);
}

static __inline hi_u16 hi_ext_system_ge_sensithreshold_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_ge_base(vi_pipe)  +  0x64);
}

/* ge_enable */
static __inline hi_void hi_ext_system_ge_enable_write(hi_vi_pipe vi_pipe,  hi_u8 data)
{
    iowr_8direct(vreg_ge_base(vi_pipe)  +  0x66, data & 0x1);
}

static __inline hi_u8 hi_ext_system_ge_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_ge_base(vi_pipe)  +  0x66) & 0x1);
}

static __inline hi_void hi_ext_system_ge_coef_update_en_write(hi_vi_pipe vi_pipe,  hi_u8 data)
{
    iowr_8direct(vreg_ge_base(vi_pipe)  +  0x67, data & 0x1);
}

static __inline hi_u8 hi_ext_system_ge_coef_update_en_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_ge_base(vi_pipe)  +  0x67) & 0x1);
}

/* register: hi_ext_system_demosaic_manual_mode_write */
/* args: data (8-bit) */
#define HI_EXT_SYSTEM_DEMOSAIC_MANUAL_MODE_DEFAULT 0
static __inline hi_void hi_ext_system_demosaic_manual_mode_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dm_base(vi_pipe) + 0x10, data & 0x1);
}
static __inline hi_u8 hi_ext_system_demosaic_manual_mode_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_dm_base(vi_pipe) + 0x10) & 0x1);
}

/* register: hi_ext_system_demosaic_enable_write */
/* args: data (8-bit) */
#define HI_EXT_SYSTEM_DEMOSAIC_ENABLE_DEFAULT 0x1
#define HI_EXT_SYSTEM_DEMOSAIC_ENABLE_DATASIZE 1
static __inline hi_void hi_ext_system_demosaic_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dm_base(vi_pipe) + 0x11, data & 0x1);
}
static __inline hi_u8 hi_ext_system_demosaic_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_dm_base(vi_pipe) + 0x11) & 0x1);
}

/* register: hi_ext_system_demosaic_attr_update_en_write */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_demosaic_attr_update_en_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dm_base(vi_pipe) + 0x12, data & 0x1);
}
static __inline hi_u8 hi_ext_system_demosaic_attr_update_en_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_dm_base(vi_pipe) + 0x12) & 0x1);
}

/* register: hi_ext_system_demosaic_manual_nondirection_midfreq_detailenhance_strength_write */
/* args: data (8-bit) */
#define HI_EXT_SYSTEM_DEMOSAIC_MANUAL_NDDM_MF_EHNC_STR_LINDFT  8
#define HI_EXT_SYSTEM_DEMOSAIC_MANUAL_NDDM_MF_EHNC_STR_WDRDFT  0
static __inline hi_void hi_ext_system_demosaic_manual_nddm_mfehnc_str_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dm_base(vi_pipe) + 0x13, data);
}
static __inline hi_u8 hi_ext_system_demosaic_manual_nddm_mfehnc_str_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dm_base(vi_pipe) + 0x13);
}

/* register: hi_ext_system_demosaic_manual_nondirection_higfreq_detailenhance_strength_write */
/* args: data (8-bit) */
#define HI_EXT_SYSTEM_DEMOSAIC_MANUAL_NDDM_HF_EHNC_STR_LINDFT  3
#define HI_EXT_SYSTEM_DEMOSAIC_MANUAL_NDDM_HF_EHNC_STR_WDRDFT  0
static __inline hi_void hi_ext_system_demosaic_manual_nddm_hfehnc_str_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dm_base(vi_pipe) + 0x14, data);
}
static __inline hi_u8 hi_ext_system_demosaic_manual_nddm_hfehnc_str_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dm_base(vi_pipe) + 0x14);
}

/* register: hi_ext_system_demosaic_manual_nondirection_strength_write */
/* args: data (8-bit) */
#define HI_EXT_SYSTEM_DEMOSAIC_MANUAL_NDDM_STR_LINDFT 64
#define HI_EXT_SYSTEM_DEMOSAIC_MANUAL_NDDM_STR_WDRDFT 64
static __inline hi_void hi_ext_system_demosaic_manual_nddm_str_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dm_base(vi_pipe) + 0x15, data);
}
static __inline hi_u8 hi_ext_system_demosaic_manual_nddm_str_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dm_base(vi_pipe) + 0x15);
}

/* register: hi_ext_system_demosaic_manual_detail_smooth_range_write */
/* args: data (8-bit) */
#define HI_EXT_SYSTEM_DEMOSAIC_MANUAL_DTLSMOOTH_RANGE_LINDFT 2
#define HI_EXT_SYSTEM_DEMOSAIC_MANUAL_DTLSMOOTH_RANGE_WDRDFT 2
static __inline hi_void hi_ext_system_demosaic_manual_dtlsmooth_range_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dm_base(vi_pipe) + 0x16, data);
}
static __inline hi_u8 hi_ext_system_demosaic_manual_dtlsmooth_range_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dm_base(vi_pipe) + 0x16);
}

/* register: hi_ext_system_demosaic_manual_detail_smooth_strength_write */
/* args: data (16-bit) */
#define HI_EXT_SYSTEM_DEMOSAIC_MANUAL_DTLSMOOTH_STR_LINDFT 256
#define HI_EXT_SYSTEM_DEMOSAIC_MANUAL_DTLSMOOTH_STR_WDRDFT 256
static __inline hi_void hi_ext_system_demosaic_manual_dtlsmooth_str_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_dm_base(vi_pipe) + 0x18, data);
}
static __inline hi_u16 hi_ext_system_demosaic_manual_dtlsmooth_str_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_dm_base(vi_pipe) + 0x18);
}

/* register: hi_ext_system_demosaic_auto_nondirection_midfreq_detailenhance_strength_write */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_demosaic_auto_nddm_mfehnc_str_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u8 data)
{
    iowr_8direct(vreg_dm_base(vi_pipe) + 0x20 + (index * sizeof(hi_u8)), data);
}
static __inline hi_u8 hi_ext_system_demosaic_auto_nddm_mfehnc_str_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return iord_8direct(vreg_dm_base(vi_pipe) + 0x20 + (index * sizeof(hi_u8)));
}

/* register: hi_ext_system_demosaic_auto_detail_smooth_range_write */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_demosaic_auto_nddm_hfehnc_str_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u8 data)
{
    iowr_8direct(vreg_dm_base(vi_pipe) + 0x30 + (index * sizeof(hi_u8)), data);
}
static __inline hi_u8 hi_ext_system_demosaic_auto_nddm_hfehnc_str_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return iord_8direct(vreg_dm_base(vi_pipe) + 0x30 + (index * sizeof(hi_u8)));
}

/* register: hi_ext_system_demosaic_auto_nondirection_strength_write */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_demosaic_auto_nddm_str_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u8 data)
{
    iowr_8direct(vreg_dm_base(vi_pipe) + 0x40 + (index * sizeof(hi_u8)), data);
}
static __inline hi_u8 hi_ext_system_demosaic_auto_nddm_str_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return iord_8direct(vreg_dm_base(vi_pipe) + 0x40 + (index * sizeof(hi_u8)));
}

/* register: hi_ext_system_demosaic_auto_detail_smooth_range_write */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_demosaic_auto_dtlsmooth_range_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u8 data)
{
    iowr_8direct(vreg_dm_base(vi_pipe) + 0x50 + (index * sizeof(hi_u8)), data);
}
static __inline hi_u8 hi_ext_system_demosaic_auto_dtlsmooth_range_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return iord_8direct(vreg_dm_base(vi_pipe) + 0x50 + (index * sizeof(hi_u8)));
}

/* register: hi_ext_system_demosaic_auto_detail_smooth_strength_write */
/* args: data (16-bit) */
#define HI_EXT_SYSTEM_DEMOSAIC_AUTO_DTLSMOOTH_STR_DEFAULT 256
static __inline hi_void hi_ext_system_demosaic_auto_dtlsmooth_str_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u16 data)
{
    iowr_16direct(vreg_dm_base(vi_pipe) + 0x60 + (index * sizeof(hi_u16)), data);
}
static __inline hi_u16 hi_ext_system_demosaic_auto_dtlsmooth_str_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return iord_16direct(vreg_dm_base(vi_pipe) + 0x60 + (index * sizeof(hi_u16)));
}

/* register: hi_ext_system_demosaic_manual_colornoise_thdf_write */
#define HI_EXT_SYSTEM_DEMOSAIC_COLORNOISE_CTRL_THDF_DEFAULT 0
static __inline hi_void hi_ext_system_demosaic_manual_colornoise_thdf_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dm_base(vi_pipe) + 0x80, data);
}
static __inline hi_u8 hi_ext_system_demosaic_manual_colornoise_thdf_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dm_base(vi_pipe) + 0x80);
}

/* register: hi_ext_system_demosaic_manual_colornoise_strf_write */
#define HI_EXT_SYSTEM_DEMOSAIC_COLORNOISE_CTRL_STRF_DEFAULT 8
static __inline hi_void hi_ext_system_demosaic_manual_colornoise_strf_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dm_base(vi_pipe) + 0x81, data);
}
static __inline hi_u8 hi_ext_system_demosaic_manual_colornoise_strf_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dm_base(vi_pipe) + 0x81);
}

/* register: hi_ext_system_demosaic_manual_desat_dark_range_write */
/* args: data (8-bit) */
#define HI_EXT_SYSTEM_DEMOSAIC_MANUAL_DESAT_DARK_RANGE_DEFAULT  0
#define HI_EXT_SYSTEM_DEMOSAIC_MANUAL_DESAT_DARK_RANGE_DATASIZE 1
static __inline hi_void hi_ext_system_demosaic_manual_desat_dark_range_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dm_base(vi_pipe) + 0x82, data);
}
static __inline hi_u8 hi_ext_system_demosaic_manual_desat_dark_range_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dm_base(vi_pipe) + 0x82);
}

/* register: hi_ext_system_demosaic_manual_desat_dark_strength_write */
/* args: data (8-bit) */
#define HI_EXT_SYSTEM_DEMOSAIC_MANUAL_DESAT_DARK_STRENGTH_DEFAULT  0
#define HI_EXT_SYSTEM_DEMOSAIC_MANUAL_DESAT_DARK_STRENGTH_DATASIZE 1
static __inline hi_void hi_ext_system_demosaic_manual_desat_dark_strength_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dm_base(vi_pipe) + 0x83, data);
}
static __inline hi_u8 hi_ext_system_demosaic_manual_desat_dark_strength_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dm_base(vi_pipe) + 0x83);
}
/* register: hi_ext_system_demosaic_auto_colornoise_thdf_write */
/* args: data (16-bit) */
#define HI_EXT_SYSTEM_DEMOSAIC_AUTO_COLORNOISE_THDF_DEFAULT 0
static __inline hi_void hi_ext_system_demosaic_auto_colornoise_thdf_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u8 data)
{
    iowr_8direct(vreg_dm_base(vi_pipe) + 0x90 + (index * sizeof(hi_u8)), data);
}
static __inline hi_u8 hi_ext_system_demosaic_auto_colornoise_thdf_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return iord_8direct(vreg_dm_base(vi_pipe) + 0x90 + (index * sizeof(hi_u8)));
}

/* register: hi_ext_system_demosaic_auto_colornoise_strf_write */
/* args: data (16-bit) */
#define HI_EXT_SYSTEM_DEMOSAIC_AUTO_COLORNOISE_STRF_DEFAULT 8
static __inline hi_void hi_ext_system_demosaic_auto_colornoise_strf_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u8 data)
{
    iowr_8direct(vreg_dm_base(vi_pipe) + 0xa0 + (index * sizeof(hi_u8)), data);
}
static __inline hi_u8 hi_ext_system_demosaic_auto_colornoise_strf_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return iord_8direct(vreg_dm_base(vi_pipe) + 0xa0 + (index * sizeof(hi_u8)));
}

/* register: hi_ext_system_demosaic_auto_desat_dark_range */
/* demosaic desat dark range */
#define HI_EXT_SYSTEM_DEMOSAIC_AUTO_DESAT_DARK_RANGE_DEFAULT 30
#define HI_EXT_SYSTEM_DEMOSAIC_AUTO_DESAT_DARK_RANGE_DATASIZE 16
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_demosaic_auto_desat_dark_range_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u8 data)
{
    iowr_8direct(vreg_dm_base(vi_pipe) + 0xb0 + (index * sizeof(hi_u8)), data);
}
static __inline hi_u8 hi_ext_system_demosaic_auto_desat_dark_range_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return iord_8direct(vreg_dm_base(vi_pipe) + 0xb0 + (index * sizeof(hi_u8)));
}

/* register: hi_ext_system_demosaic_auto_desat_dark_strength */
/* demosaic desat dark strength */
#define HI_EXT_SYSTEM_DEMOSAIC_AUTO_DESAT_DARK_STRENGTH_DEFAULT 0
#define HI_EXT_SYSTEM_DEMOSAIC_AUTO_DESAT_DARK_STRENGTH_DATASIZE 16
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_demosaic_auto_desat_dark_strength_write(hi_vi_pipe vi_pipe,
                                                                              hi_u8 index, hi_u8 data)
{
    iowr_8direct(vreg_dm_base(vi_pipe) + 0xc0 + (index * sizeof(hi_u8)), data);
}
static __inline hi_u8 hi_ext_system_demosaic_auto_desat_dark_strength_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return iord_8direct(vreg_dm_base(vi_pipe) + 0xc0 + (index * sizeof(hi_u8)));
}
/* register: hi_ext_system_antifalsecolor_enable_write */
/* args: data (8-bit) */
#define HI_EXT_SYSTEM_ANTIFALSECOLOR_ENABLE_DEFAULT 0x1
static __inline hi_void hi_ext_system_afc_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_fcr_base(vi_pipe), (data & 0x1));
}
static __inline hi_u8 hi_ext_system_afc_enable_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_fcr_base(vi_pipe));
}

/* register: hi_ext_system_fcr_manual_mode_write */
/* args: data (8-bit) */
#define HI_EXT_SYSTEM_ANTIFALSECOLOR_MANUAL_MODE_DEFAULT 0
static __inline hi_void hi_ext_system_afc_manual_mode_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_fcr_base(vi_pipe) + 0x1, data);
}
static __inline hi_u8 hi_ext_system_afc_manual_mode_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_fcr_base(vi_pipe) + 0x1);
}

/* register: hi_ext_system_antifalsecolor_auto_threshold_write */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_afc_auto_thd_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u8 data)
{
    iowr_8direct(vreg_fcr_base(vi_pipe) + 0x2 + (index * sizeof(hi_u8)), data);
}
static __inline hi_u8 hi_ext_system_afc_auto_thd_read(hi_vi_pipe vi_pipe, hi_u8 index)
{
    return iord_8direct(vreg_fcr_base(vi_pipe) + 0x2 + (index * sizeof(hi_u8)));
}

/* register: hi_ext_system_antifalsecolor_auto_strenght_write */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_afc_auto_str_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u8 data)
{
    iowr_8direct(vreg_fcr_base(vi_pipe) + 0x12 + (index * sizeof(hi_u8)), data);
}
static __inline hi_u8 hi_ext_system_afc_auto_str_read(hi_vi_pipe vi_pipe, hi_u8 index)
{
    return iord_8direct(vreg_fcr_base(vi_pipe) + 0x12 + (index * sizeof(hi_u8)));
}

/* register: hi_ext_system_antifalsecolor_manual_threshold_write */
/* args: data (8-bit) */
#define HI_EXT_SYSTEM_ANTIFALSECOLOR_MANUAL_THRESHOLD_DEFAULT 8
static __inline hi_void hi_ext_system_afc_manual_thd_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_fcr_base(vi_pipe) + 0x22, data);
}
static __inline hi_u8 hi_ext_system_afc_manual_thd_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_fcr_base(vi_pipe) + 0x22);
}

/* register: hi_ext_system_antifalsecolor_manual_strenght_write */
/* args: data (8-bit) */
#define HI_EXT_SYSTEM_ANTIFALSECOLOR_MANUAL_STRENGTH_DEFAULT 8
static __inline hi_void hi_ext_system_afc_manual_str_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_fcr_base(vi_pipe) + 0x23, data);
}
static __inline hi_u8 hi_ext_system_afc_manual_str_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_fcr_base(vi_pipe) + 0x23);
}

/* BAYER NR: vreg_bnr_base(vi_pipe) ~ vreg_bnr_base(vi_pipe)  + */
/* register: hi_ext_system_bayernr_enable_write */
/* args: data (8-bit) */
#define HI_EXT_SYSTEM_BAYERNR_ENABLE_DEFAULT  1
static __inline hi_void hi_ext_system_bayernr_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_bnr_base(vi_pipe), (data & 0x1));
}
static __inline hi_u8 hi_ext_system_bayernr_enable_read(hi_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_bnr_base(vi_pipe))) & 0x1);
}

/* register: hi_ext_system_bayernr_manual_mode_write */
/* args: data (8-bit) */
#define HI_EXT_SYSTEM_BAYERNR_MANU_MODE_DEFAULT  0
static __inline hi_void hi_ext_system_bayernr_manual_mode_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_bnr_base(vi_pipe) + 0x1, data);
}
static __inline hi_u8 hi_ext_system_bayernr_manual_mode_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_bnr_base(vi_pipe) + 0x1);
}

/* register: hi_ext_system_bayernr_low_power_enable_write */
/* args: data (8-bit) */
#define HI_EXT_SYSTEM_BAYERNR_LOW_POWER_ENABLE_DEFAULT  0
static __inline hi_void hi_ext_system_bayernr_low_power_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_bnr_base(vi_pipe) + 0x2, (data & 0x1));
}
static __inline hi_u8 hi_ext_system_bayernr_low_power_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_bnr_base(vi_pipe) + 0x2) & 0x1);
}

/* register: hi_ext_system_bayernr_lsc_enable_write */
/* args: data (8-bit) */
#define HI_EXT_SYSTEM_BAYERNR_LSC_ENABLE_DEFAULT  0
static __inline hi_void hi_ext_system_bayernr_lsc_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_bnr_base(vi_pipe) + 0x3, data & 0x1);
}
static __inline hi_u8 hi_ext_system_bayernr_lsc_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_bnr_base(vi_pipe) + 0x3) & 0x1);
}

/* register: hi_ext_system_bayernr_coef_update_en_write */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_bayernr_coef_update_en_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_bnr_base(vi_pipe) + 0x6, data & 0x1);
}
static __inline hi_u8 hi_ext_system_bayernr_coef_update_en_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_bnr_base(vi_pipe) + 0x6) & 0x1);
}

/* register: hi_ext_system_bayernr_mono_sensor_write */
/* args: data (8-bit) */
#define HI_EXT_SYSTEM_BAYERNR_MONO_SENSOR_ENABLE_DEFAULT  0
static __inline hi_void hi_ext_system_bayernr_mono_sensor_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_bnr_base(vi_pipe) + 0x7, data);
}
static __inline hi_u8 hi_ext_system_bayernr_mono_sensor_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_bnr_base(vi_pipe) + 0x7);
}

/* register: hi_ext_system_bayernr_lsc_nr_ratio_write */
/* args: data (16-bit) */
#define HI_EXT_SYSTEM_BAYERNR_LSC_NR_RATIO_DEFAULT   255
static __inline hi_void hi_ext_system_bayernr_lsc_nr_ratio_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_bnr_base(vi_pipe) + 0x8, data);
}
static __inline hi_u8 hi_ext_system_bayernr_lsc_nr_ratio_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_bnr_base(vi_pipe) + 0x8);
}

/* register: hi_ext_system_bayernr_lsc_max_gain_write */
/* args: data (8-bit) */
#define HI_EXT_SYSTEM_BAYERNR_LSC_MAX_GAIN_DEFAULT   96
static __inline hi_void hi_ext_system_bayernr_lsc_max_gain_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_bnr_base(vi_pipe) + 0x9, data);
}
static __inline hi_u8 hi_ext_system_bayernr_lsc_max_gain_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_bnr_base(vi_pipe) + 0x9);
}

/* register: hi_ext_system_bayernr_lsc_cmp_strength_write */
/* args: data (16-bit) */
#define HI_EXT_SYSTEM_BAYERNR_LSC_CMP_STRENGTH_DEFAULT   256
static __inline hi_void hi_ext_system_bayernr_lsc_cmp_strength_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_bnr_base(vi_pipe) + 0xa, data);
}
static __inline hi_u16 hi_ext_system_bayernr_lsc_cmp_strength_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_bnr_base(vi_pipe) + 0xa);
}

/* register: hi_ext_system_bayernr_auto_coarse_strength_r */
/* args: data (16-bit) */
#define HI_EXT_SYSTEM_BAYERNR_AUTO_COARSE_STRENGTH_R_DEFAULT  102
static __inline hi_void hi_ext_system_bayernr_auto_coarse_strength_r_write(hi_vi_pipe vi_pipe, hi_u8 i, hi_u16 data)
{
    iowr_16direct(vreg_bnr_base(vi_pipe) + 0x20 + (i * sizeof(hi_u16)), data);
}
static __inline hi_u16 hi_ext_system_bayernr_auto_coarse_strength_r_read(hi_vi_pipe vi_pipe,  hi_u8 i)
{
    return iord_16direct(vreg_bnr_base(vi_pipe) + 0x20 + (i * sizeof(hi_u16)));
}

/* register: hi_ext_system_bayernr_auto_coarse_strength_gr */
/* args: data (16-bit) */
#define HI_EXT_SYSTEM_BAYERNR_AUTO_COARSE_STRENGTH_GR_DEFAULT  100
static __inline hi_void hi_ext_system_bayernr_auto_coarse_strength_gr_write(hi_vi_pipe vi_pipe, hi_u8 i, hi_u16 data)
{
    iowr_16direct(vreg_bnr_base(vi_pipe) + 0x40 + (i * sizeof(hi_u16)), data);
}
static __inline hi_u16 hi_ext_system_bayernr_auto_coarse_strength_gr_read(hi_vi_pipe vi_pipe,  hi_u8 i)
{
    return iord_16direct(vreg_bnr_base(vi_pipe) + 0x40 + (i * sizeof(hi_u16)));
}

/* register: hi_ext_system_bayernr_auto_coarse_strength_gb */
/* args: data (16-bit) */
#define HI_EXT_SYSTEM_BAYERNR_AUTO_COARSE_STRENGTH_GB_DEFAULT  100
static __inline hi_void hi_ext_system_bayernr_auto_coarse_strength_gb_write(hi_vi_pipe vi_pipe, hi_u8 i, hi_u16 data)
{
    iowr_16direct(vreg_bnr_base(vi_pipe) + 0x60 + (i * sizeof(hi_u16)), data);
}
static __inline hi_u16 hi_ext_system_bayernr_auto_coarse_strength_gb_read(hi_vi_pipe vi_pipe,  hi_u8 i)
{
    return iord_16direct(vreg_bnr_base(vi_pipe) + 0x60 + (i * sizeof(hi_u16)));
}

/* register: hi_ext_system_bayernr_auto_coarse_strength_b */
/* args: data (16-bit) */
#define HI_EXT_SYSTEM_BAYERNR_AUTO_COARSE_STRENGTH_B_DEFAULT  102
static __inline hi_void hi_ext_system_bayernr_auto_coarse_strength_b_write(hi_vi_pipe vi_pipe, hi_u8 i, hi_u16 data)
{
    iowr_16direct(vreg_bnr_base(vi_pipe) + 0x80 + (i * sizeof(hi_u16)), data);
}
static __inline hi_u16 hi_ext_system_bayernr_auto_coarse_strength_b_read(hi_vi_pipe vi_pipe,  hi_u8 i)
{
    return iord_16direct(vreg_bnr_base(vi_pipe) + 0x80 + (i * sizeof(hi_u16)));
}

/* register: hi_ext_system_bayernr_auto_chroma_strength_r_write */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_bayernr_auto_chroma_strength_r_write(hi_vi_pipe vi_pipe, hi_u8 i, hi_u8 data)
{
    iowr_8direct(vreg_bnr_base(vi_pipe) + 0xa0 + (i * sizeof(hi_u8)), data);
}
static __inline hi_u8 hi_ext_system_bayernr_auto_chroma_strength_r_read(hi_vi_pipe vi_pipe,  hi_u8 i)
{
    return iord_8direct(vreg_bnr_base(vi_pipe) + 0xa0 + (i * sizeof(hi_u8)));
}

/* register: hi_ext_system_bayernr_auto_chroma_strength_gr_write */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_bayernr_auto_chroma_strength_gr_write(hi_vi_pipe vi_pipe, hi_u8 i, hi_u8 data)
{
    iowr_8direct(vreg_bnr_base(vi_pipe) + 0xb0 + (i * sizeof(hi_u8)), data);
}
static __inline hi_u8 hi_ext_system_bayernr_auto_chroma_strength_gr_read(hi_vi_pipe vi_pipe,  hi_u8 i)
{
    return iord_8direct(vreg_bnr_base(vi_pipe) + 0xb0 + (i * sizeof(hi_u8)));
}

/* register: hi_ext_system_bayernr_auto_chroma_strength_gb_write */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_bayernr_auto_chroma_strength_gb_write(hi_vi_pipe vi_pipe, hi_u8 i, hi_u8 data)
{
    iowr_8direct(vreg_bnr_base(vi_pipe) + 0xc0 + (i * sizeof(hi_u8)), data);
}
static __inline hi_u8 hi_ext_system_bayernr_auto_chroma_strength_gb_read(hi_vi_pipe vi_pipe,  hi_u8 i)
{
    return iord_8direct(vreg_bnr_base(vi_pipe) + 0xc0 + (i * sizeof(hi_u8)));
}

/* register: hi_ext_system_bayernr_auto_chroma_strength_b_write */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_bayernr_auto_chroma_strength_b_write(hi_vi_pipe vi_pipe, hi_u8 i, hi_u8 data)
{
    iowr_8direct(vreg_bnr_base(vi_pipe) + 0xd0 + (i * sizeof(hi_u8)), data);
}
static __inline hi_u8 hi_ext_system_bayernr_auto_chroma_strength_b_read(hi_vi_pipe vi_pipe,  hi_u8 i)
{
    return iord_8direct(vreg_bnr_base(vi_pipe) + 0xd0 + (i * sizeof(hi_u8)));
}

/* register: hi_ext_system_bayernr_auto_fine_strength_write */
/* c ratio table */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_bayernr_auto_fine_strength_write(hi_vi_pipe vi_pipe, hi_u8 i, hi_u8 data)
{
    iowr_8direct(vreg_bnr_base(vi_pipe) + 0xe0 + (i * sizeof(hi_u8)), data);
}
static __inline hi_u8 hi_ext_system_bayernr_auto_fine_strength_read(hi_vi_pipe vi_pipe,  hi_u8 i)
{
    return iord_8direct(vreg_bnr_base(vi_pipe) + 0xe0 + (i * sizeof(hi_u8)));
}

/* register: hi_ext_system_bayernr_auto_coring_weight */
/* bayernr jnlm coring low */
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_bayernr_auto_coring_weight_write(hi_vi_pipe vi_pipe, hi_u8 i, hi_u16 data)
{
    iowr_16direct(vreg_bnr_base(vi_pipe) + 0x100 + (i * sizeof(hi_u16)), data);
}
static __inline hi_u16 hi_ext_system_bayernr_auto_coring_weight_read(hi_vi_pipe vi_pipe,  hi_u8 i)
{
    return iord_16direct(vreg_bnr_base(vi_pipe) + 0x100 + (i * sizeof(hi_u16)));
}

/* register: hi_ext_system_bayernr_manual_chroma_strength_write */
/* args: data (8-bit) */
#define HI_EXT_SYSTEM_BAYERNR_MANU_CHROMA_STRENGTH_DEFAULT 0

static __inline hi_void hi_ext_system_bayernr_manual_chroma_strength_write(hi_vi_pipe vi_pipe, hi_u8 i, hi_u8 data)
{
    iowr_8direct(vreg_bnr_base(vi_pipe) + 0x200 + (i * sizeof(hi_u8)), data);
}
static __inline hi_u8 hi_ext_system_bayernr_manual_chroma_strength_read(hi_vi_pipe vi_pipe,  hi_u8 i)
{
    return iord_8direct(vreg_bnr_base(vi_pipe) + 0x200 + (i * sizeof(hi_u8)));
}

/* register: hi_ext_system_bayernr_manual_fine_strength_write */
/* args: data (8-bit) */
#define HI_EXT_SYSTEM_BAYERNR_MANU_FINE_STRENGTH_DEFAULT  112
static __inline hi_void hi_ext_system_bayernr_manual_fine_strength_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_bnr_base(vi_pipe) + 0x210, data);
}
static __inline hi_u8 hi_ext_system_bayernr_manual_fine_strength_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_bnr_base(vi_pipe) + 0x210);
}

/* register: hi_ext_system_bayernr_manual_coring_weight_write */
/* args: data (16-bit) */
#define  HI_EXT_SYSTEM_BAYERNR_MANU_CORING_WEIGHT_DEFAULT 50
static __inline hi_void hi_ext_system_bayernr_manual_coring_weight_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_bnr_base(vi_pipe) + 0x212, data);
}
static __inline hi_u16 hi_ext_system_bayernr_manual_coring_weight_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_bnr_base(vi_pipe) + 0x212);
}

/* register: hi_ext_system_bayernr_manual_coarse_strength_write */
/* args: data (16-bit) */
#define  HI_EXT_SYSTEM_BAYERNR_MANU_COARSE_STRENGTH_DEFAULT 102
static __inline hi_void hi_ext_system_bayernr_manual_coarse_strength_write(hi_vi_pipe vi_pipe, hi_u8 i, hi_u16 data)
{
    iowr_16direct(vreg_bnr_base(vi_pipe) + 0x220 + (i * sizeof(hi_u16)), data);
}
static __inline hi_u16 hi_ext_system_bayernr_manual_coarse_strength_read(hi_vi_pipe vi_pipe, hi_u8 i)
{
    return iord_16direct(vreg_bnr_base(vi_pipe) + 0x220  + (i * sizeof(hi_u16)));
}

/* register: hi_ext_system_bayernr_wdr_frame_strength_write */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_bayernr_wdr_frame_strength_write(hi_vi_pipe vi_pipe, hi_u8 i, hi_u8 data)
{
    iowr_8direct(vreg_bnr_base(vi_pipe) + 0x250 + (i * sizeof(hi_u8)), data);
}
static __inline hi_u8 hi_ext_system_bayernr_wdr_frame_strength_read(hi_vi_pipe vi_pipe,  hi_u8 i)
{
    return iord_8direct(vreg_bnr_base(vi_pipe) + 0x250 + (i * sizeof(hi_u8)));
}

/* register: hi_ext_system_bayernr_fusion_frame_strength_write */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_bayernr_fusion_frame_strength_write(hi_vi_pipe vi_pipe, hi_u8 i, hi_u8 data)
{
    iowr_8direct(vreg_bnr_base(vi_pipe) + 0x270 + (i * sizeof(hi_u8)), data);
}
static __inline hi_u8 hi_ext_system_bayernr_fusion_frame_strength_read(hi_vi_pipe vi_pipe,  hi_u8 i)
{
    return iord_8direct(vreg_bnr_base(vi_pipe) + 0x270 + (i * sizeof(hi_u8)));
}

/* register: hi_ext_system_bayernr_coring_low_ratio_write */
/* args: data (16-bit) */
#define HI_EXT_SYSTEM_BAYERNR_CORING_RATIO_LUT_DEFAULT  800
static __inline hi_void hi_ext_system_bayernr_coring_ratio_write(hi_vi_pipe vi_pipe, hi_u8 i, hi_u16 data)
{
    iowr_16direct(vreg_bnr_base(vi_pipe) + 0x290 + (i * sizeof(hi_u16)), data);
}
static __inline hi_u16 hi_ext_system_bayernr_coring_ratio_read(hi_vi_pipe vi_pipe,  hi_u8 i)
{
    return iord_16direct(vreg_bnr_base(vi_pipe) + 0x290 + (i * sizeof(hi_u16)));
}

/* frame stitch WDR */
/* WDR---U8 --EXT_REGISTER */
/* register: hi_ext_wdr_manual_mode_enable */
/* 0: auto; 1: manual; */
/* hi_bool */
static __inline hi_void hi_ext_system_wdr_manual_mode_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe), data);
}
static __inline hi_u8 hi_ext_system_wdr_manual_mode_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_wdr_base(vi_pipe));
}

static __inline hi_void hi_ext_system_wdr_coef_update_en_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x1, data);
}
static __inline hi_u8 hi_ext_system_wdr_coef_update_en_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_wdr_base(vi_pipe)  +  0x1);
}

/* wdr_en */
#define HI_EXT_SYSTEM_WDR_EN_DEFAULT 0
static __inline hi_void hi_ext_system_wdr_en_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x2, data);
}
static __inline hi_u8 hi_ext_system_wdr_en_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_wdr_base(vi_pipe)  +  0x2);
}

/* fusion_mode */
#define HI_EXT_SYSTEM_FUSION_MODE_DEFAULT 0
static __inline hi_void hi_ext_system_fusion_mode_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x3, data);
}
static __inline hi_u8 hi_ext_system_fusion_mode_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_wdr_base(vi_pipe)  +  0x3);
}

/* mdt_en */
#define HI_EXT_SYSTEM_MDT_EN_DEFAULT 1
static __inline hi_void hi_ext_system_mdt_en_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x4, data);
}
static __inline hi_u8 hi_ext_system_mdt_en_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_wdr_base(vi_pipe)  +  0x4);
}

/* erosion_en */
#define HI_EXT_SYSTEM_EROSION_EN_DEFAULT 0
static __inline hi_void hi_ext_system_erosion_en_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x5, data);
}
static __inline hi_u8 hi_ext_system_erosion_en_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_wdr_base(vi_pipe)  +  0x5);
}

/* shortexpo_chk */
static __inline hi_void hi_ext_system_wdr_shortexpo_chk_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x6, data);
}
static __inline hi_u8 hi_ext_system_wdr_shortexpo_chk_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_wdr_base(vi_pipe)  +  0x6);
}

/* hi_u8 */
/* nos_g_wgt_mod */
#define HI_EXT_SYSTEM_WDR_NOISE_G_WEIGHT_MODE_DEFAULT 0x3
static __inline hi_void hi_ext_system_wdr_noise_g_weight_mode_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x10, data);
}
static __inline hi_u8 hi_ext_system_wdr_noise_g_weight_mode_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0x10));
}

/* nos_c_wgt_mod */
#define HI_EXT_SYSTEM_WDR_NOISE_C_WEIGHT_MODE_DEFAULT 0x3
static __inline hi_void hi_ext_system_wdr_noise_c_weight_mode_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x11, data);
}
static __inline hi_u8 hi_ext_system_wdr_noise_c_weight_mode_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0x11));
}

/* mdt_l_bld */
#define HI_EXT_SYSTEM_WDR_MDT_LONG_BLEND_DEFAULT 0xFE

static __inline hi_void hi_ext_system_wdr_mdt_long_blend_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x12, data);
}
static __inline hi_u8 hi_ext_system_wdr_mdt_long_blend_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0x12));
}

/* noise_model_coef */
#define HI_EXT_SYSTEM_WDR_NOISE_MODEL_COEF_DEFAULT 0x50
static __inline hi_void hi_ext_system_wdr_noise_model_coef_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x13, data);
}
static __inline hi_u8 hi_ext_system_wdr_noise_model_coef_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0x13));
}

/* noise_ratio_rg */
#define HI_EXT_SYSTEM_WDR_NOISE_RATIO_RG_DEFAULT 0x5A
static __inline hi_void hi_ext_system_wdr_noise_ratio_rg_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x14, data);
}
static __inline hi_u8 hi_ext_system_wdr_noise_ratio_rg_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0x14));
}

/* noise_ratio_bg */
#define HI_EXT_SYSTEM_WDR_NOISE_RATIO_BG_DEFAULT 0x5A
static __inline hi_void hi_ext_system_wdr_noise_ratio_bg_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x15, data);
}
static __inline hi_u8 hi_ext_system_wdr_noise_ratio_bg_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0x15));
}

/* g_sigma_gain1 */
#define HI_EXT_SYSTEM_WDR_G_SIGMA_GAIN1_DEFAULT 0x10
static __inline hi_void hi_ext_system_wdr_g_sigma_gain1_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x16, data);
}
static __inline hi_u8 hi_ext_system_wdr_g_sigma_gain1_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0x16));
}

/* g_sigma_gain2 */
#define HI_EXT_SYSTEM_WDR_G_SIGMA_GAIN2_DEFAULT 0x30
static __inline hi_void hi_ext_system_wdr_g_sigma_gain2_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x17, data);
}
static __inline hi_u8 hi_ext_system_wdr_g_sigma_gain2_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0x17));
}

/* g_sigma_gain3 */
#define HI_EXT_SYSTEM_WDR_G_SIGMA_GAIN3_DEFAULT 0x50
static __inline hi_void hi_ext_system_wdr_g_sigma_gain3_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x18, data);
}
static __inline hi_u8 hi_ext_system_wdr_g_sigma_gain3_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0x18));
}

/* csigma_gain2 */
#define HI_EXT_SYSTEM_WDR_C_SIGMA_GAIN1_DEFAULT 0x8
static __inline hi_void hi_ext_system_wdr_c_sigma_gain1_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x19, data);
}
static __inline hi_u8 hi_ext_system_wdr_c_sigma_gain1_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0x19));
}

/* csigma_gain2 */
#define HI_EXT_SYSTEM_WDR_C_SIGMA_GAIN2_DEFAULT 0x10
static __inline hi_void hi_ext_system_wdr_c_sigma_gain2_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x1a, data);
}
static __inline hi_u8 hi_ext_system_wdr_c_sigma_gain2_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0x1a));
}

/* csigma_gain3 */
#define HI_EXT_SYSTEM_WDR_C_SIGMA_GAIN3_DEFAULT 0x20
static __inline hi_void hi_ext_system_wdr_c_sigma_gain3_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x1b, data);
}
static __inline hi_u8 hi_ext_system_wdr_c_sigma_gain3_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0x1b));
}

/* bnr_full_mdt_thr */
#define HI_EXT_SYSTEM_WDR_BNR_FULL_MDT_THR_DEFAULT 0x50
static __inline hi_void hi_ext_system_wdr_bnr_full_mdt_thr_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x1d, data);
}
static __inline hi_u8 hi_ext_system_wdr_bnr_full_mdt_thr_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0x1d));
}

/* mdt_full_thr */
#define HI_EXT_SYSTEM_WDR_MDT_FULL_THR_DEFAULT 0xFE
static __inline hi_void hi_ext_system_wdr_mdt_full_thr_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x1e, data);
}
static __inline hi_u8 hi_ext_system_wdr_mdt_full_thr_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0x1e));
}

/* mdt_still_thr */
#define HI_EXT_SYSTEM_WDR_MDT_STILL_THR_DEFAULT 0x14
static __inline hi_void hi_ext_system_wdr_mdt_still_thr_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x1f, data);
}
static __inline hi_u8 hi_ext_system_wdr_mdt_still_thr_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0x1f));
}

/* full_mot_sig_wgt; */
#define HI_EXT_SYSTEM_WDR_FULL_MOT_SIGMA_WEIGHT_DEFAULT 31
static __inline hi_void hi_ext_system_wdr_full_mot_sigma_weight_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x20, data);
}
static __inline hi_u8 hi_ext_system_wdr_full_mot_sigma_weight_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0x20));
}

/* bnrmode */
#define HI_EXT_SYSTEM_BNR_MODE_WRITE_DEFAULT 1
static __inline hi_void hi_ext_system_bnr_mode_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x21, data);
}
static __inline hi_u8 hi_ext_system_bnr_mode_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0x21));
}

static __inline hi_void hi_ext_system_wdr_manual_mdthr_low_gain_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x22, data);
}
static __inline hi_u8 hi_ext_system_wdr_manual_mdthr_low_gain_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0x22));
}

static __inline hi_void hi_ext_system_wdr_manual_mdthr_hig_gain_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x23, data);
}
static __inline hi_u8 hi_ext_system_wdr_manual_mdthr_hig_gain_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0x23));
}

#define HI_EXT_SYSTEM_WDR_MDREF_FLICKER_DEFAULT 0x1

static __inline hi_void hi_ext_system_wdr_mdref_flicker_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0x24, data);
}
static __inline hi_u8 hi_ext_system_wdr_mdref_flicker_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0x24));
}

/* hi_u16 */
/* long thr */
#define HI_EXT_SYSTEM_WDR_LONGTHR_WRITE_DEFAULT 0xBC0
static __inline hi_void hi_ext_system_wdr_longthr_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_wdr_base(vi_pipe) + 0x40, data);
}
static __inline hi_u16 hi_ext_system_wdr_longthr_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_wdr_base(vi_pipe) + 0x40));
}

/* short thr */
#define HI_EXT_SYSTEM_WDR_SHORTTHR_WRITE_DEFAULT 0xFC0
static __inline hi_void hi_ext_system_wdr_shortthr_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_wdr_base(vi_pipe) + 0x42, data);
}
static __inline hi_u16 hi_ext_system_wdr_shortthr_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_wdr_base(vi_pipe) + 0x42));
}

static __inline hi_void hi_ext_system_fusion_thr_write(hi_vi_pipe vi_pipe, hi_u8 i, hi_u16 data)
{
    iowr_16direct(vreg_wdr_base(vi_pipe) + 0x44 + (i * sizeof(hi_u16)), data);
}
static __inline hi_u16 hi_ext_system_fusion_thr_read(hi_vi_pipe vi_pipe,  hi_u8 i)
{
    return iord_16direct(vreg_wdr_base(vi_pipe) + 0x44 + (i * sizeof(hi_u16)));
}

#define HI_EXT_SYSTEM_WDR_WBGAIN_POSITION_DEFAULT 1
#define HI_EXT_SYSTEM_WDR_WBGAIN_IN_DG1 0
#define HI_EXT_SYSTEM_WDR_WBGAIN_IN_WB 1

/* args: data (2-bit) */
static __inline hi_void hi_ext_system_wdr_wbgain_position_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_wdr_base(vi_pipe) + 0x4f), data);
}
static __inline hi_u8 hi_ext_system_wdr_wbgain_position_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_wdr_base(vi_pipe) + 0x4f);
}

/* LUT */
/* wdr floorset */
static __inline hi_void hi_ext_system_wdr_floorset_write(hi_vi_pipe vi_pipe, hi_u8 i, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0xb0 + (i * sizeof(hi_u8)), data);
}
static __inline hi_u8 hi_ext_system_wdr_floorset_read(hi_vi_pipe vi_pipe,  hi_u8 i)
{
    return iord_8direct(vreg_wdr_base(vi_pipe) + 0xb0 + (i * sizeof(hi_u8)));
}

static __inline hi_void hi_ext_system_wdr_auto_mdthr_low_gain_write(hi_vi_pipe vi_pipe, hi_u8 i, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0xc0 + (i * sizeof(hi_u8)), data);
}
static __inline hi_u8 hi_ext_system_wdr_auto_mdthr_low_gain_read(hi_vi_pipe vi_pipe, hi_u8 i)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0xc0 + (i * sizeof(hi_u8))));
}

static __inline hi_void hi_ext_system_wdr_auto_mdthr_hig_gain_write(hi_vi_pipe vi_pipe, hi_u8 i, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0xd0 + (i * sizeof(hi_u8)), data);
}
static __inline hi_u8 hi_ext_system_wdr_auto_mdthr_hig_gain_read(hi_vi_pipe vi_pipe, hi_u8 i)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0xd0 + (i * sizeof(hi_u8))));
}

/* texture_thd_wgt */
#define HI_EXT_SYSTEM_WDR_TEXTURE_THD_WGT 0x18
static __inline hi_void hi_ext_system_wdr_texture_thd_wgt_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0xe0, data);
}

static __inline hi_u8 hi_ext_system_wdr_texture_thd_wgt_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0xe0));
}

/* sfnr */
#define HI_EXT_SYSTEM_WDR_WDR_SFNR_EN_WGT 0x1
static __inline hi_void hi_ext_system_wdr_sfnr_en_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0xe1, data);
}

static __inline hi_u8 hi_ext_system_wdr_sfnr_en_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0xe1));
}

/* isp_wdr_forcelong_en */
#define HI_EXT_SYSTEM_WDR_FORCELONG_EN 0x1
static __inline hi_void hi_ext_system_wdr_forcelong_en_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0xe2, data);
}

static __inline hi_u8 hi_ext_system_wdr_forcelong_en_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0xe2));
}

/* isp_wdr_forcelong_low_thd */
#define HI_EXT_SYSTEM_WDR_FORCELONG_LOW_THD 0x1F4
static __inline hi_void hi_ext_system_wdr_forcelong_low_thd_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_wdr_base(vi_pipe) + 0xe4, data);
}

static __inline hi_u16 hi_ext_system_wdr_forcelong_low_thd_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_wdr_base(vi_pipe) + 0xe4));
}

/* isp_wdr_forcelong_high_thd */
#define HI_EXT_SYSTEM_WDR_FORCELONG_HIGH_THD 0x2BC
static __inline hi_void hi_ext_system_wdr_forcelong_high_thd_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_wdr_base(vi_pipe) + 0xe6, data);
}

static __inline hi_u16 hi_ext_system_wdr_forcelong_high_thd_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_wdr_base(vi_pipe) + 0xe6));
}

/* isp_wdr_shortcheck_thd */
#define HI_EXT_SYSTEM_WDR_SHORTCHECK_THD 0xc
static __inline hi_void hi_ext_system_wdr_shortcheck_thd_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_wdr_base(vi_pipe) + 0xe8, data);
}

static __inline hi_u16 hi_ext_system_wdr_shortcheck_thd_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_wdr_base(vi_pipe) + 0xe8));
}

/* isp_wdr_forcelong_en */
#define HI_EXT_SYSTEM_WDR_MOT2SIG_GWGT_WGT 0x18
static __inline hi_void hi_ext_system_wdr_mot2sig_gwgt_high_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0xea, data);
}

static __inline hi_u8 hi_ext_system_wdr_mot2sig_gwgt_high_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0xea));
}

#define HI_EXT_SYSTEM_WDR_MOT2SIG_CWGT_WGT 0x18
static __inline hi_void hi_ext_system_wdr_mot2sig_cwgt_high_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0xec, data);
}

static __inline hi_u8 hi_ext_system_wdr_mot2sig_cwgt_high_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0xec));
}

#define HI_EXT_SYSTEM_WDR_SHORTSIGMAL1_GWGT_WGT 16
static __inline hi_void hi_ext_system_wdr_shortsigmal1_gwgt_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0xf0, data);
}

static __inline hi_u8 hi_ext_system_wdr_shortsigmal1_gwgt_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0xf0));
}

#define HI_EXT_SYSTEM_WDR_SHORTSIGMAL2_GWGT_WGT 48

static __inline hi_void hi_ext_system_wdr_shortsigmal2_gwgt_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0xf2, data);
}

static __inline hi_u8 hi_ext_system_wdr_shortsigmal2_gwgt_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0xf2));
}

#define HI_EXT_SYSTEM_WDR_SHORTSIGMAL1_CWGT_WGT 16

static __inline hi_void hi_ext_system_wdr_shortsigmal1_cwgt_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0xf4, data);
}

static __inline hi_u8 hi_ext_system_wdr_shortsigmal1_cwgt_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0xf4));
}

#define HI_EXT_SYSTEM_WDR_SHORTSIGMAL2_CWGT_WGT 48

static __inline hi_void hi_ext_system_wdr_shortsigmal2_cwgt_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0xf6, data);
}

static __inline hi_u8 hi_ext_system_wdr_shortsigmal2_cwgt_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0xf6));
}

#define HI_EXT_SYSTEM_WDR_FUSIONSIGMA_CWGT0_WGT 16

static __inline hi_void hi_ext_system_wdr_fusionsigma_cwgt0_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0xf8, data);
}

static __inline hi_u8 hi_ext_system_wdr_fusionsigma_cwgt0_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0xf8));
}

#define HI_EXT_SYSTEM_WDR_FUSIONSIGMA_CWGT1_WGT 8

static __inline hi_void hi_ext_system_wdr_fusionsigma_cwgt1_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0xf9, data);
}

static __inline hi_u8 hi_ext_system_wdr_fusionsigma_cwgt1_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0xf9));
}

#define HI_EXT_SYSTEM_WDR_FUSIONSIGMA_GWGT0_WGT 16

static __inline hi_void hi_ext_system_wdr_fusionsigma_gwgt0_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0xfa, data);
}

static __inline hi_u8 hi_ext_system_wdr_fusionsigma_gwgt0_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0xfa));
}

#define HI_EXT_SYSTEM_WDR_FUSIONSIGMA_GWGT1_WGT 8

static __inline hi_void hi_ext_system_wdr_fusionsigma_gwgt1_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0xfb, data);
}

static __inline hi_u8 hi_ext_system_wdr_fusionsigma_gwgt1_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0xfb));
}

#define HI_EXT_SYSTEM_WDR_FUSION_BNR_STR_WGT 16
static __inline hi_void hi_ext_system_wdr_fusionbnrstr_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0xfc, data);
}
static __inline hi_u8 hi_ext_system_wdr_fusionbnrstr_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0xfc));
}

#define HI_EXT_SYSTEM_WDR_MOTION_BNR_STR_WGT 24

static __inline hi_void hi_ext_system_wdr_motionbnrstr_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0xfd, data);
}

static __inline hi_u8 hi_ext_system_wdr_motionbnrstr_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0xfd));
}

#define HI_EXT_SYSTEM_WDR_SHORTFRAME_NR_STR_WGT 48

static __inline hi_void hi_ext_system_wdr_shortframe_nrstr_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_wdr_base(vi_pipe) + 0xfe, data);
}
static __inline hi_u8 hi_ext_system_wdr_shortframe_nrstr_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_wdr_base(vi_pipe) + 0xfe));
}

/* WDR---U32 ARRAY--EXT_REGISTER */
/* FLICKER */
/* register: hi_ext_system_flicker_min_band_num */
/* flicker_min_band_num */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_flicker_min_band_num_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_flick_base(vi_pipe)  +  0X0, data);
}
static __inline hi_u8 hi_ext_system_flicker_min_band_num_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_flick_base(vi_pipe)  +  0X0);
}

/* register: hi_ext_system_flicker_min_valid_band_pcnt */
/* flicker_min_valid_band_pcnt */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_flicker_min_valid_band_pcnt_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_flick_base(vi_pipe)  +  0x1, data);
}
static __inline hi_u8 hi_ext_system_flicker_min_valid_band_pcnt_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_flick_base(vi_pipe)  +  0x1);
}

/* register: hi_ext_system_flicker_wave_diff1 */
/* flicker_wave_diff1 */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_flicker_wave_diff1_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_flick_base(vi_pipe)  +  0x2, data);
}
static __inline hi_u8 hi_ext_system_flicker_wave_diff1_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_flick_base(vi_pipe)  +  0x2);
}

/* register: hi_ext_system_flicker_wave_diff2 */
/* flicker_wave_diff2 */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_flicker_wave_diff2_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_flick_base(vi_pipe)  +  0x3, data);
}
static __inline hi_u8 hi_ext_system_flicker_wave_diff2_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_flick_base(vi_pipe)  +  0x3);
}

/* register: hi_ext_system_flicker_period */
/* flicker_period */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_flicker_period_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_flick_base(vi_pipe)  +  0x4, data);
}
static __inline hi_u8 hi_ext_system_flicker_period_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_flick_base(vi_pipe)  +  0x4);
}

/* register: hi_ext_system_flicker_gr_cnt */
/* flicker_gr_cnt */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_flicker_gr_cnt_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_flick_base(vi_pipe)  +  0x5, data);
}
static __inline hi_u8 hi_ext_system_flicker_gr_cnt_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_flick_base(vi_pipe)  +  0x5);
}

/* register: hi_ext_system_flicker_gb_cnt */
/* flicker_gb_cnt */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_flicker_gb_cnt_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_flick_base(vi_pipe)  +  0x6, data);
}
static __inline hi_u8 hi_ext_system_flicker_gb_cnt_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_flick_base(vi_pipe)  +  0x6);
}

/* register: hi_ext_system_flicker_result */
/* flicker_result */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_flicker_result_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_flick_base(vi_pipe)  +  0x7, data);
}
static __inline hi_u8 hi_ext_system_flicker_result_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_flick_base(vi_pipe)  +  0x7);
}

/* register: hi_ext_system_freq_result */
/* freq_result */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_freq_result_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_flick_base(vi_pipe)  +  0x8, data);
}
static __inline hi_u8 hi_ext_system_freq_result_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_flick_base(vi_pipe)  +  0x8);
}

static __inline hi_void hi_ext_system_flicker_over_cnt_thr_write(hi_vi_pipe vi_pipe, hi_u32 data)
{
    iowr_32direct(vreg_flick_base(vi_pipe)  +  0xc, data);
}
static __inline hi_u32 hi_ext_system_flicker_over_cnt_thr_read(hi_vi_pipe vi_pipe)
{
    return iord_32direct(vreg_flick_base(vi_pipe)  +  0xc);
}

static __inline hi_void hi_ext_system_flicker_over_thr_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_flick_base(vi_pipe)  +  0x10, data);
}
static __inline hi_u16 hi_ext_system_flicker_over_thr_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_flick_base(vi_pipe)  +  0x10);
}

/* lSC */
static __inline hi_void hi_ext_system_isp_mesh_shading_enable_write(hi_vi_pipe vi_pipe,  hi_u8 data)
{
    iowr_8direct(vreg_lsc_base(vi_pipe), data & 0x1);
}

static __inline hi_u8 hi_ext_system_isp_mesh_shading_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_lsc_base(vi_pipe)) & 0x1);
}

/* args: data (8-bit) */
static __inline hi_void hi_ext_system_isp_mesh_shading_lut_attr_updata_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_lsc_base(vi_pipe)  +  1, data);
}
static __inline hi_u8 hi_ext_system_isp_mesh_shading_lut_attr_updata_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_lsc_base(vi_pipe)  +  1);
}

/* args: data (8-bit) */
static __inline hi_void hi_ext_system_isp_mesh_shading_attr_updata_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_lsc_base(vi_pipe)  +  0x2, data);
}
static __inline hi_u8 hi_ext_system_isp_mesh_shading_attr_updata_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_lsc_base(vi_pipe)  +  0x2);
}

static __inline hi_void hi_ext_system_isp_mesh_shading_mesh_scale_write(hi_vi_pipe vi_pipe,  hi_u8 data)
{
    iowr_8direct(vreg_lsc_base(vi_pipe)  +  0x3, data);
}

static __inline hi_u8 hi_ext_system_isp_mesh_shading_mesh_scale_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_lsc_base(vi_pipe)  +  0x3));
}

static __inline hi_void hi_ext_system_isp_mesh_shading_mesh_strength_write(hi_vi_pipe vi_pipe,  hi_u16 data)
{
    iowr_16direct(vreg_lsc_base(vi_pipe)  +  0x4, data);
}

static __inline hi_u16 hi_ext_system_isp_mesh_shading_mesh_strength_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_lsc_base(vi_pipe)  +  0x4));
}

static __inline hi_void hi_ext_system_isp_fe_lsc_enable_write(hi_vi_pipe vi_pipe,  hi_u8 data)
{
    iowr_8direct(vreg_lsc_base(vi_pipe)  +  0x6, data & 0x1);
}

static __inline hi_u8 hi_ext_system_isp_fe_lsc_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_lsc_base(vi_pipe)  +  0x6) & 0x1);
}

/* length : 32;bytes: 16 * 2 */
static __inline hi_void hi_ext_system_isp_mesh_shading_xgrid_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u16 data)
{
    iowr_16direct(vreg_lsc_base(vi_pipe)  +  0x8  + (index * sizeof(hi_u16)), data);
}

static __inline hi_u16 hi_ext_system_isp_mesh_shading_xgrid_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return iord_16direct(vreg_lsc_base(vi_pipe)  +  0x8  + (index * sizeof(hi_u16)));
}

/* length: 16;bytes: 16 * 2 */
static __inline hi_void hi_ext_system_isp_mesh_shading_ygrid_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u16 data)
{
    iowr_16direct(vreg_lsc_base(vi_pipe)  +  0x28  + (index * sizeof(hi_u16)), data);
}

static __inline hi_u16 hi_ext_system_isp_mesh_shading_ygrid_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return iord_16direct(vreg_lsc_base(vi_pipe)  +  0x28  + (index * sizeof(hi_u16)));
}

/* bytes : 2 */
static __inline hi_void hi_ext_system_isp_mesh_shading_blendratio_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_lsc_base(vi_pipe)  +  0x48, data);
}

static __inline hi_u16 hi_ext_system_isp_mesh_shading_blendratio_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_lsc_base(vi_pipe)  +  0x48);
}

/* length:  1089;bytes: 1089 * 2 */
static __inline hi_void hi_ext_system_isp_mesh_shading_r_gain0_write(hi_vi_pipe vi_pipe, hi_u16 index, hi_u16 data)
{
    iowr_16direct(vreg_lsc_base(vi_pipe) + 0x50 + (index * sizeof(hi_u16)), data);
}

static __inline hi_u16 hi_ext_system_isp_mesh_shading_r_gain0_read(hi_vi_pipe vi_pipe, hi_u16 index)
{
    return iord_16direct(vreg_lsc_base(vi_pipe) + 0x50 + (index * sizeof(hi_u16)));
}

/* length:  1089;bytes: 1089 * 2 */
static __inline hi_void hi_ext_system_isp_mesh_shading_gr_gain0_write(hi_vi_pipe vi_pipe, hi_u16 index, hi_u16 data)
{
    iowr_16direct(vreg_lsc_base(vi_pipe) + 0x8D2 + (index * sizeof(hi_u16)), data);
}

static __inline hi_u16 hi_ext_system_isp_mesh_shading_gr_gain0_read(hi_vi_pipe vi_pipe, hi_u16 index)
{
    return iord_16direct(vreg_lsc_base(vi_pipe) + 0x8D2 + (index * sizeof(hi_u16)));
}

/* length: 1089;bytes: 1089 * 2 */
static __inline hi_void hi_ext_system_isp_mesh_shading_gb_gain0_write(hi_vi_pipe vi_pipe, hi_u16 index, hi_u16 data)
{
    iowr_16direct(vreg_lsc_base(vi_pipe) + 0x1154 + (index * sizeof(hi_u16)), data);
}

static __inline hi_u16 hi_ext_system_isp_mesh_shading_gb_gain0_read(hi_vi_pipe vi_pipe, hi_u16 index)
{
    return iord_16direct(vreg_lsc_base(vi_pipe) + 0x1154 + (index * sizeof(hi_u16)));
}

/* length: 1089;bytes: 1089 * 2 */
static __inline hi_void hi_ext_system_isp_mesh_shading_b_gain0_write(hi_vi_pipe vi_pipe, hi_u16 index, hi_u16 data)
{
    iowr_16direct(vreg_lsc_base(vi_pipe) + 0x19D6 + (index * sizeof(hi_u16)), data);
}

static __inline hi_u16 hi_ext_system_isp_mesh_shading_b_gain0_read(hi_vi_pipe vi_pipe, hi_u16 index)
{
    return iord_16direct(vreg_lsc_base(vi_pipe) + 0x19D6 + (index * sizeof(hi_u16)));
}

/* length: 1089;bytes: 1089 * 2 */
static __inline hi_void hi_ext_system_isp_mesh_shading_r_gain1_write(hi_vi_pipe vi_pipe, hi_u16 index, hi_u16 data)
{
    iowr_16direct(vreg_lsc_base(vi_pipe) + 0x2258 + (index * sizeof(hi_u16)), data);
}

static __inline hi_u16 hi_ext_system_isp_mesh_shading_r_gain1_read(hi_vi_pipe vi_pipe, hi_u16 index)
{
    return iord_16direct(vreg_lsc_base(vi_pipe) + 0x2258 + (index * sizeof(hi_u16)));
}

/* length: 1089;bytes: 1089 * 2 */
static __inline hi_void hi_ext_system_isp_mesh_shading_gr_gain1_write(hi_vi_pipe vi_pipe, hi_u16 index, hi_u16 data)
{
    iowr_16direct(vreg_lsc_base(vi_pipe) + 0x2ADA + (index * sizeof(hi_u16)), data);
}

static __inline hi_u16 hi_ext_system_isp_mesh_shading_gr_gain1_read(hi_vi_pipe vi_pipe, hi_u16 index)
{
    return iord_16direct(vreg_lsc_base(vi_pipe) + 0x2ADA + (index * sizeof(hi_u16)));
}

/* length: 1089;bytes: 1089 * 2 */
static __inline hi_void hi_ext_system_isp_mesh_shading_gb_gain1_write(hi_vi_pipe vi_pipe, hi_u16 index, hi_u16 data)
{
    iowr_16direct(vreg_lsc_base(vi_pipe) + 0x335C + (index * sizeof(hi_u16)), data);
}

static __inline hi_u16 hi_ext_system_isp_mesh_shading_gb_gain1_read(hi_vi_pipe vi_pipe, hi_u16 index)
{
    return iord_16direct(vreg_lsc_base(vi_pipe) + 0x335C + (index * sizeof(hi_u16)));
}

/* length: 1089;bytes: 1089 * 2 */
static __inline hi_void hi_ext_system_isp_mesh_shading_b_gain1_write(hi_vi_pipe vi_pipe, hi_u16 index, hi_u16 data)
{
    iowr_16direct(vreg_lsc_base(vi_pipe) + 0x3BDE + (index * sizeof(hi_u16)), data);
}

static __inline hi_u16 hi_ext_system_isp_mesh_shading_b_gain1_read(hi_vi_pipe vi_pipe, hi_u16 index)
{
    return iord_16direct(vreg_lsc_base(vi_pipe) + 0x3BDE + (index * sizeof(hi_u16)));
}

static __inline hi_void hi_ext_system_isp_mesh_shading_fe_lut_attr_updata_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_lsc_base(vi_pipe)  +  0x4460, data);
}
static __inline hi_u8 hi_ext_system_isp_mesh_shading_fe_lut_attr_updata_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_lsc_base(vi_pipe)  +  0x4460);
}

/* args: data (8-bit) */
static __inline hi_void hi_ext_system_isp_mesh_shading_fe_attr_updata_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_lsc_base(vi_pipe)  +  0x4461, data);
}
static __inline hi_u8 hi_ext_system_isp_mesh_shading_fe_attr_updata_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_lsc_base(vi_pipe)  +  0x4461);
}

/* length : 129; bytes : 129*2 = 258 = 0x102 */
static __inline hi_void hi_ext_system_isp_bnr_shading_r_gain_write(hi_vi_pipe vi_pipe,  hi_u8 index, hi_u16 data)
{
    iowr_16direct((vreg_lsc_base(vi_pipe) + 0x4462 + index * sizeof(hi_u16)), data);
}

static __inline hi_u16 hi_ext_system_isp_bnr_shading_r_gain_read(hi_vi_pipe vi_pipe, hi_u8 index)
{
    return (iord_16direct((vreg_lsc_base(vi_pipe) + 0x4462 + index * sizeof(hi_u16))));
}

/* length : 129; bytes : 129*2 = 258 = 0x102 */
static __inline hi_void hi_ext_system_isp_bnr_shading_gr_gain_write(hi_vi_pipe vi_pipe,  hi_u8 index, hi_u16 data)
{
    iowr_16direct((vreg_lsc_base(vi_pipe) + 0x4564 + index * sizeof(hi_u16)), data);
}

static __inline hi_u16 hi_ext_system_isp_bnr_shading_gr_gain_read(hi_vi_pipe vi_pipe, hi_u8 index)
{
    return (iord_16direct((vreg_lsc_base(vi_pipe) + 0x4564 + index * sizeof(hi_u16))));
}

/* length : 129; bytes : 129*2 = 258 = 0x102 */
static __inline hi_void hi_ext_system_isp_bnr_shading_gb_gain_write(hi_vi_pipe vi_pipe,  hi_u8 index, hi_u16 data)
{
    iowr_16direct((vreg_lsc_base(vi_pipe) + 0x4666 + index * sizeof(hi_u16)), data);
}

static __inline hi_u16 hi_ext_system_isp_bnr_shading_gb_gain_read(hi_vi_pipe vi_pipe, hi_u8 index)
{
    return (iord_16direct((vreg_lsc_base(vi_pipe) + 0x4666 + index * sizeof(hi_u16))));
}

/* length : 129; bytes : 129*2 = 258 = 0x102 */
static __inline hi_void hi_ext_system_isp_bnr_shading_b_gain_write(hi_vi_pipe vi_pipe,  hi_u8 index, hi_u16 data)
{
    iowr_16direct((vreg_lsc_base(vi_pipe) + 0x4768 + index * sizeof(hi_u16)), data);
}

static __inline hi_u16 hi_ext_system_isp_bnr_shading_b_gain_read(hi_vi_pipe vi_pipe, hi_u8 index)
{
    return (iord_16direct((vreg_lsc_base(vi_pipe) + 0x4768 + index * sizeof(hi_u16))));
}

/* ACS */
static __inline hi_void hi_ext_system_isp_acs_enable_write(hi_vi_pipe vi_pipe,  hi_u8 data)
{
    iowr_8direct(vreg_lsc_base(vi_pipe) + 0x4870, data & 0x1);
}

static __inline hi_u8 hi_ext_system_isp_acs_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_lsc_base(vi_pipe) + 0x4870) & 0x1);
}

static __inline hi_void hi_ext_system_isp_acs_y_strength_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_lsc_base(vi_pipe) + 0x4872, data);
}

static __inline hi_u16 hi_ext_system_isp_acs_y_strength_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_lsc_base(vi_pipe) + 0x4872);
}

static __inline hi_void hi_ext_system_isp_acs_run_interval_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_lsc_base(vi_pipe) + 0x4874, data);
}

static __inline hi_u16 hi_ext_system_isp_acs_run_interval_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_lsc_base(vi_pipe) + 0x4874);
}

static __inline hi_void hi_ext_system_isp_acs_lock_enable_write(hi_vi_pipe vi_pipe,  hi_u8 data)
{
    iowr_8direct(vreg_lsc_base(vi_pipe) + 0x4876, data & 0x1);
}

static __inline hi_u8 hi_ext_system_isp_acs_lock_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_lsc_base(vi_pipe) + 0x4876) & 0x1);
}

static __inline hi_void hi_ext_system_isp_acs_attr_updata_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_lsc_base(vi_pipe) + 0x4878, data & 0x1);
}

static __inline hi_u8 hi_ext_system_isp_acs_attr_updata_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_lsc_base(vi_pipe) + 0x4878) & 0x1);
}

static __inline hi_void hi_ext_system_isp_acs_lsc_lut_attr_updata_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_lsc_base(vi_pipe) + 0x487A, data);
}

static __inline hi_u8 hi_ext_system_isp_acs_lsc_lut_attr_updata_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_lsc_base(vi_pipe) + 0x487A);
}

/* RLSC */
static __inline hi_void hi_ext_system_isp_radial_shading_enable_write(hi_vi_pipe vi_pipe,  hi_u8 data)
{
    iowr_8direct(vreg_rlsc_base(vi_pipe) + 0x0, data & 0x1);
}

static __inline hi_u8 hi_ext_system_isp_radial_shading_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_rlsc_base(vi_pipe) + 0x0));
}

static __inline hi_void hi_ext_system_isp_radial_shading_scale_write(hi_vi_pipe vi_pipe,  hi_u8 data)
{
    iowr_8direct(vreg_rlsc_base(vi_pipe) + 0x6, data);
}

static __inline hi_u8 hi_ext_system_isp_radial_shading_scale_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_rlsc_base(vi_pipe) + 0x6));
}

static __inline hi_void hi_ext_system_isp_radial_shading_strength_write(hi_vi_pipe vi_pipe,  hi_u16 data)
{
    iowr_16direct(vreg_rlsc_base(vi_pipe) + 0x8, data);
}

static __inline hi_u16 hi_ext_system_isp_radial_shading_strength_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_rlsc_base(vi_pipe) + 0x8));
}

static __inline hi_void hi_ext_system_isp_radial_shading_coefupdate_write(hi_vi_pipe vi_pipe,  hi_u8 data)
{
    iowr_8direct(vreg_rlsc_base(vi_pipe) + 0xa, data);
}

static __inline hi_u8 hi_ext_system_isp_radial_shading_coefupdate_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_rlsc_base(vi_pipe) + 0xa));
}

static __inline hi_void hi_ext_system_isp_radial_shading_lutupdate_write(hi_vi_pipe vi_pipe,  hi_u8 data)
{
    iowr_8direct(vreg_rlsc_base(vi_pipe) + 0xb, data);
}

static __inline hi_u8 hi_ext_system_isp_radial_shading_lutupdate_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_rlsc_base(vi_pipe) + 0xb));
}

static __inline hi_void hi_ext_system_isp_radial_shading_lightmode_write(hi_vi_pipe vi_pipe,  hi_u8 data)
{
    iowr_8direct(vreg_rlsc_base(vi_pipe) + 0xc, data);
}

static __inline hi_u8 hi_ext_system_isp_radial_shading_lightmode_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_rlsc_base(vi_pipe) + 0xc));
}

static __inline hi_void hi_ext_system_isp_radial_shading_blendratio_write(hi_vi_pipe vi_pipe,  hi_u16 data)
{
    iowr_16direct(vreg_rlsc_base(vi_pipe) + 0xe, data);
}

static __inline hi_u16 hi_ext_system_isp_radial_shading_blendratio_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_rlsc_base(vi_pipe) + 0xe));
}

static __inline hi_void hi_ext_system_isp_radial_shading_lightinfo_write(hi_vi_pipe vi_pipe,  hi_u8 index, hi_u8 data)
{
    iowr_8direct((vreg_rlsc_base(vi_pipe) + 0x10 + index * sizeof(hi_u8)), data);
}

static __inline hi_u8 hi_ext_system_isp_radial_shading_lightinfo_read(hi_vi_pipe vi_pipe, hi_u8 index)
{
    return (iord_8direct((vreg_rlsc_base(vi_pipe) + 0x10 + index * sizeof(hi_u8))));
}

static __inline hi_void hi_ext_system_isp_radial_shading_centerrx_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_rlsc_base(vi_pipe) + 0x12, data);
}

static __inline hi_u16 hi_ext_system_isp_radial_shading_centerrx_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_rlsc_base(vi_pipe) + 0x12));
}

static __inline hi_void hi_ext_system_isp_radial_shading_centerry_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_rlsc_base(vi_pipe) + 0x14, data);
}

static __inline hi_u16 hi_ext_system_isp_radial_shading_centerry_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_rlsc_base(vi_pipe) + 0x14));
}

static __inline hi_void hi_ext_system_isp_radial_shading_centergrx_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_rlsc_base(vi_pipe) + 0x16, data);
}

static __inline hi_u16 hi_ext_system_isp_radial_shading_centergrx_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_rlsc_base(vi_pipe) + 0x16));
}

static __inline hi_void hi_ext_system_isp_radial_shading_centergry_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_rlsc_base(vi_pipe) + 0x18, data);
}

static __inline hi_u16 hi_ext_system_isp_radial_shading_centergry_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_rlsc_base(vi_pipe) + 0x18));
}

static __inline hi_void hi_ext_system_isp_radial_shading_centergbx_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_rlsc_base(vi_pipe) + 0x1a, data);
}

static __inline hi_u16 hi_ext_system_isp_radial_shading_centergbx_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_rlsc_base(vi_pipe) + 0x1a));
}

static __inline hi_void hi_ext_system_isp_radial_shading_centergby_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_rlsc_base(vi_pipe) + 0x1c, data);
}

static __inline hi_u16 hi_ext_system_isp_radial_shading_centergby_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_rlsc_base(vi_pipe) + 0x1c));
}

static __inline hi_void hi_ext_system_isp_radial_shading_centerbx_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_rlsc_base(vi_pipe) + 0x1e, data);
}

static __inline hi_u16 hi_ext_system_isp_radial_shading_centerbx_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_rlsc_base(vi_pipe) + 0x1e));
}

static __inline hi_void hi_ext_system_isp_radial_shading_centerby_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_rlsc_base(vi_pipe) + 0x20, data);
}

static __inline hi_u16 hi_ext_system_isp_radial_shading_centerby_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_rlsc_base(vi_pipe) + 0x20));
}

static __inline hi_void hi_ext_system_isp_radial_shading_offcenterr_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_rlsc_base(vi_pipe) + 0x22, data);
}

static __inline hi_u16 hi_ext_system_isp_radial_shading_offcenterr_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_rlsc_base(vi_pipe) + 0x22));
}

static __inline hi_void hi_ext_system_isp_radial_shading_offcentergr_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_rlsc_base(vi_pipe) + 0x24, data);
}

static __inline hi_u16 hi_ext_system_isp_radial_shading_offcentergr_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_rlsc_base(vi_pipe) + 0x24));
}

static __inline hi_void hi_ext_system_isp_radial_shading_offcentergb_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_rlsc_base(vi_pipe) + 0x26, data);
}

static __inline hi_u16 hi_ext_system_isp_radial_shading_offcentergb_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_rlsc_base(vi_pipe) + 0x26));
}

static __inline hi_void hi_ext_system_isp_radial_shading_offcenterb_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_rlsc_base(vi_pipe) + 0x28, data);
}

static __inline hi_u16 hi_ext_system_isp_radial_shading_offcenterb_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_rlsc_base(vi_pipe) + 0x28));
}

static __inline hi_void hi_ext_system_isp_radial_shading_r_gain0_write(hi_vi_pipe vi_pipe,  hi_u8 index, hi_u16 data)
{
    iowr_16direct((vreg_rlsc_base(vi_pipe) + 0x02a + index * sizeof(hi_u16)), data);
}

static __inline hi_u16 hi_ext_system_isp_radial_shading_r_gain0_read(hi_vi_pipe vi_pipe, hi_u8 index)
{
    return (iord_16direct((vreg_rlsc_base(vi_pipe) + 0x02a + index * sizeof(hi_u16))));
}

static __inline hi_void hi_ext_system_isp_radial_shading_r_gain1_write(hi_vi_pipe vi_pipe,  hi_u8 index, hi_u16 data)
{
    iowr_16direct((vreg_rlsc_base(vi_pipe) + 0x13a + index * sizeof(hi_u16)), data);
}

static __inline hi_u16 hi_ext_system_isp_radial_shading_r_gain1_read(hi_vi_pipe vi_pipe, hi_u8 index)
{
    return (iord_16direct((vreg_rlsc_base(vi_pipe) + 0x13a + index * sizeof(hi_u16))));
}

static __inline hi_void hi_ext_system_isp_radial_shading_r_gain2_write(hi_vi_pipe vi_pipe,  hi_u8 index, hi_u16 data)
{
    iowr_16direct((vreg_rlsc_base(vi_pipe) + 0x24a + index * sizeof(hi_u16)), data);
}

static __inline hi_u16 hi_ext_system_isp_radial_shading_r_gain2_read(hi_vi_pipe vi_pipe, hi_u8 index)
{
    return (iord_16direct((vreg_rlsc_base(vi_pipe) + 0x24a + index * sizeof(hi_u16))));
}

static __inline hi_void hi_ext_system_isp_radial_shading_gr_gain0_write(hi_vi_pipe vi_pipe,  hi_u8 index, hi_u16 data)
{
    iowr_16direct((vreg_rlsc_base(vi_pipe) + 0x35a + index * sizeof(hi_u16)), data);
}

static __inline hi_u16 hi_ext_system_isp_radial_shading_gr_gain0_read(hi_vi_pipe vi_pipe, hi_u8 index)
{
    return (iord_16direct((vreg_rlsc_base(vi_pipe) + 0x35a + index * sizeof(hi_u16))));
}

static __inline hi_void hi_ext_system_isp_radial_shading_gr_gain1_write(hi_vi_pipe vi_pipe,  hi_u8 index, hi_u16 data)
{
    iowr_16direct((vreg_rlsc_base(vi_pipe) + 0x46a + index * sizeof(hi_u16)), data);
}

static __inline hi_u16 hi_ext_system_isp_radial_shading_gr_gain1_read(hi_vi_pipe vi_pipe, hi_u8 index)
{
    return (iord_16direct((vreg_rlsc_base(vi_pipe) + 0x46a + index * sizeof(hi_u16))));
}

static __inline hi_void hi_ext_system_isp_radial_shading_gr_gain2_write(hi_vi_pipe vi_pipe,  hi_u8 index, hi_u16 data)
{
    iowr_16direct((vreg_rlsc_base(vi_pipe) + 0x57a + index * sizeof(hi_u16)), data);
}

static __inline hi_u16 hi_ext_system_isp_radial_shading_gr_gain2_read(hi_vi_pipe vi_pipe, hi_u8 index)
{
    return (iord_16direct((vreg_rlsc_base(vi_pipe) + 0x57a + index * sizeof(hi_u16))));
}

static __inline hi_void hi_ext_system_isp_radial_shading_gb_gain0_write(hi_vi_pipe vi_pipe,  hi_u8 index, hi_u16 data)
{
    iowr_16direct((vreg_rlsc_base(vi_pipe) + 0x68a + index * sizeof(hi_u16)), data);
}

static __inline hi_u16 hi_ext_system_isp_radial_shading_gb_gain0_read(hi_vi_pipe vi_pipe, hi_u8 index)
{
    return (iord_16direct((vreg_rlsc_base(vi_pipe) + 0x68a + index * sizeof(hi_u16))));
}

static __inline hi_void hi_ext_system_isp_radial_shading_gb_gain1_write(hi_vi_pipe vi_pipe,  hi_u8 index, hi_u16 data)
{
    iowr_16direct((vreg_rlsc_base(vi_pipe) + 0x79a + index * sizeof(hi_u16)), data);
}

static __inline hi_u16 hi_ext_system_isp_radial_shading_gb_gain1_read(hi_vi_pipe vi_pipe, hi_u8 index)
{
    return (iord_16direct((vreg_rlsc_base(vi_pipe) + 0x79a + index * sizeof(hi_u16))));
}

static __inline hi_void hi_ext_system_isp_radial_shading_gb_gain2_write(hi_vi_pipe vi_pipe,  hi_u8 index, hi_u16 data)
{
    iowr_16direct((vreg_rlsc_base(vi_pipe) + 0x8aa + index * sizeof(hi_u16)), data);
}

static __inline hi_u16 hi_ext_system_isp_radial_shading_gb_gain2_read(hi_vi_pipe vi_pipe, hi_u8 index)
{
    return (iord_16direct((vreg_rlsc_base(vi_pipe) + 0x8aa + index * sizeof(hi_u16))));
}

static __inline hi_void hi_ext_system_isp_radial_shading_b_gain0_write(hi_vi_pipe vi_pipe,  hi_u8 index, hi_u16 data)
{
    iowr_16direct((vreg_rlsc_base(vi_pipe) + 0x9ba + index * sizeof(hi_u16)), data);
}

static __inline hi_u16 hi_ext_system_isp_radial_shading_b_gain0_read(hi_vi_pipe vi_pipe, hi_u8 index)
{
    return (iord_16direct((vreg_rlsc_base(vi_pipe) + 0x9ba + index * sizeof(hi_u16))));
}

static __inline hi_void hi_ext_system_isp_radial_shading_b_gain1_write(hi_vi_pipe vi_pipe,  hi_u8 index, hi_u16 data)
{
    iowr_16direct((vreg_rlsc_base(vi_pipe) + 0xaca + index * sizeof(hi_u16)), data);
}

static __inline hi_u16 hi_ext_system_isp_radial_shading_b_gain1_read(hi_vi_pipe vi_pipe, hi_u8 index)
{
    return (iord_16direct((vreg_rlsc_base(vi_pipe) + 0xaca + index * sizeof(hi_u16))));
}

static __inline hi_void hi_ext_system_isp_radial_shading_b_gain2_write(hi_vi_pipe vi_pipe,  hi_u8 index, hi_u16 data)
{
    iowr_16direct((vreg_rlsc_base(vi_pipe) + 0xbda + index * sizeof(hi_u16)), data);
}

static __inline hi_u16 hi_ext_system_isp_radial_shading_b_gain2_read(hi_vi_pipe vi_pipe, hi_u8 index)
{
    return (iord_16direct((vreg_rlsc_base(vi_pipe) + 0xbda + index * sizeof(hi_u16))));
}

/* YUV SHARPEN */
/* sharpening core */
#define HI_EXT_SYSTEM_SHARPEN_MANU_MODE_DEFAULT 0
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_sharpen_manu_mode_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe)  +  0x000, data & 0x01);
}
static __inline hi_u8 hi_ext_system_sharpen_manu_mode_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe)  +  0x000) & 0x01);
}

#define     HI_EXT_SYSTEM_MANUAL_SHARPEN_EN_DEFAULT     1
static __inline hi_void hi_ext_system_manual_sharpen_en_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe)  +  0x002, data & 0x01);
}
static __inline hi_u8 hi_ext_system_manual_sharpen_en_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe)  +  0x002) & 0x01);
}

#define     HI_EXT_SYSTEM_SHARPEN_MPI_UPDATE_EN_DEFAULT     0
static __inline hi_void hi_ext_system_sharpen_mpi_update_en_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe)  +  0x003, data & 0x01);
}
static __inline hi_u8 hi_ext_system_sharpen_mpi_update_en_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe)  +  0x003) & 0x01);
}

#define     HI_EXT_SYSTEM_MANUAL_SHARPEN_TEXTURESTR_DEF     300
static __inline hi_void hi_ext_system_manual_sharpen_texture_str_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u16 data)
{
    iowr_16direct(vreg_sharpen_base(vi_pipe)  +  0x004  + (index * sizeof(hi_u16)), data & 0xFFF);
}
static __inline hi_u16 hi_ext_system_manual_sharpen_texture_str_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return (iord_16direct(vreg_sharpen_base(vi_pipe)  +  0x004  + (index * sizeof(hi_u16))) & 0xFFF);
}

#define     HI_EXT_SYSTEM_MANUAL_SHARPEN_EDGESTR_DEF        400
static __inline hi_void hi_ext_system_manual_sharpen_edge_str_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u16 data)
{
    iowr_16direct(vreg_sharpen_base(vi_pipe)  +  0x044  + (index * sizeof(hi_u16)), data & 0xFFF);
}
static __inline hi_u16 hi_ext_system_manual_sharpen_edge_str_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return (iord_16direct(vreg_sharpen_base(vi_pipe)  +  0x044  + (index * sizeof(hi_u16))) & 0xFFF);
}

#define     HI_EXT_SYSTEM_MANUAL_SHARPEN_TEXTUREFREQ_DEF        128
static __inline hi_void hi_ext_system_manual_sharpen_texture_freq_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_sharpen_base(vi_pipe)  +  0x084, data & 0xFFF);
}
static __inline hi_u16 hi_ext_system_manual_sharpen_texture_freq_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_sharpen_base(vi_pipe)  +  0x084) & 0xFFF);
}

#define     HI_EXT_SYSTEM_MANUAL_SHARPEN_EDGEFREQ_DEF       96
static __inline hi_void hi_ext_system_manual_sharpen_edge_freq_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_sharpen_base(vi_pipe)  +  0x086, data & 0xFFF);
}
static __inline hi_u16 hi_ext_system_manual_sharpen_edge_freq_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_sharpen_base(vi_pipe)  +  0x086) & 0xFFF);
}

#define     HI_EXT_SYSTEM_MANUAL_SHARPEN_OVERSHOOT_DEF     100
static __inline hi_void hi_ext_system_manual_sharpen_over_shoot_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe)  +  0x088, data & 0x7f);
}
static __inline hi_u8 hi_ext_system_manual_sharpen_over_shoot_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe)  +  0x088) & 0x7f);
}

#define     HI_EXT_SYSTEM_MANUAL_SHARPEN_UNDERSHOOT_DEF     127
static __inline hi_void hi_ext_system_manual_sharpen_under_shoot_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe)  +  0x089, data & 0x7f);
}
static __inline hi_u8 hi_ext_system_manual_sharpen_under_shoot_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe)  +  0x089) & 0x7f);
}

#define     HI_EXT_SYSTEM_MANUAL_SHARPEN_SHOOTSUPSTR_DEF        60
static __inline hi_void hi_ext_system_manual_sharpen_shoot_sup_str_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe)  +  0x08a, data);
}
static __inline hi_u8 hi_ext_system_manual_sharpen_shoot_sup_str_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe)  +  0x08a));
}

#define     HI_EXT_SYSTEM_MANUAL_SHARPEN_DETAILCTRL_DEF     128
static __inline hi_void hi_ext_system_manual_sharpen_detailctrl_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe)  +  0x08b, data);
}
static __inline hi_u8 hi_ext_system_manual_sharpen_detailctrl_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe)  +  0x08b));
}

#define     HI_EXT_SYSTEM_MANUAL_SHARPEN_EDGEFILTSTR_DEF        53
static __inline hi_void hi_ext_system_manual_sharpen_edge_filt_str_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe)  +  0x08c, data & 0x3f);
}
static __inline hi_u8 hi_ext_system_manual_sharpen_edge_filt_str_read(hi_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_sharpen_base(vi_pipe)  +  0x08c)) & 0x3f);
}

/* register: hi_ext_system_manual_sharpen_luma_wgt */
/* sharpening manual luma_wgt table length:32*1 BYTE */
/* args: data (8-bit) */
/* 32 multi addrs */
#define     HI_EXT_SYSTEM_MANUAL_SHARPEN_LUMAWGT_DEF        (HI_ISP_SHARPEN_LUMAWGT)
static __inline hi_void hi_ext_system_manual_sharpen_luma_wgt_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe)  +  0x08e  + (index * sizeof(hi_u8)), data & HI_ISP_SHARPEN_LUMAWGT_BIT);
}
static __inline hi_u8 hi_ext_system_manual_sharpen_luma_wgt_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe)  +  0x08e  + (index * sizeof(hi_u8))) & HI_ISP_SHARPEN_LUMAWGT_BIT);
}

/* r_gain: data (8-bit) */
#define     HI_EXT_SYSTEM_MANUAL_SHARPEN_RGAIN_DEF      (HI_ISP_SHARPEN_RGAIN)
static __inline hi_void hi_ext_system_manual_sharpen_r_gain_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe)  +  0xae, data & HI_ISP_SHARPEN_RGAIN_BIT);
}
static __inline hi_u8 hi_ext_system_manual_sharpen_r_gain_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe)  +  0xae)&HI_ISP_SHARPEN_RGAIN_BIT);
}

/* b_gain: data (8-bit) */
#define     HI_EXT_SYSTEM_MANUAL_SHARPEN_BGAIN_DEF      (HI_ISP_SHARPEN_BGAIN)
static __inline hi_void hi_ext_system_manual_sharpen_b_gain_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe)  +  0xaf, data & HI_ISP_SHARPEN_BGAIN_BIT);
}
static __inline hi_u8 hi_ext_system_manual_sharpen_b_gain_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe)  +  0xaf)&HI_ISP_SHARPEN_BGAIN_BIT);
}

/* skin_gain: data (8-bit) */
#define     HI_EXT_SYSTEM_MANUAL_SHARPEN_SKINGAIN_DEF       31
static __inline hi_void hi_ext_system_manual_sharpen_skin_gain_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe)  +  0xb0, data & 0x1f);
}
static __inline hi_u8 hi_ext_system_manual_sharpen_skin_gain_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe)  +  0xb0) & 0x1f);
}

/* edge_filt_max_cap: data (8-bit) */
#define     HI_EXT_SYSTEM_MANUAL_SHARPEN_EDGEFILTMAXCAP_DEF        (HI_ISP_SHARPEN_EdgeFiltMaxCap)
static __inline hi_void hi_ext_system_manual_sharpen_edge_filt_max_cap_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe)  +  0xb1, data & HI_ISP_SHARPEN_EdgeFiltMaxCap_BIT);
}
static __inline hi_u8 hi_ext_system_manual_sharpen_edge_filt_max_cap_read(hi_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_sharpen_base(vi_pipe)  +  0xb1)) & HI_ISP_SHARPEN_EdgeFiltMaxCap_BIT);
}

/* auto 16 */
/* auoto 32 x 16 multi addrs */
static __inline hi_void hi_ext_system_sharpen_texture_str_write(hi_vi_pipe vi_pipe, hi_u16 index, hi_u16 data)
{
    iowr_16direct((vreg_sharpen_base(vi_pipe)  +  0xb2  +  index * sizeof(hi_u16)), data & 0xFFF);
}
static __inline hi_u16 hi_ext_system_sharpen_texture_str_read(hi_vi_pipe vi_pipe,  hi_u16 index)
{
    return ((iord_16direct((vreg_sharpen_base(vi_pipe)  +  0xb2  +  index * sizeof(hi_u16)))) & 0xFFF);
}

/* auoto 32 x 16 multi addrs */
static __inline hi_void hi_ext_system_sharpen_edge_str_write(hi_vi_pipe vi_pipe, hi_u16 index, hi_u16 data)
{
    iowr_16direct((vreg_sharpen_base(vi_pipe)  +  0x4b2  +  index * sizeof(hi_u16)), data & 0xFFF);
}
static __inline hi_u16 hi_ext_system_sharpen_edge_str_read(hi_vi_pipe vi_pipe,  hi_u16 index)
{
    return ((iord_16direct((vreg_sharpen_base(vi_pipe)  +  0x4b2  +  index * sizeof(hi_u16)))) & 0xFFF);
}

/* auoto 16 multi addrs */
static __inline hi_void hi_ext_system_sharpen_texture_freq_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u16 data)
{
    iowr_16direct((vreg_sharpen_base(vi_pipe)  +  0x8b2  +  index * sizeof(hi_u16)), data & 0xFFF);
}
static __inline hi_u16 hi_ext_system_sharpen_texture_freq_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return ((iord_16direct((vreg_sharpen_base(vi_pipe)  +  0x8b2  +  index * sizeof(hi_u16)))) & 0xFFF);
}

/* auoto 16 multi addrs */
static __inline hi_void hi_ext_system_sharpen_edge_freq_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u16 data)
{
    iowr_16direct((vreg_sharpen_base(vi_pipe)  +  0x8d2  +  index * sizeof(hi_u16)), data & 0xFFF);
}
static __inline hi_u16 hi_ext_system_sharpen_edge_freq_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return ((iord_16direct((vreg_sharpen_base(vi_pipe)  +  0x8d2  +  index * sizeof(hi_u16)))) & 0xFFF);
}

/* auoto 16 multi addrs */
static __inline hi_void hi_ext_system_sharpen_over_shoot_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u8 data)
{
    iowr_8direct((vreg_sharpen_base(vi_pipe)  +  0x8f2  +  index * sizeof(hi_u8)), data & 0x7f);
}
static __inline hi_u8 hi_ext_system_sharpen_over_shoot_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return ((iord_8direct((vreg_sharpen_base(vi_pipe)  +  0x8f2  +  index * sizeof(hi_u8)))) & 0x7f);
}

/* auoto 16 multi addrs */
static __inline hi_void hi_ext_system_sharpen_under_shoot_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u8 data)
{
    iowr_8direct((vreg_sharpen_base(vi_pipe)  +  0x902  +  index * sizeof(hi_u8)), data & 0x7f);
}
static __inline hi_u8 hi_ext_system_sharpen_under_shoot_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return ((iord_8direct((vreg_sharpen_base(vi_pipe)  +  0x902  +  index * sizeof(hi_u8)))) & 0x7f);
}

/* auoto 16 multi addrs */
static __inline hi_void hi_ext_system_sharpen_shoot_sup_str_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u8 data)
{
    iowr_8direct((vreg_sharpen_base(vi_pipe)  +  0x912  +  index * sizeof(hi_u8)), data);
}
static __inline hi_u8 hi_ext_system_sharpen_shoot_sup_str_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return ((iord_8direct((vreg_sharpen_base(vi_pipe)  +  0x912  +  index * sizeof(hi_u8)))));
}

/* auoto 16 multi addrs */
static __inline hi_void hi_ext_system_sharpen_detailctrl_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u8 data)
{
    iowr_8direct((vreg_sharpen_base(vi_pipe)  +  0x922  +  index * sizeof(hi_u8)), data);
}
static __inline hi_u8 hi_ext_system_sharpen_detailctrl_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return ((iord_8direct((vreg_sharpen_base(vi_pipe)  +  0x922  +  index * sizeof(hi_u8)))));
}

/* auoto 16 multi addrs */
static __inline hi_void hi_ext_system_sharpen_r_gain_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u8 data)
{
    iowr_8direct((vreg_sharpen_base(vi_pipe)  +  0x932  +  index * sizeof(hi_u8)), data & HI_ISP_SHARPEN_RGAIN_BIT);
}
static __inline hi_u8 hi_ext_system_sharpen_r_gain_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return ((iord_8direct((vreg_sharpen_base(vi_pipe)  +  0x932  +  index * sizeof(hi_u8))))&HI_ISP_SHARPEN_RGAIN_BIT);
}

/* auoto 16 multi addrs */
static __inline hi_void hi_ext_system_sharpen_b_gain_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u8 data)
{
    iowr_8direct((vreg_sharpen_base(vi_pipe)  +  0x942  +  index * sizeof(hi_u8)), data & HI_ISP_SHARPEN_BGAIN_BIT);
}
static __inline hi_u8 hi_ext_system_sharpen_b_gain_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return ((iord_8direct((vreg_sharpen_base(vi_pipe)  +  0x942  +  index * sizeof(hi_u8))))&HI_ISP_SHARPEN_BGAIN_BIT);
}

/* auoto 16 multi addrs */
static __inline hi_void hi_ext_system_sharpen_skin_gain_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u8 data)
{
    iowr_8direct((vreg_sharpen_base(vi_pipe)  +  0x952  +  index * sizeof(hi_u8)), data & 0x1f);
}
static __inline hi_u8 hi_ext_system_sharpen_skin_gain_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return ((iord_8direct((vreg_sharpen_base(vi_pipe)  +  0x952  +  index * sizeof(hi_u8)))) & 0x1f);
}

/* auoto 16 multi addrs */
static __inline hi_void hi_ext_system_sharpen_edge_filt_str_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u8 data)
{
    iowr_8direct((vreg_sharpen_base(vi_pipe)  +  0x962  +  index * sizeof(hi_u8)), data & 0x3f);
}
static __inline hi_u8 hi_ext_system_sharpen_edge_filt_str_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return ((iord_8direct((vreg_sharpen_base(vi_pipe)  +  0x962  +  index * sizeof(hi_u8)))) & 0x3f);
}

/* auoto 32 x 16 multi addrs */
static __inline hi_void hi_ext_system_sharpen_luma_wgt_write(hi_vi_pipe vi_pipe, hi_u16 index, hi_u8 data)
{
    iowr_8direct((vreg_sharpen_base(vi_pipe) + 0x972 +  index * sizeof(hi_u8)), data & HI_ISP_SHARPEN_LUMAWGT_BIT);
}
static __inline hi_u16 hi_ext_system_sharpen_luma_wgt_read(hi_vi_pipe vi_pipe,  hi_u16 index)
{
    return ((iord_8direct((vreg_sharpen_base(vi_pipe) + 0x972 + index * sizeof(hi_u8)))) & HI_ISP_SHARPEN_LUMAWGT_BIT);
}

/* new add manual */
#define     HI_EXT_SYSTEM_MANUAL_SHARPEN_DETAILCTRLTHR_DEF     160

static __inline hi_void hi_ext_system_manual_sharpen_detailctrl_thr_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe)  +  0xB80, data);
}
static __inline hi_u8 hi_ext_system_manual_sharpen_detailctrl_thr_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe)  +  0xB80));
}

#define     HI_EXT_SYSTEM_MANUAL_SHARPEN_SHOOTSUPADJ_DEF     9

static __inline hi_void hi_ext_system_manual_sharpen_shoot_sup_adj_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe)  +  0xB81, data & 0x0f);
}
static __inline hi_u8 hi_ext_system_manual_sharpen_shoot_sup_adj_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe)  +  0xB81) & 0x0f);
}

#define     HI_EXT_SYSTEM_MANUAL_SHARPEN_MAXSHARPGAIN_DEF     160

static __inline hi_void hi_ext_system_manual_sharpen_max_sharp_gain_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_sharpen_base(vi_pipe)  +  0xB82, data & 0x7FF);
}
static __inline hi_u16 hi_ext_system_manual_sharpen_max_sharp_gain_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_sharpen_base(vi_pipe)  +  0xB82) & 0x7FF);
}

#define     HI_EXT_SYSTEM_MANUAL_SHARPEN_SKINUMIN_DEF     100

static __inline hi_void hi_ext_system_manual_sharpen_skin_umin_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe)  +  0xB86, data);
}
static __inline hi_u8 hi_ext_system_manual_sharpen_skin_umin_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe)  +  0xB86));
}

#define     HI_EXT_SYSTEM_MANUAL_SHARPEN_SKINVMIN_DEF     135

static __inline hi_void hi_ext_system_manual_sharpen_skin_vmin_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe)  +  0xB87, data);
}
static __inline hi_u8 hi_ext_system_manual_sharpen_skin_vmin_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe)  +  0xB87));
}

#define     HI_EXT_SYSTEM_MANUAL_SHARPEN_SKINUMAX_DEF     127

static __inline hi_void hi_ext_system_manual_sharpen_skin_umax_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe)  +  0xB88, data);
}
static __inline hi_u8 hi_ext_system_manual_sharpen_skin_umax_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe)  +  0xB88));
}

#define     HI_EXT_SYSTEM_MANUAL_SHARPEN_SKINVMAX_DEF     160

static __inline hi_void hi_ext_system_manual_sharpen_skin_vmax_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe)  +  0xB89, data);
}
static __inline hi_u8 hi_ext_system_manual_sharpen_skin_vmax_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe) + 0xB89));
}
/* g_gain: data (8-bit) */
#define     HI_EXT_SYSTEM_MANUAL_SHARPEN_GGAIN_DEF      (HI_ISP_SHARPEN_GGAIN)

static __inline hi_void hi_ext_system_manual_sharpen_g_gain_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe) + 0xB8A, data & HI_ISP_SHARPEN_GGAIN_BIT);
}
static __inline hi_u8 hi_ext_system_manual_sharpen_g_gain_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe) + 0xB8A) & HI_ISP_SHARPEN_GGAIN_BIT);
}
#define     HI_EXT_SYSTEM_MANUAL_SHARPEN_WEAKDETAILGAIN_DEF     0
static __inline hi_void hi_ext_system_manual_sharpen_weak_detail_gain_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_sharpen_base(vi_pipe)  +  0xB8b, data & 0x7F);
}
static __inline hi_u8 hi_ext_system_manual_sharpen_weak_detail_gain_read(hi_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_sharpen_base(vi_pipe)  +  0xB8b)) & 0x7F);
}
/* new add auto */
static __inline hi_void hi_ext_system_sharpen_detailctrl_thr_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u8 data)
{
    iowr_8direct((vreg_sharpen_base(vi_pipe)  +  0xB8C  +  index * sizeof(hi_u8)), data);
}
static __inline hi_u8 hi_ext_system_sharpen_detailctrl_thr_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return ((iord_8direct((vreg_sharpen_base(vi_pipe)  +  0xB8C  +  index * sizeof(hi_u8)))));
}

static __inline hi_void hi_ext_system_sharpen_max_sharp_gain_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u16 data)
{
    iowr_16direct((vreg_sharpen_base(vi_pipe)  +  0xBaC  +  index * sizeof(hi_u16)), data & 0x7ff);
}
static __inline hi_u16 hi_ext_system_sharpen_max_sharp_gain_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return ((iord_16direct((vreg_sharpen_base(vi_pipe)  +  0xBaC  +  index * sizeof(hi_u16)))) & 0x7ff);
}

static __inline hi_void hi_ext_system_sharpen_shoot_sup_adj_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u8 data)
{
    iowr_8direct((vreg_sharpen_base(vi_pipe)  +  0xBCC  +  index * sizeof(hi_u8)), data & 0x0f);
}
static __inline hi_u8 hi_ext_system_sharpen_shoot_sup_adj_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return ((iord_8direct((vreg_sharpen_base(vi_pipe)  +  0xBCC  +  index * sizeof(hi_u8)))) & 0x0f);
}

static __inline hi_void hi_ext_system_sharpen_weak_detail_gain_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u8 data)
{
    iowr_8direct((vreg_sharpen_base(vi_pipe)  +  0xBDC  +  index * sizeof(hi_u8)), data & 0x7f);
}
static __inline hi_u8 hi_ext_system_sharpen_weak_detail_gain_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return ((iord_8direct((vreg_sharpen_base(vi_pipe)  +  0xBDC  +  index * sizeof(hi_u8)))) & 0x7f);
}
static __inline hi_void hi_ext_system_sharpen_g_gain_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u8 data)
{
    iowr_8direct((vreg_sharpen_base(vi_pipe)  +  0xBEC  +  index * sizeof(hi_u8)), data & HI_ISP_SHARPEN_GGAIN_BIT);
}
static __inline hi_u8 hi_ext_system_sharpen_g_gain_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return ((iord_8direct((vreg_sharpen_base(vi_pipe)  +  0xBEC  +  index * sizeof(hi_u8))))&HI_ISP_SHARPEN_GGAIN_BIT);
}

/* auoto 16 multi addrs */
static __inline hi_void hi_ext_system_sharpen_edge_filt_max_cap_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u8 data)
{
    iowr_8direct((vreg_sharpen_base(vi_pipe) + 0xBFC + index * sizeof(hi_u8)),
                 data & HI_ISP_SHARPEN_EdgeFiltMaxCap_BIT);
}
static __inline hi_u8 hi_ext_system_sharpen_edge_filt_max_cap_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return (iord_8direct(vreg_sharpen_base(vi_pipe) + 0xBFC + index * sizeof(hi_u8)) &
            HI_ISP_SHARPEN_EdgeFiltMaxCap_BIT);
}

/* edge_mark */
static __inline hi_void hi_ext_system_manual_edgemark_en_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_edgemark_base(vi_pipe), data & 0x01);
}
static __inline hi_u8 hi_ext_system_manual_edgemark_en_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_edgemark_base(vi_pipe)) & 0x01);
}

static __inline hi_void hi_ext_system_manual_edgemark_threshold_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_edgemark_base(vi_pipe)  +  0x1, data);
}
static __inline hi_u8 hi_ext_system_manual_edgemark_threshold_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_edgemark_base(vi_pipe)  +  0x1));
}

static __inline hi_void hi_ext_system_edgemark_mpi_update_en_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_edgemark_base(vi_pipe)  +  0x2, data & 0x01);
}
static __inline hi_u8 hi_ext_system_edgemark_mpi_update_en_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_edgemark_base(vi_pipe)  + 0x2) & 0x01);
}

static __inline hi_void hi_ext_system_manual_edgemark_color_write(hi_vi_pipe vi_pipe, hi_u32 data)
{
    iowr_32direct(vreg_edgemark_base(vi_pipe)  +  0x4, data & 0xFFFFFF);
}
static __inline hi_u32 hi_ext_system_manual_edgemark_color_read(hi_vi_pipe vi_pipe)
{
    return (iord_32direct(vreg_edgemark_base(vi_pipe)  +  0x4) & 0xFFFFFF);
}

/* high light constraint */
static __inline hi_void hi_ext_system_manual_hlc_en_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_hlc_base(vi_pipe), data & 0x01);
}
static __inline hi_u8 hi_ext_system_manual_hlc_en_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_hlc_base(vi_pipe)) & 0x01);
}

static __inline hi_void hi_ext_system_manual_hlc_lumathr_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_hlc_base(vi_pipe)  +  0x1, data);
}
static __inline hi_u8 hi_ext_system_manual_hlc_lumathr_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_hlc_base(vi_pipe)  +  0x1));
}

static __inline hi_void hi_ext_system_manual_hlc_lumatarget_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_hlc_base(vi_pipe)  +  0x2, data);
}
static __inline hi_u8 hi_ext_system_manual_hlc_lumatarget_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_hlc_base(vi_pipe)  +  0x2));
}

static __inline hi_void hi_ext_system_hlc_mpi_update_en_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_hlc_base(vi_pipe)  +  0x3, data & 0x01);
}
static __inline hi_u8 hi_ext_system_hlc_mpi_update_en_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_hlc_base(vi_pipe)  + 0x3) & 0x01);
}

/* CAC */
/* register: hi_ext_system_local_cac_enable */
/* local_cac_enable */
static __inline hi_void hi_ext_system_local_cac_enable_write(hi_vi_pipe vi_pipe,  hi_u8 data)
{
    iowr_8direct(vreg_lcac_base(vi_pipe), data & 0x1);
}

static __inline hi_u16 hi_ext_system_local_cac_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_lcac_base(vi_pipe)) & 0x1);
}
/* register: hi_ext_system_local_cac_manual_mode_enable */
/* local_cac_manual_mode_enable */
static __inline hi_void hi_ext_system_local_cac_manual_mode_enable_write(hi_vi_pipe vi_pipe,  hi_u8 data)
{
    iowr_8direct(vreg_lcac_base(vi_pipe)  +  1, data & 0x1);
}

static __inline hi_u8 hi_ext_system_local_cac_manual_mode_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_lcac_base(vi_pipe)  +  1) & 0x1);
}

/* register: hi_ext_system_local_cac_purple_det_range */
/* local_cac_purple_det_range */
static __inline hi_void hi_ext_system_local_cac_purple_det_range_write(hi_vi_pipe vi_pipe,  hi_u16 data)
{
    iowr_16direct(vreg_lcac_base(vi_pipe)  +  0x2, data);
}

static __inline hi_u16 hi_ext_system_local_cac_purple_det_range_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_lcac_base(vi_pipe)  +  0x2);
}

/* register: hi_ext_system_local_cac_purple_var_thr */
/* local_cac_purple_var_thr */
static __inline hi_void hi_ext_system_local_cac_purple_var_thr_write(hi_vi_pipe vi_pipe,  hi_u16 data)
{
    iowr_16direct(vreg_lcac_base(vi_pipe)  +  0x4, data);
}

static __inline hi_u16 hi_ext_system_local_cac_purple_var_thr_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_lcac_base(vi_pipe)  +  0x4);
}

/* register: hi_ext_system_local_cac_manual_cb_str */
/* local_cac_manual_cb */
static __inline hi_void hi_ext_system_local_cac_manual_cb_str_write(hi_vi_pipe vi_pipe,  hi_u8 data)
{
    iowr_8direct(vreg_lcac_base(vi_pipe)  +  0x6, data);
}

static __inline hi_u8 hi_ext_system_local_cac_manual_cb_str_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_lcac_base(vi_pipe)  +  0x6);
}

/* register: hi_ext_system_local_cac_manual_cr_str */
/* local_cac_manual_cr */
static __inline hi_void hi_ext_system_local_cac_manual_cr_str_write(hi_vi_pipe vi_pipe,  hi_u8 data)
{
    iowr_8direct(vreg_lcac_base(vi_pipe)  +  0x7, data);
}

static __inline hi_u8 hi_ext_system_local_cac_manual_cr_str_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_lcac_base(vi_pipe)  +  0x7);
}

/* register: hi_ext_system_local_cac_cb_str_table */
/* local_cac_cb_str_table */
static __inline hi_void hi_ext_system_local_cac_auto_cb_str_table_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u8 data)
{
    iowr_8direct(vreg_lcac_base(vi_pipe)  +  0x8  + (index * sizeof(hi_u8)), data);
}

static __inline hi_u8 hi_ext_system_local_cac_auto_cb_str_table_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return iord_8direct(vreg_lcac_base(vi_pipe)  +  0x8  + (index * sizeof(hi_u8)));
}

/* register: hi_ext_system_local_cac_cr_str_table */
/* local_cac_cr_str_table */
static __inline hi_void hi_ext_system_local_cac_auto_cr_str_table_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u8 data)
{
    iowr_8direct(vreg_lcac_base(vi_pipe)  +  0x18  + (index * sizeof(hi_u8)), data);
}

static __inline hi_u8 hi_ext_system_local_cac_auto_cr_str_table_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return iord_8direct(vreg_lcac_base(vi_pipe)  +  0x18  + (index * sizeof(hi_u8)));
}

/* register: hi_ext_system_local_cac_coef_update_en */
/* local_cac_coef_update_en */
static __inline hi_void hi_ext_system_local_cac_coef_update_en_write(hi_vi_pipe vi_pipe,  hi_u8 data)
{
    iowr_8direct(vreg_lcac_base(vi_pipe)  +  0x28, data & 0x1);
}

static __inline hi_u8 hi_ext_system_local_cac_coef_update_en_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_lcac_base(vi_pipe)  +  0x28) & 0x1);
}

/* register: hi_ext_system_local_cac_luma_high_cnt_thr */
static __inline hi_void hi_ext_system_local_cac_luma_high_cnt_thr_write(hi_vi_pipe vi_pipe,  hi_u8 data)
{
    iowr_8direct(vreg_lcac_base(vi_pipe)  +  0x29, data);
}

static __inline hi_u8 hi_ext_system_local_cac_luma_high_cnt_thr_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_lcac_base(vi_pipe)  +  0x29);
}

/* register: hi_ext_system_local_cac_cb_cnt_high_thr */
static __inline hi_void hi_ext_system_local_cac_cb_cnt_high_thr_write(hi_vi_pipe vi_pipe,    hi_u8 data)
{
    iowr_8direct(vreg_lcac_base(vi_pipe)  +  0x2a, data);
}

static __inline hi_u8 hi_ext_system_local_cac_cb_cnt_high_thr_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_lcac_base(vi_pipe)  +  0x2a);
}
/* register: hi_ext_system_local_cac_cb_cnt_low_thr */
static __inline hi_void hi_ext_system_local_cac_cb_cnt_low_thr_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_lcac_base(vi_pipe)  +  0x2b, data);
}

static __inline hi_u8 hi_ext_system_local_cac_cb_cnt_low_thr_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_lcac_base(vi_pipe)  +  0x2b);
}

/* register: hi_ext_system_local_cac_luma_thr */
static __inline hi_void hi_ext_system_local_cac_luma_thr_write(hi_vi_pipe vi_pipe,   hi_u16 data)
{
    iowr_16direct(vreg_lcac_base(vi_pipe)  + 0x2c, data);
}

static __inline hi_u16 hi_ext_system_local_cac_luma_thr_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_lcac_base(vi_pipe) +  0x2c);
}

/* register: hi_ext_system_local_cac_cb_thr */
static __inline hi_void hi_ext_system_local_cac_cb_thr_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_lcac_base(vi_pipe)  + 0x2e, data);
}

static __inline hi_u16 hi_ext_system_local_cac_cb_thr_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_lcac_base(vi_pipe) +  0x2e);
}

/* register: hi_ext_system_local_cac_bld_avg_cur */
static __inline hi_void hi_ext_system_local_cac_bld_avg_cur_write(hi_vi_pipe vi_pipe,    hi_u8 data)
{
    iowr_8direct(vreg_lcac_base(vi_pipe)  +  0x30, data);
}

static __inline hi_u8 hi_ext_system_local_cac_bld_avg_cur_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_lcac_base(vi_pipe)  +  0x30);
}

/* register: hi_ext_system_local_cac_defcolor_cr */
static __inline hi_void hi_ext_system_local_cac_defcolor_cr_write(hi_vi_pipe vi_pipe,    hi_u8 data)
{
    iowr_8direct(vreg_lcac_base(vi_pipe)  +  0x31, data);
}

static __inline hi_u8 hi_ext_system_local_cac_defcolor_cr_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_lcac_base(vi_pipe)  +  0x31);
}

/* register: hi_ext_system_local_cac_defcolor_cb */
static __inline hi_void hi_ext_system_local_cac_defcolor_cb_write(hi_vi_pipe vi_pipe,    hi_u8 data)
{
    iowr_8direct(vreg_lcac_base(vi_pipe)  +  0x32, data);
}

static __inline hi_u8 hi_ext_system_local_cac_defcolor_cb_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_lcac_base(vi_pipe)  +  0x32);
}


/* Register: hi_ext_system_local_cac_r_thd_table */
static __inline HI_VOID hi_ext_system_local_cac_r_thd_table_write(hi_vi_pipe vi_pipe, hi_u8 i, hi_u16 data)
{
    iowr_16direct(vreg_lcac_base(vi_pipe)  +  0x34  + (i * sizeof(hi_u16)), data);
}

static __inline hi_u16 hi_ext_system_local_cac_r_thd_table_read(hi_vi_pipe vi_pipe, hi_u8 i)
{
    return iord_16direct(vreg_lcac_base(vi_pipe)  +  0x34  + (i * sizeof(hi_u16)));
}


/* Register: hi_ext_system_local_cac_g_thd_table */
static __inline HI_VOID hi_ext_system_local_cac_g_thd_table_write(hi_vi_pipe vi_pipe, hi_u8 i, hi_u16 data)
{
    iowr_16direct(vreg_lcac_base(vi_pipe)  +  0x3a  + (i * sizeof(hi_u16)), data);
}

static __inline hi_u16 hi_ext_system_local_cac_g_thd_table_read(hi_vi_pipe vi_pipe, hi_u8 i)
{
    return iord_16direct(vreg_lcac_base(vi_pipe)  +  0x3a  + (i * sizeof(hi_u16)));
}


/* Register: hi_ext_system_local_cac_b_thd_table */
static __inline HI_VOID hi_ext_system_local_cac_b_thd_table_write(hi_vi_pipe vi_pipe, hi_u8 i, hi_u16 data)
{
    iowr_16direct(vreg_lcac_base(vi_pipe)  +  0x40  + (i * sizeof(hi_u16)), data);
}

static __inline hi_u16 hi_ext_system_local_cac_b_thd_table_read(hi_vi_pipe vi_pipe, hi_u8 i)
{
    return iord_16direct(vreg_lcac_base(vi_pipe)  +  0x40  + (i * sizeof(hi_u16)));
}


/* Register: hi_ext_system_local_cac_luma_thd_table */
static __inline HI_VOID hi_ext_system_local_cac_luma_thd_table_write(hi_vi_pipe vi_pipe, hi_u8 i, hi_u16 data)
{
    iowr_16direct(vreg_lcac_base(vi_pipe)  +  0x46  + (i * sizeof(hi_u16)), data);
}

static __inline hi_u16 hi_ext_system_local_cac_luma_thd_table_read(hi_vi_pipe vi_pipe, hi_u8 i)
{
    return iord_16direct(vreg_lcac_base(vi_pipe)  +  0x46  + (i * sizeof(hi_u16)));
}


/* Register: hi_ext_system_local_cac_luma_thd_table */
static __inline HI_VOID hi_ext_system_local_cac_cbcr_ratio_table_write(hi_vi_pipe vi_pipe, hi_u8 i, hi_s16 data)
{
    iowr_16direct(vreg_lcac_base(vi_pipe)  +  0x4c  + (i * sizeof(hi_s16)), data);
}

static __inline hi_s16 hi_ext_system_local_cac_cbcr_ratio_table_read(hi_vi_pipe vi_pipe, hi_u8 i)
{
    return iord_16direct(vreg_lcac_base(vi_pipe)  +  0x4c  + (i * sizeof(hi_s16)));
}

/* register: hi_ext_system_global_cac_enable */
/* global_cac_enable */
static __inline hi_void hi_ext_system_global_cac_enable_write(hi_vi_pipe vi_pipe,  hi_u8 data)
{
    iowr_8direct(vreg_gcac_base(vi_pipe), data & 0x1);
}

static __inline hi_u8 hi_ext_system_global_cac_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_gcac_base(vi_pipe)) & 0x1);
}
/* register: hi_ext_system_global_cac_coef_update_en */
/* global_cac_coef_update_en */
static __inline hi_void hi_ext_system_global_cac_coef_update_en_write(hi_vi_pipe vi_pipe,  hi_u8 data)
{
    iowr_8direct(vreg_gcac_base(vi_pipe)  +  1, data & 0x1);
}

static __inline hi_u8 hi_ext_system_global_cac_coef_update_en_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_gcac_base(vi_pipe)  +  1) & 0x1);
}

/* register: hi_ext_system_global_cac_coor_center_ver */
/* global_cac_coor_center_ver */
static __inline hi_void hi_ext_system_global_cac_coor_center_ver_write(hi_vi_pipe vi_pipe,  hi_u16 data)
{
    iowr_16direct(vreg_gcac_base(vi_pipe)  +  0x2, data);
}

static __inline hi_u16 hi_ext_system_global_cac_coor_center_ver_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_gcac_base(vi_pipe)  +  0x2);
}
/* register: hi_ext_system_global_cac_coor_center_hor */
/* global_cac_coor_center_hor */
static __inline hi_void hi_ext_system_global_cac_coor_center_hor_write(hi_vi_pipe vi_pipe,  hi_u16 data)
{
    iowr_16direct(vreg_gcac_base(vi_pipe)  +  0x4, data);
}

static __inline hi_u16 hi_ext_system_global_cac_coor_center_hor_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_gcac_base(vi_pipe)  +  0x4);
}
/* register: hi_ext_system_global_cac_param_red_a */
/* global_cac_param_red_a */
static __inline hi_void hi_ext_system_global_cac_param_red_a_write(hi_vi_pipe vi_pipe,  hi_s16 data)
{
    iowr_16direct(vreg_gcac_base(vi_pipe)  +  0x6, data);
}

static __inline hi_s16 hi_ext_system_global_cac_param_red_a_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_gcac_base(vi_pipe)  +  0x6);
}

/* register: hi_ext_system_global_cac_param_red_b */
/* global_cac_param_red_b */
static __inline hi_void hi_ext_system_global_cac_param_red_b_write(hi_vi_pipe vi_pipe,  hi_s16 data)
{
    iowr_16direct(vreg_gcac_base(vi_pipe)  +  0x8, data);
}

static __inline hi_s16 hi_ext_system_global_cac_param_red_b_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_gcac_base(vi_pipe)  +  0x8);
}

/* register: hi_ext_system_global_cac_param_red_c */
/* global_cac_param_red_c */
static __inline hi_void hi_ext_system_global_cac_param_red_c_write(hi_vi_pipe vi_pipe,  hi_s16 data)
{
    iowr_16direct(vreg_gcac_base(vi_pipe)  +  0xa, data);
}

static __inline hi_s16 hi_ext_system_global_cac_param_red_c_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_gcac_base(vi_pipe)  +  0xa);
}

/* register: hi_ext_system_global_cac_param_blue_a */
/* global_cac_param_blue_a */
static __inline hi_void hi_ext_system_global_cac_param_blue_a_write(hi_vi_pipe vi_pipe,  hi_s16 data)
{
    iowr_16direct(vreg_gcac_base(vi_pipe)  +  0xc, data);
}

static __inline hi_s16 hi_ext_system_global_cac_param_blue_a_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_gcac_base(vi_pipe)  +  0xc);
}

/* register: hi_ext_system_global_cac_param_blue_b */
/* global_cac_param_blue_b */
static __inline hi_void hi_ext_system_global_cac_param_blue_b_write(hi_vi_pipe vi_pipe,  hi_s16 data)
{
    iowr_16direct(vreg_gcac_base(vi_pipe)  +  0xe, data);
}

static __inline hi_s16 hi_ext_system_global_cac_param_blue_b_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_gcac_base(vi_pipe)  +  0xe);
}

/* register: hi_ext_system_global_cac_param_blue_c */
/* global_cac_param_blue_c */
static __inline hi_void hi_ext_system_global_cac_param_blue_c_write(hi_vi_pipe vi_pipe,  hi_s16 data)
{
    iowr_16direct(vreg_gcac_base(vi_pipe)  +  0x10, data);
}

static __inline hi_s16 hi_ext_system_global_cac_param_blue_c_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_gcac_base(vi_pipe)  +  0x10);
}

/* register: hi_ext_system_global_cac_y_ns_norm */
/* global_cac_y_ns_norm */
static __inline hi_void hi_ext_system_global_cac_y_ns_norm_write(hi_vi_pipe vi_pipe,  hi_u8 data)
{
    iowr_8direct(vreg_gcac_base(vi_pipe)  +  0x12, data);
}

static __inline hi_u8 hi_ext_system_global_cac_y_ns_norm_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_gcac_base(vi_pipe)  +  0x12));
}

/* register: hi_ext_system_global_cac_y_nf_norm */
/* global_cac_y_nf_norm */
static __inline hi_void hi_ext_system_global_cac_y_nf_norm_write(hi_vi_pipe vi_pipe,  hi_u8 data)
{
    iowr_8direct(vreg_gcac_base(vi_pipe)  +  0x13, data);
}

static __inline hi_u8 hi_ext_system_global_cac_y_nf_norm_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_gcac_base(vi_pipe)  +  0x13));
}

/* register: hi_ext_system_global_cac_x_ns_norm */
/* global_cac_x_ns_norm */
static __inline hi_void hi_ext_system_global_cac_x_ns_norm_write(hi_vi_pipe vi_pipe,  hi_u8 data)
{
    iowr_8direct(vreg_gcac_base(vi_pipe)  +  0x14, data);
}

static __inline hi_u8 hi_ext_system_global_cac_x_ns_norm_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_gcac_base(vi_pipe)  +  0x14));
}

/* register: hi_ext_system_global_cac_x_nf_norm */
/* global_cac_x_nf_norm */
static __inline hi_void hi_ext_system_global_cac_x_nf_norm_write(hi_vi_pipe vi_pipe,  hi_u8 data)
{
    iowr_8direct(vreg_gcac_base(vi_pipe)  +  0x15, data);
}

static __inline hi_u8 hi_ext_system_global_cac_x_nf_norm_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_gcac_base(vi_pipe)  +  0x15));
}
/* register: hi_ext_system_global_cac_cor_thr */
/* global_cac_cor_thr */
static __inline hi_void hi_ext_system_global_cac_cor_thr_write(hi_vi_pipe vi_pipe,  hi_u16 data)
{
    iowr_16direct(vreg_gcac_base(vi_pipe)  +  0x16, data);
}

static __inline hi_u16 hi_ext_system_global_cac_cor_thr_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_gcac_base(vi_pipe)  +  0x16);
}

/* CA */
/* register: hi_ext_system_ca_y_ratio_lut_write */
/* LEN =256 DATASIZE hi_u32 */
static __inline hi_void hi_ext_system_ca_y_ratio_lut_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u32 data)
{
    iowr_32direct(vreg_ca_base(vi_pipe)  + (index * sizeof(hi_u32)), data);
}

static __inline hi_u32 hi_ext_system_ca_y_ratio_lut_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return iord_32direct(vreg_ca_base(vi_pipe)  + (index * sizeof(hi_u32)));
}
/* register: hi_ext_system_ca_y_ratio_lut_write */
/* LEN =16 DATASIZE hi_u16  16*8*2 */
static __inline hi_void hi_ext_system_ca_iso_ratio_lut_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u32 data)
{
    iowr_32direct(vreg_ca_base(vi_pipe)  +  0x400  + (index * sizeof(hi_u32)), data);
}

static __inline hi_u16 hi_ext_system_ca_iso_ratio_lut_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return iord_32direct(vreg_ca_base(vi_pipe)  +  0x400  + (index * sizeof(hi_u32)));
}

/* register: hi_ext_system_ca_saturation_ratio_write */
#define HI_ISP_EXT_CA_SATURATION_RATIO_DEFAULT 1000
static __inline hi_void hi_ext_system_ca_saturation_ratio_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_ca_base(vi_pipe)  +  0x440, data);
}

static __inline hi_u16 hi_ext_system_ca_saturation_ratio_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_ca_base(vi_pipe)  +  0x440);
}

/* register: hi_ext_system_ca_luma_thd_high_write */
#define HI_ISP_EXT_CA_LUMA_THD_HIGH_DEFAULT 400
static __inline hi_void hi_ext_system_ca_luma_thd_high_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_ca_base(vi_pipe)  +  0x442, data);
}

static __inline hi_u16 hi_ext_system_ca_luma_thd_high_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_ca_base(vi_pipe)  +  0x442);
}

/* register: hi_ext_system_ca_en_write */
static __inline hi_void hi_ext_system_ca_en_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_ca_base(vi_pipe)  +  0x444, data & 0x1);
}

static __inline hi_u8 hi_ext_system_ca_en_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_ca_base(vi_pipe)  +  0x444) & 0x1);
}
/* register: hi_ext_system_ca_cp_en_write */
static __inline hi_void hi_ext_system_ca_cp_en_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_ca_base(vi_pipe)  +  0x445, data & 0x1);
}

static __inline hi_u8 hi_ext_system_ca_cp_en_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_ca_base(vi_pipe)  +  0x445) & 0x1);
}

/* register: hi_ext_system_ca_cp_en_write */
static __inline hi_void hi_ext_system_ca_coef_update_en_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_ca_base(vi_pipe)  +  0x446, data & 0x1);
}

static __inline hi_u8 hi_ext_system_ca_coef_update_en_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_ca_base(vi_pipe)  +  0x446) & 0x1);
}

static __inline hi_void hi_ext_system_ca_cp_lut_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u32 data)
{
    iowr_32direct(vreg_ca_base(vi_pipe) + 0x450 + (index * sizeof(hi_u32)), data);
}

static __inline hi_u32 hi_ext_system_ca_cp_lut_read(hi_vi_pipe vi_pipe, hi_u8 index)
{
    return iord_32direct(vreg_ca_base(vi_pipe) + 0x450 + (index * sizeof(hi_u32)));
}

/* module: csc(original CSC) */
/* register: hi_ext_system_csc_coef len = 9 */
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_csc_coef_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u16 data)
{
    iowr_16direct(vreg_csc_base(vi_pipe) + 0x0 + index * sizeof(hi_u16), data);
}

static __inline hi_u16 hi_ext_system_csc_coef_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return iord_16direct(vreg_csc_base(vi_pipe) + 0x0 + index * sizeof(hi_u16));
}

/* register: hi_ext_system_csc_dcin len = 3 */
/* args: data (10-bit) */
static __inline hi_void hi_ext_system_csc_dcin_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u16 data)
{
    iowr_16direct(vreg_csc_base(vi_pipe) + 0x20 + index * sizeof(hi_u16), data);
}

static __inline hi_u16 hi_ext_system_csc_dcin_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return iord_16direct(vreg_csc_base(vi_pipe) + 0x20 +  index * sizeof(hi_u16));
}

/* register: hi_ext_system_csc_dcout len = 3 */
/* args: data (10-bit) */
static __inline hi_void hi_ext_system_csc_dcout_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u16 data)
{
    iowr_16direct(vreg_csc_base(vi_pipe) + 0x30 + index * sizeof(hi_u16), data);
}

static __inline hi_u16 hi_ext_system_csc_dcout_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return iord_16direct(vreg_csc_base(vi_pipe) + 0x30 +  index * sizeof(hi_u16));
}

/* register: hi_ext_system_csc_dcout len = 3 */
/* args: data (10-bit) */
static __inline hi_void hi_ext_system_csc_gamuttype_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_csc_base(vi_pipe) + 0x40, data);
}

static __inline hi_u8 hi_ext_system_csc_gamuttype_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_csc_base(vi_pipe) + 0x40);
}

/* register: hi_ext_system_csc_enable */
/* args: data (1-bit) */
static __inline hi_void hi_ext_system_csc_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_csc_base(vi_pipe) + 0x50), (data & 0x1));
}

static __inline hi_u8 hi_ext_system_csc_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_csc_base(vi_pipe) + 0x50) & 0x1);
}

/* register: hi_ext_system_csc_enable */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_csc_contrast_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_csc_base(vi_pipe) + 0x51), data);
}

static __inline hi_u8 hi_ext_system_csc_contrast_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_csc_base(vi_pipe) + 0x51));
}

/* register: hi_ext_system_csc_enable */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_csc_hue_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_csc_base(vi_pipe) + 0x52), data);
}

static __inline hi_u8 hi_ext_system_csc_hue_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_csc_base(vi_pipe) + 0x52));
}

/* register: hi_ext_system_csc_enable */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_csc_sat_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_csc_base(vi_pipe) + 0x53), data);
}

static __inline hi_u8 hi_ext_system_csc_sat_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_csc_base(vi_pipe) + 0x53));
}

/* register: hi_ext_system_csc_enable */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_csc_luma_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_csc_base(vi_pipe) + 0x54), data);
}

static __inline hi_u8 hi_ext_system_csc_luma_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_csc_base(vi_pipe) + 0x54));
}

/* register: hi_ext_system_csc_enable */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_csc_limitrange_en_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_csc_base(vi_pipe) + 0x55), data);
}

static __inline hi_u8 hi_ext_system_csc_limitrange_en_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_csc_base(vi_pipe) + 0x55));
}

/* register: hi_ext_system_csc_enable */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_csc_ext_en_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_csc_base(vi_pipe) + 0x56), data);
}

static __inline hi_u8 hi_ext_system_csc_ext_en_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_csc_base(vi_pipe) + 0x56));
}

/* register: hi_ext_system_csc_enable */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_csc_ctmode_en_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_csc_base(vi_pipe) + 0x57), data);
}

static __inline hi_u8 hi_ext_system_csc_ctmode_en_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_csc_base(vi_pipe) + 0x57));
}

/* register: hi_ext_system_csc_update */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_csc_attr_update_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_csc_base(vi_pipe) + 0x58), data);
}

static __inline hi_u8 hi_ext_system_csc_attr_update_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_csc_base(vi_pipe) + 0x58));
}
/* LDCI */
/* ldci---U8--EXT_REGISTER */
#define HI_EXT_SYSTEM_LDCI_MANU_MODE_DEFAULT 0
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_ldci_manu_mode_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_ldci_base(vi_pipe), data & 0x1);
}
static __inline hi_u8 hi_ext_system_ldci_manu_mode_read(hi_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_ldci_base(vi_pipe))) & 0x1);
}

#define HI_EXT_SYSTEM_LDCI_MANU_HEPOSWGT_DEFAULT 64
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_ldci_manu_he_poswgt_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_ldci_base(vi_pipe) + 0x1, data & 0xff);
}
static __inline hi_u8 hi_ext_system_ldci_manu_he_pos_wgt_read(hi_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_ldci_base(vi_pipe) + 0x1)) & 0xff);
}

#define HI_EXT_SYSTEM_LDCI_MANU_HEPOSSIGMA_DEFAULT 80
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_ldci_manu_he_pos_sigma_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_ldci_base(vi_pipe) + 0x2, data & 0xff);
}
static __inline hi_u8 hi_ext_system_ldci_manu_he_pos_sigma_read(hi_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_ldci_base(vi_pipe) + 0x2)) & 0xff);
}

#define HI_EXT_SYSTEM_LDCI_MANU_HEPOSMEAN_DEFAULT 0
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_ldci_manu_he_pos_mean_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_ldci_base(vi_pipe) + 0x3, data & 0xff);
}
static __inline hi_u8 hi_ext_system_ldci_manu_he_pos_mean_read(hi_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_ldci_base(vi_pipe) + 0x3)) & 0xff);
}

#define HI_EXT_SYSTEM_LDCI_MANU_HENEGWGT_DEFAULT 50
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_ldci_manu_he_negwgt_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_ldci_base(vi_pipe) + 0x4, data & 0xff);
}
static __inline hi_u8 hi_ext_system_ldci_manu_he_neg_wgt_read(hi_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_ldci_base(vi_pipe) + 0x4)) & 0xff);
}

#define HI_EXT_SYSTEM_LDCI_MANU_HENEGSIGMA_DEFAULT 80
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_ldci_manu_he_negsigma_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_ldci_base(vi_pipe) + 0x5, data & 0xff);
}
static __inline hi_u8 hi_ext_system_ldci_manu_he_neg_sigma_read(hi_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_ldci_base(vi_pipe) + 0x5)) & 0xff);
}

#define HI_EXT_SYSTEM_LDCI_MANU_HENEGMEAN_DEFAULT 0
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_ldci_manu_he_negmean_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_ldci_base(vi_pipe) + 0x6, data & 0xff);
}
static __inline hi_u8 hi_ext_system_ldci_manu_he_neg_mean_read(hi_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_ldci_base(vi_pipe) + 0x6)) & 0xff);
}

#define HI_EXT_SYSTEM_LDCI_ENABLE_DEFAULT 0
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_ldci_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_ldci_base(vi_pipe) + 0x7, data & 0x1);
}
static __inline hi_u8 hi_ext_system_ldci_enable_read(hi_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_ldci_base(vi_pipe) + 0x7)) & 0x1);
}

#define HI_EXT_SYSTEM_LDCI_GAUSSLPFSIGMA_DEFAULT 36
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_ldci_gauss_lpfsigma_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_ldci_base(vi_pipe) + 0x8, data & 0xff);
}
static __inline hi_u8 hi_ext_system_ldci_gauss_lpfsigma_read(hi_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_ldci_base(vi_pipe) + 0x8)) & 0xff);
}

/* ldci---U16--EXT_REGISTER */
#define HI_EXT_SYSTEM_LDCI_MANU_BLCCTRL_DEFAULT 5
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_ldci_manu_blc_ctrl_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_ldci_base(vi_pipe) + 0xa, data & 0x1ff);
}
static __inline hi_u16 hi_ext_system_ldci_manu_blc_ctrl_read(hi_vi_pipe vi_pipe)
{
    return ((iord_16direct(vreg_ldci_base(vi_pipe) + 0xa)) & 0x1ff);
}

/* ldci---ARRAY--EXT_REGISTER */
/* auoto 16 multi addrs */
static __inline hi_void hi_ext_system_ldci_he_pos_wgt_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u8 data)
{
    iowr_8direct((vreg_ldci_base(vi_pipe) + 0x10 + index * sizeof(hi_u8)), data & 0xff);
}
static __inline hi_u8 hi_ext_system_ldci_he_pos_wgt_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return ((iord_8direct((vreg_ldci_base(vi_pipe) + 0x10 + index * sizeof(hi_u8)))) & 0xff);
}

/* auoto 16 multi addrs */
static __inline hi_void hi_ext_system_ldci_he_pos_sigma_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u8 data)
{
    iowr_8direct((vreg_ldci_base(vi_pipe) + 0x20 + index * sizeof(hi_u8)), data & 0xff);
}
static __inline hi_u8 hi_ext_system_ldci_he_pos_sigma_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return ((iord_8direct((vreg_ldci_base(vi_pipe) + 0x20 + index * sizeof(hi_u8)))) & 0xff);
}

/* auoto 16 multi addrs */
static __inline hi_void hi_ext_system_ldci_he_pos_mean_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u8 data)
{
    iowr_8direct((vreg_ldci_base(vi_pipe) + 0x30 + index * sizeof(hi_u8)), data & 0xff);
}
static __inline hi_u8 hi_ext_system_ldci_he_pos_mean_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return ((iord_8direct((vreg_ldci_base(vi_pipe) + 0x30 + index * sizeof(hi_u8)))) & 0xff);
}

/* auoto 16 multi addrs */
static __inline hi_void hi_ext_system_ldci_he_neg_wgt_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u8 data)
{
    iowr_8direct((vreg_ldci_base(vi_pipe) + 0x40 + index * sizeof(hi_u8)), data & 0xff);
}
static __inline hi_u8 hi_ext_system_ldci_he_neg_wgt_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return ((iord_8direct((vreg_ldci_base(vi_pipe) + 0x40 + index * sizeof(hi_u8)))) & 0xff);
}

/* auoto 16 multi addrs */
static __inline hi_void hi_ext_system_ldci_he_neg_sigma_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u8 data)
{
    iowr_8direct((vreg_ldci_base(vi_pipe) + 0x50 + index * sizeof(hi_u8)), data & 0xff);
}
static __inline hi_u8 hi_ext_system_ldci_he_neg_sigma_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return ((iord_8direct((vreg_ldci_base(vi_pipe) + 0x50 + index * sizeof(hi_u8)))) & 0xff);
}

/* auoto 16 multi addrs */
static __inline hi_void hi_ext_system_ldci_he_neg_mean_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u8 data)
{
    iowr_8direct((vreg_ldci_base(vi_pipe) + 0x60 + index * sizeof(hi_u8)), data & 0xff);
}
static __inline hi_u8 hi_ext_system_ldci_he_neg_mean_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return ((iord_8direct((vreg_ldci_base(vi_pipe) + 0x60 + index * sizeof(hi_u8)))) & 0xff);
}

/* auoto 16 multi addrs */
static __inline hi_void hi_ext_system_ldci_blc_ctrl_write(hi_vi_pipe vi_pipe, hi_u8 index, hi_u16 data)
{
    iowr_16direct((vreg_ldci_base(vi_pipe) + 0x70 + index * sizeof(hi_u16)), data & 0x1ff);
}
static __inline hi_u16 hi_ext_system_ldci_blc_ctrl_read(hi_vi_pipe vi_pipe,  hi_u8 index)
{
    return ((iord_16direct((vreg_ldci_base(vi_pipe) + 0x70 + index * sizeof(hi_u16)))) & 0x1ff);
}

#define HI_EXT_SYSTEM_LDCI_TPR_INCR_COEF_DEFAULT 8

static __inline hi_void hi_ext_system_ldci_tpr_incr_coef_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_ldci_base(vi_pipe) + 0x90, data & 0x1ff);
}
static __inline hi_u16 hi_ext_system_ldci_tpr_incr_coef_read(hi_vi_pipe vi_pipe)
{
    return ((iord_16direct(vreg_ldci_base(vi_pipe) + 0x90)) & 0x1ff);
}

#define HI_EXT_SYSTEM_LDCI_TPR_DECR_COEF_DEFAULT 8

static __inline hi_void hi_ext_system_ldci_tpr_decr_coef_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_ldci_base(vi_pipe) + 0x92, data & 0x1ff);
}
static __inline hi_u16 hi_ext_system_ldci_tpr_decr_coef_read(hi_vi_pipe vi_pipe)
{
    return ((iord_16direct(vreg_ldci_base(vi_pipe) + 0x92)) & 0x1ff);
}

/* fe_log_lut */
#define HI_EXT_SYSTEM_FELOGLUT_ENABLE_DEFAULT 0
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_feloglut_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_feloglut_base(vi_pipe), data & 0x1);
}
static __inline hi_u8 hi_ext_system_feloglut_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_feloglut_base(vi_pipe)) & 0x1);
}
/* log_lut */
#define HI_EXT_SYSTEM_LOGLUT_ENABLE_DEFAULT 0
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_loglut_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_loglut_base(vi_pipe), data & 0x1);
}
static __inline hi_u8 hi_ext_system_loglut_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_loglut_base(vi_pipe)) & 0x1);
}

/* register: DNG info */
/* args: data (32-bit) */
#define HI_EXT_SYSTEM_DNG_STATIC_INFO_VALID_DEFAULT 0x0

/* args: data (8-bit) */
static __inline hi_void hi_ext_system_dng_static_info_valid_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_dnginfo_base(vi_pipe), data);
}

static __inline hi_u8 hi_ext_system_dng_static_info_valid_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_dnginfo_base(vi_pipe));
}

#define HI_EXT_SYSTEM_DNG_HIGH_WB_GAIN_R_DEFAULT 0x400
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_dng_high_wb_gain_r_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_dnginfo_base(vi_pipe) + 0x4, data);
}

static __inline hi_u16 hi_ext_system_dng_high_wb_gain_r_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_dnginfo_base(vi_pipe) + 0x4);
}

#define HI_EXT_SYSTEM_DNG_HIGH_WB_GAIN_G_DEFAULT 0x400
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_dng_high_wb_gain_g_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_dnginfo_base(vi_pipe) + 0x8, data);
}

static __inline hi_u16 hi_ext_system_dng_high_wb_gain_g_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_dnginfo_base(vi_pipe) + 0x8);
}

#define HI_EXT_SYSTEM_DNG_HIGH_WB_GAIN_B_DEFAULT 0x400
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_dng_high_wb_gain_b_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_dnginfo_base(vi_pipe) + 0xc, data);
}

static __inline hi_u16 hi_ext_system_dng_high_wb_gain_b_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_dnginfo_base(vi_pipe) + 0xc);
}

#define HI_EXT_SYSTEM_DNG_LOW_WB_GAIN_R_DEFAULT 0x400
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_dng_low_wb_gain_r_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_dnginfo_base(vi_pipe) + 0x10, data);
}

static __inline hi_u16 hi_ext_system_dng_low_wb_gain_r_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_dnginfo_base(vi_pipe) + 0x10);
}

#define HI_EXT_SYSTEM_DNG_LOW_WB_GAIN_G_DEFAULT 0x400
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_dng_low_wb_gain_g_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_dnginfo_base(vi_pipe) + 0x14, data);
}

static __inline hi_u16 hi_ext_system_dng_low_wb_gain_g_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_dnginfo_base(vi_pipe) + 0x14);
}

#define HI_EXT_SYSTEM_DNG_LOW_WB_GAIN_B_DEFAULT 0x400
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_dng_low_wb_gain_b_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_dnginfo_base(vi_pipe) + 0x18, data);
}

static __inline hi_u16 hi_ext_system_dng_low_wb_gain_b_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_dnginfo_base(vi_pipe) + 0x18);
}

/* radial crop */
/* register: hi_ext_system_rc_en_write */
static __inline hi_void hi_ext_system_rc_en_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_rc_base(vi_pipe)  +  0x0, data & 0x1);
}

static __inline hi_u8 hi_ext_system_rc_en_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_rc_base(vi_pipe)  +  0x0) & 0x1);
}

static __inline hi_void hi_ext_system_rc_coef_update_en_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_rc_base(vi_pipe)  +  0x1, data & 0x1);
}

static __inline hi_u8 hi_ext_system_rc_coef_update_en_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_rc_base(vi_pipe)  +  0x1) & 0x1);
}

/* register: hi_ext_system_rc_center_hor_coor_write */
static __inline hi_void hi_ext_system_rc_center_hor_coor_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_rc_base(vi_pipe)  +  0x2, data);
}

static __inline hi_u16 hi_ext_system_rc_center_hor_coor_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_rc_base(vi_pipe)  +  0x2));
}

/* register: hi_ext_system_rc_center_ver_coor_write */
static __inline hi_void hi_ext_system_rc_center_ver_coor_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_rc_base(vi_pipe)  +  0x4, data);
}

static __inline hi_u16 hi_ext_system_rc_center_ver_coor_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_rc_base(vi_pipe)  +  0x4));
}

/* register: hi_ext_system_rc_radius_write */
static __inline hi_void hi_ext_system_rc_radius_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct(vreg_rc_base(vi_pipe)  +  0x6, data);
}

static __inline hi_u16 hi_ext_system_rc_radius_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_rc_base(vi_pipe)  +  0x6));
}

/* radial crop */
/* register: hi_ext_system_gamma_lut_write */
static __inline hi_void hi_ext_system_gamma_en_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_gamma_base(vi_pipe), data);
}

static __inline hi_u8 hi_ext_system_gamma_en_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_gamma_base(vi_pipe));
}

static __inline hi_void hi_ext_system_gamma_lut_update_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_gamma_base(vi_pipe) + 0x1, data);
}

static __inline hi_u8 hi_ext_system_gamma_lut_update_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_gamma_base(vi_pipe) + 0x1);
}

static __inline hi_void hi_ext_system_gamma_lut_write(hi_vi_pipe vi_pipe, hi_u16 index, hi_u16 data)
{
    iowr_16direct(vreg_gamma_base(vi_pipe) + 0x4 + (index * sizeof(hi_u16)), data);
}

static __inline hi_u16 hi_ext_system_gamma_lut_read(hi_vi_pipe vi_pipe,  hi_u16 index)
{
    return (iord_16direct(vreg_gamma_base(vi_pipe) + 0x4 + (index * sizeof(hi_u16))));
}

/* register: hi_ext_system_pregamma_lut_write */
static __inline hi_void hi_ext_system_pregamma_en_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_pregamma_base(vi_pipe), data);
}

static __inline hi_u8 hi_ext_system_pregamma_en_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_pregamma_base(vi_pipe));
}

static __inline hi_void hi_ext_system_pregamma_lut_update_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_pregamma_base(vi_pipe) + 0x1, data);
}

static __inline hi_u8 hi_ext_system_pregamma_lut_update_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_pregamma_base(vi_pipe) + 0x1);
}

/* depth : 257 * sizeof(hi_u32)  = 0x404 */
static __inline hi_void hi_ext_system_pregamma_lut_write(hi_vi_pipe vi_pipe, hi_u16 index, hi_u32 data)
{
    iowr_32direct(vreg_pregamma_base(vi_pipe) + 0x4 + (index * sizeof(hi_u32)), data);
}

static __inline hi_u32 hi_ext_system_pregamma_lut_read(hi_vi_pipe vi_pipe,  hi_u32 index)
{
    return (iord_32direct(vreg_pregamma_base(vi_pipe) + 0x4 + (index * sizeof(hi_u32))));
}

/* CLUT */
/* register: hi_ext_system_clut_en_write */
static __inline hi_void hi_ext_system_clut_en_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_clut_base(vi_pipe)  +  0x0, data & 0x1);
}

static __inline hi_u8 hi_ext_system_clut_en_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_clut_base(vi_pipe)  +  0x0) & 0x1);
}

static __inline hi_void hi_ext_system_clut_lut_update_en_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_clut_base(vi_pipe)  +  0x1, data & 0x1);
}
static __inline hi_u8 hi_ext_system_clut_lut_update_en_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_clut_base(vi_pipe)  +  0x1) & 0x1);
}

/* register: hi_ext_system_clut_gain_r_write */
static __inline hi_void hi_ext_system_clut_gain_r_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_clut_base(vi_pipe) + 0x2), data);
}

static __inline hi_u16 hi_ext_system_clut_gain_r_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_clut_base(vi_pipe) + 0x2));
}

/* register: hi_ext_system_clut_gain_g_write */
static __inline hi_void hi_ext_system_clut_gain_g_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_clut_base(vi_pipe) + 0x4), data);
}

static __inline hi_u16 hi_ext_system_clut_gain_g_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_clut_base(vi_pipe) + 0x4));
}

/* register: hi_ext_system_clut_gain_b_write */
static __inline hi_void hi_ext_system_clut_gain_b_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_clut_base(vi_pipe) + 0x6), data);
}

static __inline hi_u16 hi_ext_system_clut_gain_b_read(hi_vi_pipe vi_pipe)
{
    return (iord_16direct(vreg_clut_base(vi_pipe) + 0x6));
}

/* register: hi_ext_system_clut_b_lut_write */
static __inline hi_void hi_ext_system_clut_ctrl_update_en_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_clut_base(vi_pipe)  +  0x8, data & 0x1);
}
static __inline hi_u8 hi_ext_system_clut_ctrl_update_en_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_clut_base(vi_pipe)  +  0x8) & 0x1);
}

/* register: hi_ext_system_zone_row_awb */
/* config information of awb statistics */
#define HI_EXT_SYSTEM_AWB_SWITCH_DEFAULT 0
#define HI_EXT_SYSTEM_AWB_SWITCH_AFTER_DG 0
#define HI_EXT_SYSTEM_AWB_SWITCH_AFTER_EXPANDER 1
#define HI_EXT_SYSTEM_AWB_SWITCH_AFTER_DRC 2

/* args: data (2-bit) */
static __inline hi_void hi_ext_system_awb_switch_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0x2), data);
}
static __inline hi_u8 hi_ext_system_awb_switch_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0x2);
}

#define HI_EXT_SYSTEM_AWB_HNUM_DEFAULT 0x20

/* args: data (16-bit) */
static __inline hi_void hi_ext_system_awb_hnum_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0x4), data);
}

static __inline hi_u16 hi_ext_system_awb_hnum_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0x4);
}

#define HI_EXT_SYSTEM_AWB_VNUM_DEFAULT 0x20

/* args: data (16-bit) */
static __inline hi_void hi_ext_system_awb_vnum_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0x6), data);
}

static __inline hi_u16 hi_ext_system_awb_vnum_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0x6);
}

#define HI_EXT_SYSTEM_AWB_ZONE_BIN_DEFAULT 0x1

/* args: data (16-bit) */
static __inline hi_void hi_ext_system_awb_zone_bin_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0x8), data);
}

static __inline hi_u16 hi_ext_system_awb_zone_bin_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0x8);
}

#define HI_EXT_SYSTEM_AWB_HIST_BIN_THRESH0_DEFAULT 0x2000

/* args: data (16-bit) */
static __inline hi_void hi_ext_system_awb_hist_bin_thresh0_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0xa), data);
}

static __inline hi_u16 hi_ext_system_awb_hist_bin_thresh0_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0xa);
}

#define HI_EXT_SYSTEM_AWB_HIST_BIN_THRESH1_DEFAULT 0x4000

/* args: data (16-bit) */
static __inline hi_void hi_ext_system_awb_hist_bin_thresh1_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0xc), data);
}

static __inline hi_u16 hi_ext_system_awb_hist_bin_thresh1_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0xc);
}

#define HI_EXT_SYSTEM_AWB_HIST_BIN_THRESH2_DEFAULT 0x8000

/* args: data (16-bit) */
static __inline hi_void hi_ext_system_awb_hist_bin_thresh2_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0xe), data);
}

static __inline hi_u16 hi_ext_system_awb_hist_bin_thresh2_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0xe);
}

#define HI_EXT_SYSTEM_AWB_HIST_BIN_THRESH3_DEFAULT 0xffff

/* args: data (16-bit) */
static __inline hi_void hi_ext_system_awb_hist_bin_thresh3_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0x10), data);
}

static __inline hi_u16 hi_ext_system_awb_hist_bin_thresh3_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0x10);
}

#define HI_EXT_SYSTEM_AWB_WHITE_LEVEL_DEFAULT 0xffff

/* args: data (16-bit) */
static __inline hi_void hi_ext_system_awb_white_level_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0x12), data);
}

static __inline hi_u16 hi_ext_system_awb_white_level_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0x12);
}

#define HI_EXT_SYSTEM_AWB_BLACK_LEVEL_DEFAULT 0x0

/* args: data (16-bit) */
static __inline hi_void hi_ext_system_awb_black_level_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0x14), data);
}

static __inline hi_u16 hi_ext_system_awb_black_level_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0x14);
}

#define HI_EXT_SYSTEM_AWB_CR_REF_MAX_DEFAULT 0x180

/* args: data (16-bit) */
static __inline hi_void hi_ext_system_awb_cr_ref_max_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0x16), data);
}

static __inline hi_u16 hi_ext_system_awb_cr_ref_max_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0x16);
}

#define HI_EXT_SYSTEM_AWB_CR_REF_MIN_DEFAULT 0x40

/* args: data (16-bit) */
static __inline hi_void hi_ext_system_awb_cr_ref_min_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0x18), data);
}

static __inline hi_u16 hi_ext_system_awb_cr_ref_min_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0x18);
}

#define HI_EXT_SYSTEM_AWB_CB_REF_MAX_DEFAULT 0x180

/* args: data (16-bit) */
static __inline hi_void hi_ext_system_awb_cb_ref_max_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0x1a), data);
}

static __inline hi_u16 hi_ext_system_awb_cb_ref_max_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0x1a);
}

#define HI_EXT_SYSTEM_AWB_CB_REF_MIN_DEFAULT 0x40

/* args: data (16-bit) */
static __inline hi_void hi_ext_system_awb_cb_ref_min_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0x1c), data);
}

static __inline hi_u16 hi_ext_system_awb_cb_ref_min_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0x1c);
}

#define HI_EXT_SYSTEM_AWB_GAIN_ENABLE_DEFAULT 0x1

/* args: data (8-bit) */
static __inline hi_void hi_ext_system_awb_gain_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_awb_base(vi_pipe) + 0x1e), (data & 0x1));
}

static __inline hi_u8 hi_ext_system_awb_gain_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_awb_base(vi_pipe) + 0x1e) & 0x1);
}

/* args: data (16-bit) */
static __inline hi_void hi_ext_system_cc_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_awb_base(vi_pipe) + 0x20), (data & 0x1));
}

static __inline hi_u8 hi_ext_system_cc_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_awb_base(vi_pipe) + 0x20) & 0x1);
}

/* register: hi_ext_system_cc_colortone_rgain */
/* cc colortone */
#define HI_EXT_SYSTEM_CCM_COLORTONE_RGAIN_DEFAULT 256

/* args: data (16-bit) */
static __inline hi_void hi_ext_system_cc_colortone_rgain_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0x22), data);
}

static __inline hi_u16 hi_ext_system_cc_colortone_rgain_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0x22);
}

#define HI_EXT_SYSTEM_CCM_COLORTONE_GGAIN_DEFAULT 256

/* args: data (16-bit) */
static __inline hi_void hi_ext_system_cc_colortone_ggain_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0x24), data);
}

static __inline hi_u16 hi_ext_system_cc_colortone_ggain_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0x24);
}

#define HI_EXT_SYSTEM_CCM_COLORTONE_BGAIN_DEFAULT 256

/* args: data (16-bit) */
static __inline hi_void hi_ext_system_cc_colortone_bgain_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0x26), data);
}

static __inline hi_u16 hi_ext_system_cc_colortone_bgain_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0x26);
}

#define HI_EXT_SYSTEM_WB_STATISTICS_MPI_UPDATE_EN_DEFAULT 0

/* args: data (8-bit) */
static __inline hi_void hi_ext_system_wb_statistics_mpi_update_en_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_awb_base(vi_pipe) + 0x28), data);
}

static __inline hi_u8 hi_ext_system_wb_statistics_mpi_update_en_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_awb_base(vi_pipe) + 0x28);
}

/* args: data (16-bit) */
static __inline hi_void hi_ext_system_awb_sta_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_awb_base(vi_pipe) + 0x2a), (data & 0x1));
}

static __inline hi_u8 hi_ext_system_awb_sta_enable_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_awb_base(vi_pipe) + 0x2a) & 0x1);
}

/* register: hi_ext_system_crop_en */
/* CROP enable */
#define HI_EXT_SYSTEM_AWB_CROP_EN_DEFAULT 0x0

/* args: data (8-bit) */
static __inline hi_void hi_ext_system_awb_crop_en_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct((vreg_awb_base(vi_pipe) + 0x2c), data);
}
static __inline hi_u8 hi_ext_system_awb_crop_en_read(hi_vi_pipe vi_pipe)
{
    return iord_8direct(vreg_awb_base(vi_pipe) + 0x2c);
}

/* register: hi_ext_system_awb_crop_x */
/* AE CROP X */
#define HI_EXT_SYSTEM_AWB_CROP_X_DEFAULT 0x0

/* args: data (16-bit) */
static __inline hi_void hi_ext_system_awb_crop_x_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0x2e), data);
}
static __inline hi_u16 hi_ext_system_awb_crop_x_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0x2e);
}

/* register: hi_ext_system_awb_crop_y */
/* AWB CROP Y */
#define HI_EXT_SYSTEM_AWB_CROP_Y_DEFAULT 0x0

/* args: data (16-bit) */
static __inline hi_void hi_ext_system_awb_crop_y_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0x30), data);
}
static __inline hi_u16 hi_ext_system_awb_crop_y_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0x30);
}

/* register: hi_ext_system_awb_crop_height */
/* AWB CROP HEIGHT */
#define HI_EXT_SYSTEM_AWB_CROP_HEIGHT_DEFAULT 0x0

/* args: data (16-bit) */
static __inline hi_void hi_ext_system_awb_crop_height_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0x32), data);
}
static __inline hi_u16 hi_ext_system_awb_crop_height_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0x32);
}

/* register: hi_ext_system_awb_crop_width */
/* AWB CROP WIDTH */
#define HI_EXT_SYSTEM_AWB_CROP_WIDTH_DEFAULT 0x0

/* args: data (16-bit) */
static __inline hi_void hi_ext_system_awb_crop_width_write(hi_vi_pipe vi_pipe, hi_u16 data)
{
    iowr_16direct((vreg_awb_base(vi_pipe) + 0x34), data);
}
static __inline hi_u16 hi_ext_system_awb_crop_width_read(hi_vi_pipe vi_pipe)
{
    return iord_16direct(vreg_awb_base(vi_pipe) + 0x34);
}

/* DETAIL: vreg_de_base(vi_pipe) ~ vreg_de_base(vi_pipe)  + */
/* register: hi_ext_system_detail_enable_write */
/* args: data (8-bit) */
#define HI_EXT_SYSTEM_DETAIL_ENABLE_DEFAULT  1
static __inline hi_void hi_ext_system_detail_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_de_base(vi_pipe), (data & 0x1));
}
static __inline hi_u8 hi_ext_system_detail_enable_read(hi_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_de_base(vi_pipe))) & 0x1);
}

/* register: hi_ext_system_detail_manual_mode_write */
/* args: data (8-bit) */
#define HI_EXT_SYSTEM_DETAIL_MANU_MODE_DEFAULT  0
static __inline hi_void hi_ext_system_detail_manual_mode_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_de_base(vi_pipe) + 0x1, (data & 0x1));
}
static __inline hi_u8 hi_ext_system_detail_manual_mode_read(hi_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_de_base(vi_pipe) + 0x1)) & 0x1);
}

/* register: hi_ext_system_detail_luma_gain_lut_write */
/* args: data (16-bit) */
#define HI_EXT_SYSTEM_DETAIL_LUMA_GAIN_LUT_DEFAULT  1
static __inline hi_void hi_ext_system_detail_luma_gain_lut_write(hi_vi_pipe vi_pipe, hi_u16 i, hi_u16 data)
{
    iowr_16direct(vreg_de_base(vi_pipe) + 0x2 + i * sizeof(hi_u16), (data & 0x1ff));   /* end 36 = 2 + 17x2 */
}
static __inline hi_u16 hi_ext_system_detail_luma_gain_lut_read(hi_vi_pipe vi_pipe, hi_u16 i)
{
    return ((iord_16direct(vreg_de_base(vi_pipe) + 0x2 + i * sizeof(hi_u16))) & 0x1ff);
}

/* register: hi_ext_system_detail_auto_global_gain */
/* args: data (16-bit) */
#define HI_EXT_SYSTEM_DETAIL_AUTO_GLOBAL_GAIN_DEFAULT  200
static __inline hi_void hi_ext_system_detail_auto_global_gain_write(hi_vi_pipe vi_pipe, hi_u16 i, hi_u16 data)
{
    iowr_16direct(vreg_de_base(vi_pipe) + 0x28 + i * sizeof(hi_u16), (data & 0x1ff));     /* end 40 + 16x2 */
}
static __inline hi_u16 hi_ext_system_detail_auto_global_gain_read(hi_vi_pipe vi_pipe, hi_u16 i)
{
    return ((iord_16direct(vreg_de_base(vi_pipe) + 0x28 + i * sizeof(hi_u16))) & 0x1ff);
}

/* register: hi_ext_system_detail_auto_gain_lf */
/* args: data (16-bit) */
#define HI_EXT_SYSTEM_DETAIL_AUTO_GAIN_LF_DEFAULT  12
static __inline hi_void hi_ext_system_detail_auto_gain_lf_write(hi_vi_pipe vi_pipe, hi_u16 i, hi_u16 data)
{
    iowr_16direct(vreg_de_base(vi_pipe) + 0x50 + i * sizeof(hi_u16), (data & 0x1ff));    /* end 80 + 16x2 */
}
static __inline hi_u16 hi_ext_system_detail_auto_gain_lf_read(hi_vi_pipe vi_pipe, hi_u16 i)
{
    return ((iord_16direct(vreg_de_base(vi_pipe) + 0x50 + i * sizeof(hi_u16))) & 0x1ff);
}

/* register: hi_ext_system_detail_auto_gain_hf */
/* args: data (16-bit) */
#define HI_EXT_SYSTEM_DETAIL_AUTO_GAIN_HF_DEFAULT  12
static __inline hi_void hi_ext_system_detail_auto_gain_hf_write(hi_vi_pipe vi_pipe, hi_u16 i, hi_u16 data)
{
    iowr_16direct(vreg_de_base(vi_pipe) + 0x78 + i * sizeof(hi_u16), (data & 0x1ff));  /* end 120 + 16x2 */
}
static __inline hi_u16 hi_ext_system_detail_auto_gain_hf_read(hi_vi_pipe vi_pipe, hi_u16 i)
{
    return ((iord_16direct(vreg_de_base(vi_pipe) + 0x78 + i * sizeof(hi_u16))) & 0x1ff);
}

/* de manual start */
/* register: hi_ext_system_detail_manual_sgm_pos4_gain */
/* args: data (8-bit) */
/* register: hi_ext_system_detail_manual_global_gain */
/* args: data (16-bit) */
#define HI_EXT_SYSTEM_DETAIL_MANUAL_GLOBAL_GAIN_DEFAULT  200
static __inline hi_void hi_ext_system_detail_manual_global_gain_write(hi_vi_pipe vi_pipe,  hi_u16 data)
{
    iowr_16direct(vreg_de_base(vi_pipe) + 0xA0, (data & 0x1ff));    /* 160 */
}
static __inline hi_u16 hi_ext_system_detail_manual_global_gain_read(hi_vi_pipe vi_pipe)
{
    return ((iord_16direct(vreg_de_base(vi_pipe) + 0xA0)) & 0x1ff);
}

/* register: hi_ext_system_detail_manual_gain_lf */
/* args: data (16-bit) */
#define HI_EXT_SYSTEM_DETAIL_MANUAL_GAIN_LF_DEFAULT  12
static __inline hi_void hi_ext_system_detail_manual_gain_lf_write(hi_vi_pipe vi_pipe,  hi_u16 data)
{
    iowr_16direct(vreg_de_base(vi_pipe) + 0xA2, (data & 0x1ff));   /* 162 */
}
static __inline hi_u16 hi_ext_system_detail_manual_gain_lf_read(hi_vi_pipe vi_pipe)
{
    return ((iord_16direct(vreg_de_base(vi_pipe) + 0xA2)) & 0x1ff);
}

/* register: hi_ext_system_detail_manual_gain_hf */
/* args: data (16-bit) */
#define HI_EXT_SYSTEM_DETAIL_MANUAL_GAIN_HF_DEFAULT  12
static __inline hi_void hi_ext_system_detail_manual_gain_hf_write(hi_vi_pipe vi_pipe,  hi_u16 data)
{
    iowr_16direct(vreg_de_base(vi_pipe) + 0xA4, (data & 0x1ff));  /* 164 */
}
static __inline hi_u16 hi_ext_system_detail_manual_gain_hf_read(hi_vi_pipe vi_pipe)
{
    return ((iord_16direct(vreg_de_base(vi_pipe) + 0xA4)) & 0x1ff);
}

static __inline hi_void hi_ext_system_detail_attr_update_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_de_base(vi_pipe) + 0xA6, (data & 0x1));    /* 166 */
}
static __inline hi_u8 hi_ext_system_detail_attr_update_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_de_base(vi_pipe) + 0xA6) & 0x1);
}

/* args: data (8-bit) */
static __inline hi_void hi_ext_system_rgbir_enable_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_rgbir_base(vi_pipe), (data & 0x1));
}
static __inline hi_u8 hi_ext_system_rgbir_enable_read(hi_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_rgbir_base(vi_pipe))) & 0x1);
}

/* register: hi_ext_system_rgbir_th_write */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_rgbir_th_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_rgbir_base(vi_pipe) + 0x1, (data & 0xff));
}
static __inline hi_u8 hi_ext_system_rgbir_th_read(hi_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_rgbir_base(vi_pipe) + 0x1)) & 0xff);
}

/* register: hi_ext_system_rgbir_tv_write */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_rgbir_tv_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_rgbir_base(vi_pipe) + 0x2, (data & 0xff));
}
static __inline hi_u8 hi_ext_system_rgbir_tv_read(hi_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_rgbir_base(vi_pipe) + 0x2)) & 0xff);
}

/* register: hi_ext_system_rgbir_outpattern_write */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_rgbir_outpattern_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_rgbir_base(vi_pipe) + 0x3, (data & 0x3));
}
static __inline hi_u8 hi_ext_system_rgbir_outpattern_read(hi_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_rgbir_base(vi_pipe) + 0x3)) & 0x3);
}

/* register: hi_ext_system_rgbir_outpattern_write */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_rgbir_inpattern_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_rgbir_base(vi_pipe) + 0x4, (data & 0x7));
}
static __inline hi_u8 hi_ext_system_rgbir_inpattern_read(hi_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_rgbir_base(vi_pipe) + 0x4)) & 0x7);
}

/* register: hi_ext_system_rgbir_expctrl_write */
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_rgbir_expctrl_write(hi_vi_pipe vi_pipe, hi_u16 data, hi_u16 i)
{
    iowr_16direct(vreg_rgbir_base(vi_pipe) + 0x6 + i * sizeof(hi_u16), (data & 0xffff));
}
static __inline hi_u16 hi_ext_system_rgbir_expctrl_read(hi_vi_pipe vi_pipe, hi_u16 i)
{
    return ((iord_16direct(vreg_rgbir_base(vi_pipe) + 0x6 + i * sizeof(hi_u16))) & 0xffff);
}

/* register: hi_ext_system_rgbir_expctrl_write */
/* args: data (16-bit) */
static __inline hi_void hi_ext_system_rgbir_gain_write(hi_vi_pipe vi_pipe, hi_u16 data, hi_u16 i)
{
    iowr_16direct(vreg_rgbir_base(vi_pipe) + 0xa + i * sizeof(hi_u16), (data & 0xffff));
}
static __inline hi_u16 hi_ext_system_rgbir_gain_read(hi_vi_pipe vi_pipe, hi_u16 i)
{
    return ((iord_16direct(vreg_rgbir_base(vi_pipe) + 0xa + i * sizeof(hi_u16))) & 0xffff);
}

/* register: hi_ext_system_rgbir_outpattern_write */
/* args: data (8-bit) */
static __inline hi_void hi_ext_system_rgbir_irstatus_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_rgbir_base(vi_pipe) + 0xe, (data & 0x3));
}
static __inline hi_u8 hi_ext_system_rgbir_irstatus_read(hi_vi_pipe vi_pipe)
{
    return ((iord_8direct(vreg_rgbir_base(vi_pipe) + 0xe)) & 0x3);
}

/* register: hi_ext_system_rgbir_cvtmatrix_write */
/* args: data (16-bit, array) */
#define ISP_RGBIR_CVTMATRIX_NUM 12
static __inline hi_void hi_ext_system_rgbir_cvtmatrix_write(hi_vi_pipe vi_pipe, hi_u16 data, hi_u16 i)
{
    iowr_16direct(vreg_rgbir_base(vi_pipe) + 0x10 + i * sizeof(hi_u16), (data & 0xffff));
}
static __inline hi_u16 hi_ext_system_rgbir_cvtmatrix_read(hi_vi_pipe vi_pipe, hi_u16 i)
{
    return ((iord_16direct(vreg_rgbir_base(vi_pipe) + 0x10 + i * sizeof(hi_u16))) & 0xffff);
}

static __inline hi_void hi_ext_system_rgbir_attr_update_write(hi_vi_pipe vi_pipe, hi_u8 data)
{
    iowr_8direct(vreg_rgbir_base(vi_pipe) + 0x20, (data & 0x1));
}
static __inline hi_u8 hi_ext_system_rgbir_attr_update_read(hi_vi_pipe vi_pipe)
{
    return (iord_8direct(vreg_rgbir_base(vi_pipe) + 0x20) & 0x1);
}
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __ISP_EXT_CONFIG_H__ */
