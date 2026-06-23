/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef ISP_DRV_H
#define ISP_DRV_H

#include "ot_common_isp.h"
#include "sys_ext.h"
#include "isp.h"

#define isp_bitmask(bit)  ((1 << (bit)) - 1)
#define isp_drv_bereg_ctx(blk_dev, ctx)      ctx = (isp_be_reg_type *)(isp_drv_get_isp_be_base_va(blk_dev))
#define isp_drv_viprocreg_ctx(blk_dev, ctx)  ctx = (isp_viproc_reg_type *)(isp_drv_get_viproc_base_va(blk_dev))

#define ISP_INT_BE_PRE_MASK      0x00F0
#define ISP_INT_BE_PRE           0x0310
#define ISP_INT_BE_PRE_FSTART_BIT    16
#define ISP_INT_FE_FEND         (1 << 8)

#define io_pre_proc_addr(be_id, x)            ((td_uintptr_t)isp_drv_get_viproc_base_va(be_id) + (x))
#define io_rw_pre_proc_address(be_id, offset) (*((volatile unsigned int *)io_pre_proc_addr(be_id, offset)))

td_s32 isp_drv_set_online_stt_addr(ot_vi_pipe vi_pipe, td_u64 phy_base_addr);
td_void isp_drv_set_fe_stt_addr(ot_vi_pipe vi_pipe_bind, td_u64 phy_addr);
td_void isp_drv_stitch_sync_info_ready(isp_drv_ctx *drv_ctx_s, td_s32 ready_idx, td_bool is_ready);
td_void isp_drv_stitch_sync_set_working_index(isp_drv_ctx *drv_ctx_s, td_s32 working_idx);
td_s32 isp_drv_stitch_sync_get_working_index(isp_drv_ctx *drv_ctx_s);
td_s32 isp_drv_get_pre_be_reg_virt_addr(ot_vi_pipe vi_pipe, isp_pre_be_reg_type *be_reg[]);
td_s32 isp_drv_get_post_be_reg_virt_addr(ot_vi_pipe vi_pipe, isp_post_be_reg_type *be_reg[]);
td_s32 isp_drv_get_pre_viproc_reg_virt_addr(ot_vi_pipe vi_pipe, isp_viproc_reg_type *viproc_reg[]);
td_s32 isp_drv_get_post_viproc_reg_virt_addr(ot_vi_pipe vi_pipe, isp_viproc_reg_type *viproc_reg[]);
td_void isp_drv_clear_pre_proc_fstart_int(td_void);
td_u32 isp_drv_get_pre_proc_fstart_status(td_void);
td_void isp_drv_clear_pre_proc_fstart_int_mask(td_void);
td_void isp_drv_set_pre_proc_fstart_int_mask(td_void);
td_u8 isp_drv_get_block_id(ot_vi_pipe vi_pipe, isp_running_mode running_mode);
td_void isp_drv_mem_share_init(ot_vi_pipe vi_pipe);
td_void isp_drv_enbale_vicap_int_mask(td_s32 vi_dev, ot_vi_pipe vi_pipe, td_bool int0_mask_en);
td_void isp_drv_disable_vicap_int_mask(td_s32 vi_dev, ot_vi_pipe vi_pipe);
td_void isp_drv_change_vicap_int_mask(ot_vi_pipe vi_pipe, td_bool int0_mask_en);

#endif /* ISP_DRV_H */
