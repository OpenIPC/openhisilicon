/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2006-2019. All rights reserved.
 * Description: Hiir module's init, exit, read, hidog_dev register.
 * Author: Hisilicon multimedia software group
 * Create: 2006-12-16
 */

#include "hiir.h"
#include "hi_osal.h"
#include "hiir_codedef.h"
#include "hi_ir_hal.h"

#ifndef NULL
#define NULL  ((void *)0)
#endif

/* irq */
unsigned int g_ir_irq = 0;

/* io */
#define IR_REG_LENGTH 0x1000
volatile void *g_ir_reg_base = NULL;

#define IR_ENABLE   ((uintptr_t)g_ir_reg_base + 0x00)
#define IR_CONFIG   ((uintptr_t)g_ir_reg_base + 0x04)
#define CNT_LEADS   ((uintptr_t)g_ir_reg_base + 0x08)
#define CNT_LEADE   ((uintptr_t)g_ir_reg_base + 0x0c)
#define CNT_SLEADE  ((uintptr_t)g_ir_reg_base + 0x10)
#define CNT0_B      ((uintptr_t)g_ir_reg_base + 0x14)
#define CNT1_B      ((uintptr_t)g_ir_reg_base + 0x18)
#define IR_BUSY     ((uintptr_t)g_ir_reg_base + 0x1c)
#define IR_DATAH    ((uintptr_t)g_ir_reg_base + 0x20)
#define IR_DATAL    ((uintptr_t)g_ir_reg_base + 0x24)
#define IR_INTM     ((uintptr_t)g_ir_reg_base + 0x28)
#define IR_INTS     ((uintptr_t)g_ir_reg_base + 0x2c)
#define IR_INTC     ((uintptr_t)g_ir_reg_base + 0x30)
#define IR_START    ((uintptr_t)g_ir_reg_base + 0x34)

/* interrupt mask */
#define INTMS_RCV      (1L << 16)
#define INTMS_FRAMERR  (1L << 17)
#define INTMS_OVERFLOW (1L << 18)
#define INTMS_RELEASE  (1L << 19)

#define INT_MASK    (INTMS_RCV | INTMS_FRAMERR | INTMS_OVERFLOW | INTMS_RELEASE)

/* define macro */
#define write_reg(addr, value) ((*(volatile unsigned int *)(addr)) = (value))
#define read_reg(addr)         (*(volatile unsigned int *)(addr))

/* debug */
static int g_dbg_flag = 0;
#define HIIR_PFX "hiir: "
#define hiir_dbg(params...) \
    do { \
        if (g_dbg_flag) { \
            osal_printk(params); \
        } \
    } while (0);

/* unit is ms */
#define DEFAULT_DELAYTIME (200 * 10)

#define HI_IR_MAX_BUF 100
#define DEFAULT_BUF_LEN 8
#define BUF_HEAD g_hiir_dev.buf[g_hiir_dev.head]
#define BUF_TAIL g_hiir_dev.buf[g_hiir_dev.tail]
#define BUF_LAST g_hiir_dev.buf[(g_hiir_dev.head == 0) ? (g_hiir_dev.buf_len - 1) : (g_hiir_dev.head - 1)]

typedef struct {
    hiir_dev_param dev_parm;
    unsigned int head;
    unsigned int tail;
    irkey_info_s buf[HI_IR_MAX_BUF + 1]; /* 1:fifo reserve */
    unsigned int buf_len;

    unsigned int enable_repkey;
    unsigned int repkey_checkflag;
    unsigned int repkey_delaytime; /* ms */

    unsigned int enable_keyup;

    osal_wait_t ir_key_wait;
} hiir_dev_struct;

static hiir_dev_struct g_hiir_dev;

static void repkey_timeout_handler(unsigned long data);
static struct osal_timer g_repkey_timeout_timer;
static int g_need_iounmap = 0;

static void hiir_show_reg(void)
{
    if (g_dbg_flag) {
        osal_printk("HIIR11 REG:###################################################\n");
        osal_printk("%.8x ", read_reg(IR_ENABLE));
        osal_printk("%.8x ", read_reg(IR_CONFIG));
        osal_printk("%.8x ", read_reg(CNT_LEADS));
        osal_printk("%.8x ", read_reg(CNT_LEADE));
        osal_printk("%.8x ", read_reg(CNT_SLEADE));
        osal_printk("%.8x ", read_reg(CNT0_B));
        osal_printk("%.8x ", read_reg(CNT1_B));
        osal_printk("\n");
        osal_printk("%.8x ", read_reg(IR_BUSY));
        osal_printk("%.8x ", read_reg(IR_DATAH));
        osal_printk("%.8x ", read_reg(IR_DATAL));
        osal_printk("%.8x ", read_reg(IR_INTM));
        osal_printk("%.8x ", read_reg(IR_INTS));
        osal_printk("%.8x ", read_reg(IR_INTC));
        osal_printk("%.8x ", read_reg(IR_START));
        osal_printk("\n");
    }
}

