/******************************************************************************
  A simple program of Hisilicon HI3520D video input and output implementation.
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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "sample_comm.h"

VIDEO_NORM_E gs_enNorm = VIDEO_ENCODING_MODE_PAL;
SAMPLE_VIDEO_LOSS_S gs_stVideoLoss;
HI_U32 gs_u32ViFrmRate = 0; 

/******************************************************************************
* function : show usage
******************************************************************************/
void SAMPLE_VIO_Usage(char *sPrgNm)
{
    printf("Usage : %s <index>\n", sPrgNm);
    printf("index:\n");
    printf("\t 0) VI:8*D1; VO:HD0(HDMI,VGA)+SD0(CVBS)+SD1 video preview.\n");
    printf("\t 1) VI:1*1080p; VO:SD0(CVBS), HD0(HDMI,1080p@60 + VGA).\n");
    printf("\t 2) VI:VI:8*2cif; VO:HD0(HDMI  720P50), WBC to SD0(CVBS) video preview.\n");
    printf("\t 3) VI:VI:8*D1(6fps)/2cif(19fps); VO:HD0(HDMI  720P50)video preview\n");	
    printf("\t 4) VI: 1*D1, user picture; VO: SD0(CVBS) video preview.\n");
    printf("\t 5) VI: 1*D1; VO:HD0(VGA) HD Zoom preview.\n");
    printf("\t 6) VI: 1*D1; VO:SD0(CVBS) SD Zoom preview.\n");
    printf("\t 7) VI: 4*720p; HD0(HDMI,1080p@50 + VGA). Only for Hi3520DV200\n");

    return;
}

/******************************************************************************
* function : to process abnormal case                                         
******************************************************************************/
void SAMPLE_VIO_HandleSig(HI_S32 signo)
{
    if (SIGINT == signo || SIGTSTP == signo)
    {
        SAMPLE_COMM_SYS_Exit();
        printf("\033[0;31mprogram termination abnormally!\033[0;39m\n");
    }
    exit(-1);
}

/******************************************************************************
* function : video loss detect process                                         
* NOTE: If your ADC stop output signal when NoVideo, you can open VDET_USE_VI macro.
******************************************************************************/

void *SAMPLE_VI_AD26828_VLossDetProc(void *parg)
{  
    int fd;
    HI_S32 s32Ret, i, s32ChnPerDev;
    VI_DEV ViDev;
    VI_CHN ViChn;
    tw2865_video_loss video_loss;
    SAMPLE_VI_PARAM_S stViParam;
    SAMPLE_VIDEO_LOSS_S *ctl = (SAMPLE_VIDEO_LOSS_S*)parg;
    
    fd = open(CX26828_FILE, O_RDWR);//TW2865_FILE
    if (fd < 0)
    {
        printf("open %s fail\n", CX26828_FILE);
        ctl->bStart = HI_FALSE;
        return NULL;
    }

    s32Ret = SAMPLE_COMM_VI_Mode2Param(ctl->enViMode, &stViParam);
    if (HI_SUCCESS !=s32Ret)
    {
        SAMPLE_PRT("vi get param failed!\n");
        return NULL;
    }
    s32ChnPerDev = stViParam.s32ViChnCnt / stViParam.s32ViDevCnt;
    
    while (ctl->bStart)
    {
        for (i = 0; i < stViParam.s32ViChnCnt; i++)
        {
            ViChn = i * stViParam.s32ViChnInterval;
            ViDev = SAMPLE_COMM_VI_GetDev(ctl->enViMode, ViChn);
            if (ViDev < 0)
            {
                SAMPLE_PRT("get vi dev failed !\n");
                return NULL;
            }
            
            //video_loss.chip = stViParam.s32ViDevCnt;
            video_loss.chip = 0;
            video_loss.ch   = ViChn % s32ChnPerDev;
			//printf("video_loss.chip %d, video_loss.ch %d\n",video_loss.chip,video_loss.ch );
            ioctl(fd, CX26828_GET_VIDEO_LOSS, &video_loss);

            if (video_loss.is_lost)
            {   
                printf("pic loss\n");
                HI_MPI_VI_EnableUserPic(ViChn);
            }
            else
            {
                HI_MPI_VI_DisableUserPic(ViChn);
            }                
        }
        usleep(500000);
    }
    
    close(fd);
    ctl->bStart = HI_FALSE;
    
    return NULL;
}

void *SAMPLE_VI_AD2865_VLossDetProc(void *parg)
{  
    int fd;
    HI_S32 s32Ret, i, s32ChnPerDev;
    VI_DEV ViDev;
    VI_CHN ViChn;
    tw2865_video_loss video_loss;
    SAMPLE_VI_PARAM_S stViParam;
    SAMPLE_VIDEO_LOSS_S *ctl = (SAMPLE_VIDEO_LOSS_S*)parg;
    
    fd = open(TW2865_FILE, O_RDWR);//TW2865_FILE
    if (fd < 0)
    {
        printf("open %s fail\n", TW2865_FILE);
        ctl->bStart = HI_FALSE;
        return NULL;
    }

    s32Ret = SAMPLE_COMM_VI_Mode2Param(ctl->enViMode, &stViParam);
    if (HI_SUCCESS !=s32Ret)
    {
        SAMPLE_PRT("vi get param failed!\n");
        return NULL;
    }
    s32ChnPerDev = stViParam.s32ViChnCnt / stViParam.s32ViDevCnt;
    
    while (ctl->bStart)
    {
        for (i = 0; i < stViParam.s32ViChnCnt; i++)
        {
            ViChn = i * stViParam.s32ViChnInterval;
            ViDev = SAMPLE_COMM_VI_GetDev(ctl->enViMode, ViChn);
            if (ViDev < 0)
            {
                SAMPLE_PRT("get vi dev failed !\n");
                return NULL;
            }
            
            //video_loss.chip = stViParam.s32ViDevCnt;
            video_loss.chip = 0;
            video_loss.ch   = ViChn % s32ChnPerDev;
			//printf("video_loss.chip %d, video_loss.ch %d\n",video_loss.chip,video_loss.ch );
            ioctl(fd, TW2865_GET_VIDEO_LOSS, &video_loss);

            if (video_loss.is_lost)
            {   
                printf("pic loss\n");
                HI_MPI_VI_EnableUserPic(ViChn);
            }
            else
            {
                HI_MPI_VI_DisableUserPic(ViChn);
            }                
        }
        usleep(500000);
    }
    
    close(fd);
    ctl->bStart = HI_FALSE;
    
    return NULL;
}


//#define VDET_USE_VI   
#ifdef VDET_USE_VI
static HI_S32 s_astViLastIntCnt[VIU_MAX_CHN_NUM] = {0};
void *SAMPLE_VI_VLossDetProc(void *parg)
{ 
    VI_CHN ViChn;
    SAMPLE_VI_PARAM_S stViParam;
    HI_S32 s32Ret, i, s32ChnPerDev;
    VI_CHN_STAT_S stStat;
    SAMPLE_VIDEO_LOSS_S *ctl = (SAMPLE_VIDEO_LOSS_S*)parg;
    
    s32Ret = SAMPLE_COMM_VI_Mode2Param(ctl->enViMode, &stViParam);
    if (HI_SUCCESS !=s32Ret)
    {
        SAMPLE_PRT("vi get param failed!\n");
        return NULL;
    }
    s32ChnPerDev = stViParam.s32ViChnCnt / stViParam.s32ViDevCnt;
    
    while (ctl->bStart)
    {
        for (i = 0; i < stViParam.s32ViChnCnt; i++)
        {
            ViChn = i * stViParam.s32ViChnInterval;

            s32Ret = HI_MPI_VI_Query(ViChn, &stStat);
            if (HI_SUCCESS !=s32Ret)
            {
                SAMPLE_PRT("HI_MPI_VI_Query failed with %#x!\n", s32Ret);
                return NULL;
            }            

            if (stStat.u32IntCnt == s_astViLastIntCnt[i])
            
            {
                printf("VI Chn (%d) int lost , int_cnt:%d \n", ViChn, stStat.u32IntCnt);
                HI_MPI_VI_EnableUserPic(ViChn);
            }
            
            else
            {
                HI_MPI_VI_DisableUserPic(ViChn);
            }
            s_astViLastIntCnt[i] = stStat.u32IntCnt;
        }
        usleep(500000);
    }
    
    ctl->bStart = HI_FALSE;
    
    return NULL;
}

#endif

