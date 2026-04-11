/*
 * ./arch/arm/mach-hi3511_v100_f01/hi_dmac.c
 *
 * Copyright (c) 2006 Hisilicon Co., Ltd.
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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 *
 *
 * History:
 *      17-August-2006 create this file
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
#include <linux/spinlock.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
/* mach/hardware.h removed â€” platform-specific, not needed on 4.9+ */
#include <asm/io.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/string.h>
#include <linux/dma-mapping.h>

#include "hi_dmac.h"
#include "dmac_ext.h"

void __iomem *reg_dmac_base_va;
#define IO_DMAC_ADDRESS(x) (reg_dmac_base_va + ((x)-(DMAC_BASE_REG)))

#define CLR_INT(i)	(*(volatile unsigned int *)IO_DMAC_ADDRESS(DMAC_BASE_REG+0x008))=(1<<i)
#define CHANNEL_NUM     2

#if HICHIP==0x35180100
static int          dmac_channel[CHANNEL_NUM]={2,3}; /* for 3518 only 4 channel, 0,1 for SIO*/
#else
static int          dmac_channel[CHANNEL_NUM]={6,7};
#endif

//static int    g_channel_status[DMAC_MAX_CHANNELS];
int    g_channel_status[DMAC_MAX_CHANNELS];

//static int sio0_mode = 0; /* 0-i2s mode 1-pcm mode */
//static int sio1_mode = 0; /* 0-i2s mode 1-pcm mode */
//static int sio2_mode = 0; /* 0-i2s mode 1-pcm mode */
//static int sio3_mode = 0; /* 0-i2s mode 1-pcm mode */
//static int sio4_mode = 0; /* 0-i2s mode 1-pcm mode */
//static int sio5_mode = 0; /* 0-i2s mode 1-pcm mode */

/*
 * 	Define Memory range
 */
mem_addr mem_num[MEM_MAX_NUM]=
{
    	{DDRAM_ADRS,DDRAM_SIZE},
    	{FLASH_BASE,FLASH_SIZE},
    	{ITCM_BASE,ITCM_SIZE}
};

typedef void REG_ISR(int *p_dma_chn,int *p_dma_status) ;
REG_ISR *function[DMAC_MAX_CHANNELS];


/*
 *	DMA config array!
 */
#if (HICHIP==0x35310100 || HICHIP==0x35320100) 
dmac_peripheral  g_peripheral[DMAC_MAX_PERIPHERALS]=
{
    	/*periphal 0,SIO0_RX*/
    	{ DMAC_SIO0_RX_REQ,(unsigned int*)SIO0_RX_FIFO,SIO0_RX_CONTROL,SIO0_RX_CONFIG },
    	/*periphal 1,SIO1_RX*/
    	{ DMAC_SIO1_RX_REQ,(unsigned int*)SIO1_RX_FIFO,SIO1_RX_CONTROL,SIO1_RX_CONFIG },

    	/*periphal 2,SIO2_RX*/
    	{ DMAC_SIO2_RX_REQ,(unsigned int*)SIO2_RX_FIFO,SIO2_RX_CONTROL,SIO2_RX_CONFIG },
        /*periphal 3,SIO3_RX*/
    	{ DMAC_SIO3_RX_REQ,(unsigned int*)SIO3_RX_FIFO,SIO3_RX_CONTROL,SIO3_RX_CONFIG },

    	/*periphal 4,SIO4_RX*/
    	{ DMAC_SIO4_RX_REQ,(unsigned int*)SIO4_RX_FIFO,SIO4_RX_CONTROL,SIO4_RX_CONFIG },
    	/*periphal 5,SIO4_TX*/
        { DMAC_SIO4_TX_REQ,(unsigned int*)SIO4_TX_FIFO,SIO4_TX_CONTROL,SIO4_TX_CONFIG },
        /*periphal 6,SIO5_TX*/
        { DMAC_SIO5_TX_REQ,(unsigned int*)SIO5_TX_FIFO,SIO5_TX_CONTROL,SIO5_TX_CONFIG },
};
#elif (HICHIP==0x35210100 || HICHIP==0x3520A100) 
dmac_peripheral  g_peripheral[DMAC_MAX_PERIPHERALS]=
{
    	/*periphal 0,SIO0_RX*/
    	{ DMAC_SIO0_RX_REQ,(unsigned int*)SIO0_RX_FIFO,SIO0_RX_CONTROL,SIO0_RX_CONFIG },
            
    	/*periphal 1,SIO1_RX*/
    	{ DMAC_SIO1_RX_REQ,(unsigned int*)SIO1_RX_FIFO,SIO1_RX_CONTROL,SIO1_RX_CONFIG },
            
    	/*periphal 2,SIO2_RX*/
    	{ DMAC_SIO2_RX_REQ,(unsigned int*)SIO2_RX_FIFO,SIO2_RX_CONTROL,SIO2_RX_CONFIG },
            
        /*periphal 2,SIO2_TX*/
    	{ DMAC_SIO2_TX_REQ,(unsigned int*)SIO2_TX_FIFO,SIO2_TX_CONTROL,SIO2_TX_CONFIG },
            
        /*periphal 3,SIO3_TX*/
    	{ DMAC_SIO3_TX_REQ,(unsigned int*)SIO3_TX_FIFO,SIO3_TX_CONTROL,SIO3_TX_CONFIG },
};
#elif (HICHIP==0x35180100) 
dmac_peripheral  g_peripheral[DMAC_MAX_PERIPHERALS]=
{
    	/*periphal 0,SIO0_RX*/
    	{ DMAC_SIO0_RX_REQ,(unsigned int*)SIO0_RX_FIFO,SIO0_RX_CONTROL,SIO0_RX_CONFIG },
            
    	/*periphal 1,SIO0_RX*/
    	{ DMAC_SIO0_TX_REQ,(unsigned int*)SIO0_TX_FIFO,SIO0_TX_CONTROL,SIO0_TX_CONFIG },
};

#else
    #error HICHIP define may be error
#endif

/*
 *	dmac interrupt handle function
 */
