#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include "hi_comm_video.h"
#include "hi_sns_ctrl.h"

#ifdef HI_GPIO_I2C
#include "gpioi2c_ex.h"
#else
#include "hi_i2c.h"
#endif

const unsigned char imx290_i2c_addr     =    0x34;        /* I2C Address of IMX290 */
const unsigned int  imx290_addr_byte    =    2;
const unsigned int  imx290_data_byte    =    1;
static int g_fd[ISP_MAX_DEV_NUM] = {-1, -1};

extern ISP_SNS_STATE_S              g_astimx290[ISP_MAX_DEV_NUM];
extern ISP_SNS_COMMBUS_U      g_aunImx290BusInfo[];


//sensor fps mode
#define IMX290_SENSOR_1080P_30FPS_LINEAR_MODE      (1)
#define IMX290_SENSOR_1080P_30FPS_3t1_WDR_MODE     (2)
#define IMX290_SENSOR_1080P_30FPS_2t1_WDR_MODE     (3)

int imx290_i2c_init(ISP_DEV IspDev)
{
    char acDevFile[16] = {0};
    HI_U8 u8DevNum;
    
    if(g_fd[IspDev] >= 0)
    {
        return 0;
    }    
#ifdef HI_GPIO_I2C
    int ret;

    g_fd[IspDev] = open("/dev/gpioi2c_ex", 0);
    if(g_fd[IspDev] < 0)
    {
        printf("Open gpioi2c_ex error!\n");
        return -1;
    }
#else
    int ret;

    u8DevNum = g_aunImx290BusInfo[IspDev].s8I2cDev;
    snprintf_s(acDevFile, sizeof(acDevFile), sizeof(acDevFile)-1, "/dev/i2c-%d", u8DevNum);
    
    g_fd[IspDev] = open(acDevFile, O_RDWR);
    if(g_fd[IspDev] < 0)
    {
        printf("Open /dev/i2c-%d error!\n", IspDev);
        return -1;
    }

    ret = ioctl(g_fd[IspDev], I2C_SLAVE_FORCE, (imx290_i2c_addr>>1));
    if (ret < 0)
    {
        printf("CMD_SET_DEV error!\n");
        return ret;
    }
#endif

    return 0;
}

int imx290_i2c_exit(ISP_DEV IspDev)
{
    if (g_fd[IspDev] >= 0)
    {
        close(g_fd[IspDev]);
        g_fd[IspDev] = -1;
        return 0;
    }
    return -1;
}

int imx290_read_register(ISP_DEV IspDev,int addr)
{
    // TODO: 
    
    return 0;
}

#ifdef __HuaweiLite__
int imx290_write_register(ISP_DEV IspDev,int addr, int data)
{
    if (0 > g_fd[IspDev])
    {
        return 0;
    }
#ifdef HI_GPIO_I2C
    i2c_data.dev_addr = imx290_i2c_addr;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = imx290_addr_byte;
    i2c_data.data = data;
    i2c_data.data_byte_num = imx290_data_byte;
    ret = ioctl(g_fd[IspDev], GPIO_I2C_WRITE, &i2c_data);
    if (ret)
    {
        printf("GPIO-I2C write faild!\n");
        return ret;
    }
#else
    int idx = 0;
    int ret;
    char buf[8];
    buf[idx++] = addr & 0xff;
    if (imx290_addr_byte == 2)
    {
        ret = ioctl(g_fd[IspDev], I2C_16BIT_REG, 1);
        buf[idx++] = (addr >> 8) & 0xff;
    }
    else
    {
        //ret = ioctl(g_fd[IspDev], I2C_16BIT_REG, 0);
    }
    buf[idx++] = data & 0xff;
    if (imx290_data_byte == 2)
    {
        //ret = ioctl(g_fd[IspDev], I2C_16BIT_DATA, 1);
        //buf[idx++] = (data >> 8) & 0xff;
    }
    else
    {
        ret = ioctl(g_fd[IspDev], I2C_16BIT_DATA, 0);
    }
    ret = write(g_fd[IspDev], buf, imx290_addr_byte + imx290_data_byte);
    if(ret < 0)
    {
        printf("I2C_WRITE error!\n");
        return -1;
    }
#endif
    return 0;
}
#else
int imx290_write_register(ISP_DEV IspDev,int addr, int data)
{
    if (0 > g_fd[IspDev])
    {
        return 0;
    }

#ifdef HI_GPIO_I2C
    i2c_data.dev_addr = imx290_i2c_addr;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte_num = imx290_addr_byte;
    i2c_data.data = data;
    i2c_data.data_byte_num = imx290_data_byte;

    ret = ioctl(g_fd[IspDev], GPIO_I2C_WRITE, &i2c_data);

    if (ret)
    {
        printf("GPIO-I2C write faild!\n");
        return ret;
    }
#else
    int idx = 0;
    int ret;
    char buf[8];

    if (imx290_addr_byte == 2)
    {
        buf[idx] = (addr >> 8) & 0xff;
        idx++;
        buf[idx] = addr & 0xff;
        idx++;
    }
    else
    {
        //buf[idx] = addr & 0xff;
        //idx++;
    }

    if (imx290_data_byte == 2)
    {
        //buf[idx] = (data >> 8) & 0xff;
        //idx++;
        //buf[idx] = data & 0xff;
        //idx++;
    }
    else
    {
        buf[idx] = data & 0xff;
        idx++;
    }
    
    ret = write(g_fd[IspDev], buf, imx290_addr_byte + imx290_data_byte);
    if(ret < 0)
    {
        printf("I2C_WRITE error!\n");
        return -1;
    }

#endif
    return 0;
}
#endif

static void delay_ms(int ms) { 
    hi_usleep(ms*1000);
}

void imx290_prog(ISP_DEV IspDev,int* rom) 
{
    int i = 0;
    while (1) {
        int lookup = rom[i++];
        int addr = (lookup >> 16) & 0xFFFF;
        int data = lookup & 0xFFFF;
        if (addr == 0xFFFE) {
            delay_ms(data);
        } else if (addr == 0xFFFF) {
            return;
        } else {
            imx290_write_register(IspDev,addr, data);
        }
    }
}

void imx290_standby(ISP_DEV IspDev)
{
    // TODO:
    return;
}

void imx290_restart(ISP_DEV IspDev)
{
    // TODO:
    return;
}

void imx290_wdr_1080p30_2to1_init(ISP_DEV IspDev);
void imx290_wdr_1080p60_2to1_init(ISP_DEV IspDev);
void imx290_wdr_1080p120_2to1_init(ISP_DEV IspDev);
void imx290_wdr_720p60_2to1_init(ISP_DEV IspDev);
void imx290_wdr_1080p30_3to1_init(ISP_DEV IspDev);
void imx290_wdr_1080p120_3to1_init(ISP_DEV IspDev);
void imx290_wdr_720p60_3to1_init(ISP_DEV IspDev);
void imx290_linear_1080p30_init(ISP_DEV IspDev);


void imx290_init(ISP_DEV IspDev)
{
	HI_U32           i;
    WDR_MODE_E       enWDRMode;
    HI_BOOL          bInit;

    bInit       = g_astimx290[IspDev].bInit;
    enWDRMode   = g_astimx290[IspDev].enWDRMode;
    imx290_i2c_init(IspDev);
    
    /* When sensor first init, config all registers */
    if (HI_FALSE == bInit) 
    {
        if (WDR_MODE_2To1_LINE == enWDRMode)
        {
            imx290_wdr_1080p60_2to1_init(IspDev);
        }
        else if (WDR_MODE_3To1_LINE == enWDRMode)
        {
            imx290_wdr_1080p120_3to1_init(IspDev);
        }
        else
        {
            imx290_linear_1080p30_init(IspDev);
        }
    }
    /* When sensor switch mode(linear<->WDR or resolution), config different registers(if possible) */
    else 
    {
        if(WDR_MODE_2To1_LINE == enWDRMode)
        {
            imx290_wdr_1080p60_2to1_init(IspDev);
        }
        
        else if(WDR_MODE_3To1_LINE == enWDRMode)
        {
            imx290_wdr_1080p120_3to1_init(IspDev);
        }
        
        else
        {
            imx290_linear_1080p30_init(IspDev);
        }       
    }

	for (i=0; i<g_astimx290[IspDev].astRegsInfo[0].u32RegNum; i++)
	{
		imx290_write_register(IspDev, g_astimx290[IspDev].astRegsInfo[0].astI2cData[i].u32RegAddr, g_astimx290[IspDev].astRegsInfo[0].astI2cData[i].u32Data);
	}
	
    g_astimx290[IspDev].bInit = HI_TRUE;
    return ;
}

