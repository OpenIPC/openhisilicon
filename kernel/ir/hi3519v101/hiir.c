/* interface/hiir/hiir.c
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
 * History:
 *      16-12-2006 Start of Hi3560 CPU support
 */


#include "hi_osal.h"

#include "hiir.h"
#include "hiir_codedef.h"

#ifndef NULL
#define NULL  ((void *)0)
#endif

/* irq */

#define HIIR_DEVICE_IRQ_NO 47
unsigned int ir_irq = 0;

/* io */
#define IR_REG_BASE 0x120f0000
#define IR_REG_LENGTH 0x1000
volatile void *pIrRegBase = NULL;
#define IR_ENABLE   (pIrRegBase + 0x00)
#define IR_CONFIG   (pIrRegBase + 0x04)
#define CNT_LEADS   (pIrRegBase + 0x08)
#define CNT_LEADE   (pIrRegBase + 0x0c)
#define CNT_SLEADE  (pIrRegBase + 0x10)
#define CNT0_B      (pIrRegBase + 0x14)
#define CNT1_B      (pIrRegBase + 0x18)
#define IR_BUSY     (pIrRegBase + 0x1c)
#define IR_DATAH    (pIrRegBase + 0x20)
#define IR_DATAL    (pIrRegBase + 0x24)
#define IR_INTM     (pIrRegBase + 0x28)
#define IR_INTS     (pIrRegBase + 0x2c)
#define IR_INTC     (pIrRegBase + 0x30)
#define IR_START    (pIrRegBase + 0x34)

//#define IOCONFIG  (OSAL_IO_ADDRESS(0x200f01F8)) //TODO: ASIC config pin share

/* interrupt mask */
#define INTMS_RCV      (1L << 16)
#define INTMS_FRAMERR  (1L << 17)
#define INTMS_OVERFLOW (1L << 18)
#define INTMS_RELEASE  (1L << 19)

/* define macro */
#define WRITE_REG(Addr, Value) ((*(volatile unsigned int *)(Addr)) = (Value))
#define READ_REG(Addr)         (*(volatile unsigned int *)(Addr))

/* debug */
static int g_dbg_flag = 0;
#define HIIR_PFX "hiir: "
#define hiir_dbg(params...) \
    do{ \
        if(g_dbg_flag) \
        { \
            osal_printk(params); \
        } \
    }while(0);
#define hiir_show_reg() \
    do{ \
        if(g_dbg_flag) \
        { \
            osal_printk("HIIR11 REG:###################################################\n"); \
            osal_printk("%.8x ", READ_REG(IR_ENABLE)); \
            osal_printk("%.8x ", READ_REG(IR_CONFIG)); \
            osal_printk("%.8x ", READ_REG(CNT_LEADS)); \
            osal_printk("%.8x ", READ_REG(CNT_LEADE)); \
            osal_printk("%.8x ", READ_REG(CNT_SLEADE)); \
            osal_printk("%.8x ", READ_REG(CNT0_B)); \
            osal_printk("%.8x ", READ_REG(CNT1_B)); \
            osal_printk("\n"); \
            osal_printk("%.8x ", READ_REG(IR_BUSY)); \
            osal_printk("%.8x ", READ_REG(IR_DATAH)); \
            osal_printk("%.8x ", READ_REG(IR_DATAL)); \
            osal_printk("%.8x ", READ_REG(IR_INTM)); \
            osal_printk("%.8x ", READ_REG(IR_INTS)); \
            osal_printk("%.8x ", READ_REG(IR_INTC)); \
            osal_printk("%.8x ", READ_REG(IR_START)); \
            osal_printk("\n"); \
        } \
    }while(0);

// unit is ms
#define DEFAULT_DELAYTIME (200*10)

#define HI_IR_MAX_BUF 100
#define DEFAULT_BUF_LEN 8
#define BUF_HEAD hiir_dev.buf[hiir_dev.head]
#define BUF_TAIL hiir_dev.buf[hiir_dev.tail]
#define BUF_LAST hiir_dev.buf[(hiir_dev.head == 0)? (hiir_dev.buf_len - 1): (hiir_dev.head - 1)]
#define INC_BUF(x,len) 	({\
        unsigned int __ret = 0;\
        ++(x);\
        __ret=(x)%(len);\
        __ret;\
    })