static inline unsigned int inc_buf(unsigned int x, unsigned int len)
{
    ++x;

    if (len == 0) {
        return x % DEFAULT_BUF_LEN;
    } else {
        return x % len;
    }
}

static void write_reg32(unsigned int value, unsigned int mask, const void *addr)
{
    unsigned int t;

    t = osal_readl((void *)addr);
    t &= ~mask;
    t |= value & mask;

    osal_writel(t, (void *)addr);
}

static int hiir_enable_clock(void)
{
    void *ir_crg_addr = NULL;

    ir_crg_addr = osal_ioremap(IR_CRG_ADDR, (unsigned long)IR_CLOCK_REG_LENGTH);
    if (ir_crg_addr == NULL) {
        return -1;
    }

    write_reg32(0x1 << IR_BIT, 0x1 << IR_BIT, ir_crg_addr); /* 0x1: [1] bit for clock control */

    osal_iounmap((void *)ir_crg_addr);

    return 0;
}

static void hiir_disable_clock(void)
{
    void *ir_crg_addr = NULL;

    ir_crg_addr = osal_ioremap(IR_CRG_ADDR, (unsigned long)IR_CLOCK_REG_LENGTH);
    if (ir_crg_addr == NULL) {
        return;
    }

    write_reg32(0x0 << IR_BIT, 0x1 << IR_BIT, ir_crg_addr); /* 0x1: [1] bit for clock control */

    osal_iounmap((void *)ir_crg_addr);
}

static void hiir_config(void)
{
    unsigned int value;

    write_reg(IR_ENABLE, 0x01); /* 0x01: for IR control */
    while (read_reg(IR_BUSY)) {
        hiir_dbg("IR_BUSY. Wait...\n");
    }

    value = (g_hiir_dev.dev_parm.codetype << 14);       /* 14: [15:14] bits for code type */
    value |= (g_hiir_dev.dev_parm.code_len - 1) << 8;   /* 8: [13:8] bits for  code len */
    value |= (g_hiir_dev.dev_parm.frequence - 1);       /* [6:0] bits for frequence */
    write_reg(IR_CONFIG, value);

    value = g_hiir_dev.dev_parm.leads_min << 16;    /* 16:  [25:16] bits for min leads */
    value |= g_hiir_dev.dev_parm.leads_max;         /* [9:0] bits fot max leads */
    write_reg(CNT_LEADS, value);

    value = g_hiir_dev.dev_parm.leade_min << 16;    /* 16, bits [24:16] for min leads */
    value |= g_hiir_dev.dev_parm.leade_max;         /* [8:0] bits for max leads */
    write_reg(CNT_LEADE, value);

    value = g_hiir_dev.dev_parm.sleade_min << 16;   /* 16, bits [24:16] for min sleade */
    value |= g_hiir_dev.dev_parm.sleade_max;        /* [8:0] bits for max sleade */
    write_reg(CNT_SLEADE, value);

    value = g_hiir_dev.dev_parm.cnt0_b_min << 16;   /* 16, bits [24:16] for min cnt0 */
    value |= g_hiir_dev.dev_parm.cnt0_b_max;        /* [8:0] bits for max cnt0 */
    write_reg(CNT0_B, value);

    value = g_hiir_dev.dev_parm.cnt1_b_min << 16;   /* 16, bits [24:16] for min cnt1 */
    value |= g_hiir_dev.dev_parm.cnt1_b_max;        /* [8:0] bits for max cnt1 */
    write_reg(CNT1_B, value);

    write_reg(IR_INTM, 0x00);
    write_reg(IR_START, 0x00);
}

static void repkey_timeout_handler(unsigned long data)
{
    osal_del_timer(&g_repkey_timeout_timer);
    g_hiir_dev.repkey_checkflag = 0;
}

