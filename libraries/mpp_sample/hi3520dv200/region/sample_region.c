/******************************************************************************
  A simple program of Hisilicon HI3531 osd implementation.
  the flow as follows:
    1) init mpp system.
    2) start vi ( internal isp, ViDev 0, 2 vichn)                  
    3) start venc
    4) osd process, you can see video from some H264 streams files. the video will show as follows step:
        4.1) create some cover/osd regions
        4.2) display  cover/osd regions ( One Region -- Multi-VencGroup )
        4.3) change all vencGroups Regions' Layer
        4.4) change all vencGroups Regions' position
        4.5) change all vencGroups Regions' color
        4.6) change all vencGroups Regions' alpha (front and backgroud)
        4.7) load bmp form bmp-file to Region-0
        4.8) change BmpRegion-0
    6) stop venc
    7) stop vi and system.
  Copyright (C), 2010-2011, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
    Modification:  2011-2 Created
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
#include "loadbmp.h"

#include "sample_comm.h"

HI_BOOL bCase0 = HI_FALSE;
HI_BOOL bCase1 = HI_FALSE;

VI_SCAN_MODE_E    gs_enViScanMode = VI_SCAN_INTERLACED;

static HI_U32 gs_s32ChnCnt = 1;		/* vi, venc chn count */
static HI_S32 gs_s32RgnCntCur = 0;
static HI_S32 gs_s32RgnCnt = 5;
VIDEO_NORM_E gs_enNorm = VIDEO_ENCODING_MODE_PAL;


#define SAMPLE_RGN_SLEEP_TIME (200*1000)
#define SAMPLE_RGN_LOOP_COUNT 6
HI_U32    gs_u32ViFrmRate = 0;

#define SAMPLE_RGN_NOT_PASS(err)\
do {\
	printf("\033[0;31mtest case <%s>not pass at line:%d err:%x\033[0;39m\n",\
		__FUNCTION__,__LINE__,err);\
    exit(-1);\
}while(0)

/******************************************************************************
* function : Set region memory location
******************************************************************************/
HI_S32 SAMPLE_RGN_MemConfig(HI_VOID)
{
    HI_S32 i = 0;
    HI_S32 s32Ret = HI_SUCCESS;

    HI_CHAR * pcMmzName;
    MPP_CHN_S stMppChnRGN;

	/*the max chn of vpss,grp and venc is 64*/
    for(i=0; i<RGN_HANDLE_MAX; i++)
    {
        stMppChnRGN.enModId  = HI_ID_RGN;
        stMppChnRGN.s32DevId = 0;
        stMppChnRGN.s32ChnId = 0;

        if(0 == (i%2))
        {
            pcMmzName = NULL;  
        }
        else
        {
            pcMmzName = "ddr1";
        }

        s32Ret = HI_MPI_SYS_SetMemConf(&stMppChnRGN,pcMmzName);
        if (s32Ret)
        {
            SAMPLE_PRT("HI_MPI_SYS_SetMemConf ERR !\n");
            return HI_FAILURE;
        }
    }
    
    return HI_SUCCESS;
}

/******************************************************************************
* funciton : osd region change position
******************************************************************************/
HI_S32 SAMPLE_RGN_ChgPosition(RGN_HANDLE RgnHandle, VENC_GRP VencGrp, POINT_S *pstPoint)
{
    MPP_CHN_S stChn;
    RGN_CHN_ATTR_S stChnAttr;
    HI_S32 s32Ret;

    stChn.enModId = HI_ID_GROUP;
    stChn.s32DevId = VencGrp;
    stChn.s32ChnId = 0;

    if (NULL == pstPoint)
    {
        SAMPLE_PRT("input parameter is null. it is invaild!\n");
        return HI_FAILURE;
    }
        
    s32Ret = HI_MPI_RGN_GetDisplayAttr(RgnHandle, &stChn, &stChnAttr);
    if(HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_RGN_GetDisplayAttr (%d)) failed with %#x!\n",\
               RgnHandle, s32Ret);
        return HI_FAILURE;
    }

    stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = pstPoint->s32X;
    stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = pstPoint->s32Y;
    s32Ret = HI_MPI_RGN_SetDisplayAttr(RgnHandle,&stChn,&stChnAttr);
    if(HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_RGN_SetDisplayAttr (%d)) failed with %#x!\n",\
               RgnHandle, s32Ret);
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}

/******************************************************************************
* funciton : osd region show or hide
******************************************************************************/
HI_S32 SAMPLE_RGN_ShowOrHide(RGN_HANDLE RgnHandle, VENC_GRP VencGrp, HI_BOOL bShow)
{
    MPP_CHN_S stChn;
    RGN_CHN_ATTR_S stChnAttr;
    HI_S32 s32Ret;

    stChn.enModId = HI_ID_GROUP;
    stChn.s32DevId = VencGrp;
    stChn.s32ChnId = 0;
        
    s32Ret = HI_MPI_RGN_GetDisplayAttr(RgnHandle, &stChn, &stChnAttr);
    if(HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_RGN_GetDisplayAttr (%d)) failed with %#x!\n",\
               RgnHandle, s32Ret);
        return HI_FAILURE;
    }

    stChnAttr.bShow = bShow;
    
    s32Ret = HI_MPI_RGN_SetDisplayAttr(RgnHandle,&stChn,&stChnAttr);
    if(HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_RGN_SetDisplayAttr (%d)) failed with %#x!\n",\
               RgnHandle, s32Ret);
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}
    