void imx290_exit(ISP_DEV IspDev)
{
    imx290_i2c_exit(IspDev);

    return;
}


/* 1080P30 and 1080P25 */
void imx290_linear_1080p30_init(ISP_DEV IspDev)
{
    imx290_write_register (IspDev,0x3000, 0x01); /* standby */
    imx290_write_register (IspDev,0x3002, 0x01); /* XTMSTA */
    
    imx290_write_register (IspDev,0x3005, 0x01); //ADBIT
    imx290_write_register (IspDev,0x3129, 0x00); //ADBIT1
    imx290_write_register (IspDev,0x317c, 0x00); //ADBIT2
    imx290_write_register (IspDev,0x31ec, 0x0e); //ADBIT3
    imx290_write_register (IspDev,0x3441, 0x0c); //CSI_DT_FMT
    imx290_write_register (IspDev,0x3442, 0x0c); //CSI_DT_FMT
    
    imx290_write_register (IspDev,0x3007, 0x00);  
    imx290_write_register (IspDev,0x300c, 0x00);
    imx290_write_register (IspDev,0x300f, 0x00);
    imx290_write_register (IspDev,0x3010, 0x21);
    imx290_write_register (IspDev,0x3012, 0x64);
    imx290_write_register (IspDev,0x3016, 0x09);
    imx290_write_register (IspDev,0x3017, 0x00);

#if 1
    imx290_write_register (IspDev,0x3020, 0x01);  /* SHS1 */
    imx290_write_register (IspDev,0x3021, 0x00);
    imx290_write_register (IspDev,0x3024, 0x00);  /* SHS2 */ 
    imx290_write_register (IspDev,0x3025, 0x00);
    imx290_write_register (IspDev,0x3028, 0x00);  /* SHS3 */
    imx290_write_register (IspDev,0x3029, 0x00);
    imx290_write_register (IspDev,0x3030, 0x00);  /* RHS1 */
    imx290_write_register (IspDev,0x3031, 0x00);
    imx290_write_register (IspDev,0x3034, 0x00);  /* RHS2 */ 
    imx290_write_register (IspDev,0x3035, 0x00);
#else
    imx290_write_register (IspDev,0x3020, 0x02);
#endif
    
    imx290_write_register (IspDev,0x305c, 0x18);
    imx290_write_register (IspDev,0x305d, 0x03); 
    imx290_write_register (IspDev,0x305e, 0x20); 
    imx290_write_register (IspDev,0x305f, 0x01);
    imx290_write_register (IspDev,0x3070, 0x02);
    imx290_write_register (IspDev,0x3071, 0x11);
    imx290_write_register (IspDev,0x309b, 0x10);
    imx290_write_register (IspDev,0x309c, 0x22);
    imx290_write_register (IspDev,0x30a2, 0x02);
    imx290_write_register (IspDev,0x30a6, 0x20);
    imx290_write_register (IspDev,0x30a8, 0x20);
    imx290_write_register (IspDev,0x30aa, 0x20);
    imx290_write_register (IspDev,0x30ac, 0x20);
    
    imx290_write_register (IspDev,0x30b0, 0x43);
    imx290_write_register (IspDev,0x3119, 0x9e);
    imx290_write_register (IspDev,0x311c, 0x1e);
    imx290_write_register (IspDev,0x311e, 0x08);
    imx290_write_register (IspDev,0x3128, 0x05);
    imx290_write_register (IspDev,0x313d, 0x83);
    imx290_write_register (IspDev,0x3150, 0x03);

    imx290_write_register (IspDev,0x317e, 0x00);
    imx290_write_register (IspDev,0x315e, 0x1a);
    imx290_write_register (IspDev,0x3164, 0x1a);
    imx290_write_register (IspDev,0x32b8, 0x50);

    imx290_write_register (IspDev,0x32b9, 0x10);
    imx290_write_register (IspDev,0x32ba, 0x00);
    imx290_write_register (IspDev,0x32bb, 0x04);
    imx290_write_register (IspDev,0x32c8, 0x50);
    imx290_write_register (IspDev,0x32c9, 0x10);
    imx290_write_register (IspDev,0x32ca, 0x00);
    imx290_write_register (IspDev,0x32cb, 0x04);
    imx290_write_register (IspDev,0x332c, 0xd3);
    imx290_write_register (IspDev,0x332d, 0x10);
    imx290_write_register (IspDev,0x332e, 0x0d);
    imx290_write_register (IspDev,0x3358, 0x06);
    imx290_write_register (IspDev,0x3359, 0xe1);
    imx290_write_register (IspDev,0x335a, 0x11);
    imx290_write_register (IspDev,0x3360, 0x1e);
    
    imx290_write_register (IspDev,0x3361, 0x61);
    imx290_write_register (IspDev,0x3362, 0x10);
    imx290_write_register (IspDev,0x33b0, 0x50);
    imx290_write_register (IspDev,0x33b2, 0x1a);
    imx290_write_register (IspDev,0x33b3, 0x04);
    imx290_write_register (IspDev,0x3414, 0x0a);
    imx290_write_register (IspDev,0x3418, 0x49);
    imx290_write_register (IspDev,0x3419, 0x04);
    imx290_write_register (IspDev,0x3444, 0x20);
    imx290_write_register (IspDev,0x3445, 0x25);

    imx290_write_register (IspDev,0x3446, 0x47);
    imx290_write_register (IspDev,0x3447, 0x0);
    imx290_write_register (IspDev,0x3448, 0x1f);
    imx290_write_register (IspDev,0x3449, 0x0);
    imx290_write_register (IspDev,0x344a, 0x17);
    imx290_write_register (IspDev,0x344b, 0x0);
    imx290_write_register (IspDev,0x344c, 0x0f);
    imx290_write_register (IspDev,0x344d, 0x0);
    imx290_write_register (IspDev,0x344e, 0x17);
    imx290_write_register (IspDev,0x344f, 0x0);
    imx290_write_register (IspDev,0x3450, 0x47);
    imx290_write_register (IspDev,0x3451, 0x0);
    imx290_write_register (IspDev,0x3452, 0x0f);
    imx290_write_register (IspDev,0x3453, 0x0);
    imx290_write_register (IspDev,0x3454, 0x0f);
    imx290_write_register (IspDev,0x3455, 0x0);
    imx290_write_register (IspDev,0x3480, 0x49);

    imx290_write_register (IspDev,0x3000, 0x00); /* standby */
    delay_ms(20);
    imx290_write_register (IspDev,0x3002, 0x00); /* master mode start */
    imx290_write_register (IspDev,0x304b, 0x0a); /* XVSOUTSEL XHSOUTSEL */
    
    printf("===IMX290 1080P 30fps 12bit LINE Init OK!===\n");    
    return;
}


