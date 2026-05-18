#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>

#if 0
#include "hi_common.h"
#include "hi_comm_vi.h"
#include "hi_comm_video.h"
#include "hi_comm_sys.h"

#include "mpi_vb.h"
#include "mpi_sys.h"
#include "mpi_vi.h"
#endif

#include "mpi_ive.h"
#include "hi_comm_ive.h"

#include "sample_comm.h"


#define IVECHARCALH 8
#define IVECHARCALW 8
#define IVECHARNUM IVECHARCALW*IVECHARCALH

#define CLIP(a, maxv, minv)      (((a)>(maxv)) ? (maxv) : (((a) < (minv)) ? (minv) : (a)))

VIDEO_NORM_E gs_enNorm = VIDEO_ENCODING_MODE_PAL;
HI_U32 gs_u32ViFrmRate = 0; 

HI_S32   SAMPLE_IVE_MST_INIT_MPI()
{
    HI_S32 sRet = HI_FAILURE;
    VB_CONF_S       stVbConf;
    MPP_SYS_CONF_S   stSysConf;
	HI_BOOL bMpiInit=HI_FALSE;
    if(HI_TRUE == bMpiInit)
    {
        printf("MPI has been inited \n ");
        return sRet;
    }
    /*初始化之前先确定系统已退出*/
    HI_MPI_SYS_Exit();
    HI_MPI_VB_Exit();

    /*VB初始化之前先配置VB*/
    memset(&stVbConf,0,sizeof(VB_CONF_S));

    /*1080p*/
    stVbConf.u32MaxPoolCnt = 64;
    stVbConf.astCommPool[0].u32BlkSize   = 1920*1088*2;
    stVbConf.astCommPool[0].u32BlkCnt    = 0;

     /*720p*/

    stVbConf.astCommPool[1].u32BlkSize   = 1280*720*2;
    stVbConf.astCommPool[1].u32BlkCnt    = 0;


     /*D1*/

    stVbConf.astCommPool[2].u32BlkSize   = 768*576*2;
    stVbConf.astCommPool[2].u32BlkCnt    = 0;


     /*Cif*/

    stVbConf.astCommPool[3].u32BlkSize   = 384*288*2;
    stVbConf.astCommPool[3].u32BlkCnt    = 0;


  #if 1
    stVbConf.astCommPool[4].u32BlkSize   = 176*144*2;
    stVbConf.astCommPool[4].u32BlkCnt    = 0;

   #endif
    sRet = HI_MPI_VB_SetConf(&stVbConf);
    if(HI_SUCCESS != sRet)
    {
        printf("Config VB fail!\n");
        return sRet;
    }


    sRet = HI_MPI_VB_Init();
    if(HI_SUCCESS != sRet)
    {
        printf("Init VB fail!\n");
        return sRet;
    }


    memset(&stSysConf,0,sizeof(MPP_SYS_CONF_S));
    stSysConf.u32AlignWidth = 16;

    sRet = HI_MPI_SYS_SetConf(&stSysConf);
    if(HI_SUCCESS != sRet)
    {
        printf("Config sys fail!\n");
        HI_MPI_VB_Exit();
        return sRet;
    }


    sRet = HI_MPI_SYS_Init();
     if(HI_SUCCESS != sRet)
    {
        printf("Init sys fail!\n");
        HI_MPI_VB_Exit();
        return sRet;
    }

    bMpiInit = HI_TRUE;

    return sRet;
}


HI_BOOL g_bStopSignal;

typedef struct hiIVE_LINEAR_DATA_S
{
	HI_S32 s32LinearNum;
	HI_S32 s32ThreshNum;
	POINT_S *pstLinearPoint;
}IVE_LINEAR_DATA_S;


