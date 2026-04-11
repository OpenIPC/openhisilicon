/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_metering_mem_config.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2015/05/15
  Description   : 
  History       :
  1.Date        : 2015/05/15
    Author      : 
    Modification: Created file

******************************************************************************/
#ifndef __ISP_METERING_MEM_CONFIG_H__
#define __ISP_METERING_MEM_CONFIG_H__

#include "hi_vreg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

// ------------------------------------------------------------------------------ //
// Instance 'metering_mem' of module 'metering_mem'
// ------------------------------------------------------------------------------ //

#define HI_METERING_MEM_BASE_ADDR (0x8000)
#define HI_METERING_MEM_SIZE (0x8000)

#define HI_AE_METERING_MEM_BASE_ADDR  (0x8000)
#define HI_AE_METERING_MEM_SIZE  (0x7F8)  //0x800

#define HI_AE_SUM_METERING_MEM_BASE_ADDR  (0xA800)
#define HI_AE_SUM_METERING_MEM_SIZE  (0x7F0)  //0x800

#define HI_AWB_METERING_MEM_BASE_ADDR (0x8800)
#define HI_AWB_METERING_MEM_SIZE  (0x2000) //0x800

#define HI_AWB_SUM_METERING_MEM_BASE_ADDR (0xB000)
#define HI_AWB_SUM_METERING_MEM_SIZE  (0x3000) //0x600

#define HI_AF_METERING_MEM_BASE_ADDR  (0xd200)
#define HI_AF_METERING_MEM_SIZE  (0x7FC)  //0x800

#define HISI_AF_METERING_MEM_BASE_ADDR  (0x1228c)
#define HISI_AF_METERING_MEM_SIZE  (0xBF0)  

#define HI_METERING_MEM_ARRAY_DATA_DEFAULT (0x0)
#define HI_METERING_MEM_ARRAY_DATA_DATASIZE (32)

static __inline HI_U32 hi_metering_mem_arrary_data_read_ae(ISP_DEV IspDev, HI_U16 index){
    HI_U32 addr = HI_AE_METERING_MEM_BASE_ADDR + (index << 2);
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr);
}

//Add AWB statistic information read function
static __inline HI_U16 hi_metering_mem_array_data_read_rg(ISP_DEV IspDev, HI_U16 index) {
    HI_U32 addr = HI_AWB_METERING_MEM_BASE_ADDR + (index << 3);
    return (HI_U16) (IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr) & 0xfff);
}

static __inline HI_U16 hi_metering_mem_array_data_read_bg(ISP_DEV IspDev, HI_U16 index) {
    HI_U32 addr = HI_AWB_METERING_MEM_BASE_ADDR + (index << 3);
    return (HI_U16) ((IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr) >> 16) & 0xfff);
}

static __inline HI_U32 hi_metering_mem_array_data_read_bg_rg(ISP_DEV IspDev, HI_U16 index) {
    HI_U32 addr = HI_AWB_METERING_MEM_BASE_ADDR + (index << 3);
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr);
}

static __inline HI_U32 hi_metering_mem_array_data_read_sum(ISP_DEV IspDev, HI_U16 index) {
    HI_U32 addr = HI_AWB_METERING_MEM_BASE_ADDR + (index << 3) + 4;
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr);
}


//Each AF zone data spans 32 bits. 16 bits are allocated for contrast metrics and 16 bits for noise threshold. 
static __inline HI_U16 hi_metering_mem_array_data_read_af(ISP_DEV IspDev, HI_U16 index) {
    HI_U32 addr = HI_AF_METERING_MEM_BASE_ADDR + (index << 2);
    return (HI_U16) (IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr) & 0xffff);
}

//Hisi AF areazone data stat 32 bits.  
static __inline HI_U32 hi_area_stat_mem_array_data_read_af_v1h1(ISP_DEV IspDev, HI_U16 index) {
    HI_U32 addr = (index * 4 + 0);

    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + 0x12288, addr);
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + HISI_AF_METERING_MEM_BASE_ADDR);
}

static __inline HI_U32 hi_area_stat_mem_array_data_read_af_v2h2(ISP_DEV IspDev, HI_U16 index) {
    HI_U32 addr = (index * 4 + 1);

    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + 0x12288, addr);
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + HISI_AF_METERING_MEM_BASE_ADDR);
}

