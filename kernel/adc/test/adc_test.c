/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "adc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif/*__cplusplus*/


#define DEV_FILE "/dev/lsadc"

typedef enum LSADC_SCAN_MODE
{
    LSADC_SCAN_MODE_SINGLE_STEP,
    LSADC_SCAN_MODE_CONTINUOUS,
    LSADC_SCAN_MODE_BUTT
}LSADC_SCAN_MODE;

int Sample_LSADC_SingleStepScanMode(LSADC_SCAN_MODE enAdcMode, int chn)
{
    int i, value;
    int fd  = open(DEV_FILE, O_RDWR);

    if (fd < 0)
    {
        fprintf(stderr, "fail to open file:%s\n", DEV_FILE);
        return -1;
    }

    if(ioctl(fd, LSADC_IOC_MODEL_SEL, &enAdcMode) <0)
    {
        fprintf(stderr, "adc model select error.\n");
        goto exit;
    }

    if(ioctl(fd, LSADC_IOC_CHN_ENABLE, &chn) <0)
    {
        fprintf(stderr, "enable chn %d error.\n", chn);
        goto exit;
    }


    for(i=0; i<20; i++)
    {
        if(ioctl(fd, LSADC_IOC_START) <0)
        {
            fprintf(stderr, "start lsadc error.\n");
            goto exit;
        }

        value = ioctl(fd, LSADC_IOC_GET_CHNVAL, &chn);
        printf("get value:%d,chn[%d]\n", value,chn);
        sleep(1);

        if(ioctl(fd, LSADC_IOC_STOP) <0)
        {
            fprintf(stderr, "stop lsadc error.\n");
        }
    }

exit:
    if(ioctl(fd, LSADC_IOC_STOP) <0)
    {
        fprintf(stderr, "stop lsadc error.\n");
    }

    if(ioctl(fd, LSADC_IOC_CHN_DISABLE, &chn) <0)
    {
        fprintf(stderr, "disable chn %d error.\n", chn);
    }

    close(fd);
    return 0;
}

int Sample_LSADC_ContinuousScanMode(LSADC_SCAN_MODE enAdcMode, int chn)
{
    int i, value;
    int fd  = open(DEV_FILE, O_RDWR);

    if (fd < 0)
    {
        fprintf(stderr, "fail to open file:%s\n", DEV_FILE);
        return -1;
    }

    if(ioctl(fd, LSADC_IOC_MODEL_SEL, &enAdcMode) <0)
    {
        fprintf(stderr, "adc model select error.\n");
        goto exit;
    }

    if(ioctl(fd, LSADC_IOC_CHN_ENABLE, &chn) <0)
    {
        fprintf(stderr, "enable chn %d error.\n", chn);
        goto exit;
    }

    if(ioctl(fd, LSADC_IOC_START) <0)
    {
        fprintf(stderr, "start lsadc error.\n");
        goto exit;
    }


    for(i = 0; i < 20; i++)
    {
        value = ioctl(fd, LSADC_IOC_GET_CHNVAL, &chn);
        printf("get value:%d,chn[%d]\n", value,chn);
        sleep(1);
    }

exit:
    if(ioctl(fd, LSADC_IOC_STOP) <0)
    {
        fprintf(stderr, "stop lsadc error.\n");
    }

    if(ioctl(fd, LSADC_IOC_CHN_DISABLE, &chn) <0)
    {
        fprintf(stderr, "disable chn %d error.\n", chn);
    }

    close(fd);
    return 0;
}

#ifdef __LITEOS__
int app_main(int argc, char *argv[])
#else
int main(int argc, char* argv[])
#endif
{
    LSADC_SCAN_MODE enAdcMode = LSADC_SCAN_MODE_BUTT;
    int chn = 0;

    printf("select scan mode:\n");
    printf(" - [0] is single step scan mode\n");
    printf(" - [1] is Continuous scan mode\n");

    enAdcMode = getchar() - '0';

    getchar();
    printf("select ADC CHN:\n");
    printf(" - [0] is CHN[0]\n");
    printf(" - [1] is CHN[1]\n");

    chn = getchar() - '0';

    switch (enAdcMode)
    {
        case LSADC_SCAN_MODE_SINGLE_STEP:
            Sample_LSADC_SingleStepScanMode(enAdcMode, chn);
            break;
        case LSADC_SCAN_MODE_CONTINUOUS:
            Sample_LSADC_ContinuousScanMode(enAdcMode, chn);
            break;
        default:
            printf("error scan mode\n");
            break;
    }
    return 0;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif/*__cplusplus*/


