/*
 * Copyright (c) XMEDIA. All rights reserved.
 */
#ifndef _RESAMPLER_API_H_
#define _RESAMPLER_API_H_

#include "type.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAXFRAMESIZE 2048

#ifndef ERR_CODE_RESAMPLE_PREFIX
#define ERR_CODE_RESAMPLE_PREFIX 0x80000000
#endif

/* input handle is invalid */
#define ERR_CODE_RESAMPLE_HANDLE (ERR_CODE_RESAMPLE_PREFIX | 0x0001)

/* pcm circ buffer state is invalid */
#define ERR_CODE_RESAMPLE_PCMBUF (ERR_CODE_RESAMPLE_PREFIX | 0x0002)

/* input sample number is more than MAXFRAMESIZE or input buffer size
   , or input sample number is not invalid (eg. even) */
#define ERR_CODE_RESAMPLE_SAMPLE_NUMBER (ERR_CODE_RESAMPLE_PREFIX | 0x0003)

/* output pcm buffer space is not enough */
#define ERR_CODE_RESAMPLE_OUTPCM_SPACE (ERR_CODE_RESAMPLE_PREFIX | 0x0004)

/* the channels of input pcm is invalid */
#define ERR_CODE_PCM_CHANNEL (ERR_CODE_RESAMPLE_PREFIX | 0x0005)

/* the bit width of input pcm is invalid */
#define ERR_CODE_PCM_FORMAT (ERR_CODE_RESAMPLE_PREFIX | 0x0006)

/* invalid bypass flag */
#define ERR_CODE_INVALID_BYPASSFLAG (ERR_CODE_RESAMPLE_PREFIX | 0x0007)

/* error unknown */
#define ERR_CODE_UNKNOWN (ERR_CODE_RESAMPLE_PREFIX | 0x0008)

/* input Empty pointer */
#define ERR_CODE_INPUT_EMPTY_POINTER (ERR_CODE_RESAMPLE_PREFIX | 0x0009)

/**************************************************************************************
 * Function:    AUDIO_Resampler_Create
 *
 * Description: allocate memory for platform-specific data
 *              clear all the user-accessible fields
 *
 * Inputs:      inrate:  8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000
 *              outrate: 8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000
 *              chans:   1 or 2
 * Outputs:     none
 *
 * Return:      handle to Resampler instance, 0 if malloc fails
 **************************************************************************************/
XMEDIA_VOID *AUDIO_Resampler_Create(XMEDIA_S32 s32Inrate, XMEDIA_S32 s32Outrate, XMEDIA_S32 s32Chans);

/**************************************************************************************
 * Function:    AUDIO_Resampler_Process
 *
 * Description: Resample pcm data to specific samplerate, only for interlaced format
 *
 * Inputs:      inst: valid Resampler instance pointer (HResampler)
 *              inbuf:   pointer to inputbuf
 *              insamps: input number of sample pointers
 * Outputs:     outbuf:  pointer to outputbuf
 *
 * Return:      output sample number per-channel
 * Notes:       sure insamps < MAXFRAMESIZE


 **************************************************************************************/
XMEDIA_S32 AUDIO_Resampler_Process(XMEDIA_VOID *inst, XMEDIA_S16 *s16Inbuf, XMEDIA_S32 s32Insamps, XMEDIA_S16 *s16Outbuf);

/**************************************************************************************
 * Function:    AUDIO_Resampler_Destroy
 *
 * Description: free platform-specific data allocated by ResamplerCreate
 *
 * Inputs:      valid Resampler instance pointer (HResampler)
 * Outputs:     none
 *
 * Return:      none
 **************************************************************************************/
XMEDIA_VOID AUDIO_Resampler_Destroy(XMEDIA_VOID *inst);

/*******************************************************************************
 * Function:	AUDIO_Resampler_GetMaxOutputNum
 *
 * Description: Caculate max output number at specific input number
 *
 * Inputs:		inst:	  valid Resampler instance pointer (XMEDIA_HANDLE)
 * 				insamps:  input data number per-channel, insamps must be even
 * Outputs: 	none
 * Return:		>=0:	  Success, return the max output number per-channel
 * 				other:	  Fail, return error code
 * Notes:
 * 1  if stereo(chans==2), sure insamps%2 == 0
 ******************************************************************************/
XMEDIA_S32 AUDIO_Resampler_GetMaxOutputNum(XMEDIA_VOID *inst, XMEDIA_S32 s32Insamps);

#ifdef __cplusplus
}
#endif

#endif
