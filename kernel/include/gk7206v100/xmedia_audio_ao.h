/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __XMEDIA_API_AO_H__
#define __XMEDIA_API_AO_H__

#include "common.h"
#include "xmedia_type.h"
#include "xmedia_audio_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define XMEDIA_AO_CHN_MAX 3
#define XMEDIA_AO_SYSCHN_CHNID (XMEDIA_AO_CHN_MAX - 1)

typedef enum {
    XMEDIA_AO_DEV_DAC0,
    XMEDIA_AO_DEV_I2S0,
    XMEDIA_AO_DEV_MAX,
} ao_dev;

typedef struct {
   xmedia_u32 reserved;
} ao_dac_attr;

typedef struct {
   xmedia_bool master;
   audio_i2s_mode i2s_mode;
   audio_i2s_mclk_sel mclk_sel;
   audio_i2s_bclk_sel bclk_sel;
   xmedia_bool pcm_sample_rise_edge;
   audio_i2s_pcm_delay pcm_delay_cycle;
} ao_i2s_attr;

typedef struct {
    audio_dev_work_mode mode;
    audio_sample_rate sample_rate;
    audio_channel channels;
    audio_bit_depth bit_depth;
    xmedia_u32 pcm_frame_max_num;
    xmedia_u32 pcm_samples_per_frame;
    union {
        ao_dac_attr dac_attr;
        ao_i2s_attr i2s_attr;
    } un_attr;
} ao_dev_attr;

typedef enum {
    AO_VQE_ENHANCE_ATTR_VERSION_1 = 0x1,
    AO_VQE_ENHANCE_ATTR_VERSION_2 = 0x2,
    AO_VQE_ATTR_VERSION_MAX,
} ao_vqe_attr_version;

typedef struct {
    ao_vqe_attr_version version;
    xmedia_void* attr; /* refer to ao_vqe_attr_v1 or ao_vqe_attr_v2 */
} ao_vqe_attr;

typedef struct {
    xmedia_s32 threshold;   /* support -31 ~ 0db */
    xmedia_s32 step;        /* support 0 ~ 5 */
} dac_drc_attr;

typedef struct ao_chn_state_s {
    xmedia_u32 total_num;
    xmedia_u32 free_num;
    xmedia_u32 busy_num;
} ao_chn_state_s;

typedef enum {
    AO_PARAM_CODEC_SET_MUTE = 0x3000000,    /* refer to xmedia_bool */
    AO_PARAM_CODEC_GET_MUTE,                /* refer to xmedia_bool */
    AO_PARAM_CODEC_SET_DAC_GAIN,            /* refer to xmedia_s32, range[-95,18], by step of 1 dB */
    AO_PARAM_CODEC_GET_DAC_GAIN,            /* refer to xmedia_s32 */
    AO_PARAM_CODEC_SET_DAC_DRC_ATTR,        /* not support, refer to dac_drc_attr */
    AO_PARAM_CODEC_GET_DAC_DRC_ATTR,        /* not support, refer to dac_drc_attr */
    AO_PARAM_CODEC_CLEAR_DAC_DRC_ATTR,      /* not support, refer to dac_drc_attr */
    AO_PARAM_CODEC_SET_DAC_LEFT_GAIN,       /* refer to xmedia_s32, range[-95,18], by step of 1 dB */
    AO_PARAM_CODEC_GET_DAC_LEFT_GAIN,       /* refer to xmedia_s32 */
    AO_PARAM_CODEC_SET_DAC_RIGHT_GAIN,      /* refer to xmedia_s32, range[-95,18], by step of 1 dB */
    AO_PARAM_CODEC_GET_DAC_RIGHT_GAIN,      /* refer to xmedia_s32 */
    AO_PARAM_CODEC_SET_DAC_LEFT_MUTE,       /* refer to xmedia_bool */
    AO_PARAM_CODEC_GET_DAC_LEFT_MUTE,       /* refer to xmedia_bool */
    AO_PARAM_CODEC_SET_DAC_RIGHT_MUTE,      /* refer to xmedia_bool */
    AO_PARAM_CODEC_GET_DAC_RIGHT_MUTE,      /* refer to xmedia_bool */

    AO_PARAM_SET_OUTPORT_VOLUME,            /* not support, refer to xmedia_s32, range[-81,6], by step of 1 dB */
    AO_PARAM_GET_OUTPORT_VOLUME,            /* not support, refer to xmedia_s32 */
    AO_PARAM_SET_OUTPORT_MUTE,              /* not support, refer to xmedia_bool */
    AO_PARAM_GET_OUTPORT_MUTE,              /* not support, refer to xmedia_bool */

    AO_PARAM_CODEC_SET_DAC_ANALOG_GAIN,     /* refer to xmedia_s32, range[-21,0], by step of 3 dB */
    AO_PARAM_CODEC_GET_DAC_ANALOG_GAIN,     /* refer to xmedia_s32 */
} ao_dev_param_cmd;