void imx290_wdr_1080p30_2to1_init(ISP_DEV IspDev)
{
    imx290_write_register (IspDev,0x3000, 0x01); /* standby */
    imx290_write_register (IspDev,0x3002, 0x01); /* XTMSTA */

    //10bit
    imx290_write_register (IspDev,0x3005, 0x00);
    imx290_write_register (IspDev,0x3007, 0x00);
    imx290_write_register (IspDev,0x300a, 0x3c);
    imx290_write_register (IspDev,0x300c, 0x11);
    imx290_write_register (IspDev,0x300f, 0x00);
    imx290_write_register (IspDev,0x3010, 0x21);
    imx290_write_register (IspDev,0x3012, 0x64);
    imx290_write_register (IspDev,0x3016, 0x09);

    imx290_write_register (IspDev,0x3018, 0xA6);   /**** VMAX ****/
    imx290_write_register (IspDev,0x3019, 0x04);
    imx290_write_register (IspDev,0x301A, 0x00);

    imx290_write_register (IspDev,0x301C, 0xD8);  /***** HMAX ****/
    imx290_write_register (IspDev,0x301D, 0x0F);
    
    imx290_write_register (IspDev,0x3020, 0x02);
    imx290_write_register (IspDev,0x3024, 0xc9);
    imx290_write_register (IspDev,0x3030, 0x0b);
    imx290_write_register (IspDev,0x3045, 0x05);
    imx290_write_register (IspDev,0x3046, 0x00);
    imx290_write_register (IspDev,0x304b, 0x0a);
    imx290_write_register (IspDev,0x305c, 0x18);
    imx290_write_register (IspDev,0x305d, 0x03);
    imx290_write_register (IspDev,0x305e, 0x20);
    imx290_write_register (IspDev,0x305f, 0x01);
    imx290_write_register (IspDev,0x3070, 0x02);
    imx290_write_register (IspDev,0x3071, 0x11);
    imx290_write_register (IspDev,0x309b, 0x10);
    imx290_write_register (IspDev,0x309c, 0x22);
    imx290_write_register (IspDev,0x30a2, 0x02);
    imx290_write_register (IspDev,0x30a6, 0x20);
    imx290_write_register (IspDev,0x30a8, 0x20);
    imx290_write_register (IspDev,0x30aa, 0x20);
    imx290_write_register (IspDev,0x30ac, 0x20);
    imx290_write_register (IspDev,0x30b0, 0x43);
    imx290_write_register (IspDev,0x3106, 0x11);
    imx290_write_register (IspDev,0x3119, 0x9e);
    imx290_write_register (IspDev,0x311c, 0x1e);
    imx290_write_register (IspDev,0x311e, 0x08);
    imx290_write_register (IspDev,0x3128, 0x05);
    imx290_write_register (IspDev,0x3129, 0x1d);
    imx290_write_register (IspDev,0x313d, 0x83);
    imx290_write_register (IspDev,0x3150, 0x03);
    imx290_write_register (IspDev,0x315e, 0x1a);
    imx290_write_register (IspDev,0x3164, 0x1a);
    imx290_write_register (IspDev,0x317c, 0x12);
    imx290_write_register (IspDev,0x317e, 0x00);
    imx290_write_register (IspDev,0x31ec, 0x37);
    imx290_write_register (IspDev,0x32b8, 0x50);
    imx290_write_register (IspDev,0x32b9, 0x10);
    imx290_write_register (IspDev,0x32ba, 0x00);
    imx290_write_register (IspDev,0x32bb, 0x04);
    imx290_write_register (IspDev,0x32c8, 0x50);
    imx290_write_register (IspDev,0x32c9, 0x10);
    imx290_write_register (IspDev,0x32ca, 0x00);
    imx290_write_register (IspDev,0x32cb, 0x04);
    imx290_write_register (IspDev,0x332c, 0xd3);
    imx290_write_register (IspDev,0x332d, 0x10);
    imx290_write_register (IspDev,0x332e, 0x0d);
    imx290_write_register (IspDev,0x3358, 0x06);
    imx290_write_register (IspDev,0x3359, 0xe1);
    imx290_write_register (IspDev,0x335a, 0x11);
    imx290_write_register (IspDev,0x3360, 0x1e);
    imx290_write_register (IspDev,0x3361, 0x61);
    imx290_write_register (IspDev,0x3362, 0x10);
    imx290_write_register (IspDev,0x33b0, 0x50);
    imx290_write_register (IspDev,0x33b2, 0x1a);
    imx290_write_register (IspDev,0x33b3, 0x04);

    imx290_write_register (IspDev,0x3418, 0xb2); /**** Y_OUT_SIZE *****/ 
    imx290_write_register (IspDev,0x3419, 0x08);
   
    imx290_write_register (IspDev,0x3441, 0x0a);
    imx290_write_register (IspDev,0x3442, 0x0a);
    imx290_write_register (IspDev,0x3444, 0x20);
    imx290_write_register (IspDev,0x3445, 0x25);
    imx290_write_register (IspDev,0x3480, 0x49);
 
    imx290_write_register (IspDev,0x3000, 0x00); /* standby */
    delay_ms(20); 
    imx290_write_register (IspDev,0x3002, 0x00); /* master mode start */
 
    printf("===Imx290 sensor 1080P15fps 10bit 2to1 WDR(30fps->15fps) init success!=====\n");

    return;
    
}

