/******************************************************************************

  Copyright (C), 2016-2018, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
 File Name     : tde_type.h
Version       : Initial Draft
Author        :
Created       : 2016/5/21
Last Modified :
Description   : TDE public type
Function List :
History       :
1.Date        : 2016/5/21
Author      :
Modification: Created file

 ******************************************************************************/
#ifndef __DRV_TDE_TYPE_H__
#define __DRV_TDE_TYPE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#include "hi_tde_errcode.h"
/****************************************************************************/
/*                             TDE2 types define                             */
/****************************************************************************/

/*************************** Structure Definition ****************************/
/** \addtogroup      TDE */

/**Definition of the TDE handle */
/**TDE callback functions */
typedef hi_void (*tde_func_cb)(hi_void *pParaml, hi_void *pParamr);

typedef hi_void (*tde_trig_sel)(hi_u32);

/**RGB and packet YUV formats */
typedef enum {
    HI_TDE_COLOR_FMT_RGB444 = 0,       /**<For the RGB444 format, red occupies four bits, green occupies four bits, and blue occupies four bits. Other formats may be deduced by analogy. */
    HI_TDE_COLOR_FMT_BGR444,           /**<BGR444 format */
    HI_TDE_COLOR_FMT_RGB555,           /**<RGB555 format */
    HI_TDE_COLOR_FMT_BGR555,           /**<BGR555 format */
    HI_TDE_COLOR_FMT_RGB565,           /**<RGB565 format */
    HI_TDE_COLOR_FMT_BGR565,           /**<BGR565 format */
    HI_TDE_COLOR_FMT_RGB888,           /**<RGB888 format */
    HI_TDE_COLOR_FMT_BGR888,           /**<BGR888 format */
    HI_TDE_COLOR_FMT_ARGB4444,         /**<ARGB4444 format */
    HI_TDE_COLOR_FMT_ABGR4444,         /**<ABGR4444 format */
    HI_TDE_COLOR_FMT_RGBA4444,         /**<RGBA4444 format */
    HI_TDE_COLOR_FMT_BGRA4444,         /**<BGRA4444 format */
    HI_TDE_COLOR_FMT_ARGB1555,         /**<ARGB1555 format */
    HI_TDE_COLOR_FMT_ABGR1555,         /**<ABGR1555 format */
    HI_TDE_COLOR_FMT_RGBA1555,         /**<RGBA1555 format */
    HI_TDE_COLOR_FMT_BGRA1555,         /**<BGRA1555 format */
    HI_TDE_COLOR_FMT_ARGB8565,         /**<ARGB8565 format */
    HI_TDE_COLOR_FMT_ABGR8565,         /**<ABGR8565 format */
    HI_TDE_COLOR_FMT_RGBA8565,         /**<RGBA8565 format */
    HI_TDE_COLOR_FMT_BGRA8565,         /**<BGRA8565 format */
    HI_TDE_COLOR_FMT_ARGB8888,         /**<ARGB8888 format */
    HI_TDE_COLOR_FMT_ABGR8888,         /**<ABGR8888 format */
    HI_TDE_COLOR_FMT_RGBA8888,         /**<RGBA8888 format */
    HI_TDE_COLOR_FMT_BGRA8888,         /**<BGRA8888 format */
    HI_TDE_COLOR_FMT_RABG8888,         /**<RABG8888 format */
    HI_TDE_COLOR_FMT_CLUT1,            /**<1-bit palette format without alpha component. Each pixel occupies one bit. */
    HI_TDE_COLOR_FMT_CLUT2,            /**<2-bit palette format without alpha component. Each pixel occupies two bits. */
    HI_TDE_COLOR_FMT_CLUT4,            /**<4-bit palette format without alpha component. Each pixel occupies four bits. */
    HI_TDE_COLOR_FMT_CLUT8,            /**<8-bit palette format without alpha component. Each pixel occupies eight bits. */
    HI_TDE_COLOR_FMT_ACLUT44,          /**<1-bit palette format with alpha component. Each pixel occupies one bit. */
    HI_TDE_COLOR_FMT_ACLUT88,          /**<1-bit palette format with alpha component. Each pixel occupies one bit. */
    HI_TDE_COLOR_FMT_A1,               /**<Alpha format. Each pixel occupies one bit. */
    HI_TDE_COLOR_FMT_A8,               /**<Alpha format. Each pixel occupies eight bits. */
    HI_TDE_COLOR_FMT_YCbCr888,         /**<YUV packet format without alpha component */
    HI_TDE_COLOR_FMT_AYCbCr8888,       /**<YUV packet format with alpha component */
    HI_TDE_COLOR_FMT_YCbCr422,         /**<YUV packet422 format */
    HI_TDE_COLOR_FMT_PKGVYUY,          /**<YUV packet422 format, VYUY format */
    HI_TDE_COLOR_FMT_byte,             /**<Only for fast copy */
    HI_TDE_COLOR_FMT_halfword,         /**<Only for fast copy */
    HI_TDE_COLOR_FMT_JPG_YCbCr400MBP,  /**<Semi-planar YUV400 format, for JPG decoding */
    HI_TDE_COLOR_FMT_JPG_YCbCr422MBHP, /**<Semi-planar YUV422 format, horizontal sampling, for JPG decoding */
    HI_TDE_COLOR_FMT_JPG_YCbCr422MBVP, /**<Semi-planar YUV422 format, vertical sampling, for JPG decoding */
    HI_TDE_COLOR_FMT_MP1_YCbCr420MBP,  /**<Semi-planar YUV420 format */
    HI_TDE_COLOR_FMT_MP2_YCbCr420MBP,  /**<Semi-planar YUV420 format */
    HI_TDE_COLOR_FMT_MP2_YCbCr420MBI,  /**<Semi-planar YUV400 format */
    HI_TDE_COLOR_FMT_JPG_YCbCr420MBP,  /**<Semi-planar YUV400 format, for JPG decoding */
    HI_TDE_COLOR_FMT_JPG_YCbCr444MBP,  /**<Semi-planar YUV444 format */
    HI_TDE_COLOR_FMT_MAX              /**<End of enumeration */
} hi_tde_color_fmt;