typedef struct
{
    hiir_dev_param dev_parm;
    unsigned int head;
    unsigned int tail;
    irkey_info_s buf[HI_IR_MAX_BUF + 1];
    unsigned int buf_len;

    unsigned int enable_repkey;
    unsigned int repkey_checkflag;
    unsigned int repkey_delaytime;/*ms*/

    unsigned int enable_keyup;

    osal_wait_t irkey_wait;
}hiir_dev_struct;

static hiir_dev_struct hiir_dev;

static void repkey_timeout_handler(unsigned long data);
static struct osal_timer repkey_timeout_timer;

static void hiir_config(void)
{
    int value = 0;

/* TODO: ASIC config pin share */
#if 0
    value = READ_REG(IOCONFIG);
    value = 0;
    WRITE_REG(IOCONFIG, value);
#endif

    WRITE_REG(IR_ENABLE, 0x01);
    while(READ_REG(IR_BUSY))
    {
        hiir_dbg("IR_BUSY. Wait...\n");
        //udelay(1);
    }

    value = (hiir_dev.dev_parm.codetype << 14);
    value |= (hiir_dev.dev_parm.code_len - 1) << 8;
    value |= (hiir_dev.dev_parm.frequence - 1);
    WRITE_REG(IR_CONFIG, value);

    value = hiir_dev.dev_parm.leads_min << 16;
    value |= hiir_dev.dev_parm.leads_max;
    WRITE_REG(CNT_LEADS, value);

    value = hiir_dev.dev_parm.leade_min << 16;
    value |= hiir_dev.dev_parm.leade_max;
    WRITE_REG(CNT_LEADE, value);

    value = hiir_dev.dev_parm.sleade_min << 16;
    value |= hiir_dev.dev_parm.sleade_max;
    WRITE_REG(CNT_SLEADE, value);

    value = hiir_dev.dev_parm.cnt0_b_min << 16;
    value |= hiir_dev.dev_parm.cnt0_b_max;
    WRITE_REG(CNT0_B, value);

    value = hiir_dev.dev_parm.cnt1_b_min << 16;
    value |= hiir_dev.dev_parm.cnt1_b_max;
    WRITE_REG(CNT1_B, value);

    WRITE_REG(IR_INTM, 0x00);
    WRITE_REG(IR_START, 0x00);

    //hiir_dbg("current config is...\n");
    //hiir_show_reg();
}

static void repkey_timeout_handler(unsigned long data)
{
    osal_del_timer(&repkey_timeout_timer);
    hiir_dev.repkey_checkflag = 0;
}

