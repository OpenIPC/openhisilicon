/*
 * Copyright (c); XMEDIA. All rights reserved.
 */

#include "icm42670.h"
#include <linux/slab.h>
#include <linux/kernel.h>
#include <asm/dma.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/workqueue.h>
#include "xmedia_msensor.h"
#include "msensor_ext.h"
#include "msensor_gpio.h"
#include "msensor_spi.h"
#include "inv_imu_defs.h"
#include "inv_imu_transport.h"
#include "inv_imu_driver.h"
#include "inv_imu_apex.h"

#ifdef TRANSFER_I2C
#include "i2c_dev.h"
#elif (defined TRANSFER_SPI)
#include "spi_dev.h"
#endif

extern xmedia_u64 drv_sys_get_timestamp(void);

/*
 * Select communication link between SmartMotion and IMU
 */
#define SERIF_TYPE UI_SPI4
// #define SERIF_TYPE           UI_I2C

/*
 * Set power mode flag
 * Set this flag to run example in low-noise mode.
 * Reset this flag to run example in low-power mode.
 * Note: low-noise mode is not available with sensor data frequencies less than 12.5Hz.
 */
#define USE_LOW_NOISE_MODE 1

/*
 * Select Fifo resolution Mode (default is low resolution mode)
 * Low resolution mode: 16 bits data format
 * High resolution mode: 20 bits data format
 * Warning: Enabling High Res mode will force FSR to 16g and 2000dps
 */
#define USE_HIGH_RES_MODE 0

/*
 * Select to use FIFO or to read data from registers
 */
#define USE_FIFO 1

/*
 * Print raw data or scaled data
 * 0 : print raw accel, gyro and temp data
 * 1 : print scaled accel, gyro and temp data in g, dps and degree Celsius
 */
#define SCALED_DATA_G_DPS 1

static const xmedia_u32 g_msensor_spi_num = 1;

static icm42670_dev_info *g_icm42670_dev[XMEDIA_MSENSOR_MAX_DEV_NUM] = { XMEDIA_NULL };
static xmedia_msensor_data g_imu_data[XMEDIA_MSENSOR_MAX_DEV_NUM] = { 0 };

static osal_spinlock_t g_lock_get_data[XMEDIA_MSENSOR_MAX_DEV_NUM];

static xmedia_msensor_attr g_msensor_mode[XMEDIA_MSENSOR_MAX_DEV_NUM] = { 0 };
static xmedia_u64 g_time_backup[XMEDIA_MSENSOR_MAX_DEV_NUM][TIME_RECORD_CNT] = { 0 };
static xmedia_u32 g_every_data_cnt_backup[XMEDIA_MSENSOR_MAX_DEV_NUM][DATA_RECORD_CNT] = { 0 };
static xmedia_u8 g_time_cnt[XMEDIA_MSENSOR_MAX_DEV_NUM] = { 0 };
static xmedia_u8 g_data_count[XMEDIA_MSENSOR_MAX_DEV_NUM] = { 0 };
static xmedia_u8 g_timer_frist_flag[XMEDIA_MSENSOR_MAX_DEV_NUM] = { 0 };

/* for debug */
msensor_triger_mode g_msensor_triger_mode[XMEDIA_MSENSOR_MAX_DEV_NUM] = { TRIGER_TIMER };
xmedia_u8 g_msensor_fifo_en[XMEDIA_MSENSOR_MAX_DEV_NUM] = { 1, 1 };

static struct inv_imu_device icm_driver[XMEDIA_MSENSOR_MAX_DEV_NUM] = { 0 };
static struct inv_imu_serif icm_serif[XMEDIA_MSENSOR_MAX_DEV_NUM] = { 0 };

EXPORT_SYMBOL_GPL(g_msensor_triger_mode);
EXPORT_SYMBOL_GPL(g_msensor_fifo_en);

#define MAX_FIFO_LEN 1024

xmedia_u32 g_data_num[XMEDIA_MSENSOR_MAX_DEV_NUM] = { 0 };

xmedia_u64 g_pts_now[XMEDIA_MSENSOR_MAX_DEV_NUM] = { 0 };

#define IMU_ACC_DATA_X(dev)     g_imu_data[dev].acc_buf.acc_data[g_imu_data[dev].acc_buf.data_num].x
#define IMU_ACC_DATA_Y(dev)     g_imu_data[dev].acc_buf.acc_data[g_imu_data[dev].acc_buf.data_num].y
#define IMU_ACC_DATA_Z(dev)     g_imu_data[dev].acc_buf.acc_data[g_imu_data[dev].acc_buf.data_num].z
#define IMU_ACC_DATA_TEMP(dev)  g_imu_data[dev].acc_buf.acc_data[g_imu_data[dev].acc_buf.data_num].temperature
#define IMU_ACC_DATA_PTS(dev)   g_imu_data[dev].acc_buf.acc_data[g_imu_data[dev].acc_buf.data_num].pts

#define IMU_GYRO_DATA_X(dev)    g_imu_data[dev].gyro_buf.gyro_data[g_imu_data[dev].gyro_buf.data_num].x
#define IMU_GYRO_DATA_Y(dev)    g_imu_data[dev].gyro_buf.gyro_data[g_imu_data[dev].gyro_buf.data_num].y
#define IMU_GYRO_DATA_Z(dev)    g_imu_data[dev].gyro_buf.gyro_data[g_imu_data[dev].gyro_buf.data_num].z
#define IMU_GYRO_DATA_TEMP(dev) g_imu_data[dev].gyro_buf.gyro_data[g_imu_data[dev].gyro_buf.data_num].temperature
#define IMU_GYRO_DATA_PTS(dev)  g_imu_data[dev].gyro_buf.gyro_data[g_imu_data[dev].gyro_buf.data_num].pts

// #define get_fifo_data(dev, high, low)
//(xmedia_s16)(((xmedia_s16)(g_icm42670_dev[dev]->fifo_buf[high]) << 8) & 0xff00) | (g_icm42670_dev[dev]->fifo_buf[low]
//& 0xff)

/* get mod */
#define remainder(a, b) ((a) - (b) * ((a) / (b)))

#define safe_kfree(memory)             \
    do {                               \
        if ((memory) != XMEDIA_NULL) { \
            osal_kfree(memory);        \
            memory = XMEDIA_NULL;      \
        }                              \
    } while (0)

/* Function definition */
static xmedia_u8 *get_register_cache_addr(struct inv_imu_device *s, xmedia_u32 reg);
static xmedia_s32 write_sreg(xmedia_s32 dev, struct inv_imu_device *s, xmedia_u8 reg, xmedia_u32 len,
                             const xmedia_u8 *buf);
static xmedia_s32 read_sreg(xmedia_s32 dev, struct inv_imu_device *s, xmedia_u8 reg, xmedia_u32 len, xmedia_u8 *buf);
static xmedia_s32 write_mclk_reg(xmedia_s32 dev, struct inv_imu_device *s, uint16_t regaddr, xmedia_u8 wr_cnt,
                                 const xmedia_u8 *buf);
static xmedia_s32 read_mclk_reg(xmedia_s32 dev, struct inv_imu_device *s, uint16_t regaddr, xmedia_u8 rd_cnt,
                                xmedia_u8 *buf);
xmedia_u64 inv_imu_get_time_us(void);

static xmedia_s32 icm42670_transfer_read(xmedia_s32 dev, xmedia_u8 reg_addr, xmedia_u8 *reg_data, xmedia_u32 cnt,
                                         xmedia_bool fifo_mode)
{
    xmedia_s32 ret;
#ifdef TRANSFER_I2C
    ret = msensor_i2c_read(g_icm42670_dev[dev]->client, reg_addr, reg_data, cnt);
#elif defined TRANSFER_SPI
    ret = xmedia_msensor_ssp_read_alt(dev, reg_addr, reg_data, cnt, fifo_mode);
#else
    ret = XMEDIA_FAILURE;
#endif
    return ret;
}

static xmedia_s32 icm42670_transfer_write(xmedia_s32 dev, xmedia_u8 reg_addr, const xmedia_u8 *reg_data, xmedia_u32 cnt)
{
    xmedia_s32 ret;
#ifdef TRANSFER_I2C
    ret = msensor_i2c_write(g_imu_data[dev]->client, reg_addr, reg_data, cnt);
#elif defined TRANSFER_SPI
    xmedia_unused(cnt);
    ret = xmedia_msensor_ssp_write_alt(dev, reg_addr, reg_data);
#else
    ret = XMEDIA_FAILURE;
#endif
    return ret;
}

icm42670_dev_info *chip_get_dev_info(xmedia_s32 dev)
{
    return g_icm42670_dev[dev];
}

static xmedia_s32 icm42670_reset_fifo(xmedia_s32 dev)
{
    xmedia_u8 ch, ret;

    ret = icm42670_transfer_read(dev, (xmedia_u8)SIGNAL_PATH_RESET, &ch, 1, XMEDIA_FALSE);
    if (ret != XMEDIA_SUCCESS) {
        MSENSOR_ERR_TRACE("dev %d reset fifo failed\n", dev);
        return -EAGAIN;
    }

    ch |= 0x04;

    ret = icm42670_transfer_write(dev, (xmedia_u8)SIGNAL_PATH_RESET, &ch, 1);
    if (ret != XMEDIA_SUCCESS) {
        MSENSOR_ERR_TRACE("dev %d reset fifo failed\n", dev);
        return -EAGAIN;
    }

    g_icm42670_dev[dev]->flag_fifo_incomming = 0;
    return XMEDIA_SUCCESS;
}

xmedia_s32 chip_fifo_data_reset(xmedia_s32 dev)
{
    xmedia_s32 ret;

    if (g_icm42670_dev[dev]->fifo_en == 0) {
        return XMEDIA_SUCCESS;
    }

    ret = icm42670_reset_fifo(dev);
    if (ret != XMEDIA_SUCCESS) {
        MSENSOR_ERR_TRACE("dev %d reset fifo failed\n", dev);
        return -EAGAIN;
    }

    return ret;
}

/* Static function */
static xmedia_u8 *get_register_cache_addr(struct inv_imu_device *s, xmedia_u32 reg)
{
    struct inv_imu_transport *t = (struct inv_imu_transport *)s;

    switch (reg) {
        case PWR_MGMT0:
            return &(t->register_cache.pwr_mgmt0_reg);
        case GYRO_CONFIG0:
            return &(t->register_cache.gyro_config0_reg);
        case ACCEL_CONFIG0:
            return &(t->register_cache.accel_config0_reg);
        case TMST_CONFIG1_MREG1:
            return &(t->register_cache.tmst_config1_reg);
        default:
            return (xmedia_u8 *)0;
    }
}

xmedia_s32 read_mclk_reg(xmedia_s32 dev, struct inv_imu_device *s, uint16_t regaddr, xmedia_u8 rd_cnt, xmedia_u8 *buf)
{
    xmedia_u8 data;
    xmedia_u8 blk_sel = (regaddr & 0xFF00) >> 8;
    xmedia_s32 status = 0;

    // Have IMU not in IDLE mode to access MCLK domain
    status |= inv_imu_switch_on_mclk(dev, s);

    // optimize by changing BLK_SEL only if not NULL
    if (blk_sel)
        status |= write_sreg(dev, s, (xmedia_u8)BLK_SEL_R & 0xff, 1, &blk_sel);

    data = (regaddr & 0x00FF);
    status |= write_sreg(dev, s, (xmedia_u8)MADDR_R, 1, &data);
    osal_udelay(10); // wait 10us
    status |= read_sreg(dev, s, (xmedia_u8)M_R, rd_cnt, buf);
    osal_udelay(10); // wait 10us

    if (blk_sel) {
        data = 0;
        status |= write_sreg(dev, s, (xmedia_u8)BLK_SEL_R, 1, &data);
    }

    // switch OFF MCLK if needed
    status |= inv_imu_switch_off_mclk(dev, s);

    return status;
}

xmedia_s32 read_sreg(xmedia_s32 dev, struct inv_imu_device *s, xmedia_u8 reg, xmedia_u32 len, xmedia_u8 *buf)
{
    // First field of struct inv_imu_device is assumed to be a struct inv_imu_serif object.
    // So let's cast s to struct inv_imu_serif and ignore the rest of struct inv_imu_device.
    struct inv_imu_serif *serif = (struct inv_imu_serif *)s;

    if (len > serif->max_read)
        return -5; // len error: -5

    if (serif->read_reg(dev, serif, reg, buf, len) != 0)
        return -3; // read error: -3

    return 0;
}

xmedia_s32 write_sreg(xmedia_s32 dev, struct inv_imu_device *s, xmedia_u8 reg, xmedia_u32 len, const xmedia_u8 *buf)
{
    // First field of struct inv_imu_device is assumed to be a struct inv_imu_serif object.
    // So let's cast s to struct inv_imu_serif and ignore the rest of struct inv_imu_device.
    struct inv_imu_serif *serif = (struct inv_imu_serif *)s;

    if (len > serif->max_write) {
        MSENSOR_ERR_TRACE("dev %d len %d error, max %d !\n", dev, len, serif->max_write);
        return -5; // len error: -5
    }

    if (serif->write_reg(dev, serif, reg, buf, len) != 0) {
        MSENSOR_ERR_TRACE("dev %d write reg err!\n", dev);
        return -3; // write error: -3
    }

    return 0;
}

static xmedia_s32 write_mclk_reg(xmedia_s32 dev, struct inv_imu_device *s, uint16_t regaddr, xmedia_u8 wr_cnt,
                                 const xmedia_u8 *buf)
{
    xmedia_u8 data;
    xmedia_u8 blk_sel = (regaddr & 0xFF00) >> 8;
    xmedia_s32 status = 0;
    xmedia_u8 i;

    // Have IMU not in IDLE mode to access MCLK domain
    status |= inv_imu_switch_on_mclk(dev, s);

    // optimize by changing BLK_SEL only if not NULL
    if (blk_sel)
        status |= write_sreg(dev, s, (xmedia_u8)BLK_SEL_W, 1, &blk_sel);

    data = (regaddr & 0x00FF);
    status |= write_sreg(dev, s, (xmedia_u8)MADDR_W, 1, &data);
    for (i = 0; i < wr_cnt; i++) {
        status |= write_sreg(dev, s, (xmedia_u8)M_W, 1, &buf[i]);
        // wait 16 MCLK (4MHz) clock cycles
        osal_udelay(10); // wait 10us
    }

    if (blk_sel) {
        data = 0;
        status = write_sreg(dev, s, (xmedia_u8)BLK_SEL_W, 1, &data);
    }

    status |= inv_imu_switch_off_mclk(dev, s);

    return status;
}

xmedia_s32 inv_imu_read_reg(xmedia_s32 dev, struct inv_imu_device *s, xmedia_u32 reg, xmedia_u32 len, xmedia_u8 *buf)
{
    xmedia_u32 i;
    xmedia_s32 rc = 0;

    for (i = 0; i < len; i++) {
        xmedia_u8 *cache_addr = get_register_cache_addr(s, reg + i);

        if (cache_addr) {
            buf[i] = *cache_addr;
        } else {
            if (!(reg & 0x10000)) {
                rc |= read_mclk_reg(dev, s, ((reg + i) & 0xFFFF), 1, &buf[i]);
            } else {
                rc |= read_sreg(dev, s, (xmedia_u8)reg + i, len - i, &buf[i]);
                break;
            }
        }
    }

    return rc;
}

xmedia_s32 inv_imu_write_reg(xmedia_s32 dev, struct inv_imu_device *s, xmedia_u32 reg, xmedia_u32 len,
                             const xmedia_u8 *buf)
{
    xmedia_u32 i;
    xmedia_s32 rc = 0;
    for (i = 0; i < len; i++) {
        xmedia_u8 *cache_addr = get_register_cache_addr(s, reg + i);

        if (cache_addr)
            *cache_addr = buf[i];

        if (!(reg & 0x10000))
            rc |= write_mclk_reg(dev, s, ((reg + i) & 0xFFFF), 1, &buf[i]);
    }

    if (reg & 0x10000)
        rc |= write_sreg(dev, s, (xmedia_u8)reg, len, buf);

    return rc;
}

xmedia_s32 inv_imu_switch_on_mclk(xmedia_s32 dev, struct inv_imu_device *s)
{
    xmedia_s32 status = 0;
    xmedia_u8 data;
    xmedia_u64 start;

    struct inv_imu_transport *t = (struct inv_imu_transport *)s;

    /* set IDLE bit only if it is not set yet */
    if (t->need_mclk_cnt == 0) {
        status |= inv_imu_read_reg(dev, s, PWR_MGMT0, 1, &data);
        data |= PWR_MGMT0_IDLE_MASK;
        status |= inv_imu_write_reg(dev, s, PWR_MGMT0, 1, &data);
        if (status) {
            MSENSOR_ERR_TRACE("dev %d status: %d\n", dev, status);
            return status;
        }

        /* Check if MCLK is ready */
        start = inv_imu_get_time_us();
        do {
            status = inv_imu_read_reg(dev, s, MCLK_RDY, 1, &data); // |=
            if (inv_imu_get_time_us() - start > 10000) { // max time interval: 10000us
                status |= -1;
                break;
            }
        } while ((status != 0) || !(data & MCLK_RDY_MCLK_RDY_MASK));
    } else {
        /* Make sure it is already on */
        status |= inv_imu_read_reg(dev, s, PWR_MGMT0, 1, &data);

        if (0 == (data &= PWR_MGMT0_IDLE_MASK)) {
            MSENSOR_ERR_TRACE("dev %d status: %d\n", dev, status);
            status |= -1;
        }
    }
    /* Increment the counter to keep track of number of MCLK requesters */
    t->need_mclk_cnt++;

    return status;
}

