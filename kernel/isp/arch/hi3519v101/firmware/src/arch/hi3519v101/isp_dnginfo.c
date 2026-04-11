#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include "mkp_isp.h"
#include "mpi_sys.h"
#include "isp_sensor.h"
#include "isp_main.h"
#include "isp_alg.h"
#include "isp_dnginfo.h"
#include "isp_ext_config.h"


extern HI_S32     g_as32IspFd[ISP_MAX_DEV_NUM];
HI_S32 ISP_DngInfoInit(ISP_DEV IspDev, ISP_DNG_INFO_CTRL_S* pstDngInfoCtrl)
{
    HI_S32 s32Ret;

    s32Ret = ioctl(g_as32IspFd[IspDev], ISP_DNG_INFO_BUF_INIT, &pstDngInfoCtrl->u32DngInfoPhyaddr);
    if (HI_SUCCESS != s32Ret)
    {
        pstDngInfoCtrl->bSupportDng = HI_FALSE;

        if (HI_ERR_ISP_NOT_SUPPORT == s32Ret)
        {
            s32Ret = HI_SUCCESS;
        }
        else
        {
            printf("init Dng bufs failed %x!\n", s32Ret);
        }
        return s32Ret;
    }

    pstDngInfoCtrl->pstIspDng = HI_MPI_SYS_Mmap(pstDngInfoCtrl->u32DngInfoPhyaddr, sizeof(DNG_IMAGE_STATIC_INFO_S));
    if (HI_NULL == pstDngInfoCtrl->pstIspDng)
    {
        printf("mmap Dng info buf failed!\n");
        s32Ret = ioctl(g_as32IspFd[IspDev], ISP_DNG_INFO_BUF_EXIT);
        if (HI_SUCCESS != s32Ret)
        {
            printf("exit dng info buf failed %x!\n", s32Ret);
            return s32Ret;
        }

        return HI_ERR_ISP_NOMEM;
    }
    pstDngInfoCtrl->bSupportDng = HI_TRUE;

    return HI_SUCCESS;
}

