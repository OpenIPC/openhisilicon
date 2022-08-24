/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include <stdio.h>
#include "isp_alg.h"
#include "isp_ext_config.h"
#include "isp_config.h"
#include "isp_sensor.h"
#include "isp_math_utils.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define LSC_GAIN_MAX 1023

static const GK_U16 g_au16MeshGainDef[8] = { 512, 256, 128, 64, 0, 0, 0, 0 };

typedef struct ISP_LSC {
	GK_BOOL bLscEnable;
	GK_BOOL bLscCoefUpdata;
	GK_BOOL bLutUpdate;

	GK_U8 u8MeshScale;
	GK_U16 u16MeshStrength;
	GK_U16 u16MeshWeight;
	GK_U16 au16FirstPointPosX[ISP_STRIPING_MAX_NUM];
	GK_U16 au16CurWidth[ISP_STRIPING_MAX_NUM];
	GK_U16 u16BlkColStart;
	GK_U16 u16BlkColEnd;

	GK_U16 au16DeltaX[ISP_LSC_GRID_COL - 1];
	GK_U16 au16DeltaY[(ISP_LSC_GRID_ROW - 1) / 2];
	GK_U16 au16InvX[ISP_LSC_GRID_COL - 1];
	GK_U16 au16InvY[(ISP_LSC_GRID_ROW - 1) / 2];
} ISP_LSC_S;

ISP_LSC_S *g_pastLscCtx[ISP_MAX_PIPE_NUM] = { GK_NULL };

#define LSC_GET_CTX(dev, pstCtx) (pstCtx = g_pastLscCtx[dev])
#define LSC_SET_CTX(dev, pstCtx) (g_pastLscCtx[dev] = pstCtx)
#define LSC_RESET_CTX(dev) (g_pastLscCtx[dev] = GK_NULL)

static GK_S32 LscCtxInit(VI_PIPE ViPipe)
{
	ISP_LSC_S *pastLscCtx = GK_NULL;

	LSC_GET_CTX(ViPipe, pastLscCtx);

	if (pastLscCtx == GK_NULL) {
		pastLscCtx = (ISP_LSC_S *)ISP_MALLOC(sizeof(ISP_LSC_S));
		if (pastLscCtx == GK_NULL) {
			ISP_TRACE(MODULE_DBG_ERR,
				  "Isp[%d] LscCtx malloc memory failed!\n",
				  ViPipe);
			return ERR_CODE_ISP_NOMEM;
		}
	}

	memset(pastLscCtx, 0, sizeof(ISP_LSC_S));

	LSC_SET_CTX(ViPipe, pastLscCtx);

	return GK_SUCCESS;
}

static GK_VOID LscCtxExit(VI_PIPE ViPipe)
{
	ISP_LSC_S *pastLscCtx = GK_NULL;

	LSC_GET_CTX(ViPipe, pastLscCtx);
	ISP_FREE(pastLscCtx);
	LSC_RESET_CTX(ViPipe);
}

static GK_VOID geometricInvSize(ISP_LSC_S *pstLsc)
{
	GK_S32 i;

	for (i = 0; i < (ISP_LSC_GRID_COL - 1); i++) {
		if (pstLsc->au16DeltaX[i] != 0) {
			pstLsc->au16InvX[i] =
				(4096 * 1024 / pstLsc->au16DeltaX[i] + 512) >>
				10;
		} else {
			pstLsc->au16InvX[i] = 0;
		}
	}

	for (i = 0; i < ((ISP_LSC_GRID_ROW - 1) / 2); i++) {
		if (pstLsc->au16DeltaY[i] != 0) {
			pstLsc->au16InvY[i] =
				(4096 * 1024 / pstLsc->au16DeltaY[i] + 512) >>
				10;
		} else {
			pstLsc->au16InvY[i] = 0;
		}
	}

	return;
}

static GK_VOID LscGetLutIndex(GK_U8 u8CurBlk, ISP_LSC_S *pstLsc,
			      ISP_LSC_USR_CFG_S *pstUsrRegCfg)
{
	GK_U16 u16Width = pstLsc->au16CurWidth[u8CurBlk] >> 1;
	GK_U16 u16FirstPointPosX = pstLsc->au16FirstPointPosX[u8CurBlk] >> 1;
	GK_U16 u16LastPointPosX;
	GK_U16 u16WidthSumOfBlk;
	GK_U16 u16Dis2Right, u16Dis2Left;
	GK_U16 u16BlkColStart;
	GK_U16 u16BlkColEnd;

	u16BlkColStart = 0;
	u16BlkColEnd = 0;
	u16WidthSumOfBlk = pstLsc->au16DeltaX[0];

	while ((u16FirstPointPosX >= u16WidthSumOfBlk) &&
	       (u16FirstPointPosX != 0)) {
		u16BlkColStart++;
		u16WidthSumOfBlk += pstLsc->au16DeltaX[u16BlkColStart];
	}

	u16Dis2Right = u16WidthSumOfBlk - u16FirstPointPosX;
	u16Dis2Left = pstLsc->au16DeltaX[u16BlkColStart] - u16Dis2Right;

	pstUsrRegCfg->u16WidthOffset = u16Dis2Left;

	u16LastPointPosX = u16FirstPointPosX + u16Width;
	u16BlkColEnd = u16BlkColStart;
	while (u16LastPointPosX > u16WidthSumOfBlk) {
		u16BlkColEnd++;
		u16WidthSumOfBlk += pstLsc->au16DeltaX[u16BlkColEnd];
	}
	u16BlkColEnd += 1;

	pstLsc->u16BlkColStart = u16BlkColStart;
	pstLsc->u16BlkColEnd = u16BlkColEnd;

	return;
}

