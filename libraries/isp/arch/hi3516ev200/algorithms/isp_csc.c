/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_proc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define ISP_CSC_DFT_LUMA 50
#define ISP_CSC_DFT_CON 50
#define ISP_CSC_DFT_HUE 50
#define ISP_CSC_DFT_SAT 50

typedef struct ISP_CSC_ATTR {
	GK_S16 as16CSCIdc[3];
	GK_S16 as16CSCOdc[3];
	GK_S16 as16CSCCoef[9];
} ISP_CSC_ATTR;

typedef struct ISP_CSC_S {
	GK_BOOL bCscUsrEn;
	GK_BOOL bLimitedRangeEn;
	GK_BOOL bExtEn;
	GK_BOOL bCtmodeEn;
	COLOR_GAMUT_E enColorGamut;
	GK_U8 u8Hue;
	GK_U8 u8Contrast;
	GK_U8 u8Luma;
	GK_U8 u8Sat;
	ISP_CSC_ATTR stIspCscAttr;

	GK_BOOL bUpdate;
} ISP_CSC_S;

/* BT.709 -> RGB Coefficient Matrix */
static ISP_CSC_ATTR g_stCSCAttr_601 = {
	/* csc Input DC component (IDC) */
	{ 0, 0, 0 },
	/* csc Output DC component (ODC) */
	{ 0, 128, 128 },
	/* csc Multiplier coefficient */
	{ 299, 587, 114, -172, -339, 511, 511, -428, -83 },
};

/* BT.709 -> RGB Coefficient Matrix */
static ISP_CSC_ATTR g_stCSCAttr_601_limited = {
	/* csc Input DC component (IDC) */
	{ 0, 0, 0 },
	/* csc Output DC component (ODC) */
	{ 16, 128, 128 },
	/* csc Multiplier coefficient */
	{ 258, 504, 98, -148, -291, 439, 439, -368, -71 },
};

/* RGB -> YUV BT.709 Coefficient Matrix */
static ISP_CSC_ATTR g_stCSCAttr_709 = {
	/* csc Input DC component (IDC) */
	{ 0, 0, 0 },
	/* csc Output DC component (ODC) */
	{ 0, 128, 128 },
	/* csc Multiplier coefficient */
	{ 213, 715, 72, -117, -394, 511, 511, -464, -47 },
};

/* BT.709 -> RGB Coefficient Matrix */
static ISP_CSC_ATTR g_stCSCAttr_709_limited = {
	/* csc Input DC component (IDC) */
	{ 0, 0, 0 },
	/* csc Output DC component (ODC) */
	{ 16, 128, 128 },
	/* csc Multiplier coefficient */
	{ 184, 614, 62, -101, -338, 439, 439, -399, -40 },
};

static const int ISP_CSC_SIN_TABLE[61] = {
	-500, -485, -469, -454, -438, -422, -407, -391, -374, -358, -342,
	-325, -309, -292, -276, -259, -242, -225, -208, -191, -174, -156,
	-139, -122, -104, -87,	-70,  -52,  -35,  -17,	0,    17,   35,
	52,   70,   87,	  104,	122,  139,  156,  174,	191,  208,  225,
	242,  259,  276,  292,	309,  325,  342,  358,	374,  391,  407,
	422,  438,  454,  469,	485,  500
};

static const int ISP_CSC_COS_TABLE[61] = {
	866, 875, 883, 891,  899,  906,	 914, 921, 927, 934, 940, 946, 951,
	956, 961, 966, 970,  974,  978,	 982, 985, 988, 990, 993, 995, 996,
	998, 999, 999, 1000, 1000, 1000, 999, 999, 998, 996, 995, 993, 990,
	988, 985, 982, 978,  974,  970,	 966, 961, 956, 951, 946, 940, 934,
	927, 921, 914, 906,  899,  891,	 883, 875, 866
};

ISP_CSC_S g_astCscCtx[ISP_MAX_PIPE_NUM];
ISP_CSC_ATTR g_astCscAttrCtx[ISP_MAX_PIPE_NUM];

#define CSC_GET_CTX(dev, pstCtx) pstCtx = &g_astCscCtx[dev]
#define CSC_ATTR_GET_CTX(dev, pstAttrCtx) pstAttrCtx = &g_astCscAttrCtx[dev]

