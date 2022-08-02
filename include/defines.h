/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __DEFINES_H__
#define __DEFINES_H__

#include "autoconf.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifndef CHIP_NAME
#define CHIP_NAME                         CONFIG_CHIP_TYPE
#endif

#define CHIP_NAME_GK7205V200                   0x72050200
#define CHIP_NAME_GK7205V210                   0x72050210
#define CHIP_NAME_GK7205V300                   0x72050300
#define CHIP_NAME_GK7202V300                   0x72020300
#define CHIP_NAME_GK7605V100                   0x76050100

#define CHIP_NAME_TEST                        0x0

#if CHIP_NAME == CHIP_NAME_GK7205V200
#define CHIP_NAME_STR                      "GK7205V200"
#else
#error CHIP_NAME define may be error
#endif

#define MPP_VER_PRIX                   "MPP_V"

#define ALIGN_NUM                      4

#define LUMA_PHY_ALIGN                 16

#define DEFAULT_ALIGN                  8
#define MAX_ALIGN                      1024
#define SEG_CMP_LENGTH                 128

#define MAX_MMZ_NAME_LEN               16

#define VO_MAX_NODE_NUM                16

#define VB_MAX_POOLS                   512

#define VENC_MAX_NAME_LEN              16
#define VENC_MAX_CHN_NUM               16
#define VEDU_IP_NUM                    1
#define H264E_MAX_WIDTH                3072
#define H264E_MAX_HEIGHT               3072
#define H264E_MIN_WIDTH                114
#define H264E_MIN_HEIGHT               114
#define H265E_MAX_WIDTH                3072
#define H265E_MAX_HEIGHT               3072
#define H265E_MIN_WIDTH                114
#define H265E_MIN_HEIGHT               114
#define JPEGE_MAX_WIDTH                8192
#define JPEGE_MAX_HEIGHT               8192
#define JPEGE_MIN_WIDTH                32
#define JPEGE_MIN_HEIGHT               32
#define JPGE_MAX_NUM                   1
#define VENC_MAX_ROI_NUM               8
#define H264E_MIN_HW_INDEX             0
#define H264E_MAX_HW_INDEX             11
#define H264E_MIN_VW_INDEX             0
#define H264E_MAX_VW_INDEX             3
#define VENC_QP_HISGRM_NUM             52
#define MAX_TILE_NUM                   1
#define H265E_ADAPTIVE_FRAME_TYPE      4
#define H265E_ADAPTIVE_QP_TYPE         5
#define H265E_LCU_SIZE                 32

#define VENC_MIN_INPUT_FRAME_RATE      1
#define VENC_MAX_INPUT_FRAME_RATE      240

#define VENC_MAX_RECEIVE_SOURCE        4

#define VENC_PIC_RECEIVE_SOURCE0       0
#define VENC_PIC_RECEIVE_SOURCE1       1
#define VENC_PIC_RECEIVE_SOURCE2       2
#define VENC_PIC_RECEIVE_SOURCE3       3

#define RC_TEXTURE_THR_SIZE            16
#define MIN_BITRATE                    2
#define MAX_BITRATE                    60 * 1024
#define MAX_EXTRA_BITRATE              1000 * 1024

#define RGN_MIN_WIDTH                  2
#define RGN_MIN_HEIGHT                 2

#define RGN_COVER_MIN_X                0
#define RGN_COVER_MIN_Y                0
#define RGN_COVER_MAX_X                4094
#define RGN_COVER_MAX_Y                4094
#define RGN_COVER_MAX_WIDTH            4096
#define RGN_COVER_MAX_HEIGHT           4096
#define RGN_COVER_MIN_THICK            2
#define RGN_COVER_MAX_THICK            8

#define RGN_COVEREX_MIN_X              -4096
#define RGN_COVEREX_MIN_Y              -4096
#define RGN_COVEREX_MAX_X              4094
#define RGN_COVEREX_MAX_Y              4094
#define RGN_COVEREX_MAX_WIDTH          4096
#define RGN_COVEREX_MAX_HEIGHT         4096
#define RGN_COVEREX_MIN_THICK          2
#define RGN_COVEREX_MAX_THICK          14

#define RGN_OVERLAY_MIN_X              0
#define RGN_OVERLAY_MIN_Y              0
#define RGN_OVERLAY_MAX_X              4094
#define RGN_OVERLAY_MAX_Y              4094
#define RGN_OVERLAY_MAX_WIDTH          4096
#define RGN_OVERLAY_MAX_HEIGHT         4096