static GK_S32 LscGetGainLutInitialize(VI_PIPE ViPipe, ISP_LSC_S *pstLsc,
				      ISP_LSC_USR_CFG_S *pstUsrRegCfg)
{
	GK_U16 i, j;
	GK_U16 u16BlkColEnd;
	GK_U16 u16BlkColStart;
	GK_U16 u16IndexOffset;
	GK_U16 u16SrcIndex;
	GK_U32 u32DefGain;
	ISP_CMOS_DEFAULT_S *pstSnsDft = GK_NULL;
	const ISP_CMOS_LSC_S *pstCmosLsc = GK_NULL;

	GK_U8 u8BlendRatio = 0;
	GK_U16 u16BlendA = 0;
	GK_U16 u16BlendB = 0;
	ISP_SensorGetDefault(ViPipe, &pstSnsDft);

	u16BlkColEnd = pstLsc->u16BlkColEnd;
	u16BlkColStart = pstLsc->u16BlkColStart;

	u8BlendRatio = 256 - pstLsc->u16MeshWeight;

	if (pstSnsDft->unKey.bit1Lsc) {
		ISP_CHECK_POINTER(pstSnsDft->pstLsc);

		pstCmosLsc = pstSnsDft->pstLsc;

		for (j = 0; j < ISP_LSC_GRID_ROW; j++) {
			for (i = 0; i <= (u16BlkColEnd - u16BlkColStart); i++) {
				u16IndexOffset = j * ISP_LSC_GRID_COL;
				u16SrcIndex =
					u16IndexOffset + u16BlkColStart + i;
				// (A*(256-a) + B*a)>>8  =  A + (B-A)*a>>8
				u16BlendA = pstCmosLsc->astLscCalibTable[1]
						    .au16R_Gain[u16SrcIndex];
				u16BlendB = pstCmosLsc->astLscCalibTable[0]
						    .au16R_Gain[u16SrcIndex];
				pstUsrRegCfg->au32RGain[u16IndexOffset + i] =
					u16BlendA + (((u16BlendB - u16BlendB) *
						      u8BlendRatio) >>
						     8);

				u16BlendA = pstCmosLsc->astLscCalibTable[1]
						    .au16Gr_Gain[u16SrcIndex];
				u16BlendB = pstCmosLsc->astLscCalibTable[0]
						    .au16Gr_Gain[u16SrcIndex];
				pstUsrRegCfg->au32GrGain[u16IndexOffset + i] =
					u16BlendA + (((u16BlendB - u16BlendB) *
						      u8BlendRatio) >>
						     8);

				u16BlendA = pstCmosLsc->astLscCalibTable[1]
						    .au16Gb_Gain[u16SrcIndex];
				u16BlendB = pstCmosLsc->astLscCalibTable[0]
						    .au16Gb_Gain[u16SrcIndex];
				pstUsrRegCfg->au32GbGain[u16IndexOffset + i] =
					u16BlendA + (((u16BlendB - u16BlendB) *
						      u8BlendRatio) >>
						     8);

				u16BlendA = pstCmosLsc->astLscCalibTable[1]
						    .au16B_Gain[u16SrcIndex];
				u16BlendB = pstCmosLsc->astLscCalibTable[0]
						    .au16B_Gain[u16SrcIndex];
				pstUsrRegCfg->au32BGain[u16IndexOffset + i] =
					u16BlendA + (((u16BlendB - u16BlendB) *
						      u8BlendRatio) >>
						     8);
			}
		}
	} else {
		u32DefGain = g_au16MeshGainDef[pstLsc->u8MeshScale];

		for (j = 0; j < ISP_LSC_GRID_ROW; j++) {
			for (i = 0; i <= (u16BlkColEnd - u16BlkColStart); i++) {
				u16IndexOffset = j * ISP_LSC_GRID_COL;

				pstUsrRegCfg->au32RGain[u16IndexOffset + i] =
					u32DefGain;
				pstUsrRegCfg->au32GrGain[u16IndexOffset + i] =
					u32DefGain;
				pstUsrRegCfg->au32GbGain[u16IndexOffset + i] =
					u32DefGain;
				pstUsrRegCfg->au32BGain[u16IndexOffset + i] =
					u32DefGain;
			}
		}
	}

	for (i = 0; i < (u16BlkColEnd - u16BlkColStart); i++) {
		pstUsrRegCfg->au16DeltaX[i] =
			pstLsc->au16DeltaX[u16BlkColStart + i];
		pstUsrRegCfg->au16InvX[i] =
			pstLsc->au16InvX[u16BlkColStart + i];
	}

	for (i = 0; i < (ISP_LSC_GRID_ROW - 1) / 2; i++) {
		pstUsrRegCfg->au16DeltaY[i] = pstLsc->au16DeltaY[i];
		pstUsrRegCfg->au16InvY[i] = pstLsc->au16InvY[i];
	}

	return GK_SUCCESS;
}

static GK_VOID LscGetGainLut(VI_PIPE ViPipe, ISP_LSC_S *pstLsc,
			     ISP_LSC_USR_CFG_S *pstUsrRegCfg)
{
	GK_U16 i, j;
	GK_U16 u16BlkColEnd;
	GK_U16 u16BlkColStart;
	GK_U16 u16IndexOffset;
	GK_U16 u16SrcIndex;
	GK_U16 r_gain0, r_gain1, gr_gain0, gr_gain1, gb_gain0, gb_gain1,
		b_gain0, b_gain1;

	u16BlkColEnd = pstLsc->u16BlkColEnd;
	u16BlkColStart = pstLsc->u16BlkColStart;

	for (j = 0; j < ISP_LSC_GRID_ROW; j++) {
		for (i = 0; i <= (u16BlkColEnd - u16BlkColStart); i++) {
			u16IndexOffset = j * ISP_LSC_GRID_COL;
			u16SrcIndex = u16IndexOffset + u16BlkColStart + i;

			r_gain0 = ext_system_isp_mesh_shading_r_gain0_read(
				ViPipe, u16SrcIndex);
			r_gain1 = ext_system_isp_mesh_shading_r_gain1_read(
				ViPipe, u16SrcIndex);

			gr_gain0 = ext_system_isp_mesh_shading_gr_gain0_read(
				ViPipe, u16SrcIndex);
			gr_gain1 = ext_system_isp_mesh_shading_gr_gain1_read(
				ViPipe, u16SrcIndex);

			gb_gain0 = ext_system_isp_mesh_shading_gb_gain0_read(
				ViPipe, u16SrcIndex);
			gb_gain1 = ext_system_isp_mesh_shading_gb_gain1_read(
				ViPipe, u16SrcIndex);

			b_gain0 = ext_system_isp_mesh_shading_b_gain0_read(
				ViPipe, u16SrcIndex);
			b_gain1 = ext_system_isp_mesh_shading_b_gain1_read(
				ViPipe, u16SrcIndex);

			pstUsrRegCfg->au32RGain[u16IndexOffset + i] =
				(r_gain1 * (256 - pstLsc->u16MeshWeight) +
				 r_gain0 * pstLsc->u16MeshWeight) >>
				8;
			pstUsrRegCfg->au32GrGain[u16IndexOffset + i] =
				(gr_gain1 * (256 - pstLsc->u16MeshWeight) +
				 gr_gain0 * pstLsc->u16MeshWeight) >>
				8;
			pstUsrRegCfg->au32GbGain[u16IndexOffset + i] =
				(gb_gain1 * (256 - pstLsc->u16MeshWeight) +
				 gb_gain0 * pstLsc->u16MeshWeight) >>
				8;
			pstUsrRegCfg->au32BGain[u16IndexOffset + i] =
				(b_gain1 * (256 - pstLsc->u16MeshWeight) +
				 b_gain0 * pstLsc->u16MeshWeight) >>
				8;
		}
	}

	for (i = 0; i < (u16BlkColEnd - u16BlkColStart); i++) {
		pstUsrRegCfg->au16DeltaX[i] =
			pstLsc->au16DeltaX[u16BlkColStart + i];
		pstUsrRegCfg->au16InvX[i] =
			pstLsc->au16InvX[u16BlkColStart + i];
	}

	for (i = 0; i < (ISP_LSC_GRID_ROW - 1) / 2; i++) {
		pstUsrRegCfg->au16DeltaY[i] = pstLsc->au16DeltaY[i];
		pstUsrRegCfg->au16InvY[i] = pstLsc->au16InvY[i];
	}
}

