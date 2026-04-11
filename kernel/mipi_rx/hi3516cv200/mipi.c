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


#include <linux/device.h>
#include <linux/version.h>
#include <linux/pagemap.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/delay.h>
//#include <linux/miscdevice.h>
#include <linux/moduleparam.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <asm/io.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <asm/uaccess.h>

#include "hi_mipi.h"
#include "mipi_reg.h"
#include "mipi_hal.h"
#include "hi_type.h"

#include "himedia.h"

/****************************************************************************
 * MACRO DEFINITION                                                         *
 ****************************************************************************/
static struct himedia_device s_stHiMipiDevice;

#define OSDRV_MODULE_VERSION_STRING "HISI_mipi @HiMPP"

#define MIPI_DEV_NAME	    "hi_mipi"
#define MIPI_PROC_NAME      "driver/hi_mipi"

#define MIPI_IRQ            28
#define MIPI_DELAY_TIME     20

#define MIPI_ENABLE_PROC_FS

#define MAX2(x,y)       ( (x)>(y) ? (x):(y) )
#define MIN2(x,y)       ( (x)<(y) ? (x):(y) )


/****************************************************************************
 * Struct DEFINITION                                                        *
 ****************************************************************************/
typedef  struct hi_MIPI_VC_FREAM_ERR_INFO_S
{
    unsigned int  vc0_frame_err_cnt;
    unsigned int  vc1_frame_err_cnt;
    unsigned int  vc2_frame_err_cnt;
    unsigned int  vc3_frame_err_cnt;
} MIPI_VC_FREAM_ERR_INFO_S;

typedef  struct hi_MIPI_VC_FRAME_ORDER_ERR_INFO_S
{
    unsigned int  vc0_order_err_cnt;
    unsigned int  vc1_order_err_cnt;
    unsigned int  vc2_order_err_cnt;
    unsigned int  vc3_order_err_cnt;
} MIPI_VC_FRAME_ORDER_ERR_INFO_S;

typedef struct hi_MIPI_FRAME_NO_MATCH_INFO_S
{
    unsigned int    vc0_nomatch_cnt;
    unsigned int 	vc1_nomatch_cnt;
    unsigned int 	vc2_nomatch_cnt;
    unsigned int 	vc3_nomatch_cnt;
} MIPI_VC_FRAME_NO_MATCH_INFO;

typedef struct hi_MIPI_VC_DT_ERR_INFO_S
{
    unsigned int   vc0_dt_err_cnt;
    unsigned int   vc1_dt_err_cnt;
    unsigned int   vc2_dt_err_cnt;
    unsigned int   vc3_dt_err_cnt;
} MIPI_VC_DT_ERR_INFO_S;

typedef struct hi_LVDS_SYNC_INTR_INFO_STAT
{
    unsigned int lane0_sync_err_cnt;
    unsigned int lane1_sync_err_cnt;
    unsigned int lane2_sync_err_cnt;
    unsigned int lane3_sync_err_cnt;
    unsigned int lane4_sync_err_cnt;
    unsigned int lane5_sync_err_cnt;
    unsigned int lane6_sync_err_cnt;
    unsigned int lane7_sync_err_cnt;
    unsigned int phy0_line_sync_err_cnt;
    unsigned int phy0_frame_sync_err_cnt;
    unsigned int phy1_line_sync_err_cnt;
    unsigned int phy1_frame_sync_err_cnt;
} LVDS_SYNC_INTR_INFO_STAT;