static   irqreturn_t dmac_isr(int irq, void * dev_id)
{
	unsigned int channel_status, tmp_channel_status[3], channel_tc_status, channel_err_status;
	unsigned int i,j,count = 0;

    /*read the status of current interrupt */
	dmac_readw(DMAC_INTSTATUS, channel_status);
    //printk("func--%s, line--%d:channel_status:0x%x\n", __FUNCTION__, __LINE__, channel_status);

    /*decide which channel has trigger the interrupt*/
	for(i = 0; i < DMAC_MAX_CHANNELS; i++)
    {
        count = 0;
        while(1)
        {
            for(j=0;j<3;j++)
            {
                dmac_readw(DMAC_INTSTATUS, channel_status);                 /*3 times for fault-tolerant*/
                tmp_channel_status[j] = (channel_status >> i) & 0x01;
            }

            if ((tmp_channel_status[0] == 0x1) && (tmp_channel_status[1] == 0x1) && (tmp_channel_status[2] == 0x1))
            {
                break;
            }
            else if ((tmp_channel_status[0] == 0x0) && (tmp_channel_status[1] == 0x0) && (tmp_channel_status[2] == 0x0))
            {
                break;
            }

            count++;
            if(count > 10)
            {
                printk("DMAC %d channel Int status is error.\n",i);
                break;
            }
        }

    	if((tmp_channel_status[0] == 0x01))
    	{
            /* [HSCP201306240006],l00181524,20130625, The INT status should be read first, then clear it */
            //CLR_INT(i);
            dmac_readw(DMAC_INTTCSTATUS, channel_tc_status);
            dmac_readw(DMAC_INTERRORSTATUS, channel_err_status);
            CLR_INT(i);

            //printk(".....channel_err_status:%d.....\n",channel_err_status);
            /*¡¾HSCP201403110002¡¿ l00183122 20140723*/
        	if(g_channel_status[i] == DMAC_CHN_VACANCY && (function[i]) == NULL) // chn free
            {
                if((0x01 == ((channel_tc_status >> i) & 0x01)))
                {
                     dmac_writew(DMAC_INTTCCLEAR, (0x01 << i));
                }
                else if((0x01 == ((channel_err_status >> i)&0x01)))
                {
                    dmac_writew(DMAC_INTERRCLR, (0x01 << i));
                }

                continue;
            }

            /*save the current channel transfer status to g_channel_status[i]*/
            if((0x01 == ((channel_tc_status >> i) & 0x01)))
            {
                //printk(">");
            	g_channel_status[i] = DMAC_CHN_SUCCESS;
            	dmac_writew(DMAC_INTTCCLEAR, (0x01 << i));
            }
            else if((0x01 == ((channel_err_status >> i)&0x01)))
            {
            	g_channel_status[i] = -DMAC_CHN_ERROR;
            	printk("Error in DMAC %d channel as finish!\n",i);
            	dmac_writew(DMAC_INTERRCLR, (0x01 << i));
            }
            else
            {
            	/*printk("Isr Error in DMAC_IntHandeler %d! channel\n" ,i); */
            }
            if((function[i])!=NULL)
            {
            	function[i](&i,&g_channel_status[i]);
    	    }
        }
    }

	return IRQ_RETVAL(1);
}

/*
 *	memory address validity check
 */
static int mem_check_valid(unsigned int addr)
{
	unsigned int cnt;

	for(cnt=0;cnt < MEM_MAX_NUM;cnt++)
	{
    	if((addr >= mem_num[cnt].addr_base) && (addr <= (mem_num[cnt].addr_base + mem_num[cnt].size)))
    	{
    		return 0;
		}
    }
	return -1;
}

/*
 *	check the state of channels
 */
int dmac_check_over(unsigned int channel)
{
    	if (-DMAC_CHN_ERROR == g_channel_status[channel])
    	{
        	/*printk( "The transfer of Channel %d has finished with errors!\n",channel);*/
        	dmac_writew(DMAC_CxCONFIG(channel),DMAC_CxDISABLE);
        	g_channel_status[channel] = DMAC_CHN_VACANCY;
        	return -DMAC_CHN_ERROR;
    	}

    	else if (DMAC_NOT_FINISHED == g_channel_status[channel])
    	{
        	return DMAC_NOT_FINISHED;
    	}

    	else if(DMAC_CHN_ALLOCAT == g_channel_status[channel])
    	{
        	return DMAC_CHN_ALLOCAT;
    	}

    	else if(DMAC_CHN_VACANCY == g_channel_status[channel])
    	{
        	return DMAC_CHN_VACANCY;
    	}

    	else if(-DMAC_CHN_TIMEOUT == g_channel_status[channel])
    	{

    		printk("The transfer of Channel %d has timeout!\n",channel);
        	return -DMAC_CHN_TIMEOUT;
    	}
    	else if (DMAC_CHN_SUCCESS == g_channel_status[channel])
    	{
    		/*The transfer of Channel %d has finished successfully!*/
        	return DMAC_CHN_SUCCESS;
    	}
     	else
     	{
        	dmac_writew(DMAC_CxCONFIG(channel),DMAC_CxDISABLE);
        	g_channel_status[channel] =DMAC_CHN_VACANCY;
        	return -DMAC_CHN_ERROR;
     	}
}

spinlock_t my_lcok = __SPIN_LOCK_UNLOCKED(old_style_spin_init);
unsigned long flags;
/*
 *	allocate channel.
 */
int  dmac_channel_allocate(void *pisr)
{
    	unsigned int i,channelinfo;
    	for(i=0;i<CHANNEL_NUM;i++)
    	{
    		dmac_check_over(dmac_channel[i]);
    	}
        spin_lock_irqsave(&my_lcok,flags);
        dmac_readw(DMAC_ENBLDCHNS, channelinfo);
        channelinfo= channelinfo&0x00ff;

    	for (i = 0; i < CHANNEL_NUM; i++)
    	{
//    	    printk("allocate channel status is %d......\n",g_channel_status[i]);
            if (g_channel_status[dmac_channel[i]]==DMAC_CHN_VACANCY)
        	{
        	    channelinfo = channelinfo >> dmac_channel[i];
                if (0x00 == (channelinfo & 0x01))
                {
            		dmac_writew(DMAC_INTERRCLR, (0x01 << dmac_channel[i])); /*clear the interrupt in this channel */
            		dmac_writew(DMAC_INTTCCLEAR, (0x01 << dmac_channel[i]));

            		function[dmac_channel[i]] =(void *)pisr;
            		g_channel_status[dmac_channel[i]] = DMAC_CHN_ALLOCAT;
//            		printk("allocate channel is %d......\n",i);
            		spin_unlock_irqrestore(&my_lcok,flags);
            		return dmac_channel[i];
            	}
        	}
    	}
    	spin_unlock_irqrestore(&my_lcok,flags);
    	printk("DMAC :no available channel can allocate!\n");
    	return -EINVAL;
}


int dmac_register_isr(unsigned int channel,void *pisr)
{
	if((channel < 0) || (channel > 7))
	{
	    printk("channel which choosed %d is error !\n",channel);
		return -1;
	}
    if(g_channel_status[channel] != DMAC_CHN_VACANCY)
    {
        printk("dma chn %d is in used!\n",channel);
        return -1;
    }
	/*clear the interrupt in this channel */
	dmac_writew(DMAC_INTERRCLR, (0x01 << channel));
	dmac_writew(DMAC_INTTCCLEAR, (0x01 << channel));
	function[channel] = (void *)pisr;
	g_channel_status[channel] = DMAC_CHN_ALLOCAT;
	return 0;
}


/*
 *	free channel
 */
int  dmac_channel_free(unsigned int channel)
{
     	g_channel_status[channel] = DMAC_CHN_VACANCY;
     	function[channel]= NULL;
     	return 0;
}



/*
 *	init dmac register
 *	clear interupt flags
 *	called by dma_driver_init
 */