/**Definition of the semi-planar YUV format */
typedef enum {
    HI_TDE_MB_COLOR_FMT_JPG_YCbCr400MBP = 0, /**<Semi-planar YUV400 format, for JPG decoding */
    HI_TDE_MB_COLOR_FMT_JPG_YCbCr422MBHP,    /**<Semi-planar YUV422 format, horizontal sampling, for JPG decoding */
    HI_TDE_MB_COLOR_FMT_JPG_YCbCr422MBVP,    /**<Semi-planar YUV422 format, vertical sampling, for JPG decoding */
    HI_TDE_MB_COLOR_FMT_MP1_YCbCr420MBP,     /**<Semi-planar YUV420 format */
    HI_TDE_MB_COLOR_FMT_MP2_YCbCr420MBP,     /**<Semi-planar YUV420 format */
    HI_TDE_MB_COLOR_FMT_MP2_YCbCr420MBI,     /**<Semi-planar YUV400 format */
    HI_TDE_MB_COLOR_FMT_JPG_YCbCr420MBP,     /**<Semi-planar YUV400 format, for JPG pictures */
    HI_TDE_MB_COLOR_FMT_JPG_YCbCr444MBP,     /**<Semi-planar YUV444 format, for JPG pictures */
    HI_TDE_MB_COLOR_FMT_MAX
} hi_tde_mb_color_fmt;

/**Structure of the bitmap information set by customers */
typedef struct {
    hi_u64 phy_addr; /**<Header address of a bitmap or the Y component */

    hi_tde_color_fmt color_fmt; /**<Color format */

    hi_u32 height; /**<Bitmap height */

    hi_u32 width; /**<Bitmap width */

    hi_u32 stride; /**<Stride of a bitmap or the Y component */

    hi_bool is_ycbcr_clut; /**<Whether the CLUT is in the YCbCr space. */

    hi_bool alpha_max_is_255; /**<The maximum alpha value of a bitmap is 255 or 128. */

    hi_bool support_alpha_ext_1555; /* <Whether to enable the alpha extension of an ARGB1555 bitmap. */
    hi_u8 alpha0;        /**<Values of alpha0 and alpha1, used as the ARGB1555 format */
    hi_u8 alpha1;        /**<Values of alpha0 and alpha1, used as the ARGB1555 format */
    hi_u64 cbcr_phy_addr;    /**<Address of the CbCr component, pilot */
    hi_u32 cbcr_stride;  /**<Stride of the CbCr component, pilot */
    hi_u64 clut_phy_addr;    /**<Address of the color look-up table (CLUT), for color extension or color correction */
} hi_tde_surface;