#define RGN_OVERLAYEX_MIN_X            0
#define RGN_OVERLAYEX_MIN_Y            0
#define RGN_OVERLAYEX_MAX_X            4094
#define RGN_OVERLAYEX_MAX_Y            4094
#define RGN_OVERLAYEX_MAX_WIDTH        4096
#define RGN_OVERLAYEX_MAX_HEIGHT       4096

#define RGN_MOSAIC_X_ALIGN             4
#define RGN_MOSAIC_Y_ALIGN             2
#define RGN_MOSAIC_WIDTH_ALIGN         4
#define RGN_MOSAIC_HEIGHT_ALIGN        2

#define RGN_MOSAIC_MIN_X               0
#define RGN_MOSAIC_MIN_Y               0
#define RGN_MOSAIC_MAX_X               4092
#define RGN_MOSAIC_MAX_Y               4094
#define RGN_MOSAIC_MIN_WIDTH           8
#define RGN_MOSAIC_MIN_HEIGHT          8
#define RGN_MOSAIC_MAX_WIDTH           4096
#define RGN_MOSAIC_MAX_HEIGHT          4096

#define RGN_ALIGN                      2

#define RGN_HANDLE_MAX                 128
#define RGN_MAX_BUF_NUM                6

#define COVER_MAX_NUM_VI               0
#define COVEREX_MAX_NUM_VI             16
#define OVERLAY_MAX_NUM_VI             0
#define OVERLAYEX_MAX_NUM_VI           16
#define MOSAIC_MAX_NUM_VI              0

#define OVERLAY_MAX_NUM_VENC           8
#define OVERLAYEX_MAX_NUM_VENC         8

#define COVER_MAX_NUM_VPSS             8
#define COVEREX_MAX_NUM_VPSS           8
#define OVERLAY_MAX_NUM_VPSS           0
#define OVERLAYEX_MAX_NUM_VPSS         8
#define MOSAIC_MAX_NUM_VPSS            4

#define OVERLAYEX_MAX_NUM_AVS          0

#define COVEREX_MAX_NUM_VO             1
#define OVERLAYEX_MAX_NUM_VO           1

#define OVERLAYEX_MAX_NUM_PCIV         0

#define RGN_VGS_TASK_WIDTH_MAX         4096

#define VENC_MAX_SSE_NUM               8
#define SVP_MAX_SENSOR_NUM            2

#define VI_MAX_DEV_NUM                 1
#define VI_MAX_PHY_PIPE_NUM            2
#define VI_MAX_VIR_PIPE_NUM            2
#define VI_MAX_PIPE_NUM                (VI_MAX_PHY_PIPE_NUM + VI_MAX_VIR_PIPE_NUM)
#define VI_MAX_STITCH_GRP_NUM          1
#define VI_MAX_WDR_NUM                 2

#define VI_MAX_PHY_CHN_NUM             1

#ifdef CONFIG_VI_EXTCHN_SUPPORT
#define VI_MAX_EXT_CHN_NUM             8
#else
#define VI_MAX_EXT_CHN_NUM             0
#endif
#define VI_EXT_CHN_START               VI_MAX_PHY_CHN_NUM
#define VI_MAX_CHN_NUM                 (VI_MAX_PHY_CHN_NUM + VI_MAX_EXT_CHN_NUM)
#define VI_MAX_EXTCHN_BIND_PER_CHN     8

#define VIPROC_IRQ_NUM                 1
#define VI_MAX_WDR_FRAME_NUM           2
#define VI_MAX_NODE_NUM                2
#define VIPROC_IP_NUM                  1
#define VICAP_IP_NUM                   1

#define VI_MAX_SPLIT_NODE_NUM          2

#define VI_DEV_MIN_WIDTH               120
#define VI_DEV_MIN_HEIGHT              120
#define VI_DEV_MAX_WIDTH               3072
#define VI_DEV_MAX_HEIGHT              3072
#define VI_FPN_MAX_WIDTH               3072
#define VI_FPN_MAX_HEIGHT              VI_DEV_MAX_HEIGHT

#define VI_PIPE_OFFLINE_MIN_WIDTH      120
#define VI_PIPE_OFFLINE_MIN_HEIGHT     120
#define VI_PIPE_OFFLINE_MAX_WIDTH      3072
#define VI_PIPE_OFFLINE_MAX_HEIGHT     3072

