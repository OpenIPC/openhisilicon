/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef DEFINES_H
#define DEFINES_H

#ifdef __cplusplus
extern "C" {
#endif

#define CHIP_NAME_TEST                 0x0

#define MPP_VER_PRIX                   "MPP_V"

#define ALIGN_NUM                      4

#define LUMA_PHY_ALIGN                 16

#define DEFAULT_ALIGN                  16
#define MAX_ALIGN                      1024
#define SEG_CMP_LENGTH                 128

#define QUAD_POINT_NUM                 4

#define MAX_MMZ_NAME_LEN               32

#define VO_MAX_NODE_NUM                16

#define VENC_MAX_NAME_LEN              16
#define VENC_MAX_CHN_NUM               16
#define VDEC_MAX_CHN_NUM               4
#define VDEC_MAX_WIDTH                 5376
#define VDEC_MAX_HEIGHT                5376
#define VDEC_MIN_WIDTH                 128
#define VDEC_MIN_HEIGHT                128
#define VDEC_MAX_PIC_AREA              (VDEC_MAX_WIDTH * VDEC_MAX_HEIGHT)
#define VEDU_IP_NUM                    1
#define H264E_MAX_WIDTH                3840
#define H264E_MAX_HEIGHT               3840
#define H264E_MIN_WIDTH                128
#define H264E_MIN_HEIGHT               128
#define H265E_MAX_WIDTH                3840
#define H265E_MAX_HEIGHT               3840
#define H265E_MIN_WIDTH                128
#define H265E_MIN_HEIGHT               128
#define JPEGE_MAX_WIDTH                8192
#define JPEGE_MAX_HEIGHT               8192
#define JPEGE_MIN_WIDTH                120
#define JPEGE_MIN_HEIGHT               120
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
#define VENC_RGN_MAX_X                 3838
#define VENC_RGN_MAX_Y                 2158
#define VENC_RGN_MAX_WIDTH             3840
#define VENC_RGN_MAX_HEIGHT            2160
#define VECN_RGN_COVER_MIN_X           0
#define VECN_RGN_COVER_MIN_Y           0

#define VENC_MIN_INPUT_FRAME_RATE      1
#define VENC_MAX_INPUT_FRAME_RATE      240

#define VENC_MAX_RECEIVE_SOURCE        4

#define VENC_PIC_RECEIVE_SOURCE0       0
#define VENC_PIC_RECEIVE_SOURCE1       1
#define VENC_PIC_RECEIVE_SOURCE2       2
#define VENC_PIC_RECEIVE_SOURCE3       3

#define RC_BG_THR_SIZE                 8
#define MIN_BITRATE                    2
#define MAX_BITRATE                    80 * 1024
#define MAX_EXTRA_BITRATE              1000 * 1024

#define RGN_MIN_WIDTH                  2
#define RGN_MIN_HEIGHT                 2

#define RGN_COVER_RECT_MIN_X           0
#define RGN_COVER_RECT_MIN_Y           0
#define RGN_COVER_RECT_MAX_X           8190
#define RGN_COVER_RECT_MAX_Y           8190
#define RGN_COVER_QUAD_MIN_X           0
#define RGN_COVER_QUAD_MIN_Y           0
#define RGN_COVER_QUAD_MAX_X           8192
#define RGN_COVER_QUAD_MAX_Y           8192
#define RGN_COVER_MAX_WIDTH            8192
#define RGN_COVER_MAX_HEIGHT           8192

#define RGN_COVEREX_RECT_MIN_X         0
#define RGN_COVEREX_RECT_MIN_Y         0
#define RGN_COVEREX_RECT_MAX_X         3838
#define RGN_COVEREX_RECT_MAX_Y         3838
#define RGN_COVEREX_QUAD_MIN_X         0
#define RGN_COVEREX_QUAD_MIN_Y         0
#define RGN_COVEREX_QUAD_MAX_X         3840
#define RGN_COVEREX_QUAD_MAX_Y         3840
#define RGN_COVEREX_MAX_WIDTH          3840
#define RGN_COVEREX_MAX_HEIGHT         3840
#define RGN_COVEREX_MIN_THICK          2
#define RGN_COVEREX_MAX_THICK          14

#define RGN_OVERLAY_MIN_X              0
#define RGN_OVERLAY_MIN_Y              0
#define RGN_OVERLAY_MAX_X              8190
#define RGN_OVERLAY_MAX_Y              8190
#define RGN_OVERLAY_MAX_WIDTH          8192
#define RGN_OVERLAY_MAX_HEIGHT         8192

#define RGN_OVERLAYEX_MIN_X            0
#define RGN_OVERLAYEX_MIN_Y            0
#define RGN_OVERLAYEX_MAX_X            3838
#define RGN_OVERLAYEX_MAX_Y            3838
#define RGN_OVERLAYEX_MAX_WIDTH        3840
#define RGN_OVERLAYEX_MAX_HEIGHT       3840

#define RGN_MOSAIC_MIN_X               0
#define RGN_MOSAIC_MIN_Y               0
#define RGN_MOSAIC_MAX_X               8184
#define RGN_MOSAIC_MAX_Y               8184
#define RGN_MOSAIC_MIN_WIDTH           8
#define RGN_MOSAIC_MIN_HEIGHT          8
#define RGN_MOSAIC_MAX_WIDTH           8192
#define RGN_MOSAIC_MAX_HEIGHT          8192

#define RGN_MOSAICEX_MIN_X             0
#define RGN_MOSAICEX_MIN_Y             0
#define RGN_MOSAICEX_MAX_X             3832
#define RGN_MOSAICEX_MAX_Y             3832
#define RGN_MOSAICEX_MIN_WIDTH         8
#define RGN_MOSAICEX_MIN_HEIGHT        8
#define RGN_MOSAICEX_MAX_WIDTH         3840
#define RGN_MOSAICEX_MAX_HEIGHT        3840

#define RGN_CORNER_RECT_MIN_X          0
#define RGN_CORNER_RECT_MIN_Y          0
#define RGN_CORNER_RECT_MAX_X          8190
#define RGN_CORNER_RECT_MAX_Y          8190
#define RGN_CORNER_RECT_MIN_WIDTH      2
#define RGN_CORNER_RECT_MIN_HEIGHT     2
#define RGN_CORNER_RECT_MAX_WIDTH      8192
#define RGN_CORNER_RECT_MAX_HEIGHT     8192
#define RGN_CORNER_RECT_MIN_THICK      2
#define RGN_CORNER_RECT_MAX_THICK      16
#define RGN_CORNER_RECT_MIN_HOR_LEN    2
#define RGN_CORNER_RECT_MAX_HOR_LEN    4096
#define RGN_CORNER_RECT_MIN_VER_LEN    2
#define RGN_CORNER_RECT_MAX_VER_LEN    4096

#define RGN_CORNER_RECTEX_MIN_X        0
#define RGN_CORNER_RECTEX_MIN_Y        0
#define RGN_CORNER_RECTEX_MAX_X        3838
#define RGN_CORNER_RECTEX_MAX_Y        3838
#define RGN_CORNER_RECTEX_MIN_WIDTH    2
#define RGN_CORNER_RECTEX_MIN_HEIGHT   2
#define RGN_CORNER_RECTEX_MAX_WIDTH    3840
#define RGN_CORNER_RECTEX_MAX_HEIGHT   3840
#define RGN_CORNER_RECTEX_MIN_THICK    2
#define RGN_CORNER_RECTEX_MAX_THICK    16
#define RGN_CORNER_RECTEX_MIN_HOR_LEN  2
#define RGN_CORNER_RECTEX_MAX_HOR_LEN  1920
#define RGN_CORNER_RECTEX_MIN_VER_LEN  2
#define RGN_CORNER_RECTEX_MAX_VER_LEN  1920

#define RGN_ALIGN                      2

#define RGN_HANDLE_MAX                 128
#define RGN_MAX_BUF_NUM                6

#define RGN_MOSAIC_MAX_NUM_VPSS            0
#define RGN_MOSAICEX_MAX_NUM_VPSS          4
#define RGN_COVER_MAX_NUM_VPSS             0
#define RGN_COVEREX_MAX_NUM_VPSS           8
#define RGN_COVEREX_MAX_NUM_VO             1
#define RGN_OVERLAY_MAX_NUM_VENC           8
#define RGN_MOSAIC_MAX_NUM_VENC            8
#define RGN_COVER_MAX_NUM_VENC             8
#define RGN_CORNER_MAX_NUM_VENC            8
#define RGN_OVERLAYEX_MAX_NUM_VPSS         8
#define RGN_OVERLAYEX_MAX_NUM_VO           1
#define RGN_CORNER_RECT_MAX_NUM_VPSS       0
#define RGN_CORNER_RECTEX_MAX_NUM_VPSS     32
#define RGN_CORNER_RECTEX_MAX_NUM_VO       32


#define VENC_MAX_SSE_NUM               8
#define SVP_MAX_SENSOR_NUM            2

#define VI_MAX_DEV_NUM                 3
#define VI_MAX_PHY_PIPE_NUM            4
#define VI_MAX_VIR_PIPE_NUM            2
#define VI_MAX_PIPE_NUM                (VI_MAX_PHY_PIPE_NUM + VI_MAX_VIR_PIPE_NUM)

#define VI_MAX_STITCH_GRP_NUM          1
#define VI_MAX_WDR_NUM                 2

#define VI_MAX_PHY_CHN_NUM             1

#define VI_MAX_STITCH_NUM              1
#define VI_MIPI_RX_NUM                 2
#define VI_MAX_GRP_NUM                 2

#define VI_MAX_EXT_CHN_NUM             4
#define VI_EXT_CHN_START               VI_MAX_PHY_CHN_NUM
#define VI_MAX_CHN_NUM                 (VI_MAX_PHY_CHN_NUM + VI_MAX_EXT_CHN_NUM)
#define VI_MAX_EXTCHN_BIND_PER_CHN     8

#define VIPROC_IRQ_NUM                 1
#define VI_MAX_WDR_FRAME_NUM           2
#define VI_MAX_NODE_NUM                2
#define VIPROC_IP_NUM                  1
#define VICAP_IP_NUM                   1

#define VI_MAX_SPLIT_NODE_NUM          2

#define VI_DEV_MIN_WIDTH               32
#define VI_DEV_MIN_HEIGHT              32
#define VI_DEV_MAX_WIDTH               3840
#define VI_DEV_MAX_HEIGHT              3840
#define VI_FPN_MAX_WIDTH               3840
#define VI_FPN_MAX_HEIGHT              VI_DEV_MAX_HEIGHT

#define VI_PIPE_OFFLINE_MIN_WIDTH      32
#define VI_PIPE_OFFLINE_MIN_HEIGHT     32
#define VI_PIPE_OFFLINE_MAX_WIDTH      3840
#define VI_PIPE_OFFLINE_MAX_HEIGHT     3840

#define VI_PIPE_ONLINE_MIN_WIDTH       32
#define VI_PIPE_ONLINE_MIN_HEIGHT      32

#define VI_PIPE_ONLINE_MAX_WIDTH       3200

#define VI_PIPE_ONLINE_MAX_HEIGHT      3840

#define VI_PHYCHN_OFFLINE_MIN_WIDTH    32
#define VI_PHYCHN_OFFLINE_MIN_HEIGHT   32
#define VI_PHYCHN_OFFLINE_MAX_WIDTH    3840
#define VI_PHYCHN_OFFLINE_MAX_HEIGHT   3840

#define VI_PHYCHN_ONLINE_MIN_WIDTH     32
#define VI_PHYCHN_ONLINE_MIN_HEIGHT    32

#define VI_PHYCHN_ONLINE_MAX_WIDTH     3200

#define VI_PHYCHN_ONLINE_MAX_HEIGHT    3840

#define VI_EXTCHN_MIN_WIDTH            32
#define VI_EXTCHN_MIN_HEIGHT           32
#define VI_EXTCHN_MAX_WIDTH            3840
#define VI_EXTCHN_MAX_HEIGHT           3840

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

#define VO_MIN_CHN_WIDTH               32
#define VO_MIN_CHN_HEIGHT              32
#define VO_MAX_ZOOM_RATIO              1000
#define VO_MAX_DEV_NUM                 4
#define VO_MAX_LAYER_NUM               4
#define VO_MAX_CHN_NUM                 4
#define VO_MULTI_MODE_MAX_CHN_NUM      2
#define VO_MIN_TOLERATE                1
#define VO_MAX_TOLERATE                100000
#define VO_GAMMA_MAP_SIZE              256
#define FB_MAX_LAYER_NUM               1

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
#define VPSS_LINE_BUFFER               3200

#define VPSS_MAX_STITCH_NUM            4
#define VPSS_MIN_STITCH_IN_WIDTH       640
#define VPSS_MIN_STITCH_IN_HEIGHT      360

#define VPSS_MAX_2_STITCH_IN_WIDTH     2688
#define VPSS_MAX_2_STITCH_IN_HEIGHT    2688
#define VPSS_MAX_2_STITCH_OUT_WIDTH    5374
#define VPSS_MAX_2_STITCH_OUT_HEIGHT   2688

#define VPSS_MAX_3_STITCH_IN_WIDTH     1920
#define VPSS_MAX_3_STITCH_IN_HEIGHT    1920
#define VPSS_MAX_3_STITCH_OUT_WIDTH    5756
#define VPSS_MAX_3_STITCH_OUT_HEIGHT   1920

#define VPSS_MAX_4_STITCH_IN_WIDTH     1920
#define VPSS_MAX_4_STITCH_IN_HEIGHT    1920
#define VPSS_MAX_4_STITCH_OUT_WIDTH    7674
#define VPSS_MAX_4_STITCH_OUT_HEIGHT   1920

#define VPSS_MIN_STITCH_OVERLAP_WIDTH  2
#define VPSS_MIN_STITCH_OVERLAP_WIDTH_CURVE 16
#define VPSS_MAX_STITCH_OVERLAP_WIDTH  1022
#define VPSS_MAX_PIPE_NUM              8
#define VPSS_MAX_ICHN_NUM              1
#define VPSS_MAX_PHY_OCHN_NUM          3
#define VPSS_MAX_EXT_OCHN_NUM          4
#define VPSS_MAX_OCHN_NUM              (VPSS_MAX_PHY_OCHN_NUM + VPSS_MAX_EXT_OCHN_NUM)

#define VPSS_WIDTH_ALIGN               2
#define VPSS_HEIGHT_ALIGN              2

#define VPSS_MIN_ICHN_WIDTH            32
#define VPSS_MIN_ICHN_HEIGHT           32
#define VPSS_MAX_ICHN_WIDTH            3840
#define VPSS_MAX_ICHN_HEIGHT           3840

#define VPSS_MIN_PHY_OCHN_WIDTH        32
#define VPSS_MIN_PHY_OCHN_HEIGHT       32
#define VPSS_MAX_PHY_OCHN_WIDTH        4032
#define VPSS_MAX_PHY_OCHN_HEIGHT       3120

#define VPSS_MIN_EXT_OCHN_WIDTH        32
#define VPSS_MIN_EXT_OCHN_HEIGHT       32
#define VPSS_MAX_EXT_OCHN_WIDTH        3840
#define VPSS_MAX_EXT_OCHN_HEIGHT       3840

#define VPSS_MAX_ONLINE_PHY_OCHN_ZOOMIN 2
#define VPSS_MAX_PHY_OCHN_ZOOMIN       16
#define VPSS_MAX_PHY_OCHN_ZOOMOUT      15
#define VPSS_MAX_EXT_OCHN_ZOOMIN       16
#define VPSS_MAX_EXT_OCHN_ZOOMOUT      15

#define VPSS_MAX_RGN_LUMA_NUM          8
#define VPSS_MAX_RGN_LUMA_WIDTH        3840
#define VPSS_MAX_RGN_LUMA_HEIGHT       2160
#define VPSS_MAX_RGN_LUMA_EX_NUM       32
#define VPSS_MAX_RGN_LUMA_EX_WIDTH     3840
#define VPSS_MAX_RGN_LUMA_EX_HEIGHT    3840

#define VPSS_MAX_SPLICING_GRP_NUM      2

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

#define TDE_IP_NUM                     1

#define MCF_MAX_PIPE_NUM               2
#define MCF_MAX_ICHN_NUM               2
#define MCF_MAX_OCHN_NUM               1

#define MCF_MIN_IN_WIDTH               128
#define MCF_MIN_IN_HEIGHT              128
#define MCF_MAX_IN_WIDTH               4096
#define MCF_MAX_IN_HEIGHT              2160

#define ISP_ISO_MODEL_MAX_NUM          1
#define ISP_WDR_MAX_EXP_RATIO          32
#define ISP_TRIG_DEV_MAX_NUM           1

#define AINR_MODEL_MAX_NUM             ISP_ISO_MODEL_MAX_NUM

#ifdef __cplusplus
}
#endif

#endif
