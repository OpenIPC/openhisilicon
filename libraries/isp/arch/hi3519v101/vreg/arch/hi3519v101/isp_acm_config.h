/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_acm_config.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/01/16
  Description   : 
  History       :
  1.Date        : 2013/01/16
    Author      : n00168968
    Modification: Created file

******************************************************************************/

#ifndef __ISP_ACM_CONFIG_H__
#define __ISP_ACM_CONFIG_H__

#include "hi_vreg.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

static __inline HI_U32 hi_vi_acm_read_enable(ISP_DEV IspDev)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x31400); 
    return (u32Cfg >>31);
}

static __inline HI_VOID hi_vi_acm_write_enable(ISP_DEV IspDev, HI_BOOL en)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x31400); 
    u32Cfg &= ~(1 << 31);
    u32Cfg |= (en << 31);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x31400, u32Cfg);
}

static __inline HI_U32 hi_vi_acm_read_dbg_enable(ISP_DEV IspDev)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x31400); 
    return (u32Cfg >>30) & 1;
}

static __inline HI_VOID hi_vi_acm_write_dbg_enable(ISP_DEV IspDev, HI_BOOL en)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x31400); 
    u32Cfg &= ~(1 << 30);
    u32Cfg |= (en << 30);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x31400, u32Cfg);
}

static __inline HI_U32 hi_vi_acm_read_stretch(ISP_DEV IspDev)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x31400); 
    return (u32Cfg >>29) & 1;
}

static __inline HI_VOID hi_vi_acm_write_stretch(ISP_DEV IspDev, HI_BOOL en)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x31400); 
    u32Cfg &= ~(1 << 29);
    u32Cfg |= (en << 29);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x31400, u32Cfg);
}

static __inline HI_U32 hi_vi_acm_read_cliprange(ISP_DEV IspDev)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x31400); 
    return (u32Cfg >>28) & 1;
}

static __inline HI_VOID hi_vi_acm_write_cliprange(ISP_DEV IspDev, HI_BOOL en)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x31400); 
    u32Cfg &= ~(1 << 28);
    u32Cfg |= (en << 28);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x31400, u32Cfg);
}

static __inline HI_U32 hi_vi_acm_read_cliporwrap(ISP_DEV IspDev)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x31400); 
    return (u32Cfg >>27) & 1;
}

static __inline HI_VOID hi_vi_acm_write_cliporwrap(ISP_DEV IspDev, HI_BOOL en)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x31400); 
    u32Cfg &= ~(1 << 27);
    u32Cfg |= (en << 27);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x31400, u32Cfg);
}

static __inline HI_U32 hi_vi_acm_read_dbg_pos(ISP_DEV IspDev)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x31400); 
    return ((u32Cfg >> 10 ) & 0x1FFF );
}

static __inline HI_VOID hi_vi_acm_write_dbg_pos(ISP_DEV IspDev, HI_U32 u32Pos)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x31400); 
    u32Cfg &= ~(0x1FFF<<10);
    u32Cfg |= ((u32Pos & 0x1FFF) << 10);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x31400, u32Cfg);
}

static __inline HI_U32 hi_vi_acm_read_cbcrth(ISP_DEV IspDev)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x31400); 
    return (u32Cfg & 0xFF);
}

static __inline HI_VOID hi_vi_acm_write_cbcrth(ISP_DEV IspDev, HI_U8 u8Thr)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x31400); 
    u32Cfg &= ~(0xFF);
    u32Cfg |= (u8Thr & 0xFF);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x31400, u32Cfg);
}

static __inline HI_U32 hi_vi_acm_read_luma_gain(ISP_DEV IspDev)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x31404); 
    return ((u32Cfg >> 20 ) & 0x3FF );
}

static __inline HI_VOID hi_vi_acm_write_luma_gain(ISP_DEV IspDev, HI_U32 u32Gain)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x31404); 
    u32Cfg &= ~(0x3FF<<20);
    u32Cfg |= ((u32Gain & 0x3FF) << 20);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x31404, u32Cfg);
}

static __inline HI_U32 hi_vi_acm_read_hue_gain(ISP_DEV IspDev)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x31404); 
    return ((u32Cfg >> 10 ) & 0x3FF );
}

static __inline HI_VOID hi_vi_acm_write_hue_gain(ISP_DEV IspDev, HI_U32 u32Gain)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x31404); 
    u32Cfg &= ~(0x3FF<<10);
    u32Cfg |= ((u32Gain & 0x3FF) << 10);
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x31404, u32Cfg);
}

static __inline HI_U32 hi_vi_acm_read_sat_gain(ISP_DEV IspDev)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x31404); 
    return (u32Cfg & 0x3FF );
}

static __inline HI_VOID hi_vi_acm_write_sat_gain(ISP_DEV IspDev, HI_U32 u32Gain)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x31404); 
    u32Cfg &= ~(0x3FF);
    u32Cfg |= (u32Gain & 0x3FF) ;
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x31404, u32Cfg);
}

static __inline HI_U32 hi_vi_acm_read_coeff_addr(ISP_DEV IspDev)
{     
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x31414); 
}

static __inline HI_VOID hi_vi_acm_write_coeff_addr(ISP_DEV IspDev, HI_U32 u32Addr)
{     
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x31414, u32Addr);
}

static __inline HI_U32 hi_vi_acm_read_para_up(ISP_DEV IspDev)
{     
    return IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x31420); 
}

static __inline HI_VOID hi_vi_acm_write_para_up(ISP_DEV IspDev, HI_BOOL bEn)
{     
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x31420, bEn);
}

static __inline HI_U32 hi_vi_acm_read_width(ISP_DEV IspDev)
{    
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x31410); 
    return (u32Cfg & 0x1FFF );
}

static __inline HI_VOID hi_vi_acm_write_width(ISP_DEV IspDev, HI_U32 w)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x31410); 
    u32Cfg &= ~(0x1FFF);
    u32Cfg |= (w & 0x1FFF) ;
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x31410, u32Cfg);
}

static __inline HI_U32 hi_vi_acm_read_height(ISP_DEV IspDev)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x31410); 
    return ((u32Cfg>>16) & 0x1FFF );
}

static __inline HI_VOID hi_vi_acm_write_height(ISP_DEV IspDev, HI_U32 h)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x31410); 
    u32Cfg &= ~(0x1FFFF<<16);
    u32Cfg |= (h & 0x1FFF) <<16;
    IOWR_32DIRECT(ISP_REG_BASE(IspDev) + VI_ISP_OFFSET + 0x31410, u32Cfg);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __ISP_ACM_CONFIG_H__ */

