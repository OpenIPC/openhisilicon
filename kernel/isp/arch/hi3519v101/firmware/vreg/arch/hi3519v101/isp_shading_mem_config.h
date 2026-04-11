/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_shading_mem_config.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2015/05/15
  Description   : 
  History       :
  1.Date        : 2015/05/15
    Author      : 
    Modification: Created file

******************************************************************************/

#ifndef __ISP_SHADING_MEM_CONFIG_H__
#define __ISP_SHADING_MEM_CONFIG_H__

#include "hi_vreg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */
// Instance 'shading_mem' of module 'shading_mem'
// ------------------------------------------------------------------------------ //

#define HI_RADIAL_SHADING_MEM_BASE_ADDR (0x6000)
#define HI_RADIAL_SHADING_MEM_SIZE (0x1000)
#define HI_MESH_SHADING_MEM_BASE_ADDR (0x1C000)
#define HI_MESH_SHADING_MEM_SIZE (0x3000)


#define HI_SHADING_MEM_ARRAY_DATA_DEFAULT (0x0)
#define HI_SHADING_MEM_ARRAY_DATA_DATASIZE (32)

// args: index (0-3071), data (32-bit)
static __inline HI_VOID hi_shading_mem_array_data_write(ISP_DEV IspDev, HI_U32 index, HI_U32 data) {
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x6000 + (index << 2), data);
}
static __inline HI_U32 hi_shading_mem_array_data_read(ISP_DEV IspDev, HI_U32 index) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x6000 + (index << 2));
}
// ------------------------------------------------------------------------------ //

#define HI_RADIAL_SHADING_MEM_LUTS_SHADING_LUTR_DEFAULT (4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096)
#define HI_RADIAL_SHADING_MEM_LUTS_SHADING_LUTR_DATASIZE (16)
// args: index (0-128), data (16-bit)
static __inline void hi_radial_shading_mem_luts_shading_lutr_write(ISP_DEV IspDev, HI_U32 index, HI_U16 data) { 
    HI_U32 addr = HI_RADIAL_SHADING_MEM_BASE_ADDR + (index << 2);    
    IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr, data & 0xFFFF);
}

static __inline HI_U16 hi_radial_shading_mem_luts_shading_lutr_read(ISP_DEV IspDev, HI_U32 index) { 
    HI_U32 addr = HI_RADIAL_SHADING_MEM_BASE_ADDR + (index << 2);    
    return (HI_U16) IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr);
}

// ------------------------------------------------------------------------------ //
// Register: hi_radial_shading_mem_luts_shading_lutg
// ------------------------------------------------------------------------------ //
// Lookup table for the green channel
// ------------------------------------------------------------------------------ //

#define HI_RADIAL_SHADING_MEM_LUTS_SHADING_LUTG_DEFAULT (4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096)
#define HI_RADIAL_SHADING_MEM_LUTS_SHADING_LUTG_DATASIZE (16)

// args: index (0-128), data (16-bit)
static __inline void hi_radial_shading_mem_luts_shading_lutg_write(ISP_DEV IspDev, HI_U32 index, HI_U16 data) {
    HI_U32 addr = HI_RADIAL_SHADING_MEM_BASE_ADDR + 0x400 + (index << 2);    
    IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr, data & 0xFFFF);
}

static __inline HI_U16 hi_radial_shading_mem_luts_shading_lutg_read(ISP_DEV IspDev, HI_U32 index) {
    HI_U32 addr = HI_RADIAL_SHADING_MEM_BASE_ADDR + 0x400 + (index << 2);   
    return (HI_U16) IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr);
}

// ------------------------------------------------------------------------------ //
// Register: hi_radial_shading_mem_luts_shading_lutb
// ------------------------------------------------------------------------------ //
// Lookup table for the blue channel
// ------------------------------------------------------------------------------ //
#define HI_RADIAL_SHADING_MEM_LUTS_SHADING_LUTB_DEFAULT (4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096)
#define HI_RADIAL_SHADING_MEM_LUTS_SHADING_LUTB_DATASIZE (16)

