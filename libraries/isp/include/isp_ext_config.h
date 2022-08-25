/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __ISP_EXT_CONFIG_H__
#define __ISP_EXT_CONFIG_H__

#include "isp_vreg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

// ------------------------------------------------------------------------------ //
// Instance 'ext' of module 'ext_config'
// ------------------------------------------------------------------------------ //

#define ISP_EXT_BASE_ADDR(ViPipe) (ISP_VIR_REG_BASE(ViPipe))

#ifdef CONFIG_ISP_DPC_STATIC_TABLE_SUPPORT
#define ISP_EXT_SIZE (0x20000)
#else
#define ISP_EXT_SIZE (0x10000)
#endif
// ----------------------------------------------------------------------------------------//
// TOP: 0x10000~0x10FFF
// ----------------------------------------------------------------------------------------//
#define VREG_TOP_OFFSET (0x0)
#define VREG_TOP_SIZE (0x1000)
#define VREG_TOP_BASE(ViPipe) (ISP_EXT_BASE_ADDR(ViPipe) + VREG_TOP_OFFSET)
// -----------------------------------------------------------------------------------------//
// SIZE < 0x100
// ----------------------------------------------------------------------------------------//
// -----------------------------------------------------------------------------------------//
// SIZE < 0x100
// ----------------------------------------------------------------------------------------//
/* BLC:   0x1000~0x100F */
#define VREG_BLC_OFFSET (VREG_TOP_OFFSET + VREG_TOP_SIZE)
#define VREG_BLC_SIZE (0x10)
#define VREG_BLC_BASE(ViPipe) (ISP_EXT_BASE_ADDR(ViPipe) + VREG_BLC_OFFSET)

/* FeLogLUT:0x1010~0x101f */
#define VREG_FELOGLUT_OFFSET (VREG_BLC_OFFSET + VREG_BLC_SIZE)
#define VREG_FELOGLUT_SIZE (0x10)
#define VREG_FELOGLUT_BASE(ViPipe) \
	(ISP_EXT_BASE_ADDR(ViPipe) + VREG_FELOGLUT_OFFSET)

/* BeLogLUT:0x1020~0x102f */
#define VREG_LOGLUT_OFFSET (VREG_FELOGLUT_OFFSET + VREG_FELOGLUT_SIZE)
#define VREG_LOGLUT_SIZE (0x10)
#define VREG_LOGLUT_BASE(ViPipe) \
	(ISP_EXT_BASE_ADDR(ViPipe) + VREG_LOGLUT_OFFSET)

/* EdgeMark:0x1030~0x103f */
#define VREG_EDGEMARK_OFFSET (VREG_LOGLUT_OFFSET + VREG_LOGLUT_SIZE)
#define VREG_EDGEMARK_SIZE (0x10)
#define VREG_EDGEMARK_BASE(ViPipe) \
	(ISP_EXT_BASE_ADDR(ViPipe) + VREG_EDGEMARK_OFFSET)

/* Hlc:0x1040~0x104f */
#define VREG_HLC_OFFSET (VREG_EDGEMARK_OFFSET + VREG_EDGEMARK_SIZE)
#define VREG_HLC_SIZE (0x10)
#define VREG_HLC_BASE(ViPipe) (ISP_EXT_BASE_ADDR(ViPipe) + VREG_HLC_OFFSET)

/* RC:   0x1050~0x105F */
#define VREG_RC_OFFSET (VREG_HLC_OFFSET + VREG_HLC_SIZE)
#define VREG_RC_SIZE (0x10)
#define VREG_RC_BASE(ViPipe) (ISP_EXT_BASE_ADDR(ViPipe) + VREG_RC_OFFSET)

/* CLUT:0x1060~0x107F */
#define VREG_CLUT_OFFSET (VREG_RC_OFFSET + VREG_RC_SIZE)
#define VREG_CLUT_SIZE (0x20)
#define VREG_CLUT_BASE(ViPipe) (ISP_EXT_BASE_ADDR(ViPipe) + VREG_CLUT_OFFSET)

/* FPN:   0x1080~0x109F */
#define VREG_FPN_OFFSET (VREG_CLUT_OFFSET + VREG_CLUT_SIZE)
#define VREG_FPN_SIZE (0x20)
#define VREG_FPN_BASE(ViPipe) (ISP_EXT_BASE_ADDR(ViPipe) + VREG_FPN_OFFSET)

/* GCAC: 0x10A0~0x10BF */
#define VREG_GCAC_OFFSET (VREG_FPN_OFFSET + VREG_FPN_SIZE)
#define VREG_GCAC_SIZE (0x20)
#define VREG_GCAC_BASE(ViPipe) (ISP_EXT_BASE_ADDR(ViPipe) + VREG_GCAC_OFFSET)

/* FLICKER:  0x10C0~0x10DF */
#define VREG_FLICK_OFFSET (VREG_GCAC_OFFSET + VREG_GCAC_SIZE)
#define VREG_FLICK_SIZE (0x20)
#define VREG_FLICK_BASE(ViPipe) (ISP_EXT_BASE_ADDR(ViPipe) + VREG_FLICK_OFFSET)

/* LCAC: 0x10E0~0x112F */
#define VREG_LCAC_OFFSET (VREG_FLICK_OFFSET + VREG_FLICK_SIZE)
#define VREG_LCAC_SIZE (0x50)
#define VREG_LCAC_BASE(ViPipe) (ISP_EXT_BASE_ADDR(ViPipe) + VREG_LCAC_OFFSET)

/* FCR: 0x1130~0x117F */
#define VREG_FCR_OFFSET (VREG_LCAC_OFFSET + VREG_LCAC_SIZE)
#define VREG_FCR_SIZE (0x50)
#define VREG_FCR_BASE(ViPipe) (ISP_EXT_BASE_ADDR(ViPipe) + VREG_FCR_OFFSET)

/* DngInfo : 0x1180 ~ 0x11CF */
#define VREG_DNGINFO_OFFSET (VREG_FCR_OFFSET + VREG_FCR_SIZE)
#define VREG_DNGINFO_SIZE (0x50)
#define VREG_DNGINFO_BASE(ViPipe) \
	(ISP_EXT_BASE_ADDR(ViPipe) + VREG_DNGINFO_OFFSET)

/* GE:     0x11D0~0x124F */
#define VREG_GE_OFFSET (VREG_DNGINFO_OFFSET + VREG_DNGINFO_SIZE)
#define VREG_GE_SIZE (0x80)
#define VREG_GE_BASE(ViPipe) (ISP_EXT_BASE_ADDR(ViPipe) + VREG_GE_OFFSET)

/* csc: 0x1250~0x12FF */
#define VREG_CSC_OFFSET (VREG_GE_OFFSET + VREG_GE_SIZE)
#define VREG_CSC_SIZE (0xB0) /* 0x50 reserved */
#define VREG_CSC_BASE(ViPipe) (ISP_EXT_BASE_ADDR(ViPipe) + VREG_CSC_OFFSET)

// -----------------------------------------------------------------------------------------//
// SIZE <= 0x500
// ----------------------------------------------------------------------------------------//
/* WDR: 0x1300~0x13FF */
#define VREG_WDR_OFFSET (VREG_CSC_OFFSET + VREG_CSC_SIZE)
#define VREG_WDR_SIZE (0x100)
#define VREG_WDR_BASE(ViPipe) (ISP_EXT_BASE_ADDR(ViPipe) + VREG_WDR_OFFSET)

/* LDCI: 0x1400~0x14FF */
#define VREG_LDCI_OFFSET (VREG_WDR_OFFSET + VREG_WDR_SIZE)
#define VREG_LDCI_SIZE (0x100)
#define VREG_LDCI_BASE(ViPipe) (ISP_EXT_BASE_ADDR(ViPipe) + VREG_LDCI_OFFSET)

/* DM: 0x1500~0x15FF */
#define VREG_DM_OFFSET (VREG_LDCI_OFFSET + VREG_LDCI_SIZE)
#define VREG_DM_SIZE (0x100)
#define VREG_DM_BASE(ViPipe) (ISP_EXT_BASE_ADDR(ViPipe) + VREG_DM_OFFSET)

/* AWB:0x1600~0x16FF */
#define VREG_AWB_OFFSET (VREG_DM_OFFSET + VREG_DM_SIZE)
#define VREG_AWB_SIZE (0x100)
#define VREG_AWB_BASE(ViPipe) (ISP_EXT_BASE_ADDR(ViPipe) + VREG_AWB_OFFSET)

/* AF:0x1700~0x17FF */
#define VREG_AF_OFFSET (VREG_AWB_OFFSET + VREG_AWB_SIZE)
#define VREG_AF_SIZE (0x100)
#define VREG_AF_BASE(ViPipe) (ISP_EXT_BASE_ADDR(ViPipe) + VREG_AF_OFFSET)

/* AE:0x1800~0x19FF */
#define VREG_AE_OFFSET (VREG_AF_OFFSET + VREG_AF_SIZE)
#define VREG_AE_SIZE (0x200)
#define VREG_AE_BASE(ViPipe) (ISP_EXT_BASE_ADDR(ViPipe) + VREG_AE_OFFSET)
/* DEHAZE:  0x1A00~0x1BFF */
#define VREG_DEHAZE_OFFSET (VREG_AE_OFFSET + VREG_AE_SIZE)
#define VREG_DEHAZE_SIZE (0x200)
#define VREG_DEHAZE_BASE(ViPipe) \
	(ISP_EXT_BASE_ADDR(ViPipe) + VREG_DEHAZE_OFFSET)

/* DRC: 0x1C00~0x1EFF */
#define VREG_DRC_OFFSET (VREG_DEHAZE_OFFSET + VREG_DEHAZE_SIZE)
#define VREG_DRC_SIZE (0x300)
#define VREG_DRC_BASE(ViPipe) (ISP_EXT_BASE_ADDR(ViPipe) + VREG_DRC_OFFSET)

/* BNR: 0x1F00~0x21FF */
#define VREG_BNR_OFFSET (VREG_DRC_OFFSET + VREG_DRC_SIZE)
#define VREG_BNR_SIZE (0x300)
#define VREG_BNR_BASE(ViPipe) (ISP_EXT_BASE_ADDR(ViPipe) + VREG_BNR_OFFSET)

/* DE:0x2200~0x25FF */
#define VREG_DE_OFFSET (VREG_BNR_OFFSET + VREG_BNR_SIZE)
#define VREG_DE_SIZE (0x400)
#define VREG_DE_BASE(ViPipe) (ISP_EXT_BASE_ADDR(ViPipe) + VREG_DE_OFFSET)

/* PreGamma:0x2600~0x2AFF */
#define VREG_PREGAMMA_OFFSET (VREG_DE_OFFSET + VREG_DE_SIZE)
#define VREG_PREGAMMA_SIZE (0x500)
#define VREG_PREGAMMA_BASE(ViPipe) \
	(ISP_EXT_BASE_ADDR(ViPipe) + VREG_PREGAMMA_OFFSET)

// -----------------------------------------------------------------------------------------//
// SIZE < 0x1000
// ----------------------------------------------------------------------------------------//
/* CA:  0x2B00~0x33FF */
#define VREG_CA_OFFSET (VREG_PREGAMMA_OFFSET + VREG_PREGAMMA_SIZE)
#define VREG_CA_SIZE (0x900)
#define VREG_CA_BASE(ViPipe) (ISP_EXT_BASE_ADDR(ViPipe) + VREG_CA_OFFSET)

/* Gamma:0x3400~0x3CFF */
#define VREG_GAMMA_OFFSET (VREG_CA_OFFSET + VREG_CA_SIZE)
#define VREG_GAMMA_SIZE (0x900)
#define VREG_GAMMA_BASE(ViPipe) (ISP_EXT_BASE_ADDR(ViPipe) + VREG_GAMMA_OFFSET)

/* SHARPEN: 0x3D00~0x49FF */
#define VREG_SHARPEN_OFFSET (VREG_GAMMA_OFFSET + VREG_GAMMA_SIZE)
#define VREG_SHARPEN_SIZE (0xD00)
#define VREG_SHARPEN_BASE(ViPipe) \
	(ISP_EXT_BASE_ADDR(ViPipe) + VREG_SHARPEN_OFFSET)

/* RLSC:0x4A00~0x57FF */
#define VREG_RLSC_OFFSET (VREG_SHARPEN_OFFSET + VREG_SHARPEN_SIZE)
#define VREG_RLSC_SIZE (0xe00)
#define VREG_RLSC_BASE(ViPipe) (ISP_EXT_BASE_ADDR(ViPipe) + VREG_RLSC_OFFSET)

// -----------------------------------------------------------------------------------------//
// SIZE >= 0x1000
// ----------------------------------------------------------------------------------------//
/* LSC:  0x5800~0xA7FF */
#define VREG_LSC_OFFSET (VREG_RLSC_OFFSET + VREG_RLSC_SIZE)
#define VREG_LSC_SIZE (0x5000)
#define VREG_LSC_BASE(ViPipe) (ISP_EXT_BASE_ADDR(ViPipe) + VREG_LSC_OFFSET)

// -----------------------------------------------------------------------------------------//
// SIZE >= 0x10000
// ----------------------------------------------------------------------------------------//
/* DPC:0xA800~0x1B7FF */
#define VREG_DPC_OFFSET (VREG_LSC_OFFSET + VREG_LSC_SIZE)
#ifdef CONFIG_ISP_DPC_STATIC_TABLE_SUPPORT
#define VREG_DPC_SIZE (0x11000)
#else
#define VREG_DPC_SIZE (0x1000)
#endif
#define VREG_DPC_BASE(ViPipe) (ISP_EXT_BASE_ADDR(ViPipe) + VREG_DPC_OFFSET)

/* RGBIR:0x1B800~0x1B8FF */
#define VREG_RGBIR_OFFSET (VREG_DPC_OFFSET + VREG_DPC_SIZE)
#define VREG_RGBIR_SIZE (0x100)
#define VREG_RGBIR_BASE(ViPipe) (ISP_EXT_BASE_ADDR(ViPipe) + VREG_RGBIR_OFFSET)

// ------------------------------------------------------------------------------ //
// Group: Top
// ------------------------------------------------------------------------------ //
// ------------------------------------------------------------------------------ //
// Register: ext_top_wdr_cfg
// ------------------------------------------------------------------------------ //
// GK_TRUE: The state of WDR config is initialized;
// GK_FALSE: The state of WDR config is NOT initialized
// ------------------------------------------------------------------------------ //
#define ISP_EXT_TOP_WDR_CFG_DEFAULT (0x0)
#define ISP_EXT_TOP_WDR_CFG_DATASIZE (1)

// args: data (1-bit)
static __inline GK_VOID ext_top_wdr_cfg_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_TOP_BASE(ViPipe) + 0x0000, (data & 0x01));
}
static __inline GK_U8 ext_top_wdr_cfg_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_TOP_BASE(ViPipe) + 0x0000) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: ext_top_pub_attr_cfg
// ------------------------------------------------------------------------------ //
// GK_TRUE: The state of PubAttr config is initialized;
// GK_FALSE: The state of PubAttr config is NOT initialized
// ------------------------------------------------------------------------------ //

#define ISP_EXT_TOP_PUB_ATTR_CFG_DEFAULT (0x0)
#define ISP_EXT_TOP_PUB_ATTR_CFG_DATASIZE (1)

// args: data (1-bit)
static __inline GK_VOID ext_top_pub_attr_cfg_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_TOP_BASE(ViPipe) + 0x0002, (data & 0x01));
}
static __inline GK_U8 ext_top_pub_attr_cfg_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_TOP_BASE(ViPipe) + 0x0002) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: ext_top_wdr_switch
// ------------------------------------------------------------------------------ //
// GK_TRUE: WDR switch is complete;
// GK_FALSE: WDR switch is NOT complete
// ------------------------------------------------------------------------------ //

#define ISP_EXT_TOP_WDR_SWITCH_DEFAULT (0x0)
#define ISP_EXT_TOP_WDR_SWITCH_DATASIZE (1)

// args: data (1-bit)
static __inline GK_VOID ext_top_wdr_switch_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_TOP_BASE(ViPipe) + 0x0004, (data & 0x01));
}
static __inline GK_U8 ext_top_wdr_switch_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_TOP_BASE(ViPipe) + 0x0004) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: ext_top_res_switch
// ------------------------------------------------------------------------------ //
// GK_TRUE: Resolution switch is complete;
// GK_FALSE: Resolution switch is NOT complete
// ------------------------------------------------------------------------------ //

#define ISP_EXT_TOP_RES_SWITCH_DEFAULT (0x0)
#define ISP_EXT_TOP_RES_SWITCH_DATASIZE (1)

// args: data (1-bit)
static __inline GK_VOID ext_top_res_switch_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_TOP_BASE(ViPipe) + 0x0006, (data & 0x01));
}
static __inline GK_U8 ext_top_res_switch_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_TOP_BASE(ViPipe) + 0x0006) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: ext_top_sensor_width
// ------------------------------------------------------------------------------ //
//
// ------------------------------------------------------------------------------ //

#define ISP_EXT_TOP_SENSOR_WIDTH_DEFAULT (0xF00)
#define ISP_EXT_TOP_SENSOR_WIDTH_DATASIZE (32)

// args: data (32-bit)
static __inline GK_VOID ext_top_sensor_width_write(VI_PIPE ViPipe, GK_U32 data)
{
	IOWR_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x0008, data);
}
static __inline GK_U32 ext_top_sensor_width_read(VI_PIPE ViPipe)
{
	return IORD_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x0008);
}

// ------------------------------------------------------------------------------ //
// Register: ext_top_sensor_width
// ------------------------------------------------------------------------------ //
//
// ------------------------------------------------------------------------------ //

#define ISP_EXT_TOP_SENSOR_HEIGHT_DEFAULT (0x870)
#define ISP_EXT_TOP_SENSOR_HEIGHT_DATASIZE (32)

// args: data (32-bit)
static __inline GK_VOID ext_top_sensor_height_write(VI_PIPE ViPipe, GK_U32 data)
{
	IOWR_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x000c, data);
}
static __inline GK_U32 ext_top_sensor_height_read(VI_PIPE ViPipe)
{
	return IORD_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x000c);
}

#define ISP_EXT_SYSTEM_FPS_BASE_DEFAULT (30)
#define ISP_EXT_SYSTEM_FPS_BASE_DATASIZE (32)

// args: data (32-bit)
static __inline GK_VOID ext_system_fps_base_write(VI_PIPE ViPipe, GK_U32 data)
{
	IOWR_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x0010, data);
}
static __inline GK_U32 ext_system_fps_base_read(VI_PIPE ViPipe)
{
	return IORD_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x0010);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_sensor_max_resolution_width
// ------------------------------------------------------------------------------ //
// width of sensor resolution
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_SENSOR_MAX_RESOLUTION_WIDTH_DEFAULT (1280)
#define ISP_EXT_SYSTEM_SENSOR_MAX_RESOLUTION_WIDTH_DATASIZE (16)

// args: data (16-bit)

static __inline GK_VOID
ext_system_sensor_max_resolution_width_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x0014, data);
}

static __inline GK_U16
ext_system_sensor_max_resolution_width_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x0014);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_sensor_max_resolution_height
// ------------------------------------------------------------------------------ //
// height of sensor resolution
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_SENSOR_MAX_RESOLUTION_HEIGHT_DEFAULT (720)
#define ISP_EXT_SYSTEM_SENSOR_MAX_RESOLUTION_HEIGHT_DATASIZE (16)

// args: data (16-bit)

static __inline GK_VOID
ext_system_sensor_max_resolution_height_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x0016, data);
}

static __inline GK_U16
ext_system_sensor_max_resolution_height_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x0016);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_sys_debug_enable
// ------------------------------------------------------------------------------ //
// 0:disable system debug information
// 1:enable system debug information
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_SYS_DEBUG_ENABLE_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_SYS_DEBUG_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline GK_VOID ext_system_sys_debug_enable_write(VI_PIPE ViPipe,
							  GK_U16 data)
{
	GK_U16 u32Current = IORD_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x0018);
	IOWR_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x0018,
		      (((GK_U16)data) << 0) | (u32Current & 0xFFFE));
}

static __inline GK_U16 ext_system_sys_debug_enable_read(VI_PIPE ViPipe)
{
	return (GK_U16)((IORD_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x0018) &
			 0x0001) >>
			0);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_sys_debuggh_addr
// ------------------------------------------------------------------------------ //
// address of system debug information
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_SYS_DEBUG_HIGH_ADDR_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_SYS_DEBUG_HIGH_ADDR_DATASIZE (32)

// args: data (32-bit)
static __inline GK_VOID ext_system_sys_debuggh_addr_write(VI_PIPE ViPipe,
							  GK_U32 data)
{
	IOWR_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x001c, data);
}

static __inline GK_U32 ext_system_sys_debuggh_addr_read(VI_PIPE ViPipe)
{
	return IORD_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x001c);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_sys_debug_low_addr
// ------------------------------------------------------------------------------ //
// address of system debug information
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_SYS_DEBUG_LOW_ADDR_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_SYS_DEBUG_LOW_ADDR_DATASIZE (32)

// args: data (32-bit)
static __inline GK_VOID ext_system_sys_debug_low_addr_write(VI_PIPE ViPipe,
							    GK_U32 data)
{
	IOWR_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x0020, data);
}

static __inline GK_U32 ext_system_sys_debug_low_addr_read(VI_PIPE ViPipe)
{
	return IORD_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x0020);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_sys_debug_size
// ------------------------------------------------------------------------------ //
// size of sys debug struct
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_SYS_DEBUG_SIZE_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_SYS_DEBUG_SIZE_DATASIZE (32)

// args: data (32-bit)
static __inline GK_VOID ext_system_sys_debug_size_write(VI_PIPE ViPipe,
							GK_U32 data)
{
	IOWR_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x0024, data);
}

static __inline GK_U32 ext_system_sys_debug_size_read(VI_PIPE ViPipe)
{
	return (GK_U32)IORD_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x0024);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_debug_depth
// ------------------------------------------------------------------------------ //
// the depth of all debug information
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_DEBUG_DEPTH_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_DEBUG_DEPTH_DATASIZE (16)

// args: data (16-bit)
static __inline GK_VOID ext_system_sys_debug_depth_write(VI_PIPE ViPipe,
							 GK_U16 data)
{
	IOWR_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x0028, data);
}

static __inline GK_U16 ext_system_sys_debug_depth_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x0028);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_statistics_ctrl_lowbit
// ------------------------------------------------------------------------------ //
// ISP statistics control low bit
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_STATISTICS_LOWBIT_DEFAULT (ISP_STATISTICS_KEY)
#define ISP_EXT_SYSTEM_STATISTICS_LOWBIT_DATASIZE (32)

// args: data (32-bit)
static __inline GK_U32 ext_system_statistics_ctrl_lowbit_read(VI_PIPE ViPipe)
{
	return IORD_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x002c);
}

static __inline GK_VOID ext_system_statistics_ctrl_lowbit_write(VI_PIPE ViPipe,
								GK_U32 data)
{
	IOWR_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x002c, data);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_statistics_ctrlghbit
// ------------------------------------------------------------------------------ //
// ISP statistics control high bit
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_STATISTICS_HIGHBIT_DEFAULT (ISP_STATISTICS_KEY)
#define ISP_EXT_SYSTEM_STATISTICS_HIGHBIT_DATASIZE (32)

// args: data (32-bit)
static __inline GK_U32 ext_system_statistics_ctrlghbit_read(VI_PIPE ViPipe)
{
	return IORD_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x0030);
}

static __inline GK_VOID ext_system_statistics_ctrlghbit_write(VI_PIPE ViPipe,
							      GK_U32 data)
{
	IOWR_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x0030, data);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_bind_attr
// ------------------------------------------------------------------------------ //
// isp bind attr include AeLib AwbLib SensorId etc
// |----------------------------------------------------------------|
// |           RSV          |     AE_LIB_ID    |   AWB_LIB_ID       |
// |----------------------------------------------------------------|
// |<--------16bits--------><-------8bits------><------8bits------->|
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_BIND_ATTR_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_BIND_ATTR_DATASIZE (32)

// args: data (32-bit)
static __inline GK_U32 ext_system_bind_attr_read(VI_PIPE ViPipe)
{
	return IORD_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x0034);
}
static __inline GK_VOID ext_system_bind_attr_write(VI_PIPE ViPipe, GK_U32 data)
{
	IOWR_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x0034, data);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_freeze_firmware
// ------------------------------------------------------------------------------ //
// Disables firmware and stops updating the ISP
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_FREEZE_FIRMWARE_DEFAULT (0)
#define ISP_EXT_SYSTEM_FREEZE_FIRMWARE_DATASIZE (1)

// args: data (1-bit)
static __inline GK_VOID ext_system_freeze_firmware_write(VI_PIPE ViPipe,
							 GK_U8 data)
{
	GK_U8 u32Current = IORD_8DIRECT(VREG_TOP_BASE(ViPipe) + 0x0038);
	IOWR_8DIRECT(VREG_TOP_BASE(ViPipe) + 0x0038,
		     (data & 0x01) | (u32Current & 0xFE));
}
static __inline GK_U8 ext_system_freeze_firmware_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_TOP_BASE(ViPipe) + 0x0038) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_sensor_wdr_mode
// ------------------------------------------------------------------------------ //
// WDR mode: 0=linear 1=built-in wdr 2=2to1 full frame wdr  ...
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_SENSOR_WDR_MODE_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_SENSOR_WDR_MODE_DATASIZE (6)

// args: data (6-bit)
static __inline GK_VOID ext_system_sensor_wdr_mode_write(VI_PIPE ViPipe,
							 GK_U8 data)
{
	GK_U16 u32Current = IORD_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x003a);
	IOWR_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x003a,
		      (((GK_U16)(data & 0x003F)) << 4) | (u32Current & 0xFC0F));
}
static __inline GK_U8 ext_system_sensor_wdr_mode_read(VI_PIPE ViPipe)
{
	return (GK_U8)((IORD_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x003a) &
			0x03F0) >>
		       4);
}

// ------------------------------------------------------------------------------ //
// Register: ext_sync_total_width
// ------------------------------------------------------------------------------ //
// Total frame width including horizontal blanking
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYNC_TOTAL_WIDTH_DEFAULT (0x672)
#define ISP_EXT_SYNC_TOTAL_WIDTH_DATASIZE (16)
// args: data (16-bit)
static __inline GK_VOID ext_sync_total_width_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x3c, data);
}
static __inline GK_U16 ext_sync_total_width_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x3c);
}

// ------------------------------------------------------------------------------ //
// Register: ext_sync_total_height
// ------------------------------------------------------------------------------ //
// Total frame height including vertical blanking
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYNC_TOTAL_HEIGHT_DEFAULT (0x2EE)
#define ISP_EXT_SYNC_TOTAL_HEIGHT_DATASIZE (16)

// args: data (16-bit)
static __inline GK_VOID ext_sync_total_height_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x40, data);
}
static __inline GK_U16 ext_sync_total_height_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x40);
}

static __inline GK_VOID ext_system_be_total_width_write(VI_PIPE ViPipe,
							GK_U16 data)
{
	IOWR_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x44, data);
}
static __inline GK_U16 ext_system_be_total_width_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x44);
}

static __inline GK_VOID ext_system_be_total_height_write(VI_PIPE ViPipe,
							 GK_U16 data)
{
	IOWR_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x46, data);
}
static __inline GK_U16 ext_system_be_total_height_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x46);
}

static __inline GK_VOID ext_system_update_infogh_phyaddr_write(VI_PIPE ViPipe,
							       GK_U32 data)
{
	IOWR_32DIRECT((VREG_TOP_BASE(ViPipe) + 0xa4), data);
}

static __inline GK_U32 ext_system_update_infogh_phyaddr_read(VI_PIPE ViPipe)
{
	return IORD_32DIRECT(VREG_TOP_BASE(ViPipe) + 0xa4);
}

static __inline GK_VOID ext_system_update_info_low_phyaddr_write(VI_PIPE ViPipe,
								 GK_U32 data)
{
	IOWR_32DIRECT((VREG_TOP_BASE(ViPipe) + 0xa8), data);
}

static __inline GK_U32 ext_system_update_info_low_phyaddr_read(VI_PIPE ViPipe)
{
	return IORD_32DIRECT(VREG_TOP_BASE(ViPipe) + 0xa8);
}

static __inline GK_VOID ext_system_sys_iso_write(VI_PIPE ViPipe, GK_U32 data)
{
	IOWR_32DIRECT((VREG_TOP_BASE(ViPipe) + 0xac), data);
}

static __inline GK_U32 ext_system_sys_iso_read(VI_PIPE ViPipe)
{
	return IORD_32DIRECT(VREG_TOP_BASE(ViPipe) + 0xac);
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_gamma_curve_type
// ------------------------------------------------------------------------------ //
// the type of gamma curve
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_GAMMA_CURVE_TYPE_DEFAULT (0)
#define ISP_EXT_SYSTEM_GAMMA_CURVE_TYPE_DATASIZE (8)

// args: data (8-bit)
static __inline GK_VOID ext_system_gamma_curve_type_write(VI_PIPE ViPipe,
							  GK_U8 data)
{
	IOWR_8DIRECT(VREG_TOP_BASE(ViPipe) + 0xc8, data);
}
static __inline GK_U8 ext_system_gamma_curve_type_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_TOP_BASE(ViPipe) + 0xc8);
}

// ------------------------------------------------------------------------------ //
// Register: isp_crop_pos_x_write
// ------------------------------------------------------------------------------ //
//
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_CORP_POS_X_DATASIZE (16)
static __inline GK_VOID ext_system_corp_pos_x_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT(VREG_TOP_BASE(ViPipe) + 0xd4, data);
}
static __inline GK_U16 ext_system_corp_pos_x_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_TOP_BASE(ViPipe) + 0xd4);
}

// ------------------------------------------------------------------------------ //
// Register: isp_crop_pos_y_write
// ------------------------------------------------------------------------------ //
//
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_CORP_POS_Y_DATASIZE (16)
static __inline GK_VOID ext_system_corp_pos_y_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT(VREG_TOP_BASE(ViPipe) + 0xd6, data);
}
static __inline GK_U16 ext_system_corp_pos_y_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_TOP_BASE(ViPipe) + 0xd6);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_isp_bit_bypass
// ------------------------------------------------------------------------------ //
//
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYS_ISP_BYPASS_DEFAULT (0)
#define ISP_EXT_SYS_ISP_BYPASS_DATASIZE (32)

static __inline GK_VOID ext_system_isp_bit_bypass_write(VI_PIPE ViPipe,
							GK_U32 data)
{
	IOWR_32DIRECT(VREG_TOP_BASE(ViPipe) + 0xd8, data);
}

static __inline GK_U32 ext_system_isp_bit_bypass_read(VI_PIPE ViPipe)
{
	return IORD_32DIRECT(VREG_TOP_BASE(ViPipe) + 0xd8);
}

#define ISP_EXT_SYSTEM_TOP_BIT_RESERVE0_WRITE_DEFAULT (0)
#define ISP_EXT_SYSTEM_TOP_BIT_RESERVE0_WRITE_DATASIZE (2)

static __inline GK_VOID ext_system_top_bit_reserve0_write(VI_PIPE ViPipe,
							  GK_U8 data)
{
	IOWR_8DIRECT(VREG_TOP_BASE(ViPipe) + 0xe0, data & 0xF);
}

static __inline GK_U8 ext_system_top_bit_reserve0_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_TOP_BASE(ViPipe) + 0xe0) & 0xF);
}

#define ISP_EXT_SYSTEM_TOP_BIT_RESERVE1_WRITE_DEFAULT (0)
#define ISP_EXT_SYSTEM_TOP_BIT_RESERVE1_WRITE_DATASIZE (2)

static __inline GK_VOID ext_system_top_bit_reserve1_write(VI_PIPE ViPipe,
							  GK_U8 data)
{
	IOWR_8DIRECT(VREG_TOP_BASE(ViPipe) + 0xe1, data & 0x3);
}

static __inline GK_U8 ext_system_top_bit_reserve1_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_TOP_BASE(ViPipe) + 0xe1) & 0x3);
}

#define ISP_EXT_SYSTEM_TOP_BIT_RESERVE2_WRITE_DEFAULT (0)
#define ISP_EXT_SYSTEM_TOP_BIT_RESERVE2_WRITE_DATASIZE (2)

static __inline GK_VOID ext_system_top_bit_reserve2_write(VI_PIPE ViPipe,
							  GK_U8 data)
{
	IOWR_8DIRECT(VREG_TOP_BASE(ViPipe) + 0xe2, data & 0x3);
}

static __inline GK_U8 ext_system_top_bit_reserve2_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_TOP_BASE(ViPipe) + 0xe2) & 0x3);
}

#define ISP_EXT_SYSTEM_TOP_BIT_RESERVE3_WRITE_DEFAULT (0)
#define ISP_EXT_SYSTEM_TOP_BIT_RESERVE3_WRITE_DATASIZE (2)

static __inline GK_VOID ext_system_top_bit_reserve3_write(VI_PIPE ViPipe,
							  GK_U8 data)
{
	IOWR_8DIRECT(VREG_TOP_BASE(ViPipe) + 0xe3, data & 0x3);
}

static __inline GK_U8 ext_system_top_bit_reserve3_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_TOP_BASE(ViPipe) + 0xe3) & 0x3);
}

#define ISP_EXT_SYSTEM_TOP_BIT_RESERVE4_WRITE_DEFAULT (0)
#define ISP_EXT_SYSTEM_TOP_BIT_RESERVE4_WRITE_DATASIZE (2)

static __inline GK_VOID ext_system_top_bit_reserve4_write(VI_PIPE ViPipe,
							  GK_U8 data)
{
	IOWR_8DIRECT(VREG_TOP_BASE(ViPipe) + 0xe4, data & 0x7);
}

static __inline GK_U8 ext_system_top_bit_reserve4_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_TOP_BASE(ViPipe) + 0xe4) & 0x7);
}

static __inline GK_VOID ext_system_yuv_format_write(VI_PIPE ViPipe, GK_U32 data)
{
	IOWR_32DIRECT(VREG_TOP_BASE(ViPipe) + 0xec, data);
}

static __inline GK_U32 ext_system_yuv_format_read(VI_PIPE ViPipe)
{
	return IORD_32DIRECT(VREG_TOP_BASE(ViPipe) + 0xec);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_rggb_cfg_write
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_rggb_cfg_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_TOP_BASE(ViPipe) + 0xf0, (data & 0xff));
}
static __inline GK_U8 ext_system_rggb_cfg_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_TOP_BASE(ViPipe) + 0xf0) & 0xff);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_sensor_mode
// ------------------------------------------------------------------------------ //
// Used to distinguish the special sensor mode
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_SENSOR_MODE_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_SENSOR_MODE_DATASIZE (8)

// args: data (1-bit)
static __inline GK_VOID ext_system_sensor_mode_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_TOP_BASE(ViPipe) + 0x00f2, (data & 0xff));
}
static __inline GK_U8 ext_system_sensor_mode_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_TOP_BASE(ViPipe) + 0x00f2) & 0xff);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_isp_dgain_bypass
// ------------------------------------------------------------------------------ //
//
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYS_ISP_DGAIN_ENABLE_DEFAULT (0x0)
#define ISP_EXT_SYS_ISP_DGAIN_ENABLE_DATASIZE (1)

static __inline GK_VOID ext_system_isp_dgain_enable_write(VI_PIPE ViPipe,
							  GK_U8 data)
{
	IOWR_8DIRECT(VREG_TOP_BASE(ViPipe) + 0x00f4, (data & 0x01));
}

static __inline GK_U8 ext_system_isp_dgain_enable_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_TOP_BASE(ViPipe) + 0x00f4) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_be_free_buffergh_addr
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_BE_FREE_BUFFER_HIGH_ADDR_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_BE_FREE_BUFFER_HIGH_ADDR_DATASIZE (32)

// args: data (32-bit)
static __inline GK_VOID ext_system_be_free_buffergh_addr_write(VI_PIPE ViPipe,
							       GK_U32 data)
{
	IOWR_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x0100, data);
}

static __inline GK_U32 ext_system_be_free_buffergh_addr_read(VI_PIPE ViPipe)
{
	return IORD_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x0100);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_be_free_buffer_low_addr
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_BE_FREE_BUFFER_LOW_ADDR_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_BE_FREE_BUFFER_LOW_ADDR_DATASIZE (32)

// args: data (32-bit)
static __inline GK_VOID ext_system_be_free_buffer_low_addr_write(VI_PIPE ViPipe,
								 GK_U32 data)
{
	IOWR_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x0104, data);
}

static __inline GK_U32 ext_system_be_free_buffer_low_addr_read(VI_PIPE ViPipe)
{
	return IORD_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x0104);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_manual_sharpen_overshootAmt
// ------------------------------------------------------------------------------ //
// sharpening overshoot Amt
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_ACTUAL_SHARPEN_OVERSHOOT_DEFAULT (150)

// args: data (8-bit)

static __inline GK_VOID
ext_system_actual_sharpen_overshootAmt_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_TOP_BASE(ViPipe) + 0x0108, data);
}
static __inline GK_U8
ext_system_actual_sharpen_overshootAmt_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_TOP_BASE(ViPipe) + 0x0108);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_manual_sharpen_undershootAmt_write
// ------------------------------------------------------------------------------ //
// sharpening undershoot Amt
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_ACTUAL_SHARPEN_UNDERSHOOT_DEFAULT (200)
// args: data (8-bit)
static __inline GK_VOID
ext_system_actual_sharpen_undershootAmt_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_TOP_BASE(ViPipe) + 0x010a, data);
}
static __inline GK_U8
ext_system_actual_sharpen_undershootAmt_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_TOP_BASE(ViPipe) + 0x010a);
}

#define ISP_EXT_SYSTEM_ACTUAL_SHARPEN_SHOOTSUPST_DEFAULT (1)

static __inline GK_VOID
ext_system_actual_sharpen_shootSupSt_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_TOP_BASE(ViPipe) + 0x010c, data);
}

static __inline GK_U8 ext_system_actual_sharpen_shootSupSt_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_TOP_BASE(ViPipe) + 0x010c);
}

static __inline GK_VOID
ext_system_actual_sharpen_edge_frequence_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x010e, data);
}

static __inline GK_U16
ext_system_actual_sharpen_edge_frequence_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x010e);
}

static __inline GK_VOID
ext_system_actual_sharpen_texture_frequence_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x0110, data);
}

static __inline GK_U16
ext_system_actual_sharpen_texture_frequence_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x0110);
}

static __inline GK_VOID ext_system_actual_sharpen_edge_str_write(VI_PIPE ViPipe,
								 GK_U8 index,
								 GK_U16 data)
{
	IOWR_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x0120 + index * sizeof(GK_U16),
		      data);
}

static __inline GK_U16 ext_system_actual_sharpen_edge_str_read(VI_PIPE ViPipe,
							       GK_U8 index)
{
	return IORD_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x0120 +
			     index * sizeof(GK_U16));
}

static __inline GK_VOID
ext_system_actual_sharpen_texture_str_write(VI_PIPE ViPipe, GK_U8 index,
					    GK_U16 data)
{
	IOWR_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x0160 + index * sizeof(GK_U16),
		      data);
}

static __inline GK_U16
ext_system_actual_sharpen_texture_str_read(VI_PIPE ViPipe, GK_U8 index)
{
	return IORD_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x0160 +
			     index * sizeof(GK_U16));
}

#define ISP_EXT_SYSTEM_BAYERNR_ACTUAL_STRENGTH_DEFAULT (100)

// args: data (8-bit)
static __inline GK_VOID
ext_system_bayernr_actual_fine_strength_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_TOP_BASE(ViPipe) + 0x01A0, data);
}
static __inline GK_U8
ext_system_bayernr_actual_fine_strength_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_TOP_BASE(ViPipe) + 0x01A0);
}

#define ISP_EXT_SYSTEM_BAYERNR_ACTUAL_CORING_WEIGHT_DEFAULT (800)
static __inline GK_VOID
ext_system_bayernr_actual_coring_weight_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x01A2, data);
}

static __inline GK_U16
ext_system_bayernr_actual_coring_weight_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x01A2);
}

static __inline GK_VOID
ext_system_bayernr_actual_nr_lsc_ratio_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_TOP_BASE(ViPipe) + 0x01A4, data);
}
static __inline GK_U8
ext_system_bayernr_actual_nr_lsc_ratio_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_TOP_BASE(ViPipe) + 0x01A4);
}

// args: data (8-bit)
static __inline GK_VOID
ext_system_bayernr_actual_chroma_strength_write(VI_PIPE ViPipe, GK_U8 i,
						GK_U8 data)
{
	IOWR_8DIRECT(VREG_TOP_BASE(ViPipe) + 0x01A8 + (i * sizeof(GK_U8)),
		     data);
}
static __inline GK_U8
ext_system_bayernr_actual_chroma_strength_read(VI_PIPE ViPipe, GK_U8 i)
{
	return IORD_8DIRECT(VREG_TOP_BASE(ViPipe) + 0x01A8 +
			    (i * sizeof(GK_U8)));
}

// args: data (8-bit)
static __inline GK_VOID
ext_system_bayernr_actual_wdr_frame_strength_write(VI_PIPE ViPipe, GK_U8 i,
						   GK_U8 data)
{
	IOWR_8DIRECT(VREG_TOP_BASE(ViPipe) + 0x01AC + (i * sizeof(GK_U8)),
		     data);
}
static __inline GK_U8
ext_system_bayernr_actual_wdr_frame_strength_read(VI_PIPE ViPipe, GK_U8 i)
{
	return IORD_8DIRECT(VREG_TOP_BASE(ViPipe) + 0x01AC +
			    (i * sizeof(GK_U8)));
}

// args: data (16-bit)
static __inline GK_VOID
ext_system_bayernr_actual_coarse_strength_write(VI_PIPE ViPipe, GK_U8 i,
						GK_U16 data)
{
	IOWR_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x0200 + (i * sizeof(GK_U16)),
		      data);
}
static __inline GK_U16
ext_system_bayernr_actual_coarse_strength_read(VI_PIPE ViPipe, GK_U8 i)
{
	return IORD_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x0200 +
			     (i * sizeof(GK_U16)));
}

static __inline GK_VOID ext_system_dehaze_actual_strength_write(VI_PIPE ViPipe,
								GK_U16 data)
{
	IOWR_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x0208, data);
}
static __inline GK_U16 ext_system_dehaze_actual_strength_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x0208);
}

static __inline GK_VOID ext_system_drc_actual_strength_write(VI_PIPE ViPipe,
							     GK_U16 data)
{
	IOWR_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x020A, data);
}
static __inline GK_U16 ext_system_drc_actual_strength_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x020A);
}

static __inline GK_VOID
ext_system_actual_wdr_exposure_ratio_write(VI_PIPE ViPipe, GK_U8 i, GK_U32 data)
{
	IOWR_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x0210 + (i * sizeof(GK_U32)),
		      data);
}
static __inline GK_U32 ext_system_actual_wdr_exposure_ratio_read(VI_PIPE ViPipe,
								 GK_U8 i)
{
	return IORD_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x0210 +
			     (i * sizeof(GK_U32)));
}

static __inline GK_VOID ext_system_block_num_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_TOP_BASE(ViPipe) + 0x220, (data & 0xff));
}
static __inline GK_U8 ext_system_block_num_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_TOP_BASE(ViPipe) + 0x220) & 0xff);
}

#define ISP_EXT_SYSTEM_TOP_CHANNEL_SELECT_WRITE_DEFAULT (0)
#define ISP_EXT_SYSTEM_TOP_CHANNEL_SELECT_WRITE_DATASIZE (2)

static __inline GK_VOID ext_system_top_channel_select_write(VI_PIPE ViPipe,
							    GK_U8 data)
{
	IOWR_8DIRECT(VREG_TOP_BASE(ViPipe) + 0x224, data & 0x3);
}

static __inline GK_U8 ext_system_top_channel_select_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_TOP_BASE(ViPipe) + 0x224) & 0x3);
}

#define ISP_EXT_SYSTEM_TOP_CHANNEL_SELECT_PRE_WRITE_DEFAULT (0)
#define ISP_EXT_SYSTEM_TOP_CHANNEL_SELECT_PRE_WRITE_DATASIZE (2)

static __inline GK_VOID ext_system_top_channel_select_pre_write(VI_PIPE ViPipe,
								GK_U8 data)
{
	IOWR_8DIRECT(VREG_TOP_BASE(ViPipe) + 0x226, data & 0x3);
}

static __inline GK_U8 ext_system_top_channel_select_pre_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_TOP_BASE(ViPipe) + 0x226) & 0x3);
}

// ------------------------------------------------------------------------------ //
// ISP Multi-Pipe Different Config Attr
// ------------------------------------------------------------------------------ //
// ------------------------------------------------------------------------------ //
// Register: ext_system_isp_pipe_diff_offset
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_ISP_PIPE_DIFF_OFFSET_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_ISP_PIPE_DIFF_OFFSET_DATASIZE (32)

// args: data (32-bit)
static __inline GK_VOID
ext_system_isp_pipe_diff_offset_write(VI_PIPE ViPipe, GK_U8 index, GK_S32 data)
{
	IOWR_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x230 + (index << 2), data);
}
static __inline GK_S32 ext_system_isp_pipe_diff_offset_read(VI_PIPE ViPipe,
							    GK_U8 index)
{
	return IORD_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x230 + (index << 2));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_isp_pipe_diff_gain
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_ISP_PIPE_DIFF_GAIN_DEFAULT (0x100)
#define ISP_EXT_SYSTEM_ISP_PIPE_DIFF_GAIN_DATASIZE (32)

// args: data (32-bit)
static __inline GK_VOID
ext_system_isp_pipe_diff_gain_write(VI_PIPE ViPipe, GK_U8 index, GK_U32 data)
{
	IOWR_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x240 + (index << 2), data);
}
static __inline GK_U32 ext_system_isp_pipe_diff_gain_read(VI_PIPE ViPipe,
							  GK_U8 index)
{
	return IORD_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x240 + (index << 2));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_isp_matrix_coefft
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_ISP_PIPE_DIFF_CCM_DEFAULT (0x100)
#define ISP_EXT_SYSTEM_ISP_PIPE_DIFF_CCM_DATASIZE (16)

// args: data (16-bit)
static __inline GK_VOID
ext_system_isp_pipe_diff_ccm_write(VI_PIPE ViPipe, GK_U8 index, GK_U16 data)
{
	IOWR_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x250 + (index << 1), data);
}
static __inline GK_U16 ext_system_isp_pipe_diff_ccm_read(VI_PIPE ViPipe,
							 GK_U8 index)
{
	return IORD_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x250 + (index << 1));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_black_level_query
// ------------------------------------------------------------------------------ //
// R channel Black level
static __inline GK_VOID ext_system_black_level_query_00_write(VI_PIPE ViPipe,
							      GK_U16 data)
{
	IOWR_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x270, data);
}

static __inline GK_U16 ext_system_black_level_query_00_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x270);
}
// Gr channel Black level
static __inline GK_VOID ext_system_black_level_query_01_write(VI_PIPE ViPipe,
							      GK_U16 data)
{
	IOWR_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x272, data);
}

static __inline GK_U16 ext_system_black_level_query_01_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x272);
}

// Gb channel Black level
static __inline GK_VOID ext_system_black_level_query_10_write(VI_PIPE ViPipe,
							      GK_U16 data)
{
	IOWR_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x274, data);
}

static __inline GK_U16 ext_system_black_level_query_10_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x274);
}

// B channel Black level
static __inline GK_VOID ext_system_black_level_query_11_write(VI_PIPE ViPipe,
							      GK_U16 data)
{
	IOWR_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x276, data);
}

static __inline GK_U16 ext_system_black_level_query_11_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_TOP_BASE(ViPipe) + 0x276);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_be_free_buffergh_addr
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_BE_LUT_STT_BUFFER_HIGH_ADDR_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_BE_LUT_STT_BUFFER_HIGH_ADDR_DATASIZE (32)

// args: data (32-bit)
static __inline GK_VOID
ext_system_be_lut_stt_buffergh_addr_write(VI_PIPE ViPipe, GK_U32 data)
{
	IOWR_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x278, data);
}

static __inline GK_U32 ext_system_be_lut_stt_buffergh_addr_read(VI_PIPE ViPipe)
{
	return IORD_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x278);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_be_free_buffer_low_addr
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_BE_LUT_STT_BUFFER_LOW_ADDR_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_BE_LUT_STT_BUFFER_LOW_ADDR_DATASIZE (32)

// args: data (32-bit)
static __inline GK_VOID
ext_system_be_lut_stt_buffer_low_addr_write(VI_PIPE ViPipe, GK_U32 data)
{
	IOWR_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x27c, data);
}

static __inline GK_U32
ext_system_be_lut_stt_buffer_low_addr_read(VI_PIPE ViPipe)
{
	return IORD_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x27c);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_be_buffer_addressgh
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_BE_BUFFER_ADDRESS_HIGH_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_BE_BUFFER_ADDRESS_HIGH_DATASIZE (32)

// args: data (32-bit)
static __inline GK_VOID ext_system_be_buffer_addressgh_write(VI_PIPE ViPipe,
							     GK_U32 data)
{
	IOWR_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x02a0, data);
}

static __inline GK_U32 ext_system_be_buffer_addressgh_read(VI_PIPE ViPipe)
{
	return IORD_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x02a0);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_be_buffer_address_low
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_BE_BUFFER_ADDRESS_LOW_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_BE_BUFFER_ADDRESS_LOW_DATASIZE (32)

// args: data (32-bit)
static __inline GK_VOID ext_system_be_buffer_address_low_write(VI_PIPE ViPipe,
							       GK_U32 data)
{
	IOWR_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x02a4, data);
}

static __inline GK_U32 ext_system_be_buffer_address_low_read(VI_PIPE ViPipe)
{
	return IORD_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x02a4);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_bebuf_size
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_BEBUF_SIZE_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_BEBUF_SIZE_DATASIZE (32)

// args: data (32-bit)
static __inline GK_VOID ext_system_be_buffer_size_write(VI_PIPE ViPipe,
							GK_U32 data)
{
	IOWR_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x02a8, data);
}

static __inline GK_U32 ext_system_be_buffer_size_read(VI_PIPE ViPipe)
{
	return IORD_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x02a8);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_ldci_read_stt_buffergh_addr
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_LDCI_READ_STT_BUF_HIGH_ADDR_DEFAULT (0x0)
// args: data (32-bit)
static __inline GK_VOID
ext_system_ldci_read_stt_buffergh_addr_write(VI_PIPE ViPipe, GK_U32 data)
{
	IOWR_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x2ac, data);
}

static __inline GK_U32
ext_system_ldci_read_stt_buffergh_addr_read(VI_PIPE ViPipe)
{
	return IORD_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x2ac);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_ldci_read_stt_buffer_low_addr
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_LDCI_READ_STT_BUF_LOW_ADDR_DEFAULT (0x0)

// args: data (32-bit)
static __inline GK_VOID
ext_system_ldci_read_stt_buffer_low_addr_write(VI_PIPE ViPipe, GK_U32 data)
{
	IOWR_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x2b0, data);
}

static __inline GK_U32
ext_system_ldci_read_stt_buffer_low_addr_read(VI_PIPE ViPipe)
{
	return IORD_32DIRECT(VREG_TOP_BASE(ViPipe) + 0x2b0);
}

// ------------------------------------------------------------------------------ //
// Register: ext_af_enable
// ------------------------------------------------------------------------------ //
// Used to
// ------------------------------------------------------------------------------ //

// args: data (1-bit)
static __inline GK_VOID ext_af_enable_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0000, (data & 0x01));
}
static __inline GK_U8 ext_af_enable_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0000) & 0x01);
}

static __inline GK_VOID ext_af_iir0_enable0_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0001, (data & 0x01));
}
static __inline GK_U8 ext_af_iir0_enable0_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0001) & 0x01);
}

static __inline GK_VOID ext_af_iir0_enable1_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0002, (data & 0x01));
}
static __inline GK_U8 ext_af_iir0_enable1_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0002) & 0x01);
}

static __inline GK_VOID ext_af_iir0_enable2_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0003, (data & 0x01));
}
static __inline GK_U8 ext_af_iir0_enable2_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0003) & 0x01);
}

static __inline GK_VOID ext_af_iir1_enable0_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0004, (data & 0x01));
}
static __inline GK_U8 ext_af_iir1_enable0_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0004) & 0x01);
}

static __inline GK_VOID ext_af_iir1_enable1_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0005, (data & 0x01));
}
static __inline GK_U8 ext_af_iir1_enable1_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0005) & 0x01);
}

static __inline GK_VOID ext_af_iir1_enable2_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0006, (data & 0x01));
}
static __inline GK_U8 ext_af_iir1_enable2_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0006) & 0x01);
}

static __inline GK_VOID ext_af_peakmode_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0007, (data & 0x01));
}
static __inline GK_U8 ext_af_peakmode_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0007) & 0x01);
}

static __inline GK_VOID ext_af_squmode_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0008, (data & 0x01));
}
static __inline GK_U8 ext_af_squmode_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0008) & 0x01);
}

static __inline GK_VOID ext_af_offset_enable_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0009, (data & 0x01));
}
static __inline GK_U8 ext_af_offset_enable_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0009) & 0x01);
}

static __inline GK_VOID ext_af_crop_enable_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x000a, (data & 0x01));
}
static __inline GK_U8 ext_af_crop_enable_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x000a) & 0x01);
}

static __inline GK_VOID ext_af_lpf_enable_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x000b, (data & 0x01));
}
static __inline GK_U8 ext_af_lpf_enable_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x000b) & 0x01);
}

static __inline GK_VOID ext_af_mean_enable_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x000c, (data & 0x01));
}
static __inline GK_U8 ext_af_mean_enable_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x000c) & 0x01);
}

static __inline GK_VOID ext_af_rawmode_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x000d, (data & 0x01));
}
static __inline GK_U8 ext_af_rawmode_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x000d) & 0x01);
}

static __inline GK_VOID ext_af_iir0_ds_enable_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x000e, (data & 0x01));
}
static __inline GK_U8 ext_af_iir0_ds_enable_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x000e) & 0x01);
}

static __inline GK_VOID ext_af_iir1_ds_enable_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x000f, (data & 0x01));
}
static __inline GK_U8 ext_af_iir1_ds_enable_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x000f) & 0x01);
}

static __inline GK_VOID ext_af_fir0_lpf_enable_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0010, (data & 0x01));
}
static __inline GK_U8 ext_af_fir0_lpf_enable_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0010) & 0x01);
}

static __inline GK_VOID ext_af_fir1_lpf_enable_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0011, (data & 0x01));
}
static __inline GK_U8 ext_af_fir1_lpf_enable_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0011) & 0x01);
}

static __inline GK_VOID ext_af_iir0_ldg_enable_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0012, (data & 0x01));
}
static __inline GK_U8 ext_af_iir0_ldg_enable_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0012) & 0x01);
}

static __inline GK_VOID ext_af_iir1_ldg_enable_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0013, (data & 0x01));
}
static __inline GK_U8 ext_af_iir1_ldg_enable_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0013) & 0x01);
}

static __inline GK_VOID ext_af_fir0_ldg_enable_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0014, (data & 0x01));
}
static __inline GK_U8 ext_af_fir0_ldg_enable_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0014) & 0x01);
}

static __inline GK_VOID ext_af_fir1_ldg_enable_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0015, (data & 0x01));
}
static __inline GK_U8 ext_af_fir1_ldg_enable_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0015) & 0x01);
}

static __inline GK_VOID ext_af_updatemode_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0016, (data & 0x01));
}
static __inline GK_U8 ext_af_updatemode_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0016) & 0x01);
}

static __inline GK_VOID ext_af_update_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0017, (data & 0x01));
}
static __inline GK_U8 ext_af_update_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0017) & 0x01);
}

// 8bit

static __inline GK_VOID ext_af_bayermode_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0018, (data & 0xff));
}
static __inline GK_U8 ext_af_bayermode_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0018) & 0xff);
}

static __inline GK_VOID ext_af_gamma_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0019, (data & 0xff));
}
static __inline GK_U8 ext_af_gamma_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0019) & 0xff);
}

static __inline GK_VOID ext_af_gain_limit_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x001a, (data & 0xff));
}
static __inline GK_U8 ext_af_gain_limit_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x001a) & 0xff);
}

static __inline GK_VOID ext_af_iir_plg_group0_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x001b, (data & 0xff));
}
static __inline GK_U8 ext_af_iir_plg_group0_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x001b) & 0xff);
}

static __inline GK_VOID ext_af_iir_pls_group0_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x001c, (data & 0xff));
}
static __inline GK_U8 ext_af_iir_pls_group0_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x001c) & 0xff);
}

static __inline GK_VOID ext_af_iir_plg_group1_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x001d, (data & 0xff));
}
static __inline GK_U8 ext_af_iir_plg_group1_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x001d) & 0xff);
}

static __inline GK_VOID ext_af_iir_pls_group1_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x001e, (data & 0xff));
}
static __inline GK_U8 ext_af_iir_pls_group1_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x001e) & 0xff);
}

static __inline GK_VOID ext_af_cnt_shift0_h_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x001f, (data & 0xff));
}
static __inline GK_U8 ext_af_cnt_shift0_h_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x001f) & 0xff);
}

static __inline GK_VOID ext_af_cnt_shift1_h_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0020, (data & 0xff));
}
static __inline GK_U8 ext_af_cnt_shift1_h_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0020) & 0xff);
}

static __inline GK_VOID ext_af_cnt_shift0_v_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0021, (data & 0xff));
}
static __inline GK_U8 ext_af_cnt_shift0_v_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0021) & 0xff);
}

static __inline GK_VOID ext_af_cnt_shift_y_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0022, (data & 0xff));
}
static __inline GK_U8 ext_af_cnt_shift_y_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0022) & 0xff);
}

static __inline GK_VOID ext_af_iir_dilate0_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0023, (data & 0xff));
}
static __inline GK_U8 ext_af_iir_dilate0_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0023) & 0xff);
}

static __inline GK_VOID ext_af_iir_dilate1_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0024, (data & 0xff));
}
static __inline GK_U8 ext_af_iir_dilate1_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0024) & 0xff);
}

static __inline GK_VOID ext_afligh_thre_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0025, (data & 0xff));
}
static __inline GK_U8 ext_afligh_thre_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0025) & 0xff);
}

static __inline GK_VOID ext_af_pos_sel_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0026, (data & 0xff));
}
static __inline GK_U8 ext_af_pos_sel_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0026) & 0xff);
}

#define ISP_EXT_AF_SET_FLAG_DISABLE (0x0)
#define ISP_EXT_AF_SET_FLAG_ENABLE (0x1)
static __inline GK_VOID ext_af_set_flag_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0027, (data & 0xff));
}
static __inline GK_U8 ext_af_set_flag_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x0027) & 0xff);
}

// args: data (16-bit)
static __inline GK_VOID ext_af_mean_thres_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x0028), data);
}
static __inline GK_U16 ext_af_mean_thres_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x0028));
}

static __inline GK_VOID ext_af_window_vnum_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x002a), data);
}
static __inline GK_U16 ext_af_window_vnum_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x002a));
}

static __inline GK_VOID ext_af_window_hnum_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x002c), data);
}
static __inline GK_U16 ext_af_window_hnum_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x002c));
}

static __inline GK_VOID ext_af_iir_thre0_coring_write(VI_PIPE ViPipe,
						      GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x002e), data);
}
static __inline GK_U16 ext_af_iir_thre0_coring_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x002e));
}

static __inline GK_VOID ext_af_iir_thre1_coring_write(VI_PIPE ViPipe,
						      GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x0030), data);
}
static __inline GK_U16 ext_af_iir_thre1_coring_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x0030));
}

static __inline GK_VOID ext_af_iir_peak0_coring_write(VI_PIPE ViPipe,
						      GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x0032), data);
}
static __inline GK_U16 ext_af_iir_peak0_coring_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x0032));
}

static __inline GK_VOID ext_af_iir_peak1_coring_write(VI_PIPE ViPipe,
						      GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x0034), data);
}
static __inline GK_U16 ext_af_iir_peak1_coring_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x0034));
}

static __inline GK_VOID ext_af_fir_thre0_coring_write(VI_PIPE ViPipe,
						      GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x0036), data);
}
static __inline GK_U16 ext_af_fir_thre0_coring_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x0036));
}

static __inline GK_VOID ext_af_fir_thre1_coring_write(VI_PIPE ViPipe,
						      GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x0038), data);
}
static __inline GK_U16 ext_af_fir_thre1_coring_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x0038));
}

static __inline GK_VOID ext_af_fir_peak0_coring_write(VI_PIPE ViPipe,
						      GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x003a), data);
}
static __inline GK_U16 ext_af_fir_peak0_coring_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x003a));
}

static __inline GK_VOID ext_af_fir_peak1_coring_write(VI_PIPE ViPipe,
						      GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x003c), data);
}
static __inline GK_U16 ext_af_fir_peak1_coring_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x003c));
}

static __inline GK_VOID ext_af_iir_slope0_coring_write(VI_PIPE ViPipe,
						       GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x003e), data);
}
static __inline GK_U16 ext_af_iir_slope0_coring_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x003e));
}

static __inline GK_VOID ext_af_iir_slope1_coring_write(VI_PIPE ViPipe,
						       GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x0040), data);
}
static __inline GK_U16 ext_af_iir_slope1_coring_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x0040));
}

static __inline GK_VOID ext_af_fir_slope0_coring_write(VI_PIPE ViPipe,
						       GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x0042), data);
}
static __inline GK_U16 ext_af_fir_slope0_coring_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x0042));
}

static __inline GK_VOID ext_af_fir_slope1_coring_write(VI_PIPE ViPipe,
						       GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x0044), data);
}
static __inline GK_U16 ext_af_fir_slope1_coring_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x0044));
}

static __inline GK_VOID ext_af_offset_gr_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x0046), data);
}
static __inline GK_U16 ext_af_offset_gr_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x0046));
}

static __inline GK_VOID ext_af_offset_gb_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x0048), data);
}
static __inline GK_U16 ext_af_offset_gb_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x0048));
}

static __inline GK_VOID ext_af_input_vsize_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x004a), data);
}
static __inline GK_U16 ext_af_input_vsize_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x004a));
}

static __inline GK_VOID ext_af_input_hsize_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x004c), data);
}
static __inline GK_U16 ext_af_input_hsize_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x004c));
}

static __inline GK_VOID ext_af_iir0_shift_group0_write(VI_PIPE ViPipe,
						       GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x004e), data);
}
static __inline GK_U16 ext_af_iir0_shift_group0_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x004e));
}

static __inline GK_VOID ext_af_iir1_shift_group0_write(VI_PIPE ViPipe,
						       GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x0050), data);
}
static __inline GK_U16 ext_af_iir1_shift_group0_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x0050));
}

static __inline GK_VOID ext_af_iir2_shift_group0_write(VI_PIPE ViPipe,
						       GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x0052), data);
}
static __inline GK_U16 ext_af_iir2_shift_group0_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x0052));
}

static __inline GK_VOID ext_af_iir3_shift_group0_write(VI_PIPE ViPipe,
						       GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x0054), data);
}
static __inline GK_U16 ext_af_iir3_shift_group0_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x0054));
}

static __inline GK_VOID ext_af_iir0_shift_group1_write(VI_PIPE ViPipe,
						       GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x0056), data);
}
static __inline GK_U16 ext_af_iir0_shift_group1_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x0056));
}

static __inline GK_VOID ext_af_iir1_shift_group1_write(VI_PIPE ViPipe,
						       GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x0058), data);
}
static __inline GK_U16 ext_af_iir1_shift_group1_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x0058));
}

static __inline GK_VOID ext_af_iir2_shift_group1_write(VI_PIPE ViPipe,
						       GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x005a), data);
}
static __inline GK_U16 ext_af_iir2_shift_group1_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x005a));
}

static __inline GK_VOID ext_af_iir3_shift_group1_write(VI_PIPE ViPipe,
						       GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x005c), data);
}
static __inline GK_U16 ext_af_iir3_shift_group1_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x005c));
}

static __inline GK_VOID ext_af_iir_thre0_low_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x005e), data);
}
static __inline GK_U16 ext_af_iir_thre0_low_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x005e));
}

static __inline GK_VOID ext_af_iir_thre0gh_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x0060), data);
}
static __inline GK_U16 ext_af_iir_thre0gh_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x0060));
}

static __inline GK_VOID ext_af_iir_thre1_low_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x0062), data);
}
static __inline GK_U16 ext_af_iir_thre1_low_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x0062));
}

static __inline GK_VOID ext_af_iir_thre1gh_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x0064), data);
}
static __inline GK_U16 ext_af_iir_thre1gh_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x0064));
}

static __inline GK_VOID ext_af_iir_gain0_low_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x0066), data);
}
static __inline GK_U16 ext_af_iir_gain0_low_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x0066));
}

static __inline GK_VOID ext_af_iir_gain0gh_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x0068), data);
}
static __inline GK_U16 ext_af_iir_gain0gh_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x0068));
}

static __inline GK_VOID ext_af_iir_gain1_low_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x006a), data);
}
static __inline GK_U16 ext_af_iir_gain1_low_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x006a));
}

static __inline GK_VOID ext_af_iir_gain1gh_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x006c), data);
}
static __inline GK_U16 ext_af_iir_gain1gh_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x006c));
}

static __inline GK_VOID ext_af_iir_slope0_low_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x006e), data);
}
static __inline GK_U16 ext_af_iir_slope0_low_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x006e));
}

static __inline GK_VOID ext_af_iir_slope0gh_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x0070), data);
}
static __inline GK_U16 ext_af_iir_slope0gh_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x0070));
}

static __inline GK_VOID ext_af_iir_slope1_low_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x0072), data);
}
static __inline GK_U16 ext_af_iir_slope1_low_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x0072));
}

static __inline GK_VOID ext_af_iir_slope1gh_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x0074), data);
}
static __inline GK_U16 ext_af_iir_slope1gh_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x0074));
}

static __inline GK_VOID ext_af_fir_thre0_low_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x0076), data);
}
static __inline GK_U16 ext_af_fir_thre0_low_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x0076));
}

static __inline GK_VOID ext_af_fir_thre0gh_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x0078), data);
}
static __inline GK_U16 ext_af_fir_thre0gh_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x0078));
}

static __inline GK_VOID ext_af_fir_thre1_low_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x007a), data);
}
static __inline GK_U16 ext_af_fir_thre1_low_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x007a));
}

static __inline GK_VOID ext_af_fir_thre1gh_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x007c), data);
}
static __inline GK_U16 ext_af_fir_thre1gh_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x007c));
}

static __inline GK_VOID ext_af_fir_gain0_low_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x007e), data);
}
static __inline GK_U16 ext_af_fir_gain0_low_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x007e));
}

static __inline GK_VOID ext_af_fir_gain0gh_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x0080), data);
}
static __inline GK_U16 ext_af_fir_gain0gh_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x0080));
}

static __inline GK_VOID ext_af_fir_gain1_low_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x0082), data);
}
static __inline GK_U16 ext_af_fir_gain1_low_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x0082));
}

static __inline GK_VOID ext_af_fir_gain1gh_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x0084), data);
}
static __inline GK_U16 ext_af_fir_gain1gh_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x0084));
}

static __inline GK_VOID ext_af_fir_slope0_low_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x0086), data);
}
static __inline GK_U16 ext_af_fir_slope0_low_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x0086));
}

static __inline GK_VOID ext_af_fir_slope0gh_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x0088), data);
}
static __inline GK_U16 ext_af_fir_slope0gh_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x0088));
}

static __inline GK_VOID ext_af_fir_slope1_low_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x008a), data);
}
static __inline GK_U16 ext_af_fir_slope1_low_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x008a));
}

static __inline GK_VOID ext_af_fir_slope1gh_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x008c), data);
}
static __inline GK_U16 ext_af_fir_slope1gh_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x008c));
}

static __inline GK_VOID ext_af_acc_shift0_h_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x008e), data);
}
static __inline GK_U16 ext_af_acc_shift0_h_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x008e));
}

static __inline GK_VOID ext_af_acc_shift1_h_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x0090), data);
}
static __inline GK_U16 ext_af_acc_shift1_h_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x0090));
}

static __inline GK_VOID ext_af_acc_shift0_v_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x0092), data);
}
static __inline GK_U16 ext_af_acc_shift0_v_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x0092));
}

static __inline GK_VOID ext_af_acc_shift1_v_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x0094), data);
}
static __inline GK_U16 ext_af_acc_shift1_v_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x0094));
}

static __inline GK_VOID ext_af_acc_shift_y_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x0096), data);
}
static __inline GK_U16 ext_af_acc_shift_y_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x0096));
}

static __inline GK_VOID ext_af_crop_pos_y_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x0098), data);
}
static __inline GK_U16 ext_af_crop_pos_y_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x0098));
}

static __inline GK_VOID ext_af_crop_pos_x_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x009a), data);
}
static __inline GK_U16 ext_af_crop_pos_x_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x009a));
}

static __inline GK_VOID ext_af_crop_vsize_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x009c), data);
}
static __inline GK_U16 ext_af_crop_vsize_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x009c));
}

static __inline GK_VOID ext_af_crop_hsize_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x009e), data);
}
static __inline GK_U16 ext_af_crop_hsize_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x009e));
}

static __inline GK_VOID ext_af_iir_gain0_group0_write(VI_PIPE ViPipe,
						      GK_S16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x00a0), data);
}
static __inline GK_S16 ext_af_iir_gain0_group0_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x00a0));
}

static __inline GK_VOID ext_af_iir_gain0_group1_write(VI_PIPE ViPipe,
						      GK_S16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x00a2), data);
}
static __inline GK_S16 ext_af_iir_gain0_group1_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x00a2));
}

static __inline GK_VOID ext_af_iir_gain1_group0_write(VI_PIPE ViPipe,
						      GK_S16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x00a4), data);
}
static __inline GK_S16 ext_af_iir_gain1_group0_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x00a4));
}

static __inline GK_VOID ext_af_iir_gain1_group1_write(VI_PIPE ViPipe,
						      GK_S16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x00a6), data);
}
static __inline GK_S16 ext_af_iir_gain1_group1_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x00a6));
}

static __inline GK_VOID ext_af_iir_gain2_group0_write(VI_PIPE ViPipe,
						      GK_S16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x00a8), data);
}
static __inline GK_S16 ext_af_iir_gain2_group0_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x00a8));
}

static __inline GK_VOID ext_af_iir_gain2_group1_write(VI_PIPE ViPipe,
						      GK_S16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x00aa), data);
}
static __inline GK_S16 ext_af_iir_gain2_group1_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x00aa));
}

static __inline GK_VOID ext_af_iir_gain3_group0_write(VI_PIPE ViPipe,
						      GK_S16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x00ac), data);
}
static __inline GK_S16 ext_af_iir_gain3_group0_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x00ac));
}

static __inline GK_VOID ext_af_iir_gain3_group1_write(VI_PIPE ViPipe,
						      GK_S16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x00ae), data);
}
static __inline GK_S16 ext_af_iir_gain3_group1_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x00ae));
}

static __inline GK_VOID ext_af_iir_gain4_group0_write(VI_PIPE ViPipe,
						      GK_S16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x00b0), data);
}
static __inline GK_S16 ext_af_iir_gain4_group0_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x00b0));
}

static __inline GK_VOID ext_af_iir_gain4_group1_write(VI_PIPE ViPipe,
						      GK_S16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x00b2), data);
}
static __inline GK_S16 ext_af_iir_gain4_group1_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x00b2));
}

static __inline GK_VOID ext_af_iir_gain5_group0_write(VI_PIPE ViPipe,
						      GK_S16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x00b4), data);
}
static __inline GK_S16 ext_af_iir_gain5_group0_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x00b4));
}

static __inline GK_VOID ext_af_iir_gain5_group1_write(VI_PIPE ViPipe,
						      GK_S16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x00b6), data);
}
static __inline GK_S16 ext_af_iir_gain5_group1_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x00b6));
}

static __inline GK_VOID ext_af_iir_gain6_group0_write(VI_PIPE ViPipe,
						      GK_S16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x00b8), data);
}
static __inline GK_S16 ext_af_iir_gain6_group0_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x00b8));
}

static __inline GK_VOID ext_af_iir_gain6_group1_write(VI_PIPE ViPipe,
						      GK_S16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x00ba), data);
}
static __inline GK_S16 ext_af_iir_gain6_group1_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x00ba));
}

static __inline GK_VOID ext_af_fir_h_gain0_group0_write(VI_PIPE ViPipe,
							GK_S16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x00bc), data);
}
static __inline GK_S16 ext_af_fir_h_gain0_group0_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x00bc));
}

static __inline GK_VOID ext_af_fir_h_gain0_group1_write(VI_PIPE ViPipe,
							GK_S16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x00be), data);
}
static __inline GK_S16 ext_af_fir_h_gain0_group1_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x00be));
}

static __inline GK_VOID ext_af_fir_h_gain1_group0_write(VI_PIPE ViPipe,
							GK_S16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x00c0), data);
}
static __inline GK_S16 ext_af_fir_h_gain1_group0_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x00c0));
}

static __inline GK_VOID ext_af_fir_h_gain1_group1_write(VI_PIPE ViPipe,
							GK_S16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x00c2), data);
}
static __inline GK_S16 ext_af_fir_h_gain1_group1_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x00c2));
}

static __inline GK_VOID ext_af_fir_h_gain2_group0_write(VI_PIPE ViPipe,
							GK_S16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x00c4), data);
}
static __inline GK_S16 ext_af_fir_h_gain2_group0_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x00c4));
}

static __inline GK_VOID ext_af_fir_h_gain2_group1_write(VI_PIPE ViPipe,
							GK_S16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x00c6), data);
}
static __inline GK_S16 ext_af_fir_h_gain2_group1_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x00c6));
}

static __inline GK_VOID ext_af_fir_h_gain3_group0_write(VI_PIPE ViPipe,
							GK_S16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x00c8), data);
}
static __inline GK_S16 ext_af_fir_h_gain3_group0_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x00c8));
}

static __inline GK_VOID ext_af_fir_h_gain3_group1_write(VI_PIPE ViPipe,
							GK_S16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x00ca), data);
}
static __inline GK_S16 ext_af_fir_h_gain3_group1_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x00ca));
}

static __inline GK_VOID ext_af_fir_h_gain4_group0_write(VI_PIPE ViPipe,
							GK_S16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x00cc), data);
}
static __inline GK_S16 ext_af_fir_h_gain4_group0_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x00cc));
}

static __inline GK_VOID ext_af_fir_h_gain4_group1_write(VI_PIPE ViPipe,
							GK_S16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x00ce), data);
}
static __inline GK_S16 ext_af_fir_h_gain4_group1_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x00ce));
}

static __inline GK_VOID ext_af_shift_count_y_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AF_BASE(ViPipe) + 0x00d0), data);
}
static __inline GK_U16 ext_af_shift_count_y_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_AF_BASE(ViPipe) + 0x00d0));
}

// 8bit
static __inline GK_VOID ext_af_iir0_shift_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x00d2, (data & 0xff));
}
static __inline GK_U8 ext_af_iir0_shift_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x00d2) & 0xff);
}

static __inline GK_VOID ext_af_iir1_shift_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x00d3, (data & 0xff));
}
static __inline GK_U8 ext_af_iir1_shift_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x00d3) & 0xff);
}

// args: data (2-bit)
// * AF sta info enable, [0] = FE sta,[1] = BE sta *//
static __inline GK_VOID ext_system_af_enable_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_AF_BASE(ViPipe) + 0x00e0,
		     (data & ISP_AF_ENABLE_MASK));
}
static __inline GK_U8 ext_system_af_enable_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AF_BASE(ViPipe) + 0x00e0) &
		ISP_AF_ENABLE_MASK);
}

// ============================================================//
// DRC
// ============================================================//
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_enable_
// ------------------------------------------------------------------------------ //
// DRC Module Enable
// ------------------------------------------------------------------------------ //
static __inline GK_VOID ext_system_drc_enable_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x0, data);
}
static __inline GK_U8 ext_system_drc_enable_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x0);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_manual_mode
// ------------------------------------------------------------------------------ //
// Manual Mode Enable of Drc Module
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_manual_mode_write(VI_PIPE ViPipe,
							 GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x1), data);
}
static __inline GK_U8 ext_system_drc_manual_mode_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x1));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_monochrome_mode
// ------------------------------------------------------------------------------ //
// Monochrome Mode Enable of Drc Module
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_monochrome_mode_write(VI_PIPE ViPipe,
							     GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x2), data);
}
static __inline GK_U8 ext_system_drc_monochrome_mode_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x2));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_asymmetry
// ------------------------------------------------------------------------------ //
// Asymmetry parameter of DRC Asymmetry Curve
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_asymmetry_write(VI_PIPE ViPipe,
						       GK_U8 data)
{
	IOWR_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x5, data);
}
static __inline GK_U8 ext_system_drc_asymmetry_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x5));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_secondpole
// ------------------------------------------------------------------------------ //
// Secondpole parameter of DRC Asymmetry Curve
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_secondpole_write(VI_PIPE ViPipe,
							GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x6), data);
}
static __inline GK_U8 ext_system_drc_secondpole_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x6));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_compress
// ------------------------------------------------------------------------------ //
// Compress parameter of DRC Asymmetry Curve
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_compress_write(VI_PIPE ViPipe,
						      GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x7), data);
}
static __inline GK_U8 ext_system_drc_compress_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x7));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_stretch
// ------------------------------------------------------------------------------ //
// Stretch parameter of DRC Asymmetry Curve
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_stretch_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x8), data);
}
static __inline GK_U8 ext_system_drc_stretch_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x8));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_colorcc_lut_up_en
// ------------------------------------------------------------------------------ //
// colorcc_lut_up_en
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_colorcc_lut_up_en_write(VI_PIPE ViPipe,
							       GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x9), data);
}
static __inline GK_U8 ext_system_drc_colorcc_lut_up_en_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x9));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_bin_numz
// ------------------------------------------------------------------------------ //
// bin_num_z
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_bin_num_z_write(VI_PIPE ViPipe,
						       GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0xa), data);
}
static __inline GK_U8 ext_system_drc_bin_num_z_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0xa));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_mixing_coring
// ------------------------------------------------------------------------------ //
// drc_mixing_coring
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_mixing_coring_write(VI_PIPE ViPipe,
							   GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0xb), data);
}
static __inline GK_U8 ext_system_drc_mixing_coring_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0xb));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_mixing_dark_min
// ------------------------------------------------------------------------------ //
// mixing_dark_min
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_mixing_dark_min_write(VI_PIPE ViPipe,
							     GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0xc), data);
}
static __inline GK_U8 ext_system_drc_mixing_dark_min_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0xc));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_mixing_dark_max
// ------------------------------------------------------------------------------ //
// mixing_dark_max
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_mixing_dark_max_write(VI_PIPE ViPipe,
							     GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0xd), data);
}
static __inline GK_U8 ext_system_drc_mixing_dark_max_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0xd));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_mixing_dark_slo
// ------------------------------------------------------------------------------ //
// mixing_dark_slo
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_mixing_dark_slo_write(VI_PIPE ViPipe,
							     GK_S8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0xe), data);
}
static __inline GK_S8 ext_system_drc_mixing_dark_slo_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0xe));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_mixing_dark_thr
// ------------------------------------------------------------------------------ //
// mixing_dark_thr
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_mixing_dark_thr_write(VI_PIPE ViPipe,
							     GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0xf), data);
}
static __inline GK_U8 ext_system_drc_mixing_dark_thr_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0xf));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_mixing_bright_min
// ------------------------------------------------------------------------------ //
// mixing_bright_min
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_mixing_bright_min_write(VI_PIPE ViPipe,
							       GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x10), data);
}
static __inline GK_U8 ext_system_drc_mixing_bright_min_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x10));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_mixing_bright_max
// ------------------------------------------------------------------------------ //
// mixing_bright_max
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_mixing_bright_max_write(VI_PIPE ViPipe,
							       GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x11), data);
}
static __inline GK_U8 ext_system_drc_mixing_bright_max_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x11));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_mixing_bright_slo
// ------------------------------------------------------------------------------ //
// mixing_bright_slo
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_mixing_bright_slo_write(VI_PIPE ViPipe,
							       GK_S8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x12), data);
}
static __inline GK_S8 ext_system_drc_mixing_bright_slo_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x12));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_mixing_bright_thr
// ------------------------------------------------------------------------------ //
// mixing_bright_thr
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_mixing_bright_thr_write(VI_PIPE ViPipe,
							       GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x13), data);
}
static __inline GK_U8 ext_system_drc_mixing_bright_thr_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x13));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_gain_clip_knee
// ------------------------------------------------------------------------------ //
// mixing_gain_clip_knee
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_gain_clip_knee_write(VI_PIPE ViPipe,
							    GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x14), data);
}
static __inline GK_U8 ext_system_drc_gain_clip_knee_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x14));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_gain_clip_step
// ------------------------------------------------------------------------------ //
// mixing_gain_clip_step
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_gain_clip_step_write(VI_PIPE ViPipe,
							    GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x15), data);
}
static __inline GK_U8 ext_system_drc_gain_clip_step_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x15));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_purplegh_slo
// ------------------------------------------------------------------------------ //
// purplegh_slo
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_purplegh_slo_write(VI_PIPE ViPipe,
							  GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x16), data);
}
static __inline GK_U8 ext_system_drc_purplegh_slo_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x16));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_purplegh_thr
// ------------------------------------------------------------------------------ //
// purplegh_thr
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_purplegh_thr_write(VI_PIPE ViPipe,
							  GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x17), data);
}
static __inline GK_U8 ext_system_drc_purplegh_thr_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x17));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_purple_low_slo
// ------------------------------------------------------------------------------ //
// purple_low_slo
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_purple_low_slo_write(VI_PIPE ViPipe,
							    GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x18), data);
}
static __inline GK_U8 ext_system_drc_purple_low_slo_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x18));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_purple_low_thr
// ------------------------------------------------------------------------------ //
// purple_low_thr
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_purple_low_thr_write(VI_PIPE ViPipe,
							    GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x19), data);
}
static __inline GK_U8 ext_system_drc_purple_low_thr_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x19));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_grad_shift
// ------------------------------------------------------------------------------ //
// grad_shift
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_grad_shift_write(VI_PIPE ViPipe,
							GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x1a), data);
}
static __inline GK_U8 ext_system_drc_grad_shift_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x1a));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_grad_slope
// ------------------------------------------------------------------------------ //
// grad_slope
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_grad_slope_write(VI_PIPE ViPipe,
							GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x1b), data);
}
static __inline GK_U8 ext_system_drc_grad_slope_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x1b));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_grad_max
// ------------------------------------------------------------------------------ //
// grad_max
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_grad_max_write(VI_PIPE ViPipe,
						      GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x1c), data);
}
static __inline GK_U8 ext_system_drc_grad_max_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x1c));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_grad_thr
// ------------------------------------------------------------------------------ //
// grad_thr
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_grad_thr_write(VI_PIPE ViPipe,
						      GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x1d), data);
}
static __inline GK_U8 ext_system_drc_grad_thr_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x1d));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_spa_coarse
// ------------------------------------------------------------------------------ //
// spa_coarse
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_var_spa_coarse_write(VI_PIPE ViPipe,
							    GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x1e), data);
}
static __inline GK_U8 ext_system_drc_var_spa_coarse_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x1e));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_spa_median
// ------------------------------------------------------------------------------ //
// spa_median
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_var_spa_medium_write(VI_PIPE ViPipe,
							    GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x1f), data);
}
static __inline GK_U8 ext_system_drc_var_spa_medium_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x1f));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_spa_fine
// ------------------------------------------------------------------------------ //
// spa_fine
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_var_spa_fine_write(VI_PIPE ViPipe,
							  GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x20), data);
}
static __inline GK_U8 ext_system_drc_var_spa_fine_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x20));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_rng_coarse
// ------------------------------------------------------------------------------ //
// rng_coarse
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_var_rng_coarse_write(VI_PIPE ViPipe,
							    GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x21), data);
}
static __inline GK_U8 ext_system_drc_var_rng_coarse_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x21));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_rng_medium
// ------------------------------------------------------------------------------ //
// rng_median
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_var_rng_medium_write(VI_PIPE ViPipe,
							    GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x22), data);
}
static __inline GK_U8 ext_system_drc_var_rng_medium_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x22));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_rng_fine
// ------------------------------------------------------------------------------ //
// rng_fine
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_var_rng_fine_write(VI_PIPE ViPipe,
							  GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x23), data);
}
static __inline GK_U8 ext_system_drc_var_rng_fine_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x23));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_bin_mix_coarse_lut
// ------------------------------------------------------------------------------ //
// bin_mix_coarse_lut
// ------------------------------------------------------------------------------ //
// args: data (8-bit)

static __inline GK_VOID ext_system_drc_bin_mix_coarse_lut_write(VI_PIPE ViPipe,
								GK_U8 u8Index,
								GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x24 + u8Index * sizeof(GK_U8)),
		     data);
}
static __inline GK_U8 ext_system_drc_bin_mix_coarse_lut_read(VI_PIPE ViPipe,
							     GK_U8 u8Index)
{
	return (IORD_8DIRECT(
		(VREG_DRC_BASE(ViPipe) + 0x24 + u8Index * sizeof(GK_U8))));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_bin_mix_medium_lut
// ------------------------------------------------------------------------------ //
// bin_mix_medium_lut
// ------------------------------------------------------------------------------ //

static __inline GK_VOID ext_system_drc_bin_mix_medium_lut_write(VI_PIPE ViPipe,
								GK_U8 u8Index,
								GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x2c + u8Index * sizeof(GK_U8)),
		     data);
}
static __inline GK_U8 ext_system_drc_bin_mix_medium_lut_read(VI_PIPE ViPipe,
							     GK_U8 u8Index)
{
	return (IORD_8DIRECT(
		(VREG_DRC_BASE(ViPipe) + 0x2c + u8Index * sizeof(GK_U8))));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_detail_coring
// ------------------------------------------------------------------------------ //
// drc_detail_coring
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_detail_coring_write(VI_PIPE ViPipe,
							   GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x34), data);
}
static __inline GK_U8 ext_system_drc_detail_coring_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x34));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_detail_dark_min
// ------------------------------------------------------------------------------ //
// detail_dark_min
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_detail_dark_min_write(VI_PIPE ViPipe,
							     GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x35), data);
}
static __inline GK_U8 ext_system_drc_detail_dark_min_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x35));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_detail_dark_max
// ------------------------------------------------------------------------------ //
// detail_dark_max
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_detail_dark_max_write(VI_PIPE ViPipe,
							     GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x36), data);
}
static __inline GK_U8 ext_system_drc_detail_dark_max_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x36));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_detail_dark_slo
// ------------------------------------------------------------------------------ //
// detail_dark_slo
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_detail_dark_slo_write(VI_PIPE ViPipe,
							     GK_S8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x37), data);
}
static __inline GK_S8 ext_system_drc_detail_dark_slo_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x37));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_detail_dark_thr
// ------------------------------------------------------------------------------ //
// detail_dark_thr
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_detail_dark_thr_write(VI_PIPE ViPipe,
							     GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x38), data);
}
static __inline GK_U8 ext_system_drc_detail_dark_thr_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x38));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_detail_bright_min
// ------------------------------------------------------------------------------ //
// detail_bright_min
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_detail_bright_min_write(VI_PIPE ViPipe,
							       GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x39), data);
}
static __inline GK_U8 ext_system_drc_detail_bright_min_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x39));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_detail_bright_max
// ------------------------------------------------------------------------------ //
// detail_bright_max
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_detail_bright_max_write(VI_PIPE ViPipe,
							       GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x3a), data);
}
static __inline GK_U8 ext_system_drc_detail_bright_max_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x3a));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_detail_bright_slo
// ------------------------------------------------------------------------------ //
// detail_bright_slo
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_detail_bright_slo_write(VI_PIPE ViPipe,
							       GK_S8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x3b), data);
}
static __inline GK_S8 ext_system_drc_detail_bright_slo_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x3b));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_detail_bright_thr
// ------------------------------------------------------------------------------ //
// detail_bright_thr
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_detail_bright_thr_write(VI_PIPE ViPipe,
							       GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x3c), data);
}
static __inline GK_U8 ext_system_drc_detail_bright_thr_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x3c));
}

static __inline GK_VOID ext_system_drc_coef_update_en_write(VI_PIPE ViPipe,
							    GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x3d), data);
}
static __inline GK_U8 ext_system_drc_coef_update_en_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x3d));
}

static __inline GK_VOID ext_system_drc_lut_update_en_write(VI_PIPE ViPipe,
							   GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x3e), data);
}
static __inline GK_U8 ext_system_drc_lut_update_en_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x3e));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_manual_strength
// ------------------------------------------------------------------------------ //
// Manual setting for DRC strength (for manual DRC)
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
static __inline GK_VOID ext_system_drc_manual_strength_write(VI_PIPE ViPipe,
							     GK_U16 data)
{
	IOWR_16DIRECT(VREG_DRC_BASE(ViPipe) + 0x40, data);
}
static __inline GK_U16 ext_system_drc_manual_strength_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_DRC_BASE(ViPipe) + 0x40);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_auto_strength
// ------------------------------------------------------------------------------ //
// Auto setting for DRC strength (for auto DRC)
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
static __inline GK_VOID ext_system_drc_auto_strength_write(VI_PIPE ViPipe,
							   GK_U16 data)
{
	IOWR_16DIRECT(VREG_DRC_BASE(ViPipe) + 0x42, data);
}
static __inline GK_U16 ext_system_drc_auto_strength_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_DRC_BASE(ViPipe) + 0x42);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_auto_strength_max
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
static __inline GK_VOID ext_system_drc_auto_strength_max_write(VI_PIPE ViPipe,
							       GK_U16 data)
{
	IOWR_16DIRECT((VREG_DRC_BASE(ViPipe) + 0x44), data);
}

static __inline GK_U16 ext_system_drc_auto_strength_max_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_DRC_BASE(ViPipe) + 0x44));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_auto_strength_min
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
static __inline GK_VOID ext_system_drc_auto_strength_min_write(VI_PIPE ViPipe,
							       GK_U16 data)
{
	IOWR_16DIRECT((VREG_DRC_BASE(ViPipe) + 0x46), data);
}

static __inline GK_U16 ext_system_drc_auto_strength_min_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_DRC_BASE(ViPipe) + 0x46));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_strength_target
// ------------------------------------------------------------------------------ //
// Strength target of Drc Moudle
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
static __inline GK_VOID ext_system_drc_strength_target_write(VI_PIPE ViPipe,
							     GK_U16 data)
{
	IOWR_16DIRECT((VREG_DRC_BASE(ViPipe) + 0x48), data);
}
static __inline GK_U16 ext_system_drc_strength_target_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_DRC_BASE(ViPipe) + 0x48));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_fs_ada_max
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_fs_ada_max_write(VI_PIPE ViPipe,
							GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x4a), data);
}
static __inline GK_U8 ext_system_drc_fs_ada_max_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x4a));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_diff_thr_low
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_diff_thr_low_write(VI_PIPE ViPipe,
							  GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x4b), data);
}
static __inline GK_U8 ext_system_drc_diff_thr_low_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x4b));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_diff_thrgh
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_diff_thrgh_write(VI_PIPE ViPipe,
							GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x4c), data);
}
static __inline GK_U8 ext_system_drc_diff_thrgh_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x4c));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_flt_scale_fine
// ------------------------------------------------------------------------------ //
// DRC filter scale fine control
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_flt_scale_fine_write(VI_PIPE ViPipe,
							    GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x50), data);
}
static __inline GK_U8 ext_system_drc_flt_scale_fine_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x50));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_flt_scale_coarse
// ------------------------------------------------------------------------------ //
// DRC filter scale coarse control
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_flt_scale_coarse_write(VI_PIPE ViPipe,
							      GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x51), data);
}
static __inline GK_U8 ext_system_drc_flt_scale_coarse_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x51));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_suppress_bright_min
// ------------------------------------------------------------------------------ //
// DRC detail suppression control
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_suppress_bright_min_write(VI_PIPE ViPipe,
								 GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x52), data);
}
static __inline GK_U8 ext_system_drc_suppress_bright_min_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x52));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_suppress_bright_thr
// ------------------------------------------------------------------------------ //
// DRC detail suppression control
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_suppress_bright_thr_write(VI_PIPE ViPipe,
								 GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x53), data);
}
static __inline GK_U8 ext_system_drc_suppress_bright_thr_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x53));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_suppress_bright_slo
// ------------------------------------------------------------------------------ //
// DRC detail suppression control
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_suppress_bright_slo_write(VI_PIPE ViPipe,
								 GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x54), data);
}
static __inline GK_U8 ext_system_drc_suppress_bright_slo_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x54));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_suppress_dark_min
// ------------------------------------------------------------------------------ //
// DRC detail suppression control
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_suppress_dark_min_write(VI_PIPE ViPipe,
							       GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x55), data);
}
static __inline GK_U8 ext_system_drc_suppress_dark_min_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x55));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_suppress_dark_thr
// ------------------------------------------------------------------------------ //
// DRC detail suppression control
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_suppress_dark_thr_write(VI_PIPE ViPipe,
							       GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x56), data);
}
static __inline GK_U8 ext_system_drc_suppress_dark_thr_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x56));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_suppress_dark_slo
// ------------------------------------------------------------------------------ //
// DRC detail suppression control
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_suppress_dark_slo_write(VI_PIPE ViPipe,
							       GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x57), data);
}
static __inline GK_U8 ext_system_drc_suppress_dark_slo_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x57));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_dp_detect_rb2rb
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_dp_detect_rb2rb_write(VI_PIPE ViPipe,
							     GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x58), data);
}
static __inline GK_U8 ext_system_drc_dp_detect_rb2rb_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x58));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_dp_detect_g2rb
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_dp_detect_g2rb_write(VI_PIPE ViPipe,
							    GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x59), data);
}
static __inline GK_U8 ext_system_drc_dp_detect_g2rb_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x59));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_dp_detect_repl_ctr
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_dp_detect_repl_ctr_write(VI_PIPE ViPipe,
								GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x5a), data);
}
static __inline GK_U8 ext_system_drc_dp_detect_repl_ctr_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x5a));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_dp_detect_rng_ratio
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_dp_detect_rng_ratio_write(VI_PIPE ViPipe,
								 GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x5b), data);
}
static __inline GK_U8 ext_system_drc_dp_detect_rng_ratio_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x5b));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_dp_detect_thr_slo
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_dp_detect_thr_slo_write(VI_PIPE ViPipe,
							       GK_U8 data)
{
	IOWR_16DIRECT((VREG_DRC_BASE(ViPipe) + 0x5c), data);
}
static __inline GK_U8 ext_system_drc_dp_detect_thr_slo_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x5c));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_dp_detect_thr_min
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
static __inline GK_VOID ext_system_drc_dp_detect_thr_min_write(VI_PIPE ViPipe,
							       GK_U16 data)
{
	IOWR_16DIRECT((VREG_DRC_BASE(ViPipe) + 0x5e), data);
}
static __inline GK_U16 ext_system_drc_dp_detect_thr_min_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_DRC_BASE(ViPipe) + 0x5e));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_bright_gain_lmt
// ------------------------------------------------------------------------------ //
// Bright gain limit
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
static __inline GK_VOID ext_system_drc_bright_gain_lmt_write(VI_PIPE ViPipe,
							     GK_U16 data)
{
	IOWR_16DIRECT((VREG_DRC_BASE(ViPipe) + 0x7e), data);
}
static __inline GK_U16 ext_system_drc_bright_gain_lmt_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_DRC_BASE(ViPipe) + 0x7e));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_dark_gain_lmt_c
// ------------------------------------------------------------------------------ //
// Bright gain limit
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_dark_gain_lmt_c_write(VI_PIPE ViPipe,
							     GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x80), data);
}
static __inline GK_U8 ext_system_drc_dark_gain_lmt_c_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x80));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_dark_gain_lmt_c
// ------------------------------------------------------------------------------ //
// Bright gain limit
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_dark_gain_lmt_y_write(VI_PIPE ViPipe,
							     GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x82), data);
}
static __inline GK_U8 ext_system_drc_dark_gain_lmt_y_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x82));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_colorcc_lut_read
// ------------------------------------------------------------------------------ //
// colorcc lut read len = 33  ~0x11061
// ------------------------------------------------------------------------------ //
// args: data (16-bit)

static __inline GK_VOID
ext_system_drc_colorcc_lut_write(VI_PIPE ViPipe, GK_U8 u8Index, GK_U16 data)
{
	IOWR_16DIRECT((VREG_DRC_BASE(ViPipe) + 0x84 + u8Index * sizeof(GK_U16)),
		      data);
}
static __inline GK_U16 ext_system_drc_colorcc_lut_read(VI_PIPE ViPipe,
						       GK_U8 u8Index)
{
	return (IORD_16DIRECT(
		(VREG_DRC_BASE(ViPipe) + 0x84 + u8Index * sizeof(GK_U16))));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_cubic_ltmx0
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
static __inline GK_VOID ext_system_drc_cubic_ltmx0_write(VI_PIPE ViPipe,
							 GK_U16 data)
{
	IOWR_16DIRECT((VREG_DRC_BASE(ViPipe) + 0xce), data);
}
static __inline GK_U16 ext_system_drc_cubic_ltmx0_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_DRC_BASE(ViPipe) + 0xce));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_cubic_ltmx1
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
static __inline GK_VOID ext_system_drc_cubic_ltmx1_write(VI_PIPE ViPipe,
							 GK_U16 data)
{
	IOWR_16DIRECT((VREG_DRC_BASE(ViPipe) + 0xd0), data);
}
static __inline GK_U16 ext_system_drc_cubic_ltmx1_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_DRC_BASE(ViPipe) + 0xd0));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_cubic_ltmx2
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
static __inline GK_VOID ext_system_drc_cubic_ltmx2_write(VI_PIPE ViPipe,
							 GK_U16 data)
{
	IOWR_16DIRECT((VREG_DRC_BASE(ViPipe) + 0xd2), data);
}
static __inline GK_U16 ext_system_drc_cubic_ltmx2_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_DRC_BASE(ViPipe) + 0xd2));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_cubic_ltmx3
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
static __inline GK_VOID ext_system_drc_cubic_ltmx3_write(VI_PIPE ViPipe,
							 GK_U16 data)
{
	IOWR_16DIRECT((VREG_DRC_BASE(ViPipe) + 0xd4), data);
}
static __inline GK_U16 ext_system_drc_cubic_ltmx3_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_DRC_BASE(ViPipe) + 0xd4));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_cubic_ltmx4
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
static __inline GK_VOID ext_system_drc_cubic_ltmx4_write(VI_PIPE ViPipe,
							 GK_U16 data)
{
	IOWR_16DIRECT((VREG_DRC_BASE(ViPipe) + 0xd6), data);
}
static __inline GK_U16 ext_system_drc_cubic_ltmx4_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_DRC_BASE(ViPipe) + 0xd6));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_cubic_ltmx5
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
static __inline GK_VOID ext_system_drc_cubic_ltmx5_write(VI_PIPE ViPipe,
							 GK_U16 data)
{
	IOWR_16DIRECT((VREG_DRC_BASE(ViPipe) + 0xd8), data);
}
static __inline GK_U16 ext_system_drc_cubic_ltmx5_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_DRC_BASE(ViPipe) + 0xd8));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_cubic_ltmy0
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
static __inline GK_VOID ext_system_drc_cubic_ltmy0_write(VI_PIPE ViPipe,
							 GK_U16 data)
{
	IOWR_16DIRECT((VREG_DRC_BASE(ViPipe) + 0xda), data);
}
static __inline GK_U16 ext_system_drc_cubic_ltmy0_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_DRC_BASE(ViPipe) + 0xda));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_cubic_ltmy1
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
static __inline GK_VOID ext_system_drc_cubic_ltmy1_write(VI_PIPE ViPipe,
							 GK_U16 data)
{
	IOWR_16DIRECT((VREG_DRC_BASE(ViPipe) + 0xdc), data);
}
static __inline GK_U16 ext_system_drc_cubic_ltmy1_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_DRC_BASE(ViPipe) + 0xdc));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_cubic_ltmy2
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
static __inline GK_VOID ext_system_drc_cubic_ltmy2_write(VI_PIPE ViPipe,
							 GK_U16 data)
{
	IOWR_16DIRECT((VREG_DRC_BASE(ViPipe) + 0xde), data);
}
static __inline GK_U16 ext_system_drc_cubic_ltmy2_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_DRC_BASE(ViPipe) + 0xde));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_cubic_ltmy3
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
static __inline GK_VOID ext_system_drc_cubic_ltmy3_write(VI_PIPE ViPipe,
							 GK_U16 data)
{
	IOWR_16DIRECT((VREG_DRC_BASE(ViPipe) + 0xe0), data);
}
static __inline GK_U16 ext_system_drc_cubic_ltmy3_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_DRC_BASE(ViPipe) + 0xe0));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_cubic_ltmy4
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
static __inline GK_VOID ext_system_drc_cubic_ltmy4_write(VI_PIPE ViPipe,
							 GK_U16 data)
{
	IOWR_16DIRECT((VREG_DRC_BASE(ViPipe) + 0xe2), data);
}
static __inline GK_U16 ext_system_drc_cubic_ltmy4_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_DRC_BASE(ViPipe) + 0xe2));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_cubic_ltmy5
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
static __inline GK_VOID ext_system_drc_cubic_ltmy5_write(VI_PIPE ViPipe,
							 GK_U16 data)
{
	IOWR_16DIRECT((VREG_DRC_BASE(ViPipe) + 0xe4), data);
}
static __inline GK_U16 ext_system_drc_cubic_ltmy5_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_DRC_BASE(ViPipe) + 0xe4));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_cubic_mode
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_cubic_mode_write(VI_PIPE ViPipe,
							GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0xe6), data);
}
static __inline GK_U8 ext_system_drc_cubic_mode_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0xe6));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_curve_sel
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_curve_sel_write(VI_PIPE ViPipe,
						       GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0xe7), data);
}
static __inline GK_U8 ext_system_drc_curve_sel_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0xe7));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_cubic_slp0
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
static __inline GK_VOID ext_system_drc_cubic_slp0_write(VI_PIPE ViPipe,
							GK_U16 data)
{
	IOWR_16DIRECT((VREG_DRC_BASE(ViPipe) + 0xe8), data);
}
static __inline GK_U16 ext_system_drc_cubic_slp0_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_DRC_BASE(ViPipe) + 0xe8));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_cubic_slp1
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
static __inline GK_VOID ext_system_drc_cubic_slp1_write(VI_PIPE ViPipe,
							GK_U16 data)
{
	IOWR_16DIRECT((VREG_DRC_BASE(ViPipe) + 0xea), data);
}
static __inline GK_U16 ext_system_drc_cubic_slp1_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_DRC_BASE(ViPipe) + 0xea));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_cubic_slp2
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
static __inline GK_VOID ext_system_drc_cubic_slp2_write(VI_PIPE ViPipe,
							GK_U16 data)
{
	IOWR_16DIRECT((VREG_DRC_BASE(ViPipe) + 0xec), data);
}
static __inline GK_U16 ext_system_drc_cubic_slp2_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_DRC_BASE(ViPipe) + 0xec));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_cubic_slp3
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
static __inline GK_VOID ext_system_drc_cubic_slp3_write(VI_PIPE ViPipe,
							GK_U16 data)
{
	IOWR_16DIRECT((VREG_DRC_BASE(ViPipe) + 0xee), data);
}
static __inline GK_U16 ext_system_drc_cubic_slp3_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_DRC_BASE(ViPipe) + 0xee));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_cubic_slp4
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
static __inline GK_VOID ext_system_drc_cubic_slp4_write(VI_PIPE ViPipe,
							GK_U16 data)
{
	IOWR_16DIRECT((VREG_DRC_BASE(ViPipe) + 0xf0), data);
}
static __inline GK_U16 ext_system_drc_cubic_slp4_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_DRC_BASE(ViPipe) + 0xf0));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_darkstep
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_detail_darkstep_write(VI_PIPE ViPipe,
							     GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0xf2), data);
}
static __inline GK_U8 ext_system_drc_detail_darkstep_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0xf2));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_brightstep
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_detail_brightstep_write(VI_PIPE ViPipe,
							       GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0xf3), data);
}
static __inline GK_U8 ext_system_drc_detail_brightstep_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0xf3));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_shp_log
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_shp_log_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0xf4), data);
}
static __inline GK_U8 ext_system_drc_shp_log_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0xf4));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_shp_exp
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_shp_exp_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0xf5), data);
}
static __inline GK_U8 ext_system_drc_shp_exp_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0xf5));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_flt_spa_fine
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_flt_spa_fine_write(VI_PIPE ViPipe,
							  GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0xf6), data);
}
static __inline GK_U8 ext_system_drc_flt_spa_fine_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0xf6));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_flt_spa_medium
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_flt_spa_medium_write(VI_PIPE ViPipe,
							    GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0xf7), data);
}
static __inline GK_U8 ext_system_drc_flt_spa_medium_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0xf7));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_flt_spa_coarse
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_flt_spa_coarse_write(VI_PIPE ViPipe,
							    GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0xf8), data);
}
static __inline GK_U8 ext_system_drc_flt_spa_coarse_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0xf8));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_flt_rng_fine
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_flt_rng_fine_write(VI_PIPE ViPipe,
							  GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0xf9), data);
}
static __inline GK_U8 ext_system_drc_flt_rng_fine_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0xf9));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_flt_rng_medium
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_flt_rng_medium_write(VI_PIPE ViPipe,
							    GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0xfa), data);
}
static __inline GK_U8 ext_system_drc_flt_rng_medium_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0xfa));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_flt_rng_coarse
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_flt_rng_coarse_write(VI_PIPE ViPipe,
							    GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0xfb), data);
}
static __inline GK_U8 ext_system_drc_flt_rng_coarse_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0xfb));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_flt_rng_ada_max
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_flt_rng_ada_max_write(VI_PIPE ViPipe,
							     GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0xfc), data);
}
static __inline GK_U8 ext_system_drc_flt_rng_ada_max_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0xfc));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_dis_offset_coef
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_dis_offset_coef_write(VI_PIPE ViPipe,
							     GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0xfd), data);
}
static __inline GK_U8 ext_system_drc_dis_offset_coef_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0xfd));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_dis_thr_coef_low
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_dis_thr_coef_low_write(VI_PIPE ViPipe,
							      GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0xfe), data);
}
static __inline GK_U8 ext_system_drc_dis_thr_coef_low_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0xfe));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_dis_thr_coefgh
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_dis_thr_coefgh_write(VI_PIPE ViPipe,
							    GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0xff), data);
}
static __inline GK_U8 ext_system_drc_dis_thr_coefgh_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0xff));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_detail_sub_factor
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_detail_sub_factor_write(VI_PIPE ViPipe,
							       GK_S8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x100), data);
}
static __inline GK_S8 ext_system_drc_detail_sub_factor_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x100));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_wgt_box_tri_sel
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_wgt_box_tri_sel_write(VI_PIPE ViPipe,
							     GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x101), data);
}
static __inline GK_U8 ext_system_drc_wgt_box_tri_sel_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x101));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_color_corr_enable
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_color_corr_enable_write(VI_PIPE ViPipe,
							       GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x102), data);
}
static __inline GK_U8 ext_system_drc_color_corr_enable_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x102));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_detail_boost_enable
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_detail_boost_enable_write(VI_PIPE ViPipe,
								 GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x103), data);
}
static __inline GK_U8 ext_system_drc_detail_boost_enable_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x103));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_pdw_sum_enable
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_drc_pdw_sum_enable_write(VI_PIPE ViPipe,
							    GK_U8 data)
{
	IOWR_8DIRECT((VREG_DRC_BASE(ViPipe) + 0x104), data);
}
static __inline GK_U8 ext_system_drc_pdw_sum_enable_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DRC_BASE(ViPipe) + 0x104));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_drc_tm_lut_write len = 200
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
// 0x107 ~ 0x296
static __inline GK_VOID ext_system_drc_tm_lut_write(VI_PIPE ViPipe,
						    GK_U8 u8Index, GK_U16 data)
{
	IOWR_16DIRECT(
		(VREG_DRC_BASE(ViPipe) + 0x106 + u8Index * sizeof(GK_U16)),
		data);
}

static __inline GK_U16 ext_system_drc_tm_lut_read(VI_PIPE ViPipe, GK_U8 u8Index)
{
	return IORD_16DIRECT(
		(VREG_DRC_BASE(ViPipe) + 0x106 + u8Index * sizeof(GK_U16)));
}

// ==============================================================//
// Dehaze
// ==============================================================//
// ------------------------------------------------------------------------------ //
// Register: ext_system_dehaze_manu_mode
// ------------------------------------------------------------------------------ //
// Enables manual dehaze: 0=auto 1=manual
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_DEHAZE_MANU_MODE_DEFAULT (0)
#define ISP_EXT_SYSTEM_DEHAZE_MANU_MODE_DATASIZE (1)
// args: data (8-bit)
static __inline void ext_system_dehaze_manu_mode_write(VI_PIPE ViPipe,
						       GK_U8 data)
{
	IOWR_8DIRECT(VREG_DEHAZE_BASE(ViPipe), data);
}
static __inline GK_U32 ext_system_dehaze_manu_mode_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_DEHAZE_BASE(ViPipe));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_manual_dehaze_hblk
// ------------------------------------------------------------------------------ //
// manual dehaze hblk number
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_MANUAL_DEHAZE_HBLK_DATASIZE (8)

// args: data (8-bit)
static __inline void ext_system_manual_dehaze_hblk_write(VI_PIPE ViPipe,
							 GK_U8 data)
{
	IOWR_8DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0x1, data);
}
static __inline GK_U8 ext_system_manual_dehaze_hblk_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_manual_dehaze_vblk
// ------------------------------------------------------------------------------ //
// manual dehaze vblk number
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_MANUAL_DEHAZE_VBLK_DATASIZE (8)
// args: data (8-bit)
static __inline void ext_system_manual_dehaze_vblk_write(VI_PIPE ViPipe,
							 GK_U8 data)
{
	IOWR_8DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0x2, data);
}

static __inline GK_U8 ext_system_manual_dehaze_vblk_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0x2);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_dehaze_ctp
// ------------------------------------------------------------------------------ //
// manual dehaze curve turn point
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_DEHAZE_CTP_DEFAULT (0x80)
#define ISP_EXT_SYSTEM_DEHAZE_CTP_DATASIZE (8)

// args: data (8-bit)
static __inline void ext_system_dehaze_ctp_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0x3, data);
}
static __inline GK_U8 ext_system_dehaze_ctp_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0x3);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_manual_dehaze_mft
// ------------------------------------------------------------------------------ //
// manual dehaze minumum filter threshold
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_DEHAZE_MFT_DEFAULT (0x200)
#define ISP_EXT_SYSTEM_MANUAL_DEHAZE_MFT_DATASIZE (16)

// args: data (10-bit)
static __inline void ext_system_dehaze_mft_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0x4, data);
}
static __inline GK_U16 ext_system_dehaze_mft_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0x4) & 0x3ff);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_manual_dehaze_mfs
// ------------------------------------------------------------------------------ //
// manual dehaze minumum filter size
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_DEHAZE_MFS_DEFAULT (0x7)
#define ISP_EXT_SYSTEM_DEHAZE_MFS_DATASIZE (8)
// args: data (8-bit)
static __inline void ext_system_dehaze_mfs_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0x6, data);
}
static __inline GK_U8 ext_system_dehaze_mfs_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0x6);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_manual_dehaze_ct
// ------------------------------------------------------------------------------ //
// manual dehaze change threshold
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_DEHAZE_CT_DEFAULT (255)
#define ISP_EXT_SYSTEM_DEHAZE_CT_DATASIZE (8)
// args: data (8-bit)
static __inline void ext_system_dehaze_ct_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0x7, data);
}
static __inline GK_U8 ext_system_dehaze_ct_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0x7);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_manual_dehaze_tfic
// ------------------------------------------------------------------------------ //
// manual dehaze temper filter increase ceofficient
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_DEHAZE_TFIC_DEFAULT (0x8)
#define ISP_EXT_SYSTEM_DEHAZE_TFIC_DATASIZE (16)
// args: data (16-bit)
static __inline void ext_system_dehaze_tfic_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0x8, data);
}
static __inline GK_U16 ext_system_dehaze_tfic_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0x8);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_manual_dehaze_tfdc
// ------------------------------------------------------------------------------ //
// manual dehaze temper filter decrease ceofficient
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_DEHAZE_TFDC_DEFAULT (64)
#define ISP_EXT_SYSTEM_DEHAZE_TFDC_DATASIZE (16)
// args: data (16-bit)
static __inline void ext_system_dehaze_tfdc_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0xa, data);
}
static __inline GK_U16 ext_system_dehaze_tfdc_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0xa);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_dehaze_debug_enable
// ------------------------------------------------------------------------------ //
// dehaze debug enable
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_DEHAZE_DEBUGFILE_ENABLE_DEFAULT (0)
#define ISP_EXT_SYSTEM_DEHAZE_DEBUGFILE_ENABLE_DATASIZE (8)

// args: data (8-bit)
static __inline void ext_system_dehaze_debugfile_enable_write(VI_PIPE ViPipe,
							      GK_U8 data)
{
	IOWR_8DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0xc, data);
}
static __inline GK_U8 ext_system_dehaze_debugfile_enable_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0xc);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_manual_dehaze_strength
// ------------------------------------------------------------------------------ //
// dehaze strength
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_MANUAL_DEHAZE_STRENGTH_DEFAULT (0x80)
#define ISP_EXT_SYSTEM_MANUAL_DEHAZE_STRENGTH_DATASIZE (8)

// args: data (8-bit)
static __inline void ext_system_manual_dehaze_strength_write(VI_PIPE ViPipe,
							     GK_U8 data)
{
	IOWR_8DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0xd, data);
}
static __inline GK_U8 ext_system_manual_dehaze_strength_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0xd);
}

// ------------------------------------------------------------------------------ //
// Register: isp_ext_system_manual_dehaze_autostrength
// ------------------------------------------------------------------------------ //
// dehaze strength
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_MANUAL_DEHAZE_AUTOSTRENGTH_DEFAULT (0x80)
#define ISP_EXT_SYSTEM_MANUAL_DEHAZE_AUTOSTRENGTH_DATASIZE (8)
// args: data (8-bit)
static __inline void ext_system_manual_dehaze_autostrength_write(VI_PIPE ViPipe,
								 GK_U8 data)
{
	IOWR_8DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0xe, data);
}
static __inline GK_U8 ext_system_manual_dehaze_autostrength_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0xe);
}

#define ISP_EXT_SYSTEM_DEHAZE_ENABLE_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_DEHAZE_ENABLE_DATASIZE (8)

// args: data (8-bit)
static __inline void ext_system_dehaze_enable_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0x10, data);
}
static __inline GK_U8 ext_system_dehaze_enable_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0x10);
}

// args: data (1-bit)

#define ISP_EXT_SYSTEM_DEHAZE_DEBUG_ENABLE_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_DEHAZE_DEBUG_ENABLE_DATASIZE (1)

static __inline GK_VOID ext_system_dehaze_debug_enable_write(VI_PIPE ViPipe,
							     GK_U16 data)
{
	IOWR_16DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0x14, data & 0x1);
}
static __inline GK_U16 ext_system_dehaze_debug_enable_read(VI_PIPE ViPipe)
{
	return (GK_U16)(IORD_16DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0x14) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_dehaze_debug_addr
// ------------------------------------------------------------------------------ //
// address of dehaze debug information
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_DEHAZE_DEBUG_HIGH_ADDR_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_DEHAZE_DEBUG_HIGH_ADDR_DATASIZE (32)

// args: data (32-bit)
static __inline GK_VOID ext_system_dehaze_debuggh_addr_write(VI_PIPE ViPipe,
							     GK_U32 data)
{
	IOWR_32DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0x18, data);
}

static __inline GK_U32 ext_system_dehaze_debuggh_addr_read(VI_PIPE ViPipe)
{
	return IORD_32DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0x18);
}

#define ISP_EXT_SYSTEM_DEHAZE_DEBUG_LOW_ADDR_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_DEHAZE_DEBUG_LOW_ADDR_DATASIZE (32)

// args: data (32-bit)
static __inline GK_VOID ext_system_dehaze_debug_low_addr_write(VI_PIPE ViPipe,
							       GK_U32 data)
{
	IOWR_32DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0x1c, data);
}

static __inline GK_U32 ext_system_dehaze_debug_low_addr_read(VI_PIPE ViPipe)
{
	return IORD_32DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0x1c);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_dehaze_debug_size
// ------------------------------------------------------------------------------ //
// size of dehaze debug struct
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_DEHAZE_DEBUG_SIZE_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_DEHAZE_DEBUG_SIZE_DATASIZE (32)

// args: data (32-bit)
static __inline GK_VOID ext_system_dehaze_debug_size_write(VI_PIPE ViPipe,
							   GK_U32 data)
{
	IOWR_32DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0x20, data);
}

static __inline GK_U32 ext_system_dehaze_debug_size_read(VI_PIPE ViPipe)
{
	return IORD_32DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0x20);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_dehaze_debug_depth
// ------------------------------------------------------------------------------ //
// depth of dehaze debug struct
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_DEHAZE_DEBUG_DEPTH_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_DEHAZE_DEBUG_DEPTH_DATADEPTH (32)

// args: data (32-bit)
static __inline GK_VOID ext_system_dehaze_debug_depth_write(VI_PIPE ViPipe,
							    GK_U32 data)
{
	IOWR_32DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0x24, data);
}

static __inline GK_U32 ext_system_dehaze_debug_depth_read(VI_PIPE ViPipe)
{
	return IORD_32DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0x24);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_dehaze_debug_quit
// ------------------------------------------------------------------------------ //
// depth of dehaze debug struct
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_DEHAZE_DEBUG_QUIT_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_DEHAZE_DEBUG_QUIT_DATADEPTH (8)

// args: data (8-bit)
static __inline GK_VOID ext_system_dehaze_debug_quit_write(VI_PIPE ViPipe,
							   GK_U8 data)
{
	IOWR_8DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0x28, data);
}

static __inline GK_U8 ext_system_dehaze_debug_quit_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0x28);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_dehaze_debug_complete
// ------------------------------------------------------------------------------ //
// depth of dehaze debug struct
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_DEHAZE_DEBUG_COMPLETE_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_DEHAZE_DEBUG_COMPLETE_DATADEPTH (8)

// args: data (8-bit)
static __inline GK_VOID ext_system_dehaze_debug_complete_write(VI_PIPE ViPipe,
							       GK_U8 data)
{
	IOWR_8DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0x29, data);
}

static __inline GK_U8 ext_system_dehaze_debug_complete_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0x29);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_defog_lut_write len = 256
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_dehaze_lut_write(VI_PIPE ViPipe,
						    GK_U8 u8Index, GK_U8 data)
{
	IOWR_8DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0x30 +
			     (u8Index * sizeof(GK_U8)),
		     data);
}

static __inline GK_U8 ext_system_dehaze_lut_read(VI_PIPE ViPipe, GK_U8 u8Index)
{
	return IORD_8DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0x30 +
			    (u8Index * sizeof(GK_U8)));
}

#define ISP_EXT_SYSTEM_USER_DEHAZE_LUT_ENABLE_DEFAULT (0)
static __inline GK_VOID ext_system_user_dehaze_lut_enable_write(VI_PIPE ViPipe,
								GK_U8 data)
{
	IOWR_8DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0x140, data);
}
static __inline GK_U8 ext_system_user_dehaze_lut_enable_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0x140);
}

#define ISP_EXT_SYSTEM_USER_DEHAZE_LUT_UPDATE_DEFAULT (0)
static __inline GK_VOID ext_system_user_dehaze_lut_update_write(VI_PIPE ViPipe,
								GK_U8 data)
{
	IOWR_8DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0x142, data);
}
static __inline GK_U8 ext_system_user_dehaze_lut_update_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_DEHAZE_BASE(ViPipe) + 0x142);
}

// ==============================================================//
// Dehaze
// ==============================================================//
// ------------------------------------------------------------------------------ //
// Register: ext_system_manual_fpn_strength
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_FPN_STRENGTH_DEFAULT (0)
#define ISP_EXT_SYSTEM_FPN_STRENGTH_DATASIZE (16)
// args: data (16-bit)
// ------------------------------------------------------------------------------ //
// Register: ext_system_manual_fpn_opmode
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_FPN_OPMODE_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_FPN_OPMODE_DATASIZE (8)
// args: data (8-bit)
static __inline void ext_system_manual_fpn_opmode_write(VI_PIPE ViPipe,
							GK_U8 data)
{
	IOWR_8DIRECT(VREG_FPN_BASE(ViPipe) + 0x2, data);
}

static __inline GK_U8 ext_system_manual_fpn_opmode_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_FPN_BASE(ViPipe) + 0x2);
}

// ------------------------------------------------------------------------------ //
// Register: isp_ext_system_manual_fpn_update
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_FPN_MANU_UPDATE_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_FPN_MANU_UPDATE_DATASIZE (8)
// args: data (8-bit)
static __inline void ext_system_manual_fpn_update_write(VI_PIPE ViPipe,
							GK_U8 data)
{
	IOWR_8DIRECT(VREG_FPN_BASE(ViPipe) + 0x3, data);
}

static __inline GK_U8 ext_system_manual_fpn_update_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_FPN_BASE(ViPipe) + 0x3);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_manual_fpn_CorrCfg
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_FPN_MANU_ISO_DEFAULT (6375)
#define ISP_EXT_SYSTEM_FPN_MANU_ISO_DATASIZE (32)
// args: data (32-bit)
static __inline void ext_system_manual_fpn_iso_write(VI_PIPE ViPipe,
						     GK_U32 data)
{
	IOWR_32DIRECT(VREG_FPN_BASE(ViPipe) + 0x4, data);
}

static __inline GK_U32 ext_system_manual_fpn_iso_read(VI_PIPE ViPipe)
{
	return IORD_32DIRECT(VREG_FPN_BASE(ViPipe) + 0x4);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_sensor_iso
// ------------------------------------------------------------------------------ //
// Sensor ISO
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_FPN_SENSOR_ISO_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_FPN_SENSOR_ISO_DATASIZE (32)

// args: data (32-bit)
static __inline GK_VOID ext_system_fpn_sensor_iso_write(VI_PIPE ViPipe,
							GK_U32 data)
{
	IOWR_32DIRECT(VREG_FPN_BASE(ViPipe) + 0x8, data);
}

static __inline GK_U32 ext_system_fpn_sensor_iso_read(VI_PIPE ViPipe)
{
	return IORD_32DIRECT(VREG_FPN_BASE(ViPipe) + 0x8);
}

// ------------------------------------------------------------------------------ //
// Register: isp_ext_system_manual_fpn_CorrCfg
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_FPN_MANU_CORRCFG_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_FPN_MANU_CORRCFG_DATASIZE (8)
// args: data (8-bit)
static __inline void ext_system_manual_fpn_CorrCfg_write(VI_PIPE ViPipe,
							 GK_U8 data)
{
	IOWR_8DIRECT(VREG_FPN_BASE(ViPipe) + 0xc, data);
}

static __inline GK_U8 isp_ext_system_manual_fpn_CorrCfg_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_FPN_BASE(ViPipe) + 0xc);
}

static __inline void ext_system_manual_fpn_Type_write(VI_PIPE ViPipe,
						      GK_U8 data)
{
	IOWR_8DIRECT(VREG_FPN_BASE(ViPipe) + 0xd, data);
}

static __inline GK_U8 isp_ext_system_manual_fpn_Type_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_FPN_BASE(ViPipe) + 0xd);
}

static __inline void ext_system_manual_fpn_Enable_write(VI_PIPE ViPipe,
							GK_U8 data)
{
	IOWR_8DIRECT(VREG_FPN_BASE(ViPipe) + 0xe, data);
}

static __inline GK_U8 isp_ext_system_manual_fpn_Enable_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_FPN_BASE(ViPipe) + 0xe);
}

static __inline void ext_system_manual_fpn_Pixelformat_write(VI_PIPE ViPipe,
							     GK_U8 data)
{
	IOWR_8DIRECT(VREG_FPN_BASE(ViPipe) + 0xf, data);
}

static __inline GK_U8 isp_ext_system_manual_fpn_Pixelformat_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_FPN_BASE(ViPipe) + 0xf);
}

static __inline void ext_system_manual_fpn_Gain_write(VI_PIPE ViPipe,
						      GK_U32 data)
{
	IOWR_32DIRECT(VREG_FPN_BASE(ViPipe) + 0x10, data);
}

static __inline GK_U32 isp_ext_system_manual_fpn_Gain_read(VI_PIPE ViPipe)
{
	return IORD_32DIRECT(VREG_FPN_BASE(ViPipe) + 0x10);
}

static __inline void ext_system_manual_fpn_Offset_write(VI_PIPE ViPipe,
							GK_U32 data)
{
	IOWR_32DIRECT(VREG_FPN_BASE(ViPipe) + 0x14, data);
}

static __inline GK_U32 isp_ext_system_manual_fpn_Offset_read(VI_PIPE ViPipe)
{
	return IORD_32DIRECT(VREG_FPN_BASE(ViPipe) + 0x14);
}

#define ISP_EXT_SYSTEM_FPN_SENSOR_ISO_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_FPN_SENSOR_ISO_DATASIZE (32)

// args: data (8-bit)
static __inline GK_VOID ext_system_fpn_cablibrate_enable_write(VI_PIPE ViPipe,
							       GK_U8 data)
{
	IOWR_8DIRECT(VREG_FPN_BASE(ViPipe) + 0x19, data);
}

static __inline GK_U8 ext_system_fpn_cablibrate_enable_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_FPN_BASE(ViPipe) + 0x19);
}

// =============================================================//
// BLACK LEVEL
// =============================================================//
// Register: ext_system_black_level_00
// ------------------------------------------------------------------------------ //
// ISP black level 00
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_BLACK_LEVEL_00_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_BLACK_LEVEL_00_DATASIZE (16)
// args: data (16-bit)
static __inline GK_VOID ext_system_black_level_00_write(VI_PIPE ViPipe,
							GK_U16 data)
{
	IOWR_16DIRECT(VREG_BLC_BASE(ViPipe), data);
}

static __inline GK_U16 ext_system_black_level_00_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_BLC_BASE(ViPipe));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_black_level_01
// ------------------------------------------------------------------------------ //
// ISP black level 01
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_BLACK_LEVEL_01_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_BLACK_LEVEL_01_DATASIZE (16)
// args: data (16-bit)
static __inline GK_VOID ext_system_black_level_01_write(VI_PIPE ViPipe,
							GK_U16 data)
{
	IOWR_16DIRECT(VREG_BLC_BASE(ViPipe) + 0x2, data);
}

static __inline GK_U16 ext_system_black_level_01_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_BLC_BASE(ViPipe) + 0x2);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_black_level_10
// ------------------------------------------------------------------------------ //
// ISP black level 10
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_BLACK_LEVEL_10_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_BLACK_LEVEL_10_DATASIZE (16)
// args: data (16-bit)
static __inline GK_VOID ext_system_black_level_10_write(VI_PIPE ViPipe,
							GK_U16 data)
{
	IOWR_16DIRECT(VREG_BLC_BASE(ViPipe) + 0x4, data);
}

static __inline GK_U16 ext_system_black_level_10_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_BLC_BASE(ViPipe) + 0x4);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_black_level_11
// ------------------------------------------------------------------------------ //
// ISP black level 11
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_BLACK_LEVEL_11_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_BLACK_LEVEL_11_DATASIZE (16)
// args: data (16-bit)
static __inline GK_VOID ext_system_black_level_11_write(VI_PIPE ViPipe,
							GK_U16 data)
{
	IOWR_16DIRECT(VREG_BLC_BASE(ViPipe) + 0x6, data);
}

static __inline GK_U16 ext_system_black_level_11_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_BLC_BASE(ViPipe) + 0x6);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_black_level_change
// ------------------------------------------------------------------------------ //
// ISP black level change
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_BLACK_LEVEL_CHANGE_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_BLACK_LEVEL_CHANGE_DATASIZE (1)
// args: data (1-bit)
static __inline GK_VOID ext_system_black_level_change_write(VI_PIPE ViPipe,
							    GK_U8 data)
{
	IOWR_8DIRECT(VREG_BLC_BASE(ViPipe) + 0x8, data);
}

static __inline GK_U8 ext_system_black_level_change_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_BLC_BASE(ViPipe) + 0x8);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_black_level_manual_mode
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
#define ISP_EXT_SYSTEM_BLACK_LEVEL_MANUAL_MODE_DEFAULT (0)
static __inline GK_VOID ext_system_black_level_manual_mode_write(VI_PIPE ViPipe,
								 GK_U8 data)
{
	IOWR_8DIRECT(VREG_BLC_BASE(ViPipe) + 0xa, data & 0x1);
}
static __inline GK_U8 ext_system_black_level_manual_mode_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_BLC_BASE(ViPipe) + 0xa) & 0x1);
}

// ==============================================================//
// AE
// ==============================================================//
// ------------------------------------------------------------------------------ //
// Register: ext_system_ae_sel
// ------------------------------------------------------------------------------ //
// AE Sel
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_AE_BE_SEL_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_AE_BE_SEL_DATASIZE (8)

// args: data (8-bit)
static __inline GK_VOID ext_system_ae_be_sel_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT((VREG_AE_BASE(ViPipe)), data);
}
static __inline GK_U8 ext_system_ae_be_sel_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_AE_BASE(ViPipe));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_aest_offset_x
// ------------------------------------------------------------------------------ //
// AE hist offset X
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_AE_HIST_OFFSET_X_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_AE_HIST_OFFSET_X_DATASIZE (8)

// args: data (8-bit)
static __inline GK_VOID ext_system_aest_offset_x_write(VI_PIPE ViPipe,
						       GK_U8 data)
{
	IOWR_8DIRECT((VREG_AE_BASE(ViPipe) + 0x1), data);
}
static __inline GK_U8 ext_system_aest_offset_x_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_AE_BASE(ViPipe) + 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_aest_offset_y
// ------------------------------------------------------------------------------ //
// AE hist offset Y
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_AE_HIST_OFFSET_Y_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_AE_HIST_OFFSET_Y_DATASIZE (8)

// args: data (8-bit)
static __inline GK_VOID ext_system_aest_offset_y_write(VI_PIPE ViPipe,
						       GK_U8 data)
{
	IOWR_8DIRECT((VREG_AE_BASE(ViPipe) + 0x2), data);
}
static __inline GK_U8 ext_system_aest_offset_y_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_AE_BASE(ViPipe) + 0x2);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_aest_skip_x
// ------------------------------------------------------------------------------ //
// AE hist skip X
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_AE_HIST_SKIP_X_DEFAULT (0x1)
#define ISP_EXT_SYSTEM_AE_HIST_SKIP_X_DATASIZE (8)

// args: data (8-bit)
static __inline GK_VOID ext_system_aest_skip_x_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT((VREG_AE_BASE(ViPipe) + 0x3), data);
}
static __inline GK_U8 ext_system_aest_skip_x_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_AE_BASE(ViPipe) + 0x3);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_aest_skip_y
// ------------------------------------------------------------------------------ //
// AE hist skip Y
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_AE_HIST_SKIP_Y_DEFAULT (0x1)
#define ISP_EXT_SYSTEM_AE_HIST_SKIP_Y_DATASIZE (8)

// args: data (8-bit)
static __inline GK_VOID ext_system_aest_skip_y_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT((VREG_AE_BASE(ViPipe) + 0x4), data);
}
static __inline GK_U8 ext_system_aest_skip_y_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_AE_BASE(ViPipe) + 0x4);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_ae_fourplanemode
// ------------------------------------------------------------------------------ //
// AE Fourplanemode
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_AE_FOURPLANEMODE_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_AE_FOURPLANEMODE_DATASIZE (8)

// args: data (8-bit)
static __inline GK_VOID ext_system_ae_fourplanemode_write(VI_PIPE ViPipe,
							  GK_U8 data)
{
	IOWR_8DIRECT((VREG_AE_BASE(ViPipe) + 0x5), data);
}
static __inline GK_U8 ext_system_ae_fourplanemode_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_AE_BASE(ViPipe) + 0x5);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_aestmode
// ------------------------------------------------------------------------------ //
// AE Histmode
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_AE_HISTMODE_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_AE_HISTMODE_DATASIZE (8)

// args: data (8-bit)
static __inline GK_VOID ext_system_aestmode_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT((VREG_AE_BASE(ViPipe) + 0x6), data);
}
static __inline GK_U8 ext_system_aestmode_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_AE_BASE(ViPipe) + 0x6);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_ae_avermode
// ------------------------------------------------------------------------------ //
// AE Avermode
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_AE_AVERMODE_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_AE_AVERMODE_DATASIZE (8)

// args: data (8-bit)
static __inline GK_VOID ext_system_ae_avermode_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT((VREG_AE_BASE(ViPipe) + 0x7), data);
}
static __inline GK_U8 ext_system_ae_avermode_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_AE_BASE(ViPipe) + 0x7);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_ae_maxgainmode
// ------------------------------------------------------------------------------ //
// AE Maxgainmode
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_AE_MAXGAINMODE_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_AE_MAXGAINMODE_DATASIZE (8)

// args: data (8-bit)
static __inline GK_VOID ext_system_ae_maxgainmode_write(VI_PIPE ViPipe,
							GK_U8 data)
{
	IOWR_8DIRECT((VREG_AE_BASE(ViPipe) + 0x8), data);
}
static __inline GK_U8 ext_system_ae_maxgainmode_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_AE_BASE(ViPipe) + 0x8);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_ae_fe_en
// ------------------------------------------------------------------------------ //
// AE FE Enable
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_AE_FE_EN_DEFAULT (0x1)
#define ISP_EXT_SYSTEM_AE_FE_EN_DATASIZE (8)

// args: data (8-bit)
static __inline GK_VOID ext_system_ae_fe_en_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT((VREG_AE_BASE(ViPipe) + 0xc), data);
}
static __inline GK_U8 ext_system_ae_fe_en_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_AE_BASE(ViPipe) + 0xc);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_ae_be_en
// ------------------------------------------------------------------------------ //
// AE BE Enable
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_AE_BE_EN_DEFAULT (0x1)
#define ISP_EXT_SYSTEM_AE_BE_EN_DATASIZE (8)

// args: data (8-bit)
static __inline GK_VOID ext_system_ae_be_en_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT((VREG_AE_BASE(ViPipe) + 0xd), data);
}
static __inline GK_U8 ext_system_ae_be_en_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_AE_BASE(ViPipe) + 0xd);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_mg_en
// ------------------------------------------------------------------------------ //
// MG Enable
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_MG_EN_DEFAULT (0x1)
#define ISP_EXT_SYSTEM_MG_EN_DATASIZE (8)

// args: data (8-bit)
static __inline GK_VOID ext_system_mg_en_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT((VREG_AE_BASE(ViPipe) + 0xe), data);
}
static __inline GK_U8 ext_system_mg_en_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_AE_BASE(ViPipe) + 0xe);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_crop_en
// ------------------------------------------------------------------------------ //
// CROP Enable
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_CROP_EN_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_CROP_EN_DATASIZE (8)

// args: data (8-bit)
static __inline GK_VOID ext_system_ae_crop_en_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT((VREG_AE_BASE(ViPipe) + 0xf), data);
}
static __inline GK_U8 ext_system_ae_crop_en_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_AE_BASE(ViPipe) + 0xf);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_ae_crop_x
// ------------------------------------------------------------------------------ //
// AE CROP X
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_AE_CROP_X_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_AE_CROP_X_DATASIZE (16)

// args: data (16-bit)
static __inline GK_VOID ext_system_ae_crop_x_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AE_BASE(ViPipe) + 0x10), data);
}
static __inline GK_U32 ext_system_ae_crop_x_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_AE_BASE(ViPipe) + 0x10);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_ae_crop_y
// ------------------------------------------------------------------------------ //
// AE CROP Y
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_AE_CROP_Y_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_AE_CROP_Y_DATASIZE (16)

// args: data (16-bit)
static __inline GK_VOID ext_system_ae_crop_y_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AE_BASE(ViPipe) + 0x14), data);
}
static __inline GK_U16 ext_system_ae_crop_y_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_AE_BASE(ViPipe) + 0x14);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_ae_crop_height
// ------------------------------------------------------------------------------ //
// AE CROP HEIGHT
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_AE_CROP_HEIGHT_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_AE_CROP_HEIGHT_DATASIZE (16)

// args: data (16-bit)
static __inline GK_VOID ext_system_ae_crop_height_write(VI_PIPE ViPipe,
							GK_U16 data)
{
	IOWR_16DIRECT((VREG_AE_BASE(ViPipe) + 0x18), data);
}
static __inline GK_U16 ext_system_ae_crop_height_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_AE_BASE(ViPipe) + 0x18);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_ae_crop_width
// ------------------------------------------------------------------------------ //
// AE CROP WIDTH
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_AE_CROP_WIDTH_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_AE_CROP_WIDTH_DATASIZE (16)

// args: data (16-bit)
static __inline GK_VOID ext_system_ae_crop_width_write(VI_PIPE ViPipe,
						       GK_U16 data)
{
	IOWR_16DIRECT((VREG_AE_BASE(ViPipe) + 0x1c), data);
}
static __inline GK_U16 ext_system_ae_crop_width_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_AE_BASE(ViPipe) + 0x1c);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_smart_enable
// ------------------------------------------------------------------------------ //
// smart enable
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_SMART_ENABLE_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_SMART_ENABLE_DATASIZE (8)

// args: data (8-bit)
static __inline GK_VOID ext_system_smart_enable_write(VI_PIPE ViPipe,
						      GK_U16 index, GK_U8 data)
{
	IOWR_8DIRECT((VREG_AE_BASE(ViPipe) + 0x20 + index), data);
}
static __inline GK_U8 ext_system_smart_enable_read(VI_PIPE ViPipe, GK_U16 index)
{
	return IORD_8DIRECT(VREG_AE_BASE(ViPipe) + 0x20 + index);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_smart_available
// ------------------------------------------------------------------------------ //
// smart available
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_SMART_AVAILABLE_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_SMART_AVAILABLE_DATASIZE (8)

// args: data (8-bit)
static __inline GK_VOID
ext_system_smart_available_write(VI_PIPE ViPipe, GK_U16 index, GK_U8 data)
{
	IOWR_8DIRECT((VREG_AE_BASE(ViPipe) + 0x30 + index), data);
}
static __inline GK_U8 ext_system_smart_available_read(VI_PIPE ViPipe,
						      GK_U16 index)
{
	return IORD_8DIRECT(VREG_AE_BASE(ViPipe) + 0x30 + index);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_smart_luma
// ------------------------------------------------------------------------------ //
// smart luma
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_SMART_LUMA_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_SMART_LUMA_DATASIZE (8)

// args: data (8-bit)
static __inline GK_VOID ext_system_smart_luma_write(VI_PIPE ViPipe,
						    GK_U16 index, GK_U8 data)
{
	IOWR_8DIRECT((VREG_AE_BASE(ViPipe) + 0x40 + index), data);
}
static __inline GK_U8 ext_system_smart_luma_read(VI_PIPE ViPipe, GK_U16 index)
{
	return IORD_8DIRECT(VREG_AE_BASE(ViPipe) + 0x40 + index);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_smart_update
// ------------------------------------------------------------------------------ //
// smart update
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_SMART_UPDATE_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_SMART_UPDATE_DATASIZE (8)

// args: data (8-bit)
static __inline GK_VOID ext_system_smart_update_write(VI_PIPE ViPipe,
						      GK_U8 data)
{
	IOWR_8DIRECT((VREG_AE_BASE(ViPipe) + 0x50), data);
}
static __inline GK_U8 ext_system_smart_update_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_AE_BASE(ViPipe) + 0x50);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_ae_weight_table
// ------------------------------------------------------------------------------ //
// AE weight table
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_AE_WEIGHT_TABLE_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_AE_WEIGHT_TABLE_DATASIZE (8)

// args: data (8-bit)
static __inline GK_VOID
ext_system_ae_weight_table_write(VI_PIPE ViPipe, GK_U16 index, GK_U8 data)
{
	IOWR_8DIRECT((VREG_AE_BASE(ViPipe) + 0x100 + index), data);
}
static __inline GK_U8 ext_system_ae_weight_table_read(VI_PIPE ViPipe,
						      GK_U16 index)
{
	return IORD_8DIRECT(VREG_AE_BASE(ViPipe) + 0x100 + index);
}

// ==============================================================//
// DPC
// ==============================================================//
// ------------------------------------------------------------------------------ //
// Register: ext_system_dpc_static_cor_enable
// ------------------------------------------------------------------------------ //
// defect pixel static correction enable
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_DPC_STATIC_COR_ENABLE_DEFAULT (0x01)
#define ISP_EXT_SYSTEM_DPC_STATIC_COR_ENABLE_DATASIZE (1)

//  dpc_static_cor_enable
static __inline GK_VOID ext_system_dpc_static_cor_enable_write(VI_PIPE ViPipe,
							       GK_U8 data)
{
	IOWR_8DIRECT(VREG_DPC_BASE(ViPipe), data & 0x1);
}
static __inline GK_U8 ext_system_dpc_static_cor_enable_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DPC_BASE(ViPipe)) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_dpc_static_calib_enable
// ------------------------------------------------------------------------------ //
// defect pixel static correction enable
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_DPC_STATIC_CALIB_ENABLE_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_DPC_STATIC_CALIB_ENABLE_DATASIZE (1)

//  dpc_static_calib_enable
static __inline GK_VOID ext_system_dpc_static_calib_enable_write(VI_PIPE ViPipe,
								 GK_U8 data)
{
	IOWR_8DIRECT(VREG_DPC_BASE(ViPipe) + 1, data & 0x1);
}
static __inline GK_U8 ext_system_dpc_static_calib_enable_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DPC_BASE(ViPipe) + 1) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_dpc_defect_type
// ------------------------------------------------------------------------------ //
// ISP_STATIC_DP_BRIGHT:0x0,ISP_STATIC_DP_DARK:0x1
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_DPC_STATIC_DEFECT_TYPE_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_DPC_STATIC_DEFECT_TYPE_DATASIZE (1)

//  enStaticDPType
static __inline GK_VOID ext_system_dpc_static_defect_type_write(VI_PIPE ViPipe,
								GK_U8 data)
{
	IOWR_8DIRECT(VREG_DPC_BASE(ViPipe) + 2, data & 0x1);
}
static __inline GK_U8 ext_system_dpc_static_defect_type_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DPC_BASE(ViPipe) + 2) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_dpc_start_thresh
// ------------------------------------------------------------------------------ //
// define the start thresh of defect pixel calibration
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_DPC_START_THRESH_DEFAULT (0x3)
#define ISP_EXT_SYSTEM_DPC_START_THRESH_DATASIZE (8)

//  u8StartThresh
static __inline GK_VOID ext_system_dpc_start_thresh_write(VI_PIPE ViPipe,
							  GK_U8 data)
{
	IOWR_8DIRECT((VREG_DPC_BASE(ViPipe) + 3), data);
}
static __inline GK_U8 ext_system_dpc_start_thresh_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_DPC_BASE(ViPipe) + 3);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_dpc_finish_thresh
// ------------------------------------------------------------------------------ //
//  the finish thresh of defect pixel calibration
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_DPC_FINISH_THRESH_DATASIZE (8)

//  u8FinishThresh
static __inline GK_VOID ext_system_dpc_finish_thresh_write(VI_PIPE ViPipe,
							   GK_U8 data)
{
	IOWR_8DIRECT(VREG_DPC_BASE(ViPipe) + 4, data);
}
static __inline GK_U8 ext_system_dpc_finish_thresh_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_DPC_BASE(ViPipe) + 4);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_dpc_trigger_status
// ------------------------------------------------------------------------------ //
// the static bad pixel trigger status
// 0:Initial status
// 1:Finished
// 2:time out
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_DPC_TRIGGER_STATUS_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_DPC_TRIGGER_STATUS_DATASIZE (2)

//  enStatus
static __inline GK_VOID ext_system_dpc_trigger_status_write(VI_PIPE ViPipe,
							    GK_U8 data)
{
	IOWR_8DIRECT(VREG_DPC_BASE(ViPipe) + 5, data);
}
static __inline GK_U8 ext_system_dpc_trigger_status_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_DPC_BASE(ViPipe) + 5);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_dpc_static_dp_show
// ------------------------------------------------------------------------------ //
// highlight static defect pixel
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_DPC_STATIC_DP_SHOW_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_DPC_STATIC_DP_SHOW_DATASIZE (1)

// bShow
static __inline GK_VOID ext_system_dpc_static_dp_show_write(VI_PIPE ViPipe,
							    GK_U8 data)
{
	IOWR_8DIRECT(VREG_DPC_BASE(ViPipe) + 6, data & 0x1);
}
static __inline GK_U8 ext_system_dpc_static_dp_show_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DPC_BASE(ViPipe) + 6) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_dpc_dynamic_cor_enable
// ------------------------------------------------------------------------------ //
// defect pixel dynamic correction enable
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_DPC_DYNAMIC_COR_ENABLE_DEFAULT (0x01)
#define ISP_EXT_SYSTEM_DPC_DYNAMIC_COR_ENABLE_DATASIZE (1)

// dynamic_cor_enable
static __inline GK_VOID ext_system_dpc_dynamic_cor_enable_write(VI_PIPE ViPipe,
								GK_U8 data)
{
	IOWR_8DIRECT(VREG_DPC_BASE(ViPipe) + 7, data & 0x1);
}
static __inline GK_U8 ext_system_dpc_dynamic_cor_enable_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DPC_BASE(ViPipe) + 7) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_dpc_count_max
// ------------------------------------------------------------------------------ //
// bad pixel calibration  the maxmum bad pixel count
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_DPC_COUNT_MAX_DEFAULT (0x400)
#define ISP_EXT_SYSTEM_DPC_COUNT_MAX_DATASIZE (16)

//  u16CountMax
static __inline GK_VOID ext_system_dpc_count_max_write(VI_PIPE ViPipe,
						       GK_U16 data)
{
	IOWR_16DIRECT(VREG_DPC_BASE(ViPipe) + 8, data);
}
static __inline GK_U16 ext_system_dpc_count_max_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_DPC_BASE(ViPipe) + 8);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_dpc_count_min
// ------------------------------------------------------------------------------ //
// bad pixel calibration  the minmum bad pixel count
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_DPC_COUNT_MIN_DEFAULT (0x1)
#define ISP_EXT_SYSTEM_DPC_COUNT_MIN_DATASIZE (16)

//  u16CountMin
static __inline GK_VOID ext_system_dpc_count_min_write(VI_PIPE ViPipe,
						       GK_U16 data)
{
	IOWR_16DIRECT(VREG_DPC_BASE(ViPipe) + 0xa, data);
}
static __inline GK_U16 ext_system_dpc_count_min_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_DPC_BASE(ViPipe) + 0xa);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_dpc_trigger_time
// ------------------------------------------------------------------------------ //
// the bad pixel trigger frame cnt of time out
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_DPC_TRIGGER_TIME_DEFAULT (0x640)
#define ISP_EXT_SYSTEM_DPC_TRIGGER_TIME_DATASIZE (16)

//  enStatus
static __inline GK_VOID ext_system_dpc_trigger_time_write(VI_PIPE ViPipe,
							  GK_U16 data)
{
	IOWR_16DIRECT(VREG_DPC_BASE(ViPipe) + 0xc, data);
}
static __inline GK_U16 ext_system_dpc_trigger_time_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_DPC_BASE(ViPipe) + 0xc);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_dpc_dynamic_manual_enable
// ------------------------------------------------------------------------------ //
// 0:auto;
// 1:manual
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_DPC_DYNAMIC_MANUAL_ENABLE_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_DPC_DYNAMIC_MANUAL_ENABLE_DATASIZE (1)

// 0x1260e ~ dynamic_manual_enable
static __inline GK_VOID
ext_system_dpc_dynamic_manual_enable_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_DPC_BASE(ViPipe) + 0xe, data & 0x1);
}
static __inline GK_U8 ext_system_dpc_dynamic_manual_enable_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DPC_BASE(ViPipe) + 0xe) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_dpc_dynamic_strength_table
// ------------------------------------------------------------------------------ //
// dpc_dynamic_strength_table
#define ISP_EXT_SYSTEM_DPC_DYNAMIC_STRENGTH_TABLE_DATASIZE (16)

static __inline GK_VOID
ext_system_dpc_dynamic_strength_table_write(VI_PIPE ViPipe, GK_U8 u8Index,
					    GK_U16 data)
{
	IOWR_16DIRECT(VREG_DPC_BASE(ViPipe) + 0x10 + u8Index * sizeof(GK_U16),
		      data);
}

static __inline GK_U16
ext_system_dpc_dynamic_strength_table_read(VI_PIPE ViPipe, GK_U8 u8Index)
{
	return IORD_16DIRECT(VREG_DPC_BASE(ViPipe) + 0x10 +
			     u8Index * sizeof(GK_U16));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_dpc_dynamic_blend_ratio_table
// ------------------------------------------------------------------------------ //
// dpc_blend_ratio_table
#define ISP_EXT_SYSTEM_DPC_DYNAMIC_BLEND_RATIO_TABLE_DATASIZE (16)

static __inline GK_VOID
ext_system_dpc_dynamic_blend_ratio_table_write(VI_PIPE ViPipe, GK_U8 u8Index,
					       GK_U16 data)
{
	IOWR_16DIRECT(VREG_DPC_BASE(ViPipe) + 0x30 + u8Index * sizeof(GK_U16),
		      data);
}

static __inline GK_U16
ext_system_dpc_dynamic_blend_ratio_table_read(VI_PIPE ViPipe, GK_U8 u8Index)
{
	return IORD_16DIRECT(VREG_DPC_BASE(ViPipe) + 0x30 +
			     u8Index * sizeof(GK_U16));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_dpc_dynamic_strength
// ------------------------------------------------------------------------------ //
// dpc_dynamic_strength
#define ISP_EXT_SYSTEM_DPC_DYNAMIC_STRENGTH_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_DPC_DYNAMIC_STRENGTH_DATASIZE (16)

static __inline GK_VOID ext_system_dpc_dynamic_strength_write(VI_PIPE ViPipe,
							      GK_U16 data)
{
	IOWR_16DIRECT(VREG_DPC_BASE(ViPipe) + 0x50, data);
}

static __inline GK_U16 ext_system_dpc_dynamic_strength_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_DPC_BASE(ViPipe) + 0x50);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_dpc_dynamic_blend_ratio_table
// ------------------------------------------------------------------------------ //
//  dpc_blend_ratio

#define ISP_EXT_SYSTEM_DPC_DYNAMIC_BLEND_RATIO_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_DPC_DYNAMIC_BLEND_RATIO_DATASIZE (9)
static __inline GK_VOID ext_system_dpc_dynamic_blend_ratio_write(VI_PIPE ViPipe,
								 GK_U16 data)
{
	IOWR_16DIRECT(VREG_DPC_BASE(ViPipe) + 0x52, data);
}

static __inline GK_U16 ext_system_dpc_dynamic_blend_ratio_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_DPC_BASE(ViPipe) + 0x52);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_dpc_manual_mode
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_DPC_MANU_MODE_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_DPC_MANU_MODE_DATASIZE (1)

static __inline GK_VOID ext_system_dpc_manual_mode_write(VI_PIPE ViPipe,
							 GK_U8 data)
{
	IOWR_8DIRECT(VREG_DPC_BASE(ViPipe) + 0x54, data & 0x1);
}
static __inline GK_U8 ext_system_dpc_manual_mode_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DPC_BASE(ViPipe) + 0x54) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_dpc_alpha0_rb_write
// ------------------------------------------------------------------------------ //
// ext_system_dpc_alpha0_rb_write
#define ISP_EXT_SYSTEM_DPC_DYNAMIC_ALPHA0_RB_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_DPC_DYNAMIC_ALPHA0_RB_DATASIZE (8)

static __inline GK_VOID ext_system_dpc_alpha0_rb_write(VI_PIPE ViPipe,
						       GK_U8 data)
{
	IOWR_8DIRECT(VREG_DPC_BASE(ViPipe) + 0x55, data);
}
static __inline GK_U8 ext_system_dpc_alpha0_rb_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DPC_BASE(ViPipe) + 0x55));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_dpc_alpha0_rb_write
// ------------------------------------------------------------------------------ //
// ext_system_dpc_alpha0_rb_write
#define ISP_EXT_SYSTEM_DPC_DYNAMIC_ALPHA0_G_DEFAULT (0x0)
static __inline GK_VOID ext_system_dpc_alpha0_g_write(VI_PIPE ViPipe,
						      GK_U8 data)
{
	IOWR_8DIRECT(VREG_DPC_BASE(ViPipe) + 0x56, data);
}
static __inline GK_U8 ext_system_dpc_alpha0_g_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DPC_BASE(ViPipe) + 0x56));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_dpc_suppress_twinkle_enable_write
// ------------------------------------------------------------------------------ //
// ext_system_dpc_suppress_twinkle_enable
#define ISP_EXT_SYSTEM_DPC_SUPPRESS_TWINKLE_ENABLE_DEFAULT (0x0)
static __inline GK_VOID
ext_system_dpc_suppress_twinkle_enable_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_DPC_BASE(ViPipe) + 0x57, data & 0x1);
}
static __inline GK_U8
ext_system_dpc_suppress_twinkle_enable_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DPC_BASE(ViPipe) + 0x57) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_dpc_suppress_twinkle_thr_write
// ------------------------------------------------------------------------------ //
// ext_system_dpc_suppress_twinkle_thr
#define ISP_EXT_SYSTEM_DPC_SUPPRESS_TWINKLE_THR_DEFAULT (0x6)
static __inline GK_VOID
ext_system_dpc_suppress_twinkle_thr_write(VI_PIPE ViPipe, GK_S8 data)
{
	IOWR_16DIRECT(VREG_DPC_BASE(ViPipe) + 0x58, data);
}
static __inline GK_S8 ext_system_dpc_suppress_twinkle_thr_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_DPC_BASE(ViPipe) + 0x58) & 0xFF);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_dpc_suppress_twinkle_slope_write
// ------------------------------------------------------------------------------ //
//  ext_system_dpc_suppress_twinkle_slope
#define ISP_EXT_SYSTEM_DPC_SUPPRESS_TWINKLE_SLOPE_DEFAULT (0x15)
static __inline GK_VOID
ext_system_dpc_suppress_twinkle_slope_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_DPC_BASE(ViPipe) + 0x59, data & 0xFF);
}
static __inline GK_U8 ext_system_dpc_suppress_twinkle_slope_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DPC_BASE(ViPipe) + 0x59) & 0xFF);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_dpc_bpt_calib_number
// ------------------------------------------------------------------------------ //
// ext_system_dpc_bpt_calib_number
#define ISP_EXT_SYSTEM_DPC_BPT_CALIB_NUMBER_DEFAULT (0x0)
static __inline GK_VOID ext_system_dpc_bpt_calib_number_write(VI_PIPE ViPipe,
							      GK_U16 data)
{
	IOWR_16DIRECT(VREG_DPC_BASE(ViPipe) + 0x5a, data);
}
static __inline GK_U16 ext_system_dpc_bpt_calib_number_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_DPC_BASE(ViPipe) + 0x5a));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_dpc_bpt_cor_number
// ------------------------------------------------------------------------------ //
// ext_system_dpc_bpt_cor_number
#define ISP_EXT_SYSTEM_DPC_BPT_COR_NUMBER_DEFAULT (0x0)
static __inline GK_VOID ext_system_dpc_bpt_cor_number_write(VI_PIPE ViPipe,
							    GK_U16 data)
{
	IOWR_16DIRECT(VREG_DPC_BASE(ViPipe) + 0x5c, data);
}
static __inline GK_U16 ext_system_dpc_bpt_cor_number_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_DPC_BASE(ViPipe) + 0x5c));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_dpc_static_attr_updata
// ------------------------------------------------------------------------------ //
// ext_system_dpc_static_attr_updata
#define ISP_EXT_SYSTEM_DPC_STATIC_ATTR_UPDATE_DEFAULT (0x0)
static __inline GK_VOID ext_system_dpc_static_attr_update_write(VI_PIPE ViPipe,
								GK_U8 data)
{
	IOWR_8DIRECT(VREG_DPC_BASE(ViPipe) + 0x5e, data & 0x1);
}
static __inline GK_U8 ext_system_dpc_static_attr_update_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DPC_BASE(ViPipe) + 0x5e) & 0x1);
}

static __inline GK_VOID ext_system_dpc_dynamic_attr_update_write(VI_PIPE ViPipe,
								 GK_U8 data)
{
	IOWR_8DIRECT(VREG_DPC_BASE(ViPipe) + 0x5f, data & 0x1);
}
static __inline GK_U8 ext_system_dpc_dynamic_attr_update_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DPC_BASE(ViPipe) + 0x5f) & 0x1);
}

#ifdef CONFIG_ISP_DPC_STATIC_TABLE_SUPPORT
/* DPC Table */
// ------------------------------------------------------------------------------ //
// Register: ext_system_dpc_calib_bpt
// ------------------------------------------------------------------------------ //
// ext_system_dpc_calib_bpt
#define ISP_EXT_SYSTEM_DPC_CALIB_BPT_DATASIZE (8192)

static __inline GK_VOID
ext_system_dpc_calib_bpt_write(VI_PIPE ViPipe, GK_U16 u16Index, GK_U32 data)
{
	IOWR_32DIRECT(VREG_DPC_BASE(ViPipe) + 0x60 + u16Index * sizeof(GK_U32),
		      data);
}

static __inline GK_U32 ext_system_dpc_calib_bpt_read(VI_PIPE ViPipe,
						     GK_U16 u16Index)
{
	return IORD_32DIRECT(VREG_DPC_BASE(ViPipe) + 0x60 +
			     u16Index * sizeof(GK_U32));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_dpc_cor_bpt
// ------------------------------------------------------------------------------ //
// ext_system_dpc_cor_bpt
#define ISP_EXT_SYSTEM_DPC_COR_BPT_DATASIZE (8192)
static __inline GK_VOID
ext_system_dpc_cor_bpt_write(VI_PIPE ViPipe, GK_U16 u16Index, GK_U32 data)
{
	IOWR_32DIRECT(VREG_DPC_BASE(ViPipe) + 0x8060 +
			      u16Index * sizeof(GK_U32),
		      data);
}

static __inline GK_U32 ext_system_dpc_cor_bpt_read(VI_PIPE ViPipe,
						   GK_U16 u16Index)
{
	return IORD_32DIRECT(VREG_DPC_BASE(ViPipe) + 0x8060 +
			     u16Index * sizeof(GK_U32));
}
#endif

// ==============================================================//
// GE
// ==============================================================//
// ------------------------------------------------------------------------------ //
// Register: ext_system_ge_npoffset
// ------------------------------------------------------------------------------ //
// ge npoffset table 0x11800 - 0x1181f
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_GE_STRENGTH_DATASIZE (16)
// args: data (16-bit)
static __inline GK_VOID ext_system_ge_npoffset_write(VI_PIPE ViPipe,
						     GK_U8 u8Index, GK_U16 data)
{
	IOWR_16DIRECT(VREG_GE_BASE(ViPipe) + u8Index * sizeof(GK_U16), data);
}

static __inline GK_U16 ext_system_ge_npoffset_read(VI_PIPE ViPipe,
						   GK_U8 u8Index)
{
	return IORD_16DIRECT(VREG_GE_BASE(ViPipe) + u8Index * sizeof(GK_U16));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_ge_threshold_
// ------------------------------------------------------------------------------ //
// ge threshold_ table 0x11820 - 0x1183F
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_GE_THRESHOLD_DEFAULT (0)
#define ISP_EXT_SYSTEM_GE_THRESHOLD_DATASIZE (16)
// args: data (16-bit)
static __inline GK_VOID
ext_system_ge_threshold_write(VI_PIPE ViPipe, GK_U8 u8Index, GK_U16 data)
{
	IOWR_16DIRECT(VREG_GE_BASE(ViPipe) + 0x20 + (u8Index * sizeof(GK_U16)),
		      data);
}

static __inline GK_U16 ext_system_ge_threshold_read(VI_PIPE ViPipe,
						    GK_U8 u8Index)
{
	return IORD_16DIRECT(VREG_GE_BASE(ViPipe) + 0x20 +
			     (u8Index * sizeof(GK_U16)));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_ge_strength_
// ------------------------------------------------------------------------------ //
// ge strength_ table 0x11840 - 0x1185f
// ------------------------------------------------------------------------------ //

static __inline GK_VOID ext_system_ge_strength_write(VI_PIPE ViPipe,
						     GK_U8 u8Index, GK_U16 data)
{
	IOWR_16DIRECT(VREG_GE_BASE(ViPipe) + 0x40 + u8Index * sizeof(GK_U16),
		      data);
}

static __inline GK_U16 ext_system_ge_strength_read(VI_PIPE ViPipe,
						   GK_U8 u8Index)
{
	return IORD_16DIRECT(VREG_GE_BASE(ViPipe) + 0x40 +
			     u8Index * sizeof(GK_U16));
}

// ge_slope
static __inline GK_VOID ext_system_ge_slope_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT(VREG_GE_BASE(ViPipe) + 0x60, data);
}

static __inline GK_U16 ext_system_ge_slope_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_GE_BASE(ViPipe) + 0x60);
}

// ge_sensitivity
static __inline GK_VOID ext_system_ge_sensitivity_write(VI_PIPE ViPipe,
							GK_U16 data)
{
	IOWR_16DIRECT(VREG_GE_BASE(ViPipe) + 0x62, data);
}

static __inline GK_U16 ext_system_ge_sensitivity_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_GE_BASE(ViPipe) + 0x62);
}

// ge_sensithreshold
static __inline GK_VOID ext_system_ge_sensithreshold_write(VI_PIPE ViPipe,
							   GK_U16 data)
{
	IOWR_16DIRECT(VREG_GE_BASE(ViPipe) + 0x64, data);
}

static __inline GK_U16 ext_system_ge_sensithreshold_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_GE_BASE(ViPipe) + 0x64);
}

// ge_enable
static __inline GK_VOID ext_system_ge_enable_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_GE_BASE(ViPipe) + 0x66, data & 0x1);
}

static __inline GK_U8 ext_system_ge_enable_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_GE_BASE(ViPipe) + 0x66) & 0x1);
}

static __inline GK_VOID ext_system_ge_coef_update_en_write(VI_PIPE ViPipe,
							   GK_U8 data)
{
	IOWR_8DIRECT(VREG_GE_BASE(ViPipe) + 0x67, data & 0x1);
}

static __inline GK_U8 ext_system_ge_coef_update_en_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_GE_BASE(ViPipe) + 0x67) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_demosaic_manual_mode_write
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
#define ISP_EXT_SYSTEM_DEMOSAIC_MANUAL_MODE_DEFAULT (0)
static __inline GK_VOID ext_system_demosaic_manual_mode_write(VI_PIPE ViPipe,
							      GK_U8 data)
{
	IOWR_8DIRECT(VREG_DM_BASE(ViPipe) + 0x10, data & 0x1);
}
static __inline GK_U8 ext_system_demosaic_manual_mode_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DM_BASE(ViPipe) + 0x10) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_demosaic_enable_write
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
#define ISP_EXT_SYSTEM_DEMOSAIC_ENABLE_DEFAULT (0x1)
#define ISP_EXT_SYSTEM_DEMOSAIC_ENABLE_DATASIZE (1)
static __inline GK_VOID ext_system_demosaic_enable_write(VI_PIPE ViPipe,
							 GK_U8 data)
{
	IOWR_8DIRECT(VREG_DM_BASE(ViPipe) + 0x11, data & 0x1);
}
static __inline GK_U8 ext_system_demosaic_enable_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DM_BASE(ViPipe) + 0x11) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_demosaic_attr_update_en_write
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_demosaic_attr_update_en_write(VI_PIPE ViPipe,
								 GK_U8 data)
{
	IOWR_8DIRECT(VREG_DM_BASE(ViPipe) + 0x12, data & 0x1);
}
static __inline GK_U8 ext_system_demosaic_attr_update_en_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DM_BASE(ViPipe) + 0x12) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_demosaic_manual_nondirection_midfreq_detailenhance_strength_write
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
#define ISP_EXT_SYSTEM_DEMOSAIC_MANUAL_NONDIRECTION_MIDFREQ_DETAILENHANCE_STRENGTH_LINEAR_DEFAULT \
	(8)
#define ISP_EXT_SYSTEM_DEMOSAIC_MANUAL_NONDIRECTION_MIDFREQ_DETAILENHANCE_STRENGTH_WDR_DEFAULT \
	(0)
static __inline GK_VOID
ext_system_demosaic_manual_nondirection_midfreq_detailenhance_strength_write(
	VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_DM_BASE(ViPipe) + 0x13, data);
}
static __inline GK_U8
ext_system_demosaic_manual_nondirection_midfreq_detailenhance_strength_read(
	VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_DM_BASE(ViPipe) + 0x13);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_demosaic_manual_nondirectiongfreq_detailenhance_strength_write
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
#define ISP_EXT_SYSTEM_DEMOSAIC_MANUAL_NONDIRECTION_HIGFREQ_DETAILENHANCE_STRENGTH_LINEAR_DEFAULT \
	(3)
#define ISP_EXT_SYSTEM_DEMOSAIC_MANUAL_NONDIRECTION_HIGFREQ_DETAILENHANCE_STRENGTH_WDR_DEFAULT \
	(0)
static __inline GK_VOID
ext_system_demosaic_manual_nondirectiongfreq_detailenhance_strength_write(
	VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_DM_BASE(ViPipe) + 0x14, data);
}
static __inline GK_U8
ext_system_demosaic_manual_nondirectiongfreq_detailenhance_strength_read(
	VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_DM_BASE(ViPipe) + 0x14);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_demosaic_manual_nondirection_strength_write
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
#define ISP_EXT_SYSTEM_DEMOSAIC_MANUAL_NONDIRECTION_STRENTH_LINEAR_DEFAULT (64)
#define ISP_EXT_SYSTEM_DEMOSAIC_MANUAL_NONDIRECTION_STRENTH_WDR_DEFAULT (64)
static __inline GK_VOID
ext_system_demosaic_manual_nondirection_strength_write(VI_PIPE ViPipe,
						       GK_U8 data)
{
	IOWR_8DIRECT(VREG_DM_BASE(ViPipe) + 0x15, data);
}
static __inline GK_U8
ext_system_demosaic_manual_nondirection_strength_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_DM_BASE(ViPipe) + 0x15);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_demosaic_manual_detail_smooth_range_write
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
#define ISP_EXT_SYSTEM_DEMOSAIC_MANUAL_DETAIL_SMOOTH_RANGE_LINEAR_DEFAULT (2)
#define ISP_EXT_SYSTEM_DEMOSAIC_MANUAL_DETAIL_SMOOTH_RANGE_WDR_DEFAULT (2)
static __inline GK_VOID
ext_system_demosaic_manual_detail_smooth_range_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_DM_BASE(ViPipe) + 0x16, data);
}
static __inline GK_U8
ext_system_demosaic_manual_detail_smooth_range_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_DM_BASE(ViPipe) + 0x16);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_demosaic_manual_detail_smooth_strength_write
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
#define ISP_EXT_SYSTEM_DEMOSAIC_MANUAL_DETAIL_SMOOTH_STRENGTH_LINEAR_DEFAULT \
	(256)
#define ISP_EXT_SYSTEM_DEMOSAIC_MANUAL_DETAIL_SMOOTH_STRENGTH_WDR_DEFAULT (256)
static __inline GK_VOID
ext_system_demosaic_manual_detail_smooth_strength_write(VI_PIPE ViPipe,
							GK_U16 data)
{
	IOWR_16DIRECT(VREG_DM_BASE(ViPipe) + 0x18, data);
}
static __inline GK_U16
ext_system_demosaic_manual_detail_smooth_strength_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_DM_BASE(ViPipe) + 0x18);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_demosaic_auto_nondirection_midfreq_detailenhance_strength_write
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID
ext_system_demosaic_auto_nondirection_midfreq_detailenhance_strength_write(
	VI_PIPE ViPipe, GK_U8 u8Index, GK_U8 data)
{
	IOWR_8DIRECT(VREG_DM_BASE(ViPipe) + 0x20 + (u8Index * sizeof(GK_U8)),
		     data);
}
static __inline GK_U8
ext_system_demosaic_auto_nondirection_midfreq_detailenhance_strength_read(
	VI_PIPE ViPipe, GK_U8 u8Index)
{
	return IORD_8DIRECT(VREG_DM_BASE(ViPipe) + 0x20 +
			    (u8Index * sizeof(GK_U8)));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_demosaic_auto_detail_smooth_range_write
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID
ext_system_demosaic_auto_nondirectiongfreq_detailenhance_strength_write(
	VI_PIPE ViPipe, GK_U8 u8Index, GK_U8 data)
{
	IOWR_8DIRECT(VREG_DM_BASE(ViPipe) + 0x30 + (u8Index * sizeof(GK_U8)),
		     data);
}
static __inline GK_U8
ext_system_demosaic_auto_nondirectiongfreq_detailenhance_strength_read(
	VI_PIPE ViPipe, GK_U8 u8Index)
{
	return IORD_8DIRECT(VREG_DM_BASE(ViPipe) + 0x30 +
			    (u8Index * sizeof(GK_U8)));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_demosaic_auto_nondirection_strength_write
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID
ext_system_demosaic_auto_nondirection_strength_write(VI_PIPE ViPipe,
						     GK_U8 u8Index, GK_U8 data)
{
	IOWR_8DIRECT(VREG_DM_BASE(ViPipe) + 0x40 + (u8Index * sizeof(GK_U8)),
		     data);
}
static __inline GK_U8
ext_system_demosaic_auto_nondirection_strength_read(VI_PIPE ViPipe,
						    GK_U8 u8Index)
{
	return IORD_8DIRECT(VREG_DM_BASE(ViPipe) + 0x40 +
			    (u8Index * sizeof(GK_U8)));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_demosaic_auto_detail_smooth_range_write
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID
ext_system_demosaic_auto_detail_smooth_range_write(VI_PIPE ViPipe,
						   GK_U8 u8Index, GK_U8 data)
{
	IOWR_8DIRECT(VREG_DM_BASE(ViPipe) + 0x50 + (u8Index * sizeof(GK_U8)),
		     data);
}
static __inline GK_U8
ext_system_demosaic_auto_detail_smooth_range_read(VI_PIPE ViPipe, GK_U8 u8Index)
{
	return IORD_8DIRECT(VREG_DM_BASE(ViPipe) + 0x50 +
			    (u8Index * sizeof(GK_U8)));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_demosaic_auto_detail_smooth_strength_write
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
#define ISP_EXT_SYSTEM_DEMOSAIC_AUTO_DETAIL_SMOOTH_STRENGTH_DEFAULT (256)
static __inline GK_VOID ext_system_demosaic_auto_detail_smooth_strength_write(
	VI_PIPE ViPipe, GK_U8 u8Index, GK_U16 data)
{
	IOWR_16DIRECT(VREG_DM_BASE(ViPipe) + 0x60 + (u8Index * sizeof(GK_U16)),
		      data);
}
static __inline GK_U16
ext_system_demosaic_auto_detail_smooth_strength_read(VI_PIPE ViPipe,
						     GK_U8 u8Index)
{
	return IORD_16DIRECT(VREG_DM_BASE(ViPipe) + 0x60 +
			     (u8Index * sizeof(GK_U16)));
}

#define ISP_EXT_SYSTEM_DEMOSAIC_MANUAL_COLORNOISE_THDF_DEFAULT (0)
static __inline GK_VOID
ext_system_demosaic_manual_colornoise_thdf_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_DM_BASE(ViPipe) + 0x80, data);
}
static __inline GK_U8
ext_system_demosaic_manual_colornoise_thdf_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_DM_BASE(ViPipe) + 0x80);
}
#define ISP_EXT_SYSTEM_DEMOSAIC_MANUAL_COLORNOISE_STRF_DEFAULT (8)
static __inline GK_VOID
ext_system_demosaic_manual_colornoise_strf_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_DM_BASE(ViPipe) + 0x81, data);
}
static __inline GK_U8
ext_system_demosaic_manual_colornoise_strf_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_DM_BASE(ViPipe) + 0x81);
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_demosaic_manual_desat_dark_range_write
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
#define ISP_EXT_SYSTEM_DEMOSAIC_MANUAL_DESAT_DARK_RANGE_DEFAULT (1)
#define ISP_EXT_SYSTEM_DEMOSAIC_MANUAL_DESAT_DARK_RANGE_DATASIZE (1)
static __inline GK_VOID
ext_system_demosaic_manual_desat_dark_range_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_DM_BASE(ViPipe) + 0x82, data);
}
static __inline GK_U8
ext_system_demosaic_manual_desat_dark_range_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_DM_BASE(ViPipe) + 0x82);
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_demosaic_manual_desat_dark_strength_write
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
#define ISP_EXT_SYSTEM_DEMOSAIC_MANUAL_DESAT_DARK_STRENGTH_DEFAULT (10)
#define ISP_EXT_SYSTEM_DEMOSAIC_MANUAL_DESAT_DARK_STRENGTH_DATASIZE (1)
static __inline GK_VOID
ext_system_demosaic_manual_desat_dark_strength_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_DM_BASE(ViPipe) + 0x83, data);
}
static __inline GK_U8
ext_system_demosaic_manual_desat_dark_strength_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_DM_BASE(ViPipe) + 0x83);
}
#define ISP_EXT_SYSTEM_DEMOSAIC_AUTO_COLORNOISE_THDF_DEFAULT (0)
static __inline GK_VOID
ext_system_demosaic_auto_colornoise_thdf_write(VI_PIPE ViPipe, GK_U8 u8Index,
					       GK_U8 data)
{
	IOWR_8DIRECT(VREG_DM_BASE(ViPipe) + 0x90 + (u8Index * sizeof(GK_U8)),
		     data);
}
static __inline GK_U8
ext_system_demosaic_auto_colornoise_thdf_read(VI_PIPE ViPipe, GK_U8 u8Index)
{
	return IORD_8DIRECT(VREG_DM_BASE(ViPipe) + 0x90 +
			    (u8Index * sizeof(GK_U8)));
}

#define ISP_EXT_SYSTEM_DEMOSAIC_AUTO_COLORNOISE_STRF_DEFAULT (8)
static __inline GK_VOID
ext_system_demosaic_auto_colornoise_strf_write(VI_PIPE ViPipe, GK_U8 u8Index,
					       GK_U8 data)
{
	IOWR_8DIRECT(VREG_DM_BASE(ViPipe) + 0xa0 + (u8Index * sizeof(GK_U8)),
		     data);
}
static __inline GK_U8
ext_system_demosaic_auto_colornoise_strf_read(VI_PIPE ViPipe, GK_U8 u8Index)
{
	return IORD_8DIRECT(VREG_DM_BASE(ViPipe) + 0xa0 +
			    (u8Index * sizeof(GK_U8)));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_demosaic_auto_desat_dark_range
// ------------------------------------------------------------------------------ //
// demosaic desat dark range
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_DEMOSAIC_AUTO_DESAT_DARK_RANGE_DEFAULT (1)
#define ISP_EXT_SYSTEM_DEMOSAIC_AUTO_DESAT_DARK_RANGE_DATASIZE (16)
// args: data (8-bit)
static __inline GK_VOID
ext_system_demosaic_auto_desat_dark_range_write(VI_PIPE ViPipe, GK_U8 u8Index,
						GK_U8 data)
{
	IOWR_8DIRECT(VREG_DM_BASE(ViPipe) + 0xb0 + (u8Index * sizeof(GK_U8)),
		     data);
}
static __inline GK_U8
ext_system_demosaic_auto_desat_dark_range_read(VI_PIPE ViPipe, GK_U8 u8Index)
{
	return IORD_8DIRECT(VREG_DM_BASE(ViPipe) + 0xb0 +
			    (u8Index * sizeof(GK_U8)));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_demosaic_auto_desat_dark_strength
// ------------------------------------------------------------------------------ //
// demosaic desat dark strength
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_DEMOSAIC_AUTO_DESAT_DARK_STRENGTH_DEFAULT (10)
#define ISP_EXT_SYSTEM_DEMOSAIC_AUTO_DESAT_DARK_STRENGTH_DATASIZE (16)
// args: data (8-bit)
static __inline GK_VOID
ext_system_demosaic_auto_desat_dark_strength_write(VI_PIPE ViPipe,
						   GK_U8 u8Index, GK_U8 data)
{
	IOWR_8DIRECT(VREG_DM_BASE(ViPipe) + 0xc0 + (u8Index * sizeof(GK_U8)),
		     data);
}
static __inline GK_U8
ext_system_demosaic_auto_desat_dark_strength_read(VI_PIPE ViPipe, GK_U8 u8Index)
{
	return IORD_8DIRECT(VREG_DM_BASE(ViPipe) + 0xc0 +
			    (u8Index * sizeof(GK_U8)));
}
// ==============================================================//
// Register: ext_system_antifalsecolor_enable_write
// ------------------------------------------------------------ //
// args: data (8-bit)
#define ISP_EXT_SYSTEM_ANTIFALSECOLOR_ENABLE_DEFAULT (0x1)
static __inline GK_VOID ext_system_antifalsecolor_enable_write(VI_PIPE ViPipe,
							       GK_U8 data)
{
	IOWR_8DIRECT(VREG_FCR_BASE(ViPipe), (data & 0x1));
}
static __inline GK_U8 ext_system_antifalsecolor_enable_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_FCR_BASE(ViPipe));
}

// ------------------------------------------------------------ //
// Register: ext_system_fcr_manual_mode_write
// ------------------------------------------------------------ //
// args: data (8-bit)
#define ISP_EXT_SYSTEM_ANTIFALSECOLOR_MANUAL_MODE_DEFAULT (0)
static __inline GK_VOID
ext_system_antifalsecolor_manual_mode_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_FCR_BASE(ViPipe) + 0x1, data);
}
static __inline GK_U8 ext_system_antifalsecolor_manual_mode_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_FCR_BASE(ViPipe) + 0x1);
}

// ------------------------------------------------------------ //
// Register: ext_system_antifalsecolor_auto_threshold_write
// ------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID
ext_system_antifalsecolor_auto_threshold_write(VI_PIPE ViPipe, GK_U8 u8Index,
					       GK_U8 data)
{
	IOWR_8DIRECT(VREG_FCR_BASE(ViPipe) + 0x2 + (u8Index * sizeof(GK_U8)),
		     data);
}
static __inline GK_U8
ext_system_antifalsecolor_auto_threshold_read(VI_PIPE ViPipe, GK_U8 u8Index)
{
	return IORD_8DIRECT(VREG_FCR_BASE(ViPipe) + 0x2 +
			    (u8Index * sizeof(GK_U8)));
}

// ------------------------------------------------------------ //
// Register: ext_system_antifalsecolor_auto_strenght_write
// ------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID
ext_system_antifalsecolor_auto_strenght_write(VI_PIPE ViPipe, GK_U8 u8Index,
					      GK_U8 data)
{
	IOWR_8DIRECT(VREG_FCR_BASE(ViPipe) + 0x12 + (u8Index * sizeof(GK_U8)),
		     data);
}
static __inline GK_U8
ext_system_antifalsecolor_auto_strenght_read(VI_PIPE ViPipe, GK_U8 u8Index)
{
	return IORD_8DIRECT(VREG_FCR_BASE(ViPipe) + 0x12 +
			    (u8Index * sizeof(GK_U8)));
}

// ------------------------------------------------------------ //
// Register: ext_system_antifalsecolor_manual_threshold_write
// ------------------------------------------------------------ //
// args: data (8-bit)
#define ISP_EXT_SYSTEM_ANTIFALSECOLOR_MANUAL_THRESHOLD_DEFAULT (8)
static __inline GK_VOID
ext_system_antifalsecolor_manual_threshold_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_FCR_BASE(ViPipe) + 0x22, data);
}
static __inline GK_U8
ext_system_antifalsecolor_manual_threshold_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_FCR_BASE(ViPipe) + 0x22);
}

// ------------------------------------------------------------ //
// Register: ext_system_antifalsecolor_manual_strenght_write
// ------------------------------------------------------------ //
// args: data (8-bit)
#define ISP_EXT_SYSTEM_ANTIFALSECOLOR_MANUAL_STRENGTH_DEFAULT (8)
static __inline GK_VOID
ext_system_antifalsecolor_manual_strenght_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_FCR_BASE(ViPipe) + 0x23, data);
}
static __inline GK_U8
ext_system_antifalsecolor_manual_strenght_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_FCR_BASE(ViPipe) + 0x23);
}

// ==============================================================//
// BAYER NR: VREG_BNR_BASE(ViPipe) ~ VREG_BNR_BASE(ViPipe)  +
// ==============================================================//
// ------------------------------------------------------------------------------ //
// Register: ext_system_bayernr_enable_write
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
#define ISP_EXT_SYSTEM_BAYERNR_ENABLE_DEFAULT (1)
#define ISP_EXT_SYSTEM_BAYERNR_ENABLE_DATASIZE (8)
static __inline GK_VOID ext_system_bayernr_enable_write(VI_PIPE ViPipe,
							GK_U8 data)
{
	IOWR_8DIRECT(VREG_BNR_BASE(ViPipe), (data & 0x1));
}
static __inline GK_U8 ext_system_bayernr_enable_read(VI_PIPE ViPipe)
{
	return ((IORD_8DIRECT(VREG_BNR_BASE(ViPipe))) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_bayernr_manual_mode_write
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
#define ISP_EXT_SYSTEM_BAYERNR_MANU_MODE_DEFAULT (0)
#define ISP_EXT_SYSTEM_BAYERNR_MANU_MODE_DATASIZE (8)
static __inline GK_VOID ext_system_bayernr_manual_mode_write(VI_PIPE ViPipe,
							     GK_U8 data)
{
	IOWR_8DIRECT(VREG_BNR_BASE(ViPipe) + 0x1, data);
}
static __inline GK_U8 ext_system_bayernr_manual_mode_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_BNR_BASE(ViPipe) + 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_bayernr_low_power_enable_write
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
#define ISP_EXT_SYSTEM_BAYERNR_LOW_POWER_ENABLE_DEFAULT (0)
#define ISP_EXT_SYSTEM_BAYERNR_LOW_POWER_ENABLE_DATASIZE (8)
static __inline GK_VOID
ext_system_bayernr_low_power_enable_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_BNR_BASE(ViPipe) + 0x2, (data & 0x1));
}
static __inline GK_U8 ext_system_bayernr_low_power_enable_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_BNR_BASE(ViPipe) + 0x2) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_bayernr_lsc_enable_write
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
#define ISP_EXT_SYSTEM_BAYERNR_LSC_ENABLE_DEFAULT (0)
#define ISP_EXT_SYSTEM_BAYERNR_LSC_ENABLE_DATASIZE (8)
static __inline GK_VOID ext_system_bayernr_lsc_enable_write(VI_PIPE ViPipe,
							    GK_U8 data)
{
	IOWR_8DIRECT(VREG_BNR_BASE(ViPipe) + 0x3, data & 0x1);
}
static __inline GK_U8 ext_system_bayernr_lsc_enable_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_BNR_BASE(ViPipe) + 0x3) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_bayernr_coef_update_en_write
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_bayernr_coef_update_en_write(VI_PIPE ViPipe,
								GK_U8 data)
{
	IOWR_8DIRECT(VREG_BNR_BASE(ViPipe) + 0x6, data & 0x1);
}
static __inline GK_U8 ext_system_bayernr_coef_update_en_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_BNR_BASE(ViPipe) + 0x6) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_bayernr_mono_sensor_write
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
#define ISP_EXT_SYSTEM_BAYERNR_MONO_SENSOR_ENABLE_DEFAULT (0)
#define ISP_EXT_SYSTEM_BAYERNR_MONO_SENSOR_ENABLE_DATASIZE (8)
static __inline GK_VOID ext_system_bayernr_mono_sensor_write(VI_PIPE ViPipe,
							     GK_U8 data)
{
	IOWR_8DIRECT(VREG_BNR_BASE(ViPipe) + 0x7, data);
}
static __inline GK_U8 ext_system_bayernr_mono_sensor_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_BNR_BASE(ViPipe) + 0x7);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_bayernr_lsc_nr_ratio_write
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
#define ISP_EXT_SYSTEM_BAYERNR_LSC_NR_RATIO_DEFAULT (255)
#define ISP_EXT_SYSTEM_BAYERNR_LSC_NR_RATIO_DATASIZE (8)
static __inline GK_VOID ext_system_bayernr_lsc_nr_ratio_write(VI_PIPE ViPipe,
							      GK_U8 data)
{
	IOWR_8DIRECT(VREG_BNR_BASE(ViPipe) + 0x8, data);
}
static __inline GK_U8 ext_system_bayernr_lsc_nr_ratio_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_BNR_BASE(ViPipe) + 0x8);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_bayernr_lsc_max_gain_write
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
#define ISP_EXT_SYSTEM_BAYERNR_LSC_MAX_GAIN_DEFAULT (96)
#define ISP_EXT_SYSTEM_BAYERNR_LSC_MAX_GAIN_DATASIZE (8)
static __inline GK_VOID ext_system_bayernr_lsc_max_gain_write(VI_PIPE ViPipe,
							      GK_U8 data)
{
	IOWR_8DIRECT(VREG_BNR_BASE(ViPipe) + 0x9, data);
}
static __inline GK_U8 ext_system_bayernr_lsc_max_gain_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_BNR_BASE(ViPipe) + 0x9);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_bayernr_lsc_cmp_strength_write
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
#define ISP_EXT_SYSTEM_BAYERNR_LSC_CMP_STRENGTH_DEFAULT (256)
#define ISP_EXT_SYSTEM_BAYERNR_LSC_CMP_STRENGTH_DATASIZE (16)
static __inline GK_VOID
ext_system_bayernr_lsc_cmp_strength_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT(VREG_BNR_BASE(ViPipe) + 0xa, data);
}
static __inline GK_U16 ext_system_bayernr_lsc_cmp_strength_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_BNR_BASE(ViPipe) + 0xa);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_bayernr_auto_coarse_strength_r
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
#define ISP_EXT_SYSTEM_BAYERNR_AUTO_COARSE_STRENGTH_R_DEFAULT (102)
#define ISP_EXT_SYSTEM_BAYERNR_AUTO_COARSE_STRENGTH_R_DATASIZE (16)
static __inline GK_VOID
ext_system_bayernr_auto_coarse_strength_r_write(VI_PIPE ViPipe, GK_U8 i,
						GK_U16 data)
{
	IOWR_16DIRECT(VREG_BNR_BASE(ViPipe) + 0x20 + (i * sizeof(GK_U16)),
		      data);
}
static __inline GK_U16
ext_system_bayernr_auto_coarse_strength_r_read(VI_PIPE ViPipe, GK_U8 i)
{
	return IORD_16DIRECT(VREG_BNR_BASE(ViPipe) + 0x20 +
			     (i * sizeof(GK_U16)));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_bayernr_auto_coarse_strength_gr
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
#define ISP_EXT_SYSTEM_BAYERNR_AUTO_COARSE_STRENGTH_GR_DEFAULT (100)
#define ISP_EXT_SYSTEM_BAYERNR_AUTO_COARSE_STRENGTH_GR_DATASIZE (16)
static __inline GK_VOID
ext_system_bayernr_auto_coarse_strength_gr_write(VI_PIPE ViPipe, GK_U8 i,
						 GK_U16 data)
{
	IOWR_16DIRECT(VREG_BNR_BASE(ViPipe) + 0x40 + (i * sizeof(GK_U16)),
		      data);
}
static __inline GK_U16
ext_system_bayernr_auto_coarse_strength_gr_read(VI_PIPE ViPipe, GK_U8 i)
{
	return IORD_16DIRECT(VREG_BNR_BASE(ViPipe) + 0x40 +
			     (i * sizeof(GK_U16)));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_bayernr_auto_coarse_strength_gb
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
#define ISP_EXT_SYSTEM_BAYERNR_AUTO_COARSE_STRENGTH_GB_DEFAULT (100)
#define ISP_EXT_SYSTEM_BAYERNR_AUTO_COARSE_STRENGTH_GB_DATASIZE (16)
static __inline GK_VOID
ext_system_bayernr_auto_coarse_strength_gb_write(VI_PIPE ViPipe, GK_U8 i,
						 GK_U16 data)
{
	IOWR_16DIRECT(VREG_BNR_BASE(ViPipe) + 0x60 + (i * sizeof(GK_U16)),
		      data);
}
static __inline GK_U16
ext_system_bayernr_auto_coarse_strength_gb_read(VI_PIPE ViPipe, GK_U8 i)
{
	return IORD_16DIRECT(VREG_BNR_BASE(ViPipe) + 0x60 +
			     (i * sizeof(GK_U16)));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_bayernr_auto_coarse_strength_b
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
#define ISP_EXT_SYSTEM_BAYERNR_AUTO_COARSE_STRENGTH_B_DEFAULT (102)
#define ISP_EXT_SYSTEM_BAYERNR_AUTO_COARSE_STRENGTH_B_DATASIZE (16)
static __inline GK_VOID
ext_system_bayernr_auto_coarse_strength_b_write(VI_PIPE ViPipe, GK_U8 i,
						GK_U16 data)
{
	IOWR_16DIRECT(VREG_BNR_BASE(ViPipe) + 0x80 + (i * sizeof(GK_U16)),
		      data);
}
static __inline GK_U16
ext_system_bayernr_auto_coarse_strength_b_read(VI_PIPE ViPipe, GK_U8 i)
{
	return IORD_16DIRECT(VREG_BNR_BASE(ViPipe) + 0x80 +
			     (i * sizeof(GK_U16)));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_bayernr_auto_chroma_strength_r_write
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID
ext_system_bayernr_auto_chroma_strength_r_write(VI_PIPE ViPipe, GK_U8 i,
						GK_U8 data)
{
	IOWR_8DIRECT(VREG_BNR_BASE(ViPipe) + 0xa0 + (i * sizeof(GK_U8)), data);
}
static __inline GK_U8
ext_system_bayernr_auto_chroma_strength_r_read(VI_PIPE ViPipe, GK_U8 i)
{
	return IORD_8DIRECT(VREG_BNR_BASE(ViPipe) + 0xa0 + (i * sizeof(GK_U8)));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_bayernr_auto_chroma_strength_gr_write
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID
ext_system_bayernr_auto_chroma_strength_gr_write(VI_PIPE ViPipe, GK_U8 i,
						 GK_U8 data)
{
	IOWR_8DIRECT(VREG_BNR_BASE(ViPipe) + 0xb0 + (i * sizeof(GK_U8)), data);
}
static __inline GK_U8
ext_system_bayernr_auto_chroma_strength_gr_read(VI_PIPE ViPipe, GK_U8 i)
{
	return IORD_8DIRECT(VREG_BNR_BASE(ViPipe) + 0xb0 + (i * sizeof(GK_U8)));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_bayernr_auto_chroma_strength_gb_write
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID
ext_system_bayernr_auto_chroma_strength_gb_write(VI_PIPE ViPipe, GK_U8 i,
						 GK_U8 data)
{
	IOWR_8DIRECT(VREG_BNR_BASE(ViPipe) + 0xc0 + (i * sizeof(GK_U8)), data);
}
static __inline GK_U8
ext_system_bayernr_auto_chroma_strength_gb_read(VI_PIPE ViPipe, GK_U8 i)
{
	return IORD_8DIRECT(VREG_BNR_BASE(ViPipe) + 0xc0 + (i * sizeof(GK_U8)));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_bayernr_auto_chroma_strength_b_write
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID
ext_system_bayernr_auto_chroma_strength_b_write(VI_PIPE ViPipe, GK_U8 i,
						GK_U8 data)
{
	IOWR_8DIRECT(VREG_BNR_BASE(ViPipe) + 0xd0 + (i * sizeof(GK_U8)), data);
}
static __inline GK_U8
ext_system_bayernr_auto_chroma_strength_b_read(VI_PIPE ViPipe, GK_U8 i)
{
	return IORD_8DIRECT(VREG_BNR_BASE(ViPipe) + 0xd0 + (i * sizeof(GK_U8)));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_bayernr_auto_fine_strength_write
// ------------------------------------------------------------------------------ //
// c ratio table
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_BAYERNR_AUTO_FINE_STRENGTH_DATASIZE (8)
// args: data (8-bit)
static __inline GK_VOID
ext_system_bayernr_auto_fine_strength_write(VI_PIPE ViPipe, GK_U8 i, GK_U8 data)
{
	IOWR_8DIRECT(VREG_BNR_BASE(ViPipe) + 0xe0 + (i * sizeof(GK_U8)), data);
}
static __inline GK_U8 ext_system_bayernr_auto_fine_strength_read(VI_PIPE ViPipe,
								 GK_U8 i)
{
	return IORD_8DIRECT(VREG_BNR_BASE(ViPipe) + 0xe0 + (i * sizeof(GK_U8)));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_bayernr_auto_coring_weight
// ------------------------------------------------------------------------------ //
// bayernr jnlm coring low
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_BAYERNR_JNLM_CORING_WEIGHT_DATASIZE (16)
// args: data (16-bit)
static __inline GK_VOID
ext_system_bayernr_auto_coring_weight_write(VI_PIPE ViPipe, GK_U8 i,
					    GK_U16 data)
{
	IOWR_16DIRECT(VREG_BNR_BASE(ViPipe) + 0x100 + (i * sizeof(GK_U16)),
		      data);
}
static __inline GK_U16
ext_system_bayernr_auto_coring_weight_read(VI_PIPE ViPipe, GK_U8 i)
{
	return IORD_16DIRECT(VREG_BNR_BASE(ViPipe) + 0x100 +
			     (i * sizeof(GK_U16)));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_bayernr_manual_chroma_strength_write
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
#define ISP_EXT_SYSTEM_BAYERNR_MANU_CHROMA_STRENGTH_DEFAULT (0)
#define ISP_EXT_SYSTEM_BAYERNR_AMED_LEVEL_DATASIZE (8)

static __inline GK_VOID
ext_system_bayernr_manual_chroma_strength_write(VI_PIPE ViPipe, GK_U8 i,
						GK_U8 data)
{
	IOWR_8DIRECT(VREG_BNR_BASE(ViPipe) + 0x200 + (i * sizeof(GK_U8)), data);
}
static __inline GK_U8
ext_system_bayernr_manual_chroma_strength_read(VI_PIPE ViPipe, GK_U8 i)
{
	return IORD_8DIRECT(VREG_BNR_BASE(ViPipe) + 0x200 +
			    (i * sizeof(GK_U8)));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_bayernr_manual_fine_strength_write
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
#define ISP_EXT_SYSTEM_BAYERNR_MANU_FINE_STRENGTH_DEFAULT (112)
#define ISP_EXT_SYSTEM_BAYERNR_MANU_FINE_STRENGTH_DATASIZE (8)
static __inline GK_VOID
ext_system_bayernr_manual_fine_strength_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_BNR_BASE(ViPipe) + 0x210, data);
}
static __inline GK_U8
ext_system_bayernr_manual_fine_strength_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_BNR_BASE(ViPipe) + 0x210);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_bayernr_manual_coring_weight_write
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
#define ISP_EXT_SYSTEM_BAYERNR_MANU_CORING_WEIGHT_DEFAULT (50)
static __inline GK_VOID
ext_system_bayernr_manual_coring_weight_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT(VREG_BNR_BASE(ViPipe) + 0x212, data);
}
static __inline GK_U16
ext_system_bayernr_manual_coring_weight_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_BNR_BASE(ViPipe) + 0x212);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_bayernr_manual_coarse_strength_write
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
#define ISP_EXT_SYSTEM_BAYERNR_MANU_COARSE_STRENGTH_DEFAULT (102)
#define ISP_EXT_SYSTEM_BAYERNR_MANU_COARSE_STRENGTH_DATASIZE (16)
static __inline GK_VOID
ext_system_bayernr_manual_coarse_strength_write(VI_PIPE ViPipe, GK_U8 i,
						GK_U16 data)
{
	IOWR_16DIRECT(VREG_BNR_BASE(ViPipe) + 0x220 + (i * sizeof(GK_U16)),
		      data);
}
static __inline GK_U16
ext_system_bayernr_manual_coarse_strength_read(VI_PIPE ViPipe, GK_U8 i)
{
	return IORD_16DIRECT(VREG_BNR_BASE(ViPipe) + 0x220 +
			     (i * sizeof(GK_U16)));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_bayernr_wdr_frame_strength_write
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
#define ISP_EXT_SYSTEM_BAYERNR_WDR_FRAME_STRENGTH_DATASIZE (8)
static __inline GK_VOID
ext_system_bayernr_wdr_frame_strength_write(VI_PIPE ViPipe, GK_U8 i, GK_U8 data)
{
	IOWR_8DIRECT(VREG_BNR_BASE(ViPipe) + 0x250 + (i * sizeof(GK_U8)), data);
}
static __inline GK_U8 ext_system_bayernr_wdr_frame_strength_read(VI_PIPE ViPipe,
								 GK_U8 i)
{
	return IORD_8DIRECT(VREG_BNR_BASE(ViPipe) + 0x250 +
			    (i * sizeof(GK_U8)));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_bayernr_fusion_frame_strength_write
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
#define ISP_EXT_SYSTEM_BAYERNR_FUSION_FRAME_STRENGTH_DATASIZE (8)
static __inline GK_VOID
ext_system_bayernr_fusion_frame_strength_write(VI_PIPE ViPipe, GK_U8 i,
					       GK_U8 data)
{
	IOWR_8DIRECT(VREG_BNR_BASE(ViPipe) + 0x270 + (i * sizeof(GK_U8)), data);
}
static __inline GK_U8
ext_system_bayernr_fusion_frame_strength_read(VI_PIPE ViPipe, GK_U8 i)
{
	return IORD_8DIRECT(VREG_BNR_BASE(ViPipe) + 0x270 +
			    (i * sizeof(GK_U8)));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_bayernr_coring_low_ratio_write
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
#define ISP_EXT_SYSTEM_BAYERNR_CORING_RATIO_LUT_DEFAULT (800)
#define ISP_EXT_SYSTEM_BAYERNR_CORING_RATIO_LUT_DATASIZE (16)
static __inline GK_VOID
ext_system_bayernr_coring_ratio_write(VI_PIPE ViPipe, GK_U8 i, GK_U16 data)
{
	IOWR_16DIRECT(VREG_BNR_BASE(ViPipe) + 0x290 + (i * sizeof(GK_U16)),
		      data);
}
static __inline GK_U16 ext_system_bayernr_coring_ratio_read(VI_PIPE ViPipe,
							    GK_U8 i)
{
	return IORD_16DIRECT(VREG_BNR_BASE(ViPipe) + 0x290 +
			     (i * sizeof(GK_U16)));
}

// ==============================================================//
// Frame stitch WDR: 0x11D00~0x11EFF
// ==============================================================/
/*******************WDR---U8 --EXT_REGISTER****************************************/
// ------------------------------------------------------------------------------ //
// Register: ext_wdr_manual_mode_enable
// ------------------------------------------------------------------------------ //
// 0: auto; 1: manual;
// ------------------------------------------------------------------------------ //
// -------------------------------------------------------------------------------//
// ******************************GK_BOOL****************************************//
// -------------------------------------------------------------------------------//
static __inline GK_VOID ext_system_wdr_manual_mode_write(VI_PIPE ViPipe,
							 GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe), data);
}
static __inline GK_U8 ext_system_wdr_manual_mode_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_WDR_BASE(ViPipe));
}

static __inline GK_VOID ext_system_wdr_coef_update_en_write(VI_PIPE ViPipe,
							    GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x1, data);
}
static __inline GK_U8 ext_system_wdr_coef_update_en_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x1);
}

// WdrEn
#define ISP_EXT_SYSTEM_WDR_EN_DEFAULT (0)
static __inline GK_VOID ext_system_wdr_en_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x2, data);
}
static __inline GK_U8 ext_system_wdr_en_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x2);
}

// FusionMode
#define ISP_EXT_SYSTEM_FUSION_MODE_DEFAULT (0)
static __inline GK_VOID ext_system_fusion_mode_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x3, data);
}
static __inline GK_U8 ext_system_fusion_mode_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x3);
}

// bMdtEn
#define ISP_EXT_SYSTEM_MDT_EN_DEFAULT (1)
static __inline GK_VOID ext_system_mdt_en_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x4, data);
}
static __inline GK_U8 ext_system_mdt_en_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x4);
}

// bErosionEn
#define ISP_EXT_SYSTEM_EROSION_EN_DEFAULT (0)
static __inline GK_VOID ext_system_erosion_en_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x5, data);
}
static __inline GK_U8 ext_system_erosion_en_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x5);
}

// shortexpo_chk
static __inline GK_VOID ext_system_wdr_shortexpo_chk_write(VI_PIPE ViPipe,
							   GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x6, data);
}
static __inline GK_U8 ext_system_wdr_shortexpo_chk_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x6);
}

// -------------------------------------------------------------------------------//
// ******************************GK_U8****************************************//
// -------------------------------------------------------------------------------//

// u8NosGWgtMod
#define ISP_EXT_SYSTEM_WDR_NOISE_G_WEIGHT_MODE_DEFAULT (0x1)
static __inline GK_VOID ext_system_wdr_noise_g_weight_mode_write(VI_PIPE ViPipe,
								 GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x10, data);
}
static __inline GK_U8 ext_system_wdr_noise_g_weight_mode_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x10));
}

// u8NosCWgtMod
#define ISP_EXT_SYSTEM_WDR_NOISE_C_WEIGHT_MODE_DEFAULT (0x3)
static __inline GK_VOID ext_system_wdr_noise_c_weight_mode_write(VI_PIPE ViPipe,
								 GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x11, data);
}
static __inline GK_U8 ext_system_wdr_noise_c_weight_mode_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x11));
}

// u8MdtLBld
#define ISP_EXT_SYSTEM_WDR_MDT_LONG_BLEND_DEFAULT (0xFE)

static __inline GK_VOID ext_system_wdr_mdt_long_blend_write(VI_PIPE ViPipe,
							    GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x12, data);
}
static __inline GK_U8 ext_system_wdr_mdt_long_blend_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x12));
}

// u8NoiseModelCoef
#define ISP_EXT_SYSTEM_WDR_NOISE_MODEL_COEF_DEFAULT (0x50)
static __inline GK_VOID ext_system_wdr_noise_model_coef_write(VI_PIPE ViPipe,
							      GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x13, data);
}
static __inline GK_U8 ext_system_wdr_noise_model_coef_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x13));
}

// u8NoiseRatioRg
#define ISP_EXT_SYSTEM_WDR_NOISE_RATIO_RG_DEFAULT (0x5A)
static __inline GK_VOID ext_system_wdr_noise_ratio_rg_write(VI_PIPE ViPipe,
							    GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x14, data);
}
static __inline GK_U8 ext_system_wdr_noise_ratio_rg_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x14));
}

// u8NoiseRatioBg
#define ISP_EXT_SYSTEM_WDR_NOISE_RATIO_BG_DEFAULT (0x5A)
static __inline GK_VOID ext_system_wdr_noise_ratio_bg_write(VI_PIPE ViPipe,
							    GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x15, data);
}
static __inline GK_U8 ext_system_wdr_noise_ratio_bg_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x15));
}

// u8Gsigma_gain1
#define ISP_EXT_SYSTEM_WDR_G_SIGMA_GAIN1_DEFAULT (0x10)
static __inline GK_VOID ext_system_wdr_g_sigma_gain1_write(VI_PIPE ViPipe,
							   GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x16, data);
}
static __inline GK_U8 ext_system_wdr_g_sigma_gain1_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x16));
}

// u8Gsigma_gain2
#define ISP_EXT_SYSTEM_WDR_G_SIGMA_GAIN2_DEFAULT (0x30)
static __inline GK_VOID ext_system_wdr_g_sigma_gain2_write(VI_PIPE ViPipe,
							   GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x17, data);
}
static __inline GK_U8 ext_system_wdr_g_sigma_gain2_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x17));
}

// u8Gsigma_gain3
#define ISP_EXT_SYSTEM_WDR_G_SIGMA_GAIN3_DEFAULT (0x50)
static __inline GK_VOID ext_system_wdr_g_sigma_gain3_write(VI_PIPE ViPipe,
							   GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x18, data);
}
static __inline GK_U8 ext_system_wdr_g_sigma_gain3_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x18));
}

// u8Csigma_gain2
#define ISP_EXT_SYSTEM_WDR_C_SIGMA_GAIN1_DEFAULT (0x8)
static __inline GK_VOID ext_system_wdr_c_sigma_gain1_write(VI_PIPE ViPipe,
							   GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x19, data);
}
static __inline GK_U8 ext_system_wdr_c_sigma_gain1_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x19));
}

// u8Csigma_gain2
#define ISP_EXT_SYSTEM_WDR_C_SIGMA_GAIN2_DEFAULT (0x10)
static __inline GK_VOID ext_system_wdr_c_sigma_gain2_write(VI_PIPE ViPipe,
							   GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x1a, data);
}
static __inline GK_U8 ext_system_wdr_c_sigma_gain2_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x1a));
}

// u8Csigma_gain3
#define ISP_EXT_SYSTEM_WDR_C_SIGMA_GAIN3_DEFAULT (0x20)
static __inline GK_VOID ext_system_wdr_c_sigma_gain3_write(VI_PIPE ViPipe,
							   GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x1b, data);
}
static __inline GK_U8 ext_system_wdr_c_sigma_gain3_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x1b));
}

// u8BnrFullMdtThr
#define ISP_EXT_SYSTEM_WDR_BNR_FULL_MDT_THR_DEFAULT (0x50)
static __inline GK_VOID ext_system_wdr_bnr_full_mdt_thr_write(VI_PIPE ViPipe,
							      GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x1d, data);
}
static __inline GK_U8 ext_system_wdr_bnr_full_mdt_thr_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x1d));
}

// u8MdtFullThr
#define ISP_EXT_SYSTEM_WDR_MDT_FULL_THR_DEFAULT (0xFE)
static __inline GK_VOID ext_system_wdr_mdt_full_thr_write(VI_PIPE ViPipe,
							  GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x1e, data);
}
static __inline GK_U8 ext_system_wdr_mdt_full_thr_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x1e));
}

// u8MdtStillThr
#define ISP_EXT_SYSTEM_WDR_MDT_STILL_THR_DEFAULT (0x14)
static __inline GK_VOID ext_system_wdr_mdt_still_thr_write(VI_PIPE ViPipe,
							   GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x1f, data);
}
static __inline GK_U8 ext_system_wdr_mdt_still_thr_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x1f));
}

// u8FullMotSigWgt;
#define ISP_EXT_SYSTEM_WDR_FULL_MOT_SIGMA_WEIGHT_DEFAULT (31)
static __inline GK_VOID
ext_system_wdr_full_mot_sigma_weight_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x20, data);
}
static __inline GK_U8 ext_system_wdr_full_mot_sigma_weight_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x20));
}

// bnrmode
#define ISP_EXT_SYSTEM_BNR_MODE_WRITE_DEFAULT (1)
static __inline GK_VOID ext_system_bnr_mode_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x21, data);
}
static __inline GK_U8 ext_system_bnr_mode_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x21));
}

static __inline GK_VOID
ext_system_wdr_manual_mdthr_low_gain_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x22, data);
}
static __inline GK_U8 ext_system_wdr_manual_mdthr_low_gain_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x22));
}

static __inline GK_VOID ext_system_wdr_manual_mdthrg_gain_write(VI_PIPE ViPipe,
								GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x23, data);
}
static __inline GK_U8 ext_system_wdr_manual_mdthrg_gain_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x23));
}

#define ISP_EXT_SYSTEM_WDR_MDREF_FLICKER_DEFAULT (0x1)

static __inline GK_VOID ext_system_wdr_mdref_flicker_write(VI_PIPE ViPipe,
							   GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x24, data);
}
static __inline GK_U8 ext_system_wdr_mdref_flicker_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x24));
}

// -------------------------------------------------------------------------------//
// ******************************GK_U16****************************************//
// -------------------------------------------------------------------------------//
// long thr
#define ISP_EXT_SYSTEM_WDR_LONGTHR_WRITE_DEFAULT (0xBC0)
static __inline GK_VOID ext_system_wdr_longthr_write(VI_PIPE ViPipe,
						     GK_U16 data)
{
	IOWR_16DIRECT(VREG_WDR_BASE(ViPipe) + 0x40, data);
}
static __inline GK_U16 ext_system_wdr_longthr_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_WDR_BASE(ViPipe) + 0x40));
}

// short thr
#define ISP_EXT_SYSTEM_WDR_SHORTTHR_WRITE_DEFAULT (0xFC0)
static __inline GK_VOID ext_system_wdr_shortthr_write(VI_PIPE ViPipe,
						      GK_U16 data)
{
	IOWR_16DIRECT(VREG_WDR_BASE(ViPipe) + 0x42, data);
}
static __inline GK_U16 ext_system_wdr_shortthr_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_WDR_BASE(ViPipe) + 0x42));
}

static __inline GK_VOID ext_system_fusion_thr_write(VI_PIPE ViPipe, GK_U8 i,
						    GK_U16 data)
{
	IOWR_16DIRECT(VREG_WDR_BASE(ViPipe) + 0x44 + (i * sizeof(GK_U16)),
		      data);
}
static __inline GK_U16 ext_system_fusion_thr_read(VI_PIPE ViPipe, GK_U8 i)
{
	return IORD_16DIRECT(VREG_WDR_BASE(ViPipe) + 0x44 +
			     (i * sizeof(GK_U16)));
}

#define ISP_EXT_SYSTEM_WDR_WBGAIN_POSITION_DEFAULT (1)
#define ISP_EXT_SYSTEM_WDR_WBGAIN_POSITION_DATASIZE (8)
#define ISP_EXT_SYSTEM_WDR_WBGAIN_IN_DG1 (0)
#define ISP_EXT_SYSTEM_WDR_WBGAIN_IN_WB (1)

// args: data (2-bit)
static __inline GK_VOID ext_system_wdr_wbgain_position_write(VI_PIPE ViPipe,
							     GK_U8 data)
{
	IOWR_8DIRECT((VREG_WDR_BASE(ViPipe) + 0x4f), data);
}
static __inline GK_U8 ext_system_wdr_wbgain_position_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0x4f);
}

// -------------------------------------------------------------------------------//
// ******************************LUT****************************************//
// -------------------------------------------------------------------------------//
// wdr floorset
static __inline GK_VOID ext_system_wdr_floorset_write(VI_PIPE ViPipe, GK_U8 i,
						      GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0xb0 + (i * sizeof(GK_U8)), data);
}
static __inline GK_U8 ext_system_wdr_floorset_read(VI_PIPE ViPipe, GK_U8 i)
{
	return IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0xb0 + (i * sizeof(GK_U8)));
}

static __inline GK_VOID
ext_system_wdr_auto_mdthr_low_gain_write(VI_PIPE ViPipe, GK_U8 i, GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0xc0 + (i * sizeof(GK_U8)), data);
}
static __inline GK_U8 ext_system_wdr_auto_mdthr_low_gain_read(VI_PIPE ViPipe,
							      GK_U8 i)
{
	return (IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0xc0 +
			     (i * sizeof(GK_U8))));
}

static __inline GK_VOID
ext_system_wdr_auto_mdthrg_gain_write(VI_PIPE ViPipe, GK_U8 i, GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0xd0 + (i * sizeof(GK_U8)), data);
}
static __inline GK_U8 ext_system_wdr_auto_mdthrg_gain_read(VI_PIPE ViPipe,
							   GK_U8 i)
{
	return (IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0xd0 +
			     (i * sizeof(GK_U8))));
}

// u8TextureThdWgt
#define ISP_EXT_SYSTEM_WDR_TEXTURE_THD_WGT (0x18)
static __inline GK_VOID ext_system_wdr_texture_thd_wgt_write(VI_PIPE ViPipe,
							     GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0xe0, data);
}

static __inline GK_U8 ext_system_wdr_texture_thd_wgt_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0xe0));
}

// bSFNR
#define ISP_EXT_SYSTEM_WDR_WDR_SFNR_EN_WGT (0x1)
static __inline GK_VOID ext_system_wdr_sfnr_en_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0xe1, data);
}

static __inline GK_U8 ext_system_wdr_sfnr_en_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0xe1));
}

// isp_wdr_forcelong_en

#define ISP_EXT_SYSTEM_WDR_FORCELONG_EN (0x1)
static __inline GK_VOID ext_system_wdr_forcelong_en_write(VI_PIPE ViPipe,
							  GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0xe2, data);
}

static __inline GK_U8 ext_system_wdr_forcelong_en_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0xe2));
}

// isp_wdr_forcelong_low_thd
#define ISP_EXT_SYSTEM_WDR_FORCELONG_LOW_THD (0x1F4)
static __inline GK_VOID ext_system_wdr_forcelong_low_thd_write(VI_PIPE ViPipe,
							       GK_U16 data)
{
	IOWR_16DIRECT(VREG_WDR_BASE(ViPipe) + 0xe4, data);
}

static __inline GK_U16 ext_system_wdr_forcelong_low_thd_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_WDR_BASE(ViPipe) + 0xe4));
}

// isp_wdr_forcelonggh_thd
#define ISP_EXT_SYSTEM_WDR_FORCELONG_HIGH_THD (0x2BC)
static __inline GK_VOID ext_system_wdr_forcelonggh_thd_write(VI_PIPE ViPipe,
							     GK_U16 data)
{
	IOWR_16DIRECT(VREG_WDR_BASE(ViPipe) + 0xe6, data);
}

static __inline GK_U16 ext_system_wdr_forcelonggh_thd_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_WDR_BASE(ViPipe) + 0xe6));
}

// isp_wdr_shortcheck_thd
#define ISP_EXT_SYSTEM_WDR_SHORTCHECK_THD (0xc)
static __inline GK_VOID ext_system_wdr_shortcheck_thd_write(VI_PIPE ViPipe,
							    GK_U16 data)
{
	IOWR_16DIRECT(VREG_WDR_BASE(ViPipe) + 0xe8, data);
}

static __inline GK_U16 ext_system_wdr_shortcheck_thd_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_WDR_BASE(ViPipe) + 0xe8));
}

// isp_wdr_forcelong_en
#define ISP_EXT_SYSTEM_WDR_MOT2SIG_GWGT_WGT (0x18)
static __inline GK_VOID ext_system_wdr_mot2sig_gwgtgh_write(VI_PIPE ViPipe,
							    GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0xea, data);
}

static __inline GK_U8 ext_system_wdr_mot2sig_gwgtgh_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0xea));
}

#define ISP_EXT_SYSTEM_WDR_MOT2SIG_CWGT_WGT (0x18)
static __inline GK_VOID ext_system_wdr_mot2sig_cwgtgh_write(VI_PIPE ViPipe,
							    GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0xec, data);
}

static __inline GK_U8 ext_system_wdr_mot2sig_cwgtgh_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0xec));
}

#define ISP_EXT_SYSTEM_WDR_SHORTSIGMAL1_GWGT_WGT (16)
static __inline GK_VOID ext_system_wdr_shortsigmal1_gwgt_write(VI_PIPE ViPipe,
							       GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0xf0, data);
}

static __inline GK_U8 ext_system_wdr_shortsigmal1_gwgt_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0xf0));
}

#define ISP_EXT_SYSTEM_WDR_SHORTSIGMAL2_GWGT_WGT (48)

static __inline GK_VOID ext_system_wdr_shortsigmal2_gwgt_write(VI_PIPE ViPipe,
							       GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0xf2, data);
}

static __inline GK_U8 ext_system_wdr_shortsigmal2_gwgt_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0xf2));
}

#define ISP_EXT_SYSTEM_WDR_SHORTSIGMAL1_CWGT_WGT (16)

static __inline GK_VOID ext_system_wdr_shortsigmal1_cwgt_write(VI_PIPE ViPipe,
							       GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0xf4, data);
}

static __inline GK_U8 ext_system_wdr_shortsigmal1_cwgt_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0xf4));
}

#define ISP_EXT_SYSTEM_WDR_SHORTSIGMAL2_CWGT_WGT (48)

static __inline GK_VOID ext_system_wdr_shortsigmal2_cwgt_write(VI_PIPE ViPipe,
							       GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0xf6, data);
}

static __inline GK_U8 ext_system_wdr_shortsigmal2_cwgt_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0xf6));
}

#define ISP_EXT_SYSTEM_WDR_FUSIONSIGMA_CWGT0_WGT (16)

static __inline GK_VOID ext_system_wdr_fusionsigma_cwgt0_write(VI_PIPE ViPipe,
							       GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0xf8, data);
}

static __inline GK_U8 ext_system_wdr_fusionsigma_cwgt0_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0xf8));
}

#define ISP_EXT_SYSTEM_WDR_FUSIONSIGMA_CWGT1_WGT (8)

static __inline GK_VOID ext_system_wdr_fusionsigma_cwgt1_write(VI_PIPE ViPipe,
							       GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0xf9, data);
}

static __inline GK_U8 ext_system_wdr_fusionsigma_cwgt1_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0xf9));
}

#define ISP_EXT_SYSTEM_WDR_FUSIONSIGMA_GWGT0_WGT (16)

static __inline GK_VOID ext_system_wdr_fusionsigma_gwgt0_write(VI_PIPE ViPipe,
							       GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0xfa, data);
}

static __inline GK_U8 ext_system_wdr_fusionsigma_gwgt0_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0xfa));
}

#define ISP_EXT_SYSTEM_WDR_FUSIONSIGMA_GWGT1_WGT (8)

static __inline GK_VOID ext_system_wdr_fusionsigma_gwgt1_write(VI_PIPE ViPipe,
							       GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0xfb, data);
}

static __inline GK_U8 ext_system_wdr_fusionsigma_gwgt1_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0xfb));
}

#define ISP_EXT_SYSTEM_WDR_FUSION_BNR_STR_WGT (16)
static __inline GK_VOID ext_system_wdr_fusionbnrstr_write(VI_PIPE ViPipe,
							  GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0xfc, data);
}
static __inline GK_U8 ext_system_wdr_fusionbnrstr_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0xfc));
}

#define ISP_EXT_SYSTEM_WDR_MOTION_BNR_STR_WGT (24)

static __inline GK_VOID ext_system_wdr_motionbnrstr_write(VI_PIPE ViPipe,
							  GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0xfd, data);
}

static __inline GK_U8 ext_system_wdr_motionbnrstr_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0xfd));
}

#define ISP_EXT_SYSTEM_WDR_SHORTFRAME_NR_STR_WGT (48)

static __inline GK_VOID ext_system_wdr_shortframe_nrstr_write(VI_PIPE ViPipe,
							      GK_U8 data)
{
	IOWR_8DIRECT(VREG_WDR_BASE(ViPipe) + 0xfe, data);
}
static __inline GK_U8 ext_system_wdr_shortframe_nrstr_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_WDR_BASE(ViPipe) + 0xfe));
}

/*******************WDR---U32 ARRAY--EXT_REGISTER**********************************/
// ==============================================================//
// FLICKER
// ==============================================================//
// ------------------------------------------------------------------------------ //
// Register: ext_system_flicker_min_band_num
// ------------------------------------------------------------------------------ //
// flicker_min_band_num
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_flicker_min_band_num_write(VI_PIPE ViPipe,
							      GK_U8 data)
{
	IOWR_8DIRECT(VREG_FLICK_BASE(ViPipe) + 0X0, data);
}
static __inline GK_U8 ext_system_flicker_min_band_num_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_FLICK_BASE(ViPipe) + 0X0);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_flicker_min_valid_band_pcnt
// ------------------------------------------------------------------------------ //
// flicker_min_valid_band_pcnt
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID
ext_system_flicker_min_valid_band_pcnt_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_FLICK_BASE(ViPipe) + 0x1, data);
}
static __inline GK_U8
ext_system_flicker_min_valid_band_pcnt_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_FLICK_BASE(ViPipe) + 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_flicker_wave_diff1
// ------------------------------------------------------------------------------ //
// flicker_wave_diff1
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_flicker_wave_diff1_write(VI_PIPE ViPipe,
							    GK_U8 data)
{
	IOWR_8DIRECT(VREG_FLICK_BASE(ViPipe) + 0x2, data);
}
static __inline GK_U8 ext_system_flicker_wave_diff1_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_FLICK_BASE(ViPipe) + 0x2);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_flicker_wave_diff2
// ------------------------------------------------------------------------------ //
// flicker_wave_diff2
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_flicker_wave_diff2_write(VI_PIPE ViPipe,
							    GK_U8 data)
{
	IOWR_8DIRECT(VREG_FLICK_BASE(ViPipe) + 0x3, data);
}
static __inline GK_U8 ext_system_flicker_wave_diff2_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_FLICK_BASE(ViPipe) + 0x3);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_flicker_period
// ------------------------------------------------------------------------------ //
// flicker_period
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_flicker_period_write(VI_PIPE ViPipe,
							GK_U8 data)
{
	IOWR_8DIRECT(VREG_FLICK_BASE(ViPipe) + 0x4, data);
}
static __inline GK_U8 ext_system_flicker_period_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_FLICK_BASE(ViPipe) + 0x4);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_flicker_gr_cnt
// ------------------------------------------------------------------------------ //
// flicker_gr_cnt
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_flicker_gr_cnt_write(VI_PIPE ViPipe,
							GK_U8 data)
{
	IOWR_8DIRECT(VREG_FLICK_BASE(ViPipe) + 0x5, data);
}
static __inline GK_U8 ext_system_flicker_gr_cnt_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_FLICK_BASE(ViPipe) + 0x5);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_flicker_gb_cnt
// ------------------------------------------------------------------------------ //
// flicker_gb_cnt
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_flicker_gb_cnt_write(VI_PIPE ViPipe,
							GK_U8 data)
{
	IOWR_8DIRECT(VREG_FLICK_BASE(ViPipe) + 0x6, data);
}
static __inline GK_U8 ext_system_flicker_gb_cnt_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_FLICK_BASE(ViPipe) + 0x6);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_flicker_result
// ------------------------------------------------------------------------------ //
// flicker_result
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_flicker_result_write(VI_PIPE ViPipe,
							GK_U8 data)
{
	IOWR_8DIRECT(VREG_FLICK_BASE(ViPipe) + 0x7, data);
}
static __inline GK_U8 ext_system_flicker_result_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_FLICK_BASE(ViPipe) + 0x7);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_freq_result
// ------------------------------------------------------------------------------ //
// freq_result
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_freq_result_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_FLICK_BASE(ViPipe) + 0x8, data);
}
static __inline GK_U8 ext_system_freq_result_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_FLICK_BASE(ViPipe) + 0x8);
}

static __inline GK_VOID ext_system_flicker_over_cnt_thr_write(VI_PIPE ViPipe,
							      GK_U32 data)
{
	IOWR_32DIRECT(VREG_FLICK_BASE(ViPipe) + 0xc, data);
}
static __inline GK_U32 ext_system_flicker_over_cnt_thr_read(VI_PIPE ViPipe)
{
	return IORD_32DIRECT(VREG_FLICK_BASE(ViPipe) + 0xc);
}

static __inline GK_VOID ext_system_flicker_over_thr_write(VI_PIPE ViPipe,
							  GK_U16 data)
{
	IOWR_16DIRECT(VREG_FLICK_BASE(ViPipe) + 0x10, data);
}
static __inline GK_U16 ext_system_flicker_over_thr_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_FLICK_BASE(ViPipe) + 0x10);
}

// ==============================================================//
// lSC: 0x12000~0x13FFF
// ==============================================================//
static __inline GK_VOID ext_system_isp_mesh_shading_enable_write(VI_PIPE ViPipe,
								 GK_U8 data)
{
	IOWR_8DIRECT(VREG_LSC_BASE(ViPipe), data & 0x1);
}

static __inline GK_U8 ext_system_isp_mesh_shading_enable_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_LSC_BASE(ViPipe)) & 0x1);
}

// args: data (8-bit)

#define ISP_EXT_SYSTEM_ISP_MESH_SHADING_MANU_MODE_DEFAULT (0)
#define ISP_EXT_SYSTEM_ISP_MESH_SHADING_MANU_MODE_DATASIZE (8)

static __inline GK_VOID
ext_system_isp_mesh_shading_lut_attr_updata_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_LSC_BASE(ViPipe) + 1, data);
}
static __inline GK_U8
ext_system_isp_mesh_shading_lut_attr_updata_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_LSC_BASE(ViPipe) + 1);
}

#define ISP_EXT_SYSTEM_ISP_MESH_SHADING_UPDATA_DEFAULT (0)
#define ISP_EXT_SYSTEM_ISP_MESH_SHADING_UPDATA_DATASIZE (8)

// args: data (8-bit)
static __inline GK_VOID
ext_system_isp_mesh_shading_attr_updata_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_LSC_BASE(ViPipe) + 2, data);
}
static __inline GK_U8
ext_system_isp_mesh_shading_attr_updata_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_LSC_BASE(ViPipe) + 2);
}

static __inline GK_VOID
ext_system_isp_mesh_shading_mesh_scale_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_LSC_BASE(ViPipe) + 3, data);
}

static __inline GK_U8
ext_system_isp_mesh_shading_mesh_scale_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_LSC_BASE(ViPipe) + 3));
}

static __inline GK_VOID
ext_system_isp_mesh_shading_mesh_strength_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT(VREG_LSC_BASE(ViPipe) + 4, data);
}

static __inline GK_U16
ext_system_isp_mesh_shading_mesh_strength_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_LSC_BASE(ViPipe) + 4));
}

static __inline GK_VOID ext_system_isp_fe_lsc_enable_write(VI_PIPE ViPipe,
							   GK_U8 data)
{
	IOWR_8DIRECT(VREG_LSC_BASE(ViPipe) + 6, data & 0x1);
}

static __inline GK_U8 ext_system_isp_fe_lsc_enable_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_LSC_BASE(ViPipe) + 6) & 0x1);
}

/* length : 32;bytes: 16 * 2 */
static __inline GK_VOID ext_system_isp_mesh_shading_xgrid_write(VI_PIPE ViPipe,
								GK_U8 u8Index,
								GK_U16 data)
{
	IOWR_16DIRECT(VREG_LSC_BASE(ViPipe) + 8 + (u8Index * sizeof(GK_U16)),
		      data);
}

static __inline GK_U16 ext_system_isp_mesh_shading_xgrid_read(VI_PIPE ViPipe,
							      GK_U8 u8Index)
{
	return IORD_16DIRECT(VREG_LSC_BASE(ViPipe) + 8 +
			     (u8Index * sizeof(GK_U16)));
}

/* length: 16;bytes: 16 * 2 */
static __inline GK_VOID ext_system_isp_mesh_shading_ygrid_write(VI_PIPE ViPipe,
								GK_U8 u8Index,
								GK_U16 data)
{
	IOWR_16DIRECT(VREG_LSC_BASE(ViPipe) + 0x28 + (u8Index * sizeof(GK_U16)),
		      data);
}

static __inline GK_U16 ext_system_isp_mesh_shading_ygrid_read(VI_PIPE ViPipe,
							      GK_U8 u8Index)
{
	return IORD_16DIRECT(VREG_LSC_BASE(ViPipe) + 0x28 +
			     (u8Index * sizeof(GK_U16)));
}

/* bytes : 2 */
static __inline GK_VOID
ext_system_isp_mesh_shading_blendratio_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT(VREG_LSC_BASE(ViPipe) + 0x48, data);
}

static __inline GK_U16
ext_system_isp_mesh_shading_blendratio_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_LSC_BASE(ViPipe) + 0x48);
}

/* length:  1089;bytes: 1089 * 2 */
static __inline GK_VOID
ext_system_isp_mesh_shading_r_gain0_write(VI_PIPE ViPipe, GK_U16 u16Index,
					  GK_U16 data)
{
	IOWR_16DIRECT(VREG_LSC_BASE(ViPipe) + 0x50 +
			      (u16Index * sizeof(GK_U16)),
		      data);
}

static __inline GK_U16 ext_system_isp_mesh_shading_r_gain0_read(VI_PIPE ViPipe,
								GK_U16 u16Index)
{
	return IORD_16DIRECT(VREG_LSC_BASE(ViPipe) + 0x50 +
			     (u16Index * sizeof(GK_U16)));
}

/* length:  1089;bytes: 1089 * 2 */
static __inline GK_VOID
ext_system_isp_mesh_shading_gr_gain0_write(VI_PIPE ViPipe, GK_U16 u16Index,
					   GK_U16 data)
{
	IOWR_16DIRECT(VREG_LSC_BASE(ViPipe) + 0x8D2 +
			      (u16Index * sizeof(GK_U16)),
		      data);
}

static __inline GK_U16
ext_system_isp_mesh_shading_gr_gain0_read(VI_PIPE ViPipe, GK_U16 u16Index)
{
	return IORD_16DIRECT(VREG_LSC_BASE(ViPipe) + 0x8D2 +
			     (u16Index * sizeof(GK_U16)));
}

/* length: 1089;bytes: 1089 * 2 */
static __inline GK_VOID
ext_system_isp_mesh_shading_gb_gain0_write(VI_PIPE ViPipe, GK_U16 u16Index,
					   GK_U16 data)
{
	IOWR_16DIRECT(VREG_LSC_BASE(ViPipe) + 0x1154 +
			      (u16Index * sizeof(GK_U16)),
		      data);
}

static __inline GK_U16
ext_system_isp_mesh_shading_gb_gain0_read(VI_PIPE ViPipe, GK_U16 u16Index)
{
	return IORD_16DIRECT(VREG_LSC_BASE(ViPipe) + 0x1154 +
			     (u16Index * sizeof(GK_U16)));
}

/* length: 1089;bytes: 1089 * 2 */
static __inline GK_VOID
ext_system_isp_mesh_shading_b_gain0_write(VI_PIPE ViPipe, GK_U16 u16Index,
					  GK_U16 data)
{
	IOWR_16DIRECT(VREG_LSC_BASE(ViPipe) + 0x19D6 +
			      (u16Index * sizeof(GK_U16)),
		      data);
}

static __inline GK_U16 ext_system_isp_mesh_shading_b_gain0_read(VI_PIPE ViPipe,
								GK_U16 u16Index)
{
	return IORD_16DIRECT(VREG_LSC_BASE(ViPipe) + 0x19D6 +
			     (u16Index * sizeof(GK_U16)));
}

/* length: 1089;bytes: 1089 * 2 */
static __inline GK_VOID
ext_system_isp_mesh_shading_r_gain1_write(VI_PIPE ViPipe, GK_U16 u16Index,
					  GK_U16 data)
{
	IOWR_16DIRECT(VREG_LSC_BASE(ViPipe) + 0x2258 +
			      (u16Index * sizeof(GK_U16)),
		      data);
}

static __inline GK_U16 ext_system_isp_mesh_shading_r_gain1_read(VI_PIPE ViPipe,
								GK_U16 u16Index)
{
	return IORD_16DIRECT(VREG_LSC_BASE(ViPipe) + 0x2258 +
			     (u16Index * sizeof(GK_U16)));
}

/* length: 1089;bytes: 1089 * 2 */
static __inline GK_VOID
ext_system_isp_mesh_shading_gr_gain1_write(VI_PIPE ViPipe, GK_U16 u16Index,
					   GK_U16 data)
{
	IOWR_16DIRECT(VREG_LSC_BASE(ViPipe) + 0x2ADA +
			      (u16Index * sizeof(GK_U16)),
		      data);
}

static __inline GK_U16
ext_system_isp_mesh_shading_gr_gain1_read(VI_PIPE ViPipe, GK_U16 u16Index)
{
	return IORD_16DIRECT(VREG_LSC_BASE(ViPipe) + 0x2ADA +
			     (u16Index * sizeof(GK_U16)));
}

/* length: 1089;bytes: 1089 * 2 */
static __inline GK_VOID
ext_system_isp_mesh_shading_gb_gain1_write(VI_PIPE ViPipe, GK_U16 u16Index,
					   GK_U16 data)
{
	IOWR_16DIRECT(VREG_LSC_BASE(ViPipe) + 0x335C +
			      (u16Index * sizeof(GK_U16)),
		      data);
}

static __inline GK_U16
ext_system_isp_mesh_shading_gb_gain1_read(VI_PIPE ViPipe, GK_U16 u16Index)
{
	return IORD_16DIRECT(VREG_LSC_BASE(ViPipe) + 0x335C +
			     (u16Index * sizeof(GK_U16)));
}

/* length: 1089;bytes: 1089 * 2 */
static __inline GK_VOID
ext_system_isp_mesh_shading_b_gain1_write(VI_PIPE ViPipe, GK_U16 u16Index,
					  GK_U16 data)
{
	IOWR_16DIRECT(VREG_LSC_BASE(ViPipe) + 0x3BDE +
			      (u16Index * sizeof(GK_U16)),
		      data);
}

static __inline GK_U16 ext_system_isp_mesh_shading_b_gain1_read(VI_PIPE ViPipe,
								GK_U16 u16Index)
{
	return IORD_16DIRECT(VREG_LSC_BASE(ViPipe) + 0x3BDE +
			     (u16Index * sizeof(GK_U16)));
}

static __inline GK_VOID
ext_system_isp_mesh_shading_fe_lut_attr_updata_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_LSC_BASE(ViPipe) + 0x4460, data);
}
static __inline GK_U8
ext_system_isp_mesh_shading_fe_lut_attr_updata_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_LSC_BASE(ViPipe) + 0x4460);
}

// args: data (8-bit)
static __inline GK_VOID
ext_system_isp_mesh_shading_fe_attr_updata_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_LSC_BASE(ViPipe) + 0x4461, data);
}
static __inline GK_U8
ext_system_isp_mesh_shading_fe_attr_updata_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_LSC_BASE(ViPipe) + 0x4461);
}

/* length : 129; bytes : 129*2 = 258 = 0x102 */
static __inline GK_VOID ext_system_isp_bnr_shading_r_gain_write(VI_PIPE ViPipe,
								GK_U8 u8Index,
								GK_U16 data)
{
	IOWR_16DIRECT(
		(VREG_LSC_BASE(ViPipe) + 0x4462 + u8Index * sizeof(GK_U16)),
		data);
}

static __inline GK_U16 ext_system_isp_bnr_shading_r_gain_read(VI_PIPE ViPipe,
							      GK_U8 u8Index)
{
	return (IORD_16DIRECT(
		(VREG_LSC_BASE(ViPipe) + 0x4462 + u8Index * sizeof(GK_U16))));
}

/* length : 129; bytes : 129*2 = 258 = 0x102 */
static __inline GK_VOID ext_system_isp_bnr_shading_gr_gain_write(VI_PIPE ViPipe,
								 GK_U8 u8Index,
								 GK_U16 data)
{
	IOWR_16DIRECT(
		(VREG_LSC_BASE(ViPipe) + 0x4564 + u8Index * sizeof(GK_U16)),
		data);
}

static __inline GK_U16 ext_system_isp_bnr_shading_gr_gain_read(VI_PIPE ViPipe,
							       GK_U8 u8Index)
{
	return (IORD_16DIRECT(
		(VREG_LSC_BASE(ViPipe) + 0x4564 + u8Index * sizeof(GK_U16))));
}

/* length : 129; bytes : 129*2 = 258 = 0x102 */
static __inline GK_VOID ext_system_isp_bnr_shading_gb_gain_write(VI_PIPE ViPipe,
								 GK_U8 u8Index,
								 GK_U16 data)
{
	IOWR_16DIRECT(
		(VREG_LSC_BASE(ViPipe) + 0x4666 + u8Index * sizeof(GK_U16)),
		data);
}

static __inline GK_U16 ext_system_isp_bnr_shading_gb_gain_read(VI_PIPE ViPipe,
							       GK_U8 u8Index)
{
	return (IORD_16DIRECT(
		(VREG_LSC_BASE(ViPipe) + 0x4666 + u8Index * sizeof(GK_U16))));
}

/* length : 129; bytes : 129*2 = 258 = 0x102 */
static __inline GK_VOID ext_system_isp_bnr_shading_b_gain_write(VI_PIPE ViPipe,
								GK_U8 u8Index,
								GK_U16 data)
{
	IOWR_16DIRECT(
		(VREG_LSC_BASE(ViPipe) + 0x4768 + u8Index * sizeof(GK_U16)),
		data);
}

static __inline GK_U16 ext_system_isp_bnr_shading_b_gain_read(VI_PIPE ViPipe,
							      GK_U8 u8Index)
{
	return (IORD_16DIRECT(
		(VREG_LSC_BASE(ViPipe) + 0x4768 + u8Index * sizeof(GK_U16))));
}

// ==============================================================//
// RLSC
// ==============================================================//
static __inline GK_VOID
ext_system_isp_radial_shading_enable_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_RLSC_BASE(ViPipe) + 0x0, data & 0x1);
}

static __inline GK_U8 ext_system_isp_radial_shading_enable_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_RLSC_BASE(ViPipe) + 0x0));
}

static __inline GK_VOID
ext_system_isp_radial_shading_scale_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_RLSC_BASE(ViPipe) + 0x6, data);
}

static __inline GK_U8 ext_system_isp_radial_shading_scale_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_RLSC_BASE(ViPipe) + 0x6));
}

static __inline GK_VOID
ext_system_isp_radial_shading_strength_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT(VREG_RLSC_BASE(ViPipe) + 0x8, data);
}

static __inline GK_U16
ext_system_isp_radial_shading_strength_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_RLSC_BASE(ViPipe) + 0x8));
}

static __inline GK_VOID
ext_system_isp_radial_shading_coefupdate_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_RLSC_BASE(ViPipe) + 0xa, data);
}

static __inline GK_U8
ext_system_isp_radial_shading_coefupdate_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_RLSC_BASE(ViPipe) + 0xa));
}

static __inline GK_VOID
ext_system_isp_radial_shading_lutupdate_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_RLSC_BASE(ViPipe) + 0xb, data);
}

static __inline GK_U8
ext_system_isp_radial_shading_lutupdate_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_RLSC_BASE(ViPipe) + 0xb));
}

static __inline GK_VOID
ext_system_isp_radial_shading_lightmode_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_RLSC_BASE(ViPipe) + 0xc, data);
}

static __inline GK_U8
ext_system_isp_radial_shading_lightmode_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_RLSC_BASE(ViPipe) + 0xc));
}

static __inline GK_VOID
ext_system_isp_radial_shading_blendratio_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT(VREG_RLSC_BASE(ViPipe) + 0xe, data);
}

static __inline GK_U16
ext_system_isp_radial_shading_blendratio_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_RLSC_BASE(ViPipe) + 0xe));
}

static __inline GK_VOID
ext_system_isp_radial_shading_lightinfo_write(VI_PIPE ViPipe, GK_U8 u8Index,
					      GK_U8 data)
{
	IOWR_8DIRECT((VREG_RLSC_BASE(ViPipe) + 0x10 + u8Index * sizeof(GK_U8)),
		     data);
}

static __inline GK_U8
ext_system_isp_radial_shading_lightinfo_read(VI_PIPE ViPipe, GK_U8 u8Index)
{
	return (IORD_8DIRECT(
		(VREG_RLSC_BASE(ViPipe) + 0x10 + u8Index * sizeof(GK_U8))));
}

static __inline GK_VOID
ext_system_isp_radial_shading_centerrx_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT(VREG_RLSC_BASE(ViPipe) + 0x12, data);
}

static __inline GK_U16
ext_system_isp_radial_shading_centerrx_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_RLSC_BASE(ViPipe) + 0x12));
}

static __inline GK_VOID
ext_system_isp_radial_shading_centerry_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT(VREG_RLSC_BASE(ViPipe) + 0x14, data);
}

static __inline GK_U16
ext_system_isp_radial_shading_centerry_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_RLSC_BASE(ViPipe) + 0x14));
}

static __inline GK_VOID
ext_system_isp_radial_shading_centergrx_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT(VREG_RLSC_BASE(ViPipe) + 0x16, data);
}

static __inline GK_U16
ext_system_isp_radial_shading_centergrx_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_RLSC_BASE(ViPipe) + 0x16));
}

static __inline GK_VOID
ext_system_isp_radial_shading_centergry_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT(VREG_RLSC_BASE(ViPipe) + 0x18, data);
}

static __inline GK_U16
ext_system_isp_radial_shading_centergry_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_RLSC_BASE(ViPipe) + 0x18));
}

static __inline GK_VOID
ext_system_isp_radial_shading_centergbx_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT(VREG_RLSC_BASE(ViPipe) + 0x1a, data);
}

static __inline GK_U16
ext_system_isp_radial_shading_centergbx_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_RLSC_BASE(ViPipe) + 0x1a));
}

static __inline GK_VOID
ext_system_isp_radial_shading_centergby_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT(VREG_RLSC_BASE(ViPipe) + 0x1c, data);
}

static __inline GK_U16
ext_system_isp_radial_shading_centergby_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_RLSC_BASE(ViPipe) + 0x1c));
}

static __inline GK_VOID
ext_system_isp_radial_shading_centerbx_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT(VREG_RLSC_BASE(ViPipe) + 0x1e, data);
}

static __inline GK_U16
ext_system_isp_radial_shading_centerbx_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_RLSC_BASE(ViPipe) + 0x1e));
}

static __inline GK_VOID
ext_system_isp_radial_shading_centerby_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT(VREG_RLSC_BASE(ViPipe) + 0x20, data);
}

static __inline GK_U16
ext_system_isp_radial_shading_centerby_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_RLSC_BASE(ViPipe) + 0x20));
}

static __inline GK_VOID
ext_system_isp_radial_shading_offcenterr_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT(VREG_RLSC_BASE(ViPipe) + 0x22, data);
}

static __inline GK_U16
ext_system_isp_radial_shading_offcenterr_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_RLSC_BASE(ViPipe) + 0x22));
}

static __inline GK_VOID
ext_system_isp_radial_shading_offcentergr_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT(VREG_RLSC_BASE(ViPipe) + 0x24, data);
}

static __inline GK_U16
ext_system_isp_radial_shading_offcentergr_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_RLSC_BASE(ViPipe) + 0x24));
}

static __inline GK_VOID
ext_system_isp_radial_shading_offcentergb_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT(VREG_RLSC_BASE(ViPipe) + 0x26, data);
}

static __inline GK_U16
ext_system_isp_radial_shading_offcentergb_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_RLSC_BASE(ViPipe) + 0x26));
}

static __inline GK_VOID
ext_system_isp_radial_shading_offcenterb_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT(VREG_RLSC_BASE(ViPipe) + 0x28, data);
}

static __inline GK_U16
ext_system_isp_radial_shading_offcenterb_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_RLSC_BASE(ViPipe) + 0x28));
}

static __inline GK_VOID
ext_system_isp_radial_shading_r_gain0_write(VI_PIPE ViPipe, GK_U8 u8Index,
					    GK_U16 data)
{
	IOWR_16DIRECT(
		(VREG_RLSC_BASE(ViPipe) + 0x02a + u8Index * sizeof(GK_U16)),
		data);
}

static __inline GK_U16
ext_system_isp_radial_shading_r_gain0_read(VI_PIPE ViPipe, GK_U8 u8Index)
{
	return (IORD_16DIRECT(
		(VREG_RLSC_BASE(ViPipe) + 0x02a + u8Index * sizeof(GK_U16))));
}

static __inline GK_VOID
ext_system_isp_radial_shading_r_gain1_write(VI_PIPE ViPipe, GK_U8 u8Index,
					    GK_U16 data)
{
	IOWR_16DIRECT(
		(VREG_RLSC_BASE(ViPipe) + 0x13a + u8Index * sizeof(GK_U16)),
		data);
}

static __inline GK_U16
ext_system_isp_radial_shading_r_gain1_read(VI_PIPE ViPipe, GK_U8 u8Index)
{
	return (IORD_16DIRECT(
		(VREG_RLSC_BASE(ViPipe) + 0x13a + u8Index * sizeof(GK_U16))));
}

static __inline GK_VOID
ext_system_isp_radial_shading_r_gain2_write(VI_PIPE ViPipe, GK_U8 u8Index,
					    GK_U16 data)
{
	IOWR_16DIRECT(
		(VREG_RLSC_BASE(ViPipe) + 0x24a + u8Index * sizeof(GK_U16)),
		data);
}

static __inline GK_U16
ext_system_isp_radial_shading_r_gain2_read(VI_PIPE ViPipe, GK_U8 u8Index)
{
	return (IORD_16DIRECT(
		(VREG_RLSC_BASE(ViPipe) + 0x24a + u8Index * sizeof(GK_U16))));
}

static __inline GK_VOID
ext_system_isp_radial_shading_gr_gain0_write(VI_PIPE ViPipe, GK_U8 u8Index,
					     GK_U16 data)
{
	IOWR_16DIRECT(
		(VREG_RLSC_BASE(ViPipe) + 0x35a + u8Index * sizeof(GK_U16)),
		data);
}

static __inline GK_U16
ext_system_isp_radial_shading_gr_gain0_read(VI_PIPE ViPipe, GK_U8 u8Index)
{
	return (IORD_16DIRECT(
		(VREG_RLSC_BASE(ViPipe) + 0x35a + u8Index * sizeof(GK_U16))));
}

static __inline GK_VOID
ext_system_isp_radial_shading_gr_gain1_write(VI_PIPE ViPipe, GK_U8 u8Index,
					     GK_U16 data)
{
	IOWR_16DIRECT(
		(VREG_RLSC_BASE(ViPipe) + 0x46a + u8Index * sizeof(GK_U16)),
		data);
}

static __inline GK_U16
ext_system_isp_radial_shading_gr_gain1_read(VI_PIPE ViPipe, GK_U8 u8Index)
{
	return (IORD_16DIRECT(
		(VREG_RLSC_BASE(ViPipe) + 0x46a + u8Index * sizeof(GK_U16))));
}

static __inline GK_VOID
ext_system_isp_radial_shading_gr_gain2_write(VI_PIPE ViPipe, GK_U8 u8Index,
					     GK_U16 data)
{
	IOWR_16DIRECT(
		(VREG_RLSC_BASE(ViPipe) + 0x57a + u8Index * sizeof(GK_U16)),
		data);
}

static __inline GK_U16
ext_system_isp_radial_shading_gr_gain2_read(VI_PIPE ViPipe, GK_U8 u8Index)
{
	return (IORD_16DIRECT(
		(VREG_RLSC_BASE(ViPipe) + 0x57a + u8Index * sizeof(GK_U16))));
}

static __inline GK_VOID
ext_system_isp_radial_shading_gb_gain0_write(VI_PIPE ViPipe, GK_U8 u8Index,
					     GK_U16 data)
{
	IOWR_16DIRECT(
		(VREG_RLSC_BASE(ViPipe) + 0x68a + u8Index * sizeof(GK_U16)),
		data);
}

static __inline GK_U16
ext_system_isp_radial_shading_gb_gain0_read(VI_PIPE ViPipe, GK_U8 u8Index)
{
	return (IORD_16DIRECT(
		(VREG_RLSC_BASE(ViPipe) + 0x68a + u8Index * sizeof(GK_U16))));
}

static __inline GK_VOID
ext_system_isp_radial_shading_gb_gain1_write(VI_PIPE ViPipe, GK_U8 u8Index,
					     GK_U16 data)
{
	IOWR_16DIRECT(
		(VREG_RLSC_BASE(ViPipe) + 0x79a + u8Index * sizeof(GK_U16)),
		data);
}

static __inline GK_U16
ext_system_isp_radial_shading_gb_gain1_read(VI_PIPE ViPipe, GK_U8 u8Index)
{
	return (IORD_16DIRECT(
		(VREG_RLSC_BASE(ViPipe) + 0x79a + u8Index * sizeof(GK_U16))));
}

static __inline GK_VOID
ext_system_isp_radial_shading_gb_gain2_write(VI_PIPE ViPipe, GK_U8 u8Index,
					     GK_U16 data)
{
	IOWR_16DIRECT(
		(VREG_RLSC_BASE(ViPipe) + 0x8aa + u8Index * sizeof(GK_U16)),
		data);
}

static __inline GK_U16
ext_system_isp_radial_shading_gb_gain2_read(VI_PIPE ViPipe, GK_U8 u8Index)
{
	return (IORD_16DIRECT(
		(VREG_RLSC_BASE(ViPipe) + 0x8aa + u8Index * sizeof(GK_U16))));
}

static __inline GK_VOID
ext_system_isp_radial_shading_b_gain0_write(VI_PIPE ViPipe, GK_U8 u8Index,
					    GK_U16 data)
{
	IOWR_16DIRECT(
		(VREG_RLSC_BASE(ViPipe) + 0x9ba + u8Index * sizeof(GK_U16)),
		data);
}

static __inline GK_U16
ext_system_isp_radial_shading_b_gain0_read(VI_PIPE ViPipe, GK_U8 u8Index)
{
	return (IORD_16DIRECT(
		(VREG_RLSC_BASE(ViPipe) + 0x9ba + u8Index * sizeof(GK_U16))));
}

static __inline GK_VOID
ext_system_isp_radial_shading_b_gain1_write(VI_PIPE ViPipe, GK_U8 u8Index,
					    GK_U16 data)
{
	IOWR_16DIRECT(
		(VREG_RLSC_BASE(ViPipe) + 0xaca + u8Index * sizeof(GK_U16)),
		data);
}

static __inline GK_U16
ext_system_isp_radial_shading_b_gain1_read(VI_PIPE ViPipe, GK_U8 u8Index)
{
	return (IORD_16DIRECT(
		(VREG_RLSC_BASE(ViPipe) + 0xaca + u8Index * sizeof(GK_U16))));
}

static __inline GK_VOID
ext_system_isp_radial_shading_b_gain2_write(VI_PIPE ViPipe, GK_U8 u8Index,
					    GK_U16 data)
{
	IOWR_16DIRECT(
		(VREG_RLSC_BASE(ViPipe) + 0xbda + u8Index * sizeof(GK_U16)),
		data);
}

static __inline GK_U16
ext_system_isp_radial_shading_b_gain2_read(VI_PIPE ViPipe, GK_U8 u8Index)
{
	return (IORD_16DIRECT(
		(VREG_RLSC_BASE(ViPipe) + 0xbda + u8Index * sizeof(GK_U16))));
}

// ==============================================================//
// YUV SHARPEN
// ==============================================================//
// sharpening core
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_SHARPEN_MANU_MODE_DEFAULT (0)
// args: data (8-bit)
static __inline GK_VOID ext_system_isp_sharpen_manu_mode_write(VI_PIPE ViPipe,
							       GK_U8 data)
{
	IOWR_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0x000, data & 0x01);
}
static __inline GK_U8 ext_system_isp_sharpen_manu_mode_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0x000) & 0x01);
}

#define ISP_EXT_SYSTEM_MANUAL_SHARPEN_EN_DEFAULT (1)
static __inline GK_VOID ext_system_manual_isp_sharpen_en_write(VI_PIPE ViPipe,
							       GK_U8 data)
{
	IOWR_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0x002, data & 0x01);
}
static __inline GK_U8 ext_system_manual_isp_sharpen_en_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0x002) & 0x01);
}

#define ISP_EXT_SYSTEM_SHARPEN_MPI_UPDATE_EN_DEFAULT (0)
static __inline GK_VOID ext_system_sharpen_mpi_update_en_write(VI_PIPE ViPipe,
							       GK_U8 data)
{
	IOWR_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0x003, data & 0x01);
}
static __inline GK_U8 ext_system_sharpen_mpi_update_en_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0x003) & 0x01);
}

#define ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_TEXTURESTR_DEFAULT (300)
static __inline GK_VOID
ext_system_manual_Isp_sharpen_TextureStr_write(VI_PIPE ViPipe, GK_U8 index,
					       GK_U16 data)
{
	IOWR_16DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0x004 +
			      (index * sizeof(GK_U16)),
		      data & 0xFFF);
}
static __inline GK_U16
ext_system_manual_Isp_sharpen_TextureStr_read(VI_PIPE ViPipe, GK_U8 index)
{
	return (IORD_16DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0x004 +
			      (index * sizeof(GK_U16))) &
		0xFFF);
}

#define ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGESTR_DEFAULT (400)
static __inline GK_VOID
ext_system_manual_Isp_sharpen_EdgeStr_write(VI_PIPE ViPipe, GK_U8 index,
					    GK_U16 data)
{
	IOWR_16DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0x044 +
			      (index * sizeof(GK_U16)),
		      data & 0xFFF);
}
static __inline GK_U16
ext_system_manual_Isp_sharpen_EdgeStr_read(VI_PIPE ViPipe, GK_U8 index)
{
	return (IORD_16DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0x044 +
			      (index * sizeof(GK_U16))) &
		0xFFF);
}

#define ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_TEXTUREFREQ_DEFAULT (128)
static __inline GK_VOID
ext_system_manual_Isp_sharpen_TextureFreq_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0x084, data & 0xFFF);
}
static __inline GK_U16
ext_system_manual_Isp_sharpen_TextureFreq_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0x084) & 0xFFF);
}

#define ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGEFREQ_DEFAULT (96)
static __inline GK_VOID
ext_system_manual_Isp_sharpen_EdgeFreq_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0x086, data & 0xFFF);
}
static __inline GK_U16
ext_system_manual_Isp_sharpen_EdgeFreq_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0x086) & 0xFFF);
}

#define ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_OVERSHOOT_DEFAULT (100)
static __inline GK_VOID
ext_system_manual_Isp_sharpen_OverShoot_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0x088, data & 0x7f);
}
static __inline GK_U8
ext_system_manual_Isp_sharpen_OverShoot_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0x088) & 0x7f);
}

#define ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_UNDERSHOOT_DEFAULT (127)
static __inline GK_VOID
ext_system_manual_Isp_sharpen_UnderShoot_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0x089, data & 0x7f);
}
static __inline GK_U8
ext_system_manual_Isp_sharpen_UnderShoot_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0x089) & 0x7f);
}

#define ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SHOOTSUPSTR_DEFAULT (60)
static __inline GK_VOID
ext_system_manual_Isp_sharpen_shootSupStr_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0x08a, data);
}
static __inline GK_U8
ext_system_manual_Isp_sharpen_shootSupStr_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0x08a));
}

#define ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_DETAILCTRL_DEFAULT (128)
static __inline GK_VOID
ext_system_manual_Isp_sharpen_detailctrl_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0x08b, data);
}
static __inline GK_U8
ext_system_manual_Isp_sharpen_detailctrl_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0x08b));
}

#define ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGEFILTSTR_DEFAULT (53)
static __inline GK_VOID
ext_system_manual_Isp_sharpen_EdgeFiltStr_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0x08c, data & 0x3f);
}
static __inline GK_U8
ext_system_manual_Isp_sharpen_EdgeFiltStr_read(VI_PIPE ViPipe)
{
	return ((IORD_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0x08c)) & 0x3f);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_manual_sharpen_LumaWgt
// ------------------------------------------------------------------------------ //
// sharpening manual LumaWgt table length:32*1 BYTE
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
// ******************************32 multi addrs*********************************//
#define ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_LUMAWGT_DEFAULT (ISP_SHARPEN_LUMAWGT)
static __inline GK_VOID
ext_system_manual_Isp_sharpen_LumaWgt_write(VI_PIPE ViPipe, GK_U8 index,
					    GK_U8 data)
{
	IOWR_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0x08e +
			     (index * sizeof(GK_U8)),
		     data & ISP_SHARPEN_LUMAWGT_BIT);
}
static __inline GK_U8 ext_system_manual_Isp_sharpen_LumaWgt_read(VI_PIPE ViPipe,
								 GK_U8 index)
{
	return (IORD_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0x08e +
			     (index * sizeof(GK_U8))) &
		ISP_SHARPEN_LUMAWGT_BIT);
}

// RGain: data (8-bit)
#define ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_RGAIN_DEFAULT (ISP_SHARPEN_RGAIN)
static __inline GK_VOID
ext_system_manual_Isp_sharpen_RGain_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0xae,
		     data & ISP_SHARPEN_RGAIN_BIT);
}
static __inline GK_U8 ext_system_manual_Isp_sharpen_RGain_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0xae) &
		ISP_SHARPEN_RGAIN_BIT);
}

// BGain: data (8-bit)
#define ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_BGAIN_DEFAULT (ISP_SHARPEN_BGAIN)
static __inline GK_VOID
ext_system_manual_Isp_sharpen_BGain_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0xaf,
		     data & ISP_SHARPEN_BGAIN_BIT);
}
static __inline GK_U8 ext_system_manual_Isp_sharpen_BGain_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0xaf) &
		ISP_SHARPEN_BGAIN_BIT);
}

// SkinGain: data (8-bit)
#define ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SKINGAIN_DEFAULT (31)
static __inline GK_VOID
ext_system_manual_Isp_sharpen_SkinGain_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0xb0, data & 0x1f);
}
static __inline GK_U8
ext_system_manual_Isp_sharpen_SkinGain_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0xb0) & 0x1f);
}

// EdgeFiltMaxCap: data (8-bit)
#define ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_EDGEFILTMAXCAP_DEFAULT \
	(ISP_SHARPEN_EdgeFiltMaxCap)
//***********************************************************************//
static __inline GK_VOID
ext_system_manual_Isp_sharpen_EdgeFiltMaxCap_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0xb1,
		     data & ISP_SHARPEN_EdgeFiltMaxCap_BIT);
}
static __inline GK_U8
ext_system_manual_Isp_sharpen_EdgeFiltMaxCap_read(VI_PIPE ViPipe)
{
	return ((IORD_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0xb1)) &
		ISP_SHARPEN_EdgeFiltMaxCap_BIT);
}

// ------Auto 16-------------------//
// ******************************auoto 32 x 16 multi addrs*********************************//
static __inline GK_VOID ext_system_Isp_sharpen_TextureStr_write(VI_PIPE ViPipe,
								GK_U16 u16Index,
								GK_U16 data)
{
	IOWR_16DIRECT(
		(VREG_SHARPEN_BASE(ViPipe) + 0xb2 + u16Index * sizeof(GK_U16)),
		data & 0xFFF);
}
static __inline GK_U16 ext_system_Isp_sharpen_TextureStr_read(VI_PIPE ViPipe,
							      GK_U16 u16Index)
{
	return ((IORD_16DIRECT((VREG_SHARPEN_BASE(ViPipe) + 0xb2 +
				u16Index * sizeof(GK_U16)))) &
		0xFFF);
}

// ******************************auoto 32 x 16 multi addrs*********************************//
static __inline GK_VOID ext_system_Isp_sharpen_EdgeStr_write(VI_PIPE ViPipe,
							     GK_U16 u16Index,
							     GK_U16 data)
{
	IOWR_16DIRECT(
		(VREG_SHARPEN_BASE(ViPipe) + 0x4b2 + u16Index * sizeof(GK_U16)),
		data & 0xFFF);
}
static __inline GK_U16 ext_system_Isp_sharpen_EdgeStr_read(VI_PIPE ViPipe,
							   GK_U16 u16Index)
{
	return ((IORD_16DIRECT((VREG_SHARPEN_BASE(ViPipe) + 0x4b2 +
				u16Index * sizeof(GK_U16)))) &
		0xFFF);
}

// ******************************auoto 16 multi addrs*********************************//
static __inline GK_VOID ext_system_Isp_sharpen_TextureFreq_write(VI_PIPE ViPipe,
								 GK_U8 u8Index,
								 GK_U16 data)
{
	IOWR_16DIRECT(
		(VREG_SHARPEN_BASE(ViPipe) + 0x8b2 + u8Index * sizeof(GK_U16)),
		data & 0xFFF);
}
static __inline GK_U16 ext_system_Isp_sharpen_TextureFreq_read(VI_PIPE ViPipe,
							       GK_U8 u8Index)
{
	return ((IORD_16DIRECT((VREG_SHARPEN_BASE(ViPipe) + 0x8b2 +
				u8Index * sizeof(GK_U16)))) &
		0xFFF);
}

// ******************************auoto 16 multi addrs*********************************//
static __inline GK_VOID ext_system_Isp_sharpen_EdgeFreq_write(VI_PIPE ViPipe,
							      GK_U8 u8Index,
							      GK_U16 data)
{
	IOWR_16DIRECT(
		(VREG_SHARPEN_BASE(ViPipe) + 0x8d2 + u8Index * sizeof(GK_U16)),
		data & 0xFFF);
}
static __inline GK_U16 ext_system_Isp_sharpen_EdgeFreq_read(VI_PIPE ViPipe,
							    GK_U8 u8Index)
{
	return ((IORD_16DIRECT((VREG_SHARPEN_BASE(ViPipe) + 0x8d2 +
				u8Index * sizeof(GK_U16)))) &
		0xFFF);
}

// ******************************auoto 16 multi addrs*********************************//
static __inline GK_VOID ext_system_Isp_sharpen_OverShoot_write(VI_PIPE ViPipe,
							       GK_U8 u8Index,
							       GK_U8 data)
{
	IOWR_8DIRECT(
		(VREG_SHARPEN_BASE(ViPipe) + 0x8f2 + u8Index * sizeof(GK_U8)),
		data & 0x7f);
}
static __inline GK_U8 ext_system_Isp_sharpen_OverShoot_read(VI_PIPE ViPipe,
							    GK_U8 u8Index)
{
	return ((IORD_8DIRECT((VREG_SHARPEN_BASE(ViPipe) + 0x8f2 +
			       u8Index * sizeof(GK_U8)))) &
		0x7f);
}

// ******************************auoto 16 multi addrs*********************************//
static __inline GK_VOID ext_system_Isp_sharpen_UnderShoot_write(VI_PIPE ViPipe,
								GK_U8 u8Index,
								GK_U8 data)
{
	IOWR_8DIRECT(
		(VREG_SHARPEN_BASE(ViPipe) + 0x902 + u8Index * sizeof(GK_U8)),
		data & 0x7f);
}
static __inline GK_U8 ext_system_Isp_sharpen_UnderShoot_read(VI_PIPE ViPipe,
							     GK_U8 u8Index)
{
	return ((IORD_8DIRECT((VREG_SHARPEN_BASE(ViPipe) + 0x902 +
			       u8Index * sizeof(GK_U8)))) &
		0x7f);
}

// ******************************auoto 16 multi addrs*********************************//
static __inline GK_VOID ext_system_Isp_sharpen_shootSupStr_write(VI_PIPE ViPipe,
								 GK_U8 u8Index,
								 GK_U8 data)
{
	IOWR_8DIRECT(
		(VREG_SHARPEN_BASE(ViPipe) + 0x912 + u8Index * sizeof(GK_U8)),
		data);
}
static __inline GK_U8 ext_system_Isp_sharpen_shootSupStr_read(VI_PIPE ViPipe,
							      GK_U8 u8Index)
{
	return ((IORD_8DIRECT((VREG_SHARPEN_BASE(ViPipe) + 0x912 +
			       u8Index * sizeof(GK_U8)))));
}

// ******************************auoto 16 multi addrs*********************************//
static __inline GK_VOID ext_system_Isp_sharpen_detailctrl_write(VI_PIPE ViPipe,
								GK_U8 u8Index,
								GK_U8 data)
{
	IOWR_8DIRECT(
		(VREG_SHARPEN_BASE(ViPipe) + 0x922 + u8Index * sizeof(GK_U8)),
		data);
}
static __inline GK_U8 ext_system_Isp_sharpen_detailctrl_read(VI_PIPE ViPipe,
							     GK_U8 u8Index)
{
	return ((IORD_8DIRECT((VREG_SHARPEN_BASE(ViPipe) + 0x922 +
			       u8Index * sizeof(GK_U8)))));
}

// ******************************auoto 16 multi addrs*********************************//
static __inline GK_VOID
ext_system_Isp_sharpen_RGain_write(VI_PIPE ViPipe, GK_U8 u8Index, GK_U8 data)
{
	IOWR_8DIRECT(
		(VREG_SHARPEN_BASE(ViPipe) + 0x932 + u8Index * sizeof(GK_U8)),
		data & ISP_SHARPEN_RGAIN_BIT);
}
static __inline GK_U8 ext_system_Isp_sharpen_RGain_read(VI_PIPE ViPipe,
							GK_U8 u8Index)
{
	return ((IORD_8DIRECT((VREG_SHARPEN_BASE(ViPipe) + 0x932 +
			       u8Index * sizeof(GK_U8)))) &
		ISP_SHARPEN_RGAIN_BIT);
}

// ******************************auoto 16 multi addrs*********************************//
static __inline GK_VOID
ext_system_Isp_sharpen_BGain_write(VI_PIPE ViPipe, GK_U8 u8Index, GK_U8 data)
{
	IOWR_8DIRECT(
		(VREG_SHARPEN_BASE(ViPipe) + 0x942 + u8Index * sizeof(GK_U8)),
		data & ISP_SHARPEN_BGAIN_BIT);
}
static __inline GK_U8 ext_system_Isp_sharpen_BGain_read(VI_PIPE ViPipe,
							GK_U8 u8Index)
{
	return ((IORD_8DIRECT((VREG_SHARPEN_BASE(ViPipe) + 0x942 +
			       u8Index * sizeof(GK_U8)))) &
		ISP_SHARPEN_BGAIN_BIT);
}

// ******************************auoto 16 multi addrs*********************************//
static __inline GK_VOID
ext_system_Isp_sharpen_SkinGain_write(VI_PIPE ViPipe, GK_U8 u8Index, GK_U8 data)
{
	IOWR_8DIRECT(
		(VREG_SHARPEN_BASE(ViPipe) + 0x952 + u8Index * sizeof(GK_U8)),
		data & 0x1f);
}
static __inline GK_U8 ext_system_Isp_sharpen_SkinGain_read(VI_PIPE ViPipe,
							   GK_U8 u8Index)
{
	return ((IORD_8DIRECT((VREG_SHARPEN_BASE(ViPipe) + 0x952 +
			       u8Index * sizeof(GK_U8)))) &
		0x1f);
}

// ******************************auoto 16 multi addrs*********************************//
static __inline GK_VOID ext_system_Isp_sharpen_EdgeFiltStr_write(VI_PIPE ViPipe,
								 GK_U8 u8Index,
								 GK_U8 data)
{
	IOWR_8DIRECT(
		(VREG_SHARPEN_BASE(ViPipe) + 0x962 + u8Index * sizeof(GK_U8)),
		data & 0x3f);
}
static __inline GK_U8 ext_system_Isp_sharpen_EdgeFiltStr_read(VI_PIPE ViPipe,
							      GK_U8 u8Index)
{
	return ((IORD_8DIRECT((VREG_SHARPEN_BASE(ViPipe) + 0x962 +
			       u8Index * sizeof(GK_U8)))) &
		0x3f);
}

// ******************************auoto 32 x 16 multi addrs*********************************//
static __inline GK_VOID ext_system_Isp_sharpen_LumaWgt_write(VI_PIPE ViPipe,
							     GK_U16 u16Index,
							     GK_U8 data)
{
	IOWR_8DIRECT(
		(VREG_SHARPEN_BASE(ViPipe) + 0x972 + u16Index * sizeof(GK_U8)),
		data & ISP_SHARPEN_LUMAWGT_BIT);
}
static __inline GK_U16 ext_system_Isp_sharpen_LumaWgt_read(VI_PIPE ViPipe,
							   GK_U16 u16Index)
{
	return ((IORD_8DIRECT((VREG_SHARPEN_BASE(ViPipe) + 0x972 +
			       u16Index * sizeof(GK_U8)))) &
		ISP_SHARPEN_LUMAWGT_BIT);
}

// new add manual
#define ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_DETAILCTRLTHR_DEFAULT (160)

static __inline GK_VOID
ext_system_manual_Isp_sharpen_detailctrlThr_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0xB80, data);
}
static __inline GK_U8
ext_system_manual_Isp_sharpen_detailctrlThr_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0xB80));
}

#define ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SHOOTSUPADJ_DEFAULT (9)

static __inline GK_VOID
ext_system_manual_Isp_sharpen_ShootSupAdj_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0xB81, data & 0x0f);
}
static __inline GK_U8
ext_system_manual_Isp_sharpen_ShootSupAdj_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0xB81) & 0x0f);
}

#define ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_MAXSHARPGAIN_DEFAULT (160)

static __inline GK_VOID
ext_system_manual_Isp_sharpen_MaxSharpGain_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0xB82, data & 0x7FF);
}
static __inline GK_U16
ext_system_manual_Isp_sharpen_MaxSharpGain_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0xB82) & 0x7FF);
}

#define ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SKINUMIN_DEFAULT (100)

static __inline GK_VOID
ext_system_manual_Isp_sharpen_SkinUmin_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0xB86, data);
}
static __inline GK_U8
ext_system_manual_Isp_sharpen_SkinUmin_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0xB86));
}

#define ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SKINVMIN_DEFAULT (135)

static __inline GK_VOID
ext_system_manual_Isp_sharpen_SkinVmin_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0xB87, data);
}
static __inline GK_U8
ext_system_manual_Isp_sharpen_SkinVmin_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0xB87));
}

#define ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SKINUMAX_DEFAULT (127)

static __inline GK_VOID
ext_system_manual_Isp_sharpen_SkinUmax_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0xB88, data);
}
static __inline GK_U8
ext_system_manual_Isp_sharpen_SkinUmax_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0xB88));
}

#define ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_SKINVMAX_DEFAULT (160)

static __inline GK_VOID
ext_system_manual_Isp_sharpen_SkinVmax_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0xB89, data);
}
static __inline GK_U8
ext_system_manual_Isp_sharpen_SkinVmax_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0xB89));
}
// GGain: data (8-bit)
#define ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_GGAIN_DEFAULT (ISP_SHARPEN_GGAIN)

static __inline GK_VOID
ext_system_manual_Isp_sharpen_GGain_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0xB8A,
		     data & ISP_SHARPEN_GGAIN_BIT);
}
static __inline GK_U8 ext_system_manual_Isp_sharpen_GGain_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0xB8A) &
		ISP_SHARPEN_GGAIN_BIT);
}
#define ISP_EXT_SYSTEM_MANUAL_ISP_SHARPEN_WEAKDETAILGAIN_DEFAULT (0)
static __inline GK_VOID
ext_system_manual_Isp_sharpen_WeakDetailGain_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0xB8b, data & 0x7F);
}
static __inline GK_U8
ext_system_manual_Isp_sharpen_WeakDetailGain_read(VI_PIPE ViPipe)
{
	return ((IORD_8DIRECT(VREG_SHARPEN_BASE(ViPipe) + 0xB8b)) & 0x7F);
}
// new add auto
static __inline GK_VOID
ext_system_Isp_sharpen_detailctrlThr_write(VI_PIPE ViPipe, GK_U8 u8Index,
					   GK_U8 data)
{
	IOWR_8DIRECT(
		(VREG_SHARPEN_BASE(ViPipe) + 0xB8C + u8Index * sizeof(GK_U8)),
		data);
}
static __inline GK_U8 ext_system_Isp_sharpen_detailctrlThr_read(VI_PIPE ViPipe,
								GK_U8 u8Index)
{
	return ((IORD_8DIRECT((VREG_SHARPEN_BASE(ViPipe) + 0xB8C +
			       u8Index * sizeof(GK_U8)))));
}

static __inline GK_VOID
ext_system_Isp_sharpen_MaxSharpGain_write(VI_PIPE ViPipe, GK_U8 u8Index,
					  GK_U16 data)
{
	IOWR_16DIRECT(
		(VREG_SHARPEN_BASE(ViPipe) + 0xBaC + u8Index * sizeof(GK_U16)),
		data & 0x7ff);
}
static __inline GK_U16 ext_system_Isp_sharpen_MaxSharpGain_read(VI_PIPE ViPipe,
								GK_U8 u8Index)
{
	return ((IORD_16DIRECT((VREG_SHARPEN_BASE(ViPipe) + 0xBaC +
				u8Index * sizeof(GK_U16)))) &
		0x7ff);
}

static __inline GK_VOID ext_system_Isp_sharpen_ShootSupAdj_write(VI_PIPE ViPipe,
								 GK_U8 u8Index,
								 GK_U8 data)
{
	IOWR_8DIRECT(
		(VREG_SHARPEN_BASE(ViPipe) + 0xBCC + u8Index * sizeof(GK_U8)),
		data & 0x0f);
}
static __inline GK_U8 ext_system_Isp_sharpen_ShootSupAdj_read(VI_PIPE ViPipe,
							      GK_U8 u8Index)
{
	return ((IORD_8DIRECT((VREG_SHARPEN_BASE(ViPipe) + 0xBCC +
			       u8Index * sizeof(GK_U8)))) &
		0x0f);
}

static __inline GK_VOID
ext_system_Isp_sharpen_WeakDetailGain_write(VI_PIPE ViPipe, GK_U8 u8Index,
					    GK_U8 data)
{
	IOWR_8DIRECT(
		(VREG_SHARPEN_BASE(ViPipe) + 0xBDC + u8Index * sizeof(GK_U8)),
		data & 0x7f);
}
static __inline GK_U8 ext_system_Isp_sharpen_WeakDetailGain_read(VI_PIPE ViPipe,
								 GK_U8 u8Index)
{
	return ((IORD_8DIRECT((VREG_SHARPEN_BASE(ViPipe) + 0xBDC +
			       u8Index * sizeof(GK_U8)))) &
		0x7f);
}
static __inline GK_VOID
ext_system_Isp_sharpen_GGain_write(VI_PIPE ViPipe, GK_U8 u8Index, GK_U8 data)
{
	IOWR_8DIRECT(
		(VREG_SHARPEN_BASE(ViPipe) + 0xBEC + u8Index * sizeof(GK_U8)),
		data & ISP_SHARPEN_GGAIN_BIT);
}
static __inline GK_U8 ext_system_Isp_sharpen_GGain_read(VI_PIPE ViPipe,
							GK_U8 u8Index)
{
	return ((IORD_8DIRECT((VREG_SHARPEN_BASE(ViPipe) + 0xBEC +
			       u8Index * sizeof(GK_U8)))) &
		ISP_SHARPEN_GGAIN_BIT);
}

//******************************auoto 16 multi addrs*********************************//
static __inline GK_VOID
ext_system_Isp_sharpen_EdgeFiltMaxCap_write(VI_PIPE ViPipe, GK_U8 u8Index,
					    GK_U8 data)
{
	IOWR_8DIRECT(
		(VREG_SHARPEN_BASE(ViPipe) + 0xBFC + u8Index * sizeof(GK_U8)),
		data & ISP_SHARPEN_EdgeFiltMaxCap_BIT);
}
static __inline GK_U8 ext_system_Isp_sharpen_EdgeFiltMaxCap_read(VI_PIPE ViPipe,
								 GK_U8 u8Index)
{
	return ((IORD_8DIRECT((VREG_SHARPEN_BASE(ViPipe) + 0xBFC +
			       u8Index * sizeof(GK_U8)))) &
		ISP_SHARPEN_EdgeFiltMaxCap_BIT);
}

// EdgeMark
#define ISP_EXT_SYSTEM_MANUAL_EDGEMARK_EN_DEFAULT (0)

static __inline GK_VOID ext_system_manual_isp_edgemark_en_write(VI_PIPE ViPipe,
								GK_U8 data)
{
	IOWR_8DIRECT(VREG_EDGEMARK_BASE(ViPipe), data & 0x01);
}
static __inline GK_U8 ext_system_manual_isp_edgemark_en_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_EDGEMARK_BASE(ViPipe)) & 0x01);
}

#define ISP_EXT_SYSTEM_MANUAL_EDGEMARK_THD_DEFAULT (160)

static __inline GK_VOID
ext_system_manual_isp_edgemark_threshold_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_EDGEMARK_BASE(ViPipe) + 0x1, data);
}
static __inline GK_U8
ext_system_manual_isp_edgemark_threshold_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_EDGEMARK_BASE(ViPipe) + 0x1));
}

#define ISP_EXT_SYSTEM_EDGEMARK_MPI_UPDATE_EN_DEFAULT (0)

static __inline GK_VOID ext_system_edgemark_mpi_update_en_write(VI_PIPE ViPipe,
								GK_U8 data)
{
	IOWR_8DIRECT(VREG_EDGEMARK_BASE(ViPipe) + 0x2, data & 0x01);
}
static __inline GK_U8 ext_system_edgemark_mpi_update_en_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_EDGEMARK_BASE(ViPipe) + 0x2) & 0x01);
}

#define ISP_EXT_SYSTEM_MANUAL_EDGEMARK_COLOR_DEFAULT (0xFF0000)

static __inline GK_VOID
ext_system_manual_isp_edgemark_color_write(VI_PIPE ViPipe, GK_U32 data)
{
	IOWR_32DIRECT(VREG_EDGEMARK_BASE(ViPipe) + 0x4, data & 0xFFFFFF);
}
static __inline GK_U32 ext_system_manual_isp_edgemark_color_read(VI_PIPE ViPipe)
{
	return (IORD_32DIRECT(VREG_EDGEMARK_BASE(ViPipe) + 0x4) & 0xFFFFFF);
}

// High Light Constraint
#define ISP_EXT_SYSTEM_MANUAL_HLC_EN_DEFAULT (0)

static __inline GK_VOID ext_system_manual_isp_hlc_en_write(VI_PIPE ViPipe,
							   GK_U8 data)
{
	IOWR_8DIRECT(VREG_HLC_BASE(ViPipe), data & 0x01);
}
static __inline GK_U8 ext_system_manual_isp_hlc_en_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_HLC_BASE(ViPipe)) & 0x01);
}

#define ISP_EXT_SYSTEM_MANUAL_HLC_LUMATHR_DEFAULT (248)

static __inline GK_VOID ext_system_manual_isp_hlc_lumathr_write(VI_PIPE ViPipe,
								GK_U8 data)
{
	IOWR_8DIRECT(VREG_HLC_BASE(ViPipe) + 0x1, data);
}
static __inline GK_U8 ext_system_manual_isp_hlc_lumathr_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_HLC_BASE(ViPipe) + 0x1));
}

#define ISP_EXT_SYSTEM_MANUAL_HLC_LUMATARGET_DEFAULT (10)

static __inline GK_VOID
ext_system_manual_isp_hlc_lumatarget_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_HLC_BASE(ViPipe) + 0x2, data);
}
static __inline GK_U8 ext_system_manual_isp_hlc_lumatarget_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_HLC_BASE(ViPipe) + 0x2));
}

#define ISP_EXT_SYSTEM_HLC_MPI_UPDATE_EN_DEFAULT (0)

static __inline GK_VOID ext_system_hlc_mpi_update_en_write(VI_PIPE ViPipe,
							   GK_U8 data)
{
	IOWR_8DIRECT(VREG_HLC_BASE(ViPipe) + 0x3, data & 0x01);
}
static __inline GK_U8 ext_system_hlc_mpi_update_en_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_HLC_BASE(ViPipe) + 0x3) & 0x01);
}

// ==============================================================//
// CAC
// ==============================================================//
// ------------------------------------------------------------------------------ //
// Register: ext_system_localCAC_enable
// ------------------------------------------------------------------------------ //
//  localCAC_enable
static __inline GK_VOID ext_system_localCAC_enable_write(VI_PIPE ViPipe,
							 GK_U8 data)
{
	IOWR_8DIRECT(VREG_LCAC_BASE(ViPipe), data & 0x1);
}

static __inline GK_U16 ext_system_localCAC_enable_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_LCAC_BASE(ViPipe)) & 0x1);
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_localCAC_manual_mode_enable
// ------------------------------------------------------------------------------ //
// localCAC_manual_mode_enable
static __inline GK_VOID
ext_system_localCAC_manual_mode_enable_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_LCAC_BASE(ViPipe) + 1, data & 0x1);
}

static __inline GK_U8
ext_system_localCAC_manual_mode_enable_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_LCAC_BASE(ViPipe) + 1) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_localCAC_purple_det_range
// ------------------------------------------------------------------------------ //
//  localCAC_purple_det_range
static __inline GK_VOID
ext_system_localCAC_purple_det_range_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT(VREG_LCAC_BASE(ViPipe) + 2, data);
}

static __inline GK_U16 ext_system_localCAC_purple_det_range_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_LCAC_BASE(ViPipe) + 2);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_localCAC_purple_var_thr
// ------------------------------------------------------------------------------ //
// localCAC_purple_var_thr
static __inline GK_VOID ext_system_localCAC_purple_var_thr_write(VI_PIPE ViPipe,
								 GK_U16 data)
{
	IOWR_16DIRECT(VREG_LCAC_BASE(ViPipe) + 4, data);
}

static __inline GK_U16 ext_system_localCAC_purple_var_thr_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_LCAC_BASE(ViPipe) + 4);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_localCAC_manual_cb_str
// ------------------------------------------------------------------------------ //
//  localCAC_manual_cb
static __inline GK_VOID ext_system_localCAC_manual_cb_str_write(VI_PIPE ViPipe,
								GK_U8 data)
{
	IOWR_8DIRECT(VREG_LCAC_BASE(ViPipe) + 6, data);
}

static __inline GK_U8 ext_system_localCAC_manual_cb_str_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_LCAC_BASE(ViPipe) + 6);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_localCAC_manual_cr_str
// ------------------------------------------------------------------------------ //
//  localCAC_manual_cr
static __inline GK_VOID ext_system_localCAC_manual_cr_str_write(VI_PIPE ViPipe,
								GK_U8 data)
{
	IOWR_8DIRECT(VREG_LCAC_BASE(ViPipe) + 7, data);
}

static __inline GK_U8 ext_system_localCAC_manual_cr_str_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_LCAC_BASE(ViPipe) + 7);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_localCAC_Cb_Str_table
// ------------------------------------------------------------------------------ //
//  localCAC_Cb_Str_table
static __inline GK_VOID
ext_system_localCAC_auto_cb_str_table_write(VI_PIPE ViPipe, GK_U8 u8Index,
					    GK_U8 data)
{
	IOWR_8DIRECT(VREG_LCAC_BASE(ViPipe) + 8 + (u8Index * sizeof(GK_U8)),
		     data);
}

static __inline GK_U8 ext_system_localCAC_auto_cb_str_table_read(VI_PIPE ViPipe,
								 GK_U8 u8Index)
{
	return IORD_8DIRECT(VREG_LCAC_BASE(ViPipe) + 8 +
			    (u8Index * sizeof(GK_U8)));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_localCAC_Cr_Str_table
// ------------------------------------------------------------------------------ //
//  localCAC_Cr_Str_table
static __inline GK_VOID
ext_system_localCAC_auto_cr_str_table_write(VI_PIPE ViPipe, GK_U8 u8Index,
					    GK_U8 data)
{
	IOWR_8DIRECT(VREG_LCAC_BASE(ViPipe) + 0x18 + (u8Index * sizeof(GK_U8)),
		     data);
}

static __inline GK_U8 ext_system_localCAC_auto_cr_str_table_read(VI_PIPE ViPipe,
								 GK_U8 u8Index)
{
	return IORD_8DIRECT(VREG_LCAC_BASE(ViPipe) + 0x18 +
			    (u8Index * sizeof(GK_U8)));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_LocalCAC_coef_update_en
// ------------------------------------------------------------------------------ //
//  LocalCAC_coef_update_en
static __inline GK_VOID ext_system_LocalCAC_coef_update_en_write(VI_PIPE ViPipe,
								 GK_U8 data)
{
	IOWR_8DIRECT(VREG_LCAC_BASE(ViPipe) + 0x28, data & 0x1);
}

static __inline GK_U8 ext_system_LocalCAC_coef_update_en_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_LCAC_BASE(ViPipe) + 0x28) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_localCAC_lumagh_cnt_thr
// ------------------------------------------------------------------------------ //
static __inline GK_VOID ext_system_localCAC_lumagh_cnt_thr_write(VI_PIPE ViPipe,
								 GK_U8 data)
{
	IOWR_8DIRECT(VREG_LCAC_BASE(ViPipe) + 0x29, data);
}

static __inline GK_U8 ext_system_localCAC_lumagh_cnt_thr_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_LCAC_BASE(ViPipe) + 0x29);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_localCAC_cb_cntgh_thr
// ------------------------------------------------------------------------------ //
static __inline GK_VOID ext_system_localCAC_cb_cntgh_thr_write(VI_PIPE ViPipe,
							       GK_U8 data)
{
	IOWR_8DIRECT(VREG_LCAC_BASE(ViPipe) + 0x2a, data);
}

static __inline GK_U8 ext_system_localCAC_cb_cntgh_thr_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_LCAC_BASE(ViPipe) + 0x2a);
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_localCAC_cb_cnt_low_thr
// ------------------------------------------------------------------------------ //
static __inline GK_VOID ext_system_localCAC_cb_cnt_low_thr_write(VI_PIPE ViPipe,
								 GK_U8 data)
{
	IOWR_8DIRECT(VREG_LCAC_BASE(ViPipe) + 0x2b, data);
}

static __inline GK_U8 ext_system_localCAC_cb_cnt_low_thr_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_LCAC_BASE(ViPipe) + 0x2b);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_localCAC_luma_thr
// ------------------------------------------------------------------------------ //
static __inline GK_VOID ext_system_localCAC_luma_thr_write(VI_PIPE ViPipe,
							   GK_U16 data)
{
	IOWR_16DIRECT(VREG_LCAC_BASE(ViPipe) + 0x2c, data);
}

static __inline GK_U16 ext_system_localCAC_luma_thr_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_LCAC_BASE(ViPipe) + 0x2c);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_localCAC_cb_thr
// ------------------------------------------------------------------------------ //
static __inline GK_VOID ext_system_localCAC_cb_thr_write(VI_PIPE ViPipe,
							 GK_U16 data)
{
	IOWR_16DIRECT(VREG_LCAC_BASE(ViPipe) + 0x2e, data);
}

static __inline GK_U16 ext_system_localCAC_cb_thr_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_LCAC_BASE(ViPipe) + 0x2e);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_localCAC_bld_avg_cur
// ------------------------------------------------------------------------------ //
static __inline GK_VOID ext_system_localCAC_bld_avg_cur_write(VI_PIPE ViPipe,
							      GK_U8 data)
{
	IOWR_8DIRECT(VREG_LCAC_BASE(ViPipe) + 0x30, data);
}

static __inline GK_U8 ext_system_localCAC_bld_avg_cur_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_LCAC_BASE(ViPipe) + 0x30);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_localCAC_defcolor_cr
// ------------------------------------------------------------------------------ //
static __inline GK_VOID ext_system_localCAC_defcolor_cr_write(VI_PIPE ViPipe,
							      GK_U8 data)
{
	IOWR_8DIRECT(VREG_LCAC_BASE(ViPipe) + 0x31, data);
}

static __inline GK_U8 ext_system_localCAC_defcolor_cr_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_LCAC_BASE(ViPipe) + 0x31);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_localCAC_defcolor_cb
// ------------------------------------------------------------------------------ //
static __inline GK_VOID ext_system_localCAC_defcolor_cb_write(VI_PIPE ViPipe,
							      GK_U8 data)
{
	IOWR_8DIRECT(VREG_LCAC_BASE(ViPipe) + 0x32, data);
}

static __inline GK_U8 ext_system_localCAC_defcolor_cb_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_LCAC_BASE(ViPipe) + 0x32);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_localCAC_r_thd_table
// ------------------------------------------------------------------------------ //
static __inline GK_VOID
ext_system_localCAC_r_thd_table_write(VI_PIPE ViPipe, GK_U8 i, GK_U16 data)
{
	IOWR_16DIRECT(VREG_LCAC_BASE(ViPipe) + 0x34 + (i * sizeof(GK_U16)),
		      data);
}

static __inline GK_U16 ext_system_localCAC_r_thd_table_read(VI_PIPE ViPipe,
							    GK_U8 i)
{
	return IORD_16DIRECT(VREG_LCAC_BASE(ViPipe) + 0x34 +
			     (i * sizeof(GK_U16)));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_localCAC_g_thd_table
// ------------------------------------------------------------------------------ //
static __inline GK_VOID
ext_system_localCAC_g_thd_table_write(VI_PIPE ViPipe, GK_U8 i, GK_U16 data)
{
	IOWR_16DIRECT(VREG_LCAC_BASE(ViPipe) + 0x3a + (i * sizeof(GK_U16)),
		      data);
}

static __inline GK_U16 ext_system_localCAC_g_thd_table_read(VI_PIPE ViPipe,
							    GK_U8 i)
{
	return IORD_16DIRECT(VREG_LCAC_BASE(ViPipe) + 0x3a +
			     (i * sizeof(GK_U16)));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_localCAC_b_thd_table
// ------------------------------------------------------------------------------ //
static __inline GK_VOID
ext_system_localCAC_b_thd_table_write(VI_PIPE ViPipe, GK_U8 i, GK_U16 data)
{
	IOWR_16DIRECT(VREG_LCAC_BASE(ViPipe) + 0x40 + (i * sizeof(GK_U16)),
		      data);
}

static __inline GK_U16 ext_system_localCAC_b_thd_table_read(VI_PIPE ViPipe,
							    GK_U8 i)
{
	return IORD_16DIRECT(VREG_LCAC_BASE(ViPipe) + 0x40 +
			     (i * sizeof(GK_U16)));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_localCAC_luma_thd_table
// ------------------------------------------------------------------------------ //
static __inline GK_VOID
ext_system_localCAC_luma_thd_table_write(VI_PIPE ViPipe, GK_U8 i, GK_U16 data)
{
	IOWR_16DIRECT(VREG_LCAC_BASE(ViPipe) + 0x46 + (i * sizeof(GK_U16)),
		      data);
}

static __inline GK_U16 ext_system_localCAC_luma_thd_table_read(VI_PIPE ViPipe,
							       GK_U8 i)
{
	return IORD_16DIRECT(VREG_LCAC_BASE(ViPipe) + 0x46 +
			     (i * sizeof(GK_U16)));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_localCAC_luma_thd_table
// ------------------------------------------------------------------------------ //
static __inline GK_VOID
ext_system_localCAC_cbcr_ratio_table_write(VI_PIPE ViPipe, GK_U8 i, GK_S16 data)
{
	IOWR_16DIRECT(VREG_LCAC_BASE(ViPipe) + 0x4c + (i * sizeof(GK_S16)),
		      data);
}

static __inline GK_S16 ext_system_localCAC_cbcr_ratio_table_read(VI_PIPE ViPipe,
								 GK_U8 i)
{
	return IORD_16DIRECT(VREG_LCAC_BASE(ViPipe) + 0x4c +
			     (i * sizeof(GK_S16)));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_GlobalCAC_enable
// ------------------------------------------------------------------------------ //
// GlobalCAC_enable
static __inline GK_VOID ext_system_GlobalCAC_enable_write(VI_PIPE ViPipe,
							  GK_U8 data)
{
	IOWR_8DIRECT(VREG_GCAC_BASE(ViPipe), data & 0x1);
}

static __inline GK_U8 ext_system_GlobalCAC_enable_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_GCAC_BASE(ViPipe)) & 0x1);
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_GlobalCAC_coef_update_en
// ------------------------------------------------------------------------------ //
//  GlobalCAC_coef_update_en
static __inline GK_VOID
ext_system_GlobalCAC_coef_update_en_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_GCAC_BASE(ViPipe) + 1, data & 0x1);
}

static __inline GK_U8 ext_system_GlobalCAC_coef_update_en_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_GCAC_BASE(ViPipe) + 1) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_GlobalCAC_coor_center_ver
// ------------------------------------------------------------------------------ //
//  GlobalCAC_coor_center_ver
static __inline GK_VOID
ext_system_GlobalCAC_coor_center_ver_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT(VREG_GCAC_BASE(ViPipe) + 2, data);
}

static __inline GK_U16 ext_system_GlobalCAC_coor_center_ver_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_GCAC_BASE(ViPipe) + 2);
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_GlobalCAC_coor_center_hor
// ------------------------------------------------------------------------------ //
//  GlobalCAC_coor_center_hor
static __inline GK_VOID
ext_system_GlobalCAC_coor_center_hor_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT(VREG_GCAC_BASE(ViPipe) + 4, data);
}

static __inline GK_U16 ext_system_GlobalCAC_coor_center_hor_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_GCAC_BASE(ViPipe) + 4);
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_GlobalCAC_param_red_A
// ------------------------------------------------------------------------------ //
//  GlobalCAC_param_red_A
static __inline GK_VOID ext_system_GlobalCAC_param_red_A_write(VI_PIPE ViPipe,
							       GK_S16 data)
{
	IOWR_16DIRECT(VREG_GCAC_BASE(ViPipe) + 6, data);
}

static __inline GK_S16 ext_system_GlobalCAC_param_red_A_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_GCAC_BASE(ViPipe) + 6);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_GlobalCAC_param_red_B
// ------------------------------------------------------------------------------ //
//  GlobalCAC_param_red_B
static __inline GK_VOID ext_system_GlobalCAC_param_red_B_write(VI_PIPE ViPipe,
							       GK_S16 data)
{
	IOWR_16DIRECT(VREG_GCAC_BASE(ViPipe) + 8, data);
}

static __inline GK_S16 ext_system_GlobalCAC_param_red_B_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_GCAC_BASE(ViPipe) + 8);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_GlobalCAC_param_red_C
// ------------------------------------------------------------------------------ //
//  GlobalCAC_param_red_C
static __inline GK_VOID ext_system_GlobalCAC_param_red_C_write(VI_PIPE ViPipe,
							       GK_S16 data)
{
	IOWR_16DIRECT(VREG_GCAC_BASE(ViPipe) + 0xa, data);
}

static __inline GK_S16 ext_system_GlobalCAC_param_red_C_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_GCAC_BASE(ViPipe) + 0xa);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_GlobalCAC_param_blue_A
// ------------------------------------------------------------------------------ //
//  GlobalCAC_param_blue_A
static __inline GK_VOID ext_system_GlobalCAC_param_blue_A_write(VI_PIPE ViPipe,
								GK_S16 data)
{
	IOWR_16DIRECT(VREG_GCAC_BASE(ViPipe) + 0xc, data);
}

static __inline GK_S16 ext_system_GlobalCAC_param_blue_A_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_GCAC_BASE(ViPipe) + 0xc);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_GlobalCAC_param_blue_B
// ------------------------------------------------------------------------------ //
//  GlobalCAC_param_blue_B
static __inline GK_VOID ext_system_GlobalCAC_param_blue_B_write(VI_PIPE ViPipe,
								GK_S16 data)
{
	IOWR_16DIRECT(VREG_GCAC_BASE(ViPipe) + 0xe, data);
}

static __inline GK_S16 ext_system_GlobalCAC_param_blue_B_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_GCAC_BASE(ViPipe) + 0xe);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_GlobalCAC_param_blue_C
// ------------------------------------------------------------------------------ //
//  GlobalCAC_param_blue_C
static __inline GK_VOID ext_system_GlobalCAC_param_blue_C_write(VI_PIPE ViPipe,
								GK_S16 data)
{
	IOWR_16DIRECT(VREG_GCAC_BASE(ViPipe) + 0x10, data);
}

static __inline GK_S16 ext_system_GlobalCAC_param_blue_C_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_GCAC_BASE(ViPipe) + 0x10);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_GlobalCAC_y_ns_norm
// ------------------------------------------------------------------------------ //
// GlobalCAC_y_ns_norm
static __inline GK_VOID ext_system_GlobalCAC_y_ns_norm_write(VI_PIPE ViPipe,
							     GK_U8 data)
{
	IOWR_8DIRECT(VREG_GCAC_BASE(ViPipe) + 0x12, data);
}

static __inline GK_U8 ext_system_GlobalCAC_y_ns_norm_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_GCAC_BASE(ViPipe) + 0x12));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_GlobalCAC_y_nf_norm
// ------------------------------------------------------------------------------ //
// GlobalCAC_y_nf_norm
static __inline GK_VOID ext_system_GlobalCAC_y_nf_norm_write(VI_PIPE ViPipe,
							     GK_U8 data)
{
	IOWR_8DIRECT(VREG_GCAC_BASE(ViPipe) + 0x13, data);
}

static __inline GK_U8 ext_system_GlobalCAC_y_nf_norm_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_GCAC_BASE(ViPipe) + 0x13));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_GlobalCAC_x_ns_norm
// ------------------------------------------------------------------------------ //
//  GlobalCAC_x_ns_norm
static __inline GK_VOID ext_system_GlobalCAC_x_ns_norm_write(VI_PIPE ViPipe,
							     GK_U8 data)
{
	IOWR_8DIRECT(VREG_GCAC_BASE(ViPipe) + 0x14, data);
}

static __inline GK_U8 ext_system_GlobalCAC_x_ns_norm_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_GCAC_BASE(ViPipe) + 0x14));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_GlobalCAC_x_nf_norm
// ------------------------------------------------------------------------------ //
//  GlobalCAC_x_nf_norm
static __inline GK_VOID ext_system_GlobalCAC_x_nf_norm_write(VI_PIPE ViPipe,
							     GK_U8 data)
{
	IOWR_8DIRECT(VREG_GCAC_BASE(ViPipe) + 0x15, data);
}

static __inline GK_U8 ext_system_GlobalCAC_x_nf_norm_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_GCAC_BASE(ViPipe) + 0x15));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_GlobalCAC_cor_thr
// ------------------------------------------------------------------------------ //
//  GlobalCAC_cor_thr
static __inline GK_VOID ext_system_GlobalCAC_cor_thr_write(VI_PIPE ViPipe,
							   GK_U16 data)
{
	IOWR_16DIRECT(VREG_GCAC_BASE(ViPipe) + 0x16, data);
}

static __inline GK_U16 ext_system_GlobalCAC_cor_thr_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_GCAC_BASE(ViPipe) + 0x16);
}

// ==============================================================//
// CA
// ==============================================================//
// ------------------------------------------------------------------------------ //
// Register: ext_system_ca_y_ratio_lut_write
// ------------------------------------------------------------------------------ //
// LEN =256 DATASIZE GK_U32
static __inline GK_VOID
ext_system_ca_y_ratio_lut_write(VI_PIPE ViPipe, GK_U8 u8Index, GK_U32 data)
{
	IOWR_32DIRECT(VREG_CA_BASE(ViPipe) + (u8Index * sizeof(GK_U32)), data);
}

static __inline GK_U32 ext_system_ca_y_ratio_lut_read(VI_PIPE ViPipe,
						      GK_U8 u8Index)
{
	return IORD_32DIRECT(VREG_CA_BASE(ViPipe) + (u8Index * sizeof(GK_U32)));
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_ca_y_ratio_lut_write
// ------------------------------------------------------------------------------ //
// LEN =16 DATASIZE GK_U16  16*8*2
static __inline GK_VOID
ext_system_ca_iso_ratio_lut_write(VI_PIPE ViPipe, GK_U8 u8Index, GK_U32 data)
{
	IOWR_32DIRECT(VREG_CA_BASE(ViPipe) + 0x400 + (u8Index * sizeof(GK_U32)),
		      data);
}

static __inline GK_U16 ext_system_ca_iso_ratio_lut_read(VI_PIPE ViPipe,
							GK_U8 u8Index)
{
	return IORD_32DIRECT(VREG_CA_BASE(ViPipe) + 0x400 +
			     (u8Index * sizeof(GK_U32)));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_ca_saturation_ratio_write
// ------------------------------------------------------------------------------ //
#define ISP_EXT_CA_SATURATION_RATIO_DEFAULT (1000)
static __inline GK_VOID ext_system_ca_saturation_ratio_write(VI_PIPE ViPipe,
							     GK_U16 data)
{
	IOWR_16DIRECT(VREG_CA_BASE(ViPipe) + 0x440, data);
}

static __inline GK_U16 ext_system_ca_saturation_ratio_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_CA_BASE(ViPipe) + 0x440);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_ca_luma_thdgh_write
// ------------------------------------------------------------------------------ //
#define ISP_EXT_CA_LUMA_THD_HIGH_DEFAULT (400)
static __inline GK_VOID ext_system_ca_luma_thdgh_write(VI_PIPE ViPipe,
						       GK_U16 data)
{
	IOWR_16DIRECT(VREG_CA_BASE(ViPipe) + 0x442, data);
}

static __inline GK_U16 ext_system_ca_luma_thdgh_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_CA_BASE(ViPipe) + 0x442);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_ca_en_write
// ------------------------------------------------------------------------------ //
static __inline GK_VOID ext_system_ca_en_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_CA_BASE(ViPipe) + 0x444, data & 0x1);
}

static __inline GK_U8 ext_system_ca_en_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_CA_BASE(ViPipe) + 0x444) & 0x1);
}
// ------------------------------------------------------------------------------ //
// Register: ext_system_ca_cp_en_write
// ------------------------------------------------------------------------------ //
static __inline GK_VOID ext_system_ca_cp_en_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_CA_BASE(ViPipe) + 0x445, data & 0x1);
}

static __inline GK_U8 ext_system_ca_cp_en_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_CA_BASE(ViPipe) + 0x445) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_ca_cp_en_write
// ------------------------------------------------------------------------------ //
static __inline GK_VOID ext_system_ca_coef_update_en_write(VI_PIPE ViPipe,
							   GK_U8 data)
{
	IOWR_8DIRECT(VREG_CA_BASE(ViPipe) + 0x446, data & 0x1);
}

static __inline GK_U8 ext_system_ca_coef_update_en_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_CA_BASE(ViPipe) + 0x446) & 0x1);
}

static __inline GK_VOID ext_system_ca_cp_lut_write(VI_PIPE ViPipe,
						   GK_U8 u8Index, GK_U32 data)
{
	IOWR_32DIRECT((VREG_CA_BASE(ViPipe) + 0x450) +
			      (u8Index * sizeof(GK_U32)),
		      data);
}

static __inline GK_U32 ext_system_ca_cp_lut_read(VI_PIPE ViPipe, GK_U8 u8Index)
{
	return IORD_32DIRECT((VREG_CA_BASE(ViPipe) + 0x450) +
			     (u8Index * sizeof(GK_U32)));
}

/***********************************************************************************/
// Module: Csc(Original CSC)
/***********************************************************************************/
// ------------------------------------------------------------------------------ //
// Register: ext_system_csc_coef len = 9
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
static __inline GK_VOID ext_system_csc_coef_write(VI_PIPE ViPipe, GK_U8 u8Index,
						  GK_U16 data)
{
	IOWR_16DIRECT((VREG_CSC_BASE(ViPipe) + 0x0) + u8Index * sizeof(GK_U16),
		      data);
}

static __inline GK_U16 ext_system_csc_coef_read(VI_PIPE ViPipe, GK_U8 u8Index)
{
	return IORD_16DIRECT((VREG_CSC_BASE(ViPipe) + 0x0) +
			     u8Index * sizeof(GK_U16));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_csc_dcin len = 3
// ------------------------------------------------------------------------------ //
// args: data (10-bit)
static __inline GK_VOID ext_system_csc_dcin_write(VI_PIPE ViPipe, GK_U8 u8Index,
						  GK_U16 data)
{
	IOWR_16DIRECT((VREG_CSC_BASE(ViPipe) + 0x20) + u8Index * sizeof(GK_U16),
		      data);
}

static __inline GK_U16 ext_system_csc_dcin_read(VI_PIPE ViPipe, GK_U8 u8Index)
{
	return IORD_16DIRECT((VREG_CSC_BASE(ViPipe) + 0x20) +
			     u8Index * sizeof(GK_U16));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_csc_dcout len = 3
// ------------------------------------------------------------------------------ //
// args: data (10-bit)
static __inline GK_VOID ext_system_csc_dcout_write(VI_PIPE ViPipe,
						   GK_U8 u8Index, GK_U16 data)
{
	IOWR_16DIRECT((VREG_CSC_BASE(ViPipe) + 0x30) + u8Index * sizeof(GK_U16),
		      data);
}

static __inline GK_U16 ext_system_csc_dcout_read(VI_PIPE ViPipe, GK_U8 u8Index)
{
	return IORD_16DIRECT((VREG_CSC_BASE(ViPipe) + 0x30) +
			     u8Index * sizeof(GK_U16));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_csc_dcout len = 3
// ------------------------------------------------------------------------------ //
// args: data (10-bit)
static __inline GK_VOID ext_system_csc_gamuttype_write(VI_PIPE ViPipe,
						       GK_U8 data)
{
	IOWR_8DIRECT(VREG_CSC_BASE(ViPipe) + 0x40, data);
}

static __inline GK_U8 ext_system_csc_gamuttype_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_CSC_BASE(ViPipe) + 0x40);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_csc_enable
// ------------------------------------------------------------------------------ //
// args: data (1-bit)
static __inline GK_VOID ext_system_csc_enable_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT((VREG_CSC_BASE(ViPipe) + 0x50), (data & 0x1));
}

static __inline GK_U8 ext_system_csc_enable_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_CSC_BASE(ViPipe) + 0x50) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_csc_enable
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_csc_contrast_write(VI_PIPE ViPipe,
						      GK_U8 data)
{
	IOWR_8DIRECT((VREG_CSC_BASE(ViPipe) + 0x51), data);
}

static __inline GK_U8 ext_system_csc_contrast_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_CSC_BASE(ViPipe) + 0x51));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_csc_enable
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_csc_hue_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT((VREG_CSC_BASE(ViPipe) + 0x52), data);
}

static __inline GK_U8 ext_system_csc_hue_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_CSC_BASE(ViPipe) + 0x52));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_csc_enable
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_csc_sat_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT((VREG_CSC_BASE(ViPipe) + 0x53), data);
}

static __inline GK_U8 ext_system_csc_sat_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_CSC_BASE(ViPipe) + 0x53));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_csc_enable
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_csc_luma_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT((VREG_CSC_BASE(ViPipe) + 0x54), data);
}

static __inline GK_U8 ext_system_csc_luma_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_CSC_BASE(ViPipe) + 0x54));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_csc_enable
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_csc_limitrange_en_write(VI_PIPE ViPipe,
							   GK_U8 data)
{
	IOWR_8DIRECT((VREG_CSC_BASE(ViPipe) + 0x55), data);
}

static __inline GK_U8 ext_system_csc_limitrange_en_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_CSC_BASE(ViPipe) + 0x55));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_csc_enable
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_csc_ext_en_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT((VREG_CSC_BASE(ViPipe) + 0x56), data);
}

static __inline GK_U8 ext_system_csc_ext_en_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_CSC_BASE(ViPipe) + 0x56));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_csc_enable
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_csc_ctmode_en_write(VI_PIPE ViPipe,
						       GK_U8 data)
{
	IOWR_8DIRECT((VREG_CSC_BASE(ViPipe) + 0x57), data);
}

static __inline GK_U8 ext_system_csc_ctmode_en_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_CSC_BASE(ViPipe) + 0x57));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_csc_update
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_csc_attr_update_write(VI_PIPE ViPipe,
							 GK_U8 data)
{
	IOWR_8DIRECT((VREG_CSC_BASE(ViPipe) + 0x58), data);
}

static __inline GK_U8 ext_system_csc_attr_update_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_CSC_BASE(ViPipe) + 0x58));
}
// ==============================================================//
// LDCI
// ==============================================================//
/***********************************************************************************/
/***********************************************************************************/
/*************************ldci---U8--EXT_REGISTER************************************/
/***********************************************************************************/
/***********************************************************************************/
#define ISP_EXT_SYSTEM_LDCI_MANU_MODE_DEFAULT (0)
// args: data (8-bit)
static __inline GK_VOID ext_system_ldci_manu_mode_write(VI_PIPE ViPipe,
							GK_U8 data)
{
	IOWR_8DIRECT(VREG_LDCI_BASE(ViPipe), data & 0x1);
}
static __inline GK_U8 ext_system_ldci_manu_mode_read(VI_PIPE ViPipe)
{
	return ((IORD_8DIRECT(VREG_LDCI_BASE(ViPipe))) & 0x1);
}

#define ISP_EXT_SYSTEM_LDCI_MANU_HEPOSWGT_DEFAULT (64)
// args: data (8-bit)
static __inline GK_VOID ext_system_ldci_manu_hePosWgt_write(VI_PIPE ViPipe,
							    GK_U8 data)
{
	IOWR_8DIRECT(VREG_LDCI_BASE(ViPipe) + 0x1, data & 0xff);
}
static __inline GK_U8 ext_system_ldci_manu_hePosWgt_read(VI_PIPE ViPipe)
{
	return ((IORD_8DIRECT(VREG_LDCI_BASE(ViPipe) + 0x1)) & 0xff);
}

#define ISP_EXT_SYSTEM_LDCI_MANU_HEPOSSIGMA_DEFAULT (80)
// args: data (8-bit)
static __inline GK_VOID ext_system_ldci_manu_hePosSigma_write(VI_PIPE ViPipe,
							      GK_U8 data)
{
	IOWR_8DIRECT(VREG_LDCI_BASE(ViPipe) + 0x2, data & 0xff);
}
static __inline GK_U8 ext_system_ldci_manu_hePosSigma_read(VI_PIPE ViPipe)
{
	return ((IORD_8DIRECT(VREG_LDCI_BASE(ViPipe) + 0x2)) & 0xff);
}

#define ISP_EXT_SYSTEM_LDCI_MANU_HEPOSMEAN_DEFAULT (0)
// args: data (8-bit)
static __inline GK_VOID ext_system_ldci_manu_hePosMean_write(VI_PIPE ViPipe,
							     GK_U8 data)
{
	IOWR_8DIRECT(VREG_LDCI_BASE(ViPipe) + 0x3, data & 0xff);
}
static __inline GK_U8 ext_system_ldci_manu_hePosMean_read(VI_PIPE ViPipe)
{
	return ((IORD_8DIRECT(VREG_LDCI_BASE(ViPipe) + 0x3)) & 0xff);
}

#define ISP_EXT_SYSTEM_LDCI_MANU_HENEGWGT_DEFAULT (50)
// args: data (8-bit)
static __inline GK_VOID ext_system_ldci_manu_heNegWgt_write(VI_PIPE ViPipe,
							    GK_U8 data)
{
	IOWR_8DIRECT(VREG_LDCI_BASE(ViPipe) + 0x4, data & 0xff);
}
static __inline GK_U8 ext_system_ldci_manu_heNegWgt_read(VI_PIPE ViPipe)
{
	return ((IORD_8DIRECT(VREG_LDCI_BASE(ViPipe) + 0x4)) & 0xff);
}

#define ISP_EXT_SYSTEM_LDCI_MANU_HENEGSIGMA_DEFAULT (80)
// args: data (8-bit)
static __inline GK_VOID ext_system_ldci_manu_heNegSigma_write(VI_PIPE ViPipe,
							      GK_U8 data)
{
	IOWR_8DIRECT(VREG_LDCI_BASE(ViPipe) + 0x5, data & 0xff);
}
static __inline GK_U8 ext_system_ldci_manu_heNegSigma_read(VI_PIPE ViPipe)
{
	return ((IORD_8DIRECT(VREG_LDCI_BASE(ViPipe) + 0x5)) & 0xff);
}

#define ISP_EXT_SYSTEM_LDCI_MANU_HENEGMEAN_DEFAULT (0)
// args: data (8-bit)
static __inline GK_VOID ext_system_ldci_manu_heNegMean_write(VI_PIPE ViPipe,
							     GK_U8 data)
{
	IOWR_8DIRECT(VREG_LDCI_BASE(ViPipe) + 0x6, data & 0xff);
}
static __inline GK_U8 ext_system_ldci_manu_heNegMean_read(VI_PIPE ViPipe)
{
	return ((IORD_8DIRECT(VREG_LDCI_BASE(ViPipe) + 0x6)) & 0xff);
}

#define ISP_EXT_SYSTEM_LDCI_ENABLE_DEFAULT (0)
// args: data (8-bit)
static __inline GK_VOID ext_system_ldci_enable_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_LDCI_BASE(ViPipe) + 0x7, data & 0x1);
}
static __inline GK_U8 ext_system_ldci_enable_read(VI_PIPE ViPipe)
{
	return ((IORD_8DIRECT(VREG_LDCI_BASE(ViPipe) + 0x7)) & 0x1);
}

#define ISP_EXT_SYSTEM_LDCI_GAUSSLPFSIGMA_DEFAULT (36)
// args: data (8-bit)
static __inline GK_VOID ext_system_ldci_gaussLPFSigma_write(VI_PIPE ViPipe,
							    GK_U8 data)
{
	IOWR_8DIRECT(VREG_LDCI_BASE(ViPipe) + 0x8, data & 0xff);
}
static __inline GK_U8 ext_system_ldci_gaussLPFSigma_read(VI_PIPE ViPipe)
{
	return ((IORD_8DIRECT(VREG_LDCI_BASE(ViPipe) + 0x8)) & 0xff);
}

/***********************************************************************************/
/***********************************************************************************/
/*************************ldci---U16--EXT_REGISTER***********************************/
/***********************************************************************************/
/***********************************************************************************/
#define ISP_EXT_SYSTEM_LDCI_MANU_BLCCTRL_DEFAULT (5)
// args: data (16-bit)
static __inline GK_VOID ext_system_ldci_manu_blcCtrl_write(VI_PIPE ViPipe,
							   GK_U16 data)
{
	IOWR_16DIRECT(VREG_LDCI_BASE(ViPipe) + 0xa, data & 0x1ff);
}
static __inline GK_U16 ext_system_ldci_manu_blcCtrl_read(VI_PIPE ViPipe)
{
	return ((IORD_16DIRECT(VREG_LDCI_BASE(ViPipe) + 0xa)) & 0x1ff);
}

/***********************************************************************************/
/***********************************************************************************/
/*************************ldci---ARRAY--EXT_REGISTER*********************************/
/***********************************************************************************/
/***********************************************************************************/
// ******************************auoto 16 multi addrs*********************************//
static __inline GK_VOID
ext_system_ldci_hePosWgt_write(VI_PIPE ViPipe, GK_U8 u8Index, GK_U8 data)
{
	IOWR_8DIRECT((VREG_LDCI_BASE(ViPipe) + 0x10 + u8Index * sizeof(GK_U8)),
		     data & 0xff);
}
static __inline GK_U8 ext_system_ldci_hePosWgt_read(VI_PIPE ViPipe,
						    GK_U8 u8Index)
{
	return ((IORD_8DIRECT((VREG_LDCI_BASE(ViPipe) + 0x10 +
			       u8Index * sizeof(GK_U8)))) &
		0xff);
}

// ******************************auoto 16 multi addrs*********************************//
static __inline GK_VOID
ext_system_ldci_hePosSigma_write(VI_PIPE ViPipe, GK_U8 u8Index, GK_U8 data)
{
	IOWR_8DIRECT((VREG_LDCI_BASE(ViPipe) + 0x20 + u8Index * sizeof(GK_U8)),
		     data & 0xff);
}
static __inline GK_U8 ext_system_ldci_hePosSigma_read(VI_PIPE ViPipe,
						      GK_U8 u8Index)
{
	return ((IORD_8DIRECT((VREG_LDCI_BASE(ViPipe) + 0x20 +
			       u8Index * sizeof(GK_U8)))) &
		0xff);
}

// ******************************auoto 16 multi addrs*********************************//
static __inline GK_VOID
ext_system_ldci_hePosMean_write(VI_PIPE ViPipe, GK_U8 u8Index, GK_U8 data)
{
	IOWR_8DIRECT((VREG_LDCI_BASE(ViPipe) + 0x30 + u8Index * sizeof(GK_U8)),
		     data & 0xff);
}
static __inline GK_U8 ext_system_ldci_hePosMean_read(VI_PIPE ViPipe,
						     GK_U8 u8Index)
{
	return ((IORD_8DIRECT((VREG_LDCI_BASE(ViPipe) + 0x30 +
			       u8Index * sizeof(GK_U8)))) &
		0xff);
}

// ******************************auoto 16 multi addrs*********************************//
static __inline GK_VOID
ext_system_ldci_heNegWgt_write(VI_PIPE ViPipe, GK_U8 u8Index, GK_U8 data)
{
	IOWR_8DIRECT((VREG_LDCI_BASE(ViPipe) + 0x40 + u8Index * sizeof(GK_U8)),
		     data & 0xff);
}
static __inline GK_U8 ext_system_ldci_heNegWgt_read(VI_PIPE ViPipe,
						    GK_U8 u8Index)
{
	return ((IORD_8DIRECT((VREG_LDCI_BASE(ViPipe) + 0x40 +
			       u8Index * sizeof(GK_U8)))) &
		0xff);
}

// ******************************auoto 16 multi addrs*********************************//
static __inline GK_VOID
ext_system_ldci_heNegSigma_write(VI_PIPE ViPipe, GK_U8 u8Index, GK_U8 data)
{
	IOWR_8DIRECT((VREG_LDCI_BASE(ViPipe) + 0x50 + u8Index * sizeof(GK_U8)),
		     data & 0xff);
}
static __inline GK_U8 ext_system_ldci_heNegSigma_read(VI_PIPE ViPipe,
						      GK_U8 u8Index)
{
	return ((IORD_8DIRECT((VREG_LDCI_BASE(ViPipe) + 0x50 +
			       u8Index * sizeof(GK_U8)))) &
		0xff);
}

// ******************************auoto 16 multi addrs*********************************//
static __inline GK_VOID
ext_system_ldci_heNegMean_write(VI_PIPE ViPipe, GK_U8 u8Index, GK_U8 data)
{
	IOWR_8DIRECT((VREG_LDCI_BASE(ViPipe) + 0x60 + u8Index * sizeof(GK_U8)),
		     data & 0xff);
}
static __inline GK_U8 ext_system_ldci_heNegMean_read(VI_PIPE ViPipe,
						     GK_U8 u8Index)
{
	return ((IORD_8DIRECT((VREG_LDCI_BASE(ViPipe) + 0x60 +
			       u8Index * sizeof(GK_U8)))) &
		0xff);
}

// ******************************auoto 16 multi addrs*********************************//
static __inline GK_VOID
ext_system_ldci_blcCtrl_write(VI_PIPE ViPipe, GK_U8 u8Index, GK_U16 data)
{
	IOWR_16DIRECT(
		(VREG_LDCI_BASE(ViPipe) + 0x70 + u8Index * sizeof(GK_U16)),
		data & 0x1ff);
}
static __inline GK_U16 ext_system_ldci_blcCtrl_read(VI_PIPE ViPipe,
						    GK_U8 u8Index)
{
	return ((IORD_16DIRECT((VREG_LDCI_BASE(ViPipe) + 0x70 +
				u8Index * sizeof(GK_U16)))) &
		0x1ff);
}

#define ISP_EXT_SYSTEM_LDCI_TPR_INCR_COEF_DEFAULT (8)

static __inline GK_VOID ext_system_ldci_TprIncrCoef_write(VI_PIPE ViPipe,
							  GK_U16 data)
{
	IOWR_16DIRECT(VREG_LDCI_BASE(ViPipe) + 0x90, data & 0x1ff);
}
static __inline GK_U16 ext_system_ldci_TprIncrCoef_read(VI_PIPE ViPipe)
{
	return ((IORD_16DIRECT(VREG_LDCI_BASE(ViPipe) + 0x90)) & 0x1ff);
}

#define ISP_EXT_SYSTEM_LDCI_TPR_DECR_COEF_DEFAULT (8)

static __inline GK_VOID ext_system_ldci_TprDecrCoef_write(VI_PIPE ViPipe,
							  GK_U16 data)
{
	IOWR_16DIRECT(VREG_LDCI_BASE(ViPipe) + 0x92, data & 0x1ff);
}
static __inline GK_U16 ext_system_ldci_TprDecrCoef_read(VI_PIPE ViPipe)
{
	return ((IORD_16DIRECT(VREG_LDCI_BASE(ViPipe) + 0x92)) & 0x1ff);
}

// ==============================================================//
// FeLogLUT
// ==============================================================//

#define ISP_EXT_SYSTEM_FELOGLUT_ENABLE_DEFAULT (0)
// args: data (8-bit)
static __inline GK_VOID ext_system_feloglut_enable_write(VI_PIPE ViPipe,
							 GK_U8 data)
{
	IOWR_8DIRECT(VREG_FELOGLUT_BASE(ViPipe), data & 0x1);
}
static __inline GK_U8 ext_system_feloglut_enable_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_FELOGLUT_BASE(ViPipe)) & 0x1);
}

// ==============================================================//
// LogLUT
// ==============================================================//

#define ISP_EXT_SYSTEM_LOGLUT_ENABLE_DEFAULT (0)
// args: data (8-bit)
static __inline GK_VOID ext_system_loglut_enable_write(VI_PIPE ViPipe,
						       GK_U8 data)
{
	IOWR_8DIRECT(VREG_LOGLUT_BASE(ViPipe), data & 0x1);
}
static __inline GK_U8 ext_system_loglut_enable_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_LOGLUT_BASE(ViPipe)) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: DNG info
// ------------------------------------------------------------------------------ //
// args: data (32-bit)

#define ISP_EXT_SYSTEM_DNG_STATIC_INFO_VALID_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_DNG_STATIC_INFO_VALID_DATASIZE (8)

// args: data (8-bit)
static __inline GK_VOID ext_system_dng_static_info_valid_write(VI_PIPE ViPipe,
							       GK_U8 data)
{
	IOWR_8DIRECT(VREG_DNGINFO_BASE(ViPipe), data);
}

static __inline GK_U8 ext_system_dng_static_info_valid_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_DNGINFO_BASE(ViPipe));
}

#define ISP_EXT_SYSTEM_DNG_HIGH_WB_GAIN_R_DEFAULT (0x400)
#define ISP_EXT_SYSTEM_DNG_HIGH_WB_GAIN_R_DATASIZE (16)
// args: data (16-bit)
static __inline GK_VOID ext_system_dnggh_wb_gain_r_write(VI_PIPE ViPipe,
							 GK_U16 data)
{
	IOWR_16DIRECT(VREG_DNGINFO_BASE(ViPipe) + 0x4, data);
}

static __inline GK_U16 ext_system_dnggh_wb_gain_r_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_DNGINFO_BASE(ViPipe) + 0x4);
}

#define ISP_EXT_SYSTEM_DNG_HIGH_WB_GAIN_G_DEFAULT (0x400)
#define ISP_EXT_SYSTEM_DNG_HIGH_WB_GAIN_G_DATASIZE (16)
// args: data (16-bit)
static __inline GK_VOID ext_system_dnggh_wb_gain_g_write(VI_PIPE ViPipe,
							 GK_U16 data)
{
	IOWR_16DIRECT(VREG_DNGINFO_BASE(ViPipe) + 0x8, data);
}

static __inline GK_U16 ext_system_dnggh_wb_gain_g_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_DNGINFO_BASE(ViPipe) + 0x8);
}

#define ISP_EXT_SYSTEM_DNG_HIGH_WB_GAIN_B_DEFAULT (0x400)
#define ISP_EXT_SYSTEM_DNG_HIGH_WB_GAIN_B_DATASIZE (16)
// args: data (16-bit)
static __inline GK_VOID ext_system_dnggh_wb_gain_b_write(VI_PIPE ViPipe,
							 GK_U16 data)
{
	IOWR_16DIRECT(VREG_DNGINFO_BASE(ViPipe) + 0xc, data);
}

static __inline GK_U16 ext_system_dnggh_wb_gain_b_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_DNGINFO_BASE(ViPipe) + 0xc);
}

#define ISP_EXT_SYSTEM_DNG_LOW_WB_GAIN_R_DEFAULT (0x400)
#define ISP_EXT_SYSTEM_DNG_LOW_WB_GAIN_R_DATASIZE (16)
// args: data (16-bit)
static __inline GK_VOID ext_system_dng_low_wb_gain_r_write(VI_PIPE ViPipe,
							   GK_U16 data)
{
	IOWR_16DIRECT(VREG_DNGINFO_BASE(ViPipe) + 0x10, data);
}

static __inline GK_U16 ext_system_dng_low_wb_gain_r_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_DNGINFO_BASE(ViPipe) + 0x10);
}

#define ISP_EXT_SYSTEM_DNG_LOW_WB_GAIN_G_DEFAULT (0x400)
#define ISP_EXT_SYSTEM_DNG_LOW_WB_GAIN_G_DATASIZE (16)
// args: data (16-bit)
static __inline GK_VOID ext_system_dng_low_wb_gain_g_write(VI_PIPE ViPipe,
							   GK_U16 data)
{
	IOWR_16DIRECT(VREG_DNGINFO_BASE(ViPipe) + 0x14, data);
}

static __inline GK_U16 ext_system_dng_low_wb_gain_g_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_DNGINFO_BASE(ViPipe) + 0x14);
}

#define ISP_EXT_SYSTEM_DNG_LOW_WB_GAIN_B_DEFAULT (0x400)
#define ISP_EXT_SYSTEM_DNG_LOW_WB_GAIN_B_DATASIZE (16)
// args: data (16-bit)
static __inline GK_VOID ext_system_dng_low_wb_gain_b_write(VI_PIPE ViPipe,
							   GK_U16 data)
{
	IOWR_16DIRECT(VREG_DNGINFO_BASE(ViPipe) + 0x18, data);
}

static __inline GK_U16 ext_system_dng_low_wb_gain_b_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_DNGINFO_BASE(ViPipe) + 0x18);
}

// ==============================================================//
// Radial Crop
// ==============================================================//
// ------------------------------------------------------------------------------ //
// Register: ext_system_rc_en_write
// ------------------------------------------------------------------------------ //
static __inline GK_VOID ext_system_rc_en_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_RC_BASE(ViPipe) + 0x0, data & 0x1);
}

static __inline GK_U8 ext_system_rc_en_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_RC_BASE(ViPipe) + 0x0) & 0x1);
}

static __inline GK_VOID ext_system_rc_coef_update_en_write(VI_PIPE ViPipe,
							   GK_U8 data)
{
	IOWR_8DIRECT(VREG_RC_BASE(ViPipe) + 0x1, data & 0x1);
}

static __inline GK_U8 ext_system_rc_coef_update_en_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_RC_BASE(ViPipe) + 0x1) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_rc_center_hor_coor_write
// ------------------------------------------------------------------------------ //
static __inline GK_VOID ext_system_rc_center_hor_coor_write(VI_PIPE ViPipe,
							    GK_U16 data)
{
	IOWR_16DIRECT(VREG_RC_BASE(ViPipe) + 0x2, data);
}

static __inline GK_U16 ext_system_rc_center_hor_coor_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_RC_BASE(ViPipe) + 0x2));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_rc_center_ver_coor_write
// ------------------------------------------------------------------------------ //
static __inline GK_VOID ext_system_rc_center_ver_coor_write(VI_PIPE ViPipe,
							    GK_U16 data)
{
	IOWR_16DIRECT(VREG_RC_BASE(ViPipe) + 0x4, data);
}

static __inline GK_U16 ext_system_rc_center_ver_coor_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_RC_BASE(ViPipe) + 0x4));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_rc_radius_write
// ------------------------------------------------------------------------------ //
static __inline GK_VOID ext_system_rc_radius_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT(VREG_RC_BASE(ViPipe) + 0x6, data);
}

static __inline GK_U16 ext_system_rc_radius_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_RC_BASE(ViPipe) + 0x6));
}

// ==============================================================//
// Radial Crop
// ==============================================================//
// ------------------------------------------------------------------------------ //
// Register: ext_system_gamma_lut_write
// ------------------------------------------------------------------------------ //
static __inline GK_VOID ext_system_gamma_en_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_GAMMA_BASE(ViPipe), data);
}

static __inline GK_U8 ext_system_gamma_en_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_GAMMA_BASE(ViPipe));
}

static __inline GK_VOID ext_system_gamma_lut_update_write(VI_PIPE ViPipe,
							  GK_U8 data)
{
	IOWR_8DIRECT(VREG_GAMMA_BASE(ViPipe) + 0x1, data);
}

static __inline GK_U8 ext_system_gamma_lut_update_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_GAMMA_BASE(ViPipe) + 0x1);
}

static __inline GK_VOID ext_system_gamma_lut_write(VI_PIPE ViPipe,
						   GK_U16 u16Index, GK_U16 data)
{
	IOWR_16DIRECT(VREG_GAMMA_BASE(ViPipe) + 0x4 +
			      (u16Index * sizeof(GK_U16)),
		      data);
}

static __inline GK_U16 ext_system_gamma_lut_read(VI_PIPE ViPipe,
						 GK_U16 u16Index)
{
	return (IORD_16DIRECT(VREG_GAMMA_BASE(ViPipe) + 0x4 +
			      (u16Index * sizeof(GK_U16))));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_pregamma_lut_write
// ------------------------------------------------------------------------------ //
static __inline GK_VOID ext_system_pregamma_en_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_PREGAMMA_BASE(ViPipe), data);
}

static __inline GK_U8 ext_system_pregamma_en_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_PREGAMMA_BASE(ViPipe));
}

static __inline GK_VOID ext_system_pregamma_lut_update_write(VI_PIPE ViPipe,
							     GK_U8 data)
{
	IOWR_8DIRECT(VREG_PREGAMMA_BASE(ViPipe) + 0x1, data);
}

static __inline GK_U8 ext_system_pregamma_lut_update_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_PREGAMMA_BASE(ViPipe) + 0x1);
}

/* depth : 257 * sizeof(GK_U32)  = 0x404 */
static __inline GK_VOID
ext_system_pregamma_lut_write(VI_PIPE ViPipe, GK_U16 u16Index, GK_U32 data)
{
	IOWR_32DIRECT(VREG_PREGAMMA_BASE(ViPipe) + 0x4 +
			      (u16Index * sizeof(GK_U32)),
		      data);
}

static __inline GK_U32 ext_system_pregamma_lut_read(VI_PIPE ViPipe,
						    GK_U32 u16Index)
{
	return (IORD_32DIRECT(VREG_PREGAMMA_BASE(ViPipe) + 0x4 +
			      (u16Index * sizeof(GK_U32))));
}

/*********************************************************************************/
/*********************************************************************************/
/**********************************CLUT*******************************************/
/*********************************************************************************/
/*********************************************************************************/
// ------------------------------------------------------------------------------ //
// Register: ext_system_clut_en_write
// ------------------------------------------------------------------------------ //
static __inline GK_VOID ext_system_clut_en_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_CLUT_BASE(ViPipe) + 0x0, data & 0x1);
}

static __inline GK_U8 ext_system_clut_en_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_CLUT_BASE(ViPipe) + 0x0) & 0x1);
}

static __inline GK_VOID ext_system_clut_lut_update_en_write(VI_PIPE ViPipe,
							    GK_U8 data)
{
	IOWR_8DIRECT(VREG_CLUT_BASE(ViPipe) + 0x1, data & 0x1);
}
static __inline GK_U8 ext_system_clut_lut_update_en_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_CLUT_BASE(ViPipe) + 0x1) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_clut_gainR_write
// ------------------------------------------------------------------------------ //
static __inline GK_VOID ext_system_clut_gainR_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_CLUT_BASE(ViPipe) + 0x2), data);
}

static __inline GK_U16 ext_system_clut_gainR_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_CLUT_BASE(ViPipe) + 0x2));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_clut_gainG_write
// ------------------------------------------------------------------------------ //
static __inline GK_VOID ext_system_clut_gainG_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_CLUT_BASE(ViPipe) + 0x4), data);
}

static __inline GK_U16 ext_system_clut_gainG_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_CLUT_BASE(ViPipe) + 0x4));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_clut_gainB_write
// ------------------------------------------------------------------------------ //
static __inline GK_VOID ext_system_clut_gainB_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_CLUT_BASE(ViPipe) + 0x6), data);
}

static __inline GK_U16 ext_system_clut_gainB_read(VI_PIPE ViPipe)
{
	return (IORD_16DIRECT(VREG_CLUT_BASE(ViPipe) + 0x6));
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_clut_b_lut_write
// ------------------------------------------------------------------------------ //
static __inline GK_VOID ext_system_clut_ctrl_update_en_write(VI_PIPE ViPipe,
							     GK_U8 data)
{
	IOWR_8DIRECT(VREG_CLUT_BASE(ViPipe) + 0x8, data & 0x1);
}
static __inline GK_U8 ext_system_clut_ctrl_update_en_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_CLUT_BASE(ViPipe) + 0x8) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_zone_row_awb
// ------------------------------------------------------------------------------ //
// config information of awb statistics
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_AWB_SWITCH_DEFAULT (0)
#define ISP_EXT_SYSTEM_AWB_SWITCH_DATASIZE (8)
#define ISP_EXT_SYSTEM_AWB_SWITCH_AFTER_DG (0)
#define ISP_EXT_SYSTEM_AWB_SWITCH_AFTER_EXPANDER (1)
#define ISP_EXT_SYSTEM_AWB_SWITCH_AFTER_DRC (2)

// args: data (2-bit)
static __inline GK_VOID ext_system_awb_switch_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_16DIRECT((VREG_AWB_BASE(ViPipe) + 0x2), data);
}
static __inline GK_U8 ext_system_awb_switch_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_AWB_BASE(ViPipe) + 0x2);
}

#define ISP_EXT_SYSTEM_AWB_HNUM_DEFAULT (0x20)
#define ISP_EXT_SYSTEM_AWB_HNUM_DATASIZE (16)

// args: data (16-bit)
static __inline GK_VOID ext_system_awb_hnum_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AWB_BASE(ViPipe) + 0x4), data);
}

static __inline GK_U16 ext_system_awb_hnum_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_AWB_BASE(ViPipe) + 0x4);
}

#define ISP_EXT_SYSTEM_AWB_VNUM_DEFAULT (0x20)
#define ISP_EXT_SYSTEM_AWB_VNUM_DATASIZE (16)

// args: data (16-bit)
static __inline GK_VOID ext_system_awb_vnum_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AWB_BASE(ViPipe) + 0x6), data);
}

static __inline GK_U16 ext_system_awb_vnum_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_AWB_BASE(ViPipe) + 0x6);
}

#define ISP_EXT_SYSTEM_AWB_ZONE_BIN_DEFAULT (0x1)
#define ISP_EXT_SYSTEM_AWB_ZONE_BIN_DATASIZE (16)

// args: data (16-bit)
static __inline GK_VOID ext_system_awb_zone_bin_write(VI_PIPE ViPipe,
						      GK_U16 data)
{
	IOWR_16DIRECT((VREG_AWB_BASE(ViPipe) + 0x8), data);
}

static __inline GK_U16 ext_system_awb_zone_bin_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_AWB_BASE(ViPipe) + 0x8);
}

#define ISP_EXT_SYSTEM_AWB_HIST_BIN_THRESH0_DEFAULT (0x2000)
#define ISP_EXT_SYSTEM_AWB_HIST_BIN_THRESH0_DATASIZE (16)

// args: data (16-bit)
static __inline GK_VOID ext_system_awbst_bin_thresh0_write(VI_PIPE ViPipe,
							   GK_U16 data)
{
	IOWR_16DIRECT((VREG_AWB_BASE(ViPipe) + 0xa), data);
}

static __inline GK_U16 ext_system_awbst_bin_thresh0_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_AWB_BASE(ViPipe) + 0xa);
}

#define ISP_EXT_SYSTEM_AWB_HIST_BIN_THRESH1_DEFAULT (0x4000)
#define ISP_EXT_SYSTEM_AWB_HIST_BIN_THRESH1_DATASIZE (16)

// args: data (16-bit)
static __inline GK_VOID ext_system_awbst_bin_thresh1_write(VI_PIPE ViPipe,
							   GK_U16 data)
{
	IOWR_16DIRECT((VREG_AWB_BASE(ViPipe) + 0xc), data);
}

static __inline GK_U16 ext_system_awbst_bin_thresh1_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_AWB_BASE(ViPipe) + 0xc);
}

#define ISP_EXT_SYSTEM_AWB_HIST_BIN_THRESH2_DEFAULT (0x8000)
#define ISP_EXT_SYSTEM_AWB_HIST_BIN_THRESH2_DATASIZE (16)

// args: data (16-bit)
static __inline GK_VOID ext_system_awbst_bin_thresh2_write(VI_PIPE ViPipe,
							   GK_U16 data)
{
	IOWR_16DIRECT((VREG_AWB_BASE(ViPipe) + 0xe), data);
}

static __inline GK_U16 ext_system_awbst_bin_thresh2_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_AWB_BASE(ViPipe) + 0xe);
}

#define ISP_EXT_SYSTEM_AWB_HIST_BIN_THRESH3_DEFAULT (0xffff)
#define ISP_EXT_SYSTEM_AWB_HIST_BIN_THRESH3_DATASIZE (16)

// args: data (16-bit)
static __inline GK_VOID ext_system_awbst_bin_thresh3_write(VI_PIPE ViPipe,
							   GK_U16 data)
{
	IOWR_16DIRECT((VREG_AWB_BASE(ViPipe) + 0x10), data);
}

static __inline GK_U16 ext_system_awbst_bin_thresh3_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_AWB_BASE(ViPipe) + 0x10);
}

#define ISP_EXT_SYSTEM_AWB_WHITE_LEVEL_DEFAULT (0xffff)
#define ISP_EXT_SYSTEM_AWB_WHITE_LEVEL_DATASIZE (16)

// args: data (16-bit)
static __inline GK_VOID ext_system_awb_white_level_write(VI_PIPE ViPipe,
							 GK_U16 data)
{
	IOWR_16DIRECT((VREG_AWB_BASE(ViPipe) + 0x12), data);
}

static __inline GK_U16 ext_system_awb_white_level_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_AWB_BASE(ViPipe) + 0x12);
}

#define ISP_EXT_SYSTEM_AWB_BLACK_LEVEL_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_AWB_BLACK_LEVEL_DATASIZE (16)

// args: data (16-bit)
static __inline GK_VOID ext_system_awb_black_level_write(VI_PIPE ViPipe,
							 GK_U16 data)
{
	IOWR_16DIRECT((VREG_AWB_BASE(ViPipe) + 0x14), data);
}

static __inline GK_U16 ext_system_awb_black_level_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_AWB_BASE(ViPipe) + 0x14);
}

#define ISP_EXT_SYSTEM_AWB_CR_REF_MAX_DEFAULT (0x180)
#define ISP_EXT_SYSTEM_AWB_CR_REF_MAX_DATASIZE (16)

// args: data (16-bit)
static __inline GK_VOID ext_system_awb_cr_ref_max_write(VI_PIPE ViPipe,
							GK_U16 data)
{
	IOWR_16DIRECT((VREG_AWB_BASE(ViPipe) + 0x16), data);
}

static __inline GK_U16 ext_system_awb_cr_ref_max_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_AWB_BASE(ViPipe) + 0x16);
}

#define ISP_EXT_SYSTEM_AWB_CR_REF_MIN_DEFAULT (0x40)
#define ISP_EXT_SYSTEM_AWB_CR_REF_MIN_DATASIZE (16)

// args: data (16-bit)
static __inline GK_VOID ext_system_awb_cr_ref_min_write(VI_PIPE ViPipe,
							GK_U16 data)
{
	IOWR_16DIRECT((VREG_AWB_BASE(ViPipe) + 0x18), data);
}

static __inline GK_U16 ext_system_awb_cr_ref_min_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_AWB_BASE(ViPipe) + 0x18);
}

#define ISP_EXT_SYSTEM_AWB_CB_REF_MAX_DEFAULT (0x180)
#define ISP_EXT_SYSTEM_AWB_CB_REF_MAX_DATASIZE (16)

// args: data (16-bit)
static __inline GK_VOID ext_system_awb_cb_ref_max_write(VI_PIPE ViPipe,
							GK_U16 data)
{
	IOWR_16DIRECT((VREG_AWB_BASE(ViPipe) + 0x1a), data);
}

static __inline GK_U16 ext_system_awb_cb_ref_max_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_AWB_BASE(ViPipe) + 0x1a);
}

#define ISP_EXT_SYSTEM_AWB_CB_REF_MIN_DEFAULT (0x40)
#define ISP_EXT_SYSTEM_AWB_CB_REF_MIN_DATASIZE (16)

// args: data (16-bit)
static __inline GK_VOID ext_system_awb_cb_ref_min_write(VI_PIPE ViPipe,
							GK_U16 data)
{
	IOWR_16DIRECT((VREG_AWB_BASE(ViPipe) + 0x1c), data);
}

static __inline GK_U16 ext_system_awb_cb_ref_min_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_AWB_BASE(ViPipe) + 0x1c);
}

#define ISP_EXT_SYSTEM_AWB_GAIN_ENABLE_DEFAULT (0x1)
#define ISP_EXT_SYSTEM_AWB_GAIN_ENABLE_DATASIZE (8)

// args: data (8-bit)
static __inline GK_VOID ext_system_awb_gain_enable_write(VI_PIPE ViPipe,
							 GK_U8 data)
{
	IOWR_8DIRECT((VREG_AWB_BASE(ViPipe) + 0x1e), (data & 0x1));
}

static __inline GK_U8 ext_system_awb_gain_enable_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AWB_BASE(ViPipe) + 0x1e) & 0x1);
}

// args: data (16-bit)
static __inline GK_VOID ext_system_cc_enable_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT((VREG_AWB_BASE(ViPipe) + 0x20), (data & 0x1));
}

static __inline GK_U8 ext_system_cc_enable_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AWB_BASE(ViPipe) + 0x20) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_cc_colortone_rgain
// ------------------------------------------------------------------------------ //
// cc colortone
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_CCM_COLORTONE_RGAIN_DATASIZE (16)
#define ISP_EXT_SYSTEM_CCM_COLORTONE_RGAIN_DEFAULT (256)

// args: data (16-bit)
static __inline GK_VOID ext_system_cc_colortone_rgain_write(VI_PIPE ViPipe,
							    GK_U16 data)
{
	IOWR_16DIRECT((VREG_AWB_BASE(ViPipe) + 0x22), data);
}

static __inline GK_U16 ext_system_cc_colortone_rgain_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_AWB_BASE(ViPipe) + 0x22);
}

#define ISP_EXT_SYSTEM_CCM_COLORTONE_GGAIN_DATASIZE (16)
#define ISP_EXT_SYSTEM_CCM_COLORTONE_GGAIN_DEFAULT (256)

// args: data (16-bit)
static __inline GK_VOID ext_system_cc_colortone_ggain_write(VI_PIPE ViPipe,
							    GK_U16 data)
{
	IOWR_16DIRECT((VREG_AWB_BASE(ViPipe) + 0x24), data);
}

static __inline GK_U16 ext_system_cc_colortone_ggain_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_AWB_BASE(ViPipe) + 0x24);
}

#define ISP_EXT_SYSTEM_CCM_COLORTONE_BGAIN_DATASIZE (16)
#define ISP_EXT_SYSTEM_CCM_COLORTONE_BGAIN_DEFAULT (256)

// args: data (16-bit)
static __inline GK_VOID ext_system_cc_colortone_bgain_write(VI_PIPE ViPipe,
							    GK_U16 data)
{
	IOWR_16DIRECT((VREG_AWB_BASE(ViPipe) + 0x26), data);
}

static __inline GK_U16 ext_system_cc_colortone_bgain_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_AWB_BASE(ViPipe) + 0x26);
}

#define ISP_EXT_SYSTEM_WB_STATISTICS_MPI_UPDATE_EN_DATASIZE (8)
#define ISP_EXT_SYSTEM_WB_STATISTICS_MPI_UPDATE_EN_DEFAULT (0)

// args: data (8-bit)
static __inline GK_VOID
ext_system_wb_statistics_mpi_update_en_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT((VREG_AWB_BASE(ViPipe) + 0x28), data);
}

static __inline GK_U8
ext_system_wb_statistics_mpi_update_en_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_AWB_BASE(ViPipe) + 0x28);
}

// args: data (16-bit)
static __inline GK_VOID ext_system_awb_sta_enable_write(VI_PIPE ViPipe,
							GK_U8 data)
{
	IOWR_8DIRECT((VREG_AWB_BASE(ViPipe) + 0x2a), (data & 0x1));
}

static __inline GK_U8 ext_system_awb_sta_enable_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_AWB_BASE(ViPipe) + 0x2a) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_crop_en
// ------------------------------------------------------------------------------ //
// CROP Enable
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_AWB_CROP_EN_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_AWB_CROP_EN_DATASIZE (8)

// args: data (8-bit)
static __inline GK_VOID ext_system_awb_crop_en_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT((VREG_AWB_BASE(ViPipe) + 0x2c), data);
}
static __inline GK_U8 ext_system_awb_crop_en_read(VI_PIPE ViPipe)
{
	return IORD_8DIRECT(VREG_AWB_BASE(ViPipe) + 0x2c);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_awb_crop_x
// ------------------------------------------------------------------------------ //
// AE CROP X
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_AWB_CROP_X_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_AWB_CROP_X_DATASIZE (16)

// args: data (16-bit)
static __inline GK_VOID ext_system_awb_crop_x_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AWB_BASE(ViPipe) + 0x2e), data);
}
static __inline GK_U16 ext_system_awb_crop_x_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_AWB_BASE(ViPipe) + 0x2e);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_awb_crop_y
// ------------------------------------------------------------------------------ //
// AWB CROP Y
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_AWB_CROP_Y_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_AWB_CROP_Y_DATASIZE (16)

// args: data (16-bit)
static __inline GK_VOID ext_system_awb_crop_y_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT((VREG_AWB_BASE(ViPipe) + 0x30), data);
}
static __inline GK_U16 ext_system_awb_crop_y_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_AWB_BASE(ViPipe) + 0x30);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_awb_crop_height
// ------------------------------------------------------------------------------ //
// AWB CROP HEIGHT
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_AWB_CROP_HEIGHT_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_AWB_CROP_HEIGHT_DATASIZE (16)

// args: data (16-bit)
static __inline GK_VOID ext_system_awb_crop_height_write(VI_PIPE ViPipe,
							 GK_U16 data)
{
	IOWR_16DIRECT((VREG_AWB_BASE(ViPipe) + 0x32), data);
}
static __inline GK_U16 ext_system_awb_crop_height_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_AWB_BASE(ViPipe) + 0x32);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_awb_crop_width
// ------------------------------------------------------------------------------ //
// AWB CROP WIDTH
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_AWB_CROP_WIDTH_DEFAULT (0x0)
#define ISP_EXT_SYSTEM_AWB_CROP_WIDTH_DATASIZE (16)

// args: data (16-bit)
static __inline GK_VOID ext_system_awb_crop_width_write(VI_PIPE ViPipe,
							GK_U16 data)
{
	IOWR_16DIRECT((VREG_AWB_BASE(ViPipe) + 0x34), data);
}
static __inline GK_U16 ext_system_awb_crop_width_read(VI_PIPE ViPipe)
{
	return IORD_16DIRECT(VREG_AWB_BASE(ViPipe) + 0x34);
}

// ==============================================================//
// DETAIL: VREG_DE_BASE(ViPipe) ~ VREG_DE_BASE(ViPipe)  +
// ==============================================================//
// ------------------------------------------------------------------------------ //
// Register: ext_system_detail_enable_write
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
#define ISP_EXT_SYSTEM_DETAIL_ENABLE_DEFAULT (1)
#define ISP_EXT_SYSTEM_DETAIL_ENABLE_DATASIZE (8)
static __inline GK_VOID ext_system_detail_enable_write(VI_PIPE ViPipe,
						       GK_U8 data)
{
	IOWR_8DIRECT(VREG_DE_BASE(ViPipe), (data & 0x1));
}
static __inline GK_U8 ext_system_detail_enable_read(VI_PIPE ViPipe)
{
	return ((IORD_8DIRECT(VREG_DE_BASE(ViPipe))) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_detail_manual_mode_write
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
#define ISP_EXT_SYSTEM_DETAIL_MANU_MODE_DEFAULT (0)
#define ISP_EXT_SYSTEM_DETAIL_MANU_MODE_DATASIZE (8)
static __inline GK_VOID ext_system_detail_manual_mode_write(VI_PIPE ViPipe,
							    GK_U8 data)
{
	IOWR_8DIRECT(VREG_DE_BASE(ViPipe) + 0x1, (data & 0x1));
}
static __inline GK_U8 ext_system_detail_manual_mode_read(VI_PIPE ViPipe)
{
	return ((IORD_8DIRECT(VREG_DE_BASE(ViPipe) + 0x1)) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_detail_luma_gain_lut_write
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
#define ISP_EXT_SYSTEM_DETAIL_LUMA_GAIN_LUT_DEFAULT (1)
#define ISP_EXT_SYSTEM_DETAIL_LUMA_GAIN_LUT_DATASIZE (16)
static __inline GK_VOID
ext_system_detail_luma_gain_lut_write(VI_PIPE ViPipe, GK_U16 i, GK_U16 data)
{
	IOWR_16DIRECT(VREG_DE_BASE(ViPipe) + 0x2 + i * sizeof(GK_U16),
		      (data & 0x1ff)); // end 36 = 2 + 17x2
}
static __inline GK_U16 ext_system_detail_luma_gain_lut_read(VI_PIPE ViPipe,
							    GK_U16 i)
{
	return ((IORD_16DIRECT(VREG_DE_BASE(ViPipe) + 0x2 +
			       i * sizeof(GK_U16))) &
		0x1ff);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_detail_auto_global_gain
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
#define ISP_EXT_SYSTEM_DETAIL_AUTO_GLOBAL_GAIN_DEFAULT (200)
#define ISP_EXT_SYSTEM_DETAIL_AUTO_GLOBAL_GAIN_DATASIZE (16)
static __inline GK_VOID
ext_system_detail_auto_global_gain_write(VI_PIPE ViPipe, GK_U16 i, GK_U16 data)
{
	IOWR_16DIRECT(VREG_DE_BASE(ViPipe) + 0x28 + i * sizeof(GK_U16),
		      (data & 0x1ff)); // end 40 + 16x2
}
static __inline GK_U16 ext_system_detail_auto_global_gain_read(VI_PIPE ViPipe,
							       GK_U16 i)
{
	return ((IORD_16DIRECT(VREG_DE_BASE(ViPipe) + 0x28 +
			       i * sizeof(GK_U16))) &
		0x1ff);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_detail_auto_gain_lf
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
#define ISP_EXT_SYSTEM_DETAIL_AUTO_GAIN_LF_DEFAULT (12)
#define ISP_EXT_SYSTEM_DETAIL_AUTO_GAIN_LF_DATASIZE (16)
static __inline GK_VOID
ext_system_detail_auto_gain_lf_write(VI_PIPE ViPipe, GK_U16 i, GK_U16 data)
{
	IOWR_16DIRECT(VREG_DE_BASE(ViPipe) + 0x50 + i * sizeof(GK_U16),
		      (data & 0x1ff)); // end 80 + 16x2
}
static __inline GK_U16 ext_system_detail_auto_gain_lf_read(VI_PIPE ViPipe,
							   GK_U16 i)
{
	return ((IORD_16DIRECT(VREG_DE_BASE(ViPipe) + 0x50 +
			       i * sizeof(GK_U16))) &
		0x1ff);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_detail_auto_gain_hf
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
#define ISP_EXT_SYSTEM_DETAIL_AUTO_GAIN_HF_DEFAULT (12)
#define ISP_EXT_SYSTEM_DETAIL_AUTO_GAIN_HF_DATASIZE (16)
static __inline GK_VOID
ext_system_detail_auto_gain_hf_write(VI_PIPE ViPipe, GK_U16 i, GK_U16 data)
{
	IOWR_16DIRECT(VREG_DE_BASE(ViPipe) + 0x78 + i * sizeof(GK_U16),
		      (data & 0x1ff)); // end 120 + 16x2
}
static __inline GK_U16 ext_system_detail_auto_gain_hf_read(VI_PIPE ViPipe,
							   GK_U16 i)
{
	return ((IORD_16DIRECT(VREG_DE_BASE(ViPipe) + 0x78 +
			       i * sizeof(GK_U16))) &
		0x1ff);
}

// de manual start
// ------------------------------------------------------------------------------ //
// Register: ext_system_detail_manual_sgm_pos4_gain
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
// ------------------------------------------------------------------------------ //
// Register: ext_system_detail_manual_global_gain
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
#define ISP_EXT_SYSTEM_DETAIL_MANUAL_GLOBAL_GAIN_DEFAULT (200)
#define ISP_EXT_SYSTEM_DETAIL_MANUAL_GLOBAL_GAIN_DATASIZE (16)
static __inline GK_VOID
ext_system_detail_manual_global_gain_write(VI_PIPE ViPipe, GK_U16 data)
{
	IOWR_16DIRECT(VREG_DE_BASE(ViPipe) + 0xA0, (data & 0x1ff)); // 160
}
static __inline GK_U16 ext_system_detail_manual_global_gain_read(VI_PIPE ViPipe)
{
	return ((IORD_16DIRECT(VREG_DE_BASE(ViPipe) + 0xA0)) & 0x1ff);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_detail_manual_gain_lf
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
#define ISP_EXT_SYSTEM_DETAIL_MANUAL_GAIN_LF_DEFAULT (12)
#define ISP_EXT_SYSTEM_DETAIL_MANUAL_GAIN_LF_DATASIZE (16)
static __inline GK_VOID ext_system_detail_manual_gain_lf_write(VI_PIPE ViPipe,
							       GK_U16 data)
{
	IOWR_16DIRECT(VREG_DE_BASE(ViPipe) + 0xA2, (data & 0x1ff)); // 162
}
static __inline GK_U16 ext_system_detail_manual_gain_lf_read(VI_PIPE ViPipe)
{
	return ((IORD_16DIRECT(VREG_DE_BASE(ViPipe) + 0xA2)) & 0x1ff);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_detail_manual_gain_hf
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
#define ISP_EXT_SYSTEM_DETAIL_MANUAL_GAIN_HF_DEFAULT (12)
#define ISP_EXT_SYSTEM_DETAIL_MANUAL_GAIN_HF_DATASIZE (16)
static __inline GK_VOID ext_system_detail_manual_gain_hf_write(VI_PIPE ViPipe,
							       GK_U16 data)
{
	IOWR_16DIRECT(VREG_DE_BASE(ViPipe) + 0xA4, (data & 0x1ff)); // 164
}
static __inline GK_U16 ext_system_detail_manual_gain_hf_read(VI_PIPE ViPipe)
{
	return ((IORD_16DIRECT(VREG_DE_BASE(ViPipe) + 0xA4)) & 0x1ff);
}

// ------------------------------------------------------------------------------//

static __inline GK_VOID ext_system_detail_attr_update_write(VI_PIPE ViPipe,
							    GK_U8 data)
{
	IOWR_8DIRECT(VREG_DE_BASE(ViPipe) + 0xA6, (data & 0x1)); // 166
}
static __inline GK_U8 ext_system_detail_attr_update_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_DE_BASE(ViPipe) + 0xA6) & 0x1);
}

// args: data (8-bit)
static __inline GK_VOID ext_system_rgbir_enable_write(VI_PIPE ViPipe,
						      GK_U8 data)
{
	IOWR_8DIRECT(VREG_RGBIR_BASE(ViPipe), (data & 0x1));
}
static __inline GK_U8 ext_system_rgbir_enable_read(VI_PIPE ViPipe)
{
	return ((IORD_8DIRECT(VREG_RGBIR_BASE(ViPipe))) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_rgbir_th_write
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
#define ISP_RGBIR_THTV_DFT 100
static __inline GK_VOID ext_system_rgbir_th_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_RGBIR_BASE(ViPipe) + 0x1, (data & 0xff));
}
static __inline GK_U8 ext_system_rgbir_th_read(VI_PIPE ViPipe)
{
	return ((IORD_8DIRECT(VREG_RGBIR_BASE(ViPipe) + 0x1)) & 0xff);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_rgbir_tv_write
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_rgbir_tv_write(VI_PIPE ViPipe, GK_U8 data)
{
	IOWR_8DIRECT(VREG_RGBIR_BASE(ViPipe) + 0x2, (data & 0xff));
}
static __inline GK_U8 ext_system_rgbir_tv_read(VI_PIPE ViPipe)
{
	return ((IORD_8DIRECT(VREG_RGBIR_BASE(ViPipe) + 0x2)) & 0xff);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_rgbir_outpattern_write
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_rgbir_outpattern_write(VI_PIPE ViPipe,
							  GK_U8 data)
{
	IOWR_8DIRECT(VREG_RGBIR_BASE(ViPipe) + 0x3, (data & 0x3));
}
static __inline GK_U8 ext_system_rgbir_outpattern_read(VI_PIPE ViPipe)
{
	return ((IORD_8DIRECT(VREG_RGBIR_BASE(ViPipe) + 0x3)) & 0x3);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_rgbir_outpattern_write
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_rgbir_inpattern_write(VI_PIPE ViPipe,
							 GK_U8 data)
{
	IOWR_8DIRECT(VREG_RGBIR_BASE(ViPipe) + 0x4, (data & 0x7));
}
static __inline GK_U8 ext_system_rgbir_inpattern_read(VI_PIPE ViPipe)
{
	return ((IORD_8DIRECT(VREG_RGBIR_BASE(ViPipe) + 0x4)) & 0x7);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_rgbir_expctrl_write
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
static __inline GK_VOID ext_system_rgbir_expctrl_write(VI_PIPE ViPipe,
						       GK_U16 data, GK_U16 i)
{
	IOWR_16DIRECT(VREG_RGBIR_BASE(ViPipe) + 0x6 + i * sizeof(GK_U16),
		      (data & 0xffff));
}
static __inline GK_U16 ext_system_rgbir_expctrl_read(VI_PIPE ViPipe, GK_U16 i)
{
	return ((IORD_16DIRECT(VREG_RGBIR_BASE(ViPipe) + 0x6 +
			       i * sizeof(GK_U16))) &
		0xffff);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_rgbir_expctrl_write
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
static __inline GK_VOID ext_system_rgbir_gain_write(VI_PIPE ViPipe, GK_U16 data,
						    GK_U16 i)
{
	IOWR_16DIRECT(VREG_RGBIR_BASE(ViPipe) + 0xa + i * sizeof(GK_U16),
		      (data & 0xffff));
}
static __inline GK_U16 ext_system_rgbir_gain_read(VI_PIPE ViPipe, GK_U16 i)
{
	return ((IORD_16DIRECT(VREG_RGBIR_BASE(ViPipe) + 0xa +
			       i * sizeof(GK_U16))) &
		0xffff);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_rgbir_outpattern_write
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline GK_VOID ext_system_rgbir_irstatus_write(VI_PIPE ViPipe,
							GK_U8 data)
{
	IOWR_8DIRECT(VREG_RGBIR_BASE(ViPipe) + 0xe, (data & 0x3));
}
static __inline GK_U8 ext_system_rgbir_irstatus_read(VI_PIPE ViPipe)
{
	return ((IORD_8DIRECT(VREG_RGBIR_BASE(ViPipe) + 0xe)) & 0x3);
}

// ------------------------------------------------------------------------------ //
// Register: ext_system_rgbir_cvtmatrix_write
// ------------------------------------------------------------------------------ //
// args: data (16-bit, array)
#define ISP_RGBIR_CVTMATRIX_NUM (12)
static __inline GK_VOID ext_system_rgbir_cvtmatrix_write(VI_PIPE ViPipe,
							 GK_U16 data, GK_U16 i)
{
	IOWR_16DIRECT(VREG_RGBIR_BASE(ViPipe) + 0x10 + i * sizeof(GK_U16),
		      (data & 0xffff));
}
static __inline GK_U16 ext_system_rgbir_cvtmatrix_read(VI_PIPE ViPipe, GK_U16 i)
{
	return ((IORD_16DIRECT(VREG_RGBIR_BASE(ViPipe) + 0x10 +
			       i * sizeof(GK_U16))) &
		0xffff);
}

static __inline GK_VOID ext_system_rgbir_attr_update_write(VI_PIPE ViPipe,
							   GK_U8 data)
{
	IOWR_8DIRECT(VREG_RGBIR_BASE(ViPipe) + 0x20, (data & 0x1));
}
static __inline GK_U8 ext_system_rgbir_attr_update_read(VI_PIPE ViPipe)
{
	return (IORD_8DIRECT(VREG_RGBIR_BASE(ViPipe) + 0x20) & 0x1);
}
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __ISP_EXT_CONFIG_H__ */