void imx290_wdr_1080p60_2to1_init(ISP_DEV IspDev)
{
 #if 0   
    imx290_write_register (IspDev,0x3000, 0x01); /* standby */
    imx290_write_register (IspDev,0x3002, 0x00); /* XTMSTA */

    imx290_write_register (IspDev,0x3005, 0x01);
    imx290_write_register (IspDev,0x3007, 0x00);
    imx290_write_register (IspDev,0x3009, 0x01);
    imx290_write_register (IspDev,0x300a, 0xf0);
    imx290_write_register (IspDev,0x300c, 0x11);
    imx290_write_register (IspDev,0x300f, 0x00);
    imx290_write_register (IspDev,0x3010, 0x21);
    imx290_write_register (IspDev,0x3012, 0x64);
    imx290_write_register (IspDev,0x3016, 0x09);
    imx290_write_register (IspDev,0x3018, 0x65);
    imx290_write_register (IspDev,0x3019, 0x04);

    imx290_write_register (IspDev,0x301c, 0x98); /* HMAX */
    imx290_write_register (IspDev,0x301d, 0x08); /* HMAX */

    imx290_write_register (IspDev,0x3045, 0x05);
    imx290_write_register (IspDev,0x3046, 0x01);
    imx290_write_register (IspDev,0x304b, 0x0a);
    
    imx290_write_register (IspDev,0x305c, 0x18);
    imx290_write_register (IspDev,0x305d, 0x03);
    imx290_write_register (IspDev,0x305e, 0x20);
    imx290_write_register (IspDev,0x305f, 0x01);
    
    imx290_write_register (IspDev,0x3070, 0x02);
    imx290_write_register (IspDev,0x3071, 0x11);
    
    imx290_write_register (IspDev,0x309b, 0x10);
    imx290_write_register (IspDev,0x309c, 0x22);
    
    imx290_write_register (IspDev,0x30a2, 0x02);
    imx290_write_register (IspDev,0x30a6, 0x20);
    imx290_write_register (IspDev,0x30a8, 0x20);
    
    imx290_write_register (IspDev,0x30aa, 0x20);
    imx290_write_register (IspDev,0x30ac, 0x20);
    imx290_write_register (IspDev,0x30b0, 0x43);

    imx290_write_register (IspDev,0x3106, 0x11);
    imx290_write_register (IspDev,0x3119, 0x9e);
    imx290_write_register (IspDev,0x311c, 0x1e);
    imx290_write_register (IspDev,0x311e, 0x08);
    
    imx290_write_register (IspDev,0x3128, 0x05);
    imx290_write_register (IspDev,0x3129, 0x00);
    imx290_write_register (IspDev,0x313d, 0x83);
    imx290_write_register (IspDev,0x3150, 0x03);
    
    imx290_write_register (IspDev,0x315e, 0x1a);
    imx290_write_register (IspDev,0x3164, 0x1a);
    imx290_write_register (IspDev,0x317c, 0x00);
    imx290_write_register (IspDev,0x317e, 0x00);
    imx290_write_register (IspDev,0x31ec, 0x00);
                                         
    imx290_write_register (IspDev,0x32b8, 0x50);
    imx290_write_register (IspDev,0x32b9, 0x10);
    imx290_write_register (IspDev,0x32ba, 0x00);
    imx290_write_register (IspDev,0x32bb, 0x04);
    
    imx290_write_register (IspDev,0x32c8, 0x50);
    imx290_write_register (IspDev,0x32c9, 0x10);
    imx290_write_register (IspDev,0x32ca, 0x00);
    imx290_write_register (IspDev,0x32cb, 0x04);
                                        
    imx290_write_register (IspDev,0x332c, 0xd3);
    imx290_write_register (IspDev,0x332d, 0x10);
    imx290_write_register (IspDev,0x332e, 0x0d);
    
    imx290_write_register (IspDev,0x3358, 0x06);
    imx290_write_register (IspDev,0x3359, 0xe1);
    imx290_write_register (IspDev,0x335a, 0x11);
    
    imx290_write_register (IspDev,0x3360, 0x1e);
    imx290_write_register (IspDev,0x3361, 0x61);
    imx290_write_register (IspDev,0x3362, 0x10);
    
    imx290_write_register (IspDev,0x33b0, 0x50);
    imx290_write_register (IspDev,0x33b2, 0x1a);
    imx290_write_register (IspDev,0x33b3, 0x04);
                                         
    imx290_write_register (IspDev,0x3405, 0x10);
    imx290_write_register (IspDev,0x3407, 0x03);
    imx290_write_register (IspDev,0x3414, 0x0a);
    imx290_write_register (IspDev,0x3415, 0x00);
    imx290_write_register (IspDev,0x3418, 0xb2);
    imx290_write_register (IspDev,0x3419, 0x08);
    
    imx290_write_register (IspDev,0x3441, 0x0c);
    imx290_write_register (IspDev,0x3442, 0x0c);
    imx290_write_register (IspDev,0x3443, 0x03);
    imx290_write_register (IspDev,0x3444, 0x20);
    imx290_write_register (IspDev,0x3445, 0x25);
    imx290_write_register (IspDev,0x3446, 0x57);
    imx290_write_register (IspDev,0x3447, 0x00);
    imx290_write_register (IspDev,0x3448, 0x37);
    imx290_write_register (IspDev,0x3449, 0x00);
    imx290_write_register (IspDev,0x344a, 0x1f);
    
    imx290_write_register (IspDev,0x344b, 0x00);
    imx290_write_register (IspDev,0x344c, 0x1f);
    imx290_write_register (IspDev,0x344d, 0x00);
    imx290_write_register (IspDev,0x344e, 0x1f);
    imx290_write_register (IspDev,0x344f, 0x00);
    imx290_write_register (IspDev,0x3450, 0x77);
    imx290_write_register (IspDev,0x3451, 0x00);
    imx290_write_register (IspDev,0x3452, 0x1f);
    imx290_write_register (IspDev,0x3453, 0x00);
    imx290_write_register (IspDev,0x3454, 0x17);
    imx290_write_register (IspDev,0x3455, 0x00);
    imx290_write_register (IspDev,0x3472, 0x9c);
    imx290_write_register (IspDev,0x3473, 0x07);
    imx290_write_register (IspDev,0x3480, 0x49);

    imx290_write_register (IspDev,0x3000, 0x00); /* standby */
    delay_ms(20); 
    imx290_write_register (IspDev,0x3002, 0x00); /* master mode start */
 
    printf("===Imx290 sensor 1080P30fps 12bit 2to1 WDR(60fps->30fps) init success!=====\n");

    bSensorInit = HI_TRUE;

    return;
 #endif
    imx290_write_register (IspDev,0x3000, 0x01); /* standby */
    imx290_write_register (IspDev,0x3002, 0x01); /* XTMSTA */

    imx290_write_register (IspDev,0x3005, 0x00);
    imx290_write_register (IspDev,0x3007, 0x00);
    imx290_write_register (IspDev,0x3009, 0x01);
    imx290_write_register (IspDev,0x300a, 0x3c);
    imx290_write_register (IspDev,0x300c, 0x11);
    imx290_write_register (IspDev,0x300f, 0x00);
    imx290_write_register (IspDev,0x3010, 0x21);
    imx290_write_register (IspDev,0x3012, 0x64);
    imx290_write_register (IspDev,0x3016, 0x09);
    imx290_write_register (IspDev,0x3018, 0xC4); /* VMAX */
    imx290_write_register (IspDev,0x3019, 0x04); /* VMAX */
    imx290_write_register (IspDev,0x301c, 0xEC); /* HMAX */
    imx290_write_register (IspDev,0x301d, 0x07); /* HMAX */

#if 1
    imx290_write_register (IspDev,0x3020, 0x03);  /* SHS1 */
    imx290_write_register (IspDev,0x3021, 0x00);
    //imx290_write_register (IspDev,0x3022, 0x00);
    imx290_write_register (IspDev,0x3024, 0x99);  /* SHS2 */ 
    imx290_write_register (IspDev,0x3025, 0x00);
    //imx290_write_register (IspDev,0x3025, 0x00);
    imx290_write_register (IspDev,0x3028, 0x00);  /* SHS3 */
    imx290_write_register (IspDev,0x3029, 0x00);
    //imx290_write_register (IspDev,0x302A, 0x00);
    imx290_write_register (IspDev,0x3030, 0x93);  /* RHS1 */
    imx290_write_register (IspDev,0x3031, 0x00);
    imx290_write_register (IspDev,0x3034, 0x00);  /* RHS2 */ 
    imx290_write_register (IspDev,0x3035, 0x00);
#endif
    
    imx290_write_register (IspDev,0x3045, 0x05);
    imx290_write_register (IspDev,0x3046, 0x00);
    imx290_write_register (IspDev,0x304b, 0x0a);
    imx290_write_register (IspDev,0x305c, 0x18);
    imx290_write_register (IspDev,0x305d, 0x03);
    imx290_write_register (IspDev,0x305e, 0x20);
    imx290_write_register (IspDev,0x305f, 0x01);
    imx290_write_register (IspDev,0x3070, 0x02);
    imx290_write_register (IspDev,0x3071, 0x11);
    imx290_write_register (IspDev,0x309b, 0x10);
    imx290_write_register (IspDev,0x309c, 0x22);
    imx290_write_register (IspDev,0x30a2, 0x02);
    imx290_write_register (IspDev,0x30a6, 0x20);
    imx290_write_register (IspDev,0x30a8, 0x20);
    imx290_write_register (IspDev,0x30aa, 0x20);
    imx290_write_register (IspDev,0x30ac, 0x20);
    imx290_write_register (IspDev,0x30b0, 0x43);

    imx290_write_register (IspDev,0x3106, 0x11); 
    imx290_write_register (IspDev,0x3119, 0x9e);
    imx290_write_register (IspDev,0x311c, 0x1e);
    imx290_write_register (IspDev,0x311e, 0x08);
    imx290_write_register (IspDev,0x3128, 0x05);
    imx290_write_register (IspDev,0x3129, 0x1d);
    imx290_write_register (IspDev,0x313d, 0x83);
    imx290_write_register (IspDev,0x3150, 0x03);
    imx290_write_register (IspDev,0x315e, 0x1a);
    imx290_write_register (IspDev,0x3164, 0x1a);
    imx290_write_register (IspDev,0x317c, 0x12);
    imx290_write_register (IspDev,0x317e, 0x00);
    imx290_write_register (IspDev,0x31ec, 0x37);
                                         
    imx290_write_register (IspDev,0x32b8, 0x50);
    imx290_write_register (IspDev,0x32b9, 0x10);
    imx290_write_register (IspDev,0x32ba, 0x00);
    imx290_write_register (IspDev,0x32bb, 0x04);
    imx290_write_register (IspDev,0x32c8, 0x50);
    imx290_write_register (IspDev,0x32c9, 0x10);
    imx290_write_register (IspDev,0x32ca, 0x00);
    imx290_write_register (IspDev,0x32cb, 0x04);
                                        
    imx290_write_register (IspDev,0x332c, 0xd3);
    imx290_write_register (IspDev,0x332d, 0x10);
    imx290_write_register (IspDev,0x332e, 0x0d);
    imx290_write_register (IspDev,0x3358, 0x06);
    imx290_write_register (IspDev,0x3359, 0xe1);
    imx290_write_register (IspDev,0x335a, 0x11);
    imx290_write_register (IspDev,0x3360, 0x1e);
    imx290_write_register (IspDev,0x3361, 0x61);
    imx290_write_register (IspDev,0x3362, 0x10);
    imx290_write_register (IspDev,0x33b0, 0x50);
    imx290_write_register (IspDev,0x33b2, 0x1a);
    imx290_write_register (IspDev,0x33b3, 0x04);
                                         
    imx290_write_register (IspDev,0x3405, 0x10);
    imx290_write_register (IspDev,0x3407, 0x03);
    imx290_write_register (IspDev,0x3414, 0x0a);
    imx290_write_register (IspDev,0x3415, 0x00);
    imx290_write_register (IspDev,0x3418, 0x32);  /* Y_OUT_SIZE */
    imx290_write_register (IspDev,0x3419, 0x09);  /* Y_OUT_SIZE */
    imx290_write_register (IspDev,0x3441, 0x0a);
    imx290_write_register (IspDev,0x3442, 0x0a);
    imx290_write_register (IspDev,0x3443, 0x03);
    imx290_write_register (IspDev,0x3444, 0x20);
    imx290_write_register (IspDev,0x3445, 0x25);
    imx290_write_register (IspDev,0x3446, 0x57);
    imx290_write_register (IspDev,0x3447, 0x00);
    imx290_write_register (IspDev,0x3448, 0x37);
    imx290_write_register (IspDev,0x3449, 0x00);
    imx290_write_register (IspDev,0x344a, 0x1f);
    imx290_write_register (IspDev,0x344b, 0x00);
    imx290_write_register (IspDev,0x344c, 0x1f);
    imx290_write_register (IspDev,0x344d, 0x00);
    imx290_write_register (IspDev,0x344e, 0x1f);
    imx290_write_register (IspDev,0x344f, 0x00);
    imx290_write_register (IspDev,0x3450, 0x77);
    imx290_write_register (IspDev,0x3451, 0x00);
    imx290_write_register (IspDev,0x3452, 0x1f);
    imx290_write_register (IspDev,0x3453, 0x00);
    imx290_write_register (IspDev,0x3454, 0x17);
    imx290_write_register (IspDev,0x3455, 0x00);
    imx290_write_register (IspDev,0x3472, 0x9c);
    imx290_write_register (IspDev,0x3473, 0x07);
    imx290_write_register (IspDev,0x3480, 0x49);

    imx290_write_register (IspDev,0x3000, 0x00); /* standby */
    delay_ms(20); 
    imx290_write_register (IspDev,0x3002, 0x00); /* master mode start */
 
    printf("===Imx290 sensor 1080P30fps 10bit 2to1 WDR(60fps->30fps) init success!=====\n");

    return;

}