/******************************************************************************
* funciton : osd region change color
******************************************************************************/
HI_S32 SAMPLE_RGN_ChgColor(RGN_HANDLE RgnHandle, HI_U32 u32Color)
{
    RGN_ATTR_S stRgnAttr;
    HI_S32 s32Ret;

    s32Ret = HI_MPI_RGN_GetAttr(RgnHandle,&stRgnAttr);
    if(HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_RGN_GetAttr (%d)) failed with %#x!\n",\
               RgnHandle, s32Ret);
        return HI_FAILURE;
    }

    stRgnAttr.unAttr.stOverlay.u32BgColor = u32Color;

    s32Ret = HI_MPI_RGN_SetAttr(RgnHandle,&stRgnAttr);
    if(HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_RGN_SetAttr (%d)) failed with %#x!\n",\
               RgnHandle, s32Ret);
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}
/******************************************************************************
* funciton : osd region change (bgAlpha, fgAlpha, layer)
******************************************************************************/
HI_S32 SAMPLE_RGN_Change(RGN_HANDLE RgnHandle, VENC_GRP VencGrp, SAMPLE_RGN_CHANGE_TYPE_EN enChangeType, HI_U32 u32Val)
{
    MPP_CHN_S stChn;
    RGN_CHN_ATTR_S stChnAttr;
    HI_S32 s32Ret;

    stChn.enModId = HI_ID_GROUP;
    stChn.s32DevId = VencGrp;
    stChn.s32ChnId = 0;
    s32Ret = HI_MPI_RGN_GetDisplayAttr(RgnHandle,&stChn,&stChnAttr);
    if(HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_RGN_GetDisplayAttr (%d)) failed with %#x!\n",\
               RgnHandle, s32Ret);
        return HI_FAILURE;
    }

    switch (enChangeType)
    {
        case RGN_CHANGE_TYPE_FGALPHA:
            stChnAttr.unChnAttr.stOverlayChn.u32FgAlpha = u32Val;
            break;
        case RGN_CHANGE_TYPE_BGALPHA:
            stChnAttr.unChnAttr.stOverlayChn.u32BgAlpha = u32Val;
            break;
        case RGN_CHANGE_TYPE_LAYER:
            stChnAttr.unChnAttr.stOverlayChn.u32Layer = u32Val;
            break;
        default:
            SAMPLE_PRT("input paramter invaild!\n");
            return HI_FAILURE;
    }
    s32Ret = HI_MPI_RGN_SetDisplayAttr(RgnHandle,&stChn,&stChnAttr);
    if(HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_RGN_SetDisplayAttr (%d)) failed with %#x!\n",\
               RgnHandle, s32Ret);
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}
 
/******************************************************************************
* funciton : load bmp from file
******************************************************************************/
HI_S32 SAMPLE_RGN_LoadBmp(const HI_CHAR *filename, BITMAP_S *pstBitmap)
{
    OSD_SURFACE_S Surface;
    OSD_BITMAPFILEHEADER bmpFileHeader;
    OSD_BITMAPINFO bmpInfo;

    if(GetBmpInfo(filename,&bmpFileHeader,&bmpInfo) < 0)
    {
		SAMPLE_PRT("GetBmpInfo err!\n");
        return HI_FAILURE;
    }

    Surface.enColorFmt = OSD_COLOR_FMT_RGB1555;
    
    pstBitmap->pData = malloc(2*(bmpInfo.bmiHeader.biWidth)*(bmpInfo.bmiHeader.biHeight));
	
    if(NULL == pstBitmap->pData)
    {
        SAMPLE_PRT("malloc osd memroy err!\n");        
        return HI_FAILURE;
    }

    CreateSurfaceByBitMap(filename,&Surface,(HI_U8*)(pstBitmap->pData));
	
    pstBitmap->u32Width = Surface.u16Width;
    pstBitmap->u32Height = Surface.u16Height;
    pstBitmap->enPixelFormat = PIXEL_FORMAT_RGB_1555;		 
    return HI_SUCCESS;
}

#define START_POINT_X_OFFSET 64
#define START_POINT_Y_OFFSET 64

