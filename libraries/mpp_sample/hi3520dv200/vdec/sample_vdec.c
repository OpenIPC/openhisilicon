/******************************************************************************
  A simple program of Hisilicon HI3531 video decode implementation.
  Copyright (C), 2010-2011, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
    Modification:  2011-12 Created
******************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/mman.h>

#include "sample_comm.h"

#define SAMPLE_MAX_VDEC_CHN_CNT 8

typedef struct sample_vdec_sendparam
{
    pthread_t Pid;
    HI_BOOL bRun;
    VDEC_CHN VdChn;    
    PAYLOAD_TYPE_E enPayload;
	HI_S32 s32MinBufSize;
    VIDEO_MODE_E enVideoMode;
}SAMPLE_VDEC_SENDPARAM_S;

//HI_S32 gs_VoDevCnt = 4;
VIDEO_NORM_E gs_enNorm = VIDEO_ENCODING_MODE_PAL;
HI_U32 gs_u32ViFrmRate = 0;
SAMPLE_VDEC_SENDPARAM_S gs_SendParam[SAMPLE_MAX_VDEC_CHN_CNT];
HI_S32 gs_s32Cnt;

    
/******************************************************************************
* function : send stream to vdec
******************************************************************************/
void* SAMPLE_VDEC_SendStream(void* p)
{
    VDEC_STREAM_S stStream;
    SAMPLE_VDEC_SENDPARAM_S *pstSendParam;
    char sFileName[50], sFilePostfix[20];
    FILE* fp = NULL;
    HI_S32 s32BlockMode = HI_IO_BLOCK;
    HI_U8 *pu8Buf;
    HI_U64 u64pts;
    HI_S32 s32IntervalTime = 40000;
    HI_S32 i, s32Ret, len, start;
    HI_S32 s32UsedBytes, s32ReadLen;
    HI_BOOL bFindStart, bFindEnd;    

    start = 0;
    u64pts= 0;
    s32UsedBytes = 0;
    pstSendParam = (SAMPLE_VDEC_SENDPARAM_S *)p;

    /******************* open the stream file *****************/
    SAMPLE_COMM_SYS_Payload2FilePostfix(pstSendParam->enPayload, sFilePostfix);
    sprintf(sFileName, "stream_chn0%s", sFilePostfix);
    fp = fopen(sFileName, "r");
    if (HI_NULL == fp)
    {
        SAMPLE_PRT("can't open file %s in send stream thread:%d\n", sFileName,pstSendParam->VdChn);
        return (HI_VOID *)(HI_FAILURE);
    }
    printf("open file [%s] ok in send stream thread:%d!\n", sFileName,pstSendParam->VdChn);


    /******************* malloc the  stream buffer in user space *****************/
    if(pstSendParam->s32MinBufSize!=0)
    {
        pu8Buf=malloc(pstSendParam->s32MinBufSize);
        if(pu8Buf==NULL)
        {
            SAMPLE_PRT("can't alloc %d in send stream thread:%d\n",pstSendParam->s32MinBufSize,pstSendParam->VdChn);
            fclose(fp);
            return (HI_VOID *)(HI_FAILURE);
        }
    }
    else
    {
        SAMPLE_PRT("none buffer to operate in send stream thread:%d\n",pstSendParam->VdChn);
        return (HI_VOID *)(HI_FAILURE);
    }

    while (pstSendParam->bRun)
    {
        fseek(fp, s32UsedBytes, SEEK_SET);
        s32ReadLen = fread(pu8Buf, 1, pstSendParam->s32MinBufSize, fp);
        if (s32ReadLen<=0)
        {
             printf("file end.\n");
             break;
        }

        /******************* cutting the stream for frame *****************/
        if( (pstSendParam->enVideoMode==VIDEO_MODE_FRAME) && (pstSendParam->enPayload== PT_H264) )
        {
            bFindStart = HI_FALSE;  
            bFindEnd   = HI_FALSE;  

            for (i=0; i<s32ReadLen-8; i++)
            {
                int tmp = pu8Buf[i+3] & 0x1F;
                if (  pu8Buf[i] == 0 && pu8Buf[i+1] == 0 && pu8Buf[i+2] == 1 && 
                       (
                           ((tmp == 5 || tmp == 1) && ((pu8Buf[i+4]&0x80) == 0x80)) ||
                           (tmp == 20 && (pu8Buf[i+7]&0x80) == 0x80)
                        )
                   )            
                {
                    bFindStart = HI_TRUE;
                    i += 8;
                    break;
                }
            }

            for (; i<s32ReadLen-8; i++)
            {
                int tmp = pu8Buf[i+3] & 0x1F;
                if (  pu8Buf[i  ] == 0 && pu8Buf[i+1] == 0 && pu8Buf[i+2] == 1 && 
                            (
                                  tmp == 15 || tmp == 7 || tmp == 8 || tmp == 6 || 
                                  ((tmp == 5 || tmp == 1) && ((pu8Buf[i+4]&0x80) == 0x80)) ||
                                  (tmp == 20 && (pu8Buf[i+7]&0x80) == 0x80)
                              )
                   )                   
                {					
                    bFindEnd = HI_TRUE;
                    break;
                }
            }			
        
            s32ReadLen = i;
            if (bFindStart == HI_FALSE)
            {
                SAMPLE_PRT("can not find start code in send stream thread:%d\n",pstSendParam->VdChn);
            }
            else if (bFindEnd == HI_FALSE)
            {
                s32ReadLen = i+8;
            }            
        }
        else if( (pstSendParam->enPayload== PT_JPEG) || (pstSendParam->enPayload == PT_MJPEG) )
        {
            bFindStart = HI_FALSE;  
            bFindEnd   = HI_FALSE;                            
            for (i=0; i<s32ReadLen-2; i++)
            {
                if (pu8Buf[i] == 0xFF && pu8Buf[i+1] == 0xD8) 
                {  
                    start = i;
                    bFindStart = HI_TRUE;
                    i = i + 2;
                    break;
                }  
            }
        
            for (; i<s32ReadLen-4; i++)
            {
                if ((pu8Buf[i] == 0xFF) && (pu8Buf[i+1]& 0xF0) == 0xE0)
                {   
                     len = (pu8Buf[i+2]<<8) + pu8Buf[i+3];                    
                     i += 1 + len;                  
                }
                else
                {
                    break;
                }
            }
        
            for (; i<s32ReadLen-2; i++)
            {
                if (pu8Buf[i] == 0xFF && pu8Buf[i+1] == 0xD8)
                {
                    bFindEnd = HI_TRUE;
                    break;
                } 
            }
          
            s32ReadLen = i;
            if (bFindStart == HI_FALSE)
            {
                printf("\033[0;31mALERT!!!,can not find start code in send stream thread:%d!!!\033[0;39m\n",
                pstSendParam->VdChn);    
            }
            else if (bFindEnd == HI_FALSE)
            {
                s32ReadLen = i+2;
            }            
        }
          
         stStream.u64PTS  = u64pts;
         stStream.pu8Addr = pu8Buf + start;
         stStream.u32Len  = s32ReadLen;
        if(pstSendParam->enVideoMode==VIDEO_MODE_FRAME)
        {
            u64pts+=40000;
        }

        /******************* send stream *****************/
        if (s32BlockMode == HI_IO_BLOCK)
        {
            s32Ret=HI_MPI_VDEC_SendStream(pstSendParam->VdChn, &stStream, HI_IO_BLOCK);
        }
        else if (s32BlockMode == HI_IO_NOBLOCK)
        {
            s32Ret=HI_MPI_VDEC_SendStream(pstSendParam->VdChn, &stStream, HI_IO_NOBLOCK);
        }
        else 
        {
            s32Ret=HI_MPI_VDEC_SendStream_TimeOut(pstSendParam->VdChn, &stStream, 8000);
        }

        if (HI_SUCCESS == s32Ret)
        { 
            s32UsedBytes = s32UsedBytes +s32ReadLen + start;
        }
        else 
        {
            if (s32BlockMode != HI_IO_BLOCK)
            {
                SAMPLE_PRT("failret:%x\n",s32Ret);
            }
            usleep(s32IntervalTime);
        }
        usleep(20000);

    }

	printf("send steam thread %d return ...\n", pstSendParam->VdChn);
	fflush(stdout);
	if (pu8Buf != HI_NULL) 
	{
        free(pu8Buf);
	}
	fclose(fp);
	return (HI_VOID *)HI_SUCCESS;
}


