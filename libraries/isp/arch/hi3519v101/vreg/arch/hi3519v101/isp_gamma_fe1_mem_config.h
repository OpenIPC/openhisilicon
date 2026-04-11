/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_gamma_fe1_mem_config.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2015/05/15
  Description   : 
  History       :
  1.Date        : 2015/05/15
    Author      : 
    Modification: Created file

******************************************************************************/

#ifndef __ISP_GAMMA_FE1_MEM_CONFIG_H__
#define __ISP_GAMMA_FE1_MEM_CONFIG_H__

#include "hi_vreg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

// ------------------------------------------------------------------------------ //
// Instance 'gamma_fe1_mem' of module 'gamma_fe1_mem'
// ------------------------------------------------------------------------------ //

#define HI_GAMMA_FE1_MEM_BASE_ADDR (0x3000)
#define HI_GAMMA_FE1_MEM_SIZE (0x1000)

#define HI_GAMMA_FE1_MEM_ARRAY_DATA_DEFAULT (0x0)
#define HI_GAMMA_FE1_MEM_ARRAY_DATA_DATASIZE (32)

// args: index (0-768), data (32-bit)
static __inline HI_VOID hi_gamma_fe1_mem_array_data_write(ISP_DEV IspDev, HI_U32 index, HI_U32 data) {
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3000 + (index << 2), data);
}
static __inline HI_U32 hi_gamma_fe1_mem_array_data_read(ISP_DEV IspDev, HI_U32 index) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x3000 + (index << 2));
}
// ------------------------------------------------------------------------------ //

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __ISP_GAMMA_FE1_MEM_CONFIG_H__ */
