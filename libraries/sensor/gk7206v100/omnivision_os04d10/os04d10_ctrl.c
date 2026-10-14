#include <stdio.h>
#include <string.h>
#include "i2c_dev.h"
#include "os04d10.h"
#include "os04d10_ctrl.h"

#define OS04D10_REG_ADDR_MIRROR_FLIP 0x32
static xmedia_s32 g_os04d10_i2c_fd[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] =
                                                                     SENSOR_I2C_INVALID };
static xmedia_s32 g_os04d10_i2c_addr[XMEDIA_SENSOR_DEV_MAX_NUM] = { [0 ...(XMEDIA_SENSOR_DEV_MAX_NUM - 1)] =
                                                                     SENSOR_I2C_INVALID };

/*
 * 函数功能: os04d10 MIPI 2lane 1440p 10bit线性模式初始化序列 - 2560x1440x30fps
 * 函数参数：
 *      dev -  sensor 设备号
 * 返回值：
 *
 */

static xmedia_s32 os04d10_2lane_linear_2560x1440_init(xmedia_u32 dev)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    ret |= os04d10_write_reg(dev, 0xfd, 0x00);
    ret |= os04d10_write_reg(dev, 0x20, 0x00);
    ret |= os04d10_write_reg(dev, 0x20, 0x01);
    ret |= os04d10_write_reg(dev, 0x20, 0x01);
    ret |= os04d10_write_reg(dev, 0x20, 0x01);
    ret |= os04d10_write_reg(dev, 0x20, 0x01);
    ret |= os04d10_write_reg(dev, 0x30, 0x00);
    ret |= os04d10_write_reg(dev, 0x2e, 0x7e);
    ret |= os04d10_write_reg(dev, 0x31, 0x20);  //  row clk from dpll
    ret |= os04d10_write_reg(dev, 0x38, 0x15);  //  enable clk
    ret |= os04d10_write_reg(dev, 0x45, 0x34);
    ret |= os04d10_write_reg(dev, 0xfd, 0x01);
    ret |= os04d10_write_reg(dev, 0x03, 0x00);  //  exp 8msb
    ret |= os04d10_write_reg(dev, 0x04, 0x04);  // exp 8lsb

    ret |= os04d10_write_reg(dev, 0x05, 0x01);
    ret |= os04d10_write_reg(dev, 0x06, 0x29);  // vb 8lsb
    ret |= os04d10_write_reg(dev, 0x24, 0xff);  // analog gain
    ret |= os04d10_write_reg(dev, 0x42, 0x59);

    ret |= os04d10_write_reg(dev, 0x45, 0x02);  // scg en
    ret |= os04d10_write_reg(dev, 0x48, 0x0c);  // vb_psv_exp_en,vb_psv_fl_en
    ret |= os04d10_write_reg(dev, 0x4b, 0x88);  // dummy row1,dummy row2
    ret |= os04d10_write_reg(dev, 0xd4, 0x05);  // ulp_fl_limit[15:8]
    ret |= os04d10_write_reg(dev, 0xd5, 0xd2);  // ulp_fl_limit[7:0], 0xulp_fl=1490
    ret |= os04d10_write_reg(dev, 0xd7, 0x05);  // ulp_exp_limit[15:8]
    ret |= os04d10_write_reg(dev, 0xd8, 0xd2);  // ulp_exp_limit[7:0], 0xulp_exp=1490
    ret |= os04d10_write_reg(dev, 0x50, 0x01);  // p50
    ret |= os04d10_write_reg(dev, 0x51, 0x11);  // p51
    ret |= os04d10_write_reg(dev, 0x52, 0x18);  // p52
    ret |= os04d10_write_reg(dev, 0x53, 0x01);  // p53
    ret |= os04d10_write_reg(dev, 0x54, 0x01);  // p54
    ret |= os04d10_write_reg(dev, 0x55, 0x01);  // p55
    ret |= os04d10_write_reg(dev, 0x57, 0x08);  // p57
    ret |= os04d10_write_reg(dev, 0x5c, 0x40);  // p5c
    ret |= os04d10_write_reg(dev, 0x7c, 0x04);  // p7c
    ret |= os04d10_write_reg(dev, 0x90, 0x60);  // p90
    ret |= os04d10_write_reg(dev, 0x91, 0x0f);  // p91
    ret |= os04d10_write_reg(dev, 0x92, 0x24);  // p92
    ret |= os04d10_write_reg(dev, 0x93, 0x3a);  // p93
    ret |= os04d10_write_reg(dev, 0x94, 0x0f);  // p94
    ret |= os04d10_write_reg(dev, 0x95, 0x63);  // p95
    ret |= os04d10_write_reg(dev, 0x98, 0x5d);  // p98
    ret |= os04d10_write_reg(dev, 0xa8, 0x50);  // pa8
    ret |= os04d10_write_reg(dev, 0xaa, 0x14);  // paa
    ret |= os04d10_write_reg(dev, 0xab, 0x05);  // pab
    ret |= os04d10_write_reg(dev, 0xac, 0x14);  // pac
    ret |= os04d10_write_reg(dev, 0xad, 0x05);  // pad
    ret |= os04d10_write_reg(dev, 0xae, 0x47);  // pae
    ret |= os04d10_write_reg(dev, 0xaf, 0x10);  // paf
    ret |= os04d10_write_reg(dev, 0xc9, 0x28);  // cap_s_pd_rst_en_hcg,cap_r_fd_rst_en_hcg
    ret |= os04d10_write_reg(dev, 0xca, 0x5e);  // p86_x1 sc1
    ret |= os04d10_write_reg(dev, 0xcb, 0x5e);  // p86_x2 sc1
    ret |= os04d10_write_reg(dev, 0xcc, 0x5e);  // p86_x3 sc1
    ret |= os04d10_write_reg(dev, 0xcd, 0x5e);  // p86_x4 sc1
    ret |= os04d10_write_reg(dev, 0xce, 0x5c);  // p88_x1 sc2
    ret |= os04d10_write_reg(dev, 0xcf, 0x5c);  // p88_x2 sc2
    ret |= os04d10_write_reg(dev, 0xd0, 0x5c);  // p88_x3 sc2
    ret |= os04d10_write_reg(dev, 0xd1, 0x5c);  // p88_x4 sc2
    ret |= os04d10_write_reg(dev, 0xd2, 0x7c);  // col cap10
    ret |= os04d10_write_reg(dev, 0xd3, 0x7c);  // col cap32
    ret |= os04d10_write_reg(dev, 0xdb, 0x2f);  // bitline bias, 0xcomp1 bias
    // split
    ret |= os04d10_write_reg(dev, 0xfd, 0x01);
    ret |= os04d10_write_reg(dev, 0x46, 0x77);
    ret |= os04d10_write_reg(dev, 0xdd, 0x00);
    ret |= os04d10_write_reg(dev, 0xde, 0x3f);
    ret |= os04d10_write_reg(dev, 0xfd, 0x03);
    ret |= os04d10_write_reg(dev, 0x2b, 0x0a);
    ret |= os04d10_write_reg(dev, 0x01, 0x22);
    ret |= os04d10_write_reg(dev, 0x02, 0x03);
    ret |= os04d10_write_reg(dev, 0x00, 0x06);
    ret |= os04d10_write_reg(dev, 0x2a, 0x22);
    ret |= os04d10_write_reg(dev, 0x29, 0x0b);
    ret |= os04d10_write_reg(dev, 0x1e, 0x10);
    ret |= os04d10_write_reg(dev, 0x1f, 0x02);
    ret |= os04d10_write_reg(dev, 0x1a, 0x24);
    ret |= os04d10_write_reg(dev, 0x1b, 0x62);
    ret |= os04d10_write_reg(dev, 0x1c, 0xce);
    ret |= os04d10_write_reg(dev, 0x1d, 0xd3);
    ret |= os04d10_write_reg(dev, 0x04, 0x0f);
    ret |= os04d10_write_reg(dev, 0x36, 0x00);
    ret |= os04d10_write_reg(dev, 0x37, 0x05);
    ret |= os04d10_write_reg(dev, 0x38, 0x09);
    ret |= os04d10_write_reg(dev, 0x39, 0x19);
    ret |= os04d10_write_reg(dev, 0x3a, 0x38);
    ret |= os04d10_write_reg(dev, 0x3b, 0x22);
    ret |= os04d10_write_reg(dev, 0x3c, 0x22);
    ret |= os04d10_write_reg(dev, 0x3d, 0x22);
    ret |= os04d10_write_reg(dev, 0x3e, 0x03);
    // split
    ret |= os04d10_write_reg(dev, 0xfd, 0x02);
    ret |= os04d10_write_reg(dev, 0xce, 0x65);  // frame end dly
    ret |= os04d10_write_reg(dev, 0xfd, 0x03);
    ret |= os04d10_write_reg(dev, 0x03, 0x30);  // vcap bias
    ret |= os04d10_write_reg(dev, 0x05, 0x00);  // adc range 551.6mv
    ret |= os04d10_write_reg(dev, 0x12, 0x20);  // rcnt_num[7:0]
    ret |= os04d10_write_reg(dev, 0x13, 0x40);  // scnt_num[7:0]
    ret |= os04d10_write_reg(dev, 0x21, 0xca);  // vref_bsun_rst_hcg
    ret |= os04d10_write_reg(dev, 0x27, 0x85);  // bsun_rst_dn_en,bsun_sig_dn_en
    ret |= os04d10_write_reg(dev, 0x2c, 0x55);  // vn1=vn2=-1.3v
    ret |= os04d10_write_reg(dev, 0x2d, 0x08);  // vh=3.4v
    ret |= os04d10_write_reg(dev, 0x2e, 0xca);  // vref_bsun_rst_lcg
    ret |= os04d10_write_reg(dev, 0x3f, 0xe7);  // vcap=2.2v
    // digital crop
    ret |= os04d10_write_reg(dev, 0xfd, 0x02);
    ret |= os04d10_write_reg(dev, 0x5e, 0x22);  // digital crop enable
    ret |= os04d10_write_reg(dev, 0xfd, 0x02);
    ret |= os04d10_write_reg(dev, 0xa0, 0x00);
    ret |= os04d10_write_reg(dev, 0xa1, 0x04);  // row_start
    ret |= os04d10_write_reg(dev, 0xa2, 0x05);
    ret |= os04d10_write_reg(dev, 0xa3, 0xa2);  // row_size
    ret |= os04d10_write_reg(dev, 0xa4, 0x00);
    ret |= os04d10_write_reg(dev, 0xa5, 0x04);  // col_start
    ret |= os04d10_write_reg(dev, 0xa6, 0x0a);
    ret |= os04d10_write_reg(dev, 0xa7, 0x00);  // col_size
    ret |= os04d10_write_reg(dev, 0x8e, 0x0a);
    ret |= os04d10_write_reg(dev, 0x8f, 0x00);  // mipi col_size
    ret |= os04d10_write_reg(dev, 0x90, 0x05);
    ret |= os04d10_write_reg(dev, 0x91, 0xb0);  // mipi row_size
    // digital crop
    ret |= os04d10_write_reg(dev, 0xfd, 0x00);
    ret |= os04d10_write_reg(dev, 0x8b, 0x01);  // mipi p2s rst=0
    ret |= os04d10_write_reg(dev, 0x8d, 0x00);  // mipi pwd sel=0
    ret |= os04d10_write_reg(dev, 0xfd, 0x01);
    ret |= os04d10_write_reg(dev, 0x01, 0x02);
    ret |= os04d10_write_reg(dev, 0xfd, 0x05);
    ret |= os04d10_write_reg(dev, 0xc4, 0x62);
    ret |= os04d10_write_reg(dev, 0xc5, 0x62);
    ret |= os04d10_write_reg(dev, 0xc6, 0x62);
    ret |= os04d10_write_reg(dev, 0xc7, 0x62);
    ret |= os04d10_write_reg(dev, 0xf0, 0x40);  // blc Gb offset
    ret |= os04d10_write_reg(dev, 0xf1, 0x40);  // blc B offset
    ret |= os04d10_write_reg(dev, 0xf2, 0x40);  // blc R offset
    ret |= os04d10_write_reg(dev, 0xf3, 0x40);  // blc Gr offset
    ret |= os04d10_write_reg(dev, 0xf4, 0x00);  // random gain limit
    ret |= os04d10_write_reg(dev, 0xf9, 0x03);
    ret |= os04d10_write_reg(dev, 0xfa, 0x5d);  // blc trig en
    ret |= os04d10_write_reg(dev, 0xfb, 0x6b);  // blc en, 0xblc_bpc_en, 0xblc_filter_en, 0xrandom_en
    ret |= os04d10_write_reg(dev, 0xb1, 0x01);  // mipi en
    ret |= os04d10_write_reg(dev, 0xfd, 0x00);
    ret |= os04d10_write_reg(dev, 0x20, 0x03);  // logic start

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os04d10_write_reg failed\n");
        return XMEDIA_FAILURE;
    }

    SENSOR_PRINT("===============================================================================\n");
    SENSOR_PRINT("==== OS04D10_24MInput_MIPI_2lane_10bit_linear_2560x1440_30fps init success!====\n");
    SENSOR_PRINT("===============================================================================\n");
    return XMEDIA_SUCCESS;
}

