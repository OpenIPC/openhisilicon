/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#include "osal.h"
#include <linux/types.h>
#include "adc.h"

#ifndef NULL
#define NULL  ((xmedia_void *)0)
#endif


#define lsadc_print(fmt, ...) osal_printk("Func:%s, Line:%d, "fmt"\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)

//#define USE_LSADC_CHANNEL_0
//#define ENABLE_ADC_IRQ
#define LSADC_IRQ_ID            37
#define LSADC_BASE_ADDR         0x120a0000
#define LSADC_ADDR_OFFSET(x)    (LSADC_BASE_ADDR+(x))

#define LSADC_CRG_ADDR          0x120101BC

#define LSADC_CONFIG       0x00
#define LSADC_GLITCH       0x04
#define LSADC_TIMESCAN     0x08
#define LSADC_PD_TD        0x0C
#define LSADC_INTEN        0x10
#define LSADC_INTSTATUS    0x14
#define LSADC_INTCLR       0x18
#define LSADC_START        0x1C
#define LSADC_STOP         0x20
#define LSADC_ACTBIT       0x24
#define LSADC_CHNDATA      0x2C
#define LSADC_ENABLE       0x3C

#define LSADC_MAX_CHN_NUM       3
/* xmorca LSADC 实际是 12-bit。证据：ACTBIT 寄存器 mask 0xFFF (bits [11:0]) 见
 * lsadc_model_select 内的 ACTBIT 写入注释；CHNDATA idle 满量程 = 4095 = 2^12-1。
 * 旧值 10 是写错的（很可能是从老 SDK 抄过来没改），本宏当前未被代码引用，
 * 仅作文档说明——后续 sample/app 不要按 10-bit 解读 raw。 */
#define LSADC_NUM_BITS          12
#define LSADC_ALL_CHN_INT_MASK  0xF

#define LSADC_ACTIVE_BIT        (0xfff)
#define LSADC_ACTIVE_BIT_MASK   (0xFFF)

#define LSADC_DATA_DELTA        (0xc)
#define LSADC_DATA_DELTA_MASK   (0xF)
#define LSADC_GLITCH_SAMPLE     (0x30)
#define LSADC_TIME_SCAN         (0x200)
#define LSADC_GLITCH_TIME_US    ((LSADC_GLITCH_SAMPLE + 1) * LSADC_TIME_SCAN * 25 / 100)

volatile xmedia_void* lsadc_reg = NULL;
xmedia_s32 lsadc_irq = LSADC_IRQ_ID;

#ifdef ENABLE_ADC_IRQ
#define INVALID_CDC_VALUE       (0)
typedef struct {
    xmedia_u32     adc_value;
} lsadc_cdc_info;

lsadc_cdc_info g_cdc_info[LSADC_MAX_CHN_NUM];
static osal_spinlock_t g_lsadc_spin_lock;
#endif

#define LSADC_SPIN_LOCK_FLAG   xmedia_ulong
#define LSADC_VIR_ADDR(x) ((uintptr_t)(lsadc_reg) + ((x)-LSADC_BASE_ADDR))
#define lsadc_writel(v, x)  osal_writel(v, LSADC_VIR_ADDR(LSADC_ADDR_OFFSET(x)))
#define lsadc_readl(x)      osal_readl(LSADC_VIR_ADDR(LSADC_ADDR_OFFSET(x)))

static inline xmedia_void lsadc_reg_write(xmedia_ulong value, xmedia_ulong mask,
                                   xmedia_ulong addr)
{
    xmedia_ulong t;

    t = lsadc_readl(addr);
    t &= ~mask;
    t |= value & mask;
    lsadc_writel(t, addr);
}

static xmedia_void write_reg32(xmedia_u32 value, xmedia_u32 mask, const xmedia_void* addr)
{
    xmedia_u32 t;

    t = osal_readl((xmedia_void*)addr);
    t &= ~mask;
    t |= value & mask;

    osal_writel(t, (xmedia_void*)addr);
}