static void interrupt_rcv(void)
{
    hiir_dbg("receive data interrupt. 0x%.8x-0x%.8x\n", read_reg(IR_DATAH), read_reg(IR_DATAL));
    write_reg(IR_INTC, 0x01 << 0); /* 0x01: clear interrupt */

    if (g_hiir_dev.enable_repkey) {
        if ((g_hiir_dev.repkey_checkflag) &&
            (BUF_LAST.irkey_datah == read_reg(IR_DATAH)) &&
            (BUF_LAST.irkey_datal == read_reg(IR_DATAL))) {
            hiir_dbg("repeart key [0x%.8x]-[0x%.8x] detective\n", read_reg(IR_DATAH), read_reg(IR_DATAL));
        } else {
            /* repeat key check */
            osal_del_timer(&g_repkey_timeout_timer);
            osal_set_timer(&g_repkey_timeout_timer, g_hiir_dev.repkey_delaytime / 10); /* 10: delaytime means ms*10 */
            g_hiir_dev.repkey_checkflag = 1;
            BUF_HEAD.irkey_datah = read_reg(IR_DATAH);
            BUF_HEAD.irkey_datal = read_reg(IR_DATAL);
            BUF_HEAD.irkey_state_code = HIIR_KEY_DOWN;
            g_hiir_dev.head = inc_buf(g_hiir_dev.head, g_hiir_dev.buf_len);
            osal_wakeup(&(g_hiir_dev.ir_key_wait));
        }
    } else if (!((BUF_LAST.irkey_datah == read_reg(IR_DATAH)) &&
        (BUF_LAST.irkey_datal == read_reg(IR_DATAL)) &&
        (BUF_LAST.irkey_state_code == HIIR_KEY_DOWN))) {
        BUF_HEAD.irkey_datah = read_reg(IR_DATAH);
        BUF_HEAD.irkey_datal = read_reg(IR_DATAL);
        BUF_HEAD.irkey_state_code = HIIR_KEY_DOWN;
        g_hiir_dev.head = inc_buf(g_hiir_dev.head, g_hiir_dev.buf_len);
        osal_wakeup(&(g_hiir_dev.ir_key_wait));
    }
}

static int hiir_interrupt(int irq, void *dev_id)
{
    unsigned int int_state = read_reg(IR_INTS);

    if ((int_state & INT_MASK) == 0) {
        return OSAL_IRQ_NONE;
    }

    hiir_dbg("Enter hiir_interrupt.\n");
    hiir_show_reg();

    if (int_state & INTMS_FRAMERR) {
        hiir_dbg("frame error.\n");
        write_reg(IR_INTC, 0x01 << 1); /* 1: write 0x01 to [1] bit, clear frame error interrupt */
    } else if (int_state & INTMS_OVERFLOW) {
        hiir_dbg("hardware overflow.\n");
        write_reg(IR_INTC, 0x01 << 2); /* 2: write 0x01 to [2] bit, clear hardware overflow interrupt */
    } else if (int_state & INTMS_RELEASE) {
        hiir_dbg("release key interrupt.\n");
        write_reg(IR_INTC, 0x01 << 3); /* 3: write 0x01 to [3] bit, clear release key interrupt */

        osal_del_timer(&g_repkey_timeout_timer);
        g_hiir_dev.repkey_checkflag = 0;

        if ((g_hiir_dev.enable_keyup) && (BUF_LAST.irkey_state_code != HIIR_KEY_UP)) {
            BUF_HEAD = BUF_LAST;
            BUF_HEAD.irkey_state_code = HIIR_KEY_UP;
            g_hiir_dev.head = inc_buf(g_hiir_dev.head, g_hiir_dev.buf_len);
            osal_wakeup(&(g_hiir_dev.ir_key_wait));
        }
    } else if (int_state & INTMS_RCV) {
        interrupt_rcv();
    } else {
        osal_printk("logic Error: int_mask=0x%.8x, int_state=0x%.8x.\n", read_reg(IR_INTM), int_state);
    }

    hiir_dbg("Exit hiir_interrupt.\n");

    return OSAL_IRQ_HANDLED;
}