xmedia_s32 os04d10_init_image(xmedia_u32 dev, xmedia_u8 image_mode)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    switch(image_mode) {
        case OS04D10_4M_30FPS_10BIT_LINEAR_MODE:
            ret = os04d10_2lane_linear_2560x1440_init(dev);
            break;

        default:
            SENSOR_TRACE(MODULE_DBG_ERR, "Not support, please check image mode!\n");
            return XMEDIA_ERRCODE_NOT_SUPPORT;
    }

    return ret;
}

SENSOR_PRIORITY_FUNC xmedia_s32 os04d10_set_slave_addr(xmedia_u32 dev, xmedia_u32 slave_addr)
{
    xmedia_s32  ret;

    if (g_os04d10_i2c_fd[dev] < 0) {
        SENSOR_PRINT("Sensor dev [%d], i2c not open. please init i2c at first.\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_set_slave_addr(g_os04d10_i2c_fd[dev], slave_addr);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        i2c_close(g_os04d10_i2c_fd[dev]);
        g_os04d10_i2c_fd[dev] = SENSOR_I2C_INVALID;
        return XMEDIA_FAILURE;
    }

    g_os04d10_i2c_addr[dev] = slave_addr;
    SENSOR_PRINT("Sensor dev [%d] set slave addr 0x%x success.\n", dev, slave_addr);

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 os04d10_i2c_init(xmedia_u32 dev, xmedia_s8 i2c_dev, xmedia_u32 i2c_addr)
{
    xmedia_s32 ret;

    if (g_os04d10_i2c_fd[dev] >= 0) {
        SENSOR_PRINT("====== Sensor dev [%d], i2c already init !=======\n", dev);
        return XMEDIA_SUCCESS;
    }

    if (i2c_dev == SENSOR_I2C_INVALID) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Sensor i2c dev invalid! Please set i2c dev at first!\n");
        return XMEDIA_ERRCODE_NOT_CONFIG;
    }

    g_os04d10_i2c_fd[dev] = i2c_open(i2c_dev);
    if (g_os04d10_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", i2c_dev);
        return XMEDIA_ERRCODE_OPEN_FAILED;
    }

    ret = os04d10_set_slave_addr(dev, i2c_addr);
    SENSOR_CHECK_RET_RETURN(ret);
    SENSOR_PRINT("====== i2c[%d] init success!=======\n", i2c_dev);
    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 os04d10_i2c_exit(xmedia_u32 dev)
{
    xmedia_s32 ret;

    ret = i2c_close(g_os04d10_i2c_fd[dev]);
    if(ret < 0 ) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] close i2c failed!\n", dev);
        return ret;
    }

    g_os04d10_i2c_fd[dev] = SENSOR_I2C_INVALID;

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 os04d10_read_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 *data)
{
    xmedia_s32 ret;
    xmedia_u8  buf[OS04D10_DATA_BYTE];

    if (g_os04d10_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    ret = i2c_read(g_os04d10_i2c_fd[dev], g_os04d10_i2c_addr[dev], addr, OS04D10_ADDR_BYTE, buf, OS04D10_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d] read i2c addr [0x%X] failed!\n", dev, addr);
        return ret;
    }

    *data = buf[0]; //OS04D10_DATA_BYTE == 1

    return XMEDIA_SUCCESS;
}

