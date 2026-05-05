/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of sensor_spi.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "sensor_spi.h"

#define SPI_MAX_NUM 2
#define ISP_MAX_DEV 4
#ifndef __HuaweiLite__
static unsigned g_spi_csn[SPI_MAX_NUM] = {1, 0};
#endif
#ifdef __HuaweiLite__
static char g_sensor[64]; /* sensor structer size 64 */
#else
static char *g_sensor = "";
#endif
static unsigned g_bus_num = 0;
static unsigned g_csn = 0;
module_param(g_bus_num, uint, S_IRUGO);
MODULE_PARM_DESC(g_bus_num, "spi bus number");

module_param(g_csn, uint, S_IRUGO);
MODULE_PARM_DESC(g_csn, "chip select number");

/* some sensor has special dev addr */
module_param(g_sensor, charp, S_IRUGO);
MODULE_PARM_DESC(sg_ensor, "sensor name");

#ifndef __HuaweiLite__

struct spi_master *g_hi_master[SPI_MAX_NUM];
struct spi_device *g_hi_spi[SPI_MAX_NUM][2]; /* spi size 2 */

#define SPI_MSG_NUM 30
typedef struct hi_spi_message_s {
    struct spi_transfer t;
    struct spi_message  m;
    unsigned char   buf[8]; /* buf size 8 */
} spi_message_s;

typedef struct hi_spi_message_info_s {
    int msg_idx;
    spi_message_s spi_msg_array[SPI_MSG_NUM];
} spi_message_info_s;

static spi_message_info_s g_spi_msg = {0};
#endif
#ifdef __HuaweiLite__

int ssp_write_alt(unsigned int spi_bus, unsigned int spi_csn,
                  unsigned int dev_addr, unsigned int dev_addr_byte_num,
                  unsigned int reg_addr, unsigned int reg_addr_byte_num,
                  unsigned int data, unsigned int data_byte_num)
{
    unsigned char buf[0x10];
    struct spi_ioc_transfer transfer[1];
    int retval;
    int index = 0;
    unsigned int dev_width, reg_width, data_width;

    dev_width = 1;
    reg_width = 1;
    data_width = 1;

    dev_width = dev_addr_byte_num;
    reg_width = reg_addr_byte_num;
    data_width = data_byte_num;

    osal_memset(transfer, 0, sizeof(transfer));
    transfer[0].tx_buf = (char *)buf;
    transfer[0].rx_buf = (char *)buf;
    transfer[0].len = dev_width + reg_width + data_width;

    osal_memset(buf, 0, sizeof(buf));

    if (dev_width == 2) { /* case 2 */
        buf[index++] = dev_addr >> 8; /* dev_addr shift 8 */
    }
    buf[index++] = dev_addr & 0xFF;

    if (reg_width == 2) { /* case 2 */
        buf[index++] = reg_addr >> 8; /* reg_addr_shift 8 */
    }
    buf[index++] = reg_addr & 0xFF;

    if (data_width == 2) { /* case 2 */
        buf[index++] = data >> 8; /* data shift 8 */
    }
    buf[index++] = data & 0xFF;

    g_bus_num = spi_bus;
    g_csn = spi_csn;
    retval = spi_dev_set(g_bus_num, g_csn, &transfer[0]);
    if (retval != transfer[0].len) {
        printf("ssp_write_alt: SPI_IOC_MESSAGE error \n");
        retval = -1;
    }

end0:
    return retval;
}

#else

void reverse8(unsigned char *buf, unsigned int len)
{
    unsigned int i;
    for (i = 0; i < len; i++) {
        buf[i] = ((buf[i] & 0x55) << 1) | ((buf[i] & 0xAA) >> 1);
        buf[i] = ((buf[i] & 0x33) << 2) | ((buf[i] & 0xCC) >> 2); /* shift by 2 */
        buf[i] = ((buf[i] & 0x0F) << 4) | ((buf[i] & 0xF0) >> 4); /* shift by 4 */
    }
}

