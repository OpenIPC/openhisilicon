/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#ifndef __ISP_PIRIS_H__
#define __ISP_PIRIS_H__

#include <linux/ioctl.h> /* needed for the _IOW etc stuff used later */

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef enum PIRIS_STATUS_E {
    PIRIS_IDLE = 0,
    PIRIS_BUSY,

    PIRIS_BUTT,
} PIRIS_STATUS_E;


typedef struct PIRIS_DATA_S {
    unsigned char ZeroIsMax;
    unsigned int  TotalStep;
    int  CurPos;
} PIRIS_DATA_S;



#define PIRIS_IOC_MAGIC    'p'

#define PIRIS_SET_ACT_ARGS _IOW(PIRIS_IOC_MAGIC,  1, int)
#define PIRIS_SET_ORGIN    _IOW(PIRIS_IOC_MAGIC,  2, int)
#define PIRIS_SET_CLOSE    _IOW(PIRIS_IOC_MAGIC,  3, int)
#define PIRIS_SET_FD       _IOW(PIRIS_IOC_MAGIC,  4, int)
#define PIRIS_GET_STATUS   _IOR(PIRIS_IOC_MAGIC,  5, int)

#define PIRIS_IOC_MAXNR    5

#define PIRIS_GET_DEV(f)             (*((GK_U32*)(f)))

#define PIRIS_MAX_DEV_NUM  2


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  /* __ISP_PIRIS_H__ */
