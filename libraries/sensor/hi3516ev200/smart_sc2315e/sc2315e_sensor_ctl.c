/******************************************************************************

  Copyright (C), 2021, Dmitry Ilyin, OpenIPC

 ******************************************************************************
  File Name     : sc2315e_sensor_ctl.c

******************************************************************************/
#include "comm_video.h"
#include "sns_ctrl.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "universal.h"
#include <unistd.h>

#ifdef GPIO_I2C
#include "gpioi2c_ex.h"
#else

#ifdef __HuaweiLite__
#include "i2c.h"
#else
#include "i2c.h"
#endif

#endif

#include "gk_api_isp.h"
#include "hicompat.h"
const unsigned char sc2315e_i2c_addr = 0x60; /* I2C Address of sc2315e */
const unsigned int sc2315e_addr_byte = 2;
const unsigned int sc2315e_data_byte = 1;
static int g_fd[ISP_MAX_PIPE_NUM] = {[0 ...(ISP_MAX_PIPE_NUM - 1)] = -1};

extern ISP_SNS_STATE_S *g_pastSc2315e[ISP_MAX_PIPE_NUM];
extern ISP_SNS_COMMBUS_U g_aunSc2315eBusInfo[];

GK_S32 sc2315e_GetQuickStartFlag(VI_PIPE ViPipe, GK_U32 *quickStartFlag) {
  GK_S32 s32Ret = 0;
  ISP_MOD_PARAM_S stModParam;
  s32Ret = GK_API_ISP_GetModParam(&stModParam);
  if (s32Ret != GK_SUCCESS) {
    ISP_TRACE(MODULE_DBG_ERR, "sc2315e Get QuickStart Error\n");
    return s32Ret;
  }
  *quickStartFlag = stModParam.u32QuickStart;
  return GK_SUCCESS;
}

int sc2315e_i2c_init(VI_PIPE ViPipe) {
  char acDevFile[16] = {0};
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

  u8DevNum = g_aunSc2315eBusInfo[ViPipe].s8I2cDev;
  snprintf(acDevFile, sizeof(acDevFile), "/dev/i2c-%u", u8DevNum);

  g_fd[ViPipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);

  if (g_fd[ViPipe] < 0) {
    ISP_TRACE(MODULE_DBG_ERR, "Open /dev/hi_i2c_drv-%u error!\n", u8DevNum);
    return GK_FAILURE;
  }

  ret = ioctl(g_fd[ViPipe], I2C_SLAVE_FORCE, (sc2315e_i2c_addr >> 1));
  if (ret < 0) {
    ISP_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
    close(g_fd[ViPipe]);
    g_fd[ViPipe] = -1;
    return ret;
  }
#endif

  return GK_SUCCESS;
}

int sc2315e_i2c_exit(VI_PIPE ViPipe) {
  if (g_fd[ViPipe] >= 0) {
    close(g_fd[ViPipe]);
    g_fd[ViPipe] = -1;
    return GK_SUCCESS;
  }
  return GK_FAILURE;
}

