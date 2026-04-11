/******************************************************************************

  Copyright (C), 2004-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_config.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2014/09/22
  Description   : 
  History       :
  1.Date        : 2014/09/22
    Author      : q00214668
    Modification: Created file

******************************************************************************/

#ifndef __ISP_CONFIG_CONFIG_EX_H__
#define __ISP_CONFIG_CONFIG_EX_H__

#include "hi_vreg.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

static __inline HI_VOID hi_isp_module_dcg_sel_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x0088);
	IOWR_8DIRECT(0x0088 ,(data & 0x01) | (u8current & 0xfe));
}

static __inline HI_U8 hi_isp_module_dcg_sel_read(HI_VOID) {
	return (IORD_8DIRECT(0x0088) & 0x1);
}

#define HI_ISP_TOP_AE_SELECT_DEFAULT (0)
#define HI_ISP_TOP_AE_SELECT_DATASIZE (2)
    
#define HI_ISP_TOP_AE_SELECT_AFTER_DG (0)
#define HI_ISP_TOP_AE_SELECT_AFTER_STATIC_WB (1)
#define HI_ISP_TOP_AE_SELECT_AFTER_DRC (2)
static __inline HI_VOID hi_isp_module_ae_sel_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x0088);
	IOWR_8DIRECT(0x0088 ,((data & 0x03) << 1) | (u8current & 0xf9));
}

static __inline HI_U8 hi_isp_module_ae_sel_read(HI_VOID) {
	return ((IORD_8DIRECT(0x0088) & 0x6) >> 1);
}

static __inline HI_VOID hi_isp_module_af_sel_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x0088);
	IOWR_8DIRECT(0x0088 ,((data & 0x01)<< 3) | (u8current & 0xf7));
}

static __inline HI_U8 hi_isp_module_af_sel_read(HI_VOID) {
	return ((IORD_8DIRECT(0x0088) & 0x8) >> 3);
}

static __inline HI_VOID hi_isp_dmnr_dither_en_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x0098);
	IOWR_8DIRECT(0x0098 ,(data & 0x01) | (u8current & 0xfe));
}

static __inline HI_U8 hi_isp_dmnr_dither_en_read(HI_VOID) {
	return (IORD_8DIRECT(0x0098) & 0x1);
}

static __inline HI_VOID hi_isp_dmnr_dither_round_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x0098);
	IOWR_8DIRECT(0x0098 ,((data & 0x03) << 1) | (u8current & 0xf9));
}

static __inline HI_U8 hi_isp_dmnr_dither_round_read(HI_VOID) {
	return ((IORD_8DIRECT(0x0098) & 0x6) >> 1);
}

static __inline HI_VOID hi_isp_dmnr_dither_spatial_mode_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x0098);
	IOWR_8DIRECT(0x0098 ,((data & 0x1) << 3) | (u8current & 0xf7));
}

static __inline HI_U8 hi_isp_dmnr_dither_spatial_mode_read(HI_VOID) {
	return ((IORD_8DIRECT(0x0098) & 0x8) >> 3);
}

static __inline HI_VOID hi_isp_dmnr_dither_out_bits_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x0098);
	IOWR_8DIRECT(0x0098 ,((data & 0xf) << 4) | (u8current & 0xf));
}

static __inline HI_U8 hi_isp_dmnr_dither_out_bits_read(HI_VOID) {
	return ((IORD_8DIRECT(0x0098) & 0xf0) >> 4);
}

static __inline HI_VOID hi_isp_drc_moudle_dither_en_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x009c);
	IOWR_8DIRECT(0x009c ,(data & 0x01) | (u8current & 0xfe));
}

static __inline HI_U8 hi_isp_drc_moudle_dither_en_read(HI_VOID) {
	return (IORD_8DIRECT(0x009c) & 0x1);
}

static __inline HI_VOID hi_isp_drc_dither_round_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x009c);
	IOWR_8DIRECT(0x009c ,((data & 0x03) << 1) | (u8current & 0xf9));
}

static __inline HI_U8 hi_isp_drc_dither_round_read(HI_VOID) {
	return ((IORD_8DIRECT(0x009c) & 0x6) >> 1);
}

static __inline HI_VOID hi_isp_drc_dither_spatial_mode_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x009c);
	IOWR_8DIRECT(0x009c ,((data & 0x1) << 3) | (u8current & 0xf7));
}

static __inline HI_U8 hi_isp_drc_dither_spatial_mode_read(HI_VOID) {
	return ((IORD_8DIRECT(0x009c) & 0x8) >> 3);
}

static __inline HI_VOID hi_isp_drc_dither_out_bits_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x009c);
	IOWR_8DIRECT(0x009c ,((data & 0xf) << 4) | (u8current & 0xf));
}

static __inline HI_U8 hi_isp_drc_dither_out_bits_read(HI_VOID) {
	return ((IORD_8DIRECT(0x009c) & 0xf0) >> 4);
}

static __inline HI_VOID hi_isp_dci_dither_en_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x00a0);
	IOWR_8DIRECT(0x00a0 ,(data & 0x01) | (u8current & 0xfe));
}

static __inline HI_U8 hi_isp_dci_dither_en_read(HI_VOID) {
	return (IORD_8DIRECT(0x00a0) & 0x1);
}

static __inline HI_VOID hi_isp_dci_dither_round_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x00a0);
	IOWR_8DIRECT(0x00a0 ,((data & 0x03) << 1) | (u8current & 0xf9));
}

static __inline HI_U8 hi_isp_dci_dither_round_read(HI_VOID) {
	return ((IORD_8DIRECT(0x00a0) & 0x6) >> 1);
}

static __inline HI_VOID hi_isp_dci_dither_spatial_mode_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x00a0);
	IOWR_8DIRECT(0x00a0 ,((data & 0x1) << 3) | (u8current & 0xf7));
}

static __inline HI_U8 hi_isp_dci_dither_spatial_mode_read(HI_VOID) {
	return ((IORD_8DIRECT(0x00a0) & 0x8) >> 3);
}

static __inline HI_VOID hi_isp_dci_dither_out_bits_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x00a0);
	IOWR_8DIRECT(0x00a0 ,((data & 0xf) << 4) | (u8current & 0xf));
}

static __inline HI_U8 hi_isp_dci_dither_out_bits_read(HI_VOID) {
	return ((IORD_8DIRECT(0x00a0) & 0xf0) >> 4);
}

static __inline HI_VOID hi_isp_pix_format_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x00a4);
	IOWR_8DIRECT(0x00a4 ,(data & 0x3) | (u8current & 0xfc));
}

static __inline HI_U8 hi_isp_pix_format_read(HI_VOID) {
	return (IORD_8DIRECT(0x00a4) & 0x3);
}


#define HI_ISP_CPI_FIX_TIMING_STAT (200)

static __inline HI_VOID hi_isp_yuv444_timing_stat_write(HI_U16 data) {
	IOWR_16DIRECT(0x01e8 ,((data&0x1fff) << 1));
}

static __inline HI_U16 hi_isp_yuv444_timing_stat_read(HI_VOID) {
	return ((IORD_16DIRECT(0x01e8) >> 1) & 0x1fff);
}

static __inline HI_VOID hi_isp_yuv422_timing_stat_write(HI_U16 data) {
	IOWR_16DIRECT(0x02e8 ,((data&0x1fff) << 1));
}

static __inline HI_U16 hi_isp_yuv422_timing_stat_read(HI_VOID) {
	return ((IORD_16DIRECT(0x02e8) >> 1) & 0x1fff);
}

static __inline HI_VOID hi_isp_yuv444_width_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x01f0);
	IOWR_16DIRECT(0x01f0, (data & 0xfff) | (u16current & 0xf000));
}
static __inline HI_U16 hi_isp_yuv444_width_read(HI_VOID) {
    return (IORD_16DIRECT(0x01f0) & 0xfff);
}

static __inline HI_VOID hi_isp_yuv444_height_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x01f2);
	IOWR_16DIRECT(0x01f2, (data & 0xfff) | (u16current & 0xf000));
}
static __inline HI_U16 hi_isp_yuv444_height_read(HI_VOID) {
    return (IORD_16DIRECT(0x01f2) & 0xfff);
}

static __inline HI_VOID hi_isp_yuv422_width_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x02f0);
	IOWR_16DIRECT(0x02f0, (data & 0xfff) | (u16current & 0xf000));
}
static __inline HI_U16 hi_isp_yuv422_width_read(HI_VOID) {
    return (IORD_16DIRECT(0x02f0) & 0xfff);
}

static __inline HI_VOID hi_isp_yuv422_height_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x02f2);
	IOWR_16DIRECT(0x02f2, (data & 0xfff) | (u16current & 0xf000));
}
static __inline HI_U16 hi_isp_yuv422_height_read(HI_VOID) {
    return (IORD_16DIRECT(0x02f2) & 0xfff);
}

static __inline HI_VOID hi_isp_yuv444_rggb_start_write(HI_U8 data) {
	IOWR_8DIRECT(0x01E0, (data & 0x03));   
}
static __inline HI_U8 hi_isp_yuv444_rggb_start_read(HI_VOID) {
	return (IORD_8DIRECT(0x01E0) & 0x03); 
}

static __inline HI_VOID hi_isp_yuv422_rggb_start_write(HI_U8 data) {
	IOWR_8DIRECT(0x02E0, (data & 0x03));   
}
static __inline HI_U8 hi_isp_yuv422_rggb_start_read(HI_VOID) {
	return (IORD_8DIRECT(0x02E0) & 0x03); 
}

static __inline HI_VOID hi_isp_ae_cfg_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x2000, (data & 0x01));
}

static __inline HI_U8 hi_isp_ae_cfg_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x2000) & 0x1);
}

#define HI_ISP_AE_VERSION_V100 (1)
static __inline HI_VOID hi_isp_ae_version_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x200e);
    IOWR_16DIRECT(0x200e, ((data & 0xfff) << 4) | (u16current & 0xf));
}

static __inline HI_U16 hi_isp_ae_version_read(HI_VOID) {
    return ((IORD_16DIRECT(0x200e) & 0xfff0) >> 4);
}

static __inline HI_VOID hi_isp_ae_date_write(HI_U16 data) {
    IOWR_16DIRECT(0x200c, data);
}

static __inline HI_U16 hi_isp_ae_date_read(HI_VOID) {
    return (IORD_16DIRECT(0x200c));
}

static __inline HI_VOID hi_isp_ae_h_zone_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x2010);
    IOWR_8DIRECT(0x2010, ((data & 0x1f) | (u8current & 0xe0)));
}

static __inline HI_U8 hi_isp_ae_h_zone_read(HI_VOID) {
    return (IORD_8DIRECT(0x2010) & 0x1f);
}

static __inline HI_VOID hi_isp_ae_v_zone_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x2011);
    IOWR_8DIRECT(0x2011, ((data & 0x1f) | (u8current & 0xe0)));
}

static __inline HI_U8 hi_isp_ae_v_zone_read(HI_VOID) {
    return (IORD_8DIRECT(0x2011) & 0x1f);
}

static __inline HI_VOID hi_isp_ae_pixel_selected_count_write(HI_U32 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2018);
	IOWR_32DIRECT(0x2018, ((data & 0xffffff) | (u32current & 0xff000000)));
}

static __inline HI_U32 hi_isp_ae_pixel_selected_count_read(HI_VOID) {
	return (IORD_32DIRECT(0x2018) & 0xffffff);
}

static __inline HI_VOID hi_isp_ae_pixel_selected_weight_write(HI_U32 data) {
	HI_U32 u32current = IORD_32DIRECT(0x201c);
	IOWR_32DIRECT(0x201c, ((data & 0xfffffff) | (u32current & 0xf0000000)));
}

static __inline HI_U32 hi_isp_ae_pixel_selected_weight_read(HI_VOID) {
	return (IORD_32DIRECT(0x201c) & 0xfffffff);
}

static __inline HI_VOID hi_isp_ae_total_r_aver_write(HI_U32 data) {
	IOWR_32DIRECT(0x2020, data);
}

static __inline HI_U32 hi_isp_ae_total_r_aver_read(HI_VOID) {
	return IORD_32DIRECT(0x2020);
}

static __inline HI_VOID hi_isp_ae_total_gr_aver_write(HI_U32 data) {
	IOWR_32DIRECT(0x2024, data);
}

static __inline HI_U32 hi_isp_ae_total_gr_aver_read(HI_VOID) {
	return IORD_32DIRECT(0x2024);
}

static __inline HI_VOID hi_isp_ae_total_gb_aver_write(HI_U32 data) {
	IOWR_32DIRECT(0x2028, data);
}

static __inline HI_U32 hi_isp_ae_total_gb_aver_read(HI_VOID) {
	return IORD_32DIRECT(0x2028);
}

static __inline HI_VOID hi_isp_ae_total_b_aver_write(HI_U32 data) {
	IOWR_32DIRECT(0x202c, data);
}

static __inline HI_U32 hi_isp_ae_total_b_aver_read(HI_VOID) {
	return IORD_32DIRECT(0x202c);
}

static __inline HI_VOID hi_isp_ae_high_hist_weight_write(HI_U32 data) {
	IOWR_32DIRECT(0x2030, data);
}

static __inline HI_U32 hi_isp_ae_high_hist_weight_read(HI_VOID) {
	return IORD_32DIRECT(0x2030);
}

static __inline HI_VOID hi_isp_ae_bit_move_gamma_write(HI_U32 data) {
HI_U32 u32current = IORD_32DIRECT(0x2040);
IOWR_32DIRECT(0x2040, ((data & 0xfff) | (u32current & 0xfffff000)));
}

static __inline HI_U32 hi_isp_ae_bit_move_gamma_read(HI_VOID) {
return (IORD_32DIRECT(0x2040) & 0xfff);
}

static __inline HI_VOID hi_isp_ae_mem_hist_raddr_write(HI_U32 data) {
	IOWR_32DIRECT(0x2088, data);
}

static __inline HI_U32 hi_isp_ae_mem_hist_raddr_read(HI_VOID) {
	return IORD_32DIRECT(0x2088);
}

static __inline HI_VOID hi_isp_ae_mem_hist_rdata_write(HI_U32 data) {
	IOWR_32DIRECT(0x208c, data);
}

static __inline HI_U32 hi_isp_ae_mem_hist_rdata_read(HI_VOID) {
	return IORD_32DIRECT(0x208c);
}

static __inline HI_VOID hi_isp_ae_mem_aver_raddr_write(HI_U32 data) {
	IOWR_32DIRECT(0x2098, data);
}

static __inline HI_U32 hi_isp_ae_mem_aver_raddr_read(HI_VOID) {
	return IORD_32DIRECT(0x2098);
}

static __inline HI_VOID hi_isp_ae_mem_aver_rdata_write(HI_U32 data) {
	IOWR_32DIRECT(0x209c, data);
}

static __inline HI_U32 hi_isp_ae_mem_aver_rdata_read(HI_VOID) {
	return IORD_32DIRECT(0x209c);
}

static __inline HI_VOID hi_isp_ae_mem_wei_waddr_write(HI_U32 data) {
	IOWR_32DIRECT(0x20a0, data);
}

static __inline HI_U32 hi_isp_ae_mem_wei_waddr_read(HI_VOID) {
	return IORD_32DIRECT(0x20a0);
}

static __inline HI_VOID hi_isp_ae_mem_wei_wdata_write(HI_U32 data) {
	IOWR_32DIRECT(0x20a4, data);
}

static __inline HI_U32 hi_isp_ae_mem_wei_wdata_read(HI_VOID) {
	return IORD_32DIRECT(0x20a4);
}

static __inline HI_VOID hi_isp_ae_mem_wei_raddr_write(HI_U32 data) {
	IOWR_32DIRECT(0x20a8, data);
}

static __inline HI_U32 hi_isp_ae_mem_wei_raddr_read(HI_VOID) {
	return IORD_32DIRECT(0x20a8);
}

static __inline HI_VOID hi_isp_ae_mem_wei_rdata_write(HI_U32 data) {
	IOWR_32DIRECT(0x20ac, data);
}

static __inline HI_U32 hi_isp_ae_mem_wei_rdata_read(HI_VOID) {
	return IORD_32DIRECT(0x20ac);
}

static __inline HI_VOID hi_isp_ae_ctrl_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x20e0); 
	IOWR_8DIRECT(0x20e0, (data & 0x3) | (u8current & 0xfc));
}

static __inline HI_U8 hi_isp_ae_ctrl_read(HI_VOID) {
	return (IORD_8DIRECT(0x20e0) & 0x3);
}

static __inline HI_VOID hi_isp_ae_update_mode_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x20e4); 
	IOWR_8DIRECT(0x20e4, (data & 0x1) | (u8current & 0xfe));
}

static __inline HI_U8 hi_isp_ae_update_mode_read(HI_VOID) {
	return (IORD_8DIRECT(0x20e4) & 0x1);
}

static __inline HI_VOID hi_isp_ae_update_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x20ec); 
	IOWR_8DIRECT(0x20ec, (data & 0x1) | (u8current & 0xfe));
}

static __inline HI_U8 hi_isp_ae_update_read(HI_VOID) {
	return (IORD_8DIRECT(0x20ec) & 0x1);
}

static __inline HI_VOID hi_isp_ae_width_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x20f0); 
	IOWR_16DIRECT(0x20f0, (data & 0xfff) | (u16current & 0xf000));
}

static __inline HI_U16 hi_isp_ae_width_read(HI_VOID) {
	return (IORD_16DIRECT(0x20f0) & 0xfff);
}

static __inline HI_VOID hi_isp_ae_height_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x20f2); 
	IOWR_16DIRECT(0x20f2, (data & 0xfff) | (u16current & 0xf000));
}

static __inline HI_U16 hi_isp_ae_height_read(HI_VOID) {
	return (IORD_16DIRECT(0x20f2) & 0xfff);
}

/*****************************************************
MODULE        -----AWB-----
MODULE        -----AWB-----
MODULE        -----AWB-----
*****************************************************/
static __inline HI_VOID hi_isp_awb_cfg_enable_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x2100);
    IOWR_8DIRECT(0x2100, (data & 0x01) | (u8current & 0xfe));
}

static __inline HI_U8 hi_isp_awb_cfg_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x2100) & 0x1);
}

#define HI_ISP_AWB_VERSION_V100 (1)
static __inline HI_VOID hi_isp_awb_version_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x210e);
    IOWR_16DIRECT(0x210e, ((data & 0xfff) << 4) | (u16current & 0xf));
}

static __inline HI_U16 hi_isp_awb_version_read(HI_VOID) {
    return ((IORD_16DIRECT(0x210e) & 0xfff0) >> 4);
}

static __inline HI_VOID hi_isp_awb_date_write(HI_U16 data) {
    IOWR_16DIRECT(0x210c, data);
}

static __inline HI_U16 hi_isp_awb_date_read(HI_VOID) {
    return (IORD_16DIRECT(0x210c));
}

static __inline HI_VOID hi_isp_awb_h_zone_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x2110);
    IOWR_8DIRECT(0x2110, ((data & 0x1f) | (u8current & 0xe0)));
}

static __inline HI_U8 hi_isp_awb_h_zone_read(HI_VOID) {
    return (IORD_8DIRECT(0x2110) & 0x1f);
}

static __inline HI_VOID hi_isp_awb_v_zone_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x2111);
    IOWR_8DIRECT(0x2111, ((data & 0x1f) | (u8current & 0xe0)));
}

static __inline HI_U8 hi_isp_awb_v_zone_read(HI_VOID) {
    return (IORD_8DIRECT(0x2111) & 0x1f);
}

static __inline HI_VOID hi_isp_awb_thd_min_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x2118);
    IOWR_16DIRECT(0x2118, ((data & 0xfff) | (u16current & 0xf000)));
}

static __inline HI_U16 hi_isp_awb_thd_min_read(HI_VOID) {
    return (IORD_16DIRECT(0x2118) & 0xfff);
}

static __inline HI_VOID hi_isp_awb_thd_max_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x211c);
    IOWR_16DIRECT(0x211c, ((data & 0xfff) | (u16current & 0xf000)));
}

static __inline HI_U16 hi_isp_awb_thd_max_read(HI_VOID) {
    return (IORD_16DIRECT(0x211c) & 0xfff);
}

static __inline HI_VOID hi_isp_awb_cr_ref_max_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x2120);
    IOWR_16DIRECT(0x2120, ((data & 0xfff) | (u16current & 0xf000)));
}

static __inline HI_U16 hi_isp_awb_cr_ref_max_read(HI_VOID) {
    return (IORD_16DIRECT(0x2120) & 0xfff);
}

static __inline HI_VOID hi_isp_awb_cr_ref_min_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x2122);
    IOWR_16DIRECT(0x2122, ((data & 0xfff) | (u16current & 0xf000)));
}

static __inline HI_U16 hi_isp_awb_cr_ref_min_read(HI_VOID) {
    return (IORD_16DIRECT(0x2122) & 0xfff);
}

static __inline HI_VOID hi_isp_awb_cb_ref_max_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x2124);
    IOWR_16DIRECT(0x2124, ((data & 0xfff) | (u16current & 0xf000)));
}

static __inline HI_U16 hi_isp_awb_cb_ref_max_read(HI_VOID) {
    return (IORD_16DIRECT(0x2124) & 0xfff);
}

static __inline HI_VOID hi_isp_awb_cb_ref_min_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x2126);
    IOWR_16DIRECT(0x2126, ((data & 0xfff) | (u16current & 0xf000)));
}

static __inline HI_U16 hi_isp_awb_cb_ref_min_read(HI_VOID) {
    return (IORD_16DIRECT(0x2126) & 0xfff);
}

static __inline HI_VOID hi_isp_awb_top_hypotenuse_k_write(HI_U32 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2130);
    IOWR_32DIRECT(0x2130, ((data & 0x1ffffff) | (u32current & 0xfe000000)));
}

static __inline HI_U32 hi_isp_awb_top_hypotenuse_k_read(HI_VOID) {
    return (IORD_32DIRECT(0x2130) & 0x1ffffff);
}

static __inline HI_VOID hi_isp_awb_top_hypotenuse_b_write(HI_U32 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2134);
    IOWR_32DIRECT(0x2134, ((data & 0xffffff) | (u32current & 0xff000000)));
}

static __inline HI_U32 hi_isp_awb_top_hypotenuse_b_read(HI_VOID) {
    return (IORD_32DIRECT(0x2134) & 0xffffff);
}

static __inline HI_VOID hi_isp_awb_bot_hypotenuse_k_write(HI_U32 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2138);
    IOWR_32DIRECT(0x2138, ((data & 0x1ffffff) | (u32current & 0xfe000000)));
}

static __inline HI_U32 hi_isp_awb_bot_hypotenuse_k_read(HI_VOID) {
    return (IORD_32DIRECT(0x2138) & 0x1ffffff);
}

static __inline HI_VOID hi_isp_awb_bot_hypotenuse_b_write(HI_U32 data) {
	HI_U32 u32current = IORD_32DIRECT(0x213c);
    IOWR_32DIRECT(0x213c, ((data & 0xffffff) | (u32current & 0xff000000)));
}

static __inline HI_U32 hi_isp_awb_bot_hypotenuse_b_read(HI_VOID) {
    return (IORD_32DIRECT(0x213c) & 0xffffff);
}

static __inline HI_VOID hi_isp_awb_avg_r_write(HI_U16 data) {
    IOWR_16DIRECT(0x2140, data);
}

static __inline HI_U16 hi_isp_awb_avg_r_read(HI_VOID) {
    return IORD_16DIRECT(0x2140);
}

static __inline HI_VOID hi_isp_awb_avg_g_write(HI_U16 data) {
    IOWR_16DIRECT(0x2144, data);
}

static __inline HI_U16 hi_isp_awb_avg_g_read(HI_VOID) {
    return IORD_16DIRECT(0x2144);
}

static __inline HI_VOID hi_isp_awb_avg_b_write(HI_U16 data) {
    IOWR_16DIRECT(0x2148, data);
}

static __inline HI_U16 hi_isp_awb_avg_b_read(HI_VOID) {
    return IORD_16DIRECT(0x2148);
}

static __inline HI_VOID hi_isp_awb_count_all_write(HI_U16 data) {
    IOWR_16DIRECT(0x214c, data);
}

static __inline HI_U16 hi_isp_awb_count_all_read(HI_VOID) {
    return IORD_16DIRECT(0x214c);
}

static __inline HI_VOID hi_isp_awb_count_min_write(HI_U16 data) {
    IOWR_16DIRECT(0x2150, data);
}

static __inline HI_U16 hi_isp_awb_count_min_read(HI_VOID) {
    return IORD_16DIRECT(0x2150);
}

static __inline HI_VOID hi_isp_awb_count_max_write(HI_U16 data) {
    IOWR_16DIRECT(0x2154, data);
}

static __inline HI_U16 hi_isp_awb_count_max_read(HI_VOID) {
    return IORD_16DIRECT(0x2154);
}

static __inline HI_VOID hi_isp_awb_stat_raddr_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x2188);
    IOWR_16DIRECT(0x2188, (data & 0x3ff) | (u16current & 0xfc00));
}

static __inline HI_U16 hi_isp_awb_stat_raddr_read(HI_VOID) {
    return (IORD_16DIRECT(0x2188) & 0x3ff);
}

static __inline HI_VOID hi_isp_awb_stat_rdata_write(HI_U32 data) {
    IOWR_32DIRECT(0x218c, data);
}

static __inline HI_U32 hi_isp_awb_stat_rdata_read(HI_VOID) {
    return IORD_32DIRECT(0x218c);
}

static __inline HI_VOID hi_isp_awb_rggb_cfg_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x21e0);
    IOWR_8DIRECT(0x21e0, (data & 0x3) | (u8current & 0xfc));
}

static __inline HI_U8 hi_isp_awb_rggb_cfg_read(HI_VOID) {
    return (IORD_8DIRECT(0x21e0) & 0x3);
}

static __inline HI_VOID hi_isp_awb_update_mode_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x21e4);
    IOWR_8DIRECT(0x21e4, (data & 0x1) | (u8current & 0xfe));
}

static __inline HI_U8 hi_isp_awb_update_mode_read(HI_VOID) {
    return (IORD_8DIRECT(0x21e4) & 0x1);
}

static __inline HI_VOID hi_isp_awb_update_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x21ec);
    IOWR_8DIRECT(0x21ec, (data & 0x1) | (u8current & 0xfe));
}

static __inline HI_U8 hi_isp_awb_update_read(HI_VOID) {
    return (IORD_8DIRECT(0x21ec) & 0x1);
}

static __inline HI_VOID hi_isp_awb_hsize_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x21f0);
    IOWR_16DIRECT(0x21f0, (data & 0xfff) | (u16current & 0xf000));
}

static __inline HI_U16 hi_isp_awb_hsize_read(HI_VOID) {
    return (IORD_16DIRECT(0x21f0) & 0xfff);
}

static __inline HI_VOID hi_isp_awb_vsize_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x21f2);
    IOWR_16DIRECT(0x21f2, (data & 0xfff) | (u16current & 0xf000));
}

static __inline HI_U16 hi_isp_awb_vsize_read(HI_VOID) {
    return (IORD_16DIRECT(0x21f2) & 0xfff);
}