/* CSC Reg write interface */
static GK_VOID CscDynaRegsConfig(ISP_CSC_ATTR *pstCscAttr,
				 ISP_CSC_DYNA_CFG_S *pstDynaRegCfg)
{
	pstDynaRegCfg->s16CscCoef00 = pstCscAttr->as16CSCCoef[0] * 1024 / 1000;
	pstDynaRegCfg->s16CscCoef01 = pstCscAttr->as16CSCCoef[1] * 1024 / 1000;
	pstDynaRegCfg->s16CscCoef02 = pstCscAttr->as16CSCCoef[2] * 1024 / 1000;
	pstDynaRegCfg->s16CscCoef10 = pstCscAttr->as16CSCCoef[3] * 1024 / 1000;
	pstDynaRegCfg->s16CscCoef11 = pstCscAttr->as16CSCCoef[4] * 1024 / 1000;
	pstDynaRegCfg->s16CscCoef12 = pstCscAttr->as16CSCCoef[5] * 1024 / 1000;
	pstDynaRegCfg->s16CscCoef20 = pstCscAttr->as16CSCCoef[6] * 1024 / 1000;
	pstDynaRegCfg->s16CscCoef21 = pstCscAttr->as16CSCCoef[7] * 1024 / 1000;
	pstDynaRegCfg->s16CscCoef22 = pstCscAttr->as16CSCCoef[8] * 1024 / 1000;

	pstDynaRegCfg->s16CscInDC0 =
		SignedLeftShift(pstCscAttr->as16CSCIdc[0], 2);
	pstDynaRegCfg->s16CscInDC1 =
		SignedLeftShift(pstCscAttr->as16CSCIdc[1], 2);
	pstDynaRegCfg->s16CscInDC2 =
		SignedLeftShift(pstCscAttr->as16CSCIdc[2], 2);
	pstDynaRegCfg->s16CscOutDC0 =
		SignedLeftShift(pstCscAttr->as16CSCOdc[0], 2);
	pstDynaRegCfg->s16CscOutDC1 =
		SignedLeftShift(pstCscAttr->as16CSCOdc[1], 2);
	pstDynaRegCfg->s16CscOutDC2 =
		SignedLeftShift(pstCscAttr->as16CSCOdc[2], 2);

	pstDynaRegCfg->bResh = GK_TRUE;
}

static GK_S32 CscRegsInitialize(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfg)
{
	// Csc Regs Initialize
	GK_U8 i;
	ISP_CSC_ATTR stCscAttr;

	// Initilize Color Gamut Information
	memcpy(&stCscAttr, &g_stCSCAttr_709, sizeof(ISP_CSC_ATTR));

	// Config all four csc attributes.
	for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
		// Coefficiets config
		CscDynaRegsConfig(
			&stCscAttr,
			&pstRegCfg->stAlgRegCfg[i].stCscCfg.stDynaRegCfg);

		pstRegCfg->stAlgRegCfg[i].stCscCfg.bEnable = GK_TRUE;
	}

	// Refresh flags are forced on
	pstRegCfg->unKey.bit1CscCfg = GK_TRUE;

	return GK_SUCCESS;
}

static GK_S32 CscExtRegsInitialize(VI_PIPE ViPipe)
{
	GK_U8 i;

	ISP_CSC_ATTR stCscAttr;
	ext_system_csc_gamuttype_write(ViPipe, COLOR_GAMUT_BT709);
	memcpy(&stCscAttr, &g_stCSCAttr_709, sizeof(ISP_CSC_ATTR));

	// Write csc related parameters
	ext_system_csc_hue_write(ViPipe, ISP_CSC_DFT_HUE);
	ext_system_csc_sat_write(ViPipe, ISP_CSC_DFT_SAT);
	ext_system_csc_contrast_write(ViPipe, ISP_CSC_DFT_CON);
	ext_system_csc_luma_write(ViPipe, ISP_CSC_DFT_LUMA);
	ext_system_csc_limitrange_en_write(ViPipe, GK_FALSE);
	ext_system_csc_ext_en_write(ViPipe, GK_TRUE);
	ext_system_csc_ctmode_en_write(ViPipe, GK_TRUE);
	ext_system_csc_attr_update_write(ViPipe, GK_TRUE);

	// Write DC components
	for (i = 0; i < 3; i++) {
		ext_system_csc_dcin_write(ViPipe, i, stCscAttr.as16CSCIdc[i]);
		ext_system_csc_dcout_write(ViPipe, i, stCscAttr.as16CSCOdc[i]);
	}

	// Write Coefficients
	for (i = 0; i < 9; i++) {
		ext_system_csc_coef_write(ViPipe, i, stCscAttr.as16CSCCoef[i]);
	}

	ext_system_csc_enable_write(ViPipe, GK_TRUE);

	return GK_SUCCESS;
}