/*
 * 函数功能: 初始化音频输出模块
 * 函数参数：
 *      无
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_READY - AO节点打开失败
 */
xmedia_s32 xmedia_ao_init(xmedia_void);

/*
 * 函数功能: 去初始化音频输出模块
 * 函数参数：
 *      无
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
        XMEDIA_ERRCODE_BUSY - 设备未禁用
 */
xmedia_s32 xmedia_ao_exit(xmedia_void);

/*
 * 函数功能: 获取音频输出设备默认设置参数
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      attr - 输出参数，设备属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_NULL_PTR - 传入参数为空
 *      XMEDIA_ERRCODE_NOT_INIT - 设备初始化状态错误
 */
xmedia_s32 xmedia_ao_get_default_attr(ao_dev dev, ao_dev_attr *attr);

/*
 * 函数功能: 设置音频输出设备参数
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      attr - 输入参数，设备属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_NULL_PTR - 传入参数为空
 *      XMEDIA_ERRCODE_NOT_INIT - 设备初始化状态错误
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 设备状态错误
 *      XMEDIA_ERRCODE_INVALID_PARAM - 参数校验失败
 */
xmedia_s32 xmedia_ao_set_dev_attr(ao_dev dev, const ao_dev_attr *attr);

/*
 * 函数功能: 获取音频输出设备用户设置的属性
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      attr - 输出参数，设备属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_NULL_PTR - 传入参数为空
 *      XMEDIA_ERRCODE_NOT_CONFIG - 用户没有配置参数
 */
xmedia_s32 xmedia_ao_get_dev_attr(ao_dev dev, ao_dev_attr *attr);

/*
 * 函数功能: 使能音频输出设备
 * 函数参数：
 *      dev - 输入参数，设备ID
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_NOT_INIT - 设备初始化状态错误
 *      XMEDIA_ERRCODE_NOT_CONFIG - 设备没有配置参数
 *      XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 设备分配内存失败
 *      XMEDIA_ERRCODE_INVALID_PARAM - 函数内部位宽错误
 */
xmedia_s32 xmedia_ao_enable_dev(ao_dev dev);

/*
 * 函数功能: 禁用音频输出设备
 * 函数参数：
 *      dev - 输入参数，设备ID
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_NOT_INIT - 设备初始化状态错误
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 操作不允许，通道未销毁
 */
xmedia_s32 xmedia_ao_disable_dev(ao_dev dev);

/*
 * 函数功能: 设置音频输出设备各种参数
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      cmd - 输入参数，设置的命令
 *      param - 输入参数，需要参考的参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道ID
 *      XMEDIA_ERRCODE_NULL_PTR - param为空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM - 参数校验失败
 */
xmedia_s32 xmedia_ao_set_dev_param(ao_dev dev, ao_dev_param_cmd cmd, xmedia_void* param);

