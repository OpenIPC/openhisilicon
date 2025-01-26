/*
 * Copyright (c) GK.  All rights reserved.
 */

#include "sp2305_cmos.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#ifdef GPIO_I2C
#include "gpioi2c_ex.h"
#else
#ifdef __LITEOS__
#include "i2c.h"
#else
#include "i2c.h"
#endif

#endif

struct sp2305_i2c_msg {
    unsigned short  addr;      /* slave address */
    unsigned short  flags;
    unsigned short  len;      /* msg length */
    unsigned char  *buf;      /* pointer to msg data */
};

struct sp2305_i2c_rdwr_ioctl_data {
    struct sp2305_i2c_msg  *msgs; /* pointers to i2c_msgs */
    unsigned int nmsgs;    /* number of i2c_msgs */
};

struct i2c_read_s {
    unsigned int i2c_num;
    unsigned int dev_addr;
    unsigned int reg_addr;
    unsigned int reg_addr_end;
    unsigned int reg_width;
    unsigned int data_width;
    unsigned int reg_step;
};

#define I2C_M_RD 0x0001

static int g_fd[ISP_MAX_PIPE_NUM] = {[0 ... (ISP_MAX_PIPE_NUM - 1)] = -1};

#define SP2305_2M_1920X1080_10BIT_LINEAR30   0

/* VI 297M */
#define I2C_DEV_FILE_NUM     16

void sp2305_init_1080P30_10bit_linear(VI_PIPE ViPipe);
int sp2305_i2c_init(VI_PIPE ViPipe)
{
    char acDevFile[I2C_DEV_FILE_NUM] = {0};
    GK_U8 u8DevNum;

    if (g_fd[ViPipe] >= 0) {
        return GK_SUCCESS;
    }
#ifdef GPIO_I2C
    int ret;

    g_fd[ViPipe] = open("/dev/gpioi2c_ex", O_RDONLY, S_IRUSR);
    if (g_fd[ViPipe] < 0) {
        ISP_TRACE(MODULE_DBG_ERR, "Open gpioi2c_ex error!\n");
        return GK_FAILURE;
    }
#else
    int ret;
    ISP_SNS_COMMBUS_U *sp2305businfo = GK_NULL;

    sp2305businfo = sp2305_get_bus_Info(ViPipe);
    u8DevNum = sp2305businfo->s8I2cDev;
    ret = snprintf_s(acDevFile, sizeof(acDevFile), sizeof(acDevFile) - 1, "/dev/i2c-%u", u8DevNum);
    if (ret < 0) {
        return GK_FAILURE;
    }
    g_fd[ViPipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);

    if (g_fd[ViPipe] < 0) {
        ISP_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", u8DevNum);
        return GK_FAILURE;
    }

    ret = ioctl(g_fd[ViPipe], I2C_SLAVE_FORCE, (SP2305_I2C_ADDR >> 1));
    if (ret < 0) {
        ISP_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
        close(g_fd[ViPipe]);
        g_fd[ViPipe] = -1;
        return ret;
    }
#endif

    return GK_SUCCESS;
}

int sp2305_i2c_exit(VI_PIPE ViPipe)
{
    if (g_fd[ViPipe] >= 0) {
        close(g_fd[ViPipe]);
        g_fd[ViPipe] = -1;
        return GK_SUCCESS;
    }
    return GK_FAILURE;
}

