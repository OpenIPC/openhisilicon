/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_sharpen_mem_config.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/01/16
  Description   : 
  History       :
  1.Date        : 2013/01/16
    Author      : n00168968
    Modification: Created file

******************************************************************************/

#ifndef __ISP_DRC_MEM_CONFIG_H__
#define __ISP_DRC_MEM_CONFIG_H__

#include "hi_vreg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

// ------------------------------------------------------------------------------ //
// Instance 'drc_fwd_mem' of module 'drc_fwd_mem'
// ------------------------------------------------------------------------------ //

#define HI_DRC_FWD_MEM_BASE_ADDR (0x18000L)
#define HI_DRC_FWD_MEM_SIZE (0x400)

#define HI_DRC_FWD_MEM_ARRAY_DATA_DEFAULT (0x0)
#define HI_DRC_FWD_MEM_ARRAY_DATA_DATASIZE (32)

// args: index (0-128), data (32-bit)
static __inline HI_VOID hi_drc_fwd_mem_array_data_write(ISP_DEV IspDev, HI_U32 index, HI_U32 data) {
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x18000L + (index << 2), data);
}
static __inline HI_U32 hi_drc_fwd_mem_array_data_read(ISP_DEV IspDev, HI_U32 index) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x18000L + (index << 2));
}

// ------------------------------------------------------------------------------ //
// Instance 'drc_rev_mem' of module 'drc_rev_mem'
// ------------------------------------------------------------------------------ //

#define HI_DRC_REV_MEM_BASE_ADDR (0x18800L)
#define HI_DRC_REV_MEM_SIZE (0x400)

#define HI_DRC_REV_MEM_ARRAY_DATA_DEFAULT (0x0)
#define HI_DRC_REV_MEM_ARRAY_DATA_DATASIZE (32)

// args: index (0-128), data (32-bit)
static __inline HI_VOID hi_drc_rev_mem_array_data_write(ISP_DEV IspDev, HI_U32 index, HI_U32 data) {
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x18800L + (index << 2), data);
}
static __inline HI_U32 hi_drc_rev_mem_array_data_read(ISP_DEV IspDev, HI_U32 index) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x18800L + (index << 2));
}

// ------------------------------------------------------------------------------ //
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __ISP_DRC_MEM_CONFIG_H__ */