static __inline HI_VOID hi_isp_wb_enable_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x1100);
    IOWR_8DIRECT(0x1100, (data & 0x01) | (u8current & 0xfe));
}

static __inline HI_U8 hi_isp_wb_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x1100) & 0x1);
}

/*****************************************************
MODULE        -----CC-----
MODULE        -----CC-----
MODULE        -----CC-----
*****************************************************/
static __inline HI_VOID hi_isp_cc_enable_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x1200);
    IOWR_8DIRECT(0x1200, (data & 0x01) | (u8current & 0xfe));
}

static __inline HI_U8 hi_isp_cc_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x1200) & 0x1);
}

#define HI_ISP_CC_IN_DC0_DEFAULT (0)
#define HI_ISP_CC_IN_DC1_DEFAULT (0)
#define HI_ISP_CC_IN_DC2_DEFAULT (0)

static __inline HI_VOID hi_isp_cc_in_dc1_write(HI_U16 data) {
    IOWR_16DIRECT(0x1232, ((data & 0x7ff) << 5));
}

static __inline HI_U16 hi_isp_cc_in_dc1_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1232) & 0xffe0) >> 5);
}

static __inline HI_VOID hi_isp_cc_in_dc0_write(HI_U16 data) {
    IOWR_16DIRECT(0x1230, ((data & 0x7ff) << 5));
}

static __inline HI_U16 hi_isp_cc_in_dc0_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1230) & 0xffe0) >> 5);
}

static __inline HI_VOID hi_isp_cc_in_dc2_write(HI_U16 data) {
    IOWR_16DIRECT(0x1234, ((data & 0x7ff) << 5));
}

static __inline HI_U16 hi_isp_cc_in_dc2_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1234) & 0xffe0) >> 5);
}

#define HI_ISP_CC_OUT_DC0_DEFAULT (0)
#define HI_ISP_CC_OUT_DC1_DEFAULT (0)
#define HI_ISP_CC_OUT_DC2_DEFAULT (0)

static __inline HI_VOID hi_isp_cc_out_dc1_write(HI_U16 data) {
    IOWR_16DIRECT(0x1242, ((data & 0x7ff) << 5));
}

static __inline HI_U16 hi_isp_cc_out_dc1_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1242) & 0xffe0) >> 5);
}

static __inline HI_VOID hi_isp_cc_out_dc0_write(HI_U16 data) {
    IOWR_16DIRECT(0x1240, ((data & 0x7ff) << 5));
}

static __inline HI_U16 hi_isp_cc_out_dc0_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1240) & 0xffe0) >> 5);
}

static __inline HI_VOID hi_isp_cc_out_dc2_write(HI_U16 data) {
    IOWR_16DIRECT(0x1244, ((data & 0x7ff) << 5));
}

static __inline HI_U16 hi_isp_cc_out_dc2_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1244) & 0xffe0) >> 5);
}

/*****************************************************
MODULE        -----CAC-----
MODULE        -----CAC-----
MODULE        -----CAC-----
*****************************************************/

static __inline HI_VOID hi_isp_cac_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x5900, (data & 0x1));
}

static __inline HI_U8 hi_isp_cac_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x5900) & 0x1);
}

static __inline HI_VOID hi_isp_cac_inter_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x5904, (data & 0x1));
}

static __inline HI_U8 hi_isp_cac_inter_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x5904) & 0x1);
}

static __inline HI_VOID hi_isp_cac_version_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x590e);
    IOWR_16DIRECT(0x590e, (data << 4) | (u16current&0xf));
}

static __inline HI_U16 hi_isp_cac_version_read(HI_VOID) {
    return (IORD_16DIRECT(0x590e) >> 4);
}

static __inline HI_VOID hi_isp_cac_date_write(HI_U16 data) {
    IOWR_16DIRECT(0x590c, data);
}

static __inline HI_U16 hi_isp_cac_date_read(HI_VOID) {
    return (IORD_16DIRECT(0x590c));
}

static __inline HI_VOID hi_isp_cac_sample_ratio_v_write(HI_U8 data) {
    IOWR_8DIRECT(0x5916, (data & 0xf));
}

static __inline HI_U8 hi_isp_cac_sample_ratio_v_read(HI_VOID) {
    return (IORD_8DIRECT(0x5916) & 0xf);
}

static __inline HI_VOID hi_isp_cac_sample_ratio_h_write(HI_U8 data) {
    IOWR_8DIRECT(0x5914, (data & 0xf));
}

static __inline HI_U8 hi_isp_cac_sample_ratio_h_read(HI_VOID) {
    return (IORD_8DIRECT(0x5914) & 0xf);
}

static __inline HI_VOID hi_isp_cac_h_waddr_write(HI_U32 data) { 
    IOWR_32DIRECT(0x5980, data);
}

static __inline HI_U32 hi_isp_cac_h_waddr_read(HI_VOID) {
    return (IORD_32DIRECT(0x5980));
}

static __inline HI_VOID hi_isp_cac_h_wdata_write(HI_U32 data) { 
    IOWR_32DIRECT(0x5984, data);
}

static __inline HI_U32 hi_isp_cac_h_wdata_read(HI_VOID) {
    return (IORD_32DIRECT(0x5984));
}

static __inline HI_VOID hi_isp_cac_h_raddr_write(HI_U32 data) { 
    IOWR_32DIRECT(0x5988, data);
}

static __inline HI_U32 hi_isp_cac_h_raddr_read(HI_VOID) {
    return (IORD_32DIRECT(0x5988));
}

static __inline HI_VOID hi_isp_cac_h_rdata_write(HI_U32 data) { 
    IOWR_32DIRECT(0x598c, data);
}

static __inline HI_U32 hi_isp_cac_h_rdata_read(HI_VOID) {
    return (IORD_32DIRECT(0x598c));
}

static __inline HI_VOID hi_isp_cac_v_waddr_write(HI_U32 data) { 
    IOWR_32DIRECT(0x5990, data);
}

static __inline HI_U32 hi_isp_cac_v_waddr_read(HI_VOID) {
    return (IORD_32DIRECT(0x5990));
}

static __inline HI_VOID hi_isp_cac_v_wdata_write(HI_U32 data) { 
    IOWR_32DIRECT(0x5994, data);
}

static __inline HI_U32 hi_isp_cac_v_wdata_read(HI_VOID) {
    return (IORD_32DIRECT(0x5994));
}

static __inline HI_VOID hi_isp_cac_v_raddr_write(HI_U32 data) { 
    IOWR_32DIRECT(0x5998, data);
}

static __inline HI_U32 hi_isp_cac_v_raddr_read(HI_VOID) {
    return (IORD_32DIRECT(0x5998));
}

static __inline HI_VOID hi_isp_cac_v_rdata_write(HI_U32 data) { 
    IOWR_32DIRECT(0x599c, data);
}

static __inline HI_U32 hi_isp_cac_v_rdata_read(HI_VOID) {
    return (IORD_32DIRECT(0x599c));
}

/*****************************************************
MODULE        -----COMPANDER-----
MODULE        -----COMPANDER-----
MODULE        -----COMPANDER-----
*****************************************************/
/*****************************************************
ISP_COMPANDER_CFG:         Enable
0x205a7500:                     31:1      reserved
                                          0       COMPANDER Enable
*****************************************************/
static __inline HI_VOID hi_isp_compander_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x7500, data);
}

static __inline HI_U8 hi_isp_compander_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x7500));
}

/*****************************************************
ISP_GAMMAFE_VERSION:  COMPANDER Version
0x205a750c:                     31:20      version
                                          19:16      reserved
                                          15: 0       date
*****************************************************/
static __inline HI_VOID hi_isp_compander_version_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x400e);
    IOWR_16DIRECT(0x750e, (data << 4) | (u16current&0xf));
}

static __inline HI_U16 hi_isp_compander_version_read(HI_VOID) {
    return (IORD_16DIRECT(0x750e) >> 4);
}

static __inline HI_VOID hi_isp_compander_date_write(HI_U16 data) {
    IOWR_16DIRECT(0x750c, data);
}

static __inline HI_U16 hi_isp_compander_date_read(HI_VOID) {
    return (IORD_16DIRECT(0x750c));
}

/*****************************************************
ISP_COMPANDER_BITW:   COMPANDER Bit Width
0x205a7510:                     31:21      reserved
                                          20:16      bitw_in
                                          15:05      reserved
                                          04:00      bitw_out
*****************************************************/
static __inline HI_VOID hi_isp_compander_bitw_in_write(HI_U8 data) {
    IOWR_8DIRECT(0x7512, (data & 0x1f));
}

static __inline HI_U8 hi_isp_compander_bitw_in_read(HI_VOID) {
    return (IORD_8DIRECT(0x7512) & 0x1f);
}

static __inline HI_VOID hi_isp_compander_bitw_out_write(HI_U8 data) {
    IOWR_8DIRECT(0x7510, (data & 0x1f));
}

static __inline HI_U8 hi_isp_compander_bitw_out_read(HI_VOID) {
    return (IORD_8DIRECT(0x7510) & 0x1f);
}

/*****************************************************
ISP_COMPANDER_OFFSETR:   COMPANDER Roffset
0x205a7514:                     31:20      reserved
                                          19:00      Roffset
*****************************************************/
static __inline HI_VOID hi_isp_compander_OFFSETR_in_write(HI_U32 data) {
    IOWR_32DIRECT(0x7514, (data & 0xfffff));
}

static __inline HI_U32 hi_isp_compander_OFFSETR_in_read(HI_VOID) {
    return (IORD_32DIRECT(0x7514) & 0xfffff);
}

/*****************************************************
ISP_COMPANDER_OFFSETGR:   COMPANDER GRoffset
0x205a7518:                     31:20      reserved
                                          19:00      GRoffset
*****************************************************/
static __inline HI_VOID hi_isp_compander_OFFSETGR_in_write(HI_U32 data) {
    IOWR_32DIRECT(0x7518, (data & 0xfffff));
}

static __inline HI_U32 hi_isp_compander_OFFSETGR_in_read(HI_VOID) {
    return (IORD_32DIRECT(0x7518) & 0xfffff);
}

/*****************************************************
ISP_COMPANDER_OFFSETGB:   COMPANDER GBoffset
0x205a751c:                     31:20      reserved
                                          19:00      GBoffset
*****************************************************/
static __inline HI_VOID hi_isp_compander_OFFSETGB_in_write(HI_U32 data) {
    IOWR_32DIRECT(0x751c, (data & 0xfffff));
}

static __inline HI_U32 hi_isp_compander_OFFSETGB_in_read(HI_VOID) {
    return (IORD_32DIRECT(0x751c) & 0xfffff);
}

/*****************************************************
ISP_COMPANDER_OFFSETB:   COMPANDER Boffset
0x205a7520:                     31:20      reserved
                                          19:00      Boffset
*****************************************************/
static __inline HI_VOID hi_isp_compander_OFFSETB_in_write(HI_U32 data) {
    IOWR_32DIRECT(0x7520, (data & 0xfffff));
}

static __inline HI_U32 hi_isp_compander_OFFSETB_in_read(HI_VOID) {
    return (IORD_32DIRECT(0x7520) & 0xfffff);
}

/*****************************************************
ISP_COMPANDER_WADDR:         COMPANDER 129 LUT write addr
0x205a7580:                     31:00      compander_waddr
*****************************************************/
static __inline HI_VOID hi_isp_compander_waddr_write(HI_U32 data) {
    IOWR_32DIRECT(0x7580, data);
}

static __inline HI_U32 hi_isp_compander_waddr_read(HI_VOID) {
    return (IORD_32DIRECT(0x7580));
}

/*****************************************************
ISP_COMPANDER_WDATA:  COMPANDER 129 LUT write data
0x205a7584:                     31:00      compander_wdata
*****************************************************/
static __inline HI_VOID hi_isp_compander_wdata_write(HI_U32 data) {
    IOWR_32DIRECT(0x7584, data);
}

static __inline HI_U32 hi_isp_compander_wdata_read(HI_VOID) {
    return (IORD_32DIRECT(0x7584));
}

/*****************************************************
ISP_COMPANDER_RADDR:         COMPANDER 129 LUT read addr
0x205a7588:                     31:00      compander_raddr
*****************************************************/
static __inline HI_VOID hi_isp_compander_raddr_write(HI_U32 data) {
    IOWR_32DIRECT(0x7588, data);
}

static __inline HI_U32 hi_isp_compander_raddr_read(HI_VOID) {
    return (IORD_32DIRECT(0x7588));
}

/*****************************************************
ISP_COMPANDER_RDATA:  COMPANDER 129 LUT read data
0x205a758c:                     31:00      compander_rdata
*****************************************************/
static __inline HI_VOID hi_isp_compander_rdata_write(HI_U32 data) {
    IOWR_32DIRECT(0x758c, data);
}

static __inline HI_U32 hi_isp_compander_rdata_read(HI_VOID) {
    return (IORD_32DIRECT(0x758c));
}

/*****************************************************
MODULE        -----CSC-----
MODULE        -----CSC-----
MODULE        -----CSC-----
*****************************************************/
static __inline HI_VOID hi_isp_csc_enable_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x1300);
    IOWR_8DIRECT(0x1300, (data & 0x01) | (u8current & 0xfe));
}

static __inline HI_U8 hi_isp_csc_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x1300) & 0x1);
}

static __inline HI_VOID hi_isp_csc_coef01_write(HI_U16 data) {
    IOWR_16DIRECT(0x1312, ((data & 0x7fff) << 1));
}

static __inline HI_U16 hi_isp_csc_coef01_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1312) & 0xfffe) >> 1);
}

static __inline HI_VOID hi_isp_csc_coef00_write(HI_U16 data) {
    IOWR_16DIRECT(0x1310, ((data & 0x7fff) << 1));
}

static __inline HI_U16 hi_isp_csc_coef00_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1310) & 0xfffe) >> 1);
}

static __inline HI_VOID hi_isp_csc_coef10_write(HI_U16 data) {
    IOWR_16DIRECT(0x1316, ((data & 0x7fff) << 1));
}

static __inline HI_U16 hi_isp_csc_coef10_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1316) & 0xfffe) >> 1);
}

static __inline HI_VOID hi_isp_csc_coef02_write(HI_U16 data) {
    IOWR_16DIRECT(0x1314, ((data & 0x7fff) << 1));
}

static __inline HI_U16 hi_isp_csc_coef02_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1314) & 0xfffe) >> 1);
}

static __inline HI_VOID hi_isp_csc_coef12_write(HI_U16 data) {
    IOWR_16DIRECT(0x131a, ((data & 0x7fff) << 1));
}

static __inline HI_U16 hi_isp_csc_coef12_read(HI_VOID) {
    return ((IORD_16DIRECT(0x131a) & 0xfffe) >> 1);
}

static __inline HI_VOID hi_isp_csc_coef11_write(HI_U16 data) {
    IOWR_16DIRECT(0x1318, ((data & 0x7fff) << 1));
}

static __inline HI_U16 hi_isp_csc_coef11_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1318) & 0xfffe) >> 1);
}

static __inline HI_VOID hi_isp_csc_coef21_write(HI_U16 data) {
    IOWR_16DIRECT(0x131e, ((data & 0x7fff) << 1));
}

static __inline HI_U16 hi_isp_csc_coef21_read(HI_VOID) {
    return ((IORD_16DIRECT(0x131e) & 0xfffe) >> 1);
}

static __inline HI_VOID hi_isp_csc_coef20_write(HI_U16 data) {
    IOWR_16DIRECT(0x131c, ((data & 0x7fff) << 1));
}

static __inline HI_U16 hi_isp_csc_coef20_read(HI_VOID) {
    return ((IORD_16DIRECT(0x131c) & 0xfffe) >> 1);
}

static __inline HI_VOID hi_isp_csc_coef22_write(HI_U16 data) {
    IOWR_16DIRECT(0x1320, ((data & 0x7fff) << 1));
}

static __inline HI_U16 hi_isp_csc_coef22_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1320) & 0xfffe) >> 1);
}

static __inline HI_VOID hi_isp_csc_in_dc1_write(HI_U16 data) {
    IOWR_16DIRECT(0x1332, ((data & 0x7ff) << 5));
}

static __inline HI_U16 hi_isp_csc_in_dc1_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1332) & 0xfffe) >> 5);
}

static __inline HI_VOID hi_isp_csc_in_dc0_write(HI_U16 data) {
    IOWR_16DIRECT(0x1330, ((data & 0x7ff) << 5));
}

static __inline HI_U16 hi_isp_csc_in_dc0_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1330) & 0xfffe) >> 5);
}

static __inline HI_VOID hi_isp_csc_in_dc2_write(HI_U16 data) {
    IOWR_16DIRECT(0x1334, ((data & 0x7ff) << 5));
}

static __inline HI_U16 hi_isp_csc_in_dc2_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1334) & 0xfffe) >> 5);
}

static __inline HI_VOID hi_isp_csc_out_dc1_write(HI_U16 data) {
    IOWR_16DIRECT(0x1342, ((data & 0x7ff) << 5));
}

static __inline HI_U16 hi_isp_csc_out_dc1_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1342) & 0xfffe) >> 5);
}

static __inline HI_VOID hi_isp_csc_out_dc0_write(HI_U16 data) {
    IOWR_16DIRECT(0x1340, ((data & 0x7ff) << 5));
}

static __inline HI_U16 hi_isp_csc_out_dc0_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1340) & 0xfffe) >> 5);
}

static __inline HI_VOID hi_isp_csc_out_dc2_write(HI_U16 data) {
    IOWR_16DIRECT(0x1344, ((data & 0x7ff) << 5));
}

static __inline HI_U16 hi_isp_csc_out_dc2_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1344) & 0xfffe) >> 5);
}

/*****************************************************
MODULE        -----DEMOSAIC-----
MODULE        -----DEMOSAIC-----
MODULE        -----DEMOSAIC-----
*****************************************************/
static __inline HI_VOID hi_isp_demosaic_cfg_enable_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x3000);
    IOWR_8DIRECT(0x3000, (data & 0x01) | (u8current & 0xfe));
}

static __inline HI_U8 hi_isp_demosaic_cfg_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x3000) & 0x1);
}

static __inline HI_VOID hi_isp_demosaic_fcr_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x3006, data);
}

static __inline HI_U8 hi_isp_demosaic_fcr_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x3006));
}

static __inline HI_VOID hi_isp_demosaic_ahd_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x3004, data);
}

static __inline HI_U8 hi_isp_demosaic_ahd_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x3004));
}

static __inline HI_VOID hi_isp_demosaic_version_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x300e);
    IOWR_16DIRECT(0x300e, (data & 0xfff) << 4 | (u16current & 0xf));
}

static __inline HI_U16 hi_isp_demosaic_version_read(HI_VOID) {
    return ((IORD_16DIRECT(0x300e) & 0xfff) >> 4);
}

static __inline HI_VOID hi_isp_demosaic_date_write(HI_U16 data) {
    IOWR_16DIRECT(0x300c, data);
}

static __inline HI_U16 hi_isp_demosaic_date_read(HI_VOID) {
    return (IORD_16DIRECT(0x300c));
}

static __inline HI_VOID hi_isp_demosaic_bld_limit1_write(HI_U8 data) {
    IOWR_8DIRECT(0x3012, data);
}

static __inline HI_U8 hi_isp_demosaic_bld_limit1_read(HI_VOID) {
    return (IORD_8DIRECT(0x3012));
}

static __inline HI_VOID hi_isp_demosaic_bld_limit2_write(HI_U8 data) {
    IOWR_8DIRECT(0x3010, data);
}

static __inline HI_U8 hi_isp_demosaic_bld_limit2_read(HI_VOID) {
    return (IORD_8DIRECT(0x3010));
}

static __inline HI_VOID hi_isp_demosaic_bld_ratio1_write(HI_U16 data) {
    IOWR_16DIRECT(0x3016, (data & 0x1ff));
}

static __inline HI_U16 hi_isp_demosaic_bld_ratio1_read(HI_VOID) {
    return (IORD_16DIRECT(0x3016)& 0x1ff);
}

static __inline HI_VOID hi_isp_demosaic_bld_ratio2_write(HI_U16 data) {
    IOWR_16DIRECT(0x3014, (data & 0x1ff));
}

static __inline HI_U16 hi_isp_demosaic_bld_ratio2_read(HI_VOID) {
    return (IORD_16DIRECT(0x3014)& 0x1ff);
}

static __inline HI_VOID hi_isp_demosaic_fcr_limit1_write(HI_U16 data) {
    IOWR_16DIRECT(0x301a, (data & 0xfff));
}

static __inline HI_U16 hi_isp_demosaic_fcr_limit1_read(HI_VOID) {
    return (IORD_16DIRECT(0x301a)& 0xfff);
}

static __inline HI_VOID hi_isp_demosaic_fcr_limit2_write(HI_U16 data) {
    IOWR_16DIRECT(0x3018, (data & 0xfff));
}

static __inline HI_U16 hi_isp_demosaic_fcr_limit2_read(HI_VOID) {
    return (IORD_16DIRECT(0x3018)& 0xfff);
}

static __inline HI_VOID hi_isp_demosaic_ahd_par1_write(HI_U16 data) {
    IOWR_16DIRECT(0x301e, data);
}

static __inline HI_U16 hi_isp_demosaic_ahd_par1_read(HI_VOID) {
    return (IORD_16DIRECT(0x301e));
}

static __inline HI_VOID hi_isp_demosaic_ahd_par2_write(HI_U8 data) {
    IOWR_8DIRECT(0x301c, data);
}

static __inline HI_U8 hi_isp_demosaic_ahd_par2_read(HI_VOID) {
    return (IORD_8DIRECT(0x301c));
}

static __inline HI_VOID hi_isp_demosaic_fcr_thr_write(HI_U16 data) {
    IOWR_16DIRECT(0x3022, (data & 0xfff));
}

static __inline HI_U16 hi_isp_demosaic_fcr_thr_read(HI_VOID) {
    return (IORD_16DIRECT(0x3022) & 0xfff);
}

static __inline HI_VOID hi_isp_demosaic_fcr_gain_write(HI_U8 data) {
    IOWR_8DIRECT(0x3020, (data & 0x1f));
}

static __inline HI_U8 hi_isp_demosaic_fcr_gain_read(HI_VOID) {
    return (IORD_8DIRECT(0x3020) & 0x1f);
}

static __inline HI_VOID hi_isp_demosaic_hv_ratio_write(HI_U8 data) {
    IOWR_8DIRECT(0x3024, data);
}

static __inline HI_U8 hi_isp_demosaic_hv_ratio_read(HI_VOID) {
    return (IORD_8DIRECT(0x3024));
}

static __inline HI_VOID hi_isp_demosaic_fcr_ratio_write(HI_U8 data) {
    IOWR_8DIRECT(0x3028, (data & 0x3f));
}

static __inline HI_U8 hi_isp_demosaic_fcr_ratio_read(HI_VOID) {
    return (IORD_8DIRECT(0x3028) & 0x3f);
}

static __inline HI_VOID hi_isp_demosaic_hv_sel_write(HI_U8 data) {
    IOWR_8DIRECT(0x3030, (data & 0x3));
}

static __inline HI_U8 hi_isp_demosaic_hv_sel_read(HI_VOID) {
    return (IORD_8DIRECT(0x3030) & 0x3);
}

static __inline HI_VOID hi_isp_demosaic_tmplate_write(HI_U8 data) {
    IOWR_8DIRECT(0x3034, (data & 0x1));
}

static __inline HI_U8 hi_isp_demosaic_tmplate_read(HI_VOID) {
    return (IORD_8DIRECT(0x3034) & 0x1);
}

static __inline HI_VOID hi_isp_demosaic_rggb_cfg_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x30e0);
    IOWR_8DIRECT(0x30e0, (data & 0x3) | (u8current & 0xfc));
}

static __inline HI_U8 hi_isp_demosaic_rggb_cfg_read(HI_VOID) {
    return (IORD_8DIRECT(0x30e0) & 0x3);
}

static __inline HI_VOID hi_isp_demosaic_update_mode_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x30e4);
    IOWR_8DIRECT(0x30e4, (data & 0x1) | (u8current & 0xfe));
}

static __inline HI_U8 hi_isp_demosaic_update_mode_read(HI_VOID) {
    return (IORD_8DIRECT(0x30e4) & 0x1);
}

static __inline HI_VOID hi_isp_demosaic_cpi_fix_timing_stat_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x30e8);
    IOWR_16DIRECT(0x30e8, ((data & 0x1fff) << 1) | (u16current & 0xc001));
}

static __inline HI_U16 hi_isp_demosaic_cpi_fix_timing_stat_read(HI_VOID) {
    return ((IORD_16DIRECT(0x30e8) & 0x1fff) >> 1);
}

static __inline HI_VOID hi_isp_demosaic_cpi_auto_timing_en_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x30e8);
    IOWR_8DIRECT(0x30e8, (data & 0x1) | (u8current & 0xfe));
}

static __inline HI_U8 hi_isp_demosaic_cpi_auto_timing_en_read(HI_VOID) {
    return (IORD_8DIRECT(0x30e8) & 0x1);
}

static __inline HI_VOID hi_isp_demosaic_update_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x30ec);
    IOWR_8DIRECT(0x30ec, (data & 0x1) | (u8current & 0xfe));
}

static __inline HI_U8 hi_isp_demosaic_update_read(HI_VOID) {
    return (IORD_8DIRECT(0x30ec) & 0x1);
}

static __inline HI_VOID hi_isp_demosaic_height_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x30f2);
    IOWR_16DIRECT(0x30f2, (data & 0x1fff) | (u16current & 0xe000));
}

static __inline HI_U16 hi_isp_demosaic_height_read(HI_VOID) {
    return (IORD_16DIRECT(0x30f2) & 0x1fff);
}

static __inline HI_VOID hi_isp_demosaic_width_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x30f0);
    IOWR_16DIRECT(0x30f0, (data & 0x1fff) | (u16current & 0xe000));
}

static __inline HI_U16 hi_isp_demosaic_width_read(HI_VOID) {
    return (IORD_16DIRECT(0x30f0) & 0x1fff);
}

/*****************************************************
MODULE        -----DG-----
MODULE        -----DG-----
MODULE        -----DG-----
*****************************************************/
static __inline HI_VOID hi_isp_dg_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x1400, data);
}

static __inline HI_U8 hi_isp_dg_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x1400));
}

static __inline HI_VOID hi_isp_dg_version_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x140e);
    IOWR_16DIRECT(0x140e, (data & 0xfff) << 4 | (u16current & 0xf));
}

static __inline HI_U16 hi_isp_dg_version_read(HI_VOID) {
    return ((IORD_16DIRECT(0x140e) & 0xfff) >> 4);
}

static __inline HI_VOID hi_isp_dg_date_write(HI_U16 data) {
    IOWR_16DIRECT(0x140c, data);
}

static __inline HI_U16 hi_isp_dg_date_read(HI_VOID) {
    return (IORD_16DIRECT(0x140c));
}

/*****************************************************
ISP_DG_GAIN1:                    Gain1
0x205a1410:                    
                                          31:16      rgain
                                          15:00      grgain
*****************************************************/

static __inline HI_VOID hi_isp_dg_rgain_write(HI_U16 data) {
    IOWR_16DIRECT(0x1412, (data));
}

static __inline HI_U16 hi_isp_dg_rgain_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1412)));
}

