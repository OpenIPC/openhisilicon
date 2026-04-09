/*
 * SPDX-License-Identifier: GPL-2.0
 *
 * hi_ive.h — IVE operation control-struct definitions.
 *
 * All struct layouts match the vendor libive.so ABI, verified against:
 *   - IDA decomp at ~/libive/mpi_ive.o.c (memcpy_s offsets for ioctl args)
 *   - ive_neo.c kernel driver dispatch expectations
 *
 * Only the MVP op set needed by qemu-boot/test-*.c is declared. Ops the
 * EV200/EV300 hardware does not support are omitted.
 */
#ifndef __HI_IVE_H__
#define __HI_IVE_H__

#include "hi_type.h"
#include "hi_common.h"
#include "hi_comm_ive.h"

#ifdef __cplusplus
extern "C" {
#endif

#define IVE_HIST_NUM           256
#define IVE_MAP_NUM            256
#define IVE_MAX_REGION_NUM     254
#define IVE_ST_MAX_CORNER_NUM  500

/* --- DMA --- */

typedef enum hiIVE_DMA_MODE_E {
    IVE_DMA_MODE_DIRECT_COPY   = 0x0,
    IVE_DMA_MODE_INTERVAL_COPY = 0x1,
    IVE_DMA_MODE_SET_3BYTE     = 0x2,
    IVE_DMA_MODE_SET_8BYTE     = 0x3,
    IVE_DMA_MODE_BUTT,
} IVE_DMA_MODE_E;

typedef struct hiIVE_DMA_CTRL_S {
    IVE_DMA_MODE_E enMode;
    HI_U64         u64Val;
    HI_U8          u8HorSegSize;
    HI_U8          u8ElemSize;
    HI_U8          u8VerSegRows;
} IVE_DMA_CTRL_S;

/* --- Filter --- */

typedef struct hiIVE_FILTER_CTRL_S {
    HI_S8 as8Mask[25];
    HI_U8 u8Norm;
} IVE_FILTER_CTRL_S;

/* --- Sobel --- */

typedef enum hiIVE_SOBEL_OUT_CTRL_E {
    IVE_SOBEL_OUT_CTRL_BOTH = 0x0,
    IVE_SOBEL_OUT_CTRL_HOR  = 0x1,
    IVE_SOBEL_OUT_CTRL_VER  = 0x2,
    IVE_SOBEL_OUT_CTRL_BUTT,
} IVE_SOBEL_OUT_CTRL_E;

typedef struct hiIVE_SOBEL_CTRL_S {
    IVE_SOBEL_OUT_CTRL_E enOutCtrl;
    HI_S8                as8Mask[25];
} IVE_SOBEL_CTRL_S;

/* --- MagAndAng --- */

typedef enum hiIVE_MAG_AND_ANG_OUT_CTRL_E {
    IVE_MAG_AND_ANG_OUT_CTRL_MAG         = 0x0,
    IVE_MAG_AND_ANG_OUT_CTRL_MAG_AND_ANG = 0x1,
    IVE_MAG_AND_ANG_OUT_CTRL_BUTT,
} IVE_MAG_AND_ANG_OUT_CTRL_E;

typedef struct hiIVE_MAG_AND_ANG_CTRL_S {
    IVE_MAG_AND_ANG_OUT_CTRL_E enOutCtrl;
    HI_U16                     u16Thr;
    HI_S8                      as8Mask[25];
} IVE_MAG_AND_ANG_CTRL_S;

/* --- Dilate / Erode --- */

typedef struct hiIVE_DILATE_CTRL_S {
    HI_U8 au8Mask[25];
} IVE_DILATE_CTRL_S;

typedef struct hiIVE_ERODE_CTRL_S {
    HI_U8 au8Mask[25];
} IVE_ERODE_CTRL_S;

/* --- Thresh --- */

typedef enum hiIVE_THRESH_MODE_E {
    IVE_THRESH_MODE_BINARY      = 0x0,
    IVE_THRESH_MODE_TRUNC       = 0x1,
    IVE_THRESH_MODE_TO_MINVAL   = 0x2,
    IVE_THRESH_MODE_MIN_MID_MAX = 0x3,
    IVE_THRESH_MODE_ORI_MID_MAX = 0x4,
    IVE_THRESH_MODE_MIN_MID_ORI = 0x5,
    IVE_THRESH_MODE_MIN_ORI_MAX = 0x6,
    IVE_THRESH_MODE_ORI_MID_ORI = 0x7,
    IVE_THRESH_MODE_BUTT,
} IVE_THRESH_MODE_E;

typedef struct hiIVE_THRESH_CTRL_S {
    IVE_THRESH_MODE_E enMode;
    HI_U8             u8LowThr;
    HI_U8             u8HighThr;
    HI_U8             u8MinVal;
    HI_U8             u8MidVal;
    HI_U8             u8MaxVal;
} IVE_THRESH_CTRL_S;

/* --- Sub --- */

typedef enum hiIVE_SUB_MODE_E {
    IVE_SUB_MODE_ABS   = 0x0,
    IVE_SUB_MODE_SHIFT = 0x1,
    IVE_SUB_MODE_BUTT,
} IVE_SUB_MODE_E;

typedef struct hiIVE_SUB_CTRL_S {
    IVE_SUB_MODE_E enMode;
} IVE_SUB_CTRL_S;

/* --- Integ --- */

typedef enum hiIVE_INTEG_OUT_CTRL_E {
    IVE_INTEG_OUT_CTRL_COMBINE = 0x0,
    IVE_INTEG_OUT_CTRL_SUM     = 0x1,
    IVE_INTEG_OUT_CTRL_SQSUM   = 0x2,
    IVE_INTEG_OUT_CTRL_BUTT,
} IVE_INTEG_OUT_CTRL_E;

typedef struct hiIVE_INTEG_CTRL_S {
    IVE_INTEG_OUT_CTRL_E enOutCtrl;
} IVE_INTEG_CTRL_S;

/* --- Thresh_S16 / Thresh_U16 --- */

typedef enum hiIVE_THRESH_S16_MODE_E {
    IVE_THRESH_S16_MODE_S16_TO_S8_MIN_MID_MAX = 0x0,
    IVE_THRESH_S16_MODE_S16_TO_S8_MIN_ORI_MAX = 0x1,
    IVE_THRESH_S16_MODE_S16_TO_U8_MIN_MID_MAX = 0x2,
    IVE_THRESH_S16_MODE_S16_TO_U8_MIN_ORI_MAX = 0x3,
    IVE_THRESH_S16_MODE_BUTT,
} IVE_THRESH_S16_MODE_E;

typedef struct hiIVE_THRESH_S16_CTRL_S {
    IVE_THRESH_S16_MODE_E enMode;
    HI_S16                s16LowThr;
    HI_S16                s16HighThr;
    IVE_8BIT_U            un8MinVal;
    IVE_8BIT_U            un8MidVal;
    IVE_8BIT_U            un8MaxVal;
} IVE_THRESH_S16_CTRL_S;

typedef enum hiIVE_THRESH_U16_MODE_E {
    IVE_THRESH_U16_MODE_U16_TO_U8_MIN_MID_MAX = 0x0,
    IVE_THRESH_U16_MODE_U16_TO_U8_MIN_ORI_MAX = 0x1,
    IVE_THRESH_U16_MODE_BUTT,
} IVE_THRESH_U16_MODE_E;

typedef struct hiIVE_THRESH_U16_CTRL_S {
    IVE_THRESH_U16_MODE_E enMode;
    HI_U16                u16LowThr;
    HI_U16                u16HighThr;
    HI_U8                 u8MinVal;
    HI_U8                 u8MidVal;
    HI_U8                 u8MaxVal;
} IVE_THRESH_U16_CTRL_S;

/* --- 16BitTo8Bit --- */

typedef enum hiIVE_16BIT_TO_8BIT_MODE_E {
    IVE_16BIT_TO_8BIT_MODE_S16_TO_S8      = 0x0,
    IVE_16BIT_TO_8BIT_MODE_S16_TO_U8_ABS  = 0x1,
    IVE_16BIT_TO_8BIT_MODE_S16_TO_U8_BIAS = 0x2,
    IVE_16BIT_TO_8BIT_MODE_U16_TO_U8      = 0x3,
    IVE_16BIT_TO_8BIT_MODE_BUTT,
} IVE_16BIT_TO_8BIT_MODE_E;

typedef struct hiIVE_16BIT_TO_8BIT_CTRL_S {
    IVE_16BIT_TO_8BIT_MODE_E enMode;
    HI_U16                   u16Denominator;
    HI_U8                    u8Numerator;
    HI_S8                    s8Bias;
} IVE_16BIT_TO_8BIT_CTRL_S;

/* --- OrdStatFilter --- */

typedef enum hiIVE_ORD_STAT_FILTER_MODE_E {
    IVE_ORD_STAT_FILTER_MODE_MEDIAN = 0x0,
    IVE_ORD_STAT_FILTER_MODE_MAX    = 0x1,
    IVE_ORD_STAT_FILTER_MODE_MIN    = 0x2,
    IVE_ORD_STAT_FILTER_MODE_BUTT,
} IVE_ORD_STAT_FILTER_MODE_E;

typedef struct hiIVE_ORD_STAT_FILTER_CTRL_S {
    IVE_ORD_STAT_FILTER_MODE_E enMode;
} IVE_ORD_STAT_FILTER_CTRL_S;

/* --- Map --- */

typedef enum hiIVE_MAP_MODE_E {
    IVE_MAP_MODE_U8  = 0x0,
    IVE_MAP_MODE_S16 = 0x1,
    IVE_MAP_MODE_U16 = 0x2,
    IVE_MAP_MODE_BUTT,
} IVE_MAP_MODE_E;

typedef struct hiIVE_MAP_CTRL_S {
    IVE_MAP_MODE_E enMode;
} IVE_MAP_CTRL_S;

typedef struct hiIVE_MAP_U8BIT_LUT_MEM_S {
    HI_U8 au8Map[IVE_MAP_NUM];
} IVE_MAP_U8BIT_LUT_MEM_S;

typedef struct hiIVE_MAP_U16BIT_LUT_MEM_S {
    HI_U16 au16Map[IVE_MAP_NUM];
} IVE_MAP_U16BIT_LUT_MEM_S;

typedef struct hiIVE_MAP_S16BIT_LUT_MEM_S {
    HI_S16 as16Map[IVE_MAP_NUM];
} IVE_MAP_S16BIT_LUT_MEM_S;

/* --- EqualizeHist --- */

typedef struct hiIVE_EQUALIZE_HIST_CTRL_MEM_S {
    HI_U32 au32Hist[IVE_HIST_NUM];
    HI_U8  au8Map[IVE_MAP_NUM];
} IVE_EQUALIZE_HIST_CTRL_MEM_S;

typedef struct hiIVE_EQUALIZE_HIST_CTRL_S {
    IVE_MEM_INFO_S stMem;
} IVE_EQUALIZE_HIST_CTRL_S;

/* --- Add --- */

typedef struct hiIVE_ADD_CTRL_S {
    HI_U0Q16 u0q16X;
    HI_U0Q16 u0q16Y;
} IVE_ADD_CTRL_S;

/* --- CCL --- */

typedef struct hiIVE_REGION_S {
    HI_U32 u32Area;
    HI_U16 u16Left;
    HI_U16 u16Right;
    HI_U16 u16Top;
    HI_U16 u16Bottom;
} IVE_REGION_S;

typedef struct hiIVE_CCBLOB_S {
    HI_U16       u16CurAreaThr;
    HI_S8        s8LabelStatus;
    HI_U8        u8RegionNum;
    IVE_REGION_S astRegion[IVE_MAX_REGION_NUM];
} IVE_CCBLOB_S;

typedef enum hiIVE_CCL_MODE_E {
    IVE_CCL_MODE_4C   = 0x0,
    IVE_CCL_MODE_8C   = 0x1,
    IVE_CCL_MODE_BUTT,
} IVE_CCL_MODE_E;

typedef struct hiIVE_CCL_CTRL_S {
    IVE_CCL_MODE_E enMode;
    HI_U16         u16InitAreaThr;
    HI_U16         u16Step;
} IVE_CCL_CTRL_S;

/* --- Canny --- */

typedef struct hiIVE_CANNY_HYS_EDGE_CTRL_S {
    IVE_MEM_INFO_S stMem;
    HI_U16         u16LowThr;
    HI_U16         u16HighThr;
    HI_S8          as8Mask[25];
} IVE_CANNY_HYS_EDGE_CTRL_S;

typedef struct hiIVE_CANNY_STACK_SIZE_S {
    HI_U32 u32StackSize;
    HI_U8  u8Reserved[12];
} IVE_CANNY_STACK_SIZE_S;

/* --- NormGrad --- */

typedef enum hiIVE_NORM_GRAD_OUT_CTRL_E {
    IVE_NORM_GRAD_OUT_CTRL_HOR_AND_VER = 0x0,
    IVE_NORM_GRAD_OUT_CTRL_HOR         = 0x1,
    IVE_NORM_GRAD_OUT_CTRL_VER         = 0x2,
    IVE_NORM_GRAD_OUT_CTRL_COMBINE     = 0x3,
    IVE_NORM_GRAD_OUT_CTRL_BUTT,
} IVE_NORM_GRAD_OUT_CTRL_E;

typedef struct hiIVE_NORM_GRAD_CTRL_S {
    IVE_NORM_GRAD_OUT_CTRL_E enOutCtrl;
    HI_S8                    as8Mask[25];
    HI_U8                    u8Norm;
} IVE_NORM_GRAD_CTRL_S;

/* --- LK Optical Flow (pyramid) --- */

typedef enum hiIVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_E {
    IVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_NONE   = 0,
    IVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_STATUS = 1,
    IVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_BOTH   = 2,
    IVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_BUTT,
} IVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_E;

typedef struct hiIVE_LK_OPTICAL_FLOW_PYR_CTRL_S {
    IVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_E enOutMode;
    HI_BOOL  bUseInitFlow;
    HI_U16   u16PtsNum;
    HI_U8    u8MaxLevel;
    HI_U0Q8  u0q8MinEigThr;
    HI_U8    u8IterCnt;
    HI_U0Q8  u0q8Eps;
} IVE_LK_OPTICAL_FLOW_PYR_CTRL_S;

/* --- Shi-Tomasi Corners --- */

typedef struct hiIVE_ST_MAX_EIG_S {
    HI_U16 u16MaxEig;
    HI_U8  u8Reserved[14];
} IVE_ST_MAX_EIG_S;

typedef struct hiIVE_ST_CANDI_CORNER_CTRL_S {
    IVE_MEM_INFO_S stMem;
    HI_U0Q8        u0q8QualityLevel;
} IVE_ST_CANDI_CORNER_CTRL_S;

typedef struct hiIVE_ST_CORNER_INFO_S {
    HI_U16           u16CornerNum;
    IVE_POINT_U16_S  astCorner[IVE_ST_MAX_CORNER_NUM];
} IVE_ST_CORNER_INFO_S;

typedef struct hiIVE_ST_CORNER_CTRL_S {
    HI_U16 u16MaxCornerNum;
    HI_U16 u16MinDist;
} IVE_ST_CORNER_CTRL_S;

/* --- GMM2 (2-component Gaussian Mixture Model) --- */

typedef enum hiIVE_GMM2_SNS_FACTOR_MODE_E {
    IVE_GMM2_SNS_FACTOR_MODE_GLB  = 0x0,
    IVE_GMM2_SNS_FACTOR_MODE_PIX  = 0x1,
    IVE_GMM2_SNS_FACTOR_MODE_BUTT,
} IVE_GMM2_SNS_FACTOR_MODE_E;

typedef enum hiIVE_GMM2_LIFE_UPDATE_FACTOR_MODE_E {
    IVE_GMM2_LIFE_UPDATE_FACTOR_MODE_GLB  = 0x0,
    IVE_GMM2_LIFE_UPDATE_FACTOR_MODE_PIX  = 0x1,
    IVE_GMM2_LIFE_UPDATE_FACTOR_MODE_BUTT,
} IVE_GMM2_LIFE_UPDATE_FACTOR_MODE_E;

typedef struct hiIVE_GMM2_CTRL_S {
    IVE_GMM2_SNS_FACTOR_MODE_E         enSnsFactorMode;
    IVE_GMM2_LIFE_UPDATE_FACTOR_MODE_E enLifeUpdateFactorMode;
    HI_U16   u16GlbLifeUpdateFactor;
    HI_U16   u16LifeThr;
    HI_U16   u16FreqInitVal;
    HI_U16   u16FreqReduFactor;
    HI_U16   u16FreqAddFactor;
    HI_U16   u16FreqThr;
    HI_U16   u16VarRate;
    HI_U9Q7  u9q7MaxVar;
    HI_U9Q7  u9q7MinVar;
    HI_U8    u8GlbSnsFactor;
    HI_U8    u8ModelNum;
} IVE_GMM2_CTRL_S;

/* --- SAD --- */

typedef enum hiIVE_SAD_MODE_E {
    IVE_SAD_MODE_MB_4X4   = 0x0,
    IVE_SAD_MODE_MB_8X8   = 0x1,
    IVE_SAD_MODE_MB_16X16 = 0x2,
    IVE_SAD_MODE_BUTT,
} IVE_SAD_MODE_E;

typedef enum hiIVE_SAD_OUT_CTRL_E {
    IVE_SAD_OUT_CTRL_16BIT_BOTH = 0x0,
    IVE_SAD_OUT_CTRL_8BIT_BOTH  = 0x1,
    IVE_SAD_OUT_CTRL_16BIT_SAD  = 0x2,
    IVE_SAD_OUT_CTRL_8BIT_SAD   = 0x3,
    IVE_SAD_OUT_CTRL_THRESH     = 0x4,
    IVE_SAD_OUT_CTRL_BUTT,
} IVE_SAD_OUT_CTRL_E;

typedef struct hiIVE_SAD_CTRL_S {
    IVE_SAD_MODE_E     enMode;
    IVE_SAD_OUT_CTRL_E enOutCtrl;
    HI_U16             u16Thr;
    HI_U8              u8MinVal;
    HI_U8              u8MaxVal;
} IVE_SAD_CTRL_S;

/* --- CNN model (used by test-cnn-model.c, test-ive-mpi.c) --- */

typedef enum hiIVE_CNN_ACTIV_FUNC_E {
    IVE_CNN_ACTIV_FUNC_NONE    = 0x0,
    IVE_CNN_ACTIV_FUNC_RELU    = 0x1,
    IVE_CNN_ACTIV_FUNC_SIGMOID = 0x2,
    IVE_CNN_ACTIV_FUNC_BUTT,
} IVE_CNN_ACTIV_FUNC_E;

typedef enum hiIVE_CNN_POOLING_E {
    IVE_CNN_POOLING_NONE = 0x0,
    IVE_CNN_POOLING_MAX  = 0x1,
    IVE_CNN_POOLING_AVG  = 0x2,
    IVE_CNN_POOLING_BUTT,
} IVE_CNN_POOLING_E;

typedef struct hiIVE_CNN_CONV_POOLING_S {
    IVE_CNN_ACTIV_FUNC_E enActivFunc;
    IVE_CNN_POOLING_E    enPooling;
    HI_U8 u8FeatureMapNum;
    HI_U8 u8KernelSize;
    HI_U8 u8ConvStep;
    HI_U8 u8PoolSize;
    HI_U8 u8PoolStep;
    HI_U8 u8Reserved[3];
} IVE_CNN_CONV_POOLING_S;

typedef struct hiIVE_CNN_FULL_CONNECT_S {
    HI_U16 au16LayerCnt[8];
    HI_U16 u16MaxCnt;
    HI_U8  u8LayerNum;
    HI_U8  u8Reserved;
} IVE_CNN_FULL_CONNECT_S;

typedef struct hiIVE_CNN_MODEL_S {
    IVE_CNN_CONV_POOLING_S astConvPool[8];
    IVE_CNN_FULL_CONNECT_S stFullConnect;
    IVE_MEM_INFO_S         stConvKernelBias;
    HI_U32                 u32ConvKernelBiasSize;
    IVE_MEM_INFO_S         stFCLWgtBias;
    HI_U32                 u32FCLWgtBiasSize;
    HI_U32                 u32TotalMemSize;
    IVE_IMAGE_TYPE_E       enType;
    HI_U32                 u32Width;
    HI_U32                 u32Height;
    HI_U16                 u16ClassCount;
    HI_U8                  u8ConvPoolLayerNum;
    HI_U8                  u8Reserved;
} IVE_CNN_MODEL_S;

typedef struct hiIVE_CNN_CTRL_S {
    IVE_MEM_INFO_S stMem;
    HI_U32         u32Num;
} IVE_CNN_CTRL_S;

typedef struct hiIVE_CNN_RESULT_S {
    HI_S32 s32ClassIdx;
    HI_S32 s32Confidence;
} IVE_CNN_RESULT_S;

/*
 * -----------------------------------------------------------------------
 * Unsupported-on-EV200/EV300 ops
 * -----------------------------------------------------------------------
 * These types are provided so test-ive-mpi.c and similar comprehensive
 * op-test code can compile and link. The corresponding wrappers in
 * libive_neo return HI_ERR_IVE_NOT_SUPPORT immediately without issuing
 * any ioctl (because the vendor kernel also rejects them on this SoC).
 */

/* --- CSC / FilterAndCSC --- */

typedef enum hiIVE_CSC_MODE_E {
    IVE_CSC_MODE_VIDEO_BT601_YUV2RGB = 0x0,
    IVE_CSC_MODE_VIDEO_BT709_YUV2RGB = 0x1,
    IVE_CSC_MODE_PIC_BT601_YUV2RGB   = 0x2,
    IVE_CSC_MODE_PIC_BT709_YUV2RGB   = 0x3,
    IVE_CSC_MODE_PIC_BT601_YUV2HSV   = 0x4,
    IVE_CSC_MODE_PIC_BT709_YUV2HSV   = 0x5,
    IVE_CSC_MODE_PIC_BT601_YUV2LAB   = 0x6,
    IVE_CSC_MODE_PIC_BT709_YUV2LAB   = 0x7,
    IVE_CSC_MODE_VIDEO_BT601_RGB2YUV = 0x8,
    IVE_CSC_MODE_VIDEO_BT709_RGB2YUV = 0x9,
    IVE_CSC_MODE_PIC_BT601_RGB2YUV   = 0xa,
    IVE_CSC_MODE_PIC_BT709_RGB2YUV   = 0xb,
    IVE_CSC_MODE_BUTT,
} IVE_CSC_MODE_E;

typedef struct hiIVE_CSC_CTRL_S {
    IVE_CSC_MODE_E enMode;
} IVE_CSC_CTRL_S;

typedef struct hiIVE_FILTER_AND_CSC_CTRL_S {
    IVE_CSC_MODE_E enMode;
    HI_S8          as8Mask[25];
    HI_U8          u8Norm;
} IVE_FILTER_AND_CSC_CTRL_S;

/* --- Resize --- */

typedef enum hiIVE_RESIZE_MODE_E {
    IVE_RESIZE_MODE_LINEAR = 0x0,
    IVE_RESIZE_MODE_AREA   = 0x1,
    IVE_RESIZE_MODE_BUTT,
} IVE_RESIZE_MODE_E;

typedef struct hiIVE_RESIZE_CTRL_S {
    IVE_RESIZE_MODE_E enMode;
    IVE_MEM_INFO_S    stMem;
    HI_U16            u16Num;
} IVE_RESIZE_CTRL_S;

/* --- LBP --- */

typedef enum hiIVE_LBP_CMP_MODE_E {
    IVE_LBP_CMP_MODE_NORMAL = 0x0,
    IVE_LBP_CMP_MODE_ABS    = 0x1,
    IVE_LBP_CMP_MODE_BUTT,
} IVE_LBP_CMP_MODE_E;

typedef struct hiIVE_LBP_CTRL_S {
    IVE_LBP_CMP_MODE_E enMode;
    IVE_8BIT_U         un8BitThr;
} IVE_LBP_CTRL_S;

/* --- NCC --- */

typedef struct hiIVE_NCC_DST_MEM_S {
    HI_U64 u64Numerator;
    HI_U64 u64QuadSum1;
    HI_U64 u64QuadSum2;
    HI_U8  u8Reserved[8];
} IVE_NCC_DST_MEM_S;

/* --- GMM (1-component, legacy) --- */

typedef struct hiIVE_GMM_CTRL_S {
    HI_U22Q10 u22q10NoiseVar;
    HI_U22Q10 u22q10MaxVar;
    HI_U22Q10 u22q10MinVar;
    HI_U0Q16  u0q16LearnRate;
    HI_U0Q16  u0q16BgRatio;
    HI_U8Q8   u8q8VarThr;
    HI_U0Q16  u0q16InitWeight;
    HI_U8     u8ModelNum;
} IVE_GMM_CTRL_S;

/* --- GradFg --- */

typedef enum hiIVE_GRAD_FG_MODE_E {
    IVE_GRAD_FG_MODE_USE_CUR_GRAD  = 0x0,
    IVE_GRAD_FG_MODE_FIND_MIN_GRAD = 0x1,
    IVE_GRAD_FG_MODE_BUTT,
} IVE_GRAD_FG_MODE_E;

typedef struct hiIVE_GRAD_FG_CTRL_S {
    IVE_GRAD_FG_MODE_E enMode;
    HI_U16 u16EdwFactor;
    HI_U8  u8CrlCoefThr;
    HI_U8  u8MagCrlThr;
    HI_U8  u8MinMagDiff;
    HI_U8  u8NoiseVal;
    HI_U8  u8EdwDark;
} IVE_GRAD_FG_CTRL_S;

/* --- Background model (Match/Update) --- */

typedef struct hiIVE_CANDI_BG_PIX_S {
    HI_U8Q4F4 u8q4f4Mean;
    HI_U16    u16StartTime;
    HI_U16    u16SumAccessTime;
    HI_U16    u16ShortKeepTime;
    HI_U8     u8ChgCond;
    HI_U8     u8PotenBgLife;
} IVE_CANDI_BG_PIX_S;

typedef struct hiIVE_WORK_BG_PIX_S {
    HI_U8Q4F4 u8q4f4Mean;
    HI_U16    u16AccTime;
    HI_U8     u8PreGray;
    HI_U5Q3   u5q3DiffThr;
    HI_U8     u8AccFlag;
    HI_U8     u8BgGray[3];
} IVE_WORK_BG_PIX_S;

typedef struct hiIVE_BG_LIFE_S {
    HI_U8 u8WorkBgLife[3];
    HI_U8 u8CandiBgLife;
} IVE_BG_LIFE_S;

typedef struct hiIVE_BG_MODEL_PIX_S {
    IVE_WORK_BG_PIX_S  stWorkBgPixel;
    IVE_CANDI_BG_PIX_S stCandiPixel;
    IVE_BG_LIFE_S      stBgLife;
} IVE_BG_MODEL_PIX_S;

typedef struct hiIVE_FG_STAT_DATA_S {
    HI_U32 u32PixNum;
    HI_U32 u32SumLum;
    HI_U8  u8Reserved[8];
} IVE_FG_STAT_DATA_S;

typedef struct hiIVE_BG_STAT_DATA_S {
    HI_U32 u32PixNum;
    HI_U32 u32SumLum;
    HI_U8  u8Reserved[8];
} IVE_BG_STAT_DATA_S;

typedef struct hiIVE_MATCH_BG_MODEL_CTRL_S {
    HI_U32 u32CurFrmNum;
    HI_U32 u32PreFrmNum;
    HI_U16 u16TimeThr;
    HI_U8  u8DiffThrCrlCoef;
    HI_U8  u8DiffMaxThr;
    HI_U8  u8DiffMinThr;
    HI_U8  u8DiffThrInc;
    HI_U8  u8FastLearnRate;
    HI_U8  u8DetChgRegion;
} IVE_MATCH_BG_MODEL_CTRL_S;

typedef struct hiIVE_UPDATE_BG_MODEL_CTRL_S {
    HI_U32 u32CurFrmNum;
    HI_U32 u32PreChkTime;
    HI_U32 u32FrmChkPeriod;
    HI_U32 u32InitMinTime;
    HI_U32 u32StyBgMinBlendTime;
    HI_U32 u32StyBgMaxBlendTime;
    HI_U32 u32DynBgMinBlendTime;
    HI_U32 u32StaticDetMinTime;
    HI_U16 u16FgMaxFadeTime;
    HI_U16 u16BgMaxFadeTime;
    HI_U8  u8StyBgAccTimeRateThr;
    HI_U8  u8ChgBgAccTimeRateThr;
    HI_U8  u8DynBgAccTimeThr;
    HI_U8  u8DynBgDepth;
    HI_U8  u8BgEffStaRateThr;
    HI_U8  u8AcceBgLearn;
    HI_U8  u8DetChgRegion;
} IVE_UPDATE_BG_MODEL_CTRL_S;

/* --- ANN/MLP classifier --- */

typedef enum hiIVE_ANN_MLP_ACTIV_FUNC_E {
    IVE_ANN_MLP_ACTIV_FUNC_IDENTITY    = 0x0,
    IVE_ANN_MLP_ACTIV_FUNC_SIGMOID_SYM = 0x1,
    IVE_ANN_MLP_ACTIV_FUNC_GAUSSIAN    = 0x2,
    IVE_ANN_MLP_ACTIV_FUNC_BUTT,
} IVE_ANN_MLP_ACTIV_FUNC_E;

typedef enum hiIVE_ANN_MLP_ACCURATE_E {
    IVE_ANN_MLP_ACCURATE_SRC16_WGT16 = 0x0,
    IVE_ANN_MLP_ACCURATE_SRC14_WGT20 = 0x1,
    IVE_ANN_MLP_ACCURATE_BUTT,
} IVE_ANN_MLP_ACCURATE_E;

typedef struct hiIVE_ANN_MLP_MODEL_S {
    IVE_ANN_MLP_ACTIV_FUNC_E enActivFunc;
    IVE_ANN_MLP_ACCURATE_E   enAccurate;
    IVE_MEM_INFO_S           stWeight;
    HI_U32                   u32TotalWeightSize;
    HI_U16                   au16LayerCount[8];
    HI_U16                   u16MaxCount;
    HI_U8                    u8LayerNum;
    HI_U8                    u8Reserved;
} IVE_ANN_MLP_MODEL_S;

/* --- SVM classifier --- */

typedef enum hiIVE_SVM_TYPE_E {
    IVE_SVM_TYPE_C_SVC  = 0x0,
    IVE_SVM_TYPE_NU_SVC = 0x1,
    IVE_SVM_TYPE_BUTT,
} IVE_SVM_TYPE_E;

typedef enum hiIVE_SVM_KERNEL_TYPE_E {
    IVE_SVM_KERNEL_TYPE_LINEAR  = 0x0,
    IVE_SVM_KERNEL_TYPE_POLY    = 0x1,
    IVE_SVM_KERNEL_TYPE_RBF     = 0x2,
    IVE_SVM_KERNEL_TYPE_SIGMOID = 0x3,
    IVE_SVM_KERNEL_TYPE_BUTT,
} IVE_SVM_KERNEL_TYPE_E;

typedef struct hiIVE_SVM_MODEL_S {
    IVE_SVM_TYPE_E        enType;
    IVE_SVM_KERNEL_TYPE_E enKernelType;
    IVE_MEM_INFO_S        stSv;
    IVE_MEM_INFO_S        stDf;
    HI_U32                u32TotalDfSize;
    HI_U16                u16FeatureDim;
    HI_U16                u16SvTotal;
    HI_U8                 u8ClassCount;
} IVE_SVM_MODEL_S;

/* --- PerspTrans --- */

typedef enum hiIVE_PERSP_TRANS_ALG_MODE_E {
    IVE_PERSP_TRANS_ALG_MODE_NR_SIM = 0x0,
    IVE_PERSP_TRANS_ALG_MODE_SIM    = 0x1,
    IVE_PERSP_TRANS_ALG_MODE_AFFINE = 0x2,
    IVE_PERSP_TRANS_ALG_MODE_BUTT,
} IVE_PERSP_TRANS_ALG_MODE_E;

typedef enum hiIVE_PERSP_TRANS_CSC_MODE_E {
    IVE_PERSP_TRANS_CSC_MODE_NONE                = 0x0,
    IVE_PERSP_TRANS_CSC_MODE_VIDEO_BT601_YUV2RGB = 0x1,
    IVE_PERSP_TRANS_CSC_MODE_VIDEO_BT709_YUV2RGB = 0x2,
    IVE_PERSP_TRANS_CSC_MODE_PIC_BT601_YUV2RGB   = 0x3,
    IVE_PERSP_TRANS_CSC_MODE_PIC_BT709_YUV2RGB   = 0x4,
    IVE_PERSP_TRANS_CSC_MODE_BUTT,
} IVE_PERSP_TRANS_CSC_MODE_E;

typedef struct hiIVE_PERSP_TRANS_POINT_PAIR_S {
    IVE_POINT_U14Q2_S stSrcPoint;
    IVE_POINT_U14Q2_S stDstPoint;
} IVE_PERSP_TRANS_POINT_PAIR_S;

typedef struct hiIVE_PERSP_TRANS_CTRL_S {
    IVE_PERSP_TRANS_ALG_MODE_E enAlgMode;
    IVE_PERSP_TRANS_CSC_MODE_E enCscMode;
    HI_U16                     u16RoiNum;
    HI_U16                     u16PointPairNum;
} IVE_PERSP_TRANS_CTRL_S;

typedef struct hiIVE_RECT_S24Q8_S {
    HI_S24Q8 s24q8X;
    HI_S24Q8 s24q8Y;
    HI_U32   u32Width;
    HI_U32   u32Height;
} IVE_RECT_S24Q8_S;

typedef struct hiIVE_ROI_INFO_S {
    IVE_RECT_S24Q8_S stRoi;
    HI_U32           u32RoiId;
} IVE_ROI_INFO_S;

/* --- HOG --- */

typedef enum hiIVE_HOG_MODE_E {
    IVE_HOG_MODE_VERTICAL_TANGENT_PLANE   = 0x1,
    IVE_HOG_MODE_HORIZONTAL_TANGENT_PLANE = 0x2,
    IVE_HOG_MODE_BUTT,
} IVE_HOG_MODE_E;

typedef struct hiIVE_HOG_CTRL_S {
    IVE_CSC_MODE_E enCscMode;
    IVE_HOG_MODE_E enHogMode;
    HI_U32         u32RoiNum;
    HI_U4Q12       u4q12TrancAlfa;
    HI_U8          au8Rsv[2];
} IVE_HOG_CTRL_S;

#ifdef __cplusplus
}
#endif

#endif /* __HI_IVE_H__ */