int  dmac_init(void)
{
    	unsigned int i,tempvalue;
        
        #if (HICHIP==0x35310100 || HICHIP==0x35320100) 
        unsigned int sio0_rx_fifo =  SIO0_RX_FIFO;
        unsigned int sio1_rx_fifo =  SIO1_RX_FIFO;
        unsigned int sio2_rx_fifo =  SIO2_RX_FIFO;
        unsigned int sio3_rx_fifo =  SIO3_RX_FIFO;
        unsigned int sio4_rx_fifo =  SIO4_RX_FIFO;
        unsigned int sio4_tx_fifo =  SIO4_TX_FIFO;
        unsigned int sio5_tx_fifo =  SIO5_TX_FIFO;
        
        #elif (HICHIP==0x35210100 || HICHIP==0x3520A100)
        unsigned int sio0_rx_fifo =  SIO0_RX_FIFO;
        unsigned int sio1_rx_fifo =  SIO1_RX_FIFO;
        unsigned int sio2_rx_fifo =  SIO2_RX_FIFO;
        unsigned int sio2_tx_fifo =  SIO2_TX_FIFO;
        unsigned int sio3_tx_fifo =  SIO3_TX_FIFO;
        
        #elif (HICHIP==0x35180100)
        unsigned int sio0_rx_fifo =  SIO0_RX_FIFO;
        unsigned int sio0_tx_fifo =  SIO0_TX_FIFO;

        #endif

        
        unsigned int u32value;

        void __iomem *reg_dmac_softrst = ioremap_nocache(0x200300e0, (unsigned long)0x10000);
        dmac_readw(reg_dmac_softrst,u32value);
        u32value &= ~0x1;
        u32value |= 0x2;
        dmac_writew(reg_dmac_softrst,u32value);
        iounmap((void*)reg_dmac_softrst);
        
        reg_dmac_base_va = ioremap_nocache((unsigned long)DMAC_BASE_REG, (unsigned long)0x10000);

        dmac_readw(DMAC_CONFIG, tempvalue);
        if(tempvalue == 0)
        {
    	    dmac_writew(DMAC_CONFIG,DMAC_CONFIG_VAL);
    	    dmac_writew(DMAC_SYNC,DMAC_SYNC_VAL);
    	    dmac_writew(DMAC_INTTCCLEAR,0xFF);
    	    dmac_writew(DMAC_INTERRCLR,0xFF);
        	for(i=0;i< DMAC_MAX_CHANNELS;i++)
        	{
            	dmac_writew (DMAC_CxCONFIG(i), DMAC_CxDISABLE);
        	}
        }

        #if (HICHIP==0x35310100 || HICHIP==0x35320100) 
    	g_peripheral[DMAC_SIO0_RX_REQ].pperi_addr = (unsigned int*)sio0_rx_fifo;

    	g_peripheral[DMAC_SIO1_RX_REQ].pperi_addr = (unsigned int*)sio1_rx_fifo;

    	g_peripheral[DMAC_SIO2_RX_REQ].pperi_addr = (unsigned int*)sio2_rx_fifo;

    	g_peripheral[DMAC_SIO3_RX_REQ].pperi_addr = (unsigned int*)sio3_rx_fifo;
        
    	g_peripheral[DMAC_SIO4_RX_REQ].pperi_addr = (unsigned int*)sio4_rx_fifo;
        
        g_peripheral[DMAC_SIO4_TX_REQ].pperi_addr = (unsigned int*)sio4_tx_fifo;
    	g_peripheral[DMAC_SIO5_TX_REQ].pperi_addr = (unsigned int*)sio5_tx_fifo;
        
    #elif (HICHIP==0x35210100 || HICHIP==0x3520A100)
        g_peripheral[DMAC_SIO0_RX_REQ].pperi_addr = (unsigned int*)sio0_rx_fifo;

    	g_peripheral[DMAC_SIO1_RX_REQ].pperi_addr = (unsigned int*)sio1_rx_fifo;

    	g_peripheral[DMAC_SIO2_RX_REQ].pperi_addr = (unsigned int*)sio2_rx_fifo;

    	g_peripheral[DMAC_SIO2_TX_REQ].pperi_addr = (unsigned int*)sio2_tx_fifo;
        g_peripheral[DMAC_SIO3_TX_REQ].pperi_addr = (unsigned int*)sio3_tx_fifo;

	#elif (HICHIP==0x35180100)
        g_peripheral[DMAC_SIO0_RX_REQ].pperi_addr = (unsigned int*)sio0_rx_fifo;
        g_peripheral[DMAC_SIO0_TX_REQ].pperi_addr = (unsigned int*)sio0_tx_fifo;

	#endif
    
	return 0;
}

/*
 *	alloc_dma_lli_space
 *	output:
 *             ppheadlli[0]: memory physics address
 *             ppheadlli[1]: virtual address
 *
 */
int allocate_dmalli_space(unsigned int *ppheadlli, unsigned int page_num)
{
    	dma_addr_t dma_phys;
    	void *dma_virt ;
    	unsigned int *address;
    	address =ppheadlli;

    	dma_virt = dma_alloc_coherent(NULL,page_num*PAGE_SIZE,&dma_phys,GFP_DMA|__GFP_WAIT);
    	if (NULL == dma_virt)
    	{
        	printk("can't get dma mem from system\n");
        	return -1;
    	}
    	address[0] =(unsigned int)(dma_phys);
    	address[1] =(unsigned int)(dma_virt);
    	return 0;
}

/*
 *	free_dma_lli_space
 */
int free_dmalli_space(unsigned int *ppheadlli, unsigned int page_num)
{
    	dma_addr_t dma_phys;
    	unsigned int dma_virt ;
    	dma_phys = (dma_addr_t)(ppheadlli[0]);
    	dma_virt = ppheadlli[1];
    	dma_free_coherent(NULL,page_num*PAGE_SIZE,(void *)dma_virt,dma_phys);
        ppheadlli[0] = 0;
        ppheadlli[1] = 0;
    	return 0;
}

/*
 *	Apply DMA interrupt resource
 *	init channel state
 */
int dma_driver_init(void)
{
    	unsigned int i;

    	dmac_init();
        
    	if(request_irq(DMAC_INT,&dmac_isr,IRQF_DISABLED,"Hisilicon Dmac",NULL))
    	{
        	printk("DMA Irq %d request failed\n", DMAC_INT);
        	return -1;
    	}

    	for(i=0;i<DMAC_MAX_CHANNELS;i++)
        {
    		g_channel_status[i] = DMAC_CHN_VACANCY;
        }

    	return 0;
}

/*
 *	config register for memory to memory DMA tranfer without LLI
 *	note:
 *             it is necessary to call dmac_channelstart for channel enable
 */
int dmac_start_m2m(unsigned int  channel, unsigned int psource, unsigned int pdest, unsigned int uwnumtransfers)
{
    	unsigned int uwchannel_num,tmp_trasnsfer,addtmp;
    	/*check input paramet*/
    	addtmp = psource;
    	if((mem_check_valid(addtmp) == -1)||(addtmp & 0x03))
    	{
        	printk( "Invalidate source address,address=%x \n",(unsigned int)psource);
        	return -EINVAL;
    	}

    	addtmp = pdest;
    	if((mem_check_valid(addtmp) == -1)||(addtmp&0x03))
    	{
        	printk(  "Invalidate destination address,address=%x \n",(unsigned int)pdest);
        	return -EINVAL;
    	}

    	if((uwnumtransfers> (MAXTRANSFERSIZE<<2))||(uwnumtransfers&0x3))
    	{
        	printk( "Invalidate transfer size,size=%x \n",uwnumtransfers);
        	return -EINVAL;
    	}

    	uwchannel_num = channel;
    	if((uwchannel_num == DMAC_CHANNEL_INVALID)|| (uwchannel_num >7))
    	{
        	printk( "failure of DMAC channel allocation in M2M function!\n ");
        	return -EFAULT;
    	}

    	dmac_writew (DMAC_CxCONFIG(uwchannel_num), DMAC_CxDISABLE);
    	dmac_writew (DMAC_CxSRCADDR(uwchannel_num), (unsigned int)psource);
    	dmac_writew (DMAC_CxDESTADDR(uwchannel_num),(unsigned int)pdest);
    	dmac_writew (DMAC_CxLLI(uwchannel_num), 0);
    	tmp_trasnsfer = (uwnumtransfers >> 2) & 0xfff;
    	tmp_trasnsfer = tmp_trasnsfer | (DMAC_CxCONTROL_M2M & (~0xfff));
    	dmac_writew (DMAC_CxCONTROL(uwchannel_num), tmp_trasnsfer);
    	dmac_writew (DMAC_CxCONFIG(uwchannel_num), DMAC_CxCONFIG_M2M);


    	return 0;
}