/**Definition of the semi-planar YUV data */
typedef struct {
    hi_tde_mb_color_fmt mb_color_fmt; /**<YUV format */
    hi_u64 y_addr;             /**<Physical address of the Y component */
    hi_u32 y_width;            /**<Width of the Y component */
    hi_u32 y_height;           /**<Height of the Y component */
    hi_u32 y_stride;           /**< Stride of the Y component, indicating bytes in each row */
    hi_u64 cbcr_phy_addr;          /**<Width of the UV component */
    hi_u32 cbcr_stride;        /**<Stride of the UV component, indicating the bytes in each row */
} hi_tde_mb_surface;

/**Definition of the TDE rectangle */
typedef struct {
    hi_s32 pos_x;   /**<Horizontal coordinate */
    hi_s32 pos_y;   /**<Vertical coordinate */
    hi_u32 width;  /**<Width */
    hi_u32 height; /**<Height */
} hi_tde_rect;

/**Logical operation type */
typedef enum {
    HI_TDE_ALPHA_BLENDING_NONE = 0x0,     /**<No alpha and raster of operation (ROP) blending */
    HI_TDE_ALPHA_BLENDING_BLEND = 0x1,    /**<Alpha blending */
    HI_TDE_ALPHA_BLENDING_ROP = 0x2,      /**<ROP blending */
    HI_TDE_ALPHA_BLENDING_COLORIZE = 0x4, /**<Colorize operation */
    HI_TDE_ALPHA_BLENDING_MAX = 0x8      /**<End of enumeration */
} hi_tde_alpha_blending;

/**Definition of ROP codes */
typedef enum {
    HI_TDE_ROP_BLACK = 0,   /**<Blackness */
    HI_TDE_ROP_NOTMERGEPEN, /**<~(S2 | S1) */
    HI_TDE_ROP_MASKNOTPEN,  /**<~S2&S1 */
    HI_TDE_ROP_NOTCOPYPEN,  /**< ~S2 */
    HI_TDE_ROP_MASKPENNOT,  /**< S2&~S1 */
    HI_TDE_ROP_NOT,         /**< ~S1 */
    HI_TDE_ROP_XORPEN,      /**< S2^S1 */
    HI_TDE_ROP_NOTMASKPEN,  /**< ~(S2 & S1) */
    HI_TDE_ROP_MASKPEN,     /**< S2&S1 */
    HI_TDE_ROP_NOTXORPEN,   /**< ~(S2^S1) */
    HI_TDE_ROP_NOP,         /**< S1 */
    HI_TDE_ROP_MERGENOTPEN, /**< ~S2|S1 */
    HI_TDE_ROP_COPYPEN,     /**< S2 */
    HI_TDE_ROP_MERGEPENNOT, /**< S2|~S1 */
    HI_TDE_ROP_MERGEPEN,    /**< S2|S1 */
    HI_TDE_ROP_WHITE,       /**< Whiteness */
    HI_TDE_ROP_MAX
} hi_tde_rop_mode;

/**Definition of the blit mirror */
typedef enum {
    HI_TDE_MIRROR_NONE = 0,   /**<No mirror */
    HI_TDE_MIRROR_HORIZONTAL, /**<Horizontal mirror */
    HI_TDE_MIRROR_VERTICAL,   /**<Vertical mirror */
    HI_TDE_MIRROR_BOTH,       /**<Horizontal and vertical mirror */
    HI_TDE_MIRROR_MAX
} hi_tde_mirror_mode;

/**Clip operation type */
typedef enum {
    HI_TDE_CLIP_MODE_NONE = 0, /**<No clip */
    HI_TDE_CLIP_MODE_INSIDE,   /**<Clip the data within the rectangle to output and discard others */
    HI_TDE_CLIP_MODE_OUTSIDE,  /**<Clip the data outside the rectangle to output and discard others */
    HI_TDE_CLIP_MODE_MAX
} hi_tde_clip_mode;

/**Scaling mode for the macroblock */
typedef enum {
    HI_TDE_MB_RESIZE_NONE = 0,       /**<No scaling */
    HI_TDE_MB_RESIZE_QUALITY_LOW,    /**<Low-quality scaling */
    HI_TDE_MB_RESIZE_QUALITY_MIDDLE, /**<Medium-quality scaling */
    HI_TDE_MB_RESIZE_QUALITY_HIGH,   /**<High-quality scaling */
    HI_TDE_MB_RESIZE_MAX
} hi_tde_mb_resize;

