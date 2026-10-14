/*
 * Copyright (c) XMEDIA. All rights reserved.
 */
#ifndef __XMEDIA_VENC_H__
#define __XMEDIA_VENC_H__

#ifndef __KERNEL__
#include <sys/time.h>
#endif
#include "common.h"
//#include "osal.h"
#include "xmedia_video_common.h"
#include "comm_vb.h"
#include "xmedia_vb.h"
#include "math_fun.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 由于原本定义在common_video.h里的数据结构暂时没有的，先定义到此处 */
#if 1

#define CONFIG_CHNL_SUPPORT 1
#define CONFIG_VENC_SUPPORT 1
#define CONFIG_H265E_SUPPORT 1
#define CONFIG_H265E_USERDATA_SUPPORT 1
#define CONFIG_H265E_INTRA_REFRESH_SUPPORT 1
#define CONFIG_H264E_SUPPORT 1
#define CONFIG_H264E_SVC_SUPPORT 1
#define CONFIG_H264E_USERDATA_SUPPORT 1
#define CONFIG_H264E_INTRA_REFRESH_SUPPORT 1
#define CONFIG_JPEGE_SUPPORT 1
#define CONFIG_MJPEGE_SUPPORT 1
#define CONFIG_JPEGE_DCF_SUPPORT 1
#define CONFIG_JPEGE_USERDATA_SUPPORT 1
#define CONFIG_VENC_LOWDELAY_SUPPORT 1
#define CONFIG_VENC_FRAMEBUF_RECYCLE_SUPPORT 1
#define CONFIG_VENC_MPF_VGS_SUPPORT 1
#define CONFIG_VENC_VGS_SUPPORT 1
#define CONFIG_VENC_SMARTP_SUPPORT 1
#define CONFIG_VENC_DUALP_SUPPORT 1
#define CONFIG_VENC_RCNREF_SHARE_SUPPORT 1
#define CONFIG_VENC_DEBREATH_SUPPORT 1
#define CONFIG_VENC_SKIPREF_SUPPORT 1
#define CONFIG_VENC_SCENE0_SUPPORT 1
#define CONFIG_VENC_SCENE1_SUPPORT 1
#define CONFIG_VENC_SCENE2_SUPPORT 1
#define CONFIG_RC_AVBR_SUPPORT 1
#define CONFIG_RC_QPMAP_SUPPORT 1
#define CONFIG_RC_QVBR_SUPPORT 1
#define CONFIG_RC_CVBR_SUPPORT 1

typedef enum {
    VENC_RC_MODE_H264CBR = 1,
    VENC_RC_MODE_H264VBR,
    VENC_RC_MODE_H264AVBR,
    VENC_RC_MODE_H264FIXQP,
    VENC_RC_MODE_H264QPMAP,

    VENC_RC_MODE_MJPEGCBR,
    VENC_RC_MODE_MJPEGVBR,
    VENC_RC_MODE_MJPEGFIXQP,

    VENC_RC_MODE_H265CBR,
    VENC_RC_MODE_H265VBR,
    VENC_RC_MODE_H265AVBR,
    VENC_RC_MODE_H265FIXQP,
    VENC_RC_MODE_H265QPMAP,

    VENC_RC_MODE_MAX,
} xmedia_venc_rc_mode;

typedef enum {
    VENC_GOPMODE_NORMALP = 0,
    VENC_GOPMODE_SMARTP,
    VENC_GOPMODE_ADVSMARTP,

    VENC_GOPMODE_MAX,
} xmedia_venc_gop_mode;

typedef enum {
    VENC_RC_PRIORITY_BITRATE_FIRST = 1,
    VENC_RC_PRIORITY_FRAMEBITS_FIRST,
    VENC_RC_PRIORITY_MAX,
} xmedia_venc_rc_priority;

typedef enum {
    H264E_NALU_BSLICE = 0,
    H264E_NALU_PSLICE = 1,
    H264E_NALU_ISLICE = 2,
    H264E_NALU_IDRSLICE = 5,
    H264E_NALU_SEI    = 6,
    H264E_NALU_SPS    = 7,
    H264E_NALU_PPS    = 8,
    H264E_NALU_MAX
} xmedia_h264_nalu_type;

typedef enum {
    H265E_NALU_BSLICE = 0,
    H265E_NALU_PSLICE = 1,
    H265E_NALU_ISLICE = 2,
    H265E_NALU_IDRSLICE = 19,
    H265E_NALU_VPS    = 32,
    H265E_NALU_SPS    = 33,
    H265E_NALU_PPS    = 34,
    H265E_NALU_SEI    = 39,
    H265E_NALU_MAX
} xmedia_h265_nalu_type;

typedef enum {
    H264E_REFSLICE_FOR_1X = 1,
    H264E_REFSLICE_FOR_2X = 2,
    H264E_REFSLICE_FOR_4X = 5,
    H264E_REFSLICE_FOR_MAX
} xmedia_h264e_refslice_type;

typedef enum {
    JPEGE_PACK_HEAD = 0,
    JPEGE_PACK_ECS,
    JPEGE_PACK_APP,
    JPEGE_PACK_PIC,
    JPEGE_PACK_DCF,
    JPEGE_PACK_DCF_PIC,
    JPEGE_PACK_LAST_ECS,
    JPEGE_PACK_MAX
} xmedia_jpege_pack_type;

typedef enum {
    BASE_IDRSLICE = 0,
    BASE_PSLICE_REFTOIDR,
    BASE_PSLICE_REFBYBASE,
    BASE_PSLICE_REFBYENHANCE,
    ENHANCE_PSLICE_REFBYENHANCE,
    ENHANCE_PSLICE_NOTFORREF,
    ENHANCE_PSLICE_MAX
} xmedia_h264e_ref_type;

typedef enum {
    VENC_PIC_RECEIVE_SINGLE = 0,
    VENC_PIC_RECEIVE_MULTI,

    VENC_PIC_RECEIVE_MAX
} xmedia_venc_pic_recive_mode;

typedef enum {
    SUPERFRM_NONE=0,
    SUPERFRM_DISCARD,
    SUPERFRM_REENCODE,
    SUPERFRM_MAX
} xmedia_venc_superfrm_mode;

typedef enum {
    FRMLOST_NORMAL=0,
    FRMLOST_PSKIP,
    FRAMELOST_MAX
} xmedia_framelost_mode;

typedef enum {
    INTRA_REFRESH_ROW = 0,
    INTRA_REFRESH_COLUMN,
    INTRA_REFRESH_MAX
} xmedia_venc_intra_refresh_mode;

typedef enum {
    MODTYPE_VENC = 1,
    MODTYPE_H264E,
    MODTYPE_H265E,
    MODTYPE_JPEGE,
    MODTYPE_RC,
    MODTYPE_MAX
} xmedia_venc_mod_type;

typedef enum {
    VENC_FRAME_TYPE_NONE = 1,
    VENC_FRAME_TYPE_IDR,
    VENC_FRAME_TYPE_MAX
} xmedia_venc_frame_type;

typedef enum {
    SCENE_0  = 0,
    SCENE_1  = 1,
    SCENE_2  = 2,
    SCENE_MAX
} xmedia_venc_scene_mod;

typedef enum {
    FG_REGION_PEOPLE = 0,
    FG_REGION_FACE,
    FG_REGION_MAX
} xmedia_fg_region_type;

typedef enum {
    SVC_RECT_TYPE_FACE,
    SVC_RECT_TYPE_PEOPLE,
    SVC_RECT_TYPE_MAX
} xmedia_venc_svc_rect_type;

typedef enum {
    VENC_PME_SEARCH_WINDOW_64 = 0x0,  /* horizontal and vertical search range+-64 */
    VENC_PME_SEARCH_WINDOW_128,  /* horizontal and vertical search range+-128 */
    VENC_PME_SEARCH_WINDOW_128_96,  /* horizontal search range+-128, vertical search range+-96 */
    VENC_PME_SEARCH_WINDOW_MAX
} xmedia_venc_pme_search_window;

typedef enum {
    VENC_IME_SEARCH_WINDOW_W16_H12 = 0x0,  /* w[-16,16),h[-12,12) */
    VENC_IME_SEARCH_WINDOW_W12_H8,  /* w[-12,12),h[-8,8) */
    VENC_IME_SEARCH_WINDOW_W8_H4,  /* w[-8,8),h[-4,4) */
    VENC_IME_SEARCH_WINDOW_W8_H8,  /* w[-8,8),h[-8,8) */
    VENC_IME_SEARCH_WINDOW_MAX
} xmedia_venc_ime_search_window;


typedef struct {
    xmedia_u32      gop;
    xmedia_u32      src_frame_rate;
    xmedia_u32      dst_frame_rate ;
    xmedia_u32      i_qp;
    xmedia_u32      p_qp;
    xmedia_u32      b_qp;
} xmedia_venc_h264_fixqp;

typedef struct {
    xmedia_u32      gop;
    xmedia_u32      stat_time;
    xmedia_u32      src_frame_rate;
    xmedia_u32      dst_frame_rate;
    xmedia_u32      bit_rate;
} xmedia_venc_h264_cbr;

typedef struct {
    xmedia_u32      gop;
    xmedia_u32      stat_time;
    xmedia_u32      src_frame_rate;
    xmedia_u32      dst_frame_rate;
    xmedia_u32      max_bit_rate;
} xmedia_venc_h264_vbr;

typedef struct {
    xmedia_u32      gop;
    xmedia_u32      stat_time;
    xmedia_u32      src_frame_rate;
    xmedia_u32      dst_frame_rate ;
    xmedia_u32      max_bit_rate;
} xmedia_venc_h264_avbr;

typedef struct {
    xmedia_u32      gop;
    xmedia_u32      stat_time;
    xmedia_u32      src_frame_rate;
    xmedia_u32      dst_frame_rate;
} xmedia_venc_h264_qpmap;

typedef struct {
    xmedia_u32      gop;
    xmedia_u32      stat_time;
    xmedia_u32      src_frame_rate;
    xmedia_u32      dst_frame_rate;
} xmedia_venc_h265_qpmap;

typedef xmedia_venc_h264_cbr   xmedia_venc_h265_cbr;
typedef xmedia_venc_h264_vbr   xmedia_venc_h265_vbr;
typedef xmedia_venc_h264_avbr  xmedia_venc_h265_avbr;
typedef xmedia_venc_h264_fixqp xmedia_venc_h265_fixqp;

typedef struct {
    xmedia_u32      src_frame_rate;
    xmedia_u32      dst_frame_rate;
    xmedia_u32      q_factor;
} xmedia_venc_mjpeg_fixqp;

typedef struct {
    xmedia_u32      stat_time;
    xmedia_u32      src_frame_rate;
    xmedia_u32      dst_frame_rate;
    xmedia_u32      bit_rate;
} xmedia_venc_mjpeg_cbr;

typedef struct {
    xmedia_u32      stat_time;
    xmedia_u32      src_frame_rate;
    xmedia_u32      dst_frame_rate;
    xmedia_u32      max_bit_rate;
}xmedia_venc_mjpeg_vbr;

typedef struct {
    xmedia_venc_rc_mode rc_mode;
    union {
        xmedia_venc_h264_cbr    h264_cbr;
        xmedia_venc_h264_vbr    h264_vbr;
        xmedia_venc_h264_avbr   h264_avbr;
        xmedia_venc_h264_fixqp  h264_fixqp;
        xmedia_venc_h264_qpmap  h264_qpmap;

        xmedia_venc_mjpeg_cbr   mjpeg_cbr;
        xmedia_venc_mjpeg_vbr   mjpeg_vbr;
        xmedia_venc_mjpeg_fixqp mjpeg_fixqp;

        xmedia_venc_h265_cbr    h265_cbr;
        xmedia_venc_h265_vbr    h265_vbr;
        xmedia_venc_h265_avbr   h265_avbr;
        xmedia_venc_h265_fixqp  h265_fixqp;
        xmedia_venc_h265_qpmap  h265_qpmap;
    };
} xmedia_venc_rc_attr;

