
#ifndef __SVP_PERSPTRANS_H__
#define __SVP_PERSPTRANS_H__

#include <math.h>
#include "xmedia_sys.h"

#ifdef __cplusplus
extern "C" {
#endif

#define XMEDIA_ABS(a)       (((a) > 0) ? (a) : -(a))
#define XMEDIA_MAX(a,b)    (((a) > (b)) ? (a) : (b))
#define XMEDIA_MIN(a,b)    (((a) < (b)) ? (a) : (b))
#define XMEDIA_CLIP(a, maxv, minv)  (((a)>(maxv)) ? (maxv) : (((a) < (minv)) ? (minv) : (a))) // 使第一个参数在后两个参数之间
#define XMEDIA_CLIP3(a,b,x)         (XMEDIA_MAX(XMEDIA_MIN((x),(b)), (a)))
#define MAX_NUM_UNKNOWN    8

typedef struct {
    xmedia_s32  x;
    xmedia_s32  y;
    xmedia_s32  xprime;
    xmedia_s32  yprime;
} svp_point_pair;

typedef struct {
    svp_point_pair point_pair_arry[68];
} svp_point_pairpack;


typedef enum  {
    SVP_PERSP_TRANS_ALG_MODE_NR_SIM = 0x0,
    SVP_PERSP_TRANS_ALG_MODE_SIM = 0x1,
    SVP_PERSP_TRANS_ALG_MODE_AFFINE = 0x2,

    SVP_PERSP_TRANS_ALG_MODE_BUTT
} svp_persp_trans_alg_mode;

typedef enum {
	SVP_PERSP_TRANS_OUT_FMT_U8C1         = 0,
	SVP_PERSP_TRANS_OUT_FMT_YUV420SP     = 1,
	SVP_PERSP_TRANS_OUT_FMT_U8C3_PACKAGE = 2,

	SVP_PERSP_TRANS_OUT_FMT_BUTT
} svp_persp_trans_out_fmt;

typedef enum
{
    SVP_IN_FMT_TYPE_SINGLE       = 0,
    SVP_IN_FMT_TYPE_YUV420SP     = 1,
    SVP_IN_FMT_TYPE_YUV422SP     = 2,
    SVP_IN_FMT_TYPE_U8C3_PACKAGE = 3,
    SVP_IN_FMT_TYPE_U8C3_PLANAR  = 4,
    SVP_IN_FMT_TYPE_YUV444PLANAR = 5,
	SVP_IN_FMT_TYPE_YUV420SP_UV  = 6,
	SVP_IN_FMT_TYPE_YUV422SP_UV  = 7,

    SVP_IN_FMT_TYPE_BUTT
} svp_persp_trans_in_fmt;

xmedia_void xmedia_svp_persptrans(svp_point_pairpack *pointpairpack, xmedia_u8 *pu8imgsrcy, xmedia_u16 u16imgsrcystride,
                         xmedia_u8 *pu8imgsrcuv, xmedia_u16 u16imgsrcuvstride, xmedia_u16 u16imgsrcwidth,
                         xmedia_u16 u16imgsrcheight, xmedia_u8 *pu8imgdsty, xmedia_u16 u16imgdstystride,
                         xmedia_u8 *pu8imgdstuv, xmedia_u16 u16imgdstuvstride, xmedia_u16 u16imgdstwidth,
                         xmedia_u16 u16imgdstheight, xmedia_u16 u16roix0, xmedia_u16 u16roiy0, xmedia_u8 u8transmode,
                         xmedia_u8 u8lmktransnum, xmedia_u8 u8imginfmt, xmedia_u8 u8imgoutfmt);

#ifdef __cplusplus
}
#endif

#endif
