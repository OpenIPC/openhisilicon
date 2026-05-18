#ifndef __CX26828_H__
#define __CX26828_H__

#include <linux/ioctl.h>

#define CX26828_VERSION_CODE KERNEL_VERSION(1, 0, 104)

#define CX26828_MODE_NTSC           1
#define CX26828_MODE_PAL            2
#define CX26828_MODE_AUTO           3
#define CX26828_MODE_INVALID        4

#define CX26828_16D1        0
#define CX26828_16E1        1
#define CX26828_MIXED       2


typedef struct _cx26828_video_norm
{
    unsigned int mode;
    unsigned int chip;
} cx26828_video_norm;


#define CX26828_1D1_MODE     1
#define CX26828_2D1_MODE     2
#define CX26828_4CIF_MODE    3
#define CX26828_4D1_MODE     4

typedef struct _cx26828_work_mode
{
    unsigned int mode;
    unsigned int chip;
} cx26828_work_mode;

typedef struct _cx26828_Multiplex_mode
{
    unsigned int mode;
    unsigned int chip;
}  cx26828_Multiplex_mode;

typedef struct _cx26828_video_loss
{
    unsigned int chip;
    unsigned int ch;
    unsigned int is_lost;
} cx26828_video_loss;


#define CX26828_SET_HUE             0x01
#define CX26828_SET_CONTRAST        0x02
#define CX26828_SET_BRIGHT          0x04
#define CX26828_SET_SATURATION      0x08

#define DISABLE_DRE     0
#define SET_DRE         1

#define MAX_DAC_VOLUME  63

typedef struct _cx26828_image_adjust
{
    unsigned char chip;
    unsigned char chn;
    unsigned char hue;
    unsigned char contrast;
    unsigned char brightness;
    unsigned char saturation;
    unsigned char sharpness;
    unsigned long item_sel;/* use such as "CX26828_SET_HUE|CX26828_SET_CONTRAST" */
} cx26828_image_adjust;

typedef struct cx26828_register_struct
{
    int chip_num;
    unsigned long register_address;
    unsigned long register_value;
}cx26828_register_struct;

typedef struct cx26828_i2c_struct 
{
    unsigned char dev_address;
    unsigned char register_address;
    unsigned char register_value;   
}cx26828_i2c_struct;

typedef struct cx26828_dre_struct
{
    int command_type;
    int chip_num;
    int decoder_num;
    unsigned long coe_curve[256];
}cx26828_dre_struct;

typedef struct _cx26828_audio_format
{
    unsigned int chip;
    unsigned char format;   /* 0:i2s; 1:dsp */
    unsigned char master;   /* 0:slave; 1:master */
} cx26828_audio_format;


typedef enum _cx26828_audio_samplerate
{
    CX26828_SAMPLE_RATE_8000  = 8000,
    CX26828_SAMPLE_RATE_16000 = 16000,
    CX26828_SAMPLE_RATE_32000 = 32000,
    CX26828_SAMPLE_RATE_44100 = 44100,
    CX26828_SAMPLE_RATE_48000 = 48000,
    CX26828_SAMPLE_RATE_BUTT
} cx26828_audio_samplerate;

typedef enum _cx26828_audio_bitwidth
{
	CX26828_AUDIO_BITWIDTH_8  = 0,
	CX26828_AUDIO_BITWIDTH_16 = 1,
	CX26828_AUDIO_BITWIDTH_BUTT
} cx26828_audio_bitwidth;

typedef struct _cx26828_audio_attr
{
    unsigned int chip;
    cx26828_audio_samplerate enSamplerate;   /* sample rate*/
    cx26828_audio_bitwidth   enBitwidth;     /* bitwidth*/
    unsigned long            i2s_mode;
    unsigned long            master_mode;
    unsigned long            u32ChnCnt;      /* channle number on FS, valid value:2/4/8/16 */    
} cx26828_audio_attr;


typedef struct _cx26828_audio_playback
{
    unsigned int chip;
    unsigned int audio_ch;
    unsigned int is_loopback;
} cx26828_audio_playback;

typedef struct _cx26828_audio_dac_mute
{
    unsigned int chip;
    unsigned int mute_enable;
} cx26828_audio_dac_mute;

typedef struct _cx26828_audio_dac_volume
{
    unsigned int chip;
    unsigned int volume;
} cx26828_audio_dac_volume;

