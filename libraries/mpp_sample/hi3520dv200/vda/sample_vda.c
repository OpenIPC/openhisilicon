/******************************************************************************
  A simple program of Hisilicon HI3516 vda implementation.
  the flow as follows:
    1) init mpp system.
    2) start vi( internal isp, ViDev 0, vichn0) and vo (HD)                  
    3) vda md & od start & print information
    4) stop vi vo and system.
  Copyright (C), 2010-2011, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
    Modification:  2012-12 Created
******************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#include "sample_comm.h"
#include "loadbmp.h"

VIDEO_NORM_E gs_enNorm = VIDEO_ENCODING_MODE_PAL;
HI_U32    gs_u32ViFrmRate = 0;
/******************************************************************************
* function : to process abnormal case                                        
******************************************************************************/
void SAMPLE_VDA_HandleSig(HI_S32 signo)
{
    if (SIGINT == signo || SIGTSTP == signo)
    {
        SAMPLE_COMM_SYS_Exit();
        printf("\033[0;31mprogram exit abnormally!\033[0;39m\n");
    }

    exit(0);
}
/******************************************************************************
* function : show usage
******************************************************************************/
void SAMPLE_VDA_Usage(char *sPrgNm)
{
    printf("Usage : %s <index>\n", sPrgNm);
    printf("index:\n");
    printf("\t 0) VI:1*Cif MD+OD.\n");
    printf("\t 1) VI:MixCap MD+OD. .\n");			
    return;
}

/******************************************************************************
* function      : main() 
* Description : Vi/VO + VDA(MD&OD)
*               DC -> VI-PortA ViChn0(1080p) -> VO HD
*                              ViChn1(D1)    -> VdaChn0 MD
*                                            -> VdaChn1 OD
******************************************************************************/
//int main(int argc, char *argv[])
HI_S32 SAMPLE_Vda_Cif(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;
    VI_CHN ViChn, ViChn_Md = 0, ViChn_Od = 0;
    VDA_CHN VdaChn_Md = 0, VdaChn_Od = 1;
    VB_CONF_S stVbConf ={0};	/* vb config define */
    PIC_SIZE_E enSize_Md = PIC_CIF, enSize_Od = PIC_CIF; 	/* vda picture size */

    SAMPLE_VI_MODE_E enViMode = SAMPLE_VI_MODE_1_D1;
    HI_U32 u32ViChnCnt = 1;
    
    VO_DEV VoDev;
    VO_CHN VoChn;
    VO_PUB_ATTR_S stVoPubAttr; 
    SAMPLE_VO_MODE_E enVoMode;
    
    HI_S32 i;
    HI_U32 u32BlkSize;
    SIZE_S stSize;
    HI_U32 u32WndNum;

    /******************************************
     step  1: init global  variable 
    ******************************************/
    gs_u32ViFrmRate = (VIDEO_ENCODING_MODE_PAL== gs_enNorm)?25:30;
    
    memset(&stVbConf,0,sizeof(VB_CONF_S));

    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm,\
                PIC_D1, SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH);
    stVbConf.u32MaxPoolCnt = 128;

    stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt = u32ViChnCnt * 10;

    /******************************************
     step 2: mpp system init. 
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        goto END0;
    }

    /******************************************
     step 3: start vi dev & chn to capture
    ******************************************/
    s32Ret = SAMPLE_COMM_VI_Start(enViMode, gs_enNorm);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vi failed!\n");
        goto END0;
    }
#if 0    
    /******************************************
     step 5: start VO to preview
    ******************************************/

    VoDev = SAMPLE_VO_DEV_DSD0;
    u32WndNum = 1;
    enVoMode = VO_MODE_1MUX;
    
    stVoPubAttr.enIntfSync = VO_OUTPUT_PAL;
    stVoPubAttr.enIntfType = VO_INTF_CVBS;
    stVoPubAttr.u32BgColor = 0x000000ff;
    stVoPubAttr.bDoubleFrame = HI_FALSE;
