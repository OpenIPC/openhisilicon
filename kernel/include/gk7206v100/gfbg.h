#ifndef __GFBG_IOCTL_H__
#define __GFBG_IOCTL_H__


#include <linux/fb.h>
#include "xmedia_type.h"
#include "xmedia_errcode.h"

#ifdef __cplusplus
extern "C"{
#endif

typedef enum {
    GFBG_DRV_FMT_RGB565 = 0,
    GFBG_DRV_FMT_RGB888,
    GFBG_DRV_FMT_KRGB444,
    GFBG_DRV_FMT_KRGB555,

    GFBG_DRV_FMT_KRGB888,
    GFBG_DRV_FMT_ARGB4444,
    GFBG_DRV_FMT_ARGB1555,
    GFBG_DRV_FMT_ARGB8888,

    GFBG_DRV_FMT_ARGB8565,
    GFBG_DRV_FMT_RGBA4444,
    GFBG_DRV_FMT_RGBA5551,
    GFBG_DRV_FMT_RGBA5658,

    GFBG_DRV_FMT_RGBA8888,
    GFBG_DRV_FMT_BGR565,
    GFBG_DRV_FMT_BGR888,
    GFBG_DRV_FMT_ABGR4444,

    GFBG_DRV_FMT_ABGR1555,
    GFBG_DRV_FMT_ABGR8888,
    GFBG_DRV_FMT_ABGR8565,
    GFBG_DRV_FMT_KBGR444,

    GFBG_DRV_FMT_KBGR555,
    GFBG_DRV_FMT_KBGR888,
    GFBG_DRV_FMT_1BPP,
    GFBG_DRV_FMT_2BPP,

    GFBG_DRV_FMT_4BPP,
    GFBG_DRV_FMT_8BPP,
    GFBG_DRV_FMT_ACLUT44,
    GFBG_DRV_FMT_ACLUT88,

    GFBG_DRV_FMT_PUYVY,
    GFBG_DRV_FMT_PYUYV,
    GFBG_DRV_FMT_PYVYU,
    GFBG_DRV_FMT_YUV888,

    GFBG_DRV_FMT_AYUV8888,
    GFBG_DRV_FMT_YUVA8888,

    GFBG_DRV_FMT_ARGB2101010,
    GFBG_DRV_FMT_ARGB10101010,
    GFBG_DRV_FMT_FP16,

    GFBG_DRV_FMT_MAX
} gfbg_drv_fmt;

typedef enum
{
    GFBG_LAYER_ID_G0 = 0x0,
    GFBG_LAYER_ID_G1,
    GFBG_LAYER_ID_MAX
} gfbg_layer_id;

typedef struct {
    xmedia_bool key_enable;
    xmedia_u32 key;
} gfbg_drv_colorkey;

typedef struct {
    xmedia_s32 x;
    xmedia_s32 y;
} gfbg_drv_point;

typedef struct {
    xmedia_bool pixel_alpha;
    xmedia_bool global_alpha;
    xmedia_u8 alpha0;
    xmedia_u8 alpha1;
    xmedia_u8 global_alpha_value;
    xmedia_u8 reserved;
} gfbg_drv_alpha;

typedef struct {
    xmedia_u64 tv_sec;
    xmedia_u64 tv_msec;
    xmedia_u64 tv_usec;
    xmedia_u64 tv_nsec;
} gfbg_drv_time_val;

typedef struct {
    xmedia_u32 refresh_rate;
    gfbg_drv_time_val time_val;
} gfbg_drv_vblank_info;

typedef struct {
    xmedia_bool key_rgb;
    xmedia_bool key_alpha;
    xmedia_bool global_alpha;
    xmedia_bool cmap;
    xmedia_bool has_cmap_reg;
    xmedia_bool col_fmt[GFBG_DRV_FMT_MAX];
    xmedia_bool pre_mul;
    xmedia_u32  max_width;
    xmedia_u32  max_height;
    xmedia_u32  min_width;
    xmedia_u32  min_height;
} gfbg_drv_capability;

#define IOC_TYPE_GFBG       'F'

#define GFBG_GET_COLORKEY                _IOR(IOC_TYPE_GFBG, 90, gfbg_drv_colorkey)
#define GFBG_PUT_COLORKEY                _IOW(IOC_TYPE_GFBG, 91, gfbg_drv_colorkey)
#define GFBG_GET_ALPHA                   _IOR(IOC_TYPE_GFBG, 92, gfbg_drv_alpha)
#define GFBG_PUT_ALPHA                   _IOW(IOC_TYPE_GFBG, 93, gfbg_drv_alpha)
#define GFBG_GET_PRE_MULTIPLY            _IOR(IOC_TYPE_GFBG, 94, xmedia_bool)
#define GFBG_PUT_PRE_MULTIPLY            _IOW(IOC_TYPE_GFBG, 95, xmedia_bool)
#define GFBG_GET_SCREEN_POS              _IOR(IOC_TYPE_GFBG, 96, gfbg_drv_point)
#define GFBG_PUT_SCREEN_POS              _IOW(IOC_TYPE_GFBG, 97, gfbg_drv_point)
#define GFBG_GET_VBLANK                  _IOR(IOC_TYPE_GFBG, 98 ,gfbg_drv_vblank_info)
#define GFBG_PUT_SHOW                    _IOW(IOC_TYPE_GFBG, 99, xmedia_bool)
#define GFBG_GET_SHOW                    _IOR(IOC_TYPE_GFBG, 100, xmedia_bool)
#define GFBG_GET_CAPABILITY              _IOR(IOC_TYPE_GFBG, 101, gfbg_drv_capability)

#ifdef __cplusplus
}
#endif

#endif

