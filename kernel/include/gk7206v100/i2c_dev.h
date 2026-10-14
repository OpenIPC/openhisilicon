#ifndef _I2C_DEV_H_
#define _I2C_DEV_H_

#include "xmedia_type.h"
#include "xmedia_errcode.h"

xmedia_s32 i2c_open(xmedia_u8 i2c_dev);
xmedia_s32 i2c_close(xmedia_s32 fd);
xmedia_s32 i2c_set_slave_addr(xmedia_s32 fd, xmedia_u16 slave_addr);
xmedia_s32 i2c_read(xmedia_s32 fd, xmedia_u16 slave_addr, xmedia_u32 read_addr,
                        xmedia_u8 addr_width,  xmedia_u8 *buff, xmedia_u32 length);
xmedia_s32 i2c_write(xmedia_s32 fd, xmedia_u16 slave_addr, xmedia_u8 *buff, xmedia_u32 length);

#endif

