/* tw2868.c
 *
 * Copyright (c) 2009 Hisilicon Co., Ltd.
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
 * 2010-02-12  Modify value of SHARPNESS Control Register[0x03] from 0x19 to 0x11, Hisilicon BVT MPP.
 * 2010-03-01  Modify value of Vertical Delay Register to 0x17, And add some interface, Hisilicon BVT MPP.
 * 2010-04-16  Output ranges are limited to 16~235 for Y and 16~239 for CbCr,to fit Hi3520/Hi3515 VIU. Hisilicon BVT MPP.
 * 2010-07-23  Add function for setting audio format. Hisilicon BVT MPP.
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
//#include <asm/hardware.h>
#include <asm/io.h>
#include <asm/system.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/string.h>
#include <linux/list.h>
//#include <asm/semaphore.h>
#include <asm/delay.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/poll.h>
//#include <asm/hardware.h>
#include <asm/bitops.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <linux/moduleparam.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>

#include "gpio_i2c.h"
//#include "gpio_i2c1.h"

#include "tw2868_def.h"
#include "tw2868.h"


static int norm = TW2865_PAL;

static int chips = 2;

#define TW2865A_I2C_ADDR 	0x50
//#define TW2865B_I2C_ADDR 	0x54
#define TW2865C_I2C_ADDR 	0x52
//#define TW2865D_I2C_ADDR 	0x56

#ifdef hi3515
unsigned char tw2865_i2c_addr[] = {0x50, 0x52};
#else
unsigned char tw2865_i2c_addr[] = { TW2865A_I2C_ADDR,
                                    TW2865C_I2C_ADDR,};
#endif


#define TW2865_I2C_ADDR(chip_id)    (tw2865_i2c_addr[chip_id])

#define TW2865_INVALID_CHIPID(ChnNo) ((ChnNo) >= chips)

static unsigned char tw2865_byte_write(unsigned char chip_addr,
										     unsigned char addr     ,
										     unsigned char data     )
{

#if 0

#ifndef HI_FPGA
    gpio_i2c_write(chip_addr,addr,data);
#else

    if (TW2865A_I2C_ADDR == chip_addr || TW2865B_I2C_ADDR == chip_addr)
    {
        gpio_i2c_write(chip_addr,addr,data);
    }
    else
    {
        gpio_i2c1_write(chip_addr,addr,data);
    }

#endif
#else
    gpio_i2c_write(chip_addr,addr,data);

#endif


	return 0;
}

static unsigned char tw2865_byte_read(unsigned char chip_addr, unsigned char addr)
{

#if 0
#ifndef HI_FPGA
    return gpio_i2c_read(chip_addr,addr);
#else
    if (TW2865A_I2C_ADDR == chip_addr || TW2865B_I2C_ADDR == chip_addr)
    {
        return gpio_i2c_read(chip_addr,addr);
    }
    else
    {
        return gpio_i2c1_read(chip_addr,addr);
    }
#endif

#else
    return gpio_i2c_read(chip_addr,addr);
#endif
}


static void tw2865_write_table(unsigned char chip_addr,
		unsigned char addr, unsigned char *tbl_ptr, unsigned char tbl_cnt)
{
	unsigned char i = 0;

	for(i = 0; i < tbl_cnt; i ++)
	{
		tw2865_byte_write(chip_addr, (addr + i), *(tbl_ptr + i));
	}
}


#if 0
static void tw2865_read_table(unsigned char chip_addr,
						unsigned char addr, unsigned char reg_num)
{
	unsigned char i = 0, temp = 0;

	for(i = 0; i < reg_num; i++ )
	{
		temp = tw2865_byte_read(chip_addr, addr + i);
		printk("reg 0x%02x=0x%02x,", addr + i, temp);
		if(((i + 1) % 4) == 0)
		{
			printk("\n");
		}
	}
}
#endif


int tw2865_open(struct inode * inode, struct file * file)
{
	return 0;
}

int tw2865_close(struct inode * inode, struct file * file)
{
	return 0;
}

static int tw2865_set_work_mode_1d1(unsigned chip_addr, unsigned char ch)
{
    /* Video Channel Output Control*/
    tw2865_byte_write(chip_addr, 0x40, 0x01);/*page 1*/

    /* One Channel ITU0R BT.656 */

    tw2865_byte_write(chip_addr, 0x60, 0x00);
    tw2865_byte_write(chip_addr, 0x40, 0x00);/*page 0*/

    /* Output Enable Control and Clock Output Control */
    /* CLKN O1 27M */
    tw2865_byte_write(chip_addr, 0xfa, 0x40);
    //printk("tw2865 0x%x set to 1d1 mode ok\n", chip_addr);


    return 0;
}

static int tw2865_set_work_mode_4d1(unsigned chip_addr, unsigned char ch)
{

    /* Video Channel Output Control*/

    tw2865_byte_write(chip_addr, 0x40, 0x01);/*page 1*/
    /* Four Channel ITU0R BT.656 Time-multiplexed format output*/
    tw2865_byte_write(chip_addr, 0x60, 0xaa);
    tw2865_byte_write(chip_addr, 0x40, 0x00);/*page 0*/

    /* Output Enable Control and Clock Output Control */
    /* 108MHz clock output*/
    tw2865_byte_write(chip_addr, 0xfa, 0x4a);
    //printk("tw2865 0x%x set to 4d1 mode ok\n", chip_addr);

    return 0;
}

