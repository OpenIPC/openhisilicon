#include <stdio.h>
#include <string.h>
#include "i2c_dev.h"
#include "cv2005.h"
#include "cv2005_ctrl.h"

static xmedia_s32 g_cv2005_i2c_addr[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                       { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };

static xmedia_s32 g_cv2005_i2c_fd[XMEDIA_SENSOR_DEV_MAX_NUM] =
                                                       { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] = SENSOR_I2C_INVALID };

#define CV2005_REG_ADDR_MIRROR_FLIP 0x3028
#define CV2005_REG_ADDR_X_START     0x3038
#define CV2005_REG_ADDR_Y_START     0x3034
#define CV2005_REG_VALUE_X_START    4
#define CV2005_REG_VALUE_Y_START    4

static xmedia_s32 cv2005_2lane_linear_1920x1080_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= cv2005_write_reg(dev, 0x301C, 0x9C);
    ret |= cv2005_write_reg(dev, 0x301D, 0x04);
    ret |= cv2005_write_reg(dev, 0x301E, 0x00);
    ret |= cv2005_write_reg(dev, 0x3020, 0x4E);
    ret |= cv2005_write_reg(dev, 0x3021, 0x04);
    ret |= cv2005_write_reg(dev, 0x3808, 0x41);
    ret |= cv2005_write_reg(dev, 0x3031, 0x00);
    ret |= cv2005_write_reg(dev, 0x3204, 0x40);
    ret |= cv2005_write_reg(dev, 0x359d, 0x00);
    ret |= cv2005_write_reg(dev, 0x389D, 0x0A);
    ret |= cv2005_write_reg(dev, 0x389C, 0x6A);
    ret |= cv2005_write_reg(dev, 0x38A0, 0x2B);
    ret |= cv2005_write_reg(dev, 0x3878, 0x01);
    ret |= cv2005_write_reg(dev, 0x3879, 0x15);
    ret |= cv2005_write_reg(dev, 0x356F, 0x0f);
    ret |= cv2005_write_reg(dev, 0x3109, 0x01);
    ret |= cv2005_write_reg(dev, 0x3420, 0x2f);
    ret |= cv2005_write_reg(dev, 0x3422, 0x87);
    ret |= cv2005_write_reg(dev, 0x3424, 0x3f);
    ret |= cv2005_write_reg(dev, 0x3426, 0x57);
    ret |= cv2005_write_reg(dev, 0x3428, 0x2f);
    ret |= cv2005_write_reg(dev, 0x3000, 0x00); // Streaming
    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "cv2005_write_reg failed\n");
        return XMEDIA_FAILURE;
    } else {
        SENSOR_PRINT("=============================================================================================\n");
        SENSOR_PRINT("======== cv2005_normal_mipi_24Minput_2lane_10bit_linear_1920x1080_30fps init success ========\n");
        SENSOR_PRINT("=============================================================================================\n");
        return XMEDIA_SUCCESS;
    }
}

xmedia_s32 cv2005_init_image(xmedia_u32 dev, xmedia_u8 image_mode, xmedia_sensor_work_mode work_mode)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch (image_mode) {
        case CV2005_2LANE_1920X1080_LINEAR_MODE:
            ret = cv2005_2lane_linear_1920x1080_init(dev);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image param!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return ret;
}

SENSOR_PRIORITY_FUNC xmedia_s32 cv2005_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32 ret;

    if (g_cv2005_i2c_fd[dev] < 0) {
        SENSOR_PRINT("Sensor dev [%d], i2c not open. please init i2c at first.\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_set_slave_addr(g_cv2005_i2c_fd[dev], slave_addr);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        i2c_close(g_cv2005_i2c_fd[dev]);
        g_cv2005_i2c_fd[dev] = SENSOR_I2C_INVALID;
        return XMEDIA_FAILURE;
    }

    g_cv2005_i2c_addr[dev] = slave_addr;
    SENSOR_PRINT("Sensor dev [%d] set slave addr 0x%x success.\n", dev, slave_addr);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 cv2005_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr)
{
    xmedia_s32 ret;

    if (g_cv2005_i2c_fd[dev] >= 0) {
        SENSOR_PRINT("====== Sensor dev [%d], i2c already init !=======\n", dev);
        return XMEDIA_SUCCESS;
    }

    if (i2c_dev == SENSOR_I2C_INVALID) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor i2c dev invalid! Please set i2c dev at first!\n");
        return XMEDIA_ERRCODE_NOT_CONFIG;
    }

    g_cv2005_i2c_fd[dev] = i2c_open(i2c_dev);
    if (g_cv2005_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", i2c_dev);
        return XMEDIA_ERRCODE_OPEN_FAILED;
    }

    ret = cv2005_set_slave_addr(dev, i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);
    SENSOR_PRINT("====== i2c[%d] init success!=======\n", i2c_dev);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 cv2005_i2c_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;

    ret = i2c_close(g_cv2005_i2c_fd[dev]);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] close i2c failed!\n", dev);
        return ret;
    }

    g_cv2005_i2c_fd[dev] = SENSOR_I2C_INVALID;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 cv2005_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data)
{
    xmedia_s32 ret;
    xmedia_u8  buf[CV2005_DATA_BYTE];

    if (g_cv2005_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_read(g_cv2005_i2c_fd[dev], g_cv2005_i2c_addr[dev], addr, CV2005_ADDR_BYTE, buf, CV2005_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] read i2c addr [0x%X] failed!\n", dev, addr);
        return ret;
    }

    *data = buf[0]; // CV2005_DATA_BYTE == 1

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 cv2005_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data)
{
    xmedia_u32 idx = 0;
    xmedia_s32 ret;
    xmedia_u8  buf[8];

    if (g_cv2005_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    buf[idx] = (addr >> 8) & 0xff;
    idx++;
    buf[idx] = addr & 0xff;
    idx++;
    buf[idx] = data & 0xff;

    ret = i2c_write(g_cv2005_i2c_fd[dev], g_cv2005_i2c_addr[dev], buf, CV2005_ADDR_BYTE + CV2005_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}

xmedia_s32 cv2005_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en)
{
    xmedia_s32 ret;
    xmedia_u32 mirror_flip_type;
    xmedia_u32 x, y;

    cv2005_read_reg(dev, CV2005_REG_ADDR_MIRROR_FLIP, &mirror_flip_type);

    if (mirror_en) {
        mirror_flip_type |= 0x01;
        x                 = CV2005_REG_VALUE_X_START + 1;
    } else {
        mirror_flip_type &= 0xfe;
        x                 = CV2005_REG_VALUE_X_START;
    }

    if (flip_en) {
        mirror_flip_type |= 0x02;
        y                 = CV2005_REG_VALUE_Y_START + 1;
    } else {
        mirror_flip_type &= 0xfd;
        y                 = CV2005_REG_VALUE_Y_START;
    }

    cv2005_write_reg(dev, CV2005_REG_ADDR_X_START, x);
    cv2005_write_reg(dev, CV2005_REG_ADDR_Y_START, y);
    ret = cv2005_write_reg(dev, CV2005_REG_ADDR_MIRROR_FLIP, mirror_flip_type);

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "cv2005_write_reg failed\n");
    }

    return XMEDIA_SUCCESS;
}
