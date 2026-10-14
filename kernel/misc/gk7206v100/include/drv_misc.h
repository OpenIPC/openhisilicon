#ifndef __DRV_MISC_H__
#define __DRV_MISC_H__

#include "xmedia_debug.h"
#include "xmedia_type.h"
#include "drv_type_ioctl.h"

//#define SUPPORT_OSD_TIME                1
//#define SUPPORT_SENSOR_PWDN_MODE        1

#ifdef SUPPORT_OSD_TIME
#include "xmedia_region.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    CMD_MISC_AOV_INIT = 0,
    CMD_MISC_AOV_SET_SENSOR_MODE,
#ifdef SUPPORT_OSD_TIME
    CMD_MISC_OSD_TIME_CREATE,
    CMD_MISC_OSD_TIME_UPDATE,
    CMD_MISC_OSD_TIME_UPDATE_TIMEZONE,
    CMD_MISC_OSD_TIME_DESTROY,
#endif

#ifdef SUPPORT_SENSOR_PWDN_MODE
    CMD_MISC_SENSOR_PWDN_STANDBY,
    CMD_MISC_SENSOR_PWDN_RESUME,
#endif

    CMD_MISC_END = 0xFFFF,
} misc_ioctl_cmd;

// ioctl cmd 对应的函数指针
typedef xmedia_s32 (*misc_ctl_ptr_func)(xmedia_ulong arg);
typedef struct misc_ioctl_func_item {
    xmedia_u32       cmd;
    misc_ctl_ptr_func ptr_func;
} misc_ioctl_func_item;

#ifdef SUPPORT_OSD_TIME
typedef struct __misc_osd_time_attach_info_ {
    xmedia_chn_info mpp_chn;
    xmedia_rgn_chn_attr chn_attr;
}misc_osd_time_attach_info_t;

typedef struct _misc_osd_time_ {
    xmedia_s32 rgn_hdl;
    xmedia_rgn_attr rgn_attr;

    xmedia_u16 rgn_attach_num;
    misc_osd_time_attach_info_t attach_obj[4];

    xmedia_u16 font_size_scale;
    xmedia_u16 text_color;
    xmedia_u16 bg_color;
} misc_osd_time_attr_t;
#endif

typedef struct misc_aov_isp_info{
    xmedia_s32 isp_pipe_num;
    xmedia_u32 isp_pipe[VI_MAX_DEV_NUM];
#ifdef SUPPORT_SENSOR_PWDN_MODE
    xmedia_bool support_sensor_pwdn_standby[VI_MAX_DEV_NUM];
#endif
} misc_aov_isp_info;

typedef struct misc_aov_state{
    xmedia_bool en_aov_mode;
    xmedia_s32 frame_num;
} misc_aov_state;

#define XMEDIA_MISC_AOV_INIT                    _IOWR(IOC_TYPE_MISC, CMD_MISC_AOV_INIT, misc_aov_isp_info)
#define XMEDIA_MISC_AOV_SET_SENSOR_MODE         _IOWR(IOC_TYPE_MISC, CMD_MISC_AOV_SET_SENSOR_MODE, misc_aov_state)
#ifdef SUPPORT_OSD_TIME
#define XMEDIA_MISC_OSD_TIME_CREATE_CTRL        _IOW(IOC_TYPE_MISC, CMD_MISC_OSD_TIME_CREATE, misc_osd_time_attr_t)
#define XMEDIA_MISC_OSD_TIME_UPDATE_CTRL        _IOW(IOC_TYPE_MISC, CMD_MISC_OSD_TIME_UPDATE, misc_osd_time_attr_t)
#define XMEDIA_MISC_OSD_TIME_UPDATE_TIMEZONE_CTRL _IOW(IOC_TYPE_MISC, CMD_MISC_OSD_TIME_UPDATE_TIMEZONE, xmedia_s32)
#define XMEDIA_MISC_OSD_TIME_DESTROY_CTRL       _IOW(IOC_TYPE_MISC, CMD_MISC_OSD_TIME_DESTROY, misc_osd_time_attr_t)
#endif
#ifdef SUPPORT_SENSOR_PWDN_MODE
#define XMEDIA_MISC_SENSOR_PWDN_STANDBY         _IOWR(IOC_TYPE_MISC, CMD_MISC_SENSOR_PWDN_STANDBY, xmedia_s32)
#define XMEDIA_MISC_SENSOR_PWDN_RESUME          _IOWR(IOC_TYPE_MISC, CMD_MISC_SENSOR_PWDN_RESUME, xmedia_s32)
#endif

#define MISC_TRACE(level, fmt, ...)                                                                                 \
    do {                                                                                                          \
        MODULE_TRACE(level, MOD_ID_MISC, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif /* __DRV_MISC_H__ */
