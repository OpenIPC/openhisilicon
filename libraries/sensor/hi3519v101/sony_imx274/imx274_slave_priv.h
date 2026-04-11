/******************************************************************************

  Copyright (C), 2001-2015, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : imx274_slave_priv.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2015/06/18
  Description   : this file was private for imx226 slave mode sensor
  History       :
  1.Date        : 
    Author      : 
    Modification: Created file
******************************************************************************/
#ifndef __IMX274_SLAVE_PRIV_H_
#define __IMX274_SLAVE_PRIV_H_

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

#define IMX274_RES_IS_4K(w, h)  (w <= 3840 && h <= 2160)
#define IMX274_RES_IS_12M(w, h) (w <= 4000 && h <= 3000)
#define IMX274_RES_IS_2K(w, h)  (w <= 2048 && h <= 1080)

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
                                                                                     
// registers to control exposure            
#define     IMX274_SHR_LINEAR           (0x000C)
#define     IMX274_PGC                  (0x000A)
#define     IMX274_SVR                  (0x000E)
#define     IMX274_DGAIN                (0x0012)
#define     IMX274_SHR_DOL1             (0x002E)  // Shutter 1 for DOL For short exposure frame
#define     IMX274_SHR_DOL2             (0x0030)  // Shutter 2 for DOL For long exposure frame
#define     IMX274_RHS1                 (0x0032)  // Designate the readout for DOL HDR For short exposure frame


typedef struct hiIMX274_SENSOR_REG_S{

    HI_U16 u16Addr;
    HI_U8  u8Data;
} IMX274_SENSOR_REG_S;

typedef enum {
    IMX274_MODE0 = 0,    // mode 0: 3840x2160 6ch 12bit 29.97fps.
    IMX274_MODE1_DOL,    // mode 1 DOL: 3840x2160 10ch 10bit 60fps to 30fps.

    IMX274_MODE_BUTT
    
} IMX274_RES_MODE_E;

typedef struct hiIMX274_VIDEO_MODE_TBL_S 
{
    HI_U32  u32Inck;
    HI_U32  u32InckPerHs;
    HI_U32  u32InckPerVs;
    HI_U32  u32VertiLines;
   
    HI_U32  u32MaxFps;
    const char *pszModeName;
    
} IMX274_VIDEO_MODE_TBL_S;


#endif /* __IMX274_SLAVE_PRIV_H_ */
