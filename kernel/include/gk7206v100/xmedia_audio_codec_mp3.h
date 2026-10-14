/*
 * Copyright (c) XMEDIA. All rights reserved.
 */
#ifndef _AUDIO_CODEC_MP3_H_
#define _AUDIO_CODEC_MP3_H_

#include "xmedia_type.h"
#include "xmedia_audio_codec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/*samples per frame for MP3LC and MP3Plus */
#define MP3_SAMPLES_PER_FRAME   1152

/*max length of MP3 stream by bytes */
#define MAX_MP3_MAINBUF_SIZE    1152 * 2

typedef enum
{
    MP3_BPS_8K  = 8000,
    MP3_BPS_16K = 16000,
    MP3_BPS_24K = 24000,
    MP3_BPS_32K = 32000,
    MP3_BPS_40K = 40000,
    MP3_BPS_48K = 48000,
    MP3_BPS_56K = 56000,
    MP3_BPS_64K = 64000,
    MP3_BPS_80K = 80000,
    MP3_BPS_96K = 96000,
    MP3_BPS_112K = 112000,
    MP3_BPS_128K = 128000,
    MP3_BPS_144K = 144000,
    MP3_BPS_160K = 160000,
    MP3_BPS_192K = 192000,
    MP3_BPS_224K = 224000,
    MP3_BPS_256K = 256000,
    MP3_BPS_320K = 320000,
    MP3_BPS_MAX
} mp3_bps;

typedef enum
{
  MP3_VBR_OFF=0,
  MP3_VBR_MT,               /* obsolete, same as vbr_mtrh */
  MP3_VBR_RH,
  MP3_VBR_ABR,
  MP3_VBR_MTRH,
  MP3_VBR_MAX_INDICATOR,    /* Don't use this! It's used for sanity checks.       */
  MP3_VBR_DEFAULT=MP3_VBR_MTRH    /* change this to change the default VBR mode of LAME */
} mp3_vbr_mode;

typedef struct
{
    /*vbr_off for constant bitrate
      vbr_abr for variable bitrate abr mode
      vbr_default for variable bitrate vbr mode
    */
    mp3_vbr_mode mode;
    /* quality setting 0..9., default is 7
       0 = slowest algorithms, but potentially highest quality
       9 = faster algorithms, very poor quality
    */
    xmedia_u32 quality;
    /*
      MPEG-1   layer III sample frequencies (kHz):  32  48  44.1
      bitrates (kbps): 32 40 48 56 64 80 96 112 128 160 192 224 256 320
      MPEG-2   layer III sample frequencies (kHz):  16  24  22.05
      bitrates (kbps):  8 16 24 32 40 48 56 64 80 96 112 128 144 160
      MPEG-2.5 layer III sample frequencies (kHz):   8  12  11.025
      bitrates (kbps):  8 16 24 32 40 48 56 64
      The bitrate to be used.  Default is 128kbps in MPEG1 (64 for mono),
      64kbps in MPEG2 (32 for mono) and 32kbps in MPEG2.5 (16 for mono).
   */
    mp3_bps bit_rate;
    xmedia_s32 bit_per_sample;
    xmedia_u32 channels;
    xmedia_u32 sample_rate;
} mp3_encoder_config;

typedef struct
{
    xmedia_void* reversed;
} mp3_decoder_config;

#define mp3_enc_get_default_config(config) \
    do { \
        ((mp3_encoder_config *)(config))->quality = 7; \
        ((mp3_encoder_config *)(config))->bit_rate = MP3_BPS_128K; \
        ((mp3_encoder_config *)(config))->sample_rate = 48000; \
        ((mp3_encoder_config *)(config))->channels  = 2; \
        ((mp3_encoder_config *)(config))->bit_per_sample  = 16; \
        ((mp3_encoder_config *)(config))->mode = MP3_VBR_OFF; \
    } while (0)

#define mp3_enc_get_default_param(open_param, private_config) \
    do { \
        xmedia_u32 in_sample_per_frame = 1152; \
        ((encoder_param *)(open_param))->channels = \
            ((mp3_encoder_config *)private_config)->channels; \
        ((encoder_param *)(open_param))->interleaved = XMEDIA_TRUE; \
        ((encoder_param *)(open_param))->bit_per_sample = \
            ((mp3_encoder_config *)private_config)->bit_per_sample; \
        ((encoder_param *)(open_param))->sample_rate = \
            ((mp3_encoder_config *)private_config)->sample_rate; \
        ((encoder_param *)(open_param))->sample_per_frame = in_sample_per_frame; \
        ((encoder_param *)(open_param))->encoder_config = (xmedia_void *)private_config; \
    } while (0)

#define mp3_dec_get_default_config(config) \
        do { \
        } while (0)

#define mp3_dec_get_default_param(open_param, private_config) \
        do { \
            ((decoder_param *)(open_param))->desired_out_channels = 2; \
            ((decoder_param *)(open_param))->desired_out_samplerate = 48000; \
            ((decoder_param *)(open_param))->bit_per_sample = 16; \
            ((decoder_param *)(open_param))->interleaved  = XMEDIA_TRUE; \
            ((decoder_param *)(open_param))->decoder_config = (xmedia_void *)private_config; \
        } while (0)

/*
 *  函数功能：获取aac编码器句柄
 *  函数参数：
 *     无
 *  返回值：
 *     编码器入口句柄
 */
audio_encoder* xmedia_mp3_enc_get_handle(xmedia_void);

/*
 *  函数功能：获取mp3解码器句柄
 *  函数参数：
 *     无
 *  返回值：
 *     解码器入口句柄
 */
audio_decoder* xmedia_mp3_dec_get_handle(xmedia_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif

