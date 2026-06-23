/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "sensor_spi.h"
#include "securec.h"
#include "sensor_spi_hal.h"

#ifdef __LITEOS__
static td_char g_sensor[64]; /* sensor structure size 64 */
#else
static td_u32 g_spi_csn[SPI_MAX_NUM] = {1, 0};
#ifndef MODULE
static td_char g_sensor[64]; /* sensor structure size 64 */
#else
static td_char *g_sensor = "";
#endif
#endif
static td_u32 g_bus_num = 0;
static td_u32 g_csn = 0;

#ifndef MODULE
osal_setup_num_param(g_bus_num, g_bus_num);
osal_setup_num_param(g_csn, g_csn);
osal_setup_str_param(g_sensor, g_sensor);
#else
module_param(g_bus_num, uint, S_IRUGO);
MODULE_PARM_DESC(g_bus_num, "spi bus number");

module_param(g_csn, uint, S_IRUGO);
MODULE_PARM_DESC(g_csn, "chip select number");

/* some sensor has special dev addr */
module_param(g_sensor, charp, S_IRUGO);
MODULE_PARM_DESC(g_sensor, "sensor name");
#endif

#ifndef __LITEOS__
struct spi_master *g_ot_master[SPI_MAX_NUM];
struct spi_device *g_ot_spi[SPI_MAX_NUM][2]; /* spi size 2 */

#define SPI_MSG_NUM 30
typedef struct ot_spi_message_s {
    struct spi_transfer t;
    struct spi_message  m;
    td_uchar   buf[8]; /* buf size 8 */
} spi_message_s;

typedef struct ot_spi_message_info_s {
    td_s32 msg_idx;
    spi_message_s spi_msg_array[SPI_MSG_NUM];
} spi_message_info_s;

static spi_message_info_s g_spi_msg = {0};

#define spi_spin_lock(lock, flags)    spin_lock_irqsave(&(lock), (flags))
#define spi_spin_unlock(lock, flags)  spin_unlock_irqrestore(&(lock), (flags))
#define spi_err_trace(dev, ...)       dev_err(dev, ##__VA_ARGS__)
#endif

#ifdef __LITEOS__
td_s32 ssp_write_alt(td_u32 spi_bus, td_u32 spi_csn,
    td_u32 dev_addr, td_u32 dev_addr_byte_num,
    td_u32 reg_addr, td_u32 reg_addr_byte_num,
    td_u32 data, td_u32 data_byte_num)
{
    td_u8 buf[0x10];
    ot_spi_ioc_transfer transfer[1];
    td_s32 retval;
    td_s32 index = 0;
    td_u32 dev_width, reg_width, data_width;

    dev_width = dev_addr_byte_num;
    reg_width = reg_addr_byte_num;
    data_width = data_byte_num;

    (td_void)memset_s(transfer, sizeof(transfer), 0, sizeof(transfer));
    transfer[0].tx_buf = (td_char *)buf;
    transfer[0].rx_buf = (td_char *)buf;
    transfer[0].len = dev_width + reg_width + data_width;

    (td_void)memset_s(buf, sizeof(buf), 0, sizeof(buf));

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
        osal_printk("ssp_write_alt: SPI_IOC_MESSAGE error \n");
        retval = -1;
    }
    return retval;
}

