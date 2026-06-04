/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#include <linux/interrupt.h>
#include "ot_osal.h"
#include "hal_dma.h"
#include "hal_spi.h"

#define EDMA_IRQ_NUM 39
#define EDMA_CHN_NUM 4

struct transfer_dev_t {
    unsigned int edma_channels;
    void *edma_base;
    bool is_busy;
    bool has_data;
    osal_spinlock lock;
    osal_wait wait;
    unsigned long long memaddr;
    unsigned int length;
};

static struct transfer_dev_t transfer_dev;
static unsigned int te_on = 0;

static int dmac_interrupt_handler(int irq, void *dev)
{
    struct transfer_dev_t *transfer = &transfer_dev;
    unsigned int channel_err_status[3]; /* 3: errors at most */
    unsigned int channel_tc_status, channel_status;
    int i;
    unsigned int mask = 0;

    channel_status = dmac_readl(transfer->edma_base, EDMAC_INT_STAT);
    if (!channel_status) {
        printk("channel_status = 0x%x\n", channel_status);
        return IRQ_NONE;
    }

    for (i = 0; i < transfer->edma_channels; i++) {
        if ((channel_status >> i) & 0x1) {
            channel_tc_status = dmac_readl(transfer->edma_base, EDMAC_INT_TC1);
            channel_tc_status = (channel_tc_status >> i) & 0x01;
            if (channel_tc_status) {
                dmac_writel(transfer->edma_base, EDMAC_INT_TC1_RAW, channel_tc_status << i);
            }
            channel_tc_status = dmac_readl(transfer->edma_base, EDMAC_INT_TC2);
            channel_tc_status = (channel_tc_status >> i) & 0x01;
            if (channel_tc_status) {
                dmac_writel(transfer->edma_base, EDMAC_INT_TC2_RAW, channel_tc_status << i);
            }
            channel_err_status[0] = dmac_readl(transfer->edma_base, EDMAC_INT_ERR1);
            channel_err_status[0] = (channel_err_status[0] >> i) & 0x01;
            channel_err_status[1] = dmac_readl(transfer->edma_base, EDMAC_INT_ERR2);
            channel_err_status[1] = (channel_err_status[1] >> i) & 0x01;
            channel_err_status[2] = dmac_readl(transfer->edma_base, EDMAC_INT_ERR3);  /* 2 error index */
            channel_err_status[2] = (channel_err_status[2] >> i) & 0x01; /* 2 error index */
            /* 2 error index */
            if (channel_err_status[0] | channel_err_status[1] | channel_err_status[2]) {
                printk("Error in edmac %d finish!,ERR1 = 0x%x,ERR2 = 0x%x,ERR3 = 0x%x\n",
                    i, channel_err_status[0], channel_err_status[1], channel_err_status[2]); /* 2 error index */
                dmac_writel(transfer->edma_base, EDMAC_INT_ERR1_RAW, 1 << i);
                dmac_writel(transfer->edma_base, EDMAC_INT_ERR2_RAW, 1 << i);
                dmac_writel(transfer->edma_base, EDMAC_INT_ERR3_RAW, 1 << i);
            }
            mask |= (1 << i);
            dmac_writel(transfer->edma_base, edmac_cx_config(i), EDMAC_CX_DISABLE);
            dmac_channel_free(i);
        }
    }
    if (mask) {
        unsigned long flags;
        osal_spin_lock_irqsave(&transfer->lock, &flags);
        transfer->is_busy = false;
        osal_wait_wakeup(&transfer->wait);
        osal_spin_unlock_irqrestore(&transfer->lock, &flags);
    }
    return mask ? IRQ_HANDLED : IRQ_NONE;
}