static GK_S32 CscReadExtRegs(VI_PIPE ViPipe, ISP_CSC_S *pstCsc)
{
	GK_U8 i;
	GK_U8 Gamut;

	pstCsc->bUpdate = ext_system_csc_attr_update_read(ViPipe);
	ext_system_csc_attr_update_write(ViPipe, GK_FALSE);

	if (pstCsc->bUpdate) {
		pstCsc->bLimitedRangeEn =
			ext_system_csc_limitrange_en_read(ViPipe);
		pstCsc->bExtEn = ext_system_csc_ext_en_read(ViPipe);
		pstCsc->bCtmodeEn = ext_system_csc_ctmode_en_read(ViPipe);

		Gamut = ext_system_csc_gamuttype_read(ViPipe);
		if (Gamut == COLOR_GAMUT_BT2020) {
			ISP_TRACE(
				MODULE_DBG_WARN,
				"Invalid Color Gamut type, Set back to previous gamut type!\n");
		} else {
			pstCsc->enColorGamut = Gamut;
		}

		pstCsc->u8Contrast = ext_system_csc_contrast_read(ViPipe);
		pstCsc->u8Hue = ext_system_csc_hue_read(ViPipe);
		pstCsc->u8Sat = ext_system_csc_sat_read(ViPipe);
		pstCsc->u8Luma = ext_system_csc_luma_read(ViPipe);

		for (i = 0; i < 9; i++) {
			pstCsc->stIspCscAttr.as16CSCCoef[i] =
				ext_system_csc_coef_read(ViPipe, i);
		}

		// DC components read
		for (i = 0; i < 3; i++) {
			pstCsc->stIspCscAttr.as16CSCIdc[i] =
				ext_system_csc_dcin_read(ViPipe, i);
			pstCsc->stIspCscAttr.as16CSCOdc[i] =
				ext_system_csc_dcout_read(ViPipe, i);
		}
	}

	return GK_SUCCESS;
}