static __inline HI_VOID hi_isp_dg_grgain_write(HI_U16 data) {
    IOWR_16DIRECT(0x1410, (data));
}

static __inline HI_U16 hi_isp_dg_grgain_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1410)));
}

/*****************************************************
ISP_DG_GAIN2:         Gain2
0x205a1414:                     31:28      reserved
                                          31:16      bgain
                                          15:00      gbgain
*****************************************************/

static __inline HI_VOID hi_isp_dg_bgain_write(HI_U16 data) {
    IOWR_16DIRECT(0x1416, (data));
}

static __inline HI_U16 hi_isp_dg_bgain_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1416)));
}

static __inline HI_VOID hi_isp_dg_gbgain_write(HI_U16 data) {
    IOWR_16DIRECT(0x1414, (data));
}

static __inline HI_U16 hi_isp_dg_gbgain_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1414)));
}

/*****************************************************
MODULE        -----DPC-----
MODULE        -----DPC-----
MODULE        -----DPC-----
*****************************************************/
static __inline HI_VOID hi_isp_dpc_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x5000, (data & 0x01));
}

static __inline HI_U8 hi_isp_dpc_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x5000) & 0x1);
}

static __inline HI_VOID hi_isp_dpc_mem_sel_write(HI_U8 data) {
    IOWR_8DIRECT(0x5004, (data & 0x01));
}

static __inline HI_U8 hi_isp_dpc_mem_sel_read(HI_VOID) {
    return (IORD_8DIRECT(0x5004) & 0x1);
}

static __inline HI_VOID hi_isp_dpc_blend_write(HI_U32 data) {
    IOWR_32DIRECT(0x5008, data);
}

static __inline HI_U32 hi_isp_dpc_blend_read(HI_VOID) {
    return (IORD_32DIRECT(0x5008));
}

static __inline HI_VOID hi_isp_dpc_version_write(HI_U16 data) {
	HI_U16 u16Current = IORD_16DIRECT(0x500e);
    IOWR_16DIRECT(0x500e, ((data & 0xfff) << 4) | (u16Current & 0xf));
}

static __inline HI_U16 hi_isp_dpc_version_read(HI_VOID) {
    return ((IORD_16DIRECT(0x500e) & 0xfff) >> 4);
}

static __inline HI_VOID hi_isp_dpc_data_write(HI_U16 data) {
    IOWR_16DIRECT(0x500c, data);
}

static __inline HI_U8 hi_isp_dpc_data_read(HI_VOID) {
    return (IORD_16DIRECT(0x500c));
}

static __inline HI_VOID hi_isp_dp_mode_write(HI_U16 data) {
    IOWR_16DIRECT(0x5010, data);
}

static __inline HI_U16 hi_isp_dp_mode_read(HI_VOID) {
    return (IORD_16DIRECT(0x5010));
}

static __inline HI_VOID hi_isp_dp_IR_channel_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x5010);
    IOWR_8DIRECT(0x5010, ((data & 0x1) << 9) | (u8Current & 0x1ff));
}

static __inline HI_U8 hi_isp_dp_IR_channel_read(HI_VOID) {
    return ((IORD_8DIRECT(0x5010) & 0x200) >> 9);
}

static __inline HI_VOID hi_isp_dp_IR_position_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x5010);
    IOWR_8DIRECT(0x5010, ((data & 0x1) << 8) | (u8Current & 0x2ff));
}

static __inline HI_U8 hi_isp_dp_IR_position_read(HI_VOID) {
    return ((IORD_8DIRECT(0x5010) & 0x100) >>8);
}



static __inline HI_VOID hi_isp_dp_hightlight_en_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x5010);
    IOWR_8DIRECT(0x5010, ((data & 0x1) << 7) | (u8Current & 0x7f));
}

static __inline HI_U8 hi_isp_dp_hightlight_en_read(HI_VOID) {
    return ((IORD_8DIRECT(0x5010) & 0x80) >> 7);
}

static __inline HI_VOID hi_isp_dp_dpt_det_sel_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x5010);
    IOWR_8DIRECT(0x5010, ((data & 0x1) << 6) | (u8Current & 0xbf));
}

static __inline HI_U8 hi_isp_dp_dpt_det_sel_read(HI_VOID) {
    return ((IORD_8DIRECT(0x5010) & 0x40) >> 6);
}

static __inline HI_VOID hi_isp_dp_bpt_cor_en_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x5010);
    IOWR_8DIRECT(0x5010, ((data & 0x1) << 5) | (u8Current & 0xdf));
}

static __inline HI_U8 hi_isp_dp_bpt_cor_en_read(HI_VOID) {
    return ((IORD_8DIRECT(0x5010) & 0x20) >> 5);
}

static __inline HI_VOID hi_isp_dp_grayscale_mode_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x5010);
    IOWR_8DIRECT(0x5010, ((data & 0x1) << 4) | (u8Current & 0xef));
}

static __inline HI_U8 hi_isp_dp_grayscale_mode_read(HI_VOID) {
    return ((IORD_8DIRECT(0x5010) & 0x10) >> 4);
}

static __inline HI_VOID hi_isp_dp_bpt_update_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x5010);
    IOWR_8DIRECT(0x5010, ((data & 0x1) << 3) | (u8Current & 0xf7));
}

static __inline HI_U8 hi_isp_dp_bpt_update_read(HI_VOID) {
    return ((IORD_8DIRECT(0x5010) & 0x08) >> 3);
}

static __inline HI_VOID hi_isp_dp_six_det_en_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x5010);
    IOWR_8DIRECT(0x5010, ((data & 0x1) << 2) | (u8Current & 0xfb));
}

static __inline HI_U8 hi_isp_six_det_en_read(HI_VOID) {
    return ((IORD_8DIRECT(0x5010) & 0x04) >> 2);
}

static __inline HI_VOID hi_isp_dp_cor_en_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x5010);
    IOWR_8DIRECT(0x5010, ((data & 0x1) << 1) | (u8Current & 0xfd));
}

static __inline HI_U8 hi_isp_cor_en_read(HI_VOID) {
    return ((IORD_8DIRECT(0x5010) & 0x02) >> 1);
}

static __inline HI_VOID hi_isp_dp_det_en_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x5010);
    IOWR_8DIRECT(0x5010, ((data & 0x1)) | (u8Current & 0xfe));
}

static __inline HI_U8 hi_isp_det_en_read(HI_VOID) {
    return ((IORD_8DIRECT(0x5010) & 0x01));
}

static __inline HI_VOID hi_isp_dp_output_mode_write(HI_U8 data) {
    IOWR_8DIRECT(0x5014, data);
}

static __inline HI_U8 hi_isp_dp_output_mode_read(HI_VOID) {
    return ((IORD_8DIRECT(0x5014) & 0xf));
}

static __inline HI_VOID hi_isp_dp_set_use_write(HI_U8 data) {
    IOWR_8DIRECT(0x5018, data);
}

static __inline HI_U8 hi_isp_dp_set_use_read(HI_VOID) {
    return ((IORD_8DIRECT(0x5018) & 0xf));
}

static __inline HI_VOID hi_isp_dp_methods_set_1_write(HI_U16 data) {
    IOWR_16DIRECT(0x501c, (data & 0x1fff));
}

static __inline HI_U16 hi_isp_dp_methods_set_1_read(HI_VOID) {
    return ((IORD_16DIRECT(0x501c) & 0x1fff));
}

static __inline HI_VOID hi_isp_dp_methods_set_2_write(HI_U16 data) {
    IOWR_16DIRECT(0x5020, (data & 0x1fff));
}

static __inline HI_U16 hi_isp_dp_methods_set_2_read(HI_VOID) {
    return ((IORD_16DIRECT(0x5020) & 0x1fff));
}

static __inline HI_VOID hi_isp_dp_methods_set_3_write(HI_U16 data) {
    IOWR_16DIRECT(0x5024, (data & 0x1fff));
}

static __inline HI_U16 hi_isp_dp_methods_set_3_read(HI_VOID) {
    return ((IORD_16DIRECT(0x5024) & 0x1fff));
}

static __inline HI_VOID hi_isp_dp_line_thresh_1_write(HI_U16 data) {
    IOWR_16DIRECT(0x5028, data);
}

static __inline HI_U16 hi_isp_dp_line_thresh_1_read(HI_VOID) {
    return (IORD_16DIRECT(0x5028));
}

static __inline HI_VOID hi_isp_dp_line_mad_fac_1_write(HI_U16 data) {
    IOWR_16DIRECT(0x502c, data);
}

static __inline HI_U16 hi_isp_dp_line_mad_fac_1_read(HI_VOID) {
    return (IORD_16DIRECT(0x502c));
}

static __inline HI_VOID hi_isp_dp_pg_fac_1_write(HI_U16 data) {
    IOWR_16DIRECT(0x5030, data);
}

static __inline HI_U16 hi_isp_dp_pg_fac_1_read(HI_VOID) {
    return (IORD_16DIRECT(0x5030));
}

static __inline HI_VOID hi_isp_dp_rnd_thresh_1_write(HI_U16 data) {
    IOWR_16DIRECT(0x5034, data);
}

static __inline HI_U16 hi_isp_dp_rnd_thresh_1_read(HI_VOID) {
    return (IORD_16DIRECT(0x5034));
}

static __inline HI_VOID hi_isp_dp_rg_fac_1_write(HI_U16 data) {
    IOWR_16DIRECT(0x5038, data);
}

static __inline HI_U16 hi_isp_dp_rg_fac_1_read(HI_VOID) {
    return (IORD_16DIRECT(0x5038));
}

static __inline HI_VOID hi_isp_dp_line_thresh_2_write(HI_U16 data) {
    IOWR_16DIRECT(0x503c, data);
}

static __inline HI_U16 hi_isp_dp_line_thresh_2_read(HI_VOID) {
    return (IORD_16DIRECT(0x503c));
}

static __inline HI_VOID hi_isp_dp_line_mad_fac_2_write(HI_U16 data) {
    IOWR_16DIRECT(0x5040, data);
}

static __inline HI_U16 hi_isp_dp_line_mad_fac_2_read(HI_VOID) {
    return (IORD_16DIRECT(0x5040));
}

static __inline HI_VOID hi_isp_dp_pg_fac_2_write(HI_U16 data) {
    IOWR_16DIRECT(0x5044, data);
}

static __inline HI_U16 hi_isp_dp_pg_fac_2_read(HI_VOID) {
    return (IORD_16DIRECT(0x5044));
}

static __inline HI_VOID hi_isp_dp_rnd_thresh_2_write(HI_U16 data) {
    IOWR_16DIRECT(0x5048, data);
}

static __inline HI_U16 hi_isp_dp_rnd_thresh_2_read(HI_VOID) {
    return (IORD_16DIRECT(0x5048));
}

static __inline HI_VOID hi_isp_dp_rg_fac_2_write(HI_U16 data) {
    IOWR_16DIRECT(0x504c, data);
}

static __inline HI_U16 hi_isp_dp_rg_fac_2_read(HI_VOID) {
    return (IORD_16DIRECT(0x504c));
}

static __inline HI_VOID hi_isp_dp_line_thresh_3_write(HI_U16 data) {
    IOWR_16DIRECT(0x5050, data);
}

static __inline HI_U16 hi_isp_dp_line_thresh_3_read(HI_VOID) {
    return (IORD_16DIRECT(0x5050));
}

static __inline HI_VOID hi_isp_dp_line_mad_fac_3_write(HI_U16 data) {
    IOWR_16DIRECT(0x5054, data);
}

static __inline HI_U16 hi_isp_dp_line_mad_fac_3_read(HI_VOID) {
    return (IORD_16DIRECT(0x5054));
}

static __inline HI_VOID hi_isp_dp_pg_fac_3_write(HI_U16 data) {
    IOWR_16DIRECT(0x5058, data);
}

static __inline HI_U16 hi_isp_dp_pg_fac_3_read(HI_VOID) {
    return (IORD_16DIRECT(0x5058));
}

static __inline HI_VOID hi_isp_dp_rnd_thresh_3_write(HI_U16 data) {
    IOWR_16DIRECT(0x505c, data);
}

static __inline HI_U16 hi_isp_dp_rnd_thresh_3_read(HI_VOID) {
    return (IORD_16DIRECT(0x505c));
}

static __inline HI_VOID hi_isp_dp_rg_fac_3_write(HI_U16 data) {
    IOWR_16DIRECT(0x5060, data);
}

static __inline HI_U16 hi_isp_dp_rg_fac_3_read(HI_VOID) {
    return (IORD_16DIRECT(0x5060));
}

static __inline HI_VOID hi_isp_dp_ro_limits_write(HI_U16 data) {
    IOWR_16DIRECT(0x5064, data);
}

static __inline HI_U16 hi_isp_dp_ro_limits_read(HI_VOID) {
    return (IORD_16DIRECT(0x5064));
}

static __inline HI_VOID hi_isp_dp_rnd_offs_write(HI_U16 data) {
    IOWR_16DIRECT(0x5068, (data & 0xfff));
}

static __inline HI_U16 hi_isp_dp_rnd_offs_read(HI_VOID) {
    return (IORD_16DIRECT(0x5068) & 0xfff);
}

static __inline HI_VOID hi_isp_dp_bpt_ctrl_write(HI_U32 data) {
    IOWR_32DIRECT(0x506c, data );
}

static __inline HI_U32 hi_isp_dp_bpt_ctrl_read(HI_VOID) {
    return (IORD_32DIRECT(0x506c) );
}

static __inline HI_VOID hi_isp_dp_bpt_number_write(HI_U16 data) {
    IOWR_16DIRECT(0x5070, (data & 0xfff));
}

static __inline HI_U16 hi_isp_dp_bpt_number_read(HI_VOID) {
    return (IORD_16DIRECT(0x5070) & 0xfff);
}

static __inline HI_VOID hi_isp_dp_bpt_calib_number_write(HI_U16 data) {
    IOWR_16DIRECT(0x5074, (data & 0xfff));
}

static __inline HI_U16 hi_isp_dp_bpt_calib_number_read(HI_VOID) {
    return (IORD_16DIRECT(0x5074) & 0xfff);
}

static __inline HI_VOID hi_isp_dp_in_soft_rst_write(HI_U8 data) {
    IOWR_8DIRECT(0x5078, data);
}

static __inline HI_U8 hi_isp_dp_in_soft_rst_read(HI_VOID) {
    return (IORD_8DIRECT(0x5078) & 0x1);
}

static __inline HI_VOID hi_isp_dp_bpt_thresh_write(HI_U32 data) {
	IOWR_32DIRECT(0x507c, data);
}

static __inline HI_U32 hi_isp_dp_bpt_thresh_read(HI_VOID) {

	return (IORD_32DIRECT(0x507c));
}

static __inline HI_VOID hi_isp_dp_bpt_waddr_write(HI_U32 data) {
	IOWR_32DIRECT(0x5080, data);
}

static __inline HI_U32 hi_isp_dp_bpt_waddr_read(HI_VOID) {
	return (IORD_32DIRECT(0x5080));
}

static __inline HI_VOID hi_isp_dp_bpt_wdata_write(HI_U32 data) {
	IOWR_32DIRECT(0x5084, data);
}

static __inline HI_U32 hi_isp_dp_bpt_wdata_read(HI_VOID) {
	return (IORD_32DIRECT(0x5084));
}

static __inline HI_VOID hi_isp_dp_bpt_raddr_write(HI_U32 data) {
	IOWR_32DIRECT(0x5088, data);
}

static __inline HI_U32 hi_isp_dp_bpt_raddr_read(HI_VOID) {
	return (IORD_32DIRECT(0x5088));
}

static __inline HI_VOID hi_isp_dp_bpt_rdata_write(HI_U32 data) {
	IOWR_32DIRECT(0x508c, data);
}

static __inline HI_U32 hi_isp_dp_bpt_rdata_read(HI_VOID) {
	return (IORD_32DIRECT(0x508c));
}

static __inline HI_VOID hi_isp_dp_width_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT(0x50f0);
	IOWR_32DIRECT(0x50f0, (u32Current & 0xfffff000) | (data & 0xfff));
}

static __inline HI_U16 hi_isp_dp_width_read(HI_VOID) {
	return (IORD_32DIRECT(0x50f0) & 0xfff);
}

static __inline HI_VOID hi_isp_dp_height_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT(0x50f0);
	IOWR_32DIRECT(0x50f0, (u32Current & 0xf000ffff) | ((data & 0xfff) << 16));
}

static __inline HI_U16 hi_isp_dp_height_read(HI_VOID) {
	return ((IORD_32DIRECT(0x50f0) & 0xfff0000) >> 16);
}

/*****************************************************
MODULE        -----FPN-----
MODULE        -----FPN-----
MODULE        -----FPN-----
*****************************************************/
static __inline HI_VOID hi_isp_fpn_clk_gating_write(HI_U8 data){
	HI_U32 u32current = IORD_32DIRECT(0x5200);
    IOWR_32DIRECT(0x5200, (u32current & (~(1 << 31))) | ((data&0x1) << 31));
}

static __inline HI_U8 hi_isp_fpn_clk_gating_read(HI_VOID) {
    return ((IORD_32DIRECT(0x5200) >> 31) & 0x1);
}

static __inline HI_VOID hi_isp_fpn_mode_write(HI_U8 data){
	HI_U32 u32current = IORD_32DIRECT(0x5200);
    IOWR_32DIRECT(0x5200, (u32current & (~(1 << 9))) | ((data&0x1) << 9));
}

static __inline HI_U8 hi_isp_fpn_mode_read(HI_VOID) {
    return ((IORD_32DIRECT(0x5200) >> 9) & 0x1);
}

static __inline HI_VOID hi_isp_fpn_work_mode_write(HI_U8 data){
	HI_U32 u32current = IORD_32DIRECT(0x5200);
    IOWR_32DIRECT(0x5200, (u32current & (~(1 << 8))) | ((data&0x1) << 8));
}

static __inline HI_U8 hi_isp_fpn_work_mode_read(HI_VOID) {
    return ((IORD_32DIRECT(0x5200) >> 8) & 0x1);
}

static __inline HI_VOID hi_isp_fpn_enable_write(HI_U8 data){
	HI_U32 u32current = IORD_32DIRECT(0x5200);
    IOWR_32DIRECT(0x5200, (u32current & (~(1 << 0))) | (data&0x1));
}

static __inline HI_U8 hi_isp_fpn_enable_read(HI_VOID) {
    return (IORD_32DIRECT(0x5200) & 0x1);
}

static __inline HI_VOID hi_isp_fpn_corr_gainoffset_write(HI_U8 index, HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT(0x5230 + (index<<2));
	IOWR_32DIRECT((0x5230 + (index<<2)), ((u32Current & 0x0000f000) | data));
}

static __inline HI_U32 hi_isp_fpn_corr_gainoffset_read(HI_U8 index){
	return (IORD_32DIRECT(0x5230 + (index<<2)));
}

static __inline HI_VOID hi_isp_fpn_coverflowthr_write(HI_U16 data) {
	HI_U16 u16Current = IORD_16DIRECT(0x5254);
	IOWR_16DIRECT(0x5254, (u16Current & 0xc000) | (data & 0x3fff));
}

static __inline HI_U16 hi_isp_fpn_overflowthr_read(HI_VOID){
	return (IORD_16DIRECT(0x5254) & 0x3fff);
}

static __inline HI_VOID hi_isp_fpn_width_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT(0x52f0);
	IOWR_32DIRECT(0x52f0, (u32Current & 0xffffe000) | (data & 0x1fff));
}

static __inline HI_U16 hi_isp_fpn_width_read(HI_VOID) {
	return (IORD_32DIRECT(0x52f0) & 0x1fff);
}

static __inline HI_VOID hi_isp_fpn_height_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT(0x52f0);
	IOWR_32DIRECT(0x52f0, (u32Current & 0xe000ffff) | ((data & 0x1fff) << 16));
}

static __inline HI_U16 hi_isp_fpn_height_read(HI_VOID) {
	return ((IORD_32DIRECT(0x52f0) & 0x1fff0000) >> 16);
}

/*****************************************************
MODULE        -----DRC-----
MODULE        -----DRC-----
MODULE        -----DRC-----
*****************************************************/
/*****************************************************
ISP_DRC_CFG:         DRC Enable
0x205a6200:           31:1    reserved
                                0     DRC Enable
*****************************************************/
static __inline HI_VOID hi_isp_drc_enable_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x6200);
    IOWR_8DIRECT(0x6200, (data & 0x01) | (u8Current & 0xfe));
}

static __inline HI_U8 hi_isp_drc_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x6200) & 0x1);
}

/*****************************************************
ISP_DRC_ZONE:       DRC Zone
0x205a6210:             31:30  reserved
                               29:24  chk_y
					23:16  vnum
					15:14  reserved
                                13:8   chk_x
                                7:0     hnum
*****************************************************/
static __inline HI_VOID hi_isp_drc_hnum_write(HI_U8 data) {
    IOWR_8DIRECT(0x6210, (data & 0x7f));
}

static __inline HI_U8 hi_isp_drc_hnum_read(HI_VOID) {
    return ((IORD_8DIRECT(0x6210) & 0x7f));
}

static __inline HI_VOID hi_isp_drc_vnum_write(HI_U8 data) {
    IOWR_8DIRECT(0x6211, (data & 0x7f));
}

static __inline HI_U8 hi_isp_drc_vnum_read(HI_VOID) {
    return ((IORD_8DIRECT(0x6211) & 0x7f));
}

/*****************************************************
ISP_DRC_ZONE_SIZE:       DRC Zone Size 
0x205a6214:             31:30  reserved
                               29:24  chk_y
					23:16  vnum
					15:14  reserved
                                13:8   chk_x
                                7:0     hnum
*****************************************************/
static __inline HI_VOID hi_isp_drc_hsize_write(HI_U8 data) {
    IOWR_8DIRECT(0x6214, (data));
}

static __inline HI_U8 hi_isp_drc_hsize_read(HI_VOID) {
	return ((IORD_8DIRECT(0x6214)));
}

static __inline HI_VOID hi_isp_drc_chk_x_write(HI_U8 data) {
    IOWR_8DIRECT(0x6215, (data)&0x3f);
}

static __inline HI_U8 hi_isp_drc_chk_x_read(HI_VOID) {
	return ((IORD_8DIRECT(0x6215))&0x3f);
}

static __inline HI_VOID hi_isp_drc_vsize_write(HI_U8 data) {
    IOWR_8DIRECT(0x6216, (data));
}

static __inline HI_U8 hi_isp_drc_vsize_read(HI_VOID) {
	return ((IORD_8DIRECT(0x6216)));
}

static __inline HI_VOID hi_isp_drc_chk_y_write(HI_U8 data) {
    IOWR_8DIRECT(0x6217, (data)&0x3f);
}

static __inline HI_U8 hi_isp_drc_chk_y_read(HI_VOID) {
	return ((IORD_8DIRECT(0x6217))&0x3f);
}


/*****************************************************
ISP_DRC_ZONE_DIV0:       DRC Zone div0
0x205a6218:              31       reserved
					30:16   div_y0
                                15       reserved
                                14:0    div_x0
*****************************************************/
static __inline HI_VOID hi_isp_drc_div_x0_write(HI_U16 data) {
    IOWR_16DIRECT(0x6218, (data & 0x7fff));
}

static __inline HI_U16 hi_isp_drc_div_x0_read(HI_VOID) {
	return ((IORD_16DIRECT(0x6218) & 0x7fff));
}

static __inline HI_VOID hi_isp_drc_div_y0_write(HI_U16 data) {
    IOWR_16DIRECT(0x621a, (data & 0x7fff));
}

static __inline HI_U16 hi_isp_drc_div_y0_read(HI_VOID) {
	return ((IORD_16DIRECT(0x621a) & 0x7fff));
}

/*****************************************************
ISP_DRC_ZONE_DIV1:       DRC Zone div1
0x205a621C:             31       reserved
					30:16   div_y1
                                15       reserved
                                14:0    div_x1
*****************************************************/
static __inline HI_VOID hi_isp_drc_div_x1_write(HI_U16 data) {
    IOWR_16DIRECT(0x621c, (data & 0x7fff));
}

static __inline HI_U16 hi_isp_drc_div_x1_read(HI_VOID) {
	return ((IORD_16DIRECT(0x621c) & 0x7fff));
}

static __inline HI_VOID hi_isp_drc_div_y1_write(HI_U16 data) {
    IOWR_16DIRECT(0x621e, (data & 0x7fff));
}

static __inline HI_U16 hi_isp_drc_div_y1_read(HI_VOID) {
	return ((IORD_16DIRECT(0x621e) & 0x7fff));
}



/*****************************************************
ISP_DRC_BIN:         DRC Bin
0x205a6220:           31:05    reserved
					04:00    bin
*****************************************************/
static __inline HI_VOID hi_isp_drc_bin_write(HI_U8 data) {
    IOWR_8DIRECT(0x6220, (data & 0x1f));
}

static __inline HI_U8 hi_isp_drc_bin_read(HI_VOID) {
	return ((IORD_8DIRECT(0x6220) & 0x1f));
}

/*****************************************************
ISP_DRC_BIN_DIV:         DRC Bin Div
0x205a6224:           31:15    reserved
					14:00    div_z the reciprocal of  DRC bin
*****************************************************/
static __inline HI_VOID hi_isp_drc_div_z_write(HI_U16 data) {
    IOWR_16DIRECT(0x6224, (data & 0x7fff));
}

static __inline HI_U16 hi_isp_drc_div_z_read(HI_VOID) {
	return ((IORD_16DIRECT(0x6224) & 0x7fff));
}

/*****************************************************
ISP_DRC_BIN_SCALE:         DRC Bin scale
0x205a6228:           31:03    reserved
					02:00    bin_scale  the reciprocal of DRC bin scale
*****************************************************/
static __inline HI_VOID hi_isp_drc_bin_scale_write(HI_U8 data) {
    IOWR_8DIRECT(0x6228, (data & 0x7));
}

static __inline HI_U8 hi_isp_drc_bin_scale_read(HI_VOID) {
	return ((IORD_8DIRECT(0x6228) & 0x7));
}

/*****************************************************
ISP_DRC_RANGE_FLT_COEF:         DRC range filter coef
0x205a622c:             31:03    reserved
					03:00   range flt coef
*****************************************************/
static __inline HI_VOID hi_isp_drc_range_flt_coef_write(HI_U8 data) {
    IOWR_8DIRECT(0x622c, (data & 0xf));
}

static __inline HI_U8 hi_isp_drc_range_flt_coef_read(HI_VOID) {
	return ((IORD_8DIRECT(0x622c) & 0xf));
}

/*****************************************************
ISP_DRC_EXPOSURE:         DRC exposure
0x205a6230:           31:16    reserved
					15:00    exposure 
*****************************************************/
static __inline HI_VOID hi_isp_drc_exposure_write(HI_U16 data) {
    IOWR_16DIRECT(0x6230, (data));
}

static __inline HI_U16 hi_isp_drc_exposure_read(HI_VOID) {
	return ((IORD_16DIRECT(0x6230)));
}

/*****************************************************
ISP_DRC_LOCAL_EDGE_LMT:         DRC local edge limit
0x205a6234:           31:08    reserved
					07:00    local_edge_lmt 
*****************************************************/
static __inline HI_VOID hi_isp_drc_local_edge_lmt_write(HI_U8 data) {
    IOWR_8DIRECT(0x6234, (data));
}

