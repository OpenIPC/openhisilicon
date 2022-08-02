/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include "mkp_isp.h"
#include "ot_mpi_sys.h"
#include "isp_sensor.h"
#include "isp_main.h"
#include "isp_alg.h"
#include "isp_dnginfo.h"
#include "isp_ext_config.h"
#include "isp_defines.h"

extern GK_S32 g_as32IspFd[ISP_MAX_PIPE_NUM];

GK_S32 ISP_DngInfoInit(VI_PIPE ViPipe)
{
    GK_U64 u64PhyAddr;
    ISP_CTX_S *pstIspCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    u64PhyAddr = pstIspCtx->stIspTransInfo.stDngInfo.u64PhyAddr;

    pstIspCtx->stDngInfoCtrl.pstIspDng = MPI_SYS_Mmap(u64PhyAddr, sizeof(DNG_IMAGE_STATIC_INFO_S));

    if (pstIspCtx->stDngInfoCtrl.pstIspDng == GK_NULL) {
        ISP_TRACE(MODULE_DBG_ERR, "isp[%d] mmap Dng info buf failed!\n", ViPipe);
        return ERR_CODE_ISP_NOMEM;
    }

    return GK_SUCCESS;
}

GK_S32 ISP_DngInfoExit(VI_PIPE ViPipe)
{
    ISP_CTX_S *pstIspCtx = GK_NULL;

    ISP_CHECK_PIPE(ViPipe);
    ISP_GET_CTX(ViPipe, pstIspCtx);

    if (pstIspCtx->stDngInfoCtrl.pstIspDng != GK_NULL) {
        MPI_SYS_Munmap(pstIspCtx->stDngInfoCtrl.pstIspDng, sizeof(DNG_IMAGE_STATIC_INFO_S));
        pstIspCtx->stDngInfoCtrl.pstIspDng = GK_NULL;
    }

    return GK_SUCCESS;
}

GK_BOOL ISP_DngColorParamCheck(ISP_DNG_CCM_S *pstDngCcm, ISP_DNG_CCM_S *pstPreDngCcm)
{
    GK_BOOL bChanged = GK_FALSE;
    GK_U8 i;

    for (i = 0; i < 9; i++) {
        if (pstDngCcm->au16LowCcm[i] != pstPreDngCcm->au16LowCcm[i]) {
            bChanged = GK_TRUE;
            return bChanged;
        }
    }

    for (i = 0; i < 9; i++) {
        if (pstDngCcm->au16HighCcm[i] != pstPreDngCcm->au16HighCcm[i]) {
            bChanged = GK_TRUE;
            return bChanged;
        }
    }

    if (pstDngCcm->u16HighColorTemp != pstPreDngCcm->u16HighColorTemp) {
        bChanged = GK_TRUE;
        return bChanged;
    }

    if (pstDngCcm->u16LowColorTemp != pstPreDngCcm->u16LowColorTemp) {
        bChanged = GK_TRUE;
        return bChanged;
    }

    return bChanged;
}

void ISP_CcmDataFormat(GK_U16 *pu16CcmIn, GK_DOUBLE *pdCcmOut)
{
    GK_U8 i;
    GK_S16 tmp;
    for (i = 0; i < 9; i++) {
        tmp = (GK_S16)pu16CcmIn[i];
        pdCcmOut[i] = (GK_DOUBLE)tmp / 256;
    }
}

const double kNearZero = 1.0E-10;

static const double SRGB_to_XYZD50[9] = { 0.4361, 0.3851, 0.1431, 0.2225, 0.7169, 0.0606, 0.0139, 0.0971, 0.7141 };
static const double SRGB_to_XYZA[9] = { 0.4969, 0.4388, 0.1630, 0.2225, 0.7169, 0.0606, 0.0060, 0.0419, 0.3080 };

static const double XYZD50_to_sRGB[9] = { 3.1340, -1.6169, -0.4907, -0.9784, 1.9159, 0.0334, 0.0720, -0.2290, 1.4049 };