HI_S32 SAMPLE_VI_StartVLossDet(SAMPLE_VI_MODE_E enViMode)
{
    HI_S32 s32Ret;
    
    gs_stVideoLoss.bStart= HI_TRUE;
    gs_stVideoLoss.enViMode = enViMode;
#ifdef VDET_USE_VI  
    s32Ret = pthread_create(&gs_stVideoLoss.Pid, 0, SAMPLE_VI_VLossDetProc, &gs_stVideoLoss);
#else
    #ifdef DEMO
    s32Ret = pthread_create(&gs_stVideoLoss.Pid, 0, SAMPLE_VI_AD26828_VLossDetProc, &gs_stVideoLoss);
    #else
    s32Ret = pthread_create(&gs_stVideoLoss.Pid, 0, SAMPLE_VI_AD2865_VLossDetProc, &gs_stVideoLoss);
    #endif
#endif
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("pthread_create failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}



HI_VOID SAMPLE_VI_StopVLossDet()
{
    if (gs_stVideoLoss.bStart)
    {
        gs_stVideoLoss.bStart = HI_FALSE;
        pthread_join(gs_stVideoLoss.Pid, 0);
    }
    return;
}

HI_S32 SAMPLE_VI_SetUserPic(HI_CHAR *pszYuvFile, HI_U32 u32Width, HI_U32 u32Height,
        HI_U32 u32Stride, VIDEO_FRAME_INFO_S *pstFrame)
{
    FILE *pfd;
    VI_USERPIC_ATTR_S stUserPicAttr;

    /* open YUV file */
    pfd = fopen(pszYuvFile, "rb");
    if (!pfd)
    {
        printf("open file -> %s fail \n", pszYuvFile);
        return -1;
    }

    /* read YUV file. WARNING: we only support planar 420) */
    if (SAMPLE_COMM_VI_GetVFrameFromYUV(pfd, u32Width, u32Height, u32Stride, pstFrame))
    {
        return -1;
    }
    fclose(pfd);

    stUserPicAttr.bPub= HI_TRUE;
    stUserPicAttr.enUsrPicMode = VI_USERPIC_MODE_PIC;
    memcpy(&stUserPicAttr.unUsrPic.stUsrPicFrm, pstFrame, sizeof(VIDEO_FRAME_INFO_S));
    if (HI_MPI_VI_SetUserPic(0, &stUserPicAttr))
    {
        return -1;
    }

    printf("set vi user pic ok, yuvfile:%s\n", pszYuvFile);
    return HI_SUCCESS;
}

/******************************************************************************
* function :  VI:8*D1; VO:HD0(HDMI,VGA)+SD0(CVBS)+SD1 video preview
******************************************************************************/
HI_S32 SAMPLE_VIO_8_D1(HI_VOID)
{
    SAMPLE_VI_MODE_E enViMode = SAMPLE_VI_MODE_8_D1;

    HI_U32 u32ViChnCnt = 8;
    HI_S32 s32VpssGrpCnt = 8;
    
    VB_CONF_S stVbConf;
    VI_CHN ViChn;
    VPSS_GRP VpssGrp;
    VPSS_GRP_ATTR_S stGrpAttr;
    VPSS_CHN VpssChn_VoHD0 = VPSS_PRE0_CHN;
    VO_DEV VoDev;
    VO_CHN VoChn;
    VO_PUB_ATTR_S stVoPubAttr,stVoPubAttrSD; 
    SAMPLE_VO_MODE_E enVoMode, enPreVoMode;
    
    HI_S32 i;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32BlkSize;
    HI_CHAR ch;
    SIZE_S stSize;
    HI_U32 u32WndNum;
	
	VO_WBC_ATTR_S stWbcAttr;

    /******************************************
     step  1: init variable 
    ******************************************/
    gs_u32ViFrmRate = (VIDEO_ENCODING_MODE_PAL== gs_enNorm)?25:30;
    
    memset(&stVbConf,0,sizeof(VB_CONF_S));

    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm,\
                PIC_D1, SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH);
    stVbConf.u32MaxPoolCnt = 128;

    /* video buffer*/
    //todo: vb=15
    stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt = u32ViChnCnt * 8;

    /******************************************
     step 2: mpp system init. 
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        goto END_8D1_0;
    }

    /******************************************
     step 3: start vi dev & chn
    ******************************************/
    s32Ret = SAMPLE_COMM_VI_Start(enViMode, gs_enNorm);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vi failed!\n");
        goto END_8D1_0;
    }

    /******************************************
     step 4: start vpss and vi bind vpss
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(gs_enNorm, PIC_D1, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        goto END_8D1_0;
    }
    
    stGrpAttr.u32MaxW = stSize.u32Width;
    stGrpAttr.u32MaxH = stSize.u32Height;
    stGrpAttr.bDrEn = HI_FALSE;
    stGrpAttr.bDbEn = HI_FALSE;
    stGrpAttr.bIeEn = HI_TRUE;
    stGrpAttr.bNrEn = HI_TRUE;
    stGrpAttr.bHistEn = HI_FALSE;
    stGrpAttr.enDieMode = VPSS_DIE_MODE_AUTO;
    stGrpAttr.enPixFmt = SAMPLE_PIXEL_FORMAT;

    s32Ret = SAMPLE_COMM_VPSS_Start(s32VpssGrpCnt, &stSize, VPSS_MAX_CHN_NUM,NULL);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start Vpss failed!\n");
        goto END_8D1_1;
    }

    s32Ret = SAMPLE_COMM_VI_BindVpss(enViMode);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Vi bind Vpss failed!\n");
        goto END_8D1_2;
    }
	/******************************************
	step 5: start vo SD1(CVBS)
	******************************************/

	printf("start vo SD1.\n");
	VoDev = SAMPLE_VO_DEV_DSD1;
	u32WndNum = 1;
	enVoMode = VO_MODE_1MUX;

	stVoPubAttr.enIntfSync = VO_OUTPUT_PAL;
	stVoPubAttr.enIntfType = VO_INTF_CVBS;
	stVoPubAttr.u32BgColor = 0x000000ff;
	stVoPubAttr.bDoubleFrame = HI_FALSE;
	s32Ret = SAMPLE_COMM_VO_StartDevLayer(VoDev, &stVoPubAttr, gs_u32ViFrmRate);
	if (HI_SUCCESS != s32Ret)
	{
		 SAMPLE_PRT("SAMPLE_COMM_VO_StartDevLayer failed!\n");
		 goto END_8D1_3;
	}

	s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, &stVoPubAttr, enVoMode);
	if (HI_SUCCESS != s32Ret)
	{
		 SAMPLE_PRT("SAMPLE_COMM_VO_StartChn failed!\n");
		 goto END_8D1_4;
	}

	VoChn = 0;
	ViChn = 0;
	s32Ret = SAMPLE_COMM_VO_BindVi(VoDev,VoChn, ViChn);
	if (HI_SUCCESS != s32Ret)
	{
		 SAMPLE_PRT("SAMPLE_COMM_VO_BindVpss failed!\n");
		 goto END_8D1_4;
	}
	
    /******************************************
     step 6: start vo HD0 (HDMI+VGA), multi-screen, you can switch mode
    ******************************************/
    printf("start vo HD0.\n");
    VoDev = SAMPLE_VO_DEV_DHD0;
    u32WndNum = 16;
    enVoMode = VO_MODE_1MUX;
    
    if(VIDEO_ENCODING_MODE_PAL == gs_enNorm)
    {
        stVoPubAttr.enIntfSync = VO_OUTPUT_1080P50;
    }
    else
    {
        stVoPubAttr.enIntfSync = VO_OUTPUT_1080P60;
    }
#ifdef HI_FPGA
	stVoPubAttr.enIntfType = VO_INTF_HDMI|VO_INTF_VGA|VO_INTF_BT1120;
#else 
	stVoPubAttr.enIntfType = VO_INTF_HDMI|VO_INTF_VGA;
#endif

    stVoPubAttr.u32BgColor = 0x000000ff;
    stVoPubAttr.bDoubleFrame = HI_TRUE;
    
    s32Ret = SAMPLE_COMM_VO_StartDevLayer(VoDev, &stVoPubAttr, gs_u32ViFrmRate);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start SAMPLE_COMM_VO_StartDevLayer failed!\n");
        goto END_8D1_4;
    }
    
    s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, &stVoPubAttr, enVoMode);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start SAMPLE_COMM_VO_StartChn failed!\n");
        goto END_8D1_5;
    }

    /* if it's displayed on HDMI, we should start HDMI */
    if (stVoPubAttr.enIntfType & VO_INTF_HDMI)
    {
        if (HI_SUCCESS != SAMPLE_COMM_VO_HdmiStart(stVoPubAttr.enIntfSync))
        {
            SAMPLE_PRT("Start SAMPLE_COMM_VO_HdmiStart failed!\n");
            goto END_8D1_5;
        }
    }
    
    for(i=0;i<u32WndNum;i++)
    {
        VoChn = i;
        VpssGrp = i;
        
        s32Ret = SAMPLE_COMM_VO_BindVpss(VoDev,VoChn,VpssGrp,VpssChn_VoHD0);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start VO failed!\n");
            goto END_8D1_5;
        }
    }

	/******************************************
	step 7: start vo SD0 (CVBS) (WBC target) 
	******************************************/
	printf("start vo SD0: wbc from hd0\n");
	VoDev = SAMPLE_VO_DEV_DSD0;
	u32WndNum = 16;
	enVoMode = VO_MODE_1MUX;

	stVoPubAttrSD.enIntfSync = VO_OUTPUT_PAL;
	stVoPubAttrSD.enIntfType = VO_INTF_CVBS;
	stVoPubAttrSD.u32BgColor = 0x000000ff;
	stVoPubAttrSD.bDoubleFrame = HI_FALSE;
	s32Ret = SAMPLE_COMM_VO_StartDevLayer(VoDev, &stVoPubAttrSD, gs_u32ViFrmRate);
	if (HI_SUCCESS != s32Ret)
	{
		  SAMPLE_PRT("SAMPLE_COMM_VO_StartDevLayer failed!\n");
		  goto END_8D1_5;
	}

	s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, &stVoPubAttrSD, enVoMode);
	if (HI_SUCCESS != s32Ret)
	{
		  SAMPLE_PRT("SAMPLE_COMM_VO_StartChn failed!\n");
		  goto END_8D1_6;
	}
	  
	s32Ret = SAMPLE_COMM_VO_GetWH(VO_OUTPUT_PAL, \
					  &stWbcAttr.stTargetSize.u32Width, \
					  &stWbcAttr.stTargetSize.u32Height, \
					  &stWbcAttr.u32FrameRate);
	if (HI_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("SAMPLE_COMM_VO_GetWH failed!\n");
		goto END_8D1_6;
	}
	stWbcAttr.enPixelFormat = SAMPLE_PIXEL_FORMAT;
    stWbcAttr.enDataSource = VO_WBC_DATASOURCE_MIXER;

	s32Ret = HI_MPI_VO_SetWbcAttr(SAMPLE_VO_DEV_DHD0, &stWbcAttr);
	if (HI_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("HI_MPI_VO_SetWbcAttr failed!\n");
		goto END_8D1_6;
	}

	s32Ret = HI_MPI_VO_EnableWbc(SAMPLE_VO_DEV_DHD0);
	if (HI_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("HI_MPI_VO_SetWbcAttr failed!\n");
		goto END_8D1_7;
	}

	s32Ret = SAMPLE_COMM_VO_BindVoWbc(SAMPLE_VO_DEV_DHD0, SAMPLE_VO_DEV_DSD0, 0);
	if (HI_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("HI_MPI_VO_SetWbcAttr failed!\n");
		goto END_8D1_7;
	}
	
	/******************************************
	step 8: HD0 switch mode 
	******************************************/
	VoDev = SAMPLE_VO_DEV_DHD0;
	enVoMode = VO_MODE_1MUX;
    while(1)
    {
        enPreVoMode = enVoMode;
    
        printf("please choose preview mode, press 'q' to exit this sample.\n"); 
        printf("\t0) 1 preview\n");
        printf("\t1) 4 preview\n");
        printf("\t2) 8 preview\n");
        printf("\tq) quit\n");
 
        ch = getchar();
        getchar();
        if ('0' == ch)
        {
            u32WndNum = 1;
            enVoMode = VO_MODE_1MUX;
        }
        else if ('1' == ch)
        {
            u32WndNum = 4;
            enVoMode = VO_MODE_4MUX;
        }
		/*Indeed only 8 chns show*/
        else if ('2' == ch)
        {
            u32WndNum = 9;
            enVoMode = VO_MODE_9MUX;
        }
        else if ('q' == ch)
        {
            break;
        }
        else
        {
            SAMPLE_PRT("preview mode invaild! please try again.\n");
            continue;
        }
        SAMPLE_PRT("vo(%d) switch to %d mode\n", VoDev, u32WndNum);

        s32Ret= HI_MPI_VO_SetAttrBegin(VoDev);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start VO failed!\n");
            goto END_8D1_7;
        }
        
        s32Ret = SAMPLE_COMM_VO_StopChn(VoDev, enPreVoMode);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start VO failed!\n");
            goto END_8D1_7;
        }

        s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, &stVoPubAttr, enVoMode);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start VO failed!\n");
            goto END_8D1_7;
        }
        s32Ret= HI_MPI_VO_SetAttrEnd(VoDev);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start VO failed!\n");
            goto END_8D1_7;
        }
    }

    /******************************************
     step 8: exit process
    ******************************************/

END_8D1_7:
	SAMPLE_COMM_VO_UnBindVoWbc(SAMPLE_VO_DEV_DSD0, 0);
	HI_MPI_VO_DisableWbc(SAMPLE_VO_DEV_DHD0);

END_8D1_6:
	VoDev = SAMPLE_VO_DEV_DSD0;
	VoChn = 0;
	enVoMode = VO_MODE_1MUX;
    SAMPLE_COMM_VO_StopChn(VoDev, enVoMode);
    SAMPLE_COMM_VO_StopDevLayer(VoDev);

END_8D1_5:
	if (stVoPubAttr.enIntfType & VO_INTF_HDMI)
    {
        SAMPLE_COMM_VO_HdmiStop();
    }
    VoDev = SAMPLE_VO_DEV_DHD0;
    u32WndNum = 16;
    enVoMode = VO_MODE_16MUX;
    /*先disableChn ,再解除绑定*/
    SAMPLE_COMM_VO_StopChn(VoDev, enVoMode);
    for(i=0;i<u32WndNum;i++)
    {
        VoChn = i;
        VpssGrp = i;
        SAMPLE_COMM_VO_UnBindVpss(VoDev,VoChn,VpssGrp,VpssChn_VoHD0);
    }
    SAMPLE_COMM_VO_StopDevLayer(VoDev);

