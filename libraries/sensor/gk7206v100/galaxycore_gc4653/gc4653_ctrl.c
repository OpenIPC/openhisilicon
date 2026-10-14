#include <stdio.h>
#include <string.h>
#include "i2c_dev.h"
#include "gc4653.h"
#include "gc4653_ctrl.h"

static xmedia_s32 g_gc4653_i2c_fd[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                        { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };
static xmedia_s32 g_gc4653_i2c_addr[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                        { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };

static void gc4653_delay_ms(xmedia_s32 ms)
{
    usleep(ms * 1000); // 1ms: 1000us
    return;
}

static xmedia_s32 gc4653_2lane_linear_2560x1440_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= gc4653_write_reg(dev, 0x03fe, 0xf0);
    ret |= gc4653_write_reg(dev, 0x03fe, 0x00);
    ret |= gc4653_write_reg(dev, 0x0317, 0x00);
    ret |= gc4653_write_reg(dev, 0x0320, 0x77);
    ret |= gc4653_write_reg(dev, 0x0324, 0xc8);
    ret |= gc4653_write_reg(dev, 0x0325, 0x06);
    ret |= gc4653_write_reg(dev, 0x0326, 0x60);
    ret |= gc4653_write_reg(dev, 0x0327, 0x03);
    ret |= gc4653_write_reg(dev, 0x0334, 0x40);
    ret |= gc4653_write_reg(dev, 0x0336, 0x60);
    ret |= gc4653_write_reg(dev, 0x0337, 0x82);
    ret |= gc4653_write_reg(dev, 0x0315, 0x25);
    ret |= gc4653_write_reg(dev, 0x031c, 0xc6);
    ret |= gc4653_write_reg(dev, 0x0287, 0x18);
    ret |= gc4653_write_reg(dev, 0x0084, 0x00);
    ret |= gc4653_write_reg(dev, 0x0087, 0x50);
    ret |= gc4653_write_reg(dev, 0x029d, 0x08);
    ret |= gc4653_write_reg(dev, 0x0290, 0x00);
    ret |= gc4653_write_reg(dev, 0x0340, 0x05);
    ret |= gc4653_write_reg(dev, 0x0341, 0xdc);
    ret |= gc4653_write_reg(dev, 0x0345, 0x06);
    ret |= gc4653_write_reg(dev, 0x034b, 0xb0);
    ret |= gc4653_write_reg(dev, 0x0352, 0x08);
    ret |= gc4653_write_reg(dev, 0x0354, 0x08);
    ret |= gc4653_write_reg(dev, 0x02d1, 0xe0);
    ret |= gc4653_write_reg(dev, 0x0223, 0xf2);
    ret |= gc4653_write_reg(dev, 0x0238, 0xa4);
    ret |= gc4653_write_reg(dev, 0x02ce, 0x7f);
    ret |= gc4653_write_reg(dev, 0x0232, 0xc4);
    ret |= gc4653_write_reg(dev, 0x02d3, 0x05);
    ret |= gc4653_write_reg(dev, 0x0243, 0x06);
    ret |= gc4653_write_reg(dev, 0x02ee, 0x30);
    ret |= gc4653_write_reg(dev, 0x026f, 0x70);
    ret |= gc4653_write_reg(dev, 0x0257, 0x09);
    ret |= gc4653_write_reg(dev, 0x0211, 0x02);
    ret |= gc4653_write_reg(dev, 0x0219, 0x09);
    ret |= gc4653_write_reg(dev, 0x023f, 0x2d);
    ret |= gc4653_write_reg(dev, 0x0518, 0x00);
    ret |= gc4653_write_reg(dev, 0x0519, 0x01);
    ret |= gc4653_write_reg(dev, 0x0515, 0x08);
    ret |= gc4653_write_reg(dev, 0x02d9, 0x3f);
    ret |= gc4653_write_reg(dev, 0x02da, 0x02);
    ret |= gc4653_write_reg(dev, 0x02db, 0xe8);
    ret |= gc4653_write_reg(dev, 0x02e6, 0x20);
    ret |= gc4653_write_reg(dev, 0x021b, 0x10);
    ret |= gc4653_write_reg(dev, 0x0252, 0x22);
    ret |= gc4653_write_reg(dev, 0x024e, 0x22);
    ret |= gc4653_write_reg(dev, 0x02c4, 0x01);
    ret |= gc4653_write_reg(dev, 0x021d, 0x17);
    ret |= gc4653_write_reg(dev, 0x024a, 0x01);
    ret |= gc4653_write_reg(dev, 0x02ca, 0x02);
    ret |= gc4653_write_reg(dev, 0x0262, 0x10);
    ret |= gc4653_write_reg(dev, 0x029a, 0x20);
    ret |= gc4653_write_reg(dev, 0x021c, 0x0e);
    ret |= gc4653_write_reg(dev, 0x0298, 0x03);
    ret |= gc4653_write_reg(dev, 0x029c, 0x00);
    ret |= gc4653_write_reg(dev, 0x027e, 0x14);
    ret |= gc4653_write_reg(dev, 0x02c2, 0x10);
    ret |= gc4653_write_reg(dev, 0x0540, 0x20);
    ret |= gc4653_write_reg(dev, 0x0546, 0x01);
    ret |= gc4653_write_reg(dev, 0x0548, 0x01);
    ret |= gc4653_write_reg(dev, 0x0544, 0x01);
    ret |= gc4653_write_reg(dev, 0x0242, 0x1b);
    ret |= gc4653_write_reg(dev, 0x02c0, 0x1b);
    ret |= gc4653_write_reg(dev, 0x02c3, 0x20);
    ret |= gc4653_write_reg(dev, 0x02e4, 0x10);
    ret |= gc4653_write_reg(dev, 0x022e, 0x00);
    ret |= gc4653_write_reg(dev, 0x027b, 0x3f);
    ret |= gc4653_write_reg(dev, 0x0269, 0x0f);
    ret |= gc4653_write_reg(dev, 0x02d2, 0x40);
    ret |= gc4653_write_reg(dev, 0x027c, 0x08);
    ret |= gc4653_write_reg(dev, 0x023a, 0x2e);
    ret |= gc4653_write_reg(dev, 0x0245, 0xce);
    ret |= gc4653_write_reg(dev, 0x0530, 0x20);
    ret |= gc4653_write_reg(dev, 0x0531, 0x02);
    ret |= gc4653_write_reg(dev, 0x0228, 0x50);
    ret |= gc4653_write_reg(dev, 0x02ab, 0x00);
    ret |= gc4653_write_reg(dev, 0x0250, 0x00);
    ret |= gc4653_write_reg(dev, 0x0221, 0x50);
    ret |= gc4653_write_reg(dev, 0x02ac, 0x00);
    ret |= gc4653_write_reg(dev, 0x02a5, 0x02);
    ret |= gc4653_write_reg(dev, 0x0260, 0x0b);
    ret |= gc4653_write_reg(dev, 0x0216, 0x04);
    ret |= gc4653_write_reg(dev, 0x0299, 0x1C);
    ret |= gc4653_write_reg(dev, 0x02bb, 0x0d);
    ret |= gc4653_write_reg(dev, 0x02a3, 0x02);
    ret |= gc4653_write_reg(dev, 0x02a4, 0x02);
    ret |= gc4653_write_reg(dev, 0x021e, 0x02);
    ret |= gc4653_write_reg(dev, 0x024f, 0x08);
    ret |= gc4653_write_reg(dev, 0x028c, 0x08);
    ret |= gc4653_write_reg(dev, 0x0532, 0x3f);
    ret |= gc4653_write_reg(dev, 0x0533, 0x02);
    ret |= gc4653_write_reg(dev, 0x0277, 0xc0);
    ret |= gc4653_write_reg(dev, 0x0276, 0xc0);
    ret |= gc4653_write_reg(dev, 0x0239, 0xc0);
    ret |= gc4653_write_reg(dev, 0x0202, 0x05);
    ret |= gc4653_write_reg(dev, 0x0203, 0xd0);
    ret |= gc4653_write_reg(dev, 0x0205, 0xc0);
    ret |= gc4653_write_reg(dev, 0x02b0, 0x68);
    ret |= gc4653_write_reg(dev, 0x0002, 0xa9);
    ret |= gc4653_write_reg(dev, 0x0004, 0x01);
    ret |= gc4653_write_reg(dev, 0x021a, 0x98);
    ret |= gc4653_write_reg(dev, 0x0266, 0xa0);
    ret |= gc4653_write_reg(dev, 0x0020, 0x01);
    ret |= gc4653_write_reg(dev, 0x0021, 0x03);
    ret |= gc4653_write_reg(dev, 0x0022, 0x00);
    ret |= gc4653_write_reg(dev, 0x0023, 0x04);
    ret |= gc4653_write_reg(dev, 0x0342, 0x06);
    ret |= gc4653_write_reg(dev, 0x0343, 0x40);
    ret |= gc4653_write_reg(dev, 0x03fe, 0x10);
    ret |= gc4653_write_reg(dev, 0x03fe, 0x00);
    ret |= gc4653_write_reg(dev, 0x0106, 0x78);
    ret |= gc4653_write_reg(dev, 0x0108, 0x0c);
    ret |= gc4653_write_reg(dev, 0x0114, 0x01);
    ret |= gc4653_write_reg(dev, 0x0115, 0x12);
    ret |= gc4653_write_reg(dev, 0x0180, 0x46);
    ret |= gc4653_write_reg(dev, 0x0181, 0x30);
    ret |= gc4653_write_reg(dev, 0x0182, 0x05);
    ret |= gc4653_write_reg(dev, 0x0185, 0x01);
    ret |= gc4653_write_reg(dev, 0x03fe, 0x10);
    ret |= gc4653_write_reg(dev, 0x03fe, 0x00);
    ret |= gc4653_write_reg(dev, 0x0100, 0x09);
    ret |= gc4653_write_reg(dev, 0x000f, 0x00);
    ret |= gc4653_write_reg(dev, 0x0080, 0x02);
    ret |= gc4653_write_reg(dev, 0x0097, 0x0a);
    ret |= gc4653_write_reg(dev, 0x0098, 0x10);
    ret |= gc4653_write_reg(dev, 0x0099, 0x05);
    ret |= gc4653_write_reg(dev, 0x009a, 0xb0);
    ret |= gc4653_write_reg(dev, 0x0317, 0x08);
    ret |= gc4653_write_reg(dev, 0x0a67, 0x80);
    ret |= gc4653_write_reg(dev, 0x0a70, 0x03);
    ret |= gc4653_write_reg(dev, 0x0a82, 0x00);
    ret |= gc4653_write_reg(dev, 0x0a83, 0x10);
    ret |= gc4653_write_reg(dev, 0x0a80, 0x2b);
    ret |= gc4653_write_reg(dev, 0x05be, 0x00);
    ret |= gc4653_write_reg(dev, 0x05a9, 0x01);
    ret |= gc4653_write_reg(dev, 0x0313, 0x80);
    ret |= gc4653_write_reg(dev, 0x05be, 0x01);
    ret |= gc4653_write_reg(dev, 0x0317, 0x00);
    ret |= gc4653_write_reg(dev, 0x0a67, 0x00);

    gc4653_delay_ms(18);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "gc4653_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    SENSOR_PRINT("=======================================================================================\n");
    SENSOR_PRINT("==== GC4653_27MInput_MIPI_2lane_10bit_linear_2560x1440_30fps init success!=============\n");
    SENSOR_PRINT("=======================================================================================\n");
    return XMEDIA_SUCCESS;
}

xmedia_s32 gc4653_init_image(xmedia_u32 dev, xmedia_u8 image_mode)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch(image_mode) {
        case GC4653_4M_10BIT_LINEAR_MODE:
            ret = gc4653_2lane_linear_2560x1440_init(dev);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image param!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return ret;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc4653_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32  ret;

    if (g_gc4653_i2c_fd[dev] < 0) {
        SENSOR_PRINT("Sensor dev [%d], i2c not open. please init i2c at first.\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_set_slave_addr(g_gc4653_i2c_fd[dev], slave_addr);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        i2c_close(g_gc4653_i2c_fd[dev]);
        g_gc4653_i2c_fd[dev] = SENSOR_I2C_INVALID;
        return XMEDIA_FAILURE;
    }

    g_gc4653_i2c_addr[dev] = slave_addr;
    SENSOR_PRINT("Sensor dev [%d] set slave addr 0x%x success.\n", dev, slave_addr);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc4653_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr)
{
    xmedia_s32 ret;

    if (g_gc4653_i2c_fd[dev] >= 0) {
        SENSOR_PRINT("====== Sensor dev [%d], i2c already init !=======\n", dev);
        return XMEDIA_SUCCESS;
    }

    if (i2c_dev == SENSOR_I2C_INVALID) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor i2c dev invalid! Please set i2c dev at first!\n");
        return XMEDIA_ERRCODE_NOT_CONFIG;
    }

    g_gc4653_i2c_fd[dev] = i2c_open(i2c_dev);
    if (g_gc4653_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", i2c_dev);
        return XMEDIA_ERRCODE_OPEN_FAILED;
    }

    ret = gc4653_set_slave_addr(dev, i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);
    SENSOR_PRINT("====== i2c[%d] init success!=======\n", i2c_dev);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc4653_i2c_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;

    ret = i2c_close(g_gc4653_i2c_fd[dev]);
    if(ret < 0 ) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] close i2c failed!\n", dev);
        return ret;
    }

    g_gc4653_i2c_fd[dev] = SENSOR_I2C_INVALID;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc4653_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data)
{
    xmedia_s32 ret;
    xmedia_u8  buf[GC4653_DATA_BYTE];

    if (g_gc4653_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_read(g_gc4653_i2c_fd[dev], g_gc4653_i2c_addr[dev], addr, GC4653_ADDR_BYTE, buf, GC4653_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] read i2c addr [0x%X] failed!\n", dev, addr);
        return ret;
    }

    *data = buf[0]; //GC4653_DATA_BYTE == 1

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc4653_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data)
{
    xmedia_u32 idx = 0;
    xmedia_s32 ret;
    xmedia_u8  buf[8];

    if (g_gc4653_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    buf[idx] = (addr >> 8) & 0xff;
    idx++;
    buf[idx] = addr & 0xff;
    idx++;
    buf[idx] = data & 0xff;

    ret = i2c_write(g_gc4653_i2c_fd[dev], g_gc4653_i2c_addr[dev], buf, GC4653_ADDR_BYTE + GC4653_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}

xmedia_s32 gc4653_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    if ((!mirror_en) && (!flip_en)) {
        ret |= gc4653_write_reg(dev, 0x031d, 0x2e);
        ret |= gc4653_write_reg(dev, 0x0101, 0x00);
        ret |= gc4653_write_reg(dev, 0x031d, 0x28);
    } else if ((mirror_en) && (!flip_en)) {
        ret |= gc4653_write_reg(dev, 0x031d, 0x2e);
        ret |= gc4653_write_reg(dev, 0x0101, 0x01);
        ret |= gc4653_write_reg(dev, 0x031d, 0x28);
    } else if ((!mirror_en) && (flip_en)) {
        ret |= gc4653_write_reg(dev, 0x031d, 0x2e);
        ret |= gc4653_write_reg(dev, 0x0101, 0x02);
        ret |= gc4653_write_reg(dev, 0x031d, 0x28);
    } else if ((mirror_en) && (flip_en)) {
        ret |= gc4653_write_reg(dev, 0x031d, 0x2e);
        ret |= gc4653_write_reg(dev, 0x0101, 0x03);
        ret |= gc4653_write_reg(dev, 0x031d, 0x28);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "gc4653_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}