/******************************************************************************
  function : overlay process                
             1) create some overlay regions
             2) display overlay regions ( One Region -- Multi-VencGroup )
             3) change all vencGroups Regions' Layer
             4) change all vencGroups Regions' position
             5) change all vencGroups Regions' color
             6) load bmp form bmp-file to Region-0 
             7) change all vencGroups Regions' front alpha 
             8) change all vencGroups Regions' backgroud alpha
             9) update bitmap(not support now)
             10) show or hide overlay regions
             11) Detach overlay regions from chn
             12) Detroy overlay regions
******************************************************************************/
HI_S32 SAMPLE_RGN_OverlayProcess(VENC_GRP VencGrpStart,HI_S32 grpcnt)
{
    HI_S32 i, j;
    HI_S32 s32Ret = HI_FAILURE;
    RGN_HANDLE RgnHandle;
    RGN_ATTR_S stRgnAttr;
    MPP_CHN_S stChn;
    VENC_GRP VencGrp;
    RGN_CHN_ATTR_S stChnAttr;
    HI_U32 u32Layer;
    HI_U32 u32Color;
    HI_U32 u32Alpha;
    POINT_S stPoint;
    BITMAP_S stBitmap;
    SAMPLE_RGN_CHANGE_TYPE_EN enChangeType;
    HI_BOOL bShow = HI_FALSE;

    /****************************************
     step 1: create overlay regions
    ****************************************/
    for (i=0; i<gs_s32RgnCnt; i++)
    {
        stRgnAttr.enType = OVERLAY_RGN;
        stRgnAttr.unAttr.stOverlay.enPixelFmt = PIXEL_FORMAT_RGB_1555;
        stRgnAttr.unAttr.stOverlay.stSize.u32Width  = 180;
        stRgnAttr.unAttr.stOverlay.stSize.u32Height = 144;
        stRgnAttr.unAttr.stOverlay.u32BgColor = 0x7c00*(i%2) + ((i+1)%2)*0x1f;

        RgnHandle = i;

        s32Ret = HI_MPI_RGN_Create(RgnHandle, &stRgnAttr);
        if(HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("HI_MPI_RGN_Create (%d) failed with %#x!\n", \
                   RgnHandle, s32Ret);
            HI_MPI_RGN_Destroy(RgnHandle);
            return HI_FAILURE;
        }
        gs_s32RgnCntCur ++;
        SAMPLE_PRT("the handle:%d,creat success!\n",RgnHandle);
    }
    
    /*********************************************
     step 2: display overlay regions to venc groups
    *********************************************/
    for (i=0; i<gs_s32RgnCnt; i++)
    {
        RgnHandle = i;

        for (j=0; j<gs_s32ChnCnt; j++)
        {
            VencGrp = j+VencGrpStart;
            stChn.enModId = HI_ID_GROUP;
            stChn.s32DevId = VencGrp;
            stChn.s32ChnId = 0;
            
            memset(&stChnAttr,0,sizeof(stChnAttr));
            stChnAttr.bShow = HI_TRUE;
            stChnAttr.enType = OVERLAY_RGN;
            stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X =(i%3) * 200 + START_POINT_X_OFFSET;
            stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y =(i/3)*160 + START_POINT_Y_OFFSET;
            stChnAttr.unChnAttr.stOverlayChn.u32BgAlpha = 128;
            stChnAttr.unChnAttr.stOverlayChn.u32FgAlpha = 128;
            stChnAttr.unChnAttr.stOverlayChn.u32Layer = i;

            stChnAttr.unChnAttr.stOverlayChn.stQpInfo.bAbsQp = HI_FALSE;
            stChnAttr.unChnAttr.stOverlayChn.stQpInfo.s32Qp  = 0;

            s32Ret = HI_MPI_RGN_AttachToChn(RgnHandle, &stChn, &stChnAttr);
            if(HI_SUCCESS != s32Ret)
            {
                SAMPLE_PRT("HI_MPI_RGN_AttachToChn (%d) failed with %#x!\n",\
                       RgnHandle, s32Ret);
                return HI_FAILURE;
            }
        }
    }
    
    usleep(SAMPLE_RGN_SLEEP_TIME*5);
    printf("display region to chn success!\n");

    /*********************************************
     step 3: change overlay regions' position
    *********************************************/
    RgnHandle = 1;

    for (i=0; i<gs_s32ChnCnt; i++)
    {
        VencGrp = i+VencGrpStart;
        stPoint.s32X = 60 + START_POINT_X_OFFSET;
        stPoint.s32Y = 0 + START_POINT_Y_OFFSET;
        s32Ret = SAMPLE_RGN_ChgPosition(RgnHandle, VencGrp, &stPoint);
        if(HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("change region(%d) position failed with %#x!\n",\
                   RgnHandle, s32Ret);
            return HI_FAILURE;
        }
    }
    
    printf("handle:%d,change point success,new point(x:%d,y:%d) !\n", 
           RgnHandle,stPoint.s32X,stPoint.s32Y);
    usleep(SAMPLE_RGN_SLEEP_TIME*5);

    /*********************************************
     step 4: change layer
    *********************************************/
    RgnHandle = 0;
    enChangeType = RGN_CHANGE_TYPE_LAYER;

    for (i=0; i<gs_s32ChnCnt; i++)
    {
        VencGrp = i+VencGrpStart;
        u32Layer = 2;
        s32Ret = SAMPLE_RGN_Change(RgnHandle, VencGrp, enChangeType, u32Layer);
        if(HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("change region(%d) layer failed with %#x!\n",\
                   RgnHandle, s32Ret);
            return HI_FAILURE;
        }
    }

    printf("handle:%d,change layer success,new layer(%d) !\n",RgnHandle,u32Layer);

    usleep(SAMPLE_RGN_SLEEP_TIME*5);

    /*********************************************
     step 5: change color
    *********************************************/
    RgnHandle = 2;

    u32Color = 0x7fff;
    s32Ret = SAMPLE_RGN_ChgColor(RgnHandle, u32Color);
    if(HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("change region(%d) color failed with %#x!\n",\
               RgnHandle, s32Ret);
        return HI_FAILURE;
    }

    printf("handle:%d,change color success,new bg color(0x%x)\n",RgnHandle,u32Color);

    usleep(SAMPLE_RGN_SLEEP_TIME*5);

    /*********************************************
     step 6: show bitmap
    *********************************************/
    RgnHandle = 0;
    
    s32Ret = SAMPLE_RGN_LoadBmp("mm.bmp", &stBitmap);
    if(HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("load bmp failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_RGN_SetBitMap(RgnHandle,&stBitmap);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("HI_MPI_RGN_SetBitMap failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }
    
    if (NULL != stBitmap.pData)
    {
        free(stBitmap.pData);
        stBitmap.pData = NULL;
    }

    usleep(SAMPLE_RGN_SLEEP_TIME*5);
    printf("handle:%d,load bmp success!\n",RgnHandle);
    
    /*********************************************
     step 7: change front alpha
    *********************************************/
    RgnHandle = 0;
    enChangeType = RGN_CHANGE_TYPE_FGALPHA;

    for (i=0; i<gs_s32ChnCnt; i++)
    {
        VencGrp = i+VencGrpStart;
        u32Alpha = 32;
        s32Ret = SAMPLE_RGN_Change(RgnHandle, VencGrp, enChangeType, u32Alpha);
        if(HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("change region(%d) f-alpha failed with %#x!\n",\
                   RgnHandle, s32Ret);
            return HI_FAILURE;
        }
    }

    printf("handle:%d,change front alpha success,the new alpha:%d\n", RgnHandle,u32Alpha);

    usleep(SAMPLE_RGN_SLEEP_TIME*5);
    
    /*********************************************
     step 8: change backgroud alpha
    *********************************************/
    RgnHandle = 0;
    enChangeType = RGN_CHANGE_TYPE_BGALPHA;

    for (i=0; i<gs_s32ChnCnt; i++)
    {
        VencGrp = i+VencGrpStart;
        u32Alpha = 32;
        s32Ret = SAMPLE_RGN_Change(RgnHandle, VencGrp, enChangeType, u32Alpha);
        if(HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("change region(%d) f-alpha failed with %#x!\n",\
                   RgnHandle, s32Ret);
            return HI_FAILURE;
        }
    }

    printf("handle:%d,change backgroud alpha success,the new alpha:%d\n", RgnHandle,u32Alpha);

    usleep(SAMPLE_RGN_SLEEP_TIME*5);

    /*********************************************
     step 9: update bitmap
    *********************************************/
    /*not support now*/

    /*********************************************
     step 10: show or hide overlay regions
    *********************************************/
    RgnHandle = 4;
    bShow = HI_FALSE;
    
    for (i=0; i<SAMPLE_RGN_LOOP_COUNT; i++)
    {
        for (j=0; j<gs_s32ChnCnt; j++)
        {
            VencGrp = j+VencGrpStart;
            
            s32Ret = SAMPLE_RGN_ShowOrHide(RgnHandle, VencGrp, bShow);
            if(HI_SUCCESS != s32Ret)
            {
                printf("region(%d) show failed with %#x!\n",\
                       RgnHandle, s32Ret);
                return HI_FAILURE;
            }
        }

        bShow = !bShow;

        usleep(SAMPLE_RGN_SLEEP_TIME*5);
    }

    printf("handle:%d,show or hide osd success\n", RgnHandle);

    /*********************************************
     step 11: Detach osd from chn
    *********************************************/   
    for (i=0; i<gs_s32RgnCnt; i++)
    {
        RgnHandle = i;

        for (j=0; j<gs_s32ChnCnt; j++)
        {
            VencGrp = j+VencGrpStart;
            stChn.enModId = HI_ID_GROUP;
            stChn.s32DevId = VencGrp;
            stChn.s32ChnId = 0;

            s32Ret = HI_MPI_RGN_DetachFrmChn(RgnHandle, &stChn);
            if(HI_SUCCESS != s32Ret)
            {
                SAMPLE_PRT("HI_MPI_RGN_DetachFrmChn (%d) failed with %#x!\n",\
                       RgnHandle, s32Ret);
                return HI_FAILURE;
            }
        }

         printf("Detach handle:%d from chn success\n", RgnHandle);

         usleep(SAMPLE_RGN_SLEEP_TIME*5);
    }

    /*********************************************
     step 12: destory region
    *********************************************/
    for (i=0; i<gs_s32RgnCnt; i++)
    {
        RgnHandle = i;
        s32Ret = HI_MPI_RGN_Destroy(RgnHandle);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("HI_MPI_RGN_Destroy [%d] failed with %#x\n",\
                    RgnHandle, s32Ret);
        }
    }
    SAMPLE_PRT("destory all region success!\n");
    return HI_SUCCESS;
}