static __inline HI_U8 hi_isp_drc_local_edge_lmt_read(HI_VOID) {
	return ((IORD_8DIRECT(0x6234)));
}

/*****************************************************
ISP_DRC_STRENGTH:         DRC Strength
0x205a6238:           31:08    reserved
					07:00    strength 
*****************************************************/
static __inline HI_VOID hi_isp_drc_strength_write(HI_U8 data) {
    IOWR_8DIRECT(0x6238, (data));
}

static __inline HI_U8 hi_isp_drc_strength_read(HI_VOID) {
	return ((IORD_8DIRECT(0x6238)));
}

/*****************************************************
ISP_DRC_DETAIL_GAIN: DRC detail gain 
0x205a623C:             31:24    reserved
                               23:16    detail_gain_thres
                               15:8      detail_gain_dark
					07:00    detail_gain_bright
*****************************************************/
static __inline HI_VOID hi_isp_drc_detail_mixing_thres_write(HI_U8 data) {
    IOWR_8DIRECT(0x623e, data);
}

static __inline HI_U8 hi_isp_drc_detail_mixing_thres_read(HI_VOID) {
	return ((IORD_8DIRECT(0x623e)));
}

static __inline HI_VOID hi_isp_drc_detail_mixing_dark_write(HI_U8 data) {
    IOWR_8DIRECT(0x623d, (data));
}

static __inline HI_U8 hi_isp_drc_detail_mixing_dark_read(HI_VOID) {
	return ((IORD_8DIRECT(0x623d)));
}

static __inline HI_VOID hi_isp_drc_detail_mixing_bright_write(HI_U8 data) {
    IOWR_8DIRECT(0x623C, (data));
}

static __inline HI_U8 hi_isp_drc_detail_mixing_bright_read(HI_VOID) {
	return ((IORD_8DIRECT(0x623C)));
}





/*****************************************************
ISP_DRC_DARK_GAIN_LMT_Y:         DRC dark gain lmt 
0x205a6240:           31:16    reserved
					15:14    SFT2  
					13:10    VAL2 
					09:07    SFT1
					06:00    VAL1
*****************************************************/
static __inline HI_VOID hi_isp_drc_y_val1_write(HI_U16 data) {
	HI_U16 u16Current = IORD_16DIRECT(0x6240);
    IOWR_16DIRECT(0x6240, (data & 0x7f) | (u16Current & 0xff80));
}

static __inline HI_U16 hi_isp_drc_y_val1_read(HI_VOID) {
	return ((IORD_16DIRECT(0x6240) & 0x7f));
}

static __inline HI_VOID hi_isp_drc_y_sft1_write(HI_U16 data) {
	HI_U16 u16Current = IORD_16DIRECT(0x6240);
    IOWR_16DIRECT(0x6240, ((data & 0x7) << 7) | (u16Current & 0xfc7f));
}

static __inline HI_U16 hi_isp_drc_y_sft1_read(HI_VOID) {
	return ((IORD_16DIRECT(0x6240) & 0x380) >> 7);
}

static __inline HI_VOID hi_isp_drc_y_val2_write(HI_U16 data) {
	HI_U16 u16Current = IORD_16DIRECT(0x6240);
    IOWR_16DIRECT(0x6240, ((data & 0xf) << 10) | (u16Current & 0xc3ff));
}

static __inline HI_U16 hi_isp_drc_y_val2_read(HI_VOID) {
	return ((IORD_16DIRECT(0x6240) & 0x3c00) >> 10);
}

static __inline HI_VOID hi_isp_drc_y_sft2_write(HI_U16 data) {
	HI_U16 u16Current = IORD_16DIRECT(0x6240);
    IOWR_16DIRECT(0x6240, ((data & 0x3) << 14) | (u16Current & 0x3fff));
}

static __inline HI_U16 hi_isp_drc_y_sft2_read(HI_VOID) {
	return ((IORD_16DIRECT(0x6240) & 0xc000) >> 14);
}

/*****************************************************
ISP_DRC_DARK_GAIN_LMT_C:         DRC dark gain lmt 
0x205a6244:           31:16    reserved
					15:14    SFT2  
					13:10    VAL2 
					09:07    SFT1
					06:00    VAL1
*****************************************************/
static __inline HI_VOID hi_isp_drc_c_val1_write(HI_U16 data) {
	HI_U16 u16Current = IORD_16DIRECT(0x6244);
    IOWR_16DIRECT(0x6244, (data & 0x7f) | (u16Current & 0xff80));
}

static __inline HI_U16 hi_isp_drc_c_val1_read(HI_VOID) {
	return ((IORD_16DIRECT(0x6244) & 0x7f));
}

static __inline HI_VOID hi_isp_drc_c_sft1_write(HI_U16 data) {
	HI_U16 u16Current = IORD_16DIRECT(0x6244);
    IOWR_16DIRECT(0x6244, ((data & 0x7) << 7) | (u16Current & 0xfc7f));
}

static __inline HI_U16 hi_isp_drc_c_sft1_read(HI_VOID) {
	return ((IORD_16DIRECT(0x6244) & 0x380) >> 7);
}

static __inline HI_VOID hi_isp_drc_c_val2_write(HI_U16 data) {
	HI_U16 u16Current = IORD_16DIRECT(0x6244);
    IOWR_16DIRECT(0x6244, ((data & 0xf) << 10) | (u16Current & 0xc3ff));
}

static __inline HI_U16 hi_isp_drc_c_val2_read(HI_VOID) {
	return ((IORD_16DIRECT(0x6244) & 0x3c00) >> 10);
}

static __inline HI_VOID hi_isp_drc_c_sft2_write(HI_U16 data) {
	HI_U16 u16Current = IORD_16DIRECT(0x6244);
    IOWR_16DIRECT(0x6244, ((data & 0x3) << 14) | (u16Current & 0x3fff));
}

static __inline HI_U16 hi_isp_drc_c_sft2_read(HI_VOID) {
	return ((IORD_16DIRECT(0x6244) & 0xc000) >> 14);
}

/*****************************************************
ISP_DRC_BRIGHT_GAIN_LMT:         DRC bright gain lmt
0x205a6248:           31:10    reserved
					09:07    SFT
					06:00    VAL
*****************************************************/
static __inline HI_VOID hi_isp_drc_bright_gain_val_write(HI_U16 data) {
	HI_U16 u16Current = IORD_16DIRECT(0x6248);
    IOWR_16DIRECT(0x6248, (data & 0x7f) | (u16Current & 0xff80));
}

static __inline HI_U16 hi_isp_drc_bright_gain_val_read(HI_VOID) {
	return ((IORD_16DIRECT(0x6248) & 0x7f));
}

static __inline HI_VOID hi_isp_drc_bright_gain_sft_write(HI_U16 data) {
	HI_U16 u16Current = IORD_16DIRECT(0x6248);
    IOWR_16DIRECT(0x6248, ((data & 0x7) << 7) | (u16Current & 0xfc7f));
}

static __inline HI_U16 hi_isp_drc_bright_gain_sft_read(HI_VOID) {
	return ((IORD_16DIRECT(0x6248) & 0x380) >> 7);
}

/*****************************************************
ISP_DRC_VAR_SPATIAL:  DRC  spatial variance
0x205a6250:           07:04    var_range
				    03:00    var_spatial
*****************************************************/
static __inline HI_VOID hi_isp_drc_var_spatial_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x6250);
	IOWR_8DIRECT(0x6250, (data & 0xf) | (u8Current & 0xf0));
}

static __inline HI_U8 hi_isp_drc_var_spatial_read(HI_VOID) {
	return ((IORD_8DIRECT(0x6250) & 0xf));
}

static __inline HI_VOID hi_isp_drc_var_range_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x6250);
    IOWR_8DIRECT(0x6250, ((data&0xf) << 4) | (u8Current & 0xf));
}

static __inline HI_U8 hi_isp_drc_var_range_read(HI_VOID) {
	return ((IORD_8DIRECT(0x6250) & 0xf0)>> 4);
}



/*****************************************************
ISP_DRC_LUT_MIX_CTRL:  DRC lut mix control
0x205a6254:           31:08    reserved
					07:00    outbits
*****************************************************/
static __inline HI_VOID hi_isp_drc_lut_mix_ctrl_write(HI_U8 data) {
    IOWR_8DIRECT(0x6254, (data & 0xff));
}

static __inline HI_U8 hi_isp_drc_lut_mix_ctrl_read(HI_VOID) {
	return ((IORD_8DIRECT(0x6254) & 0xff));
}

/*****************************************************
ISP_DRC_GAIN_CLIP_KNEE:  DRC gain clip
0x205a6258:             31:08    reserved
					07:04    gain_clip_knee
					03:00    gain_clip_step
*****************************************************/
static __inline HI_VOID hi_isp_drc_gain_clip_step_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x6258);
	IOWR_8DIRECT(0x6258, (data & 0xf) | (u8Current & 0x70));  
}

static __inline HI_U8 hi_isp_drc_gain_clip_step_read(HI_VOID) {
	return ((IORD_8DIRECT(0x6258) & 0xf));
}

/*****************************************************
ISP_DRC_GAIN_CLIP_STEP:  DRC gain clip step
0x205a6258:             31:08    reserved
					07:04    gain_clip_knee
					03:00    gain_clip_step
*****************************************************/
static __inline HI_VOID hi_isp_drc_gain_clip_knee_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x6258);
	IOWR_8DIRECT(0x6258, (((data & 0xf)<<4) | (u8Current & 0xf)));  
}

static __inline HI_U8 hi_isp_drc_gain_clip_knee_read(HI_VOID) {
	return IORD_8DIRECT(((0x6258& 0xf0)>> 4));
}


/*****************************************************
ISP_DRC_STAT_IND_WADDR   DRC ToneMapping LUT0 write addr
0x205a6280:                 31:00     drc_stat_ind_waddr
*****************************************************/
static __inline HI_VOID hi_isp_drc_stat_ind_waddr0_write(HI_U32 data) { 
    IOWR_32DIRECT(0x6280, data);
}

static __inline HI_U32 hi_isp_drc_stat_ind_waddr0_read(HI_VOID) {
    return (IORD_32DIRECT(0x6280));
}

/*****************************************************
ISP_DRC_STAT_IND_WDATA   DRC ToneMapping LUT0 write data
0x205a6284:                 31:00     drc_stat_ind_wdata
*****************************************************/
static __inline HI_VOID hi_isp_drc_stat_ind_wdata0_write(HI_U32 data) { 
    IOWR_32DIRECT(0x6284, data);
}

static __inline HI_U32 hi_isp_drc_stat_ind_wdata0_read(HI_VOID) {
    return (IORD_32DIRECT(0x6284));
}

/*****************************************************
ISP_DRC_STAT_IND_RADDR  DRC ToneMapping LUT0 read addr
0x205a6288:                 31:00     drc_stat_ind_raddr
*****************************************************/
static __inline HI_VOID hi_isp_drc_stat_ind_raddr0_write(HI_U32 data) { 
    IOWR_32DIRECT(0x6288, data);
}

static __inline HI_U32 hi_isp_drc_stat_ind_raddr0_read(HI_VOID) {
    return (IORD_32DIRECT(0x6288));
}

/*****************************************************
ISP_DRC_STAT_IND_RDATA  DRC ToneMapping LUT0 read data
0x205a628c:                 31:00     drc_stat_ind_rdata
*****************************************************/
static __inline HI_VOID hi_isp_drc_stat_ind_rdata0_write(HI_U32 data) { 
    IOWR_32DIRECT(0x628c, data);
}

static __inline HI_U32 hi_isp_drc_stat_ind_rdata0_read(HI_VOID) {
    return (IORD_32DIRECT(0x628c));
}

/*****************************************************
ISP_DRC_STAT_IND_WADDR   DRC ToneMapping LUT1 write addr
0x205a6280:                 31:00     drc_stat_ind_waddr
*****************************************************/
static __inline HI_VOID hi_isp_drc_stat_ind_waddr1_write(HI_U32 data) { 
    IOWR_32DIRECT(0x6290, data);
}

static __inline HI_U32 hi_isp_drc_stat_ind_waddr1_read(HI_VOID) {
    return (IORD_32DIRECT(0x6290));
}

/*****************************************************
ISP_DRC_STAT_IND_WDATA  DRC ToneMapping LUT1 write data
0x205a6294:                 31:00     drc_stat_ind_wdata
*****************************************************/
static __inline HI_VOID hi_isp_drc_stat_ind_wdata1_write(HI_U32 data) { 
    IOWR_32DIRECT(0x6294, data);
}

static __inline HI_U32 hi_isp_drc_stat_ind_wdata1_read(HI_VOID) {
    return (IORD_32DIRECT(0x6294));
}

/*****************************************************
ISP_DRC_STAT_IND_RADDR  DRC ToneMapping LUT1 read addr
0x205a6298:                 31:00     drc_stat_ind_raddr
*****************************************************/
static __inline HI_VOID hi_isp_drc_stat_ind_raddr1_write(HI_U32 data) { 
    IOWR_32DIRECT(0x6298, data);
}

static __inline HI_U32 hi_isp_drc_stat_ind_raddr1_read(HI_VOID) {
    return (IORD_32DIRECT(0x6298));
}

/*****************************************************
ISP_DRC_STAT_IND_RDATA  DRC ToneMapping LUT1 read data
0x205a629c:                 31:00     drc_stat_ind_rdata
*****************************************************/
static __inline HI_VOID hi_isp_drc_stat_ind_rdata1_write(HI_U32 data) { 
    IOWR_32DIRECT(0x629c, data);
}

static __inline HI_U32 hi_isp_drc_stat_ind_rdata1_read(HI_VOID) {
    return (IORD_32DIRECT(0x629c));
}

/*****************************************************
ISP_DRC_STAT_IND_RDATA  DRC ColorCorrection Lut write addr
0x205a62a0:                 31:00     drc_stat_ind_rdata
*****************************************************/
static __inline HI_VOID hi_isp_drc_stat_ind_waddr2_write(HI_U32 data) { 
    IOWR_32DIRECT(0x62a0, data);
}

static __inline HI_U32 hi_isp_drc_stat_ind_waddr2_read(HI_VOID) {
    return (IORD_32DIRECT(0x62a0));
}

/*****************************************************
ISP_DRC_STAT_IND_WDATA   DRC ColorCorrection Lut  write data
0x205a62a4:                 31:00     drc_stat_ind_wdata
*****************************************************/
static __inline HI_VOID hi_isp_drc_stat_ind_wdata2_write(HI_U32 data) { 
    IOWR_32DIRECT(0x62a4, data);
}

static __inline HI_U32 hi_isp_drc_stat_ind_wdata2_read(HI_VOID) {
    return (IORD_32DIRECT(0x62a4));
}

/*****************************************************
ISP_DRC_STAT_IND_RADDR  DRC ColorCorrection Lut  read addr
0x205a62a8:                 31:00     drc_stat_ind_raddr
*****************************************************/
static __inline HI_VOID hi_isp_drc_stat_ind_raddr2_write(HI_U32 data) { 
    IOWR_32DIRECT(0x62a8, data);
}

static __inline HI_U32 hi_isp_drc_stat_ind_raddr2_read(HI_VOID) {
    return (IORD_32DIRECT(0x62a8));
}

/*****************************************************
ISP_DRC_STAT_IND_RDATA  DRC ColorCorrection Lut  read data
0x205a62ac:                 31:00     drc_stat_ind_rdata
*****************************************************/
static __inline HI_VOID hi_isp_drc_stat_ind_rdata2_write(HI_U32 data) { 
    IOWR_32DIRECT(0x62ac, data);
}

static __inline HI_U32 hi_isp_drc_stat_ind_rdata2_read(HI_VOID) {
    return (IORD_32DIRECT(0x62ac));
}

static __inline HI_VOID hi_isp_drc_width_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT(0x62f0);
	IOWR_32DIRECT(0x62f0, (u32Current & 0xfffff000) | (data & 0xfff));
}

static __inline HI_U16 hi_isp_drc_width_read(HI_VOID) {
	return (IORD_32DIRECT(0x62f0) & 0xfff);
}

static __inline HI_VOID hi_isp_drc_height_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT(0x62f0);
	IOWR_32DIRECT(0x62f0, (u32Current & 0xf000ffff) | ((data & 0xfff) << 16));
}

static __inline HI_U16 hi_isp_drc_height_read(HI_VOID) {
	return ((IORD_32DIRECT(0x62f0) & 0xfff0000) >> 16);
}

/*****************************************************
MODULE        -----GAMMA-----
MODULE        -----GAMMA-----
MODULE        -----GAMMA-----
*****************************************************/
// args: data (1-bit)
static __inline HI_VOID hi_isp_gamma_enable_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x7300);
    IOWR_8DIRECT(0x7300, (data & 0x01) | (u8Current & 0xFE));
}
static __inline HI_U8 hi_isp_gamma_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x7300) & 0x01);
}

static __inline HI_VOID hi_isp_gamma_lut_update_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x7310);
    IOWR_8DIRECT(0x7310, (data & 0x01) | (u8Current & 0xFE));
}
static __inline HI_U8 hi_isp_gamma_lut_update_read(HI_VOID) {
    return (IORD_8DIRECT(0x7310) & 0x01);
}

static __inline HI_VOID hi_isp_gamma_waddr_write(HI_U32 data) {
	IOWR_32DIRECT(0x7380, data);
}
static __inline HI_U32 hi_isp_gamma_waddr_read(HI_VOID) {
    return IORD_32DIRECT(0x7380);
}

static __inline HI_VOID hi_isp_gamma_wdata_write(HI_U32 data) {
	IOWR_32DIRECT(0x7384, data);
}
static __inline HI_U32 hi_isp_gamma_wdata_read(HI_VOID) {
    return IORD_32DIRECT(0x7384);
}

static __inline HI_VOID hi_isp_gamma_raddr_write(HI_U32 data) {
	IOWR_32DIRECT(0x7388, data);
}
static __inline HI_U32 hi_isp_gamma_raddr_read(HI_VOID) {
    return IORD_32DIRECT(0x7388);
}

static __inline HI_VOID hi_isp_gamma_rdata_write(HI_U32 data) {
	IOWR_32DIRECT(0x738c, data);
}
static __inline HI_U32 hi_isp_gamma_rdata_read(HI_VOID) {
    return IORD_32DIRECT(0x738c);
}

/*****************************************************
MODULE        -----GAMMAFE-----
MODULE        -----GAMMAFE-----
MODULE        -----GAMMAFE-----
*****************************************************/
/*****************************************************
ISP_GAMMAFE_CFG:         Enable
0x205a7400:                     31:1      reserved
                                          0           GAMMA_FE Enable
*****************************************************/
static __inline HI_VOID hi_isp_gamma_fe_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x7400, data);
}

static __inline HI_U8 hi_isp_gamma_fe_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x7400));
}

/*****************************************************
ISP_GAMMAFE_VERSION:        Version
0x205a740c:                     31:20      version
                                          19:16      reserved
                                          15: 0       date
*****************************************************/
static __inline HI_VOID hi_isp_gamma_fe_version_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x400e);
    IOWR_16DIRECT(0x740e, (data << 4) | (u16current&0xf));
}

static __inline HI_U16 hi_isp_gamma_fe_version_read(HI_VOID) {
    return (IORD_16DIRECT(0x740e) >> 4);
}

static __inline HI_VOID hi_isp_gamma_fe_date_write(HI_U16 data) {
    IOWR_16DIRECT(0x740c, data);
}

static __inline HI_U16 hi_isp_gamma_fe_date_read(HI_VOID) {
    return (IORD_16DIRECT(0x740c));
}

/*****************************************************
ISP_GAMMAFE_CTRL_BITW:         Bit Width
0x205a7410:                     31:21      reserved
                                          20:16      bitw_in
                                          15:05      reserved
                                          04:00      bitw_out
*****************************************************/
static __inline HI_VOID hi_isp_gamma_fe_bitw_in_write(HI_U8 data) {
    IOWR_8DIRECT(0x7412, (data & 0x1f));
}

static __inline HI_U8 hi_isp_gamma_fe_bitw_in_read(HI_VOID) {
    return (IORD_8DIRECT(0x7412) & 0x1f);
}

static __inline HI_VOID hi_isp_gamma_fe_bitw_out_write(HI_U8 data) {
    IOWR_8DIRECT(0x7410, (data & 0x1f));
}

static __inline HI_U8 hi_isp_gamma_fe_bitw_out_read(HI_VOID) {
    return (IORD_8DIRECT(0x7410) & 0x1f);
}

/*****************************************************
ISP_GAMMAFE_MIX_CTRL:        Gamma_Fe mix ctrl
0x205a7414:                        31:08     reserved
                                          07:00     mix_ctrl 
                                          Mix_ctrl=0  the out is shift
						   Mix_ctrl=255 the out is blending
*****************************************************/
static __inline HI_VOID hi_isp_gamma_fe_mix_ctrl_write(HI_U8 data) {
    IOWR_8DIRECT(0x7414, data);
}

static __inline HI_U8 hi_isp_gamma_fe_mix_ctrl_read(HI_VOID) {
    return (IORD_8DIRECT(0x7414));
}

/*****************************************************
ISP_GAMMAFE_WADDR0:         GAMAM_FE 33 LUT write addr
0x205a7480:                     31:00      gamma_waddr
*****************************************************/
static __inline HI_VOID hi_isp_gamma_fe_waddr0_write(HI_U32 data) {
    IOWR_32DIRECT(0x7480, data);
}

static __inline HI_U32 hi_isp_gamma_fe_waddr0_read(HI_VOID) {
    return (IORD_32DIRECT(0x7480));
}

/*****************************************************
ISP_GAMMAFE_WDATA0:  GAMAM_FE  33 LUT write data
0x205a7484:                     31:00      gamma_wdata
*****************************************************/
static __inline HI_VOID hi_isp_gamma_fe_wdata0_write(HI_U32 data) {
    IOWR_32DIRECT(0x7484, data);
}

static __inline HI_U32 hi_isp_gamma_fe_wdata0_read(HI_VOID) {
    return (IORD_32DIRECT(0x7484));
}

/*****************************************************
ISP_GAMMAFE_RADDR0:         GAMAM_FE  33 LUT read addr
0x205a7488:                     31:00      gamma_raddr
*****************************************************/
static __inline HI_VOID hi_isp_gamma_fe_raddr0_write(HI_U32 data) {
    IOWR_32DIRECT(0x7488, data);
}

static __inline HI_U32 hi_isp_gamma_fe_raddr0_read(HI_VOID) {
    return (IORD_32DIRECT(0x7488));
}

/*****************************************************
ISP_GAMMAFE_RDATA0:  GAMAM_FE  33 LUT read data
0x205a748c:                     31:00      gamma_rdata
*****************************************************/
static __inline HI_VOID hi_isp_gamma_fe_rdata0_write(HI_U32 data) {
    IOWR_32DIRECT(0x748c, data);
}

static __inline HI_U32 hi_isp_gamma_fe_rdata0_read(HI_VOID) {
    return (IORD_32DIRECT(0x748c));
}

/*****************************************************
ISP_GAMMAFE_WADDR1:         GAMAM_FE  257 LUT write addr
0x205a7490:                     31:00      gamma_waddr
*****************************************************/
static __inline HI_VOID hi_isp_gamma_fe_waddr1_write(HI_U32 data) {
    IOWR_32DIRECT(0x7490, data);
}

static __inline HI_U32 hi_isp_gamma_fe_waddr1_read(HI_VOID) {
    return (IORD_32DIRECT(0x7490));
}

/*****************************************************
ISP_GAMMAFE_WDATA0:  GAMAM_FE  257 LUT write data
0x205a7494:                     31:00      gamma_wdata
*****************************************************/
static __inline HI_VOID hi_isp_gamma_fe_wdata1_write(HI_U32 data) {
    IOWR_32DIRECT(0x7494, data);
}

static __inline HI_U32 hi_isp_gamma_fe_wdata1_read(HI_VOID) {
    return (IORD_32DIRECT(0x7494));
}

/*****************************************************
ISP_GAMMAFE_RADDR0:         GAMAM_FE  257 LUT read addr
0x205a7498:                     31:00      gamma_raddr
*****************************************************/
static __inline HI_VOID hi_isp_gamma_fe_raddr1_write(HI_U32 data) {
    IOWR_32DIRECT(0x7498, data);
}

static __inline HI_U32 hi_isp_gamma_fe_raddr1_read(HI_VOID) {
    return (IORD_32DIRECT(0x7498));
}

/*****************************************************
ISP_GAMMAFE_RDATA0:  GAMAM_FE 257 LUT read data
0x205a749c:                     31:00      gamma_rdata
*****************************************************/
static __inline HI_VOID hi_isp_gamma_fe_rdata1_write(HI_U32 data) {
    IOWR_32DIRECT(0x749c, data);
}

static __inline HI_U32 hi_isp_gamma_fe_rdata1_read(HI_VOID) {
    return (IORD_32DIRECT(0x749c));
}

/*****************************************************
MODULE        -----GE-----
MODULE        -----GE-----
MODULE        -----GE-----
*****************************************************/
static __inline HI_VOID hi_isp_ge_enable_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x5100);
    IOWR_8DIRECT(0x5100, (data & 0x1) | (u8Current & 0xfe));
}

static __inline HI_U8 hi_isp_ge_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x5100) & 0x1);
}

static __inline HI_VOID hi_isp_ge_aver_enable_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x5100);
    IOWR_8DIRECT(0x5100, ((data & 0x1) << 4) | (u8Current & 0xef));
}

static __inline HI_U8 hi_isp_ge_aver_enable_read(HI_VOID) {
    return ((IORD_8DIRECT(0x5100) & 0x10) >> 4);
}

static __inline HI_VOID hi_isp_ge_correct_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x5101, (data & 0x1));
}

static __inline HI_U8 hi_isp_ge_correct_enable_read(HI_VOID) {
    return ((IORD_8DIRECT(0x5101) & 0x1));
}

static __inline HI_VOID hi_isp_ge_version_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x510e);
    IOWR_16DIRECT(0x510e, (data & 0xfff) << 4 | (u16current & 0xf));
}

static __inline HI_U16 hi_isp_ge_version_read(HI_VOID) {
    return ((IORD_16DIRECT(0x510e) & 0xfff) >> 4);
}

static __inline HI_VOID hi_isp_ge_date_write(HI_U16 data) {
    IOWR_16DIRECT(0x510c, data);
}

static __inline HI_U16 hi_isp_ge_date_read(HI_VOID) {
    return (IORD_16DIRECT(0x510c));
}

static __inline HI_VOID hi_isp_ge_ct_th1_write(HI_U16 data) {
    IOWR_16DIRECT(0x5110, (data & 0x3fff));
}

static __inline HI_U16 hi_isp_ge_ct_th1_read(HI_VOID) {
    return (IORD_16DIRECT(0x5110) & 0x3fff);
}

static __inline HI_VOID hi_isp_ge_ct_th2_write(HI_U16 data) {
    IOWR_16DIRECT(0x5114, (data & 0x3fff));
}

static __inline HI_U16 hi_isp_ge_ct_th2_read(HI_VOID) {
    return (IORD_16DIRECT(0x5114) & 0x3fff);
}