SENSOR_PRIORITY_FUNC xmedia_s32 os04d10_write_reg(xmedia_u32 dev, xmedia_u32 addr, xmedia_u32 data)
{
    xmedia_u32 idx = 0;
    xmedia_s32 ret;
    xmedia_u8  buf[8];

    if (g_os04d10_i2c_fd[dev] < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "sensor dev[%d]'s i2c is not init!\n", dev);
        return XMEDIA_ERRCODE_NOT_INIT;
    }

    buf[idx] = addr & 0xff;
    idx++;
    buf[idx] = data & 0xff;

    ret = i2c_write(g_os04d10_i2c_fd[dev], g_os04d10_i2c_addr[dev], buf, OS04D10_ADDR_BYTE + OS04D10_DATA_BYTE);
    if (ret < 0) {
        SENSOR_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}

xmedia_s32 os04d10_set_mirror_flip(xmedia_u32 dev, xmedia_bool mirror_en, xmedia_bool flip_en)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    if (mirror_en && flip_en) {
        ret  = os04d10_write_reg(dev, 0xfd, 0x01);
        ret |= os04d10_write_reg(dev, 0x32, 0x03);
        ret |= os04d10_write_reg(dev, 0x01, 0x01);
        ret |= os04d10_write_reg(dev, 0xfd, 0x02);
        ret |= os04d10_write_reg(dev, 0xa1, 0x03);
        ret |= os04d10_write_reg(dev, 0xa5, 0x03);
        ret |= os04d10_write_reg(dev, 0xfd, 0x01);
    } else if (mirror_en && (!flip_en)) {
        ret  = os04d10_write_reg(dev, 0xfd, 0x01);
        ret |= os04d10_write_reg(dev, 0x32, 0x01);
        ret |= os04d10_write_reg(dev, 0x01, 0x01);
        ret |= os04d10_write_reg(dev, 0xfd, 0x02);
        ret |= os04d10_write_reg(dev, 0xa1, 0x04);
        ret |= os04d10_write_reg(dev, 0xa5, 0x03);
        ret |= os04d10_write_reg(dev, 0xfd, 0x01);
    } else if ((!mirror_en) && flip_en) {
        ret  = os04d10_write_reg(dev, 0xfd, 0x01);
        ret |= os04d10_write_reg(dev, 0x32, 0x02);
        ret |= os04d10_write_reg(dev, 0x01, 0x01);
        ret |= os04d10_write_reg(dev, 0xfd, 0x02);
        ret |= os04d10_write_reg(dev, 0xa1, 0x03);
        ret |= os04d10_write_reg(dev, 0xa5, 0x04);
        ret |= os04d10_write_reg(dev, 0xfd, 0x01);
    } else if ((!mirror_en) && (!flip_en)) {
        ret  = os04d10_write_reg(dev, 0xfd, 0x01);
        ret |= os04d10_write_reg(dev, 0x32, 0x00);
        ret |= os04d10_write_reg(dev, 0x01, 0x01);
        ret |= os04d10_write_reg(dev, 0xfd, 0x02);
        ret |= os04d10_write_reg(dev, 0xa1, 0x04);
        ret |= os04d10_write_reg(dev, 0xa5, 0x04);
        ret |= os04d10_write_reg(dev, 0xfd, 0x01);
    }

    if (ret != XMEDIA_SUCCESS) {
        SENSOR_TRACE(MODULE_DBG_ERR, "os04d10_write_reg failed\n");
    }

    return XMEDIA_SUCCESS;
}