static GK_VOID LscStaticRegsInitialize(GK_U8 i, VI_PIPE ViPipe,
				       ISP_LSC_STATIC_CFG_S *pstLscStaticRegCfg)
{
	pstLscStaticRegCfg->u8WinNumH = ISP_LSC_GRID_COL - 1;
	pstLscStaticRegCfg->u8WinNumV = ISP_LSC_GRID_ROW - 1;
	pstLscStaticRegCfg->bStaticResh = GK_TRUE;
}

static GK_VOID LscUsrRegsMultiInitialize(GK_U8 u8CurBlk, VI_PIPE ViPipe,
					 ISP_LSC_USR_CFG_S *pstUsrRegCfg)
{
	ISP_LSC_S *pstLsc = GK_NULL;

	LSC_GET_CTX(ViPipe, pstLsc);

	pstUsrRegCfg->u16MeshStr = pstLsc->u16MeshStrength;
	pstUsrRegCfg->u8MeshScale = pstLsc->u8MeshScale;

	LscGetLutIndex(u8CurBlk, pstLsc, pstUsrRegCfg);

	LscGetGainLutInitialize(ViPipe, pstLsc, pstUsrRegCfg);

	pstUsrRegCfg->u32UpdateIndex = 1;

	pstUsrRegCfg->bLutUpdate = GK_TRUE;
	pstUsrRegCfg->bCoefUpdate = GK_TRUE;
	pstUsrRegCfg->u8BufId = 0;
}

static GK_VOID LscUsrRegsSingleInitialize(VI_PIPE ViPipe,
					  ISP_LSC_USR_CFG_S *pstUsrRegCfg)
{
	GK_U16 i;
	GK_U32 u32DefGain;
	ISP_LSC_S *pstLsc = GK_NULL;

	ISP_CMOS_DEFAULT_S *pstSnsDft = GK_NULL;
	const ISP_CMOS_LSC_S *pstCmosLsc = GK_NULL;

	ISP_SensorGetDefault(ViPipe, &pstSnsDft);
	LSC_GET_CTX(ViPipe, pstLsc);
	ISP_CHECK_POINTER_VOID(pstLsc);

	pstUsrRegCfg->u16MeshStr = pstLsc->u16MeshStrength;
	pstUsrRegCfg->u8MeshScale = pstLsc->u8MeshScale;
	pstUsrRegCfg->u16WidthOffset = 0;

	memcpy(pstUsrRegCfg->au16DeltaX, pstLsc->au16DeltaX,
	       sizeof(GK_U16) * (ISP_LSC_GRID_COL - 1));
	memcpy(pstUsrRegCfg->au16InvX, pstLsc->au16InvX,
	       sizeof(GK_U16) * (ISP_LSC_GRID_COL - 1));

	memcpy(pstUsrRegCfg->au16DeltaY, pstLsc->au16DeltaY,
	       sizeof(GK_U16) * (ISP_LSC_GRID_ROW - 1) / 2);
	memcpy(pstUsrRegCfg->au16InvY, pstLsc->au16InvY,
	       sizeof(GK_U16) * (ISP_LSC_GRID_ROW - 1) / 2);

	if (pstSnsDft->unKey.bit1Lsc) {
		ISP_CHECK_POINTER_VOID(pstSnsDft->pstLsc);

		pstCmosLsc = pstSnsDft->pstLsc;

		for (i = 0; i < ISP_LSC_GRID_POINTS; i++) {
			pstUsrRegCfg->au32RGain[i] =
				(pstCmosLsc->astLscCalibTable[1].au16R_Gain[i] *
					 (256 - pstLsc->u16MeshWeight) +
				 pstCmosLsc->astLscCalibTable[0].au16R_Gain[i] *
					 pstLsc->u16MeshWeight) >>
				8;
			pstUsrRegCfg->au32GrGain[i] =
				(pstCmosLsc->astLscCalibTable[1].au16Gr_Gain[i] *
					 (256 - pstLsc->u16MeshWeight) +
				 pstCmosLsc->astLscCalibTable[0].au16Gr_Gain[i] *
					 pstLsc->u16MeshWeight) >>
				8;
			pstUsrRegCfg->au32GbGain[i] =
				(pstCmosLsc->astLscCalibTable[1].au16Gb_Gain[i] *
					 (256 - pstLsc->u16MeshWeight) +
				 pstCmosLsc->astLscCalibTable[0].au16Gb_Gain[i] *
					 pstLsc->u16MeshWeight) >>
				8;
			pstUsrRegCfg->au32BGain[i] =
				(pstCmosLsc->astLscCalibTable[1].au16B_Gain[i] *
					 (256 - pstLsc->u16MeshWeight) +
				 pstCmosLsc->astLscCalibTable[0].au16B_Gain[i] *
					 pstLsc->u16MeshWeight) >>
				8;
		}
	} else {
		u32DefGain = g_au16MeshGainDef[pstLsc->u8MeshScale];
		for (i = 0; i < ISP_LSC_GRID_POINTS; i++) {
			pstUsrRegCfg->au32RGain[i] = u32DefGain;
			pstUsrRegCfg->au32GrGain[i] = u32DefGain;
			pstUsrRegCfg->au32GbGain[i] = u32DefGain;
			pstUsrRegCfg->au32BGain[i] = u32DefGain;
		}
	}

	pstUsrRegCfg->u32UpdateIndex = 1;

	pstUsrRegCfg->bLutUpdate = GK_TRUE;
	pstUsrRegCfg->bCoefUpdate = GK_TRUE;
	pstUsrRegCfg->u8BufId = 0;
}