static xmedia_void lsadc_enable_clock(xmedia_void)
{
    xmedia_void  *lsadc_crg_addr;

    lsadc_crg_addr = osal_ioremap(LSADC_CRG_ADDR, (xmedia_ulong)0x4);

    write_reg32(0x0 << 2, 0x1 << 2, lsadc_crg_addr); // (bit2)ls_adc_srst_req = 0
    osal_udelay(200);

    write_reg32(0x1 << 3, 0x1 << 3, lsadc_crg_addr); // (bit3)ls_adc_cken = 1
    osal_udelay(100);

    osal_iounmap((xmedia_void*)lsadc_crg_addr);
}

static xmedia_void lsadc_disable_clock(xmedia_void)
{
    xmedia_void  *lsadc_crg_addr;

    lsadc_crg_addr = osal_ioremap(LSADC_CRG_ADDR, (xmedia_ulong)0x4);

    write_reg32(0x0 << 3, 0x1 << 3, lsadc_crg_addr); // (bit3)ls_adc_cken = 0
    osal_udelay(100);

    write_reg32(0x1 << 2, 0x1 << 2, lsadc_crg_addr); // (bit2)ls_adc_srst_req = 1
    osal_udelay(100);

    osal_iounmap((xmedia_void*)lsadc_crg_addr);
}

static xmedia_s32 lsadc_open(xmedia_void* private_data)
{
    lsadc_enable_clock();
    lsadc_reg_write(1 << 15, 1 << 15, LSADC_CONFIG); // exit reset state
    lsadc_reg_write(1 << 9, 1 << 9 , LSADC_ENABLE); // enable lsadc
    return 0;
}

static xmedia_s32 lsadc_release(xmedia_void* private_data)
{
    lsadc_reg_write(0 << 15, 1 << 15, LSADC_CONFIG); // enter reset state
    lsadc_reg_write(0 << 9, 1 << 9 , LSADC_ENABLE); // disable lsadc
    lsadc_disable_clock();
    return 0;
}

/* 0: single scanning mode
*  1: continuous scanning mode
*  The filter glitch is already eanble, only the continuous mode has a filter glitch, and the single mode is invalid.
*/
static xmedia_s32 lsadc_model_select(xmedia_s32 value)
{
    xmedia_u32 val;

    val = (xmedia_u32)value;

    if ((val != 0) && (val != 1)) {
        lsadc_print("error value:%x\n", val);
        return -1;
    }

    lsadc_reg_write(val << 13, 1 << 13, LSADC_CONFIG);

    if (val == 1) {
        lsadc_reg_write(LSADC_ACTIVE_BIT, LSADC_ACTIVE_BIT_MASK, LSADC_ACTBIT); // [11:0]
        lsadc_reg_write(LSADC_DATA_DELTA << 20, LSADC_DATA_DELTA_MASK << 20, LSADC_CONFIG); // [23:20]
        lsadc_writel(LSADC_GLITCH_SAMPLE,  LSADC_GLITCH);              //glitch_sample, must > 0
        lsadc_writel(LSADC_TIME_SCAN, LSADC_TIMESCAN);            //time_scan, must > 20
        /* set filter glitch function, 0:enable, 1:bypass, attention to LSADC_CTRL10(0x120a0028),
         * lsadc will filter the lsadc_zero value in this reg;defaule value is 0
         */
        lsadc_reg_write(0 << 17, 1 << 17, LSADC_CONFIG);
    } else {
        lsadc_reg_write(1 << 17, 1 << 17, LSADC_CONFIG); //set glitch bypass
    }
    return 0;
}

static xmedia_s32 lsadc_chn_valid(xmedia_s32 chn, xmedia_s32 enable)
{
    xmedia_ulong value;

    value = enable ? 1 : 0;

    switch (chn)
    {
#ifdef USE_LSADC_CHANNEL_0
        case 0:
            lsadc_reg_write(value << 8, 1 << 8,   LSADC_CONFIG);
            break;
#endif

        case 1:
            lsadc_reg_write(value << 9, 1 << 9,   LSADC_CONFIG);
            break;

        case 2:
            lsadc_reg_write(value << 10, 1 << 10,   LSADC_CONFIG);
            break;

        default:
            lsadc_print("error chn:%d\n", chn);
            return -1;
    }

    return 0;
}

