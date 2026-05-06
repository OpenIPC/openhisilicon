/********************************************************************************
*
*  Copyright (C) 2017 	NEXTCHIP Inc. All rights reserved.
*  Module		: install driver main
*  Description	: driver main
*  Author		:
*  Date         :
*  Version		: Version 2.0
*
********************************************************************************
*  History      :
*
*
********************************************************************************/
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/mm.h>
#include <linux/proc_fs.h>

#ifndef CONFIG_HISI_SNAPSHOT_BOOT
#include <linux/miscdevice.h>
#endif

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
#include <linux/semaphore.h>
#include <linux/kthread.h>

#ifdef I2C_INTERNAL
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#endif

//#include "gpio_i2c.h"
#include "video.h"
#include "coax.h"
#include "motion.h"
#include "common.h"
#include "audio.h"
#include "acp.h"
#include "acp_firmup.h"
#include "eq.h"

#define AF_CNT	1

extern nvp6134_equalizer s_eq;	

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
#include "himedia.h"
#define DEV_NAME "nvp6134"
#endif

#ifdef I2C_INTERNAL
static struct i2c_board_info hi_info =
{
    I2C_BOARD_INFO("nvp6134", 0x60),
};

static struct i2c_client* nvp6134_client;
#endif


#ifdef CONFIG_HISI_SNAPSHOT_BOOT
static struct himedia_device s_stNvp6134Device;
#endif

#define DRIVER_VER "17.03.03.01"

#ifdef HI_I2C   
unsigned char __I2CReadByte8(unsigned char devaddress, unsigned char address)
{
    return (HI_I2C_Read(devaddress, address, 1, 1) & 0xff);
}

void __I2CWriteByte8(unsigned char devaddress, unsigned char address, unsigned char data)
{
    HI_I2C_Write(devaddress, address, 1, data, 1);
	udelay(200);
}
#endif

#ifdef I2C_INTERNAL
void __I2CWriteByte8(unsigned char chip_addr, unsigned char reg_addr, unsigned char value)
{
    int ret;
    unsigned char buf[2];
    struct i2c_client* client = nvp6134_client;
    
    nvp6134_client->addr = chip_addr;

    buf[0] = reg_addr;
    buf[1] = value;

    ret = i2c_master_send(client, buf, 2);
    udelay(300);
    //return ret;
}

unsigned char __I2CReadByte8(unsigned char chip_addr, unsigned char reg_addr)
{
    int ret_data = 0xFF;
    int ret;
    struct i2c_client* client = nvp6134_client;
    unsigned char buf[2];

    nvp6134_client->addr = chip_addr;

    buf[0] = reg_addr;
    ret = i2c_master_recv(client, buf, 1);
    if (ret >= 0)
    {
        ret_data = buf[0];
    }
    return ret_data;
}
#endif

#if (defined HI_CHIP_HI3521A)||(defined HI_CHIP_HI3531A)
#define HI_CHIPID_BASE 0x12050000
#else
#define HI_CHIPID_BASE 0x20050000
#endif
#define HI_CHIPID0 IO_ADDRESS(HI_CHIPID_BASE + 0xEEC)
#define HI_CHIPID1 IO_ADDRESS(HI_CHIPID_BASE + 0xEE8)
#define HI_CHIPID2 IO_ADDRESS(HI_CHIPID_BASE + 0xEE4)
#define HI_CHIPID3 IO_ADDRESS(HI_CHIPID_BASE + 0xEE0)
#define HW_REG(reg)         *((volatile unsigned int *)(reg))

int g_soc_chiptype=0x3521;
int chip_id[4];
int rev_id[4];
unsigned int nvp6134_mode = PAL;  //0:ntsc, 1: pal
unsigned int nvp6134_cnt = 0;
unsigned int nvp6134_iic_addr[4] = {0x60, 0x62, 0x64, 0x66};
module_param(nvp6134_mode, uint, S_IRUGO);
struct semaphore nvp6134_lock;
unsigned char det_mode[16];
extern unsigned char ch_mode_status[16];
extern unsigned char ch_vfmt_status[16];
extern unsigned char acp_isp_wr_en[16];

extern void nvp6134_datareverse(unsigned char chip, unsigned char port);
static struct task_struct *nvp6134_kt = NULL;

/*******************************************************************************
*	Description		: Get rev ID
*	Argurments		: dec(slave address)
*	Return value	: rev ID
*	Modify			:
*	warning			:
*******************************************************************************/
int check_rev(unsigned int dec)
{
	int ret;
	gpio_i2c_write(dec, 0xFF, 0x00);
	ret = gpio_i2c_read(dec, 0xf5);
	return ret;
}

