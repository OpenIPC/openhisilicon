/* cx26828.c
 *
 * Copyright (c) 2010 Hisilicon Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program;
 *
 * 2010-09-12  Create file.
 */


#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/mm.h>
#include <linux/miscdevice.h>
#include <linux/proc_fs.h>

#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/system.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/string.h>
#include <linux/list.h>
#include <asm/delay.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/poll.h>
#include <asm/bitops.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <linux/moduleparam.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>

#include "Comm.h"
#include "gpio_i2c.h"
#include "cx26828_def.h"
#include "cx26828.h"
#include "Comm.c"
#include "AlcaeusVidDecoder.c"
#include "AlcaeusColibri.c"
//#include "motionDetection.c"



#define CX26828A_I2C_ADDR   0x8c
#define CX26828B_I2C_ADDR   0x8e

static int norm = CX26828_MODE_PAL;
static int workmode = CX26828_16D1;
static int Vi_Multiplex_Mode= CX26828_4D1_MODE;
static int Vi_Clk_Freq= OUTPUT_RATE_108_MHZ;

#if 0
static int chips = 2;
#else
static int chips = 1;
#endif


//Channel map
static unsigned long src_sel[MAX_DECODERS * 2];

//For mixed D1/960H mode, this is the channel video resolution map
static int mixed_video_resolution[MAX_DECODERS * 2];

//This is I2C address
static unsigned char cx26828_i2c_addr[] = {CX26828A_I2C_ADDR, CX26828B_I2C_ADDR};

//Define I2C interface
CX_COMMUNICATION cx26828_interface[2];

//To receive the color setting from application level
static cx26828_image_adjust analog_cfg;

//The DRE setting
static cx26828_dre_struct dre_cfg;

//The CX26828 register access
static cx26828_register_struct register_cfg;

//The I2C register access for PTZ
//static cx26828_i2c_struct i2c_register_cfg;

//The Audio DAC volume setting
static cx26828_audio_dac_volume audio_dac_volume;

//The Audio DAC Mute setting
static cx26828_audio_dac_mute audio_dac_mute;

//The Audio Sample Rate setting
static cx26828_audio_sample_rate audio_sample_rate;

//The Audio Bit Width setting
static cx26828_audio_bit_width audio_bit_width;

//The Audio Work Mode setting
static cx26828_audio_format  audio_format;

//The Audio playback setting
static cx26828_audio_playback audio_play_back;

//The Audio attr setting
#if 0
static cx26828_audio_attr  audio_attr;
#endif

//The Audio chn num setting
static cx26828_audio_chn_num audio_chn_num;

//The local color setting
static CX_COLOR_CTRL image_color_setting[2];

//The Motion Detectin setting
MOT_DET_INFO _mot_det_info[2][MAX_DECODERS];
#if 0
static MOT_DET_INFO tmp_mot_det_info;
#endif
int motion_detection_standard[2][MAX_DECODERS];

//Initialize CX26828
static int initCx26828(void);

//Reconfig mixed mode for CX26828
static int cx26828_reconfig_mixed_mode(void);

//Set Output Interface
#if 0
static int setOutputInterface(int vip_mode);
#endif

//Each video decoder channel video standard
static int current_video_standard[MAX_DECODERS];

//Each video decoder channel video resolution
static unsigned long current_video_resolution[MAX_DECODERS];

static unsigned long audio_out_sel_map[] = {
        0x0,     // audio adc 1
        0x1,     // audio adc 2
        0x2,     // audio adc 3
        0x3,     // audio adc 4
        0x4,     // audio adc 5
        0x5,     // audio adc 6
        0x6,     // audio adc 7
        0x7,     // audio adc 8
        0x8,     // audio adc 9
        0x9,     // audio_mixed
        0xA,     // audio_pb
};

/////////////////////////////////////////////////////////////////////////////////////////
//setOutputInterface
//
// Bit[1:0] out_mode 0x0: ITU-R BT.601, 0x1: ITU-R BT.656, 0x2: VIP1.1, 0x3: VIP2.0
// Bit[4]   id_ins_extend_en Extend Channel ID/RTC insertion into 1st active video line Enable
//
// The output interface defaults to 656 mode
// 00 - bt.601
// 01 - bt.656
// 10 - VIP 1.1
// 11 - VIP 2.0
#if 0
static int setOutputInterface(int vip_mode)
{
    unsigned long value = 0;
    int i, j = 0;

    unsigned long out_mode = vip_mode ? 0x12 : 0x01;

    for (j = 0; j < chips; j++)
    {
        for (i = 0; i < MAX_DECODERS; i++)
        {
            readDword( &cx26828_interface[j], ALCS_VDECA_OUT_CTRL1 + (0x200*i), &value);
            value &= 0xFFFFFFEC;
            writeDword( &cx26828_interface[j], ALCS_VDECA_OUT_CTRL1 + (0x200*i), value | out_mode);

            readDword( &cx26828_interface[j], ALCS_VDECA_OUT_CTRL_NS + (0x200*i), &value);
            value &= 0xFFFFFFEC;
            writeDword( &cx26828_interface[j], ALCS_VDECA_OUT_CTRL_NS + (0x200*i), value | out_mode);
        }
    }

    printk("setOutputInterface: vip_mode = %d \n", vip_mode);

    return 0;
}
#endif

