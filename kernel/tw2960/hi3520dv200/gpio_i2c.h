
#ifndef _GPIO_I2C_H
#define _GPIO_I2C_H

#define GPIO_I2C_MAGIC_BASE	'I'
#define GPIO_I2C_READ_BYTE   _IOR(GPIO_I2C_MAGIC_BASE,0x01,int)
#define GPIO_I2C_WRITE_BYTE  _IOW(GPIO_I2C_MAGIC_BASE,0x02,int)

#define GPIO_I2C_READ_DWORD   _IOR(GPIO_I2C_MAGIC_BASE,0x03,int)
#define GPIO_I2C_WRITE_DWORD  _IOR(GPIO_I2C_MAGIC_BASE,0x04,int)


typedef struct _cx25838_regs_data
{
	unsigned int chip;	//0x88 or 0x8a
	unsigned int addr;	//reg address
	unsigned int hibit;
	unsigned int lobit;
	unsigned int data;
} cx25838_regs_data;

typedef struct _cx25838_wdata
{
	unsigned int chip;	//0x88 or 0x8a
	unsigned int addr;	//reg address
	unsigned int vdata;
	unsigned int data;
} cx25838_wdata;



unsigned char gpio_i2c_read(unsigned char devaddress, unsigned char address);
void          gpio_i2c_write(unsigned char devaddress, unsigned char address, unsigned char value);
unsigned int  gpio_i2c2_read(unsigned char devaddress, unsigned short address, int num_bytes);
void          gpio_i2c2_write(unsigned char devaddress, unsigned short address, unsigned int data, int num_bytes);

unsigned int  gpio_i2c2_read_1bytesubaddr(unsigned char devaddress, unsigned char address, int num_bytes);
void          gpio_i2c2_write_1bytesubaddr(unsigned char devaddress, unsigned char address, unsigned int data, int num_bytes);

#endif

