/******************************************************************************

  Copyright (C), 2016-2026, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_ext_config.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2016/01/21
  Description   : 
  History       :
  1.Date        : 2016/01/21
    Author      : q00214668
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
// Group: Top    0x10000 -- 0x11000
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_ext_top_wdr_cfg
// ------------------------------------------------------------------------------ //
// HI_TRUE: The state of WDR config is initialized; 
// HI_FALSE: The state of WDR config is NOT initialized
// ------------------------------------------------------------------------------ //

#define HI_EXT_TOP_WDR_CFG_DEFAULT (0x0)
#define HI_EXT_TOP_WDR_CFG_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_top_wdr_cfg_write(HI_U8 data) {
    IOWR_8DIRECT(0x10000, (data & 0x01));
}
static __inline HI_U8 hi_ext_top_wdr_cfg_read(HI_VOID) {
    return (IORD_8DIRECT(0x10000) & 0x01);
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
static __inline HI_VOID hi_ext_top_pub_attr_cfg_write(HI_U8 data) {
    IOWR_8DIRECT(0x10001, (data & 0x01));
}
static __inline HI_U8 hi_ext_top_pub_attr_cfg_read(HI_VOID) {
    return (IORD_8DIRECT(0x10001) & 0x01);
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
static __inline HI_VOID hi_ext_top_wdr_switch_write(HI_U8 data) {
    IOWR_8DIRECT(0x10002, (data & 0x01));
}
static __inline HI_U8 hi_ext_top_wdr_switch_read(HI_VOID) {
    return (IORD_8DIRECT(0x10002) & 0x01);
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
static __inline HI_VOID hi_ext_top_res_switch_write(HI_U8 data) {
    IOWR_8DIRECT(0x10003, (data & 0x01));
}
static __inline HI_U8 hi_ext_top_res_switch_read(HI_VOID) {
    return (IORD_8DIRECT(0x10003) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_top_freeze_firmware
// ------------------------------------------------------------------------------ //
// Disables firmware and stops updating the ISP
// ------------------------------------------------------------------------------ //

#define HI_EXT_TOP_FREEZE_FIRMWARE_DEFAULT (0)
#define HI_EXT_TOP_FREEZE_FIRMWARE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_top_freeze_firmware_write(HI_U8 data) {
    IOWR_8DIRECT(0x10004, (data & 0x01));
}
static __inline HI_U8 hi_ext_top_freeze_firmware_read(HI_VOID) {
    return (IORD_8DIRECT(0x10004) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sensor_wdr_mode
// ------------------------------------------------------------------------------ //
// WDR mode: 0=linear 1=built-in wdr 2=2to1 full frame wdr  ...
// ------------------------------------------------------------------------------ //

#define HI_EXT_TOP_SENSOR_WDR_MODE_DEFAULT (0x0)
#define HI_EXT_TOP_SENSOR_WDR_MODE_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_ext_top_sensor_wdr_mode_write(HI_U8 data) {
    IOWR_8DIRECT(0x10005, data);
}
static __inline HI_U8 hi_ext_top_sensor_wdr_mode_read(HI_VOID) {
    return IORD_8DIRECT(0x10005);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_gamma_curve_type
// ------------------------------------------------------------------------------ //
// the type of gamma curve
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_GAMMA_CURVE_TYPE_DEFAULT (0)
#define HI_EXT_SYSTEM_GAMMA_CURVE_TYPE_DATASIZE (8)
    
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_gamma_curve_type_write(HI_U8 data)
{
    IOWR_8DIRECT(0x10006, data);
}
static __inline HI_U8 hi_ext_system_gamma_curve_type_read(HI_VOID) 
{
    return IORD_8DIRECT(0x10006);
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
static __inline HI_VOID hi_ext_system_sys_debug_enable_write(HI_U8 data)
{
    IOWR_8DIRECT(0x10007, data);
}

static __inline HI_U8 hi_ext_system_sys_debug_enable_read(HI_VOID) 
{
    return IORD_8DIRECT(0x10007);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sys_debug_addr
// ------------------------------------------------------------------------------ //
// address of system debug information
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_SYS_DEBUG_ADDR_DEFAULT (0x0)
#define HI_EXT_SYSTEM_SYS_DEBUG_ADDR_DATASIZE (32)
    
// args: data (32-bit)
static __inline HI_VOID hi_ext_system_sys_debug_addr_write(HI_U32 data)
{
    IOWR_32DIRECT(0x10008, data);
}

static __inline HI_U32 hi_ext_system_sys_debug_addr_read(HI_VOID) 
{
    return IORD_32DIRECT(0x10008);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sys_debug_size
// ------------------------------------------------------------------------------ //
// size of sys debug struct
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_SYS_DEBUG_SIZE_DEFAULT (0x0)
#define HI_EXT_SYSTEM_SYS_DEBUG_SIZE_DATASIZE (32)
    
// args: data (32-bit)
static __inline HI_VOID hi_ext_system_sys_debug_size_write(HI_U32 data)
{
    IOWR_32DIRECT(0x1000c, data);
}

static __inline HI_U32 hi_ext_system_sys_debug_size_read(HI_VOID) 
{
    return IORD_32DIRECT(0x1000c);
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_debug_depth
// ------------------------------------------------------------------------------ //
// the depth of all debug information
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DEBUG_DEPTH_DEFAULT (0x0)
#define HI_EXT_SYSTEM_DEBUG_DEPTH_DATASIZE (32)
    
// args: data (16-bit)
static __inline HI_VOID hi_ext_system_sys_debug_depth_write(HI_U32 data)
{
    IOWR_32DIRECT(0x10010, data);
}

static __inline HI_U32 hi_ext_system_sys_debug_depth_read(HI_VOID) 
{
    return IORD_32DIRECT(0x10010);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_fps_base
// ------------------------------------------------------------------------------ //
//fps_base
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_FPS_BASE_DEFAULT (30)
#define HI_EXT_SYSTEM_FPS_BASE_DATASIZE (32)
// args: data (32-bit)
static __inline HI_VOID hi_ext_system_fps_base_write(HI_U32 data) {
    IOWR_32DIRECT(0x10014, data);
}
static __inline HI_U32 hi_ext_system_fps_base_read(HI_VOID) {
    return IORD_32DIRECT(0x10014);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sensor_max_resolution_width
// ------------------------------------------------------------------------------ //
// width of sensor resolution 
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_SENSOR_MAX_RESOLUTION_WIDTH_DEFAULT (1280)
#define HI_EXT_SYSTEM_SENSOR_MAX_RESOLUTION_WIDTH_DATASIZE (16)
// args: data (16-bit)
static __inline HI_VOID hi_ext_system_sensor_max_resolution_width_write(HI_U16 data){
    IOWR_16DIRECT(0x10018, data);
}

static __inline HI_U16 hi_ext_system_sensor_max_resolution_width_read(HI_VOID){
    return IORD_16DIRECT(0x10018);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sensor_max_resolution_height
// ------------------------------------------------------------------------------ //
// height of sensor resolution 
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_SENSOR_MAX_RESOLUTION_HEIGHT_DEFAULT (720)
#define HI_EXT_SYSTEM_SENSOR_MAX_RESOLUTION_HEIGHT_DATASIZE (16)
// args: data (16-bit)
static __inline HI_VOID hi_ext_system_sensor_max_resolution_height_write(HI_U16 data){
    IOWR_16DIRECT(0x1001a, data);
}

static __inline HI_U16 hi_ext_system_sensor_max_resolution_height_read(HI_VOID){
    return IORD_16DIRECT(0x1001a);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_statistics_ctrl
// ------------------------------------------------------------------------------ //
// ISP statistics control
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_STATISTICS_DEFAULT  (0xfffffffd)
#define HI_EXT_SYSTEM_STATISTICS_DATASIZE (32)
// args: data (32-bit)
static __inline HI_VOID hi_ext_system_statistics_ctrl_write(HI_U32 data)
{
     IOWR_32DIRECT(0x1001c, data);
}

static __inline HI_U32 hi_ext_system_statistics_ctrl_read(HI_VOID)
{
    return  IORD_32DIRECT(0x1001c);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_total_width
// ------------------------------------------------------------------------------ //
// Total frame width including horizontal blanking
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_TOTAL_WIDTH_DEFAULT (0x672)
#define HI_EXT_SYNC_TOTAL_WIDTH_DATASIZE (16)
// args: data (16-bit)
static __inline HI_VOID hi_ext_sync_total_width_write(HI_U16 data) {
    IOWR_16DIRECT(0x10020, data);
}
static __inline HI_U16 hi_ext_sync_total_width_read(HI_VOID) {
    return IORD_16DIRECT(0x10020);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_total_height
// ------------------------------------------------------------------------------ //
// Total frame height including vertical blanking
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYNC_TOTAL_HEIGHT_DEFAULT (0x2EE)
#define HI_EXT_SYNC_TOTAL_HEIGHT_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_sync_total_height_write(HI_U16 data) {
    IOWR_16DIRECT(0x10022, data);
}
static __inline HI_U16 hi_ext_sync_total_height_read(HI_VOID) {
    return IORD_16DIRECT(0x10022);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_bas_outwidth
// ------------------------------------------------------------------------------ //
// 
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYS_BAS_OUT_WIDTH_DATASIZE (16)
static __inline HI_VOID hi_ext_system_bas_outwidth_write(HI_U16 data) {
    IOWR_16DIRECT(0x10024, data);
}
static __inline HI_U16 hi_ext_system_bas_outwidth_read(HI_VOID) {
    return IORD_16DIRECT(0x10024);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_bas_outheight
// ------------------------------------------------------------------------------ //
// 
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYS_BAS_OUT_HEIGHT_DATASIZE (16)
static __inline HI_VOID hi_ext_system_bas_outheight_write(HI_U16 data) {
    IOWR_16DIRECT(0x10026, data);
}
static __inline HI_U16 hi_ext_system_bas_outheight_read(HI_VOID) {
    return IORD_16DIRECT(0x10026);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_isp_bit_bypass
// ------------------------------------------------------------------------------ //
// 
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYS_ISP_BYPASS_DEFAULT (0)
#define HI_EXT_SYS_ISP_BYPASS_DATASIZE (32)

static __inline HI_VOID hi_ext_system_isp_bit_bypass_write(HI_U32 data)
{
	 IOWR_32DIRECT(0x10028, data);
}

static __inline HI_U8 hi_ext_system_isp_bit_bypass_read(HI_VOID)
{
	return	IORD_32DIRECT(0x10028);
}


// ------------------------------------------------------------------------------ //
// Group: Sync
// ------------------------------------------------------------------------------ //
/*.........*/

// ------------------------------------------------------------------------------ //
// Group: Sensor Access
// ------------------------------------------------------------------------------ //
/*.........*/


// ------------------------------------------------------------------------------ //
// Group: Flash interface
// ------------------------------------------------------------------------------ //
/*.........*/

// ------------------------------------------------------------------------------ //
// Group: System 0x11000~?
// ------------------------------------------------------------------------------ //