GK_S32 CscCalMatrix(VI_PIPE ViPipe, ISP_CSC_S *pstCsc, ISP_CSC_ATTR *pstCscCoef)
{
	GK_S32 s32Luma = 0;
	GK_S32 s32Contrast = 0;
	GK_S32 s32Hue = 0;
	GK_S32 s32Satu = 0;
	GK_S32 i = 0;
	GK_S16 s16BlackIn = 0, s16BlackOut = 0;
	ISP_CSC_ATTR *pstCscCoefTmp = GK_NULL;

	if (pstCsc->bExtEn) {
		s32Luma = (GK_S32)pstCsc->u8Luma - 50; /* -50 ~ 50 */
		s32Luma = s32Luma * 0x80 / 50; /* -128 ~ 128 */
		s32Luma = (s32Luma == 128) ? 127 : s32Luma;
	} else {
		s32Luma = (GK_S32)pstCsc->u8Luma * 64 / 100 - 32; /* -32 ~ 32 */
		s32Luma = CLIP3(s32Luma, -128,
				128); /* Limit the result of Luma */
	}

	s32Contrast = ((GK_S32)pstCsc->u8Contrast - 50) * 2 + 100;
	s32Hue = (GK_S32)pstCsc->u8Hue * 60 / 100;
	s32Hue =
		CLIP3(s32Hue, 0, 60); /* Limit the result of Hue array select */
	s32Satu = ((GK_S32)pstCsc->u8Sat - 50) * 2 + 100;

	switch (pstCsc->enColorGamut) {
	case COLOR_GAMUT_BT601:
		pstCscCoefTmp = (pstCsc->bLimitedRangeEn) ?
					&g_stCSCAttr_601_limited :
					&g_stCSCAttr_601;
		break;
	case COLOR_GAMUT_BT709:
		pstCscCoefTmp = (pstCsc->bLimitedRangeEn) ?
					&g_stCSCAttr_709_limited :
					&g_stCSCAttr_709;
		break;
	case COLOR_GAMUT_USER:
		pstCscCoefTmp = &(pstCsc->stIspCscAttr);
		pstCscCoefTmp->as16CSCOdc[0] = (pstCsc->bLimitedRangeEn) ? 16 :
									   0;
		break;
	default:
		ISP_TRACE(MODULE_DBG_ERR, "Invalid Color Gamut type!\n");
		return GK_FAILURE;
	}

	if ((pstCsc->bCtmodeEn) && (pstCsc->enColorGamut <= COLOR_GAMUT_USER)) {
		s16BlackIn = -128;
		s16BlackOut =
			(pstCsc->bLimitedRangeEn) ?
				110 :
				128; /* 110 = (128 * 219) / 256, Full range to limited range conversion */
	} else {
		s16BlackIn = 0;
		s16BlackOut = 0;
	}

	pstCscCoef->as16CSCCoef[0] =
		(GK_S16)((s32Contrast *
			  ((GK_S32)pstCscCoefTmp->as16CSCCoef[0])) /
			 100);
	pstCscCoef->as16CSCCoef[1] =
		(GK_S16)((s32Contrast *
			  ((GK_S32)pstCscCoefTmp->as16CSCCoef[1])) /
			 100);
	pstCscCoef->as16CSCCoef[2] =
		(GK_S16)((s32Contrast *
			  ((GK_S32)pstCscCoefTmp->as16CSCCoef[2])) /
			 100);
	pstCscCoef->as16CSCCoef[3] =
		(GK_S16)((s32Contrast * s32Satu *
			  ((GK_S32)(pstCscCoefTmp->as16CSCCoef[3] *
					    ISP_CSC_COS_TABLE[s32Hue] +
				    pstCscCoefTmp->as16CSCCoef[6] *
					    ISP_CSC_SIN_TABLE[s32Hue]) /
			   1000)) /
			 10000);
	pstCscCoef->as16CSCCoef[4] =
		(GK_S16)((s32Contrast * s32Satu *
			  ((GK_S32)(pstCscCoefTmp->as16CSCCoef[4] *
					    ISP_CSC_COS_TABLE[s32Hue] +
				    pstCscCoefTmp->as16CSCCoef[7] *
					    ISP_CSC_SIN_TABLE[s32Hue]) /
			   1000)) /
			 10000);
	pstCscCoef->as16CSCCoef[5] =
		(GK_S16)((s32Contrast * s32Satu *
			  ((GK_S32)(pstCscCoefTmp->as16CSCCoef[5] *
					    ISP_CSC_COS_TABLE[s32Hue] +
				    pstCscCoefTmp->as16CSCCoef[8] *
					    ISP_CSC_SIN_TABLE[s32Hue]) /
			   1000)) /
			 10000);
	pstCscCoef->as16CSCCoef[6] =
		(GK_S16)((s32Contrast * s32Satu *
			  ((GK_S32)(pstCscCoefTmp->as16CSCCoef[6] *
					    ISP_CSC_COS_TABLE[s32Hue] -
				    pstCscCoefTmp->as16CSCCoef[3] *
					    ISP_CSC_SIN_TABLE[s32Hue]) /
			   1000)) /
			 10000);
	pstCscCoef->as16CSCCoef[7] =
		(GK_S16)((s32Contrast * s32Satu *
			  ((GK_S32)(pstCscCoefTmp->as16CSCCoef[7] *
					    ISP_CSC_COS_TABLE[s32Hue] -
				    pstCscCoefTmp->as16CSCCoef[4] *
					    ISP_CSC_SIN_TABLE[s32Hue]) /
			   1000)) /
			 10000);
	pstCscCoef->as16CSCCoef[8] =
		(GK_S16)((s32Contrast * s32Satu *
			  ((GK_S32)(pstCscCoefTmp->as16CSCCoef[8] *
					    ISP_CSC_COS_TABLE[s32Hue] -
				    pstCscCoefTmp->as16CSCCoef[5] *
					    ISP_CSC_SIN_TABLE[s32Hue]) /
			   1000)) /
			 10000);

	for (i = 0; i < 3; i++) {
		if (pstCsc->enColorGamut == COLOR_GAMUT_USER) {
			pstCscCoefTmp->as16CSCIdc[i] = 0;
		}
		pstCscCoef->as16CSCIdc[i] =
			pstCscCoefTmp->as16CSCIdc[i] + s16BlackIn;
		pstCscCoef->as16CSCOdc[i] = pstCscCoefTmp->as16CSCOdc[i];
	}

	// Add Luma
	pstCscCoef->as16CSCOdc[0] += ((GK_S16)s32Luma + s16BlackOut);

	// Update Ext regs:

	for (i = 0; i < 3; i++) {
		ext_system_csc_dcin_write(ViPipe, i, pstCscCoef->as16CSCIdc[i]);
		ext_system_csc_dcout_write(ViPipe, i,
					   pstCscCoef->as16CSCOdc[i]);
	}

	if (pstCsc->enColorGamut != COLOR_GAMUT_USER) {
		for (i = 0; i < 9; i++) {
			ext_system_csc_coef_write(ViPipe, i,
						  pstCscCoef->as16CSCCoef[i]);
		}
	}

	return GK_SUCCESS;
}

