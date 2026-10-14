#include <stdio.h>
#include <string.h>
#include "i2c_dev.h"
#include "imx179.h"
#include "imx179_ctrl.h"


static xmedia_s32 g_imx179_i2c_addr[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                     { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };

static xmedia_s32 g_imx179_i2c_fd[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                     { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };

#define IMX179_REG_MIRROR_FLIP 0x0101

xmedia_void imx179_delay_ms(xmedia_s32 ms)
{
    sleep_us(ms * 1000);
}

xmedia_s32 imx179_4lane_linear_640x480_init(xmedia_u32 dev)
{
    //5.1.2hs_video 1288x800 60fps 24M MCLK 4lane 256Mbps/lane
     xmedia_s32 ret = XMEDIA_SUCCESS;

     ret |= imx179_write_reg(dev, 0x41C0, 0x01);
     ret |= imx179_write_reg(dev, 0x0104, 0x01);//group
     ret |= imx179_write_reg(dev, 0x0100, 0x00);//STREAM OFF
     ret |= imx179_write_reg(dev, 0x0101, 0x00);
     ret |= imx179_write_reg(dev, 0x0103, 0x01);//SW reset
     ret |= imx179_write_reg(dev, 0x0202, 0x02); //0x0202, 0x09
     ret |= imx179_write_reg(dev, 0x0203, 0x68);//(0x0203, 0xcc)
     //PLL setting
     ret |= imx179_write_reg(dev, 0x0301, 0x05);
     ret |= imx179_write_reg(dev, 0x0303, 0x01);
     ret |= imx179_write_reg(dev, 0x0305, 0x06);
     ret |= imx179_write_reg(dev, 0x0309, 0x05);
     ret |= imx179_write_reg(dev, 0x030B, 0x01);
     ret |= imx179_write_reg(dev, 0x030C, 0x00);
     ret |= imx179_write_reg(dev, 0x030D, 0xA0); //(0x030D, 0xA2)
     ret |= imx179_write_reg(dev, 0x030E, 0x01);
     ret |= imx179_write_reg(dev, 0x0340, 0x02);//(0x0340, 0x09)
     ret |= imx179_write_reg(dev, 0x0341, 0x6c);
     ret |= imx179_write_reg(dev, 0x0342, 0x0D);
     ret |= imx179_write_reg(dev, 0x0343, 0x70);//(0x0343, 0x0D)
     ret |= imx179_write_reg(dev, 0x0344, 0x00);
     ret |= imx179_write_reg(dev, 0x0345, 0x28);
     ret |= imx179_write_reg(dev, 0x0346, 0x00);
     ret |= imx179_write_reg(dev, 0x0347, 0x20);// (0x0347, 0x00)
     ret |= imx179_write_reg(dev, 0x0348, 0x0C);
     ret |= imx179_write_reg(dev, 0x0349, 0xA7);
     ret |= imx179_write_reg(dev, 0x034A, 0x09);// (0x034A, 0x09)
     ret |= imx179_write_reg(dev, 0x034B, 0x7F);//(0x034B, 0x9F)
     ret |= imx179_write_reg(dev, 0x034C, 0x02);
     ret |= imx179_write_reg(dev, 0x034D, 0x80);
     ret |= imx179_write_reg(dev, 0x034E, 0x01);
     ret |= imx179_write_reg(dev, 0x034F, 0xE0);//(0x034F, 0xD0)
     ret |= imx179_write_reg(dev, 0x0383, 0x01);
     ret |= imx179_write_reg(dev, 0x0387, 0x01);
     ret |= imx179_write_reg(dev, 0x0390, 0x02);
     ret |= imx179_write_reg(dev, 0x0401, 0x02);
     ret |= imx179_write_reg(dev, 0x0405, 0x14);
     ret |= imx179_write_reg(dev, 0x3020, 0x10);
     ret |= imx179_write_reg(dev, 0x3041, 0x15);
     ret |= imx179_write_reg(dev, 0x3042, 0x87);
     ret |= imx179_write_reg(dev, 0x3089, 0x4F);
     ret |= imx179_write_reg(dev, 0x3309, 0x9A);
     ret |= imx179_write_reg(dev, 0x3344, 0x57);//(0x3344, 0x57)
     ret |= imx179_write_reg(dev, 0x3345, 0x1F);
     ret |= imx179_write_reg(dev, 0x3362, 0x0A);
     ret |= imx179_write_reg(dev, 0x3363, 0x0A);
     ret |= imx179_write_reg(dev, 0x3364, 0x00);
     ret |= imx179_write_reg(dev, 0x3368, 0x18);
     ret |= imx179_write_reg(dev, 0x3369, 0x00);
     ret |= imx179_write_reg(dev, 0x3370, 0x77);// (0x3370, 0x77)
     ret |= imx179_write_reg(dev, 0x3371, 0x2F);//(0x3371, 0x2F)
     ret |= imx179_write_reg(dev, 0x3372, 0x4F);
     ret |= imx179_write_reg(dev, 0x3373, 0x2F);
     ret |= imx179_write_reg(dev, 0x3374, 0x2F);
     ret |= imx179_write_reg(dev, 0x3375, 0x37);
     ret |= imx179_write_reg(dev, 0x3376, 0x9F);
     ret |= imx179_write_reg(dev, 0x3377, 0x37);
     ret |= imx179_write_reg(dev, 0x33C8, 0x00);
     ret |= imx179_write_reg(dev, 0x33D4, 0x03);
     ret |= imx179_write_reg(dev, 0x33D5, 0x20);
     ret |= imx179_write_reg(dev, 0x33D6, 0x02);
     ret |= imx179_write_reg(dev, 0x33D7, 0x58);
     ret |= imx179_write_reg(dev, 0x4100, 0x0E);
     ret |= imx179_write_reg(dev, 0x4108, 0x01);
     ret |= imx179_write_reg(dev, 0x4109, 0x7C);
     ret |= imx179_write_reg(dev, 0x0104, 0x00);//group
     ret |= imx179_write_reg(dev, 0x0100, 0x01);//STREAM ON

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "imx179_write_reg failed.\n");
        return XMEDIA_FAILURE;
    }

    SENSOR_PRINT("=================================================================================\n");
    SENSOR_PRINT("=== IMX179_24MInput_MIPI_4lane_10bit_640x480_120fps_LINEAR init success!=========\n");
    SENSOR_PRINT("=================================================================================\n");

    return XMEDIA_SUCCESS;

}