HI_S32 SAMPLE_IVE_Linear2DClassifer(POINT_S *pstChar, HI_S32 s32CharNum, 
                                            POINT_S *pstLinearPoint, HI_S32 s32Linearnum )
{
	HI_S32 s32ResultNum;
	HI_S32 i,j;
	HI_BOOL bTestFlag;
	POINT_S *pstNextLinearPoint;
    
	s32ResultNum=0;
	pstNextLinearPoint=&pstLinearPoint[1];
    
	for(i=0;i<s32CharNum;i++)
	{
		bTestFlag=HI_TRUE;
		for(j=0;j<(s32Linearnum-1);j++)
		{

			if(   ( (pstChar[i].s32Y-pstLinearPoint[j].s32Y)*(pstNextLinearPoint[j].s32X-pstLinearPoint[j].s32X)>
				  (pstChar[i].s32X-pstLinearPoint[j].s32X)*(pstNextLinearPoint[j].s32Y-pstLinearPoint[j].s32Y) 
				   && (pstNextLinearPoint[j].s32X!=pstLinearPoint[j].s32X))
			   || ( (pstChar[i].s32X>pstLinearPoint[j].s32X) && (pstNextLinearPoint[j].s32X==pstLinearPoint[j].s32X) ))	
			{
				bTestFlag=HI_FALSE;
				break;
			}
		}
		if(bTestFlag==HI_TRUE)
		{
			s32ResultNum++;
		}
	}
	return s32ResultNum;
}

