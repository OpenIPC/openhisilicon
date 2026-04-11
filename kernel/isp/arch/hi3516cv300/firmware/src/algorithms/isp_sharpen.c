/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_sharpen.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/01/16
  Description   :
  History       :
  1.Date        : 2014/01/16
    Author      : q00214668
    Modification: Created file

******************************************************************************/
#include "isp_config.h"
#include "isp_sharpen_mem_config.h"
#include "hi_isp_debug.h"
#include "isp_ext_config.h"
#include "isp_math_utils.h"

#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_proc.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define         SHARPEN_EN                       (1)
#define         SHARPEN_PIXSEL               (0)
#define     SHARPEN_EN_JAG_CTRL      (1  )
#define     SHARPEN_JAG_VAR_THD1         (400)
#define     SHARPEN_JAG_VAR_THD2         (450)
#define     SHARPEN_JAG_HF_AMT       (50 )
#define         SHT_MAX_SHFT                 (0)
#define         SHT_MIN_SHFT                 (0)
#define     SHARPEN_OSHTVARTH0       (125)
#define     SHARPEN_OSHTVARTH1       (300)
#define     SHARPEN_USHTVARTH0       (125)
#define     SHARPEN_USHTVARTH1       (300)
#define     SHARPEN_MF_LIMIT             (0)
#define	  SHARPEN_DIR_RLY_SHFT	(4)
#define     SHARPEN_EDGESM_DIFFMUL   (2)
#define         SHARPEN_SKIN_U_MAX       (130)
#define         SHARPEN_SKIN_U_MIN       (100)
#define         SHARPEN_SKIN_V_MAX       (166)
#define         SHARPEN_SKIN_V_MIN       (130)
#define         SHARPEN_RCBMIN           (0)
#define     SHARPEN_RCBMAX           (255)
#define     SHARPEN_RCRMIN           (172)
#define     SHARPEN_RCRMAX           (255)
#define     SHARPEN_RCRTHD           (182)
#define     SHARPEN_BCBMIN           (145)
#define     SHARPEN_BCBMAX           (255)
#define     SHARPEN_BCRMIN           (0)
#define     SHARPEN_BCRMAX           (128)
#define     SHARPEN_BCBTHD           (155)
#define     SHARPEN_GCBMIN           (60)
#define     SHARPEN_GCBMAX           (120)
#define     SHARPEN_GCRMIN           (90)
#define     SHARPEN_GCRMAX           (130)
#define     SHARPEN_ENLOWBANDESM     (0)

typedef struct hiISP_SHARPEN_S
{
    /* Sharpening Yuv */
    //tmp registers
    HI_U8 u8ManualSharpenYuvEnabled;
    HI_U8 u8minNoiseThd;
    HI_U8 u8ShtMaxVarGain;
    HI_U8 u8GGain;
    HI_U8 u8ShadNoiseSupSt;
    HI_U16 u16SharpThdDelta;
    HI_U16 u16EdgeThdDelta;
    //HI_U8 u8DirRlyShft;
    HI_U16 u16shadVar2;
    HI_U8 u8pixSel;
    //HI_U8 u8DiffThd;
    HI_U8 u8ShtDiffMin;
    HI_U8 u8ShtDiffMax;
    //MPI
    HI_U16 u16SharpenUd;
    HI_U8 u8SharpenD;
    HI_U8 u8TextureThd;
    HI_U8 u8SharpenEdge;
    HI_U8 u8EdgeThd;
    HI_U8 u8OverShoot;
    HI_U8 u8UnderShoot;
    HI_U8 u8shootSupSt;
    HI_U8 u8DetailCtrl;
	HI_U8 u8RGain;           //u8RGain   [0, 31]
    HI_U8 u8BGain;           //u8BGain   [0, 31]
    HI_U8 u8SkinGain;        //u8SkinGain   [0,255]        
    HI_U8 u8EdgeFiltStr;               /* RW; range: [0, 127]; Format:7.0;Edge noise suppression strength*/
    HI_U8 u8JagCtrl;                     /* RW; range: [0, 255]; Format:8.0;Edge Jag Control*/
    HI_U8 u8NoiseLumaCtrl;          /* RW; range: [0, 31]; Format:5.0;Noise suppression based on the LumaWgt*/
    HI_U8 u8LumaWgt[ISP_YUV_SHPLUMA_NUM];                   //sharpen weight based on loacal luma [0, 255]
    HI_U16 au16SharpenUd[ISP_AUTO_ISO_STRENGTH_NUM];        //Undirectional sharpen strength for undirectional detail enhancement. [0, 1023]    //u16UDHfGain
    HI_U8 au8SharpenD[ISP_AUTO_ISO_STRENGTH_NUM];           //Directional sharpen strength for directional detail and edge enhancement.  [0, 255]   //u16EdgeAmt
    HI_U8 au8TextureThd[ISP_AUTO_ISO_STRENGTH_NUM];             //Sharpen threshold for SharpenUd and SharpenD, noise is deemed as below the TextureThd without sharpness. [0, 255]  //EdgeThd2
    HI_U8 au8SharpenEdge[ISP_AUTO_ISO_STRENGTH_NUM];        //Sharpen strength for strong edge enhancement. [0, 255]        //u16SharpAmt
    HI_U8 au8EdgeThd[ISP_AUTO_ISO_STRENGTH_NUM];            //Sharpen threshold to distinguish the strong edge, only the edge bigger than the EdgeThd can be sharpen by SharpenEdge.  [0, 255]  //u16SharpThd2
    HI_U8 au8OverShoot[ISP_AUTO_ISO_STRENGTH_NUM];              //u8OvershootAmt        [0, 255]
    HI_U8 au8UnderShoot[ISP_AUTO_ISO_STRENGTH_NUM];         //u8UndershootAmt       [0, 255]
    HI_U8 au8shootSupSt[ISP_AUTO_ISO_STRENGTH_NUM];         //overshoot and undershoot suppression strength, the amplitude and width of shoot will be decrease when shootSupSt increase.    [0, 255]    //u8OvShtVarMax/u8UnShtVarMax   /u8ShtSupBldr
    HI_U8 au8DetailCtrl[ISP_AUTO_ISO_STRENGTH_NUM];             //Different sharpen strength for detail and edge. When it is bigger than 128, detail sharpen strength will be stronger than edge.  //[0, 255]
    HI_U8 au8RGain[ISP_AUTO_ISO_STRENGTH_NUM];              //au8RGain       [0, 31]
    HI_U8 au8BGain[ISP_AUTO_ISO_STRENGTH_NUM];              //au8BGain       [0, 31]
    HI_U8 au8SkinGain[ISP_AUTO_ISO_STRENGTH_NUM];           //au8SkinGain   [0, 255]
    HI_U8 au8EdgeFiltStr[ISP_AUTO_ISO_STRENGTH_NUM];               /* RW; range: [0, 127]; Format:7.0;Edge noise suppression strength*/
    HI_U8 au8JagCtrl[ISP_AUTO_ISO_STRENGTH_NUM];                     /* RW; range: [0, 255]; Format:8.0;Edge Jag Control*/
    HI_U8 au8NoiseLumaCtrl[ISP_AUTO_ISO_STRENGTH_NUM];          /* RW; range: [0, 31]; Format:5.0;Noise suppression based on the LumaWgt*/
} ISP_SHARPEN_S;

ISP_SHARPEN_S g_astSharpenCtx[ISP_MAX_DEV_NUM] = {{0}};
#define SHARPEN_GET_CTX(dev, pstCtx)   pstCtx = &g_astSharpenCtx[dev]