static int initCx26828(void)
{
    int i = 0, j = 0, port = 0;

    //As the Hi3531 VO channel does not match the CX26828 video channel, we should map it.
    src_sel[0] = 4;
    src_sel[1] = 5;
    src_sel[2] = 6;
    src_sel[3] = 7;
    src_sel[4] = 0;
    src_sel[5] = 1;
    src_sel[6] = 2;
    src_sel[7] = 3;
    src_sel[8] = 4;
    src_sel[9] = 5;
    src_sel[10] = 6;
    src_sel[11] = 7;
    src_sel[12] = 0;
    src_sel[13] = 1;
    src_sel[14] = 2;
    src_sel[15] = 3;


    if(workmode == CX26828_16D1)
    {
        if(CX26828_4D1_MODE == Vi_Multiplex_Mode)
        {
            Vi_Clk_Freq = OUTPUT_RATE_108_MHZ;
        }
        else if(CX26828_2D1_MODE == Vi_Multiplex_Mode)
        {
            Vi_Clk_Freq = OUTPUT_RATE_54_MHZ;
            
        }
        else if(CX26828_1D1_MODE == Vi_Multiplex_Mode)
        {
            Vi_Clk_Freq = OUTPUT_RATE_27_MHZ;
        }
    }
    else if(CX26828_16E1 == workmode)
    {
        if(CX26828_4D1_MODE == Vi_Multiplex_Mode)
        {
            Vi_Clk_Freq = OUTPUT_RATE_144_MHZ;
        }
        else if(CX26828_2D1_MODE == Vi_Multiplex_Mode)
        {
            Vi_Clk_Freq = OUTPUT_RATE_72_MHZ;
        }
        else if(CX26828_1D1_MODE == Vi_Multiplex_Mode)
        {
            Vi_Clk_Freq = OUTPUT_RATE_36_MHZ;
        }
    }

    //Init the motion_detection_standard
    for (i = 0; i < chips; i++)
    {
        for (j = 0; j < MAX_DECODERS; j++)
        {
            if (norm == CX26828_MODE_NTSC)
            {
                switch (workmode)
                {
                    case CX26828_16D1:
                        motion_detection_standard[i][j] = NTSC_VIDEO_STD;
                        break;
                    case CX26828_16E1:
                    default:
                        motion_detection_standard[i][j] = NTSC_EFFIO_STD;
                        break;
                }
            }
            else
            {
                switch (workmode)
                {
                    case CX26828_16D1:
                        motion_detection_standard[i][j] = PAL_VIDEO_STD;
                        break;
                    case CX26828_16E1:
                    default:
                        motion_detection_standard[i][j] = PAL_EFFIO_STD;
                        break;
                }
            }
        }
    }

    //To set color setting to default value.
    for (i = 0; i < chips; i++)
    {
        for (j = 0; j < MAX_DECODERS; j++)
        {

            //For NTSC and PAL, the default brightness is not the same.
            if (norm == CX26828_MODE_NTSC)
            {
                image_color_setting[i]._brightness[j] = BRIGHTNESS_NTSC_DEFAULT;
            }
            else
            {
                image_color_setting[i]._brightness[j] = BRIGHTNESS_DEFAULT;
            }

            image_color_setting[i]._contrast[j] = CONTRAST_DEFAULT;
            image_color_setting[i]._hue[j] = HUE_DEFAULT;
            image_color_setting[i]._saturation[j] = SATURATION_DEFAULT;
            image_color_setting[i]._sharpness[j] = SHARPNESS_DEFAULT;
        }
    }

    //To initialize each CX26828 chip
    for (i = 0; i < chips; i++)
    {
        if (norm == CX26828_MODE_PAL)
        {
           

            if (workmode == CX26828_16D1)
            {
                
                for (j = 0; j < MAX_DECODERS; j++)
                    current_video_standard[j] = PAL_VIDEO_STD;

                //To initialize the video decoder for 16D1 PAL video.
                ALCS_initialize(&cx26828_interface[i], current_video_standard);
               
            }
            else if(workmode == CX26828_16E1)
            {
                
                for (j = 0; j < MAX_DECODERS; j++)
                    current_video_standard[j] = PAL_EFFIO_STD;

                //To initialize the video decoder for 16E1 PAL video.
                ALCS_initialize(&cx26828_interface[i], current_video_standard);
               
            }
           else
            {
                for (j = 0; j < MAX_DECODERS; j++)
                    current_video_standard[j] = (mixed_video_resolution[i * 8 + j] == 0 ?  PAL_VIDEO_STD : PAL_EFFIO_STD);

                //To initialize the video decoder for 16D1/E1 mixed PAL video.
                ALCS_initialize(&cx26828_interface[i], current_video_standard);
            }
           
        }
        else if(norm == CX26828_MODE_NTSC)
        {
           
            if (workmode == CX26828_16D1)
            {
                for (j = 0; j < MAX_DECODERS; j++)
                    current_video_standard[j] = NTSC_VIDEO_STD;

                //To initialize the video decoder for 16D1NTSC video.
                ALCS_initialize(&cx26828_interface[i], current_video_standard);
               
            }
            else if (workmode == CX26828_16E1)
            {
                for (j = 0; j < MAX_DECODERS; j++)
                    current_video_standard[j] = NTSC_EFFIO_STD;

                //To initialize the video decoder for 16E1 NTSC video.
                ALCS_initialize(&cx26828_interface[i], current_video_standard);
                
            }
            else
            {

                for (j = 0; j < MAX_DECODERS; j++)
                    current_video_standard[j] = (mixed_video_resolution[i * MAX_DECODERS + j] == 0 ?  NTSC_VIDEO_STD : NTSC_EFFIO_STD);

                //To initialize the video decoder for 16D1/E1 mixed NTSC video.
                ALCS_initialize(&cx26828_interface[i], current_video_standard);
            }
            
        }


        if (workmode == CX26828_16D1)
        {
            for(port = 0; port < 2; port++)
            {
                for (j = 0; j < MAX_DECODERS; j++)
                {
                    current_video_standard[j] = (norm==CX26828_MODE_PAL ? PAL_VIDEO_STD : NTSC_VIDEO_STD);
                    current_video_resolution[j] = VIDEO_EXTEND_D1;
                }


                //To initialize port A,B to 4 Byteinterleave for 16D1 video.
                ALCS_setupByteInterleaveOutput(&cx26828_interface[i], Vi_Multiplex_Mode, 720,
                        Vi_Clk_Freq, port, src_sel, 1, current_video_standard, current_video_resolution);
                                       
            }
        }
        else if (workmode == CX26828_16E1)
        {
            for(port = 0; port < 2; port++)
            {
                for (j = 0; j < MAX_DECODERS; j++)
                {
                    current_video_standard[j] = (norm==CX26828_MODE_PAL ? PAL_EFFIO_STD : NTSC_EFFIO_STD);
                    current_video_resolution[j] = VIDEO_EXTEND_960H;
                }

                //To initialize port A,B to 4 Byteinterleave for 16E1 video.
                ALCS_setupByteInterleaveOutput(&cx26828_interface[i], Vi_Multiplex_Mode, 960,
                        Vi_Clk_Freq, port, src_sel, 1,  current_video_standard, current_video_resolution);
                
          
            }
        }
        else
        {
            int map_video_resolution[16];
            map_video_resolution[0] = mixed_video_resolution[4];
            map_video_resolution[1] = mixed_video_resolution[5];
            map_video_resolution[2] = mixed_video_resolution[6];
            map_video_resolution[3] = mixed_video_resolution[7];
            map_video_resolution[4] = mixed_video_resolution[0];
            map_video_resolution[5] = mixed_video_resolution[1];
            map_video_resolution[6] = mixed_video_resolution[2];
            map_video_resolution[7] = mixed_video_resolution[3];

            map_video_resolution[8] = mixed_video_resolution[12];
            map_video_resolution[9] = mixed_video_resolution[13];
            map_video_resolution[10] = mixed_video_resolution[14];
            map_video_resolution[11] = mixed_video_resolution[15];
            map_video_resolution[12] = mixed_video_resolution[8];
            map_video_resolution[13] = mixed_video_resolution[9];
            map_video_resolution[14] = mixed_video_resolution[10];
            map_video_resolution[15] = mixed_video_resolution[11];

            for(port = 0; port < 2; port++)
            {
                for (j = 0; j < MAX_DECODERS; j++)
                {
                    current_video_standard[j] = (norm==CX26828_MODE_PAL ? PAL_EFFIO_STD : NTSC_EFFIO_STD);
                    current_video_resolution[j] =  (map_video_resolution[i * MAX_DECODERS + j] == 0 ?  VIDEO_EXTEND_D1_ACT_960H : VIDEO_EXTEND_960H); 
                }

                //To initialize port A,B to 4 Byteinterleave for 16D1/E1 mixed  video.
                ALCS_setupByteInterleaveOutput(&cx26828_interface[i], Vi_Multiplex_Mode, 960,
                        Vi_Clk_Freq, port, src_sel, 1,  current_video_standard, current_video_resolution);
            }
        }
    }
                
    return 0;
}