/**Definition of fill colors */
typedef struct {
    hi_tde_color_fmt color_fmt; /**<TDE pixel format */
    hi_u32 color_value;         /**<Fill colors that vary according to pixel formats */
} hi_tde_fill_color;

/**Definition of colorkey modes */
typedef enum {
    HI_TDE_COLOR_KEY_MODE_NONE = 0,   /**<No colorkey */
    HI_TDE_COLOR_KEY_MODE_FOREGROUND, /**<When performing the colorkey operation on the foreground bitmap, you need to perform this operation before the CLUT for color extension and perform this operation after the CLUT for color correction. */
    HI_TDE_COLOR_KEY_MODE_BACKGROUND, /**<Perform the colorkey operation on the background bitmap */
    HI_TDE_COLOR_KEY_MODE_MAX
} hi_tde_color_key_mode;

/**Definition of colorkey range */
typedef struct {
    hi_u8 component_min;   /**<Minimum value of a component */
    hi_u8 component_max;   /**<Maximum value of a component */
    hi_u8 is_component_out;    /**<The colorkey of a component is within or beyond the range. */
    hi_u8 is_component_ignore; /**<Whether to ignore a component. */
    hi_u8 component_mask;  /**<Component mask */
    hi_u8 component_reserved;
    hi_u8 component_reserved1;
    hi_u8 component_reserved2;
} hi_tde_color_key_comp;

/**Definition of colorkey values */
typedef union {
    struct {
        hi_tde_color_key_comp alpha; /**<Alpha component */
        hi_tde_color_key_comp red;   /**<Red component */
        hi_tde_color_key_comp green; /**<Green component */
        hi_tde_color_key_comp blue;  /**<Blue component */
    } argb_color_key;
    struct {
        hi_tde_color_key_comp alpha; /**<Alpha component */
        hi_tde_color_key_comp y;     /**<Y component */
        hi_tde_color_key_comp cb;    /**<Cb component */
        hi_tde_color_key_comp cr;    /**<Cr component */
    } ycbcr_color_key;
    struct {
        hi_tde_color_key_comp alpha; /**<Alpha component */
        hi_tde_color_key_comp clut;  /**<Palette component */
    } clut_color_key;
} hi_tde_color_key;

/* Definition of alpha output sources */
typedef enum {
    HI_TDE_OUT_ALPHA_FROM_NORM = 0,    /**<Output from the result of alpha blending or anti-flicker */
    HI_TDE_OUT_ALPHA_FROM_BACKGROUND,  /**<Output from the background bitmap */
    HI_TDE_OUT_ALPHA_FROM_FOREGROUND,  /**<Output from the foreground bitmap */
    HI_TDE_OUT_ALPHA_FROM_GLOBALALPHA, /**<Output from the global alpha */
    HI_TDE_OUT_ALPHA_FROM_MAX
} hi_tde_out_alpha_from;

/**Definition of filtering */
typedef enum {
    HI_TDE_FILTER_MODE_COLOR = 0, /**<Filter the color */
    HI_TDE_FILTER_MODE_ALPHA,     /**<Filter the alpha channel */
    HI_TDE_FILTER_MODE_BOTH,      /**<Filter the color and alpha channel */
    HI_TDE_FILTER_MODE_NONE,      /**<No filter */
    HI_TDE_FILTER_MODE_MAX
} hi_tde_filter_mode;

/**Configuration of the anti-flicker channel */
typedef enum {
    HI_TDE_DEFLICKER_LEVEL_MODE_NONE = 0, /* <No anti-flicker */
    HI_TDE_DEFLICKER_LEVEL_MODE_RGB,      /**<Perform anti-flicker on the RGB component */
    HI_TDE_DEFLICKER_LEVEL_MODE_BOTH,     /**<Perform anti-flicker on the alpha component */
    HI_TDE_DEFLICKER_LEVEL_MODE_MAX
} hi_tde_deflicker_mode;