static HI_VOID SharpenExtRegsInitialize(ISP_DEV IspDev)
{
    HI_U8 i;
    ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;

    ISP_SensorGetDefault(IspDev, &pstSnsDft);
    //auto ExtRegs initial
    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)
    {
        hi_ext_system_sharpen_SharpenUd_write(i, pstSnsDft->stYuvSharpen.au16SharpenUd[i]);
        hi_ext_system_sharpen_SharpenD_write(i, pstSnsDft->stYuvSharpen.au8SharpenD[i]);
        hi_ext_system_sharpen_TextureThd_write(i, pstSnsDft->stYuvSharpen.au8TextureThd[i]);
        hi_ext_system_sharpen_SharpenEdge_write(i, pstSnsDft->stYuvSharpen.au8SharpenEdge[i]);
        hi_ext_system_sharpen_EdgeThd_write(i, pstSnsDft->stYuvSharpen.au8EdgeThd[i]);
        hi_ext_system_sharpen_overshootAmt_write(i, pstSnsDft->stYuvSharpen.au8OverShoot[i]);
        hi_ext_system_sharpen_undershootAmt_write(i, pstSnsDft->stYuvSharpen.au8UnderShoot[i]);
        hi_ext_system_sharpen_shootSupSt_write(i, pstSnsDft->stYuvSharpen.au8shootSupSt[i]);
        hi_ext_system_sharpen_DetailCtrl_write(i, pstSnsDft->stYuvSharpen.au8DetailCtrl[i]);
		hi_ext_system_sharpen_RGain_write(i, pstSnsDft->stYuvSharpen.au8RGain[i]);
		hi_ext_system_sharpen_BGain_write(i, pstSnsDft->stYuvSharpen.au8BGain[i]);
		hi_ext_system_sharpen_SkinGain_write(i, pstSnsDft->stYuvSharpen.au8SkinGain[i]);
		hi_ext_system_sharpen_EdgeFiltStr_write(i, pstSnsDft->stYuvSharpen.au8EdgeFiltStr[i]);
		hi_ext_system_sharpen_JagCtrl_write(i, pstSnsDft->stYuvSharpen.au8JagCtrl[i]);
		hi_ext_system_sharpen_NoiseLumaCtrl_write(i, pstSnsDft->stYuvSharpen.au8NoiseLumaCtrl[i]);
    }
    //manual ExtRegs initial
    hi_ext_system_sharpen_manu_mode_write(HI_EXT_SYSTEM_SHARPEN_MANU_MODE_DEFAULT);
    hi_ext_system_manual_sharpen_SharpenUd_write(HI_EXT_SYSTEM_MANUAL_SHARPEN_SHARPENUD_DEFAULT);
    hi_ext_system_manual_sharpen_SharpenD_write(HI_EXT_SYSTEM_MANUAL_SHARPEN_SHARPEND_DEFAULT);
    hi_ext_system_manual_sharpen_TextureThd_write(HI_EXT_SYSTEM_MANUAL_SHARPEN_TEXTURETHD_DEFAULT);
    hi_ext_system_manual_sharpen_SharpenEdge_write(HI_EXT_SYSTEM_MANUAL_SHARPEN_SHARPENEDGE_DEFAULT);
    hi_ext_system_manual_sharpen_EdgeThd_write(HI_EXT_SYSTEM_MANUAL_SHARPEN_EDGETHD_DEFAULT);
    hi_ext_system_manual_sharpen_overshootAmt_write(HI_EXT_SYSTEM_MANUAL_SHARPEN_OVERSHOOT_DEFAULT);
    hi_ext_system_manual_sharpen_undershootAmt_write(HI_EXT_SYSTEM_MANUAL_SHARPEN_UNDERSHOOT_DEFAULT);
    hi_ext_system_manual_sharpen_DetailCtrl_write(HI_EXT_SYSTEM_MANUAL_SHARPEN_DETAILCTRL_DEFAULT);
	hi_ext_system_manual_sharpen_shootSupSt_write(HI_EXT_SYSTEM_MANUAL_SHARPEN_SHOOTSUPST_DEFAULT);
    hi_ext_system_manual_sharpen_RGain_write(HI_EXT_SYSTEM_MANUAL_SHARPEN_RGAIN_DEFAULT);
	hi_ext_system_manual_sharpen_BGain_write(HI_EXT_SYSTEM_MANUAL_SHARPEN_BGAIN_DEFAULT);
	hi_ext_system_manual_sharpen_SkinGain_write(HI_EXT_SYSTEM_MANUAL_SHARPEN_SKINGAIN_DEFAULT);
	hi_ext_system_manual_sharpen_EdgeFiltStr_write(HI_EXT_SYSTEM_MANUAL_SHARPEN_EDGE_FILT_STR_DEFAULT);
	hi_ext_system_manual_sharpen_JagCtrl_write(HI_EXT_SYSTEM_MANUAL_SHARPEN_JAGCTRL_DEFAULT);
	hi_ext_system_manual_sharpen_NoiseLumaCtrl_write(HI_EXT_SYSTEM_MANUAL_SHARPEN_NOISE_LUMA_CTRL_DEFAULT);
	
    for (i = 0; i < ISP_YUV_SHPLUMA_NUM; i++)
    {
        hi_ext_system_manual_sharpen_LumaWgt_write(i, 0xFF);
    }

    hi_ext_system_actual_sharpen_SharpenUd_write(HI_EXT_SYSTEM_ACTUAL_SHARPEN_SHARPENUD_DEFAULT);
    hi_ext_system_actual_sharpen_SharpenD_write(HI_EXT_SYSTEM_ACTUAL_SHARPEN_SHARPEND_DEFAULT);
    hi_ext_system_actual_sharpen_TextureThd_write(HI_EXT_SYSTEM_ACTUAL_SHARPEN_TEXTURETHD_DEFAULT);
    hi_ext_system_actual_sharpen_SharpenEdge_write(HI_EXT_SYSTEM_ACTUAL_SHARPEN_SHARPENEDGE_DEFAULT);
    hi_ext_system_actual_sharpen_EdgeThd_write(HI_EXT_SYSTEM_ACTUAL_SHARPEN_EDGETHD_DEFAULT);
    hi_ext_system_actual_sharpen_overshootAmt_write(HI_EXT_SYSTEM_ACTUAL_SHARPEN_OVERSHOOT_DEFAULT);
    hi_ext_system_actual_sharpen_undershootAmt_write(HI_EXT_SYSTEM_ACTUAL_SHARPEN_UNDERSHOOT_DEFAULT);
    hi_ext_system_actual_sharpen_DetailCtrl_write(HI_EXT_SYSTEM_ACTUAL_SHARPEN_DETAILCTRL_DEFAULT);
    hi_ext_system_actual_sharpen_shootSupSt_write(HI_EXT_SYSTEM_ACTUAL_SHARPEN_SHOOTSUPST_DEFAULT);
	hi_ext_system_actual_sharpen_RGain_write(HI_EXT_SYSTEM_MANUAL_SHARPEN_RGAIN_DEFAULT);
	hi_ext_system_actual_sharpen_BGain_write(HI_EXT_SYSTEM_MANUAL_SHARPEN_BGAIN_DEFAULT);
	hi_ext_system_actual_sharpen_SkinGain_write(HI_EXT_SYSTEM_MANUAL_SHARPEN_SKINGAIN_DEFAULT);

    return;
}

static HI_VOID SharpenRegsInitialize(ISP_DEV IspDev)
{
    HI_U32 u32Width = 0, u32Height = 0;
    u32Width = hi_ext_sync_total_width_read();
    u32Height = hi_ext_sync_total_height_read();
    hi_isp_sharpen_width_write(IspDev, u32Width-1);
    hi_isp_sharpen_height_write(IspDev, u32Height-1);
    hi_isp_sharpen_cfg_enable_write(IspDev, SHARPEN_EN);
    return;
}


static HI_S32 SharpenReadExtregs(ISP_DEV IspDev)
{
    HI_U8 i;
    ISP_SHARPEN_S *pstSharpen = HI_NULL;

    SHARPEN_GET_CTX(IspDev, pstSharpen);
    pstSharpen->u8ManualSharpenYuvEnabled = hi_ext_system_sharpen_manu_mode_read();
    for (i = 0; i < ISP_YUV_SHPLUMA_NUM; i++)
    {
        pstSharpen->u8LumaWgt[i] = hi_ext_system_manual_sharpen_LumaWgt_read(i);
    }
    if (pstSharpen->u8ManualSharpenYuvEnabled)
    {
        pstSharpen->u16SharpenUd = hi_ext_system_manual_sharpen_SharpenUd_read();
        pstSharpen->u8SharpenD = hi_ext_system_manual_sharpen_SharpenD_read();
        pstSharpen->u8TextureThd = hi_ext_system_manual_sharpen_TextureThd_read();
        pstSharpen->u8SharpenEdge = hi_ext_system_manual_sharpen_SharpenEdge_read();
        pstSharpen->u8EdgeThd = hi_ext_system_manual_sharpen_EdgeThd_read();
        pstSharpen->u8OverShoot = hi_ext_system_manual_sharpen_overshootAmt_read();
        pstSharpen->u8UnderShoot = hi_ext_system_manual_sharpen_undershootAmt_read();
        pstSharpen->u8DetailCtrl = hi_ext_system_manual_sharpen_DetailCtrl_read();
        pstSharpen->u8shootSupSt = hi_ext_system_manual_sharpen_shootSupSt_read();
		pstSharpen->u8RGain = hi_ext_system_manual_sharpen_RGain_read();
		pstSharpen->u8BGain = hi_ext_system_manual_sharpen_BGain_read();
		pstSharpen->u8SkinGain = hi_ext_system_manual_sharpen_SkinGain_read();
		pstSharpen->u8EdgeFiltStr= hi_ext_system_manual_sharpen_EdgeFiltStr_read();
		pstSharpen->u8JagCtrl= hi_ext_system_manual_sharpen_JagCtrl_read();
		pstSharpen->u8NoiseLumaCtrl= hi_ext_system_manual_sharpen_NoiseLumaCtrl_read();
		
        //pstSharpen->u8minNoiseThd =1;
        //pstSharpen->u8ShtMaxVarGain   =32;
        //pstSharpen->u8RGain           =16;
        //pstSharpen->u8GGain           =6;
        //pstSharpen->u8BGain           =20;
        //pstSharpen->u8SkinGain        =150;
        //pstSharpen->u8ShadNoiseSupSt=20;
        //pstSharpen->u16SharpThdDelta=10;
        //pstSharpen->u16EdgeThdDelta   =10;
        //pstSharpen->u8DirRlyShft  =5;
        //pstSharpen->u16shadVar2       =40;
        //pstSharpen->u8pixSel      =0;
        //pstSharpen->u8DiffThd     =1;
        //pstSharpen->u8ShtDiffMin  =20;
        //pstSharpen->u8ShtDiffMax  =35;
    }
    else
    {
        for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)
        {
            pstSharpen->au16SharpenUd[i] = hi_ext_system_sharpen_SharpenUd_read(i);
            pstSharpen->au8SharpenD[i] = hi_ext_system_sharpen_SharpenD_read(i);
            pstSharpen->au8TextureThd[i] = hi_ext_system_sharpen_TextureThd_read(i);
            pstSharpen->au8SharpenEdge[i] = hi_ext_system_sharpen_SharpenEdge_read(i);
            pstSharpen->au8EdgeThd[i] = hi_ext_system_sharpen_EdgeThd_read(i);
            pstSharpen->au8OverShoot[i] = hi_ext_system_sharpen_overshootAmt_read(i);
            pstSharpen->au8UnderShoot[i] = hi_ext_system_sharpen_undershootAmt_read(i);
            pstSharpen->au8DetailCtrl[i] = hi_ext_system_sharpen_DetailCtrl_read(i);
            pstSharpen->au8shootSupSt[i] = hi_ext_system_sharpen_shootSupSt_read(i);
			pstSharpen->au8RGain[i] = hi_ext_system_sharpen_RGain_read(i);
			pstSharpen->au8BGain[i] = hi_ext_system_sharpen_BGain_read(i);
			pstSharpen->au8SkinGain[i] = hi_ext_system_sharpen_SkinGain_read(i);
			pstSharpen->au8EdgeFiltStr[i] = hi_ext_system_sharpen_EdgeFiltStr_read(i);
			pstSharpen->au8JagCtrl[i] = hi_ext_system_sharpen_JagCtrl_read(i);
			pstSharpen->au8NoiseLumaCtrl[i] = hi_ext_system_sharpen_NoiseLumaCtrl_read(i);
        }
    }


    return 0;
}