int sc2315e_read_register(VI_PIPE ViPipe, GK_U32 addr) {
  GK_S32 s32RegVal = 0;
  if (g_fd[ViPipe] < 0) {
    ISP_TRACE(MODULE_DBG_ERR, "sc2315e_read_register fd not opened!\n");
    return GK_FAILURE;
  }

  GK_S32 s32Ret = 0;
  GK_U32 u32RegWidth = sc2315e_addr_byte;
  GK_U32 u32DataWidth = sc2315e_data_byte;
  GK_U8 aRecvbuf[4];

#ifdef LOSCFG_HOST_TYPE_HIBVT
  GK_U32 u32SnsI2cAddr = (sc2315e_i2c_addr >> 1);
  struct i2c_rdwr_ioctl_data stRdwr;
  struct i2c_msg astMsg[2];
  memset(&stRdwr, 0x0, sizeof(stRdwr));
  memset(astMsg, 0x0, sizeof(astMsg));
#endif

  memset(aRecvbuf, 0x0, sizeof(aRecvbuf));

#ifdef LOSCFG_HOST_TYPE_HIBVT
  astMsg[0].addr = u32SnsI2cAddr;
  astMsg[0].flags = 0;
  astMsg[0].len = u32RegWidth;
  astMsg[0].buf = aRecvbuf;

  astMsg[1].addr = u32SnsI2cAddr;
  astMsg[1].flags = 0;
  astMsg[1].flags |= I2C_M_RD;
  astMsg[1].len = u32DataWidth;
  astMsg[1].buf = aRecvbuf;
  stRdwr.msgs = &astMsg[0];
  stRdwr.nmsgs = 2;
#endif

#ifdef LOSCFG_HOST_TYPE_HIBVT
  if (u32RegWidth == 2) {
    aRecvbuf[0] = (addr >> 8) & 0xff;
    aRecvbuf[1] = addr & 0xff;
  } else {
    aRecvbuf[0] = addr & 0xff;
  }
  s32Ret = ioctl(g_fd[ViPipe], I2C_RDWR, &stRdwr);
#else
  if (u32RegWidth == 2) {
    aRecvbuf[0] = addr & 0xff;
    aRecvbuf[1] = (addr >> 8) & 0xff;
  } else {
    aRecvbuf[0] = addr & 0xff;
  }
  s32Ret = read(g_fd[ViPipe], aRecvbuf, u32RegWidth + u32DataWidth);
#endif
  if (s32Ret < 0) {
    return GK_FAILURE;
  }

  if (u32DataWidth == 2) {
    s32RegVal = aRecvbuf[0] | (aRecvbuf[1] << 8);
  } else {
    s32RegVal = aRecvbuf[0];
  }
  return s32RegVal;
}

int sc2315e_write_register(VI_PIPE ViPipe, GK_U32 addr, GK_U32 data) {
  if (g_fd[ViPipe] < 0) {
    return GK_SUCCESS;
  }

#ifdef GPIO_I2C
  i2c_data.dev_addr = sc2315e_i2c_addr;
  i2c_data.reg_addr = addr;
  i2c_data.addr_byte_num = sc2315e_addr_byte;
  i2c_data.data = data;
  i2c_data.data_byte_num = sc2315e_data_byte;

  ret = ioctl(g_fd[ViPipe], GPIO_I2C_WRITE, &i2c_data);

  if (ret) {
    ISP_TRACE(MODULE_DBG_ERR, "GPIO-I2C write faild!\n");
    return ret;
  }
#else
  int idx = 0;
  int ret;
  char buf[8];

  if (sc2315e_addr_byte == 2) {
    buf[idx] = (addr >> 8) & 0xff;
    idx++;
    buf[idx] = addr & 0xff;
    idx++;
  }
  buf[idx] = data & 0xff;
  idx++;

  ret = write(g_fd[ViPipe], buf, sc2315e_addr_byte + sc2315e_data_byte);
  if (ret < 0) {
    ISP_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
    return GK_FAILURE;
  }

#endif
  return GK_SUCCESS;
}

static void delay_ms(int ms) { usleep(ms * 1000); }

void sc2315e_standby(VI_PIPE ViPipe) { return; }

void sc2315e_restart(VI_PIPE ViPipe) { return; }

#define SC2315E_SENSOR_1080P_30FPS_LINEAR_MODE (1)

void sc2315e_linear_1080P30_init(VI_PIPE ViPipe);

void sc2315e_init(VI_PIPE ViPipe) {
  GK_U32 i;
  GK_U32 quickStartFlag = 0;
  sc2315e_GetQuickStartFlag(ViPipe, &quickStartFlag);
  sc2315e_i2c_init(ViPipe);
  /* When sensor first init, config all registers */
  if (quickStartFlag == 0) {
    sc2315e_linear_1080P30_init(ViPipe);
    delay_ms(10);
    for (i = 0; i < g_pastSc2315e[ViPipe]->astRegsInfo[0].u32RegNum; i++) {
      sc2315e_write_register(
          ViPipe,
          g_pastSc2315e[ViPipe]->astRegsInfo[0].astI2cData[i].u32RegAddr,
          g_pastSc2315e[ViPipe]->astRegsInfo[0].astI2cData[i].u32Data);
    }
  }
  g_pastSc2315e[ViPipe]->bInit = GK_TRUE;

  return;
}

void sc2315e_exit(VI_PIPE ViPipe) {
  sc2315e_i2c_exit(ViPipe);

  return;
}

#define sensor_write_register(ADDR, DATA) \
  sc2315e_write_register(ViPipe, ADDR, DATA)