static GK_VOID LscRegsInitialize(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfg)
{
	GK_S32 i;
	ISP_LSC_S *pstLsc = GK_NULL;

	LSC_GET_CTX(ViPipe, pstLsc);

	if (pstRegCfg->u8CfgNum == 1) {
		LscStaticRegsInitialize(
			0, ViPipe,
			&pstRegCfg->stAlgRegCfg[0].stLscRegCfg.stStaticRegCfg);
		LscUsrRegsSingleInitialize(
			ViPipe,
			&pstRegCfg->stAlgRegCfg[0].stLscRegCfg.stUsrRegCfg);

		pstRegCfg->stAlgRegCfg[0].stLscRegCfg.bLscEn =
			pstLsc->bLscEnable;
		pstRegCfg->stAlgRegCfg[0].stLscRegCfg.bLut2SttEn = GK_TRUE;
		pstRegCfg->stAlgRegCfg[0].stBeLutUpdateCfg.bLscLutUpdate =
			GK_TRUE;
	} else {
		for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
			LscStaticRegsInitialize(
				i, ViPipe,
				&pstRegCfg->stAlgRegCfg[i]
					 .stLscRegCfg.stStaticRegCfg);
			LscUsrRegsMultiInitialize(
				i, ViPipe,
				&pstRegCfg->stAlgRegCfg[i]
					 .stLscRegCfg.stUsrRegCfg);

			pstRegCfg->stAlgRegCfg[i].stLscRegCfg.bLscEn =
				pstLsc->bLscEnable;
			pstRegCfg->stAlgRegCfg[i].stLscRegCfg.bLut2SttEn =
				GK_TRUE;
			pstRegCfg->stAlgRegCfg[i]
				.stBeLutUpdateCfg.bLscLutUpdate = GK_TRUE;
		}
	}

	pstRegCfg->unKey.bit1LscCfg = 1;

	return;
}