HI_S32 SharpenProcWrite(ISP_DEV IspDev, ISP_CTRL_PROC_WRITE_S *pstProc)
{
    ISP_CTRL_PROC_WRITE_S stProcTmp;

    if ((HI_NULL == pstProc->pcProcBuff)|| (0 == pstProc->u32BuffLen))
    {
        return HI_FAILURE;
    }

    stProcTmp.pcProcBuff = pstProc->pcProcBuff;
    stProcTmp.u32BuffLen = pstProc->u32BuffLen;

    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
    "-----SHARPEN INFO--------------------------------------------------------------\n");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
    "%16s"    "%16s"        "%16s"          "%16s"             "%16s"       "%16s"          "%16s\n",
    "Enable", "SharpenD", "TextureThd", "SharpenEdge", "EdgeThd",   "OverShoot",    "UnderShoot");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
    "%16u"   "%16u"         "%16u"        "%16u"          "%16u"             "%16u"     "%16u\n",
    (HI_U16)hi_isp_sharpen_cfg_enable_read(IspDev),
    (HI_U16)(hi_isp_edge_amt_read(IspDev)>>4),
    (HI_U16)(hi_isp_edge_thd2_read(IspDev)>>2),
    (HI_U16)(hi_isp_sharp_amt_read(IspDev)>>4),
    (HI_U16)(hi_isp_sharp_thd2_read(IspDev)>>2),
    (HI_U16)hi_isp_over_amt_read(IspDev),
    (HI_U16)hi_isp_under_amt_read(IspDev)
    );

    pstProc->u32WriteLen += 1;

    return HI_SUCCESS;
}

static __inline HI_S32 SharpenImageResWrite(ISP_DEV IspDev, ISP_CMOS_SENSOR_IMAGE_MODE_S *pstRes)
{
    // update sharpen width and height

    hi_isp_sharpen_width_write(IspDev, pstRes->u16Width - 1);
    hi_isp_sharpen_height_write(IspDev, pstRes->u16Height - 1);

    return HI_SUCCESS;
}


HI_S32 ISP_SharpenInit(ISP_DEV IspDev)
{

    SharpenRegsInitialize(IspDev);
    SharpenExtRegsInitialize(IspDev);

    return HI_SUCCESS;
}

static __inline HI_S32 LinearInter(HI_S32 s32ISO, HI_S32 s32x0, HI_S32 s32y0, HI_S32 s32x1, HI_S32 s32y1)
{
    HI_S32 res;

    if ( s32ISO <= s32x0 ) return s32y0;
    if ( s32ISO >= s32x1 ) return s32y1;

    res = (s32y1 - s32y0) * (s32ISO - s32x0) / (s32x1 - s32x0) + s32y0;
    return res;
}

static HI_S32 CalcMulCoef(HI_S32 x0, HI_S32 y0, HI_S32 x1, HI_S32 y1, HI_S32 shift)
{
    HI_S32 res;
    if ( x0 == x1 )
    {
        res = 0;
    }
    else
    {
        res = ( (y1 - y0) << shift ) / ( x1 - x0);
    }
    return res;
}

static HI_S32 ISP_Sharpen_GetDefaultRegCfg(ISP_DEV IspDev, HI_U32 s32ISO, HI_U32 expTime)
{
    HI_U32 i;
    HI_S32 index1, index2;
    HI_U8  u8WDRMode;


    const HI_S32 SharpenLutIso[ISP_AUTO_ISO_STRENGTH_NUM] ={100,200,400,800,1600,3200,6400,12800,25600,51200,102400,204800,409600,819200,1638400,3276800};
    //const HI_U16 SharpThdDelta[ISP_AUTO_ISO_STRENGTH_NUM]={10,15,20,25,30,35,40,50,50,50,50,50,50,50,50,50};
    //const HI_U16 EdgeThdDelta[ISP_AUTO_ISO_STRENGTH_NUM]={10,15,20,25,30,40,50,80,120,150,180,220,250,300,400,500};
    const HI_U16 SharpThdDelta[ISP_AUTO_ISO_STRENGTH_NUM]={5,8,8,10,10,10,10,10,10,10,10,10,10,10,15,15};
    const HI_U16 EdgeThdDelta[ISP_AUTO_ISO_STRENGTH_NUM]={5,8,8,10,10,10,10,10,10,15,15,15,20,20,20,25};
    const HI_U16 SharpThdDeltaWDR[ISP_AUTO_ISO_STRENGTH_NUM]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    const HI_U16 EdgeThdDeltaWDR[ISP_AUTO_ISO_STRENGTH_NUM]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    //const HI_U8  DirRlyShft[ISP_AUTO_ISO_STRENGTH_NUM]={5,5,5,5,4,4,4,4,4,4,4,4,4,4,4,4};
    const HI_U16 shadVar2[ISP_AUTO_ISO_STRENGTH_NUM]={40,50,70,90,100,110,120,130,140,150,160,160,160,160,160,160};
    const HI_U8 ShtDiffMin[ISP_AUTO_ISO_STRENGTH_NUM]={20,22,23,25,28,30,32,36,37,38,39,40,40,40,40,40};
    const HI_U8 ShtDiffMax[ISP_AUTO_ISO_STRENGTH_NUM]={35,37,38,40,40,43,43,46,47,48,49,50,50,50,50,50};
    const HI_U8 minNoiseThd[ISP_AUTO_ISO_STRENGTH_NUM]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    const HI_U8 pixSel[ISP_AUTO_ISO_STRENGTH_NUM]={0,0,0,0,0,0,0,0,0,0,16,31,31,31,31,31};
    const HI_U8 ShtMaxVarGain[ISP_AUTO_ISO_STRENGTH_NUM]={32,32,32,32,30,30,30,30,30,30,30,28,26,25,24,22};
    //const HI_U8 RGain[ISP_AUTO_ISO_STRENGTH_NUM]={16,16,16,16,24,31,31,31,31,31,31,31,31,31,31,31};
    //const HI_U8 BGain[ISP_AUTO_ISO_STRENGTH_NUM]={20,20,20,20,26,31,31,31,31,31,31,31,31,31,31,31};
    const HI_U8 GGain[ISP_AUTO_ISO_STRENGTH_NUM]={6,5,4,4,4,4,4,4,4,4,4,4,4,4,4,4};
	//const HI_U8 SkinGain[ISP_AUTO_ISO_STRENGTH_NUM]={128,128,128,128,128,120,110,100,90,80,70,60,50,50,50,50};
    const HI_U8 ShadNoiseSupSt[ISP_AUTO_ISO_STRENGTH_NUM]={20,20,20,16,10,8,4,0,0,0,0,0,0,0,0,0};
    //const HI_U8 DiffThd[ISP_AUTO_ISO_STRENGTH_NUM]={1,1,1,2,2,3,3,4,4,5,6,7,8,9,10,11};

    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_SHARPEN_S *pstSharpenPara = HI_NULL;
    SHARPEN_GET_CTX(IspDev, pstSharpenPara);
    ISP_GET_CTX(IspDev, pstIspCtx);

    u8WDRMode = pstIspCtx->u8SnsWDRMode;

    //if (u8WDRMode)
    //  s32ISO = (u16WdrExpRatio * s32ISO)>>6;  //change ISO in WDR mode

    index1 = ISP_AUTO_ISO_STRENGTH_NUM - 1;

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)
    {
    if (s32ISO <= SharpenLutIso[i])
    {
        index1 = i;
        break;
    }
    }

    index2 = MAX2(index1 - 1, 0);


    if (!u8WDRMode)
    {
    pstSharpenPara->u16SharpThdDelta = LinearInter(s32ISO,
                                    SharpenLutIso[index2], SharpThdDelta[index2],
                                    SharpenLutIso[index1], SharpThdDelta[index1]);


    pstSharpenPara->u16EdgeThdDelta = LinearInter(s32ISO,
                                    SharpenLutIso[index2], EdgeThdDelta[index2],
                                    SharpenLutIso[index1], EdgeThdDelta[index1]);
    }
    else
    {
    pstSharpenPara->u16SharpThdDelta = LinearInter(s32ISO,
                                    SharpenLutIso[index2], SharpThdDeltaWDR[index2],
                                    SharpenLutIso[index1], SharpThdDeltaWDR[index1]);


    pstSharpenPara->u16EdgeThdDelta = LinearInter(s32ISO,
                                    SharpenLutIso[index2], EdgeThdDeltaWDR[index2],
                                    SharpenLutIso[index1], EdgeThdDeltaWDR[index1]);
    }
   //pstSharpenPara->u8DirRlyShft = LinearInter(s32ISO,
   //                                SharpenLutIso[index2], DirRlyShft[index2],
   //                                SharpenLutIso[index1], DirRlyShft[index1]);
    pstSharpenPara->u16shadVar2 = LinearInter(s32ISO,
                                    SharpenLutIso[index2], shadVar2[index2],
                                    SharpenLutIso[index1], shadVar2[index1]);
    pstSharpenPara->u8pixSel = LinearInter(s32ISO,
                                SharpenLutIso[index2], pixSel[index2],
                                SharpenLutIso[index1], pixSel[index1]);
    pstSharpenPara->u8minNoiseThd = LinearInter(s32ISO,
                                SharpenLutIso[index2], minNoiseThd[index2],
                                SharpenLutIso[index1], minNoiseThd[index1]);
    pstSharpenPara->u8ShtMaxVarGain = LinearInter(s32ISO,
                                SharpenLutIso[index2], ShtMaxVarGain[index2],
                                SharpenLutIso[index1], ShtMaxVarGain[index1]);
    //pstSharpenPara->u8RGain = LinearInter(s32ISO,
    //                            SharpenLutIso[index2], RGain[index2],
    //                            SharpenLutIso[index1], RGain[index1]);
    pstSharpenPara->u8GGain = LinearInter(s32ISO,
                                SharpenLutIso[index2], GGain[index2],
                                SharpenLutIso[index1], GGain[index1]);
    //pstSharpenPara->u8BGain = LinearInter(s32ISO,
    //                            SharpenLutIso[index2], BGain[index2],
    //                           SharpenLutIso[index1], BGain[index1]);
    //pstSharpenPara->u8SkinGain = LinearInter(s32ISO,
    //                            SharpenLutIso[index2], SkinGain[index2],
    //                            SharpenLutIso[index1], SkinGain[index1]);
    pstSharpenPara->u8ShadNoiseSupSt = LinearInter(s32ISO,
                                SharpenLutIso[index2], ShadNoiseSupSt[index2],
                                SharpenLutIso[index1], ShadNoiseSupSt[index1]);

    //pstSharpenPara->u8DiffThd = LinearInter(s32ISO,
    //                            SharpenLutIso[index2], DiffThd[index2],
    //                            SharpenLutIso[index1], DiffThd[index1]);
    pstSharpenPara->u8ShtDiffMin = LinearInter(s32ISO,
                                SharpenLutIso[index2], ShtDiffMin[index2],
                                SharpenLutIso[index1], ShtDiffMin[index1]);
    pstSharpenPara->u8ShtDiffMax = LinearInter(s32ISO,
                                SharpenLutIso[index2], ShtDiffMax[index2],
                                SharpenLutIso[index1], ShtDiffMax[index1]);


    return HI_SUCCESS;

}