/* blend mode */
typedef enum {
    HI_TDE_BLEND_ZERO = 0x0,
    HI_TDE_BLEND_ONE,
    HI_TDE_BLEND_SRC2COLOR,
    HI_TDE_BLEND_INVSRC2COLOR,
    HI_TDE_BLEND_SRC2ALPHA,
    HI_TDE_BLEND_INVSRC2ALPHA,
    HI_TDE_BLEND_SRC1COLOR,
    HI_TDE_BLEND_INVSRC1COLOR,
    HI_TDE_BLEND_SRC1ALPHA,
    HI_TDE_BLEND_INVSRC1ALPHA,
    HI_TDE_BLEND_SRC2ALPHASAT,
    HI_TDE_BLEND_MAX
} hi_tde_blend_mode;

/**Alpha blending command. You can set parameters or select Porter or Duff. */
/* pixel = (source * fs + destination * fd),
   sa = source alpha,
   da = destination alpha */
typedef enum {
    HI_TDE_BLEND_CMD_NONE = 0x0, /**< fs: sa      fd: 1.0-sa */
    HI_TDE_BLEND_CMD_CLEAR,      /**< fs: 0.0     fd: 0.0 */
    HI_TDE_BLEND_CMD_SRC,        /**< fs: 1.0     fd: 0.0 */
    HI_TDE_BLEND_CMD_SRCOVER,    /**< fs: 1.0     fd: 1.0-sa */
    HI_TDE_BLEND_CMD_DSTOVER,    /**< fs: 1.0-da  fd: 1.0 */
    HI_TDE_BLEND_CMD_SRCIN,      /**< fs: da      fd: 0.0 */
    HI_TDE_BLEND_CMD_DSTIN,      /**< fs: 0.0     fd: sa */
    HI_TDE_BLEND_CMD_SRCOUT,     /**< fs: 1.0-da  fd: 0.0 */
    HI_TDE_BLEND_CMD_DSTOUT,     /**< fs: 0.0     fd: 1.0-sa */
    HI_TDE_BLEND_CMD_SRCATOP,    /**< fs: da      fd: 1.0-sa */
    HI_TDE_BLEND_CMD_DSTATOP,    /**< fs: 1.0-da  fd: sa */
    HI_TDE_BLEND_CMD_ADD,        /**< fs: 1.0     fd: 1.0 */
    HI_TDE_BLEND_CMD_XOR,        /**< fs: 1.0-da  fd: 1.0-sa */
    HI_TDE_BLEND_CMD_DST,        /**< fs: 0.0     fd: 1.0 */
    HI_TDE_BLEND_CMD_CONFIG,     /**<You can set the parameteres. */
    HI_TDE_BLEND_CMD_MAX
} hi_tde_blend_cmd;

/**Options for the alpha blending operation */
typedef struct {
    hi_bool global_alpha_en;       /**<Global alpha enable */
    hi_bool pixel_alpha_en;        /**<Pixel alpha enable */
    hi_bool src1_alpha_premulti;       /**<Src1 alpha premultiply enable */
    hi_bool src2_alpha_premulti;       /**<Src2 alpha premultiply enable */
    hi_tde_blend_cmd blend_cmd;        /**<Alpha blending command */
    hi_tde_blend_mode
    src1_blend_mode; /**<Src1 blending mode select. It is valid when blend_cmd is set to HI_TDE_BLEND_CMD_CONFIG. */
    hi_tde_blend_mode
    src2_blend_mode; /**<Src2 blending mode select. It is valid when blend_cmd is set to HI_TDE_BLEND_CMD_CONFIG. */
} hi_tde_blend_opt;

/**CSC parameter option */
typedef struct {
    hi_bool src_csc_user_en;  /**User-defined ICSC parameter enable */
    hi_bool src_csc_param_reload_en; /**User-defined ICSC parameter reload enable */
    hi_bool dst_csc_user_en;  /**User-defined OCSC parameter enable */
    hi_bool dst_csc_param_reload_en; /**User-defined OCSC parameter reload enable */
    hi_u64 src_csc_param_addr;     /**ICSC parameter address. The address must be 128-bit aligned. */
    hi_u64 dst_csc_param_addr;     /**OCSC parameter address. The address must be 128-bit aligned. */
} hi_tde_csc_opt;