/******************************************************************************
  function : cover process                
             1) enable vpp, but disable IE
             2) create an cover region and attach it to vi chn0
             3) create an overlay region and attach it to venc group chn0
             4) change the cover's position,size, color and layer
             5) change the overlay's position and layer
             6) change the overlay's alpha (front and backgroud) 
             7) hide the cover and the overlay
             8) release resource
******************************************************************************/
HI_S32 SAMPLE_RGN_CoverProcess(VI_DEV ViDev,VI_CHN ViChn)
{
    HI_S32 s32Ret = HI_FAILURE;
    
    RGN_HANDLE coverHandle;
    RGN_ATTR_S stCoverAttr;
    MPP_CHN_S stCoverChn;
    RGN_CHN_ATTR_S stCoverChnAttr;
    
    /*******************************************************
     step 2: create an cover region and attach it to vi chn0
    ********************************************************/ 
   
    coverHandle = 0;
    stCoverAttr.enType = COVER_RGN;
    s32Ret = HI_MPI_RGN_Create(coverHandle, &stCoverAttr);
    if(HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    stCoverChn.enModId = HI_ID_VIU;
    stCoverChn.s32ChnId = ViChn;
    stCoverChn.s32DevId = ViDev;

    stCoverChnAttr.bShow = HI_TRUE;
    stCoverChnAttr.enType = COVER_RGN;
    stCoverChnAttr.unChnAttr.stCoverChn.stRect.s32X = 12;
    stCoverChnAttr.unChnAttr.stCoverChn.stRect.s32Y = 12;
    stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Width = 160;
    stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Height = 160;
    stCoverChnAttr.unChnAttr.stCoverChn.u32Color = 0xff;
    stCoverChnAttr.unChnAttr.stCoverChn.u32Layer = 0;       
    s32Ret = HI_MPI_RGN_AttachToChn(coverHandle, &stCoverChn, &stCoverChnAttr);
    if(HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
       goto AttachCover_failed;
    }
    
    //printf("create an cover region and attach it to vi chn0\n");
    usleep(SAMPLE_RGN_SLEEP_TIME*50);
    
    /**********************************************************
      step 4: change the cover's position, size, color and layer
     **********************************************************/
    stCoverChnAttr.unChnAttr.stCoverChn.stRect.s32X = 64;
    stCoverChnAttr.unChnAttr.stCoverChn.stRect.s32Y = 64;
    stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Width = 260;
    stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Height = 260;
    stCoverChnAttr.unChnAttr.stCoverChn.u32Color = 0xf800;
    stCoverChnAttr.unChnAttr.stCoverChn.u32Layer = 1;
    s32Ret = HI_MPI_RGN_SetDisplayAttr(coverHandle, &stCoverChn, &stCoverChnAttr);
    if(HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        goto exit;
    }
    
    usleep(SAMPLE_RGN_SLEEP_TIME*30);
    
   /*********************************************
     step 6: hide the cover and the overlay
    *********************************************/
    stCoverChnAttr.bShow = HI_FALSE;
    s32Ret = HI_MPI_RGN_SetDisplayAttr(coverHandle, &stCoverChn, &stCoverChnAttr);
    if(HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        goto exit;
    }
    
    //printf("hide the cover and the overlay success\n");
    usleep(SAMPLE_RGN_SLEEP_TIME*30);

   /*********************************************
     step 7: release resource
    *********************************************/

exit:
    HI_MPI_RGN_DetachFrmChn(coverHandle, &stCoverChn);  
AttachCover_failed:
    HI_MPI_RGN_Destroy(coverHandle);
    return s32Ret;
}

HI_S32 SAMPLE_RGN_OverlayExProcess(VI_DEV ViDev,VI_CHN ViChn)
{
    HI_S32 s32Ret = HI_FAILURE;
    
    RGN_HANDLE OverlayExHandle = 0;
    RGN_ATTR_S stOverlayExAttr;
    MPP_CHN_S stOverlayExChn;
    RGN_CHN_ATTR_S stOverlayExChnAttr;
	BITMAP_S stBitmap;
	
	stOverlayExAttr.enType = OVERLAYEX_RGN;
	stOverlayExAttr.unAttr.stOverlayEx.enPixelFmt = PIXEL_FORMAT_RGB_1555;
	stOverlayExAttr.unAttr.stOverlayEx.u32BgColor =  0x00ffffff;
	stOverlayExAttr.unAttr.stOverlayEx.stSize.u32Height = 128;
	stOverlayExAttr.unAttr.stOverlayEx.stSize.u32Width= 128;
	
    s32Ret = HI_MPI_RGN_Create(OverlayExHandle, &stOverlayExAttr);
    if(HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_RGN_Create failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    stOverlayExChnAttr.enType = OVERLAYEX_RGN;
	stOverlayExChnAttr.bShow = HI_TRUE;
	stOverlayExChnAttr.unChnAttr.stOverlayExChn.stPoint.s32X = 128;
	stOverlayExChnAttr.unChnAttr.stOverlayExChn.stPoint.s32Y = 128;
	stOverlayExChnAttr.unChnAttr.stOverlayExChn.u32BgAlpha = 50;
	stOverlayExChnAttr.unChnAttr.stOverlayExChn.u32FgAlpha = 130;
	stOverlayExChnAttr.unChnAttr.stOverlayExChn.u32Layer = 1;

    stOverlayExChn.enModId = HI_ID_VIU;
    stOverlayExChn.s32DevId = ViDev;
    stOverlayExChn.s32ChnId = ViChn;

	s32Ret = HI_MPI_RGN_AttachToChn(OverlayExHandle,&stOverlayExChn,&stOverlayExChnAttr);
    if(HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_RGN_AttachToChn failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }
	usleep(SAMPLE_RGN_SLEEP_TIME*5);
	/*load bitmap*/
	s32Ret = SAMPLE_RGN_LoadBmp("mm.bmp", &stBitmap);
    if(HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("load bmp failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_RGN_SetBitMap(OverlayExHandle,&stBitmap);
    if(s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("HI_MPI_RGN_SetBitMap failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }
    
    if (NULL != stBitmap.pData)
    {
        free(stBitmap.pData);
        stBitmap.pData = NULL;
    }
	usleep(SAMPLE_RGN_SLEEP_TIME*5);
    //printf("handle:%d,load bmp success!\n",OverlayExHandle);

	s32Ret = HI_MPI_RGN_GetDisplayAttr(OverlayExHandle,&stOverlayExChn,&stOverlayExChnAttr);
    if(HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_RGN_GetDisplayAttr (%d)) failed with %#x!\n",\
               OverlayExHandle, s32Ret);
        return HI_FAILURE;
    }

	stOverlayExChnAttr.unChnAttr.stOverlayExChn.u32BgAlpha = 200;
	stOverlayExChnAttr.unChnAttr.stOverlayExChn.u32FgAlpha = 1;
	stOverlayExChnAttr.unChnAttr.stOverlayExChn.stPoint.s32X = 256;
	stOverlayExChnAttr.unChnAttr.stOverlayExChn.stPoint.s32Y = 256;

    s32Ret = HI_MPI_RGN_SetDisplayAttr(OverlayExHandle,&stOverlayExChn,&stOverlayExChnAttr);
    if(HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_RGN_SetDisplayAttr (%d)) failed with %#x!\n",\
               OverlayExHandle, s32Ret);
        return HI_FAILURE;
    }
	usleep(SAMPLE_RGN_SLEEP_TIME*5);

	//printf("handle:%d,will be destroy!\n",OverlayExHandle);
	s32Ret = HI_MPI_RGN_DetachFrmChn(OverlayExHandle,&stOverlayExChn);
    if(HI_SUCCESS != s32Ret)
	{
        SAMPLE_PRT("HI_MPI_RGN_DetachFrmChn (%d)) failed with %#x!\n",\
               OverlayExHandle, s32Ret);
        return HI_FAILURE;
    }
	s32Ret = HI_MPI_RGN_Destroy(OverlayExHandle);
    if(HI_SUCCESS != s32Ret)
	{
        SAMPLE_PRT("HI_MPI_RGN_Destroy (%d)) failed with %#x!\n",\
               OverlayExHandle, s32Ret);
        return HI_FAILURE;
    }
	return HI_SUCCESS;

}

void SAMPLE_RGN_Usage(HI_CHAR *sPrgNm)
{
    //printf("Usage : %s <index>\n", sPrgNm);
    //printf("index:\n");
    printf("press sample command as follows!\n");
    printf("\t 0) D1 Vi cover region + OverlayEx + HD output\n");
    printf("\t 1) MixCap Vi cover + HD output\n");
	printf("\t 2) Overlay region + D1 Venc + HD output\n");
    printf("\t q) exit this sample\n");
    
    return;
}


/******************************************************************************
* function : to process abnormal case                                        
******************************************************************************/
void SAMPLE_RGN_HandleSig(HI_S32 signo)
{
    if (SIGINT == signo || SIGTSTP == signo)
    {
        HI_MPI_RGN_Destroy(gs_s32RgnCntCur);
        SAMPLE_COMM_SYS_Exit();
        printf("\033[0;31mprogram exit abnormally!\033[0;39m\n");
    }

    exit(0);
}


HI_VOID * SAMPLE_RGN_MixCap(HI_VOID * pdata)
{
	SAMPLE_VI_MODE_E enViMode = SAMPLE_VI_MODE_1_D1Cif;
	HI_U32 u32ViChnCnt = 16;
	HI_S32 s32VpssGrpCnt = 16;

	VI_DEV ViDev = 0;
	VI_CHN ViChn = 0;
	
	VB_CONF_S stVbConf;
	VPSS_GRP VpssGrp;
	VPSS_GRP_ATTR_S stGrpAttr;
	
	HI_S32 i;
	HI_S32 s32Ret = HI_SUCCESS;
	HI_U32 u32BlkSize;
	SIZE_S stSize;

	VO_DEV VoDev;
	VO_CHN VoChn;
	VO_PUB_ATTR_S stVoPubAttr; 
	SAMPLE_VO_MODE_E enVoMode;
	HI_U32 u32WndNum;

	/******************************************
	 step  1: init variable 
	******************************************/
	gs_u32ViFrmRate = (VIDEO_ENCODING_MODE_PAL == gs_enNorm)?25:30;

	memset(&stVbConf,0,sizeof(VB_CONF_S));

	/*video buffer*/
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
		goto END_0;
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
	 step 5: start vo
	******************************************/
	VoDev = SAMPLE_VO_DEV_DHD0;
	u32WndNum = 16;
	enVoMode = VO_MODE_9MUX;
	
	if(VIDEO_ENCODING_MODE_PAL == gs_enNorm)
	{
		stVoPubAttr.enIntfSync = VO_OUTPUT_720P50;
	}
	else
	{
		stVoPubAttr.enIntfSync = VO_OUTPUT_720P60;
	}
  
	stVoPubAttr.enIntfType = VO_INTF_VGA;
	stVoPubAttr.u32BgColor = 0x000000ff;
	stVoPubAttr.bDoubleFrame = HI_TRUE;
	s32Ret = SAMPLE_COMM_VO_StartDevLayer(VoDev, &stVoPubAttr, gs_u32ViFrmRate);
	if (HI_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("SAMPLE_COMM_VO_StartDevLayer failed!\n");
		goto END_3;
	}
	
	s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, &stVoPubAttr, enVoMode);
	if (HI_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("SAMPLE_COMM_VO_StartChn failed!\n");
		goto END_4;
	}

	/* if it's displayed on HDMI, we should start HDMI */
	if (stVoPubAttr.enIntfType & VO_INTF_HDMI)
	{
		if (HI_SUCCESS != SAMPLE_COMM_VO_HdmiStart(stVoPubAttr.enIntfSync))
		{
			goto END_4;
		}
	}

	for(i=0;i<u32WndNum;i++)
	{
		VoChn = i;
		VpssGrp = i;
		s32Ret = SAMPLE_COMM_VO_BindVpss(VoDev,VoChn,VpssGrp,VPSS_PRE0_CHN);
		if (HI_SUCCESS != s32Ret)
		{
			SAMPLE_PRT("SAMPLE_COMM_VO_BindVpss failed!\n");
			goto END_4;
		}
	}

	printf("please press any key to wait stop!\n");
	/*Cover Region Process*/
       while(bCase1)
        {
        	ViChn = 0;
        	s32Ret = SAMPLE_RGN_CoverProcess(ViDev,ViChn);
        	if (HI_SUCCESS != s32Ret)
        	{
        		SAMPLE_PRT("cover process failed!\n");
        		goto END_4;
        	}
              
        }
	printf("\nplease press any key to exit\n");
	getchar();
	
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

END_3:	//vi unbind vpss
	SAMPLE_COMM_VI_UnBindVpss(enViMode);
END_2:	//vpss stop
	SAMPLE_COMM_VPSS_Stop(s32VpssGrpCnt, VPSS_MAX_CHN_NUM);
END_1:	//vi stop
	SAMPLE_COMM_VI_Stop(enViMode);
END_0:	//system exit
	SAMPLE_COMM_SYS_Exit();

    if(s32Ret != HI_SUCCESS)
     {
         SAMPLE_RGN_NOT_PASS(s32Ret);
     }
	
	
	return HI_NULL;
	
	

}
HI_S32 SAMPLE_RGN_VENC(HI_VOID)
{
	SAMPLE_VI_MODE_E enViMode = SAMPLE_VI_MODE_1_D1;

	HI_U32 u32ViChnCnt = 1;
	HI_S32 s32VpssGrpCnt = 1;
	PIC_SIZE_E enSize = PIC_D1;
	
    HI_S32 s32RgnCnt = 8;
	
	VB_CONF_S stVbConf;
	VPSS_GRP VpssGrp;
	VPSS_CHN VpssChn;
	VPSS_GRP_ATTR_S stGrpAttr;

	PAYLOAD_TYPE_E enPayLoad = PT_H264;
	VENC_GRP VencGrp;
    VENC_CHN VencChn;
    SAMPLE_RC_E enRcMode;
	
	HI_S32 i;
	HI_S32 s32Ret = HI_SUCCESS;
	HI_U32 u32BlkSize;
	SIZE_S stSize;

	VO_DEV VoDev;
	VO_CHN VoChn;
	VO_PUB_ATTR_S stVoPubAttr; 
	SAMPLE_VO_MODE_E enVoMode;
	HI_U32 u32WndNum;

	/******************************************
	 step  1: init variable 
	******************************************/
	gs_u32ViFrmRate = (VIDEO_ENCODING_MODE_PAL == gs_enNorm)?25:30;

	memset(&stVbConf,0,sizeof(VB_CONF_S));

	u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm,\
				PIC_D1, SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH);
	stVbConf.u32MaxPoolCnt = 128;

	/*video buffer*/
	stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
	stVbConf.astCommPool[0].u32BlkCnt = u32ViChnCnt * 10;
	memset(stVbConf.astCommPool[0].acMmzName,0,
		sizeof(stVbConf.astCommPool[0].acMmzName));
	
    /* hist buf*/
    stVbConf.astCommPool[1].u32BlkSize = (196*4);
    stVbConf.astCommPool[1].u32BlkCnt = u32ViChnCnt * 10;
    memset(stVbConf.astCommPool[1].acMmzName,0,
        sizeof(stVbConf.astCommPool[1].acMmzName));

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
	s32Ret = SAMPLE_COMM_VI_Start(enViMode, gs_enNorm);
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
		goto END_0;
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

	s32Ret = SAMPLE_COMM_VPSS_Start(s32VpssGrpCnt, &stSize, VPSS_MAX_CHN_NUM,NULL);
	if (HI_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("Start Vpss failed!\n");
		goto END_1;
	}

	s32Ret = SAMPLE_COMM_VI_BindVpss(enViMode);
	if (HI_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("Vi bind Vpss failed!\n");
		goto END_2;
	}
	/******************************************
	 step 5: start vo
	******************************************/
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
  
	stVoPubAttr.enIntfType = VO_INTF_VGA;
	stVoPubAttr.u32BgColor = 0x000000ff;
	stVoPubAttr.bDoubleFrame = HI_TRUE;
	s32Ret = SAMPLE_COMM_VO_StartDevLayer(VoDev, &stVoPubAttr, gs_u32ViFrmRate);
	if (HI_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("SAMPLE_COMM_VO_StartDevLayer failed!\n");
		goto END_3;
	}
	
	s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, &stVoPubAttr, enVoMode);
	if (HI_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("SAMPLE_COMM_VO_StartChn failed!\n");
		goto END_4;
	}

	/* if it's displayed on HDMI, we should start HDMI */
	if (stVoPubAttr.enIntfType & VO_INTF_HDMI)
	{
		if (HI_SUCCESS != SAMPLE_COMM_VO_HdmiStart(stVoPubAttr.enIntfSync))
		{
			goto END_4;
		}
	}

	for(i=0;i<u32WndNum;i++)
	{
		VoChn = i;
		VpssGrp = i;
		s32Ret = SAMPLE_COMM_VO_BindVpss(VoDev,VoChn,VpssGrp,VPSS_PRE0_CHN);
		if (HI_SUCCESS != s32Ret)
		{
			SAMPLE_PRT("SAMPLE_COMM_VO_BindVpss failed!\n");
			goto END_4;
		}
	}

	/******************************************
     step 6: start stream venc
    ******************************************/
    enRcMode = SAMPLE_RC_CBR;

    for (i=0; i<u32ViChnCnt; i++)
    {
        /*** main frame **/
        VencGrp = i;
        VencChn = i;
        VpssGrp = i;
        s32Ret = SAMPLE_COMM_VENC_Start(VencGrp, VencChn, enPayLoad,\
                                    gs_enNorm, enSize, enRcMode);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start Venc failed!\n");
            goto END_4;
        }

        s32Ret = SAMPLE_COMM_VENC_BindVpss(VencGrp, VpssGrp, VPSS_BSTR_CHN);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("Start Venc failed!\n");
            goto END_5;
        }
    }
    /******************************************
     step 6: stream venc process -- get stream, then save it to file. 
    ******************************************/
    s32Ret = SAMPLE_COMM_VENC_StartGetStream(u32ViChnCnt);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start Venc failed!\n");
        goto END_5;
    }

	/*Overlay Region Process*/
	s32Ret = SAMPLE_RGN_OverlayProcess(VencChn, s32RgnCnt);
	if (HI_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("overlay process failed!\n");
		goto END_5;
	}
	
	printf("please press any key to exit\n");
	//getchar();
    /******************************************
     step 8: exit process
    ******************************************/
    s32Ret = SAMPLE_COMM_VENC_StopGetStream();
	if (HI_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("SAMPLE_COMM_VENC_StopGetStream failed!\n");
		goto END_5;
	}
	