static xmedia_s32 lsadc_start(xmedia_void)
{
#ifdef ENABLE_ADC_IRQ
    LSADC_SPIN_LOCK_FLAG flag;
#endif
    xmedia_s32 model_sel = 0;
    xmedia_s32 deglitch_bypass = 0;
    xmedia_s32 config = lsadc_readl(LSADC_CONFIG);
    xmedia_s32 chn_num = 0;
    xmedia_s32 sleep_time_ms = 0;
    if ( (config & (1 << 13)) != 0 ) {
        model_sel = 1;
    }
    if ( (config & (1 << 17)) != 0 ) {
        deglitch_bypass = 1;
    }
    if (1 == model_sel) {
        xmedia_s32 lsadc_active_bit = lsadc_readl(LSADC_ACTBIT) & 0xFFF;
        lsadc_print("config=%#x lsadc_active_bit=%#x\n", config, lsadc_active_bit);
    #ifdef USE_LSADC_CHANNEL_0
        if ( (config & (1 << 8)) != 0 ) {
            chn_num++;
        }
    #endif
        if ( (config & (1 << 9)) != 0 ) {
            chn_num++;
        }
        if ( (config & (1 << 10)) != 0 ) {
            chn_num++;
        }
        if (0 == deglitch_bypass) {
            xmedia_s32 sleep_time_us = LSADC_GLITCH_TIME_US * chn_num;
            sleep_time_ms = sleep_time_us / 1000;
            if (0 != (sleep_time_us % 1000))
                sleep_time_ms += 1;
        } else {
            sleep_time_ms = 1;
            lsadc_print("bypass in continuous scan mode! sleep_time_ms=%d\n", sleep_time_ms);
        }
    } else {
        sleep_time_ms = 1;
    }

#ifdef ENABLE_ADC_IRQ
    osal_memset(g_cdc_info, 0, sizeof(g_cdc_info));
    osal_spin_lock_irqsave(&g_lsadc_spin_lock, &flag);

    lsadc_reg_write(1, 1, LSADC_INTEN);      //inten enable
    lsadc_reg_write(LSADC_ALL_CHN_INT_MASK, LSADC_ALL_CHN_INT_MASK, LSADC_INTCLR); //clr all intflag
    osal_spin_unlock_irqrestore(&g_lsadc_spin_lock, &flag);
#endif
    lsadc_reg_write(1, 1, LSADC_START);      //start

    osal_msleep(sleep_time_ms); // wait for scan & calculate equ_vaule.
    return 0;
}

static xmedia_s32 lsadc_stop(xmedia_void)
{
#ifdef ENABLE_ADC_IRQ
    LSADC_SPIN_LOCK_FLAG flag;
#endif

    lsadc_reg_write(1, 1, LSADC_STOP);      //stop

#ifdef ENABLE_ADC_IRQ
    osal_spin_lock_irqsave(&g_lsadc_spin_lock, &flag);
    osal_memset(g_cdc_info, 0, sizeof(g_cdc_info));
    lsadc_reg_write(0, 1, LSADC_INTEN);     //inten disable

    osal_spin_unlock_irqrestore(&g_lsadc_spin_lock, &flag);
#endif

    return 0;
}