static GK_S32 LscExtRegsInitialize(VI_PIPE ViPipe)
{
	GK_U16 i;
#ifdef VREG_OLD
	GK_U32 u32Addr;
	GK_U8 *pVirtAddr = GK_NULL;
	GK_U8 *pVirtAddrTmp = GK_NULL;
#endif

	ISP_LSC_S *pstLsc = GK_NULL;
	ISP_CMOS_DEFAULT_S *pstSnsDft = GK_NULL;
	const ISP_CMOS_LSC_S *pstCmosLsc = GK_NULL;

	ISP_SensorGetDefault(ViPipe, &pstSnsDft);
	LSC_GET_CTX(ViPipe, pstLsc);

	ext_system_isp_mesh_shading_enable_write(ViPipe, pstLsc->bLscEnable);
	ext_system_isp_mesh_shading_attr_updata_write(ViPipe, GK_FALSE);
	ext_system_isp_mesh_shading_lut_attr_updata_write(ViPipe, GK_FALSE);
	ext_system_isp_mesh_shading_mesh_strength_write(
		ViPipe, pstLsc->u16MeshStrength);
	ext_system_isp_mesh_shading_blendratio_write(ViPipe,
						     pstLsc->u16MeshWeight);
	ext_system_isp_mesh_shading_mesh_scale_write(ViPipe,
						     pstLsc->u8MeshScale);

	for (i = 0; i < (ISP_LSC_GRID_COL - 1) / 2; i++) {
		ext_system_isp_mesh_shading_xgrid_write(ViPipe, i,
							pstLsc->au16DeltaX[i]);
	}

	for (i = 0; i < (ISP_LSC_GRID_ROW - 1) / 2; i++) {
		ext_system_isp_mesh_shading_ygrid_write(ViPipe, i,
							pstLsc->au16DeltaY[i]);
	}

	if (pstSnsDft->unKey.bit1Lsc) {
		ISP_CHECK_POINTER(pstSnsDft->pstLsc);

		pstCmosLsc = pstSnsDft->pstLsc;

#ifndef VREG_OLD
		for (i = 0; i < ISP_LSC_GRID_POINTS; i++) {
			ext_system_isp_mesh_shading_r_gain0_write(
				ViPipe, i,
				pstCmosLsc->astLscCalibTable[0].au16R_Gain[i]);
			ext_system_isp_mesh_shading_gr_gain0_write(
				ViPipe, i,
				pstCmosLsc->astLscCalibTable[0].au16Gr_Gain[i]);
			ext_system_isp_mesh_shading_gb_gain0_write(
				ViPipe, i,
				pstCmosLsc->astLscCalibTable[0].au16Gb_Gain[i]);
			ext_system_isp_mesh_shading_b_gain0_write(
				ViPipe, i,
				pstCmosLsc->astLscCalibTable[0].au16B_Gain[i]);

			ext_system_isp_mesh_shading_r_gain1_write(
				ViPipe, i,
				pstCmosLsc->astLscCalibTable[1].au16R_Gain[i]);
			ext_system_isp_mesh_shading_gr_gain1_write(
				ViPipe, i,
				pstCmosLsc->astLscCalibTable[1].au16Gr_Gain[i]);
			ext_system_isp_mesh_shading_gb_gain1_write(
				ViPipe, i,
				pstCmosLsc->astLscCalibTable[1].au16Gb_Gain[i]);
			ext_system_isp_mesh_shading_b_gain1_write(
				ViPipe, i,
				pstCmosLsc->astLscCalibTable[1].au16B_Gain[i]);
		}
#else

		u32Addr = VREG_LSC_BASE(ViPipe);
		pVirtAddrTmp = (GK_U8 *)VReg_GetVirtAddr(u32Addr);
		pVirtAddr = pVirtAddrTmp + 0x50;
		memcpy((GK_U16 *)pVirtAddr,
		       pstCmosLsc->astLscCalibTable[0].au16R_Gain,
		       ISP_LSC_GRID_POINTS * sizeof(GK_U16));

		pVirtAddr = pVirtAddrTmp + 0x8D2;
		memcpy((GK_U16 *)pVirtAddr,
		       pstCmosLsc->astLscCalibTable[0].au16Gr_Gain,
		       ISP_LSC_GRID_POINTS * sizeof(GK_U16));

		pVirtAddr = pVirtAddrTmp + 0x1154;
		memcpy((GK_U16 *)pVirtAddr,
		       pstCmosLsc->astLscCalibTable[0].au16Gb_Gain,
		       ISP_LSC_GRID_POINTS * sizeof(GK_U16));

		pVirtAddr = pVirtAddrTmp + 0x19D6;
		memcpy((GK_U16 *)pVirtAddr,
		       pstCmosLsc->astLscCalibTable[0].au16B_Gain,
		       ISP_LSC_GRID_POINTS * sizeof(GK_U16));

		pVirtAddr = pVirtAddrTmp + 0x2258;
		memcpy((GK_U16 *)pVirtAddr,
		       pstCmosLsc->astLscCalibTable[1].au16R_Gain,
		       ISP_LSC_GRID_POINTS * sizeof(GK_U16));

		pVirtAddr = pVirtAddrTmp + 0x2ADA;
		memcpy((GK_U16 *)pVirtAddr,
		       pstCmosLsc->astLscCalibTable[1].au16Gr_Gain,
		       ISP_LSC_GRID_POINTS * sizeof(GK_U16));

		pVirtAddr = pVirtAddrTmp + 0x335C;
		memcpy((GK_U16 *)pVirtAddr,
		       pstCmosLsc->astLscCalibTable[1].au16Gb_Gain,
		       ISP_LSC_GRID_POINTS * sizeof(GK_U16));

		pVirtAddr = pVirtAddrTmp + 0x3BDE;
		memcpy((GK_U16 *)pVirtAddr,
		       pstCmosLsc->astLscCalibTable[1].au16B_Gain,
		       ISP_LSC_GRID_POINTS * sizeof(GK_U16));
#endif
	} else {
#ifndef VREG_OLD

		for (i = 0; i < ISP_LSC_GRID_POINTS; i++) {
			ext_system_isp_mesh_shading_r_gain0_write(
				ViPipe, i,
				g_au16MeshGainDef[pstLsc->u8MeshScale]);
			ext_system_isp_mesh_shading_gr_gain0_write(
				ViPipe, i,
				g_au16MeshGainDef[pstLsc->u8MeshScale]);
			ext_system_isp_mesh_shading_gb_gain0_write(
				ViPipe, i,
				g_au16MeshGainDef[pstLsc->u8MeshScale]);
			ext_system_isp_mesh_shading_b_gain0_write(
				ViPipe, i,
				g_au16MeshGainDef[pstLsc->u8MeshScale]);

			ext_system_isp_mesh_shading_r_gain1_write(
				ViPipe, i,
				g_au16MeshGainDef[pstLsc->u8MeshScale]);
			ext_system_isp_mesh_shading_gr_gain1_write(
				ViPipe, i,
				g_au16MeshGainDef[pstLsc->u8MeshScale]);
			ext_system_isp_mesh_shading_gb_gain1_write(
				ViPipe, i,
				g_au16MeshGainDef[pstLsc->u8MeshScale]);
			ext_system_isp_mesh_shading_b_gain1_write(
				ViPipe, i,
				g_au16MeshGainDef[pstLsc->u8MeshScale]);
		}

#else
		u32Addr = VREG_LSC_BASE(ViPipe);
		pVirtAddrTmp = (GK_U8 *)VReg_GetVirtAddr(u32Addr);
		pVirtAddr = pVirtAddrTmp + 0x50;
		MemsetU16((GK_U16 *)pVirtAddr,
			  g_au16MeshGainDef[pstLsc->u8MeshScale],
			  ISP_LSC_GRID_POINTS);

		pVirtAddr = pVirtAddrTmp + 0x8D2;
		MemsetU16((GK_U16 *)pVirtAddr,
			  g_au16MeshGainDef[pstLsc->u8MeshScale],
			  ISP_LSC_GRID_POINTS);

		pVirtAddr = pVirtAddrTmp + 0x1154;
		MemsetU16((GK_U16 *)pVirtAddr,
			  g_au16MeshGainDef[pstLsc->u8MeshScale],
			  ISP_LSC_GRID_POINTS);

		pVirtAddr = pVirtAddrTmp + 0x19D6;
		MemsetU16((GK_U16 *)pVirtAddr,
			  g_au16MeshGainDef[pstLsc->u8MeshScale],
			  ISP_LSC_GRID_POINTS);

		pVirtAddr = pVirtAddrTmp + 0x2258;
		MemsetU16((GK_U16 *)pVirtAddr,
			  g_au16MeshGainDef[pstLsc->u8MeshScale],
			  ISP_LSC_GRID_POINTS);

		pVirtAddr = pVirtAddrTmp + 0x2ADA;
		MemsetU16((GK_U16 *)pVirtAddr,
			  g_au16MeshGainDef[pstLsc->u8MeshScale],
			  ISP_LSC_GRID_POINTS);

		pVirtAddr = pVirtAddrTmp + 0x335C;
		MemsetU16((GK_U16 *)pVirtAddr,
			  g_au16MeshGainDef[pstLsc->u8MeshScale],
			  ISP_LSC_GRID_POINTS);

		pVirtAddr = pVirtAddrTmp + 0x3BDE;
		MemsetU16((GK_U16 *)pVirtAddr,
			  g_au16MeshGainDef[pstLsc->u8MeshScale],
			  ISP_LSC_GRID_POINTS);
#endif
	}

	return GK_SUCCESS;
}

