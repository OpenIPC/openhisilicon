/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include "isp_config.h"
#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_ext_config.h"

#include "comm_vi.h"
#include "ot_mpi_vi.h"
#include "vi_ext.h"
#include "isp_proc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/****************************************************************************
 * MACRO DEFINITION                                                         *
 ****************************************************************************/
#define FPN_OVERFLOWTHR 0x7C0
#define ISP_FPN_MAX_O 0xFFF
#define FPN_OVERFLOWTHR_OFF 0x3FFF
#define FPN_CHN_NUM 2
#define ISP_FPN_MODE_CORRECTION 0x0
#define ISP_FPN_MODE_CALIBRATE 0x1

#define ISP_FPN_CLIP(min, max, x) \
	((x) <= (min) ? (min) : ((x) > (max) ? (max) : (x)))

typedef enum ISP_SENSOR_BIT_WIDTH_E {
	ISP_SENSOR_8BIT = 8,
	ISP_SENSOR_10BIT = 10,
	ISP_SENSOR_12BIT = 12,
	ISP_SENSOR_14BIT = 14,
	ISP_SENSOR_16BIT = 16,
	ISP_SENSOR_32BIT = 32,
	ISP_SENSOR_BUTT
} ISP_SENSOR_BIT_WIDTH_E;

GK_S32 ISP_SetCalibrateAttr(VI_PIPE ViPipe,
			    ISP_FPN_CALIBRATE_ATTR_S *pstCalibrate)
{
	VI_FPN_ATTR_S stFpnAttr;
	GK_S32 s32Ret;
	GK_U8 u8WDRMode;

	u8WDRMode = ext_system_sensor_wdr_mode_read(ViPipe);

	if (IS_WDR_MODE(u8WDRMode)) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "Do not support FPN Calibration in WDR mode!\n");
		return GK_FAILURE;
	}

	stFpnAttr.enFpnWorkMode = FPN_MODE_CALIBRATE;
	memcpy(&stFpnAttr.stCalibrateAttr, pstCalibrate,
	       sizeof(ISP_FPN_CALIBRATE_ATTR_S));

	ext_system_fpn_cablibrate_enable_write(ViPipe, 1);

	s32Ret = MPI_VI_SetFPNAttr(ViPipe, &stFpnAttr);
	if (s32Ret != GK_SUCCESS) {
		return s32Ret;
	}

	memcpy(pstCalibrate, &stFpnAttr.stCalibrateAttr,
	       sizeof(ISP_FPN_CALIBRATE_ATTR_S));
	pstCalibrate->stFpnCaliFrame.u32Iso =
		ext_system_fpn_sensor_iso_read(ViPipe);
	ext_system_fpn_cablibrate_enable_write(ViPipe, 0);

	return GK_SUCCESS;
}

GK_U32 ISP_Fpn_GetStrength(GK_U32 u32Iso, GK_U32 u32CalibrateIso)
{
	GK_U32 u32Strength;

	u32Strength = 256 * u32Iso / DIV_0_TO_1(u32CalibrateIso);

	return u32Strength;
}

GK_S32 ISP_SetCorrectionAttr(VI_PIPE ViPipe,
			     const ISP_FPN_ATTR_S *pstCorrection)
{
	GK_S32 s32Ret;
	GK_U8 u8WDRMode;
	VI_FPN_ATTR_S stFpnAttr;

	u8WDRMode = ext_system_sensor_wdr_mode_read(ViPipe);

	if (IS_WDR_MODE(u8WDRMode)) {
		ISP_TRACE(MODULE_DBG_ERR,
			  "Do not support FPN Correction in WDR mode!\n");
		return GK_FAILURE;
	}

	stFpnAttr.enFpnWorkMode = FPN_MODE_CORRECTION;
	memcpy(&stFpnAttr.stCorrectionAttr, pstCorrection,
	       sizeof(ISP_FPN_ATTR_S));

	s32Ret = MPI_VI_SetFPNAttr(ViPipe, &stFpnAttr);
	if (s32Ret != GK_SUCCESS) {
		return s32Ret;
	}

	ext_system_manual_fpn_opmode_write(ViPipe, pstCorrection->enOpType);
	ext_system_manual_fpn_Enable_write(ViPipe, pstCorrection->bEnable);
	ext_system_manual_fpn_iso_write(ViPipe,
					pstCorrection->stFpnFrmInfo.u32Iso);
	ext_system_manual_fpn_Gain_write(ViPipe,
					 pstCorrection->stManual.u32Strength);

	return s32Ret;
}

GK_S32 ISP_GetCorrectionAttr(VI_PIPE ViPipe, ISP_FPN_ATTR_S *pstCorrection)
{
	GK_U8 u8BlkDev = 0;
	GK_U8 index = 0;
	GK_S32 s32Ret = GK_SUCCESS;
	VI_FPN_ATTR_S stTempViFpnAttr;

	s32Ret = MPI_VI_GetFPNAttr(ViPipe, &stTempViFpnAttr);
	if (s32Ret != GK_SUCCESS) {
		return s32Ret;
	}

	memcpy(pstCorrection, &stTempViFpnAttr.stCorrectionAttr,
	       sizeof(ISP_FPN_ATTR_S));
	pstCorrection->stAuto.u32Strength =
		(isp_fpn_corr_gainoffset_read(ViPipe, u8BlkDev, index) >> 16);

	return GK_SUCCESS;
}

