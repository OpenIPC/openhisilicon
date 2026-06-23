/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef PIRIS_HAL_INTF_H
#define PIRIS_HAL_INTF_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/* base address */
#define PIRISI_ADRESS_BASE BASE_ADDR
int piris_drv_write(void *piris_ibase, unsigned char bits);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* PIRIS_HAL_INTF_H */

