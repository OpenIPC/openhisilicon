#ifndef _XMEDIA_API_AUDIO_H_
#define _XMEDIA_API_AUDIO_H_

#include "xmedia_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define MAX_AUDIO_FILE_PATH_LEN  256
#define MAX_AUDIO_FILE_PATH_NAME 256
#define MIN_AUDIO_FILE_SIZE 1   /* 1K */

typedef xmedia_u32 adec_chn;
typedef xmedia_u32 aenc_chn;
typedef xmedia_u32 ao_chn;
typedef xmedia_u32 ai_chn;

typedef enum {
    AUDIO_SAMPLE_RATE_UNKNOWN = 0,
    AUDIO_SAMPLE_RATE_8000 = 8000,
    AUDIO_SAMPLE_RATE_12000 = 12000,
    AUDIO_SAMPLE_RATE_11025 = 11025,
    AUDIO_SAMPLE_RATE_16000 = 16000,
    AUDIO_SAMPLE_RATE_22050 = 22050,
    AUDIO_SAMPLE_RATE_24000 = 24000,
    AUDIO_SAMPLE_RATE_32000 = 32000,
    AUDIO_SAMPLE_RATE_44100 = 44100,
    AUDIO_SAMPLE_RATE_48000 = 48000,
    AUDIO_SAMPLE_RATE_64000 = 64000,
    AUDIO_SAMPLE_RATE_96000 = 96000,
    AUDIO_SAMPLE_RATE_MAX,
} audio_sample_rate;

typedef enum {
    AUDIO_BIT_DEPTH_UNKNOWN = 0,
    AUDIO_BIT_DEPTH_8 = 8,
    AUDIO_BIT_DEPTH_16 = 16,
    AUDIO_BIT_DEPTH_24 = 24,
    AUDIO_BIT_DEPTH_32 = 32,
    AUDIO_BIT_DEPTH_MAX,
} audio_bit_depth;

typedef enum {
    AUDIO_CHANNEL_INVALID = -1,
    AUDIO_CHANNEL_1 = 1,
    AUDIO_CHANNEL_2 = 2,
    AUDIO_CHANNEL_4 = 4,
    AUDIO_CHANNEL_8 = 8,
    AUDIO_CHANNEL_16 = 16,
    AUDIO_CHANNEL_32 = 32,
    AUDIO_CHANNEL_MAX,
} audio_channel;

typedef enum {
    INPUT_MODE_SINGLE_ENDED = 1,
    INPUT_MODE_DIFFERENTIAL,
    INPUT_MODE_MAX,
} analog_input_mode;

typedef enum {
    AUDIO_DEV_WORK_MODE_MONO = 0,
    AUDIO_DEV_WORK_MODE_QUEUE = 1,
} audio_dev_work_mode;

typedef enum {
    AUDIO_TRACK_MODE_STEREO = 0,
    AUDIO_TRACK_MODE_DOUBLE_LEFT,
    AUDIO_TRACK_MODE_DOUBLE_RIGHT,
    AUDIO_TRACK_MODE_EXCHANGE,
    AUDIO_TRACK_MODE_DOUBLE_MONO,
    AUDIO_TRACK_MODE_ONLY_RIGHT,
    AUDIO_TRACK_MODE_ONLY_LEFT,
    AUDIO_TRACK_MODE_MUTED,

    AUDIO_TRACK_MODE_MAX
} audio_track_mode;

typedef enum {

    AUDIO_I2S_STD_MODE = 0,
    AUDIO_I2S_PCM_MODE,
    AUDIO_I2S_TDM_MODE,
    AUDIO_I2S_TDMPCM_MODE,
    AUDIO_I2S_MODE_MAX
} audio_i2s_mode;

typedef enum {
    AUDIO_I2S_PCM_0_DELAY = 0,   /* 0  bclk cycles delay */
    AUDIO_I2S_PCM_1_DELAY = 1,   /* 1  bclk cycles delay */
    AUDIO_I2S_PCM_8_DELAY = 8,   /* 8  bclk cycles delay */
    AUDIO_I2S_PCM_16_DELAY = 16, /* 16 bclk cycles delay */
    AUDIO_I2S_PCM_17_DELAY = 17, /* 17 bclk cycles delay */
    AUDIO_I2S_PCM_24_DELAY = 24, /* 24 bclk cycles delay */
    AUDIO_I2S_PCM_32_DELAY = 32, /* 32 bclk cycles delay */
    AUDIO_I2S_PCM_DELAY_MAX
} audio_i2s_pcm_delay;

typedef enum {
    AUDIO_I2S_BCLK_1_DIV = 1,
    AUDIO_I2S_BCLK_2_DIV = 2,
    AUDIO_I2S_BCLK_3_DIV = 3,
    AUDIO_I2S_BCLK_4_DIV = 4,
    AUDIO_I2S_BCLK_6_DIV = 6,
    AUDIO_I2S_BCLK_8_DIV = 8,
    AUDIO_I2S_BCLK_12_DIV = 12,
    AUDIO_I2S_BCLK_24_DIV = 24,
    AUDIO_I2S_BCLK_32_DIV = 32,
    AUDIO_I2S_BCLK_48_DIV = 48,
    AUDIO_I2S_BCLK_64_DIV = 64,
    AUDIO_I2S_BCLK_128_DIV = 128,
    AUDIO_I2S_BCLK_MAX
} audio_i2s_bclk_sel;

typedef enum {
    AUDIO_I2S_MCLK_128_FS = 0, /* mclk 128*fs */
    AUDIO_I2S_MCLK_256_FS,     /* mclk 256*fs */
    AUDIO_I2S_MCLK_384_FS,     /* mclk 384*fs */
    AUDIO_I2S_MCLK_512_FS,     /* mclk 512*fs */
    AUDIO_I2S_MCLK_768_FS,     /* mclk 768*fs */
    AUDIO_I2S_MCLK_1024_FS,    /* mclk 1024*fs */
    AUDIO_I2S_MCLK_MAX
} audio_i2s_mclk_sel;

typedef struct {
    audio_channel channels;
    audio_sample_rate sample_rate;
    audio_bit_depth bit_depth;
    xmedia_bool interleaved;
    xmedia_u64 timestamp;
    xmedia_u32 channel_mask;
    xmedia_u32 index;
    xmedia_u32 size;
    xmedia_void* data;
} audio_frame;

typedef struct {
    audio_frame org_frame;
    audio_frame ec_frame;
} audio_ext_frame;

typedef struct {
    xmedia_u32 len;
    xmedia_u32 index;
    xmedia_u64 timestamp;
    xmedia_void* data;
} audio_stream;

typedef struct {
    xmedia_char file_path[MAX_AUDIO_FILE_PATH_LEN];
    xmedia_char file_name[MAX_AUDIO_FILE_PATH_NAME];
    xmedia_u32  max_file_size;
} audio_dump;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

