/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2016-2019. All rights reserved.
 * Description: hi_comm_vi_adapt.h
 * Author: Hisilicon multimedia software group
 * Create: 2016-09-19
 */

#ifndef __HI_COMM_VI_ADAPT_H__
#define __HI_COMM_VI_ADAPT_H__

#include "hi_common_adapt.h"
#include "hi_errno_adapt.h"
#include "hi_comm_video_adapt.h"
#include "hi_comm_gdc.h"
#include "hi_comm_vi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define VI_MAX_ADCHN_NUM              4UL

#define VI_PMFCOEF_NUM                9UL
#define VI_COMPMASK_NUM               2UL
#define VI_PRO_MAX_FRAME_NUM          8UL

#define VI_INVALID_FRMRATE            (-1)
#define VI_CHN0                       0
#define VI_CHN1                       1
#define VI_CHN2                       2
#define VI_CHN3                       3
#define VI_INVALID_CHN                (-1)

#define VI_MAX_VC_NUM                 4

typedef struct {
    hi_bool enable;          /* RW; Low delay enable. */
    hi_u32 line_cnt;        /* RW; Range: [32, 16384]; Low delay shoreline. */
} hi_vi_low_delay_info;

/* Information of raw data cmpresss param */
typedef struct {
    hi_u8 cmp_param[VI_CMP_PARAM_SIZE];
} hi_vi_cmp_param;

typedef VI_USERPIC_MODE_E hi_vi_user_pic_mode;

typedef struct {
    hi_u32 bg_color;
} hi_vi_user_pic_bgc;

typedef struct {
    hi_vi_user_pic_mode       usr_pic_mode;  /* User picture mode */
    union {
        hi_video_frame_info   usr_pic_frm;   /* Information about a YUV picture */
        hi_vi_user_pic_bgc    usr_pic_bg;    /* Information about a background picture only with a color */
    } usr_pic;
} hi_vi_user_pic_attr;

typedef EN_VI_ERR_CODE_E hi_vi_err_code;

#define HI_ERR_VI_INVALID_PARA        HI_DEF_ERR(HI_ID_VI, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
#define HI_ERR_VI_INVALID_DEVID       HI_DEF_ERR(HI_ID_VI, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_DEVID)
#define HI_ERR_VI_INVALID_PIPEID      HI_DEF_ERR(HI_ID_VI, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_PIPEID)
#define HI_ERR_VI_INVALID_STITCHGRPID HI_DEF_ERR(HI_ID_VI, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_STITCHGRPID)
#define HI_ERR_VI_INVALID_CHNID       HI_DEF_ERR(HI_ID_VI, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_CHNID)
#define HI_ERR_VI_INVALID_NULL_PTR    HI_DEF_ERR(HI_ID_VI, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
#define HI_ERR_VI_FAILED_NOTCONFIG    HI_DEF_ERR(HI_ID_VI, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_CONFIG)
#define HI_ERR_VI_SYS_NOTREADY        HI_DEF_ERR(HI_ID_VI, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)
#define HI_ERR_VI_BUF_EMPTY           HI_DEF_ERR(HI_ID_VI, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_EMPTY)
#define HI_ERR_VI_BUF_FULL            HI_DEF_ERR(HI_ID_VI, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_FULL)
#define HI_ERR_VI_NOMEM               HI_DEF_ERR(HI_ID_VI, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)
#define HI_ERR_VI_NOT_SUPPORT         HI_DEF_ERR(HI_ID_VI, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)
#define HI_ERR_VI_BUSY                HI_DEF_ERR(HI_ID_VI, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)
#define HI_ERR_VI_NOT_PERM            HI_DEF_ERR(HI_ID_VI, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)