END_5:
    for (i=0; i<u32ViChnCnt; i++)
    {
        VencGrp = i;
        VencChn = i;
        VpssGrp = i;
        VpssChn =VPSS_BSTR_CHN;
        SAMPLE_COMM_VENC_UnBindVpss(VencGrp, VpssGrp, VpssChn);
        SAMPLE_COMM_VENC_Stop(VencGrp,VencChn);
    }
    SAMPLE_COMM_VI_UnBindVpss(enViMode);	
END_4:
	if (stVoPubAttr.enIntfType & VO_INTF_HDMI)
	{
		SAMPLE_COMM_VO_HdmiStop();
	}
	VoDev = SAMPLE_VO_DEV_DHD0;
	u32WndNum = 16;
	enVoMode = VO_MODE_1MUX;
	for(i=0;i<u32WndNum;i++)
	{
		VoChn = i;
		VpssGrp = i;
		SAMPLE_COMM_VO_UnBindVpss(VoDev,VoChn,VpssGrp,VPSS_PRE0_CHN);
	}
	SAMPLE_COMM_VO_StopChn(VoDev, enVoMode);
	SAMPLE_COMM_VO_StopDevLayer(VoDev);

END_3:	//vi unbind vpss
	SAMPLE_COMM_VI_UnBindVpss(enViMode);