HI_VOID * SAMPLE_IVE_BlockDetect(HI_VOID *pArgs)
{
	VIDEO_FRAME_INFO_S stFrameInfo;
	HI_S32 s32Ret,s32LinearNum;
	HI_S32 s32ThreshNum;
	IVE_MEM_INFO_S stDst;
	IVE_LINEAR_DATA_S *pstIveLinerData;
	POINT_S *pstLinearPoint;

    FILE *fp1=HI_NULL, *fp2=HI_NULL, *fp3=HI_NULL;
    HI_U8 *puSrc;
    HI_U32 tic=0;
	
	pstIveLinerData=(IVE_LINEAR_DATA_S * )pArgs;
	s32LinearNum=pstIveLinerData->s32LinearNum;
	pstLinearPoint=pstIveLinerData->pstLinearPoint;
	s32ThreshNum=pstIveLinerData->s32ThreshNum;
	
	stDst.u32PhyAddr=0;

	while(1)
	{
		IVE_SRC_INFO_S stSrc;
		IVE_HANDLE hIveHandle;
		HI_U64 *pu64VirData;
		HI_S32 i,j;
		POINT_S stChar[IVECHARNUM];
		HI_S32 w,h;
        HI_U32 u32Depth = 2;

        
        s32Ret = HI_MPI_VI_SetFrameDepth(0, u32Depth);
        if (HI_SUCCESS != s32Ret)
        {
            printf("set max depth err:0x%x\n", s32Ret);
            return s32Ret;
        }
        
		s32Ret = HI_MPI_VI_GetFrameTimeOut(0, &stFrameInfo, 0);
        
		if(s32Ret!=HI_SUCCESS)
		{
			 printf("can't get vi frame for %x\n",s32Ret);
			 return HI_NULL;
		}


        stSrc.u32Width = stFrameInfo.stVFrame.u32Width;
		stSrc.u32Height = stFrameInfo.stVFrame.u32Height;
		stSrc.stSrcMem.u32PhyAddr = stFrameInfo.stVFrame.u32PhyAddr[0];
		stSrc.stSrcMem.u32Stride = stFrameInfo.stVFrame.u32Stride[0];
		stSrc.enSrcFmt = IVE_SRC_FMT_SINGLE;

        w = stFrameInfo.stVFrame.u32Width/IVECHARCALW;
		h = stSrc.u32Height/IVECHARCALH;
        
		if(stDst.u32PhyAddr==0)
		{
			s32Ret = HI_MPI_SYS_MmzAlloc_Cached(&stDst.u32PhyAddr, (HI_VOID *)&pu64VirData, 
				"User", HI_NULL, stSrc.u32Height * stSrc.u32Width*8);
			if(s32Ret!=HI_SUCCESS)
			{
				 printf("can't alloc intergal memory for %x\n",s32Ret);
				 return HI_NULL;
			}			
			stDst.u32Stride = stFrameInfo.stVFrame.u32Width;
		}	
		else if(stDst.u32Stride!=stSrc.u32Width)
		{
			HI_MPI_SYS_MmzFree(stDst.u32PhyAddr, pu64VirData);
			s32Ret = HI_MPI_SYS_MmzAlloc_Cached(&stDst.u32PhyAddr,(HI_VOID *)&pu64VirData, 
				"User", HI_NULL, stSrc.u32Height * stSrc.u32Width*8);
			if(s32Ret!=HI_SUCCESS)
			{
				 printf("can't alloc intergal memory for %x\n",s32Ret);
				 return HI_NULL;
			}			
			stDst.u32Stride = stFrameInfo.stVFrame.u32Width;
            
		}
		s32Ret = HI_MPI_IVE_INTEG(&hIveHandle, &stSrc, &stDst, HI_TRUE);
        tic++;
		if(s32Ret != HI_SUCCESS)
		{
			HI_MPI_SYS_MmzFree(stDst.u32PhyAddr, pu64VirData);
            HI_MPI_VI_ReleaseFrame(0, &stFrameInfo);
			printf(" ive integal function can't submmit for %x\n",s32Ret);
			return HI_NULL;
		}        
      
		for(i=0; i<IVECHARCALW; i++)
		{
            
            HI_U64 u64TopLeft, u64TopRight, u64BtmLeft, u64BtmRight;
            HI_U64 *u64TopRow, *u64BtmRow;

            u64TopRow = (0 == i) ? (pu64VirData) : ( pu64VirData + (i * h -1) * stDst.u32Stride);
            u64BtmRow = pu64VirData + ((i + 1) * h - 1) * stDst.u32Stride;
            
			for(j=0;j<IVECHARCALH;j++)
			{                
				HI_U64 u64BlockSum,u64BlockSq;

                u64TopLeft  = (0 == i) ? (0) : ((0 == j) ? (0) : (u64TopRow[j * w-1]));
                u64TopRight = (0 == i) ? (0) : (u64TopRow[(j + 1) * w - 1]);
                u64BtmLeft  = (0 == j) ? (0) : (u64BtmRow[j * w - 1]);
                u64BtmRight = u64BtmRow[(j + 1) * w -1];
                              
                u64BlockSum = (u64TopLeft & 0xfffffffLL) + (u64BtmRight & 0xfffffffLL)
                            - (u64BtmLeft & 0xfffffffLL) - (u64TopRight & 0xfffffffLL);

                u64BlockSq  = (u64TopLeft >> 28) + (u64BtmRight >> 28)
                            - (u64BtmLeft >> 28) - (u64TopRight >> 28);
               // mean
				stChar[i * IVECHARCALW + j].s32X = u64BlockSum/(w*h);
               // sigma=sqrt(1/(w*h)*sum((x(i,j)-mean)^2)= sqrt(sum(x(i,j)^2)/(w*h)-mean^2)
				stChar[i * IVECHARCALW + j].s32Y = sqrt(u64BlockSq/(w*h) - stChar[i * IVECHARCALW + j].s32X * stChar[i * IVECHARCALW + j].s32X);
                 	
               //printf(" area = %llu; mean=%d, var=%d; w=%d, h=%d\n", u64BlockSum, stChar[i*IVECHARCALW+j].s32X, stChar[i*IVECHARCALW+j].s32Y, w, h);
               //printf("mean=%d, var=%d; ", stChar[i * IVECHARCALW + j].s32X, stChar[i * IVECHARCALW + j].s32Y);
                
			}
		}
		s32Ret=SAMPLE_IVE_Linear2DClassifer(&stChar[0], IVECHARNUM, pstLinearPoint, s32LinearNum);

        
        if(s32Ret>s32ThreshNum)
		{
			printf("\n\033[0;31mOcclusion detected in the %dth frame!\033[0;39m Please input 'q' to stop sample ...... \n", tic);
		}
        else
        {
            printf("The %dth frame's occlusion blocks is %d. Please input 'q' to stop sample ...... \n", tic, s32Ret);
        }        
		if(g_bStopSignal == HI_TRUE)
		{
			HI_MPI_SYS_MmzFree(stDst.u32PhyAddr, pu64VirData);
            HI_MPI_VI_ReleaseFrame(0, &stFrameInfo);
			break;
		}

        s32Ret=HI_MPI_SYS_MmzFlushCache(stDst.u32PhyAddr , pu64VirData , stSrc.u32Height * stSrc.u32Width*8);
		if(s32Ret!=HI_SUCCESS)
		{
			HI_MPI_SYS_MmzFree(stDst.u32PhyAddr, pu64VirData);
            HI_MPI_VI_ReleaseFrame(0, &stFrameInfo);
			printf(" ive integal function can't flush cache for %x\n",s32Ret);
			return HI_NULL;
		}

        HI_MPI_VI_ReleaseFrame(0, &stFrameInfo);
			
	}
	return HI_NULL;
}

