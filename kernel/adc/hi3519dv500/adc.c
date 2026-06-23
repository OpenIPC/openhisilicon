/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "ot_adc.h"
#include "ot_osal.h"
#include "ot_adc_hal.h"

#ifndef NULL
#define NULL  ((void *)0)
#endif

#define OSDRV_MODULE_VERSION_STRING "OT_LSADC"

#define lsadc_print(fmt, ...) osal_printk("Func:%s, Line:%d, "fmt"\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define OT_LSADC_CONFIG       0x00
#define OT_LSADC_GLITCH       0x04
#define OT_LSADC_TIMESCAN     0x08
#define OT_LSADC_INTEN        0x10
#define OT_LSADC_INTSTATUS    0x14
#define OT_LSADC_INTCLR       0x18
#define OT_LSADC_START        0x1C
#define OT_LSADC_STOP         0x20
#define OT_LSADC_ACTBIT       0x24
#define OT_LSADC_ZERO         0x28
#define OT_LSADC_CHNDATA      0x2C

static volatile void *g_lsadc_reg_base = NULL;
static uintptr_t g_lsadc_irq = LSADC_IRQ_ID;
static osal_spinlock g_lsadc_lock;
static int g_need_iounmap = 0;

static inline void write_reg(unsigned long value, void *addr)
{
    (*(volatile unsigned int *)(addr)) = value;
}

static inline unsigned int read_reg(void *addr)
{
    return *(const volatile unsigned int *)addr;
}

static void write_reg32(unsigned int value, unsigned int mask, void *addr)
{
    unsigned int t;

    t = read_reg(addr);
    t &= ~mask;
    t |= value & mask;
    write_reg(t, addr);
}

static void lsadc_writel(unsigned long value, unsigned long offset)
{
    uintptr_t vir_addr = ((uintptr_t)(g_lsadc_reg_base) + ((LSADC_BASE_ADDR + offset) - LSADC_BASE_ADDR));

    (*(volatile unsigned int *)(vir_addr)) = value;
}

static unsigned int lsadc_readl(unsigned long offset)
{
    uintptr_t vir_addr = ((uintptr_t)(g_lsadc_reg_base) + ((LSADC_BASE_ADDR + offset) - LSADC_BASE_ADDR));

    return *(const volatile unsigned int *)vir_addr;
}

static inline void lsadc_reg_write(unsigned long value, unsigned long mask, unsigned long addr)
{
    unsigned long t;

    t = lsadc_readl(addr);
    t &= ~mask;
    t |= value & mask;
    lsadc_writel(t, addr);
}

uintptr_t *get_adc_irq(void)
{
    return &g_lsadc_irq;
}

volatile void **get_adc_reg_base(void)
{
    return &g_lsadc_reg_base;
}

static int ot_lsadc_open(void *private_data)
{
    ot_adc_unused(private_data);

    lsadc_reg_write(0 << 15, 1 << 15, OT_LSADC_CONFIG); /* 15: unreset analog circuit [15] */
    return 0;
}

static int ot_lsadc_release(void *private_data)
{
    ot_adc_unused(private_data);

    lsadc_reg_write(1 << 15, 1 << 15, OT_LSADC_CONFIG); /* 15: reset analog circuit [15] */
    return 0;
}

/*
 * 0: single scanning mode
 * 1: continuous scanning mode
 * The filter glitch is already enable, only the continuous mode has a filter glitch, and the single mode is invalid.
 */
static int lsadc_model_select(int value)
{
    unsigned int val;
    unsigned long flag;

    val = (unsigned int)value;
    if ((val != 0) && (val != 1)) {
        lsadc_print("error value:%x\n", val);
        return -1;
    }

    osal_spin_lock_irqsave(&g_lsadc_lock, &flag);
    lsadc_reg_write(val << 13, 1 << 13, OT_LSADC_CONFIG); /* 13: [13] bit */
    if (val == 1) {
        lsadc_reg_write(0xc << 20, 0xf << 20, OT_LSADC_CONFIG); /* 20, 0xc: [23:20] bits */

        lsadc_writel(GLITCH, OT_LSADC_GLITCH); /* glitch_sample, must > 0 */
        lsadc_writel(TIME_SCAN, OT_LSADC_TIMESCAN); /* time_scan, must > 20 */
        lsadc_writel(LSADC_ZERO, OT_LSADC_ZERO); /* lsadc_zero */

        lsadc_reg_write(0 << 17, 1 << 17, OT_LSADC_CONFIG); /* 17: [17] bit set glitch not bypass */
    } else {
        lsadc_reg_write(1 << 17, 1 << 17, OT_LSADC_CONFIG); /* 17: [17] bit set glitch bypass */
    }
    osal_spin_unlock_irqrestore(&g_lsadc_lock, &flag);

    return 0;
}

