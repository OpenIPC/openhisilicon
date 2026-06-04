/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */
#include "hal_dma.h"
#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/delay.h>
#include "securec.h"
#include <asm/uaccess.h>
#include <asm/bitops.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/mm.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/proc_fs.h>
#include <linux/poll.h>
#include <linux/moduleparam.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include "ot_common.h"
#include "ot_osal.h"
#include "mm_ext.h"
#include "hal_spi.h"

#define DMAC_REG_MAX_ADDR 0xffffffff
#define DMAC_REG_MAX_BIT 32
#define DMAC_REG_BASE 0x10280000
#define DMAC_REG_SIZE 0x1000
#define CRG_REG_BASE 0x11010000
#define CRG_REG_SIZE 0x10000
#define CRG_DMA_CLK 0x2A80
#define DMA_DEV_NUM 16

#define MAX_NODE_MEM 0xFF00
#define DMA_SIZE 0x40
#define BYTE_64 64
#define BYTE_128 128

#define DMAC_CHANNEL_NUM 4
#define DMA_NODE_NUM 10
#define DST_ADDR 0x11070008
#define C0_CONFIG_TX 0x83311075  // SPI0 TX chanel DMA request, link: 75; 0x83300075 8bit

static td_void __iomem *g_reg_dma_base_virt = TD_NULL;
static td_void __iomem *g_reg_crg_base_virt = TD_NULL;

typedef struct {
    td_u32 LLI_L;
    td_u32 LLI_H;
    td_u32 noused0;
    td_u32 noused1;
    td_u32 noused2;
    td_u32 noused3;
    td_u32 noused4;
    td_u32 A_COUNT;
    td_u32 SRC_ADDR_L;
    td_u32 SRC_ADDR_H;
    td_u32 DES_ADDR_L;
    td_u32 DES_ADDR_H;
    td_u32 CONFIG;
} dma_list_node;

static int channel_status[DMAC_CHANNEL_NUM];

dma_list_node *dma_node[DMA_NODE_NUM];
td_phys_addr_t dma_phy_addr[DMA_NODE_NUM];

static td_s32 dma_list_node_init(void)
{
    td_u32 i;
    td_s32 ret;
    td_phys_addr_t phys_addr;
    td_void *virt_addr;
    mm_malloc_param malloc_param = {0};

    malloc_param.buf_name = "dma";
    malloc_param.size = BYTE_64 * DMA_NODE_NUM + BYTE_64;
    malloc_param.kernel_only = TD_TRUE;
    ret = cmpi_mmz_malloc_nocache(&malloc_param, &phys_addr, &virt_addr);
    if (ret != TD_SUCCESS) {
        dma_trace(DMA_ERR, "dma malloc err!\n");
        return ret;
    }
    memset_s(virt_addr, BYTE_64 * DMA_NODE_NUM, 0, BYTE_64 * DMA_NODE_NUM);

    for (i = 0; i < DMA_NODE_NUM; i++) {
        dma_node[i] = virt_addr + BYTE_64 * i;
        dma_phy_addr[i] = phys_addr + BYTE_64 * i;
    }
    return TD_SUCCESS;
}

static void dma_list_node_free(void)
{
    cmpi_mmz_free(dma_phy_addr[0], dma_node[0]);
}

static void dma_set_node(unsigned long long memaddr, unsigned int length)
{
    td_u32 i;
    td_u32 target_num = 0;
    td_u32 len[DMA_NODE_NUM];
    td_u64 src_addr[DMA_NODE_NUM];
    unsigned int data_len = length;
    while (data_len >= MAX_NODE_MEM) {
        len[target_num] = MAX_NODE_MEM;
        target_num++;
        data_len -= MAX_NODE_MEM;
    }
    if (data_len != 0) {
        len[target_num] = data_len;
        target_num++;
    }

    for (i = 0; i < target_num; i++) {
        src_addr[i] = memaddr + i * MAX_NODE_MEM;
    }

    for (i = 0; i < target_num; i++) {
        if (i == (target_num - 1)) {
            dma_node[i]->LLI_L = 0x0;
            dma_node[i]->LLI_H = 0x0;
        } else {
             /* 2 enable link list */
            dma_node[i]->LLI_L = (((td_u64)dma_phy_addr[i + 1]) & 0xffffffc0) + 0x2;
            /* 3: link node address bit32 bit31 is zero */
            dma_node[i]->LLI_H = (((td_u64)dma_phy_addr[i + 1]) >> DMAC_REG_MAX_BIT) & 0x3;
        }
        dma_node[i]->SRC_ADDR_L = src_addr[i] & DMAC_REG_MAX_ADDR;
        dma_node[i]->SRC_ADDR_H = ((td_u64)src_addr[i] >> DMAC_REG_MAX_BIT) & DMAC_REG_MAX_ADDR;
        dma_node[i]->DES_ADDR_L = DST_ADDR & DMAC_REG_MAX_ADDR;
        dma_node[i]->DES_ADDR_H = ((td_u64)DST_ADDR >> DMAC_REG_MAX_BIT) & DMAC_REG_MAX_ADDR;
        dma_node[i]->A_COUNT = len[i];
        if (i == (target_num - 1)) {
            dma_node[i]->CONFIG = C0_CONFIG_TX & 0xF00FFFFF;
        } else {
            dma_node[i]->CONFIG = C0_CONFIG_TX;
        }
    }
}