END_8D1_4:
#if HICHIP == HI3521_V100
	VoDev = SAMPLE_VO_DEV_DSD1;
	VoChn = 0;
	enVoMode = VO_MODE_1MUX;
    SAMPLE_COMM_VO_UnBindVi(VoDev,VoChn); 
    SAMPLE_COMM_VO_StopChn(VoDev, enVoMode);
    SAMPLE_COMM_VO_StopDevLayer(VoDev);
#endif
END_8D1_3:	//vi unbind vpss
    SAMPLE_COMM_VI_UnBindVpss(enViMode);
END_8D1_2:	//vpss stop
    SAMPLE_COMM_VPSS_Stop(s32VpssGrpCnt, VPSS_MAX_CHN_NUM);
END_8D1_1:	//vi stop
    SAMPLE_COMM_VI_Stop(enViMode);
END_8D1_0:	//system exit
    SAMPLE_COMM_SYS_Exit();
    
    return s32Ret;
}


/******************************************************************************
* function :  VI:8*2cif; VO:HD0(HDMI  720P50), WBC to SD0(CVBS) video preview. 
******************************************************************************/
HI_S32 SAMPLE_VIO_8_2cif(HI_VOID)
{
    SAMPLE_VI_MODE_E enViMode = SAMPLE_VI_MODE_8_2Cif;
    HI_U32 u32ViChnCnt = 8;
    HI_S32 s32VpssGrpCnt = 8;
    
    VB_CONF_S stVbConf;
    VPSS_GRP VpssGrp;
    VPSS_GRP_ATTR_S stGrpAttr;

	VPSS_CHN VpssChn_VoHD0 = VPSS_PRE0_CHN;
	 
    VO_DEV VoDev;
    VO_CHN VoChn;
    VO_PUB_ATTR_S stVoPubAttr; 
    VO_PUB_ATTR_S stVoPubAttr_SD; 
    SAMPLE_VO_MODE_E enVoMode, enPreVoMode;
    VO_WBC_ATTR_S stWbcAttr;
	 
    HI_S32 i;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32BlkSize;
    HI_CHAR ch;
    SIZE_S stSize,stSizeTmp;
    HI_U32 u32WndNum;

    /******************************************
     step  1: init global  variable 
    ******************************************/
    gs_u32ViFrmRate = (VIDEO_ENCODING_MODE_PAL== gs_enNorm)?25:30;
    
    memset(&stVbConf,0,sizeof(VB_CONF_S));

    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm,\
                    PIC_D1, SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH);

    stVbConf.u32MaxPoolCnt = 128;

    /*video buffer*/
    stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt = u32ViChnCnt * 12;

	u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm,\
                PIC_2CIF, SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH);
    /* video buffer*/
    stVbConf.astCommPool[1].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[1].u32BlkCnt = u32ViChnCnt * 6;

    /******************************************
     step 2: mpp system init. 
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        goto END_8_2Cif_0;
    }


    /******************************************
     step 3: start vi dev & chn to capture
    ******************************************/
    s32Ret = SAMPLE_COMM_VI_Start(enViMode, gs_enNorm);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vi failed!\n");
        goto END_8_2Cif_0;
    }
    
    /******************************************
     step 4: start vpss and vi bind vpss
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(gs_enNorm, PIC_D1, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        goto END_8_2Cif_1;
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
        goto END_8_2Cif_1;
    }
    /*open pre-scale*/
	s32Ret = SAMPLE_COMM_SYS_GetPicSize(gs_enNorm, PIC_2CIF, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        goto END_8_2Cif_1;
    }
    for(i=0;i<s32VpssGrpCnt;i++)
    {   
		s32Ret = SAMPLE_COMM_EnableVpssPreScale(i,stSize);
		if(HI_SUCCESS != s32Ret)
		{
        SAMPLE_PRT("HI_MPI_VPSS_SetPreScale failed!\n");
        goto END_8_2Cif_1;
        }
    }

		
    s32Ret = SAMPLE_COMM_VI_BindVpss(enViMode);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Vi bind Vpss failed!\n");
        goto END_8_2Cif_2;
    }

    /******************************************
     step 5: start vo HD1 (VGA+HDMI) (DoubleFrame) (WBC source) 
    ******************************************/
	printf("start vo HD0.\n");
	VoDev = SAMPLE_VO_DEV_DHD0;
	u32WndNum = 8;
	enVoMode = VO_MODE_9MUX;

	if(VIDEO_ENCODING_MODE_PAL == gs_enNorm)
	{
		stVoPubAttr.enIntfSync = VO_OUTPUT_720P50;
	}
	else
	{
		stVoPubAttr.enIntfSync = VO_OUTPUT_720P60;
	}
#ifdef HI_FPGA
	stVoPubAttr.enIntfType = VO_INTF_HDMI|VO_INTF_VGA|VO_INTF_BT1120;
#else
    stVoPubAttr.enIntfType = VO_INTF_HDMI|VO_INTF_VGA;
#endif
	stVoPubAttr.u32BgColor = 0x000000ff;
	stVoPubAttr.bDoubleFrame = HI_TRUE;

	s32Ret = SAMPLE_COMM_VO_StartDevLayer(VoDev, &stVoPubAttr, gs_u32ViFrmRate);
	if (HI_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("Start SAMPLE_COMM_VO_StartDevLayer failed!\n");
		goto END_8_2Cif_3;
	}

	s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, &stVoPubAttr, enVoMode);
	if (HI_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("Start SAMPLE_COMM_VO_StartChn failed!\n");
		goto END_8_2Cif_4;
	}

	/* if it's displayed on HDMI, we should start HDMI */
	if (stVoPubAttr.enIntfType & VO_INTF_HDMI)
	{
		if (HI_SUCCESS != SAMPLE_COMM_VO_HdmiStart(stVoPubAttr.enIntfSync))
		{
			SAMPLE_PRT("Start SAMPLE_COMM_VO_HdmiStart failed!\n");
			goto END_8_2Cif_4;
		}
	}

	for(i=0;i<u32WndNum;i++)
	{
		VoChn = i;
		VpssGrp = i;
		
		s32Ret = SAMPLE_COMM_VO_BindVpss(VoDev,VoChn,VpssGrp,VpssChn_VoHD0);
		if (HI_SUCCESS != s32Ret)
		{
			SAMPLE_PRT("Start VO failed!\n");
			goto END_8_2Cif_4;
		}
	}

	/******************************************
	step 7: start vo SD0 (CVBS) (WBC target) 
	******************************************/
	printf("start vo SD0: wbc from hd0\n");
	VoDev = SAMPLE_VO_DEV_DSD0;
	u32WndNum = 1;
	enVoMode = VO_MODE_1MUX;

	stVoPubAttr_SD.enIntfSync = VO_OUTPUT_PAL;
	stVoPubAttr_SD.enIntfType = VO_INTF_CVBS;
	stVoPubAttr_SD.u32BgColor = 0x000000ff;
	stVoPubAttr_SD.bDoubleFrame = HI_FALSE;
	s32Ret = SAMPLE_COMM_VO_StartDevLayer(VoDev, &stVoPubAttr_SD, gs_u32ViFrmRate);
	if (HI_SUCCESS != s32Ret)
	{
		  SAMPLE_PRT("SAMPLE_COMM_VO_StartDevLayer failed!\n");
		  goto END_8_2Cif_4;
	}

	s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, &stVoPubAttr_SD, enVoMode);
	if (HI_SUCCESS != s32Ret)
	{
		  SAMPLE_PRT("SAMPLE_COMM_VO_StartChn failed!\n");
		  goto END_8_2Cif_5;
	}
	  
	s32Ret = SAMPLE_COMM_VO_GetWH(VO_OUTPUT_PAL, \
					  &stWbcAttr.stTargetSize.u32Width, \
					  &stWbcAttr.stTargetSize.u32Height, \
					  &stWbcAttr.u32FrameRate);
	if (HI_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("SAMPLE_COMM_VO_GetWH failed!\n");
		goto END_8_2Cif_5;
	}
	stWbcAttr.enPixelFormat = SAMPLE_PIXEL_FORMAT;
    stWbcAttr.enDataSource = VO_WBC_DATASOURCE_MIXER;

	s32Ret = HI_MPI_VO_SetWbcAttr(SAMPLE_VO_DEV_DHD0, &stWbcAttr);
	if (HI_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("HI_MPI_VO_SetWbcAttr failed!\n");
		goto END_8_2Cif_5;
	}

	s32Ret = HI_MPI_VO_EnableWbc(SAMPLE_VO_DEV_DHD0);
	if (HI_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("HI_MPI_VO_SetWbcAttr failed!\n");
		goto END_8_2Cif_6;
	}

	s32Ret = SAMPLE_COMM_VO_BindVoWbc(SAMPLE_VO_DEV_DHD0, SAMPLE_VO_DEV_DSD0, 0);
	if (HI_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("HI_MPI_VO_SetWbcAttr failed!\n");
		goto END_8_2Cif_6;
	}
   
    /******************************************
         step 9: HD0 switch mode 
    ******************************************/
    VoDev = SAMPLE_VO_DEV_DHD0;
    enVoMode = VO_MODE_9MUX;
    while(1)
    {
  		enPreVoMode = enVoMode;
        printf("please choose preview mode, press 'q' to exit this sample.\n"); 
        printf("\t0) 1 preview\n");
        printf("\t1) 4 preview\n");
        printf("\t2) 8 preview\n");        
        printf("\tq) quit\n");
 
        ch = getchar();
        getchar();
        if ('0' == ch)
        {
            u32WndNum = 1;
            enVoMode = VO_MODE_1MUX;
        }
        else if ('1' == ch)
        {
            u32WndNum = 4;
            enVoMode = VO_MODE_4MUX;
        }
        else if ('2' == ch)
        {
            u32WndNum = 8;
            enVoMode = VO_MODE_9MUX;
        }       
        else if ('q' == ch)
        {
            break;
        }
        else
        {
            SAMPLE_PRT("preview mode invaild! please try again.\n");
            continue;
        }
		/* VI Input size should change from 2cif to D1 */
		if(( VO_MODE_1MUX== enVoMode ||VO_MODE_4MUX== enVoMode)&&( VO_MODE_9MUX== enPreVoMode))
		{
		 	stSizeTmp.u32Width = 720;
			stSizeTmp.u32Height = ((VIDEO_ENCODING_MODE_PAL==gs_enNorm)?576:480);
            if(VO_MODE_1MUX == enVoMode)
        	{                
				s32Ret = SAMPLE_COMM_DisableVpssPreScale(0,stSizeTmp);
				if (HI_SUCCESS != s32Ret)
				{
					SAMPLE_PRT("SAMPLE_COMM_DisableVpssPreScale VO failed!\n");
					goto END_8_2Cif_6;
				}
        	}
			else
			{
				for(i=0;i<u32WndNum;i++)
			 	{
					s32Ret = SAMPLE_COMM_DisableVpssPreScale(i,stSizeTmp);
					if (HI_SUCCESS != s32Ret)
					{
						SAMPLE_PRT("SAMPLE_COMM_DisableVpssPreScale VO failed!\n");
						goto END_8_2Cif_6;
					}
			 	}
			}
		}
		/* VI Input size should change from  D1 to 2cif */
        else if(( VO_MODE_1MUX== enPreVoMode ||VO_MODE_4MUX== enPreVoMode)&&( VO_MODE_9MUX== enVoMode))
        {
        	stSizeTmp.u32Width= D1_WIDTH / 2;
			stSizeTmp.u32Height = ((VIDEO_ENCODING_MODE_PAL==gs_enNorm)?576:480);
	        if(VO_MODE_9MUX == enVoMode)
        	{
        		for(i=0;i<u32WndNum;i++)
			 	{
					s32Ret = SAMPLE_COMM_EnableVpssPreScale(i,stSizeTmp);
					if (HI_SUCCESS != s32Ret)
					{
						SAMPLE_PRT("SAMPLE_COMM_DisableVpssPreScale VO failed!\n");
						goto END_8_2Cif_6;
					}
        		}
        	}
			else
			{
				for(i=0;i<u32WndNum;i++)
			 	{
					s32Ret = SAMPLE_COMM_EnableVpssPreScale(i,stSizeTmp);
					if (HI_SUCCESS != s32Ret)
					{
						SAMPLE_PRT("SAMPLE_COMM_DisableVpssPreScale VO failed!\n");
						goto END_8_2Cif_6;
					}
			 	}
			}
        }
		else if (enVoMode== enPreVoMode)
		{
			continue;
		}        
		
        SAMPLE_PRT("vo(%d) switch to %d mode\n", VoDev, u32WndNum);

        s32Ret= HI_MPI_VO_SetAttrBegin(VoDev);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start VO failed!\n");
            goto END_8_2Cif_6;
        }
        
        s32Ret = SAMPLE_COMM_VO_StopChn(VoDev, enPreVoMode);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start VO failed!\n");
            goto END_8_2Cif_6;
        }

        s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, &stVoPubAttr, enVoMode);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start VO failed!\n");
            goto END_8_2Cif_6;
        }
        s32Ret= HI_MPI_VO_SetAttrEnd(VoDev);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start VO failed!\n");
            goto END_8_2Cif_6;
        }
    }

    /******************************************
     step 10: exit process
    ******************************************/

