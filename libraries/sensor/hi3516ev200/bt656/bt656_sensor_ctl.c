/*
 * OpenIPC.org
 */
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "comm_video.h"
#include "gk_api_isp.h"
#include "sns_ctrl.h"

#ifdef GPIO_I2C
#include "gpioi2c_ex.h"
#else
#include "i2c.h"
#endif

const unsigned char bt656_i2c_addr = 0x88; /* TP9950 I2C Address */
const unsigned int bt656_addr_byte = 1;
const unsigned int bt656_data_byte = 1;
static int g_fd[ISP_MAX_PIPE_NUM] = {[0 ...(ISP_MAX_PIPE_NUM - 1)] = -1};

extern ISP_SNS_STATE_S *g_pastBT656[ISP_MAX_PIPE_NUM];
extern ISP_SNS_COMMBUS_U g_aunBT656BusInfo[];

int sensor_i2c_init(VI_PIPE ViPipe) {
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

  u8DevNum = g_aunBT656BusInfo[ViPipe].s8I2cDev;
  snprintf(acDevFile, sizeof(acDevFile), "/dev/i2c-%u", u8DevNum);

  g_fd[ViPipe] = open(acDevFile, O_RDWR, S_IRUSR | S_IWUSR);
  if (g_fd[ViPipe] < 0) {
    ISP_TRACE(MODULE_DBG_ERR, "Open /dev/i2c_drv-%u error!\n", u8DevNum);
    return GK_FAILURE;
  }

  ret = ioctl(g_fd[ViPipe], I2C_SLAVE_FORCE, (bt656_i2c_addr >> 1));
  if (ret < 0) {
    ISP_TRACE(MODULE_DBG_ERR, "I2C_SLAVE_FORCE error!\n");
    close(g_fd[ViPipe]);
    g_fd[ViPipe] = -1;
    return ret;
  }
#endif

  return GK_SUCCESS;
}

int bt656_i2c_exit(VI_PIPE ViPipe) {
  if (g_fd[ViPipe] >= 0) {
    close(g_fd[ViPipe]);
    g_fd[ViPipe] = -1;
    return GK_SUCCESS;
  }

  return GK_FAILURE;
}