/*
 *	channel enable
 *	start a dma transfer immediately
 */
int dmac_channelstart(unsigned int u32channel)
{

    	unsigned int reg_value;
    	if(u32channel >= DMAC_MAX_CHANNELS)
    	{
        	printk(  "channel number is larger than or equal to DMAC_MAX_CHANNELS %d\n",DMAC_MAX_CHANNELS);
        	return -EINVAL;
    	}
    	g_channel_status[u32channel] = DMAC_NOT_FINISHED;
    	dmac_readw(DMAC_CxCONFIG(u32channel),reg_value);
    	dmac_writew(DMAC_CxCONFIG(u32channel),(reg_value|DMAC_CHANNEL_ENABLE));


    	return 0;
}


/*
 *	wait for transfer end
 */
int dmac_wait(unsigned int channel)
{
    	int ret_result;
    	ret_result = dmac_check_over(channel);
    	while(1)
    	{
        	if(ret_result == -DMAC_CHN_ERROR)
        	{
            		printk("DMAC Transfer Error.\n");
            		return ret_result;
        	}

        	else  if(ret_result == DMAC_NOT_FINISHED)
        	{
                udelay(100);
                ret_result = dmac_check_over(channel);
        	}

        	else if(ret_result == DMAC_CHN_SUCCESS)
        	{
            		return ret_result;
        	}

           	else if  (ret_result == DMAC_CHN_VACANCY)
        	{
            		return ret_result;
        	}

        	else if(ret_result == -DMAC_CHN_TIMEOUT)
        	{
            		printk("DMAC Transfer Error.\n");
            		dmac_writew (DMAC_CxCONFIG(channel),DMAC_CxDISABLE);
            		g_channel_status[channel] =DMAC_CHN_VACANCY;
            		return ret_result;
        	}
    	}
}

/*
 *	buile LLI for memory to memory DMA tranfer
 */
int dmac_buildllim2m(unsigned int *ppheadlli, unsigned int pdest, unsigned int psource, unsigned int totaltransfersize,
                     unsigned int uwnumtransfers)
{
	unsigned int lli_num =0;
	unsigned int last_lli =0;
	unsigned int address , phy_address,srcaddr,denstaddr;
	unsigned int j;

	lli_num =(totaltransfersize / uwnumtransfers);
	if((totaltransfersize % uwnumtransfers)!= 0)
    {
		last_lli=1,++lli_num;
    }
	if(ppheadlli != NULL)
	{
    	phy_address =(unsigned int)(ppheadlli[0]);
    	address =(unsigned int)(ppheadlli[1]);
    	for (j=0; j<lli_num; j++)
    	{
    		srcaddr =(psource + (j*uwnumtransfers));
    		dmac_writew(address, srcaddr);
            address += 4;
            phy_address += 4;
    		denstaddr=(pdest + (j*uwnumtransfers));
    		dmac_writew(address, denstaddr);
            address += 4;
            phy_address += 4;
    		if(j==(lli_num -1))
            {
    			dmac_writew(address, 0);
            }
    		else
            {
    			dmac_writew(address, (((phy_address+8)&(~0x03))|DMAC_CxLLI_LM));
            }
            address += 4;
            phy_address += 4;

    		if((j==(lli_num -1)) && (last_lli == 0))
            {
    			dmac_writew(address, ((DMAC_CxCONTROL_LLIM2M &(~0xfff))|(uwnumtransfers>>2)|0x80000000));
            }
    		else if((j==(lli_num -1)) && (last_lli == 1))
            {
                dmac_writew(address, ((DMAC_CxCONTROL_LLIM2M
                                       & (~0xfff)) | ((totaltransfersize % uwnumtransfers) >> 2) | 0x80000000));
            }
    		else
            {
    			dmac_writew(address, (((DMAC_CxCONTROL_LLIM2M&(~0xfff))|(uwnumtransfers>>2))&0x7fffffff));
            }
            address += 4;
            phy_address += 4;
    	}
	}
  	return 0;
}

/*
 *	disable channel
 *	used before the operation of register configuration
 */
int dmac_channelclose(unsigned int channel)
{
    	unsigned int reg_value,count;

    	if (channel >=DMAC_MAX_CHANNELS)
    	{
        	printk("\nCLOSE :channel number is larger than or equal to DMAC_CHANNEL_NUM_TOTAL.\n");
        	return -EINVAL;
    	}

    	dmac_readw(DMAC_CxCONFIG(channel),reg_value);

    	#define CHANNEL_CLOSE_IMMEDIATE
    #ifdef CHANNEL_CLOSE_IMMEDIATE
        reg_value &= 0xFFFFFFFE;
        dmac_writew(DMAC_CxCONFIG(channel) , reg_value);
    #else
        reg_value |= DMAC_CONFIGURATIONx_HALT_DMA_ENABLE;
        dmac_writew(DMAC_CxCONFIG(channel) , reg_value);     /*ignore incoming dma request*/
        dmac_readw(DMAC_CxCONFIG(channel),reg_value);
        while ((reg_value & DMAC_CONFIGURATIONx_ACTIVE) == DMAC_CONFIGURATIONx_ACTIVE) /*if FIFO is empty*/
        {
            	dmac_readw(DMAC_CxCONFIG(channel),reg_value);
        }
        reg_value &= 0xFFFFFFFE;
        dmac_writew(DMAC_CxCONFIG(channel) , reg_value);
    #endif

    	dmac_readw(DMAC_ENBLDCHNS,reg_value);
    	reg_value= reg_value&0x00ff;
        count = 0;
    	while (((reg_value >> channel) & 0x1) == 1 )
        {
    	    dmac_readw(DMAC_ENBLDCHNS,reg_value);
    	    reg_value= reg_value&0x00ff;
            if(count++ > 10000)
            {
                printk("channel close failure.\n");
                return -1;
            }
        }
    	return 0;
}

/*
 *	load configuration from LLI for memory to memory
 */