xmedia_s32 imx179_4lane_linear_3280x2464_init(xmedia_u32 dev)
{
    //3280x2464 30fps 24M MCLK 4lane 256Mbps/lane
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= imx179_write_reg(dev, 0x41C0, 0x01);
    ret |= imx179_write_reg(dev, 0x0104, 0x01);//group
    ret |= imx179_write_reg(dev, 0x0100, 0x00);//STREAM OFF
    ret |= imx179_write_reg(dev, 0x0101, 0x00);
    ret |= imx179_write_reg(dev, 0x0103, 0x01);//SW reset
    ret |= imx179_write_reg(dev, 0x0202, 0x09); //0x0202, 0x09
    ret |= imx179_write_reg(dev, 0x0203, 0xAD);//(0x0203, 0xcc)

    ret |= imx179_write_reg(dev, 0x0202, 0x0c); //0x0202, 0x09
    ret |= imx179_write_reg(dev, 0x0203, 0x1c);//(0x0203, 0xcc)
    //PLL setting
    ret |= imx179_write_reg(dev, 0x0301, 0x05);
    ret |= imx179_write_reg(dev, 0x0303, 0x01);
    ret |= imx179_write_reg(dev, 0x0305, 0x06);
    ret |= imx179_write_reg(dev, 0x0309, 0x05);
    ret |= imx179_write_reg(dev, 0x030B, 0x01);
    ret |= imx179_write_reg(dev, 0x030C, 0x00);
    ret |= imx179_write_reg(dev, 0x030D, 0xA0); //(0x030D, 0xA2)
    ret |= imx179_write_reg(dev, 0x030E, 0x01);
    ret |= imx179_write_reg(dev, 0x0340, 0x09);//(0x0340, 0x09)
    ret |= imx179_write_reg(dev, 0x0341, 0xB1);
    ret |= imx179_write_reg(dev, 0x0342, 0x0D);
    ret |= imx179_write_reg(dev, 0x0343, 0x70);//(0x0343, 0x0D)
    ret |= imx179_write_reg(dev, 0x0344, 0x00);
    ret |= imx179_write_reg(dev, 0x0345, 0x00);
    ret |= imx179_write_reg(dev, 0x0346, 0x00);
    ret |= imx179_write_reg(dev, 0x0347, 0x00);// (0x0347, 0x00)
    ret |= imx179_write_reg(dev, 0x0348, 0x0C);
    ret |= imx179_write_reg(dev, 0x0349, 0xCF);
    ret |= imx179_write_reg(dev, 0x034A, 0x09);// (0x034A, 0x09)
    ret |= imx179_write_reg(dev, 0x034B, 0x9F);//(0x034B, 0x9F)
    ret |= imx179_write_reg(dev, 0x034C, 0x0C);
    ret |= imx179_write_reg(dev, 0x034D, 0xD0);
    ret |= imx179_write_reg(dev, 0x034E, 0x09);
    ret |= imx179_write_reg(dev, 0x034F, 0xA0);//(0x034F, 0xD0)
    ret |= imx179_write_reg(dev, 0x0383, 0x01);
    ret |= imx179_write_reg(dev, 0x0387, 0x01);
    ret |= imx179_write_reg(dev, 0x0390, 0x00);
    ret |= imx179_write_reg(dev, 0x0401, 0x00);
    ret |= imx179_write_reg(dev, 0x0405, 0x10);
    ret |= imx179_write_reg(dev, 0x3020, 0x10);
    ret |= imx179_write_reg(dev, 0x3041, 0x15);
    ret |= imx179_write_reg(dev, 0x3042, 0x87);
    ret |= imx179_write_reg(dev, 0x3089, 0x4F);
    ret |= imx179_write_reg(dev, 0x3309, 0x9A);
    ret |= imx179_write_reg(dev, 0x3344, 0x57);//(0x3344, 0x57)
    ret |= imx179_write_reg(dev, 0x3345, 0x1F);
    ret |= imx179_write_reg(dev, 0x3362, 0x0A);
    ret |= imx179_write_reg(dev, 0x3363, 0x0A);
    ret |= imx179_write_reg(dev, 0x3364, 0x00);
    ret |= imx179_write_reg(dev, 0x3368, 0x18);
    ret |= imx179_write_reg(dev, 0x3369, 0x00);
    ret |= imx179_write_reg(dev, 0x3370, 0x77);// (0x3370, 0x77)
    ret |= imx179_write_reg(dev, 0x3371, 0x2F);//(0x3371, 0x2F)
    ret |= imx179_write_reg(dev, 0x3372, 0x4F);
    ret |= imx179_write_reg(dev, 0x3373, 0x2F);
    ret |= imx179_write_reg(dev, 0x3374, 0x2F);
    ret |= imx179_write_reg(dev, 0x3375, 0x37);
    ret |= imx179_write_reg(dev, 0x3376, 0x9F);
    ret |= imx179_write_reg(dev, 0x3377, 0x37);
    ret |= imx179_write_reg(dev, 0x33C8, 0x00);
    ret |= imx179_write_reg(dev, 0x33D4, 0x0C);
    ret |= imx179_write_reg(dev, 0x33D5, 0xD0);
    ret |= imx179_write_reg(dev, 0x33D6, 0x09);
    ret |= imx179_write_reg(dev, 0x33D7, 0xA0);
    ret |= imx179_write_reg(dev, 0x4100, 0x0E);
    ret |= imx179_write_reg(dev, 0x4108, 0x01);
    ret |= imx179_write_reg(dev, 0x4109, 0x7C);
    ret |= imx179_write_reg(dev, 0x3302, 0x01);
    ret |= imx179_write_reg(dev, 0x0104, 0x00);//group
    ret |= imx179_write_reg(dev, 0x0100, 0x01);//STREAM ON

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "imx179_write_reg failed.\n");
        return XMEDIA_FAILURE;
    }

    SENSOR_PRINT("=================================================================================\n");
    SENSOR_PRINT("=== IMX179_24MInput_MIPI_4lane_10bit_3280x2464_30fps_LINEAR init success!========\n");
    SENSOR_PRINT("=================================================================================\n");

    return XMEDIA_SUCCESS;
}