#define HI_ERR_VI_FAILED_NOTENABLE    HI_DEF_ERR(HI_ID_VI, EN_ERR_LEVEL_ERROR, ERR_VI_FAILED_NOTENABLE)
#define HI_ERR_VI_FAILED_NOTDISABLE   HI_DEF_ERR(HI_ID_VI, EN_ERR_LEVEL_ERROR, ERR_VI_FAILED_NOTDISABLE)
#define HI_ERR_VI_FAILED_CHNOTDISABLE HI_DEF_ERR(HI_ID_VI, EN_ERR_LEVEL_ERROR, ERR_VI_FAILED_CHNOTDISABLE)
#define HI_ERR_VI_CFG_TIMEOUT         HI_DEF_ERR(HI_ID_VI, EN_ERR_LEVEL_ERROR, ERR_VI_CFG_TIMEOUT)
#define HI_ERR_VI_NORM_UNMATCH        HI_DEF_ERR(HI_ID_VI, EN_ERR_LEVEL_ERROR, ERR_VI_NORM_UNMATCH)
#define HI_ERR_VI_INVALID_WAYID       HI_DEF_ERR(HI_ID_VI, EN_ERR_LEVEL_ERROR, ERR_VI_INVALID_WAYID)
#define HI_ERR_VI_INVALID_PHYCHNID    HI_DEF_ERR(HI_ID_VI, EN_ERR_LEVEL_ERROR, ERR_VI_INVALID_PHYCHNID)
#define HI_ERR_VI_FAILED_NOTBIND      HI_DEF_ERR(HI_ID_VI, EN_ERR_LEVEL_ERROR, ERR_VI_FAILED_NOTBIND)
#define HI_ERR_VI_FAILED_BINDED       HI_DEF_ERR(HI_ID_VI, EN_ERR_LEVEL_ERROR, ERR_VI_FAILED_BINDED)

#define HI_ERR_VI_PIPE_EXIST          HI_DEF_ERR(HI_ID_VI, EN_ERR_LEVEL_ERROR, EN_ERR_EXIST)
#define HI_ERR_VI_PIPE_UNEXIST        HI_DEF_ERR(HI_ID_VI, EN_ERR_LEVEL_ERROR, EN_ERR_UNEXIST)

/* interface mode of video input */
typedef VI_INTF_MODE_E hi_vi_intf_mode;

/* Input mode */
typedef VI_INPUT_MODE_E hi_vi_input_mode;

/* Work mode */
typedef VI_WORK_MODE_E hi_vi_work_mode;

/* whether an input picture is interlaced or progressive */
typedef VI_SCAN_MODE_E hi_vi_scan_mode;

/* Sequence of YUV data */
typedef VI_YUV_DATA_SEQ_E hi_vi_yuv_data_seq;

/* Clock edge mode */
typedef VI_CLK_EDGE_E hi_vi_clk_edge;

/* Component mode */
typedef VI_COMPONENT_MODE_E hi_vi_component_mode;

/* Y/C composite or separation mode */
typedef VI_COMBINE_MODE_E hi_vi_combine_mode;

/* Attribute of the vertical synchronization signal */
typedef VI_VSYNC_E hi_vi_vsync;

/* Polarity of the vertical synchronization signal */
typedef VI_VSYNC_NEG_E hi_vi_vsync_neg;

/* Attribute of the horizontal synchronization signal */
typedef VI_HSYNC_E hi_vi_hsync;

/* Polarity of the horizontal synchronization signal */
typedef VI_HSYNC_NEG_E hi_vi_hsync_neg;

/* Attribute of the valid vertical synchronization signal */
typedef VI_VSYNC_VALID_E hi_vi_vsync_valid;

/* Polarity of the valid vertical synchronization signal */
typedef VI_VSYNC_VALID_NEG_E hi_vi_vsync_valid_neg;

/* Blank information of the input timing */
typedef struct {
    hi_u32 hsync_hfb ;    /* RW;Horizontal front blanking width */
    hi_u32 hsync_act ;    /* RW;Horizontal effetive width */
    hi_u32 hsync_hbb ;    /* RW;Horizontal back blanking width */
    hi_u32 vsync_vfb ;    /* RW;Vertical front blanking height of one frame or odd-field frame picture */
    hi_u32 vsync_vact ;   /* RW;Vertical effetive width of one frame or odd-field frame picture */
    hi_u32 vsync_vbb ;    /* RW;Vertical back blanking height of one frame or odd-field frame picture */
    hi_u32 vsync_vbfb ;   /* RW;Even-field vertical front blanking height when input mode is interlace
                            (invalid when progressive input mode) */
    hi_u32 vsync_vbact ;  /* RW;Even-field vertical effetive width when input mode is interlace
                            (invalid when progressive input mode) */
    hi_u32 vsync_vbbb ;   /* RW;Even-field vertical back blanking height when input mode is interlace
                            (invalid when progressive input mode) */
} hi_vi_timing_blank;

/* synchronization information about the BT.601 or DC timing */
typedef struct {
    hi_vi_vsync              vsync;
    hi_vi_vsync_neg          vsync_neg;
    hi_vi_hsync              hsync;
    hi_vi_hsync_neg          hsync_neg;
    hi_vi_vsync_valid        vsync_valid;
    hi_vi_vsync_valid_neg    vsync_valid_neg;
    hi_vi_timing_blank       timing_blank;
} hi_vi_sync_cfg;