/******************************************************************************
* function : create vdec chn
******************************************************************************/
static HI_S32 SAMPLE_VDEC_CreateVdecChn(HI_S32 s32ChnID, SIZE_S *pstSize, PAYLOAD_TYPE_E enType, VIDEO_MODE_E enVdecMode)
{
    VDEC_CHN_ATTR_S stAttr;
    VDEC_PRTCL_PARAM_S stPrtclParam;
    HI_S32 s32Ret;

    memset(&stAttr, 0, sizeof(VDEC_CHN_ATTR_S));

    stAttr.enType = enType;
    stAttr.u32BufSize = pstSize->u32Height * pstSize->u32Width;//This item should larger than u32Width*u32Height*3/4
    stAttr.u32Priority = 1;//此处必须大于0
    stAttr.u32PicWidth = pstSize->u32Width;
    stAttr.u32PicHeight = pstSize->u32Height;
    
    switch (enType)
    {
        case PT_H264:
    	    stAttr.stVdecVideoAttr.u32RefFrameNum = 2;
    	    stAttr.stVdecVideoAttr.enMode = enVdecMode;
    	    stAttr.stVdecVideoAttr.s32SupportBFrame = 0;
            break;
        case PT_JPEG:
            stAttr.stVdecJpegAttr.enMode = enVdecMode;
            break;
        case PT_MJPEG:
            stAttr.stVdecJpegAttr.enMode = enVdecMode;
            break;
        default:
            SAMPLE_PRT("err type \n");
            return HI_FAILURE;
    }

    s32Ret = HI_MPI_VDEC_CreateChn(s32ChnID, &stAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VDEC_CreateChn failed errno 0x%x \n", s32Ret);
        return s32Ret;
    }

    s32Ret = HI_MPI_VDEC_GetPrtclParam(s32ChnID, &stPrtclParam);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VDEC_GetPrtclParam failed errno 0x%x \n", s32Ret);
        return s32Ret;
    }

    stPrtclParam.s32MaxSpsNum = 21;
    stPrtclParam.s32MaxPpsNum = 22;
    stPrtclParam.s32MaxSliceNum = 100;
    s32Ret = HI_MPI_VDEC_SetPrtclParam(s32ChnID, &stPrtclParam);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VDEC_SetPrtclParam failed errno 0x%x \n", s32Ret);
        return s32Ret;
    }

    s32Ret = HI_MPI_VDEC_StartRecvStream(s32ChnID);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VDEC_StartRecvStream failed errno 0x%x \n", s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}