static long hiir_ioctl(unsigned int cmd, unsigned long arg, void *private_data)
{
    int *p = (int *)(uintptr_t)arg;
    unsigned int min_val, max_val;

    switch (cmd) {
        case IR_IOC_SET_BUF:
            if ((arg > 0) && (arg <= HI_IR_MAX_BUF)) {
                g_hiir_dev.buf_len = arg + 1;

                /* need reset buf pointrt */
                g_hiir_dev.head = 0;
                g_hiir_dev.tail = 0;
            }
            hiir_dbg("IR_IOC_SET_BUF->buf_len=%u\n", g_hiir_dev.buf_len);
            break;
        case IR_IOC_SET_ENABLE_KEYUP:
            g_hiir_dev.enable_keyup = arg;
            hiir_dbg("IR_IOC_SET_ENABLE_KEYUP->enable_keyup=%u\n", g_hiir_dev.enable_keyup);
            break;
        case IR_IOC_SET_ENABLE_REPKEY:
            g_hiir_dev.enable_repkey = arg;
            hiir_dbg("IR_IOC_SET_ENABLE_REPKEY->enable_repkey=%u\n", g_hiir_dev.enable_repkey);
            break;
        case IR_IOC_SET_REPKEY_TIMEOUTVAL:
            if (arg > 0) {
                g_hiir_dev.repkey_delaytime = arg;
            }
            hiir_dbg("IR_IOC_SET_REPKEY_TIMEOUTVAL->repkey_delaytime=%u\n", g_hiir_dev.repkey_delaytime);
            break;
        case IR_IOC_SET_FORMAT:
            if (arg > 3) { /* format:[0,3] */
                osal_printk("Error: IR_IOC_SET_FORMAT->invalid args=%lu\n", arg);
                return -1;
            }
            g_hiir_dev.dev_parm.codetype = arg;
            hiir_dbg("IR_IOC_SET_FORMAT->codetype=%u\n", g_hiir_dev.dev_parm.codetype);
            break;
        case IR_IOC_SET_CODELEN:
            g_hiir_dev.dev_parm.code_len = arg;
            hiir_dbg("IR_IOC_SET_CODELEN->code_len=%u\n", g_hiir_dev.dev_parm.code_len);
            break;
        case IR_IOC_SET_FREQ:
            if ((arg <= 0) || (arg > 128)) { /* 128:max freq */
                osal_printk("Error: IR_IOC_SET_FREQ->invalid args=%lu\n", arg);
                return -1;
            }
            g_hiir_dev.dev_parm.frequence = arg;
            hiir_dbg("IR_IOC_SET_FREQ->frequence=%u\n", g_hiir_dev.dev_parm.frequence);
            break;
        case IR_IOC_SET_LEADS:
            if (osal_copy_from_user(&min_val, p, sizeof(int)) || osal_copy_from_user(&max_val, p + 1, sizeof(int))) {
                return -1;
            }
            g_hiir_dev.dev_parm.leads_min = min_val;
            g_hiir_dev.dev_parm.leads_max = max_val;
            hiir_dbg("IR_IOC_SET_LEADS->leads_min=%u, leads_max=%u\n",
                g_hiir_dev.dev_parm.leads_min, g_hiir_dev.dev_parm.leads_max);
            break;
        case IR_IOC_SET_LEADE:
            if (osal_copy_from_user(&min_val, p, sizeof(int)) || osal_copy_from_user(&max_val, p + 1, sizeof(int))) {
                return -1;
            }
            g_hiir_dev.dev_parm.leade_min = min_val;
            g_hiir_dev.dev_parm.leade_max = max_val;
            hiir_dbg("IR_IOC_SET_LEADE->leade_min=%u, leade_max=%u\n",
                g_hiir_dev.dev_parm.leade_min, g_hiir_dev.dev_parm.leade_max);
            break;
        case IR_IOC_SET_SLEADE:
            if (osal_copy_from_user(&min_val, p, sizeof(int)) || osal_copy_from_user(&max_val, p + 1, sizeof(int))) {
                return -1;
            }
            g_hiir_dev.dev_parm.sleade_min = min_val;
            g_hiir_dev.dev_parm.sleade_max = max_val;
            hiir_dbg("IR_IOC_SET_SLEADE->sleade_min=%u, sleade_max=%u\n",
                g_hiir_dev.dev_parm.sleade_min, g_hiir_dev.dev_parm.sleade_max);
            break;
        case IR_IOC_SET_CNT0_B:
            if (osal_copy_from_user(&min_val, p, sizeof(int)) || osal_copy_from_user(&max_val, p + 1, sizeof(int))) {
                return -1;
            }
            g_hiir_dev.dev_parm.cnt0_b_min = min_val;
            g_hiir_dev.dev_parm.cnt0_b_max = max_val;
            hiir_dbg("IR_IOC_SET_CNT0_B->cnt0_b_min=%u, cnt0_b_max=%u\n",
                g_hiir_dev.dev_parm.cnt0_b_min, g_hiir_dev.dev_parm.cnt0_b_max);
            break;
        case IR_IOC_SET_CNT1_B:
            if (osal_copy_from_user(&min_val, p, sizeof(int)) || osal_copy_from_user(&max_val, p + 1, sizeof(int))) {
                return -1;
            }
            g_hiir_dev.dev_parm.cnt1_b_min = min_val;
            g_hiir_dev.dev_parm.cnt1_b_max = max_val;
            hiir_dbg("IR_IOC_SET_CNT1_B->cnt1_b_min=%u, cnt1_b_max=%u\n",
                g_hiir_dev.dev_parm.cnt1_b_min, g_hiir_dev.dev_parm.cnt1_b_max);
            break;
        case IR_IOC_GET_CONFIG:
            if (osal_copy_to_user((void *)(uintptr_t)arg, &(g_hiir_dev.dev_parm), sizeof(hiir_dev_param))) {
                osal_printk("%s: copy_to_user fail!\n", __func__);
            }
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
            osal_printk("Error: Inappropriate ioctl for device. cmd=%u\n", cmd);
            return -1;
    }

    hiir_config();
    return 0;
}