static GK_S32 LscReadExtRegs(VI_PIPE ViPipe)
{
	GK_U16 i;
	ISP_LSC_S *pstLsc = GK_NULL;

	LSC_GET_CTX(ViPipe, pstLsc);

	pstLsc->bLscCoefUpdata =
		ext_system_isp_mesh_shading_attr_updata_read(ViPipe);
	ext_system_isp_mesh_shading_attr_updata_write(ViPipe, GK_FALSE);

	if (pstLsc->bLscCoefUpdata) {
		pstLsc->u16MeshStrength =
			ext_system_isp_mesh_shading_mesh_strength_read(ViPipe);
		pstLsc->u16MeshWeight =
			ext_system_isp_mesh_shading_blendratio_read(ViPipe);
	}

	pstLsc->bLutUpdate =
		ext_system_isp_mesh_shading_lut_attr_updata_read(ViPipe);
	ext_system_isp_mesh_shading_lut_attr_updata_write(ViPipe, GK_FALSE);

	if (pstLsc->bLutUpdate) {
		pstLsc->u8MeshScale =
			ext_system_isp_mesh_shading_mesh_scale_read(ViPipe);

		for (i = 0; i < (ISP_LSC_GRID_COL - 1) / 2; i++) {
			pstLsc->au16DeltaX[i] =
				ext_system_isp_mesh_shading_xgrid_read(ViPipe,
								       i);
		}

		for (i = (ISP_LSC_GRID_COL - 1) / 2; i < ISP_LSC_GRID_COL - 1;
		     i++) {
			pstLsc->au16DeltaX[i] =
				pstLsc->au16DeltaX[ISP_LSC_GRID_COL - 2 - i];
		}

		for (i = 0; i < (ISP_LSC_GRID_ROW - 1) / 2; i++) {
			pstLsc->au16DeltaY[i] =
				ext_system_isp_mesh_shading_ygrid_read(ViPipe,
								       i);
		}

		geometricInvSize(pstLsc);
	}

	return GK_SUCCESS;
}

static GK_VOID geometricGridSizeLsc(GK_U16 *pu16Delta, GK_U16 *pu16Inv,
				    GK_U16 u16Length, GK_U16 u16GridSize)
{
	GK_U16 i, sum;
	GK_U16 u16HalfGridSize;
	GK_U16 diff;
	GK_U16 *pu16TmpStep;
	GK_U16 u16SumR;
	GK_U16 u16Tmp;
	u16HalfGridSize = (u16GridSize - 1) >> 1;

	if (u16HalfGridSize == 0) {
		return;
	}

	pu16TmpStep = (GK_U16 *)ISP_MALLOC(sizeof(GK_U16) * u16HalfGridSize);

	if (pu16TmpStep == NULL) {
		return;
	}

	memset(pu16TmpStep, 0, sizeof(GK_U16) * u16HalfGridSize);

	u16SumR = u16HalfGridSize;

	u16Tmp = ((((u16Length >> 1) << 10) / DIV_0_TO_1(u16SumR)) + 512) >> 10;

	for (i = 0; i < u16HalfGridSize; i++) {
		pu16TmpStep[i] = u16Tmp;
	}

	sum = 0;
	for (i = 0; i < u16HalfGridSize; i++) {
		sum = sum + pu16TmpStep[i];
	}

	if (sum != (u16Length >> 1)) {
		if (sum > (u16Length >> 1)) {
			diff = sum - (u16Length >> 1);
			for (i = 1; i <= diff; i++) {
				pu16TmpStep[u16HalfGridSize - i] =
					pu16TmpStep[u16HalfGridSize - i] - 1;
			}
		} else {
			diff = (u16Length >> 1) - sum;
			for (i = 1; i <= diff; i++) {
				pu16TmpStep[i - 1] = pu16TmpStep[i - 1] + 1;
			}
		}
	}

	for (i = 0; i < u16HalfGridSize; i++) {
		pu16Delta[i] = pu16TmpStep[i];
		pu16Inv[i] = (pu16Delta[i] == 0) ?
				     (0) :
				     ((4096 * 1024 / DIV_0_TO_1(pu16Delta[i]) +
				       512) >>
				      10);
	}

	ISP_FREE(pu16TmpStep);
}

static GK_VOID LscImageSize(VI_PIPE ViPipe, GK_U8 u8BlkNum, ISP_LSC_S *pstLsc)
{
	GK_U16 i;
	ISP_RECT_S stBlockRect;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);

	geometricGridSizeLsc(pstLsc->au16DeltaX, pstLsc->au16InvX,
			     pstIspCtx->stBlockAttr.stFrameRect.u32Width / 2,
			     ISP_LSC_GRID_COL);
	geometricGridSizeLsc(pstLsc->au16DeltaY, pstLsc->au16InvY,
			     pstIspCtx->stBlockAttr.stFrameRect.u32Height / 2,
			     ISP_LSC_GRID_ROW);

	for (i = 0; i < (ISP_LSC_GRID_COL - 1) / 2; i++) {
		pstLsc->au16DeltaX[ISP_LSC_GRID_COL - 2 - i] =
			pstLsc->au16DeltaX[i];
		pstLsc->au16InvX[ISP_LSC_GRID_COL - 2 - i] =
			pstLsc->au16InvX[i];
	}

	for (i = 0; i < u8BlkNum; i++) {
		ISP_GetBlockRect(&stBlockRect, &pstIspCtx->stBlockAttr, i);

		pstLsc->au16CurWidth[i] = stBlockRect.u32Width;
		pstLsc->au16FirstPointPosX[i] = stBlockRect.s32X;
	}

	return;
}

static GK_S32 LscCheckCmosParam(VI_PIPE ViPipe,
				const ISP_CMOS_LSC_S *pstCmosLsc)
{
	GK_U16 i;

	if (pstCmosLsc->u8MeshScale > 7) {
		ISP_TRACE(MODULE_DBG_ERR, "err u8MeshScale %d!\n",
			  pstCmosLsc->u8MeshScale);
		return ERR_CODE_ISP_ILLEGAL_PARAM;
	}

	if (pstCmosLsc->u16MeshStrength > 1023) {
		ISP_TRACE(MODULE_DBG_ERR, "err u16MeshStrength %d!\n",
			  pstCmosLsc->u16MeshStrength);
		return ERR_CODE_ISP_ILLEGAL_PARAM;
	}

	for (i = 0; i < ISP_LSC_GRID_POINTS; i++) {
		if ((pstCmosLsc->astLscCalibTable[0].au16R_Gain[i] >
		     LSC_GAIN_MAX) ||
		    (pstCmosLsc->astLscCalibTable[0].au16Gr_Gain[i] >
		     LSC_GAIN_MAX) ||
		    (pstCmosLsc->astLscCalibTable[0].au16Gb_Gain[i] >
		     LSC_GAIN_MAX) ||
		    (pstCmosLsc->astLscCalibTable[0].au16B_Gain[i] >
		     LSC_GAIN_MAX) ||
		    (pstCmosLsc->astLscCalibTable[1].au16R_Gain[i] >
		     LSC_GAIN_MAX) ||
		    (pstCmosLsc->astLscCalibTable[1].au16Gr_Gain[i] >
		     LSC_GAIN_MAX) ||
		    (pstCmosLsc->astLscCalibTable[1].au16Gb_Gain[i] >
		     LSC_GAIN_MAX) ||
		    (pstCmosLsc->astLscCalibTable[1].au16B_Gain[i] >
		     LSC_GAIN_MAX)) {
			ISP_TRACE(MODULE_DBG_ERR, "err Gain!\n");
			return ERR_CODE_ISP_ILLEGAL_PARAM;
		}
	}
	return GK_SUCCESS;
}

