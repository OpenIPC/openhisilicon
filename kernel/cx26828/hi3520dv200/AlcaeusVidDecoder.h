/*+++ *******************************************************************\
*
*  Copyright and Disclaimer:
*
*     ---------------------------------------------------------------
*     This software is provided "AS IS" without warranty of any kind,
*     either expressed or implied, including but not limited to the
*     implied warranties of noninfringement, merchantability and/or
*     fitness for a particular purpose.
*     ---------------------------------------------------------------
*
*     Copyright (c) 2012 Conexant Systems, Inc.
*     All rights reserved.
*
\******************************************************************* ---*/
#ifndef _ALCAEUS_VID_DECODER_H_
#define _ALCAEUS_VID_DECODER_H_

#include  "Comm.h"
#include "CxApiDefines.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/* video omux mode */
#define OMUXMODE_DISABLED                         0x0
#define OMUXMODE_PASSTHROUGH                      0x1
#define OMUXMODE_1_STREAM_REMUX                   0x2
#define OMUXMODE_2_STREAM_REMUX                   0x3
#define OMUXMODE_4_STREAM_REMUX                   0x4
#define OMUXMODE_8_STREAM_REMUX                   0x5

/* dvp demux mode */
#define DVP_DEMUX_MODE_1STREAM                    0x0
#define DVP_DEMUX_MODE_2STREAMS                   0x1
#define DVP_DEMUX_MODE_3STREAMS                   0x2
#define DVP_DEMUX_MODE_4STREAMS                   0x3

/* dvp demux clock mode */
#define DVP_DEMUX_MODE_SINGLE_EDGE                0
#define DVP_DEMUX_MODE_DUAL_EDGE                  1

/* dvp demux clock polarity */
#define DVP_DEMUX_CLK_POL_NONINVERTED             0
#define DVP_DEMUX_CLK_POL_INVERTED                1

/* dvp omux clock polarity */
#define DVP_OMUX_CLK_POL_NONINVERTED              0
#define DVP_OMUX_CLK_POL_INVERTED                 1

/* dvp omux clock mode */
#define DVP_OMUX_CLK_MODE_SINGLE_EDGE             0
#define DVP_OMUX_CLK_MODE_DUAL_EDGE               1

/* dvp omux stream bandwidth */
#define DVP_OMUX_STREAM_BW_27MBYTES_1STREAM       0x0
#define DVP_OMUX_STREAM_BW_13PT5MBYTES_1STREAM    0x1

/* DVP_OMUX_PACK_ENG_SEL */
#define DVP_OMUX_PACK_ENG_SEL_LOW                 0x0
#define DVP_OMUX_PACK_ENG_SEL_HIGH                0x1

/* Video framer select */
#define VIDEO_FRAMER_0                            0x0
#define VIDEO_FRAMER_1                            0x1
#define VIDEO_FRAMER_2                            0x2
#define VIDEO_FRAMER_3                            0x3

/*
  video_expand_mode:
      0: D1 to D1
      1: 960 to 960
      2: 960 cropped to D1
      3: D1 HBI extend to 960H
      4: D1 active extend to 960H
      5: CIF to CIF
      6: Half 960H to Half 960H
      7: Half 960H HBI extend to 960H
      8: Half 960H cropped to Half D1
      9: Half D1 HBI extend to 960H
*/
typedef enum{
    VIDEO_EXTEND_D1              = 0,
    VIDEO_EXTEND_960H            = 1,
    VIDEO_EXTEND_960H_D1         = 2,
    VIDEO_EXTEND_D1_HBI_960H     = 3,
    VIDEO_EXTEND_D1_ACT_960H     = 4,
    VIDEO_EXTEND_CIF             = 5,
    VIDEO_EXTEND_HLF_960H        = 6,
    VIDEO_EXTEND_HLF_960H_HBI    = 7,
    VIDEO_EXTEND_HLF_960H_HLF_D1 = 8,
    VIDEO_EXTEND_HLF_D1_HBI_960H = 9
}VIDEO_EXTEND_MODES;