static __inline HI_U32 hi_area_stat_mem_array_data_read_af_y(ISP_DEV IspDev, HI_U16 index) {
    HI_U32 addr = (index * 4 + 2);

    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + 0x12288, addr);
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + HISI_AF_METERING_MEM_BASE_ADDR);
}

static __inline HI_U32 hi_area_block_distribute_data_read_af(ISP_DEV IspDev) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + 0x12210);
}


// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Instance 'AE_sum_mem' of module 'AE_sum_mem'
// ------------------------------------------------------------------------------ //
#define HI_AE_SUM_MEM_BASE_ADDR (0xA800)
#define HI_AE_SUM_MEM_SIZE      (0x7F0)

static __inline HI_U16 hi_ae_sum_array_data_read_r(ISP_DEV IspDev, HI_U16 index) {
    HI_U32 addr = HI_AE_SUM_MEM_BASE_ADDR + (index << 3);
    return IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr);
}

static __inline HI_U16 hi_ae_sum_array_data_read_gr(ISP_DEV IspDev, HI_U16 index) {
    HI_U32 addr = HI_AE_SUM_MEM_BASE_ADDR + (index << 3) + 2;
    return IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr);
}

static __inline HI_U16 hi_ae_sum_array_data_read_gb(ISP_DEV IspDev, HI_U16 index) {
    HI_U32 addr = HI_AE_SUM_MEM_BASE_ADDR + (index << 3) + 4;
    return IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr);
}

static __inline HI_U16 hi_ae_sum_array_data_read_b(ISP_DEV IspDev, HI_U16 index) {
    HI_U32 addr = HI_AE_SUM_MEM_BASE_ADDR + (index << 3) + 6;
    return IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr);
}

// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Instance 'AWB_sum_mem' of module 'AWB_sum_mem'
// ------------------------------------------------------------------------------ //
#define HI_AWB_SUM_MEM_BASE_ADDR (0xB000)
#define HI_AWB_SUM_MEM_SIZE      (0x3000)

static __inline HI_U16 hi_awb_sum_array_data_read_avg_r(ISP_DEV IspDev, HI_U16 index) {
    HI_U32 addr = HI_AWB_SUM_MEM_BASE_ADDR + (index * 0xC);
    return IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr);
}

static __inline HI_U16 hi_awb_sum_array_data_read_avg_g(ISP_DEV IspDev, HI_U16 index) {
    HI_U32 addr = HI_AWB_SUM_MEM_BASE_ADDR + (index  * 0xC) + 2;
    return IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr);
}

static __inline HI_U16 hi_awb_sum_array_data_read_avg_b(ISP_DEV IspDev, HI_U16 index) {
    HI_U32 addr = HI_AWB_SUM_MEM_BASE_ADDR + (index * 0xC) + 4;
    return IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr);
}

static __inline HI_U16 hi_awb_sum_array_data_read_avg_y(ISP_DEV IspDev, HI_U16 index) {
    HI_U32 addr = HI_AWB_SUM_MEM_BASE_ADDR + (index  * 0xC) + 6;
    return IORD_16DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr);
}

static __inline HI_U32 hi_awb_sum_array_data_read_avg_g_r(ISP_DEV IspDev, HI_U16 index) {
    HI_U32 addr = HI_AWB_SUM_MEM_BASE_ADDR + (index * 0xC);
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr);
}

static __inline HI_U32 hi_awb_sum_array_data_read_all_avgb(ISP_DEV IspDev, HI_U16 index) {
    HI_U32 addr = HI_AWB_SUM_MEM_BASE_ADDR + (index * 0xC) + 4;
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr);
}

static __inline HI_U32 hi_awb_sum_array_data_read_min_max(ISP_DEV IspDev, HI_U16 index) {
    HI_U32 addr = HI_AWB_SUM_MEM_BASE_ADDR + (index * 0xC) + 8;
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + addr);
}

// args: index (0-2303), data (32-bit)
static __inline HI_VOID hi_metering_mem_array_data_write(ISP_DEV IspDev, HI_U32 index, HI_U32 data) {
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xc000 + (index << 2), data);
}
static __inline HI_U32 hi_metering_mem_array_data_read(ISP_DEV IspDev, HI_U32 index) {
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0xc000 + (index << 2));
}
// ------------------------------------------------------------------------------ //
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __ISP_METERING_MEM_CONFIG_H__ */