/***************** 1080P30 (1920X1080@30fps) ***************/
void sc2315e_linear_1080P30_init(VI_PIPE ViPipe) {
  sc2315e_write_register(ViPipe, 0x100, 0x0);
  usleep(10000);
  sensor_write_register(0x3034, 0x81);
  sensor_write_register(0x3039, 0xA6);
  sensor_write_register(0x3001, 0xFE);
  sensor_write_register(0x3018, 0x33);
  sensor_write_register(0x301C, 0x78);
  sensor_write_register(0x301F, 1);
  sensor_write_register(0x3031, 0xA); // [3:0] 10bit
  sensor_write_register(0x3037, 0x20);
  sensor_write_register(0x3038, 0xFF);
  sensor_write_register(0x303B, 0x16);
  sensor_write_register(0x303C, 0xE);
  sensor_write_register(0x320C, 8);
  sensor_write_register(0x320D, 0x98);
  sensor_write_register(0x320E, 4);
  sensor_write_register(0x320F, 0x65);
  sensor_write_register(0x3211, 0xC);
  sensor_write_register(0x3213, 4);
  sensor_write_register(0x3222, 0x29);
  sensor_write_register(0x3235, 8);
  sensor_write_register(0x3236, 0xC8);
  sensor_write_register(0x3301, 0xF);
  sensor_write_register(0x3302, 0x1F);
  sensor_write_register(0x3303, 0x20);
  sensor_write_register(0x3306, 0x48);
  sensor_write_register(0x3308, 0x10);
  sensor_write_register(0x3309, 0x60);
  sensor_write_register(0x330B, 0xCD);
  sensor_write_register(0x330E, 0x30);
  sensor_write_register(0x3314, 4);
  sensor_write_register(0x331B, 0x83);
  sensor_write_register(0x331E, 0x19);
  sensor_write_register(0x331F, 0x59);
  sensor_write_register(0x3320, 1);
  sensor_write_register(0x3326, 0);
  sensor_write_register(0x3333, 0x30);
  sensor_write_register(0x335E, 1);
  sensor_write_register(0x335F, 3);
  sensor_write_register(0x3366, 0x7C);
  sensor_write_register(0x3367, 8);
  sensor_write_register(0x3368, 4);
  sensor_write_register(0x3369, 0);
  sensor_write_register(0x336A, 0);
  sensor_write_register(0x336B, 0);
  sensor_write_register(0x337C, 4);
  sensor_write_register(0x337D, 6);
  sensor_write_register(0x337F, 3);
  sensor_write_register(0x33A0, 5);
  sensor_write_register(0x33AA, 0x10);
  sensor_write_register(0x360F, 1);
  sensor_write_register(0x3614, 0x80);
  sensor_write_register(0x3620, 0x28);
  sensor_write_register(0x3621, 0x28);
  sensor_write_register(0x3622, 6);
  sensor_write_register(0x3624, 8);
  sensor_write_register(0x3625, 2);
  sensor_write_register(0x3630, 0x9C);
  sensor_write_register(0x3631, 0x84);
  sensor_write_register(0x3632, 8);
  sensor_write_register(0x3633, 0x4F);
  sensor_write_register(0x3635, 0xA0);
  sensor_write_register(0x3636, 0x25);
  sensor_write_register(0x3637, 0x55);
  sensor_write_register(0x3638, 0x1F);
  sensor_write_register(0x3639, 9);
  sensor_write_register(0x363A, 0x9F);
  sensor_write_register(0x363B, 6);
  sensor_write_register(0x363C, 5);
  sensor_write_register(0x3641, 1);
  sensor_write_register(0x366E, 8);
  sensor_write_register(0x366F, 0x2C);
  sensor_write_register(0x3670, 0xC);
  sensor_write_register(0x3671, 0xC6);
  sensor_write_register(0x3672, 6);
  sensor_write_register(0x3673, 0x16);
  sensor_write_register(0x3677, 0x86);
  sensor_write_register(0x3678, 0x88);
  sensor_write_register(0x3679, 0x88);
  sensor_write_register(0x367A, 0x28);
  sensor_write_register(0x367B, 0x3F);
  sensor_write_register(0x367E, 8);
  sensor_write_register(0x367F, 0x28);
  sensor_write_register(0x3690, 0x33);
  sensor_write_register(0x3691, 0x11);
  sensor_write_register(0x3692, 0x43);
  sensor_write_register(0x369C, 8);
  sensor_write_register(0x369D, 0x28);
  sensor_write_register(0x3802, 0);
  sensor_write_register(0x3900, 0x19);
  sensor_write_register(0x3901, 2);
  sensor_write_register(0x3902, 0xC5);
  sensor_write_register(0x3903, 8);
  sensor_write_register(0x3905, 0x98);
  sensor_write_register(0x3907, 0);
  sensor_write_register(0x3908, 0x11);
  sensor_write_register(0x391D, 1);
  sensor_write_register(0x391E, 0);
  sensor_write_register(0x3933, 0xA);
  sensor_write_register(0x3934, 0x18);
  sensor_write_register(0x3940, 0x60);
  sensor_write_register(0x3942, 2);
  sensor_write_register(0x3943, 0x1F);
  sensor_write_register(0x395E, 0xC0);
  sensor_write_register(0x3960, 0xBA);
  sensor_write_register(0x3961, 0xAE);
  sensor_write_register(0x3962, 0x89);
  sensor_write_register(0x3963, 0x80);
  sensor_write_register(0x3966, 0xBA);
  sensor_write_register(0x3980, 0xA0);
  sensor_write_register(0x3981, 0x40);
  sensor_write_register(0x3982, 0x18);
  sensor_write_register(0x3984, 8);
  sensor_write_register(0x3985, 0x40);
  sensor_write_register(0x3986, 0x80);
  sensor_write_register(0x3987, 0xB0);
  sensor_write_register(0x3988, 8);
  sensor_write_register(0x3989, 0x10);
  sensor_write_register(0x398A, 0x20);
  sensor_write_register(0x398B, 0x30);
  sensor_write_register(0x398C, 0x60);
  sensor_write_register(0x398D, 0x20);
  sensor_write_register(0x398E, 0x10);
  sensor_write_register(0x398F, 8);
  sensor_write_register(0x3990, 0x60);
  sensor_write_register(0x3991, 0x24);
  sensor_write_register(0x3992, 0x15);
  sensor_write_register(0x3993, 8);
  sensor_write_register(0x3994, 0xA);
  sensor_write_register(0x3995, 0x20);
  sensor_write_register(0x3996, 0x38);
  sensor_write_register(0x3997, 0xA0);
  sensor_write_register(0x3998, 8);
  sensor_write_register(0x3999, 0x10);
  sensor_write_register(0x399A, 0x18);
  sensor_write_register(0x399B, 0x30);
  sensor_write_register(0x399C, 0x30);
  sensor_write_register(0x399D, 0x18);
  sensor_write_register(0x399E, 0x10);
  sensor_write_register(0x399F, 8);
  sensor_write_register(0x3E00, 0);
  sensor_write_register(0x3E01, 0x8C);
  sensor_write_register(0x3E02, 0x60);
  sensor_write_register(0x3E03, 0xB);
  sensor_write_register(0x3E06, 0);
  sensor_write_register(0x3E07, 0x80);
  sensor_write_register(0x3E08, 3);
  sensor_write_register(0x3E09, 0x10);
  sensor_write_register(0x3E1E, 0x34);
  sensor_write_register(0x3F00, 7);
  sensor_write_register(0x3F04, 4);
  sensor_write_register(0x3F05, 0x28);
  sensor_write_register(0x4603, 0);
  sensor_write_register(0x4809, 1);
  sensor_write_register(0x4827, 0x48);
  sensor_write_register(0x4837, 0x35);
  sensor_write_register(0x5000, 6);
  sensor_write_register(0x5780, 0xFF);
  sensor_write_register(0x5781, 4);
  sensor_write_register(0x5782, 3);
  sensor_write_register(0x5783, 2);
  sensor_write_register(0x5784, 1);
  sensor_write_register(0x5785, 0x18);
  sensor_write_register(0x5786, 0x10);
  sensor_write_register(0x5787, 8);
  sensor_write_register(0x5788, 2);
  sensor_write_register(0x57A0, 0);
  sensor_write_register(0x57A1, 0x71);
  sensor_write_register(0x57A2, 1);
  sensor_write_register(0x57A3, 0xF1);
  sensor_write_register(0x3034, 1);
  sensor_write_register(0x3039, 0x26);
  sensor_write_register(0x100, 1);
  usleep(10000);
  log_d("Smart sc2315e sensor 1080P30fps(MIPI) init success");
  return;
}