xmedia_s32 inv_imu_switch_off_mclk(xmedia_s32 dev, struct inv_imu_device *s)
{
    xmedia_s32 status = 0;
    xmedia_u8 data;
    struct inv_imu_transport *t = (struct inv_imu_transport *)s;

    /* Reset the IDLE but only if there is one requester left */
    if (t->need_mclk_cnt == 1) {
        status |= inv_imu_read_reg(dev, s, PWR_MGMT0, 1, &data);
        data &= ~PWR_MGMT0_IDLE_MASK;
        status |= inv_imu_write_reg(dev, s, PWR_MGMT0, 1, &data);
    } else {
        /* Make sure it is still on */
        status |= inv_imu_read_reg(dev, s, PWR_MGMT0, 1, &data);
        if (0 == (data &= PWR_MGMT0_IDLE_MASK))
            status |= -1;
    }

    /* Decrement the counter */
    t->need_mclk_cnt--;

    return status;
}

xmedia_s32 inv_imu_init_transport(xmedia_s32 dev, struct inv_imu_device *s)
{
    xmedia_s32 status = 0;
    struct inv_imu_transport *t = (struct inv_imu_transport *)s;

    status |= read_sreg(dev, s, (xmedia_u8)PWR_MGMT0, 1, &(t->register_cache.pwr_mgmt0_reg));
    status |= read_sreg(dev, s, (xmedia_u8)GYRO_CONFIG0, 1, &(t->register_cache.gyro_config0_reg));
    status |= read_sreg(dev, s, (xmedia_u8)ACCEL_CONFIG0, 1, &(t->register_cache.accel_config0_reg));
    status |= read_mclk_reg(dev, s, (TMST_CONFIG1_MREG1 & 0xFFFF), 1, &(t->register_cache.tmst_config1_reg));

    t->need_mclk_cnt = 0;

    return status;
}

xmedia_s32 inv_imu_set_drvconfig3(xmedia_s32 dev, struct inv_imu_device *s, xmedia_u8 val)
{
    return inv_imu_write_reg(dev, s, DRIVE_CONFIG3, 1, &val);
}

xmedia_s32 inv_imu_set_timestamp_resolution(xmedia_s32 dev, struct inv_imu_device *s,
                                            const TMST_CONFIG1_RESOL_t timestamp_resol)
{
    xmedia_s32 status = 0;
    xmedia_u8 data;

    status |= inv_imu_read_reg(dev, s, TMST_CONFIG1_MREG1, 1, &data);
    data &= ~TMST_CONFIG1_TMST_RES_MASK;
    data |= timestamp_resol;
    status |= inv_imu_write_reg(dev, s, TMST_CONFIG1_MREG1, 1, &data);

    return status;
}

xmedia_s32 inv_imu_configure_wom(xmedia_s32 dev, struct inv_imu_device *s, const xmedia_u8 wom_x_th,
                                 const xmedia_u8 wom_y_th, const xmedia_u8 wom_z_th, WOM_CONFIG_WOM_INT_MODE_t wom_int,
                                 WOM_CONFIG_WOM_INT_DUR_t wom_dur)
{
    xmedia_s32 status = 0;
    xmedia_u8 data[3]; // 3 axes: (X Y Z)
    xmedia_u8 value;

    data[0] = wom_x_th; // Set X threshold: data[0]
    data[1] = wom_y_th; // Set Y threshold: data[1]
    data[2] = wom_z_th; // Set Z threshold: data[2]
    status |= inv_imu_write_reg(dev, s, ACCEL_WOM_X_THR_MREG1, sizeof(data), &data[0]);

    // Compare current sample with the previous sample and WOM from the 3 axis are ORed or ANDed to produce WOM signal.
    status |= inv_imu_read_reg(dev, s, WOM_CONFIG, 1, &value);
    value &= ~WOM_CONFIG_WOM_INT_MODE_MASK;
    value |= (xmedia_u8)WOM_CONFIG_WOM_MODE_CMP_PREV | (xmedia_u8)wom_int;

    // Configure the number of overthreshold event to wait before producing the WOM signal.
    value &= ~WOM_CONFIG_WOM_INT_DUR_MASK;
    value |= (xmedia_u8)wom_dur;
    status |= inv_imu_write_reg(dev, s, WOM_CONFIG, 1, &value);

    return status;
}

xmedia_s32 inv_imu_get_config_int1(xmedia_s32 dev, struct inv_imu_device *s, inv_imu_interrupt_parameter_t *it)
{
    xmedia_s32 status = 0;
    xmedia_u8 data;

    status |= inv_imu_read_reg(dev, s, INT_SOURCE0, 1, &data);
    it->INV_UI_FSYNC =
        (inv_imu_interrupt_value)((data & INT_SOURCE0_FSYNC_INT1_EN_MASK) >> INT_SOURCE0_FSYNC_INT1_EN_POS);
    it->INV_UI_DRDY = (inv_imu_interrupt_value)((data & INT_SOURCE0_DRDY_INT1_EN_MASK) >> INT_SOURCE0_DRDY_INT1_EN_POS);
    it->INV_FIFO_THS =
        (inv_imu_interrupt_value)((data & INT_SOURCE0_FIFO_THS_INT1_EN_MASK) >> INT_SOURCE0_FIFO_THS_INT1_EN_POS);
    it->INV_FIFO_FULL =
        (inv_imu_interrupt_value)((data & INT_SOURCE0_FIFO_FULL_INT1_EN_MASK) >> INT_SOURCE0_FIFO_FULL_INT1_EN_POS);

    status |= inv_imu_read_reg(dev, s, INT_SOURCE1, 1, &data);
    it->INV_SMD = (inv_imu_interrupt_value)((data & INT_SOURCE1_SMD_INT1_EN_MASK) >> INT_SOURCE1_SMD_INT1_EN_POS);
    it->INV_WOM_X = (inv_imu_interrupt_value)((data & INT_SOURCE1_WOM_X_INT1_EN_MASK) >> INT_SOURCE1_WOM_X_INT1_EN_POS);
    it->INV_WOM_Y = (inv_imu_interrupt_value)((data & INT_SOURCE1_WOM_Y_INT1_EN_MASK) >> INT_SOURCE1_WOM_Y_INT1_EN_POS);
    it->INV_WOM_Z = (inv_imu_interrupt_value)((data & INT_SOURCE1_WOM_Z_INT1_EN_MASK) >> INT_SOURCE1_WOM_Z_INT1_EN_POS);

    status |= inv_imu_read_reg(dev, s, INT_SOURCE6_MREG1, 1, &data);
    it->INV_FF = (inv_imu_interrupt_value)((data & INT_SOURCE6_FF_INT1_EN_MASK) >> INT_SOURCE6_FF_INT1_EN_POS);
    it->INV_LOWG = (inv_imu_interrupt_value)((data & INT_SOURCE6_LOWG_INT1_EN_MASK) >> INT_SOURCE6_LOWG_INT1_EN_POS);
    it->INV_STEP_DET =
        (inv_imu_interrupt_value)((data & INT_SOURCE6_STEP_DET_INT1_EN_MASK) >> INT_SOURCE6_STEP_DET_INT1_EN_POS);
    it->INV_STEP_CNT_OVFL = (inv_imu_interrupt_value)((data & INT_SOURCE6_STEP_CNT_OFL_INT1_EN_MASK) >>
                                                      INT_SOURCE6_STEP_CNT_OFL_INT1_EN_POS);
    it->INV_TILT_DET =
        (inv_imu_interrupt_value)((data & INT_SOURCE6_TILT_DET_INT1_EN_MASK) >> INT_SOURCE6_TILT_DET_INT1_EN_POS);

    return status;
}

xmedia_s32 inv_imu_set_config_int1(xmedia_s32 dev, struct inv_imu_device *s, inv_imu_interrupt_parameter_t *it)
{
    xmedia_s32 status = 0;
    xmedia_u8 data[2]; /* 2 byte buf */

    status |= inv_imu_read_reg(dev, s, INT_SOURCE0, 2, &data[0]); // len 2

    data[0] &= ~(INT_SOURCE0_FSYNC_INT1_EN_MASK | INT_SOURCE0_DRDY_INT1_EN_MASK | INT_SOURCE0_FIFO_THS_INT1_EN_MASK |
                 INT_SOURCE0_FIFO_FULL_INT1_EN_MASK);
    data[0] |= ((it->INV_UI_FSYNC != 0) << INT_SOURCE0_FSYNC_INT1_EN_POS);
    data[0] |= ((it->INV_UI_DRDY != 0) << INT_SOURCE0_DRDY_INT1_EN_POS);
    data[0] |= ((it->INV_FIFO_THS != 0) << INT_SOURCE0_FIFO_THS_INT1_EN_POS);
    data[0] |= ((it->INV_FIFO_FULL != 0) << INT_SOURCE0_FIFO_FULL_INT1_EN_POS);

    data[1] &= ~(INT_SOURCE1_SMD_INT1_EN_MASK | INT_SOURCE1_WOM_X_INT1_EN_MASK | INT_SOURCE1_WOM_Y_INT1_EN_MASK |
                 INT_SOURCE1_WOM_Z_INT1_EN_MASK);
    data[1] |= ((it->INV_SMD != 0) << INT_SOURCE1_SMD_INT1_EN_POS);
    data[1] |= ((it->INV_WOM_X != 0) << INT_SOURCE1_WOM_X_INT1_EN_POS);
    data[1] |= ((it->INV_WOM_Y != 0) << INT_SOURCE1_WOM_Y_INT1_EN_POS);
    data[1] |= ((it->INV_WOM_Z != 0) << INT_SOURCE1_WOM_Z_INT1_EN_POS);

    status |= inv_imu_write_reg(dev, s, INT_SOURCE0, 2, &data[0]); // len 2

    status |= inv_imu_read_reg(dev, s, INT_SOURCE6_MREG1, 1, &data[0]);

    data[0] &= ~(INT_SOURCE6_FF_INT1_EN_MASK | INT_SOURCE6_LOWG_INT1_EN_MASK | INT_SOURCE6_STEP_DET_INT1_EN_MASK |
                 INT_SOURCE6_STEP_CNT_OFL_INT1_EN_MASK | INT_SOURCE6_TILT_DET_INT1_EN_MASK);
    data[0] |= ((it->INV_FF != 0) << INT_SOURCE6_FF_INT1_EN_POS);
    data[0] |= ((it->INV_LOWG != 0) << INT_SOURCE6_LOWG_INT1_EN_POS);
    data[0] |= ((it->INV_STEP_DET != 0) << INT_SOURCE6_STEP_DET_INT1_EN_POS);
    data[0] |= ((it->INV_STEP_CNT_OVFL != 0) << INT_SOURCE6_STEP_CNT_OFL_INT1_EN_POS);
    data[0] |= ((it->INV_TILT_DET != 0) << INT_SOURCE6_TILT_DET_INT1_EN_POS);
    status |= inv_imu_write_reg(dev, s, INT_SOURCE6_MREG1, 1, &data[0]);

    return status;
}

xmedia_s32 inv_imu_configure_fifo_tdk(xmedia_s32 dev, struct inv_imu_device *s, INV_IMU_FIFO_CONFIG_t fifo_config)
{
    xmedia_s32 status = 0;
    xmedia_u8 data;
    inv_imu_interrupt_parameter_t config_int = { (inv_imu_interrupt_value)0 };

    s->fifo_is_used = fifo_config;

    inv_imu_switch_on_mclk(dev, s);

    switch (fifo_config) {
        case INV_IMU_FIFO_ENABLED:
            status |= inv_imu_read_reg(dev, s, INTF_CONFIG0, 1, &data);
            data &= ~(INTF_CONFIG0_FIFO_COUNT_FORMAT_MASK | INTF_CONFIG0_FIFO_COUNT_ENDIAN_MASK);
            data |= (xmedia_u8)INTF_CONFIG0_FIFO_COUNT_REC_RECORD | (xmedia_u8)INTF_CONFIG0_FIFO_COUNT_LITTLE_ENDIAN;
            status |= inv_imu_write_reg(dev, s, INTF_CONFIG0, 1, &data);
            status |= inv_imu_read_reg(dev, s, FIFO_CONFIG1, 1, &data);
            data &= ~(FIFO_CONFIG1_FIFO_MODE_MASK | FIFO_CONFIG1_FIFO_BYPASS_MASK);
            data |= (xmedia_u8)FIFO_CONFIG1_FIFO_MODE_STREAM | (xmedia_u8)FIFO_CONFIG1_FIFO_BYPASS_OFF;
            status |= inv_imu_write_reg(dev, s, FIFO_CONFIG1, 1, &data);
            status |= inv_imu_read_reg(dev, s, TMST_CONFIG1_MREG1, 1, &data);
            data &= ~TMST_CONFIG1_TMST_EN_MASK;
            // data |= TMST_CONFIG1_TMST_EN;
            status |= inv_imu_write_reg(dev, s, TMST_CONFIG1_MREG1, 1, &data);

            /* restart and reset FIFO configuration */
            status |= inv_imu_read_reg(dev, s, FIFO_CONFIG5_MREG1, 1, &data);
            data &=
                ~(FIFO_CONFIG5_FIFO_GYRO_EN_MASK | FIFO_CONFIG5_FIFO_ACCEL_EN_MASK | FIFO_CONFIG5_FIFO_WM_GT_TH_MASK |
                  FIFO_CONFIG5_FIFO_TMST_FSYNC_EN_MASK | FIFO_CONFIG5_FIFO_HIRES_EN_MASK);
            data |= ((xmedia_u8)FIFO_CONFIG5_GYRO_EN | (xmedia_u8)FIFO_CONFIG5_ACCEL_EN |
                     (xmedia_u8)FIFO_CONFIG5_WM_GT_TH_EN);
            status |= inv_imu_write_reg(dev, s, FIFO_CONFIG5_MREG1, 1, &data);

            // Configure FIFO WM so that INT is triggered for each packet
            data = 0x5;
            status |= inv_imu_write_reg(dev, s, FIFO_CONFIG2, 1, &data);

            /* Disable Data Ready Interrupt */
            status |= inv_imu_get_config_int1(dev, s, &config_int);
            config_int.INV_UI_DRDY = INV_IMU_DISABLE;
            status |= inv_imu_set_config_int1(dev, s, &config_int);
            break;

        case INV_IMU_FIFO_DISABLED:
            status |= inv_imu_read_reg(dev, s, FIFO_CONFIG1, 1, &data);
            data &= ~FIFO_CONFIG1_FIFO_BYPASS_MASK;
            data |= (xmedia_u8)FIFO_CONFIG1_FIFO_BYPASS_ON;
            status |= inv_imu_write_reg(dev, s, FIFO_CONFIG1, 1, &data);
            status |= inv_imu_read_reg(dev, s, FIFO_CONFIG5_MREG1, 1, &data);
            data &= ~(FIFO_CONFIG5_FIFO_GYRO_EN_MASK | FIFO_CONFIG5_FIFO_ACCEL_EN_MASK |
                      FIFO_CONFIG5_FIFO_TMST_FSYNC_EN_MASK);
            data |= ((xmedia_u8)FIFO_CONFIG5_GYRO_DIS | (xmedia_u8)FIFO_CONFIG5_ACCEL_DIS |
                     (xmedia_u8)FIFO_CONFIG5_TMST_FSYNC_EN);
            status |= inv_imu_write_reg(dev, s, FIFO_CONFIG5_MREG1, 1, &data);

            /* Enable Data Ready Interrupt */
            status |= inv_imu_get_config_int1(dev, s, &config_int);
            config_int.INV_UI_DRDY = INV_IMU_ENABLE;
            status |= inv_imu_set_config_int1(dev, s, &config_int);
            break;

        default:
            status = -1;
    }

    status |= inv_imu_switch_off_mclk(dev, s);

    return status;
}