/*
 * 函数功能: 设置音频输出设备声道模式
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      mode - 输入参数，通道模式
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_INVALID_PARAM - 无效mode参数
 */
xmedia_s32 xmedia_ao_set_trackmode(ao_dev dev, audio_track_mode mode);

/*
 * 函数功能: 获取音频输出设备声道模式
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      mode - 输出参数，获取的模式
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_NULL_PTR - mode空指针
 */
xmedia_s32 xmedia_ao_get_trackmode(ao_dev dev, audio_track_mode* mode);

/*
 * 函数功能: 创建音频输出设备通道
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      chn - 输入参数，通道ID
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道ID
 *      XMEDIA_ERRCODE_NULL_PTR - 内部参数为空
 *      XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 内存分配失败
 *      XMEDIA_ERRCODE_NOT_ENABLE - 设备使能状态错误
 */
xmedia_s32 xmedia_ao_create_chn(ao_dev dev, ao_chn chn);

/*
 * 函数功能: 销毁音频输出设备通道
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      chn - 输入参数，通道ID
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道ID
 *      XMEDIA_ERRCODE_NOT_DISABLE - 通道使能状态错误
 */
xmedia_s32 xmedia_ao_destroy_chn(ao_dev dev, ao_chn chn);

/*
 * 函数功能: 使能音频输出设备通道
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      chn - 输入参数，通道ID
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道ID
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 设备状态错误
 */
xmedia_s32 xmedia_ao_enable_chn(ao_dev dev, ao_chn chn);

/*
 * 函数功能: 禁用音频输出设备通道
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      chn - 输入参数，通道ID
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道ID
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 设备状态错误
 */
xmedia_s32 xmedia_ao_disable_chn(ao_dev dev, ao_chn chn);

/*
 * 函数功能: 擦除音频输出通道buf缓存
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      chn - 输入参数，通道ID
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道ID
 *      XMEDIA_ERRCODE_NOT_EXIST - 通道不存在
 */
xmedia_s32 xmedia_ao_flush_chn(ao_dev dev, ao_chn chn);

/*
 * 函数功能: 暂停音频输出通道播放
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      chn - 输入参数，通道ID
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道ID
 *      XMEDIA_ERRCODE_NOT_EXIST - 通道不存在
 */
xmedia_s32 xmedia_ao_pause_chn(ao_dev dev, ao_chn chn);

/*
 * 函数功能: 恢复音频输出通道播放
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      chn - 输入参数，通道ID
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道ID
 *      XMEDIA_ERRCODE_NOT_EXIST - 通道不存在
 */
xmedia_s32 xmedia_ao_resume_chn(ao_dev dev, ao_chn chn);

/*
 * 函数功能: 查询通道播放状态
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      chn - 输入参数，通道ID
 *      state - 输出参数，通道播放状态
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道ID
 *      XMEDIA_ERRCODE_NULL_PTR - 参数为空
 *      XMEDIA_ERRCODE_NOT_EXIST - 通道不存在
 *      XMEDIA_ERRCODE_NOT_ENABLE - 通道未使能
 */
xmedia_s32 xmedia_ao_query_chn_state(ao_dev dev, ao_chn chn, ao_chn_state_s* state);

/*
 * 函数功能: 往音频输出设备通道送入音频帧
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      chn - 输入参数，通道ID
 *      frame - 输入参数，音频帧信息
 *      time_out - 输入参数，超时时间
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道ID
 *      XMEDIA_ERRCODE_BUFFER_FULL - 音频输出设备通道buf已满
 */
xmedia_s32 xmedia_ao_send_frame(ao_dev dev, ao_chn chn, const audio_frame* frame, xmedia_s32 time_out);

/*
 * 函数功能: 设置音频输出设备通道算法的属性
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      chn - 输入参数，通道ID
 *      attr - 输入参数，算法的属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效设备通道ID
 *      XMEDIA_ERRCODE_NULL_PTR - 参数为空
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 设备状态错误
 *      XMEDIA_ERRCODE_INVALID_PARAM - 参数校验失败
 */