static __inline HI_VOID hi_isp_ge_ct_th3_write(HI_U16 data) {
    IOWR_16DIRECT(0x5118, (data & 0x3fff));
}

static __inline HI_U16 hi_isp_ge_ct_th3_read(HI_VOID) {
    return (IORD_16DIRECT(0x5118) & 0x3fff);
}

static __inline HI_VOID hi_isp_ge_ct_slope1_write(HI_U8 data) {
    IOWR_8DIRECT(0x511c, (data & 0xf));
}

static __inline HI_U8 hi_isp_ge_ct_slope1_read(HI_VOID) {
    return (IORD_8DIRECT(0x511c) & 0xf);
}

static __inline HI_VOID hi_isp_ge_ct_slope2_write(HI_U8 data) {
    IOWR_8DIRECT(0x5120, (data & 0xf));
}

static __inline HI_U8 hi_isp_ge_ct_slope2_read(HI_VOID) {
    return (IORD_8DIRECT(0x5120) & 0xf);
}

static __inline HI_VOID hi_isp_ge_hnum_write(HI_U8 data) {
    IOWR_8DIRECT(0x5130, (data & 0x1f));
}

static __inline HI_U8 hi_isp_ge_hnum_read(HI_VOID) {
    return (IORD_8DIRECT(0x5130) & 0x1f);
}

static __inline HI_VOID hi_isp_ge_vnum_write(HI_U8 data) {
    IOWR_8DIRECT(0x5131, (data & 0x1f));
}

static __inline HI_U8 hi_isp_ge_vnum_read(HI_VOID) {
    return (IORD_8DIRECT(0x5131) & 0x1f);
}

static __inline HI_VOID hi_isp_ge_aver_raddr_write(HI_U32 data) {
    IOWR_32DIRECT(0x5188, data);
}

static __inline HI_U32 hi_isp_ge_aver_raddr_read(HI_VOID) {
    return (IORD_32DIRECT(0x5188));
}

static __inline HI_VOID hi_isp_ge_aver_rdata_write(HI_U32 data) {
    IOWR_32DIRECT(0x518c, data);
}

static __inline HI_U32 hi_isp_ge_aver_rdata_read(HI_VOID) {
    return (IORD_32DIRECT(0x518c));
}

static __inline HI_VOID hi_isp_ge_width_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT(0x51f0);
	IOWR_32DIRECT(0x51f0, (u32Current & 0xfffff000) | (data & 0xfff));
}

static __inline HI_U16 hi_isp_ge_width_read(HI_VOID) {
	return (IORD_32DIRECT(0x51f0) & 0xfff);
}

static __inline HI_VOID hi_isp_ge_height_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT(0x51f0);
	IOWR_32DIRECT(0x51f0, (u32Current & 0xf000ffff) | ((data & 0xfff) << 16));
}

static __inline HI_U16 hi_isp_ge_height_read(HI_VOID) {
	return ((IORD_32DIRECT(0x51f0) & 0xfff0000) >> 16);
}


/*****************************************************
MODULE        -----NR-----
MODULE        -----NR-----
MODULE        -----NR-----
*****************************************************/
static __inline HI_VOID hi_isp_2dnr_cfg_enable_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x4000);
    IOWR_8DIRECT(0x4000, (data & 0x1) | (u8Current & 0xfe));
}

static __inline HI_U8 hi_isp_2dnr_cfg_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x4000) & 0x1);
}

static __inline HI_VOID hi_isp_2dnr_luma_nr_enable_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x4000);
    IOWR_8DIRECT(0x4000, ((data & 0x1) << 1) | (u8Current & 0xfd));
}

static __inline HI_U8 hi_isp_2dnr_luma_nr_enable_read(HI_VOID) {
    return ((IORD_8DIRECT(0x4000) & 0x2) >> 1);
}

static __inline HI_VOID hi_isp_2dnr_version_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x400e);
    IOWR_16DIRECT(0x400e, (data << 4) | (u16current&0xf));
}

static __inline HI_U16 hi_isp_2dnr_version_read(HI_VOID) {
    return (IORD_16DIRECT(0x400e) >> 4);
}

static __inline HI_VOID hi_isp_2dnr_date_write(HI_U16 data) {
    IOWR_16DIRECT(0x400c, data);
}

static __inline HI_U16 hi_isp_2dnr_date_read(HI_VOID) {
    return (IORD_16DIRECT(0x400c));
}

static __inline HI_VOID hi_isp_2dnr_fcr_en_write(HI_U8 data) {
    IOWR_8DIRECT(0x4013, (data&0x1));
}

static __inline HI_U8 hi_isp_2dnr_fcr_en_read(HI_VOID) {
    return (IORD_8DIRECT(0x4013)&0x1);
}

static __inline HI_VOID hi_isp_2dnr_fcrshift_write(HI_U8 data) {
    IOWR_8DIRECT(0x4012, (data));
}

static __inline HI_U8 hi_isp_2dnr_fcrshift_read(HI_VOID) {
    return (IORD_8DIRECT(0x4012));
}

static __inline HI_VOID hi_isp_2dnr_fcrgain_write(HI_U8 data) {
    IOWR_8DIRECT(0x4011, (data));
}

static __inline HI_U8 hi_isp_2dnr_fcrgain_read(HI_VOID) {
    return (IORD_8DIRECT(0x4011));
}

static __inline HI_VOID hi_isp_2dnr_fcrlimit_write(HI_U8 data) {
    IOWR_8DIRECT(0x4010, (data));
}

static __inline HI_U8 hi_isp_2dnr_fcrlimit_read(HI_VOID) {
    return (IORD_8DIRECT(0x4010));
} 

static __inline HI_VOID hi_isp_2dnr_luma_gain_write(HI_U8 data) {
    IOWR_8DIRECT(0x4014, (data&0x1f));
}

static __inline HI_U8 hi_isp_2dnr_luma_gain_read(HI_VOID) {
    return (IORD_8DIRECT(0x4014)&0x1f);
}

static __inline HI_VOID hi_isp_2dnr_luma_low_write(HI_U16 data) {
    IOWR_16DIRECT(0x401a, (data&0x3fff));
}

static __inline HI_U16 hi_isp_2dnr_luma_low_read(HI_VOID) {
    return (IORD_16DIRECT(0x401a)&0x3fff);
}

static __inline HI_VOID hi_isp_2dnr_luma_high_write(HI_U16 data) {
    IOWR_16DIRECT(0x4018, (data&0x3fff));
}

static __inline HI_U16 hi_isp_2dnr_luma_high_read(HI_VOID) {
    return (IORD_16DIRECT(0x4018)&0x3fff);
}

static __inline HI_VOID hi_isp_2dnr_limit_low_write(HI_U16 data) {
    IOWR_16DIRECT(0x401e, (data&0x1ff));
}

static __inline HI_U16 hi_isp_2dnr_limit_low_read(HI_VOID) {
    return (IORD_16DIRECT(0x401e)&0x1ff);
}

static __inline HI_VOID hi_isp_2dnr_limit_high_write(HI_U16 data) {
    IOWR_16DIRECT(0x401c, (data&0x1ff));
}

static __inline HI_U16 hi_isp_2dnr_limit_high_read(HI_VOID) {
    return (IORD_16DIRECT(0x401c)&0x1ff);
}

static __inline HI_VOID hi_isp_2dnr_thres_low_write(HI_U8 index, HI_U16 data) {
    IOWR_16DIRECT((0x4022 + index*0x4),(data&0xfff));
}

static __inline HI_U16 hi_isp_2dnr_thres_low_read(HI_U8 index) {
    return (IORD_16DIRECT(0x4022 + index*0x4)&0xfff);
}

static __inline HI_VOID hi_isp_2dnr_thres_high_write(HI_U8 index, HI_U16 data) {
    IOWR_16DIRECT((0x4020 + index*0x4), (data&0xfff));
}

static __inline HI_U16 hi_isp_2dnr_thres_high_read(HI_U8 index) {
    return (IORD_16DIRECT(0x4020 + index*0x4)&0xfff);
}

static __inline HI_VOID hi_isp_2dnr_width_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT(0x40f0);
	IOWR_32DIRECT(0x40f0, (u32Current & 0xfffff000) | (data & 0xfff));
}

static __inline HI_U16 hi_isp_2dnr_width_read(HI_VOID) {
	return (IORD_32DIRECT(0x40f0) & 0xfff);
}

static __inline HI_VOID hi_isp_2dnr_height_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT(0x40f0);
	IOWR_32DIRECT(0x40f0, (u32Current & 0xf000ffff) | ((data & 0xfff) << 16));
}

static __inline HI_U16 hi_isp_2dnr_height_read(HI_VOID) {
	return ((IORD_32DIRECT(0x40f0) & 0xfff0000) >> 16);
}


/*****************************************************
MODULE        -----SHARPEN-----
MODULE        -----SHARPEN-----
MODULE        -----SHARPEN-----
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_cfg_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x4200, data);
}

static __inline HI_U8 hi_isp_sharpen_cfg_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x4200)&0x1);
}

static __inline HI_VOID hi_isp_sharpen_version_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x420e);
    IOWR_16DIRECT(0x420e, (data << 4) | (u16current&0xf));
}

static __inline HI_U16 hi_isp_sharpen_version_read(HI_VOID) {
    return (IORD_16DIRECT(0x420e) >> 4);
}

static __inline HI_VOID hi_isp_sharpen_date_write(HI_U16 data) {
    IOWR_16DIRECT(0x420c, data);
}

static __inline HI_U16 hi_isp_sharpen_date_read(HI_VOID) {
    return (IORD_16DIRECT(0x420c));
}

static __inline HI_VOID hi_isp_edge_amt_write(HI_U16 data) {
    IOWR_16DIRECT(0x4212, (data&0xfff));
}

static __inline HI_U16 hi_isp_edge_amt_read(HI_VOID) {
    return (IORD_16DIRECT(0x4212)&0xfff);
}

static __inline HI_VOID hi_isp_sharp_amt_write(HI_U16 data) {
    IOWR_16DIRECT(0x4210, (data&0xfff));
}

static __inline HI_U16 hi_isp_sharp_amt_read(HI_VOID) {
    return (IORD_16DIRECT(0x4210)&0xfff);
}

static __inline HI_VOID hi_isp_edge_thd1_write(HI_U16 data) {
    IOWR_16DIRECT(0x4216, data);
}

static __inline HI_U16 hi_isp_edge_thd1_read(HI_VOID) {
    return (IORD_16DIRECT(0x4216));
}

static __inline HI_VOID hi_isp_sharp_thd1_write(HI_U16 data) {
    IOWR_16DIRECT(0x4214, data);
}

static __inline HI_U16 hi_isp_sharp_thd1_read(HI_VOID) {
    return (IORD_16DIRECT(0x4214));
}

static __inline HI_VOID hi_isp_edge_coef_write(HI_U16 data) {
	IOWR_16DIRECT(0x421a, (data&0xfff));
}

static __inline HI_U16 hi_isp_edge_coef_read(HI_VOID) {
	return (IORD_16DIRECT(0x421a)&0xfff);
}

static __inline HI_VOID hi_isp_sharp_coef_write(HI_U16 data) {
	IOWR_16DIRECT(0x4218, (data&0xfff));
}

static __inline HI_U16 hi_isp_sharp_coef_read(HI_VOID) {
	return (IORD_16DIRECT(0x4218)&0xfff);
}

static __inline HI_VOID hi_isp_over_amt_write(HI_U8 data) {
	IOWR_8DIRECT(0x421e, data);
}

static __inline HI_U8 hi_isp_over_amt_read(HI_VOID) {
	return (IORD_8DIRECT(0x421e));
}

static __inline HI_VOID hi_isp_under_amt_write(HI_U8 data) {
	IOWR_8DIRECT(0x421c, data);
}

static __inline HI_U8 hi_isp_under_amt_read(HI_VOID) {
	return (IORD_8DIRECT(0x421c));
}

static __inline HI_VOID hi_isp_sharpen_mid_tmp02_write(HI_U8 data) {
	IOWR_8DIRECT(0x4232, data);
}

static __inline HI_U8 hi_isp_sharpen_mid_tmp02_read(HI_VOID) {
	return (IORD_8DIRECT(0x4232));
}

static __inline HI_VOID hi_isp_sharpen_mid_tmp01_write(HI_U8 data) {
	IOWR_8DIRECT(0x4231, data);
}

static __inline HI_U8 hi_isp_sharpen_mid_tmp01_read(HI_VOID) {
	return (IORD_8DIRECT(0x4231));
}

static __inline HI_VOID hi_isp_sharpen_mid_tmp00_write(HI_U8 data) {
	IOWR_8DIRECT(0x4230, data);
}

static __inline HI_U8 hi_isp_sharpen_mid_tmp00_read(HI_VOID) {
	return (IORD_8DIRECT(0x4230));
}

static __inline HI_VOID hi_isp_sharpen_mid_tmp12_write(HI_U8 data) {
	IOWR_8DIRECT(0x4236, data);
}

static __inline HI_U8 hi_isp_sharpen_mid_tmp12_read(HI_VOID) {
	return (IORD_8DIRECT(0x4236));
}

static __inline HI_VOID hi_isp_sharpen_mid_tmp11_write(HI_U8 data) {
	IOWR_8DIRECT(0x4235, data);
}

static __inline HI_U8 hi_isp_sharpen_mid_tmp11_read(HI_VOID) {
	return (IORD_8DIRECT(0x4235));
}

static __inline HI_VOID hi_isp_sharpen_mid_tmp10_write(HI_U8 data) {
	IOWR_8DIRECT(0x4234, data);
}

static __inline HI_U8 hi_isp_sharpen_mid_tmp10_read(HI_VOID) {
	return (IORD_8DIRECT(0x4234));
}

static __inline HI_VOID hi_isp_sharpen_mid_tmp22_write(HI_U8 data) {
	IOWR_8DIRECT(0x423a, data);
}

static __inline HI_U8 hi_isp_sharpen_mid_tmp22_read(HI_VOID) {
	return (IORD_8DIRECT(0x423a));
}

static __inline HI_VOID hi_isp_sharpen_mid_tmp21_write(HI_U8 data) {
	IOWR_8DIRECT(0x4239, data);
}

static __inline HI_U8 hi_isp_sharpen_mid_tmp21_read(HI_VOID) {
	return (IORD_8DIRECT(0x4239));
}

static __inline HI_VOID hi_isp_sharpen_mid_tmp20_write(HI_U8 data) {
	IOWR_8DIRECT(0x4238, data);
}

static __inline HI_U8 hi_isp_sharpen_mid_tmp20_read(HI_VOID) {
	return (IORD_8DIRECT(0x4238));
}

static __inline HI_VOID hi_isp_sharpen_line_thd2_write(HI_U8 data) {
	IOWR_8DIRECT(0x423e, data);
}

static __inline HI_U8 hi_isp_sharpen_line_thd2_read(HI_VOID) {
	return (IORD_8DIRECT(0x423e));
}

static __inline HI_VOID hi_isp_sharpen_line_thd1_write(HI_U8 data) {
	IOWR_8DIRECT(0x423c, data);
}

static __inline HI_U8 hi_isp_sharpen_line_thd1_read(HI_VOID) {
	return (IORD_8DIRECT(0x423c));
}

static __inline HI_VOID hi_isp_edge_thd2_write(HI_U16 data) {
    IOWR_16DIRECT(0x4242, data);
}

static __inline HI_U16 hi_isp_edge_thd2_read(HI_VOID) {
    return (IORD_16DIRECT(0x4242));
}

static __inline HI_VOID hi_isp_sharp_thd2_write(HI_U16 data) {
    IOWR_16DIRECT(0x4240, data);
}

static __inline HI_U16 hi_isp_sharp_thd2_read(HI_VOID) {
    return (IORD_16DIRECT(0x4240));
}

static __inline HI_VOID hi_isp_sharpen_over_thd2_write(HI_U8 data) {
	IOWR_8DIRECT(0x4247, data);
}

static __inline HI_U8 hi_isp_sharpen_over_thd2_read(HI_VOID) {
	return (IORD_8DIRECT(0x4247));
}

static __inline HI_VOID hi_isp_sharpen_over_thd1_write(HI_U8 data) {
	IOWR_8DIRECT(0x4246, data);
}

static __inline HI_U8 hi_isp_sharpen_over_thd1_read(HI_VOID) {
	return (IORD_8DIRECT(0x4246));
}

static __inline HI_VOID hi_isp_sharpen_under_thd2_write(HI_U8 data) {
	IOWR_8DIRECT(0x4245, data);
}

static __inline HI_U8 hi_isp_sharpen_under_thd2_read(HI_VOID) {
	return (IORD_8DIRECT(0x4245));
}

static __inline HI_VOID hi_isp_sharpen_under_thd1_write(HI_U8 data) {
	IOWR_8DIRECT(0x4244, data);
}

static __inline HI_U8 hi_isp_sharpen_under_thd1_read(HI_VOID) {
	return (IORD_8DIRECT(0x4244));
}

static __inline HI_VOID hi_isp_sharpen_over_coef_write(HI_U16 data) {
	IOWR_16DIRECT(0x424a, data&0x1ff);
}

static __inline HI_U16 hi_isp_sharpen_over_coef_read(HI_VOID) {
	return (IORD_16DIRECT(0x424a));
}

static __inline HI_VOID hi_isp_sharpen_under_coef_write(HI_U16 data) {
	IOWR_16DIRECT(0x4248, data&0x1ff);
}

static __inline HI_U16 hi_isp_sharpen_under_coef_read(HI_VOID) {
	return (IORD_16DIRECT(0x4248));
}

static __inline HI_VOID hi_isp_sharpen_enpixsel_write(HI_U8 data) {
	IOWR_8DIRECT(0x424c, (data&0x1));
}

static __inline HI_U8 hi_isp_sharpen_enpixsel_read(HI_VOID) {
	return (IORD_8DIRECT(0x424c) & 0x1);
}

static __inline HI_VOID hi_isp_sharpen_height_write(HI_U16 data) {
	IOWR_16DIRECT(0x42f2, (data&0x1fff));
}

static __inline HI_U16 hi_isp_sharpen_height_read(HI_VOID) {
	return (IORD_16DIRECT(0x42f2)&0x1fff);
}

static __inline HI_VOID hi_isp_sharpen_width_write(HI_U16 data) {
	IOWR_16DIRECT(0x42f0, (data&0x1fff));
}

static __inline HI_U8 hi_isp_sharpen_width_read(HI_VOID) {
	return (IORD_16DIRECT(0x42f0)&0x1fff);
}

/*****************************************************
MODULE        -----UVNR-----
MODULE        -----UVNR-----
MODULE        -----UVNR-----
*****************************************************/
static __inline HI_VOID hi_isp_uvnr_sel_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x1601);
	IOWR_8DIRECT(0x1601, (data & 0x1) << 4 | (u8Current & 0xef));
}

static __inline HI_U8 hi_isp_uvnr_sel_read(HI_VOID) {
	return ((IORD_8DIRECT(0x1601)&0x10) >> 4);
}

static __inline HI_VOID hi_isp_nr_en_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x1601);
	IOWR_8DIRECT(0x1601, (data & 0x1) | (u8Current & 0xfe));
}

static __inline HI_U8 hi_isp_nr_en_read(HI_VOID) {
	return ((IORD_8DIRECT(0x1601)&0x1));
}

static __inline HI_VOID hi_isp_midf_en_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x1600);
	IOWR_8DIRECT(0x1600, (data & 0x1) << 4 | (u8Current & 0xef));
}

static __inline HI_U8 hi_isp_midf_en_read(HI_VOID) {
	return ((IORD_8DIRECT(0x1600)&0x10) >> 4);
}

static __inline HI_VOID hi_isp_uv2c_mode_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x1600);
	IOWR_8DIRECT(0x1600, (data & 0x1) << 3 | (u8Current & 0xf7));
}

static __inline HI_U8 hi_isp_uv2c_mode_read(HI_VOID) {
	return ((IORD_8DIRECT(0x1600)&0x8) >> 3);
}

static __inline HI_VOID hi_isp_uv2c_en_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x1600);
	IOWR_8DIRECT(0x1600, (data & 0x1) << 2 | (u8Current & 0xfb));
}

static __inline HI_U8 hi_isp_uv2c_en_read(HI_VOID) {
	return ((IORD_8DIRECT(0x1600)&0x4) >> 2);
}

static __inline HI_VOID hi_isp_vcds_en_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x1600);
	IOWR_8DIRECT(0x1600, (data & 0x1) << 1 | (u8Current & 0xfd));
}

static __inline HI_U8 hi_isp_vcds_en_read(HI_VOID) {
	return ((IORD_8DIRECT(0x1600)&0x2) >> 1);
}

static __inline HI_VOID hi_isp_hcds_en_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x1600);
	IOWR_8DIRECT(0x1600, (data & 0x1)| (u8Current & 0xfe));
}

static __inline HI_U8 hi_isp_hcds_en_read(HI_VOID) {
	return ((IORD_8DIRECT(0x1600)&0x1));
}

static __inline HI_VOID hi_isp_hcds_width_out_write(HI_U16 data) {
	IOWR_16DIRECT(0x1616, (data&0x1fff));
}

static __inline HI_U16 hi_isp_hcds_width_out_read(HI_VOID) {
	return (IORD_16DIRECT(0x1616)&0x1fff);
}

static __inline HI_VOID hi_isp_hcds_width_in_write(HI_U16 data) {
	IOWR_16DIRECT(0x1614, (data&0x1fff));
}

static __inline HI_U16 hi_isp_hcds_width_in_read(HI_VOID) {
	return (IORD_16DIRECT(0x1614)&0x1fff);
}

static __inline HI_VOID hi_isp_vcus_mode_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x1618);
	IOWR_8DIRECT(0x1618, ((data & 0x1) << 2)| (u8Current & 0xfb));
}

static __inline HI_U8 hi_isp_vcus_mode_read(HI_VOID) {
	return ((IORD_8DIRECT(0x1618)&0x4) >> 2);
}

static __inline HI_VOID hi_isp_vcds_mode_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x1618);
	IOWR_8DIRECT(0x1618, ((data & 0x1) << 1)| (u8Current & 0xfd));
}

static __inline HI_U8 hi_isp_vcds_mode_read(HI_VOID) {
	return ((IORD_8DIRECT(0x1618)&0x2) >> 1);
}

static __inline HI_VOID hi_isp_hcds_mode_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x1618);
	IOWR_8DIRECT(0x1618, (data & 0x1)| (u8Current & 0xfe));
}

static __inline HI_U8 hi_isp_hcds_mode_read(HI_VOID) {
	return (IORD_8DIRECT(0x1618)&0x1);
}

static __inline HI_VOID hi_isp_hcds_coef2_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT(0x161c);
	IOWR_32DIRECT(0x161c, ((data & 0x3ff) << 20) | (u32Current & 0xc00fffff));
}

static __inline HI_U32 hi_isp_hcds_coef2_read(HI_VOID) {
	return ((IORD_32DIRECT(0x161c)&0x3ff00000) >> 20);
}

static __inline HI_VOID hi_isp_hcds_coef1_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT(0x161c);
	IOWR_32DIRECT(0x161c, ((data & 0x3ff) << 10) | (u32Current & 0xfff003ff));
}

static __inline HI_U32 hi_isp_hcds_coef1_read(HI_VOID) {
	return ((IORD_32DIRECT(0x161c)&0xffc00) >> 10);
}

static __inline HI_VOID hi_isp_hcds_coef0_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT(0x161c);
	IOWR_32DIRECT(0x161c, (data & 0x3ff) | (u32Current & 0xfffffc00));
}

static __inline HI_U32 hi_isp_hcds_coef0_read(HI_VOID) {
	return ((IORD_32DIRECT(0x161c)&0x3ff));
}

static __inline HI_VOID hi_isp_hcds_coef5_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT(0x1620);
	IOWR_32DIRECT(0x1620, ((data & 0x3ff) << 20) | (u32Current & 0xc00fffff));
}

static __inline HI_U32 hi_isp_hcds_coef5_read(HI_VOID) {
	return ((IORD_32DIRECT(0x1620)&0x3ff00000) >> 20);
}

static __inline HI_VOID hi_isp_hcds_coef4_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT(0x1620);
	IOWR_32DIRECT(0x1620, ((data & 0x3ff) << 10) | (u32Current & 0xfff003ff));
}

static __inline HI_U32 hi_isp_hcds_coef4_read(HI_VOID) {
	return ((IORD_32DIRECT(0x1620)&0xffc00) >> 10);
}

static __inline HI_VOID hi_isp_hcds_coef3_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT(0x1620);
	IOWR_32DIRECT(0x1620, (data & 0x3ff) | (u32Current & 0xfffffc00));
}

static __inline HI_U32 hi_isp_hcds_coef3_read(HI_VOID) {
	return ((IORD_32DIRECT(0x1620)&0x3ff));
}

static __inline HI_VOID hi_isp_hcds_coef7_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT(0x1624);
	IOWR_32DIRECT(0x1624, ((data & 0x3ff) << 10) | (u32Current & 0xfff003ff));
}

static __inline HI_U32 hi_isp_hcds_coef7_read(HI_VOID) {
	return ((IORD_32DIRECT(0x1624)&0xffc00) >> 10);
}

static __inline HI_VOID hi_isp_hcds_coef6_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT(0x1624);
	IOWR_32DIRECT(0x1624, (data & 0x3ff) | (u32Current & 0xfffffc00));
}

static __inline HI_U32 hi_isp_hcds_coef6_read(HI_VOID) {
	return ((IORD_32DIRECT(0x1624)&0x3ff));
}

static __inline HI_VOID hi_isp_vcds_coef3_write(HI_U8 data) {
	IOWR_8DIRECT(0x162b, (data & 0x7));
}

static __inline HI_U8 hi_isp_vcds_coef3_read(HI_VOID) {
	return (IORD_8DIRECT(0x162b)&0x7);
}

static __inline HI_VOID hi_isp_vcds_coef2_write(HI_U8 data) {
	IOWR_8DIRECT(0x162a, (data & 0x7));
}

static __inline HI_U8 hi_isp_vcds_coef2_read(HI_VOID) {
	return (IORD_8DIRECT(0x162a)&0x7);
}

static __inline HI_VOID hi_isp_vcds_coef1_write(HI_U8 data) {
	IOWR_8DIRECT(0x1629, (data & 0x7));
}

static __inline HI_U8 hi_isp_vcds_coef1_read(HI_VOID) {
	return (IORD_8DIRECT(0x1629)&0x7);
}

static __inline HI_VOID hi_isp_vcds_coef0_write(HI_U8 data) {
	IOWR_8DIRECT(0x1628, (data & 0x7));
}

static __inline HI_U8 hi_isp_vcds_coef0_read(HI_VOID) {
	return (IORD_8DIRECT(0x1628)&0x7);
}

static __inline HI_VOID hi_isp_midfilter_width_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT(0x162c);
	IOWR_32DIRECT(0x162c, (u32Current & 0xffffe000) | (data & 0x1fff));
}

static __inline HI_U16 hi_isp_midfilter_width_read(HI_VOID) {
	return (IORD_32DIRECT(0x162c) & 0x1fff);
}

