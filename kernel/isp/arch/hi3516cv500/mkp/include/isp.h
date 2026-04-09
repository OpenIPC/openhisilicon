/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __ISP_H__
#define __ISP_H__

#include "hi_osal.h"
#include "mkp_isp.h"
#include "isp_ext.h"
#include "isp_vreg.h"
#include "isp_st.h"
#include "isp_block.h"
#include "vi_ext.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct  {
    isp_stat_info stat_info;
    struct osal_list_head list;
} isp_stat_node;

typedef struct  {
    hi_bool init;
    hi_u64  phy_addr;
    hi_void *vir_addr;
    hi_u64  size;
    isp_stat_node node[MAX_ISP_STAT_BUF_NUM];
    isp_stat_info *act_stat;

    hi_u32 user_num;
    hi_u32 busy_num;
    hi_u32 free_num;
    struct osal_list_head user_list;
    struct osal_list_head busy_list;
    struct osal_list_head free_list;
} isp_stat_buf;

typedef enum {
    ISP_BE_BUF_STATE_INIT = 0,
    ISP_BE_BUF_STATE_RUNNING,
    ISP_BE_BUF_STATE_FINISH,
    ISP_BE_BUF_STATE_SWITCH_START,
    ISP_BE_BUF_STATE_SWITCH,
    ISP_BE_BUF_STATE_BUTT
} isp_running_state;

typedef enum {
    ISP_BE_BUF_READY = 0,
    ISP_BE_BUF_WAITING,
    ISP_BE_BUF_EXIT,
    ISP_BE_BUF_BUTT
} isp_exit_state;

typedef enum {
    ISP_CFG_SNS_VERY_SHORT_FRAME = 0, /* main pipe */
    ISP_CFG_SNS_SHORT_FRAME,
    ISP_CFG_SNS_MIDDLE_FRAME,
    ISP_CFG_SNS_LONG_FRAME,
    ISP_CFG_SNS_BUTT
} isp_cfg_sensor_time;

/* Sync Cfg */
#define EXP_RATIO_MAX          0xFFF /* max expratio 64X */
#define EXP_RATIO_MIN          0x40  /* min expratio 1X */

#define ISP_DIGITAL_GAIN_SHIFT 8    /* ISP digital gain register shift */
#define ISP_DIGITAL_GAIN_MAX   0xFFFF  /* max ISP digital gain 16X */
#define ISP_DIGITAL_GAIN_MIN   0x100  /* min ISP digital gain 1X */

typedef struct  {
    hi_u8                   buf_wr_flag;    /* FW write node */
    hi_u8                   buf_rd_flag;    /* ISR read, then write to ISP reg or sensor */
    isp_sync_cfg_buf_node   sync_cfg_buf_node[ISP_SYNC_BUF_NODE_NUM];
} isp_sync_cfg_buf;

typedef struct  {
    hi_u8  wdr_mode;
    hi_u8  pre_wdr_mode;
    hi_u8  vc_num;          /* if 3to1, vc_num = 0,1,2,0,1,...... */
    hi_u8  vc_num_max;       /* if 3to1, vc_num_max = 2 */
    hi_u8  vc_cfg_num;
    hi_u8  cfg2_vld_dly_max;

    hi_u64 pre_sns_gain;
    hi_u64 cur_sns_gain;
    hi_u64 pre_sns_gain_sf;
    hi_u64 cur_sns_gain_sf;
    hi_u32 drc_comp[CFG2VLD_DLY_LIMIT];  /* drc exposure compensation */
    hi_u32 exp_ratio[EXP_RATIO_NUM][CFG2VLD_DLY_LIMIT];
    hi_u8  lf_mode[CFG2VLD_DLY_LIMIT];
    hi_u32 wdr_gain[WDR_CHN_MAX][CFG2VLD_DLY_LIMIT];
    hi_u8  alg_proc[CFG2VLD_DLY_LIMIT];
    isp_sync_cfg_buf_node  *node[CFG2VLD_DLY_LIMIT + 1];

    isp_sync_cfg_buf   sync_cfg_buf;
} isp_sync_cfg;