static int tw2865_set_work_mode_2d1(unsigned chip_addr, unsigned char ch)
{
    /* Video Channel Output Control*/

    tw2865_byte_write(chip_addr, 0x40, 0x01);/*page 1*/
    /* Two Channel ITU0R BT.656 Time-multiplexed format output*/
    tw2865_byte_write(chip_addr, 0x60, 0x55);
    tw2865_byte_write(chip_addr, 0x40, 0x00);/*page 0*/

    /* Output Enable Control and Clock Output Control */
    /* 54MHz clock output*/
    tw2865_byte_write(chip_addr, 0xfa, 0x45);/* CLKNO1 pin/CLKPO1 pin */
    //tw2865_byte_write(chip_addr, 0x6a, 0x05);/* CLKNO2 pin/CLKPO2 pin */

    printk("tw2865 0x%x set to 2d1 mode ok\n", chip_addr);

    return 0;
}

static int tw2865_set_video_norm(unsigned char addr, unsigned char mode)
{
    int i;

    udelay(50);

    if (TW2865_PAL == mode)
	{
        for (i=0; i<4; i++)
        {
            tw2865_byte_write(addr, 0x0e + 0x10*i, 0x01);/* PAL (B,D,G,H,I) */
            //tw2865_byte_write(addr, 0x0e + 0x10*i, 0x07);/* Auto detection */

            tw2865_byte_write(addr, 0x07 + 0x10*i, 0x12);/* Cropping Register         , High */

            tw2865_byte_write(addr, 0x08 + 0x10*i, 0x17);/* Vertical Delay Register   , Low  */
            tw2865_byte_write(addr, 0x09 + 0x10*i, 0x20);/* Vertical Active Register  , Low  */
            tw2865_byte_write(addr, 0x0a + 0x10*i, 0x0c);/* Horizontal Delay Register , Low  */
            tw2865_byte_write(addr, 0x0b + 0x10*i, 0xd0);/* Horizontal Active Register, Low  */

            tw2865_byte_write(addr, 0x40, 0x01);/*page 1*/
            tw2865_byte_write(addr, 0x0e + 0x10*i, 0x01);/* PAL (B,D,G,H,I) */
            //tw2865_byte_write(addr, 0x0e + 0x10*i, 0x07);/* Auto detection */

            tw2865_byte_write(addr, 0x07 + 0x10*i, 0x12);/* Cropping Register         , High */

            tw2865_byte_write(addr, 0x08 + 0x10*i, 0x17);/* Vertical Delay Register   , Low  */
            tw2865_byte_write(addr, 0x09 + 0x10*i, 0x20);/* Vertical Active Register  , Low  */
            tw2865_byte_write(addr, 0x0a + 0x10*i, 0x0c);/* Horizontal Delay Register , Low  */
            tw2865_byte_write(addr, 0x0b + 0x10*i, 0xd0);/* Horizontal Active Register, Low  */
            tw2865_byte_write(addr, 0x40, 0x00);/*page 0*/
        }

		tw2865_byte_write(addr, 0x41, 0xd4); /* PAL-B -->  HZ50:1 FSCSEL:1 PHALT:1 PED:0 */

        printk("tw2865 0x%x set to PAL mode ok! \n", addr);
	}
	else if (TW2865_NTSC == mode)
	{
        for (i=0; i<4; i++)
        {
            tw2865_byte_write(addr, 0x0e + 0x10*i, 0x00);/* NTSC(M) */
            //tw2865_byte_write(addr, 0x0e + 0x10*i, 0x07);/* Auto detection */

            tw2865_byte_write(addr, 0x07 + 0x10*i, 0x02);/* Cropping Register, High */
            tw2865_byte_write(addr, 0x08 + 0x10*i, 0x15);/* Vertical Delay Register, Low  */
            tw2865_byte_write(addr, 0x09 + 0x10*i, 0xf0);/* Vertical Active Register, Low */
            tw2865_byte_write(addr, 0x0a + 0x10*i, 0x0c);/* Horizontal Delay Register, Low */
            tw2865_byte_write(addr, 0x0b + 0x10*i, 0xd0);/* Horizontal Active Register, Low */

            tw2865_byte_write(addr, 0x40, 0x01);/*page 1*/
            tw2865_byte_write(addr, 0x0e + 0x10*i, 0x00);/* NTSC(M) */
            //tw2865_byte_write(addr, 0x0e + 0x10*i, 0x07);/* Auto detection */

            tw2865_byte_write(addr, 0x07 + 0x10*i, 0x02);/* Cropping Register, High */
            tw2865_byte_write(addr, 0x08 + 0x10*i, 0x15);/* Vertical Delay Register, Low  */
            tw2865_byte_write(addr, 0x09 + 0x10*i, 0xf0);/* Vertical Active Register, Low */
            tw2865_byte_write(addr, 0x0a + 0x10*i, 0x0c);/* Horizontal Delay Register, Low */
            tw2865_byte_write(addr, 0x0b + 0x10*i, 0xd0);/* Horizontal Active Register, Low */
            tw2865_byte_write(addr, 0x40, 0x00);/*page 0*/
        }

		tw2865_byte_write(addr, 0x41, 0x40);/* NTSC-M -->  HZ50:0 FSCSEL:0 PHALT:0 PED:0 */
        printk("tw2865 0x%x set to NTSC mode ok! \n", addr);
	}
    else
    {
        printk("tw2865 not support this video norm : %d\n", addr);
        return -1;
    }
    udelay(50);
    return 0;
}