static int hiir_interrupt(int irq, void *dev_id)
{
    hiir_dbg("Enter hiir_interrupt.\n");
    hiir_show_reg();

    if(READ_REG(IR_INTS) & INTMS_FRAMERR)
    {
        hiir_dbg("frame error.\n");
        WRITE_REG(IR_INTC, 0x01<<1);
    }
    else if(READ_REG(IR_INTS) & INTMS_OVERFLOW)
    {
        hiir_dbg("hardware overflow.\n");
        WRITE_REG(IR_INTC, 0x01<<2);
    }
    else if(READ_REG(IR_INTS) & INTMS_RELEASE)
    {
        hiir_dbg("release key interrupt.\n");
        WRITE_REG(IR_INTC, 0x01<<3);

        osal_del_timer(&repkey_timeout_timer);
        hiir_dev.repkey_checkflag = 0;

        if( (hiir_dev.enable_keyup) && (HIIR_KEY_UP != BUF_LAST.irkey_state_code) )
        {
            BUF_HEAD = BUF_LAST;
            BUF_HEAD.irkey_state_code = HIIR_KEY_UP;
            hiir_dev.head = INC_BUF(hiir_dev.head, hiir_dev.buf_len);
            osal_wakeup(&(hiir_dev.irkey_wait));
        }
    }
    else if(READ_REG(IR_INTS) & INTMS_RCV)
    {
        hiir_dbg("receive data interrupt. 0x%.8x-0x%.8x\n", READ_REG(IR_DATAH), READ_REG(IR_DATAL));
        WRITE_REG(IR_INTC, 0x01<<0);

        if(hiir_dev.enable_repkey)
        {
            if( (hiir_dev.repkey_checkflag) &&
                (BUF_LAST.irkey_datah == READ_REG(IR_DATAH)) &&
                (BUF_LAST.irkey_datal == READ_REG(IR_DATAL)) )
            {
                hiir_dbg("repeart key [0x%.8x]-[0x%.8x] detective\n", READ_REG(IR_DATAH), READ_REG(IR_DATAL));
            }
            else
            {
                /* repeat key check */
                osal_del_timer(&repkey_timeout_timer);
                osal_set_timer(&repkey_timeout_timer, hiir_dev.repkey_delaytime/10);
                hiir_dev.repkey_checkflag = 1;
                BUF_HEAD.irkey_datah = READ_REG(IR_DATAH);
                BUF_HEAD.irkey_datal = READ_REG(IR_DATAL);
                BUF_HEAD.irkey_state_code = HIIR_KEY_DOWN;
                hiir_dev.head = INC_BUF(hiir_dev.head, hiir_dev.buf_len);
                osal_wakeup(&(hiir_dev.irkey_wait));
            }
        }
	else if( !((BUF_LAST.irkey_datah == READ_REG(IR_DATAH)) 
			&& (BUF_LAST.irkey_datal == READ_REG(IR_DATAL))
                    && (BUF_LAST.irkey_state_code == HIIR_KEY_DOWN) ) )
        {
            BUF_HEAD.irkey_datah = READ_REG(IR_DATAH);
            BUF_HEAD.irkey_datal = READ_REG(IR_DATAL);
            BUF_HEAD.irkey_state_code = HIIR_KEY_DOWN;
            hiir_dev.head = INC_BUF(hiir_dev.head, hiir_dev.buf_len);
            osal_wakeup(&(hiir_dev.irkey_wait));
        }
    }
    else
    {
        osal_printk("logic Error: int_mask=0x%.8x, int_state=0x%.8x.\n", READ_REG(IR_INTM), READ_REG(IR_INTS));
    }

    hiir_dbg("Exit hiir_interrupt.\n");

    return OSAL_IRQ_HANDLED;
}