/*mipi frame err infomation*/
typedef struct hi_MIPI_FRAME_DBG_INFO_S
{
    unsigned int                    u32HeaderErrCnt;
    MIPI_VC_FREAM_ERR_INFO_S        VcFrameErrInfo;
    MIPI_VC_FRAME_ORDER_ERR_INFO_S  VcFrameOrderErrInfo;
    MIPI_VC_FRAME_NO_MATCH_INFO     VcFrameNoMatchInfo;
    MIPI_VC_DT_ERR_INFO_S           VcDTErrInfo;
    LVDS_SYNC_INTR_INFO_STAT        PhySyncErrInfo;
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

/****************************************************************************
 * Variables DEFINITION                                                     *
 ****************************************************************************/
MIPI_FRAME_DBG_INFO_S  frame_dbg_info[COMBO_MAX_LINK_NUM];
mipi_phy_int_state     phy_int_state[COMBO_MAX_LINK_NUM];

int mipi_irq = -1;

static DEFINE_MUTEX(hi_mipi_lock);

link_info_s g_lane_info;

/**************************************************************
valid land id in one link:
    (0 2 4 6), (1 3 5 7), (0 1 2 3), (4 5 6 7) in one link
and,
    (0 2 4 6), (1 3 5 7): disorder
    (0 1 2 3), (4 5 6 7): order
**************************************************************/
#define VALID_NUM       4

char g_valid_lane_arr[VALID_NUM][LANE_NUM_PER_LINK] = {{0, 2, 4, 6},
                                                        {1, 3, 5, 7},
                                                        {0, 1, 2, 3},
                                                        {4, 5, 6, 7}
                                                       };

#define IS_DISORDER(idx)        (((idx) == 0) || (idx) == 1)



/****************************************************************************
 * Function DEFINITION                                                      *
 ****************************************************************************/
static inline void mipi_write_reg32(unsigned int addr,
                                    unsigned int value,
                                    unsigned int mask)
{
    unsigned int t;

    t = readl(addr);
    t &= ~mask;
    t |= value & mask;
    writel(t, addr);
}

/******************************************************************************
* asic related
******************************************************************************/
static work_mode_t get_work_mode(input_mode_t input_mode)
{
    if (INPUT_MODE_MIPI == input_mode)
    {
        return WORK_MODE_MIPI;
    }
    else if (INPUT_MODE_CMOS_18V == input_mode)
    {
        return WORK_MODE_CMOS_18V;
    }
    else if (INPUT_MODE_CMOS_33V == input_mode)
    {
        return WORK_MODE_CMOS_33V;
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

//******************************************************************************
//  Function    : set sys ctrl reg
//  Description : 20050004[22:20] mipi-0, lvds-1, cmos_1.8v-2,
//                                cmos_3.3v-3, bypass_phy-4
//  Input       : input_mode_t input_mode
//  Return      : void
//******************************************************************************
static void mipi_set_sys_work_mode(input_mode_t input_mode)
{
    int i = 0;
    /* ASIC related */
    unsigned int sys_reg;
    unsigned int ctl_mask = 0x7 << 20;

    HI_BOOL reset = HI_TRUE;
    unsigned int mipi_asic_reg;

    sys_reg = (unsigned int)ioremap_nocache(0x20120004, 0x4);
    if (!sys_reg)
    {
        printk("%s:ioremap failed!\n", __func__);
        return;
    }


    /* set sys reg via input mode */
    if (INPUT_MODE_MIPI == input_mode)
    {
        mipi_write_reg32(sys_reg, 0x0 << 20, ctl_mask);
    }
    else if (INPUT_MODE_CMOS_18V == input_mode)
    {
        mipi_write_reg32(sys_reg, 0x2 << 20, ctl_mask);
    }
    else if ((INPUT_MODE_CMOS_33V == input_mode) || (INPUT_MODE_BT1120 == input_mode))
    {
        mipi_write_reg32(sys_reg, 0x2 << 20, ctl_mask);
    }
    else if (INPUT_MODE_BYPASS == input_mode)
    {
        mipi_write_reg32(sys_reg, 0x3 << 20, ctl_mask);
    }
    else
    {
        mipi_write_reg32(sys_reg, 0x1 << 20, ctl_mask);
    }

    /***************************************************************/
    /* todo: temp use, reset/ureset mipi when sensor timing change */
    // this will be configed in CRG reg in ASIC.
    reset = HI_TRUE;
    mipi_asic_reg = (unsigned int)ioremap_nocache(0x2003002c, 0x4);
    if (!mipi_asic_reg)
    {
        printk("%s:ioremap failed!\n", __func__);
        return;
    }
    mipi_write_reg32(mipi_asic_reg, reset << 10, 1 << 10);
    msleep(10);
    reset = HI_FALSE;
    mipi_write_reg32(mipi_asic_reg, reset << 10, 1 << 10);

    /* csi reset/unreset */
    for (i = 0; i < COMBO_MAX_LINK_NUM; i++)
    {
        mipi_drv_set_csi2_rstn(i, 0);
        mipi_drv_set_csi2_rstn(i, 1);
    }

    iounmap(sys_reg);
    iounmap(mipi_asic_reg);
    /***************************************************************/
}

static void mipi_reset_sensor(void)
{
    /* ASIC related */
    HI_BOOL reset = HI_TRUE;
    unsigned int sensor_reg = (unsigned int)ioremap_nocache(0x2003002c, 0x4);
    if (!sensor_reg)
    {
        printk("%s:ioremap failed!\n", __func__);
        return;
    }

    mipi_write_reg32(sensor_reg, reset << 20, 1 << 20);

    iounmap(sensor_reg);
}

static void mipi_unreset_sensor(void)
{
    /* ASIC related */
    HI_BOOL reset = HI_FALSE;
    unsigned int sensor_reg = (unsigned int)ioremap_nocache(0x2003002c, 0x4);
    if (!sensor_reg)
    {
        printk("%s:ioremap failed!\n", __func__);
        return;
    }
    mipi_write_reg32(sensor_reg, reset << 20, 1 << 20);

    /* wait 20ms after sensor unrest, or some sensor regs can't be configed */
    msleep(20);

    iounmap(sensor_reg);
}

/******************************************************************************
* asic related end
******************************************************************************/
int mipi_drv_init_phy_power_ctl(unsigned int en)
{
    int i = 0;

    /***************************************************************
    (1)	config sensor
    (2)	PHYCIL_PWDN set 1
    (3)	delay at least 20us,PHYCIL_RSTN set 1
    (4)	CSI2_RSTN set 1
    ****************************************************************/
    /* 1. phy power up */
    for (i = 0; i < COMBO_MAX_LINK_NUM; i++)
    {
        mipi_drv_set_phy_pwdn(i, en);
    }

    /* 2. delay 20 us */
    udelay(MIPI_DELAY_TIME);

    /* 3. PHYCIL_RSTN |  CSI2_RSTN*/
    for (i = 0; i < COMBO_MAX_LINK_NUM; i++)
    {
        mipi_drv_set_phy_rstn(i, en);
        mipi_drv_set_csi2_rstn(i, en);
    }

    printk("init phy power successful!\n");
    return HI_SUCCESS;
}

void  hi_mipi_init(void)
{
    unsigned int ret = HI_SUCCESS;
    int i = 0;

    /* asic need modify */
    HI_BOOL reset = HI_TRUE;
    unsigned int mipi_apb_reset = (unsigned int)ioremap_nocache(0x2003002c, 0x4);
    if (!mipi_apb_reset)
    {
        printk("%s:ioremap failed!\n", __func__);
        return;
    }
    /* mipi apb reset/unreset */
    mipi_write_reg32(mipi_apb_reset, reset << 11, 1 << 11);
    msleep(10);
    reset = HI_FALSE;
    mipi_write_reg32(mipi_apb_reset, reset << 11, 1 << 11);


    /* power init */
    ret = mipi_drv_init_phy_power_ctl(1);
    if (HI_SUCCESS != ret)
    {
        printk("init mipi dev phy powre ctl fail\n");
    }

    /* int mask, enable frame int */
    for (i = 0; i < COMBO_MAX_LINK_NUM; i++)
    {
        mipi_drv_set_mipi_mask(i, MIPI_INT_MASK);
        mipi_drv_set_link_mask(i, COMBO_LINK_INT_MASK);
        mipi_drv_init_phy_cfg(i);
    }

    iounmap(mipi_apb_reset);
}

static inline int get_valid_idx(short p_link_lane[LANE_NUM_PER_LINK])
{
    int i, j, k;
    int ret_idx = -1;

    HI_MSG("p_link_lane[0]: %d, lane_id: \n", p_link_lane[0]);

    for (i = 0; i < VALID_NUM; i++)
    {
        int n_found = 0;
        int temp_lane_num = 0;
        int max_lane_id = 0;

        for (j = 0; j < LANE_NUM_PER_LINK; j++)
        {
            int lane_id = p_link_lane[j];

            if (lane_id != -1)
            {
                temp_lane_num++;
                max_lane_id = MAX2(lane_id, max_lane_id);

                for (k = 0; k < LANE_NUM_PER_LINK; k++)
                {
                    if (lane_id == g_valid_lane_arr[i][k])
                    {
                        n_found++;
                        break;
                    }
                }

                /* not match - break */
                if (n_found > 0 && k >= LANE_NUM_PER_LINK)
                {
                    break;
                }
            }
        }

        if ((n_found > 0) && (j >= LANE_NUM_PER_LINK - 1) && (temp_lane_num == n_found))
        {
            /* match */
            ret_idx = i;
            break;
        }
    }

    HI_MSG("\nret_idx: %d\n", ret_idx);

    return ret_idx;
}

static int check_lane_id(input_mode_t input_mode, short* p_lane_id)
{
    int i = 0, j = 0;
    int temp_lane_num = 0;
    int min_lane_id = LVDS_LANE_NUM;
    int max_lane_id = -1;

    for (i = 0; i < LVDS_LANE_NUM; i++)
    {
        int temp_id = p_lane_id[i];

        if (temp_id < -1 || temp_id >= LVDS_LANE_NUM)
        {
            HI_ERR("lane_id[%d] is invlid value %d.\n", i, temp_id);
            return HI_FAILURE;
        }

        for (j = i + 1; j < LVDS_LANE_NUM; j++)
        {
            if (temp_id == p_lane_id[j] && -1 != temp_id)
            {
                HI_ERR("lane_id[%d] can't be same value %d as lane_id[%d]\n", i, temp_id, j);
                return HI_FAILURE;
            }
        }

        if (temp_id != -1)
        {
            HI_MSG(" %d", temp_id);
            temp_lane_num++;

            min_lane_id = MIN2(min_lane_id, temp_id);
            max_lane_id = MAX2(max_lane_id, temp_id);
        }
    }

    if (min_lane_id < 0)
    {
        HI_ERR("all lane_id is invalid, min_lane_id is %d.\n", min_lane_id);
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
    memset(p_link_info, -1, sizeof(link_info_s));
    p_link_info->b_order = 0;
}

static int get_lane_info(short* p_lane_id, link_info_s* p_link_info)
{
    int i = 0, j = 0;
    int temp_lane_num = 0;
    int temp_link_num = 0;
    int temp_link0_lane_num = 0;
    int temp_link1_lane_num = 0;
    int temp_link_lane_num = 0;

    init_lane_info(p_link_info);

    for (i = 0; i < LVDS_LANE_NUM; i++)
    {
        int temp_id = p_lane_id[i];

        if (temp_id != -1)
        {
            if (i < LANE_NUM_PER_LINK)
            {
                temp_link0_lane_num ++;
            }
            else
            {
                temp_link1_lane_num ++;
            }

            temp_lane_num++;
        }
    }

    temp_link_lane_num = MAX2(temp_link0_lane_num, temp_link1_lane_num);
    p_link_info->total_lane_num = temp_lane_num;

    for (i = 0; i < COMBO_MAX_LINK_NUM; i++)
    {
        int ret_idx = get_valid_idx(&p_lane_id[i * LANE_NUM_PER_LINK]);

        if (ret_idx != -1)
        {
            p_link_info->link_arr[temp_link_num].link_idx = i;

            /* record disorder */
            if (IS_DISORDER(ret_idx))
            { p_link_info->b_order = 1; }

            for (j = 0; j < LANE_NUM_PER_LINK; j++)
            {
                int tmp_idx = i * LANE_NUM_PER_LINK + j;

                if (p_lane_id[tmp_idx] != -1)
                {
                    p_link_info->link_arr[temp_link_num].lane_num++;
                    if (p_link_info->b_order)
                    {
                        /* (0,2,4,6) -> (0,1,2,3) or  (1,3,5,7) -> (0,1,2,3)*/
                        p_link_info->link_arr[temp_link_num].lane_idx[j] = p_lane_id[tmp_idx] / 2;
                    }
                    else
                    {
                        /* (4,5,6,7) -> (0,1,2,3) */
                        p_link_info->link_arr[temp_link_num].lane_idx[j] = p_lane_id[tmp_idx] % temp_link_lane_num;
                    }
                }
            }

            temp_link_num++;
        }
    }

    p_link_info->link_num = temp_link_num;

#if 0
    printk("\n==================link info=====================================\n");
    printk("link_num: %d, disorder: %d\n", p_link_info->link_num, p_link_info->b_order);
    printk("total_lane: %d\n", p_link_info->total_lane_num);

    for (i = 0; i < COMBO_MAX_LINK_NUM; i++)
    {
        printk("link_idx: %d\n", p_link_info->link_arr[i].link_idx);
        for (j = 0; j < LANE_NUM_PER_LINK; j++)
        {
            printk("p_link_info->link_arr[%d].lane_idx[%d]: %d\n",
                   i, j, p_link_info->link_arr[i].lane_idx[j]);
        }
        printk("\n");
    }
    HI_MSG("\n==================link info end=================================\n");
#endif

    return HI_SUCCESS;
}

inline int check_mipi_dev_attr(mipi_dev_attr_t* p_attr)
{
    int ret = 0;
#if 0
    if ((p_attr->lane_num < HI_LANE_NUM1) || (p_attr->lane_num >= HI_LANE_NUM_BUTT))
    {
        HI_ERR("invalid lane number, must be in [%d, %d)\n", HI_LANE_NUM1, HI_LANE_NUM_BUTT);
        return HI_FAILURE;
    }
#endif

    if ((p_attr->raw_data_type < RAW_DATA_10BIT) || (p_attr->raw_data_type >= RAW_DATA_BUTT))
    {
        HI_ERR("invalid raw_data_type, must be in [%d, %d)\n", RAW_DATA_10BIT, RAW_DATA_BUTT);
        return HI_FAILURE;
    }

    ret = check_lane_id(INPUT_MODE_MIPI, p_attr->lane_id);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }
    return HI_SUCCESS;
}

static void mipi_clear_config(work_mode_t work_mode)
{
    int i = 0;
    short temp_lane_id[LANE_NUM_PER_LINK] = { -1, -1, -1, -1};

    for (i = 0; i < COMBO_MAX_LINK_NUM; i++)
    {
        mipi_drv_set_phy_lp_ck_en(i, work_mode, temp_lane_id);
        mipi_drv_set_phy_te_hs_en(i, temp_lane_id);
        mipi_drv_set_phy_cfg(i, work_mode, temp_lane_id);
        mipi_drv_set_link_lane_id(i, temp_lane_id);
    }
}

/* (0 2 4 6), (1 3 5 7), (0 1 2 3), (4 5 6 7) in one link */

int mipi_set_mipi_dev_attr(mipi_dev_attr_t* p_attr)
{
    unsigned int    lane_num;
    raw_data_type_e input_dt;
    COMBO_LINK link_id = 0;
    int ret = HI_SUCCESS;
    link_info_s* p_link_info = &g_lane_info;
    int i = 0;

    input_dt = p_attr->raw_data_type;

    ret = check_mipi_dev_attr(p_attr);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    ret = get_lane_info(p_attr->lane_id, p_link_info);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    /* clear last config */
    mipi_clear_config(WORK_MODE_MIPI);

    mipi_set_sys_work_mode(INPUT_MODE_MIPI);

    lane_num = p_link_info->total_lane_num;
    /* calc lane_num via lane_id */
    mipi_drv_set_lane_num(0, lane_num - 1);

    /******************************************************
    when mipi mode, select lane
    1) {3, 2, 1, 0, -1, -1, -1, -1}: phy0
    2) {-1, -1, -1, -1, 3, 2, 1, 0}: phy1
    3) don't support:
        {-1, 0, 1, -1, -1, 2, 3, -1}: two link
    *******************************************************/
    for (i = 0; i < p_link_info->link_num; i++)
    {
        link_id = p_link_info->link_arr[i].link_idx;

        mipi_drv_set_phy_mode(link_id, WORK_MODE_MIPI);
        mipi_drv_set_output_clk_edge(link_id, 0x1);

        mipi_drv_set_phy_lp_ck_en(link_id, WORK_MODE_MIPI, p_link_info->link_arr[i].lane_idx);
        mipi_drv_set_phy_te_hs_en(link_id, p_link_info->link_arr[i].lane_idx);

        mipi_drv_set_di_dt(link_id, input_dt);
        mipi_drv_set_link_lane_id(link_id, p_link_info->link_arr[i].lane_idx);

        mipi_drv_set_phy_cfg(link_id, WORK_MODE_MIPI, p_link_info->link_arr[i].lane_idx);
    }
    mipi_drv_set_mem_cken(HI_TRUE);

    printk("set mipi phy attr successful!\n");
    return HI_SUCCESS;
}

inline int check_lvds_dev_attr(lvds_dev_attr_t* p_attr)
{
    int ret;

    if ((p_attr->raw_data_type < RAW_DATA_10BIT) || (p_attr->raw_data_type >= RAW_DATA_BUTT))
    {
        HI_ERR("invalid raw_data_type, must be in [%d, %d)\n", RAW_DATA_10BIT, RAW_DATA_BUTT);
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
    if ((p_attr->sync_mode < LVDS_SYNC_MODE_SOL) || (p_attr->sync_mode >= LVDS_SYNC_MODE_BUTT))
    {
        HI_ERR("invalid sync_mode, must be in [%d, %d)\n", LVDS_SYNC_MODE_SOL, LVDS_SYNC_MODE_BUTT);
        return HI_FAILURE;
    }
    if ( (p_attr->wdr_mode != HI_WDR_MODE_NONE) && (p_attr->wdr_mode != HI_WDR_MODE_2F)
         && (p_attr->wdr_mode != HI_WDR_MODE_DOL_2F) )
    {
        HI_ERR("invalid wdr_mode, must be %d, %d or %d\n", HI_WDR_MODE_NONE, HI_WDR_MODE_2F,
               HI_WDR_MODE_DOL_2F);
        return HI_FAILURE;
    }
    if (p_attr->img_size.width < LVDS_MIN_WIDTH || p_attr->img_size.height < LVDS_MIN_HEIGHT)
    {
        HI_ERR("invalid img_size(%d, %d), can't be smaller than (%d, %d)\n",
               p_attr->img_size.width, p_attr->img_size.height, LVDS_MIN_WIDTH, LVDS_MIN_HEIGHT);
        return HI_FAILURE;
    }

    ret = check_lane_id(INPUT_MODE_LVDS, p_attr->lane_id);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    return HI_SUCCESS;
}

int mipi_set_lvds_dev_attr(input_mode_t input_mode, lvds_dev_attr_t* p_attr)
{
    int ret = HI_SUCCESS;
    img_size_t img_size;
    link_info_s* p_link_info = &g_lane_info;
    work_mode_t work_mode;
    int i;

    ret = check_lvds_dev_attr(p_attr);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    ret = get_lane_info(p_attr->lane_id, p_link_info);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }


    work_mode = get_work_mode(input_mode);
    /* clear last config */
    mipi_clear_config(work_mode);

    mipi_set_sys_work_mode(INPUT_MODE_LVDS);

    /**************************************************************
    * 1. {0, 2, 4, -1, 1, 3, 5, -1}: use phy0 and phy1;
    * 2. {4, 2, 0, -1, 3, 5, 1, -1}: disorder, use phy0 and ph1;
    * 3. {-1, 2, 4, 0, 1, 3, 5, -1}: disorder, use phy0 and ph1;
    * 4. {-1, -1, -1, -1, 1, 3, 0, 2}: disorder, use ph1
    *
    * result:
    * 1. enable/disable phy_x;
    * 2. lane_id;
    * 3. lane_en;
    ***************************************************************/

    img_size.width  = p_attr->img_size.width - 1;
    img_size.height = p_attr->img_size.height - 1;

    mipi_drv_set_lvds_image_size(&img_size);
    mipi_drv_set_lvds_wdr_mode(p_attr->wdr_mode);

    mipi_drv_set_lvds_ctrl_mode(p_attr->sync_mode, p_attr->raw_data_type,
                                p_attr->data_endian, p_attr->sync_code_endian,
                                p_link_info->b_order);

    mipi_drv_set_mem_cken(1);

    for (i = 0; i < p_link_info->link_num; i++)
    {
        COMBO_LINK link_id = p_link_info->link_arr[i].link_idx;

        mipi_drv_set_link_lane_id(link_id, p_link_info->link_arr[i].lane_idx);
        mipi_drv_set_lvds_sync_code(link_id, &p_attr->sync_code[link_id * LANE_NUM_PER_LINK]);

        mipi_drv_set_output_clk_edge(link_id, 0x1);
        mipi_drv_set_phy_mode(link_id, work_mode);

        mipi_drv_set_phy_lp_ck_en(link_id, work_mode, p_link_info->link_arr[i].lane_idx);
        mipi_drv_set_phy_te_hs_en(link_id, p_link_info->link_arr[i].lane_idx);

        mipi_drv_set_phy_cfg(link_id, work_mode, p_link_info->link_arr[i].lane_idx);
    }

    HI_MSG("set lvds phy attr successful!\n");
    return ret;
}

static int mipi_set_cmos_attr(input_mode_t input_mode)
{
    int i = 0;
    work_mode_t work_mode = get_work_mode(input_mode);
    short temp_lane_id[LANE_NUM_PER_LINK] = { -1, -1, -1, -1};

    mipi_set_sys_work_mode(input_mode);

    for (i = 0; i < COMBO_MAX_LINK_NUM; i++)
    {
        mipi_drv_set_phy_mode(i, work_mode);
        mipi_drv_set_phy_cfg(i, work_mode, temp_lane_id);
    }

    return HI_SUCCESS;
}

static bool mipi_check_valid_input_mode(input_mode_t mode)
{
    return (mode < INPUT_MODE_BUTT);
}

int mipi_set_combo_dev_attr(combo_dev_attr_t* p_attr)
{
    int ret = HI_FAILURE;

    if (!mipi_check_valid_input_mode(p_attr->input_mode))
    {
        printk("invlid input_mode(%d).\n", p_attr->input_mode);
        return HI_FAILURE;
    }

    mutex_lock(&hi_mipi_lock);

    // 1. sensor reset
    mipi_reset_sensor();

    // 2. mipi phy reset/unreset/config
    switch (p_attr->input_mode)
    {
        case INPUT_MODE_LVDS:
        {
            ret = mipi_set_lvds_dev_attr(INPUT_MODE_LVDS, &p_attr->lvds_attr);
            mipi_drv_set_phy_reg_start(1);
            break;
        }
        case INPUT_MODE_MIPI:
        {
            ret = mipi_set_mipi_dev_attr(&p_attr->mipi_attr);
            mipi_drv_set_phy_reg_start(1);
            break;
        }
        case INPUT_MODE_CMOS_18V:
        {
            ret = mipi_set_cmos_attr(INPUT_MODE_CMOS_18V);
            mipi_drv_set_phy_reg_start(1);
            break;
        }
        case INPUT_MODE_CMOS_33V:
        case INPUT_MODE_BT1120:
        {
            ret = mipi_set_cmos_attr(INPUT_MODE_CMOS_33V);
            mipi_drv_set_phy_reg_start(1);
            break;
        }
        case INPUT_MODE_BYPASS:
        {
            mipi_set_sys_work_mode(INPUT_MODE_BYPASS);
			ret = HI_SUCCESS;
            break;
        }
        case INPUT_MODE_SUBLVDS:
        {
            ret = mipi_set_lvds_dev_attr(INPUT_MODE_SUBLVDS, &p_attr->lvds_attr);
            mipi_drv_set_phy_reg_start(1);
            break;
        }
        case INPUT_MODE_HISPI:
        {
            ret = mipi_set_lvds_dev_attr(INPUT_MODE_HISPI, &p_attr->lvds_attr);
            mipi_drv_set_phy_reg_start(1);
            break;
        }
        default:
        {
            printk("invaild input mode\n");
            ret = HI_FAILURE;
            break;
        }
    }

    // 3. sensor unreset
    mipi_unreset_sensor();

    mutex_unlock(&hi_mipi_lock);
    return ret;
}

static int mipi_set_ouput_edge(unsigned int clk_edge)
{
    int ret = HI_SUCCESS;
    int i = 0;

    if ((CLK_UP_EDGE != clk_edge) && (CLK_DOWN_EDGE != clk_edge))
    {
        HI_ERR("invalid clk edge:%d, must be in {%d, %d}.\n", clk_edge,
               CLK_UP_EDGE, CLK_DOWN_EDGE);
        return HI_FAILURE;
    }

    mutex_lock(&hi_mipi_lock);

    for (i = 0; i < COMBO_MAX_LINK_NUM; i++)
    {
        mipi_drv_set_output_clk_edge(i, clk_edge);
    }

    mipi_drv_set_phy_reg_start(1);

    mutex_unlock(&hi_mipi_lock);

    return ret;
}

static int mipi_set_output_msb(unsigned int output_msb)
{
    int ret = HI_SUCCESS;
    int i = 0;

    if ((OUTPUT_NORM_MSB != output_msb) && (OUTPUT_REVERSE_MSB != output_msb))
    {
        HI_ERR("invalid output msb:%d. must be in [%d, %d].\n", output_msb,
               OUTPUT_NORM_MSB, OUTPUT_REVERSE_MSB);
        return HI_FAILURE;
    }

    mutex_lock(&hi_mipi_lock);
    for (i = 0; i < COMBO_MAX_LINK_NUM; i++)
    {
        mipi_drv_set_output_msb(i, output_msb);
    }
    mipi_drv_set_phy_reg_start(1);
    mutex_unlock(&hi_mipi_lock);

    return ret;
}

static long hi_mipi_ioctl(struct file* file,
                          unsigned int cmd,
                          unsigned long arg)
{
    unsigned int __user* argp = (unsigned int __user*)arg;
    combo_dev_attr_t  combo_dev_attr;
    unsigned int mipi_clk_edge, output_msb;
    unsigned int ret = HI_SUCCESS;

    switch (cmd)
    {
        case HI_MIPI_SET_DEV_ATTR:
        {
            if (copy_from_user(&combo_dev_attr, argp, sizeof(combo_dev_attr_t)))
            {
                return HI_FAILURE;
            }

            ret = mipi_set_combo_dev_attr(&combo_dev_attr);
            break;
        }

        case HI_MIPI_SET_OUTPUT_CLK_EDGE:
        {
            if (copy_from_user(&mipi_clk_edge, argp, sizeof(unsigned int)))
            {
                return HI_FAILURE;
            }

            ret = mipi_set_ouput_edge(mipi_clk_edge);
            break;
        }

        case HI_MIPI_SET_OUTPUT_MSB:
        {
            if (copy_from_user(&output_msb, argp, sizeof(unsigned int)))
            {
                return HI_FAILURE;
            }

            ret = mipi_set_output_msb(output_msb);
            break;
        }

        default:
        {
            HI_ERR("invalid himipi ioctl cmd\n");
            return -EINVAL;
            break;
        }
    }

    return ret;
}

static inline void mipi_int_statics(COMBO_LINK link_id)
{
    unsigned int int_raw = 0, int2_raw = 0;
#ifndef HI_FPGA
    HI_BOOL reset = HI_TRUE;
    unsigned int mipi_apb_reset;
#endif
    unsigned int temp;

    /* read clear */
    int_raw = mipi_drv_get_mipi_raw_int1(link_id);
    int2_raw = mipi_drv_get_raw_int2(link_id);

    if (int_raw || int2_raw)
    {
        reset = HI_TRUE;
        mipi_apb_reset = (unsigned int)ioremap_nocache(0x2003002c, 0x4);
        if (!mipi_apb_reset)
        {
            printk("%s:ioremap failed!\n", __func__);
            return;
        }
        /* mipi ctrl reset/unreset */
        //mipi_write_reg32(mipi_apb_reset, reset << 10, 1 << 10);
        mipi_drv_set_csi2_rstn(0, 0);
        mipi_drv_set_csi2_rstn(0, 1);

        /* MIPI UNRESET */
        reset = HI_FALSE;
        //mipi_write_reg32(mipi_apb_reset, reset << 10, 1 << 10);
        iounmap(mipi_apb_reset);

        temp = (unsigned int)ioremap_nocache(0x20681214, 0x4);
        if (!temp)
        {
            printk("%s:ioremap failed!\n", __func__);
            return;
        }
        *(volatile HI_U32*)temp = 0x1100;
        iounmap(temp);

    }

    if (int_raw & MIPI_INT1_INT_DEF)
    {
        if (int_raw & MIPI_HEADER_ERR )
        {
            frame_dbg_info[link_id].u32HeaderErrCnt++;
        }
        if (int_raw & MIPI_VC0_FRAME_CRC)
        {
            frame_dbg_info[link_id].VcFrameErrInfo.vc0_frame_err_cnt++;
        }
        if (int_raw & MIPI_VC1_FRAME_CRC)
        {
            frame_dbg_info[link_id].VcFrameErrInfo.vc1_frame_err_cnt++;
        }
        if (int_raw & MIPI_VC2_FRAME_CRC)
        {
            frame_dbg_info[link_id].VcFrameErrInfo.vc2_frame_err_cnt++;
        }
        if (int_raw & MIPI_VC3_FRAME_CRC)
        {
            frame_dbg_info[link_id].VcFrameErrInfo.vc3_frame_err_cnt++;
        }

        if (int_raw & MIPI_VC0_ORDER_ERR )
        {
            frame_dbg_info[link_id].VcFrameOrderErrInfo.vc0_order_err_cnt++;
        }
        if (int_raw & MIPI_VC1_ORDER_ERR )
        {
            frame_dbg_info[link_id].VcFrameOrderErrInfo.vc1_order_err_cnt++;
        }
        if (int_raw & MIPI_VC2_ORDER_ERR )
        {
            frame_dbg_info[link_id].VcFrameOrderErrInfo.vc2_order_err_cnt++;
        }
        if (int_raw & MIPI_VC3_ORDER_ERR )
        {
            frame_dbg_info[link_id].VcFrameOrderErrInfo.vc3_order_err_cnt++;
        }

        if (int_raw & MIPI_VC0_NO_MATCH )
        {
            frame_dbg_info[link_id].VcFrameNoMatchInfo.vc0_nomatch_cnt++;
        }
        if (int_raw & MIPI_VC1_NO_MATCH )
        {
            frame_dbg_info[link_id].VcFrameNoMatchInfo.vc1_nomatch_cnt++;
        }
        if (int_raw & MIPI_VC2_NO_MATCH )
        {
            frame_dbg_info[link_id].VcFrameNoMatchInfo.vc2_nomatch_cnt++;
        }
        if (int_raw & MIPI_VC3_NO_MATCH )
        {
            frame_dbg_info[link_id].VcFrameNoMatchInfo.vc3_nomatch_cnt++;
        }
    }

    /* read clear */

    if (int2_raw & MIPI_INT2_INT_DEF)
    {
        if (int2_raw & MIPI_VC0_INVALID_DT  )
        {
            frame_dbg_info[link_id].VcDTErrInfo.vc0_dt_err_cnt++;
        }

        if (int2_raw & MIPI_VC1_INVALID_DT  )
        {
            frame_dbg_info[link_id].VcDTErrInfo.vc1_dt_err_cnt++;
        }

        if (int2_raw & MIPI_VC2_INVALID_DT  )
        {
            frame_dbg_info[link_id].VcDTErrInfo.vc2_dt_err_cnt++;
        }

        if (int2_raw & MIPI_VC3_INVALID_DT  )
        {
            frame_dbg_info[link_id].VcDTErrInfo.vc3_dt_err_cnt++;
        }
    }
}

void mipi_link_int_statics(COMBO_LINK link_id)
{
    unsigned int link_int_status;

    link_int_status = mipi_drv_get_link_int_status(link_id);

    if (link_int_status)
    {
        mipi_drv_set_link_int_clr(link_id, 0xffc0ffff);

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


void mipi_int_set_mipi_start(void)
{
    unsigned int link_int_status0, link_int_status1;

    link_int_status0 = mipi_drv_get_link_int_status(0);
    link_int_status1 = mipi_drv_get_link_int_status(1);

    if ((link_int_status0 & LINK_SYNC_ERR) || (link_int_status1 & LINK_SYNC_ERR))
    {
        mipi_drv_set_phy_reg_start(1);

        if (link_int_status0 & LINK_SYNC_ERR)
        {
            phy_int_state[0].link_sync_err_cnt++;
            mipi_drv_set_link_int_clr(0, 0x3F0000);
        }

        if (link_int_status1 & LINK_SYNC_ERR)
        {
            phy_int_state[1].link_sync_err_cnt++;
            mipi_drv_set_link_int_clr(1, 0x3F0000);
        }
    }

    return;
}

static irqreturn_t mipi_interrupt_route(int irq, void* dev_id)
{
    int ret = IRQ_NONE;
    int i = 0;

    mipi_int_set_mipi_start();

    for (i = 0; i < COMBO_MAX_LINK_NUM; i++)
    {
        mipi_int_statics(i);
        mipi_link_int_statics(i);
    }

    ret = IRQ_HANDLED;
    return ret;
}

static int hi_mipi_open(struct inode* inode, struct file* filp)
{
    return 0;
}

static int hi_mipi_release(struct inode* inode, struct file* file)
{
    return 0;
}

static struct file_operations hi_mipi_fops =
{
    .owner	        = THIS_MODULE,
    .unlocked_ioctl = hi_mipi_ioctl,
    .open           = hi_mipi_open,
    .release        = hi_mipi_release,
};


#ifdef CONFIG_HISI_SNAPSHOT_BOOT
static int Mipi_freeze(struct himedia_device* pdev)
{
    printk(KERN_ALERT "%s  %d\n", __FUNCTION__, __LINE__);
    return 0;
}

static int Mipi_restore(struct himedia_device* pdev)
{
    printk(KERN_DEBUG "%s  %d\n", __FUNCTION__, __LINE__);
    hi_mipi_init();
    return 0;
}
#else
static int Mipi_freeze(struct himedia_device* pdev)
{
    return HI_SUCCESS;
}

static int Mipi_restore(struct himedia_device* pdev)
{
    return HI_SUCCESS;
}
#endif

struct himedia_ops stHiMipiDrvops =
{
    .pm_freeze = Mipi_freeze,
    .pm_restore  = Mipi_restore
};

#ifdef MIPI_ENABLE_PROC_FS
static int mipi_proc_show(struct seq_file* seq, void* v)
{
    COMBO_LINK link_id;

    seq_printf(seq, "\nModule: [MIPI], Build Time: [%s, %s]\n\n", __DATE__, __TIME__);
    seq_printf(seq, "-----mipi DEV ATTR-----------------------------------------------------------------\n");

    seq_printf(seq, "%8s%6s%6s%6s%6s%12s%12s", "LaneNum", "Vc0", "Vc1", "Vc2", "Vc3",
               "DataType", "PhyMode\n");

    seq_printf(seq, "%6d%7d%6d%6d%6d%11s%12s\n\n",
               mipi_drv_get_lanenum(0),
               mipi_drv_get_vc0(0),
               mipi_drv_get_vc1(0),
               mipi_drv_get_vc2(0),
               mipi_drv_get_vc3(0),
               mipi_drv_get_dt(0),
               mipi_drv_get_phymode(0, 0));

    seq_printf(seq, "-----mipi detect info-----------------------------------------------------------------\n");

    seq_printf(seq, "%8s%8s%8s%12s", "link_id", "width", "height", "data\n");

    for (link_id = 0; link_id < COMBO_MAX_LINK_NUM; link_id++)
    {
        img_size_t image_size;

        if (0 != strncmp("CMOS", mipi_drv_get_phymode(0, 0), 4))
        {
            mipi_drv_get_image_size(link_id, &image_size);
            seq_printf(seq, "%8d%8d%8d%#12x\n\n",
                   link_id, image_size.width, image_size.height, mipi_drv_get_data(link_id));
        }
        else
        {
            // MIPI Rx can not detect the PHY Data when phy mode is CMOS
            seq_printf(seq, "%8d%8s%8s%12s\n\n", link_id, "--", "--", "--");
        }
    }

    seq_printf(seq, "-----MIPI int error info-----------------------------------------------------------\n\n");

    seq_printf(seq, "%5s%10s%8s%8s%8s%8s%14s%14s%14s%14s", "link", "HeaderCnt", "vc0CRC", "vc1CRC",
               "vc2CRC", "vc3CRC", "vc0OrderErr", "vc1OrderErr", "vc2OrderErr", "vc3OrderErr\n");

    for (link_id = 0; link_id < COMBO_MAX_LINK_NUM; link_id++)
    {
        seq_printf(seq, "%5d%6d%10d%8d%8d%8d%12d%14d%14d%14d\n\n",
                   link_id,
                   frame_dbg_info[link_id].u32HeaderErrCnt,
                   frame_dbg_info[link_id].VcFrameErrInfo.vc0_frame_err_cnt,
                   frame_dbg_info[link_id].VcFrameErrInfo.vc1_frame_err_cnt,
                   frame_dbg_info[link_id].VcFrameErrInfo.vc2_frame_err_cnt,
                   frame_dbg_info[link_id].VcFrameErrInfo.vc3_frame_err_cnt,
                   frame_dbg_info[link_id].VcFrameOrderErrInfo.vc0_order_err_cnt,
                   frame_dbg_info[link_id].VcFrameOrderErrInfo.vc1_order_err_cnt,
                   frame_dbg_info[link_id].VcFrameOrderErrInfo.vc2_order_err_cnt,
                   frame_dbg_info[link_id].VcFrameOrderErrInfo.vc3_order_err_cnt);
    }

    seq_printf(seq, "%5s%11s%12s%12s%12s%10s%10s%10s%10s", "link", "vc0NMatCnt", "vc1NMatCnt",
               "vc2NMatCnt", "vc3NMatCnt", "vc0DtErr", "vc1DtErr", "vc2DtErr", "vc3DtErr\n");

    for (link_id = 0; link_id < COMBO_MAX_LINK_NUM; link_id++)
    {
        seq_printf(seq, "%5d%6d%12d%12d%12d%12d%10d%10d%10d\n\n",
                   link_id,
                   frame_dbg_info[link_id].VcFrameNoMatchInfo.vc0_nomatch_cnt,
                   frame_dbg_info[link_id].VcFrameNoMatchInfo.vc0_nomatch_cnt,
                   frame_dbg_info[link_id].VcFrameNoMatchInfo.vc0_nomatch_cnt,
                   frame_dbg_info[link_id].VcFrameNoMatchInfo.vc0_nomatch_cnt,
                   frame_dbg_info[link_id].VcDTErrInfo.vc0_dt_err_cnt,
                   frame_dbg_info[link_id].VcDTErrInfo.vc1_dt_err_cnt,
                   frame_dbg_info[link_id].VcDTErrInfo.vc2_dt_err_cnt,
                   frame_dbg_info[link_id].VcDTErrInfo.vc3_dt_err_cnt);
    }

    seq_printf(seq, "-----mipi timeout and  fifo error info---------------------------------------------\n\n");

    seq_printf(seq, "%5s%11s%10s%12s%12s%12s%10s%10s%10s%10s%10s%14s", "link", "clkTOutCnt", "d0TOutCnt", "d1TOutCnt",
               "d2TOutCnt", "d3TOutCnt", "clkEscCnt", "d0EscCnt", "d1EscCnt", "d2EscCnt", "d3EscCnt", "SynErrcCnt\n");

    for (link_id = 0; link_id < COMBO_MAX_LINK_NUM; link_id++)
    {
        seq_printf(seq, "%5d%6d%12d%12d%12d%12d%10d%10d%10d%10d%10d%12d\n\n",
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
                   phy_int_state[link_id].d3_fsm_escape_cnt,
                   phy_int_state[link_id].link_sync_err_cnt);
    }

    seq_printf(seq, "-----LVDS/SUBLVDS/HISPI phy and lane state info--------------------------------------\n\n");


    seq_printf(seq, "%5s%13s%14s%14s%14s%9s%9s%9s%9s%9s%9s%9s%10s", "link", "p0LineErrCnt", "p0FrameErrCnt", "p1LineErrCnt", "p1FrameErrCnt",
               "L0ErrCnt", "L1ErrCnt", "L2ErrCnt", "L3ErrCnt", "L4ErrCnt", "L5ErrCnt", "L6ErrCnt", "L7ErrCnt\n");

    for (link_id = 0; link_id < COMBO_MAX_LINK_NUM; link_id++)
    {
        seq_printf(seq, "%5d%7d%14d%14d%14d%12d%9d%9d%9d%9d%9d%9d%9d\n\n",
                   link_id,
                   frame_dbg_info[link_id].PhySyncErrInfo.phy0_line_sync_err_cnt,
                   frame_dbg_info[link_id].PhySyncErrInfo.phy0_frame_sync_err_cnt,
                   frame_dbg_info[link_id].PhySyncErrInfo.phy1_line_sync_err_cnt,
                   frame_dbg_info[link_id].PhySyncErrInfo.phy1_frame_sync_err_cnt,
                   frame_dbg_info[link_id].PhySyncErrInfo.lane0_sync_err_cnt,
                   frame_dbg_info[link_id].PhySyncErrInfo.lane1_sync_err_cnt,
                   frame_dbg_info[link_id].PhySyncErrInfo.lane2_sync_err_cnt,
                   frame_dbg_info[link_id].PhySyncErrInfo.lane3_sync_err_cnt,
                   frame_dbg_info[link_id].PhySyncErrInfo.lane4_sync_err_cnt,
                   frame_dbg_info[link_id].PhySyncErrInfo.lane5_sync_err_cnt,
                   frame_dbg_info[link_id].PhySyncErrInfo.lane6_sync_err_cnt,
                   frame_dbg_info[link_id].PhySyncErrInfo.lane7_sync_err_cnt);
    }

    return 0;
}

static int mipi_proc_open(struct inode* inode, struct file* file)
{
    return single_open(file, mipi_proc_show, NULL);
}

static const struct file_operations mipi_proc_fops =
{
    .owner		= THIS_MODULE,
    .open		= mipi_proc_open,
    .read		= seq_read,
    .llseek		= seq_lseek,
    .release	= single_release,
};
#endif

static int __init mipi_init(void)
{
    int ret = 0;
#ifdef MIPI_ENABLE_PROC_FS
    struct proc_dir_entry* mipi_entry;
#endif

    printk(KERN_ALERT "mipi_init\n");

    snprintf(s_stHiMipiDevice.devfs_name, 32, "%s", MIPI_DEV_NAME);
    s_stHiMipiDevice.minor =  HIMEDIA_DYNAMIC_MINOR;
    s_stHiMipiDevice.fops    =  &hi_mipi_fops;
    s_stHiMipiDevice.drvops  =  &stHiMipiDrvops;
    s_stHiMipiDevice.owner  =  THIS_MODULE;

    ret = himedia_register(&s_stHiMipiDevice);
    if (ret)
    {
        printk("Kernel: register mipi device failed!");
        return -1;
    }

    ret = request_irq(mipi_irq, &mipi_interrupt_route, 0, "mipi int", NULL);
    if (0 != ret)
    {
        HI_ERR("mipi: failed to register IRQ(%d)\n", MIPI_IRQ);
        goto MIPI_INIT_FAIL1;
    }

#ifdef MIPI_ENABLE_PROC_FS
    mipi_entry = proc_create(MIPI_PROC_NAME, 0, NULL, &mipi_proc_fops);
    if (NULL == mipi_entry)
    {
        printk(KERN_ERR "mipi: can't create %s.\n", MIPI_PROC_NAME);
        goto MIPI_INIT_FAIL2;
    }
#endif

    /* mipi and lvds init*/
    hi_mipi_init();
    //mipi_drv_set_phy_reg_start(1);   //0x20680000, reg_start

    printk("load hi_mipi.ko ...OK!\n");
    return 0;

MIPI_INIT_FAIL2:
    free_irq(MIPI_IRQ, NULL);

MIPI_INIT_FAIL1:
    himedia_unregister(&s_stHiMipiDevice);
    printk("load hi_mipi.ko ...fail!\n");
    return ret;
}

static void __exit mipi_exit(void)
{
#ifdef MIPI_ENABLE_PROC_FS
    remove_proc_entry(MIPI_PROC_NAME, NULL);
#endif

    free_irq(mipi_irq, NULL);

    himedia_unregister(&s_stHiMipiDevice);
    printk("unload hi_mipi.ko ...OK!\n");
}

#if 0
module_init(mipi_init);
module_exit(mipi_exit);
#else
extern MIPI_REGS_TYPE_S* gpMipiAllReg;
static int hi35xx_mipi_probe(struct platform_device *pdev)
{
    struct resource *mem;

    mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    gpMipiAllReg = (MIPI_REGS_TYPE_S*)devm_ioremap_resource(&pdev->dev, mem);
    if (IS_ERR(gpMipiAllReg))
            return PTR_ERR(gpMipiAllReg);

    mipi_irq = platform_get_irq(pdev, 0);
    if (mipi_irq <= 0) {
            dev_err(&pdev->dev, "cannot find mipi IRQ\n");
    }

    if(HI_SUCCESS != mipi_init())
    {
        return HI_FAILURE;
    }

    return 0;
}

static int hi35xx_mipi_remove(struct platform_device *pdev)
{
    mipi_exit();
    return 0;
}

static const struct of_device_id hi35xx_mipi_match[] = {
        { .compatible = "hisilicon,hi35xx_mipi" },
        {},
};
MODULE_DEVICE_TABLE(of, hi35xx_mipi_match);

static struct platform_driver hi35xx_mipi_driver = {
        .probe          = hi35xx_mipi_probe,
        .remove         = hi35xx_mipi_remove,
        .driver         = {
                .name   = "hi35xx_mipi",
                .of_match_table = hi35xx_mipi_match,
        },
};

module_platform_driver(hi35xx_mipi_driver);
#endif

/****************************************************************************
 * Export symbol                                                            *
 ****************************************************************************/
EXPORT_SYMBOL(mipi_set_combo_dev_attr);
EXPORT_SYMBOL(mipi_drv_set_phy_reg_start);

MODULE_AUTHOR("Digital Media Team ,Hisilicon crop ");
MODULE_DESCRIPTION("Mipi Driver");
MODULE_VERSION("HI_VERSION=" OSDRV_MODULE_VERSION_STRING);
MODULE_LICENSE("GPL");