typedef struct {
    xmedia_u32  max_i_prop;
    xmedia_u32  min_i_prop;
    xmedia_u32  i_bits_factor;
    xmedia_u32  max_qp;
    xmedia_u32  min_qp;
    xmedia_u32  max_i_qp;
    xmedia_u32  min_i_qp;
    xmedia_s32  max_re_enc_times;
    xmedia_bool qp_map_en;
} xmedia_venc_param_h264_cbr;

typedef struct {
    xmedia_s32  change_pos;
    xmedia_u32  max_i_prop;
    xmedia_u32  min_i_prop;
    xmedia_u32  i_bits_factor;
    xmedia_s32  max_re_enc_times;
    xmedia_bool qp_map_en;

    xmedia_u32  max_qp;
    xmedia_u32  min_qp;
    xmedia_u32  max_i_qp;
    xmedia_u32  min_i_qp;
} xmedia_venc_param_h264_vbr;

typedef struct {
    xmedia_s32  change_pos;
    xmedia_u32  max_i_prop;
    xmedia_u32  min_i_prop;
    xmedia_u32  i_bits_factor;
    xmedia_s32  max_re_enc_times;
    xmedia_bool qp_map_en;

    xmedia_s32  min_still_percent;
    xmedia_u32  max_still_qp;
    xmedia_u32  min_still_psnr;

    xmedia_u32  max_qp;
    xmedia_u32  min_qp;
    xmedia_u32  max_i_qp;
    xmedia_u32  min_i_qp;
    xmedia_u32  min_qp_delta;
    xmedia_u32  motion_sensitivity;
} xmedia_venc_param_h264_avbr;

typedef struct {
    xmedia_u32  max_q_factor;
    xmedia_u32  min_q_factor;
} xmedia_venc_param_mjpeg_cbr;

typedef struct {
    xmedia_s32 change_pos;
    xmedia_u32 max_q_factor;
    xmedia_u32 min_q_factor;
} xmedia_venc_param_mjpeg_vbr;

typedef struct {
    xmedia_u32  max_i_prop;
    xmedia_u32  min_i_prop;
    xmedia_u32  i_bits_factor;
    xmedia_u32  max_qp;
    xmedia_u32  min_qp;
    xmedia_u32  max_i_qp;
    xmedia_u32  min_i_qp;
    xmedia_s32  max_re_enc_times;
    xmedia_bool qp_map_en;
} xmedia_venc_param_h265_cbr;

typedef struct {
    xmedia_s32  change_pos;
    xmedia_u32  max_i_prop;
    xmedia_u32  min_i_prop;
    xmedia_u32  i_bits_factor;
    xmedia_s32  max_re_enc_times;

    xmedia_u32  max_qp;
    xmedia_u32  min_qp;
    xmedia_u32  max_i_qp;
    xmedia_u32  min_i_qp;

    xmedia_bool qp_map_en;
} xmedia_venc_param_h265_vbr;

typedef struct {
    xmedia_s32  change_pos;
    xmedia_u32  max_i_prop;
    xmedia_u32  min_i_prop;
    xmedia_u32  i_bits_factor;
    xmedia_s32  max_re_enc_times;

    xmedia_s32  min_still_percent;
    xmedia_u32  max_still_qp;
    xmedia_u32  min_still_psnr;

    xmedia_u32  max_qp;
    xmedia_u32  min_qp;
    xmedia_u32  max_i_qp;
    xmedia_u32  min_i_qp;

    xmedia_u32  min_qp_delta;
    xmedia_u32  motion_sensitivity;

    xmedia_bool qp_map_en;
} xmedia_venc_param_h265_avbr;

typedef struct {
    xmedia_bool detect_scene_change;
    xmedia_bool adaptive_insert_idr_frame;
} xmedia_venc_scene_change_detect;

typedef struct {
    xmedia_u32 adjacent_qp_delta;
    xmedia_u32 cu_qp_delta;
    xmedia_u32 row_qp_delta;
    xmedia_s32 first_frame_start_qp;
    xmedia_venc_scene_change_detect scene_change_detect;
    union {
        xmedia_venc_param_h264_cbr     param_h264_cbr;
        xmedia_venc_param_h264_vbr     param_h264_vbr;
        xmedia_venc_param_h264_avbr    param_h264_avbr;
        xmedia_venc_param_h265_cbr     param_h265_cbr;
        xmedia_venc_param_h265_vbr     param_h265_vbr;
        xmedia_venc_param_h265_avbr    param_h265_avbr;
        xmedia_venc_param_mjpeg_cbr    param_mjpeg_cbr;
        xmedia_venc_param_mjpeg_vbr    param_mjpeg_vbr;
    };
} xmedia_venc_rc_param;

typedef struct {
    xmedia_bool                frm_lost_open;
    xmedia_u32                 frm_lost_bps_thr;
    xmedia_framelost_mode      frm_lost_mode;
    xmedia_u32                 enc_frm_gaps;
} xmedia_venc_frame_lost;

typedef struct {
    xmedia_venc_superfrm_mode    super_frm_mode;
    xmedia_u32                super_i_frm_bits_thr;
    xmedia_u32                super_p_frm_bits_thr;
    xmedia_venc_rc_priority      rc_priority;
} xmedia_venc_super_frame_cfg;
#endif   /**********************************  end of #if 1  ****************************/

typedef union {
    xmedia_h264_nalu_type    h264e_type;
    xmedia_jpege_pack_type   jpege_type;
    xmedia_h265_nalu_type    h265e_type;
} xmedia_venc_data_type;

typedef struct {
    xmedia_venc_data_type  pack_type;
    xmedia_u32 pack_offset;
    xmedia_u32 pack_length;
} xmedia_venc_pack_info;

typedef struct {
    xmedia_u64 phy_addr;
    xmedia_u8 ATTRIBUTE* vir_addr;
    xmedia_u32 ATTRIBUTE len;

    xmedia_u64 pts;
    xmedia_bool is_frame_end;

    xmedia_venc_data_type data_type;
    xmedia_u32 offset;
    xmedia_u32 data_num;
    xmedia_venc_pack_info pack_info[8];
} xmedia_venc_pack;

typedef xmedia_h264e_ref_type xmedia_h265e_ref_type;

typedef struct {
    xmedia_u32                 pic_bytes_num;
    xmedia_h264e_ref_type      ref_type;
    xmedia_u32                 update_attr_cnt;
    xmedia_u32                 start_qp;
    xmedia_u32                 mean_qp;
    xmedia_bool                p_skip;
} xmedia_venc_stream_info_h264;

typedef struct {
    xmedia_u32                 pic_bytes_num;
    xmedia_h265e_ref_type      ref_type;
    xmedia_u32                 update_attr_cnt;
    xmedia_u32                 start_qp;
    xmedia_u32                 mean_qp;
    xmedia_bool                p_skip;
} xmedia_venc_stream_info_h265;

typedef struct {
    xmedia_bool sse_enable;
    xmedia_u32  sse_val;
} xmedia_venc_sse_info;

typedef struct {
    xmedia_u32             madi_val;
    xmedia_u32             madp_val;
} xmedia_venc_stream_advance_info_h264;

typedef struct {
} xmedia_venc_stream_advance_info_jpeg;

typedef struct {
    xmedia_u32             madi_val;
    xmedia_u32             madp_val;
} xmedia_venc_stream_advance_info_h265;

typedef struct {
    xmedia_u32 pic_bytes_num;
    xmedia_u32 update_attr_cnt;
    xmedia_u32 q_factor;
} xmedia_venc_stream_info_jpeg;

typedef struct {
    xmedia_venc_pack ATTRIBUTE* pack;
    xmedia_u32 ATTRIBUTE pack_count;
    xmedia_u32 seq;

    union {
        xmedia_venc_stream_info_h264   h264_info;
        xmedia_venc_stream_info_jpeg   jpeg_info;
        xmedia_venc_stream_info_h265   h265_info;
    };

    union {
        xmedia_venc_stream_advance_info_h264   advance_h264_info;
        xmedia_venc_stream_advance_info_jpeg   advance_jpeg_info;
        xmedia_venc_stream_advance_info_h265   advance_h265_info;
    };
} xmedia_venc_stream;

typedef struct {
    xmedia_h265e_ref_type ref_type;

    xmedia_u32  pic_bytes_num;
    xmedia_u32  pic_cnt;
    xmedia_u32  start_qp;
    xmedia_u32  mean_qp;
    xmedia_bool p_skip;

    xmedia_u32  madi_val;
    xmedia_u32  madp_val;
} xmedia_venc_stream_info;

typedef struct xmedia_venc_mpf_cfg {
    xmedia_u8   large_thumbnail_num;
    xmedia_video_size  large_thumbnail_size[2];
} xmedia_venc_mpf_cfg;

typedef struct {
    xmedia_bool support_dcf;
    xmedia_venc_mpf_cfg mpf_cfg;
    xmedia_venc_pic_recive_mode rcv_mode;
} xmedia_venc_attr_jpeg;

typedef struct {
} xmedia_venc_attr_mjpeg;

typedef struct {
    xmedia_bool rcn_ref_share_buf;
} xmedia_venc_attr_h264;

typedef struct {
    xmedia_bool rcn_ref_share_buf;
} xmedia_venc_attr_h265;

typedef struct {
    xmedia_payload_type en_type;

    xmedia_u32 max_pic_width;
    xmedia_u32 max_pic_height;

    xmedia_u32 stream_buf_size;
    xmedia_u32 profile;
    xmedia_bool by_frame;
    xmedia_u32 pic_width;
    xmedia_u32 pic_height;
    union {
        xmedia_venc_attr_h264 h264e_attr;
        xmedia_venc_attr_h265 h265e_attr;
        xmedia_venc_attr_mjpeg mjpege_attr;
        xmedia_venc_attr_jpeg jpege_attr;
    };
} xmedia_venc_attr;

typedef struct {
    xmedia_s32 ip_qp_delta;
} xmedia_venc_gop_normal;

typedef struct {
    xmedia_u32 sp_interval;
    xmedia_s32 sp_qp_delta;
    xmedia_s32 ip_qp_delta;
} xmedia_venc_gop_dual_p;

typedef struct {
    xmedia_u32 bg_interval;
    xmedia_s32 bg_qp_delta;
    xmedia_s32 vi_qp_delta;
} xmedia_venc_gop_smart_p;

typedef struct {
    xmedia_u32 bg_interval;
    xmedia_s32 bg_qp_delta;
    xmedia_s32 vi_qp_delta;
} xmedia_venc_gop_adv_smart_p;

typedef struct {
    xmedia_venc_gop_mode gop_mode;
    union {
        xmedia_venc_gop_normal normal_p;
        xmedia_venc_gop_smart_p smart_p;
        xmedia_venc_gop_adv_smart_p adv_smart_p;
    };
} xmedia_venc_gop_attr;

typedef struct {
    xmedia_venc_attr venc_attr;
    xmedia_venc_rc_attr rc_attr;
    xmedia_venc_gop_attr gop_attr;
} xmedia_venc_chn_attr;

typedef struct {
    xmedia_s32 recv_pic_num;
} xmedia_venc_recv_pic_param;

typedef struct {
    xmedia_u32 left_pics;
    xmedia_u32 left_stream_bytes;
    xmedia_u32 left_stream_frames;
    xmedia_u32 cur_packs;
    xmedia_u32 left_recv_pics;
    xmedia_u32 left_enc_pics;
    xmedia_bool jpeg_snap_end;
    xmedia_venc_stream_info venc_strm_info;
} xmedia_venc_chn_status;

typedef struct {
    xmedia_bool split_enable;
    xmedia_u32  mb_line_num;
} xmedia_venc_h264_slice_split;

typedef struct {
    xmedia_u32     constrained_intra_pred_flag;
} xmedia_venc_h264_intra_pred;

