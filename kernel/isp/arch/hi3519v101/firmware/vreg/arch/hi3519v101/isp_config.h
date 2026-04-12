/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_config.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2015/05/15
  Description   : 
  History       :
  1.Date        : 2015/05/15
    Author      : 
    Modification: Created file

******************************************************************************/

#ifndef __ISP_CONFIG_H__
#define __ISP_CONFIG_H__

#include "hi_vreg.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

// ------------------------------------------------------------------------------ //
// Instance 'isp' of module 'ip_config_h'
// ------------------------------------------------------------------------------ //

#define HI_ISP_BASE_ADDR (0x000)
#define HI_ISP_SIZE      (0x2000)

#define HI_ISP_MAX_ADDR (4 * 0x1ffff)

// ------------------------------------------------------------------------------ //
// Group: ID
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_id_api
// ------------------------------------------------------------------------------ //

#define HI_ISP_ID_API_DEFAULT (0x0)
#define HI_ISP_ID_API_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_id_api_read(ISP_DEV IspDev) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_id_product
// ------------------------------------------------------------------------------ //

#define HI_ISP_ID_PRODUCT_DEFAULT (0x0)
#define HI_ISP_ID_PRODUCT_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_id_product_read(ISP_DEV IspDev) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_id_version
// ------------------------------------------------------------------------------ //

#define HI_ISP_ID_VERSION_DEFAULT (0x0)
#define HI_ISP_ID_VERSION_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_id_version_read(ISP_DEV IspDev) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x8);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_id_revision
// ------------------------------------------------------------------------------ //

#define HI_ISP_ID_REVISION_DEFAULT (0x0)
#define HI_ISP_ID_REVISION_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_id_revision_read(ISP_DEV IspDev) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xc);
}

// ------------------------------------------------------------------------------ //
// Group: Top
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Miscellaneous top-level ISP controls
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_active_width
// ------------------------------------------------------------------------------ //
// Active video width in pixels
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_ACTIVE_WIDTH_DEFAULT (0x780)
#define HI_ISP_TOP_ACTIVE_WIDTH_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_top_active_width_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x10);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x10, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_top_active_width_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x10) & 0xffff) >> 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_active_height
// ------------------------------------------------------------------------------ //
// Active video height in lines
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_ACTIVE_HEIGHT_DEFAULT (0x438)
#define HI_ISP_TOP_ACTIVE_HEIGHT_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_top_active_height_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x14);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x14, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_top_active_height_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x14) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_rggb_start
// ------------------------------------------------------------------------------ //
// Start color of the rggb pattern
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_RGGB_START_DEFAULT (0x0)
#define HI_ISP_TOP_RGGB_START_DATASIZE (2)
#define HI_ISP_TOP_RGGB_START_R_GR_GB_B (0)
#define HI_ISP_TOP_RGGB_START_GR_R_B_GB (1)
#define HI_ISP_TOP_RGGB_START_GB_B_R_GR (2)
#define HI_ISP_TOP_RGGB_START_B_GB_GR_R (3)

// args: data (2-bit)
static __inline HI_VOID hi_isp_top_rggb_start_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x18);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x18, (((HI_U32) (data & 0x3)) << 0) | (curr & 0xfffffffc));
}
static __inline HI_U8 hi_isp_top_rggb_start_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x18) & 0x3) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_channel1_select
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Source select for channel 1 through VTPG and Raw FE
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_CHANNEL1_SELECT_DEFAULT (0x0)
#define HI_ISP_TOP_CHANNEL1_SELECT_DATASIZE (2)
#define HI_ISP_TOP_CHANNEL1_SELECT_INPUT_CHANNEL_1 (0)
#define HI_ISP_TOP_CHANNEL1_SELECT_INPUT_CHANNEL_2 (1)
#define HI_ISP_TOP_CHANNEL1_SELECT_INPUT_CHANNEL_3 (2)
#define HI_ISP_TOP_CHANNEL1_SELECT_INPUT_CHANNEL_4 (3)


// args: data (2-bit)
static __inline HI_VOID hi_isp_top_channel1_select_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x20);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x20, (((HI_U32) (data & 0x3)) << 0) | (curr & 0xfffffffc));
}
static __inline HI_U8 hi_isp_top_channel1_select_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x20) & 0x3) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_channel2_select
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Source select for channel 2 through VTPG and Raw FE
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_CHANNEL2_SELECT_DEFAULT (0x1)
#define HI_ISP_TOP_CHANNEL2_SELECT_DATASIZE (2)
#define HI_ISP_TOP_CHANNEL2_SELECT_INPUT_CHANNEL_1 (0)
#define HI_ISP_TOP_CHANNEL2_SELECT_INPUT_CHANNEL_2 (1)
#define HI_ISP_TOP_CHANNEL2_SELECT_INPUT_CHANNEL_3 (2)
#define HI_ISP_TOP_CHANNEL2_SELECT_INPUT_CHANNEL_4 (3)

// args: data (2-bit)
static __inline HI_VOID hi_isp_top_channel2_select_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x20);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x20, (((HI_U32) (data & 0x3)) << 2) | (curr & 0xfffffff3));
}
static __inline HI_U8 hi_isp_top_channel2_select_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x20) & 0xc) >> 2);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_channel3_select
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Source select for channel 3 through VTPG and Raw FE
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_CHANNEL3_SELECT_DEFAULT (0x2)
#define HI_ISP_TOP_CHANNEL3_SELECT_DATASIZE (2)
#define HI_ISP_TOP_CHANNEL3_SELECT_INPUT_CHANNEL_1 (0)
#define HI_ISP_TOP_CHANNEL3_SELECT_INPUT_CHANNEL_2 (1)
#define HI_ISP_TOP_CHANNEL3_SELECT_INPUT_CHANNEL_3 (2)
#define HI_ISP_TOP_CHANNEL3_SELECT_INPUT_CHANNEL_4 (3)

// args: data (2-bit)
static __inline HI_VOID hi_isp_top_channel3_select_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x20);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x20, (((HI_U32) (data & 0x3)) << 4) | (curr & 0xffffffcf));
}
static __inline HI_U8 hi_isp_top_channel3_select_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x20) & 0x30) >> 4);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_channel4_select
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Source select for channel 4 through VTPG and Raw FE
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_CHANNEL4_SELECT_DEFAULT (0x3)
#define HI_ISP_TOP_CHANNEL4_SELECT_DATASIZE (2)
#define HI_ISP_TOP_CHANNEL4_SELECT_INPUT_CHANNEL_1 (0)
#define HI_ISP_TOP_CHANNEL4_SELECT_INPUT_CHANNEL_2 (1)
#define HI_ISP_TOP_CHANNEL4_SELECT_INPUT_CHANNEL_3 (2)
#define HI_ISP_TOP_CHANNEL4_SELECT_INPUT_CHANNEL_4 (3)

// args: data (2-bit)
static __inline HI_VOID hi_isp_top_channel4_select_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x20);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x20, (((HI_U32) (data & 0x3)) << 6) | (curr & 0xffffff3f));
}
static __inline HI_U8 hi_isp_top_channel4_select_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x20) & 0xc0) >> 6);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_channel1_select
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Source select for channel 1 into Frame Stitch
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_FS_CHANNEL1_SELECT_DEFAULT (0x0)
#define HI_ISP_TOP_FS_CHANNEL1_SELECT_DATASIZE (2)
#define HI_ISP_TOP_FS_CHANNEL1_SELECT_FRONTEND_CHANNEL_1 (0)
#define HI_ISP_TOP_FS_CHANNEL1_SELECT_FRONTEND_CHANNEL_2 (1)
#define HI_ISP_TOP_FS_CHANNEL1_SELECT_FRONTEND_CHANNEL_3 (2)
#define HI_ISP_TOP_FS_CHANNEL1_SELECT_FRONTEND_CHANNEL_4 (3)

// args: data (2-bit)
static __inline HI_VOID hi_isp_top_fs_channel1_select_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c, (((HI_U32) (data & 0x3)) << 0) | (curr & 0xfffffffc));
}
static __inline HI_U8 hi_isp_top_fs_channel1_select_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c) & 0x3) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_channel2_select
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Source select for channel 2 into Frame Stitch
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_FS_CHANNEL2_SELECT_DEFAULT (0x1)
#define HI_ISP_TOP_FS_CHANNEL2_SELECT_DATASIZE (2)
#define HI_ISP_TOP_FS_CHANNEL2_SELECT_FRONTEND_CHANNEL_1 (0)
#define HI_ISP_TOP_FS_CHANNEL2_SELECT_FRONTEND_CHANNEL_2 (1)
#define HI_ISP_TOP_FS_CHANNEL2_SELECT_FRONTEND_CHANNEL_3 (2)
#define HI_ISP_TOP_FS_CHANNEL2_SELECT_FRONTEND_CHANNEL_4 (3)

// args: data (2-bit)
static __inline HI_VOID hi_isp_top_fs_channel2_select_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c, (((HI_U32) (data & 0x3)) << 2) | (curr & 0xfffffff3));
}
static __inline HI_U8 hi_isp_top_fs_channel2_select_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c) & 0xc) >> 2);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_channel3_select
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Source select for channel 3 into Frame Stitch
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_FS_CHANNEL3_SELECT_DEFAULT (0x2)
#define HI_ISP_TOP_FS_CHANNEL3_SELECT_DATASIZE (2)
#define HI_ISP_TOP_FS_CHANNEL3_SELECT_FRONTEND_CHANNEL_1 (0)
#define HI_ISP_TOP_FS_CHANNEL3_SELECT_FRONTEND_CHANNEL_2 (1)
#define HI_ISP_TOP_FS_CHANNEL3_SELECT_FRONTEND_CHANNEL_3 (2)
#define HI_ISP_TOP_FS_CHANNEL3_SELECT_FRONTEND_CHANNEL_4 (3)

// args: data (2-bit)
static __inline HI_VOID hi_isp_top_fs_channel3_select_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c, (((HI_U32) (data & 0x3)) << 4) | (curr & 0xffffffcf));
}
static __inline HI_U8 hi_isp_top_fs_channel3_select_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c) & 0x30) >> 4);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_channel4_select
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Source select for channel 4 into Frame Stitch
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_FS_CHANNEL4_SELECT_DEFAULT (0x3)
#define HI_ISP_TOP_FS_CHANNEL4_SELECT_DATASIZE (2)
#define HI_ISP_TOP_FS_CHANNEL4_SELECT_FRONTEND_CHANNEL_1 (0)
#define HI_ISP_TOP_FS_CHANNEL4_SELECT_FRONTEND_CHANNEL_2 (1)
#define HI_ISP_TOP_FS_CHANNEL4_SELECT_FRONTEND_CHANNEL_3 (2)
#define HI_ISP_TOP_FS_CHANNEL4_SELECT_FRONTEND_CHANNEL_4 (3)

// args: data (2-bit)
static __inline HI_VOID hi_isp_top_fs_channel4_select_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c, (((HI_U32) (data & 0x3)) << 6) | (curr & 0xffffff3f));
}
static __inline HI_U8 hi_isp_top_fs_channel4_select_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c) & 0xc0) >> 6);
}
// ------------------------------------------------------------------------------ //
// Register: Delay line memory configuration
// ------------------------------------------------------------------------------ //
// Memory configuration of nr delay lines.
// 00 = Maximum number of lines, shortest line length.
// 01 = 3/4 of maximum lines, 4/3 x shortest line.
// 1x = 1/2 of maximum lines, 2 x shortest line.
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_DELAY_LINE_MEMORY_CONFIGURATION_DEFAULT (0x2)
#define HI_ISP_TOP_DELAY_LINE_MEMORY_CONFIGURATION_DATASIZE (2)

// args: data (2-bit)
static __inline HI_VOID hi_isp_top_delay_line_memory_configuration_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x24);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x24, (((HI_U32) (data & 0x3)) << 0) | (curr & 0xfffffffc));
}
static __inline HI_U8 hi_isp_top_delay_line_memory_configuration_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x24) & 0x3) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_flush_hblank
// ------------------------------------------------------------------------------ //
// Horizontal blanking interval during regeneration (0=measured input interval)
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_DELAY_LINE_REGEN_HBLANK_DEFAULT (0x20)
#define HI_ISP_TOP_DELAY_LINE_REGEN_HBLANK_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_top_delay_line_regen_hblank_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x0028);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x0028, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}

static __inline HI_VOID hi_isp_top_delay_line_ispbe_hblank_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x201e8);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x201e8, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}

static __inline HI_U16 hi_isp_top_delay_line_regen_hblank_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x0028) & 0xffff) >> 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_delay_line_jitter_correction
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Select ISP configuration double-buffering mode
//   0 = Disabled (config updates immediately)
//   1 = Blocked (config never updates)
//   2 = Local (module config updates during local vertical blanking)
//   3 = Global (all module config updated during ISP vertical blanking)
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_CONFIG_BUFFER_MODE_DEFAULT (0)
#define HI_ISP_TOP_CONFIG_BUFFER_MODE_DATASIZE (2)
#define HI_ISP_TOP_CONFIG_BUFFER_MODE_DISABLED_CONFIG_UPDATES_IMMEDIATELY (0)
#define HI_ISP_TOP_CONFIG_BUFFER_MODE_BLOCKED_CONFIG_NEVER_UPDATES (1)
#define HI_ISP_TOP_CONFIG_BUFFER_MODE_LOCAL_MODULE_CONFIG_UPDATES_DURING_LOCAL_VERTICAL_BLANKING (2)
#define HI_ISP_TOP_CONFIG_BUFFER_MODE_GLOBAL_ALL_MODULE_CONFIG_UPDATED_DURING_ISP_VERTICAL_BLANKING (3)

// args: data (2-bit)
static __inline HI_VOID hi_isp_top_config_buffer_mode_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x30);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x30, (((HI_U32) (data & 0x3)) << 0) | (curr & 0xfffffffc));
}
static __inline HI_U8 hi_isp_top_config_buffer_mode_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x30) & 0x3) >> 0);
}



// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_position_gamma_fe
// ------------------------------------------------------------------------------ //
// Position of front end gamma LUTs and digital gain block.
// ------------------------------------------------------------------------------ //
#define HI_ISP_TOP_GAMMA_FE_POSITION_DEFAULT (0)
#define HI_ISP_TOP_GAMMA_FE_POSITION_DATASIZE (1)
#define HI_ISP_TOP_POSITION_GAMMA_FE_AFTER_STITCHING (0)
#define HI_ISP_TOP_POSITION_GAMMA_FE_START_OF_PIPELINE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_gamma_fe_position_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40, (((HI_U32) (data & 0x1)) << 0) | (curr & 0xfffffffe));
}
static __inline HI_U8 hi_isp_top_gamma_fe_position_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40) & 0x1) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_position_digital_gain
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Position of digital gain block wrt. FE gamma LUTs.
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_POSITION_DIGITAL_GAIN_DEFAULT (0)
#define HI_ISP_TOP_POSITION_DIGITAL_GAIN_DATASIZE (1)
#define HI_ISP_TOP_POSITION_DIGITAL_GAIN_AFTER_FE_GAMMA_LUTS (0)
#define HI_ISP_TOP_POSITION_DIGITAL_GAIN_BEFORE_FE_GAMMA_LUTS (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_position_digital_gain_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40, (((HI_U32) (data & 0x1)) << 1) | (curr & 0xfffffffd));
}
static __inline HI_U8 hi_isp_top_position_digital_gain_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40) & 0x2) >> 1);
}
// ------------------------------------------------------------------------------ //
// Register: CLK Dis frontend 3ch
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 
//            when set to '1' This will clock gate 3 raw frontend units along with their delay lines. This will also clock gate the 
//            frame stitch and all the 4 delay lines
//          
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_CLK_DIS_FRONTEND_3CH_DEFAULT (0)
#define HI_ISP_TOP_CLK_DIS_FRONTEND_3CH_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_clk_dis_frontend_3ch_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x34);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x34, (((HI_U32) (data & 0x1)) << 0) | (curr & 0xfffffffe));
}
static __inline HI_U8 hi_isp_top_clk_dis_frontend_3ch_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x34) & 0x1) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: CLK Dis hist PE 3ch
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// when set, this will disable clocks for 3 of the 4 channels (256bin) histogram.
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_CLK_DIS_HIST_PE_3CH_DEFAULT (0)
#define HI_ISP_TOP_CLK_DIS_HIST_PE_3CH_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_clk_dis_hist_pe_3ch_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x34);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x34, (((HI_U32) (data & 0x1)) << 1) | (curr & 0xfffffffd));
}
static __inline HI_U8 hi_isp_top_clk_dis_hist_pe_3ch_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x34) & 0x2) >> 1);
}
// ------------------------------------------------------------------------------ //
// Register: CLK Dis hist 1024bin
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// when set, this will disable clock for 1024 bin the histograms.
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_CLK_DIS_HIST_1024BIN_DEFAULT (0)
#define HI_ISP_TOP_CLK_DIS_HIST_1024BIN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_clk_dis_hist_1024bin_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x34);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x34, (((HI_U32) (data & 0x1)) << 2) | (curr & 0xfffffffb));
}
static __inline HI_U8 hi_isp_top_clk_dis_hist_1024bin_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x34) & 0x4) >> 2);
}
// ------------------------------------------------------------------------------ //
// Register: CLK Dis iridix
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// when set, this will disable clock for Iridix.
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_CLK_DIS_IRIDIX_DEFAULT (0)
#define HI_ISP_TOP_CLK_DIS_IRIDIX_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_clk_dis_iridix_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x34);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x34, (((HI_U32) (data & 0x1)) << 3) | (curr & 0xfffffff7));
}
static __inline HI_U8 hi_isp_top_clk_dis_iridix_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x34) & 0x8) >> 3);
}
// ------------------------------------------------------------------------------ //
// Register: CLK Dis radial shading
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// when set, this will disable clock for radial shading.
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_CLK_DIS_RADIAL_SHADING_DEFAULT (0)
#define HI_ISP_TOP_CLK_DIS_RADIAL_SHADING_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_clk_dis_radial_shading_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x34);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x34, (((HI_U32) (data & 0x1)) << 4) | (curr & 0xffffffef));
}
static __inline HI_U8 hi_isp_top_clk_dis_radial_shading_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x34) & 0x10) >> 4);
}
// ------------------------------------------------------------------------------ //
// Register: CLK Dis mesh shading
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// when set, this will disable clock for mesh shading.
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_CLK_DIS_MESH_SHADING_DEFAULT (0)
#define HI_ISP_TOP_CLK_DIS_MESH_SHADING_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_clk_dis_mesh_shading_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x34);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x34, (((HI_U32) (data & 0x1)) << 5) | (curr & 0xffffffdf));
}
static __inline HI_U8 hi_isp_top_clk_dis_mesh_shading_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x34) & 0x20) >> 5);
}
// ------------------------------------------------------------------------------ //
// Register: CLK Dis AWB
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// when set, this will disable clock for AWB.
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_CLK_DIS_AWB_DEFAULT (0)
#define HI_ISP_TOP_CLK_DIS_AWB_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_clk_dis_awb_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x34);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x34, (((HI_U32) (data & 0x1)) << 6) | (curr & 0xffffffbf));
}
static __inline HI_U8 hi_isp_top_clk_dis_awb_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x34) & 0x40) >> 6);
}
// ------------------------------------------------------------------------------ //
// Register: CG Dis frontend 3ch
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// when set, this will disable Clock gating of 3 channels in the frontend.
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_CG_DIS_FRONTEND_3CH_DEFAULT (0)
#define HI_ISP_TOP_CG_DIS_FRONTEND_3CH_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_cg_dis_frontend_3ch_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x38);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x38, (((HI_U32) (data & 0x1)) << 0) | (curr & 0xfffffffe));
}
static __inline HI_U8 hi_isp_top_cg_dis_frontend_3ch_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x38) & 0x1) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: CG Dis hist pe 3ch
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// when set, this will disable Clock gating of 3 channels of the histogram.
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_CG_DIS_HIST_PE_3CH_DEFAULT (0)
#define HI_ISP_TOP_CG_DIS_HIST_PE_3CH_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_cg_dis_hist_pe_3ch_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x38);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x38, (((HI_U32) (data & 0x1)) << 1) | (curr & 0xfffffffd));
}
static __inline HI_U8 hi_isp_top_cg_dis_hist_pe_3ch_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x38) & 0x2) >> 1);
}
// ------------------------------------------------------------------------------ //
// Register: CG Dis hist 1024bin
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// when set, this will disable Clock gating of all the histograms.
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_CG_DIS_HIST_1024BIN_DEFAULT (0)
#define HI_ISP_TOP_CG_DIS_HIST_1024BIN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_cg_dis_hist_1024bin_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x38);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x38, (((HI_U32) (data & 0x1)) << 2) | (curr & 0xfffffffb));
}
static __inline HI_U8 hi_isp_top_cg_dis_hist_1024bin_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x38) & 0x4) >> 2);
}
// ------------------------------------------------------------------------------ //
// Register: CG Dis iridix
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// when set, this will disable Clock gating for Iridix.
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_CG_DIS_IRIDIX_DEFAULT (0)
#define HI_ISP_TOP_CG_DIS_IRIDIX_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_cg_dis_iridix_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x38);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x38, (((HI_U32) (data & 0x1)) << 3) | (curr & 0xfffffff7));
}
static __inline HI_U8 hi_isp_top_cg_dis_iridix_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x38) & 0x8) >> 3);
}
// ------------------------------------------------------------------------------ //
// Register: CG Dis radial shading
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// when set, this will disable Clock gating for Radial shading.
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_CG_DIS_RADIAL_SHADING_DEFAULT (0)
#define HI_ISP_TOP_CG_DIS_RADIAL_SHADING_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_cg_dis_radial_shading_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x38);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x38, (((HI_U32) (data & 0x1)) << 4) | (curr & 0xffffffef));
}
static __inline HI_U8 hi_isp_top_cg_dis_radial_shading_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x38) & 0x10) >> 4);
}
// ------------------------------------------------------------------------------ //
// Register: CG Dis mesh shading
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// when set, this will disable Clock gating for mesh shading.
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_CG_DIS_MESH_SHADING_DEFAULT (0)
#define HI_ISP_TOP_CG_DIS_MESH_SHADING_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_cg_dis_mesh_shading_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x38);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x38, (((HI_U32) (data & 0x1)) << 5) | (curr & 0xffffffdf));
}
static __inline HI_U8 hi_isp_top_cg_dis_mesh_shading_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x38) & 0x20) >> 5);
}
// ------------------------------------------------------------------------------ //
// Register: CG Dis AWB
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// when set, this will disable Clock gating for AWB.
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_CG_DIS_AWB_DEFAULT (0)
#define HI_ISP_TOP_CG_DIS_AWB_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_cg_dis_awb_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x38);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x38, (((HI_U32) (data & 0x1)) << 6) | (curr & 0xffffffbf));
}
static __inline HI_U8 hi_isp_top_cg_dis_awb_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x38) & 0x40) >> 6);
}
// ------------------------------------------------------------------------------ //
// Register: disable clk immediately
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 
//             1: this will disable Clock in all the clock gate cells whenever the condition is true.
//             0: this will disable Clock in the next V-blank whenever the condition is true.
//          
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_DISABLE_CLK_IMMEDIATELY_DEFAULT (1)
#define HI_ISP_TOP_DISABLE_CLK_IMMEDIATELY_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_disable_clk_immediately_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3c, (((HI_U32) (data & 0x1)) << 0) | (curr & 0xfffffffe));
}
static __inline HI_U8 hi_isp_top_disable_clk_immediately_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3c) & 0x1) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Bypass input formatter
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Bypass input formatter module.
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_INPUT_FORMATTER_DEFAULT (0)
#define HI_ISP_TOP_BYPASS_INPUT_FORMATTER_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_input_formatter_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40, (((HI_U32) (data & 0x1)) << 2) | (curr & 0xfffffffb));
}
static __inline HI_U8 hi_isp_top_bypass_input_formatter_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40) & 0x4) >> 2);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_channel_switch
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Bypass channel switch module
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_CHANNEL_SWITCH_DEFAULT (0)
#define HI_ISP_TOP_BYPASS_CHANNEL_SWITCH_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_channel_switch_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40, (((HI_U32) (data & 0x1)) << 3) | (curr & 0xfffffff7));
}
static __inline HI_U8 hi_isp_top_bypass_channel_switch_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40) & 0x8) >> 3);
}

#define HI_ISP_TOP_BYPASS_FLAG_DEFAULT (0x40000)
#define HI_ISP_TOP_BYPASS_FlAG_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_isp_top_bypass_flag_write(ISP_DEV IspDev, HI_U32 data) {
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x0040, data);
}
static __inline HI_U32 hi_isp_top_bypass_flag_read(ISP_DEV IspDev) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x0040);
}

#define HI_ISP_TOP_BYPASS_DEBUGER_DEFAULT (1)
#define HI_ISP_TOP_BYPASS_DEBUGER_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_debuger_write(ISP_DEV IspDev, HI_U8 data) {
    //HI_U8 u8Current = IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x0042);
    //IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x0042, ((data & 0x01) << 2) | (u8Current & 0xFB));
}
static __inline HI_U8 hi_isp_top_bypass_debuger_read(ISP_DEV IspDev) {
return 0;
    //return ((IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x0042) & 0x04) >> 2);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_video_test_gen
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Bypass video test generator
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_flag
// ------------------------------------------------------------------------------ //
// Top Bypass Flag
// ------------------------------------------------------------------------------ //
#define HI_ISP_TOP_BYPASS_VIDEO_TEST_GEN_DEFAULT (0)
#define HI_ISP_TOP_BYPASS_VIDEO_TEST_GEN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_video_test_gen_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40, (((HI_U32) (data & 0x1)) << 4) | (curr & 0xffffffef));
}
static __inline HI_U8 hi_isp_top_bypass_video_test_gen_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40) & 0x10) >> 4);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_RAW_frontend
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Bypass RAW frontend (green equalization and dynamic defect pixel)
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_GREEN_EQUALIZE_DEFAULT (0)
#define HI_ISP_TOP_BYPASS_GREEN_EQUALIZE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_green_equalize_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40, (((HI_U32) (data & 0x1)) << 5) | (curr & 0xffffffdf));
}
static __inline HI_U8 hi_isp_top_bypass_green_equalize_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40) & 0x20) >> 5);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_defect_pixel
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Bypass static defect pixel
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_DEFECT_PIXEL_DEFAULT (0)
#define HI_ISP_TOP_BYPASS_DEFECT_PIXEL_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_defect_pixel_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40, (((HI_U32) (data & 0x1)) << 6) | (curr & 0xffffffbf));
}
static __inline HI_U8 hi_isp_top_bypass_defect_pixel_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40) & 0x40) >> 6);
}

// ------------------------------------------------------------------------------ //
// Register: Bypass frame stitch
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Bypass frame stitching logic
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_FRAME_STITCH_WDR_DEFAULT (0)
#define HI_ISP_TOP_BYPASS_FRAME_STITCH_WDR_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_frame_stitch_wdr_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40, (((HI_U32) (data & 0x1)) << 7) | (curr & 0xffffff7f));
}
static __inline HI_U8 hi_isp_top_bypass_frame_stitch_wdr_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40) & 0x80) >> 7);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_gamma_fe
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Bypass front end gamma block
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_GAMMA_FE_DEFAULT (0)
#define HI_ISP_TOP_BYPASS_GAMMA_FE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_gamma_fe_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40, (((HI_U32) (data & 0x1)) << 8) | (curr & 0xfffffeff));
}
static __inline HI_U8 hi_isp_top_bypass_gamma_fe_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40) & 0x100) >> 8);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_gamma_fe_dl
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Bypass front end gamma block
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_GAMMA_FE_DL_DEFAULT (1)
#define HI_ISP_TOP_BYPASS_GAMMA_FE_DL_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_gamma_fe_dl_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40, (((HI_U32) (data & 0x1)) << 9) | (curr & 0xfffffdff));
}
static __inline HI_U8 hi_isp_top_bypass_gamma_fe_dl_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40) & 0x200) >> 9);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_digital_gain
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Bypass digital gain module
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_DIGITAL_GAIN_DEFAULT (0)
#define HI_ISP_TOP_BYPASS_DIGITAL_GAIN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_digital_gain_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40, (((HI_U32) (data & 0x1)) << 10) | (curr & 0xfffffbff));
}
static __inline HI_U8 hi_isp_top_bypass_digital_gain_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40) & 0x400) >> 10);
}
// ------------------------------------------------------------------------------ //
// Register: Bypass sinter
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Bypass nr 
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_fs_channel_switch
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Bypass fs_channel_switch 
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_FS_CHANNEL_SWITCH_DEFAULT (0)
#define HI_ISP_TOP_BYPASS_FS_CHANNEL_SWITCH_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_fs_channel_switch_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x0040);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x0040, (((HI_U32) (data & 0x1)) << 11) | (curr & 0xfffff7ff));
}
static __inline HI_U8 hi_isp_top_bypass_fs_channel_switch_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x0040) & 0x800) >> 11);
}

#define HI_ISP_TOP_BYPASS_NR_DEFAULT (0)
#define HI_ISP_TOP_BYPASS_NR_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_nr_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x0040);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x0040, (((HI_U32) (data & 0x1)) << 12) | (curr & 0xffffefff));
}
static __inline HI_U8 hi_isp_top_bypass_nr_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x0040) & 0x1000) >> 12);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_gamma_be
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Bypass back end end gamma LUTs
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_GAMMA_BE_DEFAULT (0)
#define HI_ISP_TOP_BYPASS_GAMMA_BE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_gamma_be_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40, (((HI_U32) (data & 0x1)) << 13) | (curr & 0xffffdfff));
}
static __inline HI_U8 hi_isp_top_bypass_gamma_be_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40) & 0x2000) >> 13);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_gamma_be_dl
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Bypass back end gamma block
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_GAMMA_BE_DL_DEFAULT (1)
#define HI_ISP_TOP_BYPASS_GAMMA_BE_DL_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_gamma_be_dl_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40, (((HI_U32) (data & 0x1)) << 14) | (curr & 0xffffbfff));
}
static __inline HI_U8 hi_isp_top_bypass_gamma_be_dl_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40) & 0x4000) >> 14);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_white_balance
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Bypass static white balance
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_WHITE_BALANCE_DEFAULT (0)
#define HI_ISP_TOP_BYPASS_WHITE_BALANCE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_white_balance_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40, (((HI_U32) (data & 0x1)) << 15) | (curr & 0xffff7fff));
}
static __inline HI_U8 hi_isp_top_bypass_white_balance_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40) & 0x8000) >> 15);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_radial_shading
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Bypass Radial Shading 
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_RADIAL_SHADING_DEFAULT (0)
#define HI_ISP_TOP_BYPASS_RADIAL_SHADING_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_radial_shading_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40, (((HI_U32) (data & 0x1)) << 16) | (curr & 0xfffeffff));
}
static __inline HI_U8 hi_isp_top_bypass_radial_shading_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40) & 0x10000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_mesh_shading
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Bypass Mesh Shading 
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_MESH_SHADING_DEFAULT (0)
#define HI_ISP_TOP_BYPASS_MESH_SHADING_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_mesh_shading_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40, (((HI_U32) (data & 0x1)) << 17) | (curr & 0xfffdffff));
}
static __inline HI_U8 hi_isp_top_bypass_mesh_shading_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40) & 0x20000) >> 17);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_drc
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Bypass DRC 
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_DRC_DEFAULT (0)
#define HI_ISP_TOP_BYPASS_DRC_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_drc_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x0040);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x0040, (((HI_U32) (data & 0x1)) << 18) | (curr & 0xfffbffff));
}
static __inline HI_U8 hi_isp_top_bypass_drc_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x0040) & 0x40000) >> 18);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_demosaic
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Bypass demosaic module (output RAW data)
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_DEMOSAIC_DEFAULT (0)
#define HI_ISP_TOP_BYPASS_DEMOSAIC_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_demosaic_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40, (((HI_U32) (data & 0x1)) << 19) | (curr & 0xfff7ffff));
}
static __inline HI_U8 hi_isp_top_bypass_demosaic_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40) & 0x80000) >> 19);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_color_matrix
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Bypass color matrix
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_COLOR_MATRIX_DEFAULT (0)
#define HI_ISP_TOP_BYPASS_COLOR_MATRIX_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_color_matrix_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40, (((HI_U32) (data & 0x1)) << 20) | (curr & 0xffefffff));
}
static __inline HI_U8 hi_isp_top_bypass_color_matrix_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40) & 0x100000) >> 20);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_gamma_rgb
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Bypass RGB gamma LUT
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_GAMMA_RGB_DEFAULT (0)
#define HI_ISP_TOP_BYPASS_GAMMA_RGB_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_gamma_rgb_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40, (((HI_U32) (data & 0x1)) << 21) | (curr & 0xffdfffff));
}
static __inline HI_U8 hi_isp_top_bypass_gamma_rgb_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40) & 0x200000) >> 21);
}
// ------------------------------------------------------------------------------ //
// Register: Bypass crop
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_SHARPEN_DEFAULT (1)
#define HI_ISP_TOP_BYPASS_SHARPEN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_sharpen_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x0040);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x0040, (((HI_U32) (data & 0x1)) << 23) | (curr & 0xff7fffff));
}
static __inline HI_U8 hi_isp_top_bypass_sharpen_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x0040) & 0x800000) >> 23);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_crop
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Bypass cropping block
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_CROP_DEFAULT (0)
#define HI_ISP_TOP_BYPASS_CROP_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_crop_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40, (((HI_U32) (data & 0x1)) << 24) | (curr & 0xfeffffff));
}
static __inline HI_U8 hi_isp_top_bypass_crop_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40) & 0x1000000) >> 24);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_low_power_consumption_config
// ------------------------------------------------------------------------------ //
// 
// ------------------------------------------------------------------------------ //
#define HI_ISP_LOW_POWER_CONSUMPTION_CONFIG_DEFAULT (0x0)
#define HI_ISP_LOW_POWER_CONSUMPTION_CONFIG_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_isp_low_power_consumption_config_write(ISP_DEV IspDev, HI_U32 data) {
    //IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x0058, data);
}
static __inline HI_U8 hi_isp_low_power_consumption_config_read(ISP_DEV IspDev) {
return 0;
    //return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x0058);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_logo
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Bypass Logo 
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_LOGO_DEFAULT (1)
#define HI_ISP_TOP_BYPASS_LOGO_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_logo_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40, (((HI_U32) (data & 0x1)) << 25) | (curr & 0xfdffffff));
}
static __inline HI_U8 hi_isp_top_bypass_logo_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40) & 0x2000000) >> 25);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_cs_conv
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Bypass Colour Space Conversion
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_CS_CONV_DEFAULT (0)
#define HI_ISP_TOP_BYPASS_CS_CONV_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_cs_conv_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40, (((HI_U32) (data & 0x1)) << 26) | (curr & 0xfbffffff));
}
static __inline HI_U8 hi_isp_top_bypass_cs_conv_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40) & 0x4000000) >> 26);
}


// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_sensor_offset
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Bypass sensor offset
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_SENSOR_OFFSET_DEFAULT (0)
#define HI_ISP_TOP_BYPASS_SENSOR_OFFSET_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_sensor_offset_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40, (((HI_U32) (data & 0x1)) << 27) | (curr & 0xf7ffffff));
}
static __inline HI_U8 hi_isp_top_bypass_sensor_offset_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40) & 0x8000000) >> 27);
}


// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_sensor_offset_wdr
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Bypass sensor offset WDR 
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_SENSOR_OFFSET_WDR_DEFAULT (0)
#define HI_ISP_TOP_BYPASS_SENSOR_OFFSET_WDR_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_sensor_offset_wdr_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40, (((HI_U32) (data & 0x1)) << 28) | (curr & 0xefffffff));
}
static __inline HI_U8 hi_isp_top_bypass_sensor_offset_wdr_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40) & 0x10000000) >> 28);
}
// ------------------------------------------------------------------------------ //
// Register: Bypass WDR Gain
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Bypass WDR gain adjustment 
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_WDR_GAIN_DEFAULT (0)
#define HI_ISP_TOP_BYPASS_WDR_GAIN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_wdr_gain_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40, (((HI_U32) (data & 0x1)) << 29) | (curr & 0xdfffffff));
}
static __inline HI_U8 hi_isp_top_bypass_wdr_gain_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40) & 0x20000000) >> 29);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_isp_raw_bypass
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Used to select between normal ISP processing with image sensor data and up to 12 bit RGB or YUV422 input. In the latler case data is reinserted into pipeline after demosaic block. Crop, Scaling, Gamma and color space conversion are available for RGB input only.
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_ISP_RAW_BYPASS_DEFAULT (0)
#define HI_ISP_TOP_ISP_RAW_BYPASS_DATASIZE (1)
#define HI_ISP_TOP_ISP_RAW_BYPASS_SELECT_PROCESSED (0)
#define HI_ISP_TOP_ISP_RAW_BYPASS_BYPASS_ISP_RAW_PROCESSING (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_isp_raw_bypass_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x44);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x44, (((HI_U32) (data & 0x1)) << 9) | (curr & 0xfffffdff));
}
static __inline HI_U8 hi_isp_top_isp_raw_bypass_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x44) & 0x200) >> 9);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_isp_processing_bypass_mode
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
//  
// ISP bypass modes.  For debug purposes only. Should be set to 0 during normal operation. 
// Used to bypass entire ISP after input port or to pass the stitched image directly to the output.
//            
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_ISP_PROCESSING_BYPASS_MODE_DEFAULT (0)
#define HI_ISP_TOP_ISP_PROCESSING_BYPASS_MODE_DATASIZE (3)
#define HI_ISP_TOP_ISP_PROCESSING_BYPASS_MODE_FULL_PROCESSING (0)
#define HI_ISP_TOP_ISP_PROCESSING_BYPASS_MODE_BYPASS_ENTIRE_ISP_PROCESSING_AND_OUTPUT_MOST_SIGNIFICANT_BITS_OF_RAW_SENSOR_DATA_IN_IN_ONE_FE_PATH_ON_ALL_CHANNELS (1)
#define HI_ISP_TOP_ISP_PROCESSING_BYPASS_MODE_BYPASS_ENTIRE_ISP_PROCESSING_AND_OUTPUT_RAW_SENSOR_DATA_IN_CHANNELS_1_AND_2_6_BITS_IN_EACH (2)
#define HI_ISP_TOP_ISP_PROCESSING_BYPASS_MODE_BYPASS_ENTIRE_ISP_PROCESSING_AND_OUTPUT_8_BIT_RGB_INPUT_DATA (3)
#define HI_ISP_TOP_ISP_PROCESSING_BYPASS_MODE_BYPASS_ENTIRE_ISP_PROCESSING_AND_OUTPUT_24_MOST_SIGNIFICANT_BITS_OF_INPUT_DATA_8_BITS_PER_CHANNEL (4)
#define HI_ISP_TOP_ISP_PROCESSING_BYPASS_MODE_BYPASS_BACKEND_ISP_PROCESSING_AND_OUTPUT_16_MOST_SIGNIFICANT_BITS_OF_STICHED_DATA_IN_ALL_CHANNELS (5)
#define HI_ISP_TOP_ISP_PROCESSING_BYPASS_MODE_BYPASS_BACKEND_ISP_PROCESSING_AND_OUTPUT_STICHED_DATA_IN_CHANNELS_1_2_AND_3_884 (6)
#define HI_ISP_TOP_ISP_PROCESSING_BYPASS_MODE_FULL_BYPASS_ALL_INPUT_BITS_PASSED_TO_OUTPUT (7)

// args: data (3-bit)
static __inline HI_VOID hi_isp_top_isp_processing_bypass_mode_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x44);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x44, (((HI_U32) (data & 0x7)) << 13) | (curr & 0xffff1fff));
}
static __inline HI_U8 hi_isp_top_isp_processing_bypass_mode_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x44) & 0xe000) >> 13);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_ae_switch
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// AE tap in the pipeline.  Location of AE statistic collection.
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_AE_SWITCH_DEFAULT (0)
#define HI_ISP_TOP_AE_SWITCH_DATASIZE (2)
#define HI_ISP_TOP_AE_SWITCH_AFTER_STATIC_WB (0)
#define HI_ISP_TOP_AE_SWITCH_AFTER_TPG_CHANNEL1 (1)
#define HI_ISP_TOP_AE_SWITCH_AFTER_SHADING (2)
#define HI_ISP_TOP_AE_SWITCH_AFTER_WDR_STITCH (3)

// args: data (2-bit)
static __inline HI_VOID hi_isp_top_ae_switch_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x48);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x48, (((HI_U32) (data & 0x3)) << 0) | (curr & 0xfffffffc));
}
static __inline HI_U8 hi_isp_top_ae_switch_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x48) & 0x3) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_ae_sum_switch
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// AE tap in the pipeline.  Location of AE statistic collection.
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_AESUM_SWITCH_DEFAULT (0)
#define HI_ISP_TOP_AESUM_SWITCH_DATASIZE (2)

#define HI_ISP_TOP_AESUM_SWITCH_AFTER_STATIC_WB (0)
#define HI_ISP_TOP_AESUM_SWITCH_AFTER_TPG_CHANNEL1 (1)
#define HI_ISP_TOP_AESUM_SWITCH_AFTER_SHADING (2)
#define HI_ISP_TOP_AESUM_SWITCH_AFTER_WDR_STITCH (3)

// args: data (2-bit)
static __inline HI_VOID hi_isp_top_aesum_switch_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x48);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x48, (((HI_U32) (data & 0x3)) << 16) | (curr & 0xfffcffff));
}
static __inline HI_U8 hi_isp_top_aesum_switch_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x48) & 0x30000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_awb_switch
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// AWB tap in the pipeline.  Location of AWB statistics collection.
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_AWB_SWITCH_DEFAULT (0)
#define HI_ISP_TOP_AWB_SWITCH_DATASIZE (1)

#define HI_ISP_TOP_AWB_SWITCH_IMMEDIATELY_BEFORE_COLOR_MATRIX (0)
#define HI_ISP_TOP_AWB_SWITCH_IMMEDIATELY_AFTER_COLOR_MATRIX_FOR_SENSORS_WITH_STRONG_COLOR_CHANNEL_CROSSTALK_ (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_awb_switch_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4c, (((HI_U32) (data & 0x1)) << 0) | (curr & 0xfffffffe));
}
static __inline HI_U8 hi_isp_top_awb_switch_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4c) & 0x1) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: AWBSUM switch
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// AWBSUM tap in the pipeline.  Location of AWB statistics collection.
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_AWBSUM_SWITCH_DEFAULT (0)
#define HI_ISP_TOP_AWBSUM_SWITCH_DATASIZE (1)
#define HI_ISP_TOP_AWBSUM_SWITCH_IMMEDIATELY_BEFORE_LENSE_SHADING (0)
#define HI_ISP_TOP_AWBSUM_SWITCH_IMMEDIATELY_AFTER_LENSE_SHADING (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_awbsum_switch_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4c, (((HI_U32) (data & 0x1)) << 8) | (curr & 0xfffffeff));
}
static __inline HI_U8 hi_isp_top_awbsum_switch_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4c) & 0x100) >> 8);
}
// ------------------------------------------------------------------------------ //
// Register: Histogram switch
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// AE global histogram tap in the pipeline.  Location of statistics gathering for 1024 bin global histogram
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_HISTOGRAM_SWITCH_DEFAULT (0)
#define HI_ISP_TOP_HISTOGRAM_SWITCH_DATASIZE (2)
#define HI_ISP_TOP_HISTOGRAM_SWITCH_SAME_AS_AE (0)
#define HI_ISP_TOP_HISTOGRAM_SWITCH_AFTER_TPG_CHANNEL1 (1)
#define HI_ISP_TOP_HISTOGRAM_SWITCH_AFTER_SHADING (2)
#define HI_ISP_TOP_HISTOGRAM_SWITCH_AFTER_WDR_STITCH (3)

// args: data (2-bit)
static __inline HI_VOID hi_isp_top_histogram_switch_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x50);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x50, (((HI_U32) (data & 0x3)) << 0) | (curr & 0xfffffffc));
}
static __inline HI_U8 hi_isp_top_histogram_switch_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x50) & 0x3) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Histogram PE1 switch
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Per exposure AE global histogram enable for channel-1.  Enables statistics gathering for 256 bin global histograms
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_HISTOGRAM_PE1_SWITCH_DEFAULT (0)
#define HI_ISP_TOP_HISTOGRAM_PE1_SWITCH_DATASIZE (1)
#define HI_ISP_TOP_HISTOGRAM_PE1_SWITCH_ENABLED (0)
#define HI_ISP_TOP_HISTOGRAM_PE1_SWITCH_DISABLED (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_histogram_pe1_switch_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x58);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x58, (((HI_U32) (data & 0x1)) << 0) | (curr & 0xfffffffe));
}
static __inline HI_U8 hi_isp_top_histogram_pe1_switch_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x58) & 0x1) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Histogram PE2 switch
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Per exposure AE global histogram enable for channel-2.  Enables statistics gathering for 256 bin global histograms
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_HISTOGRAM_PE2_SWITCH_DEFAULT (0)
#define HI_ISP_TOP_HISTOGRAM_PE2_SWITCH_DATASIZE (1)
#define HI_ISP_TOP_HISTOGRAM_PE2_SWITCH_ENABLED (0)
#define HI_ISP_TOP_HISTOGRAM_PE2_SWITCH_DISABLED (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_histogram_pe2_switch_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x58);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x58, (((HI_U32) (data & 0x1)) << 1) | (curr & 0xfffffffd));
}
static __inline HI_U8 hi_isp_top_histogram_pe2_switch_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x58) & 0x2) >> 1);
}
// ------------------------------------------------------------------------------ //
// Register: Histogram PE3 switch
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Per exposure AE global histogram enable for channel-3.  Enables statistics gathering for 256 bin global histograms
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_HISTOGRAM_PE3_SWITCH_DEFAULT (0)
#define HI_ISP_TOP_HISTOGRAM_PE3_SWITCH_DATASIZE (1)
#define HI_ISP_TOP_HISTOGRAM_PE3_SWITCH_ENABLED (0)
#define HI_ISP_TOP_HISTOGRAM_PE3_SWITCH_DISABLED (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_histogram_pe3_switch_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x58);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x58, (((HI_U32) (data & 0x1)) << 2) | (curr & 0xfffffffb));
}
static __inline HI_U8 hi_isp_top_histogram_pe3_switch_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x58) & 0x4) >> 2);
}
// ------------------------------------------------------------------------------ //
// Register: Histogram PE4 switch
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Per exposure AE global histogram enable for channel-4.  Enables statistics gathering for 256 bin global histograms
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_HISTOGRAM_PE4_SWITCH_DEFAULT (0)
#define HI_ISP_TOP_HISTOGRAM_PE4_SWITCH_DATASIZE (1)
#define HI_ISP_TOP_HISTOGRAM_PE4_SWITCH_ENABLED (0)
#define HI_ISP_TOP_HISTOGRAM_PE4_SWITCH_DISABLED (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_histogram_pe4_switch_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x58);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x58, (((HI_U32) (data & 0x1)) << 3) | (curr & 0xfffffff7));
}
static __inline HI_U8 hi_isp_top_histogram_pe4_switch_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x58) & 0x8) >> 3);
}
// ------------------------------------------------------------------------------ //
// Register: Antifog Hist Switch
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Antifog histogram enable.  Enables statistics gathering for antifog global histogram
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_ANTIFOG_HIST_SWITCH_DEFAULT (0)
#define HI_ISP_TOP_ANTIFOG_HIST_SWITCH_DATASIZE (1)
#define HI_ISP_TOP_ANTIFOG_HIST_SWITCH_ENABLED (0)
#define HI_ISP_TOP_ANTIFOG_HIST_SWITCH_DISABLED (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_antifog_hist_switch_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x58);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x58, (((HI_U32) (data & 0x1)) << 16) | (curr & 0xfffeffff));
}
static __inline HI_U8 hi_isp_top_antifog_hist_switch_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x58) & 0x10000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_field_status
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Status of field signal
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_FIELD_STATUS_DEFAULT (0x0)
#define HI_ISP_TOP_FIELD_STATUS_DATASIZE (1)

// args: data (1-bit)
static __inline HI_U8 hi_isp_top_field_status_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7c) & 0x1) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_global_fsm_reset
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 1 = synchronous reset of FSMs in design (faster recovery after broken frame)
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_GLOBAL_FSM_RESET_DEFAULT (0x0)
#define HI_ISP_TOP_GLOBAL_FSM_RESET_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_global_fsm_reset_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x78);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x78, (((HI_U32) (data & 0x1)) << 0) | (curr & 0xfffffffe));
}
static __inline HI_U8 hi_isp_top_global_fsm_reset_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x78) & 0x1) >> 0);
}
// ------------------------------------------------------------------------------ //
// Group: Interrupts
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Interrupt controller
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_interrupts_interrupt0_source
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Interrupt source selector. See ISP guide for interrupt definitions and valid values.
// ------------------------------------------------------------------------------ //

#define HI_ISP_INTERRUPTS_INTERRUPT0_SOURCE_DEFAULT (0x0)
#define HI_ISP_INTERRUPTS_INTERRUPT0_SOURCE_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_isp_interrupts_interrupt0_source_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x80);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x80, (((HI_U32) (data & 0x3f)) << 0) | (curr & 0xffffffc0));
}
static __inline HI_U8 hi_isp_interrupts_interrupt0_source_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x80) & 0x3f) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_interrupts_interrupt1_source
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Interrupt source selector. See ISP guide for interrupt definitions and valid values.
// ------------------------------------------------------------------------------ //

#define HI_ISP_INTERRUPTS_INTERRUPT1_SOURCE_DEFAULT (0x0)
#define HI_ISP_INTERRUPTS_INTERRUPT1_SOURCE_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_isp_interrupts_interrupt1_source_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x80);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x80, (((HI_U32) (data & 0x3f)) << 16) | (curr & 0xffc0ffff));
}
static __inline HI_U8 hi_isp_interrupts_interrupt1_source_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x80) & 0x3f0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_interrupts_interrupt2_source
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Interrupt source selector. See ISP guide for interrupt definitions and valid values.
// ------------------------------------------------------------------------------ //

#define HI_ISP_INTERRUPTS_INTERRUPT2_SOURCE_DEFAULT (0x0)
#define HI_ISP_INTERRUPTS_INTERRUPT2_SOURCE_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_isp_interrupts_interrupt2_source_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x84);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x84, (((HI_U32) (data & 0x3f)) << 0) | (curr & 0xffffffc0));
}
static __inline HI_U8 hi_isp_interrupts_interrupt2_source_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x84) & 0x3f) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_interrupts_interrupt3_source
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Interrupt source selector. See ISP guide for interrupt definitions and valid values.
// ------------------------------------------------------------------------------ //

#define HI_ISP_INTERRUPTS_INTERRUPT3_SOURCE_DEFAULT (0x0)
#define HI_ISP_INTERRUPTS_INTERRUPT3_SOURCE_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_isp_interrupts_interrupt3_source_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x84);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x84, (((HI_U32) (data & 0x3f)) << 16) | (curr & 0xffc0ffff));
}
static __inline HI_U8 hi_isp_interrupts_interrupt3_source_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x84) & 0x3f0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_interrupts_interrupt4_source
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Interrupt source selector. See ISP guide for interrupt definitions and valid values.
// ------------------------------------------------------------------------------ //

#define HI_ISP_INTERRUPTS_INTERRUPT4_SOURCE_DEFAULT (0x0)
#define HI_ISP_INTERRUPTS_INTERRUPT4_SOURCE_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_isp_interrupts_interrupt4_source_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x88);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x88, (((HI_U32) (data & 0x3f)) << 0) | (curr & 0xffffffc0));
}
static __inline HI_U8 hi_isp_interrupts_interrupt4_source_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x88) & 0x3f) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_interrupts_interrupt5_source
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Interrupt source selector. See ISP guide for interrupt definitions and valid values.
// ------------------------------------------------------------------------------ //

#define HI_ISP_INTERRUPTS_INTERRUPT5_SOURCE_DEFAULT (0x0)
#define HI_ISP_INTERRUPTS_INTERRUPT5_SOURCE_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_isp_interrupts_interrupt5_source_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x88);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x88, (((HI_U32) (data & 0x3f)) << 16) | (curr & 0xffc0ffff));
}
static __inline HI_U8 hi_isp_interrupts_interrupt5_source_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x88) & 0x3f0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_interrupts_interrupt6_source
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Interrupt source selector. See ISP guide for interrupt definitions and valid values.
// ------------------------------------------------------------------------------ //

#define HI_ISP_INTERRUPTS_INTERRUPT6_SOURCE_DEFAULT (0x0)
#define HI_ISP_INTERRUPTS_INTERRUPT6_SOURCE_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_isp_interrupts_interrupt6_source_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x8c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x8c, (((HI_U32) (data & 0x3f)) << 0) | (curr & 0xffffffc0));
}
static __inline HI_U8 hi_isp_interrupts_interrupt6_source_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x8c) & 0x3f) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_interrupts_interrupt7_source
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Interrupt source selector. See ISP guide for interrupt definitions and valid values.
// ------------------------------------------------------------------------------ //

#define HI_ISP_INTERRUPTS_INTERRUPT7_SOURCE_DEFAULT (0x0)
#define HI_ISP_INTERRUPTS_INTERRUPT7_SOURCE_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_isp_interrupts_interrupt7_source_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x8c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x8c, (((HI_U32) (data & 0x3f)) << 16) | (curr & 0xffc0ffff));
}
static __inline HI_U8 hi_isp_interrupts_interrupt7_source_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x8c) & 0x3f0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_interrupts_interrupt8_source
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Interrupt source selector. See ISP guide for interrupt definitions and valid values.
// ------------------------------------------------------------------------------ //

#define HI_ISP_INTERRUPTS_INTERRUPT8_SOURCE_DEFAULT (0x0)
#define HI_ISP_INTERRUPTS_INTERRUPT8_SOURCE_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_isp_interrupts_interrupt8_source_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x90);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x90, (((HI_U32) (data & 0x3f)) << 0) | (curr & 0xffffffc0));
}
static __inline HI_U8 hi_isp_interrupts_interrupt8_source_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x90) & 0x3f) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_interrupts_interrupt9_source
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Interrupt source selector. See ISP guide for interrupt definitions and valid values.
// ------------------------------------------------------------------------------ //

#define HI_ISP_INTERRUPTS_INTERRUPT9_SOURCE_DEFAULT (0x0)
#define HI_ISP_INTERRUPTS_INTERRUPT9_SOURCE_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_isp_interrupts_interrupt9_source_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x90);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x90, (((HI_U32) (data & 0x3f)) << 16) | (curr & 0xffc0ffff));
}
static __inline HI_U8 hi_isp_interrupts_interrupt9_source_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x90) & 0x3f0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_interrupts_interrupt10_source
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Interrupt source selector. See ISP guide for interrupt definitions and valid values.
// ------------------------------------------------------------------------------ //

#define HI_ISP_INTERRUPTS_INTERRUPT10_SOURCE_DEFAULT (0x0)
#define HI_ISP_INTERRUPTS_INTERRUPT10_SOURCE_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_isp_interrupts_interrupt10_source_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x94);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x94, (((HI_U32) (data & 0x3f)) << 0) | (curr & 0xffffffc0));
}
static __inline HI_U8 hi_isp_interrupts_interrupt10_source_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x94) & 0x3f) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_interrupts_interrupt11_source
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Interrupt source selector. See ISP guide for interrupt definitions and valid values.
// ------------------------------------------------------------------------------ //

#define HI_ISP_INTERRUPTS_INTERRUPT11_SOURCE_DEFAULT (0x0)
#define HI_ISP_INTERRUPTS_INTERRUPT11_SOURCE_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_isp_interrupts_interrupt11_source_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x94);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x94, (((HI_U32) (data & 0x3f)) << 16) | (curr & 0xffc0ffff));
}
static __inline HI_U8 hi_isp_interrupts_interrupt11_source_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x94) & 0x3f0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_interrupts_interrupt12_source
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Interrupt source selector. See ISP guide for interrupt definitions and valid values.
// ------------------------------------------------------------------------------ //

#define HI_ISP_INTERRUPTS_INTERRUPT12_SOURCE_DEFAULT (0x0)
#define HI_ISP_INTERRUPTS_INTERRUPT12_SOURCE_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_isp_interrupts_interrupt12_source_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x98);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x98, (((HI_U32) (data & 0x3f)) << 0) | (curr & 0xffffffc0));
}
static __inline HI_U8 hi_isp_interrupts_interrupt12_source_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x98) & 0x3f) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_interrupts_interrupt13_source
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Interrupt source selector. See ISP guide for interrupt definitions and valid values.
// ------------------------------------------------------------------------------ //

#define HI_ISP_INTERRUPTS_INTERRUPT13_SOURCE_DEFAULT (0x0)
#define HI_ISP_INTERRUPTS_INTERRUPT13_SOURCE_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_isp_interrupts_interrupt13_source_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x98);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x98, (((HI_U32) (data & 0x3f)) << 16) | (curr & 0xffc0ffff));
}
static __inline HI_U8 hi_isp_interrupts_interrupt13_source_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x98) & 0x3f0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_interrupts_interrupt14_source
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Interrupt source selector. See ISP guide for interrupt definitions and valid values.
// ------------------------------------------------------------------------------ //

#define HI_ISP_INTERRUPTS_INTERRUPT14_SOURCE_DEFAULT (0x0)
#define HI_ISP_INTERRUPTS_INTERRUPT14_SOURCE_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_isp_interrupts_interrupt14_source_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x9c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x9c, (((HI_U32) (data & 0x3f)) << 0) | (curr & 0xffffffc0));
}
static __inline HI_U8 hi_isp_interrupts_interrupt14_source_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x9c) & 0x3f) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_interrupts_interrupt15_source
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Interrupt source selector. See ISP guide for interrupt definitions and valid values.
// ------------------------------------------------------------------------------ //

#define HI_ISP_INTERRUPTS_INTERRUPT15_SOURCE_DEFAULT (0x0)
#define HI_ISP_INTERRUPTS_INTERRUPT15_SOURCE_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_isp_interrupts_interrupt15_source_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x9c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x9c, (((HI_U32) (data & 0x3f)) << 16) | (curr & 0xffc0ffff));
}
static __inline HI_U8 hi_isp_interrupts_interrupt15_source_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x9c) & 0x3f0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: Interrupt status
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Interrupt event flags.
// ------------------------------------------------------------------------------ //

#define HI_ISP_INTERRUPTS_INTERRUPT_STATUS_DEFAULT (0x0)
#define HI_ISP_INTERRUPTS_INTERRUPT_STATUS_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_isp_interrupts_interrupt_status_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xa0) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Interrupt clear
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Interrupt event clear register writing 0-1 transition will clear the corresponding status bits.
// ------------------------------------------------------------------------------ //

#define HI_ISP_INTERRUPTS_INTERRUPT_CLEAR_DEFAULT (0x0)
#define HI_ISP_INTERRUPTS_INTERRUPT_CLEAR_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_interrupts_interrupt_clear_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xa4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xa4, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_interrupts_interrupt_clear_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xa4) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Group: Input port
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Controls video input port.  
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_signal_flag0
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_SIGNAL_FLAG0_DEFAULT (0)
#define HI_ISP_INPUT_PORT_SIGNAL_FLAG_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_isp_input_port_signal_flag0_write(ISP_DEV IspDev, HI_U32 data) {
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x0100, data);
}
static __inline HI_U32 hi_isp_input_port_signal_flag0_read(ISP_DEV IspDev) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x0100);
}


// ------------------------------------------------------------------------------ //
// Register: preset
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
//  Allows selection of various input port presets for standard sensor inputs.  See ISP Guide for details of available presets.
//  0-14: Frequently used presets.  If using one of available presets, remaining bits in registers 0x100 and 0x104 are not used.
//  15:   Input port configured according to registers in 0x100 and 0x104.  Consult Apical support for special input port requirements.
//      
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_PRESET_DEFAULT (2)
#define HI_ISP_INPUT_PORT_PRESET_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_isp_input_port_preset_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100, (((HI_U32) (data & 0xf)) << 0) | (curr & 0xfffffff0));
}
static __inline HI_U8 hi_isp_input_port_preset_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100) & 0xf) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_vs_use_field
// ------------------------------------------------------------------------------ //
// 
//   0 = use vsync_i port for vertical sync
//   1 = use field_i port for vertical sync
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_VS_USE_FIELD_DEFAULT (0)
#define HI_ISP_INPUT_PORT_VS_USE_FIELD_DATASIZE (1)
#define HI_ISP_INPUT_PORT_VS_USE_FIELD_USE_VSYNC_I_PORT_FOR_VERTICAL_SYNC (0)
#define HI_ISP_INPUT_PORT_VS_USE_FIELD_USE_FIELD_I_PORT_FOR_VERTICAL_SYNC (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_vs_use_field_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100, (((HI_U32) (data & 0x1)) << 8) | (curr & 0xfffffeff));
}
static __inline HI_U8 hi_isp_input_port_vs_use_field_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100) & 0x100) >> 8);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_vs_toggle
// ------------------------------------------------------------------------------ //
// 
//   0 = vsync is pulse-type
//   1 = vsync is toggle-type (field signal)
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_VS_TOGGLE_DEFAULT (0)
#define HI_ISP_INPUT_PORT_VS_TOGGLE_DATASIZE (1)
#define HI_ISP_INPUT_PORT_VS_TOGGLE_VSYNC_IS_PULSETYPE (0)
#define HI_ISP_INPUT_PORT_VS_TOGGLE_VSYNC_IS_TOGGLETYPE_FIELD_SIGNAL (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_vs_toggle_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100, (((HI_U32) (data & 0x1)) << 9) | (curr & 0xfffffdff));
}
static __inline HI_U8 hi_isp_input_port_vs_toggle_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100) & 0x200) >> 9);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_vs_polarity
// ------------------------------------------------------------------------------ //
// 
//   0 = horizontal counter reset on rising edge
//   1 = horizontal counter reset on falling edge
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_VS_POLARITY_DEFAULT (0)
#define HI_ISP_INPUT_PORT_VS_POLARITY_DATASIZE (1)
#define HI_ISP_INPUT_PORT_VS_POLARITY_HORIZONTAL_COUNTER_RESET_ON_RISING_EDGE (0)
#define HI_ISP_INPUT_PORT_VS_POLARITY_HORIZONTAL_COUNTER_RESET_ON_FALLING_EDGE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_vs_polarity_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100, (((HI_U32) (data & 0x1)) << 10) | (curr & 0xfffffbff));
}
static __inline HI_U8 hi_isp_input_port_vs_polarity_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100) & 0x400) >> 10);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_vs_polarity_acl
// ------------------------------------------------------------------------------ //
// 
//   0 = don't invert polarity for ACL gate
//   1 = invert polarity for ACL gate
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_VS_POLARITY_ACL_DEFAULT (0)
#define HI_ISP_INPUT_PORT_VS_POLARITY_ACL_DATASIZE (1)
#define HI_ISP_INPUT_PORT_VS_POLARITY_ACL_DONT_INVERT_POLARITY_FOR_ACL_GATE (0)
#define HI_ISP_INPUT_PORT_VS_POLARITY_ACL_INVERT_POLARITY_FOR_ACL_GATE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_vs_polarity_acl_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100, (((HI_U32) (data & 0x1)) << 11) | (curr & 0xfffff7ff));
}
static __inline HI_U8 hi_isp_input_port_vs_polarity_acl_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100) & 0x800) >> 11);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_hs_use_acl
// ------------------------------------------------------------------------------ //
// 
//   0 = use hsync_i port for active-line
//   1 = use acl_i port for active-line
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_HS_USE_ACL_DEFAULT (0)
#define HI_ISP_INPUT_PORT_HS_USE_ACL_DATASIZE (1)
#define HI_ISP_INPUT_PORT_HS_USE_ACL_USE_HSYNC_I_PORT_FOR_ACTIVELINE (0)
#define HI_ISP_INPUT_PORT_HS_USE_ACL_USE_ACL_I_PORT_FOR_ACTIVELINE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_hs_use_acl_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100, (((HI_U32) (data & 0x1)) << 12) | (curr & 0xffffefff));
}
static __inline HI_U8 hi_isp_input_port_hs_use_acl_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100) & 0x1000) >> 12);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_vc_c_select
// ------------------------------------------------------------------------------ //
// 
//   0 = vertical counter counts on hs
//   1 = vertical counter counts on horizontal counter overflow or reset
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_VC_C_SELECT_DEFAULT (0)
#define HI_ISP_INPUT_PORT_VC_C_SELECT_DATASIZE (1)
#define HI_ISP_INPUT_PORT_VC_C_SELECT_VERTICAL_COUNTER_COUNTS_ON_HS (0)
#define HI_ISP_INPUT_PORT_VC_C_SELECT_VERTICAL_COUNTER_COUNTS_ON_HORIZONTAL_COUNTER_OVERFLOW_OR_RESET (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_vc_c_select_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100, (((HI_U32) (data & 0x1)) << 14) | (curr & 0xffffbfff));
}
static __inline HI_U8 hi_isp_input_port_vc_c_select_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100) & 0x4000) >> 14);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_vc_r_select
// ------------------------------------------------------------------------------ //
// 
//   0 = vertical counter is reset on edge of vs
//   1 = vertical counter is reset after timeout on hs
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_VC_R_SELECT_DEFAULT (0)
#define HI_ISP_INPUT_PORT_VC_R_SELECT_DATASIZE (1)
#define HI_ISP_INPUT_PORT_VC_R_SELECT_VERTICAL_COUNTER_IS_RESET_ON_EDGE_OF_VS (0)
#define HI_ISP_INPUT_PORT_VC_R_SELECT_VERTICAL_COUNTER_IS_RESET_AFTER_TIMEOUT_ON_HS (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_vc_r_select_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100, (((HI_U32) (data & 0x1)) << 15) | (curr & 0xffff7fff));
}
static __inline HI_U8 hi_isp_input_port_vc_r_select_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100) & 0x8000) >> 15);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_hs_xor_vs
// ------------------------------------------------------------------------------ //
// 
//   0 = normal mode
//   1 = hvalid = hsync XOR vsync
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_HS_XOR_VS_DEFAULT (0)
#define HI_ISP_INPUT_PORT_HS_XOR_VS_DATASIZE (1)
#define HI_ISP_INPUT_PORT_HS_XOR_VS_NORMAL_MODE (0)
#define HI_ISP_INPUT_PORT_HS_XOR_VS_HVALID__HSYNC_XOR_VSYNC (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_hs_xor_vs_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100, (((HI_U32) (data & 0x1)) << 16) | (curr & 0xfffeffff));
}
static __inline HI_U8 hi_isp_input_port_hs_xor_vs_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100) & 0x10000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_hs_polarity
// ------------------------------------------------------------------------------ //
// 
//   0 = don't invert polarity of HS for ACL gate
//   1 = invert polarity of HS for ACL gate
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_HS_POLARITY_DEFAULT (0)
#define HI_ISP_INPUT_PORT_HS_POLARITY_DATASIZE (1)
#define HI_ISP_INPUT_PORT_HS_POLARITY_DONT_INVERT_POLARITY_OF_HS_FOR_ACL_GATE (0)
#define HI_ISP_INPUT_PORT_HS_POLARITY_INVERT_POLARITY_OF_HS_FOR_ACL_GATE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_hs_polarity_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100, (((HI_U32) (data & 0x1)) << 17) | (curr & 0xfffdffff));
}
static __inline HI_U8 hi_isp_input_port_hs_polarity_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100) & 0x20000) >> 17);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_hs_polarity_acl
// ------------------------------------------------------------------------------ //
// 
//   0 = don't invert polarity of HS for HS gate
//   1 = invert polarity of HS for HS gate
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_HS_POLARITY_ACL_DEFAULT (0)
#define HI_ISP_INPUT_PORT_HS_POLARITY_ACL_DATASIZE (1)
#define HI_ISP_INPUT_PORT_HS_POLARITY_ACL_DONT_INVERT_POLARITY_OF_HS_FOR_HS_GATE (0)
#define HI_ISP_INPUT_PORT_HS_POLARITY_ACL_INVERT_POLARITY_OF_HS_FOR_HS_GATE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_hs_polarity_acl_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100, (((HI_U32) (data & 0x1)) << 18) | (curr & 0xfffbffff));
}
static __inline HI_U8 hi_isp_input_port_hs_polarity_acl_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100) & 0x40000) >> 18);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_hs_polarity_hs
// ------------------------------------------------------------------------------ //
// 
//   0 = horizontal counter is reset on rising edge of hs
//   1 = horizontal counter is reset on vsync (e.g. when hsync is not available)
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_HS_POLARITY_HS_DEFAULT (0)
#define HI_ISP_INPUT_PORT_HS_POLARITY_HS_DATASIZE (1)
#define HI_ISP_INPUT_PORT_HS_POLARITY_HS_HORIZONTAL_COUNTER_IS_RESET_ON_RISING_EDGE_OF_HS (0)
#define HI_ISP_INPUT_PORT_HS_POLARITY_HS_HORIZONTAL_COUNTER_IS_RESET_ON_VSYNC_EG_WHEN_HSYNC_IS_NOT_AVAILABLE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_hs_polarity_hs_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100, (((HI_U32) (data & 0x1)) << 19) | (curr & 0xfff7ffff));
}
static __inline HI_U8 hi_isp_input_port_hs_polarity_hs_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100) & 0x80000) >> 19);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_hs_polarity_vc
// ------------------------------------------------------------------------------ //
// 
//   0 = vertical counter increments on rising edge of HS
//   1 = vertical counter increments on falling edge of HS
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_HS_POLARITY_VC_DEFAULT (1)
#define HI_ISP_INPUT_PORT_HS_POLARITY_VC_DATASIZE (1)
#define HI_ISP_INPUT_PORT_HS_POLARITY_VC_VERTICAL_COUNTER_INCREMENTS_ON_RISING_EDGE_OF_HS (0)
#define HI_ISP_INPUT_PORT_HS_POLARITY_VC_VERTICAL_COUNTER_INCREMENTS_ON_FALLING_EDGE_OF_HS (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_hs_polarity_vc_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100, (((HI_U32) (data & 0x1)) << 20) | (curr & 0xffefffff));
}
static __inline HI_U8 hi_isp_input_port_hs_polarity_vc_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100) & 0x100000) >> 20);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_hc_r_select
// ------------------------------------------------------------------------------ //
// 
//   0 = vertical counter is reset on rising edge of hs
//   1 = vertical counter is reset on rising edge of vs
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_HC_R_SELECT_DEFAULT (0)
#define HI_ISP_INPUT_PORT_HC_R_SELECT_DATASIZE (1)
#define HI_ISP_INPUT_PORT_HC_R_SELECT_VERTICAL_COUNTER_IS_RESET_ON_RISING_EDGE_OF_HS (0)
#define HI_ISP_INPUT_PORT_HC_R_SELECT_VERTICAL_COUNTER_IS_RESET_ON_RISING_EDGE_OF_VS (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_hc_r_select_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100, (((HI_U32) (data & 0x1)) << 23) | (curr & 0xff7fffff));
}
static __inline HI_U8 hi_isp_input_port_hc_r_select_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100) & 0x800000) >> 23);
}
// ------------------------------------------------------------------------------ //
// Register: acl polarity
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_ACL_POLARITY_DEFAULT (0)
#define HI_ISP_INPUT_PORT_ACL_POLARITY_DATASIZE (1)
#define HI_ISP_INPUT_PORT_ACL_POLARITY_DONT_INVERT_ACL_I_FOR_ACL_GATE (0)
#define HI_ISP_INPUT_PORT_ACL_POLARITY_INVERT_ACL_I_FOR_ACL_GATE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_acl_polarity_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100, (((HI_U32) (data & 0x1)) << 24) | (curr & 0xfeffffff));
}
static __inline HI_U8 hi_isp_input_port_acl_polarity_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x100) & 0x1000000) >> 24);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_signal_flag
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_SIGNAL_FLAG1_DEFAULT (0)
#define HI_ISP_INPUT_PORT_SIGNAL_FLAG1_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_isp_input_port_signal_flag1_write(ISP_DEV IspDev, HI_U32 data) {
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x0104, data);
}
static __inline HI_U32 hi_isp_input_port_signal_flag1_read(ISP_DEV IspDev) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x0104);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_field_polarity
// ------------------------------------------------------------------------------ //
// 
//   0 = don't invert field_i for field gate
//   1 = invert field_i for field gate
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_FIELD_POLARITY_DEFAULT (0)
#define HI_ISP_INPUT_PORT_FIELD_POLARITY_DATASIZE (1)
#define HI_ISP_INPUT_PORT_FIELD_POLARITY_DONT_INVERT_FIELD_I_FOR_FIELD_GATE (0)
#define HI_ISP_INPUT_PORT_FIELD_POLARITY_INVERT_FIELD_I_FOR_FIELD_GATE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_field_polarity_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104, (((HI_U32) (data & 0x1)) << 0) | (curr & 0xfffffffe));
}
static __inline HI_U8 hi_isp_input_port_field_polarity_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104) & 0x1) >> 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_field_toggle
// ------------------------------------------------------------------------------ //
// 
//   0 = field is pulse-type
//   1 = field is toggle-type
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_FIELD_TOGGLE_DEFAULT (0)
#define HI_ISP_INPUT_PORT_FIELD_TOGGLE_DATASIZE (1)
#define HI_ISP_INPUT_PORT_FIELD_TOGGLE_FIELD_IS_PULSETYPE (0)
#define HI_ISP_INPUT_PORT_FIELD_TOGGLE_FIELD_IS_TOGGLETYPE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_field_toggle_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104, (((HI_U32) (data & 0x1)) << 1) | (curr & 0xfffffffd));
}
static __inline HI_U8 hi_isp_input_port_field_toggle_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104) & 0x2) >> 1);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_aclg_window0
// ------------------------------------------------------------------------------ //
// 
//   0 = exclude window0 signal in ACL gate
//   1 = include window0 signal in ACL gate
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_ACLG_WINDOW0_DEFAULT (0)
#define HI_ISP_INPUT_PORT_ACLG_WINDOW0_DATASIZE (1)
#define HI_ISP_INPUT_PORT_ACLG_WINDOW0_EXCLUDE_WINDOW0_SIGNAL_IN_ACL_GATE (0)
#define HI_ISP_INPUT_PORT_ACLG_WINDOW0_INCLUDE_WINDOW0_SIGNAL_IN_ACL_GATE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_aclg_window0_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104, (((HI_U32) (data & 0x1)) << 8) | (curr & 0xfffffeff));
}
static __inline HI_U8 hi_isp_input_port_aclg_window0_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104) & 0x100) >> 8);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_aclg_hsync
// ------------------------------------------------------------------------------ //
// 
//   0 = exclude hsync signal in ACL gate
//   1 = include hsync signal in ACL gate
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_ACLG_HSYNC_DEFAULT (0)
#define HI_ISP_INPUT_PORT_ACLG_HSYNC_DATASIZE (1)
#define HI_ISP_INPUT_PORT_ACLG_HSYNC_EXCLUDE_HSYNC_SIGNAL_IN_ACL_GATE (0)
#define HI_ISP_INPUT_PORT_ACLG_HSYNC_INCLUDE_HSYNC_SIGNAL_IN_ACL_GATE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_aclg_hsync_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104, (((HI_U32) (data & 0x1)) << 9) | (curr & 0xfffffdff));
}
static __inline HI_U8 hi_isp_input_port_aclg_hsync_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104) & 0x200) >> 9);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_aclg_window2
// ------------------------------------------------------------------------------ //
// 
//   0 = exclude window2 signal in ACL gate
//   1 = include window2 signal in ACL gate
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_ACLG_WINDOW2_DEFAULT (0)
#define HI_ISP_INPUT_PORT_ACLG_WINDOW2_DATASIZE (1)
#define HI_ISP_INPUT_PORT_ACLG_WINDOW2_EXCLUDE_WINDOW2_SIGNAL_IN_ACL_GATE (0)
#define HI_ISP_INPUT_PORT_ACLG_WINDOW2_INCLUDE_WINDOW2_SIGNAL_IN_ACL_GATE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_aclg_window2_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104, (((HI_U32) (data & 0x1)) << 10) | (curr & 0xfffffbff));
}
static __inline HI_U8 hi_isp_input_port_aclg_window2_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104) & 0x400) >> 10);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_aclg_acl
// ------------------------------------------------------------------------------ //
// 
//   0 = exclude acl_i signal in ACL gate
//   1 = include acl_i signal in ACL gate
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_ACLG_ACL_DEFAULT (0)
#define HI_ISP_INPUT_PORT_ACLG_ACL_DATASIZE (1)
#define HI_ISP_INPUT_PORT_ACLG_ACL_EXCLUDE_ACL_I_SIGNAL_IN_ACL_GATE (0)
#define HI_ISP_INPUT_PORT_ACLG_ACL_INCLUDE_ACL_I_SIGNAL_IN_ACL_GATE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_aclg_acl_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104, (((HI_U32) (data & 0x1)) << 11) | (curr & 0xfffff7ff));
}
static __inline HI_U8 hi_isp_input_port_aclg_acl_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104) & 0x800) >> 11);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_aclg_vsync
// ------------------------------------------------------------------------------ //
// 
//   0 = exclude vsync signal in ACL gate
//   1 = include vsync signal in ACL gate
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_ACLG_VSYNC_DEFAULT (0)
#define HI_ISP_INPUT_PORT_ACLG_VSYNC_DATASIZE (1)
#define HI_ISP_INPUT_PORT_ACLG_VSYNC_EXCLUDE_VSYNC_SIGNAL_IN_ACL_GATE (0)
#define HI_ISP_INPUT_PORT_ACLG_VSYNC_INCLUDE_VSYNC_SIGNAL_IN_ACL_GATE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_aclg_vsync_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104, (((HI_U32) (data & 0x1)) << 12) | (curr & 0xffffefff));
}
static __inline HI_U8 hi_isp_input_port_aclg_vsync_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104) & 0x1000) >> 12);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_hsg_window1
// ------------------------------------------------------------------------------ //
// 
//   0 = exclude window1 signal in HS gate
//   1 = include window1 signal in HS gate
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_HSG_WINDOW1_DEFAULT (0)
#define HI_ISP_INPUT_PORT_HSG_WINDOW1_DATASIZE (1)
#define HI_ISP_INPUT_PORT_HSG_WINDOW1_EXCLUDE_WINDOW1_SIGNAL_IN_HS_GATE (0)
#define HI_ISP_INPUT_PORT_HSG_WINDOW1_INCLUDE_WINDOW1_SIGNAL_IN_HS_GATE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_hsg_window1_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104, (((HI_U32) (data & 0x1)) << 16) | (curr & 0xfffeffff));
}
static __inline HI_U8 hi_isp_input_port_hsg_window1_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104) & 0x10000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_hsg_hsync
// ------------------------------------------------------------------------------ //
// 
//   0 = exclude hsync signal in HS gate
//   1 = include hsync signal in HS gate
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_HSG_HSYNC_DEFAULT (0)
#define HI_ISP_INPUT_PORT_HSG_HSYNC_DATASIZE (1)
#define HI_ISP_INPUT_PORT_HSG_HSYNC_EXCLUDE_HSYNC_SIGNAL_IN_HS_GATE (0)
#define HI_ISP_INPUT_PORT_HSG_HSYNC_INCLUDE_HSYNC_SIGNAL_IN_HS_GATE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_hsg_hsync_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104, (((HI_U32) (data & 0x1)) << 17) | (curr & 0xfffdffff));
}
static __inline HI_U8 hi_isp_input_port_hsg_hsync_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104) & 0x20000) >> 17);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_hsg_vsync
// ------------------------------------------------------------------------------ //
// 
//   0 = exclude vsync signal in HS gate
//   1 = include vsync signal in HS gate
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_HSG_VSYNC_DEFAULT (0)
#define HI_ISP_INPUT_PORT_HSG_VSYNC_DATASIZE (1)
#define HI_ISP_INPUT_PORT_HSG_VSYNC_EXCLUDE_VSYNC_SIGNAL_IN_HS_GATE (0)
#define HI_ISP_INPUT_PORT_HSG_VSYNC_INCLUDE_VSYNC_SIGNAL_IN_HS_GATE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_hsg_vsync_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104, (((HI_U32) (data & 0x1)) << 18) | (curr & 0xfffbffff));
}
static __inline HI_U8 hi_isp_input_port_hsg_vsync_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104) & 0x40000) >> 18);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_hsg_window2
// ------------------------------------------------------------------------------ //
// 
//   0 = exclude window2 signal in HS gate
//   1 = include window2 signal in HS gate (mask out spurious hs during blank)
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_HSG_WINDOW2_DEFAULT (0)
#define HI_ISP_INPUT_PORT_HSG_WINDOW2_DATASIZE (1)
#define HI_ISP_INPUT_PORT_HSG_WINDOW2_EXCLUDE_WINDOW2_SIGNAL_IN_HS_GATE (0)
#define HI_ISP_INPUT_PORT_HSG_WINDOW2_INCLUDE_WINDOW2_SIGNAL_IN_HS_GATE_MASK_OUT_SPURIOUS_HS_DURING_BLANK (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_hsg_window2_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104, (((HI_U32) (data & 0x1)) << 19) | (curr & 0xfff7ffff));
}
static __inline HI_U8 hi_isp_input_port_hsg_window2_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104) & 0x80000) >> 19);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_fieldg_vsync
// ------------------------------------------------------------------------------ //
// 
//   0 = exclude vsync signal in Field gate
//   1 = include vsync signal in Field gate
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_FIELDG_VSYNC_DEFAULT (0)
#define HI_ISP_INPUT_PORT_FIELDG_VSYNC_DATASIZE (1)
#define HI_ISP_INPUT_PORT_FIELDG_VSYNC_EXCLUDE_VSYNC_SIGNAL_IN_FIELD_GATE (0)
#define HI_ISP_INPUT_PORT_FIELDG_VSYNC_INCLUDE_VSYNC_SIGNAL_IN_FIELD_GATE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_fieldg_vsync_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104, (((HI_U32) (data & 0x1)) << 24) | (curr & 0xfeffffff));
}
static __inline HI_U8 hi_isp_input_port_fieldg_vsync_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104) & 0x1000000) >> 24);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_fieldg_window2
// ------------------------------------------------------------------------------ //
// 
//   0 = exclude window2 signal in Field gate
//   1 = include window2 signal in Field gate
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_FIELDG_WINDOW2_DEFAULT (0)
#define HI_ISP_INPUT_PORT_FIELDG_WINDOW2_DATASIZE (1)
#define HI_ISP_INPUT_PORT_FIELDG_WINDOW2_EXCLUDE_WINDOW2_SIGNAL_IN_FIELD_GATE (0)
#define HI_ISP_INPUT_PORT_FIELDG_WINDOW2_INCLUDE_WINDOW2_SIGNAL_IN_FIELD_GATE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_fieldg_window2_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104, (((HI_U32) (data & 0x1)) << 25) | (curr & 0xfdffffff));
}
static __inline HI_U8 hi_isp_input_port_fieldg_window2_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104) & 0x2000000) >> 25);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_fieldg_field
// ------------------------------------------------------------------------------ //
// 
//   0 = exclude field_i signal in Field gate
//   1 = include field_i signal in Field gate
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_FIELDG_FIELD_DEFAULT (0)
#define HI_ISP_INPUT_PORT_FIELDG_FIELD_DATASIZE (1)
#define HI_ISP_INPUT_PORT_FIELDG_FIELD_EXCLUDE_FIELD_I_SIGNAL_IN_FIELD_GATE (0)
#define HI_ISP_INPUT_PORT_FIELDG_FIELD_INCLUDE_FIELD_I_SIGNAL_IN_FIELD_GATE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_fieldg_field_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104, (((HI_U32) (data & 0x1)) << 26) | (curr & 0xfbffffff));
}
static __inline HI_U8 hi_isp_input_port_fieldg_field_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104) & 0x4000000) >> 26);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_field_mode
// ------------------------------------------------------------------------------ //
// 
//   0 = pulse field
//   1 = toggle field
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_FIELD_MODE_DEFAULT (0)
#define HI_ISP_INPUT_PORT_FIELD_MODE_DATASIZE (1)
#define HI_ISP_INPUT_PORT_FIELD_MODE_PULSE_FIELD (0)
#define HI_ISP_INPUT_PORT_FIELD_MODE_TOGGLE_FIELD (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_field_mode_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104, (((HI_U32) (data & 0x1)) << 27) | (curr & 0xf7ffffff));
}
static __inline HI_U8 hi_isp_input_port_field_mode_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x104) & 0x8000000) >> 27);
}
// ------------------------------------------------------------------------------ //
// Register: hc limit
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// horizontal counter limit value (counts: 0,1,...hc_limit-1,hc_limit,0,1,...)
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_HC_LIMIT_DEFAULT (0xFFFF)
#define HI_ISP_INPUT_PORT_HC_LIMIT_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_input_port_hc_limit_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x108);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x108, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_input_port_hc_limit_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x108) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hc start0
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// window0 start for ACL gate.  See ISP guide for further details.
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_HC_START0_DEFAULT (0)
#define HI_ISP_INPUT_PORT_HC_START0_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_input_port_hc_start0_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x10c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x10c, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_input_port_hc_start0_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x10c) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hc size0
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// window0 size for ACL gate.  See ISP guide for further details.
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_HC_SIZE0_DEFAULT (0)
#define HI_ISP_INPUT_PORT_HC_SIZE0_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_input_port_hc_size0_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x110);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x110, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_input_port_hc_size0_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x110) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hc start1
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// window1 start for HS gate.  See ISP guide for further details.
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_HC_START1_DEFAULT (0)
#define HI_ISP_INPUT_PORT_HC_START1_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_input_port_hc_start1_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x114);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x114, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_input_port_hc_start1_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x114) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hc size1
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// window1 size for HS gate.  See ISP guide for further details.
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_HC_SIZE1_DEFAULT (0)
#define HI_ISP_INPUT_PORT_HC_SIZE1_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_input_port_hc_size1_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x118);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x118, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_input_port_hc_size1_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x118) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: vc limit
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// vertical counter limit value (counts: 0,1,...vc_limit-1,vc_limit,0,1,...)
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_VC_LIMIT_DEFAULT (0xFFFF)
#define HI_ISP_INPUT_PORT_VC_LIMIT_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_input_port_vc_limit_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x11c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x11c, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_input_port_vc_limit_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x11c) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: vc start
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// window2 start for ACL gate.  See ISP guide for further details.
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_VC_START_DEFAULT (0)
#define HI_ISP_INPUT_PORT_VC_START_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_input_port_vc_start_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x120);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x120, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_input_port_vc_start_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x120) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: vc size
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// window2 size for ACL gate.  See ISP guide for further details.
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_VC_SIZE_DEFAULT (0)
#define HI_ISP_INPUT_PORT_VC_SIZE_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_input_port_vc_size_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x124);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x124, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_input_port_vc_size_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x124) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: frame width
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// detected frame width.  Read only value.
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_FRAME_WIDTH_DEFAULT (0)
#define HI_ISP_INPUT_PORT_FRAME_WIDTH_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_isp_input_port_frame_width_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x128) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: frame height
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// detected frame height.  Read only value.  
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_FRAME_HEIGHT_DEFAULT (0)
#define HI_ISP_INPUT_PORT_FRAME_HEIGHT_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_isp_input_port_frame_height_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x12c) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: freeze config
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Used to freeze input port configuration.  Used when multiple register writes are required to change input port configuration. 
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_FREEZE_CONFIG_DEFAULT (0)
#define HI_ISP_INPUT_PORT_FREEZE_CONFIG_DATASIZE (1)
#define HI_ISP_INPUT_PORT_FREEZE_CONFIG_NORMAL_OPERATION (0)
#define HI_ISP_INPUT_PORT_FREEZE_CONFIG_HOLD_PREVIOUS_INPUT_PORT_CONFIG_STATE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_freeze_config_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x130);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x130, (((HI_U32) (data & 0x1)) << 7) | (curr & 0xffffff7f));
}
static __inline HI_U8 hi_isp_input_port_freeze_config_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x130) & 0x80) >> 7);
}
// ------------------------------------------------------------------------------ //
// Register: mode request
// ------------------------------------------------------------------------------ //
// 
//   0 = safe stop
//   1 = safe start
//   2 = urgent stop
//   3 = urgent start
//   4 = reserved4
//   5 = safer start
//   6 = reserved6
//   7 = reserved7
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_MODE_REQUEST_DEFAULT (0)
#define HI_ISP_INPUT_PORT_MODE_REQUEST_DATASIZE (3)
#define HI_ISP_INPUT_PORT_MODE_REQUEST_SAFE_STOP (0)
#define HI_ISP_INPUT_PORT_MODE_REQUEST_SAFE_START (1)
#define HI_ISP_INPUT_PORT_MODE_REQUEST_URGENT_STOP (2)
#define HI_ISP_INPUT_PORT_MODE_REQUEST_URGENT_START (3)
#define HI_ISP_INPUT_PORT_MODE_REQUEST_RESERVED4 (4)
#define HI_ISP_INPUT_PORT_MODE_REQUEST_SAFER_START (5)
#define HI_ISP_INPUT_PORT_MODE_REQUEST_RESERVED6 (6)
#define HI_ISP_INPUT_PORT_MODE_REQUEST_RESERVED7 (7)

// args: data (3-bit)
static __inline HI_VOID hi_isp_input_port_mode_request_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x130);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x130, (((HI_U32) (data & 0x7)) << 0) | (curr & 0xfffffff8));
}
static __inline HI_U8 hi_isp_input_port_mode_request_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x130) & 0x7) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: mode status
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 
//      Used to monitor input port status:
//      bit 0: 1=running, 0=stopped, bits 1,2-reserved
//      
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_MODE_STATUS_DEFAULT (0)
#define HI_ISP_INPUT_PORT_MODE_STATUS_DATASIZE (3)

// args: data (3-bit)
static __inline HI_U8 hi_isp_input_port_mode_status_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x134) & 0x7) >> 0);
}
// ------------------------------------------------------------------------------ //
// Group: Input Formatter
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 
//        Adapts received data format to ISP format see ISP guide for a table of setting to be used for various sensors/modes.
//      
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: mode in
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
//  5 bit, input mode 
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_FORMATTER_MODE_IN_DEFAULT (0)
#define HI_ISP_INPUT_FORMATTER_MODE_IN_DATASIZE (5)

// args: data (5-bit)
static __inline HI_VOID hi_isp_input_formatter_mode_in_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x140);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x140, (((HI_U32) (data & 0x1f)) << 0) | (curr & 0xffffffe0));
}
static __inline HI_U8 hi_isp_input_formatter_mode_in_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x140) & 0x1f) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: mode out
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
//  3 bit, output mode 
//          0       : 4 ch from input
//          1       : 2 ch mapped linear data
//          2       : 3 ch mapped linear data
//          3       : 4 ch mapped linear data
//          4       : 2 ch mapped 20bit linear. This is for ISP-2 where 20bit linear data is created from 2 exposures.
//          5...7   : reserved
//        
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_FORMATTER_MODE_OUT_DEFAULT (0)
#define HI_ISP_INPUT_FORMATTER_MODE_OUT_DATASIZE (3)

// args: data (3-bit)
static __inline HI_VOID hi_isp_input_formatter_mode_out_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x140);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x140, (((HI_U32) (data & 0x7)) << 8) | (curr & 0xfffff8ff));
}
static __inline HI_U8 hi_isp_input_formatter_mode_out_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x140) & 0x700) >> 8);
}
// ------------------------------------------------------------------------------ //
// Register: input bitwidth select
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
//  2 bit, input bitwidth select 
//                  0=12 bits, 1=14 bits, 2=16 bits, 3=20 bits
//        
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_FORMATTER_INPUT_BITWIDTH_SELECT_DEFAULT (0)
#define HI_ISP_INPUT_FORMATTER_INPUT_BITWIDTH_SELECT_DATASIZE (2)

// args: data (2-bit)
static __inline HI_VOID hi_isp_input_formatter_input_bitwidth_select_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x140);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x140, (((HI_U32) (data & 0x3)) << 16) | (curr & 0xfffcffff));
}
static __inline HI_U8 hi_isp_input_formatter_input_bitwidth_select_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x140) & 0x30000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: factor ML
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
//  18 bit, 6.12 fix point - ratio between long and medium exposure 
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_FORMATTER_FACTOR_ML_DEFAULT (0x1000)
#define HI_ISP_INPUT_FORMATTER_FACTOR_ML_DATASIZE (18)

// args: data (18-bit)
static __inline HI_VOID hi_isp_input_formatter_factor_ml_write(ISP_DEV IspDev, HI_U32 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x144);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x144, (((HI_U32) (data & 0x3ffff)) << 0) | (curr & 0xfffc0000));
}
static __inline HI_U32 hi_isp_input_formatter_factor_ml_read(ISP_DEV IspDev) {
    return (HI_U32)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x144) & 0x3ffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: factor MS
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
//  13 bit, 1.12 fix point - ratio between short and medium exposure 
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_FORMATTER_FACTOR_MS_DEFAULT (0x1000)
#define HI_ISP_INPUT_FORMATTER_FACTOR_MS_DATASIZE (13)

// args: data (13-bit)
static __inline HI_VOID hi_isp_input_formatter_factor_ms_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x148);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x148, (((HI_U32) (data & 0x1fff)) << 0) | (curr & 0xffffe000));
}
static __inline HI_U16 hi_isp_input_formatter_factor_ms_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x148) & 0x1fff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: black level
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
//  12 bit - black level of sensor data
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_FORMATTER_BLACK_LEVEL_DEFAULT (0)
#define HI_ISP_INPUT_FORMATTER_BLACK_LEVEL_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_input_formatter_black_level_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x14c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x14c, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_input_formatter_black_level_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x14c) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: knee point0
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
//  16 bit knee points
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_FORMATTER_KNEE_POINT0_DEFAULT (512)
#define HI_ISP_INPUT_FORMATTER_KNEE_POINT0_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_input_formatter_knee_point0_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x150);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x150, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_input_formatter_knee_point0_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x150) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: knee point1
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
//  16 bit knee points
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_FORMATTER_KNEE_POINT1_DEFAULT (1408)
#define HI_ISP_INPUT_FORMATTER_KNEE_POINT1_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_input_formatter_knee_point1_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x150);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x150, (((HI_U32) (data & 0xffff)) << 16) | (curr & 0xffff));
}
static __inline HI_U16 hi_isp_input_formatter_knee_point1_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x150) & 0xffff0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: knee point2
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
//  16 bit knee points
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_FORMATTER_KNEE_POINT2_DEFAULT (2177)
#define HI_ISP_INPUT_FORMATTER_KNEE_POINT2_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_input_formatter_knee_point2_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x154);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x154, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_input_formatter_knee_point2_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x154) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: slope0 select
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
//  4 bit slopes for companding table segments
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_FORMATTER_SLOPE0_SELECT_DEFAULT (2)
#define HI_ISP_INPUT_FORMATTER_SLOPE0_SELECT_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_isp_input_formatter_slope0_select_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x158);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x158, (((HI_U32) (data & 0xf)) << 0) | (curr & 0xfffffff0));
}
static __inline HI_U8 hi_isp_input_formatter_slope0_select_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x158) & 0xf) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: slope1 select
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
//  4 bit slopes for companding table segments
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_FORMATTER_SLOPE1_SELECT_DEFAULT (4)
#define HI_ISP_INPUT_FORMATTER_SLOPE1_SELECT_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_isp_input_formatter_slope1_select_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x158);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x158, (((HI_U32) (data & 0xf)) << 8) | (curr & 0xfffff0ff));
}
static __inline HI_U8 hi_isp_input_formatter_slope1_select_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x158) & 0xf00) >> 8);
}
// ------------------------------------------------------------------------------ //
// Register: slope2 select
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
//  4 bit slopes for companding table segments
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_FORMATTER_SLOPE2_SELECT_DEFAULT (6)
#define HI_ISP_INPUT_FORMATTER_SLOPE2_SELECT_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_isp_input_formatter_slope2_select_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x158);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x158, (((HI_U32) (data & 0xf)) << 16) | (curr & 0xfff0ffff));
}
static __inline HI_U8 hi_isp_input_formatter_slope2_select_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x158) & 0xf0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: slope3 select
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
//  4 bit slopes for companding table segments
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_FORMATTER_SLOPE3_SELECT_DEFAULT (9)
#define HI_ISP_INPUT_FORMATTER_SLOPE3_SELECT_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_isp_input_formatter_slope3_select_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x158);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x158, (((HI_U32) (data & 0xf)) << 24) | (curr & 0xf0ffffff));
}
static __inline HI_U8 hi_isp_input_formatter_slope3_select_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x158) & 0xf000000) >> 24);
}
// ------------------------------------------------------------------------------ //
// Group: video test gen
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Video test generator controls.  See ISP Guide for further details
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: test_pattern_off on
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Test pattern off-on: 0=off, 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_VIDEO_TEST_GEN_TEST_PATTERN_OFF_ON_DEFAULT (0)
#define HI_ISP_VIDEO_TEST_GEN_TEST_PATTERN_OFF_ON_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_video_test_gen_test_pattern_off_on_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x160);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x160, (((HI_U32) (data & 0x1)) << 0) | (curr & 0xfffffffe));
}
static __inline HI_U8 hi_isp_video_test_gen_test_pattern_off_on_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x160) & 0x1) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: bayer_rgb_i sel
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Bayer or rgb select for input video: 0=bayer, 1=rgb
// ------------------------------------------------------------------------------ //

#define HI_ISP_VIDEO_TEST_GEN_BAYER_RGB_I_SEL_DEFAULT (0)
#define HI_ISP_VIDEO_TEST_GEN_BAYER_RGB_I_SEL_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_video_test_gen_bayer_rgb_i_sel_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x160);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x160, (((HI_U32) (data & 0x1)) << 1) | (curr & 0xfffffffd));
}
static __inline HI_U8 hi_isp_video_test_gen_bayer_rgb_i_sel_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x160) & 0x2) >> 1);
}
// ------------------------------------------------------------------------------ //
// Register: bayer_rgb_o sel
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Bayer or rgb select for output video: 0=bayer, 1=rgb
// ------------------------------------------------------------------------------ //

#define HI_ISP_VIDEO_TEST_GEN_BAYER_RGB_O_SEL_DEFAULT (0)
#define HI_ISP_VIDEO_TEST_GEN_BAYER_RGB_O_SEL_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_video_test_gen_bayer_rgb_o_sel_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x160);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x160, (((HI_U32) (data & 0x1)) << 2) | (curr & 0xfffffffb));
}
static __inline HI_U8 hi_isp_video_test_gen_bayer_rgb_o_sel_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x160) & 0x4) >> 2);
}
// ------------------------------------------------------------------------------ //
// Register: pattern type
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Pattern type select: 0=Flat field,1=Horizontal gradient,2=Vertical Gradient,3=Vertical Bars,4=Rectangle,5-255=Default white frame on black
// ------------------------------------------------------------------------------ //

#define HI_ISP_VIDEO_TEST_GEN_PATTERN_TYPE_DEFAULT (0x03)
#define HI_ISP_VIDEO_TEST_GEN_PATTERN_TYPE_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_video_test_gen_pattern_type_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x164);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x164, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_video_test_gen_pattern_type_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x164) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: r backgnd
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Red background  value 16bit, MSB aligned to used width 
// ------------------------------------------------------------------------------ //

#define HI_ISP_VIDEO_TEST_GEN_R_BACKGND_DEFAULT (0xFFFF)
#define HI_ISP_VIDEO_TEST_GEN_R_BACKGND_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_video_test_gen_r_backgnd_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x168);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x168, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_video_test_gen_r_backgnd_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x168) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: g backgnd
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Green background value 16bit, MSB aligned to used width
// ------------------------------------------------------------------------------ //

#define HI_ISP_VIDEO_TEST_GEN_G_BACKGND_DEFAULT (0xFFFF)
#define HI_ISP_VIDEO_TEST_GEN_G_BACKGND_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_video_test_gen_g_backgnd_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x16c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x16c, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_video_test_gen_g_backgnd_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x16c) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: b backgnd
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Blue background value 16bit, MSB aligned to used width
// ------------------------------------------------------------------------------ //

#define HI_ISP_VIDEO_TEST_GEN_B_BACKGND_DEFAULT (0xFFFF)
#define HI_ISP_VIDEO_TEST_GEN_B_BACKGND_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_video_test_gen_b_backgnd_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x170);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x170, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_video_test_gen_b_backgnd_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x170) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: r foregnd
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Red foreground  value 16bit, MSB aligned to used width 
// ------------------------------------------------------------------------------ //

#define HI_ISP_VIDEO_TEST_GEN_R_FOREGND_DEFAULT (0x8FFF)
#define HI_ISP_VIDEO_TEST_GEN_R_FOREGND_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_video_test_gen_r_foregnd_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x174);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x174, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_video_test_gen_r_foregnd_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x174) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: g foregnd
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Green foreground value 16bit, MSB aligned to used width
// ------------------------------------------------------------------------------ //

#define HI_ISP_VIDEO_TEST_GEN_G_FOREGND_DEFAULT (0x8FFF)
#define HI_ISP_VIDEO_TEST_GEN_G_FOREGND_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_video_test_gen_g_foregnd_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x178);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x178, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_video_test_gen_g_foregnd_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x178) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: b foregnd
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Blue foreground value 16bit, MSB aligned to used width
// ------------------------------------------------------------------------------ //

#define HI_ISP_VIDEO_TEST_GEN_B_FOREGND_DEFAULT (0x8FFF)
#define HI_ISP_VIDEO_TEST_GEN_B_FOREGND_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_video_test_gen_b_foregnd_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x17c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x17c, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_video_test_gen_b_foregnd_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x17c) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: rgb gradient
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// RGB gradient increment per pixel (0-15) for first channel
// ------------------------------------------------------------------------------ //

#define HI_ISP_VIDEO_TEST_GEN_RGB_GRADIENT_DEFAULT (0x3CAA)
#define HI_ISP_VIDEO_TEST_GEN_RGB_GRADIENT_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_video_test_gen_rgb_gradient_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x180);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x180, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_video_test_gen_rgb_gradient_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x180) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: rgb gradient 2
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// RGB gradient increment for channel 2
// ------------------------------------------------------------------------------ //

#define HI_ISP_VIDEO_TEST_GEN_RGB_GRADIENT_2_DEFAULT (0x3CAA)
#define HI_ISP_VIDEO_TEST_GEN_RGB_GRADIENT_2_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_video_test_gen_rgb_gradient_2_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x180);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x180, (((HI_U32) (data & 0xffff)) << 16) | (curr & 0xffff));
}
static __inline HI_U16 hi_isp_video_test_gen_rgb_gradient_2_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x180) & 0xffff0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: rgb gradient 3
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// RGB gradient increment for channel 3
// ------------------------------------------------------------------------------ //

#define HI_ISP_VIDEO_TEST_GEN_RGB_GRADIENT_3_DEFAULT (0x3CAA)
#define HI_ISP_VIDEO_TEST_GEN_RGB_GRADIENT_3_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_video_test_gen_rgb_gradient_3_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x184);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x184, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_video_test_gen_rgb_gradient_3_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x184) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: rgb gradient 4
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// RGB gradient increment for channel 4
// ------------------------------------------------------------------------------ //

#define HI_ISP_VIDEO_TEST_GEN_RGB_GRADIENT_4_DEFAULT (0x3CAA)
#define HI_ISP_VIDEO_TEST_GEN_RGB_GRADIENT_4_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_video_test_gen_rgb_gradient_4_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x184);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x184, (((HI_U32) (data & 0xffff)) << 16) | (curr & 0xffff));
}
static __inline HI_U16 hi_isp_video_test_gen_rgb_gradient_4_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x184) & 0xffff0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: rgb_gradient start
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// RGB gradient start value for first channel 16bit, MSB aligned to used width
// ------------------------------------------------------------------------------ //

#define HI_ISP_VIDEO_TEST_GEN_RGB_GRADIENT_START_DEFAULT (0x0000)
#define HI_ISP_VIDEO_TEST_GEN_RGB_GRADIENT_START_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_video_test_gen_rgb_gradient_start_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x188);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x188, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_video_test_gen_rgb_gradient_start_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x188) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: rgb_gradient start 2
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// RGB gradient start value for first channel 2
// ------------------------------------------------------------------------------ //

#define HI_ISP_VIDEO_TEST_GEN_RGB_GRADIENT_START_2_DEFAULT (0x0000)
#define HI_ISP_VIDEO_TEST_GEN_RGB_GRADIENT_START_2_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_video_test_gen_rgb_gradient_start_2_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x188);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x188, (((HI_U32) (data & 0xffff)) << 16) | (curr & 0xffff));
}
static __inline HI_U16 hi_isp_video_test_gen_rgb_gradient_start_2_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x188) & 0xffff0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: rgb_gradient start 3
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// RGB gradient start value for first channel 3
// ------------------------------------------------------------------------------ //

#define HI_ISP_VIDEO_TEST_GEN_RGB_GRADIENT_START_3_DEFAULT (0x0000)
#define HI_ISP_VIDEO_TEST_GEN_RGB_GRADIENT_START_3_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_video_test_gen_rgb_gradient_start_3_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x18c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x18c, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_video_test_gen_rgb_gradient_start_3_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x18c) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: rgb_gradient start 4
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// RGB gradient start value for first channel 4
// ------------------------------------------------------------------------------ //

#define HI_ISP_VIDEO_TEST_GEN_RGB_GRADIENT_START_4_DEFAULT (0x0000)
#define HI_ISP_VIDEO_TEST_GEN_RGB_GRADIENT_START_4_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_video_test_gen_rgb_gradient_start_4_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x18c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x18c, (((HI_U32) (data & 0xffff)) << 16) | (curr & 0xffff));
}
static __inline HI_U16 hi_isp_video_test_gen_rgb_gradient_start_4_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x18c) & 0xffff0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: rect top
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
//  Rectangle top line number 1-n
// ------------------------------------------------------------------------------ //

#define HI_ISP_VIDEO_TEST_GEN_RECT_TOP_DEFAULT (0x0001)
#define HI_ISP_VIDEO_TEST_GEN_RECT_TOP_DATASIZE (14)

// args: data (14-bit)
static __inline HI_VOID hi_isp_video_test_gen_rect_top_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x190);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x190, (((HI_U32) (data & 0x3fff)) << 0) | (curr & 0xffffc000));
}
static __inline HI_U16 hi_isp_video_test_gen_rect_top_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x190) & 0x3fff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: rect bot
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
//  Rectangle bottom line number 1-n
// ------------------------------------------------------------------------------ //

#define HI_ISP_VIDEO_TEST_GEN_RECT_BOT_DEFAULT (0x0100)
#define HI_ISP_VIDEO_TEST_GEN_RECT_BOT_DATASIZE (14)

// args: data (14-bit)
static __inline HI_VOID hi_isp_video_test_gen_rect_bot_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x190);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x190, (((HI_U32) (data & 0x3fff)) << 16) | (curr & 0xc000ffff));
}
static __inline HI_U16 hi_isp_video_test_gen_rect_bot_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x190) & 0x3fff0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: rect left
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
//  Rectangle left pixel number 1-n
// ------------------------------------------------------------------------------ //

#define HI_ISP_VIDEO_TEST_GEN_RECT_LEFT_DEFAULT (0x0001)
#define HI_ISP_VIDEO_TEST_GEN_RECT_LEFT_DATASIZE (14)

// args: data (14-bit)
static __inline HI_VOID hi_isp_video_test_gen_rect_left_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x194);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x194, (((HI_U32) (data & 0x3fff)) << 0) | (curr & 0xffffc000));
}
static __inline HI_U16 hi_isp_video_test_gen_rect_left_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x194) & 0x3fff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: rect right
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
//  Rectangle right pixel number 1-n
// ------------------------------------------------------------------------------ //

#define HI_ISP_VIDEO_TEST_GEN_RECT_RIGHT_DEFAULT (0x0100)
#define HI_ISP_VIDEO_TEST_GEN_RECT_RIGHT_DATASIZE (14)

// args: data (14-bit)
static __inline HI_VOID hi_isp_video_test_gen_rect_right_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x198);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x198, (((HI_U32) (data & 0x3fff)) << 0) | (curr & 0xffffc000));
}
static __inline HI_U16 hi_isp_video_test_gen_rect_right_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x198) & 0x3fff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Group: Sensor Offset
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Black offset subtraction for each color channel
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: Black 00
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Black offset for color channel 00 (R)
// ------------------------------------------------------------------------------ //

#define HI_ISP_SENSOR_OFFSET_BLACK_00_DEFAULT (0x00)
#define HI_ISP_SENSOR_OFFSET_BLACK_00_DATASIZE (20)

// args: data (20-bit)
static __inline HI_VOID hi_isp_sensor_offset_black_00_write(ISP_DEV IspDev, HI_U32 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1a0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1a0, (((HI_U32) (data & 0xfffff)) << 0) | (curr & 0xfff00000));
}
static __inline HI_U32 hi_isp_sensor_offset_black_00_read(ISP_DEV IspDev) {
    return (HI_U32)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1a0) & 0xfffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Black 01
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Black offset for color channel 01 (Gr)
// ------------------------------------------------------------------------------ //

#define HI_ISP_SENSOR_OFFSET_BLACK_01_DEFAULT (0x00)
#define HI_ISP_SENSOR_OFFSET_BLACK_01_DATASIZE (20)

// args: data (20-bit)
static __inline HI_VOID hi_isp_sensor_offset_black_01_write(ISP_DEV IspDev, HI_U32 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1a4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1a4, (((HI_U32) (data & 0xfffff)) << 0) | (curr & 0xfff00000));
}
static __inline HI_U32 hi_isp_sensor_offset_black_01_read(ISP_DEV IspDev) {
    return (HI_U32)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1a4) & 0xfffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Black 10
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Black offset for color channel 10 (Gb)
// ------------------------------------------------------------------------------ //

#define HI_ISP_SENSOR_OFFSET_BLACK_10_DEFAULT (0x00)
#define HI_ISP_SENSOR_OFFSET_BLACK_10_DATASIZE (20)

// args: data (20-bit)
static __inline HI_VOID hi_isp_sensor_offset_black_10_write(ISP_DEV IspDev, HI_U32 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1a8);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1a8, (((HI_U32) (data & 0xfffff)) << 0) | (curr & 0xfff00000));
}
static __inline HI_U32 hi_isp_sensor_offset_black_10_read(ISP_DEV IspDev) {
    return (HI_U32)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1a8) & 0xfffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Black 11
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Black offset for color channel 11 (B)
// ------------------------------------------------------------------------------ //

#define HI_ISP_SENSOR_OFFSET_BLACK_11_DEFAULT (0x00)
#define HI_ISP_SENSOR_OFFSET_BLACK_11_DATASIZE (20)

// args: data (20-bit)
static __inline HI_VOID hi_isp_sensor_offset_black_11_write(ISP_DEV IspDev, HI_U32 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1ac);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1ac, (((HI_U32) (data & 0xfffff)) << 0) | (curr & 0xfff00000));
}
static __inline HI_U32 hi_isp_sensor_offset_black_11_read(ISP_DEV IspDev) {
    return (HI_U32)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1ac) & 0xfffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Group: Sensor Offset WDR 1
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Black offset subtraction for each color channel and exposure
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: Black 00
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Black offset for color channel 00 (R)
// ------------------------------------------------------------------------------ //

#define HI_ISP_SENSOR_OFFSET_WDR_1_BLACK_00_DEFAULT (0x00)
#define HI_ISP_SENSOR_OFFSET_WDR_1_BLACK_00_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_sensor_offset_wdr_1_black_00_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xc0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xc0, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_sensor_offset_wdr_1_black_00_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xc0) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Black 01
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Black offset for color channel 01 (Gr)
// ------------------------------------------------------------------------------ //

#define HI_ISP_SENSOR_OFFSET_WDR_1_BLACK_01_DEFAULT (0x00)
#define HI_ISP_SENSOR_OFFSET_WDR_1_BLACK_01_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_sensor_offset_wdr_1_black_01_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xc4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xc4, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_sensor_offset_wdr_1_black_01_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xc4) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Black 10
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Black offset for color channel 10 (Gb)
// ------------------------------------------------------------------------------ //

#define HI_ISP_SENSOR_OFFSET_WDR_1_BLACK_10_DEFAULT (0x00)
#define HI_ISP_SENSOR_OFFSET_WDR_1_BLACK_10_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_sensor_offset_wdr_1_black_10_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xc8);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xc8, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_sensor_offset_wdr_1_black_10_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xc8) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Black 11
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Black offset for color channel 11 (B)
// ------------------------------------------------------------------------------ //

#define HI_ISP_SENSOR_OFFSET_WDR_1_BLACK_11_DEFAULT (0x00)
#define HI_ISP_SENSOR_OFFSET_WDR_1_BLACK_11_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_sensor_offset_wdr_1_black_11_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xcc);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xcc, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_sensor_offset_wdr_1_black_11_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xcc) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Group: Sensor Offset WDR 2
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Black offset subtraction for each color channel and exposure
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: Black 00
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Black offset for color channel 00 (R)
// ------------------------------------------------------------------------------ //

#define HI_ISP_SENSOR_OFFSET_WDR_2_BLACK_00_DEFAULT (0x00)
#define HI_ISP_SENSOR_OFFSET_WDR_2_BLACK_00_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_sensor_offset_wdr_2_black_00_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xd0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xd0, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_sensor_offset_wdr_2_black_00_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xd0) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Black 01
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Black offset for color channel 01 (Gr)
// ------------------------------------------------------------------------------ //

#define HI_ISP_SENSOR_OFFSET_WDR_2_BLACK_01_DEFAULT (0x00)
#define HI_ISP_SENSOR_OFFSET_WDR_2_BLACK_01_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_sensor_offset_wdr_2_black_01_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xd4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xd4, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_sensor_offset_wdr_2_black_01_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xd4) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Black 10
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Black offset for color channel 10 (Gb)
// ------------------------------------------------------------------------------ //

#define HI_ISP_SENSOR_OFFSET_WDR_2_BLACK_10_DEFAULT (0x00)
#define HI_ISP_SENSOR_OFFSET_WDR_2_BLACK_10_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_sensor_offset_wdr_2_black_10_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xd8);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xd8, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_sensor_offset_wdr_2_black_10_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xd8) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Black 11
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Black offset for color channel 11 (B)
// ------------------------------------------------------------------------------ //

#define HI_ISP_SENSOR_OFFSET_WDR_2_BLACK_11_DEFAULT (0x00)
#define HI_ISP_SENSOR_OFFSET_WDR_2_BLACK_11_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_sensor_offset_wdr_2_black_11_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xdc);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xdc, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_sensor_offset_wdr_2_black_11_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xdc) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Group: Sensor Offset WDR 3
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Black offset subtraction for each color channel and exposure
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: Black 00
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Black offset for color channel 00 (R)
// ------------------------------------------------------------------------------ //

#define HI_ISP_SENSOR_OFFSET_WDR_3_BLACK_00_DEFAULT (0x00)
#define HI_ISP_SENSOR_OFFSET_WDR_3_BLACK_00_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_sensor_offset_wdr_3_black_00_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xe0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xe0, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_sensor_offset_wdr_3_black_00_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xe0) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Black 01
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Black offset for color channel 01 (Gr)
// ------------------------------------------------------------------------------ //

#define HI_ISP_SENSOR_OFFSET_WDR_3_BLACK_01_DEFAULT (0x00)
#define HI_ISP_SENSOR_OFFSET_WDR_3_BLACK_01_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_sensor_offset_wdr_3_black_01_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xe4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xe4, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_sensor_offset_wdr_3_black_01_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xe4) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Black 10
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Black offset for color channel 10 (Gb)
// ------------------------------------------------------------------------------ //

#define HI_ISP_SENSOR_OFFSET_WDR_3_BLACK_10_DEFAULT (0x00)
#define HI_ISP_SENSOR_OFFSET_WDR_3_BLACK_10_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_sensor_offset_wdr_3_black_10_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xe8);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xe8, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_sensor_offset_wdr_3_black_10_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xe8) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Black 11
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Black offset for color channel 11 (B)
// ------------------------------------------------------------------------------ //

#define HI_ISP_SENSOR_OFFSET_WDR_3_BLACK_11_DEFAULT (0x00)
#define HI_ISP_SENSOR_OFFSET_WDR_3_BLACK_11_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_sensor_offset_wdr_3_black_11_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xec);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xec, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_sensor_offset_wdr_3_black_11_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xec) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Group: Sensor Offset WDR 4
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Black offset subtraction for each color channel and exposure
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: Black 00
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Black offset for color channel 00 (R)
// ------------------------------------------------------------------------------ //

#define HI_ISP_SENSOR_OFFSET_WDR_4_BLACK_00_DEFAULT (0x00)
#define HI_ISP_SENSOR_OFFSET_WDR_4_BLACK_00_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_sensor_offset_wdr_4_black_00_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xf0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xf0, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_sensor_offset_wdr_4_black_00_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xf0) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Black 01
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Black offset for color channel 01 (Gr)
// ------------------------------------------------------------------------------ //

#define HI_ISP_SENSOR_OFFSET_WDR_4_BLACK_01_DEFAULT (0x00)
#define HI_ISP_SENSOR_OFFSET_WDR_4_BLACK_01_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_sensor_offset_wdr_4_black_01_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xf4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xf4, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_sensor_offset_wdr_4_black_01_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xf4) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Black 10
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Black offset for color channel 10 (Gb)
// ------------------------------------------------------------------------------ //

#define HI_ISP_SENSOR_OFFSET_WDR_4_BLACK_10_DEFAULT (0x00)
#define HI_ISP_SENSOR_OFFSET_WDR_4_BLACK_10_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_sensor_offset_wdr_4_black_10_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xf8);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xf8, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_sensor_offset_wdr_4_black_10_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xf8) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Black 11
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Black offset for color channel 11 (B)
// ------------------------------------------------------------------------------ //

#define HI_ISP_SENSOR_OFFSET_WDR_4_BLACK_11_DEFAULT (0x00)
#define HI_ISP_SENSOR_OFFSET_WDR_4_BLACK_11_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_sensor_offset_wdr_4_black_11_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xfc);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xfc, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_sensor_offset_wdr_4_black_11_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xfc) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Group: WDR Gain
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Gain adjustment for the WDR stitching 
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: Gain WDR1
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Gain applied to ch1 data in 5.8 format
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_GAIN_GAIN_WDR1_DEFAULT (0x100)
#define HI_ISP_FRAME_STITCH_GAIN_GAIN_WDR1_DATASIZE (13)

// args: data (13-bit)
static __inline HI_VOID hi_isp_frame_stitch_gain_gain_wdr1_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xb0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xb0, (((HI_U32) (data & 0x1fff)) << 0) | (curr & 0xffffe000));
}
static __inline HI_U16 hi_isp_frame_stitch_gain_gain_wdr1_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xb0) & 0x1fff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Gain WDR2
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Gain applied to ch2 data in 5.8 format
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_GAIN_GAIN_WDR2_DEFAULT (0x100)
#define HI_ISP_FRAME_STITCH_GAIN_GAIN_WDR2_DATASIZE (13)

// args: data (13-bit)
static __inline HI_VOID hi_isp_frame_stitch_gain_gain_wdr2_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xb0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xb0, (((HI_U32) (data & 0x1fff)) << 16) | (curr & 0xe000ffff));
}
static __inline HI_U16 hi_isp_frame_stitch_gain_gain_wdr2_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xb0) & 0x1fff0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: Gain WDR3
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Gain applied to ch3 data in 5.8 format
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_GAIN_GAIN_WDR3_DEFAULT (0x100)
#define HI_ISP_FRAME_STITCH_GAIN_GAIN_WDR3_DATASIZE (13)

// args: data (13-bit)
static __inline HI_VOID hi_isp_frame_stitch_gain_gain_wdr3_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xb4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xb4, (((HI_U32) (data & 0x1fff)) << 0) | (curr & 0xffffe000));
}
static __inline HI_U16 hi_isp_frame_stitch_gain_gain_wdr3_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xb4) & 0x1fff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Gain WDR4
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Gain applied to ch4 data in 5.8 format
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_GAIN_GAIN_WDR4_DEFAULT (0x100)
#define HI_ISP_FRAME_STITCH_GAIN_GAIN_WDR4_DATASIZE (13)

// args: data (13-bit)
static __inline HI_VOID hi_isp_frame_stitch_gain_gain_wdr4_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xb4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xb4, (((HI_U32) (data & 0x1fff)) << 16) | (curr & 0xe000ffff));
}
static __inline HI_U16 hi_isp_frame_stitch_gain_gain_wdr4_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xb4) & 0x1fff0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: Offset WDR1
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Sensor offset applied to ch1 data
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_GAIN_OFFSET_WDR1_DEFAULT (0x0000)
#define HI_ISP_FRAME_STITCH_GAIN_OFFSET_WDR1_DATASIZE (14)

// args: data (14-bit)
static __inline HI_VOID hi_isp_frame_stitch_gain_offset_wdr1_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xb8);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xb8, (((HI_U32) (data & 0x3fff)) << 0) | (curr & 0xffffc000));
}
static __inline HI_U16 hi_isp_frame_stitch_gain_offset_wdr1_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xb8) & 0x3fff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Offset WDR2
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Sensor offset applied to ch2 data
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_GAIN_OFFSET_WDR2_DEFAULT (0x0000)
#define HI_ISP_FRAME_STITCH_GAIN_OFFSET_WDR2_DATASIZE (14)

// args: data (14-bit)
static __inline HI_VOID hi_isp_frame_stitch_gain_offset_wdr2_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xb8);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xb8, (((HI_U32) (data & 0x3fff)) << 16) | (curr & 0xc000ffff));
}
static __inline HI_U16 hi_isp_frame_stitch_gain_offset_wdr2_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xb8) & 0x3fff0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: Offset WDR3
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Sensor offset applied to ch3 data
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_GAIN_OFFSET_WDR3_DEFAULT (0x0000)
#define HI_ISP_FRAME_STITCH_GAIN_OFFSET_WDR3_DATASIZE (14)

// args: data (14-bit)
static __inline HI_VOID hi_isp_frame_stitch_gain_offset_wdr3_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xbc);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xbc, (((HI_U32) (data & 0x3fff)) << 0) | (curr & 0xffffc000));
}
static __inline HI_U16 hi_isp_frame_stitch_gain_offset_wdr3_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xbc) & 0x3fff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Offset WDR4
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Sensor offset applied to ch4 data
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_GAIN_OFFSET_WDR4_DEFAULT (0x0000)
#define HI_ISP_FRAME_STITCH_GAIN_OFFSET_WDR4_DATASIZE (14)

// args: data (14-bit)
static __inline HI_VOID hi_isp_frame_stitch_gain_offset_wdr4_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xbc);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xbc, (((HI_U32) (data & 0x3fff)) << 16) | (curr & 0xc000ffff));
}
static __inline HI_U16 hi_isp_frame_stitch_gain_offset_wdr4_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xbc) & 0x3fff0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Group: Noise Profile RAW frontend 1
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Noise profile controls for RAW frontend
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: Exp Thresh
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Threshold for determining long/short exposure data
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_1_EXP_THRESH_DEFAULT (0xFFFF)
#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_1_EXP_THRESH_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_noise_profile_raw_frontend_1_exp_thresh_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1b0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1b0, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_noise_profile_raw_frontend_1_exp_thresh_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1b0) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Short Ratio
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Multiplier applied to short exposure data for noise profile calculation
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_1_SHORT_RATIO_DEFAULT (0x20)
#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_1_SHORT_RATIO_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_noise_profile_raw_frontend_1_short_ratio_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1b4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1b4, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_noise_profile_raw_frontend_1_short_ratio_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1b4) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Long Ratio
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Multiplier applied to long exposure data for noise profile calculation
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_1_LONG_RATIO_DEFAULT (0x04)
#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_1_LONG_RATIO_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_noise_profile_raw_frontend_1_long_ratio_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1b8);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1b8, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_noise_profile_raw_frontend_1_long_ratio_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1b8) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: NP off
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Noise profile black level offset
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_1_NP_OFF_DEFAULT (0)
#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_1_NP_OFF_DATASIZE (7)

// args: data (7-bit)
static __inline HI_VOID hi_isp_noise_profile_raw_frontend_1_np_off_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1bc);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1bc, (((HI_U32) (data & 0x7f)) << 0) | (curr & 0xffffff80));
}
static __inline HI_U8 hi_isp_noise_profile_raw_frontend_1_np_off_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1bc) & 0x7f) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: NP off reflect
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 
//          Defines how values below black level are obtained.
//          0: Repeat the first table entry.
//          1: Reflect the noise profile curve below black level.
//        
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_1_NP_OFF_REFLECT_DEFAULT (0)
#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_1_NP_OFF_REFLECT_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_noise_profile_raw_frontend_1_np_off_reflect_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1bc);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1bc, (((HI_U32) (data & 0x1)) << 7) | (curr & 0xffffff7f));
}
static __inline HI_U8 hi_isp_noise_profile_raw_frontend_1_np_off_reflect_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1bc) & 0x80) >> 7);
}
// ------------------------------------------------------------------------------ //
// Group: Noise Profile RAW frontend 2
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Noise profile controls for RAW frontend
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: Exp Thresh
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Threshold for determining long/short exposure data
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_2_EXP_THRESH_DEFAULT (0xFFFF)
#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_2_EXP_THRESH_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_noise_profile_raw_frontend_2_exp_thresh_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1c0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1c0, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_noise_profile_raw_frontend_2_exp_thresh_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1c0) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Short Ratio
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Multiplier applied to short exposure data for noise profile calculation
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_2_SHORT_RATIO_DEFAULT (0x20)
#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_2_SHORT_RATIO_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_noise_profile_raw_frontend_2_short_ratio_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1c4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1c4, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_noise_profile_raw_frontend_2_short_ratio_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1c4) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Long Ratio
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Multiplier applied to long exposure data for noise profile calculation
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_2_LONG_RATIO_DEFAULT (0x04)
#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_2_LONG_RATIO_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_noise_profile_raw_frontend_2_long_ratio_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1c8);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1c8, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_noise_profile_raw_frontend_2_long_ratio_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1c8) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: NP off
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Noise profile black level offset
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_2_NP_OFF_DEFAULT (0)
#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_2_NP_OFF_DATASIZE (7)

// args: data (7-bit)
static __inline HI_VOID hi_isp_noise_profile_raw_frontend_2_np_off_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1cc);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1cc, (((HI_U32) (data & 0x7f)) << 0) | (curr & 0xffffff80));
}
static __inline HI_U8 hi_isp_noise_profile_raw_frontend_2_np_off_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1cc) & 0x7f) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: NP off reflect
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 
//          Defines how values below black level are obtained.
//          0: Repeat the first table entry.
//          1: Reflect the noise profile curve below black level.
//        
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_2_NP_OFF_REFLECT_DEFAULT (0)
#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_2_NP_OFF_REFLECT_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_noise_profile_raw_frontend_2_np_off_reflect_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1cc);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1cc, (((HI_U32) (data & 0x1)) << 7) | (curr & 0xffffff7f));
}
static __inline HI_U8 hi_isp_noise_profile_raw_frontend_2_np_off_reflect_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1cc) & 0x80) >> 7);
}
// ------------------------------------------------------------------------------ //
// Group: Noise Profile RAW frontend 3
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Noise profile controls for RAW frontend
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: Exp Thresh
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Threshold for determining long/short exposure data
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_3_EXP_THRESH_DEFAULT (0xFFFF)
#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_3_EXP_THRESH_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_noise_profile_raw_frontend_exp_thresh_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1d0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1d0, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_noise_profile_raw_frontend_exp_thresh_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1d0) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Short Ratio
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Multiplier applied to short exposure data for noise profile calculation
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_3_SHORT_RATIO_DEFAULT (0x20)
#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_3_SHORT_RATIO_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_noise_profile_raw_frontend_short_ratio_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1d4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1d4, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_noise_profile_raw_frontend_short_ratio_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1d4) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Long Ratio
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Multiplier applied to long exposure data for noise profile calculation
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_3_LONG_RATIO_DEFAULT (0x04)
#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_3_LONG_RATIO_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_noise_profile_raw_frontend_long_ratio_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1d8);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1d8, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_noise_profile_raw_frontend_long_ratio_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1d8) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: NP off
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Noise profile black level offset
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_3_NP_OFF_DEFAULT (0)
#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_3_NP_OFF_DATASIZE (7)

// args: data (7-bit)
static __inline HI_VOID hi_isp_noise_profile_raw_frontend_np_off_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1dc);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1dc, (((HI_U32) (data & 0x7f)) << 0) | (curr & 0xffffff80));
}
static __inline HI_U8 hi_isp_noise_profile_raw_frontend_np_off_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1dc) & 0x7f) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: NP off reflect
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 
//          Defines how values below black level are obtained.
//          0: Repeat the first table entry.
//          1: Reflect the noise profile curve below black level.
//        
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_3_NP_OFF_REFLECT_DEFAULT (0)
#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_3_NP_OFF_REFLECT_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_noise_profile_raw_frontend_np_off_reflect_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1dc);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1dc, (((HI_U32) (data & 0x1)) << 7) | (curr & 0xffffff7f));
}
static __inline HI_U8 hi_isp_noise_profile_raw_frontend_np_off_reflect_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1dc) & 0x80) >> 7);
}
// ------------------------------------------------------------------------------ //
// Group: Noise Profile RAW frontend 4
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Noise profile controls for RAW frontend
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: Exp Thresh
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Threshold for determining long/short exposure data
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_4_EXP_THRESH_DEFAULT (0xFFFF)
#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_4_EXP_THRESH_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_noise_profile_raw_frontend_4_exp_thresh_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1e0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1e0, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_noise_profile_raw_frontend_4_exp_thresh_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1e0) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Short Ratio
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Multiplier applied to short exposure data for noise profile calculation
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_4_SHORT_RATIO_DEFAULT (0x20)
#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_4_SHORT_RATIO_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_noise_profile_raw_frontend_4_short_ratio_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1e4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1e4, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_noise_profile_raw_frontend_4_short_ratio_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1e4) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Long Ratio
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Multiplier applied to long exposure data for noise profile calculation
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_4_LONG_RATIO_DEFAULT (0x04)
#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_4_LONG_RATIO_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_noise_profile_raw_frontend_4_long_ratio_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1e8);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1e8, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_noise_profile_raw_frontend_4_long_ratio_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1e8) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: NP off
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Noise profile black level offset
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_4_NP_OFF_DEFAULT (0)
#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_4_NP_OFF_DATASIZE (7)

// args: data (7-bit)
static __inline HI_VOID hi_isp_noise_profile_raw_frontend_4_np_off_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1ec);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1ec, (((HI_U32) (data & 0x7f)) << 0) | (curr & 0xffffff80));
}
static __inline HI_U8 hi_isp_noise_profile_raw_frontend_4_np_off_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1ec) & 0x7f) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: NP off reflect
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 
//          Defines how values below black level are obtained.
//          0: Repeat the first table entry.
//          1: Reflect the noise profile curve below black level.
//        
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_4_NP_OFF_REFLECT_DEFAULT (0)
#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_4_NP_OFF_REFLECT_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_noise_profile_raw_frontend_4_np_off_reflect_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1ec);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1ec, (((HI_U32) (data & 0x1)) << 7) | (curr & 0xffffff7f));
}
static __inline HI_U8 hi_isp_noise_profile_raw_frontend_4_np_off_reflect_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1ec) & 0x80) >> 7);
}
// ------------------------------------------------------------------------------ //
// Group: RAW Frontend 1
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// RAW frontend processing
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_green_equalize_ge_enable
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Green equalization enable: 0=off, 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_1_GE_ENABLE_DEFAULT (0)
#define HI_ISP_RAW_FRONTEND_1_GE_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_green_equalize_ge_enable_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x200);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x200, (((HI_U32) (data & 0x1)) << 0) | (curr & 0xfffffffe));
}
static __inline HI_U8 hi_isp_green_equalize_ge_enable_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x200) & 0x1) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_green_equalize_dp_enable
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Dynamic Defect Pixel enable: 0=off, 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_1_DP_ENABLE_DEFAULT (0)
#define HI_ISP_RAW_FRONTEND_1_DP_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_green_equalize_dp_enable_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x200);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x200, (((HI_U32) (data & 0x1)) << 2) | (curr & 0xfffffffb));
}
static __inline HI_U8 hi_isp_green_equalize_dp_enable_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x200) & 0x4) >> 2);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_green_equalize_show_dynamic_defect_pixel
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Show Defect Pixel: 0=off, 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_1_SHOW_DYNAMIC_DEFECT_PIXEL_DEFAULT (0)
#define HI_ISP_RAW_FRONTEND_1_SHOW_DYNAMIC_DEFECT_PIXEL_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_green_equalize_show_dynamic_defect_pixel_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x200);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x200, (((HI_U32) (data & 0x1)) << 3) | (curr & 0xfffffff7));
}
static __inline HI_U8 hi_isp_green_equalize_show_dynamic_defect_pixel_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x200) & 0x8) >> 3);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_green_equalize_dark_disable
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Disable detection of dark pixels
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_1_DARK_DISABLE_DEFAULT (0)
#define HI_ISP_RAW_FRONTEND_1_DARK_DISABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_green_equalize_dark_disable_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x200);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x200, (((HI_U32) (data & 0x1)) << 6) | (curr & 0xffffffbf));
}
static __inline HI_U8 hi_isp_green_equalize_dark_disable_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x200) & 0x40) >> 6);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_green_equalize_bright_disable
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Disable detection of bright pixels
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_1_BRIGHT_DISABLE_DEFAULT (0)
#define HI_ISP_RAW_FRONTEND_1_BRIGHT_DISABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_green_equalize_bright_disable_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x200);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x200, (((HI_U32) (data & 0x1)) << 7) | (curr & 0xffffff7f));
}
static __inline HI_U8 hi_isp_green_equalize_bright_disable_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x200) & 0x80) >> 7);
}
// ------------------------------------------------------------------------------ //
// Register: ge strength
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Controls strength of Green equalization.  Set during calibration.
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_1_GE_STRENGTH_DEFAULT (0x00)
#define HI_ISP_RAW_FRONTEND_1_GE_STRENGTH_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_green_equalize_ge_strength_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x204);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x204, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_green_equalize_ge_strength_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x204) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: debug sel
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Debug selection port
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_1_DEBUG_SEL_DEFAULT (0x0)
#define HI_ISP_RAW_FRONTEND_1_DEBUG_SEL_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_green_equalize_debug_sel_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x208);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x208, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_green_equalize_debug_sel_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x208) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_green_equalize_dp_threshold
// ------------------------------------------------------------------------------ //
// Controls Config of green equalization
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_1_DP_THRESHOLD_DEFAULT (0x040)
#define HI_ISP_RAW_FRONTEND_1_DP_THRESHOLD_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_green_equalize_dp_threshold_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x20c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x20c, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_green_equalize_dp_threshold_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x20c) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_green_equalize_ge_threshold
// ------------------------------------------------------------------------------ //
// Controls Config of green equalization
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_1_GE_THRESHOLD_DEFAULT (0x400)
#define HI_ISP_RAW_FRONTEND_1_GE_THRESHOLD_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_green_equalize_ge_threshold_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x210);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x210, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_green_equalize_ge_threshold_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x210) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_green_equalize_dp_slope
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Slope for HP Mask function
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_1_DP_SLOPE_DEFAULT (0x200)
#define HI_ISP_RAW_FRONTEND_1_DP_SLOPE_DATASIZE (12)
#define HI_ISP_GREEN_EQUALIZE_DP_SLOPE_DEFAULT (0x200)
#define HI_ISP_GREEN_EQUALIZE_DP_SLOPE_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_green_equalize_dp_slope_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x214);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x214, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_green_equalize_dp_slope_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x214) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_green_equalize_ge_slope
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Slope for GE Mask function
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_1_GE_SLOPE_DEFAULT (0x0AA)
#define HI_ISP_RAW_FRONTEND_1_GE_SLOPE_DATASIZE (12)
#define HI_ISP_GREEN_EQUALIZE_GE_SLOPE_DEFAULT (0x0AA)
#define HI_ISP_GREEN_EQUALIZE_GE_SLOPE_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_green_equalize_ge_slope_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x218);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x218, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_green_equalize_ge_slope_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x218) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_green_equalize_ge_sens
// ------------------------------------------------------------------------------ //
// Controls the sensitivity of STD-DEV
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_1_GE_SENS_DEFAULT (0x80)
#define HI_ISP_RAW_FRONTEND_1_GE_SENS_DATASIZE (8)
#define HI_ISP_GREEN_EQUALIZE_GE_SENS_DEFAULT (0x80)
#define HI_ISP_GREEN_EQUALIZE_GE_SENS_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_green_equalize_ge_sens_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x21c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x21c, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_green_equalize_ge_sens_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x21c) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_raw_frontend_hpdev_threshold
// ------------------------------------------------------------------------------ //
// Threshold for Deviation of other colors for HP
// ------------------------------------------------------------------------------ //
// Controls the aggressiveness of the dynamic defect pixel correction near edges. 
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_1_DPDEV_THRESHOLD_DEFAULT (0x266)
#define HI_ISP_RAW_FRONTEND_1_DPDEV_THRESHOLD_DATASIZE (12)
#define HI_ISP_RAW_FRONTEND_HPDEV_THRESHOLD_DEFAULT (0x266)
#define HI_ISP_RAW_FRONTEND_HPDEV_THRESHOLD_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_raw_frontend_hpdev_threshold_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x220);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x220, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_raw_frontend_hpdev_threshold_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x220) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_raw_frontend_line_thresh
// ------------------------------------------------------------------------------ //
// Line Threshold
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_1_LINE_THRESH_DEFAULT (0x150)
#define HI_ISP_RAW_FRONTEND_1_LINE_THRESH_DATASIZE (12)
#define HI_ISP_RAW_FRONTEND_LINE_THRESH_DEFAULT (0x150)
#define HI_ISP_RAW_FRONTEND_LINE_THRESH_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_raw_frontend_line_thresh_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x224);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x224, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_raw_frontend_line_thresh_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x224) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_raw_frontend_hp_blend
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 
//        Controls blending between non-directional and directional replacement values in dynamic defect pixel correction.
//        0x00 Replace detected defects with non-directional replacement value
//        0xFF Replace detected defects with directional replacement value
//        
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_1_DP_BLEND_DEFAULT (0x00)
#define HI_ISP_RAW_FRONTEND_1_DP_BLEND_DATASIZE (8)
#define HI_ISP_RAW_FRONTEND_HP_BLEND_DEFAULT (0x00)
#define HI_ISP_RAW_FRONTEND_HP_BLEND_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_raw_frontend_hp_blend_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x228);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x228, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_raw_frontend_hp_blend_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x228) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_raw_frontend_sigma_in
// ------------------------------------------------------------------------------ //
// Manual override of noise estimation
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_1_SIGMA_IN_DEFAULT (0x00)
#define HI_ISP_RAW_FRONTEND_1_SIGMA_IN_DATASIZE (12)
#define HI_ISP_RAW_FRONTEND_SIGMA_IN_DEFAULT (0x00)
#define HI_ISP_RAW_FRONTEND_SIGMA_IN_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_raw_frontend_sigma_in_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x22c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x22c, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_raw_frontend_sigma_in_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x22c) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_raw_frontend_thresh_short
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Noise threshold for short exposure data
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_1_THRESH_SHORT_DEFAULT (0x00)
#define HI_ISP_RAW_FRONTEND_1_THRESH_SHORT_DATASIZE (8)
#define HI_ISP_RAW_FRONTEND_THRESH_SHORT_DEFAULT (0x00)
#define HI_ISP_RAW_FRONTEND_THRESH_SHORT_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_raw_frontend_thresh_short_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x230);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x230, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_raw_frontend_thresh_short_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x230) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_raw_frontend_thresh_long
// ------------------------------------------------------------------------------ //
// Noise threshold for long exposure data
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_1_THRESH_LONG_DEFAULT (0x30)
#define HI_ISP_RAW_FRONTEND_1_THRESH_LONG_DATASIZE (8)
#define HI_ISP_RAW_FRONTEND_THRESH_LONG_DEFAULT (0x30)
#define HI_ISP_RAW_FRONTEND_THRESH_LONG_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_raw_frontend_thresh_long_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x234);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x234, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_raw_frontend_thresh_long_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x234) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Group: RAW Frontend 2
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// RAW frontend processing
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: ge enable
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Green equalization enable: 0=off, 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_2_GE_ENABLE_DEFAULT (0)
#define HI_ISP_RAW_FRONTEND_2_GE_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_raw_frontend_2_ge_enable_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x240);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x240, (((HI_U32) (data & 0x1)) << 0) | (curr & 0xfffffffe));
}
static __inline HI_U8 hi_isp_raw_frontend_2_ge_enable_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x240) & 0x1) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: dp enable
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Dynamic Defect Pixel enable: 0=off, 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_2_DP_ENABLE_DEFAULT (0)
#define HI_ISP_RAW_FRONTEND_2_DP_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_raw_frontend_2_dp_enable_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x240);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x240, (((HI_U32) (data & 0x1)) << 2) | (curr & 0xfffffffb));
}
static __inline HI_U8 hi_isp_raw_frontend_2_dp_enable_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x240) & 0x4) >> 2);
}
// ------------------------------------------------------------------------------ //
// Register: show dynamic defect pixel
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Show Defect Pixel: 0=off, 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_2_SHOW_DYNAMIC_DEFECT_PIXEL_DEFAULT (0)
#define HI_ISP_RAW_FRONTEND_2_SHOW_DYNAMIC_DEFECT_PIXEL_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_raw_frontend_2_show_dynamic_defect_pixel_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x240);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x240, (((HI_U32) (data & 0x1)) << 3) | (curr & 0xfffffff7));
}
static __inline HI_U8 hi_isp_raw_frontend_2_show_dynamic_defect_pixel_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x240) & 0x8) >> 3);
}
// ------------------------------------------------------------------------------ //
// Register: dark disable
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Disable detection of dark pixels
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_2_DARK_DISABLE_DEFAULT (0)
#define HI_ISP_RAW_FRONTEND_2_DARK_DISABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_raw_frontend_2_dark_disable_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x240);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x240, (((HI_U32) (data & 0x1)) << 6) | (curr & 0xffffffbf));
}
static __inline HI_U8 hi_isp_raw_frontend_2_dark_disable_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x240) & 0x40) >> 6);
}
// ------------------------------------------------------------------------------ //
// Register: bright disable
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Disable detection of bright pixels
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_2_BRIGHT_DISABLE_DEFAULT (0)
#define HI_ISP_RAW_FRONTEND_2_BRIGHT_DISABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_raw_frontend_2_bright_disable_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x240);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x240, (((HI_U32) (data & 0x1)) << 7) | (curr & 0xffffff7f));
}
static __inline HI_U8 hi_isp_raw_frontend_2_bright_disable_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x240) & 0x80) >> 7);
}
// ------------------------------------------------------------------------------ //
// Register: ge strength
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Controls strength of Green equalization.  Set during calibration.
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_2_GE_STRENGTH_DEFAULT (0x00)
#define HI_ISP_RAW_FRONTEND_2_GE_STRENGTH_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_raw_frontend_2_ge_strength_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x244);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x244, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_raw_frontend_2_ge_strength_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x244) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: dp threshold
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Defect pixel threshold. 
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_2_DP_THRESHOLD_DEFAULT (0x040)
#define HI_ISP_RAW_FRONTEND_2_DP_THRESHOLD_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_raw_frontend_2_dp_threshold_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x24c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x24c, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_raw_frontend_2_dp_threshold_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x24c) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: ge threshold
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// green equalization threshold
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_2_GE_THRESHOLD_DEFAULT (0x400)
#define HI_ISP_RAW_FRONTEND_2_GE_THRESHOLD_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_raw_frontend_2_ge_threshold_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x250);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x250, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_raw_frontend_2_ge_threshold_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x250) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: dp slope
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Slope for HP Mask function
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_2_DP_SLOPE_DEFAULT (0x200)
#define HI_ISP_RAW_FRONTEND_2_DP_SLOPE_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_raw_frontend_2_dp_slope_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x254);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x254, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_raw_frontend_2_dp_slope_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x254) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: ge slope
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Slope for GE Mask function
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_2_GE_SLOPE_DEFAULT (0x0AA)
#define HI_ISP_RAW_FRONTEND_2_GE_SLOPE_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_raw_frontend_2_ge_slope_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x258);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x258, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_raw_frontend_2_ge_slope_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x258) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: ge sens
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Controls the sensitivity of green equalization to edges.
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_2_GE_SENS_DEFAULT (0x80)
#define HI_ISP_RAW_FRONTEND_2_GE_SENS_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_raw_frontend_2_ge_sens_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25c, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_raw_frontend_2_ge_sens_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25c) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: dpdev threshold
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Controls the aggressiveness of the dynamic defect pixel correction near edges. 
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_2_DPDEV_THRESHOLD_DEFAULT (0x266)
#define HI_ISP_RAW_FRONTEND_2_DPDEV_THRESHOLD_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_raw_frontend_2_dpdev_threshold_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x260);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x260, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_raw_frontend_2_dpdev_threshold_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x260) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: line thresh
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Controls the directional nature of the dynamic defect pixel correction near edges..
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_2_LINE_THRESH_DEFAULT (0x150)
#define HI_ISP_RAW_FRONTEND_2_LINE_THRESH_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_raw_frontend_2_line_thresh_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x264);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x264, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_raw_frontend_2_line_thresh_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x264) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: dp blend
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 
//        Controls blending between non-directional and directional replacement values in dynamic defect pixel correction.
//        0x00 Replace detected defects with non-directional replacement value
//        0xFF Replace detected defects with directional replacement value
//        
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_2_DP_BLEND_DEFAULT (0x00)
#define HI_ISP_RAW_FRONTEND_2_DP_BLEND_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_raw_frontend_2_dp_blend_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x268);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x268, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_raw_frontend_2_dp_blend_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x268) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Sigma In
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Manual override of noise estimation 
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_2_SIGMA_IN_DEFAULT (0x00)
#define HI_ISP_RAW_FRONTEND_2_SIGMA_IN_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_raw_frontend_2_sigma_in_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26c, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_raw_frontend_2_sigma_in_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26c) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Thresh Short
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Noise threshold for short exposure data
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_2_THRESH_SHORT_DEFAULT (0x00)
#define HI_ISP_RAW_FRONTEND_2_THRESH_SHORT_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_raw_frontend_2_thresh_short_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x270);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x270, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_raw_frontend_2_thresh_short_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x270) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Thresh Long
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Noise threshold for long exposure data
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_2_THRESH_LONG_DEFAULT (0x30)
#define HI_ISP_RAW_FRONTEND_2_THRESH_LONG_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_raw_frontend_2_thresh_long_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x274);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x274, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_raw_frontend_2_thresh_long_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x274) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Group: Defect Pixel
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Detection and processing of static defect-pixels
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: Multi Ch Mode
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// configuration of 4k/2k
//          00: 4ch 4K mode
//          01: 2ch 4K mode
//          10: 2ch 2K mode
//          11: reserved
//        
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEFECT_PIXEL_MULTI_CH_MODE_DEFAULT (0)
#define HI_ISP_DEFECT_PIXEL_MULTI_CH_MODE_DATASIZE (2)

// args: data (2-bit)
static __inline HI_VOID hi_isp_defect_pixel_multi_ch_mode_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x280);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x280, (((HI_U32) (data & 0x3)) << 0) | (curr & 0xfffffffc));
}
static __inline HI_U8 hi_isp_defect_pixel_multi_ch_mode_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x280) & 0x3) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Pointer Reset1
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Reset static defect-pixel table pointer each frame - set this when defect-pixel table has been written from mcu
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEFECT_PIXEL_POINTER_RESET_DEFAULT (0)
#define HI_ISP_DEFECT_PIXEL_POINTER_RESET_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_defect_pixel_pointer_reset_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x284);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x284, (((HI_U32) (data & 0x1)) << 0) | (curr & 0xfffffffe));
}
static __inline HI_U8 hi_isp_defect_pixel_pointer_reset_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x284) & 0x1) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Show Reference1
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// For debug purposes.  Show reference values which are compared with actual values to detect bad pixels
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEFECT_PIXEL_SHOW_REFERENCE1_DEFAULT (0)
#define HI_ISP_DEFECT_PIXEL_SHOW_REFERENCE1_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_defect_pixel_show_reference_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x284);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x284, (((HI_U32) (data & 0x1)) << 1) | (curr & 0xfffffffd));
}
static __inline HI_U8 hi_isp_defect_pixel_show_reference_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x284) & 0x2) >> 1);
}
// ------------------------------------------------------------------------------ //
// Register: Enable1
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Correction enable: 0=off 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEFECT_PIXEL_ENABLE_DEFAULT (0)
#define HI_ISP_DEFECT_PIXEL_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_defect_pixel_enable_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x284);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x284, (((HI_U32) (data & 0x1)) << 2) | (curr & 0xfffffffb));
}
static __inline HI_U8 hi_isp_defect_pixel_enable_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x284) & 0x4) >> 2);
}
// ------------------------------------------------------------------------------ //
// Register: Show Static Defect Pixels1
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Show which pixels have been detected as bad
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEFECT_PIXEL_SHOW_STATIC_DEFECT_PIXELS1_DEFAULT (0)
#define HI_ISP_DEFECT_PIXEL_SHOW_STATIC_DEFECT_PIXELS1_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_defect_pixel_show_static_defect_pixels_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x284);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x284, (((HI_U32) (data & 0x1)) << 3) | (curr & 0xfffffff7));
}
static __inline HI_U8 hi_isp_defect_pixel_show_static_defect_pixels_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x284) & 0x8) >> 3);
}
// ------------------------------------------------------------------------------ //
// Register: Detection Trigger1
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Starts detection on 0-1 transition
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEFECT_PIXEL_DETECTION_TRIGGER_DEFAULT (0)
#define HI_ISP_DEFECT_PIXEL_DETECTION_TRIGGER_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_defect_pixel_detection_trigger_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x284);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x284, (((HI_U32) (data & 0x1)) << 4) | (curr & 0xffffffef));
}
static __inline HI_U8 hi_isp_defect_pixel_detection_trigger_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x284) & 0x10) >> 4);
}
// ------------------------------------------------------------------------------ //
// Register: Pointer Reset2
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Reset static defect-pixel table pointer each frame - set this when defect-pixel table has been written from mcu
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEFECT_PIXEL_POINTER_RESET2_DEFAULT (0)
#define HI_ISP_DEFECT_PIXEL_POINTER_RESET2_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_defect_pixel_pointer_reset2_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x284);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x284, (((HI_U32) (data & 0x1)) << 8) | (curr & 0xfffffeff));
}
static __inline HI_U8 hi_isp_defect_pixel_pointer_reset2_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x284) & 0x100) >> 8);
}
// ------------------------------------------------------------------------------ //
// Register: Show Reference2
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// For debug purposes.  Show reference values which are compared with actual values to detect bad pixels
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEFECT_PIXEL_SHOW_REFERENCE2_DEFAULT (0)
#define HI_ISP_DEFECT_PIXEL_SHOW_REFERENCE2_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_defect_pixel_show_reference2_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x284);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x284, (((HI_U32) (data & 0x1)) << 9) | (curr & 0xfffffdff));
}
static __inline HI_U8 hi_isp_defect_pixel_show_reference2_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x284) & 0x200) >> 9);
}
// ------------------------------------------------------------------------------ //
// Register: Enable2
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Correction enable: 0=off 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEFECT_PIXEL_ENABLE2_DEFAULT (0)
#define HI_ISP_DEFECT_PIXEL_ENABLE2_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_defect_pixel_enable2_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x284);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x284, (((HI_U32) (data & 0x1)) << 10) | (curr & 0xfffffbff));
}
static __inline HI_U8 hi_isp_defect_pixel_enable2_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x284) & 0x400) >> 10);
}
// ------------------------------------------------------------------------------ //
// Register: Show Static Defect Pixels2
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Show which pixels have been detected as bad
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEFECT_PIXEL_SHOW_STATIC_DEFECT_PIXELS2_DEFAULT (0)
#define HI_ISP_DEFECT_PIXEL_SHOW_STATIC_DEFECT_PIXELS2_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_defect_pixel_show_static_defect_pixels2_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x284);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x284, (((HI_U32) (data & 0x1)) << 11) | (curr & 0xfffff7ff));
}
static __inline HI_U8 hi_isp_defect_pixel_show_static_defect_pixels2_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x284) & 0x800) >> 11);
}
// ------------------------------------------------------------------------------ //
// Register: Detection Trigger2
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Starts detection on 0-1 transition
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEFECT_PIXEL_DETECTION_TRIGGER2_DEFAULT (0)
#define HI_ISP_DEFECT_PIXEL_DETECTION_TRIGGER2_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_defect_pixel_detection_trigger2_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x284);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x284, (((HI_U32) (data & 0x1)) << 12) | (curr & 0xffffefff));
}
static __inline HI_U8 hi_isp_defect_pixel_detection_trigger2_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x284) & 0x1000) >> 12);
}
// ------------------------------------------------------------------------------ //
// Register: Overflow1
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Table overflow flag
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEFECT_PIXEL_OVERFLOW1_DEFAULT (0x0)
#define HI_ISP_DEFECT_PIXEL_OVERFLOW1_DATASIZE (1)

// args: data (1-bit)
static __inline HI_U8 hi_isp_defect_pixel_overflow_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x288) & 0x1) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Overflow2
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Table overflow flag
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEFECT_PIXEL_OVERFLOW2_DEFAULT (0x0)
#define HI_ISP_DEFECT_PIXEL_OVERFLOW2_DATASIZE (1)

// args: data (1-bit)
static __inline HI_U8 hi_isp_defect_pixel_overflow2_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x288) & 0x100) >> 8);
}
// ------------------------------------------------------------------------------ //
// Register: Defect Pixel Count1
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Number of defect-pixels detected
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEFECT_PIXEL_DEFECT_PIXEL_COUNT_DEFAULT (0x0)
#define HI_ISP_DEFECT_PIXEL_DEFECT_PIXEL_COUNT_DATASIZE (12)

// args: data (12-bit)
static __inline HI_U16 hi_isp_defect_pixel_defect_pixel_count_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x28c) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Defect Pixel Count2
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Number of defect-pixels detected
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEFECT_PIXEL_DEFECT_PIXEL_COUNT2_DEFAULT (0x0)
#define HI_ISP_DEFECT_PIXEL_DEFECT_PIXEL_COUNT2_DATASIZE (11)

// args: data (11-bit)
static __inline HI_U16 hi_isp_defect_pixel_defect_pixel_count2_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x28c) & 0x7ff0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: Table Start1
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Address of first defect-pixel in defect-pixel store
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEFECT_PIXEL_TABLE_START1_DEFAULT (0x0)
#define HI_ISP_DEFECT_PIXEL_TABLE_START1_DATASIZE (12)

// args: data (12-bit)
static __inline HI_U16 hi_isp_defect_pixel_table_start_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x290) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Table Start2
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Address of first defect-pixel in defect-pixel store
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEFECT_PIXEL_TABLE_START2_DEFAULT (0x0)
#define HI_ISP_DEFECT_PIXEL_TABLE_START2_DATASIZE (11)

// args: data (11-bit)
static __inline HI_U16 hi_isp_defect_pixel_table_start2_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x290) & 0x7ff0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: Defect Pixel Count In1
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Number of defect-pixels in the written table
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEFECT_PIXEL_DEFECT_PIXEL_COUNT_IN_DEFAULT (0x0)
#define HI_ISP_DEFECT_PIXEL_DEFECT_PIXEL_COUNT_IN_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_defect_pixel_defect_pixel_count_in_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x294);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x294, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_defect_pixel_defect_pixel_count_in_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x294) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Defect Pixel Count In2
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Number of defect-pixels in the written table
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEFECT_PIXEL_DEFECT_PIXEL_COUNT_IN2_DEFAULT (0x0)
#define HI_ISP_DEFECT_PIXEL_DEFECT_PIXEL_COUNT_IN2_DATASIZE (11)

// args: data (11-bit)
static __inline HI_VOID hi_isp_defect_pixel_defect_pixel_count_in2_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x294);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x294, (((HI_U32) (data & 0x7ff)) << 16) | (curr & 0xf800ffff));
}
static __inline HI_U16 hi_isp_defect_pixel_defect_pixel_count_in2_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x294) & 0x7ff0000) >> 16);
}


#define APICAL_DEFECT_PIXEL_MEM_BASE_ADDR (0x10000)
#define APICAL_DEFECT_PIXEL_MEM_SIZE (0x4000)
#define APICAL_DEFECT_PIXEL_MEM_ARRAY_DATA_DEFAULT (0x0)
#define APICAL_DEFECT_PIXEL_MEM_ARRAY_DATA_DATASIZE (32)

static __inline HI_VOID hi_isp_defect_pixel_table_lut_write(ISP_DEV IspDev, HI_U16 index, HI_U32 data) {
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x10000 + (index << 2), data);
}
static __inline HI_U32 hi_isp_defect_pixel_table_lut_read(ISP_DEV IspDev, HI_U16 index, int delay) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x10000 + (index << 2));
}

// ------------------------------------------------------------------------------ //
// Group: WDR stitching
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 4-exposure wide-dynamic-range blending
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: Long First
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 
//              Indicates exposure of first pixel in image: 0=short, 1=long.
//              This applies to even fields (field input is zero).
//            
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_LONG_FIRST_DEFAULT (1)
#define HI_ISP_FRAME_STITCH_LONG_FIRST_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_wdr_long_first_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a0, (((HI_U32) (data & 0x1)) << 0) | (curr & 0xfffffffe));
}
static __inline HI_U8 hi_isp_wdr_long_first_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a0) & 0x1) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Long Use Stitching Error
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_LONG_USE_STITCHING_ERROR_DEFAULT (0)
#define HI_ISP_FRAME_STITCH_LONG_USE_STITCHING_ERROR_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_frame_stitch_long_use_stitching_error_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a0, (((HI_U32) (data & 0x1)) << 1) | (curr & 0xfffffffd));
}
static __inline HI_U8 hi_isp_frame_stitch_long_use_stitching_error_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a0) & 0x2) >> 1);
}
// ------------------------------------------------------------------------------ //
// Register: Use long for np
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 
//          Indicates whether to use average Long or average short to calculate the noise
//          0: use average long to get the noise
//          1: use average short to get the noise
//        
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_USE_LONG_FOR_NP_DEFAULT (0)
#define HI_ISP_FRAME_STITCH_USE_LONG_FOR_NP_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_frame_stitch_use_long_for_np_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a0, (((HI_U32) (data & 0x1)) << 2) | (curr & 0xfffffffb));
}
static __inline HI_U8 hi_isp_frame_stitch_use_long_for_np_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a0) & 0x4) >> 2);
}
// ------------------------------------------------------------------------------ //
// Register: Enable NP
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_ENABLE_NP_DEFAULT (0)
#define HI_ISP_FRAME_STITCH_ENABLE_NP_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_frame_stitch_enable_np_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a0, (((HI_U32) (data & 0x1)) << 4) | (curr & 0xffffffef));
}
static __inline HI_U8 hi_isp_frame_stitch_enable_np_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a0) & 0x10) >> 4);
}
// ------------------------------------------------------------------------------ //
// Register: Long Use Long Override
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_LONG_USE_LONG_OVERRIDE_DEFAULT (0)
#define HI_ISP_FRAME_STITCH_LONG_USE_LONG_OVERRIDE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_wdr_use_long_override_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a0, (((HI_U32) (data & 0x1)) << 5) | (curr & 0xffffffdf));
}
static __inline HI_U8 hi_isp_wdr_use_long_override_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a0) & 0x20) >> 5);
}
// ------------------------------------------------------------------------------ //
// Register: WDR Mode
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Selects WDR mode
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_WDR_MODE_DEFAULT (0)
#define HI_ISP_FRAME_STITCH_WDR_MODE_DATASIZE (1)
#define HI_ISP_FRAME_STITCH_WDR_MODE_WDR_DISABLED (0)
#define HI_ISP_FRAME_STITCH_WDR_MODE_FRAMESWITCHING (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_wdr_wdr_mode_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a0, (((HI_U32) (data & 0x1)) << 6) | (curr & 0xffffffbf));
}
static __inline HI_U8 hi_isp_wdr_wdr_mode_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a0) & 0x40) >> 6);
}
// ------------------------------------------------------------------------------ //
// Register: Alpha Slope From Zero
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 
//          Selects Origin of alpha slope.
//          0: Alpha slope origin is at 1/exposure ratio
//          1: Alpha slope origin is at 0
//        
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_ALPHA_SLOPE_FROM_ZERO_DEFAULT (0)
#define HI_ISP_FRAME_STITCH_ALPHA_SLOPE_FROM_ZERO_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_frame_stitch_alpha_slope_from_zero_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a0, (((HI_U32) (data & 0x1)) << 7) | (curr & 0xffffff7f));
}
static __inline HI_U8 hi_isp_frame_stitch_alpha_slope_from_zero_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a0) & 0x80) >> 7);
}
// ------------------------------------------------------------------------------ //
// Register: Long Use Log
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_LONG_USE_LOG_DEFAULT (0)
#define HI_ISP_FRAME_STITCH_LONG_USE_LOG_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_wdr_use_log_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a0, (((HI_U32) (data & 0x1)) << 13) | (curr & 0xffffdfff));
}
static __inline HI_U8 hi_isp_wdr_use_log_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a0) & 0x2000) >> 13);
}
// ------------------------------------------------------------------------------ //
// Register: Long Use Max2 Intensity
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_LONG_USE_MAX2_INTENSITY_DEFAULT (1)
#define HI_ISP_FRAME_STITCH_LONG_USE_MAX2_INTENSITY_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_wdr_use_max2_intensity_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a0, (((HI_U32) (data & 0x1)) << 14) | (curr & 0xffffbfff));
}
static __inline HI_U8 hi_isp_wdr_use_max2_intensity_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a0) & 0x4000) >> 14);
}
// ------------------------------------------------------------------------------ //
// Register: Long Use Max1 Intensity
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_LONG_USE_MAX1_INTENSITY_DEFAULT (1)
#define HI_ISP_FRAME_STITCH_LONG_USE_MAX1_INTENSITY_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_wdr_use_max1_intensity_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a0, (((HI_U32) (data & 0x1)) << 15) | (curr & 0xffff7fff));
}
static __inline HI_U8 hi_isp_wdr_use_max1_intensity_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a0) & 0x8000) >> 15);
}
// ------------------------------------------------------------------------------ //
// Register: Long Short Thresh
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Data above this threshold will be taken from shorter exposure within the pair only
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_LONG_SHORT_THRESH_DEFAULT (0x0F00)
#define HI_ISP_FRAME_STITCH_LONG_SHORT_THRESH_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_frame_stitch_long_short_thresh_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a0, (((HI_U32) (data & 0xfff)) << 16) | (curr & 0xf000ffff));
}
static __inline HI_U16 hi_isp_frame_stitch_long_short_thresh_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a0) & 0xfff0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: Long Long Thresh
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Data below this threshold will be taken from longer exposure within the pair only
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_LONG_LONG_THRESH_DEFAULT (0x0C00)
#define HI_ISP_FRAME_STITCH_LONG_LONG_THRESH_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_frame_stitch_long_long_thresh_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a4, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_frame_stitch_long_long_thresh_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a4) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Long Exposure Ratio
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Sets ratio between long and its next exposure - this must match the actual exposure ratio on the sensor
//                     For 4 exposures {VS/S/M/L}, this is the exposure between {M/L}
//                     for 3 or 2 exposures, long exposure channel will not be used, so this register value is not valid
//        
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_LONG_EXPOSURE_RATIO_DEFAULT (0x100)
#define HI_ISP_FRAME_STITCH_LONG_EXPOSURE_RATIO_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_frame_stitch_long_exposure_ratio_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a4, (((HI_U32) (data & 0xfff)) << 16) | (curr & 0xf000ffff));
}
static __inline HI_U16 hi_isp_frame_stitch_long_exposure_ratio_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a4) & 0xfff0000) >> 16);
 }
 

// ------------------------------------------------------------------------------ //
// Register: Long Stitch Error Thresh
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Sets level for detection of stitching errors due to motion
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_wdr_lines_thresh
// ------------------------------------------------------------------------------ //
// Controls level at which resolution recovery will activate for detected lines
// ------------------------------------------------------------------------------ //

#define HI_ISP_WDR_LINES_THRESH_DEFAULT (0x0040)
#define HI_ISP_WDR_LINES_THRESH_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_wdr_lines_thresh_write(ISP_DEV IspDev, HI_U16 data) {
    //IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x0156, data);
}
static __inline HI_U16 hi_isp_wdr_lines_thresh_read(ISP_DEV IspDev) {
return 0;
    //return IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x0156);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_wdr_edges_thresh
// ------------------------------------------------------------------------------ //
// Controls level at which resolution recovery will activate for detected edges
// ------------------------------------------------------------------------------ //

#define HI_ISP_WDR_EDGES_THRESH_DEFAULT (0x0040)
#define HI_ISP_WDR_EDGES_THRESH_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_wdr_edges_thresh_write(ISP_DEV IspDev, HI_U16 data) {
    //IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x0158, data);
}
static __inline HI_U16 hi_isp_wdr_edges_thresh_read(ISP_DEV IspDev) {
return 0;
    //return IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x0158);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_wdr_exposure_ratio
// ------------------------------------------------------------------------------ //
// Sets ratio between long and short exposures - this must match the actual exposure ratio on the sensor
// ------------------------------------------------------------------------------ //

#define HI_ISP_WDR_EXPOSURE_RATIO_DEFAULT (0x100)
#define HI_ISP_WDR_EXPOSURE_RATIO_DATASIZE (12)

// args: data (12-bit)
// data format: unsigned 6.6-bit fixed-point
static __inline HI_VOID hi_isp_wdr_exposure_ratio_write(ISP_DEV IspDev, HI_U16 data) {
   // IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x015A, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_wdr_exposure_ratio_read(ISP_DEV IspDev) {
return 0;
    //return (IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x015A) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_wdr_stitch_correct
// ------------------------------------------------------------------------------ //
// Allows adjustment for error in sensor exposure ratio for stitching area
// ------------------------------------------------------------------------------ //

#define HI_ISP_WDR_STITCH_CORRECT_DEFAULT (0x80)
#define HI_ISP_WDR_STITCH_CORRECT_DATASIZE (8)

// args: data (8-bit)
// data format: unsigned 1.7-bit fixed-point
static __inline HI_VOID hi_isp_wdr_stitch_correct_write(ISP_DEV IspDev, HI_U8 data) {
    //IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x0160, data);
}
static __inline HI_U8 hi_isp_wdr_stitch_correct_read(ISP_DEV IspDev) {
return 0;
    //return IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x0160);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_wdr_stitch_error_limit
// ------------------------------------------------------------------------------ //
// Sets intensity level for long exposure below which stitching error detection is disabled
// ------------------------------------------------------------------------------ //

#define HI_ISP_WDR_STITCH_ERROR_LIMIT_DEFAULT (0x0200)
#define HI_ISP_WDR_STITCH_ERROR_LIMIT_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_wdr_stitch_error_limit_write(ISP_DEV IspDev, HI_U16 data) {
    //IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x016E, data);
}
static __inline HI_U16 hi_isp_wdr_stitch_error_limit_read(ISP_DEV IspDev) {
return 0;
    //return IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x016E);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_wdr_black_level_long
// ------------------------------------------------------------------------------ //
// Black level for long exposure input
// ------------------------------------------------------------------------------ //

#define HI_ISP_WDR_BLACK_LEVEL_LONG_DEFAULT (0x000)
#define HI_ISP_WDR_BLACK_LEVEL_LONG_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_wdr_black_level_long_write(ISP_DEV IspDev, HI_U16 data) {
    //IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x0170, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_wdr_black_level_long_read(ISP_DEV IspDev) {
return 0;
    //return (IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x0170) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_wdr_black_level_short
// ------------------------------------------------------------------------------ //
// Black level for short exposure input
// ------------------------------------------------------------------------------ //

#define HI_ISP_WDR_BLACK_LEVEL_SHORT_DEFAULT (0x00)
#define HI_ISP_WDR_BLACK_LEVEL_SHORT_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_wdr_black_level_short_write(ISP_DEV IspDev, HI_U16 data) {
    //IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x0174, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_wdr_black_level_short_read(ISP_DEV IspDev) {
return 0;
    //return (IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x0174) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_wdr_black_level_out
// ------------------------------------------------------------------------------ //
// Black level for module output
// ------------------------------------------------------------------------------ //

#define HI_ISP_WDR_BLACK_LEVEL_OUT_DEFAULT (0x000)
#define HI_ISP_WDR_BLACK_LEVEL_OUT_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_wdr_black_level_out_write(ISP_DEV IspDev, HI_U16 data) {
    //IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x0178, data);
}
static __inline HI_U16 hi_isp_wdr_black_level_out_read(ISP_DEV IspDev) {
return 0;
    //return IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x0178);
}

 // ------------------------------------------------------------------------------ //
// Register: hi_isp_wdr_stitch_error_thresh
// ------------------------------------------------------------------------------ //
// Sets level for detection of stitching errors due to motion
// ------------------------------------------------------------------------------ //

#define HI_ISP_WDR_STITCH_ERROR_THRESH_DEFAULT (0x0040)
#define HI_ISP_WDR_STITCH_ERROR_THRESH_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_wdr_stitch_error_thresh_write(ISP_DEV IspDev, HI_U16 data) {
    //IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x015C, data);
}
static __inline HI_U16 hi_isp_wdr_stitch_error_thresh_read(ISP_DEV IspDev) {
return 0;
    //return IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x015C);
}

#define HI_ISP_FRAME_STITCH_LONG_STITCH_ERROR_THRESH_DEFAULT (0x40)
#define HI_ISP_FRAME_STITCH_LONG_STITCH_ERROR_THRESH_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_frame_stitch_long_stitch_error_thresh_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a8);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a8, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_frame_stitch_long_stitch_error_thresh_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a8) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Long Stitch Correct
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Allows adjustment for error in sensor exposure ratio for stitching area
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_LONG_STITCH_CORRECT_DEFAULT (0x80)
#define HI_ISP_FRAME_STITCH_LONG_STITCH_CORRECT_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_frame_stitch_long_stitch_correct_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a8);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a8, (((HI_U32) (data & 0xff)) << 16) | (curr & 0xff00ffff));
}
static __inline HI_U8 hi_isp_frame_stitch_long_stitch_correct_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2a8) & 0xff0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: Long Stitch Error Limit
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Sets intensity level for long exposure below which stitching error detection is disabled
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_LONG_STITCH_ERROR_LIMIT_DEFAULT (0x2000)
#define HI_ISP_FRAME_STITCH_LONG_STITCH_ERROR_LIMIT_DATASIZE (20)

// args: data (20-bit)
static __inline HI_VOID hi_isp_frame_stitch_long_stitch_error_limit_write(ISP_DEV IspDev, HI_U32 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2ac);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2ac, (((HI_U32) (data & 0xfffff)) << 0) | (curr & 0xfff00000));
}
static __inline HI_U32 hi_isp_frame_stitch_long_stitch_error_limit_read(ISP_DEV IspDev) {
    return (HI_U32)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2ac) & 0xfffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Med Use Stitching Error
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_MED_USE_STITCHING_ERROR_DEFAULT (0)
#define HI_ISP_FRAME_STITCH_MED_USE_STITCHING_ERROR_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_frame_stitch_med_use_stitching_error_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2b0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2b0, (((HI_U32) (data & 0x1)) << 1) | (curr & 0xfffffffd));
}
static __inline HI_U8 hi_isp_frame_stitch_med_use_stitching_error_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2b0) & 0x2) >> 1);
}
// ------------------------------------------------------------------------------ //
// Register: Med Use Long Override
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_MED_USE_LONG_OVERRIDE_DEFAULT (0)
#define HI_ISP_FRAME_STITCH_MED_USE_LONG_OVERRIDE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_frame_stitch_med_use_long_override_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2b0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2b0, (((HI_U32) (data & 0x1)) << 5) | (curr & 0xffffffdf));
}
static __inline HI_U8 hi_isp_frame_stitch_med_use_long_override_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2b0) & 0x20) >> 5);
}
// ------------------------------------------------------------------------------ //
// Register: Med Use Log
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_MED_USE_LOG_DEFAULT (0)
#define HI_ISP_FRAME_STITCH_MED_USE_LOG_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_frame_stitch_med_use_log_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2b0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2b0, (((HI_U32) (data & 0x1)) << 13) | (curr & 0xffffdfff));
}
static __inline HI_U8 hi_isp_frame_stitch_med_use_log_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2b0) & 0x2000) >> 13);
}
// ------------------------------------------------------------------------------ //
// Register: Med Use Max2 Intensity
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_MED_USE_MAX2_INTENSITY_DEFAULT (1)
#define HI_ISP_FRAME_STITCH_MED_USE_MAX2_INTENSITY_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_frame_stitch_med_use_max2_intensity_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2b0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2b0, (((HI_U32) (data & 0x1)) << 14) | (curr & 0xffffbfff));
}
static __inline HI_U8 hi_isp_frame_stitch_med_use_max2_intensity_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2b0) & 0x4000) >> 14);
}
// ------------------------------------------------------------------------------ //
// Register: Med Use Max1 Intensity
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_MED_USE_MAX1_INTENSITY_DEFAULT (1)
#define HI_ISP_FRAME_STITCH_MED_USE_MAX1_INTENSITY_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_frame_stitch_med_use_max1_intensity_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2b0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2b0, (((HI_U32) (data & 0x1)) << 15) | (curr & 0xffff7fff));
}
static __inline HI_U8 hi_isp_frame_stitch_med_use_max1_intensity_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2b0) & 0x8000) >> 15);
}
// ------------------------------------------------------------------------------ //
// Register: Med Short Thresh
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Data above this threshold will be taken from shorter exposure within the pair only
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_MED_SHORT_THRESH_DEFAULT (0x0F00)
#define HI_ISP_FRAME_STITCH_MED_SHORT_THRESH_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_frame_stitch_med_short_thresh_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2b0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2b0, (((HI_U32) (data & 0xfff)) << 16) | (curr & 0xf000ffff));
}
static __inline HI_U16 hi_isp_frame_stitch_med_short_thresh_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2b0) & 0xfff0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: Med Long Thresh
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Data below this threshold will be taken from longer exposure within the pair only
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_MED_LONG_THRESH_DEFAULT (0x0C00)
#define HI_ISP_FRAME_STITCH_MED_LONG_THRESH_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_frame_stitch_med_long_thresh_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2b4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2b4, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_frame_stitch_med_long_thresh_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2b4) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Med Exposure Ratio
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Sets ratio between medium and its next exposure towards short - this must match the actual exposure ratio on the sensor
//                     For 4 exposures {Vs/S/M/L}, this is the exposure between {S/M}
//                     For 3 exposures {S/M/L}, this is the exposure between {M/L}
//                     for 2 exposures, medium exposure channel will not be used, so this register value is not valid
//        
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_MED_EXPOSURE_RATIO_DEFAULT (0x100)
#define HI_ISP_FRAME_STITCH_MED_EXPOSURE_RATIO_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_frame_stitch_med_exposure_ratio_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2b4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2b4, (((HI_U32) (data & 0xfff)) << 16) | (curr & 0xf000ffff));
}
static __inline HI_U16 hi_isp_frame_stitch_med_exposure_ratio_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2b4) & 0xfff0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: Med Stitch Error Thresh
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Sets level for detection of stitching errors due to motion
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_MED_STITCH_ERROR_THRESH_DEFAULT (0x40)
#define HI_ISP_FRAME_STITCH_MED_STITCH_ERROR_THRESH_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_frame_stitch_med_stitch_error_thresh_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2b8);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2b8, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_frame_stitch_med_stitch_error_thresh_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2b8) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Med Stitch Correct
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Allows adjustment for error in sensor exposure ratio for stitching area
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_MED_STITCH_CORRECT_DEFAULT (0x80)
#define HI_ISP_FRAME_STITCH_MED_STITCH_CORRECT_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_frame_stitch_med_stitch_correct_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2b8);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2b8, (((HI_U32) (data & 0xff)) << 16) | (curr & 0xff00ffff));
}
static __inline HI_U8 hi_isp_frame_stitch_med_stitch_correct_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2b8) & 0xff0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: Med Stitch Error Limit
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Sets intensity level for long exposure below which stitching error detection is disabled
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_MED_STITCH_ERROR_LIMIT_DEFAULT (0x2000)
#define HI_ISP_FRAME_STITCH_MED_STITCH_ERROR_LIMIT_DATASIZE (20)

// args: data (20-bit)
static __inline HI_VOID hi_isp_frame_stitch_med_stitch_error_limit_write(ISP_DEV IspDev, HI_U32 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2bc);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2bc, (((HI_U32) (data & 0xfffff)) << 0) | (curr & 0xfff00000));
}
static __inline HI_U32 hi_isp_frame_stitch_med_stitch_error_limit_read(ISP_DEV IspDev) {
    return (HI_U32)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2bc) & 0xfffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Short Use Stitching Error
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_SHORT_USE_STITCHING_ERROR_DEFAULT (0)
#define HI_ISP_FRAME_STITCH_SHORT_USE_STITCHING_ERROR_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_frame_stitch_short_use_stitching_error_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c0, (((HI_U32) (data & 0x1)) << 1) | (curr & 0xfffffffd));
}
static __inline HI_U8 hi_isp_frame_stitch_short_use_stitching_error_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c0) & 0x2) >> 1);
}
// ------------------------------------------------------------------------------ //
// Register: Short Use Long Override
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_SHORT_USE_LONG_OVERRIDE_DEFAULT (0)
#define HI_ISP_FRAME_STITCH_SHORT_USE_LONG_OVERRIDE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_frame_stitch_short_use_long_override_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c0, (((HI_U32) (data & 0x1)) << 5) | (curr & 0xffffffdf));
}
static __inline HI_U8 hi_isp_frame_stitch_short_use_long_override_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c0) & 0x20) >> 5);
}
// ------------------------------------------------------------------------------ //
// Register: Short Use Log
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_SHORT_USE_LOG_DEFAULT (0)
#define HI_ISP_FRAME_STITCH_SHORT_USE_LOG_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_frame_stitch_short_use_log_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c0, (((HI_U32) (data & 0x1)) << 13) | (curr & 0xffffdfff));
}
static __inline HI_U8 hi_isp_frame_stitch_short_use_log_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c0) & 0x2000) >> 13);
}
// ------------------------------------------------------------------------------ //
// Register: Short Use Max2 Intensity
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_SHORT_USE_MAX2_INTENSITY_DEFAULT (1)
#define HI_ISP_FRAME_STITCH_SHORT_USE_MAX2_INTENSITY_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_frame_stitch_short_use_max2_intensity_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c0, (((HI_U32) (data & 0x1)) << 14) | (curr & 0xffffbfff));
}
static __inline HI_U8 hi_isp_frame_stitch_short_use_max2_intensity_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c0) & 0x4000) >> 14);
}
// ------------------------------------------------------------------------------ //
// Register: Short Use Max1 Intensity
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_SHORT_USE_MAX1_INTENSITY_DEFAULT (1)
#define HI_ISP_FRAME_STITCH_SHORT_USE_MAX1_INTENSITY_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_frame_stitch_short_use_max1_intensity_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c0, (((HI_U32) (data & 0x1)) << 15) | (curr & 0xffff7fff));
}
static __inline HI_U8 hi_isp_frame_stitch_short_use_max1_intensity_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c0) & 0x8000) >> 15);
}
// ------------------------------------------------------------------------------ //
// Register: Short Short Thresh
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Data above this threshold will be taken from shorter exposure within the pair only
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_SHORT_SHORT_THRESH_DEFAULT (0x0F00)
#define HI_ISP_FRAME_STITCH_SHORT_SHORT_THRESH_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_frame_stitch_short_short_thresh_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c0, (((HI_U32) (data & 0xfff)) << 16) | (curr & 0xf000ffff));
}
static __inline HI_U16 hi_isp_frame_stitch_short_short_thresh_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c0) & 0xfff0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: Short Long Thresh
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Data below this threshold will be taken from longer exposure within the pair only
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_SHORT_LONG_THRESH_DEFAULT (0x0C00)
#define HI_ISP_FRAME_STITCH_SHORT_LONG_THRESH_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_frame_stitch_short_long_thresh_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c4, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_frame_stitch_short_long_thresh_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c4) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Short Exposure Ratio
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Sets ratio between short and its next exposures towards very short - this must match the actual exposure ratio on the sensor
//                     For 4 exposures {Vs/S/M/L}, this is the exposure between {VS/S}
//                     For 3 exposures {S/M/L}, this is the exposure between {S/M}
//                     For 2 exposures {S/L}, this is the exposure between {S/L}
//        
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_SHORT_EXPOSURE_RATIO_DEFAULT (0x100)
#define HI_ISP_FRAME_STITCH_SHORT_EXPOSURE_RATIO_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_frame_stitch_short_exposure_ratio_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c4, (((HI_U32) (data & 0xfff)) << 16) | (curr & 0xf000ffff));
}
static __inline HI_U16 hi_isp_frame_stitch_short_exposure_ratio_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c4) & 0xfff0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: Short Stitch Error Thresh
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Sets level for detection of stitching errors due to motion
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_SHORT_STITCH_ERROR_THRESH_DEFAULT (0x40)
#define HI_ISP_FRAME_STITCH_SHORT_STITCH_ERROR_THRESH_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_frame_stitch_short_stitch_error_thresh_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c8);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c8, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_frame_stitch_short_stitch_error_thresh_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c8) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Short Stitch Correct
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Allows adjustment for error in sensor exposure ratio for stitching area
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_SHORT_STITCH_CORRECT_DEFAULT (0x80)
#define HI_ISP_FRAME_STITCH_SHORT_STITCH_CORRECT_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_frame_stitch_short_stitch_correct_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c8);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c8, (((HI_U32) (data & 0xff)) << 16) | (curr & 0xff00ffff));
}
static __inline HI_U8 hi_isp_frame_stitch_short_stitch_correct_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2c8) & 0xff0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: Short Stitch Error Limit
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Sets intensity level for long exposure below which stitching error detection is disabled
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_SHORT_STITCH_ERROR_LIMIT_DEFAULT (0x2000)
#define HI_ISP_FRAME_STITCH_SHORT_STITCH_ERROR_LIMIT_DATASIZE (20)

// args: data (20-bit)
static __inline HI_VOID hi_isp_frame_stitch_short_stitch_error_limit_write(ISP_DEV IspDev, HI_U32 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2cc);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2cc, (((HI_U32) (data & 0xfffff)) << 0) | (curr & 0xfff00000));
}
static __inline HI_U32 hi_isp_frame_stitch_short_stitch_error_limit_read(ISP_DEV IspDev) {
    return (HI_U32)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2cc) & 0xfffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Black level Long
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Black level for long exposure input
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_BLACK_LEVEL_LONG_DEFAULT (0x000)
#define HI_ISP_FRAME_STITCH_BLACK_LEVEL_LONG_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_frame_stitch_black_level_long_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2d0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2d0, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_frame_stitch_black_level_long_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2d0) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Black level Medium1
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Black level for short exposure input
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_BLACK_LEVEL_MEDIUM1_DEFAULT (0x00)
#define HI_ISP_FRAME_STITCH_BLACK_LEVEL_MEDIUM1_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_frame_stitch_black_level_medium_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2d4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2d4, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_frame_stitch_black_level_medium_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2d4) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Black level Medium2
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Black level for short exposure input
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_BLACK_LEVEL_MEDIUM2_DEFAULT (0x00)
#define HI_ISP_FRAME_STITCH_BLACK_LEVEL_MEDIUM2_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_frame_stitch_black_level_short_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2d8);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2d8, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_frame_stitch_black_level_short_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2d8) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Black level Short
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Black level for short exposure input
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_BLACK_LEVEL_SHORT_DEFAULT (0x00)
#define HI_ISP_FRAME_STITCH_BLACK_LEVEL_SHORT_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_frame_stitch_black_level_very_short_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2dc);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2dc, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_frame_stitch_black_level_very_short_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2dc) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Black level Out
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Black level for module output
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_BLACK_LEVEL_OUT_DEFAULT (0x000)
#define HI_ISP_FRAME_STITCH_BLACK_LEVEL_OUT_DATASIZE (20)

// args: data (20-bit)
static __inline HI_VOID hi_isp_frame_stitch_black_level_out_write(ISP_DEV IspDev, HI_U32 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2e0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2e0, (((HI_U32) (data & 0xfffff)) << 0) | (curr & 0xfff00000));
}
static __inline HI_U32 hi_isp_frame_stitch_black_level_out_read(ISP_DEV IspDev) {
    return (HI_U32)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2e0) & 0xfffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Mode In
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 0: 4 ch, 1: 2 ch, 2: 3ch, 3: 4ch
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STITCH_MODE_IN_DEFAULT (0x0)
#define HI_ISP_FRAME_STITCH_MODE_IN_DATASIZE (2)

// args: data (2-bit)
static __inline HI_VOID hi_isp_frame_stitch_mode_in_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2e4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2e4, (((HI_U32) (data & 0x3)) << 0) | (curr & 0xfffffffc));
}
static __inline HI_U8 hi_isp_frame_stitch_mode_in_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2e4) & 0x3) >> 0);
}
// ------------------------------------------------------------------------------ //
// Group: Noise Profile WDR 4
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Noise profile controls for long exposure
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: Long noise offset
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_wdr_exposure_ratio
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_WDR_4_LONG_NOISE_OFFSET_DEFAULT (0x40)
#define HI_ISP_NOISE_PROFILE_WDR_4_LONG_NOISE_OFFSET_DATASIZE (9)

// args: data (9-bit)
static __inline HI_VOID hi_isp_noise_profile_wdr_4_long_noise_offset_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1004);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1004, (((HI_U32) (data & 0x1ff)) << 0) | (curr & 0xfffffe00));
}
static __inline HI_U16 hi_isp_noise_profile_wdr_4_long_noise_offset_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1004) & 0x1ff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Group: Noise Profile WDR 3
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Noise profile controls for med1 exposure
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: Med1 noise offset
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Noise offset
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_WDR_3_MED1_NOISE_OFFSET_DEFAULT (0x40)
#define HI_ISP_NOISE_PROFILE_WDR_3_MED1_NOISE_OFFSET_DATASIZE (9)

// args: data (9-bit)
static __inline HI_VOID hi_isp_noise_profile_wdr_3_med1_noise_offset_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1014);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1014, (((HI_U32) (data & 0x1ff)) << 0) | (curr & 0xfffffe00));
}
static __inline HI_U16 hi_isp_noise_profile_wdr_3_med1_noise_offset_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1014) & 0x1ff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Group: Noise Profile WDR 2
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Noise profile controls for med2 exposure
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: Med2 noise offset
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Noise offset
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_WDR_2_MED2_NOISE_OFFSET_DEFAULT (0x40)
#define HI_ISP_NOISE_PROFILE_WDR_2_MED2_NOISE_OFFSET_DATASIZE (9)

// args: data (9-bit)
static __inline HI_VOID hi_isp_noise_profile_wdr_2_med2_noise_offset_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1024);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1024, (((HI_U32) (data & 0x1ff)) << 0) | (curr & 0xfffffe00));
}
static __inline HI_U16 hi_isp_noise_profile_wdr_2_med2_noise_offset_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1024) & 0x1ff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Group: Noise Profile WDR 1
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Noise profile controls for short exposure
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: Short noise offset
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Noise offset
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_WDR_1_SHORT_NOISE_OFFSET_DEFAULT (0x40)
#define HI_ISP_NOISE_PROFILE_WDR_1_SHORT_NOISE_OFFSET_DATASIZE (9)

// args: data (9-bit)
static __inline HI_VOID hi_isp_noise_profile_wdr_1_short_noise_offset_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1034);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1034, (((HI_U32) (data & 0x1ff)) << 0) | (curr & 0xfffffe00));
}
static __inline HI_U16 hi_isp_noise_profile_wdr_1_short_noise_offset_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1034) & 0x1ff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Group: Frontend LUT
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Frontend lookup (for companded WDR sensor inputs)
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: Enable 0
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Frontend lookup0 enable: 0=off 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_GAMMA_FE_ENABLE_0_DEFAULT (0)
#define HI_ISP_GAMMA_FE_ENABLE_0_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_gammafe_enable_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x300);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x300, (((HI_U32) (data & 0x1)) << 0) | (curr & 0xfffffffe));
}
static __inline HI_U8 hi_isp_gammafe_enable_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x300) & 0x1) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Enable 1
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Frontend lookup1 enable: 0=off 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_GAMMA_FE_ENABLE_1_DEFAULT (0)
#define HI_ISP_GAMMA_FE_ENABLE_1_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_gammafe_enable1_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x300);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x300, (((HI_U32) (data & 0x1)) << 1) | (curr & 0xfffffffd));
}
static __inline HI_U8 hi_isp_gammafe_enable1_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x300) & 0x2) >> 1);
}
// ------------------------------------------------------------------------------ //
// Register: Enable DL
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Frontend lookup1 enable: 0=off 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_GAMMA_FE_ENABLE_DL_DEFAULT (0)
#define HI_ISP_GAMMA_FE_ENABLE_DL_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_gamma_fe_enable_dl_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x300);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x300, (((HI_U32) (data & 0x1)) << 2) | (curr & 0xfffffffb));
}
static __inline HI_U8 hi_isp_gamma_fe_enable_dl_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x300) & 0x4) >> 2);
}
// ------------------------------------------------------------------------------ //
// Register: Offset Mode 0
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 
//          Lookup0 reflection mode for black offset region
//          0 = Manual curve reflection
//          1 = Automatic curve reflection
//        
// ------------------------------------------------------------------------------ //

#define HI_ISP_GAMMA_FE_OFFSET_MODE_0_DEFAULT (0)
#define HI_ISP_GAMMA_FE_OFFSET_MODE_0_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_gamma_fe_offset_mode_0_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x300);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x300, (((HI_U32) (data & 0x1)) << 8) | (curr & 0xfffffeff));
}
static __inline HI_U8 hi_isp_gamma_fe_offset_mode_0_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x300) & 0x100) >> 8);
}
// ------------------------------------------------------------------------------ //
// Register: Offset Mode 1
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 
//          Lookup1 reflection mode for black offset region
//          0 = Manual curve reflection
//          1 = Automatic curve reflection
//        
// ------------------------------------------------------------------------------ //

#define HI_ISP_GAMMA_FE_OFFSET_MODE_1_DEFAULT (0)
#define HI_ISP_GAMMA_FE_OFFSET_MODE_1_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_gamma_fe_offset_mode_1_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x300);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x300, (((HI_U32) (data & 0x1)) << 9) | (curr & 0xfffffdff));
}
static __inline HI_U8 hi_isp_gamma_fe_offset_mode_1_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x300) & 0x200) >> 9);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_gammafe_mcu_priority
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 
//        Priority of CPU writes to LUTs
//        0=low. CPU read/writes from/to the frontend LUTs are only executed when LUTs are not being accessed by ISP.  Normal operation.
//        1=high. CPU read/writes from/to the frontend LUTs are always executed.  This may result in corrupt image data and invalid read back and is not recommended.
//        
// ------------------------------------------------------------------------------ //

#define HI_ISP_GAMMA_FE_MCU_PRIORITY_DEFAULT (0)
#define HI_ISP_GAMMA_FE_MCU_PRIORITY_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_gammafe_mcu_priority_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x300);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x300, (((HI_U32) (data & 0x1)) << 31) | (curr & 0x7fffffff));
}
static __inline HI_U8 hi_isp_gammafe_mcu_priority_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x300) & 0x80000000) >> 31);
}
// ------------------------------------------------------------------------------ //
// Register: MCU ready 0
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// LUT0 status indicator.  When 1, LUT0 is ready to receive the data from CPU
// ------------------------------------------------------------------------------ //

#define HI_ISP_GAMMA_FE_MCU_READY_0_DEFAULT (0x0)
#define HI_ISP_GAMMA_FE_MCU_READY_0_DATASIZE (1)

// args: data (1-bit)
static __inline HI_U8 hi_isp_gamma_fe_mcu_ready_0_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x304) & 0x1) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: MCU ready 1
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// LUT1 status indicator.  When 1, LUT1 is ready to receive the data from CPU
// ------------------------------------------------------------------------------ //

#define HI_ISP_GAMMA_FE_MCU_READY_1_DEFAULT (0x0)
#define HI_ISP_GAMMA_FE_MCU_READY_1_DATASIZE (1)

// args: data (1-bit)
static __inline HI_U8 hi_isp_gamma_fe_mcu_ready_1_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x304) & 0x2) >> 1);
}
// ------------------------------------------------------------------------------ //
// Register: Bank select 0
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// LUT0 active bank
// ------------------------------------------------------------------------------ //

#define HI_ISP_GAMMA_FE_BANK_SELECT_0_DEFAULT (0)
#define HI_ISP_GAMMA_FE_BANK_SELECT_0_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_gamma_fe_bank_select_0_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x300);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x300, (((HI_U32) (data & 0x1)) << 12) | (curr & 0xffffefff));
}
static __inline HI_U8 hi_isp_gamma_fe_bank_select_0_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x300) & 0x1000) >> 12);
}
// ------------------------------------------------------------------------------ //
// Register: Bank select 1
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// LUT1 active bank
// ------------------------------------------------------------------------------ //

#define HI_ISP_GAMMA_FE_BANK_SELECT_1_DEFAULT (0)
#define HI_ISP_GAMMA_FE_BANK_SELECT_1_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_gamma_fe_bank_select_1_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x300);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x300, (((HI_U32) (data & 0x1)) << 13) | (curr & 0xffffdfff));
}
static __inline HI_U8 hi_isp_gamma_fe_bank_select_1_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x300) & 0x2000) >> 13);
}
// ------------------------------------------------------------------------------ //
// Register: Alpha
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Front end alpha 6.12 fix point, used for gamma_fe_dl
// ------------------------------------------------------------------------------ //

#define HI_ISP_GAMMA_FE_ALPHA_DEFAULT (0x01000)
#define HI_ISP_GAMMA_FE_ALPHA_DATASIZE (18)

// args: data (18-bit)
static __inline HI_VOID hi_isp_gamma_fe_alpha_write(ISP_DEV IspDev, HI_U32 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x308);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x308, (((HI_U32) (data & 0x3ffff)) << 0) | (curr & 0xfffc0000));
}
static __inline HI_U32 hi_isp_gamma_fe_alpha_read(ISP_DEV IspDev) {
    return (HI_U32)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x308) & 0x3ffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Group: Digital Gain
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_digital_gain_gain
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: Gain
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Gain applied to data in 5.8 format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DIGITAL_GAIN_GAIN_DEFAULT (0x100)
#define HI_ISP_DIGITAL_GAIN_GAIN_DATASIZE (13)

// args: data (13-bit)
static __inline HI_VOID hi_isp_digital_gain_gain_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x310);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x310, (((HI_U32) (data & 0x1fff)) << 0) | (curr & 0xffffe000));
}
static __inline HI_U16 hi_isp_digital_gain_gain_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x310) & 0x1fff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Offset
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Data black level
// ------------------------------------------------------------------------------ //

#define HI_ISP_DIGITAL_GAIN_OFFSET_DEFAULT (0x000)
#define HI_ISP_DIGITAL_GAIN_OFFSET_DATASIZE (20)

// args: data (20-bit)
static __inline HI_VOID hi_isp_digital_gain_offset_write(ISP_DEV IspDev, HI_U32 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x314);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x314, (((HI_U32) (data & 0xfffff)) << 0) | (curr & 0xfff00000));
}
static __inline HI_U32 hi_isp_digital_gain_offset_read(ISP_DEV IspDev) {
    return (HI_U32)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x314) & 0xfffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Group: Sinter
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Spatial noise reduction
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: Config
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_CONFIG_DEFAULT (0x0)
#define HI_ISP_NR_CONFIG_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_nr_config_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x320);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x320, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_nr_config_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x320) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Enable
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Sinter enable: 0=off 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_ENABLE_DEFAULT (1)
#define HI_ISP_NR_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_nr_enable_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x320);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x320, (((HI_U32) (data & 0x1)) << 4) | (curr & 0xffffffef));
}
static __inline HI_U8 hi_isp_nr_enable_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x320) & 0x10) >> 4);
}
// ------------------------------------------------------------------------------ //
// Register: View Filter
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// For debug purposes only. Set to zero for normal operation
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_VIEW_FILTER_DEFAULT (0)
#define HI_ISP_NR_VIEW_FILTER_DATASIZE (2)

// args: data (2-bit)
static __inline HI_VOID hi_isp_nr_view_filter_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x320);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x320, (((HI_U32) (data & 0x3)) << 0) | (curr & 0xfffffffc));
}
static __inline HI_U8 hi_isp_nr_view_filter_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x320) & 0x3) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_scale_mode
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// For debug purposes only. Set to 3 for normal operation
//   0 = Use filter 0 only
//   1 = Use filters 0 and 2 only
//   2 = Use filters 0, 2 and 4 only
//   3 = Use all filters
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_SCALE_MODE_DEFAULT (3)
#define HI_ISP_NR_SCALE_MODE_DATASIZE (2)
#define HI_ISP_NR_SCALE_MODE_USE_FILTER_0_ONLY (0)
#define HI_ISP_NR_SCALE_MODE_USE_FILTERS_0_AND_2_ONLY (1)
#define HI_ISP_NR_SCALE_MODE_USE_FILTERS_0_2_AND_4_ONLY (2)
#define HI_ISP_NR_SCALE_MODE_USE_ALL_FILTERS (3)

// args: data (2-bit)
static __inline HI_VOID hi_isp_nr_scale_mode_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x320);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x320, (((HI_U32) (data & 0x3)) << 2) | (curr & 0xfffffff3));
}
static __inline HI_U8 hi_isp_nr_scale_mode_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x320) & 0xc) >> 2);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_filter_select
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Sinter filter fine tuning.  Should not be modified from suggested values.
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_FILTER_SELECT_DEFAULT (0)
#define HI_ISP_NR_FILTER_SELECT_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_nr_filter_select_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x320);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x320, (((HI_U32) (data & 0x1)) << 5) | (curr & 0xffffffdf));
}
static __inline HI_U8 hi_isp_nr_filter_select_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x320) & 0x20) >> 5);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_int_select
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Select intensity filter.  Should not be modified from suggested values.
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_INT_SELECT_DEFAULT (0)
#define HI_ISP_NR_INT_SELECT_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_nr_int_select_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x320);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x320, (((HI_U32) (data & 0x1)) << 6) | (curr & 0xffffffbf));
}
static __inline HI_U8 hi_isp_nr_int_select_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x320) & 0x40) >> 6);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_rm_enable
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 
//          Adjusts nr strength radially from center to compensate for Lens shading correction.
//          enable: 0=off, 1=on
//          
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_RM_ENABLE_DEFAULT (0)
#define HI_ISP_NR_RM_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_nr_rm_enable_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x320);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x320, (((HI_U32) (data & 0x1)) << 7) | (curr & 0xffffff7f));
}
static __inline HI_U8 hi_isp_nr_rm_enable_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x320) & 0x80) >> 7);
}
// ------------------------------------------------------------------------------ //
// Register: int config
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Intensity blending with mosaic raw
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_INT_CONFIG_DEFAULT (0x4)
#define HI_ISP_NR_INT_CONFIG_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_isp_nr_int_config_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x320);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x320, (((HI_U32) (data & 0xf)) << 8) | (curr & 0xfffff0ff));
}
static __inline HI_U8 hi_isp_nr_int_config_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x320) & 0xf00) >> 8);
}
// ------------------------------------------------------------------------------ //
// Register: rm_center_x
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Center x coordinate of shading map
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_RM_CENTER_X_DEFAULT (0x280)
#define HI_ISP_NR_RM_CENTER_X_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_nr_rm_center_x_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x324);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x324, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_nr_rm_center_x_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x324) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: rm_center_y
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Center y coordinate of shading map
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_RM_CENTER_Y_DEFAULT (0x168)
#define HI_ISP_NR_RM_CENTER_Y_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_nr_rm_center_y_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x328);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x328, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_nr_rm_center_y_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x328) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_rm_off_center_mult
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 
//        Normalizing factor which scales the radial table to the edge of the image.
//        Calculated as 2^31/R^2 where R is the furthest distance from the center coordinate to the edge of the image in pixels.
//        
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_RM_OFF_CENTER_MULT_DEFAULT (0x0100)
#define HI_ISP_NR_RM_OFF_CENTER_MULT_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_nr_rm_off_center_mult_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x32c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x32c, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_nr_rm_off_center_mult_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x32c) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Thresh 0h
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Unused - no effect
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_THRESH_0H_DEFAULT (0x00)
#define HI_ISP_NR_THRESH_0H_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_nr_thresh_0h_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x330);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x330, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_nr_thresh_0h_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x330) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Thresh 1h
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Noise threshold for high horizontal spatial frequencies
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_THRESH_1H_DEFAULT (0x00)
#define HI_ISP_NR_THRESH_1H_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_nr_thresh_1h_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x330);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x330, (((HI_U32) (data & 0xff)) << 16) | (curr & 0xff00ffff));
}
static __inline HI_U8 hi_isp_nr_thresh_1h_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x330) & 0xff0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_thresh_2h
// ------------------------------------------------------------------------------ //
// Noise threshold for medium horizontal spatial frequencies
// ------------------------------------------------------------------------------ //
// Unused - no effect
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_THRESH_2H_DEFAULT (0x00)
#define HI_ISP_NR_THRESH_2H_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_nr_thresh_2h_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x334);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x334, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_nr_thresh_2h_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x334) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_thresh_4h
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Noise threshold for low horizontal spatial frequencies
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_THRESH_4H_DEFAULT (0x00)
#define HI_ISP_NR_THRESH_4H_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_nr_thresh_4h_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x334);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x334, (((HI_U32) (data & 0xff)) << 16) | (curr & 0xff00ffff));
}
static __inline HI_U8 hi_isp_nr_thresh_4h_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x334) & 0xff0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_thresh_0v
// ------------------------------------------------------------------------------ //
// Noise threshold for high vertical spatial frequencies
// ------------------------------------------------------------------------------ //
// Unused - no effect
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_THRESH_0V_DEFAULT (0x00)
#define HI_ISP_NR_THRESH_0V_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_nr_thresh_0v_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x338);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x338, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_nr_thresh_0v_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x338) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_thresh_1v
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Noise threshold for high vertical spatial frequencies
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_THRESH_1V_DEFAULT (0x00)
#define HI_ISP_NR_THRESH_1V_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_nr_thresh_1v_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x338);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x338, (((HI_U32) (data & 0xff)) << 16) | (curr & 0xff00ffff));
}
static __inline HI_U8 hi_isp_nr_thresh_1v_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x338) & 0xff0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_thresh_2v
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Unused - no effect
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_THRESH_2V_DEFAULT (0x00)
#define HI_ISP_NR_THRESH_2V_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_nr_thresh_2v_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x33c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x33c, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_nr_thresh_2v_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x33c) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Thresh 4v
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Noise threshold for low vertical spatial frequencies
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_THRESH_4V_DEFAULT (0x00)
#define HI_ISP_NR_THRESH_4V_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_nr_thresh_4v_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x33c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x33c, (((HI_U32) (data & 0xff)) << 16) | (curr & 0xff00ffff));
}
static __inline HI_U8 hi_isp_nr_thresh_4v_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x33c) & 0xff0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_thresh_short
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Noise threshold adjustment for short exposure data
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_THRESH_SHORT_DEFAULT (0x00)
#define HI_ISP_NR_THRESH_SHORT_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_nr_thresh_short_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x344);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x344, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_nr_thresh_short_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x344) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_thresh_long
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Noise threshold adjustment for long exposure data
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_THRESH_LONG_DEFAULT (0x30)
#define HI_ISP_NR_THRESH_LONG_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_nr_thresh_long_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x348);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x348, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_nr_thresh_long_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x348) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_strength_0
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Unused - no effect
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_STRENGTH_0_DEFAULT (0xFF)
#define HI_ISP_NR_STRENGTH_0_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_nr_strength_0_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x34c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x34c, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_nr_strength_0_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x34c) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_strength_1
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Noise reduction effect for high spatial frequencies
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_STRENGTH_1_DEFAULT (0xFF)
#define HI_ISP_NR_STRENGTH_1_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_nr_strength_1_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x350);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x350, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_nr_strength_1_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x350) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_strength_2
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Unused - no effect
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_STRENGTH_2_DEFAULT (0xFF)
#define HI_ISP_NR_STRENGTH_2_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_nr_strength_2_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x354);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x354, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_nr_strength_2_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x354) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_strength_4
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Noise reduction effect for low spatial frequencies
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_STRENGTH_4_DEFAULT (0xFF)
#define HI_ISP_NR_STRENGTH_4_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_nr_strength_4_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x358);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x358, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_nr_strength_4_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x358) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Group: Backend LUT
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Backend gamma LUT
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: Enable 0
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Backend lookup0 enable: 0=off 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_GAMMA_BE_ENABLE_0_DEFAULT (0)
#define HI_ISP_GAMMA_BE_ENABLE_0_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_gamma_be_enable_0_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x360);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x360, (((HI_U32) (data & 0x1)) << 0) | (curr & 0xfffffffe));
}
static __inline HI_U8 hi_isp_gamma_be_enable_0_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x360) & 0x1) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Enable 1
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Backend lookup1 enable: 0=off 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_GAMMA_BE_ENABLE_1_DEFAULT (0)
#define HI_ISP_GAMMA_BE_ENABLE_1_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_gamma_be_enable_1_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x360);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x360, (((HI_U32) (data & 0x1)) << 1) | (curr & 0xfffffffd));
}
static __inline HI_U8 hi_isp_gamma_be_enable_1_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x360) & 0x2) >> 1);
}
// ------------------------------------------------------------------------------ //
// Register: Enable DL
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Backend lookup DL enable: 0=off 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_GAMMA_BE_ENABLE_DL_DEFAULT (0)
#define HI_ISP_GAMMA_BE_ENABLE_DL_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_gamma_be_enable_dl_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x360);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x360, (((HI_U32) (data & 0x1)) << 2) | (curr & 0xfffffffb));
}
static __inline HI_U8 hi_isp_gamma_be_enable_dl_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x360) & 0x4) >> 2);
}
// ------------------------------------------------------------------------------ //
// Register: Offset Mode 0
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 
//          Lookup0 reflection mode for black offset region
//          0 = Manual curve reflection
//          1 = Automatic curve reflection
//        
// ------------------------------------------------------------------------------ //

#define HI_ISP_GAMMA_BE_OFFSET_MODE_0_DEFAULT (0)
#define HI_ISP_GAMMA_BE_OFFSET_MODE_0_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_gamma_be_offset_mode_0_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x360);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x360, (((HI_U32) (data & 0x1)) << 8) | (curr & 0xfffffeff));
}
static __inline HI_U8 hi_isp_gamma_be_offset_mode_0_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x360) & 0x100) >> 8);
}
// ------------------------------------------------------------------------------ //
// Register: Offset Mode 1
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 
//          Lookup1 reflection mode for black offset region
//          0 = Manual curve reflection
//          1 = Automatic curve reflection
//        
// ------------------------------------------------------------------------------ //

#define HI_ISP_GAMMA_BE_OFFSET_MODE_1_DEFAULT (0)
#define HI_ISP_GAMMA_BE_OFFSET_MODE_1_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_gamma_be_offset_mode_1_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x360);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x360, (((HI_U32) (data & 0x1)) << 9) | (curr & 0xfffffdff));
}
static __inline HI_U8 hi_isp_gamma_be_offset_mode_1_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x360) & 0x200) >> 9);
}
// ------------------------------------------------------------------------------ //
// Register: MCU priority
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 
//        Priority of CPU writes to LUTs
//        0=low. CPU read/writes from/to the backend LUTs are only executed when LUTs are not being accessed by ISP.  Normal operation.
//        1=high. CPU read/writes from/to the backend LUTs are always executed.  This may result in corrupt image data and invalid read back and is not recommended.
//        
// ------------------------------------------------------------------------------ //

#define HI_ISP_GAMMA_BE_MCU_PRIORITY_DEFAULT (0)
#define HI_ISP_GAMMA_BE_MCU_PRIORITY_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_gamma_be_mcu_priority_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x360);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x360, (((HI_U32) (data & 0x1)) << 31) | (curr & 0x7fffffff));
}
static __inline HI_U8 hi_isp_gamma_be_mcu_priority_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x360) & 0x80000000) >> 31);
}
// ------------------------------------------------------------------------------ //
// Register: MCU ready 0
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// LUT0 status indicator.  When 1, LUT0 is ready to receive the data from CPU
// ------------------------------------------------------------------------------ //

#define HI_ISP_GAMMA_BE_MCU_READY_0_DEFAULT (0x0)
#define HI_ISP_GAMMA_BE_MCU_READY_0_DATASIZE (1)

// args: data (1-bit)
static __inline HI_U8 hi_isp_gamma_be_mcu_ready_0_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x364) & 0x1) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: MCU ready 1
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// LUT1 status indicator.  When 1, LUT1 is ready to receive the data from CPU
// ------------------------------------------------------------------------------ //

#define HI_ISP_GAMMA_BE_MCU_READY_1_DEFAULT (0x0)
#define HI_ISP_GAMMA_BE_MCU_READY_1_DATASIZE (1)

// args: data (1-bit)
static __inline HI_U8 hi_isp_gamma_be_mcu_ready_1_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x364) & 0x2) >> 1);
}
// ------------------------------------------------------------------------------ //
// Register: Bank select 0
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// LUT0 active bank
// ------------------------------------------------------------------------------ //

#define HI_ISP_GAMMA_BE_BANK_SELECT_0_DEFAULT (0)
#define HI_ISP_GAMMA_BE_BANK_SELECT_0_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_gamma_be_bank_select_0_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x360);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x360, (((HI_U32) (data & 0x1)) << 12) | (curr & 0xffffefff));
}
static __inline HI_U8 hi_isp_gamma_be_bank_select_0_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x360) & 0x1000) >> 12);
}
// ------------------------------------------------------------------------------ //
// Register: Bank select 1
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// LUT1 active bank
// ------------------------------------------------------------------------------ //

#define HI_ISP_GAMMA_BE_BANK_SELECT_1_DEFAULT (0)
#define HI_ISP_GAMMA_BE_BANK_SELECT_1_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_gamma_be_bank_select_1_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x360);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x360, (((HI_U32) (data & 0x1)) << 13) | (curr & 0xffffdfff));
}
static __inline HI_U8 hi_isp_gamma_be_bank_select_1_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x360) & 0x2000) >> 13);
}
// ------------------------------------------------------------------------------ //
// Register: Alpha
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Backend alpha 6.12 fix point
// ------------------------------------------------------------------------------ //

#define HI_ISP_GAMMA_BE_ALPHA_DEFAULT (0x01000)
#define HI_ISP_GAMMA_BE_ALPHA_DATASIZE (18)

// args: data (18-bit)
static __inline HI_VOID hi_isp_gamma_be_alpha_write(ISP_DEV IspDev, HI_U32 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x368);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x368, (((HI_U32) (data & 0x3ffff)) << 0) | (curr & 0xfffc0000));
}
static __inline HI_U32 hi_isp_gamma_be_alpha_read(ISP_DEV IspDev) {
    return (HI_U32)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x368) & 0x3ffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Group: Noise Profile 
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_noise_profile_exp_thresh
// ------------------------------------------------------------------------------ //
// Threshold for determining long/short exposure data
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_EXP_THRESH_DEFAULT (0xFFFF)
#define HI_ISP_NOISE_PROFILE_EXP_THRESH_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_noise_profile_exp_thresh_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x370);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x370, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_noise_profile_exp_thresh_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x370) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_noise_profile_short_ratio
// ------------------------------------------------------------------------------ //
// Multiplier applied to short exposure data for noise profile calculation
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_SHORT_RATIO_DEFAULT (0x20)
#define HI_ISP_NOISE_PROFILE_SHORT_RATIO_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_noise_profile_short_ratio_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x374);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x374, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_noise_profile_short_ratio_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x374) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Long Ratio
// ------------------------------------------------------------------------------ //
// Register: hi_isp_noise_profile_long_ratio
// ------------------------------------------------------------------------------ //
// Multiplier applied to long exposure data for noise profile calculation
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_LONG_RATIO_DEFAULT (0x04)
#define HI_ISP_NOISE_PROFILE_LONG_RATIO_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_noise_profile_long_ratio_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x378);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x378, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_noise_profile_long_ratio_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x378) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: NP off
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_noise_profile_np_off
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_NP_OFF_DEFAULT (0)
#define HI_ISP_NOISE_PROFILE_NP_OFF_DATASIZE (7)

// args: data (7-bit)
static __inline HI_VOID hi_isp_noise_profile_np_off_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x37c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x37c, (((HI_U32) (data & 0x7f)) << 0) | (curr & 0xffffff80));
}
static __inline HI_U8 hi_isp_noise_profile_np_off_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x37c) & 0x7f) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: NP off reflect
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 
//          Defines how values below black level are obtained.
//          0: Repeat the first table entry.
//          1: Reflect the noise profile curve below black level.
//        
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_NP_OFF_REFLECT_DEFAULT (0)
#define HI_ISP_NOISE_PROFILE_NP_OFF_REFLECT_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_noise_profile_np_off_reflect_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x37c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x37c, (((HI_U32) (data & 0x1)) << 7) | (curr & 0xffffff7f));
}
static __inline HI_U8 hi_isp_noise_profile_np_off_reflect_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x37c) & 0x80) >> 7);
}
// ------------------------------------------------------------------------------ //
// Group: White Balance
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Static white balance - independent gain for each color channel
//      
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_white_balance_gain_00
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Multiplier for color channel 00 (R)
// ------------------------------------------------------------------------------ //

#define HI_ISP_WHITE_BALANCE_GAIN_00_DEFAULT (0x100)
#define HI_ISP_WHITE_BALANCE_GAIN_00_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_white_balance_gain_00_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x380);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x380, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_white_balance_gain_00_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x380) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_white_balance_gain_01
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Multiplier for color channel 01 (Gr)
// ------------------------------------------------------------------------------ //

#define HI_ISP_WHITE_BALANCE_GAIN_01_DEFAULT (0x100)
#define HI_ISP_WHITE_BALANCE_GAIN_01_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_white_balance_gain_01_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x384);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x384, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_white_balance_gain_01_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x384) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_white_balance_gain_10
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Multiplier for color channel 10 (Gb)
// ------------------------------------------------------------------------------ //

#define HI_ISP_WHITE_BALANCE_GAIN_10_DEFAULT (0x100)
#define HI_ISP_WHITE_BALANCE_GAIN_10_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_white_balance_gain_10_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x388);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x388, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_white_balance_gain_10_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x388) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_white_balance_gain_11
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Multiplier for color channel 11 (B)
// ------------------------------------------------------------------------------ //

#define HI_ISP_WHITE_BALANCE_GAIN_11_DEFAULT (0x100)
#define HI_ISP_WHITE_BALANCE_GAIN_11_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_white_balance_gain_11_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x38c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x38c, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_white_balance_gain_11_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x38c) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Group: Offset
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Black offset subtraction for each color channel
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: Black 00
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Black offset for color channel 00 (R)
// ------------------------------------------------------------------------------ //

#define HI_ISP_OFFSET_BLACK_00_DEFAULT (0x0000)
#define HI_ISP_OFFSET_BLACK_00_DATASIZE (20)

// args: data (20-bit)
static __inline HI_VOID hi_isp_offset_black_00_write(ISP_DEV IspDev, HI_U32 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3a0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3a0, (((HI_U32) (data & 0xfffff)) << 0) | (curr & 0xfff00000));
}
static __inline HI_U32 hi_isp_offset_black_00_read(ISP_DEV IspDev) {
    return (HI_U32)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3a0) & 0xfffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Black 01
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Black offset for color channel 01 (Gr)
// ------------------------------------------------------------------------------ //

#define HI_ISP_OFFSET_BLACK_01_DEFAULT (0x0000)
#define HI_ISP_OFFSET_BLACK_01_DATASIZE (20)

// args: data (20-bit)
static __inline HI_VOID hi_isp_offset_black_01_write(ISP_DEV IspDev, HI_U32 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3a4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3a4, (((HI_U32) (data & 0xfffff)) << 0) | (curr & 0xfff00000));
}
static __inline HI_U32 hi_isp_offset_black_01_read(ISP_DEV IspDev) {
    return (HI_U32)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3a4) & 0xfffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Black 10
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Black offset for color channel 10 (Gb)
// ------------------------------------------------------------------------------ //

#define HI_ISP_OFFSET_BLACK_10_DEFAULT (0x0000)
#define HI_ISP_OFFSET_BLACK_10_DATASIZE (20)

// args: data (20-bit)
static __inline HI_VOID hi_isp_offset_black_10_write(ISP_DEV IspDev, HI_U32 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3a8);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3a8, (((HI_U32) (data & 0xfffff)) << 0) | (curr & 0xfff00000));
}
static __inline HI_U32 hi_isp_offset_black_10_read(ISP_DEV IspDev) {
    return (HI_U32)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3a8) & 0xfffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Black 11
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Black offset for color channel 11 (B)
// ------------------------------------------------------------------------------ //

#define HI_ISP_OFFSET_BLACK_11_DEFAULT (0x0000)
#define HI_ISP_OFFSET_BLACK_11_DATASIZE (20)

// args: data (20-bit)
static __inline HI_VOID hi_isp_offset_black_11_write(ISP_DEV IspDev, HI_U32 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3ac);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3ac, (((HI_U32) (data & 0xfffff)) << 0) | (curr & 0xfff00000));
}
static __inline HI_U32 hi_isp_offset_black_11_read(ISP_DEV IspDev) {
    return (HI_U32)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3ac) & 0xfffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Group: Radial Shading
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_radial_shading_enable
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: Enable
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Lens shading correction enable: 0=off, 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_RADIAL_SHADING_ENABLE_DEFAULT (0)
#define HI_ISP_RADIAL_SHADING_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_radial_shading_enable_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3b0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3b0, (((HI_U32) (data & 0x1)) << 0) | (curr & 0xfffffffe));
}
static __inline HI_U8 hi_isp_radial_shading_enable_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3b0) & 0x1) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: MCU priority
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 
//        Priority of CPU writes to radial shading LUTs
//        0=low. CPU read/writes from/to the shading LUTs are only executed when LUTs are not being accessed by ISP.  Normal operation.
//        1=high. CPU read/writes from/to the shading LUTs are always executed.  This may result in corrupt image data and invalid read back and is not recommended.
//        
// ------------------------------------------------------------------------------ //

#define HI_ISP_RADIAL_SHADING_MCU_PRIORITY_DEFAULT (0)
#define HI_ISP_RADIAL_SHADING_MCU_PRIORITY_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_radial_shading_mcu_priority_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3b0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3b0, (((HI_U32) (data & 0x1)) << 7) | (curr & 0xffffff7f));
}
static __inline HI_U8 hi_isp_radial_shading_mcu_priority_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3b0) & 0x80) >> 7);
}
// ------------------------------------------------------------------------------ //
// Register: MCU ready
// ------------------------------------------------------------------------------ //
// Register: hi_isp_radial_shading_mcu_ready
// ------------------------------------------------------------------------------ //
// LUT is ready to receive the data from CPU
// ------------------------------------------------------------------------------ //

#define HI_ISP_RADIAL_SHADING_MCU_READY_DEFAULT (0x0)
#define HI_ISP_RADIAL_SHADING_MCU_READY_DATASIZE (1)

// args: data (1-bit)
static __inline HI_U8 hi_isp_radial_shading_mcu_ready_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3b4) & 0x1) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_radial_shading_centerr_x
// ------------------------------------------------------------------------------ //
// Rx coordinates of shading map
// ------------------------------------------------------------------------------ //

#define HI_ISP_RADIAL_SHADING_CENTERR_X_DEFAULT (0x3C0)
#define HI_ISP_RADIAL_SHADING_CENTERR_X_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_radial_shading_centerr_x_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3b8);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3b8, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_radial_shading_centerr_x_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3b8) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_radial_shading_centerr_y
// ------------------------------------------------------------------------------ //
// Ry coordinates of shading map
// ------------------------------------------------------------------------------ //

#define HI_ISP_RADIAL_SHADING_CENTERR_Y_DEFAULT (0x21C)
#define HI_ISP_RADIAL_SHADING_CENTERR_Y_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_radial_shading_centerr_y_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3bc);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3bc, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_radial_shading_centerr_y_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3bc) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_radial_shading_centerg_x
// ------------------------------------------------------------------------------ //
// Gx coordinates of shading map
// ------------------------------------------------------------------------------ //

#define HI_ISP_RADIAL_SHADING_CENTERG_X_DEFAULT (0x3C0)
#define HI_ISP_RADIAL_SHADING_CENTERG_X_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_radial_shading_centerg_x_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3c0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3c0, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_radial_shading_centerg_x_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3c0) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_radial_shading_centerg_y
// ------------------------------------------------------------------------------ //
// Gy coordinates of shading map
// ------------------------------------------------------------------------------ //

#define HI_ISP_RADIAL_SHADING_CENTERG_Y_DEFAULT (0x21C)
#define HI_ISP_RADIAL_SHADING_CENTERG_Y_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_radial_shading_centerg_y_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3c4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3c4, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_radial_shading_centerg_y_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3c4) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_radial_shading_centerb_x
// ------------------------------------------------------------------------------ //
// Bx coordinates of shading map
// ------------------------------------------------------------------------------ //

#define HI_ISP_RADIAL_SHADING_CENTERB_X_DEFAULT (0x3C0)
#define HI_ISP_RADIAL_SHADING_CENTERB_X_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_radial_shading_centerb_x_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3c8);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3c8, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_radial_shading_centerb_x_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3c8) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_radial_shading_centerb_y
// ------------------------------------------------------------------------------ //
// By coordinates of shading map
// ------------------------------------------------------------------------------ //

#define HI_ISP_RADIAL_SHADING_CENTERB_Y_DEFAULT (0x21C)
#define HI_ISP_RADIAL_SHADING_CENTERB_Y_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_radial_shading_centerb_y_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3cc);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3cc, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_radial_shading_centerb_y_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3cc) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_radial_shading_off_center_multr
// ------------------------------------------------------------------------------ //
// normalizing factor for sum of squares
// ------------------------------------------------------------------------------ //
// 
//        Normalizing factor which scales the Red radial table to the edge of the image.
//        Calculated as 2^31/R^2 where R is the furthest distance from the center coordinate to the edge of the image in pixels.
//        
// ------------------------------------------------------------------------------ //

#define HI_ISP_RADIAL_SHADING_OFF_CENTER_MULTR_DEFAULT (0x06EA)
#define HI_ISP_RADIAL_SHADING_OFF_CENTER_MULTR_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_radial_shading_off_center_multr_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3d0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3d0, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_radial_shading_off_center_multr_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3d0) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_radial_shading_off_center_multg
// ------------------------------------------------------------------------------ //
// normalizing factor for sum of squares
// ------------------------------------------------------------------------------ //
// 
//        Normalizing factor which scales the green radial table to the edge of the image.
//        Calculated as 2^31/R^2 where R is the furthest distance from the center coordinate to the edge of the image in pixels.
//        
// ------------------------------------------------------------------------------ //

#define HI_ISP_RADIAL_SHADING_OFF_CENTER_MULTG_DEFAULT (0x06EA)
#define HI_ISP_RADIAL_SHADING_OFF_CENTER_MULTG_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_radial_shading_off_center_multg_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3d4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3d4, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_radial_shading_off_center_multg_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3d4) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_radial_shading_off_center_multb
// ------------------------------------------------------------------------------ //
// normalizing factor for sum of squares
// ------------------------------------------------------------------------------ //
// 
//        Normalizing factor which scales the blue radial table to the edge of the image.
//        Calculated as 2^31/R^2 where R is the furthest distance from the center coordinate to the edge of the image in pixels.
//        
// ------------------------------------------------------------------------------ //

#define HI_ISP_RADIAL_SHADING_OFF_CENTER_MULTB_DEFAULT (0x06EA)
#define HI_ISP_RADIAL_SHADING_OFF_CENTER_MULTB_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_radial_shading_off_center_multb_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3d8);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3d8, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_radial_shading_off_center_multb_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3d8) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Group: Mesh Shading
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Mesh Lens shading correction
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: Enable
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Lens shading correction enable: 0=off, 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_MESH_SHADING_ENABLE_DEFAULT (0)
#define HI_ISP_MESH_SHADING_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_mesh_shading_enable_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3e0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3e0, (((HI_U32) (data & 0x1)) << 0) | (curr & 0xfffffffe));
}
static __inline HI_U8 hi_isp_mesh_shading_enable_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3e0) & 0x1) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Mesh show
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Lens shading correction debug: 0=off, 1=on (show mesh data)
// ------------------------------------------------------------------------------ //

#define HI_ISP_MESH_SHADING_MESH_SHOW_DEFAULT (0)
#define HI_ISP_MESH_SHADING_MESH_SHOW_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_mesh_shading_mesh_show_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3e0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3e0, (((HI_U32) (data & 0x1)) << 1) | (curr & 0xfffffffd));
}
static __inline HI_U8 hi_isp_mesh_shading_mesh_show_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3e0) & 0x2) >> 1);
}
// ------------------------------------------------------------------------------ //
// Register: Mesh scale
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 
//        Selects the precision and maximal gain range of mesh shading correction
//        Gain range:    00->0..2; 01->0..4; 02->0..8; 03->0..16; 04->1..2; 05->1..3; 06-> 1..5; 07->1..9(float)
//        
// ------------------------------------------------------------------------------ //

#define HI_ISP_MESH_SHADING_MESH_SCALE_DEFAULT (1)
#define HI_ISP_MESH_SHADING_MESH_SCALE_DATASIZE (3)

// args: data (3-bit)
static __inline HI_VOID hi_isp_mesh_shading_mesh_scale_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3e0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3e0, (((HI_U32) (data & 0x7)) << 2) | (curr & 0xffffffe3));
}
static __inline HI_U8 hi_isp_mesh_shading_mesh_scale_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3e0) & 0x1c) >> 2);
}
// ------------------------------------------------------------------------------ //
// Register: Mesh page R
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Selects memory page for red pixels correction.  See ISP guide for further details
// ------------------------------------------------------------------------------ //

#define HI_ISP_MESH_SHADING_MESH_PAGE_R_DEFAULT (0)
#define HI_ISP_MESH_SHADING_MESH_PAGE_R_DATASIZE (2)

// args: data (2-bit)
static __inline HI_VOID hi_isp_mesh_shading_mesh_page_r_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3e0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3e0, (((HI_U32) (data & 0x3)) << 8) | (curr & 0xfffffcff));
}
static __inline HI_U8 hi_isp_mesh_shading_mesh_page_r_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3e0) & 0x300) >> 8);
}
// ------------------------------------------------------------------------------ //
// Register: Mesh page G
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Selects memory page for green pixels correction.  See ISP guide for further details
// ------------------------------------------------------------------------------ //

#define HI_ISP_MESH_SHADING_MESH_PAGE_G_DEFAULT (1)
#define HI_ISP_MESH_SHADING_MESH_PAGE_G_DATASIZE (2)

// args: data (2-bit)
static __inline HI_VOID hi_isp_mesh_shading_mesh_page_g_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3e0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3e0, (((HI_U32) (data & 0x3)) << 10) | (curr & 0xfffff3ff));
}
static __inline HI_U8 hi_isp_mesh_shading_mesh_page_g_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3e0) & 0xc00) >> 10);
}
// ------------------------------------------------------------------------------ //
// Register: Mesh page B
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Selects memory page for blue pixels correction.  See ISP guide for further details
// ------------------------------------------------------------------------------ //

#define HI_ISP_MESH_SHADING_MESH_PAGE_B_DEFAULT (2)
#define HI_ISP_MESH_SHADING_MESH_PAGE_B_DATASIZE (2)

// args: data (2-bit)
static __inline HI_VOID hi_isp_mesh_shading_mesh_page_b_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3e0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3e0, (((HI_U32) (data & 0x3)) << 12) | (curr & 0xffffcfff));
}
static __inline HI_U8 hi_isp_mesh_shading_mesh_page_b_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3e0) & 0x3000) >> 12);
}
// ------------------------------------------------------------------------------ //
// Register: Mesh width
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Number of horizontal nodes minus 1
// ------------------------------------------------------------------------------ //

#define HI_ISP_MESH_SHADING_MESH_WIDTH_DEFAULT (63)
#define HI_ISP_MESH_SHADING_MESH_WIDTH_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_isp_mesh_shading_mesh_width_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3e0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3e0, (((HI_U32) (data & 0x3f)) << 16) | (curr & 0xffc0ffff));
}
static __inline HI_U8 hi_isp_mesh_shading_mesh_width_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3e0) & 0x3f0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: Mesh height
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Number of vertical nodes minus 1
// ------------------------------------------------------------------------------ //

#define HI_ISP_MESH_SHADING_MESH_HEIGHT_DEFAULT (63)
#define HI_ISP_MESH_SHADING_MESH_HEIGHT_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_isp_mesh_shading_mesh_height_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3e0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3e0, (((HI_U32) (data & 0x3f)) << 24) | (curr & 0xc0ffffff));
}
static __inline HI_U8 hi_isp_mesh_shading_mesh_height_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3e0) & 0x3f000000) >> 24);
}
// ------------------------------------------------------------------------------ //
// Register: Mesh reload
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 0-1 triggers cache reload
// ------------------------------------------------------------------------------ //

#define HI_ISP_MESH_SHADING_MESH_RELOAD_DEFAULT (0)
#define HI_ISP_MESH_SHADING_MESH_RELOAD_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_mesh_shading_mesh_reload_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3e4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3e4, (((HI_U32) (data & 0x1)) << 0) | (curr & 0xfffffffe));
}
static __inline HI_U8 hi_isp_mesh_shading_mesh_reload_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3e4) & 0x1) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Mesh alpha mode
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 
//        Sets alpha blending between mesh shading tables.
//        0 = no alpha blending; 
//        1=2 banks (odd/even bytes) 
//        2=4 banks (one per 8 bit lane in each dword)
// ------------------------------------------------------------------------------ //

#define HI_ISP_MESH_SHADING_MESH_ALPHA_MODE_DEFAULT (0)
#define HI_ISP_MESH_SHADING_MESH_ALPHA_MODE_DATASIZE (2)

// args: data (2-bit)
static __inline HI_VOID hi_isp_mesh_shading_mesh_alpha_mode_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3f0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3f0, (((HI_U32) (data & 0x3)) << 0) | (curr & 0xfffffffc));
}
static __inline HI_U8 hi_isp_mesh_shading_mesh_alpha_mode_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3f0) & 0x3) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Mesh alpha bank R
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Bank selection for R blend: 0: 0+1; 1: 1+2; 2: 2:3; 3: 3+0; 4:0+2; 5: 1+3; 6,7: reserved
// ------------------------------------------------------------------------------ //

#define HI_ISP_MESH_SHADING_MESH_ALPHA_BANK_R_DEFAULT (0)
#define HI_ISP_MESH_SHADING_MESH_ALPHA_BANK_R_DATASIZE (3)

// args: data (3-bit)
static __inline HI_VOID hi_isp_mesh_shading_mesh_alpha_bank_r_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3f4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3f4, (((HI_U32) (data & 0x7)) << 0) | (curr & 0xfffffff8));
}
static __inline HI_U8 hi_isp_mesh_shading_mesh_alpha_bank_r_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3f4) & 0x7) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Mesh alpha bank G
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Bank selection for G blend: 0: 0+1; 1: 1+2; 2: 2:3; 3: 3+0; 4:0+2; 5: 1+3; 6,7: reserved: 
// ------------------------------------------------------------------------------ //

#define HI_ISP_MESH_SHADING_MESH_ALPHA_BANK_G_DEFAULT (0)
#define HI_ISP_MESH_SHADING_MESH_ALPHA_BANK_G_DATASIZE (3)

// args: data (3-bit)
static __inline HI_VOID hi_isp_mesh_shading_mesh_alpha_bank_g_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3f4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3f4, (((HI_U32) (data & 0x7)) << 8) | (curr & 0xfffff8ff));
}
static __inline HI_U8 hi_isp_mesh_shading_mesh_alpha_bank_g_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3f4) & 0x700) >> 8);
}
// ------------------------------------------------------------------------------ //
// Register: Mesh alpha bank B
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Bank selection for B blend: 0: 0+1; 1: 1+2; 2: 2:3; 3: 3+0; 4:0+2; 5: 1+3; 6,7: reserved
// ------------------------------------------------------------------------------ //

#define HI_ISP_MESH_SHADING_MESH_ALPHA_BANK_B_DEFAULT (0)
#define HI_ISP_MESH_SHADING_MESH_ALPHA_BANK_B_DATASIZE (3)

// args: data (3-bit)
static __inline HI_VOID hi_isp_mesh_shading_mesh_alpha_bank_b_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3f4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3f4, (((HI_U32) (data & 0x7)) << 16) | (curr & 0xfff8ffff));
}
static __inline HI_U8 hi_isp_mesh_shading_mesh_alpha_bank_b_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3f4) & 0x70000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: Mesh alpha R
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Alpha blend coeff for R
// ------------------------------------------------------------------------------ //

#define HI_ISP_MESH_SHADING_MESH_ALPHA_R_DEFAULT (0)
#define HI_ISP_MESH_SHADING_MESH_ALPHA_R_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_mesh_shading_mesh_alpha_r_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3f8);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3f8, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_mesh_shading_mesh_alpha_r_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3f8) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Mesh alpha G
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Alpha blend coeff for G
// ------------------------------------------------------------------------------ //

#define HI_ISP_MESH_SHADING_MESH_ALPHA_G_DEFAULT (0)
#define HI_ISP_MESH_SHADING_MESH_ALPHA_G_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_mesh_shading_mesh_alpha_g_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3f8);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3f8, (((HI_U32) (data & 0xff)) << 8) | (curr & 0xffff00ff));
}
static __inline HI_U8 hi_isp_mesh_shading_mesh_alpha_g_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3f8) & 0xff00) >> 8);
}
// ------------------------------------------------------------------------------ //
// Register: Mesh alpha B
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Alpha blend coeff for B
// ------------------------------------------------------------------------------ //

#define HI_ISP_MESH_SHADING_MESH_ALPHA_B_DEFAULT (0)
#define HI_ISP_MESH_SHADING_MESH_ALPHA_B_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_mesh_shading_mesh_alpha_b_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3f8);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3f8, (((HI_U32) (data & 0xff)) << 16) | (curr & 0xff00ffff));
}
static __inline HI_U8 hi_isp_mesh_shading_mesh_alpha_b_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3f8) & 0xff0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: Mesh strength
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Mesh strength in 4.12 format, e.g. 0 - no correction, 4096 - correction to match mesh data. Can be used to reduce shading correction based on AE.
// ------------------------------------------------------------------------------ //

#define HI_ISP_MESH_SHADING_MESH_STRENGTH_DEFAULT (0x1000)
#define HI_ISP_MESH_SHADING_MESH_STRENGTH_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_mesh_shading_mesh_strength_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3fc);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3fc, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_mesh_shading_mesh_strength_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3fc) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Group: Drc
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 
//          Iridix is an adaptive, space-variant tone mapping engine.
//          It is used to maintain or enhance shadow detail while preserving highlights.
//      
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_drc_enable
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Iridix enable: 0=off 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_ENABLE_DEFAULT (0x1)
#define HI_ISP_DRC_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_drc_enable_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x400);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x400, (((HI_U32) (data & 0x1)) << 0) | (curr & 0xfffffffe));
}
static __inline HI_U8 hi_isp_drc_enable_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x400) & 0x1) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_drc_max_type
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Max Bayer Algorithm Type.
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_MAX_ALG_TYPE_DEFAULT (0x1)
#define HI_ISP_DRC_MAX_ALG_TYPE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_drc_max_type_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x400);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x400, (((HI_U32) (data & 0x1)) << 3) | (curr & 0xfffffff7));
}
static __inline HI_U8 hi_isp_drc_max_type_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x400) & 0x8) >> 3);
}
// ------------------------------------------------------------------------------ //
// Register: black_level_amp0
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 1=Ignore Black level (set to zero) in amplificator. 0=Use Black level value.
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_BLACK_LEVEL_AMP0_DEFAULT (0x1)
#define HI_ISP_DRC_BLACK_LEVEL_AMP0_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_drc_black_level_amp0_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x400);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x400, (((HI_U32) (data & 0x1)) << 5) | (curr & 0xffffffdf));
}
static __inline HI_U8 hi_isp_drc_black_level_amp0_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x400) & 0x20) >> 5);
}
// ------------------------------------------------------------------------------ //
// Register: postgamma_pos
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// PosGamma application  0=gain 1=data
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_POSTGAMMA_POS_DEFAULT (0x0)
#define HI_ISP_DRC_POSTGAMMA_POS_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_drc_postgamma_pos_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x400);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x400, (((HI_U32) (data & 0x1)) << 6) | (curr & 0xffffffbf));
}
static __inline HI_U8 hi_isp_drc_postgamma_pos_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x400) & 0x40) >> 6);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_drc_collect_ovl
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_COLLECT_OVL_DEFAULT (0x0)
#define HI_ISP_DRC_COLLECT_OVL_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_drc_collect_ovl_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x400);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x400, (((HI_U32) (data & 0x1)) << 8) | (curr & 0xfffffeff));
}
static __inline HI_U8 hi_isp_drc_collect_ovl_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x400) & 0x100) >> 8);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_drc_collect_rnd
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_COLLECT_RND_DEFAULT (0x1)
#define HI_ISP_DRC_COLLECT_RND_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_drc_collect_rnd_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x400);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x400, (((HI_U32) (data & 0x1)) << 9) | (curr & 0xfffffdff));
}
static __inline HI_U8 hi_isp_drc_collect_rnd_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x400) & 0x200) >> 9);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_drc_stat_norm
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_STAT_NORM_DEFAULT (0x1)
#define HI_ISP_DRC_STAT_NORM_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_drc_stat_norm_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x400);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x400, (((HI_U32) (data & 0x1)) << 10) | (curr & 0xfffffbff));
}
static __inline HI_U8 hi_isp_drc_stat_norm_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x400) & 0x400) >> 10);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_drc_stat_mult
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_STAT_MULT_DEFAULT (0x1)
#define HI_ISP_DRC_STAT_MULT_DATASIZE (2)

// args: data (2-bit)
static __inline HI_VOID hi_isp_drc_stat_mult_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x400);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x400, (((HI_U32) (data & 0x3)) << 14) | (curr & 0xffff3fff));
}
static __inline HI_U8 hi_isp_drc_stat_mult_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x400) & 0xc000) >> 14);
}
// ------------------------------------------------------------------------------ //
// Register: variance_space
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Sets the degree of spatial sensitivity of the algorithm(Irdx7F)
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_VARIANCE_SPACE_DEFAULT (0x2)
#define HI_ISP_DRC_VARIANCE_SPACE_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_isp_drc_variance_space_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x408);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x408, (((HI_U32) (data & 0xf)) << 0) | (curr & 0xfffffff0));
}
static __inline HI_U8 hi_isp_drc_variance_space_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x408) & 0xf) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: variance_intensity
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Sets the degree of luminance sensitivity of the algorithm(Irdx7F)
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_VARIANCE_INTENSITY_DEFAULT (0x1)
#define HI_ISP_DRC_VARIANCE_INTENSITY_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_isp_drc_variance_intensity_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x408);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x408, (((HI_U32) (data & 0xf)) << 4) | (curr & 0xffffff0f));
}
static __inline HI_U8 hi_isp_drc_variance_intensity_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x408) & 0xf0) >> 4);
}
// ------------------------------------------------------------------------------ //
// Register: slope_max
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Restricts the maximum slope (gain) which can be generated by the adaptive algorithm
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_SLOPE_MAX_DEFAULT (0x80)
#define HI_ISP_DRC_SLOPE_MAX_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_drc_slope_max_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x408);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x408, (((HI_U32) (data & 0xff)) << 8) | (curr & 0xffff00ff));
}
static __inline HI_U8 hi_isp_drc_slope_max_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x408) & 0xff00) >> 8);
}
// ------------------------------------------------------------------------------ //
// Register: slope_min
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Restricts the minimum slope (gain) which can be generated by the adaptive algorithm
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_SLOPE_MIN_DEFAULT (0x40)
#define HI_ISP_DRC_SLOPE_MIN_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_drc_slope_min_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x408);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x408, (((HI_U32) (data & 0xff)) << 16) | (curr & 0xff00ffff));
}
static __inline HI_U8 hi_isp_drc_slope_min_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x408) & 0xff0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: black_level
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Iridix black level. Values below this will not be affected by Iridix.
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_BLACK_LEVEL_DEFAULT (0x00000)
#define HI_ISP_DRC_BLACK_LEVEL_DATASIZE (20)

// args: data (20-bit)
static __inline HI_VOID hi_isp_drc_black_level_write(ISP_DEV IspDev, HI_U32 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x410);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x410, (((HI_U32) (data & 0xfffff)) << 0) | (curr & 0xfff00000));
}
static __inline HI_U32 hi_isp_drc_black_level_read(ISP_DEV IspDev) {
    return (HI_U32)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x410) & 0xfffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_drc_white_level
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Drc white level. Values above this will not be affected by Drc.
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_WHITE_LEVEL_DEFAULT (0xFFFFF)
#define HI_ISP_DRC_WHITE_LEVEL_DATASIZE (20)

// args: data (20-bit)
static __inline HI_VOID hi_isp_drc_white_level_write(ISP_DEV IspDev, HI_U32 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x414);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x414, (((HI_U32) (data & 0xfffff)) << 0) | (curr & 0xfff00000));
}
static __inline HI_U32 hi_isp_drc_white_level_read(ISP_DEV IspDev) {
    return (HI_U32)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x414) & 0xfffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: collection_correction
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_COLLECTION_CORRECTION_DEFAULT (0x100)
#define HI_ISP_DRC_COLLECTION_CORRECTION_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_drc_collection_correction_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x41c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x41c, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_drc_collection_correction_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x41c) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: fwd_percept_enable
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Iridix lookup 2 enable: 0=off 1=on.
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_FWD_PERCEPT_ENABLE_DEFAULT (0x1)
#define HI_ISP_DRC_FWD_PERCEPT_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_drc_fwd_percept_enable_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x420);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x420, (((HI_U32) (data & 0x1)) << 0) | (curr & 0xfffffffe));
}
static __inline HI_U8 hi_isp_drc_fwd_percept_enable_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x420) & 0x1) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: rev_percept_enable
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Iridix lookup 1 enable: 0=off 1=on.
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_REV_PERCEPT_ENABLE_DEFAULT (0x1)
#define HI_ISP_DRC_REV_PERCEPT_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_drc_rev_percept_enable_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x420);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x420, (((HI_U32) (data & 0x1)) << 1) | (curr & 0xfffffffd));
}
static __inline HI_U8 hi_isp_drc_rev_percept_enable_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x420) & 0x2) >> 1);
}
// ------------------------------------------------------------------------------ //
// Register: strength_inroi
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_STRENGTH_INROI_DEFAULT (0x200)
#define HI_ISP_DRC_STRENGTH_INROI_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_isp_drc_strength_inroi_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x420);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x420, (((HI_U32) (data & 0x3ff)) << 16) | (curr & 0xfc00ffff));
}
static __inline HI_U16 hi_isp_drc_strength_inroi_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x420) & 0x3ff0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: strength_outroi
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_STRENGTH_OUTROI_DEFAULT (0x200)
#define HI_ISP_DRC_STRENGTH_OUTROI_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_isp_drc_strength_outroi_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x424);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x424, (((HI_U32) (data & 0x3ff)) << 0) | (curr & 0xfffffc00));
}
static __inline HI_U16 hi_isp_drc_strength_outroi_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x424) & 0x3ff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: roi_hor_start
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_ROI_HOR_START_DEFAULT (0x0000)
#define HI_ISP_DRC_ROI_HOR_START_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_drc_roi_hor_start_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x424);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x424, (((HI_U32) (data & 0xffff)) << 16) | (curr & 0xffff));
}
static __inline HI_U16 hi_isp_drc_roi_hor_start_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x424) & 0xffff0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: roi_hor_end
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_ROI_HOR_END_DEFAULT (0xFFFF)
#define HI_ISP_DRC_ROI_HOR_END_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_drc_roi_hor_end_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x428);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x428, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_drc_roi_hor_end_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x428) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: roi_ver_start
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_ROI_VER_START_DEFAULT (0x0000)
#define HI_ISP_DRC_ROI_VER_START_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_drc_roi_ver_start_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x428);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x428, (((HI_U32) (data & 0xffff)) << 16) | (curr & 0xffff));
}
static __inline HI_U16 hi_isp_drc_roi_ver_start_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x428) & 0xffff0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: roi_ver_end
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_ROI_VER_END_DEFAULT (0xFFFF)
#define HI_ISP_DRC_ROI_VER_END_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_drc_roi_ver_end_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x42c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x42c, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_drc_roi_ver_end_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x42c) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: filter_mux
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_FILTER_MUX_DEFAULT (0x00)
#define HI_ISP_DRC_FILTER_MUX_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_drc_filter_mux_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x430);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x430, (((HI_U32) (data & 0x1)) << 0) | (curr & 0xfffffffe));
}
static __inline HI_U8 hi_isp_drc_filter_mux_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x430) & 0x1) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: svariance
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_SVARIANCE_DEFAULT (0x0)
#define HI_ISP_DRC_SVARIANCE_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_isp_drc_svariance_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x430);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x430, (((HI_U32) (data & 0xf)) << 8) | (curr & 0xfffff0ff));
}
static __inline HI_U8 hi_isp_drc_svariance_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x430) & 0xf00) >> 8);
}
// ------------------------------------------------------------------------------ //
// Register: bright_pr
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_BRIGHT_PR_DEFAULT (0xA0)
#define HI_ISP_DRC_BRIGHT_PR_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_drc_bright_pr_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x430);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x430, (((HI_U32) (data & 0xff)) << 16) | (curr & 0xff00ffff));
}
static __inline HI_U8 hi_isp_drc_bright_pr_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x430) & 0xff0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: contrast
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_CONTRAST_DEFAULT (0xB0)
#define HI_ISP_DRC_CONTRAST_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_drc_contrast_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x430);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x430, (((HI_U32) (data & 0xff)) << 24) | (curr & 0xffffff));
}
static __inline HI_U8 hi_isp_drc_contrast_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x430) & 0xff000000) >> 24);
}
// ------------------------------------------------------------------------------ //
// Register: dark_enh
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_DARK_ENH_DEFAULT (0x1000)
#define HI_ISP_DRC_DARK_ENH_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_drc_dark_enh_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x434);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x434, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_drc_dark_enh_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x434) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: MCU priority
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 
//        Priority of CPU writes to LUTs
//        0=low. CPU read/writes from/to the frontend LUTs are only executed when LUTs are not being accessed by ISP.  Normal operation.
//        1=high. CPU read/writes from/to the frontend LUTs are always executed.  This may result in corrupt image data and invalid read back and is not recommended.
//        
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_MCU_PRIORITY_DEFAULT (0)
#define HI_ISP_DRC_MCU_PRIORITY_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_drc_mcu_priority_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x438);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x438, (((HI_U32) (data & 0x1)) << 0) | (curr & 0xfffffffe));
}
static __inline HI_U8 hi_isp_drc_mcu_priority_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x438) & 0x1) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: MCU ready fwd
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// fwd percept LUT status indicator.  When 1, LUT is ready to receive the data from CPU
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_MCU_READY_FWD_DEFAULT (0x0)
#define HI_ISP_DRC_MCU_READY_FWD_DATASIZE (1)

// args: data (1-bit)
static __inline HI_U8 hi_isp_drc_mcu_ready_fwd_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x438) & 0x2) >> 1);
}
// ------------------------------------------------------------------------------ //
// Register: MCU ready rev
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// rev_percept LUT status indicator.  When 1, LUT is ready to receive the data from CPU
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_MCU_READY_REV_DEFAULT (0x0)
#define HI_ISP_DRC_MCU_READY_REV_DATASIZE (1)

// args: data (1-bit)
static __inline HI_U8 hi_isp_drc_mcu_ready_rev_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x438) & 0x4) >> 2);
}
// ------------------------------------------------------------------------------ //
// Group: Antifog Hist
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: skip x
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Histogram decimation in horizontal direction: 0=every 2nd pixel; 1=every 3rd pixel; 2=every 4th pixel; 3=every 5th pixel; 4=every 8th pixel ; 5+=every 9th pixel
// ------------------------------------------------------------------------------ //

#define HI_ISP_ANTIFOG_HIST_SKIP_X_DEFAULT (0)
#define HI_ISP_ANTIFOG_HIST_SKIP_X_DATASIZE (3)

// args: data (3-bit)
static __inline HI_VOID hi_isp_antifog_hist_skip_x_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x460);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x460, (((HI_U32) (data & 0x7)) << 0) | (curr & 0xfffffff8));
}
static __inline HI_U8 hi_isp_antifog_hist_skip_x_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x460) & 0x7) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: skip y
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Histogram decimation in vertical direction: 0=every pixel; 1=every 2nd pixel; 2=every 3rd pixel; 3=every 4th pixel; 4=every 5th pixel; 5=every 8th pixel ; 6+=every 9th pixel
// ------------------------------------------------------------------------------ //

#define HI_ISP_ANTIFOG_HIST_SKIP_Y_DEFAULT (0)
#define HI_ISP_ANTIFOG_HIST_SKIP_Y_DATASIZE (3)

// args: data (3-bit)
static __inline HI_VOID hi_isp_antifog_hist_skip_y_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x460);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x460, (((HI_U32) (data & 0x7)) << 4) | (curr & 0xffffff8f));
}
static __inline HI_U8 hi_isp_antifog_hist_skip_y_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x460) & 0x70) >> 4);
}
// ------------------------------------------------------------------------------ //
// Register: offset x
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 0= start from the first column;  1=start from second column
// ------------------------------------------------------------------------------ //

#define HI_ISP_ANTIFOG_HIST_OFFSET_X_DEFAULT (0)
#define HI_ISP_ANTIFOG_HIST_OFFSET_X_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_antifog_hist_offset_x_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x460);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x460, (((HI_U32) (data & 0x1)) << 3) | (curr & 0xfffffff7));
}
static __inline HI_U8 hi_isp_antifog_hist_offset_x_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x460) & 0x8) >> 3);
}
// ------------------------------------------------------------------------------ //
// Register: offset y
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 0= start from the first row; 1= start from second row
// ------------------------------------------------------------------------------ //

#define HI_ISP_ANTIFOG_HIST_OFFSET_Y_DEFAULT (0)
#define HI_ISP_ANTIFOG_HIST_OFFSET_Y_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_antifog_hist_offset_y_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x460);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x460, (((HI_U32) (data & 0x1)) << 7) | (curr & 0xffffff7f));
}
static __inline HI_U8 hi_isp_antifog_hist_offset_y_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x460) & 0x80) >> 7);
}
// ------------------------------------------------------------------------------ //
// Register: Plane mode
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Plane separation mode
// ------------------------------------------------------------------------------ //

#define HI_ISP_ANTIFOG_HIST_PLANE_MODE_DEFAULT (0)
#define HI_ISP_ANTIFOG_HIST_PLANE_MODE_DATASIZE (3)
#define HI_ISP_ANTIFOG_HIST_PLANE_MODE_COLLECT_ALL_THE_PLANES_IN_ONE_HISTOGRAM (0)
#define HI_ISP_ANTIFOG_HIST_PLANE_MODE_COLLECT_4_BAYER_PLANES_INTO_4_SEPARATE_BANKS (1)
#define HI_ISP_ANTIFOG_HIST_PLANE_MODE_RESERVED_2 (2)
#define HI_ISP_ANTIFOG_HIST_PLANE_MODE_RESERVED_3 (3)
#define HI_ISP_ANTIFOG_HIST_PLANE_MODE_COLLECT_ODD__X_ODD__Y_PLANE_TO_BANK_0_REST_TO_BANK_1 (4)
#define HI_ISP_ANTIFOG_HIST_PLANE_MODE_COLLECT_EVEN_X_ODD__Y_PLANE_TO_BANK_0_REST_TO_BANK_1 (5)
#define HI_ISP_ANTIFOG_HIST_PLANE_MODE_COLLECT_ODD__X_EVEN_Y_PLANE_TO_BANK_0_REST_TO_BANK_1 (6)
#define HI_ISP_ANTIFOG_HIST_PLANE_MODE_COLLECT_EVEN_X_EVEN_Y_PLANE_TO_BANK_0_REST_TO_BANK_1 (7)

// args: data (3-bit)
static __inline HI_VOID hi_isp_antifog_hist_plane_mode_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x460);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x460, (((HI_U32) (data & 0x7)) << 8) | (curr & 0xfffff8ff));
}
static __inline HI_U8 hi_isp_antifog_hist_plane_mode_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x460) & 0x700) >> 8);
}
// ------------------------------------------------------------------------------ //
// Register: scale bottom
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// scale of bottom half of the range: 0=1x ,1=2x, 2=4x, 4=8x, 4=16x
// ------------------------------------------------------------------------------ //

#define HI_ISP_ANTIFOG_HIST_SCALE_BOTTOM_DEFAULT (0)
#define HI_ISP_ANTIFOG_HIST_SCALE_BOTTOM_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_isp_antifog_hist_scale_bottom_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x464);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x464, (((HI_U32) (data & 0xf)) << 0) | (curr & 0xfffffff0));
}
static __inline HI_U8 hi_isp_antifog_hist_scale_bottom_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x464) & 0xf) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: scale top
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// scale of top half of the range: 0=1x ,1=2x, 2=4x, 4=8x, 4=16x
// ------------------------------------------------------------------------------ //

#define HI_ISP_ANTIFOG_HIST_SCALE_TOP_DEFAULT (0)
#define HI_ISP_ANTIFOG_HIST_SCALE_TOP_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_isp_antifog_hist_scale_top_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x464);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x464, (((HI_U32) (data & 0xf)) << 4) | (curr & 0xffffff0f));
}
static __inline HI_U8 hi_isp_antifog_hist_scale_top_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x464) & 0xf0) >> 4);
}
// ------------------------------------------------------------------------------ //
// Register: Total Pixels
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Total number of pixels processed (skip x and skip y are taken into account)
// ------------------------------------------------------------------------------ //

#define HI_ISP_ANTIFOG_HIST_TOTAL_PIXELS_DEFAULT (0)
#define HI_ISP_ANTIFOG_HIST_TOTAL_PIXELS_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_antifog_hist_total_pixels_read(ISP_DEV IspDev) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x468);
}
// ------------------------------------------------------------------------------ //
// Register: Counted Pixels
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Number of pixels accumulated (with nonzero weight)
// ------------------------------------------------------------------------------ //

#define HI_ISP_ANTIFOG_HIST_COUNTED_PIXELS_DEFAULT (0)
#define HI_ISP_ANTIFOG_HIST_COUNTED_PIXELS_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_antifog_hist_counted_pixels_read(ISP_DEV IspDev) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x46c);
}
// ------------------------------------------------------------------------------ //
// LUT: Plane Total
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Total pixels processed for each plane
// ------------------------------------------------------------------------------ //

#define HI_ISP_ANTIFOG_HIST_PLANE_TOTAL_LUT_NODES (4)
#define HI_ISP_ANTIFOG_HIST_PLANE_TOTAL_LUT_ADDRBITS (2)
#define HI_ISP_ANTIFOG_HIST_PLANE_TOTAL_LUT_DATASIZE (27)

// args: index (0-3), data (27-bit)
static __inline HI_VOID hi_isp_antifog_hist_plane_total_lut_write(ISP_DEV IspDev, HI_U8 index,HI_U32 data) {
    HI_U32 addr = 0x440 + (index << 2);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr, data);
}
// ------------------------------------------------------------------------------ //
// LUT: Plane Counted
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Total pixels accumulated for each plane
// ------------------------------------------------------------------------------ //

#define HI_ISP_ANTIFOG_HIST_PLANE_COUNTED_LUT_NODES (4)
#define HI_ISP_ANTIFOG_HIST_PLANE_COUNTED_LUT_ADDRBITS (2)
#define HI_ISP_ANTIFOG_HIST_PLANE_COUNTED_LUT_DATASIZE (27)

// args: index (0-3), data (27-bit)
static __inline HI_VOID hi_isp_antifog_hist_plane_counted_lut_write(ISP_DEV IspDev, HI_U8 index,HI_U32 data) {
    HI_U32 addr = 0x450 + (index << 2);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr, data);
}
// ------------------------------------------------------------------------------ //
// Group: Demosaic
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Bayer Demosaic
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_vh_slope
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Slope of vertical/horizontal blending threshold in 4.4 logarithmic format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_VH_SLOPE_DEFAULT (0xC0)
#define HI_ISP_DEMOSAIC_VH_SLOPE_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_demosaic_vh_slope_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x480);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x480, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_demosaic_vh_slope_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x480) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_aa_slope
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Slope of angular blending threshold in 4.4 logarithmic format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_AA_SLOPE_DEFAULT (0xC0)
#define HI_ISP_DEMOSAIC_AA_SLOPE_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_demosaic_aa_slope_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x484);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x484, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_demosaic_aa_slope_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x484) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_va_slope
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Slope of VH-AA (VA) blending threshold in 4.4 logarithmic format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_VA_SLOPE_DEFAULT (0xAA)
#define HI_ISP_DEMOSAIC_VA_SLOPE_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_demosaic_va_slope_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x488);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x488, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_demosaic_va_slope_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x488) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_uu_slope
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Slope of undefined blending threshold in 4.4 logarithmic format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_UU_SLOPE_DEFAULT (0xAD)
#define HI_ISP_DEMOSAIC_UU_SLOPE_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_demosaic_uu_slope_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x48c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x48c, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_demosaic_uu_slope_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x48c) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_sat_slope
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Slope of saturation blending threshold in linear format 2.6
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_SAT_SLOPE_DEFAULT (0x5D)
#define HI_ISP_DEMOSAIC_SAT_SLOPE_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_demosaic_sat_slope_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x490);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x490, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_demosaic_sat_slope_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x490) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_vh_thresh
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Threshold for the range of vertical/horizontal blending
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_VH_THRESH_DEFAULT (0x131)
#define HI_ISP_DEMOSAIC_VH_THRESH_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_demosaic_vh_thresh_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x494);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x494, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_demosaic_vh_thresh_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x494) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_aa_thresh
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Threshold for the range of angular blending
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_AA_THRESH_DEFAULT (0xA0)
#define HI_ISP_DEMOSAIC_AA_THRESH_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_demosaic_aa_thresh_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x498);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x498, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_demosaic_aa_thresh_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x498) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_va_thresh
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Threshold for the range of VA blending
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_VA_THRESH_DEFAULT (0x70)
#define HI_ISP_DEMOSAIC_VA_THRESH_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_demosaic_va_thresh_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x49c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x49c, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_demosaic_va_thresh_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x49c) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_uu_thresh
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Threshold for the range of undefined blending
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_UU_THRESH_DEFAULT (0x171)
#define HI_ISP_DEMOSAIC_UU_THRESH_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_demosaic_uu_thresh_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4a0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4a0, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_demosaic_uu_thresh_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4a0) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_sat_thresh
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Threshold for the range of saturation blending  in signed 2.9 format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_SAT_THRESH_DEFAULT (0x171)
#define HI_ISP_DEMOSAIC_SAT_THRESH_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_demosaic_sat_thresh_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4a4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4a4, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_demosaic_sat_thresh_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4a4) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_vh_offset
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Offset for vertical/horizontal blending threshold
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_VH_OFFSET_DEFAULT (0x800)
#define HI_ISP_DEMOSAIC_VH_OFFSET_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_demosaic_vh_offset_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4a8);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4a8, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_demosaic_vh_offset_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4a8) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_aa_offset
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Offset for angular blending threshold
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_AA_OFFSET_DEFAULT (0x800)
#define HI_ISP_DEMOSAIC_AA_OFFSET_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_demosaic_aa_offset_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4ac);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4ac, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_demosaic_aa_offset_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4ac) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_va_offset
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Offset for VA blending threshold
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_VA_OFFSET_DEFAULT (0x800)
#define HI_ISP_DEMOSAIC_VA_OFFSET_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_demosaic_va_offset_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4b0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4b0, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_demosaic_va_offset_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4b0) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_uu_offset
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Offset for undefined blending threshold
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_UU_OFFSET_DEFAULT (0x000)
#define HI_ISP_DEMOSAIC_UU_OFFSET_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_demosaic_uu_offset_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4b4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4b4, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_demosaic_uu_offset_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4b4) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_sat_offset
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Offset for saturation blending threshold in signed 2.9 format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_SAT_OFFSET_DEFAULT (0x000)
#define HI_ISP_DEMOSAIC_SAT_OFFSET_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_demosaic_sat_offset_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4b8);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4b8, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_demosaic_sat_offset_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4b8) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_sharp_alt_d
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Directional sharp mask strength in signed 4.4 format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_SHARP_ALT_D_DEFAULT (0x30)
#define HI_ISP_DEMOSAIC_SHARP_ALT_D_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_demosaic_sharp_alt_d_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4bc);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4bc, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_demosaic_sharp_alt_d_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4bc) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_sharp_alt_ud
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Non-directional sharp mask strength in signed 4.4 format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_SHARP_ALT_UD_DEFAULT (0x20)
#define HI_ISP_DEMOSAIC_SHARP_ALT_UD_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_demosaic_sharp_alt_ud_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4c0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4c0, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_demosaic_sharp_alt_ud_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4c0) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_lum_thresh
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Luminance threshold for directional sharpening
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_LUM_THRESH_DEFAULT (0x060)
#define HI_ISP_DEMOSAIC_LUM_THRESH_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_demosaic_lum_thresh_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4c4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4c4, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_demosaic_lum_thresh_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4c4) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_np_offset
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Noise profile offset in logarithmic 4.4 format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_NP_OFFSET_DEFAULT (0x00)
#define HI_ISP_DEMOSAIC_NP_OFFSET_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_demosaic_np_offset_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4c8);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4c8, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_demosaic_np_offset_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4c8) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_dmsc_config
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Debug output select. Set to 0x00 for normal operation.
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_DMSC_CONFIG_DEFAULT (0x00)
#define HI_ISP_DEMOSAIC_DMSC_CONFIG_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_demosaic_dmsc_config_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4cc);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4cc, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_demosaic_dmsc_config_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4cc) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_ac_thresh
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Threshold for the range of AC blending in signed 2.9 format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_AC_THRESH_DEFAULT (0x1B3)
#define HI_ISP_DEMOSAIC_AC_THRESH_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_demosaic_ac_thresh_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4d0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4d0, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_demosaic_ac_thresh_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4d0) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_ac_slope
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Slope of AC blending threshold in linear format 2.6
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_AC_SLOPE_DEFAULT (0xCF)
#define HI_ISP_DEMOSAIC_AC_SLOPE_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_demosaic_ac_slope_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4d4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4d4, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_demosaic_ac_slope_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4d4) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_ac_offset
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Offset for AC blending threshold in signed 2.9 format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_AC_OFFSET_DEFAULT (0x000)
#define HI_ISP_DEMOSAIC_AC_OFFSET_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_demosaic_ac_offset_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4d8);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4d8, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_demosaic_ac_offset_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4d8) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_fc_slope
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Slope (strength) of false color correction
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_FC_SLOPE_DEFAULT (0x80)
#define HI_ISP_DEMOSAIC_FC_SLOPE_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_demosaic_fc_slope_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4dc);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4dc, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_demosaic_fc_slope_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4dc) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_fc_alias_slope
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Slope (strength) of false colour correction after blending with saturation value in 2.6 unsigned format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_FC_ALIAS_SLOPE_DEFAULT (0x55)
#define HI_ISP_DEMOSAIC_FC_ALIAS_SLOPE_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_demosaic_fc_alias_slope_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4e0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4e0, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_demosaic_fc_alias_slope_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4e0) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_fc_alias_thresh
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Threshold of false colour correction after blending with saturation valuet in in 0.8 unsigned format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_FC_ALIAS_THRESH_DEFAULT (0x00)
#define HI_ISP_DEMOSAIC_FC_ALIAS_THRESH_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_demosaic_fc_alias_thresh_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4e4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4e4, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_demosaic_fc_alias_thresh_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4e4) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Noise profile black level offset
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_NP_OFF_DEFAULT (0)
#define HI_ISP_DEMOSAIC_NP_OFF_DATASIZE (7)

// args: data (7-bit)
static __inline HI_VOID hi_isp_demosaic_np_off_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4ec);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4ec, (((HI_U32) (data & 0x7f)) << 0) | (curr & 0xffffff80));
}
static __inline HI_U8 hi_isp_demosaic_np_off_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4ec) & 0x7f) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_np_off_reflect
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 
//          Defines how values below black level are obtained.
//          0: Repeat the first table entry.
//          1: Reflect the noise profile curve below black level.
//        
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_NP_OFF_REFLECT_DEFAULT (0)
#define HI_ISP_DEMOSAIC_NP_OFF_REFLECT_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_demosaic_np_off_reflect_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4ec);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4ec, (((HI_U32) (data & 0x1)) << 7) | (curr & 0xffffff7f));
}
static __inline HI_U8 hi_isp_demosaic_np_off_reflect_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4ec) & 0x80) >> 7);
}
// ------------------------------------------------------------------------------ //
// Register: sharp_alt_ld
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Sharpen strength for L_Ld in unsigned 4.4 format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_SHARP_ALT_LD_DEFAULT (0x10)
#define HI_ISP_DEMOSAIC_SHARP_ALT_LD_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_demosaic_sharp_alt_ld_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4f0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4f0, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_demosaic_sharp_alt_ld_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4f0) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: sharp_alt_ldu
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Sharpen strength for L_Ldu in unsigned 4.4 format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_SHARP_ALT_LDU_DEFAULT (0x10)
#define HI_ISP_DEMOSAIC_SHARP_ALT_LDU_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_demosaic_sharp_alt_ldu_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4f0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4f0, (((HI_U32) (data & 0xff)) << 8) | (curr & 0xffff00ff));
}
static __inline HI_U8 hi_isp_demosaic_sharp_alt_ldu_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4f0) & 0xff00) >> 8);
}
// ------------------------------------------------------------------------------ //
// Register: sharp_alt_lu
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Sharpen strength for L_Lu in unsigned 4.4 format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_SHARP_ALT_LU_DEFAULT (0x10)
#define HI_ISP_DEMOSAIC_SHARP_ALT_LU_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_demosaic_sharp_alt_lu_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4f0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4f0, (((HI_U32) (data & 0xff)) << 16) | (curr & 0xff00ffff));
}
static __inline HI_U8 hi_isp_demosaic_sharp_alt_lu_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4f0) & 0xff0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: sad_amp
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Sad amplifier in unsigned 4.4 format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_SAD_AMP_DEFAULT (0x10)
#define HI_ISP_DEMOSAIC_SAD_AMP_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_demosaic_sad_amp_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4f0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4f0, (((HI_U32) (data & 0xff)) << 24) | (curr & 0xffffff));
}
static __inline HI_U8 hi_isp_demosaic_sad_amp_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4f0) & 0xff000000) >> 24);
}
// ------------------------------------------------------------------------------ //
// Register: min_d_strength
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Min threshold for the directional L_L in signed 2's complement s.12 format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_MIN_D_STRENGTH_DEFAULT (0x1F33)
#define HI_ISP_DEMOSAIC_MIN_D_STRENGTH_DATASIZE (13)

// args: data (13-bit)
static __inline HI_VOID hi_isp_demosaic_min_d_strength_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4f4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4f4, (((HI_U32) (data & 0x1fff)) << 0) | (curr & 0xffffe000));
}
static __inline HI_U16 hi_isp_demosaic_min_d_strength_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4f4) & 0x1fff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: min_ud_strength
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Min threshold for the un-directional L_Lu in signed 2's complement s.12 format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_MIN_UD_STRENGTH_DEFAULT (0x1F48)
#define HI_ISP_DEMOSAIC_MIN_UD_STRENGTH_DATASIZE (13)

// args: data (13-bit)
static __inline HI_VOID hi_isp_demosaic_min_ud_strength_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4f4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4f4, (((HI_U32) (data & 0x1fff)) << 16) | (curr & 0xe000ffff));
}
static __inline HI_U16 hi_isp_demosaic_min_ud_strength_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4f4) & 0x1fff0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: sharpen_alg_select
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// To select new sharp algorithm or not
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_SHARPEN_ALG_SELECT_DEFAULT (0x00)
#define HI_ISP_DEMOSAIC_SHARPEN_ALG_SELECT_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_demosaic_sharpen_alg_select_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4f8);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4f8, (((HI_U32) (data & 0x1)) << 0) | (curr & 0xfffffffe));
}
static __inline HI_U8 hi_isp_demosaic_sharpen_alg_select_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4f8) & 0x1) >> 0);
}
// ------------------------------------------------------------------------------ //
// Group: Color Matrix
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Color correction on RGB data using a 3x3 color matrix
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_matrix_enable
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Color matrix enable: 0=off 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_MATRIX_RGB_ENABLE_DEFAULT (1)
#define HI_ISP_MATRIX_RGB_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_matrix_enable_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x524);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x524, (((HI_U32) (data & 0x1)) << 0) | (curr & 0xfffffffe));
}
static __inline HI_U8 hi_isp_matrix_enable_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x524) & 0x1) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_matrix_coefft_r_r
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Matrix coefficient for red-red multiplier
// ------------------------------------------------------------------------------ //

#define HI_ISP_MATRIX_RGB_COEFFT_R_R_DEFAULT (0x0100)
#define HI_ISP_MATRIX_RGB_COEFFT_R_R_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_matrix_coefft_r_r_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x500);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x500, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_matrix_coefft_r_r_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x500) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_matrix_coefft_r_g
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Matrix coefficient for red-green multiplier
// ------------------------------------------------------------------------------ //

#define HI_ISP_MATRIX_RGB_COEFFT_R_G_DEFAULT (0x0000)
#define HI_ISP_MATRIX_RGB_COEFFT_R_G_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_matrix_coefft_r_g_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x504);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x504, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_matrix_coefft_r_g_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x504) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_matrix_coefft_r_b
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Matrix coefficient for red-blue multiplier
// ------------------------------------------------------------------------------ //

#define HI_ISP_MATRIX_RGB_COEFFT_R_B_DEFAULT (0x0000)
#define HI_ISP_MATRIX_RGB_COEFFT_R_B_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_matrix_coefft_r_b_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x508);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x508, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_matrix_coefft_r_b_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x508) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_matrix_coefft_g_r
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Matrix coefficient for green-red multiplier
// ------------------------------------------------------------------------------ //

#define HI_ISP_MATRIX_RGB_COEFFT_G_R_DEFAULT (0x0000)
#define HI_ISP_MATRIX_RGB_COEFFT_G_R_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_matrix_coefft_g_r_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x50c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x50c, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_matrix_coefft_g_r_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x50c) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_matrix_coefft_g_g
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Matrix coefficient for green-green multiplier
// ------------------------------------------------------------------------------ //

#define HI_ISP_MATRIX_RGB_COEFFT_G_G_DEFAULT (0x0100)
#define HI_ISP_MATRIX_RGB_COEFFT_G_G_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_matrix_coefft_g_g_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x510);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x510, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_matrix_coefft_g_g_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x510) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_matrix_coefft_g_b
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Matrix coefficient for green-blue multiplier
// ------------------------------------------------------------------------------ //

#define HI_ISP_MATRIX_RGB_COEFFT_G_B_DEFAULT (0x0000)
#define HI_ISP_MATRIX_RGB_COEFFT_G_B_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_matrix_coefft_g_b_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x514);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x514, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_matrix_coefft_g_b_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x514) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_matrix_coefft_b_r
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Matrix coefficient for blue-red multiplier
// ------------------------------------------------------------------------------ //

#define HI_ISP_MATRIX_RGB_COEFFT_B_R_DEFAULT (0x0000)
#define HI_ISP_MATRIX_RGB_COEFFT_B_R_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_matrix_coefft_b_r_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x518);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x518, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_matrix_coefft_b_r_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x518) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_matrix_coefft_b_g
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Matrix coefficient for blue-green multiplier
// ------------------------------------------------------------------------------ //

#define HI_ISP_MATRIX_RGB_COEFFT_B_G_DEFAULT (0x0000)
#define HI_ISP_MATRIX_RGB_COEFFT_B_G_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_matrix_coefft_b_g_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x51c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x51c, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_matrix_coefft_b_g_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x51c) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_matrix_coefft_b_b
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Matrix coefficient for blue-blue multiplier
// ------------------------------------------------------------------------------ //

#define HI_ISP_MATRIX_RGB_COEFFT_B_B_DEFAULT (0x0100)
#define HI_ISP_MATRIX_RGB_COEFFT_B_B_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_matrix_coefft_b_b_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x520);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x520, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_matrix_coefft_b_b_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x520) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_matrix_coefft_wb_r
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// gain for Red channel for antifog function
// ------------------------------------------------------------------------------ //

#define HI_ISP_MATRIX_RGB_COEFFT_WB_R_DEFAULT (0x0100)
#define HI_ISP_MATRIX_RGB_COEFFT_WB_R_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_matrix_coefft_wb_r_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x528);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x528, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_matrix_coefft_wb_r_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x528) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_matrix_coefft_wb_g
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// gain for Green channel for antifog function
// ------------------------------------------------------------------------------ //

#define HI_ISP_MATRIX_RGB_COEFFT_WB_G_DEFAULT (0x0100)
#define HI_ISP_MATRIX_RGB_COEFFT_WB_G_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_matrix_coefft_wb_g_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x52c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x52c, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_matrix_coefft_wb_g_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x52c) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_matrix_coefft_wb_b
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// gain for Blue channel for antifog function
// ------------------------------------------------------------------------------ //

#define HI_ISP_MATRIX_RGB_COEFFT_WB_B_DEFAULT (0x0100)
#define HI_ISP_MATRIX_RGB_COEFFT_WB_B_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_matrix_coefft_wb_b_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x530);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x530, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_matrix_coefft_wb_b_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x530) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_matrix_coefft_fog_offset_r
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Offset R for antifog function
// ------------------------------------------------------------------------------ //

#define HI_ISP_MATRIX_RGB_COEFFT_FOG_OFFSET_R_DEFAULT (0x0000)
#define HI_ISP_MATRIX_RGB_COEFFT_FOG_OFFSET_R_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_matrix_coefft_fog_offset_r_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x534);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x534, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_matrix_coefft_fog_offset_r_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x534) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_matrix_coefft_fog_offset_g
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Offset G for antifog function
// ------------------------------------------------------------------------------ //

#define HI_ISP_MATRIX_RGB_COEFFT_FOG_OFFSET_G_DEFAULT (0x0000)
#define HI_ISP_MATRIX_RGB_COEFFT_FOG_OFFSET_G_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_matrix_coefft_fog_offset_g_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x538);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x538, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_matrix_coefft_fog_offset_g_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x538) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_matrix_coefft_fog_offset_b
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Offset B for antifog function
// ------------------------------------------------------------------------------ //

#define HI_ISP_MATRIX_RGB_COEFFT_FOG_OFFSET_B_DEFAULT (0x0000)
#define HI_ISP_MATRIX_RGB_COEFFT_FOG_OFFSET_B_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_matrix_coefft_fog_offset_b_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x53c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x53c, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_matrix_coefft_fog_offset_b_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x53c) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Group: Gamma RGB
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Gamma correction
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_gamma_enable
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Gamma enable: 0=off 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_GAMMA_RGB_ENABLE_DEFAULT (1)
#define HI_ISP_GAMMA_RGB_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_gamma_enable_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x580);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x580, (((HI_U32) (data & 0x1)) << 0) | (curr & 0xfffffffe));
}
static __inline HI_U8 hi_isp_gamma_enable_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x580) & 0x1) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_gamma_mcu_priority
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Priority of CPU port
//          0=low. CPU read/writes from/to the LUTs are only executed when LUTs are not being accessed by ISP.  Normal operation.
//          1=high. CPU read/writes from/to the LUTs are always executed.  This may result in corrupt image data and invalid read back and is not recommended.
//        
// ------------------------------------------------------------------------------ //

#define HI_ISP_GAMMA_MCU_PRIORITY_DEFAULT (1)
#define HI_ISP_GAMMA_MCU_PRIORITY_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_gamma_mcu_priority_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x580);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x580, (((HI_U32) (data & 0x1)) << 7) | (curr & 0xffffff7f));
}
static __inline HI_U8 hi_isp_gamma_mcu_priority_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x580) & 0x80) >> 7);
}
// ------------------------------------------------------------------------------ //
// Register: Bank select
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// LUT RAM Bank select control
//          0 = RGB gamma circuit accesses bank 0
//          1 = RGB gamma circuit accesses bank 1
//        
// ------------------------------------------------------------------------------ //

#define HI_ISP_GAMMA_RGB_USE_LUT_BANK_1_DEFAULT (0)
#define HI_ISP_GAMMA_RGB_USE_LUT_BANK_1_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_gamma_rgb_use_lut_bank_1_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x580);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x580, (((HI_U32) (data & 0x1)) << 4) | (curr & 0xffffffef));
}
static __inline HI_U8 hi_isp_gamma_rgb_use_lut_bank_1_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x580) & 0x10) >> 4);
}
// ------------------------------------------------------------------------------ //
// Register: MCU ready
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// LUT is ready to receive the data from CPU
// ------------------------------------------------------------------------------ //

#define HI_ISP_GAMMA_RGB_MCU_READY_DEFAULT (0x0)
#define HI_ISP_GAMMA_RGB_MCU_READY_DATASIZE (1)

// args: data (1-bit)
static __inline HI_U8 hi_isp_gamma_rgb_mcu_ready_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x584) & 0x1) >> 0);
}
// ------------------------------------------------------------------------------ //
// Group: Gamma Bank Select
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: bank selected gamma RGB
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// This indicates RGB module is currently working on which bank.
//          0 = RGB gamma circuit accesses bank 0
//          1 = RGB gamma circuit accesses bank 1
//        
// ------------------------------------------------------------------------------ //

#define HI_ISP_GAMMA_BANK_SELECT_BANK_SELECTED_GAMMA_RGB_DEFAULT (0x0)
#define HI_ISP_GAMMA_BANK_SELECT_BANK_SELECTED_GAMMA_RGB_DATASIZE (1)

// args: data (1-bit)
static __inline HI_U8 hi_isp_gamma_bank_select_bank_selected_gamma_rgb_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x588) & 0x1) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: bank selected gamma FE0
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// This indicates RGB module is currently working on which bank.
//          0 = gamma FE0 circuit accesses bank 0
//          1 = gamma FE0 circuit accesses bank 1
//        
// ------------------------------------------------------------------------------ //

#define HI_ISP_GAMMA_BANK_SELECT_BANK_SELECTED_GAMMA_FE0_DEFAULT (0x0)
#define HI_ISP_GAMMA_BANK_SELECT_BANK_SELECTED_GAMMA_FE0_DATASIZE (1)

// args: data (1-bit)
static __inline HI_U8 hi_isp_gamma_bank_select_bank_selected_gamma_fe0_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x588) & 0x2) >> 1);
}
// ------------------------------------------------------------------------------ //
// Register: bank selected gamma FE1
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// This indicates RGB module is currently working on which bank.
//          0 = gamma FE1 circuit accesses bank 0
//          1 = gamma FE1 circuit accesses bank 1
//        
// ------------------------------------------------------------------------------ //

#define HI_ISP_GAMMA_BANK_SELECT_BANK_SELECTED_GAMMA_FE1_DEFAULT (0x0)
#define HI_ISP_GAMMA_BANK_SELECT_BANK_SELECTED_GAMMA_FE1_DATASIZE (1)

// args: data (1-bit)
static __inline HI_U8 hi_isp_gamma_bank_select_bank_selected_gamma_fe1_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x588) & 0x4) >> 2);
}
// ------------------------------------------------------------------------------ //
// Register: bank selected gamma BE0
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// This indicates RGB module is currently working on which bank.
//          0 = gamma BE0 circuit accesses bank 0
//          1 = gamma BE0 circuit accesses bank 1
//        
// ------------------------------------------------------------------------------ //

#define HI_ISP_GAMMA_BANK_SELECT_BANK_SELECTED_GAMMA_BE0_DEFAULT (0x0)
#define HI_ISP_GAMMA_BANK_SELECT_BANK_SELECTED_GAMMA_BE0_DATASIZE (1)

// args: data (1-bit)
static __inline HI_U8 hi_isp_gamma_bank_select_bank_selected_gamma_be0_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x588) & 0x8) >> 3);
}
// ------------------------------------------------------------------------------ //
// Register: bank selected gamma BE1
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// This indicates RGB module is currently working on which bank.
//          0 = gamma BE1 circuit accesses bank 0
//          1 = gamma BE1 circuit accesses bank 1
//        
// ------------------------------------------------------------------------------ //

#define HI_ISP_GAMMA_BANK_SELECT_BANK_SELECTED_GAMMA_BE1_DEFAULT (0x0)
#define HI_ISP_GAMMA_BANK_SELECT_BANK_SELECTED_GAMMA_BE1_DATASIZE (1)

// args: data (1-bit)
static __inline HI_U8 hi_isp_gamma_bank_select_bank_selected_gamma_be1_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x588) & 0x10) >> 4);
}
// ------------------------------------------------------------------------------ //
// Group: Crop
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
//  Crop 
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: Enable crop
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Crop enable: 0=off 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_FR_CROP_ENABLE_CROP_DEFAULT (0)
#define HI_ISP_FR_CROP_ENABLE_CROP_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_crop_enable_crop_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x5c0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x5c0, (((HI_U32) (data & 0x1)) << 0) | (curr & 0xfffffffe));
}
static __inline HI_U8 hi_isp_crop_enable_crop_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x5c0) & 0x1) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_crop_start_x
// ------------------------------------------------------------------------------ //
// left column of output crop window
// ------------------------------------------------------------------------------ //
// Horizontal offset from left side of image in pixels for output crop window
// ------------------------------------------------------------------------------ //

#define HI_ISP_FR_CROP_START_X_DEFAULT (0x0000)
#define HI_ISP_FR_CROP_START_X_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_crop_start_x_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x5c4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x5c4, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_crop_start_x_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x5c4) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_crop_start_y
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Vertical offset from top of image in lines for output crop window
// ------------------------------------------------------------------------------ //

#define HI_ISP_FR_CROP_START_Y_DEFAULT (0x0000)
#define HI_ISP_FR_CROP_START_Y_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_crop_start_y_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x5c8);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x5c8, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_crop_start_y_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x5c8) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_crop_size_x
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// width of output crop window
// ------------------------------------------------------------------------------ //

#define HI_ISP_FR_CROP_SIZE_X_DEFAULT (0xffff)
#define HI_ISP_FR_CROP_SIZE_X_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_crop_size_x_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x5cc);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x5cc, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_crop_size_x_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x5cc) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_crop_size_y
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// height of output crop window
// ------------------------------------------------------------------------------ //

#define HI_ISP_FR_CROP_SIZE_Y_DEFAULT (0xffff)
#define HI_ISP_FR_CROP_SIZE_Y_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_crop_size_y_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x5d0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x5d0, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_crop_size_y_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x5d0) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Group: Logo
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_logo_logo_left
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Sets x coordinate of logo (in 16-pixel steps)
// ------------------------------------------------------------------------------ //

#define HI_ISP_LOGO_LOGO_LEFT_DEFAULT (0x08)
#define HI_ISP_LOGO_LOGO_LEFT_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_logo_logo_left_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x600);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x600, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_logo_logo_left_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x600) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_logo_logo_top
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Sets y coordinate of logo (in 16-pixel steps)
// ------------------------------------------------------------------------------ //

#define HI_ISP_LOGO_LOGO_TOP_DEFAULT (0x08)
#define HI_ISP_LOGO_LOGO_TOP_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_logo_logo_top_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x604);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x604, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_logo_logo_top_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x604) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Group: Colour space conv
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Conversion of RGB to YUV data using a 3x3 color matrix plus offsets
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: Enable matrix
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Color matrix enable: 0=off 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_FR_CS_CONV_ENABLE_MATRIX_DEFAULT (0)
#define HI_ISP_FR_CS_CONV_ENABLE_MATRIX_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_cs_conv_enable_matrix_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x640);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x640, (((HI_U32) (data & 0x1)) << 0) | (curr & 0xfffffffe));
}
static __inline HI_U8 hi_isp_cs_conv_enable_matrix_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x640) & 0x1) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_cs_conv_enable_filter
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Filter enable: 0=off 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_FR_CS_CONV_ENABLE_FILTER_DEFAULT (0)
#define HI_ISP_FR_CS_CONV_ENABLE_FILTER_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_cs_conv_enable_filter_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x640);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x640, (((HI_U32) (data & 0x1)) << 1) | (curr & 0xfffffffd));
}
static __inline HI_U8 hi_isp_cs_conv_enable_filter_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x640) & 0x2) >> 1);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_cs_conv_enable_downsample
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Downsample enable: 0=off 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_FR_CS_CONV_ENABLE_HORIZONTAL_DOWNSAMPLE_DEFAULT (0)
#define HI_ISP_FR_CS_CONV_ENABLE_HORIZONTAL_DOWNSAMPLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_cs_conv_enable_downsample_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x640);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x640, (((HI_U32) (data & 0x1)) << 2) | (curr & 0xfffffffb));
}
static __inline HI_U8 hi_isp_cs_conv_enable_downsample_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x640) & 0x4) >> 2);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_cs_conv_coefft_11
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Matrix coefficient for R-Y multiplier
// ------------------------------------------------------------------------------ //

#define HI_ISP_FR_CS_CONV_COEFFT_11_DEFAULT (0x002f)
#define HI_ISP_FR_CS_CONV_COEFFT_11_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_cs_conv_coefft_11_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x610);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x610, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_cs_conv_coefft_11_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x610) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_cs_conv_coefft_12
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Matrix coefficient for G-Y multiplier
// ------------------------------------------------------------------------------ //

#define HI_ISP_FR_CS_CONV_COEFFT_12_DEFAULT (0x009d)
#define HI_ISP_FR_CS_CONV_COEFFT_12_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_cs_conv_coefft_12_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x614);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x614, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_cs_conv_coefft_12_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x614) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Coefft 13
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Matrix coefficient for B-Y multiplier
// ------------------------------------------------------------------------------ //

#define HI_ISP_FR_CS_CONV_COEFFT_13_DEFAULT (0x0010)
#define HI_ISP_FR_CS_CONV_COEFFT_13_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_cs_conv_coefft_13_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x618);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x618, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_cs_conv_coefft_13_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x618) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_cs_conv_coefft_21
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Matrix coefficient for R-Cb multiplier
// ------------------------------------------------------------------------------ //

#define HI_ISP_FR_CS_CONV_COEFFT_21_DEFAULT (0x801a)
#define HI_ISP_FR_CS_CONV_COEFFT_21_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_cs_conv_coefft_21_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x61c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x61c, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_cs_conv_coefft_21_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x61c) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Coefft 22
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Matrix coefficient for G-Cb multiplier
// ------------------------------------------------------------------------------ //

#define HI_ISP_FR_CS_CONV_COEFFT_22_DEFAULT (0x8057)
#define HI_ISP_FR_CS_CONV_COEFFT_22_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_cs_conv_coefft_22_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x620);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x620, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_cs_conv_coefft_22_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x620) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_cs_conv_coefft_23
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Matrix coefficient for B-Cb multiplier
// ------------------------------------------------------------------------------ //

#define HI_ISP_FR_CS_CONV_COEFFT_23_DEFAULT (0x0070)
#define HI_ISP_FR_CS_CONV_COEFFT_23_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_cs_conv_coefft_23_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x624);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x624, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_cs_conv_coefft_23_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x624) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Coefft 31
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Matrix coefficient for R-Cr multiplier
// ------------------------------------------------------------------------------ //

#define HI_ISP_FR_CS_CONV_COEFFT_31_DEFAULT (0x0070)
#define HI_ISP_FR_CS_CONV_COEFFT_31_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_cs_conv_coefft_31_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x628);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x628, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_cs_conv_coefft_31_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x628) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_cs_conv_coefft_32
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Matrix coefficient for G-Cr multiplier
// ------------------------------------------------------------------------------ //

#define HI_ISP_FR_CS_CONV_COEFFT_32_DEFAULT (0x8066)
#define HI_ISP_FR_CS_CONV_COEFFT_32_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_cs_conv_coefft_32_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x62c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x62c, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_cs_conv_coefft_32_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x62c) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_cs_conv_coefft_33
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Matrix coefficient for B-Cr multiplier
// ------------------------------------------------------------------------------ //

#define HI_ISP_FR_CS_CONV_COEFFT_33_DEFAULT (0x800a)
#define HI_ISP_FR_CS_CONV_COEFFT_33_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_cs_conv_coefft_33_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x630);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x630, (((HI_U32) (data & 0xffff)) << 0) | (curr & 0xffff0000));
}
static __inline HI_U16 hi_isp_cs_conv_coefft_33_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x630) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_cs_conv_coefft_o1
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Offset for Y
// ------------------------------------------------------------------------------ //

#define HI_ISP_FR_CS_CONV_COEFFT_O1_DEFAULT (0x000)
#define HI_ISP_FR_CS_CONV_COEFFT_O1_DATASIZE (11)

// args: data (11-bit)
static __inline HI_VOID hi_isp_cs_conv_coefft_o1_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x634);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x634, (((HI_U32) (data & 0x7ff)) << 0) | (curr & 0xfffff800));
}
static __inline HI_U16 hi_isp_cs_conv_coefft_o1_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x634) & 0x7ff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_cs_conv_coefft_o2
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Offset for Cb
// ------------------------------------------------------------------------------ //

#define HI_ISP_FR_CS_CONV_COEFFT_O2_DEFAULT (0x200)
#define HI_ISP_FR_CS_CONV_COEFFT_O2_DATASIZE (11)

// args: data (11-bit)
static __inline HI_VOID hi_isp_cs_conv_coefft_o2_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x638);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x638, (((HI_U32) (data & 0x7ff)) << 0) | (curr & 0xfffff800));
}
static __inline HI_U16 hi_isp_cs_conv_coefft_o2_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x638) & 0x7ff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_cs_conv_coefft_o3
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Offset for Cr
// ------------------------------------------------------------------------------ //

#define HI_ISP_FR_CS_CONV_COEFFT_O3_DEFAULT (0x200)
#define HI_ISP_FR_CS_CONV_COEFFT_O3_DATASIZE (11)

// args: data (11-bit)
static __inline HI_VOID hi_isp_cs_conv_coefft_o3_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x63c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x63c, (((HI_U32) (data & 0x7ff)) << 0) | (curr & 0xfffff800));
}
static __inline HI_U16 hi_isp_cs_conv_coefft_o3_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x63c) & 0x7ff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_cs_conv_clip_min_y
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Minimal value for Y.  Values below this are clipped.
// ------------------------------------------------------------------------------ //

#define HI_ISP_FR_CS_CONV_CLIP_MIN_Y_DEFAULT (0x000)
#define HI_ISP_FR_CS_CONV_CLIP_MIN_Y_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_isp_cs_conv_clip_min_y_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x648);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x648, (((HI_U32) (data & 0x3ff)) << 0) | (curr & 0xfffffc00));
}
static __inline HI_U16 hi_isp_cs_conv_clip_min_y_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x648) & 0x3ff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_cs_conv_clip_max_y
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Maximal value for Y.  Values above this are clipped.
// ------------------------------------------------------------------------------ //

#define HI_ISP_FR_CS_CONV_CLIP_MAX_Y_DEFAULT (0x3FF)
#define HI_ISP_FR_CS_CONV_CLIP_MAX_Y_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_isp_cs_conv_clip_max_y_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x64c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x64c, (((HI_U32) (data & 0x3ff)) << 0) | (curr & 0xfffffc00));
}
static __inline HI_U16 hi_isp_cs_conv_clip_max_y_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x64c) & 0x3ff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_cs_conv_clip_min_uv
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Minimal value for Cb, Cr.  Values below this are clipped.
// ------------------------------------------------------------------------------ //

#define HI_ISP_FR_CS_CONV_CLIP_MIN_UV_DEFAULT (0x000)
#define HI_ISP_FR_CS_CONV_CLIP_MIN_UV_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_isp_cs_conv_clip_min_uv_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x650);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x650, (((HI_U32) (data & 0x3ff)) << 0) | (curr & 0xfffffc00));
}
static __inline HI_U16 hi_isp_cs_conv_clip_min_uv_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x650) & 0x3ff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_cs_conv_clip_max_uv
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Maximal value for Cb, Cr.  Values above this are clipped.
// ------------------------------------------------------------------------------ //

#define HI_ISP_FR_CS_CONV_CLIP_MAX_UV_DEFAULT (0x3FF)
#define HI_ISP_FR_CS_CONV_CLIP_MAX_UV_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_isp_cs_conv_clip_max_uv_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x654);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x654, (((HI_U32) (data & 0x3ff)) << 0) | (curr & 0xfffffc00));
}
static __inline HI_U16 hi_isp_cs_conv_clip_max_uv_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x654) & 0x3ff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_cs_conv_data_mask_ry
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Data mask for channel 1 (R or Y).  Bit-wise and of this value and video data. 
// ------------------------------------------------------------------------------ //

#define HI_ISP_FR_CS_CONV_DATA_MASK_RY_DEFAULT (0x3FF)
#define HI_ISP_FR_CS_CONV_DATA_MASK_RY_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_isp_cs_conv_data_mask_ry_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x658);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x658, (((HI_U32) (data & 0x3ff)) << 0) | (curr & 0xfffffc00));
}
static __inline HI_U16 hi_isp_cs_conv_data_mask_ry_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x658) & 0x3ff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_cs_conv_data_mask_gu
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Data mask for channel 2 (G or U).  Bit-wise and of this value and video data.
// ------------------------------------------------------------------------------ //

#define HI_ISP_FR_CS_CONV_DATA_MASK_GU_DEFAULT (0x3FF)
#define HI_ISP_FR_CS_CONV_DATA_MASK_GU_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_isp_cs_conv_data_mask_gu_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x65c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x65c, (((HI_U32) (data & 0x3ff)) << 0) | (curr & 0xfffffc00));
}
static __inline HI_U16 hi_isp_cs_conv_data_mask_gu_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x65c) & 0x3ff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_cs_conv_data_mask_bv
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Data mask for channel 3 (B or V).  Bit-wise and of this value and video data.
// ------------------------------------------------------------------------------ //

#define HI_ISP_FR_CS_CONV_DATA_MASK_BV_DEFAULT (0x3FF)
#define HI_ISP_FR_CS_CONV_DATA_MASK_BV_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_isp_cs_conv_data_mask_bv_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x660);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x660, (((HI_U32) (data & 0x3ff)) << 0) | (curr & 0xfffffc00));
}
static __inline HI_U16 hi_isp_cs_conv_data_mask_bv_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x660) & 0x3ff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Group: Dither
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: Enable dither
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Enables dithering module
// ------------------------------------------------------------------------------ //

#define HI_ISP_DITHER_ENABLE_DITHER_DEFAULT (0x0)
#define HI_ISP_DITHER_ENABLE_DITHER_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_dither_enable_dither_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x680);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x680, (((HI_U32) (data & 0x1)) << 0) | (curr & 0xfffffffe));
}
static __inline HI_U8 hi_isp_dither_enable_dither_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x680) & 0x1) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Dither amount
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 0= dither to 9 bits; 1=dither to 8 bits; 2=dither to 7 bits; 3=dither to 6 bits 
// ------------------------------------------------------------------------------ //

#define HI_ISP_DITHER_DITHER_AMOUNT_DEFAULT (0x0)
#define HI_ISP_DITHER_DITHER_AMOUNT_DATASIZE (2)

// args: data (2-bit)
static __inline HI_VOID hi_isp_dither_dither_amount_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x680);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x680, (((HI_U32) (data & 0x3)) << 1) | (curr & 0xfffffff9));
}
static __inline HI_U8 hi_isp_dither_dither_amount_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x680) & 0x6) >> 1);
}
// ------------------------------------------------------------------------------ //
// Register: Shift mode
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 0= output is LSB aligned; 1=output is MSB aligned
// ------------------------------------------------------------------------------ //

#define HI_ISP_DITHER_SHIFT_MODE_DEFAULT (0x0)
#define HI_ISP_DITHER_SHIFT_MODE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_dither_shift_mode_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x680);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x680, (((HI_U32) (data & 0x1)) << 4) | (curr & 0xffffffef));
}
static __inline HI_U8 hi_isp_dither_shift_mode_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x680) & 0x10) >> 4);
}
// ------------------------------------------------------------------------------ //
// Group: Statistics
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Derives information for use by the AE and AWB modules
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_hist_thresh_0_1
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Histogram threshold for bin 0/1 boundary
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_HIST_THRESH_0_1_DEFAULT (0x10)
#define HI_ISP_METERING_HIST_THRESH_0_1_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_metering_hist_thresh_0_1_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x700);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x700, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_metering_hist_thresh_0_1_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x700) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_hist_thresh_1_2
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Histogram threshold for bin 1/2 boundary
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_HIST_THRESH_1_2_DEFAULT (0x20)
#define HI_ISP_METERING_HIST_THRESH_1_2_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_metering_hist_thresh_1_2_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x704);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x704, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_metering_hist_thresh_1_2_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x704) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_hist_thresh_3_4
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Histogram threshold for bin 2/3 boundary
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_HIST_THRESH_3_4_DEFAULT (0xD0)
#define HI_ISP_METERING_HIST_THRESH_3_4_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_metering_hist_thresh_3_4_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x708);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x708, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_metering_hist_thresh_3_4_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x708) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_hist_thresh_4_5
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Histogram threshold for bin 3/4 boundary
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_HIST_THRESH_4_5_DEFAULT (0xE0)
#define HI_ISP_METERING_HIST_THRESH_4_5_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_metering_hist_thresh_4_5_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x70c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x70c, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_metering_hist_thresh_4_5_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x70c) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_hist_0
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Normalized histogram results for bin 0
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_HIST_0_DEFAULT (0x0)
#define HI_ISP_METERING_HIST_0_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_isp_metering_hist_0_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x720) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_hist_1
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Normalized histogram results for bin 1
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_HIST_1_DEFAULT (0x0)
#define HI_ISP_METERING_HIST_1_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_isp_metering_hist_1_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x724) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_hist_3
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Normalized histogram results for bin 3
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_HIST_3_DEFAULT (0x0)
#define HI_ISP_METERING_HIST_3_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_isp_metering_hist_3_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x728) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_hist_4
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Normalized histogram results for bin 4
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_HIST_4_DEFAULT (0x0)
#define HI_ISP_METERING_HIST_4_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_isp_metering_hist_4_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x72c) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: AEXP Nodes Used Horiz
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Number of active zones horizontally for AE stats collection
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AEXP_NODES_USED_HORIZ_DEFAULT (17)
#define HI_ISP_METERING_AEXP_NODES_USED_HORIZ_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_metering_aexp_nodes_used_horiz_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x730);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x730, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_metering_aexp_nodes_used_horiz_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x730) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: AEXP Nodes Used Vert
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Number of active zones vertically for AE stats collection
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AEXP_NODES_USED_VERT_DEFAULT (15)
#define HI_ISP_METERING_AEXP_NODES_USED_VERT_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_metering_aexp_nodes_used_vert_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x730);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x730, (((HI_U32) (data & 0xff)) << 8) | (curr & 0xffff00ff));
}
static __inline HI_U8 hi_isp_metering_aexp_nodes_used_vert_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x730) & 0xff00) >> 8);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_awb_stats_mode
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Statistics mode: 0 - legacy(G/R,B/R), 1 - current (R/G, B/G) 
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AWB_STATS_MODE_DEFAULT (0)
#define HI_ISP_METERING_AWB_STATS_MODE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_metering_awb_stats_mode_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x768);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x768, (((HI_U32) (data & 0x1)) << 0) | (curr & 0xfffffffe));
}
static __inline HI_U8 hi_isp_metering_awb_stats_mode_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x768) & 0x1) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_white_level_awb
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Upper limit of valid data for AWB
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_WHITE_LEVEL_AWB_DEFAULT (0x3FF)
#define HI_ISP_METERING_WHITE_LEVEL_AWB_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_isp_metering_white_level_awb_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x740);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x740, (((HI_U32) (data & 0x3ff)) << 0) | (curr & 0xfffffc00));
}
static __inline HI_U16 hi_isp_metering_white_level_awb_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x740) & 0x3ff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_black_level_awb
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Lower limit of valid data for AWB
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_BLACK_LEVEL_AWB_DEFAULT (0x000)
#define HI_ISP_METERING_BLACK_LEVEL_AWB_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_isp_metering_black_level_awb_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x744);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x744, (((HI_U32) (data & 0x3ff)) << 0) | (curr & 0xfffffc00));
}
static __inline HI_U16 hi_isp_metering_black_level_awb_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x744) & 0x3ff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Cr Ref Max AWB
// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_cr_ref_max_awb
// ------------------------------------------------------------------------------ //
// Maximum value of R/G for white region
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_CR_REF_MAX_AWB_DEFAULT (0x1FF)
#define HI_ISP_METERING_CR_REF_MAX_AWB_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_metering_cr_ref_max_awb_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x748);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x748, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_metering_cr_ref_max_awb_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x748) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Cr Ref Min AWB
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Minimum value of R/G for white region
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_CR_REF_MIN_AWB_DEFAULT (0x040)
#define HI_ISP_METERING_CR_REF_MIN_AWB_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_metering_cr_ref_min_awb_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x74c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x74c, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_metering_cr_ref_min_awb_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x74c) & 0xfff) >> 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_cb_ref_max_awb
// ------------------------------------------------------------------------------ //
// Maximum value of B/G for white region
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_CB_REF_MAX_AWB_DEFAULT (0x1FF)
#define HI_ISP_METERING_CB_REF_MAX_AWB_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_metering_cb_ref_max_awb_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x750);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x750, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_metering_cb_ref_max_awb_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x750) & 0xfff) >> 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_cb_ref_min_awb
// ------------------------------------------------------------------------------ //
// Minimum value of B/G for white region
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_CB_REF_MIN_AWB_DEFAULT (0x040)
#define HI_ISP_METERING_CB_REF_MIN_AWB_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_metering_cb_ref_min_awb_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x754);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x754, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_metering_cb_ref_min_awb_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x754) & 0xfff) >> 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_awb_rg
// ------------------------------------------------------------------------------ //
// AWB statistics R/G color ratio output
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AWB_RG_DEFAULT (0x0)
#define HI_ISP_METERING_AWB_RG_DATASIZE (12)

// args: data (12-bit)
static __inline HI_U16 hi_isp_metering_awb_rg_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x758) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: AWB BG
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// AWB statistics B/G color ratio output
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AWB_BG_DEFAULT (0x0)
#define HI_ISP_METERING_AWB_BG_DATASIZE (12)

// args: data (12-bit)
static __inline HI_U16 hi_isp_metering_awb_bg_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x75c) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: AWB SUM
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// AWB output population.  Number of pixels used for AWB statistics
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AWB_SUM_DEFAULT (0x0)
#define HI_ISP_METERING_AWB_SUM_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_metering_awb_sum_read(ISP_DEV IspDev) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x760);
}
// ------------------------------------------------------------------------------ //
// Register: AWB Nodes Used Horiz
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Number of active zones horizontally for AWB stats
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AWB_NODES_USED_HORIZ_DEFAULT (32)
#define HI_ISP_METERING_AWB_NODES_USED_HORIZ_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_metering_awb_nodes_used_horiz_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x770);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x770, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_metering_awb_nodes_used_horiz_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x770) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: AWB Nodes Used Vert
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Number of active zones vertically for AWB stats
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AWB_NODES_USED_VERT_DEFAULT (32)
#define HI_ISP_METERING_AWB_NODES_USED_VERT_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_metering_awb_nodes_used_vert_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x770);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x770, (((HI_U32) (data & 0xff)) << 8) | (curr & 0xffff00ff));
}
static __inline HI_U8 hi_isp_metering_awb_nodes_used_vert_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x770) & 0xff00) >> 8);
}
// ------------------------------------------------------------------------------ //
// Register: Cr Ref High AWB
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Maximum value of R/G for white region
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_CR_REF_HIGH_AWB_DEFAULT (0xFFF)
#define HI_ISP_METERING_CR_REF_HIGH_AWB_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_metering_cr_ref_high_awb_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x780);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x780, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_metering_cr_ref_high_awb_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x780) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Cr Ref Low AWB
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Minimum value of R/G for white region
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_CR_REF_LOW_AWB_DEFAULT (0x000)
#define HI_ISP_METERING_CR_REF_LOW_AWB_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_metering_cr_ref_low_awb_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x784);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x784, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_metering_cr_ref_low_awb_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x784) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Cb Ref High AWB
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Maximum value of B/G for white region
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_CB_REF_HIGH_AWB_DEFAULT (0xFFF)
#define HI_ISP_METERING_CB_REF_HIGH_AWB_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_metering_cb_ref_high_awb_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x788);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x788, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_metering_cb_ref_high_awb_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x788) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Cb Ref Low AWB
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Minimum value of B/G for white region
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_CB_REF_LOW_AWB_DEFAULT (0x000)
#define HI_ISP_METERING_CB_REF_LOW_AWB_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_metering_cb_ref_low_awb_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x78c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x78c, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_metering_cb_ref_low_awb_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x78c) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: skip x
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Histogram decimation in horizontal direction: 0=every 2nd pixel; 1=every 3rd pixel; 2=every 4th pixel; 3=every 5th pixel; 4=every 8th pixel ; 5+=every 9th pixel
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_SKIP_X_DEFAULT (0)
#define HI_ISP_METERING_SKIP_X_DATASIZE (3)

// args: data (3-bit)
static __inline HI_VOID hi_isp_metering_skip_x_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7c0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7c0, (((HI_U32) (data & 0x7)) << 0) | (curr & 0xfffffff8));
}
static __inline HI_U8 hi_isp_metering_skip_x_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7c0) & 0x7) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: skip y
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Histogram decimation in vertical direction: 0=every pixel; 1=every 2nd pixel; 2=every 3rd pixel; 3=every 4th pixel; 4=every 5th pixel; 5=every 8th pixel ; 6+=every 9th pixel
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_SKIP_Y_DEFAULT (0)
#define HI_ISP_METERING_SKIP_Y_DATASIZE (3)

// args: data (3-bit)
static __inline HI_VOID hi_isp_metering_skip_y_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7c0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7c0, (((HI_U32) (data & 0x7)) << 4) | (curr & 0xffffff8f));
}
static __inline HI_U8 hi_isp_metering_skip_y_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7c0) & 0x70) >> 4);
}
// ------------------------------------------------------------------------------ //
// Register: offset x
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 0= start from the first column;  1=start from second column
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_OFFSET_X_DEFAULT (0)
#define HI_ISP_METERING_OFFSET_X_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_metering_offset_x_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7c0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7c0, (((HI_U32) (data & 0x1)) << 3) | (curr & 0xfffffff7));
}
static __inline HI_U8 hi_isp_metering_offset_x_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7c0) & 0x8) >> 3);
}
// ------------------------------------------------------------------------------ //
// Register: offset y
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 0= start from the first row; 1= start from second row
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_OFFSET_Y_DEFAULT (0)
#define HI_ISP_METERING_OFFSET_Y_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_metering_offset_y_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7c0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7c0, (((HI_U32) (data & 0x1)) << 7) | (curr & 0xffffff7f));
}
static __inline HI_U8 hi_isp_metering_offset_y_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7c0) & 0x80) >> 7);
}
// ------------------------------------------------------------------------------ //
// Register: Plane mode
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Plane separation mode
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_PLANE_MODE_DEFAULT (0)
#define HI_ISP_METERING_PLANE_MODE_DATASIZE (3)
#define HI_ISP_METERING_PLANE_MODE_COLLECT_ALL_THE_PLANES_IN_ONE_HISTOGRAM (0)
#define HI_ISP_METERING_PLANE_MODE_COLLECT_4_BAYER_PLANES_INTO_4_SEPARATE_BANKS (1)
#define HI_ISP_METERING_PLANE_MODE_RESERVED_2 (2)
#define HI_ISP_METERING_PLANE_MODE_RESERVED_3 (3)
#define HI_ISP_METERING_PLANE_MODE_COLLECT_ODD__X_ODD__Y_PLANE_TO_BANK_0_REST_TO_BANK_1 (4)
#define HI_ISP_METERING_PLANE_MODE_COLLECT_EVEN_X_ODD__Y_PLANE_TO_BANK_0_REST_TO_BANK_1 (5)
#define HI_ISP_METERING_PLANE_MODE_COLLECT_ODD__X_EVEN_Y_PLANE_TO_BANK_0_REST_TO_BANK_1 (6)
#define HI_ISP_METERING_PLANE_MODE_COLLECT_EVEN_X_EVEN_Y_PLANE_TO_BANK_0_REST_TO_BANK_1 (7)

// args: data (3-bit)
static __inline HI_VOID hi_isp_metering_plane_mode_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7c0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7c0, (((HI_U32) (data & 0x7)) << 8) | (curr & 0xfffff8ff));
}
static __inline HI_U8 hi_isp_metering_plane_mode_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7c0) & 0x700) >> 8);
}
// ------------------------------------------------------------------------------ //
// Register: scale bottom
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// scale of bottom half of the range: 0=1x ,1=2x, 2=4x, 4=8x, 4=16x
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_SCALE_BOTTOM_DEFAULT (0)
#define HI_ISP_METERING_SCALE_BOTTOM_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_isp_metering_scale_bottom_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7c4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7c4, (((HI_U32) (data & 0xf)) << 0) | (curr & 0xfffffff0));
}
static __inline HI_U8 hi_isp_metering_scale_bottom_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7c4) & 0xf) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: scale top
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// scale of top half of the range: 0=1x ,1=2x, 2=4x, 4=8x, 4=16x
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_SCALE_TOP_DEFAULT (0)
#define HI_ISP_METERING_SCALE_TOP_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_isp_metering_scale_top_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7c4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7c4, (((HI_U32) (data & 0xf)) << 4) | (curr & 0xffffff0f));
}
static __inline HI_U8 hi_isp_metering_scale_top_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7c4) & 0xf0) >> 4);
}
// ------------------------------------------------------------------------------ //
// Register: Total Pixels
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Total number of pixels processed (skip x and skip y are taken into account)
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_TOTAL_PIXELS_DEFAULT (0)
#define HI_ISP_METERING_TOTAL_PIXELS_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_metering_total_pixels_read(ISP_DEV IspDev) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7c8);
}
// ------------------------------------------------------------------------------ //
// Register: Counted Pixels
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Number of pixels accumulated (with nonzero weight)
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_COUNTED_PIXELS_DEFAULT (0)
#define HI_ISP_METERING_COUNTED_PIXELS_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_metering_counted_pixels_read(ISP_DEV IspDev) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7cc);
}
// ------------------------------------------------------------------------------ //
// LUT: Plane Total
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Total pixels processed for each plane
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_PLANE_TOTAL_LUT_NODES (4)
#define HI_ISP_METERING_PLANE_TOTAL_LUT_ADDRBITS (2)
#define HI_ISP_METERING_PLANE_TOTAL_LUT_DATASIZE (27)

// args: index (0-3), data (27-bit)
static __inline HI_VOID hi_isp_metering_plane_total_lut_write(ISP_DEV IspDev, HI_U8 index,HI_U32 data) {
    HI_U32 addr = 0x7a0 + (index << 2);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr, data);
}
// ------------------------------------------------------------------------------ //
// LUT: Plane Counted
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Total pixels accumulated for each plane
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_PLANE_COUNTED_LUT_NODES (4)
#define HI_ISP_METERING_PLANE_COUNTED_LUT_ADDRBITS (2)
#define HI_ISP_METERING_PLANE_COUNTED_LUT_DATASIZE (27)

// args: index (0-3), data (27-bit)
static __inline HI_VOID hi_isp_metering_plane_counted_lut_write(ISP_DEV IspDev, HI_U8 index,HI_U32 data) {
    HI_U32 addr = 0x7b0 + (index << 2);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr, data);
}
// ------------------------------------------------------------------------------ //
// Register: skip x PE
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Per Exposure histogram decimation in horizontal direction: 0=every 2nd pixel; 1=every 3rd pixel; 2=every 4th pixel; 3=every 5th pixel; 4=every 8th pixel ; 5+=every 9th pixel
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_SKIP_X_PE_DEFAULT (0)
#define HI_ISP_METERING_SKIP_X_PE_DATASIZE (3)

// args: data (3-bit)
static __inline HI_VOID hi_isp_metering_skip_x_pe_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7d0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7d0, (((HI_U32) (data & 0x7)) << 0) | (curr & 0xfffffff8));
}
static __inline HI_U8 hi_isp_metering_skip_x_pe_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7d0) & 0x7) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: skip y PE
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Per Exposure histogram decimation in vertical direction: 0=every pixel; 1=every 2nd pixel; 2=every 3rd pixel; 3=every 4th pixel; 4=every 5th pixel; 5=every 8th pixel ; 6+=every 9th pixel
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_SKIP_Y_PE_DEFAULT (0)
#define HI_ISP_METERING_SKIP_Y_PE_DATASIZE (3)

// args: data (3-bit)
static __inline HI_VOID hi_isp_metering_skip_y_pe_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7d0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7d0, (((HI_U32) (data & 0x7)) << 4) | (curr & 0xffffff8f));
}
static __inline HI_U8 hi_isp_metering_skip_y_pe_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7d0) & 0x70) >> 4);
}
// ------------------------------------------------------------------------------ //
// Register: offset x PE
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 0= start from the first column;  1=start from second column
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_OFFSET_X_PE_DEFAULT (0)
#define HI_ISP_METERING_OFFSET_X_PE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_metering_offset_x_pe_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7d0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7d0, (((HI_U32) (data & 0x1)) << 3) | (curr & 0xfffffff7));
}
static __inline HI_U8 hi_isp_metering_offset_x_pe_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7d0) & 0x8) >> 3);
}
// ------------------------------------------------------------------------------ //
// Register: offset y PE
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// 0= start from the first row; 1= start from second row
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_OFFSET_Y_PE_DEFAULT (0)
#define HI_ISP_METERING_OFFSET_Y_PE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_metering_offset_y_pe_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7d0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7d0, (((HI_U32) (data & 0x1)) << 7) | (curr & 0xffffff7f));
}
static __inline HI_U8 hi_isp_metering_offset_y_pe_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7d0) & 0x80) >> 7);
}
// ------------------------------------------------------------------------------ //
// Register: Plane mode PE
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Plane separation mode
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_PLANE_MODE_PE_DEFAULT (0)
#define HI_ISP_METERING_PLANE_MODE_PE_DATASIZE (3)
#define HI_ISP_METERING_PLANE_MODE_PE_COLLECT_ALL_THE_PLANES_IN_ONE_HISTOGRAM (0)
#define HI_ISP_METERING_PLANE_MODE_PE_COLLECT_4_BAYER_PLANES_INTO_4_SEPARATE_BANKS (1)
#define HI_ISP_METERING_PLANE_MODE_PE_RESERVED_2 (2)
#define HI_ISP_METERING_PLANE_MODE_PE_RESERVED_3 (3)
#define HI_ISP_METERING_PLANE_MODE_PE_COLLECT_ODD__X_ODD__Y_PLANE_TO_BANK_0_REST_TO_BANK_1 (4)
#define HI_ISP_METERING_PLANE_MODE_PE_COLLECT_EVEN_X_ODD__Y_PLANE_TO_BANK_0_REST_TO_BANK_1 (5)
#define HI_ISP_METERING_PLANE_MODE_PE_COLLECT_ODD__X_EVEN_Y_PLANE_TO_BANK_0_REST_TO_BANK_1 (6)
#define HI_ISP_METERING_PLANE_MODE_PE_COLLECT_EVEN_X_EVEN_Y_PLANE_TO_BANK_0_REST_TO_BANK_1 (7)

// args: data (3-bit)
static __inline HI_VOID hi_isp_metering_plane_mode_pe_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7d0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7d0, (((HI_U32) (data & 0x7)) << 8) | (curr & 0xfffff8ff));
}
static __inline HI_U8 hi_isp_metering_plane_mode_pe_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7d0) & 0x700) >> 8);
}
// ------------------------------------------------------------------------------ //
// Register: AEXP Nodes Used Horiz PE
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Number of active zones horizontally for AE stats collection
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AEXP_NODES_USED_HORIZ_PE_DEFAULT (17)
#define HI_ISP_METERING_AEXP_NODES_USED_HORIZ_PE_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_metering_aexp_nodes_used_horiz_pe_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7d0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7d0, (((HI_U32) (data & 0xff)) << 16) | (curr & 0xff00ffff));
}
static __inline HI_U8 hi_isp_metering_aexp_nodes_used_horiz_pe_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7d0) & 0xff0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: AEXP Nodes Used Vert PE
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Number of active zones vertically for AE stats collection
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AEXP_NODES_USED_VERT_PE_DEFAULT (15)
#define HI_ISP_METERING_AEXP_NODES_USED_VERT_PE_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_metering_aexp_nodes_used_vert_pe_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7d0);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7d0, (((HI_U32) (data & 0xff)) << 24) | (curr & 0xffffff));
}
static __inline HI_U8 hi_isp_metering_aexp_nodes_used_vert_pe_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7d0) & 0xff000000) >> 24);
}
// ------------------------------------------------------------------------------ //
// Register: scale bottom PE
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// scale of bottom half of the range: 0=1x ,1=2x, 2=4x, 4=8x, 4=16x
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_SCALE_BOTTOM_PE_DEFAULT (0)
#define HI_ISP_METERING_SCALE_BOTTOM_PE_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_isp_metering_scale_bottom_pe_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7d4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7d4, (((HI_U32) (data & 0xf)) << 0) | (curr & 0xfffffff0));
}
static __inline HI_U8 hi_isp_metering_scale_bottom_pe_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7d4) & 0xf) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: scale top PE
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// scale of top half of the range: 0=1x ,1=2x, 2=4x, 4=8x, 4=16x
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_SCALE_TOP_PE_DEFAULT (0)
#define HI_ISP_METERING_SCALE_TOP_PE_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_isp_metering_scale_top_pe_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7d4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7d4, (((HI_U32) (data & 0xf)) << 4) | (curr & 0xffffff0f));
}
static __inline HI_U8 hi_isp_metering_scale_top_pe_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7d4) & 0xf0) >> 4);
}
// ------------------------------------------------------------------------------ //
// Register: Total Pixels PE
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Total number of pixels processed (skip x and skip y are taken into account)
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_TOTAL_PIXELS_PE_DEFAULT (0)
#define HI_ISP_METERING_TOTAL_PIXELS_PE_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_metering_total_pixels_pe_read(ISP_DEV IspDev) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7d8);
}
// ------------------------------------------------------------------------------ //
// Register: Counted Pixels PE
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Number of pixels accumulated (with nonzero weight)
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_COUNTED_PIXELS_PE_DEFAULT (0)
#define HI_ISP_METERING_COUNTED_PIXELS_PE_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_metering_counted_pixels_pe_read(ISP_DEV IspDev) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x7dc);
}
// ------------------------------------------------------------------------------ //
// LUT: Plane Total PE
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Total pixels processed for each plane
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_PLANE_TOTAL_PE_LUT_NODES (4)
#define HI_ISP_METERING_PLANE_TOTAL_PE_LUT_ADDRBITS (2)
#define HI_ISP_METERING_PLANE_TOTAL_PE_LUT_DATASIZE (27)

// args: index (0-3), data (27-bit)
static __inline HI_VOID hi_isp_metering_plane_total_pe_lut_write(ISP_DEV IspDev, HI_U8 index,HI_U32 data) {
    HI_U32 addr = 0x7e0 + (index << 2);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr, data);
}
// ------------------------------------------------------------------------------ //
// LUT: Plane Counted PE
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Total pixels accumulated for each plane
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_PLANE_COUNTED_PE_LUT_NODES (4)
#define HI_ISP_METERING_PLANE_COUNTED_PE_LUT_ADDRBITS (2)
#define HI_ISP_METERING_PLANE_COUNTED_PE_LUT_DATASIZE (27)

// args: index (0-3), data (27-bit)
static __inline HI_VOID hi_isp_metering_plane_counted_pe_lut_write(ISP_DEV IspDev, HI_U8 index,HI_U32 data) {
    HI_U32 addr = 0x7f0 + (index << 2);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr, data);
}
// ------------------------------------------------------------------------------ //
// Register: Sum R
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Normalized sum of Red pixels
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_SUM_R_DEFAULT (0x0)
#define HI_ISP_METERING_SUM_R_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_isp_metering_sum_r_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x800) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Sum RG
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Normalized sum of Green (red row) pixels
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_SUM_RG_DEFAULT (0x0)
#define HI_ISP_METERING_SUM_RG_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_isp_metering_sum_rg_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x804) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Sum BG
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Normalized sum of Green (blue row) pixels
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_SUM_BG_DEFAULT (0x0)
#define HI_ISP_METERING_SUM_BG_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_isp_metering_sum_bg_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x808) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Sum B
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Normalized sum of Blue pixels
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_SUM_B_DEFAULT (0x0)
#define HI_ISP_METERING_SUM_B_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_isp_metering_sum_b_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x80c) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: AE Sum Nodes Used Horiz
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Number of active zones horizontally
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AE_SUM_NODES_USED_HORIZ_DEFAULT (17)
#define HI_ISP_METERING_AE_SUM_NODES_USED_HORIZ_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_metering_ae_sum_nodes_used_horiz_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x810);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x810, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_metering_ae_sum_nodes_used_horiz_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x810) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: AE Sum Nodes Used Vert
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Number of active zones vertically
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AE_SUM_NODES_USED_VERT_DEFAULT (15)
#define HI_ISP_METERING_AE_SUM_NODES_USED_VERT_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_metering_ae_sum_nodes_used_vert_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x810);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x810, (((HI_U32) (data & 0xff)) << 8) | (curr & 0xffff00ff));
}
static __inline HI_U8 hi_isp_metering_ae_sum_nodes_used_vert_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x810) & 0xff00) >> 8);
}
// ------------------------------------------------------------------------------ //
// Register: Avg R
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Normalized sum of Red component
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AVG_R_DEFAULT (0x0)
#define HI_ISP_METERING_AVG_R_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_isp_metering_avg_r_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x820) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Avg G
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Normalized sum of Green component
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AVG_G_DEFAULT (0x0)
#define HI_ISP_METERING_AVG_G_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_isp_metering_avg_g_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x824) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Avg B
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Normalized sum of Blue component
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AVG_B_DEFAULT (0x0)
#define HI_ISP_METERING_AVG_B_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_isp_metering_avg_b_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x828) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Count All
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Normalized count of pixels which were averaged in Avg R, Avg G and Avg B
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_COUNT_ALL_DEFAULT (0x0)
#define HI_ISP_METERING_COUNT_ALL_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_isp_metering_count_all_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x82c) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Count Min
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Normalized count of pixels below min threshold
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_COUNT_MIN_DEFAULT (0x0)
#define HI_ISP_METERING_COUNT_MIN_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_isp_metering_count_min_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x830) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Count Max
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Normalized count of pixels above max threshold
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_COUNT_MAX_DEFAULT (0x0)
#define HI_ISP_METERING_COUNT_MAX_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_isp_metering_count_max_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x834) & 0xffff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: AWB Sum Nodes Used Horiz
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Number of active zones horizontally
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AWB_SUM_NODES_USED_HORIZ_DEFAULT (32)
#define HI_ISP_METERING_AWB_SUM_NODES_USED_HORIZ_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_metering_awb_sum_nodes_used_horiz_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x838);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x838, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U8 hi_isp_metering_awb_sum_nodes_used_horiz_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x838) & 0xff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: AWB Sum Nodes Used Vert
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Number of active zones vertically
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AWB_SUM_NODES_USED_VERT_DEFAULT (32)
#define HI_ISP_METERING_AWB_SUM_NODES_USED_VERT_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_metering_awb_sum_nodes_used_vert_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x838);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x838, (((HI_U32) (data & 0xff)) << 8) | (curr & 0xffff00ff));
}
static __inline HI_U8 hi_isp_metering_awb_sum_nodes_used_vert_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x838) & 0xff00) >> 8);
}
// ------------------------------------------------------------------------------ //
// Register: Min threshold
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Minimal threshold for AWB sums
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_MIN_THRESHOLD_DEFAULT (0x10)
#define HI_ISP_METERING_MIN_THRESHOLD_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_metering_min_threshold_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x83c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x83c, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_metering_min_threshold_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x83c) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Max threshold
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Maximal threshold for AWB sums
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_MAX_THRESHOLD_DEFAULT (0xff0)
#define HI_ISP_METERING_MAX_THRESHOLD_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_metering_max_threshold_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x840);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x840, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_metering_max_threshold_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x840) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Min clip
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Clip pixels below minimal threshold for AWB sums
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_MIN_CLIP_DEFAULT (0)
#define HI_ISP_METERING_MIN_CLIP_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_metering_min_clip_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x844);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x844, (((HI_U32) (data & 0x1)) << 0) | (curr & 0xfffffffe));
}
static __inline HI_U8 hi_isp_metering_min_clip_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x844) & 0x1) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Max clip
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Clip pixels ablove maximal threshold for AWB sums
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_MAX_CLIP_DEFAULT (0)
#define HI_ISP_METERING_MAX_CLIP_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_metering_max_clip_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x844);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x844, (((HI_U32) (data & 0x1)) << 1) | (curr & 0xfffffffd));
}
static __inline HI_U8 hi_isp_metering_max_clip_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x844) & 0x2) >> 1);
}
// ------------------------------------------------------------------------------ //
// Register: Cr Ref Max AWB Sum
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Maximum value of R/G for white region
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_CR_REF_MAX_AWB_SUM_DEFAULT (0x1FF)
#define HI_ISP_METERING_CR_REF_MAX_AWB_SUM_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_metering_cr_ref_max_awb_sum_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x848);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x848, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_metering_cr_ref_max_awb_sum_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x848) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Cr Ref Min AWB Sum
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Minimum value of R/G for white region
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_CR_REF_MIN_AWB_SUM_DEFAULT (0x040)
#define HI_ISP_METERING_CR_REF_MIN_AWB_SUM_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_metering_cr_ref_min_awb_sum_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x848);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x848, (((HI_U32) (data & 0xfff)) << 16) | (curr & 0xf000ffff));
}
static __inline HI_U16 hi_isp_metering_cr_ref_min_awb_sum_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x848) & 0xfff0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: Cb Ref Max AWB Sum
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Maximum value of B/G for white region
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_CB_REF_MAX_AWB_SUM_DEFAULT (0x1FF)
#define HI_ISP_METERING_CB_REF_MAX_AWB_SUM_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_metering_cb_ref_max_awb_sum_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x84c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x84c, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_metering_cb_ref_max_awb_sum_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x84c) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Cb Ref Min AWB Sum
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Minimum value of B/G for white region
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_CB_REF_MIN_AWB_SUM_DEFAULT (0x040)
#define HI_ISP_METERING_CB_REF_MIN_AWB_SUM_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_metering_cb_ref_min_awb_sum_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x84c);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x84c, (((HI_U32) (data & 0xfff)) << 16) | (curr & 0xf000ffff));
}
static __inline HI_U16 hi_isp_metering_cb_ref_min_awb_sum_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x84c) & 0xfff0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: Cr Ref High AWB Sum
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Maximum value of R/G for white region
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_CR_REF_HIGH_AWB_SUM_DEFAULT (0xFFF)
#define HI_ISP_METERING_CR_REF_HIGH_AWB_SUM_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_metering_cr_ref_high_awb_sum_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x850);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x850, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_metering_cr_ref_high_awb_sum_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x850) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Cr Ref Low AWB Sum
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Minimum value of R/G for white region
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_CR_REF_LOW_AWB_SUM_DEFAULT (0x000)
#define HI_ISP_METERING_CR_REF_LOW_AWB_SUM_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_metering_cr_ref_low_awb_sum_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x850);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x850, (((HI_U32) (data & 0xfff)) << 16) | (curr & 0xf000ffff));
}
static __inline HI_U16 hi_isp_metering_cr_ref_low_awb_sum_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x850) & 0xfff0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Register: Cb Ref High AWB Sum
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Maximum value of B/G for white region
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_CB_REF_HIGH_AWB_SUM_DEFAULT (0xFFF)
#define HI_ISP_METERING_CB_REF_HIGH_AWB_SUM_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_metering_cb_ref_high_awb_sum_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x854);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x854, (((HI_U32) (data & 0xfff)) << 0) | (curr & 0xfffff000));
}
static __inline HI_U16 hi_isp_metering_cb_ref_high_awb_sum_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x854) & 0xfff) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: Cb Ref Low AWB Sum
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Minimum value of B/G for white region
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_CB_REF_LOW_AWB_SUM_DEFAULT (0x000)
#define HI_ISP_METERING_CB_REF_LOW_AWB_SUM_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_metering_cb_ref_low_awb_sum_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x854);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x854, (((HI_U32) (data & 0xfff)) << 16) | (curr & 0xf000ffff));
}
static __inline HI_U16 hi_isp_metering_cb_ref_low_awb_sum_read(ISP_DEV IspDev) {
    return (HI_U16)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x854) & 0xfff0000) >> 16);
}
// ------------------------------------------------------------------------------ //
// Group: Frame Stats
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_frame_stats_stats_reset
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STATS_STATS_RESET_DEFAULT (0)
#define HI_ISP_FRAME_STATS_STATS_RESET_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_frame_stats_stats_reset_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x900);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x900, (((HI_U32) (data & 0x1)) << 0) | (curr & 0xfffffffe));
}
static __inline HI_U8 hi_isp_frame_stats_stats_reset_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x900) & 0x1) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: stats hold
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STATS_STATS_HOLD_DEFAULT (0)
#define HI_ISP_FRAME_STATS_STATS_HOLD_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_frame_stats_stats_hold_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x904);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x904, (((HI_U32) (data & 0x1)) << 0) | (curr & 0xfffffffe));
}
static __inline HI_U8 hi_isp_frame_stats_stats_hold_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x904) & 0x1) >> 0);
}
// ------------------------------------------------------------------------------ //
// Register: active width min
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STATS_ACTIVE_WIDTH_MIN_DEFAULT (0x0)
#define HI_ISP_FRAME_STATS_ACTIVE_WIDTH_MIN_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_frame_stats_active_width_min_read(ISP_DEV IspDev) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x910);
}
// ------------------------------------------------------------------------------ //
// Register: active width max
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STATS_ACTIVE_WIDTH_MAX_DEFAULT (0x0)
#define HI_ISP_FRAME_STATS_ACTIVE_WIDTH_MAX_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_frame_stats_active_width_max_read(ISP_DEV IspDev) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x914);
}
// ------------------------------------------------------------------------------ //
// Register: active width sum
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STATS_ACTIVE_WIDTH_SUM_DEFAULT (0x0)
#define HI_ISP_FRAME_STATS_ACTIVE_WIDTH_SUM_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_frame_stats_active_width_sum_read(ISP_DEV IspDev) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x918);
}
// ------------------------------------------------------------------------------ //
// Register: active width num
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STATS_ACTIVE_WIDTH_NUM_DEFAULT (0x0)
#define HI_ISP_FRAME_STATS_ACTIVE_WIDTH_NUM_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_frame_stats_active_width_num_read(ISP_DEV IspDev) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x91c);
}
// ------------------------------------------------------------------------------ //
// Register: active height min
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STATS_ACTIVE_HEIGHT_MIN_DEFAULT (0x0)
#define HI_ISP_FRAME_STATS_ACTIVE_HEIGHT_MIN_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_frame_stats_active_height_min_read(ISP_DEV IspDev) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x920);
}
// ------------------------------------------------------------------------------ //
// Register: active height max
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STATS_ACTIVE_HEIGHT_MAX_DEFAULT (0x0)
#define HI_ISP_FRAME_STATS_ACTIVE_HEIGHT_MAX_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_frame_stats_active_height_max_read(ISP_DEV IspDev) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x924);
}
// ------------------------------------------------------------------------------ //
// Register: active height sum
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STATS_ACTIVE_HEIGHT_SUM_DEFAULT (0x0)
#define HI_ISP_FRAME_STATS_ACTIVE_HEIGHT_SUM_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_frame_stats_active_height_sum_read(ISP_DEV IspDev) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x928);
}
// ------------------------------------------------------------------------------ //
// Register: active height num
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STATS_ACTIVE_HEIGHT_NUM_DEFAULT (0x0)
#define HI_ISP_FRAME_STATS_ACTIVE_HEIGHT_NUM_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_frame_stats_active_height_num_read(ISP_DEV IspDev) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x92c);
}
// ------------------------------------------------------------------------------ //
// Register: hblank min
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STATS_HBLANK_MIN_DEFAULT (0x0)
#define HI_ISP_FRAME_STATS_HBLANK_MIN_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_frame_stats_hblank_min_read(ISP_DEV IspDev) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x930);
}
// ------------------------------------------------------------------------------ //
// Register: hblank max
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STATS_HBLANK_MAX_DEFAULT (0x0)
#define HI_ISP_FRAME_STATS_HBLANK_MAX_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_frame_stats_hblank_max_read(ISP_DEV IspDev) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x934);
}
// ------------------------------------------------------------------------------ //
// Register: hblank sum
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STATS_HBLANK_SUM_DEFAULT (0x0)
#define HI_ISP_FRAME_STATS_HBLANK_SUM_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_frame_stats_hblank_sum_read(ISP_DEV IspDev) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x938);
}
// ------------------------------------------------------------------------------ //
// Register: hblank num
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STATS_HBLANK_NUM_DEFAULT (0x0)
#define HI_ISP_FRAME_STATS_HBLANK_NUM_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_frame_stats_hblank_num_read(ISP_DEV IspDev) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x93c);
}
// ------------------------------------------------------------------------------ //
// Register: vblank min
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STATS_VBLANK_MIN_DEFAULT (0x0)
#define HI_ISP_FRAME_STATS_VBLANK_MIN_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_frame_stats_vblank_min_read(ISP_DEV IspDev) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x940);
}
// ------------------------------------------------------------------------------ //
// Register: vblank max
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STATS_VBLANK_MAX_DEFAULT (0x0)
#define HI_ISP_FRAME_STATS_VBLANK_MAX_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_frame_stats_vblank_max_read(ISP_DEV IspDev) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x944);
}
// ------------------------------------------------------------------------------ //
// Register: vblank sum
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STATS_VBLANK_SUM_DEFAULT (0x0)
#define HI_ISP_FRAME_STATS_VBLANK_SUM_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_frame_stats_vblank_sum_read(ISP_DEV IspDev) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x948);
}
// ------------------------------------------------------------------------------ //
// Register: vblank num
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STATS_VBLANK_NUM_DEFAULT (0x0)
#define HI_ISP_FRAME_STATS_VBLANK_NUM_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_frame_stats_vblank_num_read(ISP_DEV IspDev) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x94c);
}

// ------------------------------------------------------------------------------ //
// Group: Noise Profile WDR4
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_noise_profile_long_noise_offset
// ------------------------------------------------------------------------------ //
// Noise profile controls for long exposure
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_LONG_NOISE_OFFSET_DEFAULT (0x40)
#define HI_ISP_NOISE_PROFILE_LONG_NOISE_OFFSET_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_noise_profile_long_noise_offset_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1004);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1004, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U16 hi_isp_noise_profile_long_noise_offset_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1004) & 0xff) >> 0);
}

// ------------------------------------------------------------------------------ //
// Group: Noise Profile WDR3
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_noise_profile_med1_noise_offset
// ------------------------------------------------------------------------------ //
// Noise profile controls for med1 exposure
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_MED1_NOISE_OFFSET_DEFAULT (0x40)
#define HI_ISP_NOISE_PROFILE_MED1_NOISE_OFFSET_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_noise_profile_med1_noise_offset_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1014);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1014, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U16 hi_isp_noise_profile_med1_noise_offset_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1014) & 0xff) >> 0);
}

// ------------------------------------------------------------------------------ //
// Group: Noise Profile WDR2
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_noise_profile_med2_noise_offset
// ------------------------------------------------------------------------------ //
// Noise profile controls for med2 exposure
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_MED2_NOISE_OFFSET_DEFAULT (0x40)
#define HI_ISP_NOISE_PROFILE_MED2_NOISE_OFFSET_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_noise_profile_med2_noise_offset_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1024);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1024, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U16 hi_isp_noise_profile_med2_noise_offset_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1024) & 0xff) >> 0);
}

// ------------------------------------------------------------------------------ //
// Group: Noise Profile WDR1
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_noise_profile_short_noise_offset
// ------------------------------------------------------------------------------ //
// Noise profile controls for short exposure
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_SHORT_NOISE_OFFSET_DEFAULT (0x40)
#define HI_ISP_NOISE_PROFILE_SHORT_NOISE_OFFSET_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_noise_profile_short_noise_offset_write(ISP_DEV IspDev, HI_U16 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1034);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1034, (((HI_U32) (data & 0xff)) << 0) | (curr & 0xffffff00));
}
static __inline HI_U16 hi_isp_noise_profile_short_noise_offset_read(ISP_DEV IspDev) {
    return (HI_U8)((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1034) & 0xff) >> 0);
}

// ------------------------------------------------------------------------------ //
// Group: Drc LUT
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// LUT: Asymmetry
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Iridix target curve. 
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_LUT_ASYMMETRY_LUT_NODES (65)
#define HI_ISP_DRC_LUT_ASYMMETRY_LUT_ADDRBITS (7)
#define HI_ISP_DRC_LUT_ASYMMETRY_LUT_DATASIZE (20)

// args: index (0-64), data (20-bit)
static __inline HI_VOID hi_isp_drc_asymmetry_lut_write(ISP_DEV IspDev, HI_U8 index,HI_U32 data) {
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1400, index);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x1404, data);
}
// ------------------------------------------------------------------------------ //
// Group: Noise Profile Frame Stitch LUT 1
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Noise profile controls for Frame Stitch
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: Weight lut
// ------------------------------------------------------------------------------ //


// ------------------------------------------------------------------------------ //
// Noise profile LUT.  Calculated during calibration process.
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_FRAME_STITCH_1_LUT_WEIGHT_LUT_DEFAULT (0x0)
#define HI_ISP_NOISE_PROFILE_FRAME_STITCH_1_LUT_WEIGHT_LUT_DATASIZE (8)

// index (0-127), args: data (8-bit)
static __inline HI_VOID hi_isp_noise_profile_frame_stitch_1_lut_weight_lut_write(ISP_DEV IspDev, HI_U32 index,HI_U8 data) {
    HI_U32 addr = 0x1200 + (index & 0xFFFFFFFC);
    HI_U8 offset = (index & 3) << 3;
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr, ((HI_U32)data << offset) | (curr & ~(0xFF << offset)));
}
static __inline HI_U8 hi_isp_noise_profile_frame_stitch_1_lut_weight_lut_read(ISP_DEV IspDev, HI_U32 index) {
    HI_U32 addr = 0x1200 + (index & 0xFFFFFFFC);
    HI_U8 offset = (index & 3) << 3;
    return (HI_U8)(IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr) >> offset);
}

// ------------------------------------------------------------------------------ //
// Group: Noise Profile Frame Stitch LUT 2
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Noise profile controls for Frame Stitch
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: Weight lut
// ------------------------------------------------------------------------------ //


// ------------------------------------------------------------------------------ //
// Noise profile LUT.  Calculated during calibration process.
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_FRAME_STITCH_2_LUT_WEIGHT_LUT_DEFAULT (0x0)
#define HI_ISP_NOISE_PROFILE_FRAME_STITCH_2_LUT_WEIGHT_LUT_DATASIZE (8)

// index (0-127), args: data (8-bit)
static __inline HI_VOID hi_isp_noise_profile_frame_stitch_2_lut_weight_lut_write(ISP_DEV IspDev, HI_U32 index,HI_U8 data) {
    HI_U32 addr = 0x1280 + (index & 0xFFFFFFFC);
    HI_U8 offset = (index & 3) << 3;
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr, ((HI_U32)data << offset) | (curr & ~(0xFF << offset)));
}
static __inline HI_U8 hi_isp_noise_profile_frame_stitch_2_lut_weight_lut_read(ISP_DEV IspDev, HI_U32 index) {
    HI_U32 addr = 0x1280 + (index & 0xFFFFFFFC);
    HI_U8 offset = (index & 3) << 3;
    return (HI_U8)(IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr) >> offset);
}

// ------------------------------------------------------------------------------ //
// Group: Noise Profile Frame Stitch LUT 3
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Noise profile controls for Frame Stitch
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: Weight lut
// ------------------------------------------------------------------------------ //


// ------------------------------------------------------------------------------ //
// Noise profile LUT.  Calculated during calibration process.
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_FRAME_STITCH_3_LUT_WEIGHT_LUT_DEFAULT (0x0)
#define HI_ISP_NOISE_PROFILE_FRAME_STITCH_3_LUT_WEIGHT_LUT_DATASIZE (8)

// index (0-127), args: data (8-bit)
static __inline HI_VOID hi_isp_noise_profile_frame_stitch_3_lut_weight_lut_write(ISP_DEV IspDev, HI_U32 index,HI_U8 data) {
    HI_U32 addr = 0x1300 + (index & 0xFFFFFFFC);
    HI_U8 offset = (index & 3) << 3;
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr, ((HI_U32)data << offset) | (curr & ~(0xFF << offset)));
}
static __inline HI_U8 hi_isp_noise_profile_frame_stitch_3_lut_weight_lut_read(ISP_DEV IspDev, HI_U32 index) {
    HI_U32 addr = 0x1300 + (index & 0xFFFFFFFC);
    HI_U8 offset = (index & 3) << 3;
    return (HI_U8)(IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr) >> offset);
}

// ------------------------------------------------------------------------------ //
// Group: Noise Profile Frame Stitch LUT 4
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Noise profile controls for Frame Stitch
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: Weight lut
// ------------------------------------------------------------------------------ //


// ------------------------------------------------------------------------------ //
// Noise profile LUT.  Calculated during calibration process.
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_FRAME_STITCH_4_LUT_WEIGHT_LUT_DEFAULT (0x0)
#define HI_ISP_NOISE_PROFILE_FRAME_STITCH_4_LUT_WEIGHT_LUT_DATASIZE (8)

// index (0-127), args: data (8-bit)
static __inline HI_VOID hi_isp_noise_profile_frame_stitch_4_lut_weight_lut_write(ISP_DEV IspDev, HI_U32 index,HI_U8 data) {
    HI_U32 addr = 0x1380 + (index & 0xFFFFFFFC);
    HI_U8 offset = (index & 3) << 3;
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr, ((HI_U32)data << offset) | (curr & ~(0xFF << offset)));
}
static __inline HI_U8 hi_isp_noise_profile_frame_stitch_4_lut_weight_lut_read(ISP_DEV IspDev, HI_U32 index) {
    HI_U32 addr = 0x1380 + (index & 0xFFFFFFFC);
    HI_U8 offset = (index & 3) << 3;
    return (HI_U8)(IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr) >> offset);
}

// ------------------------------------------------------------------------------ //
// Group: Sinter Shading
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// LUT: rm_shading_lut
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Radial Sinter LUT.  See ISP guide for more details
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_SHADING_RM_SHADING_LUT_NODES (33)
#define HI_ISP_NR_SHADING_RM_SHADING_LUT_ADDRBITS (6)
#define HI_ISP_NR_SHADING_RM_SHADING_LUT_DATASIZE (8)

// args: index (0-32), data (8-bit)
static __inline HI_VOID hi_isp_nr_shading_rm_shading_lut_write(ISP_DEV IspDev, HI_U8 index,HI_U8 data) {
    HI_U32 addr = 0x1540 + (index & 0xFFFFFFFC);
    HI_U8 offset = (index & 3) << 3;
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr, ((HI_U32)data << offset) | (curr & ~(0xFF << offset)));
}
static __inline HI_U8 hi_isp_nr_shading_rm_shading_lut_read(ISP_DEV IspDev, HI_U8 index) {
    HI_U32 addr = 0x1540 + (index & 0xFFFFFFFC);
    HI_U8 offset = (index & 3) << 3;
    return (HI_U8)(IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr) >> offset);
}
// ------------------------------------------------------------------------------ //
// Group: Noise Profile RAW frontend LUT 1
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Noise profile controls for RAW frontend
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: Weight lut
// ------------------------------------------------------------------------------ //


// ------------------------------------------------------------------------------ //
// Noise profile LUT.  Calculated during calibration process.
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_1_LUT_WEIGHT_LUT_DEFAULT (0x0)
#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_1_LUT_WEIGHT_LUT_DATASIZE (8)

// index (0-127), args: data (8-bit)
static __inline HI_VOID hi_isp_noise_profile_raw_frontend_1_lut_weight_lut_write(ISP_DEV IspDev, HI_U32 index,HI_U8 data) {
    HI_U32 addr = 0x1580 + (index & 0xFFFFFFFC);
    HI_U8 offset = (index & 3) << 3;
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr, ((HI_U32)data << offset) | (curr & ~(0xFF << offset)));
}
static __inline HI_U8 hi_isp_noise_profile_raw_frontend_1_lut_weight_lut_read(ISP_DEV IspDev, HI_U32 index) {
    HI_U32 addr = 0x1580 + (index & 0xFFFFFFFC);
    HI_U8 offset = (index & 3) << 3;
    return (HI_U8)(IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr) >> offset);
}
// ------------------------------------------------------------------------------ //
// Group: Noise Profile RAW frontend LUT 2
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Noise profile controls for RAW frontend
// ------------------------------------------------------------------------------ //


// ------------------------------------------------------------------------------ //
// Register: Weight lut
// ------------------------------------------------------------------------------ //


// ------------------------------------------------------------------------------ //
// Noise profile LUT.  Calculated during calibration process.
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_2_LUT_WEIGHT_LUT_DEFAULT (0x0)
#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_2_LUT_WEIGHT_LUT_DATASIZE (8)

// index (0-127), args: data (8-bit)
static __inline HI_VOID hi_isp_noise_profile_raw_frontend_2_lut_weight_lut_write(ISP_DEV IspDev, HI_U32 index,HI_U8 data) {
    HI_U32 addr = 0x1600 + (index & 0xFFFFFFFC);
    HI_U8 offset = (index & 3) << 3;
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr, ((HI_U32)data << offset) | (curr & ~(0xFF << offset)));
}
static __inline HI_U8 hi_isp_noise_profile_raw_frontend_2_lut_weight_lut_read(ISP_DEV IspDev, HI_U32 index) {
    HI_U32 addr = 0x1600 + (index & 0xFFFFFFFC);
    HI_U8 offset = (index & 3) << 3;
    return (HI_U8)(IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr) >> offset);
}
// ------------------------------------------------------------------------------ //
// Group: Noise Profile RAW frontend LUT 3
// ------------------------------------------------------------------------------ //


// ------------------------------------------------------------------------------ //
// Noise profile controls for RAW frontend
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: Weight lut
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Noise profile LUT.  Calculated during calibration process.
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_3_LUT_WEIGHT_LUT_DEFAULT (0x0)
#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_3_LUT_WEIGHT_LUT_DATASIZE (8)

// index (0-127), args: data (8-bit)
static __inline HI_VOID hi_isp_noise_profile_raw_frontend_3_lut_weight_lut_write(ISP_DEV IspDev, HI_U32 index,HI_U8 data) {
    HI_U32 addr = 0x1680 + (index & 0xFFFFFFFC);
    HI_U8 offset = (index & 3) << 3;
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr, ((HI_U32)data << offset) | (curr & ~(0xFF << offset)));
}
static __inline HI_U8 hi_isp_noise_profile_raw_frontend_3_lut_weight_lut_read(ISP_DEV IspDev, HI_U32 index) {
    HI_U32 addr = 0x1680 + (index & 0xFFFFFFFC);
    HI_U8 offset = (index & 3) << 3;
    return (HI_U8)(IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr) >> offset);
}
// ------------------------------------------------------------------------------ //
// Group: Noise Profile RAW frontend LUT 4
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Noise profile controls for RAW frontend
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: Weight lut
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Noise profile LUT.  Calculated during calibration process.
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_4_LUT_WEIGHT_LUT_DEFAULT (0x0)
#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_4_LUT_WEIGHT_LUT_DATASIZE (8)

// index (0-127), args: data (8-bit)
static __inline HI_VOID hi_isp_noise_profile_raw_frontend_4_lut_weight_lut_write(ISP_DEV IspDev, HI_U32 index,HI_U8 data) {
    HI_U32 addr = 0x1700 + (index & 0xFFFFFFFC);
    HI_U8 offset = (index & 3) << 3;
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr, ((HI_U32)data << offset) | (curr & ~(0xFF << offset)));
}
static __inline HI_U8 hi_isp_noise_profile_raw_frontend_4_lut_weight_lut_read(ISP_DEV IspDev, HI_U32 index) {
    HI_U32 addr = 0x1700 + (index & 0xFFFFFFFC);
    HI_U8 offset = (index & 3) << 3;
    return (HI_U8)(IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr) >> offset);
}
// ------------------------------------------------------------------------------ //
// Group: Noise Profile LUT
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Noise profile controls for Sinter
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: Weight lut
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Noise profile LUT.  Calculated during calibration process.
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_WEIGHT_LUT_DEFAULT (0x0)
#define HI_ISP_NOISE_PROFILE_WEIGHT_LUT_DATASIZE (8)

// index (0-127), args: data (8-bit)
static __inline HI_VOID hi_isp_noise_profile_weight_lut_write(ISP_DEV IspDev, HI_U32 index,HI_U8 data) {
    HI_U32 addr = 0x1800 + (index & 0xFFFFFFFC);
    HI_U8 offset = (index & 3) << 3;
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr, ((HI_U32)data << offset) | (curr & ~(0xFF << offset)));
}
static __inline HI_U8 hi_isp_noise_profile_weight_lut_read(ISP_DEV IspDev, HI_U32 index) {
    HI_U32 addr = 0x1800 + (index & 0xFFFFFFFC);
    HI_U8 offset = (index & 3) << 3;
    return (HI_U8)(IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr) >> offset);
}
// ------------------------------------------------------------------------------ //
// Group: Noise Profile Demosaic LUT
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Bayer Demosaic lookup
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: Weight lut
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Noise profile LUT
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_WEIGHT_LUT_DEFAULT (0x0)
#define HI_ISP_DEMOSAIC_WEIGHT_LUT_DATASIZE (8)

// index (0-127), args: data (8-bit)
static __inline HI_VOID hi_isp_demosaic_weight_lut_write(ISP_DEV IspDev, HI_U32 index,HI_U8 data) {
    HI_U32 addr = 0x1880 + (index & 0xFFFFFFFC);
    HI_U8 offset = (index & 3) << 3;
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr, ((HI_U32)data << offset) | (curr & ~(0xFF << offset)));
}
static __inline HI_U8 hi_isp_demosaic_weight_lut_read(ISP_DEV IspDev, HI_U32 index) {
    HI_U32 addr = 0x1880 + (index & 0xFFFFFFFC);
    HI_U8 offset = (index & 3) << 3;
    return (HI_U8)(IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr) >> offset);
}
// ------------------------------------------------------------------------------ //
// Group: Zones
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Controls zone weighting for auto-exposure and auto-white-balance
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_zones_aexp_weight
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Sets zone weighting for auto exposure. Index is (row,col) where (0,0) is top-left zonea
// ------------------------------------------------------------------------------ //

#define HI_ISP_ZONES_AEXP_WEIGHT_DEFAULT (0xF)
#define HI_ISP_ZONES_AEXP_WEIGHT_DATASIZE (4)

// index1 (0-14), index2 (0-16), args: data (4-bit)
static __inline HI_VOID hi_isp_zones_aexp_weight_write(ISP_DEV IspDev, HI_U32 index1, HI_U32 index2,HI_U8 data) {  
#if 0
    HI_U32 addr;
    HI_U8 offset;
    HI_U32 curr;
    addr = 0x1900 + (index1 * 17 + index2);
    offset = (addr & 3) << 3;
    addr &= 0xFFFFFFFC;
    curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr, ((HI_U32)data << offset) | (curr & ~(0xFF << offset)));
#endif
    HI_U32 addr = 0x1900 + (((index1 * 17) + index2) << 0);
    addr = ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr;
    IOWR_8DIRECT(addr, data & 0x0F);
}
static __inline HI_U8 hi_isp_zones_aexp_weight_read(ISP_DEV IspDev, HI_U32 index1, HI_U32 index2) {
#if 0
    HI_U32 addr = 0x1900 + ((index1 * 17 + index2));
    HI_U8 offset = (addr & 3) << 3;
    addr &= 0xFFFFFFFC;
    return (HI_U8)(IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr) >> offset);
#endif
    HI_U32 addr = 0x1900 + (((index1 * 17) + index2) << 0);
    addr = ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr;
    return (HI_U8) IORD_8DIRECT(addr);
}

#define HI_ISP_ZONES_AEXP_WEIGHT_PE_DEFAULT (0xF)
#define HI_ISP_ZONES_AEXP_WEIGHT_PE_DATASIZE (4)

// index1 (0-16), index2 (0-14), args: data (4-bit)
static __inline HI_VOID hi_isp_zones_aexp_weight_pe_write(ISP_DEV IspDev, HI_U32 index1, HI_U32 index2,HI_U8 data) {
#if 0
    HI_U32 addr;
    HI_U8 offset;
    HI_U32 curr;
    addr = 0x1a00L + (index1 * 17 + index2);
    offset = (addr & 3) << 3;
    addr &= 0xFFFFFFFC;
    curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr, ((HI_U32)data << offset) | (curr & ~(0xFF << offset)));
#endif
    HI_U32 addr = 0x1a00L + (((index1 * 17) + index2) << 0);
    addr = ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr;
    IOWR_8DIRECT(addr, data & 0x0F);
}
static __inline HI_U8 hi_isp_zones_aexp_weight_pe_read(ISP_DEV IspDev, HI_U32 index1, HI_U32 index2) {
#if 0
    HI_U32 addr = 0x1a00L + ((index1 * 17 + index2));
    HI_U8 offset = (addr & 3) << 3;
    addr &= 0xFFFFFFFC;
    return (HI_U8)(IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr) >> offset);
#endif
    HI_U32 addr = 0x1a00L + (((index1 * 17) + index2) << 0);
    addr = ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr;
    return (HI_U8) IORD_8DIRECT(addr);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_zones_awb_weight
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Sets zone weighting for auto white balance. Index is (row,col) where (0,0) is top-left zone
// ------------------------------------------------------------------------------ //

#define HI_ISP_ZONES_AWB_WEIGHT_DEFAULT (0xF)
#define HI_ISP_ZONES_AWB_WEIGHT_DATASIZE (4)

// index1 (0-16), index2 (0-14), args: data (4-bit)
static __inline HI_VOID hi_isp_zones_awb_weight_write(ISP_DEV IspDev, HI_U32 index1, HI_U32 index2,HI_U8 data) {
    HI_U32 addr;
    HI_U8 offset;
    HI_U32 curr;
    addr = 0x1b00 + (index1 * 15 + index2);
    offset = (addr & 3) << 3;
    addr &= 0xFFFFFFFC;
    curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr, ((HI_U32)data << offset) | (curr & ~(0xFF << offset)));
}
static __inline HI_U8 hi_isp_zones_awb_weight_read(ISP_DEV IspDev, HI_U32 index1, HI_U32 index2) {
    HI_U32 addr = 0x1b00 + ((index1 * 15 + index2));
    HI_U8 offset = (addr & 3) << 3;
    addr &= 0xFFFFFFFC;
    return (HI_U8)(IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr) >> offset);
}
// ------------------------------------------------------------------------------ //


/*****************************************************
MODULE        -----DEHAZE-----
MODULE        -----DEHAZE-----
MODULE        -----DEHAZE-----
*****************************************************/
/*****************************************************
ISP_DEHAZE_CFG: 		ISP_DEHAZE_CFG DEHAZE enable
Offset:0x6200: 			31:1		reserved									 
						0			en
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_en_write(ISP_DEV IspDev, HI_U8 data){
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26200, data);
}

static __inline HI_U8 hi_isp_dehaze_en_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26200));
}

/*****************************************************
ISP_DEHAZE_PRE_UPDATE: 		ISP_DEHAZE_PRE_UPDATE DEHAZE static update
Offset:0x7010: 			31:1		reserved									 
						0			pre_update
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_pre_update_write(ISP_DEV IspDev, HI_U8 data){
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26210, data);
}

static __inline HI_U8 hi_isp_dehaze_pre_update_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26210));
}

/*****************************************************
ISP_DEHAZE_BLK_SIZE: 		ISP_DEHAZE_BLK_SIZE DEHAZE block size
Offset:0x7014: 			            31:01		reserved	
									24:16		block_sizeh   
									15:06         reserved
						               08:00		block_sizev    
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_block_sizeh_write(ISP_DEV IspDev, HI_U16 data){
	HI_U32 u32Current = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26214);
	IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26214, (u32Current & 0xfe00ffff) | ((data & 0x1ff) << 16));
}

static __inline HI_U16 hi_isp_dehaze_block_sizeh_read(ISP_DEV IspDev) {
	return ((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26214)&0x1ff0000) >> 16);
}

static __inline HI_VOID hi_isp_dehaze_block_sizev_write(ISP_DEV IspDev, HI_U16 data){
	HI_U32 u32Current = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26214);
	IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26214, (u32Current & 0xfffffe00) | (data & 0x1ff));
}

static __inline HI_U16 hi_isp_dehaze_block_sizev_read(ISP_DEV IspDev) {
	return (IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26214)&0x1ff);
}

/*****************************************************
ISP_DEHAZE_BLK_NUM: 		ISP_DEHAZE_BLK_NUM DEHAZE block num
Offset:0x7018: 			            31:11		reserved	
						       10:00		block_sum    dehaze block num,hnum*(vnum-1)
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_block_sum_write(ISP_DEV IspDev, HI_U16 data){
	HI_U32 u16Current = IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26218);
	IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26218, (u16Current & 0xf800) | (data & 0x7ff));
}

static __inline HI_U16 hi_isp_dehaze_block_sum_read(ISP_DEV IspDev) {
	return (IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26218)&0x7ff);
}

/*****************************************************
ISP_DEHAZE_DC_SIZE: 		ISP_DEHAZE_BLK_NUM
Offset:0x701c: 			            31:10		reserved	
							  09:05		dc_numh    Bilinear hnum
						       04:00		dc_numv    Bilinear vnum
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_dc_numh_write(ISP_DEV IspDev, HI_U16 data){
	HI_U16 u16Current = IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2621c);
	IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2621c, (u16Current & 0xfc1f) | ((data & 0x1f) << 5));
}

static __inline HI_U16 hi_isp_dehaze_dc_numh_read(ISP_DEV IspDev) {
	return ((IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2621c)&0x3e0) >> 5);
}

static __inline HI_VOID hi_isp_dehaze_dc_numv_write(ISP_DEV IspDev, HI_U16 data){
	HI_U16 u16Current = IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2621c);
	IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2621c, (u16Current & 0xffe0) | (data & 0x1f));
}

static __inline HI_U16 hi_isp_dehaze_dc_numv_read(ISP_DEV IspDev) {
	return (IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2621c)&0x1f);
}

/*****************************************************
ISP_DEHAZE_X: 		ISP_DEHAZE_X
Offset:0x7020: 			31:22		reserved	
				     21:00		phase_x     
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_phase_x_write(ISP_DEV IspDev, HI_U32 data){
	HI_U32 u32Current = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26220);
	IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26220, (u32Current & 0xffc00000) | (data & 0x3fffff));
}

static __inline HI_U32 hi_isp_dehaze_phase_x_read(ISP_DEV IspDev) {
	return ((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26220)&0x3fffff));
}

/*****************************************************
ISP_DEHAZE_Y: 		ISP_DEHAZE_Y
Offset:0x7024: 	     31:22		reserved	
				21:00		phase_y   
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_phase_y_write(ISP_DEV IspDev, HI_U32 data){
	HI_U32 u32Current = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26224);
	IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26224, (u32Current & 0xffc00000) | (data & 0x3fffff));
}

static __inline HI_U32 hi_isp_dehaze_phase_y_read(ISP_DEV IspDev) {
	return ((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26224)&0x3fffff));
}

/*****************************************************
ISP_DEHAZE_STAT_MODE: 		ISP_DEHAZE_STATMODE DEHAZE max control
Offset:0x7028: 			     31:1		reserved									 
						0			max_mode
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_max_mode_write(ISP_DEV IspDev, HI_U8 data){
	HI_U8 u8Current = IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26228);
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26228, (u8Current & 0xfe) | (data & 0x1));
}

static __inline HI_U8 hi_isp_dehaze_max_mode_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26228) & 0x1);
}

/*****************************************************
ISP_DEHAZE_NEG_MODE: 		ISP_DEHAZE_NEG_MODE 
Offset:0x702c: 			    31:1		reserved									 
						0			neg_mode
									0 normal
									1 neg mode
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_neg_mode_write(ISP_DEV IspDev, HI_U8 data){
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2622c, data);
}

static __inline HI_U8 hi_isp_dehaze_neg_mode_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2622c));
}

/*****************************************************
ISP_DEHAZE_AIR: 		
Offset:0x7030: 			      31:30		reserved	
					      29:20       	air_r 
						19:10		air_g 					      
						09:00		air_b 
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_air_r_write(ISP_DEV IspDev, HI_U16 data){
	HI_U32 u32Current = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26230);
	IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26230, (u32Current & 0xc00fffff) | ((data & 0x3ff) << 20));
}

static __inline HI_U16 hi_isp_dehaze_air_r_read(ISP_DEV IspDev) {
	return ((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26230)&0x3ff00000) >> 20);
}

static __inline HI_VOID hi_isp_dehaze_air_g_write(ISP_DEV IspDev, HI_U16 data){
	HI_U32 u32Current = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26230);
	IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26230, (u32Current & 0xfff003ff) | ((data & 0x3ff) << 10));
}

static __inline HI_U16 hi_isp_dehaze_air_g_read(ISP_DEV IspDev) {
	return ((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26230)&0x000ffc00) >> 10);
}

static __inline HI_VOID hi_isp_dehaze_air_b_write(ISP_DEV IspDev, HI_U16 data){
	HI_U32 u32Current = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26230);
	IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26230, (u32Current & 0xfffffc00) | (data & 0x3ff));
}

static __inline HI_U16 hi_isp_dehaze_air_b_read(ISP_DEV IspDev) {
	return (IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26230)&0x000003ff);
}

/*****************************************************
ISP_DEHAZE_THLD: 	ISP_DEHAZE_THLD
Offset:0x7034: 			31:08		reserved						      
						09:00     threshold of t
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_thld_write(ISP_DEV IspDev, HI_U16 data){
	IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26234, data);
}

static __inline HI_U16 hi_isp_dehaze_thld_read(ISP_DEV IspDev) {
	return (IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26234));
}

/*****************************************************
ISP_DEHAZE_GSTRTH: 	ISP_DEHAZE_GSTRTH
Offset:0x7038: 			31:08		reserved						      
				     07:00		gstrth global strength
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_gstrth_write(ISP_DEV IspDev, HI_U8 data){
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26238, data);
}

static __inline HI_U8 hi_isp_dehaze_gstrth_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26238));
}

/*****************************************************
ISP_DEHAZE_BLTHLD: 	ISP_DEHAZE_BLTHLD min_flt_thld
Offset:0x703c: 			31:08		reserved						      
				     09:00		blthld 
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_blthld_write(ISP_DEV IspDev, HI_U16 data){
	IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2623c, data);
}

static __inline HI_U16 hi_isp_dehaze_blthld_read(ISP_DEV IspDev) {
	return (IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2623c));
}


/*****************************************************
ISP_DEHAZE_BLTHLD: 	Dehaze_gcoef_update lut update
Offset:0x7040: 			31:01		reserved						      
					00		      LUT update
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_gcoef_update_write(ISP_DEV IspDev, HI_U16 data){
	IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26240, data);
}

static __inline HI_U16 hi_isp_dehaze_gcoef_update_read(ISP_DEV IspDev) {
	return (IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26240)&0x1);
}



/*****************************************************
ISP_DEHAZE_MINSTAT_WADDR: 	dehaze minstat write addr
Offset:0x7080: 			31:00		minstat_waddr						      
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_minstat_waddr_write(ISP_DEV IspDev, HI_U32 data){
	IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26280, data);
}

static __inline HI_U32 hi_isp_dehaze_minstat_waddr_read(ISP_DEV IspDev) {
	return (IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26280));
}

/*****************************************************
ISP_DEHAZE_MINSTAT_WDATA: 	dehaze minstat data
Offset:0x7084: 		31:26		reserved
					25:16		minstat_wdata_h
				15:10		reserved
					09:00		minstat_wdata_l
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_minstat_wdata_write(ISP_DEV IspDev, HI_U32 data){
	IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26284, data);
}

static __inline HI_U32 hi_isp_dehaze_minstat_wdata_read(ISP_DEV IspDev) {
	return (IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26284));
}

/*****************************************************
ISP_DEHAZE_MINSTAT_RADDR: 	dehaze minstat write addr
Offset:0x7088: 			31:00		minstat_raddr						      
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_minstat_raddr_write(ISP_DEV IspDev, HI_U32 data){
	IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26288, data);
}

static __inline HI_U32 hi_isp_dehaze_minstat_raddr_read(ISP_DEV IspDev) {
	return (IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26288));
}

/*****************************************************
ISP_DEHAZE_MINSTAT_RDATA: 	dehaze minstat read data
Offset:0x708c: 		31:26		reserved
					25:16		minstat_rdata_h
				15:10		reserved
					09:00		minstat_rdata_l					      
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_minstat_rdata_write(ISP_DEV IspDev, HI_U32 data){
	IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2628c, data);
}

static __inline HI_U32 hi_isp_dehaze_minstat_rdata_read(ISP_DEV IspDev) {
	return (IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2628c));
}

/*****************************************************
ISP_DEHAZE_MAXSTAT_WADDR: 	ISP_DEHAZE_MAXSTAT_WADDR maxstat addr
Offset:0x7090: 			31:00		maxstat_waddr						      
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_maxstat_waddr_write(ISP_DEV IspDev, HI_U32 data){
	IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26290, data);
}

static __inline HI_U32 hi_isp_dehaze_maxstat_waddr_read(ISP_DEV IspDev) {
	return (IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26290));
}

/*****************************************************
ISP_DEHAZE_MAXSTAT_WDATA: 	
Offset:0x7094: 			31:00		maxstat_wdata						      
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_maxstat_wdata_write(ISP_DEV IspDev, HI_U32 data){
	IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26294, data);
}

static __inline HI_U32 hi_isp_dehaze_maxstat_wdata_read(ISP_DEV IspDev) {
	return (IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26294));
}

/*****************************************************
ISP_DEHAZE_MAXSTAT_RADDR: 	
Offset:0x7098: 			31:00		maxstat_raddr						      
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_maxstat_raddr_write(ISP_DEV IspDev, HI_U32 data){
	IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26298, data);
}

static __inline HI_U32 hi_isp_dehaze_maxstat_raddr_read(ISP_DEV IspDev) {
	return (IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x26298));
}

/*****************************************************
ISP_DEHAZE_MAXSTAT_RDATA: 	
Offset:0x709c: 			31:00		maxstat_rdata						      
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_maxstat_rdata_write(ISP_DEV IspDev, HI_U32 data){
	IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2629c, data);
}

static __inline HI_U32 hi_isp_dehaze_maxstat_rdata_read(ISP_DEV IspDev) {
	return (IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2629c));
}

/*****************************************************
ISP_DEHAZE_PRESTAT_WADDR: 	
Offset:0x70a0: 			31:00		prestat_waddr						      
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_prestat_waddr_write(ISP_DEV IspDev, HI_U32 data){
	IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x262a0, data);
}

static __inline HI_U32 hi_isp_dehaze_prestat_waddr_read(ISP_DEV IspDev) {
	return (IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x262a0));
}

/*****************************************************
ISP_DEHAZE_PRESTAT_WDATA: 	
Offset:0x70a4: 		31:26		reserved
					25:16		prestat_wdata_H
				15:10		reserved
					09:00		prestat_wdata_l
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_prestat_wdata_write(ISP_DEV IspDev, HI_U32 data){
	IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x262a4, data);
}

static __inline HI_U32 hi_isp_dehaze_prestat_wdata_read(ISP_DEV IspDev) {
	return (IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x262a4));
}

/*****************************************************
ISP_DEHAZE_PRESTAT_RADDR: 	
Offset:0x70a8: 			31:00		prestat_raddr						      
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_prestat_raddr_write(ISP_DEV IspDev, HI_U32 data){
	IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x262a8, data);
}

static __inline HI_U32 hi_isp_dehaze_prestat_raddr_read(ISP_DEV IspDev) {
	return (IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x262a8));
}

/*****************************************************
ISP_DEHAZE_PRESTAT_RADDR: 	
Offset:0x70ac: 		31:26		reserved
					25:16		prestat_rdata_h
				15:10		reserved
					09:00		prestat_rdata_l	
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_prestat_rdata_write(ISP_DEV IspDev, HI_U32 data){
	IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x262ac, data);
}

static __inline HI_U32 hi_isp_dehaze_prestat_rdata_read(ISP_DEV IspDev) {
	return (IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x262ac));
}

/*****************************************************
ISP_DEHAZE_LUT_WADDR: 	dehaze strength lut addr
Offset:0x70b0: 			31:00		lut_waddr						      
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_lut_waddr_write(ISP_DEV IspDev, HI_U32 data){
	IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x262b0, data);
}

static __inline HI_U32 hi_isp_dehaze_lut_waddr_read(ISP_DEV IspDev) {
	return (IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x262b0));
}

/*****************************************************
ISP_DEHAZE_LUT_WDATA: 	dehaze strength lut write data
Offset:0x70b4: 			31:00		lut_wdata						      
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_lut_wdata_write(ISP_DEV IspDev, HI_U32 data){
	IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x262b4, data);
}

static __inline HI_U32 hi_isp_dehaze_lut_wdata_read(ISP_DEV IspDev) {
	return (IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x262b4));
}

/*****************************************************
ISP_DEHAZE_LUT_RADDR: 	dehaze lut write addr
Offset:0x70b8: 			31:00		lut_raddr						      
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_lut_raddr_write(ISP_DEV IspDev, HI_U32 data){
	IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x262b8, data);
}

static __inline HI_U32 hi_isp_dehaze_lut_raddr_read(ISP_DEV IspDev) {
	return (IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x262b8));
}

/*****************************************************
ISP_DEHAZE_LUT_RDATA: 	dehaze
Offset:0x70bc: 			31:00		lut_rdata						      
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_lut_rdata_write(ISP_DEV IspDev, HI_U32 data){
	IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x262bc, data);
}

static __inline HI_U32 hi_isp_dehaze_lut_rdata_read(ISP_DEV IspDev) {
	return (IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x262bc));
}

/*****************************************************
ISP_DEHAZE_CTRL_I: 		ISP_DEHAZE_NEG_MODE
Offset:0x70e4: 			31:1		reserved									 
						0			update_mode
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_update_mode_write(ISP_DEV IspDev, HI_U8 data){
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x262e4, data);
}

static __inline HI_U8 hi_isp_dehaze_update_mode_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x262e4));
}

/*****************************************************
ISP_DEHAZE_UPDATE: 		ISP_DEHAZE_UPDATE
Offset:0x70ec: 			31:1		reserved									 
						0			newer
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_newer_write(ISP_DEV IspDev, HI_U8 data){
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x262ec, data);
}

static __inline HI_U8 hi_isp_dehaze_newer_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x262ec));
}

/*****************************************************
ISP_DEHAZE_SIZE: 		ISP_DEHAZE_SIZE
Offset:0x70f0: 			31:29		reserved									 
						28:16		height
						15:13		reserved
						12:00		width
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_height_write(ISP_DEV IspDev, HI_U16 data) {
	IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x262f2, (data&0x1fff));
}

static __inline HI_U16 hi_isp_dehaze_height_read(ISP_DEV IspDev) {
	return (IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x262f2)&0x1fff);
}

static __inline HI_VOID hi_isp_dehaze_width_write(ISP_DEV IspDev, HI_U16 data) {
	IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x262f0, (data&0x1fff));
}

static __inline HI_U16 hi_isp_dehaze_width_read(ISP_DEV IspDev) {
	return (IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x262f0)&0x1fff);
}


// ------------------------------------------------------------------------------ //
// Register: hi_vi_top_chn_switch_enable
// ------------------------------------------------------------------------------ //
// 1: enable, 0: disable
// ------------------------------------------------------------------------------ //

#define HI_VI_TOP_CHN_SWITCH_ENABLE_DEFAULT (0)
#define HI_VI_TOP_CHN_SWITCH_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_top_chn_switch_enble_write(ISP_DEV IspDev, HI_U32 data) {
    HI_U32 u32Current = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40028);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40028, ((data & 0x1) << 0) | (u32Current & 0xFFFFFFFE));
}
static __inline HI_U32 hi_vi_top_chn_switch_enble_read(ISP_DEV IspDev) {
    return ((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40028) & 0x1) >> 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_top_chn_switch0
// ------------------------------------------------------------------------------ //
// select vi input for isp channel0 
// ------------------------------------------------------------------------------ //

#define HI_VI_TOP_CHN_SWITCH0_DEFAULT (0)
#define HI_VI_TOP_CHN_SWITCH0_DATASIZE (3)

// args: data (3-bit)
static __inline HI_VOID hi_vi_top_chn_switch0_write(ISP_DEV IspDev, HI_U32 data) {
    HI_U32 u32Current = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40028);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40028, ((data & 0x7) << 4) | (u32Current & 0xFFFFFF8F));
}
static __inline HI_U32 hi_vi_top_chn_switch0_read(ISP_DEV IspDev) {
    return ((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40028) & 0x70) >> 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_top_chn_switch1
// ------------------------------------------------------------------------------ //
// select vi input for isp channel1 
// ------------------------------------------------------------------------------ //

#define HI_VI_TOP_CHN_SWITCH1_DEFAULT (0)
#define HI_VI_TOP_CHN_SWITCH1_DATASIZE (3)

// args: data (3-bit)
static __inline HI_VOID hi_vi_top_chn_switch1_write(ISP_DEV IspDev, HI_U32 data) {
    HI_U32 u32Current = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40028);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40028, ((data & 0x7) << 8) | (u32Current & 0xFFFFF8FF));
}
static __inline HI_U32 hi_vi_top_chn_switch1_read(ISP_DEV IspDev) {
    return ((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40028) & 0x700) >> 8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_top_chn_switch2
// ------------------------------------------------------------------------------ //
// select vi input for isp channel2 
// ------------------------------------------------------------------------------ //

#define HI_VI_TOP_CHN_SWITCH2_DEFAULT (0)
#define HI_VI_TOP_CHN_SWITCH2_DATASIZE (3)

// args: data (3-bit)
static __inline HI_VOID hi_vi_top_chn_switch2_write(ISP_DEV IspDev, HI_U32 data) {
    HI_U32 u32Current = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40028);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40028, ((data & 0x7) << 12) | (u32Current & 0xFFFF8FFF));
}
static __inline HI_U32 hi_vi_top_chn_switch2_read(ISP_DEV IspDev) {
    return ((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40028) & 0x7000) >> 12);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_top_chn_switch3
// ------------------------------------------------------------------------------ //
// select vi input for isp channel3 
// ------------------------------------------------------------------------------ //

#define HI_VI_TOP_CHN_SWITCH3_DEFAULT (0)
#define HI_VI_TOP_CHN_SWITCH3_DATASIZE (3)

// args: data (3-bit)
static __inline HI_VOID hi_vi_top_chn_switch3_write(ISP_DEV IspDev, HI_U32 data) {
    HI_U32 u32Current = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40028);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40028, ((data & 0x7) << 16) | (u32Current & 0xFFF8FFFF));
}
static __inline HI_U32 hi_vi_top_chn_switch3_read(ISP_DEV IspDev) {
    return ((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40028) & 0x70000) >> 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_top_chn_switch4
// ------------------------------------------------------------------------------ //
// select vi input for isp channel4
// ------------------------------------------------------------------------------ //

#define HI_VI_TOP_CHN_SWITCH4_DEFAULT (0)
#define HI_VI_TOP_CHN_SWITCH4_DATASIZE (3)

// args: data (3-bit)
static __inline HI_VOID hi_vi_top_chn_switch4_write(ISP_DEV IspDev, HI_U32 data) {
    HI_U32 u32Current = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40028);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40028, ((data & 0x7) << 20) | (u32Current & 0xFF8FFFFF));
}
static __inline HI_U32 hi_vi_top_chn_switch4_read(ISP_DEV IspDev) {
    return ((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40028) & 0x700000) >> 20);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_fe_bypass_enable
// ------------------------------------------------------------------------------ //
// 0:Not Bypass;  1: Bypass ISP_FE
// ------------------------------------------------------------------------------ //

#define HI_ISP_FE_BYPASS_ENABLE_DEFAULT (0)
#define HI_ISP_FE_BYPASS_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_fe_bypass_enable_write(ISP_DEV IspDev, HI_U32 data) {
    HI_U32 u32Current = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4002C);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4002C, ((data & 0x1) << 0) | (u32Current & 0xFFFFFFFE));
}
static __inline HI_U32 hi_isp_fe_bypass_enable_read(ISP_DEV IspDev) {
    return ((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4002C) & 0x1) >> 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_fe_input_mode_select
// ------------------------------------------------------------------------------ //
// isp input mode switch
// 00:  linear 16bit, After ch_switch 0 channal effectively
// 01:  WDR input,each channal 12bit input
// 10:  All the 16 bit extension input, after ch_switch 0 channal effectively,All the 12 bit input, after ch_switch, 3 channal effectively
// 11:  Two channal 12 bit input, after ch_switch 0, 3 way is effective.
// ------------------------------------------------------------------------------ //

#define HI_ISP_FE_INPUT_MODE_SELECT_DEFAULT (0)
#define HI_ISP_FE_INPUT_MODE_SELECT_DATASIZE (2)

// args: data (2-bit)
static __inline HI_VOID hi_isp_fe_input_mode_select_write(ISP_DEV IspDev, HI_U32 data) {
    HI_U32 u32Current = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4002C);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4002C, ((data & 0x3) << 30) | (u32Current & 0x3FFFFFFF));
}
static __inline HI_U32 hi_isp_fe_input_mode_select_read(ISP_DEV IspDev) {
    return ((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4002C) & 0xC0000000) >> 30);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_fe_input_port_crop_enable
// ------------------------------------------------------------------------------ //
// 0: diable crop of isp input port; 1: enable crop of isp input port
// ------------------------------------------------------------------------------ //

#define HI_ISP_FE_INPUT_PORT_CROP_ENABLE_DEFAULT (0)
#define HI_ISP_FE_INPUT_PORT_CROP_ENABLE_DATASIZE (2)

// args: data (1-bit)
static __inline HI_VOID hi_isp_fe_input_port_crop_enable_write(ISP_DEV IspDev, HI_U32 data) {
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40030, (data & 0x1));
}
static __inline HI_U32 hi_isp_fe_input_port_crop_enable_read(ISP_DEV IspDev) {
    return (IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40030) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_fe_input_port_crop_x_start
// ------------------------------------------------------------------------------ //
// x_start of isp crop
// ------------------------------------------------------------------------------ //

#define HI_ISP_FE_INPUT_PORT_CROP_X_START_DEFAULT (0)
#define HI_ISP_FE_INPUT_PORT_CROP_X_START_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_fe_input_port_crop_x_start_write(ISP_DEV IspDev, HI_U32 data) {
    IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40038, (HI_U16)(data & 0xFFFF));
}
static __inline HI_U32 hi_isp_fe_input_port_crop_x_start_read(ISP_DEV IspDev) {
    return (HI_U32)(IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x40038) & 0xFFFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_fe_input_port_crop_y_start
// ------------------------------------------------------------------------------ //
// y_start of isp crop
// ------------------------------------------------------------------------------ //

#define HI_ISP_FE_INPUT_PORT_CROP_Y_START_DEFAULT (0)
#define HI_ISP_FE_INPUT_PORT_CROP_Y_START_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_fe_input_port_crop_y_start_write(ISP_DEV IspDev, HI_U32 data) {
    IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4003A, (HI_U16)(data & 0xFFFF));
}
static __inline HI_U32 hi_isp_fe_input_port_crop_y_start_read(ISP_DEV IspDev) {
    return (HI_U32)(IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4003A) & 0xFFFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_fe_input_port_crop_width
// ------------------------------------------------------------------------------ //
// width of isp crop
// ------------------------------------------------------------------------------ //

#define HI_ISP_FE_INPUT_PORT_CROP_WIDTH_DEFAULT (0)
#define HI_ISP_FE_INPUT_PORT_CROP_WIDTH_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_fe_input_port_crop_width_write(ISP_DEV IspDev, HI_U32 data) {
    IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4003C, (HI_U16)(data & 0xFFFF));
}
static __inline HI_U32 hi_isp_fe_input_port_crop_width_read(ISP_DEV IspDev) {
    return (HI_U32)(IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4003C) & 0xFFFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_fe_input_port_crop_height
// ------------------------------------------------------------------------------ //
// height of isp crop
// ------------------------------------------------------------------------------ //

#define HI_ISP_FE_INPUT_PORT_CROP_HEIGHT_DEFAULT (0)
#define HI_ISP_FE_INPUT_PORT_CROP_HEIGHT_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_fe_input_port_crop_height_write(ISP_DEV IspDev, HI_U32 data) {
    IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4003E, (HI_U16)(data & 0xFFFF));
}
static __inline HI_U32 hi_isp_fe_input_port_crop_height_read(ISP_DEV IspDev) {
    return (HI_U32)(IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x4003E) & 0xFFFF);
}


#if 1
// ------------------------------------------------------------------------------ //

/*--------------------------------------------------------------------------------------*/
/* write or read vi reg */

// ------------------------------------------------------------------------------ //
// Register: hi_vi_top_set_blc
// ------------------------------------------------------------------------------ //
// BLC enable/disable, r/gr/b/gb offset config
// ------------------------------------------------------------------------------ //

HI_S32 ISP_SetCalibrateAttr(VI_DEV ViDev, ISP_FPN_CALIBRATE_ATTR_S *pstCalibrate);
HI_S32 ISP_SetCorrectionAttr(VI_DEV ViDev, const ISP_FPN_ATTR_S *pstCorrection);
HI_S32 ISP_GetCorrectionAttr(VI_DEV ViDev, ISP_FPN_ATTR_S *pstCorrection);

HI_S32 ISP_MeshShadingCalibration(HI_U16* pu16SrcRaw, ISP_LSC_CALIBRATION_CFG_S* pstLSCCaliCfg 
    , ISP_MESH_SHADING_TABLE_S stMeshShadingResult);


HI_S32 ISP_GetLightboxGain(ISP_DEV IspDev, ISP_AWB_Calibration_Gain_S *stAWBCalibrationGain);


// ------------------------------------------------------------------------------ //
// Register: hi_vi_top_vc_enable
// ------------------------------------------------------------------------------ //
// Enable VC function
// ------------------------------------------------------------------------------ //

#define HI_VI_TOP_VC_ENABLE_DEFAULT (0)
#define HI_VI_TOP_VC_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_top_vc_enable_write(ISP_DEV IspDev, HI_U32 data) {
    HI_U32 u32Current = IORD_32DIRECT(ISP_REG_BASE(IspDev) + 0x01A4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + 0x01A4, ((data & 0x1) << 31) | (u32Current & 0x7FFFFFFF));
}
static __inline HI_U32 hi_vi_top_vc_enable_read(ISP_DEV IspDev) {
return 0;
    return ((IORD_32DIRECT(ISP_REG_BASE(IspDev) + 0x01A4) & 0x80000000) >> 31);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_top_vc_mode
// ------------------------------------------------------------------------------ //
// 0: auto, 1: manual
// ------------------------------------------------------------------------------ //

#define HI_VI_TOP_VC_MODE_DEFAULT (0)
#define HI_VI_TOP_VC_MODE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_top_vc_mode_write(ISP_DEV IspDev, HI_U32 data) {
    //HI_U32 u32Current = IORD_32DIRECT(ISP_REG_BASE(IspDev) + 0x01A4);
    //IOWR_32DIRECT(ISP_REG_BASE(IspDev) + 0x01A4, ((data & 0x1) << 30) | (u32Current & 0xBFFFFFFF));
}
static __inline HI_U32 hi_vi_top_vc_mode_read(ISP_DEV IspDev) {
return 0;
    //return ((IORD_32DIRECT(ISP_REG_BASE(IspDev) + 0x01A4) & 0x40000000) >> 30);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_top_vc_reset
// ------------------------------------------------------------------------------ //
// reset vc
// ------------------------------------------------------------------------------ //

#define HI_VI_TOP_VC_RESET_DEFAULT (0)
#define HI_VI_TOP_VC_RESET_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_top_vc_reset_write(ISP_DEV IspDev, HI_U32 data) {
    //HI_U32 u32Current = IORD_32DIRECT(ISP_REG_BASE(IspDev) + 0x01A4);
    //IOWR_32DIRECT(ISP_REG_BASE(IspDev) + 0x01A4, ((data & 0x1) << 29) | (u32Current & 0xDFFFFFFF));
}
static __inline HI_U32 hi_vi_top_vc_reset_read(ISP_DEV IspDev) {
return 0;
    //return ((IORD_32DIRECT(ISP_REG_BASE(IspDev) + 0x01A4) & 0x20000000) >> 29);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_top_vc_num
// ------------------------------------------------------------------------------ //
// vc number, when mode = auto, RO
// ------------------------------------------------------------------------------ //

#define HI_VI_TOP_VC_NUM_DEFAULT (0)
#define HI_VI_TOP_VC_NUM_DATASIZE (2)

// args: data (2-bit)
static __inline HI_VOID hi_vi_top_vc_num_write(ISP_DEV IspDev, HI_U32 data) {
    //HI_U32 u32Current = IORD_32DIRECT(ISP_REG_BASE(IspDev) + 0x01A4);
    //IOWR_32DIRECT(ISP_REG_BASE(IspDev) + 0x01A4, ((data & 0x3) << 4) | (u32Current & 0xFFFFFFCF));
}
static __inline HI_U32 hi_vi_top_vc_num_read(ISP_DEV IspDev) {
return 0;
    //return ((IORD_32DIRECT(ISP_REG_BASE(IspDev) + 0x01A4) & 0x30) >> 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_top_vc_max
// ------------------------------------------------------------------------------ //
// the max of vc number
// ------------------------------------------------------------------------------ //

#define HI_VI_TOP_VC_MAX_DEFAULT (0)
#define HI_VI_TOP_VC_MAX_DATASIZE (2)

// args: data (2-bit)
static __inline HI_VOID hi_vi_top_vc_max_write(ISP_DEV IspDev, HI_U32 data) {
    HI_U32 u32Current = IORD_32DIRECT(ISP_REG_BASE(IspDev) + 0x01A4);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + 0x01A4, ((data & 0x3) << 2) | (u32Current & 0xFFFFFFF3));
}
static __inline HI_U32 hi_vi_top_vc_max_read(ISP_DEV IspDev) {
return 0;
    return ((IORD_32DIRECT(ISP_REG_BASE(IspDev) + 0x01A4) & 0xC) >> 2);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_top_vc_init_num
// ------------------------------------------------------------------------------ //
// the init value of vc number
// ------------------------------------------------------------------------------ //

#define HI_VI_TOP_VC_INIT_NUM_DEFAULT (0)
#define HI_VI_TOP_VC_INIT_NUM_DATASIZE (2)

// args: data (2-bit)
static __inline HI_VOID hi_vi_top_vc_init_num_write(ISP_DEV IspDev, HI_U32 data) {
    //HI_U32 u32Current = IORD_32DIRECT(ISP_REG_BASE(IspDev) + 0x01A4);
    //IOWR_32DIRECT(ISP_REG_BASE(IspDev) + 0x01A4, ((data & 0x3) << 0) | (u32Current & 0xFFFFFFFC));
}
static __inline HI_U32 hi_vi_top_vc_init_num_read(ISP_DEV IspDev) {
return 0;
    //return ((IORD_32DIRECT(ISP_REG_BASE(IspDev) + 0x01A4) & 0x3) >> 0);
}


// ------------------------------------------------------------------------------ //
// Register: hi_vi_fpn_enalbe_read
// ------------------------------------------------------------------------------ //
//  enalbe,gain,offset
// ------------------------------------------------------------------------------ //


// args: data (32-bit)
static __inline HI_U32 hi_vi_fpn_en_read(ISP_DEV IspDev)
{ 
    HI_U32 u32FpnEnable;
    // fpn en && correction mode
    u32FpnEnable = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x43a00);
    return (HI_U32)((u32FpnEnable&0x1)&&(!(u32FpnEnable&0x100)));
}
// args: data (32-bit)
static __inline HI_U32 hi_vi_fpn_read_gain_offset(ISP_DEV IspDev, HI_U32 index)
{     
    return  IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x43a30+(index<<2)); 
}

static __inline HI_VOID hi_vi_fpn_write_gain_offset(ISP_DEV IspDev, HI_U32 index, HI_U32 data)
{     
   IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x43a30+(index<<2), data);
}

/******************* ACM *******************************************/


extern HI_S32 ISP_ACM_SetAttr(ISP_DEV IspDev, ISP_ACM_ATTR_S *pstAcmAttr);
extern HI_S32 ISP_ACM_GetAttr(ISP_DEV IspDev, ISP_ACM_ATTR_S *pstAcmAttr);
extern HI_S32 ISP_ACM_SetCoeff(ISP_DEV IspDev, ISP_ACM_LUT_S *pstAcmCoeff);
extern HI_S32 ISP_ACM_GetCoeff(ISP_DEV IspDev, ISP_ACM_LUT_S *pstAcmCoeff);



//----------------------useful macro for bit operation----------------------------------------

#define BITS_MASK(msb, lsb) (HI_U32)(((1ULL << (msb+1)) - 1) & ~((1ULL << lsb) - 1))
#define BITS_SHIFT(data, msb, lsb) (HI_U32)((data & (BITS_MASK(msb, lsb) >> lsb)) << lsb)

#define REG_BITS_MODIFY(wrfunc, rdfunc, offset, msb, lsb, data) \
    do {                                                        \
        HI_U32 u32Cfg = rdfunc(offset);                         \
        wrfunc(offset, ((BITS_SHIFT(data, msb, lsb)) | (u32Cfg & ~ BITS_MASK(msb, lsb)))); \
    } while (0);

#define REG_BITS_READ(wrfunc, rdfunc, offset, msb, lsb) \
    ((rdfunc(offset) & BITS_MASK(msb, lsb)) >> lsb)

#define COMPLEMENT_ALIGN32(data, signbit) \
    data & (1ULL << signbit) ? (data | BITS_MASK(31, (signbit+1))) : data;


/******************* BayerScaler *************************************/

/*****************************************************
MODULE        -----BayerScaler-----
MODULE        -----BayerScaler-----
MODULE        -----BayerScaler-----
*****************************************************/

/*****************************************************
ISP_BAS_CFG:		    bayerscl control register
0x11482a00: 			31:21 	reserved
						20	bas_rph_sel
					    19:18	reserved
						17	bas_vbr4sel
						16	bas_vgg6sel
					    15:10	reserved
						9	bas_enable_h
						8	bas_enable_v
					    7:3		reserved
						2:	dsl_en
						1:	rph_en
						0:	bas_en
*****************************************************/

#define HI_VI_BAS_ENABLE_DEFAULT (0x0)
#define HI_VI_BAS_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_bas_enable_write(ISP_DEV IspDev, HI_U8 data) {
   REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a00, 0, 0, data);
}

static __inline HI_U8 hi_vi_bas_enable_read(ISP_DEV IspDev) {
   return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a00, 0, 0);
}


#define HI_VI_BAS_RPH_EN_DEFAULT (0x0)
#define HI_VI_BAS_RPH_EN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_bas_rph_en_write(ISP_DEV IspDev, HI_U8 data) {
   REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a00, 1, 1, data);
}

static __inline HI_U8 hi_vi_bas_rph_en_read(ISP_DEV IspDev) {
   return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a00, 1, 1);
}

#define HI_VI_BAS_DSL_EN_DEFAULT (0x1)
#define HI_VI_BAS_DSL_EN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_bas_dsl_en_write(ISP_DEV IspDev, HI_U8 data) {
   REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a00, 2, 2, data);
}

static __inline HI_U8 hi_vi_bas_dsl_en_read(ISP_DEV IspDev) {
   return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a00, 2, 2);
}

#define HI_VI_BAS_ENABLE_V_DEFAULT (0x1)
#define HI_VI_BAS_ENABLE_V_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_bas_enable_v_write(ISP_DEV IspDev, HI_U8 data) {
   REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a00, 8, 8, data);
}

static __inline HI_U8 hi_vi_bas_enable_v_read(ISP_DEV IspDev) {
   return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a00, 8, 8);
}

#define HI_VI_BAS_ENABLE_H_DEFAULT (0x1)
#define HI_VI_BAS_ENABLE_H_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_bas_enable_h_write(ISP_DEV IspDev, HI_U8 data) {
  REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a00, 9, 9, data); 
}

static __inline HI_U8 hi_vi_bas_enable_h_read(ISP_DEV IspDev) {
  return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a00, 9, 9);
}

#define HI_VI_BAS_VGG6SEL_DEFAULT (0x1)
#define HI_VI_BAS_VGG6SEL_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_bas_vgg6sel_write(ISP_DEV IspDev, HI_U8 data) {
  REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a00, 16, 16, data);
}

static __inline HI_U8 hi_vi_bas_vgg6sel_read(ISP_DEV IspDev) {
  return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a00, 16, 16);
}

#define HI_VI_BAS_VBR4SEL_DEFAULT (0x1)
#define HI_VI_BAS_VBR4SEL_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_bas_vbr4sel_write(ISP_DEV IspDev, HI_U8 data) {
  REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a00, 17, 17, data);
}

static __inline HI_U8 hi_vi_bas_vbr4sel_read(ISP_DEV IspDev) {
  return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a00, 17, 17);
}

#define HI_VI_BAS_RPH_SEL_DEFAULT (0x1)
#define HI_VI_BAS_RPH_SEL_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_bas_rph_sel_write(ISP_DEV IspDev, HI_U8 data) {
  REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a00, 20, 20, data);
}

static __inline HI_U8 hi_vi_bas_rph_sel_read(ISP_DEV IspDev) {
  return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a00, 20, 20);
}

#define HI_VI_BAS_GBRSAME_DEFAULT (0x1)
#define HI_VI_BAS_GBRSAME_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_bas_gbrsame_write(ISP_DEV IspDev, HI_U8 data) {
  REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a00, 24, 24, data);
}

static __inline HI_U8 hi_vi_bas_gbrsame_read(ISP_DEV IspDev) {
  return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a00, 24, 24);
}

static __inline HI_VOID hi_vi_bas_mode_write(ISP_DEV IspDev, HI_U32 data) {
   REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a00, 31, 0, data);
}

static __inline HI_U32 hi_vi_bas_mode_read(ISP_DEV IspDev) {
   return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a00, 31, 0);
}

/*****************************************************
ISP_BAS_OUT_SIZE:         BAS output image size register 
0x11482a04:               31:29 	reserved	
                          28:16     bas_height_out
                          15:13     reserved
                          12:0		bas_width_out
*****************************************************/

#define HI_VI_BAS_HEIGHT_OUT_DEFAULT (0x021B)
#define HI_VI_BAS_HEIGHT_OUT_DATASIZE (13)

// args: data (13-bit)
static __inline HI_VOID hi_vi_bas_height_out_write(ISP_DEV IspDev, HI_U16 data) {
    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a04, 28, 16, data);
}

static __inline HI_U16 hi_vi_bas_height_out_read(ISP_DEV IspDev) {
	return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a04, 28, 16);
}

#define HI_VI_BAS_WIDTH_OUT_DEFAULT (0x03BF)
#define HI_VI_BAS_WIDTH_OUT_DATASIZE (13)

// args: data (13-bit)
static __inline HI_VOID hi_vi_bas_width_out_write(ISP_DEV IspDev, HI_U16 data) {
	REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a04, 12, 0, data);
}

static __inline HI_U16 hi_vi_bas_width_out_read(ISP_DEV IspDev) {
	return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a04, 12, 0);
}

/*****************************************************
ISP_BAS_ACCH:             downscale horizontal sampling of first point register
0x11482a10:               31:18 	reserved	
                          17:00     bas_acch
*****************************************************/

#define HI_VI_BAS_ACCH_DEFAULT (0x0)
#define HI_VI_BAS_ACCH_DATASIZE (18)

// args: data (18-bit)
static __inline HI_VOID hi_vi_bas_acch_write(ISP_DEV IspDev, HI_U32 data) {
   REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a10, 17, 0, data);
}

static __inline HI_U32 hi_vi_bas_acch_read(ISP_DEV IspDev) {
   return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a10, 17, 0);
}

/*****************************************************
ISP_BAS_ACCV:        downscale horizontal sampling of first point register
0x11482a14:          31:18 	reserved	
                     17:00     bas_accv
*****************************************************/

#define HI_VI_BAS_ACCV_DEFAULT (0x0)
#define HI_VI_BAS_ACCV_DATASIZE (18)

// args: data (18-bit)
static __inline HI_VOID hi_vi_bas_accv_write(ISP_DEV IspDev, HI_U32 data) {
   REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a14, 17, 0, data);
}

static __inline HI_U32 hi_vi_bas_accv_read(ISP_DEV IspDev) {
  return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a14, 17, 0);
}

/*****************************************************
ISP_BAS_INCH:             downscale horizontal sampling step
0x11482a18:               31:21 	reserved	
                          20:00     bas_inch
*****************************************************/

#define HI_VI_BAS_INCH_DEFAULT (0x080000)
#define HI_VI_BAS_INCH_DATASIZE (21)

// args: data (21-bit)
static __inline HI_VOID hi_vi_bas_inch_write(ISP_DEV IspDev, HI_U32 data) {
   REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a18,20, 0, data);
}

static __inline HI_U32 hi_vi_bas_inch_read(ISP_DEV IspDev) {
   return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a18,20, 0);
}

/*****************************************************
ISP_BAS_INCV:             downscale horizontal sampling step
0x11482a1c:               31:21 	reserved	
                          20:00     bas_incv
*****************************************************/

#define HI_VI_BAS_INCV_DEFAULT (0x080000)
#define HI_VI_BAS_INCV_DATASIZE (21)

// args: data (21-bit)
static __inline HI_VOID hi_vi_bas_incv_write(ISP_DEV IspDev, HI_U32 data) {
   REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a1c, 20, 0, data);
}

static __inline HI_U32 hi_vi_bas_incv_read(ISP_DEV IspDev) {
  return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a1c,20, 0);
}

/*****************************************************
ISP_RPH_WEIH:             rephase   horizontal sampling coefficient 
0x11482a20:               31:22 	reserved	
                          21:16     bas_rph_weih2
                          15:14 	reserved
                          13:08     bas_rph_weih1
                          07:06 	reserved	
                          05:00     bas_rph_weih0	
*****************************************************/

#define HI_VI_BAS_RPH_WEIH2_DEFAULT (0x04)
#define HI_VI_BAS_RPH_WEIH2_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_vi_bas_rph_weih2_write(ISP_DEV IspDev, HI_U8 data) {
   REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a20, 21, 16, data);
}

static __inline HI_U8 hi_vi_bas_rph_weih2_read(ISP_DEV IspDev) {
   return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a20,21, 16);
}

#define HI_VI_BAS_RPH_WEIH1_DEFAULT (0x1E)
#define HI_VI_BAS_RPH_WEIH1_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_vi_bas_rph_weih1_write(ISP_DEV IspDev, HI_U8 data) {
	REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a20, 13, 8, data);
}

static __inline HI_U8 hi_vi_bas_rph_weih1_read(ISP_DEV IspDev) {
	return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a20,13, 8);
}

#define HI_VI_BAS_RPH_WEIH0_DEFAULT (0x3E)
#define HI_VI_BAS_RPH_WEIH0_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_vi_bas_rph_weih0_write(ISP_DEV IspDev, HI_U8 data) {
	REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a20, 5, 0, data);
}

static __inline HI_U8 hi_vi_bas_rph_weih0_read(ISP_DEV IspDev) {
	return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a20,5, 0);
}

static __inline HI_VOID hi_vi_bas_rph_weih_write(ISP_DEV IspDev, HI_U32 data) {
    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a20, 31, 0, data);
}

static __inline HI_U32 hi_vi_bas_rph_weih_read(ISP_DEV IspDev) {
	return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a20,31, 0);
}

/*****************************************************
ISP_RPH_WEIV:              rephase  vertical sampling coefficient 
0x11482a24:                31:22 	reserved	
                           21:16    bas_rph_weiv2
                           15:14 	reserved
                           13:08    bas_rph_weiv1
                           07:06 	reserved	
                           05:00    bas_rph_weiv0	
*****************************************************/

#define HI_VI_BAS_RPH_WEIV2_DEFAULT (0x04)
#define HI_VI_BAS_RPH_WEIV2_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_vi_bas_rph_weiv2_write(ISP_DEV IspDev, HI_U8 data) {
   REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a24, 21, 16, data);
}

static __inline HI_U8 hi_vi_bas_rph_weiv2_read(ISP_DEV IspDev) {
   return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a24,21, 16);
}

#define HI_VI_BAS_RPH_WEIV1_DEFAULT (0x1E)
#define HI_VI_BAS_RPH_WEIV1_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_vi_bas_rph_weiv1_write(ISP_DEV IspDev, HI_U8 data) {
  REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a24, 13, 8, data);
}

static __inline HI_U8 hi_vi_bas_rph_weiv1_read(ISP_DEV IspDev) {
  return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a24,13, 8);
}

#define HI_VI_BAS_RPH_WEIV0_DEFAULT (0x3E)
#define HI_VI_BAS_RPH_WEIV0_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_vi_bas_rph_weiv0_write(ISP_DEV IspDev, HI_U8 data) {
  REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a24, 5, 0, data);
}

static __inline HI_U8 hi_vi_bas_rph_weiv0_read(ISP_DEV IspDev) {
  return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a24,5, 0);
}

static __inline HI_VOID hi_vi_bas_rph_weiv_write(ISP_DEV IspDev, HI_U32 data) {
  REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a24, 31, 0, data);
}

static __inline HI_U32 hi_vi_bas_rph_weiv_read(ISP_DEV IspDev) {
  return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a24,31, 0);
}

/*****************************************************
ISP_BAS_HLUT_HWADDR:      downscale horizontal sampling 6taps looktable address 
0x11482a80:               31:00 	HLUT_hwaddr	RW
                        
*****************************************************/

#define HI_VI_BAS_HLUT_H_WADDR_DEFAULT (0x0)
#define HI_VI_BAS_HLUT_H_WADDR_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_vi_bas_hlut_h_waddr_write(ISP_DEV IspDev, HI_U32 data) {
  REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a80, 31, 0, data);
}

static __inline HI_U32 hi_vi_bas_hlut_h_waddr_read(ISP_DEV IspDev) {
  return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a80,31, 0);
}

/*****************************************************
ISP_BAS_HLUT_HWDATA:     downscale horizontal sampling 6taps looktable data
0x11482a84:              31:00 	HLUT_hwdata	RO
                        
*****************************************************/

#define HI_VI_BAS_HLUT_H_WDATA_DEFAULT (0x0)
#define HI_VI_BAS_HLUT_H_WDATA_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_vi_bas_hlut_h_wdata_read(ISP_DEV IspDev) {
  return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a84,31, 0);
}

/*****************************************************
ISP_BAS_HLUT_HRADDR:     downscale horizontal sampling 6taps looktable address 
0x11482a88:              31:00 	HLUT_hraddr	RW
                        
*****************************************************/

#define HI_VI_BAS_HLUT_H_RADDR_DEFAULT (0x0)
#define HI_VI_BAS_HLUT_H_RADDR_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_vi_bas_hlut_h_raddr_write(ISP_DEV IspDev, HI_U32 data) {
  REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a88, 31, 0, data);
}

static __inline HI_U32 hi_vi_bas_hlut_h_raddr_read(ISP_DEV IspDev) {
  return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a88,31, 0);
}

/*****************************************************
ISP_BAS_HLUT_HRADDR:      downscale horizontal sampling 6taps looktable data
0x11482a8c:               31:00 	HLUT_hrdata	RW
                        
*****************************************************/

#define HI_VI_BAS_HLUT_H_RDATA_DEFAULT (0x0)
#define HI_VI_BAS_HLUT_H_RDATA_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_vi_bas_hlut_h_rdata_write(ISP_DEV IspDev, HI_U32 data) {
  REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a8c, 31, 0, data);
}
static __inline HI_U32 hi_vi_bas_hlut_h_rdata_read(ISP_DEV IspDev) {
  return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a8c,31, 0);
}

/*****************************************************
ISP_BAS_HLUT_LWADDR:      downscale horizontal sampling 6taps looktable address 
0x11482a90:               31:00 	HLUT_lwaddr	RW
                        
*****************************************************/

#define HI_VI_BAS_HLUT_L_WADDR_DEFAULT (0x0)
#define HI_VI_BAS_HLUT_L_WADDR_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_vi_bas_hlut_l_waddr_write(ISP_DEV IspDev, HI_U32 data) {
  REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a90, 31, 0, data);
}

static __inline HI_U32 hi_vi_bas_hlut_l_waddr_read(ISP_DEV IspDev) {
  return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a90,31, 0);
}

/*****************************************************
ISP_BAS_HLUT_LWDATA:      downscale horizontal sampling 6taps looktable data
0x11482a94:               31:00 	HLUT_lwdata	RO
                        
*****************************************************/

#define HI_VI_BAS_HLUT_L_WDATA_DEFAULT (0x0)
#define HI_VI_BAS_HLUT_L_WDATA_DATASIZE (32)

// args: data (32-bit)

static __inline HI_U32 hi_vi_bas_hlut_l_wdata_read(ISP_DEV IspDev) {
  return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a94,31, 0);
}

/*****************************************************
ISP_BAS_HLUT_LRADDR:      downscale horizontal sampling 6taps looktable address 
0x11482a98:               31:00 	HLUT_lraddr	RW
                        
*****************************************************/

#define HI_VI_BAS_HLUT_L_RADDR_DEFAULT (0x0)
#define HI_VI_BAS_HLUT_L_RADDR_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_vi_bas_hlut_l_raddr_write(ISP_DEV IspDev, HI_U32 data) {
  REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a98, 31, 0, data);
}

static __inline HI_U32 hi_vi_bas_hlut_l_raddr_read(ISP_DEV IspDev) {
  return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a98,31, 0);
}


/*****************************************************
ISP_BAS_HLUT_LRDATA:      downscale horizontal sampling 6taps looktable data
0x11482a9c:               31:00 	HLUT_lrdata RW	
                        
*****************************************************/

#define HI_VI_BAS_HLUT_L_RDATA_DEFAULT (0x0)
#define HI_VI_BAS_HLUT_L_RDATA_DATASIZE (32)

// args: data (32-bit)

static __inline HI_VOID hi_vi_bas_hlut_l_rdata_write(ISP_DEV IspDev, HI_U32 data) {
  REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a9c, 31, 0, data);
}
static __inline HI_U32 hi_vi_bas_hlut_l_rdata_read(ISP_DEV IspDev) {
  return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2a9c,31, 0);
}


/*****************************************************
ISP_BAS_VLUT_HWADDR:     downscale vertical sampling 6taps and 4taps looktable address 
0x11482aa0:              31:00 	v6lut_hwaddr	RW
                        
*****************************************************/

#define HI_VI_BAS_VLUT_H_WADDR_DEFAULT (0x0)
#define HI_VI_BAS_VLUT_H_WADDR_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_vi_bas_vlut_h_waddr_write(ISP_DEV IspDev, HI_U32 data) {
   REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2aa0, 31, 0, data);
}

static __inline HI_U32 hi_vi_bas_vlut_h_waddr_read(ISP_DEV IspDev) {
   return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2aa0,31, 0);
}

/*****************************************************
ISP_BAS_VLUT_HWDATA:     downscale vertical sampling 6taps and 4taps looktable data 
0x11482aa4:              31:00 	v6lut_hwdata	RO
                        
*****************************************************/

#define HI_VI_BAS_VLUT_H_WDATA_DEFAULT (0x0)
#define HI_VI_BAS_VLUT_H_WDATA_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_vi_bas_vlut_h_wdata_read(ISP_DEV IspDev) {
  return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2aa4,31, 0);
}

/*****************************************************
ISP_BAS_VLUT_HRADDR:      downscale vertical sampling 6taps and 4taps looktable address 
0x11482aa8:               31:00 	v6lut_hraddr	RW
                        
*****************************************************/

#define HI_VI_BAS_VLUT_H_RADDR_DEFAULT (0x0)
#define HI_VI_BAS_VLUT_H_RADDR_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_vi_bas_vlut_h_raddr_write(ISP_DEV IspDev, HI_U32 data) {
   REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2aa8, 31, 0, data);
}

static __inline HI_U32 hi_vi_bas_vlut_h_raddr_read(ISP_DEV IspDev) {
   return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2aa8,31, 0);
}

/*****************************************************
ISP_BAS_VLUT_HRADDR       downscale vertical sampling 6taps and 4taps looktable data 
0x11482aac:               31:00 	v6lut_hrdata	RW
                        
*****************************************************/

#define HI_VI_BAS_VLUT_H_RDATA_DEFAULT (0x0)
#define HI_VI_BAS_VLUT_H_RDATA_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_vi_bas_vlut_h_rdata_write(ISP_DEV IspDev, HI_U32 data) {
   REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2aac, 31, 0, data);
}
static __inline HI_U32 hi_vi_bas_vlut_h_rdata_read(ISP_DEV IspDev) {
   return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2aac,31, 0);
}

/*****************************************************
ISP_BAS_VLUT_LWADDR:      downscale vertical sampling 6taps and 4taps looktable address 
0x11482ab0:               31:00 	v6lut_lwaddr	RW
                        
*****************************************************/

#define HI_VI_BAS_VLUT_L_WADDR_DEFAULT (0x0)
#define HI_VI_BAS_VLUT_L_WADDR_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_vi_bas_vlut_l_waddr_write(ISP_DEV IspDev, HI_U32 data) {
   REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2ab0, 31, 0, data);
}

static __inline HI_U32 hi_vi_bas_vlut_l_waddr_read(ISP_DEV IspDev) {
   return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2ab0,31, 0);
}

/*****************************************************
ISP_BAS_VLUT_LWDATA:      downscale vertical sampling 6taps and 4taps looktable data 
0x11482ab4:               31:00 	v6lut_lwdata	RO
                        
*****************************************************/

#define HI_VI_BAS_VLUT_L_WDATA_DEFAULT (0x0)
#define HI_VI_BAS_VLUT_L_WDATA_DATASIZE (32)

// args: data (32-bit)

static __inline HI_U32 hi_vi_bas_vlut_l_wdata_read(ISP_DEV IspDev) {
   return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2ab4,31, 0);
}

/*****************************************************
ISP_BAS_VLUT_LRADDR:      downscale vertical sampling 6taps and 4taps looktable address
0x11482ab8:               31:00 	v6lut_lraddr	RW
                        
*****************************************************/

#define HI_VI_BAS_VLUT_L_RADDR_DEFAULT (0x0)
#define HI_VI_BAS_VLUT_L_RADDR_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_vi_bas_vlut_l_raddr_write(ISP_DEV IspDev, HI_U32 data) {
   REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2ab8, 31, 0, data);
}

static __inline HI_U32 hi_vi_bas_vlut_l_raddr_read(ISP_DEV IspDev) {
   return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2ab8,31, 0);
}


/*****************************************************
ISP_BAS_VLUT_LRDATA:      downscale vertical sampling 6taps and 4taps looktable data 
0x11482abc:               31:00 	v6lut_lrdata RW	
                        
*****************************************************/

#define HI_VI_BAS_VLUT_L_RDATA_DEFAULT (0x0)
#define HI_VI_BAS_VLUT_L_RDATA_DATASIZE (32)

// args: data (32-bit)

static __inline HI_VOID hi_vi_bas_vlut_l_rdata_write(ISP_DEV IspDev, HI_U32 data) {
   REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2abc, 31, 0, data);
}
static __inline HI_U32 hi_vi_bas_vlut_l_rdata_read(ISP_DEV IspDev) {
   return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2abc,31, 0);
}


/*****************************************************
ISP_BAS_CTRL_F:      control register
0x11482ae0:          31:02	reserved	
                     01:00	rggb_cfg
*****************************************************/

#define HI_VI_BAS_RGGB_CFG_DEFAULT (0x3)
#define HI_VI_BAS_RGGB_CFG_DATASIZE (2)

// args: data (2-bit)
static __inline HI_VOID hi_vi_bas_rggb_cfg_write(ISP_DEV IspDev, HI_U8 data) {
   REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2ae0, 1, 0, data);
}

static __inline HI_U8 hi_vi_bas_rggb_cfg_read(ISP_DEV IspDev) {
   return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2ae0,1, 0);
}

/*****************************************************
ISP_BAS_CTRL_I:      update register
0x11482ae4:          31:01	reserved	
                     00	    update_mode
*****************************************************/

#define HI_VI_BAS_UPDATE_MODE_DEFAULT (0x0)
#define HI_VI_BAS_UPDATE_MODE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_bas_update_mode_write(ISP_DEV IspDev, HI_U8 data) {
   REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2ae4, 0, 0, data);
}

static __inline HI_U8 hi_vi_bas_update_mode_read(ISP_DEV IspDev) {
   return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2ae4,0, 0);
}


/*****************************************************
ISP_BAS_TIMING:      output timing config register
0x11482ae8:          31:14	reserved	
                     13:01	cpi_fix_timing_stat
                     00:00	reserved
*****************************************************/
#define HI_VI_BAS_TIMING_DEFAULT (0x0040)
#define HI_VI_BAS_TIMING_DATASIZE (13)

// args: data (13-bit)
static __inline HI_VOID hi_vi_bas_timing_write(ISP_DEV IspDev, HI_U16 data) {
   REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2ae8, 13, 1, data);
}

static __inline HI_U16 hi_vi_bas_timing_read(ISP_DEV IspDev) {
   return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2ae8,13, 1);
}

/*****************************************************
ISP_BAS_UPDATE:      update register
0x11482aec:          31:01	reserved	
                     00	    update
*****************************************************/

#define HI_VI_BAS_UPDATE_DEFAULT (0x0)
#define HI_VI_BAS_UPDATE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_bas_update_write(ISP_DEV IspDev, HI_U8 data) {
   REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2aec, 0, 0, data);
}

static __inline HI_U8 hi_vi_bas_update_read(ISP_DEV IspDev) {
   return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2aec,0, 0);
}


/*****************************************************
ISP_BAS_SIZE:        BAS output image height and width
0x11482af0:          31:29		reserved									 
				     28:16		height
					 15:13		reserved
					 12:00		width
*****************************************************/

#define HI_VI_BAS_HEIGHT_DEFAULT (0x437)
#define HI_VI_BAS_HEIGHT_DATASIZE (13)

// args: data (13-bit)
static __inline HI_VOID hi_vi_bas_height_write(ISP_DEV IspDev, HI_U8 data) {
   REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2af0, 28, 16, data);
}

static __inline HI_U8 hi_vi_bas_height_read(ISP_DEV IspDev) {
   return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2af0,28, 16);
}

#define HI_VI_BAS_WIDTH_DEFAULT (0x77f)
#define HI_VI_BAS_WIDTH_DATASIZE (13)

// args: data (13-bit)
static __inline HI_VOID hi_vi_bas_width_write(ISP_DEV IspDev, HI_U8 data) {
   REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2af0, 12, 0, data);
}

static __inline HI_U8 hi_vi_bas_width_read(ISP_DEV IspDev) {
   return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x2af0,12, 0);
}


    
//--------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_pos_write
// ------------------------------------------------------------------------------ //
//  af enable
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_POS_DEFAULT (0)
#define HI_VI_AF_POS_DATASIZE (0)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_pos_write(ISP_DEV IspDev, HI_U32 data) {  

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x0050, 16, 16, data);
}
static __inline HI_U32 hi_vi_af_pos_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x0050, 16, 16);
}


// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_enable
// ------------------------------------------------------------------------------ //
//  af enable
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_ENABLE_DEFAULT (0)
#define HI_VI_AF_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_enable_write(ISP_DEV IspDev, HI_U32 data) {  

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 0, 0, data);
}
static __inline HI_U32 hi_vi_af_enable_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 0, 0);
}

// ------------------------------------------------------------------------------ //
// Register: iir0_en0
// ------------------------------------------------------------------------------ //
// iir0 en0
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_IIR0_EN1_DEFAULT (0)
#define HI_VI_AF_IIR0_EN1_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_iir0_en0_write(ISP_DEV IspDev, HI_U32 data) {

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 1, 1, data);
}
static __inline HI_U32 hi_vi_af_iir0_en0_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 1, 1);

}

// ------------------------------------------------------------------------------ //
// Register: iir0_en1
// ------------------------------------------------------------------------------ //
// iir0 en1
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_IIR0_EN1_DEFAULT (0)
#define HI_VI_AF_IIR0_EN1_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_iir0_en1_write(ISP_DEV IspDev, HI_U32 data) {

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 2, 2, data);
}
static __inline HI_U32 hi_vi_af_iir0_en1_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 2, 2);
;
}

// ------------------------------------------------------------------------------ //
// Register: iir0_en2
// ------------------------------------------------------------------------------ //
// iir0 en2
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_IIR0_EN2_DEFAULT (0)
#define HI_VI_AF_IIR0_EN2_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_iir0_en2_write(ISP_DEV IspDev, HI_U32 data) {

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 3, 3, data);
}
    
static __inline HI_U32 hi_vi_af_iir0_en2_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 3, 3);
}

// ------------------------------------------------------------------------------ //
// Register: iir1_en0
// ------------------------------------------------------------------------------ //
// iir1 en0
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_IIR1_EN1_DEFAULT (0)
#define HI_VI_AF_IIR1_EN1_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_iir1_en0_write(ISP_DEV IspDev, HI_U32 data) {

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 4, 4, data);
}
static __inline HI_U32 hi_vi_af_iir1_en0_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 4, 4);
}

// ------------------------------------------------------------------------------ //
// Register: iir1_en1
// ------------------------------------------------------------------------------ //
// iir1 en1
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_IIR1_EN1_DEFAULT (0)
#define HI_VI_AF_IIR1_EN1_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_iir1_en1_write(ISP_DEV IspDev, HI_U32 data) {

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 5, 5, data);
}
static __inline HI_U32 hi_vi_af_iir1_en1_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 5, 5);
}

// ------------------------------------------------------------------------------ //
// Register: iir1_en2
// ------------------------------------------------------------------------------ //
// iir1 en2
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_IIR1_EN2_DEFAULT (0)
#define HI_VI_AF_IIR1_EN2_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_iir1_en2_write(ISP_DEV IspDev, HI_U32 data) {

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 6, 6, data);
}
static __inline HI_U32 hi_vi_af_iir1_en2_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 6, 6);
}

// ------------------------------------------------------------------------------ //
// Register: fvpeak_mode
// ------------------------------------------------------------------------------ //
// fv peak mode
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_FV_PEAK_MODE_DEFAULT (0)
#define HI_VI_AF_FV_PEAK_MODE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_fvmode_peak_write(ISP_DEV IspDev, HI_U32 data) {

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 7, 7, data);
}
static __inline HI_U32 hi_vi_af_fvmode_peak_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 7, 7);
}

// ------------------------------------------------------------------------------ //
// Register: af squ_mode
// ------------------------------------------------------------------------------ //
// af squ mode
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_FVMODE_SQU_DEFAULT (1)
#define HI_VI_AF_FVMODE_SQU_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_fvmode_squ_write(ISP_DEV IspDev, HI_U32 data) {

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 8, 8, data);
}
static __inline HI_U32 hi_vi_af_fvmode_squ_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 8, 8);
}

// ------------------------------------------------------------------------------ //
// Register: offset_en
// ------------------------------------------------------------------------------ //
// offset en
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_OFFSET_EN_DEFAULT (0)
#define HI_VI_AF_OFFSET_EN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_offset_en_write(ISP_DEV IspDev, HI_U32 data) {

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 9, 9, data);
}
static __inline HI_U32 hi_vi_af_offset_en_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 9, 9);
}

// ------------------------------------------------------------------------------ //
// Register: crop_en
// ------------------------------------------------------------------------------ //
// crop enable
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_CROP_EN_DEFAULT (0)
#define HI_VI_AF_CROP_EN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_crop_en_write(ISP_DEV IspDev, HI_U32 data) {

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 10, 10, data);
}
static __inline HI_U32 hi_vi_af_crop_en_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 10, 10);
}

// ------------------------------------------------------------------------------ //
// Register: lpf_en
// ------------------------------------------------------------------------------ //
// af lpf enable
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_LPF_EN_DEFAULT (0)
#define HI_VI_AF_LPF_EN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_lpf_en_write(ISP_DEV IspDev, HI_U32 data) {

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 11, 11, data);
}
static __inline HI_U32 hi_vi_af_lpf_en_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 11, 11);
}

// ------------------------------------------------------------------------------ //
// Register: median_en
// ------------------------------------------------------------------------------ //
// af median filter enable
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_MEDIAN_EN_DEFAULT (0)
#define HI_VI_AF_MEDIAN_EN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_median_en_write(ISP_DEV IspDev, HI_U32 data) {

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 12, 12, data);
}
static __inline HI_U32 hi_vi_af_median_en_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 12, 12);
}

// ------------------------------------------------------------------------------ //
// Register: sqrt_en
// ------------------------------------------------------------------------------ //
// af gamma enable
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_SQRT_EN_DEFAULT (0)
#define HI_VI_AF_SQRT_EN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_sqrt_en_write(ISP_DEV IspDev, HI_U32 data) {

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 13, 13, data);
}
static __inline HI_U32 hi_vi_af_sqrt_en_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 13, 13);
}

// ------------------------------------------------------------------------------ //
// Register: raw_mode
// ------------------------------------------------------------------------------ //
// AF measure position, YUV or RAW domain
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_RAW_MODE_DEFAULT (0)
#define HI_VI_AF_RAW_MODE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_raw_mode_write(ISP_DEV IspDev, HI_U32 data) {

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 14, 14, data);
}
static __inline HI_U32 hi_vi_af_raw_mode_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 14, 14);
}

// ------------------------------------------------------------------------------ //
// Register: bayer_mode
// ------------------------------------------------------------------------------ //
// bayer pattern mode
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_BAYER_MODE_DEFAULT (0)
#define HI_VI_AF_BAYER_MODE_DATASIZE (2)

// args: data (2-bit)
static __inline HI_VOID hi_vi_af_bayer_mode_write(ISP_DEV IspDev, HI_U32 data) {

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 16, 15, data);
}
static __inline HI_U32 hi_vi_af_bayer_mode_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 16, 15);
}


// ------------------------------------------------------------------------------ //
// Register: iir0_ds_en
// ------------------------------------------------------------------------------ //
// iir0 ds enable
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_IIR0_DS_EN_DEFAULT (0x1)
#define HI_VI_AF_IIR0_DS_EN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_iir0_ds_en_write(ISP_DEV IspDev, HI_U32 data) {

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 17, 17, data);
}
static __inline HI_U32 hi_vi_af_iir0_ds_en_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 17, 17);
}

// ------------------------------------------------------------------------------ //
// Register: iir1_ds_en
// ------------------------------------------------------------------------------ //
// iir1 ds enable
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_IIR1_DS_EN_DEFAULT (0x1)
#define HI_VI_AF_IIR1_DS_EN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_iir1_ds_en_write(ISP_DEV IspDev, HI_U32 data) {

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 18, 18, data);
}
static __inline HI_U32 hi_vi_af_iir1_ds_en_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 18, 18);
}

// ------------------------------------------------------------------------------ //
// Register: fir0_lpf_en
// ------------------------------------------------------------------------------ //
// fir0 lpf enable
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_FIR0_LPF_EN_DEFAULT (0x1)
#define HI_VI_AF_FIR0_LPF_EN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_fir0_lpf_en_write(ISP_DEV IspDev, HI_U32 data) {

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 19, 19, data);
}
static __inline HI_U32 hi_vi_af_fir0_lpf_en_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 19, 19);
}

// ------------------------------------------------------------------------------ //
// Register: fir1_lpf_en
// ------------------------------------------------------------------------------ //
// fir1 lpf enable
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_FIR1_LPF_EN_DEFAULT (0x1)
#define HI_VI_AF_FIR1_LPF_EN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_fir1_lpf_en_write(ISP_DEV IspDev, HI_U32 data) {

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 20, 20, data);
}
static __inline HI_U32 hi_vi_af_fir1_lpf_en_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 20, 20);
}

// ------------------------------------------------------------------------------ //
// Register: iir0_ldg_en
// ------------------------------------------------------------------------------ //
// iir0 level depend gain enable
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_IIR0_LDG_EN_DEFAULT (0)
#define HI_VI_AF_IIR0_LDG_EN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_iir0_ldg_en_write(ISP_DEV IspDev, HI_U32 data) {   

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 21, 21, data);
}
static __inline HI_U32 hi_vi_af_iir0_ldg_en_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 21, 21);
}

// ------------------------------------------------------------------------------ //
// Register: iir1_ldg_en
// ------------------------------------------------------------------------------ //
// iir1 level depend gain enable
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_IIR1_LDG_EN_DEFAULT (0)
#define HI_VI_AF_IIR1_LDG_EN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_iir1_ldg_en_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 22, 22, data);
}
static __inline HI_U32 hi_vi_af_iir1_ldg_en_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 22, 22);
}


// ------------------------------------------------------------------------------ //
// Register: fir0_ldg_en
// ------------------------------------------------------------------------------ //
// fir0 level depend gain enable
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_FIR0_LDG_EN_DEFAULT (0)
#define HI_VI_AF_FIR0_LDG_EN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_fir0_ldg_en_write(ISP_DEV IspDev, HI_U32 data) {

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 23, 23, data);
}
static __inline HI_U32 hi_vi_af_fir0_ldg_en_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 23, 23);
}

// ------------------------------------------------------------------------------ //
// Register: fir1_ldg_en
// ------------------------------------------------------------------------------ //
// fir1 level depend gain enable
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_FIR1_LDG_EN_DEFAULT (0)
#define HI_VI_AF_FIR1_LDG_EN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_fir1_ldg_en_write(ISP_DEV IspDev, HI_U32 data) {

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 24, 24, data);
}
static __inline HI_U32 hi_vi_af_fir1_ldg_en_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 24, 24);
}

// ------------------------------------------------------------------------------ //
// Register: ck_gt_en
// ------------------------------------------------------------------------------ //
//  af gate clk enable
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_CK_GT_EN_DEFAULT (0x1)
#define HI_VI_AF_CK_GT_EN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_ck_gt_en_write(ISP_DEV IspDev, HI_U32 data) {   

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 31, 31, data);
}
static __inline HI_U32 hi_vi_af_ck_gt_en_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12200, 31, 31);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_Hwnd
// ------------------------------------------------------------------------------ //
//  af Horizontal window number
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_HWND_DEFAULT (0x11)
#define HI_VI_AF_HWND_DATASIZE (5)

// args: data (5-bit)
static __inline HI_VOID hi_vi_af_hwnd_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12210, 4, 0, data);
}
static __inline HI_U32 hi_vi_af_hwnd_read(ISP_DEV IspDev) {
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12210, 4, 0);
}


// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_Vwnd
// ------------------------------------------------------------------------------ //
//  af Vertical window number
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_VWND_DEFAULT (0xF)
#define HI_VI_AF_VWND_DATASIZE (5)

// args: data (5-bit)
static __inline HI_VOID hi_vi_af_vwnd_write(ISP_DEV IspDev, HI_U32 data) {   

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12210, 12, 8, data);
}
static __inline HI_U32 hi_vi_af_vwnd_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12210, 12, 8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_crop_posx
// ------------------------------------------------------------------------------ //
//  af crop pos x
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_CROP_POSX_DEFAULT (0x0)
#define HI_VI_AF_CROP_POSX_DATASIZE (13)

// args: data (13-bit)
static __inline HI_VOID hi_vi_af_crop_posx_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12214, 12, 0, data);

}
static __inline HI_U32 hi_vi_af_crop_posx_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12214, 12, 0);
}


// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_crop_pos_y
// ------------------------------------------------------------------------------ //
//  af crop pos y
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_CROP_POSY_DEFAULT (0x0)
#define HI_VI_AF_CROP_POSY_DATASIZE (13)

// args: data (13-bit)
static __inline HI_VOID hi_vi_af_crop_posy_write(ISP_DEV IspDev, HI_U32 data) {  

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12214, 28, 16, data);

}
static __inline HI_U32 hi_vi_af_crop_posy_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12214, 28, 16);
}


// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_crop_hsize
// ------------------------------------------------------------------------------ //
//  af crop Hsize
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_CROP_HSIZE_DEFAULT (0x77E)
#define HI_VI_AF_CROP_HSIZE_DATASIZE (13)

// args: data (13-bit)
static __inline HI_VOID hi_vi_af_crop_hsize_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12218, 12, 0, data);
}
static __inline HI_U32 hi_vi_af_crop_hsize_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12218, 12, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_crop_vsize
// ------------------------------------------------------------------------------ //
//  af crop vsize
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_CROP_VSIZE_DEFAULT (0x437)
#define HI_VI_AF_CROP_VSIZE_DATASIZE (13)

// args: data (13-bit)
static __inline HI_VOID hi_vi_af_crop_vsize_write(ISP_DEV IspDev, HI_U32 data) {

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12218, 28, 16, data);
}
static __inline HI_U32 hi_vi_af_crop_vsize_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12218, 28, 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_median_th
// ------------------------------------------------------------------------------ //
//  af median filter threshold
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_MEDIAN_TH_DEFAULT (0x0)
#define HI_VI_AF_MEDIAN_TH_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_vi_af_median_th_write(ISP_DEV IspDev, HI_U32 data) {   

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x1221c, 15, 0, data);
}
static __inline HI_U32 hi_vi_af_median_th_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x1221c, 15, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_gain0
// ------------------------------------------------------------------------------ //
//  af iir0 gain0
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_GAIN0_DEFAULT (0xA0)
#define HI_VI_AF_IIR0_GAIN0_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_iir0_gain0_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12220, 7, 0, data);
}
static __inline HI_U32 hi_vi_af_iir0_gain0_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12220, 7, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_gain0
// ------------------------------------------------------------------------------ //
//  af iir1 gain0
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_GAIN0_DEFAULT (0xA1)
#define HI_VI_AF_IIR1_GAIN0_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_iir1_gain0_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12220, 23, 16, data);
}
static __inline HI_U32 hi_vi_af_iir1_gain0_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12220, 23, 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_gain1
// ------------------------------------------------------------------------------ //
//  af iir0 gain1
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_GAIN1_DEFAULT (0x1BC)
#define HI_VI_AF_IIR0_GAIN1_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_vi_af_iir0_gain1_write(ISP_DEV IspDev, HI_U32 data) {   

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12224, 9, 0, data);
}
static __inline HI_U32 hi_vi_af_iir0_gain1_read(ISP_DEV IspDev) {
    
    return COMPLEMENT_ALIGN32(REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12224, 9, 0), 9);

}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_gain1
// ------------------------------------------------------------------------------ //
//  af iir1 gain1
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_GAIN1_DEFAULT (0x244)
#define HI_VI_AF_IIR1_GAIN1_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_vi_af_iir1_gain1_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12224, 25, 16, data);

}
static __inline HI_U32 hi_vi_af_iir1_gain1_read(ISP_DEV IspDev) {
    
    return COMPLEMENT_ALIGN32(REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12224, 25, 16), 9);

}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_gain2
// ------------------------------------------------------------------------------ //
//  af iir0 gain2
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_GAIN2_DEFAULT (0x328)
#define HI_VI_AF_IIR0_GAIN2_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_vi_af_iir0_gain2_write(ISP_DEV IspDev, HI_U32 data) {   

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12228, 9, 0, data);

}
static __inline HI_U32 hi_vi_af_iir0_gain2_read(ISP_DEV IspDev) {
    
    return COMPLEMENT_ALIGN32(REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12228, 9, 0), 9);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_gain2
// ------------------------------------------------------------------------------ //
//  af iir1 gain2
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_GAIN2_DEFAULT (0x328)
#define HI_VI_AF_IIR1_GAIN2_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_vi_af_iir1_gain2_write(ISP_DEV IspDev, HI_U32 data) {  

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12228, 25, 16, data);
}
static __inline HI_U32 hi_vi_af_iir1_gain2_read(ISP_DEV IspDev) {
    
    return COMPLEMENT_ALIGN32(REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12228, 25, 16), 9);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_gain3
// ------------------------------------------------------------------------------ //
//  af iir0 gain3
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_GAIN3_DEFAULT (0)
#define HI_VI_AF_IIR0_GAIN3_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_vi_af_iir0_gain3_write(ISP_DEV IspDev, HI_U32 data) {  

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x1222c, 9, 0, data);
}
static __inline HI_U32 hi_vi_af_iir0_gain3_read(ISP_DEV IspDev) {

    return COMPLEMENT_ALIGN32(REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x1222c, 9, 0), 9);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_gain3
// ------------------------------------------------------------------------------ //
//  af iir1 gain3
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_GAIN3_DEFAULT (0)
#define HI_VI_AF_IIR1_GAIN3_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_vi_af_iir1_gain3_write(ISP_DEV IspDev, HI_U32 data) {  

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x1222c, 25, 16, data);

}
static __inline HI_U32 hi_vi_af_iir1_gain3_read(ISP_DEV IspDev) {

    return COMPLEMENT_ALIGN32(REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x1222c, 25, 16), 9);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_gain4
// ------------------------------------------------------------------------------ //
//  af iir0 gain4
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_GAIN4_DEFAULT (0)
#define HI_VI_AF_IIR0_GAIN4_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_vi_af_iir0_gain4_write(ISP_DEV IspDev, HI_U32 data) {  

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12230, 9, 0, data);

}
static __inline HI_U32 hi_vi_af_iir0_gain4_read(ISP_DEV IspDev) {

    return COMPLEMENT_ALIGN32(REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12230, 9, 0), 9);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_gain4
// ------------------------------------------------------------------------------ //
//  af iir1 gain4
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_GAIN4_DEFAULT (0)
#define HI_VI_AF_IIR1_GAIN4_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_vi_af_iir1_gain4_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12230, 25, 16, data);
}
static __inline HI_U32 hi_vi_af_iir1_gain4_read(ISP_DEV IspDev) {

    return COMPLEMENT_ALIGN32(REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12230, 25, 16), 9);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_gain5
// ------------------------------------------------------------------------------ //
//  af iir0 gain5
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_GAIN5_DEFAULT (0x17C)
#define HI_VI_AF_IIR0_GAIN5_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_vi_af_iir0_gain5_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12234, 9, 0, data);
}
static __inline HI_U32 hi_vi_af_iir0_gain5_read(ISP_DEV IspDev) {

    return COMPLEMENT_ALIGN32(REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12234, 9, 0), 9);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_gain5
// ------------------------------------------------------------------------------ //
//  af iir1 gain5
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_GAIN5_DEFAULT (0x284)
#define HI_VI_AF_IIR1_GAIN5_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_vi_af_iir1_gain5_write(ISP_DEV IspDev, HI_U32 data) {

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12234, 25, 16, data);
}
static __inline HI_U32 hi_vi_af_iir1_gain5_read(ISP_DEV IspDev) {
    
    return COMPLEMENT_ALIGN32(REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12234, 25, 16), 9);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_gain6
// ------------------------------------------------------------------------------ //
//  af iir0 gain6
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_GAIN6_DEFAULT (0x33C)
#define HI_VI_AF_IIR0_GAIN6_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_vi_af_iir0_gain6_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12238, 9, 0, data);
}
static __inline HI_U32 hi_vi_af_iir0_gain6_read(ISP_DEV IspDev) {

    return COMPLEMENT_ALIGN32(REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12238, 9, 0), 9);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_gain6
// ------------------------------------------------------------------------------ //
//  af iir1 gain6
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_GAIN6_DEFAULT (0x33C)
#define HI_VI_AF_IIR1_GAIN6_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_vi_af_iir1_gain6_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12238, 25, 16, data);

}
static __inline HI_U32 hi_vi_af_iir1_gain6_read(ISP_DEV IspDev) {

    return COMPLEMENT_ALIGN32(REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12238, 25, 16), 9);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_plg
// ------------------------------------------------------------------------------ //
//  iir0 pre load gain
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_PLG_DEFAULT (0x96)
#define HI_VI_AF_IIR0_PLG_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_iir0_plg_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x1223c, 7, 0, data);

}
static __inline HI_U32 hi_vi_af_iir0_plg_read(ISP_DEV IspDev) {

    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x1223c, 7, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_pls
// ------------------------------------------------------------------------------ //
//  iir0 pre load shift
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_PLS_DEFAULT (0x3)
#define HI_VI_AF_IIR0_PLS_DATASIZE (3)

// args: data (3-bit)
static __inline HI_VOID hi_vi_af_iir0_pls_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x1223c, 10, 8, data);

}
static __inline HI_U32 hi_vi_af_iir0_pls_read(ISP_DEV IspDev) {

    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x1223c, 10, 8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_plg
// ------------------------------------------------------------------------------ //
//  iir1 pre load gain
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_PLG_DEFAULT (0x99)
#define HI_VI_AF_IIR1_PLG_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_iir1_plg_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x1223c, 23, 16, data);

}
static __inline HI_U32 hi_vi_af_iir1_plg_read(ISP_DEV IspDev) {

    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x1223c, 23, 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_pls
// ------------------------------------------------------------------------------ //
//  iir1 pre load shift
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_PLS_DEFAULT (0x3)
#define HI_VI_AF_IIR1_PLS_DATASIZE (3)

// args: data (3-bit)
static __inline HI_VOID hi_vi_af_iir1_pls_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x1223c, 26, 24, data);
}
static __inline HI_U32 hi_vi_af_iir1_pls_read(ISP_DEV IspDev) {

    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x1223c, 26, 24);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_shift0
// ------------------------------------------------------------------------------ //
//  af iir0 shift0
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_SHIFT0_DEFAULT (0x7)
#define HI_VI_AF_IIR0_SHIFT0_DATASIZE (3)

// args: data (3-bit)
static __inline HI_VOID hi_vi_af_iir0_shift0_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12240, 2, 0, data);
}
static __inline HI_U32 hi_vi_af_iir0_shift0_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12240, 2, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_shift1
// ------------------------------------------------------------------------------ //
//  af iir0 shift1
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_SHIFT1_DEFAULT (0x2)
#define HI_VI_AF_IIR0_SHIFT1_DATASIZE (3)

// args: data (3-bit)
static __inline HI_VOID hi_vi_af_iir0_shift1_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12240, 6, 4, data);
}
static __inline HI_U32 hi_vi_af_iir0_shift1_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12240, 6, 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_shift2
// ------------------------------------------------------------------------------ //
//  af iir0 shift2
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_SHIFT2_DEFAULT (0x0)
#define HI_VI_AF_IIR0_SHIFT2_DATASIZE (3)

// args: data (3-bit)
static __inline HI_VOID hi_vi_af_iir0_shift2_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12240, 10, 8, data);
}
static __inline HI_U32 hi_vi_af_iir0_shift2_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12240, 10, 8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_shift3
// ------------------------------------------------------------------------------ //
//  af iir0 shift3
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_SHIFT3_DEFAULT (0x1)
#define HI_VI_AF_IIR0_SHIFT3_DATASIZE (4)

// args: data (3-bit)
static __inline HI_VOID hi_vi_af_iir0_shift3_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12240, 14, 12, data);
}
static __inline HI_U32 hi_vi_af_iir0_shift3_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12240, 14, 12);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_shift0
// ------------------------------------------------------------------------------ //
//  af iir1 shift0
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_SHIFT0_DEFAULT (0x7)
#define HI_VI_AF_IIR1_SHIFT0_DATASIZE (3)

// args: data (3-bit)
static __inline HI_VOID hi_vi_af_iir1_shift0_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12240, 18, 16, data);
}
static __inline HI_U32 hi_vi_af_iir1_shift0_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12240, 18, 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_shift1
// ------------------------------------------------------------------------------ //
//  af iir1 shift1
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_SHIFT1_DEFAULT (0x2)
#define HI_VI_AF_IIR1_SHIFT1_DATASIZE (3)

// args: data (3-bit)
static __inline HI_VOID hi_vi_af_iir1_shift1_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12240, 22, 20, data);
}
static __inline HI_U32 hi_vi_af_iir1_shift1_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12240, 22, 20);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_shift2
// ------------------------------------------------------------------------------ //
//  af iir1 shift2
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_SHIFT2_DEFAULT (0x0)
#define HI_VI_AF_IIR1_SHIFT2_DATASIZE (3)

// args: data (3-bit)
static __inline HI_VOID hi_vi_af_iir1_shift2_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12240, 26, 24, data);
}
static __inline HI_U32 hi_vi_af_iir1_shift2_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12240, 26, 24);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_shift3
// ------------------------------------------------------------------------------ //
//  af iir1 shift3
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_SHIFT3_DEFAULT (0x1)
#define HI_VI_AF_IIR1_SHIFT3_DATASIZE (4)

// args: data (3-bit)
static __inline HI_VOID hi_vi_af_iir1_shift3_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12240, 30, 28, data);
}
static __inline HI_U32 hi_vi_af_iir1_shift3_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12240, 30, 28);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir0_h0
// ------------------------------------------------------------------------------ //
//  af fir0 h0
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR0_H0_DEFAULT (0x30)
#define HI_VI_AF_FIR0_H0_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_vi_af_fir0_h0_write(ISP_DEV IspDev, HI_U32 data) {

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12250, 5, 0, data);
}
static __inline HI_U32 hi_vi_af_fir0_h0_read(ISP_DEV IspDev) {

    return COMPLEMENT_ALIGN32(REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12250, 5, 0), 5);
}


// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir1_h0
// ------------------------------------------------------------------------------ //
//  af fir1 h0
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR1_H0_DEFAULT (0x30)
#define HI_VI_AF_FIR1_H0_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_vi_af_fir1_h0_write(ISP_DEV IspDev, HI_U32 data) {

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12250, 21, 16, data);
}
static __inline HI_U32 hi_vi_af_fir1_h0_read(ISP_DEV IspDev) {

    return COMPLEMENT_ALIGN32(REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12250, 21, 16), 5);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir0_h1
// ------------------------------------------------------------------------------ //
//  af fir0 h1
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR0_H1_DEFAULT (0x2B)
#define HI_VI_AF_FIR0_H1_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_vi_af_fir0_h1_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12254, 5, 0, data);
}
static __inline HI_U32 hi_vi_af_fir0_h1_read(ISP_DEV IspDev) {

    return COMPLEMENT_ALIGN32(REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12254, 5, 0), 5);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir1_h1
// ------------------------------------------------------------------------------ //
//  af fir1 h1
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR1_H1_DEFAULT (0x15)
#define HI_VI_AF_FIR1_H1_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_vi_af_fir1_h1_write(ISP_DEV IspDev, HI_U32 data) {

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12254, 21, 16, data);
}
static __inline HI_U32 hi_vi_af_fir1_h1_read(ISP_DEV IspDev) {

    return COMPLEMENT_ALIGN32(REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12254, 21, 16), 5);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir0_h2
// ------------------------------------------------------------------------------ //
//  af fir0 h2
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR0_H2_DEFAULT (0x0)
#define HI_VI_AF_FIR0_H2_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_vi_af_fir0_h2_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12258, 5, 0, data);
}
static __inline HI_U32 hi_vi_af_fir0_h2_read(ISP_DEV IspDev) {

    return COMPLEMENT_ALIGN32(REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12258, 5, 0), 5);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir1_h2
// ------------------------------------------------------------------------------ //
//  af fir1 h2
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR1_H2_DEFAULT (0)
#define HI_VI_AF_FIR1_H2_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_vi_af_fir1_h2_write(ISP_DEV IspDev, HI_U32 data) {

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12258, 21, 16, data);
}
static __inline HI_U32 hi_vi_af_fir1_h2_read(ISP_DEV IspDev) {

    return COMPLEMENT_ALIGN32(REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12258, 21, 16), 5);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir0_h3
// ------------------------------------------------------------------------------ //
//  af fir0 h3
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR0_H3_DEFAULT (0x15)
#define HI_VI_AF_FIR0_H3_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_vi_af_fir0_h3_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x1225c, 5, 0, data);

}
static __inline HI_U32 hi_vi_af_fir0_h3_read(ISP_DEV IspDev) {

    return COMPLEMENT_ALIGN32(REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x1225c, 5, 0), 5);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir1_h3
// ------------------------------------------------------------------------------ //
//  af fir1 h3
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR1_H3_DEFAULT (0x2B)
#define HI_VI_AF_FIR1_H3_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_vi_af_fir1_h3_write(ISP_DEV IspDev, HI_U32 data) {  

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x1225c, 21, 16, data);
}
static __inline HI_U32 hi_vi_af_fir1_h3_read(ISP_DEV IspDev) {

    return COMPLEMENT_ALIGN32(REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x1225c, 21, 16), 5);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir0_h4
// ------------------------------------------------------------------------------ //
//  af fir0 h4
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR0_H4_DEFAULT (0x10)
#define HI_VI_AF_FIR0_H4_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_vi_af_fir0_h4_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12260, 5, 0, data);
}
static __inline HI_U32 hi_vi_af_fir0_h4_read(ISP_DEV IspDev) {

    return COMPLEMENT_ALIGN32(REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12260, 5, 0), 5);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir1_h4
// ------------------------------------------------------------------------------ //
//  af fir1 h4
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR1_H4_DEFAULT (0x10)
#define HI_VI_AF_FIR1_H4_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_vi_af_fir1_h4_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12260, 21, 16, data);
}
static __inline HI_U32 hi_vi_af_fir1_h4_read(ISP_DEV IspDev) {

    return COMPLEMENT_ALIGN32(REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12260, 21, 16), 5);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_acc_shift_h0
// ------------------------------------------------------------------------------ //
//  af iir0 acc shift h
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_ACC_SHIFT_H0_DEFAULT (0)
#define HI_VI_AF_ACC_SHIFT_H0_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_acc_shift_h0_write(ISP_DEV IspDev, HI_U32 data) {

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12278, 3, 0, data);
}
static __inline HI_U32 hi_vi_af_acc_shift_h0_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12278, 3, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_acc_shift_h1
// ------------------------------------------------------------------------------ //
//  af iir1 acc shift h
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_ACC_SHIFT_H1_DEFAULT (0)
#define HI_VI_AF_ACC_SHIFT_H1_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_acc_shift_h1_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12278, 7, 4, data);
}
static __inline HI_U32 hi_vi_af_acc_shift_h1_read(ISP_DEV IspDev) {

    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12278, 7, 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_acc_shift_V0
// ------------------------------------------------------------------------------ //
//  af fir0 acc shift V
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_ACC_SHIFT_V0_DEFAULT (2)
#define HI_VI_AF_ACC_SHIFT_V0_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_acc_shift_v0_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12278, 11, 8, data);
}
static __inline HI_U32 hi_vi_af_acc_shift_v0_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12278, 11, 8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_acc_shift_V1
// ------------------------------------------------------------------------------ //
//  af fir1 acc shift V
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_ACC_SHIFT_V1_DEFAULT (0)
#define HI_VI_AF_ACC_SHIFT_V1_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_acc_shift_v1_write(ISP_DEV IspDev, HI_U32 data) {

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12278, 15, 12, data);
}
static __inline HI_U32 hi_vi_af_acc_shift_v1_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12278, 15, 12);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_acc_shift_y
// ------------------------------------------------------------------------------ //
//  af acc shift y
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_ACC_SHIFT_Y_DEFAULT (2)
#define HI_VI_AF_ACC_SHIFT_Y_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_acc_shift_y_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12278, 19, 16, data);
}
static __inline HI_U32 hi_vi_af_acc_shift_y_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12278, 19, 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_cnt_shift_h0
// ------------------------------------------------------------------------------ //
//  af iir0 cnt shift h
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_CNT_SHIFT_H0_DEFAULT (0)
#define HI_VI_AF_CNT_SHIFT_H0_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_cnt_shift_h0_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x1227c, 3, 0, data);
}
static __inline HI_U32 hi_vi_af_cnt_shift_h0_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x1227c, 3, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_cnt_shift_h1
// ------------------------------------------------------------------------------ //
//  af iir1 cnt shift h
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_CNT_SHIFT_H1_DEFAULT (0)
#define HI_VI_AF_CNT_SHIFT_H1_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_cnt_shift_h1_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x1227c, 7, 4, data);
}
static __inline HI_U32 hi_vi_af_cnt_shift_h1_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x1227c, 7, 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_cnt_shift_v0
// ------------------------------------------------------------------------------ //
//  af fir0 cnt shift v
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_CNT_SHIFT_V0_DEFAULT (2)
#define HI_VI_AF_CNT_SHIFT_V0_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_cnt_shift_v0_write(ISP_DEV IspDev, HI_U32 data) {

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x1227c, 11, 8, data);
}
static __inline HI_U32 hi_vi_af_cnt_shift_v0_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x1227c, 11, 8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_cnt_shift_v1
// ------------------------------------------------------------------------------ //
//  af fir1 cnt shift v
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_CNT_SHIFT_V1_DEFAULT (0)
#define HI_VI_AF_CNT_SHIFT_V1_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_cnt_shift_v1_write(ISP_DEV IspDev, HI_U32 data) {

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x1227c, 15, 12, data);
}
static __inline HI_U32 hi_vi_af_cnt_shift_v1_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x1227c, 15, 12);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_cnt_shift_highluma
// ------------------------------------------------------------------------------ //
//  af high luma cnt shift
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_CNT_SHIFT_HIGHLUMA_DEFAULT (0)
#define HI_VI_AF_CNT_SHIFT_HIGHLUMA_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_cnt_shift_highluma_write(ISP_DEV IspDev, HI_U32 data) {

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x1227c, 19, 16, data);
}
static __inline HI_U32 hi_vi_af_cnt_shift_highluma_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x1227c, 19, 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_update_mode
// ------------------------------------------------------------------------------ //
//  af update mode
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_UPDATE_MODE_DEFAULT (0x1)
#define HI_VI_AF_UPDATE_MODE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_update_mode_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x122E4, 0, 0, data);

}
static __inline HI_U32 hi_vi_af_update_mode_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x122E4, 0, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_update
// ------------------------------------------------------------------------------ //
//  af update
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_UPDATE_DEFAULT (0x0)
#define HI_VI_AF_UPDATE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_update_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x122EC, 0, 0, data);

}
static __inline HI_U32 hi_vi_af_update_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x122EC, 0, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_hsize
// ------------------------------------------------------------------------------ //
//  af image horiz size
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_HSIZE_DEFAULT (0x77f)
#define HI_VI_AF_HSIZE_DATASIZE (13)

// args: data (13-bit)
static __inline HI_VOID hi_vi_af_hsize_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x122f0, 12, 0, data);

}
static __inline HI_U32 hi_vi_af_hsize_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x122f0, 12, 0);
}


// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_vsize
// ------------------------------------------------------------------------------ //
//  af image verti size
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_VSIZE_DEFAULT (0x437)
#define HI_VI_AF_VSIZE_DATASIZE (13)

// args: data (13-bit)
static __inline HI_VOID hi_vi_af_vsize_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x122f0, 28, 16, data);

}
static __inline HI_U32 hi_vi_af_vsize_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x122f0, 28, 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_ldg_th_l
// ------------------------------------------------------------------------------ //
//  af iir0 ldg threshold low
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_LDG_TH_L_DEFAULT (0x0)
#define HI_VI_AF_IIR0_LDG_TH_L_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_iir0_ldg_th_l_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12300, 7, 0, data);

}
static __inline HI_U32 hi_vi_af_iir0_ldg_th_l_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12300, 7, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_ldg_th_h
// ------------------------------------------------------------------------------ //
//  af iir0 ldg threshold high
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_LDG_TH_H_DEFAULT (0x0)
#define HI_VI_AF_IIR0_LDG_TH_H_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_iir0_ldg_th_h_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12300, 15, 8, data);

}
static __inline HI_U32 hi_vi_af_iir0_ldg_th_h_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12300, 15, 8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_ldg_th_l
// ------------------------------------------------------------------------------ //
//  af iir1 ldg threshold low
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_LDG_TH_L_DEFAULT (0x0)
#define HI_VI_AF_IIR1_LDG_TH_L_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_iir1_ldg_th_l_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12300, 23, 16, data);

}
static __inline HI_U32 hi_vi_af_iir1_ldg_th_l_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12300, 23, 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_ldg_th_h
// ------------------------------------------------------------------------------ //
//  af iir1 ldg threshold high
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_LDG_TH_H_DEFAULT (0x0)
#define HI_VI_AF_IIR1_LDG_TH_H_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_iir1_ldg_th_h_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12300, 31, 24, data);

}
static __inline HI_U32 hi_vi_af_iir1_ldg_th_h_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12300, 31, 24);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_ldg_gain_l
// ------------------------------------------------------------------------------ //
//  af iir0 ldg gain low
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_LDG_GAIN_L_DEFAULT (0x0)
#define HI_VI_AF_IIR0_LDG_GAIN_L_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_iir0_ldg_gain_l_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12304, 7, 0, data);

}
static __inline HI_U32 hi_vi_af_iir0_ldg_gain_l_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12304, 7, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_ldg_gain_h
// ------------------------------------------------------------------------------ //
//  af iir0 ldg gain high
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_LDG_GAIN_H_DEFAULT (0x0)
#define HI_VI_AF_IIR0_LDG_GAIN_H_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_iir0_ldg_gain_h_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12304, 15, 8, data);

}
static __inline HI_U32 hi_vi_af_iir0_ldg_gain_h_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12304, 15, 8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_ldg_gain_l
// ------------------------------------------------------------------------------ //
//  af iir1 ldg gain low
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_LDG_GAIN_L_DEFAULT (0x0)
#define HI_VI_AF_IIR1_LDG_GAIN_L_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_iir1_ldg_gain_l_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12304, 23, 16, data);

}
static __inline HI_U32 hi_vi_af_iir1_ldg_gain_l_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12304, 23, 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_ldg_gain_h
// ------------------------------------------------------------------------------ //
//  af iir1 ldg gain high
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_LDG_GAIN_H_DEFAULT (0x0)
#define HI_VI_AF_IIR1_LDG_GAIN_H_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_iir1_ldg_gain_h_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12304, 31, 24, data);

}
static __inline HI_U32 hi_vi_af_iir1_ldg_gain_h_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12304, 31, 24);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_ldg_slp_l
// ------------------------------------------------------------------------------ //
//  af iir0 ldg slope low
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_LDG_SLP_L_DEFAULT (0x0)
#define HI_VI_AF_IIR0_LDG_SLP_L_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_iir0_ldg_slp_l_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12308, 3, 0, data);

}
static __inline HI_U32 hi_vi_af_iir0_ldg_slp_l_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12308, 3, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_ldg_slp_h
// ------------------------------------------------------------------------------ //
//  af iir0 ldg slope high
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_LDG_SLP_H_DEFAULT (0x0)
#define HI_VI_AF_IIR0_LDG_SLP_H_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_iir0_ldg_slp_h_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12308, 7, 4, data);

}
static __inline HI_U32 hi_vi_af_iir0_ldg_slp_h_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12308, 7, 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_ldg_slp_l
// ------------------------------------------------------------------------------ //
//  af iir1 ldg slope low
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_LDG_SLP_L_DEFAULT (0x0)
#define HI_VI_AF_IIR1_LDG_SLP_L_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_iir1_ldg_slp_l_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12308, 19, 16, data);

}
static __inline HI_U32 hi_vi_af_iir1_ldg_slp_l_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12308, 19, 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_ldg_slp_h
// ------------------------------------------------------------------------------ //
//  af iir1 ldg slope high
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_LDG_SLP_H_DEFAULT (0x0)
#define HI_VI_AF_IIR1_LDG_SLP_H_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_iir1_ldg_slp_h_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12308, 23, 20, data);

}
static __inline HI_U32 hi_vi_af_iir1_ldg_slp_h_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12308, 23, 20);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_dilate
// ------------------------------------------------------------------------------ //
//  af iir0 dilate
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_DILATE_DEFAULT (0x0)
#define HI_VI_AF_IIR0_DILATE_DATASIZE (3)

// args: data (3-bit)
static __inline HI_VOID hi_vi_af_iir0_dilate_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x1230c, 2, 0, data);

}
static __inline HI_U32 hi_vi_af_iir0_dilate_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x1230c, 2, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_dilate
// ------------------------------------------------------------------------------ //
//  af iir1 dilate
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_DILATE_DEFAULT (0x0)
#define HI_VI_AF_IIR1_DILATE_DATASIZE (3)

// args: data (3-bit)
static __inline HI_VOID hi_vi_af_iir1_dilate_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x1230c, 10, 8, data);

}
static __inline HI_U32 hi_vi_af_iir1_dilate_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x1230c, 10, 8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir0_ldg_th_l
// ------------------------------------------------------------------------------ //
//  af fir0 ldg threshold low
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR0_LDG_TH_L_DEFAULT (0x0)
#define HI_VI_AF_FIR0_LDG_TH_L_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_fir0_ldg_th_l_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12310, 7, 0, data);

}
static __inline HI_U32 hi_vi_af_fir0_ldg_th_l_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12310, 7, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir0_ldg_th_h
// ------------------------------------------------------------------------------ //
//  af fir0 ldg threshold high
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR0_LDG_TH_H_DEFAULT (0x0)
#define HI_VI_AF_FIR0_LDG_TH_H_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_fir0_ldg_th_h_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12310, 15, 8, data);

}
static __inline HI_U32 hi_vi_af_fir0_ldg_th_h_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12310, 15, 8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir1_ldg_th_l
// ------------------------------------------------------------------------------ //
//  af fir1 ldg threshold low
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR1_LDG_TH_L_DEFAULT (0x0)
#define HI_VI_AF_FIR1_LDG_TH_L_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_fir1_ldg_th_l_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12310, 23, 16, data);

}
static __inline HI_U32 hi_vi_af_fir1_ldg_th_l_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12310, 23, 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir1_ldg_th_h
// ------------------------------------------------------------------------------ //
//  af fir1 ldg threshold high
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR1_LDG_TH_H_DEFAULT (0x0)
#define HI_VI_AF_FIR1_LDG_TH_H_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_fir1_ldg_th_h_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12310, 31, 24, data);

}
static __inline HI_U32 hi_vi_af_fir1_ldg_th_h_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12310, 31, 24);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir0_ldg_gain_l
// ------------------------------------------------------------------------------ //
//  af fir0 ldg gain low
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR0_LDG_GAIN_L_DEFAULT (0x0)
#define HI_VI_AF_FIR0_LDG_GAIN_L_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_fir0_ldg_gain_l_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12314, 7, 0, data);

}
static __inline HI_U32 hi_vi_af_fir0_ldg_gain_l_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12314, 7, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir0_ldg_gain_h
// ------------------------------------------------------------------------------ //
//  af fir0 ldg gain high
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR0_LDG_GAIN_H_DEFAULT (0x0)
#define HI_VI_AF_FIR0_LDG_GAIN_H_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_fir0_ldg_gain_h_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12314, 15, 8, data);

}
static __inline HI_U32 hi_vi_af_fir0_ldg_gain_h_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12314, 15, 8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir1_ldg_gain_l
// ------------------------------------------------------------------------------ //
//  af fir1 ldg gain low
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_LDG_GAIN_L_DEFAULT (0x0)
#define HI_VI_AF_IIR1_LDG_GAIN_L_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_fir1_ldg_gain_l_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12314, 23, 16, data);

}
static __inline HI_U32 hi_vi_af_fir1_ldg_gain_l_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12314, 23, 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir1_ldg_gain_h
// ------------------------------------------------------------------------------ //
//  af iir1 ldg gain high
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR1_LDG_GAIN_H_DEFAULT (0x0)
#define HI_VI_AF_FIR1_LDG_GAIN_H_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_fir1_ldg_gain_h_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12314, 31, 24, data);

}
static __inline HI_U32 hi_vi_af_fir1_ldg_gain_h_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12314, 31, 24);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir0_ldg_slp_l
// ------------------------------------------------------------------------------ //
//  af fir0 ldg slope low
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR0_LDG_SLP_L_DEFAULT (0x0)
#define HI_VI_AF_FIR0_LDG_SLP_L_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_fir0_ldg_slp_l_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12318, 3, 0, data);

}
static __inline HI_U32 hi_vi_af_fir0_ldg_slp_l_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12318, 3, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir0_ldg_slp_h
// ------------------------------------------------------------------------------ //
//  af fir0 ldg slope high
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR0_LDG_SLP_H_DEFAULT (0x0)
#define HI_VI_AF_FIR0_LDG_SLP_H_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_fir0_ldg_slp_h_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12318, 7, 4, data);

}
static __inline HI_U32 hi_vi_af_fir0_ldg_slp_h_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12318, 7, 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir1_ldg_slp_l
// ------------------------------------------------------------------------------ //
//  af fir1 ldg slope low
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR1_LDG_SLP_L_DEFAULT (0x0)
#define HI_VI_AF_FIR1_LDG_SLP_L_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_fir1_ldg_slp_l_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12318, 19, 16, data);

}
static __inline HI_U32 hi_vi_af_fir1_ldg_slp_l_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12318, 19, 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir1_ldg_slp_h
// ------------------------------------------------------------------------------ //
//  af fir1 ldg slope high
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR1_LDG_SLP_H_DEFAULT (0x0)
#define HI_VI_AF_FIR1_LDG_SLP_H_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_fir1_ldg_slp_h_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12318, 23, 20, data);

}
static __inline HI_U32 hi_vi_af_fir1_ldg_slp_h_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12318, 23, 20);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_cor_th
// ------------------------------------------------------------------------------ //
//  af iir0 coring threshold
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_COR_TH_DEFAULT (0x0)
#define HI_VI_AF_IIR0_COR_TH_DATASIZE (11)

// args: data (11-bit)
static __inline HI_VOID hi_vi_af_iir0_cor_th_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12320, 10, 0, data);

}
static __inline HI_U32 hi_vi_af_iir0_cor_th_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12320, 10, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_cor_th
// ------------------------------------------------------------------------------ //
//  af iir1 coring threshold
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_COR_TH_DEFAULT (0x0)
#define HI_VI_AF_IIR1_COR_TH_DATASIZE (11)

// args: data (11-bit)
static __inline HI_VOID hi_vi_af_iir1_cor_th_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12320, 26, 16, data);

}
static __inline HI_U32 hi_vi_af_iir1_cor_th_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12320, 26, 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_cor_peak
// ------------------------------------------------------------------------------ //
//  af iir0 coring peak
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_COR_PEAK_DEFAULT (0x0)
#define HI_VI_AF_IIR0_COR_PEAK_DATASIZE (11)

// args: data (11-bit)
static __inline HI_VOID hi_vi_af_iir0_cor_peak_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12324, 10, 0, data);

}
static __inline HI_U32 hi_vi_af_iir0_cor_peak_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12324, 10, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_cor_peak
// ------------------------------------------------------------------------------ //
//  af iir1 coring peak
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_COR_PEAK_DEFAULT (0x0)
#define HI_VI_AF_IIR1_COR_PEAK_DATASIZE (11)

// args: data (11-bit)
static __inline HI_VOID hi_vi_af_iir1_cor_peak_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12324, 26, 16, data);

}
static __inline HI_U32 hi_vi_af_iir1_cor_peak_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12324, 26, 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_cor_slp
// ------------------------------------------------------------------------------ //
//  af iir0 coring slope
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_COR_SLP_DEFAULT (0x0)
#define HI_VI_AF_IIR0_COR_SLP_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_iir0_cor_slp_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12328, 3, 0, data);

}
static __inline HI_U32 hi_vi_af_iir0_cor_slp_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12328, 3, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_cor_slp
// ------------------------------------------------------------------------------ //
//  af iir1 coring slope
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_COR_SLP_DEFAULT (0x0)
#define HI_VI_AF_IIR1_COR_SLP_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_iir1_cor_slp_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12328, 11, 8, data);

}
static __inline HI_U32 hi_vi_af_iir1_cor_slp_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12328, 11, 8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir0_cor_th
// ------------------------------------------------------------------------------ //
//  af fir0 coring threshold
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR0_COR_TH_DEFAULT (0x0)
#define HI_VI_AF_FIR0_COR_TH_DATASIZE (11)

// args: data (11-bit)
static __inline HI_VOID hi_vi_af_fir0_cor_th_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12330, 10, 0, data);

}
static __inline HI_U32 hi_vi_af_fir0_cor_th_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12330, 10, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir1_cor_th
// ------------------------------------------------------------------------------ //
//  af fir1 coring threshold
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR1_COR_TH_DEFAULT (0x0)
#define HI_VI_AF_FIR1_COR_TH_DATASIZE (11)

// args: data (11-bit)
static __inline HI_VOID hi_vi_af_fir1_cor_th_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12330, 26, 16, data);

}
static __inline HI_U32 hi_vi_af_fir1_cor_th_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12330, 26, 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir0_cor_peak
// ------------------------------------------------------------------------------ //
//  af fir0 coring peak
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR0_COR_PEAK_DEFAULT (0x0)
#define HI_VI_AF_FIR0_COR_PEAK_DATASIZE (11)

// args: data (11-bit)
static __inline HI_VOID hi_vi_af_fir0_cor_peak_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12334, 10, 0, data);

}
static __inline HI_U32 hi_vi_af_fir0_cor_peak_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12334, 10, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir1_cor_peak
// ------------------------------------------------------------------------------ //
//  af fir1 coring peak
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR1_COR_PEAK_DEFAULT (0x0)
#define HI_VI_AF_FIR1_COR_PEAK_DATASIZE (11)

// args: data (11-bit)
static __inline HI_VOID hi_vi_af_fir1_cor_peak_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12334, 26, 16, data);

}
static __inline HI_U32 hi_vi_af_fir1_cor_peak_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12334, 26, 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir0_cor_slp
// ------------------------------------------------------------------------------ //
//  af fir0 coring slope
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR0_COR_SLP_DEFAULT (0x0)
#define HI_VI_AF_FIR0_COR_SLP_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_fir0_cor_slp_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12338, 3, 0, data);

}
static __inline HI_U32 hi_vi_af_fir0_cor_slp_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12338, 3, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir1_cor_slp
// ------------------------------------------------------------------------------ //
//  af fir1 coring slope
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR1_COR_SLP_DEFAULT (0x0)
#define HI_VI_AF_FIR1_COR_SLP_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_fir1_cor_slp_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12338, 11, 8, data);

}
static __inline HI_U32 hi_vi_af_fir1_cor_slp_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12338, 11, 8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_hiluma_th
// ------------------------------------------------------------------------------ //
//  af high luma cnt threshold
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_HIGH_LUMA_TH_DEFAULT (0x0)
#define HI_VI_AF_HIGH_LUMA_TH_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_highluma_th_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12340, 7, 0, data);

}
static __inline HI_U32 hi_vi_af_highluma_th_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12340, 7, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_offset_gr
// ------------------------------------------------------------------------------ //
//  af offset gr when measure at raw domain
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_OFFSET_GR_DEFAULT (0x0)
#define HI_VI_AF_OFFSET_GR_DATASIZE (14)

// args: data (14-bit)
static __inline HI_VOID hi_vi_af_offset_gr_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12344, 13, 0, data);

}
static __inline HI_U32 hi_vi_af_offset_gr_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12344, 13, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_offset_gb
// ------------------------------------------------------------------------------ //
//  af offset gb when measure at raw domain
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_OFFSET_GB_DEFAULT (0x0)
#define HI_VI_AF_OFFSET_GB_DATASIZE (14)

// args: data (14-bit)
static __inline HI_VOID hi_vi_af_offset_gb_write(ISP_DEV IspDev, HI_U32 data) { 

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12344, 29, 16, data);

}
static __inline HI_U32 hi_vi_af_offset_gb_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x12344, 29, 16);
}

// ------------------------------------------------------------------------------ //
// Group: YUV domain sharpen cfg
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Sharpen
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: Enable
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Sharpening cfg enable: 0=off, 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_SHARPEN_CFG_ENABLE_DEFAULT (0x0)
#define HI_ISP_SHARPEN_CFG_ENABLE_DATASIZE (1)

// args: data (1-bit)

static __inline HI_VOID hi_isp_sharpen_cfg_enable_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U32 curr = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25200);    
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25200, (data & 0x1) | (curr & 0xfffffffe));
}

static __inline HI_U8 hi_isp_sharpen_cfg_enable_read(ISP_DEV IspDev) {
    return (HI_U8)(IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25200) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_edge_amt
// ------------------------------------------------------------------------------ //
//  Sharpen strength
// ------------------------------------------------------------------------------ //

#define HI_ISP_EDGE_AMT_DEFAULT (0x0)
#define HI_ISP_EDGE_AMT_DATASIZE (12)

// args: data (12-bit)

static __inline HI_VOID hi_isp_edge_amt_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25212, (data & 0xfff));
}

static __inline HI_U16 hi_isp_edge_amt_read(ISP_DEV IspDev) {
    return (IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25212) & 0xfff);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_edge_amt
// ------------------------------------------------------------------------------ //
//  Sharpen strength
// ------------------------------------------------------------------------------ //

#define HI_ISP_SHARP_AMT_DEFAULT (0x0)
#define HI_ISP_SHARP_AMT_DATASIZE (12)

// args: data (12-bit)

static __inline HI_VOID hi_isp_sharp_amt_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25210, (data & 0xfff));
}

static __inline HI_U16 hi_isp_sharp_amt_read(ISP_DEV IspDev) {
    return (IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25210) & 0xfff);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_edge_thd1/hi_isp_sharp_thd1
// ------------------------------------------------------------------------------ //
//  Sharpen threshold
// ------------------------------------------------------------------------------ //

#define HI_ISP_EDGE_THD1_DEFAULT (0x0)
#define HI_ISP_EDGE_THD1_DATASIZE (10)

// args: data (10-bit)

static __inline HI_VOID hi_isp_edge_thd1_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25216, (data & 0x3ff));
}

static __inline HI_U16 hi_isp_edge_thd1_read(ISP_DEV IspDev) {
    return (IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25216) & 0x3ff);
}

static __inline HI_VOID hi_isp_sharp_thd1_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25214, (data & 0x3ff));
}

static __inline HI_U16 hi_isp_sharp_thd1_read(ISP_DEV IspDev) {
    return (IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25214) & 0x3ff);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_edge_coef/hi_isp_sharp_coef
// ------------------------------------------------------------------------------ //
//  Sharpen coef
// ------------------------------------------------------------------------------ //

// args: data (12-bit)

static __inline HI_VOID hi_isp_edge_coef_write(ISP_DEV IspDev, HI_U16 data) {
	IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2521a, (data & 0xfff));
}

static __inline HI_U16 hi_isp_edge_coef_read(ISP_DEV IspDev) {
	return (IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2521a) & 0xfff);
}

static __inline HI_VOID hi_isp_sharp_coef_write(ISP_DEV IspDev, HI_U16 data) {
	IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25218, (data & 0xfff));
}

static __inline HI_U16 hi_isp_sharp_coef_read(ISP_DEV IspDev) {
	return (IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25218) & 0xfff);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_over_amt/hi_isp_under_amt
// ------------------------------------------------------------------------------ //
//  Sharpen over/under amt
// ------------------------------------------------------------------------------ //

// args: data (8-bit)

static __inline HI_VOID hi_isp_over_amt_write(ISP_DEV IspDev, HI_U8 data) {
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2521e, data);
}

static __inline HI_U8 hi_isp_over_amt_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2521e));
}

static __inline HI_VOID hi_isp_under_amt_write(ISP_DEV IspDev, HI_U8 data) {
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2521c, data);
}

static __inline HI_U8 hi_isp_under_amt_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2521c));
}

static __inline HI_VOID hi_isp_sharpen_varjagthd2_write(ISP_DEV IspDev, HI_U16 data) {
	IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25222, data & 0x3FF);
}

static __inline HI_U16 hi_isp_sharpen_varjagthd2_read(ISP_DEV IspDev) {
	return (IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25222) & 0x3FF);
}

static __inline HI_VOID hi_isp_sharpen_varjagthd1_write(ISP_DEV IspDev, HI_U16 data) {
	IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25220, data & 0x3FF);
}

static __inline HI_U16 hi_isp_sharpen_varjagthd1_read(ISP_DEV IspDev) {
	return (IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25220) & 0x3FF);
}

static __inline HI_VOID hi_isp_sharpen_edgejagmulcoef_write(ISP_DEV IspDev, HI_U16 data) {
	IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25226, data & 0x1FFF);
}

static __inline HI_U16 hi_isp_sharpen_edgejagmulcoef_read(ISP_DEV IspDev) {
	return (IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25226) & 0x1FFF);
}

static __inline HI_VOID hi_isp_sharpen_edgejagamt_write(ISP_DEV IspDev, HI_U16 data) {
	IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25224, data & 0xFFF);
}

static __inline HI_U16 hi_isp_sharpen_edgejagamt_read(ISP_DEV IspDev) {
	return (IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25224) & 0xFFF);
}

static __inline HI_VOID hi_isp_sharpen_oshtjagmulcoef_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2522a, data & 0x1FFF);
}

static __inline HI_U16 hi_isp_sharpen_oshtjagmulcoef_read(ISP_DEV IspDev) {
    return (IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2522a) & 0x1FFF);
}

static __inline HI_VOID hi_isp_sharpen_oshtjagamt_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25228, data);
}

static __inline HI_U8 hi_isp_sharpen_oshtjagamt_read(ISP_DEV IspDev) {
    return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25228));
}

static __inline HI_VOID hi_isp_sharpen_ushtjagmulcoef_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2522e, data & 0x1FFF);
}

static __inline HI_U16 hi_isp_sharpen_ushtjagmulcoef_read(ISP_DEV IspDev) {
    return (IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2522e) & 0x1FFF);
}

static __inline HI_VOID hi_isp_sharpen_ushtjagamt_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2522c, data);
}

static __inline HI_U8 hi_isp_sharpen_ushtjagamt_read(ISP_DEV IspDev) {
    return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2522c));
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_sharpen_mid_tmp02/tmp01/tmp00
// ------------------------------------------------------------------------------ //
//  Sharpen Medfilter coef
// ------------------------------------------------------------------------------ //

// args: data (8-bit)

static __inline HI_VOID hi_isp_sharpen_mid_tmp02_write(ISP_DEV IspDev, HI_U8 data) {
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25232, data);
}

static __inline HI_U8 hi_isp_sharpen_mid_tmp02_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25232));
}

static __inline HI_VOID hi_isp_sharpen_mid_tmp01_write(ISP_DEV IspDev, HI_U8 data) {
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25231, data);
}

static __inline HI_U8 hi_isp_sharpen_mid_tmp01_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25231));
}

static __inline HI_VOID hi_isp_sharpen_mid_tmp00_write(ISP_DEV IspDev, HI_U8 data) {
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25230, data);
}

static __inline HI_U8 hi_isp_sharpen_mid_tmp00_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25230));
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_sharpen_mid_tmp12/tmp11/tmp10
// ------------------------------------------------------------------------------ //
//  Sharpen Medfilter coef
// ------------------------------------------------------------------------------ //

// args: data (8-bit)

static __inline HI_VOID hi_isp_sharpen_mid_tmp12_write(ISP_DEV IspDev, HI_U8 data) {
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25236, data);
}

static __inline HI_U8 hi_isp_sharpen_mid_tmp12_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25236));
}

static __inline HI_VOID hi_isp_sharpen_mid_tmp11_write(ISP_DEV IspDev, HI_U8 data) {
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25235, data);
}

static __inline HI_U8 hi_isp_sharpen_mid_tmp11_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25235));
}

static __inline HI_VOID hi_isp_sharpen_mid_tmp10_write(ISP_DEV IspDev, HI_U8 data) {
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25234, data);
}

static __inline HI_U8 hi_isp_sharpen_mid_tmp10_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25234));
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_sharpen_mid_tmp22/tmp21/tmp20
// ------------------------------------------------------------------------------ //
//  Sharpen Medfilter coef
// ------------------------------------------------------------------------------ //

// args: data (8-bit)

static __inline HI_VOID hi_isp_sharpen_mid_tmp22_write(ISP_DEV IspDev, HI_U8 data) {
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2523a, data);
}

static __inline HI_U8 hi_isp_sharpen_mid_tmp22_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2523a));
}

static __inline HI_VOID hi_isp_sharpen_mid_tmp21_write(ISP_DEV IspDev, HI_U8 data) {
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25239, data);
}

static __inline HI_U8 hi_isp_sharpen_mid_tmp21_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25239));
}

static __inline HI_VOID hi_isp_sharpen_mid_tmp20_write(ISP_DEV IspDev, HI_U8 data) {
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25238, data);
}

static __inline HI_U8 hi_isp_sharpen_mid_tmp20_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25238));
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_sharpen_line_thd2/thd1
// ------------------------------------------------------------------------------ //
//  Sharpen line detection thd
// ------------------------------------------------------------------------------ //

// args: data (8-bit)

static __inline HI_VOID hi_isp_sharpen_line_thd2_write(ISP_DEV IspDev, HI_U8 data) {
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2523e, data);
}

static __inline HI_U8 hi_isp_sharpen_line_thd2_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2523e));
}

static __inline HI_VOID hi_isp_sharpen_line_thd1_write(ISP_DEV IspDev, HI_U8 data) {
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2523c, data);
}

static __inline HI_U8 hi_isp_sharpen_line_thd1_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2523c));
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_edge_thd2/hi_isp_sharp_thd2
// ------------------------------------------------------------------------------ //
//  Sharpen thd
// ------------------------------------------------------------------------------ //

// args: data (10-bit)

static __inline HI_VOID hi_isp_edge_thd2_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25242, (data & 0x3ff));
}

static __inline HI_U16 hi_isp_edge_thd2_read(ISP_DEV IspDev) {
    return (IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25242) & 0x3ff);
}

static __inline HI_VOID hi_isp_sharp_thd2_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25240, (data & 0x3ff));
}

static __inline HI_U16 hi_isp_sharp_thd2_read(ISP_DEV IspDev) {
    return (IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25240) & 0x3ff);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_sharpen_jagctrl
// ------------------------------------------------------------------------------ //
//  Sharpen crtl
// ------------------------------------------------------------------------------ //

// args: data (1-bit)

static __inline HI_VOID hi_isp_sharpen_jagctrl_write(ISP_DEV IspDev, HI_U8 data) {
	HI_U8 curr = IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25244);
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25244, ((data & 0x1)  << 4) | (curr & 0xef));
}

static __inline HI_U8 hi_isp_sharpen_jagctrl_read(ISP_DEV IspDev) {
	return ((IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25244) & 0x10) >> 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_sharpen_shtlumamod
// ------------------------------------------------------------------------------ //
//  Sharpen crtl
// ------------------------------------------------------------------------------ //

// args: data (1-bit)

static __inline HI_VOID hi_isp_sharpen_shtlumamod_write(ISP_DEV IspDev, HI_U8 data) {
	HI_U8 curr = IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25244);
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25244, ((data & 0x1)  << 3) | (curr & 0xf7));
}

static __inline HI_U8 hi_isp_sharpen_shtlumamod_read(ISP_DEV IspDev) {
	return ((IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25244) & 0x8) >> 3);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_sharpen_shtvarctrl
// ------------------------------------------------------------------------------ //
//  Sharpen crtl
// ------------------------------------------------------------------------------ //

// args: data (1-bit)

static __inline HI_VOID hi_isp_sharpen_shtvarctrl_write(ISP_DEV IspDev, HI_U8 data) {
	HI_U8 curr = IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25244);
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25244, ((data & 0x1)  << 2) | (curr & 0xfb));
}

static __inline HI_U8 hi_isp_sharpen_shtvarctrl_read(ISP_DEV IspDev) {
	return ((IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25244) & 0x4) >> 2);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_sharpen_lumactrl
// ------------------------------------------------------------------------------ //
//  Sharpen crtl
// ------------------------------------------------------------------------------ //

// args: data (1-bit)

static __inline HI_VOID hi_isp_sharpen_lumactrl_write(ISP_DEV IspDev, HI_U8 data) {
	HI_U8 curr = IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25244);
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25244, ((data & 0x1)  << 1) | (curr & 0xfd));
}

static __inline HI_U8 hi_isp_sharpen_lumactrl_read(ISP_DEV IspDev) {
	return ((IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25244) & 0x2) >> 1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_sharpen_enpixsel
// ------------------------------------------------------------------------------ //
//  Sharpen crtl
// ------------------------------------------------------------------------------ //

// args: data (1-bit)

static __inline HI_VOID hi_isp_sharpen_enpixsel_write(ISP_DEV IspDev, HI_U8 data) {
	HI_U8 curr = IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25244);
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25244, ((data & 0x1)  << 0) | (curr & 0xfe));
}

static __inline HI_U8 hi_isp_sharpen_enpixsel_read(ISP_DEV IspDev) {
	return ((IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25244) & 0x1) >> 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_sharpen_lumath3/lumath2/lumath1/lumath0
// ------------------------------------------------------------------------------ //
//  Sharpen luma crtl thd
// ------------------------------------------------------------------------------ //

// args: data (8-bit)

static __inline HI_VOID hi_isp_sharpen_lumath3_write(ISP_DEV IspDev, HI_U8 data) {
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2524b, data);
}

static __inline HI_U8 hi_isp_sharpen_lumath3_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2524b));
}

static __inline HI_VOID hi_isp_sharpen_lumath2_write(ISP_DEV IspDev, HI_U8 data) {
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2524a, data);
}

static __inline HI_U8 hi_isp_sharpen_lumath2_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2524a));
}

static __inline HI_VOID hi_isp_sharpen_lumath1_write(ISP_DEV IspDev, HI_U8 data) {
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25249, data);
}

static __inline HI_U8 hi_isp_sharpen_lumath1_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25249));
}

static __inline HI_VOID hi_isp_sharpen_lumath0_write(ISP_DEV IspDev, HI_U8 data) {
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25248, data);
}

static __inline HI_U8 hi_isp_sharpen_lumath0_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25248));
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_sharpen_lumawgt3/lumawgt2/lumawgt1/lumawgt0
// ------------------------------------------------------------------------------ //
//  Sharpen luma crtl weight
// ------------------------------------------------------------------------------ //

// args: data (8-bit)

static __inline HI_VOID hi_isp_sharpen_lumawgt3_write(ISP_DEV IspDev, HI_U8 data) {
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2524f, data);
}

static __inline HI_U8 hi_isp_sharpen_lumawgt3_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2524f));
}

static __inline HI_VOID hi_isp_sharpen_lumawgt2_write(ISP_DEV IspDev, HI_U8 data) {
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2524e, data);
}

static __inline HI_U8 hi_isp_sharpen_lumawgt2_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2524e));
}

static __inline HI_VOID hi_isp_sharpen_lumawgt1_write(ISP_DEV IspDev, HI_U8 data) {
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2524d, data);
}

static __inline HI_U8 hi_isp_sharpen_lumawgt1_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2524d));
}

static __inline HI_VOID hi_isp_sharpen_lumawgt0_write(ISP_DEV IspDev, HI_U8 data) {
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2524c, data);
}

static __inline HI_U8 hi_isp_sharpen_lumawgt0_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2524c));
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_sharpen_lumamul2/lumamul1/lumamul0
// ------------------------------------------------------------------------------ //
//  Sharpen luma crtl coef
// ------------------------------------------------------------------------------ //

// args: data (13-bit)

static __inline HI_VOID hi_isp_sharpen_lumamul2_write(ISP_DEV IspDev, HI_U16 data) {
	IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25254, data & 0x1fff);
}

static __inline HI_U16 hi_isp_sharpen_lumamul2_read(ISP_DEV IspDev) {
	return (IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25254) & 0x1fff);
}

static __inline HI_VOID hi_isp_sharpen_lumamul1_write(ISP_DEV IspDev, HI_U16 data) {
	IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25252, data & 0x1fff);
}

static __inline HI_U16 hi_isp_sharpen_lumamul1_read(ISP_DEV IspDev) {
	return (IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25252) & 0x1fff);
}

static __inline HI_VOID hi_isp_sharpen_lumamul0_write(ISP_DEV IspDev, HI_U16 data) {
	IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25250, data & 0x1fff);
}

static __inline HI_U16 hi_isp_sharpen_lumamul0_read(ISP_DEV IspDev) {
	return (IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25250) & 0x1fff);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_sharpen_overvarth1/overvarth0
// ------------------------------------------------------------------------------ //
//  Sharpen overvar thd
// ------------------------------------------------------------------------------ //

// args: data (10-bit)

static __inline HI_VOID hi_isp_sharpen_overvarth1_write(ISP_DEV IspDev, HI_U16 data) {
	IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2525a, data & 0x3ff);
}

static __inline HI_U16 hi_isp_sharpen_overvarth1_read(ISP_DEV IspDev) {
	return (IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2525a) & 0x3ff);
}

static __inline HI_VOID hi_isp_sharpen_overvarth0_write(ISP_DEV IspDev, HI_U16 data) {
	IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25258, data & 0x3ff);
}

static __inline HI_U16 hi_isp_sharpen_overvarth0_read(ISP_DEV IspDev) {
	return (IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25258) & 0x3ff);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_sharpen_overvarmul
// ------------------------------------------------------------------------------ //
//  Sharpen overvarmul
// ------------------------------------------------------------------------------ //

// args: data (13-bit)

static __inline HI_VOID hi_isp_sharpen_overvarmul_write(ISP_DEV IspDev, HI_U16 data) {
	IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2525e, data & 0x1fff);
}

static __inline HI_U16 hi_isp_sharpen_overvarmul_read(ISP_DEV IspDev) {
	return (IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2525e) & 0x1fff);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_sharpen_overvaramt
// ------------------------------------------------------------------------------ //
//  Sharpen overvar amt
// ------------------------------------------------------------------------------ //

// args: data (8-bit)

static __inline HI_VOID hi_isp_sharpen_overvaramt_write(ISP_DEV IspDev, HI_U8 data) {
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2525c, data);
}

static __inline HI_U8 hi_isp_sharpen_overvaramt_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2525c));
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_sharpen_undervarth1/undervarth0
// ------------------------------------------------------------------------------ //
//  Sharpen undervar thd
// ------------------------------------------------------------------------------ //

// args: data (10-bit)

static __inline HI_VOID hi_isp_sharpen_undervarth1_write(ISP_DEV IspDev, HI_U16 data) {
	IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25262, data & 0x3ff);
}

static __inline HI_U16 hi_isp_sharpen_undervarth1_read(ISP_DEV IspDev) {
	return (IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25262) & 0x3ff);
}

static __inline HI_VOID hi_isp_sharpen_undervarth0_write(ISP_DEV IspDev, HI_U16 data) {
	IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25260, data & 0x3ff);
}

static __inline HI_U16 hi_isp_sharpen_undervarth0_read(ISP_DEV IspDev) {
	return (IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25260) & 0x3ff);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_sharpen_undervarmul
// ------------------------------------------------------------------------------ //
//  Sharpen undervarmul
// ------------------------------------------------------------------------------ //

// args: data (13-bit)

static __inline HI_VOID hi_isp_sharpen_undervarmul_write(ISP_DEV IspDev, HI_U16 data) {
	IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25266, data & 0x1fff);
}

static __inline HI_U16 hi_isp_sharpen_undervarmul_read(ISP_DEV IspDev) {
	return (IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25266) & 0x1fff);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_sharpen_undervaramt
// ------------------------------------------------------------------------------ //
//  Sharpen undervar amt
// ------------------------------------------------------------------------------ //

// args: data (8-bit)

static __inline HI_VOID hi_isp_sharpen_undervaramt_write(ISP_DEV IspDev, HI_U8 data) {
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25264, data);
}

static __inline HI_U8 hi_isp_sharpen_undervaramt_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25264));
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_sharpen_overlumawgt1/overlumawgt0
// ------------------------------------------------------------------------------ //
//  Sharpen overvar luma weight
// ------------------------------------------------------------------------------ //

// args: data (8-bit)

static __inline HI_VOID hi_isp_sharpen_overlumawgt1_write(ISP_DEV IspDev, HI_U8 data) {
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2526b, data);
}

static __inline HI_U8 hi_isp_sharpen_overlumawgt1_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2526b));
}

static __inline HI_VOID hi_isp_sharpen_overlumawgt0_write(ISP_DEV IspDev, HI_U8 data) {
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2526a, data);
}

static __inline HI_U8 hi_isp_sharpen_overlumawgt0_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2526a));
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_sharpen_overlumath1/overlumath0
// ------------------------------------------------------------------------------ //
//  Sharpen overlumath
// ------------------------------------------------------------------------------ //

// args: data (8-bit)

static __inline HI_VOID hi_isp_sharpen_overlumath1_write(ISP_DEV IspDev, HI_U8 data) {
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25269, data);
}

static __inline HI_U8 hi_isp_sharpen_overlumath1_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25269));
}

static __inline HI_VOID hi_isp_sharpen_overlumath0_write(ISP_DEV IspDev, HI_U8 data) {
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25268, data);
}

static __inline HI_U8 hi_isp_sharpen_overlumath0_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25268));
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_sharpen_underlumawgt1/underlumawgt0
// ------------------------------------------------------------------------------ //
//  Sharpen undervar luma weight
// ------------------------------------------------------------------------------ //

// args: data (8-bit)

static __inline HI_VOID hi_isp_sharpen_underlumawgt1_write(ISP_DEV IspDev, HI_U8 data) {
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2526f, data);
}

static __inline HI_U8 hi_isp_sharpen_underlumawgt1_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2526f));
}

static __inline HI_VOID hi_isp_sharpen_underlumawgt0_write(ISP_DEV IspDev, HI_U8 data) {
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2526e, data);
}

static __inline HI_U8 hi_isp_sharpen_underlumawgt0_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2526e));
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_sharpen_underlumath1/underlumath0
// ------------------------------------------------------------------------------ //
//  Sharpen underlumath
// ------------------------------------------------------------------------------ //

// args: data (8-bit)

static __inline HI_VOID hi_isp_sharpen_underlumath1_write(ISP_DEV IspDev, HI_U8 data) {
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2526d, data);
}

static __inline HI_U8 hi_isp_sharpen_underlumath1_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2526d));
}

static __inline HI_VOID hi_isp_sharpen_underlumath0_write(ISP_DEV IspDev, HI_U8 data) {
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2526c, data);
}

static __inline HI_U8 hi_isp_sharpen_underlumath0_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x2526c));
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_sharpen_underlumamul/overlumamul
// ------------------------------------------------------------------------------ //
//  Sharpen under/over lumamul
// ------------------------------------------------------------------------------ //

// args: data (13-bit)

static __inline HI_VOID hi_isp_sharpen_underlumamul_write(ISP_DEV IspDev, HI_U16 data) {
	IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25272, data & 0x1fff);
}

static __inline HI_U16 hi_isp_sharpen_underlumamul_read(ISP_DEV IspDev) {
	return (IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25272) & 0x1fff);
}

static __inline HI_VOID hi_isp_sharpen_overlumamul_write(ISP_DEV IspDev, HI_U16 data) {
	IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25270, data & 0x1fff);
}

static __inline HI_U16 hi_isp_sharpen_overlumamul_read(ISP_DEV IspDev) {
	return (IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25270) & 0x1fff);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_sharpen_limit
// ------------------------------------------------------------------------------ //
//  Sharpen coring limit
// ------------------------------------------------------------------------------ //

// args: data (4-bit)

static __inline HI_VOID hi_isp_sharpen_limit_write(ISP_DEV IspDev, HI_U8 data) {
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25276, data & 0xf);
}

static __inline HI_U8 hi_isp_sharpen_limit_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25276) & 0xf);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_sharpen_maxshift/minshift
// ------------------------------------------------------------------------------ //
//  Sharpen coring maxshift/minshift
// ------------------------------------------------------------------------------ //

// args: data (8-bit)

static __inline HI_VOID hi_isp_sharpen_maxshift_write(ISP_DEV IspDev, HI_U8 data) {
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25275, data);
}

static __inline HI_U8 hi_isp_sharpen_maxshift_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25275));
}

static __inline HI_VOID hi_isp_sharpen_minshift_write(ISP_DEV IspDev, HI_U8 data) {
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25274, data);
}

static __inline HI_U8 hi_isp_sharpen_minshift_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x25274));
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_sharpen_update_mode
// ------------------------------------------------------------------------------ //
//  Sharpen update_mode
// ------------------------------------------------------------------------------ //

// args: data (1-bit)

static __inline HI_VOID hi_isp_sharpen_update_mode_write(ISP_DEV IspDev, HI_U8 data) {
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x252e4, (data & 0x1));
}

static __inline HI_U8 hi_isp_sharpen_update_mode_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x252e4) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_sharpen_update
// ------------------------------------------------------------------------------ //
//  Sharpen update
// ------------------------------------------------------------------------------ //

// args: data (1-bit)

static __inline HI_VOID hi_isp_sharpen_update_write(ISP_DEV IspDev, HI_U8 data) {
	IOWR_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x252ec, (data & 0x1));
}

static __inline HI_U8 hi_isp_sharpen_update_read(ISP_DEV IspDev) {
	return (IORD_8DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x252ec) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_sharpen_height/width
// ------------------------------------------------------------------------------ //
//  Sharpen size
// ------------------------------------------------------------------------------ //

// args: data (13-bit)

static __inline HI_VOID hi_isp_sharpen_height_write(ISP_DEV IspDev, HI_U16 data) {
	IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x252f2, (data & 0x1fff));
}

static __inline HI_U16 hi_isp_sharpen_height_read(ISP_DEV IspDev) {
	return (IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x252f2) & 0x1fff);
}

static __inline HI_VOID hi_isp_sharpen_width_write(ISP_DEV IspDev, HI_U16 data) {
	IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x252f0, (data & 0x1fff));
}

static __inline HI_U8 hi_isp_sharpen_width_read(ISP_DEV IspDev) {
	return (IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x252f0) & 0x1fff);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_dci
// ------------------------------------------------------------------------------ //
//  DCI
// ------------------------------------------------------------------------------ //
// args: data (13-bit)
static __inline HI_VOID hi_vi_dci_hor_start_write(ISP_DEV IspDev, HI_U32 data) {  

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x46558, 12, 0, data);
}
static __inline HI_U32 hi_vi_dci_hor_start_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x46558, 12, 0);
}

// args: data (13-bit)
static __inline HI_VOID hi_vi_dci_hor_end_write(ISP_DEV IspDev, HI_U32 data) {  

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x46558, 28, 16, data);
}
static __inline HI_U32 hi_vi_dci_hor_end_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x46558, 28, 16);
}

// args: data (13-bit)
static __inline HI_VOID hi_vi_dci_ver_start_write(ISP_DEV IspDev, HI_U32 data) {  

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x4655C, 12, 0, data);
}
static __inline HI_U32 hi_vi_dci_ver_start_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x4655C, 12, 0);
}

// args: data (13-bit)
static __inline HI_VOID hi_vi_dci_ver_end_write(ISP_DEV IspDev, HI_U32 data) {  

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x4655C, 28, 16, data);
}
static __inline HI_U32 hi_vi_dci_ver_end_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x4655C, 28, 16);
}

// args: data (13-bit)
static __inline HI_VOID hi_vi_dci_hor_area_start_write(ISP_DEV IspDev, HI_U32 data) {  

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x46560, 12, 0, data);
}
static __inline HI_U32 hi_vi_dci_hor_area_start_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x46560, 12, 0);
}

// args: data (13-bit)
static __inline HI_VOID hi_vi_dci_hor_area_end_write(ISP_DEV IspDev, HI_U32 data) {  

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x46560, 28, 16, data);
}
static __inline HI_U32 hi_vi_dci_hor_area_end_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x46560, 28, 16);
}

// args: data (13-bit)
static __inline HI_VOID hi_vi_dci_ver_area_start_write(ISP_DEV IspDev, HI_U32 data) {  

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x46564, 12, 0, data);
}
static __inline HI_U32 hi_vi_dci_ver_area_start_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x46564, 12, 0);
}

// args: data (13-bit)
static __inline HI_VOID hi_vi_dci_ver_area_end_write(ISP_DEV IspDev, HI_U32 data) {  

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x46564, 28, 16, data);
}
static __inline HI_U32 hi_vi_dci_ver_area_end_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x46564, 28, 16);
}

// args: data (13-bit)
static __inline HI_VOID hi_vi_dci_image_width_write(ISP_DEV IspDev, HI_U32 data) {  

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x464F0, 12, 0, data);
}
static __inline HI_U32 hi_vi_dci_image_width_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x464F0, 12, 0);
}

// args: data (13-bit)
static __inline HI_VOID hi_vi_dci_image_height_write(ISP_DEV IspDev, HI_U32 data) {  

    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x464F0, 28, 16, data);
}
static __inline HI_U32 hi_vi_dci_image_height_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0x464F0, 28, 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_slave_mode_configs
// ------------------------------------------------------------------------------ //
//  /* Slave mode sensor sync signal */
// ------------------------------------------------------------------------------ //
// args: data (32-bit)
static __inline HI_VOID hi_isp_slave_mode_configs_write(ISP_DEV IspDev, HI_U32 data){
    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0xA0, 31, 0, data);
} 

static __inline HI_U32 hi_isp_slave_mode_configs_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0xA0, 31, 0);
}

// args: data (32-bit)
static __inline HI_VOID hi_isp_slave_mode_vstime_write(ISP_DEV IspDev, HI_U32 data){
    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0xB0, 31, 0, data);
} 

static __inline HI_U32 hi_isp_slave_mode_vstime_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0xB0, 31, 0);
}

// args: data (32-bit)
static __inline HI_VOID hi_isp_slave_mode_hstime_write(ISP_DEV IspDev, HI_U32 data){
    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0xB4, 31, 0, data);
} 

static __inline HI_U32 hi_isp_slave_mode_hstime_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0xB4, 31, 0);
}

// args: data (32-bit)
static __inline HI_VOID hi_isp_slave_mode_vscyc_write(ISP_DEV IspDev, HI_U32 data){
    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0xB8, 31, 0, data);
} 

static __inline HI_U32 hi_isp_slave_mode_vscyc_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0xB8, 31, 0);
}

// args: data (32-bit)
static __inline HI_VOID hi_isp_slave_mode_hscyc_write(ISP_DEV IspDev, HI_U32 data){
    REG_BITS_MODIFY(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0xBC, 31, 0, data);
} 

static __inline HI_U32 hi_isp_slave_mode_hscyc_read(ISP_DEV IspDev) {
    
    return REG_BITS_READ(IOWR_32DIRECT, IORD_32DIRECT, ISP_REG_BASE(IspDev) + 0xBC, 31, 0);
}


#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __ISP_CONFIG_H__ */