xmedia_s32 imx179_init_image(xmedia_u32 dev, xmedia_u8 image_mode)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch(image_mode) {
        case IMX179_8M_10BIT_LINEAR_MODE:
            ret = imx179_4lane_linear_3280x2464_init(dev);
            break;
        case IMX179_1M_10BIT_LINEAR_MODE:
            ret = imx179_4lane_linear_640x480_init(dev);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image param!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return ret;
}

xmedia_s32 imx179_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32  ret;

    if (g_imx179_i2c_fd[dev] < 0) {
        SENSOR_PRINT("Sensor dev [%d], i2c not open. please init i2c at first.\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_set_slave_addr(g_imx179_i2c_fd[dev], slave_addr);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        i2c_close(g_imx179_i2c_fd[dev]);
        g_imx179_i2c_fd[dev] = SENSOR_I2C_INVALID;
        return XMEDIA_FAILURE;
    }

    g_imx179_i2c_addr[dev] = slave_addr;
    SENSOR_PRINT("Sensor dev [%d] set slave addr 0x%x success.\n", dev, slave_addr);

    return XMEDIA_SUCCESS;
}

xmedia_s32 imx179_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr)
{
    xmedia_s32 ret;

    if (g_imx179_i2c_fd[dev] >= 0) {
        SENSOR_PRINT("====== Sensor dev [%d], i2c already init !=======\n", dev);
        return XMEDIA_SUCCESS;
    }

    if (i2c_dev == SENSOR_I2C_INVALID) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor i2c dev invalid! Please set i2c dev at first!\n");
        return XMEDIA_ERRCODE_NOT_CONFIG;
    }

    g_imx179_i2c_fd[dev] = i2c_open(i2c_dev);
    if (g_imx179_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", i2c_dev);
        return XMEDIA_ERRCODE_OPEN_FAILED;
    }

    ret = imx179_set_slave_addr(dev, i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);

    SENSOR_PRINT("====== i2c[%d] init success!=======\n", i2c_dev);
    return XMEDIA_SUCCESS;
}

