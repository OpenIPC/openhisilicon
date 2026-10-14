#ifndef __SNS_COMM_H__
#define __SNS_COMM_H__

#ifdef __linux__
#include <unistd.h>
#endif
#include <stdio.h>
#include "xmedia_isp.h"
#include "xmedia_errcode.h"
#ifdef __linux__
#include "xmedia_debug.h"
#else
#include "compile.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __linux__
    #define SENSOR_PRINT(fmt, ...)                                                                                    \
        XMEDIA_PRINT("[SENSOR][Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

    #define SENSOR_TRACE(level, fmt, ...)                                                                             \
        do {                                                                                                          \
            MODULE_TRACE(level, MOD_ID_ISP, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__);\
        } while (0)

    #define SENSOR_PRIORITY_FUNC
    #define SENSOR_PRIORITY_DATA
    #define SENSOR_PRIORITY_RODATA

    #define sleep_us(x) usleep(x)
#else
    #define SENSOR_PRINT(fmt, ...)
    #define SENSOR_TRACE(level, fmt, ...)

    #define SENSOR_PRIORITY_FUNC   STAGE1_FUNC
    #define SENSOR_PRIORITY_DATA   STAGE1_GLOBAL
    #define SENSOR_PRIORITY_RODATA STAGE1_CONST_GLOBAL

    #define sleep_us(x)                                                                             \
    do {                                                                                            \
        for(xmedia_s32 i=x; i>0; i--){                                                              \
            asm("nop");                                                                             \
            asm("nop");                                                                             \
            asm("nop");                                                                             \
            asm("nop");                                                                             \
            asm("nop");                                                                             \
        }                                                                                           \
    } while (0)
#endif

#define SENSOR_DIV_0_TO_1(a)       (((a) == 0) ? 1 : (a))
#define SENSOR_DIV_0_TO_1_FLOAT(a) ((((a) < 1E-10) && ((a) > -1E-10)) ? 1 : (a))

#define SENSOR_HIGH_8BITS(x) (((x)&0xFF00) >> 8) // BIT[7:0]
#define SENSOR_LOW_8BITS(x)  ((x)&0x00FF)        // BIT[7:0]

#define SENSOR_HIGHER_4BITS(x) (((x)&0xF000) >> 12) // BIT[3:0]
#define SENSOR_MIDDLE_8BITS(x) (((x)&0x0FF0) >> 4)  // BIT[7:0]
#define SENSOR_LOWER_4BITS(x)  (((x)&0x000F) << 4)  // BIT[7:4]

#define SENSOR_ALIGN_UP(x, a)     ((((x) + ((a)-1)) / a) * a)
#define SENSOR_ALIGN_DOWN(x, a)   (((x) / a) * a)

#define SENSOR_MAX(x, y) ((x) > (y) ? (x) : (y))
#define SENSOR_MIN(x, y) ((x) < (y) ? (x) : (y))

#define SENSOR_CLIP3(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

#define SENSOR_DEV_INVALID (-1)
#define SENSOR_I2C_INVALID (-1)

#define SENSOR_AWB_CHN_GIAN_MAX_NUM 3
#define SENSOR_AWB_CHN_GIAN_R       0
#define SENSOR_AWB_CHN_GIAN_G       1
#define SENSOR_AWB_CHN_GIAN_B       2

#define SENSOR_CUR_FRAME     0
#define SENSOR_PRE_FRAME     1
#define SENSOR_FRAME_MAX_NUM 2

typedef struct {
    xmedia_bool             init;
    xmedia_u8               img_mode;
    xmedia_video_wdr_mode   wdr_mode;
    xmedia_sensor_init_mode init_mode;
    xmedia_video_size       size;
    xmedia_float            fps;

    // [0]: sensor reg info of cur-frame; [1]: sensor reg info of pre-frame;
    xmedia_sensor_regs_info regs_info[SENSOR_FRAME_MAX_NUM];

    // [0]: fulllines of cur-frame; [1]: pre fulllines of pre-frame
    xmedia_u32 fl[SENSOR_FRAME_MAX_NUM];

    // fulllines std (vmax) - use for AE resume framerate after slow framerate
    xmedia_u32             fl_std;
    xmedia_isp_ae_wdr_mode ae_wdr_mode;

    //color temperature
    xmedia_u32 ct;

    // 短帧最大曝光时间
    xmedia_u32 max_short_exp;

    // 曝光比索引按升序排列, 0: 次短帧比最短帧; XMEDIA_ISP_WDR_CHN_MAX_NUM - 1: 最长帧比次长帧
    xmedia_u32 ratio[XMEDIA_ISP_WDR_EXP_RATIO_MAX_NUM];

    // 帧的索引按升序排列, 0: 最短帧; XMEDIA_ISP_WDR_CHN_MAX_NUM - 1: 最长帧;
    xmedia_u32 wdr_int_time[XMEDIA_ISP_WDR_CHN_MAX_NUM];

    xmedia_sensor_work_mode  work_mode;
    xmedia_sensor_mipi_lanes lanes;
    xmedia_bool              mirror_en;
    xmedia_bool              flip_en;
    xmedia_u32               i2c_addr;
    xmedia_sensor_commbus    bus_info;
} sensor_context;

#define SENSOR_CHECK_PTR_RETURN(ptr)                                                                                   \
    do {                                                                                                               \
        if (ptr == XMEDIA_NULL) {                                                                                      \
            SENSOR_TRACE(MODULE_DBG_ERR, "%s is Null Pointer!\n", #ptr);                                               \
            return XMEDIA_ERRCODE_NULL_PTR;                                                                            \
        }                                                                                                              \
    } while (0)

#define SENSOR_CHECK_DEV_RETURN(dev)                                                                                   \
    do {                                                                                                               \
        if ((dev) >= XMEDIA_SENSOR_DEV_MAX_NUM) {                                                                      \
            SENSOR_TRACE(MODULE_DBG_ERR, "Invalid dev. The range of dev is [0, %u), but input dev is %u. \n",          \
                         XMEDIA_SENSOR_DEV_MAX_NUM, (dev));                                                            \
            return XMEDIA_ERRCODE_INVALID_DEV_ID;                                                                      \
        }                                                                                                              \
    } while (0)

#define SENSOR_CHECK_PIPE_RETURN(pipe)                                                                                 \
    do {                                                                                                               \
        if ((pipe) >= XMEDIA_ISP_PIPE_MAX_NUM) {                                                                       \
            SENSOR_TRACE(MODULE_DBG_ERR, "Invalid pipe. The range of pipe is [0, %u), but input pipe is %u. \n",       \
                         XMEDIA_ISP_PIPE_MAX_NUM, (pipe));                                                             \
            return XMEDIA_ERRCODE_INVALID_DEV_ID;                                                                      \
        }                                                                                                              \
    } while (0)

#define SENSOR_FREE(ptr)                                                                                               \
    do {                                                                                                               \
        if (ptr != XMEDIA_NULL) {                                                                                      \
            free(ptr);                                                                                                 \
            ptr = XMEDIA_NULL;                                                                                         \
        }                                                                                                              \
    } while (0)

#define SENSOR_CHECK_RET_RETURN(ret)                                                                                   \
    do {                                                                                                               \
        if (ret != XMEDIA_SUCCESS) {                                                                                   \
            SENSOR_TRACE(MODULE_DBG_ERR, "error code = %d\n", ret);                                                    \
            return ret;                                                                                                \
        }                                                                                                              \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif
