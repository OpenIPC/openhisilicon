/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_drv_defines.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __ISP_DRV_DEFINES_H__
#define __ISP_DRV_DEFINES_H__

#include "isp.h"
#include "mkp_isp.h"
#include "isp_stt_define.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/* 0x00F8[16]: int_isp0_en; 0x00F8[0]: int_pt0_en; 0x00F8[8]: int_ch0_en; */
#define VICAP_HD_MASK                   0x00F8
#define VICAP_HD_INTTERRUPT             0x00F0
#define vicap_int_mask_pt(vi_dev)        (1 << (vi_dev))
#define vicap_int_mask_isp(vi_pipe)      (0x10000 << (vi_pipe))

#define VI_WCH_MASK             0x00F8
#define VI_WCH_INT              0xF0
#define VI_WCH_INT_FSTART       (1<<0)

#define vi_pt_base(vi_dev)      (0x1000 + (vi_dev) * 0x100)
#define VI_PT_INT_MASK          0xF8
#define VI_PT_INT               0xF0
#define VI_PT_INT_FSTART        (1<<0)
#define VI_PT_INT_ERR           ((1<<1) | (2<<1))
#define VI_PT_FSTART_DLY        0x60

#define VI_ISP_MODE             0x58
#define VI_ISP0_P2_EN_BIT       8

#define ISP_INT_FE_MASK         0x00F8
#define ISP_INT_FE              0x00F0
#define ISP_INT_FE_FSTART       (1<<0)
#define ISP_INT_FE_FEND         (1<<4)

#define VIPROC_INT_BE_MASK      0x00F0
#define VIPROC_INT_BE           0x0310
#define VIPROC_INT_BE_FSTART    (1<<16)

#define VI_PT0_ID               0xE0
#define VI_PT0_ID_CFG           0x10A4

#define CH_REG_NEWER            0x1004
#define ISP_FE_REG_NEWER        0x01EC
#define ISP_BE_REG_NEWER        0x01EC

/* 0x01A4: [31]:enable; [30]:mode; [29]:reset; [5:4]:vc_num; [3:2]:vc_num_max; [1:0]:vc_num_init_value; */
#define VC_NUM_ADDR             0x01A4

#define ISP_RESET               0x0078
#define CHN_SWITCH              0x10028

#define ISP_MAGIC_OFFSET        1
#define PROC_PRT_SLICE_SIZE     256

#define BUILT_IN_WDR_RATIO_VS_S 0x400
#define BUILT_IN_WDR_RATIO_S_M  0x400
#define BUILT_IN_WDR_RATIO_M_L  0x40

#define io_pt_address(x)                    ((hi_uintptr_t)isp_drv_get_reg_vicap_base_va() + (x))
#define io_wch_address(vi_pipe, x)          ((hi_uintptr_t)isp_drv_get_vicap_ch_base_va(vi_pipe) + (x))
#define io_fe_address(vi_pipe, x)           ((hi_uintptr_t)isp_drv_get_ispfe_base_va(vi_pipe) + (x))
#define io_be_address(blk_dev, x)           ((hi_uintptr_t)isp_drv_get_ispbe_base_va(blk_dev) + (x))

#define io_rw_pt_address(x)                 (*((volatile unsigned int *)io_pt_address(x)))
#define io_rw_fe_address(vi_pipe, x)        (*((volatile unsigned long *)io_fe_address(vi_pipe, x)))
#define io_rw_ch_address(vi_pipe, x)        (*((volatile unsigned long *)io_wch_address(vi_pipe, x)))
#define io_rw_be_address(blk_dev, x)        (*((volatile unsigned long *)io_be_address(blk_dev, x)))

#define isp_drv_fereg_ctx(vi_pipe, ctx)     ctx = (S_ISPFE_REGS_TYPE *)(isp_drv_get_ispfe_base_va(vi_pipe))
#define isp_drv_festtreg_ctx(vi_pipe, ctx)  ctx = (S_ISP_VICAP_CH_REGS_TYPE *)(isp_drv_get_vicap_ch_base_va(vi_pipe))
#define isp_drv_bereg_ctx(blk_dev, ctx)     ctx = (S_ISPBE_REGS_TYPE *)(isp_drv_get_ispbe_base_va(blk_dev))
#define isp_drv_viprocreg_ctx(blk_dev, ctx) ctx = (S_VIPROC_REGS_TYPE *)(isp_drv_get_viproc_base_va(blk_dev))

hi_s32  isp_drv_be_remap(void);
hi_void isp_drv_be_unmap(void);

hi_s32  isp_drv_vicap_remap(void);
hi_void isp_drv_vicap_unmap(void);

hi_s32  isp_drv_fe_remap(void);
hi_void isp_drv_fe_unmap(void);

hi_s32 isp_check_mod_param(hi_vi_pipe vi_pipe);

hi_u8 isp_drv_get_block_num(hi_vi_pipe vi_pipe);
hi_s8 isp_drv_get_block_id(hi_vi_pipe vi_pipe, isp_running_mode running_mode);

