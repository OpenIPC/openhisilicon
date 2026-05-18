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
*     Copyright (c) 2011 Conexant Systems, Inc.
*     All rights reserved.
*
\******************************************************************* ---*/

#ifndef __CX_API_DEFINES_H__
#define __CX_API_DEFINES_H__

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

// defines for video standards
#define NTSC_VIDEO_STD          0
#define PAL_VIDEO_STD           1
#define NTSC_EFFIO_STD          2
#define PAL_EFFIO_STD           3


// Color control constants
#define VIDEO_PROCAMP_MIN       0
#define VIDEO_PROCAMP_MAX       10000
#define UNSIGNED_BYTE_MIN       0
#define UNSIGNED_BYTE_MAX       0xFF
#define SIGNED_BYTE_MIN         -128
#define SIGNED_BYTE_MAX         127

// Default video color settings
#define SHARPNESS_DEFAULT       50
#define SATURATION_DEFAULT      5000
#define BRIGHTNESS_DEFAULT      5000
#define CONTRAST_DEFAULT        5000
#define HUE_DEFAULT             5000
#define BRIGHTNESS_NTSC_DEFAULT 6000

// Number of decoders and encoders
#define MAX_DECODERS            8
#define QUAD_DECODERS           4
#define MAX_ENCODERS            2
#define MAX_CAMERAS             16

#define MAX_D1_WIDTH            720
#define MAX_CIF_WIDTH           352

/* HSCALE_RATIO Settings */
#define PIXELS_WIDTH960         0x0
#define PIXELS_WIDTH720         0x0
#define PIXELS_WIDTH704         0x5D18
#define PIXELS_WIDTH360         0x100000
#define PIXELS_WIDTH352         0x10BA2F
#define PIXELS_WIDTH320         0x140000
#define PIXELS_WIDTH176         0x3115B2
#define PIXELS_WIDTH160         0x378D84


/* Cascade direction */
#define CASCADE_DIR_DISABLED        0x0
#define CASCADE_DIR_INPUT           0x1
#define CASCADE_DIR_OUTPUT          0x2

/* Audio Sample Rates */
#define AUDIO_SAMPLING_RATE_48  48000
#define AUDIO_SAMPLING_RATE_44  44100
#define AUDIO_SAMPLING_RATE_32  32000
#define AUDIO_SAMPLING_RATE_16  16000
#define AUDIO_SAMPLING_RATE_8   8000

typedef struct CX_COLOR_CTRL
{
    int _brightness[MAX_DECODERS];
    int _contrast[MAX_DECODERS];
    int _hue[MAX_DECODERS];
    int _saturation[MAX_DECODERS];
    int _sharpness[MAX_DECODERS];
}CX_COLOR_CTRL;

typedef enum{
    OUTPUT_RATE_13PT5_MHZ = 135,
    OUTPUT_RATE_27_MHZ = 270,
    OUTPUT_RATE_36_MHZ = 360,
    OUTPUT_RATE_54_MHZ = 540,
    OUTPUT_RATE_72_MHZ = 720,
    OUTPUT_RATE_108_MHZ = 1080,
    OUTPUT_RATE_144_MHZ = 1440
}OUTPUT_RATES;

typedef enum{
    AUDIO_GAIN_DISABLE = 0,
    AUDIO_GAIN_ENABLE  = 1
}AUDIO_OUTPUT_GAIN;

typedef enum{
    AUDIO_RECORD_OUTPUT = 0,
    AUDIO_MIX_OUTPUT    = 1,
    AUDIO_PLAYBACK      = 2
}AUDIO_OUTPUT_INTERFACES;

typedef enum{
    AUDIO_I2S_MODE  = 0,
    AUDIO_DSP_MODE  = 1,
    AUDIO_DSP1_MODE = 2
}AUDIO_I2S_MODES;

typedef enum{
    AUDIO_DATA_WIDTH_8BIT  = 0,
    AUDIO_DATA_WIDTH_16BIT = 1,
    AUDIO_DATA_WIDTH_24BIT = 2
}AUDIO_DATA_WIDTH;

typedef enum{
    AUDIO_SLAVE_MODE  = 0,
    AUDIO_MASTER_MODE = 1
}AUDIO_INTERFACE_MODES;