#endif
    /******************************************
         step 4: start VO to preview
    ******************************************/
    VoDev = SAMPLE_VO_DEV_DHD0;
    u32WndNum = 1;
    enVoMode = VO_MODE_1MUX;

    stVoPubAttr.enIntfSync = VO_OUTPUT_720P50;
    stVoPubAttr.enIntfType = VO_INTF_HDMI|VO_INTF_VGA;
    stVoPubAttr.u32BgColor = 0x000000ff;
    stVoPubAttr.bDoubleFrame = HI_FALSE;

    s32Ret = SAMPLE_COMM_VO_StartDevLayer(VoDev, &stVoPubAttr, gs_u32ViFrmRate);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartDevLayer failed!\n");
        goto END0;
    }
    
    s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, &stVoPubAttr, enVoMode);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartChn failed!\n");
        goto END0;
    }

    for(i=0;i<u32WndNum;i++)
    {
        VoChn = i;
        ViChn = i;
        s32Ret = SAMPLE_COMM_VO_BindVi(VoDev,VoChn,ViChn);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("SAMPLE_COMM_VO_BindVi failed!\n");
            goto END0;
        }
    }

    /******************************************
     step  5: VDA process
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(gs_enNorm, enSize_Md, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        goto END0;
    }
    s32Ret = SAMPLE_COMM_VDA_MdStart(VdaChn_Md, ViChn_Md, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("VDA Md Start failed!\n");
        goto END0;
    }
    
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(gs_enNorm, enSize_Od, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        goto END0;
    }
    s32Ret = SAMPLE_COMM_VDA_OdStart(VdaChn_Od, ViChn_Od, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("VDA OD Start failed!\n");
        goto END0;
    }

    printf("Press any key to stop!");
    getchar();

    SAMPLE_COMM_VDA_OdStop(VdaChn_Od, ViChn_Od);
    SAMPLE_COMM_VDA_MdStop(VdaChn_Md, ViChn_Md);
    /******************************************
     step  6: stop vi vo & sys
    ******************************************/
    /******************************************
     step 7: exit process
    ******************************************/
    for(i=0;i<u32WndNum;i++)
    {
        VoChn = i;
        SAMPLE_COMM_VO_UnBindVi(VoDev,VoChn);
    }
    SAMPLE_COMM_VO_StopChn(VoDev, enVoMode);
    SAMPLE_COMM_VO_StopDevLayer(VoDev);
    SAMPLE_COMM_VI_Stop(enViMode);
END0:
    SAMPLE_COMM_SYS_Exit();
    
    return s32Ret;
}