//static int tw2865_set_audio_samplerate(tw2865_audio_samplerate samplerate)
int tw2865_set_audio_samplerate(tw2865_audio_samplerate samplerate)
{
    int temp, val;

    switch (samplerate)
    {
        case TW2865_SAMPLE_RATE_8000:
            val = 0x00;
            break;
        case TW2865_SAMPLE_RATE_16000:
            val = 0x01;
            break;
        case TW2865_SAMPLE_RATE_32000:
            val = 0x02;
            break;
        case TW2865_SAMPLE_RATE_44100:
            val = 0x03;
            break;
        case TW2865_SAMPLE_RATE_48000:
            val = 0x04;
            break;
        default:
            printk("not support this samplerate:%d \n", samplerate);
            return -1;
    }

    /* NOTES:320xfs mode: AIN5MD=1,AFS384=0. 44.1kHz/48kHz are not supported */
    if (TW2865_SAMPLE_RATE_44100 == samplerate || TW2865_SAMPLE_RATE_48000 == samplerate)
    {
        printk("func(%s) line(%d):tw2865 not support this samplerate:%d \n",
            __FUNCTION__, __LINE__, samplerate);
        return -1;
    }

    /* ACKI control is automatically set up by AFMD register values [3] */
    temp = tw2865_byte_read(TW2865A_I2C_ADDR, 0x70);
    temp &= ~0x08;
    temp |= 0x1<<3;
    tw2865_byte_write(TW2865A_I2C_ADDR,  0x70, temp);

    /* AFAUTO control mode 0x70 [2:0] */
    temp = tw2865_byte_read(TW2865A_I2C_ADDR, 0x70);
    temp &= ~0x07;
    temp |= val;
    tw2865_byte_write(TW2865A_I2C_ADDR,  0x70, temp);

    if (TW2865_SAMPLE_RATE_8000 == samplerate)
    {
        tw2865_byte_write(TW2865A_I2C_ADDR, 0xE1, 0xc0);
        tw2865_byte_write(TW2865A_I2C_ADDR, 0xE2, 0xaa);
        tw2865_byte_write(TW2865A_I2C_ADDR, 0xE3, 0xaa);

        tw2865_byte_write(TW2865A_I2C_ADDR, 0x40, 0x01);/*page 1*/
        tw2865_byte_write(TW2865A_I2C_ADDR, 0xE1, 0xc0);
        tw2865_byte_write(TW2865A_I2C_ADDR, 0xE2, 0xaa);
        tw2865_byte_write(TW2865A_I2C_ADDR, 0xE3, 0xaa);
        tw2865_byte_write(TW2865A_I2C_ADDR, 0x40, 0x00);/*page 0*/
    }
    else
    {
        tw2865_byte_write(TW2865A_I2C_ADDR, 0xE1, 0xe0);
        tw2865_byte_write(TW2865A_I2C_ADDR, 0xE2, 0xbb);
        tw2865_byte_write(TW2865A_I2C_ADDR, 0xE3, 0xbb);

        tw2865_byte_write(TW2865A_I2C_ADDR, 0x40, 0x01);/*page 1*/
        tw2865_byte_write(TW2865A_I2C_ADDR, 0xE1, 0xe0);
        tw2865_byte_write(TW2865A_I2C_ADDR, 0xE2, 0xbb);
        tw2865_byte_write(TW2865A_I2C_ADDR, 0xE3, 0xbb);
        tw2865_byte_write(TW2865A_I2C_ADDR, 0x40, 0x00);/*page 0*/
    }

    //printk("tw2865 audio samplerate set to %d ok\n", val);
    return 0;
}
EXPORT_SYMBOL(tw2865_set_audio_samplerate);

//static int tw2865_set_audio_format(tw2865_audio_format audio_format)
int tw2865_set_audio_format(tw2865_audio_format audio_format)
{
    int i;
    unsigned char i2c_addr, temp;

    for (i = 0; i < chips; i ++)
	{
        i2c_addr = tw2865_i2c_addr[i];

        /* Define the digital serial audio data format for record
            and mixing audio on the ACLKR, ASYNR, ADATR and ADATM pin.*/
        temp = tw2865_byte_read(i2c_addr, 0xd2);
        temp &= 0xbf;/* [6]*/
        temp |= (0==audio_format.format) ? 0 : 0x40;/* 0:I2S format 1:DSP format */
        tw2865_byte_write(i2c_addr, 0xd2, temp);

        /* Define the digital serial audio data format for
            playback audio on the ACLKP, ASYNP and ADATP pin.*/
        temp = tw2865_byte_read(i2c_addr, 0xdb);
        temp &= 0xf7;/* [3]*/
        temp |= (0==audio_format.format) ? 0x0 : 0x8; /* 0:I2S format 1:DSP format */
        tw2865_byte_write(i2c_addr, 0xdb, temp);
    }

    return 0;
}
EXPORT_SYMBOL(tw2865_set_audio_format);