HI_S32 ISP_Sharpen_GetMpiRegCfg(ISP_DEV IspDev, HI_U32 s32ISO, HI_U32 expTime)
{
    HI_U32 i;
    HI_S32 index1, index2;

    const HI_S32 SharpenLutIso[ISP_AUTO_ISO_STRENGTH_NUM] ={100,200,400,800,1600,3200,6400,12800,25600,51200,102400,204800,409600,819200,1638400,3276800};


    ISP_SHARPEN_S *pstSharpenPara = HI_NULL;
    SHARPEN_GET_CTX(IspDev, pstSharpenPara);


    //u8WDRMode = pstIspCtx->u8SnsWDRMode;


    //if (u8WDRMode)
    //  s32ISO = (u16WdrExpRatio * s32ISO)>>6;  //change ISO in WDR mode

    index1 = ISP_AUTO_ISO_STRENGTH_NUM - 1;

    for (i = 0; i < ISP_AUTO_ISO_STRENGTH_NUM; i++)
    {
    if (s32ISO <= SharpenLutIso[i])
    {
        index1 = i;
        break;
    }
    }

    index2 = MAX2(index1 - 1, 0);

    /* Yuv Sharpen Auto */
    pstSharpenPara->u16SharpenUd = LinearInter(s32ISO,
                                        SharpenLutIso[index2], pstSharpenPara->au16SharpenUd[index2],
                                        SharpenLutIso[index1], pstSharpenPara->au16SharpenUd[index1]);
    pstSharpenPara->u8SharpenD = LinearInter(s32ISO,
                                        SharpenLutIso[index2], pstSharpenPara->au8SharpenD[index2],
                                        SharpenLutIso[index1], pstSharpenPara->au8SharpenD[index1]);
    pstSharpenPara->u8TextureThd = LinearInter(s32ISO,
                                        SharpenLutIso[index2], pstSharpenPara->au8TextureThd[index2],
                                        SharpenLutIso[index1], pstSharpenPara->au8TextureThd[index1]);
    pstSharpenPara->u8SharpenEdge = LinearInter(s32ISO,
                                        SharpenLutIso[index2], pstSharpenPara->au8SharpenEdge[index2],
                                        SharpenLutIso[index1], pstSharpenPara->au8SharpenEdge[index1]);
    pstSharpenPara->u8EdgeThd = LinearInter(s32ISO,
                                        SharpenLutIso[index2], pstSharpenPara->au8EdgeThd[index2],
                                        SharpenLutIso[index1], pstSharpenPara->au8EdgeThd[index1]);
    pstSharpenPara->u8OverShoot = LinearInter(s32ISO,
                                        SharpenLutIso[index2], pstSharpenPara->au8OverShoot[index2],
                                        SharpenLutIso[index1], pstSharpenPara->au8OverShoot[index1]);
    pstSharpenPara->u8UnderShoot = LinearInter(s32ISO,
                                        SharpenLutIso[index2], pstSharpenPara->au8UnderShoot[index2],
                                        SharpenLutIso[index1], pstSharpenPara->au8UnderShoot[index1]);
    pstSharpenPara->u8DetailCtrl = LinearInter(s32ISO,
                                        SharpenLutIso[index2], pstSharpenPara->au8DetailCtrl[index2],
                                        SharpenLutIso[index1], pstSharpenPara->au8DetailCtrl[index1]);
    pstSharpenPara->u8shootSupSt = LinearInter(s32ISO,
                                        SharpenLutIso[index2], pstSharpenPara->au8shootSupSt[index2],
                                        SharpenLutIso[index1], pstSharpenPara->au8shootSupSt[index1]);
	pstSharpenPara->u8RGain= LinearInter(s32ISO,
                                        SharpenLutIso[index2], pstSharpenPara->au8RGain[index2],
                                        SharpenLutIso[index1], pstSharpenPara->au8RGain[index1]);
	pstSharpenPara->u8BGain= LinearInter(s32ISO,
                                        SharpenLutIso[index2], pstSharpenPara->au8BGain[index2],
                                        SharpenLutIso[index1], pstSharpenPara->au8BGain[index1]);
	pstSharpenPara->u8SkinGain= LinearInter(s32ISO,
                                        SharpenLutIso[index2], pstSharpenPara->au8SkinGain[index2],
                                        SharpenLutIso[index1], pstSharpenPara->au8SkinGain[index1]);

	pstSharpenPara->u8EdgeFiltStr= LinearInter(s32ISO,
                                        SharpenLutIso[index2], pstSharpenPara->au8EdgeFiltStr[index2],
                                        SharpenLutIso[index1], pstSharpenPara->au8EdgeFiltStr[index1]);
	pstSharpenPara->u8JagCtrl= LinearInter(s32ISO,
                                        SharpenLutIso[index2], pstSharpenPara->au8JagCtrl[index2],
                                        SharpenLutIso[index1], pstSharpenPara->au8JagCtrl[index1]);
	pstSharpenPara->u8NoiseLumaCtrl= LinearInter(s32ISO,
                                        SharpenLutIso[index2], pstSharpenPara->au8NoiseLumaCtrl[index2],
                                        SharpenLutIso[index1], pstSharpenPara->au8NoiseLumaCtrl[index1]);


    return HI_SUCCESS;

}

