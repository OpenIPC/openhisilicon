/******************************************************************************

Copyright (C), 2013-2020, Hisilicon Tech. Co., Ltd.

******************************************************************************
File Name     : mipi_hal.c
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2013/4/11
Last Modified :
Description   :
Function List :
History       :
1.Date        : 2013/4/11
  Modification: Created file

******************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */


#include <asm/io.h>
#include <linux/ioport.h>

#include <asm/memory.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/delay.h>

#include "hi_mipi.h"
#include "mipi_hal.h"
#include "mipi_reg.h"
#include "hi_type.h"

/****************************************************************************
 * Define the struct of the module mipi                                     *
 ****************************************************************************/
MIPI_REGS_TYPE_S* gpMipiAllReg = NULL;

/****************************************************************************
 * Define the macro of the module mipi                                      *
 ****************************************************************************/
#define MIPI_BASE_ADDR 0x20680000

#define IS_VALID_ID(id)   (id != -1)

/****************************************************************************
 * Declare the function of the module mipi                                  *
 ****************************************************************************/
static inline MIPI_LINK_REGS_S* get_mipi_link_regs(COMBO_LINK link_id);

static inline MIPI_LINK_REGS_S* get_mipi_link_regs(COMBO_LINK link_id)
{
    if (link_id < COMBO_MAX_LINK_NUM)
    {
        return &gpMipiAllReg->stLinkRegs[link_id];
    }
    else
    {
        HI_ERR("get_mipi_link_regs err link_id\n");
        return NULL;
    }
}

inline PHY_STATUS_S mipi_drv_get_phy_status(COMBO_LINK link_id)
{
    MIPI_LINK_REGS_S* pstLinkReg = get_mipi_link_regs(link_id);

    return pstLinkReg->PHY_STATUS;
}

//******************************************************************************
//  Copyright (C), 2007-2013, Hisilicon Technologies Co., Ltd.
//
//******************************************************************************
// File name     : c_union_define.c
// Version       : 2.0
// Author        : xxx
// Created       : 2013-04-10
// Last Modified :
// Description   :  The C union definition file for the module mipi
// Function List :
// History       :
// 1 Date        :
// Author        : xxx
// Modification  : Create file
//******************************************************************************
/* The base address of the module mipi */
void RTL_Verify_RegWrite(const char* pFunctionName, unsigned int* reg_addr, char* reg_name, unsigned int value )
{
#ifdef HI_MIPI_REG_DEBUG
    printk("Function: %s, Reg Name: %s, Reg Addr:0x%04x, value=0x%x\n", pFunctionName, reg_name, ((unsigned int)reg_addr - (unsigned int)gpMipiAllReg), value);
#endif
}

//******************************************************************************
//  Function    : iSetCSI2_LANES_NUMcsi2_lane_num
//  Description : Set the value of the member MIPI_LANES_NUM.mipi_lane_num
//  Input       : unsigned int ucsi2_lane_num: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
void mipi_drv_set_lane_num(COMBO_LINK link_id, unsigned int lane_num)
{
    U_MIPI_LANES_NUM mipi_lane_num;
    MIPI_LINK_REGS_S* pstLinkReg = get_mipi_link_regs(link_id);

    mipi_lane_num.u32 = pstLinkReg->MIPI_LANES_NUM.u32;
    mipi_lane_num.bits.mipi_lane_num = lane_num;
    pstLinkReg->MIPI_LANES_NUM.u32 = mipi_lane_num.u32;
    RTL_Verify_RegWrite("set_csi2_lane_num", (unsigned int*) & (pstLinkReg->MIPI_LANES_NUM.u32), "U_MIPI_LANES_NUM", pstLinkReg->MIPI_LANES_NUM.u32);
}

//******************************************************************************
//  Function    : iSetCSI2_RSTNcsi2_rstn
//  Description : Set the value of the member MIPI_RSTN.csi2_rstn
//  Input       : unsigned int ucsi2_rstn: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
void mipi_drv_set_csi2_rstn(COMBO_LINK link_id, unsigned int csi2_rstn)
{
    U_MIPI_RSTN o_csi2_rstn;
    MIPI_LINK_REGS_S* pstLinkReg;

    pstLinkReg = get_mipi_link_regs(link_id);
    o_csi2_rstn.u32 = pstLinkReg->MIPI_RSTN.u32;
    o_csi2_rstn.bits.csi2_rstn = csi2_rstn;
    pstLinkReg->MIPI_RSTN.u32 = o_csi2_rstn.u32;
    RTL_Verify_RegWrite("set_csi2_rstn", (unsigned int*) & (pstLinkReg->MIPI_RSTN.u32), "U_MIPI_RSTN", pstLinkReg->MIPI_RSTN.u32);
}

