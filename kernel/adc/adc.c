/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include "osal.h"
#include <linux/types.h>
#include "adc.h"

#ifndef NULL
#define NULL  ((void *)0)
#endif


#define lsadc_print(fmt, ...) osal_printk("Func:%s, Line:%d, "fmt"\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)

//#define ENABLE_ADC_IRQ
#define LSADC_IRQ_ID            36
#define LSADC_BASE_ADDR         0x120a0000
#define LSADC_ADDR_OFFSET(x)    (LSADC_BASE_ADDR+(x))

#define LSADC_CRG_ADDR          0x120101BC

#define LSADC_CONFIG       0x00
#define LSADC_GLITCH       0x04
#define LSADC_TIMESCAN     0x08
#define LSADC_INTEN        0x10
#define LSADC_INTSTATUS    0x14
#define LSADC_INTCLR       0x18
#define LSADC_START        0x1C
#define LSADC_STOP         0x20
#define LSADC_ACTBIT       0x24
#define LSADC_CHNDATA      0x2C

#define LSADC_MAX_CHN_NUM       2
#define LSADC_NUM_BITS          10
#define LSADC_CHN_MASK          0x3

volatile void* lsadc_reg = NULL;
int lsadc_irq = LSADC_IRQ_ID;
osal_spinlock_t     g_stLSADCLock;

#define LSADC_SPIN_LOCK_FLAG   unsigned long
#define LSADC_VIR_ADDR(x) ((uintptr_t)(lsadc_reg) + ((x)-LSADC_BASE_ADDR))
#define lsadc_writel(v, x)  osal_writel(v, LSADC_VIR_ADDR(LSADC_ADDR_OFFSET(x)))
#define lsadc_readl(x)      osal_readl(LSADC_VIR_ADDR(LSADC_ADDR_OFFSET(x)))

static inline void lsadc_reg_write(unsigned long value, unsigned long mask,
                                   unsigned long addr)
{
    unsigned long t;

    t = lsadc_readl(addr);
    t &= ~mask;
    t |= value & mask;
    lsadc_writel(t, addr);
}

static void write_reg32(unsigned int value, unsigned int mask, const void* addr)
{
    unsigned int t;

    t = osal_readl((void*)addr);
    t &= ~mask;
    t |= value & mask;

    osal_writel(t, (void*)addr);
}

static int LsAdc_open(void* private_data)
{
    lsadc_reg_write(0 << 15, 1 << 15, LSADC_CONFIG); //unreset analog circuit
    return 0;
}

static int LsAdc_release(void* private_data)
{
    lsadc_reg_write(1 << 15, 1 << 15, LSADC_CONFIG); //reset analog circuit
    return 0;
}

/* 0: single scanning mode
*  1: continuous scanning mode
*  The filter glitch is already eanble, only the continuous mode has a filter glitch, and the single mode is invalid.
*/
static int lsadc_model_select(int value)
{
    unsigned int val;

    val = (unsigned int)value;

    if ((val != 0) && (val != 1))
    {
        lsadc_print("error value:%x\n", val);
        return -1;
    }

    lsadc_reg_write(val << 13, 1 << 13, LSADC_CONFIG);

    if (val == 1)
    {
        lsadc_reg_write(0xc << 20, 0xc << 20, LSADC_CONFIG);
        lsadc_writel(0x30,  LSADC_GLITCH);              //glitch_sample, must > 0
        lsadc_writel(0x200, LSADC_TIMESCAN);            //time_scan, must > 20
        lsadc_reg_write(0 << 17, 0 << 17, LSADC_CONFIG);
    }
    else
    {
        lsadc_reg_write(1 << 17, 1 << 17, LSADC_CONFIG); //set glitch bypass
    }

    return 0;
}

static int lsadc_chn_valid(int chn, int enable)
{
    unsigned long value;

    value = enable ? 1 : 0;

    switch (chn)
    {
        case 0:
            lsadc_reg_write(value << 8, 1 << 8,   LSADC_CONFIG);
            break;

        case 1:
            lsadc_reg_write(value << 9, 1 << 9,   LSADC_CONFIG);
            break;

        default:
            lsadc_print("error chn:%d\n", chn);
            return -1;
    }

    return 0;
}

static void lsadc_enable_clock(void)
{
    void  *lsadc_crg_addr;

    lsadc_crg_addr = osal_ioremap(LSADC_CRG_ADDR, (unsigned long)0x4);

    write_reg32(0x1 << 23, 0x1 << 23, lsadc_crg_addr);

    osal_iounmap((void*)lsadc_crg_addr);
}

static void lsadc_disable_clock(void)
{
    void  *lsadc_crg_addr;

    lsadc_crg_addr = osal_ioremap(LSADC_CRG_ADDR, (unsigned long)0x4);

    write_reg32(0x0 << 23, 0x1 << 23, lsadc_crg_addr);

    osal_iounmap((void*)lsadc_crg_addr);
}

static int lsadc_start(void)
{
#ifdef ENABLE_ADC_IRQ
    LSADC_SPIN_LOCK_FLAG flag;

    osal_spin_lock_irqsave(&g_stLSADCLock, &flag);

    lsadc_reg_write(1, 1, LSADC_INTEN);      //int enable

    osal_spin_unlock_irqrestore(&g_stLSADCLock, &flag);
#endif
    lsadc_reg_write(1, 1, LSADC_START);      //start


    return 0;
}

