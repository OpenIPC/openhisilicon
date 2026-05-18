/******************************************************************************
  Some simple Hisilicon HI3531 video input functions.

  Copyright (C), 2010-2011, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
    Modification:  2011-8 Created
******************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>

#include "sample_comm.h"

VI_DEV_ATTR_S DEV_ATTR_BT656D1_4MUX =
{
    /*接口模式*/
    VI_MODE_BT656,
    /*1、2、4路工作模式*/
    VI_WORK_MODE_4Multiplex,
    /* r_mask    g_mask    b_mask*/
    {0xFF000000,    0x0},
    /*逐行or隔行输入*/
    VI_SCAN_INTERLACED,
    /*AdChnId*/
    {-1, -1, -1, -1}
};

VI_DEV_ATTR_S DEV_ATTR_BT656D1_2MUX =
{
    /*接口模式*/
    VI_MODE_BT656,
    /*1、2、4路工作模式*/
    VI_WORK_MODE_2Multiplex,
    /* r_mask    g_mask    b_mask*/
    {0xFF000000,    0x0},
    /*逐行or隔行输入*/
    VI_SCAN_INTERLACED,
    /*AdChnId*/
    {-1, -1, -1, -1}
};

VI_DEV_ATTR_S DEV_ATTR_BT656D1_1MUX =
{
    /*接口模式*/
    VI_MODE_BT656,
    /*1、2、4路工作模式*/
    VI_WORK_MODE_1Multiplex,
    /* r_mask    g_mask    b_mask*/
    {0xFF000000,    0x0},
    /*逐行or隔行输入*/
    VI_SCAN_INTERLACED,
    /*AdChnId*/
    {-1, -1, -1, -1}
};

VI_DEV_ATTR_S DEV_ATTR_BT656_720P_2MUX =
{
    /*接口模式*/
    VI_MODE_BT656,
    /*1、2、4路工作模式*/
    VI_WORK_MODE_2Multiplex,
    /* r_mask    g_mask    b_mask*/
    {0xFF000000,    0x0},
    /*逐行or隔行输入*/
    VI_SCAN_PROGRESSIVE,
    /*AdChnId*/
    {-1, -1, -1, -1}	
};



VI_DEV_ATTR_S DEV_ATTR_7441_BT1120_1080P =
/* 典型时序3:7441 BT1120 1080P@60fps典型时序 (对接时序: 时序)*/
{
    /*接口模式*/
    VI_MODE_BT1120_STANDARD,
    /*1、2、4路工作模式*/
    VI_WORK_MODE_1Multiplex,
    /* r_mask    g_mask    b_mask*/
    {0xFF000000,    0xFF0000},
    /*逐行or隔行输入*/
    VI_SCAN_PROGRESSIVE,
    /*AdChnId*/
    {-1, -1, -1, -1},
    /*enDataSeq, 仅支持YUV格式*/
    VI_INPUT_DATA_UVUV,
     
    /*同步信息，对应reg手册的如下配置, --bt1120时序无效*/
    {
    /*port_vsync   port_vsync_neg     port_hsync        port_hsync_neg        */
    VI_VSYNC_PULSE, VI_VSYNC_NEG_HIGH, VI_HSYNC_VALID_SINGNAL,VI_HSYNC_NEG_HIGH,VI_VSYNC_NORM_PULSE,VI_VSYNC_VALID_NEG_HIGH,
    
    /*timing信息，对应reg手册的如下配置*/
    /*hsync_hfb    hsync_act    hsync_hhb*/
    {0,            1920,        0,
    /*vsync0_vhb vsync0_act vsync0_hhb*/
     0,            1080,        0,
    /*vsync1_vhb vsync1_act vsync1_hhb*/ 
     0,            0,            0}
    }
};


VI_DEV_ATTR_S DEV_ATTR_7441_BT1120_720P =
/* 典型时序3:7441 BT1120 720P@60fps典型时序 (对接时序: 时序)*/
{
    /*接口模式*/
    VI_MODE_BT1120_STANDARD,
    /*1、2、4路工作模式*/
    VI_WORK_MODE_1Multiplex,
    /* r_mask    g_mask    b_mask*/
    {0xFF00,    0xFF},
    /*逐行or隔行输入*/
    VI_SCAN_PROGRESSIVE,
    /*AdChnId*/
    {-1, -1, -1, -1},
    /*enDataSeq, 仅支持YUV格式*/
    VI_INPUT_DATA_UVUV,
     
    /*同步信息，对应reg手册的如下配置, --bt1120时序无效*/
    {
    /*port_vsync   port_vsync_neg     port_hsync        port_hsync_neg        */
    VI_VSYNC_PULSE, VI_VSYNC_NEG_HIGH, VI_HSYNC_VALID_SINGNAL,VI_HSYNC_NEG_HIGH,VI_VSYNC_NORM_PULSE,VI_VSYNC_VALID_NEG_HIGH,
    
    /*timing信息，对应reg手册的如下配置*/
    /*hsync_hfb    hsync_act    hsync_hhb*/
    {0,            1280,        0,
    /*vsync0_vhb vsync0_act vsync0_hhb*/
     0,            720,        0,
    /*vsync1_vhb vsync1_act vsync1_hhb*/ 
     0,            0,            0}
    }
};

VI_DEV_ATTR_S DEV_ATTR_7441_INTERLEAVED_720P =
/* 典型时序3:7441 BT1120 720P@60fps典型时序 (对接时序: 时序)*/
{
    /*接口模式*/
    VI_MODE_BT1120_INTERLEAVED,
    /*1、2、4路工作模式*/
    VI_WORK_MODE_1Multiplex,
    /* r_mask    g_mask    b_mask*/
    {0xFF000000,    0x0},
    /*逐行or隔行输入*/
    VI_SCAN_PROGRESSIVE,
    /*AdChnId*/
    {-1, -1, -1, -1},
    /*enDataSeq, 仅支持YUV格式*/
    VI_INPUT_DATA_UVUV,
     
    /*同步信息，对应reg手册的如下配置, --bt1120时序无效*/
    {
    /*port_vsync   port_vsync_neg     port_hsync        port_hsync_neg        */
    VI_VSYNC_PULSE, VI_VSYNC_NEG_HIGH, VI_HSYNC_VALID_SINGNAL,VI_HSYNC_NEG_HIGH,VI_VSYNC_NORM_PULSE,VI_VSYNC_VALID_NEG_HIGH,
    
    /*timing信息，对应reg手册的如下配置*/
    /*hsync_hfb    hsync_act    hsync_hhb*/
    {0,            1280,        0,
    /*vsync0_vhb vsync0_act vsync0_hhb*/
     0,            720,        0,
    /*vsync1_vhb vsync1_act vsync1_hhb*/ 
     0,            0,            0}
    }
};

VI_DEV g_as32ViDev[VIU_MAX_DEV_NUM];
VI_CHN g_as32MaxChn[VIU_MAX_CHN_NUM];
VI_CHN g_as32SubChn[VIU_MAX_CHN_NUM];