//******************************************************************************
//  Function    : set_input_data_type
//  Description : Set the value of the member MIPI_DI_2.di7_vc
//  Input       : unsigned int udi7_vc: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
void mipi_drv_set_di_dt(COMBO_LINK link_id, raw_data_type_e raw_data_type)
{
    U_MIPI_DI_1 csi2_di_1;
    unsigned int temp_data_type = 0;
    MIPI_LINK_REGS_S* pstLinkReg = get_mipi_link_regs(link_id);

    if (RAW_DATA_10BIT == raw_data_type)
    {
        temp_data_type = 0x2b;
    }
    else if (RAW_DATA_12BIT == raw_data_type)
    {
        temp_data_type = 0x2c;
    }
    else if (RAW_DATA_14BIT == raw_data_type)
    {
        temp_data_type = 0x2d;
    }

    csi2_di_1.u32 = pstLinkReg->MIPI_DI_1.u32;
    csi2_di_1.bits.di0_dt = temp_data_type;
    csi2_di_1.bits.di1_dt = temp_data_type;
    csi2_di_1.bits.di2_dt = temp_data_type;
    csi2_di_1.bits.di3_dt = temp_data_type;
    pstLinkReg->MIPI_DI_1.u32 = csi2_di_1.u32;

    RTL_Verify_RegWrite("mipi_drv_set_di_dt", (unsigned int*) & (pstLinkReg->MIPI_DI_1.u32),
                        "U_MIPI_DI_1", pstLinkReg->MIPI_DI_1.u32);
}

void mipi_drv_set_mipi_mask(COMBO_LINK link_id, unsigned int mask)
{
    U_MIPI_INTR1_MASK csi2_mask1;
    U_MIPI_INTR2_MASK csi2_mask2;
    MIPI_LINK_REGS_S* pstLinkReg;

    pstLinkReg = get_mipi_link_regs(link_id);
    csi2_mask1.u32 = mask;
    pstLinkReg->MIPI_INTR1_MASK.u32 =  csi2_mask1.u32;

    csi2_mask2.u32 = mask;
    pstLinkReg->MIPI_INTR2_MASK.u32 =  csi2_mask2.u32;

    RTL_Verify_RegWrite("set_csi2_mask1", (unsigned int*) & (pstLinkReg->MIPI_INTR1_MASK.u32),
                        "U_MIPI_INTR1_MASK", pstLinkReg->MIPI_INTR1_MASK.u32);
}

void mipi_drv_set_link_mask(COMBO_LINK link_id, unsigned int mask)
{
    U_INT_MASK_LINK int_mask_link;
    MIPI_LINK_REGS_S* pstLinkReg = get_mipi_link_regs(link_id);

    int_mask_link.u32 = mask;
    pstLinkReg->INT_MASK_LINK.u32 =  int_mask_link.u32;

    RTL_Verify_RegWrite("mipi_drv_set_link_mask1", (unsigned int*) & (pstLinkReg->INT_MASK_LINK.u32),
                        "U_MIPI_INTR1_MASK", pstLinkReg->INT_MASK_LINK.u32);
}

void mipi_drv_set_csi2_mask1(COMBO_LINK link_id, unsigned int mask)
{
    U_MIPI_INTR1_MASK csi2_mask1;
    MIPI_LINK_REGS_S *pstLinkReg;

    pstLinkReg = get_mipi_link_regs(link_id);
    csi2_mask1.u32 = mask;
    pstLinkReg->MIPI_INTR1_MASK.u32 =  csi2_mask1.u32;

    RTL_Verify_RegWrite("set_csi2_mask1", (unsigned int*) & (pstLinkReg->MIPI_INTR1_MASK.u32),
                        "U_MIPI_INTR1_MASK", pstLinkReg->MIPI_INTR1_MASK.u32);
}

void mipi_drv_set_csi2_mask2(COMBO_LINK link_id, unsigned int mask)
{
    U_MIPI_INTR2_MASK csi2_mask2;
    MIPI_LINK_REGS_S* pstLinkReg;

    pstLinkReg = get_mipi_link_regs(link_id);
    csi2_mask2.u32 = mask;
    pstLinkReg->MIPI_INTR2_MASK.u32 =  csi2_mask2.u32;

    RTL_Verify_RegWrite("set_csi2_mask2", (unsigned int*) & (pstLinkReg->MIPI_INTR2_MASK.u32),
                        "U_MIPI_INTR2_MASK", pstLinkReg->MIPI_INTR2_MASK.u32);
}

//******************************************************************************
//  Function    : set_output_clk_edge
//  Description : Set the value of the member PHY_MODE_LINK.frsck
//  Input       : unsigned int ufrsck: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
void mipi_drv_set_output_clk_edge(COMBO_LINK link_id, clk_edge clkedge)
{
    U_PHY_MODE_LINK phycil_mode;
    MIPI_LINK_REGS_S* pstLinkReg;

    pstLinkReg = get_mipi_link_regs(link_id);

    phycil_mode.u32 = pstLinkReg->PHY_MODE_LINK.u32;
    phycil_mode.bits.phy_frpck = clkedge;
    pstLinkReg->PHY_MODE_LINK.u32 = phycil_mode.u32;
    RTL_Verify_RegWrite("set_output_clk_edge", (unsigned int*) & (pstLinkReg->PHY_MODE_LINK.u32),
                        "U_PHY_MODE_LINK", pstLinkReg->PHY_MODE_LINK.u32);
}


//******************************************************************************
//  Function    : set_input_data_width
//  Description : Set the value of the member PHY_MODE_LINK.frsck
//  Input       : unsigned int ufrsck: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
void mipi_drv_set_input_data_width(COMBO_LINK link_id, unsigned datawidth)
{
    U_PHY_MODE_LINK phycil_mode;
    MIPI_LINK_REGS_S* pstLinkReg;

    pstLinkReg = get_mipi_link_regs(link_id);

    phycil_mode.u32 = pstLinkReg->PHY_MODE_LINK.u32;
    phycil_mode.bits.phy_btsel = datawidth;
    pstLinkReg->PHY_MODE_LINK.u32 = phycil_mode.u32;
    RTL_Verify_RegWrite("set_input_data_width", (unsigned int*) & (pstLinkReg->PHY_MODE_LINK.u32),
                        "U_PHY_MODE_LINK", pstLinkReg->PHY_MODE_LINK.u32);
}

