/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __XMEDIA_AUDIO_VQE_BCD_H__
#define __XMEDIA_AUDIO_VQE_BCD_H__

#include "xmedia_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef xmedia_s32 (*fn_bcd_callback)(xmedia_void* private_data); /* Baby Crying Detection be noticed Function Handle type */

typedef enum load_type {
    XMEDIA_CL_LOAD_MEM = 0,
    XMEDIA_CL_LOAD_FILE,
} audio_load_type;

typedef struct {
    audio_load_type load_type;    // 模型输入类别
    union {
        struct {
            xmedia_char *pathname;  // bcd模型文件
        } file;
        struct {
            xmedia_u8   *buf;       // bcd模型存放的内存
            xmedia_u32  len;        // bcd模型的长度
        } mem;
    } model_info;
} audio_load_model;

/* Defines the configure parameters of Baby Crying Detection. */
typedef struct {
    xmedia_bool usr_mode;                  /* mode 0: auto mode 1: mannual. */
    xmedia_bool bypass;                    /* value 0: enable function; value 1: bypass function. */
    xmedia_u32 alarm_threshold;            /* alarm threshold. range[0, 100] */
    xmedia_u32 time_limit;                 /* alarm time limit(s), range [0, 120]. */
    xmedia_u32 time_limit_threshold_count; /* alarm time limit threshold count, range [1, 5]. */
    xmedia_u32 interval_time;              /* alarm interval time to unfreeze(s),  [0, 120]*/
    fn_bcd_callback callback;              /* the callback function pointer */
    audio_load_model bcd_model;             /*for load npu xmm,arm not use*/
    xmedia_void* private_data;             /* the callback private data */
} audio_bcd_config;

/* Defines the process data configure of Baby Crying Detection. */
typedef struct {
    xmedia_s16* data;     /* the pointer to data buf. */
    xmedia_s32 data_size; /* the size of data buf. */
} audio_process_data;

/*
 *  函数功能：获取哭声检测版本号
 *  函数参数：
 *     version - 输出参数，版本号
 *  返回值：
 *     对应版本号数值
 */
xmedia_s32 xmedia_audio_bcd_get_version(xmedia_char* version);

/*
 *  函数功能：实例化哭声检测
 *  函数参数：
 *     bcd - 输出参数，哭声检查句柄
 *     sample_rate - 输入参数，采样率，当前只支持16K
 *     bcd_config - 输入参数，哭声检测的配置属性
 *  返回值：
 *     XMEDIA_FAILURE - 初始化失败
 *     XMEDIA_SUCCESS - 初始化成功
 */
xmedia_s32 xmedia_audio_bcd_init(xmedia_void** bcd, xmedia_s32 sample_rate, const audio_bcd_config* bcd_config);

/*
 *  函数功能：哭声检测数据处理
 *  函数参数：
 *     input_data - 输入参数，输入数据信息
 *     output_data - 输出参数，输出数据信息
 *  返回值：
 *     XMEDIA_FAILURE - 数据处理失败
 *     XMEDIA_SUCCESS - 数据处理成功
 */
xmedia_s32 xmedia_audio_bcd_process(xmedia_void* bcd, audio_process_data* input_data, audio_process_data* output_data);

/*
 *  函数功能：去实例化哭声检测
 *  函数参数：
 *     bcd - 输入参数，哭声检测句柄
 *  返回值：
 *     XMEDIA_FAILURE - 去初始化失败
 *     XMEDIA_SUCCESS - 去初始化成功
 */
xmedia_s32 xmedia_audio_bcd_deinit(xmedia_void* bcd);

#ifdef __cplusplus
}
#endif
#endif
