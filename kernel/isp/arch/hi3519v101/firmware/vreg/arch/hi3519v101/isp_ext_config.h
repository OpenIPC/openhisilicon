/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_ext_config.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/01/16
  Description   :
  History       :
  1.Date        : 2013/01/16
    Author      : n00168968
    Modification: Created file

******************************************************************************/

#ifndef __ISP_EXT_CONFIG_H__
#define __ISP_EXT_CONFIG_H__

#include "hi_vreg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

// ------------------------------------------------------------------------------ //
// Instance 'ext' of module 'ext_config'
// ------------------------------------------------------------------------------ //

#define HI_EXT_BASE_ADDR(IspDev) (ISP_VREG_BASE(IspDev))
#define HI_EXT_SIZE      (0x10000)

// ------------------------------------------------------------------------------ //
// Group: Top
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_ext_top_mem_init
// ------------------------------------------------------------------------------ //
// HI_TRUE: The state of ISP memory(ext register) is initialized;
// HI_FALSE: The state of ISP memory(ext register) is NOT initialized
// ------------------------------------------------------------------------------ //

#define HI_EXT_TOP_MEM_INIT_DEFAULT (0x0)
#define HI_EXT_TOP_MEM_INIT_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_top_mem_init_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0000, (data & 0x01));
}
static __inline HI_U8 hi_ext_top_mem_init_read(ISP_DEV IspDev) {
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0000) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_top_wdr_cfg
// ------------------------------------------------------------------------------ //
// HI_TRUE: The state of WDR config is initialized;
// HI_FALSE: The state of WDR config is NOT initialized
// ------------------------------------------------------------------------------ //

#define HI_EXT_TOP_WDR_CFG_DEFAULT (0x0)
#define HI_EXT_TOP_WDR_CFG_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_top_wdr_cfg_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0001, (data & 0x01));
}
static __inline HI_U8 hi_ext_top_wdr_cfg_read(ISP_DEV IspDev) {
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0001) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_top_pub_attr_cfg
// ------------------------------------------------------------------------------ //
// HI_TRUE: The state of PubAttr config is initialized;
// HI_FALSE: The state of PubAttr config is NOT initialized
// ------------------------------------------------------------------------------ //

#define HI_EXT_TOP_PUB_ATTR_CFG_DEFAULT (0x0)
#define HI_EXT_TOP_PUB_ATTR_CFG_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_top_pub_attr_cfg_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0002, (data & 0x01));
}
static __inline HI_U8 hi_ext_top_pub_attr_cfg_read(ISP_DEV IspDev) {
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0002) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_top_isp_init
// ------------------------------------------------------------------------------ //
// HI_TRUE: The state of ISP is initialized;
// HI_FALSE: The state of ISP is NOT initialized
// ------------------------------------------------------------------------------ //

#define HI_EXT_TOP_ISP_INIT_DEFAULT (0x0)
#define HI_EXT_TOP_ISP_INIT_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_top_isp_init_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0003, (data & 0x01));
}
static __inline HI_U8 hi_ext_top_isp_init_read(ISP_DEV IspDev) {
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0003) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_top_isp_run
// ------------------------------------------------------------------------------ //
// HI_TRUE: The state of ISP is running;
// HI_FALSE: The state of ISP is NOT running
// ------------------------------------------------------------------------------ //

#define HI_EXT_TOP_ISP_RUN_DEFAULT (0x0)
#define HI_EXT_TOP_ISP_RUN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_top_isp_run_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0004, (data & 0x01));
}
static __inline HI_U8 hi_ext_top_isp_run_read(ISP_DEV IspDev) {
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0004) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_top_isp_run_enable
// ------------------------------------------------------------------------------ //
// HI_TRUE: ISP_run is enable;
// HI_FALSE: ISP_run is disable
// ------------------------------------------------------------------------------ //

#define HI_EXT_TOP_ISP_RUN_ENABLE_DEFAULT (0x0)
#define HI_EXT_TOP_ISP_RUN_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_top_isp_run_enable_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0005, (data & 0x01));
}
static __inline HI_U8 hi_ext_top_isp_run_enable_read(ISP_DEV IspDev) {
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0005) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_top_wdr_switch
// ------------------------------------------------------------------------------ //
// HI_TRUE: WDR switch is complete;
// HI_FALSE: WDR switch is NOT complete
// ------------------------------------------------------------------------------ //

#define HI_EXT_TOP_WDR_SWITCH_DEFAULT (0x0)
#define HI_EXT_TOP_WDR_SWITCH_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_top_wdr_switch_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0006, (data & 0x01));
}
static __inline HI_U8 hi_ext_top_wdr_switch_read(ISP_DEV IspDev) {
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0006) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_top_res_switch
// ------------------------------------------------------------------------------ //
// HI_TRUE: Resolution switch is complete;
// HI_FALSE: Resolution switch is NOT complete
// ------------------------------------------------------------------------------ //

#define HI_EXT_TOP_RES_SWITCH_DEFAULT (0x0)
#define HI_EXT_TOP_RES_SWITCH_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_top_res_switch_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0007, (data & 0x01));
}
static __inline HI_U8 hi_ext_top_res_switch_read(ISP_DEV IspDev) {
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0007) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_top_isp_working_mode
// ------------------------------------------------------------------------------ //

#define HI_EXT_TOP_ISP_WORKING_MODE_DEFAULT (0x0)
#define HI_EXT_TOP_ISP_WORKING_MODE_DATASIZE (2)

// args: data (2-bit)
static __inline HI_VOID hi_ext_top_isp_working_mode_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0008, (data & 0x03));
}
static __inline HI_U8 hi_ext_top_isp_working_mode_read(ISP_DEV IspDev) {
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0008) & 0x03);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_top_sensor_width
// ------------------------------------------------------------------------------ //
//
// ------------------------------------------------------------------------------ //

#define HI_EXT_TOP_SENSOR_WIDTH_DEFAULT (0xF00)
#define HI_EXT_TOP_SENSOR_WIDTH_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_ext_top_sensor_width_write(ISP_DEV IspDev, HI_U32 data) {
    IOWR_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0010, data);
}
static __inline HI_U32 hi_ext_top_sensor_width_read(ISP_DEV IspDev) {
    return IORD_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0010);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_top_sensor_width
// ------------------------------------------------------------------------------ //
//
// ------------------------------------------------------------------------------ //

#define HI_EXT_TOP_SENSOR_HEIGHT_DEFAULT (0x870)
#define HI_EXT_TOP_SENSOR_HEIGHT_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_ext_top_sensor_height_write(ISP_DEV IspDev, HI_U32 data) {
    IOWR_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0014, data);
}
static __inline HI_U32 hi_ext_top_sensor_height_read(ISP_DEV IspDev) {
    return IORD_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0014);
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_isp_stitch_buffer_phyaddr
// ------------------------------------------------------------------------------ //

#define HI_EXT_ISP_STITCH_BUFFER_PHYADDR_DEFAULT (0x0)
#define HI_EXT_ISP_STITCH_BUFFER_PHYADDR_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_ext_isp_stitch_buffer_phyaddr_write(ISP_DEV IspDev, HI_U32 data) {
    IOWR_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0018, data);
}
static __inline HI_U32 hi_ext_isp_stitch_buffer_phyaddr_read(ISP_DEV IspDev) {
    return IORD_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0018);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_isp_stitch_buffer_size
// ------------------------------------------------------------------------------ //

#define HI_EXT_ISP_STITCH_BUFFER_SIZE_DEFAULT (0x0)
#define HI_EXT_ISP_STITCH_BUFFER_SIZE_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_ext_isp_stitch_buffer_size_write(ISP_DEV IspDev, HI_U32 data) {
    IOWR_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x001c, data);
}
static __inline HI_U32 hi_ext_isp_stitch_buffer_size_read(ISP_DEV IspDev) {
    return IORD_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x001c);
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_sns_delay_num
// ------------------------------------------------------------------------------ //
// senosr register delay number
// ------------------------------------------------------------------------------ //

#define HI_EXT_SNS_DELAY_NUM_DEFAULT (0x0)
#define HI_EXT_SNS_DELAY_NUM_DATASIZE (8)
#define HI_EXT_SNS_DELAY_NUM_SIZE (48)

static __inline HI_VOID hi_ext_sns_delay_num_write(ISP_DEV IspDev, HI_U8 index,HI_U8 data){
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0100 + index, data);
}

static __inline HI_U8 hi_ext_sns_delay_num_read(ISP_DEV IspDev, HI_U8 index){
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0100 + index);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sns_delay_num_control
// ------------------------------------------------------------------------------ //
// 0: use config of cmos.c; 1: use config of ext_reg; 2: update ext_reg from cmos.c
// ------------------------------------------------------------------------------ //

#define HI_EXT_SNS_DELAY_NUM_CTRL_DEFAULT (0x0)
#define HI_EXT_SNS_DELAY_NUM_CTRL_DATASIZE (8)
#define HI_EXT_SNS_DELAY_NUM_CTRL_USE_CMOS       (0)
#define HI_EXT_SNS_DELAY_NUM_CTRL_USE_EXTREG     (1)
#define HI_EXT_SNS_DELAY_NUM_CTRL_UPDATE_EXTREG  (2)

static __inline HI_VOID hi_ext_sns_delay_num_ctrl_write(ISP_DEV IspDev, HI_U8 data){
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0130, data);
}

static __inline HI_U8 hi_ext_sns_delay_num_ctrl_read(ISP_DEV IspDev){
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0130);
}


// ------------------------------------------------------------------------------ //
// Group: Sync
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_offset_x
// ------------------------------------------------------------------------------ //
// Offset in pixels from v-sync reference to start of active video
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_OFFSET_X_DEFAULT (0x000)
#define HI_EXT_SYNC_OFFSET_X_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_ext_sync_offset_x_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0200, (data & 0x0FFF));
}
static __inline HI_U16 hi_ext_sync_offset_x_read(ISP_DEV IspDev) {
    return (IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0200) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_offset_y
// ------------------------------------------------------------------------------ //
// Offset in lines from v-sync reference to start of active video
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_OFFSET_Y_DEFAULT (0x000)
#define HI_EXT_SYNC_OFFSET_Y_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_ext_sync_offset_y_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0202, (data & 0x0FFF));
}
static __inline HI_U16 hi_ext_sync_offset_y_read(ISP_DEV IspDev) {
    return (IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0202) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_total_width
// ------------------------------------------------------------------------------ //
// Total frame width including horizontal blanking
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_TOTAL_WIDTH_DEFAULT (0x672)
#define HI_EXT_SYNC_TOTAL_WIDTH_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_sync_total_width_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0204, data);
}
static __inline HI_U16 hi_ext_sync_total_width_read(ISP_DEV IspDev) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0204);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_total_height
// ------------------------------------------------------------------------------ //
// Total frame height including vertical blanking
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_TOTAL_HEIGHT_DEFAULT (0x2EE)
#define HI_EXT_SYNC_TOTAL_HEIGHT_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_sync_total_height_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0206, data);
}
static __inline HI_U16 hi_ext_sync_total_height_read(ISP_DEV IspDev) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0206);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_active_width
// ------------------------------------------------------------------------------ //
// Active video width in pixels
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_ACTIVE_WIDTH_DEFAULT (0x500)
#define HI_EXT_SYNC_ACTIVE_WIDTH_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_sync_active_width_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0208, data);
}
static __inline HI_U16 hi_ext_sync_active_width_read(ISP_DEV IspDev) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0208);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_active_height
// ------------------------------------------------------------------------------ //
// Active video height in lines
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_ACTIVE_HEIGHT_DEFAULT (0x2D0)
#define HI_EXT_SYNC_ACTIVE_HEIGHT_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_sync_active_height_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x020A, data);
}
static __inline HI_U16 hi_ext_sync_active_height_read(ISP_DEV IspDev) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x020A);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_h_front_porch
// ------------------------------------------------------------------------------ //
// Horizontal sync front porch (for DVI out)
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_H_FRONT_PORCH_DEFAULT (0x048)
#define HI_EXT_SYNC_H_FRONT_PORCH_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_ext_sync_h_front_porch_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x020C, (data & 0x0FFF));
}
static __inline HI_U16 hi_ext_sync_h_front_porch_read(ISP_DEV IspDev) {
    return (IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x020C) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_v_front_porch
// ------------------------------------------------------------------------------ //
// Vertical sync front porch (for DVI out)
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_V_FRONT_PORCH_DEFAULT (0x003)
#define HI_EXT_SYNC_V_FRONT_PORCH_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_ext_sync_v_front_porch_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x020E, (data & 0x0FFF));
}
static __inline HI_U16 hi_ext_sync_v_front_porch_read(ISP_DEV IspDev) {
    return (IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x020E) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_h_sync_width
// ------------------------------------------------------------------------------ //
// Horizontal sync width (for DVI out)
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_H_SYNC_WIDTH_DEFAULT (0x50)
#define HI_EXT_SYNC_H_SYNC_WIDTH_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_ext_sync_h_sync_width_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0210, (data & 0x0FFF));
}
static __inline HI_U16 hi_ext_sync_h_sync_width_read(ISP_DEV IspDev) {
    return (IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0210) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_v_sync_width
// ------------------------------------------------------------------------------ //
// Vertical sync width (for DVI out)
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_V_SYNC_WIDTH_DEFAULT (0x5)
#define HI_EXT_SYNC_V_SYNC_WIDTH_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_ext_sync_v_sync_width_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0212, (data & 0x0FFF));
}
static __inline HI_U16 hi_ext_sync_v_sync_width_read(ISP_DEV IspDev) {
    return (IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0212) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_control
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_CONTROL_DEFAULT (0x8D)
#define HI_EXT_SYNC_CONTROL_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_sync_control_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0214, data);
}
static __inline HI_U8 hi_ext_sync_control_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0214);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_h_sync_out_pol
// ------------------------------------------------------------------------------ //
// Horizontal sync out polarity (for DVI out)
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_H_SYNC_OUT_POL_DEFAULT (1)
#define HI_EXT_SYNC_H_SYNC_OUT_POL_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_sync_h_sync_out_pol_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0214);
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0214, ((data & 0x01) << 2) | (u32Current & 0xFB));
}
static __inline HI_U8 hi_ext_sync_h_sync_out_pol_read(ISP_DEV IspDev) {
    return ((IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0214) & 0x04) >> 2);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_v_sync_out_pol
// ------------------------------------------------------------------------------ //
// Vertical sync out polarity (for DVI out)
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_V_SYNC_OUT_POL_DEFAULT (1)
#define HI_EXT_SYNC_V_SYNC_OUT_POL_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_sync_v_sync_out_pol_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0214);
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0214, ((data & 0x01) << 3) | (u32Current & 0xF7));
}
static __inline HI_U8 hi_ext_sync_v_sync_out_pol_read(ISP_DEV IspDev) {
    return ((IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0214) & 0x08) >> 3);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_dvi_tx_clock_edge
// ------------------------------------------------------------------------------ //
// DVI output clock edge: 0=falling 1=rising
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_DVI_TX_CLOCK_EDGE_DEFAULT (0)
#define HI_EXT_SYNC_DVI_TX_CLOCK_EDGE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_sync_dvi_tx_clock_edge_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0220);
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0220, (data & 0x01) | (u32Current & 0xFE));
}
static __inline HI_U8 hi_ext_sync_dvi_tx_clock_edge_read(ISP_DEV IspDev) {
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0220) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_dvi_rx_clock_edge
// ------------------------------------------------------------------------------ //
// Video input clock phase: 0=0 1=90 2=180 3=270
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_DVI_RX_CLOCK_EDGE_DEFAULT (0)
#define HI_EXT_SYNC_DVI_RX_CLOCK_EDGE_DATASIZE (2)

// args: data (2-bit)
static __inline HI_VOID hi_ext_sync_dvi_rx_clock_edge_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0220);
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0220, ((data & 0x03) << 1) | (u32Current & 0xF9));
}
static __inline HI_U8 hi_ext_sync_dvi_rx_clock_edge_read(ISP_DEV IspDev) {
    return ((IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0220) & 0x06) >> 1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_h_sync_in_pol
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_H_SYNC_IN_POL_DEFAULT (0)
#define HI_EXT_SYNC_H_SYNC_IN_POL_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_sync_h_sync_in_pol_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0221);
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0221, (data & 0x01) | (u32Current & 0xFE));
}
static __inline HI_U8 hi_ext_sync_h_sync_in_pol_read(ISP_DEV IspDev) {
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0221) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_v_sync_in_pol
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_V_SYNC_IN_POL_DEFAULT (0)
#define HI_EXT_SYNC_V_SYNC_IN_POL_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_sync_v_sync_in_pol_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0221);
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0221, ((data & 0x01) << 1) | (u32Current & 0xFD));
}
static __inline HI_U8 hi_ext_sync_v_sync_in_pol_read(ISP_DEV IspDev) {
    return ((IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0221) & 0x02) >> 1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_field_mode
// ------------------------------------------------------------------------------ //
//
//   0 = Same as external
//   1 = Adjust with active-line
//   2 = Adjust with V-sync
//   3 = Reserved
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_FIELD_MODE_DEFAULT (0)
#define HI_EXT_SYNC_FIELD_MODE_DATASIZE (2)

#define HI_EXT_SYNC_FIELD_MODE_SAME_AS_EXTERNAL (0)
#define HI_EXT_SYNC_FIELD_MODE_ADJUST_WITH_ACTIVE_LINE (1)
#define HI_EXT_SYNC_FIELD_MODE_ADJUST_WITH_V_SYNC (2)
#define HI_EXT_SYNC_FIELD_MODE_RESERVED (3)

// args: data (2-bit)
static __inline HI_VOID hi_ext_sync_field_mode_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0221);
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0221, ((data & 0x03) << 4) | (u32Current & 0xCF));
}
static __inline HI_U8 hi_ext_sync_field_mode_read(ISP_DEV IspDev) {
    return ((IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0221) & 0x30) >> 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_auto_pos
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_AUTO_POS_DEFAULT (0)
#define HI_EXT_SYNC_AUTO_POS_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_sync_auto_pos_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0221);
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0221, ((data & 0x01) << 6) | (u32Current & 0xBF));
}
static __inline HI_U8 hi_ext_sync_auto_pos_read(ISP_DEV IspDev) {
    return ((IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0221) & 0x40) >> 6);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_auto_size
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_AUTO_SIZE_DEFAULT (0)
#define HI_EXT_SYNC_AUTO_SIZE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_sync_auto_size_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0221);
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0221, ((data & 0x01) << 7) | (u32Current & 0x7F));
}
static __inline HI_U8 hi_ext_sync_auto_size_read(ISP_DEV IspDev) {
    return ((IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0221) & 0x80) >> 7);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_max_line_length
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_MAX_LINE_LENGTH_DEFAULT (0xfff)
#define HI_EXT_SYNC_MAX_LINE_LENGTH_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_ext_sync_max_line_length_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0230, (data & 0x0FFF));
}
static __inline HI_U16 hi_ext_sync_max_line_length_read(ISP_DEV IspDev) {
    return (IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0230) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_max_line_length_dis_y
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_MAX_LINE_LENGTH_DIS_Y_DEFAULT (0xfff)
#define HI_EXT_SYNC_MAX_LINE_LENGTH_DIS_Y_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_ext_sync_max_line_length_dis_y_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x023C, (data & 0x0FFF));
}
static __inline HI_U16 hi_ext_sync_max_line_length_dis_y_read(ISP_DEV IspDev) {
    return (IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x023C) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_max_line_length_nr1
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_MAX_LINE_LENGTH_NR1_DEFAULT (1407)
#define HI_EXT_SYNC_MAX_LINE_LENGTH_NR1_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_ext_sync_max_line_length_nr1_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0250, (data & 0x0FFF));
}
static __inline HI_U16 hi_ext_sync_max_line_length_nr1_read(ISP_DEV IspDev) {
    return (IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0250) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_max_line_length_nr2
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_MAX_LINE_LENGTH_NR2_DEFAULT (703)
#define HI_EXT_SYNC_MAX_LINE_LENGTH_NR2_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_ext_sync_max_line_length_nr2_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0254, (data & 0x0FFF));
}
static __inline HI_U16 hi_ext_sync_max_line_length_nr2_read(ISP_DEV IspDev) {
    return (IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0254) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_input_field_toggle
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_INPUT_FIELD_TOGGLE_DEFAULT (0)
#define HI_EXT_SYNC_INPUT_FIELD_TOGGLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_sync_input_field_toggle_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0232);
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0232, (data & 0x01) | (u32Current & 0xFE));
}
static __inline HI_U8 hi_ext_sync_input_field_toggle_read(ISP_DEV IspDev) {
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0232) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_input_field_polarity
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_INPUT_FIELD_POLARITY_DEFAULT (0)
#define HI_EXT_SYNC_INPUT_FIELD_POLARITY_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_sync_input_field_polarity_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0232);
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0232, ((data & 0x01) << 1) | (u32Current & 0xFD));
}
static __inline HI_U8 hi_ext_sync_input_field_polarity_read(ISP_DEV IspDev) {
    return ((IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0232) & 0x02) >> 1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_output_field_manual
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_OUTPUT_FIELD_MANUAL_DEFAULT (0)
#define HI_EXT_SYNC_OUTPUT_FIELD_MANUAL_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_sync_output_field_manual_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0232);
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0232, ((data & 0x01) << 4) | (u32Current & 0xEF));
}
static __inline HI_U8 hi_ext_sync_output_field_manual_read(ISP_DEV IspDev) {
    return ((IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0232) & 0x10) >> 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_output_field_value
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_OUTPUT_FIELD_VALUE_DEFAULT (0)
#define HI_EXT_SYNC_OUTPUT_FIELD_VALUE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_sync_output_field_value_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0232);
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0232, ((data & 0x01) << 5) | (u32Current & 0xDF));
}
static __inline HI_U8 hi_ext_sync_output_field_value_read(ISP_DEV IspDev) {
    return ((IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0232) & 0x20) >> 5);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_jumbo_frame_mode
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_JUMBO_FRAME_MODE_DEFAULT (0)
#define HI_EXT_SYNC_JUMBO_FRAME_MODE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_sync_jumbo_frame_mode_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0232);
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0232, ((data & 0x01) << 3) | (u32Current & 0xF7));
}
static __inline HI_U8 hi_ext_sync_jumbo_frame_mode_read(ISP_DEV IspDev) {
    return ((IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0232) & 0x08) >> 3);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_mask_enable
// ------------------------------------------------------------------------------ //
// Enable masking of frame edges based on the values of Mask x and Mask y
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_MASK_ENABLE_DEFAULT (0)
#define HI_EXT_SYNC_MASK_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_sync_mask_enable_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0232);
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0232, ((data & 0x01) << 6) | (u32Current & 0xBF));
}
static __inline HI_U8 hi_ext_sync_mask_enable_read(ISP_DEV IspDev) {
    return ((IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0232) & 0x40) >> 6);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_mask_auto
// ------------------------------------------------------------------------------ //
// Calculate mask size based on offsets
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_MASK_AUTO_DEFAULT (0)
#define HI_EXT_SYNC_MASK_AUTO_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_sync_mask_auto_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0232);
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0232, ((data & 0x01) << 7) | (u32Current & 0x7F));
}
static __inline HI_U8 hi_ext_sync_mask_auto_read(ISP_DEV IspDev) {
    return ((IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0232) & 0x80) >> 7);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_dis_offset_x
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_DIS_OFFSET_X_DEFAULT (0)
#define HI_EXT_SYNC_DIS_OFFSET_X_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_sync_dis_offset_x_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0234, data);
}
static __inline HI_U8 hi_ext_sync_dis_offset_x_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0234);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_dis_offset_y
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_DIS_OFFSET_Y_DEFAULT (0)
#define HI_EXT_SYNC_DIS_OFFSET_Y_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_sync_dis_offset_y_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0235, data);
}
static __inline HI_U8 hi_ext_sync_dis_offset_y_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0235);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_scale_y
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_SCALE_Y_DEFAULT (0)
#define HI_EXT_SYNC_SCALE_Y_DATASIZE (16)

// args: data (16-bit)
// data format: sign/magnitude 2.14-bit fixed-point
static __inline HI_VOID hi_ext_sync_scale_y_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0236, data);
}
static __inline HI_U16 hi_ext_sync_scale_y_read(ISP_DEV IspDev) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0236);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_skew_x
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_SKEW_X_DEFAULT (0)
#define HI_EXT_SYNC_SKEW_X_DATASIZE (16)

// args: data (16-bit)
// data format: sign/magnitude 2.14-bit fixed-point
static __inline HI_VOID hi_ext_sync_skew_x_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0238, data);
}
static __inline HI_U16 hi_ext_sync_skew_x_read(ISP_DEV IspDev) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0238);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_mask_x
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_MASK_X_DEFAULT (0)
#define HI_EXT_SYNC_MASK_X_DATASIZE (16)

// args: data (16-bit)
// data format: unsigned 16-bit integer
static __inline HI_VOID hi_ext_sync_mask_x_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x023A, data);
}
static __inline HI_U16 hi_ext_sync_mask_x_read(ISP_DEV IspDev) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x023A);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_mask_y
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_MASK_Y_DEFAULT (0)
#define HI_EXT_SYNC_MASK_Y_DATASIZE (16)

// args: data (16-bit)
// data format: unsigned 16-bit integer
static __inline HI_VOID hi_ext_sync_mask_y_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x023E, data);
}
static __inline HI_U16 hi_ext_sync_mask_y_read(ISP_DEV IspDev) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x023E);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_tg_width
// ------------------------------------------------------------------------------ //
// sensor TG full line length
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_TG_WIDTH_DEFAULT (2401)
#define HI_EXT_SYNC_TG_WIDTH_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_sync_tg_width_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0240, data);
}
static __inline HI_U16 hi_ext_sync_tg_width_read(ISP_DEV IspDev) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0240);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_tg_height
// ------------------------------------------------------------------------------ //
// sensor TG full frame height
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_TG_HEIGHT_DEFAULT (1125)
#define HI_EXT_SYNC_TG_HEIGHT_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_sync_tg_height_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0242, data);
}
static __inline HI_U16 hi_ext_sync_tg_height_read(ISP_DEV IspDev) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0242);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_3dnr_active_width
// ------------------------------------------------------------------------------ //
// Active video width in pixels
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_3DNR_ACTIVE_WIDTH_DEFAULT (0x500)
#define HI_EXT_SYNC_3DNR_ACTIVE_WIDTH_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_sync_3dnr_active_width_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0248, data);
}
static __inline HI_U16 hi_ext_sync_3dnr_active_width_read(ISP_DEV IspDev) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0248);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_3dnr_active_height
// ------------------------------------------------------------------------------ //
// Active video height in lines
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_3DNR_ACTIVE_HEIGHT_DEFAULT (0x2D0)
#define HI_EXT_SYNC_3DNR_ACTIVE_HEIGHT_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_sync_3dnr_active_height_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x024A, data);
}
static __inline HI_U16 hi_ext_sync_3dnr_active_height_read(ISP_DEV IspDev) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x024A);
}

// ------------------------------------------------------------------------------ //
// Group: Exposure Status
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_ext_exposure_status_analog_gain_status
// ------------------------------------------------------------------------------ //
// Analog gain status
// ------------------------------------------------------------------------------ //

#define HI_EXT_EXPOSURE_STATUS_ANALOG_GAIN_STATUS_DEFAULT (0x0)
#define HI_EXT_EXPOSURE_STATUS_ANALOG_GAIN_STATUS_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_exposure_status_analog_gain_status_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02AA, data);
}
static __inline HI_U16 hi_ext_exposure_status_analog_gain_status_read(ISP_DEV IspDev) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02AA);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_exposure_status_coarse_digital_gain_status
// ------------------------------------------------------------------------------ //
// Coarse digital gain status
// ------------------------------------------------------------------------------ //

#define HI_EXT_EXPOSURE_STATUS_COARSE_DIGITAL_GAIN_STATUS_DEFAULT (0x0)
#define HI_EXT_EXPOSURE_STATUS_COARSE_DIGITAL_GAIN_STATUS_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_exposure_status_coarse_digital_gain_status_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02A1, data);
}
static __inline HI_U8 hi_ext_exposure_status_coarse_digital_gain_status_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02A1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_exposure_status_fine_digital_gain_status
// ------------------------------------------------------------------------------ //
// Fine digital gain status
// ------------------------------------------------------------------------------ //

#define HI_EXT_EXPOSURE_STATUS_FINE_DIGITAL_GAIN_STATUS_DEFAULT (0x0)
#define HI_EXT_EXPOSURE_STATUS_FINE_DIGITAL_GAIN_STATUS_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_ext_exposure_status_fine_digital_gain_status_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02A2, (data & 0x03FF));
}
static __inline HI_U16 hi_ext_exposure_status_fine_digital_gain_status_read(ISP_DEV IspDev) {
    return (IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02A2) & 0x03FF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_exposure_status_long_integration_time_status
// ------------------------------------------------------------------------------ //
// Long integration time status
// ------------------------------------------------------------------------------ //

#define HI_EXT_EXPOSURE_STATUS_LONG_INTEGRATION_TIME_STATUS_DEFAULT (0x0)
#define HI_EXT_EXPOSURE_STATUS_LONG_INTEGRATION_TIME_STATUS_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_exposure_status_long_integration_time_status_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02A4, data);
}
static __inline HI_U16 hi_ext_exposure_status_long_integration_time_status_read(ISP_DEV IspDev) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02A4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_exposure_status_short_integration_time_status
// ------------------------------------------------------------------------------ //
// Short integration time status
// ------------------------------------------------------------------------------ //

#define HI_EXT_EXPOSURE_STATUS_SHORT_INTEGRATION_TIME_STATUS_DEFAULT (0x0)
#define HI_EXT_EXPOSURE_STATUS_SHORT_INTEGRATION_TIME_STATUS_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_exposure_status_short_integration_time_status_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02A6, data);
}
static __inline HI_U16 hi_ext_exposure_status_short_integration_time_status_read(ISP_DEV IspDev) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02A6);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_exposure_status_average_brightness_status
// ------------------------------------------------------------------------------ //
// Average brightness status
// ------------------------------------------------------------------------------ //

#define HI_EXT_EXPOSURE_STATUS_AVERAGE_BRIGHTNESS_STATUS_DEFAULT (0x0)
#define HI_EXT_EXPOSURE_STATUS_AVERAGE_BRIGHTNESS_STATUS_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_exposure_status_average_brightness_status_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02A8, data);
}
static __inline HI_U16 hi_ext_exposure_status_average_brightness_status_read(ISP_DEV IspDev) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02A8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_exposure_status_illumination_status
// ------------------------------------------------------------------------------ //
// Sensor Illumination status
// ------------------------------------------------------------------------------ //

#define HI_EXT_EXPOSURE_STATUS_ILLUMINATION_STATUS_DEFAULT (0x0)
#define HI_EXT_EXPOSURE_STATUS_ILLUMINATION_STATUS_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_exposure_status_illumination_status_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02AC, data);
}
static __inline HI_U16 hi_ext_exposure_status_illumination_status_read(ISP_DEV IspDev) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02AC);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_exposure_status_exposure_ratio_status
// ------------------------------------------------------------------------------ //
// Sensor Illumination status
// ------------------------------------------------------------------------------ //

#define HI_EXT_EXPOSURE_STATUS_EXPOSURE_RATIO_STATUS_DEFAULT (0x0)
#define HI_EXT_EXPOSURE_STATUS_EXPOSURE_RATIO_STATUS_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_exposure_status_exposure_ratio_status_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02AE, data);
}
static __inline HI_U16 hi_ext_exposure_status_exposure_ratio_status_read(ISP_DEV IspDev) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02AE);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_exposure_status_anti_flicker_status
// ------------------------------------------------------------------------------ //

#define HI_EXT_EXPOSURE_STATUS_ANTI_FLICKER_STATUS_DEFAULT (0x0)
#define HI_EXT_EXPOSURE_STATUS_ANTI_FLICKER_STATUS_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_exposure_status_anti_flicker_status_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02A0, data);
}
static __inline HI_U8 hi_ext_exposure_status_anti_flicker_status_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02A0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_exposure_status_short_overexposed
// ------------------------------------------------------------------------------ //
// Short overexposed
// ------------------------------------------------------------------------------ //

#define HI_EXT_EXPOSURE_STATUS_SHORT_OVEREXPOSED_DEFAULT (0)
#define HI_EXT_EXPOSURE_STATUS_SHORT_OVEREXPOSED_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_exposure_status_short_overexposed_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02A0);
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02A0, (data & 0x01) | (u32Current & 0xFE));
}
static __inline HI_U8 hi_ext_exposure_status_short_overexposed_read(ISP_DEV IspDev) {
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02A0) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_exposure_status_short_underexposed
// ------------------------------------------------------------------------------ //
// Short underexposed
// ------------------------------------------------------------------------------ //

#define HI_EXT_EXPOSURE_STATUS_SHORT_UNDEREXPOSED_DEFAULT (0)
#define HI_EXT_EXPOSURE_STATUS_SHORT_UNDEREXPOSED_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_exposure_status_short_underexposed_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02A0);
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02A0, ((data & 0x01) << 1) | (u32Current & 0xFD));
}
static __inline HI_U8 hi_ext_exposure_status_short_underexposed_read(ISP_DEV IspDev) {
    return ((IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02A0) & 0x02) >> 1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_exposure_status_long_overexposed
// ------------------------------------------------------------------------------ //
// Long overexposed
// ------------------------------------------------------------------------------ //

#define HI_EXT_EXPOSURE_STATUS_LONG_OVEREXPOSED_DEFAULT (0)
#define HI_EXT_EXPOSURE_STATUS_LONG_OVEREXPOSED_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_exposure_status_long_overexposed_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02A0);
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02A0, ((data & 0x01) << 2) | (u32Current & 0xFB));
}
static __inline HI_U8 hi_ext_exposure_status_long_overexposed_read(ISP_DEV IspDev) {
    return ((IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02A0) & 0x04) >> 2);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_exposure_status_long_underexposed
// ------------------------------------------------------------------------------ //
// Long underexposed
// ------------------------------------------------------------------------------ //

#define HI_EXT_EXPOSURE_STATUS_LONG_UNDEREXPOSED_DEFAULT (0)
#define HI_EXT_EXPOSURE_STATUS_LONG_UNDEREXPOSED_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_exposure_status_long_underexposed_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02A0);
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02A0, ((data & 0x01) << 3) | (u32Current & 0xF7));
}
static __inline HI_U8 hi_ext_exposure_status_long_underexposed_read(ISP_DEV IspDev) {
    return ((IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02A0) & 0x08) >> 3);
}

// ------------------------------------------------------------------------------ //
// Group: Sensor Access
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sensor_access_custom_sensor_addr0
// ------------------------------------------------------------------------------ //
// Sensor address register 0
// ------------------------------------------------------------------------------ //

#define HI_EXT_SENSOR_ACCESS_CUSTOM_SENSOR_ADDR0_DEFAULT (0xFFFF)
#define HI_EXT_SENSOR_ACCESS_CUSTOM_SENSOR_ADDR0_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_sensor_access_custom_sensor_addr0_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02B0, data);
}
static __inline HI_U16 hi_ext_sensor_access_custom_sensor_addr0_read(ISP_DEV IspDev) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02B0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sensor_access_custom_sensor_data0
// ------------------------------------------------------------------------------ //
// Sensor data register 0
// ------------------------------------------------------------------------------ //