//******************************************************************************
//  Function    : set_input_data_type
//  Description : Set the value of the member PHY_MODE_LINK.frsck
//  Input       : unsigned int ufrsck: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
void mipi_drv_set_output_msb(COMBO_LINK link_id, output_msb data)
{
    U_PHY_MODE_LINK phycil_mode;
    MIPI_LINK_REGS_S* pstLinkReg;

    pstLinkReg = get_mipi_link_regs(link_id);

    phycil_mode.u32 = pstLinkReg->PHY_MODE_LINK.u32;
    phycil_mode.bits.phy_sbsel = data;
    pstLinkReg->PHY_MODE_LINK.u32 = phycil_mode.u32;
    RTL_Verify_RegWrite("set_output_msb", (unsigned int*) & (pstLinkReg->PHY_MODE_LINK.u32),
                        "U_PHY_MODE_LINK", pstLinkReg->PHY_MODE_LINK.u32);
}

//******************************************************************************
//  Function    : iSetPHYCIL_PWDNpwdn
//  Description : Set the value of the member PHYCIL_PWDN.pwdn
//  Input       : unsigned int upwdn: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
void mipi_drv_set_phy_pwdn(COMBO_LINK link_id, unsigned int upwdn)
{
    U_PHY_POWER_LINK o_power;
    MIPI_LINK_REGS_S* pstLinkReg;

    pstLinkReg = get_mipi_link_regs(link_id);

    o_power.u32 = pstLinkReg->PHY_POWER_LINK.u32;
    o_power.bits.phy_pwdn = upwdn;
    pstLinkReg->PHY_POWER_LINK.u32 = o_power.u32;
    RTL_Verify_RegWrite("set_phy_pwdn", (unsigned int*) & (pstLinkReg->PHY_POWER_LINK.u32),
                        "U_PHY_POWER_LINK", pstLinkReg->PHY_POWER_LINK.u32);
}

//******************************************************************************
//  Function    : set_phy_rstn
//  Description : Set the value of the member PHYCIL_RSTN.rstn
//  Input       : unsigned int urstn: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
void mipi_drv_set_phy_rstn(COMBO_LINK link_id, unsigned int urstn)
{
    U_PHY_POWER_LINK o_power;
    MIPI_LINK_REGS_S* pstLinkReg;

    pstLinkReg = get_mipi_link_regs(link_id);

    o_power.u32 = pstLinkReg->PHY_POWER_LINK.u32;
    o_power.bits.phy_rstn = urstn;
    pstLinkReg->PHY_POWER_LINK.u32 = o_power.u32;
    RTL_Verify_RegWrite("set_phy_pwdn", (unsigned int*) & (pstLinkReg->PHY_POWER_LINK.u32),
                        "U_PHY_POWER_LINK", pstLinkReg->PHY_POWER_LINK.u32);
}

void mipi_drv_set_phy_lp_ck_en(COMBO_LINK link_id, work_mode_t e_mode,
                               short lane_id[LANE_NUM_PER_LINK])
{
    U_PHY_EN_LINK    phycil_lp_en;
    U_MIPI_FSM_CKEN  mipi_fsm_cken;
    MIPI_LINK_REGS_S* pstLinkReg = get_mipi_link_regs(link_id);

    phycil_lp_en.u32 = pstLinkReg->PHY_EN_LINK.u32;
    mipi_fsm_cken.u32 = pstLinkReg->MIPI_FSM_CKEN.u32;

    /* only be configed when mipi */
    if (WORK_MODE_MIPI == e_mode)
    {
        phycil_lp_en.bits.phy_lp_cken = 1;
        phycil_lp_en.bits.phy_lp_d0en = IS_VALID_ID(lane_id[0]);
        phycil_lp_en.bits.phy_lp_d1en = IS_VALID_ID(lane_id[1]);
        phycil_lp_en.bits.phy_lp_d2en = IS_VALID_ID(lane_id[2]);
        phycil_lp_en.bits.phy_lp_d3en = IS_VALID_ID(lane_id[3]);

        mipi_fsm_cken.bits.mipi_ck_cken = 1;
        mipi_fsm_cken.bits.mipi_d0_cken = IS_VALID_ID(lane_id[0]);
        mipi_fsm_cken.bits.mipi_d1_cken = IS_VALID_ID(lane_id[1]);
        mipi_fsm_cken.bits.mipi_d2_cken = IS_VALID_ID(lane_id[2]);
        mipi_fsm_cken.bits.mipi_d3_cken = IS_VALID_ID(lane_id[3]);
    }
    else
    {
        phycil_lp_en.bits.phy_lp_cken = 0;
        phycil_lp_en.bits.phy_lp_d0en = 0;
        phycil_lp_en.bits.phy_lp_d1en = 0;
        phycil_lp_en.bits.phy_lp_d2en = 0;
        phycil_lp_en.bits.phy_lp_d3en = 0;

        mipi_fsm_cken.bits.mipi_ck_cken = 0;
        mipi_fsm_cken.bits.mipi_d0_cken = 0;
        mipi_fsm_cken.bits.mipi_d1_cken = 0;
        mipi_fsm_cken.bits.mipi_d2_cken = 0;
        mipi_fsm_cken.bits.mipi_d3_cken = 0;
    }

    pstLinkReg->PHY_EN_LINK.u32 = phycil_lp_en.u32;
    pstLinkReg->MIPI_FSM_CKEN.u32 = mipi_fsm_cken.u32;

    RTL_Verify_RegWrite("mipi_drv_set_phy_lp_ck_en", (unsigned int*) & (pstLinkReg->PHY_EN_LINK.u32),
                        "U_PHYCIL_LP_EN", pstLinkReg->PHY_EN_LINK.u32);

    RTL_Verify_RegWrite("mipi_drv_set_phy_lp_ck_en", (unsigned int*) & (pstLinkReg->MIPI_FSM_CKEN.u32),
                        "MIPI_FSM_CKEN", pstLinkReg->MIPI_FSM_CKEN.u32);
}

