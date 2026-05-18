#ifndef __TW2960_H__
#define __TW2960_H__

#include <linux/ioctl.h>
//#include "hi_type.h"



#define TW2960_NTSC	        1
#define TW2960_PAL	        2
#define TW2960_AUTO	        3
#define TW2960_INVALID_MODE	4

typedef struct _tw2960_video_norm
{
    unsigned int chip;
    unsigned int mode;
} tw2960_video_norm;


#define TW2960_1D1_MODE     0
#define TW2960_2D1_MODE     1
#define TW2960_4CIF_MODE    2
#define TW2960_4D1_MODE     3

typedef struct _tw2960_work_mode
{
    unsigned int chip;
    unsigned int mode;
} tw2960_work_mode;


typedef struct _tw2960_video_loss
{
    unsigned int chip;
    unsigned int ch;
    unsigned int is_lost;
} tw2960_video_loss;


#define TW2960_SET_HUE          0x01
#define TW2960_SET_CONTRAST     0x02
#define TW2960_SET_BRIGHT       0x04
#define TW2960_SET_SATURATION   0x08

typedef struct _tw2960_image_adjust
{
    unsigned char chip;
    unsigned char chn;
	unsigned char hue;
	unsigned char contrast;
	unsigned char brightness;
	unsigned char saturation;
	unsigned long item_sel;/* use such as "TW2960_SET_HUE|TW2960_SET_CONTRAST" */
} tw2960_image_adjust;


typedef struct _tw2960_audio_format
{
	unsigned char format;   /* 0:i2s; 1:dsp */
} tw2960_audio_format;

typedef struct _tw2960_audio_output
{
	unsigned char PlaybackOrLoop;   /*0:playback; 1:loop*/
	unsigned char channel;          /*[0, 15]*/
	unsigned char reserve[6];
} tw2960_audio_output;


typedef enum _tw2960_audio_samplerate
{
	TW2960_SAMPLE_RATE_8000,
	TW2960_SAMPLE_RATE_16000,
	TW2960_SAMPLE_RATE_32000,
	TW2960_SAMPLE_RATE_44100,
	TW2960_SAMPLE_RATE_48000,
	TW2960_SAMPLE_RATE_BUTT
} tw2960_audio_samplerate;

typedef struct _tw2960_audio_playback
{
    unsigned int chip;
    unsigned int chn;
} tw2960_audio_playback;

typedef struct _tw2960_audio_da_mute
{
    unsigned int chip;
} tw2960_audio_da_mute;

typedef struct _ttw2960_audio_da_volume
{
    unsigned int chip;
    unsigned int volume;
} tw2960_audio_da_volume;


#define TW2960_IOC_MAGIC            't'

#define TW2960CMD_READ_REG		      _IOWR(TW2960_IOC_MAGIC, 1, int) 
#define TW2960CMD_WRITE_REG	        _IOW(TW2960_IOC_MAGIC, 2, int) 

#define TW2960_GET_VIDEO_LOSS		    _IOWR(TW2960_IOC_MAGIC, 4, tw2960_video_loss) 
#define TW2960_SET_IMAGE_ADJUST	    _IOW(TW2960_IOC_MAGIC, 5, tw2960_image_adjust) 
#define TW2960_GET_IMAGE_ADJUST	    _IOWR(TW2960_IOC_MAGIC, 6, tw2960_image_adjust) 
#define TW2960_SET_VIDEO_NORM	      _IOW(TW2960_IOC_MAGIC, 7, tw2960_video_norm) 
#define TW2960_GET_VIDEO_NORM	      _IOWR(TW2960_IOC_MAGIC, 8, tw2960_video_norm) 
#define TW2960_SET_WORK_MODE	      _IOW(TW2960_IOC_MAGIC, 9, tw2960_work_mode) 

#define TW2960_SET_SAMPLE_RATE	    _IOW(TW2960_IOC_MAGIC, 10, tw2960_audio_samplerate) 
#define TW2960_SET_AUDIO_PLAYBACK   _IOW(TW2960_IOC_MAGIC, 11, tw2960_audio_playback) 
#define TW2960_SET_AUDIO_DA_MUTE    _IOW(TW2960_IOC_MAGIC, 12, tw2960_audio_da_mute)
#define TW2960_SET_AUDIO_DA_VOLUME  _IOW(TW2960_IOC_MAGIC, 13, tw2960_audio_da_volume)
#define TW2960_SET_AUDIO_FORMAT     _IOW(TW2960_IOC_MAGIC, 14, tw2960_audio_format)

int tw2960_set_audio_samplerate(tw2960_audio_samplerate samplerate);
int tw2960_set_audio_format(tw2960_audio_format audio_format);
#endif