hi_s32 isp_drv_get_be_regs_attr(hi_vi_pipe vi_pipe, S_ISPBE_REGS_TYPE *be_reg[], isp_be_regs_attr *blk_attr);
hi_s32  isp_drv_fe_stitch_statistics_read(hi_vi_pipe vi_pipe, isp_stat_info *stat_info);

hi_s32 isp_drv_be_apb_statistics_read(hi_vi_pipe vi_pipe, isp_stat *stat, S_ISPBE_REGS_TYPE *be_reg[],
                                      isp_be_regs_attr be_regs_attr, isp_stat_key un_statkey);

hi_s32 isp_drv_be_stt_statistics_read(hi_vi_pipe vi_pipe, isp_stat *stat, S_ISPBE_REGS_TYPE *be_reg[],
                                      isp_be_regs_attr be_regs_attr, isp_stat_key un_statkey);

hi_s32 isp_drv_be_offline_stitch_statistics_read(hi_vi_pipe vi_pipe, isp_stat_info *stat_info);
hi_s32 isp_drv_be_stitch_statistics_read(hi_vi_pipe vi_pipe, isp_stat_info *stat_info);
hi_s32 isp_drv_be_ae_global_offline_statistics_read(isp_stat *stat, S_ISP_STT_REGS_TYPE *be_stt[], hi_u8 blk_num);
hi_s32 isp_drv_be_ae_local_offline_statistics_read(isp_stat *stat, S_ISP_STT_REGS_TYPE *be_stt[], hi_u8 blk_num);
hi_s32 isp_drv_be_mg_offline_statistics_read(isp_stat *stat, S_ISP_STT_REGS_TYPE *be_stt[], hi_u8 blk_num);
hi_s32 isp_drv_be_awb_offline_statistics_read(hi_vi_pipe vi_pipe, isp_stat *stat, S_ISP_STT_REGS_TYPE *be_stt[],
                                              hi_u8 blk_num, isp_stat_key stat_key);
hi_s32 isp_drv_be_af_offline_statistics_read(hi_vi_pipe vi_pipe, isp_stat *stat, S_ISP_STT_REGS_TYPE *be_stt[],
                                             hi_u8 blk_num);
hi_s32 isp_drv_be_dehaze_offline_statistics_read(isp_stat *stat, S_ISP_STT_REGS_TYPE *be_stt[], hi_u8 blk_num);
hi_s32 isp_drv_dpc_offline_calib_info_read(isp_stat *stat, S_ISP_STT_REGS_TYPE *be_stt[], hi_u8 blk_num,
                                           hi_u64 bit1_dp_stat);
hi_s32 isp_drv_flicker_offline_statistics_read(isp_stat *stat, S_ISP_STT_REGS_TYPE *be_stt[], hi_u8 blk_num,
                                               hi_u64 bit1_flicker);

hi_u8 isp_drv_get_fe_sync_index(isp_drv_ctx *drv_ctx);

hi_bool isp_drv_snap_get_pictrue_pipe(isp_drv_ctx *drv_ctx);
hi_s32 isp_drv_reg_config_chn_sel(S_ISPBE_REGS_TYPE *be_reg[],
                                  isp_drv_ctx *drv_ctx, hi_u8 blk_num, hi_u8 blk_dev);
hi_s32 isp_drv_reg_config_wdr(hi_vi_pipe vi_pipe, S_ISPBE_REGS_TYPE *be_reg[],
                              isp_fswdr_sync_cfg *wdr_reg_cfg, isp_be_regs_attr *be_regs_attr, hi_u32 *ratio);
hi_s32 isp_drv_reg_config_ldci(hi_vi_pipe vi_pipe, S_ISPBE_REGS_TYPE *be_reg[], isp_drv_ctx *drv_ctx,
                               hi_u8 blk_num, hi_u8 blk_dev);
hi_s32 isp_drv_reg_config_de(hi_vi_pipe vi_pipe, S_ISPBE_REGS_TYPE *be_reg[], isp_sync_cfg_buf_node *cfg_node);
hi_s32 isp_drv_reg_config_drc(hi_vi_pipe vi_pipe, S_ISPBE_REGS_TYPE *be_reg[],
                              isp_drc_sync_cfg *drc_reg_cfg, isp_be_regs_attr *be_regs_attr, hi_u32 *ratio);
hi_s32 isp_drv_reg_config_fe_blc(hi_vi_pipe vi_pipe, isp_drv_ctx *drv_ctx);
hi_s32 isp_drv_reg_config_fe_dgain(hi_vi_pipe vi_pipe, isp_drv_ctx *drv_ctx);
hi_s32 isp_drv_reg_config_be_blc(VI_PIPE ViPipe, S_ISPBE_REGS_TYPE *be_reg[], isp_drv_ctx *drv_ctx,
                                 isp_sync_cfg_buf_node *cfg_node, isp_be_regs_attr *be_regs_attr);

hi_s32 isp_drv_reg_config_dgain(S_ISPBE_REGS_TYPE *be_reg[], hi_u32  isp_dgain, hi_u8 blk_num, hi_u8 blk_dev);
hi_s32 isp_drv_reg_config4_dgain(hi_vi_pipe vi_pipe, S_ISPBE_REGS_TYPE *be_reg[],
    isp_drv_ctx *drv_ctx, isp_sync_4dgain_cfg *sync_4dgain_cfg);

