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

#include <linux/module.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/fcntl.h>

#include <linux/init.h>
#include <linux/delay.h>

#include <asm/uaccess.h>
#include <asm/system.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>

#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/types.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/kthread.h>



#define IRQ_GPIO            46

#define GPIO_11_BASE 0x20150000

#define GPIO_11_DIR             IO_ADDRESS(GPIO_11_BASE + 0x400)
#define GPIO_IS                 IO_ADDRESS(GPIO_11_BASE + 0x404)
#define GPIO_IEV                IO_ADDRESS(GPIO_11_BASE + 0x40C)
#define GPIO_IBE                IO_ADDRESS(GPIO_11_BASE + 0x408)
#define GPIO_IE                 IO_ADDRESS(GPIO_11_BASE + 0x410)
#define GPIO_RIS                IO_ADDRESS(GPIO_11_BASE + 0x414)
#define GPIO_IC                 IO_ADDRESS(GPIO_11_BASE + 0x41C)

#define HW_REG(reg)         *((volatile unsigned int *)(reg))
#define muxctrl_reg111           IO_ADDRESS(0x200F01BC)
#define muxctrl_reg112           IO_ADDRESS(0x200F01C0)



static int has_motion_detection_irq = 0;
static wait_queue_head_t motion_detection_event;
extern MOT_DET_INFO _mot_det_info[2][MAX_DECODERS];
extern CX_COMMUNICATION cx26828_interface[2];
extern int motion_detection_standard[2][MAX_DECODERS];
static int queryMotionDetectionInterrupt(void);
static volatile int prev_motion_detected_flag = 0;

/*
 *  interrupt function
 */
static irqreturn_t gpio_interrupt(int irq, void *dev_id)
{
    int handled;
    int value = 0;
    
    has_motion_detection_irq = 0; 

    value = HW_REG(GPIO_RIS);

    if ((value & 0x1) != 0x0)//GPIO11_0 interrupt
    {       
        has_motion_detection_irq |= 0x1;
        wake_up_interruptible(&(motion_detection_event));
    }

    if ((value & 0x2) != 0x0)//GPIO11_1 interrupt
    {    
        has_motion_detection_irq |= 0x2;
        wake_up_interruptible(&(motion_detection_event));
    }

    //Clear interrupt flag.
    HW_REG(GPIO_IC) = 0xFF;
    handled = 1;

    HW_REG(GPIO_IE) = 0x0;


    return IRQ_RETVAL(handled);
}

static int enableMotionDetectionIrq(void)
{
    HW_REG(GPIO_IC) = 0xFF;

    //Enable GPIO11_0 and GPIO11_1.
    HW_REG(GPIO_IE) = 0x3;

    return 0;
}

static int initializeMotionDetectionIrq(void)
{
    int ret;
    int value = 0;

    //Set the GPIO11_0 and GPIO11_1 as the GPIO function.
    HW_REG(muxctrl_reg111) = 0x0;
    HW_REG(muxctrl_reg112) = 0x0;

    //Set the GPIO11_0, GPIO11_1 as input mode.
    value = HW_REG(GPIO_11_DIR);
    value &= ~(0x1 << 0);
    value &= ~(0x1 << 1);
    HW_REG(GPIO_11_DIR) = value;

    //Set the interrupt source as the sensitive mode.
    HW_REG(GPIO_IS) = 0x3;

    //Set the low-level sensitive mode.
    HW_REG(GPIO_IEV) = 0x3;

    //Set the single edge mode.
    HW_REG(GPIO_IBE) = 0x0;

    //Disable GPIO interrupt.
    HW_REG(GPIO_IE) = 0x0;

    //Clear interrupt flag.
    HW_REG(GPIO_IC) = 0xFF;

    //request irq
    ret = request_irq(IRQ_GPIO, &gpio_interrupt, 0, \
              "Motion detection interrupt", NULL);
    if (0 != ret)
    {
        printk(" hi3521 cx26828: failed to register IRQ_GPIO(%d), ret = %d\n", IRQ_GPIO, ret);
        goto IRQ_GPIO_failed;
    }

    return 0;

IRQ_GPIO_failed:
    return ret;
}


static int disableMotionDetectionIrq(void)
{
    //Disable  GPIO11_0 and GPIO11_1.
    HW_REG(GPIO_IE) = 0x0;

    return 0;
}


static void releaseMotionDetectionIrq(void)
{
    free_irq(IRQ_GPIO, NULL);
    return;
}