// args: index (0-128), data (16-bit)
static __inline void hi_radial_shading_mem_luts_shading_lutb_write(ISP_DEV IspDev, HI_U32 index, HI_U16 data) {
    HI_U32 addr = HI_RADIAL_SHADING_MEM_BASE_ADDR + 0x800 + (index << 2);    
    IOWR_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr, data & 0xFFFF);
}

static __inline HI_U16 hi_radial_shading_mem_luts_shading_lutb_read(ISP_DEV IspDev, HI_U32 index) { 
    HI_U32 addr = HI_RADIAL_SHADING_MEM_BASE_ADDR + 0x800 + (index << 2); 
    return (HI_U16) IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr);
}

// ------------------------------------------------------------------------------ //
// Register: hi_mesh_shading_mem_luts_shading_lutr
// ------------------------------------------------------------------------------ //
// Lookup table for the red channel
// ------------------------------------------------------------------------------ //

#define HI_MESH_SHADING_MEM_LUTS_SHADING_LUTR_DEFAULT (4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096)
#define HI_MESH_SHADING_MEM_LUTS_SHADING_LUTR_DATASIZE (32)
// args: index (0-128), data (16-bit)
static __inline void hi_mesh_shading_mem_luts_shading_lutr_write(ISP_DEV IspDev, HI_U32 index, HI_U32 data) { 
    HI_U32 addr = HI_MESH_SHADING_MEM_BASE_ADDR + (index << 2);    
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr, data & 0xFFFFFFFF);
}

static __inline HI_U32 hi_mesh_shading_mem_luts_shading_lutr_read(ISP_DEV IspDev, HI_U32 index) { 
    HI_U32 addr = HI_MESH_SHADING_MEM_BASE_ADDR + (index << 2);    
    return (HI_U32) IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr);
}

// ------------------------------------------------------------------------------ //
// Register: hi_mesh_shading_mem_luts_shading_lutg
// ------------------------------------------------------------------------------ //
// Lookup table for the green channel
// ------------------------------------------------------------------------------ //

#define HI_MESH_SHADING_MEM_LUTS_SHADING_LUTG_DEFAULT (4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096)
#define HI_MESH_SHADING_MEM_LUTS_SHADING_LUTG_DATASIZE (32)

// args: index (0-128), data (16-bit)
static __inline void hi_mesh_shading_mem_luts_shading_lutg_write(ISP_DEV IspDev, HI_U32 index, HI_U32 data) {
    HI_U32 addr = HI_MESH_SHADING_MEM_BASE_ADDR + 0x1000 + (index << 2);    
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr, data & 0xFFFFFFFF);
}

static __inline HI_U32 hi_mesh_shading_mem_luts_shading_lutg_read(ISP_DEV IspDev, HI_U32 index) {
    HI_U32 addr = HI_MESH_SHADING_MEM_BASE_ADDR + 0x1000 + (index << 2);   
    return (HI_U32) IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr);
}

// ------------------------------------------------------------------------------ //
// Register: hi_mesh_shading_mem_luts_shading_lutb
// ------------------------------------------------------------------------------ //
// Lookup table for the blue channel
// ------------------------------------------------------------------------------ //
#define HI_MESH_SHADING_MEM_LUTS_SHADING_LUTB_DEFAULT (4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096)
#define HI_MESH_SHADING_MEM_LUTS_SHADING_LUTB_DATASIZE (32)

// args: index (0-128), data (16-bit)
static __inline void hi_mesh_shading_mem_luts_shading_lutb_write(ISP_DEV IspDev, HI_U32 index, HI_U32 data) {
    HI_U32 addr = HI_MESH_SHADING_MEM_BASE_ADDR + 0x2000 + (index << 2);    
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr, data & 0xFFFFFFFF);
}

static __inline HI_U32 hi_mesh_shading_mem_luts_shading_lutb_read(ISP_DEV IspDev, HI_U32 index) { 
    HI_U32 addr = HI_MESH_SHADING_MEM_BASE_ADDR + 0x2000 + (index << 2); 
    return (HI_U32) IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __ISP_SHADING_MEM_CONFIG_H__ */