#else
/* This function will be called in interrupt route. So use spi_async, can't call spi_sync here. */
td_s32 ssp_write_alt(td_u32 spi_bus, td_u32 spi_csn,
    td_u32 addr1, td_u32 addr1bytenum,
    td_u32 addr2, td_u32 addr2bytenum,
    td_u32 data, td_u32 databytenum)
{
    struct spi_master   *master = g_ot_master[spi_bus];
    struct spi_device   *spi = g_ot_spi[spi_bus][spi_csn];
    struct spi_transfer *t = TD_NULL;
    struct spi_message  *m = TD_NULL;
    td_u8 *buf = TD_NULL;
    td_s32 status;
    td_ulong flags = 0;
    td_s32 buf_idx = 0;
    td_s32 idx = g_spi_msg.msg_idx;

    ot_unused(addr1bytenum);

    status = 0;

    g_spi_msg.msg_idx++;
    if (g_spi_msg.msg_idx > SPI_MSG_NUM - 1) {
        g_spi_msg.msg_idx = 0;
    }

    buf = g_spi_msg.spi_msg_array[idx].buf;
    t   = &g_spi_msg.spi_msg_array[idx].t;
    m   = &g_spi_msg.spi_msg_array[idx].m;

    /* check spi_message is or no finish */
    spi_spin_lock(master->queue_lock, flags);
    if (m->state != TD_NULL) {
        spi_spin_unlock(master->queue_lock, flags);
        spi_err_trace(&spi->dev, "%s, %d line: spi_message no finish!\n", __func__, __LINE__);
        return -EFAULT;
    }
    spi_spin_unlock(master->queue_lock, flags);

    spi->mode = SPI_MODE_3 | SPI_LSB_FIRST;

    (td_void)memset_s(buf, sizeof(g_spi_msg.spi_msg_array[idx].buf), 0, sizeof(g_spi_msg.spi_msg_array[idx].buf));

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
        spi_err_trace(&spi->dev, "%s: spi_async() error!\n", __func__);
        status = -EFAULT;
    }

    return status;
}

/* This function can't be called in interrupt route because spi_sync will schedule out. */
td_s32 ssp_read_alt(td_u32 spi_bus, td_u32 spi_csn, td_u8 devaddr, td_u8 addr, td_u8 *data)
{
    struct spi_master *master = g_ot_master[spi_bus];
    struct spi_device *spi = g_ot_spi[spi_bus][spi_csn];
    td_s32 status;
    td_ulong flags = 0;
    static struct spi_transfer  t;
    static struct spi_message   m;
    static td_u8 buf[8]; /* buf size 8 */
    td_s32 buf_idx = 0;

    status = 0;
    /* check spi_message is or no finish */
    spi_spin_lock(master->queue_lock, flags);
    if (m.state != TD_NULL) {
        spi_spin_unlock(master->queue_lock, flags);
        spi_err_trace(&spi->dev, "\n******%s, %d line: spi_message no finish!*****\n", __func__, __LINE__);
        return -EFAULT;
    }
    spi_spin_unlock(master->queue_lock, flags);

    spi->mode = SPI_MODE_3 | SPI_LSB_FIRST;

    (td_void)memset_s(buf, sizeof(buf), 0, sizeof(buf));
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
        spi_err_trace(&spi->dev, "%s: spi_async() error!\n", __func__);
        status = -EFAULT;
    }

    *data = buf[2]; /* buf[2] */

    osal_printk("func:%s rx_buf = %#x, %#x, %#x\n", __func__, buf[0], buf[1], buf[2]); /* buf[2] */
    return status;
}
#endif

td_s32 ot_ssp_write(td_u32 spi_bus, td_u32 spi_csn,
                    td_u32 addr1, td_u32 addr1bytenum,
                    td_u32 addr2, td_u32 addr2bytenum,
                    td_u32 data, td_u32 databytenum)
{
    if ((addr1bytenum > 1) || (addr2bytenum > 2) || (databytenum > 2)) { /* case 2 and case 3 */
        osal_printk("addr1_num: %d, addr2_num: %d, data_num: %d, bit_width not support now.\n",
            addr1bytenum, addr2bytenum, databytenum);
        return -1;
    }

    if (spi_bus >= SPI_MAX_NUM) {
        osal_printk("spi_bus: %d,spi bus num can't be larger than %d\n",
            spi_bus, SPI_MAX_NUM);
        return -1;
    }
    if ((spi_bus == 0) && (spi_csn != 0)) {
        osal_printk("spi_bus: %d,spi_csn must be 0\n", spi_bus);
        return -1;
    }

    return ssp_write_alt(spi_bus, spi_csn, addr1, addr1bytenum, addr2, addr2bytenum, data, databytenum);
}