#define HI_EXT_SENSOR_ACCESS_CUSTOM_SENSOR_DATA0_DEFAULT (0x0)
#define HI_EXT_SENSOR_ACCESS_CUSTOM_SENSOR_DATA0_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_sensor_access_custom_sensor_data0_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02B2, data);
}
static __inline HI_U16 hi_ext_sensor_access_custom_sensor_data0_read(ISP_DEV IspDev) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02B2);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sensor_access_custom_sensor_addr1
// ------------------------------------------------------------------------------ //
// Sensor address register 0
// ------------------------------------------------------------------------------ //

#define HI_EXT_SENSOR_ACCESS_CUSTOM_SENSOR_ADDR1_DEFAULT (0xFFFF)
#define HI_EXT_SENSOR_ACCESS_CUSTOM_SENSOR_ADDR1_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_sensor_access_custom_sensor_addr1_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02B4, data);
}
static __inline HI_U16 hi_ext_sensor_access_custom_sensor_addr1_read(ISP_DEV IspDev) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02B4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sensor_access_custom_sensor_data1
// ------------------------------------------------------------------------------ //
// Sensor data register 0
// ------------------------------------------------------------------------------ //

#define HI_EXT_SENSOR_ACCESS_CUSTOM_SENSOR_DATA1_DEFAULT (0x0)
#define HI_EXT_SENSOR_ACCESS_CUSTOM_SENSOR_DATA1_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_sensor_access_custom_sensor_data1_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02B6, data);
}
static __inline HI_U16 hi_ext_sensor_access_custom_sensor_data1_read(ISP_DEV IspDev) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02B6);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sensor_access_custom_sensor_addr2
// ------------------------------------------------------------------------------ //
// Sensor address register 0
// ------------------------------------------------------------------------------ //

#define HI_EXT_SENSOR_ACCESS_CUSTOM_SENSOR_ADDR2_DEFAULT (0xFFFF)
#define HI_EXT_SENSOR_ACCESS_CUSTOM_SENSOR_ADDR2_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_sensor_access_custom_sensor_addr2_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02B8, data);
}
static __inline HI_U16 hi_ext_sensor_access_custom_sensor_addr2_read(ISP_DEV IspDev) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02B8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sensor_access_custom_sensor_data2
// ------------------------------------------------------------------------------ //
// Sensor data register 0
// ------------------------------------------------------------------------------ //

#define HI_EXT_SENSOR_ACCESS_CUSTOM_SENSOR_DATA2_DEFAULT (0x0)
#define HI_EXT_SENSOR_ACCESS_CUSTOM_SENSOR_DATA2_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_sensor_access_custom_sensor_data2_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02BA, data);
}
static __inline HI_U16 hi_ext_sensor_access_custom_sensor_data2_read(ISP_DEV IspDev) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02BA);
}

// ------------------------------------------------------------------------------ //
// Group: General Purpose
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_ext_general_purpose_gpi
// ------------------------------------------------------------------------------ //
// Status of push buttons
// ------------------------------------------------------------------------------ //

#define HI_EXT_GENERAL_PURPOSE_GPI_DEFAULT (0x0)
#define HI_EXT_GENERAL_PURPOSE_GPI_DATASIZE (4)

// args: data (4-bit)
static __inline HI_U8 hi_ext_general_purpose_gpi_read(ISP_DEV IspDev) {
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02C0) & 0x0F);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_general_purpose_illumination_target
// ------------------------------------------------------------------------------ //
// debug register 1
// ------------------------------------------------------------------------------ //

#define HI_EXT_GENERAL_PURPOSE_ILLUMINATION_TARGET_DEFAULT (0xFF)
#define HI_EXT_GENERAL_PURPOSE_ILLUMINATION_TARGET_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_general_purpose_illumination_target_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02C4, data);
}
static __inline HI_U8 hi_ext_general_purpose_illumination_target_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02C4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_general_purpose_equilibrium_point
// ------------------------------------------------------------------------------ //
// debug register 2
// ------------------------------------------------------------------------------ //

#define HI_EXT_GENERAL_PURPOSE_EQUILIBRIUM_POINT_DEFAULT (0x0)
#define HI_EXT_GENERAL_PURPOSE_EQUILIBRIUM_POINT_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_general_purpose_equilibrium_point_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02C5, data);
}
static __inline HI_U8 hi_ext_general_purpose_equilibrium_point_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02C5);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_general_purpose_iris_rate
// ------------------------------------------------------------------------------ //
// debug register 3
// ------------------------------------------------------------------------------ //

#define HI_EXT_GENERAL_PURPOSE_IRIS_RATE_DEFAULT (0xFF)
#define HI_EXT_GENERAL_PURPOSE_IRIS_RATE_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_general_purpose_iris_rate_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02C8, data);
}
static __inline HI_U8 hi_ext_general_purpose_iris_rate_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02C8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_general_purpose_debug_4
// ------------------------------------------------------------------------------ //
// debug register 4
// ------------------------------------------------------------------------------ //

#define HI_EXT_GENERAL_PURPOSE_DEBUG_4_DEFAULT (0xFF)
#define HI_EXT_GENERAL_PURPOSE_DEBUG_4_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_general_purpose_debug_4_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02C9, data);
}
static __inline HI_U8 hi_ext_general_purpose_debug_4_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02C9);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_general_purpose_debug_5
// ------------------------------------------------------------------------------ //
// debug register 5
// ------------------------------------------------------------------------------ //

#define HI_EXT_GENERAL_PURPOSE_DEBUG_5_DEFAULT (0xFF)
#define HI_EXT_GENERAL_PURPOSE_DEBUG_5_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_general_purpose_debug_5_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02CA, data);
}
static __inline HI_U8 hi_ext_general_purpose_debug_5_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02CA);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_general_purpose_debug_6
// ------------------------------------------------------------------------------ //
// debug register 6
// ------------------------------------------------------------------------------ //

#define HI_EXT_GENERAL_PURPOSE_DEBUG_6_DEFAULT (0xFF)
#define HI_EXT_GENERAL_PURPOSE_DEBUG_6_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_general_purpose_debug_6_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02CB, data);
}
static __inline HI_U8 hi_ext_general_purpose_debug_6_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02CB);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_general_purpose_misc_control
// ------------------------------------------------------------------------------ //
// 32 bit control output
// ------------------------------------------------------------------------------ //

#define HI_EXT_GENERAL_PURPOSE_MISC_CONTROL_DEFAULT (0x04650000)
#define HI_EXT_GENERAL_PURPOSE_MISC_CONTROL_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_ext_general_purpose_misc_control_write(ISP_DEV IspDev, HI_U32 data) {
    IOWR_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02CC, data);
}
static __inline HI_U32 hi_ext_general_purpose_misc_control_read(ISP_DEV IspDev) {
    return IORD_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02CC);
}

// ------------------------------------------------------------------------------ //
// Group: Flash interface
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_ext_flash_interface_trigger_select
// ------------------------------------------------------------------------------ //
// Xenon flash trigger mode: 0 - Manual, 1 - from sensor, 2 - from timing generator, 3 - reserved
// ------------------------------------------------------------------------------ //

#define HI_EXT_FLASH_INTERFACE_TRIGGER_SELECT_DEFAULT (00)
#define HI_EXT_FLASH_INTERFACE_TRIGGER_SELECT_DATASIZE (2)

// args: data (2-bit)
static __inline HI_VOID hi_ext_flash_interface_trigger_select_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02D0);
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02D0, (data & 0x03) | (u32Current & 0xFC));
}
static __inline HI_U8 hi_ext_flash_interface_trigger_select_read(ISP_DEV IspDev) {
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02D0) & 0x03);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_flash_interface_sensor_strobe_polarity
// ------------------------------------------------------------------------------ //
// 1 - invert sensor strobe
// ------------------------------------------------------------------------------ //

#define HI_EXT_FLASH_INTERFACE_SENSOR_STROBE_POLARITY_DEFAULT (0)
#define HI_EXT_FLASH_INTERFACE_SENSOR_STROBE_POLARITY_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_flash_interface_sensor_strobe_polarity_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02D0);
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02D0, ((data & 0x01) << 4) | (u32Current & 0xEF));
}
static __inline HI_U8 hi_ext_flash_interface_sensor_strobe_polarity_read(ISP_DEV IspDev) {
    return ((IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02D0) & 0x10) >> 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_flash_interface_af_led_enable
// ------------------------------------------------------------------------------ //
// Turn on AF LED
// ------------------------------------------------------------------------------ //

#define HI_EXT_FLASH_INTERFACE_AF_LED_ENABLE_DEFAULT (00)
#define HI_EXT_FLASH_INTERFACE_AF_LED_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_flash_interface_af_led_enable_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02D2);
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02D2, (data & 0x01) | (u32Current & 0xFE));
}
static __inline HI_U8 hi_ext_flash_interface_af_led_enable_read(ISP_DEV IspDev) {
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02D2) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_flash_interface_charge
// ------------------------------------------------------------------------------ //
// Charge the flash module
// ------------------------------------------------------------------------------ //

#define HI_EXT_FLASH_INTERFACE_CHARGE_DEFAULT (00)
#define HI_EXT_FLASH_INTERFACE_CHARGE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_flash_interface_charge_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02D2);
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02D2, ((data & 0x01) << 1) | (u32Current & 0xFD));
}
static __inline HI_U8 hi_ext_flash_interface_charge_read(ISP_DEV IspDev) {
    return ((IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02D2) & 0x02) >> 1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_flash_interface_flash_trigger
// ------------------------------------------------------------------------------ //
// 1 - invert sensor strobe
// ------------------------------------------------------------------------------ //

#define HI_EXT_FLASH_INTERFACE_FLASH_TRIGGER_DEFAULT (00)
#define HI_EXT_FLASH_INTERFACE_FLASH_TRIGGER_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_flash_interface_flash_trigger_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02D2);
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02D2, ((data & 0x01) << 4) | (u32Current & 0xEF));
}
static __inline HI_U8 hi_ext_flash_interface_flash_trigger_read(ISP_DEV IspDev) {
    return ((IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02D2) & 0x10) >> 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_flash_interface_flash_ready
// ------------------------------------------------------------------------------ //
// 1 - flash is charged
// ------------------------------------------------------------------------------ //

#define HI_EXT_FLASH_INTERFACE_FLASH_READY_DEFAULT (0)
#define HI_EXT_FLASH_INTERFACE_FLASH_READY_DATASIZE (1)

// args: data (1-bit)
static __inline HI_U8 hi_ext_flash_interface_flash_ready_read(ISP_DEV IspDev) {
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02D4) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_flash_interface_flash_strobe
// ------------------------------------------------------------------------------ //
// strobe signal from sensor module
// ------------------------------------------------------------------------------ //

#define HI_EXT_FLASH_INTERFACE_FLASH_STROBE_DEFAULT (0)
#define HI_EXT_FLASH_INTERFACE_FLASH_STROBE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_U8 hi_ext_flash_interface_flash_strobe_read(ISP_DEV IspDev) {
    return ((IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x02D4) & 0x02) >> 1);
}

// ------------------------------------------------------------------------------ //
// Group: System
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_flags1
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_FLAGS1_DEFAULT (0x0)
#define HI_EXT_SYSTEM_FLAGS1_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_flags1_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0300, data);
}
static __inline HI_U8 hi_ext_system_flags1_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0300);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_freeze_firmware
// ------------------------------------------------------------------------------ //
// Disables firmware and stops updating the ISP
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_FREEZE_FIRMWARE_DEFAULT (0)
#define HI_EXT_SYSTEM_FREEZE_FIRMWARE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_system_freeze_firmware_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0300);
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0300, (data & 0x01) | (u32Current & 0xFE));
}
static __inline HI_U8 hi_ext_system_freeze_firmware_read(ISP_DEV IspDev) {
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0300) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_directional_sharpening
// ------------------------------------------------------------------------------ //
// Enables manual control of directional sharpening strength: 0=auto 1=manual
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_MANUAL_DIRECTIONAL_SHARPENING_DEFAULT (0)
#define HI_EXT_SYSTEM_MANUAL_DIRECTIONAL_SHARPENING_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_system_manual_directional_sharpening_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0300);
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0300, ((data & 0x01) << 7) | (u32Current & 0x7F));
}
static __inline HI_U8 hi_ext_system_manual_directional_sharpening_read(ISP_DEV IspDev) {
    return ((IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0300) & 0x80) >> 7);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_flags2
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_FLAGS2_DEFAULT (0xC0)
#define HI_EXT_SYSTEM_FLAGS2_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_flags2_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0301, data);
}
static __inline HI_U8 hi_ext_system_flags2_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0301);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_un_directional_sharpening
// ------------------------------------------------------------------------------ //
// Enables manual control of un-directional sharpening strength: 0=auto 1=manual
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_MANUAL_UN_DIRECTIONAL_SHARPENING_DEFAULT (0)
#define HI_EXT_SYSTEM_MANUAL_UN_DIRECTIONAL_SHARPENING_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_system_manual_un_directional_sharpening_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0301);
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0301, (data & 0x01) | (u32Current & 0xFE));
}
static __inline HI_U8 hi_ext_system_manual_un_directional_sharpening_read(ISP_DEV IspDev) {
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0301) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_drc
// ------------------------------------------------------------------------------ //
// Enables manual drc control - drc strength target controls drc strength directly
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_MANUAL_DRC_DEFAULT (0)
#define HI_EXT_SYSTEM_MANUAL_DRC_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_system_manual_drc_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0301);
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0301, ((data & 0x01) << 1) | (u32Current & 0xFD));
}
static __inline HI_U8 hi_ext_system_manual_drc_read(ISP_DEV IspDev) {
    return ((IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0301) & 0x02) >> 1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_3dnr
// ------------------------------------------------------------------------------ //
// Enables manual 3dnr control - 3dnr strength target controls 3dnr strength directly
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_MANUAL_3DNR_DEFAULT (0)
#define HI_EXT_SYSTEM_MANUAL_3DNR_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_system_manual_3dnr_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0301);
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0301, ((data & 0x01) << 3) | (u32Current & 0xF7));
}
static __inline HI_U8 hi_ext_system_manual_3dnr_read(ISP_DEV IspDev) {
    return ((IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0301) & 0x08) >> 3);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_calibrate_flag
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_CALIBRATE_FLAG_DEFAULT (0x0)
#define HI_EXT_SYSTEM_CALIBRATE_FLAG_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_calibrate_flag_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0308, data);
}
static __inline HI_U8 hi_ext_system_calibrate_flag_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0308);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_calibrate_bad_pixels
// ------------------------------------------------------------------------------ //
// Start calibration algorithm for defect pixel correction, bit will be cleared by FW when finished
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_CALIBRATE_BAD_PIXELS_DEFAULT (0)
#define HI_EXT_SYSTEM_CALIBRATE_BAD_PIXELS_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_system_calibrate_bad_pixels_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0308);
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0308, (data & 0x01) | (u32Current & 0xFE));
}
static __inline HI_U8 hi_ext_system_calibrate_bad_pixels_read(ISP_DEV IspDev) {
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0308) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_frame_rates
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_FRAME_RATES_DEFAULT (0x0)
#define HI_EXT_SYSTEM_FRAME_RATES_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_frame_rates_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0306, data);
}
static __inline HI_U8 hi_ext_system_frame_rates_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0306);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_set_1001_rate_divider
// ------------------------------------------------------------------------------ //
// Set 1/1.001 rate multiplier to achieve 29.97 FPS
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_SET_1001_RATE_DIVIDER_DEFAULT (0)
#define HI_EXT_SYSTEM_SET_1001_RATE_DIVIDER_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_system_set_1001_rate_divider_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0304);
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0304, ((data & 0x01) << 4) | (u32Current & 0xEF));
}
static __inline HI_U8 hi_ext_system_set_1001_rate_divider_read(ISP_DEV IspDev) {
    return ((IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0304) & 0x10) >> 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_half_pixel_clock
// ------------------------------------------------------------------------------ //
// Corrects internal interface dividers to match the case when pixel clock is slowed down
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_HALF_PIXEL_CLOCK_DEFAULT (0)
#define HI_EXT_SYSTEM_HALF_PIXEL_CLOCK_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_system_half_pixel_clock_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0304);
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0304, ((data & 0x01) << 5) | (u32Current & 0xDF));
}
static __inline HI_U8 hi_ext_system_half_pixel_clock_read(ISP_DEV IspDev) {
    return ((IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0304) & 0x20) >> 5);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_directional_sharpening_target