static xmedia_s32 lsadc_get_chn_value(xmedia_s32 chn)
{
    xmedia_u32 unchn;
#ifdef USE_LSADC_CHANNEL_0
    if (chn < 0 || chn >= LSADC_MAX_CHN_NUM) {
#else
    if (chn <= 0 || chn >= LSADC_MAX_CHN_NUM) {
#endif
        lsadc_print("error chn:%d\n", chn);
        return -1;
    }

    unchn = (xmedia_u32)chn;

#ifdef ENABLE_ADC_IRQ
    return g_cdc_info[unchn].adc_value;
#else
    return lsadc_readl(LSADC_CHNDATA + (unchn << 2));
#endif
}

#ifdef ENABLE_ADC_IRQ
static xmedia_s32 lsadc_irq_proc(xmedia_s32 irq, xmedia_void* devId)
{
    xmedia_u32 intstate;
    xmedia_s32 chn_value;
    xmedia_u32 chn;
    LSADC_SPIN_LOCK_FLAG flag;
    xmedia_u32 int_flag;

    osal_spin_lock_irqsave(&g_lsadc_spin_lock, &flag);

    intstate = lsadc_readl(LSADC_INTSTATUS);

    for (chn = 0; chn < LSADC_MAX_CHN_NUM; chn++) {
        int_flag = 1 << chn;
        if (intstate & (int_flag)) {
            chn_value = lsadc_readl(LSADC_CHNDATA + (chn << 2));
            g_cdc_info[chn].adc_value = chn_value;
            lsadc_reg_write(int_flag, int_flag, LSADC_INTCLR);//clr intflag
        }
    }
    osal_spin_unlock_irqrestore(&g_lsadc_spin_lock, &flag);
    return OSAL_IRQ_HANDLED;
}
#endif

static xmedia_slong lsadc_ioctl (xmedia_u32 cmd, xmedia_ulong arg, xmedia_void* private_data)
{
    xmedia_s32 ret = -1;
    xmedia_s32 param = 0;

    switch (cmd) {
        case LSADC_IOC_MODEL_SEL:
        {
            param = *(xmedia_s32*)(uintptr_t)arg;
            ret = lsadc_model_select(param);
            break;
        }

        case LSADC_IOC_CHN_ENABLE:
        {
            param = *(xmedia_s32*)(uintptr_t)arg;
            ret = lsadc_chn_valid(param, 1);
            break;
        }

        case LSADC_IOC_CHN_DISABLE:
        {
            param = *(xmedia_s32*)(uintptr_t)arg;
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
            param = *(xmedia_s32*)(uintptr_t)arg;
            ret = lsadc_get_chn_value(param);
            break;
        }

        default:
            lsadc_print("error cmd:%08x\n", cmd);
            ret = -1;
    }

    return ret;
}

static struct osal_fileops g_lsadc_fops =
{
    .open           = lsadc_open,
    .release        = lsadc_release,
    .unlocked_ioctl = lsadc_ioctl,
};

static osal_dev_t* g_lsadc_dev;

xmedia_s32 lsadc_init(xmedia_void)
{
    xmedia_s32 ret = 0;

    if (!lsadc_reg) {
        lsadc_reg = (volatile xmedia_void*)osal_ioremap(LSADC_BASE_ADDR, 0x100);

        if (!lsadc_reg) {
            lsadc_print("lsadc ioremap error.\n");
            return -1;
        }
    }

#ifdef ENABLE_ADC_IRQ
    lsadc_print("lsadc_irq=%d.\n", lsadc_irq);
    if (lsadc_irq <= 0) {
        lsadc_irq = LSADC_IRQ_ID;
        lsadc_print("lsadc_irq=LSADC_IRQ_ID(%d).\n", lsadc_irq);
    }

    osal_spin_lock_init(&g_lsadc_spin_lock);

    ret = osal_request_irq(lsadc_irq, lsadc_irq_proc, XMEDIA_NULL, "lsadc", &g_lsadc_fops);

    if (ret != 0) {
        lsadc_print("lsadc request irq error.\n");
        osal_iounmap((xmedia_void*)lsadc_reg);
        return -1;
    }
#endif

    g_lsadc_dev = osal_createdev("lsadc");
    g_lsadc_dev->minor = 255;
    g_lsadc_dev->fops = &g_lsadc_fops;
    ret = osal_registerdevice(g_lsadc_dev);

    if (ret != 0) {
        osal_destroydev(g_lsadc_dev);
        lsadc_print("lsadc register device error.\n");
        osal_free_irq(lsadc_irq, &g_lsadc_fops);
        osal_iounmap((xmedia_void*)lsadc_reg);
    }

    osal_printk("load xm_adc.ko OK!\n");

    return ret;
}


xmedia_void lsadc_exit(xmedia_void)
{
#ifdef ENABLE_ADC_IRQ
    osal_free_irq(lsadc_irq, &g_lsadc_fops);
    osal_spin_lock_destory(&g_lsadc_spin_lock);
#endif

    osal_deregisterdevice(g_lsadc_dev);
    osal_destroydev(g_lsadc_dev);

    lsadc_disable_clock();

    osal_printk("unload xm_adc.ko OK!\n");
}

