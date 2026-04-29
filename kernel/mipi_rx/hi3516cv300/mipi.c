/* mipi.c
 *
 * Copyright (c) 2012 Hisilicon Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program;
 *
 * History:
 *      2013.04.03 create this file <zengwen@huawei.com>
 */

#include <linux/module.h>

#include "hi_osal.h"
#include "hi_mipi.h"
#include "mipi_hal.h"

/****************************************************************************
 * MACRO DEFINITION                                                         *
 ****************************************************************************/
#define MIPI_DEV_NAME           "hi_mipi"
#define MIPI_PROC_NAME          "hi_mipi"
#define HIMEDIA_DYNAMIC_MINOR   255

#define MIPI_IRQ           28

#define MIPI_ENABLE_PROC_FS

#define MAX2(x,y)       ( (x)>(y) ? (x):(y) )
#define MIN2(x,y)       ( (x)<(y) ? (x):(y) )

typedef struct hi_MIPI_RX_DEV_CTX_S
{
    HI_BOOL bEnCrop;
    img_rect_t stCropRect;
    combo_dev_attr_t dev_attr;
} MIPI_RX_DEV_CTX_S;


unsigned int mipi_irq = MIPI_IRQ;

/****************************************************************************
 * ERROR CODE DEFINITION                                                         *
 ****************************************************************************/
typedef enum
{
    MIPI_VC0_NO_MATCH  = 0x1 << 0,        /*In VC0 channel, Frame Start don't match Frame End */
    MIPI_VC1_NO_MATCH  = 0x1 << 1,        /*In VC1 channel, Frame Start don't match Frame End*/
    MIPI_VC2_NO_MATCH  = 0x1 << 2,        /*In VC2 channel, Frame Start don't match Frame End*/
    MIPI_VC3_NO_MATCH  = 0x1 << 3,        /*In VC3 channel, Frame Start don't match Frame End*/

    MIPI_VC0_ORDER_ERR = 0x1 << 8,        /*VC0 order error*/
    MIPI_VC1_ORDER_ERR = 0x1 << 9,        /*VC0 order error*/
    MIPI_VC2_ORDER_ERR = 0x1 << 10,       /*VC0 order error*/
    MIPI_VC3_ORDER_ERR = 0x1 << 11,       /*VC0 order error*/

    MIPI_VC0_FRAME_CRC = 0x1 << 16,        /*VC0 frame crc err*/
    MIPI_VC1_FRAME_CRC = 0x1 << 17,        /*VC1 frame crc err*/
    MIPI_VC2_FRAME_CRC = 0x1 << 18,        /*VC2 frame crc err*/
    MIPI_VC3_FRAME_CRC = 0x1 << 19,        /*VC3 frame crc err*/
}MIPI_FRAME_INT_ERR;

typedef enum
{
    MIPI_VC0_PKT_DATA_CRC = 0x1 << 0,      /* VC0 packet data CRC error*/
    MIPI_VC1_PKT_DATA_CRC = 0x1 << 1,
    MIPI_VC2_PKT_DATA_CRC = 0x1 << 2,
    MIPI_VC3_PKT_DATA_CRC = 0x1 << 3,

    MIPI_PKT_HEADER_ERR    = 0x1 << 16,    /*Header at least two errors, ECC can't correct it*/
}MIPI_PKT_INT1_ERR;

typedef enum
{
    MIPI_VC0_PKT_INVALID_DT = 0x1 << 0,        /*VC0 channel data type is invalid*/
    MIPI_VC1_PKT_INVALID_DT = 0x1 << 1,        /*VC1 channel data type is invalid*/
    MIPI_VC2_PKT_INVALID_DT = 0x1 << 2,        /*VC2 channel data type is invalid*/
    MIPI_VC3_PKT_INVALID_DT = 0x1 << 3,        /*VC3 channel data type is invalid*/

    MIPI_VC0_PKT_HEADER_ECC = 0x1 << 16,       /* VC0 channel HEADER error, ECC corrected it*/
    MIPI_VC1_PKT_HEADER_ECC = 0x1 << 17,
    MIPI_VC2_PKT_HEADER_ECC = 0x1 << 18,
    MIPI_VC3_PKT_HEADER_ECC = 0x1 << 19,
}MIPI_PKT_INT2_ERR;

typedef enum
{
    MIPI_ESC_D0   = 0x1 << 0,               /* data lane 0 escape sequence interrupt state */
    MIPI_ESC_D1   = 0x1 << 1,               /* data lane 1 escape sequence interrupt state */
    MIPI_ESC_D2   = 0x1 << 2,               /* data lane 2 escape sequence interrupt state */
    MIPI_ESC_D3   = 0x1 << 3,               /* data lane 3 escape sequence interrupt state */
    MIPI_ESC_CLK  = 0x1 << 4,               /* clock lane escape sequence interrupt state */

    MIPI_TIMEOUT_D0  = 0x1 << 8,            /* data lane 0 FSM timeout interrupt state */
    MIPI_TIMEOUT_D1  = 0x1 << 9,            /* data lane 1 FSM timeout interrupt state */
    MIPI_TIMEOUT_D2  = 0x1 << 10,           /* data lane 2 FSM timeout interrupt state */
    MIPI_TIMEOUT_D3  = 0x1 << 11,           /* data lane 3 FSM timeout interrupt state */
    MIPI_TIMEOUT_CLK = 0x1 << 12,           /* clock lane FSM timeout interrupt state */

}LINK_INT_STAT;

typedef enum
{
    CMD_FIFO_READ_ERR   = 0x1 << 0,        /* MIPI_CTRL read command FIFO error */
    DATA_FIFO_READ_ERR  = 0x1 << 1,
    CMD_FIFO_WRITE_ERR  = 0x1 << 16,
    DATA_FIFO_WRITE_ERR = 0x1 << 17,
}MIPI_CTRL_INT_ERR;

typedef enum
{
    LANE0_SYNC_ERR  = 0x1 << 0,
    LANE1_SYNC_ERR  = 0x1 << 1,
    LANE2_SYNC_ERR  = 0x1 << 2,
    LANE3_SYNC_ERR  = 0x1 << 3,

    LINK0_WRITE_ERR = 0x1 << 7,

    LINK0_VSYNC_ERR = 0x1 << 5,

    LINK0_HSYNC_ERR = 0x1 << 6,

    LVDS_STAT_ERR   = 0x1 << 4,

    LINK0_READ_ERR  = 0x1 << 8,
}LVDS_CTRL_INTR_ERR;

typedef enum
{
    ALIGN_LANE0_ERR     = 0x1 << 0,            /* MIPI ALIGN lane0 err */
    ALIGN_LANE1_ERR     = 0x1 << 1,
    ALIGN_LANE2_ERR     = 0x1 << 2,
    ALIGN_LANE3_ERR     = 0x1 << 3,
    ALIGN_FIFO_FULL_ERR = 0x1 << 4,            /* MIPI ALIGN FIFO full */
}ALIGN_CTRL_INT_ERR;

//***************************************************************************
// Struct DEFINITION
// **************************************************************************
/* Channel data CRC error */
typedef  struct hi_MIPI_VC_DATA_CRC_ERR_INFO_S
{
    unsigned int  vc0_frame_err_cnt;
    unsigned int  vc1_frame_err_cnt;
    unsigned int  vc2_frame_err_cnt;
    unsigned int  vc3_frame_err_cnt;
} MIPI_VC_DATA_CRC_ERR_INFO_S;

/* frame sequence error */
typedef  struct hi_MIPI_VC_FRAME_ORDER_ERR_INFO_S
{
    unsigned int  vc0_order_err_cnt;
    unsigned int  vc1_order_err_cnt;
    unsigned int  vc2_order_err_cnt;
    unsigned int  vc3_order_err_cnt;
} MIPI_VC_FRAME_ORDER_ERR_INFO_S;

/* VC Channel sof and eof dismatch */
typedef struct hi_MIPI_FRAME_NO_MATCH_INFO_S
{
    unsigned int    vc0_nomatch_cnt;
    unsigned int    vc1_nomatch_cnt;
    unsigned int    vc2_nomatch_cnt;
    unsigned int    vc3_nomatch_cnt;
} MIPI_VC_FRAME_NO_MATCH_INFO;

// -------------------------------------

// MIPI_PKT_INTR_ST and MIPI_PKT_INTR2_ST
typedef struct hi_MIPI_VC_PKT_HEADER_ERR_INFO_S
{
    unsigned int header_err_cnt;    /* at least 2 err, ECC can no correct */

    unsigned int vc0_header_correct_cnt;  /* VC0 header error, ECC already correct */
    unsigned int vc1_header_correct_cnt;
    unsigned int vc2_header_correct_cnt;
    unsigned int vc3_header_correct_cnt;
}MIPI_VC_PKT_HEADER_ERR_INFO_S;

/* VC Channel data type not support */
typedef struct hi_MIPI_VC_PKT_DT_ERR_INFO_S
{
    unsigned int   vc0_dt_err_cnt;
    unsigned int   vc1_dt_err_cnt;
    unsigned int   vc2_dt_err_cnt;
    unsigned int   vc3_dt_err_cnt;
} MIPI_VC_PKT_DT_ERR_INFO_S;

typedef struct hi_MIPI_CTRL_ERR_INFO_S
{
    unsigned int cmd_fifo_rerr_cnt;
    unsigned int cmd_fifo_werr_cnt;
    unsigned int data_fifo_rerr_cnt;
    unsigned int data_fifo_werr_cnt;
} MIPI_CTRL_ERR_INFO_S;

typedef struct hi_LVDS_FRAME_DBG_INFO_S
{
    unsigned int lane_sync_err_cnt[COMBO_MAX_LANE_NUM];

    unsigned int link_write_err_cnt[COMBO_MAX_LINK_NUM];
    unsigned int link_read_err_cnt[COMBO_MAX_LINK_NUM];
    unsigned int link_vsync_err_cnt[COMBO_MAX_LINK_NUM];
    unsigned int link_hsync_err_cnt[COMBO_MAX_LINK_NUM];
    unsigned int lvds_stat_err_cnt;
} LVDS_FRAME_DBG_INFO_S;

/*mipi frame err infomation*/
typedef struct hi_MIPI_FRAME_DBG_INFO_S
{
    /* MIPI_PKT_INTR */
    MIPI_VC_PKT_HEADER_ERR_INFO_S   VcPktHeaderErrInfo;
    MIPI_VC_DATA_CRC_ERR_INFO_S     VcPktDataErrInfo;
    MIPI_VC_PKT_DT_ERR_INFO_S       VcPktDTErrInfo;

    /* MIPI_FRAME_INTR_MSK */
    MIPI_VC_DATA_CRC_ERR_INFO_S     VcFrameDataErrInfo;
    MIPI_VC_FRAME_ORDER_ERR_INFO_S  VcFrameOrderErrInfo;
    MIPI_VC_FRAME_NO_MATCH_INFO     VcFrameNoMatchInfo;

    /* MIPI_CTRL_INT */
    MIPI_CTRL_ERR_INFO_S            mipiCtrlErrInfo;
} MIPI_FRAME_DBG_INFO_S;