// ------------------------------------------------------------------------------ //
// Directional sharpening target. In manual mode this directly sets the directional sharpening strength.
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DIRECTIONAL_SHARPENING_TARGET_DEFAULT (0x08)
#define HI_EXT_SYSTEM_DIRECTIONAL_SHARPENING_TARGET_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_directional_sharpening_target_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0324, data);
}
static __inline HI_U8 hi_ext_system_directional_sharpening_target_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0324);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_maximum_directional_sharpening
// ------------------------------------------------------------------------------ //
// Maximum directional sharpening in all modes.
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_MAXIMUM_DIRECTIONAL_SHARPENING_DEFAULT (0x08)
#define HI_EXT_SYSTEM_MAXIMUM_DIRECTIONAL_SHARPENING_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_maximum_directional_sharpening_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0326, data);
}
static __inline HI_U8 hi_ext_system_maximum_directional_sharpening_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0326);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_minimum_directional_sharpening
// ------------------------------------------------------------------------------ //
// Minimum directional sharpening in all modes.
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_MINIMUM_DIRECTIONAL_SHARPENING_DEFAULT (0x08)
#define HI_EXT_SYSTEM_MINIMUM_DIRECTIONAL_SHARPENING_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_minimum_directional_sharpening_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0327, data);
}
static __inline HI_U8 hi_ext_system_minimum_directional_sharpening_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0327);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_un_directional_sharpening_target
// ------------------------------------------------------------------------------ //
// Un-directional sharpening target. In manual mode this directly sets the un-directional sharpening strength.
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_UN_DIRECTIONAL_SHARPENING_TARGET_DEFAULT (0x08)
#define HI_EXT_SYSTEM_UN_DIRECTIONAL_SHARPENING_TARGET_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_un_directional_sharpening_target_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0328, data);
}
static __inline HI_U8 hi_ext_system_un_directional_sharpening_target_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0328);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_maximum_un_directional_sharpening
// ------------------------------------------------------------------------------ //
// Maximum un-directional sharpening in all modes.
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_MAXIMUM_UN_DIRECTIONAL_SHARPENING_DEFAULT (0x08)
#define HI_EXT_SYSTEM_MAXIMUM_UN_DIRECTIONAL_SHARPENING_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_maximum_un_directional_sharpening_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x032A, data);
}
static __inline HI_U8 hi_ext_system_maximum_un_directional_sharpening_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x032A);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_minimum_un_directional_sharpening
// ------------------------------------------------------------------------------ //
// Minimum un-directional sharpening in all modes.
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_MINIMUM_UN_DIRECTIONAL_SHARPENING_DEFAULT (0x08)
#define HI_EXT_SYSTEM_MINIMUM_UN_DIRECTIONAL_SHARPENING_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_minimum_un_directional_sharpening_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x032B, data);
}
static __inline HI_U8 hi_ext_system_minimum_un_directional_sharpening_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x032B);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_drc_manual_strength
// ------------------------------------------------------------------------------ //
// Manual setting for DRC strength (for manual DRC)
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DRC_MANUAL_STRENGTH_DEFAULT (0x200)
#define HI_EXT_SYSTEM_DRC_MANUAL_STRENGTH_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_system_drc_manual_strength_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x032C, data);
}
static __inline HI_U16 hi_ext_system_drc_manual_strength_read(ISP_DEV IspDev) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x032C);
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_drc_asymmetry
// ------------------------------------------------------------------------------ //
// Asymmetry of asymmetry LUT.
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DRC_ASYMMETRY_DEFAULT (0x14)
#define HI_EXT_SYSTEM_DRC_ASYMMETRY_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_drc_asymmetry_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x032E, data);
}
static __inline HI_U8 hi_ext_system_drc_asymmetry_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x032E);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_drc_bright_enhance
// ------------------------------------------------------------------------------ //
// Bright Enhance of asymmetry LUT.
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DRC_BRIGHT_ENHANCE_DEFAULT (0xC8)
#define HI_EXT_SYSTEM_DRC_BRIGHT_ENHANCE_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_drc_bright_enhance_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x032F, data);
}
static __inline HI_U8 hi_ext_system_drc_bright_enhance_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x032F);
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_drc_auto_strength
// ------------------------------------------------------------------------------ //
// Auto setting for DRC strength (for auto DRC)
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DRC_AUTO_STRENGTH_DEFAULT (0x200)
#define HI_EXT_SYSTEM_DRC_AUTO_STRENGTH_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_system_drc_auto_strength_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0330, data);
}
static __inline HI_U16 hi_ext_system_drc_auto_strength_read(ISP_DEV IspDev) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0330);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_drc_user_tm_enable
// ------------------------------------------------------------------------------ //
// 0: DRC tone curve is generated by u32Asymmetry and u32BrightEnhance; 1: DRC tone curve is defined by au16ToneMappingValue.
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DRC_USER_TM_ENABLE_DEFAULT (0x0)
#define HI_EXT_SYSTEM_DRC_USER_TM_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_system_drc_user_tm_enable_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0334, data & 0x1);
}
static __inline HI_U16 hi_ext_system_drc_user_tm_enable_read(ISP_DEV IspDev) {
    return (IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0334) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_drc_user_tm_update
// ------------------------------------------------------------------------------ //
// 1: au16ToneMappingValue is updated
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DRC_USER_TM_UPDATE_DEFAULT (0x0)
#define HI_EXT_SYSTEM_DRC_USER_TM_UPDATE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_system_drc_user_tm_update_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0338, data & 0x1);
}
static __inline HI_U16 hi_ext_system_drc_user_tm_update_read(ISP_DEV IspDev) {
    return (IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0338) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_nr_threshold_target
// ------------------------------------------------------------------------------ //
// Overall strength of Sinter noise-reduction effect for short exposure
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_NR_THRESHOLD_TARGET_DEFAULT (0x18)
#define HI_EXT_SYSTEM_NR_THRESHOLD_TARGET_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_nr_threshold_target_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0350, data);
}
static __inline HI_U8 hi_ext_system_nr_threshold_target_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0350);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_nr_threshold_long_target
// ------------------------------------------------------------------------------ //
// Overall strength of Sinter noise-reduction effect for long exposure
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_NR_THRESHOLD_LONG_TARGET_DEFAULT (0x18)
#define HI_EXT_SYSTEM_NR_THRESHOLD_LONG_TARGET_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_nr_threshold_long_target_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0351, data);
}
static __inline HI_U8 hi_ext_system_nr_threshold_long_target_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0351);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_maximum_nr_strength
// ------------------------------------------------------------------------------ //
// Maximum nr strength in all modes.
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_MAXIMUM_NR_STRENGTH_DEFAULT (0x18)
#define HI_EXT_SYSTEM_MAXIMUM_NR_STRENGTH_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_maximum_nr_strength_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0352, data);
}
static __inline HI_U8 hi_ext_system_maximum_nr_strength_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0352);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_minimum_nr_strength
// ------------------------------------------------------------------------------ //
// Minimum nr strength in all modes.
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_MINIMUM_NR_STRENGTH_DEFAULT (0x18)
#define HI_EXT_SYSTEM_MINIMUM_NR_STRENGTH_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_minimum_nr_strength_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0353, data);
}
static __inline HI_U8 hi_ext_system_minimum_nr_strength_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0353);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_3dnr_threshold_target
// ------------------------------------------------------------------------------ //
// Overall strength of Temper noise-reduction effect
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_3DNR_THRESHOLD_TARGET_DEFAULT (0x24)
#define HI_EXT_SYSTEM_3DNR_THRESHOLD_TARGET_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_3dnr_threshold_target_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0354, data);
}
static __inline HI_U8 hi_ext_system_3dnr_threshold_target_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0354);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_maximum_3dnr_strength
// ------------------------------------------------------------------------------ //
// Maximum 3dnr strength in all modes.
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_MAXIMUM_3DNR_STRENGTH_DEFAULT (0x24)
#define HI_EXT_SYSTEM_MAXIMUM_3DNR_STRENGTH_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_maximum_3dnr_strength_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0356, data);
}
static __inline HI_U8 hi_ext_system_maximum_3dnr_strength_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0356);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_minimum_3dnr_strength
// ------------------------------------------------------------------------------ //
// Minimum 3dnr strength in all modes.
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_MINIMUM_3DNR_STRENGTH_DEFAULT (0x24)
#define HI_EXT_SYSTEM_MINIMUM_3DNR_STRENGTH_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_minimum_3dnr_strength_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0357, data);
}
static __inline HI_U8 hi_ext_system_minimum_3dnr_strength_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x0357);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_debug_0
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DEBUG_0_DEFAULT (0x00)
#define HI_EXT_SYSTEM_DEBUG_0_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_debug_0_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x03F0, data);
}
static __inline HI_U8 hi_ext_system_debug_0_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x03F0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_debug_1
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DEBUG_1_DEFAULT (0x00)
#define HI_EXT_SYSTEM_DEBUG_1_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_debug_1_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x03F1, data);
}
static __inline HI_U8 hi_ext_system_debug_1_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x03F1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_debug_2
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DEBUG_2_DEFAULT (0x00)
#define HI_EXT_SYSTEM_DEBUG_2_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_debug_2_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x03F2, data);
}
static __inline HI_U8 hi_ext_system_debug_2_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x03F2);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_debug_3
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DEBUG_3_DEFAULT (0x00)
#define HI_EXT_SYSTEM_DEBUG_3_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_debug_3_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x03F3, data);
}
static __inline HI_U8 hi_ext_system_debug_3_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x03F3);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_debug_4
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DEBUG_4_DEFAULT (0x00)
#define HI_EXT_SYSTEM_DEBUG_4_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_debug_4_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x03F4, data);
}
static __inline HI_U8 hi_ext_system_debug_4_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x03F4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_debug_5
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DEBUG_5_DEFAULT (0x00)
#define HI_EXT_SYSTEM_DEBUG_5_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_debug_5_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x03F5, data);
}
static __inline HI_U8 hi_ext_system_debug_5_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x03F5);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_debug_6
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DEBUG_6_DEFAULT (0x00)
#define HI_EXT_SYSTEM_DEBUG_6_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_debug_6_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x03F6, data);
}
static __inline HI_U8 hi_ext_system_debug_6_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x03F6);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_debug_7
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DEBUG_7_DEFAULT (0x00)
#define HI_EXT_SYSTEM_DEBUG_7_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_debug_7_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x03F7, data);
}
static __inline HI_U8 hi_ext_system_debug_7_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x03F7);
}

//==Address=========================================================//
//AE:               0x11400~0x115FF
//AWB:              0x11600~0x117FF
//AF:               0x11800~0x119FF
//NR:               0x11A00~0x11AFF
//DRC:              0x11B00~0x11BFF
//sharpening:       0x12000~0x120FF
//other ISP module: 0x12100~0x121FF
//==Address=========================================================//
#define HI_EXT_SYSTEM_FPS_BASE_DEFAULT (30)
#define HI_EXT_SYSTEM_FPS_BASE_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_ext_system_fps_base_write(ISP_DEV IspDev, HI_U32 data) {
    IOWR_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x1400, data);
}
static __inline HI_U32 hi_ext_system_fps_base_read(ISP_DEV IspDev) {
    return IORD_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x1400);
}

//==============================================================//
//NR: HI_EXT_BASE_ADDR(IspDev) + 0x1A00~0x11AFF
//==============================================================//


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_nr
// ------------------------------------------------------------------------------ //
// Enables manual nr control - nr threshold target controls drc strength
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_MANUAL_NR_DEFAULT (0)
#define HI_EXT_SYSTEM_MANUAL_NR_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_system_manual_nr_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U16 u32Current = IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x1A00);
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x1A00, (((HI_U16) (data & 0x0001)) ) | (u32Current & 0xFFFE));
}
static __inline HI_U8 hi_ext_system_manual_nr_read(ISP_DEV IspDev) {
    return (HI_U8) ((IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x1A00) & 0x0001) );
}

//==============================================================//
//DRC: 0x11B00~0x11BFF
//==============================================================//


//==============================================================//
//sharpening: 0x12000~0x120FF
//==============================================================//

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpening_core
// ------------------------------------------------------------------------------ //
// sharpening core
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_SHARPENING_ENABLE_DEFAULT (0x01)
#define HI_EXT_SYSTEM_SHARPENING_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_system_sharpening_enable_write(ISP_DEV IspDev, HI_U16 data){
    HI_U16 u32Current = IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2000);
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2000, ((HI_U16)(data & 0x0001)) | (u32Current & 0xFFFE));
}
static __inline HI_U8 hi_ext_system_sharpening_enable_read(ISP_DEV IspDev) {
    return (HI_U8) (IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2000) & 0x0001);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpening
// ------------------------------------------------------------------------------ //
// Enables manual Sharpening control - Sharpening target controls Sharpening directly
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_MANUAL_SHARPENING_DEFAULT (0)
#define HI_EXT_SYSTEM_MANUAL_SHARPENING_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_system_manual_sharpening_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U16 u32Current = IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2000);
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2000, ((HI_U16)((data & 0x0001) << 1)) | (u32Current & 0xFFFD));
}
static __inline HI_U8 hi_ext_system_manual_sharpening_read(ISP_DEV IspDev) {
    return (HI_U8) ((IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2000) & 0x0002) >> 1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpening_threshold_target
// ------------------------------------------------------------------------------ //
// Target strength of sharpening effect
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_SHARPENING_TARGET_DEFAULT (0x28)
#define HI_EXT_SYSTEM_SHARPENING_TARGET_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_sharpening_d_target_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U16 u32Current = IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2002);
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2002, (((HI_U16) data) << 8) | (u32Current & 0x00FF));
}
static __inline HI_U8 hi_ext_system_sharpening_d_target_read(ISP_DEV IspDev) {
    return (HI_U8) ((IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2002) & 0xFF00) >> 8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpening_threshold_target
// ------------------------------------------------------------------------------ //
// Target strength of sharpening effect
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_SHARPENING_UD_TARGET_DEFAULT (0x75)
#define HI_EXT_SYSTEM_SHARPENING_UD_TARGET_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_sharpening_ud_target_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U16 u32Current = IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2004);
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2004, (((HI_U16) data) ) | (u32Current & 0xFF00));
}
static __inline HI_U8 hi_ext_system_sharpening_ud_target_read(ISP_DEV IspDev) {
    return (HI_U8) (IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2004) & 0x00FF) ;
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpening_alg_select
// ------------------------------------------------------------------------------ //
// select demosaic sharpen alg
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_SHARPENING_ALG_SELECT_DEFAULT (0x0)
#define HI_EXT_SYSTEM_SHARPENING_ALG_SELECT_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_system_sharpening_alg_select_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2005);
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2005, ((HI_U8)(data & 0x01)) | (u8Current & 0xFE));
}
static __inline HI_U8 hi_ext_system_sharpening_alg_select_read(ISP_DEV IspDev) {
    return (HI_U8) (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2005) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpening_hf_target
// ------------------------------------------------------------------------------ //
// Target strength of sharpening effect
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_SHARPENING_HF_TARGET_DEFAULT (0x10)
#define HI_EXT_SYSTEM_SHARPENING_HF_TARGET_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_sharpening_hf_target_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U16 u32Current = IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2006);
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2006, (((HI_U16) data) ) | (u32Current & 0xFF00));
}
static __inline HI_U8 hi_ext_system_sharpening_hf_target_read(ISP_DEV IspDev) {
    return (HI_U8) (IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2006) & 0x00FF) ;
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpening_mf_target
// ------------------------------------------------------------------------------ //
// Target strength of sharpening effect
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_SHARPENING_MF_TARGET_DEFAULT (0x10)
#define HI_EXT_SYSTEM_SHARPENING_MF_TARGET_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_sharpening_mf_target_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U16 u32Current = IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2008);
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2008, (((HI_U16) data) ) | (u32Current & 0xFF00));
}
static __inline HI_U8 hi_ext_system_sharpening_mf_target_read(ISP_DEV IspDev) {
    return (HI_U8) (IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2008) & 0x00FF) ;
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpening_lf_target
// ------------------------------------------------------------------------------ //
// Target strength of sharpening effect
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_SHARPENING_LF_TARGET_DEFAULT (0x10)
#define HI_EXT_SYSTEM_SHARPENING_LF_TARGET_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_sharpening_lf_target_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U16 u32Current = IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x200a);
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x200a, (((HI_U16) data) ) | (u32Current & 0xFF00));
}
static __inline HI_U8 hi_ext_system_sharpening_lf_target_read(ISP_DEV IspDev) {
    return (HI_U8) (IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x200a) & 0x00FF) ;
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpening_sad_amplifier
// ------------------------------------------------------------------------------ //
// Target strength of sharpening effect
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_SHARPENING_SAD_AMPLIFIER_DEFAULT (0x10)
#define HI_EXT_SYSTEM_SHARPENING_SAD_AMPLIFIER_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_sharpening_sad_amplifier_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U16 u32Current = IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x200c);
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x200c, (((HI_U16) data) ) | (u32Current & 0xFF00));
}
static __inline HI_U8 hi_ext_system_sharpening_sad_amplifier_read(ISP_DEV IspDev) {
    return (HI_U8) (IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x200c) & 0x00FF) ;
}


//--------------------------------------------------------------------------------//
// bayer sharpen for auto mode
// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_agc_table_sharping_hf
// ------------------------------------------------------------------------------ //
//args: data(8-bits)

static __inline HI_VOID hi_ext_system_agc_table_sharping_hf_write(ISP_DEV IspDev, HI_U8 index, HI_U8 data){
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x200e + index, data);
}

static __inline HI_U8 hi_ext_system_agc_table_sharping_hf_read(ISP_DEV IspDev, HI_U8 index){
  return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x200e + index);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_agc_table_sharping_mf
// ------------------------------------------------------------------------------ //
//args: data(8-bits)

static __inline HI_VOID hi_ext_system_agc_table_sharping_mf_write(ISP_DEV IspDev, HI_U8 index, HI_U8 data){
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x201e + index, data);
}

static __inline HI_U8 hi_ext_system_agc_table_sharping_mf_read(ISP_DEV IspDev, HI_U8 index){
  return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x201e + index);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_agc_table_sharping_lf
// ------------------------------------------------------------------------------ //
//args: data(8-bits)

static __inline HI_VOID hi_ext_system_agc_table_sharping_lf_write(ISP_DEV IspDev, HI_U8 index, HI_U8 data){
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x202e + index, data);
}

static __inline HI_U8 hi_ext_system_agc_table_sharping_lf_read(ISP_DEV IspDev, HI_U8 index){
  return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x202e + index);
}

