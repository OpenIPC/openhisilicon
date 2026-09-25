/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __AUDIO_MP3ENC_H__
#define __AUDIO_MP3ENC_H__

#include "type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#define MP3ENC_MAX_NCHANS 2 /* mp3 max number of channels */

#define MP3ENC_MAX_IN_NSAMPS   1152 /* mp3 max output samples per-frame, per-channel */
#define MP3ENC_MAX_OUTBUF_SIZE 2048 /* mp3 minium size of output buffer. UNIT:bytes */

#define MP3ENC_VERSION_MAX_BYTE 64 /**< version max byte size */

typedef struct {
    XMEDIA_U32 sample_rate;     /* encode pcm samplerate, support [8000,48000]. */
    XMEDIA_U32 channels;        /* encode pcm channel, support [1,2]. */
    XMEDIA_U32 bit_rate;        /* encode bitstream bitrate, support[8, 320]. */
    XMEDIA_U32 quality;         /* encode bitstream quality, support[0, 9].
                                    0:  Highest quality, very slow; 9:  Poor quality, but fast. default 7. */
} mp3enc_attr;

typedef void *mp3enc_handle;

/* Defines MP3ENC version */
typedef struct mp3enc_version {
    xmedia_u8 version[MP3ENC_VERSION_MAX_BYTE];
} mp3enc_version;

/**
\brief Get version information.
\attention \n
N/A
\param[in] pVersion       version describe struct
\retval ::XMEDIA_SUCCESS   : Success
\retval ::XMEDIA_FAILURE          : FAILURE
\see \n
N/A
*/
xmedia_s32 mp3enc_get_version(mp3enc_version *pVersion);

/*
 * brief get reasonable default configuration.
 * attention \n
 * param[in] pstConfig    pointer to an configuration information structure.
 * retval ::XMEDIA_SUCCESS   : Success
 * retval ::XMEDIA_FAILURE          : FAILURE
 * see \n
 */
xmedia_s32 mp3enc_init_default_config(mp3enc_attr *config);

/*
 * brief create and initial encoder device.
 * attention \n
 * Before before deocede,you must call this application programming interface (API) first.
 * param[in] enc_attr mp3 encode attr.
 * retval :mp3enc_handle   : Success
 * retval :NULL      : FAILURE.
 * see \n
 */
mp3enc_handle mp3enc_create(mp3enc_attr *enc_attr);

/*
 * brief Free MP3 decoder.
 * attention \n
 * param[in] mp3_encoder MP3encode handle
 * retval \n
 * see \n
 */
xmedia_void mp3enc_destroy(mp3enc_handle mp3_encoder);

/*
 * brief encode input 1152(L2/L3) OR 384(L1) 16bit PCM samples per channel and output MPEG frame.
 * attention \n
 * param[in] mp3_encoder        MP3-Encoder handle
 * param[in] pcm_buf            address of the pointer of start-point of the input-pcm buf,
 * this data need be uninterleaved.
 * param[in] out_buf            pointer to BytesLeft that indicates bitstream numbers at input buffer,
 * indicates the left bytes.
 * param[in] num_out_bytes      the address of the out pcm buffer,pcm data in noninterlaced fotmat: L/L/L/... R/R/R/...
 * retval :: ERR_MP3_NONE :     Success
 * retval :: ERROR_CODE :       FAILURE
 */
xmedia_s32 mp3enc_process_frame(mp3enc_handle mp3_encoder, xmedia_s16 *pcm_buf[2], xmedia_u8 *out_buf, xmedia_s32 *num_out_bytes);

#ifdef __cplusplus
#if _cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* __MP3ENC_H__ */