#define  I2C_BUF_NUM   4
#define  I2C_MSG_BUF   2
int i2c_read(VI_PIPE ViPipe, struct i2c_read_s *i2c_read_data)
{
    int retval;
    unsigned char buf[I2C_BUF_NUM] = {0};
    unsigned int cur_addr;
    static struct sp2305_i2c_rdwr_ioctl_data rdwr;
    static struct sp2305_i2c_msg msg[I2C_MSG_BUF];
    unsigned int data = 0;

    msg[0].addr = (i2c_read_data->dev_addr >> 1);
    msg[0].flags = 0;
    msg[0].len = i2c_read_data->reg_width;
    msg[0].buf = buf;
    msg[1].addr = (i2c_read_data->dev_addr >> 1);
    msg[1].flags = 0;
    msg[1].flags |= I2C_M_RD;
    msg[1].len = i2c_read_data->data_width;
    msg[1].buf = buf;

    rdwr.msgs = &msg[0];
    rdwr.nmsgs = 2; /* 2 */
    for (cur_addr = i2c_read_data->reg_addr; cur_addr <= i2c_read_data->reg_addr_end;
        cur_addr += i2c_read_data->reg_step) {
        if (i2c_read_data->reg_width == 2) { /* 2 byte */
            buf[0] = (cur_addr >> 8) & 0xff; /* shift 8 */
            buf[1] = cur_addr & 0xff;
        } else {
            buf[0] = cur_addr & 0xff;
        }

        retval = ioctl(g_fd[ViPipe], I2C_RDWR, &rdwr);
        if (retval != 2) { /* 2 */
            printf("CMD_I2C_READ error!\n");
            retval = -1;
        }
        if (i2c_read_data->data_width == 2) { /* 2 byte */
            data = buf[1] | (buf[0] << 8); /* shift 8 */
        } else {
            data = buf[0];
        }
    }
    return data;
}


int sp2305_read_register(VI_PIPE ViPipe, GK_S32 addr)
{
    GK_U32 temp_data;
    struct i2c_read_s i2c_read_data;

    i2c_read_data.i2c_num = 0x00;
    i2c_read_data.dev_addr = 0x78;
    i2c_read_data.reg_addr = addr;
    i2c_read_data.reg_addr_end = addr;
    i2c_read_data.reg_width = 1;
    i2c_read_data.data_width = 1;
    i2c_read_data.reg_step = 1;

    temp_data = i2c_read(ViPipe, &i2c_read_data);

    return temp_data;
}

int sp2305_write_register(VI_PIPE ViPipe, GK_S32 addr, GK_S32 data)
{
    if (g_fd[ViPipe] < 0) {
        return GK_SUCCESS;
    }

#ifdef GPIO_I2C
    i2c_data.dev_addr = SP2305_I2C_ADDR;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = SP2305_ADDR_BYTE;
    i2c_data.data = data;
    i2c_data.data_byte_num = SP2305_DATA_BYTE;

    ret = ioctl(g_fd[ViPipe], GPIO_I2C_WRITE, &i2c_data);
    if (ret) {
        ISP_TRACE(MODULE_DBG_ERR, "GPIO-I2C write faild!\n");
        return ret;
    }
#else
    int idx = 0;
    int ret;
    char buf[8]; /* 8 */

    if (SP2305_ADDR_BYTE == 2) { /* 2 byte */
        buf[idx] = (addr >> 8) & 0xff; /* shift 8 */
        idx++;
        buf[idx] = addr & 0xff;
        idx++;
    } else {
        buf[idx] = addr & 0xff;
        idx++;
    }

    if (SP2305_DATA_BYTE == 2) { /* 2 byte */
        buf[idx] = (data >> 8) & 0xff; /* shift 8 */
        idx++;
        buf[idx] = data & 0xff;
        idx++;
    } else {
        buf[idx] = data & 0xff;
        idx++;
    }

    ret = write(g_fd[ViPipe], buf, (SP2305_ADDR_BYTE + SP2305_DATA_BYTE));
    if (ret < 0) {
        ISP_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
        return GK_FAILURE;
    }

#endif
    return GK_SUCCESS;
}

void sp2305_mirror_flip(VI_PIPE ViPipe, ISP_SNS_MIRRORFLIP_TYPE_E eSnsMirrorFlip)
{
    return;
}

static void delay_ms(int ms)
{
    usleep(ms * 1000); /* 1ms : 1000 us */
}

void sp2305_standby(VI_PIPE ViPipe)
{
    return;
}