static td_s32 ot_dev_isp_spi_unregister(td_void)
{
    td_s32 i;
    isp_bus_callback bus_cb = {0};
    bus_cb.pfn_isp_write_ssp_data = TD_NULL;
    if (ckfn_isp() && (ckfn_isp_register_bus_callback())) {
        for (i = 0; i < ISP_MAX_DEV; i++) {
            call_isp_register_bus_callback(i, ISP_BUS_TYPE_SSP, &bus_cb);
        }
    } else {
        osal_printk("unregister ssp_write_callback to isp failed, ssp exit is failed!\n");
        return -1;
    }

    return 0;
}

static td_s32 ot_dev_isp_spi_register(td_void)
{
    td_s32 i, ret;
    isp_bus_callback bus_cb = {0};

    bus_cb.pfn_isp_write_ssp_data = ot_ssp_write;
    if (ckfn_isp() && (ckfn_isp_register_bus_callback())) {
        for (i = 0; i < ISP_MAX_DEV; i++) {
            ret = call_isp_register_bus_callback(i, ISP_BUS_TYPE_SSP, &bus_cb);
            if (ret != TD_SUCCESS) {
                osal_printk("register ssp_write_callback to isp[%d] failed!\n", i);
            }
        }
    } else {
        osal_printk("register ssp_write_callback to isp failed, ssp init is failed!\n");
        return -1;
    }

    return 0;
}


#ifdef __LITEOS__
td_s32 spi_mod_init(td_void *args)
{
    td_s32 ret;
    spi_module_params *spi_module_param = (spi_module_params *)args;

    if (spi_module_param != TD_NULL) {
        g_bus_num = spi_module_param->bus_num;
        g_csn = spi_module_param->u32csn;
        (td_void)memcpy_s(g_sensor, sizeof(spi_module_param->c_sensor),
                          spi_module_param->c_sensor, sizeof(spi_module_param->c_sensor));
    }

    ret = ot_dev_isp_spi_register();

    return ret;
}

#else
td_s32 spi_mod_init(td_void)
{
    td_s32                status = 0;
    struct spi_master   *master = TD_NULL;
    struct device       *dev = TD_NULL;
    td_char                spi_name[128] = {0};
    td_s32 bus_num;
    td_u32 csn;
    td_s32 ret;

    for (bus_num = 0; bus_num < SPI_MAX_NUM; bus_num++) {
        master = spi_busnum_to_master(bus_num);
        for (csn = 0; csn < g_spi_csn[bus_num]; csn++) {
            if (master == TD_NULL) {
                spi_err_trace(NULL, "csn:%d bus_num:%d spi_busnum_to_master() error!\n", csn, bus_num);
                status = -ENXIO;
                goto end0;
            }

            g_ot_master[bus_num] = master;

            ret =  snprintf_s(spi_name, sizeof(spi_name), sizeof(spi_name) - 1, "%s.%d", dev_name(&master->dev), csn);
            if (ret < 0) {
                return TD_FAILURE;
            }

            dev = bus_find_device_by_name(&spi_bus_type, TD_NULL, spi_name);
            if (dev == TD_NULL) {
                spi_err_trace(NULL, "chipselect %d has not been used\n", csn);
                status = -ENXIO;
                goto end0;
            }

            g_ot_spi[bus_num][csn] = to_spi_device(dev);
            if (g_ot_spi[bus_num][csn] == TD_NULL) {
                spi_err_trace(dev, "to_spi_device() error!\n");
                status = -ENXIO;
                goto end1;
            }
        }
    }

    status = ot_dev_isp_spi_register();
end1:
    put_device(dev);
    if (status == 0) {
        osal_printk("load sensor_spi.ko ....OK!\n");
    }

end0:
    return status;
}
#endif

#ifndef __LITEOS__
td_void spi_mod_exit(td_void)
{
    td_s32 ret;
    ret = ot_dev_isp_spi_unregister();
    if (ret != 0) {
        return;
    }

    osal_printk("unload sensor_spi.ko ....OK!\n");
}
#endif