static int hiir_wait_condition_callback(const void *param)
{
    return (g_hiir_dev.head != g_hiir_dev.tail);
}

static int hiir_read(char *buf, int count, long *f_pos, void *private_data)
{
    irkey_info_s irkey_to_user;
    int res = 0;

retry :
    hiir_dbg("Enter hiir_read : head=%u, tail=%u, buf_len=%u\n", g_hiir_dev.head, g_hiir_dev.tail, g_hiir_dev.buf_len);

    if ((g_hiir_dev.head) == (g_hiir_dev.tail)) {
        res = osal_wait_event_interruptible(&(g_hiir_dev.ir_key_wait), hiir_wait_condition_callback, &g_hiir_dev);
        if (res != 0) {
            return res;
        }
        goto retry;
    } else {
        while (((g_hiir_dev.head) != (g_hiir_dev.tail)) && ((res + sizeof(irkey_info_s)) <= count)) {
            irkey_to_user = BUF_TAIL;
            g_hiir_dev.tail = inc_buf(g_hiir_dev.tail, g_hiir_dev.buf_len);
            if (osal_copy_to_user((buf + res), &irkey_to_user, sizeof(irkey_info_s))) {
                osal_printk("%s: copy_to_user fail!\n", __func__);
            }
            res += sizeof(irkey_info_s);
        }
    }
    return res;
}

static unsigned int hiir_select(osal_poll_t *osal_poll, void *private_data)
{
    hiir_dbg("Enter hiir_select.\n");
    if ((g_hiir_dev.head) != (g_hiir_dev.tail)) {
        return 1;
    }
    osal_poll_wait(osal_poll, &(g_hiir_dev.ir_key_wait));
    return 0;
}

static osal_atomic_t g_hiir_s_available;

static int hiir_open(void *private_data)
{
    int ret;

    hiir_dbg("Enter hiir_open.\n");

    if (osal_atomic_dec_return(&g_hiir_s_available) != 0) {
        ret = osal_atomic_inc_return(&g_hiir_s_available);
        osal_printk("Error: device already open:%d.\n", ret);
        return -1;
    }

    osal_del_timer(&g_repkey_timeout_timer);

    g_dbg_flag = 0;

    /* init g_hiir_dev */
    g_hiir_dev.dev_parm = g_static_dev_param[0]; /* nec format */
    g_hiir_dev.head = 0;
    g_hiir_dev.tail = 0;
    g_hiir_dev.buf_len = DEFAULT_BUF_LEN;
    g_hiir_dev.enable_repkey = 1;
    g_hiir_dev.repkey_checkflag = 0;
    g_hiir_dev.repkey_delaytime = DEFAULT_DELAYTIME;
    g_hiir_dev.enable_keyup = 1;
    osal_wait_init(&g_hiir_dev.ir_key_wait);

    hiir_config();

    return 0;
}

static int hiir_release(void *private_data)
{
    hiir_dbg("Enter hiir_release.\n");

    /* disable HIIR11 */
    write_reg(IR_ENABLE, 0x0);

    if (osal_atomic_inc_return(&g_hiir_s_available) != 1) {
        osal_atomic_dec_return(&g_hiir_s_available);
        return 0;
    }

    osal_del_timer(&g_repkey_timeout_timer);

    osal_wait_destory(&g_hiir_dev.ir_key_wait);

    return 0;
}