END_8_2Cif_6:
	SAMPLE_COMM_VO_UnBindVoWbc(SAMPLE_VO_DEV_DSD0, 0);
	HI_MPI_VO_DisableWbc(SAMPLE_VO_DEV_DHD0);

END_8_2Cif_5:
	VoDev = SAMPLE_VO_DEV_DSD0;
	VoChn = 0;
	enVoMode = VO_MODE_1MUX;
    SAMPLE_COMM_VO_StopChn(VoDev, enVoMode);
    SAMPLE_COMM_VO_StopDevLayer(VoDev);

END_8_2Cif_4:
	if (stVoPubAttr.enIntfType & VO_INTF_HDMI)
    {
        SAMPLE_COMM_VO_HdmiStop();
    }
    VoDev = SAMPLE_VO_DEV_DHD0;
    u32WndNum = 8;
    enVoMode = VO_MODE_9MUX;
    SAMPLE_COMM_VO_StopChn(VoDev, enVoMode);
    for(i=0;i<u32WndNum;i++)
    {
        VoChn = i;
        VpssGrp = i;
        SAMPLE_COMM_VO_UnBindVpss(VoDev,VoChn,VpssGrp,VpssChn_VoHD0);
    }    
    SAMPLE_COMM_VO_StopDevLayer(VoDev);

	
END_8_2Cif_3:
    SAMPLE_COMM_VI_UnBindVpss(enViMode);
END_8_2Cif_2:
    SAMPLE_COMM_VPSS_Stop(s32VpssGrpCnt, VPSS_MAX_CHN_NUM);
END_8_2Cif_1:
    SAMPLE_COMM_VI_Stop(enViMode);
END_8_2Cif_0:
    SAMPLE_COMM_SYS_Exit();
    
    return s32Ret;
}



/******************************************************************************
* function :  VI:1*1080p; VO:SD0(CVBS), HD0(HDMI,1080p@60 + VGA) 
******************************************************************************/
HI_S32 SAMPLE_VIO_1_1080P(HI_VOID)
{
    SAMPLE_VI_MODE_E enViMode = SAMPLE_VI_MODE_1_1080P;
    HI_U32 u32ViChnCnt = 1;
    HI_S32 s32VpssGrpCnt = 1;
    
    VB_CONF_S stVbConf;
    VPSS_GRP VpssGrp;
    VPSS_GRP_ATTR_S stGrpAttr;
    VO_DEV VoDev;
    VO_CHN VoChn;
	VI_CHN ViChn;
    VO_PUB_ATTR_S stVoPubAttr; 
    SAMPLE_VO_MODE_E enVoMode, enPreVoMode;
    
    HI_S32 i;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32BlkSize;
    HI_CHAR ch;
    SIZE_S stSize;
    HI_U32 u32WndNum;

    /******************************************
     step  1: init global  variable 
    ******************************************/
    gs_u32ViFrmRate = (VIDEO_ENCODING_MODE_PAL == gs_enNorm)?25:30;
    
    memset(&stVbConf,0,sizeof(VB_CONF_S));

    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm,\
                PIC_HD1080, SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH);
    stVbConf.u32MaxPoolCnt = 128;

    /*ddr0 video buffer*/
    stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt = u32ViChnCnt * 8;


    /******************************************
     step 2: mpp system init. 
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        goto END_1080P_0;
    }

    /******************************************
     step 3: start vi dev & chn to capture
    ******************************************/
    s32Ret = SAMPLE_COMM_VI_Start(enViMode, gs_enNorm);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vi failed!\n");
        goto END_1080P_0;
    }
    
    /******************************************
     step 4: start vpss and vi bind vpss (subchn needn't bind vpss in this mode)
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(gs_enNorm, PIC_HD1080, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        goto END_1080P_1;
    }

    stGrpAttr.u32MaxW = stSize.u32Width;
    stGrpAttr.u32MaxH = stSize.u32Height;
    stGrpAttr.bDrEn = HI_FALSE;
    stGrpAttr.bDbEn = HI_FALSE;
    stGrpAttr.bIeEn = HI_TRUE;
    stGrpAttr.bNrEn = HI_TRUE;
    stGrpAttr.bHistEn = HI_TRUE;
    stGrpAttr.enDieMode = VPSS_DIE_MODE_NODIE;
    stGrpAttr.enPixFmt = SAMPLE_PIXEL_FORMAT;

    s32Ret = SAMPLE_COMM_VPSS_Start(s32VpssGrpCnt, &stSize, VPSS_MAX_CHN_NUM,&stGrpAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start Vpss failed!\n");
        goto END_1080P_1;
    }

    s32Ret = SAMPLE_COMM_VI_BindVpss(enViMode);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Vi bind Vpss failed!\n");
        goto END_1080P_2;
    }

	/******************************************
	step 5: start VO SD0 (bind * vi )
	******************************************/
	printf("start vo sd0 bind vi chn0 \n");
	VoDev = SAMPLE_VO_DEV_DSD0;
	enVoMode = VO_MODE_1MUX;

	stVoPubAttr.enIntfSync = VO_OUTPUT_PAL;
	stVoPubAttr.enIntfType = VO_INTF_CVBS;
	stVoPubAttr.u32BgColor = 0x000000ff;
	stVoPubAttr.bDoubleFrame = HI_FALSE;//In HD, this item should be set to HI_FALSE
	s32Ret = SAMPLE_COMM_VO_StartDevLayer(VoDev, &stVoPubAttr, gs_u32ViFrmRate);
	if (HI_SUCCESS != s32Ret)
	{
	   SAMPLE_PRT("SAMPLE_COMM_VO_StartDevLayer failed!\n");
	   goto END_1080P_3;
	}

	s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, &stVoPubAttr, enVoMode);
	if (HI_SUCCESS != s32Ret)
	{
	   SAMPLE_PRT("SAMPLE_COMM_VO_StartChn failed!\n");
	   goto END_1080P_4;
	}


	VoChn = 0;
	ViChn = 0; //? is vi chn 0?
	s32Ret = SAMPLE_COMM_VO_BindVi(VoDev, VoChn, ViChn);
	if (HI_SUCCESS != s32Ret)
	{
	   SAMPLE_PRT("SAMPLE_COMM_VO_BindVi(vo:%d)-(vichn:%d) failed with %#x!\n", VoDev, VoChn, s32Ret);
	   goto END_1080P_4;
	}

	/******************************************
	 step 6: start vo HD0 (HDMI), muti-screen, you can switch mode
	******************************************/
	printf("start vo HD0.\n");
	VoDev = SAMPLE_VO_DEV_DHD0;
	u32WndNum = 4;
	enVoMode = VO_MODE_1MUX;

	if(VIDEO_ENCODING_MODE_PAL == gs_enNorm)
	{
		stVoPubAttr.enIntfSync = VO_OUTPUT_1080P50;
	}
	else
	{
		stVoPubAttr.enIntfSync = VO_OUTPUT_1080P60;
	}
#ifdef HI_FPGA
	stVoPubAttr.enIntfType = VO_INTF_HDMI|VO_INTF_VGA|VO_INTF_BT1120;
#else
	stVoPubAttr.enIntfType = VO_INTF_HDMI|VO_INTF_VGA;
#endif
	stVoPubAttr.u32BgColor = 0x000000ff;
	stVoPubAttr.bDoubleFrame = HI_FALSE;
	
	s32Ret = SAMPLE_COMM_VO_StartDevLayer(VoDev, &stVoPubAttr, gs_u32ViFrmRate);
	if (HI_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("Start SAMPLE_COMM_VO_StartDevLayer failed!\n");
		goto END_1080P_4;
	}
	
	s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, &stVoPubAttr, enVoMode);
	if (HI_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("Start SAMPLE_COMM_VO_StartChn failed!\n");
		goto END_1080P_5;
	}

	/* if it's displayed on HDMI, we should start HDMI */
	if (stVoPubAttr.enIntfType & VO_INTF_HDMI)
	{
		if (HI_SUCCESS != SAMPLE_COMM_VO_HdmiStart(stVoPubAttr.enIntfSync))
		{
			SAMPLE_PRT("Start SAMPLE_COMM_VO_HdmiStart failed!\n");
			goto END_1080P_5;
		}
	}
	
	for(i=0;i<u32WndNum;i++)
	{
		VoChn = i;
		VpssGrp = 0;
		
		s32Ret = SAMPLE_COMM_VO_BindVpss(VoDev,VoChn,VpssGrp,i);
		if (HI_SUCCESS != s32Ret)
		{
			SAMPLE_PRT("Start VO failed!\n");
			goto END_1080P_5;
		}
	}

    /******************************************
     step 7: HD0 switch mode 
    ******************************************/
	VoDev = SAMPLE_VO_DEV_DHD0;
    u32WndNum = 4;
    enVoMode = VO_MODE_1MUX;
    while(1)
    {
        enPreVoMode = enVoMode;
    
        printf("please choose preview mode, press 'q' to exit this sample.\n"); 
        printf("\t0) 1 preview\n");
        printf("\t1) 4 preview\n");
        printf("\tq) quit\n");
 
        ch = getchar();
        getchar();
        if ('0' == ch)
        {
            u32WndNum = 1;
            enVoMode = VO_MODE_1MUX;
        }
        else if ('1' == ch)
        {
            u32WndNum = 4;
            enVoMode = VO_MODE_4MUX;
        }

        else if ('q' == ch)
        {
            break;
        }
        else
        {
            SAMPLE_PRT("preview mode invaild! please try again.\n");
            continue;
        }
        SAMPLE_PRT("vo(%d) switch to %d mode\n", VoDev, u32WndNum);

        s32Ret= HI_MPI_VO_SetAttrBegin(VoDev);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start VO failed!\n");
            goto END_1080P_5;
        }
        
        s32Ret = SAMPLE_COMM_VO_StopChn(VoDev, enPreVoMode);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start VO failed!\n");
            goto END_1080P_5;
        }

        s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, &stVoPubAttr, enVoMode);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start VO failed!\n");
            goto END_1080P_5;
        }
        s32Ret= HI_MPI_VO_SetAttrEnd(VoDev);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start VO failed!\n");
            goto END_1080P_5;
        }
    }

    /******************************************
     step 8: exit process
    ******************************************/
   