/*******************************************************************************
*	Description		: Get Device ID
*	Argurments		: dec(slave address)
*	Return value	: Device ID
*	Modify			:
*	warning			:
*******************************************************************************/
int check_id(unsigned int dec)
{
	int ret;
	gpio_i2c_write(dec, 0xFF, 0x00);
	ret = gpio_i2c_read(dec, 0xf4);
	return ret;
}


int nvp6134_open(struct inode * inode, struct file * file)
{
	return 0;
} 

int nvp6134_close(struct inode * inode, struct file * file)
{
	return 0;
}

int get_hichipid(void)
{
	g_soc_chiptype = (HW_REG(HI_CHIPID0)&0xFF)<<8 | (HW_REG(HI_CHIPID1)&0xFF);
	printk("g_soc_chiptype ==> %x\n", g_soc_chiptype);
	return g_soc_chiptype;
}

unsigned char g_coax_ch;
unsigned int g_vloss=0xFFFF;
extern nvp6134_equalizer s_eq;
extern nvp6134_equalizer s_eq_type;

long nvp6134_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	unsigned int __user *argp = (unsigned int __user *)arg;	
	unsigned int on;
	unsigned char i;
	unsigned long ptz_ch;
	nvp6134_opt_mode optmode;
	unsigned int sens[16];
	nvp6134_video_mode vmode;
	nvp6134_chn_mode schnmode;
	nvp6134_video_adjust v_adj;
	nvp6134_input_videofmt vfmt;
	nvp6134_input_videofmt_ch vfmt_ch;
	nvp6134_acp_rw_data ispdata;
	nvp6134_acp_rw_data_extention acpdata;
	nvp6124_i2c_mode i2c;
	FIRMWARE_UP_FILE_INFO sfirmup_info;
	int ret;



	/* you must skip other command to improve speed of f/w update
	 * when you are updating cam's f/w up. we need to review and test */
	if( acp_dvr_checkFWUpdateStatus( cmd ) == -1 )
	{
		//printk(">>>>> DRV[%s:%d] Now cam f/w update mode. so Skip other command.\n", __func__, __LINE__ );
		return 0;
	}

	switch (cmd)
	{
		/***************************************************************
		* ACP firmware update - by Andy(2016-06-26)
		***************************************************************/
		case IOC_VDEC_ACP_POSSIBLE_FIRMUP:
			if (copy_from_user(&sfirmup_info, argp, sizeof(FIRMWARE_UP_FILE_INFO)))
				return -1;
			down(&nvp6134_lock);
			ret = acp_dvr_ispossible_update( &sfirmup_info );
			sfirmup_info.result = ret;
			up(&nvp6134_lock);
			if(copy_to_user(argp, &sfirmup_info, sizeof(FIRMWARE_UP_FILE_INFO)))
				printk("IOC_VDEC_ACP_POSSIBLE_FIRMUP error\n");
		break;
		case IOC_VDEC_ACP_FIRMUP:
			if (copy_from_user(&sfirmup_info, argp, sizeof(FIRMWARE_UP_FILE_INFO)))
				return -1;
			down(&nvp6134_lock);
			ret = acp_dvr_firmware_update( &sfirmup_info );
			sfirmup_info.result = ret;
			up(&nvp6134_lock);
			if(copy_to_user(argp, &sfirmup_info, sizeof(FIRMWARE_UP_FILE_INFO)))
				printk("IOC_VDEC_ACP_FIRMUP error\n");
		break;
		case IOC_VDEC_ACP_CHECK_ISPSTATUS:
			if (copy_from_user(&sfirmup_info, argp, sizeof(FIRMWARE_UP_FILE_INFO)))
				return -1;
			down(&nvp6134_lock);
			ret = acp_dvr_check_ispstatus( &sfirmup_info );
			sfirmup_info.result = ret;
			up(&nvp6134_lock);
			if(copy_to_user(argp, &sfirmup_info, sizeof(FIRMWARE_UP_FILE_INFO)))
				printk("IOC_VDEC_ACP_CHECK_ISPSTATUS error\n");
		break;
		case IOC_VDEC_ACP_START_FIRMUP:
			if (copy_from_user(&sfirmup_info, argp, sizeof(FIRMWARE_UP_FILE_INFO)))
				return -1;
			down(&nvp6134_lock);
			ret = acp_dvr_start_command( &sfirmup_info );
			sfirmup_info.result = ret;
			up(&nvp6134_lock);
			if(copy_to_user(argp, &sfirmup_info, sizeof(FIRMWARE_UP_FILE_INFO)))
				printk("IOC_VDEC_ACP_START_FIRMUP error\n");
		break;
		case IOC_VDEC_ACP_FIRMUP_END:
			if (copy_from_user(&sfirmup_info, argp, sizeof(FIRMWARE_UP_FILE_INFO)))
				return -1;
			down(&nvp6134_lock);
			ret = acp_dvr_end_command( sfirmup_info.result, &sfirmup_info );
			sfirmup_info.result = ret;
			up(&nvp6134_lock);
			if(copy_to_user(argp, &sfirmup_info, sizeof(FIRMWARE_UP_FILE_INFO)))
				printk("IOC_VDEC_ACP_FIRMUP_END error\n");
		break;
		/***************************************************************
		* End updating firmware using ACP(TX<->RX)
		***************************************************************/

		case IOC_VDEC_SET_I2C : // nextchip demoboard test
			if (copy_from_user(&i2c, argp, sizeof(nvp6124_i2c_mode)))
				return -1;
			down(&nvp6134_lock);

			if(i2c.flag == 0) // read
			{
				gpio_i2c_write(i2c.slaveaddr, 0xFF, i2c.bank);
				i2c.data = gpio_i2c_read(i2c.slaveaddr, i2c.address);
			}
			else //write
			{
				gpio_i2c_write(i2c.slaveaddr, 0xFF, i2c.bank);
				gpio_i2c_write(i2c.slaveaddr, i2c.address, i2c.data);
			}
			up(&nvp6134_lock);
			if(copy_to_user(argp, &i2c, sizeof(nvp6124_i2c_mode)))
				printk("IOC_VDEC_I2C error\n");
		break;
		case IOC_VDEC_GET_VIDEO_LOSS: // Not use
			down(&nvp6134_lock);	
			//g_vloss = nvp6134_getvideoloss();
			up(&nvp6134_lock);
			if(copy_to_user(argp, &g_vloss, sizeof(unsigned int)))
				printk("IOC_VDEC_GET_VIDEO_LOSS error\n");
			break;
		case IOC_VDEC_SET_EQUALIZER:
			if (copy_from_user(&s_eq_type, argp, sizeof(nvp6134_equalizer)))
				return -1;
			down(&nvp6134_lock);
			for(i=0;i<nvp6134_cnt*4;i++)
			{
				nvp6134_set_equalizer(i);
			}
			if(copy_to_user(argp, &s_eq, sizeof(nvp6134_equalizer)))
				printk("IOC_VDEC_SET_EQUALIZER error\n");
			up(&nvp6134_lock);
			break;
		case IOC_VDEC_GET_DRIVERVER:			
			if(copy_to_user(argp, &DRIVER_VER, sizeof(DRIVER_VER)))
				printk("IOC_VDEC_GET_DRIVERVER error\n");
			break;
		case IOC_VDEC_ACP_WRITE:
			if (copy_from_user(&ispdata, argp, sizeof(nvp6134_acp_rw_data)))
				return -1;
			down(&nvp6134_lock);			
			if(ispdata.opt == 0)
				acp_isp_write(ispdata.ch, ispdata.addr, ispdata.data);
			else
			{
				ispdata.data = acp_isp_read(ispdata.ch, ispdata.addr);
				if(copy_to_user(argp, &ispdata, sizeof(nvp6134_acp_rw_data)))
					printk("IOC_VDEC_ACP_WRITE error\n");	
			}
			up(&nvp6134_lock);
			break;
		case IOC_VDEC_ACP_WRITE_EXTENTION:
			if (copy_from_user(&acpdata, argp, sizeof(nvp6134_acp_rw_data_extention)))
				return -1;
			down(&nvp6134_lock);
			acp_isp_write_extention( acpdata.ch, &acpdata );
			up(&nvp6134_lock);
			break;
		case IOC_VDEC_PTZ_ACP_READ:
			if (copy_from_user(&vfmt, argp, sizeof(nvp6134_input_videofmt)))
				return -1;
			down(&nvp6134_lock);
			for(i=0;i<(4*nvp6134_cnt);i++)
			{
				if(1)
				{
					/* read A-CP */
					if(((g_vloss>>i)&0x01) == 0x00)
						acp_read(&vfmt, i);
				}
			}							
			up(&nvp6134_lock);
			if(copy_to_user(argp, &vfmt, sizeof(nvp6134_input_videofmt)))
				printk("IOC_VDEC_PTZ_ACP_READ error\n");			
			break;	
		case IOC_VDEC_PTZ_ACP_READ_EACH_CH:
			if (copy_from_user(&vfmt_ch, argp, sizeof(nvp6134_input_videofmt_ch)))
				return -1;
			down(&nvp6134_lock);
			/* read A-CP */
			if(((g_vloss>>vfmt_ch.ch)&0x01) == 0x00)
			{
				acp_read(&vfmt_ch.vfmt, vfmt_ch.ch);
			}

			up(&nvp6134_lock);
			if(copy_to_user(argp, &vfmt_ch, sizeof(nvp6134_input_videofmt_ch)))
				printk("IOC_VDEC_PTZ_ACP_READ_EACH_CH error\n");
			break;
		case IOC_VDEC_GET_INPUT_VIDEO_FMT:
			if (copy_from_user(&vfmt, argp, sizeof(nvp6134_input_videofmt)))
				return -1;
			down(&nvp6134_lock);
			g_vloss = video_fmt_det(&vfmt);
			up(&nvp6134_lock);
			if(copy_to_user(argp, &vfmt, sizeof(nvp6134_input_videofmt)))
				printk("IOC_VDEC_GET_INPUT_VIDEO_FMT error\n");
			break;	
        case IOC_VDEC_SET_VIDEO_MODE:
            if (copy_from_user(&vmode, argp, sizeof(nvp6134_video_mode)))
				return -1;
			down(&nvp6134_lock);
			//not used.				
			up(&nvp6134_lock);
			break;
		case IOC_VDEC_SET_CHDETMODE:
            if (copy_from_user(&det_mode, argp, sizeof(unsigned char)*16)) return -1;
			down(&nvp6134_lock);
			for(i=0;i<(nvp6134_cnt*4);i++)
			{
				printk("IOC_VDEC_SET_CHNMODE det_mode[%d]==%d\n", i, det_mode[i]);
				nvp6134_set_chnmode(i, ch_vfmt_status[i], NVP6134_VI_1080P_NOVIDEO);  //set channel to 1080P novideo as default
			}
			up(&nvp6134_lock);
			break;	
		case IOC_VDEC_SET_CHNMODE:
            if (copy_from_user(&schnmode, argp, sizeof(nvp6134_chn_mode))) return -1;
			down(&nvp6134_lock);
			if(0 == nvp6134_set_chnmode(schnmode.ch, schnmode.vformat, schnmode.chmode))
				printk("IOC_VDEC_SET_CHNMODE OK\n");
			up(&nvp6134_lock);
			break;
		case IOC_VDEC_SET_OUTPORTMODE:
            if(copy_from_user(&optmode, argp, sizeof(nvp6134_opt_mode))) return -1;
			down(&nvp6134_lock);
			nvp6134_set_portmode(optmode.chipsel, optmode.portsel, optmode.portmode, optmode.chid);
			up(&nvp6134_lock);
			break;	
		case IOC_VDEC_SET_BRIGHTNESS:
            if(copy_from_user(&v_adj, argp, sizeof(nvp6134_video_adjust))) return -1;
			down(&nvp6134_lock);
			//nvp6134_video_set_brightness(v_adj.ch, v_adj.value, ch_vfmt_status[v_adj.ch]);
			up(&nvp6134_lock);
			break;
		case IOC_VDEC_SET_CONTRAST:
			if(copy_from_user(&v_adj, argp, sizeof(nvp6134_video_adjust))) return -1;
			down(&nvp6134_lock);
			//nvp6134_video_set_contrast(v_adj.ch, v_adj.value, ch_vfmt_status[v_adj.ch]);
			up(&nvp6134_lock);
			break;
		case IOC_VDEC_SET_HUE:
			if(copy_from_user(&v_adj, argp, sizeof(nvp6134_video_adjust))) return -1;
			down(&nvp6134_lock);
			//nvp6134_video_set_hue(v_adj.ch, v_adj.value, ch_vfmt_status[v_adj.ch]);
			up(&nvp6134_lock);
			break;
		case IOC_VDEC_SET_SATURATION:
			if(copy_from_user(&v_adj, argp, sizeof(nvp6134_video_adjust))) return -1;
			down(&nvp6134_lock);
			//nvp6134_video_set_saturation(v_adj.ch, v_adj.value, ch_vfmt_status[v_adj.ch]);
			up(&nvp6134_lock);
			break;
		case IOC_VDEC_SET_SHARPNESS:
			if(copy_from_user(&v_adj, argp, sizeof(nvp6134_video_adjust))) return -1;
			down(&nvp6134_lock);
			nvp6134_video_set_sharpness(v_adj.ch, v_adj.value);
			up(&nvp6134_lock);
			break; 
		case IOC_VDEC_SET_ADC_CLK:
			if(copy_from_user(&v_adj, argp, sizeof(nvp6134_video_adjust))) return -1;
			down(&nvp6134_lock);
			nvp6134_video_set_adcclk(v_adj.ch, v_adj.value);
			up(&nvp6134_lock);
			break;
		case IOC_VDEC_GET_ADC_CLK:
			if (copy_from_user(&v_adj, argp, sizeof(nvp6134_video_adjust)))
				return -1;
			down(&nvp6134_lock);
			v_adj.value = nvp6134_video_get_adcclk(v_adj.ch);
			up(&nvp6134_lock);
			if(copy_to_user(argp, &v_adj, sizeof(nvp6134_video_adjust)))
				printk("IOC_VDEC_GET_ADC_CLK error\n");
			break;
		case IOC_VDEC_PTZ_PELCO_INIT:
			down(&nvp6134_lock);
			//nvp6134_pelco_coax_mode();
			up(&nvp6134_lock);
			break;
		case IOC_VDEC_PTZ_PELCO_RESET:
			down(&nvp6134_lock);
			if(ch_mode_status[g_coax_ch] == NVP6134_VI_EXT_1080P)
				nvp6134_coax_command(g_coax_ch, EXT_CMD_RESET);
			else if(ch_mode_status[g_coax_ch] == NVP6134_VI_EXC_1080P)
				nvp6134_coax_command(g_coax_ch, EXC_CMD_ZOOM_STOP);
			else
				nvp6134_coax_command(g_coax_ch, PELCO_CMD_RESET);
			up(&nvp6134_lock);
			break;
		case IOC_VDEC_PTZ_PELCO_SET:
			down(&nvp6134_lock);
			nvp6134_coax_command(g_coax_ch, PELCO_CMD_SET);
			up(&nvp6134_lock);
			break;
		case IOC_VDEC_PTZ_CHANNEL_SEL:
            if (copy_from_user(&ptz_ch, argp, sizeof(ptz_ch)))
			{
				return -1;
			}
			down(&nvp6134_lock);
			g_coax_ch = ptz_ch;
			up(&nvp6134_lock);
			break;
		case IOC_VDEC_PTZ_PELCO_UP:
			down(&nvp6134_lock);
			if(ch_mode_status[g_coax_ch] == NVP6134_VI_EXC_1080P)
				nvp6134_coax_command(g_coax_ch, EXC_CMD_OSD_UP);		//fulhan CFHD UTC osd control command
			else	
				nvp6134_coax_command(g_coax_ch, PELCO_CMD_UP);
			up(&nvp6134_lock);
		 	break;
		case IOC_VDEC_PTZ_PELCO_DOWN:
			down(&nvp6134_lock);
			if(ch_mode_status[g_coax_ch] == NVP6134_VI_EXC_1080P)
				nvp6134_coax_command(g_coax_ch, EXC_CMD_OSD_DOWN);		//fulhan CFHD UTC osd control command
			else	
				nvp6134_coax_command(g_coax_ch, PELCO_CMD_DOWN);
			up(&nvp6134_lock);
		 	break;
		case IOC_VDEC_PTZ_PELCO_LEFT:
			down(&nvp6134_lock);
			if(ch_mode_status[g_coax_ch] == NVP6134_VI_EXC_1080P)
				nvp6134_coax_command(g_coax_ch, EXC_CMD_OSD_LEFT);		//fulhan CFHD UTC osd control command
			else	
				nvp6134_coax_command(g_coax_ch, PELCO_CMD_LEFT);
			up(&nvp6134_lock);
		 	break;
		case IOC_VDEC_PTZ_PELCO_RIGHT:
			down(&nvp6134_lock);
			if(ch_mode_status[g_coax_ch] == NVP6134_VI_EXC_1080P)
				nvp6134_coax_command(g_coax_ch, EXC_CMD_OSD_RIGHT);		//fulhan CFHD UTC osd control command
			else	
				nvp6134_coax_command(g_coax_ch, PELCO_CMD_RIGHT);
			up(&nvp6134_lock);
			break;
		case IOC_VDEC_PTZ_PELCO_FOCUS_NEAR:
			down(&nvp6134_lock);
			if(ch_mode_status[g_coax_ch] == NVP6134_VI_EXT_1080P)
				nvp6134_coax_command(g_coax_ch, EXT_CMD_FOCUS_NEAR);
			else if(ch_mode_status[g_coax_ch] == NVP6134_VI_EXC_1080P)
			{
				//for(i=0;i<AF_CNT;i++)
					nvp6134_coax_command(g_coax_ch, EXC_CMD_FOCUS_NEAR);
				//msleep(35);
				//nvp6134_coax_command(g_coax_ch, EXC_CMD_ZOOM_STOP);
			}
			else
				nvp6134_coax_command(g_coax_ch, PELCO_CMD_FOCUS_NEAR);
			up(&nvp6134_lock);
		 	break;
		case IOC_VDEC_PTZ_PELCO_FOCUS_FAR:
			down(&nvp6134_lock);
			if(ch_mode_status[g_coax_ch] == NVP6134_VI_EXT_1080P)
				nvp6134_coax_command(g_coax_ch, EXT_CMD_FOCUS_FAR);
			else if(ch_mode_status[g_coax_ch] == NVP6134_VI_EXC_1080P)
			{
				//for(i=0;i<AF_CNT;i++)
					nvp6134_coax_command(g_coax_ch, EXC_CMD_FOCUS_FAR);
				//msleep(35);
				//nvp6134_coax_command(g_coax_ch, EXC_CMD_ZOOM_STOP);
			}
			else
				nvp6134_coax_command(g_coax_ch, PELCO_CMD_FOCUS_FAR);
			up(&nvp6134_lock);
		 	break;
		case IOC_VDEC_PTZ_PELCO_ZOOM_WIDE:
			down(&nvp6134_lock);
			if(ch_mode_status[g_coax_ch] == NVP6134_VI_EXT_1080P)
				nvp6134_coax_command(g_coax_ch, EXT_CMD_ZOOM_WIDE);
			else if(ch_mode_status[g_coax_ch] == NVP6134_VI_EXC_1080P)
			{
				for(i=0;i<AF_CNT;i++)
					nvp6134_coax_command(g_coax_ch, EXC_CMD_ZOOM_WIDE);
			}
			else
				nvp6134_coax_command(g_coax_ch, PELCO_CMD_ZOOM_WIDE);
			up(&nvp6134_lock);
		 	break;
		case IOC_VDEC_PTZ_PELCO_ZOOM_TELE:
			down(&nvp6134_lock);
			if(ch_mode_status[g_coax_ch] == NVP6134_VI_EXT_1080P)
				nvp6134_coax_command(g_coax_ch, EXT_CMD_ZOOM_TELE);
			else if(ch_mode_status[g_coax_ch] == NVP6134_VI_EXC_1080P)
			{
				for(i=0;i<AF_CNT;i++)
					nvp6134_coax_command(g_coax_ch, EXC_CMD_ZOOM_TELE);
				//msleep(35);
				//nvp6134_coax_command(g_coax_ch, EXC_CMD_ZOOM_STOP);
			}
			else
				nvp6134_coax_command(g_coax_ch, PELCO_CMD_ZOOM_TELE);
			up(&nvp6134_lock);
			break;

		case IOC_VDEC_INIT_MOTION:
			down(&nvp6134_lock);
			nvp6134_motion_init(0, 0);
			up(&nvp6134_lock);
			break;
		case IOC_VDEC_ENABLE_MOTION:
			break;
		case IOC_VDEC_DISABLE_MOTION:
			break;
		case IOC_VDEC_SET_MOTION_AREA:
			break;
		case IOC_VDEC_GET_MOTION_INFO:
			nvp6134_get_motion_ch();
			break;
		case IOC_VDEC_SET_MOTION_DISPLAY:
            if(copy_from_user(&on, argp, sizeof(unsigned int))) return -1;
			down(&nvp6134_lock);
			nvp6134_motion_display(0,on);
			up(&nvp6134_lock);
			break;
		case IOC_VDEC_SET_MOTION_SENS:
            if(copy_from_user(&sens, argp, sizeof(unsigned int)*16)) return -1;
			down(&nvp6134_lock);
			nvp6134_motion_sensitivity(sens);
			up(&nvp6134_lock);
			break;
		default:
            //printk("drv:invalid nc decoder ioctl cmd[%x]\n", cmd);
			break;
	}
	return 0;
}