int sensor_read_register(VI_PIPE ViPipe, int addr) {
  GK_S32 s32RegVal = 0;
  if (g_fd[ViPipe] < 0) {
    ISP_TRACE(MODULE_DBG_ERR, "bt656 fd not opened!\n");
    return GK_FAILURE;
  }

  GK_S32 s32Ret = 0;
  GK_U32 u32RegWidth = bt656_addr_byte;
  GK_U32 u32DataWidth = bt656_data_byte;
  GK_U8 aRecvbuf[4];

#ifdef LOSCFG_HOST_TYPE_VENDOR
  GK_U32 u32SnsI2cAddr = (bt656_i2c_addr >> 1);
  struct i2c_rdwr_ioctl_data stRdwr;
  struct i2c_msg astMsg[2];
  memset(&stRdwr, 0x0, sizeof(stRdwr));
  memset(astMsg, 0x0, sizeof(astMsg));
#endif

  memset(aRecvbuf, 0x0, sizeof(aRecvbuf));

#ifdef LOSCFG_HOST_TYPE_VENDOR
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

#ifdef LOSCFG_HOST_TYPE_VENDOR
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

int sensor_write_register(VI_PIPE ViPipe, int addr, int data) {
  if (0 > g_fd[ViPipe]) {
    return GK_SUCCESS;
  }

#ifdef GPIO_I2C
  i2c_data.dev_addr = bt656_i2c_addr;
  i2c_data.reg_addr = addr;
  i2c_data.addr_byte_num = bt656_addr_byte;
  i2c_data.data = data;
  i2c_data.data_byte_num = bt656_data_byte;

  ret = ioctl(g_fd[ViPipe], GPIO_I2C_WRITE, &i2c_data);
  if (ret) {
    ISP_TRACE(MODULE_DBG_ERR, "GPIO-I2C write faild!\n");
    return ret;
  }
#else
  int idx = 0;
  int ret;
  char buf[8] = {0};

  if (bt656_addr_byte == 2) {
    buf[idx] = (addr >> 8) & 0xff;
    idx++;
    buf[idx] = addr & 0xff;
    idx++;
  } else {
    buf[idx] = addr & 0xff;
    idx++;
  }

  if (bt656_data_byte == 2) {
    buf[idx] = (data >> 8) & 0xff;
    idx++;
    buf[idx] = data & 0xff;
    idx++;
  } else {
    buf[idx] = data & 0xff;
    idx++;
  }

  ret = write(g_fd[ViPipe], buf, bt656_addr_byte + bt656_data_byte);
  if (ret < 0) {
    ISP_TRACE(MODULE_DBG_ERR, "I2C_WRITE error!\n");
    return GK_FAILURE;
  }
#endif

  return GK_SUCCESS;
}

void bt656_standby(VI_PIPE ViPipe) { return; }

void bt656_restart(VI_PIPE ViPipe) { return; }

void bt656_init(VI_PIPE ViPipe) {
  sensor_i2c_init(ViPipe);

  tp9950_960h25_init(ViPipe);
  //tp9950_720p30_init(ViPipe);
  // tp9950_1080p30_init(ViPipe);

  g_pastBT656[ViPipe]->bInit = GK_TRUE;

  return;
}

void bt656_exit(VI_PIPE ViPipe) {
  bt656_i2c_exit(ViPipe);

  return;
}


void tp9950_960h25_init(VI_PIPE ViPipe) {
  sensor_write_register(ViPipe, 0x02, 0xCE);
  sensor_write_register(ViPipe, 0x05, 0x00);
  sensor_write_register(ViPipe, 0x06, 0x32);
  sensor_write_register(ViPipe, 0x07, 0xC0);
  sensor_write_register(ViPipe, 0x08, 0x00);
  sensor_write_register(ViPipe, 0x09, 0x24);
  sensor_write_register(ViPipe, 0x0A, 0x48);
  sensor_write_register(ViPipe, 0x0B, 0xC0);
  sensor_write_register(ViPipe, 0x0C, 0x13);
  sensor_write_register(ViPipe, 0x0D, 0x51);
  sensor_write_register(ViPipe, 0x0E, 0x00);
  sensor_write_register(ViPipe, 0x0F, 0x00);
  sensor_write_register(ViPipe, 0x10, 0xF8);
  sensor_write_register(ViPipe, 0x11, 0x40);
  sensor_write_register(ViPipe, 0x12, 0x60);
  sensor_write_register(ViPipe, 0x13, 0x00);
  sensor_write_register(ViPipe, 0x14, 0x00);
  sensor_write_register(ViPipe, 0x15, 0x13);
  sensor_write_register(ViPipe, 0x16, 0x70);
  sensor_write_register(ViPipe, 0x17, 0xBC);
  sensor_write_register(ViPipe, 0x18, 0x17);
  sensor_write_register(ViPipe, 0x19, 0x20);
  sensor_write_register(ViPipe, 0x1A, 0x17);
  sensor_write_register(ViPipe, 0x1B, 0x01);
  sensor_write_register(ViPipe, 0x1C, 0x09);
  sensor_write_register(ViPipe, 0x1D, 0x48);
  sensor_write_register(ViPipe, 0x1E, 0x80);
  sensor_write_register(ViPipe, 0x1F, 0x80);
  sensor_write_register(ViPipe, 0x20, 0x48);
  sensor_write_register(ViPipe, 0x21, 0x84);
  sensor_write_register(ViPipe, 0x22, 0x37);
  sensor_write_register(ViPipe, 0x23, 0x3F);
  sensor_write_register(ViPipe, 0x24, 0x04);
  sensor_write_register(ViPipe, 0x25, 0xFF);
  sensor_write_register(ViPipe, 0x26, 0x05);
  sensor_write_register(ViPipe, 0x27, 0x2D);
  sensor_write_register(ViPipe, 0x28, 0x00);
  sensor_write_register(ViPipe, 0x29, 0x48);
  sensor_write_register(ViPipe, 0x2A, 0x30);
  sensor_write_register(ViPipe, 0x2B, 0x70);
  sensor_write_register(ViPipe, 0x2C, 0x2A);
  sensor_write_register(ViPipe, 0x2D, 0x64);
  sensor_write_register(ViPipe, 0x2E, 0x56);
  sensor_write_register(ViPipe, 0x2F, 0x00);
  sensor_write_register(ViPipe, 0x30, 0x7A);
  sensor_write_register(ViPipe, 0x31, 0x4A);
  sensor_write_register(ViPipe, 0x32, 0x4D);
  sensor_write_register(ViPipe, 0x33, 0xF0);
  sensor_write_register(ViPipe, 0x34, 0x00);
  sensor_write_register(ViPipe, 0x35, 0x65);
  sensor_write_register(ViPipe, 0x36, 0xDC);
  sensor_write_register(ViPipe, 0x37, 0x00);
  sensor_write_register(ViPipe, 0x38, 0x00);
  sensor_write_register(ViPipe, 0x39, 0x04);
  sensor_write_register(ViPipe, 0x3A, 0x32);
  sensor_write_register(ViPipe, 0x3B, 0x26);
  sensor_write_register(ViPipe, 0x3C, 0x00);
  sensor_write_register(ViPipe, 0x3D, 0x60);
  sensor_write_register(ViPipe, 0x3E, 0x00);
  sensor_write_register(ViPipe, 0x3F, 0x00);
  sensor_write_register(ViPipe, 0x40, 0x00);
  sensor_write_register(ViPipe, 0x41, 0x00);
  sensor_write_register(ViPipe, 0x42, 0x00);
  sensor_write_register(ViPipe, 0x43, 0x00);
  sensor_write_register(ViPipe, 0x44, 0x00);
  sensor_write_register(ViPipe, 0x45, 0x00);
  sensor_write_register(ViPipe, 0x46, 0x00);
  sensor_write_register(ViPipe, 0x47, 0x00);
  sensor_write_register(ViPipe, 0x48, 0x00);
  sensor_write_register(ViPipe, 0x49, 0x00);
  sensor_write_register(ViPipe, 0x4A, 0x00);
  sensor_write_register(ViPipe, 0x4B, 0x00);
  sensor_write_register(ViPipe, 0x4C, 0x43);
  sensor_write_register(ViPipe, 0x4D, 0x00);
  sensor_write_register(ViPipe, 0x4E, 0x17);
  sensor_write_register(ViPipe, 0x4F, 0x00);
  sensor_write_register(ViPipe, 0x50, 0x00);
  sensor_write_register(ViPipe, 0x51, 0x00);
  sensor_write_register(ViPipe, 0x52, 0x00);
  sensor_write_register(ViPipe, 0x53, 0x00);
  sensor_write_register(ViPipe, 0x54, 0x00);

  sensor_write_register(ViPipe, 0xB3, 0xFA);
  sensor_write_register(ViPipe, 0xB4, 0x00);
  sensor_write_register(ViPipe, 0xB5, 0x00);
  sensor_write_register(ViPipe, 0xB6, 0x00);
  sensor_write_register(ViPipe, 0xB7, 0x00);
  sensor_write_register(ViPipe, 0xB8, 0x00);
  sensor_write_register(ViPipe, 0xB9, 0x00);
  sensor_write_register(ViPipe, 0xBA, 0x00);
  sensor_write_register(ViPipe, 0xBB, 0x00);
  sensor_write_register(ViPipe, 0xBC, 0x00);
  sensor_write_register(ViPipe, 0xBD, 0x00);
  sensor_write_register(ViPipe, 0xBE, 0x00);
  sensor_write_register(ViPipe, 0xBF, 0x00);
  sensor_write_register(ViPipe, 0xC0, 0x00);
  sensor_write_register(ViPipe, 0xC1, 0x00);
  sensor_write_register(ViPipe, 0xC2, 0x0B);
  sensor_write_register(ViPipe, 0xC3, 0x0C);
  sensor_write_register(ViPipe, 0xC4, 0x00);
  sensor_write_register(ViPipe, 0xC5, 0x00);
  sensor_write_register(ViPipe, 0xC6, 0x1F);
  sensor_write_register(ViPipe, 0xC7, 0x78);
  sensor_write_register(ViPipe, 0xC8, 0x27);
  sensor_write_register(ViPipe, 0xC9, 0x00);
  sensor_write_register(ViPipe, 0xCA, 0x00);
  sensor_write_register(ViPipe, 0xCB, 0x07);
  sensor_write_register(ViPipe, 0xCC, 0x08);
  sensor_write_register(ViPipe, 0xCD, 0x00);
  sensor_write_register(ViPipe, 0xCE, 0x00);
  sensor_write_register(ViPipe, 0xCF, 0x04);
  sensor_write_register(ViPipe, 0xD0, 0x00);
  sensor_write_register(ViPipe, 0xD1, 0x00);
  sensor_write_register(ViPipe, 0xD2, 0x60);
  sensor_write_register(ViPipe, 0xD3, 0x10);
  sensor_write_register(ViPipe, 0xD4, 0x06);
  sensor_write_register(ViPipe, 0xD5, 0xBE);
  sensor_write_register(ViPipe, 0xD6, 0x39);
  sensor_write_register(ViPipe, 0xD7, 0x27);
  sensor_write_register(ViPipe, 0xD8, 0x00);
  sensor_write_register(ViPipe, 0xD9, 0x00);
  sensor_write_register(ViPipe, 0xDA, 0x00);
  sensor_write_register(ViPipe, 0xDB, 0x00);
  sensor_write_register(ViPipe, 0xDC, 0x00);
  sensor_write_register(ViPipe, 0xDD, 0x00);
  sensor_write_register(ViPipe, 0xDE, 0x00);
  sensor_write_register(ViPipe, 0xDF, 0x00);
  sensor_write_register(ViPipe, 0xE0, 0x00);
  sensor_write_register(ViPipe, 0xE1, 0x00);
  sensor_write_register(ViPipe, 0xE2, 0x00);
  sensor_write_register(ViPipe, 0xE3, 0x00);
  sensor_write_register(ViPipe, 0xE4, 0x00);
  sensor_write_register(ViPipe, 0xE5, 0x00);
  sensor_write_register(ViPipe, 0xE6, 0x00);
  sensor_write_register(ViPipe, 0xE7, 0x13);
  sensor_write_register(ViPipe, 0xE8, 0x03);
  sensor_write_register(ViPipe, 0xE9, 0x00);
  sensor_write_register(ViPipe, 0xEA, 0x00);
  sensor_write_register(ViPipe, 0xEB, 0x00);
  sensor_write_register(ViPipe, 0xEC, 0x00);
  sensor_write_register(ViPipe, 0xED, 0x00);
  sensor_write_register(ViPipe, 0xEE, 0x00);
  sensor_write_register(ViPipe, 0xEF, 0x00);
  sensor_write_register(ViPipe, 0xF0, 0x00);
  sensor_write_register(ViPipe, 0xF1, 0x00);
  sensor_write_register(ViPipe, 0xF2, 0x00);
  sensor_write_register(ViPipe, 0xF3, 0x00);
  sensor_write_register(ViPipe, 0xF4, 0x20);
  sensor_write_register(ViPipe, 0xF5, 0x10);
  sensor_write_register(ViPipe, 0xF6, 0x00);
  sensor_write_register(ViPipe, 0xF7, 0x00);
  sensor_write_register(ViPipe, 0xF8, 0x00);
  sensor_write_register(ViPipe, 0xF9, 0x00);
  sensor_write_register(ViPipe, 0xFA, 0x0b);
  sensor_write_register(ViPipe, 0xFB, 0x00);
  sensor_write_register(ViPipe, 0xFC, 0x00);

  sensor_write_register(ViPipe, 0x40, 0x08);
  sensor_write_register(ViPipe, 0x00, 0x00);
  sensor_write_register(ViPipe, 0x01, 0xf8);
  sensor_write_register(ViPipe, 0x02, 0x01);
  sensor_write_register(ViPipe, 0x08, 0xf0);
  sensor_write_register(ViPipe, 0x13, 0x24);
  sensor_write_register(ViPipe, 0x14, 0x73);
  sensor_write_register(ViPipe, 0x15, 0x08);
  sensor_write_register(ViPipe, 0x20, 0x12);
  sensor_write_register(ViPipe, 0x34, 0x1b);
  sensor_write_register(ViPipe, 0x23, 0x02);
  sensor_write_register(ViPipe, 0x23, 0x00);

  sensor_write_register(ViPipe, 0x40, 0x00);


  printf("================================================================\n");
  printf("==== Techpoint TP9950 960H25fps(BT656) init success! ===========\n");
  printf("================================================================\n");
  return;
}

void tp9950_720p30_init(VI_PIPE ViPipe) {
  sensor_write_register(ViPipe, 0x0, 0x11);
  sensor_write_register(ViPipe, 0x1, 0x7a);
  sensor_write_register(ViPipe, 0x2, 0xce);
  sensor_write_register(ViPipe, 0x3, 0xd);
  sensor_write_register(ViPipe, 0x4, 0);
  sensor_write_register(ViPipe, 0x5, 0);
  sensor_write_register(ViPipe, 0x6, 0x32);
  sensor_write_register(ViPipe, 0x7, 0xc0);
  sensor_write_register(ViPipe, 0x8, 0);
  sensor_write_register(ViPipe, 0x9, 0x24);
  sensor_write_register(ViPipe, 0xa, 0x48);
  sensor_write_register(ViPipe, 0xb, 0xc0);
  sensor_write_register(ViPipe, 0xc, 0x3);
  sensor_write_register(ViPipe, 0xd, 0x71);
  sensor_write_register(ViPipe, 0xe, 0);
  sensor_write_register(ViPipe, 0xf, 0);
  sensor_write_register(ViPipe, 0x10, 0xd1);
  sensor_write_register(ViPipe, 0x11, 0x57);
  sensor_write_register(ViPipe, 0x12, 0x4f);
  sensor_write_register(ViPipe, 0x13, 0x80);
  sensor_write_register(ViPipe, 0x14, 0x1f);
  sensor_write_register(ViPipe, 0x15, 0x13);
  sensor_write_register(ViPipe, 0x16, 0x16);
  sensor_write_register(ViPipe, 0x17, 0);
  sensor_write_register(ViPipe, 0x18, 0x19);
  sensor_write_register(ViPipe, 0x19, 0xd0);
  sensor_write_register(ViPipe, 0x1a, 0x25);
  sensor_write_register(ViPipe, 0x1b, 0x1);
  sensor_write_register(ViPipe, 0x1c, 0x7);
  sensor_write_register(ViPipe, 0x1d, 0xbc);
  sensor_write_register(ViPipe, 0x1e, 0x80);
  sensor_write_register(ViPipe, 0x1f, 0x80);
  sensor_write_register(ViPipe, 0x20, 0x40);
  sensor_write_register(ViPipe, 0x21, 0x46);
  sensor_write_register(ViPipe, 0x22, 0x36);
  sensor_write_register(ViPipe, 0x23, 0x3c);
  sensor_write_register(ViPipe, 0x24, 0x4);
  sensor_write_register(ViPipe, 0x25, 0xfe);
  sensor_write_register(ViPipe, 0x26, 0x1);
  sensor_write_register(ViPipe, 0x27, 0x2d);
  sensor_write_register(ViPipe, 0x28, 0);
  sensor_write_register(ViPipe, 0x29, 0x48);
  sensor_write_register(ViPipe, 0x2a, 0x30);
  sensor_write_register(ViPipe, 0x2b, 0x60);
  sensor_write_register(ViPipe, 0x2c, 0x3a);
  sensor_write_register(ViPipe, 0x2d, 0x5a);
  sensor_write_register(ViPipe, 0x2e, 0x40);
  sensor_write_register(ViPipe, 0x2f, 0);
  sensor_write_register(ViPipe, 0x30, 0x9e);
  sensor_write_register(ViPipe, 0x31, 0x20);
  sensor_write_register(ViPipe, 0x32, 0x10);
  sensor_write_register(ViPipe, 0x33, 0x90);
  sensor_write_register(ViPipe, 0x34, 0);
  sensor_write_register(ViPipe, 0x35, 0x25);
  sensor_write_register(ViPipe, 0x36, 0xdc);
  sensor_write_register(ViPipe, 0x37, 0);
  sensor_write_register(ViPipe, 0x38, 0);
  sensor_write_register(ViPipe, 0x39, 0x18);
  sensor_write_register(ViPipe, 0x3a, 0x32);
  sensor_write_register(ViPipe, 0x3b, 0x26);
  sensor_write_register(ViPipe, 0x3c, 0);
  sensor_write_register(ViPipe, 0x3d, 0x60);
  sensor_write_register(ViPipe, 0x3e, 0);
  sensor_write_register(ViPipe, 0x3f, 0);
  sensor_write_register(ViPipe, 0x40, 0);
  sensor_write_register(ViPipe, 0x41, 0);
  sensor_write_register(ViPipe, 0x42, 0);
  sensor_write_register(ViPipe, 0x43, 0);
  sensor_write_register(ViPipe, 0x44, 0);
  sensor_write_register(ViPipe, 0x45, 0);
  sensor_write_register(ViPipe, 0x46, 0);
  sensor_write_register(ViPipe, 0x47, 0);
  sensor_write_register(ViPipe, 0x48, 0);
  sensor_write_register(ViPipe, 0x49, 0);
  sensor_write_register(ViPipe, 0x4a, 0);
  sensor_write_register(ViPipe, 0x4b, 0);
  sensor_write_register(ViPipe, 0x4c, 0x43);
  sensor_write_register(ViPipe, 0x4d, 0);
  sensor_write_register(ViPipe, 0x4e, 0x17);
  sensor_write_register(ViPipe, 0x4f, 0);
  sensor_write_register(ViPipe, 0x50, 0);
  sensor_write_register(ViPipe, 0x51, 0);
  sensor_write_register(ViPipe, 0x52, 0);
  sensor_write_register(ViPipe, 0x53, 0);
  sensor_write_register(ViPipe, 0x54, 0);
  sensor_write_register(ViPipe, 0x55, 0);
  sensor_write_register(ViPipe, 0x56, 0);
  sensor_write_register(ViPipe, 0x57, 0);
  sensor_write_register(ViPipe, 0x58, 0);
  sensor_write_register(ViPipe, 0x59, 0);
  sensor_write_register(ViPipe, 0x5a, 0);
  sensor_write_register(ViPipe, 0x5b, 0);
  sensor_write_register(ViPipe, 0x5c, 0);
  sensor_write_register(ViPipe, 0x5d, 0);
  sensor_write_register(ViPipe, 0x5e, 0);
  sensor_write_register(ViPipe, 0x5f, 0);
  sensor_write_register(ViPipe, 0x60, 0);
  sensor_write_register(ViPipe, 0x61, 0);
  sensor_write_register(ViPipe, 0x62, 0);
  sensor_write_register(ViPipe, 0x63, 0);
  sensor_write_register(ViPipe, 0x64, 0);
  sensor_write_register(ViPipe, 0x65, 0);
  sensor_write_register(ViPipe, 0x66, 0);
  sensor_write_register(ViPipe, 0x67, 0);
  sensor_write_register(ViPipe, 0x68, 0);
  sensor_write_register(ViPipe, 0x69, 0);
  sensor_write_register(ViPipe, 0x6a, 0);
  sensor_write_register(ViPipe, 0x6b, 0);
  sensor_write_register(ViPipe, 0x6c, 0);
  sensor_write_register(ViPipe, 0x6d, 0);
  sensor_write_register(ViPipe, 0x6e, 0);
  sensor_write_register(ViPipe, 0x6f, 0);
  sensor_write_register(ViPipe, 0x70, 0);
  sensor_write_register(ViPipe, 0x71, 0);
  sensor_write_register(ViPipe, 0x72, 0);
  sensor_write_register(ViPipe, 0x73, 0);
  sensor_write_register(ViPipe, 0x74, 0);
  sensor_write_register(ViPipe, 0x75, 0);
  sensor_write_register(ViPipe, 0x76, 0);
  sensor_write_register(ViPipe, 0x77, 0);
  sensor_write_register(ViPipe, 0x78, 0);
  sensor_write_register(ViPipe, 0x79, 0);
  sensor_write_register(ViPipe, 0x7a, 0);
  sensor_write_register(ViPipe, 0x7b, 0);
  sensor_write_register(ViPipe, 0x7c, 0);
  sensor_write_register(ViPipe, 0x7d, 0);
  sensor_write_register(ViPipe, 0x7e, 0);
  sensor_write_register(ViPipe, 0x7f, 0);
  sensor_write_register(ViPipe, 0x80, 0);
  sensor_write_register(ViPipe, 0x81, 0);
  sensor_write_register(ViPipe, 0x82, 0);
  sensor_write_register(ViPipe, 0x83, 0);
  sensor_write_register(ViPipe, 0x84, 0);
  sensor_write_register(ViPipe, 0x85, 0);
  sensor_write_register(ViPipe, 0x86, 0);
  sensor_write_register(ViPipe, 0x87, 0);
  sensor_write_register(ViPipe, 0x88, 0);
  sensor_write_register(ViPipe, 0x89, 0);
  sensor_write_register(ViPipe, 0x8a, 0);
  sensor_write_register(ViPipe, 0x8b, 0);
  sensor_write_register(ViPipe, 0x8c, 0);
  sensor_write_register(ViPipe, 0x8d, 0);
  sensor_write_register(ViPipe, 0x8e, 0);
  sensor_write_register(ViPipe, 0x8f, 0);
  sensor_write_register(ViPipe, 0x90, 0);
  sensor_write_register(ViPipe, 0x91, 0);
  sensor_write_register(ViPipe, 0x92, 0);
  sensor_write_register(ViPipe, 0x93, 0);
  sensor_write_register(ViPipe, 0x94, 0);
  sensor_write_register(ViPipe, 0x95, 0);
  sensor_write_register(ViPipe, 0x96, 0);
  sensor_write_register(ViPipe, 0x97, 0);
  sensor_write_register(ViPipe, 0x98, 0);
  sensor_write_register(ViPipe, 0x99, 0);
  sensor_write_register(ViPipe, 0x9a, 0);
  sensor_write_register(ViPipe, 0x9b, 0);
  sensor_write_register(ViPipe, 0x9c, 0);
  sensor_write_register(ViPipe, 0x9d, 0);
  sensor_write_register(ViPipe, 0x9e, 0);
  sensor_write_register(ViPipe, 0x9f, 0);
  sensor_write_register(ViPipe, 0xa0, 0);
  sensor_write_register(ViPipe, 0xa1, 0);
  sensor_write_register(ViPipe, 0xa2, 0);
  sensor_write_register(ViPipe, 0xa3, 0);
  sensor_write_register(ViPipe, 0xa4, 0);
  sensor_write_register(ViPipe, 0xa5, 0);
  sensor_write_register(ViPipe, 0xa6, 0);
  sensor_write_register(ViPipe, 0xa7, 0x3);
  sensor_write_register(ViPipe, 0xa8, 0);
  sensor_write_register(ViPipe, 0xa9, 0);
  sensor_write_register(ViPipe, 0xaa, 0);
  sensor_write_register(ViPipe, 0xab, 0);
  sensor_write_register(ViPipe, 0xac, 0);
  sensor_write_register(ViPipe, 0xad, 0);
  sensor_write_register(ViPipe, 0xae, 0);
  sensor_write_register(ViPipe, 0xaf, 0);
  sensor_write_register(ViPipe, 0xb0, 0);
  sensor_write_register(ViPipe, 0xb1, 0);
  sensor_write_register(ViPipe, 0xb2, 0);
  sensor_write_register(ViPipe, 0xb3, 0xfa);
  sensor_write_register(ViPipe, 0xb4, 0);
  sensor_write_register(ViPipe, 0xb5, 0);
  sensor_write_register(ViPipe, 0xb6, 0x10);
  sensor_write_register(ViPipe, 0xb7, 0);
  sensor_write_register(ViPipe, 0xb8, 0);
  sensor_write_register(ViPipe, 0xb9, 0);
  sensor_write_register(ViPipe, 0xba, 0);
  sensor_write_register(ViPipe, 0xbb, 0);
  sensor_write_register(ViPipe, 0xbc, 0);
  sensor_write_register(ViPipe, 0xbd, 0);
  sensor_write_register(ViPipe, 0xbe, 0);
  sensor_write_register(ViPipe, 0xbf, 0);
  sensor_write_register(ViPipe, 0xc0, 0);
  sensor_write_register(ViPipe, 0xc1, 0);
  sensor_write_register(ViPipe, 0xc2, 0xb);
  sensor_write_register(ViPipe, 0xc3, 0xc);
  sensor_write_register(ViPipe, 0xc4, 0);
  sensor_write_register(ViPipe, 0xc5, 0);
  sensor_write_register(ViPipe, 0xc6, 0x1f);
  sensor_write_register(ViPipe, 0xc7, 0x78);
  sensor_write_register(ViPipe, 0xc8, 0x27);
  sensor_write_register(ViPipe, 0xc9, 0);
  sensor_write_register(ViPipe, 0xca, 0);
  sensor_write_register(ViPipe, 0xcb, 0x7);
  sensor_write_register(ViPipe, 0xcc, 0x8);
  sensor_write_register(ViPipe, 0xcd, 0);
  sensor_write_register(ViPipe, 0xce, 0);
  sensor_write_register(ViPipe, 0xcf, 0x4);
  sensor_write_register(ViPipe, 0xd0, 0);
  sensor_write_register(ViPipe, 0xd1, 0);
  sensor_write_register(ViPipe, 0xd2, 0x60);
  sensor_write_register(ViPipe, 0xd3, 0x10);
  sensor_write_register(ViPipe, 0xd4, 0x6);
  sensor_write_register(ViPipe, 0xd5, 0xbe);
  sensor_write_register(ViPipe, 0xd6, 0x39);
  sensor_write_register(ViPipe, 0xd7, 0x27);
  sensor_write_register(ViPipe, 0xd8, 0);
  sensor_write_register(ViPipe, 0xd9, 0);
  sensor_write_register(ViPipe, 0xda, 0);
  sensor_write_register(ViPipe, 0xdb, 0);
  sensor_write_register(ViPipe, 0xdc, 0);
  sensor_write_register(ViPipe, 0xdd, 0);
  sensor_write_register(ViPipe, 0xde, 0);
  sensor_write_register(ViPipe, 0xdf, 0);
  sensor_write_register(ViPipe, 0xe0, 0);
  sensor_write_register(ViPipe, 0xe1, 0);
  sensor_write_register(ViPipe, 0xe2, 0);
  sensor_write_register(ViPipe, 0xe3, 0);
  sensor_write_register(ViPipe, 0xe4, 0);
  sensor_write_register(ViPipe, 0xe5, 0);
  sensor_write_register(ViPipe, 0xe6, 0);
  sensor_write_register(ViPipe, 0xe7, 0x13);
  sensor_write_register(ViPipe, 0xe8, 0x3);
  sensor_write_register(ViPipe, 0xe9, 0);
  sensor_write_register(ViPipe, 0xea, 0);
  sensor_write_register(ViPipe, 0xeb, 0);
  sensor_write_register(ViPipe, 0xec, 0);
  sensor_write_register(ViPipe, 0xed, 0);
  sensor_write_register(ViPipe, 0xee, 0);
  sensor_write_register(ViPipe, 0xef, 0);
  sensor_write_register(ViPipe, 0xf0, 0);
  sensor_write_register(ViPipe, 0xf1, 0);
  sensor_write_register(ViPipe, 0xf2, 0);
  sensor_write_register(ViPipe, 0xf3, 0);
  sensor_write_register(ViPipe, 0xf4, 0x20);
  sensor_write_register(ViPipe, 0xf5, 0x10);
  sensor_write_register(ViPipe, 0xf6, 0);
  sensor_write_register(ViPipe, 0xf7, 0);
  sensor_write_register(ViPipe, 0xf8, 0);
  sensor_write_register(ViPipe, 0xf9, 0);
  sensor_write_register(ViPipe, 0xfa, 0x88);
  sensor_write_register(ViPipe, 0xfb, 0);
  sensor_write_register(ViPipe, 0xfc, 0);
  sensor_write_register(ViPipe, 0xfd, 0x80);
  sensor_write_register(ViPipe, 0xfe, 0x28);

  printf("================================================================\n");
  printf("==== Techpoint TP9950 720P30fps(BT656) init success! ===========\n");
  printf("================================================================\n");
  return;
}

void tp9950_1080p30_init(VI_PIPE ViPipe) {
  sensor_write_register(ViPipe, 0x0, 0x11);
  sensor_write_register(ViPipe, 0x1, 0x7a);
  sensor_write_register(ViPipe, 0x2, 0xcc);
  sensor_write_register(ViPipe, 0x3, 0xd);
  sensor_write_register(ViPipe, 0x4, 0);
  sensor_write_register(ViPipe, 0x5, 0);
  sensor_write_register(ViPipe, 0x6, 0x32);
  sensor_write_register(ViPipe, 0x7, 0xc0);
  sensor_write_register(ViPipe, 0x8, 0xf0);
  sensor_write_register(ViPipe, 0x9, 0x24);
  sensor_write_register(ViPipe, 0xa, 0x48);
  sensor_write_register(ViPipe, 0xb, 0xc0);
  sensor_write_register(ViPipe, 0xc, 0x3);
  sensor_write_register(ViPipe, 0xd, 0x72);
  sensor_write_register(ViPipe, 0xe, 0);
  sensor_write_register(ViPipe, 0xf, 0);
  sensor_write_register(ViPipe, 0x10, 0xd1);
  sensor_write_register(ViPipe, 0x11, 0x57);
  sensor_write_register(ViPipe, 0x12, 0x60);
  sensor_write_register(ViPipe, 0x13, 0x04);
  sensor_write_register(ViPipe, 0x14, 0x73);
  sensor_write_register(ViPipe, 0x15, 0x01);
  sensor_write_register(ViPipe, 0x16, 0xf0);
  sensor_write_register(ViPipe, 0x17, 0x80);
  sensor_write_register(ViPipe, 0x18, 0x29);
  sensor_write_register(ViPipe, 0x19, 0x38);
  sensor_write_register(ViPipe, 0x1a, 0x47);
  sensor_write_register(ViPipe, 0x1b, 0x1);
  sensor_write_register(ViPipe, 0x1c, 0x8);
  sensor_write_register(ViPipe, 0x1d, 0x98);
  sensor_write_register(ViPipe, 0x1e, 0x80);
  sensor_write_register(ViPipe, 0x1f, 0x80);
  sensor_write_register(ViPipe, 0x20, 0x38);
  sensor_write_register(ViPipe, 0x21, 0x46);
  sensor_write_register(ViPipe, 0x22, 0x36);
  sensor_write_register(ViPipe, 0x23, 0x3c);
  sensor_write_register(ViPipe, 0x24, 0x4);
  sensor_write_register(ViPipe, 0x25, 0xfe);
  sensor_write_register(ViPipe, 0x26, 0xd);
  sensor_write_register(ViPipe, 0x27, 0x2d);
  sensor_write_register(ViPipe, 0x28, 0);
  sensor_write_register(ViPipe, 0x29, 0x48);
  sensor_write_register(ViPipe, 0x2a, 0x30);
  sensor_write_register(ViPipe, 0x2b, 0x60);
  sensor_write_register(ViPipe, 0x2c, 0x3a);
  sensor_write_register(ViPipe, 0x2d, 0x54);
  sensor_write_register(ViPipe, 0x2e, 0x40);
  sensor_write_register(ViPipe, 0x2f, 0);
  sensor_write_register(ViPipe, 0x30, 0xa5);
  sensor_write_register(ViPipe, 0x31, 0x95);
  sensor_write_register(ViPipe, 0x32, 0xe0);
  sensor_write_register(ViPipe, 0x33, 0x60);
  sensor_write_register(ViPipe, 0x34, 0);
  sensor_write_register(ViPipe, 0x35, 0x05);
  sensor_write_register(ViPipe, 0x36, 0xdc);
  sensor_write_register(ViPipe, 0x37, 0);
  sensor_write_register(ViPipe, 0x38, 0);
  sensor_write_register(ViPipe, 0x39, 0x1c);
  sensor_write_register(ViPipe, 0x3a, 0x32);
  sensor_write_register(ViPipe, 0x3b, 0x26);
  sensor_write_register(ViPipe, 0x3c, 0);
  sensor_write_register(ViPipe, 0x3d, 0x60);
  sensor_write_register(ViPipe, 0x3e, 0);
  sensor_write_register(ViPipe, 0x3f, 0);
  sensor_write_register(ViPipe, 0x40, 0);
  sensor_write_register(ViPipe, 0x41, 0);
  sensor_write_register(ViPipe, 0x42, 0);
  sensor_write_register(ViPipe, 0x43, 0);
  sensor_write_register(ViPipe, 0x44, 0);
  sensor_write_register(ViPipe, 0x45, 0);
  sensor_write_register(ViPipe, 0x46, 0);
  sensor_write_register(ViPipe, 0x47, 0);
  sensor_write_register(ViPipe, 0x48, 0);
  sensor_write_register(ViPipe, 0x49, 0);
  sensor_write_register(ViPipe, 0x4a, 0);
  sensor_write_register(ViPipe, 0x4b, 0);
  sensor_write_register(ViPipe, 0x4c, 0x43);
  sensor_write_register(ViPipe, 0x4d, 0);
  sensor_write_register(ViPipe, 0x4e, 0x17);
  sensor_write_register(ViPipe, 0x4f, 0);
  sensor_write_register(ViPipe, 0x50, 0);
  sensor_write_register(ViPipe, 0x51, 0);
  sensor_write_register(ViPipe, 0x52, 0);
  sensor_write_register(ViPipe, 0x53, 0);
  sensor_write_register(ViPipe, 0x54, 0);
  sensor_write_register(ViPipe, 0x55, 0);
  sensor_write_register(ViPipe, 0x56, 0);
  sensor_write_register(ViPipe, 0x57, 0);
  sensor_write_register(ViPipe, 0x58, 0);
  sensor_write_register(ViPipe, 0x59, 0);
  sensor_write_register(ViPipe, 0x5a, 0);
  sensor_write_register(ViPipe, 0x5b, 0);
  sensor_write_register(ViPipe, 0x5c, 0);
  sensor_write_register(ViPipe, 0x5d, 0);
  sensor_write_register(ViPipe, 0x5e, 0);
  sensor_write_register(ViPipe, 0x5f, 0);
  sensor_write_register(ViPipe, 0x60, 0);
  sensor_write_register(ViPipe, 0x61, 0);
  sensor_write_register(ViPipe, 0x62, 0);
  sensor_write_register(ViPipe, 0x63, 0);
  sensor_write_register(ViPipe, 0x64, 0);
  sensor_write_register(ViPipe, 0x65, 0);
  sensor_write_register(ViPipe, 0x66, 0);
  sensor_write_register(ViPipe, 0x67, 0);
  sensor_write_register(ViPipe, 0x68, 0);
  sensor_write_register(ViPipe, 0x69, 0);
  sensor_write_register(ViPipe, 0x6a, 0);
  sensor_write_register(ViPipe, 0x6b, 0);
  sensor_write_register(ViPipe, 0x6c, 0);
  sensor_write_register(ViPipe, 0x6d, 0);
  sensor_write_register(ViPipe, 0x6e, 0);
  sensor_write_register(ViPipe, 0x6f, 0);
  sensor_write_register(ViPipe, 0x70, 0);
  sensor_write_register(ViPipe, 0x71, 0);
  sensor_write_register(ViPipe, 0x72, 0);
  sensor_write_register(ViPipe, 0x73, 0);
  sensor_write_register(ViPipe, 0x74, 0);
  sensor_write_register(ViPipe, 0x75, 0);
  sensor_write_register(ViPipe, 0x76, 0);
  sensor_write_register(ViPipe, 0x77, 0);
  sensor_write_register(ViPipe, 0x78, 0);
  sensor_write_register(ViPipe, 0x79, 0);
  sensor_write_register(ViPipe, 0x7a, 0);
  sensor_write_register(ViPipe, 0x7b, 0);
  sensor_write_register(ViPipe, 0x7c, 0);
  sensor_write_register(ViPipe, 0x7d, 0);
  sensor_write_register(ViPipe, 0x7e, 0);
  sensor_write_register(ViPipe, 0x7f, 0);
  sensor_write_register(ViPipe, 0x80, 0);
  sensor_write_register(ViPipe, 0x81, 0);
  sensor_write_register(ViPipe, 0x82, 0);
  sensor_write_register(ViPipe, 0x83, 0);
  sensor_write_register(ViPipe, 0x84, 0);
  sensor_write_register(ViPipe, 0x85, 0);
  sensor_write_register(ViPipe, 0x86, 0);
  sensor_write_register(ViPipe, 0x87, 0);
  sensor_write_register(ViPipe, 0x88, 0);
  sensor_write_register(ViPipe, 0x89, 0);
  sensor_write_register(ViPipe, 0x8a, 0);
  sensor_write_register(ViPipe, 0x8b, 0);
  sensor_write_register(ViPipe, 0x8c, 0);
  sensor_write_register(ViPipe, 0x8d, 0);
  sensor_write_register(ViPipe, 0x8e, 0);
  sensor_write_register(ViPipe, 0x8f, 0);
  sensor_write_register(ViPipe, 0x90, 0);
  sensor_write_register(ViPipe, 0x91, 0);
  sensor_write_register(ViPipe, 0x92, 0);
  sensor_write_register(ViPipe, 0x93, 0);
  sensor_write_register(ViPipe, 0x94, 0);
  sensor_write_register(ViPipe, 0x95, 0);
  sensor_write_register(ViPipe, 0x96, 0);
  sensor_write_register(ViPipe, 0x97, 0);
  sensor_write_register(ViPipe, 0x98, 0);
  sensor_write_register(ViPipe, 0x99, 0);
  sensor_write_register(ViPipe, 0x9a, 0);
  sensor_write_register(ViPipe, 0x9b, 0);
  sensor_write_register(ViPipe, 0x9c, 0);
  sensor_write_register(ViPipe, 0x9d, 0);
  sensor_write_register(ViPipe, 0x9e, 0);
  sensor_write_register(ViPipe, 0x9f, 0);
  sensor_write_register(ViPipe, 0xa0, 0);
  sensor_write_register(ViPipe, 0xa1, 0);
  sensor_write_register(ViPipe, 0xa2, 0);
  sensor_write_register(ViPipe, 0xa3, 0);
  sensor_write_register(ViPipe, 0xa4, 0);
  sensor_write_register(ViPipe, 0xa5, 0);
  sensor_write_register(ViPipe, 0xa6, 0);
  sensor_write_register(ViPipe, 0xa7, 0x3);
  sensor_write_register(ViPipe, 0xa8, 0);
  sensor_write_register(ViPipe, 0xa9, 0);
  sensor_write_register(ViPipe, 0xaa, 0);
  sensor_write_register(ViPipe, 0xab, 0);
  sensor_write_register(ViPipe, 0xac, 0);
  sensor_write_register(ViPipe, 0xad, 0);
  sensor_write_register(ViPipe, 0xae, 0);
  sensor_write_register(ViPipe, 0xaf, 0);
  sensor_write_register(ViPipe, 0xb0, 0);
  sensor_write_register(ViPipe, 0xb1, 0);
  sensor_write_register(ViPipe, 0xb2, 0);
  sensor_write_register(ViPipe, 0xb3, 0xfa);
  sensor_write_register(ViPipe, 0xb4, 0);
  sensor_write_register(ViPipe, 0xb5, 0);
  sensor_write_register(ViPipe, 0xb6, 0x10);
  sensor_write_register(ViPipe, 0xb7, 0);
  sensor_write_register(ViPipe, 0xb8, 0);
  sensor_write_register(ViPipe, 0xb9, 0);
  sensor_write_register(ViPipe, 0xba, 0);
  sensor_write_register(ViPipe, 0xbb, 0);
  sensor_write_register(ViPipe, 0xbc, 0);
  sensor_write_register(ViPipe, 0xbd, 0);
  sensor_write_register(ViPipe, 0xbe, 0);
  sensor_write_register(ViPipe, 0xbf, 0);
  sensor_write_register(ViPipe, 0xc0, 0);
  sensor_write_register(ViPipe, 0xc1, 0);
  sensor_write_register(ViPipe, 0xc2, 0xb);
  sensor_write_register(ViPipe, 0xc3, 0xc);
  sensor_write_register(ViPipe, 0xc4, 0);
  sensor_write_register(ViPipe, 0xc5, 0);
  sensor_write_register(ViPipe, 0xc6, 0x1f);
  sensor_write_register(ViPipe, 0xc7, 0x78);
  sensor_write_register(ViPipe, 0xc8, 0x27);
  sensor_write_register(ViPipe, 0xc9, 0);
  sensor_write_register(ViPipe, 0xca, 0);
  sensor_write_register(ViPipe, 0xcb, 0x7);
  sensor_write_register(ViPipe, 0xcc, 0x8);
  sensor_write_register(ViPipe, 0xcd, 0);
  sensor_write_register(ViPipe, 0xce, 0);
  sensor_write_register(ViPipe, 0xcf, 0x4);
  sensor_write_register(ViPipe, 0xd0, 0);
  sensor_write_register(ViPipe, 0xd1, 0);
  sensor_write_register(ViPipe, 0xd2, 0x60);
  sensor_write_register(ViPipe, 0xd3, 0x10);
  sensor_write_register(ViPipe, 0xd4, 0x6);
  sensor_write_register(ViPipe, 0xd5, 0xbe);
  sensor_write_register(ViPipe, 0xd6, 0x39);
  sensor_write_register(ViPipe, 0xd7, 0x27);
  sensor_write_register(ViPipe, 0xd8, 0);
  sensor_write_register(ViPipe, 0xd9, 0);
  sensor_write_register(ViPipe, 0xda, 0);
  sensor_write_register(ViPipe, 0xdb, 0);
  sensor_write_register(ViPipe, 0xdc, 0);
  sensor_write_register(ViPipe, 0xdd, 0);
  sensor_write_register(ViPipe, 0xde, 0);
  sensor_write_register(ViPipe, 0xdf, 0);
  sensor_write_register(ViPipe, 0xe0, 0);
  sensor_write_register(ViPipe, 0xe1, 0);
  sensor_write_register(ViPipe, 0xe2, 0);
  sensor_write_register(ViPipe, 0xe3, 0);
  sensor_write_register(ViPipe, 0xe4, 0);
  sensor_write_register(ViPipe, 0xe5, 0);
  sensor_write_register(ViPipe, 0xe6, 0);
  sensor_write_register(ViPipe, 0xe7, 0x13);
  sensor_write_register(ViPipe, 0xe8, 0x3);
  sensor_write_register(ViPipe, 0xe9, 0);
  sensor_write_register(ViPipe, 0xea, 0);
  sensor_write_register(ViPipe, 0xeb, 0);
  sensor_write_register(ViPipe, 0xec, 0);
  sensor_write_register(ViPipe, 0xed, 0);
  sensor_write_register(ViPipe, 0xee, 0);
  sensor_write_register(ViPipe, 0xef, 0);
  sensor_write_register(ViPipe, 0xf0, 0);
  sensor_write_register(ViPipe, 0xf1, 0);
  sensor_write_register(ViPipe, 0xf2, 0);
  sensor_write_register(ViPipe, 0xf3, 0);
  sensor_write_register(ViPipe, 0xf4, 0x20);
  sensor_write_register(ViPipe, 0xf5, 0x10);
  sensor_write_register(ViPipe, 0xf6, 0);
  sensor_write_register(ViPipe, 0xf7, 0);
  sensor_write_register(ViPipe, 0xf8, 0);
  sensor_write_register(ViPipe, 0xf9, 0);
  sensor_write_register(ViPipe, 0xfa, 0x88);
  sensor_write_register(ViPipe, 0xfb, 0);
  sensor_write_register(ViPipe, 0xfc, 0);
  sensor_write_register(ViPipe, 0xfd, 0x80);
  sensor_write_register(ViPipe, 0xfe, 0x28);

  printf("================================================================\n");
  printf("==== Techpoint TP9950 1080P30fps(BT656) init success! ==========\n");
  printf("================================================================\n");
  return;
}