END_1080P_5:	//stop vo hd0

    VoDev = SAMPLE_VO_DEV_DHD0;
    u32WndNum = 4;
    enVoMode = VO_MODE_4MUX;   
    if (stVoPubAttr.enIntfType & VO_INTF_HDMI)
    {
        SAMPLE_COMM_VO_HdmiStop();
    }
    SAMPLE_COMM_VO_StopChn(VoDev, enVoMode);
    for(i=0;i<u32WndNum;i++)
    {
        VoChn = i;
        VpssGrp = i;
        SAMPLE_COMM_VO_UnBindVpss(VoDev,VoChn,VpssGrp,VPSS_PRE0_CHN);
    }
    SAMPLE_COMM_VO_StopDevLayer(VoDev);
 
END_1080P_4:	//stop vo sd0
    VoDev = SAMPLE_VO_DEV_DSD0;  
    enVoMode = VO_MODE_1MUX;
    VoChn = 0;
    SAMPLE_COMM_VO_UnBindVi(VoDev,VoChn);
    SAMPLE_COMM_VO_StopChn(VoDev, enVoMode);
    SAMPLE_COMM_VO_StopDevLayer(VoDev);

END_1080P_3:
    SAMPLE_COMM_VI_UnBindVpss(enViMode);
END_1080P_2:
    SAMPLE_COMM_VPSS_Stop(s32VpssGrpCnt, VPSS_MAX_CHN_NUM);
END_1080P_1:
    SAMPLE_COMM_VI_Stop(enViMode);
END_1080P_0:
    SAMPLE_COMM_SYS_Exit();
    
    return s32Ret;
}


/******************************************************************************
* function :  VI:8*D1(6fps)/2cif(19fps); VO:HD0(HDMI  720P50)video preview. 
******************************************************************************/
HI_S32 SAMPLE_VIO_8_D1Cif(HI_VOID)
{
    SAMPLE_VI_MODE_E enViMode = SAMPLE_VI_MODE_8_D1Cif;
    HI_U32 u32ViChnCnt = 8;
    HI_S32 s32VpssGrpCnt = 8;
    
    VB_CONF_S stVbConf;
    VPSS_GRP VpssGrp;
    VPSS_GRP_ATTR_S stGrpAttr;
	VPSS_CHN VpssChn_VoHD0 = VPSS_PRE0_CHN;
	 
    VO_DEV VoDev;
    VO_CHN VoChn;
    VO_PUB_ATTR_S stVoPubAttr; 

    SAMPLE_VO_MODE_E enVoMode, enPreVoMode;
	 
    HI_S32 i;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32BlkSize;
    HI_CHAR ch;
    SIZE_S stSize,stSizeTmp;
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
    stVbConf.astCommPool[0].u32BlkCnt = u32ViChnCnt * 5;
	
    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm,\
                PIC_2CIF, SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH);
    stVbConf.astCommPool[1].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[1].u32BlkCnt = u32ViChnCnt * 10;

    /******************************************
     step 2: mpp system init. 
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        goto END_8_MixCap_0;
    }


    /******************************************
     step 3: start vi dev & chn to capture
    ******************************************/
    s32Ret = SAMPLE_COMM_VI_MixCap_Start(enViMode, gs_enNorm);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vi failed!\n");
        goto END_8_MixCap_0;
    }
    
    /******************************************
     step 4: start vpss and vi bind vpss
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(gs_enNorm, PIC_D1, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        goto END_8_MixCap_1;
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
        goto END_8_MixCap_1;
    }
    /*open pre-scale*/
	s32Ret = SAMPLE_COMM_SYS_GetPicSize(gs_enNorm, PIC_2CIF, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        goto END_8_MixCap_1;
    }
    for(i=0;i<s32VpssGrpCnt;i++)
    {   
		s32Ret = SAMPLE_COMM_EnableVpssPreScale(i,stSize);
		if(HI_SUCCESS != s32Ret)
		{
        SAMPLE_PRT("HI_MPI_VPSS_SetPreScale failed!\n");
        goto END_8_MixCap_1;
        }
    }
		
    s32Ret = SAMPLE_COMM_VI_BindVpss(enViMode);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Vi bind Vpss failed!\n");
        goto END_8_MixCap_2;
    }

    /******************************************
     step 5: start vo HD0(HDMI) 
    ******************************************/
	printf("start vo HD0.\n");
	VoDev = SAMPLE_VO_DEV_DHD0;
	u32WndNum = 8;
	enVoMode = VO_MODE_9MUX;

	if(VIDEO_ENCODING_MODE_PAL == gs_enNorm)
	{
		stVoPubAttr.enIntfSync = VO_OUTPUT_1080P50;
	}
	else
	{
		stVoPubAttr.enIntfSync = VO_OUTPUT_1080P60;
	}
#ifdef HI_FPGA
	stVoPubAttr.enIntfType = VO_INTF_HDMI|VO_INTF_VGA|VO_INTF_BT1120;
#else
	stVoPubAttr.enIntfType = VO_INTF_HDMI|VO_INTF_VGA;
#endif
	stVoPubAttr.u32BgColor = 0x000000ff;
	stVoPubAttr.bDoubleFrame = HI_TRUE;

	s32Ret = SAMPLE_COMM_VO_StartDevLayer(VoDev, &stVoPubAttr, gs_u32ViFrmRate);
	if (HI_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("Start SAMPLE_COMM_VO_StartDevLayer failed!\n");
		goto END_8_MixCap_3;
	}

	s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, &stVoPubAttr, enVoMode);
	if (HI_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("Start SAMPLE_COMM_VO_StartChn failed!\n");
		goto END_8_MixCap_4;
	}

	/* if it's displayed on HDMI, we should start HDMI */
	if (stVoPubAttr.enIntfType & VO_INTF_HDMI)
	{
		if (HI_SUCCESS != SAMPLE_COMM_VO_HdmiStart(stVoPubAttr.enIntfSync))
		{
			SAMPLE_PRT("Start SAMPLE_COMM_VO_HdmiStart failed!\n");
			goto END_8_MixCap_4;
		}
	}

	for(i=0;i<u32WndNum;i++)
	{
		VoChn = i;
		VpssGrp = i;
		
		s32Ret = SAMPLE_COMM_VO_BindVpss(VoDev,VoChn,VpssGrp,VpssChn_VoHD0);
		if (HI_SUCCESS != s32Ret)
		{
			SAMPLE_PRT("Start VO failed!\n");
			goto END_8_MixCap_4;
		}
	}
 
    /******************************************
         step 9: HD0 switch mode 
    ******************************************/
    VoDev = SAMPLE_VO_DEV_DHD0;
    enVoMode = VO_MODE_9MUX;
    while(1)
    {
  		enPreVoMode = enVoMode;
        printf("please choose preview mode, press 'q' to exit this sample.\n"); 
        printf("\t0) 1 preview\n");
        printf("\t1) 4 preview\n");
        printf("\t2) 9 preview\n");       
        printf("\tq) quit\n");
 
        ch = getchar();
        getchar();
        if ('0' == ch)
        {
            u32WndNum = 1;
            enVoMode = VO_MODE_1MUX;
        }
        else if ('1' == ch)
        {
            u32WndNum = 4;
            enVoMode = VO_MODE_4MUX;
        }
        else if ('2' == ch)
        {
            u32WndNum = 8;
            enVoMode = VO_MODE_9MUX;
        }       
        else if ('q' == ch)
        {
            break;
        }
        else
        {
            SAMPLE_PRT("preview mode invaild! please try again.\n");
            continue;
        }
		/* VI Chn size should change from D1/2cif to D1 */
		if((VO_MODE_1MUX== enVoMode) || (VO_MODE_4MUX== enVoMode))
		{
		 	stSizeTmp.u32Width = 720;
			stSizeTmp.u32Height = ((VIDEO_ENCODING_MODE_PAL==gs_enNorm)?576:480);
            if(VO_MODE_1MUX == enVoMode)
        	{
				s32Ret = SAMPLE_COMM_VI_ChangeMixCap(0,HI_FALSE,gs_u32ViFrmRate);
				if (HI_SUCCESS != s32Ret)
				{
					SAMPLE_PRT("SAMPLE_COMM_VI_ChangeCapSize VO failed!\n");
					goto END_8_MixCap_4;
				}

				s32Ret = SAMPLE_COMM_DisableVpssPreScale(0,stSizeTmp);
				if (HI_SUCCESS != s32Ret)
				{
					SAMPLE_PRT("SAMPLE_COMM_DisableVpssPreScale VO failed!\n");
					goto END_8_MixCap_4;
				}
        	}
			else
			{
				for(i=0;i<u32WndNum;i++)
			 	{
					s32Ret = SAMPLE_COMM_VI_ChangeMixCap(i,HI_FALSE,gs_u32ViFrmRate);
					if (HI_SUCCESS != s32Ret)
					{
						SAMPLE_PRT("SAMPLE_COMM_VI_ChangeCapSize VO failed!\n");
						goto END_8_MixCap_4;
					}

					s32Ret = SAMPLE_COMM_DisableVpssPreScale(i,stSizeTmp);
					if (HI_SUCCESS != s32Ret)
					{
						SAMPLE_PRT("SAMPLE_COMM_DisableVpssPreScale VO failed!\n");
						goto END_8_MixCap_4;
					}
			 	}
			}
		}
		/* VI Chn size should change from  D1 to D1/2cif */
        else if((VO_MODE_9MUX== enVoMode) || (VO_MODE_16MUX== enVoMode))
        {
        	stSizeTmp.u32Width= D1_WIDTH / 2;
			stSizeTmp.u32Height = ((VIDEO_ENCODING_MODE_PAL==gs_enNorm)?576:480);
	        if(VO_MODE_9MUX == enVoMode)
        	{
        		for(i=0;i<u32WndNum;i++)
			 	{
					s32Ret = SAMPLE_COMM_VI_ChangeMixCap(i,HI_TRUE,6);
					if (HI_SUCCESS != s32Ret)
					{
						SAMPLE_PRT("SAMPLE_COMM_VI_ChangeCapSize VO failed!\n");
						goto END_8_MixCap_4;
					}

					s32Ret = SAMPLE_COMM_EnableVpssPreScale(i,stSizeTmp);
					if (HI_SUCCESS != s32Ret)
					{
						SAMPLE_PRT("SAMPLE_COMM_DisableVpssPreScale VO failed!\n");
						goto END_8_MixCap_4;
					}
        		}
        	}
			else
			{
				for(i=0;i<u32WndNum;i++)
			 	{
					s32Ret = SAMPLE_COMM_VI_ChangeMixCap(i,HI_TRUE,6);
					if (HI_SUCCESS != s32Ret)
					{
						SAMPLE_PRT("SAMPLE_COMM_VI_ChangeCapSize VO failed!\n");
						goto END_8_MixCap_4;
					}

					s32Ret = SAMPLE_COMM_EnableVpssPreScale(i,stSizeTmp);
					if (HI_SUCCESS != s32Ret)
					{
						SAMPLE_PRT("SAMPLE_COMM_DisableVpssPreScale VO failed!\n");
						goto END_8_MixCap_4;
					}
			 	}
			}
        }
		else if (enVoMode== enPreVoMode)
		{
			continue;
		}
		
        SAMPLE_PRT("vo(%d) switch to %d mode\n", VoDev, u32WndNum);

        s32Ret= HI_MPI_VO_SetAttrBegin(VoDev);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start VO failed!\n");
            goto END_8_MixCap_4;
        }
        
        s32Ret = SAMPLE_COMM_VO_StopChn(VoDev, enPreVoMode);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start VO failed!\n");
            goto END_8_MixCap_4;
        }

        s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, &stVoPubAttr, enVoMode);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start VO failed!\n");
            goto END_8_MixCap_4;
        }
        s32Ret= HI_MPI_VO_SetAttrEnd(VoDev);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start VO failed!\n");
            goto END_8_MixCap_4;
        }
    }

    /******************************************
     step 10: exit process
    ******************************************/