static int lsadc_chn_valid(int chn, int enable)
{
    unsigned long flag;
    unsigned long value;

    osal_spin_lock_irqsave(&g_lsadc_lock, &flag);
    value = enable ? 1 : 0;
    switch (chn) {
        case 0: /* chn 0 */
            lsadc_reg_write(value << 8, 1 << 8, OT_LSADC_CONFIG); /* 8: [8] bit for chn 0 */
            break;
        case 1: /* chn 1 */
            lsadc_reg_write(value << 9, 1 << 9, OT_LSADC_CONFIG); /* 9: [9] bit for chn 1 */
            break;
#if (LSADC_MAX_CHN_NUM == 4)
        case 2: /* chn 2 */
            lsadc_reg_write(value << 10, 1 << 10, OT_LSADC_CONFIG); /* 10: [10] bit for chn 2 */
            break;
        case 3: /* chn 3 */
            lsadc_reg_write(value << 11, 1 << 11, OT_LSADC_CONFIG); /* 11: [11] bit for chn 3 */
            break;
#endif
        default:
            osal_spin_unlock_irqrestore(&g_lsadc_lock, &flag);
            lsadc_print("error chn:%d\n", chn);
            return -1;
    }
    osal_spin_unlock_irqrestore(&g_lsadc_lock, &flag);

    return 0;
}

static int lsadc_enable_clock(void)
{
    void *lsadc_crg_addr = NULL;

    lsadc_crg_addr = osal_ioremap(LSADC_CRG_ADDR, (unsigned long)LSADC_CLOCK_REG_LENGTH);
    if (lsadc_crg_addr == NULL) {
        return -1;
    }

    write_reg32(0x1 << ADC_BIT, 0x1 << ADC_BIT, lsadc_crg_addr); /* 0x1: [1] bit for clock control */
    write_reg32(0x0 << ADC_RESET_BIT, 0x1 << ADC_RESET_BIT, lsadc_crg_addr); /* 0x0: [1] bit for unreset control */

    osal_iounmap((void *)lsadc_crg_addr, LSADC_CLOCK_REG_LENGTH);

    return 0;
}

static void lsadc_disable_clock(void)
{
    void *lsadc_crg_addr = NULL;

    lsadc_crg_addr = osal_ioremap(LSADC_CRG_ADDR, (unsigned long)LSADC_CLOCK_REG_LENGTH);
    if (lsadc_crg_addr == NULL) {
        return;
    }

    write_reg32(0x1 << ADC_RESET_BIT, 0x1 << ADC_RESET_BIT, lsadc_crg_addr); /* 0x1: [1] bit for reset control */
    write_reg32(0x0 << ADC_BIT, 0x1 << ADC_BIT, lsadc_crg_addr); /* 0x1: [1] bit for clock control */

    osal_iounmap((void *)lsadc_crg_addr, LSADC_CLOCK_REG_LENGTH);
}

static int lsadc_start(void)
{
    unsigned long flag;
    osal_spin_lock_irqsave(&g_lsadc_lock, &flag);

#ifdef ENABLE_ADC_IRQ
    lsadc_reg_write(1, 1, OT_LSADC_INTEN); /* int enable */
#endif

    lsadc_reg_write(1, 1, OT_LSADC_START); /* start */

    osal_spin_unlock_irqrestore(&g_lsadc_lock, &flag);

    return 0;
}

static int lsadc_stop(void)
{
    unsigned long flag;
    osal_spin_lock_irqsave(&g_lsadc_lock, &flag);

    lsadc_reg_write(1, 1, OT_LSADC_STOP); /* 1: [1] bit for adc control, stop adc */

#ifdef ENABLE_ADC_IRQ
    lsadc_reg_write(0, 1, OT_LSADC_INTEN); /* 1: [1] bit for int enable control, disable int */
#endif

    osal_spin_unlock_irqrestore(&g_lsadc_lock, &flag);

    return 0;
}

static int lsadc_get_chn_value(unsigned int chn)
{
    if (chn >= LSADC_MAX_CHN_NUM) {
        lsadc_print("error chn:%u\n", chn);
        return -1;
    }

    return lsadc_readl(OT_LSADC_CHNDATA + (chn << 2)); /* 2: each chn has 4 bytes int data, so shift left 2 bits */
}

#ifdef ENABLE_ADC_IRQ
static int lsadc_irq_proc(int irq, void *dev_id)
{
    unsigned int intstate;
    int chn_value;
    unsigned int chn;
    unsigned long flag;
    osal_spin_lock_irqsave(&g_lsadc_lock, &flag);

    intstate = lsadc_readl(OT_LSADC_INTSTATUS);
    lsadc_reg_write(LSADC_CHN_MASK, LSADC_CHN_MASK, OT_LSADC_INTCLR);  /* clr int flag all */

    osal_spin_unlock_irqrestore(&g_lsadc_lock, &flag);

    for (chn = 0; chn < LSADC_MAX_CHN_NUM; chn++) {
        if (intstate & (1 << chn)) {
            chn_value = lsadc_get_chn_value(chn);
            lsadc_print("chn[%u] value:%d\n", chn, chn_value);
        }
    }

    /* do what you want to do in irq */
    return OSAL_IRQ_HANDLED;
}
#endif