typedef struct hi_mipi_phy_int_state
{
    unsigned int link_sync_err_cnt;

    unsigned int clk_fsm_timeout_cnt;
    unsigned int d0_fsm_timeout_cnt;
    unsigned int d1_fsm_timeout_cnt;
    unsigned int d2_fsm_timeout_cnt;
    unsigned int d3_fsm_timeout_cnt;

    unsigned int clk_fsm_escape_cnt;
    unsigned int d0_fsm_escape_cnt;
    unsigned int d1_fsm_escape_cnt;
    unsigned int d2_fsm_escape_cnt;
    unsigned int d3_fsm_escape_cnt;
} mipi_phy_int_state;

typedef struct hi_ALIGN_DBG_INFO_S
{
    unsigned int fifo_full_err_cnt;
    unsigned int lane_err_cnt[COMBO_MAX_LANE_NUM];
}ALIGN_DBG_INFO_S;



/****************************************************************************
 * Variables DEFINITION                                                     *
 ****************************************************************************/
static osal_dev_t *s_pstHiMipiDevice = NULL;
static mipi_phy_int_state     phy_int_state[COMBO_MAX_LINK_NUM];

static MIPI_FRAME_DBG_INFO_S  mipi_err_info[COMBO_MAX_DEV_NUM];
static LVDS_FRAME_DBG_INFO_S  lvds_err_info[COMBO_MAX_DEV_NUM];

static ALIGN_DBG_INFO_S       align_err_info[COMBO_MAX_DEV_NUM];

static osal_mutex_t hi_mipi_lock;

static link_info_s g_lane_info[COMBO_MAX_DEV_NUM];
static MIPI_RX_DEV_CTX_S g_dev_ctx[COMBO_MAX_DEV_NUM];
static void mipi_clear_lvds_sync_codes(COMBO_DEV devno, link_info_s* p_link_info);


/******************************************************************************
* Function DEFINITION
******************************************************************************/
static work_mode_t get_work_mode(input_mode_t input_mode)
{
    if (INPUT_MODE_MIPI == input_mode)
    {
        return WORK_MODE_MIPI;
    }
    else if (INPUT_MODE_CMOS == input_mode)
    {
        return WORK_MODE_CMOS;
    }
    else if (INPUT_MODE_BT1120 == input_mode)
    {
        return WORK_MODE_BT1120;
    }
    else
    {
        return WORK_MODE_LVDS;
    }
}

static inline const char* mipi_get_work_mode_name(work_mode_t work_mode)
{
    switch (work_mode)
    {
        case WORK_MODE_LVDS:
            return "LVDS";
        case WORK_MODE_MIPI:
            return "MIPI";
        case WORK_MODE_CMOS:
            return "CMOS";
        case WORK_MODE_BT1120:
            return "BT1120";
        default:
            break;
    }

    return "N/A";
}

static inline const char* mipi_get_raw_data_type_name(raw_data_type_e type)
{
    switch (type)
    {
        case RAW_DATA_8BIT:
            return "RAW8";
        case RAW_DATA_10BIT:
            return "RAW10";
        case RAW_DATA_12BIT:
            return "RAW12";
        case RAW_DATA_14BIT:
            return "RAW14";
        case RAW_DATA_16BIT:
            return "RAW16";
        default:
            break;
    }
    return "N/A";
}

void  hi_mipi_init(void)
{
    int i;
#ifndef HI_FPGA
    HI_MSG("%s()-----ASIC-----------\n", __FUNCTION__);
#else
    HI_MSG("%s()-----FPGA-----------\n", __FUNCTION__);
#endif

#ifndef CONFIG_QUICK_START
    /* mipi apb reset and core reset */
    for (i = 0; i < COMBO_MAX_DEV_NUM; i++)
    {
         mipi_drv_core_reset(i);
         mipi_drv_apb_reset(i);

         mipi_drv_core_unreset(i);
         mipi_drv_apb_unreset(i);
    }
#endif

    // set MIPI sys interrupt
    mipi_drv_set_chn_int_mask(0, MIPI_CHN0_INT_MASK);

#if 1
    /* set the MIPI CIL int masks */
    for (i = 0; i < COMBO_MAX_LINK_NUM; i++)
    {
        mipi_drv_set_cil_int_mask(i, MIPI_CIL_INT_MASK);
    }

    mipi_drv_set_lvds_ctrl_int_mask(0, LVDS_CTRL0_INT_MASK);
    mipi_drv_set_mipi_ctrl_int_mask(0, MIPI_CTRL_INT_MASK);
    //mipi_drv_set_mipi_pkt_int1_mask(0, MIPI_PKT_INT_MASK);
    mipi_drv_set_mipi_frame_int_mask(0, MIPI_FRAME_INT_MASK);
    //mipi_drv_set_align_int_mask(0, ALIGN0_INT_MASK);  // disable align interrupt

#endif
}

int check_lane_id(input_mode_t input_mode, short p_lane_id[])
{
    int i = 0, j = 0;
    int temp_lane_num = 0;
    int min_lane_id = COMBO_MAX_LANE_NUM;
    int max_lane_id = -1;
    int lane_id_size;

    if (INPUT_MODE_MIPI == input_mode)
    {
        lane_id_size = MIPI_LANE_NUM;
    }
    else
    {
        lane_id_size = LVDS_LANE_NUM;
    }

    for (i = 0; i < lane_id_size; i++)
    {
        int temp_id = p_lane_id[i];

        if (temp_id < -1 || temp_id >= COMBO_MAX_LANE_NUM)
        {
            HI_ERR("lane_id[%d] is invalid value %d.\n", i, temp_id);
            return HI_FAILURE;
        }

        if ( -1 != temp_id)
        {
            for (j = i + 1; j < lane_id_size; j++)
            {
                if (temp_id == p_lane_id[j])
                {
                    HI_ERR("lane_id[%d] can't be same value %d as lane_id[%d]\n", i, temp_id, j);
                    return HI_FAILURE;
                }
            }

            HI_MSG(" %d", temp_id);
            temp_lane_num++;

            min_lane_id = MIN2(min_lane_id, temp_id);
            max_lane_id = MAX2(max_lane_id, temp_id);
        }
    }

    if (max_lane_id < 0 || COMBO_MAX_LANE_NUM == min_lane_id)
    {
        HI_ERR("all lane_id is invalid, min_lane_id:%d, max_lane_id:%d\n", min_lane_id, max_lane_id);
        return HI_FAILURE;
    }

    /* use 1 phy 2 lanes, invalid value: (0, 2), (0, 3) */
    if (max_lane_id > temp_lane_num - 1)
    {
        HI_ERR("max_lane_id(%d) is larger than %d, total_num: %d\n",
            max_lane_id, temp_lane_num - 1, temp_lane_num);
        return HI_FAILURE;
    }

    if (input_mode == INPUT_MODE_MIPI)
    {
        /* to do */

    }

    return HI_SUCCESS;
}

static inline void init_lane_info(link_info_s* p_link_info)
{
    osal_memset(p_link_info, -1, sizeof(link_info_s));
    p_link_info->b_order = 0;
}

//******************************************************************************
//  Function    : get_lane_info
//  Description : get the specified dev's lane_info
//  Input       : devno:  sensor number must less than COMBO_MAX_DEV_NUM
//                input_mode: input interface mode
//                p_lane_id:  point to a array, which store the lane's priority
//  output:     : p_dev_link_info, return this pointer to caller
//  Return      : int: HI_FAILURE - Error, HI_SUCCESS - Success
//******************************************************************************
static int get_lane_info(COMBO_DEV devno, input_mode_t input_mode, short* p_lane_id, link_info_s** p_dev_link_info)
{
    int i = 0, j = 0;
    int temp_link_idx = 0;
    int lane_id_size;
    link_info_s link_info, *p_link_info;

    p_link_info = &link_info;

    if (INPUT_MODE_MIPI == input_mode)
    {
        lane_id_size = MIPI_LANE_NUM;
    }
    else
    {
        lane_id_size = LVDS_LANE_NUM;
    }

    init_lane_info(p_link_info);

    p_link_info->total_lane_num = 0;
    /* i is link index */
    for (i = 0; i < lane_id_size / LANE_NUM_PER_LINK; i++)
    {
        int temp_lane_count = 0;
        /* j is lane index for every link */
        for (j = 0; j < LANE_NUM_PER_LINK; j++)
        {
            p_link_info->link_arr[temp_link_idx].lane_idx[j] = p_lane_id[i * LANE_NUM_PER_LINK + j];

            if (-1 != p_lane_id[i * LANE_NUM_PER_LINK + j])
            {
                temp_lane_count++;    /* temp_lane_idx calculate valid lane numbers for each link */
                if (-1 == p_link_info->link_arr[temp_link_idx].link_idx)
                {
                    p_link_info->link_arr[temp_link_idx].link_idx = i;
                }
            }
        }
        p_link_info->link_arr[temp_link_idx].lane_num = temp_lane_count;

        if (-1 != p_link_info->link_arr[temp_link_idx].link_idx)
        {
            temp_link_idx++;
        }
        p_link_info->total_lane_num += temp_lane_count;
    }

    p_link_info->link_num = temp_link_idx;

#ifdef HI_MIPI_DEBUG
    osal_printk("\n-------------------------------------\n");
    osal_printk("link number    total_lane_number\n");
    osal_printk("%11d    %17d\n", p_link_info->link_num, p_link_info->total_lane_num);
    for (i = 0; i < p_link_info->link_num; i++)
    {
        osal_printk("link_idx:%d    lane_num:%d\n", p_link_info->link_arr[i].link_idx, p_link_info->link_arr[i].lane_num);

        for (j = 0; j < LANE_NUM_PER_LINK; j++)
        {
            osal_printk("    %2d", p_link_info->link_arr[i].lane_idx[j]);
        }
        osal_printk("\n");
    }
#endif



    // TODO: check if sensor0 and sensor1 have the same lane_idx

    g_lane_info[devno] = *p_link_info;
    *p_dev_link_info = &g_lane_info[devno];
    return HI_SUCCESS;
}

/*
 * DESCRIPTION  :   check is the device is configured
 * RETURN VALUE :   the values returned are nonzero if device not configured
 *                  and a zero value if not
 */
static inline int is_dev_cfged(COMBO_DEV devno)
{
    if (devno >= COMBO_MAX_DEV_NUM)
    {
        return HI_FALSE;
    }

    if (g_lane_info[devno].link_num <= 0 || g_lane_info[devno].total_lane_num <= 0)
    {
        return HI_FALSE;
    }

    return HI_TRUE;
}

/*
 * DESCRIPTION  :   check is the MIPI is configured
 * RETURN VALUE :   the values returned are nonzero if at least one device is configured
 *                  and a zero value if all the devices are not configured
 */
static int is_mipi_cfged(void)
{
    COMBO_DEV devno;

    for (devno = 0; devno < COMBO_MAX_DEV_NUM; devno++)
    {
        if (0 == is_dev_cfged(devno))
        {
            return HI_TRUE;
        }
    }

    return HI_FALSE;
}

inline int check_mipi_dev_attr(mipi_dev_attr_t* p_attr)
{
    int ret = 0;
    int i;

    if ((p_attr->raw_data_type < RAW_DATA_8BIT) || (p_attr->raw_data_type >= RAW_DATA_BUTT))
    {
        HI_ERR("invalid raw_data_type, must be in [%d, %d)\n", RAW_DATA_8BIT, RAW_DATA_BUTT);
        return HI_FAILURE;
    }
    if ((p_attr->wdr_mode < HI_MIPI_WDR_MODE_NONE) || (p_attr->wdr_mode >= HI_MIPI_WDR_MODE_BUTT))
    {
        HI_ERR("invalid wdr_mode, must be in [%d, %d]\n", HI_MIPI_WDR_MODE_NONE, HI_MIPI_WDR_MODE_BUTT);
        return HI_FAILURE;
    }

    if (HI_MIPI_WDR_MODE_DT == p_attr->wdr_mode)
    {
        for (i = 0; i < WDR_VC_NUM; i++)
        {
            /* data_type must be the CSI-2 reserve Type [0x38, 0x3f] */
            if (p_attr->data_type[i] < 0x38 || p_attr->data_type[i] > 0x3f)
            {
                HI_ERR("invalid data_type[%d]: %d, must be in [0x38, 0x3f]\n", i, p_attr->data_type[i]);
                return HI_FAILURE;
            }
        }
    }

    ret = check_lane_id(INPUT_MODE_MIPI, p_attr->lane_id);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }
    return HI_SUCCESS;
}

//******************************************************************************
//  Function    : mipi_clear_config
//  Description : clear the last configs
//  Input       : devno, work_mode
//  Return      : int: HI_FAILURE - Error, HI_SUCCESS - Success
//******************************************************************************
static void mipi_clear_config(COMBO_DEV devno, work_mode_t work_mode)
{
    int i = 0;
    link_info_s* p_link_info;
    short temp_lane_id[LANE_NUM_PER_LINK] = { -1, -1, -1, -1};    // lane_id per_link

    if (HI_TRUE != is_dev_cfged(devno))
    {
        return;     // mipi not configured, ignore
    }

    p_link_info = &g_lane_info[devno];  // the last lane_info

    for (i = 0; i < p_link_info->link_num; i++)
    {
        COMBO_LINK link_id = p_link_info->link_arr[i].link_idx;

        mipi_drv_set_phy_te_hs_en(link_id, temp_lane_id);
        mipi_drv_set_phy_mode(link_id, work_mode, temp_lane_id);
        mipi_drv_set_link_lane_id(devno, link_id, temp_lane_id);
#ifndef HI_FPGA
        mipi_drv_set_phy_ckgt_en(link_id, HI_FALSE);
#endif
    }
    mipi_drv_clear_phy_chn_ctrl(devno, &g_lane_info[devno]);

}

int mipi_set_mipi_dev_attr(COMBO_DEV devno, mipi_dev_attr_t* p_attr)
{
    raw_data_type_e input_dt;
    COMBO_LINK link_id = 0;
    int ret = HI_SUCCESS;
    link_info_s* p_link_info = NULL;
    int i = 0;

    input_dt = p_attr->raw_data_type;

    ret = check_mipi_dev_attr(p_attr);
    if (HI_SUCCESS != ret)
    {
        HI_ERR("devno[%d], check mipi attr failed\n", devno);
        return ret;
    }

    /* clear last config */
    mipi_clear_config(devno, WORK_MODE_MIPI);

    ret = get_lane_info(devno, INPUT_MODE_MIPI, p_attr->lane_id, &p_link_info);
    if (NULL == p_link_info)
    {
        return ret;
    }

    /* clear dbg info */
    for (i = 0; i < p_link_info->link_num; i++)
        {
            COMBO_LINK link_id = p_link_info->link_arr[i].link_idx;
            osal_memset(&phy_int_state[link_id], 0, sizeof(mipi_phy_int_state));
        }
    osal_memset(&mipi_err_info[devno], 0, sizeof(MIPI_FRAME_DBG_INFO_S));
    osal_memset(&align_err_info[devno], 0, sizeof(ALIGN_DBG_INFO_S));

    mipi_drv_set_sys_work_mode(devno, INPUT_MODE_MIPI);

    mipi_drv_set_lane_num(devno, p_link_info->total_lane_num);
    mipi_drv_set_phy_chn_ctrl(devno, p_link_info);
    mipi_drv_set_mem_cken(HI_TRUE);

    mipi_drv_set_di_dt(devno, input_dt);

    // set MIPI WDR mode
    if (HI_MIPI_WDR_MODE_DT == p_attr->wdr_mode)
    {
        mipi_drv_set_mipi_user_dt(devno, input_dt, p_attr->data_type);
    }
    else if(HI_MIPI_WDR_MODE_DOL == p_attr->wdr_mode)
    {
        mipi_drv_set_mipi_dol_id(devno, input_dt, NULL);
    }
    mipi_drv_set_mipi_wdr_mode(devno, p_attr->wdr_mode);

    /******************************************************
    when mipi mode select lane
    1) { 3,  2,  1,  0, -1, -1, -1, -1}: phy0
    2) {-1, -1, -1, -1,  3,  2,  1,  0}: phy1
    3) {-1,  0,  1, -1, -1,  2,  3, -1}: two link
    *******************************************************/
    for (i = 0; i < p_link_info->link_num; i++)
    {
        link_id = p_link_info->link_arr[i].link_idx;

        mipi_drv_set_output_clk_edge(link_id, CLK_DOWN_EDGE);
        mipi_drv_set_phy_te_hs_en(link_id, p_link_info->link_arr[i].lane_idx);

        mipi_drv_set_link_lane_id(devno, link_id, p_link_info->link_arr[i].lane_idx);

        mipi_drv_set_phy_mode(link_id, WORK_MODE_MIPI, p_link_info->link_arr[i].lane_idx);

#ifndef HI_FPGA
        mipi_drv_set_phy_ckgt_en(link_id, HI_TRUE);
#endif
    }

    //osal_printk("set mipi phy attr successful!\n");
    return HI_SUCCESS;
}

inline int check_lvds_dev_attr(lvds_dev_attr_t* p_attr)
{
    int ret, i, lane_num = 0;

    if ((p_attr->raw_data_type < RAW_DATA_8BIT) || (p_attr->raw_data_type >= RAW_DATA_BUTT))
    {
        HI_ERR("invalid raw_data_type, must be in [%d, %d)\n", RAW_DATA_8BIT, RAW_DATA_BUTT);
        return HI_FAILURE;
    }
    if ((p_attr->data_endian < LVDS_ENDIAN_LITTLE) || (p_attr->data_endian >= LVDS_ENDIAN_BUTT))
    {
        HI_ERR("invalid lvds_bit_endian, must be in [%d, %d)\n", LVDS_ENDIAN_LITTLE, LVDS_ENDIAN_BUTT);
        return HI_FAILURE;
    }
    if ((p_attr->sync_code_endian < LVDS_ENDIAN_LITTLE) || (p_attr->sync_code_endian >= LVDS_ENDIAN_BUTT))
    {
        HI_ERR("invalid lvds_bit_endian, must be in [%d, %d)\n", LVDS_ENDIAN_LITTLE, LVDS_ENDIAN_BUTT);
        return HI_FAILURE;
    }
    if ((p_attr->sync_mode < LVDS_SYNC_MODE_SOF) || (p_attr->sync_mode >= LVDS_SYNC_MODE_BUTT))
    {
        HI_ERR("invalid sync_mode, must be in [%d, %d)\n", LVDS_SYNC_MODE_SOF, LVDS_SYNC_MODE_BUTT);
        return HI_FAILURE;
    }
    if ( (p_attr->wdr_mode < HI_WDR_MODE_NONE) || (p_attr->wdr_mode >= HI_WDR_MODE_BUTT))
    {
        HI_ERR("invalid wdr_mode, must be in [%d, %d]\n", HI_WDR_MODE_NONE, HI_WDR_MODE_DOL_4F);
        return HI_FAILURE;
    }
    if (p_attr->img_size.width < LVDS_MIN_WIDTH || p_attr->img_size.height < LVDS_MIN_HEIGHT)
    {
        HI_ERR("invalid img_size(%d, %d), can't be smaller than (%d, %d)\n",
               p_attr->img_size.width, p_attr->img_size.height, LVDS_MIN_WIDTH, LVDS_MIN_HEIGHT);
        return HI_FAILURE;
    }
    if (p_attr->vsync_type.sync_type < LVDS_VSYNC_NORMAL || p_attr->vsync_type.sync_type >= LVDS_VSYNC_BUTT)
    {
        HI_ERR("invalid vsync_code, must be in [%d, %d]\n", LVDS_VSYNC_NORMAL, LVDS_VSYNC_BUTT);
        return HI_FAILURE;
    }
    if (p_attr->fid_type.fid < LVDS_FID_NONE|| p_attr->fid_type.fid >= LVDS_FID_BUTT)
    {
        HI_ERR("invalid fid_type, must be in [%d, %d]\n", LVDS_FID_NONE, LVDS_FID_BUTT);
        return HI_FAILURE;
    }
    if (HI_TRUE != p_attr->fid_type.output_fil && HI_FALSE != p_attr->fid_type.output_fil)
    {
        HI_ERR("invalid output_fil, must be HI_TURE or HI_FALSE\n");
        return HI_FAILURE;
    }

    if (p_attr->wdr_mode <= HI_WDR_MODE_4F && p_attr->wdr_mode >= HI_WDR_MODE_2F)
    {
        if (LVDS_VSYNC_NORMAL != p_attr->vsync_type.sync_type && LVDS_VSYNC_SHARE != p_attr->vsync_type.sync_type)
        {
            HI_ERR("invalid sync_type, must be LVDS_VSYNC_NORMAL or LVDS_VSYNC_SHARE\n");
            return HI_FAILURE;
        }
    }
    else if (p_attr->wdr_mode <= HI_WDR_MODE_DOL_4F && p_attr->wdr_mode >= HI_WDR_MODE_DOL_2F)
    {
        if (LVDS_VSYNC_NORMAL == p_attr->vsync_type.sync_type)
        {
            if (LVDS_FID_IN_SAV != p_attr->fid_type.fid && LVDS_FID_IN_DATA != p_attr->fid_type.fid)
            {
                HI_ERR("invalid fid_type, must be LVDS_FID_IN_SAV or LVDS_FID_IN_DATA\n");
                return HI_FAILURE;
            }
        }
        else if (LVDS_VSYNC_HCONNECT == p_attr->vsync_type.sync_type)
        {
            if (LVDS_FID_NONE != p_attr->fid_type.fid && LVDS_FID_IN_DATA != p_attr->fid_type.fid)
            {
                HI_ERR("invalid fid_type, must be LVDS_FID_NONE or LVDS_FID_IN_DATA\n");
                return HI_FAILURE;
            }
        }
        else
        {
            HI_ERR("invalid sync_type, must be LVDS_VSYNC_NORMAL or LVDS_VSYNC_HCONNECT\n");
            return HI_FAILURE;
        }
    }

    // check width, must be divided by lane_num
    for (i = 0; i < LVDS_LANE_NUM; i++)
    {
        if (-1 != p_attr->lane_id[i])
        {
            lane_num++;
        }
    }

    if (lane_num == 0)
    {
        HI_ERR("Invalid lane_id.\n");
        return HI_FAILURE;
    }

    if (0 != p_attr->img_size.width % lane_num)
    {
        HI_ERR("invalid width %d, must be muliples of %d\n", p_attr->img_size.width, lane_num);
        return HI_FAILURE;
    }

    ret = check_lane_id(INPUT_MODE_LVDS, p_attr->lane_id);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    return HI_SUCCESS;
}