/* set phy terminal enable*/
void mipi_drv_set_phy_te_hs_en(COMBO_LINK link_id, short lane_id[LANE_NUM_PER_LINK])
{
    U_PHY_EN_LINK     phy_en_link;
    MIPI_LINK_REGS_S* pstLinkReg = get_mipi_link_regs(link_id);

    phy_en_link.u32 = pstLinkReg->PHY_EN_LINK.u32;

    phy_en_link.bits.phy_te_cken = 1;
    phy_en_link.bits.phy_te_d0en = IS_VALID_ID(lane_id[0]);
    phy_en_link.bits.phy_te_d1en = IS_VALID_ID(lane_id[1]);
    phy_en_link.bits.phy_te_d2en = IS_VALID_ID(lane_id[2]);
    phy_en_link.bits.phy_te_d3en = IS_VALID_ID(lane_id[3]);

    phy_en_link.bits.phy_hs_cken = 1;
    phy_en_link.bits.phy_hs_d0en = IS_VALID_ID(lane_id[0]);
    phy_en_link.bits.phy_hs_d1en = IS_VALID_ID(lane_id[1]);
    phy_en_link.bits.phy_hs_d2en = IS_VALID_ID(lane_id[2]);
    phy_en_link.bits.phy_hs_d3en = IS_VALID_ID(lane_id[3]);
    pstLinkReg->PHY_EN_LINK.u32 = phy_en_link.u32;

    RTL_Verify_RegWrite("mipi_drv_set_phy_hs_en", (unsigned int*) & (pstLinkReg->PHY_EN_LINK.u32),
                        "U_PHY_EN_LINK", pstLinkReg->PHY_EN_LINK.u32);
}


//******************************************************************************
//  Function    : set_mipi_mem_cken
//  Description : Set the value of the member MIPI_CTRL_PARAM.mipi_mem_cken
//  Input       : unsigned int umipi_mem_cken: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
void mipi_drv_set_mem_cken(HI_BOOL en)
{
    U_MIPI_CTRL_PARAM o_mipi_mem_cken;

    o_mipi_mem_cken.u32 = gpMipiAllReg->MIPI_CTRL_PARAM.u32;
    o_mipi_mem_cken.bits.mipi_mem_ck_gt = en;
    gpMipiAllReg->MIPI_CTRL_PARAM.u32 = o_mipi_mem_cken.u32;
    RTL_Verify_RegWrite("set_mipi_mem_cken", (unsigned int*) & (gpMipiAllReg->MIPI_CTRL_PARAM.u32), "U_MIPI_CTRL_PARAM", gpMipiAllReg->MIPI_CTRL_PARAM.u32);
}

void mipi_drv_set_phy_mode(COMBO_LINK link_id, work_mode_t mode)
{
    U_PHY_MODE_LINK phycil_mode;
    MIPI_LINK_REGS_S* pstLinkReg;

    pstLinkReg = get_mipi_link_regs(link_id);

    phycil_mode.u32 = pstLinkReg->PHY_MODE_LINK.u32;
    phycil_mode.bits.phy_ifimode = mode;
    pstLinkReg->PHY_MODE_LINK.u32 = phycil_mode.u32;

    RTL_Verify_RegWrite("mipi_drv_set_phy_mode", (unsigned int*) & (pstLinkReg->PHY_MODE_LINK.u32),
                        "U_PHY_MODE_LINK", pstLinkReg->PHY_MODE_LINK.u32);
}

/*interrupts and get mipi attr*/

unsigned int mipi_drv_get_lanenum(COMBO_LINK link_id)
{
    unsigned int value;
    MIPI_LINK_REGS_S* pstLinkReg;

    pstLinkReg = get_mipi_link_regs(link_id);
    value = pstLinkReg->MIPI_LANES_NUM.bits.mipi_lane_num;
    return value + 1;
}

unsigned int mipi_drv_get_vc0(COMBO_LINK link_id)
{
    unsigned int value;
    MIPI_LINK_REGS_S* pstLinkReg;

    pstLinkReg = get_mipi_link_regs(link_id);
    value = pstLinkReg->MIPI_DI_1.bits.di0_vc;
    return value;
}