typedef enum{
    AUDIO_BIT_DELAY_0BIT = 0,
    AUDIO_BIT_DELAY_1BIT = 1,
    AUDIO_BIT_DELAY_2BIT = 2
}AUDIO_BIT_DELAY;

typedef enum{
    AUDIO_ULAW_COMPRESSION = 0,
    AUDIO_ALAW_COMPRESSION = 1,
    AUDIO_UNCOMPRESSION    = 2
}AUDIO_COMPRESSION_MODES;

typedef enum{
    AUDIO_2_CHANNELS         = 0,
    AUDIO_4_CHANNELS         = 1,
    AUDIO_8_CHANNELS         = 2,
    AUDIO_16_CHANNELS        = 3,
    AUDIO_24_CHANNELS        = 4,
    AUDIO_PLAYBACK_R_CHANNEL = 5,
    AUDIO_PLAYBACK_L_CHANNEL = 6
}AUDIO_NUM_CHANNELS;

typedef enum{
    BEST_CROSS_COLOR                 = 0,
    CROSS_COLOR_MUCH_BETTER          = 1,
    CROSS_COLOR_SLIGHTLY_BETTER      = 2,
    BEST_DOT_CRAWL_AND_HANG_DOT      = 3,
    DEFAULT_VIDEO_QUALITY            = 4
}VIDEO_QUALITY;

typedef enum{
    LPF7        = 0,
    LPF8        = 1,
    LPF9        = 2,
    LPF10       = 3,
    LPF_DISABLE = 4
}LPF_MODE_SEL;

#define MAX_NUM_BLK 240
typedef struct _MOT_DET_INFO
{
    unsigned char   chip;
    unsigned char   ch;
    unsigned char   enable[8];
    
    unsigned char   mot_det_enable;                // Enable "motion" detection
    unsigned char   mot_det_blk[MAX_NUM_BLK];      // List of blocks detected with motion by chip
    unsigned char   mot_num_blks_detected;         // Number of blocks detected with motion

    unsigned char   no_mot_det_enable;             // Enable "still frame" detection
    unsigned char   no_mot_detected;               // Number of blocks detected with no motion

    unsigned char   black_det_enable;              // Enable "black" detection
    unsigned char   black_detected;                // Low-light condition detected

    unsigned char   white_det_enable;              // Enable "white" detection
    unsigned char   white_detected;                // Daylight condition detected

    unsigned char   mot_det_blk_sel[MAX_NUM_BLK];  // Selected blocks for detection

    unsigned char   mot_det_sens_thresh;           // Motion sensitivity threshold
    unsigned char   mot_det_block_thresh;          // Interrupt generated if number of blks detected is greater than this value

    unsigned char   no_mot_det_sens_thresh;        // Still frame sensitivity threshold
    unsigned char   no_mot_det_block_thresh;       // Number of blocks required for still frame
    unsigned char   no_mot_det_still_thresh;       // Number of consecutive still frames required before interrupt

    unsigned long   black_pixel_count_thresh;      // Black frame detected if number of "black" pixels > value
    unsigned char   black_pixel_det_thresh;        // Pixel must be less than this value to be considered "black"

    unsigned long   white_pixel_count_thresh;      // White frame detected if number of "white" pixels > value
    unsigned char   white_pixel_det_thresh;        // Pixel must be less than this value to be considered "white"

    unsigned char   mot_det_skip_n_frames;         // Number of frames to skip
    
    unsigned long   mot_stat0;                     // Motion Detection Status
    unsigned long   mot_stat1;
    unsigned long   mot_stat2;
    unsigned long   mot_stat3;
    unsigned long   mot_stat4;
    unsigned long   mot_stat5;
    unsigned long   mot_stat6;
    unsigned long   mot_stat7;
}MOT_DET_INFO, *PMOT_DET_INFO;

#define SetBit(Bit)  (1 << Bit)

inline unsigned char getBit(unsigned long sample, unsigned char index)
{
    return (unsigned char) ((sample >> index) & 1);
}

inline unsigned long clearBitAtPos(unsigned long value, unsigned char bit)
{
    return value & ~(1 << bit);
}

inline unsigned long setBitAtPos(unsigned long sample, unsigned char bit)
{
    sample |= (1 << bit);
    return sample;

}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CX_API_DEFINES_H__ */
