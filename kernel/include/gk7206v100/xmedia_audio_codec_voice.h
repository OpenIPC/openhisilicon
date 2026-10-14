/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef _AUDIO_CODEC_VOICE_H_
#define _AUDIO_CODEC_VOICE_H_

#include "xmedia_type.h"
#include "xmedia_audio_codec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define MAX_AUDIO_POINT_BYTES  4
#define MAX_VOICE_POINT_NUM    480
#define MIN_AUDIO_POINT_NUM    80

typedef enum {
    VOICE_FORMAT_G711_A = 0x01,
    VOICE_FORMAT_G711_U = 0x02,
    VOICE_FORMAT_ADPCM_DVI4 = 0x03,
    VOICE_FORMAT_G726_16KBPS = 0x04,
    VOICE_FORMAT_G726_24KBPS = 0x05,
    VOICE_FORMAT_G726_32KBPS = 0x06,
    VOICE_FORMAT_G726_40KBPS = 0x07,
    VOICE_FORMAT_ADPCM_IMA = 0x23,
    VOICE_FORMAT_MEDIA_G726_16KBPS = 0x24,
    VOICE_FORMAT_MEDIA_G726_24KBPS = 0x25,
    VOICE_FORMAT_MEDIA_G726_32KBPS = 0x26,
    VOICE_FORMAT_MEDIA_G726_40KBPS = 0x27,
    VOICE_FORMAT_G711_ORG_A = 0x41,
    VOICE_FORMAT_G711_ORG_U = 0x42,
    VOICE_FORMAT_ADPCM_ORG_DVI4 = 0x43,
    VOICE_FORMAT_G726_ORG_16KBPS = 0x44,
} voice_format;

typedef enum {
    VOICE_PER_SAMPLE_80  = 80,
    VOICE_PER_SAMPLE_81  = 81,
    VOICE_PER_SAMPLE_160 = 160,
    VOICE_PER_SAMPLE_161 = 161,
    VOICE_PER_SAMPLE_240 = 240,
    VOICE_PER_SAMPLE_241 = 241,
    VOICE_PER_SAMPLE_320 = 320,
    VOICE_PER_SAMPLE_321 = 321,
    VOICE_PER_SAMPLE_480 = 480,
    VOICE_PER_SAMPLE_481 = 481,
} voice_per_sample;

typedef struct {
    voice_format format;
    voice_per_sample sample_per_frame;
} voice_encoder_config;

typedef struct {
    voice_format format;
    xmedia_u32 sample_rate;
} voice_decoder_config;

#define voice_enc_get_default_config(config) do { \
    ((voice_encoder_config *)(config))->format = VOICE_FORMAT_G711_A; \
    ((voice_encoder_config *)(config))->sample_per_frame = VOICE_PER_SAMPLE_320; \
} while (0)

#define voice_enc_get_default_param(open_param, private_config) do { \
    ((encoder_param *)(open_param))->channels = 1; \
    ((encoder_param *)(open_param))->interleaved = XMEDIA_TRUE; \
    ((encoder_param *)(open_param))->bit_per_sample = 16; \
    ((encoder_param *)(open_param))->sample_rate = 8000; \
    ((encoder_param *)(open_param))->sample_per_frame = ((voice_encoder_config *)(private_config))->sample_per_frame; \
    ((encoder_param *)(open_param))->encoder_config = (xmedia_void *)(private_config); \
} while (0)

#define voice_dec_get_default_config(config) do { \
    ((voice_decoder_config *)(config))->format = VOICE_FORMAT_G711_A; \
    ((voice_decoder_config *)(config))->sample_rate = 8000; \
} while (0)

#define voice_dec_get_default_param(open_param, private_config) do { \
    ((decoder_param *)(open_param))->desired_out_channels = 1; \
    ((decoder_param *)(open_param))->desired_out_samplerate = 8000; \
    ((decoder_param *)(open_param))->bit_per_sample = 16; \
    ((decoder_param *)(open_param))->interleaved  = XMEDIA_TRUE; \
    ((decoder_param *)(open_param))->decoder_config = (xmedia_void *)(private_config); \
} while (0)

/*
 *  函数功能：获取g711编码器句柄
 *  函数参数：
 *     无
 *  返回值：
 *     编码器入口句柄
 */
audio_encoder* xmedia_g711_enc_get_handle(xmedia_void);

/*
 *  函数功能：获取g711解码器句柄
 *  函数参数：
 *     无
 *  返回值：
 *     解码器入口句柄
 */
audio_decoder* xmedia_g711_dec_get_handle(xmedia_void);

/*
 *  函数功能：获取g726编码器句柄
 *  函数参数：
 *     无
 *  返回值：
 *     编码器入口句柄
 */
audio_encoder* xmedia_g726_enc_get_handle(xmedia_void);

/*
 *  函数功能：获取g726解码器句柄
 *  函数参数：
 *     无
 *  返回值：
 *     解码器入口句柄
 */
audio_decoder* xmedia_g726_dec_get_handle(xmedia_void);

/*
 *  函数功能：获取adpcm编码器句柄
 *  函数参数：
 *     无
 *  返回值：
 *     编码器入口句柄
 */
audio_encoder* xmedia_adpcm_enc_get_handle(xmedia_void);

/*
 *  函数功能：获取adpcm解码器句柄
 *  函数参数：
 *     无
 *  返回值：
 *     解码器入口句柄
 */
audio_decoder* xmedia_adpcm_dec_get_handle(xmedia_void);

/*
 *  函数功能：获取lpcm编码器句柄
 *  函数参数：
 *     无
 *  返回值：
 *     编码器入口句柄
 */
audio_encoder* xmedia_lpcm_enc_get_handle(xmedia_void);

/*
 *  函数功能：获取lpcm解码器句柄
 *  函数参数：
 *     无
 *  返回值：
 *     解码器入口句柄
 */
audio_decoder* xmedia_lpcm_dec_get_handle(xmedia_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