void sp2305_restart(VI_PIPE ViPipe)
{
    return;
}

void sp2305_prog(VI_PIPE ViPipe, const int *rom)
{
    int i = 0;
    while (1) {
        unsigned int lookup = rom[i++];
        int addr = (lookup >> 16) & 0xFFFF; /* shift 16 */
        int data = lookup & 0xFFFF;
        if (addr == 0xFFFE) {
            delay_ms(data);
        } else if (addr == 0xFFFF) {
            return;
        } else {
            sp2305_write_register(ViPipe, addr, data);
        }
    }
    return;
}

void sp2305_default_reg_init(VI_PIPE ViPipe)
{
    GK_U32 i;
    GK_S32 ret = GK_SUCCESS;
    ISP_SNS_STATE_S *pastSp2305 = GK_NULL;

    pastSp2305 = sp2305_get_ctx(ViPipe);
    for (i = 0; i < pastSp2305->astRegsInfo[0].u32RegNum; i++) {
        ret += sp2305_write_register(ViPipe, pastSp2305->astRegsInfo[0].astI2cData[i].u32RegAddr,
            pastSp2305->astRegsInfo[0].astI2cData[i].u32Data);
    }

    if (ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "write register failed!\n");
    }
    return;
}

void sp2305_init(VI_PIPE ViPipe)
{
    GK_BOOL bInit;
    GK_U8 u8ImgMode;
    GK_S32 ret = GK_SUCCESS;
    ISP_SNS_STATE_S *pastSp2305 = GK_NULL;
    pastSp2305 = sp2305_get_ctx(ViPipe);
    bInit      = pastSp2305->bInit;
    u8ImgMode  = pastSp2305->u8ImgMode;

    /* i2c init */
    ret += sp2305_i2c_init(ViPipe);
    if (ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "i2c init failed!\n");
        return;
    }

    /* When sensor first init, config all registers */
    if (bInit == GK_FALSE) {
        switch (u8ImgMode) {
            case SP2305_2M_1920X1080_10BIT_LINEAR30:
                sp2305_init_1080P30_10bit_linear(ViPipe);
                break;

            default:
                ISP_TRACE(MODULE_DBG_ERR, "Not Support Image Mode %d\n", u8ImgMode);
                break;
        }
    } else {
        /* When sensor switch mode(linear<->WDR or resolution), config different registers(if possible) */
        switch (u8ImgMode) {
            case SP2305_2M_1920X1080_10BIT_LINEAR30:
                sp2305_init_1080P30_10bit_linear(ViPipe);
                break;

            default:
                ISP_TRACE(MODULE_DBG_ERR, "Not Support Image Mode %d\n", u8ImgMode);
                break;
        }
    }
    pastSp2305->bInit = GK_TRUE;

    return;
}

void sp2305_exit(VI_PIPE ViPipe)
{
    GK_S32  ret = GK_SUCCESS;
    ret += sp2305_i2c_exit(ViPipe);
    if (ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "i2c exit failed!\n");
    }
    return;
}