END_8_MixCap_4:
	if (stVoPubAttr.enIntfType & VO_INTF_HDMI)
    {
        SAMPLE_COMM_VO_HdmiStop();
    }
    VoDev = SAMPLE_VO_DEV_DHD0;
    u32WndNum = 8;
    enVoMode = VO_MODE_9MUX;    
    SAMPLE_COMM_VO_StopChn(VoDev, enVoMode);
    for(i=0;i<u32WndNum;i++)
    {
        VoChn = i;
        VpssGrp = i;
        SAMPLE_COMM_VO_UnBindVpss(VoDev,VoChn,VpssGrp,VpssChn_VoHD0);
    }
    SAMPLE_COMM_VO_StopDevLayer(VoDev);

	
END_8_MixCap_3:
    SAMPLE_COMM_VI_UnBindVpss(enViMode);
END_8_MixCap_2:
    SAMPLE_COMM_VPSS_Stop(s32VpssGrpCnt, VPSS_MAX_CHN_NUM);
END_8_MixCap_1:
    SAMPLE_COMM_VI_Stop(enViMode);
END_8_MixCap_0:
    SAMPLE_COMM_SYS_Exit();
    
    return s32Ret;
}

/******************************************************************************
* function :  VI: 1*D1, user picture; VO: SD0(CVBS) video preview
******************************************************************************/
HI_S32 SAMPLE_VIO_1D1_LossDet(HI_VOID)
{
    SAMPLE_VI_MODE_E enViMode = SAMPLE_VI_MODE_1_D1;
    HI_U32 u32ViChnCnt = 8;
    
    VB_CONF_S stVbConf;
    VI_CHN ViChn;
    VO_DEV VoDev;
    VO_CHN VoChn;
    VO_PUB_ATTR_S stVoPubAttr; 
    SAMPLE_VO_MODE_E enVoMode;
    
    HI_S32 i;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32BlkSize;
    HI_U32 u32WndNum;

    VIDEO_FRAME_INFO_S stUserFrame;

    /******************************************
     step  1: init global  variable 
    ******************************************/
    gs_u32ViFrmRate = (VIDEO_ENCODING_MODE_PAL== gs_enNorm)?25:30;
    
    memset(&stVbConf,0,sizeof(VB_CONF_S));

    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm,\
                PIC_D1, SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH);
    stVbConf.u32MaxPoolCnt = 128;

    stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt = u32ViChnCnt * 6;


    /******************************************
     step 2: mpp system init. 
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        goto END_1D1_LOSS_0;
    }

    /******************************************
     step 3: start vi dev & chn to capture
    ******************************************/
    s32Ret = SAMPLE_COMM_VI_Start(enViMode, gs_enNorm);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vi failed!\n");
        goto END_1D1_LOSS_0;
    }
    
    /******************************************
     step 4: start VO to preview
    ******************************************/
    VoDev = SAMPLE_VO_DEV_DSD0;
    u32WndNum = 1;
    enVoMode = VO_MODE_1MUX;
    
    stVoPubAttr.enIntfSync = VO_OUTPUT_PAL;
    stVoPubAttr.enIntfType = VO_INTF_CVBS;
    stVoPubAttr.u32BgColor = 0x000000ff;
    stVoPubAttr.bDoubleFrame = HI_FALSE;

    s32Ret = SAMPLE_COMM_VO_StartDevLayer(VoDev, &stVoPubAttr, gs_u32ViFrmRate);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartDevLayer failed!\n");
        goto END_1D1_LOSS_1;
    }
    
    s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, &stVoPubAttr, enVoMode);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartChn failed!\n");
        goto END_1D1_LOSS_2;
    }

    for(i=0;i<u32WndNum;i++)
    {
        VoChn = i;
        ViChn = i;
        s32Ret = SAMPLE_COMM_VO_BindVi(VoDev,VoChn, ViChn);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("SAMPLE_COMM_VO_BindVpss failed!\n");
            goto END_1D1_LOSS_2;
        }
    }

    /******************************************
     step 5: set novideo pic
    ******************************************/
    /* set novideo pic */
    s32Ret = SAMPLE_VI_SetUserPic("pic_704_576_p420_novideo01.yuv", 704, 576, 704, &stUserFrame);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VI_SetUserPic failed!\n");
        goto END_1D1_LOSS_2;
    }
    
    /******************************************
     step 6: start video loss detect, if loss, then use user-pic
    ******************************************/
    s32Ret = SAMPLE_VI_StartVLossDet(enViMode);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_VI_StartVLossDet failed!\n");
        goto END_1D1_LOSS_2;
    }

    printf("\npress twice Enter to stop sample ... ... \n");
    getchar();
    getchar();
    
    /******************************************
     step 8: exit process
    ******************************************/
    SAMPLE_VI_StopVLossDet();
    
END_1D1_LOSS_2:
    for(i=0;i<u32WndNum;i++)
    {
        VoChn = i;
        SAMPLE_COMM_VO_UnBindVi(VoDev,VoChn);
    }
    SAMPLE_COMM_VO_StopChn(VoDev, enVoMode);
    SAMPLE_COMM_VO_StopDevLayer(VoDev);
END_1D1_LOSS_1:
    SAMPLE_COMM_VI_Stop(enViMode);
END_1D1_LOSS_0:
    SAMPLE_COMM_SYS_Exit();
    
    return s32Ret;
}

/******************************************************************************
* function :  VI: 1*D1; VO:HD0(HDMI,1080p@60) HD Zoom preview. 
*                vi ---> vpss grp 0       ---> vo HD0, chn0 (D1, PIP, HIDE)
*                   ---> vpss grp 1(clip) ---> vo HD0, chn1 (1080p)
******************************************************************************/
HI_S32 SAMPLE_VIO_1D1_HDZoom()
{
    SAMPLE_VI_MODE_E enViMode = SAMPLE_VI_MODE_1_D1;
    HI_U32 u32ViChnCnt = 1;
    HI_S32 s32VpssGrpCnt = 1;
    VPSS_GRP VpssGrp = 0;
    VPSS_GRP VpssGrp_Clip = 1;
    VO_DEV VoDev = SAMPLE_VO_DEV_DHD0;
    VO_CHN VoChn = 0;
    VO_CHN VoChn_Clip = 1;
    
    VB_CONF_S stVbConf;
    VPSS_GRP_ATTR_S stGrpAttr;
    VO_PUB_ATTR_S stVoPubAttr; 
    SAMPLE_VO_MODE_E enVoMode;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;
    VPSS_CROP_INFO_S stVpssCropInfo;
    VO_VIDEO_LAYER_ATTR_S stPipLayerAttr;
    VO_CHN_ATTR_S stChnAttr;
    SIZE_S stSize;
    
    HI_S32 i;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32BlkSize;
    HI_CHAR ch;
    HI_U32 u32WndNum;


    /******************************************
     step  1: init global  variable 
    ******************************************/
    gs_u32ViFrmRate = (VIDEO_ENCODING_MODE_PAL == gs_enNorm)?25:30;
    
    memset(&stVbConf,0,sizeof(VB_CONF_S));

    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm,\
                PIC_D1, SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH);
    stVbConf.u32MaxPoolCnt = 128;
    /*ddr0 video buffer*/
    stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt = u32ViChnCnt * 6;

    /******************************************
     step 2: mpp system init. 
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        goto END_1D1_CLIP_0;
    }

   
    /******************************************
     step 3: start vi dev & chn to capture
    ******************************************/
    s32Ret = SAMPLE_COMM_VI_Start(enViMode, gs_enNorm);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vi failed!\n");
        goto END_1D1_CLIP_0;
    }

    /******************************************
     step 4: start vpss and vi bind vpss
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(gs_enNorm, PIC_D1, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        goto END_1D1_CLIP_0;
    }

    stGrpAttr.u32MaxW = stSize.u32Width;
    stGrpAttr.u32MaxH = stSize.u32Height;
    stGrpAttr.bDrEn = HI_FALSE;
    stGrpAttr.bDbEn = HI_FALSE;
    stGrpAttr.bIeEn = HI_TRUE;
    stGrpAttr.bNrEn = HI_TRUE;
    stGrpAttr.bHistEn = HI_TRUE;
    stGrpAttr.enDieMode = VPSS_DIE_MODE_AUTO;
    stGrpAttr.enPixFmt = SAMPLE_PIXEL_FORMAT;

    // start 2 vpss group. group-1 for clip use.
    s32Ret = SAMPLE_COMM_VPSS_Start(s32VpssGrpCnt+1, &stSize, VPSS_MAX_CHN_NUM,NULL);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start Vpss failed!\n");
        goto END_1D1_CLIP_1;
    }

    // bind vi to vpss group 0
    s32Ret = SAMPLE_COMM_VI_BindVpss(enViMode);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Vi bind Vpss failed!\n");
        goto END_1D1_CLIP_2;
    }

    // bind vi(0,0) to vpss group 1, for clip use.
    stSrcChn.enModId = HI_ID_VIU;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = 0;

    stDestChn.enModId = HI_ID_VPSS;
    stDestChn.s32DevId = VpssGrp_Clip; // vpss group for clip
    stDestChn.s32ChnId = 0;

    s32Ret = HI_MPI_SYS_Bind(&stSrcChn, &stDestChn);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("bind vi(0,0) to vpss group 1 failed with %#x!\n", s32Ret);
        goto END_1D1_CLIP_2;
    }
    
    /******************************************
     step 5: start VO to preview
    ******************************************/
    printf("start vo hd0\n");
    u32WndNum = 1;
    enVoMode = VO_MODE_1MUX;
    
    if(VIDEO_ENCODING_MODE_PAL == gs_enNorm)
    {
        stVoPubAttr.enIntfSync = VO_OUTPUT_1080P50;
    }
    else
    {
        stVoPubAttr.enIntfSync = VO_OUTPUT_1080P60;
    }
    
    stVoPubAttr.enIntfType = VO_INTF_VGA|VO_INTF_HDMI;
    stVoPubAttr.u32BgColor = 0x000000ff;
    stVoPubAttr.bDoubleFrame = HI_TRUE;

    s32Ret = SAMPLE_COMM_VO_StartDevLayer(VoDev, &stVoPubAttr, gs_u32ViFrmRate);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartDevLayer failed!\n");
        goto END_1D1_CLIP_3;
    }
    
    s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, &stVoPubAttr, enVoMode);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartChn failed!\n");
        goto END_1D1_CLIP_4;
    }
    /* if it's displayed on HDMI, we should start HDMI */
    if (stVoPubAttr.enIntfType & VO_INTF_HDMI)
    {
        if (HI_SUCCESS != SAMPLE_COMM_VO_HdmiStart(stVoPubAttr.enIntfSync))
        {
            SAMPLE_PRT("Start SAMPLE_COMM_VO_HdmiStart failed!\n");
            goto END_1D1_CLIP_4;
        }
    }
    s32Ret = SAMPLE_COMM_VO_BindVpss(VoDev,VoChn,VpssGrp,VPSS_PRE0_CHN);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_BindVpss failed!\n");
        goto END_1D1_CLIP_4;
    }
    
    /******************************************
     step 6: Clip process
    ******************************************/
    printf("press any key to show hd zoom.\n");
    getchar();

    /*** enable vpss group clip ***/
    stVpssCropInfo.bEnable = HI_TRUE;
    stVpssCropInfo.enCropCoordinate = VPSS_CROP_ABS_COOR;
    stVpssCropInfo.stCropRect.s32X = 0;
    stVpssCropInfo.stCropRect.s32Y = 0;
    stVpssCropInfo.stCropRect.u32Height = 400;
    stVpssCropInfo.stCropRect.u32Width = 400; 
    stVpssCropInfo.enCapSel = VPSS_CAPSEL_BOTH;

    s32Ret = HI_MPI_VPSS_SetCropCfg(VpssGrp_Clip, &stVpssCropInfo);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartChn failed with %#x!\n", s32Ret);
        goto END_1D1_CLIP_4;
    }
    
    /*** enable vo pip layer ***/
    s32Ret = HI_MPI_VO_PipLayerBindDev(VoDev);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VO_PipLayerBindDev failed with %#x!\n", s32Ret);
        goto END_1D1_CLIP_4;
    }

    s32Ret = SAMPLE_COMM_VO_GetWH(stVoPubAttr.enIntfSync, \
    	               &stPipLayerAttr.stDispRect.u32Width, \
    	               &stPipLayerAttr.stDispRect.u32Height, \
    	               &stPipLayerAttr.u32DispFrmRt);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        goto END_1D1_CLIP_5;
    }
    
    stPipLayerAttr.stDispRect.s32X = 0;
    stPipLayerAttr.stDispRect.s32Y = 0;
    stPipLayerAttr.stImageSize.u32Height = stPipLayerAttr.stDispRect.u32Height;
    stPipLayerAttr.stImageSize.u32Width = stPipLayerAttr.stDispRect.u32Width;
    stPipLayerAttr.enPixFormat = SAMPLE_PIXEL_FORMAT;
    s32Ret = HI_MPI_VO_SetPipLayerAttr(&stPipLayerAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VO_SetPipLayerAttr failed with %#x!\n", s32Ret);
        goto END_1D1_CLIP_6;
    }
    
    s32Ret = HI_MPI_VO_EnablePipLayer();
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VO_EnablePipLayer failed with %#x!\n", s32Ret);
        goto END_1D1_CLIP_6;
    }   
   
    /*** vo chn 0: normal ->pip & 1080p -> D1 ***/
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(gs_enNorm, PIC_D1, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        goto END_1D1_CLIP_4;
    }
    stChnAttr.stRect.u32Width = ALIGN_BACK(stSize.u32Width, 4);
    stChnAttr.stRect.u32Height= ALIGN_BACK(stSize.u32Height, 4);
    stChnAttr.stRect.s32X       = ALIGN_BACK(1920-stSize.u32Width, 4);//
    stChnAttr.stRect.s32Y       = ALIGN_BACK(1080-stSize.u32Height, 4);//
    stChnAttr.u32Priority        = 1;
    stChnAttr.bDeflicker         = HI_FALSE;

    s32Ret = HI_MPI_VO_SetChnAttr(VoDev, VoChn, &stChnAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VO_SetChnAttr failed with %#x!\n", s32Ret);
        goto END_1D1_CLIP_4;
    }
    
    /***start new vo Chn(0) for clip.  ***/
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(gs_enNorm, PIC_HD1080, &stSize);//
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        goto END_1D1_CLIP_4;
    }
    
    stChnAttr.stRect.u32Width = ALIGN_BACK(stSize.u32Width, 4);
    stChnAttr.stRect.u32Height= ALIGN_BACK(stSize.u32Height, 4);
    stChnAttr.stRect.s32X       = ALIGN_BACK(0, 4);
    stChnAttr.stRect.s32Y       = ALIGN_BACK(0, 4);
    stChnAttr.u32Priority        = 0;
    stChnAttr.bDeflicker         = HI_FALSE;
    s32Ret = HI_MPI_VO_SetChnAttr(VoDev, VoChn_Clip, &stChnAttr);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n",  s32Ret);
        goto END_1D1_CLIP_4;
    }
    s32Ret = HI_MPI_VO_EnableChn(VoDev, VoChn_Clip);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        goto END_1D1_CLIP_4;
    }
    s32Ret = SAMPLE_COMM_VO_BindVpss(VoDev, VoChn_Clip, VpssGrp_Clip, VPSS_PRE0_CHN);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_BindVpss failed!\n");
        goto END_1D1_CLIP_4;
    }
    
    printf("\npress 'q' to stop sample ... ... \n");
    while('q' != (ch = getchar()) )  {}
    
    /******************************************
     step 7: exit process
    ******************************************/