static GK_VOID FPNExtRegsDefault(VI_PIPE ViPipe)
{
	ext_system_fpn_sensor_iso_write(ViPipe,
					ISP_EXT_SYSTEM_FPN_SENSOR_ISO_DEFAULT);
	ext_system_manual_fpn_iso_write(ViPipe,
					ISP_EXT_SYSTEM_FPN_MANU_ISO_DEFAULT);

	ext_system_manual_fpn_CorrCfg_write(
		ViPipe, ISP_EXT_SYSTEM_FPN_MANU_CORRCFG_DEFAULT);
	ext_system_manual_fpn_Gain_write(ViPipe,
					 ISP_EXT_SYSTEM_FPN_STRENGTH_DEFAULT);
	ext_system_manual_fpn_opmode_write(ViPipe,
					   ISP_EXT_SYSTEM_FPN_OPMODE_DEFAULT);
	ext_system_manual_fpn_update_write(
		ViPipe, ISP_EXT_SYSTEM_FPN_MANU_UPDATE_DEFAULT);

	ext_system_manual_fpn_Type_write(ViPipe, 0);
	ext_system_manual_fpn_Offset_write(ViPipe, 0);
	ext_system_manual_fpn_Enable_write(ViPipe, 0);
	ext_system_manual_fpn_Pixelformat_write(ViPipe, 0);
	ext_system_fpn_cablibrate_enable_write(ViPipe, 0);

	return;
}

static GK_VOID FPNRegsDefault(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfg)
{
	pstRegCfg->unKey.bit1FpnCfg = 1;

	return;
}

static GK_VOID FPNExtRegsInitialize(VI_PIPE ViPipe)
{
	return;
}

static GK_VOID FPNRegsInitialize(VI_PIPE ViPipe)
{
	return;
}

static GK_S32 FPNReadExtregs(VI_PIPE ViPipe)
{
	return 0;
}

GK_S32 FPNUpdateExtRegs(VI_PIPE ViPipe)
{
	GK_U8 i = 0;
	ISP_CTX_S *pstIspCtx = GK_NULL;
	ISP_REGCFG_S *pRegCfg = GK_NULL;
	GK_U8 u8FpnOpMode;

	ISP_GET_CTX(ViPipe, pstIspCtx);
	ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);

	ext_system_fpn_sensor_iso_write(ViPipe,
					pstIspCtx->stLinkage.u32SensorIso);
	u8FpnOpMode = ext_system_manual_fpn_opmode_read(ViPipe);

	for (i = 0; i < pRegCfg->stRegCfg.u8CfgNum; i++) {
		if (ext_system_fpn_cablibrate_enable_read(ViPipe)) {
			pRegCfg->stRegCfg.stAlgRegCfg[i]
				.stFpnRegCfg.stDynaRegCfg.u32IspFpnCalibCorr =
				ISP_FPN_MODE_CALIBRATE;
		} else {
			pRegCfg->stRegCfg.stAlgRegCfg[i]
				.stFpnRegCfg.stDynaRegCfg.u32IspFpnCalibCorr =
				ISP_FPN_MODE_CORRECTION;
		}

		if (IS_WDR_MODE(pstIspCtx->u8PreSnsWDRMode) &&
		    (u8FpnOpMode == OP_TYPE_AUTO)) {
			pRegCfg->stRegCfg.stAlgRegCfg[i]
				.stFpnRegCfg.stDynaRegCfg.u32IspFpnOverflowThr =
				FPN_OVERFLOWTHR;
		} else {
			pRegCfg->stRegCfg.stAlgRegCfg[i]
				.stFpnRegCfg.stDynaRegCfg.u32IspFpnOverflowThr =
				FPN_OVERFLOWTHR_OFF;
		}
	}

	return GK_SUCCESS;
}