/* the highest bit of the BT.656 timing reference code */
typedef VI_BT656_FIXCODE_E hi_vi_bt656_fixcode;

/* Polarity of the field indicator bit (F) of the BT.656 timing reference code */
typedef VI_BT656_FIELD_POLAR_E hi_vi_bt656_field_polar;

/* synchronization information about the BT.656 */
typedef struct {
    hi_vi_bt656_fixcode     fix_code;
    hi_vi_bt656_field_polar field_polar;
} hi_vi_bt656_sync_cfg;

/* Input data type */
typedef VI_DATA_TYPE_E hi_vi_data_type;

typedef VI_REPHASE_MODE_E hi_vi_rephase_mode;

typedef struct {
    hi_vi_rephase_mode h_rephase_mode;
    hi_vi_rephase_mode v_rephase_mode;
} hi_vi_bas_rephase_attr;

/* Attribute of bas scale */
typedef struct {
    hi_size bas_size; /* RW;bayer scale size. */
} hi_vi_bas_scale_attr;

/* Attribute of bayer scale */
typedef struct {
    hi_vi_bas_scale_attr     sacle_attr;
    hi_vi_bas_rephase_attr   rephase_attr;
} hi_vi_bas_attr;

/* Attribute of wdr */
typedef struct {
    hi_wdr_mode  wdr_mode;        /* RW; WDR mode. */
    hi_u32      cache_line;       /* RW; WDR cache line. */
} hi_vi_wdr_attr;

/* the extended attributes of VI device */
typedef struct {
    hi_vi_input_mode         input_mode;                    /* RW;Input mode */
    hi_vi_work_mode          work_mode;                     /* RW;Work mode */

    hi_vi_combine_mode       combine_mode;                  /* RW;Y/C composite or separation mode */
    hi_vi_component_mode     component_mode;                /* RW;Component mode (single-component or dual-component) */
    hi_vi_clk_edge           clk_edge;                      /* RW;Clock edge mode (sampling on the rising or
                                                            falling edge) */

    hi_u32                   component_mask[VI_COMPMASK_NUM]; /* RW;Component mask */

    hi_vi_scan_mode          scan_mode;                     /* RW;Input scanning mode (progressive or interlaced) */
    hi_s32                   ad_chn_id[VI_MAX_ADCHN_NUM];  /* RW;AD channel ID. Typically, the default
                                                            value -1 is recommended */

    hi_vi_yuv_data_seq       data_seq;                  /* RW;Input data sequence (only the YUV format is supported) */
    hi_vi_sync_cfg           syn_cfg;                       /* RW;Sync timing. This member must be configured in BT.
                                                            601 mode or DC mode */

    hi_vi_bt656_sync_cfg     bt656_syn_cfg;                  /* RW;Sync timing. This member must be configured in BT.
                                                            656 mode */

    hi_vi_data_type          input_data_type;                /* RW;RGB: CSC-709 or CSC-601, PT YUV444 disable;
                                                            YUV: default yuv CSC coef PT YUV444 enable. */

    hi_bool                  data_reverse;                   /* RW;Data reverse */

    hi_size                  size;                         /* RW;Input size */

    hi_vi_bas_attr           bas_attr;                      /* RW;Attribute of BAS */

    hi_vi_wdr_attr           wdr_attr;                      /* RW;Attribute of WDR */

    hi_data_rate             data_rate;                     /* RW;Data rate of device */
} hi_vi_dev_attr_ex;

/* The attributes of a VI device */
typedef struct {
    hi_vi_intf_mode      intf_mode;                     /* RW;Interface mode */
    hi_vi_work_mode      work_mode;                     /* RW;Work mode */

    hi_u32               component_mask[VI_COMPMASK_NUM];  /* RW;Component mask */
    hi_vi_scan_mode      scan_mode;                     /* RW;Input scanning mode (progressive or interlaced) */
    hi_s32               ad_chn_id[VI_MAX_ADCHN_NUM];  /* RW;AD channel ID. Typically, the default value -1
                                                        is recommended */

    /* The below members must be configured in BT.601 mode or DC mode and are invalid in other modes */
    hi_vi_yuv_data_seq   data_seq;                      /* RW;Input data sequence (only the YUV format is supported) */
    hi_vi_sync_cfg       syn_cfg;                       /* RW;Sync timing. This member must be configured in BT.
                                                        601 mode or DC mode */

    hi_vi_data_type      input_data_type;                /* RW;RGB: CSC-709 or CSC-601, PT YUV444 disable; YUV: default
                                                        yuv CSC coef PT YUV444 enable. */

    hi_bool              data_reverse;                   /* RW;Data reverse */

    hi_size              size;                         /* RW;Input size */

    hi_vi_bas_attr       bas_attr;                      /* RW;Attribute of BAS */

    hi_vi_wdr_attr       wdr_attr;                      /* RW;Attribute of WDR */

    hi_data_rate         data_rate;                     /* RW;Data rate of device */
} hi_vi_dev_attr;