static inline unsigned int edmac_cx_lli_l(unsigned char chn)
{
    return (0x800 + (chn) * 0x40);
}
static inline unsigned int edmac_cx_lli_h(unsigned char chn)
{
    return (0x804 + (chn) * 0x40);
}
static inline unsigned int edmac_cx_cnt0(unsigned char chn)
{
    return (0x81c + (chn) * 0x40);
}
static inline unsigned int edmac_cx_src_addr_l(unsigned char chn)
{
    return (0x820 + (chn) * 0x40);
}
static inline unsigned int edmac_cx_src_addr_h(unsigned char chn)
{
    return (0x824 + (chn) * 0x40);
}
static inline unsigned int edmac_cx_dest_addr_l(unsigned char chn)
{
    return (0x828 + (chn) * 0x40);
}
static inline unsigned int edmac_cx_dest_addr_h(unsigned char chn)
{
    return (0x82c + (chn) * 0x40);
}

void dmac_m2p_transfer(unsigned char chn, unsigned long long memaddr, unsigned int length)
{
    spi_enable();
    if (length <= MAX_NODE_MEM) {
        dmac_writel(g_reg_dma_base_virt, edmac_cx_src_addr_l(chn), memaddr & DMAC_REG_MAX_ADDR);
        dmac_writel(g_reg_dma_base_virt, edmac_cx_src_addr_h(chn),
            ((td_u64)memaddr >> DMAC_REG_MAX_BIT) & DMAC_REG_MAX_ADDR);
        dmac_writel(g_reg_dma_base_virt, edmac_cx_dest_addr_l(chn), DST_ADDR & DMAC_REG_MAX_ADDR);
        dmac_writel(g_reg_dma_base_virt, edmac_cx_dest_addr_h(chn),
            ((td_u64)DST_ADDR >> DMAC_REG_MAX_BIT) & DMAC_REG_MAX_ADDR);
        dmac_writel(g_reg_dma_base_virt, edmac_cx_lli_l(chn), 0x0);
        dmac_writel(g_reg_dma_base_virt, edmac_cx_lli_h(chn), 0x0);
        dmac_writel(g_reg_dma_base_virt, edmac_cx_cnt0(chn), length);
        dmac_writel(g_reg_dma_base_virt, edmac_cx_config(chn), C0_CONFIG_TX);
    } else {
        dmac_writel(g_reg_dma_base_virt, edmac_cx_src_addr_l(chn), memaddr & DMAC_REG_MAX_ADDR);
        dmac_writel(g_reg_dma_base_virt, edmac_cx_src_addr_h(chn),
            ((td_u64)memaddr >> DMAC_REG_MAX_BIT) & DMAC_REG_MAX_ADDR);
        dmac_writel(g_reg_dma_base_virt, edmac_cx_dest_addr_l(chn), DST_ADDR & DMAC_REG_MAX_ADDR);
        dmac_writel(g_reg_dma_base_virt, edmac_cx_dest_addr_h(chn),
            ((td_u64)DST_ADDR >> DMAC_REG_MAX_BIT) & DMAC_REG_MAX_ADDR);
        /* 2 enable link list */
        dmac_writel(g_reg_dma_base_virt, edmac_cx_lli_l(chn), (dma_phy_addr[0] & 0xffffffc0) + 0x2);
        /* 3: link node address bit32 bit31 is zero */
        dmac_writel(g_reg_dma_base_virt, edmac_cx_lli_h(chn), (((td_u64)dma_phy_addr[0]) >> DMAC_REG_MAX_BIT) & 0x3);
        dmac_writel(g_reg_dma_base_virt, edmac_cx_cnt0(chn), MAX_NODE_MEM);
        dma_set_node(memaddr + MAX_NODE_MEM, length - MAX_NODE_MEM);
        dmac_writel(g_reg_dma_base_virt, edmac_cx_config(chn), C0_CONFIG_TX);
    }
}