typedef struct _cx26828_audio_sample_rate
{
    unsigned int chip;
    unsigned int sample_rate;
} cx26828_audio_sample_rate;

typedef struct _cx26828_audio_bit_width
{
    unsigned int chip;
    unsigned int bit_width;
} cx26828_audio_bit_width;

typedef struct _cx26828_audio_chn_num
{
    unsigned int chip;
    unsigned int chn_num;
} cx26828_audio_chn_num;


#define CX26828_IOC_MAGIC                         'c'

#define CX26828CMD_READ_REG                       _IOWR(CX26828_IOC_MAGIC, 1, cx26828_register_struct)
#define CX26828CMD_WRITE_REG                      _IOW(CX26828_IOC_MAGIC,  2, cx26828_register_struct)

#define CX26828_GET_VIDEO_LOSS                    _IOWR(CX26828_IOC_MAGIC, 4, cx26828_video_loss)
#define CX26828_SET_IMAGE_ADJUST                  _IOW(CX26828_IOC_MAGIC,  5, cx26828_image_adjust)
#define CX26828_GET_IMAGE_ADJUST                  _IOWR(CX26828_IOC_MAGIC, 6, cx26828_image_adjust)
#define CX26828_SET_VIDEO_NORM                    _IOW(CX26828_IOC_MAGIC,  7, cx26828_video_norm)
#define CX26828_GET_VIDEO_NORM                    _IOWR(CX26828_IOC_MAGIC, 8, int)
#define CX26828_SET_WORK_MODE                     _IOW(CX26828_IOC_MAGIC,  9, cx26828_work_mode)

#define CX26828_SET_SAMPLE_RATE                   _IOW(CX26828_IOC_MAGIC, 10, cx26828_audio_sample_rate)
#define CX26828_SET_BIT_WIDTH                     _IOW(CX26828_IOC_MAGIC, 11, cx26828_audio_bit_width)
#define CX26828_SET_AUDIO_FORMAT                  _IOW(CX26828_IOC_MAGIC, 12, cx26828_audio_format)
#define CX26828_SET_CHN_NUM                       _IOW(CX26828_IOC_MAGIC, 13, int)


#define CX26828_SET_AUDIO_PLAYBACK                _IOW(CX26828_IOC_MAGIC, 15, cx26828_audio_playback)
#define CX26828_SET_AUDIO_DAC_MUTE                _IOW(CX26828_IOC_MAGIC, 16, cx26828_audio_dac_mute)
#define CX26828_SET_AUDIO_DAC_VOLUME              _IOW(CX26828_IOC_MAGIC, 17, cx26828_audio_dac_volume)
#define CX26828_SET_VIDEO_QUALITY                 _IOW(CX26828_IOC_MAGIC, 18, int)
#define CX26828_SET_DRE                           _IOW(CX26828_IOC_MAGIC, 19, cx26828_dre_struct)
#define CX26828_SET_LOW_PASS_FILTER               _IOW(CX26828_IOC_MAGIC, 20, int)
#define CX26828_GET_MIXED_CHANNEL_SETTING         _IOW(CX26828_IOC_MAGIC, 21, int)
#define CX26828_SET_MIXED_CHANNEL_SETTING         _IOW(CX26828_IOC_MAGIC, 22, int)
#define CX26828_FLUSH_MIXED_CHANNEL_SETTING       _IOW(CX26828_IOC_MAGIC, 23, int)

#define CX26828_ENABLE_MOTION_DETECTION           _IOW(CX26828_IOC_MAGIC, 24, int)
#define CX26828_DISABLE_MOTION_DETECTION          _IOW(CX26828_IOC_MAGIC, 25, int)
#define CX26828_SET_MOTION_DETECTION_PARAMETERS   _IOW(CX26828_IOC_MAGIC, 26, MOT_DET_INFO)
#define CX26828_GET_MOTION_DETECTION_STATUS       _IOW(CX26828_IOC_MAGIC, 27, int) 
#define CX26828_READ_BYTE                         _IOWR(CX26828_IOC_MAGIC,28, cx26828_i2c_struct)
#define CX26828_WRITE_BYTE                        _IOW(CX26828_IOC_MAGIC, 29, cx26828_i2c_struct)
#define CX26828_SET_PTZ_CHANNEL                   _IOW(CX26828_IOC_MAGIC, 30, int)
#define CX26828_SET_Multiplex_MODE                _IOW(CX26828_IOC_MAGIC, 31, cx26828_Multiplex_mode)

#endif