/* Information of pipe binded to device */
typedef struct {
    hi_u32  num;                                     /* RW;Range [1,VI_MAX_PHY_PIPE_NUM] */
    hi_vi_pipe pipe_id[VI_MAX_PHY_PIPE_NUM];                /* RW;Array of pipe ID */
} hi_vi_dev_bind_pipe;

/* Source of 3DNR reference frame */
typedef VI_NR_REF_SOURCE_E hi_vi_nr_ref_source;

typedef VI_PIPE_BYPASS_MODE_E hi_vi_pipe_bypass_mode;

/* The attributes of 3DNR */
typedef struct {
    hi_pixel_format      pix_fmt;                       /* RW;Pixel format of reference frame */
    hi_data_bitwidth     bit_width;                     /* RW;Bit width of reference frame */
    hi_vi_nr_ref_source  nr_ref_source;                  /* RW;Source of 3DNR reference frame */
    hi_compress_mode     compress_mode;                 /* RW;Reference frame compress mode */
} hi_vi_nr_attr;

/* The attributes of pipe */
typedef struct {
    hi_vi_pipe_bypass_mode pipe_bypass_mode;
    hi_bool               yuv_skip;               /* RW;YUV skip enable */
    hi_bool               isp_bypass;             /* RW;Range:[0, 1];ISP bypass enable */
    hi_u32                max_w;                /* RW;Range:[0, 1];Range[VI_PIPE_MIN_WIDTH, VI_PIPE_MAX_WIDTH];
                                                Maximum width */
    hi_u32                max_h;                /* RW;Range[VI_PIPE_MIN_HEIGHT, VI_PIPE_MAX_HEIGHT];Maximum height */
    hi_pixel_format       pix_fmt;               /* RW;Pixel format */
    hi_compress_mode      compress_mode;         /* RW;Range:[0, 4];Compress mode. */
    hi_data_bitwidth      bit_width;             /* RW;Range:[0, 4];Bit width */
    hi_bool               nr_en;                  /* RW;Range:[0, 1];3DNR enable */
    hi_vi_nr_attr         nr_attr;               /* RW;Attribute of 3DNR */
    hi_bool               sharpen_en;             /* RW;Range:[0, 1];Sharpen enable */
    hi_frame_rate_ctrl    frame_rate;            /* RW;Frame rate */
    hi_bool               discard_pro_pic;         /* RW;Range:[0, 1];when professional mode snap, whether to discard
                                                long exposure picture in the video pipe. */
} hi_vi_pipe_attr;

typedef VI_STITCH_ISP_CFG_MODE_E hi_vi_stitch_isp_cfg_mode;

/* Information of stitch group */
typedef struct {
    hi_bool                     stitch;
    hi_vi_stitch_isp_cfg_mode   mode;
    hi_u32                      max_pts_gap;            /* RW;MAX PTS gap between frame of pipe,unit:us */
    hi_u32                      pipe_num;              /* RW;Range [2, VI_MAX_PIPE_NUM] */
    hi_vi_pipe                     pipe_id[VI_MAX_PIPE_NUM]; /* RW;Array of pipe ID */
} hi_vi_stitch_grp_attr;

typedef VI_PIPE_REPEAT_MODE_E hi_vi_pipe_repeat_mode;

typedef struct {
    hi_u8   ies;              /* RW; range:[0, 255];format 8.0;the absolute strength of image enhancement for edge */
    hi_u8   iess;             /* RW; range:[0, 255];format 8.0;the absolute strength of image enhancement for
                            texture and shadow */
    hi_u16  iedz;             /* RW; range:[0, 8192];format 14.0;the threshold of image enhancement
                            for controlling noise */
} hi_v59a_iey;