//--------------------------------------------------------------------------------//
// YUV sharpening for Be
//--------------------------------------------------------------------------------//

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_yuv_sharpening_enable
// ------------------------------------------------------------------------------ //
// YUV sharpening enable
// ------------------------------------------------------------------------------ //
#if 0
#define HI_EXT_SYSTEM_YUV_SHARPENING_ENABLE_DEFAULT (0x1)
#define HI_EXT_SYSTEM_YUV_SHARPENING_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_system_yuv_sharpening_enable_write(ISP_DEV IspDev, HI_U16 data){
    HI_U16 u16Current = IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2042);
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2042, ((HI_U16)(data & 0x0001)) | (u16Current & 0xFFFE));
}
static __inline HI_U8 hi_ext_system_yuv_sharpening_enable_read(ISP_DEV IspDev) {
    return (HI_U8) (IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2042) & 0x0001);
}
#endif
// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_yuv_sharpening
// ------------------------------------------------------------------------------ //
// YUV sharpening manual mode
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_MANUAL_YUV_SHARPENING_WRITE_DEFAULT (0x0)
#define HI_EXT_SYSTEM_YUV_SHARPENING_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_system_manual_yuv_sharpening_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U16 u16Current = IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2042);
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2042, ((HI_U16)((data & 0x0001) << 1)) | (u16Current & 0xFFFD));
}
static __inline HI_U8 hi_ext_system_manual_yuv_sharpening_read(ISP_DEV IspDev) {
    return (HI_U8) ((IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2042) & 0x0002) >> 1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_yuv_sharpening_TextureSt
// ------------------------------------------------------------------------------ //
// YUV sharpening strength
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_MANUAL_YUV_SHARPENING_TEXTUREST_WRITE_DEFAULT (50)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_manual_yuv_sharpening_TextureSt_write(ISP_DEV IspDev, HI_U8 data){
	IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2044, data);
}
static __inline HI_U8 hi_ext_system_manual_yuv_sharpening_TextureSt_read(ISP_DEV IspDev) {
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2044));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_yuv_sharpening_EdgeSt
// ------------------------------------------------------------------------------ //
// YUV sharpening strength
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_MANUAL_YUV_SHARPENING_EDGEST_WRITE_DEFAULT (32)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_manual_yuv_sharpening_EdgeSt_write(ISP_DEV IspDev, HI_U8 data){
	IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2045, data);
}
static __inline HI_U8 hi_ext_system_manual_yuv_sharpening_EdgeSt_read(ISP_DEV IspDev) {
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2045));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_yuv_sharpening_OverShoot
// ------------------------------------------------------------------------------ //
// YUV sharpening strength
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_YUV_SHARPENING_OVERSHOOT_WRITE_DEFAULT (100)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_manual_yuv_sharpening_OverShoot_write(ISP_DEV IspDev, HI_U8 data){
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2046 , data);
}
static __inline HI_U8 hi_ext_system_manual_yuv_sharpening_OverShoot_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2046);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_yuv_sharpening_UnderShoot
// ------------------------------------------------------------------------------ //
// YUV sharpening strength
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_YUV_SHARPENING_UNDERSHOOT_WRITE_DEFAULT (100)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_manual_yuv_sharpening_UnderShoot_write(ISP_DEV IspDev, HI_U8 data){
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2047 , data);
}
static __inline HI_U8 hi_ext_system_manual_yuv_sharpening_UnderShoot_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2047);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_yuv_sharpening_EnLowLumaShoot
// ------------------------------------------------------------------------------ //
// YUV sharpening enable lowluma shoot
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_YUV_SHARPENING_ENLOWLUMASHOOT_WRITE_DEFAULT (0)

// args: data (1-bit)
static __inline HI_VOID hi_ext_system_manual_yuv_sharpening_EnLowLumaShoot_write(ISP_DEV IspDev, HI_U8 data)
{
    HI_U8 u8Current = IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2048);
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2048, (data & 0x01) | (u8Current & 0xFE));
}
static __inline HI_U8 hi_ext_system_manual_yuv_sharpening_EnLowLumaShoot_read(ISP_DEV IspDev)
{
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2048) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_yuv_sharpening_TextureThd
// ------------------------------------------------------------------------------ //
// YUV sharpening
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_SHARPENING_TEXTURETHD_WRITE_DEFAULT (0)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_manual_yuv_sharpening_TextureThd_write(ISP_DEV IspDev, HI_U8 data){
	IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2049, data);
}
static __inline HI_U8 hi_ext_system_manual_yuv_sharpening_TextureThd_read(ISP_DEV IspDev) {
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2049));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_yuv_sharpening_TextureThd
// ------------------------------------------------------------------------------ //
// YUV sharpening
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_YUV_SHARPENING_EDGETHD_WRITE_DEFAULT (0)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_manual_yuv_sharpening_EdgeThd_write(ISP_DEV IspDev, HI_U8 data){
	IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x204a, data);
}
static __inline HI_U8 hi_ext_system_manual_yuv_sharpening_EdgeThd_read(ISP_DEV IspDev) {
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x204a));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_yuv_sharpening_JagCtrl
// ------------------------------------------------------------------------------ //
// YUV sharpening
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_YUV_SHARPENING_JAGCTRL_WRITE_DEFAULT (0)

// args: data (6-bit)
static __inline HI_VOID hi_ext_system_manual_yuv_sharpening_JagCtrl_write(ISP_DEV IspDev, HI_U8 data){
	IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x204b, data & 0x3F);
}
static __inline HI_U8 hi_ext_system_manual_yuv_sharpening_JagCtrl_read(ISP_DEV IspDev) {
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x204b) & 0x3F);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_yuv_sharpening_SaltCtrl
// ------------------------------------------------------------------------------ //
// YUV sharpening
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_YUV_SHARPENING_SALTCTRL_WRITE_DEFAULT (0)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_manual_yuv_sharpening_SaltCtrl_write(ISP_DEV IspDev, HI_U8 data){
	IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x204c, data);
}
static __inline HI_U8 hi_ext_system_manual_yuv_sharpening_SaltCtrl_read(ISP_DEV IspDev) {
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x204c));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_yuv_sharpening_PepperCtrl
// ------------------------------------------------------------------------------ //
// YUV sharpening
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_YUV_SHARPENING_PEPPERCTRL_WRITE_DEFAULT (0)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_manual_yuv_sharpening_PepperCtrl_write(ISP_DEV IspDev, HI_U8 data){
	IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x204d, data);
}
static __inline HI_U8 hi_ext_system_manual_yuv_sharpening_PepperCtrl_read(ISP_DEV IspDev) {
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x204d));
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_yuv_sharpening_DetailCtrl
// ------------------------------------------------------------------------------ //
// YUV sharpening
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_YUV_SHARPENING_DETAILCTRL_WRITE_DEFAULT (0)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_manual_yuv_sharpening_DetailCtrl_write(ISP_DEV IspDev, HI_U8 data){
	IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x204e, data);
}
static __inline HI_U8 hi_ext_system_manual_yuv_sharpening_DetailCtrl_read(ISP_DEV IspDev) {
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x204e));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_yuv_sharpening_LumaThd
// ------------------------------------------------------------------------------ //
// YUV sharpening
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_YUV_SHARPENING_LUMATHD_WRITE_DEFAULT (0xF9F0500A)

// args: data (32-bit)
static __inline HI_VOID hi_ext_system_manual_yuv_sharpening_LumaThd_write(ISP_DEV IspDev, HI_U32 data){
	IOWR_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2050, data);
}
static __inline HI_U32 hi_ext_system_manual_yuv_sharpening_LumaThd_read(ISP_DEV IspDev) {
    return (IORD_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2050));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_yuv_sharpening_LumaWgt
// ------------------------------------------------------------------------------ //
// YUV sharpening
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_YUV_SHARPENING_LUMAWGT_WRITE_DEFAULT (0xF0FFFF28)

// args: data (32-bit)
static __inline HI_VOID hi_ext_system_manual_yuv_sharpening_LumaWgt_write(ISP_DEV IspDev, HI_U32 data){
	IOWR_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2054, data);
}
static __inline HI_U32 hi_ext_system_manual_yuv_sharpening_LumaWgt_read(ISP_DEV IspDev) {
    return (IORD_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2054));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_yuv_sharpening_TextureSt
// ------------------------------------------------------------------------------ //
// YUV sharpening strength
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_YUV_SHARPENING_TEXTUREST_WRITE_DEFAULT (50)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_yuv_sharpening_TextureSt_write(ISP_DEV IspDev, HI_U8 index, HI_U8 data){
	IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2058 + index, data);
}
static __inline HI_U8 hi_ext_system_yuv_sharpening_TextureSt_read(ISP_DEV IspDev, HI_U8 index) {
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2058 + index));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_yuv_sharpening_EdgeSt
// ------------------------------------------------------------------------------ //
// YUV sharpening strength
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_YUV_SHARPENING_EDGEST_WRITE_DEFAULT (32)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_yuv_sharpening_EdgeSt_write(ISP_DEV IspDev, HI_U8 index, HI_U8 data){
	IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2068 + index, data);
}
static __inline HI_U8 hi_ext_system_yuv_sharpenng_EdgeSt_read(ISP_DEV IspDev, HI_U8 index) {
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2068 + index));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_yuv_sharpening_OverShoot
// ------------------------------------------------------------------------------ //
// YUV sharpening strength
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_YUV_SHARPENING_OVERSHOOT_WRITE_DEFAULT (100)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_yuv_sharpening_OverShoot_write(ISP_DEV IspDev, HI_U8 index, HI_U8 data){
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2078 + index, data);
}
static __inline HI_U8 hi_ext_system_yuv_sharpening_OverShoot_read(ISP_DEV IspDev, HI_U8 index) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2078 + index);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_yuv_sharpening_UnderShoot
// ------------------------------------------------------------------------------ //
// YUV sharpening strength
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_YUV_SHARPENING_UNDERSHOOT_WRITE_DEFAULT (100)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_yuv_sharpening_UnderShoot_write(ISP_DEV IspDev, HI_U8 index, HI_U8 data){
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2088 + index, data);
}
static __inline HI_U8 hi_ext_system_yuv_sharpening_UnderShoot_read(ISP_DEV IspDev, HI_U8 index) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2088 + index);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_yuv_sharpening_EnLowLumaShoot
// ------------------------------------------------------------------------------ //
// YUV sharpening enable lowluma shoot
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_YUV_SHARPENING_ENLOWLUMASHOOT_WRITE_DEFAULT (0)

// args: data (1-bit)
static __inline HI_VOID hi_ext_system_yuv_sharpening_EnLowLumaShoot_write(ISP_DEV IspDev, HI_U8 index, HI_U8 data)
{
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2098 + index, data);
}
static __inline HI_U8 hi_ext_system_yuv_sharpening_EnLowLumaShoot_read(ISP_DEV IspDev, HI_U8 index)
{
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2098 + index);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_yuv_sharpening_TextureThd
// ------------------------------------------------------------------------------ //
// YUV sharpening
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_SHARPENING_TEXTURETHD_WRITE_DEFAULT (0)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_yuv_sharpening_TextureThd_write(ISP_DEV IspDev, HI_U8 index, HI_U8 data){
	IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x20a8 + index, data);
}
static __inline HI_U8 hi_ext_system_yuv_sharpening_TextureThd_read(ISP_DEV IspDev, HI_U8 index) {
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x20a8 + index));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_yuv_sharpening_TextureThd
// ------------------------------------------------------------------------------ //
// YUV sharpening
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_YUV_SHARPENING_EDGETHD_WRITE_DEFAULT (0)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_yuv_sharpening_EdgeThd_write(ISP_DEV IspDev, HI_U8 index, HI_U8 data){
	IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x20b8 + index, data);
}
static __inline HI_U8 hi_ext_system_yuv_sharpening_EdgeThd_read(ISP_DEV IspDev, HI_U8 index) {
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x20b8 + index));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_yuv_sharpening_JagCtrl
// ------------------------------------------------------------------------------ //
// YUV sharpening
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_YUV_SHARPENING_JAGCTRL_WRITE_DEFAULT (0)

// args: data (6-bit)
static __inline HI_VOID hi_ext_system_yuv_sharpening_JagCtrl_write(ISP_DEV IspDev, HI_U8 index, HI_U8 data){
	IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x20c8 + index, data & 0x3F);
}
static __inline HI_U8 hi_ext_system_yuv_sharpening_JagCtrl_read(ISP_DEV IspDev, HI_U8 index) {
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x20c8 + index) & 0x3F);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_yuv_sharpening_SaltCtrl
// ------------------------------------------------------------------------------ //
// YUV sharpening
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_YUV_SHARPENING_SALTCTRL_WRITE_DEFAULT (0)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_yuv_sharpening_SaltCtrl_write(ISP_DEV IspDev, HI_U8 index, HI_U8 data){
	IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x20d8 + index, data);
}
static __inline HI_U8 hi_ext_system_yuv_sharpening_SaltCtrl_read(ISP_DEV IspDev, HI_U8 index) {
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x20d8 + index));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_yuv_sharpening_PepperCtrl
// ------------------------------------------------------------------------------ //
// YUV sharpening
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_YUV_SHARPENING_PEPPERCTRL_WRITE_DEFAULT (0)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_yuv_sharpening_PepperCtrl_write(ISP_DEV IspDev, HI_U8 index, HI_U8 data){
	IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x20e8 + index, data);
}
static __inline HI_U8 hi_ext_system_yuv_sharpening_PepperCtrl_read(ISP_DEV IspDev, HI_U8 index) {
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x20e8 + index));
}

//=============================================================//
//other ISP module: 0x12100~0x121FF
//=============================================================//

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_window_mode
// ------------------------------------------------------------------------------ //
// window mode
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_WINDOW_MODE_DEFAULT (0)
#define HI_EXT_SYSTEM_WINDOW_MODE_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_window_mode_write(ISP_DEV IspDev, HI_U8 data)
{
    HI_U16 u32Current = IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2100);
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2100, (((HI_U16) data) << 8) | (u32Current & 0x00FF));
}
static __inline HI_U8 hi_ext_system_window_mode_read(ISP_DEV IspDev)
{
    return (HI_U8) ((IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2100) & 0xFF00) >> 8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_bad_pixel_finish_thresh
// ------------------------------------------------------------------------------ //
// define the finish thresh of defect pixel detection
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_BAD_PIXEL_FINISH_THRESH_DEFAULT (0x1f)
#define HI_EXT_SYSTEM_BAD_PIXEL_FINISH_THRESH_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_bad_pixel_finish_thresh_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U16 u32Current = IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2104);
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2104, ((HI_U16) data) | (u32Current & 0xFF00));
}
static __inline HI_U8 hi_ext_system_bad_pixel_finish_thresh_read(ISP_DEV IspDev) {
    return (HI_U8) (IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2104) & 0x00FF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_bad_pixel_thresh
// ------------------------------------------------------------------------------ //
// define the start thresh of defect pixel detection
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_BAD_PIXEL_START_THRESH_DEFAULT (0x1f)
#define HI_EXT_SYSTEM_BAD_PIXEL_START_THRESH_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_bad_pixel_start_thresh_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U16 u32Current = IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2106);
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2106, ((HI_U16) data) | (u32Current & 0xFF00));
}
static __inline HI_U8 hi_ext_system_bad_pixel_start_thresh_read(ISP_DEV IspDev) {
    return (HI_U8) (IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2106) & 0x00FF);
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_bad_pixel_trigger_status
// ------------------------------------------------------------------------------ //
// the static bad pixel trigger status
// 0:Initial status
// 1:Finished
// 2:time out
//
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_BAD_PIXEL_TRIGGER_STATUS_DEFAULT (0x0)
#define HI_EXT_SYSTEM_BAD_PIXEL_TRIGGER_STATUS_DATASIZE (2)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_bad_pixel_trigger_status_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U16 u32Current = IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2106);
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2106, (((HI_U16) data) << 8) | (u32Current & 0xFCFF));
}
static __inline HI_U8 hi_ext_system_bad_pixel_trigger_status_read(ISP_DEV IspDev) {
    return (HI_U8) ((IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2106) & 0x0300) >> 8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_bad_pixel_trigger_time
// ------------------------------------------------------------------------------ //
// the bad pixel trigger frame cnt of time out
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_BAD_PIXEL_TRIGGER_TIME_DEFAULT (0x640)
#define HI_EXT_SYSTEM_BAD_PIXEL_TRIGGER_TIME_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_system_bad_pixel_trigger_time_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2108, data);
}
static __inline HI_U16 hi_ext_system_bad_pixel_trigger_time_read(ISP_DEV IspDev) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2108) ;
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_bad_pixel_count_max
// ------------------------------------------------------------------------------ //
// bad pixel calibration  the maxmum bad pixel count
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_BAD_PIXEL_COUNT_MAX_DEFAULT (0x100)
#define HI_EXT_SYSTEM_BAD_PIXEL_COUNT_MAX_DATASIZE (16)

// args: data (8-bit)
// data format: unsigned 1.7-bit fixed-point
static __inline HI_VOID hi_ext_system_bad_pixel_count_max_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2110, data);
}
static __inline HI_U16 hi_ext_system_bad_pixel_count_max_read(ISP_DEV IspDev) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2110);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_bad_pixel_count_min
// ------------------------------------------------------------------------------ //
// bad pixel calibration  the minmum bad pixel count
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_BAD_PIXEL_COUNT_MIN_DEFAULT (0x40)
#define HI_EXT_SYSTEM_BAD_PIXEL_COUNT_MIN_DATASIZE (16)

// args: data (8-bit)
// data format: unsigned 1.7-bit fixed-point
static __inline HI_VOID hi_ext_system_bad_pixel_count_min_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2112, data);
}
static __inline HI_U16 hi_ext_system_bad_pixel_count_min_read(ISP_DEV IspDev) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2112);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_low_light_enable
// ------------------------------------------------------------------------------ //
//
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_LOW_LIGHT_ENABLE_DEFAULT (0x0)
#define HI_EXT_SYSTEM_LOW_LIGHT_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_U8 hi_ext_system_low_light_enable_read(ISP_DEV IspDev) {
    return (HI_U8) ((IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2114) & 0x0001));
}
static __inline HI_VOID hi_ext_system_low_light_enable_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U16 u32Current = IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2114);
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2114, ((HI_U16) (data & 0x0001)) | (u32Current & 0xFFFE));
}