unsigned int mipi_drv_get_vc1(COMBO_LINK link_id)
{
    unsigned int value;
    MIPI_LINK_REGS_S* pstLinkReg;

    pstLinkReg = get_mipi_link_regs(link_id);
    value = pstLinkReg->MIPI_DI_1.bits.di1_vc;
    return value;
}

unsigned int mipi_drv_get_vc2(COMBO_LINK link_id)
{
    unsigned int value;
    MIPI_LINK_REGS_S* pstLinkReg;

    pstLinkReg = get_mipi_link_regs(link_id);
    value = pstLinkReg->MIPI_DI_1.bits.di2_vc;
    return value;
}

unsigned int mipi_drv_get_vc3(COMBO_LINK link_id)
{
    unsigned int value;
    MIPI_LINK_REGS_S* pstLinkReg;

    pstLinkReg = get_mipi_link_regs(link_id);
    value = pstLinkReg->MIPI_DI_1.bits.di3_vc;
    return value;
}

char* mipi_drv_get_dt(COMBO_LINK link_id)
{
    unsigned int value;
    MIPI_LINK_REGS_S* pstLinkReg;

    pstLinkReg = get_mipi_link_regs(link_id);
    value = pstLinkReg->MIPI_DI_1.bits.di0_dt;
    if (0x2b == value)
    {
        return ("RAW10");
    }
    else if (0x2c == value)
    {
        return ("RAW12");
    }
    else if (0x2d == value)
    {
        return ("RAW14");
    }
    else
    {
        return ("N/A");
    }
}

/*get interrupts err info*/
unsigned int mipi_drv_get_mipi_raw_int1(COMBO_LINK link_id)
{
    MIPI_LINK_REGS_S* pstLinkReg;

    pstLinkReg = get_mipi_link_regs(link_id);
    return pstLinkReg->MIPI_INTR1_RAW.u32;
}

unsigned int mipi_drv_get_raw_int2(COMBO_LINK link_id)
{
    MIPI_LINK_REGS_S* pstLinkReg;

    pstLinkReg = get_mipi_link_regs(link_id);
    return pstLinkReg->MIPI_INTR2_RAW.u32;
}

/*get interrupts err info*/
unsigned int mipi_drv_get_mipi_int_status(COMBO_LINK link_id)
{
    MIPI_LINK_REGS_S* pstLinkReg = get_mipi_link_regs(link_id);

    return pstLinkReg->INT_STAT_LINK.u32;
}

char* mipi_drv_get_phymode(COMBO_LINK link_id, MIPI_PHY mipi_phy)
{
    unsigned int value;
    MIPI_LINK_REGS_S* pstLinkReg;

    pstLinkReg = get_mipi_link_regs(link_id);
    value = pstLinkReg->PHY_MODE_LINK.bits.phy_ifimode;

    if (0 == value)
    {
        return ("LVDS");
    }
    else if (1 == value)
    {
        return ("DPHY");
    }
    else if (2 == value)
    {
        return ("CMOS");
    }
    else if (3 == value)
    {
        return ("HiSPi");
    }

    return "NULL";
}

unsigned int mipi_drv_get_link_int_status(COMBO_LINK link_id)
{
    MIPI_LINK_REGS_S* pstLinkReg;

    pstLinkReg = get_mipi_link_regs(link_id);
    return pstLinkReg->INT_STAT_LINK.u32;
}

unsigned int mipi_drv_get_lvds_int_raw(COMBO_LINK link_id)
{
    MIPI_LINK_REGS_S* pstLinkReg;

    pstLinkReg = get_mipi_link_regs(link_id);
    return pstLinkReg->INT_RAW_LINK.u32;
}

void mipi_drv_set_link_int_clr(COMBO_LINK link_id, unsigned int intclr)
{
    U_INT_CLR_LINK int_clr;
    MIPI_LINK_REGS_S* pstLinkReg;

    pstLinkReg = get_mipi_link_regs(link_id);

    int_clr.u32 = intclr;
    pstLinkReg->INT_CLR_LINK.u32 = int_clr.u32;
    RTL_Verify_RegWrite((char*)__FUNCTION__, (unsigned int*) & (pstLinkReg->INT_CLR_LINK.u32),
                        "INT_CLR_LINK", pstLinkReg->INT_CLR_LINK.u32);
}

int mipi_drv_set_lvds_image_size(img_size_t* p_img_size)
{
    U_LVDS_IMGSIZE lvds_img_size;

    lvds_img_size.u32 = gpMipiAllReg->LVDS_IMGSIZE.u32;

    lvds_img_size.bits.lvds_imgwidth = p_img_size->width;
    lvds_img_size.bits.lvds_imgheight = p_img_size->height;
    gpMipiAllReg->LVDS_IMGSIZE.u32 = lvds_img_size.u32;

    RTL_Verify_RegWrite((char*)__FUNCTION__, (unsigned int*) & (gpMipiAllReg->LVDS_IMGSIZE.u32),
                        "LVDS_IMGSIZE", gpMipiAllReg->LVDS_IMGSIZE.u32);
    return 0;
}

