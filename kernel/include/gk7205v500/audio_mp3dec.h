/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __AUDIO_MP3DEC_H__
#define __AUDIO_MP3DEC_H__
#include "type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif  /* __cpluscplus */
#endif  /* __cpluscplus */

#define MP3_MAX_NCHANS      2        /* mp3 max number of channels */

#define MP3_MAX_OUT_NSAMPS  1152     /* mp3 max output samples per-frame, per-channel */
#define MP3_MAINBUF_SIZE    4096     /* mp3 minium size of input buffer. UNIT:bytes */
#define MP3DEC_VERSION_MAX_BYTE 64 /**< version max byte size */

/* Defines MP3DEC Version */
typedef enum {
    MPEG1 =  0,
    MPEG2 =  1,
    MPEG25 = 2
} MPEGVersion;

typedef void* mp3dec_handle;
typedef struct mp3dec_version {
    xmedia_u8 version[MP3DEC_VERSION_MAX_BYTE];
} mp3dec_version;

/* Defines MP3DEC error */
typedef enum {
    ERR_MP3_NONE =                 0,   /* <no decode error */
    ERR_MP3_INDATA_UNDERFLOW =    -1,   /* <not enough input data */
    ERR_MP3_MAINDATA_UNDERFLOW =  -2,   /* <not enough input main data */
    ERR_MP3_FREE_BITRATE_SYNC =   -3,   /* <free mode bit_rate error */
    ERR_MP3_OUT_OF_MEMORY =       -4,   /* <decoder not enough memory */
    ERR_MP3_NULL_POINTER =        -5,   /* <input null pointer */
    ERR_MP3_INVALID_FRAMEHEADER = -6,   /* <invalid frame header */
    ERR_MP3_INVALID_SIDEINFO =    -7,   /* <invalid side information */
    ERR_MP3_INVALID_SCALEFACT =   -8,   /* <invalid scale factors */
    ERR_MP3_INVALID_HUFFCODES =   -9,   /* <Huffman decoder error */
    ERR_MP3_FAIL_SYNC =           -10,  /* <find sync word error */

    ERR_MP3_UNKNOWN =             -9999 /* <reserved */
} mp3dec_error;

/* Defines MP3DEC frame information */
typedef struct {
    int bit_rate;             /* <output bit_rate */
    int chans;                /* <output channels,range:1,2 */
    int samp_rate;            /* <output samplerate */
    int bits_per_sample;      /* <output bitwidth */
    int output_samps;         /* <output samples,range:chans*SamplePerFrame */
    int layer;                /* <output layer */
    int version;              /* <output version */
} mp3dec_frame_info;

/**
\brief Get version information.
\attention \n
N/A
\param[in] pVersion    :   version describe struct
\retval ::XMEDIA_SUCCESS   :   Success
\retval ::XMEDIA_FAILURE   :   pVersion is NULL, return XMEDIA_FAILURE
\see \n
N/A
*/
xmedia_s32 mp3dec_get_version(mp3dec_version *mp3_version);

/*
 * brief create and initial decoder device.
 * attention \n
 * Before before deocede,you must call this application programming interface (API) first.
 * param N/A
 * retval ::mp3dec_handle   : Success
 * retval ::NULL          : FAILURE.
 * see \n
 */
mp3dec_handle mp3dec_init_decoder(xmedia_void);

/*
 * brief Free MP3 decoder.
 * attention \n
 * param[in] mp3_decoder MP3decode handle
 * retval \n
 * see \n
 */
xmedia_void mp3dec_free_decoder(mp3dec_handle mp3_decoder);

/*
 * brief Find Sync word before decode.
 * attention \n
 * param[in] mp3_decoder    MP3-Decoder handle
 * param[in] ppInbufPtr     address of the pointer of start-point of the bitstream(little endian format)
 * param[in] pBytesLeft     pointer to BytesLeft that indicates bitstream numbers at input buffer,
 * indicates the left bytes
 * retval :: other : Success, return number bytes  of current frame
 * retval ::<0 ERR_MP3_INDATA_UNDERFLOW
 * see \n
 */
xmedia_s32 mp3dec_decode_find_sync_header(mp3dec_handle mp3_decoder, xmedia_u8 **ppInbufPtr,
    xmedia_s32 *pBytesLeft);

/*
 * brief decoding MPEG frame and output 1152(L2/L3) OR 384(L1) 16bit PCM samples per channel.
 * attention \n
 * param[in] mp3_decoder    MP3-Decoder handle
 * param[in] ppInbufPtr     address of the pointer of start-point of the bitstream
 * param[in] pBytesLeft     pointer to BytesLeft that indicates bitstream numbers at input buffer,
 * indicates the left bytes
 * param[in] pOutPcm        the address of the out pcm buffer,pcm data in noninterlaced fotmat: L/L/L/... R/R/R/...
 * param[in] nReserved      reserved
 * retval :: ERR_MP3_NONE : Success
 * retval :: ERROR_CODE :FAILURE
 * see \n
 */
xmedia_s32  mp3dec_decode(mp3dec_handle mp3_decoder, xmedia_u8 **ppInbufPtr,
    xmedia_s32 *pBytesLeft, xmedia_s16 *pOutPcm, xmedia_s32 nReserved);

/*
 * brief get the frame information.
 * attention \n
 * param[in] mp3_decoder    MP3-Decoder handle
 * param[out] mp3dec_frame_info  frame information
 * retval \n
 * see \n
 */
xmedia_void mp3dec_get_last_frame_info(mp3dec_handle mp3_decoder, mp3dec_frame_info *frame_info);

#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cpluscplus */
#endif  /* __cpluscplus */

#endif  /* _MP3DEC_H */