xmedia_s32 inv_imu_configure_fifo(xmedia_s32 dev, struct inv_imu_device *s, INV_IMU_FIFO_CONFIG_t fifo_config)
{
    xmedia_s32 status = 0;
    xmedia_u8 data;
    inv_imu_interrupt_parameter_t config_int = { (inv_imu_interrupt_value)0 };

    s->fifo_is_used = fifo_config;

    inv_imu_switch_on_mclk(dev, s);

    switch (fifo_config) {
        case INV_IMU_FIFO_ENABLED:
            /* Configure:
              - FIFO record mode i.e FIFO count unit is packet
              - FIFO snapshot mode i.e drop the data when the FIFO overflows
              - Timestamp is logged in FIFO
              - Little Endian fifo_count
            */

            status |= inv_imu_read_reg(dev, s, INTF_CONFIG0, 1, &data);
            data &= ~(INTF_CONFIG0_FIFO_COUNT_FORMAT_MASK | INTF_CONFIG0_FIFO_COUNT_ENDIAN_MASK);
            data |= ((xmedia_u8)INTF_CONFIG0_FIFO_COUNT_REC_RECORD | (xmedia_u8)INTF_CONFIG0_FIFO_COUNT_LITTLE_ENDIAN);
            status |= inv_imu_write_reg(dev, s, INTF_CONFIG0, 1, &data);
            status |= inv_imu_read_reg(dev, s, FIFO_CONFIG1, 1, &data);
            data &= ~(FIFO_CONFIG1_FIFO_MODE_MASK | FIFO_CONFIG1_FIFO_BYPASS_MASK);
            // Stream-to-FIFO Mode   1: STOP-on-FULL Mode
            data |= ((xmedia_u8)FIFO_CONFIG1_FIFO_MODE_STREAM | (xmedia_u8)FIFO_CONFIG1_FIFO_BYPASS_OFF);
            status |= inv_imu_write_reg(dev, s, FIFO_CONFIG1, 1, &data);

            status |= inv_imu_read_reg(dev, s, TMST_CONFIG1_MREG1, 1, &data);
            data &= ~TMST_CONFIG1_TMST_EN_MASK;
            // timestamp enable  ?
            // data |= TMST_CONFIG1_TMST_EN;
            status |= inv_imu_write_reg(dev, s, TMST_CONFIG1_MREG1, 1, &data);

            /* restart and reset FIFO configuration */
            status |= inv_imu_read_reg(dev, s, FIFO_CONFIG5_MREG1, 1, &data);
            data &= ~(FIFO_CONFIG5_FIFO_GYRO_EN_MASK | FIFO_CONFIG5_FIFO_ACCEL_EN_MASK |
                      FIFO_CONFIG5_FIFO_WM_GT_TH_MASK | FIFO_CONFIG5_FIFO_TMST_FSYNC_EN_MASK |
                      FIFO_CONFIG5_FIFO_HIRES_EN_MASK | FIFO_CONFIG5_FIFO_RESUME_PARTIAL_RD_MASK);
            // TMST in the FIFO cannot be replaced by the FSYNC timestamp
            // trigger fifo watermask interrupt when FIFO_COUNT = FIFO_WM
            // FIFO_COUNT: count indicates the number of records or bytes available in FIFO
            // (1 FIFO_COUNT = = 1 record = 16 bytes)
            data |= ((xmedia_u8)FIFO_CONFIG5_GYRO_EN | (xmedia_u8)FIFO_CONFIG5_ACCEL_EN |
                     (xmedia_u8)FIFO_CONFIG5_WM_GT_TH_EN | (xmedia_u8)(0x1 << FIFO_CONFIG5_FIFO_RESUME_PARTIAL_RD_POS));
            status |= inv_imu_write_reg(dev, s, FIFO_CONFIG5_MREG1, 1, &data);

            // Configure FIFO WM so that INT is triggered for each packet
            data = 0x1;
            status |= inv_imu_write_reg(dev, s, FIFO_CONFIG2, 1, &data);

            /* Disable Data Ready Interrupt */
            status |= inv_imu_get_config_int1(dev, s, &config_int);
            config_int.INV_UI_DRDY = INV_IMU_DISABLE;
            status |= inv_imu_set_config_int1(dev, s, &config_int);
            break;

        case INV_IMU_FIFO_DISABLED:
            /* make sure FIFO is disabled */
            status |= inv_imu_read_reg(dev, s, FIFO_CONFIG1, 1, &data);
            data &= ~FIFO_CONFIG1_FIFO_BYPASS_MASK;
            data |= (xmedia_u8)FIFO_CONFIG1_FIFO_BYPASS_ON;
            status |= inv_imu_write_reg(dev, s, FIFO_CONFIG1, 1, &data);

            /* restart and reset FIFO configuration */
            status |= inv_imu_read_reg(dev, s, FIFO_CONFIG5_MREG1, 1, &data);
            data &= ~(FIFO_CONFIG5_FIFO_GYRO_EN_MASK | FIFO_CONFIG5_FIFO_ACCEL_EN_MASK |
                      FIFO_CONFIG5_FIFO_TMST_FSYNC_EN_MASK);
            data |= ((xmedia_u8)FIFO_CONFIG5_GYRO_DIS | (xmedia_u8)FIFO_CONFIG5_ACCEL_DIS |
                     (xmedia_u8)FIFO_CONFIG5_TMST_FSYNC_EN);
            status |= inv_imu_write_reg(dev, s, FIFO_CONFIG5_MREG1, 1, &data);

            /* Enable Data Ready Interrupt */
            status |= inv_imu_get_config_int1(dev, s, &config_int);
            config_int.INV_UI_DRDY = INV_IMU_ENABLE;
            status |= inv_imu_set_config_int1(dev, s, &config_int);
            break;

        default:
            status = -1;
    }

    status |= inv_imu_switch_off_mclk(dev, s);

    return status;
}

xmedia_s32 inv_imu_set_accel_frequency(xmedia_s32 dev, struct inv_imu_device *s, const ACCEL_CONFIG0_ODR_t frequency)
{
    xmedia_s32 status = 0;
    xmedia_u8 data;

    status |= inv_imu_read_reg(dev, s, ACCEL_CONFIG0, 1, &data);
    data &= ~ACCEL_CONFIG0_ACCEL_ODR_MASK;
    data |= frequency;
    status |= inv_imu_write_reg(dev, s, ACCEL_CONFIG0, 1, &data);

    return status;
}

xmedia_s32 inv_imu_set_gyro_frequency(xmedia_s32 dev, struct inv_imu_device *s, const GYRO_CONFIG0_ODR_t frequency)
{
    xmedia_s32 status = 0;
    xmedia_u8 data;

    status |= inv_imu_read_reg(dev, s, GYRO_CONFIG0, 1, &data);
    data &= ~GYRO_CONFIG0_GYRO_ODR_MASK;
    data |= frequency;
    status |= inv_imu_write_reg(dev, s, GYRO_CONFIG0, 1, &data);

    return status;
}

xmedia_s32 inv_imu_set_accel_fsr(xmedia_s32 dev, struct inv_imu_device *s, ACCEL_CONFIG0_FS_SEL_t accel_fsr_g)
{
    xmedia_s32 status = 0;
    xmedia_u8 data;

    status |= inv_imu_read_reg(dev, s, ACCEL_CONFIG0, 1, &data);
    data &= ~ACCEL_CONFIG0_ACCEL_UI_FS_SEL_MASK;
    data |= accel_fsr_g;
    status |= inv_imu_write_reg(dev, s, ACCEL_CONFIG0, 1, &data);

    return status;
}

xmedia_s32 inv_imu_set_gyro_fsr(xmedia_s32 dev, struct inv_imu_device *s, GYRO_CONFIG0_FS_SEL_t gyro_fsr_dps)
{
    xmedia_s32 status = 0;
    xmedia_u8 data;

    status |= inv_imu_read_reg(dev, s, GYRO_CONFIG0, 1, &data);
    data &= ~GYRO_CONFIG0_GYRO_UI_FS_SEL_MASK;
    data |= gyro_fsr_dps;
    status |= inv_imu_write_reg(dev, s, GYRO_CONFIG0, 1, &data);

    return status;
}

xmedia_s32 inv_imu_get_accel_fsr(xmedia_s32 dev, struct inv_imu_device *s, ACCEL_CONFIG0_FS_SEL_t *accel_fsr_g)
{
    xmedia_s32 status = 0;
    xmedia_u8 accel_cfg_0_reg;

    if ((s->fifo_highres_enabled) && (s->fifo_is_used == INV_IMU_FIFO_ENABLED)) {
        *accel_fsr_g = ACCEL_CONFIG0_FS_SEL_MAX;
    } else {
        status |= inv_imu_read_reg(dev, s, ACCEL_CONFIG0, 1, &accel_cfg_0_reg);
        *accel_fsr_g = (ACCEL_CONFIG0_FS_SEL_t)(accel_cfg_0_reg & ACCEL_CONFIG0_ACCEL_UI_FS_SEL_MASK);
    }

    return status;
}

xmedia_s32 inv_imu_get_gyro_fsr(xmedia_s32 dev, struct inv_imu_device *s, GYRO_CONFIG0_FS_SEL_t *gyro_fsr_dps)
{
    xmedia_s32 status = 0;
    xmedia_u8 gyro_cfg_0_reg;

    if ((s->fifo_highres_enabled) && (s->fifo_is_used == INV_IMU_FIFO_ENABLED))
        *gyro_fsr_dps = GYRO_CONFIG0_FS_SEL_MAX;
    else {
        status |= inv_imu_read_reg(dev, s, GYRO_CONFIG0, 1, &gyro_cfg_0_reg);
        *gyro_fsr_dps = (GYRO_CONFIG0_FS_SEL_t)(gyro_cfg_0_reg & GYRO_CONFIG0_GYRO_UI_FS_SEL_MASK);
    }

    return status;
}

xmedia_s32 inv_imu_set_accel_lp_avg(xmedia_s32 dev, struct inv_imu_device *s, ACCEL_CONFIG1_ACCEL_FILT_AVG_t acc_avg)
{
    xmedia_u8 value;
    xmedia_s32 status = 0;

    status |= inv_imu_read_reg(dev, s, ACCEL_CONFIG1, 1, &value);
    if (status)
        return status;

    value &= ~ACCEL_CONFIG1_ACCEL_UI_AVG_MASK;
    value |= acc_avg;

    status |= inv_imu_write_reg(dev, s, ACCEL_CONFIG1, 1, &value);

    return status;
}

xmedia_s32 inv_imu_set_accel_ln_bw(xmedia_s32 dev, struct inv_imu_device *s, ACCEL_CONFIG1_ACCEL_FILT_BW_t acc_bw)
{
    xmedia_u8 value;
    xmedia_s32 status = 0;

    status |= inv_imu_read_reg(dev, s, ACCEL_CONFIG1, 1, &value);
    if (status)
        return status;

    value &= ~ACCEL_CONFIG1_ACCEL_UI_FILT_BW_MASK;
    value |= acc_bw;

    status |= inv_imu_write_reg(dev, s, ACCEL_CONFIG1, 1, &value);

    return status;
}

xmedia_s32 inv_imu_set_gyro_ln_bw(xmedia_s32 dev, struct inv_imu_device *s, GYRO_CONFIG1_GYRO_FILT_BW_t gyr_bw)
{
    xmedia_u8 value;
    xmedia_s32 status = 0;

    status |= inv_imu_read_reg(dev, s, GYRO_CONFIG1, 1, &value);
    if (status)
        return status;

    value &= ~GYRO_CONFIG1_GYRO_UI_FILT_BW_MASK;
    value |= gyr_bw;

    status |= inv_imu_write_reg(dev, s, GYRO_CONFIG1, 1, &value);

    return status;
}

xmedia_s32 inv_imu_reset_fifo(xmedia_s32 dev, struct inv_imu_device *s)
{
    xmedia_s32 status = 0;
    xmedia_u8 fifo_flush_status = (xmedia_u8)SIGNAL_PATH_RESET_FIFO_FLUSH_EN;

    status |= inv_imu_switch_on_mclk(dev, s);

    status |= inv_imu_write_reg(dev, s, SIGNAL_PATH_RESET, 1, &fifo_flush_status);
    osal_udelay(10); // wait 10us

    /* Wait for FIFO flush (idle bit will go high at appropriate time and unlock flush) */
    while ((status == 0) &&
           ((fifo_flush_status & SIGNAL_PATH_RESET_FIFO_FLUSH_MASK) == (xmedia_u8)SIGNAL_PATH_RESET_FIFO_FLUSH_EN)) {
        status |= inv_imu_read_reg(dev, s, SIGNAL_PATH_RESET, 1, &fifo_flush_status);
    }

    status |= inv_imu_switch_off_mclk(dev, s);

    return status;
}

static xmedia_s32 select_rcosc(xmedia_s32 dev, struct inv_imu_device *s)
{
    xmedia_s32 status = 0;
    xmedia_u8 data;

    status |= inv_imu_read_reg(dev, s, PWR_MGMT0, 1, &data);
    data &= ~PWR_MGMT0_ACCEL_LP_CLK_SEL_MASK;
    data |= PWR_MGMT0_ACCEL_LP_CLK_RCOSC;
    status |= inv_imu_write_reg(dev, s, PWR_MGMT0, 1, &data);

    return status;
}

xmedia_u64 inv_imu_get_time_us(void)
{
#ifdef __KERNEL__
    osal_timeval_t time;
    osal_gettimeofday(&time);
#else
    struct timeval time;
    gettimeofday(&time, NULL);
#endif
    return (xmedia_u64)((time.tv_sec * 1000000LLU) + time.tv_usec);
}

xmedia_s32 inv_imu_enable_accel_low_noise_mode(xmedia_s32 dev, struct inv_imu_device *s)
{
    xmedia_s32 status = 0;
    PWR_MGMT0_ACCEL_MODE_t accel_mode;
    PWR_MGMT0_GYRO_MODE_t gyro_mode;
    ACCEL_CONFIG0_ODR_t acc_odr_bitfield;
    xmedia_u32 accel_odr_us;
    xmedia_u8 pwr_mgmt0_reg;
    xmedia_u8 accel_cfg_0_reg;

    status |= inv_imu_read_reg(dev, s, PWR_MGMT0, 1, &pwr_mgmt0_reg);
    accel_mode = (PWR_MGMT0_ACCEL_MODE_t)(pwr_mgmt0_reg & PWR_MGMT0_ACCEL_MODE_MASK);
    gyro_mode = (PWR_MGMT0_GYRO_MODE_t)(pwr_mgmt0_reg & PWR_MGMT0_GYRO_MODE_MASK);
    /* Check if the accelerometer is the only one enabled */
    if ((accel_mode == PWR_MGMT0_ACCEL_MODE_LP) &&
        ((gyro_mode == PWR_MGMT0_GYRO_MODE_OFF) || (gyro_mode == PWR_MGMT0_GYRO_MODE_STANDBY))) {
        /* Get accelerometer's ODR for next required wait */
        status |= inv_imu_read_reg(dev, s, ACCEL_CONFIG0, 1, &accel_cfg_0_reg);
        acc_odr_bitfield = (ACCEL_CONFIG0_ODR_t)(accel_cfg_0_reg & ACCEL_CONFIG0_ACCEL_ODR_MASK);
        accel_odr_us = inv_imu_convert_odr_bitfield_to_us(dev, acc_odr_bitfield);
        /* Select the RC OSC as clock source for the accelerometer */
        status |= select_rcosc(dev, s);
        /* Wait one accel ODR before switching to low noise mode */
        osal_udelay(accel_odr_us);
    }

    /* Enable/Switch the accelerometer in/to low noise mode */
    status |=
        inv_imu_read_reg(dev, s, PWR_MGMT0, 1, &pwr_mgmt0_reg); /* Read a new time because select_rcosc() modified it */
    pwr_mgmt0_reg &= ~PWR_MGMT0_ACCEL_MODE_MASK;
    pwr_mgmt0_reg |= PWR_MGMT0_ACCEL_MODE_LN;
    status |= inv_imu_write_reg(dev, s, PWR_MGMT0, 1, &pwr_mgmt0_reg);
    osal_udelay(200); // wait 200us

    if (accel_mode == PWR_MGMT0_ACCEL_MODE_OFF) {
        /* First data are wrong after accel enable using IIR filter
         There is no signal that says accel start-up has completed and data are stable using FIR filter
         So keep track of the time at start-up to discard the invalid data, about 20ms after enable
        */
        if (s->fifo_is_used)
            s->accel_start_time_us = inv_imu_get_time_us();
    }

    return status;
}

xmedia_u32 inv_imu_convert_odr_bitfield_to_us(xmedia_s32 dev, xmedia_u32 odr_bitfield)
{
    /*
odr bitfield - frequency : odr ms
                    0 - N/A
                    1 - N/A
                    2 - N/A
                    3 - N/A
                    4 - N/A
                    5 - 1.6k      : 0.625ms
          (default) 6 - 800       : 1.25ms
                    7 - 400       : 2.5 ms
                    8 - 200       : 5 ms
                    9 - 100       : 10 ms
                    10 - 50       : 20 ms
                    11 - 25       : 40 ms
                    12 - 12.5     : 80 ms
                    13 - 6.25     : 160 ms
                    14 - 3.125    : 320 ms
                    15 - 1.5625   : 640 ms
            */
    xmedia_unused(dev);

    switch (odr_bitfield) {
        case ACCEL_CONFIG0_ODR_1600_HZ:
            return 625; // 625us
        case ACCEL_CONFIG0_ODR_800_HZ:
            return 1250; // 1250us
        case ACCEL_CONFIG0_ODR_400_HZ:
            return 2500; // 2500us
        case ACCEL_CONFIG0_ODR_200_HZ:
            return 5000; // 5000us
        case ACCEL_CONFIG0_ODR_100_HZ:
            return 10000; // 10000us
        case ACCEL_CONFIG0_ODR_50_HZ:
            return 20000; // 20000us
        case ACCEL_CONFIG0_ODR_25_HZ:
            return 40000; // 40000us
        case ACCEL_CONFIG0_ODR_12_5_HZ:
            return 80000; // 80000us
        case ACCEL_CONFIG0_ODR_6_25_HZ:
            return 160000; // 160000us
        case ACCEL_CONFIG0_ODR_3_125_HZ:
            return 320000; // 320000us
        case ACCEL_CONFIG0_ODR_1_5625_HZ:
        default:
            return 640000; // 640000us
    }
}

xmedia_s32 inv_imu_enable_gyro_low_noise_mode(xmedia_s32 dev, struct inv_imu_device *s)
{
    xmedia_s32 status = 0;
    PWR_MGMT0_ACCEL_MODE_t accel_mode;
    PWR_MGMT0_GYRO_MODE_t gyro_mode;
    ACCEL_CONFIG0_ODR_t acc_odr_bitfield;
    xmedia_u32 accel_odr_us;
    xmedia_u8 pwr_mgmt0_reg;
    xmedia_u8 accel_cfg_0_reg;

    status |= inv_imu_read_reg(dev, s, PWR_MGMT0, 1, &pwr_mgmt0_reg);
    accel_mode = (PWR_MGMT0_ACCEL_MODE_t)(pwr_mgmt0_reg & PWR_MGMT0_ACCEL_MODE_MASK);
    gyro_mode = (PWR_MGMT0_GYRO_MODE_t)(pwr_mgmt0_reg & PWR_MGMT0_GYRO_MODE_MASK);
    /* Check if the accelerometer is the only one enabled */
    if ((accel_mode == PWR_MGMT0_ACCEL_MODE_LP) &&
        ((gyro_mode == PWR_MGMT0_GYRO_MODE_OFF) || (gyro_mode == PWR_MGMT0_GYRO_MODE_STANDBY))) {
        /* Get accelerometer's ODR for next required wait */
        status |= inv_imu_read_reg(dev, s, ACCEL_CONFIG0, 1, &accel_cfg_0_reg);
        acc_odr_bitfield = (ACCEL_CONFIG0_ODR_t)(accel_cfg_0_reg & ACCEL_CONFIG0_ACCEL_ODR_MASK);
        accel_odr_us = inv_imu_convert_odr_bitfield_to_us(dev, acc_odr_bitfield);
        /* Select the RC OSC as clock source for the accelerometer */
        status |= select_rcosc(dev, s);
        /* Wait one accel ODR before enabling the gyroscope */
        osal_udelay(accel_odr_us);
    }

    /* Enable/Switch the gyroscope in/to low noise mode */
    status |=
        inv_imu_read_reg(dev, s, PWR_MGMT0, 1, &pwr_mgmt0_reg); /* Read a new time because select_rcosc() modified it */
    pwr_mgmt0_reg &= ~PWR_MGMT0_GYRO_MODE_MASK;
    pwr_mgmt0_reg |= (xmedia_u8)PWR_MGMT0_GYRO_MODE_LN;
    status |= inv_imu_write_reg(dev, s, PWR_MGMT0, 1, &pwr_mgmt0_reg);
    osal_udelay(200); // wait 200us

    if (gyro_mode == PWR_MGMT0_GYRO_MODE_OFF) {
        /* First data are wrong after gyro enable using IIR filter
         There is no signal that says Gyro start-up has completed and data are stable using FIR filter
         and the Gyro max start-up time is 40ms
         So keep track of the time at start-up to discard the invalid data, about 60ms after enable
        */
        if (s->fifo_is_used)
            s->gyro_start_time_us = inv_imu_get_time_us();
    }

    return status;
}