typedef struct {
    hi_u8   sbf     : 2;   /* RW; range:[0, 3];format 2.0;the band type of spatial filter, notice: SBF0, SBF1
                                  range is [2, 3];SBF2,SBF3,SBF4 range is [0,3], where SBF4 is related to sb_fk */
    hi_u8   str     : 4;   /* RW; range:[0, 13];format 4.0;the relative strength of spatial filter refer to
                                  the previous frame */
    hi_u8   sth_p   : 2;   /* RW; range:[0, 2];format 2.0;not recommended for debugging */
    hi_u8   sft     : 5;   /* RW; range:[0, 31];format 5.0;not recommended for debugging */
    hi_u8   k_pro   : 3;   /* RW; range:[0, 7];format 3.0;notice: the k_pro of s_fy2 range is [0, 7], the k_pro of
                                  s_fy3 range is [0, 4] */

    hi_u16  sth[3];        /* RW; range:[0, 999];3 para, format 10.0;the edge-preserve threshold for spatial filter */
    hi_u16  sbs[3];        /* RW; range:[0, 9999];3 para, format 14.0;the noise reduction strength of spatial filter
                                  for the relative bright pixel */
    hi_u16  sds[3];        /* RW; range:[0, 9999];3 para, format 14.0; the relative dark pixel */
} hi_v59a_sfy;

typedef struct {
    hi_u16  math    : 10;    /* RW; range:[0, 1023];format 10.0;the motion detection threshold for temporal filter */
    hi_u16  mate    : 4;     /* RW; range:[0, 11];format 4.0;the motion detection index of flat area
                            for temporal filter */
    hi_u16  matw    : 2;     /* RW; range:[0, 3];format 2.0;the index of suppressing trailing for temporal filter */
    hi_u8   masw    : 4;     /* RW; range:[0, 12];format 4.0;the index of suppressing raindrop noise
                            for temporal filter */
    hi_u8   mabw    : 3;     /* RW; range:[0, 4];format 3.0;the window of motion detection for temporal filter */
    hi_u8   maxn    : 1;     /* RW; range:[0, 1];format 1.0;not recommended for debugging */
} hi_v59a_mdy;

typedef struct {
    hi_u8   tfr[4];     /* RW; range:[0, 255];4 para;the relative strength of temporal filter for the static area */
    hi_u16  tdz : 14;   /* RW; range:[0, 999];format 14.0;the threshold of dead-area of temporal filter */
    hi_u16  tdx : 2;    /* RW; range:[0, 2];format 2.0;not recommended for debugging */
    hi_u16  tfs : 6;    /* RW; range:[0, 63];format 6.0;the absolute strength of temporal filter  */
} hi_v59a_tfy;

typedef struct {
    hi_u16  sfc      : 10;   /* RW; range:[0, 999];format 10.0;the strength of spatial filter for NRC0 */
    hi_u16  tfc      : 6;    /* RW; range:[0, 63];format 6.0;the strength of temporal filter for NRC0 */
    hi_u16  c_sfs    : 14;   /* RW; range:[0, 999];format 10.0;the strength of spatial filter for NRC1 */
    hi_u16  c_sfk    : 2;    /* RW; range:[0, 3];format 2.0;not recommended for debugging */
    hi_u16  c_tfs    : 4;    /* RW; range:[0, 15];format 4.0;the strength of temporal filter for NRC1 */
    hi_u16  c_iir    : 1;    /* RW; range:[0, 1];format 1.0;the mode of spatial filter for NRC1 */
    hi_u16  c_tfr    : 11;   /* RW; range:[0, 999];format 10.0;the relative strength of temporal filter for NRC1 */
    hi_u8   mode     : 1;    /* RW; range: [0,  1]; The switch for new chroma denoise mode. */
    hi_u8   presfc   : 6;    /* RW; range: [0,  32]; The strength for chroma pre spatial filter. */
    hi_u8   reserved : 1;
} hi_v59a_nr_c;