void imx290_wdr_1080p30_3to1_init(ISP_DEV IspDev)
{
    imx290_write_register (IspDev,0x3000, 0x01); /* standby */
    imx290_write_register (IspDev,0x3002, 0x01); /* XTMSTA */

    //12bit
    imx290_write_register (IspDev,0x3007, 0x00);
    imx290_write_register (IspDev,0x300c, 0x21);
    imx290_write_register (IspDev,0x300f, 0x00);
    imx290_write_register (IspDev,0x3010, 0x21);
    imx290_write_register (IspDev,0x3012, 0x64);
    imx290_write_register (IspDev,0x3016, 0x09);
    imx290_write_register (IspDev,0x3020, 0x04);
    imx290_write_register (IspDev,0x3021, 0x00);
    imx290_write_register (IspDev,0x3024, 0xF2);
    imx290_write_register (IspDev,0x3025, 0x01);
    imx290_write_register (IspDev,0x3028, 0x57);
    imx290_write_register (IspDev,0x3029, 0x02);
    imx290_write_register (IspDev,0x3030, 0xED);
    imx290_write_register (IspDev,0x3031, 0x01);
    imx290_write_register (IspDev,0x3034, 0x30);
    imx290_write_register (IspDev,0x3035, 0x02);
    imx290_write_register (IspDev,0x3045, 0x05);
    imx290_write_register (IspDev,0x304b, 0x0a);
    imx290_write_register (IspDev,0x305c, 0x18);
    imx290_write_register (IspDev,0x305d, 0x03);
    imx290_write_register (IspDev,0x305e, 0x20);
    imx290_write_register (IspDev,0x305f, 0x01);
    imx290_write_register (IspDev,0x3070, 0x02);
    imx290_write_register (IspDev,0x3071, 0x11);
    imx290_write_register (IspDev,0x309b, 0x10);
    imx290_write_register (IspDev,0x309c, 0x22);
    imx290_write_register (IspDev,0x30a2, 0x02);
    imx290_write_register (IspDev,0x30a6, 0x20);
    imx290_write_register (IspDev,0x30a8, 0x20);
    imx290_write_register (IspDev,0x30aa, 0x20);
    imx290_write_register (IspDev,0x30ac, 0x20);
    imx290_write_register (IspDev,0x30b0, 0x43);
    imx290_write_register (IspDev,0x3106, 0x33);
    imx290_write_register (IspDev,0x3119, 0x9e);
    imx290_write_register (IspDev,0x311c, 0x1e);
    imx290_write_register (IspDev,0x311e, 0x08);
    imx290_write_register (IspDev,0x3128, 0x05);
    imx290_write_register (IspDev,0x313d, 0x83);
    imx290_write_register (IspDev,0x3150, 0x03);
    imx290_write_register (IspDev,0x315e, 0x1a);
    imx290_write_register (IspDev,0x3164, 0x1a);
    imx290_write_register (IspDev,0x317e, 0x00);
    imx290_write_register (IspDev,0x32b8, 0x50);
    imx290_write_register (IspDev,0x32b9, 0x10);
    imx290_write_register (IspDev,0x32ba, 0x00);
    imx290_write_register (IspDev,0x32bb, 0x04);
    imx290_write_register (IspDev,0x32c8, 0x50);
    imx290_write_register (IspDev,0x32c9, 0x10);
    imx290_write_register (IspDev,0x32ca, 0x00);
    imx290_write_register (IspDev,0x32cb, 0x04);
    imx290_write_register (IspDev,0x332c, 0xd3);
    imx290_write_register (IspDev,0x332d, 0x10);
    imx290_write_register (IspDev,0x332e, 0x0d);
    imx290_write_register (IspDev,0x3358, 0x06);
    imx290_write_register (IspDev,0x3359, 0xe1);
    imx290_write_register (IspDev,0x335a, 0x11);
    imx290_write_register (IspDev,0x3360, 0x1e);
    imx290_write_register (IspDev,0x3361, 0x61);
    imx290_write_register (IspDev,0x3362, 0x10);
    imx290_write_register (IspDev,0x33b0, 0x50);
    imx290_write_register (IspDev,0x33b2, 0x1a);
    imx290_write_register (IspDev,0x33b3, 0x04);

    imx290_write_register (IspDev,0x3418, 0x24); /**** Y_OUT_SIZE *****/ 
    imx290_write_register (IspDev,0x3419, 0x0F);
    
    imx290_write_register (IspDev,0x3444, 0x20);
    imx290_write_register (IspDev,0x3445, 0x25);
    imx290_write_register (IspDev,0x3480, 0x49);

    imx290_write_register (IspDev,0x3000, 0x00); /* standby */
    delay_ms(20); 
    imx290_write_register (IspDev,0x3002, 0x00); /* master mode start */
 
    printf("===Imx290 imx290 1080P15fps 12bit 3to1 WDR(30fps->7p5fps) init success!=====\n");

    return;
    
}