xmedia_s32 xmedia_ao_set_vqe_attr(ao_dev dev, ao_chn chn, const ao_vqe_attr* attr);

/*
 * 函数功能: 获取音频输出设备通道算法的属性
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      chn - 输入参数，需要的设置算法的通道ID
 *      attr - 输出参数，算法的属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效设备通道ID
 *      XMEDIA_ERRCODE_NULL_PTR - 参数为空
 *      XMEDIA_ERRCODE_NOT_CONFIG - 用户没有配置参数
 *      XMEDIA_ERRCODE_INVALID_PARAM - 参数校验失败
 */
xmedia_s32 xmedia_ao_get_vqe_attr(ao_dev dev, ao_chn chn, ao_vqe_attr* attr);

/*
 * 函数功能: 使能音频输出通道语音算法功能
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      chn - 输入参数，需要的使能算法的通道ID
 *      enable - 输入参数，使能状态
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道ID
 *      XMEDIA_ERRCODE_NOT_CONFIG - 没有设置算法参数
 */
xmedia_s32 xmedia_ao_enable_vqe(ao_dev dev, ao_chn chn);

/*
 * 函数功能: 禁用音频输出通道语音算法功能
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      chn - 输入参数，需要的使能算法的通道ID
 *      enable - 输出参数，使能状态
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道ID
 *      XMEDIA_ERRCODE_NOT_CONFIG - 没有设置算法参数
 */
xmedia_s32 xmedia_ao_disable_vqe(ao_dev dev, ao_chn chn);

/*
 * 函数功能: 设置音频输出设备通道的音量
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      chn - 输入参数，需要设置的通道ID
 *      volume - 输入参数，需要设置的音量
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道ID
 *      XMEDIA_ERRCODE_NOT_ENABLE - 设备通道或者算法未使能
 */
xmedia_s32 xmedia_ao_set_volume(ao_dev dev, ao_chn chn, xmedia_s32 volume);

/*
 * 函数功能: 获取音频输出设备通道的音量
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      chn - 输入参数，需要设置的通道ID
 *      volume - 输出参数，获取的音量
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道ID
 *      XMEDIA_ERRCODE_NULL_PTR - volume为空指针
 *      XMEDIA_ERRCODE_NOT_ENABLE - 设备通道或者算法未使能
 */
xmedia_s32 xmedia_ao_get_volume(ao_dev dev, ao_chn chn, xmedia_s32* volume);

/*
 * 函数功能: 设置音频输出设备通道绑定状态
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      dst_chn - 输入参数，ao的通道ID
 *      module - 输入参数，绑定到ao的模块ID
 *      src_chn - 输入参数，绑定到ao的模块通道ID
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道ID
 */
xmedia_s32 xmedia_ao_bind(ao_dev dev, ao_chn chn, xmedia_mod_id src_module, xmedia_u32 src_chn);

/*
 * 函数功能: 设置音频输出设备通道解绑
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      dst_chn - 输入参数，ao的通道ID
 *      module - 输入参数，绑定到ao的模块ID
 *      src_chn - 输入参数，绑定到ao的模块通道ID
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道ID
 */
xmedia_s32 xmedia_ao_unbind(ao_dev dev, ao_chn chn, xmedia_mod_id src_module, xmedia_u32 src_chn);

/*
 * 函数功能: 设置音频输出设备通道dump信息以及使能状态
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      chn - 输入参数，通道ID
 *      dump - 输入参数，dump数据的存储路径以及size大小
 *      enable - 输入参数，dump功能使能状态
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道ID
 *      XMEDIA_ERRCODE_NULL_PTR - dump为空指针
 */
xmedia_s32 xmedia_ao_dump(ao_dev dev, ao_chn chn, const audio_dump* dump, xmedia_bool enable);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