static __inline HI_VOID hi_isp_midfilter_height_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT(0x162c);
	IOWR_32DIRECT(0x162c, (u32Current & 0xe000ffff) | ((data & 0x1fff) << 16));
}

static __inline HI_U16 hi_isp_midfilter_height_read(HI_VOID) {
	return ((IORD_32DIRECT(0x162c) & 0x1fff0000) >> 16);
}

static __inline HI_VOID hi_isp_uvnr_width_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT(0x1630);
	IOWR_32DIRECT(0x1630, (u32Current & 0xffffe000) | (data & 0x1fff));
}

static __inline HI_U16 hi_isp_uvnr_width_read(HI_VOID) {
	return (IORD_32DIRECT(0x1630) & 0x1fff);
}

static __inline HI_VOID hi_isp_uvnr_height_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT(0x1630);
	IOWR_32DIRECT(0x1630, (u32Current & 0xe000ffff) | ((data & 0x1fff) << 16));
}

static __inline HI_U16 hi_isp_uvnr_height_read(HI_VOID) {
	return ((IORD_32DIRECT(0x1630) & 0x1fff0000) >> 16);
}

static __inline HI_VOID hi_isp_uvnr_diffthres4_write(HI_U8 data) {
	IOWR_8DIRECT(0x1637, data);
}

static __inline HI_U8 hi_isp_uvnr_diffthres4_read(HI_VOID) {
	return IORD_8DIRECT(0x1637);
}

static __inline HI_VOID hi_isp_uvnr_diffthres3_write(HI_U8 data) {
	IOWR_8DIRECT(0x1636, data);
}

static __inline HI_U8 hi_isp_uvnr_diffthres3_read(HI_VOID) {
	return IORD_8DIRECT(0x1636);
}

static __inline HI_VOID hi_isp_uvnr_diffthres2_write(HI_U8 data) {
	IOWR_8DIRECT(0x1635, data);
}

static __inline HI_U8 hi_isp_uvnr_diffthres2_read(HI_VOID) {
	return IORD_8DIRECT(0x1635);
}

static __inline HI_VOID hi_isp_uvnr_diffthres1_write(HI_U8 data) {
	IOWR_8DIRECT(0x1634, data);
}

static __inline HI_U8 hi_isp_uvnr_diffthres1_read(HI_VOID) {
	return IORD_8DIRECT(0x1634);
}

static __inline HI_VOID hi_isp_uvnr_diffthres6_write(HI_U8 data) {
	IOWR_8DIRECT(0x1639, data);
}

static __inline HI_U8 hi_isp_uvnr_diffthres6_read(HI_VOID) {
	return IORD_8DIRECT(0x1639);
}

static __inline HI_VOID hi_isp_uvnr_diffthres5_write(HI_U8 data) {
	IOWR_8DIRECT(0x1638, data);
}

static __inline HI_U8 hi_isp_uvnr_diffthres5_read(HI_VOID) {
	return IORD_8DIRECT(0x1638);
}

static __inline HI_VOID hi_isp_uvnr_ratiolimit3_write(HI_U8 data) {
	IOWR_8DIRECT(0x163f, data&0x7f);
}

static __inline HI_U8 hi_isp_uvnr_ratiolimit3_read(HI_VOID) {
	return IORD_8DIRECT(0x163f);
}

static __inline HI_VOID hi_isp_uvnr_ratiolimit2_write(HI_U8 data) {
	IOWR_8DIRECT(0x163e, data&0x7f);
}

static __inline HI_U8 hi_isp_uvnr_ratiolimit2_read(HI_VOID) {
	return IORD_8DIRECT(0x163e);
}

static __inline HI_VOID hi_isp_uvnr_ratiolimit1_write(HI_U8 data) {
	IOWR_8DIRECT(0x163d, data&0x7f);
}

static __inline HI_U8 hi_isp_uvnr_ratiolimit1_read(HI_VOID) {
	return IORD_8DIRECT(0x163d);
}

static __inline HI_VOID hi_isp_uvnr_ratiolimit0_write(HI_U8 data) {
	IOWR_8DIRECT(0x163c, data&0x7f);
}

static __inline HI_U8 hi_isp_uvnr_ratiolimit0_read(HI_VOID) {
	return IORD_8DIRECT(0x163c);
}

static __inline HI_VOID hi_isp_uvnr_ratiolimit6_write(HI_U8 data) {
	IOWR_8DIRECT(0x1642, data&0x7f);
}

static __inline HI_U8 hi_isp_uvnr_ratiolimit6_read(HI_VOID) {
	return IORD_8DIRECT(0x1642);
}

static __inline HI_VOID hi_isp_uvnr_ratiolimit5_write(HI_U8 data) {
	IOWR_8DIRECT(0x1641, data&0x7f);
}

static __inline HI_U8 hi_isp_uvnr_ratiolimit5_read(HI_VOID) {
	return IORD_8DIRECT(0x1641);
}

static __inline HI_VOID hi_isp_uvnr_ratiolimit4_write(HI_U8 data) {
	IOWR_8DIRECT(0x1640, data&0x7f);
}

static __inline HI_U8 hi_isp_uvnr_ratiolimit4_read(HI_VOID) {
	return IORD_8DIRECT(0x1640);
}

static __inline HI_VOID hi_isp_uvnr_coring_limit_write(HI_U8 data) {
	IOWR_8DIRECT(0x1646, data&0x3f);
}

static __inline HI_U8 hi_isp_uvnr_coring_limit_read(HI_VOID) {
	return (IORD_8DIRECT(0x1646));
}

static __inline HI_VOID hi_isp_uvnr_blendratio_write(HI_U8 data) {
	IOWR_8DIRECT(0x1644, data&0x3f);
}

static __inline HI_U8 hi_isp_uvnr_blendratio_read(HI_VOID) {
	return (IORD_8DIRECT(0x1644));
}

static __inline HI_VOID hi_isp_uvnr_diffshift_write(HI_U8 data) {
	IOWR_8DIRECT(0x1645, data&0x3);
}

static __inline HI_U8 hi_isp_uvnr_diffshift_read(HI_VOID) {
	return (IORD_8DIRECT(0x1645));
}

static __inline HI_VOID hi_isp_uvnr_hsize_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT(0x16f0);
	IOWR_32DIRECT(0x16f0, (u32Current & 0xffffe000) | (data & 0x1fff));
}

static __inline HI_U16 hi_isp_uvnr_hsize_read(HI_VOID) {
	return (IORD_32DIRECT(0x16f0) & 0x1fff);
}

static __inline HI_VOID hi_isp_uvnr_vsize_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT(0x16f0);
	IOWR_32DIRECT(0x16f0, (u32Current & 0xe000ffff) | ((data & 0x1fff) << 16));
}

static __inline HI_U16 hi_isp_uvnr_vsize_read(HI_VOID) {
	return ((IORD_32DIRECT(0x16f0) & 0x1fff0000) >> 16);
}

#define HI_ISP_MIDFILTER_TIMING_DEFAULT (0x20)

static __inline HI_VOID hi_isp_midfilter_timing_write(HI_U16 data) {
	HI_U16 u16Current = IORD_16DIRECT(0x16e8);
	IOWR_16DIRECT(0x16e8, ((data&0x1fff) << 1) | (u16Current & 0x1));
}

static __inline HI_U16 hi_isp_midfilter_timing_read(HI_VOID) {
	return ((IORD_16DIRECT(0x16e8)&0x3ffe) >> 1);
}

static __inline HI_VOID hi_isp_midfilter_auto_timing_en_write(HI_U8 data) {
	HI_U16 u16Current = IORD_16DIRECT(0x16e8);
	IOWR_16DIRECT(0x16e8, (data&0x1) | (u16Current & 0x3ffe));
}

static __inline HI_U8 hi_isp_midfilter_auto_timing_en_read(HI_VOID) {
	return ((IORD_16DIRECT(0x16e8)&0x1));
}

/*****************************************************
MODULE        -----STAT-----
MODULE        -----STAT-----
MODULE        -----STAT-----
*****************************************************/
/*****************************************************
ISP_STAT_EN:      stat Enable
0x20584000: 	   31:02	 reserved
				   01:01 drc_rd_en DRC read enable
				   00:00 stat_en	
*****************************************************/
static __inline HI_VOID hi_isp_stat_en_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT_VI(0x4000);
	IOWR_32DIRECT_VI(0x4000, (data & 0x1) | (u32Current & 0x2));
}

static __inline HI_U8 hi_isp_stat_en_read(HI_VOID) {
	return (IORD_32DIRECT_VI(0x4000) & 0x1);
}

static __inline HI_VOID hi_isp_drc_rd_en_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT_VI(0x4000);
	IOWR_32DIRECT_VI(0x4000, ((data & 0x1) << 1) | (u32Current & 0x1));
}

static __inline HI_U8 hi_isp_drc_rd_en_read(HI_VOID) {
	return ((IORD_32DIRECT_VI(0x4000) & 0x2) >> 1);
}


/*****************************************************
ISP_STAT_FRM_BADDR:	   stat base frame addr
0x20584008: 		               31:12	 reserved
					               11:00  stat_frm_baddr    
*****************************************************/
static __inline HI_VOID hi_isp_stat_frm_baddr_write(HI_U32 data) {
	IOWR_32DIRECT_VI(0x4008, (data & 0xfff));
}

static __inline HI_U32 hi_isp_stat_frm_baddr_read(HI_VOID) {
	return (IORD_32DIRECT_VI(0x4008) & 0xfff);
}

/*****************************************************
ISP_STAT_DRCA_FRM_BADDR:	   drca frame base addr
0x20584020: 		               31:20	 reserved
					               19:00  drca_frm_baddr   
*****************************************************/
static __inline HI_VOID hi_isp_drca_frm_baddr_write(HI_U32 data) {
	IOWR_32DIRECT_VI(0x4020, (data & 0xfffff));
}

static __inline HI_U32 hi_isp_drca_frm_baddr_read(HI_VOID) {
	return (IORD_32DIRECT_VI(0x4020) & 0xfffff);
}

/*****************************************************
ISP_STAT_DRCB_FRM_BADDR:	   drcb frame base addr
0x20584024: 		               31:20	 reserved
					               19:00  drcb_frm_baddr  
*****************************************************/
static __inline HI_VOID hi_isp_drcb_frm_baddr_write(HI_U32 data) {
	IOWR_32DIRECT_VI(0x4024, (data & 0xfffff));
}

static __inline HI_U32 hi_isp_drcb_frm_baddr_read(HI_VOID) {
	return (IORD_32DIRECT_VI(0x4024) & 0xfffff);
}

/*****************************************************
ISP_STAT_DRCA_RD_FRM_BADDR:	   drca read frame base addr
0x20584028: 		               31:20	 reserved
					               19:00  drca_rd_frm_baddr    
*****************************************************/
static __inline HI_VOID hi_isp_drca_rd_frm_baddr_write(HI_U32 data) {
	IOWR_32DIRECT_VI(0x4028, (data & 0xfffff));
}

static __inline HI_U32 hi_isp_drca_rd_frm_baddr_read(HI_VOID) {
	return (IORD_32DIRECT_VI(0x4028) & 0xfffff);
}

/*****************************************************
ISP_STAT_DRCB_RD_FRM_BADDR:	   drcb read frame base addr
0x2058402c: 		               31:20	 reserved
					               19:00  drcb_rd_frm_baddr  
*****************************************************/
static __inline HI_VOID hi_isp_drcb_rd_frm_baddr_write(HI_U32 data) {
	IOWR_32DIRECT_VI(0x402c, (data & 0xfffff));
}

static __inline HI_U32 hi_isp_drcb_rd_frm_baddr_read(HI_VOID) {
	return (IORD_32DIRECT_VI(0x402c) & 0xfffff);
}

/*****************************************************
MODULE        -----LSC-----
MODULE        -----LSC-----
MODULE        -----LSC-----
*****************************************************/
static __inline HI_VOID hi_isp_lsc_cfg_enable_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x5800);
    IOWR_8DIRECT(0x5800, (data & 0x1) | (u8Current & 0xfe));
}

static __inline HI_U8 hi_isp_lsc_cfg_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x5800) & 0x1);
}

static __inline HI_U32 hi_isp_lsc_version_read(HI_VOID) {
    return (IORD_32DIRECT(0x580c));
}

static __inline HI_VOID hi_isp_lsc_win_num_write(HI_U8 numh, HI_U8 numv){
	HI_U32 u32Current = IORD_32DIRECT(0x5810);
	IOWR_32DIRECT(0x5810, (u32Current & 0xff00ff00) | ((numv & 0xff) << 16) | (numh & 0xff));
}

static __inline HI_U8 hi_isp_lsc_win_numh_read(HI_VOID) {
    return (IORD_32DIRECT(0x5810) & 0xfffffff0);
}

static __inline HI_U8 hi_isp_lsc_win_numv_read(HI_VOID) {
    return ((IORD_32DIRECT(0x5810) & 0xfff0ffff) >> 16);
}

static __inline HI_VOID hi_isp_lsc_hwin_info_write(HI_U8 index, HI_U16 delta, HI_U16 inv){
	HI_U32 u32Current = IORD_32DIRECT(0x5814 + (index * 0x4));
	IOWR_32DIRECT((0x5814 + (index * 0x4)), ((u32Current & 0xf000fc00) | (delta&0x3ff) | ((inv&0xfff) << 16)));
}

static __inline HI_U16 hi_isp_lsc_hwin_deltax_read(HI_U8 index){
	return (IORD_32DIRECT((0x5814 + (index * 0x4))) & 0x3ff);
}

static __inline HI_U16 hi_isp_lsc_hwin_invx_read(HI_U8 index){
	return ((IORD_32DIRECT((0x5814 + (index * 0x4))) & 0xfff0000) >> 16);
}

static __inline HI_VOID hi_isp_lsc_vwin_info_write(HI_U8 index, HI_U16 delta, HI_U16 inv){
	HI_U32 u32Current = IORD_32DIRECT(0x5834 + (index * 0x4));
	IOWR_32DIRECT((0x5834 + (index * 0x4)), ((u32Current & 0xf000fc00) | (delta&0x3ff) | ((inv&0xfff) << 16)));
}

static __inline HI_U16 hi_isp_lsc_vwin_deltay_read(HI_U8 index){
	return (IORD_32DIRECT((0x5834 + (index * 0x4))) & 0x3ff);
}

static __inline HI_U16 hi_isp_lsc_vwin_invy_read(HI_U8 index){
	return ((IORD_32DIRECT((0x5834 + (index * 0x4))) & 0xfff0000) >> 16);
}

static __inline HI_VOID hi_isp_lsc_grr_gain_waddr_write(HI_U32 data){
	IOWR_32DIRECT(0x5880, data);
}

static __inline HI_U32 hi_isp_lsc_grr_gain_waddr_read(HI_VOID) {
    return (IORD_32DIRECT(0x5880));
}

static __inline HI_VOID hi_isp_lsc_grr_gain_wdata_write(HI_U32 data){
    IOWR_32DIRECT(0x5884, data);
}

static __inline HI_U32 hi_isp_lsc_grr_gain_wdata_read(HI_VOID) {
    return (IORD_32DIRECT(0x5884));
}

static __inline HI_VOID hi_isp_lsc_grr_gain_raddr_write(HI_U32 data){
    IOWR_32DIRECT(0x5888, data);
}

static __inline HI_U32 hi_isp_lsc_grr_gain_raddr_read(HI_VOID) {
    return (IORD_32DIRECT(0x5888));
}

static __inline HI_U32 hi_isp_lsc_grr_gain_rdata_read(HI_VOID) {
    return (IORD_32DIRECT(0x588c));
}

static __inline HI_VOID hi_isp_lsc_gbb_gain_waddr_write(HI_U32 data){
	IOWR_32DIRECT(0x5890, data);
}

static __inline HI_U32 hi_isp_lsc_gbb_gain_waddr_read(HI_VOID) {
    return (IORD_32DIRECT(0x5890));
}

static __inline HI_VOID hi_isp_lsc_gbb_gain_wdata_write(HI_U32 data){
    IOWR_32DIRECT(0x5894, data);
}

static __inline HI_U32 hi_isp_lsc_gbb_gain_wdata_read(HI_VOID) {
    return (IORD_32DIRECT(0x5894));
}

static __inline HI_VOID hi_isp_lsc_gbb_gain_raddr_write(HI_U32 data){
    IOWR_32DIRECT(0x5898, data);
}

static __inline HI_U32 hi_isp_lsc_gbb_gain_raddr_read(HI_VOID) {
    return (IORD_32DIRECT(0x5898));
}

static __inline HI_U32 hi_isp_lsc_gbb_gain_rdata_read(HI_VOID) {
    return (IORD_32DIRECT(0x589c));
}

static __inline HI_VOID hi_isp_lsc_rggb_cfg_write(HI_U8 data){
    HI_U8 u8Current = IORD_8DIRECT(0x58e0);
	IOWR_8DIRECT(0x58e0, (u8Current & 0xfc) | (data & 0x3));
}

static __inline HI_U8 hi_isp_lsc_rggb_cfg_read(HI_VOID) {
    return ((IORD_8DIRECT(0x58e0) & 0x3));
}

static __inline HI_VOID hi_isp_lsc_update_mode_write(HI_U8 data){
	IOWR_8DIRECT(0x58e4, data);
}

static __inline HI_U8 hi_isp_lsc_update_mode_read(HI_VOID) {
    return (IORD_8DIRECT(0x58e4));
}

static __inline HI_VOID hi_isp_lsc_update_newer_write(HI_U8 data){
	IOWR_8DIRECT(0x58ec, data);
}

static __inline HI_U8 hi_isp_lsc_update_newer_read(HI_VOID) {
    return (IORD_8DIRECT(0x58ec));
}

static __inline HI_VOID hi_isp_colorbar_cfg_write(HI_U8 data){
	    HI_U8 u8Current = IORD_8DIRECT(0x8100);
    IOWR_8DIRECT(0x8100, (data & 0x1) | (u8Current & 0xfe));
}
static __inline HI_U8 hi_isp_colorbar_cfg_read(HI_VOID) {
	return (IORD_8DIRECT(0x8100) & 0x1);
}


/*****************************************************
MODULE        -----ACM-----
MODULE        -----ACM-----
MODULE        -----ACM-----
*****************************************************/
static __inline HI_VOID hi_isp_acm_enable_write(HI_U8 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x11400);
    IOWR_32DIRECT_ISP_REG(0x11400, (u32current & (~(1 << 31))) | ((data&0x1) << 31));
}

static __inline HI_U8 hi_isp_acm_enable_read(HI_VOID) {
    return ((IORD_32DIRECT_ISP_REG(0x11400) >> 31) & 0x1);
}

static __inline HI_VOID hi_isp_acm_dbg_enable_write(HI_U8 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x11400);
    IOWR_32DIRECT_ISP_REG(0x11400, (u32current & (~(1 << 30))) | ((data&0x1) << 30));
}

static __inline HI_U8 hi_isp_acm_dbg_enable_read(HI_VOID) {
    return ((IORD_32DIRECT_ISP_REG(0x11400) >> 30) & 0x1);
}

static __inline HI_VOID hi_isp_acm_stretch_write(HI_U8 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x11400);
    IOWR_32DIRECT_ISP_REG(0x11400, (u32current & (~(1 << 29))) | ((data&0x1) << 29));
}

static __inline HI_U8 hi_isp_acm_stretch_read(HI_VOID) {
    return ((IORD_32DIRECT_ISP_REG(0x11400) >> 29) & 0x1);
}

static __inline HI_VOID hi_isp_acm_cliprange_write(HI_U8 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x11400);
    IOWR_32DIRECT_ISP_REG(0x11400, (u32current & (~(1 << 28))) | ((data&0x1) << 28));
}

static __inline HI_U8 hi_isp_acm_cliprange_read(HI_VOID) {
    return ((IORD_32DIRECT_ISP_REG(0x11400) >> 28) & 0x1);
}

static __inline HI_VOID hi_isp_acm_cliporwrap_write(HI_U8 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x11400);
    IOWR_32DIRECT_ISP_REG(0x11400, (u32current & (~(1 << 27))) | ((data&0x1) << 27));
}

static __inline HI_U8 hi_isp_acm_cliporwrap_read(HI_VOID) {
    return ((IORD_32DIRECT_ISP_REG(0x11400) >> 27) & 0x1);
}

static __inline HI_VOID hi_isp_acm_cbcrthr_write(HI_U8 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x11400);
    IOWR_32DIRECT_ISP_REG(0x11400, (u32current & (~(0xff))) | (data & 0xff));
}

static __inline HI_U8 hi_isp_acm_cbcrthr_read(HI_VOID) {
    return (IORD_32DIRECT_ISP_REG(0x11400) & 0xff);
}

static __inline HI_VOID hi_isp_acm_luma_gain_write(HI_U16 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x11404);
    IOWR_32DIRECT_ISP_REG(0x11404, (u32current & 0xc00fffff) | ((data & 0x3ff) << 20));
}

static __inline HI_U16 hi_isp_acm_luma_gain_read(HI_VOID) {
    return ((IORD_32DIRECT_ISP_REG(0x11404) & 0x3ff00000) >> 20);
}

static __inline HI_VOID hi_isp_acm_hue_gain_write(HI_U16 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x11404);
    IOWR_32DIRECT_ISP_REG(0x11404, (u32current & 0xfff003ff) | ((data & 0x3ff) << 10));
}

static __inline HI_U16 hi_isp_acm_hue_gain_read(HI_VOID) {
    return ((IORD_32DIRECT_ISP_REG(0x11404) & 0x000ffc00) >> 10);
}

static __inline HI_VOID hi_isp_acm_sat_gain_write(HI_U16 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x11404);
    IOWR_32DIRECT_ISP_REG(0x11404, (u32current & 0xfffffc00) | (data & 0x3ff));
}

static __inline HI_U16 hi_isp_acm_sat_gain_read(HI_VOID) {
    return ((IORD_32DIRECT_ISP_REG(0x11404) & 0x3ff));
}

static __inline HI_VOID hi_isp_acm_height_write(HI_U16 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x11410);
    IOWR_32DIRECT_ISP_REG(0x11410, (u32current & 0xe000ffff) | ((data & 0x1fff) << 16));
}

static __inline HI_U16 hi_isp_acm_height_read(HI_VOID) {
    return ((IORD_32DIRECT_ISP_REG(0x11410) & 0x1fff0000) >> 16);
}

static __inline HI_VOID hi_isp_acm_width_write(HI_U16 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x11410);
    IOWR_32DIRECT_ISP_REG(0x11410, (u32current & 0xffffe000) | (data & 0x1fff));
}

static __inline HI_U16 hi_isp_acm_width_read(HI_VOID) {
    return (IORD_32DIRECT_ISP_REG(0x11410) & 0x1fff);
}

static __inline HI_VOID hi_isp_acm_coeff_addr_write(HI_U32 data){
    IOWR_32DIRECT_ISP_REG(0x11414, data);
}

static __inline HI_U32 hi_isp_acm_coeff_addr_read(HI_VOID) {
    return (IORD_32DIRECT_ISP_REG(0x11414));
}

static __inline HI_VOID hi_isp_acm_coeff_up_write(HI_U32 data){
    IOWR_32DIRECT_ISP_REG(0x11420, data);
}

static __inline HI_U32 hi_isp_acm_coeff_up_read(HI_VOID) {
    return (IORD_32DIRECT_ISP_REG(0x11420));
}



/*****************************************************
MODULE        -----ISP_CH-----
MODULE        -----ISP_CH-----
MODULE        -----ISP_CH-----
*****************************************************/
static __inline HI_VOID hi_isp_ch_ctrl_en_write(HI_U8 data){
	IOWR_32DIRECT_ISP_REG(0x12000, (data&0x1) << 31);
}

static __inline HI_U8 hi_isp_ch_ctrl_en_read(HI_VOID) {
	return ((IORD_32DIRECT_ISP_REG(0x12000) & 0x80000000) >> 31);
}

/*****************************************************
MODULE        -----DEHAZE-----
MODULE        -----DEHAZE-----
MODULE        -----DEHAZE-----
*****************************************************/

static __inline HI_VOID hi_isp_dehaze_en_write(HI_U8 data){
	IOWR_8DIRECT(0x7000, data);
}

static __inline HI_U8 hi_isp_dehaze_en_read(HI_VOID) {
	return (IORD_8DIRECT(0x7000));
}

static __inline HI_VOID hi_isp_dehaze_pre_update_write(HI_U8 data){
	IOWR_8DIRECT(0x7010, data);
}

static __inline HI_U8 hi_isp_dehaze_pre_update_read(HI_VOID) {
	return (IORD_8DIRECT(0x7010));
}

static __inline HI_VOID hi_isp_dehaze_block_sizeh_write(HI_U16 data){
	HI_U32 u32Current = IORD_32DIRECT(0x7014);
	IOWR_32DIRECT(0x7014, (u32Current & 0xfe00ffff) | ((data & 0x1ff) << 16));
}

static __inline HI_U16 hi_isp_dehaze_block_sizeh_read(HI_VOID) {
	return ((IORD_32DIRECT(0x7014)&0x1ff0000) >> 16);
}

static __inline HI_VOID hi_isp_dehaze_block_sizev_write(HI_U16 data){
	HI_U32 u32Current = IORD_32DIRECT(0x7014);
	IOWR_32DIRECT(0x7014, (u32Current & 0xfffffe00) | (data & 0x1ff));
}

static __inline HI_U16 hi_isp_dehaze_block_sizev_read(HI_VOID) {
	return (IORD_32DIRECT(0x7014)&0x1ff);
}

static __inline HI_VOID hi_isp_dehaze_block_numh_write(HI_U8 data){
	HI_U32 u32Current = IORD_32DIRECT(0x7018);
	IOWR_32DIRECT(0x7018, (u32Current & 0xffe0ffff) | ((data & 0x1f) << 16));
}

static __inline HI_U8 hi_isp_dehaze_block_numh_read(HI_VOID) {
	return ((IORD_32DIRECT(0x7018)&0x1f0000) >> 16);
}

static __inline HI_VOID hi_isp_dehaze_block_numv_write(HI_U8 data){
	HI_U32 u32Current = IORD_32DIRECT(0x7018);
	IOWR_32DIRECT(0x7018, (u32Current & 0xffffffe0) | (data & 0x1f));
}

static __inline HI_U8 hi_isp_dehaze_block_numv_read(HI_VOID) {
	return (IORD_32DIRECT(0x7018)&0x1f);
}

static __inline HI_VOID hi_isp_dehaze_dc_numh_write(HI_U16 data){
	HI_U16 u16Current = IORD_16DIRECT(0x701c);
	IOWR_16DIRECT(0x701c, (u16Current & 0xfc1f) | ((data & 0x1f) << 5));
}

static __inline HI_U16 hi_isp_dehaze_dc_numh_read(HI_VOID) {
	return ((IORD_16DIRECT(0x701c)&0x3e0) >> 5);
}

static __inline HI_VOID hi_isp_dehaze_dc_numv_write(HI_U16 data){
	HI_U16 u16Current = IORD_16DIRECT(0x701c);
	IOWR_16DIRECT(0x701c, (u16Current & 0xffe0) | (data & 0x1f));
}

static __inline HI_U16 hi_isp_dehaze_dc_numv_read(HI_VOID) {
	return (IORD_16DIRECT(0x701c)&0x1f);
}