END_1D1_CLIP_6:
    HI_MPI_VO_DisableChn(VoDev, VoChn);
    HI_MPI_VO_DisablePipLayer();
END_1D1_CLIP_5:
    HI_MPI_VO_PipLayerUnBindDev(VoDev);
END_1D1_CLIP_4:    
    if (stVoPubAttr.enIntfType & VO_INTF_HDMI)
    {
        SAMPLE_COMM_VO_HdmiStop();
    }
    SAMPLE_COMM_VO_StopChn(VoDev, enVoMode);
    HI_MPI_VO_DisableChn(VoDev, VoChn_Clip);
    for(i=0;i<u32WndNum;i++)
    {
        VoChn = i;
        VpssGrp = i;
        SAMPLE_COMM_VO_UnBindVpss(VoDev, VoChn, VpssGrp, VPSS_PRE0_CHN);
    }
    SAMPLE_COMM_VO_UnBindVpss(VoDev, VoChn_Clip, VpssGrp_Clip, VPSS_PRE0_CHN);
    SAMPLE_COMM_VO_StopDevLayer(VoDev);
END_1D1_CLIP_3:
    SAMPLE_COMM_VI_UnBindVpss(enViMode);
    
    stSrcChn.enModId = HI_ID_VIU;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = 0;
    stDestChn.enModId = HI_ID_VPSS;
    stDestChn.s32DevId = VpssGrp_Clip; // vpss group 1
    stDestChn.s32ChnId = 0;
    HI_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
END_1D1_CLIP_2:
    SAMPLE_COMM_VPSS_Stop(s32VpssGrpCnt+1, VPSS_MAX_CHN_NUM);
END_1D1_CLIP_1:
    SAMPLE_COMM_VI_Stop(enViMode);
END_1D1_CLIP_0:
    SAMPLE_COMM_SYS_Exit();
    
    return s32Ret;
}

/******************************************************************************
* function :  VI: 1*D1; VO:SD0(CVBS) SD Zoom preview.
******************************************************************************/
HI_S32 SAMPLE_VIO_1D1_SDZoom(HI_VOID)
{
    SAMPLE_VI_MODE_E enViMode = SAMPLE_VI_MODE_1_D1;
    HI_U32 u32ViChnCnt = 1;
 
    VB_CONF_S stVbConf;
    VI_CHN ViChn;
    VO_DEV VoDev;
    VO_CHN VoChn, VoChn_Clip = 1;
    VO_CHN_ATTR_S stChnAttr;
    VO_PUB_ATTR_S stVoPubAttr; 
    SAMPLE_VO_MODE_E enVoMode;
    
    HI_S32 i;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32BlkSize;
    SIZE_S stSize;
    HI_U32 u32WndNum;

    VO_ZOOM_ATTR_S stZoomWindow, stZoomWindowGet;


    /******************************************
     step  1: init global  variable 
    ******************************************/
    gs_u32ViFrmRate = (VIDEO_ENCODING_MODE_PAL== gs_enNorm)?25:30;
    
    memset(&stVbConf,0,sizeof(VB_CONF_S));

    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm,\
                PIC_D1, SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH);
    stVbConf.u32MaxPoolCnt = 128;
    /*ddr0 video buffer*/
    stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt = u32ViChnCnt * 6;


    /******************************************
     step 2: mpp system init. 
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        goto END_1D1_ZOOM_0;
    }

    /******************************************
     step 3: start vi dev & chn to capture
    ******************************************/
    s32Ret = SAMPLE_COMM_VI_Start(enViMode, gs_enNorm);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vi failed!\n");
        goto END_1D1_ZOOM_0;
    }

    /******************************************
     step 4: start VO to preview
    ******************************************/
    VoDev = SAMPLE_VO_DEV_DSD0;
    u32WndNum = 1;
    enVoMode = VO_MODE_1MUX;
    
    stVoPubAttr.enIntfSync = VO_OUTPUT_PAL;
    stVoPubAttr.enIntfType = VO_INTF_CVBS;
    stVoPubAttr.u32BgColor = 0x000000ff;
    stVoPubAttr.bDoubleFrame = HI_FALSE;
    s32Ret = SAMPLE_COMM_VO_StartDevLayer(VoDev, &stVoPubAttr, gs_u32ViFrmRate);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartDevLayer failed!\n");
        goto END_1D1_ZOOM_1;
    }
    
    s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, &stVoPubAttr, enVoMode);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartChn failed!\n");
        goto END_1D1_ZOOM_2;
    }

    for(i=0;i<u32WndNum;i++)
    {
        VoChn = i;
        ViChn = i;
        s32Ret = SAMPLE_COMM_VO_BindVi(VoDev, VoChn, ViChn);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("SAMPLE_COMM_VO_BindVpss failed!\n");
            goto END_1D1_ZOOM_2;
        }
    }

    /******************************************
     step 5: start new vo Chn for zoom
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(gs_enNorm, PIC_CIF, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        goto END_1D1_ZOOM_2;
    }
    stChnAttr.stRect.u32Width = ALIGN_BACK(stSize.u32Width, 4);
    stChnAttr.stRect.u32Height= ALIGN_BACK(stSize.u32Height, 4);
    stChnAttr.stRect.s32X       = ALIGN_BACK(0, 4);
    stChnAttr.stRect.s32Y       = ALIGN_BACK(0, 4);
    stChnAttr.u32Priority        = 1;
    stChnAttr.bDeflicker         = HI_FALSE;

    s32Ret = HI_MPI_VO_SetChnAttr(VoDev, VoChn_Clip, &stChnAttr);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n",  s32Ret);
        goto END_1D1_ZOOM_2;
    }
    s32Ret = HI_MPI_VO_EnableChn(VoDev, VoChn_Clip);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        goto END_1D1_ZOOM_2;
    }

    s32Ret = SAMPLE_COMM_VO_BindVi(VoDev,VoChn_Clip,0);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_BindVpss failed!\n");
        goto END_1D1_ZOOM_2;
    }

    /******************************************
     step 6: ZOOM process
    ******************************************/
    printf("print any key to zoom mode.\n");
    getchar();

    VoChn = 0;
    stZoomWindow.enZoomType = VOU_ZOOM_IN_RECT;
    stZoomWindow.stZoomRect.s32X = 128;
    stZoomWindow.stZoomRect.s32Y = 128;
    stZoomWindow.stZoomRect.u32Width = 200;
    stZoomWindow.stZoomRect.u32Height = 200;
    
    
    /* set zoom window */
    s32Ret = HI_MPI_VO_SetZoomInWindow(VoDev, VoChn, &stZoomWindow);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("Set zoom attribute failed with %#x!\n", s32Ret);
        goto END_1D1_ZOOM_2;
    }
    
    /* get current zoom window parameter */
    s32Ret = HI_MPI_VO_GetZoomInWindow(VoDev, VoChn, &stZoomWindowGet);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("Get zoom attribute failed with %#x!\n", s32Ret);
        goto END_1D1_ZOOM_2;
    }
    printf("Current zoom window is (%d,%d,%d,%d)!\n",
                stZoomWindowGet.stZoomRect.s32X,
                stZoomWindowGet.stZoomRect.s32Y, 
                stZoomWindowGet.stZoomRect.u32Width,
                stZoomWindowGet.stZoomRect.u32Height);

    printf("\npress 'q' to stop sample ... ... \n");
    while('q' != getchar())  {}
    
    /******************************************
     step 7: exit process
    ******************************************/