/* 3DNR spatial filter: s_fy0,s_fy1,s_fy2,s_fy3; temporal filter:t_fy0,t_fy1;chroma noise reduction: NRC0,NRC1 */
typedef struct {
    hi_v59a_iey  iey;
    hi_v59a_sfy  sfy[5];    /* sfy[5] */
    hi_v59a_mdy  mdy[2];    /* mdy[2] */
    hi_v59a_tfy  tfy[2];    /* tfy[2] */

    hi_u16  hdg_type : 1;   /* RW; range:[0, 1];format 1.0;the type of complexed mixed spatial filter whether
                            is s_fi or s_fk */
    hi_u16  bri_type : 1;   /* RW; range:[0, 1];format 1.0;the mode decide s_fy3 whether is s_fk type or s_fi type */
    hi_u16  hdg_mode : 2;   /* RW; range:[0, 3];format 2.0;the mode decide complexed mixed spatial filter band
                            for flat area */
    hi_u16  k_tab2   : 1;   /* RW; range:[0, 1];format 1.0;the parameter decide s_fy2 whether or not based on the image
                            absolute luminance */
    hi_u16  hdg_wnd  : 1;   /* RW; range:[0, 1];format 1.0;the sampling window of complexed mixed spatial filter for
                            noise detection */
    hi_u16  k_tab3   : 1;   /* RW; range:[0, 1];format 1.0;the parameter decide s_fy3 whether or not based on the image
                            absolute luminance */
    hi_u16  hdg_sfr  : 4;   /* RW; range:[0, 13];format 4.0;the trend of the noise reduction of complexed mixed spatial
                            filter for flat area */
    hi_u16  n_out    : 5;   /* RW; range:[0, 27];format 5.0;the parameter for output intermediate result of s_fy3 */
    hi_u8   hdg_ies;        /* RW; range:[0, 255];format 8.0;the strength of image enhancement for complexed
                            mixed spatial filter */
    hi_u8   n_ref    : 1;   /* RW; range:[0, 1];format 1.0;not recommended for debugging */
    hi_u8   iey_mode : 1;   /* RW; range:[0, 1];format 1.0;the image enhancement mode selection. */
    hi_u8   iey_ex[4];      /* RW; range:[0, 255];4para;the image enhancement strength for different frequency */
    hi_u8   sfr_i[4];       /* RW; range:[0, 255];4para, the relative strength of s_fy3 when the filter type is s_fi */
    hi_u8   sfr_k[4];       /* RW; range:[0, 255];format 8.0;
                               the relative strength of s_fy3 when the filter type is s_fk */
    hi_u16  sbs_k2[32];     /* RW; range:[0, 9999];format 14.0;the noise reduction strength of s_fy2 for the
                            relative bright pixel based on the image absolute luminance */
    hi_u16  sbs_k3[32];     /* RW; range:[0, 9999];format 14.0;the noise reduction strength of s_fy3 for the
                            relative bright pixel based on the image absolute luminance */
    hi_u16  sds_k2[32];     /* RW; range:[0, 9999];format 14.0;the noise reduction strength of s_fy2 for the
                            relative dark pixel based on the image absolute luminance */
    hi_u16  sds_k3[32];     /* RW; range:[0, 9999];format 14.0;the noise reduction strength of s_fy3 for the
                            relative dark pixel based on the image absolute luminance */
    hi_u16  bri_thr[16];    /* RW; range:[0, 1024];format 11.0;the threshold decide s_fy3 choose the s_fi type filter
                            or s_fk type filter in dark and bright area */
    hi_v59a_nr_c nr_c;
} hi_vi_pipe_nrx_param_v1;

typedef VI_NR_VERSION_E hi_vi_nr_version;

typedef struct {
    hi_vi_pipe_nrx_param_v1 nrx_param_v1;
} hi_nrx_param_manual_v1;

typedef struct {
    hi_u32                               param_num;
    hi_u32                   ATTRIBUTE   *iso;
    hi_vi_pipe_nrx_param_v1  ATTRIBUTE   *nrx_param_v1;
} hi_nrx_param_auto_v1;

typedef struct {
    hi_operation_mode        opt_mode;            /* RW;adaptive NR */
    hi_nrx_param_manual_v1   nrx_manual_v1;       /* RW;NRX V1 param for manual */
    hi_nrx_param_auto_v1     nrx_auto_v1;         /* RW;NRX V1 param for auto */
} hi_nrx_param_v1;

typedef struct {
    hi_u8  ies0, ies1, ies2, ies3;  /* ies0~4 ; range: [0, 255]; the gains of edge and texture enhancement.
                                     * 0~3 for different frequency response.
									 */
    hi_u16 iedz : 10, reserved : 6; /* iedz   ; range: [0, 999]; the threshold to control the generated artifacts. */
} hi_v500_vi_iey;