HI_S32 ISP_DngInfoExit(ISP_DEV IspDev, ISP_DNG_INFO_CTRL_S* pstDngInfoCtrl)
{
    HI_S32 s32Ret;

    if (HI_FALSE == pstDngInfoCtrl->bSupportDng)
    {
        return HI_SUCCESS;
    }

    if (HI_NULL != pstDngInfoCtrl->pstIspDng)
    {
        HI_MPI_SYS_Munmap(pstDngInfoCtrl->pstIspDng, sizeof(DNG_IMAGE_STATIC_INFO_S));
        pstDngInfoCtrl->pstIspDng = HI_NULL;
    }

    s32Ret = ioctl(g_as32IspFd[IspDev], ISP_DNG_INFO_BUF_EXIT);
    if (HI_SUCCESS != s32Ret)
    {
        printf("exit dng info buf failed %x!\n", s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_BOOL ISP_DngColorParamCheck(ISP_DNG_CCM_S *pstDngCcm, ISP_DNG_CCM_S *pstPreDngCcm)
{
    HI_BOOL bChanged = HI_FALSE;
    HI_U8 i;

    for (i=0; i<9; i++)
    {
        if (pstDngCcm->au16LowCcm[i] !=  pstPreDngCcm->au16LowCcm[i])
        {
            bChanged = HI_TRUE;
            return bChanged;
        }
    }

    for (i=0; i<9; i++)
    {
        if (pstDngCcm->au16HighCcm[i] !=  pstPreDngCcm->au16HighCcm[i])
        {
            bChanged = HI_TRUE;
            return bChanged;
        }
    }

    if (pstDngCcm->u16HighColorTemp != pstPreDngCcm->u16HighColorTemp)
    {
        bChanged = HI_TRUE;
        return bChanged;
    }

    if (pstDngCcm->u16LowColorTemp != pstPreDngCcm->u16LowColorTemp)
    {
        bChanged = HI_TRUE;
        return bChanged;
    }

    return bChanged;

}

void ISP_CcmDataFormat(HI_U16 *pu16CcmIn, HI_DOUBLE *pdCcmOut)
{
    HI_U8 i;
    HI_S16 tmp;
    for (i=0; i<9; i++)
    {
        tmp = (HI_S16)pu16CcmIn[i];
        pdCcmOut[i] = (HI_DOUBLE)tmp / 256;
    }
}

const double kNearZero = 1.0E-10;

//double sRGB_to_XYZD65[9] = {0.4124564,0.3575761,0.1804375,0.2126729,0.7151522,0.0721750,0.0193339,0.1191920,0.9503041};
double SRGB_to_XYZD50[9] = {0.4361, 0.3851, 0.1431, 0.2225, 0.7169, 0.0606, 0.0139, 0.0971, 0.7141};
double SRGB_to_XYZA[9] = {0.4969, 0.4388, 0.1630, 0.2225, 0.7169, 0.0606, 0.0060, 0.0419, 0.3080};

//double XYZD65_to_sRGB[9] = {3.2405, -1.5371, -0.4985, -0.9693, 1.8760, 0.0416, 0.0556, -0.2040,1.0572};
double XYZD50_to_sRGB[9] = {3.1340, -1.6169, -0.4907, -0.9784, 1.9159, 0.0334, 0.0720, -0.2290, 1.4049};
//double XYZA_to_sRGB[9] = {2.7503, -1.6168, -1.1377, -0.8589, 1.9161, 0.0777, 0.0633, -0.2291, 3.2581};

double Abs_double (double x)
{
	return (x < 0.0 ? -x : x);
}

void Invert3by3 (const double *MatrixA, double *InvMatrix)
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

	temp [0] = a11 * a22 - a21 * a12;
	temp [1] = a21 * a02 - a01 * a22;
	temp [2] = a01 * a12 - a11 * a02;
	temp [3] = a20 * a12 - a10 * a22;
	temp [4] = a00 * a22 - a20 * a02;
	temp [5] = a10 * a02 - a00 * a12;
	temp [6] = a10 * a21 - a20 * a11;
	temp [7] = a20 * a01 - a00 * a21;
	temp [8] = a00 * a11 - a10 * a01;

	det = (a00 * temp [0] +
				  a01 * temp [3] +
				  a02 * temp [6]);

	if (Abs_double (det) < kNearZero)
	{
		return;
	}

	for (i = 0; i < 3 * 3; i++)
	{
		InvMatrix[i] = temp [i] / det;
	}

	return;
}

void MultiMatrix3x3 (const double *MatrixA, const double *MatrixB, double *MutMatrix)
{
	int i, j, k;

    for (i=0; i<3; ++i)
    {
        for (j=0; j<3; ++j)
        {
            double temp = 0;

            for (k=0; k<3; ++k)
            {
                temp += MatrixA[i * 3 + k] * MatrixB[k * 3 + j];
            }
            MutMatrix[i * 3 + j] = temp;
        }
    }
}

void ISP_Dng_LightSource_Checker(HI_U16 u16ColorTmep, HI_U8 *pu8LightSource)
{
    /*Light source, actually this means white balance setting. '0' means unknown, '1' daylight, '2'
           fluorescent, '3' tungsten, '10' flash, '17' standard light A, '18' standard light B, '19' standard light
           C, '20' D55, '21' D65, '22' D75, '255' other*/
    if (u16ColorTmep >= (7500 - 500)) /*D75 7500*/
    {
        *pu8LightSource = 22;
    }
    else if ((u16ColorTmep < (6500 + 500)) && (u16ColorTmep >= (6500 - 500)))/*D65 6500*/
    {
        *pu8LightSource = 21;
    }
    else if ((u16ColorTmep < (5500 + 500)) && (u16ColorTmep >= (5500-250)))/*D55 5500*/
    {
        *pu8LightSource = 20;
    }
    else if ((u16ColorTmep < (5000 + 250)) && (u16ColorTmep >= (5000-100)))/*daylight 5000*/
    {
        *pu8LightSource = 23;
    }
    else if ((u16ColorTmep < (4800 + 100)) && (u16ColorTmep >= (4800-550))) /*B 4800*/
    {
        *pu8LightSource = 18;
    }
    else if ((u16ColorTmep < (4000 + 250)) && (u16ColorTmep >= (4000-800))) /*fluorescent 4000*/
    {
        *pu8LightSource = 2;
    }
    else if (u16ColorTmep < (2800 + 400)) /*A 2800*/
    {
        *pu8LightSource = 17;
    }
}

HI_VOID ISP_DngExtRead(ISP_DEV IspDev, ISP_DNG_COLORPARAM_S *pstDngColorParam)
{
    pstDngColorParam->stWbGain1.u16Rgain = hi_ext_system_dng_high_wb_gain_r_read(IspDev);
    pstDngColorParam->stWbGain1.u16Ggain = hi_ext_system_dng_high_wb_gain_g_read(IspDev);
    pstDngColorParam->stWbGain1.u16Bgain = hi_ext_system_dng_high_wb_gain_b_read(IspDev);

    pstDngColorParam->stWbGain2.u16Rgain = hi_ext_system_dng_low_wb_gain_r_read(IspDev);
    pstDngColorParam->stWbGain2.u16Ggain = hi_ext_system_dng_low_wb_gain_g_read(IspDev);
    pstDngColorParam->stWbGain2.u16Bgain = hi_ext_system_dng_low_wb_gain_b_read(IspDev);
}

HI_S32 ISP_DngColorParamUpdate(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    AWB_CCM_CONFIG_S stAwbCcmConfig = {0};
    ISP_DNG_CCM_S stDngCcm = {{0}};
    HI_DOUBLE adHiD50Ccm[9] = {0}, adHiACcm[9] = {0};
    HI_DOUBLE adHiD50Wbgain[9] = {0}, adHiAWbgain[9] = {0};
    HI_U16 au16HiD50Wbgain[9] = {0},au16HiAWbgain[9] = {0};
    HI_DOUBLE adAMultMatrix[9] = {0}, adD50MultMatrix[9] = {0};
    HI_DOUBLE adInvAColorMatrix[9] = {0}, adInvD50ColorMatrix[9] = {0};
    HI_DOUBLE adAColorMatrix[9] = {0},adD50ColorMatrix[9] = {0};
    HI_DOUBLE adAForwardMatrix[9] = {0},adD50ForwardMatrix[9] = {0};
    ISP_DNG_COLORPARAM_S stDngColorParam = {{0}};
    HI_U8 i = 0;
    ISP_DEV AWB_DEV = (IspDev ? 0:1);
    HI_BOOL bUseInfoCCM = HI_TRUE;

    HI_BOOL bChanged = HI_FALSE;

    ISP_GET_CTX(IspDev, pstIspCtx);

    ISP_DngExtRead(IspDev, &stDngColorParam);

    /*DNG color param have not set*/
    if (0 == stDngColorParam.stWbGain1.u16Ggain)
    {
        return HI_SUCCESS;
    }

    bUseInfoCCM = hi_ext_system_snap_use_info_ccm_enable_read(IspDev);

    memset(au16HiAWbgain, 0, sizeof(HI_U16)*9);
    memset(au16HiD50Wbgain, 0, sizeof(HI_U16)*9);

    /*get CCM from AWB libiray*/
    switch (pstIspCtx->stLinkage.enSnapType)
    {
        default:
        case SNAP_TYPE_NONE:
            ISP_AlgsCtrl(pstIspCtx->astAlgs, IspDev, AWB_CCM_CONFIG_GET, (HI_VOID *)&stAwbCcmConfig);
            break;
        case SNAP_TYPE_SYNC:
        case SNAP_TYPE_SYNC_PRO:
            if (ISP_SNAP_PICTURE == pstIspCtx->stLinkage.enSnapPipeMode)
            {
                ISP_AlgsCtrl(pstIspCtx->astAlgs, AWB_DEV, AWB_CCM_CONFIG_GET, (HI_VOID *)&stAwbCcmConfig);
            }
            else
            {
                ISP_AlgsCtrl(pstIspCtx->astAlgs, IspDev, AWB_CCM_CONFIG_GET, (HI_VOID *)&stAwbCcmConfig);
            }
            break;
        case SNAP_TYPE_NORMAL:
        case SNAP_TYPE_PRO:
            if ((ISP_SNAP_PICTURE == pstIspCtx->stLinkage.enSnapPipeMode) && (HI_TRUE == bUseInfoCCM))
            {
                ISP_AlgsCtrl(pstIspCtx->astAlgs, AWB_DEV, AWB_CCM_CONFIG_GET, (HI_VOID *)&stAwbCcmConfig);
            }
            else
            {
                ISP_AlgsCtrl(pstIspCtx->astAlgs, IspDev, AWB_CCM_CONFIG_GET, (HI_VOID *)&stAwbCcmConfig);
            }
            break;
    }

    memcpy(&stDngCcm.au16LowCcm, &stAwbCcmConfig.au16LowCCM, sizeof(HI_U16)*9);
    memcpy(&stDngCcm.au16HighCcm, &stAwbCcmConfig.au16HighCCM, sizeof(HI_U16)*9);
    stDngCcm.u16HighColorTemp = stAwbCcmConfig.u16HighColorTemp;
    stDngCcm.u16LowColorTemp = stAwbCcmConfig.u16LowColorTemp;

    /*if CCM or WB gain changed, recaculate color parameters*/
    bChanged = ISP_DngColorParamCheck(&stDngCcm, &pstIspCtx->stPreDngCcm);
    if (!bChanged)
    {
        if ((pstIspCtx->stPreDngColorParam.stWbGain1.u16Rgain != stDngColorParam.stWbGain1.u16Rgain)
            || (pstIspCtx->stPreDngColorParam.stWbGain1.u16Ggain != stDngColorParam.stWbGain1.u16Ggain)
            || (pstIspCtx->stPreDngColorParam.stWbGain1.u16Bgain != stDngColorParam.stWbGain1.u16Bgain)
            || (pstIspCtx->stPreDngColorParam.stWbGain2.u16Rgain != stDngColorParam.stWbGain2.u16Rgain)
            || (pstIspCtx->stPreDngColorParam.stWbGain2.u16Ggain != stDngColorParam.stWbGain2.u16Ggain)
            || (pstIspCtx->stPreDngColorParam.stWbGain2.u16Bgain != stDngColorParam.stWbGain2.u16Bgain))
        {
            bChanged = HI_TRUE;
        }
    }
    /*save last CCM and WB gain*/
    memcpy(&pstIspCtx->stPreDngCcm, &stDngCcm, sizeof(ISP_DNG_CCM_S));
    memcpy(&pstIspCtx->stPreDngColorParam, &stDngColorParam, sizeof(ISP_DNG_COLORPARAM_S));

    if (HI_TRUE == bChanged)
    {
        /*data format*/
        au16HiAWbgain[0] = stDngColorParam.stWbGain1.u16Rgain;
        au16HiAWbgain[4] = stDngColorParam.stWbGain1.u16Ggain;
        au16HiAWbgain[8] = stDngColorParam.stWbGain1.u16Bgain;
        au16HiD50Wbgain[0] = stDngColorParam.stWbGain2.u16Rgain;
        au16HiD50Wbgain[4] = stDngColorParam.stWbGain2.u16Ggain;
        au16HiD50Wbgain[8] = stDngColorParam.stWbGain2.u16Bgain;
        ISP_CcmDataFormat(stDngCcm.au16LowCcm, adHiACcm);
        ISP_CcmDataFormat(stDngCcm.au16HighCcm, adHiD50Ccm);
        ISP_CcmDataFormat(au16HiAWbgain, adHiAWbgain);
        ISP_CcmDataFormat(au16HiD50Wbgain, adHiD50Wbgain);

// test
//        adHiD50Wbgain[0] = 1 / 0.4545;
//        adHiD50Wbgain[8] = 1 / 0.7552;
//
//        adHiD50Ccm[0] = 479 / 256.0;
//        adHiD50Ccm[1] = -250 / 256.0;
//        adHiD50Ccm[2] = 27 / 256.0;
//        adHiD50Ccm[3] = -48 / 256.0;
//        adHiD50Ccm[4] = 346 / 256.0;
//        adHiD50Ccm[5] = -42 / 256.0;
//        adHiD50Ccm[6] = 6 / 256.0;
//        adHiD50Ccm[7] = -206 / 256.0;
//        adHiD50Ccm[8] = 456 / 256.0;

        /*calculate ColorMatrix1*/
        MultiMatrix3x3(adHiACcm, adHiAWbgain, adAMultMatrix);
        MultiMatrix3x3(SRGB_to_XYZA, adAMultMatrix, adInvAColorMatrix);
        Invert3by3(adInvAColorMatrix, adAColorMatrix);

        /*calculate ColorMatrix2*/
        MultiMatrix3x3(adHiD50Ccm, adHiD50Wbgain, adD50MultMatrix);
        MultiMatrix3x3(SRGB_to_XYZD50, adD50MultMatrix, adInvD50ColorMatrix);
        Invert3by3(adInvD50ColorMatrix, adD50ColorMatrix);

        /*calculate ForwardMatrix1*/
        Invert3by3(XYZD50_to_sRGB, adInvAColorMatrix);
        MultiMatrix3x3(adInvAColorMatrix, adHiACcm, adAForwardMatrix);

        /*calculate ForwardMatrix2*/
        Invert3by3(XYZD50_to_sRGB, adInvD50ColorMatrix);
        MultiMatrix3x3(adInvD50ColorMatrix, adHiD50Ccm, adD50ForwardMatrix);
#if 0
        printf("A: [0]%lf [1]%lf [2]%lf [3]%lf [4]%lf [5]%lf [6]%lf [7]%lf [8]%lf\n", adAColorMatrix[0], adAColorMatrix[1], adAColorMatrix[2],
            adAColorMatrix[3], adAColorMatrix[4], adAColorMatrix[5],
            adAColorMatrix[6], adAColorMatrix[7], adAColorMatrix[8]);
        printf("D50: [0]%lf [1]%lf [2]%lf [3]%lf [4]%lf [5]%lf [6]%lf [7]%lf [8]%lf\n", adD50ColorMatrix[0], adD50ColorMatrix[1], adD50ColorMatrix[2],
            adD50ColorMatrix[3], adD50ColorMatrix[4], adD50ColorMatrix[5],
            adD50ColorMatrix[6], adD50ColorMatrix[7], adD50ColorMatrix[8]);
        printf("fm A: [0]%lf [1]%lf [2]%lf [3]%lf [4]%lf [5]%lf [6]%lf [7]%lf [8]%lf\n", adAForwardMatrix[0], adAForwardMatrix[1], adAForwardMatrix[2],
            adAForwardMatrix[3], adAForwardMatrix[4], adAForwardMatrix[5],
            adAForwardMatrix[6], adAForwardMatrix[7], adAForwardMatrix[8]);
        printf("fm D50: [0]%lf [1]%lf [2]%lf [3]%lf [4]%lf [5]%lf [6]%lf [7]%lf [8]%lf\n", adD50ForwardMatrix[0], adD50ForwardMatrix[1], adD50ForwardMatrix[2],
            adD50ForwardMatrix[3], adD50ForwardMatrix[4], adD50ForwardMatrix[5],
            adD50ForwardMatrix[6], adD50ForwardMatrix[7], adD50ForwardMatrix[8]);
#endif
        for (i=0; i<9; i++)
        {
            pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix1[i].s32Numerator = (HI_S32)(adAColorMatrix[i]*1000000);
            pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix1[i].s32Denominator = 1000000;

            pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix2[i].s32Numerator = (HI_S32)(adD50ColorMatrix[i]*1000000);
            pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix2[i].s32Denominator = 1000000;

            pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix1[i].s32Numerator = (HI_S32)(adAForwardMatrix[i]*1000000);
            pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix1[i].s32Denominator = 1000000;

            pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix2[i].s32Numerator = (HI_S32)(adD50ForwardMatrix[i]*1000000);
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
#if 0
        printf("astColorMatrix1 [0]%x %x [1]%x %x [2]%x %x [3]%x %x [4]%x %x [5]%x %x [6]%x %x [7]%x %x [8]%x %x \n",
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix1[0].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix1[0].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix1[1].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix1[1].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix1[2].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix1[2].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix1[3].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix1[3].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix1[4].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix1[4].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix1[5].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix1[5].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix1[6].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix1[6].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix1[7].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix1[7].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix1[8].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix1[8].s32Denominator);
        printf("astColorMatrix2 [0]%x %x [1]%x %x [2]%x %x [3]%x %x [4]%x %x [5]%x %x [6]%x %x [7]%x %x [8]%x %x \n",
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix2[0].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix2[0].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix2[1].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix2[1].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix2[2].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix2[2].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix2[3].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix2[3].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix2[4].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix2[4].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix2[5].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix2[5].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix2[6].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix2[6].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix2[7].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix2[7].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix2[8].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astColorMatrix2[8].s32Denominator);
        printf("astForwadMatrix1 [0]%x %x [1]%x %x [2]%x %x [3]%x %x [4]%x %x [5]%x %x [6]%x %x [7]%x %x [8]%x %x \n",
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix1[0].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix1[0].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix1[1].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix1[1].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix1[2].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix1[2].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix1[3].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix1[3].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix1[4].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix1[4].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix1[5].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix1[5].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix1[6].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix1[6].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix1[7].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix1[7].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix1[8].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix1[8].s32Denominator);
        printf("astForwadMatrix2 [0]%x %x [1]%x %x [2]%x %x [3]%x %x [4]%x %x [5]%x %x [6]%x %x [7]%x %x [8]%x %x \n",
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix2[0].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix2[0].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix2[1].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix2[1].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix2[2].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix2[2].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix2[3].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix2[3].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix2[4].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix2[4].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix2[5].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix2[5].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix2[6].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix2[6].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix2[7].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix2[7].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix2[8].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astForwadMatrix2[8].s32Denominator);
        printf("astCameraCalibration1 [0]%x %x [1]%x %x [2]%x %x [3]%x %x [4]%x %x [5]%x %x [6]%x %x [7]%x %x [8]%x %x \n",
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration1[0].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration1[0].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration1[1].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration1[1].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration1[2].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration1[2].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration1[3].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration1[3].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration1[4].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration1[4].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration1[5].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration1[5].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration1[6].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration1[6].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration1[7].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration1[7].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration1[8].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration1[8].s32Denominator);
        printf("astCameraCalibration2 [0]%x %x [1]%x %x [2]%x %x [3]%x %x [4]%x %x [5]%x %x [6]%x %x [7]%x %x [8]%x %x \n",
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration2[0].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration2[0].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration2[1].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration2[1].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration2[2].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration2[2].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration2[3].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration2[3].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration2[4].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration2[4].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration2[5].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration2[5].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration2[6].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration2[6].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration2[7].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration2[7].s32Denominator,
                                                 pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration2[8].s32Numerator, pstIspCtx->stDngInfoCtrl.pstIspDng->astCameraCalibration2[8].s32Denominator);
        printf("u8CalibrationIlluminant1 %x\n", pstIspCtx->stDngInfoCtrl.pstIspDng->u8CalibrationIlluminant1);
        printf("u8CalibrationIlluminant2 %x\n", pstIspCtx->stDngInfoCtrl.pstIspDng->u8CalibrationIlluminant2);
#endif
    }

    return HI_SUCCESS;
}

HI_S32 ISP_UpdateDngImageDynamicInfo(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_CMOS_BLACK_LEVEL_S *pstSnsBlackLevel = HI_NULL;
    DNG_IMAGE_DYNAMIC_INFO_S stDngImageDynamicInfo;
    HI_U8 i;
    HI_S32 s32Ret;

    ISP_GET_CTX(IspDev, pstIspCtx);

    ISP_SensorGetBlc(IspDev, &pstSnsBlackLevel);

    if (HI_TRUE == pstSnsBlackLevel->bUpdate)
    {
        for (i=0; i<ISP_BAYER_CHN_NUM; i++)
        {
            stDngImageDynamicInfo.au32BlackLevel[i] = pstSnsBlackLevel->au16BlackLevel[i];
        }
    }
    else
    {
        stDngImageDynamicInfo.au32BlackLevel[0] = hi_ext_system_black_level_00_read(IspDev);
        stDngImageDynamicInfo.au32BlackLevel[1] = hi_ext_system_black_level_01_read(IspDev);
        stDngImageDynamicInfo.au32BlackLevel[2] = hi_ext_system_black_level_10_read(IspDev);
        stDngImageDynamicInfo.au32BlackLevel[3] = hi_ext_system_black_level_11_read(IspDev);
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

    ISP_DngColorParamUpdate(IspDev);

    s32Ret = ioctl(g_as32IspFd[IspDev], ISP_DNG_INFO_SET, &stDngImageDynamicInfo);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }
    #if 0
    printf("au32BlackLevel %d %d %d %d\n", stDngImageDynamicInfo.au32BlackLevel[0], stDngImageDynamicInfo.au32BlackLevel[1], stDngImageDynamicInfo.au32BlackLevel[2], stDngImageDynamicInfo.au32BlackLevel[3]);
    printf("astAsShotNeutral [0]num %d den %d [1]num %d den %d [2]num %d den %d\n", stDngImageDynamicInfo.astAsShotNeutral[0].u32Numerator, stDngImageDynamicInfo.astAsShotNeutral[0].u32Denominator,
                                             stDngImageDynamicInfo.astAsShotNeutral[1].u32Numerator, stDngImageDynamicInfo.astAsShotNeutral[1].u32Denominator,
                                             stDngImageDynamicInfo.astAsShotNeutral[2].u32Numerator, stDngImageDynamicInfo.astAsShotNeutral[2].u32Denominator);
    printf("adNoiseProfile %e %e %e %e %e %e\n", stDngImageDynamicInfo.adNoiseProfile[0], stDngImageDynamicInfo.adNoiseProfile[1], stDngImageDynamicInfo.adNoiseProfile[2],
                                             stDngImageDynamicInfo.adNoiseProfile[3], stDngImageDynamicInfo.adNoiseProfile[4], stDngImageDynamicInfo.adNoiseProfile[5]);
    #endif

    return HI_SUCCESS;
}

