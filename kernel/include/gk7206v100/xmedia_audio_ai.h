/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __XMEDIA_AUDIO_AI_H__
#define __XMEDIA_AUDIO_AI_H__

#include "xmedia_type.h"
#include "xmedia_audio_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef enum {
    XMEDIA_AI_DEV_ADC0,
    XMEDIA_AI_DEV_I2S0,
    XMEDIA_AI_DEV_PDM0,
    XMEDIA_AI_DEV_PDM_I2S0,
    XMEDIA_AI_DEV_MAX,
} ai_dev;

typedef struct {
   analog_input_mode mode;
} ai_adc_attr;

typedef struct {
   xmedia_bool master;
   audio_i2s_mode i2s_mode;
   audio_i2s_mclk_sel mclk_sel;
   audio_i2s_bclk_sel bclk_sel;
   xmedia_bool pcm_sample_rise_edge;
   audio_i2s_pcm_delay pcm_delay_cycle;
} ai_i2s_attr;

typedef struct {
   xmedia_u32 reserved;
} ai_pdm_attr;

typedef struct {
    audio_i2s_mode i2s_mode;
    audio_channel i2s_channels;
    audio_bit_depth i2s_bit_depth;
} ai_pdm_i2s_attr;

typedef struct {
    audio_dev_work_mode mode;
    audio_sample_rate sample_rate;
    audio_channel channels;
    audio_bit_depth bit_depth;
    xmedia_u32 pcm_frame_num;
    xmedia_u32 pcm_samples_per_frame;
    xmedia_s32 chn_map[AUDIO_CHANNEL_8];
    union {
        ai_adc_attr adc_attr;
        ai_i2s_attr i2s_attr;
        ai_pdm_attr pdm_attr;
        ai_pdm_i2s_attr pdm_i2s_attr;
    } un_attr;
} ai_dev_attr;

typedef enum {
    AI_VQE_ENHANCE_ATTR_VERSION_1 = 0x1,
    AI_VQE_ENHANCE_ATTR_VERSION_2 = 0x2,
    AI_VQE_ENHANCE_ATTR_VERSION_3 = 0x3,
    AI_VQE_DETECT_ATTR_VERSION_1 = 0x10,
    AI_VQE_ASR_ATTR_VERSION_1 = 0x20,
    AI_VQE_ATTR_VERSION_MAX,
} ai_vqe_attr_version;

typedef struct {
    ai_vqe_attr_version version;
    xmedia_s32 work_sample_rate;
    xmedia_s32 in_channels;
    xmedia_void* attr;      /* refer to ai_vqe_attr_v1 or ai_vqe_attr_v2 */
} ai_vqe_attr;

typedef enum {
    AI_EC_SOURCE_AO_DAC0 = 0x0,
    AI_EC_SOURCE_AO_I2S0 = 0x1,
    AI_EC_SOURCE_MIC = 0x10,
    AI_EC_SOURCE_PDM_REF = 0x11,    /* ec source from XMEDIA_AI_DEV_PDM_I2S0 ai i2s ref */
    AI_EC_SOURCE_MAX
} ai_ec_source;

typedef struct {
    audio_sample_rate sample_rate;
    xmedia_bool  interleaved;
} ai_user_chn_attr;

/*for adc,dmic*/
typedef struct {
    xmedia_u32 alc_enable;              /* range[0, 1] 0:disable alc, 1:enable alc */
    xmedia_u32 target_level;            /* range[0, 15], by step of 1,mean full scale - step*1.5db */
    xmedia_u32 noise_gate_enable;       /* range[0, 1] 0:disable noise gate , 1:enable noise gate */
    xmedia_u32 noise_gate_threshold;    /* range[0db, 42db],step 6db, mean full scale - 64db ~ full scale - 26db*/
    xmedia_u32 max_alc_gain;            /* range[0, 31],by step of 1，mean max_alc_gain * 1.5db */
    xmedia_u32 min_alc_gain;            /* range[0, 31],by step of 1，mean min_alc_gain * 1.5db */
    xmedia_u32 hold_time;               /* range[2ms, 65536ms],step x2 */
    xmedia_u32 attack_time;             /* range[32ms, 512ms],step 32ms */
    xmedia_u32 decay_time;              /* range[32ms, 512ms],step 32ms */
} ai_codec_alc_attr;

