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
#include "isp_reg_define.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define ISP_DEV_OFFSET   (0x100000)

HI_S32 ISP_SetCalibrateAttr(ISP_DEV IspDev, ISP_FPN_CALIBRATE_ATTR_S *pstCalibrate);
HI_S32 ISP_SetCorrectionAttr(ISP_DEV ViDev, const ISP_FPN_ATTR_S *pstCorrection);
HI_S32 ISP_GetCorrectionAttr(ISP_DEV ViDev, ISP_FPN_ATTR_S *pstCorrection);
HI_S32 ISP_ACM_SetAttr(ISP_ACM_ATTR_S *pstAcmAttr);
HI_S32 ISP_ACM_GetAttr(ISP_ACM_ATTR_S *pstAcmAttr);
HI_S32 ISP_ACM_SetCoeff(ISP_ACM_LUT_S *pstAcmCoeff);
HI_S32 ISP_ACM_GetCoeff(ISP_ACM_LUT_S *pstAcmCoeff);
HI_S32 ISP_MeshShadingCalibration(HI_U16* pu16SrcRaw, ISP_LSC_CALIBRATION_CFG_S* pstLSCCaliCfg 
    , ISP_MESH_SHADING_TABLE_S* pstMeshShadingResult);

static __inline HI_U32 hi_isp_getaddr(HI_U8 ispDev)
{
	return (VReg_GetVirtAddr(ISP_REG_BASE) + ispDev*ISP_DEV_OFFSET);
}

/*****************************************************
MODULE        -----COMM-----
MODULE        -----COMM-----
MODULE        -----COMM-----
*****************************************************/
/*****************************************************
ISP_MODULE_POS:       

*****************************************************/
#define HI_ISP_MOUDLE_POS_NR_DEFAULT  (0x0)
#define HI_ISP_MOUDLE_POS_AE_AFTER_WB (0x1)
static __inline HI_VOID hi_isp_module_nr_sel_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_FE_MODULE_POS ISP_FE_MODULE_POS;	
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_FE_MODULE_POS.u32 = pstIspReg->ISP_FE_MODULE_POS.u32;
	ISP_FE_MODULE_POS.bits.nr_sel = data;
	pstIspReg->ISP_FE_MODULE_POS.u32 = ISP_FE_MODULE_POS.u32;
}

static __inline HI_VOID hi_isp_module_dcg_sel_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_FE_MODULE_POS ISP_FE_MODULE_POS;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_FE_MODULE_POS.u32 = pstIspReg->ISP_FE_MODULE_POS.u32;
	ISP_FE_MODULE_POS.bits.dcg_sel = data;
	pstIspReg->ISP_FE_MODULE_POS.u32 = ISP_FE_MODULE_POS.u32;
}

static __inline HI_VOID hi_isp_module_ae_sel_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_FE_MODULE_POS ISP_FE_MODULE_POS;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_FE_MODULE_POS.u32 = pstIspReg->ISP_FE_MODULE_POS.u32;
	ISP_FE_MODULE_POS.bits.ae_sel = data;
	pstIspReg->ISP_FE_MODULE_POS.u32 = ISP_FE_MODULE_POS.u32;
}

#define HI_ISP_TOP_AE_SELECT_AFTER_DG (0)
#define HI_ISP_TOP_AE_SELECT_AFTER_STATIC_WB (1)
#define HI_ISP_TOP_AE_SELECT_AFTER_DRC (2)

static __inline HI_U8 hi_isp_module_ae_sel_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_FE_MODULE_POS ISP_FE_MODULE_POS;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_FE_MODULE_POS.u32 = pstIspReg->ISP_FE_MODULE_POS.u32;
	return ISP_FE_MODULE_POS.bits.ae_sel ;
}

#define HI_ISP_MOUDLE_POS_AF_AFTER_DG   (0x0)
#define HI_ISP_MOUDLE_POS_AF_AFTER_DRC  (0x1)
static __inline HI_VOID hi_isp_module_af_sel_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_FE_MODULE_POS ISP_FE_MODULE_POS;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_FE_MODULE_POS.u32 = pstIspReg->ISP_FE_MODULE_POS.u32;
	ISP_FE_MODULE_POS.bits.af_sel = data;
	pstIspReg->ISP_FE_MODULE_POS.u32 = ISP_FE_MODULE_POS.u32;
}

static __inline HI_U8 hi_isp_module_af_sel_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_FE_MODULE_POS ISP_FE_MODULE_POS;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_FE_MODULE_POS.u32 = pstIspReg->ISP_FE_MODULE_POS.u32;
	return ISP_FE_MODULE_POS.bits.af_sel;
}

/*****************************************************
MODULE        -----DITHER-----
MODULE        -----DITHER-----
MODULE        -----DITHER-----
*****************************************************/


static __inline HI_VOID hi_isp_acm_dither_en_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_ACM_DITHER ISP_ACM_DITHER;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_ACM_DITHER.u32 = pstIspReg->ISP_ACM_DITHER.u32;
	ISP_ACM_DITHER.bits.isp_dither_en = data;
	pstIspReg->ISP_ACM_DITHER.u32 = ISP_ACM_DITHER.u32;
}

static __inline HI_VOID hi_isp_acm_dither_round_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_ACM_DITHER ISP_ACM_DITHER;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_ACM_DITHER.u32 = pstIspReg->ISP_ACM_DITHER.u32;
	ISP_ACM_DITHER.bits.isp_dither_round = data;
	pstIspReg->ISP_ACM_DITHER.u32 = ISP_ACM_DITHER.u32;
}

static __inline HI_VOID hi_isp_acm_dither_spatial_mode_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_ACM_DITHER ISP_ACM_DITHER;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_ACM_DITHER.u32 = pstIspReg->ISP_ACM_DITHER.u32;
	ISP_ACM_DITHER.bits.spatial_mode = data;
	pstIspReg->ISP_ACM_DITHER.u32 = ISP_ACM_DITHER.u32;
}


static __inline HI_VOID hi_isp_acm_dither_out_bits_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_ACM_DITHER ISP_ACM_DITHER;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_ACM_DITHER.u32 = pstIspReg->ISP_ACM_DITHER.u32;
	ISP_ACM_DITHER.bits.out_bits = data;
	pstIspReg->ISP_ACM_DITHER.u32 = ISP_ACM_DITHER.u32;
}


/*****************************************************
ISP_DMNR_DITHER:    ISP DMNR DITHER 
0x****0840:         31:08      reserved
					07:04      out_bits
					03:03      spatial_mode
					02:01      isp_dither_round
					00:00      isp_dither_en
*****************************************************/
static __inline HI_VOID hi_isp_dmnr_dither_en_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DMNR_DITHER ISP_DMNR_DITHER;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DMNR_DITHER.u32 = pstIspReg->ISP_DMNR_DITHER.u32;
	ISP_DMNR_DITHER.bits.isp_dither_en = data;
	pstIspReg->ISP_DMNR_DITHER.u32 = ISP_DMNR_DITHER.u32;
}

static __inline HI_VOID hi_isp_dmnr_dither_round_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DMNR_DITHER ISP_DMNR_DITHER;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DMNR_DITHER.u32 = pstIspReg->ISP_DMNR_DITHER.u32;
	ISP_DMNR_DITHER.bits.isp_dither_round = data;
	pstIspReg->ISP_DMNR_DITHER.u32 = ISP_DMNR_DITHER.u32;
}

static __inline HI_VOID hi_isp_dmnr_dither_spatial_mode_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DMNR_DITHER ISP_DMNR_DITHER;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DMNR_DITHER.u32 = pstIspReg->ISP_DMNR_DITHER.u32;
	ISP_DMNR_DITHER.bits.spatial_mode = data;
	pstIspReg->ISP_DMNR_DITHER.u32 = ISP_DMNR_DITHER.u32;
}

static __inline HI_VOID hi_isp_dmnr_dither_out_bits_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DMNR_DITHER ISP_DMNR_DITHER;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DMNR_DITHER.u32 = pstIspReg->ISP_DMNR_DITHER.u32;
	ISP_DMNR_DITHER.bits.out_bits = data;
	pstIspReg->ISP_DMNR_DITHER.u32 = ISP_DMNR_DITHER.u32;
}

/*****************************************************
ISP_DRC_DITHER:     ISP DRC DITHER 
0x****0820:         31:08      reserved
					07:04      out_bits
					03:03      spatial_mode
					02:01      isp_dither_round
					00:00      isp_dither_en
*****************************************************/
static __inline HI_VOID hi_isp_drc_moudle_dither_en_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_DITHER ISP_DRC_DITHER;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_DITHER.u32 = pstIspReg->ISP_DRC_DITHER.u32;
	ISP_DRC_DITHER.bits.isp_dither_en = data;
	pstIspReg->ISP_DRC_DITHER.u32 = ISP_DRC_DITHER.u32;
}

static __inline HI_VOID hi_isp_drc_dither_round_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_DITHER ISP_DRC_DITHER;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_DITHER.u32 = pstIspReg->ISP_DRC_DITHER.u32;
	ISP_DRC_DITHER.bits.isp_dither_round = data;
	pstIspReg->ISP_DRC_DITHER.u32 = ISP_DRC_DITHER.u32;
}

static __inline HI_VOID hi_isp_drc_dither_spatial_mode_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_DITHER ISP_DRC_DITHER;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_DITHER.u32 = pstIspReg->ISP_DRC_DITHER.u32;
	ISP_DRC_DITHER.bits.spatial_mode = data;
	pstIspReg->ISP_DRC_DITHER.u32 = ISP_DRC_DITHER.u32;
}

static __inline HI_VOID hi_isp_drc_dither_out_bits_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_DITHER ISP_DRC_DITHER;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_DITHER.u32 = pstIspReg->ISP_DRC_DITHER.u32;
	ISP_DRC_DITHER.bits.out_bits = data;
	pstIspReg->ISP_DRC_DITHER.u32 = ISP_DRC_DITHER.u32;
}

#define HI_ISP_CPI_FIX_TIMING_STAT (200)
/*****************************************************
ISP_YUV444_TIMING_CFG:         
0x****01e8:                 31:14      reserved
							13:01      cpi_fix_timing_stat
							00:00     cpi_auto_timing_en
*****************************************************/
static __inline HI_VOID hi_isp_yuv444_timing_stat_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_FE_TIMING_CFG ISP_FE_TIMING_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_FE_TIMING_CFG.u32 = pstIspReg->ISP_FE_TIMING_CFG.u32;
	ISP_FE_TIMING_CFG.bits.fix_timing = data;
	pstIspReg->ISP_FE_TIMING_CFG.u32 = ISP_FE_TIMING_CFG.u32;
}

/*****************************************************
ISP_YUV422_TIMING_CFG:         
0x****01e8:                 31:14      reserved
							13:01      cpi_fix_timing_stat
							00:00     cpi_auto_timing_en
*****************************************************/
static __inline HI_VOID hi_isp_yuv422_timing_stat_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BE_TIMING_CFG ISP_BE_TIMING_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BE_TIMING_CFG.u32 = pstIspReg->ISP_BE_TIMING_CFG.u32;
	ISP_BE_TIMING_CFG.bits.fix_timing = data;
	pstIspReg->ISP_BE_TIMING_CFG.u32 = ISP_BE_TIMING_CFG.u32;
}

/*****************************************************
ISP_FE_CTRL_F:      
0x****01E0:         31:02     reserved
					01:00     rggb_cfg
					RGGB start

*****************************************************/
static __inline HI_VOID hi_isp_yuv444_rggb_start_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_FE_CTRL_F ISP_FE_CTRL_F;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_FE_CTRL_F.u32 = pstIspReg->ISP_FE_CTRL_F.u32;
	ISP_FE_CTRL_F.bits.rggb_cfg = data;  
	pstIspReg->ISP_FE_CTRL_F.u32 = ISP_FE_CTRL_F.u32;
}
static __inline HI_U8 hi_isp_yuv444_rggb_start_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_FE_CTRL_F ISP_FE_CTRL_F;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_FE_CTRL_F.u32 = pstIspReg->ISP_FE_CTRL_F.u32;

	return (ISP_FE_CTRL_F.bits.rggb_cfg); 
}

/*****************************************************
ISP_YUV422_CTRL_F:   
0x****01E0:          31:02     reserved
					 01:00     rggb_cfg
*****************************************************/
static __inline HI_VOID hi_isp_yuv422_rggb_start_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BE_CTRL_F ISP_BE_CTRL_F;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BE_CTRL_F.u32 = pstIspReg->ISP_BE_CTRL_F.u32;
	ISP_BE_CTRL_F.bits.rggb_cfg = data;    
	pstIspReg->ISP_BE_CTRL_F.u32 = ISP_BE_CTRL_F.u32;
}
static __inline HI_U8 hi_isp_yuv422_rggb_start_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BE_CTRL_F ISP_BE_CTRL_F;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BE_CTRL_F.u32 = pstIspReg->ISP_BE_CTRL_F.u32;

	return (ISP_BE_CTRL_F.bits.rggb_cfg); 
}

/*****************************************************
MODULE        -----AE-----
MODULE        -----AE-----
MODULE        -----AE-----
*****************************************************/
/*****************************************************
ISP_AE_CFG:                    
0x****2000:                31:1      reserved
                           0      AE enable
*****************************************************/
static __inline HI_VOID hi_isp_ae_cfg_enable_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_CFG ISP_AE_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AE_CFG.u32 = pstIspReg->ISP_AE_CFG.u32;
	ISP_AE_CFG.bits.en = data;
	pstIspReg->ISP_AE_CFG.u32 = ISP_AE_CFG.u32;
}

#define HI_ISP_METERING_AEXP_NODES_USED_HORIZ_DEFAULT (17)
#define HI_ISP_METERING_AEXP_NODES_USED_VERT_DEFAULT  (15)

/*****************************************************
ISP_AE_ZONE:               AE wondow number
0x****2010  :              31:13      reserved
                           12:  8      vertical window number
                           7 :  5      reserved
                           4 :  0      horizontal window number
*****************************************************/
static __inline HI_VOID hi_isp_ae_h_zone_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_ZONE ISP_AE_ZONE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AE_ZONE.u32 = pstIspReg->ISP_AE_ZONE.u32;
	ISP_AE_ZONE.bits.hnum = data;
	pstIspReg->ISP_AE_ZONE.u32 = ISP_AE_ZONE.u32;
}

static __inline HI_VOID hi_isp_ae_v_zone_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_ZONE ISP_AE_ZONE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AE_ZONE.u32 = pstIspReg->ISP_AE_ZONE.u32;
	ISP_AE_ZONE.bits.vnum = data;
	pstIspReg->ISP_AE_ZONE.u32 = ISP_AE_ZONE.u32;
}

/*****************************************************
ISP_AE_SKIP_CRG:                AE  pixel configuration
0x****2014          :                31:  8      reserved
                                       7  :  7      0=count begin the 0 column  1=ount begin the 1 column
                                       6  :  4      Histogram decimation in vertical direction:
									0=every pixel;      
									1=every 2nd pixel;
									2=every 3rd pixel;  
									3=every 4th pixel;
									4=every 5th pixel;  
									5=every 8th pixel ;
									6+=every 9th pixel
									
                                       2  :  0      Histogram decimation in horizontal direction:
									 0=every 2nd pixel;
									 1=every 3rd pixel;
									 2=every 4th pixel; 
									 3=every 5th pixel;
									 4=every 8th pixel ;
									 5+=every 9th pixel
*****************************************************/
static __inline HI_VOID hi_isp_metering_offset_y_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_SKIP_CRG ISP_AE_SKIP_CRG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AE_SKIP_CRG.u32 = pstIspReg->ISP_AE_SKIP_CRG.u32;
	ISP_AE_SKIP_CRG.bits.offset_y = data;
	pstIspReg->ISP_AE_SKIP_CRG.u32 = ISP_AE_SKIP_CRG.u32;
}

static __inline HI_U8 hi_isp_metering_offset_y_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_SKIP_CRG ISP_AE_SKIP_CRG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AE_SKIP_CRG.u32 = pstIspReg->ISP_AE_SKIP_CRG.u32;
	return ISP_AE_SKIP_CRG.bits.offset_y;
}

static __inline HI_VOID hi_isp_metering_skip_y_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_SKIP_CRG ISP_AE_SKIP_CRG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AE_SKIP_CRG.u32 = pstIspReg->ISP_AE_SKIP_CRG.u32;
	ISP_AE_SKIP_CRG.bits.skip_y = data;
	pstIspReg->ISP_AE_SKIP_CRG.u32 = ISP_AE_SKIP_CRG.u32;
}

static __inline HI_U8 hi_isp_metering_skip_y_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_SKIP_CRG ISP_AE_SKIP_CRG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AE_SKIP_CRG.u32 = pstIspReg->ISP_AE_SKIP_CRG.u32;
	return ISP_AE_SKIP_CRG.bits.skip_y;
}

static __inline HI_VOID hi_isp_metering_offset_x_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_SKIP_CRG ISP_AE_SKIP_CRG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AE_SKIP_CRG.u32 = pstIspReg->ISP_AE_SKIP_CRG.u32;
	ISP_AE_SKIP_CRG.bits.offset_x = data;
	pstIspReg->ISP_AE_SKIP_CRG.u32 = ISP_AE_SKIP_CRG.u32;
}

static __inline HI_U8 hi_isp_metering_offset_x_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_SKIP_CRG ISP_AE_SKIP_CRG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AE_SKIP_CRG.u32 = pstIspReg->ISP_AE_SKIP_CRG.u32;
	return ISP_AE_SKIP_CRG.bits.offset_x;
}

static __inline HI_VOID hi_isp_metering_skip_x_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_SKIP_CRG ISP_AE_SKIP_CRG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AE_SKIP_CRG.u32 = pstIspReg->ISP_AE_SKIP_CRG.u32;
	ISP_AE_SKIP_CRG.bits.skip_x = data;
	pstIspReg->ISP_AE_SKIP_CRG.u32 = ISP_AE_SKIP_CRG.u32;
}

static __inline HI_U8 hi_isp_metering_skip_x_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_SKIP_CRG ISP_AE_SKIP_CRG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AE_SKIP_CRG.u32 = pstIspReg->ISP_AE_SKIP_CRG.u32;
	return ISP_AE_SKIP_CRG.bits.skip_x;
}


/*****************************************************
ISP_AE_TOTAL_STAT:     
0x205a2018          :        31:  24      reserved                                    
                                       23  :  0      256 histogram statistics,  the total number of selected pixel in one frame
*****************************************************/
static __inline HI_U32 hi_isp_ae_pixel_selected_count_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_TOTAL_STAT ISP_AE_TOTAL_STAT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AE_TOTAL_STAT.u32 = pstIspReg->ISP_AE_TOTAL_STAT.u32;

	return (ISP_AE_TOTAL_STAT.bits.total_pixels);
}


/*****************************************************
ISP_AE_COUNT_STAT:     256
0x205a201c          :         31:  28     reserved
                                       27  :  0      256
*****************************************************/
static __inline HI_U32 hi_isp_ae_pixel_selected_weight_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_COUNT_STAT ISP_AE_COUNT_STAT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AE_COUNT_STAT.u32 = pstIspReg->ISP_AE_COUNT_STAT.u32;

	return (ISP_AE_COUNT_STAT.bits.count_pixels);
}


/*****************************************************
ISP_AE_TOTAL_R_AVER:     
0x205a2020          :         31:0    global r average statistic                                             
*****************************************************/
static __inline HI_U32 hi_isp_ae_total_r_aver_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_TOTAL_R_AVER ISP_AE_TOTAL_R_AVER;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AE_TOTAL_R_AVER.u32 = pstIspReg->ISP_AE_TOTAL_R_AVER.u32;

	return (ISP_AE_TOTAL_R_AVER.bits.total_r_aver);
}

/*****************************************************
ISP_AE_TOTAL_Gr_AVER:     
0x205a2024          :            31:0   global gr average statistic
*****************************************************/
static __inline HI_U32 hi_isp_ae_total_gr_aver_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_TOTAL_GR_AVER ISP_AE_TOTAL_GR_AVER;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AE_TOTAL_GR_AVER.u32 = pstIspReg->ISP_AE_TOTAL_GR_AVER.u32;

	return (ISP_AE_TOTAL_GR_AVER.bits.total_gr_aver);
}

/*****************************************************
ISP_AE_TOTAL_Gb_AVER:     
0x205a2028          :         31:0   global gb average statistic                                                          
*****************************************************/
static __inline HI_U32 hi_isp_ae_total_gb_aver_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_TOTAL_GB_AVER ISP_AE_TOTAL_GB_AVER;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AE_TOTAL_GB_AVER.u32 = pstIspReg->ISP_AE_TOTAL_GB_AVER.u32;

	return (ISP_AE_TOTAL_GB_AVER.bits.total_gb_aver);
}


/*****************************************************
ISP_AE_TOTAL_b_AVER:    
0x205a202c          :         31:0   global b average statistic                                                         
*****************************************************/
static __inline HI_U32 hi_isp_ae_total_b_aver_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_TOTAL_B_AVER ISP_AE_TOTAL_B_AVER;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AE_TOTAL_B_AVER.u32 = pstIspReg->ISP_AE_TOTAL_B_AVER.u32;

	return (ISP_AE_TOTAL_B_AVER.bits.total_b_aver);
}

/*****************************************************
ISP_AE_BITMOVE:     256
0x205a2040         : 31 :  10      reserved  
					9	     blc_en
					8	     gamma_en
					7   :  0              
*****************************************************/
static __inline HI_VOID hi_isp_ae_blc_en_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_BITMOVE ISP_AE_BITMOVE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AE_BITMOVE.u32 = pstIspReg->ISP_AE_BITMOVE.u32;
	ISP_AE_BITMOVE.bits.blc_en = data;
	pstIspReg->ISP_AE_BITMOVE.u32 = ISP_AE_BITMOVE.u32;
}

static __inline HI_VOID hi_isp_ae_gamma_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_BITMOVE ISP_AE_BITMOVE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AE_BITMOVE.u32 = pstIspReg->ISP_AE_BITMOVE.u32;
	ISP_AE_BITMOVE.bits.gamma_en = data;
	pstIspReg->ISP_AE_BITMOVE.u32 = ISP_AE_BITMOVE.u32;
}

static __inline HI_U8 hi_isp_ae_gamma_read(HI_U8 ispDev){
    S_HIISP_REGS_TYPE * pstIspReg;
    U_ISP_AE_BITMOVE ISP_AE_BITMOVE;
    pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
    ISP_AE_BITMOVE.u32 = pstIspReg->ISP_AE_BITMOVE.u32;
    
    return (ISP_AE_BITMOVE.bits.gamma_en);
}

static __inline HI_VOID hi_isp_ae_bit_move_gamma_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_BITMOVE ISP_AE_BITMOVE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AE_BITMOVE.u32 = pstIspReg->ISP_AE_BITMOVE.u32;
	ISP_AE_BITMOVE.bits.bitmove = data;
	pstIspReg->ISP_AE_BITMOVE.u32 = ISP_AE_BITMOVE.u32;
}

/*****************************************************
ISP_AE_OFFSET_R:     AE BLC
0x205a2044:          31:15  reserved
					 14:0	offset_r
*****************************************************/
static __inline HI_VOID hi_isp_ae_offset_r_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_OFFSET_R ISP_AE_OFFSET_R;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AE_OFFSET_R.u32 = pstIspReg->ISP_AE_OFFSET_R.u32;
	ISP_AE_OFFSET_R.bits.offset_r = data;
	pstIspReg->ISP_AE_OFFSET_R.u32 = ISP_AE_OFFSET_R.u32;
}

/*****************************************************
ISP_AE_OFFSET_Gr:     AE BLC
0x205a2048                       :     31:15   reserved
								14:0		offset_gr
*****************************************************/
static __inline HI_VOID hi_isp_ae_offset_gr_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_OFFSET_GR ISP_AE_OFFSET_GR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AE_OFFSET_GR.u32 = pstIspReg->ISP_AE_OFFSET_GR.u32;
	ISP_AE_OFFSET_GR.bits.offset_gr = data;
	pstIspReg->ISP_AE_OFFSET_GR.u32 = ISP_AE_OFFSET_GR.u32;
}

/*****************************************************
ISP_AE_OFFSET_Gb     AE BLC
0x205a204c                       :     31:15   reserved
								14:0		offset_gb
*****************************************************/
static __inline HI_VOID hi_isp_ae_offset_gb_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_OFFSET_GB ISP_AE_OFFSET_GB;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AE_OFFSET_GB.u32 = pstIspReg->ISP_AE_OFFSET_GB.u32;
	ISP_AE_OFFSET_GB.bits.offset_gb = data;
	pstIspReg->ISP_AE_OFFSET_GB.u32 = ISP_AE_OFFSET_GB.u32;
}

/*****************************************************
ISP_AE_OFFSET_B     AE BLC
0x205a2050                       :     31:15   reserved
								14:0		offset_b
*****************************************************/
static __inline HI_VOID hi_isp_ae_offset_b_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_OFFSET_B ISP_AE_OFFSET_B;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AE_OFFSET_B.u32 = pstIspReg->ISP_AE_OFFSET_B.u32;
	ISP_AE_OFFSET_B.bits.offset_b = data;
	pstIspReg->ISP_AE_OFFSET_B.u32 = ISP_AE_OFFSET_B.u32;
}

/*****************************************************
ISP_AE_OFFSET_B     AE BLC
0x205a2060          31:15   reserved
					03:00	gamma_limit
*****************************************************/
static __inline HI_VOID hi_isp_ae_gamma_limit_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_GAMMA_LIMIT ISP_AE_GAMMA_LIMIT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AE_GAMMA_LIMIT.u32 = pstIspReg->ISP_AE_GAMMA_LIMIT.u32;
	ISP_AE_GAMMA_LIMIT.bits.gamma_limit = data;	
	pstIspReg->ISP_AE_GAMMA_LIMIT.u32 = ISP_AE_GAMMA_LIMIT.u32;
}

/*****************************************************
U_ISP_AE_FOURPLANEMODE     AE BLC
0x205a2060          31:01   reserved
				  00:00	fourplanemode
*****************************************************/
static __inline HI_VOID hi_isp_ae_fourplanemode_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_FOURPLANEMODE ISP_AE_FOURPLANEMODE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AE_FOURPLANEMODE.u32 = pstIspReg->ISP_AE_FOURPLANEMODE.u32;
	ISP_AE_FOURPLANEMODE.bits.fourplanemode = data;	
	pstIspReg->ISP_AE_FOURPLANEMODE.u32 = ISP_AE_FOURPLANEMODE.u32;
}

#define HI_ISP_AE_FOUR_PLANE_MODE_DISABLE (0)
#define HI_ISP_AE_FOUR_PLANE_MODE_ENABLE (1)
static __inline HI_U8 hi_isp_ae_fourplanemode_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_FOURPLANEMODE ISP_AE_FOURPLANEMODE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AE_FOURPLANEMODE.u32 = pstIspReg->ISP_AE_FOURPLANEMODE.u32;
	return ISP_AE_FOURPLANEMODE.bits.fourplanemode;		
}


/*****************************************************
ISP_AE_MEM_HIST_RADDR:     AE
0x205a2088                       :     31:0 histogram statistic read address                                                     
*****************************************************/
static __inline HI_VOID hi_isp_ae_mem_hist_raddr_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_MEM_HIST_RADDR ISP_AE_MEM_HIST_RADDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AE_MEM_HIST_RADDR.u32 = data;	
	pstIspReg->ISP_AE_MEM_HIST_RADDR.u32 = ISP_AE_MEM_HIST_RADDR.u32;
}

/*****************************************************
ISP_AE_MEM_HIST_RDATA:     AE
0x205a208c:                31:0   histogram statistic read data                                                                          
*****************************************************/
static __inline HI_U32 hi_isp_ae_mem_hist_rdata_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_MEM_HIST_RDATA ISP_AE_MEM_HIST_RDATA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AE_MEM_HIST_RDATA.u32 = pstIspReg->ISP_AE_MEM_HIST_RDATA.u32;
	
	return ISP_AE_MEM_HIST_RDATA.u32;
}

/*****************************************************
ISP_AE_MEM_AVER_R_GR_RADDR:
0x205a2098                31:0                                                          
*****************************************************/
static __inline HI_VOID hi_isp_ae_mem_aver_r_gr_raddr_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_MEM_AVER_R_GR_RADDR ISP_AE_MEM_AVER_R_GR_RADDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));

	ISP_AE_MEM_AVER_R_GR_RADDR.u32 = data;
	pstIspReg->ISP_AE_MEM_AVER_R_GR_RADDR.u32 = ISP_AE_MEM_AVER_R_GR_RADDR.u32;
}

/*****************************************************
ISP_AE_MEM_AVER_RDATA:     AE
0x205a209c                 :     31:0
*****************************************************/
static __inline HI_U32 hi_isp_ae_mem_aver_r_gr_rdata_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_MEM_AVER_R_GR_RDATA ISP_AE_MEM_AVER_R_GR_RDATA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AE_MEM_AVER_R_GR_RDATA.u32 = pstIspReg->ISP_AE_MEM_AVER_R_GR_RDATA.u32;
	
	return ISP_AE_MEM_AVER_R_GR_RDATA.u32;
}

/*****************************************************
ISP_AE_MEM_AVER_GB_B_RADDR:     AE
0x205a2098                      :     31:0
*****************************************************/
static __inline HI_VOID hi_isp_ae_mem_aver_gb_b_raddr_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_MEM_AVER_GB_B_RADDR ISP_AE_MEM_AVER_GB_B_RADDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	
	ISP_AE_MEM_AVER_GB_B_RADDR.u32 = data;
	pstIspReg->ISP_AE_MEM_AVER_GB_B_RADDR.u32 = ISP_AE_MEM_AVER_GB_B_RADDR.u32;
}
#define HI_ISP_AE_WEI_COMBIN_COUNT  4
/*****************************************************
ISP_AE_MEM_AVER_GB_B_RDATA:     AE
0x205a209c                      :     31:0
*****************************************************/
static __inline HI_U32 hi_isp_ae_mem_aver_gb_b_rdata_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_MEM_AVER_GB_B_RDATA ISP_AE_MEM_AVER_GB_B_RDATA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AE_MEM_AVER_GB_B_RDATA.u32 = pstIspReg->ISP_AE_MEM_AVER_GB_B_RDATA.u32;
	
	return ISP_AE_MEM_AVER_GB_B_RDATA.u32;
}
/*****************************************************
ISP_AE_MEM_WEIGHT_WADDR:     AE
0x205a20b0                   31:0
*****************************************************/
static __inline HI_VOID hi_isp_ae_mem_wei_waddr_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_MEM_WEIGHT_WADDR ISP_AE_MEM_WEIGHT_WADDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AE_MEM_WEIGHT_WADDR.u32 = data;
	pstIspReg->ISP_AE_MEM_WEIGHT_WADDR.u32 = ISP_AE_MEM_WEIGHT_WADDR.u32;
}

/*****************************************************
ISP_AE_MEM_WEIGHT_WDATA:     AE
0x205a20a4                   31:0
*****************************************************/
static __inline HI_VOID hi_isp_ae_mem_wei_wdata_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_MEM_WEIGHT_WDATA ISP_AE_MEM_WEIGHT_WDATA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AE_MEM_WEIGHT_WDATA.u32 = data;
	pstIspReg->ISP_AE_MEM_WEIGHT_WDATA.u32 = ISP_AE_MEM_WEIGHT_WDATA.u32;
}

static __inline HI_U32 hi_isp_ae_mem_wei_wdata_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_MEM_WEIGHT_WDATA ISP_AE_MEM_WEIGHT_WDATA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AE_MEM_WEIGHT_WDATA.u32 = pstIspReg->ISP_AE_MEM_WEIGHT_WDATA.u32;
	
	return ISP_AE_MEM_WEIGHT_WDATA.u32;
}


/*****************************************************
ISP_AE_MEM_WEIGHT_WADDR:     AE
0x205a20a8                   31:0
*****************************************************/
static __inline HI_VOID hi_isp_ae_mem_wei_raddr_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_MEM_WEIGHT_RADDR ISP_AE_MEM_WEIGHT_RADDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));

	ISP_AE_MEM_WEIGHT_RADDR.u32 = data;
	pstIspReg->ISP_AE_MEM_WEIGHT_RADDR.u32 = ISP_AE_MEM_WEIGHT_RADDR.u32;
}

/*****************************************************
ISP_AE_MEM_WEIGHT_WDATA:     AE
0x205a20ac                   31:0
*****************************************************/
static __inline HI_U32 hi_isp_ae_mem_wei_rdata_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_MEM_WEIGHT_RDATA ISP_AE_MEM_WEIGHT_RDATA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AE_MEM_WEIGHT_RDATA.u32 = pstIspReg->ISP_AE_MEM_WEIGHT_RDATA.u32;
	
	return ISP_AE_MEM_WEIGHT_RDATA.u32;
}

#define HI_ISP_TOP_RGGB_START_R_GR_GB_B (0)
#define HI_ISP_TOP_RGGB_START_GR_R_B_GB (1)
#define HI_ISP_TOP_RGGB_START_GB_B_R_GR (2)
#define HI_ISP_TOP_RGGB_START_B_GB_GR_R (3)

/*****************************************************
ISP_AE_CTRL_F                        :     AE    
0x205a20e0                             :     31:2   reserved  
						  
*****************************************************/
static __inline HI_U8 hi_isp_ae_ctrl_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_CTRL_F ISP_AE_CTRL_F;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AE_CTRL_F.u32 = pstIspReg->ISP_AE_CTRL_F.u32;
	
	return ISP_AE_CTRL_F.bits.rggb_cfg;
}

/*****************************************************
ISP_AE_SIZE                            :     AE  
0x205a20f0                             :     31:28   reserved  
								    27:16   image hight
								    15:12   reserved
								    11:  0  image width
*****************************************************/
static __inline HI_VOID hi_isp_ae_width_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_SIZE ISP_AE_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AE_SIZE.u32 = pstIspReg->ISP_AE_SIZE.u32;
	ISP_AE_SIZE.bits.hsize = data;
	pstIspReg->ISP_AE_SIZE.u32 = ISP_AE_SIZE.u32;
}

static __inline HI_VOID hi_isp_ae_height_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AE_SIZE ISP_AE_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AE_SIZE.u32 = pstIspReg->ISP_AE_SIZE.u32;
	ISP_AE_SIZE.bits.vsize = data;
	pstIspReg->ISP_AE_SIZE.u32 = ISP_AE_SIZE.u32;
}

/*****************************************************
MODULE        -----AWB-----
MODULE        -----AWB-----
MODULE        -----AWB-----
*****************************************************/
/*****************************************************
ISP_AWB_CFG:                    
0x205a2100:                31:1      reserved
                           0      AWBenable
*****************************************************/
static __inline HI_VOID hi_isp_awb_cfg_enable_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AWB_CFG ISP_AWB_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AWB_CFG.u32 = pstIspReg->ISP_AWB_CFG.u32;
	ISP_AWB_CFG.bits.work_en = data;
	pstIspReg->ISP_AWB_CFG.u32 = ISP_AWB_CFG.u32;
}

#define HI_ISP_METERING_AWB_NODES_USED_HORIZ_DEFAULT (AWB_ZONE_COLUMN)
#define HI_ISP_METERING_AWB_NODES_USED_VERT_DEFAULT (AWB_ZONE_ROW)

/*****************************************************
ISP_AWB_ZONE:             AWB
0x205a2110  :                31:13      reserved
                           12:  8      vertical window number
                           7  :  5      reserved
                           4  :  0      horizontal window number
*****************************************************/
static __inline HI_VOID hi_isp_awb_h_zone_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AWB_ZONE ISP_AWB_ZONE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AWB_ZONE.u32 = pstIspReg->ISP_AWB_ZONE.u32;
	ISP_AWB_ZONE.bits.hnum = data;
	pstIspReg->ISP_AWB_ZONE.u32 = ISP_AWB_ZONE.u32;
}

static __inline HI_VOID hi_isp_awb_v_zone_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AWB_ZONE ISP_AWB_ZONE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AWB_ZONE.u32 = pstIspReg->ISP_AWB_ZONE.u32;
	ISP_AWB_ZONE.bits.vnum = data;
	pstIspReg->ISP_AWB_ZONE.u32 = ISP_AWB_ZONE.u32;
}

/*****************************************************
ISP_AWB_THD_MIN:       
0x205a2118  :          31:16      reserved
                       15:  0     Minimal threshold for AWB sums
*****************************************************/
static __inline HI_VOID hi_isp_metering_min_threshold_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AWB_THD_MIN ISP_AWB_THD_MIN;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AWB_THD_MIN.u32 = pstIspReg->ISP_AWB_THD_MIN.u32;
	ISP_AWB_THD_MIN.bits.threshold_min = data;
	pstIspReg->ISP_AWB_THD_MIN.u32 = ISP_AWB_THD_MIN.u32;
}

static __inline HI_U16 hi_isp_metering_min_threshold_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AWB_THD_MIN ISP_AWB_THD_MIN;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AWB_THD_MIN.u32 = pstIspReg->ISP_AWB_THD_MIN.u32;
	
	return ISP_AWB_THD_MIN.bits.threshold_min;
}

/*****************************************************
ISP_AWB_THD_MAX:     
0x205a211c  :          31:16      reserved
                       15:00     Maximal threshold for AWB sums
*****************************************************/
static __inline HI_VOID hi_isp_metering_max_threshold_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AWB_THD_MAX ISP_AWB_THD_MAX;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AWB_THD_MAX.u32 = pstIspReg->ISP_AWB_THD_MAX.u32;
	ISP_AWB_THD_MAX.bits.threshold_max = data;
	pstIspReg->ISP_AWB_THD_MAX.u32 = ISP_AWB_THD_MAX.u32;
}

static __inline HI_U16 hi_isp_metering_max_threshold_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AWB_THD_MAX ISP_AWB_THD_MAX;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AWB_THD_MAX.u32 = pstIspReg->ISP_AWB_THD_MAX.u32;
	
	return ISP_AWB_THD_MAX.bits.threshold_max;
}

/*****************************************************
ISP_AWB_CR_MM:    
0x205a2120  :                31:28     reserved
						 27:16     Minimum value of R/G for white region.unsigned 4.8-bit fixed-point
						 15:12     reserved
                                       11:  0     Maximum value of R/G for white region.unsigned 4.8-bit fixed-point
*****************************************************/
static __inline HI_VOID hi_isp_metering_cr_ref_max_awb_sum_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AWB_CR_MM ISP_AWB_CR_MM;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AWB_CR_MM.u32 = pstIspReg->ISP_AWB_CR_MM.u32;
	ISP_AWB_CR_MM.bits.cr_ref_max = data;
	pstIspReg->ISP_AWB_CR_MM.u32 = ISP_AWB_CR_MM.u32;
}

static __inline HI_U16 hi_isp_metering_cr_ref_max_awb_sum_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AWB_CR_MM ISP_AWB_CR_MM;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AWB_CR_MM.u32 = pstIspReg->ISP_AWB_CR_MM.u32;
	
	return ISP_AWB_CR_MM.bits.cr_ref_max;
}

static __inline HI_VOID hi_isp_metering_cr_ref_min_awb_sum_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AWB_CR_MM ISP_AWB_CR_MM;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AWB_CR_MM.u32 = pstIspReg->ISP_AWB_CR_MM.u32;
	ISP_AWB_CR_MM.bits.cr_ref_min = data;
	pstIspReg->ISP_AWB_CR_MM.u32 = ISP_AWB_CR_MM.u32;
}

static __inline HI_U16 hi_isp_metering_cr_ref_min_awb_sum_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AWB_CR_MM ISP_AWB_CR_MM;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AWB_CR_MM.u32 = pstIspReg->ISP_AWB_CR_MM.u32;
	
	return ISP_AWB_CR_MM.bits.cr_ref_min;
}

/*****************************************************
ISP_AWB_CB_MM:   
0x205a2124  :                31:28     reserved
						 27:16     Minimum value of B/G for white region.unsigned 4.8-bit fixed-point
						 15:12     reserved
                                       11:  0     Maximum value of B/G for white region.unsigned 4.8-bit fixed-point
*****************************************************/
static __inline HI_VOID hi_isp_metering_cb_ref_max_awb_sum_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AWB_CB_MM ISP_AWB_CB_MM;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AWB_CB_MM.u32 = pstIspReg->ISP_AWB_CB_MM.u32;
	ISP_AWB_CB_MM.bits.cb_ref_max = data;
	pstIspReg->ISP_AWB_CB_MM.u32 = ISP_AWB_CB_MM.u32;
}

static __inline HI_U16 hi_isp_metering_cb_ref_max_awb_sum_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AWB_CB_MM ISP_AWB_CB_MM;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AWB_CB_MM.u32 = pstIspReg->ISP_AWB_CB_MM.u32;
	
	return ISP_AWB_CB_MM.bits.cb_ref_max;
}

static __inline HI_VOID hi_isp_metering_cb_ref_min_awb_sum_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AWB_CB_MM ISP_AWB_CB_MM;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AWB_CB_MM.u32 = pstIspReg->ISP_AWB_CB_MM.u32;
	ISP_AWB_CB_MM.bits.cb_ref_min = data;
	pstIspReg->ISP_AWB_CB_MM.u32 = ISP_AWB_CB_MM.u32;
}

static __inline HI_U16 hi_isp_metering_cb_ref_min_awb_sum_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AWB_CB_MM ISP_AWB_CB_MM;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AWB_CB_MM.u32 = pstIspReg->ISP_AWB_CB_MM.u32;
	
	return ISP_AWB_CB_MM.bits.cb_ref_min;
}

#define HI_ISP_AWB_OFFSET_COMP_DEF (0)
/*****************************************************
ISP_AWB_OFFSET_COMP:     AWB  statistic offset comp
0x113a2128  :            31:12     reserved
						 11:00     offset_comp
*****************************************************/
static __inline HI_VOID hi_isp_awb_offset_comp_write(HI_U8 ispDev, HI_U16 data) {
    S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AWB_OFFSET_COMP ISP_AWB_OFFSET_COMP;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AWB_OFFSET_COMP.u32 = pstIspReg->ISP_AWB_OFFSET_COMP.u32;
	ISP_AWB_OFFSET_COMP.bits.offset_comp = data;
	pstIspReg->ISP_AWB_OFFSET_COMP.u32 = ISP_AWB_OFFSET_COMP.u32;
}

/*****************************************************
ISP_AWB_TOP_K:           AWB grey area top slope
0x205a2130  :                31:25     reserved
						 24:0     AWB grey area top slope .signed 13.12-bit
*****************************************************/
static __inline HI_VOID hi_isp_awb_top_hypotenuse_k_write(HI_U8 ispDev, HI_U32 data) {
    S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AWB_TOP_K ISP_AWB_TOP_K;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AWB_TOP_K.u32 = pstIspReg->ISP_AWB_TOP_K.u32;
	ISP_AWB_TOP_K.bits.awb_top_k = data;
	pstIspReg->ISP_AWB_TOP_K.u32 = ISP_AWB_TOP_K.u32;
}

/*****************************************************
ISP_AWB_TOP_B:          AWB
0x205a2134  :                31:24     reserved
						 23:0     AWB unsigned 16.8-bit
*****************************************************/
static __inline HI_VOID hi_isp_awb_top_hypotenuse_b_write(HI_U8 ispDev, HI_U32 data) {
    S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AWB_TOP_B ISP_AWB_TOP_B;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AWB_TOP_B.u32 = pstIspReg->ISP_AWB_TOP_B.u32;
	ISP_AWB_TOP_B.bits.awb_top_b = data;
	pstIspReg->ISP_AWB_TOP_B.u32 = ISP_AWB_TOP_B.u32;
}

/*****************************************************
ISP_AWB_BOT_K:           AWB grey area bottom slope
0x205a2138  :                31:25     reserved
						 24:0     AWB signed 13.12-bit
*****************************************************/
static __inline HI_VOID hi_isp_awb_bot_hypotenuse_k_write(HI_U8 ispDev, HI_U32 data) {
    S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AWB_BOT_K ISP_AWB_BOT_K;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AWB_BOT_K.u32 = pstIspReg->ISP_AWB_BOT_K.u32;
	ISP_AWB_BOT_K.bits.awb_bot_k = data;
	pstIspReg->ISP_AWB_BOT_K.u32 = ISP_AWB_BOT_K.u32;
}

/*****************************************************
ISP_AWB_BOT_B:          AWB
0x205a213c  :                31:24     reserved
						 23:0     AWB unsigned 16.8-bit
*****************************************************/
static __inline HI_VOID hi_isp_awb_bot_hypotenuse_b_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AWB_BOT_B ISP_AWB_BOT_B;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AWB_BOT_B.u32 = pstIspReg->ISP_AWB_BOT_B.u32;
	ISP_AWB_BOT_B.bits.awb_bot_b = data;
	pstIspReg->ISP_AWB_BOT_B.u32 = ISP_AWB_BOT_B.u32;
}

/*****************************************************
ISP_AWB_AVG_R:        
0x205a2140  :                31:16     reserved
						 15:0     Normalized sum of Red component
*****************************************************/
static __inline HI_U16 hi_isp_metering_avg_r_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AWB_AVG_R ISP_AWB_AVG_R;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AWB_AVG_R.u32 = pstIspReg->ISP_AWB_AVG_R.u32;
	return ISP_AWB_AVG_R.bits.avg_r;
}

/*****************************************************
ISP_AWB_AVG_G:    
0x205a2144  :                31:16     reserved
						 15:0     Normalized sum of Green component
*****************************************************/
static __inline HI_U16 hi_isp_metering_avg_g_read(HI_U8 ispDev) {
    S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AWB_AVG_G ISP_AWB_AVG_G;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AWB_AVG_G.u32 = pstIspReg->ISP_AWB_AVG_G.u32;
	return ISP_AWB_AVG_G.bits.avg_g;
}

/*****************************************************
ISP_AWB_AVG_B:       
0x205a2148  :                31:16     reserved
						 15:0     Normalized sum of Blue component
*****************************************************/
static __inline HI_U16 hi_isp_metering_avg_b_read(HI_U8 ispDev) {
    S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AWB_AVG_B ISP_AWB_AVG_B;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AWB_AVG_B.u32 = pstIspReg->ISP_AWB_AVG_B.u32;
	return ISP_AWB_AVG_B.bits.avg_b;
}


/*****************************************************
ISP_AWB_CNT_ALL:     
0x205a214c  :                31:16     reserved
						 15:0     Normalized count of pixels which were averaged in Avg R, Avg G and Avg B
*****************************************************/
static __inline HI_U16 hi_isp_metering_count_all_read(HI_U8 ispDev) {
    S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AWB_CNT_ALL ISP_AWB_CNT_ALL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AWB_CNT_ALL.u32 = pstIspReg->ISP_AWB_CNT_ALL.u32;
	return ISP_AWB_CNT_ALL.bits.count_all;
}

/*****************************************************
ISP_AWB_STAT_RADDR:      AWB
0x205a2188  :                31:10   reserved
						 9:0      AWB  statistic address ,specific order as follows:
								0:block 0   AVG G + AVG R
								1:block 0   Count ALL + AVG B
								2:block 0   Count Max + Count Min
							      total 255 block, 
*****************************************************/
static __inline HI_VOID hi_isp_awb_stat_raddr_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AWB_STAT_RADDR ISP_AWB_STAT_RADDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AWB_STAT_RADDR.u32 = pstIspReg->ISP_AWB_STAT_RADDR.u32;
	ISP_AWB_STAT_RADDR.bits.stat_raddr = data;
	pstIspReg->ISP_AWB_STAT_RADDR.u32 = ISP_AWB_STAT_RADDR.u32;
}

/*****************************************************
ISP_AWB_STAT_RDATA:      AWB  statistics read data
0x205a218c  :                     31:0   
*****************************************************/
static __inline HI_U32 hi_isp_awb_stat_rdata_read(HI_U8 ispDev) {
    S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AWB_STAT_RDATA ISP_AWB_STAT_RDATA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AWB_STAT_RDATA.u32 = pstIspReg->ISP_AWB_STAT_RDATA.u32;
	return ISP_AWB_STAT_RDATA.bits.stat_rdata;
}

/*****************************************************
ISP_AWB_SIZE:      AWB  
0x205a21f0  :         31:28   reserved
					27:16  image hight
					15:12   reserved
					11:0    image width
*****************************************************/
static __inline HI_VOID hi_isp_awb_hsize_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AWB_SIZE ISP_AWB_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AWB_SIZE.u32 = pstIspReg->ISP_AWB_SIZE.u32;
	ISP_AWB_SIZE.bits.hsize = data;
	pstIspReg->ISP_AWB_SIZE.u32 = ISP_AWB_SIZE.u32;
}

static __inline HI_VOID hi_isp_awb_vsize_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AWB_SIZE ISP_AWB_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AWB_SIZE.u32 = pstIspReg->ISP_AWB_SIZE.u32;
	ISP_AWB_SIZE.bits.vsize = data;
	pstIspReg->ISP_AWB_SIZE.u32 = ISP_AWB_SIZE.u32;
}

/*****************************************************
ISP_WB_CFG:                     
0x205a1200:                31:1   reserved
                           0      WBenable
*****************************************************/
static __inline HI_VOID hi_isp_wb_enable_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WB_CFG ISP_WB_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WB_CFG.u32 = pstIspReg->ISP_WB_CFG.u32;
	ISP_WB_CFG.bits.en = data;
	pstIspReg->ISP_WB_CFG.u32 = ISP_WB_CFG.u32;
}


static __inline HI_U8 hi_isp_wb_enable_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WB_CFG ISP_WB_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WB_CFG.u32 = pstIspReg->ISP_WB_CFG.u32;
	return ISP_WB_CFG.bits.en ;
	
}

/*****************************************************

ISP_WB_BLC_CFG:            WB BLC offset     
0x113a1204:                31:02     reserved
                           01:01     en_out output WB BLCenable
                           00:00     en_in  input  WB BLCenable
*****************************************************/
static __inline HI_VOID hi_isp_wb_blc_in_enable_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WB_BLC_CFG ISP_WB_BLC_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WB_BLC_CFG.u32 = pstIspReg->ISP_WB_BLC_CFG.u32;
	ISP_WB_BLC_CFG.bits.en_in = data;
	pstIspReg->ISP_WB_BLC_CFG.u32 = ISP_WB_BLC_CFG.u32;
}

static __inline HI_VOID hi_isp_wb_blc_out_enable_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WB_BLC_CFG ISP_WB_BLC_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WB_BLC_CFG.u32 = pstIspReg->ISP_WB_BLC_CFG.u32;
	ISP_WB_BLC_CFG.bits.en_out = data;
	pstIspReg->ISP_WB_BLC_CFG.u32 = ISP_WB_BLC_CFG.u32;
}

static __inline HI_VOID hi_isp_white_balance_gain_r_write (HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WB_GAIN1 ISP_WB_GAIN1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WB_GAIN1.u32 = pstIspReg->ISP_WB_GAIN1.u32;
	ISP_WB_GAIN1.bits.rgain = data;
	pstIspReg->ISP_WB_GAIN1.u32 = ISP_WB_GAIN1.u32;
}

static __inline HI_U16 hi_isp_white_balance_gain_r_read (HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WB_GAIN1 ISP_WB_GAIN1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WB_GAIN1.u32 = pstIspReg->ISP_WB_GAIN1.u32;
	return ISP_WB_GAIN1.bits.rgain;
}

static __inline HI_VOID hi_isp_white_balance_gain_gr_write (HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WB_GAIN1 ISP_WB_GAIN1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WB_GAIN1.u32 = pstIspReg->ISP_WB_GAIN1.u32;
	ISP_WB_GAIN1.bits.grgain = data;
	pstIspReg->ISP_WB_GAIN1.u32 = ISP_WB_GAIN1.u32;
}

static __inline HI_U16 hi_isp_white_balance_gain_gr_read (HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WB_GAIN1 ISP_WB_GAIN1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WB_GAIN1.u32 = pstIspReg->ISP_WB_GAIN1.u32;
	return ISP_WB_GAIN1.bits.grgain;
}

static __inline HI_VOID hi_isp_white_balance_gain_gb_write (HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WB_GAIN2 ISP_WB_GAIN2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WB_GAIN2.u32 = pstIspReg->ISP_WB_GAIN2.u32;
	ISP_WB_GAIN2.bits.gbgain = data;
	pstIspReg->ISP_WB_GAIN2.u32 = ISP_WB_GAIN2.u32;
}

static __inline HI_U16 hi_isp_white_balance_gain_gb_read (HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WB_GAIN2 ISP_WB_GAIN2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WB_GAIN2.u32 = pstIspReg->ISP_WB_GAIN2.u32;
	return ISP_WB_GAIN2.bits.gbgain;
}

static __inline HI_VOID hi_isp_white_balance_gain_b_write (HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WB_GAIN2 ISP_WB_GAIN2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WB_GAIN2.u32 = pstIspReg->ISP_WB_GAIN2.u32;
	ISP_WB_GAIN2.bits.bgain = data;
	pstIspReg->ISP_WB_GAIN2.u32 = ISP_WB_GAIN2.u32;
}

static __inline HI_U16 hi_isp_white_balance_gain_b_read (HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WB_GAIN2 ISP_WB_GAIN2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WB_GAIN2.u32 = pstIspReg->ISP_WB_GAIN2.u32;
	return ISP_WB_GAIN2.bits.bgain;
}

/*****************************************************
ISP_WB_BLC_OFFSET1:             BLC offset_Gr
0x205a1218:                30:16     R offset
                           14:0     Gr offset
*****************************************************/
static __inline HI_VOID hi_isp_wb_blc_offset_Gr_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WB_BLC_OFFSET1 ISP_WB_BLC_OFFSET1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WB_BLC_OFFSET1.u32 = pstIspReg->ISP_WB_BLC_OFFSET1.u32;
	ISP_WB_BLC_OFFSET1.bits.ofsgr = data;
	pstIspReg->ISP_WB_BLC_OFFSET1.u32 = ISP_WB_BLC_OFFSET1.u32;
}

static __inline HI_U16 hi_isp_wb_blc_offset_Gr_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WB_BLC_OFFSET1 ISP_WB_BLC_OFFSET1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WB_BLC_OFFSET1.u32 = pstIspReg->ISP_WB_BLC_OFFSET1.u32;
	return ISP_WB_BLC_OFFSET1.bits.ofsgr;
}
static __inline HI_VOID hi_isp_wb_blc_offset_R_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WB_BLC_OFFSET1 ISP_WB_BLC_OFFSET1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WB_BLC_OFFSET1.u32 = pstIspReg->ISP_WB_BLC_OFFSET1.u32;
	ISP_WB_BLC_OFFSET1.bits.ofsr = data;
	pstIspReg->ISP_WB_BLC_OFFSET1.u32 = ISP_WB_BLC_OFFSET1.u32;
}

static __inline HI_U16 hi_isp_wb_blc_offset_R_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WB_BLC_OFFSET1 ISP_WB_BLC_OFFSET1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WB_BLC_OFFSET1.u32 = pstIspReg->ISP_WB_BLC_OFFSET1.u32;
	return ISP_WB_BLC_OFFSET1.bits.ofsr;
}
/*****************************************************

ISP_WB_BLC_OFFSET2:             BLC offset_Gr
0x205a121c:                30:16     B offset
                           14:0     Gb offset
*****************************************************/
static __inline HI_VOID hi_isp_wb_blc_offset_Gb_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WB_BLC_OFFSET2 ISP_WB_BLC_OFFSET2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WB_BLC_OFFSET2.u32 = pstIspReg->ISP_WB_BLC_OFFSET2.u32;
	ISP_WB_BLC_OFFSET2.bits.ofsgb = data;
	pstIspReg->ISP_WB_BLC_OFFSET2.u32 = ISP_WB_BLC_OFFSET2.u32;
}

static __inline HI_U16 hi_isp_wb_blc_offset_Gb_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WB_BLC_OFFSET2 ISP_WB_BLC_OFFSET2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WB_BLC_OFFSET2.u32 = pstIspReg->ISP_WB_BLC_OFFSET2.u32;
	return ISP_WB_BLC_OFFSET2.bits.ofsgb;
}
static __inline HI_VOID hi_isp_wb_blc_offset_B_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WB_BLC_OFFSET2 ISP_WB_BLC_OFFSET2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WB_BLC_OFFSET2.u32 = pstIspReg->ISP_WB_BLC_OFFSET2.u32;
	ISP_WB_BLC_OFFSET2.bits.ofsb = data;
	pstIspReg->ISP_WB_BLC_OFFSET2.u32 = ISP_WB_BLC_OFFSET2.u32;
}

static __inline HI_U16 hi_isp_wb_blc_offset_B_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WB_BLC_OFFSET2 ISP_WB_BLC_OFFSET2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WB_BLC_OFFSET2.u32 = pstIspReg->ISP_WB_BLC_OFFSET2.u32;
	return ISP_WB_BLC_OFFSET2.bits.ofsb;
}

/*****************************************************
MODULE        -----CC-----
MODULE        -----CC-----
MODULE        -----CC-----
*****************************************************/
/*****************************************************
ISP_CC_CFG:                     
0x205c1400:                31:02      reserved
                           00:01      prot_en dark area proctect enable
                           00:00      ccenable
*****************************************************/
static __inline HI_VOID hi_isp_cc_enable_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_CFG ISP_CC_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_CFG.u32 = pstIspReg->ISP_CC_CFG.u32;
	ISP_CC_CFG.bits.en = data;
	pstIspReg->ISP_CC_CFG.u32 = ISP_CC_CFG.u32;
}


static __inline HI_U8 hi_isp_cc_enable_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_CFG ISP_CC_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_CFG.u32 = pstIspReg->ISP_CC_CFG.u32;
	return ISP_CC_CFG.bits.en ;
	
}

static __inline HI_VOID hi_isp_cc_prot_en_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_CFG ISP_CC_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_CFG.u32 = pstIspReg->ISP_CC_CFG.u32;
	ISP_CC_CFG.bits.prot_en = data;
	pstIspReg->ISP_CC_CFG.u32 = ISP_CC_CFG.u32;
}

#define HI_ISP_CCM_COLORTONE_EN_DEFAULT (1)
static __inline HI_VOID hi_isp_cc_colortone_en_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_CFG ISP_CC_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_CFG.u32 = pstIspReg->ISP_CC_CFG.u32;
	ISP_CC_CFG.bits.colortone_en = data;
	pstIspReg->ISP_CC_CFG.u32 = ISP_CC_CFG.u32;
}

#define HI_ISP_CCM_PROT_EXT_EN_DEFAULT (1)
static __inline HI_VOID hi_isp_cc_prot_ext_en_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_CFG ISP_CC_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_CFG.u32 = pstIspReg->ISP_CC_CFG.u32;
	ISP_CC_CFG.bits.prot_ext_en = data;
	pstIspReg->ISP_CC_CFG.u32 = ISP_CC_CFG.u32;
}

#define CCM_CONVERT_PRE(value) ((value & 0x8000) | (value << 3))
#define CCM_CONVERT(value) ((value & 0x8000)?((~(value & 0x7FFF)) + 1):(value))

static __inline HI_VOID hi_isp_matrix_coefft_r_r_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_COEF0 ISP_CC_COEF0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_COEF0.u32 = pstIspReg->ISP_CC_COEF0.u32;
	ISP_CC_COEF0.bits.coef00 = data;
	pstIspReg->ISP_CC_COEF0.u32 = ISP_CC_COEF0.u32;
}

static __inline HI_U16 hi_isp_matrix_coefft_r_r_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_COEF0 ISP_CC_COEF0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_COEF0.u32 = pstIspReg->ISP_CC_COEF0.u32;
	return ISP_CC_COEF0.bits.coef00;
}

static __inline HI_VOID hi_isp_matrix_coefft_r_g_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_COEF0 ISP_CC_COEF0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_COEF0.u32 = pstIspReg->ISP_CC_COEF0.u32;
	ISP_CC_COEF0.bits.coef01 = data;
	pstIspReg->ISP_CC_COEF0.u32 = ISP_CC_COEF0.u32;
}

static __inline HI_U16 hi_isp_matrix_coefft_r_g_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_COEF0 ISP_CC_COEF0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_COEF0.u32 = pstIspReg->ISP_CC_COEF0.u32;
	return ISP_CC_COEF0.bits.coef01;
}

static __inline HI_VOID hi_isp_matrix_coefft_r_b_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_COEF1 ISP_CC_COEF1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_COEF1.u32 = pstIspReg->ISP_CC_COEF1.u32;
	ISP_CC_COEF1.bits.coef02 = data;
	pstIspReg->ISP_CC_COEF1.u32 = ISP_CC_COEF1.u32;
}

static __inline HI_U16 hi_isp_matrix_coefft_r_b_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_COEF1 ISP_CC_COEF1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_COEF1.u32 = pstIspReg->ISP_CC_COEF1.u32;
	return ISP_CC_COEF1.bits.coef02;
}

static __inline HI_VOID hi_isp_matrix_coefft_g_r_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_COEF1 ISP_CC_COEF1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_COEF1.u32 = pstIspReg->ISP_CC_COEF1.u32;
	ISP_CC_COEF1.bits.coef10 = data;
	pstIspReg->ISP_CC_COEF1.u32 = ISP_CC_COEF1.u32;
}

static __inline HI_U16 hi_isp_matrix_coefft_g_r_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_COEF1 ISP_CC_COEF1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_COEF1.u32 = pstIspReg->ISP_CC_COEF1.u32;
	return ISP_CC_COEF1.bits.coef10;
}

static __inline HI_VOID hi_isp_matrix_coefft_g_g_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_COEF2 ISP_CC_COEF2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_COEF2.u32 = pstIspReg->ISP_CC_COEF2.u32;
	ISP_CC_COEF2.bits.coef11 = data;	
	pstIspReg->ISP_CC_COEF2.u32 = ISP_CC_COEF2.u32;
}

static __inline HI_U16 hi_isp_matrix_coefft_g_g_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_COEF2 ISP_CC_COEF2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_COEF2.u32 = pstIspReg->ISP_CC_COEF2.u32;
	return ISP_CC_COEF2.bits.coef11;	
}

static __inline HI_VOID hi_isp_matrix_coefft_g_b_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_COEF2 ISP_CC_COEF2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_COEF2.u32 = pstIspReg->ISP_CC_COEF2.u32;
	ISP_CC_COEF2.bits.coef12 = data;		
	pstIspReg->ISP_CC_COEF2.u32 = ISP_CC_COEF2.u32;
}

static __inline HI_U16 hi_isp_matrix_coefft_g_b_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_COEF2 ISP_CC_COEF2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_COEF2.u32 = pstIspReg->ISP_CC_COEF2.u32;
	return ISP_CC_COEF2.bits.coef12;		
}

static __inline HI_VOID hi_isp_matrix_coefft_b_r_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_COEF3 ISP_CC_COEF3;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_COEF3.u32 = pstIspReg->ISP_CC_COEF3.u32;
	ISP_CC_COEF3.bits.coef20 = data;		
	pstIspReg->ISP_CC_COEF3.u32 = ISP_CC_COEF3.u32;
}

static __inline HI_U16 hi_isp_matrix_coefft_b_r_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_COEF3 ISP_CC_COEF3;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_COEF3.u32 = pstIspReg->ISP_CC_COEF3.u32;
	return ISP_CC_COEF3.bits.coef20;		
}

static __inline HI_VOID hi_isp_matrix_coefft_b_g_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_COEF3 ISP_CC_COEF3;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_COEF3.u32 = pstIspReg->ISP_CC_COEF3.u32;
	ISP_CC_COEF3.bits.coef21 = data;		
	pstIspReg->ISP_CC_COEF3.u32 = ISP_CC_COEF3.u32;
}

static __inline HI_U16 hi_isp_matrix_coefft_b_g_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_COEF3 ISP_CC_COEF3;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_COEF3.u32 = pstIspReg->ISP_CC_COEF3.u32;
	return ISP_CC_COEF3.bits.coef21;		
}

static __inline HI_VOID hi_isp_matrix_coefft_b_b_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_COEF4 ISP_CC_COEF4;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_COEF4.u32 = pstIspReg->ISP_CC_COEF4.u32;
	ISP_CC_COEF4.bits.coef22 = data;		
	pstIspReg->ISP_CC_COEF4.u32 = ISP_CC_COEF4.u32;
}

static __inline HI_U16 hi_isp_matrix_coefft_b_b_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_COEF4 ISP_CC_COEF4;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_COEF4.u32 = pstIspReg->ISP_CC_COEF4.u32;
	return ISP_CC_COEF4.bits.coef22;		
}


/*****************************************************
ISP_CC_BRIT_THRES:                     
0x113a1448:                31:02      reserved
                           24:16      brit_thres1
                           07:00      brit_thres0
*****************************************************/
static __inline HI_VOID hi_isp_cc_brit_thres0_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_BRIT_THRES ISP_CC_BRIT_THRES;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_BRIT_THRES.u32 = pstIspReg->ISP_CC_BRIT_THRES.u32;
	ISP_CC_BRIT_THRES.bits.brit_thres0 = data;
	pstIspReg->ISP_CC_BRIT_THRES.u32 = ISP_CC_BRIT_THRES.u32;
}

static __inline HI_VOID hi_isp_cc_brit_thres1_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_BRIT_THRES ISP_CC_BRIT_THRES;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_BRIT_THRES.u32 = pstIspReg->ISP_CC_BRIT_THRES.u32;
	ISP_CC_BRIT_THRES.bits.brit_thres1 = data;
	pstIspReg->ISP_CC_BRIT_THRES.u32 = ISP_CC_BRIT_THRES.u32;
}

/*****************************************************
ISP_CC_COLOR_THRES:                     
0x113a144c:                31:02      reserved
                           24:16      color_thres1
                           07:00      color_thres0
*****************************************************/
static __inline HI_VOID hi_isp_cc_color_thres0_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_COLOR_THRES ISP_CC_COLOR_THRES;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_COLOR_THRES.u32 = pstIspReg->ISP_CC_COLOR_THRES.u32;
	ISP_CC_COLOR_THRES.bits.color_thres0 = data;
	pstIspReg->ISP_CC_COLOR_THRES.u32 = ISP_CC_COLOR_THRES.u32;
}

static __inline HI_VOID hi_isp_cc_color_thres1_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_COLOR_THRES ISP_CC_COLOR_THRES;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_COLOR_THRES.u32 = pstIspReg->ISP_CC_COLOR_THRES.u32;
	ISP_CC_COLOR_THRES.bits.color_thres1 = data;
	pstIspReg->ISP_CC_COLOR_THRES.u32 = ISP_CC_COLOR_THRES.u32;
}

/*****************************************************
ISP_CC_LOW_RATIO:                     
0x113a1450:                31:08      reserved
                           07:00      low_ratio
*****************************************************/
static __inline HI_VOID hi_isp_cc_low_ratio_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_LOW_RATIO ISP_CC_LOW_RATIO;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_LOW_RATIO.u32 = pstIspReg->ISP_CC_LOW_RATIO.u32;
	ISP_CC_LOW_RATIO.bits.low_ratio = data;
	pstIspReg->ISP_CC_LOW_RATIO.u32 = ISP_CC_LOW_RATIO.u32;
}

/*****************************************************
ISP_CC_PRO_RATIO:                     
0x113a1454:                31:28      reserved
                           27:16      brit_ratio
                           11:00      color_ratio
*****************************************************/
static __inline HI_VOID hi_isp_cc_color_ratio_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_PRO_RATIO ISP_CC_PRO_RATIO;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_PRO_RATIO.u32 = pstIspReg->ISP_CC_PRO_RATIO.u32;
	ISP_CC_PRO_RATIO.bits.color_ratio = data;
	pstIspReg->ISP_CC_PRO_RATIO.u32 = ISP_CC_PRO_RATIO.u32;
}

static __inline HI_VOID hi_isp_cc_brit_ratio_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_PRO_RATIO ISP_CC_PRO_RATIO;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_PRO_RATIO.u32 = pstIspReg->ISP_CC_PRO_RATIO.u32;
	ISP_CC_PRO_RATIO.bits.brit_ratio = data;
	pstIspReg->ISP_CC_PRO_RATIO.u32 = ISP_CC_PRO_RATIO.u32;
}

/*****************************************************
U_ISP_CC_COLORTONE_RB_GAIN:                     
0x113a1454:                31:28      reserved

b_gain                : 12 
r_gain                : 12 
*****************************************************/
#define HI_ISP_CCM_COLORTONE_RGAIN_DEFAULT (256)
#define HI_ISP_CCM_COLORTONE_BGAIN_DEFAULT (256)
#define HI_ISP_CCM_COLORTONE_GGAIN_DEFAULT (256)
static __inline HI_VOID hi_isp_cc_colortone_rgain_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_COLORTONE_RB_GAIN ISP_CC_COLORTONE_RB_GAIN;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_COLORTONE_RB_GAIN.u32 = pstIspReg->ISP_CC_COLORTONE_RB_GAIN.u32;
	ISP_CC_COLORTONE_RB_GAIN.bits.r_gain = data;
	pstIspReg->ISP_CC_COLORTONE_RB_GAIN.u32 = ISP_CC_COLORTONE_RB_GAIN.u32;
}
static __inline HI_U16 hi_isp_cc_colortone_rgain_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_COLORTONE_RB_GAIN ISP_CC_COLORTONE_RB_GAIN;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_COLORTONE_RB_GAIN.u32 = pstIspReg->ISP_CC_COLORTONE_RB_GAIN.u32;
	return (ISP_CC_COLORTONE_RB_GAIN.bits.r_gain);
}

static __inline HI_VOID hi_isp_cc_colortone_bgain_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_COLORTONE_RB_GAIN ISP_CC_COLORTONE_RB_GAIN;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_COLORTONE_RB_GAIN.u32 = pstIspReg->ISP_CC_COLORTONE_RB_GAIN.u32;
	ISP_CC_COLORTONE_RB_GAIN.bits.b_gain = data;
	pstIspReg->ISP_CC_COLORTONE_RB_GAIN.u32 = ISP_CC_COLORTONE_RB_GAIN.u32;
}
static __inline HI_U16 hi_isp_cc_colortone_bgain_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_COLORTONE_RB_GAIN ISP_CC_COLORTONE_RB_GAIN;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_COLORTONE_RB_GAIN.u32 = pstIspReg->ISP_CC_COLORTONE_RB_GAIN.u32;
	return (ISP_CC_COLORTONE_RB_GAIN.bits.b_gain);
}

static __inline HI_VOID hi_isp_cc_colortone_ggain_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_COLORTONE_G_GAIN ISP_CC_COLORTONE_G_GAIN;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_COLORTONE_G_GAIN.u32 = pstIspReg->ISP_CC_COLORTONE_G_GAIN.u32;
	ISP_CC_COLORTONE_G_GAIN.bits.g_gain = data;
	pstIspReg->ISP_CC_COLORTONE_G_GAIN.u32 = ISP_CC_COLORTONE_G_GAIN.u32;
}
static __inline HI_U16 hi_isp_cc_colortone_ggain_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_COLORTONE_G_GAIN ISP_CC_COLORTONE_G_GAIN;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_COLORTONE_G_GAIN.u32 = pstIspReg->ISP_CC_COLORTONE_G_GAIN.u32;
	return (ISP_CC_COLORTONE_G_GAIN.bits.g_gain);
}

#define HI_ISP_CCM_RR_THD0_DEFAULT (30)
static __inline HI_VOID hi_isp_cc_data_rr_thd0_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_PROT_EX_TH0 ISP_CC_PROT_EX_TH0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_PROT_EX_TH0.u32 = pstIspReg->ISP_CC_PROT_EX_TH0.u32;
	ISP_CC_PROT_EX_TH0.bits.data_rr_thd0 = data;
	pstIspReg->ISP_CC_PROT_EX_TH0.u32 = ISP_CC_PROT_EX_TH0.u32;
}

#define HI_ISP_CCM_GG_THD0_DEFAULT (6)
static __inline HI_VOID hi_isp_cc_data_gg_thd0_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_PROT_EX_TH0 ISP_CC_PROT_EX_TH0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_PROT_EX_TH0.u32 = pstIspReg->ISP_CC_PROT_EX_TH0.u32;
	ISP_CC_PROT_EX_TH0.bits.data_gg_thd0 = data;
	pstIspReg->ISP_CC_PROT_EX_TH0.u32 = ISP_CC_PROT_EX_TH0.u32;
}

#define HI_ISP_CCM_BB_THD0_DEFAULT (30)
static __inline HI_VOID hi_isp_cc_data_bb_thd0_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_PROT_EX_TH0 ISP_CC_PROT_EX_TH0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_PROT_EX_TH0.u32 = pstIspReg->ISP_CC_PROT_EX_TH0.u32;
	ISP_CC_PROT_EX_TH0.bits.data_bb_thd0 = data;
	pstIspReg->ISP_CC_PROT_EX_TH0.u32 = ISP_CC_PROT_EX_TH0.u32;
}

#define HI_ISP_CCM_RR_THD1_DEFAULT (80)
static __inline HI_VOID hi_isp_cc_data_rr_thd1_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_PROT_EX_TH1 ISP_CC_PROT_EX_TH1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_PROT_EX_TH1.u32 = pstIspReg->ISP_CC_PROT_EX_TH1.u32;
	ISP_CC_PROT_EX_TH1.bits.data_rr_thd1 = data;
	pstIspReg->ISP_CC_PROT_EX_TH1.u32 = ISP_CC_PROT_EX_TH1.u32;
}

#define HI_ISP_CCM_GG_THD1_DEFAULT (20)
static __inline HI_VOID hi_isp_cc_data_gg_thd1_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_PROT_EX_TH1 ISP_CC_PROT_EX_TH1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_PROT_EX_TH1.u32 = pstIspReg->ISP_CC_PROT_EX_TH1.u32;
	ISP_CC_PROT_EX_TH1.bits.data_gg_thd1 = data;
	pstIspReg->ISP_CC_PROT_EX_TH1.u32 = ISP_CC_PROT_EX_TH1.u32;
}

#define HI_ISP_CCM_BB_THD1_DEFAULT (80)
static __inline HI_VOID hi_isp_cc_data_bb_thd1_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_PROT_EX_TH1 ISP_CC_PROT_EX_TH1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_PROT_EX_TH1.u32 = pstIspReg->ISP_CC_PROT_EX_TH1.u32;
	ISP_CC_PROT_EX_TH1.bits.data_bb_thd1 = data;
	pstIspReg->ISP_CC_PROT_EX_TH1.u32 = ISP_CC_PROT_EX_TH1.u32;
}

#define HI_ISP_CCM_RGB_PROT_RATIO_DEFAULT (140)
static __inline HI_VOID hi_isp_cc_rgb_prot_ration_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_PROT_EX_RGB_SET ISP_CC_PROT_EX_RGB_SET;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_PROT_EX_RGB_SET.u32 = pstIspReg->ISP_CC_PROT_EX_RGB_SET.u32;
	ISP_CC_PROT_EX_RGB_SET.bits.rgb_prot_ration = data;
	pstIspReg->ISP_CC_PROT_EX_RGB_SET.u32 = ISP_CC_PROT_EX_RGB_SET.u32;
}

static __inline HI_VOID hi_isp_cc_max_rgb_thd_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CC_PROT_EX_RGB_SET ISP_CC_PROT_EX_RGB_SET;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CC_PROT_EX_RGB_SET.u32 = pstIspReg->ISP_CC_PROT_EX_RGB_SET.u32;
	ISP_CC_PROT_EX_RGB_SET.bits.max_rgb_thd = data;
	pstIspReg->ISP_CC_PROT_EX_RGB_SET.u32 = ISP_CC_PROT_EX_RGB_SET.u32;
}



/*****************************************************
MODULE        -----MG-----
*****************************************************/

/*****************************************************
ISP_MG_CFG:         MG control
0x113A2500:          31:1    reserved
*****************************************************/

static __inline HI_VOID hi_isp_mg_enable_write(HI_U8 ispDev, HI_U8 data) {
    S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_MG_CFG ISP_MG_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_MG_CFG.u32 = pstIspReg->ISP_MG_CFG.u32;
	ISP_MG_CFG.bits.en = data;
	pstIspReg->ISP_MG_CFG.u32 = ISP_MG_CFG.u32;
}

static __inline HI_U8 hi_isp_mg_enable_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_MG_CFG ISP_MG_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_MG_CFG.u32 = pstIspReg->ISP_MG_CFG.u32;
	return ISP_MG_CFG.bits.en;
}

/*****************************************************
ISP_MG_ZONE:         	MG
0x113a250c:       		31:13      reserved
                           	12:  8     vertical window number
                           	7 :  5      reserved
                           	4 :  0      horizontal window number
*****************************************************/
static __inline HI_VOID hi_isp_mg_h_zone_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_MG_ZONE ISP_MG_ZONE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_MG_ZONE.u32 = pstIspReg->ISP_MG_ZONE.u32;
	ISP_MG_ZONE.bits.hnum = data;
	pstIspReg->ISP_MG_ZONE.u32 = ISP_MG_ZONE.u32;
}

static __inline HI_VOID hi_isp_mg_v_zone_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_MG_ZONE ISP_MG_ZONE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_MG_ZONE.u32 = pstIspReg->ISP_MG_ZONE.u32;
	ISP_MG_ZONE.bits.vnum = data;
	pstIspReg->ISP_MG_ZONE.u32 = ISP_MG_ZONE.u32;
}


/*****************************************************
ISP_MG_BITMOVE:     
0x113a2540: 		31 :  10      	reserved  
		     			9	     	   	blc_en
		     			8	           	gamma_en
		     			7   :  0             
*****************************************************/
static __inline HI_VOID hi_isp_mg_blc_en_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_MG_BITMOVE ISP_MG_BITMOVE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_MG_BITMOVE.u32 = pstIspReg->ISP_MG_BITMOVE.u32;
	ISP_MG_BITMOVE.bits.blc_en = data;
	pstIspReg->ISP_MG_BITMOVE.u32 = ISP_MG_BITMOVE.u32;
}

static __inline HI_VOID hi_isp_mg_gamma_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_MG_BITMOVE ISP_MG_BITMOVE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_MG_BITMOVE.u32 = pstIspReg->ISP_MG_BITMOVE.u32;
	ISP_MG_BITMOVE.bits.gamma_en = data;
	pstIspReg->ISP_MG_BITMOVE.u32 = ISP_MG_BITMOVE.u32;
}

static __inline HI_U8 hi_isp_mg_gamma_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_MG_BITMOVE ISP_MG_BITMOVE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_MG_BITMOVE.u32 = pstIspReg->ISP_MG_BITMOVE.u32;

	return (ISP_MG_BITMOVE.bits.gamma_en);
}

static __inline HI_VOID hi_isp_mg_bit_move_gamma_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_MG_BITMOVE ISP_MG_BITMOVE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_MG_BITMOVE.u32 = pstIspReg->ISP_MG_BITMOVE.u32;
	ISP_MG_BITMOVE.bits.bitmove = data;
	pstIspReg->ISP_MG_BITMOVE.u32 = ISP_MG_BITMOVE.u32;
}

/*****************************************************
ISP_MG_OFFSET_R:     	MG BLC
0x113a2544:          		31:15  	reserved
					 	14:0	offset_r
*****************************************************/
static __inline HI_VOID hi_isp_mg_offset_r_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_MG_OFFSET_R ISP_MG_OFFSET_R;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_MG_OFFSET_R.u32 = pstIspReg->ISP_MG_OFFSET_R.u32;
	ISP_MG_OFFSET_R.bits.offset_r = data;
	pstIspReg->ISP_MG_OFFSET_R.u32 = ISP_MG_OFFSET_R.u32;
}

/*****************************************************
ISP_MG_OFFSET_Gr:     	MG BLC
0x113a2548:     			31:15   	reserved
						14:0	offset_gr
*****************************************************/
static __inline HI_VOID hi_isp_mg_offset_gr_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_MG_OFFSET_GR ISP_MG_OFFSET_GR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_MG_OFFSET_GR.u32 = pstIspReg->ISP_MG_OFFSET_GR.u32;
	ISP_MG_OFFSET_GR.bits.offset_gr = data;
	pstIspReg->ISP_MG_OFFSET_GR.u32 = ISP_MG_OFFSET_GR.u32;
}

/*****************************************************
ISP_MG_OFFSET_Gb     	MG BLC
0x113a254c:     			31:15   	reserved
						14:0	offset_gb
*****************************************************/
static __inline HI_VOID hi_isp_mg_offset_gb_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_MG_OFFSET_GB ISP_MG_OFFSET_GB;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_MG_OFFSET_GB.u32 = pstIspReg->ISP_MG_OFFSET_GB.u32;
	ISP_MG_OFFSET_GB.bits.offset_gb = data;
	pstIspReg->ISP_MG_OFFSET_GB.u32 = ISP_MG_OFFSET_GB.u32;
}

/*****************************************************
ISP_MG_OFFSET_B     	MG BLC
0x113a2550:     			31:15   	reserved
						14:0	offset_b
*****************************************************/
static __inline HI_VOID hi_isp_mg_offset_b_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_MG_OFFSET_B ISP_MG_OFFSET_B;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_MG_OFFSET_B.u32 = pstIspReg->ISP_MG_OFFSET_B.u32;
	ISP_MG_OFFSET_B.bits.offset_b = data;
	pstIspReg->ISP_MG_OFFSET_B.u32 = ISP_MG_OFFSET_B.u32;
}

/*****************************************************
ISP_MG_GAMMA_LIMIT     	MG BLC
0x113a2560:          		31:4   	reserved
						3:0		gamma_limit
*****************************************************/
static __inline HI_VOID hi_isp_mg_gamma_limit_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_MG_GAMMA_LIMIT ISP_MG_GAMMA_LIMIT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_MG_GAMMA_LIMIT.u32 = pstIspReg->ISP_MG_GAMMA_LIMIT.u32;
	ISP_MG_GAMMA_LIMIT.bits.gamma_limit = data;	
	pstIspReg->ISP_MG_GAMMA_LIMIT.u32 = ISP_MG_GAMMA_LIMIT.u32;
}

/*****************************************************
ISP_MG_MEM_AVER_RADDR:
0x113a2598:                	31:0                                                          
*****************************************************/
static __inline HI_VOID hi_isp_mg_mem_aver_raddr_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_MG_MEM_AVER_RADDR ISP_MG_MEM_AVER_RADDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_MG_MEM_AVER_RADDR.u32 = data;
	pstIspReg->ISP_MG_MEM_AVER_RADDR.u32 = ISP_MG_MEM_AVER_RADDR.u32;
}

/*****************************************************
ISP_MG_MEM_AVER_RDATA:     MG
0x113a259c:     			31:0
*****************************************************/
static __inline HI_U32 hi_isp_mg_mem_aver_rdata_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_MG_MEM_AVER_RDATA ISP_MG_MEM_AVER_RDATA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_MG_MEM_AVER_RDATA.u32 = pstIspReg->ISP_MG_MEM_AVER_RDATA.u32;
	return ISP_MG_MEM_AVER_RDATA.u32;
}

/*****************************************************
ISP_MG_CTRL_F                        :     MG    
0x205a25e0                             :     31:2   reserved  

*****************************************************/
static __inline HI_U8 hi_isp_mg_ctrl_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_MG_CTRL_F ISP_MG_CTRL_F;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_MG_CTRL_F.u32 = pstIspReg->ISP_MG_CTRL_F.u32;
	
	return ISP_MG_CTRL_F.bits.rggb_cfg;
}

/*****************************************************
ISP_MG_SIZE                            :     MG  
0x205a25f0                             :     31:28   reserved  
								    27:16   image hight
								    15:12   reserved
								    11:  0   image width
*****************************************************/
static __inline HI_VOID hi_isp_mg_width_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_MG_SIZE ISP_MG_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_MG_SIZE.u32 = pstIspReg->ISP_MG_SIZE.u32;
	ISP_MG_SIZE.bits.hsize = data;
	pstIspReg->ISP_MG_SIZE.u32 = ISP_MG_SIZE.u32;
}

static __inline HI_VOID hi_isp_mg_height_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_MG_SIZE ISP_MG_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_MG_SIZE.u32 = pstIspReg->ISP_MG_SIZE.u32;
	ISP_MG_SIZE.bits.vsize = data;
	pstIspReg->ISP_MG_SIZE.u32 = ISP_MG_SIZE.u32;
}




/*****************************************************
MODULE        -----CAC-----
MODULE        -----CAC-----
MODULE        -----CAC-----
*****************************************************/
/*****************************************************
MODULE        -----Global Cac-----
*****************************************************/
/*****************************************************
MODULE        -----Global Cac-----
*****************************************************/
/*****************************************************
ISP_GCAC_CFG:         Global CAC control
0x113c3200:           31:1    reserved
*****************************************************/
static __inline HI_VOID hi_isp_gcac_enable_write(HI_U8 ispDev, HI_U8 data) {
    S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GCAC_CFG ISP_GCAC_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GCAC_CFG.u32 = pstIspReg->ISP_GCAC_CFG.u32;
	ISP_GCAC_CFG.bits.gcac_en = data;
	pstIspReg->ISP_GCAC_CFG.u32 = ISP_GCAC_CFG.u32;
}

static __inline HI_U8 hi_isp_gcac_enable_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GCAC_CFG ISP_GCAC_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GCAC_CFG.u32 = pstIspReg->ISP_GCAC_CFG.u32;
	return ISP_GCAC_CFG.bits.gcac_en;
}


/*****************************************************
ISP_GCAC_PARAMA:   Global CAC 
0x113c3210:         31:25   reserved
				    24:16	blue coefficent   range: -256~255
					15:9    reserved
					8: 0    red coefficent 
*****************************************************/
#define HI_ISP_GCAC_PARAM_BLUE_A_DEFAULT  (3)
static __inline HI_VOID hi_isp_gcac_param_blue_A_write(HI_U8 ispDev, HI_U16 data) {
    S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GCAC_PARAMA ISP_GCAC_PARAMA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GCAC_PARAMA.u32 = pstIspReg->ISP_GCAC_PARAMA.u32;
	ISP_GCAC_PARAMA.bits.blue_a = data;
	pstIspReg->ISP_GCAC_PARAMA.u32 = ISP_GCAC_PARAMA.u32;
}

static __inline HI_S16 hi_isp_gcac_param_blue_A_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GCAC_PARAMA ISP_GCAC_PARAMA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GCAC_PARAMA.u32 = pstIspReg->ISP_GCAC_PARAMA.u32;
	return ISP_GCAC_PARAMA.bits.blue_a;
}


#define HI_ISP_GCAC_PARAM_RED_A_DEFAULT  (3)
static __inline HI_VOID hi_isp_gcac_param_red_A_write(HI_U8 ispDev, HI_U16 data) {
    S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GCAC_PARAMA ISP_GCAC_PARAMA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GCAC_PARAMA.u32 = pstIspReg->ISP_GCAC_PARAMA.u32;
	ISP_GCAC_PARAMA.bits.red_a = data;
	pstIspReg->ISP_GCAC_PARAMA.u32 = ISP_GCAC_PARAMA.u32;
}

static __inline HI_S16 hi_isp_gcac_param_red_A_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GCAC_PARAMA ISP_GCAC_PARAMA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GCAC_PARAMA.u32 = pstIspReg->ISP_GCAC_PARAMA.u32;
	return ISP_GCAC_PARAMA.bits.red_a;
}


/*****************************************************
ISP_GCAC_PARAMB:   Global CAC componet b
0x113c3214:         31:25   reserved
				  
					15:9    reserved
					8: 0    red coefficent
*****************************************************/
#define HI_ISP_GCAC_PARAM_BLUE_B_DEFAULT  (-16)
static __inline HI_VOID hi_isp_gcac_param_blue_B_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GCAC_PARAMB ISP_GCAC_PARAMB;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GCAC_PARAMB.u32 = pstIspReg->ISP_GCAC_PARAMB.u32;
	ISP_GCAC_PARAMB.bits.blue_b = data;
	pstIspReg->ISP_GCAC_PARAMB.u32 = ISP_GCAC_PARAMB.u32;
}
static __inline HI_S16 hi_isp_gcac_param_blue_B_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GCAC_PARAMB ISP_GCAC_PARAMB;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GCAC_PARAMB.u32 = pstIspReg->ISP_GCAC_PARAMB.u32;
	return ISP_GCAC_PARAMB.bits.blue_b;
}

#define HI_ISP_GCAC_PARAM_RED_B_DEFAULT  (-10)
static __inline HI_VOID hi_isp_gcac_param_red_B_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GCAC_PARAMB ISP_GCAC_PARAMB;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GCAC_PARAMB.u32 = pstIspReg->ISP_GCAC_PARAMB.u32;
	ISP_GCAC_PARAMB.bits.red_b = data;
	pstIspReg->ISP_GCAC_PARAMB.u32 = ISP_GCAC_PARAMB.u32;
}

static __inline HI_S16 hi_isp_gcac_param_red_B_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GCAC_PARAMB ISP_GCAC_PARAMB;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GCAC_PARAMB.u32 = pstIspReg->ISP_GCAC_PARAMB.u32;
	return ISP_GCAC_PARAMB.bits.red_b;
}


/*****************************************************
ISP_GCAC_PARAMC:   Global CAC 
0x113c3218:         31:25   reserved
				    24:16  blue
					15:9    reserved
					8: 0    red 
*****************************************************/
#define HI_ISP_GCAC_PARAM_BLUE_C_DEFAULT  (7)
static __inline HI_VOID hi_isp_gcac_param_blue_C_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GCAC_PARAMC ISP_GCAC_PARAMC;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GCAC_PARAMC.u32 = pstIspReg->ISP_GCAC_PARAMC.u32;
	ISP_GCAC_PARAMC.bits.blue_c = data;
	pstIspReg->ISP_GCAC_PARAMC.u32 = ISP_GCAC_PARAMC.u32;
}

static __inline HI_S16 hi_isp_gcac_param_blue_C_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GCAC_PARAMC ISP_GCAC_PARAMC;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GCAC_PARAMC.u32 = pstIspReg->ISP_GCAC_PARAMC.u32;
	return ISP_GCAC_PARAMC.bits.blue_c;
}


#define HI_ISP_GCAC_PARAM_RED_C_DEFAULT  (2)
static __inline HI_VOID hi_isp_gcac_param_red_C_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GCAC_PARAMC ISP_GCAC_PARAMC;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GCAC_PARAMC.u32 = pstIspReg->ISP_GCAC_PARAMC.u32;
	ISP_GCAC_PARAMC.bits.red_c = data;
	pstIspReg->ISP_GCAC_PARAMC.u32 = ISP_GCAC_PARAMC.u32;
}

static __inline HI_S16 hi_isp_gcac_param_red_C_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GCAC_PARAMC ISP_GCAC_PARAMC;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GCAC_PARAMC.u32 = pstIspReg->ISP_GCAC_PARAMC.u32;
	return ISP_GCAC_PARAMC.bits.red_c;
}


/*****************************************************
ISP_GCAC_YNORM:   global cac
0x113c321C:         31:19   reserved
				    18:16	 (y_ns)
					15:5    reserved
					4: 0    (y_nf)
*****************************************************/
#define HI_ISP_GCAC_Y_NORM_SHIFT_DEFAULT (6)
static __inline HI_VOID hi_isp_gcac_y_ns_norm_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GCAC_YNORM ISP_GCAC_YNORM;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GCAC_YNORM.u32 = pstIspReg->ISP_GCAC_YNORM.u32;
	ISP_GCAC_YNORM.bits.ns_y = data;
	pstIspReg->ISP_GCAC_YNORM.u32 = ISP_GCAC_YNORM.u32;
}

static __inline HI_U8 hi_isp_gcac_y_ns_norm_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GCAC_YNORM ISP_GCAC_YNORM;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GCAC_YNORM.u32 = pstIspReg->ISP_GCAC_YNORM.u32;
	return ISP_GCAC_YNORM.bits.ns_y;
}


#define HI_ISP_GCAC_Y_NORM_FACTOR_DEFAULT  (29)
static __inline HI_VOID hi_isp_gcac_y_nf_norm_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GCAC_YNORM ISP_GCAC_YNORM;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GCAC_YNORM.u32 = pstIspReg->ISP_GCAC_YNORM.u32;
	ISP_GCAC_YNORM.bits.nf_y = data;
	pstIspReg->ISP_GCAC_YNORM.u32 = ISP_GCAC_YNORM.u32;
}

static __inline HI_U8 hi_isp_gcac_y_nf_norm_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GCAC_YNORM ISP_GCAC_YNORM;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GCAC_YNORM.u32 = pstIspReg->ISP_GCAC_YNORM.u32;
	return ISP_GCAC_YNORM.bits.nf_y;
}

/*****************************************************
ISP_GCAC_XNORM:   global cac
0x113c3220:        31:19    reserved
				   18:16	(x_ns)
					15:5    reserved
					4: 0   (x_nf)
*****************************************************/
#define HI_ISP_GCAC_X_NORM_SHIFT_DEFAULT  (6)
static __inline HI_VOID hi_isp_gcac_x_ns_norm_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GCAC_XNORM ISP_GCAC_XNORM;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GCAC_XNORM.u32 = pstIspReg->ISP_GCAC_XNORM.u32;
	ISP_GCAC_XNORM.bits.ns_x = data;
	pstIspReg->ISP_GCAC_XNORM.u32 = ISP_GCAC_XNORM.u32;
}
static __inline HI_U8 hi_isp_gcac_x_ns_norm_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GCAC_XNORM ISP_GCAC_XNORM;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GCAC_XNORM.u32 = pstIspReg->ISP_GCAC_XNORM.u32;
	return ISP_GCAC_XNORM.bits.ns_x;
}

#define HI_ISP_GCAC_X_NORM_FACTOR_DEFAULT  (29)
static __inline HI_VOID hi_isp_gcac_x_nf_norm_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GCAC_XNORM ISP_GCAC_XNORM;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GCAC_XNORM.u32 = pstIspReg->ISP_GCAC_XNORM.u32;
	ISP_GCAC_XNORM.bits.nf_x = data;
	pstIspReg->ISP_GCAC_XNORM.u32 = ISP_GCAC_XNORM.u32;
}
static __inline HI_U8 hi_isp_gcac_x_nf_norm_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GCAC_XNORM ISP_GCAC_XNORM;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GCAC_XNORM.u32 = pstIspReg->ISP_GCAC_XNORM.u32;
	return ISP_GCAC_XNORM.bits.nf_x;
}


/*****************************************************
ISP_GCAC_CNTSTART:   global cac 
0x113c3224:         31:29   reserved
				    28:16	global cac 
					15:13   reserved
					12: 0   global cac 
*****************************************************/
static __inline HI_VOID hi_isp_gcac_cnt_start_ver_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GCAC_CNTSTART ISP_GCAC_CNTSTART;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GCAC_CNTSTART.u32 = pstIspReg->ISP_GCAC_CNTSTART.u32;
	ISP_GCAC_CNTSTART.bits.cnt_start_v = data;
	pstIspReg->ISP_GCAC_CNTSTART.u32 = ISP_GCAC_CNTSTART.u32;
}

static __inline HI_U16 hi_isp_gcac_cnt_start_ver_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GCAC_CNTSTART ISP_GCAC_CNTSTART;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GCAC_CNTSTART.u32 = pstIspReg->ISP_GCAC_CNTSTART.u32;
	return ISP_GCAC_CNTSTART.bits.cnt_start_v;
}

static __inline HI_VOID hi_isp_gcac_cnt_start_hor_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GCAC_CNTSTART ISP_GCAC_CNTSTART;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GCAC_CNTSTART.u32 = pstIspReg->ISP_GCAC_CNTSTART.u32;
	ISP_GCAC_CNTSTART.bits.cnt_start_h = data;
	pstIspReg->ISP_GCAC_CNTSTART.u32 = ISP_GCAC_CNTSTART.u32;
}

static __inline HI_U16 hi_isp_gcac_cnt_start_hor_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GCAC_CNTSTART ISP_GCAC_CNTSTART;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GCAC_CNTSTART.u32 = pstIspReg->ISP_GCAC_CNTSTART.u32;
	return ISP_GCAC_CNTSTART.bits.cnt_start_h;
}


/*****************************************************
ISP_GCAC_CORSTART:   global cac 
0x113c3228:         31:29   reserved
				    28:16	global cac    vertical point 
					15:13   reserved
					12: 0   global cac  horizontal point
*****************************************************/
#define HI_ISP_GCAC_COORDINATE_START_X_DEFAULT  (0)
#define HI_ISP_GCAC_COORDINATE_START_Y_DEFAULT  (0)
static __inline HI_VOID hi_isp_gcac_cor_start_ver_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GCAC_CORSTART ISP_GCAC_CORSTART;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GCAC_CORSTART.u32 = pstIspReg->ISP_GCAC_CORSTART.u32;
	ISP_GCAC_CORSTART.bits.cor_start_v = data;
	pstIspReg->ISP_GCAC_CORSTART.u32 = ISP_GCAC_CORSTART.u32;
}

static __inline HI_VOID hi_isp_gcac_cor_start_hor_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GCAC_CORSTART ISP_GCAC_CORSTART;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GCAC_CORSTART.u32 = pstIspReg->ISP_GCAC_CORSTART.u32;
	ISP_GCAC_CORSTART.bits.cor_start_h = data;
	pstIspReg->ISP_GCAC_CORSTART.u32 = ISP_GCAC_CORSTART.u32;
}

/*****************************************************
ISP_GCAC_FILTEN:   global cac 
0x113c322c:               	31:1       reserved
					     0    	global cac
*****************************************************/
#define HI_ISP_GCAC_VER_FILT_EN_DEFAULT (0)
static __inline HI_VOID hi_isp_gcac_ver_filt_en_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GCAC_FILTEN ISP_GCAC_FILTEN;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GCAC_FILTEN.u32 = pstIspReg->ISP_GCAC_FILTEN.u32;
	ISP_GCAC_FILTEN.bits.ver_filt_en = data;
	pstIspReg->ISP_GCAC_FILTEN.u32 = ISP_GCAC_FILTEN.u32;
}

/*****************************************************
ISP_GCAC_CHRVMODE:   global cac filter mode select
0x113c3230:          31:2   reserved

*****************************************************/
#define HI_ISP_GCAC_CHR_VER_MODE_DEFAULT (1) 		
static __inline HI_VOID hi_isp_gcac_chr_ver_mode_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GCAC_CHRVMODE ISP_GCAC_CHRVMODE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GCAC_CHRVMODE.u32 = pstIspReg->ISP_GCAC_CHRVMODE.u32;
	ISP_GCAC_CHRVMODE.bits.chrv_mode = data;
	pstIspReg->ISP_GCAC_CHRVMODE.u32 = ISP_GCAC_CHRVMODE.u32;
}

/*****************************************************
ISP_GCAC_CLIPMODE:   global cac 
0x113c3234:         31:18   reserved
					17:16	globalcacvetical

					15:1	reserved
					  0    	globalcac
							0:fix cliping,clip Range [64,192]
							1:dynamic cliping,
*****************************************************/
#define HI_ISP_GCAC_CLIP_MODE_VER_DEFAULT (2)
static __inline HI_VOID hi_isp_gcac_ver_clip_mode_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GCAC_CLIPMODE ISP_GCAC_CLIPMODE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GCAC_CLIPMODE.u32 = pstIspReg->ISP_GCAC_CLIPMODE.u32;
	ISP_GCAC_CLIPMODE.bits.clip_mode_v = data;
	pstIspReg->ISP_GCAC_CLIPMODE.u32 = ISP_GCAC_CLIPMODE.u32;
}

#define HI_ISP_GCAC_CLIP_MODE_HOR_DEFAULT (1)
static __inline HI_VOID hi_isp_gcac_hor_clip_mode_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GCAC_CLIPMODE ISP_GCAC_CLIPMODE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GCAC_CLIPMODE.u32 = pstIspReg->ISP_GCAC_CLIPMODE.u32;
	ISP_GCAC_CLIPMODE.bits.clip_mode_h = data;
	pstIspReg->ISP_GCAC_CLIPMODE.u32 = ISP_GCAC_CLIPMODE.u32;
}

/*****************************************************
ISP_GCAC_SIZE:   global cac         
0x113c32F0:         31:29       reserved
				    28:16		image height (minus 1)
					15:13      reserved
					12: 0    	image width (minus 1)
*****************************************************/
static __inline HI_VOID hi_isp_gcac_height_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GCAC_SIZE ISP_GCAC_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GCAC_SIZE.u32 = pstIspReg->ISP_GCAC_SIZE.u32;
	ISP_GCAC_SIZE.bits.height = data;
	pstIspReg->ISP_GCAC_SIZE.u32 = ISP_GCAC_SIZE.u32;
}

static __inline HI_VOID hi_isp_gcac_width_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GCAC_SIZE ISP_GCAC_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GCAC_SIZE.u32 = pstIspReg->ISP_GCAC_SIZE.u32;
	ISP_GCAC_SIZE.bits.width = data;
	pstIspReg->ISP_GCAC_SIZE.u32 = ISP_GCAC_SIZE.u32;
}

/*****************************************************
MODULE        -----COMPANDER-----
MODULE        -----COMPANDER-----
MODULE        -----COMPANDER-----
*****************************************************/
/*****************************************************
ISP_COMPANDER_CFG:              
0x205a6800:                     31:1      reserved
                                          0           COMPANDERenable
*****************************************************/
static __inline HI_VOID hi_isp_compander_enable_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_EXPANDER_CFG ISP_EXPANDER_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_EXPANDER_CFG.u32 = pstIspReg->ISP_EXPANDER_CFG.u32;
	ISP_EXPANDER_CFG.bits.en = data;
	pstIspReg->ISP_EXPANDER_CFG.u32 = ISP_EXPANDER_CFG.u32;
}

/*****************************************************
ISP_COMPANDER_BITW:   COMPANDER data witdth        
0x205a6810:                     31:21      reserved
                                          20:16      bitw_in
                                          15:05      reserved
                                          04:00      bitw_out
*****************************************************/
static __inline HI_VOID hi_isp_compander_bitw_in_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_EXPANDER_BITW ISP_EXPANDER_BITW;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_EXPANDER_BITW.u32 = pstIspReg->ISP_EXPANDER_BITW.u32;
	ISP_EXPANDER_BITW.bits.bitw_in = data;
	pstIspReg->ISP_EXPANDER_BITW.u32 = ISP_EXPANDER_BITW.u32;
}

static __inline HI_VOID hi_isp_compander_bitw_out_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_EXPANDER_BITW ISP_EXPANDER_BITW;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_EXPANDER_BITW.u32 = pstIspReg->ISP_EXPANDER_BITW.u32;
	ISP_EXPANDER_BITW.bits.bitw_out = data;
	pstIspReg->ISP_EXPANDER_BITW.u32 = ISP_EXPANDER_BITW.u32;
}

/*****************************************************
ISP_COMPANDER_OFFSETR:   COMPANDER
0x205a6814:                     31:20      reserved
                                          19:00      R channel offset 
*****************************************************/
static __inline HI_VOID hi_isp_compander_offsetr_in_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_EXPANDER_OFFSETR ISP_EXPANDER_OFFSETR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_EXPANDER_OFFSETR.u32 = pstIspReg->ISP_EXPANDER_OFFSETR.u32;
	ISP_EXPANDER_OFFSETR.bits.offset_r = data;
	pstIspReg->ISP_EXPANDER_OFFSETR.u32 = ISP_EXPANDER_OFFSETR.u32;
}

/*****************************************************
ISP_COMPANDER_OFFSETGR:   COMPANDER
0x205a6818:                     31:20      reserved
                                          19:00      GR channel offset 
*****************************************************/
static __inline HI_VOID hi_isp_compander_offsetgr_in_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_EXPANDER_OFFSETGR ISP_EXPANDER_OFFSETGR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_EXPANDER_OFFSETGR.u32 = pstIspReg->ISP_EXPANDER_OFFSETGR.u32;
	ISP_EXPANDER_OFFSETGR.bits.offset_gr = data;
	pstIspReg->ISP_EXPANDER_OFFSETGR.u32 = ISP_EXPANDER_OFFSETGR.u32;
}

/*****************************************************
ISP_COMPANDER_OFFSETGB:   COMPANDER
0x205a681c:                     31:20      reserved
                                          19:00      GB channel offset 
*****************************************************/
static __inline HI_VOID hi_isp_compander_offsetgb_in_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_EXPANDER_OFFSETGB ISP_EXPANDER_OFFSETGB;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_EXPANDER_OFFSETGB.u32 = pstIspReg->ISP_EXPANDER_OFFSETGB.u32;
	ISP_EXPANDER_OFFSETGB.bits.offset_gb = data;
	pstIspReg->ISP_EXPANDER_OFFSETGB.u32 = ISP_EXPANDER_OFFSETGB.u32;
}

/*****************************************************
ISP_COMPANDER_OFFSETB:   COMPANDER
0x205a6820:                     31:20      reserved
                                          19:00      B channel offset 
*****************************************************/
static __inline HI_VOID hi_isp_compander_offsetb_in_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_EXPANDER_OFFSETB ISP_EXPANDER_OFFSETB;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_EXPANDER_OFFSETB.u32 = pstIspReg->ISP_EXPANDER_OFFSETB.u32;
	ISP_EXPANDER_OFFSETB.bits.offset_b = data;
	pstIspReg->ISP_EXPANDER_OFFSETB.u32 = ISP_EXPANDER_OFFSETB.u32;
}

/*****************************************************
ISP_COMPANDER_WADDR:         COMPANDER 129 Lut address
0x205a6880:                     31:00      compander_waddr
*****************************************************/
static __inline HI_VOID hi_isp_compander_waddr_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_EXPANDER_WADDR ISP_EXPANDER_WADDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_EXPANDER_WADDR.u32 = pstIspReg->ISP_EXPANDER_WADDR.u32;
	ISP_EXPANDER_WADDR.bits.expander_waddr = data;
	pstIspReg->ISP_EXPANDER_WADDR.u32 = ISP_EXPANDER_WADDR.u32;
}

/*****************************************************
ISP_COMPANDER_WDATA:  COMPANDER  129  lut data 
0x205a6884:                     31:00      compander_wdata
*****************************************************/
static __inline HI_VOID hi_isp_compander_wdata_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_EXPANDER_WDATA ISP_EXPANDER_WDATA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_EXPANDER_WDATA.u32 = pstIspReg->ISP_EXPANDER_WDATA.u32;
	ISP_EXPANDER_WDATA.bits.expander_wdata = data;
	pstIspReg->ISP_EXPANDER_WDATA.u32 = ISP_EXPANDER_WDATA.u32;
}

/*****************************************************
MODULE        -----CSC-----
MODULE        -----CSC-----
MODULE        -----CSC-----
*****************************************************/
/*****************************************************
ISP_CSC_CFG:              
0x205c1500:                     31:1      reserved

*****************************************************/
static __inline HI_VOID hi_isp_csc_enable_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CSC_CFG ISP_CSC_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CSC_CFG.u32 = pstIspReg->ISP_CSC_CFG.u32;
	ISP_CSC_CFG.bits.en = data;
	pstIspReg->ISP_CSC_CFG.u32 = ISP_CSC_CFG.u32;
}


static __inline HI_U8 hi_isp_csc_enable_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CSC_CFG ISP_CSC_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CSC_CFG.u32 = pstIspReg->ISP_CSC_CFG.u32;
	return ISP_CSC_CFG.bits.en ;
}


/*****************************************************
ISP_CSC_COEF0:              
0x205a1310:                     31:17      coef01
                                          16:16      reserved
                                          15:01      coef00
                                            0:  0      reserved
*****************************************************/
static __inline HI_VOID hi_isp_csc_coef01_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CSC_COEF0 ISP_CSC_COEF0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CSC_COEF0.u32 = pstIspReg->ISP_CSC_COEF0.u32;
	ISP_CSC_COEF0.bits.coef01 = data;
	pstIspReg->ISP_CSC_COEF0.u32 = ISP_CSC_COEF0.u32;
}

static __inline HI_U16 hi_isp_csc_coef01_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CSC_COEF0 ISP_CSC_COEF0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CSC_COEF0.u32 = pstIspReg->ISP_CSC_COEF0.u32;
	return ISP_CSC_COEF0.bits.coef01;
}

static __inline HI_VOID hi_isp_csc_coef00_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CSC_COEF0 ISP_CSC_COEF0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CSC_COEF0.u32 = pstIspReg->ISP_CSC_COEF0.u32;
	ISP_CSC_COEF0.bits.coef00 = data;
	pstIspReg->ISP_CSC_COEF0.u32 = ISP_CSC_COEF0.u32;
}

static __inline HI_U16 hi_isp_csc_coef00_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CSC_COEF0 ISP_CSC_COEF0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CSC_COEF0.u32 = pstIspReg->ISP_CSC_COEF0.u32;
	return ISP_CSC_COEF0.bits.coef00;
}

/*****************************************************
ISP_CSC_COEF1:              
0x205a1314:                     31:17      coef10
                                          16:16      reserved
                                          15:01      coef02
                                            0:  0      reserved
*****************************************************/
static __inline HI_VOID hi_isp_csc_coef10_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CSC_COEF1 ISP_CSC_COEF1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CSC_COEF1.u32 = pstIspReg->ISP_CSC_COEF1.u32;
	ISP_CSC_COEF1.bits.coef10 = data;
	pstIspReg->ISP_CSC_COEF1.u32 = ISP_CSC_COEF1.u32;
}

static __inline HI_U16 hi_isp_csc_coef10_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CSC_COEF1 ISP_CSC_COEF1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CSC_COEF1.u32 = pstIspReg->ISP_CSC_COEF1.u32;
	return ISP_CSC_COEF1.bits.coef10;
}

static __inline HI_VOID hi_isp_csc_coef02_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CSC_COEF1 ISP_CSC_COEF1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CSC_COEF1.u32 = pstIspReg->ISP_CSC_COEF1.u32;
	ISP_CSC_COEF1.bits.coef02 = data;
	pstIspReg->ISP_CSC_COEF1.u32 = ISP_CSC_COEF1.u32;
}

static __inline HI_U16 hi_isp_csc_coef02_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CSC_COEF1 ISP_CSC_COEF1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CSC_COEF1.u32 = pstIspReg->ISP_CSC_COEF1.u32;
	return ISP_CSC_COEF1.bits.coef02;
}

/*****************************************************
ISP_CSC_COEF2:               
0x205a1318:             31:17      coef12
						16:16      reserved
						15:01      coef11
						00:00      reserved
*****************************************************/
static __inline HI_VOID hi_isp_csc_coef12_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CSC_COEF2 ISP_CSC_COEF2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CSC_COEF2.u32 = pstIspReg->ISP_CSC_COEF2.u32;
	ISP_CSC_COEF2.bits.coef12 = data;
	pstIspReg->ISP_CSC_COEF2.u32 = ISP_CSC_COEF2.u32;
}

static __inline HI_U16 hi_isp_csc_coef12_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CSC_COEF2 ISP_CSC_COEF2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CSC_COEF2.u32 = pstIspReg->ISP_CSC_COEF2.u32;
	return ISP_CSC_COEF2.bits.coef12;
}

static __inline HI_VOID hi_isp_csc_coef11_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CSC_COEF2 ISP_CSC_COEF2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CSC_COEF2.u32 = pstIspReg->ISP_CSC_COEF2.u32;
	ISP_CSC_COEF2.bits.coef11 = data;
	pstIspReg->ISP_CSC_COEF2.u32 = ISP_CSC_COEF2.u32;
}

static __inline HI_U16 hi_isp_csc_coef11_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CSC_COEF2 ISP_CSC_COEF2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CSC_COEF2.u32 = pstIspReg->ISP_CSC_COEF2.u32;
	return ISP_CSC_COEF2.bits.coef11;
}

/*****************************************************
ISP_CSC_COEF3:              
0x205a131c:                     31:17      coef21
                                          16:16      reserved
                                          15:01      coef20
                                          00:00      reserved
*****************************************************/
static __inline HI_VOID hi_isp_csc_coef21_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CSC_COEF3 ISP_CSC_COEF3;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CSC_COEF3.u32 = pstIspReg->ISP_CSC_COEF3.u32;
	ISP_CSC_COEF3.bits.coef21 = data;
	pstIspReg->ISP_CSC_COEF3.u32 = ISP_CSC_COEF3.u32;
}

static __inline HI_U16 hi_isp_csc_coef21_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CSC_COEF3 ISP_CSC_COEF3;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CSC_COEF3.u32 = pstIspReg->ISP_CSC_COEF3.u32;
	return ISP_CSC_COEF3.bits.coef21;
}

static __inline HI_VOID hi_isp_csc_coef20_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CSC_COEF3 ISP_CSC_COEF3;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CSC_COEF3.u32 = pstIspReg->ISP_CSC_COEF3.u32;
	ISP_CSC_COEF3.bits.coef20 = data;
	pstIspReg->ISP_CSC_COEF3.u32 = ISP_CSC_COEF3.u32;
}

static __inline HI_U16 hi_isp_csc_coef20_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CSC_COEF3 ISP_CSC_COEF3;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CSC_COEF3.u32 = pstIspReg->ISP_CSC_COEF3.u32;
	return ISP_CSC_COEF3.bits.coef20;
}

/*****************************************************
ISP_CSC_COEF4:              
0x205a1320:                     31:17      reserved
                                          16:16      reserved
                                          15:01      coef22
                                          00:00      reserved
*****************************************************/
static __inline HI_VOID hi_isp_csc_coef22_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CSC_COEF4 ISP_CSC_COEF4;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CSC_COEF4.u32 = pstIspReg->ISP_CSC_COEF4.u32;
	ISP_CSC_COEF4.bits.coef22 = data;
	pstIspReg->ISP_CSC_COEF4.u32 = ISP_CSC_COEF4.u32;
}

static __inline HI_U16 hi_isp_csc_coef22_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CSC_COEF4 ISP_CSC_COEF4;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CSC_COEF4.u32 = pstIspReg->ISP_CSC_COEF4.u32;
	return ISP_CSC_COEF4.bits.coef22;
}

/*****************************************************
ISP_CSC_IN_DC0:              
0x205a1330:                     31:21      in_dc1
                                          20:16      reserved
                                          15:05      in_dc0
                                            4:  0      reserved
*****************************************************/
static __inline HI_VOID hi_isp_csc_in_dc1_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CSC_IN_DC0 ISP_CSC_IN_DC0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CSC_IN_DC0.u32 = pstIspReg->ISP_CSC_IN_DC0.u32;
	ISP_CSC_IN_DC0.bits.in_dc1 = data;
	pstIspReg->ISP_CSC_IN_DC0.u32 = ISP_CSC_IN_DC0.u32;
}

static __inline HI_U16 hi_isp_csc_in_dc1_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CSC_IN_DC0 ISP_CSC_IN_DC0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CSC_IN_DC0.u32 = pstIspReg->ISP_CSC_IN_DC0.u32;
	return ISP_CSC_IN_DC0.bits.in_dc1;
}

static __inline HI_VOID hi_isp_csc_in_dc0_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CSC_IN_DC0 ISP_CSC_IN_DC0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CSC_IN_DC0.u32 = pstIspReg->ISP_CSC_IN_DC0.u32;
	ISP_CSC_IN_DC0.bits.in_dc0 = data;
	pstIspReg->ISP_CSC_IN_DC0.u32 = ISP_CSC_IN_DC0.u32;
}

static __inline HI_U16 hi_isp_csc_in_dc0_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CSC_IN_DC0 ISP_CSC_IN_DC0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CSC_IN_DC0.u32 = pstIspReg->ISP_CSC_IN_DC0.u32;
	return ISP_CSC_IN_DC0.bits.in_dc0;
}

/*****************************************************
ISP_CSC_IN_DC1:              
0x205a1334:                     31:21      reserved
                                          20:16      reserved
                                          15:05      in_dc2
                                            4:  0      reserved
*****************************************************/
static __inline HI_VOID hi_isp_csc_in_dc2_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CSC_IN_DC1 ISP_CSC_IN_DC1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CSC_IN_DC1.u32 = pstIspReg->ISP_CSC_IN_DC1.u32;
	ISP_CSC_IN_DC1.bits.in_dc2 = data;
	pstIspReg->ISP_CSC_IN_DC1.u32 = ISP_CSC_IN_DC1.u32;
}

static __inline HI_U16 hi_isp_csc_in_dc2_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CSC_IN_DC1 ISP_CSC_IN_DC1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CSC_IN_DC1.u32 = pstIspReg->ISP_CSC_IN_DC1.u32;
	return ISP_CSC_IN_DC1.bits.in_dc2;
}

/*****************************************************
ISP_CSC_OUT_DC0:              
0x205a1340:                     31:21      out_dc1
                                          20:16      reserved
                                          15:05      out_dc0
                                            4:  0      reserved
*****************************************************/
static __inline HI_VOID hi_isp_csc_out_dc1_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CSC_OUT_DC0 ISP_CSC_OUT_DC0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CSC_OUT_DC0.u32 = pstIspReg->ISP_CSC_OUT_DC0.u32;
	ISP_CSC_OUT_DC0.bits.out_dc1 = data;
	pstIspReg->ISP_CSC_OUT_DC0.u32 = ISP_CSC_OUT_DC0.u32;
}

static __inline HI_U16 hi_isp_csc_out_dc1_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CSC_OUT_DC0 ISP_CSC_OUT_DC0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CSC_OUT_DC0.u32 = pstIspReg->ISP_CSC_OUT_DC0.u32;
	return ISP_CSC_OUT_DC0.bits.out_dc1;
}

static __inline HI_VOID hi_isp_csc_out_dc0_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CSC_OUT_DC0 ISP_CSC_OUT_DC0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CSC_OUT_DC0.u32 = pstIspReg->ISP_CSC_OUT_DC0.u32;
	ISP_CSC_OUT_DC0.bits.out_dc0 = data;
	pstIspReg->ISP_CSC_OUT_DC0.u32 = ISP_CSC_OUT_DC0.u32;
}

static __inline HI_U16 hi_isp_csc_out_dc0_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CSC_OUT_DC0 ISP_CSC_OUT_DC0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CSC_OUT_DC0.u32 = pstIspReg->ISP_CSC_OUT_DC0.u32;
	return ISP_CSC_OUT_DC0.bits.out_dc0;
}

/*****************************************************
ISP_CSC_OUT_DC0:              
0x205a1344:                     31:21      reserved
                                          20:16      reserved
                                          15:05      out_dc2
                                            4:  0      reserved
*****************************************************/
static __inline HI_VOID hi_isp_csc_out_dc2_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CSC_OUT_DC1 ISP_CSC_OUT_DC1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CSC_OUT_DC1.u32 = pstIspReg->ISP_CSC_OUT_DC1.u32;
	ISP_CSC_OUT_DC1.bits.out_dc2 = data;
	pstIspReg->ISP_CSC_OUT_DC1.u32 = ISP_CSC_OUT_DC1.u32;
}

static __inline HI_U16 hi_isp_csc_out_dc2_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CSC_OUT_DC1 ISP_CSC_OUT_DC1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CSC_OUT_DC1.u32 = pstIspReg->ISP_CSC_OUT_DC1.u32;
	return ISP_CSC_OUT_DC1.bits.out_dc2;
}

/*****************************************************
MODULE        -----DEMOSAIC-----
MODULE        -----DEMOSAIC-----
MODULE        -----DEMOSAIC-----
*****************************************************/
/*****************************************************
ISP_DEMOSAIC_CFG:              
0x205c4000:                     31:1      reserved
                                          0      DEMOSAICenable
*****************************************************/
#define HI_ISP_DEMOSAIC_CFG_EN_DEFAULT  (1)
static __inline HI_VOID hi_isp_demosaic_cfg_enable_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_CFG ISP_DEMOSAIC_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_CFG.u32 = pstIspReg->ISP_DEMOSAIC_CFG.u32;
	ISP_DEMOSAIC_CFG.bits.en = data;
	pstIspReg->ISP_DEMOSAIC_CFG.u32 = ISP_DEMOSAIC_CFG.u32;
}


static __inline HI_U8 hi_isp_demosaic_cfg_enable_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_CFG ISP_DEMOSAIC_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_CFG.u32 = pstIspReg->ISP_DEMOSAIC_CFG.u32;
	return ISP_DEMOSAIC_CFG.bits.en ;
}


/*****************************************************
ISP_DEMOSAIC_CFG1:              
0x205a3004:                 31:17      reserved
							21:21      de_fake_en
							20:20      mid_filter_en
							19:19      hv_dir_en
							18:18      gcac_blend_select
							17:17      local_cac_en
							16:16      fcrenable
							15:01      reserved
							00:00      ahdenable
*****************************************************/
#define HI_ISP_DEMOSAIC_FAKE_EN_DEFAULT (1)
static __inline HI_VOID hi_isp_demosaic_de_fake_enable_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_CFG1 ISP_DEMOSAIC_CFG1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_CFG1.u32 = pstIspReg->ISP_DEMOSAIC_CFG1.u32;
	ISP_DEMOSAIC_CFG1.bits.de_fake_en = data;
	pstIspReg->ISP_DEMOSAIC_CFG1.u32 = ISP_DEMOSAIC_CFG1.u32;
}

#define HI_ISP_DEMOSAIC_MID_FILTER_EN_DEFAULT (1)
static __inline HI_VOID hi_isp_demosaic_mid_filter_enable_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_CFG1 ISP_DEMOSAIC_CFG1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_CFG1.u32 = pstIspReg->ISP_DEMOSAIC_CFG1.u32;
	ISP_DEMOSAIC_CFG1.bits.mid_filter_en = data;
	pstIspReg->ISP_DEMOSAIC_CFG1.u32 = ISP_DEMOSAIC_CFG1.u32;
}

#define HI_ISP_DEMOSAIC_HV_DIR_EN_DEFAULT (0)
static __inline HI_VOID hi_isp_demosaic_hv_dir_enable_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_CFG1 ISP_DEMOSAIC_CFG1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_CFG1.u32 = pstIspReg->ISP_DEMOSAIC_CFG1.u32;
	ISP_DEMOSAIC_CFG1.bits.hv_dir_en = data;
	pstIspReg->ISP_DEMOSAIC_CFG1.u32 = ISP_DEMOSAIC_CFG1.u32;
}

#define HI_ISP_GCAC_BLEND_SELECTION_DEFAULT (0)	/*0:Gcac blend with DM;   1:Gcac output*/
static __inline HI_VOID hi_isp_demosaic_gcac_blend_select_enable_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_CFG1 ISP_DEMOSAIC_CFG1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_CFG1.u32 = pstIspReg->ISP_DEMOSAIC_CFG1.u32;
	ISP_DEMOSAIC_CFG1.bits.gcac_blend_select = data;
	pstIspReg->ISP_DEMOSAIC_CFG1.u32 = ISP_DEMOSAIC_CFG1.u32;
}


static __inline HI_VOID hi_isp_demosaic_local_cac_enable_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_CFG1 ISP_DEMOSAIC_CFG1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_CFG1.u32 = pstIspReg->ISP_DEMOSAIC_CFG1.u32;
	ISP_DEMOSAIC_CFG1.bits.local_cac_en = data;
	pstIspReg->ISP_DEMOSAIC_CFG1.u32 = ISP_DEMOSAIC_CFG1.u32;
}

static __inline HI_U8 hi_isp_demosaic_local_cac_enable_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_CFG1 ISP_DEMOSAIC_CFG1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_CFG1.u32 = pstIspReg->ISP_DEMOSAIC_CFG1.u32;
	return ISP_DEMOSAIC_CFG1.bits.local_cac_en;
}

#define HI_ISP_DEMOSAIC_FCR_EN_DEFAULT (1)
static __inline HI_VOID hi_isp_demosaic_fcr_enable_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_CFG1 ISP_DEMOSAIC_CFG1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_CFG1.u32 = pstIspReg->ISP_DEMOSAIC_CFG1.u32;
	ISP_DEMOSAIC_CFG1.bits.fcr_en = data;
	pstIspReg->ISP_DEMOSAIC_CFG1.u32 = ISP_DEMOSAIC_CFG1.u32;
}

static __inline HI_U8 hi_isp_demosaic_fcr_enable_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_CFG1 ISP_DEMOSAIC_CFG1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_CFG1.u32 = pstIspReg->ISP_DEMOSAIC_CFG1.u32;
	return ISP_DEMOSAIC_CFG1.bits.fcr_en ;
}


#define HI_ISP_DEMOSAIC_AND_EN_DEFAULT (1)
static __inline HI_VOID hi_isp_demosaic_ahd_enable_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_CFG1 ISP_DEMOSAIC_CFG1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_CFG1.u32 = pstIspReg->ISP_DEMOSAIC_CFG1.u32;
	ISP_DEMOSAIC_CFG1.bits.ahd_en = data;
	pstIspReg->ISP_DEMOSAIC_CFG1.u32 = ISP_DEMOSAIC_CFG1.u32;
}

/*****************************************************
ISP_DEMOSAIC_COEF0:              
0x113c4010:                     31:24       reserve  
                                          23:16      bld_limit1
                                          15:  8       reserve  
                                            7:  0       bld_limit2
*****************************************************/
#define HI_ISP_DEMOSAIC_BLENDLIMIT1_DEFAULT (24)
#define HI_ISP_DEMOSAIC_BLENDLIMIT2_DEFAULT (40)
static __inline HI_VOID hi_isp_demosaic_bld_limit1_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_COEF0 ISP_DEMOSAIC_COEF0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_COEF0.u32 = pstIspReg->ISP_DEMOSAIC_COEF0.u32;
	ISP_DEMOSAIC_COEF0.bits.bld_limit1 = data;
	pstIspReg->ISP_DEMOSAIC_COEF0.u32 = ISP_DEMOSAIC_COEF0.u32;
}
static __inline HI_U8 hi_isp_demosaic_bld_limit1_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_COEF0 ISP_DEMOSAIC_COEF0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_COEF0.u32 = pstIspReg->ISP_DEMOSAIC_COEF0.u32;
	return ISP_DEMOSAIC_COEF0.bits.bld_limit1 ;
}

static __inline HI_VOID hi_isp_demosaic_bld_limit2_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_COEF0 ISP_DEMOSAIC_COEF0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_COEF0.u32 = pstIspReg->ISP_DEMOSAIC_COEF0.u32;
	ISP_DEMOSAIC_COEF0.bits.bld_limit2 = data;
	pstIspReg->ISP_DEMOSAIC_COEF0.u32 = ISP_DEMOSAIC_COEF0.u32;
}
static __inline HI_U8 hi_isp_demosaic_bld_limit2_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_COEF0 ISP_DEMOSAIC_COEF0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_COEF0.u32 = pstIspReg->ISP_DEMOSAIC_COEF0.u32;
	return ISP_DEMOSAIC_COEF0.bits.bld_limit2 ;
}


/*****************************************************
ISP_DEMOSAIC_COEF1:              
0x205a3014:                     31:25       reserve  
						    24:16       bld_ratio1
						    15:9          reserve  
						      8:0         bld_ratio2
*****************************************************/
#define HI_ISP_DEMOSAIC_BLENDRATIO1_DEFAULT (0x0)
#define HI_ISP_DEMOSAIC_BLENDRATIO2_DEFAULT (0x100)
static __inline HI_VOID hi_isp_demosaic_bld_ratio1_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_COEF1 ISP_DEMOSAIC_COEF1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_COEF1.u32 = pstIspReg->ISP_DEMOSAIC_COEF1.u32;
	ISP_DEMOSAIC_COEF1.bits.bld_ratio1 = data;
	pstIspReg->ISP_DEMOSAIC_COEF1.u32 = ISP_DEMOSAIC_COEF1.u32;
}

static __inline HI_VOID hi_isp_demosaic_bld_ratio2_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_COEF1 ISP_DEMOSAIC_COEF1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_COEF1.u32 = pstIspReg->ISP_DEMOSAIC_COEF1.u32;
	ISP_DEMOSAIC_COEF1.bits.bld_ratio2 = data;
	pstIspReg->ISP_DEMOSAIC_COEF1.u32 = ISP_DEMOSAIC_COEF1.u32;
}


/*****************************************************
ISP_DEMOSAIC_COEF2:              
0x205a3018:                     31:28       reserve  
						    27:16      fcr_limit1
						    15:12       reserve  
						    11:  0      fcr_limit2
*****************************************************/
#define HI_ISP_DEMOSAIC_FCR_LIMIT1_DEFAULT (4)
#define HI_ISP_DEMOSAIC_FCR_LIMIT2_DEFAULT (40)
static __inline HI_VOID hi_isp_demosaic_fcr_limit1_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_COEF2 ISP_DEMOSAIC_COEF2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_COEF2.u32 = pstIspReg->ISP_DEMOSAIC_COEF2.u32;
	ISP_DEMOSAIC_COEF2.bits.fcr_limit1 = data;
	pstIspReg->ISP_DEMOSAIC_COEF2.u32 = ISP_DEMOSAIC_COEF2.u32;
}

static __inline HI_VOID hi_isp_demosaic_fcr_limit2_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_COEF2 ISP_DEMOSAIC_COEF2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_COEF2.u32 = pstIspReg->ISP_DEMOSAIC_COEF2.u32;
	ISP_DEMOSAIC_COEF2.bits.fcr_limit2 = data;
	pstIspReg->ISP_DEMOSAIC_COEF2.u32 = ISP_DEMOSAIC_COEF2.u32;
}

/*****************************************************
ISP_DEMOSAIC_COEF3:              
0x205a301c:                     31:26       reserve  
						    25:16      ahd_par1
						    15:  8       reserve  
						      7:  0      ahd_par2
*****************************************************/
#define HI_ISP_DEMOSAIC_AHDPART1_DEFAULT (400)
static __inline HI_VOID hi_isp_demosaic_ahd_par1_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_COEF3 ISP_DEMOSAIC_COEF3;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_COEF3.u32 = pstIspReg->ISP_DEMOSAIC_COEF3.u32;
	ISP_DEMOSAIC_COEF3.bits.ahd_par1 = data;
	pstIspReg->ISP_DEMOSAIC_COEF3.u32 = ISP_DEMOSAIC_COEF3.u32;
}

static __inline HI_U16 hi_isp_demosaic_ahd_par1_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_COEF3 ISP_DEMOSAIC_COEF3;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_COEF3.u32 = pstIspReg->ISP_DEMOSAIC_COEF3.u32;
	return ISP_DEMOSAIC_COEF3.bits.ahd_par1;	
}

#define HI_ISP_DEMOSAIC_AHDPART2_DEFAULT (8)
static __inline HI_VOID hi_isp_demosaic_ahd_par2_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_COEF3 ISP_DEMOSAIC_COEF3;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_COEF3.u32 = pstIspReg->ISP_DEMOSAIC_COEF3.u32;
	ISP_DEMOSAIC_COEF3.bits.ahd_par2 = data;
	pstIspReg->ISP_DEMOSAIC_COEF3.u32 = ISP_DEMOSAIC_COEF3.u32;
}

/*****************************************************
ISP_DEMOSAIC_COEF4:              
0x205a3020:                     31:28       reserve  
						    27:16      fcr_thr
						    15:  5       reserve  
						      4:  0      fcr_gain
*****************************************************/
#define HI_ISP_DEMOSAIC_FCR_THR_DEFAULT (1)
static __inline HI_VOID hi_isp_demosaic_fcr_thr_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_COEF4 ISP_DEMOSAIC_COEF4;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_COEF4.u32 = pstIspReg->ISP_DEMOSAIC_COEF4.u32;
	ISP_DEMOSAIC_COEF4.bits.fcr_thr = data;
	pstIspReg->ISP_DEMOSAIC_COEF4.u32 = ISP_DEMOSAIC_COEF4.u32;
}

#define HI_ISP_DEMOSAIC_FCR_GAIN_DEFAULT (8)
static __inline HI_VOID hi_isp_demosaic_fcr_gain_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_COEF4 ISP_DEMOSAIC_COEF4;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_COEF4.u32 = pstIspReg->ISP_DEMOSAIC_COEF4.u32;
	ISP_DEMOSAIC_COEF4.bits.fcr_gain = data;
	pstIspReg->ISP_DEMOSAIC_COEF4.u32 = ISP_DEMOSAIC_COEF4.u32;
}

/*****************************************************
ISP_DEMOSAIC_COEF5:              
0x205a3024:                     31:8       reserve  
						    7:  0        hv_ratio
*****************************************************/
#define HI_ISP_DEMOSAIC_HV_RATIO_DEFAULT (48)
static __inline HI_VOID hi_isp_demosaic_hv_ratio_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_COEF5 ISP_DEMOSAIC_COEF5;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_COEF5.u32 = pstIspReg->ISP_DEMOSAIC_COEF5.u32;
	ISP_DEMOSAIC_COEF5.bits.hv_ratio = data;
	pstIspReg->ISP_DEMOSAIC_COEF5.u32 = ISP_DEMOSAIC_COEF5.u32;

}
static __inline HI_U8 hi_isp_demosaic_hv_ratio_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_COEF5 ISP_DEMOSAIC_COEF5;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_COEF5.u32 = pstIspReg->ISP_DEMOSAIC_COEF5.u32;
	return ISP_DEMOSAIC_COEF5.bits.hv_ratio ;
}

/*****************************************************
ISP_DEMOSAIC_COEF6:              
0x205a3028:                     31:06       reserve  
						    05:00      fcr_ratio
*****************************************************/
#define HI_ISP_DEMOSAIC_FCR_RATIO_DEFAULT (10)
static __inline HI_VOID hi_isp_demosaic_fcr_ratio_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_COEF6 ISP_DEMOSAIC_COEF6;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_COEF6.u32 = pstIspReg->ISP_DEMOSAIC_COEF6.u32;
	ISP_DEMOSAIC_COEF6.bits.fcr_ratio = data;
	pstIspReg->ISP_DEMOSAIC_COEF6.u32 = ISP_DEMOSAIC_COEF6.u32;
}

/*****************************************************
ISP_DEMOSAIC_COEF6:              
0x205a3030:                     31:24       reserve  
						      1:  0      hv_sel
*****************************************************/
#define HI_ISP_DEMOSAIC_HV_SEL_DEFAULT (3)
static __inline HI_VOID hi_isp_demosaic_hv_sel_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_SEL ISP_DEMOSAIC_SEL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_SEL.u32 = pstIspReg->ISP_DEMOSAIC_SEL.u32;
	ISP_DEMOSAIC_SEL.bits.hv_sel = data;
	pstIspReg->ISP_DEMOSAIC_SEL.u32 = ISP_DEMOSAIC_SEL.u32;
}

/*****************************************************
ISP_DEMOSAIC_COEF7:              
0x205a3034:                     31:1      reserved
                                          0           tmplate
*****************************************************/
static __inline HI_VOID hi_isp_demosaic_tmplate_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_TMP ISP_DEMOSAIC_TMP;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_TMP.u32 = pstIspReg->ISP_DEMOSAIC_TMP.u32;
	ISP_DEMOSAIC_TMP.bits.tmplate = data;
	pstIspReg->ISP_DEMOSAIC_TMP.u32 = ISP_DEMOSAIC_TMP.u32;
}

/*****************************************************
ISP_DEMOSAIC_LCAC_CNT_THR:     
0x113c4038:                     31:01      reserved
                                15:10  g_counter_thr
                                09:05  b_counter_thr
                                04:00  r_counter_thr
*****************************************************/
#define HI_ISP_LCAC_COUNT_THR_R_DEFAULT (1)
#define HI_ISP_LCAC_COUNT_THR_G_DEFAULT (1)
#define HI_ISP_LCAC_COUNT_THR_B_DEFAULT (1)

static __inline HI_VOID hi_isp_demosaic_g_counter_thr_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_LCAC_CNT_THR ISP_DEMOSAIC_LCAC_CNT_THR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_LCAC_CNT_THR.u32 = pstIspReg->ISP_DEMOSAIC_LCAC_CNT_THR.u32;
	ISP_DEMOSAIC_LCAC_CNT_THR.bits.g_counter_thr = data;
	pstIspReg->ISP_DEMOSAIC_LCAC_CNT_THR.u32 = ISP_DEMOSAIC_LCAC_CNT_THR.u32;
}

static __inline HI_VOID hi_isp_demosaic_b_counter_thr_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_LCAC_CNT_THR ISP_DEMOSAIC_LCAC_CNT_THR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_LCAC_CNT_THR.u32 = pstIspReg->ISP_DEMOSAIC_LCAC_CNT_THR.u32;
	ISP_DEMOSAIC_LCAC_CNT_THR.bits.b_counter_thr = data;
	pstIspReg->ISP_DEMOSAIC_LCAC_CNT_THR.u32 = ISP_DEMOSAIC_LCAC_CNT_THR.u32;
}

static __inline HI_VOID hi_isp_demosaic_r_counter_thr_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_LCAC_CNT_THR ISP_DEMOSAIC_LCAC_CNT_THR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_LCAC_CNT_THR.u32 = pstIspReg->ISP_DEMOSAIC_LCAC_CNT_THR.u32;
	ISP_DEMOSAIC_LCAC_CNT_THR.bits.r_counter_thr = data;
	pstIspReg->ISP_DEMOSAIC_LCAC_CNT_THR.u32 = ISP_DEMOSAIC_LCAC_CNT_THR.u32;
}

/*****************************************************
ISP_DEMOSAIC_LCAC_LUMA_RB_THR:  
0x113c403c:                     31:01      reserved
                                27:16  b_luma_thr
                                11:00  r_luma_thr
*****************************************************/
static __inline HI_VOID hi_isp_demosaic_b_luma_thr_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_LCAC_LUMA_RB_THR ISP_DEMOSAIC_LCAC_LUMA_RB_THR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_LCAC_LUMA_RB_THR.u32 = pstIspReg->ISP_DEMOSAIC_LCAC_LUMA_RB_THR.u32;
	ISP_DEMOSAIC_LCAC_LUMA_RB_THR.bits.b_luma_thr = data;
	pstIspReg->ISP_DEMOSAIC_LCAC_LUMA_RB_THR.u32 = ISP_DEMOSAIC_LCAC_LUMA_RB_THR.u32;
}

static __inline HI_U16 hi_isp_demosaic_b_luma_thr_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_LCAC_LUMA_RB_THR ISP_DEMOSAIC_LCAC_LUMA_RB_THR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_LCAC_LUMA_RB_THR.u32 = pstIspReg->ISP_DEMOSAIC_LCAC_LUMA_RB_THR.u32;
	return ISP_DEMOSAIC_LCAC_LUMA_RB_THR.bits.b_luma_thr;
}

static __inline HI_VOID hi_isp_demosaic_r_luma_thr_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_LCAC_LUMA_RB_THR ISP_DEMOSAIC_LCAC_LUMA_RB_THR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_LCAC_LUMA_RB_THR.u32 = pstIspReg->ISP_DEMOSAIC_LCAC_LUMA_RB_THR.u32;
	ISP_DEMOSAIC_LCAC_LUMA_RB_THR.bits.r_luma_thr = data;
	pstIspReg->ISP_DEMOSAIC_LCAC_LUMA_RB_THR.u32 = ISP_DEMOSAIC_LCAC_LUMA_RB_THR.u32;
}

static __inline HI_U16 hi_isp_demosaic_r_luma_thr_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_LCAC_LUMA_RB_THR ISP_DEMOSAIC_LCAC_LUMA_RB_THR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_LCAC_LUMA_RB_THR.u32 = pstIspReg->ISP_DEMOSAIC_LCAC_LUMA_RB_THR.u32;
	return ISP_DEMOSAIC_LCAC_LUMA_RB_THR.bits.r_luma_thr;
}


/*****************************************************
ISP_DEMOSAIC_LCAC_LUMA_G_THR:  
0x113c4040:                     31:01  reserved
                                11:00  g_luma_thr
*****************************************************/
static __inline HI_VOID hi_isp_demosaic_g_luma_thr_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_LCAC_LUMA_G_THR ISP_DEMOSAIC_LCAC_LUMA_G_THR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_LCAC_LUMA_G_THR.u32 = pstIspReg->ISP_DEMOSAIC_LCAC_LUMA_G_THR.u32;
	ISP_DEMOSAIC_LCAC_LUMA_G_THR.bits.g_luma_thr = data;
	pstIspReg->ISP_DEMOSAIC_LCAC_LUMA_G_THR.u32 = ISP_DEMOSAIC_LCAC_LUMA_G_THR.u32;
}

static __inline HI_U16 hi_isp_demosaic_g_luma_thr_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_LCAC_LUMA_G_THR ISP_DEMOSAIC_LCAC_LUMA_G_THR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_LCAC_LUMA_G_THR.u32 = pstIspReg->ISP_DEMOSAIC_LCAC_LUMA_G_THR.u32;
	return ISP_DEMOSAIC_LCAC_LUMA_G_THR.bits.g_luma_thr;
}


/*****************************************************
ISP_DEMOSAIC_PURPLE_VAR_THR:   
0x113c4044:                     31:01  reserved
                                11:00  purple_var_thr
*****************************************************/
#define HI_ISP_LCAC_PURPLE_DET_THR_DEFAULT (0)
static __inline HI_VOID hi_isp_demosaic_purple_var_thr_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_PURPLE_VAR_THR ISP_DEMOSAIC_PURPLE_VAR_THR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_PURPLE_VAR_THR.u32 = pstIspReg->ISP_DEMOSAIC_PURPLE_VAR_THR.u32;
	ISP_DEMOSAIC_PURPLE_VAR_THR.bits.purple_var_thr = data;
	pstIspReg->ISP_DEMOSAIC_PURPLE_VAR_THR.u32 = ISP_DEMOSAIC_PURPLE_VAR_THR.u32;
}

static __inline HI_U16 hi_isp_demosaic_purple_var_thr_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_PURPLE_VAR_THR ISP_DEMOSAIC_PURPLE_VAR_THR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_PURPLE_VAR_THR.u32 = pstIspReg->ISP_DEMOSAIC_PURPLE_VAR_THR.u32;
	return ISP_DEMOSAIC_PURPLE_VAR_THR.bits.purple_var_thr;
}


/*****************************************************
ISP_DEMOSAIC_FAKE_CR_VAR_THR: 
0x113c4048:                     31:01  reserved
                                27:16  fake_cr_var_thr_high
                                11:00  fake_cr_var_thr_low
*****************************************************/
#define HI_ISP_DEMOSAIC_FAKE_CR_THR_HIGH_DEFAULT  (300)
static __inline HI_VOID hi_isp_demosaic_fake_cr_var_thr_high_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_FAKE_CR_VAR_THR ISP_DEMOSAIC_FAKE_CR_VAR_THR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_FAKE_CR_VAR_THR.u32 = pstIspReg->ISP_DEMOSAIC_FAKE_CR_VAR_THR.u32;
	ISP_DEMOSAIC_FAKE_CR_VAR_THR.bits.fake_cr_var_thr_high = data;
	pstIspReg->ISP_DEMOSAIC_FAKE_CR_VAR_THR.u32 = ISP_DEMOSAIC_FAKE_CR_VAR_THR.u32;
}

#define HI_ISP_DEMOSAIC_FAKE_CR_THR_LOW_DEFAULT   (0)
static __inline HI_VOID hi_isp_demosaic_fake_cr_var_thr_low_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_FAKE_CR_VAR_THR ISP_DEMOSAIC_FAKE_CR_VAR_THR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_FAKE_CR_VAR_THR.u32 = pstIspReg->ISP_DEMOSAIC_FAKE_CR_VAR_THR.u32;
	ISP_DEMOSAIC_FAKE_CR_VAR_THR.bits.fake_cr_var_thr_low = data;
	pstIspReg->ISP_DEMOSAIC_FAKE_CR_VAR_THR.u32 = ISP_DEMOSAIC_FAKE_CR_VAR_THR.u32;
}

/*****************************************************
ISP_DEMOSAIC_DEPURPLECTR:    
0x113c404c:                     31:01      reserved
                                19:16      depurplectrcr
                                03:00      depurplectrcb
*****************************************************/
static __inline HI_VOID hi_isp_demosaic_depurplectrcr_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_DEPURPLECTR ISP_DEMOSAIC_DEPURPLECTR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_DEPURPLECTR.u32 = pstIspReg->ISP_DEMOSAIC_DEPURPLECTR.u32;
	ISP_DEMOSAIC_DEPURPLECTR.bits.depurplectrcr = data;
	pstIspReg->ISP_DEMOSAIC_DEPURPLECTR.u32 = ISP_DEMOSAIC_DEPURPLECTR.u32;
}

static __inline HI_U8 hi_isp_demosaic_depurplectrcr_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_DEPURPLECTR ISP_DEMOSAIC_DEPURPLECTR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_DEPURPLECTR.u32 = pstIspReg->ISP_DEMOSAIC_DEPURPLECTR.u32;
	return ISP_DEMOSAIC_DEPURPLECTR.bits.depurplectrcr;
}


static __inline HI_VOID hi_isp_demosaic_depurplectrcb_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_DEPURPLECTR ISP_DEMOSAIC_DEPURPLECTR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_DEPURPLECTR.u32 = pstIspReg->ISP_DEMOSAIC_DEPURPLECTR.u32;
	ISP_DEMOSAIC_DEPURPLECTR.bits.depurplectrcb = data;
	pstIspReg->ISP_DEMOSAIC_DEPURPLECTR.u32 = ISP_DEMOSAIC_DEPURPLECTR.u32;
}

static __inline HI_U8 hi_isp_demosaic_depurplectrcb_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_DEPURPLECTR ISP_DEMOSAIC_DEPURPLECTR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_DEPURPLECTR.u32 = pstIspReg->ISP_DEMOSAIC_DEPURPLECTR.u32;
	return ISP_DEMOSAIC_DEPURPLECTR.bits.depurplectrcb;
}


/*****************************************************
ISP_DEMOSAIC_HV_POINT_THLD:   
0x113c4050:                     31:01  reserved
                                23:16  tot_point_thld
                                09:00  sig_point_thld
*****************************************************/
#define HI_ISP_DEMOSAIC_TOT_POINT_AVG_THR_DEFAULT (20)
static __inline HI_VOID hi_isp_demosaic_tot_point_thld_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_HV_POINT_THLD ISP_DEMOSAIC_HV_POINT_THLD;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_HV_POINT_THLD.u32 = pstIspReg->ISP_DEMOSAIC_HV_POINT_THLD.u32;
	ISP_DEMOSAIC_HV_POINT_THLD.bits.tot_point_thld = data;
	pstIspReg->ISP_DEMOSAIC_HV_POINT_THLD.u32 = ISP_DEMOSAIC_HV_POINT_THLD.u32;
}

#define HI_ISP_DEMOSAIC_SIG_POINT_AVG_THR_DEFAULT (20)
static __inline HI_VOID hi_isp_demosaic_sig_point_thld_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_HV_POINT_THLD ISP_DEMOSAIC_HV_POINT_THLD;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_HV_POINT_THLD.u32 = pstIspReg->ISP_DEMOSAIC_HV_POINT_THLD.u32;
	ISP_DEMOSAIC_HV_POINT_THLD.bits.sig_point_thld = data;
	pstIspReg->ISP_DEMOSAIC_HV_POINT_THLD.u32 = ISP_DEMOSAIC_HV_POINT_THLD.u32;
}

/*****************************************************
ISP_DEMOSAIC_BLD_ADJUST:     
0x113c4054:                     31:04      reserved
                                03:00      bld_adjust
*****************************************************/
#define HI_ISP_DEMOSAIC_BLEND_ADJUST_DEFAULT (4)
static __inline HI_VOID hi_isp_demosaic_bld_adjust_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_BLD_ADJUST ISP_DEMOSAIC_BLD_ADJUST;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_BLD_ADJUST.u32 = pstIspReg->ISP_DEMOSAIC_BLD_ADJUST.u32;
	ISP_DEMOSAIC_BLD_ADJUST.bits.bld_adjust = data;
	pstIspReg->ISP_DEMOSAIC_BLD_ADJUST.u32 = ISP_DEMOSAIC_BLD_ADJUST.u32;
}


/*****************************************************
ISP_DEMOSAIC_CBCRAVGTHLD:    
0x113c4058:                     31:04      reserved
                                11:00      cbcr_avg_thld
*****************************************************/
#define HI_ISP_DEMOSAIC_CB_CR_AVG_THR_DEFAULT     (150)
static __inline HI_VOID hi_isp_demosaic_cbcr_avg_thld_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_CBCRAVGTHLD ISP_DEMOSAIC_CBCRAVGTHLD;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_CBCRAVGTHLD.u32 = pstIspReg->ISP_DEMOSAIC_CBCRAVGTHLD.u32;
	ISP_DEMOSAIC_CBCRAVGTHLD.bits.cbcr_avg_thld = data;
	pstIspReg->ISP_DEMOSAIC_CBCRAVGTHLD.u32 = ISP_DEMOSAIC_CBCRAVGTHLD.u32;
}

/*****************************************************
ISP_DEMOSAIC_GAPTHLD:    
0x113c405c:                 31:04      reserved
                            27:16      g_gap_thld
                            11:00      cr_gap_thld
*****************************************************/
#define HI_ISP_DEMOSAIC_G_GAP_THR_DEFAULT (100)
static __inline HI_VOID hi_isp_demosaic_g_gap_thld_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_GAPTHLD ISP_DEMOSAIC_GAPTHLD;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_GAPTHLD.u32 = pstIspReg->ISP_DEMOSAIC_GAPTHLD.u32;
	ISP_DEMOSAIC_GAPTHLD.bits.g_gap_thld = data;
	pstIspReg->ISP_DEMOSAIC_GAPTHLD.u32 = ISP_DEMOSAIC_GAPTHLD.u32;
}

#define HI_ISP_DEMOSAIC_CR_GAP_THR_DEFAULT   (200)
static __inline HI_VOID hi_isp_demosaic_cr_gap_thld_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_GAPTHLD ISP_DEMOSAIC_GAPTHLD;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_GAPTHLD.u32 = pstIspReg->ISP_DEMOSAIC_GAPTHLD.u32;
	ISP_DEMOSAIC_GAPTHLD.bits.cr_gap_thld = data;
	pstIspReg->ISP_DEMOSAIC_GAPTHLD.u32 = ISP_DEMOSAIC_GAPTHLD.u32;
}

/*****************************************************
ISP_DEMOSAIC_COLORTHLD:    
0x113c4060:                 31:04      reserved
                            11:00      colorthld
*****************************************************/
#define HI_ISP_DEMOSAIC_COLOR_THR_DEFAULT    (50)
static __inline HI_VOID hi_isp_demosaic_colorthld_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_COLORTHLD ISP_DEMOSAIC_COLORTHLD;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_COLORTHLD.u32 = pstIspReg->ISP_DEMOSAIC_COLORTHLD.u32;
	ISP_DEMOSAIC_COLORTHLD.bits.colorthld = data;
	pstIspReg->ISP_DEMOSAIC_COLORTHLD.u32 = ISP_DEMOSAIC_COLORTHLD.u32;
}

/*****************************************************
ISP_DEMOSAIC_Gvar_Blend_THLD:   
0x113c4064:                 31:04      reserved
                            11:00      VarThrForBlend
*****************************************************/
#define HI_ISP_DEMOSAIC_GCAC_BLEND_THR_DEFAULT (130)
static __inline HI_VOID hi_isp_demosaic_varthrforblend_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_GVAR_BLEND_THLD ISP_DEMOSAIC_GVAR_BLEND_THLD;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_GVAR_BLEND_THLD.u32 = pstIspReg->ISP_DEMOSAIC_GVAR_BLEND_THLD.u32;
	ISP_DEMOSAIC_GVAR_BLEND_THLD.bits.varthrforblend = data;
	pstIspReg->ISP_DEMOSAIC_GVAR_BLEND_THLD.u32 = ISP_DEMOSAIC_GVAR_BLEND_THLD.u32;
}

static __inline HI_U16 hi_isp_demosaic_varthrforblend_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_GVAR_BLEND_THLD ISP_DEMOSAIC_GVAR_BLEND_THLD;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_GVAR_BLEND_THLD.u32 = pstIspReg->ISP_DEMOSAIC_GVAR_BLEND_THLD.u32;
	return ISP_DEMOSAIC_GVAR_BLEND_THLD.bits.varthrforblend;
}


/*****************************************************
ISP_DEMOSAIC_SATU_THR:          
0x113c4068:                 31:04      reserved
                            11:00      satu_thr
*****************************************************/
#define HI_ISP_LCAC_SATU_THR_DEFAULT (128)
static __inline HI_VOID hi_isp_demosaic_satu_thr_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_SATU_THR ISP_DEMOSAIC_SATU_THR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_SATU_THR.u32 = pstIspReg->ISP_DEMOSAIC_SATU_THR.u32;
	ISP_DEMOSAIC_SATU_THR.bits.satu_thr = data;
	pstIspReg->ISP_DEMOSAIC_SATU_THR.u32 = ISP_DEMOSAIC_SATU_THR.u32;
}

static __inline HI_U16 hi_isp_demosaic_satu_thr_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_SATU_THR ISP_DEMOSAIC_SATU_THR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_SATU_THR.u32 = pstIspReg->ISP_DEMOSAIC_SATU_THR.u32;
	return ISP_DEMOSAIC_SATU_THR.bits.satu_thr;
}


/*****************************************************
U_ISP_DEMOSAIC_CBCR_RATIO_LIMIT:         
0x113c406c:              31:04      reserved
					cbcr_ratio_low_limit  : 12 
					cbcr_ratio_high_limit : 12 
*****************************************************/
static __inline HI_VOID hi_isp_demosaic_cbcr_ratio_lmt_low_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_CBCR_RATIO_LIMIT ISP_DEMOSAIC_CBCR_RATIO_LIMIT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_CBCR_RATIO_LIMIT.u32 = pstIspReg->ISP_DEMOSAIC_CBCR_RATIO_LIMIT.u32;
	ISP_DEMOSAIC_CBCR_RATIO_LIMIT.bits.cbcr_ratio_low_limit = data;
	pstIspReg->ISP_DEMOSAIC_CBCR_RATIO_LIMIT.u32 = ISP_DEMOSAIC_CBCR_RATIO_LIMIT.u32;
}

static __inline HI_U16 hi_isp_demosaic_cbcr_ratio_lmt_low_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_CBCR_RATIO_LIMIT ISP_DEMOSAIC_CBCR_RATIO_LIMIT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_CBCR_RATIO_LIMIT.u32 = pstIspReg->ISP_DEMOSAIC_CBCR_RATIO_LIMIT.u32;
	return ISP_DEMOSAIC_CBCR_RATIO_LIMIT.bits.cbcr_ratio_low_limit;
}


static __inline HI_VOID hi_isp_demosaic_cbcr_ratio_lmt_high_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_CBCR_RATIO_LIMIT ISP_DEMOSAIC_CBCR_RATIO_LIMIT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_CBCR_RATIO_LIMIT.u32 = pstIspReg->ISP_DEMOSAIC_CBCR_RATIO_LIMIT.u32;
	ISP_DEMOSAIC_CBCR_RATIO_LIMIT.bits.cbcr_ratio_high_limit = data;
	pstIspReg->ISP_DEMOSAIC_CBCR_RATIO_LIMIT.u32 = ISP_DEMOSAIC_CBCR_RATIO_LIMIT.u32;
}

static __inline HI_U16 hi_isp_demosaic_cbcr_ratio_lmt_high_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_CBCR_RATIO_LIMIT ISP_DEMOSAIC_CBCR_RATIO_LIMIT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_CBCR_RATIO_LIMIT.u32 = pstIspReg->ISP_DEMOSAIC_CBCR_RATIO_LIMIT.u32;
	return ISP_DEMOSAIC_CBCR_RATIO_LIMIT.bits.cbcr_ratio_high_limit;
}

/*****************************************************
U_ISP_DEMOSAIC_FCR_GRAY_RATIO:          
0x113c4070:              31:04      reserved
					fcr_gray_ratio : 5
*****************************************************/
#define HI_ISP_DEMOSAIC_FCRGRAYRATIO_DEFAULT (4)
static __inline HI_VOID hi_isp_demosaic_fcr_gray_ratio_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_FCR_GRAY_RATIO ISP_DEMOSAIC_FCR_GRAY_RATIO;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_FCR_GRAY_RATIO.u32 = pstIspReg->ISP_DEMOSAIC_FCR_GRAY_RATIO.u32;
	ISP_DEMOSAIC_FCR_GRAY_RATIO.bits.fcr_gray_ratio = data;
	pstIspReg->ISP_DEMOSAIC_FCR_GRAY_RATIO.u32 = ISP_DEMOSAIC_FCR_GRAY_RATIO.u32;
}

/*****************************************************
ISP_DEMOSAIC_FCR_SEL:          
0x113c4074:              31:04      reserved
					fcr_gray_ratio : 5
*****************************************************/
#define HI_ISP_DEMOSAIC_FCR_CMAX_SEL_DEFAULT (2)
#define HI_ISP_DEMOSAIC_FCR_DETG_SEL_DEFAULT (0)
static __inline HI_VOID hi_isp_demosaic_fcr_cmax_sel_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_FCR_SEL ISP_DEMOSAIC_FCR_SEL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_FCR_SEL.u32 = pstIspReg->ISP_DEMOSAIC_FCR_SEL.u32;
	ISP_DEMOSAIC_FCR_SEL.bits.fcr_cmax_sel = data;
	pstIspReg->ISP_DEMOSAIC_FCR_SEL.u32 = ISP_DEMOSAIC_FCR_SEL.u32;
}


static __inline HI_U8 hi_isp_demosaic_fcr_cmax_sel_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_FCR_SEL ISP_DEMOSAIC_FCR_SEL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_FCR_SEL.u32 = pstIspReg->ISP_DEMOSAIC_FCR_SEL.u32;
	return ISP_DEMOSAIC_FCR_SEL.bits.fcr_cmax_sel ;
}


static __inline HI_VOID hi_isp_demosaic_fcr_detg_sel_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_FCR_SEL ISP_DEMOSAIC_FCR_SEL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_FCR_SEL.u32 = pstIspReg->ISP_DEMOSAIC_FCR_SEL.u32;
	ISP_DEMOSAIC_FCR_SEL.bits.fcr_detg_sel = data;
	pstIspReg->ISP_DEMOSAIC_FCR_SEL.u32 = ISP_DEMOSAIC_FCR_SEL.u32;
}

static __inline HI_U8 hi_isp_demosaic_fcr_detg_sel_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_FCR_SEL ISP_DEMOSAIC_FCR_SEL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_FCR_SEL.u32 = pstIspReg->ISP_DEMOSAIC_FCR_SEL.u32;
	return ISP_DEMOSAIC_FCR_SEL.bits.fcr_detg_sel ;
}



/*****************************************************
ISP_DEMOSAIC_FCR_SEL:     
0x113c4074:              31:04      reserved
					fcr_gray_ratio : 5
*****************************************************/
#define HI_ISP_DEMOSAIC_VARBLENDTHR_DEFAULT (0)
static __inline HI_VOID hi_isp_demosaic_var_blend_th_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_BLEND_RATIO ISP_DEMOSAIC_BLEND_RATIO;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_BLEND_RATIO.u32 = pstIspReg->ISP_DEMOSAIC_BLEND_RATIO.u32;
	ISP_DEMOSAIC_BLEND_RATIO.bits.var_blend_th = data;
	pstIspReg->ISP_DEMOSAIC_BLEND_RATIO.u32 = ISP_DEMOSAIC_BLEND_RATIO.u32;

}
static __inline HI_U16 hi_isp_demosaic_var_blend_th_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_BLEND_RATIO ISP_DEMOSAIC_BLEND_RATIO;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_BLEND_RATIO.u32 = pstIspReg->ISP_DEMOSAIC_BLEND_RATIO.u32;
	return ISP_DEMOSAIC_BLEND_RATIO.bits.var_blend_th ;
}


#define HI_ISP_DEMOSAIC_DIF_BLEND_RATIO0_DEFAULT (0)
static __inline HI_VOID hi_isp_demosaic_dif_blend_ratio1_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_BLEND_RATIO ISP_DEMOSAIC_BLEND_RATIO;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_BLEND_RATIO.u32 = pstIspReg->ISP_DEMOSAIC_BLEND_RATIO.u32;
	ISP_DEMOSAIC_BLEND_RATIO.bits.dif_blend_ratio1 = data;
	pstIspReg->ISP_DEMOSAIC_BLEND_RATIO.u32 = ISP_DEMOSAIC_BLEND_RATIO.u32;
}
#define HI_ISP_DEMOSAIC_DIF_BLEND_RATIO1_DEFAULT (256)
static __inline HI_VOID hi_isp_demosaic_dif_blend_ratio0_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_BLEND_RATIO ISP_DEMOSAIC_BLEND_RATIO;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_BLEND_RATIO.u32 = pstIspReg->ISP_DEMOSAIC_BLEND_RATIO.u32;
	ISP_DEMOSAIC_BLEND_RATIO.bits.dif_blend_ratio0 = data;
	pstIspReg->ISP_DEMOSAIC_BLEND_RATIO.u32 = ISP_DEMOSAIC_BLEND_RATIO.u32;
}

/*****************************************************
ISP_DEMOSAIC_FCR_SEL:     
0x113c4074:              31:04      reserved
					fcr_gray_ratio : 5
*****************************************************/
#define HI_ISP_DEMOSAIC_DIF_BLEND_LIMIT0_DEFAULT (20)
static __inline HI_VOID hi_isp_demosaic_dif_blend_limit1_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_BLEND_DIF_LIMIT ISP_DEMOSAIC_BLEND_DIF_LIMIT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_BLEND_DIF_LIMIT.u32 = pstIspReg->ISP_DEMOSAIC_BLEND_DIF_LIMIT.u32;
	ISP_DEMOSAIC_BLEND_DIF_LIMIT.bits.dif_blend_limit1 = data;
	pstIspReg->ISP_DEMOSAIC_BLEND_DIF_LIMIT.u32 = ISP_DEMOSAIC_BLEND_DIF_LIMIT.u32;

}
static __inline HI_U8 hi_isp_demosaic_dif_blend_limit1_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_BLEND_DIF_LIMIT ISP_DEMOSAIC_BLEND_DIF_LIMIT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_BLEND_DIF_LIMIT.u32 = pstIspReg->ISP_DEMOSAIC_BLEND_DIF_LIMIT.u32;
	return ISP_DEMOSAIC_BLEND_DIF_LIMIT.bits.dif_blend_limit1 ;
}


#define HI_ISP_DEMOSAIC_DIF_BLEND_LIMIT1_DEFAULT (32)
static __inline HI_VOID hi_isp_demosaic_dif_blend_limit0_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_BLEND_DIF_LIMIT ISP_DEMOSAIC_BLEND_DIF_LIMIT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_BLEND_DIF_LIMIT.u32 = pstIspReg->ISP_DEMOSAIC_BLEND_DIF_LIMIT.u32;
	ISP_DEMOSAIC_BLEND_DIF_LIMIT.bits.dif_blend_limit0 = data;
	pstIspReg->ISP_DEMOSAIC_BLEND_DIF_LIMIT.u32 = ISP_DEMOSAIC_BLEND_DIF_LIMIT.u32;
}
static __inline HI_U8 hi_isp_demosaic_dif_blend_limit0_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_BLEND_DIF_LIMIT ISP_DEMOSAIC_BLEND_DIF_LIMIT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_BLEND_DIF_LIMIT.u32 = pstIspReg->ISP_DEMOSAIC_BLEND_DIF_LIMIT.u32;
	return ISP_DEMOSAIC_BLEND_DIF_LIMIT.bits.dif_blend_limit0 ;
}


#define HI_ISP_DEMOSAIC_DIF2MAX_RES_TH_DEFAULT (100)
static __inline HI_VOID hi_isp_demosaic_dif2max_res_th_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_BLEND_DIF_LIMIT ISP_DEMOSAIC_BLEND_DIF_LIMIT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_BLEND_DIF_LIMIT.u32 = pstIspReg->ISP_DEMOSAIC_BLEND_DIF_LIMIT.u32;
	ISP_DEMOSAIC_BLEND_DIF_LIMIT.bits.dif2max_res_th = data;
	pstIspReg->ISP_DEMOSAIC_BLEND_DIF_LIMIT.u32 = ISP_DEMOSAIC_BLEND_DIF_LIMIT.u32;
}

static __inline HI_U16 hi_isp_demosaic_dif2max_res_th_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_BLEND_DIF_LIMIT ISP_DEMOSAIC_BLEND_DIF_LIMIT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_BLEND_DIF_LIMIT.u32 = pstIspReg->ISP_DEMOSAIC_BLEND_DIF_LIMIT.u32;
	return ISP_DEMOSAIC_BLEND_DIF_LIMIT.bits.dif2max_res_th ;
}
#define HI_ISP_DEMOSAIC_HF_INTP_TH_LOW_DEFAULT (16)
static __inline HI_VOID hi_isp_demosaic_hf_intp_th_low_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_HF_INTP_TH ISP_DEMOSAIC_HF_INTP_TH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_HF_INTP_TH.u32 = pstIspReg->ISP_DEMOSAIC_HF_INTP_TH.u32;
	ISP_DEMOSAIC_HF_INTP_TH.bits.hf_intp_th_low = data;
	pstIspReg->ISP_DEMOSAIC_HF_INTP_TH.u32 = ISP_DEMOSAIC_HF_INTP_TH.u32;
}

static __inline HI_U16 hi_isp_demosaic_hf_intp_th_low_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_HF_INTP_TH ISP_DEMOSAIC_HF_INTP_TH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_HF_INTP_TH.u32 = pstIspReg->ISP_DEMOSAIC_HF_INTP_TH.u32;
	
	return (HI_U16)(ISP_DEMOSAIC_HF_INTP_TH.bits.hf_intp_th_low);
}

#define HI_ISP_DEMOSAIC_HF_INTP_TH_HIGH_DEFAULT (48)
static __inline HI_VOID hi_isp_demosaic_hf_intp_th_high_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_HF_INTP_TH ISP_DEMOSAIC_HF_INTP_TH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_HF_INTP_TH.u32 = pstIspReg->ISP_DEMOSAIC_HF_INTP_TH.u32;
	ISP_DEMOSAIC_HF_INTP_TH.bits.hf_intp_th_high = data;
	pstIspReg->ISP_DEMOSAIC_HF_INTP_TH.u32 = ISP_DEMOSAIC_HF_INTP_TH.u32;
}

static __inline HI_U16 hi_isp_demosaic_hf_intp_th_high_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_HF_INTP_TH ISP_DEMOSAIC_HF_INTP_TH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_HF_INTP_TH.u32 = pstIspReg->ISP_DEMOSAIC_HF_INTP_TH.u32;
	
	return (HI_U16)(ISP_DEMOSAIC_HF_INTP_TH.bits.hf_intp_th_high);
}

#define HI_ISP_DEMOSAIC_HF_INTP_BLD_LOW_DEFAULT (0x96)
static __inline HI_VOID hi_isp_demosaic_hf_intp_bld_low_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_HF_INTP_BLD ISP_DEMOSAIC_HF_INTP_BLD;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_HF_INTP_BLD.u32 = pstIspReg->ISP_DEMOSAIC_HF_INTP_BLD.u32;
	ISP_DEMOSAIC_HF_INTP_BLD.bits.hf_intp_bld_low = data;
	pstIspReg->ISP_DEMOSAIC_HF_INTP_BLD.u32 = ISP_DEMOSAIC_HF_INTP_BLD.u32;
}

static __inline HI_U16 hi_isp_demosaic_hf_intp_bld_low_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_HF_INTP_BLD ISP_DEMOSAIC_HF_INTP_BLD;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_HF_INTP_BLD.u32 = pstIspReg->ISP_DEMOSAIC_HF_INTP_BLD.u32;
	
	return (HI_U16)(ISP_DEMOSAIC_HF_INTP_BLD.bits.hf_intp_bld_low);
}


#define HI_ISP_DEMOSAIC_HF_INTP_BLD_HIGH_DEFAULT (0x100)
static __inline HI_VOID hi_isp_demosaic_hf_intp_bld_high_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_HF_INTP_BLD ISP_DEMOSAIC_HF_INTP_BLD;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_HF_INTP_BLD.u32 = pstIspReg->ISP_DEMOSAIC_HF_INTP_BLD.u32;
	ISP_DEMOSAIC_HF_INTP_BLD.bits.hf_intp_bld_high = data;
	pstIspReg->ISP_DEMOSAIC_HF_INTP_BLD.u32 = ISP_DEMOSAIC_HF_INTP_BLD.u32;
}

static __inline HI_U16 hi_isp_demosaic_hf_intp_bld_high_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_HF_INTP_BLD ISP_DEMOSAIC_HF_INTP_BLD;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_HF_INTP_BLD.u32 = pstIspReg->ISP_DEMOSAIC_HF_INTP_BLD.u32;
	
	return (HI_U16)(ISP_DEMOSAIC_HF_INTP_BLD.bits.hf_intp_bld_high);
}



#define HI_ISP_DEMOSAIC_HF_INTP_TH_LOW1_LINEAR_DEFAULT (53)
#define HI_ISP_DEMOSAIC_HF_INTP_TH_LOW1_WDR_DEFAULT    (150)
static __inline HI_VOID hi_isp_demosaic_hf_intp_th_low1_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_HF_INTP_TH1 ISP_DEMOSAIC_HF_INTP_TH1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_HF_INTP_TH1.u32 = pstIspReg->ISP_DEMOSAIC_HF_INTP_TH1.u32;
	ISP_DEMOSAIC_HF_INTP_TH1.bits.hf_intp_th_low1 = data;
	pstIspReg->ISP_DEMOSAIC_HF_INTP_TH1.u32 = ISP_DEMOSAIC_HF_INTP_TH1.u32;
}

static __inline HI_U16 hi_isp_demosaic_hf_intp_th_low1_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_HF_INTP_TH1 ISP_DEMOSAIC_HF_INTP_TH1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_HF_INTP_TH1.u32 = pstIspReg->ISP_DEMOSAIC_HF_INTP_TH1.u32;
	
	return (HI_U16)(ISP_DEMOSAIC_HF_INTP_TH1.bits.hf_intp_th_low1);
}

#define HI_ISP_DEMOSAIC_HF_INTP_TH_HIGH1_LINEAR_DEFAULT (100)
#define HI_ISP_DEMOSAIC_HF_INTP_TH_HIGH1_WDR_DEFAULT    (180)
static __inline HI_VOID hi_isp_demosaic_hf_intp_th_high1_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_HF_INTP_TH1 ISP_DEMOSAIC_HF_INTP_TH1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_HF_INTP_TH1.u32 = pstIspReg->ISP_DEMOSAIC_HF_INTP_TH1.u32;
	ISP_DEMOSAIC_HF_INTP_TH1.bits.hf_intp_th_high1 = data;
	pstIspReg->ISP_DEMOSAIC_HF_INTP_TH1.u32 = ISP_DEMOSAIC_HF_INTP_TH1.u32;
}

static __inline HI_U16 hi_isp_demosaic_hf_intp_th_high1_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_HF_INTP_TH1 ISP_DEMOSAIC_HF_INTP_TH1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_HF_INTP_TH1.u32 = pstIspReg->ISP_DEMOSAIC_HF_INTP_TH1.u32;
	
	return (HI_U16)(ISP_DEMOSAIC_HF_INTP_TH1.bits.hf_intp_th_high1);
}

#define HI_ISP_DEMOSAIC_INTERP_RATIO0_DEFAULT    (128)
static __inline HI_VOID hi_isp_demosaic_interp_ratio0_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_HF_INTP_RATIO0 ISP_DEMOSAIC_HF_INTP_RATIO0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_HF_INTP_RATIO0.u32 = pstIspReg->ISP_DEMOSAIC_HF_INTP_RATIO0.u32;
	ISP_DEMOSAIC_HF_INTP_RATIO0.bits.interp_ratio = data;
	pstIspReg->ISP_DEMOSAIC_HF_INTP_RATIO0.u32 = ISP_DEMOSAIC_HF_INTP_RATIO0.u32;
}

#define HI_ISP_DEMOSAIC_INTERP_RATIO1_DEFAULT    (128)
static __inline HI_VOID hi_isp_demosaic_interp_ratio1_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_HF_INTP_RATIO1 ISP_DEMOSAIC_HF_INTP_RATIO1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_HF_INTP_RATIO1.u32 = pstIspReg->ISP_DEMOSAIC_HF_INTP_RATIO1.u32;
	ISP_DEMOSAIC_HF_INTP_RATIO1.bits.interp_ratio = data;
	pstIspReg->ISP_DEMOSAIC_HF_INTP_RATIO0.u32 = ISP_DEMOSAIC_HF_INTP_RATIO1.u32;
}


/*****************************************************
ISP_DEMOSAIC_SIZE:   DEMOSAIC    
0x205a30f0:                     31:29      reserved
                                         28:16      height
                                         15:13      reserved
                                         12:00      width
*****************************************************/
static __inline HI_VOID hi_isp_demosaic_height_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_SIZE ISP_DEMOSAIC_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_SIZE.u32 = pstIspReg->ISP_DEMOSAIC_SIZE.u32;
	ISP_DEMOSAIC_SIZE.bits.height = data;
	pstIspReg->ISP_DEMOSAIC_SIZE.u32 = ISP_DEMOSAIC_SIZE.u32;
}

static __inline HI_VOID hi_isp_demosaic_width_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEMOSAIC_SIZE ISP_DEMOSAIC_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEMOSAIC_SIZE.u32 = pstIspReg->ISP_DEMOSAIC_SIZE.u32;
	ISP_DEMOSAIC_SIZE.bits.width = data;
	pstIspReg->ISP_DEMOSAIC_SIZE.u32 = ISP_DEMOSAIC_SIZE.u32;
}

/*****************************************************
MODULE        -----DG-----
MODULE        -----DG-----
MODULE        -----DG-----
*****************************************************/
/*****************************************************
ISP_DG_CFG:         DG        
0x205a1300:         31:1      reserved
                                   0      en
*****************************************************/
static __inline HI_VOID hi_isp_dg_enable_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DG_CFG ISP_DG_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DG_CFG.u32 = pstIspReg->ISP_DG_CFG.u32;
	ISP_DG_CFG.bits.en = data;
	pstIspReg->ISP_DG_CFG.u32 = ISP_DG_CFG.u32;
}

static __inline HI_U8 hi_isp_dg_enable_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DG_CFG ISP_DG_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DG_CFG.u32 = pstIspReg->ISP_DG_CFG.u32;
	return ISP_DG_CFG.bits.en;
}

/*****************************************************

ISP_DG_BLC_CFG:            DG BLC offset     
0x113a1304:                31:02      reserved


*****************************************************/
static __inline HI_VOID hi_isp_dg_blc_in_enable_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DG_BLC_CFG ISP_DG_BLC_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DG_BLC_CFG.u32 = pstIspReg->ISP_DG_BLC_CFG.u32;
	ISP_DG_BLC_CFG.bits.en_in = data;
	pstIspReg->ISP_DG_BLC_CFG.u32 = ISP_DG_BLC_CFG.u32;
}

static __inline HI_VOID hi_isp_dg_blc_out_enable_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DG_BLC_CFG ISP_DG_BLC_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DG_BLC_CFG.u32 = pstIspReg->ISP_DG_BLC_CFG.u32;
	ISP_DG_BLC_CFG.bits.en_out = data;
	pstIspReg->ISP_DG_BLC_CFG.u32 = ISP_DG_BLC_CFG.u32;
}

/*****************************************************
ISP_DG_GAIN1:              
0x205a1310:                    
                                          31:16      rgain
                                          15:00      grgain
*****************************************************/
static __inline HI_VOID hi_isp_dg_rgain_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DG_GAIN1 ISP_DG_GAIN1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DG_GAIN1.u32 = pstIspReg->ISP_DG_GAIN1.u32;
	ISP_DG_GAIN1.bits.rgain = data;
	pstIspReg->ISP_DG_GAIN1.u32 = ISP_DG_GAIN1.u32;
}

static __inline HI_VOID hi_isp_dg_grgain_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DG_GAIN1 ISP_DG_GAIN1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DG_GAIN1.u32 = pstIspReg->ISP_DG_GAIN1.u32;
	ISP_DG_GAIN1.bits.grgain = data;
	pstIspReg->ISP_DG_GAIN1.u32 = ISP_DG_GAIN1.u32;
}

/*****************************************************
ISP_DG_GAIN2:              
0x205a1314:                     31:28       reserve  
                                          31:16      bgain
                                          15:00      gbgain
*****************************************************/

static __inline HI_VOID hi_isp_dg_bgain_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DG_GAIN2 ISP_DG_GAIN2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DG_GAIN2.u32 = pstIspReg->ISP_DG_GAIN2.u32;
	ISP_DG_GAIN2.bits.bgain = data;
	pstIspReg->ISP_DG_GAIN2.u32 = ISP_DG_GAIN2.u32;
}

static __inline HI_VOID hi_isp_dg_gbgain_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DG_GAIN2 ISP_DG_GAIN2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DG_GAIN2.u32 = pstIspReg->ISP_DG_GAIN2.u32;
	ISP_DG_GAIN2.bits.gbgain = data;
	pstIspReg->ISP_DG_GAIN2.u32 = ISP_DG_GAIN2.u32;
}

/*****************************************************

ISP_WB_BLC_OFFSET1:             BLC offset_Gr
0x205a1218:                30:16     R offset
                           14:0     Gr offset
*****************************************************/
static __inline HI_VOID hi_isp_dg_blc_offset_Gr_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DG_BLC_OFFSET1 ISP_DG_BLC_OFFSET1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DG_BLC_OFFSET1.u32 = pstIspReg->ISP_DG_BLC_OFFSET1.u32;
	ISP_DG_BLC_OFFSET1.bits.ofsgr = data;
	pstIspReg->ISP_DG_BLC_OFFSET1.u32 = ISP_DG_BLC_OFFSET1.u32;
}

static __inline HI_U16 hi_isp_dg_blc_offset_Gr_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DG_BLC_OFFSET1 ISP_DG_BLC_OFFSET1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DG_BLC_OFFSET1.u32 = pstIspReg->ISP_DG_BLC_OFFSET1.u32;
	return ISP_DG_BLC_OFFSET1.bits.ofsgr;
}
static __inline HI_VOID hi_isp_dg_blc_offset_R_write(HI_U8 ispDev, HI_U16 data) {
    S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DG_BLC_OFFSET1 ISP_DG_BLC_OFFSET1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DG_BLC_OFFSET1.u32 = pstIspReg->ISP_DG_BLC_OFFSET1.u32;
	ISP_DG_BLC_OFFSET1.bits.ofsr = data;
	pstIspReg->ISP_DG_BLC_OFFSET1.u32 = ISP_DG_BLC_OFFSET1.u32;
}

static __inline HI_U16 hi_isp_dg_blc_offset_R_read(HI_U8 ispDev) {
    S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DG_BLC_OFFSET1 ISP_DG_BLC_OFFSET1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DG_BLC_OFFSET1.u32 = pstIspReg->ISP_DG_BLC_OFFSET1.u32;
	return ISP_DG_BLC_OFFSET1.bits.ofsr;
}
/*****************************************************

ISP_WB_BLC_OFFSET2:             BLC offset_Gr
0x205a121c:                30:16     B offset
                           14:0     Gb offset
*****************************************************/
static __inline HI_VOID hi_isp_dg_blc_offset_Gb_write(HI_U8 ispDev, HI_U16 data) {
    S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DG_BLC_OFFSET2 ISP_DG_BLC_OFFSET2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DG_BLC_OFFSET2.u32 = pstIspReg->ISP_DG_BLC_OFFSET2.u32;
	ISP_DG_BLC_OFFSET2.bits.ofsgb = data;
	pstIspReg->ISP_DG_BLC_OFFSET2.u32 = ISP_DG_BLC_OFFSET2.u32;
}

static __inline HI_U16 hi_isp_dg_blc_offset_Gb_read(HI_U8 ispDev) {
    S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DG_BLC_OFFSET2 ISP_DG_BLC_OFFSET2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DG_BLC_OFFSET2.u32 = pstIspReg->ISP_DG_BLC_OFFSET2.u32;
	return ISP_DG_BLC_OFFSET2.bits.ofsgb;
}
static __inline HI_VOID hi_isp_dg_blc_offset_B_write(HI_U8 ispDev, HI_U16 data) {
    S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DG_BLC_OFFSET2 ISP_DG_BLC_OFFSET2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DG_BLC_OFFSET2.u32 = pstIspReg->ISP_DG_BLC_OFFSET2.u32;
	ISP_DG_BLC_OFFSET2.bits.ofsb = data;
	pstIspReg->ISP_DG_BLC_OFFSET2.u32 = ISP_DG_BLC_OFFSET2.u32;
}

static __inline HI_U16 hi_isp_dg_blc_offset_B_read(HI_U8 ispDev) {
    S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DG_BLC_OFFSET2 ISP_DG_BLC_OFFSET2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DG_BLC_OFFSET2.u32 = pstIspReg->ISP_DG_BLC_OFFSET2.u32;
	return ISP_DG_BLC_OFFSET2.bits.ofsb;
}

/*****************************************************
MODULE        -----4DG-----
MODULE        -----4DG-----
MODULE        -----4DG-----
*****************************************************/
/*****************************************************
ISP_4DG_CFG:         4DG
0x205a1C00:         31:1      reserved
                                   0      en
*****************************************************/
static __inline HI_VOID hi_isp_4dg_enable_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_4DG_CFG ISP_4DG_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_4DG_CFG.u32 = pstIspReg->ISP_4DG_CFG.u32;
	ISP_4DG_CFG.bits.en = data;
	pstIspReg->ISP_4DG_CFG.u32 = ISP_4DG_CFG.u32;
}

static __inline HI_U8 hi_isp_4dg_enable_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_4DG_CFG ISP_4DG_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_4DG_CFG.u32 = pstIspReg->ISP_4DG_CFG.u32;
	return ISP_4DG_CFG.bits.en;
}

/*****************************************************

ISP_4DG_BLC_CFG:            4DG BLC offset
0x113a1304:                31:02      reserved
                           01:01     en_out 
                           00:00     en_in  
*****************************************************/
static __inline HI_VOID hi_isp_4dg_blc_in_enable_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_4DG_BLC_CFG ISP_4DG_BLC_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_4DG_BLC_CFG.u32 = pstIspReg->ISP_4DG_BLC_CFG.u32;
	ISP_4DG_BLC_CFG.bits.en_in = data;
	pstIspReg->ISP_4DG_BLC_CFG.u32 = ISP_4DG_BLC_CFG.u32;
}

static __inline HI_VOID hi_isp_4dg_blc_out_enable_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_4DG_BLC_CFG ISP_4DG_BLC_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_4DG_BLC_CFG.u32 = pstIspReg->ISP_4DG_BLC_CFG.u32;
	ISP_4DG_BLC_CFG.bits.en_out = data;
	pstIspReg->ISP_4DG_BLC_CFG.u32 = ISP_4DG_BLC_CFG.u32;
}

/*****************************************************
ISP_4DG_0_GAIN1:         
0x205a1310:                    
                                          31:16      rgain
                                          15:00      grgain
*****************************************************/
static __inline HI_VOID hi_isp_4dg_0_rgain_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_4DG_0_GAIN1 ISP_4DG_0_GAIN1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_4DG_0_GAIN1.u32 = pstIspReg->ISP_4DG_0_GAIN1.u32;
	ISP_4DG_0_GAIN1.bits.rgain = data;
	pstIspReg->ISP_4DG_0_GAIN1.u32 = ISP_4DG_0_GAIN1.u32;
}

static __inline HI_VOID hi_isp_4dg_0_grgain_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_4DG_0_GAIN1 ISP_4DG_0_GAIN1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_4DG_0_GAIN1.u32 = pstIspReg->ISP_4DG_0_GAIN1.u32;
	ISP_4DG_0_GAIN1.bits.grgain = data;
	pstIspReg->ISP_4DG_0_GAIN1.u32 = ISP_4DG_0_GAIN1.u32;
}

/*****************************************************
ISP_4DG_0_GAIN2:         
0x205a1314:                     31:28      
                                          31:16      bgain
                                          15:00      gbgain
*****************************************************/

static __inline HI_VOID hi_isp_4dg_0_bgain_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_4DG_0_GAIN2 ISP_4DG_0_GAIN2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_4DG_0_GAIN2.u32 = pstIspReg->ISP_4DG_0_GAIN2.u32;
	ISP_4DG_0_GAIN2.bits.bgain = data;
	pstIspReg->ISP_4DG_0_GAIN2.u32 = ISP_4DG_0_GAIN2.u32;
}

static __inline HI_VOID hi_isp_4dg_0_gbgain_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_4DG_0_GAIN2 ISP_4DG_0_GAIN2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_4DG_0_GAIN2.u32 = pstIspReg->ISP_4DG_0_GAIN2.u32;
	ISP_4DG_0_GAIN2.bits.gbgain = data;
	pstIspReg->ISP_4DG_0_GAIN2.u32 = ISP_4DG_0_GAIN2.u32;
}

/*****************************************************

ISP_WB_BLC_OFFSET1:             BLC offset_Gr
0x205a1218:                30:16     R offset
                           14:0     Gr offset
*****************************************************/
static __inline HI_VOID hi_isp_4dg_0_blc_offset_Gr_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_4DG_0_BLC_OFFSET1 ISP_4DG_0_BLC_OFFSET1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_4DG_0_BLC_OFFSET1.u32 = pstIspReg->ISP_4DG_0_BLC_OFFSET1.u32;
	ISP_4DG_0_BLC_OFFSET1.bits.ofsgr = data;
	pstIspReg->ISP_4DG_0_BLC_OFFSET1.u32 = ISP_4DG_0_BLC_OFFSET1.u32;
}

static __inline HI_U16 hi_isp_4dg_0_blc_offset_Gr_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_4DG_0_BLC_OFFSET1 ISP_4DG_0_BLC_OFFSET1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_4DG_0_BLC_OFFSET1.u32 = pstIspReg->ISP_4DG_0_BLC_OFFSET1.u32;
	return ISP_4DG_0_BLC_OFFSET1.bits.ofsgr;
}
static __inline HI_VOID hi_isp_4dg_0_blc_offset_R_write(HI_U8 ispDev, HI_U16 data) {
    S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_4DG_0_BLC_OFFSET1 ISP_4DG_0_BLC_OFFSET1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_4DG_0_BLC_OFFSET1.u32 = pstIspReg->ISP_4DG_0_BLC_OFFSET1.u32;
	ISP_4DG_0_BLC_OFFSET1.bits.ofsr = data;
	pstIspReg->ISP_4DG_0_BLC_OFFSET1.u32 = ISP_4DG_0_BLC_OFFSET1.u32;
}

static __inline HI_U16 hi_isp_4dg_0_blc_offset_R_read(HI_U8 ispDev) {
    S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_4DG_0_BLC_OFFSET1 ISP_4DG_0_BLC_OFFSET1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_4DG_0_BLC_OFFSET1.u32 = pstIspReg->ISP_4DG_0_BLC_OFFSET1.u32;
	return ISP_4DG_0_BLC_OFFSET1.bits.ofsr;
}
/*****************************************************

ISP_WB_BLC_OFFSET2:             BLC offset_Gr
0x205a121c:                30:16     B offset
                           14:0     Gb offset
*****************************************************/
static __inline HI_VOID hi_isp_4dg_0_blc_offset_Gb_write(HI_U8 ispDev, HI_U16 data) {
    S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_4DG_0_BLC_OFFSET2 ISP_4DG_0_BLC_OFFSET2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_4DG_0_BLC_OFFSET2.u32 = pstIspReg->ISP_4DG_0_BLC_OFFSET2.u32;
	ISP_4DG_0_BLC_OFFSET2.bits.ofsgb = data;
	pstIspReg->ISP_4DG_0_BLC_OFFSET2.u32 = ISP_4DG_0_BLC_OFFSET2.u32;
}

static __inline HI_U16 hi_isp_4dg_0_blc_offset_Gb_read(HI_U8 ispDev) {
    S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_4DG_0_BLC_OFFSET2 ISP_4DG_0_BLC_OFFSET2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_4DG_0_BLC_OFFSET2.u32 = pstIspReg->ISP_4DG_0_BLC_OFFSET2.u32;
	return ISP_4DG_0_BLC_OFFSET2.bits.ofsgb;
}
static __inline HI_VOID hi_isp_4dg_0_blc_offset_B_write(HI_U8 ispDev, HI_U16 data) {
    S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_4DG_0_BLC_OFFSET2 ISP_4DG_0_BLC_OFFSET2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_4DG_0_BLC_OFFSET2.u32 = pstIspReg->ISP_4DG_0_BLC_OFFSET2.u32;
	ISP_4DG_0_BLC_OFFSET2.bits.ofsb = data;
	pstIspReg->ISP_4DG_0_BLC_OFFSET2.u32 = ISP_4DG_0_BLC_OFFSET2.u32;
}

static __inline HI_U16 hi_isp_4dg_0_blc_offset_B_read(HI_U8 ispDev) {
    S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_4DG_0_BLC_OFFSET2 ISP_4DG_0_BLC_OFFSET2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_4DG_0_BLC_OFFSET2.u32 = pstIspReg->ISP_4DG_0_BLC_OFFSET2.u32;
	return ISP_4DG_0_BLC_OFFSET2.bits.ofsb;
}

/*****************************************************
ISP_4DG_1_GAIN1:         
0x205a1310:                    
                                          31:16      rgain
                                          15:00      grgain
*****************************************************/
static __inline HI_VOID hi_isp_4dg_1_rgain_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_4DG_1_GAIN1 ISP_4DG_1_GAIN1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_4DG_1_GAIN1.u32 = pstIspReg->ISP_4DG_1_GAIN1.u32;
	ISP_4DG_1_GAIN1.bits.rgain = data;
	pstIspReg->ISP_4DG_1_GAIN1.u32 = ISP_4DG_1_GAIN1.u32;
}

static __inline HI_VOID hi_isp_4dg_1_grgain_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_4DG_1_GAIN1 ISP_4DG_1_GAIN1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_4DG_1_GAIN1.u32 = pstIspReg->ISP_4DG_1_GAIN1.u32;
	ISP_4DG_1_GAIN1.bits.grgain = data;
	pstIspReg->ISP_4DG_1_GAIN1.u32 = ISP_4DG_1_GAIN1.u32;
}

/*****************************************************
ISP_4DG_1_GAIN2:         
0x205a1314:                     31:28      
                                          31:16      bgain
                                          15:00      gbgain
*****************************************************/

static __inline HI_VOID hi_isp_4dg_1_bgain_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_4DG_1_GAIN2 ISP_4DG_1_GAIN2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_4DG_1_GAIN2.u32 = pstIspReg->ISP_4DG_1_GAIN2.u32;
	ISP_4DG_1_GAIN2.bits.bgain = data;
	pstIspReg->ISP_4DG_1_GAIN2.u32 = ISP_4DG_1_GAIN2.u32;
}

static __inline HI_VOID hi_isp_4dg_1_gbgain_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_4DG_1_GAIN2 ISP_4DG_1_GAIN2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_4DG_1_GAIN2.u32 = pstIspReg->ISP_4DG_1_GAIN2.u32;
	ISP_4DG_1_GAIN2.bits.gbgain = data;
	pstIspReg->ISP_4DG_1_GAIN2.u32 = ISP_4DG_1_GAIN2.u32;
}

/*****************************************************

ISP_4DG_BLC_OFFSET1:             BLC offset_Gr
0x205a1218:                30:16     R offset
                           14:0     Gr offset
*****************************************************/
static __inline HI_VOID hi_isp_4dg_1_blc_offset_Gr_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_4DG_1_BLC_OFFSET1 ISP_4DG_1_BLC_OFFSET1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_4DG_1_BLC_OFFSET1.u32 = pstIspReg->ISP_4DG_1_BLC_OFFSET1.u32;
	ISP_4DG_1_BLC_OFFSET1.bits.ofsgr = data;
	pstIspReg->ISP_4DG_1_BLC_OFFSET1.u32 = ISP_4DG_1_BLC_OFFSET1.u32;
}

static __inline HI_U16 hi_isp_4dg_1_blc_offset_Gr_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_4DG_1_BLC_OFFSET1 ISP_4DG_1_BLC_OFFSET1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_4DG_1_BLC_OFFSET1.u32 = pstIspReg->ISP_4DG_1_BLC_OFFSET1.u32;
	return ISP_4DG_1_BLC_OFFSET1.bits.ofsgr;
}
static __inline HI_VOID hi_isp_4dg_1_blc_offset_R_write(HI_U8 ispDev, HI_U16 data) {
    S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_4DG_1_BLC_OFFSET1 ISP_4DG_1_BLC_OFFSET1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_4DG_1_BLC_OFFSET1.u32 = pstIspReg->ISP_4DG_1_BLC_OFFSET1.u32;
	ISP_4DG_1_BLC_OFFSET1.bits.ofsr = data;
	pstIspReg->ISP_4DG_1_BLC_OFFSET1.u32 = ISP_4DG_1_BLC_OFFSET1.u32;
}

static __inline HI_U16 hi_isp_4dg_1_blc_offset_R_read(HI_U8 ispDev) {
    S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_4DG_1_BLC_OFFSET1 ISP_4DG_1_BLC_OFFSET1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_4DG_1_BLC_OFFSET1.u32 = pstIspReg->ISP_4DG_1_BLC_OFFSET1.u32;
	return ISP_4DG_1_BLC_OFFSET1.bits.ofsr;
}
/*****************************************************

ISP_4DG_BLC_OFFSET2:             BLC offset_Gr
0x205a121c:                30:16     B offset
                           14:0     Gb offset
*****************************************************/
static __inline HI_VOID hi_isp_4dg_1_blc_offset_Gb_write(HI_U8 ispDev, HI_U16 data) {
    S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_4DG_1_BLC_OFFSET2 ISP_4DG_1_BLC_OFFSET2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_4DG_1_BLC_OFFSET2.u32 = pstIspReg->ISP_4DG_1_BLC_OFFSET2.u32;
	ISP_4DG_1_BLC_OFFSET2.bits.ofsgb = data;
	pstIspReg->ISP_4DG_1_BLC_OFFSET2.u32 = ISP_4DG_1_BLC_OFFSET2.u32;
}

static __inline HI_U16 hi_isp_4dg_1_blc_offset_Gb_read(HI_U8 ispDev) {
    S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_4DG_1_BLC_OFFSET2 ISP_4DG_1_BLC_OFFSET2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_4DG_1_BLC_OFFSET2.u32 = pstIspReg->ISP_4DG_1_BLC_OFFSET2.u32;
	return ISP_4DG_1_BLC_OFFSET2.bits.ofsgb;
}
static __inline HI_VOID hi_isp_4dg_1_blc_offset_B_write(HI_U8 ispDev, HI_U16 data) {
    S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_4DG_1_BLC_OFFSET2 ISP_4DG_1_BLC_OFFSET2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_4DG_1_BLC_OFFSET2.u32 = pstIspReg->ISP_4DG_1_BLC_OFFSET2.u32;
	ISP_4DG_1_BLC_OFFSET2.bits.ofsb = data;
	pstIspReg->ISP_4DG_1_BLC_OFFSET2.u32 = ISP_4DG_1_BLC_OFFSET2.u32;
}

static __inline HI_U16 hi_isp_4dg_1_blc_offset_B_read(HI_U8 ispDev) {
    S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_4DG_1_BLC_OFFSET2 ISP_4DG_1_BLC_OFFSET2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_4DG_1_BLC_OFFSET2.u32 = pstIspReg->ISP_4DG_1_BLC_OFFSET2.u32;
	return ISP_4DG_1_BLC_OFFSET2.bits.ofsb;
}

/*****************************************************
MODULE        -----DPC-----
MODULE        -----DPC-----
MODULE        -----DPC-----
*****************************************************/
/*****************************************************
ISP_DPC_CFG:         DPC        
0x205a3800:           31:1    reserved
*****************************************************/
static __inline HI_VOID hi_isp_dpc_enable_write(HI_U8 ispDev, HI_U8 chn, HI_U8 data) {
    S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_CFG ISP_DPC_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_CFG.u32 = pstIspReg->ISP_DPC_CFG.u32;

	switch (chn)
	{
		case 0:
	ISP_DPC_CFG.bits.en0 = data;
	pstIspReg->ISP_DPC_CFG.u32 = ISP_DPC_CFG.u32;
			break;
		case 1:
	ISP_DPC_CFG.bits.en1 = data;
	pstIspReg->ISP_DPC_CFG.u32 = ISP_DPC_CFG.u32;
			break;
		case 2:
	ISP_DPC_CFG.bits.en2 = data;
	pstIspReg->ISP_DPC_CFG.u32 = ISP_DPC_CFG.u32;
			break;
		case 3:
	ISP_DPC_CFG.bits.en3 = data;
	pstIspReg->ISP_DPC_CFG.u32 = ISP_DPC_CFG.u32;
			break;
		default:
			break;			
	}
	
	return;
}

static __inline HI_U8 hi_isp_dpc_enable_read(HI_U8 ispDev, HI_U8 chn) {
    S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_CFG ISP_DPC_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_CFG.u32 = pstIspReg->ISP_DPC_CFG.u32;
	
	switch (chn)
	{
		case 0:
			return ISP_DPC_CFG.bits.en0;
		case 1:
			return ISP_DPC_CFG.bits.en1;
		case 2:
			return ISP_DPC_CFG.bits.en2;
		case 3:
			return ISP_DPC_CFG.bits.en3;
		default:
			return 0;			
	}
}

/*****************************************************
ISP_DCG_MODE:    DGC

*****************************************************/
#define HI_ISP_DCG_MODE_DEFAULT (0)
static __inline HI_VOID hi_isp_dcg_mode_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DCG_MODE ISP_DCG_MODE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DCG_MODE.u32 = pstIspReg->ISP_DCG_MODE.u32;
	ISP_DCG_MODE.bits.mode = data;
	pstIspReg->ISP_DCG_MODE.u32 = ISP_DCG_MODE.u32;
}


/*****************************************************
ISP_DPC_BLEND:    DPC 
0x205a3808:               31:0    reserved
*****************************************************/
static __inline HI_VOID hi_isp_dpc_blend_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_ALPHA ISP_DPC_ALPHA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_ALPHA.u32 = data;
	pstIspReg->ISP_DPC_ALPHA.u32 = ISP_DPC_ALPHA.u32;
}

/*****************************************************
                            
*****************************************************/
static __inline HI_VOID hi_isp_dp_mode_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_MODE ISP_DPC_MODE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_MODE.u32 = data;
	pstIspReg->ISP_DPC_MODE.u32 = ISP_DPC_MODE.u32;
}

/*****************************************************
ISP_DPC_MODE:          DPC        
0x205a3814:               31:4      reserved
					      3:0      output_mode
*****************************************************/
static __inline HI_VOID hi_isp_dp_output_mode_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_OUTPUT_MODE ISP_DPC_OUTPUT_MODE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_OUTPUT_MODE.u32 = data;
	pstIspReg->ISP_DPC_OUTPUT_MODE.u32 = ISP_DPC_OUTPUT_MODE.u32;
}

/*****************************************************
ISP_DPC_SET_USE:          DPC        
0x205a3818:               31:4      reserved
					      3:0      set_use
*****************************************************/
static __inline HI_VOID hi_isp_dp_set_use_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_SET_USE ISP_DPC_SET_USE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_SET_USE.u32 = data;
	pstIspReg->ISP_DPC_SET_USE.u32 = ISP_DPC_SET_USE.u32;
}

/*****************************************************
ISP_DPC_METHODS_SET_1:          DPC        
0x205a381c:               31:13      reserved
					      12:0      methods_set_1
*****************************************************/
static __inline HI_VOID hi_isp_dp_methods_set_1_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_METHODS_SET_1 ISP_DPC_METHODS_SET_1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_METHODS_SET_1.u32 = data;
	pstIspReg->ISP_DPC_METHODS_SET_1.u32 = ISP_DPC_METHODS_SET_1.u32;
}

/*****************************************************
ISP_DPC_METHODS_SET_2:          DPC        
0x205a3820:               31:13      reserved
					      12:0      methods_set_2
*****************************************************/
static __inline HI_VOID hi_isp_dp_methods_set_2_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_METHODS_SET_2 ISP_DPC_METHODS_SET_2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_METHODS_SET_2.u32 = data;
	pstIspReg->ISP_DPC_METHODS_SET_2.u32 = ISP_DPC_METHODS_SET_2.u32;
}

/*****************************************************
ISP_DPC_METHODS_SET_3:          DPC        
0x205a3824:               31:13      reserved
					      12:0      methods_set_3
*****************************************************/
static __inline HI_VOID hi_isp_dp_methods_set_3_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_METHODS_SET_3 ISP_DPC_METHODS_SET_3;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_METHODS_SET_3.u32 = data;
	pstIspReg->ISP_DPC_METHODS_SET_3.u32 = ISP_DPC_METHODS_SET_3.u32;
}

/*****************************************************
ISP_DPC_LINE_THRESH_1:          DPC        
0x205a3828:               31:16      reserved
					    15: 00      line_thresh_1
*****************************************************/
static __inline HI_VOID hi_isp_dp_line_thresh_1_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_LINE_THRESH_1 ISP_DPC_LINE_THRESH_1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_LINE_THRESH_1.u32 = data;
	pstIspReg->ISP_DPC_LINE_THRESH_1.u32 = ISP_DPC_LINE_THRESH_1.u32;
}
static __inline HI_U16 hi_isp_dp_line_thresh_1_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_LINE_THRESH_1 ISP_DPC_LINE_THRESH_1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_LINE_THRESH_1.u32 = pstIspReg->ISP_DPC_LINE_THRESH_1.u32;
	 return ISP_DPC_LINE_THRESH_1.u32;
}

/*****************************************************
ISP_DPC_LINE_MAD_FAC_1:          DPC        
0x205a382c:               31:16      reserved
					    15: 00      line_mad_fac_1
*****************************************************/
static __inline HI_VOID hi_isp_dp_line_mad_fac_1_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_LINE_MAD_FAC_1 ISP_DPC_LINE_MAD_FAC_1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_LINE_MAD_FAC_1.u32 = data;
	pstIspReg->ISP_DPC_LINE_MAD_FAC_1.u32 = ISP_DPC_LINE_MAD_FAC_1.u32;
}
static __inline HI_U16 hi_isp_dp_line_mad_fac_1_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_LINE_MAD_FAC_1 ISP_DPC_LINE_MAD_FAC_1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_LINE_MAD_FAC_1.u32 = pstIspReg->ISP_DPC_LINE_MAD_FAC_1.u32;
	 return ISP_DPC_LINE_MAD_FAC_1.u32;
}


/*****************************************************
ISP_DPC_PG_FAC_1:          DPC        
0x205a5030:               31:16      reserved
					    15: 00      pg_fac_1
*****************************************************/
static __inline HI_VOID hi_isp_dp_pg_fac_1_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_PG_FAC_1 ISP_DPC_PG_FAC_1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_PG_FAC_1.u32 = data;
	pstIspReg->ISP_DPC_PG_FAC_1.u32 = ISP_DPC_PG_FAC_1.u32;
}

/*****************************************************
ISP_DPC_RND_THRESH_1:          DPC        
0x205a3834:               31:16      reserved
					    15: 00     rnd_thresh_1
*****************************************************/
static __inline HI_VOID hi_isp_dp_rnd_thresh_1_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_RND_THRESH_1 ISP_DPC_RND_THRESH_1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_RND_THRESH_1.u32 = data;
	pstIspReg->ISP_DPC_RND_THRESH_1.u32 = ISP_DPC_RND_THRESH_1.u32;
}

/*****************************************************
ISP_DPC_RG_FAC_1:          DPC        
0x205a3838:               31:16      reserved
					    15: 00     rg_fac_1
*****************************************************/
static __inline HI_VOID hi_isp_dp_rg_fac_1_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_RG_FAC_1 ISP_DPC_RG_FAC_1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_RG_FAC_1.u32 = data;
	pstIspReg->ISP_DPC_RG_FAC_1.u32 = ISP_DPC_RG_FAC_1.u32;
}

/*****************************************************
ISP_DPC_LINE_THRESH_2:          DPC        
0x205a383c:               31:16      reserved
					    15: 00     line_thresh_2
*****************************************************/
static __inline HI_VOID hi_isp_dp_line_thresh_2_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_LINE_THRESH_2 ISP_DPC_LINE_THRESH_2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_LINE_THRESH_2.u32 = data;
	pstIspReg->ISP_DPC_LINE_THRESH_2.u32 = ISP_DPC_LINE_THRESH_2.u32;
}

/*****************************************************
ISP_DPC_LINE_MAD_FAC_2:          DPC        
0x205a3840:               31:16      reserved
					    15: 00     line_mad_fac_2
*****************************************************/
static __inline HI_VOID hi_isp_dp_line_mad_fac_2_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_LINE_MAD_FAC_2 ISP_DPC_LINE_MAD_FAC_2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_LINE_MAD_FAC_2.u32 = data;
	pstIspReg->ISP_DPC_LINE_MAD_FAC_2.u32 = ISP_DPC_LINE_MAD_FAC_2.u32;
}

/*****************************************************
ISP_DPC_PG_FAC_2:          DPC        
0x205a3844:               31:16      reserved
					    15: 00     pg_fac_2
*****************************************************/
static __inline HI_VOID hi_isp_dp_pg_fac_2_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_PG_FAC_2 ISP_DPC_PG_FAC_2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_PG_FAC_2.u32 = data;
	pstIspReg->ISP_DPC_PG_FAC_2.u32 = ISP_DPC_PG_FAC_2.u32;
}

/*****************************************************
ISP_DPC_RND_THRESH_2:          DPC        
0x205a3848:               31:16      reserved
					    15: 00     rnd_thresh_2
*****************************************************/
static __inline HI_VOID hi_isp_dp_rnd_thresh_2_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_RND_THRESH_2 ISP_DPC_RND_THRESH_2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_RND_THRESH_2.u32 = data;
	pstIspReg->ISP_DPC_RND_THRESH_2.u32 = ISP_DPC_RND_THRESH_2.u32;
}

/*****************************************************
ISP_DPC_RG_FAC_2:          DPC        
0x205a384c:               31:16      reserved
					    15: 00     rg_fac_2
*****************************************************/
static __inline HI_VOID hi_isp_dp_rg_fac_2_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_RG_FAC_2 ISP_DPC_RG_FAC_2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_RG_FAC_2.u32 = data;
	pstIspReg->ISP_DPC_RG_FAC_2.u32 = ISP_DPC_RG_FAC_2.u32;
}

/*****************************************************
ISP_DPC_LINE_THRESH_3:          DPC        
0x205a3850:               31:16      reserved
					    15: 00     line_thresh_3
*****************************************************/
static __inline HI_VOID hi_isp_dp_line_thresh_3_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_LINE_THRESH_3 ISP_DPC_LINE_THRESH_3;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_LINE_THRESH_3.u32 = data;
	pstIspReg->ISP_DPC_LINE_THRESH_3.u32 = ISP_DPC_LINE_THRESH_3.u32;
}

/*****************************************************
ISP_DPC_LINE_MAD_FAC_3: DPC        
0x205a5054:             31:16      reserved
					    15: 00     line_mad_fac_3
*****************************************************/
static __inline HI_VOID hi_isp_dp_line_mad_fac_3_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_LINE_MAD_FAC_3 ISP_DPC_LINE_MAD_FAC_3;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_LINE_MAD_FAC_3.u32 = data;
	pstIspReg->ISP_DPC_LINE_MAD_FAC_3.u32 = ISP_DPC_LINE_MAD_FAC_3.u32;
}

/*****************************************************
ISP_DPC_PG_FAC_3:          DPC        
0x205a3858:               31:16      reserved
					    15: 00     pg_fac_3
*****************************************************/
static __inline HI_VOID hi_isp_dp_pg_fac_3_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_PG_FAC_3 ISP_DPC_PG_FAC_3;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_PG_FAC_3.u32 = data;
	pstIspReg->ISP_DPC_PG_FAC_3.u32 = ISP_DPC_PG_FAC_3.u32;
}

/*****************************************************
ISP_DPC_RND_THRESH_3:          DPC        
0x205a385c:               31:16      reserved
					    15: 00     rnd_thresh_3
*****************************************************/
static __inline HI_VOID hi_isp_dp_rnd_thresh_3_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_RND_THRESH_3 ISP_DPC_RND_THRESH_3;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_RND_THRESH_3.u32 = data;
	pstIspReg->ISP_DPC_RND_THRESH_3.u32 = ISP_DPC_RND_THRESH_3.u32;
}

/*****************************************************
ISP_DPC_RG_FAC_3:          DPC        
0x205a3860:               31:16      reserved
					    15: 00     rg_fac_3
*****************************************************/
static __inline HI_VOID hi_isp_dp_rg_fac_3_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_RG_FAC_3 ISP_DPC_RG_FAC_3;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_RG_FAC_3.u32 = data;
	pstIspReg->ISP_DPC_RG_FAC_3.u32 = ISP_DPC_RG_FAC_3.u32;
}

/*****************************************************
ISP_DPC_RO_LIMITS:          DPC        
0x205a3864:               31:16      reserved
					    15: 00     ro_limits
*****************************************************/
static __inline HI_VOID hi_isp_dp_ro_limits_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_RO_LIMITS ISP_DPC_RO_LIMITS;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_RO_LIMITS.u32 = data;
	pstIspReg->ISP_DPC_RO_LIMITS.u32 = ISP_DPC_RO_LIMITS.u32;
}

/*****************************************************
ISP_DPC_RND_OFFS:          DPC        
0x205a3868:               31:12      reserved
					    11: 00     rnd_offs
*****************************************************/
static __inline HI_VOID hi_isp_dp_rnd_offs_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_RND_OFFS ISP_DPC_RND_OFFS;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));

	ISP_DPC_RND_OFFS.u32 = data;
	pstIspReg->ISP_DPC_RND_OFFS.u32 = ISP_DPC_RND_OFFS.u32;
}

/*****************************************************
ISP_DPC_BPT_CTRL:   DPC        
0x205a386c:               31:16      bp_data
					    15:12      reserved
					    11: 00     bpt_ctrl
*****************************************************/
#define HI_ISP_DPCC_BPDATA_DEFAULT (16383)

static __inline HI_VOID hi_isp_dp_bpt_ctrl_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_BPT_CTRL ISP_DPC_BPT_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_BPT_CTRL.u32 = data;
	pstIspReg->ISP_DPC_BPT_CTRL.u32 = ISP_DPC_BPT_CTRL.u32;
}

/*****************************************************
ISP_DPC_BPT_NUMBER:   DPC        
0x205a3870:               31:12      reserved
					    11: 00     bpt_number
*****************************************************/
static __inline HI_VOID hi_isp_dp_bpt_number_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_BPT_NUMBER ISP_DPC_BPT_NUMBER;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_BPT_NUMBER.u32 = pstIspReg->ISP_DPC_BPT_NUMBER.u32;
	ISP_DPC_BPT_NUMBER.bits.bpt_number = data;
	pstIspReg->ISP_DPC_BPT_NUMBER.u32 = ISP_DPC_BPT_NUMBER.u32;
}

static __inline HI_U16 hi_isp_dp_bpt_number_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_BPT_NUMBER ISP_DPC_BPT_NUMBER;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_BPT_NUMBER.u32 = pstIspReg->ISP_DPC_BPT_NUMBER.u32;
	return ISP_DPC_BPT_NUMBER.bits.bpt_number;	
}
/*****************************************************
ISP_DPC_BPT_CALIB_NUMBER:   DPC        
0x205a3874:               31:12      reserved
					    11: 00     bpt_calib_number
*****************************************************/
static __inline HI_VOID hi_isp_dp_bpt_calib_number_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_BPT_CALIB_NUMBER ISP_DPC_BPT_CALIB_NUMBER;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_BPT_CALIB_NUMBER.u32 = pstIspReg->ISP_DPC_BPT_CALIB_NUMBER.u32;
	ISP_DPC_BPT_CALIB_NUMBER.u32 = data;
	pstIspReg->ISP_DPC_BPT_CALIB_NUMBER.u32 = ISP_DPC_BPT_CALIB_NUMBER.u32;
}

static __inline HI_U16 hi_isp_dp_bpt_calib_number_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_BPT_CALIB_NUMBER ISP_DPC_BPT_CALIB_NUMBER;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_BPT_CALIB_NUMBER.u32 = pstIspReg->ISP_DPC_BPT_CALIB_NUMBER.u32;
	return ISP_DPC_BPT_CALIB_NUMBER.u32;
}

/*****************************************************
ISP_DPC_BPT_THRD:   DPC        
0x205a387c:               31: 24      abs_hot_thresh
					    23: 16      dev_hot_thresh
					    15: 08      abs_dead_thresh
					    07: 00      dev_dead_thresh
*****************************************************/
static __inline HI_VOID hi_isp_dp_bpt_thresh_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_BPT_THRD ISP_DPC_BPT_THRD;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_BPT_THRD.u32 = data;
	pstIspReg->ISP_DPC_BPT_THRD.u32 = ISP_DPC_BPT_THRD.u32;
}

/*****************************************************
ISP_DPC_BPT_WADDR:   DPC        
0x205a3880:               31: 00      bpt_waddr
*****************************************************/
static __inline HI_VOID hi_isp_dp_bpt_waddr_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_BPT_WADDR ISP_DPC_BPT_WADDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_BPT_WADDR.u32 = data;
	pstIspReg->ISP_DPC_BPT_WADDR.u32 = ISP_DPC_BPT_WADDR.u32;
}

/*****************************************************
ISP_DPC_BPT_WDATA:   DPC        
0x205a3884:               31: 00      bpt_wdata
*****************************************************/
static __inline HI_VOID hi_isp_dp_bpt_wdata_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_BPT_WDATA ISP_DPC_BPT_WDATA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_BPT_WDATA.u32 = data;
	pstIspReg->ISP_DPC_BPT_WDATA.u32 = ISP_DPC_BPT_WDATA.u32;
}

/*****************************************************
ISP_DPC_BPT_RADDR:   DPC        
0x205a3888:                  31: 00      bpt_raddr
*****************************************************/
static __inline HI_VOID hi_isp_dp_bpt_raddr_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_BPT_RADDR ISP_DPC_BPT_RADDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_BPT_RADDR.u32 = data;
	pstIspReg->ISP_DPC_BPT_RADDR.u32 = ISP_DPC_BPT_RADDR.u32;
}

/*****************************************************
ISP_DPC_BPT_RDATA:   DPC        
0x205a388c:                  31: 00      bpt_rdata
*****************************************************/
static __inline HI_U32 hi_isp_dp_bpt_rdata_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_BPT_RDATA ISP_DPC_BPT_RDATA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_BPT_RDATA.u32 = pstIspReg->ISP_DPC_BPT_RDATA.u32;
	return ISP_DPC_BPT_RDATA.u32;
}

/*****************************************************
ISP_DPC_SIZE:   DPC    
0x205a30f0:                     31:29      reserved
                                         28:16      height
                                         15:13      reserved
                                         12:00      width
*****************************************************/
static __inline HI_VOID hi_isp_dpc_height_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_SIZE ISP_DPC_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_SIZE.u32 = pstIspReg->ISP_DPC_SIZE.u32;
	ISP_DPC_SIZE.bits.height = data;
	pstIspReg->ISP_DPC_SIZE.u32 = ISP_DPC_SIZE.u32;
}

static __inline HI_VOID hi_isp_dpc_width_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_SIZE ISP_DPC_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_SIZE.u32 = pstIspReg->ISP_DPC_SIZE.u32;
	ISP_DPC_SIZE.bits.width = data;
	pstIspReg->ISP_DPC_SIZE.u32 = ISP_DPC_SIZE.u32;
}

/*****************************************************

*****************************************************/
#define HI_ISP_DPCC_SOFT_THR_MAX_DEFAULT (6)
#define HI_ISP_DPCC_SOFT_THR_MIN_DEFAULT (-15)

static __inline HI_VOID hi_isp_dpc_soft_thr_max_write(HI_U8 ispDev, HI_S16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_SOFT_THR ISP_DPC_SOFT_THR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_SOFT_THR.u32 = pstIspReg->ISP_DPC_SOFT_THR.u32;
	ISP_DPC_SOFT_THR.bits.soft_thr_max = data;
	pstIspReg->ISP_DPC_SOFT_THR.u32 = ISP_DPC_SOFT_THR.u32;
}

static __inline HI_S16 hi_isp_dpc_soft_thr_max_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_SOFT_THR ISP_DPC_SOFT_THR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_SOFT_THR.u32 = pstIspReg->ISP_DPC_SOFT_THR.u32;
	return ISP_DPC_SOFT_THR.bits.soft_thr_max;
}

static __inline HI_VOID hi_isp_dpc_soft_thr_min_write(HI_U8 ispDev, HI_S16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_SOFT_THR ISP_DPC_SOFT_THR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_SOFT_THR.u32 = pstIspReg->ISP_DPC_SOFT_THR.u32;
	ISP_DPC_SOFT_THR.bits.soft_thr_min = data;
	pstIspReg->ISP_DPC_SOFT_THR.u32 = ISP_DPC_SOFT_THR.u32;
}

static __inline HI_S16 hi_isp_dpc_soft_thr_min_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_SOFT_THR ISP_DPC_SOFT_THR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_SOFT_THR.u32 = pstIspReg->ISP_DPC_SOFT_THR.u32;
	return ISP_DPC_SOFT_THR.bits.soft_thr_min;
}

/*****************************************************
0x113C3D14:                     31:1      reserved
                                00      hard_thr_en
*****************************************************/
#define HI_ISP_DPCC_HARD_THR_ENABLE_DEFAULT (1)
static __inline HI_VOID hi_isp_dpc_hard_thr_enable_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_HARDTHR_EN ISP_DPC_HARDTHR_EN;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_HARDTHR_EN.u32 = pstIspReg->ISP_DPC_HARDTHR_EN.u32;
	ISP_DPC_HARDTHR_EN.bits.hard_thr_en = data;
	pstIspReg->ISP_DPC_HARDTHR_EN.u32 = ISP_DPC_HARDTHR_EN.u32;
}

static __inline HI_U32 hi_isp_dpc_hard_thr_enable_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_HARDTHR_EN ISP_DPC_HARDTHR_EN;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_HARDTHR_EN.u32 = pstIspReg->ISP_DPC_HARDTHR_EN.u32;
	return ISP_DPC_HARDTHR_EN.bits.hard_thr_en;
}
/*****************************************************
0x113C3D18:                     31:10      reserved
                                09:00      rake_ratio
*****************************************************/
#define HI_ISP_DPCC_RAKE_RATIO_DEFAULT (24)
static __inline HI_VOID hi_isp_dpc_rake_ratio_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_RAKERATIO ISP_DPC_RAKERATIO;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_RAKERATIO.u32 = pstIspReg->ISP_DPC_RAKERATIO.u32;
	ISP_DPC_RAKERATIO.bits.rake_ratio = data;
	pstIspReg->ISP_DPC_RAKERATIO.u32 = ISP_DPC_RAKERATIO.u32;
}

static __inline HI_U32 hi_isp_dpc_rake_ratio_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DPC_RAKERATIO ISP_DPC_RAKERATIO;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DPC_RAKERATIO.u32 = pstIspReg->ISP_DPC_RAKERATIO.u32;
	return ISP_DPC_RAKERATIO.bits.rake_ratio;
}


static __inline HI_U8 hi_isp_fpn_enable_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_FPN_CFG ISP_FPN_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_FPN_CFG.u32 = pstIspReg->ISP_FPN_CFG.u32;
	return ISP_FPN_CFG.bits.enable;	
}

/*****************************************************

0x205a5230 ~ 0x205a523C:                    	
								31:16		strength
								15:12		reserved
								11:0		offset
*****************************************************/
static __inline HI_VOID hi_isp_fpn_corr_gainoffset_write(HI_U8 ispDev, HI_U8 index, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_FPN_CORR0 ISP_FPN_CORR0;
    U_ISP_FPN_CORR1 ISP_FPN_CORR1;
    U_ISP_FPN_CORR2 ISP_FPN_CORR2;
    U_ISP_FPN_CORR3 ISP_FPN_CORR3;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));

	switch (index)
	{
		case 0:
	        ISP_FPN_CORR0.u32 = data;
	        pstIspReg->ISP_FPN_CORR0.u32 = ISP_FPN_CORR0.u32;
			break;
		case 1:
			ISP_FPN_CORR1.u32 = data;
			pstIspReg->ISP_FPN_CORR1.u32 = ISP_FPN_CORR1.u32;
			break;
		case 2:
			ISP_FPN_CORR2.u32 = data;
			pstIspReg->ISP_FPN_CORR2.u32 = ISP_FPN_CORR2.u32;
			break;
		case 3:
			ISP_FPN_CORR3.u32 = data;
			pstIspReg->ISP_FPN_CORR3.u32 = ISP_FPN_CORR3.u32;
			break;		
		default:
			break;
	}
	
}

static __inline HI_U32 hi_isp_fpn_corr_gainoffset_read(HI_U8 ispDev, HI_U8 index){
	S_HIISP_REGS_TYPE * pstIspReg;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	switch (index)
	{
		case 0:
			return pstIspReg->ISP_FPN_CORR0.u32;
		case 1:
			return pstIspReg->ISP_FPN_CORR1.u32;
		case 2:
			return pstIspReg->ISP_FPN_CORR2.u32;
		case 3:
			return pstIspReg->ISP_FPN_CORR3.u32;
		default:
			return 0;
	}
}


/*****************************************************

0x205a5254:                    	
								31:14		reserved
								13:0		overflowthr
*****************************************************/
static __inline HI_VOID hi_isp_fpn_coverflowthr_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_FPN_OVERFLOWTHR ISP_FPN_OVERFLOWTHR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_FPN_OVERFLOWTHR.u32 = pstIspReg->ISP_FPN_OVERFLOWTHR.u32;
	ISP_FPN_OVERFLOWTHR.bits.overflowthr = data;
	pstIspReg->ISP_FPN_OVERFLOWTHR.u32 = ISP_FPN_OVERFLOWTHR.u32;
}


/*****************************************************
ISP_FPN_SIZE:      
0x205a3af0:                     31:29    reserved
                                          28:16   height
                                          15:13   reserved
                                          12:  0   width
*****************************************************/
static __inline HI_VOID hi_isp_fpn_height_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_FPN_SIZE ISP_FPN_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_FPN_SIZE.u32 = pstIspReg->ISP_FPN_SIZE.u32;
	ISP_FPN_SIZE.bits.height = data;
	pstIspReg->ISP_FPN_SIZE.u32 = ISP_FPN_SIZE.u32;
}

static __inline HI_VOID hi_isp_fpn_width_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_FPN_SIZE ISP_FPN_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_FPN_SIZE.u32 = pstIspReg->ISP_FPN_SIZE.u32;
	ISP_FPN_SIZE.bits.width = data;
	pstIspReg->ISP_FPN_SIZE.u32 = ISP_FPN_SIZE.u32;
}


/*****************************************************
MODULE        -----DRC-----
MODULE        -----DRC-----
MODULE        -----DRC-----
*****************************************************/
/*****************************************************
ISP_DRC_CFG:         DRC        
0x205a6300:           31:1    reserved
*****************************************************/
static __inline HI_VOID hi_isp_drc_enable_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_CFG ISP_DRC_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_CFG.u32 = pstIspReg->ISP_DRC_CFG.u32;
	ISP_DRC_CFG.bits.drc_en = data;
	pstIspReg->ISP_DRC_CFG.u32 = ISP_DRC_CFG.u32;
}

static __inline HI_U8 hi_isp_drc_enable_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_CFG ISP_DRC_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_CFG.u32 = pstIspReg->ISP_DRC_CFG.u32;
	return ISP_DRC_CFG.bits.drc_en;
}

/*****************************************************
ISP_DRC_ZONE:     
0x205a6310:             31:30  reserved
                               29:24  chk_y
					23:16  vnum
					15:14  reserved
                                13:8   chk_x
                                7:0     hnum
*****************************************************/
static __inline HI_VOID hi_isp_drc_hnum_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_ZONE ISP_DRC_ZONE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_ZONE.u32 = pstIspReg->ISP_DRC_ZONE.u32;
	ISP_DRC_ZONE.bits.hnum = data;
	pstIspReg->ISP_DRC_ZONE.u32 = ISP_DRC_ZONE.u32;
}

static __inline HI_U8 hi_isp_drc_hnum_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_ZONE ISP_DRC_ZONE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_ZONE.u32 = pstIspReg->ISP_DRC_ZONE.u32;
	return ISP_DRC_ZONE.bits.hnum;
}

static __inline HI_VOID hi_isp_drc_vnum_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_ZONE ISP_DRC_ZONE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_ZONE.u32 = pstIspReg->ISP_DRC_ZONE.u32;
	ISP_DRC_ZONE.bits.vnum = data;
	pstIspReg->ISP_DRC_ZONE.u32 = ISP_DRC_ZONE.u32;
}

static __inline HI_U8 hi_isp_drc_vnum_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_ZONE ISP_DRC_ZONE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_ZONE.u32 = pstIspReg->ISP_DRC_ZONE.u32;
	return ISP_DRC_ZONE.bits.vnum;
}

/*****************************************************
ISP_DRC_ZONE_SIZE:       DRC zone size
0x205a6314:             31:30  reserved
                               29:24  chk_y
					23:16  vnum
					15:14  reserved
                                13:8   chk_x
                                7:0     hnum
*****************************************************/
static __inline HI_VOID hi_isp_drc_hsize_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_ZONE_SIZE ISP_DRC_ZONE_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_ZONE_SIZE.u32 = pstIspReg->ISP_DRC_ZONE_SIZE.u32;
	ISP_DRC_ZONE_SIZE.bits.hsize = data;
	pstIspReg->ISP_DRC_ZONE_SIZE.u32 = ISP_DRC_ZONE_SIZE.u32;
}

static __inline HI_VOID hi_isp_drc_chk_x_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_ZONE_SIZE ISP_DRC_ZONE_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_ZONE_SIZE.u32 = pstIspReg->ISP_DRC_ZONE_SIZE.u32;
	ISP_DRC_ZONE_SIZE.bits.chk_x = data;
	pstIspReg->ISP_DRC_ZONE_SIZE.u32 = ISP_DRC_ZONE_SIZE.u32;
}

static __inline HI_VOID hi_isp_drc_vsize_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_ZONE_SIZE ISP_DRC_ZONE_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_ZONE_SIZE.u32 = pstIspReg->ISP_DRC_ZONE_SIZE.u32;
	ISP_DRC_ZONE_SIZE.bits.vsize = data;
	pstIspReg->ISP_DRC_ZONE_SIZE.u32 = ISP_DRC_ZONE_SIZE.u32;
}

static __inline HI_VOID hi_isp_drc_chk_y_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_ZONE_SIZE ISP_DRC_ZONE_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_ZONE_SIZE.u32 = pstIspReg->ISP_DRC_ZONE_SIZE.u32;
	ISP_DRC_ZONE_SIZE.bits.chk_y = data;
	pstIspReg->ISP_DRC_ZONE_SIZE.u32 = ISP_DRC_ZONE_SIZE.u32;
}

/*****************************************************
ISP_DRC_ZONE_DIV0:       DRC block size
0x205a6318:              31       reserved
					30:16   div_y0
                                15       reserved
                                14:0    div_x0
*****************************************************/
static __inline HI_VOID hi_isp_drc_div_x0_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_ZONE_DIV0 ISP_DRC_ZONE_DIV0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_ZONE_DIV0.u32 = pstIspReg->ISP_DRC_ZONE_DIV0.u32;
	ISP_DRC_ZONE_DIV0.bits.div_x = data;
	pstIspReg->ISP_DRC_ZONE_DIV0.u32 = ISP_DRC_ZONE_DIV0.u32;
}

static __inline HI_VOID hi_isp_drc_div_y0_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_ZONE_DIV0 ISP_DRC_ZONE_DIV0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_ZONE_DIV0.u32 = pstIspReg->ISP_DRC_ZONE_DIV0.u32;
	ISP_DRC_ZONE_DIV0.bits.div_y = data;
	pstIspReg->ISP_DRC_ZONE_DIV0.u32 = ISP_DRC_ZONE_DIV0.u32;
}

/*****************************************************
ISP_DRC_ZONE_DIV1:       DRC
0x205a611C:             31       reserved
					30:16   div_y1
                                15       reserved
                                14:0    div_x1
*****************************************************/
static __inline HI_VOID hi_isp_drc_div_x1_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_ZONE_DIV1 ISP_DRC_ZONE_DIV1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_ZONE_DIV1.u32 = pstIspReg->ISP_DRC_ZONE_DIV1.u32;
	ISP_DRC_ZONE_DIV1.bits.div_x = data;
	pstIspReg->ISP_DRC_ZONE_DIV1.u32 = ISP_DRC_ZONE_DIV1.u32;
}

static __inline HI_VOID hi_isp_drc_div_y1_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_ZONE_DIV1 ISP_DRC_ZONE_DIV1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_ZONE_DIV1.u32 = pstIspReg->ISP_DRC_ZONE_DIV1.u32;
	ISP_DRC_ZONE_DIV1.bits.div_y = data;
	pstIspReg->ISP_DRC_ZONE_DIV1.u32 = ISP_DRC_ZONE_DIV1.u32;
}

/*****************************************************
ISP_DRC_BIN:         DRC
0x205a6120:           31:05    reserved
					04:00    bin
*****************************************************/
static __inline HI_VOID hi_isp_drc_bin_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_BIN ISP_DRC_BIN;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_BIN.u32 = pstIspReg->ISP_DRC_BIN.u32;
	ISP_DRC_BIN.bits.bin = data;
	pstIspReg->ISP_DRC_BIN.u32 = ISP_DRC_BIN.u32;
}

static __inline HI_U8 hi_isp_drc_bin_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_BIN ISP_DRC_BIN;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_BIN.u32 = pstIspReg->ISP_DRC_BIN.u32;
	return ISP_DRC_BIN.bits.bin;
}

/*****************************************************
ISP_DRC_BIN_DIV:         DRC
0x205a6124:           31:15    reserved
					14:00    div_z DRC
*****************************************************/
static __inline HI_VOID hi_isp_drc_div_z_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_BIN_DIV ISP_DRC_BIN_DIV;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_BIN_DIV.u32 = pstIspReg->ISP_DRC_BIN_DIV.u32;
	ISP_DRC_BIN_DIV.bits.div_z = data;
	pstIspReg->ISP_DRC_BIN_DIV.u32 = ISP_DRC_BIN_DIV.u32;
}

static __inline HI_U16 hi_isp_drc_div_z_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_BIN_DIV ISP_DRC_BIN_DIV;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_BIN_DIV.u32 = pstIspReg->ISP_DRC_BIN_DIV.u32;
	return ISP_DRC_BIN_DIV.bits.div_z;
}

/*****************************************************
ISP_DRC_BIN_SCALE:         DRC
0x205a6128:           31:03    reserved
					02:00    bin_scale DRC
*****************************************************/
static __inline HI_VOID hi_isp_drc_bin_scale_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_BIN_SCALE ISP_DRC_BIN_SCALE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_BIN_SCALE.u32 = pstIspReg->ISP_DRC_BIN_SCALE.u32;
	ISP_DRC_BIN_SCALE.bits.bin_scale = data;
	pstIspReg->ISP_DRC_BIN_SCALE.u32 = ISP_DRC_BIN_SCALE.u32;
}

/*****************************************************
ISP_DRC_RANGE_FLT_COEF:     
0x205a612c:             31:03    reserved
					03:00   range flt coef
*****************************************************/
static __inline HI_VOID hi_isp_drc_range_flt_coef_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_RANGE_FLT_COEF ISP_DRC_RANGE_FLT_COEF;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_RANGE_FLT_COEF.u32 = pstIspReg->ISP_DRC_RANGE_FLT_COEF.u32;
	ISP_DRC_RANGE_FLT_COEF.bits.range_flt_coef = data;
	pstIspReg->ISP_DRC_RANGE_FLT_COEF.u32 = ISP_DRC_RANGE_FLT_COEF.u32;
}

/*****************************************************
ISP_DRC_EXPOSURE:  
0x205a6130:         31:16    reserved
					15:00    exposure DRC
*****************************************************/
static __inline HI_VOID hi_isp_drc_exposure_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_EXPOSURE ISP_DRC_EXPOSURE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_EXPOSURE.u32 = pstIspReg->ISP_DRC_EXPOSURE.u32;
	ISP_DRC_EXPOSURE.bits.exposure = data;
	pstIspReg->ISP_DRC_EXPOSURE.u32 = ISP_DRC_EXPOSURE.u32;
}

/*****************************************************
ISP_DRC_LOCAL_EDGE_LMT:       
0x205a6134:           31:08    reserved
					07:00    local_edge_lmt DRC
*****************************************************/
static __inline HI_VOID hi_isp_drc_local_edge_lmt_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_LOCAL_EDGE_LMT ISP_DRC_LOCAL_EDGE_LMT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_LOCAL_EDGE_LMT.u32 = pstIspReg->ISP_DRC_LOCAL_EDGE_LMT.u32;
	ISP_DRC_LOCAL_EDGE_LMT.bits.local_edge_lmt = data;
	pstIspReg->ISP_DRC_LOCAL_EDGE_LMT.u32 = ISP_DRC_LOCAL_EDGE_LMT.u32;
}

/*****************************************************
ISP_DRC_STRENGTH:         DRC
0x205a6138:           31:08    reserved
					07:00    strength DRC
*****************************************************/
static __inline HI_VOID hi_isp_drc_strength_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_STRENGTH ISP_DRC_STRENGTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_STRENGTH.u32 = pstIspReg->ISP_DRC_STRENGTH.u32;
	ISP_DRC_STRENGTH.bits.strength = data;
	pstIspReg->ISP_DRC_STRENGTH.u32 = ISP_DRC_STRENGTH.u32;
}

static __inline HI_U8 hi_isp_drc_strength_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_STRENGTH ISP_DRC_STRENGTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_STRENGTH.u32 = pstIspReg->ISP_DRC_STRENGTH.u32;
	return ISP_DRC_STRENGTH.bits.strength;
}

/*****************************************************
ISP_DRC_DETAIL_GAIN: DRC
0x205a613C:             31:24    reserved
                               23:16    detail_gain_thres
                               15:8      detail_gain_dark
					07:00    detail_gain_bright
*****************************************************/
static __inline HI_VOID hi_isp_drc_detail_mixing_thres_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_DETAIL_GAIN ISP_DRC_DETAIL_GAIN;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_DETAIL_GAIN.u32 = pstIspReg->ISP_DRC_DETAIL_GAIN.u32;
	ISP_DRC_DETAIL_GAIN.bits.detail_mixing_thres = data;
	pstIspReg->ISP_DRC_DETAIL_GAIN.u32 = ISP_DRC_DETAIL_GAIN.u32;
}

static __inline HI_U8 hi_isp_drc_detail_mixing_thres_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_DETAIL_GAIN ISP_DRC_DETAIL_GAIN;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_DETAIL_GAIN.u32 = pstIspReg->ISP_DRC_DETAIL_GAIN.u32;
	return ISP_DRC_DETAIL_GAIN.bits.detail_mixing_thres;
}

static __inline HI_VOID hi_isp_drc_detail_mixing_dark_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_DETAIL_GAIN ISP_DRC_DETAIL_GAIN;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_DETAIL_GAIN.u32 = pstIspReg->ISP_DRC_DETAIL_GAIN.u32;
	ISP_DRC_DETAIL_GAIN.bits.detail_mixing_dark = data;
	pstIspReg->ISP_DRC_DETAIL_GAIN.u32 = ISP_DRC_DETAIL_GAIN.u32;
}

static __inline HI_U8 hi_isp_drc_detail_mixing_dark_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_DETAIL_GAIN ISP_DRC_DETAIL_GAIN;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_DETAIL_GAIN.u32 = pstIspReg->ISP_DRC_DETAIL_GAIN.u32;
	return ISP_DRC_DETAIL_GAIN.bits.detail_mixing_dark;
}

static __inline HI_VOID hi_isp_drc_detail_mixing_bright_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_DETAIL_GAIN ISP_DRC_DETAIL_GAIN;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_DETAIL_GAIN.u32 = pstIspReg->ISP_DRC_DETAIL_GAIN.u32;
	ISP_DRC_DETAIL_GAIN.bits.detail_mixing_bright = data;
	pstIspReg->ISP_DRC_DETAIL_GAIN.u32 = ISP_DRC_DETAIL_GAIN.u32;
}

static __inline HI_U8 hi_isp_drc_detail_mixing_bright_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_DETAIL_GAIN ISP_DRC_DETAIL_GAIN;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_DETAIL_GAIN.u32 = pstIspReg->ISP_DRC_DETAIL_GAIN.u32;
	return ISP_DRC_DETAIL_GAIN.bits.detail_mixing_bright;
}

/*****************************************************
ISP_DRC_DARK_GAIN_LMT_Y:         DRC
0x205a6140:           31:16    reserved
					15:14    SFT2  DRC
					13:10    VAL2 DRC
					09:07    SFT1
					06:00    VAL1
*****************************************************/
static __inline HI_VOID hi_isp_drc_y_val1_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_DARK_GAIN_LMT_Y ISP_DRC_DARK_GAIN_LMT_Y;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_DARK_GAIN_LMT_Y.u32 = pstIspReg->ISP_DRC_DARK_GAIN_LMT_Y.u32;
	ISP_DRC_DARK_GAIN_LMT_Y.bits.val1 = data;
	pstIspReg->ISP_DRC_DARK_GAIN_LMT_Y.u32 = ISP_DRC_DARK_GAIN_LMT_Y.u32;
}

static __inline HI_VOID hi_isp_drc_y_sft1_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_DARK_GAIN_LMT_Y ISP_DRC_DARK_GAIN_LMT_Y;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_DARK_GAIN_LMT_Y.u32 = pstIspReg->ISP_DRC_DARK_GAIN_LMT_Y.u32;
	ISP_DRC_DARK_GAIN_LMT_Y.bits.sft1 = data;
	pstIspReg->ISP_DRC_DARK_GAIN_LMT_Y.u32 = ISP_DRC_DARK_GAIN_LMT_Y.u32;
}

static __inline HI_VOID hi_isp_drc_y_val2_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_DARK_GAIN_LMT_Y ISP_DRC_DARK_GAIN_LMT_Y;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_DARK_GAIN_LMT_Y.u32 = pstIspReg->ISP_DRC_DARK_GAIN_LMT_Y.u32;
	ISP_DRC_DARK_GAIN_LMT_Y.bits.val2 = data;
	pstIspReg->ISP_DRC_DARK_GAIN_LMT_Y.u32 = ISP_DRC_DARK_GAIN_LMT_Y.u32;
}

static __inline HI_VOID hi_isp_drc_y_sft2_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_DARK_GAIN_LMT_Y ISP_DRC_DARK_GAIN_LMT_Y;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_DARK_GAIN_LMT_Y.u32 = pstIspReg->ISP_DRC_DARK_GAIN_LMT_Y.u32;
	ISP_DRC_DARK_GAIN_LMT_Y.bits.sft2 = data;
	pstIspReg->ISP_DRC_DARK_GAIN_LMT_Y.u32 = ISP_DRC_DARK_GAIN_LMT_Y.u32;
}

/*****************************************************
ISP_DRC_DARK_GAIN_LMT_C:         DRC
0x205a6144:           31:16    reserved
					15:14    SFT2  DRC
					13:10    VAL2 DRC
					09:07    SFT1
					06:00    VAL1
*****************************************************/
static __inline HI_VOID hi_isp_drc_c_val1_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_DARK_GAIN_LMT_C ISP_DRC_DARK_GAIN_LMT_C;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_DARK_GAIN_LMT_C.u32 = pstIspReg->ISP_DRC_DARK_GAIN_LMT_C.u32;
	ISP_DRC_DARK_GAIN_LMT_C.bits.val1 = data;
	pstIspReg->ISP_DRC_DARK_GAIN_LMT_C.u32 = ISP_DRC_DARK_GAIN_LMT_C.u32;
}

static __inline HI_VOID hi_isp_drc_c_sft1_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_DARK_GAIN_LMT_C ISP_DRC_DARK_GAIN_LMT_C;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_DARK_GAIN_LMT_C.u32 = pstIspReg->ISP_DRC_DARK_GAIN_LMT_C.u32;
	ISP_DRC_DARK_GAIN_LMT_C.bits.sft1 = data;
	pstIspReg->ISP_DRC_DARK_GAIN_LMT_C.u32 = ISP_DRC_DARK_GAIN_LMT_C.u32;
}

static __inline HI_VOID hi_isp_drc_c_val2_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_DARK_GAIN_LMT_C ISP_DRC_DARK_GAIN_LMT_C;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_DARK_GAIN_LMT_C.u32 = pstIspReg->ISP_DRC_DARK_GAIN_LMT_C.u32;
	ISP_DRC_DARK_GAIN_LMT_C.bits.val2 = data;
	pstIspReg->ISP_DRC_DARK_GAIN_LMT_C.u32 = ISP_DRC_DARK_GAIN_LMT_C.u32;
}

static __inline HI_VOID hi_isp_drc_c_sft2_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_DARK_GAIN_LMT_C ISP_DRC_DARK_GAIN_LMT_C;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_DARK_GAIN_LMT_C.u32 = pstIspReg->ISP_DRC_DARK_GAIN_LMT_C.u32;
	ISP_DRC_DARK_GAIN_LMT_C.bits.sft2 = data;
	pstIspReg->ISP_DRC_DARK_GAIN_LMT_C.u32 = ISP_DRC_DARK_GAIN_LMT_C.u32;
}

/*****************************************************
ISP_DRC_BRIGHT_GAIN_LMT:         DRC
0x205a6148:           31:10    reserved
					09:07    SFT
					06:00    VAL
*****************************************************/
static __inline HI_VOID hi_isp_drc_bright_gain_val_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_BRIGHT_GAIN_LMT ISP_DRC_BRIGHT_GAIN_LMT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_BRIGHT_GAIN_LMT.u32 = pstIspReg->ISP_DRC_BRIGHT_GAIN_LMT.u32;
	ISP_DRC_BRIGHT_GAIN_LMT.bits.val = data;
	pstIspReg->ISP_DRC_BRIGHT_GAIN_LMT.u32 = ISP_DRC_BRIGHT_GAIN_LMT.u32;
}

static __inline HI_VOID hi_isp_drc_bright_gain_sft_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_BRIGHT_GAIN_LMT ISP_DRC_BRIGHT_GAIN_LMT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_BRIGHT_GAIN_LMT.u32 = pstIspReg->ISP_DRC_BRIGHT_GAIN_LMT.u32;
	ISP_DRC_BRIGHT_GAIN_LMT.bits.sft = data;
	pstIspReg->ISP_DRC_BRIGHT_GAIN_LMT.u32 = ISP_DRC_BRIGHT_GAIN_LMT.u32;
}

/*****************************************************
U_ISP_DRC_RGB_WGT:         DRC
0x113a634C:           31:10    reserved
					06:00    r_wgt
*****************************************************/
#define HI_ISP_DRC_WEIGHT_R (3)
#define HI_ISP_DRC_WEIGHT_G (31)
#define HI_ISP_DRC_WEIGHT_B (-2)
static __inline HI_VOID hi_isp_drc_rgb_wgt_write(HI_U8 ispDev, HI_U8 r_wgt, HI_U8 g_wgt, HI_U8 b_wgt) {
	U_ISP_DRC_RGB_WGT ISP_DRC_RGB_WGT;
	S_HIISP_REGS_TYPE * pstIspReg;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));

	ISP_DRC_RGB_WGT = pstIspReg->ISP_DRC_RGB_WGT;
	
	ISP_DRC_RGB_WGT.bits.r_wgt = r_wgt;
	ISP_DRC_RGB_WGT.bits.g_wgt = g_wgt;
	ISP_DRC_RGB_WGT.bits.b_wgt = b_wgt;

	pstIspReg->ISP_DRC_RGB_WGT = ISP_DRC_RGB_WGT;
}

/*****************************************************






ISP_DRC_LUT_MIX_CTRL:  DRC TM LUT 
0x205a6154:           31:08    reserved
					07:00    outbits
*****************************************************/
static __inline HI_VOID hi_isp_drc_lut_mix_ctrl_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_LUT_MIX_CRTL ISP_DRC_LUT_MIX_CRTL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_LUT_MIX_CRTL.u32 = pstIspReg->ISP_DRC_LUT_MIX_CRTL.u32;
	ISP_DRC_LUT_MIX_CRTL.bits.lut_mix_ctrl = data;
	pstIspReg->ISP_DRC_LUT_MIX_CRTL.u32 = ISP_DRC_LUT_MIX_CRTL.u32;
}

static __inline HI_U8 hi_isp_drc_lut_mix_ctrl_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_LUT_MIX_CRTL ISP_DRC_LUT_MIX_CRTL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_LUT_MIX_CRTL.u32 = pstIspReg->ISP_DRC_LUT_MIX_CRTL.u32;
	return ISP_DRC_LUT_MIX_CRTL.bits.lut_mix_ctrl;
}

/*****************************************************
ISP_DRC_GAIN_CLIP_KNEE:  DRC
0x205a6158:             31:08    reserved
					07:04    gain_clip_knee
					03:00    gain_clip_step
*****************************************************/
static __inline HI_VOID hi_isp_drc_gain_clip_step_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_GAIN_CLIP ISP_DRC_GAIN_CLIP;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_GAIN_CLIP.u32 = pstIspReg->ISP_DRC_GAIN_CLIP.u32;
	ISP_DRC_GAIN_CLIP.bits.gain_clip_step = data;
	pstIspReg->ISP_DRC_GAIN_CLIP.u32 = ISP_DRC_GAIN_CLIP.u32;
}

/*****************************************************
ISP_DRC_GAIN_CLIP_STEP:  DRC  gain limit  
0x205a6158:             31:08    reserved
					07:04    gain_clip_knee
					03:00    gain_clip_step
*****************************************************/
static __inline HI_VOID hi_isp_drc_gain_clip_knee_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_GAIN_CLIP ISP_DRC_GAIN_CLIP;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_GAIN_CLIP.u32 = pstIspReg->ISP_DRC_GAIN_CLIP.u32;
	ISP_DRC_GAIN_CLIP.bits.gain_clip_knee = data;
	pstIspReg->ISP_DRC_GAIN_CLIP.u32 = ISP_DRC_GAIN_CLIP.u32;
}

/*****************************************************
U_ISP_DRC_COLOR_CTRL:  DRC  gain limit  
0x113a635c:             31:08    reserved
					07:04    cc_lin_pow
					03:00    cc_lut_ctrl
*****************************************************/
#define HI_ISP_DRC_CC_LUT_CTRL_DEFAULT (1)
#define HI_ISP_DRC_CC_LIN_POW_DEFAULT (1)
static __inline HI_VOID hi_isp_drc_color_ctrl_write(HI_U8 ispDev, HI_U8 cc_lut_ctrl, HI_U8 cc_lin_pow) {
	U_ISP_DRC_COLOR_CTRL   ISP_DRC_COLOR_CTRL;
	S_HIISP_REGS_TYPE * pstIspReg;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));

	ISP_DRC_COLOR_CTRL = pstIspReg->ISP_DRC_COLOR_CTRL;
	
	ISP_DRC_COLOR_CTRL.bits.cc_lut_ctrl = cc_lut_ctrl;
	ISP_DRC_COLOR_CTRL.bits.cc_lin_pow = cc_lin_pow;

	pstIspReg->ISP_DRC_COLOR_CTRL = ISP_DRC_COLOR_CTRL;
}

/*****************************************************
U_ISP_DRC_GLOBAL_CORR:  DRC  gain limit  
0x113a6360:             31:08    reserved
					11:00    cc_global_corr
*****************************************************/
#define HI_ISP_DRC_GLOBAL_CC_CORR_DEFAULT (1024)
static __inline HI_VOID hi_isp_drc_cc_corr_write(HI_U8 ispDev, HI_U16 cc_global_corr) {
	U_ISP_DRC_GLOBAL_CORR   ISP_DRC_GLOBAL_CORR;
	S_HIISP_REGS_TYPE * pstIspReg;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));

	ISP_DRC_GLOBAL_CORR = pstIspReg->ISP_DRC_GLOBAL_CORR;
	
	ISP_DRC_GLOBAL_CORR.bits.cc_global_corr = cc_global_corr;

	pstIspReg->ISP_DRC_GLOBAL_CORR = ISP_DRC_GLOBAL_CORR;
}

/*****************************************************
U_ISP_DRC_MIXING_CORING:  DRC  gain limit  
0x113a6364:             31:08    reserved
					07:00    mixing_coring
*****************************************************/
#define HI_ISP_DRC_MIXING_CORING_DEFAULT (2)
static __inline HI_VOID hi_isp_drc_mixing_coring_write(HI_U8 ispDev, HI_U8 mixing_coring) {
	U_ISP_DRC_MIXING_CORING   ISP_DRC_MIXING_CORING;
	S_HIISP_REGS_TYPE * pstIspReg;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));

	ISP_DRC_MIXING_CORING = pstIspReg->ISP_DRC_MIXING_CORING;
	
	ISP_DRC_MIXING_CORING.bits.mixing_coring = mixing_coring;

	pstIspReg->ISP_DRC_MIXING_CORING = ISP_DRC_MIXING_CORING;
}

/*****************************************************
U_ISP_DRC_MIXING_DARK:  DRC  gain limit  
0x113a6368:             31:08    reserved
	dark_min			  : 8	;
	dark_max			  : 8	; 
	dark_thr			  : 8	;
	dark_slo			  : 3	;

*****************************************************/
#define HI_ISP_DRC_MIXING_DARK_MIN_DEFAULT (45)
#define HI_ISP_DRC_MIXING_DARK_MAX_DEFAULT (64)
#define HI_ISP_DRC_MIXING_DARK_THR_DEFAULT (128)
#define HI_ISP_DRC_MIXING_DARK_SLO_DEFAULT (3)
static __inline HI_VOID hi_isp_drc_mixing_dark_min_write(HI_U8 ispDev, HI_U8 dark_min) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_MIXING_DARK ISP_DRC_MIXING_DARK;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_MIXING_DARK.u32 = pstIspReg->ISP_DRC_MIXING_DARK.u32;

	ISP_DRC_MIXING_DARK.bits.dark_min = dark_min;	
	pstIspReg->ISP_DRC_MIXING_DARK.u32 = ISP_DRC_MIXING_DARK.u32;
}

static __inline HI_VOID hi_isp_drc_mixing_dark_max_write(HI_U8 ispDev, HI_U8 dark_max) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_MIXING_DARK ISP_DRC_MIXING_DARK;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_MIXING_DARK.u32 = pstIspReg->ISP_DRC_MIXING_DARK.u32;

	ISP_DRC_MIXING_DARK.bits.dark_max = dark_max;	
	pstIspReg->ISP_DRC_MIXING_DARK.u32 = ISP_DRC_MIXING_DARK.u32;
}

static __inline HI_VOID hi_isp_drc_mixing_dark_thr_write(HI_U8 ispDev, HI_U8 dark_thr) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_MIXING_DARK ISP_DRC_MIXING_DARK;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_MIXING_DARK.u32 = pstIspReg->ISP_DRC_MIXING_DARK.u32;

	ISP_DRC_MIXING_DARK.bits.dark_thr = dark_thr;	
	pstIspReg->ISP_DRC_MIXING_DARK.u32 = ISP_DRC_MIXING_DARK.u32;
}

static __inline HI_VOID hi_isp_drc_mixing_dark_slo_write(HI_U8 ispDev, HI_U8 dark_slo) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_MIXING_DARK ISP_DRC_MIXING_DARK;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_MIXING_DARK.u32 = pstIspReg->ISP_DRC_MIXING_DARK.u32;

	ISP_DRC_MIXING_DARK.bits.dark_slo = dark_slo;	
	pstIspReg->ISP_DRC_MIXING_DARK.u32 = ISP_DRC_MIXING_DARK.u32;
}


/*****************************************************
U_ISP_DRC_MIXING_BRIGHT:  DRC  gain limit  
0x113a636c:             31:08    reserved
	dark_min			  : 8	;
	dark_max			  : 8	; 
	dark_thr			  : 8	;
	dark_slo			  : 3	;

*****************************************************/
#define HI_ISP_DRC_MIXING_BRIGHT_MIN_DEFAULT (45)
#define HI_ISP_DRC_MIXING_BRIGHT_MAX_DEFAULT (64)
#define HI_ISP_DRC_MIXING_BRIGHT_THR_DEFAULT (64)
#define HI_ISP_DRC_MIXING_BRIGHT_SLO_DEFAULT (7)

static __inline HI_VOID hi_isp_drc_mixing_bright_min_write(HI_U8 ispDev, HI_U8 bright_min) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_MIXING_BRIGHT ISP_DRC_MIXING_BRIGHT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_MIXING_BRIGHT.u32 = pstIspReg->ISP_DRC_MIXING_BRIGHT.u32;

	ISP_DRC_MIXING_BRIGHT.bits.bright_min = bright_min;	
	pstIspReg->ISP_DRC_MIXING_BRIGHT.u32 = ISP_DRC_MIXING_BRIGHT.u32;
}

static __inline HI_VOID hi_isp_drc_mixing_bright_max_write(HI_U8 ispDev, HI_U8 bright_max) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_MIXING_BRIGHT ISP_DRC_MIXING_BRIGHT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_MIXING_BRIGHT.u32 = pstIspReg->ISP_DRC_MIXING_BRIGHT.u32;

	ISP_DRC_MIXING_BRIGHT.bits.bright_max = bright_max;	
	pstIspReg->ISP_DRC_MIXING_BRIGHT.u32 = ISP_DRC_MIXING_BRIGHT.u32;
}

static __inline HI_VOID hi_isp_drc_mixing_bright_thr_write(HI_U8 ispDev, HI_U8 bright_thr) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_MIXING_BRIGHT ISP_DRC_MIXING_BRIGHT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_MIXING_BRIGHT.u32 = pstIspReg->ISP_DRC_MIXING_BRIGHT.u32;

	ISP_DRC_MIXING_BRIGHT.bits.bright_thr = bright_thr;	
	pstIspReg->ISP_DRC_MIXING_BRIGHT.u32 = ISP_DRC_MIXING_BRIGHT.u32;
}

static __inline HI_VOID hi_isp_drc_mixing_bright_slo_write(HI_U8 ispDev, HI_U8 bright_slo) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_MIXING_BRIGHT ISP_DRC_MIXING_BRIGHT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_MIXING_BRIGHT.u32 = pstIspReg->ISP_DRC_MIXING_BRIGHT.u32;

	ISP_DRC_MIXING_BRIGHT.bits.bright_slo = bright_slo;	
	pstIspReg->ISP_DRC_MIXING_BRIGHT.u32 = ISP_DRC_MIXING_BRIGHT.u32;
}

/*****************************************************
U_ISP_DRC_RG_CTRL:  DRC  gain limit  
0x113a6370:             31:08    reserved
 rg_ctr                : 8   ;
 rg_wid                : 7   ;
 reserved_0            : 1   ;
 rg_slo                : 3   ;
 reserved_1            : 13  ;
*****************************************************/
#define HI_ISP_DRC_RG_CTR_DEFAULT (74)
#define HI_ISP_DRC_RG_WIDTH_DEFAULT (39)
#define HI_ISP_DRC_RG_SLOPE_DEFAULT (4)
static __inline HI_VOID hi_isp_drc_rg_ctrl_write(HI_U8 ispDev, HI_U8 rg_ctr, HI_U8 rg_wid, HI_U8 rg_slo) {
	U_ISP_DRC_RG_CTRL   ISP_DRC_RG_CTRL;
	S_HIISP_REGS_TYPE * pstIspReg;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));

	ISP_DRC_RG_CTRL = pstIspReg->ISP_DRC_RG_CTRL;
	
	ISP_DRC_RG_CTRL.bits.rg_ctr = rg_ctr;
	ISP_DRC_RG_CTRL.bits.rg_wid = rg_wid;
	ISP_DRC_RG_CTRL.bits.rg_slo = rg_slo;

	pstIspReg->ISP_DRC_RG_CTRL = ISP_DRC_RG_CTRL;
}

/*****************************************************
U_ISP_DRC_BG_CTRL:  DRC  gain limit  
0x113a6374:             31:08    reserved
 bg_thr                : 8   
 bg_slo                : 3   
*****************************************************/
#define HI_ISP_DRC_BG_THR_DEFAULT (89)
#define HI_ISP_DRC_BG_SLO_DEFAULT (2)
static __inline HI_VOID hi_isp_drc_bg_ctrl_write(HI_U8 ispDev, HI_U8 bg_thr, HI_U8 bg_slo) {
	U_ISP_DRC_BG_CTRL   ISP_DRC_BG_CTRL;
	S_HIISP_REGS_TYPE * pstIspReg;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));

	ISP_DRC_BG_CTRL = pstIspReg->ISP_DRC_BG_CTRL;
	
	ISP_DRC_BG_CTRL.bits.bg_thr = bg_thr;
	ISP_DRC_BG_CTRL.bits.bg_slo = bg_slo;

	pstIspReg->ISP_DRC_BG_CTRL = ISP_DRC_BG_CTRL;
}

/*****************************************************
U_ISP_DRC_PDW:  DRC
0x113a6378:             31:08    reserved

low_thr               : 8  
low_slo               : 4  
 
high_thr              : 8  
high_slo              : 4  

*****************************************************/
#define HI_ISP_DRC_PDW_LOW_SLO_DEFAULT (12)
#define HI_ISP_DRC_PDW_LOW_THR_DEFAULT (20)
#define HI_ISP_DRC_PDW_HIG_SLO_DEFAULT (8)
#define HI_ISP_DRC_PDW_HIG_THR_DEFAULT (35)
static __inline HI_VOID hi_isp_drc_pdw_low_write(HI_U8 ispDev, HI_U8 low_thr, HI_U8 low_slo) {
	U_ISP_DRC_PDW   ISP_DRC_PDW;
	S_HIISP_REGS_TYPE * pstIspReg;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));

	ISP_DRC_PDW = pstIspReg->ISP_DRC_PDW;
	
	ISP_DRC_PDW.bits.low_thr = low_thr;
	ISP_DRC_PDW.bits.low_slo = low_slo;

	pstIspReg->ISP_DRC_PDW = ISP_DRC_PDW;
}

static __inline HI_VOID hi_isp_drc_pdw_hig_write(HI_U8 ispDev, HI_U8 high_thr, HI_U8 high_slo) {
	U_ISP_DRC_PDW   ISP_DRC_PDW;
	S_HIISP_REGS_TYPE * pstIspReg;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));

	ISP_DRC_PDW = pstIspReg->ISP_DRC_PDW;
	
	ISP_DRC_PDW.bits.high_thr = high_thr;
	ISP_DRC_PDW.bits.high_slo = high_slo;

	pstIspReg->ISP_DRC_PDW = ISP_DRC_PDW;
}


static __inline HI_U8 hi_isp_drc_pdw_hig_read(HI_U8 ispDev) {
	U_ISP_DRC_PDW   ISP_DRC_PDW;
	S_HIISP_REGS_TYPE * pstIspReg;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_PDW.u32 = pstIspReg->ISP_DRC_PDW.u32;

	return ISP_DRC_PDW.bits.high_thr;
	
}


static __inline HI_VOID hi_isp_drc_pdw_hig_thr_write(HI_U8 ispDev, HI_U8 high_thr) {
	U_ISP_DRC_PDW   ISP_DRC_PDW;
	S_HIISP_REGS_TYPE * pstIspReg;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));

	ISP_DRC_PDW = pstIspReg->ISP_DRC_PDW;
	
	ISP_DRC_PDW.bits.high_thr = high_thr;

	pstIspReg->ISP_DRC_PDW = ISP_DRC_PDW;
}


/*****************************************************
ISP_DRC_STAT_IND_WADDR   DRC ToneMapping
0x205a6180:                 31:00     drc_stat_ind_waddr
*****************************************************/
static __inline HI_VOID hi_isp_drc_stat_ind_waddr0_write(HI_U8 ispDev, HI_U32 data) { 
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_STAT_IND_WADDR0 ISP_DRC_STAT_IND_WADDR0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_STAT_IND_WADDR0.u32 = pstIspReg->ISP_DRC_STAT_IND_WADDR0.u32;
	ISP_DRC_STAT_IND_WADDR0.bits.drc_stat_ind_waddr0 = data;
	pstIspReg->ISP_DRC_STAT_IND_WADDR0.u32 = ISP_DRC_STAT_IND_WADDR0.u32;
}

/*****************************************************
ISP_DRC_STAT_IND_WDATA   DRC ToneMapping
0x205a6184:                 31:00     drc_stat_ind_wdata
*****************************************************/
static __inline HI_VOID hi_isp_drc_stat_ind_wdata0_write(HI_U8 ispDev, HI_U32 data) { 
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_STAT_IND_WDATA0 ISP_DRC_STAT_IND_WDATA0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_STAT_IND_WDATA0.u32 = pstIspReg->ISP_DRC_STAT_IND_WDATA0.u32;
	ISP_DRC_STAT_IND_WDATA0.bits.drc_stat_ind_wdata0 = data;
	pstIspReg->ISP_DRC_STAT_IND_WDATA0.u32 = ISP_DRC_STAT_IND_WDATA0.u32;
}

/*****************************************************
ISP_DRC_STAT_IND_WADDR   DRC ToneMapping
0x205a6190:                 31:00     drc_stat_ind_waddr
*****************************************************/
static __inline HI_VOID hi_isp_drc_stat_ind_waddr1_write(HI_U8 ispDev, HI_U32 data) { 
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_STAT_IND_WADDR1 ISP_DRC_STAT_IND_WADDR1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_STAT_IND_WADDR1.u32 = pstIspReg->ISP_DRC_STAT_IND_WADDR1.u32;
	ISP_DRC_STAT_IND_WADDR1.bits.drc_stat_ind_waddr1 = data;
	pstIspReg->ISP_DRC_STAT_IND_WADDR1.u32 = ISP_DRC_STAT_IND_WADDR1.u32;
}

/*****************************************************
ISP_DRC_STAT_IND_WDATA   DRC ToneMapping
0x205a6194:                 31:00     drc_stat_ind_wdata
*****************************************************/
static __inline HI_VOID hi_isp_drc_stat_ind_wdata1_write(HI_U8 ispDev, HI_U32 data) { 
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_STAT_IND_WDATA1 ISP_DRC_STAT_IND_WDATA1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_STAT_IND_WDATA1.u32 = pstIspReg->ISP_DRC_STAT_IND_WDATA1.u32;
	ISP_DRC_STAT_IND_WDATA1.bits.drc_stat_ind_wdata1 = data;
	pstIspReg->ISP_DRC_STAT_IND_WDATA1.u32 = ISP_DRC_STAT_IND_WDATA1.u32;
}

/*****************************************************
ISP_DRC_STAT_IND_RDATA  DRC ColorCorrectionLut
0x205a61a0:                 31:00     drc_stat_ind_rdata
*****************************************************/
static __inline HI_VOID hi_isp_drc_stat_ind_waddr2_write(HI_U8 ispDev, HI_U32 data) { 
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_STAT_IND_WADDR2 ISP_DRC_STAT_IND_WADDR2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_STAT_IND_WADDR2.u32 = pstIspReg->ISP_DRC_STAT_IND_WADDR2.u32;
	ISP_DRC_STAT_IND_WADDR2.bits.drc_stat_ind_waddr2 = data;
	pstIspReg->ISP_DRC_STAT_IND_WADDR2.u32 = ISP_DRC_STAT_IND_WADDR2.u32;
}

/*****************************************************
ISP_DRC_STAT_IND_WDATA   DRC ColorCorrectionLut
0x205a61a4:                 31:00     drc_stat_ind_wdata
*****************************************************/
static __inline HI_VOID hi_isp_drc_stat_ind_wdata2_write(HI_U8 ispDev, HI_U32 data) { 
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_STAT_IND_WDATA2 ISP_DRC_STAT_IND_WDATA2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_STAT_IND_WDATA2.u32 = pstIspReg->ISP_DRC_STAT_IND_WDATA2.u32;
	ISP_DRC_STAT_IND_WDATA2.bits.drc_stat_ind_wdata2 = data;
	pstIspReg->ISP_DRC_STAT_IND_WDATA2.u32 = ISP_DRC_STAT_IND_WDATA2.u32;
}

/*****************************************************
ISP_DRC_GRAD_REV   DRC
0x205a61a4:                 31:00     grad_rev_shift
*****************************************************/
#define HI_ISP_DRC_GRADREV_SHIFT_DEFAULT (2)
static __inline HI_VOID hi_isp_drc_grad_rev_shift_write(HI_U8 ispDev, HI_U8 data) { 
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_GRAD_REV ISP_DRC_GRAD_REV;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_GRAD_REV.u32 = pstIspReg->ISP_DRC_GRAD_REV.u32;
	ISP_DRC_GRAD_REV.bits.grad_rev_shift = data;
	pstIspReg->ISP_DRC_GRAD_REV.u32 = ISP_DRC_GRAD_REV.u32;
}
#define HI_ISP_DRC_GRADREV_SLOPE_DEFAULT (13)
static __inline HI_VOID hi_isp_drc_grad_rev_slope_write(HI_U8 ispDev, HI_U8 data) { 
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_GRAD_REV ISP_DRC_GRAD_REV;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_GRAD_REV.u32 = pstIspReg->ISP_DRC_GRAD_REV.u32;
	ISP_DRC_GRAD_REV.bits.grad_rev_slope = data;
	pstIspReg->ISP_DRC_GRAD_REV.u32 = ISP_DRC_GRAD_REV.u32;
}
#define HI_ISP_DRC_GRADREV_MAX_DEFAULT (0)
static __inline HI_VOID hi_isp_drc_grad_rev_max_write(HI_U8 ispDev, HI_U8 data) { 
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_GRAD_REV ISP_DRC_GRAD_REV;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_GRAD_REV.u32 = pstIspReg->ISP_DRC_GRAD_REV.u32;
	ISP_DRC_GRAD_REV.bits.grad_rev_max = data;
	pstIspReg->ISP_DRC_GRAD_REV.u32 = ISP_DRC_GRAD_REV.u32;
}
#define HI_ISP_DRC_GRADREV_THRES_DEFAULT (44)
static __inline HI_VOID hi_isp_drc_grad_rev_thres_write(HI_U8 ispDev, HI_U8 data) { 
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_GRAD_REV ISP_DRC_GRAD_REV;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_GRAD_REV.u32 = pstIspReg->ISP_DRC_GRAD_REV.u32;
	ISP_DRC_GRAD_REV.bits.grad_rev_thres = data;
	pstIspReg->ISP_DRC_GRAD_REV.u32 = ISP_DRC_GRAD_REV.u32;
}
#define HI_ISP_DRC_VAR_RNG_FINE_DEFAULT (8)
static __inline HI_VOID hi_isp_drc_var_rng_fine_write(HI_U8 ispDev, HI_U8 data) { 
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_VBI_STRENGTH ISP_DRC_VBI_STRENGTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_VBI_STRENGTH.u32 = pstIspReg->ISP_DRC_VBI_STRENGTH.u32;
	ISP_DRC_VBI_STRENGTH.bits.var_rng_fine = data;
	pstIspReg->ISP_DRC_VBI_STRENGTH.u32 = ISP_DRC_VBI_STRENGTH.u32;
}

static __inline HI_U8 hi_isp_drc_var_rng_fine_read(HI_U8 ispDev) { 
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_VBI_STRENGTH ISP_DRC_VBI_STRENGTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_VBI_STRENGTH.u32 = pstIspReg->ISP_DRC_VBI_STRENGTH.u32;
    return ISP_DRC_VBI_STRENGTH.bits.var_rng_fine ;
}

#define HI_ISP_DRC_VAR_RNG_MEDIUM_DEFAULT (15)
static __inline HI_VOID hi_isp_drc_var_rng_medium_write(HI_U8 ispDev, HI_U8 data) { 
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_VBI_STRENGTH ISP_DRC_VBI_STRENGTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_VBI_STRENGTH.u32 = pstIspReg->ISP_DRC_VBI_STRENGTH.u32;
	ISP_DRC_VBI_STRENGTH.bits.var_rng_medium = data;
	pstIspReg->ISP_DRC_VBI_STRENGTH.u32 = ISP_DRC_VBI_STRENGTH.u32;
}

#define HI_ISP_DRC_VAR_RNG_COARSE_DEFAULT (15)
static __inline HI_VOID hi_isp_drc_var_rng_coarse_write(HI_U8 ispDev, HI_U8 data) { 
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_VBI_STRENGTH ISP_DRC_VBI_STRENGTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_VBI_STRENGTH.u32 = pstIspReg->ISP_DRC_VBI_STRENGTH.u32;
	ISP_DRC_VBI_STRENGTH.bits.var_rng_coarse = data;
	pstIspReg->ISP_DRC_VBI_STRENGTH.u32 = ISP_DRC_VBI_STRENGTH.u32;
}

#define HI_ISP_DRC_VARSAPFINE_DEFAULT (6)
static __inline HI_VOID hi_isp_drc_var_spa_fine_write(HI_U8 ispDev, HI_U8 data) { 
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_VBI_STRENGTH ISP_DRC_VBI_STRENGTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_VBI_STRENGTH.u32 = pstIspReg->ISP_DRC_VBI_STRENGTH.u32;
	ISP_DRC_VBI_STRENGTH.bits.var_spa_fine = data;
	pstIspReg->ISP_DRC_VBI_STRENGTH.u32 = ISP_DRC_VBI_STRENGTH.u32;
}

#define HI_ISP_DRC_VARSAPMEDIUM_DEFAULT (8)
static __inline HI_VOID hi_isp_drc_var_spa_medium_write(HI_U8 ispDev, HI_U8 data) { 
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_VBI_STRENGTH ISP_DRC_VBI_STRENGTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_VBI_STRENGTH.u32 = pstIspReg->ISP_DRC_VBI_STRENGTH.u32;
	ISP_DRC_VBI_STRENGTH.bits.var_spa_medium = data;
	pstIspReg->ISP_DRC_VBI_STRENGTH.u32 = ISP_DRC_VBI_STRENGTH.u32;
}

#define HI_ISP_DRC_VARSAPCOARSE_DEFAULT (15)
static __inline HI_VOID hi_isp_drc_var_sap_coarse_write(HI_U8 ispDev, HI_U8 data) { 
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_VBI_STRENGTH ISP_DRC_VBI_STRENGTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_VBI_STRENGTH.u32 = pstIspReg->ISP_DRC_VBI_STRENGTH.u32;
	ISP_DRC_VBI_STRENGTH.bits.var_sap_coarse = data;
	pstIspReg->ISP_DRC_VBI_STRENGTH.u32 = ISP_DRC_VBI_STRENGTH.u32;
}

#define HI_ISP_DRC_BINMIXMEDIUM_DEFAULT (6)
static __inline HI_VOID hi_isp_drc_bin_mix_medium_write(HI_U8 ispDev, HI_U8 data) { 
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_VBI_MIXING ISP_DRC_VBI_MIXING;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_VBI_MIXING.u32 = pstIspReg->ISP_DRC_VBI_MIXING.u32;
	ISP_DRC_VBI_MIXING.bits.bin_mix_medium = data;
	pstIspReg->ISP_DRC_VBI_MIXING.u32 = ISP_DRC_VBI_MIXING.u32;
}

static __inline HI_U8 hi_isp_drc_bin_mix_medium_read(HI_U8 ispDev) { 
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_VBI_MIXING ISP_DRC_VBI_MIXING;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_VBI_MIXING.u32 = pstIspReg->ISP_DRC_VBI_MIXING.u32;
	return ISP_DRC_VBI_MIXING.bits.bin_mix_medium;
	
}


#define HI_ISP_DRC_BINMIXCOARSE_DEFAULT (10)
static __inline HI_VOID hi_isp_drc_bin_mix_coarse_write(HI_U8 ispDev, HI_U8 data) { 
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_VBI_MIXING ISP_DRC_VBI_MIXING;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_VBI_MIXING.u32 = pstIspReg->ISP_DRC_VBI_MIXING.u32;
	ISP_DRC_VBI_MIXING.bits.bin_mix_coarse = data;
	pstIspReg->ISP_DRC_VBI_MIXING.u32 = ISP_DRC_VBI_MIXING.u32;
}

#if 0
#define HI_ISP_DRC_VARRNG_DEFAULT (10)
static __inline HI_VOID hi_isp_drc_var_rng_write(HI_U8 ispDev, HI_U8 data) {
S_HIISP_REGS_TYPE * pstIspReg;	
U_ISP_DRC_FLT_STRENGTH ISP_DRC_FLT_STRENGTH;
pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
ISP_DRC_FLT_STRENGTH.u32 = pstIspReg->ISP_DRC_FLT_STRENGTH.u32;
ISP_DRC_FLT_STRENGTH.bits.var_rng = data;
pstIspReg->ISP_DRC_FLT_STRENGTH.u32 = ISP_DRC_FLT_STRENGTH.u32;
}

static __inline HI_U8 hi_isp_drc_var_rng_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	return pstIspReg->ISP_DRC_FLT_STRENGTH.bits.var_rng;
}
#endif

/*****************************************************
ISP_DRC_SIZE:      
0x205a61f0:                     31:29    reserved
                                          28:16   height
                                          15:13   reserved
                                          12:  0   width
*****************************************************/
static __inline HI_VOID hi_isp_drc_height_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_SIZE ISP_DRC_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_SIZE.u32 = pstIspReg->ISP_DRC_SIZE.u32;
	ISP_DRC_SIZE.bits.vsize = data;
	pstIspReg->ISP_DRC_SIZE.u32 = ISP_DRC_SIZE.u32;
}

static __inline HI_VOID hi_isp_drc_width_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DRC_SIZE ISP_DRC_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DRC_SIZE.u32 = pstIspReg->ISP_DRC_SIZE.u32;
	ISP_DRC_SIZE.bits.hsize = data;
	pstIspReg->ISP_DRC_SIZE.u32 = ISP_DRC_SIZE.u32;
}

/*****************************************************
MODULE        -----GAMMA-----
MODULE        -----GAMMA-----
MODULE        -----GAMMA-----
*****************************************************/
// args: data (1-bit)
static __inline HI_VOID hi_isp_gamma_enable_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GAMMA_CFG ISP_GAMMA_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GAMMA_CFG.u32 = pstIspReg->ISP_GAMMA_CFG.u32;
	ISP_GAMMA_CFG.bits.en = data;
	pstIspReg->ISP_GAMMA_CFG.u32 = ISP_GAMMA_CFG.u32;
}
static __inline HI_U8 hi_isp_gamma_enable_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GAMMA_CFG ISP_GAMMA_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GAMMA_CFG.u32 = pstIspReg->ISP_GAMMA_CFG.u32;
	return ISP_GAMMA_CFG.bits.en;
}

static __inline HI_VOID hi_isp_gamma_lut_update_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GAMMA_LUT_UPDATE ISP_GAMMA_LUT_UPDATE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GAMMA_LUT_UPDATE.u32 = pstIspReg->ISP_GAMMA_LUT_UPDATE.u32;
	ISP_GAMMA_LUT_UPDATE.bits.update = data;
	pstIspReg->ISP_GAMMA_LUT_UPDATE.u32 = ISP_GAMMA_LUT_UPDATE.u32;
}

static __inline HI_VOID hi_isp_gamma_waddr_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GAMMA_WADDR ISP_GAMMA_WADDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GAMMA_WADDR.u32 = pstIspReg->ISP_GAMMA_WADDR.u32;
	ISP_GAMMA_WADDR.bits.gamma_waddr = data;
	pstIspReg->ISP_GAMMA_WADDR.u32 = ISP_GAMMA_WADDR.u32;
}

static __inline HI_VOID hi_isp_gamma_wdata_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GAMMA_WDATA ISP_GAMMA_WDATA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GAMMA_WDATA.u32 = pstIspReg->ISP_GAMMA_WDATA.u32;
	ISP_GAMMA_WDATA.bits.gamma_wdata = data;
	pstIspReg->ISP_GAMMA_WDATA.u32 = ISP_GAMMA_WDATA.u32;
}

static __inline HI_VOID hi_isp_gamma_raddr_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GAMMA_RADDR ISP_GAMMA_RADDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GAMMA_RADDR.u32 = pstIspReg->ISP_GAMMA_RADDR.u32;
	ISP_GAMMA_RADDR.bits.gamma_raddr = data;
	pstIspReg->ISP_GAMMA_RADDR.u32 = ISP_GAMMA_RADDR.u32;
}

static __inline HI_U32 hi_isp_gamma_rdata_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GAMMA_RDATA ISP_GAMMA_RDATA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GAMMA_RDATA.u32 = pstIspReg->ISP_GAMMA_RDATA.u32;
	return ISP_GAMMA_RDATA.bits.gamma_rdata;
}

/*****************************************************
MODULE        -----CA-----
MODULE        -----CA-----
MODULE        -----CA-----
*****************************************************/
static __inline HI_VOID hi_isp_ca_cfg_en_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_CFG ISP_CA_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_CFG.u32 = pstIspReg->ISP_CA_CFG.u32;
	ISP_CA_CFG.bits.en = data;
	pstIspReg->ISP_CA_CFG.u32 = ISP_CA_CFG.u32;

}

static __inline HI_U8 hi_isp_ca_cfg_en_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_CFG ISP_CA_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_CFG.u32 = pstIspReg->ISP_CA_CFG.u32;
	return ISP_CA_CFG.bits.en;
}

static __inline HI_VOID hi_isp_ca_llhcproc_en_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_CTRL ISP_CA_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_CTRL.u32 = pstIspReg->ISP_CA_CTRL.u32;
	ISP_CA_CTRL.bits.llhcproc_en = data;
	pstIspReg->ISP_CA_CTRL.u32 = ISP_CA_CTRL.u32;
}

static __inline HI_U8 hi_isp_ca_llhcproc_en_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_CTRL ISP_CA_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_CTRL.u32 = pstIspReg->ISP_CA_CTRL.u32;
	return ISP_CA_CTRL.bits.llhcproc_en ;
}

static __inline HI_VOID hi_isp_ca_skinproc_en_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_CTRL ISP_CA_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_CTRL.u32 = pstIspReg->ISP_CA_CTRL.u32;
	ISP_CA_CTRL.bits.skinproc_en = data;
	pstIspReg->ISP_CA_CTRL.u32 = ISP_CA_CTRL.u32;
}

static __inline HI_U8 hi_isp_ca_skinproc_en_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_CTRL ISP_CA_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_CTRL.u32 = pstIspReg->ISP_CA_CTRL.u32;
	return ISP_CA_CTRL.bits.skinproc_en ;

}


static __inline HI_VOID hi_isp_ca_colorproc_en_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_CTRL ISP_CA_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_CTRL.u32 = pstIspReg->ISP_CA_CTRL.u32;
	ISP_CA_CTRL.bits.colorproc_en = data;
	pstIspReg->ISP_CA_CTRL.u32 = ISP_CA_CTRL.u32;
}

static __inline HI_U8 hi_isp_ca_colorproc_en_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_CTRL ISP_CA_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_CTRL.u32 = pstIspReg->ISP_CA_CTRL.u32;
	return ISP_CA_CTRL.bits.colorproc_en ;

}

static __inline HI_VOID hi_isp_ca_satadj_en_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_CTRL ISP_CA_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_CTRL.u32 = pstIspReg->ISP_CA_CTRL.u32;
	ISP_CA_CTRL.bits.satadj_en = data;
	pstIspReg->ISP_CA_CTRL.u32 = ISP_CA_CTRL.u32;
}

static __inline HI_U8 hi_isp_ca_satadj_en_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_CTRL ISP_CA_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_CTRL.u32 = pstIspReg->ISP_CA_CTRL.u32;
	return ISP_CA_CTRL.bits.satadj_en ;
}


#define HI_ISP_CA_LUMA_THD_LOW_DEFAULT (250)
static __inline HI_VOID hi_isp_ca_lumath_low_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_LUMATH ISP_CA_LUMATH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_LUMATH.u32 = pstIspReg->ISP_CA_LUMATH.u32;
	ISP_CA_LUMATH.bits.lumath_low = data;
	pstIspReg->ISP_CA_LUMATH.u32 = ISP_CA_LUMATH.u32;
}

static __inline HI_VOID hi_isp_ca_lumath_high_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_LUMATH ISP_CA_LUMATH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_LUMATH.u32 = pstIspReg->ISP_CA_LUMATH.u32;
	ISP_CA_LUMATH.bits.lumath_high = data;
	pstIspReg->ISP_CA_LUMATH.u32 = ISP_CA_LUMATH.u32;
}

#define HI_ISP_CA_DARKCHROMA_THD_LOW_DEFAULT (135*4)
static __inline HI_VOID hi_isp_ca_darkchromath_low_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_DARKCHROMA_TH ISP_CA_DARKCHROMA_TH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_DARKCHROMA_TH.u32 = pstIspReg->ISP_CA_DARKCHROMA_TH.u32;
	ISP_CA_DARKCHROMA_TH.bits.darkchromath_low = data;
	pstIspReg->ISP_CA_DARKCHROMA_TH.u32 = ISP_CA_DARKCHROMA_TH.u32;
}

#define HI_ISP_CA_DARKCHROMA_THD_HIGH_DEFAULT (138*4)
static __inline HI_VOID hi_isp_ca_darkchromath_high_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_DARKCHROMA_TH ISP_CA_DARKCHROMA_TH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_DARKCHROMA_TH.u32 = pstIspReg->ISP_CA_DARKCHROMA_TH.u32;
	ISP_CA_DARKCHROMA_TH.bits.darkchromath_high = data;
	pstIspReg->ISP_CA_DARKCHROMA_TH.u32 = ISP_CA_DARKCHROMA_TH.u32;
}

#define HI_ISP_CA_SDARKCHROMA_THD_LOW_DEFAULT (135*4)
static __inline HI_VOID hi_isp_ca_sdarkchromath_low_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_SDARKCHROMA_TH ISP_CA_SDARKCHROMA_TH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_SDARKCHROMA_TH.u32 = pstIspReg->ISP_CA_SDARKCHROMA_TH.u32;
	ISP_CA_SDARKCHROMA_TH.bits.sdarkchromath_low = data;
	pstIspReg->ISP_CA_SDARKCHROMA_TH.u32 = ISP_CA_SDARKCHROMA_TH.u32;
}

#define HI_ISP_CA_SDARKCHROMA_THD_HIGH_DEFAULT (140*4)
static __inline HI_VOID hi_isp_ca_sdarkchromath_high_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_SDARKCHROMA_TH ISP_CA_SDARKCHROMA_TH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_SDARKCHROMA_TH.u32 = pstIspReg->ISP_CA_SDARKCHROMA_TH.u32;
	ISP_CA_SDARKCHROMA_TH.bits.sdarkchromath_high = data;
	pstIspReg->ISP_CA_SDARKCHROMA_TH.u32 = ISP_CA_SDARKCHROMA_TH.u32;
}

#define HI_ISP_CA_LUMA_RATIO_LOW_DEFAULT (800)
static __inline HI_VOID hi_isp_ca_lumaratio_low_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_LLHC_RATIO ISP_CA_LLHC_RATIO;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_LLHC_RATIO.u32 = pstIspReg->ISP_CA_LLHC_RATIO.u32;
	ISP_CA_LLHC_RATIO.bits.lumaratio_low = data;
	pstIspReg->ISP_CA_LLHC_RATIO.u32 = ISP_CA_LLHC_RATIO.u32;
}

static __inline HI_VOID hi_isp_ca_lumaratio_high_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_LLHC_RATIO ISP_CA_LLHC_RATIO;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_LLHC_RATIO.u32 = pstIspReg->ISP_CA_LLHC_RATIO.u32;
	ISP_CA_LLHC_RATIO.bits.lumaratio_high = data;
	pstIspReg->ISP_CA_LLHC_RATIO.u32 = ISP_CA_LLHC_RATIO.u32;
}

static __inline HI_VOID hi_isp_ca_isoratio_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_ISORATIO ISP_CA_ISORATIO;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_ISORATIO.u32 = pstIspReg->ISP_CA_ISORATIO.u32;
	ISP_CA_ISORATIO.bits.isoratio = data;
	pstIspReg->ISP_CA_ISORATIO.u32 = ISP_CA_ISORATIO.u32;
}

static __inline HI_VOID hi_isp_ca_lut_update_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_LUT_UPDATE ISP_CA_LUT_UPDATE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_LUT_UPDATE.u32 = pstIspReg->ISP_CA_LUT_UPDATE.u32;
	ISP_CA_LUT_UPDATE.bits.lut_update = data;
	pstIspReg->ISP_CA_LUT_UPDATE.u32 = ISP_CA_LUT_UPDATE.u32;
}

static __inline HI_VOID hi_isp_ca_yuv2rgb_coef00_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_YUV2RGB_COEF0 ISP_CA_YUV2RGB_COEF0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_YUV2RGB_COEF0.u32 = pstIspReg->ISP_CA_YUV2RGB_COEF0.u32;
	ISP_CA_YUV2RGB_COEF0.bits.yuv2rgb_coef00 = data;
	pstIspReg->ISP_CA_YUV2RGB_COEF0.u32 = ISP_CA_YUV2RGB_COEF0.u32;
}

static __inline HI_VOID hi_isp_ca_yuv2rgb_coef01_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_YUV2RGB_COEF0 ISP_CA_YUV2RGB_COEF0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_YUV2RGB_COEF0.u32 = pstIspReg->ISP_CA_YUV2RGB_COEF0.u32;
	ISP_CA_YUV2RGB_COEF0.bits.yuv2rgb_coef01 = data;
	pstIspReg->ISP_CA_YUV2RGB_COEF0.u32 = ISP_CA_YUV2RGB_COEF0.u32;
}

static __inline HI_VOID hi_isp_ca_yuv2rgb_coef02_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_YUV2RGB_COEF1 ISP_CA_YUV2RGB_COEF1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_YUV2RGB_COEF1.u32 = pstIspReg->ISP_CA_YUV2RGB_COEF1.u32;
	ISP_CA_YUV2RGB_COEF1.bits.yuv2rgb_coef02 = data;
	pstIspReg->ISP_CA_YUV2RGB_COEF1.u32 = ISP_CA_YUV2RGB_COEF1.u32;
}

static __inline HI_VOID hi_isp_ca_yuv2rgb_coef10_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_YUV2RGB_COEF1 ISP_CA_YUV2RGB_COEF1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_YUV2RGB_COEF1.u32 = pstIspReg->ISP_CA_YUV2RGB_COEF1.u32;
	ISP_CA_YUV2RGB_COEF1.bits.yuv2rgb_coef10 = data;
	pstIspReg->ISP_CA_YUV2RGB_COEF1.u32 = ISP_CA_YUV2RGB_COEF1.u32;
}

static __inline HI_VOID hi_isp_ca_yuv2rgb_coef11_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_YUV2RGB_COEF2 ISP_CA_YUV2RGB_COEF2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_YUV2RGB_COEF2.u32 = pstIspReg->ISP_CA_YUV2RGB_COEF2.u32;
	ISP_CA_YUV2RGB_COEF2.bits.yuv2rgb_coef11 = data;
	pstIspReg->ISP_CA_YUV2RGB_COEF2.u32 = ISP_CA_YUV2RGB_COEF2.u32;
}

static __inline HI_VOID hi_isp_ca_yuv2rgb_coef12_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_YUV2RGB_COEF2 ISP_CA_YUV2RGB_COEF2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_YUV2RGB_COEF2.u32 = pstIspReg->ISP_CA_YUV2RGB_COEF2.u32;
	ISP_CA_YUV2RGB_COEF2.bits.yuv2rgb_coef12 = data;
	pstIspReg->ISP_CA_YUV2RGB_COEF2.u32 = ISP_CA_YUV2RGB_COEF2.u32;
}

static __inline HI_VOID hi_isp_ca_yuv2rgb_coef20_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_YUV2RGB_COEF3 ISP_CA_YUV2RGB_COEF3;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_YUV2RGB_COEF3.u32 = pstIspReg->ISP_CA_YUV2RGB_COEF3.u32;
	ISP_CA_YUV2RGB_COEF3.bits.yuv2rgb_coef20 = data;
	pstIspReg->ISP_CA_YUV2RGB_COEF3.u32 = ISP_CA_YUV2RGB_COEF3.u32;
}

static __inline HI_VOID hi_isp_ca_yuv2rgb_coef21_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_YUV2RGB_COEF3 ISP_CA_YUV2RGB_COEF3;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_YUV2RGB_COEF3.u32 = pstIspReg->ISP_CA_YUV2RGB_COEF3.u32;
	ISP_CA_YUV2RGB_COEF3.bits.yuv2rgb_coef21 = data;
	pstIspReg->ISP_CA_YUV2RGB_COEF3.u32 = ISP_CA_YUV2RGB_COEF3.u32;
}

static __inline HI_VOID hi_isp_ca_yuv2rgb_coef22_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_YUV2RGB_COEF4 ISP_CA_YUV2RGB_COEF4;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_YUV2RGB_COEF4.u32 = pstIspReg->ISP_CA_YUV2RGB_COEF4.u32;
	ISP_CA_YUV2RGB_COEF4.bits.yuv2rgb_coef22 = data;
	pstIspReg->ISP_CA_YUV2RGB_COEF4.u32 = ISP_CA_YUV2RGB_COEF4.u32;
}

static __inline HI_VOID hi_isp_ca_yuv2rgb_indc0_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_YUV2RGB_INDC0 ISP_CA_YUV2RGB_INDC0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_YUV2RGB_INDC0.u32 = pstIspReg->ISP_CA_YUV2RGB_INDC0.u32;
	ISP_CA_YUV2RGB_INDC0.bits.yuv2rgb_indc0 = data;
	pstIspReg->ISP_CA_YUV2RGB_INDC0.u32 = ISP_CA_YUV2RGB_INDC0.u32;
}

static __inline HI_VOID hi_isp_ca_yuv2rgb_indc1_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_YUV2RGB_INDC0 ISP_CA_YUV2RGB_INDC0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_YUV2RGB_INDC0.u32 = pstIspReg->ISP_CA_YUV2RGB_INDC0.u32;
	ISP_CA_YUV2RGB_INDC0.bits.yuv2rgb_indc1 = data;
	pstIspReg->ISP_CA_YUV2RGB_INDC0.u32 = ISP_CA_YUV2RGB_INDC0.u32;
}

static __inline HI_VOID hi_isp_ca_yuv2rgb_indc2_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_YUV2RGB_INDC1 ISP_CA_YUV2RGB_INDC1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_YUV2RGB_INDC1.u32 = pstIspReg->ISP_CA_YUV2RGB_INDC1.u32;
	ISP_CA_YUV2RGB_INDC1.bits.yuv2rgb_indc2 = data;
	pstIspReg->ISP_CA_YUV2RGB_INDC1.u32 = ISP_CA_YUV2RGB_INDC1.u32;
}

static __inline HI_VOID hi_isp_ca_yuv2rgb_outdc0_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_YUV2RGB_OUTDC0 ISP_CA_YUV2RGB_OUTDC0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_YUV2RGB_OUTDC0.u32 = pstIspReg->ISP_CA_YUV2RGB_OUTDC0.u32;
	ISP_CA_YUV2RGB_OUTDC0.bits.yuv2rgb_outdc0 = data;
	pstIspReg->ISP_CA_YUV2RGB_OUTDC0.u32 = ISP_CA_YUV2RGB_OUTDC0.u32;
}

static __inline HI_VOID hi_isp_ca_yuv2rgb_outdc1_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_YUV2RGB_OUTDC0 ISP_CA_YUV2RGB_OUTDC0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_YUV2RGB_OUTDC0.u32 = pstIspReg->ISP_CA_YUV2RGB_OUTDC0.u32;
	ISP_CA_YUV2RGB_OUTDC0.bits.yuv2rgb_outdc1 = data;
	pstIspReg->ISP_CA_YUV2RGB_OUTDC0.u32 = ISP_CA_YUV2RGB_OUTDC0.u32;
}

static __inline HI_VOID hi_isp_ca_yuv2rgb_outdc2_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_YUV2RGB_OUTDC1 ISP_CA_YUV2RGB_OUTDC1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_YUV2RGB_OUTDC1.u32 = pstIspReg->ISP_CA_YUV2RGB_OUTDC1.u32;
	ISP_CA_YUV2RGB_OUTDC1.bits.yuv2rgb_outdc2 = data;
	pstIspReg->ISP_CA_YUV2RGB_OUTDC1.u32 = ISP_CA_YUV2RGB_OUTDC1.u32;
}

static __inline HI_VOID hi_isp_ca_rgb2yuv_coef00_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_RGB2YUV_COEF0 ISP_CA_RGB2YUV_COEF0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_RGB2YUV_COEF0.u32 = pstIspReg->ISP_CA_RGB2YUV_COEF0.u32;
	ISP_CA_RGB2YUV_COEF0.bits.rgb2yuv_coef00 = data;
	pstIspReg->ISP_CA_RGB2YUV_COEF0.u32 = ISP_CA_RGB2YUV_COEF0.u32;
}

static __inline HI_VOID hi_isp_ca_rgb2yuv_coef01_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_RGB2YUV_COEF0 ISP_CA_RGB2YUV_COEF0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_RGB2YUV_COEF0.u32 = pstIspReg->ISP_CA_RGB2YUV_COEF0.u32;
	ISP_CA_RGB2YUV_COEF0.bits.rgb2yuv_coef01 = data;
	pstIspReg->ISP_CA_RGB2YUV_COEF0.u32 = ISP_CA_RGB2YUV_COEF0.u32;
}

static __inline HI_VOID hi_isp_ca_rgb2yuv_coef02_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_RGB2YUV_COEF1 ISP_CA_RGB2YUV_COEF1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_RGB2YUV_COEF1.u32 = pstIspReg->ISP_CA_RGB2YUV_COEF1.u32;
	ISP_CA_RGB2YUV_COEF1.bits.rgb2yuv_coef02 = data;
	pstIspReg->ISP_CA_RGB2YUV_COEF1.u32 = ISP_CA_RGB2YUV_COEF1.u32;
}

static __inline HI_VOID hi_isp_ca_rgb2yuv_coef10_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_RGB2YUV_COEF1 ISP_CA_RGB2YUV_COEF1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_RGB2YUV_COEF1.u32 = pstIspReg->ISP_CA_RGB2YUV_COEF1.u32;
	ISP_CA_RGB2YUV_COEF1.bits.rgb2yuv_coef10 = data;
	pstIspReg->ISP_CA_RGB2YUV_COEF1.u32 = ISP_CA_RGB2YUV_COEF1.u32;
}

static __inline HI_VOID hi_isp_ca_rgb2yuv_coef11_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_RGB2YUV_COEF2 ISP_CA_RGB2YUV_COEF2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_RGB2YUV_COEF2.u32 = pstIspReg->ISP_CA_RGB2YUV_COEF2.u32;
	ISP_CA_RGB2YUV_COEF2.bits.rgb2yuv_coef11 = data;
	pstIspReg->ISP_CA_RGB2YUV_COEF2.u32 = ISP_CA_RGB2YUV_COEF2.u32;
}

static __inline HI_VOID hi_isp_ca_rgb2yuv_coef12_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_RGB2YUV_COEF2 ISP_CA_RGB2YUV_COEF2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_RGB2YUV_COEF2.u32 = pstIspReg->ISP_CA_RGB2YUV_COEF2.u32;
	ISP_CA_RGB2YUV_COEF2.bits.rgb2yuv_coef12 = data;
	pstIspReg->ISP_CA_RGB2YUV_COEF2.u32 = ISP_CA_RGB2YUV_COEF2.u32;
}

static __inline HI_VOID hi_isp_ca_rgb2yuv_coef20_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_RGB2YUV_COEF3 ISP_CA_RGB2YUV_COEF3;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_RGB2YUV_COEF3.u32 = pstIspReg->ISP_CA_RGB2YUV_COEF3.u32;
	ISP_CA_RGB2YUV_COEF3.bits.rgb2yuv_coef20 = data;
	pstIspReg->ISP_CA_RGB2YUV_COEF3.u32 = ISP_CA_RGB2YUV_COEF3.u32;
}

static __inline HI_VOID hi_isp_ca_rgb2yuv_coef21_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_RGB2YUV_COEF3 ISP_CA_RGB2YUV_COEF3;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_RGB2YUV_COEF3.u32 = pstIspReg->ISP_CA_RGB2YUV_COEF3.u32;
	ISP_CA_RGB2YUV_COEF3.bits.rgb2yuv_coef21 = data;
	pstIspReg->ISP_CA_RGB2YUV_COEF3.u32 = ISP_CA_RGB2YUV_COEF3.u32;
}

static __inline HI_VOID hi_isp_ca_rgb2yuv_coef22_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_RGB2YUV_COEF4 ISP_CA_RGB2YUV_COEF4;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_RGB2YUV_COEF4.u32 = pstIspReg->ISP_CA_RGB2YUV_COEF4.u32;
	ISP_CA_RGB2YUV_COEF4.bits.rgb2yuv_coef22 = data;
	pstIspReg->ISP_CA_RGB2YUV_COEF4.u32 = ISP_CA_RGB2YUV_COEF4.u32;
}

static __inline HI_VOID hi_isp_ca_rgb2yuv_indc0_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_RGB2YUV_INDC0 ISP_CA_RGB2YUV_INDC0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_RGB2YUV_INDC0.u32 = pstIspReg->ISP_CA_RGB2YUV_INDC0.u32;
	ISP_CA_RGB2YUV_INDC0.bits.rgb2yuv_indc0 = data;
	pstIspReg->ISP_CA_RGB2YUV_INDC0.u32 = ISP_CA_RGB2YUV_INDC0.u32;
}

static __inline HI_VOID hi_isp_ca_rgb2yuv_indc1_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_RGB2YUV_INDC0 ISP_CA_RGB2YUV_INDC0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_RGB2YUV_INDC0.u32 = pstIspReg->ISP_CA_RGB2YUV_INDC0.u32;
	ISP_CA_RGB2YUV_INDC0.bits.rgb2yuv_indc1 = data;
	pstIspReg->ISP_CA_RGB2YUV_INDC0.u32 = ISP_CA_RGB2YUV_INDC0.u32;
}

static __inline HI_VOID hi_isp_ca_rgb2yuv_indc2_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_RGB2YUV_INDC1 ISP_CA_RGB2YUV_INDC1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_RGB2YUV_INDC1.u32 = pstIspReg->ISP_CA_RGB2YUV_INDC1.u32;
	ISP_CA_RGB2YUV_INDC1.bits.rgb2yuv_indc2 = data;
	pstIspReg->ISP_CA_RGB2YUV_INDC1.u32 = ISP_CA_RGB2YUV_INDC1.u32;
}

static __inline HI_VOID hi_isp_ca_rgb2yuv_outdc0_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_RGB2YUV_OUTDC0 ISP_CA_RGB2YUV_OUTDC0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_RGB2YUV_OUTDC0.u32 = pstIspReg->ISP_CA_RGB2YUV_OUTDC0.u32;
	ISP_CA_RGB2YUV_OUTDC0.bits.rgb2yuv_outdc0 = data;
	pstIspReg->ISP_CA_RGB2YUV_OUTDC0.u32 = ISP_CA_RGB2YUV_OUTDC0.u32;
}

static __inline HI_VOID hi_isp_ca_rgb2yuv_outdc1_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_RGB2YUV_OUTDC0 ISP_CA_RGB2YUV_OUTDC0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_RGB2YUV_OUTDC0.u32 = pstIspReg->ISP_CA_RGB2YUV_OUTDC0.u32;
	ISP_CA_RGB2YUV_OUTDC0.bits.rgb2yuv_outdc1 = data;
	pstIspReg->ISP_CA_RGB2YUV_OUTDC0.u32 = ISP_CA_RGB2YUV_OUTDC0.u32;
}

static __inline HI_VOID hi_isp_ca_rgb2yuv_outdc2_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_RGB2YUV_OUTDC1 ISP_CA_RGB2YUV_OUTDC1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_RGB2YUV_OUTDC1.u32 = pstIspReg->ISP_CA_RGB2YUV_OUTDC1.u32;
	ISP_CA_RGB2YUV_OUTDC1.bits.rgb2yuv_outdc2 = data;
	pstIspReg->ISP_CA_RGB2YUV_OUTDC1.u32 = ISP_CA_RGB2YUV_OUTDC1.u32;
}

#define HI_ISP_CA_SKINLOWLUAM_UMIN_DEFAULT (115*4)
#define HI_ISP_CA_SKINLOWLUAM_UMAX_DEFAULT (128*4)
#define HI_ISP_CA_SKINLOWLUAM_UYMIN_DEFAULT (64)
#define HI_ISP_CA_SKINLOWLUAM_UYMAX_DEFAULT (400)
#define HI_ISP_CA_SKINHIGHLUAM_UMIN_DEFAULT (96*4)
#define HI_ISP_CA_SKINHIGHLUAM_UMAX_DEFAULT (128*4)
#define HI_ISP_CA_SKINHIGHLUAM_UYMIN_DEFAULT (400)
#define HI_ISP_CA_SKINHIGHLUAM_UYMAX_DEFAULT (960)

static __inline HI_VOID hi_isp_ca_skinlluma_umin_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_SKINLLUMA_UTH ISP_CA_SKINLLUMA_UTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_SKINLLUMA_UTH.u32 = pstIspReg->ISP_CA_SKINLLUMA_UTH.u32;
	ISP_CA_SKINLLUMA_UTH.bits.skinlluma_umin = data;
	pstIspReg->ISP_CA_SKINLLUMA_UTH.u32 = ISP_CA_SKINLLUMA_UTH.u32;
}

static __inline HI_VOID hi_isp_ca_skinlluma_umax_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_SKINLLUMA_UTH ISP_CA_SKINLLUMA_UTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_SKINLLUMA_UTH.u32 = pstIspReg->ISP_CA_SKINLLUMA_UTH.u32;
	ISP_CA_SKINLLUMA_UTH.bits.skinlluma_umax = data;
	pstIspReg->ISP_CA_SKINLLUMA_UTH.u32 = ISP_CA_SKINLLUMA_UTH.u32;
}

static __inline HI_VOID hi_isp_ca_skinlluma_uymin_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_SKINLLUMA_UYTH ISP_CA_SKINLLUMA_UYTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_SKINLLUMA_UYTH.u32 = pstIspReg->ISP_CA_SKINLLUMA_UYTH.u32;
	ISP_CA_SKINLLUMA_UYTH.bits.skinlluma_uymin = data;
	pstIspReg->ISP_CA_SKINLLUMA_UYTH.u32 = ISP_CA_SKINLLUMA_UYTH.u32;
}

static __inline HI_VOID hi_isp_ca_skinlluma_uymax_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_SKINLLUMA_UYTH ISP_CA_SKINLLUMA_UYTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_SKINLLUMA_UYTH.u32 = pstIspReg->ISP_CA_SKINLLUMA_UYTH.u32;
	ISP_CA_SKINLLUMA_UYTH.bits.skinlluma_uymax = data;
	pstIspReg->ISP_CA_SKINLLUMA_UYTH.u32 = ISP_CA_SKINLLUMA_UYTH.u32;
}

static __inline HI_VOID hi_isp_ca_skinhluma_umin_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_SKINHLUMA_UTH ISP_CA_SKINHLUMA_UTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_SKINHLUMA_UTH.u32 = pstIspReg->ISP_CA_SKINHLUMA_UTH.u32;
	ISP_CA_SKINHLUMA_UTH.bits.skinhluma_umin = data;
	pstIspReg->ISP_CA_SKINHLUMA_UTH.u32 = ISP_CA_SKINHLUMA_UTH.u32;
}

static __inline HI_VOID hi_isp_ca_skinhluma_umax_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_SKINHLUMA_UTH ISP_CA_SKINHLUMA_UTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_SKINHLUMA_UTH.u32 = pstIspReg->ISP_CA_SKINHLUMA_UTH.u32;
	ISP_CA_SKINHLUMA_UTH.bits.skinhluma_umax = data;
	pstIspReg->ISP_CA_SKINHLUMA_UTH.u32 = ISP_CA_SKINHLUMA_UTH.u32;
}

static __inline HI_VOID hi_isp_ca_gainlut_waddr_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_GAINLUT_WADDR ISP_CA_GAINLUT_WADDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_GAINLUT_WADDR.u32 = pstIspReg->ISP_CA_GAINLUT_WADDR.u32;
	ISP_CA_GAINLUT_WADDR.bits.gainlut_waddr = data;
	pstIspReg->ISP_CA_GAINLUT_WADDR.u32 = ISP_CA_GAINLUT_WADDR.u32;
}

static __inline HI_VOID hi_isp_ca_gainlut_wdata_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_GAINLUT_WDATA ISP_CA_GAINLUT_WDATA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_GAINLUT_WDATA.u32 = pstIspReg->ISP_CA_GAINLUT_WDATA.u32;
	ISP_CA_GAINLUT_WDATA.bits.gainlut_wdata = data;
	pstIspReg->ISP_CA_GAINLUT_WDATA.u32 = ISP_CA_GAINLUT_WDATA.u32;
}

static __inline HI_VOID hi_isp_ca_skinhluma_uymin_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_SKINHLUMA_UYTH ISP_CA_SKINHLUMA_UYTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_SKINHLUMA_UYTH.u32 = pstIspReg->ISP_CA_SKINHLUMA_UYTH.u32;
	ISP_CA_SKINHLUMA_UYTH.bits.skinhluma_uymin = data;
	pstIspReg->ISP_CA_SKINHLUMA_UYTH.u32 = ISP_CA_SKINHLUMA_UYTH.u32;
}

static __inline HI_VOID hi_isp_ca_skinhluma_uymax_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_SKINHLUMA_UYTH ISP_CA_SKINHLUMA_UYTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_SKINHLUMA_UYTH.u32 = pstIspReg->ISP_CA_SKINHLUMA_UYTH.u32;
	ISP_CA_SKINHLUMA_UYTH.bits.skinhluma_uymax = data;
	pstIspReg->ISP_CA_SKINHLUMA_UYTH.u32 = ISP_CA_SKINHLUMA_UYTH.u32;
}

#define HI_ISP_CA_SKINLOWLUAM_VMIN_DEFAULT (130*4)
#define HI_ISP_CA_SKINLOWLUAM_VMAX_DEFAULT (156*4+2)
#define HI_ISP_CA_SKINLOWLUAM_VYMIN_DEFAULT (64)
#define HI_ISP_CA_SKINLOWLUAM_VYMAX_DEFAULT (400)
#define HI_ISP_CA_SKINHIGHLUAM_VMIN_DEFAULT (130*4)
#define HI_ISP_CA_SKINHIGHLUAM_VMAX_DEFAULT (156*4+2)
#define HI_ISP_CA_SKINHIGHLUAM_VYMIN_DEFAULT (400)
#define HI_ISP_CA_SKINHIGHLUAM_VYMAX_DEFAULT (831)

static __inline HI_VOID hi_isp_ca_skinlluma_vmin_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_SKINLLUMA_VTH ISP_CA_SKINLLUMA_VTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_SKINLLUMA_VTH.u32 = pstIspReg->ISP_CA_SKINLLUMA_VTH.u32;
	ISP_CA_SKINLLUMA_VTH.bits.skinlluma_vmin = data;
	pstIspReg->ISP_CA_SKINLLUMA_VTH.u32 = ISP_CA_SKINLLUMA_VTH.u32;
}

static __inline HI_VOID hi_isp_ca_skinlluma_vmax_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_SKINLLUMA_VTH ISP_CA_SKINLLUMA_VTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_SKINLLUMA_VTH.u32 = pstIspReg->ISP_CA_SKINLLUMA_VTH.u32;
	ISP_CA_SKINLLUMA_VTH.bits.skinlluma_vmax = data;
	pstIspReg->ISP_CA_SKINLLUMA_VTH.u32 = ISP_CA_SKINLLUMA_VTH.u32;
}

static __inline HI_VOID hi_isp_ca_skinlluma_vymin_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_SKINLLUMA_VYTH ISP_CA_SKINLLUMA_VYTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_SKINLLUMA_VYTH.u32 = pstIspReg->ISP_CA_SKINLLUMA_VYTH.u32;
	ISP_CA_SKINLLUMA_VYTH.bits.skinlluma_vymin = data;
	pstIspReg->ISP_CA_SKINLLUMA_VYTH.u32 = ISP_CA_SKINLLUMA_VYTH.u32;
}

static __inline HI_VOID hi_isp_ca_skinlluma_vymax_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_SKINLLUMA_VYTH ISP_CA_SKINLLUMA_VYTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_SKINLLUMA_VYTH.u32 = pstIspReg->ISP_CA_SKINLLUMA_VYTH.u32;
	ISP_CA_SKINLLUMA_VYTH.bits.skinlluma_vymax = data;
	pstIspReg->ISP_CA_SKINLLUMA_VYTH.u32 = ISP_CA_SKINLLUMA_VYTH.u32;
}

static __inline HI_VOID hi_isp_ca_skinhluma_vmin_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_SKINHLUMA_VTH ISP_CA_SKINHLUMA_VTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_SKINHLUMA_VTH.u32 = pstIspReg->ISP_CA_SKINHLUMA_VTH.u32;
	ISP_CA_SKINHLUMA_VTH.bits.skinhluma_vmin = data;
	pstIspReg->ISP_CA_SKINHLUMA_VTH.u32 = ISP_CA_SKINHLUMA_VTH.u32;
}

static __inline HI_VOID hi_isp_ca_skinhluma_vmax_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_SKINHLUMA_VTH ISP_CA_SKINHLUMA_VTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_SKINHLUMA_VTH.u32 = pstIspReg->ISP_CA_SKINHLUMA_VTH.u32;
	ISP_CA_SKINHLUMA_VTH.bits.skinhluma_vmax = data;
	pstIspReg->ISP_CA_SKINHLUMA_VTH.u32 = ISP_CA_SKINHLUMA_VTH.u32;
}

static __inline HI_VOID hi_isp_ca_skinhluma_vymin_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_SKINHLUMA_VYTH ISP_CA_SKINHLUMA_VYTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_SKINHLUMA_VYTH.u32 = pstIspReg->ISP_CA_SKINHLUMA_VYTH.u32;
	ISP_CA_SKINHLUMA_VYTH.bits.skinhluma_vymin = data;
	pstIspReg->ISP_CA_SKINHLUMA_VYTH.u32 = ISP_CA_SKINHLUMA_VYTH.u32;
}

static __inline HI_VOID hi_isp_ca_skinhluma_vymax_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_SKINHLUMA_VYTH ISP_CA_SKINHLUMA_VYTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_SKINHLUMA_VYTH.u32 = pstIspReg->ISP_CA_SKINHLUMA_VYTH.u32;
	ISP_CA_SKINHLUMA_VYTH.bits.skinhluma_vymax = data;
	pstIspReg->ISP_CA_SKINHLUMA_VYTH.u32 = ISP_CA_SKINHLUMA_VYTH.u32;
}

#define HI_ISP_CA_SKINUVDIFF_DEFAULT (10*4)
#define HI_ISP_CA_SKINRATIOTHD_HIGH_DEFAULT (1500)
#define HI_ISP_CA_SKINRATIOTHD_MID_DEFAULT  (1300)
#define HI_ISP_CA_SKINRATIOTHD_LOW_DEFAULT (900)

static __inline HI_VOID hi_isp_ca_skin_uvdiff_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_SKIN_UVDIFF ISP_CA_SKIN_UVDIFF;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_SKIN_UVDIFF.u32 = pstIspReg->ISP_CA_SKIN_UVDIFF.u32;
	ISP_CA_SKIN_UVDIFF.bits.skin_uvdiff = data;
	pstIspReg->ISP_CA_SKIN_UVDIFF.u32 = ISP_CA_SKIN_UVDIFF.u32;
}

static __inline HI_VOID hi_isp_ca_skinratioth_low_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_SKIN_RATIOTH0 ISP_CA_SKIN_RATIOTH0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_SKIN_RATIOTH0.u32 = pstIspReg->ISP_CA_SKIN_RATIOTH0.u32;
	ISP_CA_SKIN_RATIOTH0.bits.skinratioth_low = data;
	pstIspReg->ISP_CA_SKIN_RATIOTH0.u32 = ISP_CA_SKIN_RATIOTH0.u32;
}

static __inline HI_VOID hi_isp_ca_skinratioth_mid_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_SKIN_RATIOTH0 ISP_CA_SKIN_RATIOTH0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_SKIN_RATIOTH0.u32 = pstIspReg->ISP_CA_SKIN_RATIOTH0.u32;
	ISP_CA_SKIN_RATIOTH0.bits.skinratioth_mid = data;
	pstIspReg->ISP_CA_SKIN_RATIOTH0.u32 = ISP_CA_SKIN_RATIOTH0.u32;
}

static __inline HI_VOID hi_isp_ca_skinratioth_high_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_SKIN_RATIOTH1 ISP_CA_SKIN_RATIOTH1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_SKIN_RATIOTH1.u32 = pstIspReg->ISP_CA_SKIN_RATIOTH1.u32;
	ISP_CA_SKIN_RATIOTH1.bits.skinratioth_high = data;
	pstIspReg->ISP_CA_SKIN_RATIOTH1.u32 = ISP_CA_SKIN_RATIOTH1.u32;
}

#define HI_ISP_CA_COLORLOWLUAM_UMIN_DEFAULT (0)
#define HI_ISP_CA_COLORLOWLUAM_UMAX_DEFAULT (0)
#define HI_ISP_CA_COLORLOWLUAM_UYMIN_DEFAULT (0)
#define HI_ISP_CA_COLORLOWLUAM_UYMAX_DEFAULT (0)
#define HI_ISP_CA_COLORHIGHLUAM_UMIN_DEFAULT (0)
#define HI_ISP_CA_COLORHIGHLUAM_UMAX_DEFAULT (0)
#define HI_ISP_CA_COLORHIGHLUAM_UYMIN_DEFAULT (0)
#define HI_ISP_CA_COLORHIGHLUAM_UYMAX_DEFAULT (0)

#define HI_ISP_CA_COLORLOWLUAM_VMIN_DEFAULT (0)
#define HI_ISP_CA_COLORLOWLUAM_VMAX_DEFAULT (0)
#define HI_ISP_CA_COLORLOWLUAM_VYMIN_DEFAULT (0)
#define HI_ISP_CA_COLORLOWLUAM_VYMAX_DEFAULT (0)
#define HI_ISP_CA_COLORHIGHLUAM_VMIN_DEFAULT (0)
#define HI_ISP_CA_COLORHIGHLUAM_VMAX_DEFAULT (0)
#define HI_ISP_CA_COLORHIGHLUAM_VYMIN_DEFAULT (0)
#define HI_ISP_CA_COLORHIGHLUAM_VYMAX_DEFAULT (0)

static __inline HI_VOID hi_isp_ca_colorlluma_umin_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_COLORLLUMA_UTH ISP_CA_COLORLLUMA_UTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_COLORLLUMA_UTH.u32 = pstIspReg->ISP_CA_COLORLLUMA_UTH.u32;
	ISP_CA_COLORLLUMA_UTH.bits.colorlluma_umin = data;
	pstIspReg->ISP_CA_COLORLLUMA_UTH.u32 = ISP_CA_COLORLLUMA_UTH.u32;
}

static __inline HI_VOID hi_isp_ca_colorlluma_umax_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_COLORLLUMA_UTH ISP_CA_COLORLLUMA_UTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_COLORLLUMA_UTH.u32 = pstIspReg->ISP_CA_COLORLLUMA_UTH.u32;
	ISP_CA_COLORLLUMA_UTH.bits.colorlluma_umax = data;
	pstIspReg->ISP_CA_COLORLLUMA_UTH.u32 = ISP_CA_COLORLLUMA_UTH.u32;
}

static __inline HI_VOID hi_isp_ca_colorlluma_uymin_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_COLORLLUMA_UYTH ISP_CA_COLORLLUMA_UYTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_COLORLLUMA_UYTH.u32 = pstIspReg->ISP_CA_COLORLLUMA_UYTH.u32;
	ISP_CA_COLORLLUMA_UYTH.bits.colorlluma_uymin = data;
	pstIspReg->ISP_CA_COLORLLUMA_UYTH.u32 = ISP_CA_COLORLLUMA_UYTH.u32;
}

static __inline HI_VOID hi_isp_ca_colorlluma_uymax_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_COLORLLUMA_UYTH ISP_CA_COLORLLUMA_UYTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_COLORLLUMA_UYTH.u32 = pstIspReg->ISP_CA_COLORLLUMA_UYTH.u32;
	ISP_CA_COLORLLUMA_UYTH.bits.colorlluma_uymax = data;
	pstIspReg->ISP_CA_COLORLLUMA_UYTH.u32 = ISP_CA_COLORLLUMA_UYTH.u32;
}

static __inline HI_VOID hi_isp_ca_colorhluma_umin_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_COLORHLUMA_UTH ISP_CA_COLORHLUMA_UTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_COLORHLUMA_UTH.u32 = pstIspReg->ISP_CA_COLORHLUMA_UTH.u32;
	ISP_CA_COLORHLUMA_UTH.bits.colorhluma_umin = data;
	pstIspReg->ISP_CA_COLORHLUMA_UTH.u32 = ISP_CA_COLORHLUMA_UTH.u32;
}

static __inline HI_VOID hi_isp_ca_colorhluma_umax_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_COLORHLUMA_UTH ISP_CA_COLORHLUMA_UTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_COLORHLUMA_UTH.u32 = pstIspReg->ISP_CA_COLORHLUMA_UTH.u32;
	ISP_CA_COLORHLUMA_UTH.bits.colorhluma_umax = data;
	pstIspReg->ISP_CA_COLORHLUMA_UTH.u32 = ISP_CA_COLORHLUMA_UTH.u32;
}

static __inline HI_VOID hi_isp_ca_colorhluma_uymin_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_COLORHLUMA_UYTH ISP_CA_COLORHLUMA_UYTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_COLORHLUMA_UYTH.u32 = pstIspReg->ISP_CA_COLORHLUMA_UYTH.u32;
	ISP_CA_COLORHLUMA_UYTH.bits.colorhluma_uymin = data;
	pstIspReg->ISP_CA_COLORHLUMA_UYTH.u32 = ISP_CA_COLORHLUMA_UYTH.u32;
}

static __inline HI_VOID hi_isp_ca_colorhluma_uymax_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_COLORHLUMA_UYTH ISP_CA_COLORHLUMA_UYTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_COLORHLUMA_UYTH.u32 = pstIspReg->ISP_CA_COLORHLUMA_UYTH.u32;
	ISP_CA_COLORHLUMA_UYTH.bits.colorhluma_uymax = data;
	pstIspReg->ISP_CA_COLORHLUMA_UYTH.u32 = ISP_CA_COLORHLUMA_UYTH.u32;
}

static __inline HI_VOID hi_isp_ca_colorlluma_vmin_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_COLORLLUMA_VTH ISP_CA_COLORLLUMA_VTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_COLORLLUMA_VTH.u32 = pstIspReg->ISP_CA_COLORLLUMA_VTH.u32;
	ISP_CA_COLORLLUMA_VTH.bits.colorlluma_vmin = data;
	pstIspReg->ISP_CA_COLORLLUMA_VTH.u32 = ISP_CA_COLORLLUMA_VTH.u32;
}

static __inline HI_VOID hi_isp_ca_colorlluma_vmax_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_COLORLLUMA_VTH ISP_CA_COLORLLUMA_VTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_COLORLLUMA_VTH.u32 = pstIspReg->ISP_CA_COLORLLUMA_VTH.u32;
	ISP_CA_COLORLLUMA_VTH.bits.colorlluma_vmax = data;
	pstIspReg->ISP_CA_COLORLLUMA_VTH.u32 = ISP_CA_COLORLLUMA_VTH.u32;
}

static __inline HI_VOID hi_isp_ca_colorlluma_vymin_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_COLORLLUMA_VYTH ISP_CA_COLORLLUMA_VYTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_COLORLLUMA_VYTH.u32 = pstIspReg->ISP_CA_COLORLLUMA_VYTH.u32;
	ISP_CA_COLORLLUMA_VYTH.bits.colorlluma_vymin = data;
	pstIspReg->ISP_CA_COLORLLUMA_VYTH.u32 = ISP_CA_COLORLLUMA_VYTH.u32;
}

static __inline HI_VOID hi_isp_ca_colorlluma_vymax_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_COLORLLUMA_VYTH ISP_CA_COLORLLUMA_VYTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_COLORLLUMA_VYTH.u32 = pstIspReg->ISP_CA_COLORLLUMA_VYTH.u32;
	ISP_CA_COLORLLUMA_VYTH.bits.colorlluma_vymax = data;
	pstIspReg->ISP_CA_COLORLLUMA_VYTH.u32 = ISP_CA_COLORLLUMA_VYTH.u32;
}

static __inline HI_VOID hi_isp_ca_colorhluma_vmin_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_COLORHLUMA_VTH ISP_CA_COLORHLUMA_VTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_COLORHLUMA_VTH.u32 = pstIspReg->ISP_CA_COLORHLUMA_VTH.u32;
	ISP_CA_COLORHLUMA_VTH.bits.colorhluma_vmin = data;
	pstIspReg->ISP_CA_COLORHLUMA_VTH.u32 = ISP_CA_COLORHLUMA_VTH.u32;
}

static __inline HI_VOID hi_isp_ca_colorhluma_vmax_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_COLORHLUMA_VTH ISP_CA_COLORHLUMA_VTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_COLORHLUMA_VTH.u32 = pstIspReg->ISP_CA_COLORHLUMA_VTH.u32;
	ISP_CA_COLORHLUMA_VTH.bits.colorhluma_vmax = data;
	pstIspReg->ISP_CA_COLORHLUMA_VTH.u32 = ISP_CA_COLORHLUMA_VTH.u32;
}

static __inline HI_VOID hi_isp_ca_colorhluma_vymin_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_COLORHLUMA_VYTH ISP_CA_COLORHLUMA_VYTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_COLORHLUMA_VYTH.u32 = pstIspReg->ISP_CA_COLORHLUMA_VYTH.u32;
	ISP_CA_COLORHLUMA_VYTH.bits.colorhluma_vymin = data;
	pstIspReg->ISP_CA_COLORHLUMA_VYTH.u32 = ISP_CA_COLORHLUMA_VYTH.u32;
}

static __inline HI_VOID hi_isp_ca_colorhluma_vymax_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_COLORHLUMA_VYTH ISP_CA_COLORHLUMA_VYTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_COLORHLUMA_VYTH.u32 = pstIspReg->ISP_CA_COLORHLUMA_VYTH.u32;
	ISP_CA_COLORHLUMA_VYTH.bits.colorhluma_vymax = data;
	pstIspReg->ISP_CA_COLORHLUMA_VYTH.u32 = ISP_CA_COLORHLUMA_VYTH.u32;
}

#define HI_ISP_CA_COLORUVDIFF_DEFAULT (0)
#define HI_ISP_CA_COLORRATIOTHD_HIGH_DEFAULT (0)
#define HI_ISP_CA_COLORRATIOTHD_MID_DEFAULT  (0)
#define HI_ISP_CA_COLORRATIOTHD_LOW_DEFAULT (0)

static __inline HI_VOID hi_isp_ca_color_uvdiff_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_COLOR_UVDIFF ISP_CA_COLOR_UVDIFF;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_COLOR_UVDIFF.u32 = pstIspReg->ISP_CA_COLOR_UVDIFF.u32;
	ISP_CA_COLOR_UVDIFF.bits.color_uvdiff = data;
	pstIspReg->ISP_CA_COLOR_UVDIFF.u32 = ISP_CA_COLOR_UVDIFF.u32;
}

static __inline HI_VOID hi_isp_ca_colorratioth_low_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_COLOR_RATIOTH0 ISP_CA_COLOR_RATIOTH0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_COLOR_RATIOTH0.u32 = pstIspReg->ISP_CA_COLOR_RATIOTH0.u32;
	ISP_CA_COLOR_RATIOTH0.bits.colorratioth_low = data;
	pstIspReg->ISP_CA_COLOR_RATIOTH0.u32 = ISP_CA_COLOR_RATIOTH0.u32;
}

static __inline HI_VOID hi_isp_ca_colorratioth_mid_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_COLOR_RATIOTH0 ISP_CA_COLOR_RATIOTH0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_COLOR_RATIOTH0.u32 = pstIspReg->ISP_CA_COLOR_RATIOTH0.u32;
	ISP_CA_COLOR_RATIOTH0.bits.colorratioth_mid = data;
	pstIspReg->ISP_CA_COLOR_RATIOTH0.u32 = ISP_CA_COLOR_RATIOTH0.u32;
}

static __inline HI_VOID hi_isp_ca_colorratioth_high_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_COLOR_RATIOTH1 ISP_CA_COLOR_RATIOTH1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_COLOR_RATIOTH1.u32 = pstIspReg->ISP_CA_COLOR_RATIOTH1.u32;
	ISP_CA_COLOR_RATIOTH1.bits.colorratioth_high = data;
	pstIspReg->ISP_CA_COLOR_RATIOTH1.u32 = ISP_CA_COLOR_RATIOTH1.u32;
}

static __inline HI_VOID hi_isp_ca_width_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_SIZE ISP_CA_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_SIZE.u32 = pstIspReg->ISP_CA_SIZE.u32;
	ISP_CA_SIZE.bits.width = data;
	pstIspReg->ISP_CA_SIZE.u32 = ISP_CA_SIZE.u32;
}

static __inline HI_VOID hi_isp_ca_height_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_SIZE ISP_CA_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CA_SIZE.u32 = pstIspReg->ISP_CA_SIZE.u32;
	ISP_CA_SIZE.bits.height = data;
	pstIspReg->ISP_CA_SIZE.u32 = ISP_CA_SIZE.u32;
}

/*****************************************************
MODULE        -----GE-----
MODULE        -----GE-----
MODULE        -----GE-----
*****************************************************/

#define HI_ISP_GE_ENABLE_DEFAULT (1)
static __inline HI_VOID hi_isp_ge_enable_write(HI_U8 ispDev, HI_U8 chn, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GE_CFG ISP_GE_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GE_CFG.u32 = pstIspReg->ISP_GE_CFG.u32;

	switch (chn)
	{
		case 0:
	ISP_GE_CFG.bits.ge0_en = data;
	pstIspReg->ISP_GE_CFG.u32 = ISP_GE_CFG.u32;
			break;
		case 1:
	ISP_GE_CFG.bits.ge1_en = data;
	pstIspReg->ISP_GE_CFG.u32 = ISP_GE_CFG.u32;
			break;
		case 2:
	ISP_GE_CFG.bits.ge2_en = data;
	pstIspReg->ISP_GE_CFG.u32 = ISP_GE_CFG.u32;
			break;
		case 3:
	ISP_GE_CFG.bits.ge3_en = data;
	pstIspReg->ISP_GE_CFG.u32 = ISP_GE_CFG.u32;
			break;
		default:
			break;			
	}	
}

static __inline HI_VOID hi_isp_ge_aver_enable_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GE_CFG ISP_GE_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GE_CFG.u32 = pstIspReg->ISP_GE_CFG.u32;
	ISP_GE_CFG.bits.ge_aver_en = data;
	pstIspReg->ISP_GE_CFG.u32 = ISP_GE_CFG.u32;
}

static __inline HI_VOID hi_isp_ge_correct_enable_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GE_CFG ISP_GE_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GE_CFG.u32 = pstIspReg->ISP_GE_CFG.u32;
	ISP_GE_CFG.bits.ge_cor_en = data;
	pstIspReg->ISP_GE_CFG.u32 = ISP_GE_CFG.u32;
}

/*****************************************************
ISP_GE_CT_TH1:         GE
0x205a3910:         31:14      reserved
				     13:00      ct_th1   
*****************************************************/
#define HI_ISP_GE_NPOFFSET_DEFAULT (1024)
static __inline HI_VOID hi_isp_ge_ct_th1_write(HI_U8 ispDev, HI_U8 chn, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GE0_CT_TH1 ISP_GE0_CT_TH1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GE0_CT_TH1.u32 = pstIspReg->ISP_GE0_CT_TH1.u32;

	switch (chn)
	{
		case 0:
	ISP_GE0_CT_TH1.bits.ge0_ct_th1 = data;
	pstIspReg->ISP_GE0_CT_TH1.u32 = ISP_GE0_CT_TH1.u32;
			break;
		case 1:
			pstIspReg->ISP_GE1_CT_TH1.bits.ge1_ct_th1 = data;
			break;
		case 2:
			pstIspReg->ISP_GE2_CT_TH1.bits.ge2_ct_th1 = data;
			break;
		case 3:
			pstIspReg->ISP_GE3_CT_TH1.bits.ge3_ct_th1 = data;
			break;
		default:
			break;			
	}
}

/*****************************************************
ISP_GE_CT_TH2:         GE
0x205a3914:         31:14      reserved
				     13:00      ct_th2   
*****************************************************/
#define HI_ISP_GE_SENSITHR_DEFAULT (300)
static __inline HI_VOID hi_isp_ge_ct_th2_write(HI_U8 ispDev, HI_U8 chn, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GE0_CT_TH2 ISP_GE0_CT_TH2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GE0_CT_TH2.u32 = pstIspReg->ISP_GE0_CT_TH2.u32;

	switch (chn)
	{
		case 0:
	ISP_GE0_CT_TH2.bits.ge0_ct_th2 = data;
	pstIspReg->ISP_GE0_CT_TH2.u32 = ISP_GE0_CT_TH2.u32;
			break;
		case 1:
			pstIspReg->ISP_GE1_CT_TH2.bits.ge1_ct_th2 = data;
			break;
		case 2:
			pstIspReg->ISP_GE2_CT_TH2.bits.ge2_ct_th2 = data;
			break;
		case 3:
			pstIspReg->ISP_GE3_CT_TH2.bits.ge3_ct_th2 = data;
			break;
		default:
			break;			
	}
}

/*****************************************************
ISP_GE_CT_TH3:         GE
0x205a3918:         31:14      reserved
				     13:00      ct_th3   
*****************************************************/
#define HI_ISP_GE_THRESHOLD_DEFAULT   (300)
static __inline HI_VOID hi_isp_ge_ct_th3_write(HI_U8 ispDev, HI_U8 chn, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GE0_CT_TH3 ISP_GE0_CT_TH3;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GE0_CT_TH3.u32 = pstIspReg->ISP_GE0_CT_TH3.u32;

	switch (chn)
	{
		case 0:
	ISP_GE0_CT_TH3.bits.ge0_ct_th3 = data;
	pstIspReg->ISP_GE0_CT_TH3.u32 = ISP_GE0_CT_TH3.u32;
			break;
		case 1:
			pstIspReg->ISP_GE1_CT_TH3.bits.ge1_ct_th3 = data;
			break;
		case 2:
			pstIspReg->ISP_GE2_CT_TH3.bits.ge2_ct_th3 = data;
			break;
		case 3:
			pstIspReg->ISP_GE3_CT_TH3.bits.ge3_ct_th3 = data;
			break;
		default:
			break;			
	}
}

/*****************************************************
ISP_GE_CT_SLOPE1:         GE
0x205a391c:         31:04      reserved
				    07:04      ct_slope1   0:1,1:2,2:4,3:8
				    03:00      ct_slope1   0:1,1:2,2:4,3:8
*****************************************************/
#define HI_ISP_GE_SLOPE_DEFAULT   (9)
#define HI_ISP_GE_SENSISLOPE_DEFAULT   (9)
static __inline HI_VOID hi_isp_ge_ct_slope_write(HI_U8 ispDev, HI_U8 chn, HI_U8 data1, HI_U8 data2) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GE0_CT_SLOPE ISP_GE0_CT_SLOPE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GE0_CT_SLOPE.u32 = pstIspReg->ISP_GE0_CT_SLOPE.u32;

	switch (chn)
	{
		case 0:
	ISP_GE0_CT_SLOPE.bits.ge0_ct_slope1 = data1;
	pstIspReg->ISP_GE0_CT_SLOPE.u32 = ISP_GE0_CT_SLOPE.u32;
	ISP_GE0_CT_SLOPE.bits.ge0_ct_slope2 = data2;
	pstIspReg->ISP_GE0_CT_SLOPE.u32 = ISP_GE0_CT_SLOPE.u32;
			break;
		case 1:
			pstIspReg->ISP_GE1_CT_SLOPE.bits.ge1_ct_slope1 = data1;
			pstIspReg->ISP_GE1_CT_SLOPE.bits.ge1_ct_slope2 = data2;
			break;
		case 2:
			pstIspReg->ISP_GE2_CT_SLOPE.bits.ge2_ct_slope1 = data1;
			pstIspReg->ISP_GE2_CT_SLOPE.bits.ge2_ct_slope2 = data2;
			break;
		case 3:
			pstIspReg->ISP_GE3_CT_SLOPE.bits.ge3_ct_slope1 = data1;
			pstIspReg->ISP_GE3_CT_SLOPE.bits.ge3_ct_slope2 = data2;
			break;
		default:
			break;			
	}
}

/*****************************************************
ISP_GE_ZONE:         GE
0x205a3950:         31:13      reserved
				     12:08      vnum     vetical
				     07:05      reserved
				     04:00      hnum     
*****************************************************/
static __inline HI_VOID hi_isp_ge_hnum_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GE_ZONE ISP_GE_ZONE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GE_ZONE.u32 = pstIspReg->ISP_GE_ZONE.u32;
	ISP_GE_ZONE.bits.hnum = data;
	pstIspReg->ISP_GE_ZONE.u32 = ISP_GE_ZONE.u32;
}

static __inline HI_VOID hi_isp_ge_vnum_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GE_ZONE ISP_GE_ZONE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GE_ZONE.u32 = pstIspReg->ISP_GE_ZONE.u32;
	ISP_GE_ZONE.bits.vnum = data;
	pstIspReg->ISP_GE_ZONE.u32 = ISP_GE_ZONE.u32;
}

/*****************************************************
ISP_GE_ZONE:         GE
0x205a3954:         31:3      reserved
				     2      gr_gb_en   gr gb enable
				     1      gb_en      gb enable
				     0      gr_en     gr enable
*****************************************************/
#define HI_ISP_GE_MODE_DEFAULT (4)
static __inline HI_VOID hi_isp_ge_mode_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GE_MODE ISP_GE_MODE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GE_MODE.u32 = pstIspReg->ISP_GE_MODE.u32;
	ISP_GE_MODE.bits.ge_mode = data;
	pstIspReg->ISP_GE_MODE.u32 = ISP_GE_MODE.u32;
}

/*****************************************************
ISP_GE_ZONE:         GE
0x205a3958:         31:9      reserved
				     8:0      ge_strength   ge
*****************************************************/
#define HI_ISP_GE_STRENGTH_DEFAULT (128)
static __inline HI_VOID hi_isp_ge_strength_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GE_STRENGTH ISP_GE_STRENGTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GE_STRENGTH.u32 = pstIspReg->ISP_GE_STRENGTH.u32;
	ISP_GE_STRENGTH.bits.strength = data;
	pstIspReg->ISP_GE_STRENGTH.u32 = ISP_GE_STRENGTH.u32;
}

/*****************************************************
ISP_GE_MEM_AVER_RADDR:         GE
0x205a3988:         31:00      aver_raddr
*****************************************************/
static __inline HI_VOID hi_isp_ge_aver_raddr_write(HI_U8 ispDev, HI_U8 chn, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GE_MEM_AVER_RADDR0 ISP_GE_MEM_AVER_RADDR0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GE_MEM_AVER_RADDR0.u32 = pstIspReg->ISP_GE_MEM_AVER_RADDR0.u32;

	switch (chn)
	{
		case 0:
	ISP_GE_MEM_AVER_RADDR0.bits.aver0_raddr = data;
	pstIspReg->ISP_GE_MEM_AVER_RADDR0.u32 = ISP_GE_MEM_AVER_RADDR0.u32;
			break;
		case 1:
			pstIspReg->ISP_GE_MEM_AVER_RADDR1.bits.aver1_raddr = data;
			break;
//		case 2:
//			pstIspReg->ISP_GE_MEM_AVER_RADDR2.bits.aver_raddr = data;
//			break;
//		case 3:
//			pstIspReg->ISP_GE_MEM_AVER_RADDR3.bits.aver_raddr = data;
//			break;
		default:
			break;			
	}
}

/*****************************************************
ISP_GE_MEM_AVER_RDATA:         GE
0x205a398c:         31:00      aver_rdata
*****************************************************/
static __inline HI_U32 hi_isp_ge_aver_rdata_read(HI_U8 ispDev, HI_U8 chn) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GE_MEM_AVER_RDATA0 ISP_GE_MEM_AVER_RDATA0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GE_MEM_AVER_RDATA0.u32 = pstIspReg->ISP_GE_MEM_AVER_RDATA0.u32;

	switch (chn)
	{
		case 0:
			return ISP_GE_MEM_AVER_RDATA0.bits.aver0_rdata;
		case 1:
			return pstIspReg->ISP_GE_MEM_AVER_RDATA1.bits.aver1_rdata;
		//case 2:
			//return pstIspReg->ISP_GE_MEM_AVER_RDATA2.bits.aver_rdata;
		//case 3:
			//return pstIspReg->ISP_GE_MEM_AVER_RDATA3.bits.aver_rdata;
		default:
			return 0;			
	}
}

/*****************************************************
ISP_GE_SIZE:      
0x205c39f0:                     31:29    reserved
                                          28:16   height
                                          15:13   reserved
                                          12:  0   width
*****************************************************/
static __inline HI_VOID hi_isp_ge_height_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GE_SIZE ISP_GE_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GE_SIZE.u32 = pstIspReg->ISP_GE_SIZE.u32;
	ISP_GE_SIZE.bits.height = data;
	pstIspReg->ISP_GE_SIZE.u32 = ISP_GE_SIZE.u32;
}

static __inline HI_VOID hi_isp_ge_width_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_GE_SIZE ISP_GE_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_GE_SIZE.u32 = pstIspReg->ISP_GE_SIZE.u32;
	ISP_GE_SIZE.bits.width = data;
	pstIspReg->ISP_GE_SIZE.u32 = ISP_GE_SIZE.u32;
}



/*****************************************************
MODULE        -----SHARPEN-----
MODULE        -----SHARPEN-----
MODULE        -----SHARPEN-----
*****************************************************/
/*****************************************************
ISP_SHARPEN_CFG:              
0x205c5200:                     31:1      reserved
                                          0      sharpenenable
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_cfg_enable_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_CFG ISP_SHARPEN_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_CFG.u32 = pstIspReg->ISP_SHARPEN_CFG.u32;
	ISP_SHARPEN_CFG.bits.en = data;
	pstIspReg->ISP_SHARPEN_CFG.u32 = ISP_SHARPEN_CFG.u32;
}

static __inline HI_U8 hi_isp_sharpen_cfg_enable_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_CFG ISP_SHARPEN_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_CFG.u32 = pstIspReg->ISP_SHARPEN_CFG.u32;
	return ISP_SHARPEN_CFG.bits.en;
}

/*****************************************************
ISP_SHARPEN_AMT:         SHARPEN
0x205a4210:                     31:28      reserved
                                          27:16      edge_amt
                                          15:12      reserved
                                          11:  0      sharp_amt
*****************************************************/
static __inline HI_VOID hi_isp_edge_amt_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_AMT ISP_SHARPEN_AMT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_AMT.u32 = pstIspReg->ISP_SHARPEN_AMT.u32;
	ISP_SHARPEN_AMT.bits.edge_amt = data;
	pstIspReg->ISP_SHARPEN_AMT.u32 = ISP_SHARPEN_AMT.u32;
}

static __inline HI_U16 hi_isp_edge_amt_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_AMT ISP_SHARPEN_AMT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_AMT.u32 = pstIspReg->ISP_SHARPEN_AMT.u32;
	return ISP_SHARPEN_AMT.bits.edge_amt;
}

static __inline HI_VOID hi_isp_sharp_amt_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_AMT ISP_SHARPEN_AMT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_AMT.u32 = pstIspReg->ISP_SHARPEN_AMT.u32;
	ISP_SHARPEN_AMT.bits.sharp_amt = data;
	pstIspReg->ISP_SHARPEN_AMT.u32 = ISP_SHARPEN_AMT.u32;
}

static __inline HI_U16 hi_isp_sharp_amt_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_AMT ISP_SHARPEN_AMT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_AMT.u32 = pstIspReg->ISP_SHARPEN_AMT.u32;
	return ISP_SHARPEN_AMT.bits.sharp_amt;
}

/*****************************************************
ISP_SHARPEN_THD1:         SHARPEN
0x205a4214:                     
                                          25:16     edge_thd1
                                          9:  0     sharp_thd1
*****************************************************/
static __inline HI_VOID hi_isp_edge_thd1_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_THD1 ISP_SHARPEN_THD1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_THD1.u32 = pstIspReg->ISP_SHARPEN_THD1.u32;
	ISP_SHARPEN_THD1.bits.edge_thd1 = data;
	pstIspReg->ISP_SHARPEN_THD1.u32 = ISP_SHARPEN_THD1.u32;
}

static __inline HI_U16 hi_isp_edge_thd1_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_THD1 ISP_SHARPEN_THD1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_THD1.u32 = pstIspReg->ISP_SHARPEN_THD1.u32;
	return ISP_SHARPEN_THD1.bits.edge_thd1;
}

static __inline HI_VOID hi_isp_sharp_thd1_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_THD1 ISP_SHARPEN_THD1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_THD1.u32 = pstIspReg->ISP_SHARPEN_THD1.u32;
	ISP_SHARPEN_THD1.bits.sharp_thd1 = data;
	pstIspReg->ISP_SHARPEN_THD1.u32 = ISP_SHARPEN_THD1.u32;
}

static __inline HI_U16 hi_isp_sharp_thd1_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_THD1 ISP_SHARPEN_THD1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_THD1.u32 = pstIspReg->ISP_SHARPEN_THD1.u32;
	return ISP_SHARPEN_THD1.bits.sharp_thd1;
}

/*****************************************************
ISP_SHARPEN_THD2:         SHARPEN
0x113a5218:                     
                                          25:16     edge_thd2
                                          9:  0     sharp_thd2
*****************************************************/
static __inline HI_VOID hi_isp_edge_thd2_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_THD2 ISP_SHARPEN_THD2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_THD2.u32 = pstIspReg->ISP_SHARPEN_THD2.u32;
	ISP_SHARPEN_THD2.bits.edge_thd2 = data;
	pstIspReg->ISP_SHARPEN_THD2.u32 = ISP_SHARPEN_THD2.u32;
}

static __inline HI_U16 hi_isp_edge_thd2_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_THD2 ISP_SHARPEN_THD2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_THD2.u32 = pstIspReg->ISP_SHARPEN_THD2.u32;
	return ISP_SHARPEN_THD2.bits.edge_thd2;
}

static __inline HI_VOID hi_isp_sharp_thd2_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_THD2 ISP_SHARPEN_THD2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_THD2.u32 = pstIspReg->ISP_SHARPEN_THD2.u32;
	ISP_SHARPEN_THD2.bits.sharp_thd2 = data;
	pstIspReg->ISP_SHARPEN_THD2.u32 = ISP_SHARPEN_THD2.u32;
}

static __inline HI_U16 hi_isp_sharp_thd2_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_THD2 ISP_SHARPEN_THD2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_THD2.u32 = pstIspReg->ISP_SHARPEN_THD2.u32;
	return ISP_SHARPEN_THD2.bits.sharp_thd2;
}

/*****************************************************
ISP_SHARPEN_COEF:         SHARPEN
0x113a521c:               31:28     reserved
                          27:16     edge_coef
                          15:12     reserved
                          11:00     sharp_coef
*****************************************************/
static __inline HI_VOID hi_isp_edge_coef_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_COEF ISP_SHARPEN_COEF;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_COEF.u32 = pstIspReg->ISP_SHARPEN_COEF.u32;
	ISP_SHARPEN_COEF.bits.edge_coef = data;
	pstIspReg->ISP_SHARPEN_COEF.u32 = ISP_SHARPEN_COEF.u32;
}

static __inline HI_U16 hi_isp_edge_coef_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_COEF ISP_SHARPEN_COEF;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_COEF.u32 = pstIspReg->ISP_SHARPEN_COEF.u32;
	return ISP_SHARPEN_COEF.bits.edge_coef;
}

static __inline HI_VOID hi_isp_sharp_coef_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_COEF ISP_SHARPEN_COEF;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_COEF.u32 = pstIspReg->ISP_SHARPEN_COEF.u32;
	ISP_SHARPEN_COEF.bits.sharp_coef = data;
	pstIspReg->ISP_SHARPEN_COEF.u32 = ISP_SHARPEN_COEF.u32;
}

static __inline HI_U16 hi_isp_sharp_coef_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_COEF ISP_SHARPEN_COEF;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_COEF.u32 = pstIspReg->ISP_SHARPEN_COEF.u32;
	return ISP_SHARPEN_COEF.bits.sharp_coef;
}

/*****************************************************
ISP_SHARPEN_SHOOTAMT:      SHARPENshoot      
0x113a5220:                     31:24   reserved
                                          23:16   over_amt
                                          15:8     reserved
                                          7:  0     under_amt
*****************************************************/
static __inline HI_VOID hi_isp_over_amt_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SHOOTAMT ISP_SHARPEN_SHOOTAMT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SHOOTAMT.u32 = pstIspReg->ISP_SHARPEN_SHOOTAMT.u32;
	ISP_SHARPEN_SHOOTAMT.bits.over_amt = data;
	pstIspReg->ISP_SHARPEN_SHOOTAMT.u32 = ISP_SHARPEN_SHOOTAMT.u32;
}

static __inline HI_U8 hi_isp_over_amt_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SHOOTAMT ISP_SHARPEN_SHOOTAMT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SHOOTAMT.u32 = pstIspReg->ISP_SHARPEN_SHOOTAMT.u32;
	return ISP_SHARPEN_SHOOTAMT.bits.over_amt;
}

static __inline HI_VOID hi_isp_under_amt_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SHOOTAMT ISP_SHARPEN_SHOOTAMT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SHOOTAMT.u32 = pstIspReg->ISP_SHARPEN_SHOOTAMT.u32;
	ISP_SHARPEN_SHOOTAMT.bits.under_amt = data;
	pstIspReg->ISP_SHARPEN_SHOOTAMT.u32 = ISP_SHARPEN_SHOOTAMT.u32;
}

static __inline HI_U8 hi_isp_under_amt_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SHOOTAMT ISP_SHARPEN_SHOOTAMT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SHOOTAMT.u32 = pstIspReg->ISP_SHARPEN_SHOOTAMT.u32;
	return ISP_SHARPEN_SHOOTAMT.bits.under_amt;
}

/*****************************************************
ISP_SHARPEN_JAGTHD:     
0x113c5224:                     31:26    reserved
                                          25:16   varjagthd2
                                      15:10   reserved
                                          9:  0   varjagthd1
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_varjagthd2_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_JAGTHD ISP_SHARPEN_JAGTHD;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_JAGTHD.u32 = pstIspReg->ISP_SHARPEN_JAGTHD.u32;
	ISP_SHARPEN_JAGTHD.bits.varjagthd2 = data;
	pstIspReg->ISP_SHARPEN_JAGTHD.u32 = ISP_SHARPEN_JAGTHD.u32;
}

static __inline HI_U16 hi_isp_sharpen_varjagthd2_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_JAGTHD ISP_SHARPEN_JAGTHD;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_JAGTHD.u32 = pstIspReg->ISP_SHARPEN_JAGTHD.u32;
	return ISP_SHARPEN_JAGTHD.bits.varjagthd2;
}

static __inline HI_VOID hi_isp_sharpen_varjagthd1_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_JAGTHD ISP_SHARPEN_JAGTHD;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_JAGTHD.u32 = pstIspReg->ISP_SHARPEN_JAGTHD.u32;
	ISP_SHARPEN_JAGTHD.bits.varjagthd1 = data;
	pstIspReg->ISP_SHARPEN_JAGTHD.u32 = ISP_SHARPEN_JAGTHD.u32;
}

static __inline HI_U16 hi_isp_sharpen_varjagthd1_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_JAGTHD ISP_SHARPEN_JAGTHD;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_JAGTHD.u32 = pstIspReg->ISP_SHARPEN_JAGTHD.u32;
	return ISP_SHARPEN_JAGTHD.bits.varjagthd1;
}

/*****************************************************
ISP_SHARPEN_EDGEJAG:   
0x113c5228:                     31:29    reserved
                                          28:16   edgejagmulcoef
                                      15:12   reserved
                                          11:  0   edgejagamt
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_edgejagmulcoef_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_EDGEJAG ISP_SHARPEN_EDGEJAG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_EDGEJAG.u32 = pstIspReg->ISP_SHARPEN_EDGEJAG.u32;
	ISP_SHARPEN_EDGEJAG.bits.edgejagmulcoef = data;
	pstIspReg->ISP_SHARPEN_EDGEJAG.u32 = ISP_SHARPEN_EDGEJAG.u32;
}

static __inline HI_U16 hi_isp_sharpen_edgejagmulcoef_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_EDGEJAG ISP_SHARPEN_EDGEJAG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_EDGEJAG.u32 = pstIspReg->ISP_SHARPEN_EDGEJAG.u32;
	return ISP_SHARPEN_EDGEJAG.bits.edgejagmulcoef;
}

static __inline HI_VOID hi_isp_sharpen_edgejagamt_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_EDGEJAG ISP_SHARPEN_EDGEJAG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_EDGEJAG.u32 = pstIspReg->ISP_SHARPEN_EDGEJAG.u32;
	ISP_SHARPEN_EDGEJAG.bits.edgejagamt = data;
	pstIspReg->ISP_SHARPEN_EDGEJAG.u32 = ISP_SHARPEN_EDGEJAG.u32;
}

static __inline HI_U16 hi_isp_sharpen_edgejagamt_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_EDGEJAG ISP_SHARPEN_EDGEJAG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_EDGEJAG.u32 = pstIspReg->ISP_SHARPEN_EDGEJAG.u32;
	return ISP_SHARPEN_EDGEJAG.bits.edgejagamt;
}

/*****************************************************
ISP_SHARPEN_OSHTJAG:   
0x113c522c:                     31:29    reserved
                                          28:16   oshtjagmulcoef
                                      15:12   reserved
                                          7:  0   oshtjagamt
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_oshtjagmulcoef_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_OSHTJAG ISP_SHARPEN_OSHTJAG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_OSHTJAG.u32 = pstIspReg->ISP_SHARPEN_OSHTJAG.u32;
	ISP_SHARPEN_OSHTJAG.bits.oshtjagmulcoef = data;
	pstIspReg->ISP_SHARPEN_OSHTJAG.u32 = ISP_SHARPEN_OSHTJAG.u32;
}

static __inline HI_U16 hi_isp_sharpen_oshtjagmulcoef_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_OSHTJAG ISP_SHARPEN_OSHTJAG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_OSHTJAG.u32 = pstIspReg->ISP_SHARPEN_OSHTJAG.u32;
	return ISP_SHARPEN_OSHTJAG.bits.oshtjagmulcoef;
}

static __inline HI_VOID hi_isp_sharpen_oshtjagamt_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_OSHTJAG ISP_SHARPEN_OSHTJAG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_OSHTJAG.u32 = pstIspReg->ISP_SHARPEN_OSHTJAG.u32;
	ISP_SHARPEN_OSHTJAG.bits.oshtjagamt = data;
	pstIspReg->ISP_SHARPEN_OSHTJAG.u32 = ISP_SHARPEN_OSHTJAG.u32;
}

static __inline HI_U8 hi_isp_sharpen_oshtjagamt_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_OSHTJAG ISP_SHARPEN_OSHTJAG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_OSHTJAG.u32 = pstIspReg->ISP_SHARPEN_OSHTJAG.u32;
	return ISP_SHARPEN_OSHTJAG.bits.oshtjagamt;
}

/*****************************************************
ISP_SHARPEN_USHTJAG:     
0x113c5230:                     31:29    reserved
                                          28:16   ushtjagmulcoef
                                      15:12   reserved
                                          7:  0   ushtjagamt
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_ushtjagmulcoef_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_USHTJAG ISP_SHARPEN_USHTJAG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_USHTJAG.u32 = pstIspReg->ISP_SHARPEN_USHTJAG.u32;
	ISP_SHARPEN_USHTJAG.bits.ushtjagmulcoef = data;
	pstIspReg->ISP_SHARPEN_USHTJAG.u32 = ISP_SHARPEN_USHTJAG.u32;
}

static __inline HI_U16 hi_isp_sharpen_ushtjagmulcoef_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_USHTJAG ISP_SHARPEN_USHTJAG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_USHTJAG.u32 = pstIspReg->ISP_SHARPEN_USHTJAG.u32;
	return ISP_SHARPEN_USHTJAG.bits.ushtjagmulcoef;
}

static __inline HI_VOID hi_isp_sharpen_ushtjagamt_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_USHTJAG ISP_SHARPEN_USHTJAG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_USHTJAG.u32 = pstIspReg->ISP_SHARPEN_USHTJAG.u32;
	ISP_SHARPEN_USHTJAG.bits.ushtjagamt = data;
	pstIspReg->ISP_SHARPEN_USHTJAG.u32 = ISP_SHARPEN_USHTJAG.u32;
}

static __inline HI_U8 hi_isp_sharpen_ushtjagamt_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_USHTJAG ISP_SHARPEN_USHTJAG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_USHTJAG.u32 = pstIspReg->ISP_SHARPEN_USHTJAG.u32;
	return ISP_SHARPEN_USHTJAG.bits.ushtjagamt;
}



/*****************************************************
ISP_SHARPEN_MID0:      SHARPEN
0x113c5234:                     31:24   reserved
                                          23:16   mid_tmp02
                                          15:8     mid_tmp01
                                          7:  0     mid_tmp00
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_mid_tmp02_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_MID0 ISP_SHARPEN_MID0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_MID0.u32 = pstIspReg->ISP_SHARPEN_MID0.u32;
	ISP_SHARPEN_MID0.bits.mid_tmp02 = data;
	pstIspReg->ISP_SHARPEN_MID0.u32 = ISP_SHARPEN_MID0.u32;
}

static __inline HI_U8 hi_isp_sharpen_mid_tmp02_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_MID0 ISP_SHARPEN_MID0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_MID0.u32 = pstIspReg->ISP_SHARPEN_MID0.u32;
	return ISP_SHARPEN_MID0.bits.mid_tmp02;
}

static __inline HI_VOID hi_isp_sharpen_mid_tmp01_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_MID0 ISP_SHARPEN_MID0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_MID0.u32 = pstIspReg->ISP_SHARPEN_MID0.u32;
	ISP_SHARPEN_MID0.bits.mid_tmp01 = data;
	pstIspReg->ISP_SHARPEN_MID0.u32 = ISP_SHARPEN_MID0.u32;
}

static __inline HI_U8 hi_isp_sharpen_mid_tmp01_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_MID0 ISP_SHARPEN_MID0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_MID0.u32 = pstIspReg->ISP_SHARPEN_MID0.u32;
	return ISP_SHARPEN_MID0.bits.mid_tmp01;
}

static __inline HI_VOID hi_isp_sharpen_mid_tmp00_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_MID0 ISP_SHARPEN_MID0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_MID0.u32 = pstIspReg->ISP_SHARPEN_MID0.u32;
	ISP_SHARPEN_MID0.bits.mid_tmp00 = data;
	pstIspReg->ISP_SHARPEN_MID0.u32 = ISP_SHARPEN_MID0.u32;
}

static __inline HI_U8 hi_isp_sharpen_mid_tmp00_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_MID0 ISP_SHARPEN_MID0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_MID0.u32 = pstIspReg->ISP_SHARPEN_MID0.u32;
	return ISP_SHARPEN_MID0.bits.mid_tmp00;
}

/*****************************************************
ISP_SHARPEN_MID1:      SHARPEN
0x113c5238:                     31:24   reserved
                                          23:16   mid_tmp12
                                          15:8     mid_tmp11
                                          7:  0     mid_tmp10
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_mid_tmp12_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_MID1 ISP_SHARPEN_MID1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_MID1.u32 = pstIspReg->ISP_SHARPEN_MID1.u32;
	ISP_SHARPEN_MID1.bits.mid_tmp12 = data;
	pstIspReg->ISP_SHARPEN_MID1.u32 = ISP_SHARPEN_MID1.u32;
}

static __inline HI_U8 hi_isp_sharpen_mid_tmp12_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_MID1 ISP_SHARPEN_MID1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_MID1.u32 = pstIspReg->ISP_SHARPEN_MID1.u32;
	return ISP_SHARPEN_MID1.bits.mid_tmp12;
}

static __inline HI_VOID hi_isp_sharpen_mid_tmp11_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_MID1 ISP_SHARPEN_MID1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_MID1.u32 = pstIspReg->ISP_SHARPEN_MID1.u32;
	ISP_SHARPEN_MID1.bits.mid_tmp11 = data;
	pstIspReg->ISP_SHARPEN_MID1.u32 = ISP_SHARPEN_MID1.u32;
}

static __inline HI_U8 hi_isp_sharpen_mid_tmp11_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_MID1 ISP_SHARPEN_MID1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_MID1.u32 = pstIspReg->ISP_SHARPEN_MID1.u32;
	return ISP_SHARPEN_MID1.bits.mid_tmp11;
}

static __inline HI_VOID hi_isp_sharpen_mid_tmp10_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_MID1 ISP_SHARPEN_MID1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_MID1.u32 = pstIspReg->ISP_SHARPEN_MID1.u32;
	ISP_SHARPEN_MID1.bits.mid_tmp10 = data;
	pstIspReg->ISP_SHARPEN_MID1.u32 = ISP_SHARPEN_MID1.u32;
}

static __inline HI_U8 hi_isp_sharpen_mid_tmp10_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_MID1 ISP_SHARPEN_MID1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_MID1.u32 = pstIspReg->ISP_SHARPEN_MID1.u32;
	return ISP_SHARPEN_MID1.bits.mid_tmp10;
}

/*****************************************************
ISP_SHARPEN_MID2:      SHARPEN
0x113c523c:                     31:24   reserved
                                          23:16   mid_tmp22
                                          15:8     mid_tmp21
                                          7:  0     mid_tmp20
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_mid_tmp22_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_MID2 ISP_SHARPEN_MID2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_MID2.u32 = pstIspReg->ISP_SHARPEN_MID2.u32;
	ISP_SHARPEN_MID2.bits.mid_tmp22 = data;
	pstIspReg->ISP_SHARPEN_MID2.u32 = ISP_SHARPEN_MID2.u32;
}

static __inline HI_U8 hi_isp_sharpen_mid_tmp22_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_MID2 ISP_SHARPEN_MID2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_MID2.u32 = pstIspReg->ISP_SHARPEN_MID2.u32;
	return ISP_SHARPEN_MID2.bits.mid_tmp22;
}

static __inline HI_VOID hi_isp_sharpen_mid_tmp21_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_MID2 ISP_SHARPEN_MID2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_MID2.u32 = pstIspReg->ISP_SHARPEN_MID2.u32;
	ISP_SHARPEN_MID2.bits.mid_tmp21 = data;
	pstIspReg->ISP_SHARPEN_MID2.u32 = ISP_SHARPEN_MID2.u32;
}

static __inline HI_U8 hi_isp_sharpen_mid_tmp21_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_MID2 ISP_SHARPEN_MID2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_MID2.u32 = pstIspReg->ISP_SHARPEN_MID2.u32;
	return ISP_SHARPEN_MID2.bits.mid_tmp21;
}

static __inline HI_VOID hi_isp_sharpen_mid_tmp20_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_MID2 ISP_SHARPEN_MID2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_MID2.u32 = pstIspReg->ISP_SHARPEN_MID2.u32;
	ISP_SHARPEN_MID2.bits.mid_tmp20 = data;
	pstIspReg->ISP_SHARPEN_MID2.u32 = ISP_SHARPEN_MID2.u32;
}

static __inline HI_U8 hi_isp_sharpen_mid_tmp20_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_MID2 ISP_SHARPEN_MID2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_MID2.u32 = pstIspReg->ISP_SHARPEN_MID2.u32;
	return ISP_SHARPEN_MID2.bits.mid_tmp20;
}

/*****************************************************
ISP_SHARPEN_CTRL:   SHARPEN      
0x205a5244:         31:5   reserved
					12:8   pixsel	
					4		jagctrl
					3		lumactrlnoise
					2		shtvarctrl
					1		lumactrl
					0		skinctrl
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_pixsel_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_CTRL ISP_SHARPEN_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_CTRL.u32 = pstIspReg->ISP_SHARPEN_CTRL.u32;
	ISP_SHARPEN_CTRL.bits.pixsel = data;
	pstIspReg->ISP_SHARPEN_CTRL.u32 = ISP_SHARPEN_CTRL.u32;
}

static __inline HI_U8 hi_isp_sharpen_pixsel_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_CTRL ISP_SHARPEN_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_CTRL.u32 = pstIspReg->ISP_SHARPEN_CTRL.u32;
	return ISP_SHARPEN_CTRL.bits.pixsel;
}

static __inline HI_VOID hi_isp_sharpen_jagctrl_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_CTRL ISP_SHARPEN_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_CTRL.u32 = pstIspReg->ISP_SHARPEN_CTRL.u32;
	ISP_SHARPEN_CTRL.bits.jagctrl = data;
	pstIspReg->ISP_SHARPEN_CTRL.u32 = ISP_SHARPEN_CTRL.u32;
}

static __inline HI_U8 hi_isp_sharpen_jagctrl_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_CTRL ISP_SHARPEN_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_CTRL.u32 = pstIspReg->ISP_SHARPEN_CTRL.u32;
	return ISP_SHARPEN_CTRL.bits.jagctrl;
}


static __inline HI_VOID hi_isp_sharpen_lumactrlnoise_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_CTRL ISP_SHARPEN_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_CTRL.u32 = pstIspReg->ISP_SHARPEN_CTRL.u32;
	ISP_SHARPEN_CTRL.bits.lumactrlnoise = data;
	pstIspReg->ISP_SHARPEN_CTRL.u32 = ISP_SHARPEN_CTRL.u32;
}

static __inline HI_U8 hi_isp_sharpen_lumactrlnoise_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_CTRL ISP_SHARPEN_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_CTRL.u32 = pstIspReg->ISP_SHARPEN_CTRL.u32;
	return ISP_SHARPEN_CTRL.bits.lumactrlnoise;
}

static __inline HI_VOID hi_isp_sharpen_shtvarctrl_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_CTRL ISP_SHARPEN_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_CTRL.u32 = pstIspReg->ISP_SHARPEN_CTRL.u32;
	ISP_SHARPEN_CTRL.bits.shtvarctrl = data;
	pstIspReg->ISP_SHARPEN_CTRL.u32 = ISP_SHARPEN_CTRL.u32;
}

static __inline HI_U8 hi_isp_sharpen_shtvarctrl_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_CTRL ISP_SHARPEN_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_CTRL.u32 = pstIspReg->ISP_SHARPEN_CTRL.u32;
	return ISP_SHARPEN_CTRL.bits.shtvarctrl;
}

static __inline HI_VOID hi_isp_sharpen_lumactrl_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_CTRL ISP_SHARPEN_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_CTRL.u32 = pstIspReg->ISP_SHARPEN_CTRL.u32;
	ISP_SHARPEN_CTRL.bits.lumactrl = data;
	pstIspReg->ISP_SHARPEN_CTRL.u32 = ISP_SHARPEN_CTRL.u32;
}

static __inline HI_U8 hi_isp_sharpen_lumactrl_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_CTRL ISP_SHARPEN_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_CTRL.u32 = pstIspReg->ISP_SHARPEN_CTRL.u32;
	return ISP_SHARPEN_CTRL.bits.lumactrl;
}

static __inline HI_VOID hi_isp_sharpen_skinctrl_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_CTRL ISP_SHARPEN_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_CTRL.u32 = pstIspReg->ISP_SHARPEN_CTRL.u32;
	ISP_SHARPEN_CTRL.bits.skinctrl = data;
	pstIspReg->ISP_SHARPEN_CTRL.u32 = ISP_SHARPEN_CTRL.u32;
}

static __inline HI_U8 hi_isp_sharpen_skinctrl_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_CTRL ISP_SHARPEN_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_CTRL.u32 = pstIspReg->ISP_SHARPEN_CTRL.u32;
	return ISP_SHARPEN_CTRL.bits.skinctrl;
}

static __inline HI_VOID hi_isp_sharpen_shadsup_en_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_CTRL ISP_SHARPEN_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_CTRL.u32 = pstIspReg->ISP_SHARPEN_CTRL.u32;
	ISP_SHARPEN_CTRL.bits.shadsup_en = data;
	pstIspReg->ISP_SHARPEN_CTRL.u32 = ISP_SHARPEN_CTRL.u32;
}

static __inline HI_VOID hi_isp_sharpen_rgbshp_en_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_CTRL ISP_SHARPEN_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_CTRL.u32 = pstIspReg->ISP_SHARPEN_CTRL.u32;
	ISP_SHARPEN_CTRL.bits.rgbshp_en = data;
	pstIspReg->ISP_SHARPEN_CTRL.u32 = ISP_SHARPEN_CTRL.u32;
}

static __inline HI_VOID hi_isp_sharpen_lowbandesm_en_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_CTRL ISP_SHARPEN_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_CTRL.u32 = pstIspReg->ISP_SHARPEN_CTRL.u32;
	ISP_SHARPEN_CTRL.bits.lowbandesm_en = data;
	pstIspReg->ISP_SHARPEN_CTRL.u32 = ISP_SHARPEN_CTRL.u32;
}

/*****************************************************
ISP_SHARPEN_LUMATH:      LUMATH
0x113c5248:                         31:16 
                                          15:8     maxshift
                                          7:  0     minshift
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_maxshift_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SHIFT ISP_SHARPEN_SHIFT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SHIFT.u32 = pstIspReg->ISP_SHARPEN_SHIFT.u32;
	ISP_SHARPEN_SHIFT.bits.maxshift = data;
	pstIspReg->ISP_SHARPEN_SHIFT.u32 = ISP_SHARPEN_SHIFT.u32;
}

static __inline HI_U8 hi_isp_sharpen_maxshift_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SHIFT ISP_SHARPEN_SHIFT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SHIFT.u32 = pstIspReg->ISP_SHARPEN_SHIFT.u32;
	return ISP_SHARPEN_SHIFT.bits.maxshift;
}

static __inline HI_VOID hi_isp_sharpen_minshift_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SHIFT ISP_SHARPEN_SHIFT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SHIFT.u32 = pstIspReg->ISP_SHARPEN_SHIFT.u32;
	ISP_SHARPEN_SHIFT.bits.minshift = data;
	pstIspReg->ISP_SHARPEN_SHIFT.u32 = ISP_SHARPEN_SHIFT.u32;
}

static __inline HI_U8 hi_isp_sharpen_minshift_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SHIFT ISP_SHARPEN_SHIFT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SHIFT.u32 = pstIspReg->ISP_SHARPEN_SHIFT.u32;
	return ISP_SHARPEN_SHIFT.bits.minshift;
}

/*****************************************************
ISP_SHARPEN_LUMAWGT:      LUMAWgt
0x205a524c:               31:24 reserved
                          18:16	dirrlyshft
                          10:08	luma_nost
                          04:00	luma_limit
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_dirrlyshft_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_ST ISP_SHARPEN_ST;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_ST.u32 = pstIspReg->ISP_SHARPEN_ST.u32;
	ISP_SHARPEN_ST.bits.dirrlyshft = data;
	pstIspReg->ISP_SHARPEN_ST.u32 = ISP_SHARPEN_ST.u32;
}

static __inline HI_U8 hi_isp_sharpen_dirrlyshft_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_ST ISP_SHARPEN_ST;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_ST.u32 = pstIspReg->ISP_SHARPEN_ST.u32;
	return ISP_SHARPEN_ST.bits.dirrlyshft;
}

static __inline HI_VOID hi_isp_sharpen_luma_nost_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_ST ISP_SHARPEN_ST;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_ST.u32 = pstIspReg->ISP_SHARPEN_ST.u32;
	ISP_SHARPEN_ST.bits.luma_nost = data;
	pstIspReg->ISP_SHARPEN_ST.u32 = ISP_SHARPEN_ST.u32;
}

static __inline HI_U8 hi_isp_sharpen_luma_nost_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_ST ISP_SHARPEN_ST;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_ST.u32 = pstIspReg->ISP_SHARPEN_ST.u32;
	return ISP_SHARPEN_ST.bits.luma_nost;
}

static __inline HI_VOID hi_isp_sharpen_luma_limit_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_ST ISP_SHARPEN_ST;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_ST.u32 = pstIspReg->ISP_SHARPEN_ST.u32;
	ISP_SHARPEN_ST.bits.luma_limit = data;
	pstIspReg->ISP_SHARPEN_ST.u32 = ISP_SHARPEN_ST.u32;
}

static __inline HI_U8 hi_isp_sharpen_luma_limit_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_ST ISP_SHARPEN_ST;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_ST.u32 = pstIspReg->ISP_SHARPEN_ST.u32;
	return ISP_SHARPEN_ST.bits.luma_limit;
}

/*****************************************************
ISP_SHARPEN_OSHTVARTH:    
0x113c5250:                 31:24   reserverd
							25:16	overvarth1
							15:10   reserverd
							09:00	overvarth0
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_overvarth1_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_OSHTVARTH ISP_SHARPEN_OSHTVARTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_OSHTVARTH.u32 = pstIspReg->ISP_SHARPEN_OSHTVARTH.u32;
	ISP_SHARPEN_OSHTVARTH.bits.overvarth1 = data;
	pstIspReg->ISP_SHARPEN_OSHTVARTH.u32 = ISP_SHARPEN_OSHTVARTH.u32;
}

static __inline HI_U16 hi_isp_sharpen_overvarth1_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_OSHTVARTH ISP_SHARPEN_OSHTVARTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_OSHTVARTH.u32 = pstIspReg->ISP_SHARPEN_OSHTVARTH.u32;
	return ISP_SHARPEN_OSHTVARTH.bits.overvarth1;
}	

static __inline HI_VOID hi_isp_sharpen_overvarth0_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_OSHTVARTH ISP_SHARPEN_OSHTVARTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_OSHTVARTH.u32 = pstIspReg->ISP_SHARPEN_OSHTVARTH.u32;
	ISP_SHARPEN_OSHTVARTH.bits.overvarth0 = data;
	pstIspReg->ISP_SHARPEN_OSHTVARTH.u32 = ISP_SHARPEN_OSHTVARTH.u32;
}

static __inline HI_U16 hi_isp_sharpen_overvarth0_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_OSHTVARTH ISP_SHARPEN_OSHTVARTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_OSHTVARTH.u32 = pstIspReg->ISP_SHARPEN_OSHTVARTH.u32;
	return ISP_SHARPEN_OSHTVARTH.bits.overvarth0;
}

/*****************************************************
ISP_SHARPEN_OSHTVAR:  
0x113c5254:                     31:24   reserverd
                                          28:16		overvarmul
                                      15:10    reserverd
                                          7:  0		overvaramt
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_overvarmul_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_OSHTVAR ISP_SHARPEN_OSHTVAR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_OSHTVAR.u32 = pstIspReg->ISP_SHARPEN_OSHTVAR.u32;
	ISP_SHARPEN_OSHTVAR.bits.overvarmul = data;
	pstIspReg->ISP_SHARPEN_OSHTVAR.u32 = ISP_SHARPEN_OSHTVAR.u32;
}

static __inline HI_U16 hi_isp_sharpen_overvarmul_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_OSHTVAR ISP_SHARPEN_OSHTVAR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_OSHTVAR.u32 = pstIspReg->ISP_SHARPEN_OSHTVAR.u32;
	return ISP_SHARPEN_OSHTVAR.bits.overvarmul;
}

static __inline HI_VOID hi_isp_sharpen_overvaramt_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_OSHTVAR ISP_SHARPEN_OSHTVAR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_OSHTVAR.u32 = pstIspReg->ISP_SHARPEN_OSHTVAR.u32;
	ISP_SHARPEN_OSHTVAR.bits.overvaramt = data;
	pstIspReg->ISP_SHARPEN_OSHTVAR.u32 = ISP_SHARPEN_OSHTVAR.u32;
}

static __inline HI_U8 hi_isp_sharpen_overvaramt_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_OSHTVAR ISP_SHARPEN_OSHTVAR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_OSHTVAR.u32 = pstIspReg->ISP_SHARPEN_OSHTVAR.u32;
	return ISP_SHARPEN_OSHTVAR.bits.overvaramt;
}

/*****************************************************
ISP_SHARPEN_USHTVARTH:   
0x113c5258:                 31:24   reserverd
							25:16		undervarth1
							15:10    reserverd
							9:  0		undervarth0
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_undervarth1_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_USHTVARTH ISP_SHARPEN_USHTVARTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_USHTVARTH.u32 = pstIspReg->ISP_SHARPEN_USHTVARTH.u32;
	ISP_SHARPEN_USHTVARTH.bits.undervarth1 = data;
	pstIspReg->ISP_SHARPEN_USHTVARTH.u32 = ISP_SHARPEN_USHTVARTH.u32;
}

static __inline HI_U16 hi_isp_sharpen_undervarth1_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_USHTVARTH ISP_SHARPEN_USHTVARTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_USHTVARTH.u32 = pstIspReg->ISP_SHARPEN_USHTVARTH.u32;
	return ISP_SHARPEN_USHTVARTH.bits.undervarth1;
}	

static __inline HI_VOID hi_isp_sharpen_undervarth0_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_USHTVARTH ISP_SHARPEN_USHTVARTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_USHTVARTH.u32 = pstIspReg->ISP_SHARPEN_USHTVARTH.u32;
	ISP_SHARPEN_USHTVARTH.bits.undervarth0 = data;
	pstIspReg->ISP_SHARPEN_USHTVARTH.u32 = ISP_SHARPEN_USHTVARTH.u32;
}

static __inline HI_U16 hi_isp_sharpen_undervarth0_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_USHTVARTH ISP_SHARPEN_USHTVARTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_USHTVARTH.u32 = pstIspReg->ISP_SHARPEN_USHTVARTH.u32;
	return ISP_SHARPEN_USHTVARTH.bits.undervarth0;
}


/*****************************************************
ISP_SHARPEN_USHTVAR:    
0x113c525c:                   31:24   reserverd
                                      28:16		undervarmul 	
                                      15:10    reserverd
                                       7:  0		undervaramt
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_undervarmul_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_USHTVAR ISP_SHARPEN_USHTVAR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_USHTVAR.u32 = pstIspReg->ISP_SHARPEN_USHTVAR.u32;
	ISP_SHARPEN_USHTVAR.bits.undervarmul = data;
	pstIspReg->ISP_SHARPEN_USHTVAR.u32 = ISP_SHARPEN_USHTVAR.u32;
}

static __inline HI_U16 hi_isp_sharpen_undervarmul_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_USHTVAR ISP_SHARPEN_USHTVAR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_USHTVAR.u32 = pstIspReg->ISP_SHARPEN_USHTVAR.u32;
	return ISP_SHARPEN_USHTVAR.bits.undervarmul;
}

static __inline HI_VOID hi_isp_sharpen_undervaramt_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_USHTVAR ISP_SHARPEN_USHTVAR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_USHTVAR.u32 = pstIspReg->ISP_SHARPEN_USHTVAR.u32;
	ISP_SHARPEN_USHTVAR.bits.undervaramt = data;
	pstIspReg->ISP_SHARPEN_USHTVAR.u32 = ISP_SHARPEN_USHTVAR.u32;
}

static __inline HI_U8 hi_isp_sharpen_undervaramt_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_USHTVAR ISP_SHARPEN_USHTVAR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_USHTVAR.u32 = pstIspReg->ISP_SHARPEN_USHTVAR.u32;
	return ISP_SHARPEN_USHTVAR.bits.undervaramt;
}

/*****************************************************
ISP_SHARPEN_OSHTLUMA:   
0x113c5260:                     	    31:24        lumawgt3
                                          23:16		lumawgt2 	
                                          15:8		lumawgt1
                                          7:  0		lumawgt0
*****************************************************/
#define HI_ISP_SHARPEN_LUMAWGT_INDEX_MAX (8)
static __inline HI_VOID hi_isp_sharpen_lumawgt_write(HI_U8 ispDev, HI_U8 index, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_LUMAWGT ISP_SHARPEN_LUMAWGT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_LUMAWGT.u32 = data;
	pstIspReg->ISP_SHARPEN_LUMAWGT[index].u32 = ISP_SHARPEN_LUMAWGT.u32;
}

static __inline HI_U32 hi_isp_sharpen_lumawgt_read(HI_U8 ispDev, HI_U8 index) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_LUMAWGT ISP_SHARPEN_LUMAWGT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_LUMAWGT.u32 = pstIspReg->ISP_SHARPEN_LUMAWGT[index].u32;
	return ISP_SHARPEN_LUMAWGT.u32;
}

/*****************************************************
ISP_SHARPEN_SIZE:      SHARPEN
0x205a42f0:                     31:29    reserved
                                          28:16   height
                                          15:13   reserved
                                          12:  0   width
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_height_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SIZE ISP_SHARPEN_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SIZE.u32 = pstIspReg->ISP_SHARPEN_SIZE.u32;
	ISP_SHARPEN_SIZE.bits.height  = data;
	pstIspReg->ISP_SHARPEN_SIZE.u32 = ISP_SHARPEN_SIZE.u32;
}

static __inline HI_VOID hi_isp_sharpen_width_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SIZE ISP_SHARPEN_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SIZE.u32 = pstIspReg->ISP_SHARPEN_SIZE.u32;
	ISP_SHARPEN_SIZE.bits.width = data;
	pstIspReg->ISP_SHARPEN_SIZE.u32 = ISP_SHARPEN_SIZE.u32;
}

/*****************************************************
ISP_SHARPEN_SKIN_CNT:      LUMAWgt
0x113c5310:                         31:24   reserved
                                          12:08	skincntmulcoef
                                          07:04	skin_cnt_th2
                                          03:00	skin_cnt_th1
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_skincntmulcoef_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SKIN_CNT ISP_SHARPEN_SKIN_CNT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SKIN_CNT.u32 = pstIspReg->ISP_SHARPEN_SKIN_CNT.u32;
	ISP_SHARPEN_SKIN_CNT.bits.skincntmulcoef = data;
	pstIspReg->ISP_SHARPEN_SKIN_CNT.u32 = ISP_SHARPEN_SKIN_CNT.u32;
}

static __inline HI_U8 hi_isp_sharpen_skincntmulcoef_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SKIN_CNT ISP_SHARPEN_SKIN_CNT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SKIN_CNT.u32 = pstIspReg->ISP_SHARPEN_SKIN_CNT.u32;
	return ISP_SHARPEN_SKIN_CNT.bits.skincntmulcoef;
}

static __inline HI_VOID hi_isp_sharpen_skin_cnt_th2_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SKIN_CNT ISP_SHARPEN_SKIN_CNT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SKIN_CNT.u32 = pstIspReg->ISP_SHARPEN_SKIN_CNT.u32;
	ISP_SHARPEN_SKIN_CNT.bits.skin_cnt_th2 = data;
	pstIspReg->ISP_SHARPEN_SKIN_CNT.u32 = ISP_SHARPEN_SKIN_CNT.u32;
}

static __inline HI_U8 hi_isp_sharpen_skin_cnt_th2_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SKIN_CNT ISP_SHARPEN_SKIN_CNT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SKIN_CNT.u32 = pstIspReg->ISP_SHARPEN_SKIN_CNT.u32;
	return ISP_SHARPEN_SKIN_CNT.bits.skin_cnt_th2;
}

static __inline HI_VOID hi_isp_sharpen_skin_cnt_th1_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SKIN_CNT ISP_SHARPEN_SKIN_CNT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SKIN_CNT.u32 = pstIspReg->ISP_SHARPEN_SKIN_CNT.u32;
	ISP_SHARPEN_SKIN_CNT.bits.skin_cnt_th1 = data;
	pstIspReg->ISP_SHARPEN_SKIN_CNT.u32 = ISP_SHARPEN_SKIN_CNT.u32;
}

static __inline HI_U8 hi_isp_sharpen_skin_cnt_th1_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SKIN_CNT ISP_SHARPEN_SKIN_CNT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SKIN_CNT.u32 = pstIspReg->ISP_SHARPEN_SKIN_CNT.u32;
	return ISP_SHARPEN_SKIN_CNT.bits.skin_cnt_th1;
}

/*****************************************************
ISP_SHARPEN_SKIN_VARTH:      LUMAWgt
0x113c5314:                       25:16     skin_var_th2
                                          09:00	skin_var_th1
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_skin_var_th2_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SKIN_VARTH ISP_SHARPEN_SKIN_VARTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SKIN_VARTH.u32 = pstIspReg->ISP_SHARPEN_SKIN_VARTH.u32;
	ISP_SHARPEN_SKIN_VARTH.bits.skin_var_th2 = data;
	pstIspReg->ISP_SHARPEN_SKIN_VARTH.u32 = ISP_SHARPEN_SKIN_VARTH.u32;
}

static __inline HI_U16 hi_isp_sharpen_skin_var_th2_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SKIN_VARTH ISP_SHARPEN_SKIN_VARTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SKIN_VARTH.u32 = pstIspReg->ISP_SHARPEN_SKIN_VARTH.u32;
	return ISP_SHARPEN_SKIN_VARTH.bits.skin_var_th2;
}

static __inline HI_VOID hi_isp_sharpen_skin_var_th1_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SKIN_VARTH ISP_SHARPEN_SKIN_VARTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SKIN_VARTH.u32 = pstIspReg->ISP_SHARPEN_SKIN_VARTH.u32;
	ISP_SHARPEN_SKIN_VARTH.bits.skin_var_th1 = data;
	pstIspReg->ISP_SHARPEN_SKIN_VARTH.u32 = ISP_SHARPEN_SKIN_VARTH.u32;
}

static __inline HI_U16 hi_isp_sharpen_skin_var_th1_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SKIN_VARTH ISP_SHARPEN_SKIN_VARTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SKIN_VARTH.u32 = pstIspReg->ISP_SHARPEN_SKIN_VARTH.u32;
	return ISP_SHARPEN_SKIN_VARTH.bits.skin_var_th1;
}

/*****************************************************
ISP_SHARPEN_SKIN_VARWGT:      LUMAWgt
0x113c5318:                       28:16     skinvarwgtmulcoef
						15:08     skin_var_wgt2
                                          07:00	skin_var_wgt1
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_skinvarwgtmulcoef_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SKIN_VARWGT ISP_SHARPEN_SKIN_VARWGT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SKIN_VARWGT.u32 = pstIspReg->ISP_SHARPEN_SKIN_VARWGT.u32;
	ISP_SHARPEN_SKIN_VARWGT.bits.skinvarwgtmulcoef = data;
	pstIspReg->ISP_SHARPEN_SKIN_VARWGT.u32 = ISP_SHARPEN_SKIN_VARWGT.u32;
}

static __inline HI_U16 hi_isp_sharpen_skinvarwgtmulcoef_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SKIN_VARWGT ISP_SHARPEN_SKIN_VARWGT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SKIN_VARWGT.u32 = pstIspReg->ISP_SHARPEN_SKIN_VARWGT.u32;
	return ISP_SHARPEN_SKIN_VARWGT.bits.skinvarwgtmulcoef;
}

static __inline HI_VOID hi_isp_sharpen_skin_var_wgt2_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SKIN_VARWGT ISP_SHARPEN_SKIN_VARWGT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SKIN_VARWGT.u32 = pstIspReg->ISP_SHARPEN_SKIN_VARWGT.u32;
	ISP_SHARPEN_SKIN_VARWGT.bits.skin_var_wgt2 = data;
	pstIspReg->ISP_SHARPEN_SKIN_VARWGT.u32 = ISP_SHARPEN_SKIN_VARWGT.u32;
}

static __inline HI_U8 hi_isp_sharpen_skin_var_wgt2_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SKIN_VARWGT ISP_SHARPEN_SKIN_VARWGT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SKIN_VARWGT.u32 = pstIspReg->ISP_SHARPEN_SKIN_VARWGT.u32;
	return ISP_SHARPEN_SKIN_VARWGT.bits.skin_var_wgt2;
}

static __inline HI_VOID hi_isp_sharpen_skin_var_wgt1_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SKIN_VARWGT ISP_SHARPEN_SKIN_VARWGT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SKIN_VARWGT.u32 = pstIspReg->ISP_SHARPEN_SKIN_VARWGT.u32;
	ISP_SHARPEN_SKIN_VARWGT.bits.skin_var_wgt1 = data;
	pstIspReg->ISP_SHARPEN_SKIN_VARWGT.u32 = ISP_SHARPEN_SKIN_VARWGT.u32;
}

static __inline HI_U8 hi_isp_sharpen_skin_var_wgt1_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SKIN_VARWGT ISP_SHARPEN_SKIN_VARWGT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SKIN_VARWGT.u32 = pstIspReg->ISP_SHARPEN_SKIN_VARWGT.u32;
	return ISP_SHARPEN_SKIN_VARWGT.bits.skin_var_wgt1;
}

/*****************************************************
ISP_SHARPEN_SKIN_U:      LUMAWgt
0x113c531c:                       
						23:16     skin_u_max
                                          07:00	skin_u_min
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_skin_u_max_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SKIN_U ISP_SHARPEN_SKIN_U;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SKIN_U.u32 = pstIspReg->ISP_SHARPEN_SKIN_U.u32;
	ISP_SHARPEN_SKIN_U.bits.skin_u_max = data;
	pstIspReg->ISP_SHARPEN_SKIN_U.u32 = ISP_SHARPEN_SKIN_U.u32;
}

static __inline HI_U8 hi_isp_sharpen_skin_u_max_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SKIN_U ISP_SHARPEN_SKIN_U;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SKIN_U.u32 = pstIspReg->ISP_SHARPEN_SKIN_U.u32;
	return ISP_SHARPEN_SKIN_U.bits.skin_u_max;
}

static __inline HI_VOID hi_isp_sharpen_skin_u_min_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SKIN_U ISP_SHARPEN_SKIN_U;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SKIN_U.u32 = pstIspReg->ISP_SHARPEN_SKIN_U.u32;
	ISP_SHARPEN_SKIN_U.bits.skin_u_min = data;
	pstIspReg->ISP_SHARPEN_SKIN_U.u32 = ISP_SHARPEN_SKIN_U.u32;
}

static __inline HI_U8 hi_isp_sharpen_skin_u_min_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SKIN_U ISP_SHARPEN_SKIN_U;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SKIN_U.u32 = pstIspReg->ISP_SHARPEN_SKIN_U.u32;
	return ISP_SHARPEN_SKIN_U.bits.skin_u_min;
}

/*****************************************************
ISP_SHARPEN_SKIN_V:      LUMAWgt
0x113c5320:                       
						23:16     skin_v_max
						07:00	skin_v_min
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_skin_v_max_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SKIN_V ISP_SHARPEN_SKIN_V;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SKIN_V.u32 = pstIspReg->ISP_SHARPEN_SKIN_V.u32;
	ISP_SHARPEN_SKIN_V.bits.skin_v_max = data;
	pstIspReg->ISP_SHARPEN_SKIN_V.u32 = ISP_SHARPEN_SKIN_V.u32;
}

static __inline HI_U8 hi_isp_sharpen_skin_v_max_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SKIN_V ISP_SHARPEN_SKIN_V;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SKIN_V.u32 = pstIspReg->ISP_SHARPEN_SKIN_V.u32;
	return ISP_SHARPEN_SKIN_V.bits.skin_v_max;
}

static __inline HI_VOID hi_isp_sharpen_skin_v_min_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SKIN_V ISP_SHARPEN_SKIN_V;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SKIN_V.u32 = pstIspReg->ISP_SHARPEN_SKIN_V.u32;
	ISP_SHARPEN_SKIN_V.bits.skin_v_min = data;
	pstIspReg->ISP_SHARPEN_SKIN_V.u32 = ISP_SHARPEN_SKIN_V.u32;
}

static __inline HI_U8 hi_isp_sharpen_skin_v_min_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SKIN_V ISP_SHARPEN_SKIN_V;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SKIN_V.u32 = pstIspReg->ISP_SHARPEN_SKIN_V.u32;
	return ISP_SHARPEN_SKIN_V.bits.skin_v_min;
}

/*****************************************************
ISP_SHARPEN_SKIN_LEN:      LUMAWgt
0x113c5324:                       
						
                                          03:00	skin_len
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_skin_len_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SKIN_LEN ISP_SHARPEN_SKIN_LEN;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SKIN_LEN.u32 = pstIspReg->ISP_SHARPEN_SKIN_LEN.u32;
	ISP_SHARPEN_SKIN_LEN.bits.skin_len = data;
	pstIspReg->ISP_SHARPEN_SKIN_LEN.u32 = ISP_SHARPEN_SKIN_LEN.u32;
}

static __inline HI_U8 hi_isp_sharpen_skin_len_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SKIN_LEN ISP_SHARPEN_SKIN_LEN;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SKIN_LEN.u32 = pstIspReg->ISP_SHARPEN_SKIN_LEN.u32;
	return ISP_SHARPEN_SKIN_LEN.bits.skin_len;
}

/*****************************************************
ISP_SHARPEN_EDGSM:     
0x113c5328:             24:24 edgsm_en
						16:12 edgsm_str
						11:04 diff_thd
						03:00 diff_mul
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_diff_mul_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_EDGSM ISP_SHARPEN_EDGSM;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_EDGSM.u32 = pstIspReg->ISP_SHARPEN_EDGSM.u32;
	ISP_SHARPEN_EDGSM.bits.diff_mul = data;
	pstIspReg->ISP_SHARPEN_EDGSM.u32 = ISP_SHARPEN_EDGSM.u32;
}

static __inline HI_U8 hi_isp_sharpen_diff_mul_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_EDGSM ISP_SHARPEN_EDGSM;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_EDGSM.u32 = pstIspReg->ISP_SHARPEN_EDGSM.u32;
	return ISP_SHARPEN_EDGSM.bits.diff_mul;
}

static __inline HI_VOID hi_isp_sharpen_diff_thd_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_EDGSM ISP_SHARPEN_EDGSM;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_EDGSM.u32 = pstIspReg->ISP_SHARPEN_EDGSM.u32;
	ISP_SHARPEN_EDGSM.bits.diff_thd = data;
	pstIspReg->ISP_SHARPEN_EDGSM.u32 = ISP_SHARPEN_EDGSM.u32;
}

static __inline HI_U16 hi_isp_sharpen_diff_thd_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_EDGSM ISP_SHARPEN_EDGSM;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_EDGSM.u32 = pstIspReg->ISP_SHARPEN_EDGSM.u32;
	return ISP_SHARPEN_EDGSM.bits.diff_thd;
}

static __inline HI_VOID hi_isp_sharpen_edgsm_str_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_EDGSM ISP_SHARPEN_EDGSM;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_EDGSM.u32 = pstIspReg->ISP_SHARPEN_EDGSM.u32;
	ISP_SHARPEN_EDGSM.bits.edgsm_str = data;
	pstIspReg->ISP_SHARPEN_EDGSM.u32 = ISP_SHARPEN_EDGSM.u32;
}

static __inline HI_U32 hi_isp_sharpen_edgsm_str_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_EDGSM ISP_SHARPEN_EDGSM;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_EDGSM.u32 = pstIspReg->ISP_SHARPEN_EDGSM.u32;
	return ISP_SHARPEN_EDGSM.bits.edgsm_str;
}

static __inline HI_VOID hi_isp_sharpen_edgsm_en_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_EDGSM ISP_SHARPEN_EDGSM;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_EDGSM.u32 = pstIspReg->ISP_SHARPEN_EDGSM.u32;
	ISP_SHARPEN_EDGSM.bits.edgsm_en = data;
	pstIspReg->ISP_SHARPEN_EDGSM.u32 = ISP_SHARPEN_EDGSM.u32;
}

static __inline HI_U8 hi_isp_sharpen_edgsm_en_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_EDGSM ISP_SHARPEN_EDGSM;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_EDGSM.u32 = pstIspReg->ISP_SHARPEN_EDGSM.u32;
	return ISP_SHARPEN_EDGSM.bits.edgsm_en;
}

static __inline HI_VOID hi_isp_sharpen_bshpgainmul_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SHPGAIN_MUL ISP_SHARPEN_SHPGAIN_MUL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SHPGAIN_MUL.u32 = pstIspReg->ISP_SHARPEN_SHPGAIN_MUL.u32;
	ISP_SHARPEN_SHPGAIN_MUL.bits.bshpgainmul = data;
	pstIspReg->ISP_SHARPEN_SHPGAIN_MUL.u32 = ISP_SHARPEN_SHPGAIN_MUL.u32;
}

static __inline HI_VOID hi_isp_sharpen_rshpgainmul_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SHPGAIN_MUL ISP_SHARPEN_SHPGAIN_MUL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SHPGAIN_MUL.u32 = pstIspReg->ISP_SHARPEN_SHPGAIN_MUL.u32;
	ISP_SHARPEN_SHPGAIN_MUL.bits.rshpgainmul = data;
	pstIspReg->ISP_SHARPEN_SHPGAIN_MUL.u32 = ISP_SHARPEN_SHPGAIN_MUL.u32;
}


/*****************************************************
U_ISP_SHARPEN_OSUP_VAR:    
0x113c5330:             
					oshtvarmin			  : 8	; 
					oshtvarmax			  : 8	; 
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_oshtvar_min_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_OSUP_VAR ISP_SHARPEN_OSUP_VAR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_OSUP_VAR.u32 = pstIspReg->ISP_SHARPEN_OSUP_VAR.u32;
	ISP_SHARPEN_OSUP_VAR.bits.oshtvarmin = data;
	pstIspReg->ISP_SHARPEN_OSUP_VAR.u32 = ISP_SHARPEN_OSUP_VAR.u32;
}

static __inline HI_VOID hi_isp_sharpen_oshtvar_max_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_OSUP_VAR ISP_SHARPEN_OSUP_VAR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_OSUP_VAR.u32 = pstIspReg->ISP_SHARPEN_OSUP_VAR.u32;
	ISP_SHARPEN_OSUP_VAR.bits.oshtvarmax = data;
	pstIspReg->ISP_SHARPEN_OSUP_VAR.u32 = ISP_SHARPEN_OSUP_VAR.u32;
}

/*****************************************************
U_ISP_SHARPEN_OSUP_VARCOEF:      
0x113c5334:             
				oshtsupmul            : 12
				oshtwgtmin            : 8
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_oshtsup_mul_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_OSUP_VARCOEF ISP_SHARPEN_OSUP_VARCOEF;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_OSUP_VARCOEF.u32 = pstIspReg->ISP_SHARPEN_OSUP_VARCOEF.u32;
	ISP_SHARPEN_OSUP_VARCOEF.bits.oshtsupmul = data;
	pstIspReg->ISP_SHARPEN_OSUP_VARCOEF.u32 = ISP_SHARPEN_OSUP_VARCOEF.u32;
}

static __inline HI_VOID hi_isp_sharpen_oshtwgt_min_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_OSUP_VARCOEF ISP_SHARPEN_OSUP_VARCOEF;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_OSUP_VARCOEF.u32 = pstIspReg->ISP_SHARPEN_OSUP_VARCOEF.u32;
	ISP_SHARPEN_OSUP_VARCOEF.bits.oshtwgtmin = data;
	pstIspReg->ISP_SHARPEN_OSUP_VARCOEF.u32 = ISP_SHARPEN_OSUP_VARCOEF.u32;
}

/*****************************************************
U_ISP_SHARPEN_USUP_VAR:    
0x113c5338:             
					ushtvarmin            : 8
					ushtvarmax            : 8
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_ushtvar_min_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_USUP_VAR ISP_SHARPEN_USUP_VAR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_USUP_VAR.u32 = pstIspReg->ISP_SHARPEN_USUP_VAR.u32;
	ISP_SHARPEN_USUP_VAR.bits.ushtvarmin = data;
	pstIspReg->ISP_SHARPEN_USUP_VAR.u32 = ISP_SHARPEN_USUP_VAR.u32;
}

static __inline HI_VOID hi_isp_sharpen_ushtvar_max_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_USUP_VAR ISP_SHARPEN_USUP_VAR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_USUP_VAR.u32 = pstIspReg->ISP_SHARPEN_USUP_VAR.u32;
	ISP_SHARPEN_USUP_VAR.bits.ushtvarmax = data;
	pstIspReg->ISP_SHARPEN_USUP_VAR.u32 = ISP_SHARPEN_USUP_VAR.u32;
}

/*****************************************************
U_ISP_SHARPEN_USUP_VARCOEF:    
0x113c533c:             
				ushtsupmul            : 12
				ushtwgtmin            : 8 
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_ushtsup_mul_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_USUP_VARCOEF ISP_SHARPEN_USUP_VARCOEF;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_USUP_VARCOEF.u32 = pstIspReg->ISP_SHARPEN_USUP_VARCOEF.u32;
	ISP_SHARPEN_USUP_VARCOEF.bits.ushtsupmul = data;
	pstIspReg->ISP_SHARPEN_USUP_VARCOEF.u32 = ISP_SHARPEN_USUP_VARCOEF.u32;
}

static __inline HI_VOID hi_isp_sharpen_ushtwgt_min_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_USUP_VARCOEF ISP_SHARPEN_USUP_VARCOEF;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_USUP_VARCOEF.u32 = pstIspReg->ISP_SHARPEN_USUP_VARCOEF.u32;
	ISP_SHARPEN_USUP_VARCOEF.bits.ushtwgtmin = data;
	pstIspReg->ISP_SHARPEN_USUP_VARCOEF.u32 = ISP_SHARPEN_USUP_VARCOEF.u32;
}

/*****************************************************
ISP_SHARPEN_OSUP_DIFF:    
0x113c5340:             
				oshtdiffmin           : 8
				oshtdiffmax           : 8
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_oshtdiff_min_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_OSUP_DIFF ISP_SHARPEN_OSUP_DIFF;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_OSUP_DIFF.u32 = pstIspReg->ISP_SHARPEN_OSUP_DIFF.u32;
	ISP_SHARPEN_OSUP_DIFF.bits.oshtdiffmin = data;
	pstIspReg->ISP_SHARPEN_OSUP_DIFF.u32 = ISP_SHARPEN_OSUP_DIFF.u32;
}

static __inline HI_VOID hi_isp_sharpen_oshtdiff_max_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_OSUP_DIFF ISP_SHARPEN_OSUP_DIFF;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_OSUP_DIFF.u32 = pstIspReg->ISP_SHARPEN_OSUP_DIFF.u32;
	ISP_SHARPEN_OSUP_DIFF.bits.oshtdiffmax = data;
	pstIspReg->ISP_SHARPEN_OSUP_DIFF.u32 = ISP_SHARPEN_OSUP_DIFF.u32;
}

/*****************************************************
U_ISP_SHARPEN_OSUP_DIFFCOEF:    
0x113c5344:             
				oshtsupdiffmul        : 13
				oshtdiffwgtmin        : 8 
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_oshtsupdiff_mul_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_OSUP_DIFFCOEF ISP_SHARPEN_OSUP_DIFFCOEF;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_OSUP_DIFFCOEF.u32 = pstIspReg->ISP_SHARPEN_OSUP_DIFFCOEF.u32;
	ISP_SHARPEN_OSUP_DIFFCOEF.bits.oshtsupdiffmul = data;
	pstIspReg->ISP_SHARPEN_OSUP_DIFFCOEF.u32 = ISP_SHARPEN_OSUP_DIFFCOEF.u32;
}

static __inline HI_VOID hi_isp_sharpen_oshtdiffwgt_min_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_OSUP_DIFFCOEF ISP_SHARPEN_OSUP_DIFFCOEF;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_OSUP_DIFFCOEF.u32 = pstIspReg->ISP_SHARPEN_OSUP_DIFFCOEF.u32;
	ISP_SHARPEN_OSUP_DIFFCOEF.bits.oshtdiffwgtmin = data;
	pstIspReg->ISP_SHARPEN_OSUP_DIFFCOEF.u32 = ISP_SHARPEN_OSUP_DIFFCOEF.u32;
}

/*****************************************************
U_ISP_SHARPEN_USUP_DIFF:     
0x113c5348:             
				ushtdiffmin           : 8
				ushtdiffmax           : 8
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_ushtdiff_min_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_USUP_DIFF ISP_SHARPEN_USUP_DIFF;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_USUP_DIFF.u32 = pstIspReg->ISP_SHARPEN_USUP_DIFF.u32;
	ISP_SHARPEN_USUP_DIFF.bits.ushtdiffmin = data;
	pstIspReg->ISP_SHARPEN_USUP_DIFF.u32 = ISP_SHARPEN_USUP_DIFF.u32;
}

static __inline HI_VOID hi_isp_sharpen_ushtdiff_max_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_USUP_DIFF ISP_SHARPEN_USUP_DIFF;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_USUP_DIFF.u32 = pstIspReg->ISP_SHARPEN_USUP_DIFF.u32;
	ISP_SHARPEN_USUP_DIFF.bits.ushtdiffmax = data;
	pstIspReg->ISP_SHARPEN_USUP_DIFF.u32 = ISP_SHARPEN_USUP_DIFF.u32;
}

/*****************************************************
U_ISP_SHARPEN_USUP_DIFFCOEF:     
0x113c534c:             
ushtsupdiffmul        : 13 
ushtdiffwgtmin        : 8  
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_ushtsupdiff_mul_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_USUP_DIFFCOEF ISP_SHARPEN_USUP_DIFFCOEF;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_USUP_DIFFCOEF.u32 = pstIspReg->ISP_SHARPEN_USUP_DIFFCOEF.u32;
	ISP_SHARPEN_USUP_DIFFCOEF.bits.ushtsupdiffmul = data;
	pstIspReg->ISP_SHARPEN_USUP_DIFFCOEF.u32 = ISP_SHARPEN_USUP_DIFFCOEF.u32;
}

static __inline HI_VOID hi_isp_sharpen_ushtdiffwgt_min_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_USUP_DIFFCOEF ISP_SHARPEN_USUP_DIFFCOEF;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_USUP_DIFFCOEF.u32 = pstIspReg->ISP_SHARPEN_USUP_DIFFCOEF.u32;
	ISP_SHARPEN_USUP_DIFFCOEF.bits.ushtdiffwgtmin = data;
	pstIspReg->ISP_SHARPEN_USUP_DIFFCOEF.u32 = ISP_SHARPEN_USUP_DIFFCOEF.u32;
}

/*****************************************************
U_ISP_SHARPEN_SUP_NOS:          
0x113c5350:             
			minnosthd             : 8
			maxnosthd             : 8
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_minnosthd_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SUP_NOS ISP_SHARPEN_SUP_NOS;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SUP_NOS.u32 = pstIspReg->ISP_SHARPEN_SUP_NOS.u32;
	ISP_SHARPEN_SUP_NOS.bits.minnosthd = data;
	pstIspReg->ISP_SHARPEN_SUP_NOS.u32 = ISP_SHARPEN_SUP_NOS.u32;
}

static __inline HI_VOID hi_isp_sharpen_maxnosthd_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SUP_NOS ISP_SHARPEN_SUP_NOS;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SUP_NOS.u32 = pstIspReg->ISP_SHARPEN_SUP_NOS.u32;
	ISP_SHARPEN_SUP_NOS.bits.maxnosthd = data;
	pstIspReg->ISP_SHARPEN_SUP_NOS.u32 = ISP_SHARPEN_SUP_NOS.u32;
}

/*****************************************************
U_ISP_SHARPEN_SUP_BLDR:          
0x113c5354:             
				shtsupbldr            : 4   
				shtsup_en             : 1  
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_shtsupbldr_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SUP_BLDR ISP_SHARPEN_SUP_BLDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SUP_BLDR.u32 = pstIspReg->ISP_SHARPEN_SUP_BLDR.u32;
	ISP_SHARPEN_SUP_BLDR.bits.shtsupbldr = data;
	pstIspReg->ISP_SHARPEN_SUP_BLDR.u32 = ISP_SHARPEN_SUP_BLDR.u32;
}

static __inline HI_VOID hi_isp_sharpen_shtsup_en_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SUP_BLDR ISP_SHARPEN_SUP_BLDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SUP_BLDR.u32 = pstIspReg->ISP_SHARPEN_SUP_BLDR.u32;
	ISP_SHARPEN_SUP_BLDR.bits.shtsup_en = data;
	pstIspReg->ISP_SHARPEN_SUP_BLDR.u32 = ISP_SHARPEN_SUP_BLDR.u32;
}

static __inline HI_VOID hi_isp_sharpen_shtmax_en_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SUP_BLDR ISP_SHARPEN_SUP_BLDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SUP_BLDR.u32 = pstIspReg->ISP_SHARPEN_SUP_BLDR.u32;
	ISP_SHARPEN_SUP_BLDR.bits.shtmax_en = data;
	pstIspReg->ISP_SHARPEN_SUP_BLDR.u32 = ISP_SHARPEN_SUP_BLDR.u32;
}

/*****************************************************
ISP_SHARPEN_UDHFGAIN:               
0x113c5358:             
udhfgain        : 09:00 
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_udhfgain_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_UDHFGAIN ISP_SHARPEN_UDHFGAIN;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_UDHFGAIN.u32 = pstIspReg->ISP_SHARPEN_UDHFGAIN.u32;
	ISP_SHARPEN_UDHFGAIN.bits.udhfgain = data;
	pstIspReg->ISP_SHARPEN_UDHFGAIN.u32 = ISP_SHARPEN_UDHFGAIN.u32;
}

/*****************************************************
ISP_SHARPEN_SHTMAX:     
0x113c535c:             
oshtmaxvargain        07:00 
ushtmaxvargain        15:08 
oshtmaxchg             23:16
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_oshtmaxvargain_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SHTMAX ISP_SHARPEN_SHTMAX;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SHTMAX.u32 = pstIspReg->ISP_SHARPEN_SHTMAX.u32;
	ISP_SHARPEN_SHTMAX.bits.oshtmaxvargain = data;
	pstIspReg->ISP_SHARPEN_SHTMAX.u32 = ISP_SHARPEN_SHTMAX.u32;
}

static __inline HI_VOID hi_isp_sharpen_ushtmaxvargain_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SHTMAX ISP_SHARPEN_SHTMAX;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SHTMAX.u32 = pstIspReg->ISP_SHARPEN_SHTMAX.u32;
	ISP_SHARPEN_SHTMAX.bits.ushtmaxvargain = data;
	pstIspReg->ISP_SHARPEN_SHTMAX.u32 = ISP_SHARPEN_SHTMAX.u32;
}

static __inline HI_VOID hi_isp_sharpen_oshtmaxchg_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SHTMAX ISP_SHARPEN_SHTMAX;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SHTMAX.u32 = pstIspReg->ISP_SHARPEN_SHTMAX.u32;
	ISP_SHARPEN_SHTMAX.bits.oshtmaxchg = data;
	pstIspReg->ISP_SHARPEN_SHTMAX.u32 = ISP_SHARPEN_SHTMAX.u32;
}

static __inline HI_VOID hi_isp_sharpen_ushtmaxchg_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SHTMAX ISP_SHARPEN_SHTMAX;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SHTMAX.u32 = pstIspReg->ISP_SHARPEN_SHTMAX.u32;
	ISP_SHARPEN_SHTMAX.bits.ushtmaxchg = data;
	pstIspReg->ISP_SHARPEN_SHTMAX.u32 = ISP_SHARPEN_SHTMAX.u32;
}

static __inline HI_VOID hi_isp_sharpen_shadvar1_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SHADVAR ISP_SHARPEN_SHADVAR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SHADVAR.u32 = pstIspReg->ISP_SHARPEN_SHADVAR.u32;
	ISP_SHARPEN_SHADVAR.bits.shadvar1 = data;
	pstIspReg->ISP_SHARPEN_SHADVAR.u32 = ISP_SHARPEN_SHADVAR.u32;
}

static __inline HI_VOID hi_isp_sharpen_shadvar2_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_SHADVAR ISP_SHARPEN_SHADVAR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_SHADVAR.u32 = pstIspReg->ISP_SHARPEN_SHADVAR.u32;
	ISP_SHARPEN_SHADVAR.bits.shadvar2 = data;
	pstIspReg->ISP_SHARPEN_SHADVAR.u32 = ISP_SHARPEN_SHADVAR.u32;
}

static __inline HI_VOID hi_isp_sharpen_gradthd1_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_GRADTHD ISP_SHARPEN_GRADTHD;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_GRADTHD.u32 = pstIspReg->ISP_SHARPEN_GRADTHD.u32;
	ISP_SHARPEN_GRADTHD.bits.gradthd1 = data;
	pstIspReg->ISP_SHARPEN_GRADTHD.u32 = ISP_SHARPEN_GRADTHD.u32;
}

static __inline HI_VOID hi_isp_sharpen_gradthd2_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_GRADTHD ISP_SHARPEN_GRADTHD;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_GRADTHD.u32 = pstIspReg->ISP_SHARPEN_GRADTHD.u32;
	ISP_SHARPEN_GRADTHD.bits.gradthd2 = data;
	pstIspReg->ISP_SHARPEN_GRADTHD.u32 = ISP_SHARPEN_GRADTHD.u32;
}

static __inline HI_VOID hi_isp_sharpen_wgtthd1_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_WGTCOEF ISP_SHARPEN_WGTCOEF;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_WGTCOEF.u32 = pstIspReg->ISP_SHARPEN_WGTCOEF.u32;
	ISP_SHARPEN_WGTCOEF.bits.wgtthd1 = data;
	pstIspReg->ISP_SHARPEN_WGTCOEF.u32 = ISP_SHARPEN_WGTCOEF.u32;
}

static __inline HI_VOID hi_isp_sharpen_wgtthd2_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_WGTCOEF ISP_SHARPEN_WGTCOEF;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_WGTCOEF.u32 = pstIspReg->ISP_SHARPEN_WGTCOEF.u32;
	ISP_SHARPEN_WGTCOEF.bits.wgtthd2 = data;
	pstIspReg->ISP_SHARPEN_WGTCOEF.u32 = ISP_SHARPEN_WGTCOEF.u32;
}

static __inline HI_VOID hi_isp_sharpen_wgtmul_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_WGTCOEF ISP_SHARPEN_WGTCOEF;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_WGTCOEF.u32 = pstIspReg->ISP_SHARPEN_WGTCOEF.u32;
	ISP_SHARPEN_WGTCOEF.bits.wgtmul = data;
	pstIspReg->ISP_SHARPEN_WGTCOEF.u32 = ISP_SHARPEN_WGTCOEF.u32;
}

static __inline HI_VOID hi_isp_sharpen_rcbmax_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_RC ISP_SHARPEN_RC;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_RC.u32 = pstIspReg->ISP_SHARPEN_RC.u32;
	ISP_SHARPEN_RC.bits.rcbmax = data;
	pstIspReg->ISP_SHARPEN_RC.u32 = ISP_SHARPEN_RC.u32;
}

static __inline HI_VOID hi_isp_sharpen_rcrmax_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_RC ISP_SHARPEN_RC;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_RC.u32 = pstIspReg->ISP_SHARPEN_RC.u32;
	ISP_SHARPEN_RC.bits.rcrmax = data;
	pstIspReg->ISP_SHARPEN_RC.u32 = ISP_SHARPEN_RC.u32;
}

static __inline HI_VOID hi_isp_sharpen_rcbmin_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_RC ISP_SHARPEN_RC;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_RC.u32 = pstIspReg->ISP_SHARPEN_RC.u32;
	ISP_SHARPEN_RC.bits.rcbmin = data;
	pstIspReg->ISP_SHARPEN_RC.u32 = ISP_SHARPEN_RC.u32;
}

static __inline HI_VOID hi_isp_sharpen_rcrmin_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_RC ISP_SHARPEN_RC;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_RC.u32 = pstIspReg->ISP_SHARPEN_RC.u32;
	ISP_SHARPEN_RC.bits.rcrmin = data;
	pstIspReg->ISP_SHARPEN_RC.u32 = ISP_SHARPEN_RC.u32;
}

static __inline HI_VOID hi_isp_sharpen_bcbmax_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_BC ISP_SHARPEN_BC;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_BC.u32 = pstIspReg->ISP_SHARPEN_BC.u32;
	ISP_SHARPEN_BC.bits.bcbmax = data;
	pstIspReg->ISP_SHARPEN_BC.u32 = ISP_SHARPEN_BC.u32;
}

static __inline HI_VOID hi_isp_sharpen_bcrmax_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_BC ISP_SHARPEN_BC;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_BC.u32 = pstIspReg->ISP_SHARPEN_BC.u32;
	ISP_SHARPEN_BC.bits.bcrmax = data;
	pstIspReg->ISP_SHARPEN_BC.u32 = ISP_SHARPEN_BC.u32;
}

static __inline HI_VOID hi_isp_sharpen_bcbmin_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_BC ISP_SHARPEN_BC;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_BC.u32 = pstIspReg->ISP_SHARPEN_BC.u32;
	ISP_SHARPEN_BC.bits.bcbmin = data;
	pstIspReg->ISP_SHARPEN_BC.u32 = ISP_SHARPEN_BC.u32;
}

static __inline HI_VOID hi_isp_sharpen_bcrmin_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_BC ISP_SHARPEN_BC;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_BC.u32 = pstIspReg->ISP_SHARPEN_BC.u32;
	ISP_SHARPEN_BC.bits.bcrmin = data;
	pstIspReg->ISP_SHARPEN_BC.u32 = ISP_SHARPEN_BC.u32;
}

static __inline HI_VOID hi_isp_sharpen_gcbmax_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_GC ISP_SHARPEN_GC;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_GC.u32 = pstIspReg->ISP_SHARPEN_GC.u32;
	ISP_SHARPEN_GC.bits.gcbmax = data;
	pstIspReg->ISP_SHARPEN_GC.u32 = ISP_SHARPEN_GC.u32;
}

static __inline HI_VOID hi_isp_sharpen_gcrmax_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_GC ISP_SHARPEN_GC;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_GC.u32 = pstIspReg->ISP_SHARPEN_GC.u32;
	ISP_SHARPEN_GC.bits.gcrmax = data;
	pstIspReg->ISP_SHARPEN_GC.u32 = ISP_SHARPEN_GC.u32;
}

static __inline HI_VOID hi_isp_sharpen_gcbmin_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_GC ISP_SHARPEN_GC;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_GC.u32 = pstIspReg->ISP_SHARPEN_GC.u32;
	ISP_SHARPEN_GC.bits.gcbmin = data;
	pstIspReg->ISP_SHARPEN_GC.u32 = ISP_SHARPEN_GC.u32;
}

static __inline HI_VOID hi_isp_sharpen_gcrmin_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_GC ISP_SHARPEN_GC;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_GC.u32 = pstIspReg->ISP_SHARPEN_GC.u32;
	ISP_SHARPEN_GC.bits.gcrmin = data;
	pstIspReg->ISP_SHARPEN_GC.u32 = ISP_SHARPEN_GC.u32;
}

static __inline HI_VOID hi_isp_sharpen_gwgt_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_RGBWGT ISP_SHARPEN_RGBWGT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_RGBWGT.u32 = pstIspReg->ISP_SHARPEN_RGBWGT.u32;
	ISP_SHARPEN_RGBWGT.bits.gwgt = data;
	pstIspReg->ISP_SHARPEN_RGBWGT.u32 = ISP_SHARPEN_RGBWGT.u32;
}

static __inline HI_VOID hi_isp_sharpen_bwgt_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_RGBWGT ISP_SHARPEN_RGBWGT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_RGBWGT.u32 = pstIspReg->ISP_SHARPEN_RGBWGT.u32;
	ISP_SHARPEN_RGBWGT.bits.bwgt = data;
	pstIspReg->ISP_SHARPEN_RGBWGT.u32 = ISP_SHARPEN_RGBWGT.u32;
}

static __inline HI_VOID hi_isp_sharpen_rwgt_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_RGBWGT ISP_SHARPEN_RGBWGT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_RGBWGT.u32 = pstIspReg->ISP_SHARPEN_RGBWGT.u32;
	ISP_SHARPEN_RGBWGT.bits.rwgt = data;
	pstIspReg->ISP_SHARPEN_RGBWGT.u32 = ISP_SHARPEN_RGBWGT.u32;
}

static __inline HI_VOID hi_isp_sharpen_bshpgainsft_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_RBCOEF ISP_SHARPEN_RBCOEF;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_RBCOEF.u32 = pstIspReg->ISP_SHARPEN_RBCOEF.u32;
	ISP_SHARPEN_RBCOEF.bits.bshpgainsft = data;
	pstIspReg->ISP_SHARPEN_RBCOEF.u32 = ISP_SHARPEN_RBCOEF.u32;
}

static __inline HI_VOID hi_isp_sharpen_rshpgainsft_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_RBCOEF ISP_SHARPEN_RBCOEF;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_RBCOEF.u32 = pstIspReg->ISP_SHARPEN_RBCOEF.u32;
	ISP_SHARPEN_RBCOEF.bits.rshpgainsft = data;
	pstIspReg->ISP_SHARPEN_RBCOEF.u32 = ISP_SHARPEN_RBCOEF.u32;
}

static __inline HI_VOID hi_isp_sharpen_bcbthd_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_RBCOEF ISP_SHARPEN_RBCOEF;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_RBCOEF.u32 = pstIspReg->ISP_SHARPEN_RBCOEF.u32;
	ISP_SHARPEN_RBCOEF.bits.bcbthd = data;
	pstIspReg->ISP_SHARPEN_RBCOEF.u32 = ISP_SHARPEN_RBCOEF.u32;
}

static __inline HI_VOID hi_isp_sharpen_rcrthd_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_SHARPEN_RBCOEF ISP_SHARPEN_RBCOEF;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_SHARPEN_RBCOEF.u32 = pstIspReg->ISP_SHARPEN_RBCOEF.u32;
	ISP_SHARPEN_RBCOEF.bits.rcrthd = data;
	pstIspReg->ISP_SHARPEN_RBCOEF.u32 = ISP_SHARPEN_RBCOEF.u32;
}

/*****************************************************
MODULE        -----NDDM-----
MODULE        -----NDDM-----
MODULE        -----NDDM-----
*****************************************************/
static __inline HI_VOID hi_isp_nddm_enable_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_CFG ISP_NDDM_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_CFG.u32 = pstIspReg->ISP_NDDM_CFG.u32;
	ISP_NDDM_CFG.bits.en = data;
	pstIspReg->ISP_NDDM_CFG.u32 = ISP_NDDM_CFG.u32;
}

static __inline HI_U8 hi_isp_nddm_enable_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_CFG ISP_NDDM_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_CFG.u32 = pstIspReg->ISP_NDDM_CFG.u32;
	return ISP_NDDM_CFG.bits.en ;
}

static __inline HI_VOID hi_isp_nddm_cac_blend_en_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_CFG ISP_NDDM_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_CFG.u32 = pstIspReg->ISP_NDDM_CFG.u32;
	ISP_NDDM_CFG.bits.cac_blend_en = data;
	pstIspReg->ISP_NDDM_CFG.u32 = ISP_NDDM_CFG.u32;
}

#define HI_ISP_NDDM_GF_TH_HIGH_DEFAULT (256)
static __inline HI_VOID hi_isp_nddm_gf_th_high_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_GF_TH ISP_NDDM_GF_TH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_GF_TH.u32 = pstIspReg->ISP_NDDM_GF_TH.u32;
	ISP_NDDM_GF_TH.bits.gf_th_high = data;
	pstIspReg->ISP_NDDM_GF_TH.u32 = ISP_NDDM_GF_TH.u32;
}

#define HI_ISP_NDDM_GF_TH_LOW_DEFAULT (0)
static __inline HI_VOID hi_isp_nddm_gf_th_low_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_GF_TH ISP_NDDM_GF_TH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_GF_TH.u32 = pstIspReg->ISP_NDDM_GF_TH.u32;
	ISP_NDDM_GF_TH.bits.gf_th_low = data;
	pstIspReg->ISP_NDDM_GF_TH.u32 = ISP_NDDM_GF_TH.u32;
}

static __inline HI_VOID hi_isp_nddm_bldr_var_str_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_GF_BLDR ISP_NDDM_GF_BLDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_GF_BLDR.u32 = pstIspReg->ISP_NDDM_GF_BLDR.u32;
	ISP_NDDM_GF_BLDR.bits.bldr_var_str = data;
	pstIspReg->ISP_NDDM_GF_BLDR.u32 = ISP_NDDM_GF_BLDR.u32;
}

#define HI_ISP_NDDM_BLDR_CBCR_DEFAULT (8)
static __inline HI_VOID hi_isp_nddm_bldr_cbcr_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_GF_BLDR ISP_NDDM_GF_BLDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_GF_BLDR.u32 = pstIspReg->ISP_NDDM_GF_BLDR.u32;
	ISP_NDDM_GF_BLDR.bits.bldr_cbcr = data;
	pstIspReg->ISP_NDDM_GF_BLDR.u32 = ISP_NDDM_GF_BLDR.u32;
}

#define HI_ISP_NDDM_BLDR_GF_STR_DEFAULT (8)
static __inline HI_VOID hi_isp_nddm_bldr_gf_str_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_GF_BLDR ISP_NDDM_GF_BLDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_GF_BLDR.u32 = pstIspReg->ISP_NDDM_GF_BLDR.u32;
	ISP_NDDM_GF_BLDR.bits.bldr_gf_str = data;
	pstIspReg->ISP_NDDM_GF_BLDR.u32 = ISP_NDDM_GF_BLDR.u32;
}

#define HI_ISP_NDDM_CHRM_FIX_DEFAULT (4)
static __inline HI_VOID hi_isp_nddm_chrm_fix_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_GF_CHRM ISP_NDDM_GF_CHRM;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_GF_CHRM.u32 = pstIspReg->ISP_NDDM_GF_CHRM.u32;
	ISP_NDDM_GF_CHRM.bits.chrm_fix = data;
	pstIspReg->ISP_NDDM_GF_CHRM.u32 = ISP_NDDM_GF_CHRM.u32;
}

#define HI_ISP_NDDM_CHRM_HIGH_DEFAULT (190)
static __inline HI_VOID hi_isp_nddm_chrm_high_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_GF_CHRM ISP_NDDM_GF_CHRM;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_GF_CHRM.u32 = pstIspReg->ISP_NDDM_GF_CHRM.u32;
	ISP_NDDM_GF_CHRM.bits.chrm_high = data;
	pstIspReg->ISP_NDDM_GF_CHRM.u32 = ISP_NDDM_GF_CHRM.u32;
}

#define HI_ISP_NDDM_CHRM_LOW_DEFAULT (130)
static __inline HI_VOID hi_isp_nddm_chrm_low_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_GF_CHRM ISP_NDDM_GF_CHRM;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_GF_CHRM.u32 = pstIspReg->ISP_NDDM_GF_CHRM.u32;
	ISP_NDDM_GF_CHRM.bits.chrm_low = data;
	pstIspReg->ISP_NDDM_GF_CHRM.u32 = ISP_NDDM_GF_CHRM.u32;
}

static __inline HI_U8 hi_isp_nddm_multi_mf_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_USM_CFG ISP_NDDM_USM_CFG;

	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_USM_CFG.u32 = pstIspReg->ISP_NDDM_USM_CFG.u32;
	return (HI_U16)(ISP_NDDM_USM_CFG.bits.multi_mf);
}

#define HI_ISP_NDDM_MULTI_MF_DEFAULT (6)
static __inline HI_VOID hi_isp_nddm_multi_mf_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_USM_CFG ISP_NDDM_USM_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_USM_CFG.u32 = pstIspReg->ISP_NDDM_USM_CFG.u32;
	ISP_NDDM_USM_CFG.bits.multi_mf = data;
	pstIspReg->ISP_NDDM_USM_CFG.u32 = ISP_NDDM_USM_CFG.u32;
}

#define HI_ISP_NDDM_CLIP_USM_DEFAULT (256)
static __inline HI_VOID hi_isp_nddm_clip_usm_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_USM_CFG ISP_NDDM_USM_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_USM_CFG.u32 = pstIspReg->ISP_NDDM_USM_CFG.u32;
	ISP_NDDM_USM_CFG.bits.clip_usm = data;
	pstIspReg->ISP_NDDM_USM_CFG.u32 = ISP_NDDM_USM_CFG.u32;
}

#define HI_ISP_NDDM_SATU_TH_FIX_DEFAULT (164)
static __inline HI_VOID hi_isp_nddm_satu_th_fix_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_USM_SATU ISP_NDDM_USM_SATU;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_USM_SATU.u32 = pstIspReg->ISP_NDDM_USM_SATU.u32;
	ISP_NDDM_USM_SATU.bits.satu_th_fix = data;
	pstIspReg->ISP_NDDM_USM_SATU.u32 = ISP_NDDM_USM_SATU.u32;
}

#define HI_ISP_NDDM_SATU_TH_HIGH_DEFAULT (200)
static __inline HI_VOID hi_isp_nddm_satu_th_high_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_USM_SATU ISP_NDDM_USM_SATU;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_USM_SATU.u32 = pstIspReg->ISP_NDDM_USM_SATU.u32;
	ISP_NDDM_USM_SATU.bits.satu_th_high = data;
	pstIspReg->ISP_NDDM_USM_SATU.u32 = ISP_NDDM_USM_SATU.u32;
}

#define HI_ISP_NDDM_SATU_TH_LOW_DEFAULT (130)
static __inline HI_VOID hi_isp_nddm_satu_th_low_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_USM_SATU ISP_NDDM_USM_SATU;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_USM_SATU.u32 = pstIspReg->ISP_NDDM_USM_SATU.u32;
	ISP_NDDM_USM_SATU.bits.satu_th_low = data;
	pstIspReg->ISP_NDDM_USM_SATU.u32 = ISP_NDDM_USM_SATU.u32;
}

#define HI_ISP_NDDM_GRAY_THFIX_LOW_DEFAULT (32)
static __inline HI_VOID hi_isp_nddm_gray_th_fix_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_USM_GRAY ISP_NDDM_USM_GRAY;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_USM_GRAY.u32 = pstIspReg->ISP_NDDM_USM_GRAY.u32;
	ISP_NDDM_USM_GRAY.bits.gray_th_fix = data;
	pstIspReg->ISP_NDDM_USM_GRAY.u32 = ISP_NDDM_USM_GRAY.u32;
}

#define HI_ISP_NDDM_GRAY_TH_HIGH_DEFAULT (20)
static __inline HI_VOID hi_isp_nddm_gray_th_high_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_USM_GRAY ISP_NDDM_USM_GRAY;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_USM_GRAY.u32 = pstIspReg->ISP_NDDM_USM_GRAY.u32;
	ISP_NDDM_USM_GRAY.bits.gray_th_high = data;
	pstIspReg->ISP_NDDM_USM_GRAY.u32 = ISP_NDDM_USM_GRAY.u32;
}

#define HI_ISP_NDDM_GRAY_TH_LOW_DEFAULT (0)
static __inline HI_VOID hi_isp_nddm_gray_th_low_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_USM_GRAY ISP_NDDM_USM_GRAY;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_USM_GRAY.u32 = pstIspReg->ISP_NDDM_USM_GRAY.u32;
	ISP_NDDM_USM_GRAY.bits.gray_th_low = data;
	pstIspReg->ISP_NDDM_USM_GRAY.u32 = ISP_NDDM_USM_GRAY.u32;
}

#define HI_ISP_NDDM_BLDR_GRGB_DEFAULT (4)
static __inline HI_VOID hi_isp_nddm_bldr_gr_gb_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_APT_INTP_BLD ISP_NDDM_APT_INTP_BLD;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_APT_INTP_BLD.u32 = pstIspReg->ISP_NDDM_APT_INTP_BLD.u32;
	ISP_NDDM_APT_INTP_BLD.bits.bldr_gr_gb = data;
	pstIspReg->ISP_NDDM_APT_INTP_BLD.u32 = ISP_NDDM_APT_INTP_BLD.u32;
}

static __inline HI_VOID hi_isp_nddm_dith_mask_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_DITHER_CFG ISP_NDDM_DITHER_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_DITHER_CFG.u32 = pstIspReg->ISP_NDDM_DITHER_CFG.u32;
	ISP_NDDM_DITHER_CFG.bits.dith_mask = data;
	pstIspReg->ISP_NDDM_DITHER_CFG.u32 = ISP_NDDM_DITHER_CFG.u32;
}

static __inline HI_VOID hi_isp_nddm_dith_ratio_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_DITHER_CFG ISP_NDDM_DITHER_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_DITHER_CFG.u32 = pstIspReg->ISP_NDDM_DITHER_CFG.u32;
	ISP_NDDM_DITHER_CFG.bits.dith_ratio = data;
	pstIspReg->ISP_NDDM_DITHER_CFG.u32 = ISP_NDDM_DITHER_CFG.u32;
}

static __inline HI_VOID hi_isp_nddm_dith_max_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_DITHER_CFG ISP_NDDM_DITHER_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_DITHER_CFG.u32 = pstIspReg->ISP_NDDM_DITHER_CFG.u32;
	ISP_NDDM_DITHER_CFG.bits.dith_max = data;
	pstIspReg->ISP_NDDM_DITHER_CFG.u32 = ISP_NDDM_DITHER_CFG.u32;
}
#define HI_ISP_NDDM_CLIP_DELTA_INTP_LOW_DEFAULT (10)
static __inline HI_VOID hi_isp_nddm_clip_delta_intp_low_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_APT_AVG_CFG_0 ISP_NDDM_APT_AVG_CFG_0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_APT_AVG_CFG_0.u32 = pstIspReg->ISP_NDDM_APT_AVG_CFG_0.u32;
	ISP_NDDM_APT_AVG_CFG_0.bits.clip_delta_intp_low = data;
	pstIspReg->ISP_NDDM_APT_AVG_CFG_0.u32 = ISP_NDDM_APT_AVG_CFG_0.u32;
}

#define HI_ISP_NDDM_CLIP_DELTA_INTP_HIGH_DEFAULT (10)
static __inline HI_VOID hi_isp_nddm_clip_delta_intp_high_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_APT_AVG_CFG_0 ISP_NDDM_APT_AVG_CFG_0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_APT_AVG_CFG_0.u32 = pstIspReg->ISP_NDDM_APT_AVG_CFG_0.u32;
	ISP_NDDM_APT_AVG_CFG_0.bits.clip_delta_intp_high = data;
	pstIspReg->ISP_NDDM_APT_AVG_CFG_0.u32 = ISP_NDDM_APT_AVG_CFG_0.u32;
}

#define HI_ISP_NDDM_FILTER_STR_INTP_DEFAULT (16)
static __inline HI_VOID hi_isp_nddm_filter_str_intp_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_APT_AVG_CFG_0 ISP_NDDM_APT_AVG_CFG_0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_APT_AVG_CFG_0.u32 = pstIspReg->ISP_NDDM_APT_AVG_CFG_0.u32;
	ISP_NDDM_APT_AVG_CFG_0.bits.filter_str_intp = data;
	pstIspReg->ISP_NDDM_APT_AVG_CFG_0.u32 = ISP_NDDM_APT_AVG_CFG_0.u32;
}

#define HI_ISP_NDDM_CLIP_DELTA_FILT_LOW_DEFAULT (10)
static __inline HI_VOID hi_isp_nddm_clip_delta_filt_low_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_APT_AVG_CFG_1 ISP_NDDM_APT_AVG_CFG_1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_APT_AVG_CFG_1.u32 = pstIspReg->ISP_NDDM_APT_AVG_CFG_1.u32;
	ISP_NDDM_APT_AVG_CFG_1.bits.clip_delta_filt_low = data;
	pstIspReg->ISP_NDDM_APT_AVG_CFG_1.u32 = ISP_NDDM_APT_AVG_CFG_1.u32;
}

#define HI_ISP_NDDM_CLIP_DELTA_FILT_HIGH_DEFAULT (10)
static __inline HI_VOID hi_isp_nddm_clip_delta_filt_high_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_APT_AVG_CFG_1 ISP_NDDM_APT_AVG_CFG_1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_APT_AVG_CFG_1.u32 = pstIspReg->ISP_NDDM_APT_AVG_CFG_1.u32;
	ISP_NDDM_APT_AVG_CFG_1.bits.clip_delta_filt_high = data;
	pstIspReg->ISP_NDDM_APT_AVG_CFG_1.u32 = ISP_NDDM_APT_AVG_CFG_1.u32;
}

#define HI_ISP_NDDM_FILTER_STR_FILT_DEFAULT (16)
static __inline HI_VOID hi_isp_nddm_filter_str_filt_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_APT_AVG_CFG_1 ISP_NDDM_APT_AVG_CFG_1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_APT_AVG_CFG_1.u32 = pstIspReg->ISP_NDDM_APT_AVG_CFG_1.u32;
	ISP_NDDM_APT_AVG_CFG_1.bits.filter_str_filt = data;
	pstIspReg->ISP_NDDM_APT_AVG_CFG_1.u32 = ISP_NDDM_APT_AVG_CFG_1.u32;
}

#define HI_ISP_NDDM_VAR_OFFSET_GAIN_DEFAULT (32)
static __inline HI_VOID hi_isp_nddm_var_offset_gain_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_VAR_OFFSET_CFG ISP_NDDM_VAR_OFFSET_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_VAR_OFFSET_CFG.u32 = pstIspReg->ISP_NDDM_VAR_OFFSET_CFG.u32;
	ISP_NDDM_VAR_OFFSET_CFG.bits.var_offset_gain = data;
	pstIspReg->ISP_NDDM_VAR_OFFSET_CFG.u32 = ISP_NDDM_VAR_OFFSET_CFG.u32;
}

#define HI_ISP_NDDM_VAR_OFFSET_LOW_DEFAULT (300)
static __inline HI_VOID hi_isp_nddm_var_offset_low_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_VAR_OFFSET_CFG ISP_NDDM_VAR_OFFSET_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_VAR_OFFSET_CFG.u32 = pstIspReg->ISP_NDDM_VAR_OFFSET_CFG.u32;
	ISP_NDDM_VAR_OFFSET_CFG.bits.var_offset_low = data;
	pstIspReg->ISP_NDDM_VAR_OFFSET_CFG.u32 = ISP_NDDM_VAR_OFFSET_CFG.u32;
}

static __inline HI_VOID hi_isp_nddm_sht_ctrl_gain_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_SHT_CTRL ISP_NDDM_SHT_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_SHT_CTRL.u32 = pstIspReg->ISP_NDDM_SHT_CTRL.u32;
	ISP_NDDM_SHT_CTRL.bits.sht_ctrl_gain = data;
	pstIspReg->ISP_NDDM_SHT_CTRL.u32 = ISP_NDDM_SHT_CTRL.u32;
}
static __inline HI_VOID hi_isp_nddm_sht_ctrl_th_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_SHT_CTRL ISP_NDDM_SHT_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_SHT_CTRL.u32 = pstIspReg->ISP_NDDM_SHT_CTRL.u32;
	ISP_NDDM_SHT_CTRL.bits.sht_ctrl_th = data;
	pstIspReg->ISP_NDDM_SHT_CTRL.u32 = ISP_NDDM_SHT_CTRL.u32;
}

static __inline HI_VOID hi_isp_nddm_clip_r_ud_sht_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_SHT_CLIP_R ISP_NDDM_SHT_CLIP_R;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_SHT_CLIP_R.u32 = pstIspReg->ISP_NDDM_SHT_CLIP_R.u32;
	ISP_NDDM_SHT_CLIP_R.bits.clip_r_ud_sht = data;
	pstIspReg->ISP_NDDM_SHT_CLIP_R.u32 = ISP_NDDM_SHT_CLIP_R.u32;
}

static __inline HI_VOID hi_isp_nddm_clip_r_ov_sht_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_SHT_CLIP_R ISP_NDDM_SHT_CLIP_R;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_SHT_CLIP_R.u32 = pstIspReg->ISP_NDDM_SHT_CLIP_R.u32;
	ISP_NDDM_SHT_CLIP_R.bits.clip_r_ov_sht = data;
	pstIspReg->ISP_NDDM_SHT_CLIP_R.u32 = ISP_NDDM_SHT_CLIP_R.u32;
}

static __inline HI_VOID hi_isp_nddm_clip_b_ov_sht_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_SHT_CLIP_B ISP_NDDM_SHT_CLIP_B;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_SHT_CLIP_B.u32 = pstIspReg->ISP_NDDM_SHT_CLIP_B.u32;
	ISP_NDDM_SHT_CLIP_B.bits.clip_b_ov_sht = data;
	pstIspReg->ISP_NDDM_SHT_CLIP_B.u32 = ISP_NDDM_SHT_CLIP_B.u32;
}

static __inline HI_VOID hi_isp_nddm_clip_b_ud_sht_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_SHT_CLIP_B ISP_NDDM_SHT_CLIP_B;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_SHT_CLIP_B.u32 = pstIspReg->ISP_NDDM_SHT_CLIP_B.u32;
	ISP_NDDM_SHT_CLIP_B.bits.clip_b_ud_sht = data;
	pstIspReg->ISP_NDDM_SHT_CLIP_B.u32 = ISP_NDDM_SHT_CLIP_B.u32;
}

static __inline HI_VOID hi_isp_nddm_multi_mf_r_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_MULTI_MF_RB ISP_NDDM_MULTI_MF_RB;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_MULTI_MF_RB.u32 = pstIspReg->ISP_NDDM_MULTI_MF_RB.u32;
	ISP_NDDM_MULTI_MF_RB.bits.multi_mf_r = data;
	pstIspReg->ISP_NDDM_MULTI_MF_RB.u32 = ISP_NDDM_MULTI_MF_RB.u32;
}

static __inline HI_VOID hi_isp_nddm_multi_mf_b_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_MULTI_MF_RB ISP_NDDM_MULTI_MF_RB;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_MULTI_MF_RB.u32 = pstIspReg->ISP_NDDM_MULTI_MF_RB.u32;
	ISP_NDDM_MULTI_MF_RB.bits.multi_mf_b = data;
	pstIspReg->ISP_NDDM_MULTI_MF_RB.u32 = ISP_NDDM_MULTI_MF_RB.u32;
}
static __inline HI_VOID hi_isp_nddm_satu_r_th_low_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_USM_SATU_R ISP_NDDM_USM_SATU_R;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_USM_SATU_R.u32 = pstIspReg->ISP_NDDM_USM_SATU_R.u32;
	ISP_NDDM_USM_SATU_R.bits.satu_r_th_low = data;
	pstIspReg->ISP_NDDM_USM_SATU_R.u32 = ISP_NDDM_USM_SATU_R.u32;
}

static __inline HI_VOID hi_isp_nddm_satu_r_th_high_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_USM_SATU_R ISP_NDDM_USM_SATU_R;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_USM_SATU_R.u32 = pstIspReg->ISP_NDDM_USM_SATU_R.u32;
	ISP_NDDM_USM_SATU_R.bits.satu_r_th_high = data;
	pstIspReg->ISP_NDDM_USM_SATU_R.u32 = ISP_NDDM_USM_SATU_R.u32;
}

static __inline HI_VOID hi_isp_nddm_satu_r_th_fix_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_USM_SATU_R ISP_NDDM_USM_SATU_R;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_USM_SATU_R.u32 = pstIspReg->ISP_NDDM_USM_SATU_R.u32;
	ISP_NDDM_USM_SATU_R.bits.satu_r_th_fix = data;
	pstIspReg->ISP_NDDM_USM_SATU_R.u32 = ISP_NDDM_USM_SATU_R.u32;
}

static __inline HI_VOID hi_isp_nddm_satu_b_th_low_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_USM_SATU_B ISP_NDDM_USM_SATU_B;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_USM_SATU_B.u32 = pstIspReg->ISP_NDDM_USM_SATU_B.u32;
	ISP_NDDM_USM_SATU_B.bits.satu_b_th_low = data;
	pstIspReg->ISP_NDDM_USM_SATU_B.u32 = ISP_NDDM_USM_SATU_B.u32;
}

static __inline HI_VOID hi_isp_nddm_satu_b_th_high_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_USM_SATU_B ISP_NDDM_USM_SATU_B;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_USM_SATU_B.u32 = pstIspReg->ISP_NDDM_USM_SATU_B.u32;
	ISP_NDDM_USM_SATU_B.bits.satu_b_th_high = data;
	pstIspReg->ISP_NDDM_USM_SATU_B.u32 = ISP_NDDM_USM_SATU_B.u32;
}

static __inline HI_VOID hi_isp_nddm_satu_b_th_fix_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_USM_SATU_B ISP_NDDM_USM_SATU_B;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_USM_SATU_B.u32 = pstIspReg->ISP_NDDM_USM_SATU_B.u32;
	ISP_NDDM_USM_SATU_B.bits.satu_b_th_fix = data;
	pstIspReg->ISP_NDDM_USM_SATU_B.u32 = ISP_NDDM_USM_SATU_B.u32;
}

static __inline HI_VOID hi_isp_nddm_satu_fix_ehcy_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_SATU_FIX_EHCY ISP_NDDM_SATU_FIX_EHCY;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_SATU_FIX_EHCY.u32 = pstIspReg->ISP_NDDM_SATU_FIX_EHCY.u32;
	ISP_NDDM_SATU_FIX_EHCY.bits.satu_fix_ehcy = data;
	pstIspReg->ISP_NDDM_SATU_FIX_EHCY.u32 = ISP_NDDM_SATU_FIX_EHCY.u32;
}


#define HI_ISP_NDDM_FCR_LIMIT_HIGH_DEFAULT (256)
static __inline HI_VOID hi_isp_nddm_fcr_limit_high_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_FCR_LIMIT ISP_NDDM_FCR_LIMIT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_FCR_LIMIT.u32 = pstIspReg->ISP_NDDM_FCR_LIMIT.u32;
	ISP_NDDM_FCR_LIMIT.bits.fcr_limit_high = data;
	pstIspReg->ISP_NDDM_FCR_LIMIT.u32 = ISP_NDDM_FCR_LIMIT.u32;
}

#define HI_ISP_NDDM_FCR_LIMIT_LOW_DEFAULT (128)
static __inline HI_VOID hi_isp_nddm_fcr_limit_low_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_FCR_LIMIT ISP_NDDM_FCR_LIMIT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_FCR_LIMIT.u32 = pstIspReg->ISP_NDDM_FCR_LIMIT.u32;
	ISP_NDDM_FCR_LIMIT.bits.fcr_limit_low = data;
	pstIspReg->ISP_NDDM_FCR_LIMIT.u32 = ISP_NDDM_FCR_LIMIT.u32;
}

#define HI_ISP_NDDM_FCR_DET_LOW_DEFAULT (150)
static __inline HI_VOID hi_isp_nddm_fcr_det_low_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_FCR_GAIN ISP_NDDM_FCR_GAIN;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_FCR_GAIN.u32 = pstIspReg->ISP_NDDM_FCR_GAIN.u32;
	ISP_NDDM_FCR_GAIN.bits.fcr_det_low = data;
	pstIspReg->ISP_NDDM_FCR_GAIN.u32 = ISP_NDDM_FCR_GAIN.u32;
}

#define HI_ISP_NDDM_FCR_GF_GAIN_DEFAULT (4)
static __inline HI_VOID hi_isp_nddm_fcr_gf_gain_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_FCR_GAIN ISP_NDDM_FCR_GAIN;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_FCR_GAIN.u32 = pstIspReg->ISP_NDDM_FCR_GAIN.u32;
	ISP_NDDM_FCR_GAIN.bits.fcr_gf_gain = data;
	pstIspReg->ISP_NDDM_FCR_GAIN.u32 = ISP_NDDM_FCR_GAIN.u32;
}

#define HI_ISP_NDDM_FCR_SCALE_DEFAULT (2)
static __inline HI_VOID hi_isp_nddm_scale_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_FCR_SCALE ISP_NDDM_FCR_SCALE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_FCR_SCALE.u32 = pstIspReg->ISP_NDDM_FCR_SCALE.u32;
	ISP_NDDM_FCR_SCALE.bits.scale = data;
	pstIspReg->ISP_NDDM_FCR_SCALE.u32 = ISP_NDDM_FCR_SCALE.u32;
}

#define HI_ISP_NDDM_CACBLENDRATE_DEFAULT (256)
static __inline HI_VOID hi_isp_nddm_cac_blend_rate_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_DM_BLDRATE ISP_NDDM_DM_BLDRATE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_DM_BLDRATE.u32 = pstIspReg->ISP_NDDM_DM_BLDRATE.u32;
	ISP_NDDM_DM_BLDRATE.bits.cac_blend_rate = data;
	pstIspReg->ISP_NDDM_DM_BLDRATE.u32 = ISP_NDDM_DM_BLDRATE.u32;
}

static __inline HI_VOID hi_isp_nddm_gf_lut_update_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_GF_LUT_UPDATE ISP_NDDM_GF_LUT_UPDATE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_GF_LUT_UPDATE.u32 = pstIspReg->ISP_NDDM_GF_LUT_UPDATE.u32;
	ISP_NDDM_GF_LUT_UPDATE.bits.update = data;
	pstIspReg->ISP_NDDM_GF_LUT_UPDATE.u32 = ISP_NDDM_GF_LUT_UPDATE.u32;
}

static __inline HI_VOID hi_isp_nddm_usm_lut_update_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_USM_LUT_UPDATE ISP_NDDM_USM_LUT_UPDATE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_USM_LUT_UPDATE.u32 = pstIspReg->ISP_NDDM_USM_LUT_UPDATE.u32;
	ISP_NDDM_USM_LUT_UPDATE.bits.update = data;
	pstIspReg->ISP_NDDM_USM_LUT_UPDATE.u32 = ISP_NDDM_USM_LUT_UPDATE.u32;
}

#define HI_ISP_NDDM_GRAY_THFIX_HIGH_DEFAULT (256)
static __inline HI_VOID hi_isp_nddm_gray_th_fix2_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_USM_GRAY_TH_FIX2 ISP_NDDM_USM_GRAY_TH_FIX2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_USM_GRAY_TH_FIX2.u32 = pstIspReg->ISP_NDDM_USM_GRAY_TH_FIX2.u32;
	ISP_NDDM_USM_GRAY_TH_FIX2.bits.gray_th_fix2 = data;
	pstIspReg->ISP_NDDM_USM_GRAY_TH_FIX2.u32 = ISP_NDDM_USM_GRAY_TH_FIX2.u32;
}

#define HI_ISP_NDDM_CLIP_ADJUST_MAX_DEFAULT (40)
static __inline HI_VOID hi_isp_nddm_clip_adjust_max_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_APT_INTP_CLIP2 ISP_NDDM_APT_INTP_CLIP2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_APT_INTP_CLIP2.u32 = pstIspReg->ISP_NDDM_APT_INTP_CLIP2.u32;
	ISP_NDDM_APT_INTP_CLIP2.bits.clip_adjust_max = data;
	pstIspReg->ISP_NDDM_APT_INTP_CLIP2.u32 = ISP_NDDM_APT_INTP_CLIP2.u32;
}

#define HI_ISP_NDDM_CLIP_DELTA_GAIN_DEFAULT (2)
static __inline HI_VOID hi_isp_nddm_clip_delta_gain_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_APT_INTP_CLIP2 ISP_NDDM_APT_INTP_CLIP2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_APT_INTP_CLIP2.u32 = pstIspReg->ISP_NDDM_APT_INTP_CLIP2.u32;
	ISP_NDDM_APT_INTP_CLIP2.bits.clip_delta_gain = data;
	pstIspReg->ISP_NDDM_APT_INTP_CLIP2.u32 = ISP_NDDM_APT_INTP_CLIP2.u32;
}
static __inline HI_U8 hi_isp_nddm_clip_delta_gain_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	return pstIspReg->ISP_NDDM_APT_INTP_CLIP2.bits.clip_delta_gain ;
}

static __inline HI_VOID hi_isp_nddm_gf_lut_waddr_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_GF_LUT_WADDR ISP_NDDM_GF_LUT_WADDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_GF_LUT_WADDR.u32 = pstIspReg->ISP_NDDM_GF_LUT_WADDR.u32;
	ISP_NDDM_GF_LUT_WADDR.bits.gf_lut_waddr = data;
	pstIspReg->ISP_NDDM_GF_LUT_WADDR.u32 = ISP_NDDM_GF_LUT_WADDR.u32;
}

static __inline HI_VOID hi_isp_nddm_gf_lut_wdata_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_GF_LUT_WDATA ISP_NDDM_GF_LUT_WDATA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_GF_LUT_WDATA.u32 = pstIspReg->ISP_NDDM_GF_LUT_WDATA.u32;
	ISP_NDDM_GF_LUT_WDATA.bits.gf_lut_wdata = data;
	pstIspReg->ISP_NDDM_GF_LUT_WDATA.u32 = ISP_NDDM_GF_LUT_WDATA.u32;
}

static __inline HI_VOID hi_isp_nddm_usm_lut_waddr_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_USM_LUT_WADDR ISP_NDDM_USM_LUT_WADDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_USM_LUT_WADDR.u32 = pstIspReg->ISP_NDDM_USM_LUT_WADDR.u32;
	ISP_NDDM_USM_LUT_WADDR.bits.usm_lut_waddr = data;
	pstIspReg->ISP_NDDM_USM_LUT_WADDR.u32 = ISP_NDDM_USM_LUT_WADDR.u32;
}

static __inline HI_VOID hi_isp_nddm_usm_lut_wdata_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_USM_LUT_WDATA ISP_NDDM_USM_LUT_WDATA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_USM_LUT_WDATA.u32 = pstIspReg->ISP_NDDM_USM_LUT_WDATA.u32;
	ISP_NDDM_USM_LUT_WDATA.bits.usm_lut_wdata = data;
	pstIspReg->ISP_NDDM_USM_LUT_WDATA.u32 = ISP_NDDM_USM_LUT_WDATA.u32;
}

static __inline HI_VOID hi_isp_nddm_width_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_SIZE ISP_NDDM_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_SIZE.u32 = pstIspReg->ISP_NDDM_SIZE.u32;
	ISP_NDDM_SIZE.bits.width = data;
	pstIspReg->ISP_NDDM_SIZE.u32 = ISP_NDDM_SIZE.u32;
}

static __inline HI_VOID hi_isp_nddm_height_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_NDDM_SIZE ISP_NDDM_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_NDDM_SIZE.u32 = pstIspReg->ISP_NDDM_SIZE.u32;
	ISP_NDDM_SIZE.bits.height = data;
	pstIspReg->ISP_NDDM_SIZE.u32 = ISP_NDDM_SIZE.u32;
}

/*****************************************************
MODULE        -----UVNR-----
MODULE        -----UVNR-----
MODULE        -----UVNR-----
*****************************************************/
/*****************************************************
ISP_UVNR_CFG:          ISP_UVNR_CFG?        UVNR
0x205c1800: 		   31:13	      reserved
					   //12:12      uvnr_sel     
					   //11:09      reserved
					   //08:08      nr_en         
					   //07:05      reserved
					   04:04      midf_en         
					   03:03      uv2c_mode     
					   02:02      uv2c_en         
					   01:01      vcds_en         
					   00:00      hcds_en         
*****************************************************/
static __inline HI_VOID hi_isp_midf_en_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_MCDS_CFG ISP_MCDS_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_MCDS_CFG.u32 = pstIspReg->ISP_MCDS_CFG.u32;
	ISP_MCDS_CFG.bits.midf_en = data;
	pstIspReg->ISP_MCDS_CFG.u32 = ISP_MCDS_CFG.u32;
}

static __inline HI_VOID hi_isp_uv2c_mode_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_MCDS_CFG ISP_MCDS_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_MCDS_CFG.u32 = pstIspReg->ISP_MCDS_CFG.u32;
	ISP_MCDS_CFG.bits.uv2c_mode = data;
	pstIspReg->ISP_MCDS_CFG.u32 = ISP_MCDS_CFG.u32;
}

static __inline HI_VOID hi_isp_vcds_en_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_MCDS_CFG ISP_MCDS_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_MCDS_CFG.u32 = pstIspReg->ISP_MCDS_CFG.u32;
	ISP_MCDS_CFG.bits.vcds_en = data;
	pstIspReg->ISP_MCDS_CFG.u32 = ISP_MCDS_CFG.u32;
}

static __inline HI_VOID hi_isp_hcds_en_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_MCDS_CFG ISP_MCDS_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_MCDS_CFG.u32 = pstIspReg->ISP_MCDS_CFG.u32;
	ISP_MCDS_CFG.bits.hcds_en = data;
	pstIspReg->ISP_MCDS_CFG.u32 = ISP_MCDS_CFG.u32;
}

/*****************************************************
ISP_HCDS_SIZE:       ISP_HCD_SSIZE
0x205a1614:          31:29   reserved
                                 28:16   width_out  
                                 15:13   reserved
                                 12:00   width_in     
*****************************************************/
static __inline HI_VOID hi_isp_hcds_width_out_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_HCDS_SIZE ISP_HCDS_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_HCDS_SIZE.u32 = pstIspReg->ISP_HCDS_SIZE.u32;
	ISP_HCDS_SIZE.bits.width_out = data;
	pstIspReg->ISP_HCDS_SIZE.u32 = ISP_HCDS_SIZE.u32;
}

static __inline HI_VOID hi_isp_hcds_width_in_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_HCDS_SIZE ISP_HCDS_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_HCDS_SIZE.u32 = pstIspReg->ISP_HCDS_SIZE.u32;
	ISP_HCDS_SIZE.bits.width_in = data;
	pstIspReg->ISP_HCDS_SIZE.u32 = ISP_HCDS_SIZE.u32;
}

/*****************************************************
ISP_HCDS_COEF0:      ISP_HCDS_COEF0
0x205a161c:              31:30   reserved
                                  29:20   coef2  
                                  19:10   coef1 
                                  09:00   coef0  
*****************************************************/
static __inline HI_VOID hi_isp_hcds_coef2_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_HCDS_COEF0 ISP_HCDS_COEF0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_HCDS_COEF0.u32 = pstIspReg->ISP_HCDS_COEF0.u32;
	ISP_HCDS_COEF0.bits.coef2 = data;
	pstIspReg->ISP_HCDS_COEF0.u32 = ISP_HCDS_COEF0.u32;
}

static __inline HI_VOID hi_isp_hcds_coef1_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_HCDS_COEF0 ISP_HCDS_COEF0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_HCDS_COEF0.u32 = pstIspReg->ISP_HCDS_COEF0.u32;
	ISP_HCDS_COEF0.bits.coef1 = data;
	pstIspReg->ISP_HCDS_COEF0.u32 = ISP_HCDS_COEF0.u32;
}

static __inline HI_VOID hi_isp_hcds_coef0_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_HCDS_COEF0 ISP_HCDS_COEF0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_HCDS_COEF0.u32 = pstIspReg->ISP_HCDS_COEF0.u32;
	ISP_HCDS_COEF0.bits.coef0 = data;
	pstIspReg->ISP_HCDS_COEF0.u32 = ISP_HCDS_COEF0.u32;
}

/*****************************************************
ISP_HCDS_COEF1:     ISP_HCDS_COEF0
0x205a1620:             31:30   reserved
                                  29:20   coef5  
                                  19:10   coef4 
                                  09:00   coef3  
*****************************************************/
static __inline HI_VOID hi_isp_hcds_coef5_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_HCDS_COEF1 ISP_HCDS_COEF1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_HCDS_COEF1.u32 = pstIspReg->ISP_HCDS_COEF1.u32;
	ISP_HCDS_COEF1.bits.coef5 = data;
	pstIspReg->ISP_HCDS_COEF1.u32 = ISP_HCDS_COEF1.u32;
}

static __inline HI_VOID hi_isp_hcds_coef4_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_HCDS_COEF1 ISP_HCDS_COEF1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_HCDS_COEF1.u32 = pstIspReg->ISP_HCDS_COEF1.u32;
	ISP_HCDS_COEF1.bits.coef4 = data;
	pstIspReg->ISP_HCDS_COEF1.u32 = ISP_HCDS_COEF1.u32;
}

static __inline HI_VOID hi_isp_hcds_coef3_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_HCDS_COEF1 ISP_HCDS_COEF1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_HCDS_COEF1.u32 = pstIspReg->ISP_HCDS_COEF1.u32;
	ISP_HCDS_COEF1.bits.coef3 = data;
	pstIspReg->ISP_HCDS_COEF1.u32 = ISP_HCDS_COEF1.u32;
}


/*****************************************************
ISP_HCDS_COEF1:     ISP_HCDS_COEF0
0x205a1624:             31:20   reserved
                                  19:10   coef7  
                                  09:00   coef6 
*****************************************************/
static __inline HI_VOID hi_isp_hcds_coef7_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_HCDS_COEF2 ISP_HCDS_COEF2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_HCDS_COEF2.u32 = pstIspReg->ISP_HCDS_COEF2.u32;
	ISP_HCDS_COEF2.bits.coef7 = data;
	pstIspReg->ISP_HCDS_COEF2.u32 = ISP_HCDS_COEF2.u32;
}

static __inline HI_VOID hi_isp_hcds_coef6_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_HCDS_COEF2 ISP_HCDS_COEF2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_HCDS_COEF2.u32 = pstIspReg->ISP_HCDS_COEF2.u32;
	ISP_HCDS_COEF2.bits.coef6 = data;
	pstIspReg->ISP_HCDS_COEF2.u32 = ISP_HCDS_COEF2.u32;
}

/*****************************************************
ISP_VCDS_COEF:   
0x205a1628:            31:27   reserved
                                 26:24    coef3,same as coef0
                                 23:19   reserved
                                 18:16   coef2,same as coef1
                                 15:11   reserved
                                 10:08   coef1
                                 07:03   reserved
                                 02:00   coef0
*****************************************************/
static __inline HI_VOID hi_isp_vcds_coef3_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_VCDS_COEF ISP_VCDS_COEF;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_VCDS_COEF.u32 = pstIspReg->ISP_VCDS_COEF.u32;
	ISP_VCDS_COEF.bits.coef3 = data;
	pstIspReg->ISP_VCDS_COEF.u32 = ISP_VCDS_COEF.u32;
}

static __inline HI_VOID hi_isp_vcds_coef2_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_VCDS_COEF ISP_VCDS_COEF;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_VCDS_COEF.u32 = pstIspReg->ISP_VCDS_COEF.u32;
	ISP_VCDS_COEF.bits.coef2 = data;
	pstIspReg->ISP_VCDS_COEF.u32 = ISP_VCDS_COEF.u32;
}

static __inline HI_VOID hi_isp_vcds_coef1_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_VCDS_COEF ISP_VCDS_COEF;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_VCDS_COEF.u32 = pstIspReg->ISP_VCDS_COEF.u32;
	ISP_VCDS_COEF.bits.coef1 = data;
	pstIspReg->ISP_VCDS_COEF.u32 = ISP_VCDS_COEF.u32;
}

static __inline HI_VOID hi_isp_vcds_coef0_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_VCDS_COEF ISP_VCDS_COEF;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_VCDS_COEF.u32 = pstIspReg->ISP_VCDS_COEF.u32;
	ISP_VCDS_COEF.bits.coef0 = data;
	pstIspReg->ISP_VCDS_COEF.u32 = ISP_VCDS_COEF.u32;
}

/*****************************************************
ISP_MIDFILTER_SIZE:     
0x205a162c:                      31:29   reserved
						    28:16   height
                                          15:13   reserved
                                          12:00   width
*****************************************************/
static __inline HI_VOID hi_isp_midfilter_width_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_MIDF_SIZE ISP_MIDF_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_MIDF_SIZE.u32 = pstIspReg->ISP_MIDF_SIZE.u32;
	ISP_MIDF_SIZE.bits.width = data;
	pstIspReg->ISP_MIDF_SIZE.u32 = ISP_MIDF_SIZE.u32;
}

static __inline HI_VOID hi_isp_midfilter_height_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_MIDF_SIZE ISP_MIDF_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_MIDF_SIZE.u32 = pstIspReg->ISP_MIDF_SIZE.u32;
	ISP_MIDF_SIZE.bits.height = data;
	pstIspReg->ISP_MIDF_SIZE.u32 = ISP_MIDF_SIZE.u32;
}

/*****************************************************
ISP_UVNR_PARA:       NR
0x205a1644:              31:29   reserved
					28:24   midf_bldr   Range:0-16
					23   	reserved   Range: 0-63
					22:16   limit          Range: 0-127
					15:00   reserved
*****************************************************/
static __inline HI_VOID hi_isp_mcds_midf_bldr_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_MCDS_PARA ISP_MCDS_PARA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_MCDS_PARA.u32 = pstIspReg->ISP_MCDS_PARA.u32;
	ISP_MCDS_PARA.bits.midf_bldr = data;
	pstIspReg->ISP_MCDS_PARA.u32 = ISP_MCDS_PARA.u32;
}

static __inline HI_VOID hi_isp_mcds_limit_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_MCDS_PARA ISP_MCDS_PARA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_MCDS_PARA.u32 = pstIspReg->ISP_MCDS_PARA.u32;
	ISP_MCDS_PARA.bits.limit = data;
	pstIspReg->ISP_MCDS_PARA.u32 = ISP_MCDS_PARA.u32;
}

#define HI_ISP_MIDFILTER_TIMING_DEFAULT (0x20)
/*****************************************************
ISP_MIDFILTER_TIMING:       MIDFILTER   
0x205a18e8:                 31:14   reserved
                            13:01   cpi_fix_timing_stat 
                            00:00   cpi_auto_timing_en
*****************************************************/
static __inline HI_VOID hi_isp_midfilter_timing_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_MCDS_TIMING ISP_MCDS_TIMING;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_MCDS_TIMING.u32 = pstIspReg->ISP_MCDS_TIMING.u32;
	ISP_MCDS_TIMING.bits.cpi_fix_timing_stat = data;
	pstIspReg->ISP_MCDS_TIMING.u32 = ISP_MCDS_TIMING.u32;
}

/*****************************************************
ISP_MCDS_SIZE:      MCDS        
0x113c18f0:         31:29   reserved
					28:16   height
					15:13   reserved
					12:00   width
*****************************************************/
static __inline HI_VOID hi_isp_mcds_width_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_MCDS_SIZE ISP_MCDS_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_MCDS_SIZE.u32 = pstIspReg->ISP_MCDS_SIZE.u32;
	ISP_MCDS_SIZE.bits.width = data;
	pstIspReg->ISP_MCDS_SIZE.u32 = ISP_MCDS_SIZE.u32;
}

static __inline HI_VOID hi_isp_mcds_height_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_MCDS_SIZE ISP_MCDS_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_MCDS_SIZE.u32 = pstIspReg->ISP_MCDS_SIZE.u32;
	ISP_MCDS_SIZE.bits.height = data;
	pstIspReg->ISP_MCDS_SIZE.u32 = ISP_MCDS_SIZE.u32;
}

/*****************************************************
MODULE        -----LSC-----
MODULE        -----LSC-----
MODULE        -----LSC-----
*****************************************************/
/*****************************************************
ISP_LSC_CFG:              
0x205a3000:                     31:1      reserved
                                          0      lscenable
*****************************************************/
static __inline HI_VOID hi_isp_lsc_cfg_enable_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_LSC_CFG ISP_LSC_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_LSC_CFG.u32 = pstIspReg->ISP_LSC_CFG.u32;
	ISP_LSC_CFG.bits.en = data;
	pstIspReg->ISP_LSC_CFG.u32 = ISP_LSC_CFG.u32;
}

static __inline HI_U8 hi_isp_lsc_cfg_enable_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_LSC_CFG ISP_LSC_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_LSC_CFG.u32 = pstIspReg->ISP_LSC_CFG.u32;
	return ISP_LSC_CFG.bits.en;
}

/*****************************************************
ISP_LSC_WINNUM:      
0x205a3010:             31:21		reserved                                        
                        20:16		numv
                        15:5		reserved
                        4:0			numh			
*****************************************************/
static __inline HI_VOID hi_isp_lsc_win_num_write(HI_U8 ispDev, HI_U8 numh, HI_U8 numv){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_LSC_WINNUM ISP_LSC_WINNUM;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_LSC_WINNUM.u32 = pstIspReg->ISP_LSC_WINNUM.u32;
	ISP_LSC_WINNUM.bits.numh = numh;	
	pstIspReg->ISP_LSC_WINNUM.u32 = ISP_LSC_WINNUM.u32;
	ISP_LSC_WINNUM.bits.numv = numv;
	pstIspReg->ISP_LSC_WINNUM.u32 = ISP_LSC_WINNUM.u32;
}

/*****************************************************
ISP_LSC_WINX:         LSC
0x205a3014~0x205a3030       31:28		reserved                                         
                    		27:16		invx1
                      		15:10		reserved
                    		9:0			deltax1			
*****************************************************/
static __inline HI_VOID hi_isp_lsc_hwin_info_write(HI_U8 ispDev, HI_U8 index, HI_U16 delta, HI_U16 inv){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_LSC_WINX ISP_LSC_WINX;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_LSC_WINX.u32 = pstIspReg->ISP_LSC_WINX[index].u32;
	ISP_LSC_WINX.bits.deltax = delta;	
	pstIspReg->ISP_LSC_WINX[index].u32 = ISP_LSC_WINX.u32;
	ISP_LSC_WINX.bits.invx   = inv;
	pstIspReg->ISP_LSC_WINX[index].u32 = ISP_LSC_WINX.u32;
}

/*****************************************************
ISP_LSC_WINX:     
0x205a3034~0x205a3050                   31:28		reserved                                         
                                		27:16		invy1
			                      		15:10		reserved
                                		9:0			deltay1		
*****************************************************/
static __inline HI_VOID hi_isp_lsc_vwin_info_write(HI_U8 ispDev, HI_U8 index, HI_U16 delta, HI_U16 inv){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_LSC_WINY ISP_LSC_WINY;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_LSC_WINY.u32 = pstIspReg->ISP_LSC_WINY[index].u32;
	ISP_LSC_WINY.bits.deltay = delta;	
	pstIspReg->ISP_LSC_WINY[index].u32 = ISP_LSC_WINY.u32;
	ISP_LSC_WINY.bits.invy   = inv;
	pstIspReg->ISP_LSC_WINY[index].u32 = ISP_LSC_WINY.u32;
}

/*****************************************************
ISP_LSC_BLCEN:         LSC BLC offset enable  
0x113a3054:            31:00		rgain_waddr                                         
                       08:08	    blc_out_en
                       00:00	    blc_in_en
*****************************************************/
static __inline HI_VOID hi_isp_lsc_blc_in_enable_write(HI_U8 ispDev, HI_U8 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_LSC_BLCEN ISP_LSC_BLCEN;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_LSC_BLCEN.u32 = pstIspReg->ISP_LSC_BLCEN.u32;
	ISP_LSC_BLCEN.bits.blc_in_en = data;	
	pstIspReg->ISP_LSC_BLCEN.u32 = ISP_LSC_BLCEN.u32;
}

static __inline HI_VOID hi_isp_lsc_blc_out_enable_write(HI_U8 ispDev, HI_U8 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_LSC_BLCEN ISP_LSC_BLCEN;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_LSC_BLCEN.u32 = pstIspReg->ISP_LSC_BLCEN.u32;
	ISP_LSC_BLCEN.bits.blc_out_en = data;	
	pstIspReg->ISP_LSC_BLCEN.u32 = ISP_LSC_BLCEN.u32;
}

/*****************************************************
ISP_LSC_RBLC:         LSC BLC offset enable  
0x113a3058:            31:15		reserved                                         
                       14:00	    blc_r
*****************************************************/
static __inline HI_VOID hi_isp_lsc_blc_r_write(HI_U8 ispDev, HI_U16 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_LSC_RBLC ISP_LSC_RBLC;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_LSC_RBLC.u32 = pstIspReg->ISP_LSC_RBLC.u32;
	ISP_LSC_RBLC.bits.blc_r = data;
	pstIspReg->ISP_LSC_RBLC.u32 = ISP_LSC_RBLC.u32;
}

/*****************************************************
ISP_LSC_GRBLC:         LSC BLC offset enable  
0x113a305c:            31:15		reserved                                         
                       14:00	    blc_gr
*****************************************************/
static __inline HI_VOID hi_isp_lsc_blc_gr_write(HI_U8 ispDev, HI_U16 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_LSC_GRBLC ISP_LSC_GRBLC;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_LSC_GRBLC.u32 = pstIspReg->ISP_LSC_GRBLC.u32;
	ISP_LSC_GRBLC.bits.blc_gr = data;
	pstIspReg->ISP_LSC_GRBLC.u32 = ISP_LSC_GRBLC.u32;
}

/*****************************************************
ISP_LSC_GBBLC:         LSC BLC offset enable  
0x113a3060:            31:15		reserved                                         
                       14:00	    blc_gb
*****************************************************/
static __inline HI_VOID hi_isp_lsc_blc_gb_write(HI_U8 ispDev, HI_U16 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_LSC_GBBLC ISP_LSC_GBBLC;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_LSC_GBBLC.u32 = pstIspReg->ISP_LSC_GBBLC.u32;
	ISP_LSC_GBBLC.bits.blc_gb = data;
	pstIspReg->ISP_LSC_GBBLC.u32 = ISP_LSC_GBBLC.u32;
}

/*****************************************************
ISP_LSC_BBLC:         LSC BLC offset enable  
0x113a3064:            31:15		reserved                                         
                       14:00	    blc_b
*****************************************************/
static __inline HI_VOID hi_isp_lsc_blc_b_write(HI_U8 ispDev, HI_U16 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_LSC_BBLC ISP_LSC_BBLC;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_LSC_BBLC.u32 = pstIspReg->ISP_LSC_BBLC.u32;
	ISP_LSC_BBLC.bits.blc_b = data;
	pstIspReg->ISP_LSC_BBLC.u32 = ISP_LSC_BBLC.u32;
}

/*****************************************************
ISP_LSC_LUT_UPDATE:       
0x113a3068:     31:01	    reserved                                         
                       00	    lut_update
*****************************************************/
static __inline HI_VOID hi_isp_lsc_lut_update_write(HI_U8 ispDev, HI_U8 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_LSC_LUT_UPDATE ISP_LSC_LUT_UPDATE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_LSC_LUT_UPDATE.u32 = pstIspReg->ISP_LSC_LUT_UPDATE.u32;
	ISP_LSC_LUT_UPDATE.bits.lut_update = data;
	pstIspReg->ISP_LSC_LUT_UPDATE.u32 = ISP_LSC_LUT_UPDATE.u32;
}


/*****************************************************
ISP_LSC_GRRGAIN_WADDR:      
0x205a3080:                    	31:0		rgain_waddr                                         
                      	
*****************************************************/
static __inline HI_VOID hi_isp_lsc_grr_gain_waddr_write(HI_U8 ispDev, HI_U32 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_LSC_GRRGAIN_WADDR ISP_LSC_GRRGAIN_WADDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_LSC_GRRGAIN_WADDR.u32 = pstIspReg->ISP_LSC_GRRGAIN_WADDR.u32;
	ISP_LSC_GRRGAIN_WADDR.bits.rgain_waddr = data;
	pstIspReg->ISP_LSC_GRRGAIN_WADDR.u32 = ISP_LSC_GRRGAIN_WADDR.u32;
}

/*****************************************************
ISP_LSC_GRRGAIN_WDATA:        
0x205a3084:                    	31:0		rgain_wdata                                        
                      	
*****************************************************/
static __inline HI_VOID hi_isp_lsc_grr_gain_wdata_write(HI_U8 ispDev, HI_U32 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_LSC_GRRGAIN_WDATA ISP_LSC_GRRGAIN_WDATA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_LSC_GRRGAIN_WDATA.u32 = pstIspReg->ISP_LSC_GRRGAIN_WDATA.u32;
	ISP_LSC_GRRGAIN_WDATA.bits.rgain_wdata = data;
	pstIspReg->ISP_LSC_GRRGAIN_WDATA.u32 = ISP_LSC_GRRGAIN_WDATA.u32;
}

/*****************************************************
ISP_LSC_GBBGAIN_WADDR:        
0x205a5890:                    	31:0		grgain_waddr                                       
                      	
*****************************************************/
static __inline HI_VOID hi_isp_lsc_gbb_gain_waddr_write(HI_U8 ispDev, HI_U32 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_LSC_GBBGAIN_WADDR ISP_LSC_GBBGAIN_WADDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_LSC_GBBGAIN_WADDR.u32 = pstIspReg->ISP_LSC_GBBGAIN_WADDR.u32;
	ISP_LSC_GBBGAIN_WADDR.bits.grgain_waddr = data;
	pstIspReg->ISP_LSC_GBBGAIN_WADDR.u32 = ISP_LSC_GBBGAIN_WADDR.u32;
}

/*****************************************************
ISP_LSC_GBBGAIN_WDATA:      
0x205a5894:                    	31:0		grgain_wdata                                      
                      	
*****************************************************/
static __inline HI_VOID hi_isp_lsc_gbb_gain_wdata_write(HI_U8 ispDev, HI_U32 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_LSC_GBBGAIN_WDATA ISP_LSC_GBBGAIN_WDATA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_LSC_GBBGAIN_WDATA.u32 = pstIspReg->ISP_LSC_GBBGAIN_WDATA.u32;
	ISP_LSC_GBBGAIN_WDATA.bits.grgain_wdata = data;
	pstIspReg->ISP_LSC_GBBGAIN_WDATA.u32 = ISP_LSC_GBBGAIN_WDATA.u32;
}

/*****************************************************
ISP_LSC_CTRL_F:       
0x205a30e0:                    	31:2		greserved                                    
                      			1:0			rggb_cfg
*****************************************************/
static __inline HI_VOID hi_isp_lsc_rggb_cfg_write(HI_U8 ispDev, HI_U8 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_LSC_CTRL_F ISP_LSC_CTRL_F;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_LSC_CTRL_F.u32 = pstIspReg->ISP_LSC_CTRL_F.u32;
	ISP_LSC_CTRL_F.bits.rggb_cfg = data;
	pstIspReg->ISP_LSC_CTRL_F.u32 = ISP_LSC_CTRL_F.u32;
}

/*****************************************************
MODULE        -----ACM-----
MODULE        -----ACM-----
MODULE        -----ACM-----
*****************************************************/
/*****************************************************
ISP_ACM_CTRL:         			ACM        
0x205c1400:                    	31		acm_en
						30		acm_dbg_en
						29		acm_stretch
						28		acm_cliprange
						27		acm_cliporwrap
					26:23	reserved
						22:10	acm_dbg_pos
						9		acm_dbg_mode
                      			8:0		acm_cbcrthr
*****************************************************/
static __inline HI_VOID hi_isp_acm_enable_write(HI_U8 ispDev, HI_U8 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_ACM_CTRL ISP_ACM_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_ACM_CTRL.u32 = pstIspReg->ISP_ACM_CTRL.u32;
	ISP_ACM_CTRL.bits.acm_en = data;
	pstIspReg->ISP_ACM_CTRL.u32 = ISP_ACM_CTRL.u32;
}

static __inline HI_U8 hi_isp_acm_enable_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_ACM_CTRL ISP_ACM_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_ACM_CTRL.u32 = pstIspReg->ISP_ACM_CTRL.u32;
	return ISP_ACM_CTRL.bits.acm_en;
}

static __inline HI_VOID hi_isp_acm_dbg_enable_write(HI_U8 ispDev, HI_U8 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_ACM_CTRL ISP_ACM_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_ACM_CTRL.u32 = pstIspReg->ISP_ACM_CTRL.u32;
	ISP_ACM_CTRL.bits.acm_dbg_en = data;
	pstIspReg->ISP_ACM_CTRL.u32 = ISP_ACM_CTRL.u32;
}

static __inline HI_U8 hi_isp_acm_dbg_enable_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_ACM_CTRL ISP_ACM_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_ACM_CTRL.u32 = pstIspReg->ISP_ACM_CTRL.u32;
	return ISP_ACM_CTRL.bits.acm_dbg_en;
}

static __inline HI_VOID hi_isp_acm_stretch_write(HI_U8 ispDev, HI_U8 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_ACM_CTRL ISP_ACM_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_ACM_CTRL.u32 = pstIspReg->ISP_ACM_CTRL.u32;
	ISP_ACM_CTRL.bits.acm_stretch = data;
	pstIspReg->ISP_ACM_CTRL.u32 = ISP_ACM_CTRL.u32;
}

static __inline HI_U8 hi_isp_acm_stretch_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_ACM_CTRL ISP_ACM_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_ACM_CTRL.u32 = pstIspReg->ISP_ACM_CTRL.u32;
	return ISP_ACM_CTRL.bits.acm_stretch;
}

static __inline HI_VOID hi_isp_acm_cliprange_write(HI_U8 ispDev, HI_U8 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_ACM_CTRL ISP_ACM_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_ACM_CTRL.u32 = pstIspReg->ISP_ACM_CTRL.u32;
	ISP_ACM_CTRL.bits.acm_cliprange = data;
	pstIspReg->ISP_ACM_CTRL.u32 = ISP_ACM_CTRL.u32;
}

static __inline HI_U8 hi_isp_acm_cliprange_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_ACM_CTRL ISP_ACM_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_ACM_CTRL.u32 = pstIspReg->ISP_ACM_CTRL.u32;
	return ISP_ACM_CTRL.bits.acm_cliprange;
}

static __inline HI_VOID hi_isp_acm_cliporwrap_write(HI_U8 ispDev, HI_U8 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_ACM_CTRL ISP_ACM_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_ACM_CTRL.u32 = pstIspReg->ISP_ACM_CTRL.u32;
	ISP_ACM_CTRL.bits.acm_cliporwrap = data;
	pstIspReg->ISP_ACM_CTRL.u32 = ISP_ACM_CTRL.u32;
}

static __inline HI_U8 hi_isp_acm_cliporwrap_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_ACM_CTRL ISP_ACM_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_ACM_CTRL.u32 = pstIspReg->ISP_ACM_CTRL.u32;
	return ISP_ACM_CTRL.bits.acm_cliporwrap;
}

static __inline HI_VOID hi_isp_acm_dbg_pos_write(HI_U8 ispDev, HI_U16 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_ACM_CTRL ISP_ACM_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_ACM_CTRL.u32 = pstIspReg->ISP_ACM_CTRL.u32;
	ISP_ACM_CTRL.bits.acm_dbg_pos = data;
	pstIspReg->ISP_ACM_CTRL.u32 = ISP_ACM_CTRL.u32;
}

static __inline HI_U16 hi_isp_acm_dbg_pos_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_ACM_CTRL ISP_ACM_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_ACM_CTRL.u32 = pstIspReg->ISP_ACM_CTRL.u32;
	return ISP_ACM_CTRL.bits.acm_dbg_pos;
}

static __inline HI_VOID hi_isp_acm_dbg_mode_write(HI_U8 ispDev, HI_U8 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_ACM_CTRL ISP_ACM_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_ACM_CTRL.u32 = pstIspReg->ISP_ACM_CTRL.u32;
	ISP_ACM_CTRL.bits.acm_dbg_mode = data;
	pstIspReg->ISP_ACM_CTRL.u32 = ISP_ACM_CTRL.u32;
}

static __inline HI_U8 hi_isp_acm_dbg_mode_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_ACM_CTRL ISP_ACM_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_ACM_CTRL.u32 = pstIspReg->ISP_ACM_CTRL.u32;
	return ISP_ACM_CTRL.bits.acm_dbg_mode;
}

static __inline HI_VOID hi_isp_acm_cbcrthr_write(HI_U8 ispDev, HI_U8 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_ACM_CTRL ISP_ACM_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_ACM_CTRL.u32 = pstIspReg->ISP_ACM_CTRL.u32;
	ISP_ACM_CTRL.bits.acm_cbcrthr = data;
	pstIspReg->ISP_ACM_CTRL.u32 = ISP_ACM_CTRL.u32;
}

static __inline HI_U8 hi_isp_acm_cbcrthr_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_ACM_CTRL ISP_ACM_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_ACM_CTRL.u32 = pstIspReg->ISP_ACM_CTRL.u32;
	return ISP_ACM_CTRL.bits.acm_cbcrthr;
}

/*****************************************************
ISP_ACM_CTRL:         			
0x205b1404:                    	31:30		reserved
								29:20		acm_gain0 (luma)
								19:10		acm_gain1 (hue)
								9:0			acm_gain2 (sat)
								
*****************************************************/
static __inline HI_VOID hi_isp_acm_luma_gain_write(HI_U8 ispDev, HI_U16 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_ACM_ADJ ISP_ACM_ADJ;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_ACM_ADJ.u32 = pstIspReg->ISP_ACM_ADJ.u32;
	ISP_ACM_ADJ.bits.acm_gain0 = data;
	pstIspReg->ISP_ACM_ADJ.u32 = ISP_ACM_ADJ.u32;
}

static __inline HI_U16 hi_isp_acm_luma_gain_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_ACM_ADJ ISP_ACM_ADJ;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_ACM_ADJ.u32 = pstIspReg->ISP_ACM_ADJ.u32;
	return ISP_ACM_ADJ.bits.acm_gain0;
}

static __inline HI_VOID hi_isp_acm_hue_gain_write(HI_U8 ispDev, HI_U16 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_ACM_ADJ ISP_ACM_ADJ;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_ACM_ADJ.u32 = pstIspReg->ISP_ACM_ADJ.u32;
	ISP_ACM_ADJ.bits.acm_gain1 = data;
	pstIspReg->ISP_ACM_ADJ.u32 = ISP_ACM_ADJ.u32;
}

static __inline HI_U16 hi_isp_acm_hue_gain_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_ACM_ADJ ISP_ACM_ADJ;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_ACM_ADJ.u32 = pstIspReg->ISP_ACM_ADJ.u32;
	return ISP_ACM_ADJ.bits.acm_gain1;
}

static __inline HI_VOID hi_isp_acm_sat_gain_write(HI_U8 ispDev, HI_U16 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_ACM_ADJ ISP_ACM_ADJ;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_ACM_ADJ.u32 = pstIspReg->ISP_ACM_ADJ.u32;
	ISP_ACM_ADJ.bits.acm_gain2 = data;
	pstIspReg->ISP_ACM_ADJ.u32 = ISP_ACM_ADJ.u32;
}

static __inline HI_U16 hi_isp_acm_sat_gain_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_ACM_ADJ ISP_ACM_ADJ;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_ACM_ADJ.u32 = pstIspReg->ISP_ACM_ADJ.u32;
	return ISP_ACM_ADJ.bits.acm_gain2;
}

/*****************************************************
ISP_ACM_SIZE:         			
0x205b1410:                    	31:29		reserved
								28:16		height
								15:13		reserved
								12:0		width
								
*****************************************************/
static __inline HI_VOID hi_isp_acm_height_write(HI_U8 ispDev, HI_U16 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_ACM_SIZE ISP_ACM_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_ACM_SIZE.u32 = pstIspReg->ISP_ACM_SIZE.u32;
	ISP_ACM_SIZE.bits.height = data;
	pstIspReg->ISP_ACM_SIZE.u32 = ISP_ACM_SIZE.u32;
}


static __inline HI_U16 hi_isp_acm_height_read(HI_U8 ispDev){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_ACM_SIZE ISP_ACM_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_ACM_SIZE.u32 = pstIspReg->ISP_ACM_SIZE.u32;
	return ISP_ACM_SIZE.bits.height;

}

static __inline HI_VOID hi_isp_acm_width_write(HI_U8 ispDev, HI_U16 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_ACM_SIZE ISP_ACM_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_ACM_SIZE.u32 = pstIspReg->ISP_ACM_SIZE.u32;
	ISP_ACM_SIZE.bits.width = data;
	pstIspReg->ISP_ACM_SIZE.u32 = ISP_ACM_SIZE.u32;
}

static __inline HI_U16 hi_isp_acm_width_read(HI_U8 ispDev){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_ACM_SIZE ISP_ACM_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_ACM_SIZE.u32 = pstIspReg->ISP_ACM_SIZE.u32;
	return ISP_ACM_SIZE.bits.width ;
}

/*****************************************************
ISP_ACM_PARA_ADDR:         	
0x205b1414:                    	31:0		acm_para_addr
								
*****************************************************/
static __inline HI_VOID hi_isp_acm_coeff_addr_write(HI_U8 ispDev, HI_U32 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_ACM_PARA_ADDR ISP_ACM_PARA_ADDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_ACM_PARA_ADDR.u32 = pstIspReg->ISP_ACM_PARA_ADDR.u32;
	ISP_ACM_PARA_ADDR.bits.acm_para_addr = data;
	pstIspReg->ISP_ACM_PARA_ADDR.u32 = ISP_ACM_PARA_ADDR.u32;
}

/*****************************************************
ISP_ACM_PARA_UP:         	
0x205b1420:                    	31:1		reserved
								0			para_up
								
*****************************************************/
static __inline HI_VOID hi_isp_acm_coeff_up_write(HI_U8 ispDev, HI_U8 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_ACM_PARA_UP ISP_ACM_PARA_UP;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_ACM_PARA_UP.u32 = pstIspReg->ISP_ACM_PARA_UP.u32;
	ISP_ACM_PARA_UP.bits.para_up = data;
	pstIspReg->ISP_ACM_PARA_UP.u32 = ISP_ACM_PARA_UP.u32;
}

/*****************************************************
MODULE        -----DEHAZE-----
MODULE        -----DEHAZE-----
MODULE        -----DEHAZE-----
*****************************************************/
/*****************************************************
ISP_DEHAZE_CFG: 		ISP_DEHAZE_CFG DEHAZEenable
0x205c6200: 			31:1		reserved									 
						0			en
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_en_write(HI_U8 ispDev, HI_U8 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEHAZE_CFG ISP_DEHAZE_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEHAZE_CFG.u32 = pstIspReg->ISP_DEHAZE_CFG.u32;
	ISP_DEHAZE_CFG.bits.en = data;
	pstIspReg->ISP_DEHAZE_CFG.u32 = ISP_DEHAZE_CFG.u32;
}

static __inline HI_U8 hi_isp_dehaze_en_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEHAZE_CFG ISP_DEHAZE_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEHAZE_CFG.u32 = pstIspReg->ISP_DEHAZE_CFG.u32;
	return ISP_DEHAZE_CFG.bits.en;
}

/*****************************************************
ISP_DEHAZE_PRE_UPDATE: 		
0x205a7010: 			31:1		reserved									 
						0			pre_update
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_pre_update_write(HI_U8 ispDev, HI_U8 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEHAZE_PRE_UPDATE ISP_DEHAZE_PRE_UPDATE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEHAZE_PRE_UPDATE.u32 = pstIspReg->ISP_DEHAZE_PRE_UPDATE.u32;
	ISP_DEHAZE_PRE_UPDATE.bits.pre_update = data;
	pstIspReg->ISP_DEHAZE_PRE_UPDATE.u32 = ISP_DEHAZE_PRE_UPDATE.u32;
}

/*****************************************************
ISP_DEHAZE_BLK_SIZE: 		
0x205a7014: 			            31:01		reserved	
									24:16		block_sizeh  
									15:06         reserved
						            08:00		block_sizev    vetical
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_block_sizeh_write(HI_U8 ispDev, HI_U16 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEHAZE_BLK_SIZE ISP_DEHAZE_BLK_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEHAZE_BLK_SIZE.u32 = pstIspReg->ISP_DEHAZE_BLK_SIZE.u32;
	ISP_DEHAZE_BLK_SIZE.bits.block_sizeh = data;
	pstIspReg->ISP_DEHAZE_BLK_SIZE.u32 = ISP_DEHAZE_BLK_SIZE.u32;
}

static __inline HI_VOID hi_isp_dehaze_block_sizev_write(HI_U8 ispDev, HI_U16 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEHAZE_BLK_SIZE ISP_DEHAZE_BLK_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEHAZE_BLK_SIZE.u32 = pstIspReg->ISP_DEHAZE_BLK_SIZE.u32;
	ISP_DEHAZE_BLK_SIZE.bits.block_sizev = data;
	pstIspReg->ISP_DEHAZE_BLK_SIZE.u32 = ISP_DEHAZE_BLK_SIZE.u32;
}

/*****************************************************
ISP_DEHAZE_BLK_NUM: 		
0x205a7018: 			            31:11		reserved	
						            10:00		block_sum    dehaze
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_block_sum_write(HI_U8 ispDev, HI_U16 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEHAZE_BLK_SUM ISP_DEHAZE_BLK_SUM;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEHAZE_BLK_SUM.u32 = pstIspReg->ISP_DEHAZE_BLK_SUM.u32;
	ISP_DEHAZE_BLK_SUM.bits.block_sum = data;
	pstIspReg->ISP_DEHAZE_BLK_SUM.u32 = ISP_DEHAZE_BLK_SUM.u32;
}

/*****************************************************
ISP_DEHAZE_DC_SIZE: 		
0x205a701c: 			            31:10		reserved	
									09:05		dc_numh   
						            04:00		dc_numv   
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_dc_numh_write(HI_U8 ispDev, HI_U16 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEHAZE_DC_SIZE ISP_DEHAZE_DC_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEHAZE_DC_SIZE.u32 = pstIspReg->ISP_DEHAZE_DC_SIZE.u32;
	ISP_DEHAZE_DC_SIZE.bits.dc_numh = data;
	pstIspReg->ISP_DEHAZE_DC_SIZE.u32 = ISP_DEHAZE_DC_SIZE.u32;
}

static __inline HI_VOID hi_isp_dehaze_dc_numv_write(HI_U8 ispDev, HI_U16 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEHAZE_DC_SIZE ISP_DEHAZE_DC_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEHAZE_DC_SIZE.u32 = pstIspReg->ISP_DEHAZE_DC_SIZE.u32;
	ISP_DEHAZE_DC_SIZE.bits.dc_numv = data;
	pstIspReg->ISP_DEHAZE_DC_SIZE.u32 = ISP_DEHAZE_DC_SIZE.u32;
}

/*****************************************************
ISP_DEHAZE_X: 		
0x205a7020: 			31:22		reserved	
						21:00		phase_x  
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_phase_x_write(HI_U8 ispDev, HI_U32 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEHAZE_X ISP_DEHAZE_X;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEHAZE_X.u32 = pstIspReg->ISP_DEHAZE_X.u32;
	ISP_DEHAZE_X.bits.phase_x = data;
	pstIspReg->ISP_DEHAZE_X.u32 = ISP_DEHAZE_X.u32;
}

/*****************************************************
ISP_DEHAZE_Y: 		
0x205a7024: 			31:22		reserved	
						21:00		phase_y   
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_phase_y_write(HI_U8 ispDev, HI_U32 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEHAZE_Y ISP_DEHAZE_Y;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEHAZE_Y.u32 = pstIspReg->ISP_DEHAZE_Y.u32;
	ISP_DEHAZE_Y.bits.phase_y = data;
	pstIspReg->ISP_DEHAZE_Y.u32 = ISP_DEHAZE_Y.u32;
}

/*****************************************************
ISP_DEHAZE_STAT_MODE: 	
0x205a7028: 			31:1		reserved									 
						0			max_mode
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_max_mode_write(HI_U8 ispDev, HI_U8 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEHAZE_STAT_MODE ISP_DEHAZE_STAT_MODE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEHAZE_STAT_MODE.u32 = pstIspReg->ISP_DEHAZE_STAT_MODE.u32;
	ISP_DEHAZE_STAT_MODE.bits.max_mode = data;
	pstIspReg->ISP_DEHAZE_STAT_MODE.u32 = ISP_DEHAZE_STAT_MODE.u32;
}

/*****************************************************
ISP_DEHAZE_NEG_MODE: 	
0x205a702c: 			31:1		reserved									 
						0			neg_mode
									0
									1
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_neg_mode_write(HI_U8 ispDev, HI_U8 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEHAZE_NEG_MODE ISP_DEHAZE_NEG_MODE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEHAZE_NEG_MODE.u32 = pstIspReg->ISP_DEHAZE_NEG_MODE.u32;
	ISP_DEHAZE_NEG_MODE.bits.neg_mode = data;
	pstIspReg->ISP_DEHAZE_NEG_MODE.u32 = ISP_DEHAZE_NEG_MODE.u32;
}

/*****************************************************
ISP_DEHAZE_AIR: 		IS
0x205a7030: 			31:30		reserved	
					      29:20       	air_r r
						19:10		air_g g		      
						09:00		air_b b
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_air_r_write(HI_U8 ispDev, HI_U16 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEHAZE_AIR ISP_DEHAZE_AIR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEHAZE_AIR.u32 = pstIspReg->ISP_DEHAZE_AIR.u32;
	ISP_DEHAZE_AIR.bits.air_r = data;
	pstIspReg->ISP_DEHAZE_AIR.u32 = ISP_DEHAZE_AIR.u32;
}

static __inline HI_VOID hi_isp_dehaze_air_g_write(HI_U8 ispDev, HI_U16 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEHAZE_AIR ISP_DEHAZE_AIR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEHAZE_AIR.u32 = pstIspReg->ISP_DEHAZE_AIR.u32;
	ISP_DEHAZE_AIR.bits.air_g = data;
	pstIspReg->ISP_DEHAZE_AIR.u32 = ISP_DEHAZE_AIR.u32;
}

static __inline HI_VOID hi_isp_dehaze_air_b_write(HI_U8 ispDev, HI_U16 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEHAZE_AIR ISP_DEHAZE_AIR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEHAZE_AIR.u32 = pstIspReg->ISP_DEHAZE_AIR.u32;
	ISP_DEHAZE_AIR.bits.air_b = data;
	pstIspReg->ISP_DEHAZE_AIR.u32 = ISP_DEHAZE_AIR.u32;
}

/*****************************************************
ISP_DEHAZE_THLD: 	
0x205a7034: 			31:08		reserved						      
						09:00		thld t
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_thld_write(HI_U8 ispDev, HI_U16 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEHAZE_THLD ISP_DEHAZE_THLD;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEHAZE_THLD.u32 = pstIspReg->ISP_DEHAZE_THLD.u32;
	ISP_DEHAZE_THLD.bits.thld = data;
	pstIspReg->ISP_DEHAZE_THLD.u32 = ISP_DEHAZE_THLD.u32;
}

/*****************************************************
ISP_DEHAZE_GSTRTH: 	
0x205a7038: 			31:08		reserved						      
						07:00		gstrth t
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_gstrth_write(HI_U8 ispDev, HI_U8 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEHAZE_GSTRTH ISP_DEHAZE_GSTRTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEHAZE_GSTRTH.u32 = pstIspReg->ISP_DEHAZE_GSTRTH.u32;
	ISP_DEHAZE_GSTRTH.bits.gstrth = data;
	pstIspReg->ISP_DEHAZE_GSTRTH.u32 = ISP_DEHAZE_GSTRTH.u32;
}

/*****************************************************
ISP_DEHAZE_BLTHLD: 	
0x205a703c: 			31:08		reserved						      
						09:00		blthld 
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_blthld_write(HI_U8 ispDev, HI_U16 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEHAZE_BLTHLD ISP_DEHAZE_BLTHLD;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEHAZE_BLTHLD.u32 = pstIspReg->ISP_DEHAZE_BLTHLD.u32;
	ISP_DEHAZE_BLTHLD.bits.blthld = data;
	pstIspReg->ISP_DEHAZE_BLTHLD.u32 = ISP_DEHAZE_BLTHLD.u32;
}

/*****************************************************
ISP_DEHAZE_GCOEF_UPDATE:
0x205a7040: 			31:01		reserved						      
						00		update
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_gcoef_update_write(HI_U8 ispDev, HI_U32 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEHAZE_STR_LUT_UPDATE ISP_DEHAZE_STR_LUT_UPDATE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEHAZE_STR_LUT_UPDATE.u32 = pstIspReg->ISP_DEHAZE_STR_LUT_UPDATE.u32;
	ISP_DEHAZE_STR_LUT_UPDATE.bits.str_lut_update = data;
	pstIspReg->ISP_DEHAZE_STR_LUT_UPDATE.u32 = ISP_DEHAZE_STR_LUT_UPDATE.u32;
}

/*****************************************************
ISP_DEHAZE_MINSTAT_RADDR: 	
0x205a7088: 			31:00		minstat_raddr						      
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_minstat_raddr_write(HI_U8 ispDev, HI_U32 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEHAZE_MINSTAT_RADDR ISP_DEHAZE_MINSTAT_RADDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEHAZE_MINSTAT_RADDR.u32 = pstIspReg->ISP_DEHAZE_MINSTAT_RADDR.u32;
	ISP_DEHAZE_MINSTAT_RADDR.bits.minstat_raddr = data;
	pstIspReg->ISP_DEHAZE_MINSTAT_RADDR.u32 = ISP_DEHAZE_MINSTAT_RADDR.u32;
}

/*****************************************************
ISP_DEHAZE_MINSTAT_RDATA: 	
0x205a708c: 		31:26		reserved
					25:16		minstat_rdata_h
					15:10		reserved
					09:00		minstat_rdata_l					      
*****************************************************/
static __inline HI_U32 hi_isp_dehaze_minstat_rdata_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEHAZE_MINSTAT_RDATA ISP_DEHAZE_MINSTAT_RDATA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEHAZE_MINSTAT_RDATA.u32 = pstIspReg->ISP_DEHAZE_MINSTAT_RDATA.u32;
	return ISP_DEHAZE_MINSTAT_RDATA.u32;
}

/*****************************************************
ISP_DEHAZE_MAXSTAT_RADDR: 	
0x205a7098: 			31:00		maxstat_raddr						      
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_maxstat_raddr_write(HI_U8 ispDev, HI_U32 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEHAZE_MAXSTAT_RADDR ISP_DEHAZE_MAXSTAT_RADDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEHAZE_MAXSTAT_RADDR.u32 = pstIspReg->ISP_DEHAZE_MAXSTAT_RADDR.u32;
	ISP_DEHAZE_MAXSTAT_RADDR.bits.maxstat_raddr = data;
	pstIspReg->ISP_DEHAZE_MAXSTAT_RADDR.u32 = ISP_DEHAZE_MAXSTAT_RADDR.u32;
}

/*****************************************************
ISP_DEHAZE_MAXSTAT_RDATA: 	
0x205a709c: 			31:00		maxstat_rdata						      
*****************************************************/
static __inline HI_U32 hi_isp_dehaze_maxstat_rdata_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEHAZE_MAXSTAT_RDATA ISP_DEHAZE_MAXSTAT_RDATA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEHAZE_MAXSTAT_RDATA.u32 = pstIspReg->ISP_DEHAZE_MAXSTAT_RDATA.u32;
	return ISP_DEHAZE_MAXSTAT_RDATA.u32;
}

/*****************************************************
ISP_DEHAZE_PRESTAT_WADDR: 
0x205a70a0: 			31:00		prestat_waddr						      
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_prestat_waddr_write(HI_U8 ispDev, HI_U32 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEHAZE_PRESTAT_WADDR ISP_DEHAZE_PRESTAT_WADDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEHAZE_PRESTAT_WADDR.u32 = pstIspReg->ISP_DEHAZE_PRESTAT_WADDR.u32;
	ISP_DEHAZE_PRESTAT_WADDR.bits.prestat_waddr = data;
	pstIspReg->ISP_DEHAZE_PRESTAT_WADDR.u32 = ISP_DEHAZE_PRESTAT_WADDR.u32;
}

/*****************************************************
ISP_DEHAZE_PRESTAT_WDATA: 
0x205a70a4: 		31:26		reserved
					25:16		prestat_wdata_H
				15:10		reserved
					09:00		prestat_wdata_l
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_prestat_wdata_write(HI_U8 ispDev, HI_U32 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEHAZE_PRESTAT_WDATA ISP_DEHAZE_PRESTAT_WDATA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEHAZE_PRESTAT_WDATA.u32 = data;
	pstIspReg->ISP_DEHAZE_PRESTAT_WDATA.u32 = ISP_DEHAZE_PRESTAT_WDATA.u32;
}

/*****************************************************
ISP_DEHAZE_LUT_WADDR: 
0x205a70b0: 			31:00		lut_waddr						      
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_lut_waddr_write(HI_U8 ispDev, HI_U32 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEHAZE_LUT_WADDR ISP_DEHAZE_LUT_WADDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEHAZE_LUT_WADDR.u32 = pstIspReg->ISP_DEHAZE_LUT_WADDR.u32;
	ISP_DEHAZE_LUT_WADDR.bits.lut_waddr = data;
	pstIspReg->ISP_DEHAZE_LUT_WADDR.u32 = ISP_DEHAZE_LUT_WADDR.u32;
}

/*****************************************************
ISP_DEHAZE_LUT_WDATA: 	
0x205a70b4: 			31:00		lut_wdata						      
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_lut_wdata_write(HI_U8 ispDev, HI_U32 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEHAZE_LUT_WDATA ISP_DEHAZE_LUT_WDATA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEHAZE_LUT_WDATA.u32 = pstIspReg->ISP_DEHAZE_LUT_WDATA.u32;
	ISP_DEHAZE_LUT_WDATA.bits.lut_wdata = data;
	pstIspReg->ISP_DEHAZE_LUT_WDATA.u32 = ISP_DEHAZE_LUT_WDATA.u32;
}

/*****************************************************
ISP_DEHAZE_SIZE: 		
0x205a70f0: 			31:29		reserved									 
						28:16		height
						15:13		reserved
						12:00		width
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_height_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEHAZE_SIZE ISP_DEHAZE_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEHAZE_SIZE.u32 = pstIspReg->ISP_DEHAZE_SIZE.u32;
	ISP_DEHAZE_SIZE.bits.height = data;
	pstIspReg->ISP_DEHAZE_SIZE.u32 = ISP_DEHAZE_SIZE.u32;
}

static __inline HI_VOID hi_isp_dehaze_width_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_DEHAZE_SIZE ISP_DEHAZE_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_DEHAZE_SIZE.u32 = pstIspReg->ISP_DEHAZE_SIZE.u32;
	ISP_DEHAZE_SIZE.bits.width = data;
	pstIspReg->ISP_DEHAZE_SIZE.u32 = ISP_DEHAZE_SIZE.u32;
}

/*****************************************************
MODULE        -----AF-----
MODULE        -----AF-----
MODULE        -----AF-----
*****************************************************/

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
    
//--------------------------------------------------------------------------------------------

/*
   cause hi3516cv300's offset has a little bit of difference from 3519, construct a new function...
   ...for adaptability
*/
#define HI_VI_AF_BASE_OFFSET 0x200

#define REG_BITS_MODIFY_TYPE1(wrfunc, rdfunc, offset, msb, lsb, data) \
    REG_BITS_MODIFY(wrfunc, rdfunc, (offset+HI_VI_AF_BASE_OFFSET), msb, lsb, data)
    
#define REG_BITS_READ_TYPE1(wrfunc, rdfunc, offset, msb, lsb) \
    REG_BITS_READ(wrfunc, rdfunc, (offset+HI_VI_AF_BASE_OFFSET), msb, lsb)

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_pos_write
// ------------------------------------------------------------------------------ //
//  af enable
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_POS_DEFAULT (0)
#define HI_VI_AF_POS_DATASIZE (0)
#define HI_VI_AF_POS_FE     (0)
#define HI_VI_AF_POS_BE     (1)
// args: data (1-bit)
static __inline HI_VOID hi_vi_af_pos_write(HI_U32 data) {  

    REG_BITS_MODIFY(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x0050, 16, 16, data);
}
static __inline HI_U32 hi_vi_af_pos_read(HI_VOID) {
    
    return REG_BITS_READ(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x0050, 16, 16);
}


// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_enable
// ------------------------------------------------------------------------------ //
//  af enable
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_ENABLE_DEFAULT (0)
#define HI_VI_AF_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_enable_write(HI_U32 data) {  

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 0, 0, data);
}
static __inline HI_U32 hi_vi_af_enable_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 0, 0);
}

// ------------------------------------------------------------------------------ //
// Register: iir0_en0
// ------------------------------------------------------------------------------ //
// iir0 en0
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_IIR0_EN1_DEFAULT (0)
#define HI_VI_AF_IIR0_EN1_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_iir0_en0_write(HI_U32 data) {

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 1, 1, data);
}
static __inline HI_U32 hi_vi_af_iir0_en0_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 1, 1);

}

// ------------------------------------------------------------------------------ //
// Register: iir0_en1
// ------------------------------------------------------------------------------ //
// iir0 en1
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_IIR0_EN1_DEFAULT (0)
#define HI_VI_AF_IIR0_EN1_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_iir0_en1_write(HI_U32 data) {

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 2, 2, data);
}
static __inline HI_U32 hi_vi_af_iir0_en1_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 2, 2);
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
static __inline HI_VOID hi_vi_af_iir0_en2_write(HI_U32 data) {

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 3, 3, data);
}
    
static __inline HI_U32 hi_vi_af_iir0_en2_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 3, 3);
}

// ------------------------------------------------------------------------------ //
// Register: iir1_en0
// ------------------------------------------------------------------------------ //
// iir1 en0
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_IIR1_EN1_DEFAULT (0)
#define HI_VI_AF_IIR1_EN1_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_iir1_en0_write(HI_U32 data) {

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 4, 4, data);
}
static __inline HI_U32 hi_vi_af_iir1_en0_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 4, 4);
}

// ------------------------------------------------------------------------------ //
// Register: iir1_en1
// ------------------------------------------------------------------------------ //
// iir1 en1
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_IIR1_EN1_DEFAULT (0)
#define HI_VI_AF_IIR1_EN1_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_iir1_en1_write(HI_U32 data) {

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 5, 5, data);
}
static __inline HI_U32 hi_vi_af_iir1_en1_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 5, 5);
}

// ------------------------------------------------------------------------------ //
// Register: iir1_en2
// ------------------------------------------------------------------------------ //
// iir1 en2
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_IIR1_EN2_DEFAULT (0)
#define HI_VI_AF_IIR1_EN2_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_iir1_en2_write(HI_U32 data) {

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 6, 6, data);
}
static __inline HI_U32 hi_vi_af_iir1_en2_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 6, 6);
}

// ------------------------------------------------------------------------------ //
// Register: fvpeak_mode
// ------------------------------------------------------------------------------ //
// fv peak mode
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_FV_PEAK_MODE_DEFAULT (0)
#define HI_VI_AF_FV_PEAK_MODE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_fvmode_peak_write(HI_U32 data) {

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 7, 7, data);
}
static __inline HI_U32 hi_vi_af_fvmode_peak_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 7, 7);
}

// ------------------------------------------------------------------------------ //
// Register: af squ_mode
// ------------------------------------------------------------------------------ //
// af squ mode
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_FVMODE_SQU_DEFAULT (1)
#define HI_VI_AF_FVMODE_SQU_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_fvmode_squ_write(HI_U32 data) {

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 8, 8, data);
}
static __inline HI_U32 hi_vi_af_fvmode_squ_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 8, 8);
}

// ------------------------------------------------------------------------------ //
// Register: offset_en
// ------------------------------------------------------------------------------ //
// offset en
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_OFFSET_EN_DEFAULT (0)
#define HI_VI_AF_OFFSET_EN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_offset_en_write(HI_U32 data) {

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 9, 9, data);
}
static __inline HI_U32 hi_vi_af_offset_en_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 9, 9);
}

// ------------------------------------------------------------------------------ //
// Register: crop_en
// ------------------------------------------------------------------------------ //
// crop enable
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_CROP_EN_DEFAULT (0)
#define HI_VI_AF_CROP_EN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_crop_en_write(HI_U32 data) {

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 10, 10, data);
}
static __inline HI_U32 hi_vi_af_crop_en_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 10, 10);
}

// ------------------------------------------------------------------------------ //
// Register: lpf_en
// ------------------------------------------------------------------------------ //
// af lpf enable
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_LPF_EN_DEFAULT (0)
#define HI_VI_AF_LPF_EN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_lpf_en_write(HI_U32 data) {

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 11, 11, data);
}
static __inline HI_U32 hi_vi_af_lpf_en_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 11, 11);
}

// ------------------------------------------------------------------------------ //
// Register: median_en
// ------------------------------------------------------------------------------ //
// af median filter enable
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_MEDIAN_EN_DEFAULT (0)
#define HI_VI_AF_MEDIAN_EN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_median_en_write(HI_U32 data) {

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 12, 12, data);
}
static __inline HI_U32 hi_vi_af_median_en_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 12, 12);
}

// ------------------------------------------------------------------------------ //
// Register: sqrt_en
// ------------------------------------------------------------------------------ //
// af gamma enable
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_SQRT_EN_DEFAULT (0)
#define HI_VI_AF_SQRT_EN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_sqrt_en_write(HI_U32 data) {

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 13, 13, data);
}
static __inline HI_U32 hi_vi_af_sqrt_en_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 13, 13);
}

// ------------------------------------------------------------------------------ //
// Register: raw_mode
// ------------------------------------------------------------------------------ //
// AF measure position, YUV or RAW domain
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_RAW_MODE_DEFAULT (0)
#define HI_VI_AF_RAW_MODE_DATASIZE (1)
#define HI_VI_AF_RAW_MODE_YUV (0)
#define HI_VI_AF_RAW_MODE_RAW (1)
// args: data (1-bit)
static __inline HI_VOID hi_vi_af_raw_mode_write(HI_U32 data) {

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 14, 14, data);
}
static __inline HI_U32 hi_vi_af_raw_mode_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 14, 14);
}

// ------------------------------------------------------------------------------ //
// Register: bayer_mode
// ------------------------------------------------------------------------------ //
// bayer pattern mode
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_BAYER_MODE_DEFAULT (0)
#define HI_VI_AF_BAYER_MODE_DATASIZE (2)

// args: data (2-bit)
static __inline HI_VOID hi_vi_af_bayer_mode_write(HI_U32 data) {

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 16, 15, data);
}
static __inline HI_U32 hi_vi_af_bayer_mode_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 16, 15);
}


// ------------------------------------------------------------------------------ //
// Register: iir0_ds_en
// ------------------------------------------------------------------------------ //
// iir0 ds enable
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_IIR0_DS_EN_DEFAULT (0x1)
#define HI_VI_AF_IIR0_DS_EN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_iir0_ds_en_write(HI_U32 data) {

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 17, 17, data);
}
static __inline HI_U32 hi_vi_af_iir0_ds_en_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 17, 17);
}

// ------------------------------------------------------------------------------ //
// Register: iir1_ds_en
// ------------------------------------------------------------------------------ //
// iir1 ds enable
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_IIR1_DS_EN_DEFAULT (0x1)
#define HI_VI_AF_IIR1_DS_EN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_iir1_ds_en_write(HI_U32 data) {

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 18, 18, data);
}
static __inline HI_U32 hi_vi_af_iir1_ds_en_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 18, 18);
}

// ------------------------------------------------------------------------------ //
// Register: fir0_lpf_en
// ------------------------------------------------------------------------------ //
// fir0 lpf enable
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_FIR0_LPF_EN_DEFAULT (0x1)
#define HI_VI_AF_FIR0_LPF_EN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_fir0_lpf_en_write(HI_U32 data) {

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 19, 19, data);
}
static __inline HI_U32 hi_vi_af_fir0_lpf_en_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 19, 19);
}

// ------------------------------------------------------------------------------ //
// Register: fir1_lpf_en
// ------------------------------------------------------------------------------ //
// fir1 lpf enable
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_FIR1_LPF_EN_DEFAULT (0x1)
#define HI_VI_AF_FIR1_LPF_EN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_fir1_lpf_en_write(HI_U32 data) {

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 20, 20, data);
}
static __inline HI_U32 hi_vi_af_fir1_lpf_en_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 20, 20);
}

// ------------------------------------------------------------------------------ //
// Register: iir0_ldg_en
// ------------------------------------------------------------------------------ //
// iir0 level depend gain enable
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_IIR0_LDG_EN_DEFAULT (0)
#define HI_VI_AF_IIR0_LDG_EN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_iir0_ldg_en_write(HI_U32 data) {   

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 21, 21, data);
}
static __inline HI_U32 hi_vi_af_iir0_ldg_en_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 21, 21);
}

// ------------------------------------------------------------------------------ //
// Register: iir1_ldg_en
// ------------------------------------------------------------------------------ //
// iir1 level depend gain enable
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_IIR1_LDG_EN_DEFAULT (0)
#define HI_VI_AF_IIR1_LDG_EN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_iir1_ldg_en_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 22, 22, data);
}
static __inline HI_U32 hi_vi_af_iir1_ldg_en_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 22, 22);
}


// ------------------------------------------------------------------------------ //
// Register: fir0_ldg_en
// ------------------------------------------------------------------------------ //
// fir0 level depend gain enable
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_FIR0_LDG_EN_DEFAULT (0)
#define HI_VI_AF_FIR0_LDG_EN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_fir0_ldg_en_write(HI_U32 data) {

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 23, 23, data);
}
static __inline HI_U32 hi_vi_af_fir0_ldg_en_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 23, 23);
}

// ------------------------------------------------------------------------------ //
// Register: fir1_ldg_en
// ------------------------------------------------------------------------------ //
// fir1 level depend gain enable
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_FIR1_LDG_EN_DEFAULT (0)
#define HI_VI_AF_FIR1_LDG_EN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_fir1_ldg_en_write(HI_U32 data) {

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 24, 24, data);
}
static __inline HI_U32 hi_vi_af_fir1_ldg_en_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 24, 24);
}

// ------------------------------------------------------------------------------ //
// Register: ck_gt_en
// ------------------------------------------------------------------------------ //
//  af gate clk enable
// ------------------------------------------------------------------------------ //

#define HI_VI_AF_CK_GT_EN_DEFAULT (0x1)
#define HI_VI_AF_CK_GT_EN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_ck_gt_en_write(HI_U32 data) {   

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 31, 31, data);
}
static __inline HI_U32 hi_vi_af_ck_gt_en_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12000, 31, 31);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_Hwnd
// ------------------------------------------------------------------------------ //
//  af Horizontal window number
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_HWND_DEFAULT (0x11)
#define HI_VI_AF_HWND_DATASIZE (5)

// args: data (5-bit)
static __inline HI_VOID hi_vi_af_hwnd_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12010, 4, 0, data);
}
static __inline HI_U32 hi_vi_af_hwnd_read(HI_VOID) {
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12010, 4, 0);
}


// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_Vwnd
// ------------------------------------------------------------------------------ //
//  af Vertical window number
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_VWND_DEFAULT (0xF)
#define HI_VI_AF_VWND_DATASIZE (5)

// args: data (5-bit)
static __inline HI_VOID hi_vi_af_vwnd_write(HI_U32 data) {   

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12010, 12, 8, data);
}
static __inline HI_U32 hi_vi_af_vwnd_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12010, 12, 8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_crop_posx
// ------------------------------------------------------------------------------ //
//  af crop pos x
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_CROP_POSX_DEFAULT (0x0)
#define HI_VI_AF_CROP_POSX_DATASIZE (13)

// args: data (13-bit)
static __inline HI_VOID hi_vi_af_crop_posx_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12014, 12, 0, data);

}
static __inline HI_U32 hi_vi_af_crop_posx_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12014, 12, 0);
}


// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_crop_pos_y
// ------------------------------------------------------------------------------ //
//  af crop pos y
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_CROP_POSY_DEFAULT (0x0)
#define HI_VI_AF_CROP_POSY_DATASIZE (13)

// args: data (13-bit)
static __inline HI_VOID hi_vi_af_crop_posy_write(HI_U32 data) {  

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12014, 28, 16, data);

}
static __inline HI_U32 hi_vi_af_crop_posy_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12014, 28, 16);
}


// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_crop_hsize
// ------------------------------------------------------------------------------ //
//  af crop Hsize
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_CROP_HSIZE_DEFAULT (0x77E)
#define HI_VI_AF_CROP_HSIZE_DATASIZE (13)

// args: data (13-bit)
static __inline HI_VOID hi_vi_af_crop_hsize_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12018, 12, 0, data);
}
static __inline HI_U32 hi_vi_af_crop_hsize_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12018, 12, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_crop_vsize
// ------------------------------------------------------------------------------ //
//  af crop vsize
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_CROP_VSIZE_DEFAULT (0x437)
#define HI_VI_AF_CROP_VSIZE_DATASIZE (13)

// args: data (13-bit)
static __inline HI_VOID hi_vi_af_crop_vsize_write(HI_U32 data) {

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12018, 28, 16, data);
}
static __inline HI_U32 hi_vi_af_crop_vsize_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12018, 28, 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_median_th
// ------------------------------------------------------------------------------ //
//  af median filter threshold
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_MEDIAN_TH_DEFAULT (0x0)
#define HI_VI_AF_MEDIAN_TH_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_vi_af_median_th_write(HI_U32 data) {   

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x1201c, 15, 0, data);
}
static __inline HI_U32 hi_vi_af_median_th_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x1201c, 15, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_gain0
// ------------------------------------------------------------------------------ //
//  af iir0 gain0
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_GAIN0_DEFAULT (0xA0)
#define HI_VI_AF_IIR0_GAIN0_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_iir0_gain0_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12020, 7, 0, data);
}
static __inline HI_U32 hi_vi_af_iir0_gain0_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12020, 7, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_gain0
// ------------------------------------------------------------------------------ //
//  af iir1 gain0
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_GAIN0_DEFAULT (0xA1)
#define HI_VI_AF_IIR1_GAIN0_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_iir1_gain0_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12020, 23, 16, data);
}
static __inline HI_U32 hi_vi_af_iir1_gain0_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12020, 23, 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_gain1
// ------------------------------------------------------------------------------ //
//  af iir0 gain1
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_GAIN1_DEFAULT (0x1BC)
#define HI_VI_AF_IIR0_GAIN1_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_vi_af_iir0_gain1_write(HI_U32 data) {   

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12024, 9, 0, data);
}
static __inline HI_U32 hi_vi_af_iir0_gain1_read(HI_VOID) {
    
    return COMPLEMENT_ALIGN32(REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12024, 9, 0), 9);

}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_gain1
// ------------------------------------------------------------------------------ //
//  af iir1 gain1
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_GAIN1_DEFAULT (0x244)
#define HI_VI_AF_IIR1_GAIN1_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_vi_af_iir1_gain1_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12024, 25, 16, data);

}
static __inline HI_U32 hi_vi_af_iir1_gain1_read(HI_VOID) {
    
    return COMPLEMENT_ALIGN32(REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12024, 25, 16), 9);

}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_gain2
// ------------------------------------------------------------------------------ //
//  af iir0 gain2
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_GAIN2_DEFAULT (0x328)
#define HI_VI_AF_IIR0_GAIN2_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_vi_af_iir0_gain2_write(HI_U32 data) {   

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12028, 9, 0, data);

}
static __inline HI_U32 hi_vi_af_iir0_gain2_read(HI_VOID) {
    
    return COMPLEMENT_ALIGN32(REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12028, 9, 0), 9);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_gain2
// ------------------------------------------------------------------------------ //
//  af iir1 gain2
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_GAIN2_DEFAULT (0x328)
#define HI_VI_AF_IIR1_GAIN2_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_vi_af_iir1_gain2_write(HI_U32 data) {  

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12028, 25, 16, data);
}
static __inline HI_U32 hi_vi_af_iir1_gain2_read(HI_VOID) {
    
    return COMPLEMENT_ALIGN32(REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12028, 25, 16), 9);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_gain3
// ------------------------------------------------------------------------------ //
//  af iir0 gain3
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_GAIN3_DEFAULT (0)
#define HI_VI_AF_IIR0_GAIN3_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_vi_af_iir0_gain3_write(HI_U32 data) {  

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x1202c, 9, 0, data);
}
static __inline HI_U32 hi_vi_af_iir0_gain3_read(HI_VOID) {

    return COMPLEMENT_ALIGN32(REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x1202c, 9, 0), 9);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_gain3
// ------------------------------------------------------------------------------ //
//  af iir1 gain3
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_GAIN3_DEFAULT (0)
#define HI_VI_AF_IIR1_GAIN3_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_vi_af_iir1_gain3_write(HI_U32 data) {  

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x1202c, 25, 16, data);

}
static __inline HI_U32 hi_vi_af_iir1_gain3_read(HI_VOID) {

    return COMPLEMENT_ALIGN32(REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x1202c, 25, 16), 9);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_gain4
// ------------------------------------------------------------------------------ //
//  af iir0 gain4
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_GAIN4_DEFAULT (0)
#define HI_VI_AF_IIR0_GAIN4_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_vi_af_iir0_gain4_write(HI_U32 data) {  

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12030, 9, 0, data);

}
static __inline HI_U32 hi_vi_af_iir0_gain4_read(HI_VOID) {

    return COMPLEMENT_ALIGN32(REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12030, 9, 0), 9);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_gain4
// ------------------------------------------------------------------------------ //
//  af iir1 gain4
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_GAIN4_DEFAULT (0)
#define HI_VI_AF_IIR1_GAIN4_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_vi_af_iir1_gain4_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12030, 25, 16, data);
}
static __inline HI_U32 hi_vi_af_iir1_gain4_read(HI_VOID) {

    return COMPLEMENT_ALIGN32(REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12030, 25, 16), 9);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_gain5
// ------------------------------------------------------------------------------ //
//  af iir0 gain5
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_GAIN5_DEFAULT (0x17C)
#define HI_VI_AF_IIR0_GAIN5_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_vi_af_iir0_gain5_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12034, 9, 0, data);
}
static __inline HI_U32 hi_vi_af_iir0_gain5_read(HI_VOID) {

    return COMPLEMENT_ALIGN32(REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12034, 9, 0), 9);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_gain5
// ------------------------------------------------------------------------------ //
//  af iir1 gain5
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_GAIN5_DEFAULT (0x284)
#define HI_VI_AF_IIR1_GAIN5_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_vi_af_iir1_gain5_write(HI_U32 data) {

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12034, 25, 16, data);
}
static __inline HI_U32 hi_vi_af_iir1_gain5_read(HI_VOID) {
    
    return COMPLEMENT_ALIGN32(REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12034, 25, 16), 9);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_gain6
// ------------------------------------------------------------------------------ //
//  af iir0 gain6
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_GAIN6_DEFAULT (0x33C)
#define HI_VI_AF_IIR0_GAIN6_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_vi_af_iir0_gain6_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12038, 9, 0, data);
}
static __inline HI_U32 hi_vi_af_iir0_gain6_read(HI_VOID) {

    return COMPLEMENT_ALIGN32(REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12038, 9, 0), 9);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_gain6
// ------------------------------------------------------------------------------ //
//  af iir1 gain6
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_GAIN6_DEFAULT (0x33C)
#define HI_VI_AF_IIR1_GAIN6_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_vi_af_iir1_gain6_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12038, 25, 16, data);

}
static __inline HI_U32 hi_vi_af_iir1_gain6_read(HI_VOID) {

    return COMPLEMENT_ALIGN32(REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12038, 25, 16), 9);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_plg
// ------------------------------------------------------------------------------ //
//  iir0 pre load gain
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_PLG_DEFAULT (0x96)
#define HI_VI_AF_IIR0_PLG_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_iir0_plg_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x1203c, 7, 0, data);

}
static __inline HI_U32 hi_vi_af_iir0_plg_read(HI_VOID) {

    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x1203c, 7, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_pls
// ------------------------------------------------------------------------------ //
//  iir0 pre load shift
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_PLS_DEFAULT (0x3)
#define HI_VI_AF_IIR0_PLS_DATASIZE (3)

// args: data (3-bit)
static __inline HI_VOID hi_vi_af_iir0_pls_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x1203c, 10, 8, data);

}
static __inline HI_U32 hi_vi_af_iir0_pls_read(HI_VOID) {

    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x1203c, 10, 8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_plg
// ------------------------------------------------------------------------------ //
//  iir1 pre load gain
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_PLG_DEFAULT (0x99)
#define HI_VI_AF_IIR1_PLG_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_iir1_plg_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x1203c, 23, 16, data);

}
static __inline HI_U32 hi_vi_af_iir1_plg_read(HI_VOID) {

    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x1203c, 23, 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_pls
// ------------------------------------------------------------------------------ //
//  iir1 pre load shift
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_PLS_DEFAULT (0x3)
#define HI_VI_AF_IIR1_PLS_DATASIZE (3)

// args: data (3-bit)
static __inline HI_VOID hi_vi_af_iir1_pls_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x1203c, 26, 24, data);
}
static __inline HI_U32 hi_vi_af_iir1_pls_read(HI_VOID) {

    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x1203c, 26, 24);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_shift0
// ------------------------------------------------------------------------------ //
//  af iir0 shift0
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_SHIFT0_DEFAULT (0x7)
#define HI_VI_AF_IIR0_SHIFT0_DATASIZE (3)

// args: data (3-bit)
static __inline HI_VOID hi_vi_af_iir0_shift0_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12040, 2, 0, data);
}
static __inline HI_U32 hi_vi_af_iir0_shift0_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12040, 2, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_shift1
// ------------------------------------------------------------------------------ //
//  af iir0 shift1
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_SHIFT1_DEFAULT (0x2)
#define HI_VI_AF_IIR0_SHIFT1_DATASIZE (3)

// args: data (3-bit)
static __inline HI_VOID hi_vi_af_iir0_shift1_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12040, 6, 4, data);
}
static __inline HI_U32 hi_vi_af_iir0_shift1_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12040, 6, 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_shift2
// ------------------------------------------------------------------------------ //
//  af iir0 shift2
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_SHIFT2_DEFAULT (0x0)
#define HI_VI_AF_IIR0_SHIFT2_DATASIZE (3)

// args: data (3-bit)
static __inline HI_VOID hi_vi_af_iir0_shift2_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12040, 10, 8, data);
}
static __inline HI_U32 hi_vi_af_iir0_shift2_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12040, 10, 8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_shift3
// ------------------------------------------------------------------------------ //
//  af iir0 shift3
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_SHIFT3_DEFAULT (0x1)
#define HI_VI_AF_IIR0_SHIFT3_DATASIZE (4)

// args: data (3-bit)
static __inline HI_VOID hi_vi_af_iir0_shift3_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12040, 14, 12, data);
}
static __inline HI_U32 hi_vi_af_iir0_shift3_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12040, 14, 12);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_shift0
// ------------------------------------------------------------------------------ //
//  af iir1 shift0
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_SHIFT0_DEFAULT (0x7)
#define HI_VI_AF_IIR1_SHIFT0_DATASIZE (3)

// args: data (3-bit)
static __inline HI_VOID hi_vi_af_iir1_shift0_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12040, 18, 16, data);
}
static __inline HI_U32 hi_vi_af_iir1_shift0_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12040, 18, 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_shift1
// ------------------------------------------------------------------------------ //
//  af iir1 shift1
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_SHIFT1_DEFAULT (0x2)
#define HI_VI_AF_IIR1_SHIFT1_DATASIZE (3)

// args: data (3-bit)
static __inline HI_VOID hi_vi_af_iir1_shift1_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12040, 22, 20, data);
}
static __inline HI_U32 hi_vi_af_iir1_shift1_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12040, 22, 20);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_shift2
// ------------------------------------------------------------------------------ //
//  af iir1 shift2
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_SHIFT2_DEFAULT (0x0)
#define HI_VI_AF_IIR1_SHIFT2_DATASIZE (3)

// args: data (3-bit)
static __inline HI_VOID hi_vi_af_iir1_shift2_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12040, 26, 24, data);
}
static __inline HI_U32 hi_vi_af_iir1_shift2_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12040, 26, 24);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_shift3
// ------------------------------------------------------------------------------ //
//  af iir1 shift3
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_SHIFT3_DEFAULT (0x1)
#define HI_VI_AF_IIR1_SHIFT3_DATASIZE (4)

// args: data (3-bit)
static __inline HI_VOID hi_vi_af_iir1_shift3_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12040, 30, 28, data);
}
static __inline HI_U32 hi_vi_af_iir1_shift3_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12040, 30, 28);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir0_h0
// ------------------------------------------------------------------------------ //
//  af fir0 h0
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR0_H0_DEFAULT (0x30)
#define HI_VI_AF_FIR0_H0_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_vi_af_fir0_h0_write(HI_U32 data) {

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12050, 5, 0, data);
}
static __inline HI_U32 hi_vi_af_fir0_h0_read(HI_VOID) {

    return COMPLEMENT_ALIGN32(REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12050, 5, 0), 5);
}


// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir1_h0
// ------------------------------------------------------------------------------ //
//  af fir1 h0
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR1_H0_DEFAULT (0x30)
#define HI_VI_AF_FIR1_H0_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_vi_af_fir1_h0_write(HI_U32 data) {

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12050, 21, 16, data);
}
static __inline HI_U32 hi_vi_af_fir1_h0_read(HI_VOID) {

    return COMPLEMENT_ALIGN32(REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12050, 21, 16), 5);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir0_h1
// ------------------------------------------------------------------------------ //
//  af fir0 h1
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR0_H1_DEFAULT (0x2B)
#define HI_VI_AF_FIR0_H1_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_vi_af_fir0_h1_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12054, 5, 0, data);
}
static __inline HI_U32 hi_vi_af_fir0_h1_read(HI_VOID) {

    return COMPLEMENT_ALIGN32(REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12054, 5, 0), 5);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir1_h1
// ------------------------------------------------------------------------------ //
//  af fir1 h1
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR1_H1_DEFAULT (0x15)
#define HI_VI_AF_FIR1_H1_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_vi_af_fir1_h1_write(HI_U32 data) {

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12054, 21, 16, data);
}
static __inline HI_U32 hi_vi_af_fir1_h1_read(HI_VOID) {

    return COMPLEMENT_ALIGN32(REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12054, 21, 16), 5);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir0_h2
// ------------------------------------------------------------------------------ //
//  af fir0 h2
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR0_H2_DEFAULT (0x0)
#define HI_VI_AF_FIR0_H2_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_vi_af_fir0_h2_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12058, 5, 0, data);
}
static __inline HI_U32 hi_vi_af_fir0_h2_read(HI_VOID) {

    return COMPLEMENT_ALIGN32(REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12058, 5, 0), 5);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir1_h2
// ------------------------------------------------------------------------------ //
//  af fir1 h2
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR1_H2_DEFAULT (0)
#define HI_VI_AF_FIR1_H2_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_vi_af_fir1_h2_write(HI_U32 data) {

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12058, 21, 16, data);
}
static __inline HI_U32 hi_vi_af_fir1_h2_read(HI_VOID) {

    return COMPLEMENT_ALIGN32(REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12058, 21, 16), 5);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir0_h3
// ------------------------------------------------------------------------------ //
//  af fir0 h3
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR0_H3_DEFAULT (0x15)
#define HI_VI_AF_FIR0_H3_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_vi_af_fir0_h3_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x1205c, 5, 0, data);

}
static __inline HI_U32 hi_vi_af_fir0_h3_read(HI_VOID) {

    return COMPLEMENT_ALIGN32(REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x1205c, 5, 0), 5);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir1_h3
// ------------------------------------------------------------------------------ //
//  af fir1 h3
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR1_H3_DEFAULT (0x2B)
#define HI_VI_AF_FIR1_H3_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_vi_af_fir1_h3_write(HI_U32 data) {  

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x1205c, 21, 16, data);
}
static __inline HI_U32 hi_vi_af_fir1_h3_read(HI_VOID) {

    return COMPLEMENT_ALIGN32(REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x1205c, 21, 16), 5);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir0_h4
// ------------------------------------------------------------------------------ //
//  af fir0 h4
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR0_H4_DEFAULT (0x10)
#define HI_VI_AF_FIR0_H4_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_vi_af_fir0_h4_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12060, 5, 0, data);
}
static __inline HI_U32 hi_vi_af_fir0_h4_read(HI_VOID) {

    return COMPLEMENT_ALIGN32(REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12060, 5, 0), 5);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir1_h4
// ------------------------------------------------------------------------------ //
//  af fir1 h4
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR1_H4_DEFAULT (0x10)
#define HI_VI_AF_FIR1_H4_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_vi_af_fir1_h4_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12060, 21, 16, data);
}
static __inline HI_U32 hi_vi_af_fir1_h4_read(HI_VOID) {

    return COMPLEMENT_ALIGN32(REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12060, 21, 16), 5);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_acc_shift_h0
// ------------------------------------------------------------------------------ //
//  af iir0 acc shift h
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_ACC_SHIFT_H0_DEFAULT (0)
#define HI_VI_AF_ACC_SHIFT_H0_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_acc_shift_h0_write(HI_U32 data) {

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12078, 3, 0, data);
}
static __inline HI_U32 hi_vi_af_acc_shift_h0_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12078, 3, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_acc_shift_h1
// ------------------------------------------------------------------------------ //
//  af iir1 acc shift h
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_ACC_SHIFT_H1_DEFAULT (0)
#define HI_VI_AF_ACC_SHIFT_H1_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_acc_shift_h1_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12078, 7, 4, data);
}
static __inline HI_U32 hi_vi_af_acc_shift_h1_read(HI_VOID) {

    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12078, 7, 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_acc_shift_V0
// ------------------------------------------------------------------------------ //
//  af fir0 acc shift V
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_ACC_SHIFT_V0_DEFAULT (2)
#define HI_VI_AF_ACC_SHIFT_V0_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_acc_shift_v0_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12078, 11, 8, data);
}
static __inline HI_U32 hi_vi_af_acc_shift_v0_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12078, 11, 8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_acc_shift_V1
// ------------------------------------------------------------------------------ //
//  af fir1 acc shift V
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_ACC_SHIFT_V1_DEFAULT (0)
#define HI_VI_AF_ACC_SHIFT_V1_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_acc_shift_v1_write(HI_U32 data) {

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12078, 15, 12, data);
}
static __inline HI_U32 hi_vi_af_acc_shift_v1_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12078, 15, 12);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_acc_shift_y
// ------------------------------------------------------------------------------ //
//  af acc shift y
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_ACC_SHIFT_Y_DEFAULT (2)
#define HI_VI_AF_ACC_SHIFT_Y_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_acc_shift_y_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12078, 19, 16, data);
}
static __inline HI_U32 hi_vi_af_acc_shift_y_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12078, 19, 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_cnt_shift_h0
// ------------------------------------------------------------------------------ //
//  af iir0 cnt shift h
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_CNT_SHIFT_H0_DEFAULT (0)
#define HI_VI_AF_CNT_SHIFT_H0_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_cnt_shift_h0_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x1207c, 3, 0, data);
}
static __inline HI_U32 hi_vi_af_cnt_shift_h0_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x1207c, 3, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_cnt_shift_h1
// ------------------------------------------------------------------------------ //
//  af iir1 cnt shift h
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_CNT_SHIFT_H1_DEFAULT (0)
#define HI_VI_AF_CNT_SHIFT_H1_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_cnt_shift_h1_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x1207c, 7, 4, data);
}
static __inline HI_U32 hi_vi_af_cnt_shift_h1_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x1207c, 7, 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_cnt_shift_v0
// ------------------------------------------------------------------------------ //
//  af fir0 cnt shift v
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_CNT_SHIFT_V0_DEFAULT (2)
#define HI_VI_AF_CNT_SHIFT_V0_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_cnt_shift_v0_write(HI_U32 data) {

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x1207c, 11, 8, data);
}
static __inline HI_U32 hi_vi_af_cnt_shift_v0_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x1207c, 11, 8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_cnt_shift_v1
// ------------------------------------------------------------------------------ //
//  af fir1 cnt shift v
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_CNT_SHIFT_V1_DEFAULT (0)
#define HI_VI_AF_CNT_SHIFT_V1_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_cnt_shift_v1_write(HI_U32 data) {

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x1207c, 15, 12, data);
}
static __inline HI_U32 hi_vi_af_cnt_shift_v1_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x1207c, 15, 12);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_cnt_shift_highluma
// ------------------------------------------------------------------------------ //
//  af high luma cnt shift
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_CNT_SHIFT_HIGHLUMA_DEFAULT (0)
#define HI_VI_AF_CNT_SHIFT_HIGHLUMA_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_cnt_shift_highluma_write(HI_U32 data) {

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x1207c, 19, 16, data);
}
static __inline HI_U32 hi_vi_af_cnt_shift_highluma_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x1207c, 19, 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_update_mode
// ------------------------------------------------------------------------------ //
//  af update mode
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_UPDATE_MODE_DEFAULT (0x1)
#define HI_VI_AF_UPDATE_MODE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_update_mode_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x120E4, 0, 0, data);

}
static __inline HI_U32 hi_vi_af_update_mode_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x120E4, 0, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_update
// ------------------------------------------------------------------------------ //
//  af update
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_UPDATE_DEFAULT (0x0)
#define HI_VI_AF_UPDATE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_update_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x120EC, 0, 0, data);

}
static __inline HI_U32 hi_vi_af_update_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x120EC, 0, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_hsize
// ------------------------------------------------------------------------------ //
//  af image horiz size
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_HSIZE_DEFAULT (0x77f)
#define HI_VI_AF_HSIZE_DATASIZE (13)

// args: data (13-bit)
static __inline HI_VOID hi_vi_af_hsize_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x120f0, 12, 0, data);

}
static __inline HI_U32 hi_vi_af_hsize_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x120f0, 12, 0);
}


// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_vsize
// ------------------------------------------------------------------------------ //
//  af image verti size
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_VSIZE_DEFAULT (0x437)
#define HI_VI_AF_VSIZE_DATASIZE (13)

// args: data (13-bit)
static __inline HI_VOID hi_vi_af_vsize_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x120f0, 28, 16, data);

}
static __inline HI_U32 hi_vi_af_vsize_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x120f0, 28, 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_ldg_th_l
// ------------------------------------------------------------------------------ //
//  af iir0 ldg threshold low
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_LDG_TH_L_DEFAULT (0x0)
#define HI_VI_AF_IIR0_LDG_TH_L_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_iir0_ldg_th_l_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12100, 7, 0, data);

}
static __inline HI_U32 hi_vi_af_iir0_ldg_th_l_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12100, 7, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_ldg_th_h
// ------------------------------------------------------------------------------ //
//  af iir0 ldg threshold high
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_LDG_TH_H_DEFAULT (0x0)
#define HI_VI_AF_IIR0_LDG_TH_H_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_iir0_ldg_th_h_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12100, 15, 8, data);

}
static __inline HI_U32 hi_vi_af_iir0_ldg_th_h_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12100, 15, 8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_ldg_th_l
// ------------------------------------------------------------------------------ //
//  af iir1 ldg threshold low
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_LDG_TH_L_DEFAULT (0x0)
#define HI_VI_AF_IIR1_LDG_TH_L_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_iir1_ldg_th_l_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12100, 23, 16, data);

}
static __inline HI_U32 hi_vi_af_iir1_ldg_th_l_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12100, 23, 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_ldg_th_h
// ------------------------------------------------------------------------------ //
//  af iir1 ldg threshold high
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_LDG_TH_H_DEFAULT (0x0)
#define HI_VI_AF_IIR1_LDG_TH_H_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_iir1_ldg_th_h_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12100, 31, 24, data);

}
static __inline HI_U32 hi_vi_af_iir1_ldg_th_h_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12100, 31, 24);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_ldg_gain_l
// ------------------------------------------------------------------------------ //
//  af iir0 ldg gain low
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_LDG_GAIN_L_DEFAULT (0x0)
#define HI_VI_AF_IIR0_LDG_GAIN_L_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_iir0_ldg_gain_l_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12104, 7, 0, data);

}
static __inline HI_U32 hi_vi_af_iir0_ldg_gain_l_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12104, 7, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_ldg_gain_h
// ------------------------------------------------------------------------------ //
//  af iir0 ldg gain high
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_LDG_GAIN_H_DEFAULT (0x0)
#define HI_VI_AF_IIR0_LDG_GAIN_H_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_iir0_ldg_gain_h_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12104, 15, 8, data);

}
static __inline HI_U32 hi_vi_af_iir0_ldg_gain_h_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12104, 15, 8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_ldg_gain_l
// ------------------------------------------------------------------------------ //
//  af iir1 ldg gain low
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_LDG_GAIN_L_DEFAULT (0x0)
#define HI_VI_AF_IIR1_LDG_GAIN_L_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_iir1_ldg_gain_l_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12104, 23, 16, data);

}
static __inline HI_U32 hi_vi_af_iir1_ldg_gain_l_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12104, 23, 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_ldg_gain_h
// ------------------------------------------------------------------------------ //
//  af iir1 ldg gain high
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_LDG_GAIN_H_DEFAULT (0x0)
#define HI_VI_AF_IIR1_LDG_GAIN_H_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_iir1_ldg_gain_h_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12104, 31, 24, data);

}
static __inline HI_U32 hi_vi_af_iir1_ldg_gain_h_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12104, 31, 24);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_ldg_slp_l
// ------------------------------------------------------------------------------ //
//  af iir0 ldg slope low
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_LDG_SLP_L_DEFAULT (0x0)
#define HI_VI_AF_IIR0_LDG_SLP_L_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_iir0_ldg_slp_l_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12108, 3, 0, data);

}
static __inline HI_U32 hi_vi_af_iir0_ldg_slp_l_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12108, 3, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_ldg_slp_h
// ------------------------------------------------------------------------------ //
//  af iir0 ldg slope high
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_LDG_SLP_H_DEFAULT (0x0)
#define HI_VI_AF_IIR0_LDG_SLP_H_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_iir0_ldg_slp_h_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12108, 7, 4, data);

}
static __inline HI_U32 hi_vi_af_iir0_ldg_slp_h_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12108, 7, 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_ldg_slp_l
// ------------------------------------------------------------------------------ //
//  af iir1 ldg slope low
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_LDG_SLP_L_DEFAULT (0x0)
#define HI_VI_AF_IIR1_LDG_SLP_L_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_iir1_ldg_slp_l_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12108, 19, 16, data);

}
static __inline HI_U32 hi_vi_af_iir1_ldg_slp_l_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12108, 19, 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_ldg_slp_h
// ------------------------------------------------------------------------------ //
//  af iir1 ldg slope high
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_LDG_SLP_H_DEFAULT (0x0)
#define HI_VI_AF_IIR1_LDG_SLP_H_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_iir1_ldg_slp_h_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12108, 23, 20, data);

}
static __inline HI_U32 hi_vi_af_iir1_ldg_slp_h_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12108, 23, 20);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_dilate
// ------------------------------------------------------------------------------ //
//  af iir0 dilate
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_DILATE_DEFAULT (0x0)
#define HI_VI_AF_IIR0_DILATE_DATASIZE (3)

// args: data (3-bit)
static __inline HI_VOID hi_vi_af_iir0_dilate_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x1210c, 2, 0, data);

}
static __inline HI_U32 hi_vi_af_iir0_dilate_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x1210c, 2, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_dilate
// ------------------------------------------------------------------------------ //
//  af iir1 dilate
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_DILATE_DEFAULT (0x0)
#define HI_VI_AF_IIR1_DILATE_DATASIZE (3)

// args: data (3-bit)
static __inline HI_VOID hi_vi_af_iir1_dilate_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x1210c, 10, 8, data);

}
static __inline HI_U32 hi_vi_af_iir1_dilate_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x1210c, 10, 8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir0_ldg_th_l
// ------------------------------------------------------------------------------ //
//  af fir0 ldg threshold low
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR0_LDG_TH_L_DEFAULT (0x0)
#define HI_VI_AF_FIR0_LDG_TH_L_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_fir0_ldg_th_l_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12110, 7, 0, data);

}
static __inline HI_U32 hi_vi_af_fir0_ldg_th_l_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12110, 7, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir0_ldg_th_h
// ------------------------------------------------------------------------------ //
//  af fir0 ldg threshold high
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR0_LDG_TH_H_DEFAULT (0x0)
#define HI_VI_AF_FIR0_LDG_TH_H_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_fir0_ldg_th_h_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12110, 15, 8, data);

}
static __inline HI_U32 hi_vi_af_fir0_ldg_th_h_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12110, 15, 8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir1_ldg_th_l
// ------------------------------------------------------------------------------ //
//  af fir1 ldg threshold low
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR1_LDG_TH_L_DEFAULT (0x0)
#define HI_VI_AF_FIR1_LDG_TH_L_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_fir1_ldg_th_l_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12110, 23, 16, data);

}
static __inline HI_U32 hi_vi_af_fir1_ldg_th_l_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12110, 23, 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir1_ldg_th_h
// ------------------------------------------------------------------------------ //
//  af fir1 ldg threshold high
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR1_LDG_TH_H_DEFAULT (0x0)
#define HI_VI_AF_FIR1_LDG_TH_H_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_fir1_ldg_th_h_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12110, 31, 24, data);

}
static __inline HI_U32 hi_vi_af_fir1_ldg_th_h_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12110, 31, 24);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir0_ldg_gain_l
// ------------------------------------------------------------------------------ //
//  af fir0 ldg gain low
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR0_LDG_GAIN_L_DEFAULT (0x0)
#define HI_VI_AF_FIR0_LDG_GAIN_L_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_fir0_ldg_gain_l_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12114, 7, 0, data);

}
static __inline HI_U32 hi_vi_af_fir0_ldg_gain_l_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12114, 7, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir0_ldg_gain_h
// ------------------------------------------------------------------------------ //
//  af fir0 ldg gain high
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR0_LDG_GAIN_H_DEFAULT (0x0)
#define HI_VI_AF_FIR0_LDG_GAIN_H_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_fir0_ldg_gain_h_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12114, 15, 8, data);

}
static __inline HI_U32 hi_vi_af_fir0_ldg_gain_h_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12114, 15, 8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir1_ldg_gain_l
// ------------------------------------------------------------------------------ //
//  af fir1 ldg gain low
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_LDG_GAIN_L_DEFAULT (0x0)
#define HI_VI_AF_IIR1_LDG_GAIN_L_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_fir1_ldg_gain_l_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12114, 23, 16, data);

}
static __inline HI_U32 hi_vi_af_fir1_ldg_gain_l_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12114, 23, 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir1_ldg_gain_h
// ------------------------------------------------------------------------------ //
//  af iir1 ldg gain high
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR1_LDG_GAIN_H_DEFAULT (0x0)
#define HI_VI_AF_FIR1_LDG_GAIN_H_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_fir1_ldg_gain_h_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12114, 31, 24, data);

}
static __inline HI_U32 hi_vi_af_fir1_ldg_gain_h_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12114, 31, 24);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir0_ldg_slp_l
// ------------------------------------------------------------------------------ //
//  af fir0 ldg slope low
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR0_LDG_SLP_L_DEFAULT (0x0)
#define HI_VI_AF_FIR0_LDG_SLP_L_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_fir0_ldg_slp_l_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12118, 3, 0, data);

}
static __inline HI_U32 hi_vi_af_fir0_ldg_slp_l_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12118, 3, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir0_ldg_slp_h
// ------------------------------------------------------------------------------ //
//  af fir0 ldg slope high
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR0_LDG_SLP_H_DEFAULT (0x0)
#define HI_VI_AF_FIR0_LDG_SLP_H_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_fir0_ldg_slp_h_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12118, 7, 4, data);

}
static __inline HI_U32 hi_vi_af_fir0_ldg_slp_h_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12118, 7, 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir1_ldg_slp_l
// ------------------------------------------------------------------------------ //
//  af fir1 ldg slope low
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR1_LDG_SLP_L_DEFAULT (0x0)
#define HI_VI_AF_FIR1_LDG_SLP_L_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_fir1_ldg_slp_l_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12118, 19, 16, data);

}
static __inline HI_U32 hi_vi_af_fir1_ldg_slp_l_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12118, 19, 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir1_ldg_slp_h
// ------------------------------------------------------------------------------ //
//  af fir1 ldg slope high
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR1_LDG_SLP_H_DEFAULT (0x0)
#define HI_VI_AF_FIR1_LDG_SLP_H_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_fir1_ldg_slp_h_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12118, 23, 20, data);

}
static __inline HI_U32 hi_vi_af_fir1_ldg_slp_h_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12118, 23, 20);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_cor_th
// ------------------------------------------------------------------------------ //
//  af iir0 coring threshold
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_COR_TH_DEFAULT (0x0)
#define HI_VI_AF_IIR0_COR_TH_DATASIZE (11)

// args: data (11-bit)
static __inline HI_VOID hi_vi_af_iir0_cor_th_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12120, 10, 0, data);

}
static __inline HI_U32 hi_vi_af_iir0_cor_th_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12120, 10, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_cor_th
// ------------------------------------------------------------------------------ //
//  af iir1 coring threshold
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_COR_TH_DEFAULT (0x0)
#define HI_VI_AF_IIR1_COR_TH_DATASIZE (11)

// args: data (11-bit)
static __inline HI_VOID hi_vi_af_iir1_cor_th_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12120, 26, 16, data);

}
static __inline HI_U32 hi_vi_af_iir1_cor_th_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12120, 26, 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_cor_peak
// ------------------------------------------------------------------------------ //
//  af iir0 coring peak
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_COR_PEAK_DEFAULT (0x0)
#define HI_VI_AF_IIR0_COR_PEAK_DATASIZE (11)

// args: data (11-bit)
static __inline HI_VOID hi_vi_af_iir0_cor_peak_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12124, 10, 0, data);

}
static __inline HI_U32 hi_vi_af_iir0_cor_peak_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12124, 10, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_cor_peak
// ------------------------------------------------------------------------------ //
//  af iir1 coring peak
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_COR_PEAK_DEFAULT (0x0)
#define HI_VI_AF_IIR1_COR_PEAK_DATASIZE (11)

// args: data (11-bit)
static __inline HI_VOID hi_vi_af_iir1_cor_peak_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12124, 26, 16, data);

}
static __inline HI_U32 hi_vi_af_iir1_cor_peak_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12124, 26, 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_cor_slp
// ------------------------------------------------------------------------------ //
//  af iir0 coring slope
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_COR_SLP_DEFAULT (0x0)
#define HI_VI_AF_IIR0_COR_SLP_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_iir0_cor_slp_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12128, 3, 0, data);

}
static __inline HI_U32 hi_vi_af_iir0_cor_slp_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12128, 3, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_cor_slp
// ------------------------------------------------------------------------------ //
//  af iir1 coring slope
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_COR_SLP_DEFAULT (0x0)
#define HI_VI_AF_IIR1_COR_SLP_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_iir1_cor_slp_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12128, 11, 8, data);

}
static __inline HI_U32 hi_vi_af_iir1_cor_slp_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12128, 11, 8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir0_cor_th
// ------------------------------------------------------------------------------ //
//  af fir0 coring threshold
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR0_COR_TH_DEFAULT (0x0)
#define HI_VI_AF_FIR0_COR_TH_DATASIZE (11)

// args: data (11-bit)
static __inline HI_VOID hi_vi_af_fir0_cor_th_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12130, 10, 0, data);

}
static __inline HI_U32 hi_vi_af_fir0_cor_th_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12130, 10, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir1_cor_th
// ------------------------------------------------------------------------------ //
//  af fir1 coring threshold
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR1_COR_TH_DEFAULT (0x0)
#define HI_VI_AF_FIR1_COR_TH_DATASIZE (11)

// args: data (11-bit)
static __inline HI_VOID hi_vi_af_fir1_cor_th_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12130, 26, 16, data);

}
static __inline HI_U32 hi_vi_af_fir1_cor_th_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12130, 26, 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir0_cor_peak
// ------------------------------------------------------------------------------ //
//  af fir0 coring peak
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR0_COR_PEAK_DEFAULT (0x0)
#define HI_VI_AF_FIR0_COR_PEAK_DATASIZE (11)

// args: data (11-bit)
static __inline HI_VOID hi_vi_af_fir0_cor_peak_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12134, 10, 0, data);

}
static __inline HI_U32 hi_vi_af_fir0_cor_peak_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12134, 10, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir1_cor_peak
// ------------------------------------------------------------------------------ //
//  af fir1 coring peak
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR1_COR_PEAK_DEFAULT (0x0)
#define HI_VI_AF_FIR1_COR_PEAK_DATASIZE (11)

// args: data (11-bit)
static __inline HI_VOID hi_vi_af_fir1_cor_peak_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12134, 26, 16, data);

}
static __inline HI_U32 hi_vi_af_fir1_cor_peak_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12134, 26, 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir0_cor_slp
// ------------------------------------------------------------------------------ //
//  af fir0 coring slope
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR0_COR_SLP_DEFAULT (0x0)
#define HI_VI_AF_FIR0_COR_SLP_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_fir0_cor_slp_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12138, 3, 0, data);

}
static __inline HI_U32 hi_vi_af_fir0_cor_slp_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12138, 3, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir1_cor_slp
// ------------------------------------------------------------------------------ //
//  af fir1 coring slope
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR1_COR_SLP_DEFAULT (0x0)
#define HI_VI_AF_FIR1_COR_SLP_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_fir1_cor_slp_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12138, 11, 8, data);

}
static __inline HI_U32 hi_vi_af_fir1_cor_slp_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12138, 11, 8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_hiluma_th
// ------------------------------------------------------------------------------ //
//  af high luma cnt threshold
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_HIGH_LUMA_TH_DEFAULT (0x0)
#define HI_VI_AF_HIGH_LUMA_TH_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_highluma_th_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12140, 7, 0, data);

}
static __inline HI_U32 hi_vi_af_highluma_th_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12140, 7, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_offset_gr
// ------------------------------------------------------------------------------ //
//  af offset gr when measure at raw domain
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_OFFSET_GR_DEFAULT (0x0)
#define HI_VI_AF_OFFSET_GR_DATASIZE (14)

// args: data (14-bit)
static __inline HI_VOID hi_vi_af_offset_gr_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12144, 13, 0, data);

}
static __inline HI_U32 hi_vi_af_offset_gr_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12144, 13, 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_offset_gb
// ------------------------------------------------------------------------------ //
//  af offset gb when measure at raw domain
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_OFFSET_GB_DEFAULT (0x0)
#define HI_VI_AF_OFFSET_GB_DATASIZE (14)

// args: data (14-bit)
static __inline HI_VOID hi_vi_af_offset_gb_write(HI_U32 data) { 

    REG_BITS_MODIFY_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12144, 29, 16, data);

}
static __inline HI_U32 hi_vi_af_offset_gb_read(HI_VOID) {
    
    return REG_BITS_READ_TYPE1(IOWR_32DIRECT_VI, IORD_32DIRECT_VI, 0x12144, 29, 16);
}


/*****************************************************
MODULE        -----ISP_CROP_CFG-----
MODULE        -----ISP_CROP_CFG-----
MODULE        -----ISP_CROP_CFG-----
*****************************************************/
/*****************************************************
ISP_CROP_CFG:			ISP_CROP_CFG
0x205a0800: 				31:01          reserved
                                          00:00          en
*****************************************************/
static __inline HI_VOID hi_isp_crop_en_write(HI_U8 ispDev, HI_U8 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CROP0_CFG ISP_CROP0_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CROP0_CFG.u32 = pstIspReg->ISP_CROP0_CFG.u32;
	ISP_CROP0_CFG.bits.n0_en = data;
	pstIspReg->ISP_CROP0_CFG.u32 = ISP_CROP0_CFG.u32;
}

static __inline HI_U8 hi_isp_crop_en_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CROP0_CFG ISP_CROP0_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CROP0_CFG.u32 = pstIspReg->ISP_CROP0_CFG.u32;
	return ISP_CROP0_CFG.bits.n0_en;
}

/*****************************************************
ISP_CROP0_START:			ISP_CROP_CFG
0x205a0808: 				31:29          reserved
						28:16          y_start
						15:13          reserved
                                          12:00          x_start
*****************************************************/
static __inline HI_VOID hi_isp_crop_x_start_write(HI_U8 ispDev, HI_U16 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CROP0_START ISP_CROP0_START;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CROP0_START.u32 = pstIspReg->ISP_CROP0_START.u32;
	ISP_CROP0_START.bits.x_start = data;
	pstIspReg->ISP_CROP0_START.u32 = ISP_CROP0_START.u32;
}

static __inline HI_U16 hi_isp_crop_x_start_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CROP0_START ISP_CROP0_START;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CROP0_START.u32 = pstIspReg->ISP_CROP0_START.u32;
	return ISP_CROP0_START.bits.x_start;
}

static __inline HI_VOID hi_isp_crop_y_start_write(HI_U8 ispDev, HI_U16 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CROP0_START ISP_CROP0_START;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CROP0_START.u32 = pstIspReg->ISP_CROP0_START.u32;
	ISP_CROP0_START.bits.y_start = data;
	pstIspReg->ISP_CROP0_START.u32 = ISP_CROP0_START.u32;
}

static __inline HI_U16 hi_isp_crop_y_start_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CROP0_START ISP_CROP0_START;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CROP0_START.u32 = pstIspReg->ISP_CROP0_START.u32;
	return ISP_CROP0_START.bits.y_start;
}

/*****************************************************
ISP_CROP0_SIZE:			ISP_CROP_CFG
0x205a080c: 				31:29          reserved
						28:16          height
						15:13          reserved
                                          12:00          width
*****************************************************/
static __inline HI_VOID hi_isp_crop_width_write(HI_U8 ispDev, HI_U16 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CROP0_SIZE ISP_CROP0_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CROP0_SIZE.u32 = pstIspReg->ISP_CROP0_SIZE.u32;
	ISP_CROP0_SIZE.bits.width = data;
	pstIspReg->ISP_CROP0_SIZE.u32 = ISP_CROP0_SIZE.u32;
}

static __inline HI_U16 hi_isp_crop_width_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CROP0_SIZE ISP_CROP0_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CROP0_SIZE.u32 = pstIspReg->ISP_CROP0_SIZE.u32;
	return ISP_CROP0_SIZE.bits.width;
}

static __inline HI_VOID hi_isp_crop_height_write(HI_U8 ispDev, HI_U16 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CROP0_SIZE ISP_CROP0_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CROP0_SIZE.u32 = pstIspReg->ISP_CROP0_SIZE.u32;
	ISP_CROP0_SIZE.bits.height = data;
	pstIspReg->ISP_CROP0_SIZE.u32 = ISP_CROP0_SIZE.u32;
}

static __inline HI_U16 hi_isp_crop_height_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CROP0_SIZE ISP_CROP0_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_CROP0_SIZE.u32 = pstIspReg->ISP_CROP0_SIZE.u32;
	return ISP_CROP0_SIZE.bits.height;
}



/*****************************************************
MODULE        -----WDR-----
MODULE        -----WDR-----
MODULE        -----WDR-----
*****************************************************/
/*****************************************************
ISP_WDR_CFG:		 WDR        
0x205a6000: 			31:01 	reserved
					00:00     en    WDRenable
								
								
*****************************************************/
static __inline HI_VOID hi_isp_wdr_enable_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_CFG ISP_WDR_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_CFG.u32 = pstIspReg->ISP_WDR_CFG.u32;
	ISP_WDR_CFG.bits.en = data;
	pstIspReg->ISP_WDR_CFG.u32 = ISP_WDR_CFG.u32;
}

static __inline HI_U8 hi_isp_wdr_enable_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_CFG ISP_WDR_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_CFG.u32 = pstIspReg->ISP_WDR_CFG.u32;
	return ISP_WDR_CFG.bits.en;
}


/*****************************************************
ISP_WDR_INBLC1:	
0x205a6010: 		31:31 	reserved
					30:16   f0_inblc
					15:15   reserved
					14:00   f1_inblc
*****************************************************/
static __inline HI_VOID hi_isp_wdr_f1_inblc_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_INBLC ISP_WDR_INBLC;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_INBLC.u32 = pstIspReg->ISP_WDR_INBLC.u32;
	ISP_WDR_INBLC.bits.f1_inblc = data;
	pstIspReg->ISP_WDR_INBLC.u32 = ISP_WDR_INBLC.u32;
}

static __inline HI_U16 hi_isp_wdr_f1_inblc_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_INBLC ISP_WDR_INBLC;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_INBLC.u32 = pstIspReg->ISP_WDR_INBLC.u32;
	return ISP_WDR_INBLC.bits.f1_inblc;
}

static __inline HI_VOID hi_isp_wdr_f0_inblc_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_INBLC ISP_WDR_INBLC;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_INBLC.u32 = pstIspReg->ISP_WDR_INBLC.u32;
	ISP_WDR_INBLC.bits.f0_inblc = data;
	pstIspReg->ISP_WDR_INBLC.u32 = ISP_WDR_INBLC.u32;
}

static __inline HI_U16 hi_isp_wdr_f0_inblc_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_INBLC ISP_WDR_INBLC;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_INBLC.u32 = pstIspReg->ISP_WDR_INBLC.u32;
	return ISP_WDR_INBLC.bits.f0_inblc;
}

/*****************************************************
ISP_WDR_OUTBLC:	
0x205a6014: 		31:15 	reserved
					19:00     outblc
*****************************************************/
static __inline HI_VOID hi_isp_wdr_outblc_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_OUTBLC ISP_WDR_OUTBLC;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_OUTBLC.u32 = pstIspReg->ISP_WDR_OUTBLC.u32;
	ISP_WDR_OUTBLC.bits.outblc = data;
	pstIspReg->ISP_WDR_OUTBLC.u32 = ISP_WDR_OUTBLC.u32;
}

static __inline HI_U32 hi_isp_wdr_outblc_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_OUTBLC ISP_WDR_OUTBLC;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_OUTBLC.u32 = pstIspReg->ISP_WDR_OUTBLC.u32;
	return ISP_WDR_OUTBLC.bits.outblc;
}

/*****************************************************
ISP_WDR_EXPOSRATIOS1:	
0x205a6018: 		31:28 	reserved
					25:16	exporatio_r
					15:12	reserved
					11:00   exporatio0
*****************************************************/
static __inline HI_VOID hi_isp_wdr_exporatio0_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_EXPOSRATIOS ISP_WDR_EXPOSRATIOS;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_EXPOSRATIOS.u32 = pstIspReg->ISP_WDR_EXPOSRATIOS.u32;
	ISP_WDR_EXPOSRATIOS.bits.exporatio = data;
	pstIspReg->ISP_WDR_EXPOSRATIOS.u32 = ISP_WDR_EXPOSRATIOS.u32;
}

static __inline HI_U16 hi_isp_wdr_exporatio0_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_EXPOSRATIOS ISP_WDR_EXPOSRATIOS;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_EXPOSRATIOS.u32 = pstIspReg->ISP_WDR_EXPOSRATIOS.u32;
	return ISP_WDR_EXPOSRATIOS.bits.exporatio;
}

static __inline HI_VOID hi_isp_wdr_exporatio_r_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_EXPOSRATIOS ISP_WDR_EXPOSRATIOS;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_EXPOSRATIOS.u32 = pstIspReg->ISP_WDR_EXPOSRATIOS.u32;
	ISP_WDR_EXPOSRATIOS.bits.exporatio_r = data;
	pstIspReg->ISP_WDR_EXPOSRATIOS.u32 = ISP_WDR_EXPOSRATIOS.u32;
}

static __inline HI_U16 hi_isp_wdr_exporatio_r_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_EXPOSRATIOS ISP_WDR_EXPOSRATIOS;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_EXPOSRATIOS.u32 = pstIspReg->ISP_WDR_EXPOSRATIOS.u32;
	return ISP_WDR_EXPOSRATIOS.bits.exporatio_r;
}

/*****************************************************
ISP_WDR_BLCSAVE:	BLC reserve

*****************************************************/
static __inline HI_VOID hi_isp_wdr_bsaveblc_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_BLCSAVE ISP_WDR_BLCSAVE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_BLCSAVE.u32 = pstIspReg->ISP_WDR_BLCSAVE.u32;
	ISP_WDR_BLCSAVE.bits.bsaveblc = data;
	pstIspReg->ISP_WDR_BLCSAVE.u32 = ISP_WDR_BLCSAVE.u32;
}

static __inline HI_U8 hi_isp_wdr_bsaveblc_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_BLCSAVE ISP_WDR_BLCSAVE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_BLCSAVE.u32 = pstIspReg->ISP_WDR_BLCSAVE.u32;
	return ISP_WDR_BLCSAVE.bits.bsaveblc;
}

static __inline HI_VOID hi_isp_wdr_esaveblc_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_BLCSAVE ISP_WDR_BLCSAVE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_BLCSAVE.u32 = pstIspReg->ISP_WDR_BLCSAVE.u32;
	ISP_WDR_BLCSAVE.bits.esaveblc = data;
	pstIspReg->ISP_WDR_BLCSAVE.u32 = ISP_WDR_BLCSAVE.u32;
}

static __inline HI_U8 hi_isp_wdr_esaveblc_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_BLCSAVE ISP_WDR_BLCSAVE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_BLCSAVE.u32 = pstIspReg->ISP_WDR_BLCSAVE.u32;
	return ISP_WDR_BLCSAVE.bits.esaveblc;
}

/*****************************************************
ISP_WDR_ZOOMBLC:	BLC reserve 
0x205a6020: 		31:09 	reserved
					06:00   zoomblc
*****************************************************/
#define HI_ISP_WDR_ZOOMBLC_DEFAULT      (0x01)
static __inline HI_VOID hi_isp_wdr_zoomblc_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_ZOOMBLC ISP_WDR_ZOOMBLC;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_ZOOMBLC.u32 = pstIspReg->ISP_WDR_ZOOMBLC.u32;
	ISP_WDR_ZOOMBLC.bits.zoomblc = data; 
	pstIspReg->ISP_WDR_ZOOMBLC.u32 = ISP_WDR_ZOOMBLC.u32;
}

/*****************************************************
ISP_WDR_EWGTMIX:	BLC reserve 
0x205a6024: 		31:09 	reserved
					20:16   bldrlhfidx
					04:00   bldrclridx
*****************************************************/
#define HI_ISP_WDR_BLDRLHFIDX_DEFAULT   (4) 
static __inline HI_VOID hi_isp_wdr_bldrlhfidx_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_BLENDRATIO ISP_WDR_BLENDRATIO;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_BLENDRATIO.u32 = pstIspReg->ISP_WDR_BLENDRATIO.u32;
	ISP_WDR_BLENDRATIO.bits.bldrlhfidx = data; 
	pstIspReg->ISP_WDR_BLENDRATIO.u32 = ISP_WDR_BLENDRATIO.u32;
}

#define HI_ISP_WDR_BLDRCLRIDX_DEFAULT   (0)   
static __inline HI_VOID hi_isp_wdr_bldrclridx_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_BLENDRATIO ISP_WDR_BLENDRATIO;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_BLENDRATIO.u32 = pstIspReg->ISP_WDR_BLENDRATIO.u32;
	ISP_WDR_BLENDRATIO.bits.bldrclridx = data; 
	pstIspReg->ISP_WDR_BLENDRATIO.u32 = ISP_WDR_BLENDRATIO.u32;
}

/*****************************************************
ISP_WDR_FLRGTTH:	BLC reserve 
0x205a6028: 		31:09 	reserved
					29:16   flrgtth_high
					13:00   flrgtth_low
*****************************************************/
#define HI_ISP_WDR_RGTLOWTHFL_DEFAULT   (0x2F00)
static __inline HI_VOID hi_isp_wdr_flrgtth_low_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_FLRGTTH ISP_WDR_FLRGTTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_FLRGTTH.u32 = pstIspReg->ISP_WDR_FLRGTTH.u32;
	ISP_WDR_FLRGTTH.bits.flrgtth_low = data; 
	pstIspReg->ISP_WDR_FLRGTTH.u32 = ISP_WDR_FLRGTTH.u32;
}
static __inline HI_U16 hi_isp_wdr_flrgtth_low_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_FLRGTTH ISP_WDR_FLRGTTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_FLRGTTH.u32 = pstIspReg->ISP_WDR_FLRGTTH.u32;
	return ISP_WDR_FLRGTTH.bits.flrgtth_low; 	
}
#define HI_ISP_WDR_RGTHIGTHFL_DEFAULT   (0x3F00)
static __inline HI_VOID hi_isp_wdr_flrgtth_high_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_FLRGTTH ISP_WDR_FLRGTTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_FLRGTTH.u32 = pstIspReg->ISP_WDR_FLRGTTH.u32;
	ISP_WDR_FLRGTTH.bits.flrgtth_high = data; 
	pstIspReg->ISP_WDR_FLRGTTH.u32 = ISP_WDR_FLRGTTH.u32;
}
static __inline HI_U16 hi_isp_wdr_flrgtth_high_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_FLRGTTH ISP_WDR_FLRGTTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_FLRGTTH.u32 = pstIspReg->ISP_WDR_FLRGTTH.u32;
	return ISP_WDR_FLRGTTH.bits.flrgtth_high ; 	
}


/*****************************************************
ISP_WDR_FLDFTWGT:	BLC reserve 
0x205a602c: 		31:09 	reserved
					08:00   fl_dftwgt
*****************************************************/
#define HI_ISP_WDR_DFTWGTTHFL_DEFAULT   (0x100)
static __inline HI_VOID hi_isp_wdr_fl_dftwgt_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_FLDFTWGT ISP_WDR_FLDFTWGT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_FLDFTWGT.u32 = pstIspReg->ISP_WDR_FLDFTWGT.u32;
	ISP_WDR_FLDFTWGT.bits.fl_dftwgt = data; 
	pstIspReg->ISP_WDR_FLDFTWGT.u32 = ISP_WDR_FLDFTWGT.u32;
}

/*****************************************************
ISP_WDR_MDT_CTRL:	
0x205a6030: 		31:29     reserved
					28:28     bmdthf
					24:24     fl_bmdtmnu
					16:16     bmdtstrong
					00:00     balgprocmdt
*****************************************************/
#define HI_ISP_WDR_BMDTHF_DEFAULT       (HI_TRUE)
static __inline HI_VOID hi_isp_wdr_bmdthf_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_MDT_CTRL ISP_WDR_MDT_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_MDT_CTRL.u32 = pstIspReg->ISP_WDR_MDT_CTRL.u32;
	ISP_WDR_MDT_CTRL.bits.bmdthf = data; 
	pstIspReg->ISP_WDR_MDT_CTRL.u32 = ISP_WDR_MDT_CTRL.u32;
}

#define HI_ISP_WDR_FL_MDTMNU_DEFAULT  (0)
static __inline HI_VOID hi_isp_wdr_fl_bmdtmnu_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_MDT_CTRL ISP_WDR_MDT_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_MDT_CTRL.u32 = pstIspReg->ISP_WDR_MDT_CTRL.u32;
	ISP_WDR_MDT_CTRL.bits.fl_bmdtmnu = data; 
	pstIspReg->ISP_WDR_MDT_CTRL.u32 = ISP_WDR_MDT_CTRL.u32;
}
static __inline HI_U8 hi_isp_wdr_fl_bmdtmnu_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_MDT_CTRL ISP_WDR_MDT_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_MDT_CTRL.u32 = pstIspReg->ISP_WDR_MDT_CTRL.u32;
	return ISP_WDR_MDT_CTRL.bits.fl_bmdtmnu ; 	
}

#define HI_ISP_WDR_MDT_STRONG_DEFAULT (1)
static __inline HI_VOID hi_isp_wdr_bmdtstrong_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_MDT_CTRL ISP_WDR_MDT_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_MDT_CTRL.u32 = pstIspReg->ISP_WDR_MDT_CTRL.u32;
	ISP_WDR_MDT_CTRL.bits.bmdtstrong = data; 
	pstIspReg->ISP_WDR_MDT_CTRL.u32 = ISP_WDR_MDT_CTRL.u32;
}

#define HI_ISP_WDR_FL_MDTPROC_DEFAULT (1)
static __inline HI_VOID hi_isp_wdr_balgprocmdt_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_MDT_CTRL ISP_WDR_MDT_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_MDT_CTRL.u32 = pstIspReg->ISP_WDR_MDT_CTRL.u32;
	ISP_WDR_MDT_CTRL.bits.balgprocmdt = data; 
	pstIspReg->ISP_WDR_MDT_CTRL.u32 = ISP_WDR_MDT_CTRL.u32;
}
static __inline HI_U16 hi_isp_wdr_balgprocmdt_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_MDT_CTRL ISP_WDR_MDT_CTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_MDT_CTRL.u32 = pstIspReg->ISP_WDR_MDT_CTRL.u32;
	return ISP_WDR_MDT_CTRL.bits.balgprocmdt ; 	
}

/*****************************************************
ISP_WDR_BLDRMDTMAX:	
0x205a6034: 		31:29     reserved
					04:00     bldrmdtmax
*****************************************************/
static __inline HI_VOID hi_isp_wdr_bldrmdtmax_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_BLDRMDTMAX ISP_WDR_BLDRMDTMAX;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_BLDRMDTMAX.u32 = pstIspReg->ISP_WDR_BLDRMDTMAX.u32;
	ISP_WDR_BLDRMDTMAX.bits.bldrmdtmax = data; 
	pstIspReg->ISP_WDR_BLDRMDTMAX.u32 = ISP_WDR_BLDRMDTMAX.u32;
}

/*****************************************************
ISP_WDR_MDT_CTRL:
0x205a6038: 			31:14 	reserved
					13:00     nosclipth
*****************************************************/
static __inline HI_VOID hi_isp_wdr_nosclipth_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_NOSCLIPTH ISP_WDR_NOSCLIPTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_NOSCLIPTH.u32 = pstIspReg->ISP_WDR_NOSCLIPTH.u32;
	ISP_WDR_NOSCLIPTH.bits.nosclipth = data; 
	pstIspReg->ISP_WDR_NOSCLIPTH.u32 = ISP_WDR_NOSCLIPTH.u32;
}

/*****************************************************
ISP_WDR_NOSFACTOR:
0x205a603c: 			31:22 	reserved
					21:16     nosfactor_low
					15:06	reserved
					05:00     nosfactor_high
*****************************************************/
static __inline HI_VOID hi_isp_wdr_nosfactor_low_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_NOSFACTOR ISP_WDR_NOSFACTOR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_NOSFACTOR.u32 = pstIspReg->ISP_WDR_NOSFACTOR.u32;
	ISP_WDR_NOSFACTOR.bits.nosfactor_low = data;
	pstIspReg->ISP_WDR_NOSFACTOR.u32 = ISP_WDR_NOSFACTOR.u32;
}

static __inline HI_VOID hi_isp_wdr_nosfactor_high_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_NOSFACTOR ISP_WDR_NOSFACTOR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_NOSFACTOR.u32 = pstIspReg->ISP_WDR_NOSFACTOR.u32;
	ISP_WDR_NOSFACTOR.bits.nosfactor_high = data;
	pstIspReg->ISP_WDR_NOSFACTOR.u32 = ISP_WDR_NOSFACTOR.u32;
}

/*****************************************************
ISP_WDR_LUT_UPDATE:	
0x205a6040: 		31:22 	reserved
					00:00   noslut_update
*****************************************************/
static __inline HI_VOID hi_isp_wdr_noslut_update_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_LUT_UPDATE ISP_WDR_LUT_UPDATE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_LUT_UPDATE.u32 = pstIspReg->ISP_WDR_LUT_UPDATE.u32;
	ISP_WDR_LUT_UPDATE.bits.noslut_update = data;
	pstIspReg->ISP_WDR_LUT_UPDATE.u32 = ISP_WDR_LUT_UPDATE.u32;
}

/*****************************************************
ISP_WDR_FSNRJUDGE:	
0x113a6044: 		31:22 	reserved
					08:00   fsnr_judge
*****************************************************/
#define HI_ISP_WDR_FSNR_JUDGE_DEFAULT   (0x1)
static __inline HI_VOID hi_isp_wdr_fsnr_judge_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_FSNRJUDGE ISP_WDR_FSNRJUDGE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_FSNRJUDGE.u32 = pstIspReg->ISP_WDR_FSNRJUDGE.u32;
	ISP_WDR_FSNRJUDGE.bits.fsnr_judge = data;
	pstIspReg->ISP_WDR_FSNRJUDGE.u32 = ISP_WDR_FSNRJUDGE.u32;
}
static __inline HI_U16 hi_isp_wdr_fsnr_judge_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_FSNRJUDGE ISP_WDR_FSNRJUDGE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_FSNRJUDGE.u32 = pstIspReg->ISP_WDR_FSNRJUDGE.u32;
	return ISP_WDR_FSNRJUDGE.bits.fsnr_judge ;	
}

/*****************************************************
ISP_WDR_FSNRTH:	
0x113a6048: 		31:22 	reserved
					24:16   fsnrth_low
					08:00   fsnrth_high
*****************************************************/
#define HI_ISP_WDR_FSNRTH_HIGHT_DEFAULT (0x90)
static __inline HI_VOID hi_isp_wdr_fsnrth_high_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_FSNRTH ISP_WDR_FSNRTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_FSNRTH.u32 = pstIspReg->ISP_WDR_FSNRTH.u32;
	ISP_WDR_FSNRTH.bits.fsnrth_high = data;
	pstIspReg->ISP_WDR_FSNRTH.u32 = ISP_WDR_FSNRTH.u32;
}

#define HI_ISP_WDR_FSNRTH_LOW_DEFAULT (0x10)
static __inline HI_VOID hi_isp_wdr_fsnrth_low_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_FSNRTH ISP_WDR_FSNRTH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_FSNRTH.u32 = pstIspReg->ISP_WDR_FSNRTH.u32;
	ISP_WDR_FSNRTH.bits.fsnrth_low = data;
	pstIspReg->ISP_WDR_FSNRTH.u32 = ISP_WDR_FSNRTH.u32;
}

/*****************************************************
ISP_WDR_FSNRGN:	
0x113a604c: 		31:22 	reserved
					21:16   fsnrgn_low
					05:00   fsnrgn_high
*****************************************************/
#define HI_ISP_WDR_FSNRGN_HIGH_DEFAULT  (0xa)
static __inline HI_VOID hi_isp_wdr_fsnrgn_high_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_FSNRGN ISP_WDR_FSNRGN;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_FSNRGN.u32 = pstIspReg->ISP_WDR_FSNRGN.u32;
	ISP_WDR_FSNRGN.bits.fsnrgn_high = data;
	pstIspReg->ISP_WDR_FSNRGN.u32 = ISP_WDR_FSNRGN.u32;
}

#define HI_ISP_WDR_FSNRGN_LOW_DEFAULT   (0x5)
static __inline HI_VOID hi_isp_wdr_fsnrgn_low_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_FSNRGN ISP_WDR_FSNRGN;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_FSNRGN.u32 = pstIspReg->ISP_WDR_FSNRGN.u32;
	ISP_WDR_FSNRGN.bits.fsnrgn_low = data;
	pstIspReg->ISP_WDR_FSNRGN.u32 = ISP_WDR_FSNRGN.u32;
}

/*****************************************************
U_ISP_WDR_MDT_REFNOS:	
0x113a6050: 		
					bmdtrefnos            : 1
*****************************************************/
#define HI_ISP_WDR_BMDTREFNOS_DEFAULT (1)
static __inline HI_VOID hi_isp_wdr_bmdtrefnos_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_MDT_REFNOS ISP_WDR_MDT_REFNOS;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_MDT_REFNOS.u32 = pstIspReg->ISP_WDR_MDT_REFNOS.u32;
	ISP_WDR_MDT_REFNOS.bits.bmdtrefnos = data;
	pstIspReg->ISP_WDR_MDT_REFNOS.u32 = ISP_WDR_MDT_REFNOS.u32;
}

static __inline HI_U8 hi_isp_wdr_bmdtrefnos_read(HI_U8 ispDev ) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_MDT_REFNOS ISP_WDR_MDT_REFNOS;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_MDT_REFNOS.u32 = pstIspReg->ISP_WDR_MDT_REFNOS.u32;
	return ISP_WDR_MDT_REFNOS.bits.bmdtrefnos;	
}


/*****************************************************
ISP_WDR_F2WGTLUT_WADDR:	
0x205a6080: 			31:06 	reserved
					05:00     noslut_waddr
*****************************************************/
static __inline HI_VOID hi_isp_wdr_noslut_waddr_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_NOSLUT_WADDR ISP_WDR_NOSLUT_WADDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_NOSLUT_WADDR.u32 = pstIspReg->ISP_WDR_NOSLUT_WADDR.u32;
	ISP_WDR_NOSLUT_WADDR.bits.noslut_waddr = data;
	pstIspReg->ISP_WDR_NOSLUT_WADDR.u32 = ISP_WDR_NOSLUT_WADDR.u32;
}

/*****************************************************
ISP_WDR_FOCLRWGT_WDATA:
0x205a60b4: 			31:09 	reserved
					15:00     noslut_wdata
*****************************************************/
static __inline HI_VOID hi_isp_wdr_noslut_wdata_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_NOSLUT_WDATA ISP_WDR_NOSLUT_WDATA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_NOSLUT_WDATA.u32 = pstIspReg->ISP_WDR_NOSLUT_WDATA.u32;
	ISP_WDR_NOSLUT_WDATA.bits.noslut_wdata = data;
	pstIspReg->ISP_WDR_NOSLUT_WDATA.u32 = ISP_WDR_NOSLUT_WDATA.u32;
}

/*****************************************************
ISP_WDR_SIZE:             
0x205a60f0:                     31:29    reserved
                                          28:16   height
                                          15:13   reserved
                                          12:  0   width
*****************************************************/
static __inline HI_VOID hi_isp_wdr_height_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_SIZE ISP_WDR_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_SIZE.u32 = pstIspReg->ISP_WDR_SIZE.u32;
	ISP_WDR_SIZE.bits.height = data;
	pstIspReg->ISP_WDR_SIZE.u32 = ISP_WDR_SIZE.u32;
}

static __inline HI_VOID hi_isp_wdr_width_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDR_SIZE ISP_WDR_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDR_SIZE.u32 = pstIspReg->ISP_WDR_SIZE.u32;
	ISP_WDR_SIZE.bits.width = data;
	pstIspReg->ISP_WDR_SIZE.u32 = ISP_WDR_SIZE.u32;
}


/*****************************************************
MODULE        -----ISP_MODULE_POS-----
MODULE        -----ISP_MODULE_POS-----
MODULE        -----ISP_MODULE_POS-----
*****************************************************/
/*****************************************************
ISP_FE_MODULE_POS:		
0x205a0830: 			31:31 	reserved
					30:30     isp_fe_sel
					29:19 	reserved
					18:16     input4_sel
					15:15 	reserved
					14:12     input3_sel
					11:11 	reserved
					10:08     input2_sel
					07:07 	reserved
					06:04     input1_sel
					03:03 	reserved
					02:00     input0_sel
*****************************************************/
static __inline HI_VOID hi_isp_top_chn_switch0_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_INPUT_MUX ISP_INPUT_MUX;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_INPUT_MUX.u32 = pstIspReg->ISP_INPUT_MUX.u32;
	ISP_INPUT_MUX.bits.input0_sel = data;
	pstIspReg->ISP_INPUT_MUX.u32 = ISP_INPUT_MUX.u32;
}


static __inline HI_U8 hi_isp_top_chn_switch0_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_INPUT_MUX ISP_INPUT_MUX;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_INPUT_MUX.u32 = pstIspReg->ISP_INPUT_MUX.u32;
	return ISP_INPUT_MUX.bits.input0_sel;
}

static __inline HI_VOID hi_isp_top_chn_switch1_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_INPUT_MUX ISP_INPUT_MUX;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_INPUT_MUX.u32 = pstIspReg->ISP_INPUT_MUX.u32;
	ISP_INPUT_MUX.bits.input1_sel = data;
	pstIspReg->ISP_INPUT_MUX.u32 = ISP_INPUT_MUX.u32;
}


static __inline HI_U8 hi_isp_top_chn_switch1_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_INPUT_MUX ISP_INPUT_MUX;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_INPUT_MUX.u32 = pstIspReg->ISP_INPUT_MUX.u32;
	return ISP_INPUT_MUX.bits.input1_sel ;
}

static __inline HI_VOID hi_isp_top_chn_switch2_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_INPUT_MUX ISP_INPUT_MUX;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_INPUT_MUX.u32 = pstIspReg->ISP_INPUT_MUX.u32;
	ISP_INPUT_MUX.bits.input2_sel = data;
	pstIspReg->ISP_INPUT_MUX.u32 = ISP_INPUT_MUX.u32;
}

static __inline HI_VOID hi_isp_top_chn_switch3_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_INPUT_MUX ISP_INPUT_MUX;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_INPUT_MUX.u32 = pstIspReg->ISP_INPUT_MUX.u32;
	ISP_INPUT_MUX.bits.input3_sel = data;
	pstIspReg->ISP_INPUT_MUX.u32 = ISP_INPUT_MUX.u32;
}

static __inline HI_VOID hi_isp_top_chn_switch4_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_INPUT_MUX ISP_INPUT_MUX;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_INPUT_MUX.u32 = pstIspReg->ISP_INPUT_MUX.u32;
	ISP_INPUT_MUX.bits.input4_sel = data;
	pstIspReg->ISP_INPUT_MUX.u32 = ISP_INPUT_MUX.u32;
}

/*****************************************************
MODULE        -----ISP_BAYERNR-----
MODULE        -----ISP_BAYERNR-----
MODULE        -----ISP_BAYERNR-----
*****************************************************/
/*****************************************************
ISP_BNR_CFG:		
0x205a5400: 			31:01 	reserved
					00:00     bnr_en
*****************************************************/
static __inline HI_VOID hi_isp_bnr_en_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_CFG ISP_BNR_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_CFG.u32 = pstIspReg->ISP_BNR_CFG.u32;
	ISP_BNR_CFG.bits.bnr_en = data;
	pstIspReg->ISP_BNR_CFG.u32 = ISP_BNR_CFG.u32;
}

static __inline HI_U8 hi_isp_bnr_en_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_CFG ISP_BNR_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_CFG.u32 = pstIspReg->ISP_BNR_CFG.u32;
	return ISP_BNR_CFG.bits.bnr_en ;
}


/*****************************************************
ISP_BNR_CRATIO:		
0x205a5410: 		31:06 	reserved
					29:24     bg_bnr_cratio
					21:16     gb_bnr_cratio
					13:08     gr_bnr_cratio
					05:00     rg_bnr_cratio
*****************************************************/
static __inline HI_VOID hi_isp_bnr_cratio_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_CRATIO ISP_BNR_CRATIO;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_CRATIO.u32 = data;
	pstIspReg->ISP_BNR_CRATIO.u32 = ISP_BNR_CRATIO.u32;
}

/*****************************************************
ISP_BNR_LEV:		
0x205a5414: 		31:26 	reserved
					31:31   bg_amed_mode
					30:30   gb_amed_mode
					29:29   gr_amed_mode
					28:28   rg_amed_mode
					25:24   bgrg_amed_lev
					23:18 	reserved
					17:16   gbrg_amed_lev
					15:10 	reserved
					09:08   grbg_amed_lev
					07:02 	reserved
					01:00   rggb_amed_lev
*****************************************************/
static __inline HI_VOID hi_isp_rggb_amed_lev_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_LEV ISP_BNR_LEV;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_LEV.u32 = pstIspReg->ISP_BNR_LEV.u32;
	ISP_BNR_LEV.bits.rg_amed_lev = data;
	pstIspReg->ISP_BNR_LEV.u32 = ISP_BNR_LEV.u32;
}

static __inline HI_VOID hi_isp_grbg_amed_lev_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_LEV ISP_BNR_LEV;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_LEV.u32 = pstIspReg->ISP_BNR_LEV.u32;
	ISP_BNR_LEV.bits.gr_amed_lev = data;
	pstIspReg->ISP_BNR_LEV.u32 = ISP_BNR_LEV.u32;
}

static __inline HI_VOID hi_isp_gbrg_amed_lev_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_LEV ISP_BNR_LEV;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_LEV.u32 = pstIspReg->ISP_BNR_LEV.u32;
	ISP_BNR_LEV.bits.gb_amed_lev = data;
	pstIspReg->ISP_BNR_LEV.u32 = ISP_BNR_LEV.u32;
}

static __inline HI_VOID hi_isp_bgrg_amed_lev_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_LEV ISP_BNR_LEV;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_LEV.u32 = pstIspReg->ISP_BNR_LEV.u32;
	ISP_BNR_LEV.bits.bg_amed_lev = data;
	pstIspReg->ISP_BNR_LEV.u32 = ISP_BNR_LEV.u32;
}

static __inline HI_VOID hi_isp_bgrg_amed_mode_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_LEV ISP_BNR_LEV;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_LEV.u32 = pstIspReg->ISP_BNR_LEV.u32;
	ISP_BNR_LEV.bits.rg_amed_mode = data&0x1;
	pstIspReg->ISP_BNR_LEV.u32 = ISP_BNR_LEV.u32;	
	ISP_BNR_LEV.bits.gr_amed_mode = (data&0x2)>>1;
	pstIspReg->ISP_BNR_LEV.u32 = ISP_BNR_LEV.u32;
	ISP_BNR_LEV.bits.gb_amed_mode = (data&0x4)>>2;
	pstIspReg->ISP_BNR_LEV.u32 = ISP_BNR_LEV.u32;	
	ISP_BNR_LEV.bits.bg_amed_mode = (data&0x8)>>3;
	pstIspReg->ISP_BNR_LEV.u32 = ISP_BNR_LEV.u32;
}

/*****************************************************
ISP_BNR_SEL:		
0x205a5418: 			31:02 	reserved
					01:00     bnr_jnlm_sel
*****************************************************/
static __inline HI_VOID hi_isp_bnr_jnlm_sel_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_SEL ISP_BNR_SEL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_SEL.u32 = pstIspReg->ISP_BNR_SEL.u32;
	ISP_BNR_SEL.bits.bnr_jnlm_sel = data;
	pstIspReg->ISP_BNR_SEL.u32 = ISP_BNR_SEL.u32;
}

static __inline HI_VOID hi_isp_bnr_jnlm_shift_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_SEL ISP_BNR_SEL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_SEL.u32 = pstIspReg->ISP_BNR_SEL.u32;
	ISP_BNR_SEL.bits.bnr_jnlm_bitshift = data;
	pstIspReg->ISP_BNR_SEL.u32 = ISP_BNR_SEL.u32;
}


/*****************************************************
ISP_BNR_GAIN:		
0x205a541c: 			31:08 	reserved
					07:00     bnr_jnlm_gain
*****************************************************/
static __inline HI_VOID hi_isp_bnr_jnlm_gain_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_GAIN ISP_BNR_GAIN;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_GAIN.u32 = pstIspReg->ISP_BNR_GAIN.u32;
	ISP_BNR_GAIN.bits.bnr_jnlm_gain = data;
	pstIspReg->ISP_BNR_GAIN.u32 = ISP_BNR_GAIN.u32;
}

/*****************************************************
ISP_BNR_SYMCOEF:		
0x113a5420: 			31:08 	reserved
					07:00     bnr_jnlm_symcoef
*****************************************************/
#define HI_ISP_BNR_JNLM_SYMCOEF_LINEAR_DEFAULT (45)
#define HI_ISP_BNR_JNLM_SYMCOEF_WDR_DEFAULT (128)
static __inline HI_VOID hi_isp_bnr_jnlm_symcoef_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_SYMCOEF ISP_BNR_SYMCOEF;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_SYMCOEF.u32 = pstIspReg->ISP_BNR_SYMCOEF.u32;
	ISP_BNR_SYMCOEF.bits.bnr_jnlm_symcoef = data;
	pstIspReg->ISP_BNR_SYMCOEF.u32 = ISP_BNR_SYMCOEF.u32;
}


/*****************************************************
ISP_BNR_CORING:		
0x205a5424: 			31:24 	reserved
					29:16     bnr_coring_hig
					15:08     reserved
					13:00     bnr_coring_low
*****************************************************/
//static __inline HI_VOID hi_isp_bnr_coring_low_write(HI_U8 ispDev, HI_U16 data) {
//	S_HIISP_REGS_TYPE * pstIspReg;
//	U_ISP_BNR_CORING ISP_BNR_CORING;
//	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
//	ISP_BNR_CORING.u32 = pstIspReg->ISP_BNR_CORING.u32;
//	ISP_BNR_CORING.bits.bnr_coring_low = data;
//	pstIspReg->ISP_BNR_CORING.u32 = ISP_BNR_CORING.u32;
//}

static __inline HI_VOID hi_isp_bnr_coring_hig_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_CORING ISP_BNR_CORING;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_CORING.u32 = pstIspReg->ISP_BNR_CORING.u32;
	ISP_BNR_CORING.bits.bnr_coring_hig = data;
	pstIspReg->ISP_BNR_CORING.u32 = ISP_BNR_CORING.u32;
}

/*****************************************************
ISP_BNR_FRMNUM:	
0x205a5428 			31:03	reserved
					02:00     frm_num
*****************************************************/
static __inline HI_VOID hi_isp_bnr_frm_num_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_FRMNUM ISP_BNR_FRMNUM;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_FRMNUM.u32 = pstIspReg->ISP_BNR_FRMNUM.u32;
	ISP_BNR_FRMNUM.bits.frm_num = data;
	pstIspReg->ISP_BNR_FRMNUM.u32 = ISP_BNR_FRMNUM.u32;
}

/*****************************************************
ISP_BNR_LUT_UPDATE:	
0x205a542c: 			31:01	reserved
					00:00     lut_update
*****************************************************/
static __inline HI_VOID hi_isp_bnr_lut_update_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_LUT_UPDATE ISP_BNR_LUT_UPDATE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_LUT_UPDATE.u32 = pstIspReg->ISP_BNR_LUT_UPDATE.u32;
	ISP_BNR_LUT_UPDATE.bits.lut_update = data;
	pstIspReg->ISP_BNR_LUT_UPDATE.u32 = ISP_BNR_LUT_UPDATE.u32;
}

/*****************************************************
ISP_BNR_RLMT:		
0x113a5430 			31:03	reserved
                               29:20    bnr_rlmt_bgain
                               17:08    bnr_rlmt_rgain
				      04:00     bnr_rlmt_blc
*****************************************************/
static __inline HI_VOID hi_isp_bnr_rlmt_bgain_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_RLMT ISP_BNR_RLMT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_RLMT.u32 = pstIspReg->ISP_BNR_RLMT.u32;
	ISP_BNR_RLMT.bits.bnr_rlmt_bgain = data;
	pstIspReg->ISP_BNR_RLMT.u32 = ISP_BNR_RLMT.u32;
}

static __inline HI_VOID hi_isp_bnr_rlmt_rgain_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_RLMT ISP_BNR_RLMT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_RLMT.u32 = pstIspReg->ISP_BNR_RLMT.u32;
	ISP_BNR_RLMT.bits.bnr_rlmt_rgain = data;
	pstIspReg->ISP_BNR_RLMT.u32 = ISP_BNR_RLMT.u32;
}

#define HI_ISP_BNR_RLMT_BLC_DEFAULT (16)
static __inline HI_VOID hi_isp_bnr_rlmt_blc_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_RLMT ISP_BNR_RLMT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_RLMT.u32 = pstIspReg->ISP_BNR_RLMT.u32;
	ISP_BNR_RLMT.bits.bnr_rlmt_blc = data;
	pstIspReg->ISP_BNR_RLMT.u32 = ISP_BNR_RLMT.u32;
}

/*****************************************************
ISP_BNR_SATU0:		
0x113a5434 			31:03	reserved
                               08:00    bnr_satu_ratio
*****************************************************/
#define HI_ISP_BNR_SATU_RATIO_LINEAR_DEFAULT (256)
#define HI_ISP_BNR_SATU_RATIO_WDR_DEFAULT (256)
static __inline HI_VOID hi_isp_bnr_satu_ratio_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_SATU0 ISP_BNR_SATU0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_SATU0.u32 = pstIspReg->ISP_BNR_SATU0.u32;
	ISP_BNR_SATU0.bits.bnr_satu_ratio = data;
	pstIspReg->ISP_BNR_SATU0.u32 = ISP_BNR_SATU0.u32;
}



static __inline HI_U16 hi_isp_bnr_satu_ratio_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_SATU0 ISP_BNR_SATU0;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_SATU0.u32 = pstIspReg->ISP_BNR_SATU0.u32;
	return ISP_BNR_SATU0.bits.bnr_satu_ratio;
}




/*****************************************************
ISP_BNR_SATU1:		
0x113a5438 			31:03	reserved
                               24:16    bnr_satu_thhig1
                               08:00    bnr_satu_thlow1
*****************************************************/
#define HI_ISP_BNR_SATU_THHIG1_DEFAULT (110)
#define HI_ISP_BNR_SATU_THLOW1_DEFAULT (85)
static __inline HI_VOID hi_isp_bnr_satu_thhig1_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_SATU1 ISP_BNR_SATU1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_SATU1.u32 = pstIspReg->ISP_BNR_SATU1.u32;
	ISP_BNR_SATU1.bits.bnr_satu_thhig1 = data;
	pstIspReg->ISP_BNR_SATU1.u32 = ISP_BNR_SATU1.u32;
}

static __inline HI_VOID hi_isp_bnr_satu_thlow1_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_SATU1 ISP_BNR_SATU1;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_SATU1.u32 = pstIspReg->ISP_BNR_SATU1.u32;
	ISP_BNR_SATU1.bits.bnr_satu_thlow1 = data;
	pstIspReg->ISP_BNR_SATU1.u32 = ISP_BNR_SATU1.u32;
}

/*****************************************************
ISP_BNR_SATU2:	
0x113a543c 			31:03	reserved
                               24:16    bnr_satu_thhig2
                               08:00    bnr_satu_thlow2
*****************************************************/
#define HI_ISP_BNR_SATU_THHIG2_DEFAULT (180)
#define HI_ISP_BNR_SATU_THLOW2_DEFAULT (140)
static __inline HI_VOID hi_isp_bnr_satu_thhig2_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_SATU2 ISP_BNR_SATU2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_SATU2.u32 = pstIspReg->ISP_BNR_SATU2.u32;
	ISP_BNR_SATU2.bits.bnr_satu_thhig2 = data;
	pstIspReg->ISP_BNR_SATU2.u32 = ISP_BNR_SATU2.u32;
}

static __inline HI_VOID hi_isp_bnr_satu_thlow2_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_SATU2 ISP_BNR_SATU2;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_SATU2.u32 = pstIspReg->ISP_BNR_SATU2.u32;
	ISP_BNR_SATU2.bits.bnr_satu_thlow2 = data;
	pstIspReg->ISP_BNR_SATU2.u32 = ISP_BNR_SATU2.u32;
}



/*****************************************************
ISP_BNR_RGGB_ODD_WADDR:		
0x205a5480: 			31:00 	LUT_hwaddr
*****************************************************/
static __inline HI_VOID hi_isp_bnr_odd_rggb_waddr_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_RGGB_ODD_WADDR ISP_BNR_RGGB_ODD_WADDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_RGGB_ODD_WADDR.u32 = pstIspReg->ISP_BNR_RGGB_ODD_WADDR.u32;
	ISP_BNR_RGGB_ODD_WADDR.bits.lut_hwaddr = data;
	pstIspReg->ISP_BNR_RGGB_ODD_WADDR.u32 = ISP_BNR_RGGB_ODD_WADDR.u32;
}

/*****************************************************
ISP_BNR_RGGB_ODD_WADDR:	
0x205a5484: 			31:00 	LUT_hwaddr
*****************************************************/
static __inline HI_VOID hi_isp_bnr_odd_rggb_wdata_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_RGGB_ODD_WDATA ISP_BNR_RGGB_ODD_WDATA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_RGGB_ODD_WDATA.u32 = pstIspReg->ISP_BNR_RGGB_ODD_WDATA.u32;
	ISP_BNR_RGGB_ODD_WDATA.bits.lut_hwdata = data;
	pstIspReg->ISP_BNR_RGGB_ODD_WDATA.u32 = ISP_BNR_RGGB_ODD_WDATA.u32;
}

/*****************************************************
ISP_BNR_GRBG_ODD_WADDR:		
0x205a5490: 			31:00 	LUT_hwaddr
*****************************************************/
static __inline HI_VOID hi_isp_bnr_odd_grbg_waddr_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_GRBG_ODD_WADDR ISP_BNR_GRBG_ODD_WADDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_GRBG_ODD_WADDR.u32 = pstIspReg->ISP_BNR_GRBG_ODD_WADDR.u32;
	ISP_BNR_GRBG_ODD_WADDR.bits.lut_hwaddr = data;
	pstIspReg->ISP_BNR_GRBG_ODD_WADDR.u32 = ISP_BNR_GRBG_ODD_WADDR.u32;
}

/*****************************************************
ISP_BNR_RGGB_ODD_WADDR:		
0x205a5494: 			31:00 	LUT_hwaddr
*****************************************************/
static __inline HI_VOID hi_isp_bnr_odd_grbg_wdata_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_GRBG_ODD_WDATA ISP_BNR_GRBG_ODD_WDATA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_GRBG_ODD_WDATA.u32 = pstIspReg->ISP_BNR_GRBG_ODD_WDATA.u32;
	ISP_BNR_GRBG_ODD_WDATA.bits.lut_hwdata = data;
	pstIspReg->ISP_BNR_GRBG_ODD_WDATA.u32 = ISP_BNR_GRBG_ODD_WDATA.u32;
}

/*****************************************************
ISP_BNR_GBRG_ODD_WADDR:		
0x205a54a0: 			31:00 	LUT_hwaddr
*****************************************************/
static __inline HI_VOID hi_isp_bnr_odd_gbrg_waddr_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_GBRG_ODD_WADDR ISP_BNR_GBRG_ODD_WADDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_GBRG_ODD_WADDR.u32 = pstIspReg->ISP_BNR_GBRG_ODD_WADDR.u32;
	ISP_BNR_GBRG_ODD_WADDR.bits.lut_hwaddr = data;
	pstIspReg->ISP_BNR_GBRG_ODD_WADDR.u32 = ISP_BNR_GBRG_ODD_WADDR.u32;
}

/*****************************************************
ISP_BNR_RGGB_ODD_WADDR:		
0x205a54a4: 			31:00 	LUT_hwaddr
*****************************************************/
static __inline HI_VOID hi_isp_bnr_odd_gbrg_wdata_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_GBRG_ODD_WDATA ISP_BNR_GBRG_ODD_WDATA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_GBRG_ODD_WDATA.u32 = pstIspReg->ISP_BNR_GBRG_ODD_WDATA.u32;
	ISP_BNR_GBRG_ODD_WDATA.bits.lut_hwdata = data;
	pstIspReg->ISP_BNR_GBRG_ODD_WDATA.u32 = ISP_BNR_GBRG_ODD_WDATA.u32;
}

/*****************************************************
ISP_BNR_BGGR_ODD_WADDR:		
0x205a54b0: 			31:00 	LUT_hwaddr
*****************************************************/
static __inline HI_VOID hi_isp_bnr_odd_bggr_waddr_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_BGGR_ODD_WADDR ISP_BNR_BGGR_ODD_WADDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_BGGR_ODD_WADDR.u32 = pstIspReg->ISP_BNR_BGGR_ODD_WADDR.u32;
	ISP_BNR_BGGR_ODD_WADDR.bits.lut_hwaddr = data;
	pstIspReg->ISP_BNR_BGGR_ODD_WADDR.u32 = ISP_BNR_BGGR_ODD_WADDR.u32;
}

/*****************************************************
ISP_BNR_RGGB_ODD_WADDR:		
0x205a54b4: 			31:00 	LUT_hwaddr
*****************************************************/
static __inline HI_VOID hi_isp_bnr_odd_bggr_wdata_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_BGGR_ODD_WDATA ISP_BNR_BGGR_ODD_WDATA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_BGGR_ODD_WDATA.u32 = pstIspReg->ISP_BNR_BGGR_ODD_WDATA.u32;
	ISP_BNR_BGGR_ODD_WDATA.bits.lut_hwdata = data;
	pstIspReg->ISP_BNR_BGGR_ODD_WDATA.u32 = ISP_BNR_BGGR_ODD_WDATA.u32;
}

/*****************************************************
ISP_BNR_RLMT_ODD_WADDR:		
0x113a54c0: 			31:00 	lut_hwaddr
*****************************************************/
static __inline HI_VOID hi_isp_bnr_odd_rlmt_waddr_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_RLMT_ODD_WADDR ISP_BNR_RLMT_ODD_WADDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_RLMT_ODD_WADDR.u32 = pstIspReg->ISP_BNR_RLMT_ODD_WADDR.u32;
	ISP_BNR_RLMT_ODD_WADDR.bits.lut_hwaddr = data;
	pstIspReg->ISP_BNR_RLMT_ODD_WADDR.u32 = ISP_BNR_RLMT_ODD_WADDR.u32;
}

/*****************************************************
ISP_BNR_RLMT_ODD_WDATA:	
0x113a54c4: 			31:00 	lut_hwdata
*****************************************************/
static __inline HI_VOID hi_isp_bnr_odd_rlmt_wdata_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_RLMT_ODD_WDATA ISP_BNR_RLMT_ODD_WDATA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_RLMT_ODD_WDATA.u32 = pstIspReg->ISP_BNR_RLMT_ODD_WDATA.u32;
	ISP_BNR_RLMT_ODD_WDATA.bits.lut_hwdata = data;
	pstIspReg->ISP_BNR_RLMT_ODD_WDATA.u32 = ISP_BNR_RLMT_ODD_WDATA.u32;
}


/*****************************************************
ISP_BNR_RGGB_ODD_WADDR:		
0x205a5480: 			31:00 	LUT_hwaddr
*****************************************************/
static __inline HI_VOID hi_isp_bnr_even_rggb_waddr_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_RGGB_EVEN_WADDR ISP_BNR_RGGB_EVEN_WADDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_RGGB_EVEN_WADDR.u32 = pstIspReg->ISP_BNR_RGGB_EVEN_WADDR.u32;
	ISP_BNR_RGGB_EVEN_WADDR.bits.lut_hwaddr = data;
	pstIspReg->ISP_BNR_RGGB_EVEN_WADDR.u32 = ISP_BNR_RGGB_EVEN_WADDR.u32;
}


/*****************************************************
ISP_BNR_RGGB_ODD_WADDR:		
0x205a5484: 			31:00 	LUT_hwaddr
*****************************************************/
static __inline HI_VOID hi_isp_bnr_even_rggb_wdata_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_RGGB_EVEN_WDATA ISP_BNR_RGGB_EVEN_WDATA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_RGGB_EVEN_WDATA.u32 = pstIspReg->ISP_BNR_RGGB_EVEN_WDATA.u32;
	ISP_BNR_RGGB_EVEN_WDATA.bits.lut_hwdata = data;
	pstIspReg->ISP_BNR_RGGB_EVEN_WDATA.u32 = ISP_BNR_RGGB_EVEN_WDATA.u32;
}

/*****************************************************
ISP_BNR_GRBG_ODD_WADDR:		
0x205a5490: 			31:00 	LUT_hwaddr
*****************************************************/
static __inline HI_VOID hi_isp_bnr_even_grbg_waddr_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_GRBG_EVEN_WADDR ISP_BNR_GRBG_EVEN_WADDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_GRBG_EVEN_WADDR.u32 = pstIspReg->ISP_BNR_GRBG_EVEN_WADDR.u32;
	ISP_BNR_GRBG_EVEN_WADDR.bits.lut_hwaddr = data;
	pstIspReg->ISP_BNR_GRBG_EVEN_WADDR.u32 = ISP_BNR_GRBG_EVEN_WADDR.u32;
}

/*****************************************************
ISP_BNR_RGGB_ODD_WADDR:		
0x205a5494: 			31:00 	LUT_hwaddr
*****************************************************/
static __inline HI_VOID hi_isp_bnr_even_grbg_wdata_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_GRBG_EVEN_WDATA ISP_BNR_GRBG_EVEN_WDATA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_GRBG_EVEN_WDATA.u32 = pstIspReg->ISP_BNR_GRBG_EVEN_WDATA.u32;
	ISP_BNR_GRBG_EVEN_WDATA.bits.lut_hwdata = data;
	pstIspReg->ISP_BNR_GRBG_EVEN_WDATA.u32 = ISP_BNR_GRBG_EVEN_WDATA.u32;
}

/*****************************************************
ISP_BNR_GBRG_ODD_WADDR:	
0x205a54a0: 			31:00 	LUT_hwaddr
*****************************************************/
static __inline HI_VOID hi_isp_bnr_even_gbrg_waddr_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_GBRG_EVEN_WADDR ISP_BNR_GBRG_EVEN_WADDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_GBRG_EVEN_WADDR.u32 = pstIspReg->ISP_BNR_GBRG_EVEN_WADDR.u32;
	ISP_BNR_GBRG_EVEN_WADDR.bits.lut_hwaddr = data;
	pstIspReg->ISP_BNR_GBRG_EVEN_WADDR.u32 = ISP_BNR_GBRG_EVEN_WADDR.u32;
}

/*****************************************************
ISP_BNR_RGGB_ODD_WADDR:		   
0x205a54a4: 			31:00 	LUT_hwaddr
*****************************************************/
static __inline HI_VOID hi_isp_bnr_even_gbrg_wdata_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_GBRG_EVEN_WDATA ISP_BNR_GBRG_EVEN_WDATA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_GBRG_EVEN_WDATA.u32 = pstIspReg->ISP_BNR_GBRG_EVEN_WDATA.u32;
	ISP_BNR_GBRG_EVEN_WDATA.bits.lut_hwdata = data;
	pstIspReg->ISP_BNR_GBRG_EVEN_WDATA.u32 = ISP_BNR_GBRG_EVEN_WDATA.u32;
}

/*****************************************************
ISP_BNR_BGGR_ODD_WADDR:		   
0x205a54b0: 			31:00 	LUT_hwaddr
*****************************************************/
static __inline HI_VOID hi_isp_bnr_even_bggr_waddr_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_BGGR_EVEN_WADDR ISP_BNR_BGGR_EVEN_WADDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_BGGR_EVEN_WADDR.u32 = pstIspReg->ISP_BNR_BGGR_EVEN_WADDR.u32;
	ISP_BNR_BGGR_EVEN_WADDR.bits.lut_hwaddr = data;
	pstIspReg->ISP_BNR_BGGR_EVEN_WADDR.u32 = ISP_BNR_BGGR_EVEN_WADDR.u32;
}

/*****************************************************
ISP_BNR_RGGB_ODD_WADDR:		   
0x205a54b4: 			31:00 	LUT_hwaddr
*****************************************************/
static __inline HI_VOID hi_isp_bnr_even_bggr_wdata_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_BGGR_EVEN_WDATA ISP_BNR_BGGR_EVEN_WDATA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_BGGR_EVEN_WDATA.u32 = pstIspReg->ISP_BNR_BGGR_EVEN_WDATA.u32;
	ISP_BNR_BGGR_EVEN_WDATA.bits.lut_hwdata = data;
	pstIspReg->ISP_BNR_BGGR_EVEN_WDATA.u32 = ISP_BNR_BGGR_EVEN_WDATA.u32;
}

/*****************************************************
ISP_BNR_RLMT_EVEN_WADDR:		
0x113a55c0: 			31:00 	lut_hwaddr
*****************************************************/
static __inline HI_VOID hi_isp_bnr_even_rlmt_waddr_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_RLMT_EVEN_WADDR ISP_BNR_RLMT_EVEN_WADDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_RLMT_EVEN_WADDR.u32 = pstIspReg->ISP_BNR_RLMT_EVEN_WADDR.u32;
	ISP_BNR_RLMT_EVEN_WADDR.bits.lut_hwaddr = data;
	pstIspReg->ISP_BNR_RLMT_EVEN_WADDR.u32 = ISP_BNR_RLMT_EVEN_WADDR.u32;
}

/*****************************************************
ISP_BNR_RLMT_EVEN_WDATA:		
0x113a55c4: 			31:00 	lut_hwdata
*****************************************************/
static __inline HI_VOID hi_isp_bnr_even_rlmt_wdata_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_RLMT_EVEN_WDATA ISP_BNR_RLMT_EVEN_WDATA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_RLMT_EVEN_WDATA.u32 = pstIspReg->ISP_BNR_RLMT_EVEN_WDATA.u32;
	ISP_BNR_RLMT_EVEN_WDATA.bits.lut_hwdata = data;
	pstIspReg->ISP_BNR_RLMT_EVEN_WDATA.u32 = ISP_BNR_RLMT_EVEN_WDATA.u32;
}

/*****************************************************
ISP_BAYERNR_SIZE:             
0x205a54f0:                     31:29    reserved
                                          28:16   height
                                          15:13   reserved
                                          12:  0   width
*****************************************************/
static __inline HI_VOID hi_isp_bnr_height_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_SIZE ISP_BNR_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_SIZE.u32 = pstIspReg->ISP_BNR_SIZE.u32;
	ISP_BNR_SIZE.bits.height = data;
	pstIspReg->ISP_BNR_SIZE.u32 = ISP_BNR_SIZE.u32;
}

static __inline HI_VOID hi_isp_bnr_width_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BNR_SIZE ISP_BNR_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BNR_SIZE.u32 = pstIspReg->ISP_BNR_SIZE.u32;
	ISP_BNR_SIZE.bits.width = data;
	pstIspReg->ISP_BNR_SIZE.u32 = ISP_BNR_SIZE.u32;
}

/*****************************************************
MODULE        -----ISP_BAYERNR COM-----
MODULE        -----ISP_BAYERNR COM-----
MODULE        -----ISP_BAYERNR DEC-----
*****************************************************/

/*****************************************************
ISP_BCOM_CFG:                    
0x113a6c00:                     31:01    reserved
                                        00:00   width
*****************************************************/
static __inline HI_VOID hi_isp_bcom_enable_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BCOM_CFG ISP_BCOM_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BCOM_CFG.u32 = pstIspReg->ISP_BCOM_CFG.u32;
	ISP_BCOM_CFG.bits.en = data;
	pstIspReg->ISP_BCOM_CFG.u32 = ISP_BCOM_CFG.u32;
}

/*****************************************************
ISP_BCOM_ALPHA:               
0x113a6c10:                    31:01    reserved
                                        02:00   alpha
*****************************************************/
static __inline HI_VOID hi_isp_bcom_alpha_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BCOM_ALPHA ISP_BCOM_ALPHA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BCOM_ALPHA.u32 = pstIspReg->ISP_BCOM_ALPHA.u32;
	ISP_BCOM_ALPHA.bits.alpha = data;
	pstIspReg->ISP_BCOM_ALPHA.u32 = ISP_BCOM_ALPHA.u32;
}

/*****************************************************
ISP_BDEC_CFG:                    
0x113a6D00:                  31:01   reserved
                             00:00   width
*****************************************************/
static __inline HI_VOID hi_isp_bdec_enable_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BDEC_CFG ISP_BDEC_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BDEC_CFG.u32 = pstIspReg->ISP_BDEC_CFG.u32;
	ISP_BDEC_CFG.bits.en = data;
	pstIspReg->ISP_BDEC_CFG.u32 = ISP_BDEC_CFG.u32;
}

/*****************************************************
ISP_BDEC_ALPHA:               
0x113a6d10:                    31:01    reserved
                                        02:00   alpha
*****************************************************/
static __inline HI_VOID hi_isp_bdec_alpha_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_BDEC_ALPHA ISP_BDEC_ALPHA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_BDEC_ALPHA.u32 = pstIspReg->ISP_BDEC_ALPHA.u32;
	ISP_BDEC_ALPHA.bits.alpha = data;
	pstIspReg->ISP_BDEC_ALPHA.u32 = ISP_BDEC_ALPHA.u32;
}

/*****************************************************
MODULE        -----ISP_AE_WDR-----
MODULE        -----ISP_AE_WDR-----
MODULE        -----ISP_AE_WDR-----
*****************************************************/
#define HI_AE_WDR_REG_ALIGN (0x100)
/*****************************************************
ISP_AEWDR0_CFG:
0x113a9000:                     31:01  reserved
                                        00:00   en
*****************************************************/
static __inline HI_VOID hi_isp_ae_wdr_en_write(HI_U8 ispDev, HI_U8 frame, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AEWDR0_CFG       ISP_AEWDR0_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AEWDR0_CFG.u32 = pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_CFG.u32;
	ISP_AEWDR0_CFG.bits.en = data;
	pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_CFG.u32 = ISP_AEWDR0_CFG.u32;
}

/*****************************************************
ISP_AEWDR0_ZONE:       
0x113a9010:                     12:08  vnum
                                        04:00   hnum
*****************************************************/
static __inline HI_VOID hi_isp_ae_wdr_zone_hnum_write(HI_U8 ispDev, HI_U8 frame, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AEWDR0_ZONE ISP_AEWDR0_ZONE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AEWDR0_ZONE.u32 = pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_ZONE.u32;
	ISP_AEWDR0_ZONE.bits.hnum = data;
	pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_ZONE.u32 = ISP_AEWDR0_ZONE.u32;
}

static __inline HI_VOID hi_isp_ae_wdr_zone_vnum_write(HI_U8 ispDev, HI_U8 frame, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AEWDR0_ZONE ISP_AEWDR0_ZONE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AEWDR0_ZONE.u32 = pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_ZONE.u32;
	ISP_AEWDR0_ZONE.bits.vnum = data;
	pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_ZONE.u32 = ISP_AEWDR0_ZONE.u32;
}

/*****************************************************
ISP_AEWDR0_SKIP_CRG:   
0x113a9014:                     07:07  offset_y
						06:04   skip_y
						03:03   offset_x
						02:00   skip_x
*****************************************************/
static __inline HI_VOID hi_isp_ae_wdr_skip_x_write(HI_U8 ispDev, HI_U8 frame, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AEWDR0_SKIP_CRG ISP_AEWDR0_SKIP_CRG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AEWDR0_SKIP_CRG.u32 = pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_SKIP_CRG.u32;
	ISP_AEWDR0_SKIP_CRG.bits.skip_x = data;
	pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_SKIP_CRG.u32 = ISP_AEWDR0_SKIP_CRG.u32;
}

static __inline HI_VOID hi_isp_ae_wdr_offset_x_write(HI_U8 ispDev, HI_U8 frame, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AEWDR0_SKIP_CRG ISP_AEWDR0_SKIP_CRG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AEWDR0_SKIP_CRG.u32 = pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_SKIP_CRG.u32;
	ISP_AEWDR0_SKIP_CRG.bits.offset_x = data;
	pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_SKIP_CRG.u32 = ISP_AEWDR0_SKIP_CRG.u32;
}

static __inline HI_VOID hi_isp_ae_wdr_skip_y_write(HI_U8 ispDev, HI_U8 frame, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AEWDR0_SKIP_CRG ISP_AEWDR0_SKIP_CRG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AEWDR0_SKIP_CRG.u32 = pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_SKIP_CRG.u32;
	ISP_AEWDR0_SKIP_CRG.bits.skip_y = data;
	pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_SKIP_CRG.u32 = ISP_AEWDR0_SKIP_CRG.u32;
}

static __inline HI_VOID hi_isp_ae_wdr_offset_y_write(HI_U8 ispDev, HI_U8 frame, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AEWDR0_SKIP_CRG ISP_AEWDR0_SKIP_CRG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AEWDR0_SKIP_CRG.u32 = pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_SKIP_CRG.u32;
	ISP_AEWDR0_SKIP_CRG.bits.offset_y = data;
	pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_SKIP_CRG.u32 = ISP_AEWDR0_SKIP_CRG.u32;
}

/*****************************************************
ISP_AEWDR0_TOTAL_STAT:      
0x113a9018:                    31:24  reserved
                                        23:00   total_pixels
*****************************************************/
static __inline HI_VOID hi_isp_ae_wdr_total_pixels_write(HI_U8 ispDev, HI_U8 frame, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AEWDR0_TOTAL_STAT ISP_AEWDR0_TOTAL_STAT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AEWDR0_TOTAL_STAT.u32 = pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_TOTAL_STAT.u32;
	ISP_AEWDR0_TOTAL_STAT.bits.total_pixels = data;
	pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_TOTAL_STAT.u32 = ISP_AEWDR0_TOTAL_STAT.u32;
}

static __inline HI_U32 hi_isp_ae_wdr_total_pixels_read(HI_U8 ispDev, HI_U8 frame) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AEWDR0_TOTAL_STAT ISP_AEWDR0_TOTAL_STAT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AEWDR0_TOTAL_STAT.u32 = pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_TOTAL_STAT.u32;
	return ISP_AEWDR0_TOTAL_STAT.bits.total_pixels;
}

/*****************************************************
ISP_AEWDR0_TOTAL_STAT:      
0x113a901c:                    31:28  reserved
                                        27:00   count_pixels
*****************************************************/
static __inline HI_VOID hi_isp_ae_wdr_count_pixels_write(HI_U8 ispDev, HI_U8 frame, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AEWDR0_COUNT_STAT ISP_AEWDR0_COUNT_STAT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AEWDR0_COUNT_STAT.u32 = pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_COUNT_STAT.u32;
	ISP_AEWDR0_COUNT_STAT.bits.count_pixels = data;
	pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_COUNT_STAT.u32 = ISP_AEWDR0_COUNT_STAT.u32;
}

static __inline HI_U32 hi_isp_ae_wdr_count_pixels_read(HI_U8 ispDev, HI_U8 frame) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AEWDR0_COUNT_STAT ISP_AEWDR0_COUNT_STAT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AEWDR0_COUNT_STAT.u32 = pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_COUNT_STAT.u32;
	return ISP_AEWDR0_COUNT_STAT.bits.count_pixels;
}

/*****************************************************
ISP_AEWDR0_HIST_HIGH:      
0x113a9030:                    31:00  hist_high
*****************************************************/
static __inline HI_VOID hi_isp_ae_wdr_hist_high_write(HI_U8 ispDev, HI_U8 frame, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AEWDR0_HIST_HIGH ISP_AEWDR0_HIST_HIGH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AEWDR0_HIST_HIGH.u32 = pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_HIST_HIGH.u32;
	ISP_AEWDR0_HIST_HIGH.bits.hist_high = data;
	pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_HIST_HIGH.u32 = ISP_AEWDR0_HIST_HIGH.u32;
}

/*****************************************************
ISP_AEWDR0_BITMOVE:      
0x113a9040:                    09:09  blc_en
					     04:00  bitmove
*****************************************************/
static __inline HI_VOID hi_isp_ae_wdr_bitmove_write(HI_U8 ispDev, HI_U8 frame, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AEWDR0_BITMOVE ISP_AEWDR0_BITMOVE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AEWDR0_BITMOVE.u32 = pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_BITMOVE.u32;
	ISP_AEWDR0_BITMOVE.bits.bitmove = data;
	pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_BITMOVE.u32 = ISP_AEWDR0_BITMOVE.u32;
}

static __inline HI_VOID hi_isp_ae_wdr_blc_en_write(HI_U8 ispDev, HI_U8 frame, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AEWDR0_BITMOVE ISP_AEWDR0_BITMOVE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AEWDR0_BITMOVE.u32 = pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_BITMOVE.u32;
	ISP_AEWDR0_BITMOVE.bits.blc_en = data;
	pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_BITMOVE.u32 = ISP_AEWDR0_BITMOVE.u32;
}

/*****************************************************
ISP_AEWDR0_OFFSET_R:      
0x113a9044:                    
					     14:00  offset_r
*****************************************************/
static __inline HI_VOID hi_isp_ae_wdr_offset_r_write(HI_U8 ispDev, HI_U8 frame, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AEWDR0_OFFSET_R ISP_AEWDR0_OFFSET_R;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AEWDR0_OFFSET_R.u32 = pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_OFFSET_R.u32;
	ISP_AEWDR0_OFFSET_R.bits.offset_r = data;
	pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_OFFSET_R.u32 = ISP_AEWDR0_OFFSET_R.u32;
}

/*****************************************************
ISP_AEWDR0_OFFSET_GR:      
0x113a9048:                    
					     14:00  offset_gr
*****************************************************/
static __inline HI_VOID hi_isp_ae_wdr_offset_gr_write(HI_U8 ispDev, HI_U8 frame, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AEWDR0_OFFSET_GR ISP_AEWDR0_OFFSET_GR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AEWDR0_OFFSET_GR.u32 = pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_OFFSET_GR.u32;
	ISP_AEWDR0_OFFSET_GR.bits.offset_gr = data;
	pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_OFFSET_GR.u32 = ISP_AEWDR0_OFFSET_GR.u32;
}

/*****************************************************
ISP_AEWDR0_OFFSET_GB:      
0x113a904c:                    
					     14:00  offset_gb
*****************************************************/
static __inline HI_VOID hi_isp_ae_wdr_offset_gb_write(HI_U8 ispDev, HI_U8 frame, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AEWDR0_OFFSET_GB ISP_AEWDR0_OFFSET_GB;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AEWDR0_OFFSET_GB.u32 = pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_OFFSET_GB.u32;
	ISP_AEWDR0_OFFSET_GB.bits.offset_gb = data;
	pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_OFFSET_GB.u32 = ISP_AEWDR0_OFFSET_GB.u32;
}

/*****************************************************
ISP_AEWDR0_OFFSET_B:      
0x113a9050:                    
					     14:00  offset_b
*****************************************************/
static __inline HI_VOID hi_isp_ae_wdr_offset_b_write(HI_U8 ispDev, HI_U8 frame, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AEWDR0_OFFSET_B ISP_AEWDR0_OFFSET_B;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AEWDR0_OFFSET_B.u32 = pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_OFFSET_B.u32;
	ISP_AEWDR0_OFFSET_B.bits.offset_b = data;
	pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_OFFSET_B.u32 = ISP_AEWDR0_OFFSET_B.u32;
}

/*****************************************************
ISP_AEWDR0_MEM_HIST_RADDR:   
0x113a9088:                    
					     31:00  hist_raddr
*****************************************************/
static __inline HI_VOID hi_isp_ae_wdr_hist_raddr_write(HI_U8 ispDev, HI_U8 frame, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AEWDR0_MEM_HIST_RADDR ISP_AEWDR0_MEM_HIST_RADDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AEWDR0_MEM_HIST_RADDR.u32 = pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_MEM_HIST_RADDR.u32;
	ISP_AEWDR0_MEM_HIST_RADDR.bits.hist_raddr = data;
	pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_MEM_HIST_RADDR.u32 = ISP_AEWDR0_MEM_HIST_RADDR.u32;
}

/*****************************************************
ISP_AEWDR0_MEM_HIST_RDATA:   
0x113a908c:                    
					     31:00  hist_rdata
*****************************************************/
static __inline HI_U32 hi_isp_ae_wdr_hist_rdata_read(HI_U8 ispDev, HI_U8 frame) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AEWDR0_MEM_HIST_RDATA ISP_AEWDR0_MEM_HIST_RDATA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AEWDR0_MEM_HIST_RDATA.u32 = pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_MEM_HIST_RDATA.u32;
	return ISP_AEWDR0_MEM_HIST_RDATA.bits.hist_rdata;
}

/*****************************************************
ISP_AEWDR0_MEM_WEIGHT_WADDR: 
0x113a90a0:                    
					     31:00  wei_waddr
*****************************************************/
static __inline HI_VOID hi_isp_ae_wdr_wei_waddr_write(HI_U8 ispDev, HI_U8 frame, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AEWDR0_MEM_WEIGHT_WADDR ISP_AEWDR0_MEM_WEIGHT_WADDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AEWDR0_MEM_WEIGHT_WADDR.u32 = pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_MEM_WEIGHT_WADDR.u32;
	ISP_AEWDR0_MEM_WEIGHT_WADDR.bits.wei_waddr = data;
	pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_MEM_WEIGHT_WADDR.u32 = ISP_AEWDR0_MEM_WEIGHT_WADDR.u32;
}

/*****************************************************
ISP_AEWDR0_MEM_WEIGHT_WADDR: 
0x113a90a4:                    
					     31:00  wei_wdata
*****************************************************/
static __inline HI_VOID hi_isp_ae_wdr_wei_wdata_write(HI_U8 ispDev, HI_U8 frame, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AEWDR0_MEM_WEIGHT_WDATA ISP_AEWDR0_MEM_WEIGHT_WDATA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AEWDR0_MEM_WEIGHT_WDATA.u32 = pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_MEM_WEIGHT_WDATA.u32;
	ISP_AEWDR0_MEM_WEIGHT_WDATA.bits.wei_wdata = data;
	pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_MEM_WEIGHT_WDATA.u32 = ISP_AEWDR0_MEM_WEIGHT_WDATA.u32;
}

/*****************************************************
ISP_AEWDR0_SIZE: 
0x113a90F0:                    
					     28:16  vsize   
					     12:00  hsize
*****************************************************/
static __inline HI_VOID hi_isp_ae_wdr_vsize_write(HI_U8 ispDev, HI_U8 frame, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AEWDR0_SIZE ISP_AEWDR0_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AEWDR0_SIZE.u32 = pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_SIZE.u32;
	ISP_AEWDR0_SIZE.bits.vsize = data;
	pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_SIZE.u32 = ISP_AEWDR0_SIZE.u32;
}

static __inline HI_VOID hi_isp_ae_wdr_hsize_write(HI_U8 ispDev, HI_U8 frame, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_AEWDR0_SIZE ISP_AEWDR0_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_AEWDR0_SIZE.u32 = pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_SIZE.u32;
	ISP_AEWDR0_SIZE.bits.hsize = data;
	pstIspReg->ISP_AE_WDR[frame].ISP_AEWDR0_SIZE.u32 = ISP_AEWDR0_SIZE.u32;
}

/*****************************************************
MODULE        -----ISP_ISP_FLICK-----
MODULE        -----ISP_ISP_FLICK-----
MODULE        -----ISP_ISP_FLICK-----
*****************************************************/

/*****************************************************
ISP_FLICK_CFG:   
0x113a2600:                     31:01  reserved
                                        00:00   en
*****************************************************/
static __inline HI_VOID hi_isp_flick_en_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_FLICK_CFG ISP_FLICK_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_FLICK_CFG.u32 = pstIspReg->ISP_FLICK_CFG.u32;
	ISP_FLICK_CFG.bits.en = data;
	pstIspReg->ISP_FLICK_CFG.u32 = ISP_FLICK_CFG.u32;
}

/*****************************************************
ISP_FLICK_FRM_MERGE:  
0x113a2610:                     31:03  reserved
                                02:00   frm_merge
*****************************************************/
static __inline HI_VOID hi_isp_flick_frm_merge_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_FLICK_FRM_MERGE ISP_FLICK_FRM_MERGE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_FLICK_FRM_MERGE.u32 = pstIspReg->ISP_FLICK_FRM_MERGE.u32;
	ISP_FLICK_FRM_MERGE.bits.frm_merge = data;
	pstIspReg->ISP_FLICK_FRM_MERGE.u32 = ISP_FLICK_FRM_MERGE.u32;
}

/*****************************************************
ISP_FLICK_BLC_IN:   
0x113a2614:                     31:31  reserved
                                30:16  frm1_blc
                                14:00  frm0_blc
*****************************************************/
static __inline HI_VOID hi_isp_flick_frm0_blc_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_FLICK_BLC_IN ISP_FLICK_BLC_IN;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_FLICK_BLC_IN.u32 = pstIspReg->ISP_FLICK_BLC_IN.u32;
	ISP_FLICK_BLC_IN.bits.frm0_blc = data;
	pstIspReg->ISP_FLICK_BLC_IN.u32 = ISP_FLICK_BLC_IN.u32;
}

static __inline HI_VOID hi_isp_flick_frm1_blc_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_FLICK_BLC_IN ISP_FLICK_BLC_IN;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_FLICK_BLC_IN.u32 = pstIspReg->ISP_FLICK_BLC_IN.u32;
	ISP_FLICK_BLC_IN.bits.frm1_blc = data;
	pstIspReg->ISP_FLICK_BLC_IN.u32 = ISP_FLICK_BLC_IN.u32;
}

/*****************************************************
ISP_FLICK_EXP_RAT:  
0x113a2618:                   31:31  reserved
                              11:00  exp_ratios0
*****************************************************/
static __inline HI_VOID hi_isp_flick_exp_ratios0_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_FLICK_EXP_RAT ISP_FLICK_EXP_RAT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_FLICK_EXP_RAT.u32 = pstIspReg->ISP_FLICK_EXP_RAT.u32;
	ISP_FLICK_EXP_RAT.bits.exp_ratios0 = data;
	pstIspReg->ISP_FLICK_EXP_RAT.u32 = ISP_FLICK_EXP_RAT.u32;
}

/*****************************************************
ISP_FLICK_OVER_TH:      
0x113a261c:                   31:10  reserved
                              09:00  over_th
*****************************************************/
static __inline HI_VOID hi_isp_flick_over_th_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_FLICK_OVER_TH ISP_FLICK_OVER_TH;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_FLICK_OVER_TH.u32 = pstIspReg->ISP_FLICK_OVER_TH.u32;
	ISP_FLICK_OVER_TH.bits.over_th = data;
	pstIspReg->ISP_FLICK_OVER_TH.u32 = ISP_FLICK_OVER_TH.u32;
}

/*****************************************************
ISP_FLICK_PRE_AVG_GR:      
0x113a2620:                   31:10  reserved
                              10:00  pre_avg_gr
*****************************************************/
static __inline HI_VOID hi_isp_flick_pre_avg_gr_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_FLICK_PRE_AVG_GR ISP_FLICK_PRE_AVG_GR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_FLICK_PRE_AVG_GR.u32 = pstIspReg->ISP_FLICK_PRE_AVG_GR.u32;
	ISP_FLICK_PRE_AVG_GR.bits.pre_avg_gr = data;
	pstIspReg->ISP_FLICK_PRE_AVG_GR.u32 = ISP_FLICK_PRE_AVG_GR.u32;
}

/*****************************************************
ISP_FLICK_PRE_AVG_GB:     
0x113a2624:                   31:10  reserved
                              10:00  pre_avg_gb
*****************************************************/
static __inline HI_VOID hi_isp_flick_pre_avg_gb_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_FLICK_PRE_AVG_GB ISP_FLICK_PRE_AVG_GB;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_FLICK_PRE_AVG_GB.u32 = pstIspReg->ISP_FLICK_PRE_AVG_GB.u32;
	ISP_FLICK_PRE_AVG_GB.bits.pre_avg_gb = data;
	pstIspReg->ISP_FLICK_PRE_AVG_GB.u32 = ISP_FLICK_PRE_AVG_GB.u32;
}

/*****************************************************
ISP_FLICK_CUR_SUM_GR:     
0x113a2628:                   31:10  reserved
                              22:00  cur_sum_gr
*****************************************************/
static __inline HI_VOID hi_isp_flick_cur_sum_gr_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_FLICK_CUR_SUM_GR ISP_FLICK_CUR_SUM_GR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_FLICK_CUR_SUM_GR.u32 = pstIspReg->ISP_FLICK_CUR_SUM_GR.u32;
	ISP_FLICK_CUR_SUM_GR.bits.cur_sum_gr = data;
	pstIspReg->ISP_FLICK_CUR_SUM_GR.u32 = ISP_FLICK_CUR_SUM_GR.u32;
}

static __inline HI_U32 hi_isp_flick_cur_sum_gr_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_FLICK_CUR_SUM_GR ISP_FLICK_CUR_SUM_GR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_FLICK_CUR_SUM_GR.u32 = pstIspReg->ISP_FLICK_CUR_SUM_GR.u32;
	return ISP_FLICK_CUR_SUM_GR.bits.cur_sum_gr;
}

/*****************************************************
ISP_FLICK_CUR_SUM_GB:     
0x113a262c:                   31:10  reserved
                              22:00  cur_sum_gb
*****************************************************/
static __inline HI_VOID hi_isp_flick_cur_sum_gb_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_FLICK_CUR_SUM_GB ISP_FLICK_CUR_SUM_GB;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_FLICK_CUR_SUM_GB.u32 = pstIspReg->ISP_FLICK_CUR_SUM_GB.u32;
	ISP_FLICK_CUR_SUM_GB.bits.cur_sum_gb = data;
	pstIspReg->ISP_FLICK_CUR_SUM_GB.u32 = ISP_FLICK_CUR_SUM_GB.u32;
}

static __inline HI_U32 hi_isp_flick_cur_sum_gb_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_FLICK_CUR_SUM_GB ISP_FLICK_CUR_SUM_GB;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_FLICK_CUR_SUM_GB.u32 = pstIspReg->ISP_FLICK_CUR_SUM_GB.u32;
	return ISP_FLICK_CUR_SUM_GB.bits.cur_sum_gb;
}

/*****************************************************
ISP_FLICK_CUR_ABS_GR:      
0x113a2630:                   31:10  reserved
                              22:00  cur_abs_gr
*****************************************************/
static __inline HI_VOID hi_isp_flick_cur_abs_gr_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_FLICK_CUR_ABS_GR ISP_FLICK_CUR_ABS_GR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_FLICK_CUR_ABS_GR.u32 = pstIspReg->ISP_FLICK_CUR_ABS_GR.u32;
	ISP_FLICK_CUR_ABS_GR.bits.cur_abs_gr = data;
	pstIspReg->ISP_FLICK_CUR_ABS_GR.u32 = ISP_FLICK_CUR_ABS_GR.u32;
}

static __inline HI_U32 hi_isp_flick_cur_abs_gr_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_FLICK_CUR_ABS_GR ISP_FLICK_CUR_ABS_GR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_FLICK_CUR_ABS_GR.u32 = pstIspReg->ISP_FLICK_CUR_ABS_GR.u32;
	return ISP_FLICK_CUR_ABS_GR.bits.cur_abs_gr;
}

/*****************************************************
ISP_FLICK_CUR_ABS_GB:      
0x113a2634:                   31:10  reserved
                              22:00  cur_abs_gb
*****************************************************/
static __inline HI_VOID hi_isp_flick_cur_abs_gb_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_FLICK_CUR_ABS_GB ISP_FLICK_CUR_ABS_GB;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_FLICK_CUR_ABS_GB.u32 = pstIspReg->ISP_FLICK_CUR_ABS_GB.u32;
	ISP_FLICK_CUR_ABS_GB.bits.cur_abs_gb = data;
	pstIspReg->ISP_FLICK_CUR_ABS_GB.u32 = ISP_FLICK_CUR_ABS_GB.u32;
}

static __inline HI_U32 hi_isp_flick_cur_abs_gb_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_FLICK_CUR_ABS_GB ISP_FLICK_CUR_ABS_GB;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_FLICK_CUR_ABS_GB.u32 = pstIspReg->ISP_FLICK_CUR_ABS_GB.u32;
	return ISP_FLICK_CUR_ABS_GB.bits.cur_abs_gb;
}

/*****************************************************
ISP_FLICK_PGMEAN_RADDR:      
0x113a2688:                   31:00  gmean_raddr
*****************************************************/
static __inline HI_VOID hi_isp_flick_gmean_raddr_write(HI_U8 ispDev, HI_U32 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_FLICK_PGMEAN_RADDR ISP_FLICK_PGMEAN_RADDR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_FLICK_PGMEAN_RADDR.u32 = pstIspReg->ISP_FLICK_PGMEAN_RADDR.u32;
	ISP_FLICK_PGMEAN_RADDR.bits.gmean_raddr = data;
	pstIspReg->ISP_FLICK_PGMEAN_RADDR.u32 = ISP_FLICK_PGMEAN_RADDR.u32;
}

/*****************************************************
ISP_FLICK_PGMEAN_RDATA:      
0x113a268c:                   21:11  grmean_rdata
							  10:00	 gbmean_rdata
*****************************************************/
static __inline HI_U32 hi_isp_flick_mean_rdata_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_FLICK_PGMEAN_RDATA ISP_FLICK_PGMEAN_RDATA;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_FLICK_PGMEAN_RDATA.u32 = pstIspReg->ISP_FLICK_PGMEAN_RDATA.u32;
	return ISP_FLICK_PGMEAN_RDATA.bits.gbmean_rdata;
}

/*****************************************************
ISP_FLICK_SIZE:      
0x113a26F0:                   21:11  grmean_wdata
							  28:16	 height
							  12:00	 width
*****************************************************/
static __inline HI_VOID hi_isp_flick_width_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_FLICK_SIZE ISP_FLICK_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_FLICK_SIZE.u32 = pstIspReg->ISP_FLICK_SIZE.u32;
	ISP_FLICK_SIZE.bits.width = data;
	pstIspReg->ISP_FLICK_SIZE.u32 = ISP_FLICK_SIZE.u32;
}

static __inline HI_VOID hi_isp_flick_height_write(HI_U8 ispDev, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_FLICK_SIZE ISP_FLICK_SIZE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_FLICK_SIZE.u32 = pstIspReg->ISP_FLICK_SIZE.u32;
	ISP_FLICK_SIZE.bits.height = data;
	pstIspReg->ISP_FLICK_SIZE.u32 = ISP_FLICK_SIZE.u32;
}


/*****************************************************
MODULE        -----ISP_ISP_SPLIT-----
MODULE        -----ISP_ISP_SPLIT-----
MODULE        -----ISP_ISP_SPLIT-----
*****************************************************/
/*****************************************************
ISP_WDRSPLIT_CFG: 		
0x113a6E00: 				  21:01  reserved
							  00:00  en
*****************************************************/
static __inline HI_VOID hi_isp_wdrsplit_en_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDRSPLIT_CFG ISP_WDRSPLIT_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDRSPLIT_CFG.u32 = pstIspReg->ISP_WDRSPLIT_CFG.u32;
	ISP_WDRSPLIT_CFG.bits.en = data;
	pstIspReg->ISP_WDRSPLIT_CFG.u32 = ISP_WDRSPLIT_CFG.u32;
}

static __inline HI_U8 hi_isp_wdrsplit_en_read(HI_U8 ispDev) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_CA_CFG ISP_WDRSPLIT_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDRSPLIT_CFG.u32 = pstIspReg->ISP_WDRSPLIT_CFG.u32;
	return ISP_WDRSPLIT_CFG.bits.en;
}
/*****************************************************
ISP_WDRSPLIT_BITW: 	
0x113a6E10: 				  21:01  reserved
							  20:16  bitw_in
							  04:00  bitw_out
*****************************************************/
#define HI_ISP_WDRSPLIT_OUTBITS (12)
static __inline HI_VOID hi_isp_wdrsplit_bitw_write(HI_U8 ispDev, HI_U8 out, HI_U8 in) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDRSPLIT_BITW ISP_WDRSPLIT_BITW;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDRSPLIT_BITW.u32 = pstIspReg->ISP_WDRSPLIT_BITW.u32;
	ISP_WDRSPLIT_BITW.bits.bitw_out = out;
	pstIspReg->ISP_WDRSPLIT_BITW.u32 = ISP_WDRSPLIT_BITW.u32;
	ISP_WDRSPLIT_BITW.bits.bitw_in = in;	
	pstIspReg->ISP_WDRSPLIT_BITW.u32 = ISP_WDRSPLIT_BITW.u32;
}

/*****************************************************
ISP_WDRSPLIT_MODE: 	
0x113a6E14: 				  21:01  reserved
							  05:04  mode_in
							  01:00  mode_out
*****************************************************/
static __inline HI_VOID hi_isp_wdrsplit_mode_write(HI_U8 ispDev, HI_U8 out, HI_U8 in) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDRSPLIT_MODE ISP_WDRSPLIT_MODE;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDRSPLIT_MODE.u32 = pstIspReg->ISP_WDRSPLIT_MODE.u32;
	ISP_WDRSPLIT_MODE.bits.mode_out = out;
	pstIspReg->ISP_WDRSPLIT_MODE.u32 = ISP_WDRSPLIT_MODE.u32;
	ISP_WDRSPLIT_MODE.bits.mode_in = in;	
	pstIspReg->ISP_WDRSPLIT_MODE.u32 = ISP_WDRSPLIT_MODE.u32;
}

/*****************************************************
ISP_WDRSPLIT_OFFSETR: 		
0x113a6E20: 				  21:20  reserved
							  19:00  offset_r
*****************************************************/
static __inline HI_VOID hi_isp_wdrsplit_offset_r_write(HI_U8 ispDev, HI_U8 offset_r) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDRSPLIT_OFFSETR ISP_WDRSPLIT_OFFSETR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDRSPLIT_OFFSETR.u32 = pstIspReg->ISP_WDRSPLIT_OFFSETR.u32;
	ISP_WDRSPLIT_OFFSETR.bits.offset_r = offset_r;
	pstIspReg->ISP_WDRSPLIT_OFFSETR.u32 = ISP_WDRSPLIT_OFFSETR.u32;
}

/*****************************************************
ISP_WDRSPLIT_OFFSETGR: 		
0x113a6E24: 				  21:20  reserved
							  19:00  offset_gr
*****************************************************/
static __inline HI_VOID hi_isp_wdrsplit_offset_gr_write(HI_U8 ispDev, HI_U8 offset_gr) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDRSPLIT_OFFSETGR ISP_WDRSPLIT_OFFSETGR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDRSPLIT_OFFSETGR.u32 = pstIspReg->ISP_WDRSPLIT_OFFSETGR.u32;
	ISP_WDRSPLIT_OFFSETGR.bits.offset_gr = offset_gr;
	pstIspReg->ISP_WDRSPLIT_OFFSETGR.u32 = ISP_WDRSPLIT_OFFSETGR.u32;
}

/*****************************************************
ISP_WDRSPLIT_OFFSETGB: 	
0x113a6E28: 				  21:20  reserved
							  19:00  offset_gb
*****************************************************/
static __inline HI_VOID hi_isp_wdrsplit_offset_gb_write(HI_U8 ispDev, HI_U8 offset_gb) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDRSPLIT_OFFSETGB ISP_WDRSPLIT_OFFSETGB;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDRSPLIT_OFFSETGB.u32 = pstIspReg->ISP_WDRSPLIT_OFFSETGB.u32;
	ISP_WDRSPLIT_OFFSETGB.bits.offset_gb = offset_gb;
	pstIspReg->ISP_WDRSPLIT_OFFSETGB.u32 = ISP_WDRSPLIT_OFFSETGB.u32;
}

/*****************************************************
ISP_WDRSPLIT_OFFSETB: 		
0x113a6E2c: 				  21:20  reserved
							  19:00  offset_b
*****************************************************/
static __inline HI_VOID hi_isp_wdrsplit_offset_b_write(HI_U8 ispDev, HI_U8 offset_b) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDRSPLIT_OFFSETB ISP_WDRSPLIT_OFFSETB;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDRSPLIT_OFFSETB.u32 = pstIspReg->ISP_WDRSPLIT_OFFSETB.u32;
	ISP_WDRSPLIT_OFFSETB.bits.offset_b = offset_b;
	pstIspReg->ISP_WDRSPLIT_OFFSETB.u32 = ISP_WDRSPLIT_OFFSETB.u32;
}

/*****************************************************
ISP_WDRSPLIT_BLC: 	
0x113a6E30: 				  21:20  reserved
							  19:00  blc
*****************************************************/
static __inline HI_VOID hi_isp_wdrsplit_blc_write(HI_U8 ispDev, HI_U8 blc) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_WDRSPLIT_BLC ISP_WDRSPLIT_BLC;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_WDRSPLIT_BLC.u32 = pstIspReg->ISP_WDRSPLIT_BLC.u32;
	ISP_WDRSPLIT_BLC.bits.blc = blc;
	pstIspReg->ISP_WDRSPLIT_BLC.u32 = ISP_WDRSPLIT_BLC.u32;
}


/*****************************************************
MODULE        -----DCI-----
MODULE        -----DCI---
MODULE        -----DCI-----
*****************************************************/
/*****************************************************
ISP_DG_CFG:         DG        
0x205a1300:         31:1      reserved
                                   0      en
*****************************************************/
static __inline HI_VOID hi_isp_dci_gainpos_thr_write(HI_U8 ispDev, HI_U8 index, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_VPDCIPOSTHR ISP_VPDCIPOSTHR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_VPDCIPOSTHR.u32 = pstIspReg->ISP_VPDCIPOSTHR[index/4].u32;

	switch ((index%4)+1)
	{
		case 1:
	ISP_VPDCIPOSTHR.bits.dci_gainpos_thr1 = data; 			
	pstIspReg->ISP_VPDCIPOSTHR[index/4].u32 = ISP_VPDCIPOSTHR.u32;
			break;
		case 2:
	ISP_VPDCIPOSTHR.bits.dci_gainpos_thr2 = data; 
	pstIspReg->ISP_VPDCIPOSTHR[index/4].u32 = ISP_VPDCIPOSTHR.u32;
			break;
		case 3:
	ISP_VPDCIPOSTHR.bits.dci_gainpos_thr3 = data; 
	pstIspReg->ISP_VPDCIPOSTHR[index/4].u32 = ISP_VPDCIPOSTHR.u32;
			break;
		case 4:
	ISP_VPDCIPOSTHR.bits.dci_gainpos_thr4 = data; 
	pstIspReg->ISP_VPDCIPOSTHR[index/4].u32 = ISP_VPDCIPOSTHR.u32;
			break;
	}	
}

static __inline HI_VOID hi_isp_dci_gainpos_cbcr_write(HI_U8 ispDev, HI_U8 index, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_VPDCIPOSGAIN ISP_VPDCIPOSGAIN;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_VPDCIPOSGAIN.u32 = pstIspReg->ISP_VPDCIPOSGAIN[index/8].u32;

	switch ((index%8)+1)
	{
		case 1:
	ISP_VPDCIPOSGAIN.bits.dci_gainpos_cbcr0 = data; 			
	pstIspReg->ISP_VPDCIPOSGAIN[index/8].u32 = ISP_VPDCIPOSGAIN.u32;
			break;
		case 2:
	ISP_VPDCIPOSGAIN.bits.dci_gainpos_cbcr1 = data; 
	pstIspReg->ISP_VPDCIPOSGAIN[index/8].u32 = ISP_VPDCIPOSGAIN.u32;
			break;
		case 3:
	ISP_VPDCIPOSGAIN.bits.dci_gainpos_cbcr2 = data; 
	pstIspReg->ISP_VPDCIPOSGAIN[index/8].u32 = ISP_VPDCIPOSGAIN.u32;
			break;
		case 4:
	ISP_VPDCIPOSGAIN.bits.dci_gainpos_cbcr3 = data; 
	pstIspReg->ISP_VPDCIPOSGAIN[index/8].u32 = ISP_VPDCIPOSGAIN.u32;
			break;
		case 5:
	ISP_VPDCIPOSGAIN.bits.dci_gainpos_cbcr4 = data; 
	pstIspReg->ISP_VPDCIPOSGAIN[index/8].u32 = ISP_VPDCIPOSGAIN.u32;
			break;
		case 6:
	ISP_VPDCIPOSGAIN.bits.dci_gainpos_cbcr5 = data; 
	pstIspReg->ISP_VPDCIPOSGAIN[index/8].u32 = ISP_VPDCIPOSGAIN.u32;
			break;
		case 7:
	ISP_VPDCIPOSGAIN.bits.dci_gainpos_cbcr6 = data;
	pstIspReg->ISP_VPDCIPOSGAIN[index/8].u32 = ISP_VPDCIPOSGAIN.u32;
			break;
		case 8:
	ISP_VPDCIPOSGAIN.bits.dci_gainpos_cbcr7 = data; 
	pstIspReg->ISP_VPDCIPOSGAIN[index/8].u32 = ISP_VPDCIPOSGAIN.u32;
			break;
	}	
}

static __inline HI_VOID hi_isp_dci_gainpos_slp_write(HI_U8 ispDev, HI_U8 index, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_VPDCIPOSSLP ISP_VPDCIPOSSLP;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_VPDCIPOSSLP.u32 = pstIspReg->ISP_VPDCIPOSSLP[index/3].u32;

	switch ((index%3)+1)
	{
		case 1:
	ISP_VPDCIPOSSLP.bits.dci_gainpos_slp0 = data; 			
	pstIspReg->ISP_VPDCIPOSSLP[index/3].u32 = ISP_VPDCIPOSSLP.u32;
			break;
		case 2:
	ISP_VPDCIPOSSLP.bits.dci_gainpos_slp1 = data; 
	pstIspReg->ISP_VPDCIPOSSLP[index/3].u32 = ISP_VPDCIPOSSLP.u32;
			break;
		case 3:
	ISP_VPDCIPOSSLP.bits.dci_gainpos_slp2 = data; 
	pstIspReg->ISP_VPDCIPOSSLP[index/3].u32 = ISP_VPDCIPOSSLP.u32;
			break;
	}
}

static __inline HI_VOID hi_isp_dci_gainneg_thr_write(HI_U8 ispDev, HI_U8 index, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_VPDCINEGTHR ISP_VPDCINEGTHR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_VPDCINEGTHR.u32 = pstIspReg->ISP_VPDCINEGTHR[index/4].u32;

	switch ((index%4)+1)
	{
		case 1:
	ISP_VPDCINEGTHR.bits.dci_gainneg_thr1 = data; 			
	pstIspReg->ISP_VPDCINEGTHR[index/4].u32 = ISP_VPDCINEGTHR.u32;
			break;
		case 2:
	ISP_VPDCINEGTHR.bits.dci_gainneg_thr2 = data; 
	pstIspReg->ISP_VPDCINEGTHR[index/4].u32 = ISP_VPDCINEGTHR.u32;
			break;
		case 3:
	ISP_VPDCINEGTHR.bits.dci_gainneg_thr3 = data; 
	pstIspReg->ISP_VPDCINEGTHR[index/4].u32 = ISP_VPDCINEGTHR.u32;
			break;
		case 4:
	ISP_VPDCINEGTHR.bits.dci_gainneg_thr4 = data; 
	pstIspReg->ISP_VPDCINEGTHR[index/4].u32 = ISP_VPDCINEGTHR.u32;
			break;
	}	
}

static __inline HI_VOID hi_isp_dci_gainneg_cbcr_write(HI_U8 ispDev, HI_U8 index, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_VPDCINEGGAIN ISP_VPDCINEGGAIN;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_VPDCINEGGAIN.u32 = pstIspReg->ISP_VPDCINEGGAIN[index/8].u32;

	switch ((index%8)+1)
	{
		case 1:
	ISP_VPDCINEGGAIN.bits.dci_gainneg_cbcr0 = data; 			
	pstIspReg->ISP_VPDCINEGGAIN[index/8].u32 = ISP_VPDCINEGGAIN.u32;
			break;
		case 2:
	ISP_VPDCINEGGAIN.bits.dci_gainneg_cbcr1 = data; 
	pstIspReg->ISP_VPDCINEGGAIN[index/8].u32 = ISP_VPDCINEGGAIN.u32;
			break;
		case 3:
	ISP_VPDCINEGGAIN.bits.dci_gainneg_cbcr2 = data; 
	pstIspReg->ISP_VPDCINEGGAIN[index/8].u32 = ISP_VPDCINEGGAIN.u32;
			break;
		case 4:
	ISP_VPDCINEGGAIN.bits.dci_gainneg_cbcr3 = data; 
	pstIspReg->ISP_VPDCINEGGAIN[index/8].u32 = ISP_VPDCINEGGAIN.u32;
			break;
		case 5:
	ISP_VPDCINEGGAIN.bits.dci_gainneg_cbcr4 = data; 
	pstIspReg->ISP_VPDCINEGGAIN[index/8].u32 = ISP_VPDCINEGGAIN.u32;
			break;
		case 6:
	ISP_VPDCINEGGAIN.bits.dci_gainneg_cbcr5 = data; 
	pstIspReg->ISP_VPDCINEGGAIN[index/8].u32 = ISP_VPDCINEGGAIN.u32;
			break;
		case 7:
	ISP_VPDCINEGGAIN.bits.dci_gainneg_cbcr6 = data;
	pstIspReg->ISP_VPDCINEGGAIN[index/8].u32 = ISP_VPDCINEGGAIN.u32;
			break;
		case 8:
	ISP_VPDCINEGGAIN.bits.dci_gainneg_cbcr7 = data; 
	pstIspReg->ISP_VPDCINEGGAIN[index/8].u32 = ISP_VPDCINEGGAIN.u32;
			break;
	}	
}

static __inline HI_VOID hi_isp_dci_gainneg_slp_write(HI_U8 ispDev, HI_U8 index, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_VPDCINEGSLP ISP_VPDCINEGSLP;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_VPDCINEGSLP.u32 = pstIspReg->ISP_VPDCINEGSLP[index/3].u32;

	switch ((index%3)+1)
	{
		case 1:
	ISP_VPDCINEGSLP.bits.dci_gainneg_slp0 = data; 			
	pstIspReg->ISP_VPDCINEGSLP[index/3].u32 = ISP_VPDCINEGSLP.u32;
			break;
		case 2:
	ISP_VPDCINEGSLP.bits.dci_gainneg_slp1 = data; 
	pstIspReg->ISP_VPDCINEGSLP[index/3].u32 = ISP_VPDCINEGSLP.u32;
			break;
		case 3:
	ISP_VPDCINEGSLP.bits.dci_gainneg_slp2 = data; 
	pstIspReg->ISP_VPDCINEGSLP[index/3].u32 = ISP_VPDCINEGSLP.u32;
			break;
	}
}


static __inline HI_VOID hi_isp_dci_bin_wgt_write(HI_U8 ispDev, HI_U8 index, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_VDDCIBINWGT ISP_VDDCIBINWGT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_VDDCIBINWGT.u32 = pstIspReg->ISP_VDDCIBINWGT[index/4].u32;

	switch (index%4)
	{
		case 0:
	ISP_VDDCIBINWGT.bits.wgt_lut0 = data; 			
	pstIspReg->ISP_VDDCIBINWGT[index/4].u32 = ISP_VDDCIBINWGT.u32;
			break;
		case 1:
	ISP_VDDCIBINWGT.bits.wgt_lut1 = data; 
	pstIspReg->ISP_VDDCIBINWGT[index/4].u32 = ISP_VDDCIBINWGT.u32;
			break;
		case 2:
	ISP_VDDCIBINWGT.bits.wgt_lut2 = data; 
	pstIspReg->ISP_VDDCIBINWGT[index/4].u32 = ISP_VDDCIBINWGT.u32;
			break;
		case 3:
	ISP_VDDCIBINWGT.bits.wgt_lut3 = data; 
	pstIspReg->ISP_VDDCIBINWGT[index/4].u32 = ISP_VDDCIBINWGT.u32;
			break;
	}	
}


static __inline HI_VOID hi_isp_dci_adj_wgt_write(HI_U8 ispDev, HI_U8 index, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_VDDCIADJLUT ISP_VDDCIADJLUT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_VDDCIADJLUT.u32 = pstIspReg->ISP_VDDCIADJLUT[index/3].u32;

	switch (index%3)
	{
		case 0:
	ISP_VDDCIADJLUT.bits.adj_0 = data; 			
	pstIspReg->ISP_VDDCIADJLUT[index/3].u32 = ISP_VDDCIADJLUT.u32;
			break;
		case 1:
	ISP_VDDCIADJLUT.bits.adj_1 = data; 
	pstIspReg->ISP_VDDCIADJLUT[index/3].u32 = ISP_VDDCIADJLUT.u32;
			break;
		case 2:
	ISP_VDDCIADJLUT.bits.adj_2 = data; 
	pstIspReg->ISP_VDDCIADJLUT[index/3].u32 = ISP_VDDCIADJLUT.u32;
			break;
			break;
	}	
}

static __inline HI_VOID hi_isp_dci_div_wgt_write(HI_U8 ispDev, HI_U8 index, HI_U16 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_VDDCIDIVLUT ISP_VDDCIDIVLUT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_VDDCIDIVLUT.u32 = pstIspReg->ISP_VDDCIDIVLUT[index/2].u32;

	switch (index%2)
	{
		case 0:
	ISP_VDDCIDIVLUT.bits.dci_div0 = data; 			
	pstIspReg->ISP_VDDCIDIVLUT[index/2].u32 = ISP_VDDCIDIVLUT.u32;
			break;
		case 1:
	ISP_VDDCIDIVLUT.bits.dci_div1 = data; 
	pstIspReg->ISP_VDDCIDIVLUT[index/2].u32 = ISP_VDDCIDIVLUT.u32;
			break;
	}	
}

static __inline HI_VOID hi_isp_dci_shiftctrl_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_VPDCICTRL ISP_VPDCICTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_VPDCICTRL.u32 = pstIspReg->ISP_VPDCICTRL.u32;

	ISP_VPDCICTRL.bits.dci_shift_ctrl = data;
	pstIspReg->ISP_VPDCICTRL.u32 = ISP_VPDCICTRL.u32;
}

static __inline HI_VOID hi_isp_dci_dbg_en_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_VPDCICTRL ISP_VPDCICTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_VPDCICTRL.u32 = pstIspReg->ISP_VPDCICTRL.u32;

	ISP_VPDCICTRL.bits.dci_dbg_en = data;
	pstIspReg->ISP_VPDCICTRL.u32 = ISP_VPDCICTRL.u32;
}

static __inline HI_VOID hi_isp_dci_cbcrcmp_en_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_VPDCICTRL ISP_VPDCICTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_VPDCICTRL.u32 = pstIspReg->ISP_VPDCICTRL.u32;

	ISP_VPDCICTRL.bits.dci_cbcrcmp_en = data;
	pstIspReg->ISP_VPDCICTRL.u32 = ISP_VPDCICTRL.u32;
}

static __inline HI_VOID hi_isp_dci_cbcrsta_en_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_VPDCICTRL ISP_VPDCICTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_VPDCICTRL.u32 = pstIspReg->ISP_VPDCICTRL.u32;

	ISP_VPDCICTRL.bits.dci_cbcrsta_en = data;
	pstIspReg->ISP_VPDCICTRL.u32 = ISP_VPDCICTRL.u32;
}

static __inline HI_VOID hi_isp_dci_in_range_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_VPDCICTRL ISP_VPDCICTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_VPDCICTRL.u32 = pstIspReg->ISP_VPDCICTRL.u32;

	ISP_VPDCICTRL.bits.dci_in_range = data;
	pstIspReg->ISP_VPDCICTRL.u32 = ISP_VPDCICTRL.u32;
}

static __inline HI_VOID hi_isp_dci_out_range_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_VPDCICTRL ISP_VPDCICTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_VPDCICTRL.u32 = pstIspReg->ISP_VPDCICTRL.u32;

	ISP_VPDCICTRL.bits.dci_out_range = data;
	pstIspReg->ISP_VPDCICTRL.u32 = ISP_VPDCICTRL.u32;
}

static __inline HI_VOID hi_isp_dci_cbcrsta_write(HI_U8 ispDev, HI_U8 cr, HI_U8 cb, HI_U8 y) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_VPDCIHISBLD ISP_VPDCIHISBLD;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_VPDCIHISBLD.u32 = pstIspReg->ISP_VPDCIHISBLD.u32;

	ISP_VPDCIHISBLD.bits.dci_cbcrsta_cr = cr;
	pstIspReg->ISP_VPDCIHISBLD.u32 = ISP_VPDCIHISBLD.u32;
	ISP_VPDCIHISBLD.bits.dci_cbcrsta_cb = cb;
	pstIspReg->ISP_VPDCIHISBLD.u32 = ISP_VPDCIHISBLD.u32;
	ISP_VPDCIHISBLD.bits.dci_cbcrsta_y = y;
	pstIspReg->ISP_VPDCIHISBLD.u32 = ISP_VPDCIHISBLD.u32;
}


static __inline HI_VOID hi_isp_dci_cbcrsta_oft_write(HI_U8 ispDev, HI_U16 oft) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_VPDCIHISOFT ISP_VPDCIHISOFT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_VPDCIHISOFT.u32 = pstIspReg->ISP_VPDCIHISOFT.u32;

	ISP_VPDCIHISOFT.bits.dci_cbcrsta_oft = oft;
	pstIspReg->ISP_VPDCIHISOFT.u32 = ISP_VPDCIHISOFT.u32;
}

static __inline HI_VOID hi_isp_dci_histcor_thr_write(HI_U8 ispDev, HI_U8 thr0, HI_U8 thr1, HI_U8 thr2) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_VPDCIHISCOR ISP_VPDCIHISCOR;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_VPDCIHISCOR.u32 = pstIspReg->ISP_VPDCIHISCOR.u32;

	ISP_VPDCIHISCOR.bits.dci_histcor_thr0 = thr0;
	pstIspReg->ISP_VPDCIHISCOR.u32 = ISP_VPDCIHISCOR.u32;
	ISP_VPDCIHISCOR.bits.dci_histcor_thr1 = thr1;
	pstIspReg->ISP_VPDCIHISCOR.u32 = ISP_VPDCIHISCOR.u32;
	ISP_VPDCIHISCOR.bits.dci_histcor_thr2 = thr2;
	pstIspReg->ISP_VPDCIHISCOR.u32 = ISP_VPDCIHISCOR.u32;
}

static __inline HI_VOID hi_isp_dci_metrc_abld_write(HI_U8 ispDev, HI_U8 abld0, HI_U8 abld1, HI_U8 abld2) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_VPDCIMERBLD ISP_VPDCIMERBLD;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_VPDCIMERBLD.u32 = pstIspReg->ISP_VPDCIMERBLD.u32;

	ISP_VPDCIMERBLD.bits.dci_metrc_abld0 = abld0;
	pstIspReg->ISP_VPDCIMERBLD.u32 = ISP_VPDCIMERBLD.u32;
	ISP_VPDCIMERBLD.bits.dci_metrc_abld1 = abld1;
	pstIspReg->ISP_VPDCIMERBLD.u32 = ISP_VPDCIMERBLD.u32;
	ISP_VPDCIMERBLD.bits.dci_metrc_abld2 = abld2;
	pstIspReg->ISP_VPDCIMERBLD.u32 = ISP_VPDCIMERBLD.u32;
}

static __inline HI_VOID hi_isp_dci_org_abld_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_VPDCIMERBLD ISP_VPDCIMERBLD;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_VPDCIMERBLD.u32 = pstIspReg->ISP_VPDCIMERBLD.u32;

	ISP_VPDCIMERBLD.bits.dci_org_abld = data;
	pstIspReg->ISP_VPDCIMERBLD.u32 = ISP_VPDCIMERBLD.u32;
}

static __inline HI_VOID hi_isp_dci_hist_abld_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_VPDCIMERBLD ISP_VPDCIMERBLD;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_VPDCIMERBLD.u32 = pstIspReg->ISP_VPDCIMERBLD.u32;

	ISP_VPDCIMERBLD.bits.dci_hist_abld = data;
	pstIspReg->ISP_VPDCIMERBLD.u32 = ISP_VPDCIMERBLD.u32;
}

static __inline HI_VOID hi_isp_dci_man_adjwgt_write(HI_U8 ispDev, HI_U8 wgt0, HI_U8 wgt1, HI_U8 wgt2) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_VPDCIADJWGT ISP_VPDCIADJWGT;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_VPDCIADJWGT.u32 = pstIspReg->ISP_VPDCIADJWGT.u32;

	ISP_VPDCIADJWGT.bits.dci_man_adjwgt0 = wgt0;
	pstIspReg->ISP_VPDCIADJWGT.u32 = ISP_VPDCIADJWGT.u32;
	ISP_VPDCIADJWGT.bits.dci_man_adjwgt1 = wgt1;
	pstIspReg->ISP_VPDCIADJWGT.u32 = ISP_VPDCIADJWGT.u32;
	ISP_VPDCIADJWGT.bits.dci_man_adjwgt2 = wgt2;
	pstIspReg->ISP_VPDCIADJWGT.u32 = ISP_VPDCIADJWGT.u32;
}

static __inline HI_VOID hi_isp_dci_wgt_clip_write(HI_U8 ispDev, HI_U8 index, HI_U8 low, HI_U8 high) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_VPDCICLIP ISP_VPDCICLIP;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_VPDCICLIP.u32 = pstIspReg->ISP_VPDCICLIP[index].u32;

	ISP_VPDCICLIP.bits.dci_wgt_cliplow0 = low;
	pstIspReg->ISP_VPDCICLIP[index].u32 = ISP_VPDCICLIP.u32;
	ISP_VPDCICLIP.bits.dci_wgt_cliphigh0 = high;
	pstIspReg->ISP_VPDCICLIP[index].u32 = ISP_VPDCICLIP.u32;
}

static __inline HI_VOID hi_isp_dci_glb_gain_write(HI_U8 ispDev, HI_U8 gain0, HI_U8 gain1, HI_U8 gain2) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_VPDCIGLBGAIN ISP_VPDCIGLBGAIN;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_VPDCIGLBGAIN.u32 = pstIspReg->ISP_VPDCIGLBGAIN.u32;

	ISP_VPDCIGLBGAIN.bits.dci_glb_gain0 = gain0;
	pstIspReg->ISP_VPDCIGLBGAIN.u32 = ISP_VPDCIGLBGAIN.u32;
	ISP_VPDCIGLBGAIN.bits.dci_glb_gain1 = gain1;
	pstIspReg->ISP_VPDCIGLBGAIN.u32 = ISP_VPDCIGLBGAIN.u32;
	ISP_VPDCIGLBGAIN.bits.dci_glb_gain2 = gain2;
	pstIspReg->ISP_VPDCIGLBGAIN.u32 = ISP_VPDCIGLBGAIN.u32;
}

static __inline HI_VOID hi_isp_dci_hpos_write(HI_U8 ispDev, HI_U16 x, HI_U16 h) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_VPDCIHPOS ISP_VPDCIHPOS;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_VPDCIHPOS.u32 = pstIspReg->ISP_VPDCIHPOS.u32;

	ISP_VPDCIHPOS.bits.dci_hstart = x;
	pstIspReg->ISP_VPDCIHPOS.u32 = ISP_VPDCIHPOS.u32;
	ISP_VPDCIHPOS.bits.dci_hend = h;
	pstIspReg->ISP_VPDCIHPOS.u32 = ISP_VPDCIHPOS.u32;
}

static __inline HI_VOID hi_isp_dci_vpos_write(HI_U8 ispDev, HI_U16 y, HI_U16 v) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_VPDCIVPOS ISP_VPDCIVPOS;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_VPDCIVPOS.u32 = pstIspReg->ISP_VPDCIVPOS.u32;

	ISP_VPDCIVPOS.bits.dci_vstart = y;
	pstIspReg->ISP_VPDCIVPOS.u32 = ISP_VPDCIVPOS.u32;
	ISP_VPDCIVPOS.bits.dci_vend = v;
	pstIspReg->ISP_VPDCIVPOS.u32 = ISP_VPDCIVPOS.u32;
}


static __inline HI_VOID hi_isp_dci_en_write(HI_U8 ispDev, HI_U8 data) {
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_VPDCICTRL ISP_VPDCICTRL;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_VPDCICTRL.u32 = pstIspReg->ISP_VPDCICTRL.u32;

	ISP_VPDCICTRL.bits.dci_en = data;
	pstIspReg->ISP_VPDCICTRL.u32 = ISP_VPDCICTRL.u32;
}

static __inline HI_VOID hi_isp_color_bar_cfg_write(HI_U8 ispDev,HI_U8 data){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_COLORBAR_CFG ISP_COLORBAR_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_COLORBAR_CFG.u32 = pstIspReg->ISP_COLORBAR_CFG.u32;
	ISP_COLORBAR_CFG.bits.en = data;
	pstIspReg->ISP_COLORBAR_CFG.u32 = ISP_COLORBAR_CFG.u32;
}


static __inline HI_U8 hi_isp_color_bar_cfg_read(HI_U8 ispDev){
	S_HIISP_REGS_TYPE * pstIspReg;
	U_ISP_COLORBAR_CFG ISP_COLORBAR_CFG;
	pstIspReg = (S_HIISP_REGS_TYPE *)(hi_isp_getaddr(ispDev));
	ISP_COLORBAR_CFG.u32 = pstIspReg->ISP_COLORBAR_CFG.u32;
	return ISP_COLORBAR_CFG.bits.en;
}

static __inline HI_VOID hi_vi_top_vc_enable_write(HI_U32 data) {
    HI_U32 u32Current = IORD_32DIRECT_VI(0x01A4);
    IOWR_32DIRECT_VI(0x01A4, ((data & 0x1) << 31) | (u32Current & 0x7FFFFFFF));
}
static __inline HI_U32 hi_vi_top_vc_enable_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x01A4) & 0x80000000) >> 31);
}

static __inline HI_VOID hi_vi_top_vc_reset_write(HI_U32 data) {
    HI_U32 u32Current = IORD_32DIRECT_VI(0x01A4);
    IOWR_32DIRECT_VI(0x01A4, ((data & 0x1) << 29) | (u32Current & 0xDFFFFFFF));
}
static __inline HI_U32 hi_vi_top_vc_reset_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x01A4) & 0x20000000) >> 29);
}

static __inline HI_VOID hi_vi_top_vc_max_write(HI_U32 data) {
    HI_U32 u32Current = IORD_32DIRECT_VI(0x01A4);
    IOWR_32DIRECT_VI(0x01A4, ((data & 0x3) << 2) | (u32Current & 0xFFFFFFF3));
}
static __inline HI_U32 hi_vi_top_vc_max_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x01A4) & 0xC) >> 2);
}

HI_S32 ISP_GetLightboxGain(ISP_DEV IspDev, ISP_AWB_Calibration_Gain_S *stAWBCalibrationGain);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __ISP_CONFIG_CONFIG_EX_H__ */