#if 0
// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_exp_ratio_enable
// ------------------------------------------------------------------------------ //
// 0:auto exposure ratio, 1:manual exposure ratio. only used in frame switching mode.
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_MANUAL_EXP_RATIO_ENABLE_DEFAULT (0x0)
#define HI_EXT_SYSTEM_MANUAL_EXP_RATIO_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_system_manual_exp_ratio_enable_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U16 u32Current = IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2114);
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2114, (((HI_U16) (data & 0x0001)) << 1) | (u32Current & 0xFFFD));
}
static __inline HI_U8 hi_ext_system_manual_exp_ratio_enable_read(ISP_DEV IspDev) {
    return (HI_U8) ((IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2114) & 0x0002) >> 1);
}
#endif

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sensor_wdr_mode
// ------------------------------------------------------------------------------ //
// WDR mode: 0=linear 1=built-in wdr 2=2to1 full frame wdr  ...
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_SENSOR_WDR_MODE_DEFAULT (0x0)
#define HI_EXT_SYSTEM_SENSOR_WDR_MODE_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_ext_system_sensor_wdr_mode_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U16 u32Current = IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2114);
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2114, (((HI_U16) (data & 0x003F)) << 4) | (u32Current & 0xFC0F));
}
static __inline HI_U8 hi_ext_system_sensor_wdr_mode_read(ISP_DEV IspDev) {
    return (HI_U8) ((IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2114) & 0x03F0) >> 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_static_defect_pixel_detect_type
// ------------------------------------------------------------------------------ //
// ISP_STATIC_DP_BRIGHT:0x0,ISP_STATIC_DP_DARK:0x1
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_BAD_PIXEL_DETECT_TYPE_DEFAULT (0x0)
#define HI_EXT_SYSTEM_BAD_PIXEL_DETECT_TYPE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_system_bad_pixel_detect_type_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2116, data&0x1);
}
static __inline HI_U8 hi_ext_system_bad_pixel_detect_type_read(ISP_DEV IspDev) {
    return  (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2116) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_gamma_curve_type
// ------------------------------------------------------------------------------ //
// the type of gamma curve
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_GAMMA_CURVE_TYPE_DEFAULT (0)
#define HI_EXT_SYSTEM_GAMMA_CURVE_TYPE_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_gamma_curve_type_write(ISP_DEV IspDev, HI_U8 data)
{
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2120, data);
}
static __inline HI_U8 hi_ext_system_gamma_curve_type_read(ISP_DEV IspDev)
{
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2120);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sys_debug_enable
// ------------------------------------------------------------------------------ //
// 0:disable system debug information
// 1:enable system debug information
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_SYS_DEBUG_ENABLE_DEFAULT (0x0)
#define HI_EXT_SYSTEM_SYS_DEBUG_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_system_sys_debug_enable_write(ISP_DEV IspDev, HI_U16 data)
{
    HI_U16 u32Current = IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x218C);
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x218C, (((HI_U16) data) << 0) | (u32Current & 0xFFFE));
}

static __inline HI_U16 hi_ext_system_sys_debug_enable_read(ISP_DEV IspDev)
{
    return (HI_U16) ((IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x218C) & 0x0001) >> 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sys_debug_addr
// ------------------------------------------------------------------------------ //
// address of system debug information
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_SYS_DEBUG_ADDR_DEFAULT (0x0)
#define HI_EXT_SYSTEM_SYS_DEBUG_ADDR_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_ext_system_sys_debug_addr_write(ISP_DEV IspDev, HI_U32 data)
{
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2190, (HI_U16)(data & 0xFFFF) );
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2192, (HI_U16)((data & 0xFFFF0000) >> 16) );
}

static __inline HI_U32 hi_ext_system_sys_debug_addr_read(ISP_DEV IspDev)
{
    return (HI_U32)IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2190) + ((HI_U32)IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2192) << 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sys_debug_size
// ------------------------------------------------------------------------------ //
// size of sys debug struct
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_SYS_DEBUG_SIZE_DEFAULT (0x0)
#define HI_EXT_SYSTEM_SYS_DEBUG_SIZE_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_ext_system_sys_debug_size_write(ISP_DEV IspDev, HI_U32 data)
{
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2194, (HI_U16)(data & 0xFFFF) );
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2196, (HI_U16)((data & 0xFFFF0000) >> 16) );
}

static __inline HI_U32 hi_ext_system_sys_debug_size_read(ISP_DEV IspDev)
{
    return (HI_U32)IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2194) + ((HI_U32)IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2196) << 16);
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_debug_depth
// ------------------------------------------------------------------------------ //
// the depth of all debug information
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DEBUG_DEPTH_DEFAULT (0x0)
#define HI_EXT_SYSTEM_DEBUG_DEPTH_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_system_sys_debug_depth_write(ISP_DEV IspDev, HI_U16 data)
{
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2198, data);
}

static __inline HI_U16 hi_ext_system_sys_debug_depth_read(ISP_DEV IspDev)
{
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2198);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_shading_table_node_number
// ------------------------------------------------------------------------------ //
// the node number of shading table
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_SHADING_TABLE_NODE_NUMBER_DEFAULT (0x0081)
#define HI_EXT_SYSTEM_SHADING_TABLE_NODE_NUMBER_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_system_shading_table_node_number_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x219a, data);
}
static __inline HI_U16 hi_ext_system_shading_table_node_number_read(ISP_DEV IspDev) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x219a);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dynamic_defect_pixel_slope
// ------------------------------------------------------------------------------ //
// slope of dynamic defect pixel
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DYNAMIC_DEFECT_PIXEL_SLOPE_DEFAULT (0x200)
#define HI_EXT_SYSTEM_DYNAMIC_DEFECT_PIXEL_SLOPE_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_system_dynamic_defect_pixel_slope_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x219c, data);
}
static __inline HI_U16 hi_ext_system_dynamic_defect_pixel_slope_read(ISP_DEV IspDev) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x219c);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dynamic_defect_pixel_thresh
// ------------------------------------------------------------------------------ //
// thresh of dynamic defect pixel
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DYNAMIC_DEFECT_PIXEL_THRESH_DEFAULT (0x40)
#define HI_EXT_SYSTEM_DYNAMIC_DEFECT_PIXEL_THRESH_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_system_dynamic_defect_pixel_thresh_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x219e, data);
}
static __inline HI_U16 hi_ext_system_dynamic_defect_pixel_thresh_read(ISP_DEV IspDev) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x219e);
}

// ------------------------------------------------------------------------------ //


//==============================================================//
//Frame stitch WDR: 0x121A0~0x121BF
//==============================================================//
#if 0   /* move to 'hi_ae_ext_config.h' */
// ------------------------------------------------------------------------------ //
// Register: hi_ext_wdr_manual_exp_ratio_enable
// ------------------------------------------------------------------------------ //
// 0: auto exposure ratio; 1: manual exposure ratio;
// ------------------------------------------------------------------------------ //

#define HI_EXT_WDR_MANUAL_EXP_RATIO_ENABLE_DEFAULT (0x0)
#define HI_EXT_WDR_MANUAL_EXP_RATIO_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_wdr_manual_exp_ratio_enable_write(ISP_DEV IspDev, HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x21A0);
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x21A0, ((data & 0x0001) << 0) | (u32Current & 0xFE));
}
static __inline HI_U8 hi_ext_wdr_manual_exp_ratio_enable_read(ISP_DEV IspDev) {
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x21A0) & 0x1) >> 0 ;
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_wdr_exp_ratio_target
// ------------------------------------------------------------------------------ //
// exposure ratio target
// ------------------------------------------------------------------------------ //

#define HI_EXT_WDR_EXP_RATIO_TARGET_DEFAULT (0x400)
#define HI_EXT_WDR_EXP_RATIO_TARGET_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_ext_wdr_exp_ratio_target_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x21A2, data & 0xFFF);
}
static __inline HI_U16 hi_ext_wdr_exp_ratio_target_read(ISP_DEV IspDev) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x21A2) & 0xFFF;
}
#endif

// ------------------------------------------------------------------------------ //
// Register: hi_ext_wdr_compress_mode
// ------------------------------------------------------------------------------ //
// buffer number in WDR compress mode
// ------------------------------------------------------------------------------ //

#define HI_EXT_WDR_COMPRESS_DEFAULT (0x1)
#define HI_EXT_WDR_COMPRESS_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_wdr_compress_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x21A4, data & 0xFFFF);
}
static __inline HI_U16 hi_ext_wdr_compress_read(ISP_DEV IspDev) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x21A4) & 0xFFFF;
}

//==============================================================//
//Dehaze: 0x12300~0x123FF
//==============================================================//

// ------------------------------------------------------------------------------ //
// Register: isp_ext_system_manual_dehaze_enable
// ------------------------------------------------------------------------------ //
// Enables manual dehaze: 0=auto 1=manual
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_MANUAL_DEHAZE_ENABLE_DEFAULT (1)
#define ISP_EXT_SYSTEM_MANUAL_DEHAZE_ENABLE_DATASIZE (1)

// args: data (8-bit)
static __inline void isp_ext_system_manual_dehaze_enable_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2300, data);
}
static __inline HI_U32 isp_ext_system_manual_dehaze_enable_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2300);
}

// ------------------------------------------------------------------------------ //
// Register: isp_ext_system_manual_dehaze_hblk
// ------------------------------------------------------------------------------ //
// manual dehaze hblk number
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_MANUAL_DEHAZE_HBLK_DEFAULT (32)
#define ISP_EXT_SYSTEM_MANUAL_DEHAZE_HBLK_DATASIZE (8)

// args: data (8-bit)
static __inline void isp_ext_system_manual_dehaze_hblk_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2301, data);
}
static __inline HI_U8 isp_ext_system_manual_dehaze_hblk_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2301);
}

// ------------------------------------------------------------------------------ //
// Register: isp_ext_system_manual_dehaze_vblk
// ------------------------------------------------------------------------------ //
// manual dehaze vblk number
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_MANUAL_DEHAZE_VBLK_DEFAULT (30)
#define ISP_EXT_SYSTEM_MANUAL_DEHAZE_VBLK_DATASIZE (8)

// args: data (8-bit)
static __inline void isp_ext_system_manual_dehaze_vblk_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2302, data);
}
static __inline HI_U8 isp_ext_system_manual_dehaze_vblk_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2302);
}

// ------------------------------------------------------------------------------ //
// Register: isp_ext_system_manual_dehaze_ctp
// ------------------------------------------------------------------------------ //
// manual dehaze curve turn point
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_DEHAZE_CTP_DEFAULT (250)
#define ISP_EXT_SYSTEM_DEHAZE_CTP_DATASIZE (8)

// args: data (8-bit)
static __inline void isp_ext_system_dehaze_ctp_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2303, data);
}
static __inline HI_U8 isp_ext_system_dehaze_ctp_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2303);
}

// ------------------------------------------------------------------------------ //
// Register: isp_ext_system_manual_dehaze_mft
// ------------------------------------------------------------------------------ //
// manual dehaze minumum filter threshold
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_DEHAZE_MFT_DEFAULT (128)
#define ISP_EXT_SYSTEM_MANUAL_DEHAZE_MFT_DATASIZE (16)

// args: data (10-bit)
static __inline void isp_ext_system_dehaze_mft_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2304, data);
}
static __inline HI_U16 isp_ext_system_dehaze_mft_read(ISP_DEV IspDev) {
    return (IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2304)&0x3ff);
}

// ------------------------------------------------------------------------------ //
// Register: isp_ext_system_manual_dehaze_mfs
// ------------------------------------------------------------------------------ //
// manual dehaze minumum filter size
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_DEHAZE_MFS_DEFAULT (7)
#define ISP_EXT_SYSTEM_DEHAZE_MFS_DATASIZE (8)

// args: data (8-bit)
static __inline void isp_ext_system_dehaze_mfs_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2306, data);
}
static __inline HI_U8 isp_ext_system_dehaze_mfs_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2306);
}

// ------------------------------------------------------------------------------ //
// Register: isp_ext_system_manual_dehaze_ct
// ------------------------------------------------------------------------------ //
// manual dehaze change threshold
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_DEHAZE_CT_DEFAULT (255)
#define ISP_EXT_SYSTEM_DEHAZE_CT_DATASIZE (8)

// args: data (8-bit)
static __inline void isp_ext_system_dehaze_ct_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2307, data);
}
static __inline HI_U8 isp_ext_system_dehaze_ct_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2307);
}

// ------------------------------------------------------------------------------ //
// Register: isp_ext_system_manual_dehaze_tfic
// ------------------------------------------------------------------------------ //
// manual dehaze temper filter increase ceofficient
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_DEHAZE_TFIC_DEFAULT (64)
#define ISP_EXT_SYSTEM_DEHAZE_TFIC_DATASIZE (8)

// args: data (8-bit)
static __inline void isp_ext_system_dehaze_tfic_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2308, data);
}
static __inline HI_U8 isp_ext_system_dehaze_tfic_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2308);
}

// ------------------------------------------------------------------------------ //
// Register: isp_ext_system_manual_dehaze_tfdc
// ------------------------------------------------------------------------------ //
// manual dehaze temper filter decrease ceofficient
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_DEHAZE_TFDC_DEFAULT (64)
#define ISP_EXT_SYSTEM_DEHAZE_TFDC_DATASIZE (8)

// args: data (8-bit)
static __inline void isp_ext_system_dehaze_tfdc_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2309, data);
}
static __inline HI_U16 isp_ext_system_dehaze_tfdc_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2309);
}

#define ISP_EXT_SYSTEM_DEHAZE_ENABLE_DEFAULT (0)
#define ISP_EXT_SYSTEM_DEHAZE_ENABLE_DATASIZE (8)

// args: data (8-bit)
static __inline void isp_ext_system_dehaze_enable_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x230b, data);
}
static __inline HI_U16 isp_ext_system_dehaze_enable_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x230b);
}


// ------------------------------------------------------------------------------ //
// Register: isp_ext_system_dehaze_debug_enable
// ------------------------------------------------------------------------------ //
// dehaze debug enable
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_DEHAZE_DEBUG_ENABLE_DEFAULT (0)
#define ISP_EXT_SYSTEM_DEHAZE_DEBUG_ENABLE_DATASIZE (8)

// args: data (8-bit)
static __inline void isp_ext_system_dehaze_debug_enable_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x230C, data);
}
static __inline HI_U8 isp_ext_system_dehaze_debug_enable_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x230C);
}

// ------------------------------------------------------------------------------ //
// Register: isp_ext_system_manual_dehaze_strength
// ------------------------------------------------------------------------------ //
// dehaze strength
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_MANUAL_DEHAZE_STRENGTH_DEFAULT  (0xD2)
#define ISP_EXT_SYSTEM_MANUAL_DEHAZE_STRENGTH_DATASIZE (8)

// args: data (8-bit)
static __inline void isp_ext_system_manual_dehaze_strength_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x230D, data);
}
static __inline HI_U8 isp_ext_system_manual_dehaze_strength_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x230D);
}

// ------------------------------------------------------------------------------ //
// Register: isp_ext_system_manual_fpn_strength
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_FPN_STRENGTH_DEFAULT  (0)
#define ISP_EXT_SYSTEM_FPN_STRENGTH_DATASIZE (16)

static __inline void isp_ext_system_manual_fpn_strength_write(ISP_DEV IspDev, HI_U16 data)
{
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x230E, data);
}
static __inline HI_U16 isp_ext_system_manual_fpn_strength_read(ISP_DEV IspDev)
{
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x230E);
}

// ------------------------------------------------------------------------------ //
// Register: isp_ext_system_manual_fpn_opmode
// ------------------------------------------------------------------------------ //
static __inline void isp_ext_system_manual_fpn_opmode_write(ISP_DEV IspDev, HI_U8 data)
{
     HI_U16 u32Current = IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2310);
     IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2310, ((HI_U16)data) |(u32Current & 0xFF00));
}

static __inline HI_U8 isp_ext_system_manual_fpn_opmode_read(ISP_DEV IspDev)
{
    return (HI_U8) (IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2310) & 0x00FF);
}


// ------------------------------------------------------------------------------ //
// Register: isp_ext_system_manual_fpn_update
// ------------------------------------------------------------------------------ //

static __inline void isp_ext_system_manual_fpn_update_write(ISP_DEV IspDev, HI_U8 data)
{
     HI_U16 u32Current = IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2312);
     IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2312, ((HI_U16)data) |(u32Current & 0xFF00));
}

static __inline HI_U8 isp_ext_system_manual_fpn_update_read(ISP_DEV IspDev)
{
    return (HI_U8) (IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2312) & 0x00FF);
}

// ------------------------------------------------------------------------------ //
// Register: isp_ext_system_manual_fpn_CorrCfg
// ------------------------------------------------------------------------------ //

static __inline void isp_ext_system_manual_fpn_CorrCfg_write(ISP_DEV IspDev, HI_U8 data)
{
    HI_U16 u32Current = IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2314);
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2314, ((HI_U16)data) |(u32Current & 0xFF00));
}

static __inline HI_U8 isp_ext_system_manual_fpn_CorrCfg_read(ISP_DEV IspDev)
{
    return (HI_U8) (IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2314) & 0x00FF);
}

#define ISP_EXT_SYSTEM_MANUAL_DEHAZE_AUTOSTRENGTH_DEFAULT  (0xf0)
#define ISP_EXT_SYSTEM_MANUAL_DEHAZE_AUTOSTRENGTH_DATASIZE (8)

// args: data (8-bit)
static __inline void isp_ext_system_manual_dehaze_autostrength_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2316, data);
}
static __inline HI_U8 isp_ext_system_manual_dehaze_autostrength_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2316);
}

/* write the ISO of calibration */
static __inline void isp_ext_system_manual_FPN_ISO_write(ISP_DEV IspDev, HI_U32 data)
{
    IOWR_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2317, data);
}
/* read the ISO of calibration */
static __inline HI_U32 isp_ext_system_manual_FPN_ISO_read(ISP_DEV IspDev)
{
    return IORD_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2317);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_agc_table_sharp_alt_d
// ------------------------------------------------------------------------------ //
//args: data(8-bits)

static __inline HI_VOID hi_ext_system_agc_table_sharp_alt_d_write(ISP_DEV IspDev, HI_U8 index, HI_U8 data){
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2326 + index, data);
}