#define VI_PIPE_ONLINE_MIN_WIDTH       120
#define VI_PIPE_ONLINE_MIN_HEIGHT      120
#define VI_PIPE_ONLINE_MAX_WIDTH       2304
#define VI_PIPE_ONLINE_MAX_HEIGHT      3072

#define VI_PHYCHN_OFFLINE_MIN_WIDTH    120
#define VI_PHYCHN_OFFLINE_MIN_HEIGHT   120
#define VI_PHYCHN_OFFLINE_MAX_WIDTH    3072
#define VI_PHYCHN_OFFLINE_MAX_HEIGHT   3072

#define VI_PHYCHN_ONLINE_MIN_WIDTH     120
#define VI_PHYCHN_ONLINE_MIN_HEIGHT    120
#define VI_PHYCHN_ONLINE_MAX_WIDTH     2304
#define VI_PHYCHN_ONLINE_MAX_HEIGHT    3072

#define VI_EXTCHN_MIN_WIDTH            32
#define VI_EXTCHN_MIN_HEIGHT           32
#define VI_EXTCHN_MAX_WIDTH            4096
#define VI_EXTCHN_MAX_HEIGHT           4096

#define VI_PHY_CHN1_MAX_ZOOMIN         1
#define VI_PHY_CHN1_MAX_ZOOMOUT        30
#define VI_EXT_CHN_MAX_ZOOMIN          16
#define VI_EXT_CHN_MAX_ZOOMOUT         30

#define VI_CMP_PARAM_SIZE              144

#define VI_VPSS_DEFAULT_EARLINE        128

#define DIS_PYRAMID_LAYER_NUM          5
#define DIS_MAX_CHN_NUM                2
#define DIS_MAX_IMAGE_WIDTH            (3840)
#define DIS_MIN_IMAGE_WIDTH            (1280)
#define DIS_MAX_IMAGE_HEIGHT           (2160)
#define DIS_MIN_IMAGE_HEIGHT           (720)


#define VO_MAX_VIRT_DEV_NUM            0
#define VO_MAX_PHY_DEV_NUM             1

#define VO_MIN_CHN_WIDTH               32
#define VO_MIN_CHN_HEIGHT              32
#define VO_MAX_ZOOM_RATIO              1000
#define VO_MAX_DEV_NUM                 1
#define VO_MAX_LAYER_NUM               1
#define VO_MAX_PRIORITY                1
#define VO_MAX_CHN_NUM                 1
#define VO_MAX_LAYER_IN_DEV            1
#define VO_MAX_GRAPHICS_LAYER_NUM      1
#define VO_MAX_WBC_NUM                 1
#define VO_MIN_TOLERATE                1
#define VO_MAX_TOLERATE                100000

#define AVS_MAX_GRP_NUM                32
#define AVS_PIPE_NUM                   4
#define AVS_MAX_CHN_NUM                2
#define AVS_SPLIT_NUM                  2
#define AVS_SPLIT_PIPE_NUM             6
#define AVS_CUBE_MAP_SURFACE_NUM       6

#define AVS_MAX_IN_WIDTH               8192
#define AVS_MAX_IN_HEIGHT              8192
#define AVS_MIN_IN_WIDTH               1280
#define AVS_MIN_IN_HEIGHT              720

#define AVS_MAX_OUT_WIDTH              8192
#define AVS_MAX_OUT_HEIGHT             8192
#define AVS_MIN_OUT_WIDTH              256
#define AVS_MIN_OUT_HEIGHT             256

#define AI_DEV_MAX_NUM                 1
#define AO_DEV_MIN_NUM                 0
#define AO_DEV_MAX_NUM                 1
#define AIO_MAX_NUM                    1
#define AENC_MAX_CHN_NUM               32
#define ADEC_MAX_CHN_NUM               32

#define AI_MAX_CHN_NUM                 2
#define AO_MAX_CHN_NUM                 3
#define AO_SYSCHN_CHNID                (AO_MAX_CHN_NUM - 1)

#define AIO_MAX_CHN_NUM                ((AO_MAX_CHN_NUM > AI_MAX_CHN_NUM) ? AO_MAX_CHN_NUM : AI_MAX_CHN_NUM)