#define SAMPLE_IVE_ExitMpp()\
do{\
    if (HI_MPI_SYS_Exit())\
    {\
        printf("sys exit fail\n");\
        return -1;\
    }\
    if (HI_MPI_VB_Exit())\
    {\
        printf("vb exit fail\n");\
        return -1;\
    }\
    return 0;\
}while(0)

#define SAMPLE_IVE_NOT_PASS(err)\
	do\
	{\
		printf("\033[0;31mtest case <%s>not pass at line:%d.\033[0;39m\n",__FUNCTION__,__LINE__);\
	}while(0)


#define SAMPLE_IVE_CHECK_RET(express,name)\
do{\
    HI_S32 s32Ret;\
    s32Ret = express;\
    if (HI_SUCCESS != s32Ret)\
    {\
        printf("%s failed at %s: LINE: %d with %#x!\n", name, __FUNCTION__, __LINE__, s32Ret);\
		SAMPLE_IVE_NOT_PASS(err);\
	    SAMPLE_IVE_ExitMpp();\
	    return HI_FAILURE;\
    }\
}while(0)


void SAMPLE_IVE_HandleSig(HI_S32 signo)
{
    if (SIGINT == signo || SIGTSTP == signo)
    {
        printf("\033[0;31mprogram exit abnormally!\033[0;39m\n");

        HI_MPI_SYS_Exit();
	    HI_MPI_VB_Exit();
    }

    exit(0);
}

#if 0	
VI_DEV_ATTR_S DEV_ATTR_BT656D1_1MUX =
{
	/*interface mode*/
	VI_MODE_BT656,
	/*1\2\4 chnl mode*/
	VI_WORK_MODE_1Multiplex,
	/* r_mask	 g_mask    b_mask*/
	{0xFF000000,	0x0},
	/*prog or interlace mode input*/
	VI_SCAN_INTERLACED,
	/*AdChnId*/
	{-1, -1, -1, -1}
};
#endif 

VI_CHN_ATTR_S CHN_ATTR_720x576_422 =
/*classic chnl attr 2:720x576@xxfps format 422*/
{
    /*crop_x crop_y */  
    {0,     0, 720,   576}, 
    /*crop_w  crop_h  */
    {720,   576 },
    /*enCapSel*/
    VI_CAPSEL_BOTH,
    /* chnl format */
    PIXEL_FORMAT_YUV_SEMIPLANAR_422,
    /*bMirr  bFilp   bChromaResample*/
    0,      0,      0,
    /*s32SrcFrameRate   s32FrameRate*/
	-1, -1
};