/*
 * video_frmr_mode:
 *        0: D1 NTSC Normal Frame{I}
 *        1: D1 PAL Normal Frame{I}
 *        2: E1 NTSC Normal Frame{I}
 *        3: E1 PAL Normal Frame{I}
 *        4: D1 NTSC Normal Frame{II}
 *        5: D1 PAL Normal Frame{II}
 *        6: E1 NTSC Normal Frame{II}
 *        7: E1 PAL Normal Frame{II}
 *        8: D1 NTSC Big Frame x2way
 *        9: D1 PAL Big Frame x2way
 *        A: E1 NTSC Big Frame x2way
 *        B: E1 PAL Big Frame x2way
 *        C: D1 NTSC Big Frame x4way
 *        D: D1 PAL Big Frame x4way
 *        E: E1 NTSC Big Frame x4way
 *        F: E1 PAL Big Frame x4way
*/
typedef enum{
    VIDEO_D1_NTSC_NORMAL_FRAMER_I  = 0x0,
    VIDEO_D1_PAL_NORMAL_FRAMER_I   = 0x1,
    VIDEO_E1_NTSC_NORMAL_FRAMER_I  = 0x2,
    VIDEO_E1_PAL_NORMAL_FRAMER_I   = 0x3,
    VIDEO_D1_NTSC_NORMAL_FRAMER_II = 0x4,
    VIDEO_D1_PAL_NORMAL_FRAMER_II  = 0x5,
    VIDEO_E1_NTSC_NORMAL_FRAMER_II = 0x6,
    VIDEO_E1_PAL_NORMAL_FRAMER_II  = 0x7,
    VIDEO_D1_NTSC_BIG_FRAMER_X2WAY = 0x8,
    VIDEO_D1_PAL_BIG_FRAMER_X2WAY  = 0x9,
    VIDEO_E1_NTSC_BIG_FRAMER_X2WAY = 0xA,
    VIDEO_E1_PAL_BIG_FRAMER_X2WAY  = 0xB,
    VIDEO_D1_NTSC_BIG_FRAMER_X4WAY = 0xC,
    VIDEO_D1_PAL_BIG_FRAMER_X4WAY  = 0xD,
    VIDEO_E1_NTSC_BIG_FRAMER_X4WAY = 0xE,
    VIDEO_E1_PAL_BIG_FRAMER_X4WAY  = 0xF
}VIDEO_FRAMER_MODES;

typedef enum
{
    EQ_MODE_AUTO        = 0,
    EQ_MODE_MANUAL      = 1,
    EQ_MODE_FULLMANUAL  = 2,
    EQ_MODE_DISABLE     = 3
}EQ_MODE_SEL;

typedef enum
{
    EQ_CABLE_CAT5       = 0,
    EQ_CABLE_COAX       = 1,
}EQ_CABLE_TYPE;

extern void ALCS_Reset(const CX_COMMUNICATION *p_comm);

extern int ALCS_initialize(const CX_COMMUNICATION *p_comm, int *video_standard);

extern int ALCS_setVideoStandard(const CX_COMMUNICATION *p_comm,
                                 int *standard);

extern int
ALCS_setResolution(const CX_COMMUNICATION *p_comm,
                   unsigned long width,
                   const unsigned long height);

extern int
ALCS_setMacrovisionEnable(const CX_COMMUNICATION *p_comm,
                          const unsigned char enable);

extern int
ALCS_setUvSuppress(const CX_COMMUNICATION *p_comm,
                   const unsigned char value);

extern int
ALCS_setupD1(const CX_COMMUNICATION *p_comm,
             const unsigned long channel,
             const unsigned char dvp_port,
             const unsigned long cascade_direction,
             const unsigned char use_scaled_output);

extern int
ALCS_setupByteInterleaveOutput(const CX_COMMUNICATION *p_comm,
                               const unsigned char num_channels, /* Max 4 channels per port, 1, 2, 4, 8 */
                               const unsigned long size,         /* 720, 360, 352, 320 */
                               const unsigned long output_rate,  /* 13.5MHz, 27MHz, 54MHz, 108MHz */
                               const unsigned char dvp_port,     /* port A, B, C, D */
                               unsigned long *src_sel,           /* Video source/s */
                               const unsigned char strip_invalid_pixels,
                               const int *standard,
                               unsigned long *video_extend_mode);

extern int
ALCS_setupByteInterleaveInput(const CX_COMMUNICATION *p_comm,
                              const unsigned char num_channels, /* Max 4 channels per port, 1, 2, 4 */
                              const unsigned long size,         /* 720, 360, 352, 320 */
                              const unsigned long output_rate,  /* 13.5MHz, 27MHz, 54MHz, 108MHz */
                              const unsigned char dvp_port,     /* port A, B, C, D */
                              unsigned long *src_sel);          /* Video source/s */

extern int
ALCS_setup16CifBigFrame(const CX_COMMUNICATION *p_comm,
                        unsigned long standard);