int mipi_drv_set_lvds_wdr_mode(wdr_mode_e wdr_mode)
{
    U_LVDS_WDR_MODE lvds_wdr_mode;

    lvds_wdr_mode.u32 = gpMipiAllReg->LVDS_WDR_MODE.u32;
    if (HI_WDR_MODE_NONE == wdr_mode)
    {
        lvds_wdr_mode.bits.lvds_wdr_en = 0;
    }
    else
    {
        lvds_wdr_mode.bits.lvds_wdr_en = 1;
        lvds_wdr_mode.bits.lvds_wdr_num = 1;
    }

    if (HI_WDR_MODE_DOL_2F == wdr_mode)
    {
        lvds_wdr_mode.bits.wdr_mode = 1;
    }
    else
    {
        lvds_wdr_mode.bits.wdr_mode = 0;
    }

    gpMipiAllReg->LVDS_WDR_MODE.u32 = lvds_wdr_mode.u32;

    RTL_Verify_RegWrite(__FUNCTION__, (unsigned int*) & (gpMipiAllReg->LVDS_WDR_MODE.u32),
                        "LVDS_WDR_MODE", gpMipiAllReg->LVDS_WDR_MODE.u32);
    return 0;
}

int mipi_drv_set_lvds_ctrl_mode(lvds_sync_mode_e sync_mode,
                                raw_data_type_e raw_data_type,
                                lvds_bit_endian  data_endian,
                                lvds_bit_endian sync_code_endian,
                                HI_BOOL b_disorder)
{
    U_LVDS_CTRL_MODE lvds_ctrl_mode;

    lvds_ctrl_mode.u32 = gpMipiAllReg->LVDS_CTRL_MODE.u32;
    lvds_ctrl_mode.bits.lvds_sync_mode = sync_mode;
    lvds_ctrl_mode.bits.lvds_raw_type = raw_data_type;
    lvds_ctrl_mode.bits.lvds_pix_big_endian = data_endian;
    lvds_ctrl_mode.bits.lvds_code_big_endian = sync_code_endian;
    lvds_ctrl_mode.bits.lvds_link_seq = b_disorder;
    gpMipiAllReg->LVDS_CTRL_MODE.u32 = lvds_ctrl_mode.u32;

    RTL_Verify_RegWrite(__FUNCTION__, (unsigned int*) & (gpMipiAllReg->LVDS_CTRL_MODE.u32),
                        "LVDS_CTRL_MODE", gpMipiAllReg->LVDS_CTRL_MODE.u32);
    return 0;
}

int mipi_drv_set_link_lane_id(COMBO_LINK link_id, short lane_id[LANE_NUM_PER_LINK])
{
    U_LANE_ID_LINK lane_id_link;
    U_LANE_EN_LINK lane_en;
    MIPI_LINK_REGS_S* pstLinkReg;

    pstLinkReg = get_mipi_link_regs(link_id);

    /* set lane_en */
    lane_en.u32 = 0x0;
    lane_en.bits.lane0_en = IS_VALID_ID(lane_id[0]);
    lane_en.bits.lane1_en = IS_VALID_ID(lane_id[1]);
    lane_en.bits.lane2_en = IS_VALID_ID(lane_id[2]);
    lane_en.bits.lane3_en = IS_VALID_ID(lane_id[3]);
    pstLinkReg->LANE_EN_LINK.u32 = lane_en.u32;

    /* set lane_id */
    lane_id_link.u32 = pstLinkReg->LANE_ID_LINK.u32;
    if (lane_id[0] >= 0)
    {
        lane_id_link.bits.lane0_id = lane_id[0];
    }
    if (lane_id[1] >= 0)
    {
        lane_id_link.bits.lane1_id = lane_id[1];
    }
    if (lane_id[2] >= 0)
    {
        lane_id_link.bits.lane2_id = lane_id[2];
    }
    if (lane_id[3] >= 0)
    {
        lane_id_link.bits.lane3_id = lane_id[3];
    }
    pstLinkReg->LANE_ID_LINK.u32 = lane_id_link.u32;

    RTL_Verify_RegWrite(__FUNCTION__, (unsigned int*) & (pstLinkReg->LANE_EN_LINK.u32),
                        "LVDS_LANE_EN", pstLinkReg->LANE_EN_LINK.u32);
    RTL_Verify_RegWrite(__FUNCTION__, (unsigned int*) & (pstLinkReg->LANE_ID_LINK.u32),
                        "LVDS_LANE_ID", pstLinkReg->LANE_ID_LINK.u32);
    return 0;
}