//int tw2865_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
long tw2865_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    unsigned int __user *argp = (unsigned int __user *)arg;
    int i2c_addr;
    int tmp     ;
    int ret = 0 ;
	tw2865_image_adjust    analog_cfg;
    tw2865_video_norm      video_norm;
    tw2865_work_mode       work_mode ;
    tw2865_video_loss      video_loss;
    tw2865_audio_playback  audio_playback ;
    tw2865_audio_da_volume audio_da_volume;
	switch (_IOC_NR(cmd))
	{
        case _IOC_NR(TW2865_SET_WORK_MODE):
        {
            if (copy_from_user(&work_mode, argp, sizeof(tw2865_work_mode)))
    			return -1;
            i2c_addr = TW2865_I2C_ADDR(work_mode.chip);

            if (TW2865_2D1_MODE == work_mode.mode)
            {
                ret = tw2865_set_work_mode_2d1(i2c_addr, 0);
            }
            else if (TW2865_4D1_MODE == work_mode.mode)
            {
                ret = tw2865_set_work_mode_4d1(i2c_addr, 0);
            }
            else if(TW2865_1D1_MODE == work_mode.mode)
            {
                ret = tw2865_set_work_mode_1d1(i2c_addr, 0);
            }
            else
            {
                printk("now not support this work mode:%d\n", work_mode.mode);
                return -1;
            }
            break;
        }

        case _IOC_NR(TW2865_SET_VIDEO_NORM):
        {
            if (copy_from_user(&video_norm, argp, sizeof(tw2865_video_norm)))
    			return -1;
    	    if ((video_norm.mode != TW2865_NTSC) && (video_norm.mode != TW2865_PAL))
    	    {
    			printk("invalid video mode %d \n ",video_norm.mode);
    			return -1;
    	    }
            	printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
            if (TW2865_INVALID_CHIPID(video_norm.chip))
            {
                printk("invalid chip id %d \n ",video_norm.chip);
    			return -1;
            }
            	printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
            i2c_addr = TW2865_I2C_ADDR(video_norm.chip);
    	    ret = tw2865_set_video_norm(i2c_addr, video_norm.mode);
    	    break;
        }

        case _IOC_NR(TW2865_GET_VIDEO_NORM):
        {
            if (copy_from_user(&video_norm, argp, sizeof(tw2865_video_norm)))
    			return -1;
            i2c_addr = TW2865_I2C_ADDR(video_norm.chip);

            tmp = tw2865_byte_read(i2c_addr, 0x0e);
            tmp = (tmp >> 6) & 0x7; /* [6:4]*/

            if (0==tmp || 3==tmp)
            {
                video_norm.mode = TW2865_NTSC;
            }
            else if (1==tmp || 4==tmp || 5==tmp || 6==tmp)
            {
                video_norm.mode = TW2865_PAL;
            }
            else
            {
                video_norm.mode = TW2865_INVALID_MODE;
            }
            if (copy_to_user(argp, &video_norm, sizeof(tw2865_video_norm)))
                return -1;
    	    break;
        }

        case _IOC_NR(TW2865_GET_VIDEO_LOSS):/*get video loss state */
        {
            if (copy_from_user(&video_loss, argp, sizeof(video_loss)))
    			return -1;
            if (TW2865_INVALID_CHIPID(video_loss.chip))
            {
                printk("invalid chip id %d \n ",video_loss.chip);
    			return -1;
            }
            i2c_addr = TW2865_I2C_ADDR(video_loss.chip);

            if(video_loss.ch<4)
            {
                tmp = tw2865_byte_read(i2c_addr, 0x00+video_loss.ch*0x10);
            }
            else
            {
               tw2865_byte_write(i2c_addr, 0x40, 0x01);/*page 1*/
                tmp = tw2865_byte_read(i2c_addr, 0x00+(video_loss.ch-4)*0x10);
                tw2865_byte_write(i2c_addr, 0x40, 0x00);/*page 0*/
            }

            video_loss.is_lost = (tmp >> 7) & 0x01;   /* [7] */

			if (copy_to_user(argp, &video_loss, sizeof(video_loss)))
                return -1;
			break;
		}

		case _IOC_NR(TW2865_SET_IMAGE_ADJUST):
        {
			if (copy_from_user(&analog_cfg, argp, sizeof(analog_cfg)))
			{
				return -1;
			}
			if (TW2865_INVALID_CHIPID(analog_cfg.chip))
			{
				return -1;
			}
            i2c_addr = TW2865_I2C_ADDR(analog_cfg.chip);
			if (analog_cfg.item_sel & TW2865_SET_HUE)//hue
			{
    			if(analog_cfg.chn<4)
                {
                    tw2865_byte_write(i2c_addr, 0x06+analog_cfg.chn*0x10, analog_cfg.hue);
                }
                else
                {
                   tw2865_byte_write(i2c_addr, 0x40, 0x01);/*page 1*/
                    tw2865_byte_write(i2c_addr, 0x06+(analog_cfg.chn-4)*0x10, analog_cfg.hue);
                    tw2865_byte_write(i2c_addr, 0x40, 0x00);/*page 0*/
                }

			}
			if (analog_cfg.item_sel & TW2865_SET_CONTRAST)//contrast
			{
			    if(analog_cfg.chn<4)
                {
                    tw2865_byte_write(i2c_addr, 0x02+analog_cfg.chn*0x10, analog_cfg.contrast);
                }
                else
                {
                    tw2865_byte_write(i2c_addr, 0x40, 0x01);/*page 1*/
                    tw2865_byte_write(i2c_addr, 0x02+(analog_cfg.chn-4)*0x10, analog_cfg.contrast);
                    tw2865_byte_write(i2c_addr, 0x40, 0x00);/*page 0*/
                }

			}
			if (analog_cfg.item_sel & TW2865_SET_BRIGHT)//brightness
			{
			    if(analog_cfg.chn<4)
                {
                    tw2865_byte_write(i2c_addr, 0x01+analog_cfg.chn*0x10, analog_cfg.brightness);
                }
                else
                {
                    tw2865_byte_write(i2c_addr, 0x40, 0x01);/*page 1*/
                    tw2865_byte_write(i2c_addr, 0x01+(analog_cfg.chn-4)*0x10, analog_cfg.brightness);
                    tw2865_byte_write(i2c_addr, 0x40, 0x00);/*page 0*/
                }


			}
			if (analog_cfg.item_sel & TW2865_SET_SATURATION)//saturation
			{
			    if(analog_cfg.chn<4)
                {
                    tw2865_byte_write(i2c_addr, 0x04+analog_cfg.chn*0x10, analog_cfg.saturation);
                    tw2865_byte_write(i2c_addr, 0x05+analog_cfg.chn*0x10, analog_cfg.saturation);
                }
                else
                {
                    tw2865_byte_write(i2c_addr, 0x40, 0x01);/*page 1*/
                    tw2865_byte_write(i2c_addr, 0x04+(analog_cfg.chn-4)*0x10, analog_cfg.saturation);
                    tw2865_byte_write(i2c_addr, 0x05+(analog_cfg.chn-4)*0x10, analog_cfg.saturation);
                    tw2865_byte_write(i2c_addr, 0x40, 0x00);/*page 0*/
                }

			}
			break;
		}

		case _IOC_NR(TW2865_GET_IMAGE_ADJUST):
        {
			if (copy_from_user(&analog_cfg, argp, sizeof(analog_cfg)))
				return -1;
			if(TW2865_INVALID_CHIPID(analog_cfg.chip))
			{
				return -1;
			}
            i2c_addr = TW2865_I2C_ADDR(analog_cfg.chip);
			if(analog_cfg.item_sel & TW2865_SET_HUE)//hue
			{
				analog_cfg.hue = tw2865_byte_read(i2c_addr, 0x06+analog_cfg.chn*0x10);
			}
			if(analog_cfg.item_sel & TW2865_SET_CONTRAST)//contrast
			{
				analog_cfg.contrast = tw2865_byte_read(i2c_addr, 0x02+analog_cfg.chn*0x10);
			}
			if(analog_cfg.item_sel & TW2865_SET_BRIGHT)//brightness
			{
				analog_cfg.brightness = tw2865_byte_read(i2c_addr, 0x01+analog_cfg.chn*0x10);
			}
			if(analog_cfg.item_sel & TW2865_SET_SATURATION)//saturation
			{
				analog_cfg.saturation = tw2865_byte_read(i2c_addr, 0x04+analog_cfg.chn*0x10);
			}
			if (copy_to_user(argp, &analog_cfg, sizeof(analog_cfg)))
                return -1;
			break;
		}
        case _IOC_NR(TW2865_SET_SAMPLE_RATE):
        {
            tw2865_audio_samplerate samplerate;

            if (copy_from_user(&samplerate, argp, sizeof(samplerate)))
		        return -1;
            ret = tw2865_set_audio_samplerate(samplerate);
            break;
        }

        case _IOC_NR(TW2865_SET_AUDIO_PLAYBACK):
        {
            if (copy_from_user(&audio_playback, argp, sizeof(tw2865_audio_playback)))
		        return -1;
		    if(TW2865_INVALID_CHIPID(audio_playback.chip))
				return -1;
            if(audio_playback.chn > 24 || audio_playback.chn < 0)
			    return -1;

            i2c_addr = TW2865_I2C_ADDR(audio_playback.chip);
            tmp = tw2865_byte_read(i2c_addr, 0xe0);
            tmp &= 0xe0;
            tmp |= audio_playback.chn;
            tw2865_byte_write(i2c_addr, 0xe0, tmp);

            break;
        }
        case _IOC_NR(TW2865_SET_AUDIO_DA_VOLUME):
        {
            if (copy_from_user(&audio_da_volume, argp, sizeof(tw2865_audio_da_volume)))
		        return -1;
		    if(TW2865_INVALID_CHIPID(audio_da_volume.chip))
				return -1;
			if(audio_da_volume.volume > 15 || audio_da_volume.volume < 0)
			    return -1;

			i2c_addr = TW2865_I2C_ADDR(audio_da_volume.chip);
            tmp = tw2865_byte_read(i2c_addr, 0xdf);
            tmp &= 0x0f;
            tmp |= (audio_da_volume.volume << 4);
            tw2865_byte_write(i2c_addr, 0xdf, tmp);

            break;
        }
        case _IOC_NR(TW2865_SET_AUDIO_FORMAT):
        {
            tw2865_audio_format audio_format;
            if (copy_from_user(&audio_format, argp, sizeof(tw2865_audio_format)))
		        return -1;
            if (audio_format.format != 0 && audio_format.format != 1)
            {
                printk("invalid audio_format:%d\n", audio_format.format);
                return -1;
            }
            tw2865_set_audio_format(audio_format);
            break;
        }
		default:
        {
            printk("invalid tw2865 ioctl cmd\n");
            ret = -1;
			break;
		}
	}

	return ret;
}