extern int
ALCS_setupLowFrameRate(const CX_COMMUNICATION *p_comm,
                       const int standard,
                       const unsigned char dvp_port,             /* port A, B, C, D */
                       unsigned long *src_sel,                   /* Video source/s */
                       const unsigned char strip_invalid_pixels);

/* audio */
extern int
ALCS_setDACVolume(const CX_COMMUNICATION *p_comm,
                  const unsigned long volume);                    /* volume 0-63 */

extern unsigned long
ALCS_getDACVolume(const CX_COMMUNICATION *p_comm);

extern int
ALCS_setupAudioADCInput(const CX_COMMUNICATION *p_comm,
                        const unsigned long sampling_rate);

extern int
ALCS_setupI2SCascade(const CX_COMMUNICATION *p_comm,
                     const unsigned long data_width,
                     const unsigned long bit_delay,
                     const unsigned long cascade_direction);
extern int
ALCS_setupAudioDACOutput(const CX_COMMUNICATION *p_comm,
                         const unsigned long sampling_rate,
                         const unsigned long audio_out_sel,
                         const unsigned long audio_gain_en,
                         const unsigned long master_mode);

extern int
ALCS_setupI2SInterface(const CX_COMMUNICATION *p_comm,
                       const unsigned long output_interface,     /* Record, Mix, Playback */
                       const unsigned long i2s_mode,             /* I2S, DSP, DSP1 */
                       const unsigned long master_mode,          /* Master mode, Slave mode */
                       const unsigned long data_width,           /* 8bit, 16bit, 24bit */
                       const unsigned long bit_delay,            /* 0bit, 1bit, 2bit */
                       const unsigned long num_channels,
                       unsigned long      *map_sources,
                       const unsigned long compression_mode);    /* uLaw, aLaw, unCompression */

/* color control */
extern int
ALCS_setBrightness(const CX_COMMUNICATION *p_comm,
                   const int brightness,
                   const unsigned char decoder);

extern int
ALCS_setContrast(const CX_COMMUNICATION *p_comm,
                 const int contrast,
                 const unsigned char decoder);

extern int
ALCS_setHue(const CX_COMMUNICATION *p_comm,
            const int hue,
            const unsigned char decoder);

extern int
ALCS_setSaturation(const CX_COMMUNICATION *p_comm,
                   const int saturation,
                   const unsigned char decoder);

extern int
ALCS_setSharpness(const CX_COMMUNICATION *p_comm,
                  const int sharpness,
                  const unsigned char decoder);

/* Motion detect */
extern int
ALCS_initializeMotionDetect(const CX_COMMUNICATION *p_comm,
                            const PMOT_DET_INFO p_mot_det_info,
                            const unsigned long standard,
                            const int ch);

extern int
ALCS_startMotionDetect(const CX_COMMUNICATION *p_comm,
                       const PMOT_DET_INFO p_mot_det_info,
                       const int ch,
                       unsigned long *mot_det_intr_msk);

extern int
ALCS_stopMotionDetect(const CX_COMMUNICATION *p_comm,
                      const int ch);

extern int
ALCS_processMotionDetect(const CX_COMMUNICATION *p_comm,
                         const PMOT_DET_INFO p_mot_det_info,
                         const unsigned long standard,
                         const int ch);

extern int
ALCS_setDRE(const CX_COMMUNICATION *p_comm,
            unsigned long *coe_curve,
            const unsigned char decoder);

extern int
ALCS_disableDRE(const CX_COMMUNICATION *p_comm,
                const unsigned char decoder);

extern int
ALCS_setVideoQuality(const CX_COMMUNICATION *p_comm,
                     const unsigned long standard,
                     const unsigned long video_quality,
                     const unsigned char decoder);

extern int
ALCS_setLowPassFilter(const CX_COMMUNICATION *p_comm,
                     const unsigned long lpf_sel,
                     const unsigned char decoder);

extern int
ALCS_disableLowPassFilter(const CX_COMMUNICATION *p_comm,
                         const unsigned char decoder);
                         
extern int
ALCS_writeRegister(const CX_COMMUNICATION *p_comm,
                const unsigned long registerAddress,
                const unsigned long value);

extern int
ALCS_readRegister(const CX_COMMUNICATION *p_comm,
               const unsigned long registerAddress,
               unsigned long *p_value);

extern int
ALCS_getVideoLostStatus(const CX_COMMUNICATION *p_comm,
                        const unsigned char decoder,
                        int *lostStatus);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // _ALCAEUS_VID_DECODER_H_