int mipi_drv_set_lvds_sync_code(COMBO_LINK link_id,
                                unsigned short sync_code[LANE_NUM_PER_LINK][WDR_VC_NUM][SYNC_CODE_NUM])
{
    int i ;
    MIPI_LINK_REGS_S* pstLinkReg;

    pstLinkReg = get_mipi_link_regs(link_id);

    // unsigned short **sync_code;
    for (i = 0; i < LANE_NUM_PER_LINK; i++)
    {
        U_LANE_SYNC_CODE lane_sync_code = pstLinkReg->LANE_SYNC_CODE[i];

        {
            U_LINK_LANE_SOF_01 lvds_sof_01;
            lvds_sof_01.u32 = lane_sync_code.LINK_LANE_SOF_01.u32;
            lvds_sof_01.bits.lane0_sof_0 = sync_code[i][0][0];
            lvds_sof_01.bits.lane0_sof_1 = sync_code[i][1][0];
            pstLinkReg->LANE_SYNC_CODE[i].LINK_LANE_SOF_01.u32 = lvds_sof_01.u32;
        }
        {
            U_LINK_LANE_SOF_23 lvds_sof_23;
            lvds_sof_23.u32 = lane_sync_code.LINK_LANE_SOF_23.u32;
            lvds_sof_23.bits.lane0_sof_2 = sync_code[i][2][0];
            lvds_sof_23.bits.lane0_sof_3 = sync_code[i][3][0];
            pstLinkReg->LANE_SYNC_CODE[i].LINK_LANE_SOF_23.u32 = lvds_sof_23.u32;
        }
        {
            U_LINK_LANE_EOF_01 lvds_eof_01;
            lvds_eof_01.u32 = lane_sync_code.LINK_LANE_EOF_01.u32;
            lvds_eof_01.bits.lane0_eof_0 = sync_code[i][0][1];
            lvds_eof_01.bits.lane0_eof_1 = sync_code[i][1][1];
            pstLinkReg->LANE_SYNC_CODE[i].LINK_LANE_EOF_01.u32 = lvds_eof_01.u32;
        }
        {
            U_LINK_LANE_EOF_23 lvds_eof_23;
            lvds_eof_23.u32 = lane_sync_code.LINK_LANE_EOF_23.u32;
            lvds_eof_23.bits.lane0_eof_2 = sync_code[i][2][1];
            lvds_eof_23.bits.lane0_eof_3 = sync_code[i][3][1];
            pstLinkReg->LANE_SYNC_CODE[i].LINK_LANE_EOF_23.u32 = lvds_eof_23.u32;
        }
        {
            U_LINK_LANE_SOL_01 lvds_sol_01;
            lvds_sol_01.u32 = lane_sync_code.LINK_LANE_SOL_01.u32;
            lvds_sol_01.bits.lane0_sol_0 = sync_code[i][0][2];
            lvds_sol_01.bits.lane0_sol_1 = sync_code[i][1][2];
            pstLinkReg->LANE_SYNC_CODE[i].LINK_LANE_SOL_01.u32 = lvds_sol_01.u32;
        }
        {
            U_LINK_LANE_SOL_23 lvds_sol_23;
            lvds_sol_23.u32 = lane_sync_code.LINK_LANE_SOL_23.u32;
            lvds_sol_23.bits.lane0_sol_2 = sync_code[i][2][2];
            lvds_sol_23.bits.lane0_sol_3 = sync_code[i][3][2];
            pstLinkReg->LANE_SYNC_CODE[i].LINK_LANE_SOL_23.u32 = lvds_sol_23.u32;
        }
        {
            U_LINK_LANE_EOL_01 lvds_eol_01;
            lvds_eol_01.u32 = lane_sync_code.LINK_LANE_EOL_01.u32;
            lvds_eol_01.bits.lane0_eol_0 = sync_code[i][0][3];
            lvds_eol_01.bits.lane0_eol_1 = sync_code[i][1][3];
            pstLinkReg->LANE_SYNC_CODE[i].LINK_LANE_EOL_01.u32 = lvds_eol_01.u32;
        }
        {
            U_LINK_LANE_EOL_23 lvds_eol_23;
            lvds_eol_23.u32 = lane_sync_code.LINK_LANE_EOL_23.u32;
            lvds_eol_23.bits.lane0_eol_2 = sync_code[i][2][3];
            lvds_eol_23.bits.lane0_eol_3 = sync_code[i][3][3];
            pstLinkReg->LANE_SYNC_CODE[i].LINK_LANE_EOL_23.u32 = lvds_eol_23.u32;
        }
        //printk("set lane%d sync code\n",i);
        RTL_Verify_RegWrite(__FUNCTION__, (unsigned int*) & (pstLinkReg->LANE_SYNC_CODE[i].LINK_LANE_SOF_23.u32), "LINK_LANE_SOF_23", pstLinkReg->LANE_SYNC_CODE[i].LINK_LANE_SOF_23.u32);
        RTL_Verify_RegWrite(__FUNCTION__, (unsigned int*) & (pstLinkReg->LANE_SYNC_CODE[i].LINK_LANE_SOF_23.u32), "LINK_LANE_SOF_23", pstLinkReg->LANE_SYNC_CODE[i].LINK_LANE_SOF_23.u32);
        RTL_Verify_RegWrite(__FUNCTION__, (unsigned int*) & (pstLinkReg->LANE_SYNC_CODE[i].LINK_LANE_EOF_01.u32), "LINK_LANE_EOF_01", pstLinkReg->LANE_SYNC_CODE[i].LINK_LANE_EOF_01.u32);
        RTL_Verify_RegWrite(__FUNCTION__, (unsigned int*) & (pstLinkReg->LANE_SYNC_CODE[i].LINK_LANE_EOF_23.u32), "LINK_LANE_EOF_23", pstLinkReg->LANE_SYNC_CODE[i].LINK_LANE_EOF_23.u32);
        RTL_Verify_RegWrite(__FUNCTION__, (unsigned int*) & (pstLinkReg->LANE_SYNC_CODE[i].LINK_LANE_SOL_01.u32), "LINK_LANE_SOL_01", pstLinkReg->LANE_SYNC_CODE[i].LINK_LANE_SOL_01.u32);
        RTL_Verify_RegWrite(__FUNCTION__, (unsigned int*) & (pstLinkReg->LANE_SYNC_CODE[i].LINK_LANE_SOL_23.u32), "LINK_LANE_SOL_23", pstLinkReg->LANE_SYNC_CODE[i].LINK_LANE_SOL_23.u32);
        RTL_Verify_RegWrite(__FUNCTION__, (unsigned int*) & (pstLinkReg->LANE_SYNC_CODE[i].LINK_LANE_EOL_01.u32), "LINK_LANE_EOL_01", pstLinkReg->LANE_SYNC_CODE[i].LINK_LANE_EOL_01.u32);
        RTL_Verify_RegWrite(__FUNCTION__, (unsigned int*) & (pstLinkReg->LANE_SYNC_CODE[i].LINK_LANE_EOL_23.u32), "LINK_LANE_EOL_23", pstLinkReg->LANE_SYNC_CODE[i].LINK_LANE_EOL_23.u32);
    }
    return 0;
}

