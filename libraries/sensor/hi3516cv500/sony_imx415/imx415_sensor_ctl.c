/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of imx415_sensor_ctl.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "imx415_cmos.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/mman.h>
#include <memory.h>

#ifdef HI_GPIO_I2C
#include "gpioi2c_ex.h"
#else
#include "hi_i2c.h"

#endif

static int g_fd[ISP_MAX_PIPE_NUM] = {[0 ... (ISP_MAX_PIPE_NUM - 1)] = -1};
static HI_BOOL g_bStandby[ISP_MAX_PIPE_NUM] = {0};

#ifndef NA
#define NA 0xFFFF
#endif
#define I2C_DEV_FILE_NUM     16
#define I2C_BUF_NUM          8
int imx415_i2c_init(VI_PIPE vi_pipe)
{
    int ret;
    HI_U8 u8DevNum;
    char acDevFile[I2C_DEV_FILE_NUM];

    if (g_fd[vi_pipe] >= 0) {
        return HI_SUCCESS;
    }
    ISP_SNS_COMMBUS_U *imx415businfo = HI_NULL;
    imx415businfo = imx415_get_bus_Info(vi_pipe);
    u8DevNum = imx415businfo->s8I2cDev;

    snprintf_s(acDevFile, sizeof(acDevFile), sizeof(acDevFile) - 1, "/dev/i2c-%u", u8DevNum);

    g_fd[vi_pipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);
    if (g_fd[vi_pipe] < 0) {
        SNS_ERR_TRACE("Open /dev/hi_i2c_drv-%u error!\n", u8DevNum);
        return HI_FAILURE;
    }

    ret = ioctl(g_fd[vi_pipe], I2C_SLAVE_FORCE, (IMX415_I2C_ADDR >> 1));
    if (ret < 0) {
        SNS_ERR_TRACE("I2C_SLAVE_FORCE error!\n");
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return ret;
    }

    return HI_SUCCESS;
}