static void cx26828_set_ptz_channel(int camera_src_sel) 
{
    //
    // Custom settings to support PTZ   
    //
    unsigned long value = 0;     
    
    if ( camera_src_sel >= MAX_DECODERS ) 
        return; 
    
    readDword(&cx26828_interface[0], ALCS_VP_OMUX_C, &value);
    value &= 0xFFF3E08F;
    value |= (0xE0010 | ( (camera_src_sel * 2) << 8) );
    writeDword(&cx26828_interface[0], ALCS_VP_OMUX_C, value);

    writeDword(&cx26828_interface[1], ALCS_VP_OMUX_C, 0x0000); 

    readDword(&cx26828_interface[0], ALCS_VDECA_OUT_CTRL1 + (0x200 * camera_src_sel), &value);
    value &= 0xFFFFFCFF; //clear clk_ph_sel bits
    writeDword(&cx26828_interface[0], ALCS_VDECA_OUT_CTRL1 + (0x200 * camera_src_sel), value | 0x200);  
}

static int cx26828_reconfig_mixed_mode(void) 
{
    int i = 0, j = 0, port = 0;  
    unsigned char num_channels = 4;  

    int map_video_resolution[16]; 
    map_video_resolution[0] = mixed_video_resolution[4];
    map_video_resolution[1] = mixed_video_resolution[5];
    map_video_resolution[2] = mixed_video_resolution[6];
    map_video_resolution[3] = mixed_video_resolution[7];
    map_video_resolution[4] = mixed_video_resolution[0];
    map_video_resolution[5] = mixed_video_resolution[1];
    map_video_resolution[6] = mixed_video_resolution[2];
    map_video_resolution[7] = mixed_video_resolution[3];

    map_video_resolution[8] = mixed_video_resolution[12];
    map_video_resolution[9] = mixed_video_resolution[13];
    map_video_resolution[10] = mixed_video_resolution[14];
    map_video_resolution[11] = mixed_video_resolution[15];
    map_video_resolution[12] = mixed_video_resolution[8];
    map_video_resolution[13] = mixed_video_resolution[9];
    map_video_resolution[14] = mixed_video_resolution[10];
    map_video_resolution[15] = mixed_video_resolution[11];
            
    //To initialize each CX26828 chip
    for (i = 0; i < chips; i++)
    {   
        for(port = 0; port < 2; port++)
        {
            ALCS_disableOutputPort( &cx26828_interface[i], num_channels, port);
        }
        
        if (norm == CX26828_MODE_PAL)
        {
            for (j = 0; j < MAX_DECODERS; j++)
                current_video_standard[j] = (mixed_video_resolution[i * MAX_DECODERS + j] == 0 ?  PAL_VIDEO_STD : PAL_EFFIO_STD);
        }
        else if(norm == CX26828_MODE_NTSC)
        {
            for (j = 0; j < MAX_DECODERS; j++)
                current_video_standard[j] = (mixed_video_resolution[i * MAX_DECODERS + j] == 0 ?  NTSC_VIDEO_STD : NTSC_EFFIO_STD);
        }
              
    
        ALCS_initDecoder(&cx26828_interface[i], current_video_standard); 
        

        if (workmode == CX26828_MIXED)
        {            
            for(port = 0; port < 2; port++)
            {
                for (j = 0; j < MAX_DECODERS; j++)
                {
                    current_video_standard[j] = (norm==CX26828_MODE_PAL ? PAL_EFFIO_STD : NTSC_EFFIO_STD);
                    current_video_resolution[j] =  (map_video_resolution[i * MAX_DECODERS + j] == 0 ?  VIDEO_EXTEND_D1_ACT_960H : VIDEO_EXTEND_960H);
                }

                //To initialize port A,B to 4 Byteinterleave for 16D1/E1 mixed  video.
                ALCS_setupByteInterleaveOutput(&cx26828_interface[i], num_channels, 960,
                        OUTPUT_RATE_144_MHZ, port, src_sel, 1,  current_video_standard, current_video_resolution);
            }
        }
    }

    return 0;
}