typedef enum {
    AI_PARAM_ENABLE_CODEC_HPF_FILTER = 0x20000000,  /* not support, refer to xmedia_bool */
    AI_PARAM_SET_CODEC_WNF_MODE,                    /* not support, refer to xmedia_s32, range[0,3] */
    AI_PARAM_GET_CODEC_WNF_MODE,                    /* not support, refer to xmedia_s32 */
    AI_PARAM_SET_CODEC_MUTE,                        /* refer to xmedia_bool */
    AI_PARAM_SET_CODEC_MIC_GAIN,                    /* refer to xmedia_s32, range[-95,18], by step of 1 dB */
    AI_PARAM_SET_CODEC_ADC_GAIN,                    /* refer to xmedia_s32, range[-95,48], by step of 1 dB  */
    AI_PARAM_GET_CODEC_MIC_GAIN,                    /* refer to xmedia_s32 */
    AI_PARAM_GET_CODEC_ADC_GAIN,                    /* refer to xmedia_s32 */
    AI_PARAM_SET_CODEC_ADC_LEFT_BOOST_GAIN,         /* refer to xmedia_s32, range[0,30], by step of 3 dB, not surpport 3 dB */
    AI_PARAM_SET_CODEC_ADC_RIGHT_BOOST_GAIN,        /* refer to xmedia_s32, range[0,30], by step of 3 dB, not surpport 3 dB */
    AI_PARAM_SET_CODEC_ADC_LEFT_DIGAL_GAIN,         /* refer to xmedia_s32, range[-95,18], by step of 1 dB */
    AI_PARAM_SET_CODEC_ADC_RIGHT_DIGAL_GAIN,        /* refer to xmedia_s32, range[-95,18], by step of 1 dB */
    AI_PARAM_SET_CODEC_ALC_ATTR,                    /* not support, refer to ai_codec_alc_attr*/
    AI_PARAM_GET_CODEC_ADC_LEFT_BOOST_GAIN,         /* refer to xmedia_s32 */
    AI_PARAM_GET_CODEC_ADC_RIGHT_BOOST_GAIN,        /* refer to xmedia_s32 */
    AI_PARAM_GET_CODEC_ADC_LEFT_DIGAL_GAIN,         /* refer to xmedia_s32 */
    AI_PARAM_GET_CODEC_ADC_RIGHT_DIGAL_GAIN,        /* refer to xmedia_s32 */
    AI_PARAM_GET_CODEC_MUTE,                        /* refer to xmedia_bool */
    AI_PARAM_GET_CODEC_ALC_ATTR,                    /* not support, refer to ai_codec_alc_attr*/
} ai_dev_param_cmd;

/*
 * 函数功能: 初始化音频输入模块
 * 函数参数：
 *      无
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_READY - AI设备节点打开失败
 */
xmedia_s32 xmedia_ai_init(xmedia_void);

/*
 * 函数功能: 去初始化音频输入模块
 * 函数参数：
 *      无
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_BUSY - 设备未禁用
 */
xmedia_s32 xmedia_ai_exit(xmedia_void);

/*
 * 函数功能: 设置音频快启模式
 * 函数参数：
 *      enable - 输入参数，快启模式使能标志
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 操作不允许，通道已存在
 */
xmedia_s32 xmedia_ai_set_share_mode(xmedia_bool enable);

/*
 * 函数功能: 获取音频输入设备默认设置参数
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      attr - 输出参数，设备属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_NULL_PTR - 传入参数为空
 *      XMEDIA_ERRCODE_NOT_INIT - 设备初始化状态错误
 */
xmedia_s32 xmedia_ai_get_default_attr(ai_dev dev, ai_dev_attr* attr);

/*
 * 函数功能: 设置音频输入设备参数
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      attr - 输入参数，设备属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_NULL_PTR - 传入参数为空
 *      XMEDIA_ERRCODE_NOT_INIT - 设备初始化状态错误
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 设备使能状态错误
 *      XMEDIA_ERRCODE_INVALID_PARAM - 无效的设备参数
 */
xmedia_s32 xmedia_ai_set_dev_attr(ai_dev dev, const ai_dev_attr* attr);

/*
 * 函数功能: 获取音频输入设备用户设置的属性
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      attr - 输出参数，设备属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_NULL_PTR - 传入参数为空
 *      XMEDIA_ERRCODE_NOT_CONFIG - 设备没有配置
 */