HI_VOID SAMPLE_IVE_SetViMask(VI_DEV ViDev, VI_DEV_ATTR_S *pstDevAttr)
{
    pstDevAttr->au32CompMask[0] = 0x0;
    pstDevAttr->au32CompMask[1] = 0x0;
	switch (ViDev % 4)
	{
		case 0:
			pstDevAttr->au32CompMask[0] = 0xFF000000;
			if (VI_MODE_BT1120_STANDARD == pstDevAttr->enIntfMode)
			{
				pstDevAttr->au32CompMask[1] = 0x00FF0000;
			}
			else if (VI_MODE_BT1120_INTERLEAVED == pstDevAttr->enIntfMode)
			{
				pstDevAttr->au32CompMask[1] = 0x0;
			}
			break;
		case 1:
			pstDevAttr->au32CompMask[0] = 0xFF0000;
			if (VI_MODE_BT1120_INTERLEAVED == pstDevAttr->enIntfMode)
			{
				pstDevAttr->au32CompMask[1] = 0x0;
			}
			break;
		case 2:
			pstDevAttr->au32CompMask[0] = 0xFF00;
			if (VI_MODE_BT1120_STANDARD == pstDevAttr->enIntfMode)
			{
				pstDevAttr->au32CompMask[1] = 0xFF;
			}
			else if (VI_MODE_BT1120_INTERLEAVED == pstDevAttr->enIntfMode)
			{
				pstDevAttr->au32CompMask[1] = 0x0;
			}
            #if (VI_MST_TEST_CHIP==3531 || VI_MST_TEST_CHIP==3532)
                #ifndef HI_FPGA
                    if ((VI_MODE_BT1120_STANDARD != pstDevAttr->enIntfMode)
                        && (VI_MODE_BT1120_INTERLEAVED != pstDevAttr->enIntfMode)
                        && (VI_WORK_MODE_4Multiplex == pstDevAttr->enWorkMode || VI_WORK_MODE_2Multiplex == pstDevAttr->enWorkMode))
                    {
                        if (HI_FALSE == g_b4x960H)//16x960H还是采用原来的方案，设为自己的mask，与16D1不同
                        {
                            /* 3531的ASIC板是两个BT1120口出16D1，此时dev2/6要设成dev1/5的MASK */
                            pstDevAttr->au32CompMask[0] = 0xFF0000; 
                        }
                    }
                #endif
            #endif
			break;
		case 3:
			pstDevAttr->au32CompMask[0] = 0xFF;
			if (VI_MODE_BT1120_INTERLEAVED == pstDevAttr->enIntfMode)
			{
				pstDevAttr->au32CompMask[1] = 0x0;
			}
			break;
		default:
			HI_ASSERT(0);
	}
}


HI_S32 SAMPLE_IVE_StartViDevAndWay(HI_S32 u32StartDevNum, HI_U32 u32FistDev, const VI_DEV_ATTR_S *pstDevAttr)
{
    VI_DEV_ATTR_S stDevAttr;
    int i = 0;
 
    for (i = u32FistDev; i < VIU_MAX_DEV_NUM; i+=VIU_MAX_DEV_NUM/u32StartDevNum)
    {
    	memcpy(&stDevAttr, pstDevAttr, sizeof(VI_DEV_ATTR_S));
    	SAMPLE_IVE_SetViMask(i, &stDevAttr);
        SAMPLE_IVE_CHECK_RET(HI_MPI_VI_SetDevAttr(i, &stDevAttr),"SetDevAttr");
        SAMPLE_IVE_CHECK_RET(HI_MPI_VI_EnableDev(i),"Enable Dev");
    }
    
    return HI_SUCCESS;
}