void cx26828_audio_init(const CX_COMMUNICATION *p_comm)
{
    //For Hi3521 board, there is only one master I2S clock. So the second CX26828 chip must be slave mode.
    unsigned long  master_mode = (p_comm->i2c_addr == cx26828_i2c_addr[1]) ? AUDIO_SLAVE_MODE : AUDIO_MASTER_MODE;

    ALCS_configAudioRecord(p_comm,
                            AUDIO_I2S_MODE,
                            master_mode,
                            AUDIO_DATA_WIDTH_16BIT,
                            AUDIO_BIT_DELAY_1BIT,
                            AUDIO_8_CHANNELS,
                            audio_out_sel_map,
                            AUDIO_UNCOMPRESSION);

    ALCS_configAudioPlayback(p_comm,
                            AUDIO_I2S_MODE,
                            master_mode,
                            AUDIO_DATA_WIDTH_16BIT,
                            AUDIO_BIT_DELAY_1BIT,
                            AUDIO_PLAYBACK_L_CHANNEL,
                            AUDIO_UNCOMPRESSION);


    ALCS_setupAudioDACOutput( p_comm,
                              AUDIO_SAMPLING_RATE_8,
                              0xA,
                              AUDIO_GAIN_ENABLE,
                              master_mode);
    ALCS_setDACVolume( p_comm, 0x10 );
}

int cx26828_open(struct inode * inode, struct file * file)
{
    return 0;
}

int cx26828_close(struct inode * inode, struct file * file)
{
    return 0;
}

#if 0
//CX26828 I2C access function. For another hardware platform, we should modify this function.
static unsigned int cx26828_write_i2c_1bytesubaddr(unsigned char i2c_addr, unsigned char register_addr, int value, int size) 
{
    gpio_i2c2_write_1bytesubaddr(i2c_addr, register_addr, value, size);
    return 0;
}

//CX26828 I2C access function. For another hardware platform, we should modify this function.
static  unsigned int cx26828_read_i2c_1bytesubaddr(unsigned char i2c_addr, unsigned char register_addr, void *buffer, int num_bytes)
{
    int value = gpio_i2c2_read_1bytesubaddr(i2c_addr, register_addr, num_bytes);

    switch (num_bytes)
    {
        case 1:
            {
                unsigned char register_return_value = value;
                memcpy(buffer, &register_return_value, 1);
            }
            break;
        case 2:
            {
                unsigned short register_return_value = value;
                memcpy(buffer, &register_return_value, 2);
            }
            break;
        case 4:
            {
                unsigned int register_return_value = value;
                memcpy(buffer, &register_return_value, 4);
            }
            break;
        default:
            printk("Read I2C failed, the error data length is %d\n", num_bytes);
            break;
    }

    return 0;
}
#endif