END_2:	//vpss stop
	SAMPLE_COMM_VPSS_Stop(s32VpssGrpCnt, VPSS_MAX_CHN_NUM);
END_1:	//vi stop
	SAMPLE_COMM_VI_Stop(enViMode);
END_0:	//system exit
	SAMPLE_COMM_SYS_Exit();
	
	return s32Ret;
	
}

HI_VOID * SAMPLE_RGN_D1(HI_VOID *pdata)
{
    SAMPLE_VI_MODE_E enViMode = SAMPLE_VI_MODE_1_D1;

    HI_U32 u32ViChnCnt = 1;
    HI_S32 s32VpssGrpCnt = 1;
    VI_DEV ViDev = 0;
    VI_CHN ViChn = 0;
    
    VB_CONF_S stVbConf;
    VPSS_GRP VpssGrp;
    VPSS_GRP_ATTR_S stGrpAttr;
    
    HI_S32 i;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32BlkSize;
    SIZE_S stSize;

    VO_DEV VoDev;
    VO_CHN VoChn;
    VO_PUB_ATTR_S stVoPubAttr; 
    SAMPLE_VO_MODE_E enVoMode;
    HI_U32 u32WndNum;

    /******************************************
     step  1: init variable 
    ******************************************/
    gs_u32ViFrmRate = (VIDEO_ENCODING_MODE_PAL == gs_enNorm)?25:30;

    memset(&stVbConf,0,sizeof(VB_CONF_S));

    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm,\
                PIC_D1, SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH);
    stVbConf.u32MaxPoolCnt = 128;

    /*video buffer*/
    stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt = u32ViChnCnt * 10;
    memset(stVbConf.astCommPool[0].acMmzName,0,
        sizeof(stVbConf.astCommPool[0].acMmzName));
	/* hist buf*/
    stVbConf.astCommPool[1].u32BlkSize = (196*4);
    stVbConf.astCommPool[1].u32BlkCnt = u32ViChnCnt * 10;
    memset(stVbConf.astCommPool[1].acMmzName,0,
        sizeof(stVbConf.astCommPool[1].acMmzName));

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
    s32Ret = SAMPLE_COMM_VI_Start(enViMode, gs_enNorm);
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
        goto END_0;
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

    s32Ret = SAMPLE_COMM_VPSS_Start(s32VpssGrpCnt, &stSize, VPSS_MAX_CHN_NUM,NULL);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start Vpss failed!\n");
        goto END_1;
    }

    s32Ret = SAMPLE_COMM_VI_BindVpss(enViMode);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Vi bind Vpss failed!\n");
        goto END_2;
    }
    /******************************************
     step 5: start vo
    ******************************************/
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
  
    stVoPubAttr.enIntfType = VO_INTF_VGA;
    stVoPubAttr.u32BgColor = 0x000000ff;
    stVoPubAttr.bDoubleFrame = HI_TRUE;
    s32Ret = SAMPLE_COMM_VO_StartDevLayer(VoDev, &stVoPubAttr, gs_u32ViFrmRate);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartDevLayer failed!\n");
        goto END_3;
    }
    
    s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, &stVoPubAttr, enVoMode);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartChn failed!\n");
        goto END_4;
    }

    /* if it's displayed on HDMI, we should start HDMI */
    if (stVoPubAttr.enIntfType & VO_INTF_HDMI)
    {
        if (HI_SUCCESS != SAMPLE_COMM_VO_HdmiStart(stVoPubAttr.enIntfSync))
        {
            goto END_4;
        }
    }

    for(i=0;i<u32WndNum;i++)
    {
        VoChn = i;
        VpssGrp = i;
        s32Ret = SAMPLE_COMM_VO_BindVpss(VoDev,VoChn,VpssGrp,VPSS_PRE0_CHN);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("SAMPLE_COMM_VO_BindVpss failed!\n");
            goto END_4;
        }
    }

    printf("please press any key to wait stop!\n");
     /******************************************
      step 5: start region
     ******************************************/
      while(bCase0)
     {  	
        
            /*Cover Region Process*/
          s32Ret = SAMPLE_RGN_CoverProcess(ViDev,ViChn);
          if (HI_SUCCESS != s32Ret)
          {
          	SAMPLE_PRT("cover process failed!\n");
          	goto END_4;
          }
          /*OverlayEx Region Process*/
          s32Ret = SAMPLE_RGN_OverlayExProcess(ViDev,ViChn);
          if (HI_SUCCESS != s32Ret)
          {
          	SAMPLE_PRT("overlayex process failed!\n");
          	goto END_4;
          }
      }
    printf("\n please press any key to exit!\n");
	
	getchar();
	
