/********************************************************************************
*
*  Copyright (C) 2017 	NEXTCHIP Inc. All rights reserved.
*  Module		: The decoder's audio module
*  Description	: Audio i/o
*  Author		:
*  Date         :
*  Version		: Version 2.0
*
********************************************************************************
*  History      :
*
*
********************************************************************************/
#include <linux/string.h>
#include <linux/delay.h>

#include "common.h"
#include "audio.h"


/*******************************************************************************
 * extern variable
 *******************************************************************************/
extern int 				chip_id[4];				/* Chip ID */
extern int 				rev_id[4];				/* Reversion ID */
extern unsigned int 	nvp6134_cnt;			/* Chip count */
extern unsigned int 	nvp6134_iic_addr[4];	/* Slave address of Chip */

/*******************************************************************************
 * internal variable
 *******************************************************************************/


/*******************************************************************************
 *
 *
 *
 *  Internal Functions
 *
 *
 *
 *******************************************************************************/



/*******************************************************************************
 *
 *
 *
 *  External Functions
 *
 *
 *
 *******************************************************************************/
/*******************************************************************************
*	Description		: initialize audio
*	Argurments		: recmaster(0[slave],1[master];), pbmaster(), ch_num(audio channel number)
*					  samplerate(sample rate), bits(bits)
*	Return value	: void
*	Modify			:
*	warning			:
*
* 	 param:
*		- xxmaster:0[slave],1[master];
*		- ch_num: audio channel number
*		- samplerate: 0[8k], 1[16k]
*		- bits: 0[16bits], 1[8bits]
*
*******************************************************************************/
void audio_init(unsigned char recmaster, unsigned char pbmaster, unsigned char ch_num, unsigned char samplerate, unsigned char bits)
{
	int i;
	for(i=0;i<nvp6134_cnt;i++)
	{
		gpio_i2c_write(nvp6134_iic_addr[i], 0xff, 0x01);
		gpio_i2c_write(nvp6134_iic_addr[i], 0x94, 0x50);  //audio system clk = 74.25MHz
		gpio_i2c_write(nvp6134_iic_addr[i], 0x3a, 0x03);  //audio dac gain x1
		gpio_i2c_write(nvp6134_iic_addr[i], 0x3b, 0x30);  //audio afe gain x1
		gpio_i2c_write(nvp6134_iic_addr[i], 0x48, 0x10);  //audio sampling selection
	 	if(chip_id[i] == NVP6134B_R0_ID && rev_id[i]!=NVP6134B_REV_ID) 
			gpio_i2c_write(nvp6134_iic_addr[i], 0x31, 0x32); // NVP6134C 
		else
			gpio_i2c_write(nvp6134_iic_addr[i], 0x31, 0x82); // NVP6134&NVP6134B 
		
		if(i == 0)
		{
	 		if(chip_id[i] == NVP6134B_R0_ID && rev_id[i]!=NVP6134B_REV_ID) 
				gpio_i2c_write(nvp6134_iic_addr[i], 0x06, 0x3a); //Set chip0 NVP6134C to first stage
			else
				gpio_i2c_write(nvp6134_iic_addr[i], 0x06, 0x1a); // NVP6134&NVP6134B
			
			if(recmaster == 1)
			{
				gpio_i2c_write(nvp6134_iic_addr[i], 0x07, 0x80|(samplerate<<3)|(bits<<2));	//i2s rec: master
				gpio_i2c_write(nvp6134_iic_addr[i], 0x39, 0x00);
			}
			else
			{
				gpio_i2c_write(nvp6134_iic_addr[i], 0x07, 0x00|(samplerate<<3)|(bits<<2));	//i2s rec: slave
				gpio_i2c_write(nvp6134_iic_addr[i], 0x39, 0x80);
			}
			if(pbmaster == 1)
			{
				gpio_i2c_write(nvp6134_iic_addr[i], 0x13, 0x80|(samplerate<<3)|(bits<<2));	// PB I2C 8k 16bit : master
				gpio_i2c_write(nvp6134_iic_addr[i], 0xd5, 0x00);  
			}
			else
			{
				gpio_i2c_write(nvp6134_iic_addr[i], 0x13, 0x00|(samplerate<<3)|(bits<<2));	// PB I2C 8k 16bit : slave
				gpio_i2c_write(nvp6134_iic_addr[i], 0xd5, 0x01); 
			}
			
			if(8 == ch_num)
			{
  	 			if(chip_id[i] == NVP6134B_R0_ID && rev_id[i]!=NVP6134B_REV_ID) 
  					gpio_i2c_write(nvp6134_iic_addr[i], 0x06, 0x3a); // Set chip0 NVP6134C to first stage 
	  			else
		  			gpio_i2c_write(nvp6134_iic_addr[i], 0x06, 0x1b); // NVP6134&NVP6134B
 
				gpio_i2c_write(nvp6134_iic_addr[i], 0x08, 0x02);
				gpio_i2c_write(nvp6134_iic_addr[i], 0x0f, 0x54);    //set I2S right sequence
				gpio_i2c_write(nvp6134_iic_addr[i], 0x10, 0x76);
			}
			else if(4 == ch_num)
			{
				if(chip_id[i] == NVP6134B_R0_ID && rev_id[i]!=NVP6134B_REV_ID) 
					gpio_i2c_write(nvp6134_iic_addr[i], 0x06, 0x3b); // Set chip0 NVP6134C to single mode
				else
					gpio_i2c_write(nvp6134_iic_addr[i], 0x06, 0x1b); // NVP6134&NVP6134B

				gpio_i2c_write(nvp6134_iic_addr[i], 0x08, 0x01);
				gpio_i2c_write(nvp6134_iic_addr[i], 0x0f, 0x32);   //set I2S right sequence
			}
			
			gpio_i2c_write(nvp6134_iic_addr[i], 0x23, 0x10);  // Audio playback out
			//gpio_i2c_write(nvp6134_iic_addr[i], 0x23, 0x18);  // Audio mix out | for AD loopback test
		}
		else
		{
 	 		if(chip_id[i] == NVP6134B_R0_ID && rev_id[i]!=NVP6134B_REV_ID) 
 	 		{
 	 			if(8 == ch_num)
					gpio_i2c_write(nvp6134_iic_addr[i], 0x06, 0x39); // Set chip1 NVP6134C to last stage
				else if(16 == ch_num)
				{
					if(i==1 || i==2)
						gpio_i2c_write(nvp6134_iic_addr[i], 0x06, 0x3A); // Set chipx NVP6134C to middle stage
					else
						gpio_i2c_write(nvp6134_iic_addr[i], 0x06, 0x39); // Set chip3 NVP6134C to last stage
				}
 	 		}
			else
				gpio_i2c_write(nvp6134_iic_addr[i], 0x06, 0x19); // NVP6134&NVP6134B

			gpio_i2c_write(nvp6134_iic_addr[i], 0x07, 0x80|(samplerate<<3)|(bits<<2));	//Rec I2C 16K 16bit : master
			gpio_i2c_write(nvp6134_iic_addr[i], 0x39, 0x00);
			gpio_i2c_write(nvp6134_iic_addr[i], 0x13, 0x00|(samplerate<<3)|(bits<<2));	// PB I2C 8k 16bit : slave
			gpio_i2c_write(nvp6134_iic_addr[i], 0x23, 0x10);  // Audio playback out
			gpio_i2c_write(nvp6134_iic_addr[i], 0xd5, 0x01);
		}	
		gpio_i2c_write(nvp6134_iic_addr[i], 0x01, AIG_DEF);  // ch1 Audio input gain init
		gpio_i2c_write(nvp6134_iic_addr[i], 0x02, AIG_DEF);
		gpio_i2c_write(nvp6134_iic_addr[i], 0x03, AIG_DEF);
		gpio_i2c_write(nvp6134_iic_addr[i], 0x04, AIG_DEF);
		gpio_i2c_write(nvp6134_iic_addr[i], 0x05, AIG_DEF); 
		gpio_i2c_write(nvp6134_iic_addr[i], 0x40, AIG_DEF);  
		gpio_i2c_write(nvp6134_iic_addr[i], 0x41, AIG_DEF);
		gpio_i2c_write(nvp6134_iic_addr[i], 0x42, AIG_DEF);
		gpio_i2c_write(nvp6134_iic_addr[i], 0x43, AIG_DEF);
		gpio_i2c_write(nvp6134_iic_addr[i], 0x22, AOG_DEF);  
		
		gpio_i2c_write(nvp6134_iic_addr[i], 0x24, 0x14); //set mic_1's data to i2s_sp left channel
		gpio_i2c_write(nvp6134_iic_addr[i], 0x25, 0x15); //set mic_2's data to i2s_sp right channel

		gpio_i2c_write(nvp6134_iic_addr[i], 0x44, 0x11);  //audio LPF to reduce noise
		//gpio_i2c_write(nvp6134_iic_addr[i], 0x39, 0x00);
		gpio_i2c_write(nvp6134_iic_addr[i], 0x94, 0x50);
		gpio_i2c_write(nvp6134_iic_addr[i], 0x49, 0x88);  //audio live mute 
		gpio_i2c_write(nvp6134_iic_addr[i], 0x38, 0x18);
		msleep(20);
		gpio_i2c_write(nvp6134_iic_addr[i], 0x38, 0x08);
		
		printk("nvp6134 audio init,CH:%d REC:%s,PB:%s\n", ch_num, recmaster==1?"MASTER":"SLAVE", pbmaster==1?"MASTER":"SLAVE");
	}
}

/*******************************************************************************
*	End of file
*******************************************************************************/