typedef struct  {
    hi_bool mode_switch;
    hi_s8   stitch_flag;
    hi_u8   read_all;
    hi_u8   first_read_all;
    hi_u8   first_order;
    hi_u8   all_init;
    hi_u32  vi_callback_count;
    hi_u64  pts;
} isp_sync_stitch_ctrl;

typedef struct {
    hi_u64 isp_last_int_time;           /* Time of last interrupt, for debug */
    hi_u64 isp_last_rate_time;          /* Time of last interrupt rate, for debug */
    hi_u32 isp_int_cnt;                /* Count of interrupt, for debug */
    hi_u32 isp_int_time;               /* Process time of interrupt, for debug */
    hi_u32 isp_int_time_max;            /* Maximal process time of interrupt, for debug */
    hi_u32 isp_int_gap_time;            /* Gap of two interrupts, for debug */
    hi_u32 isp_int_gap_time_max;         /* Maximal gap of two interrupts, for debug */
    hi_u32 isp_rate_int_cnt;            /* Count of interrupt rate, for debug */
    hi_u32 isp_rate;                  /* Interrupt Rate, interrupt count per sencond, for debug */

    hi_u64 pt_last_int_time;            /* Time of last interrupt, for debug */
    hi_u64 pt_last_rate_time;           /* Time of last interrupt rate, for debug */
    hi_u32 pt_int_cnt;                 /* Count of interrupt, for debug */
    hi_u32 pt_int_time;                /* Process time of interrupt, for debug */
    hi_u32 pt_int_time_max;             /* Maximal process time of interrupt, for debug */
    hi_u32 pt_int_gap_time;             /* Gap of two interrupts, for debug */
    hi_u32 pt_int_gap_time_max;          /* Maximal gap of two interrupts, for debug */
    hi_u32 pt_rate_int_cnt;             /* Count of interrupt rate, for debug */
    hi_u32 pt_rate;                   /* Interrupt Rate, interrupt count per sencond, for debug */

    hi_u32 sensor_cfg_time;            /* Time of sensor config, for debug */
    hi_u32 sensor_cfg_time_max;         /* Maximal time of sensor config, for debug */

    hi_u32 isp_reset_cnt;              /* Count of ISP reset when vi width or height changed */
    hi_u32 isp_be_sta_lost; /* Count of ISP BE statistics lost number When the ISP processing is not timely, for debug
                             */
} isp_drv_dbg_info;

typedef struct {
    hi_u32 port_int_status;
    hi_u32 isp_int_status;
    hi_u32 port_int_err;
    hi_u32 wch_int_status;
} isp_interrupt_sch;

typedef struct {
    isp_interrupt_sch int_sch[ISP_INTERRUPTS_SAVEINFO_MAX];
    hi_u8 read_flag;
    hi_u8 write_flag;
} isp_interrupt_info;

typedef struct {
    hi_vi_pipe vi_pipe_id;
    hi_u32 isp_int_status;
} isp_slave_pipe_int_status;