/* This function will be called in interrupt route. So use spi_async, can't call spi_sync here. */
int ssp_write_alt(unsigned int spi_bus, unsigned int spi_csn,
                  unsigned int addr1, unsigned int addr1bytenum,
                  unsigned int addr2, unsigned int addr2bytenum,
                  unsigned int data, unsigned int databytenum)
{
    struct spi_master   *master = g_hi_master[spi_bus];
    struct spi_device   *spi = g_hi_spi[spi_bus][spi_csn];
    struct spi_transfer *t = HI_NULL;
    struct spi_message  *m = HI_NULL;
    unsigned char       *buf = HI_NULL;
    int                 status;
    unsigned long       flags;
    int                 buf_idx = 0;
    int idx = g_spi_msg.msg_idx;

    status = 0;

    g_spi_msg.msg_idx++;
    if (g_spi_msg.msg_idx > SPI_MSG_NUM - 1) {
        g_spi_msg.msg_idx = 0;
    }

    buf = g_spi_msg.spi_msg_array[idx].buf;
    t   = &g_spi_msg.spi_msg_array[idx].t;
    m   = &g_spi_msg.spi_msg_array[idx].m;

    /* check spi_message is or no finish */
    spin_lock_irqsave(&master->queue_lock, flags);
    if (m->state != HI_NULL) {
        spin_unlock_irqrestore(&master->queue_lock, flags);
        dev_err(&spi->dev, "%s, %s, %d line: spi_message no finish!\n", __FILE__, __func__, __LINE__);
        return -EFAULT;
    }
    spin_unlock_irqrestore(&master->queue_lock, flags);

    spi->mode = SPI_MODE_3 | SPI_LSB_FIRST;

    osal_memset(buf, 0, sizeof(g_spi_msg.spi_msg_array[idx].buf));

    buf[buf_idx++] = addr1;

    if (addr2bytenum == 2) { /* case 2 bytes */
        buf[buf_idx++] = addr2 >> 8; /* addr2 shift by 8 */
    }
    buf[buf_idx++] = addr2;

    if (databytenum == 2) { /* case 2 bytes */
        buf[buf_idx++] = data >> 8; /* data shift by 8 */
    }
    buf[buf_idx++] = data;
    t->tx_buf    = buf;
    t->rx_buf    = buf;
    t->len       = buf_idx;
    t->cs_change = 1;
    t->speed_hz  = 2000000; /* 2000000 hz */
    t->bits_per_word = 8; /* 8 bits per word */

    spi_message_init(m);
    spi_message_add_tail(t, m);
    m->state = m;
    status = spi_async(spi, m);
    if (status) {
        dev_err(&spi->dev, "%s: spi_async() error!\n", __func__);
        status = -EFAULT;
    }

    return status;
}
#endif

hi_s32 hi_ssp_write(unsigned int spi_bus, unsigned int spi_csn,
                    unsigned int addr1, unsigned int addr1bytenum,
                    unsigned int addr2, unsigned int addr2bytenum,
                    unsigned int data, unsigned int databytenum)
{
    if ((addr1bytenum > 1) || (addr2bytenum > 2) || (databytenum > 2)) { /* case 2 and case 3 */
        printk("addr1_num: %d, addr2_num: %d, data_num: %d, bit_width not support now.\n",
               addr1bytenum, addr2bytenum, databytenum);
        return -1;
    }

    if (spi_bus >= SPI_MAX_NUM) {
        printk("spi_bus: %d,spi bus num can't be larger than %d\n",
               spi_bus, SPI_MAX_NUM);
        return -1;
    }
    if ((spi_bus == 0) && (spi_csn != 0)) {
        printk("spi_bus: %d,spi_csn must be 0\n", spi_bus);
        return -1;
    }

    return ssp_write_alt(spi_bus, spi_csn, addr1, addr1bytenum, addr2, addr2bytenum, data, databytenum);
}

#ifndef __HuaweiLite__
/* This function can't be called in interrupt route because spi_sync will schedule out. */
int ssp_read_alt(unsigned int spi_bus, unsigned int spi_csn,
                 unsigned char devaddr, unsigned char addr, unsigned char *data)
{
    struct spi_master           *master = g_hi_master[spi_bus];
    struct spi_device           *spi = g_hi_spi[spi_bus][spi_csn];
    int                         status;
    unsigned long               flags;
    static struct spi_transfer  t;
    static struct spi_message   m;
    static unsigned char        buf[8]; /* buf size 8 */
    int                         buf_idx = 0;

    status = 0;
    /* check spi_message is or no finish */
    spin_lock_irqsave(&master->queue_lock, flags);
    if (m.state != HI_NULL) {
        spin_unlock_irqrestore(&master->queue_lock, flags);
        dev_err(&spi->dev, "\n******%s, %s, %d line: spi_message no finish!*****\n", __FILE__, __func__, __LINE__);
        return -EFAULT;
    }
    spin_unlock_irqrestore(&master->queue_lock, flags);

    spi->mode = SPI_MODE_3 | SPI_LSB_FIRST;

    osal_memset(buf, 0, sizeof(buf));
    buf[buf_idx++] = devaddr | 0x80;

    buf[buf_idx++] = addr;
    buf[buf_idx++] = 0;

    t.tx_buf    = buf;
    t.rx_buf    = buf;
    t.len       = buf_idx;
    t.cs_change = 1;
    t.speed_hz  = 2000000; /* 2000000 hz */
    t.bits_per_word = 8; /* 8 bits per word */

    spi_message_init(&m);
    spi_message_add_tail(&t, &m);
    m.state = &m;
    status  = spi_sync(spi, &m);
    if (status) {
        dev_err(&spi->dev, "%s: spi_async() error!\n", __func__);
        status = -EFAULT;
    }

    *data = buf[2]; /* buf[2] */

    printk("func:%s rx_buf = %#x, %#x, %#x\n", __func__, buf[0], buf[1], buf[2]); /* buf[2] */
    return status;
}
#endif