GK_VOID IspSetStrength(VI_PIPE ViPipe)
{
	GK_U32 u32Iso, u32Gain, i;
	GK_U32 u32CalibrateIso;
	GK_U8 u8FpnOpMode, u8FpnEn;
	ISP_REGCFG_S *pRegCfg = GK_NULL;

	ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);

	u8FpnEn = isp_ext_system_manual_fpn_Enable_read(ViPipe);
	if (!u8FpnEn) {
		return;
	}

	u8FpnOpMode = ext_system_manual_fpn_opmode_read(ViPipe);
	if (u8FpnOpMode == OP_TYPE_MANUAL) {
		for (i = 0; i < FPN_CHN_NUM; i++) {
			pRegCfg->stRegCfg.stAlgRegCfg[0]
				.stFpnRegCfg.stDynaRegCfg.u32IspFpnStrength[i] =
				isp_ext_system_manual_fpn_Gain_read(ViPipe);
		}
		pRegCfg->stRegCfg.unKey.bit1FpnCfg = 1;
		return;
	}

	u32Iso = ext_system_fpn_sensor_iso_read(ViPipe);
	u32CalibrateIso = ext_system_manual_fpn_iso_read(ViPipe);
	u32Gain = ISP_Fpn_GetStrength(u32Iso, u32CalibrateIso);
	u32Gain = ISP_FPN_CLIP(0, 0x3FF, u32Gain);

	for (i = 0; i < FPN_CHN_NUM; i++) {
		pRegCfg->stRegCfg.stAlgRegCfg[0]
			.stFpnRegCfg.stDynaRegCfg.u32IspFpnStrength[i] =
			u32Gain;
	}

	pRegCfg->stRegCfg.unKey.bit1FpnCfg = 1;
}

GK_S32 ISP_FPNInit(VI_PIPE ViPipe, GK_VOID *pRegCfg)
{
	FPNRegsDefault(ViPipe, (ISP_REG_CFG_S *)pRegCfg);
	FPNExtRegsDefault(ViPipe);
	FPNReadExtregs(ViPipe);
	FPNRegsInitialize(ViPipe);
	FPNExtRegsInitialize(ViPipe);

	return GK_SUCCESS;
}

GK_S32 ISP_FPNRun(VI_PIPE ViPipe, const GK_VOID *pStatInfo, GK_VOID *pRegCfg,
		  GK_S32 s32Rsv)
{
	FPNUpdateExtRegs(ViPipe);
	IspSetStrength(ViPipe);

	return GK_SUCCESS;
}

GK_S32 FpnProcWrite(VI_PIPE ViPipe, ISP_CTRL_PROC_WRITE_S *pstProc)
{
	GK_U8 u8BlkDev =
		0; /* BE Num, for SBS mode, BE0 & BE1 is the same FPN config, so just 0 is ok */
	ISP_CTRL_PROC_WRITE_S stProcTmp;
	GK_U32 u32Offset = 0;
	GK_U32 u32Strength = 0;

	if ((pstProc->pcProcBuff == GK_NULL) || (pstProc->u32BuffLen == 0)) {
		return GK_FAILURE;
	}

	stProcTmp.pcProcBuff = pstProc->pcProcBuff;
	stProcTmp.u32BuffLen = pstProc->u32BuffLen;

	ISP_PROC_PRINTF(
		&stProcTmp, pstProc->u32WriteLen,
		"-----FPN CORRECT INFO------------------------------------------------------------\n");

	ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
			"%8s"
			"%7s"
			"%9s"
			"%8s",
			"En", "OpType", "Strength", "Offset\n");

	u32Offset = isp_fpn_corr_gainoffset_read(ViPipe, u8BlkDev, 0) & 0xfff;
	u32Strength =
		(isp_fpn_corr_gainoffset_read(ViPipe, u8BlkDev, 0) >> 16) &
		0xffff;

	if (isp_ext_system_manual_fpn_Enable_read(ViPipe)) {
		ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
				"%8d"
				"%4d"
				"%9d"
				"%8d",
				isp_ext_system_manual_fpn_Enable_read(ViPipe),
				ext_system_manual_fpn_opmode_read(ViPipe),
				u32Strength, u32Offset);
	} else {
		ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
				"%8d"
				"%4s"
				"%9s"
				"%8s\n",
				isp_fpn_en_read(ViPipe, u8BlkDev), "--", "--",
				"--");
	}

	pstProc->u32WriteLen += 1;

	return GK_SUCCESS;
}

GK_S32 ISP_FPNCtrl(VI_PIPE ViPipe, GK_U32 u32Cmd, GK_VOID *pValue)
{
	switch (u32Cmd) {
	case ISP_PROC_WRITE:
		FpnProcWrite(ViPipe, (ISP_CTRL_PROC_WRITE_S *)pValue);
		break;

	default:
		break;
	}
	return GK_SUCCESS;
}

GK_S32 ISP_FPNExit(VI_PIPE ViPipe)
{
	return GK_SUCCESS;
}

GK_S32 ISP_AlgRegisterFPN(VI_PIPE ViPipe)
{
	ISP_CTX_S *pstIspCtx = GK_NULL;
	ISP_ALG_NODE_S *pstAlgs = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);
	ISP_ALG_CHECK(pstIspCtx->unAlgKey.bit1Fpn);
	pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
	ISP_CHECK_POINTER(pstAlgs);

	pstAlgs->enAlgType = ISP_ALG_FPN;
	pstAlgs->stAlgFunc.pfn_alg_init = ISP_FPNInit;
	pstAlgs->stAlgFunc.pfn_alg_run = ISP_FPNRun;
	pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_FPNCtrl;
	pstAlgs->stAlgFunc.pfn_alg_exit = ISP_FPNExit;
	pstAlgs->bUsed = GK_TRUE;

	return GK_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