typedef struct {
    hi_bool                  edge;
    hi_bool                  vd_start;
    hi_bool                  vd_end;
    hi_bool                  yuv_mode;
    hi_bool                  vd_be_end;
    hi_bool                  mem_init;
    hi_bool                  isp_init;
    hi_bool                  pub_attr_ok;
    hi_bool                  stitch_sync;
    hi_bool                  dng_info_init;
    hi_bool                  pro_enable;
    hi_bool                  pro_start;
    hi_bool                  run_once_ok;
    hi_bool                  run_once_flag;
    hi_bool                  yuv_run_once_ok;
    hi_bool                  yuv_run_once_flag;
    hi_bool                  isp_run_flag;
    hi_char                  name[MAX_MMZ_NAME_LEN];
    hi_u8                    vc_num;
    hi_u32                   int_pos;
    hi_u32                   frame_cnt;
    hi_u32                   status;
    hi_u32                   bit16_isr_access;
    hi_u32                   pro_trig_flag;
    hi_u32                   reg_cfg_info_flag;
    hi_u8                    *save_stt_stat;

    isp_chn_switch           chn_sel_attr[ISP_STRIPING_MAX_NUM];
    isp_running_state        running_state;
    isp_exit_state           exit_state;
    isp_working_mode         work_mode;
    vi_stitch_attr           stitch_attr;
    vi_pipe_wdr_attr         wdr_attr;
    isp_wdr_cfg              wdr_cfg;
    isp_sync_cfg             sync_cfg;
    isp_stat_buf             statistics_buf;
    isp_kernel_reg_cfg       kernel_cfg[PING_PONG_NUM]; /* ping pong reg cfg info */
    isp_be_cfg_buf_info      be_buf_info;
    isp_be_buf_queue         be_buf_queue;
    isp_be_buf_node          *use_node;
    isp_be_sync_para         be_sync_para;
    isp_sync_stitch_ctrl     isp_sync_ctrl;
    isp_be_sync_para         be_pre_sync_para;
    isp_be_lut2stt_attr      be_lut2stt_attr;
    isp_fe_stt_attr          fe_stt_attr;
    isp_fe_stit_stt_attr     fe_stit_stt_attr;
    isp_be_online_stt_info   be_online_stt_buf;
    isp_be_off_stt_attr      be_off_stt_attr;
    isp_be_stit_stt_attr     be_off_stit_stt_attr;
    isp_ldci_buf             ldci_read_buf_attr;
    isp_ldci_buf             ldci_write_buf_attr;
    isp_ldci_stt_addr        ldci_stt_addr;
    isp_clut_buf             clut_buf_attr;
    isp_spec_awb_buf         spec_awb_buf_attr;
    isp_bus_callback         bus_cb;
    isp_piris_callback       piris_cb;
    isp_vibus_callback       vi_bus_cb;
    isp_drv_dbg_info         drv_dbg_info;
    hi_isp_pub_attr          proc_pub_info;
    isp_trans_info_buf       trans_info;
    isp_pro_info_buf         pro_info;
    isp_proc_mem             porc_mem;
    struct osal_semaphore    proc_sem;
    isp_interrupt_sch        int_sch;   /* isp interrupt schedule */
    isp_interrupt_info       isp_int_info;

    osal_wait_t              isp_wait;
    osal_wait_t              isp_wait_vd_start;
    osal_wait_t              isp_wait_vd_end;
    osal_wait_t              isp_wait_vd_be_end;
    osal_wait_t              isp_exit_wait;
    struct osal_semaphore    pro_info_sem;
    struct osal_semaphore    isp_sem;
    struct osal_semaphore    isp_sem_vd;
    struct osal_semaphore    isp_sem_be_vd;
    hi_isp_config_info       snap_info_save[ISP_SAVEINFO_MAX]; /* Frame end and start use index 0 and 1 respectively. */
    isp_snap_info            snap_info_load;
    isp_snap_attr            snap_attr;
    pq_ai_attr               ai_info;
    hi_dng_image_dynamic_info dng_image_dynamic_info[ISP_SAVEINFO_MAX];
    hi_frame_interrupt_attr  frame_int_attr;
    hi_isp_ir_bayer_format   isp_rgbir_format;
    hi_isp_dcf_update_info   update_info;
    hi_isp_frame_info        frame_info;
    isp_slave_pipe_int_status slave_pipe_int_status[ISP_WDR_CHN_MAX - 1];
} isp_drv_ctx;

#ifdef CONFIG_HI_ISP_LITEOS_BOTTOM_HALF_SUPPORT
typedef struct {
    hi_vi_pipe vi_pipe;
    struct osal_work_struct worker;
    struct osal_semaphore sem;
} isp_work_queue_ctx;
isp_work_queue_ctx g_isp_work_queue_ctx;
#endif

typedef enum {
    ISP_CHN_SWITCH_NORMAL = 0,
    ISP_CHN_SWITCH_2LANE  = 1,
    ISP_CHN_SWITCH_3LANE  = 2,
    ISP_CHN_SWITCH_4LANE  = 3,
} isp_chn_sel;

typedef enum {
    ISP_VC_NUM_VS = 0,
    ISP_VC_NUM_S  = 1,
    ISP_VC_NUM_M  = 2,
    ISP_VC_NUM_L  = 3,
} isp_vc_num;