long cx26828_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    cx26828_video_norm stVideoMode;
    cx26828_work_mode work_mode;
    cx26828_Multiplex_mode multiplex_mode;

    int ret = 0, value = 0;
    int i = 0, j;
    unsigned int __user *argp = (unsigned int __user *)arg;

    
   // printk("\ncx26828_ioctl\n");


    switch (_IOC_NR(cmd))
    {
        case _IOC_NR(CX26828CMD_READ_REG)://Read CX26828 register
        {
            if (copy_from_user(&register_cfg, argp, sizeof(register_cfg)))
            {
                return -1;
            }

            ret = ALCS_readRegister(&cx26828_interface[register_cfg.chip_num], register_cfg.register_address, &register_cfg.register_value);

            if (ret >= 0)
            {
                copy_to_user(argp, &register_cfg, sizeof(register_cfg));
            }

            break;
        }

        case _IOC_NR(CX26828CMD_WRITE_REG)://Write CX26828 register
        {
            if (copy_from_user(&register_cfg, argp, sizeof(register_cfg)))
            {
                return -1;
            }
            //printk("Driver Write reigster: chip_num = %d, register_address = 0x%x register_value = 0x%x\n",
            //  register_cfg.chip_num, register_cfg.register_address, register_cfg.register_value);

            ret = ALCS_writeRegister(&cx26828_interface[register_cfg.chip_num], register_cfg.register_address, register_cfg.register_value);
            break;
        }
 #if 0       
        case _IOC_NR(CX26828_READ_BYTE): 
        {
            if (copy_from_user(&i2c_register_cfg, argp, sizeof(i2c_register_cfg)))
            {
                return -1;
            }

            ret = cx26828_read_i2c_1bytesubaddr( i2c_register_cfg.dev_address, i2c_register_cfg.register_address, &i2c_register_cfg.register_value, 1);

            //printk("Driver ReadByte: dev_address = 0x%x, register_address = 0x%x register_value = 0x%x\n",
            //        i2c_register_cfg.dev_address, i2c_register_cfg.register_address, i2c_register_cfg.register_value);
            if (ret >= 0)
            {
                copy_to_user(argp, &i2c_register_cfg, sizeof(i2c_register_cfg));
            }

            break;
        }

        case _IOC_NR(CX26828_WRITE_BYTE):
        {
            if (copy_from_user(&i2c_register_cfg, argp, sizeof(i2c_register_cfg)))
            {
                return -1;
            }
            
            cx26828_write_i2c_1bytesubaddr( i2c_register_cfg.dev_address, i2c_register_cfg.register_address, i2c_register_cfg.register_value, 1);

            //printk("Driver WriteByte: register_address = 0x%x register_value = 0x%x\n",
                    //i2c_register_cfg.register_address, i2c_register_cfg.register_value);

            break;
        }
#endif
        case _IOC_NR(CX26828_SET_PTZ_CHANNEL): //Set PTZ camera channel 
        {
            if (copy_from_user(&value, argp, sizeof(value)))
            {
                return -1;
            }
            
            if (value < 0 || value >= 16)
            {
                return -1;
            }
            
            cx26828_set_ptz_channel( value );

            break;
        }

        case _IOC_NR(CX26828_SET_DRE)://Disable/Set DRE
        {
            if (copy_from_user(&dre_cfg, argp, sizeof(dre_cfg)))
            {
                return -1;
            }

            //printk("Driver SET DRE : command = %d, chip_num = %d, decoder_num = %d\n", dre_cfg.command_type, dre_cfg.chip_num, dre_cfg.decoder_num);
            if (dre_cfg.command_type == DISABLE_DRE)
            {
                i = dre_cfg.chip_num;
                ret = ALCS_disableDRE(&cx26828_interface[i], dre_cfg.decoder_num);
            }
            else
            {
                i = dre_cfg.chip_num;
                ret = ALCS_setDRE(&cx26828_interface[i], dre_cfg.coe_curve, dre_cfg.decoder_num);
            }

            break;
        }

        case _IOC_NR(CX26828_SET_VIDEO_QUALITY)://Set video quality style
        {

            if (copy_from_user(&value, argp, sizeof(value)))
            {
                return -1;
            }


            for (i = 0; i < chips; i++)
            {
                for (j = 0; j < MAX_DECODERS; j++)
                {
                    if (workmode == CX26828_16D1)
                        ALCS_setVideoQuality(&cx26828_interface[i], (norm == CX26828_MODE_PAL) ? PAL_VIDEO_STD : NTSC_VIDEO_STD, value, j);
                    else if (workmode == CX26828_16E1)
                        ALCS_setVideoQuality(&cx26828_interface[i], (norm == CX26828_MODE_PAL) ? PAL_EFFIO_STD : NTSC_EFFIO_STD, value, j);
                }
            }
            break;
        }

        case _IOC_NR(CX26828_SET_LOW_PASS_FILTER): //Set/disable low pass filter
        {
            if (copy_from_user(&value, argp, sizeof(value)))
            {
                return -1;
            }

            for (i = 0; i < chips; i++)
            {
                for (j = 0; j < MAX_DECODERS; j++)
                {
                    if (value == LPF_DISABLE)
                        ALCS_disableLowPassFilter( &cx26828_interface[i], j );
                    else
                        ALCS_setLowPassFilter( &cx26828_interface[i], value, j );
                }
            }
            break;
        }

        case _IOC_NR(CX26828_SET_WORK_MODE)://Set to 16D1/E1/Mixed
        {
            
            if (copy_from_user(&work_mode, argp, sizeof(cx26828_work_mode)))
            {
                return -1;
            }

            if (workmode == work_mode.mode)
            {
                //printk("Driver Set work mode RETURN since same work_mode.mode = %d \n", work_mode.mode);
                break;
            }

            if ( !(work_mode.mode == CX26828_16D1 || work_mode.mode == CX26828_16E1 || work_mode.mode == CX26828_MIXED) )             
            {
                //printk("CX26828_SET_WORK_MODE value = %d, so IGNORE \n", value);
                break;
            }

            workmode = work_mode.mode;
            initCx26828();
            break;
        }
        
        case _IOC_NR(CX26828_SET_Multiplex_MODE)://Set to Multiplex_MODE
        {
            if (copy_from_user(&multiplex_mode, argp, sizeof(cx26828_Multiplex_mode)))
            {
                return -1;
            }

            if (Vi_Multiplex_Mode == multiplex_mode.mode)
            {
                //printk("Driver Set work mode RETURN since same value = %d \n", value);
                break;
            }

            if ( !(multiplex_mode.mode == CX26828_4D1_MODE || multiplex_mode.mode == CX26828_2D1_MODE 
                || multiplex_mode.mode == CX26828_1D1_MODE|| multiplex_mode.mode == CX26828_4CIF_MODE) )             
            {
                //printk("CX26828_SET_WORK_MODE value = %d, so IGNORE \n", value);
                break;
            }

            Vi_Multiplex_Mode = multiplex_mode.mode;
            initCx26828();
            break;
        }

        case _IOC_NR(CX26828_SET_VIDEO_NORM):
        {
           
            if (copy_from_user(&stVideoMode, argp, sizeof(cx26828_video_norm)))
            {
                return -1;
            }

            if (norm == stVideoMode.mode)
            {
                //printk("Driver Set video norm RETURN since same stVideoMode.mode = %d \n", stVideoMode.mode);
                break;
            }

            if ( !(stVideoMode.mode == CX26828_MODE_PAL || stVideoMode.mode == CX26828_MODE_NTSC) )
            {
                //printk("CX26828_SET_VIDEO_NORM value = %d, current norm = %d, so IGNORE \n", value, norm);
                break;
            }

            norm = stVideoMode.mode;
            initCx26828();
            break;
        }

        case _IOC_NR(CX26828_GET_VIDEO_NORM)://Set to PAL/NTSC
        {
            copy_to_user(argp, &norm, sizeof(norm));
            //printk("Driver Get video quality : %d\n", norm);
            break;
        }

        case _IOC_NR(CX26828_GET_VIDEO_LOSS):/*get video loss state */
        {
            cx26828_video_loss lostStatus;
            if (copy_from_user(&lostStatus, argp, sizeof(lostStatus)))
            {
                return -1;
            }

            if (lostStatus.chip < 0 || lostStatus.chip > 1)
            {
                return -1;
            }

            if (lostStatus.ch < 0 || lostStatus.ch >= MAX_DECODERS)
            {
                return -1;
            }

            ret = ALCS_getVideoLostStatus(&cx26828_interface[lostStatus.chip], lostStatus.ch, &lostStatus.is_lost);
            copy_to_user(argp, &lostStatus, sizeof(lostStatus));

            break;
        }

        case _IOC_NR(CX26828_SET_IMAGE_ADJUST)://Adjust color setting
        {
            int value;

            if (copy_from_user(&analog_cfg, argp, sizeof(analog_cfg)))
            {
                return -1;
            }
            if (analog_cfg.chip < 0 || analog_cfg.chip >= chips )
            {
                return -1;
            }
            if (analog_cfg.chn < 0 || analog_cfg.chn >= MAX_DECODERS)
            {
                return -1;
            }

            //printk("Driver Set color : chip_num = %d, decoder_num = %d\n", analog_cfg.chip, analog_cfg.chn);

            value = analog_cfg.brightness * (10000/100);
            ALCS_setBrightness(&cx26828_interface[analog_cfg.chip],value, analog_cfg.chn);
            image_color_setting[analog_cfg.chip]._brightness[analog_cfg.chn] = value;

            value = analog_cfg.contrast * (10000/100);
            ALCS_setContrast(&cx26828_interface[analog_cfg.chip], value, analog_cfg.chn);
            image_color_setting[analog_cfg.chip]._contrast[analog_cfg.chn] = value;

            value = analog_cfg.saturation * (10000/100);
            ALCS_setSaturation(&cx26828_interface[analog_cfg.chip], value, analog_cfg.chn);
            image_color_setting[analog_cfg.chip]._saturation[analog_cfg.chn] = value;

            value = analog_cfg.hue * (10000/100);
            ALCS_setHue(&cx26828_interface[analog_cfg.chip], value, analog_cfg.chn);
            image_color_setting[analog_cfg.chip]._hue[analog_cfg.chn] = value;

            value = analog_cfg.sharpness * (100/100);
            ALCS_setSharpness(&cx26828_interface[analog_cfg.chip], value, analog_cfg.chn);
            image_color_setting[analog_cfg.chip]._sharpness[analog_cfg.chn] = value;
            break;
        }

        case _IOC_NR(CX26828_GET_IMAGE_ADJUST)://Get color setting
        {
            if (copy_from_user(&analog_cfg, argp, sizeof(analog_cfg)))
            {
                return -1;
            }
            if (analog_cfg.chip < 0 || analog_cfg.chip >= chips )
            {
                return -1;
            }
            if (analog_cfg.chn < 0 || analog_cfg.chn >= MAX_DECODERS)
            {
                return -1;
            }

            //printk("Driver Get color : chip_num = %d, decoder_num = %d\n", analog_cfg.chip, analog_cfg.chn);

            analog_cfg.brightness = image_color_setting[analog_cfg.chip]._brightness[analog_cfg.chn]/100;
            analog_cfg.contrast = image_color_setting[analog_cfg.chip]._contrast[analog_cfg.chn]/100;
            analog_cfg.saturation = image_color_setting[analog_cfg.chip]._saturation[analog_cfg.chn]/100;
            analog_cfg.hue = image_color_setting[analog_cfg.chip]._hue[analog_cfg.chn]/100;
            analog_cfg.sharpness = image_color_setting[analog_cfg.chip]._sharpness[analog_cfg.chn];

            copy_to_user(argp, &analog_cfg, sizeof(analog_cfg));

            break;
        }

        case _IOC_NR(CX26828_GET_MIXED_CHANNEL_SETTING)://Get D1/E1 mixed channel map
        {
            copy_to_user(argp, mixed_video_resolution, sizeof(mixed_video_resolution));
            ret = 0;
            break;
        }

        case _IOC_NR(CX26828_SET_MIXED_CHANNEL_SETTING)://To set D1/E1 mixed channel map
        {
            if (copy_from_user(&mixed_video_resolution, argp, sizeof(mixed_video_resolution)))
            {
                return -1;
            }

            ret = 0;
            break;
        }

        case _IOC_NR(CX26828_FLUSH_MIXED_CHANNEL_SETTING)://To initialize CX26828 per mixed D1/E1 channel map
        {
            cx26828_reconfig_mixed_mode(); 
            ret = 0;
            break;
        }

        case _IOC_NR(CX26828_SET_SAMPLE_RATE):
        {
            if (copy_from_user(&audio_sample_rate, argp, sizeof(audio_sample_rate)))
            {
                return -1;
            }

            if (audio_sample_rate.chip < 0 || audio_sample_rate.chip >= chips)
            {
                return -1;
            }

            switch(audio_sample_rate.sample_rate)
            {
                case AUDIO_SAMPLING_RATE_44:
                    ret  = ALCS_setAudioADC_44K( &cx26828_interface[audio_sample_rate.chip] );
                    ret |= ALCS_setAudioDAC_44K( &cx26828_interface[audio_sample_rate.chip] );
                    break;
                case AUDIO_SAMPLING_RATE_32:
                    ret  = ALCS_setAudioADC_32K( &cx26828_interface[audio_sample_rate.chip] );
                    ret |= ALCS_setAudioDAC_32K( &cx26828_interface[audio_sample_rate.chip] );
                    break;
                case AUDIO_SAMPLING_RATE_16:
                    ret  = ALCS_setAudioADC_16K( &cx26828_interface[audio_sample_rate.chip] );
                    ret |= ALCS_setAudioDAC_16K( &cx26828_interface[audio_sample_rate.chip] );
                    break;
                case AUDIO_SAMPLING_RATE_8:
                    ret  = ALCS_setAudioADC_8K( &cx26828_interface[audio_sample_rate.chip] );
                    ret |= ALCS_setAudioDAC_8K( &cx26828_interface[audio_sample_rate.chip] );
                    break;
                case AUDIO_SAMPLING_RATE_48:
                default:
                    ret  = ALCS_setAudioADC_48K( &cx26828_interface[audio_sample_rate.chip] );
                    ret |= ALCS_setAudioDAC_48K( &cx26828_interface[audio_sample_rate.chip] );
                    break;
            }
            break;
        }

        case _IOC_NR(CX26828_SET_BIT_WIDTH):
        {
            if (copy_from_user(&audio_bit_width, argp, sizeof(audio_bit_width)))
            {
                return -1;
            }

            if (audio_bit_width.chip < 0 || audio_bit_width.chip >= chips)
            {
                return -1;
            }

            if (AUDIO_DATA_WIDTH_8BIT == audio_bit_width.bit_width)
            {
                ret = ALCS_setAudioBitWidth_8Bit( &cx26828_interface[audio_bit_width.chip] );
            }
            else if (AUDIO_DATA_WIDTH_16BIT == audio_bit_width.bit_width)
            {
                ret = ALCS_setAudioBitWidth_16Bit( &cx26828_interface[audio_bit_width.chip] );
            }
            else if (AUDIO_DATA_WIDTH_24BIT == audio_bit_width.bit_width)
            {
                ret = ALCS_setAudioBitWidth_24Bit( &cx26828_interface[audio_bit_width.chip] );
            }
            else
            {
                printk("invalid bit_width:%d\n", audio_bit_width.bit_width);
                return -1;
            }

            break;
        }

        case _IOC_NR(CX26828_SET_AUDIO_FORMAT):
        {
            if (copy_from_user(&audio_format, argp, sizeof(cx26828_audio_format)))
            {
		        return -1;
            }

            if (audio_format.chip < 0 || audio_format.chip >= chips)
            {
                return -1;
            }
            
            if (0 == audio_format.format)
            {
                ret = ALCS_setAudioFormat_I2sMode( &cx26828_interface[audio_bit_width.chip] );
            }
            else if (1 == audio_format.format)
            {
                ret = ALCS_setAudioFormat_Pcm0Mode( &cx26828_interface[audio_bit_width.chip] );
            }
            else if (2 == audio_format.format)
            {
                ret = ALCS_setAudioFormat_Pcm1Mode( &cx26828_interface[audio_bit_width.chip] );
            }
            else
            {
                printk("invalid audio_format:%d\n", audio_format.format);
                return -1;
            }

            if (0 == audio_format.master)
            {
                ret = ALCS_setAudioMode_Slave( &cx26828_interface[audio_bit_width.chip] );
            }
            else if (1 == audio_format.master)
            {
                ret = ALCS_setAudioMode_Master( &cx26828_interface[audio_bit_width.chip] );
            }
            else
            {
                printk("invalid audio_mode:%d\n", audio_format.master);
                return -1; 
            }
            
            break;
        }
        
        case _IOC_NR(CX26828_SET_CHN_NUM):
        {
            if (copy_from_user(&audio_chn_num, argp, sizeof(audio_chn_num)))
            {
                return -1;
            }

            if (audio_chn_num.chip < 0 || audio_chn_num.chip >= chips)
            {
                return -1;
            }
            
            ret = ALCS_setAudioChnNum(&cx26828_interface[audio_chn_num.chip], audio_chn_num.chn_num);
            
            break;
        }        
        
        case _IOC_NR(CX26828_SET_AUDIO_PLAYBACK):
        {
            unsigned long  master_mode;
            if (copy_from_user(&audio_play_back, argp, sizeof(audio_play_back)))
            {
                return -1;
            }

            if (audio_play_back.chip < 0 || audio_play_back.chip >= chips)
            {
                return -1;
            }

            if (audio_play_back.audio_ch > 15 || audio_play_back.audio_ch < 0)
            {
                return -1;
            }

            audio_play_back.audio_ch %= 8;


            ALCS_setupAudioADCInput( &cx26828_interface[audio_play_back.chip], audio_sample_rate.sample_rate );

            master_mode = (cx26828_interface[audio_play_back.chip].i2c_addr == cx26828_i2c_addr[1]) ? AUDIO_SLAVE_MODE : AUDIO_MASTER_MODE;

            ALCS_setupI2SInterface(&cx26828_interface[audio_play_back.chip],
                                   AUDIO_PLAYBACK,
                                   AUDIO_I2S_MODE,
                                   master_mode,
                                   AUDIO_DATA_WIDTH_16BIT,
                                   AUDIO_BIT_DELAY_1BIT,
                                   AUDIO_2_CHANNELS,
                                   audio_out_sel_map,
                                   AUDIO_UNCOMPRESSION);

            if ( audio_play_back.is_loopback == 0 )
            {
                audio_play_back.audio_ch = 0xA; //audio_pb
            }

            //If loopback is enabled, audio is internally looped from ADC channel to DAC.
            //If loopback is disabled, audio I2S comes from codec.
            ALCS_setupAudioDACOutput( &cx26828_interface[audio_play_back.chip],
                                      AUDIO_SAMPLING_RATE_48,
                                      audio_play_back.audio_ch,
                                      AUDIO_GAIN_ENABLE,
                                      AUDIO_SLAVE_MODE);

            ALCS_setDACVolume( &cx26828_interface[audio_play_back.chip], audio_dac_volume.volume );
            break;
        }
        case _IOC_NR(CX26828_SET_AUDIO_DAC_MUTE):
        {
            if (copy_from_user(&audio_dac_mute, argp, sizeof(audio_dac_mute)))
            {
                return -1;
            }

            if (audio_dac_mute.chip < 0 || audio_dac_mute.chip >= chips )
            {
                return -1;
            }

            if(audio_dac_mute.mute_enable < 0)
            {
                return -1;
            }

            if (audio_dac_mute.mute_enable)
                ALCS_setDACVolume( &cx26828_interface[audio_dac_mute.chip], 0);
            else //Set previous DAC volume
                ALCS_setDACVolume( &cx26828_interface[audio_dac_mute.chip], audio_dac_volume.volume);

            break;
        }
        case _IOC_NR(CX26828_SET_AUDIO_DAC_VOLUME):
        {
            if (copy_from_user(&audio_dac_volume, argp, sizeof(audio_dac_volume)))
            {
                return -1;
            }

            if (audio_dac_volume.chip < 0 || audio_dac_volume.chip >= chips )
            {
                return -1;
            }

            if(audio_dac_volume.volume > MAX_DAC_VOLUME || audio_dac_volume.volume < 0)
            {
                return -1;
            }

            ALCS_setDACVolume( &cx26828_interface[audio_dac_volume.chip], audio_dac_volume.volume);
            break;
        }
        case _IOC_NR(CX26828_GET_MOTION_DETECTION_STATUS):
        {
            #if 0
            for (i = 0; i < 2; i++) 
            {
                for (j = 0; j < MAX_DECODERS; j++)
                {
                    _mot_det_info[i][j].mot_num_blks_detected = 0; //clear
                    _mot_det_info[i][j].no_mot_detected = 0;
                    _mot_det_info[i][j].black_detected = 0; 
                    _mot_det_info[i][j].white_detected = 0;
                }
            }

            queryMotionDetectionInterrupt(); 
            copy_to_user(argp, &_mot_det_info, sizeof(_mot_det_info));
            #endif
            break;
        }
        case _IOC_NR(CX26828_SET_MOTION_DETECTION_PARAMETERS):
        {
            #if 0
            unsigned long mot_intr_msk = 0;
            int chip_num = 0;
            
            if (copy_from_user(&tmp_mot_det_info, argp, sizeof(tmp_mot_det_info)))
            {
                return -1;
            }

            if (tmp_mot_det_info.chip < 0 || tmp_mot_det_info.chip >= chips)
            {
                return -1;
            }

            chip_num = tmp_mot_det_info.chip;

            setMotionDetectThreshold( &tmp_mot_det_info );

            for (i = 0; i < MAX_DECODERS; i++)
            {
                ALCS_stopMotionDetect( &cx26828_interface[chip_num], i);

                if( tmp_mot_det_info.enable[i] != 0 )
                {                
                    ALCS_initializeMotionDetect( &cx26828_interface[chip_num], &_mot_det_info[chip_num][i], current_video_standard[i], i );

                    ALCS_startMotionDetect( &cx26828_interface[chip_num], &_mot_det_info[chip_num][i], i, &mot_intr_msk);
                }
            }

            
            printk("SET_MOTION_PARAMS: sens_thresh = %d, block_thresh = %d \n", tmp_mot_det_info.mot_det_sens_thresh, tmp_mot_det_info.mot_det_block_thresh);
            #endif
            break;
        }
        default:
        {
            printk("invalid cx26828 ioctl cmd %d\n", cmd);
            ret = -1;
            break;
        }
    }

    return ret;
}