#ifdef I2C_INTERNAL
static int i2c_client_init(void)
{
    struct i2c_adapter* i2c_adap;

    #ifdef HI_CHIP_HI3521A
	printk("HI_CHIP_HI3521A\n");
    i2c_adap = i2c_get_adapter(0);
    #else
	printk("HI_CHIP_HI3531A\n");
    i2c_adap = i2c_get_adapter(1);
    #endif
    nvp6134_client = i2c_new_device(i2c_adap, &hi_info);
    i2c_put_adapter(i2c_adap);

    return 0;
}

static void i2c_client_exit(void)
{
    i2c_unregister_device(nvp6134_client);
}
#endif

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
static int nvp6134_freeze(struct himedia_device* pdev)
{
    printk(KERN_ALERT "%s  %d\n", __FUNCTION__, __LINE__);
    return 0;
}

static int nvp6134_restore(struct himedia_device* pdev)
{  
    printk(KERN_ALERT "%s  %d\n", __FUNCTION__, __LINE__);
    return 0;
}
#endif


static struct file_operations nvp6134_fops = {
	.owner      = THIS_MODULE,
    .unlocked_ioctl	= nvp6134_ioctl,
	.open       = nvp6134_open,
	.release    = nvp6134_close
};

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
struct himedia_ops stNvp6134DrvOps =
{
    .pm_freeze = nvp6134_freeze,
    .pm_restore  = nvp6134_restore
};
#else
static struct miscdevice nvp6134_dev = {
	.minor		= MISC_DYNAMIC_MINOR,
	.name		= "nc_vdec",
	.fops  		= &nvp6134_fops,
};
#endif