xmedia_s32 xmedia_ai_get_dev_attr(ai_dev dev, ai_dev_attr* attr);

/*
 * 函数功能: 使能音频输入设备
 * 函数参数：
 *      dev - 输入参数，设备ID
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 设备初始化状态错误
 *      XMEDIA_ERRCODE_NOT_CONFIG - 设备没有配置
 *      XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 设备分配内存失败
 *      XMEDIA_ERRCODE_INVALID_PARAM - 函数内部参数错误
 */
xmedia_s32 xmedia_ai_enable_dev(ai_dev dev);

/*
 * 函数功能: 非使能音频输入设备
 * 函数参数：
 *      dev - 输入参数，设备ID
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 设备状态错误
 */
xmedia_s32 xmedia_ai_disable_dev(ai_dev dev);

/*
 * 函数功能: 设置音频输入设备各种参数
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      cmd - 输入参数，设置的命令
 *      param - 输入参数，设置命令对应的参数
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道ID
 */
xmedia_s32 xmedia_ai_set_dev_param(ai_dev dev, ai_dev_param_cmd cmd, xmedia_void* param);

/*
 * 函数功能: 设置音频输入设备通道模式
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      mode - 输入参数，通道模式
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_INVALID_PARAM - 无效mode参数
 */
xmedia_s32 xmedia_ai_set_trackmode(ai_dev dev, audio_track_mode mode);

/*
 * 函数功能: 获取音频输入设备通道模式
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      mode - 输出参数，获取的模式
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_NULL_PTR - 参数为空
 */
xmedia_s32 xmedia_ai_get_trackmode(ai_dev dev, audio_track_mode* mode);

/*
 * 函数功能: 创建音频输入设备的通道
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      chn - 输入参数，通道ID
 *      chn_attr - 输入参数，需要设置的通道的属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道ID
 *      XMEDIA_ERRCODE_NULL_PTR - 输入参数chn_attr为空
 *      XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 内存分配错误
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 设备状态错误
 *      XMEDIA_ERRCODE_INVALID_PARAM - 无效的通道用户参数
 *      XMEDIA_ERRCODE_NOT_ENABLE - 设备没有使能
 */
xmedia_s32 xmedia_ai_create_chn(ai_dev dev, ai_chn chn, const ai_user_chn_attr* chn_attr);

/*
 * 函数功能: 销毁音频输入设备的通道
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      chn - 输入参数，通道ID
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道ID
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 设备状态错误
 */
xmedia_s32 xmedia_ai_destroy_chn(ai_dev dev, ai_chn chn);

/*
 * 函数功能: 使能音频输入设备的通道
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      chn - 输入参数，通道ID
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道ID
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 设备状态错误
 */
xmedia_s32 xmedia_ai_enable_chn(ai_dev dev, ai_chn chn);

/*
 * 函数功能: 非使能音频输入设备的通道
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      chn - 输入参数，通道ID
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道ID
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 设备状态错误
 */
xmedia_s32 xmedia_ai_disable_chn(ai_dev dev, ai_chn chn);

/*
 * 函数功能: 设置音频输入设备通道算法的属性
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      chn - 输入参数，通道ID
 *      attr - 输入参数，算法的属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道ID
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 设备状态错误
 *      XMEDIA_ERRCODE_NULL_PTR - 参数为空
 *      XMEDIA_ERRCODE_INVALID_PARAM - 无效的vqe参数
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 版本不支持
 */
xmedia_s32 xmedia_ai_set_vqe_attr(ai_dev dev, ai_chn chn, const ai_vqe_attr* attr);

/*
 * 函数功能: 获取音频输入设备通道算法的属性
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      chn - 输入参数，通道ID
 *      attr - 输出参数，算法的属性
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道ID
 *      XMEDIA_ERRCODE_NULL_PTR - 参数为空
 */
xmedia_s32 xmedia_ai_get_vqe_attr(ai_dev dev, ai_chn chn, ai_vqe_attr* attr);

/*
 * 函数功能: 使能音频输入设备通道算法
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      chn - 输入参数，通道ID
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道ID
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 设备状态错误
 */
xmedia_s32 xmedia_ai_enable_vqe(ai_dev dev, ai_chn chn);

/*
 * 函数功能: 非使能音频输入设备通道算法
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      chn - 输入参数，通道ID
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道ID
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 设备状态错误
 */