static __inline HI_VOID hi_isp_dehaze_phase_x_write(HI_U32 data){
	HI_U32 u32Current = IORD_32DIRECT(0x7020);
	IOWR_32DIRECT(0x7020, (u32Current & 0xffc00000) | (data & 0x3fffff));
}

static __inline HI_U32 hi_isp_dehaze_phase_x_read(HI_VOID) {
	return ((IORD_32DIRECT(0x7020)&0x3fffff));
}

static __inline HI_VOID hi_isp_dehaze_phase_y_write(HI_U32 data){
	HI_U32 u32Current = IORD_32DIRECT(0x7024);
	IOWR_32DIRECT(0x7024, (u32Current & 0xffc00000) | (data & 0x3fffff));
}

static __inline HI_U32 hi_isp_dehaze_phase_y_read(HI_VOID) {
	return ((IORD_32DIRECT(0x7024)&0x3fffff));
}

static __inline HI_VOID hi_isp_dehaze_max_mode_write(HI_U8 data){
	HI_U8 u8Current = IORD_8DIRECT(0x7028);
	IOWR_8DIRECT(0x7028, (u8Current & 0xfb) | (data & 0x1) << 2);
}

static __inline HI_U8 hi_isp_dehaze_max_mode_read(HI_VOID) {
	return ((IORD_8DIRECT(0x7028) & 0x1) >> 2);
}

static __inline HI_VOID hi_isp_dehaze_min_mode_write(HI_U8 data){
	HI_U8 u8Current = IORD_8DIRECT(0x7028);
	IOWR_8DIRECT(0x7028, (u8Current & 0xfc) | (data & 0x3));
}

static __inline HI_U8 hi_isp_dehaze_min_mode_read(HI_VOID) {
	return (IORD_8DIRECT(0x7028) & 0x3);
}

static __inline HI_VOID hi_isp_dehaze_neg_mode_write(HI_U8 data){
	IOWR_8DIRECT(0x702c, data);
}

static __inline HI_U8 hi_isp_dehaze_neg_mode_read(HI_VOID) {
	return (IORD_8DIRECT(0x702c));
}

static __inline HI_VOID hi_isp_dehaze_air_r_write(HI_U8 data){
	HI_U32 u32Current = IORD_32DIRECT(0x7030);
	IOWR_32DIRECT(0x7030, (u32Current & 0xff00ffff) | ((data & 0xff) << 16));
}

static __inline HI_U8 hi_isp_dehaze_air_r_read(HI_VOID) {
	return ((IORD_32DIRECT(0x7030)&0x00ff0000) >> 16);
}

static __inline HI_VOID hi_isp_dehaze_air_g_write(HI_U8 data){
	HI_U32 u32Current = IORD_32DIRECT(0x7030);
	IOWR_32DIRECT(0x7030, (u32Current & 0xffff00ff) | ((data & 0xff) << 8));
}

static __inline HI_U8 hi_isp_dehaze_air_g_read(HI_VOID) {
	return ((IORD_32DIRECT(0x7030)&0x0000ff00) >> 8);
}

static __inline HI_VOID hi_isp_dehaze_air_b_write(HI_U8 data){
	HI_U32 u32Current = IORD_32DIRECT(0x7030);
	IOWR_32DIRECT(0x7030, (u32Current & 0xffffff00) | (data & 0xff));
}

static __inline HI_U8 hi_isp_dehaze_air_b_read(HI_VOID) {
	return (IORD_32DIRECT(0x7030)&0x000000ff);
}

static __inline HI_VOID hi_isp_dehaze_thld_write(HI_U8 data){
	IOWR_8DIRECT(0x7034, data);
}

static __inline HI_U8 hi_isp_dehaze_thld_read(HI_VOID) {
	return (IORD_8DIRECT(0x7034));
}

static __inline HI_VOID hi_isp_dehaze_gstrth_write(HI_U8 data){
	IOWR_8DIRECT(0x7038, data);
}

static __inline HI_U8 hi_isp_dehaze_gstrth_read(HI_VOID) {
	return (IORD_8DIRECT(0x7038));
}

static __inline HI_VOID hi_isp_dehaze_blthld_write(HI_U8 data){
	IOWR_8DIRECT(0x703c, data);
}

static __inline HI_U8 hi_isp_dehaze_blthld_read(HI_VOID) {
	return (IORD_8DIRECT(0x703c));
}

static __inline HI_VOID hi_isp_dehaze_minstat_waddr_write(HI_U32 data){
	IOWR_32DIRECT(0x7080, data);
}

static __inline HI_U32 hi_isp_dehaze_minstat_waddr_read(HI_VOID) {
	return (IORD_32DIRECT(0x7080));
}

static __inline HI_VOID hi_isp_dehaze_minstat_wdata_write(HI_U32 data){
	IOWR_32DIRECT(0x7084, data);
}

static __inline HI_U32 hi_isp_dehaze_minstat_wdata_read(HI_VOID) {
	return (IORD_32DIRECT(0x7084));
}

static __inline HI_VOID hi_isp_dehaze_minstat_raddr_write(HI_U32 data){
	IOWR_32DIRECT(0x7088, data);
}

static __inline HI_U32 hi_isp_dehaze_minstat_raddr_read(HI_VOID) {
	return (IORD_32DIRECT(0x7088));
}

static __inline HI_VOID hi_isp_dehaze_minstat_rdata_write(HI_U32 data){
	IOWR_32DIRECT(0x708c, data);
}

static __inline HI_U32 hi_isp_dehaze_minstat_rdata_read(HI_VOID) {
	return (IORD_32DIRECT(0x708c));
}

static __inline HI_VOID hi_isp_dehaze_maxstat_waddr_write(HI_U32 data){
	IOWR_32DIRECT(0x7090, data);
}

static __inline HI_U32 hi_isp_dehaze_maxstat_waddr_read(HI_VOID) {
	return (IORD_32DIRECT(0x7090));
}

static __inline HI_VOID hi_isp_dehaze_maxstat_wdata_write(HI_U32 data){
	IOWR_32DIRECT(0x7094, data);
}

static __inline HI_U32 hi_isp_dehaze_maxstat_wdata_read(HI_VOID) {
	return (IORD_32DIRECT(0x7094));
}

static __inline HI_VOID hi_isp_dehaze_maxstat_raddr_write(HI_U32 data){
	IOWR_32DIRECT(0x7098, data);
}

static __inline HI_U32 hi_isp_dehaze_maxstat_raddr_read(HI_VOID) {
	return (IORD_32DIRECT(0x7098));
}

static __inline HI_VOID hi_isp_dehaze_maxstat_rdata_write(HI_U32 data){
	IOWR_32DIRECT(0x709c, data);
}

static __inline HI_U32 hi_isp_dehaze_maxstat_rdata_read(HI_VOID) {
	return (IORD_32DIRECT(0x709c));
}

static __inline HI_VOID hi_isp_dehaze_prestat_waddr_write(HI_U32 data){
	IOWR_32DIRECT(0x70a0, data);
}

static __inline HI_U32 hi_isp_dehaze_prestat_waddr_read(HI_VOID) {
	return (IORD_32DIRECT(0x70a0));
}

static __inline HI_VOID hi_isp_dehaze_prestat_wdata_write(HI_U32 data){
	IOWR_32DIRECT(0x70a4, data);
}

static __inline HI_U32 hi_isp_dehaze_prestat_wdata_read(HI_VOID) {
	return (IORD_32DIRECT(0x70a4));
}

static __inline HI_VOID hi_isp_dehaze_prestat_raddr_write(HI_U32 data){
	IOWR_32DIRECT(0x70a8, data);
}

static __inline HI_U32 hi_isp_dehaze_prestat_raddr_read(HI_VOID) {
	return (IORD_32DIRECT(0x70a8));
}

static __inline HI_VOID hi_isp_dehaze_prestat_rdata_write(HI_U32 data){
	IOWR_32DIRECT(0x70ac, data);
}

static __inline HI_U32 hi_isp_dehaze_prestat_rdata_read(HI_VOID) {
	return (IORD_32DIRECT(0x70ac));
}

static __inline HI_VOID hi_isp_dehaze_lut_waddr_write(HI_U32 data){
	IOWR_32DIRECT(0x70b0, data);
}

static __inline HI_U32 hi_isp_dehaze_lut_waddr_read(HI_VOID) {
	return (IORD_32DIRECT(0x70b0));
}

static __inline HI_VOID hi_isp_dehaze_lut_wdata_write(HI_U32 data){
	IOWR_32DIRECT(0x70b4, data);
}

static __inline HI_U32 hi_isp_dehaze_lut_wdata_read(HI_VOID) {
	return (IORD_32DIRECT(0x70b4));
}

static __inline HI_VOID hi_isp_dehaze_lut_raddr_write(HI_U32 data){
	IOWR_32DIRECT(0x70b8, data);
}

static __inline HI_U32 hi_isp_dehaze_lut_raddr_read(HI_VOID) {
	return (IORD_32DIRECT(0x70b8));
}

static __inline HI_VOID hi_isp_dehaze_lut_rdata_write(HI_U32 data){
	IOWR_32DIRECT(0x70bc, data);
}

static __inline HI_U32 hi_isp_dehaze_lut_rdata_read(HI_VOID) {
	return (IORD_32DIRECT(0x70bc));
}

static __inline HI_VOID hi_isp_dehaze_update_mode_write(HI_U8 data){
	IOWR_8DIRECT(0x70e4, data);
}

static __inline HI_U8 hi_isp_dehaze_update_mode_read(HI_VOID) {
	return (IORD_8DIRECT(0x70e4));
}

static __inline HI_VOID hi_isp_dehaze_newer_write(HI_U8 data){
	IOWR_8DIRECT(0x70ec, data);
}

static __inline HI_U8 hi_isp_dehaze_newer_read(HI_VOID) {
	return (IORD_8DIRECT(0x70ec));
}

static __inline HI_VOID hi_isp_dehaze_height_write(HI_U16 data){
	HI_U32 u32Current = IORD_32DIRECT(0x70f0);
	IOWR_32DIRECT(0x70f0, (u32Current & 0xe000ffff) | ((data & 0x1fff) << 16));
}

static __inline HI_U16 hi_isp_dehaze_height_read(HI_VOID) {
	return ((IORD_32DIRECT(0x70f0)&0x1fff0000) >> 16);
}

static __inline HI_VOID hi_isp_dehaze_width_write(HI_U16 data){
	IOWR_16DIRECT(0x70f0, data&0x1fff);
}

static __inline HI_U16 hi_isp_dehaze_width_read(HI_VOID) {
	return (IORD_16DIRECT(0x70f0)&0x1fff);
}

/*****************************************************
MODULE        -----AF-----
MODULE        -----AF-----
MODULE        -----AF-----
*****************************************************/
/*****************************************************
ISP_AF_CFG:      AF
0x205a2200:      31		   	ck_gt_en		|	9			data_mode
	                 30:16   	reserved			|	8			squ_mode
	                 15			bayer_mode	|	7			peak_mode
	                 14			raw_mode	|	6			iir1_en2
	                 13			gamma_en	|	5			iir1_en1
	                 12			median_en	|	4			iir1_en0
	                 11			lpf_en		|	3			iir0_en2
	                 10			crop_en		|	2			iir0_en1
                                					|	1			iir0_en0
                                					|	0			af_en                                                          
*****************************************************/
//------------------------------------------------------------------------------ 
//  History        :
//  1.Date         : 2015/12/8
//    Author       : c00298856
//    Modification : add reg [ISP_MODULE_POS 0x205a0088] to select AF position 
//------------------------------------------------------------------------------

static __inline HI_VOID hi_vi_af_pos_write(HI_U8 data) {
	HI_U32 u32current = IORD_32DIRECT_VI(0x20088);
    IOWR_32DIRECT_VI(0x20088, (u32current & (~(0x1 << 3))) | ((data&0x1) << 3));
}

static __inline HI_U8 hi_vi_af_pos_read(HI_VOID) {
	return ((IORD_32DIRECT_VI(0x20088) & (0x1 << 3)) >> 3);
}

static __inline HI_VOID hi_isp_af_enable_write(HI_U8 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2200);
    IOWR_32DIRECT(0x2200, (u32current & (~(1))) | (data&0x1));
}

static __inline HI_U8 hi_isp_af_enable_read(HI_VOID) {
	return (IORD_32DIRECT(0x2200) & 0x1);
}

static __inline HI_VOID hi_isp_af_iir0_en0_write(HI_U8 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2200);
    IOWR_32DIRECT(0x2200, (u32current & (~(1 << 1))) | ((data&0x1) << 1));
}

static __inline HI_U8 hi_isp_af_iir0_en0_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2200) & (0x1 << 1)) >> 1);
}

static __inline HI_VOID hi_isp_af_iir0_en1_write(HI_U8 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2200);
    IOWR_32DIRECT(0x2200, (u32current & (~(1 << 2))) | ((data&0x1) << 2));
}

static __inline HI_U8 hi_isp_af_iir0_en1_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2200) & (0x1 << 2)) >> 2);
}

static __inline HI_VOID hi_isp_af_iir0_en2_write(HI_U8 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2200);
    IOWR_32DIRECT(0x2200, (u32current & (~(1 << 3))) | ((data&0x1) << 3));
}

static __inline HI_U8 hi_isp_af_iir0_en2_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2200) & (0x1 << 3)) >> 3);
}

static __inline HI_VOID hi_isp_af_iir1_en0_write(HI_U8 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2200);
    IOWR_32DIRECT(0x2200, (u32current & (~(1 << 4))) | ((data&0x1) << 4));
}

static __inline HI_U8 hi_isp_af_iir1_en0_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2200) & (0x1 << 4)) >> 4);
}

static __inline HI_VOID hi_isp_af_iir1_en1_write(HI_U8 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2200);
    IOWR_32DIRECT(0x2200, (u32current & (~(1 << 5))) | ((data&0x1) << 5));
}

static __inline HI_U8 hi_isp_af_iir1_en1_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2200) & (0x1 << 5)) >> 5);
}

static __inline HI_VOID hi_isp_af_iir1_en2_write(HI_U8 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2200);
    IOWR_32DIRECT(0x2200, (u32current & (~(1 << 6))) | ((data&0x1) << 6));
}

static __inline HI_U8 hi_isp_af_iir1_en2_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2200) & (0x1 << 6)) >> 6);
}

static __inline HI_VOID hi_isp_af_peak_mode_write(HI_U8 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2200);
    IOWR_32DIRECT(0x2200, (u32current & (~(1 << 7))) | ((data&0x1) << 7));
}

static __inline HI_U8 hi_isp_af_peak_mode_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2200) & (0x1 << 7)) >> 7);
}

static __inline HI_VOID hi_isp_af_squ_mode_write(HI_U8 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2200);
    IOWR_32DIRECT(0x2200, (u32current & (~(1 << 8))) | ((data&0x1) << 8));
}

static __inline HI_U8 hi_isp_af_squ_mode_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2200) & (0x1 << 8)) >> 8);
}

static __inline HI_VOID hi_isp_af_data_mode_write(HI_U8 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2200);
    IOWR_32DIRECT(0x2200, (u32current & (~(1 << 9))) | ((data&0x1) << 9));
}

static __inline HI_U8 hi_isp_af_data_mode_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2200) & (0x1 << 9)) >> 9);
}

static __inline HI_VOID hi_isp_af_crop_en_write(HI_U8 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2200);
    IOWR_32DIRECT(0x2200, (u32current & (~(1 << 10))) | ((data&0x1) << 10));
}

static __inline HI_U8 hi_isp_af_crop_en_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2200) & (0x1 << 10)) >> 10);
}

static __inline HI_VOID hi_isp_af_lpf_en_write(HI_U8 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2200);
    IOWR_32DIRECT(0x2200, (u32current & (~(1 << 11))) | ((data&0x1) << 11));
}

static __inline HI_U8 hi_isp_af_lpf_en_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2200) & (0x1 << 11)) >> 11);
}

static __inline HI_VOID hi_isp_af_median_en_write(HI_U8 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2200);
    IOWR_32DIRECT(0x2200, (u32current & (~(1 << 12))) | ((data&0x1) << 12));
}

static __inline HI_U8 hi_isp_af_median_en_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2200) & (0x1 << 12)) >> 12);
}

static __inline HI_VOID hi_isp_af_gamma_en_write(HI_U8 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2200);
    IOWR_32DIRECT(0x2200, (u32current & (~(1 << 13))) | ((data&0x1) << 13));
}

static __inline HI_U8 hi_isp_af_gamma_en_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2200) & (0x1 << 13)) >> 13);
}

static __inline HI_VOID hi_isp_af_raw_mode_write(HI_U8 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2200);
    IOWR_32DIRECT(0x2200, (u32current & (~(1 << 14))) | ((data&0x1) << 14));
}

static __inline HI_U8 hi_isp_af_raw_mode_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2200) & (0x1 << 14)) >> 14);
}

static __inline HI_VOID hi_isp_af_bayer_mode_write(HI_U8 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2200);
    IOWR_32DIRECT(0x2200, (u32current & (~(1 << 15))) | ((data&0x1) << 15));
}

static __inline HI_U8 hi_isp_af_bayer_mode_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2200) & (0x1 << 15)) >> 15);
}

static __inline HI_VOID hi_isp_af_ck_gt_en_write(HI_U8 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2200);
    IOWR_32DIRECT(0x2200, (u32current & (~(1 << 31))) | ((data&0x1) << 31));
}

static __inline HI_U8 hi_isp_af_ck_gt_en_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2200) & (0x1 << 31)) >> 31);
}

static __inline HI_VOID hi_isp_af_vnum_write(HI_U8 data){
	HI_U16 u16Current = IORD_16DIRECT(0x2210);
	IOWR_16DIRECT(0x2210, (u16Current & 0xe0ff) | ((data & 0x1f) << 8));
}

static __inline HI_U8 hi_isp_af_vnum_read(HI_VOID) {
	return ((IORD_16DIRECT(0x2210)&0x1f00) >> 8);
}

static __inline HI_VOID hi_isp_af_hnum_write(HI_U8 data){
	HI_U16 u16Current = IORD_16DIRECT(0x2210);
	IOWR_16DIRECT(0x2210, (u16Current & 0xffe0) | (data & 0x1f));
}

static __inline HI_U8 hi_isp_af_hnum_read(HI_VOID) {
	return (IORD_16DIRECT(0x2210)&0x1f);
}

static __inline HI_VOID hi_isp_af_pos_y_write(HI_U16 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2214);
	IOWR_32DIRECT(0x2214, (u32Current & 0xf000ffff) | ((data & 0xfff) << 16));
}

static __inline HI_U16 hi_isp_af_pos_y_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2214)&0xfff0000) >> 16);
}

static __inline HI_VOID hi_isp_af_pos_x_write(HI_U16 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2214);
	IOWR_32DIRECT(0x2214, (u32Current & 0xfffff000) | (data & 0xfff));
}

static __inline HI_U16 hi_isp_af_pos_x_read(HI_VOID) {
	return (IORD_32DIRECT(0x2214)&0x0fff);
}

static __inline HI_VOID hi_isp_af_crop_vsize_write(HI_U16 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2218);
	IOWR_32DIRECT(0x2218, (u32Current & 0xf000ffff) | ((data & 0xfff) << 16));
}

static __inline HI_U16 hi_isp_af_crop_vsize_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2218)&0xfff0000) >> 16);
}

static __inline HI_VOID hi_isp_af_crop_hsize_write(HI_U16 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2218);
	IOWR_32DIRECT(0x2218, (u32Current & 0xfffff000) | (data & 0xfff));
}

static __inline HI_U16 hi_isp_af_crop_hsize_read(HI_VOID) {
	return (IORD_32DIRECT(0x2218)&0x0fff);
}	

static __inline HI_VOID hi_isp_af_median_thres_write(HI_U16 data){
	HI_U16 u16Current = IORD_16DIRECT(0x221c);
	IOWR_16DIRECT(0x221c, (u16Current & 0x0) | (data & 0xffff));
}

static __inline HI_U16 hi_isp_af_median_thres_read(HI_VOID) {
	return (IORD_16DIRECT(0x221c)&0xffff);
}	

static __inline HI_VOID hi_isp_af_iir1_g0_write(HI_U8 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2220);
	IOWR_32DIRECT(0x2220, (u32Current & 0xff00ffff) | ((data & 0xff) << 16));
}

static __inline HI_U8 hi_isp_af_iir1_g0_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2220) & 0xff0000) >> 16);
}

static __inline HI_VOID hi_isp_af_iir0_g0_write(HI_U8 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2220);
	IOWR_32DIRECT(0x2220, (u32Current & 0xffffff00) | (data & 0xff));
}

static __inline HI_U8 hi_isp_af_iir0_g0_read(HI_VOID) {
	return (IORD_32DIRECT(0x2220) & 0xff);
}

static __inline HI_VOID hi_isp_af_iir1_g_write(HI_U8 index, HI_U16 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2220 + (index << 2));
	IOWR_32DIRECT(0x2220 + (index << 2), (u32Current & 0xfc00ffff) | ((data & 0x3ff) << 16));
}

static __inline HI_U16 hi_isp_af_iir1_g_read(HI_U8 index) {
	return ((IORD_32DIRECT(0x2220 + (index << 2)) & 0x3ff0000) >> 16);
}

static __inline HI_VOID hi_isp_af_iir0_g_write(HI_U8 index, HI_U16 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2220 + (index << 2));
	IOWR_32DIRECT(0x2220 + (index << 2), (u32Current & 0xfffffc00) | (data & 0x3ff));
}

static __inline HI_U16 hi_isp_af_iir0_g_read(HI_U8 index) {
	return (IORD_32DIRECT(0x2220 + (index << 2)) & 0x3ff);
}

static __inline HI_VOID hi_isp_af_iir1_shift_write(HI_U8 index, HI_U8 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2240);
	IOWR_32DIRECT(0x2240, (u32Current & (~(0xf << (16 + (index<<2))))) | ((data & 0xf) << (16 + (index<<2))));
}

static __inline HI_U8 hi_isp_af_iir1_shift_read(HI_U8 index) {
	return ((IORD_32DIRECT(0x2240) & (0xf << (16 + (index<<2)))) >> (16 + (index<<2)));
}

static __inline HI_VOID hi_isp_af_iir0_shift_write(HI_U8 index, HI_U8 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2240);
	IOWR_32DIRECT(0x2240, (u32Current & (~(0xf << (index<<2)))) | ((data & 0xf) << (index<<2)));
}

static __inline HI_U8 hi_isp_af_iir0_shift_read(HI_U8 index) {
	return ((IORD_32DIRECT(0x2240) & (0xf << (index<<2))) >> (index<<2));
}

static __inline HI_VOID hi_isp_af_fir1_h_write(HI_U8 index, HI_U8 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2250 + (index << 2));
	IOWR_32DIRECT(0x2250 + (index << 2), (u32Current & 0xffc0ffff) | ((data & 0x3f) << 16));
}

static __inline HI_U8 hi_isp_af_fir1_h_read(HI_U8 index) {
	return ((IORD_32DIRECT(0x2250 + (index << 2)) & 0x3f0000) >> 16);
}

static __inline HI_VOID hi_isp_af_fir0_h_write(HI_U8 index, HI_U8 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2250 + (index << 2));
	IOWR_32DIRECT(0x2250 + (index << 2), (u32Current & 0xffffffc0) | (data & 0x3f));
}

static __inline HI_U8 hi_isp_af_fir0_h_read(HI_U8 index) {
	return (IORD_32DIRECT(0x2250 + (index << 2)) & 0x3f);
}

static __inline HI_VOID hi_isp_af_iir1_thre_x_write(HI_U16 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2270);
	IOWR_32DIRECT(0x2270, (u32Current & 0xf800ffff) | ((data & 0x7ff) << 16));
}

static __inline HI_U16 hi_isp_af_iir1_thre_x_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2270)&0x7ff0000) >> 16);
}

static __inline HI_VOID hi_isp_af_iir0_thre_x_write(HI_U16 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2270);
	IOWR_32DIRECT(0x2270, (u32Current & 0xfffff800) | (data & 0x7ff));
}

static __inline HI_U16 hi_isp_af_iir0_thre_x_read(HI_VOID) {
	return (IORD_32DIRECT(0x2270)&0x7ff);
}

static __inline HI_VOID hi_isp_af_fir1_thre_y_write(HI_U16 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2274);
	IOWR_32DIRECT(0x2274, (u32Current & 0xf800ffff) | ((data & 0x7ff) << 16));
}

static __inline HI_U16 hi_isp_af_fir1_thre_y_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2274)&0x7ff0000) >> 16);
}

static __inline HI_VOID hi_isp_af_fir0_thre_y_write(HI_U16 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2274);
	IOWR_32DIRECT(0x2274, (u32Current & 0xfffff800) | (data & 0x7ff));
}

static __inline HI_U16 hi_isp_af_fir0_thre_y_read(HI_VOID) {
	return (IORD_32DIRECT(0x2274)&0x7ff);
}

static __inline HI_VOID hi_isp_af_acc_shift_y_write(HI_U8 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2278);
	IOWR_32DIRECT(0x2278, (u32Current & 0xfff0ffff) | ((data & 0xf) << 16));
}

static __inline HI_U8 hi_isp_af_acc_shift_y_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2278)&0xf0000) >> 16);
}

static __inline HI_VOID hi_isp_af_acc_shift_v1_write(HI_U8 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2278);
	IOWR_32DIRECT(0x2278, (u32Current & 0xffff0fff) | ((data & 0xf) << 12));
}

static __inline HI_U8 hi_isp_af_acc_shift_v1_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2278)&0xf000) >> 12);
}

static __inline HI_VOID hi_isp_af_acc_shift_v0_write(HI_U8 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2278);
	IOWR_32DIRECT(0x2278, (u32Current & 0xfffff0ff) | ((data & 0xf) << 8));
}

static __inline HI_U8 hi_isp_af_acc_shift_v0_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2278)&0xf00) >> 8);
}

static __inline HI_VOID hi_isp_af_acc_shift_h1_write(HI_U8 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2278);
	IOWR_32DIRECT(0x2278, (u32Current & 0xffffff0f) | ((data & 0xf) << 4));
}

static __inline HI_U8 hi_isp_af_acc_shift_h1_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2278)&0xf0) >> 4);
}

static __inline HI_VOID hi_isp_af_acc_shift_h0_write(HI_U8 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2278);
	IOWR_32DIRECT(0x2278, (u32Current & 0xfffffff0) | (data & 0xf));
}

static __inline HI_U8 hi_isp_af_acc_shift_h0_read(HI_VOID) {
	return (IORD_32DIRECT(0x2278)&0xf);
}

static __inline HI_VOID hi_isp_af_cnt_shift_v1_write(HI_U8 data){
	HI_U32 u32Current = IORD_32DIRECT(0x227c);
	IOWR_32DIRECT(0x227c, (u32Current & 0xffff0fff) | ((data & 0xf) << 12));
}

static __inline HI_U8 hi_isp_af_cnt_shift_v1_read(HI_VOID) {
	return ((IORD_32DIRECT(0x227c)&0xf000) >> 12);
}

