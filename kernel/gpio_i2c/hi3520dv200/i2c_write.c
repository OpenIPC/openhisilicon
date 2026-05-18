
#include <stdio.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "strfunc.h"
#include "gpio_i2c.h"


int main(int argc , char* argv[])
{
	int fd = -1;
	int ret =0;
    int value;
	unsigned int device_addr, reg_addr, reg_value, data1, data2;
		
	if(argc < 4)
    {
    	printf("usage: %s <device_addr> <reg_addr> <value>. sample: %s 0x56 0x0 0x28\n", argv[0], argv[0]);
        return -1;
    }
	
	fd = open("/dev/gpioi2c", 0);
    if(fd<0)
    {
    	printf("Open gpioi2c error!\n");
    	return -1;
    }
    
    if (StrToNumber(argv[1], &device_addr))
    {    	
    	return 0;
    }
    
    
    if (StrToNumber(argv[2], &reg_addr))
    {    
    	return 0;
    }
    
    if (StrToNumber(argv[3], &reg_value))
    {    
    	return 0;
    }
    if(argc==4)
    {
        printf("device_addr:0x%x; reg_addr:0x%x; reg_value:0x%x.\n", device_addr, reg_addr, reg_value);
        
        value = ((device_addr&0xff)<<24) | ((reg_addr&0xff)<<16) | (reg_value&0xffff);
        
        ret = ioctl(fd, GPIO_I2C_WRITE_BYTE, &value);
	}
	else
    {
    	cx25838_regs_data value1;
    	StrToNumber(argv[4], &data1);
    	StrToNumber(argv[5], &data2);
    	value1.chip=device_addr;
    	value1.addr=reg_addr;
    	value1.lobit=reg_value;
    	value1.hibit=data1;
    	value1.data=data2;
    	
    	ret = ioctl(fd, GPIO_I2C_WRITE_DWORD, &value1);
	}
        
    return 0;
}