hi_void isp_drv_set_input_sel(S_ISPBE_REGS_TYPE *isp_be_regs, hi_u32 *input_sel);
hi_s32 isp_drv_config_ccm(isp_drv_ctx *drv_ctx, hi_u16 *ccm);
HI_S32 isp_drv_reg_config_awb_en(hi_vi_pipe vi_pipe, hi_bool awb_en);

hi_s32 isp_drv_fe_apb_statistics_read(isp_stat_info *stat_info, S_ISPFE_REGS_TYPE *fe_reg,
    isp_drv_ctx *drv_ctx, hi_vi_pipe vi_pipe_bind, hi_u32 k);

hi_s32 isp_drv_fe_stt_statistics_read(isp_stat_info *stat_info, S_ISPFE_REGS_TYPE *fe_reg,
    isp_drv_ctx *drv_ctx, hi_vi_pipe vi_pipe_bind, hi_u32 k);

hi_s32 isp_drv_fe_stt_addr_init(hi_vi_pipe vi_pipe);
hi_s32 isp_drv_fe_stitch_stt_addr_init(hi_vi_pipe vi_pipe);

hi_s32 isp_drv_stt_buf_init(hi_vi_pipe vi_pipe);
hi_s32 isp_drv_stt_buf_exit(hi_vi_pipe vi_pipe);

hi_s32 isp_drv_get_p2_en_info(hi_vi_pipe vi_pipe, hi_bool *p2_en);
hi_s32 isp_drv_fe_stitch_non_statistics_read(hi_vi_pipe vi_pipe, isp_stat_info *stat_info);
hi_s32 isp_drv_sta_kernel_get(hi_vi_pipe vi_pipe, isp_drv_af_statistics *focus_stat);

hi_s32 isp_drv_stitch_sync_ctrl_init(hi_vi_pipe vi_pipe);
hi_s32 isp_drv_get_stitch_be_sync_para(hi_vi_pipe vi_pipe, isp_be_sync_para *be_sync_para);
hi_s32 isp_drv_reset_fe_cfg(hi_vi_pipe vi_pipe);

hi_s32 isp_drv_ldci_offline_statistics_read(hi_vi_pipe vi_pipe, isp_stat *stat);
hi_s32 isp_drv_ldci_online_attr_update(hi_vi_pipe vi_pipe, isp_stat *stat);
hi_void isp_drv_set_ldci_write_stt_addr(S_VIPROC_REGS_TYPE *viproc_reg, hi_u64 write_addr);
hi_void isp_drv_set_ldci_read_stt_addr(S_VIPROC_REGS_TYPE *viproc_reg, hi_u64 read_addr);
hi_s32 isp_drv_get_viproc_regs_attr(hi_vi_pipe vi_pipe, S_VIPROC_REGS_TYPE *viproc_reg[], isp_be_regs_attr *blk_attr);
hi_s32 isp_drv_update_ldci_tpr_offline_stat(hi_vi_pipe vi_pipe, isp_be_wo_reg_cfg *be_cfg);
hi_s32 isp_drv_update_ldci_tpr_stt_addr(hi_vi_pipe vi_pipe, isp_be_wo_reg_cfg *be_cfg);
hi_s32 isp_drv_reg_config_be_regup(hi_vi_pipe vi_pipe);

hi_u8 isp_drv_get_be_sync_index(isp_drv_ctx *drv_ctx);

isp_drv_ctx *isp_drv_get_ctx(hi_vi_pipe vi_pipe);
osal_spinlock_t *isp_drv_get_lock(hi_vi_pipe vi_pipe);
osal_spinlock_t *isp_drv_get_sync_lock(hi_vi_pipe vi_pipe);
hi_bool isp_drv_get_ldci_tpr_flt_en(hi_vi_pipe vi_pipe);

hi_void *isp_drv_get_reg_vicap_base_va(hi_void);
hi_void *isp_drv_get_vicap_ch_base_va(hi_vi_pipe vi_pipe);
hi_void *isp_drv_get_ispfe_base_va(hi_vi_pipe vi_pipe);
hi_void *isp_drv_get_ispbe_base_va(hi_u8 blk_dev);
hi_void *isp_drv_get_viproc_base_va(hi_u8 blk_dev);

#ifdef CONFIG_HI_ISP_PQ_FOR_AI_SUPPORT
hi_s32 isp_set_pq_ai_attr(pq_ai_attr *ai_attr);
hi_s32 isp_get_pq_ai_attr(hi_vi_pipe vi_pipe, pq_ai_attr *ai_attr);
hi_s32 isp_set_pq_ai_post_nr_attr(hi_vi_pipe vi_pipe, hi_pq_ai_noiseness_post_attr *noise_attr);
hi_s32 isp_get_pq_ai_post_nr_attr(hi_vi_pipe vi_pipe, hi_pq_ai_noiseness_post_attr *noise_attr);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