int dmac_start_llim2m(unsigned int channel, unsigned int *pfirst_lli)
{
    	unsigned int uwchannel_num;
    	dmac_lli  plli;
    	unsigned int first_lli;
    	if(NULL == pfirst_lli)
    	{
        	printk("Invalidate LLI head!\n");
        	return -EFAULT;
    	}

    	uwchannel_num = channel;
    	if((uwchannel_num == DMAC_CHANNEL_INVALID)|| (uwchannel_num >7))
    	{
        	printk("failure of DMAC channel allocation in LLIM2M function,channel=%x!\n ",uwchannel_num);
        	return -EINVAL;
    	}

    	memset(&plli,0,sizeof(plli));
    	first_lli =(unsigned int )pfirst_lli[1];
    	dmac_readw(first_lli,plli.src_addr);
    	dmac_readw(first_lli+4,plli.dst_addr);
    	dmac_readw(first_lli+8, plli.next_lli);
    	dmac_readw(first_lli+12,plli.lli_transfer_ctrl);

    	dmac_channelclose(uwchannel_num);
    	dmac_writew (DMAC_INTTCCLEAR, (0x1<<uwchannel_num));
    	dmac_writew (DMAC_INTERRCLR, (0x1<<uwchannel_num));
    	dmac_writew (DMAC_SYNC , 0x0);

    	dmac_writew(DMAC_CxCONFIG(uwchannel_num),DMAC_CxDISABLE);
    	dmac_writew (DMAC_CxSRCADDR(uwchannel_num), (unsigned int)(plli.src_addr));
    	dmac_writew (DMAC_CxDESTADDR (uwchannel_num),(unsigned int)(plli.dst_addr));
    	dmac_writew (DMAC_CxLLI (uwchannel_num),(unsigned int)(plli.next_lli));
    	dmac_writew (DMAC_CxCONTROL(uwchannel_num), (unsigned int)(plli.lli_transfer_ctrl));
    	dmac_writew(DMAC_CxCONFIG(uwchannel_num), DMAC_CxCONFIG_LLIM2M);

    	return 0;
}

/*
 *	load configuration from LLI for memory and peripheral
 */
int dmac_start_llim2p(unsigned int channel, unsigned int *pfirst_lli, unsigned int uwperipheralid)
{
    	unsigned int uwchannel_num;
    	dmac_lli   plli;
    	unsigned int first_lli;
        unsigned int temp = 0;

    	if(NULL == pfirst_lli)
    	{
        	printk("Invalidate LLI head!\n");
        	return -EINVAL;
    	}
    	uwchannel_num = channel;
    	if((uwchannel_num == DMAC_CHANNEL_INVALID)|| (uwchannel_num >7))
    	{
        	printk(" failure of DMAC channel allocation in LLIM2P function,channel=%x!\n ",uwchannel_num);
        	return -EINVAL;
    	}

    	memset(&plli,0,sizeof(plli));
    	first_lli =(unsigned int )pfirst_lli[1];
    	dmac_readw(first_lli,plli.src_addr);
    	dmac_readw(first_lli+4,plli.dst_addr);
    	dmac_readw(first_lli+8, plli.next_lli);
    	dmac_readw(first_lli+12,plli.lli_transfer_ctrl);

    	dmac_channelclose(uwchannel_num);
    	dmac_writew (DMAC_INTTCCLEAR, (0x1<<uwchannel_num));
    	dmac_writew (DMAC_INTERRCLR, (0x1<<uwchannel_num));
    	dmac_writew (DMAC_SYNC , 0x0);

        dmac_readw  (DMAC_CxCONFIG(uwchannel_num), temp);
    	dmac_writew (DMAC_CxCONFIG(uwchannel_num), temp|DMAC_CxDISABLE);
    	dmac_writew (DMAC_CxSRCADDR(uwchannel_num), plli.src_addr);
    	dmac_writew (DMAC_CxDESTADDR(uwchannel_num),plli.dst_addr);
    	dmac_writew (DMAC_CxLLI(uwchannel_num),plli.next_lli);
    	dmac_writew (DMAC_CxCONTROL(uwchannel_num),plli.lli_transfer_ctrl);

        dmac_readw  (DMAC_CxCONFIG(uwchannel_num), temp);
    dmac_writew (DMAC_CxCONFIG(uwchannel_num), temp
                 | ((g_peripheral[uwperipheralid].transfer_cfg) & DMAC_CHANNEL_DISABLE));

    	return 0;
}

/*
 *	enable memory and peripheral dma transfer
 *	note:
 *	       it is necessary to call dmac_channelstart to enable channel
 */
int dmac_start_m2p(unsigned int channel, unsigned int pmemaddr, unsigned int uwperipheralid,
        unsigned int  uwnumtransfers ,unsigned int next_lli_addr)
{

    	unsigned int uwchannel_num,uwtrans_control =0;
    	unsigned int addtmp,tmp;
    	unsigned int uwdst_addr = 0,uwsrc_addr = 0;
    	unsigned int uwwidth;
    	addtmp = pmemaddr;
    	if((mem_check_valid(addtmp) == -1)||(addtmp&0x3))
    	{
        	printk("Invalidate source address,address=%x \n",(unsigned int)pmemaddr);
        	return -EINVAL;
    	}

    	if((uwperipheralid>15))
    	{
        	printk("Invalidate peripheral id in M2P function, id=%x! \n",uwperipheralid);
        	return -EINVAL;
    	}

    	uwchannel_num = channel;
    	if((uwchannel_num == DMAC_CHANNEL_INVALID) || (uwchannel_num > 7) || ( uwchannel_num < 0))
    	{
        	printk("failure of DMAC channel allocation in M2P function\n");
        	return -EFAULT;
    	}

#if 0
    	if((DMAC_UART_TX_REQ == uwperipheralid)||(DMAC_UART_RX_REQ == uwperipheralid)\
        || (DMAC_SSP_TX_REQ == uwperipheralid) || (DMAC_SSP_RX_REQ == uwperipheralid))
    {
        uwwidth = 0;
    } \
    else if ((DMAC_SIO0_TX_REQ == uwperipheralid) || (DMAC_SIO0_RX_REQ == uwperipheralid) \
             || (DMAC_SIO1_RX_REQ == uwperipheralid))
    {
        uwwidth = 1;
    }
    	else
#endif
        	uwwidth=2;

        if(uwperipheralid>3)
        {
        	uwsrc_addr = pmemaddr;
        	uwdst_addr = (unsigned int)(g_peripheral[uwperipheralid].pperi_addr);
        }
        else
        {

        	uwsrc_addr=(unsigned int)(g_peripheral[uwperipheralid].pperi_addr);
        	uwdst_addr=pmemaddr;
        }

    	tmp= uwnumtransfers>>uwwidth;
    	if(tmp & (~0x0fff))
    	{
        	printk("Invalidate transfer size,size=%x! \n",uwnumtransfers);
        	return -EINVAL;
    	}
    	tmp = tmp &0xfff;
    	uwtrans_control = tmp|(g_peripheral[uwperipheralid].transfer_ctrl&(~0xfff));
        //printk("(in)uwdst_addr = %#x\n",(unsigned int)uwdst_addr);
    	dmac_writew(DMAC_INTTCCLEAR , (0x1<<uwchannel_num));
    	dmac_writew(DMAC_INTERRCLR , (0x1<<uwchannel_num));
    	dmac_writew(DMAC_CxSRCADDR(uwchannel_num), (unsigned int)uwsrc_addr);
    	dmac_writew(DMAC_CxDESTADDR(uwchannel_num),(unsigned int)uwdst_addr);
    	dmac_writew(DMAC_CxLLI(uwchannel_num), (unsigned int)next_lli_addr);
    	dmac_writew(DMAC_CxCONTROL(uwchannel_num), (unsigned int)uwtrans_control );
    	dmac_writew (DMAC_CxCONFIG(uwchannel_num), ((g_peripheral[uwperipheralid].transfer_cfg)&DMAC_CHANNEL_DISABLE));

    	return 0;
}