xmedia_s32 inv_imu_apex_init_parameters_struct(struct inv_imu_device *s, inv_imu_apex_parameters_t *apex_inputs)
{
    xmedia_s32 status = 0;
    (void)s;

    /* Default parameters at POR */
    apex_inputs->pedo_amp_th = APEX_CONFIG3_PEDO_AMP_TH_62_MG;
    apex_inputs->pedo_step_cnt_th = 0x5;
    apex_inputs->pedo_step_det_th = 0x2;
    apex_inputs->pedo_sb_timer_th = APEX_CONFIG4_PEDO_SB_TIMER_TH_150_SAMPLES;
    apex_inputs->pedo_hi_enrgy_th = APEX_CONFIG4_PEDO_HI_ENRGY_TH_104_MG;
    apex_inputs->tilt_wait_time = APEX_CONFIG5_TILT_WAIT_TIME_4_S;
    apex_inputs->power_save_time = APEX_CONFIG2_DMP_POWER_SAVE_TIME_SEL_8_S;
    apex_inputs->power_save = APEX_CONFIG0_DMP_POWER_SAVE_EN;
    apex_inputs->sensitivity_mode = APEX_CONFIG9_SENSITIVITY_MODE_NORMAL;
    apex_inputs->low_energy_amp_th = APEX_CONFIG2_LOW_ENERGY_AMP_TH_SEL_80_MG;
    apex_inputs->smd_sensitivity = APEX_CONFIG9_SMD_SENSITIVITY_0;
    apex_inputs->ff_debounce_duration = APEX_CONFIG9_FF_DEBOUNCE_DURATION_2000_MS;
    apex_inputs->ff_max_duration_cm = APEX_CONFIG12_FF_MAX_DURATION_204_CM;
    apex_inputs->ff_min_duration_cm = APEX_CONFIG12_FF_MIN_DURATION_10_CM;
    apex_inputs->lowg_peak_th = APEX_CONFIG10_LOWG_PEAK_TH_563_MG;
    apex_inputs->lowg_peak_hyst = APEX_CONFIG5_LOWG_PEAK_TH_HYST_156_MG;
    apex_inputs->lowg_samples_th = APEX_CONFIG10_LOWG_TIME_TH_1_SAMPLE;
    apex_inputs->highg_peak_th = APEX_CONFIG11_HIGHG_PEAK_TH_2500_MG;
    apex_inputs->highg_peak_hyst = APEX_CONFIG5_HIGHG_PEAK_TH_HYST_156_MG;
    apex_inputs->highg_samples_th = APEX_CONFIG11_HIGHG_TIME_TH_1_SAMPLE;

    return status;
}

xmedia_s32 inv_imu_enable_wom(xmedia_s32 dev, struct inv_imu_device *s)
{
    xmedia_s32 status = 0;
    xmedia_u8 value;
    inv_imu_interrupt_parameter_t config_int = { (inv_imu_interrupt_value)0 };

    /* Disable fifo threshold int1 */
    status |= inv_imu_get_config_int1(dev, s, &config_int);
    config_int.INV_FIFO_THS = INV_IMU_DISABLE;
    status |= inv_imu_set_config_int1(dev, s, &config_int);

    /* Enable WOM */
    status |= inv_imu_read_reg(dev, s, WOM_CONFIG, 1, &value);
    value &= ~WOM_CONFIG_WOM_EN_MASK;
    value |= (xmedia_u8)WOM_CONFIG_WOM_EN_ENABLE;
    status |= inv_imu_write_reg(dev, s, WOM_CONFIG, 1, &value);

    return status;
}

xmedia_s32 inv_imu_disable_wom(xmedia_s32 dev, struct inv_imu_device *s)
{
    xmedia_s32 status = 0;
    xmedia_u8 value;
    inv_imu_interrupt_parameter_t config_int = { (inv_imu_interrupt_value)0 };

    /* Disable WOM */
    status |= inv_imu_read_reg(dev, s, WOM_CONFIG, 1, &value);
    value &= ~WOM_CONFIG_WOM_EN_MASK;
    value |= WOM_CONFIG_WOM_EN_DISABLE;
    status |= inv_imu_write_reg(dev, s, WOM_CONFIG, 1, &value);

    /* Enable fifo threshold int1 */
    status |= inv_imu_get_config_int1(dev, s, &config_int);
    config_int.INV_FIFO_THS = INV_IMU_ENABLE;
    status |= inv_imu_set_config_int1(dev, s, &config_int);

    return status;
}

static xmedia_s32 icm42670_set_gyro_odr(xmedia_s32 dev, msensor_gyro_status gyro_status)
{
    xmedia_s32 rc = 0;
    osal_printk("dev %d set gyro odr is %d\n", dev, gyro_status.gyro_config.odr);

    if (gyro_status.gyro_config.odr == 50) { /* 50 odr */
        rc = inv_imu_set_gyro_frequency(dev, &icm_driver[dev], GYRO_CONFIG0_ODR_50_HZ);
    } else if (gyro_status.gyro_config.odr == 100) { /* 100 odr */
        rc = inv_imu_set_gyro_frequency(dev, &icm_driver[dev], GYRO_CONFIG0_ODR_100_HZ);
    } else if (gyro_status.gyro_config.odr == 200) { /* 200 odr */
        rc = inv_imu_set_gyro_frequency(dev, &icm_driver[dev], GYRO_CONFIG0_ODR_200_HZ);
    } else if (gyro_status.gyro_config.odr == 400) { /* 400 odr */
        rc = inv_imu_set_gyro_frequency(dev, &icm_driver[dev], GYRO_CONFIG0_ODR_400_HZ);
    } else if (gyro_status.gyro_config.odr == 800) { /* 800 odr */
        rc = inv_imu_set_gyro_frequency(dev, &icm_driver[dev], GYRO_CONFIG0_ODR_800_HZ);
    } else if (gyro_status.gyro_config.odr == 1600) { /* 1600 odr */
        rc = inv_imu_set_gyro_frequency(dev, &icm_driver[dev], GYRO_CONFIG0_ODR_1600_HZ);
    } else {
        MSENSOR_ERR_TRACE("dev %d odr:%u is out of range\n", dev, gyro_status.gyro_config.odr);
        return XMEDIA_FAILURE;
    }

    return rc;
}

static xmedia_s32 icm42670_set_gyro_fsr(xmedia_s32 dev, msensor_gyro_status gyro_status)
{
    xmedia_s32 rc = 0;
    osal_printk("dev %d set gyro fsr is %d\n", dev, gyro_status.gyro_config.fsr);

    switch (gyro_status.gyro_config.fsr) {
        case GYRO_FULL_SCALE_RANGE_250DPS:
            rc = inv_imu_set_gyro_fsr(dev, &icm_driver[dev], GYRO_CONFIG0_FS_SEL_250dps);
            break;
        case GYRO_FULL_SCALE_RANGE_500DPS:
            rc = inv_imu_set_gyro_fsr(dev, &icm_driver[dev], GYRO_CONFIG0_FS_SEL_500dps);
            break;
        case GYRO_FULL_SCALE_RANGE_1KDPS:
            rc = inv_imu_set_gyro_fsr(dev, &icm_driver[dev], GYRO_CONFIG0_FS_SEL_1000dps);
            break;
        case GYRO_FULL_SCALE_RANGE_2KDPS:
            rc = inv_imu_set_gyro_fsr(dev, &icm_driver[dev], GYRO_CONFIG0_FS_SEL_2000dps);
            break;
        default:
            MSENSOR_ERR_TRACE("dev %d icm42670 gyro attr is invalid\n", dev);
            return -EINVAL;
    }

    return rc;
}

static xmedia_s32 icm42670_set_accel_odr(xmedia_s32 dev, msensor_acc_status acc_status)
{
    xmedia_s32 rc = 0;

    /* set ACCEL FSR */
    osal_printk("dev %d set accel odr is %d\n", dev, acc_status.acc_config.odr);

    if (acc_status.acc_config.odr == 50) { /* 50 odr */
        rc = inv_imu_set_accel_frequency(dev, &icm_driver[dev], ACCEL_CONFIG0_ODR_50_HZ);
    } else if (acc_status.acc_config.odr == 100) { /* 100 odr */
        rc = inv_imu_set_accel_frequency(dev, &icm_driver[dev], ACCEL_CONFIG0_ODR_100_HZ);
    } else if (acc_status.acc_config.odr == 200) { /* 200 odr */
        rc = inv_imu_set_accel_frequency(dev, &icm_driver[dev], ACCEL_CONFIG0_ODR_200_HZ);
    } else if (acc_status.acc_config.odr == 400) { /* 400 odr */
        rc = inv_imu_set_accel_frequency(dev, &icm_driver[dev], ACCEL_CONFIG0_ODR_400_HZ);
    } else if (acc_status.acc_config.odr == 800) { /* 800 odr */
        rc = inv_imu_set_accel_frequency(dev, &icm_driver[dev], ACCEL_CONFIG0_ODR_800_HZ);
    } else if (acc_status.acc_config.odr == 1600) { /* 1600 odr */
        rc = inv_imu_set_accel_frequency(dev, &icm_driver[dev], ACCEL_CONFIG0_ODR_1600_HZ);
    } else {
        MSENSOR_ERR_TRACE("dev %d odr:%u is out of range\n", dev, acc_status.acc_config.odr);
        return XMEDIA_FAILURE;
    }

    return rc;
}

static xmedia_s32 icm42670_set_accel_fsr(xmedia_s32 dev, msensor_acc_status acc_status)
{
    xmedia_s32 rc = 0;
    /* set ACCEL FSR */
    osal_printk("dev %d set accel fsr is %d\n", dev, acc_status.acc_config.fsr);

    switch (acc_status.acc_config.fsr) {
        case ACCEL_UI_FULL_SCALE_SET_2G:
            rc = inv_imu_set_accel_fsr(dev, &icm_driver[dev], ACCEL_CONFIG0_FS_SEL_2g);
            break;
        case ACCEL_UI_FULL_SCALE_SET_4G:
            rc = inv_imu_set_accel_fsr(dev, &icm_driver[dev], ACCEL_CONFIG0_FS_SEL_4g);
            break;
        case ACCEL_UI_FULL_SCALE_SET_8G:
            rc = inv_imu_set_accel_fsr(dev, &icm_driver[dev], ACCEL_CONFIG0_FS_SEL_8g);
            break;
        case ACCEL_UI_FULL_SCALE_SET_16G:
            rc = inv_imu_set_accel_fsr(dev, &icm_driver[dev], ACCEL_CONFIG0_FS_SEL_16g);
            break;
        default:
            MSENSOR_ERR_TRACE("dev %d icm42670 acc attr is invalid\n", dev);
            return -EINVAL;
    }

    return rc;
}