/******************************************************************************
* function      : main() 
* Description : Vi/VO + VDA(MD&OD)
*               DC -> VI-PortA ViChn0(1080p) -> VO HD
*                              ViChn1(D1)    -> VdaChn0 MD
*                                            -> VdaChn1 OD
******************************************************************************/
//int main(int argc, char *argv[])
HI_S32 SAMPLE_Vda_MixCap(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;
    VI_CHN ViChn_Md = 0, ViChn_Od = 0;
    VDA_CHN VdaChn_Md = 0, VdaChn_Od = 1;
    VB_CONF_S stVbConf ={0};	/* vb config define */
    PIC_SIZE_E enSize_Md = PIC_CIF, enSize_Od = PIC_CIF; 	/* vda picture size */

    SAMPLE_VI_MODE_E enViMode = SAMPLE_VI_MODE_1_D1Cif;
    HI_U32 u32ViChnCnt = 1;
	
    HI_S32 s32VpssGrpCnt = 16;
    VPSS_GRP VpssGrp;
    VPSS_GRP_ATTR_S stGrpAttr;
	
    VO_DEV VoDev;
    VO_CHN VoChn;
    VO_PUB_ATTR_S stVoPubAttr; 
    SAMPLE_VO_MODE_E enVoMode;
    
    HI_S32 i;
    HI_U32 u32BlkSize;
    SIZE_S stSize;
    HI_U32 u32WndNum;

    /******************************************
     step  1: init global  variable 
    ******************************************/
    gs_u32ViFrmRate = (VIDEO_ENCODING_MODE_PAL== gs_enNorm)?25:30;
    
    memset(&stVbConf,0,sizeof(VB_CONF_S));

    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm,\
                PIC_D1, SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH);
    stVbConf.u32MaxPoolCnt = 128;
    stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt = u32ViChnCnt * 4;
	
    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm,\
                PIC_2CIF, SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH);
    stVbConf.u32MaxPoolCnt = 128;
    stVbConf.astCommPool[1].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[1].u32BlkCnt = u32ViChnCnt * 6;

    /* hist buf*/
    stVbConf.astCommPool[2].u32BlkSize = (196*4);
    stVbConf.astCommPool[2].u32BlkCnt =  32;
    memset(stVbConf.astCommPool[2].acMmzName,0,
    sizeof(stVbConf.astCommPool[2].acMmzName));
	

    /******************************************
     step 2: mpp system init. 
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        goto END_0;
    }

    /******************************************
     step 3: start vi dev & chn to capture
    ******************************************/
    s32Ret = SAMPLE_COMM_VI_MixCap_Start(enViMode, gs_enNorm);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vi failed!\n");
        goto END_0;
    }

	/******************************************
	step 4: start vpss and vi bind vpss
	******************************************/
	s32Ret = SAMPLE_COMM_SYS_GetPicSize(gs_enNorm, PIC_D1, &stSize);
	if (HI_SUCCESS != s32Ret)
	{
	   SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
	   goto END_1;
	}

	stGrpAttr.u32MaxW = 720;
	stGrpAttr.u32MaxH = (VIDEO_ENCODING_MODE_PAL==gs_enNorm)?576:480;
	stGrpAttr.bDrEn = HI_FALSE;
	stGrpAttr.bDbEn = HI_FALSE;
	stGrpAttr.bIeEn = HI_TRUE;
	stGrpAttr.bNrEn = HI_TRUE;
	stGrpAttr.bHistEn = HI_TRUE;
	stGrpAttr.enDieMode = VPSS_DIE_MODE_AUTO;
	stGrpAttr.enPixFmt = SAMPLE_PIXEL_FORMAT;

	s32Ret = SAMPLE_COMM_VPSS_Start(s32VpssGrpCnt, &stSize, VPSS_MAX_CHN_NUM,NULL);
	if (HI_SUCCESS != s32Ret)
	{
	   SAMPLE_PRT("Start Vpss failed!\n");
	   goto END_1;
	}
	/*open pre-scale*/
	s32Ret = SAMPLE_COMM_SYS_GetPicSize(gs_enNorm, PIC_2CIF, &stSize);
	if (HI_SUCCESS != s32Ret)
	{
	   SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
	   goto END_2;
	}
	for(i=0;i<s32VpssGrpCnt;i++)
	{   
	   s32Ret = SAMPLE_COMM_EnableVpssPreScale(i,stSize);
	   if(HI_SUCCESS != s32Ret)
	   {
	   SAMPLE_PRT("HI_MPI_VPSS_SetPreScale failed!\n");
	   goto END_2;
	   }
	}
	   
	s32Ret = SAMPLE_COMM_VI_BindVpss(enViMode);
	if (HI_SUCCESS != s32Ret)
	{
	   SAMPLE_PRT("Vi bind Vpss failed!\n");
	   goto END_2;
	}

	/******************************************
	step 5: start vo HD0(HDMI) 
	******************************************/
	printf("start vo HD0.\n");
	VoDev = SAMPLE_VO_DEV_DHD0;
	u32WndNum = 1;
	enVoMode = VO_MODE_1MUX;

	if(VIDEO_ENCODING_MODE_PAL == gs_enNorm)
	{
	   stVoPubAttr.enIntfSync = VO_OUTPUT_720P50;
	}
	else
	{
	   stVoPubAttr.enIntfSync = VO_OUTPUT_720P60;
	}

	stVoPubAttr.enIntfType = VO_INTF_HDMI|VO_INTF_VGA;

	stVoPubAttr.u32BgColor = 0x000000ff;
	stVoPubAttr.bDoubleFrame = HI_TRUE;

	s32Ret = SAMPLE_COMM_VO_StartDevLayer(VoDev, &stVoPubAttr, gs_u32ViFrmRate);
	if (HI_SUCCESS != s32Ret)
	{
	   SAMPLE_PRT("Start SAMPLE_COMM_VO_StartDevLayer failed!\n");
	   goto END_3;
	}

	s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, &stVoPubAttr, enVoMode);
	if (HI_SUCCESS != s32Ret)
	{
	   SAMPLE_PRT("Start SAMPLE_COMM_VO_StartChn failed!\n");
	   goto END_3;
	}

	/* if it's displayed on HDMI, we should start HDMI */
	if (stVoPubAttr.enIntfType & VO_INTF_HDMI)
	{
	   if (HI_SUCCESS != SAMPLE_COMM_VO_HdmiStart(stVoPubAttr.enIntfSync))
	   {
		   SAMPLE_PRT("Start SAMPLE_COMM_VO_HdmiStart failed!\n");
		   goto END_4;
	   }
	}

	/*Only show the chn which vda bind*/
	VoChn = 0;
	VpssGrp = 0;

	s32Ret = SAMPLE_COMM_VO_BindVpss(VoDev,VoChn,VpssGrp,VPSS_PRE0_CHN);
	if (HI_SUCCESS != s32Ret)
	{
	   SAMPLE_PRT("Start VO failed!\n");
	   goto END_4;
	}

    /******************************************
     step  6: VDA process
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(gs_enNorm, enSize_Md, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        goto END_4;
    }
    s32Ret = SAMPLE_COMM_VDA_MdStart(VdaChn_Md, ViChn_Md, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("VDA Md Start failed!\n");
        goto END_4;
    }
    
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(gs_enNorm, enSize_Od, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        goto END_4;
    }
    s32Ret = SAMPLE_COMM_VDA_OdStart(VdaChn_Od, ViChn_Od, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("VDA OD Start failed!\n");
        goto END_4;
    }

    printf("Press any key to stop!");
    getchar();

    SAMPLE_COMM_VDA_OdStop(VdaChn_Od, ViChn_Od);
    SAMPLE_COMM_VDA_MdStop(VdaChn_Md, ViChn_Md);
    /******************************************
     step  7: stop vi vo & sys
    ******************************************/
    /******************************************
     step 8: exit process
    ******************************************/