static void tw2865_audio_init(unsigned char chip_addr)
{
    /* Analog Power Down Control */
    //tw2865_byte_write(chip_addr, 0xCE, 0x40 );

    /* 0xD0, 0xD1, 0x7F - Analog Audio Input Gain */
	tw2865_byte_write(chip_addr, 0x7f, 0x80);/* [7:4] AIGAIN51 [3:0] MIXRATIO51 */
    tw2865_byte_write(chip_addr, 0xD0, 0x88);/* [7:4] AIGAIN2 [3:0] AIGAIN1 */
    tw2865_byte_write(chip_addr, 0xD1, 0x88);/* [7:4] AIGAIN4 [3:0] AIGAIN3 */

    tw2865_byte_write(chip_addr, 0x40, 0x01);/*page 1*/
    tw2865_byte_write(chip_addr, 0x7f, 0x80);/* [7:4] AIGAIN52 [3:0] MIXRATIO52 */
    tw2865_byte_write(chip_addr, 0xD0, 0x88);/* [7:4] AIGAIN6 [3:0] AIGAIN5 */
    tw2865_byte_write(chip_addr, 0xD1, 0x88);/* [7:4] AIGAIN8 [3:0] AIGAIN7 */
    tw2865_byte_write(chip_addr, 0x40, 0x00);/*page 0*/

    /* Number of Audio to be Recorded */
    tw2865_byte_write(chip_addr, 0xD2, 0x02);/* recode: I2S, 8 chn */

    /* 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA ¨C Sequence of Audio to be Recorded*/
    tw2865_byte_write(chip_addr, 0xD3, 0x10);/* default value: 1 0 */
    tw2865_byte_write(chip_addr, 0xD4, 0x32);/* default value: 3 2 */
    tw2865_byte_write(chip_addr, 0xD7, 0x54);/* default value: 9 8 */
    tw2865_byte_write(chip_addr, 0xD8, 0x76);/* default value: B A */

    /* playback: I2S, master, 16bit, ACLKR pin is output */
    tw2865_byte_write(chip_addr, 0xDB, 0xE1);

    /* u-Law/A-Law Output and Mix Mute Control */
    tw2865_byte_write(chip_addr, 0xDC, 0x00);/* PCM output */

    /* Mix Output Selection */
    tw2865_byte_write(chip_addr, 0xE0, 0x10);/* Select playback audio of the first stage chips*/

    /* Audio Detection Threshold, 8K typical setting value */
    tw2865_byte_write(chip_addr, 0xE1, 0xc0);
    tw2865_byte_write(chip_addr, 0xE2, 0xaa);
    tw2865_byte_write(chip_addr, 0xE3, 0xaa);

    tw2865_byte_write(chip_addr, 0x40, 0x01);/*page 1*/
    tw2865_byte_write(chip_addr, 0xE1, 0xc0);
    tw2865_byte_write(chip_addr, 0xE2, 0xaa);
    tw2865_byte_write(chip_addr, 0xE3, 0xaa);
    tw2865_byte_write(chip_addr, 0x40, 0x00);/*page 0*/

    /* Audio Clock Increment, ACKI[21:0]: 09B583h for fs = 8kHz is default. */
    tw2865_byte_write(chip_addr, 0xF0, 0x83);/* ACKI[7:0] */
    tw2865_byte_write(chip_addr, 0xF1, 0xb5);/* ACKI[15:8] */
    tw2865_byte_write(chip_addr, 0xF2, 0x09);/* ACKI[21:16] */

    /* [3] ACKI control is automatically set up by AFMD register values */
    /* [2:0] AFAUTO control mode. 0: 8kHz setting(default). */
	tw2865_byte_write(chip_addr, 0x70, 0x08);/* [2:0] 0:8k, 1:16k, 2:32k, 3:44.1k 4:48k */

    /* Audio Clock Control */
    tw2865_byte_write(chip_addr, 0xF8, 0xc4);/* bit2: Loop open in ACKG block */

    /* Enable state register updating and interrupt request of audio AIN5 detection for each input*/
	tw2865_byte_write(chip_addr, 0x73, 0x01);

    /* ADATM I2S Output Select (default value)*/
    tw2865_byte_write(chip_addr, 0x7B, 0x15);/* Select record audio of channel 51(AIN51)*/
    tw2865_byte_write(chip_addr, 0x7C, 0x15);/* Select record audio of channel 51(AIN51)*/
    /* MIX_MUTE_A5 ?????? */
    tw2865_byte_write(chip_addr, 0x7E, 0xc0);
    /* Audio Fs Mode Control */
    tw2865_byte_write(chip_addr, 0x89, 0x05);/* AIN5MD=1, AFS384=0 */
}