END_4:
	if (stVoPubAttr.enIntfType & VO_INTF_HDMI)
	{
		SAMPLE_COMM_VO_HdmiStop();
	}
	VoDev = SAMPLE_VO_DEV_DHD0;
	u32WndNum = 16;
	enVoMode = VO_MODE_1MUX;
	for(i=0;i<u32WndNum;i++)
	{
		VoChn = i;
		VpssGrp = i;
		SAMPLE_COMM_VO_UnBindVpss(VoDev,VoChn,VpssGrp,VPSS_PRE0_CHN);
	}
	SAMPLE_COMM_VO_StopChn(VoDev, enVoMode);
	SAMPLE_COMM_VO_StopDevLayer(VoDev);

END_3:	//vi unbind vpss
	SAMPLE_COMM_VI_UnBindVpss(enViMode);
END_2:	//vpss stop
	SAMPLE_COMM_VPSS_Stop(s32VpssGrpCnt, VPSS_MAX_CHN_NUM);
END_1:	//vi stop
	SAMPLE_COMM_VI_Stop(enViMode);
END_0:	//system exit
	SAMPLE_COMM_SYS_Exit();

    if(s32Ret != HI_SUCCESS)
     {
         SAMPLE_RGN_NOT_PASS(s32Ret);
     }
	
	return HI_NULL;

}