static GK_S32 LscInitialize(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfg)
{
	GK_S32 s32Ret;
	ISP_LSC_S *pstLsc = GK_NULL;
	ISP_CMOS_DEFAULT_S *pstSnsDft = GK_NULL;

	LSC_GET_CTX(ViPipe, pstLsc);
	ISP_SensorGetDefault(ViPipe, &pstSnsDft);

	pstLsc->u16MeshWeight = ISP_LSC_DEFAULT_WEIGHT;

	if (pstSnsDft->unKey.bit1Lsc) {
		ISP_CHECK_POINTER(pstSnsDft->pstLsc);

		s32Ret = LscCheckCmosParam(ViPipe, pstSnsDft->pstLsc);
		if (s32Ret != GK_SUCCESS) {
			return s32Ret;
		}

		pstLsc->u16MeshStrength = pstSnsDft->pstLsc->u16MeshStrength;
		pstLsc->u8MeshScale = pstSnsDft->pstLsc->u8MeshScale;
	} else {
		pstLsc->u16MeshStrength = ISP_LSC_DEFAULT_MESH_STRENGTH;
		pstLsc->u8MeshScale = ISP_LSC_DEFAULT_MESH_SCALE;
	}

	LscImageSize(ViPipe, pstRegCfg->u8CfgNum, pstLsc);

	pstLsc->bLscEnable = GK_TRUE;

	return GK_SUCCESS;
}

static GK_VOID Lsc_Usr_Fw(VI_PIPE ViPipe, GK_U8 u8CurBlk, ISP_LSC_S *pstLsc,
			  ISP_LSC_USR_CFG_S *pstUsrRegCfg)
{
	LscGetLutIndex(u8CurBlk, pstLsc, pstUsrRegCfg);

	LscGetGainLut(ViPipe, pstLsc, pstUsrRegCfg);

	pstUsrRegCfg->u8MeshScale = pstLsc->u8MeshScale;
	pstUsrRegCfg->u32UpdateIndex += 1;
	pstUsrRegCfg->bLutUpdate = GK_TRUE;
}

static GK_VOID Lsc_Usr_Fw_Normal(VI_PIPE ViPipe, ISP_LSC_S *pstLsc,
				 ISP_LSC_USR_CFG_S *pstUsrRegCfg)
{
	GK_U16 i;
	GK_U16 r_gain0, r_gain1, gr_gain0, gr_gain1, gb_gain0, gb_gain1,
		b_gain0, b_gain1;

	pstUsrRegCfg->u16WidthOffset = 0;
	pstUsrRegCfg->u8MeshScale = pstLsc->u8MeshScale;
	pstUsrRegCfg->u32UpdateIndex += 1;
	pstUsrRegCfg->bLutUpdate = GK_TRUE;

	memcpy(pstUsrRegCfg->au16DeltaX, pstLsc->au16DeltaX,
	       sizeof(GK_U16) * (ISP_LSC_GRID_COL - 1));
	memcpy(pstUsrRegCfg->au16InvX, pstLsc->au16InvX,
	       sizeof(GK_U16) * (ISP_LSC_GRID_COL - 1));

	memcpy(pstUsrRegCfg->au16DeltaY, pstLsc->au16DeltaY,
	       sizeof(GK_U16) * (ISP_LSC_GRID_ROW - 1) / 2);
	memcpy(pstUsrRegCfg->au16InvY, pstLsc->au16InvY,
	       sizeof(GK_U16) * (ISP_LSC_GRID_ROW - 1) / 2);

	for (i = 0; i < ISP_LSC_GRID_POINTS; i++) {
		r_gain0 = ext_system_isp_mesh_shading_r_gain0_read(ViPipe, i);
		r_gain1 = ext_system_isp_mesh_shading_r_gain1_read(ViPipe, i);

		gr_gain0 = ext_system_isp_mesh_shading_gr_gain0_read(ViPipe, i);
		gr_gain1 = ext_system_isp_mesh_shading_gr_gain1_read(ViPipe, i);

		gb_gain0 = ext_system_isp_mesh_shading_gb_gain0_read(ViPipe, i);
		gb_gain1 = ext_system_isp_mesh_shading_gb_gain1_read(ViPipe, i);

		b_gain0 = ext_system_isp_mesh_shading_b_gain0_read(ViPipe, i);
		b_gain1 = ext_system_isp_mesh_shading_b_gain1_read(ViPipe, i);

		pstUsrRegCfg->au32RGain[i] =
			(r_gain1 * (256 - pstLsc->u16MeshWeight) +
			 r_gain0 * pstLsc->u16MeshWeight) >>
			8;
		pstUsrRegCfg->au32GrGain[i] =
			(gr_gain1 * (256 - pstLsc->u16MeshWeight) +
			 gr_gain0 * pstLsc->u16MeshWeight) >>
			8;
		pstUsrRegCfg->au32GbGain[i] =
			(gb_gain1 * (256 - pstLsc->u16MeshWeight) +
			 gb_gain0 * pstLsc->u16MeshWeight) >>
			8;
		pstUsrRegCfg->au32BGain[i] =
			(b_gain1 * (256 - pstLsc->u16MeshWeight) +
			 b_gain0 * pstLsc->u16MeshWeight) >>
			8;
	}
}

static GK_BOOL __inline CheckLscOpen(ISP_LSC_S *pstLsc)
{
	return (pstLsc->bLscEnable == GK_TRUE);
}