static int adc_do_ioctl(unsigned int cmd, void *arg, void *private_data)
{
    int ret;
    int param;
    ot_adc_unused(private_data);

    switch (cmd) {
        case LSADC_IOC_MODEL_SEL:
            param = *(int *)arg;
            ret = lsadc_model_select(param);
            break;

        case LSADC_IOC_CHN_ENABLE:
            param = *(int *)arg;
            ret = lsadc_chn_valid(param, 1);
            break;

        case LSADC_IOC_CHN_DISABLE:
            param = *(int *)arg;
            ret = lsadc_chn_valid(param, 0);
            break;

        case LSADC_IOC_START:
            ret = lsadc_start();
            break;

        case LSADC_IOC_STOP:
            ret = lsadc_stop();
            break;

        case LSADC_IOC_GET_CHNVAL:
            param = *(int *)arg;
            ret = lsadc_get_chn_value(param);
            break;

        default:
            lsadc_print("error cmd:%08x\n", cmd);
            ret = -1;
            break;
    }

    return ret;
}

static osal_ioctl_cmd g_ot_adc_ioctl_cmd_list[] = {
    { LSADC_IOC_MODEL_SEL, adc_do_ioctl },
    { LSADC_IOC_CHN_ENABLE, adc_do_ioctl },
    { LSADC_IOC_CHN_DISABLE, adc_do_ioctl },
    { LSADC_IOC_START, adc_do_ioctl },
    { LSADC_IOC_STOP, adc_do_ioctl },
    { LSADC_IOC_GET_CHNVAL, adc_do_ioctl },
};

/* Kernel Interfaces */
static osal_fileops g_ot_adc_fops = {
    .open     = ot_lsadc_open,
    .release  = ot_lsadc_release,
    .cmd_list = g_ot_adc_ioctl_cmd_list,
    .cmd_cnt  = sizeof(g_ot_adc_ioctl_cmd_list) / sizeof(g_ot_adc_ioctl_cmd_list[0]),
};

static osal_dev *g_lsadc_device = NULL;

static int lsadc_device_init(void)
{
    g_lsadc_device = osal_dev_create("ot_lsadc");
    if (g_lsadc_device == NULL) {
        osal_printk("fail to create dev\n");
        return -1;
    }
    g_lsadc_device->minor = 255; /* dev_minor 255 */
    g_lsadc_device->fops = &g_ot_adc_fops;
    if (osal_dev_register(g_lsadc_device) != 0) {
        lsadc_print("lsadc register device error.\n");
        osal_dev_destroy(g_lsadc_device);
        return -1;
    }

    return 0;
}

int lsadc_init(void)
{
    if (g_lsadc_reg_base == NULL) {
        g_lsadc_reg_base = (volatile void *)osal_ioremap(LSADC_BASE_ADDR, LSADC_REGS_LENGTH);
        if (g_lsadc_reg_base == NULL) {
            lsadc_print("lsadc ioremap error.\n");
            return -1;
        }
        g_need_iounmap = 1;
    }

#ifdef ENABLE_ADC_IRQ
    if (g_lsadc_irq == 0) {
        g_lsadc_irq = LSADC_IRQ_ID;
    }

    if (osal_irq_request(g_lsadc_irq, lsadc_irq_proc, 0, "ot_lsadc", &g_ot_adc_fops) != 0) {
        lsadc_print("lsadc request irq error.\n");
        goto requeset_irq_fail;
    }
#endif

    osal_spin_lock_init(&g_lsadc_lock);

    if (lsadc_device_init() != 0) {
        goto init_device_fail;
    }

    if (lsadc_enable_clock() != 0) {
        lsadc_print("lsadc enable clock error.\n");
        goto enable_clock_fail;
    }
    osal_printk("ot_lsadc init ok. ver=%s, %s.\n", __DATE__, __TIME__);

    return 0;

enable_clock_fail:
    osal_dev_unregister(g_lsadc_device);
    osal_dev_destroy(g_lsadc_device);
init_device_fail:
    osal_spin_lock_destroy(&g_lsadc_lock);

#ifdef ENABLE_ADC_IRQ
    osal_irq_free(g_lsadc_irq, &g_ot_adc_fops);
requeset_irq_fail:
#endif

    if (g_need_iounmap) {
        osal_iounmap((void *)g_lsadc_reg_base, LSADC_REGS_LENGTH);
        g_need_iounmap = 0;
        g_lsadc_reg_base = NULL;
    }

    return -1;
}

void lsadc_exit(void)
{
#ifdef ENABLE_ADC_IRQ
    osal_irq_free(g_lsadc_irq, &g_ot_adc_fops);
#endif

    osal_spin_lock_destroy(&g_lsadc_lock);

    osal_dev_unregister(g_lsadc_device);
    osal_dev_destroy(g_lsadc_device);
    if (g_need_iounmap) {
        osal_iounmap((void *)g_lsadc_reg_base, LSADC_REGS_LENGTH);
        g_need_iounmap = 0;
        g_lsadc_reg_base = NULL;
    }

    lsadc_disable_clock();
    osal_printk("ot_lsadc exit ok.\n");
}