// Check API
static void SharpenCheckReg( ISP_SHARPEN_REG_CFG_S *pstSharpenReg )
{
    HI_U8 i;

    pstSharpenReg->u16SharpAmt = CLIP_MAX(pstSharpenReg->u16SharpAmt, 4095);  // U4.8
    pstSharpenReg->u16EdgeAmt   = CLIP_MAX(pstSharpenReg->u16EdgeAmt, 4095);  // U4.8
    pstSharpenReg->u16UDHfGain   = CLIP_MAX(pstSharpenReg->u16UDHfGain,  1023);  // U6.4

    pstSharpenReg->u16SharpThd1 = CLIP_MAX(pstSharpenReg->u16SharpThd1,  1023);                // U10.0
    pstSharpenReg->u16SharpThd2 = CLIP_MAX(pstSharpenReg->u16SharpThd2, 1023);                // U10.0

    pstSharpenReg->u16EdgeThd1 = CLIP_MAX(pstSharpenReg->u16EdgeThd1,  1023);               // U10.0
    pstSharpenReg->u16EdgeThd2 = CLIP_MAX(pstSharpenReg->u16EdgeThd2,  1023);               // U10.0

    pstSharpenReg->u8PixSel      =   CLIP_MAX(pstSharpenReg->u8PixSel,  31);        // U5.0

    pstSharpenReg->s8ShtMaxShft    =   CLIP3(pstSharpenReg->s8ShtMaxShft, -128, 127); // S8.0
    pstSharpenReg->s8ShtMinShft    =   CLIP3(pstSharpenReg->s8ShtMinShft, -128, 127); // S8.0

    pstSharpenReg->bEnEdgSm      =   CLIP_MAX(pstSharpenReg->bEnEdgSm,  1);        // U1.0
    pstSharpenReg->u8DiffMul   =   CLIP_MAX(pstSharpenReg->u8DiffMul,  15);  // U10.0

    pstSharpenReg->u8EdgSmStr  =   CLIP3(pstSharpenReg->u8EdgSmStr, 1, 31); // U10.0

    pstSharpenReg->bEnJagCtrl      =   CLIP_MAX(pstSharpenReg->bEnJagCtrl,1);        // U1.0
        pstSharpenReg->u16VarJagThd1  =   CLIP_MAX(pstSharpenReg->u16VarJagThd1, 1023);  // U10.0
        pstSharpenReg->u16VarJagThd2 =   CLIP_MAX(pstSharpenReg->u16VarJagThd2,  1023); // U10.0
    pstSharpenReg->u16EdgeJagAmt    =   CLIP_MAX(pstSharpenReg->u16EdgeJagAmt, 4095); // U12.0


    pstSharpenReg->bEnLumaCtrl   =   CLIP_MAX(pstSharpenReg->bEnLumaCtrl, 1);     // U1.0
    pstSharpenReg->bEnLumaCtrlNoise   =   CLIP_MAX(pstSharpenReg->bEnLumaCtrlNoise, 1);     // U1.0
    pstSharpenReg->u8lumNoSt   =   CLIP_MAX(pstSharpenReg->u8lumNoSt, 7);     // U3.0


    pstSharpenReg->bEnShtCtrlVarMod   =   CLIP_MAX(pstSharpenReg->bEnShtCtrlVarMod,  1);// U1.0
    for( i = 0; i < 2; i++ )
    {
        pstSharpenReg->au16OShtVarTh[i]  =   CLIP_MAX(pstSharpenReg->au16OShtVarTh[i],  1023);  // U10.0
        pstSharpenReg->au16UShtVarTh[i]  =   CLIP_MAX(pstSharpenReg->au16UShtVarTh[i], 1023);  // U10.0
    }


    pstSharpenReg->u8dirRlyShft    =   CLIP_MAX(pstSharpenReg->u8dirRlyShft,  6); // U8.0

    pstSharpenReg->u8Limit   =   CLIP_MAX(pstSharpenReg->u8Limit, 31);  // U5.0

    pstSharpenReg->bEnSkinCtrl      =   CLIP_MAX(pstSharpenReg->bEnSkinCtrl, 1);        // U1.0
    pstSharpenReg->u8skin_cnt_th1     =   CLIP_MAX(pstSharpenReg->u8skin_cnt_th1,  15); // U8.0
    pstSharpenReg->u8skin_cnt_th2    =   CLIP_MAX(pstSharpenReg->u8skin_cnt_th2,  15); // U10.0
    pstSharpenReg->u16skin_var_th1     =   CLIP_MAX(pstSharpenReg->u16skin_var_th1,  1023); // U8.0
    pstSharpenReg->u16skin_var_th2     =   CLIP_MAX(pstSharpenReg->u16skin_var_th2,  1023); // U8.0



    pstSharpenReg->u8skin_len    =   CLIP3(pstSharpenReg->u8skin_len, 1, 12); // U4.0

    pstSharpenReg->bEnShtSup      =   CLIP_MAX(pstSharpenReg->bEnShtSup,  1);        // U1.0

    pstSharpenReg->u8UnShtWgtMin  =   CLIP_MAX(pstSharpenReg->u8UnShtWgtMin, 255); // U8.0


    pstSharpenReg->u8ShtSupBldr  =   CLIP_MAX(pstSharpenReg->u8ShtSupBldr,15); // U4.0

    pstSharpenReg->bEnShtMax      =   CLIP_MAX(pstSharpenReg->bEnShtMax,  1);        // U1.0



    pstSharpenReg->bEnShadSup      =   CLIP_MAX(pstSharpenReg->bEnShadSup,  1);        // U1.0
    pstSharpenReg->u16shadVar1   =   CLIP_MAX(pstSharpenReg->u16shadVar1,  1023);  // U9.0
    pstSharpenReg->u16shadVar2   =   CLIP_MAX(pstSharpenReg->u16shadVar2,  1023);  // U9.0
    pstSharpenReg->u8gradThd1   =   CLIP_MAX(pstSharpenReg->u8gradThd1,  20);  // U9.0
    pstSharpenReg->u8gradThd2   =   CLIP_MAX(pstSharpenReg->u8gradThd2,  20);  // U9.0
    pstSharpenReg->u8shadWgt1   =   CLIP_MAX(pstSharpenReg->u8shadWgt1,  31);  // U9.0
    pstSharpenReg->u8shadWgt2   =   CLIP_MAX(pstSharpenReg->u8shadWgt2,  31);  // U9.0

    pstSharpenReg->bEnRgbShp                    =   CLIP_MAX(pstSharpenReg->bEnRgbShp           ,   1);
    pstSharpenReg->u8RcBmin                         =   CLIP_MAX(pstSharpenReg->u8RcBmin        ,  143);
    pstSharpenReg->u8RcBmax                         =   CLIP_MAX(pstSharpenReg->u8RcBmax        ,  143);
    pstSharpenReg->u8RcRmin                         =   CLIP_MAX(pstSharpenReg->u8RcRmin        ,  143);
    pstSharpenReg->u8RcRmax                         =   CLIP_MAX(pstSharpenReg->u8RcRmax        ,  143);
    pstSharpenReg->u8RcRThd                         =   CLIP_MAX(pstSharpenReg->u8RcRThd        ,  143);
    pstSharpenReg->u8rWgt                           =   CLIP_MAX(pstSharpenReg->u8rWgt          ,  31);
    pstSharpenReg->s8rShpGainSft                    =   CLIP3(pstSharpenReg->s8rShpGainSft      , -32,  31);
    pstSharpenReg->u8BcBmin                         =   CLIP_MAX(pstSharpenReg->u8BcBmin        ,   143);
    pstSharpenReg->u8BcBmax                         =   CLIP_MAX(pstSharpenReg->u8BcBmax        ,   143);
    pstSharpenReg->u8BcRmin                         =   CLIP_MAX(pstSharpenReg->u8BcRmin        ,   143);
    pstSharpenReg->u8BcRmax                         =   CLIP_MAX(pstSharpenReg->u8BcRmax        ,   143);
    pstSharpenReg->u8BcBThd                         =   CLIP_MAX(pstSharpenReg->u8BcBThd        ,   143);
    pstSharpenReg->u8bWgt                           =   CLIP_MAX(pstSharpenReg->u8bWgt          ,   31);
    pstSharpenReg->s8bShpGainSft                    =   CLIP3(pstSharpenReg->s8bShpGainSft      , -32,  31);
    pstSharpenReg->u8GcBmin                         =   CLIP_MAX(pstSharpenReg->u8GcBmin        ,   143);
    pstSharpenReg->u8GcBmax                         =   CLIP_MAX(pstSharpenReg->u8GcBmax        ,   143);
    pstSharpenReg->u8GcRmin                         =   CLIP_MAX(pstSharpenReg->u8GcRmin        ,   143);
    pstSharpenReg->u8GcRmax                         =   CLIP_MAX(pstSharpenReg->u8GcRmax        ,   143);
    pstSharpenReg->u8gWgt                           =   CLIP_MAX(pstSharpenReg->u8gWgt          ,   15);
    pstSharpenReg->bEnLowBandEsm                    =   CLIP_MAX(pstSharpenReg->bEnLowBandEsm   ,   1);

}