void setMotionDetectBlocks(int chip)
{
    int i = 0, j;
    for (i = 0; i < MAX_NUM_BLK; i++)
    {
        // Enable all region blocks for motion detection
        for (j = 0; j < MAX_DECODERS; j++)
            _mot_det_info[chip][j].mot_det_blk_sel[i] = 1;
    }
}

int setMotionDetectThreshold(PMOT_DET_INFO mot_det_data)
{
    int i, chip;

    if (!mot_det_data)
        return 0;
    chip = mot_det_data->chip;

    for (i = 0; i < MAX_DECODERS; i++)
    {
        _mot_det_info[chip][i].enable[i]                 = mot_det_data->enable[i];
        _mot_det_info[chip][i].mot_det_enable            = (mot_det_data->no_mot_det_enable == 1) ? 0 : mot_det_data->enable[i]; 
        _mot_det_info[chip][i].mot_det_sens_thresh       = mot_det_data->mot_det_sens_thresh;
        _mot_det_info[chip][i].mot_det_block_thresh      = mot_det_data->mot_det_block_thresh;
        
        _mot_det_info[chip][i].no_mot_det_enable         = mot_det_data->no_mot_det_enable; 
        _mot_det_info[chip][i].no_mot_det_still_thresh   = mot_det_data->no_mot_det_still_thresh;
        _mot_det_info[chip][i].no_mot_det_sens_thresh    = mot_det_data->no_mot_det_sens_thresh;
        _mot_det_info[chip][i].no_mot_det_block_thresh   = mot_det_data->no_mot_det_block_thresh;
        
        _mot_det_info[chip][i].black_det_enable          = mot_det_data->black_det_enable; 
        _mot_det_info[chip][i].black_pixel_count_thresh  = mot_det_data->black_pixel_count_thresh;
        _mot_det_info[chip][i].black_pixel_det_thresh    = mot_det_data->black_pixel_det_thresh;
        
        _mot_det_info[chip][i].white_det_enable          = mot_det_data->white_det_enable; 
        _mot_det_info[chip][i].white_pixel_count_thresh  = mot_det_data->white_pixel_count_thresh;
        _mot_det_info[chip][i].white_pixel_det_thresh    = mot_det_data->white_pixel_det_thresh;
        
        _mot_det_info[chip][i].mot_det_skip_n_frames     = mot_det_data->mot_det_skip_n_frames;
    }

    return 0;
}

int initializeMotionDetectionParameters(const CX_COMMUNICATION *p_comm, int chip, int * video_standard)
{
    int ch = 0, j;
    unsigned long mot_intr_msk = 0;

    memset(&_mot_det_info[chip], 0, sizeof(_mot_det_info[chip]));


    for (j = 0; j < MAX_DECODERS; j++)
    {
        _mot_det_info[chip][j].chip = chip;
        _mot_det_info[chip][j].ch = j;

        memset(_mot_det_info[chip][j].mot_det_blk_sel, 0, MAX_NUM_BLK);
        memset(_mot_det_info[chip][j].mot_det_blk, 0, MAX_NUM_BLK);

        // Initialize threshold values
        _mot_det_info[chip][j].black_pixel_count_thresh = 0x11940;
        _mot_det_info[chip][j].black_pixel_det_thresh = 0x3E;

        _mot_det_info[chip][j].white_pixel_count_thresh = 0x11940;
        _mot_det_info[chip][j].white_pixel_det_thresh = 0xC8;

        _mot_det_info[chip][j].no_mot_det_still_thresh = 20;
        _mot_det_info[chip][j].no_mot_det_sens_thresh = 10;
        _mot_det_info[chip][j].no_mot_det_block_thresh = 20;

        _mot_det_info[chip][j].mot_det_sens_thresh = 3;
        _mot_det_info[chip][j].mot_det_block_thresh = 4;

        //Enable motion detection.
        _mot_det_info[chip][j].mot_det_enable = 1;
        _mot_det_info[chip][j].no_mot_det_enable = 0;
        _mot_det_info[chip][j].black_det_enable = 0;
        _mot_det_info[chip][j].white_det_enable = 0;
        
        // Motion Detection Status
        _mot_det_info[chip][j].mot_stat0 = 0;                     
        _mot_det_info[chip][j].mot_stat1 = 0;
        _mot_det_info[chip][j].mot_stat2 = 0;
        _mot_det_info[chip][j].mot_stat3 = 0;
        _mot_det_info[chip][j].mot_stat4 = 0;
        _mot_det_info[chip][j].mot_stat5 = 0;
        _mot_det_info[chip][j].mot_stat6 = 0;
        _mot_det_info[chip][j].mot_stat7 = 0;
        
        _mot_det_info[chip][j].mot_det_skip_n_frames = 0;
        
    }

    setMotionDetectBlocks( chip );


    for (ch = 0; ch < MAX_DECODERS; ch++)
    {
        motion_detection_standard[chip][ch] = video_standard[ch];
        ALCS_initializeMotionDetect( p_comm, &_mot_det_info[chip][ch], video_standard[ch], ch );
        ALCS_startMotionDetect( p_comm, &_mot_det_info[chip][ch], ch, &mot_intr_msk);
    }

    return 0;
}