typedef struct {
    xmedia_u32     intra_trans_mode;
    xmedia_u32     inter_trans_mode;

    xmedia_bool    scaling_list_valid;
    xmedia_u8      inter_scaling_list8X8[64];
    xmedia_u8      intra_scaling_list8X8[64];

    xmedia_s32     chroma_qp_index_offset;
} xmedia_venc_h264_trans;

typedef struct {
    xmedia_u32 entropy_enc_mode_i;
    xmedia_u32 entropy_enc_mode_p;
    xmedia_u32 entropy_enc_mode_b;
    xmedia_u32 cabac_init_idc;
} xmedia_venc_h264_entropy;

typedef struct {
    xmedia_u32 pic_order_cnt_type;
} xmedia_venc_h264_poc;

typedef struct {
    xmedia_u32 disable_deblocking_filter_idc;
    xmedia_s32 slice_alpha_c0_offset_div2;
    xmedia_s32 slice_beta_offset_div2;
} xmedia_venc_h264_dblk;

typedef struct {
    xmedia_u8  timing_info_present_flag;
    xmedia_u8  fixed_frame_rate_flag;
    xmedia_u32 num_units_in_tick;
    xmedia_u32 time_scale;
} xmedia_venc_vui_h264_time_info;

typedef struct {
    xmedia_u8  aspect_ratio_info_present_flag;
    xmedia_u8  aspect_ratio_idc;
    xmedia_u8  overscan_info_present_flag;
    xmedia_u8  overscan_appropriate_flag;
    xmedia_u16 sar_width;
    xmedia_u16 sar_height ;
} xmedia_venc_vui_aspect_ratio;

typedef struct {
    xmedia_u8  video_signal_type_present_flag ;
    xmedia_u8  video_format ;
    xmedia_u8  video_full_range_flag;
    xmedia_u8  colour_description_present_flag ;
    xmedia_u8  colour_primaries ;
    xmedia_u8  transfer_characteristics;
    xmedia_u8  matrix_coefficients;
} xmedia_venc_vui_video_signal;

typedef struct {
    xmedia_u8  bitstream_restriction_flag ;
} xmedia_venc_vui_bitstream_restric;

typedef struct {
    xmedia_venc_vui_aspect_ratio           vui_aspect_ratio;
    xmedia_venc_vui_h264_time_info         vui_time_info;
    xmedia_venc_vui_video_signal           vui_video_signal;
    xmedia_venc_vui_bitstream_restric      vui_bitstream_restric;
} xmedia_venc_h264_vui;

typedef struct {
    xmedia_u32 timing_info_present_flag;
    xmedia_u32 num_units_in_tick;
    xmedia_u32 time_scale;
    xmedia_u32 num_ticks_poc_diff_one_minus1;
} xmedia_venc_vui_h265_time_info;

typedef struct {
    xmedia_venc_vui_aspect_ratio       vui_aspect_ratio;
    xmedia_venc_vui_h265_time_info     vui_time_info;
    xmedia_venc_vui_video_signal       vui_video_signal;
    xmedia_venc_vui_bitstream_restric  vui_bitstream_restric;
} xmedia_venc_h265_vui;

#define VB_SUPPLEMENT_JPEG_MASK      VB_SUPPLEMENT_ISPINFO_MASK
#define JPEG_DRSCRIPTION_LENGTH      32
#define JPEG_CAPTURE_TIME_LENGTH     20

typedef struct {
    xmedia_u8       image_description[JPEG_DRSCRIPTION_LENGTH];//图像描述
    xmedia_u8       maker[JPEG_DRSCRIPTION_LENGTH];//相机制造商
    xmedia_u8       model[JPEG_DRSCRIPTION_LENGTH];//相机型号
    xmedia_u8       software[JPEG_DRSCRIPTION_LENGTH];//软件版本

    xmedia_u8       light_source;//光源
    xmedia_u32      focal_length;//焦距
    xmedia_u8       scene_type;//场景类型
    xmedia_u8       custom_rendered;//是否经过特殊处理
    xmedia_u8       focal_length_in35mm_film;//35mm焦距
    xmedia_u8       scene_capture_type;//现场捕捉类型
    xmedia_u8       gain_control;//增益控制
    xmedia_u8       contrast;//对比度模式
    xmedia_u8       saturation;//饱和度
    xmedia_u8       sharpness;//锐度
    xmedia_u8       metering_mode;//测光方式
} xmedia_venc_exif_info;

typedef struct {
    xmedia_u32 q_factor;
    xmedia_u8 y_qt[64];
    xmedia_u8 cb_qt[64];
    xmedia_u8 cr_qt[64];
    xmedia_u32 mcu_per_ecs;
    xmedia_u32 max_renc_times;
    xmedia_bool ecs_output_en;
} xmedia_venc_jpeg_param;

typedef struct {
    xmedia_u8 y_qt[64];
    xmedia_u8 cb_qt[64];
    xmedia_u8 cr_qt[64];
    xmedia_u32 mcu_per_ecs;
    xmedia_u32 max_renc_times;
    xmedia_bool ecs_output_en;
} xmedia_venc_mjpeg_param;

typedef struct {
    xmedia_u32  index;
    xmedia_bool enable;
    xmedia_bool abs_qp;
    xmedia_s32  qp;
    xmedia_video_rect  rect;
} xmedia_venc_roi_attr;

typedef struct {
    xmedia_u32  index;
    xmedia_bool enable[3]; // 0:I, 1:P/B, 2:VI
    xmedia_bool abs_qp[3];
    xmedia_s32  qp[3];
    xmedia_video_rect  rect[3];
}xmedia_venc_roi_attr_ex;

typedef struct {
    xmedia_s32 src_frm_rate;
    xmedia_s32 dst_frm_rate;
} xmedia_venc_roibg_frame_rate;

typedef struct {
    xmedia_u32       base;
    xmedia_u32       enhance;
    xmedia_bool      enable_pred;
} xmedia_venc_ref_param;

typedef struct {
    xmedia_u64   phy_addr[MAX_TILE_NUM];
    xmedia_void ATTRIBUTE* user_addr[MAX_TILE_NUM];
    xmedia_u64  ATTRIBUTE buf_size[MAX_TILE_NUM];
} xmedia_venc_stream_buf_info;

typedef struct {
    xmedia_bool split_enable;
    xmedia_u32  lcu_line_num;
} xmedia_venc_h265_slice_split;

typedef struct {
    xmedia_u32    constrained_intra_pred_flag;
    xmedia_u32    strong_intra_smoothing_enabled_flag;
} xmedia_venc_h265_pu;

typedef struct {
    xmedia_s32  cb_qp_offset;
    xmedia_s32  cr_qp_offset;

    xmedia_bool scaling_list_enabled;

    xmedia_bool scaling_list_tu4_valid;
    xmedia_u8   inter_scaling_list4X4[2][16];
    xmedia_u8   intra_scaling_list4X4[2][16];

    xmedia_bool scaling_list_tu8_valid;
    xmedia_u8   inter_scaling_list8X8[2][64];
    xmedia_u8   intra_scaling_list8X8[2][64];

    xmedia_bool scaling_list_tu16_valid;
    xmedia_u8   inter_scaling_list16X16[2][64];
    xmedia_u8   intra_scaling_list16X16[2][64];

    xmedia_bool scaling_list_tu32_valid;
    xmedia_u8   inter_scaling_list32X32[64];
    xmedia_u8   intra_scaling_list32X32[64];
} xmedia_venc_h265_trans;

typedef struct {
    xmedia_u32 cabac_init_flag;
} xmedia_venc_h265_entropy;

typedef struct {
    xmedia_u32 slice_deblocking_filter_disabled_flag;
    xmedia_s32 slice_beta_offset_div2;
    xmedia_s32 slice_tc_offset_div2;
} xmedia_venc_h265_dblk;

typedef struct {
    xmedia_u32  slice_sao_en;
} xmedia_venc_h265_sao;



typedef struct {
    xmedia_bool                     refresh_enable;
    xmedia_venc_intra_refresh_mode   intra_refresh_mode;
    xmedia_u32                      refresh_num;
    xmedia_u32                      req_i_qp;
} xmedia_venc_intra_refresh;

typedef struct {
    xmedia_u32             one_stream_buffer;
    xmedia_u32             h264e_mini_buf_mode;
    xmedia_u32             h264e_power_save_en;
    xmedia_video_vb_source h264e_vb_source;
    xmedia_bool            qp_hstgrm_en;
} xmedia_venc_mod_h264e;

typedef struct {
    xmedia_u32             one_stream_buffer;
    xmedia_u32             h265e_mini_buf_mode;
    xmedia_u32             h265e_power_save_en;
    xmedia_video_vb_source h265e_vb_source;
    xmedia_bool            qp_hstgrm_en;
} xmedia_venc_mod_h265e;

typedef struct {
    xmedia_u32  jpege_one_stream_buffer;
    xmedia_u32  jpege_mini_buf_mode;
    xmedia_u32  jpege_clear_stream_buf;
    xmedia_u32  jpege_dring_mode;
} xmedia_venc_mod_jpege;

typedef struct {
    xmedia_u32  clr_stat_after_set_br;
} xmedia_venc_mod_rc;

typedef struct {
    xmedia_u32 venc_buffer_cache;
    xmedia_u32 frame_buf_recycle;
} xmedia_venc_mod_venc;

typedef struct {
    xmedia_venc_mod_type venc_mod_type;
    union {
        xmedia_venc_mod_venc  venc_mod_param;
        xmedia_venc_mod_h264e h264e_mod_param;
        xmedia_venc_mod_h265e h265e_mod_param;
        xmedia_venc_mod_jpege jpege_mod_param;
        xmedia_venc_mod_rc    rc_mod_param;
    };
} xmedia_venc_param_mod;

typedef struct {
    xmedia_bool qp_map_valid;
    xmedia_bool skip_weight_valid;
    xmedia_u32  blk_start_qp;
    xmedia_u64  qp_map_phy_addr;
    xmedia_u64  skip_weight_phy_addr;
    xmedia_venc_frame_type frame_type;
} xmedia_user_rc_info;

typedef struct {
    xmedia_video_frame_info user_frame;
    xmedia_user_rc_info     user_rc_info;
} xmedia_user_frame_info;

typedef struct {
    xmedia_u32  index;
    xmedia_bool enable;
    xmedia_video_rect  rect;
} xmedia_venc_sse_cfg;


typedef struct {
    xmedia_bool enable;
    xmedia_video_rect  rect;
} xmedia_venc_crop_info;

typedef struct {
    xmedia_s32 src_frm_rate;
    xmedia_s32 dst_frm_rate;
} xmedia_venc_frame_rate;

typedef struct {
    xmedia_bool color_to_grey;
    xmedia_u32 priority;
    xmedia_u32 max_strm_cnt;
    xmedia_u32 poll_wake_up_frm_cnt;
    xmedia_venc_crop_info crop_cfg;
    xmedia_venc_frame_rate frame_rate;
    xmedia_bool jpeg_420_to_422_enable;
} xmedia_venc_chn_param;

typedef struct {
    xmedia_bool background_cu_rc_en;
    xmedia_bool tracking_upper_layer_en;
    xmedia_bool skip_weight_en;
    xmedia_s32 fg_weight;
    xmedia_s32 bg_weight;
    xmedia_u32 background_thresh[RC_BG_THR_SIZE];
    xmedia_s32 background_qp_delta[RC_BG_THR_SIZE];
    xmedia_s32 tracking_qp_delta[RC_BG_THR_SIZE];
} xmedia_venc_foreground_protect;

typedef struct {
    xmedia_bool   enable;
    xmedia_s32    strength0;
    xmedia_s32    strength1;
} xmedia_venc_debreath_effect;