/******************************************************************************
* function : force to stop decoder and destroy channel.
*            stream left in decoder will not be decoded.
******************************************************************************/
void SAMPLE_VDEC_ForceDestroyVdecChn(HI_S32 s32ChnID)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_VDEC_StopRecvStream(s32ChnID);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VDEC_StopRecvStream failed errno 0x%x \n", s32Ret);
    }

    s32Ret = HI_MPI_VDEC_DestroyChn(s32ChnID);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VDEC_DestroyChn failed errno 0x%x \n", s32Ret);
    }
}

/******************************************************************************
* function : wait for decoder finished and destroy channel.
*            Stream left in decoder will be decoded.
******************************************************************************/
void SAMPLE_VDEC_WaitDestroyVdecChn(HI_S32 s32ChnID, VIDEO_MODE_E enVdecMode)
{
    HI_S32 s32Ret;
    VDEC_CHN_STAT_S stStat;

    memset(&stStat, 0, sizeof(VDEC_CHN_STAT_S));

    s32Ret = HI_MPI_VDEC_StopRecvStream(s32ChnID);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("HI_MPI_VDEC_StopRecvStream failed errno 0x%x \n", s32Ret);
        return;
    }

    /*** wait destory ONLY used at frame mode! ***/
    if (VIDEO_MODE_FRAME == enVdecMode)
    {
        while (1)
        {
            //printf("LeftPics:%d, LeftStreamFrames:%d\n", stStat.u32LeftPics,stStat.u32LeftStreamFrames);
            usleep(40000);
            s32Ret = HI_MPI_VDEC_Query(s32ChnID, &stStat);
            if (s32Ret != HI_SUCCESS)
            {
                SAMPLE_PRT("HI_MPI_VDEC_Query failed errno 0x%x \n", s32Ret);
                return;
            }
            if ((stStat.u32LeftPics == 0) && (stStat.u32LeftStreamFrames == 0))
            {
                printf("had no stream and pic left\n");
                break;
            }
        }
    }
    s32Ret = HI_MPI_VDEC_DestroyChn(s32ChnID);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("HI_MPI_VDEC_DestroyChn failed errno 0x%x \n", s32Ret);
        return;
    }
}

/******************************************************************************
* function : show usage
******************************************************************************/
void SAMPLE_VDEC_Usage(char *sPrgNm)
{
    printf("Usage : %s <index>\n", sPrgNm);
    printf("index:\n");
    printf("\t0) H264 -> VPSS -> VO(HD).\n");
    printf("\t1) JPEG ->VPSS -> VO(HD).\n");
    printf("\t2) MJPEG -> VO(SD).\n");
    printf("\t3) H264 -> VPSS -> VO(HD PIP PAUSE STEP).\n");
    return;
}

/******************************************************************************
* function : to process abnormal case
******************************************************************************/
void SAMPLE_VDEC_HandleSig(HI_S32 signo)
{
    HI_S32 i;

    if (SIGINT == signo || SIGTSTP == signo)
    {
        printf("SAMPLE_VDEC_HandleSig\n");
        for (i=0; i<gs_s32Cnt; i++)
        {
            if (HI_FALSE != gs_SendParam[i].bRun)
            {
                gs_SendParam[i].bRun = HI_FALSE;
                pthread_join(gs_SendParam[i].Pid, 0);
            }
            printf("join thread %d.\n", i);
        }

        SAMPLE_COMM_SYS_Exit();

        printf("program exit abnormally!\n");    
    }

    exit(-1);
}