static struct file_operations cx26828_fops = {
    .owner           = THIS_MODULE,
    .unlocked_ioctl  = cx26828_ioctl,
    .open            = cx26828_open,
    .release         = cx26828_close
};

static struct miscdevice cx26828_dev = {
    .minor      = MISC_DYNAMIC_MINOR,
    .name       = "cx268xx",
    .fops       = &cx26828_fops,
};

//CX26828 I2C access function. For another hardware platform, we should modify this function.
static unsigned int cx26828_write_i2c(unsigned i2c_addr, unsigned register_addr, int value, int size)
{
    gpio_i2c2_write(i2c_addr, register_addr, value, size);
    return 0;
}

//CX26828 I2C access function. For another hardware platform, we should modify this function.
static  unsigned int  cx26828_read_i2c(unsigned char i2c_addr, unsigned short register_addr, void *buffer, int num_bytes)
{
    int value = gpio_i2c2_read(i2c_addr, register_addr, num_bytes);

    switch (num_bytes)
    {
        case 1:
            {
                unsigned char register_return_value = value;
                memcpy(buffer, &register_return_value, 1);
            }
            break;
        case 2:
            {
                unsigned short register_return_value = value;
                memcpy(buffer, &register_return_value, 2);
            }
            break;
        case 4:
            {
                unsigned int register_return_value = value;
                memcpy(buffer, &register_return_value, 4);
            }
            break;
        default:
            printk("Read I2C failed, the error data length is %d\n", num_bytes);
            break;
    }

    return 0;
}