#define VPSS_IP_NUM                    1
#define VPSS0                          0
#define VPSS_MAX_GRP_NUM               8
#define VPSS_MAX_GRP_PIPE_NUM          1
#define VPSS_PARALLEL_PIC_NUM          1
#define VPSS_MAX_PHY_CHN_NUM           3
#define VPSS_LOWDELAY_CHN_NUM          2
#define VPSS_MAX_EXT_CHN_NUM           4
#define VPSS_MAX_CHN_NUM               (VPSS_MAX_PHY_CHN_NUM + VPSS_MAX_EXT_CHN_NUM)
#define VPSS_MIN_IMAGE_WIDTH_SBS       512
#define VPSS_MIN_IMAGE_WIDTH           64
#define VPSS_MIN_IMAGE_HEIGHT          64
#define VPSS_MAX_IMAGE_WIDTH_SLAVE     4096
#define VPSS_MAX_IN_IMAGE_WIDTH        4096
#define VPSS_MAX_IMAGE_WIDTH           4096
#define VPSS_MAX_IMAGE_HEIGHT          4096
#define VPSS_MAX_CMP_IMAGE_WIDTH       3072
#define VPSS_MAX_CMP_IMAGE_HEIGHT      3072
#define VPSS_EXTCHN_MAX_IMAGE_WIDTH    4096
#define VPSS_EXTCHN_MAX_IMAGE_HEIGHT   4096
#define VPSS_MAX_ZOOMIN                16
#define VPSS_MAX_ZOOMOUT               15
#define VPSS_EXT_CHN_MAX_ZOOMIN        16
#define VPSS_EXT_CHN_MAX_ZOOMOUT       30
#define VPSS_LINE_BUFFER               2304

#define PCIV_MAX_CHN_NUM               16

#define MD_MAX_CHN                     64
#define MD_MAX_WIDTH                   1920
#define MD_MAX_HEIGHT                  1080
#define MD_MIN_WIDTH                   64
#define MD_MIN_HEIGHT                  64

#define DPU_RECT_MAX_GRP_NUM           8
#define DPU_RECT_MAX_PIPE_NUM          2
#define DPU_RECT_MAX_CHN_NUM           2

#define DPU_RECT_IN_IMAGE_MAX_WIDTH    2048
#define DPU_RECT_IN_IMAGE_MAX_HEIGHT   2048
#define DPU_RECT_IN_IMAGE_MIN_WIDTH    128
#define DPU_RECT_IN_IMAGE_MIN_HEIGHT   64
#define DPU_RECT_OUT_IMAGE_MAX_WIDTH   1920
#define DPU_RECT_OUT_IMAGE_MAX_HEIGHT  1080
#define DPU_RECT_OUT_IMAGE_MIN_WIDTH   128
#define DPU_RECT_OUT_IMAGE_MIN_HEIGHT  64

#define DPU_MATCH_MAX_GRP_NUM          8
#define DPU_MATCH_MAX_PIPE_NUM         2
#define DPU_MATCH_MAX_CHN_NUM          1

#define DPU_MATCH_IN_IMAGE_MAX_WIDTH   1920
#define DPU_MATCH_IN_IMAGE_MAX_HEIGHT  1080
#define DPU_MATCH_IN_IMAGE_MIN_WIDTH   128
#define DPU_MATCH_IN_IMAGE_MIN_HEIGHT  64
#define DPU_MATCH_OUT_IMAGE_MAX_WIDTH  1920
#define DPU_MATCH_OUT_IMAGE_MAX_HEIGHT 1080
#define DPU_MATCH_OUT_IMAGE_MIN_WIDTH  128
#define DPU_MATCH_OUT_IMAGE_MIN_HEIGHT 64

#define FISHEYE_MIN_IN_IMAGE_WIDTH     1920
#define FISHEYE_MIN_IN_IMAGE_HEIGHT    1080
#define FISHEYE_MIN_OUT_IMAGE_WIDTH    640
#define FISHEYE_MIN_OUT_IMAGE_HEIGHT   360

#define LDC_MIN_IMAGE_WIDTH            640
#define LDC_MIN_IMAGE_HEIGHT           480

#define SPREAD_MIN_IMAGE_WIDTH         640
#define SPREAD_MIN_IMAGE_HEIGHT        480

#define PMF_MIN_IMAGE_WIDTH            480
#define PMF_MIN_IMAGE_HEIGHT           360

#define ROTATION_EX_MIN_IMAGE_WIDTH    640
#define ROTATION_EX_MIN_IMAGE_HEIGHT   480

#define GDC_MAX_IMAGE_WIDTH            4608
#define GDC_MAX_IMAGE_HEIGHT           4608
#define GDC_IP_NUM                     1
#define GDC_FUSION_NUM                 9

#define VGS_IP_NUM                     1
#define VGS0                           0
#define VGS1                           1

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