typedef struct {
    xmedia_video_operation_mode pred_mode;

    xmedia_s8 cu32_intra_cost_prefer;
    xmedia_s8 cu16_intra_cost_prefer;
    xmedia_s8 cu8_intra_cost_prefer;
    xmedia_s8 cu4_intra_cost_prefer;

    xmedia_s8 cu8_inter_nomal_cost_prefer;
    xmedia_s8 cu16_inter_nomal_cost_prefer;
    xmedia_s8 cu32_inter_nomal_cost_prefer;

    xmedia_s8 cu8_merge_cost_prefer;
    xmedia_s8 cu16_merge_cost_prefer;
    xmedia_s8 cu32_merge_cost_prefer;

    xmedia_s8 cu8_skip_merge_cost_prefer;
    xmedia_s8 cu16_skip_merge_cost_prefer;
    xmedia_s8 cu32_skip_merge_cost_prefer;

    xmedia_s8 cu8_block_split_cost_prefer;
    xmedia_s8 cu16_block_split_cost_prefer;
    xmedia_s8 cu32_block_split_cost_prefer;
    xmedia_s32 cu8x8_part_4x4;   /* Does 8x8 cu divide 4x4 configuration */
} xmedia_venc_rc_rdo_prefer;

typedef struct {
    xmedia_video_operation_mode pred_mode;

    xmedia_u32 intra32_cost_thres;
    xmedia_u32 intra32_cost_offset;
    xmedia_u32 intra16_cost_thres;
    xmedia_u32 intra16_cost_offset;
    xmedia_u32 intra8_cost_thres;
    xmedia_u32 intra8_cost_offset;
    xmedia_u32 intra4_cost_thres;
    xmedia_u32 intra4_cost_offset;

    xmedia_u32 inter64_cost_thres;
    xmedia_u32 inter64_cost_offset;
    xmedia_u32 inter32_cost_thres;
    xmedia_u32 inter32_cost_offset;
    xmedia_u32 inter16_cost_thres;
    xmedia_u32 inter16_cost_offset;
    xmedia_u32 inter8_cost_thres;
    xmedia_u32 inter8_cost_offset;

    xmedia_u32 inter64_skip_cost_thres;
    xmedia_u32 inter64_skip_cost_offset;
    xmedia_u32 inter32_skip_cost_thres;
    xmedia_u32 inter32_skip_cost_offset;
    xmedia_u32 inter16_skip_cost_thres;
    xmedia_u32 inter16_skip_cost_offset;
    xmedia_u32 inter8_skip_cost_thres;
    xmedia_u32 inter8_skip_cost_offset;
    xmedia_s32 cu8x8_part_4x4;   /* 8x8 cu是否划分4x4配置 */
} xmedia_venc_cu_prediction;

typedef struct {
     xmedia_bool   skip_bias_en;
     xmedia_u32    skip_thresh_gain;
     xmedia_u32    skip_thresh_offset;
     xmedia_u32    skip_background_cost;
     xmedia_u32    skip_foreground_cost;
} xmedia_venc_skip_bias;

typedef struct {
    xmedia_bool     hierarchical_qp_en;
    xmedia_s32      hierarchical_qp_delta[4];
    xmedia_s32      hierarchical_frame_num[4];
} xmedia_venc_hierarchical_qp;

typedef struct {
    xmedia_u32 pic_vb_pool;
    xmedia_u32 pic_info_vb_pool;
} xmedia_venc_chn_pool;

typedef struct {
    xmedia_u32 clear_stat_after_set_attr;
} xmedia_venc_rc_adv_param;

typedef struct {
    xmedia_u8 qpmap_value_i;
    xmedia_u8 qpmap_value_p;
    xmedia_s8 skipmap_value;
} xmedia_venc_svc_map_param;

typedef struct {
    xmedia_venc_svc_map_param  fg_region[FG_REGION_MAX];
    xmedia_venc_svc_map_param  activity_region;
    xmedia_venc_svc_map_param  bg_region;
    xmedia_bool fg_protect_adaptive_en;
    xmedia_u8 bg_area_thr;
} xmedia_venc_svc_param;

#define VENC_MAX_SVC_RECT_NUM 32

typedef struct {
    xmedia_u32 rect_num;
    xmedia_video_size base_resolution;
    xmedia_video_rect rect_attr[VENC_MAX_SVC_RECT_NUM];
    xmedia_venc_svc_rect_type detect_type[VENC_MAX_SVC_RECT_NUM];
    xmedia_u64 pts;
} xmedia_venc_svc_rect_info;

typedef struct {
    xmedia_u16 scene_complexity;
    xmedia_u8  object_level;
} xmedia_venc_svc_detect_result;

typedef struct {
    xmedia_venc_pme_search_window pme_search;
    xmedia_venc_ime_search_window ime_search;
} xmedia_venc_search_window;

typedef struct {
    xmedia_bool switch_enable;
    xmedia_s32 pipe_id;
} xmedia_venc_switch_chn;

typedef struct {
    xmedia_s32 chn_id;
    xmedia_u32 rcn_ratio;
}xmedia_venc_rcn_ratio;

typedef struct {
    xmedia_u32 lcu;
    xmedia_u32 total_lcu;
}xmedia_venc_report_lcu;

typedef struct {
    xmedia_bool skip_enable;
    xmedia_u32 vi_frame_rate;
    xmedia_u32 skip_frame_rate;
} xmedia_venc_skip_param;


#define SVP_MAXINUM_LIMIT 10000

#ifndef __KERNEL__
/*
 *  函数功能：监听某个venc通道是否有码流待获取
 *  函数参数：
        chn_mask -  [31:16] venc最大通道个数为16，[31:16]的bit位设置为0
                    [15: 0] 对所有为1的bit位对应的通道进行监听
 *      timeout_val - 设置超时时间
                      设置为NULL：永久阻塞；设置为0，不阻塞；设置为大于0，阻塞对应时间
 *  返回值：
 *      XMEDIA_FAILURE               - 失败
 *      XMEDIA_SUCCESS               - 有码流待收取
 *      XMEDIA_ERRCODE_INVALID_PARAM - 设置chn_mask值非法，[31:16]有bit位设置为1
 *      XMEDIA_ERRCODE_TIMEOUT       - 超出设置阻塞时间未检测到码流
 *      XMEDIA_ERRCODE_NOT_READY     - 系统未准备好
 */
xmedia_s32 xmedia_venc_select(xmedia_u32 chn_mask, struct timeval *timeout_val);
#endif

/*
 *  函数功能：获取某个venc通道的fd
 *  函数参数：
        venc_chn - 通道号
 *  返回值：
 *      venc_chn对应通道的fd
 */
xmedia_s32 xmedia_venc_get_fd(xmedia_s32 venc_chn);

/*
 *  函数功能：关闭某个venc通道的fd
 *  函数参数：
        venc_chn - 通道号
 *  返回值：
 *      XMEDIA_SUCCESS                - 关闭对应通道fd成功
 *      XMEDIA_FAILURE                - 函数执行失败
 */
xmedia_s32 xmedia_venc_close_fd(xmedia_s32 venc_chn);

/*
 *  函数功能：VENC模块初始化
 *  函数参数：
 *      无
 *  返回值：
 *      XMEDIA_SUCCESS           - 函数执行成功
 *      XMEDIA_FAILURE           - 函数执行失败
 *      XMEDIA_ERRCODE_NOT_READY - 系统未准备好
 */
xmedia_s32 xmedia_venc_init(xmedia_void);

/*
 *  函数功能：VENC模块去初始化
 *  函数参数：
 *      无
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_FAILURE - 函数执行失败
 */
xmedia_s32 xmedia_venc_exit(xmedia_void);

/*
 *  函数功能：创建VENC通道
 *  函数参数：
 *      venc_chn - 通道号
 *      chn_attr - 通道参数集
 *  返回值：
 *      XMEDIA_SUCCESS                  - 函数执行成功
 *      XMEDIA_FAILURE                   - 函数执行失败
 *      XMEDIA_ERRCODE_INVALID_CHN_ID    - 无效的通道号
 *      XMEDIA_ERRCODE_NOT_SUPPORT       - 不支持的类型
 *      XMEDIA_ERRCODE_NULL_PTR          - 空指针
 *      XMEDIA_ERRCODE_NO_BUFFER_FREE    - 没有空闲buffer
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 无效参数
 *      XMEDIA_ERRCODE_EXIST             - 通道已创建
 *      XMEDIA_ERRCODE_NOT_READY         - 系统未准备好
 *      XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 内存不足
 *      XMEDIA_ERRCODE_NOT_EXIST         - 通道不存在
 */
xmedia_s32 xmedia_venc_create_chn(xmedia_s32 venc_chn, const xmedia_venc_chn_attr *chn_attr);

/*
 *  函数功能：销毁VENC通道
 *  函数参数：
 *      venc_chn - 通道号
 *  返回值：
 *      XMEDIA_SUCCESS                - 函数执行成功
 *      XMEDIA_FAILURE                - 函数执行失败
 *      XMEDIA_ERRCODE_NOT_EXIST      - 通道不存在
 *      XMEDIA_ERRCODE_NOT_PERMITTED  - 操作不允许
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道号
 *      XMEDIA_ERRCODE_BUSY           - 设备正在执行其他操作
 *      XMEDIA_ERRCODE_INVALID_PARAM  - 无效参数
 *      XMEDIA_ERRCODE_NULL_PTR       - 参数为空指针
 *      XMEDIA_ERRCODE_TYPE_ERROR     - chnl模块通道类型不支持
 *      XMEDIA_ERRCODE_NOT_READY      - 系统未准备好
 */
xmedia_s32 xmedia_venc_destroy_chn(xmedia_s32 venc_chn);

/*
 *  函数功能：VENC通道重置
 *  函数参数：
 *      venc_chn - 通道号
 *  返回值：
 *      XMEDIA_SUCCESS                - 函数执行成功
 *      XMEDIA_FAILURE                - 函数执行失败
 *      XMEDIA_ERRCODE_INVALID_PARAM  - 无效参数
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道号
 *      XMEDIA_ERRCODE_NOT_READY      - 系统未准备好
 *      XMEDIA_ERRCODE_NOT_EXIST      - 通道不存在
 *      XMEDIA_ERRCODE_NOT_PERMITTED  - 操作不允许
 *      XMEDIA_ERRCODE_TYPE_ERROR     - chnl模块通道类型不支持
 */
xmedia_s32 xmedia_venc_reset_chn(xmedia_s32 venc_chn);

/*
 *  函数功能：VENC通道开始收帧&编码
 *  函数参数：
 *      venc_chn   - 通道号
 *      recv_param - jpge持续编码多少帧，设置为-1为不限编码帧数，h264/h265无效
 *  返回值：
 *      XMEDIA_SUCCESS                - 函数执行成功
 *      XMEDIA_FAILURE                - 函数执行失败
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道号
 *      XMEDIA_ERRCODE_INVALID_PARAM  - 无效参数
 *      XMEDIA_ERRCODE_NOT_READY      - 系统未准备好
 *      XMEDIA_ERRCODE_NULL_PTR       - 入参为空指针
 *      XMEDIA_ERRCODE_NOT_PERMITTED  - 操作不允许
 *      XMEDIA_ERRCODE_NOT_EXIST      - 未创建通道
 */
xmedia_s32 xmedia_venc_start_recv_frame(xmedia_s32 venc_chn, const xmedia_venc_recv_pic_param *recv_param);

/*
 *  函数功能：VENC通道停止收帧&编码
 *  函数参数：
 *      venc_chn       - 通道号
 *  返回值：
 *      XMEDIA_SUCCESS                - 函数执行成功
 *      XMEDIA_FAILURE                - 函数执行失败
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道号
 *      XMEDIA_ERRCODE_NOT_READY      - 系统未准备好
 *      XMEDIA_ERRCODE_NOT_EXIST      - 通道未创建
 */
xmedia_s32 xmedia_venc_stop_recv_frame(xmedia_s32 venc_chn);

/*
 *  函数功能：获取VENC通道状态
 *  函数参数：
 *      venc_chn   - 通道号
 *      status     - 通道状态集
 *  返回值：
 *      XMEDIA_SUCCESS                - 函数执行成功
 *      XMEDIA_FAILURE                - 函数执行失败
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道号
 *      XMEDIA_ERRCODE_NOT_READY      - 系统未准备好
 *      XMEDIA_ERRCODE_NULL_PTR       - 入参为空指针
 *      XMEDIA_ERRCODE_NOT_EXIST      - 通道未创建
 */
