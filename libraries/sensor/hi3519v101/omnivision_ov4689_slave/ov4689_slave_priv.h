/******************************************************************************

  Copyright (C), 2001-2016, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : OV4689_slave_priv.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2016/03/07
  Description   : this file was private for OV4689 slave mode sensor
  History       :
  1.Date        : 
    Author      : xgl
    Modification: Created file
******************************************************************************/
#ifndef __OV4689_SLAVE_PRIV_H_
#define __OV4689_SLAVE_PRIV_H_

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

#define OV4689_RES_IS_4M(w, h)  (w <= 2592 && h <= 1520)
#define OV4689_RES_IS_2K(w, h)  (w <= 1920 && h <= 1080)


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

// difference registers 
#define OV4689_FULL_LINES_MAX       (0x7FFF)

// registers to control exposure and gain           
#define     OV4689_EXPO_L            (0x3502)
#define     OV4689_EXPO_M            (0x3501)
#define     OV4689_EXPO_H            (0x3500)
#define     OV4689_VMAX_L            (0x380F)
#define     OV4689_VMAX_H            (0x380E)
#define     OV4689_AGAIN_REG         (0x3508)
#define     OV4689_AGAIN             (0x3509)
#define     OV4689_R_RST_FSIN_H      (0x3826)
#define     OV4689_R_RST_FSIN_L      (0x3827)
#define     OV4689_FSIN_ENABLE       (0x3823)

typedef enum {
    OV4689_MODE0 = 0,    // mode 0: 1920x1080  12bit 30fps
    OV4689_MODE1,       // mode 1:  2592x1520   12bit 30fps

    OV4689_MODE_BUTT
    
} OV4689_RES_MODE_E;

typedef struct hiOV4689_VIDEO_MODE_TBL_S 
{
    HI_U32  u32InckPerVs;
    HI_U32  u32VertiLines;
    HI_U32  u32MaxFps;
    const char *pszModeName;
    
} OV4689_VIDEO_MODE_TBL_S;
                                                                                   
#endif /* __OV4689_SLAVE_PRIV_H_ */