HI_S32 ISP_SharpenRun(ISP_DEV IspDev, const HI_VOID *pStatInfo, HI_VOID *pRegCfg, HI_S32 s32Rsv)
{

    HI_U8  i;
    HI_U32 u32Iso = 0;
    HI_U32 u32ExpTime=0;
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_SHARPEN_S *pstSharpen = HI_NULL;
    ISP_REG_CFG_S *pstRegCfg = (ISP_REG_CFG_S *)pRegCfg;

    HI_U8   ShtVarMax[36]   ={1, 2, 1, 3, 2, 1, 4, 3, 2, 1, 5, 4, 3, 2, 1, 6, 5, 4, 3, 2, 1, 7, 6, 5, 4, 3, 2, 1,  8, 7,   6, 5, 4, 3, 2,  1  };
    HI_U8   ShtSupBldr[36]  ={4, 4, 5, 4, 5, 6, 4, 5, 6, 7, 4, 5, 6, 7, 8, 4, 5, 6, 7, 8, 9, 4, 5, 6, 7, 8, 9, 10, 4, 5,  6, 7, 8, 9, 10, 11};
    HI_U8   shootSupStIdx;

    HI_S32 bpCoef[3][3] = {{-16, -16, -16},
                            {-16,  28,  28},
                            {-16,  28,  32}};

    SHARPEN_GET_CTX(IspDev, pstSharpen);
    ISP_GET_CTX(IspDev, pstIspCtx);

    if (pstIspCtx->stLinkage.bDefectPixel)
    {
        return HI_SUCCESS;
    }

    /* calculate every two interrupts */
    if (0 != pstIspCtx->u32FrameCnt % 2)
    {
        return HI_SUCCESS;
    }

    SharpenReadExtregs(IspDev);

    /* sharpen strength linkage with the iso calculated by ae */
    u32Iso = pstIspCtx->stLinkage.u32Iso;
    u32ExpTime = pstIspCtx->stLinkage.u32Inttime;

    ISP_Sharpen_GetDefaultRegCfg(IspDev, u32Iso, u32ExpTime);

    if (pstSharpen->u8ManualSharpenYuvEnabled == 0) //auto mode
    {
        ISP_Sharpen_GetMpiRegCfg(IspDev, u32Iso, u32ExpTime);
    }

    {

        hi_ext_system_actual_sharpen_SharpenUd_write(pstSharpen->u16SharpenUd);
        hi_ext_system_actual_sharpen_SharpenD_write(pstSharpen->u8SharpenD);
        hi_ext_system_actual_sharpen_TextureThd_write(pstSharpen->u8TextureThd);
        hi_ext_system_actual_sharpen_SharpenEdge_write(pstSharpen->u8SharpenEdge);
        hi_ext_system_actual_sharpen_EdgeThd_write(pstSharpen->u8EdgeThd);
        hi_ext_system_actual_sharpen_overshootAmt_write(pstSharpen->u8OverShoot);
        hi_ext_system_actual_sharpen_undershootAmt_write(pstSharpen->u8UnderShoot);
        hi_ext_system_actual_sharpen_DetailCtrl_write(pstSharpen->u8DetailCtrl);
        hi_ext_system_actual_sharpen_shootSupSt_write(pstSharpen->u8shootSupSt);
		hi_ext_system_actual_sharpen_RGain_write(pstSharpen->u8RGain);
		hi_ext_system_actual_sharpen_BGain_write(pstSharpen->u8BGain);
		hi_ext_system_actual_sharpen_SkinGain_write(pstSharpen->u8SkinGain);
    }

    /* Fixed Logic Bpfilter coef */
    pstRegCfg->stSharpenRegCfg.G3[0][0] = bpCoef[0][0];
    pstRegCfg->stSharpenRegCfg.G3[0][1] = bpCoef[0][1];
    pstRegCfg->stSharpenRegCfg.G3[0][2] = bpCoef[0][2];
    pstRegCfg->stSharpenRegCfg.G3[1][0] = bpCoef[1][0];
    pstRegCfg->stSharpenRegCfg.G3[1][1] = bpCoef[1][1];
    pstRegCfg->stSharpenRegCfg.G3[1][2] = bpCoef[1][2];
    pstRegCfg->stSharpenRegCfg.G3[2][0] = bpCoef[2][0];
    pstRegCfg->stSharpenRegCfg.G3[2][1] = bpCoef[2][1];
    pstRegCfg->stSharpenRegCfg.G3[2][2] = bpCoef[2][2];

    /* Variable Logic coef */
    pstRegCfg->stSharpenRegCfg.u8PixSel = pstSharpen->u8pixSel;
    pstRegCfg->stSharpenRegCfg.u16SharpAmt = (pstSharpen->u8SharpenEdge << 4);
    pstRegCfg->stSharpenRegCfg.u16EdgeAmt = (pstSharpen->u8SharpenD << 4);
    pstRegCfg->stSharpenRegCfg.u16UDHfGain = pstSharpen->u16SharpenUd;
    if((pstRegCfg->stSharpenRegCfg.u16EdgeAmt == 0) && (pstRegCfg->stSharpenRegCfg.u16UDHfGain > 0))
        pstRegCfg->stSharpenRegCfg.u16EdgeAmt = 1;
    pstRegCfg->stSharpenRegCfg.u16SharpThd1 = (pstSharpen->u8EdgeThd << 2) + pstSharpen->u16SharpThdDelta;
    pstRegCfg->stSharpenRegCfg.u16SharpThd2 = (pstSharpen->u8EdgeThd << 2);
    pstRegCfg->stSharpenRegCfg.u16EdgeThd1 = (pstSharpen->u8TextureThd << 2) + pstSharpen->u16EdgeThdDelta;
    pstRegCfg->stSharpenRegCfg.u16EdgeThd2 = (pstSharpen->u8TextureThd << 2);

    pstRegCfg->stSharpenRegCfg.u8OvershootAmt = pstSharpen->u8OverShoot;
    pstRegCfg->stSharpenRegCfg.u8UndershootAmt = pstSharpen->u8UnderShoot;

    /* hi3519 new add */
    pstRegCfg->stSharpenRegCfg.u16VarJagThd1 = SHARPEN_JAG_VAR_THD1;
    pstRegCfg->stSharpenRegCfg.u16VarJagThd2 = SHARPEN_JAG_VAR_THD2;
       
    if(pstSharpen->u8JagCtrl == 0)
    {
		pstRegCfg->stSharpenRegCfg.bEnJagCtrl = 0;
		pstRegCfg->stSharpenRegCfg.u16EdgeJagAmt =  pstRegCfg->stSharpenRegCfg.u16EdgeAmt;
    }
    else
    {
		 pstRegCfg->stSharpenRegCfg.bEnJagCtrl = 1;
		 pstRegCfg->stSharpenRegCfg.u16EdgeJagAmt =  CLIP3(((255 -pstSharpen->u8JagCtrl )<<4), 0, pstRegCfg->stSharpenRegCfg.u16EdgeAmt);
    }
    pstRegCfg->stSharpenRegCfg.u8OshtJagAmt = pstRegCfg->stSharpenRegCfg.u8OvershootAmt;
    pstRegCfg->stSharpenRegCfg.u8UshtJagAmt = pstRegCfg->stSharpenRegCfg.u8UndershootAmt;
    //if (pstIspCtx->u8SnsWDRMode)
    //pstRegCfg->stSharpenRegCfg.u8OshtJagAmt = pstRegCfg->stSharpenRegCfg.u8OvershootAmt;

    pstRegCfg->stSharpenRegCfg.s8ShtMaxShft = SHT_MAX_SHFT;
    pstRegCfg->stSharpenRegCfg.s8ShtMinShft = SHT_MIN_SHFT;

    /* control sharpen based on luma */
    pstRegCfg->stSharpenRegCfg.bEnLumaCtrl = 0;
    for (i = 0; i < ISP_YUV_SHPLUMA_NUM; i++)
    {
        pstRegCfg->stSharpenRegCfg.u8lumaWgt[i] = pstSharpen->u8LumaWgt[i];
        if(pstRegCfg->stSharpenRegCfg.u8lumaWgt[i] < 255)
            pstRegCfg->stSharpenRegCfg.bEnLumaCtrl = 1;
    }

    /* control shoot based on var */
    pstRegCfg->stSharpenRegCfg.bEnShtCtrlVarMod = 1;
    if(pstSharpen->u8DetailCtrl == 128)
        pstRegCfg->stSharpenRegCfg.bEnShtCtrlVarMod = 0;
    pstRegCfg->stSharpenRegCfg.au16OShtVarTh[0]= SHARPEN_OSHTVARTH0;
    pstRegCfg->stSharpenRegCfg.au16OShtVarTh[1]= SHARPEN_OSHTVARTH1;
    pstRegCfg->stSharpenRegCfg.au16UShtVarTh[0]= SHARPEN_USHTVARTH0;
    pstRegCfg->stSharpenRegCfg.au16UShtVarTh[1]= SHARPEN_USHTVARTH1;
    pstRegCfg->stSharpenRegCfg.u8OShtVarAmt = CLIP3((pstRegCfg->stSharpenRegCfg.u8OvershootAmt + (pstSharpen->u8DetailCtrl << 1) - 255), 0, 255);
    pstRegCfg->stSharpenRegCfg.u8UShtVarAmt = CLIP3((pstRegCfg->stSharpenRegCfg.u8UndershootAmt + (pstSharpen->u8DetailCtrl << 1) - 255), 0, 255);

    pstRegCfg->stSharpenRegCfg.u8Limit = pstSharpen->u8NoiseLumaCtrl;
    pstRegCfg->stSharpenRegCfg.u8dirRlyShft = SHARPEN_DIR_RLY_SHFT;

    if(pstSharpen->u8EdgeFiltStr == 0)
    {
          pstRegCfg->stSharpenRegCfg.bEnEdgSm = 0;
          pstRegCfg->stSharpenRegCfg.u8DiffMul = SHARPEN_EDGESM_DIFFMUL;
          pstRegCfg->stSharpenRegCfg.u8DiffThd = 6;
          pstRegCfg->stSharpenRegCfg.u8EdgSmStr = 1;
    }
    else if(pstSharpen->u8EdgeFiltStr < 90)
    {
          pstRegCfg->stSharpenRegCfg.bEnEdgSm = 1;
          pstRegCfg->stSharpenRegCfg.u8DiffMul = SHARPEN_EDGESM_DIFFMUL;
          pstRegCfg->stSharpenRegCfg.u8DiffThd = 95 - pstSharpen->u8EdgeFiltStr;
          pstRegCfg->stSharpenRegCfg.u8EdgSmStr = 1;
    }
    else if(pstSharpen->u8EdgeFiltStr < 120)
    {
          pstRegCfg->stSharpenRegCfg.bEnEdgSm = 1;
          pstRegCfg->stSharpenRegCfg.u8DiffMul = SHARPEN_EDGESM_DIFFMUL;
          pstRegCfg->stSharpenRegCfg.u8DiffThd = 6;
          pstRegCfg->stSharpenRegCfg.u8EdgSmStr = pstSharpen->u8EdgeFiltStr - 88;
    }
    else if(pstSharpen->u8EdgeFiltStr < 127)
    {
          pstRegCfg->stSharpenRegCfg.bEnEdgSm = 1;
          pstRegCfg->stSharpenRegCfg.u8DiffMul = SHARPEN_EDGESM_DIFFMUL;
          pstRegCfg->stSharpenRegCfg.u8DiffThd = 126 - pstSharpen->u8EdgeFiltStr;
          pstRegCfg->stSharpenRegCfg.u8EdgSmStr = 31;
    }
    else if(pstSharpen->u8EdgeFiltStr == 127)
    {
          pstRegCfg->stSharpenRegCfg.bEnEdgSm = 1;
          pstRegCfg->stSharpenRegCfg.u8DiffMul = 1;
          pstRegCfg->stSharpenRegCfg.u8DiffThd = 0;
          pstRegCfg->stSharpenRegCfg.u8EdgSmStr = 31;
    }
   else
    {
          pstRegCfg->stSharpenRegCfg.bEnEdgSm = 1;
          pstRegCfg->stSharpenRegCfg.u8DiffMul = 1;
          pstRegCfg->stSharpenRegCfg.u8DiffThd = 0;
          pstRegCfg->stSharpenRegCfg.u8EdgSmStr = 31;
    }
     

    /* control noise thd based on luma */
    pstRegCfg->stSharpenRegCfg.bEnLumaCtrlNoise = 0;
    pstRegCfg->stSharpenRegCfg.u8lumNoSt = 0;

    //Skin detection
	pstRegCfg->stSharpenRegCfg.bEnSkinCtrl 		= 1;
    pstRegCfg->stSharpenRegCfg.u8skin_cnt_th1   = 5;
    pstRegCfg->stSharpenRegCfg.u8skin_cnt_th2   = 8;
    pstRegCfg->stSharpenRegCfg.u16skin_var_th1  = 0;
    pstRegCfg->stSharpenRegCfg.u16skin_var_th2  = 450;
    pstRegCfg->stSharpenRegCfg.u8skin_var_wgt1  = 255-pstSharpen->u8SkinGain ;
    pstRegCfg->stSharpenRegCfg.u8skin_var_wgt2  = 60;
    if(pstRegCfg->stSharpenRegCfg.u8skin_var_wgt1 <= pstRegCfg->stSharpenRegCfg.u8skin_var_wgt2)
        pstRegCfg->stSharpenRegCfg.bEnSkinCtrl      = 0;

    pstRegCfg->stSharpenRegCfg.u8skin_u_max     = SHARPEN_SKIN_U_MAX ;
    pstRegCfg->stSharpenRegCfg.u8skin_u_min         = SHARPEN_SKIN_U_MIN ;
    pstRegCfg->stSharpenRegCfg.u8skin_v_max     = SHARPEN_SKIN_V_MAX ;
    pstRegCfg->stSharpenRegCfg.u8skin_v_min         = SHARPEN_SKIN_V_MIN ;
    pstRegCfg->stSharpenRegCfg.u8skin_len       = 4;


    pstRegCfg->stSharpenRegCfg.bEnShtSup     = 1;
    pstRegCfg->stSharpenRegCfg.u8OvShtVarMin = 0;
    pstRegCfg->stSharpenRegCfg.u8OvShtWgtMin = 100;
    pstRegCfg->stSharpenRegCfg.u8UnShtVarMin = 0;
    pstRegCfg->stSharpenRegCfg.u8UnShtWgtMin = 120;

    pstRegCfg->stSharpenRegCfg.u8UnShtDiffMax = pstSharpen->u8ShtDiffMax;
    pstRegCfg->stSharpenRegCfg.u8UnShtDiffMin = pstSharpen->u8ShtDiffMin;
    pstRegCfg->stSharpenRegCfg.u8UnShtDiffWgtMin = 20;

    pstRegCfg->stSharpenRegCfg.u8OvShtDiffMax = pstSharpen->u8ShtDiffMax;
    pstRegCfg->stSharpenRegCfg.u8OvShtDiffMin = pstSharpen->u8ShtDiffMin;
    pstRegCfg->stSharpenRegCfg.u8OvShtDiffWgtMin = 10;
    pstRegCfg->stSharpenRegCfg.u8MaxNoiseThd = pstSharpen->u8minNoiseThd;
    pstRegCfg->stSharpenRegCfg.u8MinNoiseThd = pstSharpen->u8minNoiseThd;
    if(pstSharpen->u8shootSupSt == 0)
    {
        pstRegCfg->stSharpenRegCfg.bEnShtSup     = 0;
        pstRegCfg->stSharpenRegCfg.u8OvShtVarMax = 0;
        pstRegCfg->stSharpenRegCfg.u8UnShtVarMax = 0;
        pstRegCfg->stSharpenRegCfg.u8ShtSupBldr = 0;
    }
    else if(pstSharpen->u8shootSupSt < 36)
    {
        pstSharpen->u8shootSupSt = CLIP3((pstSharpen->u8shootSupSt-1) , 0, 255);
        pstRegCfg->stSharpenRegCfg.u8OvShtVarMax = ShtVarMax[pstSharpen->u8shootSupSt];
        pstRegCfg->stSharpenRegCfg.u8UnShtVarMax = pstRegCfg->stSharpenRegCfg.u8OvShtVarMax;
        pstRegCfg->stSharpenRegCfg.u8ShtSupBldr = ShtSupBldr[pstSharpen->u8shootSupSt];
    }
    else
    {
        shootSupStIdx = 28 + (pstSharpen->u8shootSupSt - 36)%8;
        pstRegCfg->stSharpenRegCfg.u8OvShtVarMax = ShtVarMax[shootSupStIdx] + 1 + (pstSharpen->u8shootSupSt - 36)/8;
        pstRegCfg->stSharpenRegCfg.u8UnShtVarMax = pstRegCfg->stSharpenRegCfg.u8OvShtVarMax;
        pstRegCfg->stSharpenRegCfg.u8ShtSupBldr = ShtSupBldr[shootSupStIdx];
    }

    pstRegCfg->stSharpenRegCfg.bEnShtMax     = 1;
    pstRegCfg->stSharpenRegCfg.u8uShtMaxChg = 200;
    pstRegCfg->stSharpenRegCfg.u8oShtMaxChg = 200;
    pstRegCfg->stSharpenRegCfg.u8uShtMaxVarGain = pstSharpen->u8ShtMaxVarGain;
    pstRegCfg->stSharpenRegCfg.u8oShtMaxVarGain = pstSharpen->u8ShtMaxVarGain;
    if(pstRegCfg->stSharpenRegCfg.bEnShtSup == 0)
        pstRegCfg->stSharpenRegCfg.bEnShtMax     = 0;

    pstRegCfg->stSharpenRegCfg.bEnShadSup = 1;
    pstRegCfg->stSharpenRegCfg.u16shadVar1  = 6;
    pstRegCfg->stSharpenRegCfg.u16shadVar2  = pstSharpen->u16shadVar2;;
    pstRegCfg->stSharpenRegCfg.u8gradThd1  = 10;
    pstRegCfg->stSharpenRegCfg.u8gradThd2  = 16;
    pstRegCfg->stSharpenRegCfg.u8shadWgt1  = 0;
    pstRegCfg->stSharpenRegCfg.u8shadWgt2  = pstSharpen->u8ShadNoiseSupSt;

    pstRegCfg->stSharpenRegCfg.bEnRgbShp                        = 1     ;
    pstRegCfg->stSharpenRegCfg.u8RcBmin                         = ((SHARPEN_RCBMIN) * 9) >> 4       ;
    pstRegCfg->stSharpenRegCfg.u8RcBmax                         = ((SHARPEN_RCBMAX) * 9) >> 4       ;
    pstRegCfg->stSharpenRegCfg.u8RcRmin                         = ((SHARPEN_RCRMIN) * 9) >> 4       ;
    pstRegCfg->stSharpenRegCfg.u8RcRmax                         = ((SHARPEN_RCRMAX) * 9) >> 4       ;
    pstRegCfg->stSharpenRegCfg.u8RcRThd                         = ((SHARPEN_RCRTHD) * 9) >> 4       ;
    pstRegCfg->stSharpenRegCfg.u8rWgt                           = pstSharpen->u8RGain       ;
    pstRegCfg->stSharpenRegCfg.s8rShpGainSft                    = 31    ;
    pstRegCfg->stSharpenRegCfg.u8BcBmin                         = ((SHARPEN_BCBMIN) * 9) >> 4       ;
    pstRegCfg->stSharpenRegCfg.u8BcBmax                         = ((SHARPEN_BCBMAX) * 9) >> 4       ;
    pstRegCfg->stSharpenRegCfg.u8BcRmin                         = ((SHARPEN_BCRMIN) * 9) >> 4       ;
    pstRegCfg->stSharpenRegCfg.u8BcRmax                         = ((SHARPEN_BCRMAX) * 9) >> 4       ;
    pstRegCfg->stSharpenRegCfg.u8BcBThd                         = ((SHARPEN_BCBTHD) * 9) >> 4       ;
    pstRegCfg->stSharpenRegCfg.u8bWgt                           = pstSharpen->u8BGain       ;
    pstRegCfg->stSharpenRegCfg.s8bShpGainSft                    = 31;
    pstRegCfg->stSharpenRegCfg.u8GcBmin                         = ((SHARPEN_GCBMIN) * 9) >> 4       ;
    pstRegCfg->stSharpenRegCfg.u8GcBmax                         = ((SHARPEN_GCBMAX) * 9) >> 4       ;
    pstRegCfg->stSharpenRegCfg.u8GcRmin                         = ((SHARPEN_GCRMIN) * 9) >> 4       ;
    pstRegCfg->stSharpenRegCfg.u8GcRmax                         = ((SHARPEN_GCRMAX) * 9) >> 4       ;
    pstRegCfg->stSharpenRegCfg.u8gWgt                           = pstSharpen->u8GGain       ;
    pstRegCfg->stSharpenRegCfg.bEnLowBandEsm                        = SHARPEN_ENLOWBANDESM      ;

    SharpenCheckReg(&(pstRegCfg->stSharpenRegCfg));

    /* Calc sharpThd1MulCoef */
    pstRegCfg->stSharpenRegCfg.u16SharpThd1MulCoef = CalcMulCoef(pstRegCfg->stSharpenRegCfg.u16SharpThd2, 0, pstRegCfg->stSharpenRegCfg.u16SharpThd1, 1, 12);

    /* Calc edgeThd1MulCoef */
    pstRegCfg->stSharpenRegCfg.u16EdgeThd1MulCoef  = CalcMulCoef(pstRegCfg->stSharpenRegCfg.u16EdgeThd2, 0,pstRegCfg->stSharpenRegCfg.u16EdgeThd1, 1, 12);

    pstRegCfg->stSharpenRegCfg.s16EdgeJagMulCoef = CalcMulCoef(pstRegCfg->stSharpenRegCfg.u16VarJagThd1, pstRegCfg->stSharpenRegCfg.u16EdgeAmt,
                                                               pstRegCfg->stSharpenRegCfg.u16VarJagThd2, pstRegCfg->stSharpenRegCfg.u16EdgeJagAmt, 0);

    pstRegCfg->stSharpenRegCfg.s16OshtJagMulCoef = CalcMulCoef(pstRegCfg->stSharpenRegCfg.u16VarJagThd1, pstRegCfg->stSharpenRegCfg.u8OvershootAmt,
                                                               pstRegCfg->stSharpenRegCfg.u16VarJagThd2, pstRegCfg->stSharpenRegCfg.u8OshtJagAmt, 4);
    pstRegCfg->stSharpenRegCfg.s16UshtJagMulCoef = CalcMulCoef(pstRegCfg->stSharpenRegCfg.u16VarJagThd1, pstRegCfg->stSharpenRegCfg.u8UndershootAmt,
                                                               pstRegCfg->stSharpenRegCfg.u16VarJagThd2, pstRegCfg->stSharpenRegCfg.u8UshtJagAmt, 4);

    pstRegCfg->stSharpenRegCfg.s16OShtVarMul = CalcMulCoef(pstRegCfg->stSharpenRegCfg.au16OShtVarTh[0], pstRegCfg->stSharpenRegCfg.u8OShtVarAmt,
                                            pstRegCfg->stSharpenRegCfg.au16OShtVarTh[1], pstRegCfg->stSharpenRegCfg.u8OvershootAmt, 4);
    pstRegCfg->stSharpenRegCfg.s16UShtVarMul = CalcMulCoef(pstRegCfg->stSharpenRegCfg.au16UShtVarTh[0], pstRegCfg->stSharpenRegCfg.u8UShtVarAmt,
                                            pstRegCfg->stSharpenRegCfg.au16UShtVarTh[1], pstRegCfg->stSharpenRegCfg.u8UndershootAmt, 4);

    pstRegCfg->stSharpenRegCfg.u8skinCntMulCoef     = CalcMulCoef(pstRegCfg->stSharpenRegCfg.u8skin_cnt_th1, 0,
                                                           pstRegCfg->stSharpenRegCfg.u8skin_cnt_th2, 31, 0);
    pstRegCfg->stSharpenRegCfg.s16skinVarWgtMulCoef = CalcMulCoef(pstRegCfg->stSharpenRegCfg.u16skin_var_th1, pstRegCfg->stSharpenRegCfg.u8skin_var_wgt1,
                                                             pstRegCfg->stSharpenRegCfg.u16skin_var_th2, pstRegCfg->stSharpenRegCfg.u8skin_var_wgt2, 4);

    // shoot suppression
    pstRegCfg->stSharpenRegCfg.u16OvShtSupMul = CalcMulCoef(pstRegCfg->stSharpenRegCfg.u8OvShtVarMin, pstRegCfg->stSharpenRegCfg.u8OvShtWgtMin, pstRegCfg->stSharpenRegCfg.u8OvShtVarMax, 255, 4);

    pstRegCfg->stSharpenRegCfg.u16UnShtSupMul = CalcMulCoef(pstRegCfg->stSharpenRegCfg.u8UnShtVarMin, pstRegCfg->stSharpenRegCfg.u8UnShtWgtMin, pstRegCfg->stSharpenRegCfg.u8UnShtVarMax, 255, 4);

    pstRegCfg->stSharpenRegCfg.u16UnShtSupDiffMul = CalcMulCoef(pstRegCfg->stSharpenRegCfg.u8UnShtDiffMin, 255, pstRegCfg->stSharpenRegCfg.u8UnShtDiffMax, pstRegCfg->stSharpenRegCfg.u8UnShtDiffWgtMin, 4);

    pstRegCfg->stSharpenRegCfg.u16OvShtSupDiffMul = CalcMulCoef(pstRegCfg->stSharpenRegCfg.u8OvShtDiffMin, 255, pstRegCfg->stSharpenRegCfg.u8OvShtDiffMax, pstRegCfg->stSharpenRegCfg.u8OvShtDiffWgtMin, 4);

    pstRegCfg->stSharpenRegCfg.u8ShadWgtMul = CalcMulCoef(pstRegCfg->stSharpenRegCfg.u8gradThd1, pstRegCfg->stSharpenRegCfg.u8shadWgt1, pstRegCfg->stSharpenRegCfg.u8gradThd2, pstRegCfg->stSharpenRegCfg.u8shadWgt2, 3);

    pstRegCfg->stSharpenRegCfg.s16rShpGainMul = CalcMulCoef(pstRegCfg->stSharpenRegCfg.u8RcRmin, 31, pstRegCfg->stSharpenRegCfg.u8RcRThd, pstRegCfg->stSharpenRegCfg.u8rWgt, 5);

    pstRegCfg->stSharpenRegCfg.s16bShpGainMul = CalcMulCoef(pstRegCfg->stSharpenRegCfg.u8BcBmin, 31, pstRegCfg->stSharpenRegCfg.u8BcBThd, pstRegCfg->stSharpenRegCfg.u8bWgt, 5);

    pstRegCfg->unKey.bit1SharpenCfg = 1;

    return HI_SUCCESS;
}

HI_S32 ISP_SharpenCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{

    switch (u32Cmd)
    {
        case ISP_WDR_MODE_SET :
            ISP_SharpenInit(IspDev);
            break;
        case ISP_PROC_WRITE:
            SharpenProcWrite(IspDev, (ISP_CTRL_PROC_WRITE_S *)pValue);
            break;
        case ISP_CHANGE_IMAGE_MODE_SET:
            SharpenImageResWrite(IspDev, (ISP_CMOS_SENSOR_IMAGE_MODE_S *)pValue);
            break;
        default :
            break;
    }
    return HI_SUCCESS;
}

HI_S32 ISP_SharpenExit(ISP_DEV IspDev)
{
    hi_isp_sharpen_cfg_enable_write(IspDev, HI_FALSE);
    hi_isp_sharpen_pixsel_write(IspDev, HI_FALSE);
    hi_ext_system_sharpen_manu_mode_write(HI_FALSE);

    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterSharpen(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;

    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_SHARPEN;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_SharpenInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_SharpenRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_SharpenCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_SharpenExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