void *dmac_get_base(void)
{
    return g_reg_dma_base_virt;
}

static void dma_clk_cfg(void)
{
    g_reg_dma_base_virt = ioremap((unsigned long)CRG_REG_BASE, (unsigned long)CRG_REG_SIZE);
    /* DMA */
    dmac_writel(g_reg_dma_base_virt, CRG_DMA_CLK, 0x30); /* dma reset&cken */
}

static td_s32 __init ot_dma_init(td_void)
{
    g_reg_crg_base_virt = ioremap((unsigned long)CRG_REG_BASE, (unsigned long)CRG_REG_SIZE);
    if (g_reg_crg_base_virt == TD_NULL) {
        dma_trace(DMA_ERR, "kernel: ioremap crg base failed!\n");
        return -ENOMEM;
    }

    g_reg_dma_base_virt = ioremap((unsigned long)DMAC_REG_BASE, (unsigned long)DMAC_REG_SIZE);
    if (g_reg_dma_base_virt == TD_NULL) {
        dma_trace(DMA_ERR, "kernel: ioremap dma base failed!\n");
        return -ENOMEM;
    }
    dma_trace(DMA_ERR, "kernel: ssp initial ok!\n");
    return 0;
}

int dma_init(void)
{
    int i;
    int ret;

    dma_clk_cfg();
    ot_dma_init();
    dmac_writel(g_reg_dma_base_virt, EDMAC_INT_TC1_RAW, EDMAC_ALL_CHAN_CLR);
    dmac_writel(g_reg_dma_base_virt, EDMAC_INT_TC2_RAW, EDMAC_ALL_CHAN_CLR);
    dmac_writel(g_reg_dma_base_virt, EDMAC_INT_ERR1_RAW, EDMAC_ALL_CHAN_CLR);
    dmac_writel(g_reg_dma_base_virt, EDMAC_INT_ERR2_RAW, EDMAC_ALL_CHAN_CLR);
    dmac_writel(g_reg_dma_base_virt, EDMAC_INT_TC1_MASK, EDMAC_INT_ENABLE_ALL_CHAN);
    dmac_writel(g_reg_dma_base_virt, EDMAC_INT_TC2_MASK, EDMAC_INT_ENABLE_ALL_CHAN);
    dmac_writel(g_reg_dma_base_virt, EDMAC_INT_ERR1_MASK, 0x0);
    dmac_writel(g_reg_dma_base_virt, EDMAC_INT_ERR2_MASK, 0x0);
    dmac_writel(g_reg_dma_base_virt, EDMAC_INT_ERR3_MASK, 0x0);
    for (i = 0; i < DMAC_CHANNEL_NUM; i++) {
        dmac_writel(g_reg_dma_base_virt, edmac_cx_config(i), 0x0);
    }
    ret = dma_list_node_init();
    if (ret != TD_SUCCESS) {
        dma_trace(DMA_ERR, "dma init err!\n");
    }
    return ret;
}

static td_void __exit ot_dma_exit(td_void)
{
    iounmap((td_void *)g_reg_dma_base_virt);
    g_reg_dma_base_virt = TD_NULL;

    iounmap((td_void *)g_reg_crg_base_virt);
    g_reg_crg_base_virt = TD_NULL;
}

void dma_exit()
{
    int i;
    for (i = 0; i < DMAC_CHANNEL_NUM; i++) {
        dmac_writel(g_reg_dma_base_virt, edmac_cx_config(i), 0x0);
    }
    dma_list_node_free();
    ot_dma_exit();
}

int dmac_channel_alloc(void)
{
    unsigned int i;
    unsigned int val;

    val = dmac_readl(g_reg_dma_base_virt, EDMAC_CH_STAT);
    for (i = 0; i < DMAC_CHANNEL_NUM; i++) {
        if ((channel_status[i] == 0) && (((0x1 << i) & val) == 0)) {
            channel_status[i] = 1;
            return i;
        }
    }

    return -1;
}

int dmac_channel_free(unsigned int channel)
{
    if (channel < DMAC_CHANNEL_NUM) {
        channel_status[channel] = 0;
    }
    return 0;
}