double Abs_double(double x)
{
    return (x < 0.0 ? -x : x);
}

void Invert3by3(const double *MatrixA, double *InvMatrix)
{
    double a00 = MatrixA[0];
    double a01 = MatrixA[1];
    double a02 = MatrixA[2];
    double a10 = MatrixA[3];
    double a11 = MatrixA[4];
    double a12 = MatrixA[5];
    double a20 = MatrixA[6];
    double a21 = MatrixA[7];
    double a22 = MatrixA[8];
    double temp[9];
    double det;
    int i;

    temp[0] = a11 * a22 - a21 * a12;
    temp[1] = a21 * a02 - a01 * a22;
    temp[2] = a01 * a12 - a11 * a02;
    temp[3] = a20 * a12 - a10 * a22;
    temp[4] = a00 * a22 - a20 * a02;
    temp[5] = a10 * a02 - a00 * a12;
    temp[6] = a10 * a21 - a20 * a11;
    temp[7] = a20 * a01 - a00 * a21;
    temp[8] = a00 * a11 - a10 * a01;

    det = (a00 * temp[0] +
           a01 * temp[3] +
           a02 * temp[6]);

    if (Abs_double(det) < kNearZero) {
        return;
    }

    for (i = 0; i < 3 * 3; i++) {
        InvMatrix[i] = temp[i] / det;
    }

    return;
}

void MultiMatrix3x3(const double *MatrixA, const double *MatrixB, double *MutMatrix)
{
    int i, j, k;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            double temp = 0;

            for (k = 0; k < 3; ++k) {
                temp += MatrixA[i * 3 + k] * MatrixB[k * 3 + j];
            }
            MutMatrix[i * 3 + j] = temp;
        }
    }
}

void ISP_Dng_LightSource_Checker(GK_U16 u16ColorTmep, GK_U8 *pu8LightSource)
{
    /* Light source, actually this means white balance setting. '0' means unknown, '1' daylight, '2'
           fluorescent, '3' tungsten, '10' flash, '17' standard light A, '18' standard light B, '19' standard light
           C, '20' D55, '21' D65, '22' D75, '255' other */
    if (u16ColorTmep >= (7500 - 500)) { /* D75 7500 */
        *pu8LightSource = 22;
    } else if ((u16ColorTmep < (6500 + 500)) && (u16ColorTmep >= (6500 - 500))) { /* D65 6500 */
        *pu8LightSource = 21;
    } else if ((u16ColorTmep < (5500 + 500)) && (u16ColorTmep >= (5500 - 250))) { /* D55 5500 */
        *pu8LightSource = 20;
    } else if ((u16ColorTmep < (5000 + 250)) && (u16ColorTmep >= (5000 - 100))) { /* daylight 5000 */
        *pu8LightSource = 23;
    } else if ((u16ColorTmep < (4800 + 100)) && (u16ColorTmep >= (4800 - 550))) { /* B 4800 */
        *pu8LightSource = 18;
    } else if ((u16ColorTmep < (4000 + 250)) && (u16ColorTmep >= (4000 - 800))) { /* fluorescent 4000 */
        *pu8LightSource = 2;
    } else if (u16ColorTmep < (2800 + 400)) { /* A 2800 */
        *pu8LightSource = 17;
    }
}

GK_VOID ISP_DngExtRead(VI_PIPE ViPipe, ISP_DNG_COLORPARAM_S *pstDngColorParam)
{
    pstDngColorParam->stWbGain1.u16Rgain = ext_system_dnggh_wb_gain_r_read(ViPipe);
    pstDngColorParam->stWbGain1.u16Ggain = ext_system_dnggh_wb_gain_g_read(ViPipe);
    pstDngColorParam->stWbGain1.u16Bgain = ext_system_dnggh_wb_gain_b_read(ViPipe);

    pstDngColorParam->stWbGain2.u16Rgain = ext_system_dng_low_wb_gain_r_read(ViPipe);
    pstDngColorParam->stWbGain2.u16Ggain = ext_system_dng_low_wb_gain_g_read(ViPipe);
    pstDngColorParam->stWbGain2.u16Bgain = ext_system_dng_low_wb_gain_b_read(ViPipe);
}