static int hiir_write(const char *buf, int size, long *offset, void *private_data)
{
    return 0;
}

static struct osal_fileops g_hiir_fops = {
    open            : hiir_open,
    unlocked_ioctl  : hiir_ioctl,
    poll            : hiir_select,
    read            : hiir_read,
    write           : hiir_write,
    release         : hiir_release,
};

static struct osal_dev *g_hiir_miscdev = NULL;

static int hiir_sw_init(void)
{
    int res;

    if (g_ir_irq <= 0) {
        g_ir_irq = HIIR_DEVICE_IRQ_NO;
    }

    if (g_ir_reg_base == NULL) {
        g_ir_reg_base = (volatile void *)osal_ioremap(IR_REG_BASE, IR_REG_LENGTH);
        if (g_ir_reg_base == NULL) {
            osal_printk("osal_ioremap err. \n");
            return 1;
        }
        g_need_iounmap = 1;
    }

    res = osal_atomic_init(&g_hiir_s_available);
    if (res != 0) {
        osal_printk("Error: init atomic\n");
        if (g_need_iounmap) {
            osal_iounmap((void *)g_ir_reg_base);
            g_need_iounmap = 0;
            g_ir_reg_base = NULL;
        }
        return 1;
    }
    osal_atomic_set(&g_hiir_s_available, 1);

    g_repkey_timeout_timer.function = repkey_timeout_handler;
    res = osal_timer_init(&g_repkey_timeout_timer);
    if (res != 0) {
        if (g_need_iounmap) {
            osal_iounmap((void *)g_ir_reg_base);
            g_need_iounmap = 0;
            g_ir_reg_base = NULL;
        }
        osal_atomic_destory(&g_hiir_s_available);
        return 1;
    }
    return 0;
}

int hiir_init(void)
{
    if (hiir_sw_init() != 0) {
        return -1;
    }

    if (hiir_enable_clock() != 0) {
        osal_printk("Error: enable clock fail\n");
        goto enable_clock_fail;
    }

    /* disable HIIR */
    write_reg(IR_ENABLE, 0x00);

    g_hiir_miscdev = osal_createdev(HIIR_DEVICE_NAME);
    if (g_hiir_miscdev == NULL) {
        osal_printk("Error: can't create dev\n");
        goto cteate_dev_fail;
    }

    g_hiir_miscdev->minor = 255; /* 255:dev_minor */
    g_hiir_miscdev->fops = &g_hiir_fops;
    if (osal_registerdevice(g_hiir_miscdev) != 0) {
        osal_printk("Error: can't register\n");
        goto register_drvice_fail;
    }

    if (osal_request_irq(g_ir_irq, hiir_interrupt, NULL, HIIR_DEVICE_NAME, &hiir_interrupt) != 0) {
        osal_printk("Error: request IRQ(%u) failed\n", g_ir_irq);
        goto request_irq_fail;
    }

    osal_printk("hiir init ok. ver=%s, %s.\n", __DATE__, __TIME__);
    return 0;

request_irq_fail:
    osal_deregisterdevice(g_hiir_miscdev);
register_drvice_fail:
    osal_destroydev(g_hiir_miscdev);
cteate_dev_fail:
    hiir_disable_clock();
enable_clock_fail:
    osal_timer_destory(&g_repkey_timeout_timer);
    osal_atomic_destory(&g_hiir_s_available);
    if (g_need_iounmap) {
        osal_iounmap((void *)g_ir_reg_base);
        g_need_iounmap = 0;
        g_ir_reg_base = NULL;
    }
    return -1;
}

void hiir_exit(void)
{
    osal_free_irq(g_ir_irq, &hiir_interrupt);

    osal_deregisterdevice(g_hiir_miscdev);
    osal_destroydev(g_hiir_miscdev);

    osal_del_timer(&g_repkey_timeout_timer);
    osal_timer_destory(&g_repkey_timeout_timer);

    osal_atomic_destory(&g_hiir_s_available);
    if (g_need_iounmap) {
        osal_iounmap((void *)g_ir_reg_base);
        g_need_iounmap = 0;
        g_ir_reg_base = NULL;
    }

    hiir_disable_clock();

    osal_printk("hiir exit ok.\n");
}