//============================================================//
//DRC: 0x11000~0x110FF
//============================================================//

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_drc_enable_
// ------------------------------------------------------------------------------ //
// Flag of DRC MOUDLE Enable
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_drc_manual_strength
// ------------------------------------------------------------------------------ //
// Manual setting for DRC strength (for manual DRC)
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DRC_MANUAL_STRENGTH_DEFAULT (0x80)
#define HI_EXT_SYSTEM_DRC_MANUAL_STRENGTH_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_drc_manual_strength_write(HI_U8 data) {
    IOWR_8DIRECT(0x11000, data);
}
static __inline HI_U8 hi_ext_system_drc_manual_strength_read(HI_VOID) {
    return IORD_8DIRECT(0x11000);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_drc_auto_strength
// ------------------------------------------------------------------------------ //
// Auto setting for DRC strength (for auto DRC)
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DRC_AUTO_STRENGTH_DEFAULT (0x80)
#define HI_EXT_SYSTEM_DRC_AUTO_STRENGTH_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_drc_auto_strength_write(HI_U8 data) {
    IOWR_8DIRECT(0x11001, data);
}
static __inline HI_U8 hi_ext_system_drc_auto_strength_read(HI_VOID) {
    return IORD_8DIRECT(0x11001);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_drc_asymmetry
// ------------------------------------------------------------------------------ //
// 
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DRC_ASYMMERY_DEFAULT (0x80)
#define HI_EXT_SYSTEM_DRC_ASYMMERY_DATASIZE (8)

// args: data (8-bit)

static __inline HI_VOID hi_ext_system_drc_asymmetry_write(HI_U8 data){
    IOWR_8DIRECT((0x11002), data);
}
static __inline HI_U8 hi_ext_system_drc_asymmetry_read(HI_VOID){
    return (IORD_8DIRECT(0x11002));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_drc_secondpole
// ------------------------------------------------------------------------------ //
// 
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DRC_SECONDPOLE_DEFAULT (0x80)
#define HI_EXT_SYSTEM_DRC_SECONDPOLE_DATASIZE (8)
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_drc_secondpole_write(HI_U8 data){
    IOWR_8DIRECT((0x11003), data);
}
static __inline HI_U8 hi_ext_system_drc_secondpole_read(HI_VOID){
    return (IORD_8DIRECT(0x11003));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_drc_manual_mode
// ------------------------------------------------------------------------------ //
// Manual Mode of Drc Moudle
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DRC_MANUAL_MODE_DEFAULT (0x0)
#define HI_EXT_SYSTEM_DRC_MANUAL_MODE_DATASIZE (8)
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_drc_manual_mode_write(HI_U8 data){
	IOWR_8DIRECT((0x11004), data);
}
static __inline HI_U8 hi_ext_system_drc_manual_mode_read(HI_VOID){
	return (IORD_8DIRECT(0x11004));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_drc_strength_target
// ------------------------------------------------------------------------------ //
// Strength target of Drc Moudle
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DRC_STRENGTH_TARGET_MODE_DEFAULT (0x0)
#define HI_EXT_SYSTEM_DRC_STRENGTH_TARGET_DATASIZE (8)
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_drc_strength_target_write(HI_U8 data){
    IOWR_8DIRECT((0x11005), data);
}
static __inline HI_U8 hi_ext_system_drc_strength_target_read(HI_VOID){
    return (IORD_8DIRECT(0x11005));    
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_drc_compress
// ------------------------------------------------------------------------------ //
// Drc Compress
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DRC_STRENGTH_TARGET_DATASIZE (8)
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_drc_compress_write(HI_U8 data){
    IOWR_8DIRECT((0x11006), data);
}
static __inline HI_U8 hi_ext_system_drc_compress_read(HI_VOID){
    return (IORD_8DIRECT(0x11006));    
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_drc_stretch
// ------------------------------------------------------------------------------ //
// Stretch of Drc Moudle
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DRC_STRETCH_DEFAULT (54)
#define HI_EXT_SYSTEM_DRC_STRETCH_DATASIZE (32)
// args: data (32-bit)
static __inline HI_VOID hi_ext_system_drc_stretch_write(HI_U8 data){
    IOWR_8DIRECT((0x11007), data);
}
static __inline HI_U8 hi_ext_system_drc_stretch_read(HI_VOID){
    return (IORD_8DIRECT(0x11007));    
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_drc_bright_gain_lmt
// ------------------------------------------------------------------------------ //
// Bright gain limit
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DRC_BRIGHT_GAIN_LMT_DEFAULT  (127)
#define HI_EXT_SYSTEM_DRC_BRIGHT_GAIN_LMT_DATASIZE (16)
// args: data (32-bit)
static __inline HI_VOID hi_ext_system_drc_bright_gain_lmt_write(HI_U16 data){
    IOWR_16DIRECT((0x1100c), data);
}
static __inline HI_U16 hi_ext_system_drc_bright_gain_lmt_read(HI_VOID){
    return (IORD_16DIRECT(0x1100c));    
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_drc_dark_gain_lmt_c
// ------------------------------------------------------------------------------ //
// Bright gain limit
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DRC_DARK_GAIN_LMT_C_DEFAULT  (127)
#define HI_EXT_SYSTEM_DRC_DARK_GAIN_LMT_C_DATASIZE (16)
// args: data (32-bit)
static __inline HI_VOID hi_ext_system_drc_dark_gain_lmt_c_write(HI_U16 data){
    IOWR_16DIRECT((0x1100e), data);
}
static __inline HI_U16 hi_ext_system_drc_dark_gain_lmt_c_read(HI_VOID){
    return (IORD_16DIRECT(0x1100e));    
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_drc_dark_gain_lmt_c
// ------------------------------------------------------------------------------ //
// Bright gain limit
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DRC_DARK_GAIN_LMT_Y_DEFAULT  (127)
#define HI_EXT_SYSTEM_DRC_DARK_GAIN_LMT_Y_DATASIZE (16)
// args: data (32-bit)
static __inline HI_VOID hi_ext_system_drc_dark_gain_lmt_y_write(HI_U16 data){
    IOWR_16DIRECT((0x11010), data);
}
static __inline HI_U16 hi_ext_system_drc_dark_gain_lmt_y_read(HI_VOID){
    return (IORD_16DIRECT(0x11010));    
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_drc_colorcc_lut_up_en
// ------------------------------------------------------------------------------ //
// colorcc_lut_up_en
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_drc_colorcc_lut_up_en_write(HI_U8 data){
    IOWR_8DIRECT((0x11012), data);
}
static __inline HI_U8 hi_ext_system_drc_colorcc_lut_up_en_read(HI_VOID){
    return (IORD_8DIRECT(0x11012));    
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_drc_mixing_bright_max
// ------------------------------------------------------------------------------ //
// mixing_bright_max
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_drc_mixing_bright_max_write(HI_U8 data){
    IOWR_8DIRECT((0x11013), data);
}
static __inline HI_U8 hi_ext_system_drc_mixing_bright_max_read(HI_VOID){
    return (IORD_8DIRECT(0x11013));    
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_drc_mixing_bright_min
// ------------------------------------------------------------------------------ //
// mixing_bright_min
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_drc_mixing_bright_min_write(HI_U8 data){
    IOWR_8DIRECT((0x11014), data);
}
static __inline HI_U8 hi_ext_system_drc_mixing_bright_min_read(HI_VOID){
    return (IORD_8DIRECT(0x11014));    
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_drc_mixing_dark_max
// ------------------------------------------------------------------------------ //
// mixing_dark_max
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_drc_mixing_dark_max_write(HI_U8 data){
    IOWR_8DIRECT((0x11015), data);
}
static __inline HI_U8 hi_ext_system_drc_mixing_dark_max_read(HI_VOID){
    return (IORD_8DIRECT(0x11015));    
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_drc_mixing_dark_min
// ------------------------------------------------------------------------------ //
// mixing_dark_min
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_drc_mixing_dark_min_write(HI_U8 data){
    IOWR_8DIRECT((0x11016), data);
}
static __inline HI_U8 hi_ext_system_drc_mixing_dark_min_read(HI_VOID){
    return (IORD_8DIRECT(0x11016));    
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_drc_mixing_coring
// ------------------------------------------------------------------------------ //
// mixing_coring
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_drc_mixing_coring_write(HI_U8 data){
    IOWR_8DIRECT((0x11017), data);
}
static __inline HI_U8 hi_ext_system_drc_mixing_coring_read(HI_VOID){
    return (IORD_8DIRECT(0x11017));    
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_drc_pdw_high
// ------------------------------------------------------------------------------ //
// pdw_high
// ------------------------------------------------------------------------------ //
// args: data (8-bit)

static __inline HI_VOID hi_ext_system_drc_pdw_high_write(HI_U8 data){
    IOWR_8DIRECT((0x11018), data);
}
static __inline HI_U8 hi_ext_system_drc_pdw_high_read(HI_VOID){
    return (IORD_8DIRECT(0x11018));    
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_drc_gain_clip_knee
// ------------------------------------------------------------------------------ //
// gain clip knee 
// ------------------------------------------------------------------------------ //
// args: data (8-bit)

static __inline HI_VOID hi_ext_system_drc_gain_clip_knee_write(HI_U8 data){
    IOWR_8DIRECT((0x11019), data);
}
static __inline HI_U8 hi_ext_system_drc_gain_clip_knee_read(HI_VOID){
    return (IORD_8DIRECT(0x11019));    
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_drc_gain_clip_step
// ------------------------------------------------------------------------------ //
// gain clip step 
// ------------------------------------------------------------------------------ //
// args: data (8-bit)

static __inline HI_VOID hi_ext_system_drc_gain_clip_step_write(HI_U8 data){
    IOWR_8DIRECT((0x1101A), data);
}
static __inline HI_U8 hi_ext_system_drc_gain_clip_step_read(HI_VOID){
    return (IORD_8DIRECT(0x1101A));    
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_drc_colorcc_lut_read
// ------------------------------------------------------------------------------ //
// colorcc lut read len = 33  ~0x11061
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DRC_COLORCCLUT_DATASIZE (16)
// args: data (16-bit)

static __inline HI_VOID hi_ext_system_drc_colorcc_lut_write(HI_U8 u8Index, HI_U16 data){
    IOWR_16DIRECT((0x11020 + u8Index*sizeof(HI_U16)), data);
}
static __inline HI_U16 hi_ext_system_drc_colorcc_lut_read(HI_U8 u8Index) {
    return (IORD_16DIRECT((0x11020 + u8Index*sizeof(HI_U16))));
}


static __inline HI_VOID hi_ext_system_drc_enable_write(HI_U8 data){
	IOWR_8DIRECT((0x11070), data&0x1);
}
static __inline HI_U8 hi_ext_system_drc_enable_read(HI_VOID){
    return (IORD_8DIRECT(0x11070)&0x1);    
}

static __inline HI_VOID hi_ext_system_drc_var_rng_fine_write(HI_U8 data){
	IOWR_8DIRECT((0x11072), data);
}
static __inline HI_U8 hi_ext_system_drc_var_rng_fine_read(HI_VOID){
	return (IORD_8DIRECT(0x11072));    
}

static __inline HI_VOID hi_ext_system_drc_var_spa_fine_write(HI_U8 data){
    IOWR_8DIRECT((0x11073), data);
}
static __inline HI_U8 hi_ext_system_drc_var_spa_fine_read(HI_VOID){
    return (IORD_8DIRECT(0x11073));    
}
static __inline HI_VOID hi_ext_system_drc_var_rng_medium_write(HI_U8 data){
    IOWR_8DIRECT((0x11074), data);
}
static __inline HI_U8 hi_ext_system_drc_var_rng_medium_read(HI_VOID){
    return (IORD_8DIRECT(0x11074));    
}
static __inline HI_VOID hi_ext_system_drc_var_spa_medium_write(HI_U8 data){
    IOWR_8DIRECT((0x11075), data);
}
static __inline HI_U8 hi_ext_system_drc_var_spa_medium_read(HI_VOID){
    return (IORD_8DIRECT(0x11075));    
}
static __inline HI_VOID hi_ext_system_drc_var_rng_coarse_write(HI_U8 data){
    IOWR_8DIRECT((0x11076), data);
}
static __inline HI_U8 hi_ext_system_drc_var_rng_coarse_read(HI_VOID){
    return (IORD_8DIRECT(0x11076));    
}
static __inline HI_VOID hi_ext_system_drc_var_spa_coarse_write(HI_U8 data){
    IOWR_8DIRECT((0x11077), data);
}
static __inline HI_U8 hi_ext_system_drc_var_spa_coarse_read(HI_VOID){
    return (IORD_8DIRECT(0x11077));    
}
static __inline HI_VOID hi_ext_system_drc_bin_mix_medium_write(HI_U8 data){
	IOWR_8DIRECT((0x11078), data);
}
static __inline HI_U8 hi_ext_system_drc_bin_mix_medium_read(HI_VOID){
	return (IORD_8DIRECT(0x11078));    
}
static __inline HI_VOID hi_ext_system_drc_bin_mix_coarse_write(HI_U8 data){
    IOWR_8DIRECT((0x11079), data);
}
static __inline HI_U8 hi_ext_system_drc_bin_mix_coarse_read(HI_VOID){
    return (IORD_8DIRECT(0x11079));    
}



//==============================================================//
//2DNR: 0x11100~0x112FF 
//==============================================================//

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_nr_manu_mode
// ------------------------------------------------------------------------------ //
// manual mode
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_NR_MANU_MODE_DEFAULT  (0)
#define HI_EXT_SYSTEM_NR_MANU_MODE_DATASIZE (8)
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_nr_manu_mode_write(HI_U8 data){
    IOWR_8DIRECT(0x11100, data);
}
static __inline HI_U8 hi_ext_system_nr_manu_mode_read(HI_VOID) {
    return (IORD_8DIRECT(0x11100));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_nr_manual_fcrshift_write
// ------------------------------------------------------------------------------ //
// manual mode
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_NR_MANUAL_FCR_SHIFT_DEFAULT  (8)
#define HI_EXT_SYSTEM_NR_MANUAL_FCR_SHIFT_DATASIZE (8)
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_nr_manual_fcrshift_write(HI_U8 data) {
    IOWR_8DIRECT(0x11101, data);
}

static __inline HI_U8 hi_ext_system_nr_manual_fcrshift_read(HI_VOID) {
    return (IORD_8DIRECT(0x11101));
} 

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_nr_manual_fcrgain_write
// ------------------------------------------------------------------------------ //
// manual mode
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_NR_MANUAL_FCR_GAIN_DEFAULT  (32)
#define HI_EXT_SYSTEM_NR_MANUAL_FCR_GAIN_DATASIZE (8)
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_nr_manual_fcrgain_write(HI_U8 data) {
    IOWR_8DIRECT(0x11102, data);
}

static __inline HI_U8 hi_ext_system_nr_manual_fcrgain_read(HI_VOID) {
    return (IORD_8DIRECT(0x11102));
} 

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_nr_manual_fcrlimit_write
// ------------------------------------------------------------------------------ //
// manual mode
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_NR_MANUAL_FCR_LIMIT_DEFAULT  (32)
#define HI_EXT_SYSTEM_NR_MANUAL_FCR_LIMIT_DATASIZE (8)
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_nr_manual_fcrlimit_write(HI_U8 data) {
    IOWR_8DIRECT(0x11103, data);
}

static __inline HI_U8 hi_ext_system_nr_manual_fcrlimit_read(HI_VOID) {
    return (IORD_8DIRECT(0x11103));
} 

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_nr_manual_blendlowlmt_write
// ------------------------------------------------------------------------------ //
// manual mode
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_NR_MANUAL_BLENDLOWLMT_DEFAULT  (72)
#define HI_EXT_SYSTEM_NR_MANUAL_BLENDLOWLMT_DATASIZE (8)
// args: data (16-bit)
static __inline HI_VOID hi_ext_system_nr_manual_blendlowlmt_write(HI_U16 data) {
    IOWR_16DIRECT(0x11104, data);
}

static __inline HI_U16 hi_ext_system_nr_manual_blendlowlmt_read(HI_VOID) {
    return (IORD_16DIRECT(0x11104));
} 

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_nr_manual_blendhighlmt_write
// ------------------------------------------------------------------------------ //
// manual mode
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_NR_MANUAL_BLENDHIGHLMT_DEFAULT  (256)
#define HI_EXT_SYSTEM_NR_MANUAL_BLENDHIGHLMT_DATASIZE (8)
// args: data (16-bit)
static __inline HI_VOID hi_ext_system_nr_manual_blendhighlmt_write(HI_U16 data) {
    IOWR_16DIRECT(0x11106, data);
}

static __inline HI_U16 hi_ext_system_nr_manual_blendhighlmt_read(HI_VOID) {
    return (IORD_16DIRECT(0x11106));
} 


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_nr_manual_fcr_en_write
// ------------------------------------------------------------------------------ //
// manual mode
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_NR_FCR_EN_DEFAULT  (1)
#define HI_EXT_SYSTEM_NR_FCR_EN_DATASIZE (8)
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_nr_fcr_en_write(HI_U8 data){
    IOWR_8DIRECT(0x11108, data);
}
static __inline HI_U8 hi_ext_system_nr_fcr_en_read(HI_VOID) {
    return IORD_8DIRECT(0x11108);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_nr_nodir_low_thd
// ------------------------------------------------------------------------------ //
// manual mode
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_NR_NODIR_LOW_THD_DEFAULT  (0x0)
#define HI_EXT_SYSTEM_NR_NODIR_LOW_THD_DATASIZE (8)
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_nr_nodir_low_thd_write(HI_U8 data){
    IOWR_8DIRECT(0x11109, data);
}
static __inline HI_U8 hi_ext_system_nr_nodir_low_thd_read(HI_VOID) {
    return IORD_8DIRECT(0x11109);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_nr_nodir_hig_thd_write
// ------------------------------------------------------------------------------ //
// manual mode
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_NR_NODIR_HIG_THD_DEFAULT  (0xe)
#define HI_EXT_SYSTEM_NR_NODIR_HIG_THD_DATASIZE (8)
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_nr_nodir_hig_thd_write(HI_U8 data){
    IOWR_8DIRECT(0x1110a, data);
}
static __inline HI_U8 hi_ext_system_nr_nodir_hig_thd_read(HI_VOID) {
    return IORD_8DIRECT(0x1110a);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_nr_nodir_gain_manual_write
// ------------------------------------------------------------------------------ //
// manual mode
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_NR_NODIR_GAIN_MANUAL_DEFAULT  (0xe)
#define HI_EXT_SYSTEM_NR_NODIR_GAIN_MANUAL_DATASIZE (8)
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_nr_nodir_gain_manual_write(HI_U8 data){
    IOWR_8DIRECT(0x1110b, data);
}
static __inline HI_U8 hi_ext_system_nr_nodir_gain_manual_read(HI_VOID) {
    return IORD_8DIRECT(0x1110b);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_nr_manual_rangecoeflow_write
// ------------------------------------------------------------------------------ //
// manual mode
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_NR_RANGELOW_MANUAL_DEFAULT  (1400)
#define HI_EXT_SYSTEM_NR_RANGELOW_MANUAL_DATASIZE (8)
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_nr_manual_rangecoeflow_write(HI_U32 data){
    IOWR_32DIRECT(0x1110c, data);
}
static __inline HI_U32 hi_ext_system_nr_manual_rangecoeflow_read(HI_VOID) {
    return IORD_32DIRECT(0x1110c);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_nr_manual_rangecoeflow_write
// ------------------------------------------------------------------------------ //
// manual mode
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_NR_RANGEHIGH_MANUAL_DEFAULT  (1400)
#define HI_EXT_SYSTEM_NR_RANGEHIGH_MANUAL_DATASIZE (8)
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_nr_manual_rangecoefhigh_write(HI_U32 data){
    IOWR_32DIRECT(0x11110, data);
}
static __inline HI_U32 hi_ext_system_nr_manual_rangecoefhigh_read(HI_VOID) {
    return IORD_32DIRECT(0x11110);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_nr_auto_fcr_gain_write
// ------------------------------------------------------------------------------ //
// auto fcr gain
// ------------------------------------------------------------------------------ //
// args: data (8-bit)

static __inline HI_VOID hi_ext_system_nr_auto_fcrgain_write(HI_U8 u8Index, HI_U8 data){
    IOWR_8DIRECT((0x11130 + u8Index*sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_nr_auto_fcrgain_read(HI_U8 u8Index) {
    return (IORD_8DIRECT((0x11130 + u8Index*sizeof(HI_U8))));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_nr_auto_fcrlimit_write
// ------------------------------------------------------------------------------ //
// auto fcr limit
// ------------------------------------------------------------------------------ //
// args: data (8-bit)

static __inline HI_VOID hi_ext_system_nr_auto_fcrlimit_write(HI_U8 u8Index, HI_U8 data){
    IOWR_8DIRECT((0x11140 + u8Index*sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_nr_auto_fcrlimit_read(HI_U8 u8Index) {
    return (IORD_8DIRECT((0x11140 + u8Index*sizeof(HI_U8))));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_nr_auto_fcrshift_write
// ------------------------------------------------------------------------------ //
// auto fcr limit
// ------------------------------------------------------------------------------ //
// args: data (8-bit)

static __inline HI_VOID hi_ext_system_nr_auto_fcrshift_write(HI_U8 u8Index, HI_U8 data){
    IOWR_8DIRECT((0x11150 + u8Index*sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_nr_auto_fcrshift_read(HI_U8 u8Index) {
    return (IORD_8DIRECT((0x11150 + u8Index*sizeof(HI_U8))));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_nr_auto_blendlowlmt_write
// ------------------------------------------------------------------------------ //
// auto fcr limit
// ------------------------------------------------------------------------------ //
// args: data (8-bit)

static __inline HI_VOID hi_ext_system_nr_auto_blendlowlmt_write(HI_U8 u8Index, HI_U16 data){
    IOWR_16DIRECT((0x11160 + u8Index*sizeof(HI_U16)), data);
}
static __inline HI_U16 hi_ext_system_nr_auto_blendlowlmt_read(HI_U8 u8Index) {
    return (IORD_16DIRECT((0x11160 + u8Index*sizeof(HI_U16))));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_nr_auto_blendhighlmt_write
// ------------------------------------------------------------------------------ //
// auto fcr limit
// ------------------------------------------------------------------------------ //
// args: data (8-bit)

static __inline HI_VOID hi_ext_system_nr_auto_blendhighlmt_write(HI_U8 u8Index, HI_U16 data){
    IOWR_16DIRECT((0x11180 + u8Index*sizeof(HI_U16)), data);
}
static __inline HI_U16 hi_ext_system_nr_auto_blendhighlmt_read(HI_U8 u8Index) {
    return (IORD_16DIRECT((0x11180 + u8Index*sizeof(HI_U16))));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_nr_auto_rangecoefhigh_write
// ------------------------------------------------------------------------------ //
// auto fcr limit
// ------------------------------------------------------------------------------ //
// args: data (32-bit)

static __inline HI_VOID hi_ext_system_nr_auto_rangecoefhigh_write(HI_U8 u8Index, HI_U32 data){
    IOWR_32DIRECT((0x111a0 + u8Index*sizeof(HI_U32)), data);
}
static __inline HI_U32 hi_ext_system_nr_auto_rangecoefhigh_read(HI_U8 u8Index) {
    return (IORD_32DIRECT((0x111a0 + u8Index*sizeof(HI_U32))));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_nr_auto_rangecoeflow_write
// ------------------------------------------------------------------------------ //
// auto rangecoeflow
// ------------------------------------------------------------------------------ //
// args: data (32-bit)

static __inline HI_VOID hi_ext_system_nr_auto_rangecoeflow_write(HI_U8 u8Index, HI_U32 data){
    IOWR_32DIRECT((0x111e0 + u8Index*sizeof(HI_U32)), data);
}
static __inline HI_U32 hi_ext_system_nr_auto_rangecoeflow_read(HI_U8 u8Index) {
    return (IORD_32DIRECT((0x111e0 + u8Index*sizeof(HI_U32))));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_nr_auto_lumaisolut_write
// ------------------------------------------------------------------------------ //
// auto rangecoeflow
// ------------------------------------------------------------------------------ //
// args: data (32-bit)

static __inline HI_VOID hi_ext_system_nr_auto_lumaisolut_write(HI_U8 u8Index, HI_U32 data){
    IOWR_32DIRECT((0x11220 + u8Index*sizeof(HI_U32)), data);
}
static __inline HI_U32 hi_ext_system_nr_auto_lumaisolut_read(HI_U8 u8Index) {
    return (IORD_32DIRECT((0x11220 + u8Index*sizeof(HI_U32))));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_nr_auto_lumagainlut_write
// ------------------------------------------------------------------------------ //
// auto rangecoeflow 0x11238 --0x1123d 
// ------------------------------------------------------------------------------ //
// args: data (32-bit)

static __inline HI_VOID hi_ext_system_nr_auto_lumagainlut_write(HI_U8 u8Index, HI_U8 data){
    IOWR_8DIRECT((0x11238 + u8Index*sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_nr_auto_lumagainlut_read(HI_U8 u8Index) {
    return (IORD_8DIRECT((0x11238 + u8Index*sizeof(HI_U8))));
}





//==============================================================//
//Dehaze: 0x11300~0x114FF
//==============================================================//

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dehaze_manu_mode
// ------------------------------------------------------------------------------ //
// Enables manual dehaze: 0=auto 1=manual
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DEHAZE_MANU_MODE_DEFAULT  (0)
#define HI_EXT_SYSTEM_DEHAZE_MANU_MODE_DATASIZE (1)
// args: data (8-bit)
static __inline void hi_ext_system_dehaze_manu_mode_write(HI_U8 data) {
    IOWR_8DIRECT(0x11300, data);
}
static __inline HI_U32 hi_ext_system_dehaze_manu_mode_read(HI_VOID) {
    return IORD_8DIRECT(0x11300);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_dehaze_hblk
// ------------------------------------------------------------------------------ //
// manual dehaze hblk number
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_DEHAZE_HBLK_DEFAULT (32)
#define HI_EXT_SYSTEM_MANUAL_DEHAZE_HBLK_DATASIZE (8)

// args: data (8-bit)
static __inline void hi_ext_system_manual_dehaze_hblk_write(HI_U8 data) {
    IOWR_8DIRECT(0x11301, data);
}
static __inline HI_U8 hi_ext_system_manual_dehaze_hblk_read(HI_VOID) {
    return IORD_8DIRECT(0x11301);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_dehaze_vblk
// ------------------------------------------------------------------------------ //
// manual dehaze vblk number
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_DEHAZE_VBLK_DEFAULT (32)
#define HI_EXT_SYSTEM_MANUAL_DEHAZE_VBLK_DATASIZE (8)
// args: data (8-bit)
static __inline void hi_ext_system_manual_dehaze_vblk_write(HI_U8 data) {
    IOWR_8DIRECT(0x11302, data);
}

static __inline HI_U8 hi_ext_system_manual_dehaze_vblk_read(HI_VOID) {
    return IORD_8DIRECT(0x11302);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dehaze_ctp
// ------------------------------------------------------------------------------ //
// manual dehaze curve turn point
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DEHAZE_CTP_DEFAULT (0x80)
#define HI_EXT_SYSTEM_DEHAZE_CTP_DATASIZE (8)

// args: data (8-bit)
static __inline void hi_ext_system_dehaze_ctp_write(HI_U8 data) {
    IOWR_8DIRECT(0x11303, data);
}
static __inline HI_U8 hi_ext_system_dehaze_ctp_read(HI_VOID) {
    return IORD_8DIRECT(0x11303);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_dehaze_mft
// ------------------------------------------------------------------------------ //
// manual dehaze minumum filter threshold
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DEHAZE_MFT_DEFAULT (0x200)
#define HI_EXT_SYSTEM_MANUAL_DEHAZE_MFT_DATASIZE (16)

// args: data (10-bit)
static __inline void hi_ext_system_dehaze_mft_write(HI_U16 data) {
    IOWR_16DIRECT(0x11304,data);
}
static __inline HI_U16 hi_ext_system_dehaze_mft_read(HI_VOID) {
    return (IORD_16DIRECT(0x11304) & 0x3ff);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_dehaze_mfs
// ------------------------------------------------------------------------------ //
// manual dehaze minumum filter size
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DEHAZE_MFS_DEFAULT (0x7)
#define HI_EXT_SYSTEM_DEHAZE_MFS_DATASIZE (8)
// args: data (8-bit)
static __inline void hi_ext_system_dehaze_mfs_write(HI_U8 data) {
    IOWR_8DIRECT(0x11306, data);
}
static __inline HI_U8 hi_ext_system_dehaze_mfs_read(HI_VOID) {
    return IORD_8DIRECT(0x11306);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_dehaze_ct
// ------------------------------------------------------------------------------ //
// manual dehaze change threshold
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DEHAZE_CT_DEFAULT (255)
#define HI_EXT_SYSTEM_DEHAZE_CT_DATASIZE (8)
// args: data (8-bit)
static __inline void hi_ext_system_dehaze_ct_write(HI_U8 data) {
    IOWR_8DIRECT(0x11307, data);
}
static __inline HI_U8 hi_ext_system_dehaze_ct_read(HI_VOID) {
    return IORD_8DIRECT(0x11307);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_dehaze_tfic
// ------------------------------------------------------------------------------ //
// manual dehaze temper filter increase ceofficient
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DEHAZE_TFIC_DEFAULT (0x8)
#define HI_EXT_SYSTEM_DEHAZE_TFIC_DATASIZE (16)
// args: data (16-bit)
static __inline void hi_ext_system_dehaze_tfic_write(HI_U16 data) {
    IOWR_16DIRECT(0x11308, data);
}
static __inline HI_U16 hi_ext_system_dehaze_tfic_read(HI_VOID) {
    return IORD_16DIRECT(0x11308);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_dehaze_tfdc
// ------------------------------------------------------------------------------ //
// manual dehaze temper filter decrease ceofficient
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DEHAZE_TFDC_DEFAULT (64)
#define HI_EXT_SYSTEM_DEHAZE_TFDC_DATASIZE (16)
// args: data (16-bit)
static __inline void hi_ext_system_dehaze_tfdc_write(HI_U16 data) {
    IOWR_16DIRECT(0x1130A, data);
}
static __inline HI_U16 hi_ext_system_dehaze_tfdc_read(HI_VOID) {
    return IORD_16DIRECT(0x1130A);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dehaze_debug_enable
// ------------------------------------------------------------------------------ //
// dehaze debug enable
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DEHAZE_DEBUG_ENABLE_DEFAULT (0)
#define HI_EXT_SYSTEM_DEHAZE_DEBUG_ENABLE_DATASIZE (8)

// args: data (8-bit)
static __inline void hi_ext_system_dehaze_debug_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x1130C, data);
}
static __inline HI_U8 hi_ext_system_dehaze_debug_enable_read(HI_VOID) {
    return IORD_8DIRECT(0x1130C);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_dehaze_strength
// ------------------------------------------------------------------------------ //
// dehaze strength
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_MANUAL_DEHAZE_STRENGTH_DEFAULT  (0xF0)
#define ISP_EXT_SYSTEM_MANUAL_DEHAZE_STRENGTH_DATASIZE (8)

// args: data (8-bit)
static __inline void hi_ext_system_manual_dehaze_strength_write(HI_U8 data) {
    IOWR_8DIRECT(0x1130D, data);
}
static __inline HI_U8 hi_ext_system_manual_dehaze_strength_read(HI_VOID) {
    return IORD_8DIRECT(0x1130D);
}

// ------------------------------------------------------------------------------ //
// Register: isp_ext_system_manual_dehaze_autostrength
// ------------------------------------------------------------------------------ //
// dehaze strength
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_DEHAZE_AUTOSTRENGTH_DEFAULT  (0x80)
#define HI_EXT_SYSTEM_MANUAL_DEHAZE_AUTOSTRENGTH_DATASIZE (8)
// args: data (8-bit)
static __inline void hi_ext_system_manual_dehaze_autostrength_write(HI_U8 data) {
    IOWR_8DIRECT(0x1130e, data);
}
static __inline HI_U8 hi_ext_system_manual_dehaze_autostrength_read(HI_VOID) {
    return IORD_8DIRECT(0x1130e);
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dehaze_debug_addr
// ------------------------------------------------------------------------------ //
// address of dehaze debug information
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DEHAZE_DEBUG_ADDR_DEFAULT (0x0)
#define HI_EXT_SYSTEM_DEHAZE_DEBUG_ADDR_DATASIZE (32)
    
// args: data (32-bit)
static __inline HI_VOID hi_ext_system_dehaze_debug_addr_write(HI_U32 data){
    IOWR_32DIRECT(0x11404, data);
}

static __inline HI_U32 hi_ext_system_dehaze_debug_addr_read(HI_VOID) {
    return IORD_32DIRECT(0x11404);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dehaze_debug_size
// ------------------------------------------------------------------------------ //
// size of dehaze debug struct
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DEHAZE_DEBUG_SIZE_DEFAULT (0x0)
#define HI_EXT_SYSTEM_DEHAZE_DEBUG_SIZE_DATASIZE (32)
    
// args: data (32-bit)
static __inline HI_VOID hi_ext_system_dehaze_debug_size_write(HI_U32 data){
    IOWR_32DIRECT(0x11408, data);
}

static __inline HI_U32 hi_ext_system_dehaze_debug_size_read(HI_VOID) {
    return IORD_32DIRECT(0x11408);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dehaze_debug_depth
// ------------------------------------------------------------------------------ //
// depth of dehaze debug struct
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DEHAZE_DEBUG_DEPTH_DEFAULT (0x0)
#define HI_EXT_SYSTEM_DEHAZE_DEBUG_DEPTH_DATADEPTH (32)
    
// args: data (32-bit)
static __inline HI_VOID hi_ext_system_dehaze_debug_depth_write(HI_U32 data){
    IOWR_32DIRECT(0x1140c, data);
}

static __inline HI_U32 hi_ext_system_dehaze_debug_depth_read(HI_VOID) {
    return IORD_32DIRECT(0x1140c);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dehaze_debug_quit
// ------------------------------------------------------------------------------ //
// depth of dehaze debug struct
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DEHAZE_DEBUG_QUIT_DEFAULT (0x0)
#define HI_EXT_SYSTEM_DEHAZE_DEBUG_QUIT_DATADEPTH (8)
    
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_dehaze_debug_quit_write(HI_U8 data){
    IOWR_8DIRECT(0x11410, data);
}

static __inline HI_U8 hi_ext_system_dehaze_debug_quit_read(HI_VOID) {
    return IORD_8DIRECT(0x11410);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dehaze_debug_complete
// ------------------------------------------------------------------------------ //
// depth of dehaze debug struct
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DEHAZE_DEBUG_COMPLETE_DEFAULT (0x0)
#define HI_EXT_SYSTEM_DEHAZE_DEBUG_COMPLETE_DATADEPTH (8)
    
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_dehaze_debug_complete_write(HI_U8 data){
    IOWR_8DIRECT(0x11412, data);
}

static __inline HI_U8 hi_ext_system_dehaze_debug_complete_read(HI_VOID) {
    return IORD_8DIRECT(0x11412);
}

static __inline void hi_ext_system_dehaze_debugfile_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x11414, data);
}
static __inline HI_U8 hi_ext_system_dehaze_debugfile_enable_read(HI_VOID) {
    return IORD_8DIRECT(0x11414);
}



//==============================================================//
//Dehaze: 0x11500~0x115FF
//==============================================================//

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_fpn_strength
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_FPN_STRENGTH_DEFAULT  (0)
#define HI_EXT_SYSTEM_FPN_STRENGTH_DATASIZE (16)
// args: data (16-bit)

static __inline void hi_ext_system_manual_fpn_strength_write(HI_U16 data)
{
    IOWR_16DIRECT(0x11500, data);
}
static __inline HI_U16 hi_ext_system_manual_fpn_strength_read(HI_VOID)
{
    return IORD_16DIRECT(0x11500);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_fpn_opmode
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_FPN_OPMODE_DEFAULT  (0x0)
#define HI_EXT_SYSTEM_FPN_OPMODE_DATASIZE (8)
// args: data (8-bit)

static __inline void hi_ext_system_manual_fpn_opmode_write(HI_U8 data)
{
     IOWR_8DIRECT(0x11502, data);
}

static __inline HI_U8 hi_ext_system_manual_fpn_opmode_read(HI_VOID)
{
    return IORD_8DIRECT(0x11502);
}


// ------------------------------------------------------------------------------ //
// Register: isp_ext_system_manual_fpn_update
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_FPN_MANU_UPDATE_DEFAULT  (0x0)
#define HI_EXT_SYSTEM_FPN_MANU_UPDATE_DATASIZE (8)
// args: data (8-bit)

static __inline void hi_ext_system_manual_fpn_update_write(HI_U8 data)
{
     IOWR_8DIRECT(0x11503, data);
}

static __inline HI_U8 hi_ext_system_manual_fpn_update_read(HI_VOID)
{
    return IORD_8DIRECT(0x11503);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_fpn_CorrCfg
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_FPN_MANU_ISO_DEFAULT  (6375)
#define HI_EXT_SYSTEM_FPN_MANU_ISO_DATASIZE (32)
// args: data (32-bit)


static __inline void hi_ext_system_manual_fpn_iso_write(HI_U32 data) 
{
    IOWR_32DIRECT(0x11504, data);
}

static __inline HI_U32 hi_ext_system_manual_fpn_iso_read(HI_VOID) 
{
    return IORD_32DIRECT(0x11504);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sensor_iso
// ------------------------------------------------------------------------------ //
// Sensor ISO 
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_FPN_SENSOR_ISO_DEFAULT  (0x0)
#define HI_EXT_SYSTEM_FPN_SENSOR_ISO_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_ext_system_fpn_sensor_iso_write(HI_U32 data)
{
     IOWR_32DIRECT(0x11508, data);
}

static __inline HI_U32 hi_ext_system_fpn_sensor_iso_read(HI_VOID)
{
    return  IORD_32DIRECT(0x11508);
}


// ------------------------------------------------------------------------------ //
// Register: isp_ext_system_manual_fpn_CorrCfg
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_FPN_MANU_CORRCFG_DEFAULT  (0x0)
#define HI_EXT_SYSTEM_FPN_MANU_CORRCFG_DATASIZE (8)
// args: data (8-bit)

static __inline void hi_ext_system_manual_fpn_CorrCfg_write(HI_U8 data)
{
    IOWR_8DIRECT(0x1150c, data);
}

static __inline HI_U8 isp_ext_system_manual_fpn_CorrCfg_read(HI_VOID)
{
    return IORD_8DIRECT(0x1150c);
}

// ------------------------------------------------------------------------------ //
// Register: isp_ext_system_manual_fpn_CorrCfg
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_FPN_ACTUAL_STRENGTH_DEFAULT  (0x0)
#define HI_EXT_SYSTEM_FPN_ACTUAL_STRENGTH_DATASIZE (16)
// args: data (8-bit)

static __inline void hi_ext_system_fpn_strength_write(HI_U32 data)
{
    IOWR_32DIRECT(0x11510, data);
}

static __inline HI_U32 hi_isp_ext_system_fpn_strength_read(HI_VOID)
{
    return IORD_32DIRECT(0x11510);
}



//=============================================================//
//BLACK LEVEL: 0x11600~0x116FF
//=============================================================//

// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_black_level_00
// ------------------------------------------------------------------------------ //
// ISP black level 00
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_BLACK_LEVEL_00_DEFAULT  (0x0)
#define HI_EXT_SYSTEM_BLACK_LEVEL_00_DATASIZE (16)
// args: data (16-bit)

static __inline HI_VOID hi_ext_system_black_level_00_write(HI_U16 data)
{
     IOWR_16DIRECT(0x11600, data);
}

static __inline HI_U16 hi_ext_system_black_level_00_read(HI_VOID)
{
    return  IORD_16DIRECT(0x11600);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_black_level_01
// ------------------------------------------------------------------------------ //
// ISP black level 01
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_BLACK_LEVEL_01_DEFAULT  (0x0)
#define HI_EXT_SYSTEM_BLACK_LEVEL_01_DATASIZE (16)
// args: data (16-bit)

static __inline HI_VOID hi_ext_system_black_level_01_write(HI_U16 data)
{
     IOWR_16DIRECT(0x11602, data);
}

static __inline HI_U16 hi_ext_system_black_level_01_read(HI_VOID)
{
    return  IORD_16DIRECT(0x11602);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_black_level_10
// ------------------------------------------------------------------------------ //
// ISP black level 10
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_BLACK_LEVEL_10_DEFAULT  (0x0)
#define HI_EXT_SYSTEM_BLACK_LEVEL_10_DATASIZE (16)
// args: data (16-bit)

static __inline HI_VOID hi_ext_system_black_level_10_write(HI_U16 data)
{
     IOWR_16DIRECT(0x11604, data);
}

static __inline HI_U16 hi_ext_system_black_level_10_read(HI_VOID)
{
    return  IORD_16DIRECT(0x11604);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_black_level_11
// ------------------------------------------------------------------------------ //
// ISP black level 11
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_BLACK_LEVEL_11_DEFAULT  (0x0)
#define HI_EXT_SYSTEM_BLACK_LEVEL_11_DATASIZE (16)
// args: data (16-bit)

static __inline HI_VOID hi_ext_system_black_level_11_write(HI_U16 data)
{
     IOWR_16DIRECT(0x11606, data);
}

static __inline HI_U16 hi_ext_system_black_level_11_read(HI_VOID)
{
    return  IORD_16DIRECT(0x11606);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_black_level_change
// ------------------------------------------------------------------------------ //
// ISP black level change 
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_BLACK_LEVEL_CHANGE_DEFAULT  (0x0)
#define HI_EXT_SYSTEM_BLACK_LEVEL_CHANGE_DATASIZE (1)
// args: data (1-bit)

static __inline HI_VOID hi_ext_system_black_level_change_write(HI_U8 data)
{
    IOWR_8DIRECT(0x11608, data);
}

static __inline HI_U8 hi_ext_system_black_level_change_read(HI_VOID)
{
    return IORD_8DIRECT(0x11608);
}

//==============================================================//
//DPC: 0x11700~0x117FF
//==============================================================//

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dpc_static_cor_enable
// ------------------------------------------------------------------------------ //
// defect pixel static correction enable
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DPC_STATIC_COR_ENABLE_DEFAULT (0x01)
#define HI_EXT_SYSTEM_DPC_STATIC_COR_ENABLE_DATASIZE (1)

// 0x12600 ~ dpc_static_cor_enable
static __inline HI_VOID hi_ext_system_dpc_static_cor_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x11700, data&0x1);
}
static __inline HI_U8 hi_ext_system_dpc_static_cor_enable_read(HI_VOID) {
    return  (IORD_8DIRECT(0x11700) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dpc_static_calib_enable
// ------------------------------------------------------------------------------ //
// defect pixel static correction enable
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DPC_STATIC_CALIB_ENABLE_DEFAULT (0x0)
#define HI_EXT_SYSTEM_DPC_STATIC_CALIB_ENABLE_DATASIZE (1)

// 0x12601 ~ dpc_static_calib_enable
static __inline HI_VOID hi_ext_system_dpc_static_calib_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x11701, data&0x1);
}
static __inline HI_U8 hi_ext_system_dpc_static_calib_enable_read(HI_VOID) {
    return  (IORD_8DIRECT(0x11701) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dpc_defect_type
// ------------------------------------------------------------------------------ //
// ISP_STATIC_DP_BRIGHT:0x0,ISP_STATIC_DP_DARK:0x1
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DPC_STATIC_DEFECT_TYPE_DEFAULT (0x0)
#define HI_EXT_SYSTEM_DPC_STATIC_DEFECT_TYPE_DATASIZE (1)

// 0x12602 ~ enStaticDPType
static __inline HI_VOID hi_ext_system_dpc_static_defect_type_write(HI_U8 data) {
    IOWR_8DIRECT(0x11702, data&0x1);
}
static __inline HI_U8 hi_ext_system_dpc_static_defect_type_read(HI_VOID) {
    return  (IORD_8DIRECT(0x11702) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dpc_start_thresh
// ------------------------------------------------------------------------------ //
// define the start thresh of defect pixel calibration
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DPC_START_THRESH_DEFAULT (0x3)
#define HI_EXT_SYSTEM_DPC_START_THRESH_DATASIZE (8)

// 0x12603 ~ u8StartThresh
static __inline HI_VOID hi_ext_system_dpc_start_thresh_write(HI_U8 data) {
    IOWR_8DIRECT((0x11703), data);
}
static __inline HI_U8 hi_ext_system_dpc_start_thresh_read(HI_VOID) {
    return IORD_8DIRECT(0x11703);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dpc_finish_thresh
// ------------------------------------------------------------------------------ //
//  the finish thresh of defect pixel calibration
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DPC_FINISH_THRESH_DATASIZE (8)

// 0x12604 ~ u8FinishThresh
static __inline HI_VOID hi_ext_system_dpc_finish_thresh_write(HI_U8 data) {
    IOWR_8DIRECT(0x11704, data);
}
static __inline HI_U8 hi_ext_system_dpc_finish_thresh_read(HI_VOID) {
    return IORD_8DIRECT(0x11704);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dpc_trigger_status
// ------------------------------------------------------------------------------ //
// the static bad pixel trigger status 
// 0:Initial status
// 1:Finished
// 2:time out
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DPC_TRIGGER_STATUS_DEFAULT (0x0)
#define HI_EXT_SYSTEM_DPC_TRIGGER_STATUS_DATASIZE (2)

// 0x12605 ~ enStatus
static __inline HI_VOID hi_ext_system_dpc_trigger_status_write(HI_U8 data) {
    IOWR_8DIRECT(0x11705, data);
}
static __inline HI_U8 hi_ext_system_dpc_trigger_status_read(HI_VOID) {
    return IORD_8DIRECT(0x11705);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dpc_static_dp_show
// ------------------------------------------------------------------------------ //
// highlight static defect pixel
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DPC_STATIC_DP_SHOW_DEFAULT (0x0)
#define HI_EXT_SYSTEM_DPC_STATIC_DP_SHOW_DATASIZE (1)

// 0x12606 ~ bShow
static __inline HI_VOID hi_ext_system_dpc_static_dp_show_write(HI_U8 data) {
    IOWR_8DIRECT(0x11706, data&0x1);
}
static __inline HI_U8 hi_ext_system_dpc_static_dp_show_read(HI_VOID) {
    return  (IORD_8DIRECT(0x11706) & 0x1);
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dpc_dynamic_cor_enable
// ------------------------------------------------------------------------------ //
// defect pixel dynamic correction enable
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DPC_DYNAMIC_COR_ENABLE_DEFAULT (0x01)
#define HI_EXT_SYSTEM_DPC_DYNAMIC_COR_ENABLE_DATASIZE (1)

// 0x12607 ~ dynamic_cor_enable
static __inline HI_VOID hi_ext_system_dpc_dynamic_cor_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x11707, data&0x1);
}
static __inline HI_U8 hi_ext_system_dpc_dynamic_cor_enable_read(HI_VOID) {
    return  (IORD_8DIRECT(0x11707) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dpc_count_max
// ------------------------------------------------------------------------------ //
// bad pixel calibration  the maxmum bad pixel count
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DPC_COUNT_MAX_DEFAULT (0x400)
#define HI_EXT_SYSTEM_DPC_COUNT_MAX_DATASIZE (16)

// 0x12608 ~ 0x12609 u16CountMax
static __inline HI_VOID hi_ext_system_dpc_count_max_write(HI_U16 data) {
    IOWR_16DIRECT(0x11708, data);
}
static __inline HI_U16 hi_ext_system_dpc_count_max_read(HI_VOID) {
    return IORD_16DIRECT(0x11708);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dpc_count_min
// ------------------------------------------------------------------------------ //
// bad pixel calibration  the minmum bad pixel count
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DPC_COUNT_MIN_DEFAULT (0x1)
#define HI_EXT_SYSTEM_DPC_COUNT_MIN_DATASIZE (16)

// 0x1260a ~0x1260b u16CountMin
static __inline HI_VOID hi_ext_system_dpc_count_min_write(HI_U16 data) {
    IOWR_16DIRECT(0x1170a, data);
}
static __inline HI_U16 hi_ext_system_dpc_count_min_read(HI_VOID) {
    return IORD_16DIRECT(0x1170a);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dpc_trigger_time
// ------------------------------------------------------------------------------ //
// the bad pixel trigger frame cnt of time out 
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DPC_TRIGGER_TIME_DEFAULT (0x640)
#define HI_EXT_SYSTEM_DPC_TRIGGER_TIME_DATASIZE (16)

// 0x1260c ~ 0x1260d enStatus
static __inline HI_VOID hi_ext_system_dpc_trigger_time_write(HI_U16 data) {
    IOWR_16DIRECT(0x1170c, data);
}
static __inline HI_U16 hi_ext_system_dpc_trigger_time_read(HI_VOID) {
    return IORD_16DIRECT(0x1170c) ;
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dpc_dynamic_manual_enable
// ------------------------------------------------------------------------------ //
// 0:auto;
//1:manual
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DPC_DYNAMIC_MANUAL_ENABLE_DEFAULT (0x0)
#define HI_EXT_SYSTEM_DPC_DYNAMIC_MANUAL_ENABLE_DATASIZE (1)

// 0x1260e ~ dynamic_manual_enable
static __inline HI_VOID hi_ext_system_dpc_dynamic_manual_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x1170e, data&0x1);
}
static __inline HI_U8 hi_ext_system_dpc_dynamic_manual_enable_read(HI_VOID) {
    return  (IORD_8DIRECT(0x1170e) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dpc_dynamic_strength_table
// ------------------------------------------------------------------------------ //
// 0x11710 ~ 0x1172f dpc_dynamic_strength_table
#define HI_EXT_SYSTEM_DPC_DYNAMIC_STRENGTH_TABLE_DATASIZE (16)

static __inline HI_VOID hi_ext_system_dpc_dynamic_strength_table_write(HI_U8 u8Index, HI_U16 data){
	IOWR_16DIRECT(0x11710+u8Index*sizeof(HI_U16), data);
}

static __inline HI_U16 hi_ext_system_dpc_dynamic_strength_table_read(HI_U8 u8Index){
	return IORD_16DIRECT(0x11710 + u8Index*sizeof(HI_U16));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dpc_dynamic_blend_ratio_table
// ------------------------------------------------------------------------------ //
// 0x11730 ~ 0x1174f dpc_blend_ratio_table
#define HI_EXT_SYSTEM_DPC_DYNAMIC_BLEND_RATIO_TABLE_DATASIZE (16)

static __inline HI_VOID hi_ext_system_dpc_dynamic_blend_ratio_table_write(HI_U8 u8Index, HI_U16 data){
	IOWR_16DIRECT(0x11730+u8Index*sizeof(HI_U16), data);
}

static __inline HI_U16 hi_ext_system_dpc_dynamic_blend_ratio_table_read(HI_U8 u8Index){
	return IORD_16DIRECT(0x11730 + u8Index*sizeof(HI_U16));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dpc_dynamic_strength
// ------------------------------------------------------------------------------ //
// 0x12650 ~ 0x12651 dpc_dynamic_strength

#define HI_EXT_SYSTEM_DPC_DYNAMIC_STRENGTH_DEFAULT (0x0)
#define HI_EXT_SYSTEM_DPC_DYNAMIC_STRENGTH_DATASIZE (16)

static __inline HI_VOID hi_ext_system_dpc_dynamic_strength_write( HI_U16 data){
	IOWR_16DIRECT(0x11750, data);
}

static __inline HI_U16 hi_ext_system_dpc_dynamic_strength_read(HI_VOID){
	return IORD_16DIRECT(0x11750);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dpc_dynamic_blend_ratio_table
// ------------------------------------------------------------------------------ //
// 0x12652 ~ 0x12653 dpc_blend_ratio

#define HI_EXT_SYSTEM_DPC_DYNAMIC_BLEND_RATIO_DEFAULT (0x0)
#define HI_EXT_SYSTEM_DPC_DYNAMIC_BLEND_RATIO_DATASIZE (9)
static __inline HI_VOID hi_ext_system_dpc_dynamic_blend_ratio_write( HI_U16 data){
	IOWR_16DIRECT(0x11752, data);
}

static __inline HI_U16 hi_ext_system_dpc_dynamic_blend_ratio_read(HI_VOID){
	return IORD_16DIRECT(0x11752);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dpc_manual_mode
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DPC_MANU_MODE_DEFAULT (0x0)
#define HI_EXT_SYSTEM_DPC_MANU_MODE_DATASIZE (1)

static __inline HI_VOID hi_ext_system_dpc_manual_mode_write(HI_U8 data) {
    IOWR_8DIRECT(0x11754, data&0x1);
}
static __inline HI_U8 hi_ext_system_dpc_manual_mode_read(HI_VOID) {
    return (IORD_8DIRECT(0x11754) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dpc_alpha0_rb_write
// ------------------------------------------------------------------------------ //
// 0x12655 ~hi_ext_system_dpc_alpha0_rb_write
#define HI_EXT_SYSTEM_DPC_DYNAMIC_ALPHA0_RB_DEFAULT  (0x0)
#define HI_EXT_SYSTEM_DPC_DYNAMIC_ALPHA0_RB_DATASIZE (8)

static __inline HI_VOID hi_ext_system_dpc_alpha0_rb_write(HI_U8 data) {
    IOWR_8DIRECT(0x11755, data);
}
static __inline HI_U8 hi_ext_system_dpc_alpha0_rb_read(HI_VOID) {
    return (IORD_8DIRECT(0x11755));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dpc_alpha0_rb_write
// ------------------------------------------------------------------------------ //
// 0x12655 ~hi_ext_system_dpc_alpha0_rb_write
#define HI_EXT_SYSTEM_DPC_DYNAMIC_ALPHA0_G_DEFAULT (0x0)
static __inline HI_VOID hi_ext_system_dpc_alpha0_g_write(HI_U8 data) {
    IOWR_8DIRECT(0x11756, data);
}
static __inline HI_U8 hi_ext_system_dpc_alpha0_g_read(HI_VOID) {
    return (IORD_8DIRECT(0x11756));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dpc_suppress_twinkle_enable_write
// ------------------------------------------------------------------------------ //
// 0x12657 ~hi_ext_system_dpc_suppress_twinkle_enable
#define HI_EXT_SYSTEM_DPC_SUPPRESS_TWINKLE_ENABLE_DEFAULT (0x0)
static __inline HI_VOID hi_ext_system_dpc_suppress_twinkle_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x11757, data&0x1);
}
static __inline HI_U8 hi_ext_system_dpc_suppress_twinkle_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x11757)&0x1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dpc_suppress_twinkle_thr_write
// ------------------------------------------------------------------------------ //
// 0x12658 ~hi_ext_system_dpc_suppress_twinkle_thr
#define HI_EXT_SYSTEM_DPC_SUPPRESS_TWINKLE_THR_DEFAULT (0x6)
static __inline HI_VOID hi_ext_system_dpc_suppress_twinkle_thr_write(HI_S8 data) {
    IOWR_16DIRECT(0x11758, data);
}
static __inline HI_S8 hi_ext_system_dpc_suppress_twinkle_thr_read(HI_VOID) {
    return (IORD_16DIRECT(0x11758)&0xFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dpc_suppress_twinkle_slope_write
// ------------------------------------------------------------------------------ //
// 0x12659 ~0x1265a hi_ext_system_dpc_suppress_twinkle_slope
#define HI_EXT_SYSTEM_DPC_SUPPRESS_TWINKLE_SLOPE_DEFAULT (0x15)
static __inline HI_VOID hi_ext_system_dpc_suppress_twinkle_slope_write(HI_U8 data) {
    IOWR_16DIRECT(0x1175a, data&0xFF);
}
static __inline HI_U8 hi_ext_system_dpc_suppress_twinkle_slope_read(HI_VOID) {
    return (IORD_16DIRECT(0x1175a)&0xFF);
}




//==============================================================//
//GE: 0x11800~0x118FF
//==============================================================//
// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_ge_npoffset
// ------------------------------------------------------------------------------ //
// ge npoffset table 0x11800 - 0x1181f
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_GE_STRENGTH_DATASIZE (16)
// args: data (16-bit)

static __inline HI_VOID hi_ext_system_ge_npoffset_write(HI_U8 u8Index, HI_U16 data){
    IOWR_16DIRECT(0x11800+u8Index*sizeof(HI_U16), data);
}

static __inline HI_U16 hi_ext_system_ge_npoffset_read(HI_U8 u8Index){
    return IORD_16DIRECT(0x11800 + u8Index*sizeof(HI_U16));
}

// 0x12722 ~ 0x12723 ge_slope
static __inline HI_VOID hi_ext_system_ge_slope_write( HI_U16 data){
	IOWR_16DIRECT(0x11822, data);
}

static __inline HI_U16 hi_ext_system_ge_slope_read(HI_VOID){
	return IORD_16DIRECT(0x11822);
}

// 0x12724 ~ 0x12725 ge_sensitivity
static __inline HI_VOID hi_ext_system_ge_sensitivity_write( HI_U16 data){
	IOWR_16DIRECT(0x11824, data);
}

static __inline HI_U16 hi_ext_system_ge_sensitivity_read(HI_VOID){
	return IORD_16DIRECT(0x11824);
}

// 0x12726 ~ 0x12727 ge_sensithreshold
static __inline HI_VOID hi_ext_system_ge_sensithreshold_write( HI_U16 data){
	IOWR_16DIRECT(0x11826, data);
}

static __inline HI_U16 hi_ext_system_ge_sensithreshold_read(HI_VOID){
	return IORD_16DIRECT(0x11826);
}

// 0x12728 ~ 0x12728 ge_enable

static __inline HI_VOID hi_ext_system_ge_enable_write( HI_U8 data){
	IOWR_8DIRECT(0x11828, data&0x1);
}

static __inline HI_U8 hi_ext_system_ge_enable_read(HI_VOID){
	return (IORD_8DIRECT(0x11828)& 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_ge_threshold_
// ------------------------------------------------------------------------------ //
// ge threshold_ table 0x1182a - 0x11849
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_GE_THRESHOLD_DEFAULT (0)
#define HI_EXT_SYSTEM_GE_THRESHOLD_DATASIZE (16)
// args: data (16-bit)

static __inline HI_VOID hi_ext_system_ge_threshold_write(HI_U8 u8Index, HI_U16 data){
    IOWR_16DIRECT(0x11830+(u8Index*sizeof(HI_U16)), data);
}

static __inline HI_U16 hi_ext_system_ge_threshold_read(HI_U8 u8Index){
    return IORD_16DIRECT(0x11830 + (u8Index*sizeof(HI_U16)));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_ge_strength_
// ------------------------------------------------------------------------------ //
// ge strength_ table 0x11850 - 0x1186f
// ------------------------------------------------------------------------------ //

static __inline HI_VOID hi_ext_system_ge_strength_write(HI_U8 u8Index, HI_U16 data){
    IOWR_16DIRECT(0x11850+u8Index*sizeof(HI_U16), data);
}

static __inline HI_U16 hi_ext_system_ge_strength_read(HI_U8 u8Index){
    return IORD_16DIRECT(0x11850 + u8Index*sizeof(HI_U16));
}

//==============================================================//
//DEMOSAIC: 0x11900~0x119FF
//==============================================================//
// 0x12800 ~ fcr_manu_mode
#define HI_EXT_SYSTEM_DEMOSAIC_MANUAL_MODE_DEFAULT (0)
static __inline HI_VOID hi_ext_system_demosaic_manual_mode_write(HI_U8 data) {
    IOWR_8DIRECT(0x11900, data&0x1);
}
static __inline HI_U8 hi_ext_system_demosaic_manual_mode_read(HI_VOID) {
    return (IORD_8DIRECT(0x11900)&0x1);
}

static __inline HI_VOID hi_ext_system_fcr_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x11901, data&0x1);
}
static __inline HI_U8 hi_ext_system_fcr_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x11901)&0x1);
}

#define HI_EXT_SYSTEM_DEMOSAIC_MANUAL_SATUFIXEHCY_LINEAR_DEFAULT (256)
#define HI_EXT_SYSTEM_DEMOSAIC_MANUAL_SATUFIXEHCY_WDR_DEFAULT (320)
static __inline HI_VOID hi_ext_system_demosaic_manual_satufixehcy_write(HI_U16 data) {
    IOWR_16DIRECT(0x11902, data);
}
static __inline HI_U16 hi_ext_system_demosaic_manual_satufixehcy_read(HI_VOID) {
    return (IORD_16DIRECT(0x11902));
}

/*0x11904 ~ 0x11913*/

/*0x11914 ~ 0x11923*/

static __inline HI_VOID hi_ext_system_manual_fcr_strength_write(HI_U8 data) {
    IOWR_8DIRECT(0x11904 , data);
}
static __inline HI_U8 hi_ext_system_manual_fcr_strength_read(HI_VOID) {
    return IORD_8DIRECT(0x11904);
}

static __inline HI_VOID hi_ext_system_manual_fcr_threshold_write(HI_U8 data) {
    IOWR_8DIRECT(0x11905 , data);
}
static __inline HI_U8 hi_ext_system_manual_fcr_threshold_read(HI_VOID) {
    return IORD_8DIRECT(0x11905);
}

#define HI_EXT_SYSTEM_FCR_MANUAL_MODE_DEFAULT (0)
static __inline HI_VOID hi_ext_system_fcr_manual_mode_write(HI_U8 data) {
    IOWR_8DIRECT(0x11906, data&0x1);
}
static __inline HI_U8 hi_ext_system_fcr_manual_mode_read(HI_VOID) {
    return ((IORD_8DIRECT(0x11906)&0x1));
}

#define HI_EXT_SYSTEM_FCR_ENABLE_DEFAULT (0x1)

#define HI_EXT_SYSTEM_DEMOSAIC_ENABLE_DEFAULT (0x1)
#define HI_EXT_SYSTEM_DEMOSAIC_ENABLE_DATASIZE (1)

static __inline HI_VOID hi_ext_system_demosaic_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x11907, data&0x1);
}
static __inline HI_U8 hi_ext_system_demosaic_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x11907)&0x1);
}

#define HI_EXT_SYSTEM_DEMOSAIC_MANUAL_NOISE_SUPPRESS_STR_LINEAR_DEFAULT (0)
#define HI_EXT_SYSTEM_DEMOSAIC_MANUAL_NOISE_SUPPRESS_STR_WDR_DEFAULT (20)
static __inline HI_VOID  hi_ext_system_demosaic_manual_noise_suppress_strength_write(HI_U16 data){
	IOWR_16DIRECT(0x11908, data);
}
static __inline HI_U16 hi_ext_system_demosaic_manual_noise_suppress_strength_read(HI_VOID){
	 return IORD_16DIRECT(0x11908);
}

#define HI_EXT_SYSTEM_DEMOSAIC_MANUAL_COLORNOISE_CTRL_THRESH_LINEAR_DEFAULT (0)
#define HI_EXT_SYSTEM_DEMOSAIC_MANUAL_COLORNOISE_CTRL_THRESH_WDR_DEFAULT (0)
static __inline HI_VOID  hi_ext_system_demosaic_manual_colornoise_ctrl_thresh_write(HI_U16 data){
	IOWR_16DIRECT(0x1190a, data);
}
static __inline HI_U16 hi_ext_system_demosaic_manual_colornoise_ctrl_thresh_read(HI_VOID){
	 return IORD_16DIRECT(0x1190a);
}
#define HI_EXT_SYSTEM_DEMOSAIC_MANU_HFINTP_BLD_LOW_NR_EDGE_SLOPE2_LINEAR_DEFAULT (105)
#define HI_EXT_SYSTEM_DEMOSAIC_MANU_HFINTP_BLD_LOW_NR_EDGE_SLOPE2_WDR_DEFAULT (255)
static __inline HI_VOID hi_ext_system_demosaic_manual_hf_intp_bld_low_nr_edge_slope2_write(HI_U8 data){
	IOWR_8DIRECT(0x1190c, data );
}
static __inline HI_U8 hi_ext_system_demosaic_manual_hf_intp_bld_low_nr_edge_slope2_read(HI_VOID){
	return IORD_8DIRECT(0x1190c);
}

#define HI_EXT_SYSTEM_DEMOSAIC_MANU_ANTIALIAS_TH_LINEAR_DEFAULT (53)
#define HI_EXT_SYSTEM_DEMOSAIC_MANU_ANTIALIAS_TH_WDR_DEFAULT (1022)
static __inline HI_VOID hi_ext_system_demosaic_manual_antialias_thresh_write(HI_U16 data){
	IOWR_16DIRECT(0x1190e, data);
}
static __inline HI_U16 hi_ext_system_demosaic_manual_antialias_thresh_read(HI_VOID){
	return IORD_16DIRECT(0x1190e);
}

#define HI_EXT_SYSTEM_DEMOSAIC_MANU_ANTIALIAS_SLOPE_LINEAR_DEFAULT (256)
#define HI_EXT_SYSTEM_DEMOSAIC_MANU_ANTIALIAS_SLOPE_WDR_DEFAULT (1023)
static __inline HI_VOID hi_ext_system_demosaic_manual_antialias_slope_write(HI_U16 data){
	IOWR_16DIRECT(0x11910, data);
}
static __inline HI_U16 hi_ext_system_demosaic_manual_antialias_slope_read(HI_VOID){
	return IORD_16DIRECT(0x11910);
}

#define HI_ISP_EXT_DEMOSAIC_HV_RATIO_GRAY_EDGE_SLOPE_DEFAULT (207)
static __inline HI_VOID hi_ext_system_demosaic_hv_ratio_gray_edge_slope_write(HI_U8 data){
	IOWR_8DIRECT(0x11912, data);
}

static __inline HI_U8 hi_ext_system_demosaic_hv_ratio_gray_edge_slope_read(HI_VOID){
	return IORD_8DIRECT(0x11912);
}

#define HI_EXT_SYSTEM_DEMOSAIC_MANU_AHD_PART1_COLOR_EDGE_SLOPE2_DEFAULT (400)
static __inline HI_VOID hi_ext_system_demosaic_manual_ahd_part1_color_edge_slope2_write(HI_U16 data){
	IOWR_16DIRECT(0x11914, data);
}

static __inline HI_U16 hi_ext_system_demosaic_manual_ahd_part1_color_edge_slope2_read(HI_VOID){
	return IORD_16DIRECT(0x11914);
}

static __inline HI_VOID hi_ext_system_manual_fcr_offset_write(HI_U16 data){
	IOWR_16DIRECT(0x11916, data);
}

static __inline HI_U16 hi_ext_system_manual_fcr_offset_read(HI_VOID){
	return IORD_16DIRECT(0x11916);
}

#define HI_EXT_SYSTEM_DEMOSAIC_MANU_EDGE_SMOOTH_TH_LINEAR_DEFAULT (16)
#define HI_EXT_SYSTEM_DEMOSAIC_MANU_EDGE_SMOOTH_TH_WDR_DEFAULT (1022)
static __inline HI_VOID hi_ext_system_demosaic_manual_edge_smooth_thresh_write(HI_U16 data){
	IOWR_16DIRECT(0x11918, data);
}
static __inline HI_U16 hi_ext_system_demosaic_manual_edge_smooth_thresh_read(HI_VOID){
	return IORD_16DIRECT(0x11918);
}

#define HI_EXT_SYSTEM_DEMOSAIC_MANU_EDGE_SMOOTH_SLOPE_LINEAR_DEFAULT (32)
#define HI_EXT_SYSTEM_DEMOSAIC_MANU_EDGE_SMOOTH_SLOPE_WDR_DEFAULT (0)
static __inline HI_VOID hi_ext_system_demosaic_manual_edge_smooth_slope_write(HI_U16 data){
	IOWR_16DIRECT(0x1191a, data);
}
static __inline HI_U16 hi_ext_system_demosaic_manual_edge_smooth_slope_read(HI_VOID){
	return IORD_16DIRECT(0x1191a);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_ehcgainlut_write len =17
// ------------------------------------------------------------------------------ //
static __inline HI_VOID hi_ext_system_ehcgainlut_update_write(HI_U8 data){
	IOWR_8DIRECT(0x1191c, data);
}
static __inline HI_U8 hi_ext_system_ehcgainlut_update_read(HI_VOID){
	return IORD_8DIRECT(0x1191c);
}


static __inline HI_VOID hi_ext_system_auto_fcr_strength_write(HI_U8 u8Index,HI_U8 data) {
    IOWR_8DIRECT(0x11930 + u8Index*sizeof(HI_U8), data);
}
static __inline HI_U8 hi_ext_system_auto_fcr_strength_read(HI_U8 u8Index) {
    return IORD_8DIRECT(0x11930 + (u8Index*sizeof(HI_U8)));
}

static __inline HI_VOID hi_ext_system_auto_fcr_threshold_write(HI_U8 u8Index,HI_U8 data) {
    IOWR_8DIRECT(0x11940 + u8Index*sizeof(HI_U8), data);
}
static __inline HI_U8 hi_ext_system_auto_fcr_threshold_read(HI_U8 u8Index) {
    return IORD_8DIRECT(0x11940 + (u8Index*sizeof(HI_U8)));
}
static __inline HI_VOID hi_ext_system_demosaic_auto_colornoise_ctrl_thresh_write(HI_U8 u8Index,HI_U16 data) {
    IOWR_16DIRECT(0x11950 + u8Index*sizeof(HI_U16), data);
}
static __inline HI_U16 hi_ext_system_demosaic_auto_colornoise_ctrl_thresh_read(HI_U8 u8Index) {
    return IORD_16DIRECT(0x11950 + (u8Index*sizeof(HI_U16)));
}

#define HI_EXT_SYSTEM_DEMOSAIC_AUTO_HFINTP_TH_HIGH_NR_EDGE_SLOPE1_DEFAULT (32)
static __inline HI_VOID hi_ext_system_demosaic_auto_edge_smooth_slope_write(HI_U8 u8Index, HI_U16 data){
	IOWR_16DIRECT(0x11970+(u8Index*sizeof(HI_U16)), data);
}
static __inline HI_U16 hi_ext_system_demosaic_auto_edge_smooth_slope_read(HI_U8 u8Index){
	return IORD_16DIRECT(0x11970+(u8Index*sizeof(HI_U16)));
}


#define HI_EXT_SYSTEM_DEMOSAIC_AUTO_HFINTP_BLD_LOW_NR_EDGE_SLOPE2_DEFAULT (105)
static __inline HI_VOID hi_ext_system_demosaic_auto_hf_intp_bld_low_nr_edge_slope2_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT(0x11990+(u8Index*sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_demosaic_auto_hf_intp_bld_low_nr_edge_slope2_read(HI_U8 u8Index){
	return IORD_8DIRECT(0x11990+(u8Index*sizeof(HI_U8)));
}
#define HI_EXT_SYSTEM_DEMOSAIC_AUTO_ANTIALIAS_SLOPE2_DEFAULT (970)
static __inline HI_VOID hi_ext_system_demosaic_auto_antialias_thresh_write(HI_U8 u8Index, HI_U16 data){
	IOWR_16DIRECT(0x119a0+(u8Index*sizeof(HI_U16)), data);
}
static __inline HI_U16 hi_ext_system_demosaic_auto_antialias_thresh_read(HI_U8 u8Index){
	return IORD_16DIRECT(0x119a0+(u8Index*sizeof(HI_U16)));
}
#define HI_EXT_SYSTEM_DEMOSAIC_AUTO_ANTIALIAS_SLOPE3_DEFAULT (820)
static __inline HI_VOID hi_ext_system_demosaic_auto_antialias_slope_write(HI_U8 u8Index, HI_U16 data){
	IOWR_16DIRECT(0x119c0+(u8Index*sizeof(HI_U16)), data);
}
static __inline HI_U16 hi_ext_system_demosaic_auto_antialias_slope_read(HI_U8 u8Index){
	return IORD_16DIRECT(0x119c0+(u8Index*sizeof(HI_U16)));
}

#define HI_ISP_EXT_FCR_OFFSET_DEFAULT (150)
static __inline HI_VOID hi_ext_system_auto_fcr_offset_write(HI_U8 u8Index, HI_U16 data){
	IOWR_16DIRECT(0x119e0+(u8Index*sizeof(HI_U16)), data);
}
static __inline HI_U16 hi_ext_system_auto_fcr_offset_read(HI_U8 u8Index){
	return IORD_16DIRECT(0x119e0+(u8Index*sizeof(HI_U16)));
}

//==============================================================//
//MCDS: 0x11A00~0x11AFF
//==============================================================//
// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_mcds_midfbldr
// ------------------------------------------------------------------------------ //
//middle fliter blend ratio table
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MCDS_MIDFBLDR_DATASIZE (8)
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_mcds_midfbldr_write(HI_U8 i, HI_U8 data) {
    IOWR_8DIRECT(0x11a00+(i*sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_mcds_midfbldr_read(HI_U8 i) {
    return IORD_8DIRECT(0x11a00+(i*sizeof(HI_U8)));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_mcds_coringlimit
// ------------------------------------------------------------------------------ //
//coring limit 
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MCDS_CORINGLMT_DATASIZE (8)
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_mcds_coringlimit_write(HI_U8 i, HI_U8 data) {
    IOWR_8DIRECT(0x11a10+(i*sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_mcds_coringlimit_read(HI_U8 i) {
    return IORD_8DIRECT(0x11a10+(i*sizeof(HI_U8)));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_mcds_midfbldr
// ------------------------------------------------------------------------------ //
//middle fliter blend ratio 
// ------------------------------------------------------------------------------ //
#define HI_ISP_EXT_MANUAL_MIDFBLDR_DEFAULT (0x8)
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_manual_mcds_midfbldr_write(HI_U8 data) {
    IOWR_8DIRECT(0x11a20, data);
}
static __inline HI_U8 hi_ext_system_manual_mcds_midfbldr_read(HI_VOID) {
    return IORD_8DIRECT(0x11a20);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_mcds_midfbldr
// ------------------------------------------------------------------------------ //
//middle fliter blend ratio 
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
#define HI_ISP_EXT_MANUAL_CORINGLIMIT_DEFAULT (0x0)

static __inline HI_VOID hi_ext_system_manual_mcds_coringlimit_write(HI_U8 data) {
    IOWR_8DIRECT(0x11a21, data);
}
static __inline HI_U8 hi_ext_system_manual_mcds_coringlimit_read(HI_VOID) {
    return IORD_8DIRECT(0x11a21);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_mcds_midfbldr
// ------------------------------------------------------------------------------ //
//middle fliter blend ratio 
// ------------------------------------------------------------------------------ //
// args: data (8-bit)

#define HI_ISP_EXT_MANUAL_MODE_DEFAULT (0x0)
static __inline HI_VOID hi_ext_system_mcds_manual_mode_write(HI_U8 data) {
    IOWR_8DIRECT(0x11a22, data);
}
static __inline HI_U8 hi_ext_system_mcds_manual_mode_read(HI_VOID) {
    return IORD_8DIRECT(0x11a22);
}
/*
//==============================================================//
//BAYER NR: 0x11B00~0x11CFF
//==============================================================//
// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_bayernr_rawnr_cratio_rb
// ------------------------------------------------------------------------------ //
//c ratio table
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_BAYERNR_CRATIO_R_DATASIZE (8)
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_bayernr_chroma_ratio_r_write(HI_U8 i, HI_U8 data) {
    IOWR_8DIRECT(0x11b00+(i*sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_bayernr_chroma_ratio_r_read(HI_U8 i) {
    return IORD_8DIRECT(0x11b00+(i*sizeof(HI_U8)));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_bayernr_rawnr_cratio_g
// ------------------------------------------------------------------------------ //
//c ratio table
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_BAYERNR_CRATIO_G_DATASIZE (8)
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_bayernr_chroma_ratio_gr_write(HI_U8 i, HI_U8 data) {
    IOWR_8DIRECT(0x11b10+(i*sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_bayernr_chroma_ratio_gr_read(HI_U8 i) {
    return IORD_8DIRECT(0x11b10+(i*sizeof(HI_U8)));
}
*/
// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_bayernr_jnlm_gain
// ------------------------------------------------------------------------------ //
//c ratio table
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_BAYERNR_JNLM_GAIN_DATASIZE (8)
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_bayernr_auto_fine_strength_write(HI_U8 i, HI_U8 data) {
    IOWR_8DIRECT(0x11b20+(i*sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_bayernr_auto_fine_strength_read(HI_U8 i) {
    return IORD_8DIRECT(0x11b20+(i*sizeof(HI_U8)));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_bayernr_jnlm_coring_hig
// ------------------------------------------------------------------------------ //
//c ratio table
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_BAYERNR_JNLM_CORING_HIG_DATASIZE (16)
// args: data (8-bit)
static __inline HI_VOID hi_isp_bayernr_jnlm_coring_hig_write(HI_U8 i, HI_U16 data) {
    IOWR_16DIRECT(0x11b30+(i*sizeof(HI_U16)), data);
}
static __inline HI_U16 hi_isp_bayernr_jnlm_coring_hig_read(HI_U8 i) {
    return IORD_16DIRECT(0x11b30+(i*sizeof(HI_U16)));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_bayernr_jnlm_coring_low
// ------------------------------------------------------------------------------ //
//bayernr jnlm coring low
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_BAYERNR_JNLM_CORING_LOW_DATASIZE (16)
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_bayernr_auto_coring_weight_write(HI_U8 i, HI_U16 data) {
    IOWR_16DIRECT(0x11b50+(i*sizeof(HI_U16)), data);
}
static __inline HI_U16 hi_ext_system_bayernr_auto_coring_weight_read(HI_U8 i) {
    return IORD_16DIRECT(0x11b50+(i*sizeof(HI_U16)));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_bayernr_amed_level
// ------------------------------------------------------------------------------ //
//bayernr amed level
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_BAYERNR_MANU_CHROMA_STRENGTH_DEFAULT (0)
#define HI_EXT_SYSTEM_BAYERNR_AMED_LEVEL_DATASIZE (8)
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_bayernr_manual_chroma_strength_write(HI_U8 i, HI_U8 data) {
    IOWR_8DIRECT(0x11b70+(i*sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_bayernr_manual_chroma_strength_read(HI_U8 i) {
    return IORD_8DIRECT(0x11b70+(i*sizeof(HI_U8)));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_bayernr_wdr_strength
// ------------------------------------------------------------------------------ //
//bayernr strength table in wdr mode
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_BAYERNR_WDR_STRENGTH_DATASIZE (8)
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_bayernr_wdr_strength_write(HI_U8 i, HI_U8 data) {
    IOWR_8DIRECT(0x11b80+(i*sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_bayernr_wdr_strength_read(HI_U8 i) {
    return IORD_8DIRECT(0x11b80+(i*sizeof(HI_U8)));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_bayernr_wdr_frameths
// ------------------------------------------------------------------------------ //
//bayernr frame threshold table in wdr mode
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_BAYERNR_FRAMETHS_DATASIZE (16)
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_bayernr_wdr_frameths_write(HI_U8 i, HI_U16 data) {
    IOWR_16DIRECT(0x11b90+(i*sizeof(HI_U16)), data);
}
static __inline HI_U16 hi_ext_system_bayernr_wdr_frameths_read(HI_U8 i) {
    return IORD_16DIRECT(0x11b90+(i*sizeof(HI_U16)));
}
/*
// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_bayernr_cprclip
// ------------------------------------------------------------------------------ //
//bayernr frame threshold table in wdr mode
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_BAYERNR_CPRCLIP_DATASIZE (16)
// args: data (16-bit)
static __inline HI_VOID hi_ext_system_bayernr_cprclip_write(HI_U8 i, HI_U16 data) {
    IOWR_16DIRECT(0x11bb0+(i*sizeof(HI_U16)), data);
}
static __inline HI_U16 hi_ext_system_bayernr_cprclip_read(HI_U8 i) {
    return IORD_16DIRECT(0x11bb0+(i*sizeof(HI_U16)));
}*/

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_bayernr_pattern_strength
// ------------------------------------------------------------------------------ //
//bayernr frame threshold table in wdr mode
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_BAYERNR_PATTERN_STRENGTH_DEFAULT  (100)
#define HI_EXT_SYSTEM_BAYERNR_PATTERN_STRENGTH_DATASIZE (16)
// args: data (16-bit)
static __inline HI_VOID hi_ext_system_bayernr_coarse_strength_write(HI_U8 i, HI_U16 data) {
    IOWR_16DIRECT(0x11bd0+(i*sizeof(HI_U16)), data);
}
static __inline HI_U16 hi_ext_system_bayernr_coarse_strength_read(HI_U8 i) {
    return IORD_16DIRECT(0x11bd0+(i*sizeof(HI_U16)));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_bayernr_amed_mode
// ------------------------------------------------------------------------------ //
//bayernr frame threshold table in wdr mode
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_BAYERNR_AMED_MODE_DATASIZE (8)
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_bayernr_amed_mode_write(HI_U8 i, HI_U8 data) {
    IOWR_8DIRECT(0x11bf0+(i*sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_bayernr_amed_mode_read(HI_U8 i) {
    return IORD_8DIRECT(0x11bf0+(i*sizeof(HI_U8)));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_bayernr_manual_mode
// ------------------------------------------------------------------------------ //
//bayernr manual mode
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_BAYERNR_MANU_MODE_DEFAULT  (0)
#define HI_EXT_SYSTEM_BAYERNR_MANU_MODE_DATASIZE (8)
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_bayernr_manual_mode_write(HI_U8 data) {
    IOWR_8DIRECT(0x11c00, data);
}
static __inline HI_U8 hi_ext_system_bayernr_manual_mode_read(HI_VOID) {
    return IORD_8DIRECT(0x11c00);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_bayernr_wdr_blc_init
// ------------------------------------------------------------------------------ //
//bayernr wdr blc
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_BAYERNR_WDR_BLCINIT_DEFAULT  (0)
#define HI_EXT_SYSTEM_BAYERNR_WDR_BLCINIT_DATASIZE (8)
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_bayernr_wdr_blc_init_write(HI_U8 data) {
    IOWR_8DIRECT(0x11c01, data);
}
static __inline HI_U8 hi_ext_system_bayernr_wdr_blc_init_read(HI_VOID) {
    return IORD_8DIRECT(0x11c01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_bayernr_strength
// ------------------------------------------------------------------------------ //
//bayernr_strength
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_BAYERNR_MANU_STRENGTH_DEFAULT  (100)
#define HI_EXT_SYSTEM_BAYERNR_STRENGTH_DATASIZE (16)
// args: data (16-bit)
static __inline HI_VOID hi_ext_system_bayernr_manual_fine_strength_write(HI_U16 data) {
    IOWR_16DIRECT(0x11c02, data);
}
static __inline HI_U16 hi_ext_system_bayernr_manual_fine_strength_read(HI_VOID) {
    return IORD_16DIRECT(0x11c02);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_bayernr_wdr_blc_thresh
// ------------------------------------------------------------------------------ //
//bayernr wdr blc thresh
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_BAYERNR_WDR_BLCTHRESH_DEFAULT  (1024)
#define HI_EXT_SYSTEM_BAYERNR_WDR_BLCTHRESH_DATASIZE (16)
// args: data (16-bit)
static __inline HI_VOID hi_ext_system_bayernr_wdr_blc_thresh_write(HI_U16 data) {
    IOWR_16DIRECT(0x11c04, data);
}
static __inline HI_U16 hi_ext_system_bayernr_wdr_blc_thresh_read(HI_VOID) {
    return IORD_16DIRECT(0x11c04);
}

#define  HI_EXT_SYSTEM_BAYERNR_MANU_CORING_WEIGHT_DEFAULT (800)
static __inline HI_VOID hi_ext_system_bayernr_manual_coring_weight_write(HI_U16 data) {
    IOWR_16DIRECT(0x11c06, data);
}
static __inline HI_U16 hi_ext_system_bayernr_manual_coring_weight_read(HI_VOID) {
    return IORD_16DIRECT(0x11c06);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_bayernr_wdr_blc_init
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_BAYERNR_ENABLE_DEFAULT  (1)
#define HI_EXT_SYSTEM_BAYERNR_ENABLE_DATASIZE (8)
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_bayernr_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x11c08, data);
}
static __inline HI_U8 hi_ext_system_bayernr_enable_read(HI_VOID) {
    return IORD_8DIRECT(0x11c08);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_bayernr_coring_low_ratio_write
// ------------------------------------------------------------------------------ //
//bayernr len =33 0x11c08 -- 0x11c49
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_BAYERNR_CORING_RATIO_LUT_DEFAULT  (800)
#define HI_EXT_SYSTEM_BAYERNR_CORING_RATIO_LUT_DATASIZE (16)
// args: data (16-bit)
static __inline HI_VOID hi_ext_system_bayernr_coring_low_ratio_write(HI_U8 i, HI_U16 data) {
    IOWR_16DIRECT(0x11c20+(i*sizeof(HI_U16)), data);
}
static __inline HI_U16 hi_ext_system_bayernr_coring_low_ratio_read(HI_U8 i) {
    return IORD_16DIRECT(0x11c20+(i*sizeof(HI_U16)));
}
/*
// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_bayernr_lut_b_write
// ------------------------------------------------------------------------------ //
//bayernr len =16 0x11c50 -- 0x11c6f
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_BAYERNR_LUT_B_DATASIZE (16)
// args: data (16-bit)
static __inline HI_VOID hi_ext_system_bayernr_lut_b_write(HI_U8 i, HI_U16 data) {
    IOWR_16DIRECT(0x11c50+(i*sizeof(HI_U16)), data);
}
static __inline HI_U16 hi_ext_system_bayernr_lut_b_read(HI_U8 i) {
    return IORD_16DIRECT(0x11c50+(i*sizeof(HI_U16)));
}

#define HI_EXT_SYSTEM_BAYERNR_CRATIO_GB_DATASIZE (8)
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_bayernr_chroma_ratio_gb_write(HI_U8 i, HI_U8 data) {
    IOWR_8DIRECT(0x11c70+(i*sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_bayernr_chroma_ratio_gb_read(HI_U8 i) {
    return IORD_8DIRECT(0x11c70+(i*sizeof(HI_U8)));
}

#define HI_EXT_SYSTEM_BAYERNR_CRATIO_B_DATASIZE (8)
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_bayernr_chroma_ratio_b_write(HI_U8 i, HI_U8 data) {
    IOWR_8DIRECT(0x11c80+(i*sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_bayernr_chroma_ratio_b_read(HI_U8 i) {
    return IORD_8DIRECT(0x11c80+(i*sizeof(HI_U8)));
}*/

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_bayernr_auto_chroma_strength_r_write(HI_U8 i, HI_U8 data) {
    IOWR_8DIRECT(0x11c90+(i*sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_bayernr_auto_chroma_strength_r_read(HI_U8 i) {
    return IORD_8DIRECT(0x11c90+(i*sizeof(HI_U8)));
}

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_bayernr_auto_chroma_strength_gr_write(HI_U8 i, HI_U8 data) {
    IOWR_8DIRECT(0x11ca0+(i*sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_bayernr_auto_chroma_strength_gr_read(HI_U8 i) {
    return IORD_8DIRECT(0x11ca0+(i*sizeof(HI_U8)));
}

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_bayernr_auto_chroma_strength_gb_write(HI_U8 i, HI_U8 data) {
    IOWR_8DIRECT(0x11cb0+(i*sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_bayernr_auto_chroma_strength_gb_read(HI_U8 i) {
    return IORD_8DIRECT(0x11cb0+(i*sizeof(HI_U8)));
}


// args: data (8-bit)
static __inline HI_VOID hi_ext_system_bayernr_auto_chroma_strength_b_write(HI_U8 i, HI_U8 data) {
    IOWR_8DIRECT(0x11cc0+(i*sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_bayernr_auto_chroma_strength_b_read(HI_U8 i) {
    return IORD_8DIRECT(0x11cc0+(i*sizeof(HI_U8)));
}

//==============================================================//
//Frame stitch WDR: 0x11D00~0x11EFF
//==============================================================//
// ------------------------------------------------------------------------------ //
// Register: hi_ext_wdr_manual_mode_enable
// ------------------------------------------------------------------------------ //
// 0: auto; 1: manual; 
// ------------------------------------------------------------------------------ //
static __inline HI_VOID hi_ext_system_wdr_manual_mode_write(HI_U8 data) {
    IOWR_8DIRECT(0x11d00, data);
}
static __inline HI_U8 hi_ext_system_wdr_manual_mode_read(HI_VOID) {
    return IORD_8DIRECT(0x11d00);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_wdr_blc0
// ------------------------------------------------------------------------------ //
//wdr blc0
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
static __inline HI_VOID hi_ext_system_wdr_blc0_write(HI_U16 data) {
    IOWR_16DIRECT(0x11d02, data);
}
static __inline HI_U16 hi_ext_system_wdr_blc0_read(HI_VOID) {
    return IORD_16DIRECT(0x11d02);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_wdr_blc1
// ------------------------------------------------------------------------------ //
//wdr blc1
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
static __inline HI_VOID hi_ext_system_wdr_blc1_write(HI_U16 data) {
    IOWR_16DIRECT(0x11d04, data);
}
static __inline HI_U16 hi_ext_system_wdr_blc1_read(HI_VOID) {
    return IORD_16DIRECT(0x11d04);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_wdr_blc2
// ------------------------------------------------------------------------------ //
//wdr blc2
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
static __inline HI_VOID hi_ext_system_wdr_blc2_write(HI_U16 data) {
    IOWR_16DIRECT(0x11d06, data);
}
static __inline HI_U16 hi_ext_system_wdr_blc2_read(HI_VOID) {
    return IORD_16DIRECT(0x11d06);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_wdr_blc3
// ------------------------------------------------------------------------------ //
//wdr blc3
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
static __inline HI_VOID hi_ext_system_wdr_blc3_write(HI_U16 data) {
    IOWR_16DIRECT(0x11d08, data);
}
static __inline HI_U16 hi_ext_system_wdr_blc3_read(HI_VOID) {
    return IORD_16DIRECT(0x11d08);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_wdr_out_blc
// ------------------------------------------------------------------------------ //
//wdr out blc
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
static __inline HI_VOID hi_ext_system_wdr_out_blc_write(HI_U16 data) {
    IOWR_16DIRECT(0x11d0a, data);
}
static __inline HI_U16 hi_ext_system_wdr_out_blc_read(HI_VOID) {
    return IORD_16DIRECT(0x11d0a);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_wdr_procmdt_iso
// ------------------------------------------------------------------------------ //
//wdr procmdt iso
// ------------------------------------------------------------------------------ //
// args: data (32-bit)
static __inline HI_VOID hi_ext_system_wdr_procmdt_iso_write(HI_U32 data) {
    IOWR_32DIRECT(0x11d0c, data);
}
static __inline HI_U32 hi_ext_system_wdr_procmdt_iso_read(HI_VOID) {
    return IORD_32DIRECT(0x11d0c);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_wdr_lutmdtnoise
// ------------------------------------------------------------------------------ //
//wdr lutmdtnoise
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_wdr_lutmdtnoise_write(HI_U8 i, HI_U8 data) {
    IOWR_8DIRECT(0x11e00+(i*sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_wdr_lutmdtnoise_read(HI_U8 i) {
    return IORD_8DIRECT(0x11e00+(i*sizeof(HI_U8)));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_wdr_lutmdtlowth
// ------------------------------------------------------------------------------ //
//wdr lutmdtlowth
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_wdr_lutmdtlowth_write(HI_U8 i, HI_U8 data) {
    IOWR_8DIRECT(0x11e10+(i*sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_wdr_lutmdtlowth_read(HI_U8 i) {
    return IORD_8DIRECT(0x11e10+(i*sizeof(HI_U8)));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_wdr_lutmdthigth
// ------------------------------------------------------------------------------ //
//wdr lutmdthigth
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_wdr_lutmdthigth_write(HI_U8 i, HI_U8 data) {
    IOWR_8DIRECT(0x11e20+(i*sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_wdr_lutmdthigth_read(HI_U8 i) {
    return IORD_8DIRECT(0x11e20+(i*sizeof(HI_U8)));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_wdr_lutmdtclip
// ------------------------------------------------------------------------------ //
//wdr lutmdtclip
// ------------------------------------------------------------------------------ //
// args: data (16-bit)
static __inline HI_VOID hi_ext_system_wdr_lutmdtclip_write(HI_U8 i, HI_U16 data) {
    IOWR_16DIRECT(0x11e30+(i*sizeof(HI_U16)), data);
}
static __inline HI_U16 hi_ext_system_wdr_lutmdtclip_read(HI_U8 i) {
    return IORD_16DIRECT(0x11e30+(i*sizeof(HI_U16)));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_wdr_mdtmaxth
// ------------------------------------------------------------------------------ //
//wdr mdtmaxth
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_wdr_mdtmaxth_write(HI_U8 i, HI_U8 data) {
    IOWR_8DIRECT(0x11e50+(i*sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_wdr_mdtmaxth_read(HI_U8 i) {
    return IORD_8DIRECT(0x11e50+(i*sizeof(HI_U8)));
}

//Manual Lutmdtlowth

#define HI_EXT_SYSTEM_MANUAL_WDR_LUTMDTLOWTH_WRITE_DEFAULT (6)

static __inline HI_VOID hi_ext_system_manual_wdr_lutmdtlowth_write(HI_U8 data){
	IOWR_8DIRECT(0x11e60, data);
}
static __inline HI_U8 hi_ext_system_manual_wdr_lutmdtlowth_read(HI_VOID) {
    return (IORD_8DIRECT(0x11e60));
}

//Manual Lutmdthigth
#define HI_EXT_SYSTEM_MANUAL_WDR_LUTMDTHIGTH_WRITE_DEFAULT (16)

static __inline HI_VOID hi_ext_system_manual_wdr_lutmdthigth_write(HI_U8 data){
	IOWR_8DIRECT(0x11e70, data);
}
static __inline HI_U8 hi_ext_system_manual_wdr_lutmdthigth_read(HI_VOID) {
    return (IORD_8DIRECT(0x11e70));
}

//Manual Lutmdtclip
#define HI_EXT_SYSTEM_MANUAL_WDR_LUTMDTCLIP_WRITE_DEFAULT (250)

static __inline HI_VOID hi_ext_system_manual_wdr_lutmdtclip_write(HI_U16 data){
	IOWR_16DIRECT(0x11e80, data);
}
static __inline HI_U16 hi_ext_system_manual_wdr_lutmdtclip_read(HI_VOID) {
    return (IORD_16DIRECT(0x11e80));
}

//Manual Lutmaxth
#define HI_EXT_SYSTEM_MANUAL_WDR_MDTMAXTH_WRITE_DEFAULT (12)

static __inline HI_VOID hi_ext_system_manual_wdr_mdtmaxth_write(HI_U8 data){
	IOWR_8DIRECT(0x11ea0, data);
}
static __inline HI_U8 hi_ext_system_manual_wdr_mdtmaxth_read(HI_VOID) {
    return (IORD_8DIRECT(0x11ea0));
}
//Noise Ref

#define HI_EXT_SYSTEM_WDR_MDTNOSREF_WRITE_DEFAULT (1)

static __inline HI_VOID hi_ext_system_wdr_mdtnosref_write(HI_U8 data){
	IOWR_8DIRECT(0x11eb0, data);
}
static __inline HI_U8 hi_ext_system_wdr_mdtnosref_read(HI_VOID) {
    return (IORD_8DIRECT(0x11eb0));
}
//motion comp
#define HI_EXT_SYSTEM_WDR_MDTCOMP_WRITE_DEFAULT (1)

static __inline HI_VOID hi_ext_system_wdr_mdtcomp_write(HI_U8 data){
	IOWR_8DIRECT(0x11ec0, data);
}
static __inline HI_U8 hi_ext_system_wdr_mdtcomp_read(HI_VOID) {
    return (IORD_8DIRECT(0x11ec0));
}
//motion comp mode
#define HI_EXT_SYSTEM_WDR_MDTCOMP_MODE_WRITE_DEFAULT (0)

static __inline HI_VOID hi_ext_system_wdr_mdtcomp_mode_write(HI_U8 data){
	IOWR_8DIRECT(0x11ed0, data);
}
static __inline HI_U8 hi_ext_system_wdr_mdtcomp_mode_read(HI_VOID) {
    return (IORD_8DIRECT(0x11ed0));
}
//long thr
#define HI_EXT_SYSTEM_WDR_LONGTHR_WRITE_DEFAULT (0x2F00)

static __inline HI_VOID hi_ext_system_wdr_longthr_write(HI_U16 data){
	IOWR_16DIRECT(0x11ee0, data);
}
static __inline HI_U16 hi_ext_system_wdr_longthr_read(HI_VOID) {
    return (IORD_16DIRECT(0x11ee0));
}
//short thr
#define HI_EXT_SYSTEM_WDR_SHORTTHR_WRITE_DEFAULT (0x3F00)

static __inline HI_VOID hi_ext_system_wdr_shortthr_write(HI_U16 data){
	IOWR_16DIRECT(0x11f20, data);
}
static __inline HI_U16 hi_ext_system_wdr_shortthr_read(HI_VOID) {
    return (IORD_16DIRECT(0x11f20));
}

//short noise thr
#define HI_EXT_SYSTEM_WDR_SHORTNOSTHR_WRITE_DEFAULT (0x1)

static __inline HI_VOID hi_ext_system_wdr_snthr_write(HI_U16 data){
	IOWR_16DIRECT(0x11f40, data);
}
static __inline HI_U16 hi_ext_system_wdr_snthr_read(HI_VOID) {
    return (IORD_16DIRECT(0x11f40));
}

//chn switch 
#define HI_EXT_SYSTEM_CHNSWITCH_WRITE_DEFAULT (0x0)

static __inline HI_VOID hi_ext_system_chnswitch_write(HI_U16 data){
	IOWR_8DIRECT(0x11f44, data&(0x1));
}
static __inline HI_U16 hi_ext_system_chnswitch_read(HI_VOID) {
    return (IORD_8DIRECT(0x11f44));
}

//==============================================================//
//FLICKER: 0x11F00~0x11F00
//==============================================================//
// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_flicker_min_band_num
// ------------------------------------------------------------------------------ //
//flicker_min_band_num
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_flicker_min_band_num_write(HI_U8 data) {
    IOWR_8DIRECT(0x11F00, data);
}
static __inline HI_U8 hi_ext_system_flicker_min_band_num_read(HI_VOID) {
    return IORD_8DIRECT(0x11F00);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_flicker_min_valid_band_pcnt
// ------------------------------------------------------------------------------ //
//flicker_min_valid_band_pcnt
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_flicker_min_valid_band_pcnt_write(HI_U8 data) {
    IOWR_8DIRECT(0x11F01, data);
}
static __inline HI_U8 hi_ext_system_flicker_min_valid_band_pcnt_read(HI_VOID) {
    return IORD_8DIRECT(0x11F01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_flicker_wave_diff1
// ------------------------------------------------------------------------------ //
//flicker_wave_diff1
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_flicker_wave_diff1_write(HI_U8 data) {
    IOWR_8DIRECT(0x11F02, data);
}
static __inline HI_U8 hi_ext_system_flicker_wave_diff1_read(HI_VOID) {
    return IORD_8DIRECT(0x11F02);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_flicker_wave_diff2
// ------------------------------------------------------------------------------ //
//flicker_wave_diff2
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_flicker_wave_diff2_write(HI_U8 data) {
    IOWR_8DIRECT(0x11F03, data);
}
static __inline HI_U8 hi_ext_system_flicker_wave_diff2_read(HI_VOID) {
    return IORD_8DIRECT(0x11F03);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_flicker_period
// ------------------------------------------------------------------------------ //
//flicker_period
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_flicker_period_write(HI_U8 data) {
    IOWR_8DIRECT(0x11F04, data);
}
static __inline HI_U8 hi_ext_system_flicker_period_read(HI_VOID) {
    return IORD_8DIRECT(0x11F04);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_flicker_gr_cnt
// ------------------------------------------------------------------------------ //
//flicker_gr_cnt
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_flicker_gr_cnt_write(HI_U8 data) {
    IOWR_8DIRECT(0x11F05, data);
}
static __inline HI_U8 hi_ext_system_flicker_gr_cnt_read(HI_VOID) {
    return IORD_8DIRECT(0x11F05);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_flicker_gb_cnt
// ------------------------------------------------------------------------------ //
//flicker_gb_cnt
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_flicker_gb_cnt_write(HI_U8 data) {
    IOWR_8DIRECT(0x11F06, data);
}
static __inline HI_U8 hi_ext_system_flicker_gb_cnt_read(HI_VOID) {
    return IORD_8DIRECT(0x11F06);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_flicker_result
// ------------------------------------------------------------------------------ //
//flicker_result
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_flicker_result_write(HI_U8 data) {
    IOWR_8DIRECT(0x11F07, data);
}
static __inline HI_U8 hi_ext_system_flicker_result_read(HI_VOID) {
    return IORD_8DIRECT(0x11F07);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_freq_result
// ------------------------------------------------------------------------------ //
//freq_result
// ------------------------------------------------------------------------------ //
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_freq_result_write(HI_U8 data) {
    IOWR_8DIRECT(0x11F08, data);
}
static __inline HI_U8 hi_ext_system_freq_result_read(HI_VOID) {
    return IORD_8DIRECT(0x11F08);
}


//==============================================================//
//lSC: 0x12000~0x13FFF
//==============================================================//

//args: index (0 ~ 288), data (32-bit)

//shading gain 0x12000-0x12480
static __inline HI_VOID hi_ext_system_isp_mesh_shading_r_gain_write(HI_U16 index, HI_U32 data)
{
   HI_U32 addr = 0x12000 + (index << 2);
   IOWR_32DIRECT(addr,data);
}

static __inline HI_U32 hi_ext_system_isp_mesh_shading_r_gain_read(HI_U16 index)
{
	HI_U32 addr = 0x12000 + (index << 2);
	return	IORD_32DIRECT(addr);
}

//shading gain 0x12484-0x12904
static __inline HI_VOID hi_ext_system_isp_mesh_shading_gr_gain_write(HI_U16 index, HI_U32 data)
{
   HI_U32 addr = 0x12484 + (index << 2);
   IOWR_32DIRECT(addr,data);
}

static __inline HI_U32 hi_ext_system_isp_mesh_shading_gr_gain_read(HI_U16 index)
{
	HI_U32 addr = 0x12484 + (index << 2);
	return	IORD_32DIRECT(addr);
}

//shading gain 0x12908-0x12D88
static __inline HI_VOID hi_ext_system_isp_mesh_shading_gb_gain_write(HI_U16 index, HI_U32 data)
{
   HI_U32 addr = 0x12908 + (index << 2);
   IOWR_32DIRECT(addr,data);
}

static __inline HI_U32 hi_ext_system_isp_mesh_shading_gb_gain_read(HI_U16 index)
{
	HI_U32 addr = 0x12908 + (index << 2);
	return	IORD_32DIRECT(addr);
}

//shading gain 0x12D8C-0x1320C
static __inline HI_VOID hi_ext_system_isp_mesh_shading_b_gain_write(HI_U16 index, HI_U32 data)
{
   HI_U32 addr = 0x12D8C + (index << 2);
   IOWR_32DIRECT(addr,data);
}

static __inline HI_U32 hi_ext_system_isp_mesh_shading_b_gain_read(HI_U16 index)
{
	HI_U32 addr = 0x12D8C + (index << 2);
	return	IORD_32DIRECT(addr);
}


#define HI_EXT_SYSTEM_ISP_MESH_SHADING_UPDATA_DEFAULT (0)
#define HI_EXT_SYSTEM_ISP_MESH_SHADING_UPDATA_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_isp_mesh_shading_updata_write(HI_U8 data) {
    IOWR_8DIRECT(0x13210, data);
}
static __inline HI_U8 hi_ext_system_isp_mesh_shading_updata_read(HI_VOID) {
    return IORD_8DIRECT(0x13210);
}

#define HI_EXT_SYSTEM_ISP_MESH_SHADING_MANU_MODE_DEFAULT (0)
#define HI_EXT_SYSTEM_ISP_MESH_SHADING_MANU_MODE_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_isp_mesh_shading_manu_mode_write(HI_U8 data) {
    IOWR_8DIRECT(0x13211, data);
}
static __inline HI_U8 hi_ext_system_isp_mesh_shading_manu_mode_read(HI_VOID) {
    return IORD_8DIRECT(0x13211);
}


static __inline HI_VOID hi_ext_system_isp_mesh_shading_enable_write( HI_U8 data){
    IOWR_8DIRECT(0x13214 , data&0x1);
}

static __inline HI_U16 hi_ext_system_isp_mesh_shading_enable_read(HI_VOID){
    return (IORD_8DIRECT(0x13214)&0x1);
}


//shading gain 0x13220 - 0x136a0 //Do we need such big space?
static __inline HI_VOID hi_ext_system_isp_mesh_shading_noise_control_write(HI_U16 index, HI_U32 data)
{
   HI_U32 addr = 0x13220 + (index << 2);  
   IOWR_32DIRECT(addr, data);
}

static __inline HI_U32 hi_ext_system_isp_mesh_shading_noise_control_read(HI_U16 index)
{
    HI_U32 addr = 0x13220 + (index << 2);
    return IORD_32DIRECT(addr);
}

//xgrid 0x13700 - 0x1371c 
static __inline HI_VOID hi_ext_system_isp_mesh_shading_xgrid_write(HI_U16 index, HI_U32 data)
{
    HI_U32 addr = 0x13700 + (index << 2);  
    IOWR_32DIRECT(addr, data);

}

static __inline HI_U32 hi_ext_system_isp_mesh_shading_xgrid_read(HI_U16 index)
{
    HI_U32 addr = 0x13700 + (index << 2);
    return IORD_32DIRECT(addr);
}


//ygrid 0x13720 - 0x1373c
static __inline HI_VOID hi_ext_system_isp_mesh_shading_ygrid_write(HI_U16 index, HI_U32 data)
{
    HI_U32 addr = 0x13720 + (index << 2);  
    IOWR_32DIRECT(addr, data);

}

static __inline HI_U32 hi_ext_system_isp_mesh_shading_ygrid_read(HI_U16 index)
{
    HI_U32 addr = 0x13720 + (index << 2);
    return IORD_32DIRECT(addr);
}

//meshstrength 0x13740 - 0x13748
#define HI_EXT_SYSTEM_ISP_MESH_SHADING_MESHSTRENGTH_DEFAULT (4095)

static __inline HI_VOID hi_ext_system_isp_mesh_shading_meshstrength_write(HI_U16 data)
{
    IOWR_16DIRECT(0x13740, data);

}

static __inline HI_U32 hi_ext_system_isp_mesh_shading_meshstrength_read(HI_VOID)
{
   
    return IORD_16DIRECT(0x13740);
}

//==============================================================//
//YUV SHARPEN: 0x14000~0x15FFF
//==============================================================//

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpening_core
// ------------------------------------------------------------------------------ //
// sharpening core
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_SHARPEN_MANU_MODE_DEFAULT (0)
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_sharpen_manu_mode_write(HI_U8 data){
    IOWR_8DIRECT(0x14000, data);
}
static __inline HI_U8 hi_ext_system_sharpen_manu_mode_read(HI_VOID) {
    return (IORD_8DIRECT(0x14000));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpening_core
// ------------------------------------------------------------------------------ //
// sharpening core
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpening_core
// ------------------------------------------------------------------------------ //
// sharpening core
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_SHARPEN_TEXTURENOISETHD_DEFAULT (20)
// args: data (8-bit)

static __inline HI_VOID hi_ext_system_manual_sharpen_TextureNoiseThd_write(HI_U8 data){
	IOWR_8DIRECT(0x14002, data);
}
static __inline HI_U8 hi_ext_system_manual_sharpen_TextureNoiseThd_read(HI_VOID) {
    return (IORD_8DIRECT(0x14002));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_EdgeNoiseThd
// ------------------------------------------------------------------------------ //
// sharpening edge noise threshold
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_SHARPEN_EDGETHD_DEFAULT               (80)

// args: data (8-bit)

static __inline HI_VOID hi_ext_system_manual_sharpen_EdgeThd_write(HI_U8 data){
	IOWR_8DIRECT(0x14003, data);
}
static __inline HI_U8 hi_ext_system_manual_sharpen_EdgeThd_read(HI_VOID) {
    return (IORD_8DIRECT(0x14003));
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_EnLumaCtrl
// ------------------------------------------------------------------------------ //
// sharpening luma enable
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_SHARPEN_ENLUMACTRL_DEFAULT (1)
// args: data (8-bit)

static __inline HI_VOID hi_ext_system_manual_sharpen_EnLumaCtrl_write(HI_U8 data){
	IOWR_8DIRECT(0x14005, data);
}
static __inline HI_U8 hi_ext_system_manual_sharpen_EnLumaCtrl_read(HI_VOID) {
    return (IORD_8DIRECT(0x14005));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_lumNoSt
// ------------------------------------------------------------------------------ //
// sharpening luma no st
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_SHARPEN_LUMNOST_DEFAULT (0)
// args: data (8-bit)

static __inline HI_VOID hi_ext_system_manual_sharpen_lumNoSt_write(HI_U8 data){
	IOWR_8DIRECT(0x14006, data);
}
static __inline HI_U8 hi_ext_system_manual_sharpen_lumNoSt_read(HI_VOID) {
    return (IORD_8DIRECT(0x14006));
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_TextureSt
// ------------------------------------------------------------------------------ //
// sharpening texture
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_SHARPEN_TEXTUREST_DEFAULT (30)
// args: data (8-bit)

static __inline HI_VOID hi_ext_system_manual_sharpen_TextureSt_write(HI_U8 data){
	IOWR_8DIRECT(0x14008, data);
}
static __inline HI_U8 hi_ext_system_manual_sharpen_TextureSt_read(HI_VOID) {
    return (IORD_8DIRECT(0x14008));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_EdgeSt
// ------------------------------------------------------------------------------ //
// sharpening edge st
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_SHARPEN_SHARPENEDGE_DEFAULT                (60)
// args: data (8-bit)

static __inline HI_VOID hi_ext_system_manual_sharpen_SharpenEdge_write(HI_U8 data){
	IOWR_8DIRECT(0x14009, data);
}
static __inline HI_U8 hi_ext_system_manual_sharpen_SharpenEdge_read(HI_VOID) {
    return (IORD_8DIRECT(0x14009));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_overshootAmt
// ------------------------------------------------------------------------------ //
// sharpening overshoot Amt
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_SHARPEN_OVERSHOOT_DEFAULT					(150)

// args: data (8-bit)

static __inline HI_VOID hi_ext_system_manual_sharpen_overshootAmt_write(HI_U8 data){
    IOWR_8DIRECT(0x1400a , data);
}
static __inline HI_U8 hi_ext_system_manual_sharpen_overshootAmt_read(HI_VOID) {
    return IORD_8DIRECT(0x1400a);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_undershootAmt_write
// ------------------------------------------------------------------------------ //
// sharpening undershoot Amt
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_SHARPEN_UNDERSHOOT_DEFAULT            (200)
// args: data (8-bit)

static __inline HI_VOID hi_ext_system_manual_sharpen_undershootAmt_write(HI_U8 data){
    IOWR_8DIRECT(0x1400b , data);
}
static __inline HI_U8 hi_ext_system_manual_sharpen_undershootAmt_read(HI_VOID) {
    return IORD_8DIRECT(0x1400b);
}



// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_EnSkinCtrl
// ------------------------------------------------------------------------------ //
// sharpening EnSkinCtrl
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_SHARPEN_ENSKINCTRL_DEFAULT (1)
// args: data (8-bit)

static __inline HI_VOID hi_ext_system_manual_sharpen_EnSkinCtrl_write(HI_U8 data){
	IOWR_8DIRECT(0x1400d, data);
}
static __inline HI_U8 hi_ext_system_manual_sharpen_EnSkinCtrl_read(HI_VOID) {
    return (IORD_8DIRECT(0x1400d));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_UDHfGain_write
// ------------------------------------------------------------------------------ //
// sharpening UDHfGain
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_SHARPEN_UDHfGAIN_DEFAULT (16)
// args: data (16-bit)

static __inline HI_VOID hi_ext_system_manual_sharpen_UDHfGain_write(HI_U16 data){
	IOWR_16DIRECT(0x1400e, data);
}
static __inline HI_U16 hi_ext_system_manual_sharpen_UDHfGain_read(HI_VOID) {
    return (IORD_16DIRECT(0x1400e));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_EnShtMax_write
// ------------------------------------------------------------------------------ //
// sharpening EnShtMax
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_SHARPEN_ENSHTMAX_DEFAULT (1)
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_manual_sharpen_EnShtMax_write(HI_U8 data){
	IOWR_8DIRECT(0x14010, data);
}
static __inline HI_U8 hi_ext_system_manual_sharpen_EnShtMax_read(HI_VOID) {
    return (IORD_8DIRECT(0x14010));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_EnShtMax_write
// ------------------------------------------------------------------------------ //
// sharpening EnShtMax
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_SHARPEN_SHTMAXCHG_DEFAULT (100)
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_manual_sharpen_ShtMaxChg_write(HI_U8 data){
	IOWR_8DIRECT(0x14011, data);
}
static __inline HI_U8 hi_ext_system_manual_sharpen_ShtMaxChg_read(HI_VOID) {
    return (IORD_8DIRECT(0x14011));
}



// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_edgsm_en
// ------------------------------------------------------------------------------ //
// sharpening edgsm_en
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_SHARPEN_EDGE_FILT_STR_DEFAULT (0)
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_manual_sharpen_EdgeFiltStr_write(HI_U8 data){
	IOWR_8DIRECT(0x14013, (data&0x7F));
}
static __inline HI_U8 hi_ext_system_manual_sharpen_EdgeFiltStr_read(HI_VOID) {
    return ((IORD_8DIRECT(0x14013))&0x7F);
}



#if 0
// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_SkinVarTh1
// ------------------------------------------------------------------------------ //
// sharpening SkinVarTh1
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_SHARPEN_SKINVARTH1_DEFAULT (0x0)
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_manual_sharpen_SkinVarTh1_write(HI_U8 data){
	IOWR_8DIRECT(0x14014, data);
}
static __inline HI_U8 hi_ext_system_manual_sharpen_SkinVarTh1_read(HI_VOID) {
    return (IORD_8DIRECT(0x14014));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_SkinVarTh2
// ------------------------------------------------------------------------------ //
// sharpening SkinVarTh2
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_SHARPEN_SKINVARTH2_DEFAULT (150)
// args: data (8-bit)

static __inline HI_VOID hi_ext_system_manual_sharpen_SkinVarTh2_write(HI_U8 data){
	IOWR_8DIRECT(0x14015, data);
}
static __inline HI_U8 hi_ext_system_manual_sharpen_SkinVarTh2_read(HI_VOID) {
    return (IORD_8DIRECT(0x14015));
}
#endif
// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_SkinVarWgt1
// ------------------------------------------------------------------------------ //
// sharpening SkinVarWgt1
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_SHARPEN_SKINGAIN_DEFAULT (127)
// args: data (8-bit)

static __inline HI_VOID hi_ext_system_manual_sharpen_SkinGain_write(HI_U8 data){
	IOWR_8DIRECT(0x14016, data);
}
static __inline HI_U8 hi_ext_system_manual_sharpen_SkinGain_read(HI_VOID) {
    return (IORD_8DIRECT(0x14016));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_SkinVarWgt2
// ------------------------------------------------------------------------------ //
// sharpening SkinVarWgt2
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_SHARPEN_SKINVARWGT2_DEFAULT (32)
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_manual_sharpen_SkinVarWgt2_write(HI_U8 data){
	IOWR_8DIRECT(0x14017, data);
}
static __inline HI_U8 hi_ext_system_manual_sharpen_SkinVarWgt2_read(HI_VOID) {
    return (IORD_8DIRECT(0x14017));
}
#if 0
// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_Log2SkinLen
// ------------------------------------------------------------------------------ //
// sharpening Log2SkinLen
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_SHARPEN_LOG2SKINLEN_DEFAULT (0x0)
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_manual_sharpen_Log2SkinLen_write(HI_U8 data){
	IOWR_8DIRECT(0x14018, data);
}
static __inline HI_U8 hi_ext_system_manual_sharpen_Log2SkinLen_read(HI_VOID) {
    return (IORD_8DIRECT(0x14018));
}
#endif
// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_diffmul
// ------------------------------------------------------------------------------ //
// sharpening diffmul
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_diffthd
// ------------------------------------------------------------------------------ //
// sharpening diffthd
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_SHARPEN_NOISE_LUMA_CTRL_DEFAULT (0x0)
// args: data (8-bit)

static __inline HI_VOID hi_ext_system_manual_sharpen_NoiseLumaCtrl_write(HI_U8 data){
	IOWR_8DIRECT(0x1401a, (data&0x1F));
}
static __inline HI_U8 hi_ext_system_manual_sharpen_NoiseLumaCtrl_read(HI_VOID) {
    return ((IORD_8DIRECT(0x1401a))&0x1F);
}



// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_EnShtSup
// ------------------------------------------------------------------------------ //
// sharpening EnShtSup
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_SHARPEN_SHTSUPEN_DEFAULT (0x1)
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_manual_sharpen_EnShtSup_write(HI_U8 data){
	IOWR_8DIRECT(0x1401d, data);
}

static __inline HI_U8 hi_ext_system_manual_sharpen_EnShtSup_read(HI_VOID){
	return IORD_8DIRECT(0x1401d);
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_OvShtVarMax
// ------------------------------------------------------------------------------ //
// sharpening OvShtVarMax
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_SHARPEN_JAGCTRL_DEFAULT (57)
// args: data (8-bit) 	

static __inline HI_VOID hi_ext_system_manual_sharpen_JagCtrl_write(HI_U8 data){
	IOWR_8DIRECT(0x1401f, data);
}    

static __inline HI_U8 hi_ext_system_manual_sharpen_JagCtrl_read(HI_VOID){
	return IORD_8DIRECT(0x1401f);
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_UnShtVarMin
// ------------------------------------------------------------------------------ //
// sharpening UnShtVarMin
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_SHARPEN_UNSHTVARMIN_DEFAULT (0)
// args: data (8-bit) 	  			
static __inline HI_VOID hi_ext_system_manual_sharpen_UnShtVarMin_write(HI_U8 data){
	IOWR_8DIRECT(0x14021, data);
}  

static __inline HI_U8 hi_ext_system_manual_sharpen_UnShtVarMin_read(HI_VOID){
	return IORD_8DIRECT(0x14021);
}  

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_UnShtVarMax
// ------------------------------------------------------------------------------ //
// sharpening UnShtVarMax
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_SHARPEN_UNSHTVARMAX_DEFAULT (23)
// args: data (8-bit) 	    
static __inline HI_VOID hi_ext_system_manual_sharpen_UnShtVarMax_write(HI_U8 data){
	IOWR_8DIRECT(0x14022, data);
}    

static __inline HI_U8 hi_ext_system_manual_sharpen_UnShtVarMax_read(HI_VOID){
	return IORD_8DIRECT(0x14022);
}  

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_UnShtWgtMin
// ------------------------------------------------------------------------------ //
// sharpening UnShtWgtMin
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_SHARPEN_UNSHTWGTMIN_DEFAULT (60)
// args: data (8-bit) 	   
static __inline HI_VOID hi_ext_system_manual_sharpen_UnShtWgtMin_write(HI_U8 data){
	IOWR_8DIRECT(0x14023, data);
}  

static __inline HI_U8 hi_ext_system_manual_sharpen_UnShtWgtMin_read(HI_VOID){
	return IORD_8DIRECT(0x14023);
}  

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_UnShtDiffMax
// ------------------------------------------------------------------------------ //
// sharpening UnShtDiffMax
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_SHARPEN_UNSHTDIFFMAX_DEFAULT (25)
// args: data (8-bit) 	
static __inline HI_VOID hi_ext_system_manual_sharpen_UnShtDiffMax_write(HI_U8 data){
	IOWR_8DIRECT(0x14024, data);
}    

static __inline HI_U8 hi_ext_system_manual_sharpen_UnShtDiffMax_read(HI_VOID){
	return IORD_8DIRECT(0x14024);
}  

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_UnShtDiffMin
// ------------------------------------------------------------------------------ //
// sharpening UnShtDiffMin
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_SHARPEN_UNSHTDIFFMIN_DEFAULT (15)
// args: data (8-bit) 
static __inline HI_VOID hi_ext_system_manual_sharpen_UnShtDiffMin_write(HI_U8 data){
	IOWR_8DIRECT(0x14025, data);
}  

static __inline HI_U8 hi_ext_system_manual_sharpen_UnShtDiffMin_read(HI_VOID){
	return IORD_8DIRECT(0x14025);
}  

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_UnShtDiffWgtMin
// ------------------------------------------------------------------------------ //
// sharpening UnShtDiffMin
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_SHARPEN_UNSHTDIFFWGTMIN_DEFAULT (50)
// args: data (8-bit)  
static __inline HI_VOID hi_ext_system_manual_sharpen_UnShtDiffWgtMin_write(HI_U8 data){
	IOWR_8DIRECT(0x14026, data);
}  

static __inline HI_U8 hi_ext_system_manual_sharpen_UnShtDiffWgtMin_read(HI_VOID){
	return IORD_8DIRECT(0x14026);
}  

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_OvShtDiffMax
// ------------------------------------------------------------------------------ //
// sharpening OvShtDiffMax
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_SHARPEN_OVSHTDIFFMAX_DEFAULT (25)
// args: data (8-bit)  
static __inline HI_VOID hi_ext_system_manual_sharpen_OvShtDiffMax_write(HI_U8 data){
	IOWR_8DIRECT(0x14027, data);
} 

static __inline HI_U8 hi_ext_system_manual_sharpen_OvShtDiffMax_read(HI_VOID){
	return IORD_8DIRECT(0x14027);
} 

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_OvShtDiffMin
// ------------------------------------------------------------------------------ //
// sharpening OvShtDiffMax
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_SHARPEN_OVSHTDIFFMIN_DEFAULT (15)
// args: data (8-bit)   
static __inline HI_VOID hi_ext_system_manual_sharpen_OvShtDiffMin_write(HI_U8 data){
	IOWR_8DIRECT(0x14028, data);
}  

static __inline HI_U8 hi_ext_system_manual_sharpen_OvShtDiffMin_read(HI_VOID){
	return IORD_8DIRECT(0x14028);
} 

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_OvShtDiffWgtMin
// ------------------------------------------------------------------------------ //
// sharpening OvShtDiffMax
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_SHARPEN_OVSHTDIFFWGTMIN_DEFAULT (35)
// args: data (8-bit)   
static __inline HI_VOID hi_ext_system_manual_sharpen_OvShtDiffWgtMin_write(HI_U8 data){
	IOWR_8DIRECT(0x14029, data);
}  

static __inline HI_U8 hi_ext_system_manual_sharpen_OvShtDiffWgtMin_read(HI_VOID){
	return IORD_8DIRECT(0x14029);
} 


// args: data (8-bit)  
static __inline HI_VOID hi_ext_system_actual_sharpen_SkinGain_write(HI_U8 data){
	IOWR_8DIRECT(0x1402a, data);
}  

static __inline HI_U8 hi_ext_system_actual_sharpen_SkinGain_read(HI_VOID){
	return IORD_8DIRECT(0x1402a);
} 


// args: data (8-bit) 
static __inline HI_VOID hi_ext_system_actual_sharpen_RGain_write(HI_U8 data){
	IOWR_8DIRECT(0x1402b, data&0x1F);
}  

static __inline HI_U8 hi_ext_system_actual_sharpen_RGain_read(HI_VOID){
	return (IORD_8DIRECT(0x1402b)&0x1F);
} 


static __inline HI_VOID hi_ext_system_actual_sharpen_BGain_write(HI_U8 data){
	IOWR_8DIRECT(0x14030, data&0x1F);
}  

static __inline HI_U8 hi_ext_system_actual_sharpen_BGain_read(HI_VOID){
	return (IORD_8DIRECT(0x14030)&0x1F);
} 

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_VarJagThd2
// ------------------------------------------------------------------------------ //
// sharpening VarJagThd2
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_SHARPEN_VARJAGTHD2_DEFAULT (520)
// args: data (8-bit) 

static __inline HI_VOID hi_ext_system_manual_sharpen_VarJagThd2_write(HI_U16 data){
	IOWR_16DIRECT(0x14032, data);
}  

static __inline HI_U16 hi_ext_system_manual_sharpen_VarJagThd2_read(HI_VOID){
	return IORD_16DIRECT(0x14032);
} 

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_BGain
// ------------------------------------------------------------------------------ //
// sharpening LineThd1
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_SHARPEN_RGAIN_DEFAULT (16)
// args: data (8-bit) 
static __inline HI_VOID hi_ext_system_manual_sharpen_RGain_write(HI_U8 data){
	IOWR_8DIRECT(0x14034, data&0x1F);
}  

static __inline HI_U8 hi_ext_system_manual_sharpen_RGain_read(HI_VOID){
	return (IORD_8DIRECT(0x14034)&0x1F);
} 

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_BGain
// ------------------------------------------------------------------------------ //
// sharpening LineThd2
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_MANUAL_SHARPEN_BGAIN_DEFAULT (20)
// args: data (8-bit) 
static __inline HI_VOID hi_ext_system_manual_sharpen_BGain_write(HI_U8 data){
	IOWR_8DIRECT(0x14035, data&0x1F);
}  

static __inline HI_U8 hi_ext_system_manual_sharpen_BGain_read(HI_VOID){
	return (IORD_8DIRECT(0x14035)&0x1F);
} 


#define HI_EXT_SYSTEM_MANUAL_SHARPEN_SHARPEND_DEFAULT					(60)

static __inline HI_VOID hi_ext_system_manual_sharpen_SharpenD_write(HI_U8 data){
	IOWR_8DIRECT(0x14037, data);
}  

static __inline HI_U8 hi_ext_system_manual_sharpen_SharpenD_read(HI_VOID){
	return IORD_8DIRECT(0x14037);
} 

#define HI_EXT_SYSTEM_MANUAL_SHARPEN_DETAILCTRL_DEFAULT        (128)

static __inline HI_VOID hi_ext_system_manual_sharpen_DetailCtrl_write(HI_U8 data){
	IOWR_8DIRECT(0x14038, data);
}  

static __inline HI_U8 hi_ext_system_manual_sharpen_DetailCtrl_read(HI_VOID){
	return IORD_8DIRECT(0x14038);
} 


#define HI_EXT_SYSTEM_MANUAL_SHARPEN_SHOOTSUPST_DEFAULT            (1)

static __inline HI_VOID hi_ext_system_manual_sharpen_shootSupSt_write(HI_U8 data){
	IOWR_8DIRECT(0x14039, data);
}  

static __inline HI_U8 hi_ext_system_manual_sharpen_shootSupSt_read(HI_VOID){
	return IORD_8DIRECT(0x14039);
} 

#define HI_EXT_SYSTEM_MANUAL_SHARPEN_FLATNOISESUPST_DEFAULT        (1)

static __inline HI_VOID hi_ext_system_manual_sharpen_FlatNoiseSupSt_write(HI_U8 data){
	IOWR_8DIRECT(0x1403a, data);
}  

static __inline HI_U8 hi_ext_system_manual_sharpen_FlatNoiseSupSt_read(HI_VOID){
	return IORD_8DIRECT(0x1403a);
} 

#define HI_EXT_SYSTEM_MANUAL_SHARPEN_TEXTURETHD_DEFAULT         (0)

static __inline HI_VOID hi_ext_system_manual_sharpen_TextureThd_write(HI_U8 data){
	IOWR_8DIRECT(0x1403b, data);
}  

static __inline HI_U8 hi_ext_system_manual_sharpen_TextureThd_read(HI_VOID){
	return IORD_8DIRECT(0x1403b);
} 

#define HI_EXT_SYSTEM_MANUAL_SHARPEN_SHARPENUD_DEFAULT					(16)
// args: data (16-bit) 
static __inline HI_VOID hi_ext_system_manual_sharpen_SharpenUd_write(HI_U16 data){
	IOWR_16DIRECT(0x14048, data&0x3FF);
}  
static __inline HI_U16 hi_ext_system_manual_sharpen_SharpenUd_read(HI_VOID){
	return (IORD_16DIRECT(0x14048)&0x3FF);
} 



#define HI_EXT_SYSTEM_ACTUAL_SHARPEN_SHARPEND_DEFAULT					(60)

static __inline HI_VOID hi_ext_system_actual_sharpen_SharpenD_write(HI_U8 data){
	IOWR_8DIRECT(0x14050, data);
}  

static __inline HI_U8 hi_ext_system_actual_sharpen_SharpenD_read(HI_VOID){
	return IORD_8DIRECT(0x14050);
} 

#define HI_EXT_SYSTEM_ACTUAL_SHARPEN_DETAILCTRL_DEFAULT        (128)

static __inline HI_VOID hi_ext_system_actual_sharpen_DetailCtrl_write(HI_U8 data){
	IOWR_8DIRECT(0x14051, data);
}  

static __inline HI_U8 hi_ext_system_actual_sharpen_DetailCtrl_read(HI_VOID){
	return IORD_8DIRECT(0x14051);
} 


#define HI_EXT_SYSTEM_ACTUAL_SHARPEN_SHOOTSUPST_DEFAULT            (1)

static __inline HI_VOID hi_ext_system_actual_sharpen_shootSupSt_write(HI_U8 data){
	IOWR_8DIRECT(0x14052, data);
}  

static __inline HI_U8 hi_ext_system_actual_sharpen_shootSupSt_read(HI_VOID){
	return IORD_8DIRECT(0x14052);
} 


#define HI_EXT_SYSTEM_ACTUAL_SHARPEN_TEXTURETHD_DEFAULT         (0)

static __inline HI_VOID hi_ext_system_actual_sharpen_TextureThd_write(HI_U8 data){
	IOWR_8DIRECT(0x14054, data);
}  

static __inline HI_U8 hi_ext_system_actual_sharpen_TextureThd_read(HI_VOID){
	return IORD_8DIRECT(0x14054);
} 

#define HI_EXT_SYSTEM_ACTUAL_SHARPEN_SHARPENUD_DEFAULT					(16)
// args: data (16-bit) 
static __inline HI_VOID hi_ext_system_actual_sharpen_SharpenUd_write(HI_U16 data){
	IOWR_16DIRECT(0x14056, data);
}  
static __inline HI_U16 hi_ext_system_actual_sharpen_SharpenUd_read(HI_VOID){
	return IORD_16DIRECT(0x14056);
} 


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_EdgeSt
// ------------------------------------------------------------------------------ //
// sharpening edge st
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_ACTUAL_SHARPEN_SHARPENEDGE_DEFAULT                (60)
// args: data (8-bit)

static __inline HI_VOID hi_ext_system_actual_sharpen_SharpenEdge_write(HI_U8 data){
	IOWR_8DIRECT(0x14058, data);
}
static __inline HI_U8 hi_ext_system_actual_sharpen_SharpenEdge_read(HI_VOID) {
    return (IORD_8DIRECT(0x14058));
}



// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_EdgeNoiseThd
// ------------------------------------------------------------------------------ //
// sharpening edge noise threshold
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_ACTUAL_SHARPEN_EDGETHD_DEFAULT               (80)

// args: data (8-bit)

static __inline HI_VOID hi_ext_system_actual_sharpen_EdgeThd_write(HI_U8 data){
	IOWR_8DIRECT(0x14060, data);
}
static __inline HI_U8 hi_ext_system_actual_sharpen_EdgeThd_read(HI_VOID) {
    return (IORD_8DIRECT(0x14060));
}



// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_overshootAmt
// ------------------------------------------------------------------------------ //
// sharpening overshoot Amt
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_ACTUAL_SHARPEN_OVERSHOOT_DEFAULT					(150)

// args: data (8-bit)

static __inline HI_VOID hi_ext_system_actual_sharpen_overshootAmt_write(HI_U8 data){
    IOWR_8DIRECT(0x14062 , data);
}
static __inline HI_U8 hi_ext_system_actual_sharpen_overshootAmt_read(HI_VOID) {
    return IORD_8DIRECT(0x14062);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_undershootAmt_write
// ------------------------------------------------------------------------------ //
// sharpening undershoot Amt
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_ACTUAL_SHARPEN_UNDERSHOOT_DEFAULT            (200)
// args: data (8-bit)

static __inline HI_VOID hi_ext_system_actual_sharpen_undershootAmt_write(HI_U8 data){
    IOWR_8DIRECT(0x14064 , data);
}
static __inline HI_U8 hi_ext_system_actual_sharpen_undershootAmt_read(HI_VOID) {
    return IORD_8DIRECT(0x14064);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpen_overshootAmt
// ------------------------------------------------------------------------------ //
// sharpening overshootAmt table
// ------------------------------------------------------------------------------ //
// args: data (8-bit)

/*
overshootAmt[16]
*/
static __inline HI_VOID hi_ext_system_sharpen_overshootAmt_write(HI_U8 u8Index, HI_U8 data){
    IOWR_8DIRECT((0x15000 + u8Index*sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_sharpen_overshootAmt_read(HI_U8 u8Index) {
    return (IORD_8DIRECT((0x15000 + u8Index*sizeof(HI_U8))));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpen_undershootAmt
// ------------------------------------------------------------------------------ //
// sharpening undershootAmt table
// ------------------------------------------------------------------------------ //
// args: data (8-bit)

/*
undershootAmt[16]
*/
static __inline HI_VOID hi_ext_system_sharpen_undershootAmt_write(HI_U8 u8Index, HI_U8 data){
    IOWR_8DIRECT((0x15010 + u8Index*sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_sharpen_undershootAmt_read(HI_U8 u8Index) {
    return (IORD_8DIRECT((0x15010 + u8Index*sizeof(HI_U8))));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpen_LowLumaShoot
// ------------------------------------------------------------------------------ //
// sharpening LowLumaShoot table
// ------------------------------------------------------------------------------ //
// args: data (8-bit)

// EnLowLumaShoot[16] (1-bit) 0x12054

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpen_TextureNoiseThd
// ------------------------------------------------------------------------------ //
// sharpening TextureNoiseThd table
// ------------------------------------------------------------------------------ //
// args: data (8-bit)

/*
TextureNoiseThd[16] 0x12064
*/
static __inline HI_VOID hi_ext_system_sharpen_TextureNoiseThd_write(HI_U8 u8Index, HI_U8 data){
    IOWR_8DIRECT((0x15030 + u8Index*sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_sharpen_TextureNoiseThd_read(HI_U8 u8Index) {
    return (IORD_8DIRECT((0x15030 + u8Index*sizeof(HI_U8))));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpen_EdgeNoiseThd
// ------------------------------------------------------------------------------ //
// sharpening EdgeNoiseThd table
// ------------------------------------------------------------------------------ //
// args: data (8-bit)

/*
EdgeNoiseThd[16] 0x12074
*/
static __inline HI_VOID hi_ext_system_sharpen_EdgeThd_write(HI_U8 u8Index, HI_U8 data){
    IOWR_8DIRECT((0x15040 + u8Index*sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_sharpen_EdgeThd_read(HI_U8 u8Index) {
    return (IORD_8DIRECT((0x15040 + u8Index*sizeof(HI_U8))));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpen_TextureSt
// ------------------------------------------------------------------------------ //
// sharpening TextureSt table
// ------------------------------------------------------------------------------ //
// args: data (8-bit)

static __inline HI_VOID hi_ext_system_sharpen_TextureSt_write(HI_U8 Index, HI_U8 data){
	IOWR_8DIRECT(0x15050 + Index, data);
}
static __inline HI_U8 hi_ext_system_sharpen_TextureSt_read(HI_U8 Index) {
    return (IORD_8DIRECT(0x15050 + Index));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpen_EdgeSt
// ------------------------------------------------------------------------------ //
// sharpening EdgeSt table
// ------------------------------------------------------------------------------ //
// args: data (8-bit)

static __inline HI_VOID hi_ext_system_sharpen_SharpenEdge_write(HI_U8 Index, HI_U8 data){
	IOWR_8DIRECT(0x15060 + Index, data);
}
static __inline HI_U8 hi_ext_system_sharpen_SharpenEdge_read(HI_U8 Index) {
    return (IORD_8DIRECT(0x15060 + Index));
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpen_EnLumaCtrl
// ------------------------------------------------------------------------------ //
// sharpening EnLumaCtrl table
// ------------------------------------------------------------------------------ //
// args: data (8-bit)

static __inline HI_VOID hi_ext_system_sharpen_EnLumaCtrl_write(HI_U8 Index, HI_U8 data){
	IOWR_8DIRECT(0x15080 + Index, data);
}
static __inline HI_U8 hi_ext_system_sharpen_EnLumaCtrl_read(HI_U8 Index) {
    return (IORD_8DIRECT(0x15080 + Index));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpen_lumNoSt
// ------------------------------------------------------------------------------ //
// sharpening lumNoSt table
// ------------------------------------------------------------------------------ //
// args: data (8-bit)



// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpen_Limit
// ------------------------------------------------------------------------------ //
// sharpening Limit table
// ------------------------------------------------------------------------------ //
// args: data (8-bit)

static __inline HI_VOID hi_ext_system_sharpen_NoiseLumaCtrl_write(HI_U8 Index, HI_U8 data){
	IOWR_8DIRECT(0x150b0 + (Index *sizeof(HI_U8)), (data&0x1F));
}
static __inline HI_U8 hi_ext_system_sharpen_NoiseLumaCtrl_read(HI_U8 Index) {
    return ((IORD_8DIRECT(0x150b0 + (Index *sizeof(HI_U8))))&0x1F);
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpen_edgsm_en
// ------------------------------------------------------------------------------ //
// sharpening edgsm en table
// ------------------------------------------------------------------------------ //
// args: data (8-bit)

static __inline HI_VOID hi_ext_system_sharpen_EdgeFiltStr_write(HI_U8 index, HI_U8 data){
	IOWR_8DIRECT(0x150d0 + (index *sizeof(HI_U8)), (data&0x7F));
}
static __inline HI_U8 hi_ext_system_sharpen_EdgeFiltStr_read(HI_U8 index) {
    return ((IORD_8DIRECT(0x150d0 + (index *sizeof(HI_U8))))&0x7F);
}



// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpen_diffthd
// ------------------------------------------------------------------------------ //
// sharpening diffthd table
// ------------------------------------------------------------------------------ //
// args: data (8-bit)

static __inline HI_VOID hi_ext_system_sharpen_diffthd_write(HI_U8 index, HI_U8 data){
	IOWR_8DIRECT(0x150f0 + (index *sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_sharpen_diffthd_read(HI_U8 index) {
    return (IORD_8DIRECT(0x150f0 + (index *sizeof(HI_U8))));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpen_edgsmstr
// ------------------------------------------------------------------------------ //
// sharpening edgsmstr table
// ------------------------------------------------------------------------------ //
// args: data (8-bit)

static __inline HI_VOID hi_ext_system_sharpen_edgsmstr_write(HI_U8 index, HI_U8 data){
	IOWR_8DIRECT(0x15100 + (index *sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_sharpen_edgsmstr_read(HI_U8 index) {
    return (IORD_8DIRECT(0x15100 + (index *sizeof(HI_U8))));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpen_dirRlyShift
// ------------------------------------------------------------------------------ //
// sharpening dirRlyShift table
// ------------------------------------------------------------------------------ //
// args: data (8-bit)

static __inline HI_VOID hi_ext_system_sharpen_dirRlyShift_write(HI_U8 index, HI_U8 data){
	IOWR_8DIRECT(0x15110 + (index *sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_sharpen_dirRlyShift_read(HI_U8 index) {
    return (IORD_8DIRECT(0x15110 + (index *sizeof(HI_U8))));
}

static __inline HI_VOID hi_ext_system_auto_sharpen_EnShtSup_write(HI_U8 u8Index, HI_U8 data){
    IOWR_8DIRECT((0x15120 + u8Index*sizeof(HI_U8)), data);
}

static __inline HI_U8 hi_ext_system_auto_sharpen_EnShtSup_read(HI_U8 u8Index){
    return IORD_8DIRECT((0x15120 + u8Index*sizeof(HI_U8)));
}

static __inline HI_VOID hi_ext_system_auto_sharpen_OvShtVarMin_write(HI_U8 u8Index, HI_U8 data){
    IOWR_8DIRECT((0x15130 + u8Index*sizeof(HI_U8)), data);
}

static __inline HI_U8 hi_ext_system_auto_sharpen_OvShtVarMin_read(HI_U8 u8Index){
    return IORD_8DIRECT((0x15130 + u8Index*sizeof(HI_U8)));
}
  
static __inline HI_VOID hi_ext_system_auto_sharpen_OvShtVarMax_write(HI_U8 u8Index, HI_U8 data){
    IOWR_8DIRECT((0x15140 + u8Index*sizeof(HI_U8)), data);
}

static __inline HI_U8 hi_ext_system_auto_sharpen_OvShtVarMax_read(HI_U8 u8Index){
    return IORD_8DIRECT((0x15140 + u8Index*sizeof(HI_U8)));
}

static __inline HI_VOID hi_ext_system_auto_sharpen_OvShtWgtMin_write(HI_U8 u8Index, HI_U8 data){
    IOWR_8DIRECT((0x15150 + u8Index*sizeof(HI_U8)), data);
}  		

static __inline HI_U8 hi_ext_system_auto_sharpen_OvShtWgtMin_read(HI_U8 u8Index){
    return IORD_8DIRECT((0x15150 + u8Index*sizeof(HI_U8)));
}
	
static __inline HI_VOID hi_ext_system_auto_sharpen_UnShtVarMin_write(HI_U8 u8Index, HI_U8 data){
    IOWR_8DIRECT((0x15160 + u8Index*sizeof(HI_U8)), data);
}  

static __inline HI_U8 hi_ext_system_auto_sharpen_UnShtVarMin_read(HI_U8 u8Index){
    return IORD_8DIRECT((0x15160 + u8Index*sizeof(HI_U8)));
}
 
static __inline HI_VOID hi_ext_system_auto_sharpen_UnShtVarMax_write(HI_U8 u8Index, HI_U8 data){
    IOWR_8DIRECT((0x15170 + u8Index*sizeof(HI_U8)), data);
}   

static __inline HI_U8 hi_ext_system_auto_sharpen_UnShtVarMax_read(HI_U8 u8Index){
    return IORD_8DIRECT((0x15170 + u8Index*sizeof(HI_U8)));
}
 
static __inline HI_VOID hi_ext_system_auto_sharpen_UnShtWgtMin_write(HI_U8 u8Index, HI_U8 data){
    IOWR_8DIRECT((0x15180 + u8Index*sizeof(HI_U8)), data);
}

static __inline HI_U8 hi_ext_system_auto_sharpen_UnShtWgtMin_read(HI_U8 u8Index){
    return IORD_8DIRECT((0x15180 + u8Index*sizeof(HI_U8)));
}

static __inline HI_VOID hi_ext_system_auto_sharpen_UnShtDiffMax_write(HI_U8 u8Index, HI_U8 data){
    IOWR_8DIRECT((0x15190 + u8Index*sizeof(HI_U8)), data);
} 

static __inline HI_U8 hi_ext_system_auto_sharpen_UnShtDiffMax_read(HI_U8 u8Index){
    return IORD_8DIRECT((0x15190 + u8Index*sizeof(HI_U8)));
}

static __inline HI_VOID hi_ext_system_auto_sharpen_UnShtDiffMin_write(HI_U8 u8Index, HI_U8 data){
    IOWR_8DIRECT((0x151a0 + u8Index*sizeof(HI_U8)), data);
} 

static __inline HI_U8 hi_ext_system_auto_sharpen_UnShtDiffMin_read(HI_U8 u8Index){
    return IORD_8DIRECT((0x151a0 + u8Index*sizeof(HI_U8)));
}

static __inline HI_VOID hi_ext_system_auto_sharpen_UnShtDiffWgtMin_write(HI_U8 u8Index, HI_U8 data){
    IOWR_8DIRECT((0x151b0 + u8Index*sizeof(HI_U8)), data);
}

static __inline HI_U8 hi_ext_system_auto_sharpen_UnShtDiffWgtMin_read(HI_U8 u8Index){
    return IORD_8DIRECT((0x151b0 + u8Index*sizeof(HI_U8)));
}

static __inline HI_VOID hi_ext_system_auto_sharpen_OvShtDiffMax_write(HI_U8 u8Index, HI_U8 data){
    IOWR_8DIRECT((0x151c0 + u8Index*sizeof(HI_U8)), data);
}

static __inline HI_U8 hi_ext_system_auto_sharpen_OvShtDiffMax_read(HI_U8 u8Index){
    return IORD_8DIRECT((0x151c0 + u8Index*sizeof(HI_U8)));
}

static __inline HI_VOID hi_ext_system_auto_sharpen_OvShtDiffMin_write(HI_U8 u8Index, HI_U8 data){
    IOWR_8DIRECT((0x151d0 + u8Index*sizeof(HI_U8)), data);
}

static __inline HI_U8 hi_ext_system_auto_sharpen_OvShtDiffMin_read(HI_U8 u8Index){
    return IORD_8DIRECT((0x151d0 + u8Index*sizeof(HI_U8)));
}

static __inline HI_VOID hi_ext_system_auto_sharpen_OvShtDiffWgtMin_write(HI_U8 u8Index, HI_U8 data){
    IOWR_8DIRECT((0x151e0 + u8Index*sizeof(HI_U8)), data);
}

static __inline HI_U8 hi_ext_system_auto_sharpen_OvShtDiffWgtMin_read(HI_U8 u8Index){
    return IORD_8DIRECT((0x151e0 + u8Index*sizeof(HI_U8)));
}

static __inline HI_VOID hi_ext_system_auto_sharpen_MaxNoiseThd_write(HI_U8 u8Index, HI_U8 data){
    IOWR_8DIRECT((0x151f0 + u8Index*sizeof(HI_U8)), data);
}

static __inline HI_U8 hi_ext_system_auto_sharpen_MaxNoiseThd_read(HI_U8 u8Index){
    return IORD_8DIRECT((0x151f0 + u8Index*sizeof(HI_U8)));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpen_LumaWgt
// ------------------------------------------------------------------------------ //
// sharpening LumaWgt table length:32*16 BYTE
// ------------------------------------------------------------------------------ //
// args: data (8-bit)

static __inline HI_VOID hi_ext_system_sharpen_LumaWgt_write(HI_U16 Index, HI_U8 data){
	IOWR_8DIRECT(0x15200 + (Index *sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_sharpen_LumaWgt_read(HI_U16 Index) {
    return (IORD_8DIRECT(0x15200 + (Index *sizeof(HI_U8))));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_LumaWgt
// ------------------------------------------------------------------------------ //
// sharpening manual LumaWgt table length:32*1 BYTE
// ------------------------------------------------------------------------------ //
// args: data (8-bit)

static __inline HI_VOID hi_ext_system_manual_sharpen_LumaWgt_write(HI_U8 index, HI_U8 data){
	IOWR_8DIRECT(0x15400 + (index *sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_manual_sharpen_LumaWgt_read(HI_U8 index) {
    return (IORD_8DIRECT(0x15400 + (index *sizeof(HI_U8))));
}



// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_JagCtrl
// ------------------------------------------------------------------------------ //
// sharpening VarJagThd1
// ------------------------------------------------------------------------------ //
// args: data (16-bit) 

static __inline HI_VOID hi_ext_system_sharpen_JagCtrl_write(HI_U8 u8Index, HI_U8 data){
    IOWR_8DIRECT((0x15430 + u8Index*sizeof(HI_U8)), data);
}

static __inline HI_U8 hi_ext_system_sharpen_JagCtrl_read(HI_U8 u8Index){
    return IORD_8DIRECT((0x15430 + u8Index*sizeof(HI_U8)));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_VarJagThd1
// ------------------------------------------------------------------------------ //
// sharpening VarJagThd1
// ------------------------------------------------------------------------------ //
// args: data (16-bit) 


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_sharpen_VarJagThd2
// ------------------------------------------------------------------------------ //
// sharpening VarJagThd2
// ------------------------------------------------------------------------------ //
// args: data (8-bit) 

static __inline HI_VOID hi_ext_system_auto_sharpen_VarJagThd2_write(HI_U8 u8Index, HI_U16 data){
	IOWR_16DIRECT((0x15460 + u8Index*sizeof(HI_U16)), data);
}  

static __inline HI_U16 hi_ext_system_auto_sharpen_VarJagThd2_read(HI_U8 u8Index){
	return IORD_16DIRECT((0x15460 + u8Index*sizeof(HI_U16)));
} 

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpen_TextureStIn
// ------------------------------------------------------------------------------ //
// sharpening TextureStIn len = 2
// ------------------------------------------------------------------------------ //
// args: data (8-bit) 

static __inline HI_VOID hi_ext_system_sharpen_TextureStIn_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT((0x15480 + u8Index*sizeof(HI_U8)), data);
}  

static __inline HI_U8 hi_ext_system_sharpen_TextureStIn_read(HI_U8 u8Index){
	return IORD_8DIRECT((0x15480 + u8Index*sizeof(HI_U8)));
} 

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpen_TextureStOut
// ------------------------------------------------------------------------------ //
// sharpening TextureStOut len = 2
// ------------------------------------------------------------------------------ //
// args: data (8-bit) 

static __inline HI_VOID hi_ext_system_sharpen_TextureStOut_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT((0x15482 + u8Index*sizeof(HI_U8)), data);
}  

static __inline HI_U8 hi_ext_system_sharpen_TextureStOut_read(HI_U8 u8Index){
	return IORD_8DIRECT((0x15482 + u8Index*sizeof(HI_U8)));
} 


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpen_EdgeStIn
// ------------------------------------------------------------------------------ //
// sharpening EdgeStIn len = 2
// ------------------------------------------------------------------------------ //
// args: data (8-bit) 

static __inline HI_VOID hi_ext_system_sharpen_EdgeStIn_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT((0x15484 + u8Index*sizeof(HI_U8)), data);
}  

static __inline HI_U8 hi_ext_system_sharpen_EdgeStIn_read(HI_U8 u8Index){
	return IORD_8DIRECT((0x15484 + u8Index*sizeof(HI_U8)));
} 

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpen_EdgeStOut
// ------------------------------------------------------------------------------ //
// sharpening EdgeStOut len = 2
// ------------------------------------------------------------------------------ //
// args: data (8-bit) 

static __inline HI_VOID hi_ext_system_sharpen_EdgeStOut_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT((0x15486 + u8Index*sizeof(HI_U8)), data);
}  

static __inline HI_U8 hi_ext_system_sharpen_EdgeStOut_read(HI_U8 u8Index){
	return IORD_8DIRECT((0x15486 + u8Index*sizeof(HI_U8)));
} 


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpen_overshootAmtIn
// ------------------------------------------------------------------------------ //
// sharpening overshootAmtIn len = 2
// ------------------------------------------------------------------------------ //
// args: data (8-bit) 

static __inline HI_VOID hi_ext_system_sharpen_overshootAmtIn_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT((0x15488 + u8Index*sizeof(HI_U8)), data);
}  

static __inline HI_U8 hi_ext_system_sharpen_overshootAmtIn_read(HI_U8 u8Index){
	return IORD_8DIRECT((0x15488 + u8Index*sizeof(HI_U8)));
} 

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpen_overshootAmtOut
// ------------------------------------------------------------------------------ //
// sharpening overshootAmtOut len = 2
// ------------------------------------------------------------------------------ //
// args: data (8-bit) 

static __inline HI_VOID hi_ext_system_sharpen_overshootAmtOut_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT((0x1548a + u8Index*sizeof(HI_U8)), data);
}  

static __inline HI_U8 hi_ext_system_sharpen_overshootAmtOut_read(HI_U8 u8Index){
	return IORD_8DIRECT((0x1548a + u8Index*sizeof(HI_U8)));
} 

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpen_undershootAmtIn
// ------------------------------------------------------------------------------ //
// sharpening undershootAmtIn len = 2
// ------------------------------------------------------------------------------ //
// args: data (8-bit) 

static __inline HI_VOID hi_ext_system_sharpen_undershootAmtIn_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT((0x1548c + u8Index*sizeof(HI_U8)), data);
}  

static __inline HI_U8 hi_ext_system_sharpen_undershootAmtIn_read(HI_U8 u8Index){
	return IORD_8DIRECT((0x1548c + u8Index*sizeof(HI_U8)));
} 

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpen_undershootAmtOut
// ------------------------------------------------------------------------------ //
// sharpening undershootAmtOut len = 2
// ------------------------------------------------------------------------------ //
// args: data (8-bit) 

static __inline HI_VOID hi_ext_system_sharpen_undershootAmtOut_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT((0x1548e + u8Index*sizeof(HI_U8)), data);
}  

static __inline HI_U8 hi_ext_system_sharpen_undershootAmtOut_read(HI_U8 u8Index){
	return IORD_8DIRECT((0x1548e + u8Index*sizeof(HI_U8)));
} 


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpen_TextureNoiseThdIn
// ------------------------------------------------------------------------------ //
// sharpening TextureNoiseThdIn len = 2
// ------------------------------------------------------------------------------ //
// args: data (8-bit) 

static __inline HI_VOID hi_ext_system_sharpen_TextureNoiseThdIn_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT((0x15490 + u8Index*sizeof(HI_U8)), data);
}  

static __inline HI_U8 hi_ext_system_sharpen_TextureNoiseThdIn_read(HI_U8 u8Index){
	return IORD_8DIRECT((0x15490 + u8Index*sizeof(HI_U8)));
} 

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpen_TextureNoiseThdOut
// ------------------------------------------------------------------------------ //
// sharpening TextureNoiseThdOut len = 2
// ------------------------------------------------------------------------------ //
// args: data (8-bit) 

static __inline HI_VOID hi_ext_system_sharpen_TextureNoiseThdOut_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT((0x15492 + u8Index*sizeof(HI_U8)), data);
}  

static __inline HI_U8 hi_ext_system_sharpen_TextureNoiseThdOut_read(HI_U8 u8Index){
	return IORD_8DIRECT((0x15492 + u8Index*sizeof(HI_U8)));
} 


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpen_EdgeNoiseThdIn
// ------------------------------------------------------------------------------ //
// sharpening EdgeNoiseThdIn len = 2
// ------------------------------------------------------------------------------ //
// args: data (8-bit) 

static __inline HI_VOID hi_ext_system_sharpen_EdgeNoiseThdIn_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT((0x15494 + u8Index*sizeof(HI_U8)), data);
}  

static __inline HI_U8 hi_ext_system_sharpen_EdgeNoiseThdIn_read(HI_U8 u8Index){
	return IORD_8DIRECT((0x15494 + u8Index*sizeof(HI_U8)));
} 

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpen_EdgeNoiseThdOut
// ------------------------------------------------------------------------------ //
// sharpening EdgeNoiseThdOut len = 2
// ------------------------------------------------------------------------------ //
// args: data (8-bit) 

static __inline HI_VOID hi_ext_system_sharpen_EdgeNoiseThdOut_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT((0x15496 + u8Index*sizeof(HI_U8)), data);
}  

static __inline HI_U8 hi_ext_system_sharpen_EdgeNoiseThdOut_read(HI_U8 u8Index){
	return IORD_8DIRECT((0x15496 + u8Index*sizeof(HI_U8)));
} 


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpen_DetailCtrlIn
// ------------------------------------------------------------------------------ //
// sharpening DetailCtrlIn len = 2
// ------------------------------------------------------------------------------ //
// args: data (8-bit) 

static __inline HI_VOID hi_ext_system_sharpen_DetailCtrlIn_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT((0x15498 + u8Index*sizeof(HI_U8)), data);
}  

static __inline HI_U8 hi_ext_system_sharpen_DetailCtrlIn_read(HI_U8 u8Index){
	return IORD_8DIRECT((0x15498 + u8Index*sizeof(HI_U8)));
} 

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpen_DetailCtrlOut
// ------------------------------------------------------------------------------ //
// sharpening DetailCtrlOut len = 2
// ------------------------------------------------------------------------------ //
// args: data (8-bit) 

static __inline HI_VOID hi_ext_system_sharpen_DetailCtrlOut_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT((0x1549a + u8Index*sizeof(HI_U8)), data);
}  

static __inline HI_U8 hi_ext_system_sharpen_DetailCtrlOut_read(HI_U8 u8Index){
	return IORD_8DIRECT((0x1549a + u8Index*sizeof(HI_U8)));
} 

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpen_UDHfGainIn_write
// ------------------------------------------------------------------------------ //
// sharpening UDHfGainIn len = 2
// ------------------------------------------------------------------------------ //
// args: data (16-bit) 

static __inline HI_VOID hi_ext_system_sharpen_UDHfGainIn_write(HI_U8 u8Index, HI_U16 data){
	IOWR_16DIRECT((0x1549c + u8Index*sizeof(HI_U16)), data);
}  

static __inline HI_U16 hi_ext_system_sharpen_UDHfGainIn_read(HI_U8 u8Index){
	return IORD_16DIRECT((0x1549c + u8Index*sizeof(HI_U16)));
} 

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpen_UDHfGainOut_write
// ------------------------------------------------------------------------------ //
// sharpening UDHfGainOut len = 2
// ------------------------------------------------------------------------------ //
// args: data (16-bit) 

static __inline HI_VOID hi_ext_system_sharpen_UDHfGainOut_write(HI_U8 u8Index, HI_U16 data){
	IOWR_16DIRECT((0x154a0 + u8Index*sizeof(HI_U16)), data);
}  

static __inline HI_U16 hi_ext_system_sharpen_UDHfGainOut_read(HI_U8 u8Index){
	return IORD_16DIRECT((0x154a0 + u8Index*sizeof(HI_U16)));
} 

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpen_UDHfGain_write
// ------------------------------------------------------------------------------ //
// sharpening UDHfGain len = 16
// ------------------------------------------------------------------------------ //
// args: data (16-bit) 

static __inline HI_VOID hi_ext_system_sharpen_UDHfGain_write(HI_U8 u8Index, HI_U16 data){
	IOWR_16DIRECT((0x154b0 + u8Index*sizeof(HI_U16)), data);
}  

static __inline HI_U16 hi_ext_system_sharpen_UDHfGain_read(HI_U8 u8Index){
	return IORD_16DIRECT((0x154b0 + u8Index*sizeof(HI_U16)));
} 

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_auto_sharpen_EnShtMax_write
// ------------------------------------------------------------------------------ //
// sharpening EnShtMax len = 16
// ------------------------------------------------------------------------------ //
// args: data (8-bit) 

static __inline HI_VOID hi_ext_system_auto_sharpen_EnShtMax_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT((0x154d0 + u8Index*sizeof(HI_U8)), data);
}  

static __inline HI_U8 hi_ext_system_auto_sharpen_EnShtMax_read(HI_U8 u8Index){
	return IORD_8DIRECT((0x154d0 + u8Index*sizeof(HI_U8)));
} 

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_auto_sharpen_ShtMaxChg_write
// ------------------------------------------------------------------------------ //
// sharpening ShtMaxChg len = 16
// ------------------------------------------------------------------------------ //
// args: data (8-bit) 

static __inline HI_VOID hi_ext_system_auto_sharpen_ShtMaxChg_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT((0x154e0 + u8Index*sizeof(HI_U8)), data);
}  

static __inline HI_U8 hi_ext_system_auto_sharpen_ShtMaxChg_read(HI_U8 u8Index){
	return IORD_8DIRECT((0x154e0 + u8Index*sizeof(HI_U8)));
} 

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_auto_sharpen_ShtMaxVarGain_write
// ------------------------------------------------------------------------------ //
// sharpening ShtMaxChg len = 16
// ------------------------------------------------------------------------------ //
// args: data (8-bit) 

static __inline HI_VOID hi_ext_system_auto_sharpen_ShtMaxVarGain_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT((0x154f0 + u8Index*sizeof(HI_U8)), data);
}  

static __inline HI_U8 hi_ext_system_auto_sharpen_ShtMaxVarGain_read(HI_U8 u8Index){
	return IORD_8DIRECT((0x154f0 + u8Index*sizeof(HI_U8)));
} 




static __inline HI_VOID hi_ext_system_sharpen_SharpenD_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT((0x15510 + u8Index*sizeof(HI_U8)), data);
}  

static __inline HI_U8 hi_ext_system_sharpen_SharpenD_read(HI_U8 u8Index){
	return IORD_8DIRECT((0x15510 + u8Index*sizeof(HI_U8)));

}


static __inline HI_VOID hi_ext_system_sharpen_DetailCtrl_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT((0x15520 + u8Index*sizeof(HI_U8)), data);
}  

static __inline HI_U8 hi_ext_system_sharpen_DetailCtrl_read(HI_U8 u8Index){
	return IORD_8DIRECT((0x15520 + u8Index*sizeof(HI_U8)));

}


static __inline HI_VOID hi_ext_system_sharpen_shootSupSt_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT((0x15530 + u8Index*sizeof(HI_U8)), data);
}  

static __inline HI_U8 hi_ext_system_sharpen_shootSupSt_read(HI_U8 u8Index){
	return IORD_8DIRECT((0x15530 + u8Index*sizeof(HI_U8)));

}

static __inline HI_VOID hi_ext_system_sharpen_FlatNoiseSupSt_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT((0x15540 + u8Index*sizeof(HI_U8)), data);
}  

static __inline HI_U8 hi_ext_system_sharpen_FlatNoiseSupSt_read(HI_U8 u8Index){
	return IORD_8DIRECT((0x15540 + u8Index*sizeof(HI_U8)));

}

static __inline HI_VOID hi_ext_system_sharpen_TextureThd_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT((0x15550 + u8Index*sizeof(HI_U8)), data);
}  

static __inline HI_U8 hi_ext_system_sharpen_TextureThd_read(HI_U8 u8Index){
	return IORD_8DIRECT((0x15550 + u8Index*sizeof(HI_U8)));

}

static __inline HI_VOID hi_ext_system_sharpen_RGain_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT((0x15560 + u8Index*sizeof(HI_U8)), data&0x1F);
}  

static __inline HI_U8 hi_ext_system_sharpen_RGain_read(HI_U8 u8Index){
	return (IORD_8DIRECT((0x15560 + u8Index*sizeof(HI_U8)))&0x1F);

}

static __inline HI_VOID hi_ext_system_sharpen_GGain_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT((0x15570 + u8Index*sizeof(HI_U8)), data);
}  

static __inline HI_U8 hi_ext_system_sharpen_GGain_read(HI_U8 u8Index){
	return IORD_8DIRECT((0x15570 + u8Index*sizeof(HI_U8)));

}

static __inline HI_VOID hi_ext_system_sharpen_BGain_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT((0x15580 + u8Index*sizeof(HI_U8)), data&0x1F);
}  

static __inline HI_U8 hi_ext_system_sharpen_BGain_read(HI_U8 u8Index){
	return (IORD_8DIRECT((0x15580 + u8Index*sizeof(HI_U8)))&0x1F);

}

static __inline HI_VOID hi_ext_system_sharpen_SkinGain_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT((0x15590 + u8Index*sizeof(HI_U8)), data);
}  

static __inline HI_U8 hi_ext_system_sharpen_SkinGain_read(HI_U8 u8Index){
	return IORD_8DIRECT((0x15590 + u8Index*sizeof(HI_U8)));

}

static __inline HI_VOID hi_ext_system_sharpen_EdgeSmoothSt_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT((0x155a0 + u8Index*sizeof(HI_U8)), data);
}  

static __inline HI_U8 hi_ext_system_sharpen_EdgeSmoothSt_read(HI_U8 u8Index){
	return IORD_8DIRECT((0x155a0 + u8Index*sizeof(HI_U8)));

}


static __inline HI_VOID hi_ext_system_sharpen_ShtSupBldr_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT((0x155b0 + u8Index*sizeof(HI_U8)), data);
}  

static __inline HI_U8 hi_ext_system_sharpen_ShtSupBldr_read(HI_U8 u8Index){
	return IORD_8DIRECT((0x155b0 + u8Index*sizeof(HI_U8)));

}

static __inline HI_VOID hi_ext_system_sharpen_RmfGainSft_write(HI_U8 u8Index, HI_S8 data){
	IOWR_8DIRECT((0x155c0 + u8Index*sizeof(HI_S8)), data);
}  

static __inline HI_S8 hi_ext_system_sharpen_RmfGainSft_read(HI_U8 u8Index){
	return IORD_8DIRECT((0x155c0 + u8Index*sizeof(HI_S8)));

}

static __inline HI_VOID hi_ext_system_sharpen_ShadNoiseSupSt_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT((0x155d0 + u8Index*sizeof(HI_U8)), data);
}  

static __inline HI_U8 hi_ext_system_sharpen_ShadNoiseSupSt_read(HI_U8 u8Index){
	return IORD_8DIRECT((0x155d0 + u8Index*sizeof(HI_U8)));

}



static __inline HI_VOID hi_ext_system_sharpen_SharpenUd_write(HI_U8 u8Index, HI_U16 data){
	IOWR_16DIRECT((0x155e0 + u8Index*sizeof(HI_U16)), data&0x3FF);
}  

static __inline HI_U16 hi_ext_system_sharpen_SharpenUd_read(HI_U8 u8Index){
	return (IORD_16DIRECT((0x155e0 + u8Index*sizeof(HI_U16)))&0x3FF);

}



//==============================================================//
//CAC: 0x16000~0x160FF
//==============================================================//

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_localCAC_Cb_Str_table
// ------------------------------------------------------------------------------ //
// 0x154ab ~ 0x154ba localCAC_Cb_Str_table
static __inline HI_VOID hi_ext_system_localCAC_auto_cb_str_table_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT(0x16000+(u8Index*sizeof(HI_U8)), data);
}

static __inline HI_U8 hi_ext_system_localCAC_auto_cb_str_table_read(HI_U8 u8Index){
	return IORD_8DIRECT(0x16000 + (u8Index*sizeof(HI_U8)));
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_localCAC_Cr_Str_table
// ------------------------------------------------------------------------------ //
// 0x154bb ~ 0x154ca localCAC_Cr_Str_table
static __inline HI_VOID hi_ext_system_localCAC_auto_cr_str_table_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT(0x16010+(u8Index*sizeof(HI_U8)), data);
}

static __inline HI_U8 hi_ext_system_localCAC_auto_cr_str_table_read(HI_U8 u8Index){
	return IORD_8DIRECT(0x16010 + (u8Index*sizeof(HI_U8)));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_localCAC_wdr_Auto_Str_enable
// ------------------------------------------------------------------------------ //
// 0x11730 ~ 0x1174f localCAC_wdr_Auto_Str

static __inline HI_VOID hi_ext_system_localCAC_wdr_auto_str_enable_write( HI_U8 data){
    IOWR_8DIRECT(0x16020 , data);
}

static __inline HI_U8 hi_ext_system_localCAC_wdr_auto_str_enable_read(HI_VOID){
    return IORD_8DIRECT(0x16020);
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_localCAC_manual_cb_str
// ------------------------------------------------------------------------------ //
// 0x11730 ~ 0x1174f localCAC_manual_cb
static __inline HI_VOID hi_ext_system_localCAC_manual_cb_str_write( HI_U8 data){
    IOWR_8DIRECT(0x16021 , data);
}

static __inline HI_U8 hi_ext_system_localCAC_manual_cb_str_read(HI_VOID){
    return IORD_8DIRECT(0x16021);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_localCAC_manual_cr_str
// ------------------------------------------------------------------------------ //
// 0x11730 ~ 0x1174f localCAC_manual_cr
static __inline HI_VOID hi_ext_system_localCAC_manual_cr_str_write( HI_U8 data){
    IOWR_8DIRECT(0x16024 , data);
}

static __inline HI_U8 hi_ext_system_localCAC_manual_cr_str_read(HI_VOID){
    return IORD_8DIRECT(0x16024);
}


static __inline HI_VOID hi_ext_system_localCAC_manual_enable_write( HI_U8 data){
    IOWR_8DIRECT(0x16022 , data&0x1);
}

static __inline HI_U8 hi_ext_system_localCAC_manual_enable_read(HI_VOID){
    return (IORD_8DIRECT(0x16022)&0x1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_localCAC_purple_det_range
// ------------------------------------------------------------------------------ //
// 0x11730 ~ 0x1174f localCAC_purple_det_range
static __inline HI_VOID hi_ext_system_localCAC_purple_det_range_write( HI_U16 data){
    IOWR_16DIRECT(0x16028 , data);
}

static __inline HI_U16 hi_ext_system_localCAC_purple_det_range_read(HI_VOID){
    return IORD_16DIRECT(0x16028);
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_localCAC_enable
// ------------------------------------------------------------------------------ //
// 0x11730 ~ 0x1174f localCAC_enable
static __inline HI_VOID hi_ext_system_localCAC_enable_write( HI_U8 data){
    IOWR_8DIRECT(0x16030 , data&0x1);
}

static __inline HI_U16 hi_ext_system_localCAC_enable_read(HI_VOID){
    return (IORD_8DIRECT(0x16030)&0x1);
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_localCAC_purple_var_thr
// ------------------------------------------------------------------------------ //
// 0x11730 ~ 0x1174f localCAC_purple_var_thr
static __inline HI_VOID hi_ext_system_localCAC_purple_var_thr_write( HI_U16 data){
	IOWR_16DIRECT(0x16034 , data);
}

static __inline HI_U16 hi_ext_system_localCAC_purple_var_thr_read(HI_VOID){
	return IORD_16DIRECT(0x16034);
}


//==============================================================//
//CA: 0x16100~0x162FF
//==============================================================//

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_ca_y_ratio_lut_write
// ------------------------------------------------------------------------------ //
// LEN =128 DATASIZE HI_U16  16*8*2
//0x16100 ~ 0x161FF
static __inline HI_VOID hi_ext_system_ca_y_ratio_lut_write(HI_U8 u8Index, HI_U16 data){
	IOWR_16DIRECT(0x16100+(u8Index*sizeof(HI_U16)), data);
}

static __inline HI_U16 hi_ext_system_ca_y_ratio_lut_read(HI_U8 u8Index){
	return IORD_16DIRECT(0x16100 + (u8Index*sizeof(HI_U16)));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_ca_saturation_ratio_write
// ------------------------------------------------------------------------------ //
//0x16200 ~ 0x16200
#define HI_ISP_EXT_CA_SATURATION_RATIO_DEFAULT (1000)
static __inline HI_VOID hi_ext_system_ca_saturation_ratio_write(HI_U16 data){
	IOWR_16DIRECT(0x16200, data);
}

static __inline HI_U16 hi_ext_system_ca_saturation_ratio_read(HI_VOID){
	return IORD_16DIRECT(0x16200);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_ca_saturation_ratio_write
// ------------------------------------------------------------------------------ //
//0x16200 ~ 0x16200
#define HI_ISP_EXT_CA_LUMA_THD_HIGH_DEFAULT (400)
static __inline HI_VOID hi_ext_system_ca_luma_thd_high_write(HI_U16 data){
	IOWR_16DIRECT(0x16202, data);
}

static __inline HI_U16 hi_ext_system_ca_luma_thd_high_read(HI_VOID){
	return IORD_16DIRECT(0x16202);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_ca_y_ratio_lut_write
// ------------------------------------------------------------------------------ //
// LEN =128 DATASIZE HI_U16  16*8*2
//0x16204 ~ 0x16213
static __inline HI_VOID hi_ext_system_ca_iso_ratio_lut_write(HI_U8 u8Index, HI_U32 data){
	IOWR_32DIRECT(0x16204+(u8Index*sizeof(HI_U32)), data);
}

static __inline HI_U16 hi_ext_system_ca_iso_ratio_lut_read(HI_U8 u8Index){
	return IORD_32DIRECT(0x16204 + (u8Index*sizeof(HI_U32)));
}

//==============================================================//
//==============================================================//
//==============================================================//
//NDDM==========================================================//
//NDDM: 0x16300~0x164FF
//NDDM==========================================================//
//==============================================================//
//==============================================================//
//==============================================================//
//==============================================================//

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_nddm_gain_write
// ------------------------------------------------------------------------------ //
//0x16300 ~ 0x16300
// args: data (16-bit) 
static __inline HI_VOID hi_ext_system_demosaic_auto_gain_write(HI_U8 u8Index, HI_U16 data){
	IOWR_16DIRECT(0x16300+(u8Index*sizeof(HI_U16)), data);
}

static __inline HI_U16 hi_ext_system_demosaic_auto_gain_read(HI_U8 u8Index){
	return IORD_16DIRECT(0x16300+(u8Index*sizeof(HI_U16)));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_bldr_crcb_write
// ------------------------------------------------------------------------------ //
//0x16300 ~ 0x16300
// args: data (8-bit) 
static __inline HI_VOID hi_ext_system_varoffsetgain_lut_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT(0x16310+(u8Index*sizeof(HI_U8)), data);
}

static __inline HI_U8 hi_ext_system_varoffsetgain_lut_read(HI_U8 u8Index){
	return IORD_8DIRECT(0x16310+(u8Index*sizeof(HI_U8)));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_bldr_gfstr_write
// ------------------------------------------------------------------------------ //
//0x16300 ~ 0x16300
// args: data (8-bit) 
static __inline HI_VOID hi_ext_system_demosaic_auto_detail_enhance_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT(0x16320+(u8Index*sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_demosaic_auto_detail_enhance_read(HI_U8 u8Index){
	return IORD_8DIRECT(0x16320+(u8Index*sizeof(HI_U8)));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_multimf_write
// ------------------------------------------------------------------------------ //
//0x16300 ~ 0x16300
// args: data (8-bit) 
static __inline HI_VOID hi_ext_system_dithmax_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT(0x16330+(u8Index*sizeof(HI_U8)), data);
}

static __inline HI_U8 hi_ext_system_dithmax_read(HI_U8 u8Index){
	return IORD_8DIRECT(0x16330+(u8Index*sizeof(HI_U8)));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_clipapt_write
// ------------------------------------------------------------------------------ //
//0x16300 ~ 0x16300
// args: data (8-bit) 
static __inline HI_VOID hi_ext_system_chrmgffix_lut_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT(0x16340+(u8Index*sizeof(HI_U8)), data);
}

static __inline HI_U8 hi_ext_system_chrmgffix_lut_read(HI_U8 u8Index){
	return IORD_8DIRECT(0x16340+(u8Index*sizeof(HI_U8)));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_rawbldrset_write
// ------------------------------------------------------------------------------ //
//0x16300 ~ 0x16300
// args: data (8-bit) 
static __inline HI_VOID hi_ext_system_multimfred_lut_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT(0x16350+(u8Index*sizeof(HI_U8)), data);
}

static __inline HI_U8 hi_ext_system_multimfred_lut_read(HI_U8 u8Index){
	return IORD_8DIRECT(0x16350+(u8Index*sizeof(HI_U8)));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_fcrgfgain_write
// ------------------------------------------------------------------------------ //
//0x16300 ~ 0x16300
// args: data (8-bit) 
static __inline HI_VOID hi_ext_system_multimfblue_lut_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT(0x16360+(u8Index*sizeof(HI_U8)), data);
}

static __inline HI_U8 hi_ext_system_multimfblue_lut_read(HI_U8 u8Index){
	return IORD_8DIRECT(0x16360+(u8Index*sizeof(HI_U8)));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_clipusm_write
// ------------------------------------------------------------------------------ //
//0x16300 ~ 0x16300
// args: data (16-bit) 
static __inline HI_VOID hi_ext_system_filterstrintp_lut_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT(0x16370 +(u8Index*sizeof(HI_U8)), data);
}

static __inline HI_U8 hi_ext_system_filterstrintp_lut_read(HI_U8 u8Index){
	return IORD_8DIRECT(0x16370 +(u8Index*sizeof(HI_U8)));
}

static __inline HI_VOID hi_ext_system_clipdeltagain_lut_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT(0x16380 +(u8Index*sizeof(HI_U8)), data);
}

static __inline HI_U8 hi_ext_system_clipdeltagain_lut_read(HI_U8 u8Index){
	return IORD_8DIRECT(0x16380 +(u8Index*sizeof(HI_U8)));
}
// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_ehcgainlut_write len =17
// ------------------------------------------------------------------------------ //
//0x16300 ~ 0x16300
// args: data (16-bit) 
static __inline HI_VOID hi_ext_system_clipadjustmax_lut_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT(0x16390 +(u8Index*sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_clipadjustmax_lut_read(HI_U8 u8Index){
	return IORD_8DIRECT(0x16390 +(u8Index*sizeof(HI_U8)));
}
static __inline HI_VOID hi_ext_system_fcrgfgain_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT(0x163a0+(u8Index*sizeof(HI_U8)), data);
}

static __inline HI_U8 hi_ext_system_fcrgfgain_read(HI_U8 u8Index){
	return IORD_8DIRECT(0x163a0+(u8Index*sizeof(HI_U8)));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_chrmgflow_lut_write len =9
// ------------------------------------------------------------------------------ //
//0x163a4 ~ 0x163b5
// args: data (16-bit) 
static __inline HI_VOID hi_ext_system_saturthfix_lut_write(HI_U8 u8Index, HI_U16 data){
	IOWR_16DIRECT(0x163b0+(u8Index*sizeof(HI_U16)), data);
}

static __inline HI_U16 hi_ext_system_saturthfix_lut_read(HI_U8 u8Index){
	return IORD_16DIRECT(0x163b0+(u8Index*sizeof(HI_U16)));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_chrmgfhigh_lut_write len =9
// ------------------------------------------------------------------------------ //
//0x163b8 ~ 0x163c9
// args: data (16-bit) 
static __inline HI_VOID hi_ext_system_satubthfix_lut_write(HI_U8 u8Index, HI_U16 data){
	IOWR_16DIRECT(0x163d0+(u8Index*sizeof(HI_U16)), data);
}

static __inline HI_U16 hi_ext_system_satubthfix_lut_read(HI_U8 u8Index){
	return IORD_16DIRECT(0x163d0+(u8Index*sizeof(HI_U16)));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_satuthfix_lut_write len =9
// ------------------------------------------------------------------------------ //
//0x163cc ~ 0x163dd
// args: data (16-bit) 
static __inline HI_VOID hi_ext_system_satufixehcy_lut_write(HI_U8 u8Index, HI_U16 data){
	IOWR_16DIRECT(0x16400+(u8Index*sizeof(HI_U16)), data);
}

static __inline HI_U16 hi_ext_system_demosaic_auto_satufixehcy_lut_read(HI_U8 u8Index){
	return IORD_16DIRECT(0x16400+(u8Index*sizeof(HI_U16)));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_satuthlow_lut_write len =9
// ------------------------------------------------------------------------------ //
//0x163e0 ~ 0x163f1
// args: data (16-bit) 
static __inline HI_VOID hi_ext_system_demosaic_auto_noise_suppress_strength_write(HI_U8 u8Index, HI_U16 data){
	IOWR_16DIRECT(0x16420 +(u8Index*sizeof(HI_U16)), data);
}

static __inline HI_U16 hi_ext_system_demosaic_auto_noise_suppress_strength_read(HI_U8 u8Index){
	return IORD_16DIRECT(0x16420 +(u8Index*sizeof(HI_U16)));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_satuthhigh_lut_write len =9
// ------------------------------------------------------------------------------ //
//0x163f4 ~ 0x16405
// args: data (16-bit) 
static __inline HI_VOID hi_ext_system_satuthfix_lut_write(HI_U8 u8Index, HI_U16 data){
	IOWR_16DIRECT(0x16440+(u8Index*sizeof(HI_U16)), data);
}

static __inline HI_U8 hi_ext_system_satuthfix_lut_read(HI_U8 u8Index){
	return IORD_16DIRECT(0x16440+(u8Index*sizeof(HI_U16)));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_graythfix_lut_write len =9
// ------------------------------------------------------------------------------ //
//0x16408 ~ 0x16419
// args: data (16-bit) 
static __inline HI_VOID hi_ext_system_graythfixlow_lut_write(HI_U8 u8Index, HI_U16 data){
	IOWR_16DIRECT(0x16460+(u8Index*sizeof(HI_U16)), data);
}

static __inline HI_U16 hi_ext_system_graythfixlow_lut_read(HI_U8 u8Index){
	return IORD_16DIRECT(0x16460+(u8Index*sizeof(HI_U16)));
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_graythhigh_lut_write len =9
// ------------------------------------------------------------------------------ //
//0x16430 ~ 0x16441
// args: data (16-bit) 
#define HI_EXT_SYSTEM_DEMOSAIC_MANUAL_GAIN_LINEAR_DEFAULT (128)
#define HI_EXT_SYSTEM_DEMOSAIC_MANUAL_GAIN_WDR_DEFAULT (128)
static __inline HI_VOID hi_ext_system_demosaic_manual_gain_write(HI_U16 data){
	IOWR_16DIRECT(0x16500, data);
}
static __inline HI_U16 hi_ext_system_demosaic_manual_gain_read(HI_VOID){
	return IORD_16DIRECT(0x16500);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_rawsatuhigh_lut_write len =9
// ------------------------------------------------------------------------------ //
//0x16458 ~ 0x16469
// args: data (16-bit) 
#define HI_EXT_SYSTEM_DEMOSAIC_MANUAL_DETAIL_ENHANCE_LINEAR_DEFAULT (4)
#define HI_EXT_SYSTEM_DEMOSAIC_MANUAL_DETAIL_ENHANCE_WDR_DEFAULT (8)
static __inline HI_VOID hi_ext_system_demosaic_manual_detail_enhance_write(HI_U8 data){
	IOWR_8DIRECT(0x16502, data);
}
static __inline HI_U8 hi_ext_system_demosaic_manual_detail_enhance_read(HI_VOID){
	return IORD_8DIRECT(0x16502);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_bayernr
// ------------------------------------------------------------------------------ //
//0x164c8 ~ 0x164d9
// args: data (16-bit) 

static __inline HI_VOID hi_ext_system_manual_bayernr_cprclip_write(HI_U16 data) {
        IOWR_8DIRECT(0x1680c, data);
}
static __inline HI_U16 hi_ext_system_manual_bayernr_cprclip_read(HI_VOID) {
    return IORD_8DIRECT(0x1680c);
}


static __inline HI_VOID hi_ext_system_manual_bayernr_coring_low_ratio_write(HI_U16 data) {
    IOWR_8DIRECT(0x1680e, data);
}
static __inline HI_U16 hi_ext_system_manual_bayernr_coring_low_ratio_read(HI_VOID) {
   return IORD_8DIRECT(0x1680e);
}


#define HI_EXT_SYSTEM_DEMOSAIC_AUTO_HFINTP_TH_LOW_NR_EDGE_TH_DEFAULT (16)
static __inline HI_VOID hi_ext_system_demosaic_auto_edge_smooth_thresh_write(HI_U8 u8Index, HI_U16 data){
	IOWR_16DIRECT(0x18000+(u8Index*sizeof(HI_U16)), data);
}
static __inline HI_U16 hi_ext_system_demosaic_auto_edge_smooth_thresh_read(HI_U8 u8Index){
	return IORD_16DIRECT(0x18000+(u8Index*sizeof(HI_U16)));
}

static __inline HI_VOID hi_ext_system_isp_balancefe_enable_write(HI_U8 data){
	IOWR_8DIRECT(0x18020, data);
}

static __inline HI_U8 hi_ext_system_isp_balancefe_enable_read(HI_VOID){
	return IORD_8DIRECT(0x18020);
}

#define HI_EXT_SYSTEM_TOP_BIT_RESERVE0_WRITE_DEFAULT (0)
#define HI_EXT_SYSTEM_TOP_BIT_RESERVE0_WRITE_DATASIZE (2)

static __inline HI_VOID hi_ext_system_top_bit_reserve0_write(HI_U8 data)
{
  IOWR_8DIRECT(0x18022, data&0x3);
}

static __inline HI_U8 hi_ext_system_top_bit_reserve0_read(HI_VOID)
{
	return (IORD_8DIRECT(0x18022)&0x3);
}



#define HI_EXT_SYSTEM_TOP_BIT_RESERVE1_WRITE_DEFAULT (0)
#define HI_EXT_SYSTEM_TOP_BIT_RESERVE1_WRITE_DATASIZE (2)

static __inline HI_VOID hi_ext_system_top_bit_reserve1_write(HI_U8 data)
{
  IOWR_8DIRECT(0x18024, data&0x3);
}

static __inline HI_U8 hi_ext_system_top_bit_reserve1_read(HI_VOID)
{
	return (IORD_8DIRECT(0x18024)&0x3);
}

#define HI_EXT_SYSTEM_TOP_BIT_RESERVE2_WRITE_DEFAULT (0)
#define HI_EXT_SYSTEM_TOP_BIT_RESERVE2_WRITE_DATASIZE (2)

static __inline HI_VOID hi_ext_system_top_bit_reserve2_write(HI_U8 data)
{
  IOWR_8DIRECT(0x18026, data&0x3);
}

static __inline HI_U8 hi_ext_system_top_bit_reserve2_read(HI_VOID)
{
	return (IORD_8DIRECT(0x18026)&0x3);
}

#define HI_EXT_SYSTEM_TOP_BIT_RESERVE3_WRITE_DEFAULT (0)
#define HI_EXT_SYSTEM_TOP_BIT_RESERVE3_WRITE_DATASIZE (2)

static __inline HI_VOID hi_ext_system_top_bit_reserve3_write(HI_U8 data)
{
  IOWR_8DIRECT(0x18028, data&0x3);
}

static __inline HI_U8 hi_ext_system_top_bit_reserve3_read(HI_VOID)
{
	return (IORD_8DIRECT(0x18028)&0x3);
}

#define HI_EXT_SYSTEM_TOP_BIT_RESERVE4_WRITE_DEFAULT (0)
#define HI_EXT_SYSTEM_TOP_BIT_RESERVE4_WRITE_DATASIZE (2)

static __inline HI_VOID hi_ext_system_top_bit_reserve4_write(HI_U8 data)
{
  IOWR_8DIRECT(0x1802a, data&0x7);
}

static __inline HI_U8 hi_ext_system_top_bit_reserve4_read(HI_VOID)
{
	return (IORD_8DIRECT(0x1802a)&0x7);
}


static __inline HI_VOID hi_ext_system_update_info_phyaddr_write(HI_U32 data) {
    IOWR_32DIRECT(0x18100, data);
}

static __inline HI_U32 hi_ext_system_update_info_phyaddr_read(HI_VOID) {
    return IORD_32DIRECT(0x18100);
}



// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_bayernr_amed_level
// ------------------------------------------------------------------------------ //
//bayernr amed level
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_BAYERNR_ACTUAL_CHROMA_STRENGTH_DEFAULT (0)
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_bayernr_actual_chroma_strength_write(HI_U8 i, HI_U8 data) {
    IOWR_8DIRECT(0x18200+(i*sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_bayernr_actual_chroma_strength_read(HI_U8 i) {
    return IORD_8DIRECT(0x18200+(i*sizeof(HI_U8)));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_bayernr_strength
// ------------------------------------------------------------------------------ //
//bayernr_strength
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_BAYERNR_ACTUAL_STRENGTH_DEFAULT  (100)

// args: data (16-bit)
static __inline HI_VOID hi_ext_system_bayernr_actual_fine_strength_write(HI_U16 data) {
    IOWR_16DIRECT(0x18210, data);
}
static __inline HI_U16 hi_ext_system_bayernr_actual_fine_strength_read(HI_VOID) {
    return IORD_16DIRECT(0x18210);
}

#define  HI_EXT_SYSTEM_BAYERNR_ACTUAL_CORING_WEIGHT_DEFAULT (800)
static __inline HI_VOID hi_ext_system_bayernr_actual_coring_weight_write(HI_U16 data) {
    IOWR_16DIRECT(0x18220, data);
}
static __inline HI_U16 hi_ext_system_bayernr_actual_coring_weight_read(HI_VOID) {
    return IORD_16DIRECT(0x18220);
}

static __inline HI_VOID  hi_ext_system_dehaze_actual_strength_write(HI_U8 data){
	IOWR_8DIRECT(0x18224, data);
}
static __inline HI_U8  hi_ext_system_dehaze_actual_strength_read(HI_VOID){
	return IORD_8DIRECT(0x18224);
}

static __inline HI_VOID  hi_ext_system_drc_actual_strength_write(HI_U8 data){
	IOWR_8DIRECT(0x18226, data);
}
static __inline HI_U8  hi_ext_system_drc_actual_strength_read(HI_VOID){
	return IORD_8DIRECT(0x18226);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_drc_user_tm_enable
// ------------------------------------------------------------------------------ //
static __inline HI_VOID  hi_ext_system_drc_user_tm_enable_write(HI_U8 data){
    IOWR_8DIRECT(0x18227, data);
}
static __inline HI_U8  hi_ext_system_drc_user_tm_enable_read(HI_VOID){
    return IORD_8DIRECT(0x18227);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_drc_user_tm_update
// ------------------------------------------------------------------------------ //
static __inline HI_VOID  hi_ext_system_drc_user_tm_update_write(HI_U8 data){
    IOWR_8DIRECT(0x18228, data);
}

static __inline HI_U8  hi_ext_system_drc_user_tm_update_read(HI_VOID){
    return IORD_8DIRECT(0x18228);
}

static __inline HI_VOID hi_ext_isp_stat_info_phyaddr_write(HI_U32 data){
   IOWR_32DIRECT(0x18229, data);
}

static __inline HI_U32 hi_ext_isp_stat_info_phyaddr_read(HI_VOID){
   return IORD_32DIRECT(0x18229);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_drc_auto_strength_max
// ------------------------------------------------------------------------------ //
static __inline HI_VOID hi_ext_system_drc_auto_strength_max_write(HI_U8 data){
   IOWR_8DIRECT(0x1822D, data);
}

static __inline HI_U8 hi_ext_system_drc_auto_strength_max_read(HI_VOID){
   return IORD_8DIRECT(0x1822D);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_drc_auto_strength_min
// ------------------------------------------------------------------------------ //
static __inline HI_VOID hi_ext_system_drc_auto_strength_min_write(HI_U8 data){
   IOWR_8DIRECT(0x1822E, data);
}

static __inline HI_U8 hi_ext_system_drc_auto_strength_min_read(HI_VOID){
   return IORD_8DIRECT(0x1822E);
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_drc_tm_lut_write len = 200
// ------------------------------------------------------------------------------ //
//0x18230 ~ 0x183C0
// args: data (16-bit) 
static __inline HI_VOID hi_ext_system_drc_tm_lut_write(HI_U8 u8Index, HI_U16 data){
    IOWR_16DIRECT(0x18230 + (u8Index*sizeof(HI_U16)), data);
}

static __inline HI_U16 hi_ext_system_drc_tm_lut_read(HI_U8 u8Index){
    return IORD_16DIRECT(0x18230 + (u8Index*sizeof(HI_U16)));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_defog_lut_write len = 256
// ------------------------------------------------------------------------------ //
//0x183D0~0x184D0
// args: data (8-bit) 
static __inline HI_VOID hi_ext_system_defog_lut_write(HI_U8 u8Index, HI_U8 data){
    IOWR_8DIRECT(0x183D0 + (u8Index*sizeof(HI_U8)), data);
}

static __inline HI_U8 hi_ext_system_defog_lut_read(HI_U8 u8Index){
    return IORD_8DIRECT(0x183D0 + (u8Index*sizeof(HI_U8)));
}
#define HI_EXT_SYSTEM_USER_DEFOG_LUT_ENABLE_DEFAULT  (0)
static __inline HI_VOID hi_ext_system_user_defog_lut_enable_write(HI_U8 data){
   IOWR_8DIRECT(0x184D4, data);
}
static __inline HI_U8 hi_ext_system_user_defog_lut_enable_read(HI_VOID){
   return IORD_8DIRECT(0x184D4);
}
#define HI_EXT_SYSTEM_USER_DEFOG_LUT_UPDATE_DEFAULT  (0)
static __inline HI_VOID hi_ext_system_user_defog_lut_update_write(HI_U8 data){
   IOWR_8DIRECT(0x184D8, data);
}
static __inline HI_U8 hi_ext_system_user_defog_lut_update_read(HI_VOID){
   return IORD_8DIRECT(0x184D8);
}
// ------------------------------------------------------------------------------ //
// Register: hi_ext_system__iso
// ------------------------------------------------------------------------------ //
// sys ISO
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_SYS_ISO_DEFAULT  (0x0)
#define HI_EXT_SYSTEM_SYS_ISO_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_ext_system_sys_iso_read(HI_VOID)
{
    return  IORD_32DIRECT(0x184DA);
}

static __inline HI_VOID hi_ext_system_sys_iso_write(HI_U32 data)
{
     IOWR_32DIRECT(0x184DA, data);
}
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __ISP_EXT_CONFIG_H__ */

