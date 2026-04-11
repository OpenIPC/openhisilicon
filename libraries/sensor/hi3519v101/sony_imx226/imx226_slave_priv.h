/******************************************************************************

  Copyright (C), 2001-2015, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : imx226_slave_priv.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2015/06/18
  Description   : this file was private for imx226 slave mode sensor
  History       :
  1.Date        : 
    Author      : 
    Modification: Created file
******************************************************************************/
#ifndef __IMX226_SLAVE_PRIV_H_
#define __IMX226_SLAVE_PRIV_H_

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif


// usefull macro
#define HIGH_8BITS(x) ((x & 0xff00) >> 8)
#define LOW_8BITS(x)  (x & 0x00ff)
#ifndef MAX
#define MAX(a, b) (((a) < (b)) ?  (b) : (a))
#endif

#ifndef MIN
#define MIN(a, b) (((a) > (b)) ?  (b) : (a))
#endif

#define IMX226_RES_IS_4K30(w, h)    (w <= 3840 && h <= 2160)
#define IMX226_RES_IS_4K60(w, h)    (w <= 4096 && h <= 2160)
#define IMX226_RES_IS_12M(w, h)     (w <= 4000 && h <= 3000)
#define IMX226_RES_IS_2K(w, h)      (w <= 2048 && h <= 1080)

#define CHECK_RET(express)\
do{\
    HI_S32 s32Ret;\
    s32Ret = express;\
    if (HI_SUCCESS != s32Ret)\
    {\
        printf("Failed at %s: LINE: %d with %#x!\n", __FUNCTION__, __LINE__, s32Ret);\
    }\
}while(0)

/*
--------------------------------------------------------------------------------
- Structure For Slave Mode Sensor Using
--------------------------------------------------------------------------------
*/
#define     FULL_LINES_MAX              (0xFFFF)

// difference registers in various mode
#define     IMX226_VIDEO_MODE_REG_NUM   (0x0004)
#define     IMX226_03                   (0x0003)           
#define     IMX226_MDSEL1               (0x0004)      
#define     IMX226_MDSEL2               (0x0005)
#define     IMX226_MDSEL6               (0x00B6)
                                            
                                            
// registers to control exposure            
#define     IMX226_SHR_L                (0x000b)
#define     IMX226_SHR_H                (0x000c)
#define     IMX226_PGC_L                (0x0009)
#define     IMX226_PGC_H                (0x000a)
#define     IMX226_APGC01_L             (0x0352)
#define     IMX226_APGC01_H             (0x0353)
#define     IMX226_APGC02_L             (0x0356)
#define     IMX226_APGC02_H             (0x0357)
#define     IMX226_SVR_L                (0x000d)
#define     IMX226_SVR_H                (0x000e)
#define     IMX226_DGAIN                (0x0011)


typedef struct hiIMX226_SENSOR_REG_S{

    HI_U16 u16Addr;
    HI_U8  u8Data;
} IMX226_SENSOR_REG_S;

typedef enum {
    IMX226_MODE0 = 0,    // mode 0: 4000x3000 10ch 12bit 34.97fps
    IMX226_MODE1,        // mode 1: 4000x3000 10ch 10bit 39.96fps
    IMX226_MODE2,        // mode 2: 4096X2160 8ch 12bit 29.97fps
    IMX226_MODE3,        // mode 3: 4096X2160 4ch 12bit 14.99fps
    IMX226_MODE4,        // mode 4: 4096X2160 10ch 10bit 59.94fps
    IMX226_MODE5,        // mode 5: 2x2 binning 2048x1080 4ch 10bit 59.94fps

    IMX226_MODE_BUTT
    
} IMX226_RES_MODE_E;

typedef struct hiIMX226_VIDEO_MODE_TBL_S 
{
    HI_U32  u32Inck;
    HI_U32  u32InckPerHs;
    HI_U32  u32InckPerVs;
    HI_U32  u32VertiLines;
   
    IMX226_SENSOR_REG_S astVideoModeReg[IMX226_VIDEO_MODE_REG_NUM];
    HI_U32  u32MaxFps;
    const char *pszModeName;
    
} IMX226_VIDEO_MODE_TBL_S;


#endif /* __IMX226_SLAVE_PRIV_H_ */