void sp2305_init_1080P30_10bit_linear(VI_PIPE ViPipe)
{
    GK_S32 ret = GK_SUCCESS;
    ret += sp2305_write_register(ViPipe, 0xfd, 0x00);
    ret += sp2305_write_register(ViPipe, 0x2f, 0x10);
    ret += sp2305_write_register(ViPipe, 0x34, 0x00);
    ret += sp2305_write_register(ViPipe, 0x30, 0x15); /*  change NCP&pll_pcp control mode, pll_clk/6  */
    ret += sp2305_write_register(ViPipe, 0x33, 0x01);
    ret += sp2305_write_register(ViPipe, 0x35, 0x20);
    ret += sp2305_write_register(ViPipe, 0xfd, 0x01);
    ret += sp2305_write_register(ViPipe, 0x0d, 0x00);
    ret += sp2305_write_register(ViPipe, 0x30, 0x00);
    ret += sp2305_write_register(ViPipe, 0x03, 0x04);
    ret += sp2305_write_register(ViPipe, 0x04, 0x48);
    ret += sp2305_write_register(ViPipe, 0x09, 0x00);
    ret += sp2305_write_register(ViPipe, 0x0a, 0x80);
    ret += sp2305_write_register(ViPipe, 0x06, 0x0a);
    ret += sp2305_write_register(ViPipe, 0x24, 0x40);
    ret += sp2305_write_register(ViPipe, 0x01, 0x01);
    ret += sp2305_write_register(ViPipe, 0xfb, 0x73);
    ret += sp2305_write_register(ViPipe, 0xfd, 0x01);
    ret += sp2305_write_register(ViPipe, 0x1a, 0x6b);
    ret += sp2305_write_register(ViPipe, 0x1c, 0xea);
    ret += sp2305_write_register(ViPipe, 0x16, 0x0c);
    ret += sp2305_write_register(ViPipe, 0x21, 0x00);
    ret += sp2305_write_register(ViPipe, 0x11, 0x63);
    ret += sp2305_write_register(ViPipe, 0x19, 0xc3);
    ret += sp2305_write_register(ViPipe, 0x29, 0x01);
    ret += sp2305_write_register(ViPipe, 0x33, 0x6f);
    ret += sp2305_write_register(ViPipe, 0x2a, 0xea);
    ret += sp2305_write_register(ViPipe, 0x2c, 0x40);
    ret += sp2305_write_register(ViPipe, 0xd0, 0x02);
    ret += sp2305_write_register(ViPipe, 0xd1, 0x01);
    ret += sp2305_write_register(ViPipe, 0xd2, 0x20);
    ret += sp2305_write_register(ViPipe, 0xd3, 0x04);
    ret += sp2305_write_register(ViPipe, 0xd4, 0x2a);
    ret += sp2305_write_register(ViPipe, 0x50, 0x00);
    ret += sp2305_write_register(ViPipe, 0x51, 0x2c);
    ret += sp2305_write_register(ViPipe, 0x52, 0x29);
    ret += sp2305_write_register(ViPipe, 0x53, 0x00);
    ret += sp2305_write_register(ViPipe, 0x55, 0x44);
    ret += sp2305_write_register(ViPipe, 0x58, 0x29);
    ret += sp2305_write_register(ViPipe, 0x5a, 0x00);
    ret += sp2305_write_register(ViPipe, 0x5b, 0x00);
    ret += sp2305_write_register(ViPipe, 0x5d, 0x00);
    ret += sp2305_write_register(ViPipe, 0x64, 0x2f);
    ret += sp2305_write_register(ViPipe, 0x66, 0x62);
    ret += sp2305_write_register(ViPipe, 0x68, 0x5b);
    ret += sp2305_write_register(ViPipe, 0x75, 0x46);
    ret += sp2305_write_register(ViPipe, 0x76, 0x36);
    ret += sp2305_write_register(ViPipe, 0x77, 0x4f);
    ret += sp2305_write_register(ViPipe, 0x78, 0xef);
    ret += sp2305_write_register(ViPipe, 0x72, 0xcf);
    ret += sp2305_write_register(ViPipe, 0x73, 0x36);
    ret += sp2305_write_register(ViPipe, 0x7d, 0x0d);
    ret += sp2305_write_register(ViPipe, 0x7e, 0x0d);
    ret += sp2305_write_register(ViPipe, 0x8a, 0x77);
    ret += sp2305_write_register(ViPipe, 0x8b, 0x77);
    ret += sp2305_write_register(ViPipe, 0xfd, 0x01);
    ret += sp2305_write_register(ViPipe, 0xb1, 0x83); /*  DPHY enable  */
    ret += sp2305_write_register(ViPipe, 0xb3, 0x0b); /*  0b */
    ret += sp2305_write_register(ViPipe, 0xb4, 0x14); /*  MIPI PLL enable;14 */
    ret += sp2305_write_register(ViPipe, 0x9d, 0x40);
    ret += sp2305_write_register(ViPipe, 0xa1, 0x03); /*  speed */
    ret += sp2305_write_register(ViPipe, 0xb5, 0x50);
    ret += sp2305_write_register(ViPipe, 0xa0, 0x01); /*  mipi enable */
    ret += sp2305_write_register(ViPipe, 0x25, 0xe0);
    ret += sp2305_write_register(ViPipe, 0x20, 0x7b);
    ret += sp2305_write_register(ViPipe, 0xfd, 0x01);
    ret += sp2305_write_register(ViPipe, 0xf0, 0x40);
    ret += sp2305_write_register(ViPipe, 0xf1, 0x40);
    ret += sp2305_write_register(ViPipe, 0xf2, 0x40);
    ret += sp2305_write_register(ViPipe, 0xf3, 0x40);
    ret += sp2305_write_register(ViPipe, 0xfd, 0x02); /*  crop to 1920x1080 */
    ret += sp2305_write_register(ViPipe, 0xa0, 0x00); /*  Image vertical start MSB3bits */
    ret += sp2305_write_register(ViPipe, 0xa1, 0x08); /*  Image vertical start LSB8bits */
    ret += sp2305_write_register(ViPipe, 0xa2, 0x04); /*  image vertical size_value  MSB8bits */
    ret += sp2305_write_register(ViPipe, 0xa3, 0x38); /*  image vertical size_value  LSB8bits */
    ret += sp2305_write_register(ViPipe, 0xa4, 0x00);
    ret += sp2305_write_register(ViPipe, 0xa5, 0x08); /*  H start 8Lsb */
    ret += sp2305_write_register(ViPipe, 0xa6, 0x03);
    ret += sp2305_write_register(ViPipe, 0xa7, 0xc0); /*  Half H size_value Lsb8bits */
    ret += sp2305_write_register(ViPipe, 0xfd, 0x01);
    ret += sp2305_write_register(ViPipe, 0x8e, 0x07);
    ret += sp2305_write_register(ViPipe, 0x8f, 0x80); /*  MIPI column number */
    ret += sp2305_write_register(ViPipe, 0x90, 0x04); /*  MIPI row number */
    ret += sp2305_write_register(ViPipe, 0x91, 0x38);
    ret += sp2305_write_register(ViPipe, 0xfd, 0x01);
    ret += sp2305_write_register(ViPipe, 0x05, 0x00);
    ret += sp2305_write_register(ViPipe, 0x06, 0x70); /*  change min VTS, Vblank, VTS:0x4c1, 30.037fps */
    ret += sp2305_write_register(ViPipe, 0x01, 0x01);
    ret += sp2305_write_register(ViPipe, 0xfd, 0x00); /* pll_clk 168M, DAC 84M, pclk_pre 84M timer_clk 42M(Pclk=84M) */
    ret += sp2305_write_register(ViPipe, 0x2f, 0x10);
    ret += sp2305_write_register(ViPipe, 0x30, 0x15);
    ret += sp2305_write_register(ViPipe, 0x33, 0x01);
    ret += sp2305_write_register(ViPipe, 0x34, 0x00);
    ret += sp2305_write_register(ViPipe, 0xfd, 0x01); /*  MIPI , bclk:105M, mipi_bclk_phy 52.5M */
    ret += sp2305_write_register(ViPipe, 0xb2, 0x40);
    ret += sp2305_write_register(ViPipe, 0xb3, 0x0b);
    ret += sp2305_write_register(ViPipe, 0xb4, 0x14);
    ret += sp2305_write_register(ViPipe, 0x97, 0x2b);
    if (ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "write register failed!\n");
        return;
    }
    printf("====================================================================\n");
    printf("==Ominivision SP2305 sensor HCG 1080P30fps(MIPI port) init success!==\n");
    printf("====================================================================\n");
    return;

}
