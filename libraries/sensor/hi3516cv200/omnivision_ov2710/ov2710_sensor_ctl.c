#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include "hi_comm_video.h"

#ifdef HI_GPIO_I2C
#include "gpioi2c_ex.h"
#else
#include "hi_i2c.h"
#endif

const unsigned int sensor_i2c_addr	=	0x6c;		/* I2C Address of OV2710 */
const unsigned int read_dev_addr	=	0x6d;
const unsigned int sensor_addr_byte	=	2;
const unsigned int sensor_data_byte	=	1;
static int g_fd = -1;
static int flag_init = 0;

extern WDR_MODE_E genSensorMode;
extern HI_U8 gu8SensorImageMode;
extern HI_BOOL bSensorInit;

int sensor_i2c_init(void)
{
    if(g_fd >= 0)
    {
        return 0;
    }
#ifdef HI_GPIO_I2C
    int ret;

    g_fd = open("/dev/gpioi2c_ex", 0);
    if(g_fd < 0)
    {
        printf("Open gpioi2c_ex error!\n");
        return -1;
    }
#else
    int ret;

    g_fd = open("/dev/i2c-0", O_RDWR);
    if(g_fd < 0)
    {
        printf("Open /dev/i2c-0 error!\n");
        return -1;
    }

    ret = ioctl(g_fd, I2C_SLAVE_FORCE, sensor_i2c_addr);
    if (ret < 0)
    {
        printf("CMD_SET_DEV error!\n");
        return ret;
    }
#endif

    return 0;
}

int sensor_i2c_exit(void)
{
    if (g_fd >= 0)
    {
        close(g_fd);
        g_fd = -1;
        return 0;
    }
    return -1;
}

int sensor_read_register(int addr)
{
    if(flag_init == 0)
    {
        sensor_i2c_init();
        flag_init = 1;
    }

    int ret = ioctl(g_fd, I2C_SLAVE_FORCE, read_dev_addr);
    if (ret < 0)
    {
        printf("ioctl device addr[%#x] error.\n", read_dev_addr);
        return -1;
    }

    if (sensor_addr_byte == 2)
        ioctl(g_fd, I2C_16BIT_REG, 1);
    else
        ioctl(g_fd, I2C_16BIT_REG, 0);

    if (sensor_data_byte == 2)
        ioctl(g_fd, I2C_16BIT_DATA, 1);
    else
        ioctl(g_fd, I2C_16BIT_DATA, 0);

    char recvbuf[4];
    memset(recvbuf, 0, sizeof(recvbuf));

    if (sensor_addr_byte == 2)
    {
        recvbuf[0] = addr & 0xff;
        recvbuf[1] = (addr >> 8) & 0xff;
    }
    else
        recvbuf[0] = addr & 0xff;

    ret = read(g_fd, recvbuf, sensor_addr_byte);
    if (ret < 0)
    {
        printf("CMD_I2C_READ error!\n");
        return -1;
    }

    int data = 0;
    if (sensor_data_byte == 2)
        data = recvbuf[0] | (recvbuf[1] << 8);
    else
        data = recvbuf[0];

    return data;

}

int sensor_write_register(int addr, int data)
{
#ifdef HI_GPIO_I2C
    i2c_data.dev_addr = sensor_i2c_addr;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = sensor_addr_byte;
    i2c_data.data = data;
    i2c_data.data_byte_num = sensor_data_byte;

    ret = ioctl(g_fd, GPIO_I2C_WRITE, &i2c_data);

    if (ret)
    {
        printf("GPIO-I2C write faild!\n");
        return ret;
    }
#else
    if(flag_init == 0)
    {
	    sensor_i2c_init();
	    flag_init = 1;
    }

    int idx = 0;
    int ret;
    char buf[8];

    ret = ioctl(g_fd, I2C_SLAVE_FORCE, sensor_i2c_addr);
    if (ret < 0)
    {
        printf("ioctl device addr[%#x] error.\n", sensor_i2c_addr);
        return -1;
    }

    buf[idx++] = addr & 0xFF;
    if (sensor_addr_byte == 2)
    {
    	ret = ioctl(g_fd, I2C_16BIT_REG, 1);
        buf[idx++] = addr >> 8;
    }
    else
    {
    	ret = ioctl(g_fd, I2C_16BIT_REG, 0);
    }

    if (ret < 0)
    {
        printf("CMD_SET_REG_WIDTH error!\n");
        return -1;
    }

    buf[idx++] = data;
    if (sensor_data_byte == 2)
    {
    	ret = ioctl(g_fd, I2C_16BIT_DATA, 1);
        buf[idx++] = data >> 8;
    }
    else
    {
    	ret = ioctl(g_fd, I2C_16BIT_DATA, 0);
    }

    if (ret)
    {
        printf("hi_i2c write faild!\n");
        return -1;
    }

    ret = write(g_fd, buf, idx);
    if(ret < 0)
    {
    	printf("I2C_WRITE error!\n");
    	return -1;
    }
#endif
	return 0;
}