void imx290_wdr_720p60_2to1_init(ISP_DEV IspDev)
{
    imx290_write_register (IspDev,0x3000, 0x01); /* standby */
    imx290_write_register (IspDev,0x3002, 0x01); /* XTMSTA */

    //12bit
    imx290_write_register (IspDev,0x3005, 0x01);
    imx290_write_register (IspDev,0x3007, 0x10);
    imx290_write_register (IspDev,0x3009, 0x01);
    imx290_write_register (IspDev,0x300a, 0xf0);
    imx290_write_register (IspDev,0x300c, 0x11);
    imx290_write_register (IspDev,0x300f, 0x00);
    imx290_write_register (IspDev,0x3010, 0x21);
    imx290_write_register (IspDev,0x3012, 0x64);
    imx290_write_register (IspDev,0x3016, 0x09);
    imx290_write_register (IspDev,0x3018, 0xee);
    imx290_write_register (IspDev,0x3019, 0x02);
    imx290_write_register (IspDev,0x301c, 0xe4);
    imx290_write_register (IspDev,0x301d, 0x0c);
    imx290_write_register (IspDev,0x3045, 0x05);
    imx290_write_register (IspDev,0x3046, 0x01);
    imx290_write_register (IspDev,0x304b, 0x0a);

    imx290_write_register (IspDev,0x305c, 0x20); //INCKSEL1
    imx290_write_register (IspDev,0x305d, 0x03); //INCKSEL2
    imx290_write_register (IspDev,0x305e, 0x20); //INCKSEL3
    imx290_write_register (IspDev,0x305f, 0x01); //INCKSEL4

    imx290_write_register (IspDev,0x3070, 0x02);
    imx290_write_register (IspDev,0x3071, 0x11);
    imx290_write_register (IspDev,0x309b, 0x10);
    imx290_write_register (IspDev,0x309c, 0x22);
    imx290_write_register (IspDev,0x30a2, 0x02);
    imx290_write_register (IspDev,0x30a6, 0x20);
    imx290_write_register (IspDev,0x30a8, 0x20);
    imx290_write_register (IspDev,0x30aa, 0x20);
    imx290_write_register (IspDev,0x30ac, 0x20);
    imx290_write_register (IspDev,0x30b0, 0x43);

    //Add 
    imx290_write_register (IspDev,0x3106, 0x11);
    
    imx290_write_register (IspDev,0x3119, 0x9e);
    imx290_write_register (IspDev,0x311c, 0x1e);
    imx290_write_register (IspDev,0x311e, 0x08);
    imx290_write_register (IspDev,0x3128, 0x05);
    imx290_write_register (IspDev,0x3129, 0x00);
    imx290_write_register (IspDev,0x313d, 0x83);
    imx290_write_register (IspDev,0x3150, 0x03);
    imx290_write_register (IspDev,0x315e, 0x1a);
    imx290_write_register (IspDev,0x3164, 0x1a);
    imx290_write_register (IspDev,0x317c, 0x00);
    imx290_write_register (IspDev,0x317e, 0x00);
    imx290_write_register (IspDev,0x31ec, 0x00);
    
    imx290_write_register (IspDev,0x32b8, 0x50);
    imx290_write_register (IspDev,0x32b9, 0x10);
    imx290_write_register (IspDev,0x32ba, 0x00);
    imx290_write_register (IspDev,0x32bb, 0x04);
    imx290_write_register (IspDev,0x32c8, 0x50);
    imx290_write_register (IspDev,0x32c9, 0x10);
    imx290_write_register (IspDev,0x32ca, 0x00);
    imx290_write_register (IspDev,0x32cb, 0x04);
    
    imx290_write_register (IspDev,0x332c, 0xd3);
    imx290_write_register (IspDev,0x332d, 0x10);
    imx290_write_register (IspDev,0x332e, 0x0d);
    imx290_write_register (IspDev,0x3358, 0x06);
    imx290_write_register (IspDev,0x3359, 0xe1);
    imx290_write_register (IspDev,0x335a, 0x11);
    imx290_write_register (IspDev,0x3360, 0x1e);
    imx290_write_register (IspDev,0x3361, 0x61);
    imx290_write_register (IspDev,0x3362, 0x10);
    imx290_write_register (IspDev,0x33b0, 0x50);
    imx290_write_register (IspDev,0x33b2, 0x1a);
    imx290_write_register (IspDev,0x33b3, 0x04);
    
    imx290_write_register (IspDev,0x3405, 0x10);
    imx290_write_register (IspDev,0x3407, 0x03);
    imx290_write_register (IspDev,0x3414, 0x04);
    imx290_write_register (IspDev,0x3418, 0xc6);
    imx290_write_register (IspDev,0x3419, 0x05);
    imx290_write_register (IspDev,0x3441, 0x0c);
    imx290_write_register (IspDev,0x3442, 0x0c);
    imx290_write_register (IspDev,0x3443, 0x03);
    imx290_write_register (IspDev,0x3444, 0x20);
    imx290_write_register (IspDev,0x3445, 0x25);
    imx290_write_register (IspDev,0x3446, 0x4f);
    imx290_write_register (IspDev,0x3447, 0x00);
    imx290_write_register (IspDev,0x3448, 0x2f);
    imx290_write_register (IspDev,0x3449, 0x00);
    imx290_write_register (IspDev,0x344a, 0x17);
    imx290_write_register (IspDev,0x344b, 0x00);
    imx290_write_register (IspDev,0x344c, 0x17);
    imx290_write_register (IspDev,0x344d, 0x00);
    imx290_write_register (IspDev,0x344e, 0x17);
    imx290_write_register (IspDev,0x344f, 0x00);
    imx290_write_register (IspDev,0x3450, 0x57);
    imx290_write_register (IspDev,0x3451, 0x00);
    imx290_write_register (IspDev,0x3452, 0x17);
    imx290_write_register (IspDev,0x3453, 0x00);
    imx290_write_register (IspDev,0x3454, 0x17);
    imx290_write_register (IspDev,0x3455, 0x00);
    imx290_write_register (IspDev,0x3472, 0x1c);
    imx290_write_register (IspDev,0x3473, 0x05);
    imx290_write_register (IspDev,0x3480, 0x49);

    imx290_write_register (IspDev,0x3000, 0x00); /* standby */
    delay_ms(20); 
    imx290_write_register (IspDev,0x3002, 0x00); /* master mode start */
 
    printf("===Imx290 imx290 720P30fps 12bit 2to1 WDR(60fps->30fps) init success!=====\n");

    return;
}

void imx290_wdr_720p60_3to1_init(ISP_DEV IspDev)
{
    imx290_write_register (IspDev,0x3000, 0x01); /* standby */
    imx290_write_register (IspDev,0x3002, 0x01); /* XTMSTA */
    
    //12bit
    imx290_write_register (IspDev,0x3005, 0x01);
    imx290_write_register (IspDev,0x3007, 0x10);
    imx290_write_register (IspDev,0x3009, 0x01);
    imx290_write_register (IspDev,0x300a, 0xf0);
    imx290_write_register (IspDev,0x300c, 0x31);
    imx290_write_register (IspDev,0x300f, 0x00);
    imx290_write_register (IspDev,0x3010, 0x21);
    imx290_write_register (IspDev,0x3012, 0x64);
    imx290_write_register (IspDev,0x3016, 0x09);
    imx290_write_register (IspDev,0x3018, 0xee);
    imx290_write_register (IspDev,0x3019, 0x02);
    imx290_write_register (IspDev,0x301c, 0xe4);
    imx290_write_register (IspDev,0x301d, 0x0c);
    imx290_write_register (IspDev,0x3045, 0x05);
    imx290_write_register (IspDev,0x3046, 0x01);
    imx290_write_register (IspDev,0x304b, 0x0a);

    imx290_write_register (IspDev,0x305c, 0x20); //INCKSEL1
    imx290_write_register (IspDev,0x305d, 0x03); //INCKSEL2
    imx290_write_register (IspDev,0x305e, 0x20); //INCKSEL3
    imx290_write_register (IspDev,0x305f, 0x01); //INCKSEL4
    
    imx290_write_register (IspDev,0x3070, 0x02);
    imx290_write_register (IspDev,0x3071, 0x11);
    imx290_write_register (IspDev,0x309b, 0x10);
    imx290_write_register (IspDev,0x309c, 0x22);
    imx290_write_register (IspDev,0x30a2, 0x02);
    imx290_write_register (IspDev,0x30a6, 0x20);
    imx290_write_register (IspDev,0x30a8, 0x20);
    imx290_write_register (IspDev,0x30aa, 0x20);
    imx290_write_register (IspDev,0x30ac, 0x20);
    imx290_write_register (IspDev,0x30b0, 0x43);

    //Add 
    imx290_write_register (IspDev,0x3106, 0x33);
    
    imx290_write_register (IspDev,0x3119, 0x9e);
    imx290_write_register (IspDev,0x311c, 0x1e);
    imx290_write_register (IspDev,0x311e, 0x08);
    imx290_write_register (IspDev,0x3128, 0x05);
    imx290_write_register (IspDev,0x3129, 0x00);
    imx290_write_register (IspDev,0x313d, 0x83);
    imx290_write_register (IspDev,0x3150, 0x03);
    imx290_write_register (IspDev,0x315e, 0x1a);
    imx290_write_register (IspDev,0x3164, 0x1a);
    imx290_write_register (IspDev,0x317c, 0x00);
    imx290_write_register (IspDev,0x317e, 0x00);
    imx290_write_register (IspDev,0x31ec, 0x00);
    
    imx290_write_register (IspDev,0x32b8, 0x50);
    imx290_write_register (IspDev,0x32b9, 0x10);
    imx290_write_register (IspDev,0x32ba, 0x00);
    imx290_write_register (IspDev,0x32bb, 0x04);
    imx290_write_register (IspDev,0x32c8, 0x50);
    imx290_write_register (IspDev,0x32c9, 0x10);
    imx290_write_register (IspDev,0x32ca, 0x00);
    imx290_write_register (IspDev,0x32cb, 0x04);
    
    imx290_write_register (IspDev,0x332c, 0xd3);
    imx290_write_register (IspDev,0x332d, 0x10);
    imx290_write_register (IspDev,0x332e, 0x0d);
    imx290_write_register (IspDev,0x3358, 0x06);
    imx290_write_register (IspDev,0x3359, 0xe1);
    imx290_write_register (IspDev,0x335a, 0x11);
    imx290_write_register (IspDev,0x3360, 0x1e);
    imx290_write_register (IspDev,0x3361, 0x61);
    imx290_write_register (IspDev,0x3362, 0x10);
    imx290_write_register (IspDev,0x33b0, 0x50);
    imx290_write_register (IspDev,0x33b2, 0x1a);
    imx290_write_register (IspDev,0x33b3, 0x04);
    
    imx290_write_register (IspDev,0x3405, 0x10);
    imx290_write_register (IspDev,0x3407, 0x03);
    imx290_write_register (IspDev,0x3414, 0x04);
    imx290_write_register (IspDev,0x3418, 0xb5);
    imx290_write_register (IspDev,0x3419, 0x08);
    imx290_write_register (IspDev,0x3441, 0x0c);
    imx290_write_register (IspDev,0x3442, 0x0c);
    imx290_write_register (IspDev,0x3443, 0x03);
    imx290_write_register (IspDev,0x3444, 0x20);
    imx290_write_register (IspDev,0x3445, 0x25);
    imx290_write_register (IspDev,0x3446, 0x4f);
    imx290_write_register (IspDev,0x3447, 0x00);
    imx290_write_register (IspDev,0x3448, 0x2f);
    imx290_write_register (IspDev,0x3449, 0x00);
    imx290_write_register (IspDev,0x344a, 0x17);
    imx290_write_register (IspDev,0x344b, 0x00);
    imx290_write_register (IspDev,0x344c, 0x17);
    imx290_write_register (IspDev,0x344d, 0x00);
    imx290_write_register (IspDev,0x344e, 0x17);
    imx290_write_register (IspDev,0x344f, 0x00);
    imx290_write_register (IspDev,0x3450, 0x57);
    imx290_write_register (IspDev,0x3451, 0x00);
    imx290_write_register (IspDev,0x3452, 0x17);
    imx290_write_register (IspDev,0x3453, 0x00);
    imx290_write_register (IspDev,0x3454, 0x17);
    imx290_write_register (IspDev,0x3455, 0x00);
    imx290_write_register (IspDev,0x3472, 0x1c);
    imx290_write_register (IspDev,0x3473, 0x05);
    imx290_write_register (IspDev,0x3480, 0x49);

    imx290_write_register (IspDev,0x3000, 0x00); /* standby */
    delay_ms(20); 
    imx290_write_register (IspDev,0x3002, 0x00); /* master mode start */
 
    printf("===Imx290 imx290 720P15fps 12bit 3to1 WDR(60fps->15fps) init success!=====\n");
    return;
}