xmedia_s32 xmedia_venc_query_status(xmedia_s32 venc_chn, xmedia_venc_chn_status *status);

/*
 *  函数功能：设置VENC通道参数
 *  函数参数：
 *      venc_chn   - 通道号
 *      chn_attr   - 通道参数集
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_FAILURE                   - 函数执行失败
 *      XMEDIA_ERRCODE_INVALID_CHN_ID    - 无效的通道号
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 无效参数
 *      XMEDIA_ERRCODE_NOT_READY         - 系统未准备好
 *      XMEDIA_ERRCODE_NULL_PTR          - 入参为空指针
 *      XMEDIA_ERRCODE_NOT_EXIST         - 通道未创建
 *      XMEDIA_ERRCODE_NOT_PERMITTED     - 操作不允许
 *      XMEDIA_ERRCODE_NOT_SUPPORT       - 不支持的协议类型
 *      XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 内存不足
 *      XMEDIA_ERRCODE_TYPE_ERROR        - 不支持的类型
 */
xmedia_s32 xmedia_venc_set_chn_attr(xmedia_s32 venc_chn, const xmedia_venc_chn_attr *chn_attr);

/*
 *  函数功能：获取VENC通道参数
 *  函数参数：
 *      venc_chn   - 通道号
 *      chn_attr   - 通道参数集
 *  返回值：
 *      XMEDIA_SUCCESS                - 函数执行成功
 *      XMEDIA_FAILURE                - 函数执行失败
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道号
 *      XMEDIA_ERRCODE_NOT_READY      - 系统未准备好
 *      XMEDIA_ERRCODE_NULL_PTR       - 入参为空指针
 *      XMEDIA_ERRCODE_NOT_EXIST      - 通道未创建
 */
xmedia_s32 xmedia_venc_get_chn_attr(xmedia_s32 venc_chn, xmedia_venc_chn_attr *chn_attr);

/*
 *  函数功能：获取VENC通道码流
 *  函数参数：
 *      venc_chn   - 通道号
 *      stream     - 码流信息
 *      milli_sec  - 超时等待时间（毫秒），设置为-1为阻塞式等待
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_FAILURE                   - 函数执行失败
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 无效参数
 *      XMEDIA_ERRCODE_NOT_READY         - 系统未准备好
 *      XMEDIA_ERRCODE_INVALID_CHN_ID    - 无效的通道号
 *      XMEDIA_ERRCODE_NULL_PTR          - 入参为空指针
 *      XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 内存不足
 *      XMEDIA_ERRCODE_NOT_EXIST         - 通道未创建
 *      XMEDIA_ERRCODE_NOT_SUPPORT       - milli_sec值设置<-1,返回该值
 *      XMEDIA_ERRCODE_BUSY              - 在指定的milli_sec内被唤醒，并且没有被信号打断
 *      XMEDIA_ERRCODE_BUFFER_EMPTY      - buffer为空，获取不到帧
 */
xmedia_s32 xmedia_venc_get_stream(xmedia_s32 venc_chn, xmedia_venc_stream *stream, xmedia_s32 milli_sec);

/*
 *  函数功能：释放码流
 *  函数参数：
 *      venc_chn   - 通道号
 *      stream     - 码流信息
 *  返回值：
 *      XMEDIA_SUCCESS                - 函数执行成功
 *      XMEDIA_FAILURE                - 函数执行失败
 *      XMEDIA_ERRCODE_INVALID_PARAM  - 无效参数
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道号
 *      XMEDIA_ERRCODE_NOT_READY      - 系统未准备好
 *      XMEDIA_ERRCODE_NULL_PTR       - 入参为空指针
 *      XMEDIA_ERRCODE_NOT_EXIST      - 通道未创建
 *      XMEDIA_ERRCODE_NO_BUFFER_FREE - 没有帧需要释放
 */
xmedia_s32 xmedia_venc_release_stream(xmedia_s32 venc_chn, xmedia_venc_stream *stream);

/*
 *  函数功能：编码插入用户数据
 *  函数参数：
 *      venc_chn       - 通道号
 *      data           - 要插入的数据的虚拟地址
 *      len            - 要插入的数据长度
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_FAILURE                   - 函数执行失败
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 无效参数
 *      XMEDIA_ERRCODE_INVALID_CHN_ID    - 无效的通道号
 *      XMEDIA_ERRCODE_NOT_READY         - 系统未准备好
 *      XMEDIA_ERRCODE_NULL_PTR          - 入参为空指针
 *      XMEDIA_ERRCODE_NOT_EXIST         - 通道未创建
 *      XMEDIA_ERRCODE_NOT_SUPPORT       - 不支持插入用户数据
 *      XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 分配内存失败
 *      XMEDIA_ERRCODE_BUFFER_FULL       - 无法分配SEI memory
 *      XMEDIA_ERRCODE_NO_BUFFER_FREE    - 获取队列节点失败
 */
xmedia_s32 xmedia_venc_insert_user_data(xmedia_s32 venc_chn, xmedia_u8 *data, xmedia_u32 len);

/*
 *  函数功能：用户送帧编码
 *  函数参数：
 *      venc_chn   - 通道号
 *      frame      - 帧信息
 *      milli_sec  - 设置超时时间（毫秒），-1为阻塞式等待， 0 非阻塞， > 0 超时时间
 *  返回值：
 *      XMEDIA_SUCCESS                - 函数执行成功
 *      XMEDIA_FAILURE                - 函数执行失败
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道号
 *      XMEDIA_ERRCODE_INVALID_PARAM  - 无效参数
 *      XMEDIA_ERRCODE_NOT_READY      - 系统未准备好
 *      XMEDIA_ERRCODE_NULL_PTR       - 入参为空指针
 *      XMEDIA_ERRCODE_NOT_PERMITTED  - 操作不允许
 *      XMEDIA_ERRCODE_NOT_EXIST      - 通道未创建
 *      XMEDIA_ERRCODE_BUSY           - 在指定的milli_sec内被唤醒，并且没有被信号打断
 *      XMEDIA_ERRCODE_NO_BUFFER_FREE - 未获取到可用的buffer
 */
xmedia_s32 xmedia_venc_send_frame(xmedia_s32 venc_chn, const xmedia_video_frame_info *frame_info, xmedia_s32 milli_sec);

/*
 *  函数功能：用户送帧编码，可自定义qpmap和skipmap
 *  函数参数：
 *      venc_chn   - 通道号
 *      frame      - 帧信息
 *      milli_sec  - 设置超时时间（毫秒），-1为阻塞式等待， 0 非阻塞， > 0 超时时间
 *  返回值：
 *      XMEDIA_SUCCESS                - 函数执行成功
 *      XMEDIA_FAILURE                - 函数执行失败
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道号
 *      XMEDIA_ERRCODE_INVALID_PARAM  - 无效参数
 *      XMEDIA_ERRCODE_NOT_READY      - 系统未准备好
 *      XMEDIA_ERRCODE_NULL_PTR       - 入参为空指针
 *      XMEDIA_ERRCODE_NOT_PERMITTED  - 操作不允许
 *      XMEDIA_ERRCODE_NOT_EXIST      - 通道未创建
 *      XMEDIA_ERRCODE_BUSY           - 在指定的milli_sec内被唤醒，并且没有被信号打断
 *      XMEDIA_ERRCODE_NO_BUFFER_FREE - 未获取到可用的buffer
 */
xmedia_s32 xmedia_venc_send_frame_ex(xmedia_s32 venc_chn, const xmedia_user_frame_info *frame_info, xmedia_s32 milli_sec);

/*
 *  函数功能：请求idr帧
 *  函数参数：
 *      venc_chn       - 通道号
 *      instant        - 实例
 *  返回值：
 *      XMEDIA_SUCCESS                - 函数执行成功
 *      XMEDIA_FAILURE                - 函数执行失败
 *      XMEDIA_ERRCODE_INVALID_PARAM  - 无效参数
 *      XMEDIA_ERRCODE_NOT_READY      - 系统未准备好
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道号
 *      XMEDIA_ERRCODE_NOT_EXIST      - 通道未创建
 *      XMEDIA_ERRCODE_NOT_SUPPORT    - 不支持请求idr帧协议类型

 */
xmedia_s32 xmedia_venc_request_idr(xmedia_s32 venc_chn, xmedia_bool instant);

/*
 *  函数功能：设置roi参数
 *  函数参数：
 *      venc_chn   - 通道号
 *      roi_attr   - roi参数集
 *  返回值：
 *      XMEDIA_SUCCESS                - 函数执行成功
 *      XMEDIA_FAILURE                - 函数执行失败
 *      XMEDIA_ERRCODE_INVALID_PARAM  - 无效参数
 *      XMEDIA_ERRCODE_NOT_READY      - 系统未准备好
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道号
 *      XMEDIA_ERRCODE_NULL_PTR       - 入参为空指针
 *      XMEDIA_ERRCODE_NOT_EXIST      - 通道未创建
 *      XMEDIA_ERRCODE_NOT_SUPPORT    - 协议类型不支持roi
 */
xmedia_s32 xmedia_venc_set_roi_attr(xmedia_s32 venc_chn, const xmedia_venc_roi_attr *roi_attr);

/*
 *  函数功能：获取roi参数
 *  函数参数：
 *      venc_chn   - 通道号
 *      roi_attr   - roi参数集
 *  返回值：
 *      XMEDIA_SUCCESS                - 函数执行成功
 *      XMEDIA_FAILURE                - 函数执行失败
 *      XMEDIA_ERRCODE_INVALID_PARAM  - 无效参数
 *      XMEDIA_ERRCODE_NOT_READY      - 系统未准备好
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道号
 *      XMEDIA_ERRCODE_NULL_PTR       - 入参为空指针
 *      XMEDIA_ERRCODE_NOT_EXIST      - 通道未创建
 *      XMEDIA_ERRCODE_NOT_SUPPORT    - 协议类型不支持roi
 */
xmedia_s32 xmedia_venc_get_roi_attr(xmedia_s32 venc_chn, xmedia_u32 index, xmedia_venc_roi_attr *roi_attr);

/*
 *  函数功能：获取不同帧类型的扩展roi参数
 *  函数参数：
 *      venc_chn   - 通道号
 *      roi_attr   - roi参数集
 *  返回值：
 *      XMEDIA_SUCCESS                - 函数执行成功
 *      XMEDIA_FAILURE                - 函数执行失败
 *      XMEDIA_ERRCODE_INVALID_PARAM  - 无效参数
 *      XMEDIA_ERRCODE_NOT_READY      - 系统未准备好
 *      XMEDIA_ERRCODE_NULL_PTR       - 入参为空指针
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道号
 *      XMEDIA_ERRCODE_NOT_EXIST      - 通道未创建
 *      XMEDIA_ERRCODE_NOT_SUPPORT    - 协议类型不支持roi
 */
xmedia_s32 xmedia_venc_get_roi_attr_ex(xmedia_s32 venc_chn, xmedia_u32 index, xmedia_venc_roi_attr_ex *roi_attr_ex);

/*
 *  函数功能：针对不同帧类型，设置扩展roi参数
 *  函数参数：
 *      venc_chn   - 通道号
 *      roi_attr   - roi参数集
 *  返回值：
 *      XMEDIA_SUCCESS                - 函数执行成功
 *      XMEDIA_FAILURE                - 函数执行失败
 *      XMEDIA_ERRCODE_INVALID_CHN_ID - 无效的通道号
 *      XMEDIA_ERRCODE_INVALID_PARAM  - 无效参数
 *      XMEDIA_ERRCODE_NOT_READY      - 系统未准备好
 *      XMEDIA_ERRCODE_NULL_PTR       - 入参为空指针
 *      XMEDIA_ERRCODE_NOT_EXIST      - 通道未创建
 *      XMEDIA_ERRCODE_NOT_SUPPORT    - 协议类型不支持roi
 */