/*
 *	build LLI for memory to sio0
 *      called by dmac_buildllim2p
 */
void buildlli4m2sio0(unsigned int *ppheadlli,
                     unsigned int *pmemaddr,
                     unsigned int  uwperipheralid,
                     unsigned int  lli_num,
                     unsigned int  totaltransfersize,
                     unsigned int  uwnumtransfers,
                     unsigned int  uwwidth,
                     unsigned int  last_lli)
{
    unsigned int srcaddr, address, phy_address, j;

    phy_address = (unsigned int)(ppheadlli[0]);
    address = (unsigned int)(ppheadlli[1]);

    for (j = 0; j < lli_num; j++)
    {
        srcaddr = (pmemaddr[0] + (j * uwnumtransfers));
        dmac_writew(address, srcaddr);

        address += 4;
        phy_address += 4;
        dmac_writew(address, SIO0_TX_LEFT_FIFO);

        address += 4;
        phy_address += 4;
        dmac_writew(address, (((phy_address + 8) & (~0x03)) | DMAC_CxLLI_LM));

        address += 4;
        phy_address += 4;
        if ((j == (lli_num - 1)) && (last_lli == 1))
        {
            dmac_writew(address, ((g_peripheral[uwperipheralid].transfer_ctrl & (~0xfff)) | \
                                  (((totaltransfersize % uwnumtransfers) >> uwwidth) & 0x7fffffff)));
        }
        else
        {
            dmac_writew(address, ((g_peripheral[uwperipheralid].transfer_ctrl & (~0xfff)) | \
                                  (uwnumtransfers >> uwwidth)) & 0x7fffffff);
        }

        address += 4;
        phy_address += 4;
        srcaddr = (pmemaddr[1] + (j * uwnumtransfers));
        dmac_writew(address, srcaddr);

        address += 4;
        phy_address += 4;
        dmac_writew(address, SIO0_TX_RIGHT_FIFO);

        address += 4;
        phy_address += 4;
        if (j == (lli_num - 1))
        {
            dmac_writew(address, 0);
        }
        else
        {
            dmac_writew(address, (((phy_address + 8) & (~0x03)) | DMAC_CxLLI_LM));
        }

        address += 4;
        phy_address += 4;
        if ((j == (lli_num - 1)) && (last_lli == 0))
        {
            dmac_writew(address, ((g_peripheral[uwperipheralid].transfer_ctrl & (~0xfff)) | \
                                  (uwnumtransfers >> uwwidth) | 0x80000000));
        }
        else if ((j == (lli_num - 1)) && (last_lli == 1))
        {
            dmac_writew(address, ((g_peripheral[uwperipheralid].transfer_ctrl & (~0xfff)) | \
                                  ((totaltransfersize % uwnumtransfers) >> uwwidth) | 0x80000000));
        }
        else
        {
            dmac_writew(address, ((g_peripheral[uwperipheralid].transfer_ctrl & (~0xfff)) | \
                                  (uwnumtransfers >> uwwidth)) & 0x7fffffff);
        }

        address += 4;
        phy_address += 4;
    	}
}


/*
 *	build LLI for sio0 to memory
 *      called by dmac_buildllim2p
 */
void buildlli4sio02m(unsigned int *ppheadlli,
                     unsigned int *pmemaddr,
                     unsigned int  uwperipheralid,
                     unsigned int  lli_num,
                     unsigned int  totaltransfersize,
                     unsigned int  uwnumtransfers,
                     unsigned int  uwwidth,
                     unsigned int  last_lli)
{
    unsigned int srcaddr, address, phy_address, j;

    phy_address = (unsigned int)(ppheadlli[0]);
    address = (unsigned int)(ppheadlli[1]);
    srcaddr = (pmemaddr[0]);

    for (j = 0; j < lli_num; j++)
    {
        dmac_writew(address, SIO0_RX_LEFT_FIFO);
        address += 4;
        phy_address += 4;
        srcaddr = (pmemaddr[0] + (j * uwnumtransfers));
        dmac_writew(address, srcaddr);
        address += 4;
        phy_address += 4;
        dmac_writew(address, (((phy_address + 8) & (~0x03)) | DMAC_CxLLI_LM));
        address += 4;
        phy_address += 4;
        if ((j == (lli_num - 1)) && (last_lli == 1))
        {
            dmac_writew(address, ((g_peripheral[uwperipheralid].transfer_ctrl & (~0xfff)) \
                                  | (((totaltransfersize % uwnumtransfers) >> uwwidth) & 0x7fffffff)));
        }
        else
        {
            dmac_writew(address, ((g_peripheral[uwperipheralid].transfer_ctrl & (~0xfff)) | \
                                  (uwnumtransfers >> uwwidth)) & 0x7fffffff);
        }

        address += 4;
        phy_address += 4;
        dmac_writew(address, SIO0_RX_RIGHT_FIFO);
        address += 4;
        phy_address += 4;
        srcaddr = (pmemaddr[1] + (j * uwnumtransfers));
        dmac_writew(address, srcaddr);
        address += 4;
        phy_address += 4;
        if (j == (lli_num - 1))
        {
            dmac_writew(address, 0);
        }
        else
        {
            dmac_writew(address, (((phy_address + 8) & (~0x03)) | DMAC_CxLLI_LM));
        }

        address += 4;
        phy_address += 4;

        if ((j == (lli_num - 1)) && (last_lli == 0))
        {
            dmac_writew(address, ((g_peripheral[uwperipheralid].transfer_ctrl & (~0xfff)) \
                                  | (uwnumtransfers >> uwwidth) | 0x80000000));
        }
        else if ((j == (lli_num - 1)) && (last_lli == 1))
        {
            dmac_writew(address, ((g_peripheral[uwperipheralid].transfer_ctrl & (~0xfff)) \
                                  | ((totaltransfersize % uwnumtransfers) >> uwwidth) | 0x80000000));
        }
        else
        {
            dmac_writew(address, ((g_peripheral[uwperipheralid].transfer_ctrl & (~0xfff)) | \
                                  (uwnumtransfers >> uwwidth)));
        }

        address += 4;
        phy_address += 4;
    }
}

/*
 *	build LLI for sio1 to memory
 *      called by dmac_buildllim2p
 */