static long hiir_ioctl(unsigned int cmd, unsigned long arg, void *private_data)
{
    int *p = (int *)arg;
    //unsigned int min_val, max_val;

    switch(cmd)
    {
        case IR_IOC_SET_BUF:
            if((0<arg) && (arg<=HI_IR_MAX_BUF))
            {
                hiir_dev.buf_len = arg + 1;

                /* need reset buf pointrt */
                hiir_dev.head = 0;
                hiir_dev.tail = 0;
            }
            hiir_dbg("IR_IOC_SET_BUF -> buf_len=%d\n", hiir_dev.buf_len);
            break;
        case IR_IOC_SET_ENABLE_KEYUP:
            hiir_dev.enable_keyup = arg;
            hiir_dbg("IR_IOC_SET_ENABLE_KEYUP -> enable_keyup=%d\n", hiir_dev.enable_keyup);
            break;
        case IR_IOC_SET_ENABLE_REPKEY:
            hiir_dev.enable_repkey = arg;
            hiir_dbg("IR_IOC_SET_ENABLE_REPKEY -> enable_repkey=%d\n", hiir_dev.enable_repkey);
            break;
        case IR_IOC_SET_REPKEY_TIMEOUTVAL:
            if(arg > 0)
            {
                hiir_dev.repkey_delaytime = arg;
            }
            hiir_dbg("IR_IOC_SET_REPKEY_TIMEOUTVAL -> repkey_delaytime=%d\n", hiir_dev.repkey_delaytime);
            break;
        case IR_IOC_SET_FORMAT:
            if((arg > 3) )
            {
                osal_printk("Error: IR_IOC_SET_FORMAT -> invalid args=%lu\n", arg);
                return -1;
            }
            hiir_dev.dev_parm.codetype = arg;
            hiir_dbg("IR_IOC_SET_FORMAT -> codetype=%d\n", hiir_dev.dev_parm.codetype);
            break;
        case IR_IOC_SET_CODELEN:
            hiir_dev.dev_parm.code_len = arg;
            hiir_dbg("IR_IOC_SET_CODELEN -> code_len=%d\n", hiir_dev.dev_parm.code_len);
            break;
        case IR_IOC_SET_FREQ:
            if( (arg <= 0) || (arg > 128) )
            {
                osal_printk("Error: IR_IOC_SET_FREQ -> invalid args=%lu\n", arg);
                return -1;
            }
            hiir_dev.dev_parm.frequence = arg;
            hiir_dbg("IR_IOC_SET_FREQ -> frequence=%d\n", hiir_dev.dev_parm.frequence);
            break;
        case IR_IOC_SET_LEADS:
            #if 0
            if (osal_copy_from_user(&min_val,p,sizeof(int)) || osal_copy_from_user(&max_val,p+1,sizeof(int)))
            {
                return -1;
            }
            hiir_dev.dev_parm.leads_min = min_val;
            hiir_dev.dev_parm.leads_max = max_val;
            #else
            hiir_dev.dev_parm.leads_min = *p++;
            hiir_dev.dev_parm.leads_max = *p;
            #endif
            hiir_dbg("IR_IOC_SET_LEADS -> leads_min=%d, leads_max=%d\n", hiir_dev.dev_parm.leads_min, hiir_dev.dev_parm.leads_max);
            break;
        case IR_IOC_SET_LEADE:
            #if 0
            if (osal_copy_from_user(&min_val,p,sizeof(int)) || osal_copy_from_user(&max_val,p+1,sizeof(int)))
            {
                return -1;
            }
            hiir_dev.dev_parm.leade_min = min_val;
            hiir_dev.dev_parm.leade_max = max_val;
            #else
            hiir_dev.dev_parm.leade_min = *p++;
            hiir_dev.dev_parm.leade_min = *p;
            #endif
            hiir_dbg("IR_IOC_SET_LEADE -> leade_min=%d, leade_max=%d\n", hiir_dev.dev_parm.leade_min, hiir_dev.dev_parm.leade_max);
            break;
        case IR_IOC_SET_SLEADE:
            #if 0
            if (osal_copy_from_user(&min_val,p,sizeof(int)) || osal_copy_from_user(&max_val,p+1,sizeof(int)))
            {
                return -1;
            }
            hiir_dev.dev_parm.sleade_min = min_val;
            hiir_dev.dev_parm.sleade_max = max_val;
            #else
            hiir_dev.dev_parm.sleade_min = *p++;
            hiir_dev.dev_parm.sleade_max = *p;
            #endif            
            hiir_dbg("IR_IOC_SET_SLEADE -> sleade_min=%d, sleade_max=%d\n", hiir_dev.dev_parm.sleade_min, hiir_dev.dev_parm.sleade_max);
            break;
        case IR_IOC_SET_CNT0_B:
            #if 0
            if (osal_copy_from_user(&min_val,p,sizeof(int)) || osal_copy_from_user(&max_val,p+1,sizeof(int)))
            {
                return -1;
            }
            hiir_dev.dev_parm.cnt0_b_min = min_val;
            hiir_dev.dev_parm.cnt0_b_max = max_val;
            #else
            hiir_dev.dev_parm.cnt0_b_min = *p++;
            hiir_dev.dev_parm.cnt0_b_max = *p;
            #endif
            hiir_dbg("IR_IOC_SET_CNT0_B -> cnt0_b_min=%d, cnt0_b_max=%d\n", hiir_dev.dev_parm.cnt0_b_min, hiir_dev.dev_parm.cnt0_b_max);
            break;
        case IR_IOC_SET_CNT1_B:
            #if 0
            if (osal_copy_from_user(&min_val,p,sizeof(int)) || osal_copy_from_user(&max_val,p+1,sizeof(int)))
            {
                return -1;
            }
            hiir_dev.dev_parm.cnt1_b_min = min_val;
            hiir_dev.dev_parm.cnt1_b_max = max_val;
            #else
            hiir_dev.dev_parm.cnt1_b_min = *p++;
            hiir_dev.dev_parm.cnt1_b_max = *p;
            #endif
            hiir_dbg("IR_IOC_SET_CNT1_B -> cnt1_b_min=%d, cnt1_b_max=%d\n", hiir_dev.dev_parm.cnt1_b_min, hiir_dev.dev_parm.cnt1_b_max);
            break;
        case IR_IOC_GET_CONFIG:
            #if 0
            if(osal_copy_to_user((void *)arg, &(hiir_dev.dev_parm), sizeof(hiir_dev_param)))
            {
				osal_printk("%s: copy_to_user fail!\n",__func__);
            }
            #else
            osal_memcpy((void *)arg, &(hiir_dev.dev_parm), sizeof(hiir_dev_param));
            #endif
            hiir_dbg("IR_IOC_GET_CONFIG\n");
            break;
        case IR_IOC_ENDBG:
            g_dbg_flag = 1;
            hiir_dbg("IR_IOC_ENDBG\n");
            break;
        case IR_IOC_DISDBG:
            g_dbg_flag = 0;
            hiir_dbg("IR_IOC_DISDBG\n");
            break;
        default:
            osal_printk("Error: Inappropriate ioctl for device. cmd=%d\n", cmd);
            return -1;
    }

    hiir_config();

    return 0;
}