void imx290_wdr_1080p120_2to1_init(ISP_DEV IspDev)
{
    imx290_write_register (IspDev,0x3000, 0x01); /* standby */
    imx290_write_register (IspDev,0x3002, 0x01); /* XTMSTA */

    imx290_write_register (IspDev,0x3005, 0x00);
    imx290_write_register (IspDev,0x3007, 0x00);
    imx290_write_register (IspDev,0x3009, 0x00);
    imx290_write_register (IspDev,0x300a, 0x3c);
    imx290_write_register (IspDev,0x300c, 0x11);
    imx290_write_register (IspDev,0x300f, 0x00);
    imx290_write_register (IspDev,0x3010, 0x21);
    imx290_write_register (IspDev,0x3012, 0x64);
    imx290_write_register (IspDev,0x3016, 0x09);
    imx290_write_register (IspDev,0x3018, 0x65);
    imx290_write_register (IspDev,0x3019, 0x04);
    imx290_write_register (IspDev,0x301c, 0xF6);
    imx290_write_register (IspDev,0x301d, 0x03);
    imx290_write_register (IspDev,0x3020, 0x02);
    imx290_write_register (IspDev,0x3024, 0xc9);
    imx290_write_register (IspDev,0x3030, 0x0b);
    imx290_write_register (IspDev,0x3045, 0x05);
    imx290_write_register (IspDev,0x3046, 0x00);
    imx290_write_register (IspDev,0x304b, 0x0a);
    imx290_write_register (IspDev,0x305c, 0x18);
    imx290_write_register (IspDev,0x305d, 0x03);
    imx290_write_register (IspDev,0x305e, 0x20);
    imx290_write_register (IspDev,0x305f, 0x01);
    imx290_write_register (IspDev,0x3070, 0x02);
    imx290_write_register (IspDev,0x3071, 0x11);
    imx290_write_register (IspDev,0x309b, 0x10);
    imx290_write_register (IspDev,0x309c, 0x22);
    imx290_write_register (IspDev,0x30a2, 0x02);
    imx290_write_register (IspDev,0x30a6, 0x20);
    imx290_write_register (IspDev,0x30a8, 0x20);
    imx290_write_register (IspDev,0x30aa, 0x20);
    imx290_write_register (IspDev,0x30ac, 0x20);
    imx290_write_register (IspDev,0x30b0, 0x43);
    imx290_write_register (IspDev,0x3106, 0x11);
    imx290_write_register (IspDev,0x3119, 0x9e);
    imx290_write_register (IspDev,0x311c, 0x1e);
    imx290_write_register (IspDev,0x311e, 0x08);
    imx290_write_register (IspDev,0x3128, 0x05);
    imx290_write_register (IspDev,0x3129, 0x1d);
    imx290_write_register (IspDev,0x313d, 0x83);
    imx290_write_register (IspDev,0x3150, 0x03);
    imx290_write_register (IspDev,0x315e, 0x1a);
    imx290_write_register (IspDev,0x3164, 0x1a);
    imx290_write_register (IspDev,0x317c, 0x12);
    imx290_write_register (IspDev,0x317e, 0x00);
    imx290_write_register (IspDev,0x31ec, 0x37);
                                         
    imx290_write_register (IspDev,0x32b8, 0x50);
    imx290_write_register (IspDev,0x32b9, 0x10);
    imx290_write_register (IspDev,0x32ba, 0x00);
    imx290_write_register (IspDev,0x32bb, 0x04);
    imx290_write_register (IspDev,0x32c8, 0x50);
    imx290_write_register (IspDev,0x32c9, 0x10);
    imx290_write_register (IspDev,0x32ca, 0x00);
    imx290_write_register (IspDev,0x32cb, 0x04);
                                        
    imx290_write_register (IspDev,0x332c, 0xd3);
    imx290_write_register (IspDev,0x332d, 0x10);
    imx290_write_register (IspDev,0x332e, 0x0d);
    imx290_write_register (IspDev,0x3358, 0x06);
    imx290_write_register (IspDev,0x3359, 0xe1);
    imx290_write_register (IspDev,0x335a, 0x11);
    imx290_write_register (IspDev,0x3360, 0x1e);
    imx290_write_register (IspDev,0x3361, 0x61);
    imx290_write_register (IspDev,0x3362, 0x10);
    imx290_write_register (IspDev,0x33b0, 0x50);
    imx290_write_register (IspDev,0x33b2, 0x1a);
    imx290_write_register (IspDev,0x33b3, 0x04);
                                         
    imx290_write_register (IspDev,0x3405, 0x00);
    imx290_write_register (IspDev,0x3407, 0x03);
    imx290_write_register (IspDev,0x3414, 0x0a);
    imx290_write_register (IspDev,0x3418, 0xb2);
    imx290_write_register (IspDev,0x3419, 0x08);
    imx290_write_register (IspDev,0x3441, 0x0a);
    imx290_write_register (IspDev,0x3442, 0x0a);
    imx290_write_register (IspDev,0x3443, 0x03);
    imx290_write_register (IspDev,0x3444, 0x20);
    imx290_write_register (IspDev,0x3445, 0x25);
    imx290_write_register (IspDev,0x3446, 0x77);
    imx290_write_register (IspDev,0x3447, 0x00);
    imx290_write_register (IspDev,0x3448, 0x67);
    imx290_write_register (IspDev,0x3449, 0x00);
    imx290_write_register (IspDev,0x344a, 0x47);
    imx290_write_register (IspDev,0x344b, 0x00);
    imx290_write_register (IspDev,0x344c, 0x37);
    imx290_write_register (IspDev,0x344d, 0x00);
    imx290_write_register (IspDev,0x344e, 0x3f);
    imx290_write_register (IspDev,0x344f, 0x00);
    imx290_write_register (IspDev,0x3450, 0xff);
    imx290_write_register (IspDev,0x3451, 0x00);
    imx290_write_register (IspDev,0x3452, 0x3f);
    imx290_write_register (IspDev,0x3453, 0x00);
    imx290_write_register (IspDev,0x3454, 0x37);
    imx290_write_register (IspDev,0x3455, 0x00);
    imx290_write_register (IspDev,0x3472, 0x9c);
    imx290_write_register (IspDev,0x3473, 0x07);
    imx290_write_register (IspDev,0x3480, 0x49);

    imx290_write_register (IspDev,0x3000, 0x00); /* standby */
    delay_ms(20); 
    imx290_write_register (IspDev,0x3002, 0x00); /* master mode start */
 
    printf("===Imx290 imx290 1080P60fps 10bit 2to1 WDR(120fps->60fps) init success!=====\n");
    return;
}