END_4:
	if (stVoPubAttr.enIntfType & VO_INTF_HDMI)
    {
        SAMPLE_COMM_VO_HdmiStop();
    }
    VoDev = SAMPLE_VO_DEV_DHD0;
    u32WndNum = 16;
    enVoMode = VO_MODE_16MUX;
    for(i=0;i<u32WndNum;i++)
    {
        VoChn = i;
        VpssGrp = i;
        SAMPLE_COMM_VO_UnBindVpss(VoDev,VoChn,VpssGrp,VPSS_PRE0_CHN);
    }
    SAMPLE_COMM_VO_StopChn(VoDev, enVoMode);
    SAMPLE_COMM_VO_StopDevLayer(VoDev);	
END_3:
    SAMPLE_COMM_VI_UnBindVpss(enViMode);
END_2:
    SAMPLE_COMM_VPSS_Stop(s32VpssGrpCnt, VPSS_MAX_CHN_NUM);
END_1:
    SAMPLE_COMM_VI_Stop(enViMode);
END_0:
    SAMPLE_COMM_SYS_Exit();
    return s32Ret;
}

/******************************************************************************
* function    : main()
* Description : video preview sample
******************************************************************************/
int main(int argc, char *argv[])
{
    HI_S32 s32Ret;

    if ( (argc < 2) || (1 != strlen(argv[1])))
    {
        SAMPLE_VDA_Usage(argv[0]);
        return HI_FAILURE;
    }

    signal(SIGINT, SAMPLE_VDA_HandleSig);
    signal(SIGTERM, SAMPLE_VDA_HandleSig);

    switch (*argv[1])
    {
        case '0':/* Cif MD + OD. */
            s32Ret = SAMPLE_Vda_Cif();
            break;
        case '1':/* Mix Cap MD + OD */
            s32Ret = SAMPLE_Vda_MixCap();
            break;

        default:
            SAMPLE_VDA_Usage(argv[0]);
            return HI_FAILURE;
    }

    if (HI_SUCCESS == s32Ret)
        printf("program exit normally!\n");
    else
        printf("program exit abnormally!\n");
    exit(s32Ret);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