HI_S32 SAMPLE_TW2865_CfgV(VIDEO_NORM_E enVideoMode,VI_WORK_MODE_E enWorkMode)
{
    int fd, i;
    int video_mode;
    tw2865_video_norm stVideoMode;
    tw2865_work_mode work_mode;

    int chip_cnt = 2;

    fd = open(TW2865_FILE, O_RDWR);
    if (fd < 0)
    {
        SAMPLE_PRT("open 2865 (%s) fail\n", TW2865_FILE);
        return -1;
    }

    video_mode = (VIDEO_ENCODING_MODE_PAL == enVideoMode) ? TW2865_PAL : TW2865_NTSC ;

    for (i=0; i<chip_cnt; i++)
    {
        stVideoMode.chip    = i;
        stVideoMode.mode    = video_mode;
        if (ioctl(fd, TW2865_SET_VIDEO_NORM, &stVideoMode))
        {
            SAMPLE_PRT("set tw2865(%d) video mode fail\n", i);
            close(fd);
            return -1;
        }
    }

    for (i=0; i<chip_cnt; i++)
    {
        work_mode.chip = i;
        if (VI_WORK_MODE_4Multiplex== enWorkMode)
        {
            work_mode.mode = TW2865_4D1_MODE;
        }
        else if (VI_WORK_MODE_2Multiplex== enWorkMode)
        {
            work_mode.mode = TW2865_2D1_MODE;
        }
        else if (VI_WORK_MODE_1Multiplex == enWorkMode)
        {
            work_mode.mode = TW2865_1D1_MODE;
        }
        else
        {
            SAMPLE_PRT("work mode not support\n");
            return -1;
        }
        ioctl(fd, TW2865_SET_WORK_MODE, &work_mode);
    }

    close(fd);
    return 0;
}


HI_S32 SAMPLE_TW2960_CfgV(VIDEO_NORM_E enVideoMode,VI_WORK_MODE_E enWorkMode)
{
    int fd, i;
    int video_mode;
    tw2865_video_norm stVideoMode;
    tw2865_work_mode work_mode;

    int chip_cnt = 4;

    fd = open(TW2960_FILE, O_RDWR);
    if (fd < 0)
    {
        SAMPLE_PRT("open 2960(%s) fail\n", TW2960_FILE);
        return -1;
    }

    video_mode = (VIDEO_ENCODING_MODE_PAL == enVideoMode) ? TW2960_PAL : TW2960_NTSC ;

    for (i=0; i<chip_cnt; i++)
    {
        stVideoMode.chip    = i;
        stVideoMode.mode    = video_mode;
        if (ioctl(fd, TW2960_SET_VIDEO_NORM, &stVideoMode))
        {
            SAMPLE_PRT("set tw2865(%d) video mode fail\n", i);
            close(fd);
            return -1;
        }
    }

    for (i=0; i<chip_cnt; i++)
    {
        work_mode.chip = i;
        if (VI_WORK_MODE_4Multiplex== enWorkMode)
        {
            work_mode.mode = TW2960_4D1_MODE;
        }
        else if (VI_WORK_MODE_2Multiplex== enWorkMode)
        {
            work_mode.mode = TW2960_2D1_MODE;
        }
        else if (VI_WORK_MODE_1Multiplex == enWorkMode)
        {
            work_mode.mode = TW2960_1D1_MODE;
        }
        else
        {
            SAMPLE_PRT("work mode not support\n");
            return -1;
        }
        ioctl(fd, TW2960_SET_WORK_MODE, &work_mode);
    }

    close(fd);
    return 0;
}


HI_S32 SAMPLE_CX26828_CfgV(VIDEO_NORM_E enVideoMode, int resolution_ratio, 
                                      VI_WORK_MODE_E enWorkMode)
{
    int fd, i;
    int video_mode;
    int chip_cnt = 1;
    cx26828_video_norm stVideoMode;
    cx26828_work_mode work_mode;
    cx26828_Multiplex_mode multiplex_mode;
    
    fd = open(CX26828_FILE, O_RDWR);
    if (fd < 0)
    {
        printf("open %s fail\n", CX26828_FILE);
        return -1;
    }

    video_mode = (VIDEO_ENCODING_MODE_PAL == enVideoMode) ? CX26828_MODE_PAL : CX26828_MODE_NTSC;
    for (i=0; i<chip_cnt; i++)
    {
        stVideoMode.chip    = i;
        stVideoMode.mode    = video_mode;
        if (ioctl(fd, CX26828_SET_VIDEO_NORM, &stVideoMode))
        {
            printf("set cx26828(%d) video mode fail\n", i);
            close(fd);
            return -1;
        }
    }

    for (i=0; i<chip_cnt; i++)
    {
        work_mode.chip = i;
        if (0 == resolution_ratio)
        {
            work_mode.mode = CX26828_16D1;
        }
        else if (1 == resolution_ratio)
        {
            work_mode.mode = CX26828_16E1;
        }
        else
        {
            printf("work mode not support.\n");
            return -1;
        }
        ioctl(fd, CX26828_SET_WORK_MODE, &work_mode);
    }

    for (i=0; i<chip_cnt; i++)
    {
        multiplex_mode.chip = i;
        if (VI_WORK_MODE_4Multiplex== enWorkMode)
        {
            multiplex_mode.mode = CX26828_4D1_MODE;
        }
        else if (VI_WORK_MODE_2Multiplex== enWorkMode)
        {
            multiplex_mode.mode = CX26828_2D1_MODE;
        }
        else if (VI_WORK_MODE_1Multiplex== enWorkMode)
        {
            multiplex_mode.mode = CX26828_1D1_MODE;
        }
        else
        {
            printf("multiplex mode not support\n");
            return -1;
        }
        ioctl(fd, CX26828_SET_Multiplex_MODE, &multiplex_mode);
    }

    close(fd);
   
    return 0;
}


HI_S32 SAMPLE_AD_CfgV_D1(VIDEO_NORM_E enVideoMode,VI_WORK_MODE_E enWorkMode)
{
#ifdef DEMO
    return SAMPLE_CX26828_CfgV(enVideoMode, 0, enWorkMode);
#else
    return SAMPLE_TW2865_CfgV(enVideoMode, enWorkMode);
#endif
    
}

HI_S32 SAMPLE_AD_CfgV_960H(VIDEO_NORM_E enVideoMode,VI_WORK_MODE_E enWorkMode)
{
#ifdef DEMO
    return SAMPLE_CX26828_CfgV(enVideoMode, 1, enWorkMode);
#else
    return SAMPLE_TW2960_CfgV(enVideoMode, enWorkMode);
#endif
}
    