#if HICHIP!=0x35180100
void buildlli4sio12m(unsigned int *ppheadlli,
                     unsigned int *pmemaddr,
                     unsigned int  uwperipheralid,
                     unsigned int  lli_num,
                     unsigned int  totaltransfersize,
                     unsigned int  uwnumtransfers,
                     unsigned int  uwwidth,
                     unsigned int  last_lli)
{
    unsigned int srcaddr, address, phy_address, j;

    phy_address = (unsigned int)(ppheadlli[0]);
    address = (unsigned int)(ppheadlli[1]);
    srcaddr = (pmemaddr[0]);

    for (j = 0; j < lli_num; j++)
    {
        dmac_writew(address, SIO1_RX_LEFT_FIFO);
        address += 4;
        phy_address += 4;
        srcaddr = (pmemaddr[0] + (j * uwnumtransfers));
        dmac_writew(address, srcaddr);
        address += 4;
        phy_address += 4;
        dmac_writew(address, (((phy_address + 8) & (~0x03)) | DMAC_CxLLI_LM));
        address += 4;
        phy_address += 4;
        if ((j == (lli_num - 1)) && (last_lli == 1))
        {
            dmac_writew(address, ((g_peripheral[uwperipheralid].transfer_ctrl & (~0xfff)) \
                                  | (((totaltransfersize % uwnumtransfers) >> uwwidth) & 0x7fffffff)));
        }
        else
        {
            dmac_writew(address, ((g_peripheral[uwperipheralid].transfer_ctrl & (~0xfff)) | \
                                  (uwnumtransfers >> uwwidth)) & 0x7fffffff);
        }

        address += 4;
        phy_address += 4;
        dmac_writew(address, SIO1_RX_RIGHT_FIFO);
        address += 4;
        phy_address += 4;
        srcaddr = (pmemaddr[1] + (j * uwnumtransfers));
        dmac_writew(address, srcaddr);
        address += 4;
        phy_address += 4;
        if (j == (lli_num - 1))
        {
            dmac_writew(address, 0);
        }
        else
        {
            dmac_writew(address, (((phy_address + 8) & (~0x03)) | DMAC_CxLLI_LM));
        }

        address += 4;
        phy_address += 4;

        if ((j == (lli_num - 1)) && (last_lli == 0))
        {
            dmac_writew(address, ((g_peripheral[uwperipheralid].transfer_ctrl & (~0xfff)) \
                                  | (uwnumtransfers >> uwwidth) | 0x80000000));
        }
        else if ((j == (lli_num - 1)) && (last_lli == 1))
        {
            dmac_writew(address, ((g_peripheral[uwperipheralid].transfer_ctrl & (~0xfff)) \
                                  | ((totaltransfersize % uwnumtransfers) >> uwwidth) | 0x80000000));
        }
        else
        {
            dmac_writew(address, ((g_peripheral[uwperipheralid].transfer_ctrl & (~0xfff)) | \
                                  (uwnumtransfers >> uwwidth)));
        }

        address += 4;
        phy_address += 4;
    }
}
#endif

/*
 *	build LLI for memory to peripheral
 *   2010-05-20 Add function of circle lli, by p00123320
 */
int  dmac_buildllim2p( unsigned int *ppheadlli, unsigned int *pmemaddr,
                       unsigned int uwperipheralid, unsigned int totaltransfersize,
                       unsigned int uwnumtransfers, unsigned int burstsize)
{
    unsigned int lli_cnt;
    unsigned int address = 0, phy_address = 0, destaddr = 0, srcaddr = 0;
    unsigned int uwwidth = 0, lli_num = 0, last_lli = 0;
    unsigned int i, j = 0;
    unsigned int uMaxid = 0;
    lli_cnt = (2 == burstsize) ? burstsize : 1;     /* borrow parameter(only value 2 used for circel lli) */

#if 0
    for (i = 0; i < lli_cnt; i++)
    {
        //printk("%s -> mem_addr[%d]:0x%x \n", __FUNCTION__, i, pmemaddr[i]);
        if ((mem_check_valid(pmemaddr[i]) == -1) || (pmemaddr[i] & 0x3))
        {
            printk("Invalidate source address,address=%x \n", (unsigned int)pmemaddr[0]);
            return -EINVAL;
        }
    }
#endif


    if (uwperipheralid > 15)
    {
        printk("Invalidate peripheral id %x! \n", uwperipheralid);
        return -EINVAL;
    }

#if 0
    if (((DMAC_SSP_TX_REQ == uwperipheralid) || (DMAC_SSP_RX_REQ == uwperipheralid)))
    {
        uwwidth = 0;
    }
    else
#endif
    {
        uwwidth = 2;
    }

    if ((uwnumtransfers > (MAXTRANSFERSIZE << uwwidth)))
    {
        printk("Invalidate transfer size,size=%x \n", uwnumtransfers);
        return -EINVAL;
    }

    lli_num = (totaltransfersize / uwnumtransfers);
    if ((totaltransfersize % uwnumtransfers) != 0)
    {
        last_lli = 1, ++lli_num;

        //printk("Notice-> totaltransfersize:%d, uwnumtransfers:%d,  uwperipheralid:%d \n",
        //    totaltransfersize, uwnumtransfers, uwperipheralid);
    }

    phy_address = (unsigned int)(ppheadlli[0]);
    address = (unsigned int)(ppheadlli[1]);

    /*memory to peripheral*/
#if (HICHIP==0x35310100 || HICHIP==0x35320100)
    uMaxid = 4;
#elif (HICHIP==0x35210100 || HICHIP==0x3520A100)
    uMaxid = 2;
#endif

    /* for hi3518 DMAC_SIO0_RX_REQ=0 (p2m), DMAC_SIO0_TX_REQ=1 (m2p) */
    if (uwperipheralid > uMaxid)
    {
        /*create lli for sio*/
        for (i = 0; i < lli_cnt; i++)
        {
            for (j = 0; j < lli_num; j++)
            {
                /* 1) src addr */
                srcaddr = (pmemaddr[i] + (j * uwnumtransfers));
                dmac_writew(address, srcaddr);
                address += 4;
                phy_address += 4;

                /* 2) dest addr */
                dmac_writew(address, (unsigned int)(g_peripheral[uwperipheralid].pperi_addr));
                address += 4;
                phy_address += 4;

                /* 3) next lli */
                if (j == (lli_num - 1))
                {
                    /* the last node */
                    if (lli_cnt == 1)
                    {
                        /* only one lli */
                        dmac_writew(address, 0);
                    }
                    else
                    {
                        /* muti circle lli */
                        unsigned int next_addr = (i == lli_cnt - 1) ? (ppheadlli[0]) : (phy_address + 8);
                        dmac_writew(address, ((next_addr) & (~0x03)) | DMAC_CxLLI_LM);
                    }
                }
                else
                {
                    dmac_writew(address, (((phy_address + 8) & (~0x03)) | DMAC_CxLLI_LM));
                }

                address += 4;
                phy_address += 4;

                /* 4) control */
                if ((j == (lli_num - 1)) && (last_lli == 0))
                {
                    dmac_writew(address, ((g_peripheral[uwperipheralid].transfer_ctrl
                                           & (~0xfff)) | (uwnumtransfers >> uwwidth) | 0x80000000));
                }
                else if ((j == (lli_num - 1)) && (last_lli == 1))
                {
                    dmac_writew(address, ((g_peripheral[uwperipheralid].transfer_ctrl\
                                           & (~0xfff)) | ((totaltransfersize % uwnumtransfers) \
                                                          >> uwwidth)
                                          | 0x80000000));
                }
                else
                {
                    dmac_writew(address, ((g_peripheral[uwperipheralid].transfer_ctrl
                                           & (~0xfff)) | (uwnumtransfers >> uwwidth)) & 0x7fffffff);
                }

                address += 4;
                phy_address += 4;
            }
        }
    }
    /*peripheral to memory*/
    else
    {
        /*create lli for sio*/
        for (i = 0; i < lli_cnt; i++)
        {
            for (j = 0; j < lli_num; j++)
            {
                /* 1) src addr */
                dmac_writew(address, (unsigned int)(g_peripheral[uwperipheralid].pperi_addr));
                address += 4;
                phy_address += 4;

                /* 2) dest addr */
                destaddr = (pmemaddr[i] + (j * uwnumtransfers));
                dmac_writew(address, destaddr);
                address += 4;
                phy_address += 4;

                /* 3) next lli */
                if (j == (lli_num - 1))
                {
                    /* the last node */
                    if (lli_cnt == 1)
                    {
                        /* only one lli */
                        dmac_writew(address, 0);
                    }
                    else
                    {
                        /* muti circle lli */
                        unsigned int next_addr = (i == lli_cnt - 1) ? (ppheadlli[0]) : (phy_address + 8);
                        dmac_writew(address, ((next_addr) & (~0x03)) | DMAC_CxLLI_LM);
                    }
                }
                else
                {
                    dmac_writew(address, (((phy_address + 8) & (~0x03)) | DMAC_CxLLI_LM));
                }

                address += 4;
                phy_address += 4;

                /* 4) control */
                if ((j == (lli_num - 1)) && (last_lli == 0))
                {
                    dmac_writew(address, ((g_peripheral[uwperipheralid].transfer_ctrl
                                           & (~0xfff)) | (uwnumtransfers >> uwwidth) | 0x80000000));
                }
                else if ((j == (lli_num - 1)) && (last_lli == 1))
                {
                    dmac_writew(address, ((g_peripheral[uwperipheralid].transfer_ctrl\
                                           & (~0xfff)) | ((totaltransfersize % uwnumtransfers) \
                                                          >> uwwidth)
                                          | 0x80000000));
                }
                else
                {
                    dmac_writew(address, ((g_peripheral[uwperipheralid].transfer_ctrl
                                           & (~0xfff)) | (uwnumtransfers >> uwwidth)) & 0x7fffffff);
                }

                address += 4;
                phy_address += 4;
            }
        }
    }

    return 0;
}