static void tw2865_audio_cascade(int chip_num)
{
    int i;
    unsigned char temp;

    /* cascade */
    for (i=0; i<chip_num; i++)
    {
        tw2865_byte_write(tw2865_i2c_addr[i], 0x82, 0x00);
    }

    for (i=0; i<chip_num; i++)
    {
        /* the number of audio for record on the ADATR pin */
        temp = tw2865_byte_read(tw2865_i2c_addr[i], 0xd2);
        temp &= 0xfc;/* [1:0]*/
        temp |= 0x3 ;/* 0x3 16chn */
        tw2865_byte_write(tw2865_i2c_addr[i], 0xd2, temp);

        /* SMD = b'01 */
        temp  = tw2865_byte_read(tw2865_i2c_addr[i], 0xcf);
        temp &= 0x3f;/* [7:6]*/
        temp |= 0x80;/* 0x10 */ /* not 0x01*/
        tw2865_byte_write(tw2865_i2c_addr[i], 0xcf, temp);

        /* set PB_MASTER ???? */
        temp = tw2865_byte_read(tw2865_i2c_addr[i], 0xdb);
        temp &= 0xfc;
        temp |= 0x01;
        tw2865_byte_write(tw2865_i2c_addr[i], 0xdb, temp);
    }

    for (i=0; i<chip_num; i++)
    {
        /* Sequence of Audio to be Recorded */
        tw2865_byte_write(tw2865_i2c_addr[i], 0xD3, 0x10);
        tw2865_byte_write(tw2865_i2c_addr[i], 0xD4, 0x32);
        tw2865_byte_write(tw2865_i2c_addr[i], 0xD5, 0x54);
        tw2865_byte_write(tw2865_i2c_addr[i], 0xD6, 0x76);
        tw2865_byte_write(tw2865_i2c_addr[i], 0xD7, 0x98);
        tw2865_byte_write(tw2865_i2c_addr[i], 0xD8, 0xBA);
        tw2865_byte_write(tw2865_i2c_addr[i], 0xD9, 0xDC);
        tw2865_byte_write(tw2865_i2c_addr[i], 0xDA, 0xFE);

        /*the amplifier gain for analog audio output, default 0x80*/
        tw2865_byte_write(tw2865_i2c_addr[i], 0xDF, 0x80);

        /* Mix Ratio Value */
        //tw2865_byte_write(tw2865_i2c_addr[i], 0xDD, 0x33);
        //tw2865_byte_write(tw2865_i2c_addr[i], 0xDE, 0x33);
        //tw2865_byte_write(tw2865_i2c_addr[i], 0xDF, 0x33);
    }
}