/******************************************************************************
* function : vdec process
*            vo is sd : vdec -> vo
*            vo is hd : vdec -> vpss -> vo
******************************************************************************/
HI_S32 SAMPLE_VDEC_Process(PIC_SIZE_E enPicSize, PAYLOAD_TYPE_E enType, HI_S32 s32Cnt, VO_DEV VoDev)
{
    VDEC_CHN VdChn;
    HI_S32 s32Ret;
    SIZE_S stSize;
    VB_CONF_S stVbConf;
    HI_S32 i;
    VPSS_GRP VpssGrp;
    VIDEO_MODE_E enVdecMode;
    HI_CHAR ch;

    VO_CHN VoChn;
    VO_PUB_ATTR_S stVoPubAttr; 
    SAMPLE_VO_MODE_E enVoMode;
    HI_U32 u32WndNum, u32BlkSize;

    HI_BOOL bVoHd; // through Vpss or not. if vo is SD, needn't through vpss
 
    /******************************************
     step 1: init varaible.
    ******************************************/
    gs_u32ViFrmRate = (VIDEO_ENCODING_MODE_PAL == gs_enNorm)?25:30;
    
    if (s32Cnt > SAMPLE_MAX_VDEC_CHN_CNT || s32Cnt <= 0)
    {
        SAMPLE_PRT("Vdec count %d err, should be in [%d,%d]. \n", s32Cnt, 1, SAMPLE_MAX_VDEC_CHN_CNT);
        
        return HI_FAILURE;
    }
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(gs_enNorm, enPicSize, &stSize);
    if (HI_SUCCESS !=s32Ret)
    {
        SAMPLE_PRT("get picture size failed!\n");
        return HI_FAILURE;
    }
    if (704 == stSize.u32Width)
    {
        stSize.u32Width = 720;
    }
    else if (352 == stSize.u32Width)
    {
        stSize.u32Width = 360;
    }
    else if (176 == stSize.u32Width)
    {
        stSize.u32Width = 180;
    }
    
    // through Vpss or not. if vo is SD, needn't through vpss
    if (SAMPLE_VO_DEV_DHD0 != VoDev ) 
    {
        bVoHd = HI_FALSE;
    }
    else
    {
        bVoHd = HI_TRUE;
    }
    /******************************************
     step 2: mpp system init.
    ******************************************/
    memset(&stVbConf,0,sizeof(VB_CONF_S));

    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm,\
                PIC_D1, SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH);
    stVbConf.u32MaxPoolCnt = 128;

    /* hist buf*/
    stVbConf.astCommPool[0].u32BlkSize = (196*4);
    stVbConf.astCommPool[0].u32BlkCnt = s32Cnt * 6;
    memset(stVbConf.astCommPool[0].acMmzName,0,
        sizeof(stVbConf.astCommPool[0].acMmzName));
	
    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("mpp init failed!\n");
        return HI_FAILURE;
    }
  
    /******************************************
     step 3: start vpss, if ov is hd.
    ******************************************/    
    if (HI_TRUE == bVoHd)
    {
        s32Ret = SAMPLE_COMM_VPSS_Start(s32Cnt, &stSize, VPSS_MAX_CHN_NUM,NULL);
        if (HI_SUCCESS !=s32Ret)
        {
            SAMPLE_PRT("vpss start failed!\n");
            goto END_0;
        }
    }
 
    /******************************************
     step 4: start vo
    ******************************************/
    u32WndNum = 1;
    enVoMode = VO_MODE_1MUX;

    if (HI_TRUE == bVoHd)
    {
        if(VIDEO_ENCODING_MODE_PAL== gs_enNorm)
        {
            stVoPubAttr.enIntfSync = VO_OUTPUT_720P50;
        }
        else
        {
            stVoPubAttr.enIntfSync = VO_OUTPUT_720P60;
        }
        
        stVoPubAttr.enIntfType = VO_INTF_VGA;
        stVoPubAttr.u32BgColor = 0x000000ff;
        stVoPubAttr.bDoubleFrame = HI_FALSE;
    }
    else
    {
        if(VIDEO_ENCODING_MODE_PAL== gs_enNorm)
        {
            stVoPubAttr.enIntfSync = VO_OUTPUT_PAL;
        }
        else
        {
            stVoPubAttr.enIntfSync = VO_OUTPUT_NTSC;
        }
        
        stVoPubAttr.enIntfType = VO_INTF_CVBS;
        stVoPubAttr.u32BgColor = 0x000000ff;
        stVoPubAttr.bDoubleFrame = HI_FALSE;
    }
    
    s32Ret = SAMPLE_COMM_VO_StartDevLayer(VoDev, &stVoPubAttr, gs_u32ViFrmRate);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start SAMPLE_COMM_VO_StartDevLayer failed!\n");
        goto END_1;
    }
    
    s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, &stVoPubAttr, enVoMode);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start SAMPLE_COMM_VO_StartChn failed!\n");
        goto END_2;
    }

    if (HI_TRUE == bVoHd)
    {
        /* if it's displayed on HDMI, we should start HDMI */
        if (stVoPubAttr.enIntfType & VO_INTF_HDMI)
        {
            if (HI_SUCCESS != SAMPLE_COMM_VO_HdmiStart(stVoPubAttr.enIntfSync))
            {
                SAMPLE_PRT("Start SAMPLE_COMM_VO_HdmiStart failed!\n");
                goto END_1;
            }
        }
    }

    for(i=0;i<u32WndNum;i++)
    {
        VoChn = i;

        if (HI_TRUE == bVoHd)
        {
            VpssGrp = i;
            s32Ret = SAMPLE_COMM_VO_BindVpss(VoDev,VoChn,VpssGrp,VPSS_PRE0_CHN);
            if (HI_SUCCESS != s32Ret)
            {
                SAMPLE_PRT("SAMPLE_COMM_VO_BindVpss failed!\n");
                goto END_2;
            }
        }
    }

    /******************************************
     step 5: start vdec & bind it to vpss or vo
    ******************************************/ 
    if (PT_H264 == enType)
    { 
        while(1)
        {
            printf("please choose vdec mode:\n");
            printf("\t0) frame mode.\n");
            printf("\t1) stream mode.\n");
            ch = getchar();
            getchar();
            if ('0' == ch)
            {
                enVdecMode = VIDEO_MODE_FRAME;
                break;
            }
            else if ('1' == ch)
            {
                enVdecMode = VIDEO_MODE_STREAM;
                break;
            }
            else
            {
                printf("input invaild! please try again.\n");
                continue;
            }
        }
    }
    else if (PT_JPEG == enType || PT_MJPEG ==enType)	
    {
        /* JPEG, MJPEG must be Frame mode! */
        enVdecMode = VIDEO_MODE_FRAME;
    }
    
    for (i=0; i<s32Cnt; i++)
    {
        /***** create vdec chn *****/
        VdChn = i;        
        s32Ret = SAMPLE_VDEC_CreateVdecChn(VdChn, &stSize, enType, enVdecMode);
        if (HI_SUCCESS !=s32Ret)
        {
            SAMPLE_PRT("create vdec chn failed!\n");
            goto END_3;
        }
        
        /***** bind vdec to vpss *****/
        if (HI_TRUE == bVoHd)
        {
            VpssGrp = i;
            s32Ret = SAMLE_COMM_VDEC_BindVpss(VdChn, VpssGrp);
            if (HI_SUCCESS !=s32Ret)
            {
                SAMPLE_PRT("vdec(vdch=%d) bind vpss(vpssg=%d) failed!\n", VdChn, VpssGrp);
                goto END_3;
            }
        }
        else
    	{
            VoChn =  i;
            s32Ret = SAMLE_COMM_VDEC_BindVo(VdChn, VoDev, VoChn);
            if (HI_SUCCESS !=s32Ret)
            {
                SAMPLE_PRT("vdec(vdch=%d) bind vpss(vpssg=%d) failed!\n", VdChn, VpssGrp);
                goto END_3;
            }
        }
    }

    /******************************************
     step 6: open file & video decoder
    ******************************************/
    for (i=0; i<s32Cnt; i++)
    {
        gs_SendParam[i].bRun = HI_TRUE;
        gs_SendParam[i].VdChn = i;
        gs_SendParam[i].enPayload = enType;
        gs_SendParam[i].enVideoMode = enVdecMode;
        gs_SendParam[i].s32MinBufSize = stSize.u32Height * stSize.u32Width * 3/ 4;
        pthread_create(&gs_SendParam[i].Pid, NULL, SAMPLE_VDEC_SendStream, &gs_SendParam[i]);
    }

    if (PT_JPEG != enType)
    {
        printf("you can press ctrl+c to terminate program before normal exit.\n");
    }
    /******************************************
     step 7: join thread
    ******************************************/
    for (i=0; i<s32Cnt; i++)
    {      
        pthread_join(gs_SendParam[i].Pid, 0);
        printf("join thread %d.\n", i);
    }

    printf("press two enter to quit!\n");
    getchar();
    getchar();
    /******************************************
     step 8: Unbind vdec to vpss & destroy vdec-chn
    ******************************************/