/**Definition of blit operation options */
typedef struct {
    hi_tde_alpha_blending alpha_blending_cmd; /**<Logical operation type */

    hi_tde_rop_mode rop_color; /**<ROP type of the color space */

    hi_tde_rop_mode rop_alpha; /**<ROP type of the alpha component */

    hi_tde_color_key_mode color_key_mode; /**<Colorkey mode */

    hi_tde_color_key color_key_value; /**<Colorkey value */

    hi_tde_clip_mode clip_mode; /**<Perform the clip operation within or beyond the area */

    hi_tde_rect clip_rect; /**<Definition of the clipping area */

    hi_tde_deflicker_mode deflicker_mode; /**<Anti-flicker mode */

    hi_bool resize; /**<Whether to scale */

    hi_tde_filter_mode filter_mode;        /**<Filtering mode during scaling*/

    hi_tde_mirror_mode mirror; /**<Mirror type */

    hi_bool clut_reload; /**<Whether to reload the CLUT */

    hi_u8 global_alpha; /**<Global alpha value */

    hi_tde_out_alpha_from out_alpha_from;    /**<Source of the output alpha*/

    hi_u32 color_resize; /**<Colorize value */

    hi_tde_blend_opt blend_opt;

    hi_tde_csc_opt csc_opt;
    hi_bool is_compress;
    hi_bool is_decompress;
} hi_tde_opt;

/**Definition of macroblock operation options */
typedef struct {
    hi_tde_clip_mode clip_mode; /**<Clip mode */

    hi_tde_rect clip_rect; /**<Definition of the clipping area */

    hi_bool is_deflicker; /**<Whether to perform anti-flicker */

    hi_tde_mb_resize resize_en; /**<Scaling information */

    hi_bool is_set_out_alpha; /**<If the alpha value is not set, the maximum alpha value is output by default. */

    hi_u8 out_alpha; /**<Global alpha for operation */
} hi_tde_mb_opt;

/**Definition of the pattern filling operation */
typedef struct {
    hi_tde_alpha_blending alpha_blending_cmd; /**<Logical operation type */

    hi_tde_rop_mode rop_color; /**<ROP type of the color space */

    hi_tde_rop_mode rop_alpha; /**<ROP type of the alpha component */

    hi_tde_color_key_mode color_key_mode; /**<Colorkey mode */

    hi_tde_color_key color_key_value; /**<Colorkey value */

    hi_tde_clip_mode clip_mode; /**<Clip mode */

    hi_tde_rect clip_rect; /**<Clipping area */

    hi_bool clut_reload; /**<Whether to reload the CLUT */

    hi_u8 global_alpha; /**<Global alpha */

    hi_tde_out_alpha_from out_alpha_from;    /**<Source of the output alpha*/

    hi_u32 color_resize; /**<Colorize value */

    hi_tde_blend_opt blend_opt; /**<Options of the blending operation */

    hi_tde_csc_opt csc_opt; /**<CSC parameter option */

} hi_tde_pattern_fill_opt;

/**Definition of rotation directions */
typedef enum {
    HI_TDE_ROTATE_CLOCKWISE_90 = 0, /**<Rotate 90 degree clockwise */
    HI_TDE_ROTATE_CLOCKWISE_180,    /**<Rotate 180 degree clockwise */
    HI_TDE_ROTATE_CLOCKWISE_270,    /**<Rotate 270 degree clockwise */
    HI_TDE_ROTATE_MAX
} hi_tde_rotate_angle;

/**Definition of anti-flicker levels */
typedef enum {
    HI_TDE_DEFLICKER_LEVEL_AUTO = 0, /**<Adaptation. The anti-flicker coefficient is selected by the TDE. */
    HI_TDE_DEFLICKER_LEVEL_LOW,      /**<Low-level anti-flicker */
    HI_TDE_DEFLICKER_LEVEL_MIDDLE,   /**<Medium-level anti-flicker */
    HI_TDE_DEFLICKER_LEVEL_HIGH,     /**High-level anti-flicker */
    HI_TDE_DEFLICKER_LEVEL_MAX
} hi_tde_deflicker_level;

/* composed surface info */
typedef struct {
    hi_tde_surface src_surface;
    hi_tde_rect src_rect;
    hi_tde_rect dst_rect;
    hi_tde_opt option;
} hi_tde_composor;

/* composed surface list */
typedef struct {
    hi_u32 surface_num;
    hi_tde_surface *dst_surface;
    hi_tde_composor *composor;
} hi_tde_surface_list;
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifndef __TDE_TYPE_H__ */