END_1D1_ZOOM_2:
    for(i=0;i<u32WndNum;i++)
    {
        VoChn = i;
        ViChn= i;
        SAMPLE_COMM_VO_UnBindVi(VoDev, VoChn);
    }
    SAMPLE_COMM_VO_UnBindVi(VoDev, VoChn_Clip);
    SAMPLE_COMM_VO_StopChn(VoDev, enVoMode);
    HI_MPI_VO_DisableChn(VoDev, VoChn_Clip);
    SAMPLE_COMM_VO_StopDevLayer(VoDev);
END_1D1_ZOOM_1:
    SAMPLE_COMM_VI_Stop(enViMode);
END_1D1_ZOOM_0:
    SAMPLE_COMM_SYS_Exit();
    
    return s32Ret;
}



/******************************************************************************
* function :  VI:4*720P;  HD0(HDMI,1080p@50 + VGA) 
******************************************************************************/
HI_S32 SAMPLE_VIO_4_720P(HI_VOID)
{
    SAMPLE_VI_MODE_E enViMode = SAMPLE_VI_MODE_4_720P;
    HI_U32 u32ViChnCnt = 4;
    HI_S32 s32VpssGrpCnt = 4;
    
    VB_CONF_S stVbConf;
    VPSS_GRP VpssGrp;
    VPSS_GRP_ATTR_S stGrpAttr;
    VO_DEV VoDev;
    VO_CHN VoChn;
	VI_CHN ViChn;
    VO_PUB_ATTR_S stVoPubAttr; 
    SAMPLE_VO_MODE_E enVoMode, enPreVoMode;
    
    HI_S32 i;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32BlkSize;
    HI_CHAR ch;
    SIZE_S stSize;
    HI_U32 u32WndNum;

    /******************************************
     step  1: init global  variable 
    ******************************************/
    gs_u32ViFrmRate = (VIDEO_ENCODING_MODE_PAL == gs_enNorm)?25:30;
    
    memset(&stVbConf,0,sizeof(VB_CONF_S));

    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm,\
                PIC_HD720, SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH);
    stVbConf.u32MaxPoolCnt = 128;

    /*ddr0 video buffer*/
    stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt = u32ViChnCnt * 8;


    /******************************************
     step 2: mpp system init. 
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        goto END_720P_0;
    }

    /******************************************
     step 3: start vi dev & chn to capture
    ******************************************/
    s32Ret = SAMPLE_COMM_VI_Start(enViMode, gs_enNorm);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vi failed!\n");
        goto END_720P_0;
    }
    
    /******************************************
     step 4: start vpss and vi bind vpss (subchn needn't bind vpss in this mode)
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(gs_enNorm, PIC_HD720, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        goto END_720P_1;
    }

    stGrpAttr.u32MaxW = stSize.u32Width;
    stGrpAttr.u32MaxH = stSize.u32Height;
    stGrpAttr.bDrEn = HI_FALSE;
    stGrpAttr.bDbEn = HI_FALSE;
    stGrpAttr.bIeEn = HI_TRUE;
    stGrpAttr.bNrEn = HI_TRUE;
    stGrpAttr.bHistEn = HI_TRUE;
    stGrpAttr.enDieMode = VPSS_DIE_MODE_NODIE;
    stGrpAttr.enPixFmt = SAMPLE_PIXEL_FORMAT;

    s32Ret = SAMPLE_COMM_VPSS_Start(s32VpssGrpCnt, &stSize, VPSS_MAX_CHN_NUM,&stGrpAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start Vpss failed!\n");
        goto END_720P_1;
    }

    s32Ret = SAMPLE_COMM_VI_BindVpss(enViMode);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Vi bind Vpss failed!\n");
        goto END_720P_2;
    }

	/******************************************
	 step 5: start vo HD0 (HDMI), muti-screen, you can switch mode
	******************************************/
	printf("start vo HD0.\n");
	VoDev = SAMPLE_VO_DEV_DHD0;
	u32WndNum = 4;
	enVoMode = VO_MODE_1MUX;

	if(VIDEO_ENCODING_MODE_PAL == gs_enNorm)
	{
		stVoPubAttr.enIntfSync = VO_OUTPUT_1080P50;
	}
	else
	{
		stVoPubAttr.enIntfSync = VO_OUTPUT_1080P60;
	}
#ifdef HI_FPGA
	stVoPubAttr.enIntfType = VO_INTF_HDMI|VO_INTF_VGA|VO_INTF_BT1120;
#else
	stVoPubAttr.enIntfType = VO_INTF_HDMI|VO_INTF_VGA;
#endif
	stVoPubAttr.u32BgColor = 0x000000ff;
	stVoPubAttr.bDoubleFrame = HI_FALSE;
	
	s32Ret = SAMPLE_COMM_VO_StartDevLayer(VoDev, &stVoPubAttr, gs_u32ViFrmRate);
	if (HI_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("Start SAMPLE_COMM_VO_StartDevLayer failed!\n");
		goto END_720P_3;
	}
	
	s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, &stVoPubAttr, enVoMode);
	if (HI_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("Start SAMPLE_COMM_VO_StartChn failed!\n");
		goto END_720P_4;
	}

	/* if it's displayed on HDMI, we should start HDMI */
	if (stVoPubAttr.enIntfType & VO_INTF_HDMI)
	{
		if (HI_SUCCESS != SAMPLE_COMM_VO_HdmiStart(stVoPubAttr.enIntfSync))
		{
			SAMPLE_PRT("Start SAMPLE_COMM_VO_HdmiStart failed!\n");
			goto END_720P_4;
		}
	}
	
	for(i=0;i<u32WndNum;i++)
	{
		VoChn = i;
		VpssGrp = i;
		
		s32Ret = SAMPLE_COMM_VO_BindVpss(VoDev,VoChn,VpssGrp,i);
		if (HI_SUCCESS != s32Ret)
		{
			SAMPLE_PRT("Start VO failed!\n");
			goto END_720P_4;
		}
	}

    /******************************************
     step 6: HD0 switch mode 
    ******************************************/
	VoDev = SAMPLE_VO_DEV_DHD0;
    u32WndNum = 4;
    enVoMode = VO_MODE_1MUX;
    while(1)
    {
        enPreVoMode = enVoMode;
    
        printf("please choose preview mode, press 'q' to exit this sample.\n"); 
        printf("\t0) 1 preview\n");
        printf("\t1) 4 preview\n");
        printf("\tq) quit\n");
 
        ch = getchar();
        getchar();
        if ('0' == ch)
        {
            u32WndNum = 1;
            enVoMode = VO_MODE_1MUX;
        }
        else if ('1' == ch)
        {
            u32WndNum = 4;
            enVoMode = VO_MODE_4MUX;
        }

        else if ('q' == ch)
        {
            break;
        }
        else
        {
            SAMPLE_PRT("preview mode invaild! please try again.\n");
            continue;
        }
        SAMPLE_PRT("vo(%d) switch to %d mode\n", VoDev, u32WndNum);

        s32Ret= HI_MPI_VO_SetAttrBegin(VoDev);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start VO failed!\n");
            goto END_720P_4;
        }
        
        s32Ret = SAMPLE_COMM_VO_StopChn(VoDev, enPreVoMode);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start VO failed!\n");
            goto END_720P_4;
        }

        s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, &stVoPubAttr, enVoMode);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start VO failed!\n");
            goto END_720P_4;
        }
        s32Ret= HI_MPI_VO_SetAttrEnd(VoDev);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start VO failed!\n");
            goto END_720P_4;
        }
    }

    /******************************************
     step 7: exit process
    ******************************************/
   
END_720P_4:	//stop vo hd0

    VoDev = SAMPLE_VO_DEV_DHD0;
    u32WndNum = 4;
    enVoMode = VO_MODE_4MUX;   
    if (stVoPubAttr.enIntfType & VO_INTF_HDMI)
    {
        SAMPLE_COMM_VO_HdmiStop();
    }
    SAMPLE_COMM_VO_StopChn(VoDev, enVoMode);
    for(i=0;i<u32WndNum;i++)
    {
        VoChn = i;
        VpssGrp = i;
        SAMPLE_COMM_VO_UnBindVpss(VoDev,VoChn,VpssGrp,VPSS_PRE0_CHN);
    }
    SAMPLE_COMM_VO_StopDevLayer(VoDev);
END_720P_3:
    SAMPLE_COMM_VI_UnBindVpss(enViMode);
END_720P_2:
    SAMPLE_COMM_VPSS_Stop(s32VpssGrpCnt, VPSS_MAX_CHN_NUM);
END_720P_1:
    SAMPLE_COMM_VI_Stop(enViMode);
END_720P_0:
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
        SAMPLE_VIO_Usage(argv[0]);
        return HI_FAILURE;
    }

    signal(SIGINT, SAMPLE_VIO_HandleSig);
    signal(SIGTERM, SAMPLE_VIO_HandleSig);

    switch (*argv[1])
    {
        case '0':/* VI:8*D1; VO:HD0(HDMI,VGA)+SD0(CVBS)+SD1 video preview. */
            s32Ret = SAMPLE_VIO_8_D1();
            break;
        case '1':/* VI:1*1080P; VO:HD0(HDMI ),  video preview */
            s32Ret = SAMPLE_VIO_1_1080P();
            break;
        case '2':/* VI:8*2cif; VO: HD0(HDMI,1080p@60 ), WBC to SD0(CVBS) video preview. */
            s32Ret = SAMPLE_VIO_8_2cif();
            break;
	    case '3':/* VI:8*D1/2Cif MixCap;  HD0(HDMI,1080p@60 ) video preview. */
            s32Ret = SAMPLE_VIO_8_D1Cif();
            break;
        case '4':/* VI: 1*D1, user picture; VO: SD0(CVBS) video preview. */
            s32Ret = SAMPLE_VIO_1D1_LossDet();
            break;
        case '5':/* VI: 1*D1; VO:HD0(HDMI) HD Zoom preview.  */
            s32Ret = SAMPLE_VIO_1D1_HDZoom();
            break;
        case '6':/* VI: 1*D1; VO:SD0(CVBS) SD Zoom preview. */
            s32Ret = SAMPLE_VIO_1D1_SDZoom();
            break;
		case '7':/* VI:4*720P; VO:HD0(HDMI ),  video preview . Only for Hi3520DV200 */
			s32Ret = SAMPLE_VIO_4_720P();
			break;
			
        default:
            printf("the index is invaild!\n");
            SAMPLE_VIO_Usage(argv[0]);
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