int imx415_i2c_exit(VI_PIPE vi_pipe)
{
    if (g_fd[vi_pipe] >= 0) {
        close(g_fd[vi_pipe]);
        g_fd[vi_pipe] = -1;
        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

int imx415_read_register(VI_PIPE vi_pipe, HI_U32 addr)
{
    return HI_SUCCESS;
}
int imx415_write_register(VI_PIPE vi_pipe, HI_U32 addr, HI_U32 data)
{
    HI_U32 idx = 0;
    HI_S32 ret;
    HI_U8 buf[I2C_BUF_NUM];

    if (g_fd[vi_pipe] < 0) {
        return HI_SUCCESS;
    }

    if (IMX415_ADDR_BYTE == 2) {  /* 2 byte */
        buf[idx] = (addr >> 8) & 0xff;  /* shift 8 */
        idx++;
        buf[idx] = addr & 0xff;
        idx++;
    } else {
        buf[idx] = addr & 0xff;
        idx++;
    }

    if (IMX415_DATA_BYTE == 2) {  /* 2 byte */
        buf[idx] = (data >> 8) & 0xff;  /* shift 8 */
        idx++;
        buf[idx] = data & 0xff;
        idx++;
    } else {
        buf[idx] = data & 0xff;
        idx++;
    }

    ret = write(g_fd[vi_pipe], buf, (IMX415_ADDR_BYTE + IMX415_DATA_BYTE));
    if (ret < 0) {
        SNS_ERR_TRACE("I2C_WRITE DATA error!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

void imx415_standby(VI_PIPE vi_pipe)
{
    HI_S32 ret;
    ret = imx415_write_register(vi_pipe, 0x3000, 0x00);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("write register failed!\n");
        return;
    }
    g_bStandby[vi_pipe] = HI_TRUE;
    return;
}

void imx415_restart(VI_PIPE vi_pipe)
{
    HI_S32 ret;
    ret = imx415_write_register(vi_pipe, 0x3000, 0x01);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("write register failed!\n");
        return;
    }
    g_bStandby[vi_pipe] = HI_FALSE;
    return;
}

static void delay_ms(int ms)
{
    usleep(ms * 1000);  /* 1ms: 1000us */
}

void imx415_mirror_flip(VI_PIPE vi_pipe, ISP_SNS_MIRRORFLIP_TYPE_E eSnsMirrorFlip)
{
    switch (eSnsMirrorFlip) {
        default:
        case ISP_SNS_NORMAL:
            imx415_write_register(vi_pipe, 0x3030, 0x0);
            break;

        case ISP_SNS_MIRROR:
            imx415_write_register(vi_pipe, 0x3030, 0x01);
            break;

        case ISP_SNS_FLIP:
            imx415_write_register(vi_pipe, 0x3030, 0x02);
            break;

        case ISP_SNS_MIRROR_FLIP:
            imx415_write_register(vi_pipe, 0x3030, 0x03);
            break;
    }

    return;
}

/* Mode columns:
 *   0 = 4K@30   all-pixel,        891 Mbps (SYS_MODE=5)
 *   1 = 4K@20   all-pixel,        594 Mbps (SYS_MODE=7)
 *   2 = 1080p@60 binning,         891 Mbps (SYS_MODE=5 + ADDMODE=1)
 *   3 = 4K@60   all-pixel,       1485 Mbps (SYS_MODE=8)  *broken on av300*
 *   4 = Window-crop flex,         891 Mbps (SYS_MODE=5 + WINMODE=4)
 *   5 = Binning + Window-crop,    891 Mbps (SYS_MODE=5 + ADDMODE=1 + WINMODE=4)
 *                                 1H = 7.26 µs (binning) — fps_max well above the
 *                                 lane-rate-limited Mode 4 ceiling, capped by FE
 *                                 finalisation at ~60 fps.
 * Last column is the I2C register address. NA = skip (use sensor default).
 *
 * Mode 4 (window crop) inherits Mode 0's SYS_MODE/INCKSEL/D-PHY timing.
 * Mode 5 (binning + crop) inherits Mode 2's binning bits PLUS Mode 4's crop bits.
 *
 * Driver does not write ADBIT/MDBIT (defaults = 12-bit), so all modes transmit
 * RAW12 on MIPI regardless of the mode-name suffix. */
static const HI_U16 g_au16SensorCfgSeq[][IMX415_MODE_BUTT + 1] = {
    /* 0     1     2     3     4     5   addr */
    {0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x3008},
    {0x5B, 0x5B, 0x5B, 0x5B, 0x5B, 0x5B, 0x300A},
    {  NA,   NA, 0x01,   NA,   NA, 0x01, 0X3020},  /* Mode 5 binning bit */
    {  NA,   NA, 0x01,   NA,   NA, 0x01, 0X3021},
    {  NA,   NA, 0x01,   NA,   NA, 0x01, 0X3022},  /* ADDMODE */
    {  NA,   NA, 0xF8,   NA,   NA, 0xF8, 0X3024},
    {0x4C, 0x72, 0x1B, 0x26, 0x4C, 0x1B, 0x3028},  /* HMAX low — Mode 5 = Mode 2 (binning) */
    {0x04, 0x06,   NA, 0x02, 0x04,   NA, 0x3029},
    {  NA,   NA, 0x00,   NA,   NA, 0x00, 0X3031},  /* ADBIT — 10-bit analog for binning modes */
    {  NA,   NA,   NA,   NA,   NA, 0x00, 0X3032},  /* MDBIT — 10-bit MIPI for Mode 5 (saves ~20%
                                                    * bandwidth vs default 12-bit padded MIPI) */
    {0x05, 0x07, 0x05, 0x08, 0x05, 0x05, 0x3033},
    {0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3050},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30C1},
    {  NA,   NA, 0x02,   NA,   NA, 0x02, 0X30D9},
    {  NA,   NA, 0x01,   NA,   NA, 0x01, 0X30DA},
    {0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x3116},
    {0xC0, 0x80,   NA, 0xA0, 0xC0,   NA, 0x3118},
    {  NA,   NA,   NA, 0xE0,   NA,   NA, 0x311A},
    {0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x311E},
    {0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x32D4},
    {0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1, 0x32EC},
    {0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x3452},
    {0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x3453},
    {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x358A},
    {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x35A1},
    {0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x36BC},
    {0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x36CC},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36CD},
    {0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x36CE},
    {0x8C, 0x8C, 0x8C, 0x8C, 0x8C, 0x8C, 0x36D0},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36D1},
    {0x71, 0x71, 0x71, 0x71, 0x71, 0x71, 0x36D2},
    {0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x36D4},
    {0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x36D6},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36D7},
    {0x71, 0x71, 0x71, 0x71, 0x71, 0x71, 0x36D8},
    {0x8C, 0x8C, 0x8C, 0x8C, 0x8C, 0x8C, 0x36DA},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36DB},
    {  NA,   NA, 0x00,   NA,   NA,   NA, 0X3701},
    {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x3724},
    {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x3726},
    {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x3732},
    {0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x3734},
    {0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x3736},
    {0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x3742},
    {0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0x3862},
    {0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x38CC},
    {0x2F, 0x2F, 0x2F, 0x2F, 0x2F, 0x2F, 0x38CD},
    {0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x395C},
    {0xD1, 0xD1, 0xD1, 0xD1, 0xD1, 0xD1, 0x3A42},
    {0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x3A4C},
    {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x3AE0},
    {0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x3AEC},
    {0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x3B00},
    {0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x3B06},
    {0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x3B98},
    {0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x3B99},
    {0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x3B9B},
    {0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x3B9C},
    {0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x3B9D},
    {0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x3B9E},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3BA1},
    {0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x3BA2},
    {0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x3BA3},
    {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x3BA4},
    {0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x3BA5},
    {0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3BA6},
    {0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x3BA7},
    {0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x3BA8},
    {0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x3BA9},
    {0xED, 0xED, 0xED, 0xED, 0xED, 0xED, 0x3BAC},
    {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x3BAD},
    {0xF6, 0xF6, 0xF6, 0xF6, 0xF6, 0xF6, 0x3BAE},
    {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x3BAF},
    {0xA2, 0xA2, 0xA2, 0xA2, 0xA2, 0xA2, 0x3BB0},
    {0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x3BB1},
    {0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0x3BB2},
    {0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x3BB3},
    {0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0x3BB4},
    {0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x3BB5},
    {0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0x3BB6},
    {0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x3BB7},
    {0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0x3BB8},
    {0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0x3BBA},
    {0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0x3BBC},
    {0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x3BBE},
    {0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x3BC0},
    {0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x7B, 0x3BC2},
    {0xA2, 0xA2, 0xA2, 0xA2, 0xA2, 0xA2, 0x3BC4},
    {0xBD, 0xBD, 0xBD, 0xBD, 0xBD, 0xBD, 0x3BC8},
    {0xBD, 0xBD, 0xBD, 0xBD, 0xBD, 0xBD, 0x3BCA},
    {0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x4004},  /* TXCLKESC_FREQ low (0x0948 @ INCK 37.125 MHz) */
    {0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x4005},  /* TXCLKESC_FREQ hi */
    {0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x400C},  /* INCKSEL6: Mode3 = 0x01 for 1485Mbps PLL */
    {0x7F, 0x67, 0x7F, 0xA7, 0x7F, 0x7F, 0x4018},  /* TCLKPOST   — Mode4 = Mode0 (891 Mbps) */
    {0x37, 0x27, 0x37, 0x57, 0x37, 0x37, 0x401A},  /* TCLKPREPARE */
    {0x37, 0x27, 0x37, 0x57, 0x37, 0x37, 0x401C},  /* TCLKTRAIL */
    {0xF7, 0xB7, 0xF7, 0x97, 0xF7, 0xF7, 0x401E},  /* TCLKZERO low */
    {0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x401F},  /* TCLKZERO hi */
    {0x3F, 0x2F, 0x3F, 0x57, 0x3F, 0x3F, 0x4020},  /* THSPREPARE */
    {0x6F, 0x4F, 0x6F, 0xA7, 0x6F, 0x6F, 0x4022},  /* THSZERO */
    {0x3F, 0x2F, 0x3F, 0x57, 0x3F, 0x3F, 0x4024},  /* THSTRAIL */
    {0x5F, 0x47, 0x5F, 0x97, 0x5F, 0x5F, 0x4026},  /* THSEXIT */
    {0x2F, 0x27, 0x2F, 0x47, 0x2F, 0x2F, 0x4028},  /* TLPX */
    {0x01, 0x01, 0x01, 0x00, 0x01, 0x01, 0x4074},  /* INCKSEL7 */

    /* ─── Mode 4: window-cropping setup (WINMODE=4 + active area defines) ───
     * Default 1920×1080 crop centred in the 3840×2160 sensor area:
     *   PIX_HST    = (3840 - 1920)/2 = 960  → 0x03C0
     *   PIX_HWIDTH = 1920                   → 0x0780
     *   PIX_VST    = ((2160 - 1080)/2)×2 = 1080 → 0x0438 (V units = lines×2)
     *   PIX_VWIDTH = 1080 × 2                = 2160 → 0x0870
     * Multiple-of-N constraints per datasheet p63: HST×2, HWIDTH×24, VST×4,
     * VWIDTH×4 — all satisfied by 1920×1080 centred. */
    {  NA,   NA,   NA,   NA, 0x04, 0x04, 0x301C},  /* WINMODE = 4h (window cropping) */
    {  NA,   NA,   NA,   NA, 0xC0, 0xC0, 0x3040},  /* PIX_HST    low  = 0xC0 (960 & 0xFF) */
    {  NA,   NA,   NA,   NA, 0x03, 0x03, 0x3041},  /* PIX_HST    high = 0x03 (960 >> 8) */
    {  NA,   NA,   NA,   NA, 0x80, 0x80, 0x3042},  /* PIX_HWIDTH low  = 0x80 (1920 & 0xFF) */
    {  NA,   NA,   NA,   NA, 0x07, 0x07, 0x3043},  /* PIX_HWIDTH high = 0x07 */
    {  NA,   NA,   NA,   NA, 0x38, 0x38, 0x3044},  /* PIX_VST    low  = 0x38 (1080 & 0xFF) */
    {  NA,   NA,   NA,   NA, 0x04, 0x04, 0x3045},  /* PIX_VST    high = 0x04 */
    {  NA,   NA,   NA,   NA, 0x70, 0x70, 0x3046},  /* PIX_VWIDTH low  = 0x70 (2160 & 0xFF) */
    {  NA,   NA,   NA,   NA, 0x08, 0x08, 0x3047},  /* PIX_VWIDTH high = 0x08 */
};