int hiir_wait_condition_callback(void *pParam)
{
    int s32Ret = 0;
    s32Ret = (hiir_dev.head != hiir_dev.tail);
    return s32Ret;

}

static int hiir_read(char *buf, int count, long *f_pos, void *private_data)
{
    irkey_info_s irkey_to_user;
    int res = 0;

retry :
    hiir_dbg("Enter hiir_read : head=%d, tail=%d, buf_len=%d\n", hiir_dev.head, hiir_dev.tail, hiir_dev.buf_len);

    if((hiir_dev.head)==(hiir_dev.tail))
    {

        res = osal_wait_event_interruptible(&(hiir_dev.irkey_wait), hiir_wait_condition_callback, &hiir_dev);
        if (0 != res)
        {
            return res;
        }
        goto retry;
    }
    else
    {
        while( ((hiir_dev.head)!=(hiir_dev.tail)) && ((res + sizeof(irkey_info_s)) <= count) )
        {
            irkey_to_user = BUF_TAIL;
            hiir_dev.tail = INC_BUF(hiir_dev.tail, hiir_dev.buf_len);
            #if 0
            if(osal_copy_to_user((buf + res), &irkey_to_user, sizeof(irkey_info_s)))
            {
				osal_printk("%s: copy_to_user fail!\n",__func__);
            }
            #else
            osal_memcpy((buf + res), &irkey_to_user, sizeof(irkey_info_s));
            #endif
            res += sizeof(irkey_info_s);
        }
    }
    return res;
}

static unsigned int hiir_select (osal_poll_t *osal_poll, void *private_data)
{
    hiir_dbg("Enter hiir_select.\n");
    if((hiir_dev.head)!=(hiir_dev.tail))
    {
        return 1;
    }
    osal_poll_wait(osal_poll, &(hiir_dev.irkey_wait));
    return 0;
}

static osal_atomic_t hiir_s_available;