static void tw2865_video_init(unsigned char addr, unsigned char mode)
{
    int i;

    for (i=0; i<4; i++)
    {
//        tw2865_byte_write(addr, 0x01 + 0x10*i, 0x00);/* brightness.             */
//        tw2865_byte_write(addr, 0x02 + 0x10*i, 0x64);/* luminance contrast gain */
//        tw2865_byte_write(addr, 0x03 + 0x10*i, 0x11);/* sharpness  */
//        tw2865_byte_write(addr, 0x04 + 0x10*i, 0x80);/* Chroma (U) */
//        tw2865_byte_write(addr, 0x05 + 0x10*i, 0x80);/* Chroma (V) */
//        tw2865_byte_write(addr, 0x06 + 0x10*i, 0x00);/* hue        */

        #if 0
        tw2865_byte_write(addr, 0x10 * i + 0x1, 0x8);
        tw2865_byte_write(addr, 0x10 * i + 0x2, 0x50);
        tw2865_byte_write(addr, 0x10 * i + 0x3, 0xb9);
        tw2865_byte_write(addr, 0x10 * i + 0x4, 0x70);
        tw2865_byte_write(addr, 0x10 * i + 0x5, 0x70);
        tw2865_byte_write(addr, 0x10 * i + 0x6, 0x0);

        #else
        tw2865_byte_write(addr, 0x10 * i + 0x1, 0xf8);
        tw2865_byte_write(addr, 0x10 * i + 0x2, 0x64);
        tw2865_byte_write(addr, 0x10 * i + 0x3, 0x11);
        tw2865_byte_write(addr, 0x10 * i + 0x4, 0x80);
        tw2865_byte_write(addr, 0x10 * i + 0x5, 0x80);
        tw2865_byte_write(addr, 0x10 * i + 0x6, 0x00);

        tw2865_byte_write(addr, 0x40, 0x01);/*page 1*/
        tw2865_byte_write(addr, 0x10 * i + 0x1, 0xf8);
        tw2865_byte_write(addr, 0x10 * i + 0x2, 0x64);
        tw2865_byte_write(addr, 0x10 * i + 0x3, 0x11);
        tw2865_byte_write(addr, 0x10 * i + 0x4, 0x80);
        tw2865_byte_write(addr, 0x10 * i + 0x5, 0x80);
        tw2865_byte_write(addr, 0x10 * i + 0x6, 0x00);
        tw2865_byte_write(addr, 0x40, 0x00);/*page 0*/
        #endif
    }

    tw2865_set_video_norm(addr, mode);

    tw2865_byte_write(addr, 0x43, 0x08);/* YDEL = 8h */

	/* CLKPO,CLKPN, 4*/
    /* Output Enable Control and Clock Output Control */
	tw2865_byte_write(addr, 0xfa, 0x40);/* CLKN O1 27M */
//	tw2865_byte_write(addr, 0xfa, 0x4a);/* CLKN O1 108M */

	/* CHID*/
	tw2865_byte_write(addr, 0x9e, 0x52);

	/* Video Channel Output Control */
	tw2865_byte_write(addr, 0xca, 0x0);/* One Channel ITU0R BT.656 */
//	tw2865_byte_write(addr, 0xca, 0xaa);/* Four Channel ITU0R BT.656 Time-multiplexed */

    /* Video AGC Control */
    tw2865_byte_write(addr, 0xaa, 0x02);/* */

    tw2865_byte_write(addr, 0x40, 0x01);/*page 1*/
    tw2865_byte_write(addr, 0xaa, 0x02);/* */
    tw2865_byte_write(addr, 0x40, 0x00);/*page 0*/

    /* Enable Video and Audio Detection*/
    tw2865_byte_write(addr, 0x73, 0x01);

    /* Video Miscellaneous Function Control*/
    tw2865_byte_write(addr, 0xf9, 0x51);/* limit output ranges */

    /* 0xD0, 0xD1, 0x7F - Analog Audio Input Gain */
    tw2865_byte_write(addr, 0x7f, 0x80);

    /* Miscellaneous Control II */
    tw2865_byte_write(addr, 0x96, 0xe6);/* Blue color, else set to 0xe0 */

    /*2011.8.10 l00181524, Modify 0xaf value (0x22 -> 0x0), to close vertical peaking*/
    tw2865_byte_write(addr, 0xaf, 0x0);
    tw2865_byte_write(addr, 0xb0, 0x0);

    tw2865_byte_write(addr, 0x40, 0x01);/*page 1*/
    tw2865_byte_write(addr, 0xaf, 0x0);
    tw2865_byte_write(addr, 0xb0, 0x0);
    tw2865_byte_write(addr, 0x40, 0x00);/*page 0*/

    /* Enable video encoder  */
    tw2865_byte_write(addr, 0x41, 0xd4);
    tw2865_byte_write(addr, 0x43, 0x00);
    tw2865_byte_write(addr, 0x4b, 0x00);

    /* For Hi3515 Demo board 2D1 mode */
    //tw2865_byte_write(addr, 0xcc, 0x0d);
    //tw2865_byte_write(addr, 0xcd, 0x00);

    tw2865_byte_write(addr, 0x40, 0x01);/*page 1*/
    tw2865_byte_write(addr, 0x67, 0x10);/*VD4 output select-VIN1~VIN4*/
    tw2865_byte_write(addr, 0x68, 0x32);
    tw2865_byte_write(addr, 0x65, 0x54);/*VD3 output select-VIN5~VIN8*/
    tw2865_byte_write(addr, 0x66, 0x76);

    tw2865_byte_write(addr, 0x40, 0x00);/*page 0*/
    tw2865_byte_write(addr, 0xfb, 0x8f);


#if 1   /* ??????? */
	if (mode == TW2865_PAL)
	{
		tw2865_write_table(addr, 0xf0, tbl_pal_tw2865_8KHz, 6);
	}
	else
	{
		tw2865_write_table(addr, 0xf0, tbl_ntsc_tw2865_8KHz, 6);
	}
#endif
}