int mipi_drv_set_phy_reg_start(HI_BOOL en)
{
    U_MIPI_START  reg_start;
    /* mipi start will be invalid when config times is multiple of 2 */
    static int bconfig_first = 1;

    reg_start.u32 = gpMipiAllReg->MIPI_START.u32;
    reg_start.bits.mipi_start = en;

    /* ensure odd times config */
    if (bconfig_first)
    {
        gpMipiAllReg->MIPI_START.u32 = reg_start.u32;
        bconfig_first = 0;
    }
    else
    {
        gpMipiAllReg->MIPI_START.u32 = reg_start.u32;
        gpMipiAllReg->MIPI_START.u32 = reg_start.u32;
    }

    RTL_Verify_RegWrite(__FUNCTION__, (unsigned int*) & (gpMipiAllReg->MIPI_START.u32),
                        "PHYCIL_REG_START", gpMipiAllReg->MIPI_START.u32);
    return 0;
}

void mipi_drv_init_phy_cfg(COMBO_LINK link_id)
{
    U_PHY_CFG_LINK phy_cfg_link;
    MIPI_LINK_REGS_S* pstLinkReg;

    pstLinkReg = get_mipi_link_regs(link_id);

    phy_cfg_link = pstLinkReg->PHY_CFG_LINK;
    /*
    phy_rg_sa_en    1
    phy_rg_it_dt    0
    phy_rg_it_ck    0
    phy_rg_it_lp    2
    */
    phy_cfg_link.bits.phy_rg_sa_en = 1;
    phy_cfg_link.bits.phy_rg_it_dt = 0;
    phy_cfg_link.bits.phy_rg_it_ck = 0;
    phy_cfg_link.bits.phy_rg_it_lp = 2;
    pstLinkReg->PHY_CFG_LINK = phy_cfg_link;
}

void mipi_drv_set_phy_cfg(COMBO_LINK link_id, work_mode_t work_mode,
                          short lane_id[LANE_NUM_PER_LINK])
{
    U_PHY_CFG_LINK phy_cfg_link;
    MIPI_LINK_REGS_S* pstLinkReg;
    unsigned int phy_data_en = 0;
    bool b_cmos = 0;

    pstLinkReg = get_mipi_link_regs(link_id);

    phy_cfg_link = pstLinkReg->PHY_CFG_LINK;

    phy_data_en = (IS_VALID_ID(lane_id[0]) | IS_VALID_ID(lane_id[1]) << 1
                   | IS_VALID_ID(lane_id[2]) << 2 | IS_VALID_ID(lane_id[3]) << 3);

    phy_cfg_link.bits.phy_rg_en_d    = phy_data_en;
    /* if some lanes is enable, then enable clk */
    phy_cfg_link.bits.phy_rg_en_clk  = phy_data_en ? 1 : 0;

    if ((WORK_MODE_CMOS_18V == work_mode) || (WORK_MODE_CMOS_33V == work_mode))
    {
        b_cmos = 1;
    }
    phy_cfg_link.bits.phy_rg_en_cmos = b_cmos;
    phy_cfg_link.bits.phy_rg_en_lp   = (WORK_MODE_MIPI == work_mode) ? 1 : 0;

    pstLinkReg->PHY_CFG_LINK = phy_cfg_link;
}

void mipi_drv_get_image_size(COMBO_LINK link_id, img_size_t* p_size)
{
    if (0 == link_id)
    {
        U_MIPI_IMGSIZE_CHN0 mipi_img_size0 = gpMipiAllReg->MIPI_IMGSIZE_CHN0;
        p_size->width  = mipi_img_size0.bits.imgwidth_statis_vc0;
        p_size->height = mipi_img_size0.bits.imgheight_statis_vc0;
    }
    else if (1 == link_id)
    {
        U_MIPI_IMGSIZE_CHN1 mipi_img_size1 = gpMipiAllReg->MIPI_IMGSIZE_CHN1;
        p_size->width  = mipi_img_size1.bits.imgwidth_statis_vc1;
        p_size->height = mipi_img_size1.bits.imgheight_statis_vc1;
    }
}

unsigned int mipi_drv_get_data(COMBO_LINK link_id)
{
    unsigned int *data1;
    unsigned int *data2;

    data1 = (unsigned int*)ioremap_nocache(0x20681200, 0x4);
    if (NULL == data1)
        return -1;

    data2 = (unsigned int*)ioremap_nocache(0x20681600, 0x4);
    if (NULL == data2)
        return -1;

    if (0 == link_id)
    {
        return *data1;
    }
    else if (1 == link_id)
    {
        return *data2;
    }

    return 0;
}