static __inline HI_U8 hi_ext_system_agc_table_sharp_alt_d_read(ISP_DEV IspDev, HI_U8 index){
  return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2326 + index);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_agc_table_sharp_alt_ud
// ------------------------------------------------------------------------------ //
//args: data(8-bits)

static __inline HI_VOID hi_ext_system_agc_table_sharp_alt_ud_write(ISP_DEV IspDev, HI_U8 index, HI_U8 data){
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2336 + index, data);
}

static __inline HI_U8 hi_ext_system_agc_table_sharp_alt_ud_read(ISP_DEV IspDev, HI_U8 index){
  return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2336 + index);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_agc_table_RGBsharp_strength
// ------------------------------------------------------------------------------ //
//args: data(8-bits)

//static __inline HI_VOID hi_ext_system_agc_table_RGBsharp_strength_write(ISP_DEV IspDev, HI_U8 index,HI_U8 data){
//    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2346 + index, data);
//}


//static __inline HI_U8 hi_ext_system_agc_table_RGBsharp_strength_read(ISP_DEV IspDev, HI_U8 index){
//  return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2346 + index);
//}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_agc_table_snr_thresh
// ------------------------------------------------------------------------------ //
//args: data(8-bits)
static __inline HI_VOID hi_ext_system_agc_table_snr_thresh_write(ISP_DEV IspDev, HI_U8 index,HI_U8 data){
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2356 + index, data);
}

static __inline HI_U8 hi_ext_system_agc_table_snr_thresh_read(ISP_DEV IspDev, HI_U8 index){
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2356 + index);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_agc_table_ge_strength
// ------------------------------------------------------------------------------ //

static __inline HI_VOID hi_ext_system_agc_table_ge_strength_write(ISP_DEV IspDev, HI_U8 index,HI_U8 data){
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2366 + index, data);
}

static __inline HI_U8 hi_ext_system_agc_table_ge_strength_read(ISP_DEV IspDev, HI_U8 index){
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2366 + index);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_agc_table_demosaic_np_offset
// ------------------------------------------------------------------------------ //
//args: data(8-bits)
static __inline HI_VOID hi_ext_system_agc_table_demosaic_np_offset_write(ISP_DEV IspDev, HI_U8 index,HI_U8 data){
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2376 + index, data);
}

static __inline HI_U8 hi_ext_system_agc_table_demosaic_np_offset_read(ISP_DEV IspDev, HI_U8 index){
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2376 + index);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sensor_max_resolution_width
// ------------------------------------------------------------------------------ //
// width of sensor resolution
// ------------------------------------------------------------------------------ //


#define HI_EXT_SYSTEM_SENSOR_MAX_RESOLUTION_WIDTH_DEFAULT (1280)
#define HI_EXT_SYSTEM_SENSOR_MAX_RESOLUTION_WIDTH_DATASIZE (16)

// args: data (16-bit)

static __inline HI_VOID hi_ext_system_sensor_max_resolution_width_write(ISP_DEV IspDev, HI_U16 data){
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2396, data);
}

static __inline HI_U16 hi_ext_system_sensor_max_resolution_width_read(ISP_DEV IspDev){
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2396);
}



// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sensor_max_resolution_height
// ------------------------------------------------------------------------------ //
// height of sensor resolution
// ------------------------------------------------------------------------------ //


#define HI_EXT_SYSTEM_SENSOR_MAX_RESOLUTION_HEIGHT_DEFAULT (720)
#define HI_EXT_SYSTEM_SENSOR_MAX_RESOLUTION_HEIGHT_DATASIZE (16)

// args: data (16-bit)

static __inline HI_VOID hi_ext_system_sensor_max_resolution_height_write(ISP_DEV IspDev, HI_U16 data){
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2398, data);
}

static __inline HI_U16 hi_ext_system_sensor_max_resolution_height_read(ISP_DEV IspDev){
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2398);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sensor_max_resolution_fps
// ------------------------------------------------------------------------------ //
// fps of sensor output
// ------------------------------------------------------------------------------ //


#define HI_EXT_SYSTEM_SENSOR_MAX_RESOLUTION_FPS_DEFAULT (30)
#define HI_EXT_SYSTEM_SENSOR_MAX_RESOLUTION_FPS_DATASIZE (16)

// args: data (16-bit)

static __inline HI_VOID hi_ext_system_sensor_max_resolution_fps_write(ISP_DEV IspDev, HI_U16 data){
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x239a, data);
}

static __inline HI_U16 hi_ext_system_sensor_max_resolution_fps_read(ISP_DEV IspDev){
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x239a);
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_statistics_ctrl
// ------------------------------------------------------------------------------ //
// ISP statistics control
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_STATISTICS_DEFAULT  (0xfffffffd)
#define HI_EXT_SYSTEM_STATISTICS_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_ext_system_statistics_ctrl_read(ISP_DEV IspDev)
{
    return  IORD_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x239c);
}

static __inline HI_VOID hi_ext_system_statistics_ctrl_write(ISP_DEV IspDev, HI_U32 data)
{
     IOWR_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x239c, data);
}

// ------------------------------------------------------------------------------ //
// Register: isp_ext_gamma_rgb_mem_array_data
// ------------------------------------------------------------------------------ //
// Please see other documentation for a description of the contents of this array.
// ------------------------------------------------------------------------------ //

#define HI_ISP_EXT_GAMMA_RGB_MEM_ARRAY_DATA_DEFAULT (0x0)
#define HI_ISP_EXT_GAMMA_RGB_MEM_ARRAY_DATA_DATASIZE (16)

// args: index (0-256), data (16-bit)
static __inline void hi_isp_ext_gamma_rgb_mem_array_data_write(ISP_DEV IspDev, HI_U16 index, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x23a0 + (index << 1), data);
}

static __inline HI_U16 hi_isp_ext_gamma_rgb_mem_array_data_read(ISP_DEV IspDev, HI_U16 index) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x23a0 + (index << 1));
}


#if 0
// ------------------------------------------------------------------------------ //
// Register: isp_ext_system_gamma_change
// ------------------------------------------------------------------------------ //
// Gamma change config control
// ------------------------------------------------------------------------------ //

#define HI_ISP_EXT_SYSTEM_GAMMA_CHANGE_DEFAULT (0)
#define HI_ISP_EXT_SYSTEM_GAMMA_CHANGE_DATASIZE (1)

// args: data (1-bit)
static __inline void hi_isp_ext_system_gamma_change_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x25a2, (data & 0x1));
}
static __inline HI_U32 hi_isp_ext_system_gamma_change_read(ISP_DEV IspDev) {
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x25a2)& 0x1);
}
#endif

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_black_level_00
// ------------------------------------------------------------------------------ //
// ISP black level 00
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_BLACK_LEVEL_00_DEFAULT  (0x0)
#define HI_EXT_SYSTEM_BLACK_LEVEL_00_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_ext_system_black_level_00_read(ISP_DEV IspDev)
{
    return  IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x25a4);
}

static __inline HI_VOID hi_ext_system_black_level_00_write(ISP_DEV IspDev, HI_U16 data)
{
     IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x25a4, data);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_black_level_01
// ------------------------------------------------------------------------------ //
// ISP black level 01
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_BLACK_LEVEL_01_DEFAULT  (0x0)
#define HI_EXT_SYSTEM_BLACK_LEVEL_01_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_ext_system_black_level_01_read(ISP_DEV IspDev)
{
    return  IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x25a6);
}

static __inline HI_VOID hi_ext_system_black_level_01_write(ISP_DEV IspDev, HI_U16 data)
{
     IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x25a6, data);
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_black_level_10
// ------------------------------------------------------------------------------ //
// ISP black level 10
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_BLACK_LEVEL_10_DEFAULT  (0x0)
#define HI_EXT_SYSTEM_BLACK_LEVEL_10_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_ext_system_black_level_10_read(ISP_DEV IspDev)
{
    return  IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x25a8);
}

static __inline HI_VOID hi_ext_system_black_level_10_write(ISP_DEV IspDev, HI_U16 data)
{
     IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x25a8, data);
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_black_level_11
// ------------------------------------------------------------------------------ //
// ISP black level 11
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_BLACK_LEVEL_11_DEFAULT  (0x0)
#define HI_EXT_SYSTEM_BLACK_LEVEL_11_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_ext_system_black_level_11_read(ISP_DEV IspDev)
{
    return  IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x25aa);
}

static __inline HI_VOID hi_ext_system_black_level_11_write(ISP_DEV IspDev, HI_U16 data)
{
     IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x25aa, data);
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_black_level_change
// ------------------------------------------------------------------------------ //
// ISP black level change
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_BLACK_LEVEL_CHANGE_DEFAULT  (0x0)
#define HI_EXT_SYSTEM_BLACK_LEVEL_CHANGE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_U8 hi_ext_system_black_level_change_read(ISP_DEV IspDev)
{
    return (HI_U8)(IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x25ac) & 0x0001);
}

static __inline HI_VOID hi_ext_system_black_level_change_write(ISP_DEV IspDev, HI_U8 data)
{
    HI_U16 u32Current = IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x25ac);
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x25ac, ((HI_U16) (data & 0x0001)) | (u32Current & 0xFFFE));
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sensor_iso
// ------------------------------------------------------------------------------ //
// Sensor ISO
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_SENSOR_ISO_DEFAULT  (0x0)
#define HI_EXT_SYSTEM_SENSOR_ISO_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_ext_system_sensor_iso_read(ISP_DEV IspDev)
{
    return  IORD_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x25b0);
}

static __inline HI_VOID hi_ext_system_sensor_iso_write(ISP_DEV IspDev, HI_U32 data)
{
     IOWR_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x25b0, data);
}
// ------------------------------------------------------------------------------ //
// Register: hi_ext_system__iso
// ------------------------------------------------------------------------------ //
// sys ISO
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_SYS_ISO_DEFAULT  (0x0)
#define HI_EXT_SYSTEM_SYS_ISO_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_ext_system_sys_iso_read(ISP_DEV IspDev)
{
    return  IORD_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x25b4);
}

static __inline HI_VOID hi_ext_system_sys_iso_write(ISP_DEV IspDev, HI_U32 data)
{
     IOWR_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x25b4, data);
}
#if 0
// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_shading_type
// ------------------------------------------------------------------------------ //
// Shading Type
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_SHADING_TYPE_DEFAULT  (0x0)
#define HI_EXT_SYSTEM_SHADING_TYPE_DATASIZE (1)

// args: data (32-bit)
static __inline HI_U32 hi_ext_system_shading_type_read(ISP_DEV IspDev)
{
    return  (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x25c0) & 0x1);
}

static __inline HI_VOID hi_ext_system_shading_type_write(ISP_DEV IspDev, HI_U8 data)
{
     IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x25c0, (data & 0x1));
}
#endif

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_mesh_shading_color_temperature_h
// ------------------------------------------------------------------------------ //
// mesh shading color temperature high
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_MESH_COL_TEMP_H_DEFAULT  (0x0)
#define HI_EXT_SYSTEM_MESH_COL_TEMP_H_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_ext_system_mesh_col_temp_h_read(ISP_DEV IspDev)
{
    return  IORD_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x25c4);
}

static __inline HI_VOID hi_ext_system_mesh_col_temp_h_write(ISP_DEV IspDev, HI_U32 data)
{
     IOWR_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x25c4, data);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_mesh_shading_color_temperature_m
// ------------------------------------------------------------------------------ //
// mesh shading color temperature median
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_MESH_COL_TEMP_M_DEFAULT  (0x0)
#define HI_EXT_SYSTEM_MESH_COL_TEMP_M_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_ext_system_mesh_col_temp_m_read(ISP_DEV IspDev)
{
    return  IORD_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x25c8);
}

static __inline HI_VOID hi_ext_system_mesh_col_temp_m_write(ISP_DEV IspDev, HI_U32 data)
{
     IOWR_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x25c8, data);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_mesh_shading_color_temperature_l
// ------------------------------------------------------------------------------ //
// mesh shading color temperature low
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_MESH_COL_TEMP_L_DEFAULT  (0x0)
#define HI_EXT_SYSTEM_MESH_COL_TEMP_L_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_ext_system_mesh_col_temp_l_read(ISP_DEV IspDev)
{
    return  IORD_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x25cc);
}

static __inline HI_VOID hi_ext_system_mesh_col_temp_l_write(ISP_DEV IspDev, HI_U32 data)
{
     IOWR_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x25cc, data);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_mesh_shading_color_temperature_ll
// ------------------------------------------------------------------------------ //
// mesh shading color temperature lowest
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_MESH_COL_TEMP_LL_DEFAULT  (0x0)
#define HI_EXT_SYSTEM_MESH_COL_TEMP_LL_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_ext_system_mesh_col_temp_ll_read(ISP_DEV IspDev)
{
    return  IORD_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x25d0);
}

static __inline HI_VOID hi_ext_system_mesh_col_temp_ll_write(ISP_DEV IspDev, HI_U32 data)
{
     IOWR_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x25d0, data);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_yuv_sharpening_LumaThd
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_YUV_SHARPENING_LUMATHD_WRITE_DEFAULT (0)

// args: data (32-bit)
static __inline HI_VOID hi_ext_system_yuv_sharpening_LumaThd_write(ISP_DEV IspDev, HI_U8 index, HI_U32 data){
	IOWR_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2600 + (index << 2), data);
}
static __inline HI_U32 hi_ext_system_yuv_sharpening_LumaThd_read(ISP_DEV IspDev, HI_U8 index) {
    return (IORD_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2600 + (index << 2)));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_yuv_sharpening_LumaWgt
// ------------------------------------------------------------------------------ //
// YUV sharpening
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_YUV_SHARPENING_LUMAWGT_WRITE_DEFAULT (0)

// args: data (32-bit)
static __inline HI_VOID hi_ext_system_yuv_sharpening_LumaWgt_write(ISP_DEV IspDev, HI_U8 index, HI_U32 data){
	IOWR_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2640 + (index << 2), data);
}
static __inline HI_U32 hi_ext_system_yuv_sharpening_LumaWgt_read(ISP_DEV IspDev, HI_U8 index) {
    return (IORD_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2640 + (index << 2)));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_agc_table_demosaic_lum_thresh
// ------------------------------------------------------------------------------ //

//args: data(12-bits)
static __inline HI_VOID hi_ext_system_agc_table_demosaic_lum_thresh_write(ISP_DEV IspDev, HI_U8 index, HI_U16 data){
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2680 + (index << 1), data & 0xFFF);
}

static __inline HI_U16 hi_ext_system_agc_table_demosaic_lum_thresh_read(ISP_DEV IspDev, HI_U8 index){
    return (IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2680 + (index << 1)) & 0xFFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_agc_table_sharping_sad_amplifier
// ------------------------------------------------------------------------------ //
//args: data(8-bits)

static __inline HI_VOID hi_ext_system_agc_table_sharping_sad_amplifier_write(ISP_DEV IspDev, HI_U8 index, HI_U8 data){
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x26b0 + index, data);
}

static __inline HI_U8 hi_ext_system_agc_table_sharping_sad_amplifier_read(ISP_DEV IspDev, HI_U8 index){
  return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x26b0 + index);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_yuv_sharpening_DetailCtrl
// ------------------------------------------------------------------------------ //
// YUV sharpening
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_YUV_SHARPENING_DETAILCTRL_WRITE_DEFAULT (0)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_yuv_sharpening_DetailCtrl_write(ISP_DEV IspDev, HI_U8 index, HI_U8 data){
	IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x26c0 + index, data);
}
static __inline HI_U8 hi_ext_system_yuv_sharpening_DetailCtrl_read(ISP_DEV IspDev, HI_U8 index) {
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x26c0 + index));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_demosaic_lum_thresh_target
// ------------------------------------------------------------------------------ //
// Target strength of demosaic sharpening effect
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DEMOSAIC_LUM_THRESH_TARGET_DEFAULT (0x40)
#define HI_EXT_SYSTEM_DEMOSAIC_LUM_THRESH_TARGET_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_ext_system_demosaic_lum_thresh_target_write(ISP_DEV IspDev, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x26d0, data & 0xFFF);
}
static __inline HI_U16 hi_ext_system_demosaic_lum_thresh_target_read(ISP_DEV IspDev) {
    return (IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x26d0) & 0xFFF) ;
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_bayer_statistics_blacklevel
// ------------------------------------------------------------------------------ //
// the corresponding  logic value is sensor offset + bayer_statistics_blacklevel
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_BAYER_STATISTICS_BLACKLEVEL_DEFAULT (0x0)
#define HI_EXT_SYSTEM_BAYER_STATISTICS_BLACKLEVEL_DATASIZE (12)

// args: data (16-bit)
static __inline HI_VOID hi_ext_system_bayer_statistics_blacklevel_write(ISP_DEV IspDev, HI_U16 data)
{
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x26e0, (data & 0xfff));
}
static __inline HI_U16 hi_ext_system_bayer_statistics_blacklevel_read(ISP_DEV IspDev)
{
    return (IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x26e0) & 0xfff);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_bayer_statistics_whitelevel
// ------------------------------------------------------------------------------ //
// the corresponding  logic value isbayer_statistics_whitelevel
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_BAYER_STATISTICS_WHITELEVEL_DEFAULT (0xfff)
#define HI_EXT_SYSTEM_BAYER_STATISTICS_WHITELEVEL_DATASIZE (12)

// args: data (16-bit)
static __inline HI_VOID hi_ext_system_bayer_statistics_whitelevel_write(ISP_DEV IspDev, HI_U16 data)
{
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x26e2, (data & 0xfff));
}
static __inline HI_U16 hi_ext_system_bayer_statistics_whitelevel_read(ISP_DEV IspDev)
{
    return (IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x26e2) & 0xfff);
}