END_3:
    for (i=0; i<s32Cnt; i++)
    {
        VdChn = i;
        SAMPLE_VDEC_WaitDestroyVdecChn(VdChn, enVdecMode);
        if (HI_TRUE == bVoHd)
        {
            VpssGrp = i;
            SAMLE_COMM_VDEC_UnBindVpss(VdChn, VpssGrp);
        }
        else
        {
            VoChn = i;
            SAMLE_COMM_VDEC_UnBindVo(VdChn, VoDev, VoChn);
        }
    }
    /******************************************
     step 9: stop vo
    ******************************************/
END_2:
    SAMPLE_COMM_VO_StopChn(VoDev, enVoMode);
    for(i=0;i<u32WndNum;i++)
    {
        VoChn = i;
        if (HI_TRUE == bVoHd)
        {
            VpssGrp = i;
            SAMPLE_COMM_VO_UnBindVpss(VoDev,VoChn,VpssGrp,VPSS_PRE0_CHN);
        }
    }
    SAMPLE_COMM_VO_StopDevLayer(VoDev);
END_1:
    if (HI_TRUE == bVoHd)
    {
        if (stVoPubAttr.enIntfType & VO_INTF_HDMI)
        {
            SAMPLE_COMM_VO_HdmiStop();
        }
        SAMPLE_COMM_VPSS_Stop(s32Cnt, VPSS_MAX_CHN_NUM);
    }
    /******************************************
     step 10: exit mpp system
    ******************************************/
END_0:
    SAMPLE_COMM_SYS_Exit();

    return HI_SUCCESS;
}