static int lsadc_stop(void)
{
    lsadc_reg_write(1, 1, LSADC_STOP);      //stop

#ifdef ENABLE_ADC_IRQ
    LSADC_SPIN_LOCK_FLAG flag;

    osal_spin_lock_irqsave(&g_stLSADCLock, &flag);

    lsadc_reg_write(0, 1, LSADC_INTEN);     //int disable

    osal_spin_unlock_irqrestore(&g_stLSADCLock, &flag);
#endif

    return 0;
}

static int lsadc_get_chn_value(int chn)
{
    unsigned int unchn;

    if (chn < 0 || chn >= LSADC_MAX_CHN_NUM)
    {
        lsadc_print("error chn:%d\n", chn);
        return -1;
    }

    unchn = (unsigned int)chn;

    return lsadc_readl(LSADC_CHNDATA + (unchn << 2));
}

#ifdef ENABLE_ADC_IRQ
static int lsadc_irq_proc(int irq, void* devId)
{
    unsigned int intstate;
    int chn_value;
    int chn;
    LSADC_SPIN_LOCK_FLAG flag;

    osal_spin_lock_irqsave(&g_stLSADCLock, &flag);

    intstate = lsadc_readl(LSADC_INTSTATUS);
    lsadc_reg_write(LSADC_CHN_MASK, LSADC_CHN_MASK, LSADC_INTCLR);        //clr int flag all

    osal_spin_unlock_irqrestore(&g_stLSADCLock, &flag);

    for (chn = 0; chn < LSADC_MAX_CHN_NUM; chn++)
    {
        if (intstate & (1 << chn))
        {
            chn_value = lsadc_get_chn_value(chn);
            lsadc_print("chn[%d] value:%d\n", chn, chn_value);
        }
    }

    //do what you want to do in irq

    return OSAL_IRQ_HANDLED;
}
#endif

static long LsAdc_ioctl (unsigned int cmd, unsigned long arg, void* private_data)
{
    int ret = -1;
    int param = 0;

    switch (cmd)
    {
        case LSADC_IOC_MODEL_SEL:
        {
            param = *(int*)(uintptr_t)arg;
            ret = lsadc_model_select(param);
            break;
        }

        case LSADC_IOC_CHN_ENABLE:
        {
            param = *(int*)(uintptr_t)arg;
            ret = lsadc_chn_valid(param, 1);
            break;
        }

        case LSADC_IOC_CHN_DISABLE:
        {
            param = *(int*)(uintptr_t)arg;
            ret = lsadc_chn_valid(param, 0);
            break;
        }

        case LSADC_IOC_START:
        {
            ret = lsadc_start();
            break;
        }

        case LSADC_IOC_STOP:
        {
            ret = lsadc_stop();
            break;
        }

        case LSADC_IOC_GET_CHNVAL:
        {
            param = *(int*)(uintptr_t)arg;
            ret = lsadc_get_chn_value(param);
            break;
        }

        default:
            lsadc_print("error cmd:%08x\n", cmd);
            ret = -1;
    }

    return ret;
}

static struct osal_fileops LsAdc_fops =
{
    .open           = LsAdc_open,
    .release        = LsAdc_release,
    .unlocked_ioctl = LsAdc_ioctl,
};

static osal_dev_t* s_stLsAdcDevice;


int lsadc_init(void)
{
    int ret = 0;

    if (!lsadc_reg)
    {
        lsadc_reg = (volatile void*)osal_ioremap(LSADC_BASE_ADDR, 0x100);

        if (!lsadc_reg)
        {
            lsadc_print("lsadc ioremap error.\n");
            return -1;
        }
    }

#ifdef ENABLE_ADC_IRQ

    if (lsadc_irq <= 0)
    {
        lsadc_irq = LSADC_IRQ_ID;
    }

    osal_spin_lock_init(&g_stLSADCLock);

    ret = osal_request_irq(lsadc_irq, lsadc_irq_proc, 0, "lsadc", &LsAdc_fops);

    if (ret != 0)
    {
        lsadc_print("lsadc request irq error.\n");
        osal_iounmap((void*)lsadc_reg);
        return -1;
    }

#endif

    s_stLsAdcDevice = osal_createdev("lsadc");
    s_stLsAdcDevice->minor = 255;
    s_stLsAdcDevice->fops = &LsAdc_fops;
    ret = osal_registerdevice(s_stLsAdcDevice);

    if (ret != 0)
    {
        osal_destroydev(s_stLsAdcDevice);
        lsadc_print("lsadc register device error.\n");
        osal_free_irq(lsadc_irq, &LsAdc_fops);
        osal_iounmap((void*)lsadc_reg);
    }

	lsadc_enable_clock();

    osal_printk("lsadc init ok. ver=%s, %s.\n", __DATE__, __TIME__);

    return ret;
}


void lsadc_exit(void)
{
#ifdef ENABLE_ADC_IRQ
    osal_free_irq(lsadc_irq, &LsAdc_fops);
    osal_spin_lock_destory(&g_stLSADCLock);
#endif

    osal_deregisterdevice(s_stLsAdcDevice);
    osal_destroydev(s_stLsAdcDevice);

    //osal_iounmap((void*)lsadc_reg);

	lsadc_disable_clock();

    osal_printk("lsadc exit ok.\n");
}

