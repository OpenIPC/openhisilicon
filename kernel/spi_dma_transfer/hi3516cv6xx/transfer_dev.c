/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>
#include <linux/mod_devicetable.h>
#include <linux/gpio.h>
#include "sys_ext.h"
#include "ot_osal.h"
#include "ot_common.h"
#include "transfer_data.h"
#include "hal_spi.h"
#include "hal_dma.h"
#include "mm_ext.h"
#include "transfer_dev.h"

#define GPIO_GRP_BIT_NUM 8
typedef struct  {
    unsigned int te_on;
    unsigned int gpio_grp_num;
    unsigned int gpio_bit_num;
} transfer_dev_ctl_t;

static td_s32 g_dma_irq_num = -1;
static td_s32 g_te_irq_num = -1;
static int g_test_show = 0;
transfer_dev_ctl_t g_te_cfg = {
    .te_on = 1,
    .gpio_grp_num = 7, /* GPIO7 */
    .gpio_bit_num = 1
};
module_param_named(test_show, g_test_show, int, 0644);

int transfer_open(struct inode *inode, struct file *file)
{
    return 0;
}

int transfer_close(struct inode *inode, struct file *file)
{
    return 0;
}


long transfer_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    unsigned int __user *argp = (unsigned int __user *)arg;
    int ret = 0;

    struct transfer_frame_info frame_info;

    if (argp == NULL) {
        return -1;
    }

    switch (_IOC_NR(cmd)) {
        case _IOC_NR(TRANSFER_SEND_FRAME): {
            if (copy_from_user(&frame_info, argp, sizeof(struct transfer_frame_info))) {
                return -1;
            }
            ret = transfer_data_send(frame_info.frame_phys_addr, frame_info.size, frame_info.timeout);
            if (ret != 0) {
                printk("transfer_data_send ioctl cmd!\n");
            }
            break;
        }

        default: {
            printk("Invalid transfer ioctl cmd!\n");
            ret = -1;
            break;
        }
    }
    return ret;
}

static struct file_operations g_transfer_fops = {
    .owner = THIS_MODULE,
    .unlocked_ioctl = transfer_ioctl,
    .open = transfer_open,
    .release = transfer_close
};

static struct miscdevice g_transfer_dev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "transfer",
    .fops = &g_transfer_fops,
};

static inline unsigned short rgb888_to_rgb565(unsigned char red, unsigned char green, unsigned char blue)
{
     /* red discartd low 3 bit, then  move 11 bit right;
        green discartd low 2 bit, then  move 5 bit right;
        blue discartd low 3 bit */
    return ((red >> 3) << 11 | (green >> 2) << 5 | (blue >> 3));
}
#define Red            0x00FF0000
static void data_fill(unsigned short int *virt_data_addr, td_u32 len, unsigned int color)
{
    unsigned int i;
    unsigned short int data565;
    unsigned char R, G, B;
    unsigned char *p = NULL;

    B = color;
    G = color >> 8; /* bit[8] is Green */
    R = color >> 16; /* bit[16] is Red */

    data565 = rgb888_to_rgb565(R, G, B);

    /* data565 format pix occupation 2 bytes , total size is len */
    for (i = 0; i < len / 2; i++) {
            virt_data_addr[i] = data565;
    }
    p = (unsigned char *)virt_data_addr;
    printk("rgb565 pix value: %x, little endtion %x, %x\n", data565, p[0], p[1]);
}
static void transfer_test_send_data(void)
{
    td_s32 ret;
    td_u32 len = 240 * 320 * 2;
    mm_malloc_param malloc_param = {0};
    td_phys_addr_t phys_addr;
    td_void *virt_addr;

    /* alloc */
    malloc_param.buf_name = "rgb_data";
    malloc_param.size = len;
    malloc_param.kernel_only = TD_TRUE;
    ret = cmpi_mmz_malloc_nocache(&malloc_param, &phys_addr, &virt_addr);
    if (ret != 0) {
        printk("mmz alloc rgb_data failed\n");
        return;
    }
    /* fill */
    data_fill((unsigned short int *)virt_addr, len, Red);

    printk("send data start len = %u\n", len);
    ret = transfer_data_send(phys_addr, len, -1);
    if (ret != 0) {
        printk("transfer data failed\n");
    } else {
        printk("send data ok\n");
    }
    osal_msleep(1000); /* 1000 ms delay */
    /* free */
    cmpi_mmz_free(phys_addr, virt_addr);
}

static int spi_dma_transfer_init(struct platform_device *pdev)
{
    td_s32 ret;

    g_dma_irq_num = osal_platform_get_irq(pdev, 0);
    if (g_dma_irq_num < 0) {
        printk("can not find dma irq num\n");
        return -1;
    }
    printk("spi_dma_transfer init g_dma_irq_num=%d\n", g_dma_irq_num);
    if (g_te_cfg.te_on == 1) {
        g_te_irq_num = gpio_to_irq(g_te_cfg.gpio_grp_num * GPIO_GRP_BIT_NUM + g_te_cfg.gpio_bit_num);
        if (g_te_irq_num < 0) {
            printk("can not find gpio[%d_%d] irq num\n", g_te_cfg.gpio_grp_num, g_te_cfg.gpio_bit_num);
            return -1;
        }
        printk("spi_dma_transfer init g_te_irq_num=%d\n", g_te_irq_num);
    }
    ret = misc_register(&g_transfer_dev);
    if (ret) {
        printk("transfer dev misc_register failed\n");
        return -1;
    }
    ret = dma_init();
    if (ret != TD_SUCCESS) {
        printk("transfer dev init dma failed\n");
        goto unregister_dev;
    }
    ret = ot_spi_dma_ssp_init(0);
    if (ret != TD_SUCCESS) {
        printk("transfer dev init spi dma ssp failed\n");
        goto dev_dma_exit;
    }
    ret = transfer_data_init(g_dma_irq_num, g_te_irq_num);
    if (ret != TD_SUCCESS) {
        printk("register transfer device failed!\n");
        goto dma_ssp_exit;
    }
    printk("spi_dma_transfer init\n");
    if (g_test_show != 0) {
        printk("DMA SPI Mode Test Send Data\n");
        transfer_test_send_data();
    }
    return 0;
dma_ssp_exit:
    ot_spi_dma_ssp_exit();
dev_dma_exit:
    dma_exit();
unregister_dev:
    misc_deregister(&g_transfer_dev);
    return ret;
}
static int spi_dma_transfer_exit(struct platform_device *pdev)
{
    transfer_data_exit(g_dma_irq_num, g_te_irq_num);

    ot_spi_dma_ssp_exit();

    dma_exit();

    misc_deregister(&g_transfer_dev);

    printk("spi_dma_transfer exit\n");

    return 0;
}

static const struct of_device_id g_transfer_match[] = {
    { .compatible = "vendor,edmacv310" },
    {},
};

MODULE_DEVICE_TABLE(of, g_transfer_match);

static struct platform_driver g_transfer_driver = {
    .probe  = spi_dma_transfer_init,
    .remove = spi_dma_transfer_exit,
    .driver = {
        .name = "spi_dma_transfer",
        .of_match_table = g_transfer_match,
    },
};

osal_module_platform_driver(g_transfer_driver);
MODULE_LICENSE("GPL");