GK_S32 ISP_DngColorParamUpdate(VI_PIPE ViPipe)
{
    ISP_CTX_S *pstIspCtx = GK_NULL;
    AWB_CCM_CONFIG_S stAwbCcmConfig = { 0 };
    ISP_DNG_CCM_S stDngCcm = { 0 };
    GK_DOUBLE adD50Ccm[9] = { 0 }, adACcm[9] = { 0 };
    GK_DOUBLE adD50Wbgain[9] = { 0 }, adAWbgain[9] = { 0 };
    GK_U16 au16D50Wbgain[9] = { 0 }, au16AWbgain[9] = { 0 };
    GK_DOUBLE adAMultMatrix[9] = { 0 }, adD50MultMatrix[9] = { 0 };
    GK_DOUBLE adInvAColorMatrix[9] = { 0 }, adInvD50ColorMatrix[9] = { 0 };
    GK_DOUBLE adAColorMatrix[9] = { 0 }, adD50ColorMatrix[9] = { 0 };
    GK_DOUBLE adAForwardMatrix[9] = { 0 }, adD50ForwardMatrix[9] = { 0 };
    ISP_DNG_COLORPARAM_S stDngColorParam = { 0 };
    GK_U8 i;
    VI_PIPE AWB_DEV = (ViPipe ? 0 : 1);

    GK_BOOL bChanged = GK_FALSE;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    ISP_DngExtRead(ViPipe, &stDngColorParam);

    /* DNG color param have not set */
    if (stDngColorParam.stWbGain1.u16Ggain == 0) {
        return GK_SUCCESS;
    }

    memset(au16AWbgain, 0, sizeof(GK_U16) * 9);
    memset(au16D50Wbgain, 0, sizeof(GK_U16) * 9);

    /* get CCM from AWB libiray */
    switch (pstIspCtx->stLinkage.enSnapType) {
        default:

        case SNAP_TYPE_NORMAL:
        case SNAP_TYPE_PRO:
            if ((pstIspCtx->stLinkage.enSnapPipeMode == ISP_SNAP_PICTURE) && (pstIspCtx->stLinkage.bLoadCCM == GK_TRUE)) {
                ISP_AlgsCtrl(pstIspCtx->astAlgs, AWB_DEV, AWB_CCM_CONFIG_GET, (GK_VOID *)&stAwbCcmConfig);
            } else {
                ISP_AlgsCtrl(pstIspCtx->astAlgs, ViPipe, AWB_CCM_CONFIG_GET, (GK_VOID *)&stAwbCcmConfig);
            }
            break;
    }

    memcpy(&stDngCcm.au16LowCcm, &stAwbCcmConfig.au16LowCCM, sizeof(GK_U16) * 9);
    memcpy(&stDngCcm.au16HighCcm, &stAwbCcmConfig.au16HighCCM, sizeof(GK_U16) * 9);
    stDngCcm.u16HighColorTemp = stAwbCcmConfig.u16HighColorTemp;
    stDngCcm.u16LowColorTemp = stAwbCcmConfig.u16LowColorTemp;

    /* if CCM or WB gain changed, recaculate color parameters */
    bChanged = ISP_DngColorParamCheck(&stDngCcm, &pstIspCtx->stPreDngCcm);
    if (!bChanged) {
        if ((pstIspCtx->stPreDngColorParam.stWbGain1.u16Rgain != stDngColorParam.stWbGain1.u16Rgain) ||
            (pstIspCtx->stPreDngColorParam.stWbGain1.u16Ggain != stDngColorParam.stWbGain1.u16Ggain) ||
            (pstIspCtx->stPreDngColorParam.stWbGain1.u16Bgain != stDngColorParam.stWbGain1.u16Bgain) ||
            (pstIspCtx->stPreDngColorParam.stWbGain2.u16Rgain != stDngColorParam.stWbGain2.u16Rgain) ||
            (pstIspCtx->stPreDngColorParam.stWbGain2.u16Ggain != stDngColorParam.stWbGain2.u16Ggain) ||
            (pstIspCtx->stPreDngColorParam.stWbGain2.u16Bgain != stDngColorParam.stWbGain2.u16Bgain)) {
            bChanged = GK_TRUE;
        }
    }
    /* save last CCM and WB gain */
    memcpy(&pstIspCtx->stPreDngCcm, &stDngCcm, sizeof(ISP_DNG_CCM_S));
    memcpy(&pstIspCtx->stPreDngColorParam, &stDngColorParam, sizeof(ISP_DNG_COLORPARAM_S));

    if (bChanged == GK_TRUE) {
        /* data format */
        au16AWbgain[0] = stDngColorParam.stWbGain1.u16Rgain;
        au16AWbgain[4] = stDngColorParam.stWbGain1.u16Ggain;
        au16AWbgain[8] = stDngColorParam.stWbGain1.u16Bgain;
        au16D50Wbgain[0] = stDngColorParam.stWbGain2.u16Rgain;
        au16D50Wbgain[4] = stDngColorParam.stWbGain2.u16Ggain;
        au16D50Wbgain[8] = stDngColorParam.stWbGain2.u16Bgain;
        ISP_CcmDataFormat(stDngCcm.au16LowCcm, adACcm);
        ISP_CcmDataFormat(stDngCcm.au16HighCcm, adD50Ccm);
        ISP_CcmDataFormat(au16AWbgain, adAWbgain);
        ISP_CcmDataFormat(au16D50Wbgain, adD50Wbgain);

        /* calculate ColorMatrix1 */
        MultiMatrix3x3(adACcm, adAWbgain, adAMultMatrix);
        MultiMatrix3x3(SRGB_to_XYZA, adAMultMatrix, adInvAColorMatrix);
        Invert3by3(adInvAColorMatrix, adAColorMatrix);

        /* calculate ColorMatrix2 */
        MultiMatrix3x3(adD50Ccm, adD50Wbgain, adD50MultMatrix);
        MultiMatrix3x3(SRGB_to_XYZD50, adD50MultMatrix, adInvD50ColorMatrix);
        Invert3by3(adInvD50ColorMatrix, adD50ColorMatrix);

        /* calculate ForwardMatrix1 */
        Invert3by3(XYZD50_to_sRGB, adInvAColorMatrix);
        MultiMatrix3x3(adInvAColorMatrix, adACcm, adAForwardMatrix);

        /* calculate ForwardMatrix2 */
        Invert3by3(XYZD50_to_sRGB, adInvD50ColorMatrix);
        MultiMatrix3x3(adInvD50ColorMatrix, adD50Ccm, adD50ForwardMatrix);
        for (i = 0; i < 9; i++) {
            pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix1[i].s32Numerator = (GK_S32)(adAColorMatrix[i] * 1000000);
            pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix1[i].s32Denominator = 1000000;

            pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix2[i].s32Numerator = (GK_S32)(adD50ColorMatrix[i] * 1000000);
            pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix2[i].s32Denominator = 1000000;

            pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix1[i].s32Numerator = (GK_S32)(adAForwardMatrix[i] * 1000000);
            pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix1[i].s32Denominator = 1000000;

            pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix2[i].s32Numerator = (GK_S32)(adD50ForwardMatrix[i] * 1000000);
            pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix2[i].s32Denominator = 1000000;

            pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration1[i].s32Numerator = 0;
            pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration1[i].s32Denominator = 1000000;

            pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration2[i].s32Numerator = 0;
            pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration2[i].s32Denominator = 1000000;
        }
        pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration1[0].s32Numerator = 1000000;
        pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration1[4].s32Numerator = 1000000;
        pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration1[8].s32Numerator = 1000000;

        pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration2[0].s32Numerator = 1000000;
        pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration2[4].s32Numerator = 1000000;
        pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration2[8].s32Numerator = 1000000;

        ISP_Dng_LightSource_Checker(stDngCcm.u16LowColorTemp, &pstIspCtx->stDngInfoCtrl.pstIspDng->u8CalibrationIlluminant1);
        ISP_Dng_LightSource_Checker(stDngCcm.u16HighColorTemp, &pstIspCtx->stDngInfoCtrl.pstIspDng->u8CalibrationIlluminant2);
    }

    return GK_SUCCESS;
}

