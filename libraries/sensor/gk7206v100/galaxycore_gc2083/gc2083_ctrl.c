#include <stdio.h>
#include <string.h>
#include "i2c_dev.h"
#include "gc2083.h"
#include "gc2083_ctrl.h"

SENSOR_PRIORITY_DATA static xmedia_s32 g_gc2083_i2c_addr[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };
SENSOR_PRIORITY_DATA static xmedia_s32 g_gc2083_i2c_fd[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };

/*
 * 函数功能: gc2083 MIPI 2lane 1080p线性模式初始化序列 - 1920x1080x30fps
 * 函数参数：
 *      dev -  sensor 设备号
 * 返回值：
 *
 */
SENSOR_PRIORITY_FUNC static xmedia_s32 gc2083_2lane_linear_1920x1080_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    //system
    ret |= gc2083_write_reg(dev, 0x03fe, 0xf0);
    ret |= gc2083_write_reg(dev, 0x03fe, 0xf0);
    ret |= gc2083_write_reg(dev, 0x03fe, 0xf0);
    ret |= gc2083_write_reg(dev, 0x03fe, 0x00);
    ret |= gc2083_write_reg(dev, 0x03f2, 0x00);
    ret |= gc2083_write_reg(dev, 0x03f3, 0x00);
    ret |= gc2083_write_reg(dev, 0x03f4, 0x36);
    ret |= gc2083_write_reg(dev, 0x03f5, 0xc0);
    ret |= gc2083_write_reg(dev, 0x03f6, 0x24);
    ret |= gc2083_write_reg(dev, 0x03f7, 0x01);
    ret |= gc2083_write_reg(dev, 0x03f8, 0x2c);
    ret |= gc2083_write_reg(dev, 0x03f9, 0x43);
    ret |= gc2083_write_reg(dev, 0x03fc, 0x8e);
    ret |= gc2083_write_reg(dev, 0x0381, 0x07);
    ret |= gc2083_write_reg(dev, 0x00d7, 0x29);
    //CISCTL & ANALOG
    ret |= gc2083_write_reg(dev, 0x0d6d, 0x18);
    ret |= gc2083_write_reg(dev, 0x00d5, 0x03);
    ret |= gc2083_write_reg(dev, 0x0082, 0x01);
    ret |= gc2083_write_reg(dev, 0x0db3, 0xd4);
    ret |= gc2083_write_reg(dev, 0x0db0, 0x0d);
    ret |= gc2083_write_reg(dev, 0x0db5, 0x96);
    ret |= gc2083_write_reg(dev, 0x0d03, 0x02);
    ret |= gc2083_write_reg(dev, 0x0d04, 0x02);
    ret |= gc2083_write_reg(dev, 0x0d05, 0x05);
    ret |= gc2083_write_reg(dev, 0x0d06, 0xba);
    ret |= gc2083_write_reg(dev, 0x0d07, 0x00);
    ret |= gc2083_write_reg(dev, 0x0d08, 0x11);
    ret |= gc2083_write_reg(dev, 0x0d09, 0x00);
    ret |= gc2083_write_reg(dev, 0x0d0a, 0x02);
    ret |= gc2083_write_reg(dev, 0x000b, 0x00);
    ret |= gc2083_write_reg(dev, 0x000c, 0x02);
    ret |= gc2083_write_reg(dev, 0x0d0d, 0x04);
    ret |= gc2083_write_reg(dev, 0x0d0e, 0x40);
    ret |= gc2083_write_reg(dev, 0x000f, 0x07);
    ret |= gc2083_write_reg(dev, 0x0010, 0x90);
    ret |= gc2083_write_reg(dev, 0x0017, 0x0c);
    ret |= gc2083_write_reg(dev, 0x0d73, 0x92);
    ret |= gc2083_write_reg(dev, 0x0076, 0x00);
    ret |= gc2083_write_reg(dev, 0x0d76, 0x00);
    ret |= gc2083_write_reg(dev, 0x0d41, 0x04);
    ret |= gc2083_write_reg(dev, 0x0d42, 0x65);
    ret |= gc2083_write_reg(dev, 0x0d7a, 0x10);
    ret |= gc2083_write_reg(dev, 0x0d19, 0x31);
    ret |= gc2083_write_reg(dev, 0x0d25, 0x0b);
    ret |= gc2083_write_reg(dev, 0x0d20, 0x60);
    ret |= gc2083_write_reg(dev, 0x0d27, 0x03);
    ret |= gc2083_write_reg(dev, 0x0d29, 0x60);
    ret |= gc2083_write_reg(dev, 0x0d43, 0x10);
    ret |= gc2083_write_reg(dev, 0x0d49, 0x10);
    ret |= gc2083_write_reg(dev, 0x0d55, 0x18);
    ret |= gc2083_write_reg(dev, 0x0dc2, 0x44);
    ret |= gc2083_write_reg(dev, 0x0058, 0x3c);
    ret |= gc2083_write_reg(dev, 0x00d8, 0x68);
    ret |= gc2083_write_reg(dev, 0x00d9, 0x14);
    ret |= gc2083_write_reg(dev, 0x00da, 0xc1);
    ret |= gc2083_write_reg(dev, 0x0050, 0x18);
    ret |= gc2083_write_reg(dev, 0x0db6, 0x3d);
    ret |= gc2083_write_reg(dev, 0x00d2, 0xbc);
    ret |= gc2083_write_reg(dev, 0x0d66, 0x42);
    ret |= gc2083_write_reg(dev, 0x008c, 0x07);
    ret |= gc2083_write_reg(dev, 0x008d, 0xff);
    //gain
    ret |= gc2083_write_reg(dev, 0x007a, 0x50); //global gain
    ret |= gc2083_write_reg(dev, 0x00d0, 0x00);
    ret |= gc2083_write_reg(dev, 0x0dc1, 0x00);
    //isp
    ret |= gc2083_write_reg(dev, 0x0102, 0xa9); //89
    ret |= gc2083_write_reg(dev, 0x0158, 0x00);
    ret |= gc2083_write_reg(dev, 0x0107, 0xa6);
    ret |= gc2083_write_reg(dev, 0x0108, 0xa9);
    ret |= gc2083_write_reg(dev, 0x0109, 0xa8);
    ret |= gc2083_write_reg(dev, 0x010a, 0xa7);
    ret |= gc2083_write_reg(dev, 0x010b, 0xff);
    ret |= gc2083_write_reg(dev, 0x010c, 0xff);
    ret |= gc2083_write_reg(dev, 0x0428, 0x86);//84
    ret |= gc2083_write_reg(dev, 0x0429, 0x86);//84
    ret |= gc2083_write_reg(dev, 0x042a, 0x86);//84
    ret |= gc2083_write_reg(dev, 0x042b, 0x68);//84
    ret |= gc2083_write_reg(dev, 0x042c, 0x68);//84
    ret |= gc2083_write_reg(dev, 0x042d, 0x68);//84
    ret |= gc2083_write_reg(dev, 0x042e, 0x68);//83
    ret |= gc2083_write_reg(dev, 0x042f, 0x68);//82
    ret |= gc2083_write_reg(dev, 0x0430, 0x4f);//82
    ret |= gc2083_write_reg(dev, 0x0431, 0x68);//82
    ret |= gc2083_write_reg(dev, 0x0432, 0x67);//82
    ret |= gc2083_write_reg(dev, 0x0433, 0x66);//82
    ret |= gc2083_write_reg(dev, 0x0434, 0x66);//82
    ret |= gc2083_write_reg(dev, 0x0435, 0x66);//82
    ret |= gc2083_write_reg(dev, 0x0436, 0x66);//64
    ret |= gc2083_write_reg(dev, 0x0437, 0x66);//68
    ret |= gc2083_write_reg(dev, 0x0438, 0x62);
    ret |= gc2083_write_reg(dev, 0x0439, 0x62);
    ret |= gc2083_write_reg(dev, 0x043a, 0x62);
    ret |= gc2083_write_reg(dev, 0x043b, 0x62);
    ret |= gc2083_write_reg(dev, 0x043c, 0x62);
    ret |= gc2083_write_reg(dev, 0x043d, 0x62);
    ret |= gc2083_write_reg(dev, 0x043e, 0x62);
    ret |= gc2083_write_reg(dev, 0x043f, 0x62);
    //dark sun
    ret |= gc2083_write_reg(dev, 0x0077, 0x01); //01 settle_en
    ret |= gc2083_write_reg(dev, 0x0078, 0x65); //settle_exp_th
    ret |= gc2083_write_reg(dev, 0x0079, 0x04); //settle_exp_th
    ret |= gc2083_write_reg(dev, 0x0067, 0xa0); //settle_ref_th
    ret |= gc2083_write_reg(dev, 0x0054, 0xff); //settle_sig_th
    ret |= gc2083_write_reg(dev, 0x0055, 0x02); //settle_sig_th
    ret |= gc2083_write_reg(dev, 0x0056, 0x00); //settle_colgain_th
    ret |= gc2083_write_reg(dev, 0x0057, 0x04); //settle_colgain_th
    ret |= gc2083_write_reg(dev, 0x005a, 0xff); //settle_data_value
    ret |= gc2083_write_reg(dev, 0x005b, 0x07); //settle_data_value
    //blk
    ret |= gc2083_write_reg(dev, 0x0026, 0x01);
    ret |= gc2083_write_reg(dev, 0x0152, 0x02);
    ret |= gc2083_write_reg(dev, 0x0153, 0x50);
    ret |= gc2083_write_reg(dev, 0x0155, 0x93);
    ret |= gc2083_write_reg(dev, 0x0410, 0x16);
    ret |= gc2083_write_reg(dev, 0x0411, 0x16);
    ret |= gc2083_write_reg(dev, 0x0412, 0x16);
    ret |= gc2083_write_reg(dev, 0x0413, 0x16);
    ret |= gc2083_write_reg(dev, 0x0414, 0x6f);
    ret |= gc2083_write_reg(dev, 0x0415, 0x6f);
    ret |= gc2083_write_reg(dev, 0x0416, 0x6f);
    ret |= gc2083_write_reg(dev, 0x0417, 0x6f);
    ret |= gc2083_write_reg(dev, 0x04e0, 0x18);
    //window
    ret |= gc2083_write_reg(dev, 0x0192, 0x02);
    ret |= gc2083_write_reg(dev, 0x0194, 0x02);
    ret |= gc2083_write_reg(dev, 0x0195, 0x04);
    ret |= gc2083_write_reg(dev, 0x0196, 0x38);
    ret |= gc2083_write_reg(dev, 0x0197, 0x07);
    ret |= gc2083_write_reg(dev, 0x0198, 0x80);
    //DVP & MIPI
    ret |= gc2083_write_reg(dev, 0x0201, 0x27); //20//27[6:5]clkctr [2]phy-lane1_en [1]phy-lane0_en [0]phy_clk_en
    ret |= gc2083_write_reg(dev, 0x0202, 0x53); //56//[7:6]data1ctr [5:4]data0ctr [3:0]mipi_diff

    //b2//b6[7]clklane_p2s_sel [6:5]data0hs_ph [4]data0_delay1s [3]clkdelay1s [2]mipi_en [1:0]clkhs_ph
    ret |= gc2083_write_reg(dev, 0x0203, 0xce);
    ret |= gc2083_write_reg(dev, 0x0204, 0x40);
    ret |= gc2083_write_reg(dev, 0x0212, 0x07);
    ret |= gc2083_write_reg(dev, 0x0213, 0x80); //LWC
    ret |= gc2083_write_reg(dev, 0x0215, 0x12); //[1:0]clk_lane_mode
    ret |= gc2083_write_reg(dev, 0x0229, 0x05);
    ret |= gc2083_write_reg(dev, 0x0237, 0x03);

    //91//40//91[7]lane_ena [6]DVPBUF_ena [5]ULPEna [4]MIPI_ena [3]mipi_set_auto_disable
    //[2]RAW8_mode [1]ine_sync_mode [0]double_lane_enff  03fe  00
    ret |= gc2083_write_reg(dev, 0x023e, 0x99);
    printf("===Galaxycore GC2083_1080P_30FPS_10BIT_LINEAR_Init_OK!===\n");

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "gc2083_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=================================================================================\n");
        SENSOR_PRINT("===== GC2083_27MInput_MIPI_2lane_10bit_1920x1080_30fps linear init success!======\n");
        SENSOR_PRINT("=================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

static xmedia_void gc2083_slave_init(xmedia_u32 dev)
{
    gc2083_write_reg(dev, 0x0068, 0x93);
    gc2083_write_reg(dev, 0x0069, 0x00);
    gc2083_write_reg(dev, 0x0d67, 0x00);
    gc2083_write_reg(dev, 0x0d69, 0x30);
    gc2083_write_reg(dev, 0x0d6a, 0x08);
    gc2083_write_reg(dev, 0x0d6b, 0x70);
    gc2083_write_reg(dev, 0x0d6c, 0x00);
    gc2083_write_reg(dev, 0x0d6d, 0x13);
    gc2083_write_reg(dev, 0x0d6e, 0x00);
    gc2083_write_reg(dev, 0x0d6f, 0x04);
    gc2083_write_reg(dev, 0x0d70, 0x00);
    gc2083_write_reg(dev, 0x0d71, 0x12);

    SENSOR_PRINT("====== GC2083 slave setting init success!=======\n");
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc2083_init_image(xmedia_u32 dev, xmedia_u8 image_mode, xmedia_sensor_work_mode work_mode)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch(image_mode) {
        case GC2083_2M_10BIT_LINEAR_MODE:
            ret = gc2083_2lane_linear_1920x1080_init(dev);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image param!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    if (work_mode == XMEDIA_SENSOR_WORK_MODE_SLAVE && image_mode == GC2083_2M_10BIT_LINEAR_MODE) {
        gc2083_slave_init(dev);
    }

    return ret;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc2083_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32 ret;

    if (g_gc2083_i2c_fd[dev] < 0) {
        SENSOR_PRINT("Sensor dev [%d], i2c not open. please init i2c at first.\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_set_slave_addr(g_gc2083_i2c_fd[dev], slave_addr);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        i2c_close(g_gc2083_i2c_fd[dev]);
        g_gc2083_i2c_fd[dev] = SENSOR_I2C_INVALID;
        return XMEDIA_FAILURE;
    }

    g_gc2083_i2c_addr[dev] = slave_addr;
    SENSOR_PRINT("Sensor dev [%d] set slave addr 0x%x success.\n", dev, slave_addr);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc2083_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr)
{
    xmedia_s32 ret;

    if (g_gc2083_i2c_fd[dev] >= 0) {
        SENSOR_PRINT("====== Sensor dev [%d], i2c already init !=======\n", dev);
        return XMEDIA_SUCCESS;
    }

    if (i2c_dev == SENSOR_I2C_INVALID) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor i2c dev invalid! Please set i2c dev at first!\n");
        return XMEDIA_ERRCODE_NOT_CONFIG;
    }

    g_gc2083_i2c_fd[dev] = i2c_open(i2c_dev);
    if (g_gc2083_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", i2c_dev);
        return XMEDIA_ERRCODE_OPEN_FAILED;
    }

    ret = gc2083_set_slave_addr(dev, i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);
    SENSOR_PRINT("====== i2c[%d] init success!=======\n", i2c_dev);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc2083_i2c_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;

    ret = i2c_close(g_gc2083_i2c_fd[dev]);
    if(ret < 0 ) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] close i2c failed!\n", dev);
        return ret;
    }

    g_gc2083_i2c_fd[dev] = SENSOR_I2C_INVALID;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc2083_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data)
{
    xmedia_s32 ret;
    xmedia_u8  buf[GC2083_DATA_BYTE];

    if (g_gc2083_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_read(g_gc2083_i2c_fd[dev], g_gc2083_i2c_addr[dev], addr, GC2083_ADDR_BYTE, buf, GC2083_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] read i2c addr [0x%X] failed!\n", dev, addr);
        return ret;
    }

    *data = buf[0]; //GC2083_DATA_BYTE == 1

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc2083_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data)
{
    xmedia_u32 idx = 0;
    xmedia_s32 ret;
    xmedia_u8  buf[8];

    if (g_gc2083_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    buf[idx] = (addr >> 8) & 0xff;
    idx++;
    buf[idx] = addr & 0xff;
    idx++;
    buf[idx] = data & 0xff;

    ret = i2c_write(g_gc2083_i2c_fd[dev], g_gc2083_i2c_addr[dev], buf, GC2083_ADDR_BYTE + GC2083_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 gc2083_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en)
{
    xmedia_s32 ret = 0;

    if ((!mirror_en) && (!flip_en)) {
        ret |= gc2083_write_reg(dev, 0x0015, 0x00);
        ret |= gc2083_write_reg(dev, 0x0d15, 0x00);
        ret |= gc2083_write_reg(dev, 0x0192, 0x02);
    } else if((mirror_en) && (!flip_en)) {
        ret |= gc2083_write_reg(dev, 0x0015, 0x01);
        ret |= gc2083_write_reg(dev, 0x0d15, 0x00);
        ret |= gc2083_write_reg(dev, 0x0192, 0x02);
    } else if((!mirror_en) && (flip_en)) {
        ret |= gc2083_write_reg(dev, 0x0015, 0x00);
        ret |= gc2083_write_reg(dev, 0x0d15, 0x02);
        ret |= gc2083_write_reg(dev, 0x0192, 0x03);
    } else {
        ret |= gc2083_write_reg(dev, 0x0015, 0x01);
        ret |= gc2083_write_reg(dev, 0x0d15, 0x02);
        ret |= gc2083_write_reg(dev, 0x0192, 0x03);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "gc2083_write_reg failed\n");
    }

    return XMEDIA_SUCCESS;
}
