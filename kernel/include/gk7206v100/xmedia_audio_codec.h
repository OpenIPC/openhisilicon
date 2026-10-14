/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef  _XMEDIA_AUDIO_CODEC_H_
#define  _XMEDIA_AUDIO_CODEC_H_

#include "xmedia_type.h"
#include "xmedia_audio_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define CODEC_VERSION_MAX_LEN 64

typedef enum {
    CODEC_MASK_G711 = 0x01,
    CODEC_MASK_G726 = 0x02,
    CODEC_MASK_ADPCM = 0x04,
    CODEC_MASK_AAC = 0x08,
    CODEC_MASK_HEAAC = 0x10,
    CODEC_MASK_MP3 = 0x20,
    CODEC_MASK_LPCM = 0x40,
    CODEC_MASK_MAX,
} codec_mask_type;

typedef enum {
    CODEC_TYPE_G711 = 0x01,
    CODEC_TYPE_G726 = 0x02,
    CODEC_TYPE_ADPCM = 0x03,
    CODEC_TYPE_AAC = 0x04,
    CODEC_TYPE_MP3 = 0x05,
    CODEC_TYPE_LPCM = 0x06,
    CODEC_TYPE_MAX,
} audio_codec_type;

typedef struct {
    xmedia_s8 version[CODEC_VERSION_MAX_LEN];
} codec_version;

typedef struct {
    xmedia_u32 channels;
    xmedia_bool interleaved;
    xmedia_u32 bit_per_sample;
    xmedia_u32 sample_rate;
    xmedia_u32 sample_per_frame;
    xmedia_void* encoder_config;
} encoder_param;

typedef struct {
    xmedia_s64 pts;
    xmedia_u8* data;
    xmedia_u32 size;
} encoder_in_packet;

typedef struct {
    xmedia_u8* out_buf;
    xmedia_u32 out_buf_size;
    xmedia_u32 out_frame_len;
    xmedia_u32 bit_rate;
}encoder_out_packet;

typedef struct {
    audio_codec_type type;
    xmedia_char* name;    /* encoder type,be used to print proc information */
    xmedia_s32 (*get_version)(codec_version* ver);
    xmedia_s32 (*init)(encoder_param* param, xmedia_void** h_encoder);
    xmedia_s32 (*deinit)(xmedia_void* h_encoder);
    xmedia_s32 (*get_max_bytes_per_frame)(xmedia_void* h_encoder, xmedia_u32* out_bytes);
    xmedia_s32 (*set_config)(xmedia_void* h_encoder, xmedia_void* config);
    xmedia_s32 (*encode_frame)(xmedia_void* h_encoder, encoder_in_packet* frame, encoder_out_packet* stream);
} audio_encoder;

typedef struct {
    xmedia_u32 desired_out_channels;
    xmedia_u32 desired_out_samplerate;
    xmedia_u32 bit_per_sample;
    xmedia_bool interleaved;
    xmedia_void *decoder_config;
} decoder_param;

typedef struct {
    xmedia_u8* data;
    xmedia_s32 size;
    xmedia_bool eos;
    xmedia_s64 pts;
    xmedia_u32 reserved;
} decoder_in_packet;

typedef struct {
    xmedia_u8* out_buf;
    xmedia_u32 out_buf_size;
    xmedia_u32 out_samples;
    xmedia_bool interleaved;
    xmedia_u32 bit_per_sample;
    xmedia_u32 channels;
    xmedia_u32 sample_rate;
    xmedia_u32 bit_rate;
    xmedia_u32 decoder_bytes_left;
    xmedia_u32 frame_index;
} decoder_out_packet;

typedef struct {
    audio_codec_type type;
    xmedia_char* name;
    xmedia_s32 (*get_version)(codec_version* ver);
    xmedia_s32 (*init)(decoder_param* param, xmedia_void** h_decoder);
    xmedia_s32 (*deinit)(xmedia_void* h_decoder);
    xmedia_s32 (*get_max_bytes_per_frame)(xmedia_void* h_decoder, xmedia_u32* out_bytes);
    xmedia_s32 (*set_config)(xmedia_void* h_decoder, xmedia_void* config);
    xmedia_s32 (*decode_frame)(xmedia_void* h_decoder, decoder_in_packet* stream, decoder_out_packet* frame);
    xmedia_s32 (*reset)(xmedia_void* h_decoder);
} audio_decoder;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif/* End of #ifndef __XM_AUDIO_CODEC_H__ */

