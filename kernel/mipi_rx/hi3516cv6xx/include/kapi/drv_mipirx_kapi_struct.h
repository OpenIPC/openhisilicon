/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */
#ifndef MIPIRX_KAP_STRUCT_H
#define MIPIRX_KAP_STRUCT_H

#include "drv_mipirx_osal.h"

#define MIPIRX_LANE_NUM                4
#define MIPIRX_WDR_VC_NUM              2
#define MIPIRX_SYNC_CODE_NUM           4
#define MIPIRX_MAX_EXT_DATA_TYPE_NUM   3
#define MIPIRX_MAX_EXT_DATA_TYPE_BIT_WIDTH    16
#define MIPIRX_MIN_EXT_DATA_TYPE_BIT_WIDTH    8
#define MIPIRX_EXT_DATA_TYPE_EM_BIT_WIDTH     0

typedef enum {
    MODE_4_LANE = 0,
    MODE_2_PLUS_2_LANE = 1,
    LANE_DIVIDE_MODE_MAX
} mipirx_lane_mode;

typedef enum {
    MIPIRX_INPUT_MODE_MIPI = 0x0,    /* MIPI */
    MIPIRX_INPUT_MODE_SUBLVDS = 0x1, /* SUB_LVDS */
    MIPIRX_INPUT_MODE_LVDS = 0x2,    /* LVDS */
    MIPIRX_INPUT_MODE_HISPI = 0x3,   /* HISPI */
    MIPIRX_INPUT_MODE_MAX
} mipirx_work_mode;

typedef enum {
    MIPIRX_DATA_RATE_X1 = 0, /* output 1 pixel per clock */
    MIPIRX_DATA_RATE_X2 = 1, /* output 2 pixel per clock */
    MIPIRX_DATA_RATE_MAX
} mipirx_data_rate;

typedef struct {
    td_u32 x;
    td_u32 y;
    td_u32 width;
    td_u32 height;
} mipirx_img_rect;

typedef enum {
    MIPIRX_DATA_TYPE_RAW_8BIT = 0,
    MIPIRX_DATA_TYPE_RAW_10BIT,
    MIPIRX_DATA_TYPE_RAW_12BIT,
    MIPIRX_DATA_TYPE_RAW_14BIT,
    MIPIRX_DATA_TYPE_YUV420_8BIT_NORMAL,
    MIPIRX_DATA_TYPE_YUV420_8BIT_LEGACY,
    MIPIRX_DATA_TYPE_YUV422_8BIT,
    MIPIRX_DATA_TYPE_YUV422_PACKED,
    MIPIRX_DATA_TYPE_MAX
} mipirx_data_type;

/* MIPI D_PHY WDR MODE defines */
typedef enum {
    MIPI_WDR_MODE_NONE = 0x0,
    MIPI_WDR_MODE_VC = 0x1,  /* Virtual Channel */
    MIPI_WDR_MODE_DT = 0x2,  /* Data Type */
    MIPI_WDR_MODE_DOL = 0x3, /* DOL Mode */
    MIPI_WDR_MODE_MAX
} mipirx_mipi_wdr_mode;

typedef struct {
    td_u8 num;
    td_u16 data_bit_width[MIPIRX_MAX_EXT_DATA_TYPE_NUM];
    td_u32 data_type[MIPIRX_MAX_EXT_DATA_TYPE_NUM];
} mipirx_mipi_udf_dt;

typedef struct {
    mipirx_mipi_wdr_mode wdr_mode;     /* MIPI WDR mode */
    td_u16 wdr_data_type[MIPIRX_WDR_VC_NUM]; /* WDR DT mode, user defined data type value */
    mipirx_mipi_udf_dt udf_dt;
} mipirx_mipi_cfg;

typedef enum {
    MIPIRX_LVDS_SYNC_MODE_SOF = 0, /* sensor SOL, EOL, SOF, EOF */
    MIPIRX_LVDS_SYNC_MODE_SAV,     /* SAV, EAV */
    MIPIRX_LVDS_SYNC_MODE_MAX
} mipirx_lvds_sync_mode;

typedef enum {
    LVDS_WDR_NONE = 0x00,
    /* SOF-EOF WDR, long exposure frame and short exposure frame has independent sync code */
    LVDS_WDR_INDEPENDENT_SOF = 0x01,
    LVDS_WDR_SHARE_SOF = 0x02, /* SOF-EOF WDR, long exposure frame and short exposure frame share the SOF and EOF */
    LVDS_WDR_DOL_4TH_CODE = 0x03, /* frame identification id in SAV 4th */
    LVDS_WDR_DOL_HCONNECT = 0x04, /* shared SAV EAV */
    LVDS_WDR_DOL_5TH_CODE = 0x05, /* frame identification id in first data(5th code) */
    LVDS_WDR_TYPE_MAX
} mipirx_lvds_wdr_type;

typedef enum {
    LVDS_WDR_NUM_NONE = 0x00,
    LVDS_WDR_2F = 0x01,
    LVDS_WDR_NUM_MAX
} mipirx_wdr_num_type;

typedef struct {
    mipirx_lvds_wdr_type wdr_type;
    mipirx_wdr_num_type wdr_num;
    /* hconnect vsync blanking len, valid when the wdr_type is LVDS_DOL_WDR_HCONNECT */
    td_u16 hblank1;
    td_u16 hblank2;
} mipirx_lvds_wdr_attr;

typedef enum {
    MIPIRX_LVDS_ENDIAN_LITTLE = 0x0,
    MIPIRX_LVDS_ENDIAN_BIG = 0x1,
    LVDS_ENDIAN_MAX
} mipirx_lvds_endian;

typedef struct {
    mipirx_lvds_sync_mode sync_mode;   /* linear sync mode: SOF, SAV */
    mipirx_lvds_wdr_attr wdr_attr; /* wdr attributes */
    
    mipirx_lvds_endian data_endian;      /* data endian: little/big */
    mipirx_lvds_endian sync_code_endian; /* sync code endian: little/big */

    /* each vc has 4 params, sync_code[i]:
       sync_mode is SYNC_MODE_SOF: SOF, EOF, SOL, EOL
       sync_mode is SYNC_MODE_SAV: invalid sav, invalid eav, valid sav, valid eav
       WDR_VC_NUM is how many frames combined into one WDR frame. Currently we only support two frames. */
    td_u16 sync_code[MIPIRX_LANE_NUM][MIPIRX_WDR_VC_NUM][MIPIRX_SYNC_CODE_NUM];
} mipirx_lvds_cfg;

typedef struct {
    td_u8 port_id; /* device number */
    mipirx_work_mode work_mode; /* input mode: MIPI/LVDS/SUBLVDS/HISPI */
    mipirx_img_rect img_rect; /* MIPI Rx device crop area (corresponding to the original sensor input image size) */
    mipirx_data_rate data_rate; /* one clock period one pixel or one clock period two pixels */
    td_s8 lane_id[MIPIRX_LANE_NUM]; /* lane_id: -1 - disable */
    mipirx_data_type data_type;  /* data type: 8/10/12/14 bit */

    union {
        mipirx_mipi_cfg mipi_attr;
        mipirx_lvds_cfg lvds_attr;
    };
} mipirx_port_cfg;

#endif