static irqreturn_t te_interrupt_handler(int irq, void *dev)
{
    int chn;
    unsigned long flags;
    osal_spin_lock_irqsave(&transfer_dev.lock, &flags);
    if (transfer_dev.has_data == false) {
        osal_spin_unlock_irqrestore(&transfer_dev.lock, &flags);
        return IRQ_HANDLED;
    }
    transfer_dev.has_data = false;
    osal_spin_unlock_irqrestore(&transfer_dev.lock, &flags);
    chn = dmac_channel_alloc();
    if (chn < 0) {
        return IRQ_NONE;
    }
    dmac_m2p_transfer(chn, transfer_dev.memaddr, transfer_dev.length);
    return IRQ_HANDLED;
}
int transfer_data_init(td_s32 irq_num, td_s32 irq_gpio_num)
{
    int ret;

    transfer_dev.edma_channels = EDMA_CHN_NUM;
    transfer_dev.edma_base = dmac_get_base();
    transfer_dev.is_busy = false;
    transfer_dev.has_data = false;
    osal_spin_lock_init(&transfer_dev.lock);
    osal_wait_init(&transfer_dev.wait);

    ret = osal_irq_request(irq_num, dmac_interrupt_handler, NULL, "edma", dmac_interrupt_handler);
    if (ret < 0) {
        printk("osal_irq_request dmac_interrupt failed %d\n", ret);
        return ret;
    }
    if (irq_gpio_num > 0) {
        ret = request_threaded_irq(irq_gpio_num, NULL, te_interrupt_handler,
            IRQF_TRIGGER_FALLING | IRQF_ONESHOT, "gpio_te", NULL);
        if (ret < 0) {
            printk("osal_irq_request te_interrupt failed %d\n", ret);
            return ret;
        }
        te_on = 1;
    }
    return 0;
}

void transfer_data_exit(td_u32 irq_num, td_s32 irq_gpio_num)
{
    if (irq_gpio_num > 0) {
        free_irq(irq_gpio_num, NULL);
    }
    osal_irq_free(irq_num, dmac_interrupt_handler);
    osal_wait_destroy(&transfer_dev.wait);
    osal_spin_lock_destroy(&transfer_dev.lock);
}

static int transfer_get_status(const void *param)
{
    int condition = 0;
    unsigned long flags;
    struct transfer_dev_t *transfer = (struct transfer_dev_t*)param;
    osal_spin_lock_irqsave(&transfer->lock, &flags);
    if (transfer->is_busy == false) {
        condition = 1;
    }
    osal_spin_unlock_irqrestore(&transfer->lock, &flags);
    return condition;
}

static int transfer_wait_event_timeout(struct transfer_dev_t *transfer, int (*func)(const void *), int timeout)
{
    int ret;
    if (timeout > 0) {
        ret = osal_wait_timeout_interruptible(&transfer->wait, func, transfer, timeout);
        if (ret < 0) {
            return ret;
        } else if (ret == 0) {
            return -1;
        } else {
            ret = 0;
        }
    } else if (timeout == -1) {
        ret = osal_wait_interruptible(&transfer->wait, func, transfer);
    } else {
        ret = 0;
    }
    return ret;
}

int transfer_data_send(unsigned long long memaddr, unsigned int length, int timeout)
{
    int ret;
    int chn;
    unsigned long flags;
    ret = transfer_wait_event_timeout(&transfer_dev, transfer_get_status, timeout);
    if (ret != 0) {
        return ret;
    }
    osal_spin_lock_irqsave(&transfer_dev.lock, &flags);
    if (transfer_dev.is_busy == true) {
        osal_spin_unlock_irqrestore(&transfer_dev.lock, &flags);
        return -1;
    }
    transfer_dev.is_busy = true;
    transfer_dev.has_data = true;
    transfer_dev.memaddr = memaddr;
    transfer_dev.length = length;
    osal_spin_unlock_irqrestore(&transfer_dev.lock, &flags);
    if (te_on == 0) {
        chn = dmac_channel_alloc();
        if (chn < 0) {
            return -1;
        }
        dmac_m2p_transfer(chn, memaddr, length);
    }
    return 0;
}