#ifdef __HuaweiLite__
int sensor_spi_dev_init(void *args)
{
    int i;
    isp_bus_callback bus_cb = {0};

    spi_module_params *spi_module_param = (spi_module_params *)args;

    if (spi_module_param != HI_NULL) {
        g_bus_num = spi_module_param->bus_num;
        g_csn = spi_module_param->u32csn;
        osal_memcpy(g_sensor, spi_module_param->c_sensor, sizeof(spi_module_param->c_sensor));
    }
    bus_cb.pfn_isp_write_ssp_data = hi_ssp_write;
    if (ckfn_isp_register_bus_callback()) {
        for (i = 0; i < ISP_MAX_DEV; i++) {
            call_isp_register_bus_callback(i, ISP_BUS_TYPE_SSP, &bus_cb);
        }
    } else {
        printk("register ssp_write_callback to isp failed, ssp init is failed!\n");
        return -1;
    }

    return 0;
}

#else

static int __init sensor_spi_dev_init(void)
{
    int i;
    int                 status = 0;
    struct spi_master   *master = HI_NULL;
    struct device       *dev = HI_NULL;
    char                spi_name[128] = {0};
    int bus_num = 0;
    int csn = 0;

    isp_bus_callback bus_cb = {0};
    bus_cb.pfn_isp_write_ssp_data = hi_ssp_write;
    if (ckfn_isp() && (ckfn_isp_register_bus_callback())) {
        for (i = 0; i < ISP_MAX_DEV; i++) {
            call_isp_register_bus_callback(i, ISP_BUS_TYPE_SSP, &bus_cb);
        }
    } else {
        printk("register ssp_write_callback to isp failed, ssp init is failed!\n");
        return -1;
    }

    for (bus_num = 0; bus_num < SPI_MAX_NUM; bus_num++) {
        master = spi_busnum_to_controller(bus_num);
        for (csn = 0; csn < g_spi_csn[bus_num]; csn++) {
            if (!master) {
                dev_err(NULL, "spi_busnum_to_master() error!\n");
                status = -ENXIO;
                goto end0;
            }

            g_hi_master[bus_num] = master;
            osal_snprintf(spi_name, sizeof(spi_name), "%s.%d", dev_name(&master->dev), csn);
            dev = bus_find_device_by_name(&spi_bus_type, HI_NULL, spi_name);
            if (dev == HI_NULL) {
                dev_err(NULL, "chipselect %d has not been used\n", csn);
                status = -ENXIO;
                goto end1;
            }

            g_hi_spi[bus_num][csn] = to_spi_device(dev);
            if (g_hi_spi[bus_num][csn] == HI_NULL) {
                dev_err(dev, "to_spi_device() error!\n");
                status = -ENXIO;
                goto end1;
            }
        }
    }

end1:
    put_device(dev);
    printk("load sensor_spi.ko for %s...OK !\n", CHIP_NAME);

end0:
    return status;
}
#endif

#ifndef __HuaweiLite__
static void __exit sensor_spi_dev_exit(void)
{
    printk("unload sensor_spi.ko for %s...OK!\n", CHIP_NAME);
}
#endif

#ifndef __HuaweiLite__
module_init(sensor_spi_dev_init);
module_exit(sensor_spi_dev_exit);
MODULE_AUTHOR("BVT OSDRV");
MODULE_LICENSE("GPL");
MODULE_ALIAS("sensor spidev");
#endif