static GK_S32 LscImageResWrite(VI_PIPE ViPipe, ISP_REG_CFG_S *pstRegCfg)
{
	GK_U8 i;
	ISP_LSC_S *pstLsc = GK_NULL;

	LSC_GET_CTX(ViPipe, pstLsc);
	ISP_CHECK_POINTER(pstLsc);

	LscImageSize(ViPipe, pstRegCfg->u8CfgNum, pstLsc);

	LscExtRegsInitialize(ViPipe);

	if (pstRegCfg->u8CfgNum == 1) {
		Lsc_Usr_Fw_Normal(
			ViPipe, pstLsc,
			&pstRegCfg->stAlgRegCfg[0].stLscRegCfg.stUsrRegCfg);
	} else {
		for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
			Lsc_Usr_Fw(ViPipe, i, pstLsc,
				   &pstRegCfg->stAlgRegCfg[i]
					    .stLscRegCfg.stUsrRegCfg);
		}
	}

	pstRegCfg->unKey.bit1LscCfg = 1;

	return GK_SUCCESS;
}

GK_S32 ISP_LscInit(VI_PIPE ViPipe, GK_VOID *pRegCfg)
{
	GK_S32 s32Ret = GK_SUCCESS;
	ISP_REG_CFG_S *pstRegCfg = (ISP_REG_CFG_S *)pRegCfg;
	s32Ret = LscCtxInit(ViPipe);

	if (s32Ret != GK_SUCCESS) {
		return s32Ret;
	}

	s32Ret = LscInitialize(ViPipe, pstRegCfg);
	if (s32Ret != GK_SUCCESS) {
		return GK_FAILURE;
	}

	LscRegsInitialize(ViPipe, pstRegCfg);
	LscExtRegsInitialize(ViPipe);

	return GK_SUCCESS;
}

GK_S32 ISP_LscRun(VI_PIPE ViPipe, const GK_VOID *pStatInfo, GK_VOID *pRegCfg,
		  GK_S32 s32Rsv)
{
	GK_S32 i;
	ISP_CTX_S *pstIspCtx = GK_NULL;
	ISP_LSC_S *pstLsc = GK_NULL;
	ISP_REG_CFG_S *pstRegCfg = (ISP_REG_CFG_S *)pRegCfg;

	ISP_GET_CTX(ViPipe, pstIspCtx);
	LSC_GET_CTX(ViPipe, pstLsc);
	ISP_CHECK_POINTER(pstLsc);

	if (pstIspCtx->stLinkage.bDefectPixel) {
		return GK_SUCCESS;
	}

	pstLsc->bLscEnable = ext_system_isp_mesh_shading_enable_read(ViPipe);

	for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
		pstRegCfg->stAlgRegCfg[i].stLscRegCfg.bLscEn =
			pstLsc->bLscEnable;
	}

	pstRegCfg->unKey.bit1LscCfg = 1;

	/* check hardware setting */
	if (!CheckLscOpen(pstLsc)) {
		return GK_SUCCESS;
	}

	LscReadExtRegs(ViPipe);

	if (pstLsc->bLscCoefUpdata) {
		for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
			pstRegCfg->stAlgRegCfg[i]
				.stLscRegCfg.stUsrRegCfg.bCoefUpdate = GK_TRUE;
			pstRegCfg->stAlgRegCfg[i]
				.stLscRegCfg.stUsrRegCfg.u16MeshStr =
				pstLsc->u16MeshStrength;
		}
	}

	if (pstLsc->bLutUpdate || pstLsc->bLscCoefUpdata) {
		if (pstRegCfg->u8CfgNum == 1) {
			Lsc_Usr_Fw_Normal(ViPipe, pstLsc,
					  &pstRegCfg->stAlgRegCfg[0]
						   .stLscRegCfg.stUsrRegCfg);
		} else {
			for (i = 0; i < pstRegCfg->u8CfgNum; i++) {
				Lsc_Usr_Fw(ViPipe, i, pstLsc,
					   &pstRegCfg->stAlgRegCfg[i]
						    .stLscRegCfg.stUsrRegCfg);
			}
		}
	}

	return GK_SUCCESS;
}

GK_S32 ISP_LscCtrl(VI_PIPE ViPipe, GK_U32 u32Cmd, GK_VOID *pValue)
{
	ISP_REGCFG_S *pRegCfg = GK_NULL;
	ISP_CTX_S *pstIspCtx = GK_NULL;

	switch (u32Cmd) {
	case ISP_CHANGE_IMAGE_MODE_SET:
		ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);
		ISP_CHECK_POINTER(pRegCfg);
		LscImageResWrite(ViPipe, &pRegCfg->stRegCfg);
		break;
	case ISP_WDR_MODE_SET:
		ISP_GET_CTX(ViPipe, pstIspCtx);

		if (pstIspCtx->stBlockAttr.u8BlockNum !=
		    pstIspCtx->stBlockAttr.u8PreBlockNum) {
			ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);
			ISP_CHECK_POINTER(pRegCfg);
			LscImageResWrite(ViPipe, &pRegCfg->stRegCfg);
		}
		break;
	default:
		break;
	}
	return GK_SUCCESS;
}

GK_S32 ISP_LscExit(VI_PIPE ViPipe)
{
	GK_U8 i;
	ISP_REGCFG_S *pRegCfg = GK_NULL;

	ISP_REGCFG_GET_CTX(ViPipe, pRegCfg);

	for (i = 0; i < pRegCfg->stRegCfg.u8CfgNum; i++) {
		pRegCfg->stRegCfg.stAlgRegCfg[i].stLscRegCfg.bLscEn = GK_FALSE;
	}

	pRegCfg->stRegCfg.unKey.bit1LscCfg = 1;

	LscCtxExit(ViPipe);

	return GK_SUCCESS;
}

GK_S32 ISP_AlgRegisterLsc(VI_PIPE ViPipe)
{
	ISP_CTX_S *pstIspCtx = GK_NULL;
	ISP_ALG_NODE_S *pstAlgs = GK_NULL;

	ISP_GET_CTX(ViPipe, pstIspCtx);
	ISP_ALG_CHECK(pstIspCtx->unAlgKey.bit1Lsc);
	pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
	ISP_CHECK_POINTER(pstAlgs);

	pstAlgs->enAlgType = ISP_ALG_LSC;
	pstAlgs->stAlgFunc.pfn_alg_init = ISP_LscInit;
	pstAlgs->stAlgFunc.pfn_alg_run = ISP_LscRun;
	pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_LscCtrl;
	pstAlgs->stAlgFunc.pfn_alg_exit = ISP_LscExit;
	pstAlgs->bUsed = GK_TRUE;

	return GK_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