typedef struct {
    hi_u8  spn6 : 3, sfr  : 5;      /* SPN6; range: [0, 5];  the selection of filters to be mixed for NO.6 filter. */
                                    /* SFR ; range: [0, 31];  the relative NR strength in the s_fi and s_fk filter. */
    hi_u8  sbn6 : 3, pbr6 : 5;      /* SBN6; range: [0, 5];  the selection of filters to be mixed for NO.6 filter. */
                                    /* PBR6; range: [0, 16];  the mix ratio between SPN6 and SBN6. */
    hi_u16 srt0 : 5, srt1 : 5, j_mode : 3, de_idx : 3;    /* JMODE;      range: [0, 4]; the selection modes
                                                         for the blending of spatial filters */
                                                        /* srt0, srt1; range: [0, 16]; the blending ratio
                                                         of different filters. (used in serial filtering mode (SFM).) */
                                                        /* de_idx;      range: [3, 6]; the selection number of
                                                         filters that textures and details will be added to. */
    hi_u8  de_rate, sfr6[3];                             /* de_rate;     range: [0, 255]; the enhancement strength
                                                         for the SFM (when de_rate > 0, the SFM will be activated) */
                                                        /* SFR6;       range: [0, 31]; the relative NR strength
                                                         for NO.6 filter. (effective when JMODE = 4) */
    hi_u8  sfs1, sft1, sbr1;                            /* SFS1, SFT1, SBR1; range: [0, 255];  the NR strength
                                                         parameters for NO.1 filter. */
    hi_u8  sfs2, sft2, sbr2;                            /* SFS2, SFT2, SBR2; range: [0, 255];  the NR strength
                                                         parameters for NO.2 filter. */
    hi_u8  sfs4, sft4, sbr4;                            /* SFS4, SFT4, SBR4; range: [0, 255];  the NR strength
                                                         parameters for NO.3 and NO.4 filters. */

    hi_u16 sth1 : 9, sfn1 : 3, nr_y_en : 1, sfn0  : 3;    /* STH1~3; range: [0, 511]; the thresholds for protection
                                                         of edges from blurring */
                                                        /* n_ry_en ; range: [0, 1]; the NR switches */
    hi_u16 sth2 : 9, sfn2 : 3, bw_sf4 : 1, k_mode : 3;    /* SFN0~3; range: [0, 6]; filter selection for different
                                                         image areas based on STH1~3. */
                                                        /* BWSF4 ; range: [0, 1]; the NR window size for the
                                                         NO.3 and NO.4 filters.  */
    hi_u16 sth3 : 9, sfn3 : 3, tri_th : 1, reserved_0 : 3;    /* k_mode ; range: [0, 3]; the denoise mode based
                                                         on image brightness. */
                                                        /* trith ; range: [0, 1]; the switch to choose 3 STH
                                                         threshold or 2 STH threshold */
} hi_v500_vi_sfy;

typedef struct {
    hi_v500_vi_iey iey;
    hi_v500_vi_sfy sfy;
} hi_vi_pipe_nrx_param_v2;

typedef struct {
    hi_vi_pipe_nrx_param_v2 nrx_param_v2;
} hi_nrx_param_manual_v2;

typedef struct {
    hi_u32                               param_num;
    hi_u32                   ATTRIBUTE   *iso;
    hi_vi_pipe_nrx_param_v2  ATTRIBUTE   *nrx_param_v2;
} hi_nrx_param_auto_v2;

typedef struct {
    hi_operation_mode        opt_mode;            /* RW;adaptive NR */
    hi_nrx_param_manual_v2   nrx_manual_v2;       /* RW;NRX V2 param for manual */
    hi_nrx_param_auto_v2     nrx_auto_v2;         /* RW;NRX V2 param for auto */
} hi_nrx_param_v2;

typedef struct {
    hi_vi_nr_version nr_version; /* RW;3DNR version */
    union {
        hi_nrx_param_v1 nrx_param_v1; /* RW;3DNR X param version 1 */
        hi_nrx_param_v2 nrx_param_v2; /* RW;3DNR X param version 2 */
    };
} hi_vi_pipe_nrx_param;

/* the attributes of channel */
typedef struct {
    hi_size              size;             /* RW;channel out put size */
    hi_pixel_format      pixel_format;     /* RW;pixel format */
    hi_dynamic_range     dynamic_range;    /* RW;dynamic range */
    hi_video_format      video_format;     /* RW;video format */
    hi_compress_mode     compress_mode;    /* RW;256B segment compress or no compress. */
    hi_bool              mirror;           /* RW;mirror enable */
    hi_bool              flip;             /* RW;flip enable */
    hi_u32               depth;            /* RW;range [0,8];depth */
    hi_frame_rate_ctrl   frame_rate;       /* RW;frame rate */
} hi_vi_chn_attr;

/* the status of pipe */
typedef struct {
    hi_bool enable;                       /* RO;whether this pipe is enabled */
    hi_u32  int_cnt;                      /* RO;the video frame interrupt count */
    hi_u32  frame_rate;                   /* RO;current frame rate */
    hi_u32  lost_frame;                   /* RO;lost frame count */
    hi_u32  vb_fail;                      /* RO;video buffer malloc failure */
    hi_size size;                         /* RO;current pipe output size */
} hi_vi_pipe_status;

/* VS signal output mode */
typedef VI_VS_SIGNAL_MODE_E hi_vi_vs_signal_mode;