HI_S32 SAMPLE_VI_1_D1(HI_VOID)
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
    SIZE_S stSize;
    
    HI_S32 i;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32BlkSize;
    HI_CHAR ch;
    HI_U32 u32WndNum;
	pthread_t hIveThread;
	IVE_LINEAR_DATA_S stIveLinerData;
	
    /******************************************
     step  1: init variable 
    ******************************************/
    gs_u32ViFrmRate = (VIDEO_ENCODING_MODE_PAL== gs_enNorm)?25:30;
    
    memset(&stVbConf,0,sizeof(VB_CONF_S));

    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm,\
                PIC_D1, SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH);
    stVbConf.u32MaxPoolCnt = 128;

    /*ddr0 video buffer*/
    stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt = u32ViChnCnt * 6;
    memset(stVbConf.astCommPool[0].acMmzName,0,
        sizeof(stVbConf.astCommPool[0].acMmzName));

    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        goto END_0;
    }

    s32Ret = SAMPLE_COMM_VI_MemConfig(enViMode);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VI_MemConfig failed with %d!\n", s32Ret);
        goto END_0;
    }

    s32Ret = SAMPLE_COMM_VPSS_MemConfig();
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VPSS_MemConfig failed with %d!\n", s32Ret);
        goto END_0;
    }

    s32Ret = SAMPLE_COMM_VO_MemConfig(SAMPLE_VO_DEV_DHD0, NULL);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_MemConfig failed with %d!\n", s32Ret);
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

    // start 2 vpss group. group-1 for clip use.
    s32Ret = SAMPLE_COMM_VPSS_Start(s32VpssGrpCnt, &stSize, VPSS_MAX_CHN_NUM,NULL);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Start Vpss failed!\n");
        goto END_1;
    }

    // bind vi to vpss group 0
    s32Ret = SAMPLE_COMM_VI_BindVpss(enViMode);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("Vi bind Vpss failed!\n");
        goto END_2;
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
    
    stVoPubAttr.enIntfType = VO_INTF_HDMI;
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
            SAMPLE_PRT("Start SAMPLE_COMM_VO_HdmiStart failed!\n");
            goto END_4;
        }
    }
    s32Ret = SAMPLE_COMM_VO_BindVpss(VoDev,VoChn,VpssGrp,VPSS_PRE0_CHN);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_BindVpss failed!\n");
        goto END_4;
    }
    

	g_bStopSignal = HI_FALSE;

	stIveLinerData.pstLinearPoint = malloc(sizeof(POINT_S)*10);
	stIveLinerData.s32LinearNum = 2;
	stIveLinerData.s32ThreshNum = IVECHARNUM/2;
	stIveLinerData.pstLinearPoint[0].s32X = 25;
	stIveLinerData.pstLinearPoint[0].s32Y = 0;
	stIveLinerData.pstLinearPoint[1].s32X = 25;
	stIveLinerData.pstLinearPoint[1].s32Y = 256;

	pthread_create(&hIveThread, 0, SAMPLE_IVE_BlockDetect, (HI_VOID *)&stIveLinerData);

    
    printf("\nPress 'q' to stop sample ... ... \n");
    while('q' != (ch = getchar()) )  {}

    g_bStopSignal = HI_TRUE;
	pthread_join(hIveThread,HI_NULL);
	free(stIveLinerData.pstLinearPoint);
    
    /******************************************
     step 7: exit process
    ******************************************/

END_4:    
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

HI_S32 main(int argc, char *argv[])
{	
	signal(SIGINT, SAMPLE_IVE_HandleSig);
    signal(SIGTERM, SAMPLE_IVE_HandleSig);
    
#if 0
    SAMPLE_IVE_CHECK_RET(SAMPLE_IVE_StartViDevAndWay(1,0,&DEV_ATTR_BT656D1_1MUX), "startDev");	
	SAMPLE_IVE_CHECK_RET(HI_MPI_VI_SetChnAttr(0, &CHN_ATTR_720x576_422),"setChnAttr");	
	SAMPLE_IVE_CHECK_RET(HI_MPI_VI_EnableChn(0),"EnableChn");
#endif
    
    SAMPLE_VI_1_D1();
   
	return 0;
}