GK_S32 ISP_UpdateDngImageDynamicInfo(VI_PIPE ViPipe)
{
    ISP_CTX_S *pstIspCtx = GK_NULL;
    ISP_CMOS_BLACK_LEVEL_S *pstSnsBlackLevel = GK_NULL;
    DNG_IMAGE_DYNAMIC_INFO_S stDngImageDynamicInfo;
    GK_U8 i;
    GK_S32 s32Ret;

    ISP_GET_CTX(ViPipe, pstIspCtx);

    ISP_SensorGetBlc(ViPipe, &pstSnsBlackLevel);

    if (pstSnsBlackLevel->bUpdate == GK_TRUE) {
        for (i = 0; i < ISP_BAYER_CHN_NUM; i++) {
            stDngImageDynamicInfo.au32BlackLevel[i] = pstSnsBlackLevel->au16BlackLevel[i];
        }
    } else {
        stDngImageDynamicInfo.au32BlackLevel[0] = ext_system_black_level_query_00_read(ViPipe);
        stDngImageDynamicInfo.au32BlackLevel[1] = ext_system_black_level_query_01_read(ViPipe);
        stDngImageDynamicInfo.au32BlackLevel[2] = ext_system_black_level_query_10_read(ViPipe);
        stDngImageDynamicInfo.au32BlackLevel[3] = ext_system_black_level_query_11_read(ViPipe);
    }

    stDngImageDynamicInfo.astAsShotNeutral[0].u32Denominator = MAX2(pstIspCtx->stLinkage.au32WhiteBalanceGain[0], 1);
    stDngImageDynamicInfo.astAsShotNeutral[0].u32Numerator = pstIspCtx->stLinkage.au32WhiteBalanceGain[1];
    stDngImageDynamicInfo.astAsShotNeutral[1].u32Denominator = MAX2(pstIspCtx->stLinkage.au32WhiteBalanceGain[1], 1);
    stDngImageDynamicInfo.astAsShotNeutral[1].u32Numerator = pstIspCtx->stLinkage.au32WhiteBalanceGain[1];
    stDngImageDynamicInfo.astAsShotNeutral[2].u32Denominator = MAX2(pstIspCtx->stLinkage.au32WhiteBalanceGain[3], 1);
    stDngImageDynamicInfo.astAsShotNeutral[2].u32Numerator = pstIspCtx->stLinkage.au32WhiteBalanceGain[1];

    stDngImageDynamicInfo.adNoiseProfile[0] = 2.0E-5;
    stDngImageDynamicInfo.adNoiseProfile[1] = 4.5E-7;
    stDngImageDynamicInfo.adNoiseProfile[2] = 2.0E-5;
    stDngImageDynamicInfo.adNoiseProfile[3] = 4.5E-7;
    stDngImageDynamicInfo.adNoiseProfile[4] = 2.0E-5;
    stDngImageDynamicInfo.adNoiseProfile[5] = 4.5E-7;

    ISP_DngColorParamUpdate(ViPipe);

    s32Ret = ioctl(g_as32IspFd[ViPipe], ISP_DNG_INFO_SET, &stDngImageDynamicInfo);
    if (s32Ret != GK_SUCCESS) {
        return s32Ret;
    }

    return GK_SUCCESS;
}