xmedia_s32 imx179_i2c_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;

    ret = i2c_close(g_imx179_i2c_fd[dev]);
    if(ret < 0 ) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] close i2c failed!\n", dev);
        return ret;
    }

    g_imx179_i2c_fd[dev] = SENSOR_I2C_INVALID;

    return XMEDIA_SUCCESS;
}

xmedia_s32 imx179_read_reg(xmedia_u32 dev, xmedia_u32 addr,xmedia_u32 *data)
{
    xmedia_s32 ret;
    xmedia_u8  buf[IMX179_DATA_BYTE];

    if (g_imx179_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_read(g_imx179_i2c_fd[dev], g_imx179_i2c_addr[dev], addr, IMX179_ADDR_BYTE, buf, IMX179_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] read i2c addr [0x%X] failed!\n", dev, addr);
        return ret;
    }

    *data = buf[0]; //IMX179_DATA_BYTE == 1

    return XMEDIA_SUCCESS;
}


xmedia_s32 imx179_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data)
{
    xmedia_u32 idx = 0;
    xmedia_s32 ret;
    xmedia_u8  buf[8];

    if (g_imx179_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    buf[idx] = (addr >> 8) & 0xff;
    idx++;
    buf[idx] = addr & 0xff;
    idx++;
    buf[idx] = data & 0xff;

    ret = i2c_write(g_imx179_i2c_fd[dev], g_imx179_i2c_addr[dev], buf, IMX179_ADDR_BYTE + IMX179_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 imx179_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en)
{
    xmedia_s32 ret;
    xmedia_u32 mirror_flip;

    imx179_read_reg(dev, IMX179_REG_MIRROR_FLIP, &mirror_flip);

    if (mirror_en) {
        mirror_flip |= 0x01;
    } else {
        mirror_flip &= 0xfe;
    }

    if (flip_en) {
        mirror_flip |= 0x02;
    } else {
        mirror_flip &= 0xfd;
    }

    ret =  imx179_write_reg(dev, IMX179_REG_MIRROR_FLIP, mirror_flip);

    if (ret != XMEDIA_SUCCESS) {
            SENSOR_TRACE(MODULE_DBG_ERR, "imx179_write_reg failed\n");
    }

    return XMEDIA_SUCCESS;
}