/******************************************************************************
* function    : main()
* Description : region
******************************************************************************/
int main(int argc, char *argv[])
{
    HI_S32 s32Ret;
    HI_S32 s32Tm = 2;
    pthread_t SampRgnThread[3];
    if ( (argc < 2) || (1 != strlen(argv[1])))
    {
        SAMPLE_RGN_Usage(argv[0]);
        return HI_FAILURE;
    }

    signal(SIGINT, SAMPLE_RGN_HandleSig);
    signal(SIGTERM, SAMPLE_RGN_HandleSig);

    switch (*argv[1])
    {
        case '0':/* VI: D1 + Cover+ OverlayEx  */
            s32Tm = 0;
            bCase0 = HI_TRUE;
            pthread_create(&SampRgnThread[0], 0, SAMPLE_RGN_D1, (HI_VOID *)HI_NULL);
            //s32Ret = SAMPLE_RGN_D1();
            break;
        case '1':/* Mix Cap + Cover */
            s32Tm = 1;
            bCase1 = HI_TRUE;
            pthread_create(&SampRgnThread[1], 0, SAMPLE_RGN_MixCap, (HI_VOID *)HI_NULL);
            //s32Ret = SAMPLE_RGN_MixCap();
            break;
	 case '2':/* VI: D1 + Overlay(venc) */
	     s32Ret = SAMPLE_RGN_VENC();
			break;

        default:
            SAMPLE_RGN_Usage(argv[0]);
            return HI_FAILURE;
    }

    getchar();   
    

    bCase0= HI_FALSE;
    bCase1= HI_FALSE;

    if(0 == s32Tm || 1 == s32Tm)
    {
        fprintf(stderr,"please wait ,program is exiting...");
        pthread_join(SampRgnThread[s32Tm], 0);
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