/*****************************************************************************
* function : set vi mask.
*****************************************************************************/
HI_VOID SAMPLE_COMM_VI_SetMask(VI_DEV ViDev, VI_DEV_ATTR_S *pstDevAttr)
{
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

            #if HICHIP == HI3531_V100
                #ifndef HI_FPGA
                    if ((VI_MODE_BT1120_STANDARD != pstDevAttr->enIntfMode)
                        && (VI_MODE_BT1120_INTERLEAVED != pstDevAttr->enIntfMode))
                    {
                        /* 3531的ASIC板是两个BT1120口出16D1，此时dev2/6要设成dev1/5的MASK */
                        pstDevAttr->au32CompMask[0] = 0xFF0000; 
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

HI_S32 SAMPLE_COMM_VI_Mode2Param(SAMPLE_VI_MODE_E enViMode, SAMPLE_VI_PARAM_S *pstViParam)
{
    switch (enViMode)
    {
        case SAMPLE_VI_MODE_1_D1:
		case SAMPLE_VI_MODE_1_D1Cif:
            pstViParam->s32ViDevCnt = 1;
            pstViParam->s32ViDevInterval = 1;
            pstViParam->s32ViChnCnt = 1;
            pstViParam->s32ViChnInterval = 1;
            break;
        case SAMPLE_VI_MODE_16_D1:
            pstViParam->s32ViDevCnt = 4;
            pstViParam->s32ViDevInterval = 2;
            pstViParam->s32ViChnCnt = 16;
            pstViParam->s32ViChnInterval = 1;
            break;
        case SAMPLE_VI_MODE_16_960H:
            pstViParam->s32ViDevCnt = 4;
            pstViParam->s32ViDevInterval = 2;
            pstViParam->s32ViChnCnt = 16;
            pstViParam->s32ViChnInterval = 1;
            break;
        case SAMPLE_VI_MODE_4_720P:
            pstViParam->s32ViDevCnt = 2;
            pstViParam->s32ViDevInterval = 1;
            pstViParam->s32ViChnCnt = 4;
            pstViParam->s32ViChnInterval = 2;
            break;
        
        case SAMPLE_VI_MODE_4_1080P:
            pstViParam->s32ViDevCnt = 4;
            pstViParam->s32ViDevInterval = 2;
            pstViParam->s32ViChnCnt = 4;
            pstViParam->s32ViChnInterval = 4;            
            break;
        case SAMPLE_VI_MODE_1_1080P:    
            pstViParam->s32ViDevCnt = 1;
            pstViParam->s32ViDevInterval = 1;
            pstViParam->s32ViChnCnt = 1;
            pstViParam->s32ViChnInterval = 1;
            break;

        /*For Hi3521*/
		case SAMPLE_VI_MODE_8_D1:
            pstViParam->s32ViDevCnt = 2;
            pstViParam->s32ViDevInterval = 1;
            pstViParam->s32ViChnCnt = 8;
            pstViParam->s32ViChnInterval = 1;	
			break;
		case SAMPLE_VI_MODE_1_720P:
            pstViParam->s32ViDevCnt = 1;
            pstViParam->s32ViDevInterval = 1;
            pstViParam->s32ViChnCnt = 1;
            pstViParam->s32ViChnInterval = 1;	
			break;
		case SAMPLE_VI_MODE_16_Cif:
        case SAMPLE_VI_MODE_16_2Cif:
		case SAMPLE_VI_MODE_16_D1Cif:
            pstViParam->s32ViDevCnt = 4;
            pstViParam->s32ViDevInterval = 1;
            pstViParam->s32ViChnCnt = 16;
            pstViParam->s32ViChnInterval = 1;	
			break;
        case SAMPLE_VI_MODE_4_D1:
            pstViParam->s32ViDevCnt = 1;
            pstViParam->s32ViDevInterval = 1;
            pstViParam->s32ViChnCnt = 4;
            pstViParam->s32ViChnInterval = 1;	
			break; 
        case SAMPLE_VI_MODE_8_2Cif:
        case SAMPLE_VI_MODE_8_D1Cif:
            pstViParam->s32ViDevCnt = 2;
            pstViParam->s32ViDevInterval = 1;
            pstViParam->s32ViChnCnt = 8;
            pstViParam->s32ViChnInterval = 1;	
			break;  
        default:
            SAMPLE_PRT("ViMode invaild!\n");
            return HI_FAILURE;
    }
    return HI_SUCCESS;
}

/*****************************************************************************
* function : get vi parameter, according to vi type
*****************************************************************************/
HI_S32 SAMPLE_COMM_VI_ADStart(SAMPLE_VI_MODE_E enViMode, VIDEO_NORM_E enNorm)
{
    VI_WORK_MODE_E enWorkMode;
    HI_S32 s32Ret;
    
    switch (enViMode)
    {
        case SAMPLE_VI_MODE_1_D1:
		case SAMPLE_VI_MODE_1_D1Cif:
            enWorkMode = VI_WORK_MODE_4Multiplex;
            s32Ret = SAMPLE_AD_CfgV_D1(enNorm, enWorkMode);
            if (s32Ret != HI_SUCCESS)
            {
                SAMPLE_PRT("SAMPLE_TW2865_CfgV failed with %#x!\n",\
                        s32Ret);
                return HI_FAILURE;
            }
            break;
        case SAMPLE_VI_MODE_16_D1:
		case SAMPLE_VI_MODE_8_D1:
        case SAMPLE_VI_MODE_4_D1:
        case SAMPLE_VI_MODE_16_Cif:
		case SAMPLE_VI_MODE_16_2Cif:
        case SAMPLE_VI_MODE_8_2Cif:
        case SAMPLE_VI_MODE_8_D1Cif:
		case SAMPLE_VI_MODE_16_D1Cif:
            enWorkMode = VI_WORK_MODE_4Multiplex;
            s32Ret = SAMPLE_AD_CfgV_D1(enNorm, enWorkMode);
            if (s32Ret != HI_SUCCESS)
            {
                SAMPLE_PRT("SAMPLE_TW2865_CfgV failed with %#x!\n",\
                        s32Ret);
                return HI_FAILURE;
            }
            break;
        case SAMPLE_VI_MODE_16_960H:
            enWorkMode = VI_WORK_MODE_4Multiplex;
            s32Ret = SAMPLE_AD_CfgV_960H(enNorm, enWorkMode);
            if (s32Ret != HI_SUCCESS)
            {
                SAMPLE_PRT("SAMPLE_TW2960_CfgV failed with %#x!\n",\
                        s32Ret);
                return HI_FAILURE;
            }
            break;
        case SAMPLE_VI_MODE_4_720P:
		case SAMPLE_VI_MODE_1_720P:
            break;
        case SAMPLE_VI_MODE_4_1080P:
        case SAMPLE_VI_MODE_1_1080P:
            break;
        default:
            SAMPLE_PRT("AD not support!\n");
            return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}


/*****************************************************************************
* function : get vi parameter, according to vi type
*****************************************************************************/
HI_S32 SAMPLE_COMM_VI_Mode2Size(SAMPLE_VI_MODE_E enViMode, VIDEO_NORM_E enNorm, RECT_S *pstCapRect, SIZE_S *pstDestSize)
{
    pstCapRect->s32X = 0;
    pstCapRect->s32Y = 0;
    switch (enViMode)
    {
        case SAMPLE_VI_MODE_1_D1:
        case SAMPLE_VI_MODE_16_D1:
		case SAMPLE_VI_MODE_8_D1:
            pstDestSize->u32Width = D1_WIDTH;
            pstDestSize->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?576:480;
            pstCapRect->u32Width = D1_WIDTH;
            pstCapRect->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?576:480;
            break;
        case SAMPLE_VI_MODE_16_960H:
            pstDestSize->u32Width = 960;
            pstDestSize->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?576:480;
            pstCapRect->u32Width = 960;
            pstCapRect->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?576:480;
            break;
        case SAMPLE_VI_MODE_4_720P:
		case SAMPLE_VI_MODE_1_720P:	
            pstDestSize->u32Width = 1280;
            pstDestSize->u32Height = 720;
            pstCapRect->u32Width = 1280;
            pstCapRect->u32Height = 720;
            break;
        case SAMPLE_VI_MODE_4_1080P:
        case SAMPLE_VI_MODE_1_1080P:
            pstDestSize->u32Width = 1920;
            pstDestSize->u32Height = 1080;
            pstCapRect->u32Width = 1920;
            pstCapRect->u32Height = 1080;
            break;
		/*For Hi3521*/
		case SAMPLE_VI_MODE_16_2Cif:
		    pstDestSize->u32Width = D1_WIDTH / 2;
            pstDestSize->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?576:480;
            pstCapRect->u32Width = D1_WIDTH;
            pstCapRect->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?576:480;
			break;
        /*For Hi3520A*/
		case SAMPLE_VI_MODE_16_Cif:
		    pstDestSize->u32Width = D1_WIDTH /2 ;
            pstDestSize->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?288:240;
            pstCapRect->u32Width = D1_WIDTH;
            pstCapRect->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?576:480;
			break;
        case SAMPLE_VI_MODE_4_D1:
            pstDestSize->u32Width = D1_WIDTH;
            pstDestSize->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?576:480;
            pstCapRect->u32Width = D1_WIDTH;
            pstCapRect->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?576:480;
            break;
        case SAMPLE_VI_MODE_8_2Cif:
		    pstDestSize->u32Width = D1_WIDTH / 2;
            pstDestSize->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?576:480;
            pstCapRect->u32Width = D1_WIDTH;
            pstCapRect->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?576:480;
			break;
        default:
            SAMPLE_PRT("vi mode invaild!\n");
            return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_VI_GetSubChnSize(VI_CHN ViChn_Sub, VIDEO_NORM_E enNorm, SIZE_S *pstSize)
{
    VI_CHN ViChn;
    
    ViChn = ViChn_Sub - 16;

    if (0==(ViChn%4)) //(0,4,8,12) subchn max size is 960x1600
    {
        pstSize->u32Width = 720;
        pstSize->u32Height = (VIDEO_ENCODING_MODE_PAL==enNorm)?576:480;
    }
    else if (0==(ViChn%2)) //(2,6,10,14) subchn max size is 640x720
    {
        pstSize->u32Width = SAMPLE_VI_SUBCHN_2_W;
        pstSize->u32Height = SAMPLE_VI_SUBCHN_2_H;
    }
    else
    {
        SAMPLE_PRT("Vi odd sub_chn(%d) is invaild!\n", ViChn_Sub);
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}
/*****************************************************************************
* function : vi input is hd or not.
*****************************************************************************/
HI_BOOL SAMPLE_COMM_VI_IsHd(SAMPLE_VI_MODE_E enViMode)
{
        return HI_FALSE;
}

/*****************************************************************************
* function : Get Vi Dev No. according to Vi_Chn No.
*****************************************************************************/
VI_DEV SAMPLE_COMM_VI_GetDev(SAMPLE_VI_MODE_E enViMode, VI_CHN ViChn)
{
    HI_S32 s32Ret, s32ChnPerDev;
    SAMPLE_VI_PARAM_S stViParam;

    s32Ret = SAMPLE_COMM_VI_Mode2Param(enViMode, &stViParam);
    if (HI_SUCCESS !=s32Ret)
    {
        SAMPLE_PRT("vi get param failed!\n");
        return (VI_DEV)-1;
    }

    s32ChnPerDev = stViParam.s32ViChnCnt / stViParam.s32ViDevCnt;
    return (VI_DEV)(ViChn /stViParam.s32ViChnInterval / s32ChnPerDev * stViParam.s32ViDevInterval);
}

/******************************************************************************
* function : Set vi system memory location
******************************************************************************/
HI_S32 SAMPLE_COMM_VI_MemConfig(SAMPLE_VI_MODE_E enViMode)
{
    HI_CHAR * pcMmzName;
    MPP_CHN_S stMppChnVI;
    SAMPLE_VI_PARAM_S stViParam;
    VI_DEV ViDev;
    VI_CHN ViChn;

    HI_S32 i, j, s32Ret;

    s32Ret = SAMPLE_COMM_VI_Mode2Param(enViMode, &stViParam);
    if (HI_SUCCESS !=s32Ret)
    {
        SAMPLE_PRT("vi get param failed!\n");
        return HI_FAILURE;
    }

    j = 0;
    for(i=0; i<stViParam.s32ViChnCnt; i++)
    {
        ViChn = i * stViParam.s32ViChnInterval;
        ViDev = SAMPLE_COMM_VI_GetDev(enViMode, ViChn);
        //printf("dev:%d, chn:%d\n", ViDev, ViChn);
        if (ViDev < 0)
        {
            SAMPLE_PRT("get vi dev failed !\n");
            return HI_FAILURE;
        }

        pcMmzName = (0==i%2)?NULL: "ddr1";
        stMppChnVI.enModId = HI_ID_VIU;
        stMppChnVI.s32DevId = 0; //For VIU mode, this item must be set to zero
        stMppChnVI.s32ChnId = ViChn;
        s32Ret = HI_MPI_SYS_SetMemConf(&stMppChnVI,pcMmzName);
        if (s32Ret)
        {
            SAMPLE_PRT("VI HI_MPI_SYS_SetMemConf failed with %#x!\n", s32Ret);
            return HI_FAILURE;
        }
        
        /* HD mode, we will start vi sub-chn */
        if (HI_TRUE == SAMPLE_COMM_VI_IsHd(enViMode))
        {
            ViChn += 16;
            
            pcMmzName = (0==j%2)?NULL: "ddr1";
            stMppChnVI.enModId = HI_ID_VIU;
            stMppChnVI.s32DevId = 0; //For VIU mode, this item must be set to zero
            stMppChnVI.s32ChnId = ViChn;
            s32Ret = HI_MPI_SYS_SetMemConf(&stMppChnVI, pcMmzName);
            if (s32Ret)
            {
                SAMPLE_PRT("VI subchn HI_MPI_SYS_SetMemConf failed with %#x!\n", s32Ret);
                return HI_FAILURE;
            }
            j++;
        }
    }

    return HI_SUCCESS;
}

/*****************************************************************************
* function : star vi dev (cfg vi_dev_attr; set_dev_cfg; enable dev)
*****************************************************************************/
HI_S32 SAMPLE_COMM_VI_StartDev(VI_DEV ViDev, SAMPLE_VI_MODE_E enViMode)
{
    HI_S32 s32Ret;
    VI_DEV_ATTR_S    stViDevAttr;
	
    memset(&stViDevAttr,0,sizeof(stViDevAttr));

    switch (enViMode)
    {
        case SAMPLE_VI_MODE_1_D1:
		case SAMPLE_VI_MODE_1_D1Cif:
            memcpy(&stViDevAttr,&DEV_ATTR_BT656D1_4MUX,sizeof(stViDevAttr));
            SAMPLE_COMM_VI_SetMask(ViDev,&stViDevAttr);
            break;
        case SAMPLE_VI_MODE_16_D1:
		case SAMPLE_VI_MODE_8_D1:
        case SAMPLE_VI_MODE_4_D1:
        case SAMPLE_VI_MODE_16_Cif:
		case SAMPLE_VI_MODE_16_2Cif:
        case SAMPLE_VI_MODE_8_2Cif:
        case SAMPLE_VI_MODE_8_D1Cif:
		case SAMPLE_VI_MODE_16_D1Cif:
            memcpy(&stViDevAttr,&DEV_ATTR_BT656D1_4MUX,sizeof(stViDevAttr));
            SAMPLE_COMM_VI_SetMask(ViDev,&stViDevAttr);
            break;
        case SAMPLE_VI_MODE_16_960H:
            memcpy(&stViDevAttr,&DEV_ATTR_BT656D1_4MUX,sizeof(stViDevAttr));
            SAMPLE_COMM_VI_SetMask(ViDev,&stViDevAttr);
            break;
        case SAMPLE_VI_MODE_4_720P:
            memcpy(&stViDevAttr,&DEV_ATTR_BT656_720P_2MUX,sizeof(stViDevAttr));
            SAMPLE_COMM_VI_SetMask(ViDev,&stViDevAttr);
            break;			
		case SAMPLE_VI_MODE_1_720P:
            memcpy(&stViDevAttr,&DEV_ATTR_7441_BT1120_720P,sizeof(stViDevAttr));
            SAMPLE_COMM_VI_SetMask(ViDev,&stViDevAttr);
            break;
        case SAMPLE_VI_MODE_4_1080P:
        case SAMPLE_VI_MODE_1_1080P:
            memcpy(&stViDevAttr,&DEV_ATTR_7441_BT1120_1080P,sizeof(stViDevAttr));
            SAMPLE_COMM_VI_SetMask(ViDev,&stViDevAttr);
            break;
        default:
            SAMPLE_PRT("vi input type[%d] is invalid!\n", enViMode);
            return HI_FAILURE;
    }

#ifdef DEMO
    stViDevAttr.bDataRev = HI_TRUE;
#else
    stViDevAttr.bDataRev = HI_FALSE;
#endif
    
    s32Ret = HI_MPI_VI_SetDevAttr(ViDev, &stViDevAttr);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("HI_MPI_VI_SetDevAttr failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

	if(SAMPLE_VI_MODE_4_720P == enViMode)
	{
		s32Ret = HI_MPI_VI_SetPortMode(ViDev, VI_PORTMODE_720P);
	    if (s32Ret != HI_SUCCESS)
	    {
	        SAMPLE_PRT("HI_MPI_VI_SetPortMode failed with %#x!\n", s32Ret);
	        return HI_FAILURE;
	    }				
	}

    s32Ret = HI_MPI_VI_EnableDev(ViDev);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("HI_MPI_VI_EnableDev failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/*****************************************************************************
* function : star vi chn
*****************************************************************************/
HI_S32 SAMPLE_COMM_VI_StartChn(VI_CHN ViChn, RECT_S *pstCapRect, SIZE_S *pstTarSize, 
    SAMPLE_VI_MODE_E enViMode, SAMPLE_VI_CHN_SET_E enViChnSet)
{
    HI_S32 s32Ret;
    VI_CHN_ATTR_S stChnAttr;
	VI_CHN_BIND_ATTR_S stChnBindAttr;

	if(SAMPLE_VI_MODE_4_720P == enViMode)
	{
		if(2 == ViChn)
		{
			s32Ret = HI_MPI_VI_ChnUnBind(ViChn);
		    if (s32Ret != HI_SUCCESS)
		    {
		        SAMPLE_PRT("HI_MPI_VI_ChnUnBind failed with %#x!\n", s32Ret);
		        return HI_FAILURE;
		    }
			stChnBindAttr.ViDev = 0;
			stChnBindAttr.ViWay = 1;
			s32Ret = HI_MPI_VI_ChnBind(ViChn, &stChnBindAttr);
		    if (s32Ret != HI_SUCCESS)
		    {
		        SAMPLE_PRT("HI_MPI_VI_ChnBind failed with %#x!\n", s32Ret);
		        return HI_FAILURE;
		    }			
		}
		else if(6 == ViChn)
		{
			s32Ret = HI_MPI_VI_ChnUnBind(ViChn);
		    if (s32Ret != HI_SUCCESS)
		    {
		        SAMPLE_PRT("HI_MPI_VI_ChnUnBind failed with %#x!\n", s32Ret);
		        return HI_FAILURE;
		    }
			stChnBindAttr.ViDev = 1;
			stChnBindAttr.ViWay = 1;
			s32Ret = HI_MPI_VI_ChnBind(ViChn, &stChnBindAttr);
		    if (s32Ret != HI_SUCCESS)
		    {
		        SAMPLE_PRT("HI_MPI_VI_ChnBind failed with %#x!\n", s32Ret);
		        return HI_FAILURE;
		    }			
		}			
	}

    /* step  5: config & start vicap dev */
    memcpy(&stChnAttr.stCapRect, pstCapRect, sizeof(RECT_S));
    if (SAMPLE_VI_MODE_16_Cif == enViMode)
    {
        stChnAttr.enCapSel = VI_CAPSEL_BOTTOM;
    }
    else
    {
        stChnAttr.enCapSel = VI_CAPSEL_BOTH;
    }
    /* to show scale. this is a sample only, we want to show dist_size = D1 only */
    stChnAttr.stDestSize.u32Width = pstTarSize->u32Width;
    stChnAttr.stDestSize.u32Height = pstTarSize->u32Height;
    stChnAttr.enPixFormat = SAMPLE_PIXEL_FORMAT;   /* sp420 or sp422 */
    stChnAttr.bMirror = (VI_CHN_SET_MIRROR == enViChnSet)?HI_TRUE:HI_FALSE;
    stChnAttr.bFlip = (VI_CHN_SET_FILP == enViChnSet)?HI_TRUE:HI_FALSE;

    stChnAttr.bChromaResample = HI_FALSE;
    stChnAttr.s32SrcFrameRate = -1;
    stChnAttr.s32FrameRate = -1;

    s32Ret = HI_MPI_VI_SetChnAttr(ViChn, &stChnAttr);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

	if((SAMPLE_VI_MODE_4_720P == enViMode)&& (2==ViChn || 6==ViChn))
	{	
		s32Ret = HI_MPI_VI_EnableChn422to420(ViChn);
	    if (s32Ret != HI_SUCCESS)
	    {
	        SAMPLE_PRT("HI_MPI_VI_EnableChn422to420 failed with %#x!\n", s32Ret);
	        return HI_FAILURE;
	    }	
		s32Ret = HI_MPI_VI_EnableChn422to420(ViChn);
	    if (s32Ret != HI_SUCCESS)
	    {
	        SAMPLE_PRT("HI_MPI_VI_EnableChn422to420 failed with %#x!\n", s32Ret);
	        return HI_FAILURE;
	    }		
	}
	
    s32Ret = HI_MPI_VI_EnableChn(ViChn);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/*****************************************************************************
* function : star vi according to product type
*            if vi input is hd, we will start sub-chn for cvbs preview
*****************************************************************************/
HI_S32 SAMPLE_COMM_VI_Start(SAMPLE_VI_MODE_E enViMode, VIDEO_NORM_E enNorm)
{
    VI_DEV ViDev;
    VI_CHN ViChn, ViChn_Sub;
    HI_S32 i;
    HI_S32 s32Ret;
    SAMPLE_VI_PARAM_S stViParam;
    SIZE_S stMainTargetSize;
    SIZE_S stSubTargetSize;
    RECT_S stCapRect;
    
    /*** get parameter from Sample_Vi_Mode ***/
    s32Ret = SAMPLE_COMM_VI_Mode2Param(enViMode, &stViParam);
    if (HI_SUCCESS !=s32Ret)
    {
        SAMPLE_PRT("vi get param failed!\n");
        return HI_FAILURE;
    }
    s32Ret = SAMPLE_COMM_VI_Mode2Size(enViMode, enNorm, &stCapRect, &stMainTargetSize);
    if (HI_SUCCESS !=s32Ret)
    {
        SAMPLE_PRT("vi get size failed!\n");
        return HI_FAILURE;
    }
    
    /*** Start AD ***/
    s32Ret = SAMPLE_COMM_VI_ADStart(enViMode, enNorm);
    if (HI_SUCCESS !=s32Ret)
    {
        SAMPLE_PRT("Start AD failed!\n");
        return HI_FAILURE;
    }
    
    /*** Start VI Dev ***/
    for(i=0; i<stViParam.s32ViDevCnt; i++)
    {
        ViDev = i * stViParam.s32ViDevInterval;
        s32Ret = SAMPLE_COMM_VI_StartDev(ViDev, enViMode);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("SAMPLE_COMM_VI_StartDev failed with %#x\n", s32Ret);
            return HI_FAILURE;
        }
    }
    
    /*** Start VI Chn ***/
    for(i=0; i<stViParam.s32ViChnCnt; i++)
    {
        ViChn = i * stViParam.s32ViChnInterval;
        
        s32Ret = SAMPLE_COMM_VI_StartChn(ViChn, &stCapRect, &stMainTargetSize, enViMode, VI_CHN_SET_NORMAL);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("call SAMPLE_COMM_VI_StarChn failed with %#x\n", s32Ret);
            return HI_FAILURE;
        } 
        /* HD mode, we will start vi sub-chn */
        if (HI_TRUE == SAMPLE_COMM_VI_IsHd(enViMode))
        {
            ViChn_Sub = SUBCHN(ViChn);
            s32Ret = SAMPLE_COMM_VI_GetSubChnSize(ViChn_Sub, enNorm, &stSubTargetSize);
            if (HI_SUCCESS != s32Ret)
            {
                SAMPLE_PRT("SAMPLE_COMM_VI_GetSubChnSize(%d) failed!\n", ViChn_Sub);
                return HI_FAILURE;
            }
            s32Ret = SAMPLE_COMM_VI_StartChn(ViChn_Sub, &stCapRect, &stSubTargetSize,enViMode, VI_CHN_SET_NORMAL);
            if (HI_SUCCESS != s32Ret)
            {
                SAMPLE_PRT("SAMPLE_COMM_VI_StartChn (Sub_Chn-%d) failed!\n", ViChn_Sub);
                return HI_FAILURE;
            }
        }
    }

    return HI_SUCCESS;
}
/*****************************************************************************
* function : stop vi accroding to product type
*****************************************************************************/
HI_S32 SAMPLE_COMM_VI_Stop(SAMPLE_VI_MODE_E enViMode)
{
    VI_DEV ViDev;
    VI_CHN ViChn;
    HI_S32 i;
    HI_S32 s32Ret;
    SAMPLE_VI_PARAM_S stViParam;

    /*** get parameter from Sample_Vi_Mode ***/
    s32Ret = SAMPLE_COMM_VI_Mode2Param(enViMode, &stViParam);
    if (HI_SUCCESS !=s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VI_Mode2Param failed!\n");
        return HI_FAILURE;
    }

    /*** Stop VI Chn ***/
    for(i=0;i<stViParam.s32ViChnCnt;i++)
    {
        /* Stop vi phy-chn */
        ViChn = i * stViParam.s32ViChnInterval;
        s32Ret = HI_MPI_VI_DisableChn(ViChn);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("SAMPLE_COMM_VI_StopChn failed with %#x\n",s32Ret);
            return HI_FAILURE;
        }
        /* HD mode, we will stop vi sub-chn */
        if (HI_TRUE == SAMPLE_COMM_VI_IsHd(enViMode))
        {
            ViChn += 16;
            s32Ret = HI_MPI_VI_DisableChn(ViChn);
            if (HI_SUCCESS != s32Ret)
            {
                SAMPLE_PRT("SAMPLE_COMM_VI_StopChn failed with %#x\n", s32Ret);
                return HI_FAILURE;
            }
        }
    }

    /*** Stop VI Dev ***/
    for(i=0; i<stViParam.s32ViDevCnt; i++)
    {
        ViDev = i * stViParam.s32ViDevInterval;
        s32Ret = HI_MPI_VI_DisableDev(ViDev);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("SAMPLE_COMM_VI_StopDev failed with %#x\n", s32Ret);
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

/*****************************************************************************
* function : Vi chn bind vpss group
*****************************************************************************/
HI_S32 SAMPLE_COMM_VI_BindVpss(SAMPLE_VI_MODE_E enViMode)
{
    HI_S32 j, s32Ret;
    VPSS_GRP VpssGrp;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;
    SAMPLE_VI_PARAM_S stViParam;
    VI_CHN ViChn;

    s32Ret = SAMPLE_COMM_VI_Mode2Param(enViMode, &stViParam);
    if (HI_SUCCESS !=s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VI_Mode2Param failed!\n");
        return HI_FAILURE;
    }
    
    VpssGrp = 0;
    for (j=0; j<stViParam.s32ViChnCnt; j++)
    {
        ViChn = j * stViParam.s32ViChnInterval;
        
        stSrcChn.enModId = HI_ID_VIU;
        stSrcChn.s32DevId = 0;
        stSrcChn.s32ChnId = ViChn;
    
        stDestChn.enModId = HI_ID_VPSS;
        stDestChn.s32DevId = VpssGrp;
        stDestChn.s32ChnId = 0;
    
        s32Ret = HI_MPI_SYS_Bind(&stSrcChn, &stDestChn);
        if (s32Ret != HI_SUCCESS)
        {
            SAMPLE_PRT("failed with %#x!\n", s32Ret);
            return HI_FAILURE;
        }
        
        VpssGrp ++;
    }
    return HI_SUCCESS;
}


/*****************************************************************************
* function : Vi chn unbind vpss group
*****************************************************************************/
HI_S32 SAMPLE_COMM_VI_UnBindVpss(SAMPLE_VI_MODE_E enViMode)
{
    HI_S32 i, j, s32Ret;
    VPSS_GRP VpssGrp;
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;
    SAMPLE_VI_PARAM_S stViParam;
    VI_DEV ViDev;
    VI_CHN ViChn;

    s32Ret = SAMPLE_COMM_VI_Mode2Param(enViMode, &stViParam);
    if (HI_SUCCESS !=s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VI_Mode2Param failed!\n");
        return HI_FAILURE;
    }
    
    VpssGrp = 0;    
    for (i=0; i<stViParam.s32ViDevCnt; i++)
    {
        ViDev = i * stViParam.s32ViDevInterval;

        for (j=0; j<stViParam.s32ViChnCnt; j++)
        {
            ViChn = j * stViParam.s32ViChnInterval;
            
            stSrcChn.enModId = HI_ID_VIU;
            stSrcChn.s32DevId = ViDev;
            stSrcChn.s32ChnId = ViChn;
        
            stDestChn.enModId = HI_ID_VPSS;
            stDestChn.s32DevId = VpssGrp;
            stDestChn.s32ChnId = 0;
        
            s32Ret = HI_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
            if (s32Ret != HI_SUCCESS)
            {
                SAMPLE_PRT("failed with %#x!\n", s32Ret);
                return HI_FAILURE;
            }
            
            VpssGrp ++;
        }
    }
    return HI_SUCCESS;
}

/******************************************************************************
* function : read frame
******************************************************************************/
HI_VOID SAMPLE_COMM_VI_ReadFrame(FILE * fp, HI_U8 * pY, HI_U8 * pU, HI_U8 * pV, HI_U32 width, HI_U32 height, HI_U32 stride, HI_U32 stride2)
{
    HI_U8 * pDst;

    HI_U32 u32Row;

    pDst = pY;
    for ( u32Row = 0; u32Row < height; u32Row++ )
    {
        fread( pDst, width, 1, fp );
        pDst += stride;
    }

    pDst = pU;
    for ( u32Row = 0; u32Row < height/2; u32Row++ )
    {
        fread( pDst, width/2, 1, fp );
        pDst += stride2;
    }

    pDst = pV;
    for ( u32Row = 0; u32Row < height/2; u32Row++ )
    {
        fread( pDst, width/2, 1, fp );
        pDst += stride2;
    }
}
 
/******************************************************************************
* function : Plan to Semi
******************************************************************************/
HI_S32 SAMPLE_COMM_VI_PlanToSemi(HI_U8 *pY, HI_S32 yStride,
                       HI_U8 *pU, HI_S32 uStride,
                       HI_U8 *pV, HI_S32 vStride,
                       HI_S32 picWidth, HI_S32 picHeight)
{
    HI_S32 i;
    HI_U8* pTmpU, *ptu;
    HI_U8* pTmpV, *ptv;
    HI_S32 s32HafW = uStride >>1 ;
    HI_S32 s32HafH = picHeight >>1 ;
    HI_S32 s32Size = s32HafW*s32HafH;

    pTmpU = malloc( s32Size ); ptu = pTmpU;
    pTmpV = malloc( s32Size ); ptv = pTmpV;

    memcpy(pTmpU,pU,s32Size);
    memcpy(pTmpV,pV,s32Size);

    for(i = 0;i<s32Size>>1;i++)
    {
        *pU++ = *pTmpV++;
        *pU++ = *pTmpU++;

    }
    for(i = 0;i<s32Size>>1;i++)
    {
        *pV++ = *pTmpV++;
        *pV++ = *pTmpU++;
    }

    free( ptu );
    free( ptv );

    return HI_SUCCESS;
}

/******************************************************************************
* function : Get from YUV
******************************************************************************/
HI_S32 SAMPLE_COMM_VI_GetVFrameFromYUV(FILE *pYUVFile, HI_U32 u32Width, HI_U32 u32Height,HI_U32 u32Stride, VIDEO_FRAME_INFO_S *pstVFrameInfo)
{
    HI_U32             u32LStride;
    HI_U32             u32CStride;
    HI_U32             u32LumaSize;
    HI_U32             u32ChrmSize;
    HI_U32             u32Size;
    VB_BLK VbBlk;
    HI_U32 u32PhyAddr;
    HI_U8 *pVirAddr;

    u32LStride  = u32Stride;
    u32CStride  = u32Stride;

    u32LumaSize = (u32LStride * u32Height);
    u32ChrmSize = (u32CStride * u32Height) >> 2;/* YUV 420 */
    u32Size = u32LumaSize + (u32ChrmSize << 1);

    /* alloc video buffer block ---------------------------------------------------------- */
    VbBlk = HI_MPI_VB_GetBlock(VB_INVALID_POOLID, u32Size, NULL);
    if (VB_INVALID_HANDLE == VbBlk)
    {
        SAMPLE_PRT("HI_MPI_VB_GetBlock err! size:%d\n",u32Size);
        return -1;
    }
    u32PhyAddr = HI_MPI_VB_Handle2PhysAddr(VbBlk);
    if (0 == u32PhyAddr)
    {
        return -1;
    }

    pVirAddr = (HI_U8 *) HI_MPI_SYS_Mmap(u32PhyAddr, u32Size);
    if (NULL == pVirAddr)
    {
        return -1;
    }

    pstVFrameInfo->u32PoolId = HI_MPI_VB_Handle2PoolId(VbBlk);
    if (VB_INVALID_POOLID == pstVFrameInfo->u32PoolId)
    {
        return -1;
    }
    SAMPLE_PRT("pool id :%d, phyAddr:%x,virAddr:%x\n" ,pstVFrameInfo->u32PoolId,u32PhyAddr,(int)pVirAddr);

    pstVFrameInfo->stVFrame.u32PhyAddr[0] = u32PhyAddr;
    pstVFrameInfo->stVFrame.u32PhyAddr[1] = pstVFrameInfo->stVFrame.u32PhyAddr[0] + u32LumaSize;
    pstVFrameInfo->stVFrame.u32PhyAddr[2] = pstVFrameInfo->stVFrame.u32PhyAddr[1] + u32ChrmSize;

    pstVFrameInfo->stVFrame.pVirAddr[0] = pVirAddr;
    pstVFrameInfo->stVFrame.pVirAddr[1] = pstVFrameInfo->stVFrame.pVirAddr[0] + u32LumaSize;
    pstVFrameInfo->stVFrame.pVirAddr[2] = pstVFrameInfo->stVFrame.pVirAddr[1] + u32ChrmSize;

    pstVFrameInfo->stVFrame.u32Width  = u32Width;
    pstVFrameInfo->stVFrame.u32Height = u32Height;
    pstVFrameInfo->stVFrame.u32Stride[0] = u32LStride;
    pstVFrameInfo->stVFrame.u32Stride[1] = u32CStride;
    pstVFrameInfo->stVFrame.u32Stride[2] = u32CStride;
    pstVFrameInfo->stVFrame.enPixelFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
    pstVFrameInfo->stVFrame.u32Field = VIDEO_FIELD_INTERLACED;/* Intelaced D1,otherwise VIDEO_FIELD_FRAME */

    /* read Y U V data from file to the addr ----------------------------------------------*/
    SAMPLE_COMM_VI_ReadFrame(pYUVFile, pstVFrameInfo->stVFrame.pVirAddr[0],
       pstVFrameInfo->stVFrame.pVirAddr[1], pstVFrameInfo->stVFrame.pVirAddr[2],
       pstVFrameInfo->stVFrame.u32Width, pstVFrameInfo->stVFrame.u32Height,
       pstVFrameInfo->stVFrame.u32Stride[0], pstVFrameInfo->stVFrame.u32Stride[1] >> 1 );

    /* convert planar YUV420 to sem-planar YUV420 -----------------------------------------*/
    SAMPLE_COMM_VI_PlanToSemi(pstVFrameInfo->stVFrame.pVirAddr[0], pstVFrameInfo->stVFrame.u32Stride[0],
      pstVFrameInfo->stVFrame.pVirAddr[1], pstVFrameInfo->stVFrame.u32Stride[1],
      pstVFrameInfo->stVFrame.pVirAddr[2], pstVFrameInfo->stVFrame.u32Stride[1],
      pstVFrameInfo->stVFrame.u32Width, pstVFrameInfo->stVFrame.u32Height);

    HI_MPI_SYS_Munmap(pVirAddr, u32Size);
    return 0;
}

HI_S32 SAMPLE_COMM_VI_ChangeCapSize(VI_CHN ViChn, HI_U32 u32CapWidth, HI_U32 u32CapHeight,HI_U32 u32Width, HI_U32 u32Height)
{
    VI_CHN_ATTR_S stChnAttr;
	HI_S32 S32Ret = HI_SUCCESS;
	S32Ret = HI_MPI_VI_GetChnAttr(ViChn, &stChnAttr);
	if(HI_SUCCESS!= S32Ret)
	{
	    SAMPLE_PRT( "HI_MPI_VI_GetChnAttr failed\n");
	}
    stChnAttr.stCapRect.u32Width = u32CapWidth;
    stChnAttr.stCapRect.u32Height = u32CapHeight;
    stChnAttr.stDestSize.u32Width = u32Width;
    stChnAttr.stDestSize.u32Height = u32Height;

    S32Ret = HI_MPI_VI_SetChnAttr(ViChn, &stChnAttr);
	if(HI_SUCCESS!= S32Ret)
	{
	    SAMPLE_PRT( "HI_MPI_VI_SetChnAttr failed\n");
	}

    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_VI_ChangeDestSize(VI_CHN ViChn, HI_U32 u32Width, HI_U32 u32Height)
{
    VI_CHN_ATTR_S stChnAttr;
	HI_S32 S32Ret = HI_SUCCESS;
	S32Ret = HI_MPI_VI_GetChnAttr(ViChn, &stChnAttr);
	if(HI_SUCCESS!= S32Ret)
	{
	    SAMPLE_PRT( "HI_MPI_VI_GetChnAttr failed\n");
	}
    stChnAttr.stDestSize.u32Width = u32Width;
    stChnAttr.stDestSize.u32Height = u32Height;

    S32Ret = HI_MPI_VI_SetChnAttr(ViChn, &stChnAttr);
	if(HI_SUCCESS!= S32Ret)
	{
	    SAMPLE_PRT( "HI_MPI_VI_SetChnAttr failed\n");
	}

    return HI_SUCCESS;
}


/*****************************************************************************
* function : star vi according to product type
*           only for Hi3520D MixCap 
*****************************************************************************/
HI_S32 SAMPLE_COMM_VI_MixCap_Start(SAMPLE_VI_MODE_E enViMode, VIDEO_NORM_E enNorm)
{
    VI_DEV ViDev;
    VI_CHN ViChn;
    HI_S32 i;
    HI_S32 s32Ret;
    SAMPLE_VI_PARAM_S stViParam;
    VI_CHN_ATTR_S stChnAttr,stChnMinorAttr;
	
    /*** get parameter from Sample_Vi_Mode ***/
    s32Ret = SAMPLE_COMM_VI_Mode2Param(enViMode, &stViParam);
    if (HI_SUCCESS !=s32Ret)
    {
        SAMPLE_PRT("vi get param failed!\n");
        return HI_FAILURE;
    }
    
    /*** Start AD ***/
    s32Ret = SAMPLE_COMM_VI_ADStart(enViMode, enNorm);
    if (HI_SUCCESS !=s32Ret)
    {
        SAMPLE_PRT("Start AD failed!\n");
        return HI_FAILURE;
    }
    
    /*** Start VI Dev ***/
    for(i=0; i<stViParam.s32ViDevCnt; i++)
    {
        ViDev = i * stViParam.s32ViDevInterval;
        s32Ret = SAMPLE_COMM_VI_StartDev(ViDev, enViMode);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("SAMPLE_COMM_VI_StartDev failed with %#x\n", s32Ret);
            return HI_FAILURE;
        }
    }
    
    /*** Start VI Chn ***/
    for(i=0; i<stViParam.s32ViChnCnt; i++)
    {
        ViChn = i * stViParam.s32ViChnInterval;

	    stChnAttr.stCapRect.s32X = 0;
	    stChnAttr.stCapRect.s32Y = 0;
	    stChnAttr.enCapSel = VI_CAPSEL_BOTH;                      
	    stChnAttr.enPixFormat = SAMPLE_PIXEL_FORMAT;   /* sp420 or sp422 */
	    stChnAttr.bMirror = HI_FALSE;                             
	    stChnAttr.bFlip   = HI_FALSE;                              
	    stChnAttr.bChromaResample = HI_FALSE;                      
	    stChnAttr.s32SrcFrameRate = -1;
	    stChnAttr.s32FrameRate = -1;
		stChnAttr.stCapRect.u32Width= D1_WIDTH;
	    stChnAttr.stCapRect.u32Height= (VIDEO_ENCODING_MODE_PAL==enNorm)?576:480;	
	    stChnAttr.stDestSize.u32Width= D1_WIDTH;
	    stChnAttr.stDestSize.u32Height= (VIDEO_ENCODING_MODE_PAL==enNorm)?576:480;
		
		memcpy(&stChnMinorAttr, &stChnAttr, sizeof(VI_CHN_ATTR_S));
	    stChnMinorAttr.stDestSize.u32Width= D1_WIDTH / 2;
	    stChnMinorAttr.stDestSize.u32Height= (VIDEO_ENCODING_MODE_PAL==enNorm)?576:480;

		stChnAttr.s32SrcFrameRate = (VIDEO_ENCODING_MODE_PAL== enNorm)?25:30;
		stChnAttr.s32FrameRate = 6;
		
		s32Ret = HI_MPI_VI_SetChnAttr(ViChn, &stChnAttr);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("call HI_MPI_VI_SetChnAttr failed with %#x\n", s32Ret);
            return HI_FAILURE;
        } 
		s32Ret = HI_MPI_VI_SetChnMinorAttr(ViChn, &stChnMinorAttr);
		if (HI_SUCCESS != s32Ret)
		{
			SAMPLE_PRT("call HI_MPI_VI_SetChnMinorAttr failed with %#x\n", s32Ret);
			return HI_FAILURE;
		} 
		s32Ret = HI_MPI_VI_EnableChn(ViChn);
		if (s32Ret != HI_SUCCESS)
		{
			SAMPLE_PRT("HI_MPI_VI_EnableChn failed with %#x!\n", s32Ret);
			return HI_FAILURE;
		}

    }

    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_VI_ChangeMixCap(VI_CHN ViChn,HI_BOOL bMixCap,HI_U32 FrameRate)
{
    VI_CHN_ATTR_S stChnAttr,stChnMinorAttr;
	HI_S32 S32Ret = HI_SUCCESS;
	S32Ret = HI_MPI_VI_GetChnAttr(ViChn, &stChnAttr);
	if(HI_SUCCESS!= S32Ret)
	{
	    SAMPLE_PRT( "HI_MPI_VI_GetChnAttr failed");
	}
	
	if(HI_TRUE == bMixCap)
	{
		memcpy(&stChnMinorAttr, &stChnAttr, sizeof(VI_CHN_ATTR_S));
	    stChnMinorAttr.stDestSize.u32Width = D1_WIDTH / 2;
		
		stChnAttr.s32FrameRate = FrameRate;
		
		S32Ret = HI_MPI_VI_SetChnAttr(ViChn, &stChnAttr);
        if (HI_SUCCESS != S32Ret)
        {
            SAMPLE_PRT("call HI_MPI_VI_SetChnAttr failed with %#x\n", S32Ret);
            return HI_FAILURE;
        } 
		S32Ret = HI_MPI_VI_SetChnMinorAttr(ViChn, &stChnMinorAttr);
		if (HI_SUCCESS != S32Ret)
		{
			SAMPLE_PRT("call HI_MPI_VI_SetChnMinorAttr failed with %#x\n", S32Ret);
			return HI_FAILURE;
		} 
	}
	else
	{
		stChnAttr.s32FrameRate = stChnAttr.s32SrcFrameRate;
		S32Ret = HI_MPI_VI_SetChnAttr(ViChn, &stChnAttr);
        if (HI_SUCCESS != S32Ret)
        {
            SAMPLE_PRT("call HI_MPI_VI_SetChnAttr failed with %#x\n", S32Ret);
            return HI_FAILURE;
        } 
	}
	return HI_SUCCESS;
}
	

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