HI_S32 SAMPLE_VDEC_ProcessForPip(PIC_SIZE_E enPicSize, PAYLOAD_TYPE_E enType, HI_S32 s32Cnt, VO_DEV VoDev)
{
    VDEC_CHN VdChn;
    HI_S32 s32Ret;
    SIZE_S stSize;
    VB_CONF_S stVbConf;
    HI_S32 i;
    VPSS_GRP VpssGrp, VpssGrpForPip = 1;
    VIDEO_MODE_E enVdecMode;

    VO_CHN VoChn;
    VO_PUB_ATTR_S stVoPubAttr; 
    SAMPLE_VO_MODE_E enVoMode;
    HI_U32 u32WndNum, u32BlkSize;

    HI_BOOL bVoHd; // through Vpss or not. if vo is SD, needn't through vpss
    VO_CHN_ATTR_S stVoChnAttr;
    VO_VIDEO_LAYER_ATTR_S stPipLayerAttr;
    VPSS_CROP_INFO_S stVpssCropInfo;
    
    /******************************************
     step 1: init varaible.
    ******************************************/
    gs_u32ViFrmRate = (VIDEO_ENCODING_MODE_PAL== gs_enNorm)?25:30;
    
    if (SAMPLE_MAX_VDEC_CHN_CNT < s32Cnt)
    {
        SAMPLE_PRT("Vdec count is bigger than sample define!\n");
        return HI_FAILURE;
    }
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(gs_enNorm, enPicSize, &stSize);
    if (HI_SUCCESS !=s32Ret)
    {
        SAMPLE_PRT("get picture size failed!\n");
        return HI_FAILURE;
    }
    if (704 == stSize.u32Width)
    {
        stSize.u32Width = 720;
    }
    else if (352 == stSize.u32Width)
    {
        stSize.u32Width = 360;
    }
    else if (176 == stSize.u32Width)
    {
        stSize.u32Width = 180;
    }

    /* through Vpss or not. if vo is SD, needn't through vpss */
    if (SAMPLE_VO_DEV_DHD0 != VoDev) 
    {
        bVoHd = HI_FALSE;
    }
    else
    {
        bVoHd = HI_TRUE;
    }
    /******************************************
     step 2: mpp system init.
    ******************************************/
    memset(&stVbConf,0,sizeof(VB_CONF_S));

    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm,\
                PIC_D1, SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH);
    stVbConf.u32MaxPoolCnt = 128;
    /*vdec no need common video buffer!*/

    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("mpp init failed!\n");
        return HI_FAILURE;
    }
    
    /******************************************
     step 3: start vpss, if ov is hd.
    ******************************************/    
    if (HI_TRUE == bVoHd)
    {
        s32Ret = SAMPLE_COMM_VPSS_Start(s32Cnt+1, &stSize, VPSS_MAX_CHN_NUM,NULL);
        if (HI_SUCCESS !=s32Ret)
        {
            SAMPLE_PRT("vpss start failed!\n");
            goto END_0;
        }
    }
 
    /******************************************
     step 4: start vo
    ******************************************/
    u32WndNum = 1;
    enVoMode = VO_MODE_1MUX;

    if (HI_TRUE == bVoHd)
    {
        if(VIDEO_ENCODING_MODE_PAL== gs_enNorm)
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
    }
    else
    {
        if(VIDEO_ENCODING_MODE_PAL== gs_enNorm)
        {
            stVoPubAttr.enIntfSync = VO_OUTPUT_PAL;
        }
        else
        {
            stVoPubAttr.enIntfSync = VO_OUTPUT_NTSC;
        }
        
        stVoPubAttr.enIntfType = VO_INTF_CVBS;
        stVoPubAttr.u32BgColor = 0x000000ff;
        stVoPubAttr.bDoubleFrame = HI_FALSE;
    }
    
    s32Ret = SAMPLE_COMM_VO_StartDevLayer(VoDev, &stVoPubAttr, gs_u32ViFrmRate);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start SAMPLE_COMM_VO_StartDevLayer failed!\n");
        goto END_1;
    }
    
    s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, &stVoPubAttr, enVoMode);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start SAMPLE_COMM_VO_StartChn failed!\n");
        goto END_2;
    }

    if (HI_TRUE == bVoHd)
    {
        /* if it's displayed on HDMI, we should start HDMI */
        if (stVoPubAttr.enIntfType & VO_INTF_HDMI)
        {
            if (HI_SUCCESS != SAMPLE_COMM_VO_HdmiStart(stVoPubAttr.enIntfSync))
            {
                SAMPLE_PRT("Start SAMPLE_COMM_VO_HdmiStart failed!\n");
                goto END_1;
            }
        }
    }

    for(i=0;i<u32WndNum;i++)
    {
        VoChn = i;

        if (HI_TRUE == bVoHd)
        {
            VpssGrp = i;
            s32Ret = SAMPLE_COMM_VO_BindVpss(VoDev,VoChn,VpssGrp,VPSS_PRE0_CHN);
            if (HI_SUCCESS != s32Ret)
            {
                SAMPLE_PRT("SAMPLE_COMM_VO_BindVpss failed!\n");
                goto END_2;
            }
        }
    }

    /******************************************
     step 5: start vdec & bind it to vpss or vo
    ******************************************/ 
    enVdecMode = VIDEO_MODE_FRAME;
    
    for (i=0; i<s32Cnt; i++)
    {
        /*** create vdec chn ***/
        VdChn = i;
        s32Ret = SAMPLE_VDEC_CreateVdecChn(VdChn, &stSize, enType, enVdecMode);
        if (HI_SUCCESS !=s32Ret)
        {
            SAMPLE_PRT("create vdec chn failed!\n");
            goto END_3;
        }
        /*** bind vdec to vpss ***/
        if (HI_TRUE == bVoHd)
        {
            VpssGrp = i;
            s32Ret = SAMLE_COMM_VDEC_BindVpss(VdChn, VpssGrp);
            if (HI_SUCCESS !=s32Ret)
            {
                SAMPLE_PRT("vdec(vdch=%d) bind vpss(vpssg=%d) failed!\n", VdChn, VpssGrp);
                goto END_3;
            }
        }
        else
    	{
            VoChn =  i;
            s32Ret = SAMLE_COMM_VDEC_BindVo(VdChn, VoDev, VoChn);
            if (HI_SUCCESS !=s32Ret)
            {
                SAMPLE_PRT("vdec(vdch=%d) bind vpss(vpssg=%d) failed!\n", VdChn, VpssGrp);
                goto END_3;
            }
        }
    }

    /******************************************
     step 6: open file & video decoder
    ******************************************/
    for (i=0; i<s32Cnt; i++)
    {
        gs_SendParam[i].bRun = HI_TRUE;
        gs_SendParam[i].VdChn = i;
        gs_SendParam[i].enPayload = enType;
        gs_SendParam[i].enVideoMode = enVdecMode;
        gs_SendParam[i].s32MinBufSize = stSize.u32Height * stSize.u32Width *3 / 4;
        pthread_create(&gs_SendParam[i].Pid, NULL, SAMPLE_VDEC_SendStream, &gs_SendParam[i]);
    }

    if (PT_JPEG != enType)
    {
        printf("you can press ctrl+c to terminate program before normal exit.\n");
    }
    
    printf("please press any key to show pip:\n");
    getchar();

    s32Ret = HI_MPI_VO_ChnPause(VoDev, 0);
    if(s32Ret != HI_SUCCESS)
    {
        printf("HI_MPI_VO_ChnPause err 0x%x\n",s32Ret);
        return s32Ret;
    }

    s32Ret = HI_MPI_VO_PipLayerBindDev(VoDev);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VO_PipLayerBindDev failed with %#x!\n", s32Ret);
        return s32Ret;
    }

    stPipLayerAttr.stDispRect.s32X = 0;
    stPipLayerAttr.stDispRect.s32Y = 0;

    stPipLayerAttr.stDispRect.u32Height  = 1080;
    stPipLayerAttr.stDispRect.u32Width   = 1920;
    
    stPipLayerAttr.stImageSize.u32Height = 1080;
    stPipLayerAttr.stImageSize.u32Width = 1920;
    stPipLayerAttr.enPixFormat = SAMPLE_PIXEL_FORMAT;
    stPipLayerAttr.u32DispFrmRt = 25;

    s32Ret = HI_MPI_VO_SetPipLayerAttr(&stPipLayerAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VO_SetPipLayerAttr failed with %#x!\n", s32Ret);
        goto END_3;
    }

    s32Ret = HI_MPI_VO_EnablePipLayer();
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VO_EnablePipLayer failed with %#x!\n", s32Ret);
        goto END_3;
    }

    stVoChnAttr.stRect.s32X       = 0;
    stVoChnAttr.stRect.s32Y       = 0;
    stVoChnAttr.stRect.u32Width   = 1920;
    stVoChnAttr.stRect.u32Height  = 1080;
    stVoChnAttr.u32Priority       = 0;
    stVoChnAttr.bDeflicker        = HI_FALSE;
    s32Ret = HI_MPI_VO_SetChnAttr(VoDev, 0, &stVoChnAttr);
    if(s32Ret != HI_SUCCESS)
    {
        printf("HI_MPI_VO_SetChnAttr err 0x%x\n",s32Ret);
        goto END_3;
    }

    stVoChnAttr.stRect.s32X       = 1200;
    stVoChnAttr.stRect.s32Y       = 504;
    stVoChnAttr.stRect.u32Width   = 720;
    stVoChnAttr.stRect.u32Height  = 576;
    stVoChnAttr.u32Priority       = 1;
    stVoChnAttr.bDeflicker        = HI_FALSE;
    s32Ret = HI_MPI_VO_SetChnAttr(VoDev, 1, &stVoChnAttr);
    if(s32Ret != HI_SUCCESS)
    {
        printf("HI_MPI_VO_SetChnAttr err 0x%x\n",s32Ret);
        goto END_3;
    }
    s32Ret = HI_MPI_VO_EnableChn(VoDev, 1);
    if(s32Ret != HI_SUCCESS)
    {
        printf("HI_MPI_VO_EnableChn err 0x%x\n",s32Ret);
        goto END_3;
    }

    s32Ret = SAMPLE_COMM_VO_BindVpss(VoDev, 1, VpssGrpForPip, VPSS_PRE0_CHN);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_BindVpss failed!\n");
        goto END_3;
    }
    