// set lvds sync codes
static void mipi_set_lvds_sync_codes(COMBO_DEV devno, lvds_dev_attr_t* p_attr, link_info_s* p_link_info)
{
    int i, j, k;
    unsigned short (*p_nxt_sync_code)[WDR_VC_NUM][SYNC_CODE_NUM] = NULL;
    HI_BOOL alloc_flag = HI_FALSE;
    unsigned int lane_cnt;

    lane_cnt = p_link_info->total_lane_num;

    if (p_attr->wdr_mode >= HI_WDR_MODE_DOL_2F && p_attr->wdr_mode <= HI_WDR_MODE_DOL_4F)
    {
        /* Sony DOL Mode */
        p_nxt_sync_code = osal_vmalloc(sizeof(unsigned short) * lane_cnt * WDR_VC_NUM * SYNC_CODE_NUM);
        if (NULL == p_nxt_sync_code)
        {
            HI_ERR("alloc memory for nxt_sync_code failed\n");
            return;
        }
        alloc_flag = HI_TRUE;

        /* SONY DOL N frame and N+1 Frame has the diffrent sync_code
        * N+1 Frame FSET is 1, FSET is the 10th bit
        */
        // TODO: assume raw data is 12bit, FSET may be diffrent among other raw data
        for (i = 0; i < lane_cnt; i++)
        {
            for (j = 0; j < WDR_VC_NUM; j++)
            {
                for (k = 0; k < SYNC_CODE_NUM; k++)
                {
                    if (RAW_DATA_10BIT == p_attr->raw_data_type)
                    {
                        p_nxt_sync_code[i][j][k] = p_attr->sync_code[i][j][k] + (1 << 8);
                    }
                    else
                    {
                        p_nxt_sync_code[i][j][k] = p_attr->sync_code[i][j][k] + (1 << 10);
                    }
                }
            }
        }

        /* Set Dony DOL Line Information */
        if (p_attr->fid_type.fid == LVDS_FID_IN_DATA)
        {
            mipi_drv_set_dol_line_information(devno, p_attr->wdr_mode, p_attr->raw_data_type);
        }
    }
    else
    {
        p_nxt_sync_code = p_attr->sync_code;
    }

    /* LVDS_CTRL Sync code */
    mipi_drv_set_lvds_sync_code(devno, lane_cnt, p_attr->sync_code);
    mipi_drv_set_lvds_nxt_sync_code(devno, lane_cnt, p_nxt_sync_code);

    /* PHY Sync code detect setting */
    for (i = 0; i < p_link_info->link_num; i++)
    {
        COMBO_LINK link_id = p_link_info->link_arr[i].link_idx;
        mipi_drv_set_phy_sync_dct(link_id, p_attr->raw_data_type, p_attr->sync_code_endian);

        mipi_drv_set_lvds_phy_sync_code(link_id, p_link_info->link_arr[i].lane_idx,
                     p_attr->sync_code, p_nxt_sync_code);
    }

    if (HI_TRUE == alloc_flag)
    {
        osal_vfree(p_nxt_sync_code);
    }
}

// p_link_info must be the previous linkinfo, so this function must be called before get_lane_info
static void mipi_clear_lvds_sync_codes(COMBO_DEV devno, link_info_s* p_link_info)
{
    int i;
    unsigned short (*sync_code)[WDR_VC_NUM][SYNC_CODE_NUM] = NULL;
    unsigned int lane_cnt;

    lane_cnt = p_link_info->total_lane_num;

    sync_code = osal_vmalloc(sizeof(unsigned short) * lane_cnt * WDR_VC_NUM * SYNC_CODE_NUM);
    if (NULL == sync_code)
    {
        HI_ERR("alloc mem for syc_code failed\n");
        return;
    }
    // sync_code all set to 0, to clear the previous sync_code
    osal_memset(sync_code, 0, sizeof(unsigned short) * lane_cnt * WDR_VC_NUM * SYNC_CODE_NUM);

    mipi_drv_set_lvds_sync_code(devno, lane_cnt, sync_code);
    mipi_drv_set_lvds_nxt_sync_code(devno, lane_cnt, sync_code);

    for (i = 0; i < p_link_info->link_num; i++)
    {
        COMBO_LINK link_id = p_link_info->link_arr[i].link_idx;

        // the default value is RAW8 and big endian
        mipi_drv_set_phy_sync_dct(link_id, RAW_DATA_8BIT, LVDS_ENDIAN_BIG);

        mipi_drv_set_lvds_phy_sync_code(link_id, p_link_info->link_arr[i].lane_idx,
                     sync_code, sync_code);
    }

    if (NULL != sync_code)
    {
        osal_vfree(sync_code);
    }
}

int mipi_set_lvds_dev_attr(COMBO_DEV devno, input_mode_t input_mode, lvds_dev_attr_t* p_attr)
{
    int ret = HI_SUCCESS;
    img_size_t img_size;
    link_info_s* p_link_info = NULL;
    work_mode_t work_mode;
    int i;

    ret = check_lvds_dev_attr(p_attr);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    work_mode = get_work_mode(input_mode);

    /* clear last config, must be called before get_lane_info */
    mipi_clear_config(devno, work_mode);
    if (g_lane_info[devno].link_num > 0)
    {
        mipi_clear_lvds_sync_codes(devno, &g_lane_info[devno]);     // clear the last sync code
    }

    ret = get_lane_info(devno, input_mode, p_attr->lane_id, &p_link_info);
    if (NULL == p_link_info)
    {
        HI_ERR("devno[%d], get_lane_info failed\n", devno);
        return ret;
    }

    /* clear dbg info */
    for (i = 0; i < p_link_info->link_num; i++)
    {
        COMBO_LINK link_id = p_link_info->link_arr[i].link_idx;
        osal_memset(&phy_int_state[link_id], 0, sizeof(mipi_phy_int_state));
    }
    osal_memset(&lvds_err_info[devno], 0, sizeof(LVDS_FRAME_DBG_INFO_S));
    osal_memset(&align_err_info[devno], 0, sizeof(ALIGN_DBG_INFO_S));

    mipi_drv_set_sys_work_mode(devno, INPUT_MODE_LVDS);

    mipi_drv_set_phy_chn_ctrl(devno, p_link_info);
    /**************************************************************
    * if input_mode is MIPI or LVDS, then only PHY2 is available for sensor1
    * lane_id example
    * 1. { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11}: use phy0, phy1 and phy2;
    * 2. {11,  7,  3, 10,  6,  2,  9,  5,  1,  8,  4,  0}: disorder, use phy0, phy1 and phy2
    * 3. { 0,  1,  2,  3,  4,  5,  6,  7, -1, -1, -1, -1}: use phy0 and phy1
    * 4. {-1, -1, -1, -1,  0,  1,  2,  3, -1, -1, -1, -1}: use phy1
    * 5. {-1, -1, -1, -1, -1,  1, -1,  3,  0, -1,  2, -1}: disorder, use phy1 and phy2
    ***************************************************************/

    img_size.height = p_attr->img_size.height -1;
    // image width is each lane's height
    img_size.width  = (p_attr->img_size.width / p_link_info->total_lane_num) -1;

    mipi_drv_set_lvds_image_size(devno, &img_size);
    mipi_drv_set_lvds_wdr_mode(devno, p_attr->wdr_mode, &p_attr->vsync_type, &p_attr->fid_type);

    mipi_drv_set_lvds_ctrl_mode(devno, p_attr->sync_mode, p_attr->raw_data_type,
                                p_attr->data_endian, p_attr->sync_code_endian,
                                p_link_info->b_order);

    mipi_drv_set_mem_cken(HI_TRUE);

    // if in WDR mode, set the LVDS_PRESYNC will call the sync error,
    // can not get the width and height statis info
    //if (HI_WDR_MODE_NONE == p_attr->wdr_mode)
    {
        mipi_drv_set_lvds_presync_clr_en(HI_TRUE);
    }

    for (i = 0; i < p_link_info->link_num; i++)
    {
        COMBO_LINK link_id = p_link_info->link_arr[i].link_idx;

        mipi_drv_set_link_lane_id(devno, link_id, p_link_info->link_arr[i].lane_idx);

        mipi_drv_set_output_clk_edge(link_id, CLK_DOWN_EDGE);
        mipi_drv_set_phy_te_hs_en(link_id, p_link_info->link_arr[i].lane_idx);

        mipi_drv_set_phy_mode(link_id, work_mode, p_link_info->link_arr[i].lane_idx);
#ifndef HI_FPGA
        mipi_drv_set_phy_ckgt_en(link_id, HI_TRUE);
#endif
    }

    mipi_set_lvds_sync_codes(devno, p_attr, p_link_info);

    HI_MSG("set lvds phy attr successful!\n");
    return ret;
}

//**********************************************************************
// CMOS use phy1 and phy2
//**********************************************************************
static int mipi_set_cmos_attr(COMBO_DEV devno, input_mode_t input_mode)
{
    work_mode_t work_mode = get_work_mode(input_mode);
    short temp_lane_id[LANE_NUM_PER_LINK] = { -1, -1, -1, -1};

    mipi_drv_set_sys_work_mode(devno, input_mode);

    // dev0 use link0 and link1
    mipi_drv_set_phy_mode(0, work_mode, temp_lane_id);

    return HI_SUCCESS;
}