static GK_BOOL __inline CheckCscOpen(ISP_CSC_S *pstCsc)
{
	return (pstCsc->bCscUsrEn == GK_TRUE);
}

GK_S32 CscProcWrite(VI_PIPE ViPipe, ISP_CTRL_PROC_WRITE_S *pstProc)
{
	return GK_SUCCESS;
}

GK_S32 CscColorGamutinfoGet(VI_PIPE ViPipe,
			    ISP_COLORGAMMUT_INFO_S *pstColorGamutInfo)
{
	ISP_CSC_S *pstCsc = GK_NULL;

	CSC_GET_CTX(ViPipe, pstCsc);

	pstColorGamutInfo->enColorGamut = pstCsc->enColorGamut;

	return GK_SUCCESS;
}

GK_S32 ISP_CscInit(VI_PIPE ViPipe, GK_VOID *pRegCfg)
{
	ISP_REG_CFG_S *pstRegCfg = (ISP_REG_CFG_S *)pRegCfg;

	CscRegsInitialize(ViPipe, pstRegCfg);
	CscExtRegsInitialize(ViPipe);

	return GK_SUCCESS;
}

GK_S32 ISP_CscRun(VI_PIPE ViPipe, const GK_VOID *pStatInfo, GK_VOID *pRegCfg,
		  GK_S32 s32Rsv)
{
	GK_U16 i;

	ISP_CSC_ATTR *pstCscAttr = GK_NULL;
	ISP_CSC_S *pstCsc = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);
	CSC_GET_CTX(ViPipe, pstCsc);
	CSC_ATTR_GET_CTX(ViPipe, pstCscAttr);

	ISP_REG_CFG_S *pstRegCfg = (ISP_REG_CFG_S *)pRegCfg;

	if (pstIspCtx->stLinkage.bDefectPixel) {
		return GK_SUCCESS;
	}

	pstCsc->bCscUsrEn = ext_system_csc_enable_read(ViPipe);

	for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
		pstRegCfg->stAlgRegCfg[i].stCscCfg.bEnable = pstCsc->bCscUsrEn;
	}

	pstRegCfg->unKey.bit1CscCfg = 1;

	/* check hardware setting */
	if (!CheckCscOpen(pstCsc)) {
		return GK_SUCCESS;
	}

	// read ext regs
	CscReadExtRegs(ViPipe, pstCsc);

	// Calculate actual csc coefs
	CscCalMatrix(ViPipe, pstCsc, pstCscAttr);

	for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
		// Coefficiets config
		CscDynaRegsConfig(
			pstCscAttr,
			&pstRegCfg->stAlgRegCfg[i].stCscCfg.stDynaRegCfg);
	}

	// Refresh flags are forced on
	pstRegCfg->unKey.bit1CscCfg = GK_TRUE;

	return GK_SUCCESS;
}

GK_S32 ISP_CscCtrl(VI_PIPE ViPipe, GK_U32 u32Cmd, GK_VOID *pValue)
{
	switch (u32Cmd) {
	case ISP_PROC_WRITE:
		CscProcWrite(ViPipe, (ISP_CTRL_PROC_WRITE_S *)pValue);
		break;
	case ISP_COLORGAMUTINFO_GET:
		CscColorGamutinfoGet(ViPipe, (ISP_COLORGAMMUT_INFO_S *)pValue);
		break;

	default:
		break;
	}

	return GK_SUCCESS;
}

GK_S32 ISP_CscExit(VI_PIPE ViPipe)
{
	return GK_SUCCESS;
}

GK_S32 ISP_AlgRegisterCsc(VI_PIPE ViPipe)
{
	ISP_CTX_S *pstIspCtx = GK_NULL;
	ISP_ALG_NODE_S *pstAlgs = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);
	ISP_ALG_CHECK(pstIspCtx->unAlgKey.bit1Csc);
	pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
	ISP_CHECK_POINTER(pstAlgs);

	pstAlgs->enAlgType = ISP_ALG_CSC;
	pstAlgs->stAlgFunc.pfn_alg_init = ISP_CscInit;
	pstAlgs->stAlgFunc.pfn_alg_run = ISP_CscRun;
	pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_CscCtrl;
	pstAlgs->stAlgFunc.pfn_alg_exit = ISP_CscExit;
	pstAlgs->bUsed = GK_TRUE;

	return GK_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
