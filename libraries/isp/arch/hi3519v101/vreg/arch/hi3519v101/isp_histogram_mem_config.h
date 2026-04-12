/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_histogram_mem_config.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2015/05/15
  Description   : 
  History       :
  1.Date        : 2015/05/15
    Author      : 
    Modification: Created file

******************************************************************************/

#ifndef __ISP_HISTOGRAM_MEM_CONFIG_H__
#define __ISP_HISTOGRAM_MEM_CONFIG_H__

#include "hi_vreg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

// ------------------------------------------------------------------------------ //
// Instance 'histogram_mem' of module 'histogram_mem'
// ------------------------------------------------------------------------------ //

#define HI_HISTOGRAM_MEM_BASE_ADDR (0x14000)
#define HI_HISTOGRAM_MEM_SIZE (0x1000)

#define HI_HISTOGRAM_MEM_ARRAY_DATA_DEFAULT (0x0)
#define HI_HISTOGRAM_MEM_ARRAY_DATA_DATASIZE (32)

// args: index (0-1023), data (32-bit)
static __inline HI_VOID hi_histogram_mem_array_data_write(ISP_DEV IspDev, HI_U32 index, HI_U32 data) {
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x14000 + (index << 2), data);
}
static __inline HI_U32 hi_histogram_mem_array_data_read(ISP_DEV IspDev, HI_U32 index) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x14000 + (index << 2));
}
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Instance 'histogram_1_mem' of module 'histogram256_mem'
// ------------------------------------------------------------------------------ //

#define HI_HISTOGRAM_1_MEM_BASE_ADDR (0x15000)
#define HI_HISTOGRAM_1_MEM_SIZE (0x400)

#define HI_HISTOGRAM_1_MEM_ARRAY_DATA_DEFAULT (0x0)
#define HI_HISTOGRAM_1_MEM_ARRAY_DATA_DATASIZE (32)

// args: index (0-255), data (32-bit)
static __inline HI_VOID hi_histogram_1_mem_array_data_write(ISP_DEV IspDev, HI_U32 index, HI_U32 data) {
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x15000 + (index << 2), data);
}
static __inline HI_U32 hi_histogram_1_mem_array_data_read(ISP_DEV IspDev, HI_U32 index) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x15000 + (index << 2));
}

// ------------------------------------------------------------------------------ //
// Instance 'histogram_2_mem' of module 'histogram256_mem'
// ------------------------------------------------------------------------------ //

#define HI_HISTOGRAM_2_MEM_BASE_ADDR (0x15400)
#define HI_HISTOGRAM_2_MEM_SIZE (0x400)

#define HI_HISTOGRAM_2_MEM_ARRAY_DATA_DEFAULT (0x0)
#define HI_HISTOGRAM_2_MEM_ARRAY_DATA_DATASIZE (32)

// args: index (0-255), data (32-bit)
static __inline HI_VOID hi_histogram_2_mem_array_data_write(ISP_DEV IspDev, HI_U32 index, HI_U32 data) {
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x15400 + (index << 2), data);
}
static __inline HI_U32 hi_histogram_2_mem_array_data_read(ISP_DEV IspDev, HI_U32 index) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x15400 + (index << 2));
}

// ------------------------------------------------------------------------------ //
// Instance 'histogram_3_mem' of module 'histogram256_mem'
// ------------------------------------------------------------------------------ //

#define HI_HISTOGRAM_3_MEM_BASE_ADDR (0x15800)
#define HI_HISTOGRAM_3_MEM_SIZE (0x400)

#define HI_HISTOGRAM_3_MEM_ARRAY_DATA_DEFAULT (0x0)
#define HI_HISTOGRAM_3_MEM_ARRAY_DATA_DATASIZE (32)

// args: index (0-255), data (32-bit)
static __inline HI_VOID hi_histogram_3_mem_array_data_write(ISP_DEV IspDev, HI_U32 index, HI_U32 data) {
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x15800 + (index << 2), data);
}
static __inline HI_U32 hi_histogram_3_mem_array_data_read(ISP_DEV IspDev, HI_U32 index) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x15800 + (index << 2));
}
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Instance 'histogram_4_mem' of module 'histogram256_mem'
// ------------------------------------------------------------------------------ //

#define HI_HISTOGRAM_4_MEM_BASE_ADDR (0x15c00)
#define HI_HISTOGRAM_4_MEM_SIZE (0x400)

#define HI_HISTOGRAM_4_MEM_ARRAY_DATA_DEFAULT (0x0)
#define HI_HISTOGRAM_4_MEM_ARRAY_DATA_DATASIZE (32)

// args: index (0-255), data (32-bit)
static __inline HI_VOID hi_histogram_4_mem_array_data_write(ISP_DEV IspDev, HI_U32 index, HI_U32 data) {
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x15c00 + (index << 2), data);
}
static __inline HI_U32 hi_histogram_4_mem_array_data_read(ISP_DEV IspDev, HI_U32 index) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x15c00 + (index << 2));
}
// ------------------------------------------------------------------------------ //

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __ISP_HISTOGRAM_1_MEM_CONFIG_H__ */