xmedia_s32 xmedia_venc_set_roi_attr_ex(xmedia_s32 venc_chn, const xmedia_venc_roi_attr_ex *roi_attr_ex);

/*
 *  函数功能：设置jpeg编码参数(压缩率、量化表、ecs)
 *  函数参数：
 *      venc_chn    - 通道号
 *      jpeg_param  - jpeg编码参数
 *  返回值：
 *      XMEDIA_SUCCESS                 - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_CHN_ID  - 无效的通道号
 *      XMEDIA_ERRCODE_NOT_EXIST       - 通道不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT     - 不支持的通道类型
 *      XMEDIA_ERRCODE_NOT_READY       - 系统未准备好
 *      XMEDIA_ERRCODE_NULL_PTR        - 空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM   - 输入参数非法
 *  注意：
 *      通道必须已创建
 */
xmedia_s32 xmedia_venc_set_jpeg_param(xmedia_s32 venc_chn, const xmedia_venc_jpeg_param *jpeg_param);

/*
 *  函数功能：获取jpeg编码参数(压缩率、量化表、ecs)
 *  函数参数：
 *      venc_chn    - 通道号
 *      jpeg_param  - jpeg编码参数
 *  返回值：
 *      XMEDIA_SUCCESS                 - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_CHN_ID  - 无效的通道号
 *      XMEDIA_ERRCODE_NOT_EXIST       - 通道不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT     - 不支持的通道类型
 *      XMEDIA_ERRCODE_NOT_READY       - 系统未准备好
 *      XMEDIA_ERRCODE_NULL_PTR        - 空指针
 *  注意：
 *      通道必须已创建
 */
xmedia_s32 xmedia_venc_get_jpeg_param(xmedia_s32 venc_chn, xmedia_venc_jpeg_param *jpeg_param);

/*
 *  函数功能：设置jpeg exif信息
 *  函数参数：
 *      venc_chn        - 通道号
 *      exif_info       - exif信息集
 *  返回值：
 *      XMEDIA_SUCCESS                 - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_CHN_ID  - 无效的通道号
 *      XMEDIA_ERRCODE_NOT_EXIST       - 通道不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT     - 不支持的通道类型
 *      XMEDIA_ERRCODE_NOT_READY       - 系统未准备好
 *      XMEDIA_ERRCODE_NULL_PTR        - 空指针
 *  注意：
 *      通道必须已创建
 */
xmedia_s32 xmedia_venc_set_jpeg_exif_info(xmedia_s32 venc_chn, xmedia_venc_exif_info *exif_info);

/*
 *  函数功能：获取jpeg exif信息
 *  函数参数：
 *      venc_chn        - 通道号
 *      exif_info       - exif信息集
 *  返回值：
 *      XMEDIA_SUCCESS                 - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_CHN_ID  - 无效的通道号
 *      XMEDIA_ERRCODE_NOT_EXIST       - 通道不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT     - 不支持的通道类型
 *      XMEDIA_ERRCODE_NOT_READY       - 系统未准备好
 *      XMEDIA_ERRCODE_NULL_PTR        - 空指针
 *  注意：
 *      通道必须已创建
 */
xmedia_s32 xmedia_venc_get_jpeg_exif_info(xmedia_s32 venc_chn, xmedia_venc_exif_info *exif_info);

/*
 *  函数功能：设置mjpeg编码参数(量化表、ecs)
 *  函数参数：
 *      venc_chn    - 通道号
 *      mjpeg_param - jpeg编码参数
 *  返回值：
 *      XMEDIA_SUCCESS                 - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_CHN_ID  - 无效的通道号
 *      XMEDIA_ERRCODE_NOT_EXIST       - 通道不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT     - 不支持的通道类型
 *      XMEDIA_ERRCODE_NOT_READY       - 系统未准备好
 *      XMEDIA_ERRCODE_NULL_PTR        - 空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM   - 输入参数非法
 *  注意：
 *      通道必须已创建
 */
xmedia_s32 xmedia_venc_set_mjpeg_param(xmedia_s32 venc_chn, const xmedia_venc_mjpeg_param *mjpeg_param);

/*
 *  函数功能：获取mjpeg编码参数(量化表、ecs)
 *  函数参数：
 *      venc_chn    - 通道号
 *      mjpeg_param - jpeg编码参数
 *  返回值：
 *      XMEDIA_SUCCESS                 - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_CHN_ID  - 无效的通道号
 *      XMEDIA_ERRCODE_NOT_EXIST       - 通道不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT     - 不支持的通道类型
 *      XMEDIA_ERRCODE_NOT_READY       - 系统未准备好
 *      XMEDIA_ERRCODE_NULL_PTR        - 空指针
 *  注意：
 *      通道必须已创建
 */
xmedia_s32 xmedia_venc_get_mjpeg_param(xmedia_s32 venc_chn, xmedia_venc_mjpeg_param *mjpeg_param);

/*
 *  函数功能：获取rc参数
 *  函数参数：
 *      venc_chn    - 通道号
 *      rc_param    - rc参数
 *  返回值：
 *      XMEDIA_SUCCESS                 - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_CHN_ID  - 无效的通道号
 *      XMEDIA_ERRCODE_NOT_EXIST       - 通道不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT     - 不支持的通道类型
 *      XMEDIA_ERRCODE_NOT_READY       - 系统未准备好
 *      XMEDIA_ERRCODE_NULL_PTR        - 空指针
 *  注意：
 *      通道必须已创建
 */
xmedia_s32 xmedia_venc_get_rc_param(xmedia_s32 venc_chn, xmedia_venc_rc_param *rc_param);

/*
 *  函数功能：设置rc参数
 *  函数参数：
 *      venc_chn    - 通道号
 *      rc_param    - rc参数
 *  返回值：
 *      XMEDIA_SUCCESS                 - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_CHN_ID  - 无效的通道号
 *      XMEDIA_ERRCODE_NOT_EXIST       - 通道不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT     - 不支持的通道类型
 *      XMEDIA_ERRCODE_NOT_READY       - 系统未准备好
 *      XMEDIA_ERRCODE_NULL_PTR        - 空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM   - 输入参数非法
 *  注意：
 *      通道必须已创建
 */
xmedia_s32 xmedia_venc_set_rc_param(xmedia_s32 venc_chn, const xmedia_venc_rc_param *rc_param);

/*
 *  函数功能：获取码流buf信息
 *  函数参数：
 *      venc_chn        - 通道号
 *      stream_buf_info - 码流信息
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_CHN_ID    - 无效的通道号
 *      XMEDIA_ERRCODE_NOT_EXIST         - 通道不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT       - 不支持的通道类型
 *      XMEDIA_ERRCODE_NOT_READY         - 系统未准备好
 *      XMEDIA_ERRCODE_NULL_PTR          - 空指针
 *      XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 内存不够
 *  注意：
 *      通道必须已创建
 */
xmedia_s32 xmedia_venc_get_stream_buf_info(xmedia_s32 venc_chn, xmedia_venc_stream_buf_info *stream_buf_info);

/*
 *  函数功能：设置h265 slice划分
 *  函数参数：
 *      venc_chn          - 通道号
 *      slice_split   - slice划分信息
 *  返回值：
 *      XMEDIA_SUCCESS                 - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_CHN_ID  - 无效的通道号
 *      XMEDIA_ERRCODE_NOT_EXIST       - 通道不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT     - 不支持的通道类型
 *      XMEDIA_ERRCODE_NOT_READY       - 系统未准备好
 *      XMEDIA_ERRCODE_NULL_PTR        - 空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM   - 输入参数非法
 *  注意：
 *      通道必须已创建
 */
xmedia_s32 xmedia_venc_set_h265_slice_split(xmedia_s32 venc_chn, const xmedia_venc_h265_slice_split *slice_split);

/*
 *  函数功能：获取h265 slice划分
 *  函数参数：
 *      venc_chn      - 通道号
 *      slice_split   - slice划分信息
 *  返回值：
 *      XMEDIA_SUCCESS                 - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_CHN_ID  - 无效的通道号
 *      XMEDIA_ERRCODE_NOT_EXIST       - 通道不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT     - 不支持的通道类型
 *      XMEDIA_ERRCODE_NOT_READY       - 系统未准备好
 *      XMEDIA_ERRCODE_NULL_PTR        - 空指针
 *  注意：
 *      通道必须已创建
 */
xmedia_s32 xmedia_venc_get_h265_slice_split(xmedia_s32 venc_chn, xmedia_venc_h265_slice_split *slice_split);

/*
 *  函数功能：设置h265 trans信息
 *  函数参数：
 *      venc_chn      - 通道号
 *      h265_trans    - trans信息
 *  返回值：
 *      XMEDIA_SUCCESS                 - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_CHN_ID  - 无效的通道号
 *      XMEDIA_ERRCODE_NOT_EXIST       - 通道不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT     - 不支持的通道类型
 *      XMEDIA_ERRCODE_NOT_READY       - 系统未准备好
 *      XMEDIA_ERRCODE_NULL_PTR        - 空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM   - 输入参数非法
 *  注意：
 *      通道必须已创建
 */
xmedia_s32 xmedia_venc_set_h265_trans(xmedia_s32 venc_chn, const xmedia_venc_h265_trans *h265_trans);

/*
 *  函数功能：获取h265 trans信息
 *  函数参数：
 *      venc_chn      - 通道号
 *      h265_trans    - trans信息
 *  返回值：
 *      XMEDIA_SUCCESS                 - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_CHN_ID  - 无效的通道号
 *      XMEDIA_ERRCODE_NOT_EXIST       - 通道不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT     - 不支持的通道类型
 *      XMEDIA_ERRCODE_NOT_READY       - 系统未准备好
 *      XMEDIA_ERRCODE_NULL_PTR        - 空指针
 *  注意：
 *      通道必须已创建
 */
xmedia_s32 xmedia_venc_get_h265_trans(xmedia_s32 venc_chn, xmedia_venc_h265_trans *h265_trans);

/*
 *  函数功能：设置h265 去块效应参数
 *  函数参数：
 *      venc_chn     - 通道号
 *      h265_dblk    - 去块参数
 *  返回值：
 *      XMEDIA_SUCCESS                 - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_CHN_ID  - 无效的通道号
 *      XMEDIA_ERRCODE_NOT_EXIST       - 通道不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT     - 不支持的通道类型
 *      XMEDIA_ERRCODE_NOT_READY       - 系统未准备好
 *      XMEDIA_ERRCODE_NULL_PTR        - 空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM   - 输入参数非法
 *  注意：
 *      通道必须已创建
 */
xmedia_s32 xmedia_venc_set_h265_dblk(xmedia_s32 venc_chn, const xmedia_venc_h265_dblk *h265_dblk);

/*
 *  函数功能：获取h265 去块效应参数
 *  函数参数：
 *      venc_chn     - 通道号
 *      h265_dblk    - 去块参数
 *  返回值：
 *      XMEDIA_SUCCESS                 - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_CHN_ID  - 无效的通道号
 *      XMEDIA_ERRCODE_NOT_EXIST       - 通道不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT     - 不支持的通道类型
 *      XMEDIA_ERRCODE_NOT_READY       - 系统未准备好
 *      XMEDIA_ERRCODE_NULL_PTR        - 空指针
 *  注意：
 *      通道必须已创建
 */
xmedia_s32 xmedia_venc_get_h265_dblk(xmedia_s32 venc_chn, xmedia_venc_h265_dblk *h265_dblk);

/*
 *  函数功能：设置h265 vui
 *  函数参数：
 *      venc_chn  - 通道号
 *      h265_vui  - vui参数集
 *  返回值：
 *      XMEDIA_SUCCESS                 - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_CHN_ID  - 无效的通道号
 *      XMEDIA_ERRCODE_NOT_EXIST       - 通道不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT     - 不支持的通道类型
 *      XMEDIA_ERRCODE_NOT_READY       - 系统未准备好
 *      XMEDIA_ERRCODE_NULL_PTR        - 空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM   - 输入参数非法
 *  注意：
 *      通道必须已创建
 */