//Sleep function. For another OS platform, we should modify this function.
static void cx26828_sleep(unsigned long msec)
{
    udelay(msec*1000);
}


static int __init cx26828_module_init(void)
{
    int ret = 0, i = 0;
    
    //printk("\ncx26828_module_init\n");

    /* first check the parameters*/
    if ((norm != CX26828_MODE_PAL) && (norm != CX26828_MODE_NTSC))
    {
        printk("module param norm must be PAL(%d) or NTSC(%d)\n", CX26828_MODE_PAL ,CX26828_MODE_NTSC);
        return -1;
    }

    if (chips <= 0 || chips > 2)
    {
        printk("module param chip invalid value:%d\n", chips);
        return -1;
    }

    printk("cx26828 driver version %d.%d.%d loaded\n",
           (CX26828_VERSION_CODE >> 16) & 0xff,
           (CX26828_VERSION_CODE >>  8) & 0xff,
            CX26828_VERSION_CODE & 0xff);


    /* register misc device*/
    ret = misc_register(&cx26828_dev);
    if (ret)
    {
        printk("ERROR: could not register cx26828 devices\n");
        return ret;
    }

    for (i = 0; i < chips * MAX_DECODERS; i++)
        mixed_video_resolution[i] = i % 2;

    audio_sample_rate.sample_rate = AUDIO_SAMPLING_RATE_48;
    audio_dac_volume.volume       = MAX_DAC_VOLUME;

    /* initize each cx26828 i2c interface*/
    for (i = 0; i < chips; i++)
    {
        cx26828_interface[i].i2c_addr = cx26828_i2c_addr[i];
        cx26828_interface[i].p_handle = &cx26828_interface[i];
        cx26828_interface[i].write = cx26828_write_i2c;
        cx26828_interface[i].read = cx26828_read_i2c;
        cx26828_interface[i].sleep = cx26828_sleep;
    }

    //To reset video decoder and each port
    for (i = 0; i < chips; i++)
        ALCS_Reset(&cx26828_interface[i]);

    initCx26828();

    for (i = 0; i < chips; i++)
    {
        //To initialize CX26828 audio. 8K, 16bit.
        cx26828_audio_init(&cx26828_interface[i]);
    }

#if 0
    for (i = 0; i < chips; i++)
    {
        initializeMotionDetectionParameters( &cx26828_interface[i], i, current_video_standard );
    }

    initializeMotionDetectionDevice();
#endif

    printk("cx26828 driver init successful !\n");

    return 0;
}



static void __exit cx26828_module_exit(void)
{
#if 0
    releaseMotionDetection();
#endif

    misc_deregister(&cx26828_dev);
}

module_init(cx26828_module_init);
module_exit(cx26828_module_exit);

module_param(norm, uint, S_IRUGO);
module_param(chips, uint, S_IRUGO);
module_param(workmode, uint, S_IRUGO);


MODULE_LICENSE("GPL");