int initializeMotionDetectionDevice(void)
{
    return initializeMotionDetectionIrq();
}

void releaseMotionDetection(void)
{
    releaseMotionDetectionIrq();
}

static int queryMotionDetectionInterrupt(void)
{
    int i = 0, j;
    int result = 0;
    int wait_event_result = -1;
    int wait_event_timeout_result = -1;
    unsigned long int_status0 = 0;


    init_waitqueue_head(&motion_detection_event);
    has_motion_detection_irq = 0;
    enableMotionDetectionIrq();

    
    if (prev_motion_detected_flag == 1)
    {
        wait_event_timeout_result = wait_event_interruptible_timeout(motion_detection_event,has_motion_detection_irq != 0, HZ/2);
 
        if (wait_event_timeout_result == -ERESTARTSYS || has_motion_detection_irq == 0) //has timed-out
        {
            wait_event_timeout_result = 0; 
    
            disableMotionDetectionIrq();         
            prev_motion_detected_flag = 0; 
        }          
    }
    else
    {
        wait_event_result = wait_event_interruptible(motion_detection_event, has_motion_detection_irq != 0);
    }
    
    if (wait_event_result >= 0 || wait_event_timeout_result > 0)
    {      
        disableMotionDetectionIrq(); 
        
        prev_motion_detected_flag = 0;

        switch(has_motion_detection_irq)
        {
            case 2:            
                result &= readDword( &cx26828_interface[0], ALCS_INT_STAT0, &int_status0 );                                
                int_status0 = (int_status0 & 0xFF00) >> 8;
                    
                //Wake up by GPIO11_1 interrupt
                for (i = 0; i < MAX_DECODERS; i++)
                {                            
                    if ( (_mot_det_info[0][i].enable[i] == 1) && (getBit(int_status0, i) == 1) ) 
                    {                    
                        ALCS_processMotionDetect(&cx26828_interface[0],
                                                 &_mot_det_info[0][i],
                                                 motion_detection_standard[0][i],
                                                 i); 
                            
                        if ((_mot_det_info[0][i].mot_num_blks_detected > 0) ||
                            (_mot_det_info[0][i].no_mot_detected == 1) )
                            prev_motion_detected_flag = 1; 
                    }
                }

                break;

            case 1:                   
                result &= readDword( &cx26828_interface[1], ALCS_INT_STAT0, &int_status0 );                                
                int_status0 = (int_status0 & 0xFF00) >> 8;
                
                //Wake up by GPIO11_0 interrupt
                for (i = 0; i < MAX_DECODERS; i++)
                {                        
                    if ( (_mot_det_info[1][i].mot_det_enable == 1) && (getBit(int_status0, i) == 1) )
                    {                        
                        ALCS_processMotionDetect(&cx26828_interface[1],
                                                 &_mot_det_info[1][i],
                                                 motion_detection_standard[1][i],
                                                 i);
                            
                        if ((_mot_det_info[1][i].mot_num_blks_detected > 0) ||
                            (_mot_det_info[1][i].no_mot_detected == 1) )
                            prev_motion_detected_flag = 1; 
                    }
                }

                break;

            case 3:                   
                //Wake up by GPIO11_0 and GPIO11_1 interrupt
                for (i = 0; i < 2; i++)
                {
                    result &= readDword( &cx26828_interface[i], ALCS_INT_STAT0, &int_status0 );                                
                    int_status0 = (int_status0 & 0xFF00) >> 8;
                
                    for (j = 0; j < MAX_DECODERS; j++)
                    {                        
                        if ( (_mot_det_info[i][j].mot_det_enable == 1) && (getBit(int_status0, j) == 1) )
                        {
                            ALCS_processMotionDetect(&cx26828_interface[i],
                                                     &_mot_det_info[i][j],
                                                     motion_detection_standard[i][j],
                                                     j); 
                            
                            if ((_mot_det_info[i][j].mot_num_blks_detected > 0) ||
                                (_mot_det_info[i][j].no_mot_detected == 1))
                                prev_motion_detected_flag = 1; 
                        }
                    }
                }

                break;
        }
    }

    has_motion_detection_irq = 0;
    
    return prev_motion_detected_flag;
}