void imx290_wdr_1080p120_3to1_init(ISP_DEV IspDev)
{
    imx290_write_register (IspDev,0x3000, 0x01); /* standby */
    imx290_write_register (IspDev,0x3002, 0x01); /* XTMSTA */

    imx290_write_register (IspDev,0x3005, 0x00);
    imx290_write_register (IspDev,0x3007, 0x00);
    imx290_write_register (IspDev,0x3009, 0x00);
    imx290_write_register (IspDev,0x300a, 0x3c);
    imx290_write_register (IspDev,0x300c, 0x21);
    imx290_write_register (IspDev,0x300f, 0x00);
    imx290_write_register (IspDev,0x3010, 0x21);
    imx290_write_register (IspDev,0x3012, 0x64);
    imx290_write_register (IspDev,0x3016, 0x09);
    imx290_write_register (IspDev,0x3018, 0x65);
    imx290_write_register (IspDev,0x3019, 0x04);
    imx290_write_register (IspDev,0x301c, 0x4c);
    imx290_write_register (IspDev,0x301d, 0x04);

#if 1
    imx290_write_register (IspDev,0x3020, 0x04);  /* SHS1 */
    imx290_write_register (IspDev,0x3021, 0x00);
    //imx290_write_register (IspDev,0x3022, 0x00);
    imx290_write_register (IspDev,0x3024, 0xf2);  /* SHS2 */ 
    imx290_write_register (IspDev,0x3025, 0x01);
    //imx290_write_register (IspDev,0x3025, 0x00);
    imx290_write_register (IspDev,0x3028, 0x57);  /* SHS3 */
    imx290_write_register (IspDev,0x3029, 0x02);
    //imx290_write_register (IspDev,0x302A, 0x00);
    imx290_write_register (IspDev,0x3030, 0xed);  /* RHS1 */
    imx290_write_register (IspDev,0x3031, 0x01);
    imx290_write_register (IspDev,0x3034, 0x30);  /* RHS2 */ 
    imx290_write_register (IspDev,0x3035, 0x02);
#else
    imx290_write_register (IspDev,0x3020, 0x04);
    imx290_write_register (IspDev,0x3024, 0x89);
    imx290_write_register (IspDev,0x3028, 0x93);
    imx290_write_register (IspDev,0x3029, 0x01);
    imx290_write_register (IspDev,0x3030, 0x85);
    imx290_write_register (IspDev,0x3034, 0x92);
#endif

    imx290_write_register (IspDev,0x3045, 0x05);
    imx290_write_register (IspDev,0x3046, 0x00);
    imx290_write_register (IspDev,0x304b, 0x0a);
    imx290_write_register (IspDev,0x305c, 0x18);
    imx290_write_register (IspDev,0x305d, 0x03);
    imx290_write_register (IspDev,0x305e, 0x20);
    imx290_write_register (IspDev,0x305f, 0x01);
    imx290_write_register (IspDev,0x3070, 0x02);
    imx290_write_register (IspDev,0x3071, 0x11);
    imx290_write_register (IspDev,0x309b, 0x10);
    imx290_write_register (IspDev,0x309c, 0x22);
    imx290_write_register (IspDev,0x30a2, 0x02);
    imx290_write_register (IspDev,0x30a6, 0x20);
    imx290_write_register (IspDev,0x30a8, 0x20);
    imx290_write_register (IspDev,0x30aa, 0x20);
    imx290_write_register (IspDev,0x30ac, 0x20);
    imx290_write_register (IspDev,0x30b0, 0x43);
    imx290_write_register (IspDev,0x3106, 0x33);
    imx290_write_register (IspDev,0x3119, 0x9e);
    imx290_write_register (IspDev,0x311c, 0x1e);
    imx290_write_register (IspDev,0x311e, 0x08);
    imx290_write_register (IspDev,0x3128, 0x05);
    imx290_write_register (IspDev,0x3129, 0x1d);
    imx290_write_register (IspDev,0x313d, 0x83);
    imx290_write_register (IspDev,0x3150, 0x03);
    imx290_write_register (IspDev,0x315e, 0x1a);
    imx290_write_register (IspDev,0x3164, 0x1a);
    imx290_write_register (IspDev,0x317c, 0x12);
    imx290_write_register (IspDev,0x317e, 0x00);
    imx290_write_register (IspDev,0x31ec, 0x37);
                                         
    imx290_write_register (IspDev,0x32b8, 0x50);
    imx290_write_register (IspDev,0x32b9, 0x10);
    imx290_write_register (IspDev,0x32ba, 0x00);
    imx290_write_register (IspDev,0x32bb, 0x04);
    imx290_write_register (IspDev,0x32c8, 0x50);
    imx290_write_register (IspDev,0x32c9, 0x10);
    imx290_write_register (IspDev,0x32ca, 0x00);
    imx290_write_register (IspDev,0x32cb, 0x04);
                                        
    imx290_write_register (IspDev,0x332c, 0xd3);
    imx290_write_register (IspDev,0x332d, 0x10);
    imx290_write_register (IspDev,0x332e, 0x0d);
    imx290_write_register (IspDev,0x3358, 0x06);
    imx290_write_register (IspDev,0x3359, 0xe1);
    imx290_write_register (IspDev,0x335a, 0x11);
    imx290_write_register (IspDev,0x3360, 0x1e);
    imx290_write_register (IspDev,0x3361, 0x61);
    imx290_write_register (IspDev,0x3362, 0x10);
    imx290_write_register (IspDev,0x33b0, 0x50);
    imx290_write_register (IspDev,0x33b2, 0x1a);
    imx290_write_register (IspDev,0x33b3, 0x04);
                                         
    imx290_write_register (IspDev,0x3405, 0x00);
    imx290_write_register (IspDev,0x3407, 0x03);
    imx290_write_register (IspDev,0x3414, 0x0a);
    imx290_write_register (IspDev,0x3418, 0x55);
    imx290_write_register (IspDev,0x3419, 0x11);
    imx290_write_register (IspDev,0x3441, 0x0a);
    imx290_write_register (IspDev,0x3442, 0x0a);
    imx290_write_register (IspDev,0x3443, 0x03);
    imx290_write_register (IspDev,0x3444, 0x20);
    imx290_write_register (IspDev,0x3445, 0x25);
    imx290_write_register (IspDev,0x3446, 0x77);
    imx290_write_register (IspDev,0x3447, 0x00);
    imx290_write_register (IspDev,0x3448, 0x67);
    imx290_write_register (IspDev,0x3449, 0x00);
    imx290_write_register (IspDev,0x344a, 0x47);
    imx290_write_register (IspDev,0x344b, 0x00);
    imx290_write_register (IspDev,0x344c, 0x37);
    imx290_write_register (IspDev,0x344d, 0x00);
    imx290_write_register (IspDev,0x344e, 0x3f);
    imx290_write_register (IspDev,0x344f, 0x00);
    imx290_write_register (IspDev,0x3450, 0xff);
    imx290_write_register (IspDev,0x3451, 0x00);
    imx290_write_register (IspDev,0x3452, 0x3f);
    imx290_write_register (IspDev,0x3453, 0x00);
    imx290_write_register (IspDev,0x3454, 0x37);
    imx290_write_register (IspDev,0x3455, 0x00);
    imx290_write_register (IspDev,0x3472, 0x9c);
    imx290_write_register (IspDev,0x3473, 0x07);
    imx290_write_register (IspDev,0x3480, 0x49);

    imx290_write_register (IspDev,0x3000, 0x00); /* standby */
    delay_ms(20); 
    imx290_write_register (IspDev,0x3002, 0x00); /* master mode start */
 
    printf("===Imx290 imx290 1080P30fps 10bit 3to1 WDR(120fps->30fps) init success!=====\n");

    return;
}