static int hiir_open(void *private_data)
{
    int ret;
    hiir_dbg("Enter hiir_open.\n");

    if(0 != osal_atomic_dec_return(&hiir_s_available) )
    {
       
       ret = osal_atomic_inc_return(&hiir_s_available);
       osal_printk("Error: device already open:%d.\n", ret);
       return -1;
    }

    osal_del_timer(&repkey_timeout_timer);

    g_dbg_flag = 0;

    /* init hiir_dev */
    hiir_dev.dev_parm = static_dev_param[0];/*nec format*/
    hiir_dev.head = 0;
    hiir_dev.tail = 0;
    hiir_dev.buf_len = DEFAULT_BUF_LEN;
    hiir_dev.enable_repkey = 1;
    hiir_dev.repkey_checkflag = 0;
    hiir_dev.repkey_delaytime = DEFAULT_DELAYTIME;
    hiir_dev.enable_keyup = 1;
    osal_wait_init(&hiir_dev.irkey_wait);

    hiir_config();

    return 0;
}

static int hiir_release(void *private_data)
{
    hiir_dbg("Enter hiir_release.\n");
    
    /* disable HIIR11 */
    WRITE_REG(IR_ENABLE, 0x00);

    if(1 != osal_atomic_inc_return(&hiir_s_available) )
    {
       osal_atomic_dec_return(&hiir_s_available);
       return 0;
    }

    osal_del_timer(&repkey_timeout_timer);
    
    osal_wait_destory(&hiir_dev.irkey_wait);

    return 0;
}

static int hiir_write(const char *buf, int size, long *offset, void *private_data)
{
    return 0;
}

static struct osal_fileops hiir_fops =
{
    open    : hiir_open,
    unlocked_ioctl   : hiir_ioctl,
    poll    : hiir_select,
    read    : hiir_read,
    write   : hiir_write,
    release : hiir_release,
};

static struct osal_dev *hiir_miscdev = NULL;

int hiir_init(void)
{
    int res = 0;

    if (ir_irq <= 0)
    {
        ir_irq = HIIR_DEVICE_IRQ_NO;
    }

    if (!pIrRegBase)
    {
        pIrRegBase = (volatile void *)osal_ioremap_nocache(IR_REG_BASE, IR_REG_LENGTH);
        if (!pIrRegBase)
        {
            osal_printk("osal_ioremap_nocache err. \n");
            return -1;
        }
    }
    
    res = osal_atomic_init(&hiir_s_available);
    if (res)
    {
        osal_printk("Error: init atomic\n");
        return -1;
    }
    osal_atomic_set(&hiir_s_available, 1);
    
    repkey_timeout_timer.function = repkey_timeout_handler;
    res = osal_timer_init(&repkey_timeout_timer);
    if (res)
    {
        goto out0;
    }
    
    /* disable HIIR */
    WRITE_REG(IR_ENABLE, 0x00);

    hiir_miscdev = osal_createdev(HIIR_DEVICE_NAME);
    if (NULL == hiir_miscdev)
    {
        osal_printk("Error: can't create dev\n");
        goto out1;
    }
        
    hiir_miscdev->minor = 255;
    hiir_miscdev->fops = &hiir_fops;
    res = osal_registerdevice(hiir_miscdev);
    if (0 != res)
    {
        osal_printk("Error: can't register\n");
        goto out2;
    }    
           
    res = osal_request_irq(ir_irq, hiir_interrupt, NULL, HIIR_DEVICE_NAME, &hiir_interrupt);
    if(res)
    {
        osal_printk("Error: request IRQ(%d) failed\n", ir_irq);
        goto out3;
    }

    osal_printk("hiir init ok. ver=%s, %s.\n", __DATE__, __TIME__);
    return 0;
    
out3:
    osal_deregisterdevice(hiir_miscdev);
out2:
    osal_destroydev(hiir_miscdev);
out1:
    osal_timer_destory(&repkey_timeout_timer);
out0:
    osal_atomic_destory(&hiir_s_available);
    return -1;
}

void hiir_exit(void)
{
    osal_free_irq(ir_irq, &hiir_interrupt);
    
    osal_deregisterdevice(hiir_miscdev);
    osal_destroydev(hiir_miscdev);

    osal_del_timer(&repkey_timeout_timer);
    osal_timer_destory(&repkey_timeout_timer);

    osal_atomic_destory(&hiir_s_available);
    
	osal_printk("hiir exit ok.\n");
}