static xmedia_s32 icm42670_axis_set_attr(xmedia_s32 dev, xmedia_u32 dev_mode)
{
    xmedia_s32 ret;

    if (XMEDIA_MSENSOR_DEVICE_GYRO & dev_mode) {
        /* 5. 250dps, 8K ODR, 250_hz BW for gyro */
        ret = icm42670_set_gyro_odr(dev, g_icm42670_dev[dev]->gyro_status);
        if (ret != XMEDIA_SUCCESS) {
            MSENSOR_ERR_TRACE("dev %d icm42670 set gyro odr failed\n", dev);
            return XMEDIA_FAILURE;
        }

        ret = icm42670_set_gyro_fsr(dev, g_icm42670_dev[dev]->gyro_status);
        if (ret != XMEDIA_SUCCESS) {
            MSENSOR_ERR_TRACE("dev %d icm42670 set gyro fsr failed\n", dev);
            return XMEDIA_FAILURE;
        }
    }

    if (XMEDIA_MSENSOR_DEVICE_ACC & dev_mode) {
        /* 6. accel FSR setting: UI accel to 4G, OIS accel to 2G */
        ret = icm42670_set_accel_odr(dev, g_icm42670_dev[dev]->acc_status);
        if (ret != XMEDIA_SUCCESS) {
            MSENSOR_ERR_TRACE("dev %d icm42670 set accel odr failed\n", dev);
            return XMEDIA_FAILURE;
        }

        ret = icm42670_set_accel_fsr(dev, g_icm42670_dev[dev]->acc_status);
        if (ret != XMEDIA_SUCCESS) {
            MSENSOR_ERR_TRACE("dev %d icm42670 set accel fsr failed\n", dev);
            return XMEDIA_FAILURE;
        }
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 configure_imu_device(xmedia_s32 dev, xmedia_u32 dev_mode)
{
    xmedia_s32 rc = 0;

    if (!USE_FIFO)
        rc |= inv_imu_configure_fifo(dev, &icm_driver[dev], INV_IMU_FIFO_DISABLED);

    if (USE_HIGH_RES_MODE) {
        rc |= inv_imu_enable_high_resolution_fifo(dev, &icm_driver[dev]);
    } else {
        rc |= icm42670_axis_set_attr(dev, dev_mode);
    }

    if (USE_LOW_NOISE_MODE) {
        rc |= inv_imu_enable_accel_low_noise_mode(dev, &icm_driver[dev]);
    } else {
        rc |= inv_imu_enable_accel_low_power_mode(dev, &icm_driver[dev]);
    }

    rc |= inv_imu_enable_gyro_low_noise_mode(dev, &icm_driver[dev]);

    if (!USE_FIFO)
        osal_udelay(GYR_STARTUP_TIME_US);

    return rc;
}

static xmedia_s32 resume_dmp(xmedia_s32 dev, struct inv_imu_device *s)
{
    xmedia_s32 status = 0;
    xmedia_u8 value;
    xmedia_u64 start;

    status |= inv_imu_read_reg(dev, s, APEX_CONFIG0, 1, &value);
    value &= ~APEX_CONFIG0_DMP_INIT_EN_MASK;
    value |= (xmedia_u8)APEX_CONFIG0_DMP_INIT_EN;
    status |= inv_imu_write_reg(dev, s, APEX_CONFIG0, 1, &value);

    /* wait to make sure dmp_init_en = 0 */
    start = inv_imu_get_time_us();
    do {
        inv_imu_read_reg(dev, s, APEX_CONFIG0, 1, &value);
        osal_udelay(100); // wait 100us

        if ((value & APEX_CONFIG0_DMP_INIT_EN_MASK) == 0)
            break;
    } while (inv_imu_get_time_us() - start < 50000); // max time interval: 50000us

    return status;
}

xmedia_s32 inv_imu_reset_dmp(xmedia_s32 dev, struct inv_imu_device *s, const APEX_CONFIG0_DMP_MEM_RESET_t sram_reset)
{
    const xmedia_s32 ref_timeout = 5000; /* 50 ms */
    xmedia_s32 status = 0;
    xmedia_s32 timeout = ref_timeout;
    xmedia_u8 data_dmp_reset;
    xmedia_u8 value = 0;

    status |= inv_imu_switch_on_mclk(dev, s);

    // Reset DMP internal memories
    status |= inv_imu_read_reg(dev, s, APEX_CONFIG0, 1, &value);
    value &= ~APEX_CONFIG0_DMP_MEM_RESET_EN_MASK;
    value |= (sram_reset & APEX_CONFIG0_DMP_MEM_RESET_EN_MASK);
    status |= inv_imu_write_reg(dev, s, APEX_CONFIG0, 1, &value);

    osal_udelay(1000); // wait 1000us

    // Make sure reset procedure has finished by reading back mem_reset_en bit
    do {
        osal_udelay(10); // wait 10us
        status |= inv_imu_read_reg(dev, s, APEX_CONFIG0, 1, &data_dmp_reset);
    } while (((data_dmp_reset & APEX_CONFIG0_DMP_MEM_RESET_EN_MASK) != APEX_CONFIG0_DMP_MEM_RESET_DIS) && timeout-- &&
             !status);

    status |= inv_imu_switch_off_mclk(dev, s);

    if (timeout <= 0)
        return -4; // timeout error: -4

    return status;
}

xmedia_s32 inv_imu_start_dmp(xmedia_s32 dev, struct inv_imu_device *s)
{
    xmedia_s32 status = 0;

    // On first enabling of DMP, reset internal state
    if (!s->dmp_is_on) {
        // Reset SRAM to 0's
        status |= inv_imu_reset_dmp(dev, s, APEX_CONFIG0_DMP_MEM_RESET_APEX_ST_EN);
        if (status)
            return status;
        s->dmp_is_on = 1;
    }

    // Initialize DMP
    status |= resume_dmp(dev, s);

    return status;
}

xmedia_s32 inv_io_hal_read_reg(xmedia_s32 dev, struct inv_imu_serif *serif, xmedia_u8 reg, xmedia_u8 *rbuffer,
                               xmedia_u32 rlen)
{
    return icm42670_transfer_read(dev, reg, rbuffer, rlen, XMEDIA_FALSE);
}

xmedia_s32 inv_io_hal_write_reg(xmedia_s32 dev, struct inv_imu_serif *serif, xmedia_u8 reg, const xmedia_u8 *wbuffer,
                                xmedia_u32 wlen)
{
    return icm42670_transfer_write(dev, reg, wbuffer, wlen);
}

static void setup_mcu(struct inv_imu_serif *icm_serif)
{
    /* Initialize serial interface between MCU and IMU */
    icm_serif->context = 0; /* no need */
    icm_serif->read_reg = inv_io_hal_read_reg;
    icm_serif->write_reg = inv_io_hal_write_reg;
    icm_serif->max_read = 1024 * 32; /* maximum number of bytes allowed per serial read: 32kb = 32 * 1024bit */
    icm_serif->max_write = 1024 * 32; /* maximum number of bytes allowed per serial write: 32kb = 32 * 1024bit */
    icm_serif->serif_type = UI_SPI4;
}

xmedia_s32 inv_imu_get_endianness(xmedia_s32 dev, struct inv_imu_device *s)
{
    xmedia_s32 status = 0;
    xmedia_u8 value;

    status |= inv_imu_read_reg(dev, s, INTF_CONFIG0, 1, &value);
    if (!status)
        s->endianness_data = value & INTF_CONFIG0_SENSOR_DATA_ENDIAN_MASK;

    return status;
}

/*
 * Static functions definition
 */
static xmedia_s32 configure_serial_interface(xmedia_s32 dev, struct inv_imu_device *s)
{
    xmedia_u8 value;
    xmedia_s32 status = 0;

    /* Ensure BLK_SEL_R and BLK_SEL_W are set to 0 */
    value = 0;
    status |= inv_imu_write_reg(dev, s, BLK_SEL_R, 1, &value);
    status |= inv_imu_write_reg(dev, s, BLK_SEL_W, 1, &value);

    if (s->transport.serif.serif_type == UI_I2C) {
        /* Enable I2C 50ns spike filtering */
        status |= inv_imu_read_reg(dev, s, INTF_CONFIG1, 1, &value);
        value &= ~(INTF_CONFIG1_I3C_SDR_EN_MASK | INTF_CONFIG1_I3C_DDR_EN_MASK);
        status |= inv_imu_write_reg(dev, s, INTF_CONFIG1, 1, &value);
    } else {
        /* Configure SPI */
        if (s->transport.serif.serif_type == UI_SPI4)
            value = (xmedia_u8)DEVICE_CONFIG_SPI_4WIRE | (xmedia_u8)DEVICE_CONFIG_SPI_MODE_0_3;
        else if (s->transport.serif.serif_type == UI_SPI3)
            value = (xmedia_u8)DEVICE_CONFIG_SPI_3WIRE | (xmedia_u8)DEVICE_CONFIG_SPI_MODE_0_3;
        status |= inv_imu_write_reg(dev, s, DEVICE_CONFIG, 1, &value);
    }

    return status;
}

xmedia_s32 inv_imu_disable_fsync(xmedia_s32 dev, struct inv_imu_device *s)
{
    xmedia_s32 status = 0;
    xmedia_u8 value;

    status |= inv_imu_switch_on_mclk(dev, s);

    // Disable Fsync
    status |= inv_imu_read_reg(dev, s, FSYNC_CONFIG_MREG1, 1, &value);
    value &= ~FSYNC_CONFIG_FSYNC_UI_SEL_MASK;
    value |= (xmedia_u8)FSYNC_CONFIG_UI_SEL_NO;
    status |= inv_imu_write_reg(dev, s, FSYNC_CONFIG_MREG1, 1, &value);

    status |= inv_imu_read_reg(dev, s, TMST_CONFIG1_MREG1, 1, &value);
    value &= ~TMST_CONFIG1_TMST_FSYNC_EN_MASK;
    value |= TMST_CONFIG1_TMST_FSYNC_DIS;
    status |= inv_imu_write_reg(dev, s, TMST_CONFIG1_MREG1, 1, &value);

    status |= inv_imu_switch_off_mclk(dev, s);

    return status;
}

static xmedia_s32 init_hardware_from_ui(xmedia_s32 dev, struct inv_imu_device *s)
{
    xmedia_s32 status = 0;
    xmedia_u8 value;
    inv_imu_interrupt_parameter_t config_int = { (inv_imu_interrupt_value)0 };

    /* Deactivate FSYNC by default */
    status |= inv_imu_disable_fsync(dev, s);

    /* Set default timestamp resolution 16us (Mobile use cases) */
    status |= inv_imu_set_timestamp_resolution(dev, s, TMST_CONFIG1_RESOL_16us);

    /* Enable push pull on INT1 to avoid moving in Test Mode after a soft reset */
    status |= inv_imu_read_reg(dev, s, INT_CONFIG, 1, &value);
    value &= ~INT_CONFIG_INT1_DRIVE_CIRCUIT_MASK;
    value |= (xmedia_u8)INT_CONFIG_INT1_DRIVE_CIRCUIT_PP;
    status |= inv_imu_write_reg(dev, s, INT_CONFIG, 1, &value);

    /* Configure the INT1 interrupt pulse as active high */
    status |= inv_imu_read_reg(dev, s, INT_CONFIG, 1, &value);
    value &= ~INT_CONFIG_INT1_POLARITY_MASK;
    value |= (xmedia_u8)INT_CONFIG_INT1_POLARITY_HIGH;
    status |= inv_imu_write_reg(dev, s, INT_CONFIG, 1, &value);

    /* Set interrupt config */
    config_int.INV_UI_FSYNC = INV_IMU_DISABLE;
    config_int.INV_UI_DRDY = INV_IMU_DISABLE;
    config_int.INV_FIFO_THS = INV_IMU_ENABLE;
    config_int.INV_FIFO_FULL = INV_IMU_DISABLE;
    config_int.INV_SMD = INV_IMU_ENABLE;
    config_int.INV_WOM_X = INV_IMU_ENABLE;
    config_int.INV_WOM_Y = INV_IMU_ENABLE;
    config_int.INV_WOM_Z = INV_IMU_ENABLE;
    config_int.INV_FF = INV_IMU_ENABLE;
    config_int.INV_LOWG = INV_IMU_ENABLE;
    config_int.INV_STEP_DET = INV_IMU_ENABLE;
    config_int.INV_STEP_CNT_OVFL = INV_IMU_ENABLE;
    config_int.INV_TILT_DET = INV_IMU_ENABLE;
    status |= inv_imu_set_config_int1(dev, s, &config_int);

    /* Enable FIFO: use 16-bit format by default (i.e. high res is disabled) */
    status |= inv_imu_configure_fifo(dev, s, INV_IMU_FIFO_ENABLED);

    /*
     * Disable the automatic RCOSC power on to avoid
     * extra power consumption in sleep mode (all sensors and clocks off)
     */
    status |= inv_imu_read_reg(dev, s, FIFO_CONFIG6_MREG1, 1, &value);
    value |= ((1 & FIFO_CONFIG6_RCOSC_REQ_ON_FIFO_THS_DIS_MASK) << FIFO_CONFIG6_RCOSC_REQ_ON_FIFO_THS_DIS_POS);
    status |= inv_imu_write_reg(dev, s, FIFO_CONFIG6_MREG1, 1, &value);

    return status;
}

xmedia_s32 inv_imu_device_reset(xmedia_s32 dev, struct inv_imu_device *s)
{
    xmedia_s32 status = INV_ERROR_SUCCESS;
    xmedia_u8 data;

    /* Ensure BLK_SEL_R and BLK_SEL_W are set to 0 */
    data = 0;
    status |= inv_imu_write_reg(dev, s, BLK_SEL_R, 1, &data);
    status |= inv_imu_write_reg(dev, s, BLK_SEL_W, 1, &data);

    /* Trigger soft reset */
    data = (xmedia_u8)SIGNAL_PATH_RESET_SOFT_RESET_DEVICE_CONFIG_EN;
    status |= inv_imu_write_reg(dev, s, SIGNAL_PATH_RESET, 1, &data);

    /* Wait 1ms for soft reset to be effective */
    osal_msleep(1);

    /* Re-configure serial interface since it was reset */
    status |= configure_serial_interface(dev, s);

    /* Clear the reset done interrupt */
    status |= inv_imu_read_reg(dev, s, INT_STATUS, 1, &data);
    if (data != INT_STATUS_RESET_DONE_INT_MASK) {
        status |= INV_ERROR_UNEXPECTED;
        osal_printk("dev %d inv_imu_read_reg INT_STATUS value: %d error, spi get data failed!", dev, data);
    }

    return status;
}

xmedia_s32 inv_imu_init(xmedia_s32 dev, struct inv_imu_device *s, struct inv_imu_serif *serif,
    void (*sensor_event_cb)(xmedia_s32 dev, uint16_t packet_count, uint16_t idx,
    uint64_t time_inter, inv_imu_sensor_event_t *event))
{
    xmedia_s32 status = 0;

    osal_memset(s, 0, sizeof(*s));

    s->transport.serif = *serif;

    /* Supply ramp time max is 3 ms */
    osal_msleep(3);

    /* Register sensor event callback */
    s->sensor_event_cb = sensor_event_cb;

    /* Make sure `need_mclk_cnt` is cleared */
    s->transport.need_mclk_cnt = 0;

    /* Configure serial interface so we can trigger device reset */
    status |= configure_serial_interface(dev, s);

    /* Reset device */
    status |= inv_imu_device_reset(dev, s);
    if (status) {
        osal_printk("dev %d inv_imu_device_reset failed!", dev);
        return status;
    }

    /* Init transport layer */
    status |= inv_imu_init_transport(dev, s);

    /* Read and set endianness for further processing */
    status |= inv_imu_get_endianness(dev, s);

    /* Initialize hardware */
    status |= init_hardware_from_ui(dev, s);

    /* Set default value for sensor start/stop time */
    s->gyro_start_time_us = UINT_MAX;
    s->accel_start_time_us = UINT_MAX;

    return status;
}

xmedia_s32 inv_imu_get_who_am_i(xmedia_s32 dev, struct inv_imu_device *s, xmedia_u8 *who_am_i)
{
    return inv_imu_read_reg(dev, s, WHO_AM_I, 1, who_am_i);
}

static void inv_imu_callback(xmedia_s32 dev, uint16_t packet_count, uint16_t idx, uint64_t time_inter,
                             inv_imu_sensor_event_t *event)
{
    if (g_imu_data[dev].gyro_buf.data_num >= XMEDIA_MSENSOR_MAX_DATA_NUM) {
        g_imu_data[dev].gyro_buf.data_num = 0;
        MSENSOR_ERR_TRACE("dev %d gyro_buf is full, max num is %d\n", dev, XMEDIA_MSENSOR_MAX_DATA_NUM);
        return;
    }

    if (g_imu_data[dev].acc_buf.data_num >= XMEDIA_MSENSOR_MAX_DATA_NUM) {
        g_imu_data[dev].acc_buf.data_num = 0;
        MSENSOR_ERR_TRACE("dev %d acc_buf is full, max num is %d\n", dev, XMEDIA_MSENSOR_MAX_DATA_NUM);
        return;
    }

    if (event->sensor_mask & (1 << INV_SENSOR_GYRO)) {
        IMU_GYRO_DATA_X(dev) = event->gyro[0]; // gyro_data_x: gyro[0]
        IMU_GYRO_DATA_Y(dev) = event->gyro[1]; // gyro_data_y: gyro[1]
        IMU_GYRO_DATA_Z(dev) = event->gyro[2]; // gyro_data_z: gyro[2]
        IMU_GYRO_DATA_PTS(dev) = g_icm42670_dev[dev]->gyro_status.last_pts + time_inter;
        g_icm42670_dev[dev]->gyro_status.last_pts = IMU_GYRO_DATA_PTS(dev);

        if (event->sensor_mask & (1 << INV_SENSOR_TEMPERATURE)) {
            // 8bit quantity: temp = TEMP_DATA / 2 + 25
            IMU_GYRO_DATA_TEMP(dev) = (event->temperature * 1024 / 2) + 25 * 1024; // 1024: GRADIENT
        } else {
            IMU_GYRO_DATA_TEMP(dev) = 0;
        }

        g_imu_data[dev].gyro_buf.data_num++;
    }

    if (event->sensor_mask & (1 << INV_SENSOR_ACCEL)) {
        IMU_ACC_DATA_X(dev) = event->accel[0]; // acc_data_x: accel[0]
        IMU_ACC_DATA_Y(dev) = event->accel[1]; // acc_data_y: accel[1]
        IMU_ACC_DATA_Z(dev) = event->accel[2]; // acc_data_z: accel[2]
        IMU_ACC_DATA_PTS(dev) = g_icm42670_dev[dev]->acc_status.last_pts + time_inter;
        g_icm42670_dev[dev]->acc_status.last_pts = IMU_ACC_DATA_PTS(dev);

        if (event->sensor_mask & (1 << INV_SENSOR_TEMPERATURE)) {
            // 8bit quantity: temp = TEMP_DATA / 2 + 25
            IMU_ACC_DATA_TEMP(dev) = (event->temperature * 1024 / 2) + 25 * 1024; // 1024: GRADIENT
        } else {
            IMU_ACC_DATA_TEMP(dev) = 0;
        }

        g_imu_data[dev].acc_buf.data_num++;
    }

    if (g_time_cnt[dev] < TIME_RECORD_CNT) {
        g_time_backup[dev][g_time_cnt[dev]] = g_icm42670_dev[dev]->gyro_status.last_pts;
        g_time_cnt[dev]++;
    } else {
        g_time_backup[dev][TIME_RECORD_CNT - 1] = g_icm42670_dev[dev]->gyro_status.last_pts;
    }

    msensor_debug_trace(
        "dev %d idx %d gyro:%6d, %6d, %6d, acc:%6d, %6d, %6d, temperature:%4d, pts = %lld, sensor_mask = 0x%x\n", dev,
        idx, event->gyro[0], event->gyro[1], event->gyro[2], // gyro_data_z: gyro[2]
        event->accel[0], event->accel[1], event->accel[2], event->temperature, // acc_data_z: accel[2]
        g_icm42670_dev[dev]->acc_status.last_pts, event->sensor_mask);
}

xmedia_s32 setup_imu_device(xmedia_s32 dev, struct inv_imu_serif *icm_serif)
{
    xmedia_s32 rc = 0;
    xmedia_u8 who_am_i;

    MSENSOR_ERR_TRACE("initialize IMU %d!\n", dev);

    /* Init device */
    rc = inv_imu_init(dev, &icm_driver[dev], icm_serif, inv_imu_callback);
    if (rc != 0) {
        MSENSOR_ERR_TRACE("dev %d Failed to initialize IMU!\n", dev);
        return rc;
    }

    MSENSOR_ERR_TRACE("success to initialize IMU %d!\n", dev);

    /* Check WHOAMI */
    rc = inv_imu_get_who_am_i(dev, &icm_driver[dev], &who_am_i);
    if (rc != XMEDIA_SUCCESS) {
        MSENSOR_ERR_TRACE("dev %d Failed to read whoami!\n", dev);
        return rc;
    }

    if (who_am_i != ICM42670_SELFID) {
        MSENSOR_ERR_TRACE("dev %d Bad WHOAMI value!\n", dev);
        MSENSOR_ERR_TRACE("dev %d Read 0x%02x, expected 0x%02x\n", dev, who_am_i, ICM42670_SELFID);
        return XMEDIA_FAILURE;
    }
    osal_printk("dev %d imu chip id: 0x%x\n", dev, who_am_i);

    return rc;
}

static xmedia_s32 icm42670_sensor_init(xmedia_s32 dev, xmedia_u32 dev_mode)
{
    xmedia_s32 ret;

    if ((dev_mode & XMEDIA_MSENSOR_DEVICE_GYRO) || (dev_mode & XMEDIA_MSENSOR_DEVICE_ACC)) {
        ret = configure_imu_device(dev, dev_mode);
        if (ret != XMEDIA_SUCCESS) {
            MSENSOR_ERR_TRACE("dev %d icm42670 sensor init failed! ret=%x\n", dev, ret);
        }
    } else {
        MSENSOR_ERR_TRACE("dev %d icm42670 sensor init not support this mode: %d\n", dev, dev_mode);
        ret = XMEDIA_FAILURE;
    }
    return ret;
}

xmedia_void icm42670_sample_data_assign(xmedia_msensor_sample_data *dst, const xmedia_msensor_sample_data *src)
{
    dst->x = src->x;
    dst->y = src->y;
    dst->z = src->z;
    dst->temperature = src->temperature;
    dst->pts = src->pts;
    return;
}

static xmedia_void icm42670_get_time_inter(xmedia_s32 dev, xmedia_u64 *time_inter)
{
    xmedia_s32 i;
    xmedia_u32 adddatacnt = 0;

    if (g_data_count[dev] < DATA_RECORD_CNT) {
        g_every_data_cnt_backup[dev][g_data_count[dev]] = g_icm42670_dev[dev]->record_num;
    } else {
        for (i = 0; i < DATA_RECORD_CNT - 1; i++) {
            g_every_data_cnt_backup[dev][i] = g_every_data_cnt_backup[dev][i + 1];
        }
        g_every_data_cnt_backup[dev][DATA_RECORD_CNT - 1] = g_icm42670_dev[dev]->record_num;
    }
    if (g_time_cnt[dev] < TIME_RECORD_CNT) {
        g_time_backup[dev][g_time_cnt[dev]] = g_pts_now[dev];

        for (i = 0; i <= g_data_count[dev]; i++) {
            adddatacnt += g_every_data_cnt_backup[dev][i];
        }
        *time_inter = osal_div_u64(g_time_backup[dev][g_time_cnt[dev]] - g_time_backup[dev][0], div_0_to_1(adddatacnt));
    } else {
        for (i = 0; i < TIME_RECORD_CNT - 1; i++) {
            g_time_backup[dev][i] = g_time_backup[dev][i + 1];
        }

        g_time_backup[dev][TIME_RECORD_CNT - 1] = g_pts_now[dev];
        for (i = 1; i < DATA_RECORD_CNT; i++) {
            adddatacnt += g_every_data_cnt_backup[dev][i];
        }
        *time_inter =
            osal_div_u64(g_time_backup[dev][TIME_RECORD_CNT - 1] - g_time_backup[dev][0], div_0_to_1(adddatacnt));
    }
    if (g_data_count[dev] < DATA_RECORD_CNT) {
        g_data_count[dev]++;
    }
    return;
}

static xmedia_s32 icm42670_get_data_from_fifo(xmedia_s32 dev, struct inv_imu_device *s)
{
    xmedia_s32 status = 0;
    uint16_t packet_count_i, packet_count, total_packet_count = 0;
    uint16_t packet_size =
        FIFO_HEADER_SIZE + FIFO_ACCEL_DATA_SIZE + FIFO_GYRO_DATA_SIZE + FIFO_TEMP_DATA_SIZE + FIFO_TS_FSYNC_SIZE;
    fifo_header_t *header;
    // xmedia_u8 i;
    xmedia_u64 time_inter = 0;

    xmedia_u8 data[2] = { 0 }; /* 2 byte buf */

    /* FIFO record mode configured at driver init, so we read packet number, not byte count */
    // read how many records in fifo
    if ((status |= inv_imu_read_reg(dev, s, FIFO_COUNTH, 2, &data[0])) != XMEDIA_SUCCESS) { /* read count 2 */
        return status;
    }

    total_packet_count = (uint16_t)(data[0] | (data[1] << 8));
    packet_count = total_packet_count;

#if 0
    if (total_packet_count > 255) {
        status |= inv_imu_reset_fifo(dev, s);
        MSENSOR_ERR_TRACE("packet_count = %u, reset\n", packet_count);
        return status;
    }
#endif
    g_icm42670_dev[dev]->record_num = packet_count;
    g_icm42670_dev[dev]->fifo_length = packet_count * packet_size;

    while (packet_count > 0) {
        uint16_t invalid_frame_cnt = 0;
        /* Read FIFO only when data is expected in FIFO */
        /* fifo_idx type variable must be large enough to parse the FIFO_MIRRORING_SIZE */
        uint16_t fifo_idx = 0;

        if (s->fifo_highres_enabled) {
            packet_size = FIFO_20BYTES_PACKET_SIZE;
        }

/* begin to read fifo in several sections */
#if 0
        for (i = 0; i < packet_count; i++) {
            status |= icm42670_transfer_read(0x3f, s->fifo_data + i * packet_size, packet_size, XMEDIA_TRUE);
        }
#endif

        icm42670_transfer_read(dev, 0x3f, s->fifo_data, packet_size * packet_count, XMEDIA_TRUE);

        if (status != 0) {
            /*
             * Sensor data is in FIFO according to FIFO_COUNT but failed to read FIFO,
             * reset FIFO and try next chance
             */
            status |= inv_imu_reset_fifo(dev, s);
            status |= inv_imu_switch_off_mclk(dev, s);
            return status;
        }

        icm42670_get_time_inter(dev, &time_inter);

        for (packet_count_i = 0; packet_count_i < packet_count; packet_count_i++) {
            inv_imu_sensor_event_t event;
            event.sensor_mask = 0;
            header = (fifo_header_t *)&s->fifo_data[fifo_idx];
            fifo_idx += FIFO_HEADER_SIZE;

            // MSENSOR_ERR_TRACE("gyro_odr[%d],accel_odr[%d],fsync[%d],timestamp[%d],"
            //                   "twentybits[%d],gyro[%d],accel[%d],msg[%d],endian[%d]\n",
            //                   header->bits.gyro_odr_different, header->bits.accel_odr_different,
            //                   header->bits.fsync_bit, header->bits.timestamp_bit, header->bits.twentybits_bit,
            //                   header->bits.gyro_bit, header->bits.accel_bit, header->bits.msg_bit,
            //                   s->endianness_data);

            /* Decode invalid frame, this typically happens if packet_count is greater
            than 2 in case of WOM event since FIFO_THS IRQ is disabled if WOM is enabled,
            and we wake up only upon a WOM event so we can have more than 1 ACC packet in FIFO
            and we do not wait the oscillator wake-up time so we will receive 1 invalid packet,
            which we will read again upon next FIFO read operation thanks to while() loop */
            if (header->Byte == 0x80) {
                xmedia_u8 is_invalid_frame = 1;
                xmedia_u8 i;
                /* Check N-FIFO_HEADER_SIZE remaining bytes are all 0 to be invalid frame */
                for (i = 0; i < (packet_size - FIFO_HEADER_SIZE); i++) {
                    if (s->fifo_data[fifo_idx + i]) {
                        is_invalid_frame = 0;
                        break;
                    }
                }
                /* In case of invalid frame read FIFO will be retried for this packet */
                invalid_frame_cnt += is_invalid_frame;
                fifo_idx += packet_size - FIFO_HEADER_SIZE;
            } else {
                /* Decode packet */
                if (header->bits.msg_bit) {
                    /* MSG BIT set in FIFO header, Resetting FIFO */
                    status |= inv_imu_reset_fifo(dev, s);
                    status |= inv_imu_switch_off_mclk(dev, s);
                    MSENSOR_ERR_TRACE("dev %d status: %d\n", dev, status);
                    return INV_ERROR;
                }

                if (header->bits.accel_bit) {
                    if (s->endianness_data == INTF_CONFIG0_DATA_BIG_ENDIAN) {
                        event.accel[0] = (s->fifo_data[0 + fifo_idx] << 8) | s->fifo_data[1 + fifo_idx];
                        event.accel[1] = (s->fifo_data[2 + fifo_idx] << 8) | s->fifo_data[3 + fifo_idx];
                        event.accel[2] = // acc_data_z: accel[2]
                            (s->fifo_data[4 + fifo_idx] << 8) | s->fifo_data[5 + fifo_idx];
                    } else {
                        event.accel[0] = (s->fifo_data[1 + fifo_idx] << 8) | s->fifo_data[0 + fifo_idx];
                        event.accel[1] = (s->fifo_data[3 + fifo_idx] << 8) | s->fifo_data[2 + fifo_idx];
                        event.accel[2] = // acc_data_z: accel[2]
                            (s->fifo_data[5 + fifo_idx] << 8) | s->fifo_data[4 + fifo_idx];
                    }
                    fifo_idx += FIFO_ACCEL_DATA_SIZE;
                }

                if (header->bits.gyro_bit) {
                    if (s->endianness_data == INTF_CONFIG0_DATA_BIG_ENDIAN) {
                        event.gyro[0] = (s->fifo_data[0 + fifo_idx] << 8) | s->fifo_data[1 + fifo_idx];
                        event.gyro[1] = (s->fifo_data[2 + fifo_idx] << 8) | s->fifo_data[3 + fifo_idx];
                        event.gyro[2] = // gyro_data_z: gyro[2]
                            (s->fifo_data[4 + fifo_idx] << 8) | s->fifo_data[5 + fifo_idx];
                    } else {
                        event.gyro[0] = (s->fifo_data[1 + fifo_idx] << 8) | s->fifo_data[0 + fifo_idx];
                        event.gyro[1] = (s->fifo_data[3 + fifo_idx] << 8) | s->fifo_data[2 + fifo_idx];
                        event.gyro[2] = // gyro_data_z: gyro[2]
                            (s->fifo_data[5 + fifo_idx] << 8) | s->fifo_data[4 + fifo_idx];
                    }
                    fifo_idx += FIFO_GYRO_DATA_SIZE;
                }

                if ((header->bits.accel_bit) || (header->bits.gyro_bit)) {
                    /*
                     * The coarse temperature (8 or 16B FIFO packet format)
                     * range is ± 64 degrees with 0.5°C resolution.
                     * but the fine temperature range (2 bytes) (20B FIFO packet format) is
                     * ± 256 degrees with (1/128)°C resolution
                     */
                    if (header->bits.twentybits_bit) {
                        if (s->endianness_data == INTF_CONFIG0_DATA_BIG_ENDIAN) {
                            event.temperature =
                                (((xmedia_s16)s->fifo_data[0 + fifo_idx]) << 8) | s->fifo_data[1 + fifo_idx];
                        } else {
                            event.temperature =
                                (((xmedia_s16)s->fifo_data[1 + fifo_idx]) << 8) | s->fifo_data[0 + fifo_idx];
                        }
                        fifo_idx += FIFO_TEMP_DATA_SIZE + FIFO_TEMP_HIGH_RES_SIZE;

                        /* new temperature data */
                        if (event.temperature != INVALID_VALUE_FIFO)
                            event.sensor_mask |= (1 << INV_SENSOR_TEMPERATURE);
                    } else {
                        /* cast to int8_t since FIFO is in 16 bits mode (temperature on 8 bits) */
                        event.temperature = (int8_t)s->fifo_data[0 + fifo_idx];
                        fifo_idx += FIFO_TEMP_DATA_SIZE;

                        /* new temperature data */
                        if (event.temperature != INVALID_VALUE_FIFO_1B)
                            event.sensor_mask |= (1 << INV_SENSOR_TEMPERATURE);
                    }
                }

                if ((header->bits.timestamp_bit) || (header->bits.fsync_bit)) {
                    if (s->endianness_data == INTF_CONFIG0_DATA_BIG_ENDIAN)
                        event.timestamp_fsync = (s->fifo_data[0 + fifo_idx] << 8) | s->fifo_data[1 + fifo_idx];
                    else
                        event.timestamp_fsync = (s->fifo_data[1 + fifo_idx] << 8) | s->fifo_data[0 + fifo_idx];
                    fifo_idx += FIFO_TS_FSYNC_SIZE;
                    /* new fsync event */
                    if (header->bits.fsync_bit)
                        event.sensor_mask |= (1 << INV_SENSOR_FSYNC_EVENT);
                }

                if (header->bits.accel_bit) {
                    if ((event.accel[0] != INVALID_VALUE_FIFO) && (event.accel[1] != INVALID_VALUE_FIFO) &&
                        (event.accel[2] != INVALID_VALUE_FIFO)) { // acc_data_z: accel[2]
                        if (s->accel_start_time_us == UINT_MAX) {
                            event.sensor_mask |= (1 << INV_SENSOR_ACCEL);
                        } else {
                            if (!header->bits.fsync_bit) {
                                /* Discard first data after startup to let output to settle */
                                if ((inv_imu_get_time_us() - s->accel_start_time_us) >= ACC_STARTUP_TIME_US) {
                                    s->accel_start_time_us = UINT_MAX;
                                    event.sensor_mask |= (1 << INV_SENSOR_ACCEL);
                                }
                            }
                        }

                        if ((event.sensor_mask & (1 << INV_SENSOR_ACCEL)) && (header->bits.twentybits_bit)) {
                            // accel_high_resolution_x: accel_high_res[0]
                            event.accel_high_res[0] = (s->fifo_data[0 + fifo_idx] >> 4) & 0xF;
                            // accel_high_resolution_y: accel_high_res[1]
                            event.accel_high_res[1] = (s->fifo_data[1 + fifo_idx] >> 4) & 0xF;
                            // accel_high_resolution_z: accel_high_res[2]
                            event.accel_high_res[2] = (s->fifo_data[2 + fifo_idx] >> 4) & 0xF;
                        }
                    }
                }

                if (header->bits.gyro_bit) {
                    if ((event.gyro[0] != INVALID_VALUE_FIFO) && (event.gyro[1] != INVALID_VALUE_FIFO) &&
                        (event.gyro[2] != INVALID_VALUE_FIFO)) { // gyro_data_z: gyro[2]
                        if (s->gyro_start_time_us == UINT_MAX) {
                            event.sensor_mask |= (1 << INV_SENSOR_GYRO);
                        } else {
                            if (!header->bits.fsync_bit) {
                                /* Discard first data after startup to let output to settle */
                                if ((inv_imu_get_time_us() - s->gyro_start_time_us) >= GYR_STARTUP_TIME_US) {
                                    s->gyro_start_time_us = UINT_MAX;
                                    event.sensor_mask |= (1 << INV_SENSOR_GYRO);
                                }
                            }
                        }

                        if ((event.sensor_mask & (1 << INV_SENSOR_GYRO)) && (header->bits.twentybits_bit)) {
                            event.gyro_high_res[0] = (s->fifo_data[0 + fifo_idx]) & 0xF;
                            event.gyro_high_res[1] = (s->fifo_data[1 + fifo_idx]) & 0xF;
                            event.gyro_high_res[2] = (s->fifo_data[2 + fifo_idx]) & 0xF;
                        }
                    }
                }

                if (header->bits.twentybits_bit)
                    fifo_idx += FIFO_ACCEL_GYRO_HIGH_RES_SIZE;

                /* call sensor event callback */
                if (s->sensor_event_cb)
                    s->sensor_event_cb(dev, packet_count, packet_count_i, time_inter, &event);
            } /* end of else invalid frame */
        } /* end of FIFO read for loop */
        packet_count = invalid_frame_cnt;
    } /* end of while: packet_count > 0 */

    return XMEDIA_SUCCESS;
}

xmedia_s32 icm42670_get_data(xmedia_s32 dev, struct inv_imu_device *s)
{
    xmedia_s32 status = 0;
    xmedia_u8 int_status;

    /* Ensure data ready status bit is set */
    if ((status |= inv_imu_read_reg(dev, s, INT_STATUS, 1, &int_status))) {
        return status;
    }

    if ((int_status & INT_STATUS_FIFO_THS_INT_MASK) || (int_status & INT_STATUS_FIFO_FULL_INT_MASK)) {
        /*
         * Force the idle bit to disable
         * The chip expects no delay between reading the FIFO count and reading FIFO data.
         * If the program is interupted for more than 1ms, the chip will switch the clock,
         * and we won't guarantee the proper functioning of the "read fifo" operation.
         */
        status |= inv_imu_switch_on_mclk(dev, s);

        status |= icm42670_get_data_from_fifo(dev, &icm_driver[dev]);

        status |= inv_imu_switch_off_mclk(dev, s);
    }

    return status;
}

static xmedia_void icm42670_get_data_attr(xmedia_s32 dev)
{
    // xmedia_s32 ret;

    if ((g_icm42670_dev[dev]->enable_kthread == XMEDIA_TRUE) && (g_data_num[dev] < 1000)) { /* max data num 1000 */
        /* get PTS */
        if (g_timer_frist_flag[dev] == 0) {
            xmedia_u64 time_now = drv_sys_get_timestamp();
            g_icm42670_dev[dev]->gyro_status.last_pts = time_now;
            g_icm42670_dev[dev]->acc_status.last_pts = time_now;

            g_timer_frist_flag[dev] = XMEDIA_TRUE;
            g_pts_now[dev] = time_now;
            g_time_backup[dev][g_time_cnt[dev]] = g_pts_now[dev];
            g_time_cnt[dev]++;

            // chip_fifo_data_reset();
            return;
        }

        if (g_icm42670_dev[dev]->fifo_en != 0) {
            /* get fifo count */
            // ret = icm42670_get_fifo_length_and_count(&icm_driver[dev]);
            // if (ret != XMEDIA_SUCCESS) {
            //    MSENSOR_ERR_TRACE("get_icm42670_fifo_length_and_count failed\n");
            //    return;
            // }
        }

        g_pts_now[dev] = drv_sys_get_timestamp();
    }
    return;
}

/* vi interrupt */
xmedia_s32 chip_get_data_for_one_frm(xmedia_s32 dev)
{
    xmedia_ulong flags;
    xmedia_s32 ret;

    /* ktime_t stime */
    /* stime is ktime_set(0, g_imu_data[dev]->triger_data.u_triger_info.timer_config.interval * NSEC_PER_USEC) */

    osal_spin_lock_irqsave(&g_lock_get_data[dev], &flags);
    icm42670_get_data_attr(dev);
    if (g_icm42670_dev[dev]->fifo_en != 0) {
        ret = icm42670_get_data(dev, &icm_driver[dev]);
        if (ret != XMEDIA_SUCCESS) {
            MSENSOR_ERR_TRACE("dev %d read data failed: %d\n", dev, ret);
            osal_spin_unlock_irqrestore(&g_lock_get_data[dev], &flags);
            return XMEDIA_FAILURE;
        }
    }

    osal_memcpy(&g_imu_data[dev].attr, &g_msensor_mode[dev], sizeof(xmedia_msensor_attr));
#ifdef MNGBUFF_ENABLE
    msensor_chip_int_callback(dev, &g_imu_data[dev]);
#endif

    (xmedia_void) osal_memset(&g_imu_data[dev], 0, sizeof(xmedia_msensor_data));
    osal_spin_unlock_irqrestore(&g_lock_get_data[dev], &flags);

    return XMEDIA_SUCCESS;
}

/* bottom half */
static xmedia_void icm42670_work1(osal_work_struct_t *work)
{
    xmedia_s32 dev = 1;
    xmedia_s32 ret = XMEDIA_SUCCESS;
    xmedia_ulong flags;
    xmedia_unused(work);

    osal_spin_lock_irqsave(&g_lock_get_data[dev], &flags);

    if (g_icm42670_dev[dev]->enable_kthread != 0) {
        icm42670_get_data_attr(dev);
        if ((g_icm42670_dev[dev]->fifo_en == XMEDIA_FALSE) && (g_data_num[dev] < 1000)) { /* max data num 1000 */
            // ret = icm42670_get_data_directly();
            if (ret != XMEDIA_SUCCESS) {
                MSENSOR_ERR_TRACE("dev %d read_data failed: %d\n", dev, ret);
                goto err0;
            }
        } else {
            ret = icm42670_get_data(dev, &icm_driver[dev]);
            if (ret != XMEDIA_SUCCESS) {
                MSENSOR_ERR_TRACE("dev %d get data handle failed: %d\n", dev, ret);
                goto err0;
            }
        }
    }

    osal_memcpy(&g_imu_data[dev].attr, &g_msensor_mode[dev], sizeof(xmedia_msensor_attr));
    if ((g_imu_data[dev].acc_buf.data_num > 0) || (g_imu_data[dev].gyro_buf.data_num > 0)) {

#ifdef MNGBUFF_ENABLE
        msensor_chip_int_callback(dev, &g_imu_data[dev]);
#endif
        (xmedia_void) osal_memset(&g_imu_data[dev], 0, sizeof(xmedia_msensor_data));
    }

err0:
    g_icm42670_dev[dev]->workqueue_call_times--;
    osal_wakeup(&g_icm42670_dev[dev]->wait_call_stop_working);
    osal_spin_unlock_irqrestore(&g_lock_get_data[dev], &flags);

    if (g_msensor_triger_mode[dev] == TRIGER_EXTERN_INTERRUPT) {
        // inv_imu_write_reg(&icm_driver[dev], INT_SOURCE0, 1, &g_int_source0_data);
    }
    return;
}

static xmedia_void icm42670_work0(osal_work_struct_t *work)
{
    xmedia_s32 dev = 0;
    xmedia_s32 ret = XMEDIA_SUCCESS;
    xmedia_ulong flags;
    xmedia_unused(work);

    osal_spin_lock_irqsave(&g_lock_get_data[dev], &flags);

    if (g_icm42670_dev[dev]->enable_kthread != 0) {
        icm42670_get_data_attr(dev);
        if ((g_icm42670_dev[dev]->fifo_en == XMEDIA_FALSE) && (g_data_num[dev] < 1000)) { /* max data num 1000 */
            // ret = icm42670_get_data_directly();
            if (ret != XMEDIA_SUCCESS) {
                MSENSOR_ERR_TRACE("dev %d read_data failed: %d\n", dev, ret);
                goto err0;
            }
        } else {
            ret = icm42670_get_data(dev, &icm_driver[dev]);
            if (ret != XMEDIA_SUCCESS) {
                MSENSOR_ERR_TRACE("dev %d get data handle failed: %d\n", dev, ret);
                goto err0;
            }
        }
    }

    osal_memcpy(&g_imu_data[dev].attr, &g_msensor_mode[dev], sizeof(xmedia_msensor_attr));
    if ((g_imu_data[dev].acc_buf.data_num > 0) || (g_imu_data[dev].gyro_buf.data_num > 0)) {

#ifdef MNGBUFF_ENABLE
        msensor_chip_int_callback(dev, &g_imu_data[dev]);
#endif
        (xmedia_void) osal_memset(&g_imu_data[dev], 0, sizeof(xmedia_msensor_data));
    }

err0:
    g_icm42670_dev[dev]->workqueue_call_times--;
    osal_wakeup(&g_icm42670_dev[dev]->wait_call_stop_working);
    osal_spin_unlock_irqrestore(&g_lock_get_data[dev], &flags);

    if (g_msensor_triger_mode[dev] == TRIGER_EXTERN_INTERRUPT) {
        // inv_imu_write_reg(&icm_driver[dev], INT_SOURCE0, 1, &g_int_source0_data);
    }
    return;
}

static xmedia_void icm42670_work2(osal_work_struct_t *work)
{
    xmedia_s32 dev = 2;
    xmedia_s32 ret = XMEDIA_SUCCESS;
    xmedia_ulong flags;
    xmedia_unused(work);

    osal_spin_lock_irqsave(&g_lock_get_data[dev], &flags);

    if (g_icm42670_dev[dev]->enable_kthread != 0) {
        icm42670_get_data_attr(dev);
        if ((g_icm42670_dev[dev]->fifo_en == XMEDIA_FALSE) && (g_data_num[dev] < 1000)) { /* max data num 1000 */
            // ret = icm42670_get_data_directly();
            if (ret != XMEDIA_SUCCESS) {
                MSENSOR_ERR_TRACE("dev %d read_data failed: %d\n", dev, ret);
                goto err0;
            }
        } else {
            ret = icm42670_get_data(dev, &icm_driver[dev]);
            if (ret != XMEDIA_SUCCESS) {
                MSENSOR_ERR_TRACE("dev %d get data handle failed: %d\n", dev, ret);
                goto err0;
            }
        }
    }

    osal_memcpy(&g_imu_data[dev].attr, &g_msensor_mode[dev], sizeof(xmedia_msensor_attr));
    if ((g_imu_data[dev].acc_buf.data_num > 0) || (g_imu_data[dev].gyro_buf.data_num > 0)) {

#ifdef MNGBUFF_ENABLE
        msensor_chip_int_callback(dev, &g_imu_data[dev]);
#endif
        (xmedia_void) osal_memset(&g_imu_data[dev], 0, sizeof(xmedia_msensor_data));
    }

err0:
    g_icm42670_dev[dev]->workqueue_call_times--;
    osal_wakeup(&g_icm42670_dev[dev]->wait_call_stop_working);
    osal_spin_unlock_irqrestore(&g_lock_get_data[dev], &flags);

    if (g_msensor_triger_mode[dev] == TRIGER_EXTERN_INTERRUPT) {
        // inv_imu_write_reg(&icm_driver[dev], INT_SOURCE0, 1, &g_int_source0_data);
    }
    return;
}

static xmedia_void icm42670_work3(osal_work_struct_t *work)
{
    xmedia_s32 dev = 3;
    xmedia_s32 ret = XMEDIA_SUCCESS;
    xmedia_ulong flags;
    xmedia_unused(work);

    osal_spin_lock_irqsave(&g_lock_get_data[dev], &flags);

    if (g_icm42670_dev[dev]->enable_kthread != 0) {
        icm42670_get_data_attr(dev);
        if ((g_icm42670_dev[dev]->fifo_en == XMEDIA_FALSE) && (g_data_num[dev] < 1000)) { /* max data num 1000 */
            // ret = icm42670_get_data_directly();
            if (ret != XMEDIA_SUCCESS) {
                MSENSOR_ERR_TRACE("dev %d read_data failed: %d\n", dev, ret);
                goto err0;
            }
        } else {
            ret = icm42670_get_data(dev, &icm_driver[dev]);
            if (ret != XMEDIA_SUCCESS) {
                MSENSOR_ERR_TRACE("dev %d get data handle failed: %d\n", dev, ret);
                goto err0;
            }
        }
    }

    osal_memcpy(&g_imu_data[dev].attr, &g_msensor_mode[dev], sizeof(xmedia_msensor_attr));
    if ((g_imu_data[dev].acc_buf.data_num > 0) || (g_imu_data[dev].gyro_buf.data_num > 0)) {

#ifdef MNGBUFF_ENABLE
        msensor_chip_int_callback(dev, &g_imu_data[dev]);
#endif
        (xmedia_void) osal_memset(&g_imu_data[dev], 0, sizeof(xmedia_msensor_data));
    }

err0:
    g_icm42670_dev[dev]->workqueue_call_times--;
    osal_wakeup(&g_icm42670_dev[dev]->wait_call_stop_working);
    osal_spin_unlock_irqrestore(&g_lock_get_data[dev], &flags);

    if (g_msensor_triger_mode[dev] == TRIGER_EXTERN_INTERRUPT) {
        // inv_imu_write_reg(&icm_driver[dev], INT_SOURCE0, 1, &g_int_source0_data);
    }
    return;
}

/* external interrupt */
static xmedia_s32 icm42670_clear_irq(xmedia_s32 dev)
{
    xmedia_u8 ch, ret;
    /* read INT status */
    ret = icm42670_transfer_read(dev, (xmedia_u8)INT_STATUS_DRDY, &ch, 1, XMEDIA_FALSE);
    if (ret != XMEDIA_SUCCESS) {
        MSENSOR_ERR_TRACE("dev %d read INT status failed\n", dev);
        return XMEDIA_FAILURE;
    }

    /* read watermark status */
    if (g_icm42670_dev[dev]->fifo_en != 0) {
        ret = icm42670_transfer_read(dev, (xmedia_u8)INT_SOURCE3, &ch, 1, XMEDIA_FALSE);
        if (ret != XMEDIA_SUCCESS) {
            MSENSOR_ERR_TRACE("dev %d read INT status failed\n", dev);
            return XMEDIA_FAILURE;
        }
    }
    return XMEDIA_SUCCESS;
}

// 中断触发方式后续在调试
static irqreturn_t icm42670_irq(xmedia_s32 irq, xmedia_void *data)
{
    xmedia_s32 ret;

    xmedia_unused(irq);
    xmedia_unused(data);

    ret = icm42670_clear_irq(1);
    if (ret != XMEDIA_SUCCESS) {
        MSENSOR_ERR_TRACE("clear irq status failed\n");
        return XMEDIA_FAILURE;
    }

    // inv_imu_read_reg(&icm_driver[dev], INT_SOURCE0, 1, &g_int_source0_data);
    // inv_imu_write_reg(&icm_driver[dev], INT_SOURCE0, 1, &data_set);

    icm42670_get_data_attr(1);
    osal_schedule_work(&g_icm42670_dev[0]->work);
    g_icm42670_dev[0]->workqueue_call_times++;

    return IRQ_HANDLED;
}

xmedia_s32 chip_interrupt_run(xmedia_s32 dev)
{
    xmedia_s32 ret;

    g_icm42670_dev[dev]->enable_kthread = XMEDIA_TRUE;
    ret = osal_init_work(&g_icm42670_dev[dev]->work, icm42670_work1);
    if (ret != XMEDIA_SUCCESS) {
        return ret;
    }

    g_icm42670_dev[dev]->irq_num = gpio_to_irq(gpio_num(INT_GPIO_CHIP, INT_GPIO_OFFSET));
    ret = request_threaded_irq(g_icm42670_dev[dev]->irq_num, XMEDIA_NULL, icm42670_irq,
                               IRQF_TRIGGER_FALLING | IRQF_ONESHOT, "MotionSensor", g_icm42670_dev[dev]->client);
    if (ret != XMEDIA_SUCCESS) {
        MSENSOR_ERR_TRACE("gpio:%d_%d request irq failed: %d\n", INT_GPIO_CHIP, INT_GPIO_OFFSET, ret);
        return ret;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 chip_interrupt_stop(xmedia_s32 dev)
{

    if (g_icm42670_dev[dev]->irq_num != 0) {
        free_irq(g_icm42670_dev[dev]->irq_num, g_icm42670_dev[dev]->client);
        g_icm42670_dev[dev]->enable_kthread = XMEDIA_FALSE;

    } else {
        MSENSOR_ERR_TRACE("dev %d irq has already free!!\n", dev);
    }

    osal_destroy_work(&g_icm42670_dev[dev]->work);
    return XMEDIA_SUCCESS;
}

/* timer */
static xmedia_s32 icm42670_wait_stop_working_call_back(const xmedia_void *param)
{
    xmedia_u32 *dev = XMEDIA_NULL;

    if (param == XMEDIA_NULL) {
        MSENSOR_ERR_TRACE("param is null!\n");
        return 0;
    }

    dev = (xmedia_s32 *)param;

    if (g_icm42670_dev[*dev]->workqueue_call_times == 0) {
        return 1;
    }
    return 0;
}

static xmedia_void icm42670_timer_start(xmedia_s32 dev)
{
    ktime_t stime;

    stime = ktime_set(0, g_icm42670_dev[dev]->triger_data.triger_info.timer_config.interval * NSEC_PER_USEC);
    hrtimer_start(&g_icm42670_dev[dev]->hrtimer, stime, HRTIMER_MODE_REL);
    return;
}

static enum hrtimer_restart timer_hr_interrupt0(struct hrtimer *timer)
{
    xmedia_s32 ret;
    ktime_t stime;

    stime = ktime_set(0, g_icm42670_dev[0]->triger_data.triger_info.timer_config.interval * NSEC_PER_USEC);
    hrtimer_forward_now(timer, stime);

    ret = osal_schedule_work(&g_icm42670_dev[0]->work);
    if (ret == XMEDIA_FALSE) {
        return XMEDIA_FAILURE;
    }
    g_icm42670_dev[0]->workqueue_call_times++;
    return HRTIMER_RESTART;
}

static enum hrtimer_restart timer_hr_interrupt1(struct hrtimer *timer)
{
    xmedia_s32 ret;

    ktime_t stime;

    stime = ktime_set(0, g_icm42670_dev[1]->triger_data.triger_info.timer_config.interval * NSEC_PER_USEC);
    hrtimer_forward_now(timer, stime);

    ret = osal_schedule_work(&g_icm42670_dev[1]->work);
    if (ret == XMEDIA_FALSE) {
        return XMEDIA_FAILURE;
    }
    g_icm42670_dev[1]->workqueue_call_times++;
    return HRTIMER_RESTART;
}

static enum hrtimer_restart timer_hr_interrupt2(struct hrtimer *timer)
{
    xmedia_s32 ret;

    ktime_t stime;
    stime = ktime_set(0, g_icm42670_dev[2]->triger_data.triger_info.timer_config.interval * NSEC_PER_USEC);
    hrtimer_forward_now(timer, stime);

    ret = osal_schedule_work(&g_icm42670_dev[2]->work);
    if (ret == XMEDIA_FALSE) {
        return XMEDIA_FAILURE;
    }
    g_icm42670_dev[2]->workqueue_call_times++;
    return HRTIMER_RESTART;
}

static enum hrtimer_restart timer_hr_interrupt3(struct hrtimer *timer)
{
    xmedia_s32 ret;

    ktime_t stime;
    stime = ktime_set(0, g_icm42670_dev[3]->triger_data.triger_info.timer_config.interval * NSEC_PER_USEC);
    hrtimer_forward_now(timer, stime);

    ret = osal_schedule_work(&g_icm42670_dev[3]->work);
    if (ret == XMEDIA_FALSE) {
        return XMEDIA_FAILURE;
    }
    g_icm42670_dev[3]->workqueue_call_times++;
    return HRTIMER_RESTART;
}

static xmedia_s32 icm42670_timer_init(xmedia_s32 dev)
{
    if (dev == 0) {
        hrtimer_init(&g_icm42670_dev[dev]->hrtimer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
        g_icm42670_dev[dev]->hrtimer.function = timer_hr_interrupt0;
    } else if (dev == 1) {
        hrtimer_init(&g_icm42670_dev[dev]->hrtimer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
        g_icm42670_dev[dev]->hrtimer.function = timer_hr_interrupt1;
    } else if (dev == 2) {
        hrtimer_init(&g_icm42670_dev[dev]->hrtimer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
        g_icm42670_dev[dev]->hrtimer.function = timer_hr_interrupt2;
    } else if (dev == 3) {
        hrtimer_init(&g_icm42670_dev[dev]->hrtimer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
        g_icm42670_dev[dev]->hrtimer.function = timer_hr_interrupt3;
    } else {
        hrtimer_init(&g_icm42670_dev[dev]->hrtimer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
        g_icm42670_dev[dev]->hrtimer.function = timer_hr_interrupt1;
    }

    return XMEDIA_SUCCESS;
}

static xmedia_s32 icm42670_timer_exit(xmedia_s32 dev)
{
    hrtimer_cancel(&g_icm42670_dev[dev]->hrtimer);
    return XMEDIA_SUCCESS;
}

xmedia_s32 chip_timer_run(xmedia_s32 dev)
{
    xmedia_s32 ret;

    g_icm42670_dev[dev]->enable_kthread = XMEDIA_TRUE;
    ret = icm42670_timer_init(dev);
    if (ret != XMEDIA_SUCCESS) {
        MSENSOR_ERR_TRACE("dev %d msensor timer init failed\n", dev);
        return XMEDIA_FAILURE;
    }
    if (dev == 0) {
        osal_init_work(&g_icm42670_dev[dev]->work, icm42670_work0);
    } else if (dev == 1) {
        osal_init_work(&g_icm42670_dev[dev]->work, icm42670_work1);
    } else if (dev == 2) {
        osal_init_work(&g_icm42670_dev[dev]->work, icm42670_work2);
    } else if (dev == 3) {
        osal_init_work(&g_icm42670_dev[dev]->work, icm42670_work3);
    } else {
        osal_init_work(&g_icm42670_dev[dev]->work, icm42670_work1);
    }

    g_time_cnt[dev] = 0;
    g_data_count[dev] = 0;
    g_timer_frist_flag[dev] = XMEDIA_FALSE;
    g_icm42670_dev[dev]->record_num = 0;

    icm42670_timer_start(dev);

    return XMEDIA_SUCCESS;
}

xmedia_s32 chip_timer_stop(xmedia_s32 dev)
{
    xmedia_ulong flags;
    xmedia_s32 ret;

    ret = icm42670_timer_exit(dev);
    if (ret != XMEDIA_SUCCESS) {
        MSENSOR_ERR_TRACE("dev %d msensor timer deinit failed\n", dev);
        return XMEDIA_FAILURE;
    }

    osal_wait_uninterruptible(&g_icm42670_dev[dev]->wait_call_stop_working, icm42670_wait_stop_working_call_back, &dev);

    osal_spin_lock_irqsave(&g_lock_get_data[dev], &flags);
    g_icm42670_dev[dev]->enable_kthread = XMEDIA_FALSE;
    osal_destroy_work(&g_icm42670_dev[dev]->work);
    osal_spin_unlock_irqrestore(&g_lock_get_data[dev], &flags);
    (xmedia_void) osal_memset(g_time_backup[dev], 0, sizeof(g_time_backup[dev]));
    (xmedia_void) osal_memset(g_every_data_cnt_backup[dev], 0, sizeof(g_every_data_cnt_backup[dev]));
    return XMEDIA_SUCCESS;
}

/* user init */
static xmedia_s32 icm42670_set_gyro_param(xmedia_s32 dev, xmedia_msensor_param *msensor_param)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    xmedia_u32 sample_rate, value;

    xmedia_unused(dev);
    /* set gyro_odr_config */
    value = msensor_param->config.gyro_config.odr / XMEDIA_MSENSOR_GRADIENT;
    sample_rate = value;

    if (value == 0) {
        return XMEDIA_FAILURE;
    }

    if (sample_rate != GYRO_OUTPUT_DATA_RATE_1600HZ) {
        if (((1600 % value) != 0) || (value > 1600)) { /* max odr 1600 */
            value = GYRO_OUTPUT_DATA_RATE_BUTT;
        } else {
            value = GYRO_OUTPUT_DATA_RATE_800HZ;
        }
    }

    switch (value) {
        case GYRO_OUTPUT_DATA_RATE_800HZ:
        case GYRO_OUTPUT_DATA_RATE_1600HZ:
            msensor_param->config.gyro_config.odr = sample_rate;
            break;

        case GYRO_OUTPUT_DATA_RATE_BUTT:
            MSENSOR_ERR_TRACE("dev %d not support gyro odr!\n", dev);
            ret = XMEDIA_FAILURE;
            break;

        default:
            MSENSOR_ERR_TRACE("dev %d out of gyro range!!!\n", dev);
            ret = XMEDIA_FAILURE;
            break;
    }

    /* set gyro valid data bit */
    msensor_param->config.gyro_config.data_width = ICM42670_VALID_DATA_BIT;
    return ret;
}

static xmedia_s32 icm42670_set_acc_param(xmedia_s32 dev, xmedia_msensor_param *msensor_param)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    xmedia_u32 sample_rate, value;

    xmedia_unused(dev);
    /* set accel_odr_config */
    value = msensor_param->config.acc_config.odr / XMEDIA_MSENSOR_GRADIENT;
    sample_rate = value;

    if (value == 0) {
        return XMEDIA_FAILURE;
    }

    if (sample_rate != ACCEL_OUTPUT_DATA_RATE_1600HZ) {
        if (((1600 % value) != 0) || (value > 1600)) { /* max odr 1600 */
            value = ACCEL_OUTPUT_DATA_RATE_BUTT;
        } else {
            value = ACCEL_OUTPUT_DATA_RATE_800HZ;
        }
    }

    switch (value) {
        case ACCEL_OUTPUT_DATA_RATE_800HZ:
        case ACCEL_OUTPUT_DATA_RATE_1600HZ:
            msensor_param->config.acc_config.odr = sample_rate;
            break;
        case ACCEL_OUTPUT_DATA_RATE_BUTT:
            MSENSOR_ERR_TRACE("dev %d not support accel odr!\n", dev);
            ret = XMEDIA_FAILURE;
            break;
        default:
            MSENSOR_ERR_TRACE("dev %d out of accel range!!!\n", dev);
            ret = XMEDIA_FAILURE;
            break;
    }

    /* set accel valid data bit */
    msensor_param->config.acc_config.data_width = ICM42670_VALID_DATA_BIT;
    return ret;
}

static xmedia_s32 icm42670_set_param(xmedia_s32 dev, xmedia_msensor_param *msensor_param)
{
    xmedia_s32 ret;

    if (XMEDIA_MSENSOR_DEVICE_GYRO & msensor_param->attr.device_mask) {
        ret = icm42670_set_gyro_param(dev, msensor_param);
        if (ret != XMEDIA_SUCCESS) {
            return ret;
        }
    }

    if (XMEDIA_MSENSOR_DEVICE_ACC & msensor_param->attr.device_mask) {
        ret = icm42670_set_acc_param(dev, msensor_param);
        if (ret != XMEDIA_SUCCESS) {
            return ret;
        }
    }

    osal_memcpy(&g_msensor_mode[dev], &msensor_param->attr, sizeof(xmedia_msensor_attr));

    return ret;
}

static xmedia_s32 icm42670_config_to_param(xmedia_s32 dev, xmedia_msensor_param *msensor_param)
{
    xmedia_s32 ret;

    if (msensor_param == XMEDIA_NULL) {
        MSENSOR_ERR_TRACE("dev %d motion_sensor_status is null\n", dev);
        return XMEDIA_FAILURE;
    }

    if ((msensor_param->attr.device_mask & XMEDIA_MSENSOR_DEVICE_GYRO) ||
        (msensor_param->attr.device_mask & XMEDIA_MSENSOR_DEVICE_ACC)) {
        ret = icm42670_set_param(dev, msensor_param);
        if (ret != XMEDIA_SUCCESS) {
            MSENSOR_ERR_TRACE("dev %d icm42670 set param failed! ret=%x\n", dev, ret);
        }
    } else {
        MSENSOR_ERR_TRACE("dev %d not support mode!\n", dev);
        ret = XMEDIA_FAILURE;
    }
    return ret;
}

static xmedia_s32 icm42670_gyro_param_init(xmedia_s32 dev, xmedia_msensor_param *msensor_param)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    /* gyro attr set */
    if ((XMEDIA_MSENSOR_DEVICE_GYRO & msensor_param->attr.device_mask) == 0) {
        return ret;
    }

    if (msensor_param->config.gyro_config.odr == GYRO_OUTPUT_DATA_RATE_1600HZ) {
        g_icm42670_dev[dev]->gyro_status.band_width = GYRO_BAND_WIDTH_180HZ;
    } else if (msensor_param->config.gyro_config.odr <= 100) { /* 100 odr */
        g_icm42670_dev[dev]->gyro_status.band_width = GYRO_BAND_WIDTH_34HZ;
    } else if (msensor_param->config.gyro_config.odr == 200) { /* 200 odr */
        g_icm42670_dev[dev]->gyro_status.band_width = GYRO_BAND_WIDTH_53HZ;
    } else if (msensor_param->config.gyro_config.odr == 400) { /* 400 odr */
        g_icm42670_dev[dev]->gyro_status.band_width = GYRO_BAND_WIDTH_73HZ;
    } else if (msensor_param->config.gyro_config.odr == 800) { /* 800 odr */
        g_icm42670_dev[dev]->gyro_status.band_width = GYRO_BAND_WIDTH_121HZ;
    } else {
        MSENSOR_ERR_TRACE("dev %d icm42670 param init failed! gyro_odr:%d not found !\n", dev,
                          msensor_param->config.gyro_config.odr);
        ret = XMEDIA_FAILURE;
    }
    g_icm42670_dev[dev]->gyro_status.gyro_config.odr = msensor_param->config.gyro_config.odr;

    switch (msensor_param->config.gyro_config.fsr) {
        case GYRO_FULL_SCALE_SET_2KDPS:
        case GYRO_FULL_SCALE_SET_1KDPS:
        case GYRO_FULL_SCALE_SET_500DPS:
        case GYRO_FULL_SCALE_SET_250DPS:
            g_icm42670_dev[dev]->gyro_status.gyro_config.fsr = msensor_param->config.gyro_config.fsr;
            break;

        default:
            MSENSOR_ERR_TRACE("dev %d icm42670 param init failed! gyro_fsr:%d not found !\n", dev,
                              msensor_param->config.gyro_config.fsr);
            ret = XMEDIA_FAILURE;
            break;
    }
    return ret;
}

static xmedia_s32 icm42670_acc_param_init(xmedia_s32 dev, xmedia_msensor_param *msensor_param)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    /* accel attr set */
    if ((XMEDIA_MSENSOR_DEVICE_ACC & msensor_param->attr.device_mask) == 0) {
        return ret;
    }

    if (msensor_param->config.acc_config.odr == ACCEL_OUTPUT_DATA_RATE_1600HZ) {
        g_icm42670_dev[dev]->acc_status.band_width = ACCEL_BAND_WIDTH_180HZ;
    } else if (msensor_param->config.acc_config.odr <= 100) { /* 100 odr */
        g_icm42670_dev[dev]->acc_status.band_width = ACCEL_BAND_WIDTH_34HZ;
    } else if (msensor_param->config.acc_config.odr == 200) { /* 200 odr */
        g_icm42670_dev[dev]->acc_status.band_width = ACCEL_BAND_WIDTH_53HZ;
    } else if (msensor_param->config.acc_config.odr == 400) { /* 400 odr */
        g_icm42670_dev[dev]->acc_status.band_width = ACCEL_BAND_WIDTH_73HZ;
    } else if (msensor_param->config.acc_config.odr == 800) { /* 800 odr */
        g_icm42670_dev[dev]->acc_status.band_width = ACCEL_BAND_WIDTH_121HZ;
    } else {
        MSENSOR_ERR_TRACE("dev %d icm42670 param init failed! acc_odr:%d not found !\n", dev,
                          msensor_param->config.acc_config.odr);
        ret = XMEDIA_FAILURE;
    }
    g_icm42670_dev[dev]->acc_status.acc_config.odr = msensor_param->config.acc_config.odr;

    switch (msensor_param->config.acc_config.fsr) {
        case ACCEL_UI_FULL_SCALE_SET_2G:
        case ACCEL_UI_FULL_SCALE_SET_4G:
        case ACCEL_UI_FULL_SCALE_SET_8G:
        case ACCEL_UI_FULL_SCALE_SET_16G:
            g_icm42670_dev[dev]->acc_status.acc_config.fsr = msensor_param->config.acc_config.fsr;
            break;

        default:
            MSENSOR_ERR_TRACE("dev %d icm42670 param init failed! accel_range:%d not found !\n", dev,
                              msensor_param->config.acc_config.fsr);
            ret = XMEDIA_FAILURE;
            break;
    }
    return ret;
}

static xmedia_s32 icm42670_param_init(xmedia_s32 dev, xmedia_msensor_param msensor_param)
{
    xmedia_s32 ret;

    ret = icm42670_gyro_param_init(dev, &msensor_param);
    if (ret != XMEDIA_SUCCESS) {
        return ret;
    }

    ret = icm42670_acc_param_init(dev, &msensor_param);
    if (ret != XMEDIA_SUCCESS) {
        return ret;
    }

    return ret;
}

static xmedia_void icm42670_triger_mode_init(xmedia_s32 dev, xmedia_msensor_param *msensor_param)
{
    g_icm42670_dev[dev]->triger_data.triger_mode = g_msensor_triger_mode[dev];
    g_icm42670_dev[dev]->fifo_en = g_msensor_fifo_en[dev];

    if (g_icm42670_dev[dev]->triger_data.triger_mode == TRIGER_TIMER) {
        if (msensor_param->config.gyro_config.odr <= 50) { /* 50 odr */
            g_icm42670_dev[dev]->triger_data.triger_info.timer_config.interval = 40000; // 40000 interval
        } else if (msensor_param->config.gyro_config.odr <= 200) { /* 200 odr */
            g_icm42670_dev[dev]->triger_data.triger_info.timer_config.interval = 30000; // 30000 interval
        } else if (msensor_param->config.gyro_config.odr <= 500) { /* 500 odr */
            g_icm42670_dev[dev]->triger_data.triger_info.timer_config.interval = 20000; // 20000 interval
        } else if (msensor_param->config.gyro_config.odr <= 800) { /* 800 odr */
            g_icm42670_dev[dev]->triger_data.triger_info.timer_config.interval = 10000; // 10000 interval
        } else if (msensor_param->config.gyro_config.odr <= 1600) { /* 1600 odr */
            g_icm42670_dev[dev]->triger_data.triger_info.timer_config.interval = 5000; // 5000 interval
        } else {
            g_icm42670_dev[dev]->triger_data.triger_info.timer_config.interval = 10000; // 10000 interval
        }
    } else if (g_icm42670_dev[dev]->triger_data.triger_mode == TRIGER_EXTERN_INTERRUPT) {
        g_icm42670_dev[dev]->triger_data.triger_info.extern_interrupt_config.interrupt_num = ICM42670_FIFO_MAX_RECORD;
    }
    return;
}

xmedia_s32 chip_get_triger_config(xmedia_s32 dev, triger_config *triger_data)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    triger_data->triger_mode = g_icm42670_dev[dev]->triger_data.triger_mode;
    triger_data->triger_info.timer_config.interval = g_icm42670_dev[dev]->triger_data.triger_info.timer_config.interval;

    return ret;
}

static xmedia_s32 icm42670_dev_init(xmedia_s32 dev, xmedia_msensor_param *msensor_param)
{
    xmedia_s32 ret;

    ret = icm42670_config_to_param(dev, msensor_param);
    if (ret != XMEDIA_SUCCESS) {
        MSENSOR_ERR_TRACE("dev %d icm42670 config to param failed! ret=%x\n", dev, ret);
        goto err_init;
    }

    ret = icm42670_param_init(dev, *msensor_param);
    if (ret != XMEDIA_SUCCESS) {
        MSENSOR_ERR_TRACE("dev %d icm42670 param init failed! ret=%x\n", dev, ret);
        goto err_init;
    }

    icm42670_triger_mode_init(dev, msensor_param);
    ret = icm42670_sensor_init(dev, msensor_param->attr.device_mask);
    if (ret != XMEDIA_SUCCESS) {
        MSENSOR_ERR_TRACE("dev %d icm42670 sensor init failed! ret=%x\n", dev, ret);
        goto err_init;
    }
    return XMEDIA_SUCCESS;

err_init:
    return XMEDIA_FAILURE;
}

xmedia_s32 chip_dev_init(xmedia_s32 dev, xmedia_msensor_param *msensor_param)
{
    xmedia_s32 ret;

    setup_mcu(&icm_serif[dev]);
    ret = setup_imu_device(dev, &icm_serif[dev]);
    if (ret != 0) {
        MSENSOR_ERR_TRACE("dev %d setup_imu_device err, ret %d\n", dev, ret);
        return XMEDIA_FAILURE;
    }

    ret = icm42670_dev_init(dev, msensor_param);
    if (ret != XMEDIA_SUCCESS) {
        MSENSOR_ERR_TRACE("dev %d icm42670 dev init failed\n", dev);
        return XMEDIA_FAILURE;
    }

    g_timer_frist_flag[dev] = XMEDIA_FALSE;
    g_time_cnt[dev] = 0;
    g_data_count[dev] = 0;

    return XMEDIA_SUCCESS;
}

/* for send and save yuv only */
xmedia_s32 chip_dev_mng_init(xmedia_s32 dev, xmedia_msensor_param *msensor_param)
{
    xmedia_s32 ret;

    osal_printk("######fun:%s line:%d motion_sensor_status:%p######\n", __func__, __LINE__, msensor_param);

    if (msensor_param == XMEDIA_NULL) {
        return XMEDIA_FAILURE;
    }

    ret = icm42670_config_to_param(dev, msensor_param);
    if (ret != XMEDIA_SUCCESS) {
        MSENSOR_ERR_TRACE("dev %d icm42670 config to param failed! ret=%x\n", dev, ret);
        return XMEDIA_FAILURE;
    }

    osal_printk("######fun:%s line:%d ######\n", __func__, __LINE__);

    ret = icm42670_param_init(dev, *msensor_param);
    if (ret != XMEDIA_SUCCESS) {
        MSENSOR_ERR_TRACE("dev %d icm42670 param init failed! ret=%x\n", dev, ret);
        return XMEDIA_FAILURE;
    }

    icm42670_triger_mode_init(dev, msensor_param);

    return XMEDIA_SUCCESS;
}

static xmedia_void icm42670_axis_fifo_sensor_deinit(xmedia_s32 dev)
{
    safe_kfree(g_icm42670_dev[dev]->fifo_buf);
    return;
}

static xmedia_void icm42670_sensor_exit(xmedia_s32 dev, xmedia_u32 axis_mode)
{
    if ((axis_mode & XMEDIA_MSENSOR_DEVICE_GYRO) || (axis_mode & XMEDIA_MSENSOR_DEVICE_ACC)) {
        icm42670_axis_fifo_sensor_deinit(dev);
    } else {
        MSENSOR_ERR_TRACE("dev %d icm42670 sensor deinit not support this mode : %u\n", dev, axis_mode);
    }
    return;
}

void chip_dev_exit(xmedia_s32 dev, xmedia_msensor_param *msensor_param)
{
    /* exit may need to be forced out when deinit is not called */
    g_icm42670_dev[dev]->record_num = 0;
    icm42670_sensor_exit(dev, msensor_param->attr.device_mask);
}

#ifdef TRANSFER_I2C
static struct i2c_board_info g_xmedia_icm42670_info = {
    I2C_BOARD_INFO("ICM42670", ICM42670_DEV_ADDR),
};
#endif

/* module init */
xmedia_s32 chip_sensor_init(xmedia_s32 dev)
{
    /* 1.malloc a ICM42670 dev */
    g_icm42670_dev[dev] = osal_kmalloc(sizeof(icm42670_dev_info), osal_gfp_kernel);
    if (g_icm42670_dev[dev] == XMEDIA_NULL) {
        MSENSOR_ERR_TRACE("could not allocate memory\n");
        return -ENOMEM;
    }

    (xmedia_void) osal_memset(g_icm42670_dev[dev], 0, sizeof(icm42670_dev_info));

    // ret = gpio_init();

    osal_spin_lock_init(&g_lock_get_data[dev]);
    osal_wait_init(&g_icm42670_dev[dev]->wait_call_stop_working);

    return XMEDIA_SUCCESS;
}

void chip_sensor_exit(xmedia_s32 dev)
{
    osal_wait_destroy(&g_icm42670_dev[dev]->wait_call_stop_working);

    gpio_deinit();

    osal_spin_lock_destroy(&g_lock_get_data[dev]);
    safe_kfree(g_icm42670_dev[dev]);
}