static __inline HI_VOID hi_isp_af_cnt_shift_v0_write(HI_U8 data){
	HI_U32 u32Current = IORD_32DIRECT(0x227c);
	IOWR_32DIRECT(0x227c, (u32Current & 0xfffff0ff) | ((data & 0xf) << 8));
}

static __inline HI_U8 hi_isp_af_cnt_shift_v0_read(HI_VOID) {
	return ((IORD_32DIRECT(0x227c)&0xf00) >> 8);
}

static __inline HI_VOID hi_isp_af_cnt_shift_h1_write(HI_U8 data){
	HI_U32 u32Current = IORD_32DIRECT(0x227c);
	IOWR_32DIRECT(0x227c, (u32Current & 0xffffff0f) | ((data & 0xf) << 4));
}

static __inline HI_U8 hi_isp_af_cnt_shift_h1_read(HI_VOID) {
	return ((IORD_32DIRECT(0x227c)&0xf0) >> 4);
}

static __inline HI_VOID hi_isp_af_cnt_shift_h0_write(HI_U8 data){
	HI_U32 u32Current = IORD_32DIRECT(0x227c);
	IOWR_32DIRECT(0x227c, (u32Current & 0xfffffff0) | (data & 0xf));
}

static __inline HI_U8 hi_isp_af_cnt_shift_h0_read(HI_VOID) {
	return (IORD_32DIRECT(0x227c)&0xf0);
}

static __inline HI_VOID hi_isp_af_stat_ind_raddr_write(HI_U32 data){
	IOWR_32DIRECT(0x2288, data);
}

static __inline HI_U32 hi_isp_af_stat_ind_raddr_read(HI_VOID) {
	return (IORD_32DIRECT(0x2288));
}

static __inline HI_U32 hi_isp_af_stat_ind_rdata_read(HI_VOID) {
	return (IORD_32DIRECT(0x228c));
}

static __inline HI_VOID hi_isp_af_vsize_write(HI_U16 data){
	HI_U32 u32Current = IORD_32DIRECT(0x22f0);
	IOWR_32DIRECT(0x22f0, (u32Current & 0xf000ffff) | ((data & 0xfff) << 16));
}

static __inline HI_U16 hi_isp_af_vsize_read(HI_VOID) {
	return ((IORD_32DIRECT(0x22f0)&0xfff0000) >> 16);
}

static __inline HI_VOID hi_isp_af_hsize_write(HI_U16 data){
	IOWR_16DIRECT(0x22f0, data&0xfff);
}

static __inline HI_U16 hi_isp_af_hsize_read(HI_VOID) {
	return (IORD_16DIRECT(0x22f0)&0xfff);
}

/*****************************************************
MODULE        -----ISP_CROP_CFG-----
MODULE        -----ISP_CROP_CFG-----
MODULE        -----ISP_CROP_CFG-----
*****************************************************/

static __inline HI_VOID hi_isp_crop_en_write(HI_U8 data){
	IOWR_32DIRECT_ISP_REG(0x11100, data & 0x1);
}

static __inline HI_U8 hi_isp_crop_en_read(HI_VOID) {
	return (IORD_32DIRECT_ISP_REG(0x11100)&0x1);
}

static __inline HI_VOID hi_isp_crop_x_start_write(HI_U16 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x11110);
	IOWR_32DIRECT_ISP_REG(0x11110, (data & 0x1fff) | (u32current & 0xffffe000));
}

static __inline HI_U16 hi_isp_crop_x_start_read(HI_VOID) {
	return (IORD_32DIRECT_ISP_REG(0x11110)&0x1fff);
}

static __inline HI_VOID hi_isp_crop_y_start_write(HI_U16 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x11110);
	IOWR_32DIRECT_ISP_REG(0x11110, ((data & 0x1fff) << 16) | (u32current & 0x1fff));
}

static __inline HI_U16 hi_isp_crop_y_start_read(HI_VOID) {
	return ((IORD_32DIRECT_ISP_REG(0x11110)&0x1fff0000) >> 16);
}


static __inline HI_VOID hi_isp_crop_width_write(HI_U16 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x11114);
	IOWR_32DIRECT_ISP_REG(0x11114, (data & 0x1fff) | (u32current & 0xffffe000));
}

static __inline HI_U16 hi_isp_crop_width_read(HI_VOID) {
	return (IORD_32DIRECT_ISP_REG(0x11114)&0x1fff);
}

static __inline HI_VOID hi_isp_crop_height_write(HI_U16 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x11114);
	IOWR_32DIRECT_ISP_REG(0x11114, ((data & 0x1fff) << 16) | (u32current & 0x1fff));
}

static __inline HI_U16 hi_isp_crop_height_read(HI_VOID) {
	return ((IORD_32DIRECT_ISP_REG(0x11114)&0x1fff0000) >> 16);
}

/*****************************************************
MODULE        -----WCH_Y_SIZE-----
MODULE        -----WCH_Y_SIZE-----
MODULE        -----WCH_Y_SIZE-----
*****************************************************/
static __inline HI_VOID hi_isp_wch_y_width_write(HI_U16 data){
	HI_U32 u32current = IO_READ32_VI(0x1084);
	IO_WRITE32_VI(0x1084, (data & 0x1fff) | (u32current & 0xffffe000));
}

static __inline HI_U16 hi_isp_wch_y_width_read(HI_VOID) {
	return (IO_READ32_VI(0x1084)&0x1fff);
}

static __inline HI_VOID hi_isp_wch_y_height_write(HI_U16 data){
	HI_U32 u32current = IO_READ32_VI(0x1084);
	IO_WRITE32_VI(0x1084, ((data & 0x1fff) << 16) | (u32current & 0x1fff));
}

static __inline HI_U16 hi_isp_wch_y_height_read(HI_VOID) {
	return ((IO_READ32_VI(0x1084)&0x1fff0000) >> 16);
}

static __inline HI_VOID hi_isp_wch_c_width_write(HI_U16 data){
	HI_U32 u32current = IO_READ32_VI(0x10a4);
	IO_WRITE32_VI(0x10a4, (data & 0x1fff) | (u32current & 0xffffe000));
}

static __inline HI_U16 hi_isp_wch_c_width_read(HI_VOID) {
	return (IO_READ32_VI(0x10a4)&0x1fff);
}

static __inline HI_VOID hi_isp_wch_c_height_write(HI_U16 data){
	HI_U32 u32current = IO_READ32_VI(0x10a4);
	IO_WRITE32_VI(0x10a4, ((data & 0x1fff) << 16) | (u32current & 0x1fff));
}

static __inline HI_U16 hi_isp_wch_c_height_read(HI_VOID) {
	return ((IO_READ32_VI(0x10a4)&0x1fff0000) >> 16);
}

/*****************************************************
ISP_RGBIR_EN: 			RGBIR module enable
0x205a3100: 				31:1 	reserved		
						0		enable
*****************************************************/
static __inline HI_VOID hi_isp_rgbir_en_write(HI_U8 data){
    IOWR_8DIRECT(0x3100, (data & 0x1));
}

static __inline HI_U32 hi_isp_rgbir_en_read(HI_VOID) {
    return (IORD_32DIRECT(0x3100) & 0x1);
}

/*****************************************************
ISP_RGBIR_EN: 			RGBIR control enable
0x205a3104: 				31:20 	reserved	
                           19       ir_out_en
                           18       ir_filter_en
                           17       blc_off_en
                           16       removel ir enable
                           15:2     reserved
                           1        auto gain enable
						0		manul gain enable
*****************************************************/
static __inline HI_VOID hi_isp_rgbir_ir_out_en_write(HI_U8 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3104);
    IOWR_32DIRECT(0x3104, (u32Current & 0x7ffff) | ((data & 0x1) << 19));
}

static __inline HI_U32 hi_isp_rgbir_ir_out_en_read(HI_VOID) {
    return ((IORD_32DIRECT(0x3104) & 0x80000) >> 19);
}

static __inline HI_VOID hi_isp_rgbir_ir_filter_en_write(HI_U8 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3104);
    IOWR_32DIRECT(0x3104, (u32Current & 0xbffff) | ((data & 0x1) << 18));
}

static __inline HI_U32 hi_isp_rgbir_ir_filter_en_read(HI_VOID) {
    return ((IORD_32DIRECT(0x3104) & 0x40000) >> 18);
}

static __inline HI_VOID hi_isp_rgbir_blc_off_en_write(HI_U8 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3104);
    IOWR_32DIRECT(0x3104, (u32Current & 0xdffff) | ((data & 0x1) << 17));
}

static __inline HI_U32 hi_isp_rgbir_blc_off_en_read(HI_VOID) {
    return ((IORD_32DIRECT(0x3104) & 0x20000) >> 17);
}

static __inline HI_VOID hi_isp_rgbir_removel_en_write(HI_U8 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3104);
    IOWR_32DIRECT(0x3104, (u32Current & 0xeffff) | ((data & 0x1) << 16));
}

static __inline HI_U32 hi_isp_rgbir_removel_en_read(HI_VOID) {
    return ((IORD_32DIRECT(0x3104) & 0x10000) >> 16);
}

static __inline HI_VOID hi_isp_rgbir_auto_gain_en_write(HI_U8 data){
    HI_U32 u32Current = IORD_8DIRECT(0x3104);
    IOWR_8DIRECT(0x3104, (u32Current & 0xffffd) | ((data & 0x1) << 1));
}

static __inline HI_U32 hi_isp_rgbir_auto_gain_en_read(HI_VOID) {
    return ((IORD_8DIRECT(0x3104) & 0x2) >> 1);
}

static __inline HI_VOID hi_isp_rgbir_manul_gain_en_write(HI_U8 data){
    HI_U32 u32Current = IORD_8DIRECT(0x3104);
    IOWR_8DIRECT(0x3104, (u32Current & 0xffffe) | (data & 0x1));
}

static __inline HI_U32 hi_isp_rgbir_manul_gain_en_read(HI_VOID) {
    return (IORD_8DIRECT(0x3104) & 0x1);
}

/*****************************************************
ISP_RGBIR_CTRL: 			RGBIR control 
0x205a3110: 				31:1 	reserved	
						0		IR position
*****************************************************/
static __inline HI_VOID hi_isp_rgbir_ir_position_write(HI_U8 data){
    IOWR_8DIRECT(0x3110, (data & 0x1));
}
    
static __inline HI_U32 hi_isp_rgbir_ir_position_read(HI_VOID) {
    return (IORD_8DIRECT(0x3110) & 0x1);
}

/*****************************************************
ISP_RGBIR_MANU_GAIN: 		RGBIR compensation manul gain 
0x205a3120: 				31:11 	reserved	
						9:0		manul gain
*****************************************************/
static __inline HI_VOID hi_isp_rgbir_manul_gain_write(HI_U16 data){
    IOWR_16DIRECT(0x3120, (data & 0x3ff));
}
    
static __inline HI_U32 hi_isp_rgbir_manul_gain_read(HI_VOID) {
    return (IORD_16DIRECT(0x3120) & 0x3ff);
}

/*****************************************************
ISP_RGBIR_G_MIN:          RGBIR green channel min
0x205a3130:               31:12 	reserved	
                          11:0		min_g
*****************************************************/
static __inline HI_VOID hi_isp_rgbir_g_min_write(HI_U16 data){
    IOWR_32DIRECT(0x3130, (data & 0xfff));
}
    
static __inline HI_U32 hi_isp_rgbir_g_min_read(HI_VOID) {
    return (IORD_32DIRECT(0x3130) & 0xfff);
}

/*****************************************************
ISP_RGBIR_RB_MAX:          RGBIR red and blue channel max
0x205a3134:               31:12 	reserved	
                          11:0		max_rb
*****************************************************/
static __inline HI_VOID hi_isp_rgbir_rb_max_write(HI_U16 data){
    IOWR_32DIRECT(0x3134, (data & 0xfff));
}
    
static __inline HI_U32 hi_isp_rgbir_rb_max_read(HI_VOID) {
    return (IORD_32DIRECT(0x3134) & 0xfff);
}

/*****************************************************
ISP_RGBIR_BLACK_LEVEL:    RGBIR black level
0x205a3138:               31:12 	reserved	
                          11:0		black_level
*****************************************************/
static __inline HI_VOID hi_isp_rgbir_black_level_write(HI_U16 data){
    IOWR_32DIRECT(0x3138, (data & 0xfff));
}
    
static __inline HI_U32 hi_isp_rgbir_black_level_read(HI_VOID) {
    return (IORD_32DIRECT(0x3138) & 0xfff);
}

/*****************************************************
ISP_RGBIR_F0:             RGBIR filter coefficient f0
0x205a3140:               31:12 	reserved	
                          11:8      coef02
                          7:4       coef01
                          3:0		coef00
*****************************************************/
static __inline HI_VOID hi_isp_rgbir_filter_coef02_write(HI_U8 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3140);
    IOWR_16DIRECT(0x3140, (u32Current & 0xff) | ((data & 0xf) << 8));
}
    
static __inline HI_U32 hi_isp_rgbir_filter_coef02_read(HI_VOID) {
    return ((IORD_16DIRECT(0x3140) & 0xf00) >> 8);
}

static __inline HI_VOID hi_isp_rgbir_filter_coef01_write(HI_U8 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3140);
    IOWR_16DIRECT(0x3140, (u32Current & 0xf0f) | ((data & 0xf) << 4));
}
    
static __inline HI_U32 hi_isp_rgbir_filter_coef01_read(HI_VOID) {
    return ((IORD_16DIRECT(0x3140) & 0xf0) >> 4);
}

static __inline HI_VOID hi_isp_rgbir_filter_coef00_write(HI_U8 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3140);
    IOWR_16DIRECT(0x3140, (u32Current & 0xff0) | (data & 0xf));
}
    
static __inline HI_U32 hi_isp_rgbir_filter_coef00_read(HI_VOID) {
    return (IORD_16DIRECT(0x3140) & 0xf);
}

/*****************************************************
ISP_RGBIR_F1:             RGBIR filter coefficient f1
0x205a3144:               31:12 	reserved	
                          11:8      coef12
                          7:4       coef11
                          3:0		coef10
*****************************************************/
static __inline HI_VOID hi_isp_rgbir_filter_coef12_write(HI_U8 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3144);
    IOWR_16DIRECT(0x3144, (u32Current & 0xff) | ((data & 0xf) << 8));
}
    
static __inline HI_U32 hi_isp_rgbir_filter_coef12_read(HI_VOID) {
    return ((IORD_16DIRECT(0x3144) & 0xf00) >> 8);
}

static __inline HI_VOID hi_isp_rgbir_filter_coef11_write(HI_U8 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3144);
    IOWR_16DIRECT(0x3144, (u32Current & 0xf0f) | ((data & 0xf) << 4));
}
    
static __inline HI_U32 hi_isp_rgbir_filter_coef11_read(HI_VOID) {
    return ((IORD_16DIRECT(0x3144) & 0xf0) >> 4);
}

static __inline HI_VOID hi_isp_rgbir_filter_coef10_write(HI_U8 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3144);
    IOWR_16DIRECT(0x3144, (u32Current & 0xff0) | (data & 0xf));
}
    
static __inline HI_U32 hi_isp_rgbir_filter_coef10_read(HI_VOID) {
    return (IORD_16DIRECT(0x3144) & 0xf);
}

/*****************************************************
ISP_RGBIR_F2:             RGBIR filter coefficient f2
0x205a3148:               31:12 	reserved	
                          11:8      coef22
                          7:4       coef21
                          3:0		coef20
*****************************************************/
static __inline HI_VOID hi_isp_rgbir_filter_coef22_write(HI_U8 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3148);
    IOWR_16DIRECT(0x3148, (u32Current & 0xff) | ((data & 0xf) << 8));
}
    
static __inline HI_U32 hi_isp_rgbir_filter_coef22_read(HI_VOID) {
    return ((IORD_16DIRECT(0x3148) & 0xf00) >> 8);
}

static __inline HI_VOID hi_isp_rgbir_filter_coef21_write(HI_U8 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3148);
    IOWR_16DIRECT(0x3148, (u32Current & 0xf0f) | ((data & 0xf) << 4));
}
    
static __inline HI_U32 hi_isp_rgbir_filter_coef21_read(HI_VOID) {
    return ((IORD_16DIRECT(0x3148) & 0xf0) >> 4);
}

static __inline HI_VOID hi_isp_rgbir_filter_coef20_write(HI_U8 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3148);
    IOWR_16DIRECT(0x3148, (u32Current & 0xff0) | (data & 0xf));
}
    
static __inline HI_U32 hi_isp_rgbir_filter_coef20_read(HI_VOID) {
    return (IORD_16DIRECT(0x3148) & 0xf);
}

/*****************************************************
ISP_RGBIR_THRE:           RGBIR threshold
0x205a3150:               31:24 	reserved	
                          23:16     thre2
                          15:8      reserved
                          7:0		thre1
*****************************************************/
static __inline HI_VOID hi_isp_rgbir_thre2_write(HI_U8 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3150);
    IOWR_32DIRECT(0x3150, (u32Current & 0xffff) | ((data & 0xff) << 16));
}
    
static __inline HI_U32 hi_isp_rgbir_thre2_read(HI_VOID) {
    return ((IORD_32DIRECT(0x3150) & 0xff0000) >> 16);
}

static __inline HI_VOID hi_isp_rgbir_thre1_write(HI_U8 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3150);
    IOWR_16DIRECT(0x3150, (u32Current & 0xffff00) | (data & 0xff));
}
    
static __inline HI_U32 hi_isp_rgbir_thre1_read(HI_VOID) {
    return (IORD_16DIRECT(0x3150) & 0xff);
}

/*****************************************************
ISP_RGBIR_EXP_VALU:       RGBIR exposure max value
0x205a3160:               31:12 	reserved	
                          11:0		exp_value
*****************************************************/
static __inline HI_VOID hi_isp_rgbir_exp_value_write(HI_U16 data){
    IOWR_16DIRECT(0x3160, (data & 0xfff));
}
    
static __inline HI_U32 hi_isp_rgbir_exp_value_read(HI_VOID) {
    return (IORD_16DIRECT(0x3160) & 0xfff);
}

/*****************************************************
ISP_RGBIR_GAIN_USE:       RGBIR compensation gain use(read only)
0x205a3164:               31:12 	reserved	
                          11:0		gain_use
*****************************************************/

static __inline HI_U32 hi_isp_rgbir_gain_use_read(HI_VOID) {
    return (IORD_16DIRECT(0x3164) & 0xfff);
}

/*****************************************************
ISP_RGBIR_EXP_CNT:       RGBIR over exposure count
0x205a3168:               31:20 	reserved	
                          19:0		exp_cnt
*****************************************************/

static __inline HI_U32 hi_isp_rgbir_exp_cnt_read(HI_VOID) {
    return (IORD_32DIRECT(0x3168) & 0xfffff);
}

/*****************************************************
ISP_RGBIR_SCALE_B:        RGBIR scale b coefficient
0x205a3170:               31:30 	reserved	
                          29:20     b_b
                          19:10     b_g
                          9:0		b_r
*****************************************************/
static __inline HI_VOID hi_isp_rgbir_scale_b_b_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3170);
    IOWR_32DIRECT(0x3170, (u32Current & 0xfffff) | ((data & 0x3ff) << 20));
}
    
static __inline HI_U32 hi_isp_rgbir_scale_b_b_read(HI_VOID) {
    HI_U32 u32Tmp;
    
    u32Tmp = ((IORD_32DIRECT(0x3170) & 0x3ff00000) >> 20);
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

static __inline HI_VOID hi_isp_rgbir_scale_b_g_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3170);
    IOWR_32DIRECT(0x3170, (u32Current & 0x3ff003ff) | ((data & 0x3ff) << 10));
}
    
static __inline HI_U32 hi_isp_rgbir_scale_b_g_read(HI_VOID) {
    HI_U32 u32Tmp;

    u32Tmp = (IORD_32DIRECT(0x3170) & 0xffc00) >> 10;
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

static __inline HI_VOID hi_isp_rgbir_scale_b_r_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3170);
    IOWR_32DIRECT(0x3170, (u32Current & 0x3ffffc00) | (data & 0x3ff));
}
    
static __inline HI_U32 hi_isp_rgbir_scale_b_r_read(HI_VOID) {
    HI_U32 u32Tmp;

    u32Tmp = (IORD_32DIRECT(0x3170) & 0x3ff);
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

/*****************************************************
ISP_RGBIR_SCALE_G:        RGBIR scale g coefficient
0x205a3174:               31:30 	reserved	
                          29:20     g_b
                          19:10     g_g
                          9:0		g_r
*****************************************************/
static __inline HI_VOID hi_isp_rgbir_scale_g_b_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3174);
    IOWR_32DIRECT(0x3174, (u32Current & 0xfffff) | ((data & 0x3ff) << 20));
}
    
static __inline HI_U32 hi_isp_rgbir_scale_g_b_read(HI_VOID) {
    HI_U32 u32Tmp;

    u32Tmp = ((IORD_32DIRECT(0x3174) & 0x3ff00000) >> 20);
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

static __inline HI_VOID hi_isp_rgbir_scale_g_g_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3174);
    IOWR_32DIRECT(0x3174, (u32Current & 0x3ff003ff) | ((data & 0x3ff) << 10));
}
    
static __inline HI_U32 hi_isp_rgbir_scale_g_g_read(HI_VOID) {
    HI_U32 u32Tmp;

    u32Tmp = ((IORD_32DIRECT(0x3174) & 0xffc00) >> 10);
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

static __inline HI_VOID hi_isp_rgbir_scale_g_r_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3174);
    IOWR_32DIRECT(0x3174, (u32Current & 0x3ffffc00) | (data & 0x3ff));
}
    
static __inline HI_U32 hi_isp_rgbir_scale_g_r_read(HI_VOID) {
    HI_U32 u32Tmp;

    u32Tmp = (IORD_32DIRECT(0x3174) & 0x3ff);
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

/*****************************************************
ISP_RGBIR_SCALE_R:        RGBIR scale r coefficient
0x205a3178:               31:30 	reserved	
                          29:20     r_b
                          19:10     r_g
                          9:0		r_r
*****************************************************/
static __inline HI_VOID hi_isp_rgbir_scale_r_b_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3178);
    IOWR_32DIRECT(0x3178, (u32Current & 0xfffff) | ((data & 0x3ff) << 20));
}
    
static __inline HI_U32 hi_isp_rgbir_scale_r_b_read(HI_VOID) {
    HI_U32 u32Tmp;

    u32Tmp = ((IORD_32DIRECT(0x3178) & 0x3ff00000) >> 20);
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

static __inline HI_VOID hi_isp_rgbir_scale_r_g_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3178);
    IOWR_32DIRECT(0x3178, (u32Current & 0x3ff003ff) | ((data & 0x3ff) << 10));
}
    
static __inline HI_U32 hi_isp_rgbir_scale_r_g_read(HI_VOID) {
    HI_U32 u32Tmp;

    u32Tmp = ((IORD_32DIRECT(0x3178) & 0xffc00) >> 10);
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

static __inline HI_VOID hi_isp_rgbir_scale_r_r_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3178);
    IOWR_32DIRECT(0x3178, (u32Current & 0x3ffffc00) | (data & 0x3ff));
}
    
static __inline HI_U32 hi_isp_rgbir_scale_r_r_read(HI_VOID) {
    HI_U32 u32Tmp;

    u32Tmp = (IORD_32DIRECT(0x3178) & 0x3ff);
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

/*****************************************************
ISP_RGBIR_SCALE_IR:        RGBIR scale ir coefficient
0x205a317c:               31:30 	reserved	
                          29:20     ir_b
                          19:10     ir_g
                          9:0		ir_r
*****************************************************/
static __inline HI_VOID hi_isp_rgbir_scale_ir_b_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x317c);
    IOWR_32DIRECT(0x317c, (u32Current & 0xfffff) | ((data & 0x3ff) << 20));
}
    
static __inline HI_U32 hi_isp_rgbir_scale_ir_b_read(HI_VOID) {
    HI_U32 u32Tmp;

    u32Tmp = ((IORD_32DIRECT(0x317c) & 0x3ff00000) >> 20);
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

static __inline HI_VOID hi_isp_rgbir_scale_ir_g_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x317c);
    IOWR_32DIRECT(0x317c, (u32Current & 0x3ff003ff) | ((data & 0x3ff) << 10));
}
    
static __inline HI_U32 hi_isp_rgbir_scale_ir_g_read(HI_VOID) {
    HI_U32 u32Tmp;

    u32Tmp = ((IORD_32DIRECT(0x317c) & 0xffc00) >> 10);
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

static __inline HI_VOID hi_isp_rgbir_scale_ir_r_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x317c);
    IOWR_32DIRECT(0x317c, (u32Current & 0x3ffffc00) | (data & 0x3ff));
}
    
static __inline HI_U32 hi_isp_rgbir_scale_ir_r_read(HI_VOID) {
    HI_U32 u32Tmp;
    
    u32Tmp = (IORD_32DIRECT(0x317c) & 0x3ff);
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

/*****************************************************
ISP_RGBIR_SCALE_C:        RGBIR scale c coefficient
0x205a3180:               31:30 	reserved	
                          29:20     b_c
                          19:10     g_c
                          9:0		r_c
*****************************************************/
static __inline HI_VOID hi_isp_rgbir_scale_c_b_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3180);
    IOWR_32DIRECT(0x3180, (u32Current & 0xfffff) | ((data & 0x3ff) << 20));
}
    
static __inline HI_U32 hi_isp_rgbir_scale_c_b_read(HI_VOID) {
    HI_U32 u32Tmp;

    u32Tmp = ((IORD_32DIRECT(0x3180) & 0x3ff00000) >> 20);
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

static __inline HI_VOID hi_isp_rgbir_scale_c_g_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3180);
    IOWR_32DIRECT(0x3180, (u32Current & 0x3ff003ff) | ((data & 0x3ff) << 10));
}
    
static __inline HI_U32 hi_isp_rgbir_scale_c_g_read(HI_VOID) {
    HI_U32 u32Tmp;

    u32Tmp = ((IORD_32DIRECT(0x3180) & 0xffc00) >> 10);
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

static __inline HI_VOID hi_isp_rgbir_scale_c_r_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3180);
    IOWR_32DIRECT(0x3180, (u32Current & 0x3ffffc00) | (data & 0x3ff));
}
    
static __inline HI_U32 hi_isp_rgbir_scale_c_r_read(HI_VOID) {
    HI_U32 u32Tmp;

    u32Tmp = (IORD_32DIRECT(0x3180) & 0x3ff);
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

static __inline HI_VOID hi_isp_rgbir_width_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT(0x31f0);
	IOWR_32DIRECT(0x31f0, (u32Current & 0xfffff000) | (data & 0xfff));
}

static __inline HI_U16 hi_isp_rgbir_width_read(HI_VOID) {
	return (IORD_32DIRECT(0x31f0) & 0xfff);
}

static __inline HI_VOID hi_isp_rgbir_height_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT(0x31f0);
	IOWR_32DIRECT(0x31f0, (u32Current & 0xf000ffff) | ((data & 0xfff) << 16));
}

static __inline HI_U16 hi_isp_rgbir_height_read(HI_VOID) {
	return ((IORD_32DIRECT(0x31f0) & 0xfff0000) >> 16);
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __ISP_CONFIG_CONFIG_EX_H__ */

