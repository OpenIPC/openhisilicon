/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef __HAL_DMA_H__
#define __HAL_DMA_H__

#include <linux/io.h>
#include <ot_type.h>

#define DMA_ERR KERN_ALERT
#define DMA_INFO KERN_DEBUG
#define dma_trace(level, fmt, ...) printk(level fmt, ##__VA_ARGS__)

#define MAX_TRANSFER_BYTES 0xffff

#define EDMAC_INT_STAT 0x0
#define EDMAC_INT_TC1 0x4
#define EDMAC_INT_TC2 0x8
#define EDMAC_INT_ERR1 0xc
#define EDMAC_INT_ERR2 0x10
#define EDMAC_INT_ERR3 0x14

#define EDMAC_INT_TC1_MASK 0x18
#define EDMAC_INT_TC2_MASK 0x1c
#define EDMAC_INT_ERR1_MASK 0x20
#define EDMAC_INT_ERR2_MASK 0x24
#define EDMAC_INT_ERR3_MASK 0x28

#define EDMAC_INT_TC1_RAW 0x600
#define EDMAC_INT_TC2_RAW 0x608
#define EDMAC_INT_ERR1_RAW 0x610
#define EDMAC_INT_ERR2_RAW 0x618
#define EDMAC_INT_ERR3_RAW 0x620

#define EDMAC_CH_PRI 0x688
#define EDMAC_CH_STAT 0x690
#define EDMAC_DMA_CTRL 0x698
#define EDMAC_CX_DISABLE 0x0

#define EDMAC_ALL_CHAN_CLR 0xff
#define EDMAC_INT_ENABLE_ALL_CHAN 0xff

#define EDMAC_CONFIG_SRC_INC (1 << 31)
#define EDMAC_CONFIG_DST_INC (1 << 30)

#define EDMAC_CONFIG_SRC_WIDTH_SHIFT 16
#define EDMAC_CONFIG_DST_WIDTH_SHIFT 12
#define EDMAC_WIDTH_8BIT 0b0
#define EDMAC_WIDTH_16BIT 0b1
#define EDMAC_WIDTH_32BIT 0b10
#define EDMAC_WIDTH_64BIT 0b11
#ifdef CONFIG_64BIT
#define EDMAC_MEM_BIT_WIDTH EDMAC_WIDTH_64BIT
#else
#define EDMAC_MEM_BIT_WIDTH EDMAC_WIDTH_32BIT
#endif

#define EDMAC_MAX_BURST_WIDTH 16
#define EDMAC_MIN_BURST_WIDTH 1
#define EDMAC_CONFIG_SRC_BURST_SHIFT 24
#define EDMAC_CONFIG_DST_BURST_SHIFT 20

#define EDMAC_LLI_ALIGN 0x40
#define EDMAC_LLI_DISABLE 0x0
#define EDMAC_LLI_ENABLE 0x2

#define EDMAC_CXCONFIG_SIGNAL_SHIFT 0x4
#define EDMAC_CXCONFIG_MEM_TYPE 0x0
#define EDMAC_CXCONFIG_DEV_MEM_TYPE 0x1
#define EDMAC_CXCONFIG_TSF_TYPE_SHIFT 0x2
#define EDMAC_CXCONFIG_LLI_START 0x1

#define EDMAC_CXCONFIG_ITC_EN 0x1
#define EDMAC_CXCONFIG_ITC_EN_SHIFT 0x1

static inline unsigned int dmac_readl(void* addr, unsigned int offset)
{
    return (*((volatile int *)((uintptr_t)(addr) + (offset))));
}

static inline void dmac_writel(void* addr, unsigned int offset, unsigned int value)
{
    *((volatile unsigned int *)((uintptr_t)(addr) + (offset))) = value;
}

static inline unsigned int edmac_cx_config(unsigned char chn)
{
    return (0x830 + (chn) * 0x40);
}

void dmac_m2p_transfer(unsigned char chn, unsigned long long memaddr, unsigned int length);
int dmac_channel_free(unsigned int channel);
int dmac_channel_alloc(void);
void *dmac_get_base(void);
int dma_init(void);
void dma_exit(void);
#endif