/* the attributes of VS signal */
typedef struct {
    hi_vi_vs_signal_mode mode;          /* RW;output one time, output frequently */
    hi_u32              start_time;     /* RW;output start time,unit: sensor pix clk. */
    hi_u32              duration;       /* RW;output high duration, unit: sensor pix clk. */
    hi_u32              cap_frm_index;  /* RW;VS signal will be output after trigger by which vframe,
                                         * default is 0.
                                         */
    hi_u32              interval;       /* RW;output frequently interval, unit: frame */
} hi_vi_vs_signal_attr;

typedef struct {
    hi_bool enable;
    hi_u32 depth;
} hi_bnr_dump_attr;

typedef VI_EXT_CHN_SOURCE_E hi_vi_ext_chn_source;

typedef struct {
    hi_vi_ext_chn_source source;
    hi_vi_chn           bind_chn;      /* RW;range [VI_CHN0, VI_MAX_PHY_CHN_NUM);the channel num which extend
                                        * channel will bind to
                                        */
    hi_size             size;          /* RW;channel out put size */
    hi_pixel_format     pix_format;    /* RW;pixel format */
    hi_dynamic_range    dynamic_range; /* RW;dynamic range */
    hi_compress_mode    compress_mode; /* RW;256B segment compress or no compress. */
    hi_u32              depth;         /* RW;range [0,8];depth */
    hi_frame_rate_ctrl  frame_rate;    /* RW;frame rate */
} hi_vi_ext_chn_attr;

typedef VI_CROP_COORDINATE_E hi_vi_crop_coordinate;

/* information of chn crop */
typedef struct {
    hi_bool                 enable;            /* RW;CROP enable */
    hi_vi_crop_coordinate   crop_coordinate;   /* RW;coordinate mode of the crop start point */
    hi_rect                 crop_rect;         /* RW;CROP rectangular */
} hi_vi_crop_info;

/* the attributes of LDC */
typedef struct {
    hi_bool enable; /* RW;range [0,1];whether LDC is enbale */
    hi_ldc_attr attr;
} hi_vi_ldc_attr;

/* the attributes of LDCV2 */
typedef struct {
    hi_bool enable; /* RW;whether LDC is enbale */
    hi_ldc_v2_attr attr;
} hi_vi_ldc_v2_attr;

/* the attributes of LDCV3 */
typedef struct {
    hi_bool enable; /* RW;whether LDC is enbale */
    hi_ldc_v3_attr attr;
} hi_vi_ldc_v3_attr;

typedef struct {
    hi_bool        enable;                 /* RW;range [0,1];whether rotate_ex is enbale */
    hi_rotation_ex rotation_ex;
} hi_vi_rotation_ex_attr;

/* the status of chn */
typedef struct {
    hi_bool enable;                    /* RO;whether this channel is enabled */
    hi_u32  frame_rate;                /* RO;current frame rate */
    hi_u32  lost_frame;                /* RO;lost frame count */
    hi_u32  vb_fail;                   /* RO;video buffer malloc failure */
    hi_size size;                      /* RO;chn output size */
} hi_vi_chn_status;

typedef struct {
    hi_bool enable;                            /* RW;whether PMF is enable */
    hi_size dest_size;                         /* RW;target size */
    hi_s64  pmf_coef[VI_PMFCOEF_NUM];          /* RW; array of PMF coefficients */
} hi_vi_pmf_attr;

typedef VI_DUMP_TYPE_E hi_vi_dump_type;

typedef struct {
    hi_bool         enable;              /* RW;whether dump is enable */
    hi_u32          depth;               /* RW;range [0,8];depth */
    hi_vi_dump_type dump_type;
} hi_vi_dump_attr;

typedef VI_PIPE_FRAME_SOURCE_E hi_vi_pipe_frame_source;

typedef struct {
    hi_video_frame_info      video_frame;
    hi_isp_config_info       isp_info;
} hi_vi_raw_info;

/* module params */
typedef struct {
    hi_s32       detect_err_frame;
    hi_u32       drop_err_frame;
    hi_vb_source vi_vb_source;
} hi_vi_mod_param;

typedef struct {
    hi_bool enable;               /* RW;range:[0,1];whether enable VI generate timing */
    hi_s32  frm_rate;             /* RW;range:(0,0xffffff];;generate timing frame rate */
} hi_vi_dev_timing_attr;

typedef struct {
    hi_bool enable;
    hi_u32 line_cnt;
} hi_vi_early_interrupt;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_COMM_VI_ADAPT_H__ */