#if 0
    s32Ret = HI_MPI_VPSS_UserGetGrpFrame(VpssGrp, &stVideoFrame, 0);
    if(s32Ret != HI_SUCCESS)
    {
        printf("HI_MPI_VPSS_UserGetGrpFrame err 0x%x\n",s32Ret);
        goto END_3;
    }

    s32Ret = HI_MPI_VPSS_UserSendFrame(VpssGrpForPip, &stVideoFrame);
    if(s32Ret != HI_SUCCESS)
    {
        printf("HI_MPI_VPSS_UserSendFrame err 0x%x\n",s32Ret);
        goto END_3;
    }
    s32Ret = HI_MPI_VPSS_UserReleaseGrpFrame(VpssGrp, &stVideoFrame);
    if(s32Ret != HI_SUCCESS)
    {
        printf("HI_MPI_VPSS_UserReleaseGrpFrame err 0x%x\n",s32Ret);
        goto END_3;
    }
#endif


    stVpssCropInfo.bEnable = HI_TRUE;
    stVpssCropInfo.enCropCoordinate = VPSS_CROP_ABS_COOR;
    stVpssCropInfo.stCropRect.s32X = 0;
    stVpssCropInfo.stCropRect.s32Y = 0;
    stVpssCropInfo.stCropRect.u32Height = 288;
    stVpssCropInfo.stCropRect.u32Width = 352;
    stVpssCropInfo.enCapSel = VPSS_CAPSEL_BOTH;
    s32Ret = HI_MPI_VPSS_SetCropCfg(VpssGrp, &stVpssCropInfo);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartChn failed with %#x!\n", s32Ret);
        goto END_3;
    }

    s32Ret = HI_MPI_VO_ChnRefresh(VoDev, 0);
    if(s32Ret != HI_SUCCESS)
    {
        printf("HI_MPI_VO_ChnRefresh err 0x%x\n",s32Ret);
        goto END_3;
    }
    
    for (i = 0; i < 10; i++)
    {
        printf("press any key to step %d!\n", i);
        getchar();
        s32Ret = HI_MPI_VO_ChnStep(VoDev, 0);
        if(s32Ret != HI_SUCCESS)
        {
            printf("HI_MPI_VO_ChnStep err 0x%x\n",s32Ret);
            goto END_3;
        }
        usleep(20000); // Here we should usleep some ms to make sure the step has taken effect, then we can get the same pic from VPSS.


#if 0        
        s32Ret = HI_MPI_VPSS_UserGetGrpFrame(VpssGrp, &stVideoFrame, 0);
        if(s32Ret != HI_SUCCESS)
        {
            printf("HI_MPI_VPSS_UserGetGrpFrame err 0x%x\n",s32Ret);
            goto END_3;
        }
        
        s32Ret = HI_MPI_VPSS_UserSendFrame(VpssGrpForPip, &stVideoFrame);
        if(s32Ret != HI_SUCCESS)
        {
            printf("HI_MPI_VPSS_UserSendFrame err 0x%x\n",s32Ret);
            goto END_3;
        }
        s32Ret = HI_MPI_VPSS_UserReleaseGrpFrame(VpssGrp, &stVideoFrame);
        if(s32Ret != HI_SUCCESS)
        {
            printf("HI_MPI_VPSS_UserReleaseGrpFrame err 0x%x\n",s32Ret);
            goto END_3;
        }
#endif



    }

    printf("press any key to resume!\n");
    getchar();

    s32Ret = HI_MPI_VO_ChnResume(VoDev, 0);
    if(s32Ret != HI_SUCCESS)
    {
        printf("HI_MPI_VO_ChnResume err 0x%x\n",s32Ret);
        goto END_3;
    }

    s32Ret = SAMLE_COMM_VDEC_BindVpss(VdChn, VpssGrpForPip);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_BindVpss failed!\n");
        goto END_3;
        
    }

    s32Ret = HI_MPI_VPSS_ResetGrp(VpssGrp);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VPSS_ResetGrp failed!\n");
        goto END_3;
        
    }
    s32Ret = HI_MPI_VPSS_ResetGrp(VpssGrpForPip);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VPSS_ResetGrp failed!\n");
        goto END_3;
        
    }
    s32Ret = HI_MPI_VO_ClearChnBuffer(VoDev, 0, HI_FALSE);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VO_ClearChnBuffer failed!\n");
        goto END_3;
        
    }
    s32Ret = HI_MPI_VO_ClearChnBuffer(VoDev, 1, HI_FALSE);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VO_ClearChnBuffer failed!\n");
        goto END_3;
        
    }

    /******************************************
     step 7: join thread
    ******************************************/
    for (i=0; i<s32Cnt; i++)
    {
        pthread_join(gs_SendParam[i].Pid, 0);
        printf("join thread %d.\n", i);
    }

    printf("press two enter to quit!\n");
    getchar();
    getchar();


    /******************************************
     step 8: Unbind vdec to vpss & destroy vdec-chn
    ******************************************/