// check the common attr
static inline int check_comm_attr(combo_dev_attr_t* p_attr)
{
    if (p_attr->input_mode < INPUT_MODE_MIPI && p_attr->input_mode >= INPUT_MODE_BUTT)
    {
        HI_ERR("invalid input_mode(%d).\n", p_attr->input_mode);
        return HI_FAILURE;
    }

    if (p_attr->devno >= COMBO_MAX_DEV_NUM)
    {
        HI_ERR("invalid dev number(%d).\n", p_attr->devno);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

int mipi_set_combo_dev_attr(combo_dev_attr_t* p_attr)
{
    int ret = HI_FAILURE;

    if (HI_SUCCESS != check_comm_attr(p_attr))
    {
        return HI_FAILURE;
    }

    osal_mutex_lock(&hi_mipi_lock);

    switch (p_attr->input_mode)
    {
        case INPUT_MODE_LVDS:
        {
            ret = mipi_set_lvds_dev_attr(p_attr->devno, INPUT_MODE_LVDS, &p_attr->lvds_attr);
            break;
        }
        case INPUT_MODE_MIPI:
        {
            ret = mipi_set_mipi_dev_attr(p_attr->devno, &p_attr->mipi_attr);
            break;
        }
        case INPUT_MODE_CMOS:
        case INPUT_MODE_BT1120:
        {
            ret = mipi_set_cmos_attr(p_attr->devno, p_attr->input_mode);
            break;
        }
        case INPUT_MODE_SUBLVDS:
        {
            ret = mipi_set_lvds_dev_attr(p_attr->devno, INPUT_MODE_SUBLVDS, &p_attr->lvds_attr);
            break;
        }
        case INPUT_MODE_HISPI:
        {
            ret = mipi_set_lvds_dev_attr(p_attr->devno, INPUT_MODE_HISPI, &p_attr->lvds_attr);
            break;
        }
        default:
        {
            HI_ERR("invaild input mode\n");
            ret = HI_FAILURE;
            break;
        }
    }

    if (HI_SUCCESS == ret)
    {
        mipi_drv_set_phy_cfg_en(p_attr->devno, HI_TRUE);
    }

    osal_mutex_unlock(&hi_mipi_lock);
    return ret;
}

static int mipi_set_ouput_edge(unsigned int clk_edge)
{
    int ret = HI_SUCCESS;
    int i = 0;
    COMBO_DEV devno;
    COMBO_LINK link_id;

    if (0 != is_mipi_cfged())
    {
        HI_ERR("MIPI has not beed configured\n");
        return HI_FAILURE;
    }

    if ((CLK_UP_EDGE != clk_edge) && (CLK_DOWN_EDGE != clk_edge))
    {
        HI_ERR("invalid clk edge:%d, must be in {%d, %d}.\n", clk_edge,
               CLK_UP_EDGE, CLK_DOWN_EDGE);
        return HI_FAILURE;
    }

    osal_mutex_lock(&hi_mipi_lock);

    for (devno = 0; devno < COMBO_MAX_DEV_NUM; devno++)
    {
        for (i = 0; i < g_lane_info[devno].link_num; i++)
        {
            link_id = g_lane_info[devno].link_arr[i].link_idx;
            if (link_id < COMBO_MAX_LINK_NUM)
            {
                mipi_drv_set_output_clk_edge(i, clk_edge);
            }
        }
    }

    mipi_drv_set_phy_cfg_en(devno, HI_TRUE);

    osal_mutex_unlock(&hi_mipi_lock);

    return ret;
}

static int mipi_set_output_msb(unsigned int output_msb)
{
    int i = 0;
    COMBO_DEV devno;
    COMBO_LINK link_id;

    if (0 != is_mipi_cfged())
    {
        HI_ERR("MIPI has not beed configured\n");
        return HI_FAILURE;
    }

    if ((OUTPUT_NORM_MSB != output_msb) && (OUTPUT_REVERSE_MSB != output_msb))
    {
        HI_ERR("invalid output msb:%d. must be in [%d, %d].\n", output_msb,
               OUTPUT_NORM_MSB, OUTPUT_REVERSE_MSB);
        return HI_FAILURE;
    }

    osal_mutex_lock(&hi_mipi_lock);

    for (devno = 0; devno < COMBO_MAX_DEV_NUM; devno++)
    {
        for (i = 0; i < g_lane_info[devno].link_num; i++)
        {
            link_id = g_lane_info[devno].link_arr[i].link_idx;
            if (link_id < COMBO_MAX_LINK_NUM)
            {
                mipi_drv_set_output_msb(link_id, output_msb);
            }
        }

        mipi_drv_set_phy_cfg_en(devno, HI_TRUE);
    }

    osal_mutex_unlock(&hi_mipi_lock);

    return HI_SUCCESS;

}

//**********************************************************************
// set crop
//**********************************************************************
static int mipi_set_crop(COMBO_DEV devno, img_rect_t* p_rect)
{
    work_mode_t work_mode;
    link_info_s *plink_info = &g_lane_info[devno];

    if (!is_dev_cfged(devno))
    {
        HI_ERR("must config dev attr first\n");
        return HI_FAILURE;
    }

    work_mode = mipi_drv_get_sys_work_mode(devno);
    if (WORK_MODE_LVDS != work_mode && WORK_MODE_MIPI != work_mode)
    {
        HI_ERR("devno: %d, work mode: %d is not support crop.\n", devno, work_mode);
        return HI_FAILURE;
    }
    if (p_rect->x % 2 != 0 || p_rect->y % 2 != 0)
    {
        HI_ERR("crop x and y (%d, %d) must be even value\n", p_rect->x, p_rect->y);
        return HI_FAILURE;
    }
    if (p_rect->width % 2 != 0 || p_rect->height % 2 != 0)
    {
        HI_ERR("img_size(%d, %d) must be even value\n", p_rect->width, p_rect->height);
        return HI_FAILURE;
    }
    if (p_rect->x < 0 || p_rect->y < 0)
    {
        HI_ERR("crop x and y (%d, %d) must be great than 0\n", p_rect->x, p_rect->y);
        return HI_FAILURE;
    }
    if (p_rect->width < LVDS_MIN_WIDTH || p_rect->height < LVDS_MIN_HEIGHT)
    {
        HI_ERR("invalid img_size(%d, %d), can't be smaller than (%d, %d)\n",
               p_rect->width, p_rect->height, LVDS_MIN_WIDTH, LVDS_MIN_HEIGHT);
        return HI_FAILURE;
    }

    if (0 != p_rect->width % plink_info->total_lane_num)
    {
        HI_ERR("invalid width %d, must be muliples of %d\n", p_rect->width, plink_info->total_lane_num);
        return HI_FAILURE;
    }

    if (WORK_MODE_LVDS == work_mode)
    {
        mipi_drv_set_lvds_image_rect(devno, p_rect, g_lane_info[devno].total_lane_num);
        mipi_drv_set_lvds_crop_en(devno, HI_TRUE);
    }
    else if (WORK_MODE_MIPI == work_mode)
    {
        mipi_drv_set_mipi_image_rect(devno, p_rect);
        mipi_drv_set_mipi_crop_en(devno, HI_TRUE);
    }
    return HI_SUCCESS;
}

//**********************************************************************
// set the common mode voltage mode
//**********************************************************************
static int mipi_set_phy_cmvmode(COMBO_DEV devno, phy_cmv_e cmv_mode)
{
    int i;
    COMBO_LINK link_id;
    work_mode_t work_mode;
    int ret = HI_SUCCESS;

    if (COMBO_MAX_DEV_NUM <= devno)
    {
        HI_ERR("invalid dev number(%d).\n", devno);
        return HI_FAILURE;
    }

    if (HI_TRUE != is_dev_cfged(devno))
    {
        HI_ERR("MIPI device %d has not beed configured\n", devno);
        return HI_FAILURE;
    }

    if (cmv_mode < PHY_CMV_GE900MV || cmv_mode >= PHY_CMV_BUTT)
    {
        HI_ERR("invalid common mode voltage mode: %d, must be in [%d, %d)\n",
                cmv_mode, PHY_CMV_GE900MV, PHY_CMV_BUTT);
        return HI_FAILURE;
    }

    osal_mutex_lock(&hi_mipi_lock);

    work_mode = mipi_drv_get_sys_work_mode(devno);
    if (WORK_MODE_LVDS != work_mode && WORK_MODE_MIPI != work_mode)
    {
        osal_mutex_unlock(&hi_mipi_lock);
        HI_ERR("devno: %d, work mode: %d, not support set common voltage mode\n", devno, work_mode);
        return HI_FAILURE;
    }

    for (i = 0; i < g_lane_info[devno].link_num; i++)
    {
        link_id = g_lane_info[devno].link_arr[i].link_idx;
        if (link_id < COMBO_MAX_LINK_NUM)
        {
            mipi_drv_set_phy_cmv(link_id, cmv_mode);
        }
    }
    mipi_drv_set_phy_cfg_en(devno, HI_TRUE);

    osal_mutex_unlock(&hi_mipi_lock);
    return ret;
}

static long hi_mipi_ioctl(unsigned int cmd, unsigned long arg, void* private_data)
{
    unsigned int *argp = (unsigned int*)arg;
    unsigned int ret = HI_SUCCESS;

    switch (cmd)
    {
        case HI_MIPI_SET_DEV_ATTR:
        {
            combo_dev_attr_t*  pst_combo_dev_attr = HI_NULL;

            pst_combo_dev_attr = (combo_dev_attr_t*)argp;
            ret = mipi_set_combo_dev_attr(pst_combo_dev_attr);
            if (HI_SUCCESS == ret)
            {
                osal_memcpy(&g_dev_ctx[0].dev_attr, pst_combo_dev_attr, sizeof(combo_dev_attr_t));
            }
            break;
        }

        case HI_MIPI_SET_OUTPUT_CLK_EDGE:
        {
            unsigned int mipi_clk_edge;

            osal_memcpy(&mipi_clk_edge, argp, sizeof(unsigned int));
            ret = mipi_set_ouput_edge(mipi_clk_edge);
            break;
        }

        case HI_MIPI_SET_OUTPUT_MSB:
        {
            unsigned int  msb;

            osal_memcpy(&msb, argp, sizeof(unsigned int));
            ret = mipi_set_output_msb(msb);
            break;
        }

        case HI_MIPI_SET_PHY_CMVMODE:
        {
            phy_cmv_t phy_cmv;

            osal_memcpy(&phy_cmv, argp, sizeof(phy_cmv_t));
            ret = mipi_set_phy_cmvmode(phy_cmv.devno, phy_cmv.cmv_mode);
            break;
        }

        case HI_MIPI_RESET_SENSOR:
        {
            COMBO_DEV dev;

            osal_memcpy(&dev, argp, sizeof(COMBO_DEV));
            mipi_drv_reset_sensor(dev);
            break;
        }

        case HI_MIPI_UNRESET_SENSOR:
        {
            COMBO_DEV dev;

            osal_memcpy(&dev, argp, sizeof(COMBO_DEV));
            mipi_drv_unreset_sensor(dev);
            break;
        }

        case HI_MIPI_RESET_MIPI:
        {
            COMBO_DEV dev;

            osal_memcpy(&dev, argp, sizeof(COMBO_DEV));
            mipi_drv_core_reset(dev);
            break;
        }

        case HI_MIPI_UNRESET_MIPI:
        {
            COMBO_DEV dev;

            osal_memcpy(&dev, argp, sizeof(COMBO_DEV));
            mipi_drv_core_unreset(dev);
            mipi_drv_set_phy_cfg_en(dev, HI_TRUE);
            break;
        }

        case HI_MIPI_SET_CROP:
        {
            img_rect_t rect;

            osal_memcpy(&rect, argp, sizeof(img_rect_t));
            ret = mipi_set_crop(0, &rect);
            if (HI_SUCCESS == ret)
            {
                g_dev_ctx[0].bEnCrop = HI_TRUE;
                g_dev_ctx[0].stCropRect = rect;
            }
            break;
        }

        default:
        {
            HI_ERR("invalid himipi ioctl cmd\n");
            return HI_FAILURE;
            break;
        }
    }

    return ret;
}

static inline void mipi_int_statics(COMBO_DEV devno)
{
    unsigned int pkt_int1, pkt_int2;
    unsigned int frame_int;
    unsigned int mipi_ctrl_int;

    /* read clear */
    pkt_int1      = mipi_drv_get_mipi_pkt_int1(devno);
    pkt_int2      = mipi_drv_get_mipi_pkt_int2(devno);
    frame_int     = mipi_drv_get_mipi_frame_int(devno);
    mipi_ctrl_int = mipi_drv_get_mipi_ctrl_raw_int(devno);

    if (pkt_int1 || pkt_int2 || frame_int)
    {
        mipi_drv_clear_mipi_csi_int(devno, 0xffffffff);
    }
    if (mipi_ctrl_int)
    {
        mipi_drv_clear_mipi_ctrl_int(devno, mipi_ctrl_int);
    }

    // TODO: reset mipi
    #if 0
    if (int_raw || int2_raw)
    {

        *(volatile HI_U32*)OSAL_IO_ADDERSS(0x20670004) = 1;

        mipi_drv_set_csi2_rstn(0, 0);

        mipi_drv_set_csi2_rstn(0, 1);

        *(volatile HI_U32*)OSAL_IO_ADDERSS(0x20670004) = 0;

        *(volatile HI_U32*)OSAL_IO_ADDERSS( 0x20681214) = 0x1100;
    }
    #endif

    /* MIPI_FRAME_INTR_ST */
    if (frame_int & MIPI_FRAME_INT_DEF)
    {
        if (frame_int & MIPI_VC0_FRAME_CRC)
        {
            mipi_err_info[devno].VcFrameDataErrInfo.vc0_frame_err_cnt++;
        }
        if (frame_int & MIPI_VC1_FRAME_CRC)
        {
            mipi_err_info[devno].VcFrameDataErrInfo.vc1_frame_err_cnt++;
        }
        if (frame_int & MIPI_VC2_FRAME_CRC)
        {
            mipi_err_info[devno].VcFrameDataErrInfo.vc2_frame_err_cnt++;
        }
        if (frame_int & MIPI_VC3_FRAME_CRC)
        {
            mipi_err_info[devno].VcFrameDataErrInfo.vc3_frame_err_cnt++;
        }

        if (frame_int & MIPI_VC0_ORDER_ERR )
        {
            mipi_err_info[devno].VcFrameOrderErrInfo.vc0_order_err_cnt++;
        }
        if (frame_int & MIPI_VC1_ORDER_ERR )
        {
            mipi_err_info[devno].VcFrameOrderErrInfo.vc1_order_err_cnt++;
        }
        if (frame_int & MIPI_VC2_ORDER_ERR )
        {
            mipi_err_info[devno].VcFrameOrderErrInfo.vc2_order_err_cnt++;
        }
        if (frame_int & MIPI_VC3_ORDER_ERR )
        {
            mipi_err_info[devno].VcFrameOrderErrInfo.vc3_order_err_cnt++;
        }

        if (frame_int & MIPI_VC0_NO_MATCH )
        {
            mipi_err_info[devno].VcFrameNoMatchInfo.vc0_nomatch_cnt++;
        }
        if (frame_int & MIPI_VC1_NO_MATCH )
        {
            mipi_err_info[devno].VcFrameNoMatchInfo.vc1_nomatch_cnt++;
        }
        if (frame_int & MIPI_VC2_NO_MATCH )
        {
            mipi_err_info[devno].VcFrameNoMatchInfo.vc2_nomatch_cnt++;
        }
        if (frame_int & MIPI_VC3_NO_MATCH )
        {
            mipi_err_info[devno].VcFrameNoMatchInfo.vc3_nomatch_cnt++;
        }
    }

    /* MIPI_PKT_INTR_ST */
    if (pkt_int1 & MIPI_PKT_INT1_DEF)
    {
        if (pkt_int1 & MIPI_PKT_HEADER_ERR)
        {
            mipi_err_info[devno].VcPktHeaderErrInfo.header_err_cnt++;
        }
        if (pkt_int1 & MIPI_VC0_PKT_DATA_CRC)
        {
            mipi_err_info[devno].VcPktDataErrInfo.vc0_frame_err_cnt++;
        }
        if (pkt_int1 & MIPI_VC1_PKT_DATA_CRC)
        {
            mipi_err_info[devno].VcPktDataErrInfo.vc1_frame_err_cnt++;
        }
        if (pkt_int1 & MIPI_VC2_PKT_DATA_CRC)
        {
            mipi_err_info[devno].VcPktDataErrInfo.vc2_frame_err_cnt++;
        }
        if (pkt_int1 & MIPI_VC3_PKT_DATA_CRC)
        {
            mipi_err_info[devno].VcPktDataErrInfo.vc3_frame_err_cnt++;
        }
    }

    /* MIPI_PKT_INTR2_ST */
    if (pkt_int2 & MIPI_PKT_INT2_DEF)
    {
        if (pkt_int2 & MIPI_VC0_PKT_INVALID_DT)
        {
            mipi_err_info[devno].VcPktDTErrInfo.vc0_dt_err_cnt++;
        }
        if (pkt_int2 & MIPI_VC1_PKT_INVALID_DT)
        {
            mipi_err_info[devno].VcPktDTErrInfo.vc1_dt_err_cnt++;
        }
        if (pkt_int2 & MIPI_VC2_PKT_INVALID_DT)
        {
            mipi_err_info[devno].VcPktDTErrInfo.vc2_dt_err_cnt++;
        }
        if (pkt_int2 & MIPI_VC3_PKT_INVALID_DT)
        {
            mipi_err_info[devno].VcPktDTErrInfo.vc3_dt_err_cnt++;
        }

        if (pkt_int2 & MIPI_VC0_PKT_HEADER_ECC)
        {
            mipi_err_info[devno].VcPktHeaderErrInfo.vc0_header_correct_cnt++;
        }
        if (pkt_int2 & MIPI_VC1_PKT_HEADER_ECC)
        {
            mipi_err_info[devno].VcPktHeaderErrInfo.vc1_header_correct_cnt++;
        }
        if (pkt_int2 & MIPI_VC2_PKT_HEADER_ECC)
        {
            mipi_err_info[devno].VcPktHeaderErrInfo.vc2_header_correct_cnt++;
        }
        if (pkt_int2 & MIPI_VC3_PKT_HEADER_ECC)
        {
            mipi_err_info[devno].VcPktHeaderErrInfo.vc3_header_correct_cnt++;
        }
    }

    if (mipi_ctrl_int & MIPI_CTRL_INT_DEF)
    {
        if (mipi_ctrl_int & CMD_FIFO_READ_ERR)
        {
            mipi_err_info[devno].mipiCtrlErrInfo.cmd_fifo_rerr_cnt++;
        }
        if (mipi_ctrl_int & DATA_FIFO_READ_ERR)
        {
            mipi_err_info[devno].mipiCtrlErrInfo.data_fifo_rerr_cnt++;
        }
        if (mipi_ctrl_int & CMD_FIFO_WRITE_ERR)
        {
            mipi_err_info[devno].mipiCtrlErrInfo.cmd_fifo_werr_cnt++;
        }
        if (mipi_ctrl_int & DATA_FIFO_WRITE_ERR)
        {
            mipi_err_info[devno].mipiCtrlErrInfo.data_fifo_werr_cnt++;
        }
    }
}

static inline void lvds_int_statics(COMBO_DEV devno)
{
    int i;
    unsigned int lvds_ctrl_int;
    unsigned int mask = 0;

    lvds_ctrl_int = mipi_drv_get_lvds_ctrl_raw_int(devno);

    if (lvds_ctrl_int)
    {
        mipi_drv_clear_lvds_ctrl_int(devno, lvds_ctrl_int);
    }

    if (0 == devno)
    {
        mask = LVDS_CTRL0_INT_DEF;
    }

    if (lvds_ctrl_int & mask)
    {
        if (lvds_ctrl_int & LVDS_STAT_ERR)
        {
            lvds_err_info[devno].lvds_stat_err_cnt++;
        }

        for (i = 0; i < COMBO_MAX_LANE_NUM; i++)
        {
            if (lvds_ctrl_int & (LANE0_SYNC_ERR << i))
            {
                lvds_err_info[devno].lane_sync_err_cnt[i]++;
            }
        }

        if (lvds_ctrl_int & LINK0_WRITE_ERR)
        {
            lvds_err_info[devno].link_write_err_cnt[0]++;
        }
        if (lvds_ctrl_int & LINK0_READ_ERR)
        {
            lvds_err_info[devno].link_read_err_cnt[0]++;
        }
        if (lvds_ctrl_int & LINK0_VSYNC_ERR)
        {
            lvds_err_info[devno].link_vsync_err_cnt[0]++;
        }
        if (lvds_ctrl_int & LINK0_HSYNC_ERR)
        {
            lvds_err_info[devno].link_hsync_err_cnt[0]++;
        }

    }
}

static void align_int_statis(COMBO_DEV devno)
{
    unsigned int align_int;
    unsigned int mask = 0;
    int i;

    align_int = mipi_drv_get_align_raw_int(devno);

    if (align_int)
    {
       mipi_drv_clear_align_int(devno, align_int);
    }

    if (0 == devno)
    {
        mask = ALIGN0_INT_DEF;
    }

    if (align_int & mask)
    {
        if (align_int & ALIGN_FIFO_FULL_ERR)
        {
            align_err_info[devno].fifo_full_err_cnt++;
        }
        for (i = 0; i < COMBO_MAX_LANE_NUM; i++)
        {
            if (align_int & (ALIGN_LANE0_ERR << i))
            {
                align_err_info[devno].lane_err_cnt[i]++;
            }
        }
    }
}

// MIPI_CIL_INT, FSM time out and escape seq
void mipi_cil_int_statis(COMBO_LINK link_id)
{
    unsigned int link_int_status;

    link_int_status = mipi_drv_get_cil_raw_int(link_id);

    if (link_int_status)
    {
        mipi_drv_clear_cil_int(link_id, link_int_status);

        if (link_int_status & MIPI_ESC_CLK)
        {
            phy_int_state[link_id].clk_fsm_escape_cnt++;
        }

        if (link_int_status & MIPI_ESC_D0)
        {
            phy_int_state[link_id].d0_fsm_escape_cnt++;
        }
        if (link_int_status & MIPI_ESC_D1)
        {
            phy_int_state[link_id].d1_fsm_escape_cnt++;
        }
        if (link_int_status & MIPI_ESC_D2)
        {
            phy_int_state[link_id].d2_fsm_escape_cnt++;
        }
        if (link_int_status & MIPI_ESC_D3)
        {
            phy_int_state[link_id].d3_fsm_escape_cnt++;
        }

        if (link_int_status & MIPI_TIMEOUT_CLK)
        {
            phy_int_state[link_id].clk_fsm_timeout_cnt++;
        }

        if (link_int_status & MIPI_TIMEOUT_D0)
        {
            phy_int_state[link_id].d0_fsm_timeout_cnt++;
        }
        if (link_int_status & MIPI_TIMEOUT_D1)
        {
            phy_int_state[link_id].d1_fsm_timeout_cnt++;
        }
        if (link_int_status & MIPI_TIMEOUT_D2)
        {
            phy_int_state[link_id].d2_fsm_timeout_cnt++;
        }
        if (link_int_status & MIPI_TIMEOUT_D3)
        {
            phy_int_state[link_id].d3_fsm_timeout_cnt++;
        }
    }
}

static int mipi_interrupt_route(int irq, void* dev_id)
{
    int ret = OSAL_IRQ_NONE;
    int i = 0;
    int s32MipiDev = 0;

    /* phy int statistic */
    for (i = 0; i < COMBO_MAX_LINK_NUM; i++)
    {
        mipi_cil_int_statis(i);
    }

    for (i = 0; i < COMBO_MAX_DEV_NUM; i++)
    {
        mipi_int_statics(i);
        lvds_int_statics(i);
        align_int_statis(i);
    }

    if( (INPUT_MODE_MIPI == g_dev_ctx[s32MipiDev].dev_attr.input_mode) )
    {
        mipi_exception_handling(s32MipiDev);
    }

    ret = OSAL_IRQ_HANDLED;
    return ret;
}

static int hi_mipi_open(void* data)
{
    return 0;
}

static int hi_mipi_release(void *data)
{
    return 0;
}

static osal_fileops_t hi_mipi_fops =
{
    .unlocked_ioctl = hi_mipi_ioctl,
    .open           = hi_mipi_open,
    .release        = hi_mipi_release,
};


#ifdef CONFIG_HISI_SNAPSHOT_BOOT
static int Mipi_freeze(osal_dev_t *pdev)
{
    osal_printk("%s  %d\n", __FUNCTION__, __LINE__);
    return 0;
}

static int Mipi_restore(osal_dev_t *pdev)
{
    osal_printk("%s  %d\n", __FUNCTION__, __LINE__);
    hi_mipi_init();
    return 0;
}
#else
static int Mipi_freeze(osal_dev_t *pdev)
{
    return HI_SUCCESS;
}

static int Mipi_restore(osal_dev_t *pdev)
{
    return HI_SUCCESS;
}
#endif

static osal_pmops_t stHiMipiDrvops =
{
    .pm_freeze = Mipi_freeze,
    .pm_restore  = Mipi_restore
};

#ifdef MIPI_ENABLE_PROC_FS

static void proc_show_combo_device(osal_proc_entry_t *s)
{
    COMBO_DEV devno;
    COMBO_LINK link_id;
    work_mode_t work_mode;
    raw_data_type_e raw_data_type;

    osal_seq_printf(s, "\n-----Combo DEV ATTR-----------------------------------------------------------------\n");

    // Combo Device info
    osal_seq_printf(s, "%6s%9s%12s%8s%10s%14s%12s\n",
            "Devno", "WorkMode", "RawDataType",
            "LinkIdx", "LaneCount", "LaneId", "PhyData");
    for (devno = 0; devno < COMBO_MAX_DEV_NUM; devno++)
    {
        work_mode = mipi_drv_get_sys_work_mode(devno);
        if (WORK_MODE_MIPI == work_mode)
        {
            raw_data_type = mipi_drv_get_dt(devno, 0);
        }
        else if (WORK_MODE_LVDS == work_mode)
        {
            raw_data_type = mipi_drv_get_lvds_raw_data_type(devno);
        }
        else
        {
            raw_data_type = RAW_DATA_BUTT;
        }


        if (WORK_MODE_CMOS == work_mode)
        {
            osal_seq_printf(s, "%6d%9s%12s%8s%10s%14s%12s\n",
                    devno,
                    mipi_get_work_mode_name(work_mode),
                    "N/A", "N/A", "N/A", "N/A", "N/A");
        }
        else
        {
            for (link_id = 0; link_id < g_lane_info[devno].link_num; link_id++)
            {
                osal_seq_printf(s, "%6d%9s%12s%8d%10d   %2d,%2d,%2d,%2d%#12x\n",
                        devno,
                        mipi_get_work_mode_name(work_mode),
                        mipi_get_raw_data_type_name(raw_data_type),
                        g_lane_info[devno].link_arr[link_id].link_idx,
                        g_lane_info[devno].link_arr[link_id].lane_num,
                        g_lane_info[devno].link_arr[link_id].lane_idx[0],
                        g_lane_info[devno].link_arr[link_id].lane_idx[1],
                        g_lane_info[devno].link_arr[link_id].lane_idx[2],
                        g_lane_info[devno].link_arr[link_id].lane_idx[3],
                        mipi_drv_get_data(link_id));
            }
        }
    }

    osal_seq_printf(s, "\n-----Mipi Crop Info-----------------------------------------------------------------\n");

    // mipi crop info
    osal_seq_printf(s, "%8s%10s%8s%8s%8s%8s\n",
            "Devno", "bEnCrop", "ImgX", "ImgY", "ImgW", "ImgH");

    for (devno = 0; devno < COMBO_MAX_DEV_NUM; devno++)
    {
        osal_seq_printf(s, "%8d%10d%8d%8d%8d%8d\n",
                        devno,
                        g_dev_ctx[0].bEnCrop,
                        g_dev_ctx[0].stCropRect.x,
                        g_dev_ctx[0].stCropRect.y,
                        g_dev_ctx[0].stCropRect.width,
                        g_dev_ctx[0].stCropRect.height);
    }
}

static void proc_show_mipi_attr(osal_proc_entry_t *s, COMBO_DEV devno_array[], int mipi_cnt)
{
    img_size_t image_size;
    unsigned int vc_num;
    unsigned int devno_idx;

    osal_seq_printf(s, "-----mipi detect info----------------------------------------------------\n");
    osal_seq_printf(s, "%6s%3s%8s%8s\n", "Devno", "VC", "width", "height");

    if (COMBO_MAX_DEV_NUM < mipi_cnt)
    {
        return;
    }
    for (devno_idx = 0; devno_idx < mipi_cnt; devno_idx++)
    {
        for (vc_num = 0; vc_num < WDR_VC_NUM; vc_num++)
        {
            mipi_drv_get_mipi_imgsize_statis(devno_array[devno_idx], vc_num, &image_size);
            osal_seq_printf(s, "%6d%3d%8d%8d\n",
                    devno_array[devno_idx], vc_num, image_size.width, image_size.height);
        }
    }
}

static void proc_show_lvds_attr(osal_proc_entry_t *s, COMBO_DEV devno_array[], int lvds_cnt)
{
    img_size_t image_size;
    unsigned int ef;
    unsigned int devno_idx;
    char* ef_name[4] = {"LEF", "SEF1", "SEF2", "SEF3"};

    osal_seq_printf(s, "\n-----lvds detect info----------------------------------------------------\n");
    if (COMBO_MAX_DEV_NUM < lvds_cnt)
    {
        return;
    }
    for (devno_idx = 0; devno_idx < lvds_cnt; devno_idx++)
    {
        osal_seq_printf(s, "%6s%10s%8s%8s\n", "Devno", "WDR_Frame", "width", "height");

        for (ef = 0; ef < WDR_VC_NUM; ef++)
        {
            mipi_drv_get_lvds_imgsize_statis(devno_array[devno_idx], ef, &image_size);
            osal_seq_printf(s, "%6d%10s%8d%8d\n",
                    devno_array[devno_idx],
                    ef_name[ef], image_size.width, image_size.height);
        }
    }
}

static void proc_show_mipi_int_err(osal_proc_entry_t *s, COMBO_DEV devno_array[], int mipi_cnt)
{
    int devno_idx;
    COMBO_DEV devno;

    osal_seq_printf(s, "-----MIPI int error info-----------------------------------------------------------\n\n");
    osal_seq_printf(s, "%6s%7s%7s%7s%7s%12s%12s%12s%12s%11s%11s%11s%11s\n",
            "Devno",
            "vc0CRC", "vc1CRC", "vc2CRC", "vc3CRC",
            "vc0OrderErr", "vc1OrderErr", "vc2OrderErr", "vc3OrderErr",
            "vc0NMatCnt", "vc1NMatCnt", "vc2NMatCnt", "vc3NMatCnt");
    for (devno_idx = 0; devno_idx < mipi_cnt; devno_idx++)
    {
        devno = devno_array[devno_idx];
        osal_seq_printf(s, "%6d%7d%7d%7d%7d%12d%12d%12d%12d%11d%11d%11d%11d\n\n",
                devno,
                mipi_err_info[devno].VcFrameDataErrInfo.vc0_frame_err_cnt,
                mipi_err_info[devno].VcFrameDataErrInfo.vc1_frame_err_cnt,
                mipi_err_info[devno].VcFrameDataErrInfo.vc2_frame_err_cnt,
                mipi_err_info[devno].VcFrameDataErrInfo.vc3_frame_err_cnt,
                mipi_err_info[devno].VcFrameOrderErrInfo.vc0_order_err_cnt,
                mipi_err_info[devno].VcFrameOrderErrInfo.vc1_order_err_cnt,
                mipi_err_info[devno].VcFrameOrderErrInfo.vc2_order_err_cnt,
                mipi_err_info[devno].VcFrameOrderErrInfo.vc3_order_err_cnt,
                mipi_err_info[devno].VcFrameNoMatchInfo.vc0_nomatch_cnt,
                mipi_err_info[devno].VcFrameNoMatchInfo.vc1_nomatch_cnt,
                mipi_err_info[devno].VcFrameNoMatchInfo.vc2_nomatch_cnt,
                mipi_err_info[devno].VcFrameNoMatchInfo.vc3_nomatch_cnt);
    }

    osal_seq_printf(s, "%6s%8s%8s%8s%8s%8s%9s%9s%9s%9s\n",
            "Devno",
            "HCntErr",
            "vc0HECC","vc1HECC","vc2HECC","vc3HECC",
            "vc0DtErr", "vc1DtErr", "vc2DtErr", "vc3DtErr");
    for (devno_idx = 0; devno_idx < mipi_cnt; devno_idx++)
    {
        devno = devno_array[devno_idx];
        osal_seq_printf(s, "%6d%8d%8d%8d%8d%8d%9d%9d%9d%9d\n\n",
                devno,
                mipi_err_info[devno].VcPktHeaderErrInfo.header_err_cnt,
                mipi_err_info[devno].VcPktHeaderErrInfo.vc0_header_correct_cnt,
                mipi_err_info[devno].VcPktHeaderErrInfo.vc1_header_correct_cnt,
                mipi_err_info[devno].VcPktHeaderErrInfo.vc2_header_correct_cnt,
                mipi_err_info[devno].VcPktHeaderErrInfo.vc3_header_correct_cnt,
                mipi_err_info[devno].VcPktDTErrInfo.vc0_dt_err_cnt,
                mipi_err_info[devno].VcPktDTErrInfo.vc1_dt_err_cnt,
                mipi_err_info[devno].VcPktDTErrInfo.vc2_dt_err_cnt,
                mipi_err_info[devno].VcPktDTErrInfo.vc3_dt_err_cnt);
    }

    osal_seq_printf(s, "%6s%14s%15s%14s%15s\n",
            "Devno",
            "CMD_FIFO_RERR", "DATA_FIFO_RERR", "CMD_FIFO_WERR", "DATA_FIFO_WERR");
    for (devno_idx = 0; devno_idx < mipi_cnt; devno_idx++)
    {
        devno = devno_array[devno_idx];
        osal_seq_printf(s, "%6d%14d%15d%14d%15d\n\n",
                devno,
                mipi_err_info[devno].mipiCtrlErrInfo.cmd_fifo_rerr_cnt,
                mipi_err_info[devno].mipiCtrlErrInfo.data_fifo_rerr_cnt,
                mipi_err_info[devno].mipiCtrlErrInfo.cmd_fifo_werr_cnt,
                mipi_err_info[devno].mipiCtrlErrInfo.data_fifo_werr_cnt);
    }
}

static void proc_show_cil_int_err(osal_proc_entry_t *s)
{
    COMBO_LINK link_id;

    osal_seq_printf(s, "\n-----fsm timeout and escape info---------------------------------------------\n");

    osal_seq_printf(s, "%5s%11s%10s%12s%12s%12s%10s%10s%10s%10s%10s\n",
            "link",
            "clkTOutCnt",
            "d0TOutCnt", "d1TOutCnt", "d2TOutCnt", "d3TOutCnt",
            "clkEscCnt",
            "d0EscCnt", "d1EscCnt", "d2EscCnt", "d3EscCnt");

    for (link_id = 0; link_id < COMBO_MAX_LINK_NUM; link_id++)
    {
        osal_seq_printf(s, "%5d%11d%10d%12d%12d%12d%10d%10d%10d%10d%10d\n",
                link_id,
                phy_int_state[link_id].clk_fsm_timeout_cnt,
                phy_int_state[link_id].d0_fsm_timeout_cnt,
                phy_int_state[link_id].d1_fsm_timeout_cnt,
                phy_int_state[link_id].d2_fsm_timeout_cnt,
                phy_int_state[link_id].d3_fsm_timeout_cnt,

                phy_int_state[link_id].clk_fsm_escape_cnt,
                phy_int_state[link_id].d0_fsm_escape_cnt,
                phy_int_state[link_id].d1_fsm_escape_cnt,
                phy_int_state[link_id].d2_fsm_escape_cnt,
                phy_int_state[link_id].d3_fsm_escape_cnt);
    }
}

static void proc_show_lvds_int_err(osal_proc_entry_t *s, COMBO_DEV devno_array[], int lvds_cnt)
{
    COMBO_DEV devno;
    unsigned int devno_idx;
    unsigned int lane_id;

    osal_seq_printf(s, "\n-----LVDS/SUBLVDS/HISPI phy and lane state info--------------------------------------\n");

    osal_seq_printf(s, "%6s%8s%10s%10s%13s%13s\n",
            "Devno", "StatErr",
            "Link0WErr",
            "Link0RErr",
            "Link0HSynErr",
            "Link0VSynErr");
    if (COMBO_MAX_DEV_NUM < lvds_cnt)
    {
        return;
    }
    for (devno_idx = 0; devno_idx < lvds_cnt; devno_idx++)
    {
        devno = devno_array[devno_idx];
        osal_seq_printf(s, "%6d%7d%10d%10d%13d%13d\n",
                devno,
                lvds_err_info[devno].lvds_stat_err_cnt,
                lvds_err_info[devno].link_write_err_cnt[0],
                lvds_err_info[devno].link_read_err_cnt[0],
                lvds_err_info[devno].link_hsync_err_cnt[0],
                lvds_err_info[devno].link_vsync_err_cnt[0]);
    }

    osal_seq_printf(s, "%6s%10s%10s%10s%10s\n",
            "Devno",
            "Lane0Err", "Lane1Err", "Lane2Err", "Lane3Err");
    for (devno_idx = 0; devno_idx < lvds_cnt; devno_idx++)
    {
        devno = devno_array[devno_idx];
        osal_seq_printf(s, "%6d", devno);
        for (lane_id = 0; lane_id < COMBO_MAX_LANE_NUM; lane_id++)
        {
            osal_seq_printf(s, "%10d", lvds_err_info[devno].lane_sync_err_cnt[lane_id]);
        }
        osal_seq_printf(s, "\n");
    }
}

static void proc_show_align_int_err(osal_proc_entry_t *s)
{
    COMBO_DEV devno;
    unsigned int lane_id;
    unsigned int lane_nums = 0;

    osal_seq_printf(s, "\n-----ALING Err info--------------------------------------\n");

    for (devno = 0; devno < COMBO_MAX_DEV_NUM; devno++)
    {
        if (g_lane_info[devno].total_lane_num <= 0)
        {
            continue;
        }

        if (0 == devno)
        {
            lane_nums = COMBO_MAX_LANE_NUM;
            osal_seq_printf(s, "%6s%13s%10s%10s%10s%10s\n",
                    "Devno", "FIFO_FullErr",
                    "Lane0Err", "Lane1Err", "Lane2Err", "Lane3Err");
        }

        osal_seq_printf(s, "%6d%13d", devno, align_err_info[devno].fifo_full_err_cnt);
        for (lane_id = 0; lane_id < lane_nums; lane_id++)
        {
            osal_seq_printf(s, "%10d", align_err_info[devno].lane_err_cnt[lane_id]);
        }
        osal_seq_printf(s, "\n");
    }
}

static int mipi_proc_show(osal_proc_entry_t *s)
{
    work_mode_t work_mode;
    int devno_idx;
    int mipi_cnt = 0;
    int lvds_cnt = 0;
    COMBO_DEV devno_mipi[COMBO_MAX_DEV_NUM] = {0};
    COMBO_DEV devno_lvds[COMBO_MAX_DEV_NUM] = {0};

    for (devno_idx = 0; devno_idx < COMBO_MAX_DEV_NUM; devno_idx++)
    {
        work_mode = mipi_drv_get_sys_work_mode(devno_idx);

        if (WORK_MODE_CMOS == work_mode)
        {
            continue;
        }

        // MIPI SBLVDS LVDS HiSPi
        if (g_lane_info[devno_idx].total_lane_num <= 0)
        {
            continue;
        }

        if (work_mode == WORK_MODE_MIPI)
        {
            devno_mipi[mipi_cnt] = devno_idx;
            mipi_cnt++;
        }
        else if (work_mode == WORK_MODE_LVDS)
        {
            devno_lvds[lvds_cnt] = devno_idx;
            lvds_cnt++;
        }
    }

    osal_seq_printf(s, "\nModule: [MIPI], Build Time: [%s, %s]\n", __DATE__, __TIME__);

    proc_show_combo_device(s);

    // MIPI Attr:
    if (mipi_cnt > 0)
    {
        proc_show_mipi_attr(s, devno_mipi, mipi_cnt);
    }

    if (lvds_cnt > 0)
    {
        proc_show_lvds_attr(s, devno_lvds, lvds_cnt);
    }

    // int status
    proc_show_cil_int_err(s);

    if (mipi_cnt > 0)
    {
        proc_show_mipi_int_err(s, devno_mipi, mipi_cnt);
    }

    if (lvds_cnt > 0)
    {
        proc_show_lvds_int_err(s, devno_lvds, lvds_cnt);
    }

    proc_show_align_int_err(s);
    return 0;
}
#endif

int mipi_init(void)
{
    int ret = 0;
#ifdef MIPI_ENABLE_PROC_FS
    osal_proc_entry_t *mipi_entry;
#endif

    HI_MSG("mipi_init\n");

    s_pstHiMipiDevice = osal_createdev(MIPI_DEV_NAME);
    if (NULL == s_pstHiMipiDevice)
    {
        HI_ERR("mipi: create device failed\n");
        return -1;
    }
    s_pstHiMipiDevice->fops  = &hi_mipi_fops;
    s_pstHiMipiDevice->minor = HIMEDIA_DYNAMIC_MINOR;
    s_pstHiMipiDevice->osal_pmops = &stHiMipiDrvops;
    ret = osal_registerdevice(s_pstHiMipiDevice);
    if (ret)
    {
        osal_destroydev(s_pstHiMipiDevice);
        s_pstHiMipiDevice = NULL;
        HI_ERR("Kernel: register mipi device failed!");
        return -1;
    }

    ret = mipi_drv_init_reg();
    if (HI_SUCCESS != ret)
    {
        HI_ERR("mipi: failed to init register(%d)\n", ret);
        goto MIPI_INIT_FAIL1;
    }

    ret = osal_request_irq(mipi_irq, mipi_interrupt_route, NULL, "mipi0_int", mipi_interrupt_route);
    if (0 != ret)
    {
        HI_ERR("mipi: failed to register IRQ(%d)\n", mipi_irq);
        goto MIPI_INIT_FAIL1;
    }

#ifdef MIPI_ENABLE_PROC_FS
    mipi_entry = osal_create_proc_entry(MIPI_PROC_NAME, NULL);
    if (NULL == mipi_entry)
    {
        HI_ERR("mipi: can't create %s.\n", MIPI_PROC_NAME);
        ret = -1;
        goto MIPI_INIT_FAIL2;
    }
    mipi_entry->read = mipi_proc_show;
#endif

    /* mipi and lvds init*/
    hi_mipi_init();
    osal_mutex_init(&hi_mipi_lock);

    osal_printk("load hi_mipi driver successful!\n");
    return 0;

MIPI_INIT_FAIL2:
    osal_free_irq(mipi_irq, mipi_interrupt_route);

MIPI_INIT_FAIL1:
    osal_deregisterdevice(s_pstHiMipiDevice);
    osal_destroydev(s_pstHiMipiDevice);
    s_pstHiMipiDevice = NULL;

    return ret;
}

void mipi_exit(void)
{
#ifdef MIPI_ENABLE_PROC_FS
    osal_remove_proc_entry(MIPI_PROC_NAME, NULL);
#endif

    osal_free_irq(mipi_irq, mipi_interrupt_route);

    osal_deregisterdevice(s_pstHiMipiDevice);
    osal_destroydev(s_pstHiMipiDevice);
    s_pstHiMipiDevice = NULL;

    osal_mutex_destory(&hi_mipi_lock);
    osal_printk("unload hi_mipi driver successful!\n");
}

module_init(mipi_init);
module_exit(mipi_exit);
MODULE_LICENSE("GPL");