#define ISP_ONLINE_LDCI_TPR_BUF_NUM       2
#define ISP_STRIPING_LDCI_NORMAL_BUF_NUM  2

#define ISP_GAIN_SHIFT                    8

/* Buffer control begin */
static hi_s32 __inline isp_sync_buf_init(isp_sync_cfg_buf *sync_cfg_buf)
{
    osal_memset(sync_cfg_buf, 0, sizeof(isp_sync_cfg_buf));
    return 0;
}

static hi_s32 __inline isp_sync_buf_is_full(isp_sync_cfg_buf *sync_cfg_buf)
{
    return ((sync_cfg_buf->buf_wr_flag + 1) % ISP_SYNC_BUF_NODE_NUM) == sync_cfg_buf->buf_rd_flag;
}

static hi_s32 __inline isp_sync_buf_is_emrty(isp_sync_cfg_buf *sync_cfg_buf)
{
    return sync_cfg_buf->buf_wr_flag == sync_cfg_buf->buf_rd_flag;
}

static hi_s32 __inline isp_interrupt_buf_is_full(isp_interrupt_info *isp_int_info)
{
    return ((isp_int_info->write_flag + 1) % ISP_INTERRUPTS_SAVEINFO_MAX) == isp_int_info->read_flag;
}

static hi_s32 __inline isp_interrupt_buf_is_empty(isp_interrupt_info *isp_int_info)
{
    return isp_int_info->write_flag == isp_int_info->read_flag;
}

static hi_s32 __inline isp_sync_buf_is_err(isp_sync_cfg_buf *sync_cfg_buf)
{
    if (isp_sync_buf_is_emrty(sync_cfg_buf)) {
        return 0;
    }

    if (((sync_cfg_buf->buf_rd_flag + 1) % ISP_SYNC_BUF_NODE_NUM) == sync_cfg_buf->buf_wr_flag) {
        return 0;
    }

    return 1;
}

static hi_s32 __inline isp_sync_buf_write(isp_sync_cfg_buf *sync_cfg_buf,
    isp_sync_cfg_buf_node *sync_cfg_buf_node)
{
    if (isp_sync_buf_is_full(sync_cfg_buf)) {
        return -1;
    }

    osal_memcpy(&sync_cfg_buf->sync_cfg_buf_node[sync_cfg_buf->buf_wr_flag], sync_cfg_buf_node,
                sizeof(isp_sync_cfg_buf_node));
    sync_cfg_buf->buf_wr_flag = (sync_cfg_buf->buf_wr_flag + 1) % ISP_SYNC_BUF_NODE_NUM;

    return 0;
}

static hi_s32 __inline isp_sync_buf_read(isp_sync_cfg_buf *sync_cfg_buf, isp_sync_cfg_buf_node **sync_cfg_buf_node)
{
    if (isp_sync_buf_is_emrty(sync_cfg_buf)) {
        return -1;
    }

    *sync_cfg_buf_node = &sync_cfg_buf->sync_cfg_buf_node[sync_cfg_buf->buf_rd_flag];
    sync_cfg_buf->buf_rd_flag = (sync_cfg_buf->buf_rd_flag + 1) % ISP_SYNC_BUF_NODE_NUM;

    return 0;
}

static hi_s32 __inline isp_sync_buf_read2(isp_sync_cfg_buf *sync_cfg_buf,
    isp_sync_cfg_buf_node **sync_cfg_buf_node)
{
    hi_u8 tmp ;
    if (isp_sync_buf_is_emrty(sync_cfg_buf)) {
        return -1;
    }

    tmp = (sync_cfg_buf->buf_wr_flag + ISP_SYNC_BUF_NODE_NUM - 1) % ISP_SYNC_BUF_NODE_NUM;

    *sync_cfg_buf_node = &sync_cfg_buf->sync_cfg_buf_node[tmp];
    sync_cfg_buf->buf_rd_flag = (tmp + 1) % ISP_SYNC_BUF_NODE_NUM;

    return 0;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __ISP_H__ */
