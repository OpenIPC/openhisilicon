/*
 * Copyright (c) XMEDIA. All rights reserved.
 */
#ifndef _AUDIO_CODEC_AAC_H_
#define _AUDIO_CODEC_AAC_H_

#include "xmedia_type.h"
#include "xmedia_audio_codec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/*samples per frame for AACLC and aacPlus */
#define AACLD_SAMPLES_PER_FRAME         512
#define AACLC_SAMPLES_PER_FRAME         1024
#define AACPLUS_SAMPLES_PER_FRAME       2048

/*max length of AAC stream by bytes */
#define MAX_AAC_MAINBUF_SIZE    768*2

typedef enum
{
    AAC_TYPE_AACLC      = 0,
    AAC_TYPE_EAAC       = 1,            /* eAAC  (HEAAC or AAC+  or aacPlusV1) */
    AAC_TYPE_EAACPLUS   = 2,            /* eAAC+ (AAC++ or aacPlusV2) */
    AAC_TYPE_AACLD      = 3,
    AAC_TYPE_AACELD     = 4,
    AAC_TYPE_MAX
} aac_type;

typedef enum
{
    AAC_BPS_8K      = 8000,
    AAC_BPS_16K     = 16000,
    AAC_BPS_22K     = 22000,
    AAC_BPS_24K     = 24000,
    AAC_BPS_32K     = 32000,
    AAC_BPS_48K     = 48000,
    AAC_BPS_64K     = 64000,
    AAC_BPS_96K     = 96000,
    AAC_BPS_128K    = 128000,
    AAC_BPS_256K    = 256000,
    AAC_BPS_320K    = 320000,
    AAC_BPS_MAX
} aac_bps;

typedef enum
{
    AAC_TRANS_TYPE_ADTS = 0,
    AAC_TRANS_TYPE_LOAS= 1,
    AAC_TRANS_TYPE_MAX
} aac_trans_type;

/*
    AAC Commendatory Parameter:
    Sampling Rate(HZ)    LC bit_rate(Kbit/s)    EAAC bit_rate (Kbit/s)    EAAC+ bit_rate (Kbit/s)
    48000                128                    48                        32,24
    44100                128                    48                        32,24
    32000                96                     22                        16
    24000                64
    22050                64
    16000                48
*/
typedef struct
{
    aac_type type;          /* AAC profile type */
    aac_bps bit_rate;      /* AAC bit_rate (LC:16~320, EAAC:24~128, EAAC+:16~64, AACLD:16~320, AACELD:32~320) */
    xmedia_u32 sample_rate;   /* AAC sample rate (LC:8~48, EAAC:16~48, EAAC+:16~48, AACLD:8~48, AACELD:8~48) */
    xmedia_u32 bit_width;     /* AAC bit width (only support 16bit) */
    xmedia_u32 channels;
    aac_trans_type trans_type;
    xmedia_u16 band_width; /* targeted audio band_width in Hz (0 or 1000~enSmpRate/2), the default is 0 */
} aac_encoder_config;

typedef struct
{
    aac_trans_type trans_type;
} aac_decoder_config;

#define aac_enc_get_default_config(config) \
    do { \
        ((aac_encoder_config *)(config))->type = AAC_TYPE_AACLC; \
        ((aac_encoder_config *)(config))->bit_width = 16; \
        ((aac_encoder_config *)(config))->bit_rate = 128000; \
        ((aac_encoder_config *)(config))->sample_rate = 48000; \
        ((aac_encoder_config *)(config))->band_width    = ((aac_encoder_config *)(config))->sample_rate / 2; \
        ((aac_encoder_config *)(config))->channels  = 2; \
        ((aac_encoder_config *)(config))->trans_type  = AAC_TRANS_TYPE_ADTS; \
    } while (0)

#define aac_enc_get_default_param(open_param, private_config) \
    do { \
        xmedia_u32 in_sample_per_frame = 1024; \
        if ((((aac_encoder_config *)private_config)->type == AAC_TYPE_EAAC) || \
            (((aac_encoder_config *)private_config)->type == AAC_TYPE_EAACPLUS)) { \
            in_sample_per_frame <<= 1; \
        } else if ((((aac_encoder_config *)private_config)->type == AAC_TYPE_AACLD) || \
            (((aac_encoder_config *)private_config)->type == AAC_TYPE_AACELD)) { \
            in_sample_per_frame >>= 1; \
        } \
        ((encoder_param *)(open_param))->channels = \
            ((aac_encoder_config *)private_config)->channels; \
        ((encoder_param *)(open_param))->interleaved = XMEDIA_TRUE; \
        ((encoder_param *)(open_param))->bit_per_sample = \
            ((aac_encoder_config *)private_config)->bit_width; \
        ((encoder_param *)(open_param))->sample_rate = \
            ((aac_encoder_config *)private_config)->sample_rate; \
        ((encoder_param *)(open_param))->sample_per_frame = in_sample_per_frame; \
        ((encoder_param *)(open_param))->encoder_config = (xmedia_void *)private_config; \
    } while (0)

#define aac_dec_get_default_config(config) \
    do { \
        ((aac_decoder_config *)(config))->trans_type = AAC_TRANS_TYPE_ADTS; \
    } while (0)

#define aac_dec_get_default_param(open_param, private_config) \
    do { \
        ((decoder_param *)(open_param))->desired_out_channels = 2; \
        ((decoder_param *)(open_param))->desired_out_samplerate = 48000; \
        ((decoder_param *)(open_param))->bit_per_sample = 16; \
        ((decoder_param *)(open_param))->interleaved  = XMEDIA_TRUE; \
        ((decoder_param *)(open_param))->decoder_config = (xmedia_void *)private_config; \
    } while (0)

/*
 *  函数功能：获取aac解码器句柄
 *  函数参数：
 *     无
 *  返回值：
 *     解码器入口句柄
 */
audio_decoder* xmedia_aac_dec_get_handle(xmedia_void);

/*
 *  函数功能：获取aac编码器句柄
 *  函数参数：
 *     无
 *  返回值：
 *     编码器入口句柄
 */
audio_encoder* xmedia_aac_enc_get_handle(xmedia_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
