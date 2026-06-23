/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef OT_PIRIS_H
#define OT_PIRIS_H

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

#define piris_get_dev(f)             (*((td_u32*)(f)))

#define PIRIS_MAX_DEV_NUM  2

#define piris_check_pointer_return(ptr) \
    do { \
        if ((ptr) == TD_NULL) { \
            return (-1); \
        } \
    } while (0)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  /* OT_PIRIS_H */