/*
 *	execute memory to memory dma transfer without LLI
 */
int dmac_m2m_transfer(unsigned int *psource, unsigned int *pdest, unsigned int uwtransfersize)
{
    unsigned int ulchnn, dma_size = 0;
    unsigned int dma_count, left_size;

    left_size = uwtransfersize;
    dma_count = 0;
    ulchnn = dmac_channel_allocate(NULL);
    if (DMAC_CHANNEL_INVALID == ulchnn)
    {
		printk("dmac_channel_allocate failed and return\n");
        return -1;
    }

    dma_size = 0xffc;
    while ((left_size >> 2) >= 0xffc)
    {
        left_size -= dma_size * 4;
        dmac_start_m2m(ulchnn, (unsigned int)(psource + dma_count * dma_size), (unsigned int)(pdest + dma_count * dma_size), (dma_size << 2));
        if (dmac_channelstart(ulchnn) != 0)
        {
			printk("dmac_channelstart failed and return\n");
            dmac_channel_free(ulchnn);
            return -1;
        }

        if (dmac_wait(ulchnn) != DMAC_CHN_SUCCESS)
        {
			printk("dmac_wait failed and return\n");
            dmac_channel_free(ulchnn);
            return -1;
        }

        dma_count++;
    }

	//printk("dma_count=%d left_size=%d\n",dma_count,left_size);
    dmac_start_m2m(ulchnn, (unsigned int)(psource + dma_count * dma_size), (unsigned int)(pdest + dma_count * dma_size), left_size);
    if (dmac_channelstart(ulchnn) != 0)
    {
        dmac_channel_free(ulchnn);
        return -1;
    }

    if (dmac_wait(ulchnn) != DMAC_CHN_SUCCESS)
    {
        dmac_channel_free(ulchnn);
        return -1;
    }
    dmac_channel_free(ulchnn);

    return 0;
}

/*
 *	execute memory to peripheral dma transfer without LLI
 */
int dmac_m2p_transfer(unsigned int *pmemaddr, unsigned int uwperipheralid, unsigned int uwtransfersize)
{
    unsigned int ulchnn, dma_size = 0;
    unsigned int dma_count, left_size, uwwidth;

    left_size = uwtransfersize;
    dma_count = 0;

    ulchnn = dmac_channel_allocate(NULL);
    if (DMAC_CHANNEL_INVALID == ulchnn)
    {
        return -1;
    }

    //printk("ulchnn=%d\n",ulchnn);

#if 0
    if ((DMAC_UART0_TX_REQ == uwperipheralid) || (DMAC_UART0_RX_REQ == uwperipheralid) \
        || (DMAC_SSP_TX_REQ == uwperipheralid) || (DMAC_SSP_RX_REQ == uwperipheralid))
    {
        uwwidth = 0;
    }
    else
    {
        uwwidth = 2;
    }

    if (uwtransfersize > (MAXTRANSFERSIZE << uwwidth)) //3FFC
    {
        printk("Invalidate transfer size,size=%x \n", uwtransfersize);
        return -EINVAL;
    }
#endif


    uwwidth = 2;
    while ((left_size >> uwwidth) >= 0xffc)
    {
        dma_size   = 0xffc;
        left_size -= dma_size * 2 * uwwidth;
        dmac_start_m2p(ulchnn, (unsigned int)(pmemaddr + dma_count * dma_size), uwperipheralid, (dma_size << 2), 0);
        if (dmac_channelstart(ulchnn) != 0)
        {
            return -1;
        }

        if (dmac_wait(ulchnn) != DMAC_CHN_SUCCESS)
        {
            return -1;
        }

        dma_count++;
    }

    //printk("pmemaddr=0x%x \n",(unsigned int)(pmemaddr + dma_count * dma_size));
    dmac_start_m2p(ulchnn, (unsigned int)(pmemaddr + dma_count * dma_size), uwperipheralid, left_size, 0);

    if (dmac_channelstart(ulchnn) != 0)
    {
        return -1;
    }

    if (dmac_wait(ulchnn) != 0)
    {
        return -1;
    }

    dmac_channel_free(ulchnn);
    return 0;
}


static int __init dmac_module_init(void)
{
    	int ret;
        #if 0
    	ret = kcom_hidmac_register();
        if(0 != ret)
    	    return -1;
        #endif
    	ret = dma_driver_init();
    	if(0 != ret)
    	    return -1;
    	//printk(KERN_INFO OSDRV_MODULE_VERSION_STRING "\n");
    	return 0;
}

static void __exit dma_module_exit(void)
{
    	free_irq(DMAC_INT, NULL);
    	//kcom_hidmac_unregister();
        iounmap((void*)reg_dmac_base_va);
}


module_init(dmac_module_init);
module_exit(dma_module_exit);


//module_param(sio4_mode, int, S_IRUGO);

EXPORT_SYMBOL(dmac_channelclose);

EXPORT_SYMBOL(dmac_register_isr);

EXPORT_SYMBOL(dmac_channel_free);

EXPORT_SYMBOL(free_dmalli_space);

EXPORT_SYMBOL(dmac_start_llim2p);

EXPORT_SYMBOL(dmac_buildllim2m);

EXPORT_SYMBOL(dmac_channelstart);

EXPORT_SYMBOL(dmac_start_llim2m);

EXPORT_SYMBOL(dmac_channel_allocate);

EXPORT_SYMBOL(allocate_dmalli_space);

EXPORT_SYMBOL(dmac_buildllim2p);

EXPORT_SYMBOL(dmac_start_m2p);

EXPORT_SYMBOL(dmac_start_m2m);

EXPORT_SYMBOL(dmac_wait);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hisilicon");
MODULE_VERSION("HI_VERSION=" OSDRV_MODULE_VERSION_STRING);

