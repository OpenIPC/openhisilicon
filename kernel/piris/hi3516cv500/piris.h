/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of piris.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __HI_PIRIS_H__
#define __HI_PIRIS_H__

#include <linux/ioctl.h> /* needed for the _IOW etc stuff used later */

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef enum {
    PIRIS_IDLE = 0,
    PIRIS_BUSY,
    PIRIS_BUTT,
} piris_status;

typedef struct {
    unsigned char zero_is_max;
    unsigned int  total_step;
    int  cur_pos;
} piris_data;

#define PIRIS_IOC_MAGIC    'p'

#define PIRIS_SET_ACT_ARGS _IOW(PIRIS_IOC_MAGIC,  1, int)
#define PIRIS_SET_ORGIN    _IOW(PIRIS_IOC_MAGIC,  2, int)
#define PIRIS_SET_CLOSE    _IOW(PIRIS_IOC_MAGIC,  3, int)
#define PIRIS_SET_FD       _IOW(PIRIS_IOC_MAGIC,  4, int)
#define PIRIS_GET_STATUS   _IOR(PIRIS_IOC_MAGIC,  5, int)

#define PIRIS_IOC_MAXNR    5

#define piris_get_dev(f)             (*((hi_u32*)(f)))

#define PIRIS_MAX_DEV_NUM  2

#define PIRIS_CHECK_POINTER(ptr)\
    do {\
        if ((ptr) == HI_NULL) {\
             \
            return -1; \
        }\
    }while (0)


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  /* __HI_PIRIS_H__ */