xmedia_s32 xmedia_venc_set_h265_vui(xmedia_s32 venc_chn, const xmedia_venc_h265_vui *h265_vui);

/*
 *  函数功能：获取h265 vui
 *  函数参数：
 *      venc_chn  - 通道号
 *      h265_vui  - vui参数集
 *  返回值：
 *      XMEDIA_SUCCESS                 - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_CHN_ID  - 无效的通道号
 *      XMEDIA_ERRCODE_NOT_EXIST       - 通道不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT     - 不支持的通道类型
 *      XMEDIA_ERRCODE_NOT_READY       - 系统未准备好
 *      XMEDIA_ERRCODE_NULL_PTR        - 空指针
 *  注意：
 *      通道必须已创建
 */
xmedia_s32 xmedia_venc_get_h265_vui(xmedia_s32 venc_chn, xmedia_venc_h265_vui *h265_vui);

/*
 *  函数功能：设置丢帧策略
 *  函数参数：
 *      venc_chn        - 通道号
 *      frm_lost_param  - 丢帧策略
 *  返回值：
 *      XMEDIA_SUCCESS                 - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_CHN_ID  - 无效的通道号
 *      XMEDIA_ERRCODE_NOT_EXIST       - 通道不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT     - 不支持的通道类型或rc模式
 *      XMEDIA_ERRCODE_NOT_READY       - 系统未准备好
 *      XMEDIA_ERRCODE_NULL_PTR        - 空指针
 *      XMEDIA_ERRCODE_NOT_PERMITTED   - 不允许的操作
 *      XMEDIA_ERRCODE_INVALID_PARAM   - 输入参数非法
 *  注意：
 *      通道必须已创建
 */
xmedia_s32 xmedia_venc_set_frame_lost_strategy(xmedia_s32 venc_chn, const xmedia_venc_frame_lost *frm_lost_param);

/*
 *  函数功能：获取丢帧策略
 *  函数参数：
 *      venc_chn        - 通道号
 *      frm_lost_param  - 丢帧策略
 *  返回值：
 *      XMEDIA_SUCCESS                 - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_CHN_ID  - 无效的通道号
 *      XMEDIA_ERRCODE_NOT_EXIST       - 通道不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT     - 不支持的通道类型或rc模式
 *      XMEDIA_ERRCODE_NOT_READY       - 系统未准备好
 *      XMEDIA_ERRCODE_NULL_PTR        - 空指针
 *  注意：
 *      通道必须已创建
 */
xmedia_s32 xmedia_venc_get_frame_lost_strategy(xmedia_s32 venc_chn, xmedia_venc_frame_lost *frm_lost_param);

/*
 *  函数功能：设置超大帧策略
 *  函数参数：
 *      venc_chn        - 通道号
 *      super_frm_param - 超大帧策略
 *  返回值：
 *      XMEDIA_SUCCESS                 - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_CHN_ID  - 无效的通道号
 *      XMEDIA_ERRCODE_NOT_EXIST       - 通道不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT     - 不支持的通道类型或rc模式
 *      XMEDIA_ERRCODE_NOT_READY       - 系统未准备好
 *      XMEDIA_ERRCODE_NULL_PTR        - 空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM   - 输入参数非法
 *  注意：
 *      通道必须已创建
 */
xmedia_s32 xmedia_venc_set_super_frame_strategy(xmedia_s32 venc_chn, const xmedia_venc_super_frame_cfg *super_frm_param);

/*
 *  函数功能：获取超大帧策略
 *  函数参数：
 *      venc_chn        - 通道号
 *      super_frm_param - 超大帧策略
 *  返回值：
 *      XMEDIA_SUCCESS                 - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_CHN_ID  - 无效的通道号
 *      XMEDIA_ERRCODE_NOT_EXIST       - 通道不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT     - 不支持的通道类型
 *      XMEDIA_ERRCODE_NOT_READY       - 系统未准备好
 *  注意：
 *      通道必须已创建
 */
xmedia_s32 xmedia_venc_get_super_frame_strategy(xmedia_s32 venc_chn, xmedia_venc_super_frame_cfg *super_frm_param);

/*
 *  函数功能：设置venc 通道参数
 *  函数参数：
 *      venc_chn    - 通道号
 *      chn_param   - 通道参数集
 *  返回值：
 *      XMEDIA_SUCCESS                 - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_CHN_ID  - 无效的通道号
 *      XMEDIA_ERRCODE_NOT_EXIST       - 通道不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT     - 不支持的通道类型
 *      XMEDIA_ERRCODE_NOT_READY       - 系统未准备好
 *      XMEDIA_ERRCODE_NULL_PTR        - 空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM   - 输入参数非法
 *      XMEDIA_ERRCODE_TYPE_ERROR      - 通道类型错误
 *      XMEDIA_ERRCODE_NOT_PERMITTED   - multi模式不支持裁剪
 *  注意：
 *      通道必须已创建
 */
xmedia_s32 xmedia_venc_set_chn_param(xmedia_s32 venc_chn, const xmedia_venc_chn_param *chn_param);

/*
 *  函数功能：获取venc 通道参数
 *  函数参数：
 *      venc_chn    - 通道号
 *      chn_param   - 通道参数集
 *  返回值：
 *      XMEDIA_SUCCESS                 - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_CHN_ID  - 无效的通道号
 *      XMEDIA_ERRCODE_NOT_EXIST       - 通道不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT     - 不支持的通道类型
 *      XMEDIA_ERRCODE_NOT_READY       - 系统未准备好
 *      XMEDIA_ERRCODE_NULL_PTR        - 空指针
 *  注意：
 *      通道必须已创建
 */
xmedia_s32 xmedia_venc_get_chn_param(xmedia_s32 venc_chn, xmedia_venc_chn_param *chn_param);

/*
 *  函数功能：设置venc 通道模式(创建通道前)
 *  函数参数：
 *      venc_chn    - 通道号
 *      mod_param   - 通道模式
 *  返回值：
 *      XMEDIA_SUCCESS                 - 函数执行成功
 *      XMEDIA_ERRCODE_NULL_PTR        - 空指针
 *      XMEDIA_ERRCODE_NOT_READY       - 系统未准备好
 *      XMEDIA_ERRCODE_NOT_PERMITTED   - 不允许创建通道后配置模块属性
 *      XMEDIA_ERRCODE_INVALID_PARAM   - 输入参数非法
 *  注意：
 *      创建通道前调用才能生效
 */
xmedia_s32 xmedia_venc_set_mod_param(const xmedia_venc_param_mod *mod_param);

/*
 *  函数功能：获取venc 通道模式(创建通道前)
 *  函数参数：
 *      venc_chn    - 通道号
 *      mod_param   - 通道模式
 *  返回值：
 *      XMEDIA_SUCCESS                 - 函数执行成功
 *      XMEDIA_ERRCODE_NULL_PTR        - 空指针
 *      XMEDIA_ERRCODE_NOT_READY       - 系统未准备好
 *      XMEDIA_ERRCODE_INVALID_PARAM   - 输入参数非法
 */
xmedia_s32 xmedia_venc_get_mod_param(xmedia_venc_param_mod *mod_param);

/*
 *  函数功能：设置scene模式
 *  函数参数：
 *      venc_chn      - 通道号
 *      en_scene_mode - scene模式
 *  返回值：
 *      XMEDIA_SUCCESS                 - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_CHN_ID  - 无效的通道号
 *      XMEDIA_ERRCODE_NOT_EXIST       - 通道不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT     - 不支持的通道类型
 *      XMEDIA_ERRCODE_INVALID_PARAM   - 输入参数非法
 *      XMEDIA_ERRCODE_NULL_PTR        - 空指针
 *  注意：
 *      通道必须已创建
 */
xmedia_s32 xmedia_venc_set_scene_mode(xmedia_s32 venc_chn, const xmedia_venc_scene_mod scene_mode);

/*
 *  函数功能：获取scene模式
 *  函数参数：
 *      venc_chn      - 通道号
 *      en_scene_mode - scene模式
 *  返回值：
 *      XMEDIA_SUCCESS                 - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_CHN_ID  - 无效的通道号
 *      XMEDIA_ERRCODE_NOT_EXIST       - 通道不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT     - 不支持的通道类型
 *      XMEDIA_ERRCODE_NOT_READY       - 系统未准备好
 *      XMEDIA_ERRCODE_NULL_PTR        - 空指针
 *  注意：
 *      通道必须已创建
 */
xmedia_s32 xmedia_venc_get_scene_mode(xmedia_s32 venc_chn, xmedia_venc_scene_mod *scene_mode);

/*
 *  函数功能：attach vbpool
 *  函数参数：
 *      venc_chn    - 通道号
 *      pool        - vbpool信息
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_CHN_ID  - 无效的通道号
 *      XMEDIA_ERRCODE_NOT_EXIST       - 通道不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT     - 不支持的通道类型或VB模式
 *      XMEDIA_ERRCODE_NOT_READY       - 系统未准备好
 *      XMEDIA_ERRCODE_NULL_PTR        - 空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM   - 无效的vb pool
 *      XMEDIA_ERRCODE_NOT_PERMITTED   - 参考、重构帧共用一个BUF时不允许绑定
 *  注意：
 *      通道必须已创建
 */
xmedia_s32 xmedia_venc_attach_vbpool(xmedia_s32 venc_chn, const xmedia_venc_chn_pool *pool);

/*
 *  函数功能：attach vbpool
 *  函数参数：
 *      venc_chn    - 通道号
 *      pool        - vbpool信息
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_CHN_ID  - 无效的通道号
 *      XMEDIA_ERRCODE_NOT_EXIST       - 通道不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT     - 不支持的通道类型或VB模式
 *      XMEDIA_ERRCODE_NOT_READY       - 系统未准备好
 *      XMEDIA_ERRCODE_NOT_PERMITTED   - 参考、重构帧共用一个BUF时不允许解绑
 *  注意：
 *      通道必须已创建
 */
xmedia_s32 xmedia_venc_detach_vbpool(xmedia_s32 venc_chn);

/*
 *  函数功能：设置roi背景帧帧率
 *  函数参数：
 *      venc_chn           - 通道号
 *      roi_bgfrm_rate - roi帧率信息
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      其它错误码          - 函数执行失败
 */
xmedia_s32 xmedia_venc_set_roi_bgframe_rate(xmedia_s32 venc_chn, const xmedia_venc_roibg_frame_rate *roi_bgframe_rate);

/*
 *  函数功能：获取roi背景帧帧率
 *  函数参数：
 *      venc_chn           - 通道号
 *      roi_bgfrm_rate - roi帧率信息
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      其它错误码          - 函数执行失败
 */
xmedia_s32 xmedia_venc_get_roi_bgframe_rate(xmedia_s32 venc_chn, xmedia_venc_roibg_frame_rate *roi_bgframe_rate);

/*
 *  函数功能：设置h265 sao使能状态
 *  函数参数：
 *      venc_chn     - 通道号
 *      h265_dblk    - sao参数
 *  返回值：
 *      XMEDIA_SUCCESS                 - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_CHN_ID  - 无效的通道号
 *      XMEDIA_ERRCODE_NOT_EXIST       - 通道不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT     - 不支持的通道类型
 *      XMEDIA_ERRCODE_NOT_READY       - 系统未准备好
 *      XMEDIA_ERRCODE_NULL_PTR        - 空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM   - 输入参数非法
 *  注意：
 *      通道必须已创建
 */
xmedia_s32 xmedia_venc_set_h265_sao(xmedia_s32 venc_chn, const xmedia_venc_h265_sao *h265_sao);