void imx415_init(VI_PIPE vi_pipe)
{
    HI_U32 i;
    HI_U8 u8ImgMode;
    HI_BOOL bInit;
    HI_U16 reg_data;
    HI_U16 u16RegAddr;
    HI_U32 u32SeqEntries;
    HI_S32 ret = HI_SUCCESS;
    ISP_SNS_STATE_S *pastimx415 = HI_NULL;
    const IMX415_VIDEO_MODE_TBL_S *imx415modetb1 = HI_NULL;
    pastimx415 = imx415_get_ctx(vi_pipe);
    bInit       = pastimx415->bInit;
    u8ImgMode   = pastimx415->u8ImgMode;
    imx415modetb1 = imx415_get_mode_tb1(u8ImgMode);
    if (bInit == HI_FALSE) {
        /* 2. sensor i2c init */
        imx415_i2c_init(vi_pipe);
    }

    u32SeqEntries = sizeof(g_au16SensorCfgSeq) / sizeof(g_au16SensorCfgSeq[0]);

    for (i = 0 ; i < u32SeqEntries; i++) {
        u16RegAddr = g_au16SensorCfgSeq[i][IMX415_MODE_BUTT];
        reg_data = g_au16SensorCfgSeq[i][u8ImgMode];

        if (reg_data == NA) {
            continue;
        }

        ret += imx415_write_register(vi_pipe, u16RegAddr, reg_data);
    }

    for (i = 0; i < pastimx415->astRegsInfo[0].u32RegNum; i++) {
        ret += imx415_write_register(vi_pipe,
                                     pastimx415->astRegsInfo[0].astI2cData[i].u32RegAddr,
                                     pastimx415->astRegsInfo[0].astI2cData[i].u32Data);
    }

    /* ─── Mode 4 / Mode 5: runtime crop W×H override ──────────────────────
     * Env vars IMX415_CROP_W and IMX415_CROP_H let the user pick a different
     * crop window without recompiling. Values are clamped to sensor extents,
     * snapped to the datasheet multiple-of-N rules, and centred in the 4K
     * active area. Applies to Mode 4 (all-pixel crop) and Mode 5 (binning
     * crop). Without this, Mode 5 keeps emitting 1920×1080 binned per frame
     * — VMAX shrinks faster than the active line count and the sensor's
     * output frame no longer fits in VMAX lines, capping VI at the largest
     * fps where VMAX ≥ 1080+58 (≈121 fps). With the crop applied for Mode 5,
     * fps scales freely with crop_h until either lane or ISP_FE hits. */
    if (u8ImgMode == IMX415_CROP_FLEX_LINEAR_MODE ||
        u8ImgMode == IMX415_CROP_BIN_FLEX_LINEAR_MODE) {
        const char *cw = getenv("IMX415_CROP_W");
        const char *ch = getenv("IMX415_CROP_H");
        HI_U32 crop_w = cw ? (HI_U32)atoi(cw) : IMX415_CROP_W_DEFAULT;
        HI_U32 crop_h = ch ? (HI_U32)atoi(ch) : IMX415_CROP_H_DEFAULT;
        /* snap to required multiples: HWIDTH×24, VWIDTH×4 (lines) */
        /* Crop registers operate on PRE-binning coordinates per datasheet.
         * Mode 5 has ADDMODE=1 (2×2 binning), so the post-binning output is
         * crop_w × crop_h while the AREA registers must describe the
         * pre-binning crop_w*2 × crop_h*2 window. */
        HI_U32 bin = (u8ImgMode == IMX415_CROP_BIN_FLEX_LINEAR_MODE) ? 2 : 1;
        crop_w = (crop_w / (12 * bin)) * (12 * bin);
        crop_h = (crop_h / (2 * bin))  * (2 * bin);
        if (crop_w < 12 * bin) crop_w = 12 * bin;
        if (crop_w > IMX415_SENSOR_W / bin) crop_w = IMX415_SENSOR_W / bin;
        if (crop_h < 2 * bin)  crop_h = 2 * bin;
        if (crop_h > IMX415_SENSOR_H / bin) crop_h = IMX415_SENSOR_H / bin;
        HI_U32 pre_w  = crop_w * bin;                          /* pre-bin pixels */
        HI_U32 pre_h  = crop_h * bin;                          /* pre-bin lines  */
        HI_U32 hst    = (IMX415_SENSOR_W - pre_w) / 2;         /* horiz start, pixels */
        HI_U32 vst    = (IMX415_SENSOR_H - pre_h);             /* V start in V units = lines*2 */
        HI_U32 hwidth = pre_w;                                 /* pixels */
        HI_U32 vwidth = pre_h * 2;                             /* V units = lines*2 */
        hst = (hst / 2) * 2;
        vst = (vst / 4) * 4;
        ret += imx415_write_register(vi_pipe, IMX415_PIX_HST_LOW,     hst    & 0xFF);
        ret += imx415_write_register(vi_pipe, IMX415_PIX_HST_HIGH,   (hst    >> 8) & 0x1F);
        ret += imx415_write_register(vi_pipe, IMX415_PIX_HWIDTH_LOW,  hwidth & 0xFF);
        ret += imx415_write_register(vi_pipe, IMX415_PIX_HWIDTH_HIGH,(hwidth >> 8) & 0x1F);
        ret += imx415_write_register(vi_pipe, IMX415_PIX_VST_LOW,     vst    & 0xFF);
        ret += imx415_write_register(vi_pipe, IMX415_PIX_VST_HIGH,   (vst    >> 8) & 0x1F);
        ret += imx415_write_register(vi_pipe, IMX415_PIX_VWIDTH_LOW,  vwidth & 0xFF);
        ret += imx415_write_register(vi_pipe, IMX415_PIX_VWIDTH_HIGH,(vwidth >> 8) & 0x1F);
        printf("IMX415 crop: %ux%u centred (HST=%u HWIDTH=%u VST=%u VWIDTH=%u)\n",
               crop_w, crop_h, hst, hwidth, vst, vwidth);
    }

    ret += imx415_write_register(vi_pipe, 0x3000, 0x00); /* Standby Cancel */

    delay_ms(24);  /* delay 24 ms */
    ret += imx415_write_register(vi_pipe, 0x3002, 0x00);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("write register failed!\n");
        return;
    }

    pastimx415->bInit = HI_TRUE;

    printf("IMX415 %s init succuss!\n", imx415modetb1->pszModeName);

    return ;
}

void imx415_exit(VI_PIPE vi_pipe)
{
    HI_S32 ret;
    ret = imx415_i2c_exit(vi_pipe);
    if (ret != HI_SUCCESS) {
        SNS_ERR_TRACE("i2c exit failed!\n");
        return;
    }
    g_bStandby[vi_pipe] = HI_FALSE;

    return;
}