static void tw2865_comm_init(unsigned char addr)
{
    unsigned char id = 0;

    id = tw2865_byte_read(addr, 0xff);
    if (0xff == id || 0x00 == id)
    {
        printk("warning: tw2865 0x%x i2c_read err, val:0x%x!!!\n", addr, id);
        return;
    }

    /* Software Reset*/
    tw2865_byte_write(addr, 0x80, 0x3f);

    /* Clock PLL Control */
    /* 2x XTI input frequency.54MHz clock input mode */
    tw2865_byte_write(addr, 0x60, 0x05);

    /* 108MHz Clock Select*/
    /* Analog Clock PLL 108MHz output is used for all  system clock source*/
    tw2865_byte_write(addr, 0x61, 0x03);

    /* Enable Video and Audio Detection*/
    tw2865_byte_write(addr, 0xFC, 0xFF);

    tw2865_byte_write(addr, 0x40, 0x01);/*page 1*/
    tw2865_byte_write(addr, 0xFC, 0xFF);
    tw2865_byte_write(addr, 0x40, 0x00);/*page 0*/
}


static struct file_operations tw2865_fops = {
	.owner      = THIS_MODULE,
    //.ioctl      = tw2865_ioctl,
    .unlocked_ioctl  = tw2865_ioctl,
	.open       = tw2865_open,
	.release    = tw2865_close
};

static struct miscdevice tw2865_dev = {
	.minor		= MISC_DYNAMIC_MINOR,
	.name		= "tw2868dev",
	.fops  		= &tw2865_fops,
};


module_param(norm, uint, S_IRUGO);
module_param(chips, uint, S_IRUGO);


static int __init tw2865_module_init(void)
{
	int ret = 0, i = 0;
    unsigned char i2c_addr;

	/* first check the parameters*/
	if ((norm != TW2865_PAL) && (norm != TW2865_NTSC))
	{
		printk("module param norm must be PAL(%d) or NTSC(%d)\n", TW2865_PAL ,TW2865_NTSC);
		return -1;
	}
    if (chips <= 0 || chips > 4)
    {
        printk("module param chip invalid value:%d\n", chips);
		return -1;
    }

	/* register misc device*/
	ret = misc_register(&tw2865_dev);
	if (ret)
	{
		printk("ERROR: could not register tw2865 devices\n");
		return ret;
	}

	/* initize each tw2865*/
	for (i = 0; i < chips; i ++)
	{
        i2c_addr = tw2865_i2c_addr[i];

		tw2865_comm_init(i2c_addr);

        tw2865_video_init(i2c_addr, norm);

        tw2865_audio_init(i2c_addr);
	}

    /* audio cascade */
    tw2865_audio_cascade(chips);

	printk("tw2865 driver init successful!\n");

	return 0;
}



static void __exit tw2865_module_exit(void)
{
	misc_deregister(&tw2865_dev);
}

module_init(tw2865_module_init);
module_exit(tw2865_module_exit);

#ifdef MODULE
//#include <linux/compile.h>
#endif
MODULE_LICENSE("GPL");