void sensor_linear_1080p30_init();

void sensor_init()
{
    sensor_i2c_init();

    sensor_linear_1080p30_init();

    return ;
}

void sensor_exit()
{
    sensor_i2c_exit();
	flag_init = 0;
    return;
}

/* 1080P30*/
void sensor_linear_1080p30_init()
{
    sensor_write_register(0x3103,0x93);
    sensor_write_register(0x3008,0x82);
    usleep(10*1000);
    sensor_write_register(0x3008,0x42);
    sensor_write_register(0x3017,0x00);
    sensor_write_register(0x3018,0x00);
    sensor_write_register(0x3706,0x61);
    sensor_write_register(0x3712,0x0c);
    sensor_write_register(0x3630,0x6d);
    sensor_write_register(0x3801,0xb4);
    sensor_write_register(0x3621,0x04);
    sensor_write_register(0x3604,0x60);
    sensor_write_register(0x3603,0xa7);
    sensor_write_register(0x3631,0x26);
    sensor_write_register(0x3600,0x04);
    sensor_write_register(0x3620,0x37);
    sensor_write_register(0x3623,0x00);
    sensor_write_register(0x3702,0x9e);
    sensor_write_register(0x3703,0x5c);
    sensor_write_register(0x3704,0x40);
    sensor_write_register(0x370d,0x0f);
    sensor_write_register(0x3713,0x9f);
    sensor_write_register(0x3714,0x4c);
    sensor_write_register(0x3710,0x9e);
    sensor_write_register(0x3801,0xc4);
    sensor_write_register(0x3605,0x05);
    sensor_write_register(0x3606,0x3f);
    sensor_write_register(0x302d,0x90);
    sensor_write_register(0x370b,0x40);
    sensor_write_register(0x3716,0x31);
    sensor_write_register(0x3707,0x52);
    sensor_write_register(0x380d,0x74);
    sensor_write_register(0x5181,0x20);
    sensor_write_register(0x518f,0x00);
    sensor_write_register(0x4301,0xff);
    sensor_write_register(0x4303,0x00);
    sensor_write_register(0x3a00,0x78);
    //sensor_write_register(0x300f,0x88);
    //sensor_write_register(0x3011,0x28);
    sensor_write_register(0x3a1a,0x06);
    sensor_write_register(0x3a18,0x00);
    sensor_write_register(0x3a19,0x7a);
    sensor_write_register(0x3a13,0x54);
    sensor_write_register(0x382e,0x0f);
    sensor_write_register(0x381a,0x1a);
    sensor_write_register(0x401d,0x02);
    sensor_write_register(0x5688,0x03);
    sensor_write_register(0x5684,0x07);
    sensor_write_register(0x5685,0xa0);
    sensor_write_register(0x5686,0x04);
    sensor_write_register(0x5687,0x43);

    //set for mipi
    sensor_write_register(0x3011,0x0a);
    sensor_write_register(0x300f,0xc3);

    usleep(10*1000);

    sensor_write_register(0x300e,0x04);
    sensor_write_register(0x3030,0x2b);
    sensor_write_register(0x4801,0x0f);

    sensor_write_register(0x3a0f,0x40);
    sensor_write_register(0x3a10,0x38);
    sensor_write_register(0x3a1b,0x48);
   // sensor_write_register(0x3a13,0x30);
    sensor_write_register(0x3a1e,0x30);
    sensor_write_register(0x3a11,0x90);
    sensor_write_register(0x3a1f,0x10);

    //set frame rate
    sensor_write_register(0x3010,0x00);       //0x20,10fps;0x10,15fps;0x00,30fps

    //set AEC/AGC TO mannual model
    sensor_write_register(0x3503,0x07);

    //To close AE set:
    sensor_write_register(0x3503,0x7);
    sensor_write_register(0x3501,0x2e);
    sensor_write_register(0x3502,0x00);
    sensor_write_register(0x350b,0x10);

    //To Close AWB set:
    sensor_write_register(0x5001,0x4f);
    sensor_write_register(0x3406,0x01);
    sensor_write_register(0x3400,0x04);
    sensor_write_register(0x3401,0x00);
    sensor_write_register(0x3402,0x04);
    sensor_write_register(0x3403,0x00);
    sensor_write_register(0x3404,0x04);
    sensor_write_register(0x3405,0x00);

		/* modified blc */
		sensor_write_register(0x4000,0x05);

		/*drive capacity*/
		sensor_write_register(0x302c,0x00);

    //close shading
    sensor_write_register(0x5000,0x5f);   //df bit[8]=0

    sensor_write_register(0x3008,0x02);

    bSensorInit = HI_TRUE;
    printf("======================================================================\n");
    printf("===omnivision ov2710 sensor 1080P30fps(mipi port) init success!===\n");
    printf("======================================================================\n");
    return;
}


