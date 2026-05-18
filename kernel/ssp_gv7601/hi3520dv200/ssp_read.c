/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : ssp_read.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/09/27
  Description   : 
  History       :
  1.Date        : 2010/09/27
    Author      : x00100808
    Modification: Created file

******************************************************************************/

#include <stdio.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "strfunc.h"
#include "hi_ssp.h"


int main(int argc , char* argv[])
{
	int fd = -1;
	int ret;
	unsigned int reg_addr, reg_value;
		
	if(argc != 2)
    {
    	printf("usage: %s <addr>. sample: %s 0x2010\n", argv[0], argv[0]);
        return -1;
    }
	
	fd = open("/dev/ssp", 0);
    if(fd<0)
    {
    	printf("Open /dev/ssp error!\n");
    	return -1;
    }
    
    if (StrToNumber(argv[1], &reg_addr))
    {    	
    	return 0;
    }
    
    reg_value = reg_addr;
    
    ret = ioctl(fd, SSP_READ_ALT, &reg_value);

    printf("ssp_read %#x = %#x\n", reg_addr, reg_value);

	close(fd);
        
    return 0;
}