/*
 *  函数功能：获取h265 sao使能状态
 *  函数参数：
 *      venc_chn     - 通道号
 *      h265_dblk    - sao参数
 *  返回值：
 *      XMEDIA_SUCCESS                 - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_CHN_ID  - 无效的通道号
 *      XMEDIA_ERRCODE_NOT_EXIST       - 通道不存在
 *      XMEDIA_ERRCODE_NOT_SUPPORT     - 不支持的通道类型
 *      XMEDIA_ERRCODE_NOT_READY       - 系统未准备好
 *      XMEDIA_ERRCODE_NULL_PTR        - 空指针
 *  注意：
 *      通道必须已创建
 */
xmedia_s32 xmedia_venc_get_h265_sao(xmedia_s32 venc_chn, xmedia_venc_h265_sao *h265_sao);

/*
 *  函数功能：设置h264 slice划分
 *  函数参数：
 *      venc_chn           - 通道号
 *      slice_split    - slice划分信息
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      其它错误码          - 函数执行失败
 */
xmedia_s32 xmedia_venc_set_h264_slice_split(xmedia_s32 venc_chn, const xmedia_venc_h264_slice_split *slice_split);

/*
 *  函数功能：获取h264 slice划分
 *  函数参数：
 *      venc_chn           - 通道号
 *      slice_split    - slice划分信息
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      其它错误码          - 函数执行失败
 */
xmedia_s32 xmedia_venc_get_h264_slice_split(xmedia_s32 venc_chn, xmedia_venc_h264_slice_split *slice_split);

/*
 *  函数功能：设置h264 trans信息
 *  函数参数：
 *      venc_chn           - 通道号
 *      h264_trans     - trance划分信息
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      其它错误码          - 函数执行失败
 */
xmedia_s32 xmedia_venc_set_h264_trans(xmedia_s32 venc_chn, const xmedia_venc_h264_trans *h264_trans);

/*
 *  函数功能：获取h264 trans信息
 *  函数参数：
 *      venc_chn           - 通道号
 *      h264_trans     - trance划分信息
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      其它错误码          - 函数执行失败
 */
xmedia_s32 xmedia_venc_get_h264_trans(xmedia_s32 venc_chn, xmedia_venc_h264_trans *h264_trans);

/*
 *  函数功能：设置h264去块滤波
 *  函数参数：
 *      venc_chn      - 通道号
 *      h264_dblk - 去块参数
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      其它错误码          - 函数执行失败
 */
xmedia_s32 xmedia_venc_set_h264_dblk(xmedia_s32 venc_chn, const xmedia_venc_h264_dblk *h264_dblk);

/*
 *  函数功能：获取h264去块滤波
 *  函数参数：
 *      venc_chn      - 通道号
 *      h264_dblk - 去块参数
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      其它错误码          - 函数执行失败
 */
xmedia_s32 xmedia_venc_get_h264_dblk(xmedia_s32 venc_chn, xmedia_venc_h264_dblk *h264_dblk);

/*
 *  函数功能：设置h264 vui
 *  函数参数：
 *      venc_chn      - 通道号
 *      h264_vui  - vui参数集
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      其它错误码          - 函数执行失败
 */
xmedia_s32 xmedia_venc_set_h264_vui(xmedia_s32 venc_chn, const xmedia_venc_h264_vui *h264_vui);

/*
 *  函数功能：获取h264 vui
 *  函数参数：
 *      venc_chn      - 通道号
 *      h264_vui  - vui参数集
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      其它错误码          - 函数执行失败
 */
xmedia_s32 xmedia_venc_get_h264_vui(xmedia_s32 venc_chn, xmedia_venc_h264_vui *h264_vui);

/*
 *  函数功能：设置参考帧信息
 *  函数参数：
 *      venc_chn        - 通道号
 *      ref_param   - 参考
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      其它错误码          - 函数执行失败
 */
xmedia_s32 xmedia_venc_set_ref_param(xmedia_s32 venc_chn, const xmedia_venc_ref_param *ref_param);
xmedia_s32 xmedia_venc_get_ref_param(xmedia_s32 venc_chn, xmedia_venc_ref_param *ref_param);

/*
 *  函数功能：使能IDR帧
 *  函数参数：
 *      venc_chn      - 通道号
 *      enable_idr    - idr使能
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      其它错误码          - 函数执行失败
 */
xmedia_s32 xmedia_venc_enable_idr(xmedia_s32 venc_chn, xmedia_bool enable_idr);

/*
 *  函数功能：intra 刷新设置
 *  函数参数：
 *      venc_chn            - 通道号
 *      intra_refresh   - intra信息
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      其它错误码          - 函数执行失败
 */
xmedia_s32 xmedia_venc_set_intra_refresh(xmedia_s32 venc_chn, const xmedia_venc_intra_refresh *intra_refresh);
xmedia_s32 xmedia_venc_get_intra_refresh(xmedia_s32 venc_chn, xmedia_venc_intra_refresh *intra_refresh);

/*
 *  函数功能：获取前景保护
 *  函数参数：
 *      venc_chn               - 通道号
 *      foreground_protect - 前景保护信息
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      其它错误码          - 函数执行失败
 */
xmedia_s32 xmedia_venc_get_foreground_protect(xmedia_s32 venc_chn, xmedia_venc_foreground_protect *foreground_protect);

/*
 *  函数功能：设置前景保护
 *  函数参数：
 *      venc_chn               - 通道号
 *      foreground_protect - 前景保护信息
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      其它错误码          - 函数执行失败
 */
xmedia_s32 xmedia_venc_set_foreground_protect(xmedia_s32 venc_chn, const xmedia_venc_foreground_protect *foreground_protect);

/*
 *  函数功能：设置intra和Intra之间的倾向性以及h.265的大小块选择的倾向性
 *  函数参数：
 *      venc_chn       - 通道号
 *      cur_prediction - 倾向性选择
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      其它错误码     - 函数执行失败
 */
xmedia_s32 xmedia_venc_set_cu_prediction(xmedia_s32 venc_chn, const xmedia_venc_cu_prediction *cu_prediction);

/*
 *  函数功能：获取intra和Intra之间的倾向性以及h.265的大小块选择的倾向性
 *  函数参数：
 *      venc_chn       - 通道号
 *      cur_prediction - 倾向性选择
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      其它错误码     - 函数执行失败
 */
xmedia_s32 xmedia_venc_get_cu_prediction(xmedia_s32 venc_chn, xmedia_venc_cu_prediction *cu_prediction);

/*
 *  函数功能：设置rc rdo_prefer参数
 *  函数参数：
 *      venc_chn       - 通道号
 *      rc_rdo_prefer - 倾向性选择
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      其它错误码     - 函数执行失败
 */
xmedia_s32 xmedia_venc_set_rc_rdo_prefer(xmedia_s32 venc_chn, const xmedia_venc_rc_rdo_prefer *rc_rdo_prefer);

/*
 *  函数功能：获取rc rdo_prefer参数
 *  函数参数：
 *      venc_chn       - 通道号
 *      rc_rdo_prefer - 倾向性选择
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      其它错误码     - 函数执行失败
 */
xmedia_s32 xmedia_venc_get_rc_rdo_prefer(xmedia_s32 venc_chn, xmedia_venc_rc_rdo_prefer *rc_rdo_prefer);

/*
 *  函数功能：设置skip的倾向性
 *  函数参数：
 *      venc_chn  - 通道号
 *      skip_bias - skip倾向性
 *	返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      其它错误码     - 函数执行失败
 */
xmedia_s32 xmedia_venc_set_skip_bias(xmedia_s32 venc_chn, const xmedia_venc_skip_bias *skip_bias);

/*
 *  函数功能：获取skip的倾向性
 *  函数参数：
 *      venc_chn  - 通道号
 *      skip_bias - skip倾向性
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      其它错误码     - 函数执行失败
 */
xmedia_s32 xmedia_venc_get_skip_bias(xmedia_s32 venc_chn, xmedia_venc_skip_bias *skip_bias);

/*
 *  函数功能：设置去呼吸效应参数
 *  函数参数：
 *      venc_chn        - 通道号
 *      debreath_effect - 去呼吸效应参数
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      其它错误码     - 函数执行失败
 */
xmedia_s32 xmedia_venc_set_debreath_effect(xmedia_s32 venc_chn, const xmedia_venc_debreath_effect *debreath_effect);

/*
 *  函数功能：获取去呼吸效应参数
 *  函数参数：
 *      venc_chn        - 通道号
 *      debreath_effect - 去呼吸效应参数
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      其它错误码     - 函数执行失败
 */
xmedia_s32 xmedia_venc_get_debreath_effect(xmedia_s32 venc_chn, xmedia_venc_debreath_effect *debreath_effect);

/*
 *  函数功能：设置qp等级
 *  函数参数：
 *      venc_chn        - 通道号
 *      hierarchical_qp - qp参数
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      其它错误码     - 函数执行失败
 */
xmedia_s32 xmedia_venc_set_hierarchical_qp(xmedia_s32 venc_chn, const xmedia_venc_hierarchical_qp *hierarchical_qp);
xmedia_s32 xmedia_venc_get_hierarchical_qp(xmedia_s32 venc_chn, xmedia_venc_hierarchical_qp *hierarchical_qp);

/*
 *  函数功能：设置rc adv参数
 *  函数参数：
 *      venc_chn      - 通道号
 *      rc_adv_param  - rc adv参数
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      其它错误码     - 函数执行失败
 */
xmedia_s32 xmedia_venc_set_rc_adv_param(xmedia_s32 venc_chn, const xmedia_venc_rc_adv_param *rc_adv_param);
xmedia_s32 xmedia_venc_get_rc_adv_param(xmedia_s32 venc_chn, xmedia_venc_rc_adv_param *rc_adv_param);

/*
 *  函数功能：使能SVC
 *  函数参数：
 *      venc_chn - 通道号
 *      enable   - SVC使能
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      其它错误码     - 函数执行失败
 */
xmedia_s32 xmedia_venc_enable_svc(xmedia_s32 venc_chn, xmedia_bool enable);

/*
 *  函数功能：设置SVC参数
 *  函数参数：
 *      venc_chn    - 通道号
 *      svc_param   - SVC参数集
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      其它错误码     - 函数执行失败
 */
xmedia_s32 xmedia_venc_set_svc_param(xmedia_s32 venc_chn, const xmedia_venc_svc_param *svc_param);
xmedia_s32 xmedia_venc_get_svc_param(xmedia_s32 venc_chn, xmedia_venc_svc_param *svc_param);

/*
 *  函数功能：设置SVC场景复杂度
 *  函数参数：
 *      venc_chn          - 通道号
 *      svc_detect_result - 复杂度参数
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      其它错误码     - 函数执行失败
 */
xmedia_s32 xmedia_venc_get_svc_scene_complexity(xmedia_s32 venc_chn, xmedia_venc_svc_detect_result *svc_detect_result);

/*
 *  函数功能：设置SVC区域
 *  函数参数：
 *      venc_chn   - 通道号
 *      svc_region - SVC区域
 *	返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      其它错误码     - 函数执行失败
 */
xmedia_s32 xmedia_venc_send_svc_region(xmedia_s32 venc_chn, xmedia_venc_svc_rect_info *svc_region);

/*
 *  函数功能：搜索窗设置
 *  函数参数：
 *      venc_chn      - 通道号
 *      search_window - 搜索窗信息
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      其它错误码     - 函数执行失败
 */
xmedia_s32 xmedia_venc_set_search_window(xmedia_s32 venc_chn, const xmedia_venc_search_window *search_window);
xmedia_s32 xmedia_venc_get_search_window(xmedia_s32 venc_chn, xmedia_venc_search_window *search_window);

/*
 *  函数功能：设置编码旋转角度
 *  函数参数：
 *      venc_chn - 通道号
 *      rotation - 旋转角度
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      其它错误码     - 函数执行失败
 */
xmedia_s32 xmedia_venc_set_rotation(xmedia_s32 venc_chn, xmedia_video_rotation rotation);

xmedia_s32 xmedia_venc_get_skip_param(xmedia_s32 venc_chn, xmedia_venc_skip_param *skip_param);
xmedia_s32 xmedia_venc_set_skip_param(xmedia_s32 venc_chn, xmedia_venc_skip_param *skip_param);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