/*******************************************************************************
*	Description		: kernel thread for EQ (now, not used)
*	Argurments		: void
*	Return value	: 0
*	Modify			:
*	warning			:
*******************************************************************************/
static int nvp6134_kernel_thread(void *data)
{
	//int ch;
	//nvp6134_input_videofmt videofmt;

	while(!kthread_should_stop())
	{
		
		#if 0   //standard rutine of a process
		down(&nvp6134_lock);
		nvp6134_getvideoloss();
		video_fmt_det(&videofmt);
		if(videofmt.getvideofmt[ch]==0x90)
		{	
			nvp6134_set_chnmode(ch, PAL, NVP6134_VI_3M_NRT);
			nvp6134_set_portmode(ch/4, 1, NVP6134_OUTMODE_2MUX_FHD, 0);
		}
		nvp6134_set_equalizer(ch);
		up(&nvp6134_lock);
		#endif
		schedule_timeout_interruptible(msecs_to_jiffies(1000));
		//printk("nvp6134_kernel_thread running\n");
	}

	return 0;
}

/*******************************************************************************
*	Description		: It is called when "insmod nvp61XX_ex.ko" command run
*	Argurments		: void
*	Return value	: -1(could not register nvp61XX device), 0(success)
*	Modify			:
*	warning			:
*******************************************************************************/
static int __init nvp6134_module_init(void)
{
	int ret = 0;
	int ch = 0;
	int chip = 0;
#ifdef CONFIG_HISI_SNAPSHOT_BOOT
    snprintf(s_stNvp6134Device.devfs_name, sizeof(s_stNvp6134Device.devfs_name), DEV_NAME);
    s_stNvp6134Device.minor  = HIMEDIA_DYNAMIC_MINOR;
    s_stNvp6134Device.fops   = &nvp6134_fops;
    s_stNvp6134Device.drvops = &stNvp6134DrvOps;
    s_stNvp6134Device.owner  = THIS_MODULE;

    ret = himedia_register(&s_stNvp6134Device);
    if (ret)
    {
        printk(0, "could not register nvp6134_dev device");
        return -1;
    }
#else
	ret = misc_register(&nvp6134_dev);
   	if (ret)
	{
		printk("ERROR: could not register nvp6134_dev devices:%#x \n",ret);		
        return -1;
	}
#endif
	
	printk("NVP6134(B/C) EXT Driver %s COMPILE TIME[%s %s]\n", DRIVER_VER, __DATE__,__TIME__);
	#ifdef I2C_INTERNAL
    i2c_client_init();
	#endif
	
    /* check Device ID of maxium 4chip on the slave address,
     * manage slave address. chip count. */
	for(chip=0;chip<4;chip++)
	{
		chip_id[chip] = check_id(nvp6134_iic_addr[chip]);
		rev_id[chip]  = check_rev(nvp6134_iic_addr[chip]);
		if( (chip_id[chip] != NVP6134_R0_ID )  	&& 
			(chip_id[chip] != NVP6134B_R0_ID) )
		{
			printk("Device ID Error... %x\n", chip_id[chip]);
		}
		else
		{
			printk("Device (0x%x) ID OK... %x\n", nvp6134_iic_addr[chip], chip_id[chip]);
			printk("Device (0x%x) REV ... %x\n", nvp6134_iic_addr[chip], rev_id[chip]);
			nvp6134_iic_addr[nvp6134_cnt] = nvp6134_iic_addr[chip];	
			if(nvp6134_cnt<chip)
				nvp6134_iic_addr[chip] = 0xFF;
			chip_id[nvp6134_cnt] = chip_id[chip];
			rev_id[nvp6134_cnt]  = rev_id[chip];
			nvp6134_cnt++;
		}
	}
	
	printk("Chip Count = %d, [0x%x][0x%x][0x%x][0x%x]\n", \
		nvp6134_cnt, nvp6134_iic_addr[0],nvp6134_iic_addr[1],\
		nvp6134_iic_addr[2],nvp6134_iic_addr[3]);
	
	/* initialize semaphore */
	sema_init(&nvp6134_lock, 1);

	/* initialize eq structure */
	memset(&s_eq, 0x00, sizeof(nvp6134_equalizer));
	memset(&s_eq_type, 0x00, sizeof(nvp6134_equalizer));

    /* initialize common value of AHD */
	for(chip=0;chip<nvp6134_cnt;chip++)
		nvp6134_common_init(chip);

	/* set channel mode(AHD 1080P) each chip - default */
	for(ch=0;ch<nvp6134_cnt*4;ch++)
	{
		//没有视频接入的时候，必须默认设置为AHD 1080P novideo模式.
		det_mode[ch] = NVP6134_DET_MODE_AUTO;
		nvp6134_set_chnmode(ch, PAL, NVP6134_VI_1080P_NOVIDEO);   
	}
	/* set port(1MUX AHD 1080P) each chip - default */
	for(chip=0;chip<nvp6134_cnt;chip++)
	{
		if(chip_id[chip] == NVP6134_R0_ID)
		{
			//设置nvp6134 4个port的输出模式 0~3 port口可用
			nvp6134_set_portmode(chip, 0, NVP6134_OUTMODE_1MUX_FHD, 0);  
			nvp6134_set_portmode(chip, 1, NVP6134_OUTMODE_1MUX_FHD, 1);
			nvp6134_set_portmode(chip, 2, NVP6134_OUTMODE_1MUX_FHD, 2);
			nvp6134_set_portmode(chip, 3, NVP6134_OUTMODE_1MUX_FHD, 3);
		}
		else if(chip_id[chip] == NVP6134B_R0_ID)
		{
			//设置nvp6134b 2个port的输出模式, 1/2 port口可用
			nvp6134_set_portmode(chip, 1, NVP6134_OUTMODE_2MUX_FHD, 0);  
			nvp6134_set_portmode(chip, 2, NVP6134_OUTMODE_2MUX_FHD, 1);
		}
	}
	
	/* initialize Audio
	 * recmaster, pbmaster, ch_num, samplerate, bits */
	audio_init(1,0,16,0,0);

	/* create kernel thread for EQ, But Now not used. */
	nvp6134_kt = kthread_create(nvp6134_kernel_thread, NULL, "nvp6134_kt");
    if(!IS_ERR(nvp6134_kt))
        wake_up_process(nvp6134_kt);
    else {
        printk("create nvp6134 watchdog thread failed!!\n");
        nvp6134_kt = 0;
        return 0;
    }
	
	return 0;
}

/*******************************************************************************
*	Description		: It is called when "rmmod nvp61XX_ex.ko" command run
*	Argurments		: void
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
static void __exit nvp6134_module_exit(void)
{
	if(nvp6134_kt)
        kthread_stop(nvp6134_kt);
#ifdef CONFIG_HISI_SNAPSHOT_BOOT
    himedia_unregister(&s_stNvp6134Device);
#else
    misc_deregister(&nvp6134_dev);
#endif	
#ifdef I2C_INTERNAL
    i2c_client_exit();
#endif	
}

module_init(nvp6134_module_init);
module_exit(nvp6134_module_exit);

MODULE_LICENSE("GPL");

/*******************************************************************************
*	End of file
*******************************************************************************/