END_3:
    for (i=0; i<s32Cnt; i++)
    {
        VdChn = i;
        SAMPLE_VDEC_WaitDestroyVdecChn(VdChn, enVdecMode);
        if (HI_TRUE == bVoHd)
        {
            VpssGrp = i;
            SAMLE_COMM_VDEC_UnBindVpss(VdChn, VpssGrp);
        }
        else
        {
            VoChn = i;
            SAMLE_COMM_VDEC_UnBindVo(VdChn, VoDev, VoChn);
        }
    }
    
    s32Ret = HI_MPI_VO_DisableChn(VoDev, 1);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VO_DisableChn failed with %#x!\n", s32Ret);
    }

    s32Ret = HI_MPI_VO_DisablePipLayer();
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VO_DisablePipLayer failed with %#x!\n", s32Ret);
    }
    /******************************************
     step 9: stop vo
    ******************************************/
END_2:
    SAMPLE_COMM_VO_StopChn(VoDev, enVoMode);
    for(i=0;i<u32WndNum;i++)
    {
        VoChn = i;
        if (HI_TRUE == bVoHd)
        {
            VpssGrp = i;
            SAMPLE_COMM_VO_UnBindVpss(VoDev,VoChn,VpssGrp,VPSS_PRE0_CHN);
        }
    }
    SAMPLE_COMM_VO_StopDevLayer(VoDev);
END_1:
    if (HI_TRUE == bVoHd)
    {
        if (stVoPubAttr.enIntfType & VO_INTF_HDMI)
        {
            SAMPLE_COMM_VO_HdmiStop();
        }
        SAMPLE_COMM_VPSS_Stop(s32Cnt+1, VPSS_MAX_CHN_NUM);
    }
    /******************************************
     step 10: exit mpp system
    ******************************************/
END_0:
    SAMPLE_COMM_SYS_Exit();

    return HI_SUCCESS;
}

/****************************************************************************
* function: main
****************************************************************************/
int main(int argc, char* argv[])
{
    HI_S32 s32Index;
    if (argc != 2)
    {
        SAMPLE_VDEC_Usage(argv[0]);
        return HI_FAILURE;
    }
    
    signal(SIGINT, SAMPLE_VDEC_HandleSig);
    signal(SIGTERM, SAMPLE_VDEC_HandleSig);

    s32Index = atoi(argv[1]);

    switch (s32Index)
    {
        case 0: /* H264 -> VPSS -> VO(HD) */
            gs_s32Cnt = 1;
            SAMPLE_VDEC_Process(PIC_D1, PT_H264, gs_s32Cnt, SAMPLE_VO_DEV_DHD0);
            break;
        case 1: /* JPEG ->VPSS -> VO(HD) */
            gs_s32Cnt = 1;
            SAMPLE_VDEC_Process(PIC_D1, PT_JPEG, gs_s32Cnt, SAMPLE_VO_DEV_DHD0);
            break;
        case 2: /* MJPEG -> VO(SD) */
            gs_s32Cnt = 1;
            SAMPLE_VDEC_Process(PIC_D1, PT_MJPEG, gs_s32Cnt, SAMPLE_VO_DEV_DSD0);
            break;
        case 3:  /* H264 -> VPSS -> VO(HD PIP PAUSE STEP)*/
            gs_s32Cnt = 1;
            if (HI_SUCCESS != SAMPLE_VDEC_ProcessForPip(PIC_D1, PT_H264, gs_s32Cnt, SAMPLE_VO_DEV_DHD0))
            {
                break;
            }
            break;
        default:
            printf("the index is invaild!\n");
            SAMPLE_VDEC_Usage(argv[0]);
            return HI_FAILURE;
        break;
    }

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