xmedia_s32 xmedia_ai_disable_vqe(ai_dev dev, ai_chn chn);

/*
 * 函数功能: 设置音频输入设备通道的音量
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      chn - 输入参数，通道ID
 *      volume - 输入参数，需要设置的音量
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道ID
 *      XMEDIA_ERRCODE_NULL_PTR - 内部状态空
 */
xmedia_s32 xmedia_ai_set_volume(ai_dev dev, ai_chn chn, xmedia_s32 volume);

/*
 * 函数功能: 获取音频输入设备通道的音量
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      chn - 输入参数，通道ID
 *      volume - 输出参数，获取的音量
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道ID
 *      XMEDIA_ERRCODE_NULL_PTR - 参数为空
 */
xmedia_s32 xmedia_ai_get_volume(ai_dev dev, ai_chn chn, xmedia_s32* volume);

/*
 * 函数功能: 获取音频输入设备通道的帧
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      chn - 输入参数，通道ID
 *      frame - 输出参数，获取的输入帧
 *      ext_frame - 输出参数，获取的原始或者参考
 *      time_out - 输入参数，超时时间
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效通道ID
 *      XMEDIA_ERRCODE_NULL_PTR - 参数空指针
 *      XMEDIA_ERRCODE_BUFFER_EMPTY - 数据buffer为空，无数据
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 设备状态错误
 */
xmedia_s32 xmedia_ai_acquire_frame(ai_dev dev, ai_chn chn, audio_frame* frame, audio_ext_frame* ext_frame, xmedia_s32 time_out);

/*
 * 函数功能: 释放音频输入设备通道的帧
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      chn - 输入参数，通道ID
 *      frame - 输入参数，需要释放的输入帧
 *      ext_frame - 输入参数，需要释放的原始或者参考
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道ID
 *      XMEDIA_ERRCODE_NULL_PTR - 参数空指针
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 设备状态错误
 */
xmedia_s32 xmedia_ai_release_frame(ai_dev dev, ai_chn chn, audio_frame* frame, audio_ext_frame* ext_frame);

/*
 * 函数功能: 使能音频输入设备通道的参考帧
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      chn - 输入参数，通道ID
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效设备通道ID
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 设备状态错误
 */
xmedia_s32 xmedia_ai_enable_ec_frame(ai_dev dev, ai_chn chn);

/*
 * 函数功能: 非使能音频输入设备通道的参考帧
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      chn - 输入参数，通道ID
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效设备通道ID
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 设备状态错误
 */
xmedia_s32 xmedia_ai_disable_ec_frame(ai_dev dev, ai_chn chn);

/*
 * 函数功能: 使能音频输入设备通道的原始帧
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      chn - 输入参数，需要设置的通道ID
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效设备通道ID
 */
xmedia_s32 xmedia_ai_enable_origin_frame(ai_dev dev, ai_chn chn);

/*
 * 函数功能: 非使能音频输入设备通道的原始帧
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      chn - 输入参数，需要设置的通道ID
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效设备通道ID
 */
xmedia_s32 xmedia_ai_disable_origin_frame(ai_dev dev, ai_chn chn);

/*
 * 函数功能: 设置音频输入设备通道参考帧的来源
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      chn - 输入参数，通道ID
 *      ref_dev - 输入参数，需要参考的源
 *      ref_chn - 输入参数，需要参考的源的通道
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道ID
 *      XMEDIA_ERRCODE_INVALID_PARAM - 错误的源
 *      XMEDIA_ERRCODE_NOT_PERMITTED - 设备状态错误
 *      XMEDIA_ERRCODE_NOT_SUPPORT - 源不支持
 */
xmedia_s32 xmedia_ai_set_ec_source(ai_dev dev, ai_chn chn, ai_ec_source ref_dev, xmedia_u32 ref_chn);

/*
 * 函数功能: 音频输入设备通道保存数据接口
 * 函数参数：
 *      dev - 输入参数，设备ID
 *      chn - 输入参数，通道ID
 *      dump - 输入参数，dump数据的存储路径以及size大小
 *      enable - 输入参数，dump功能使能状态
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_DEV_ID - 无效的设备ID
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道ID
 */
xmedia_s32 xmedia_ai_dump(ai_dev dev, ai_chn chn, const audio_dump* dump, xmedia_bool enable);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