// ------------------------------------------------------------------------------ //
// YUV sharpening  Reg:0x2600 ---- 0x26FF
// ------------------------------------------------------------------------------ //


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_bind_attr
// ------------------------------------------------------------------------------ //
// isp bind attr include AeLib AwbLib SensorId etc
//|----------------------------------------------------------------|
//|           RSV          |     AE_LIB_ID    |   AWB_LIB_ID       |
//|----------------------------------------------------------------|
//|<--------16bits--------><-------8bits------><------8bits------->|
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_BIND_ATTR_DEFAULT  (0x0)
#define HI_EXT_SYSTEM_BIND_ATTR_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_ext_system_bind_attr_read(ISP_DEV IspDev)
{
    return  IORD_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2700);
}
static __inline HI_VOID hi_ext_system_bind_attr_write(ISP_DEV IspDev, HI_U32 data)
{
     IOWR_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2700, data);
}


// ------------------------------------------------------------------------------ //
// ISP Dual Pipe Different Config Attr
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_isp_pipe_diff_offset
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_ISP_PIPE_DIFF_OFFSET_DEFAULT (0x0)
#define HI_EXT_SYSTEM_ISP_PIPE_DIFF_OFFSET_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_ext_system_isp_pipe_diff_offset_write(ISP_DEV IspDev, HI_U8 index, HI_S32 data)
{
    IOWR_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2710 + (index << 2), data);
}
static __inline HI_S32 hi_ext_system_isp_pipe_diff_offset_read(ISP_DEV IspDev, HI_U8 index)
{
    return IORD_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2710 + (index << 2));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_isp_pipe_diff_gain
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_ISP_PIPE_DIFF_GAIN_DEFAULT (0x100)
#define HI_EXT_SYSTEM_ISP_PIPE_DIFF_GAIN_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_ext_system_isp_pipe_diff_gain_write(ISP_DEV IspDev, HI_U8 index, HI_U32 data)
{
    IOWR_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2720 + (index << 2), data);
}
static __inline HI_U32 hi_ext_system_isp_pipe_diff_gain_read(ISP_DEV IspDev, HI_U8 index)
{
    return IORD_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2720 + (index << 2));
}



// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_isp_matrix_coefft
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_ISP_PIPE_DIFF_CCM_DEFAULT (0x100)
#define HI_EXT_SYSTEM_ISP_PIPE_DIFF_CCM_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_system_isp_pipe_diff_ccm_write(ISP_DEV IspDev, HI_U8 index, HI_U16 data) {
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2730 + (index << 1), data);
}
static __inline HI_U16 hi_ext_system_isp_pipe_diff_ccm_read(ISP_DEV IspDev, HI_U8 index) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2730 + (index << 1));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_snap_type
// ------------------------------------------------------------------------------ //
// the snap type
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_SNAP_TYPE_DEFAULT (0x0)
#define HI_EXT_SYSTEM_SNAP_TYPE_DATASIZE (4)

// args: data (16-bit)
static __inline HI_VOID hi_ext_system_snap_type_write(ISP_DEV IspDev, HI_U16 data)
{
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x27f0, (data & 0xf));
}
static __inline HI_U16 hi_ext_system_snap_type_read(ISP_DEV IspDev)
{
    return (IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x27f0) & 0xf);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_snap_pipe_mode
// ------------------------------------------------------------------------------ //
// the snap pipe mode
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_SNAP_PIPE_MODE_DEFAULT (0x0)
#define HI_EXT_SYSTEM_SNAP_PIPE_MODE_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_snap_pipe_mode_write(ISP_DEV IspDev, HI_U8 data)
{
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x27f2, (data & 0xff));
}
static __inline HI_U8 hi_ext_system_snap_pipe_mode_read(ISP_DEV IspDev)
{
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x27f2) & 0xff);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_snap_pipe_mode
// ------------------------------------------------------------------------------ //
// the snap pipe mode
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_SNAP_USE_INFO_CCM_ENABLE_DEFAULT (0x1)
#define HI_EXT_SYSTEM_SNAP_USE_INFO_CCM_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_system_snap_use_info_ccm_enable_write(ISP_DEV IspDev, HI_U8 data)
{
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x27f4, (data & 0x1));
}
static __inline HI_U8 hi_ext_system_snap_use_info_ccm_enable_read(ISP_DEV IspDev)
{
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x27f4) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_pro_exp_step_table
// ------------------------------------------------------------------------------ //
// professional snap, auto mode exposure step table,PRO_MAX_FRAME_NUM = 8
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_PRO_EXP_STEP_TABLE_DEFAULT (0x100)
#define HI_EXT_SYSTEM_PRO_EXP_STEP_TABLE_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_system_pro_exp_step_table_write(ISP_DEV IspDev, HI_U16 index, HI_U16 data) {
    IOWR_16DIRECT((HI_EXT_BASE_ADDR(IspDev)+0x2800+(index << 1)), data);
}
static __inline HI_U16 hi_ext_system_pro_exp_step_table_read(ISP_DEV IspDev, HI_U16 index) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev)+0x2800+(index << 1));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_pro_man_exp_time_table
// ------------------------------------------------------------------------------ //
// professional snap, manul mode exposure time table,PRO_MAX_FRAME_NUM = 8
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_PRO_MAN_EXP_TIME_TABLE_DEFAULT (0x1000)
#define HI_EXT_SYSTEM_PRO_MAN_EXP_TIME_TABLE_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_ext_system_pro_man_exp_time_table_write(ISP_DEV IspDev, HI_U16 index, HI_U32 data) {
    IOWR_32DIRECT((HI_EXT_BASE_ADDR(IspDev)+0x2810+(index << 2)), data);
}
static __inline HI_U32 hi_ext_system_pro_man_exp_time_table_read(ISP_DEV IspDev, HI_U16 index) {
    return IORD_32DIRECT(HI_EXT_BASE_ADDR(IspDev)+0x2810+(index << 2));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_pro_man_sysgain_table
// ------------------------------------------------------------------------------ //
// professional snap, manul mode system gain table,PRO_MAX_FRAME_NUM = 8
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_PRO_MAN_SYSGAIN_TABLE_DEFAULT (0x400)
#define HI_EXT_SYSTEM_PRO_MAN_SYSGAIN_TABLE_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_ext_system_pro_man_sysgain_table_write(ISP_DEV IspDev, HI_U16 index, HI_U32 data) {
    IOWR_32DIRECT((HI_EXT_BASE_ADDR(IspDev)+0x2830+(index << 2)), data);
}
static __inline HI_U32 hi_ext_system_pro_man_sysgain_table_read(ISP_DEV IspDev, HI_U16 index) {
    return IORD_32DIRECT(HI_EXT_BASE_ADDR(IspDev)+0x2830+(index << 2));
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_pro_frame_num
// ------------------------------------------------------------------------------ //
// professional snap, frame number
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_PRO_FRAME_NUM_DEFAULT (0x3)
#define HI_EXT_SYSTEM_PRO_FRAME_NUM_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_pro_frame_num_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT((HI_EXT_BASE_ADDR(IspDev)+0x2850), data);
}
static __inline HI_U8 hi_ext_system_pro_frame_num_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev)+0x2850);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_pro_type
// ------------------------------------------------------------------------------ //
// professional snap type, auto or manul
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_PRO_TYPE_DEFAULT (0x0)
#define HI_EXT_SYSTEM_PRO_TYPE_DATASIZE (1)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_pro_type_write(ISP_DEV IspDev, HI_U8 data) {
    IOWR_8DIRECT((HI_EXT_BASE_ADDR(IspDev)+0x2851), data&0x1);
}
static __inline HI_U8 hi_ext_system_pro_type_read(ISP_DEV IspDev) {
    return (IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev)+0x2851) & 0x1);
}

static __inline HI_VOID hi_ext_system_update_info_phyaddr_write(ISP_DEV IspDev, HI_U32 data) {
    IOWR_32DIRECT((HI_EXT_BASE_ADDR(IspDev)+0x2854), data);
}

static __inline HI_U32 hi_ext_system_update_info_phyaddr_read(ISP_DEV IspDev) {
    return IORD_32DIRECT(HI_EXT_BASE_ADDR(IspDev)+0x2854);
}

// args: data (1-bit)

#define HI_EXT_SYSTEM_DEHAZE_DEBUG_ENABLE_DEFAULT (0x0)
#define HI_EXT_SYSTEM_DEHAZE_DEBUG_ENABLE_DATASIZE (1)

static __inline HI_VOID hi_ext_system_dehaze_debug_enable_write(ISP_DEV IspDev, HI_U16 data){
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev)+0x2860, data & 0x1);
}
static __inline HI_U16 hi_ext_system_dehaze_debug_enable_read(ISP_DEV IspDev) {
    return (HI_U16) (IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev)+0x2860)&0x1);
    //return (HI_U16) (IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev)+0x2860));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dehaze_debug_addr
// ------------------------------------------------------------------------------ //
// address of dehaze debug information
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DEHAZE_DEBUG_ADDR_DEFAULT (0x0)
#define HI_EXT_SYSTEM_DEHAZE_DEBUG_ADDR_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_ext_system_dehaze_debug_addr_write(ISP_DEV IspDev, HI_U32 data){
    IOWR_32DIRECT(HI_EXT_BASE_ADDR(IspDev)+0x2864, data);
}

static __inline HI_U32 hi_ext_system_dehaze_debug_addr_read(ISP_DEV IspDev) {
    return IORD_32DIRECT(HI_EXT_BASE_ADDR(IspDev)+0x2864);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dehaze_debug_size
// ------------------------------------------------------------------------------ //
// size of dehaze debug struct
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DEHAZE_DEBUG_SIZE_DEFAULT (0x0)
#define HI_EXT_SYSTEM_DEHAZE_DEBUG_SIZE_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_ext_system_dehaze_debug_size_write(ISP_DEV IspDev, HI_U32 data){
    IOWR_32DIRECT(HI_EXT_BASE_ADDR(IspDev)+0x2868, data);
}

static __inline HI_U32 hi_ext_system_dehaze_debug_size_read(ISP_DEV IspDev) {
    return IORD_32DIRECT(HI_EXT_BASE_ADDR(IspDev)+0x2868);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dehaze_debug_depth
// ------------------------------------------------------------------------------ //
// depth of dehaze debug struct
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DEHAZE_DEBUG_DEPTH_DEFAULT (0x0)
#define HI_EXT_SYSTEM_DEHAZE_DEBUG_DEPTH_DATADEPTH (32)

// args: data (32-bit)
static __inline HI_VOID hi_ext_system_dehaze_debug_depth_write(ISP_DEV IspDev, HI_U32 data){
    IOWR_32DIRECT(HI_EXT_BASE_ADDR(IspDev)+0x286c, data);
}

static __inline HI_U32 hi_ext_system_dehaze_debug_depth_read(ISP_DEV IspDev) {
    return IORD_32DIRECT(HI_EXT_BASE_ADDR(IspDev)+0x286c);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dehaze_debug_quit
// ------------------------------------------------------------------------------ //
// depth of dehaze debug struct
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DEHAZE_DEBUG_QUIT_DEFAULT (0x0)
#define HI_EXT_SYSTEM_DEHAZE_DEBUG_QUIT_DATADEPTH (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_dehaze_debug_quit_write(ISP_DEV IspDev, HI_U8 data){
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev)+0x2870, data);
}

static __inline HI_U8 hi_ext_system_dehaze_debug_quit_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev)+0x2870);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dehaze_debug_complete
// ------------------------------------------------------------------------------ //
// depth of dehaze debug struct
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DEHAZE_DEBUG_COMPLETE_DEFAULT (0x0)
#define HI_EXT_SYSTEM_DEHAZE_DEBUG_COMPLETE_DATADEPTH (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_dehaze_debug_complete_write(ISP_DEV IspDev, HI_U8 data){
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev)+0x2872, data);
}

static __inline HI_U8 hi_ext_system_dehaze_debug_complete_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev)+0x2872);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_drc_tm_lut_write len = 65
// ------------------------------------------------------------------------------ //
//0x2900 ~ 0x2A04
// args: data (32-bit)
static __inline HI_VOID hi_ext_system_drc_tm_lut_write(ISP_DEV IspDev, HI_U8 u8Index, HI_U32 data){
    IOWR_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2900 + (u8Index * sizeof(HI_U32)), data & 0xFFFFF);
}

static __inline HI_U32 hi_ext_system_drc_tm_lut_read(ISP_DEV IspDev, HI_U8 u8Index){
    return IORD_32DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2900 + (u8Index * sizeof(HI_U32))) & 0xFFFFF;
}

// ------------------------------------------------------------------------------ //
// Register: DNG info
// ------------------------------------------------------------------------------ //
//0x2a10 ~ 0x2a1c
// args: data (32-bit) 

#define HI_EXT_SYSTEM_DNG_STATIC_INFO_VALID_DEFAULT (0x0)
#define HI_EXT_SYSTEM_DNG_STATIC_INFO_VALID_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_dng_static_info_valid_write(ISP_DEV IspDev, HI_U8 data){
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev)+0x2a10, data);
}

static __inline HI_U8 hi_ext_system_dng_static_info_valid_read(ISP_DEV IspDev) {
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev)+0x2a10);
}

#define HI_EXT_SYSTEM_DNG_HIGH_WB_GAIN_R_DEFAULT (0x400)
#define HI_EXT_SYSTEM_DNG_HIGH_WB_GAIN_R_DATASIZE (16)
// args: data (16-bit)
static __inline HI_VOID hi_ext_system_dng_high_wb_gain_r_write(ISP_DEV IspDev, HI_U16 data){
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev)+0x2a12, data);
}

static __inline HI_U16 hi_ext_system_dng_high_wb_gain_r_read(ISP_DEV IspDev) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev)+0x2a12);
}

#define HI_EXT_SYSTEM_DNG_HIGH_WB_GAIN_G_DEFAULT (0x400)
#define HI_EXT_SYSTEM_DNG_HIGH_WB_GAIN_G_DATASIZE (16)
// args: data (16-bit)
static __inline HI_VOID hi_ext_system_dng_high_wb_gain_g_write(ISP_DEV IspDev, HI_U16 data){
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev)+0x2a14, data);
}

static __inline HI_U16 hi_ext_system_dng_high_wb_gain_g_read(ISP_DEV IspDev) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev)+0x2a14);
}

#define HI_EXT_SYSTEM_DNG_HIGH_WB_GAIN_B_DEFAULT (0x400)
#define HI_EXT_SYSTEM_DNG_HIGH_WB_GAIN_B_DATASIZE (16)
// args: data (16-bit)
static __inline HI_VOID hi_ext_system_dng_high_wb_gain_b_write(ISP_DEV IspDev, HI_U16 data){
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev)+0x2a16, data);
}

static __inline HI_U16 hi_ext_system_dng_high_wb_gain_b_read(ISP_DEV IspDev) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev)+0x2a16);
}

#define HI_EXT_SYSTEM_DNG_LOW_WB_GAIN_R_DEFAULT (0x400)
#define HI_EXT_SYSTEM_DNG_LOW_WB_GAIN_R_DATASIZE (16)
// args: data (16-bit)
static __inline HI_VOID hi_ext_system_dng_low_wb_gain_r_write(ISP_DEV IspDev, HI_U16 data){
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev)+0x2a18, data);
}

static __inline HI_U16 hi_ext_system_dng_low_wb_gain_r_read(ISP_DEV IspDev) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev)+0x2a18);
}

#define HI_EXT_SYSTEM_DNG_LOW_WB_GAIN_G_DEFAULT (0x400)
#define HI_EXT_SYSTEM_DNG_LOW_WB_GAIN_G_DATASIZE (16)
// args: data (16-bit)
static __inline HI_VOID hi_ext_system_dng_low_wb_gain_g_write(ISP_DEV IspDev, HI_U16 data){
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev)+0x2a1a, data);
}

static __inline HI_U16 hi_ext_system_dng_low_wb_gain_g_read(ISP_DEV IspDev) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev)+0x2a1a);
}

#define HI_EXT_SYSTEM_DNG_LOW_WB_GAIN_B_DEFAULT (0x400)
#define HI_EXT_SYSTEM_DNG_LOW_WB_GAIN_B_DATASIZE (16)
// args: data (16-bit)
static __inline HI_VOID hi_ext_system_dng_low_wb_gain_b_write(ISP_DEV IspDev, HI_U16 data){
    IOWR_16DIRECT(HI_EXT_BASE_ADDR(IspDev)+0x2a1c, data);
}

static __inline HI_U16 hi_ext_system_dng_low_wb_gain_b_read(ISP_DEV IspDev) {
    return IORD_16DIRECT(HI_EXT_BASE_ADDR(IspDev)+0x2a1c);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_defog_lut_write len = 256
// ------------------------------------------------------------------------------ //
//0x2B00~0x2C00
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_defog_lut_write(ISP_DEV IspDev, HI_U8 u8Index, HI_U8 data){
    IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2B00 + u8Index, data);
}
static __inline HI_U8 hi_ext_system_defog_lut_read(ISP_DEV IspDev, HI_U8 u8Index){
    return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2B00 + u8Index);
}

#define HI_EXT_SYSTEM_USER_DEFOG_LUT_ENABLE_DEFAULT  (0)
static __inline HI_VOID hi_ext_system_user_defog_lut_enable_write(ISP_DEV IspDev, HI_U8 data){
   IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2C04, data);
}
static __inline HI_U8 hi_ext_system_user_defog_lut_enable_read(ISP_DEV IspDev){
   return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2C04);
}

#define HI_EXT_SYSTEM_USER_DEFOG_LUT_UPDATE_DEFAULT  (0)
static __inline HI_VOID hi_ext_system_user_defog_lut_update_write(ISP_DEV IspDev, HI_U8 data){
   IOWR_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2C08, data);
}
static __inline HI_U8 hi_ext_system_user_defog_lut_update_read(ISP_DEV IspDev){
   return IORD_8DIRECT(HI_EXT_BASE_ADDR(IspDev) + 0x2C08);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __ISP_EXT_CONFIG_H__ */

