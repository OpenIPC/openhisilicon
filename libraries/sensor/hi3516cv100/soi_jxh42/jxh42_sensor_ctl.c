#include <stdio.h>
// #include <sys/types.h>
// #include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include "hi_i2c.h"

int sensor_write_register(int addr, int data)
{
    int fd = open("/dev/hi_i2c", 0);
    if (fd < 0)
    {
        fprintf(stderr, "Open /dev/hi_i2c error!");
        return -1;
    }

    I2C_DATA_S i2c_data;    
    i2c_data.data = data;
    i2c_data.dev_addr = 0x60;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = 1;
    i2c_data.data_byte_num = 1;

    //fprintf(stderr, "DBG: sensor_write_register(0x%x, 0x%x)\n", i2c_data.reg_addr, i2c_data.data);

    int ret = ioctl(fd, CMD_I2C_WRITE, &i2c_data);    
    if (ret)
    {
        fprintf(stderr, "\tERROR write!\n");
    }

    close(fd);

    return ret;
}

void sensor_init()
{
    sensor_write_register(0x12, 0x40);
    sensor_write_register(0xD, 0x40);
    sensor_write_register(0x1F, 4);
    sensor_write_register(0xE, 0x1D);
    sensor_write_register(0xF, 9);
    sensor_write_register(0x10, 0x1E);
    sensor_write_register(0x11, 0x80);
    sensor_write_register(0x20, 0x40);
    sensor_write_register(0x21, 6);
    sensor_write_register(0x22, 0xF0);
    sensor_write_register(0x23, 2);
    sensor_write_register(0x24, 0);
    sensor_write_register(0x25, 0xDC);
    sensor_write_register(0x26, 0x25);
    sensor_write_register(0x27, 0x3B);
    sensor_write_register(0x28, 9);
    sensor_write_register(0x29, 1);
    sensor_write_register(0x2A, 0x24);
    sensor_write_register(0x2B, 0x29);
    sensor_write_register(0x2C, 4);
    sensor_write_register(0x2D, 0);
    sensor_write_register(0x2E, 0xBB);
    sensor_write_register(0x2F, 0);
    sensor_write_register(0x30, 0x92);
    sensor_write_register(0x31, 0xA);
    sensor_write_register(0x32, 0xAA);
    sensor_write_register(0x33, 0x14);
    sensor_write_register(0x34, 0x38);
    sensor_write_register(0x35, 0x54);
    sensor_write_register(0x42, 0x41);
    sensor_write_register(0x43, 0x50);
    sensor_write_register(0x1D, 0xFF);
    sensor_write_register(0x1E, 0x9F);
    sensor_write_register(0x6C, 0x90);
    sensor_write_register(0x73, 0xB3);
    sensor_write_register(0x70, 0x68);
    sensor_write_register(0x76, 0x40);
    sensor_write_register(0x77, 6);
    sensor_write_register(0x60, 0xA4);
    sensor_write_register(0x61, 0xFF);
    sensor_write_register(0x62, 0x40);
    sensor_write_register(0x63, 0x51);
    sensor_write_register(0x65, 0);
    sensor_write_register(0x66, 0x20);
    sensor_write_register(0x67, 0x30);
    sensor_write_register(0x68, 4);
    sensor_write_register(0x69, 0x74);
    sensor_write_register(0x6A, 9);
    sensor_write_register(0x6F, 4);
    sensor_write_register(0x13, 0x87);
    sensor_write_register(0x14, 0x80);
    sensor_write_register(0x16, 0xC0);
    sensor_write_register(0x17, 0x40);
    sensor_write_register(0x18, 0xE1);
    sensor_write_register(0x38, 0x38);
    sensor_write_register(0x39, 0x92);
    sensor_write_register(0x4A, 3);
    sensor_write_register(0x48, 0x40);
    sensor_write_register(0x49, 4);
    sensor_write_register(0xD, 0x40);
    sensor_write_register(0x12, 0);
    puts("soih42 sensor 720p init success!");
}

