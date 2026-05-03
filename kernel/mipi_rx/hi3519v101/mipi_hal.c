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
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#include "hi_osal.h"
#include "hi_mipi.h"
#include "mipi_hal.h"
#include "mipi_reg.h"
#include "hi_type.h"

/****************************************************************************
 * Define the struct of the module mipi                                     *
 ****************************************************************************/
MIPI_REGS_TYPE_S *gpMipiAllReg = NULL;
extern void   *reg_crg_base_va;
extern void   *reg_misc_base_va;


/****************************************************************************
 * Define the macro of the module mipi                                      *
 ****************************************************************************/
#define MIPI_BASE_ADDR 0x11300000

#define IS_VALID_ID(id)   (id != -1)

#define MIPI_CHECK_NULL_PTR(ptr)\
do{\
    if(NULL == ptr)\
    {\
        HI_ERR("NULL point \n");\
        return;\
    }\
}while(0)

/****************************************************************************
 * Declare the function of the module mipi                                  *
 ****************************************************************************/

static inline void mipi_write_reg32(unsigned int addr,
                                    unsigned int value,
                                    unsigned int mask)
{
    unsigned int t;

    t = osal_readl((void*)addr);
    t &= ~mask;
    t |= value & mask;
    osal_writel(t, (void*)addr);
}

static inline unsigned int mipi_read_reg32(unsigned int addr)
{
    unsigned int t;

    t = osal_readl((void*)addr);
    return t;
}

unsigned int mipi_drv_init_reg(void)
{
    unsigned int ret = HI_SUCCESS;

    if (!gpMipiAllReg)
    {
        /* OSAL_IO_ADDRESS panics on Linux >= 3.18 (osal_ioaddress in
         * kernel/osal/hi3519v101/osal_addr.c); v101 ships kernel 3.18.20,
         * so map the MIPI register region with osal_ioremap_nocache. */
        gpMipiAllReg = (MIPI_REGS_TYPE_S*)osal_ioremap_nocache(MIPI_BASE_ADDR, sizeof(MIPI_REGS_TYPE_S));
        if(NULL == gpMipiAllReg)
        {
            ret = HI_FAILURE;
            osal_printk("init mipi reg fail\n");
        }
    }
#if 0
    {
        int i = 0;

        for (i = 0; i < COMBO_MAX_LINK_NUM; i++)
        {
            MIPI_LINK_REGS_S* pstLinkReg = get_mipi_link_regs(i);

            osal_printk("MIPI_INTR2_MASK: 0x%x\n", (unsigned int)&pstLinkReg->MIPI_INTR2_MASK.u32 - (unsigned int)gpMipiAllReg);
            osal_printk("INT_STAT_LINK: 0x%x\n", (unsigned int)&pstLinkReg->INT_STAT_LINK - (unsigned int)gpMipiAllReg);
            osal_printk("MIPI_FSM_PARAM: 0x%x\n", (unsigned int)&pstLinkReg->MIPI_FSM_PARAM.u32 - (unsigned int)gpMipiAllReg);
            osal_printk("DATA_PHY_LINK: 0x%x\n", (unsigned int)&pstLinkReg->PHY_STATUS - (unsigned int)gpMipiAllReg);
            osal_printk("LVDS_IMGSIZE: 0x%x\n", (unsigned int)&gpMipiAllReg->LVDS_IMGSIZE.u32 - (unsigned int)gpMipiAllReg);
        }
    }
#endif

    return ret;
}

void mipi_drv_exit_reg(void)
{
    if (gpMipiAllReg)
    {
        osal_iounmap(gpMipiAllReg);
        gpMipiAllReg = NULL;
    }
}

void mipi_drv_reset_sensor(COMBO_DEV dev)
{
#ifndef HI_FPGA
    /* ASIC related */
    unsigned int sensor_reset = (unsigned int)(((HI_U8*)reg_crg_base_va)+0x40);

    if (0 == dev)
    {
        mipi_write_reg32(sensor_reset, 0x1 << 5, 0x1 << 5);   // reset sensor 0
    }
    else if (1 == dev)
    {
        mipi_write_reg32(sensor_reset, 0x1 << 13, 0x1 << 13);  // reset sensor 1
    }
#else
    /*  FPGA related */
    unsigned int sensor_reset = (unsigned int)OSAL_IO_ADDRESS(0x11310000);

	if (0 == dev)
	{
		mipi_write_reg32(sensor_reset, 0x1, 0x1);   // reset sensor 0
	}
	else if (1 == dev)
	{
		mipi_write_reg32(sensor_reset, 0x1 << 4, 0x1 << 4);  // reset sensor 1
	}
#endif
}

void mipi_drv_unreset_sensor(COMBO_DEV dev)
{
#ifndef HI_FPGA
    /* ASIC related */
    unsigned int sensor_reg = (unsigned int)(((HI_U8*)reg_crg_base_va)+0x40);

    if (0 == dev)
    {
        mipi_write_reg32(sensor_reg, 0x0 << 5, 0x1 << 5);
    }
    else if (1 == dev)
    {
        mipi_write_reg32(sensor_reg, 0x0 << 13, 0x1 << 13);
    }
#else
    /*  FPGA related */
    unsigned int sensor_reset = (unsigned int)OSAL_IO_ADDRESS(0x11310000);

    if (0 == dev)
	{
		mipi_write_reg32(sensor_reset, 0x0, 0x1);   // reset sensor 0
	}
	else if (1 == dev)
	{
		mipi_write_reg32(sensor_reset, 0x0, 0x1 << 0x4);  // reset sensor 1
	}
#endif
}

//******************************************************************************
//  Function    : mipi_drv_apb_reset
//  Description : all MIPIRx register is reset to default value
//  Input       : devno, the apb reset is global, so devno is not used
//  Return      : void
//******************************************************************************
void mipi_drv_apb_reset(COMBO_DEV dev)
{
#ifndef HI_FPGA
    unsigned int mipi_reset = (unsigned int)(((HI_U8*)reg_crg_base_va)+0x3c);
    mipi_write_reg32(mipi_reset, 0x1 << 3, 0x1 << 3);    // MIPI_RX apb reset
#else
    unsigned int mipi_reset  = (unsigned int)OSAL_IO_ADDRESS(0x11310008);
    mipi_write_reg32(mipi_reset, 1, 0x1);
#endif
}

//******************************************************************************
//  Function    : mipi_drv_apb_unreset
//  Description : unreset MIPIRx APB bus
//  Input       : devno, the apb reset is global, so devno is not used
//  Return      : void
//******************************************************************************
void mipi_drv_apb_unreset(COMBO_DEV dev)
{
#ifndef HI_FPGA
    unsigned int mipi_reset = (unsigned int)(((HI_U8*)reg_crg_base_va)+0x3c);
    mipi_write_reg32(mipi_reset, 0, 0x1 << 3);      // MIPI_RX apb  unreset

#else
    unsigned int mipi_reset  = (unsigned int)OSAL_IO_ADDRESS(0x11310008);
    mipi_write_reg32(mipi_reset, 0, 0x1);
#endif
}

//******************************************************************************
//  Function    : mipi_drv_core_reset
//  Description : mipi core soft reset, reset inner state
//  Input       : devno
//  Return      : void
//******************************************************************************
void mipi_drv_core_reset(COMBO_DEV dev)
{
#ifndef HI_FPGA
	unsigned int mipi_reset  = (unsigned int)(((HI_U8*)reg_crg_base_va)+0x3c);

	if (0 == dev)
	{
		mipi_write_reg32(mipi_reset, 0x1 << 6, 0x1 << 6); 	 // mipi0 core  reset
	}
	else if (1 == dev)
	{
		mipi_write_reg32(mipi_reset, 0x1 << 22, 0x1 << 22); // mipi1 core  reset
	}
#else
	unsigned int mipi_reset  = (unsigned int)OSAL_IO_ADDRESS(0x11310004);

	if (0 == dev)
	{
		mipi_write_reg32(mipi_reset, 0x1 << 8, 0x1 << 8); 	 // mipi0 core  unreset
	}
	else if (1 == dev)
	{
		mipi_write_reg32(mipi_reset, 0x1 << 12, 0x1 << 12); // mipi1 core  unreset
	}
#endif
}

//******************************************************************************
//  Function    : mipi_drv_core_unreset
//  Description : mipi core soft unreset, MIPIRx works after core unreset
//  Input       : devno
//  Return      : void
//******************************************************************************
void mipi_drv_core_unreset(COMBO_DEV dev)
{
#ifndef HI_FPGA
	unsigned int mipi_reset  = (unsigned int)(((HI_U8*)reg_crg_base_va)+0x3c);	

    if (0 == dev)
	{
		mipi_write_reg32(mipi_reset, 0, 0x1 << 6);	 // mipi0 core	unreset
	}
	else if (1 == dev)
	{
		mipi_write_reg32(mipi_reset, 0, 0x1 << 22); // mipi1 core  unreset
	}
#else
	unsigned int mipi_reset  = (unsigned int)OSAL_IO_ADDRESS(0x11310004);

    if (0 == dev)
	{
		mipi_write_reg32(mipi_reset, 0, 0x1 << 8); 	 // mipi0 core  unreset
	}
	else if (1 == dev)
	{
		mipi_write_reg32(mipi_reset, 0, 0x1 << 12); // mipi1 core  unreset
	}	
	#endif
}

//******************************************************************************
//  Function    : set_sys_work_mode
//  Description : set MIPI Rx interface work mode and do MIPI core reset an unreset
//  Input       : devno, input_mode
//  Return      : void
//******************************************************************************
void mipi_drv_set_sys_work_mode(COMBO_DEV devno, input_mode_t input_mode)
{
    /* ASIC related */
	unsigned int ctl_mask = 0x3;
	unsigned int ctl_shift;
#ifndef HI_FPGA
    unsigned int misc_reg  = (unsigned int)(((HI_U8*)reg_misc_base_va));     // MISC register

    if ( 0 == devno)
    {
        ctl_shift = 6;
    }
    else if ( 1 == devno)
    {
        ctl_shift = 4;
    }
    else
    {
        return;
    }

    /* set sys reg via input mode */

    if (INPUT_MODE_MIPI == input_mode)
    {
        mipi_write_reg32(misc_reg, 0x0 << ctl_shift, ctl_mask << ctl_shift);
    }
    else if ((INPUT_MODE_CMOS == input_mode) || (INPUT_MODE_BT1120 == input_mode))
    {
        mipi_write_reg32(misc_reg, 0x2 << ctl_shift, ctl_mask << ctl_shift);
    }    
    else
    {
        // LVDS, SubLVDS, HiSPi
        mipi_write_reg32(misc_reg, 0x1 << ctl_shift, ctl_mask << ctl_shift);
    }

#else
    unsigned int misc_reg  = (unsigned int)OSAL_IO_ADDRESS(0x11310018);  // mipi mode

	if ( 0 == devno)
    {
        ctl_shift = 0;
    }
    else if ( 1 == devno)
    {
        ctl_shift = 4;
    }
    else
    {
        return ;
    }
    if (INPUT_MODE_MIPI == input_mode)
    {
        mipi_write_reg32(misc_reg,  0x0 << ctl_shift, ctl_mask << ctl_shift);
    }
    else if ((INPUT_MODE_CMOS == input_mode) || (INPUT_MODE_BT1120 == input_mode))
    {
        mipi_write_reg32(misc_reg, 0x2 << ctl_shift, ctl_mask << ctl_shift);
    }    
    else
    {
        // LVDS, SubLVDS, HiSPi
        mipi_write_reg32(misc_reg, 0x1 << ctl_shift, ctl_mask << ctl_shift);
    }
#endif
}

//******************************************************************************
//  Function    : get_sys_work_mode
//  Description : get MIPI Rx interface work mode
//  Input       : devno, input_mode
//  Return      : void
//******************************************************************************
work_mode_t mipi_drv_get_sys_work_mode(COMBO_DEV devno)
{
    work_mode_t  work_mode = WORK_MODE_BUTT;
    unsigned int regval, val;
	unsigned int ctl_shift;
	unsigned int ctl_mask = 0x3;
#ifndef HI_FPGA
    /* ASIC related */
    unsigned int misc_reg  = (unsigned int)(((HI_U8*)reg_misc_base_va));
  
    if ( 0 == devno)
    {
        ctl_shift = 6;
    }
    else if ( 1 == devno)
    {
        ctl_shift = 4;
    }
    else
    {
        return WORK_MODE_BUTT;
    }

    regval = mipi_read_reg32(misc_reg);
    val = (regval >> ctl_shift) & ctl_mask;

    if (0 == val)
    {
        work_mode = WORK_MODE_MIPI;
    }
    else if (1 == val)
    {
        work_mode = WORK_MODE_LVDS;
    }
    else if (2 == val)
    {
        work_mode = WORK_MODE_CMOS;
    }
    
    
#else
    unsigned int misc_reg  = (unsigned int)OSAL_IO_ADDRESS(0x11310018);  // mipi mode

	if ( 0 == devno)
    {
        ctl_shift = 0;
    }
    else if ( 1 == devno)
    {
        ctl_shift = 4;
    }
    else
    {
        return WORK_MODE_BUTT;
    }
    regval = mipi_read_reg32(misc_reg);
	val = (regval >> ctl_shift) & ctl_mask;

    if (0 == val)
    {
        work_mode = WORK_MODE_MIPI;
    }
    else if (1 == val)
    {
        work_mode = WORK_MODE_LVDS;
    }
    else if (2 == val)
    {
        work_mode = WORK_MODE_CMOS;
    }
#endif

    return work_mode;
}

static inline MIPI_LINK_REGS_S* get_mipi_link_regs(COMBO_LINK link_id)
{
    if (link_id < COMBO_MAX_LINK_NUM)
    {
        return &gpMipiAllReg->stLinkRegs[link_id];
    }
    else
    {
        HI_ERR("get_mipi_link_regs err link_id: %d\n", link_id);
        return NULL;
    }
}

static inline MIPI_CTRL_REGS_S* get_mipi_ctrl_regs(COMBO_DEV devno)
{
    if (devno < COMBO_MAX_DEV_NUM)
    {
        return &gpMipiAllReg->stComboDevRegs[devno].stMIPI_CtrlRegs;
    }
    else
    {
        HI_ERR("get_mipi_ctrl_regs err devno %d\n", devno);
        return NULL;
    }
}

static inline LVDS_CTRL_REGS_S* get_lvds_ctrl_regs(COMBO_DEV devno)
{
    if (devno < COMBO_MAX_DEV_NUM)
    {
        return &gpMipiAllReg->stComboDevRegs[devno].stLVDS_CtrlRegs;
    }
    else
    {
        HI_ERR("get_lvds_ctrl_regs err devno %d\n", devno);
        return NULL;	
    }
}

/* The base address of the module mipi */
void RTL_Verify_RegWrite(const char *pFunctionName, unsigned int *reg_addr, char *reg_name, unsigned int value )
{
    #ifdef HI_MIPI_REG_DEBUG
    osal_printk("Function: %s, Reg Name: %s, Reg Addr:0x%04x, value=0x%x\n", pFunctionName, reg_name, ((unsigned int)reg_addr - (unsigned int)gpMipiAllReg), value);
    #endif
}

//******************************************************************************
//  Function    : iSetMIPI0_LANES_NUMlane_num
//  Description : Set the value of the member MIPI0_LANES_NUM.lane_num
//  Input       : unsigned int ulane_num: 3 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
void mipi_drv_set_lane_num(COMBO_DEV devno, unsigned int lane_num)
{
    U_MIPI_LANES_NUM mipi_lane_num;
    MIPI_CTRL_REGS_S* pstCtrlReg = get_mipi_ctrl_regs(devno);
    MIPI_CHECK_NULL_PTR(pstCtrlReg);

    mipi_lane_num.u32 = pstCtrlReg->MIPI_LANES_NUM.u32;
    mipi_lane_num.bits.lane_num = lane_num - 1;
    pstCtrlReg->MIPI_LANES_NUM.u32 = mipi_lane_num.u32;
    RTL_Verify_RegWrite("mipi_drv_set_lane_num", (unsigned int *)&(pstCtrlReg->MIPI_LANES_NUM.u32), "U_MIPI_LANES_NUM",pstCtrlReg->MIPI_LANES_NUM.u32);
}

//******************************************************************************
//  Function    : set_input_data_type
//  Description : Set the value of the member MIPI_DI_2.di7_vc
//  Input       : unsigned int udi7_vc: 2 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
void mipi_drv_set_di_dt(COMBO_DEV devno, raw_data_type_e raw_data_type)
{
    U_MIPI_DI_1 csi2_di_1;
	U_MIPI_DI_2 csi2_di_2;
    unsigned int temp_data_type = 0;    
    MIPI_CTRL_REGS_S* pstCtrlReg = get_mipi_ctrl_regs(devno);
    MIPI_CHECK_NULL_PTR(pstCtrlReg);

    if (RAW_DATA_8BIT == raw_data_type)
    {
		temp_data_type = 0x2a;
    }
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

    csi2_di_1.u32 = pstCtrlReg->MIPI_DI_1.u32;
    csi2_di_1.bits.di0_dt= temp_data_type;
    csi2_di_1.bits.di1_dt= temp_data_type;
    csi2_di_1.bits.di2_dt= temp_data_type;
    csi2_di_1.bits.di3_dt= temp_data_type;
    pstCtrlReg->MIPI_DI_1.u32 = csi2_di_1.u32;

    csi2_di_2.u32 = pstCtrlReg->MIPI_DI_2.u32;
    csi2_di_2.bits.di4_dt= temp_data_type;
    csi2_di_2.bits.di5_dt= temp_data_type;
    csi2_di_2.bits.di6_dt= temp_data_type;
    csi2_di_2.bits.di7_dt= temp_data_type;
    pstCtrlReg->MIPI_DI_2.u32 = csi2_di_2.u32;

    RTL_Verify_RegWrite("mipi_drv_set_di_dt", (unsigned int *)&(pstCtrlReg->MIPI_DI_1.u32),
                        "U_MIPI_DI_1",pstCtrlReg->MIPI_DI_1.u32);
    RTL_Verify_RegWrite("mipi_drv_set_di_dt", (unsigned int *)&(pstCtrlReg->MIPI_DI_2.u32),
                        "U_MIPI_DI_2",pstCtrlReg->MIPI_DI_2.u32);
}

//******************************************************************************
//  Function    : set_mipi_user_dt
//  Description : Set user define data type
//  Input       : devno, raw_data_type, data_type
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
void mipi_drv_set_mipi_user_dt(COMBO_DEV devno, raw_data_type_e raw_data_type, 
                                    short data_type[WDR_VC_NUM])
{
    U_MIPI_USERDEF_DT user_def_dt;
    U_MIPI_USER_DEF   user_def;

    MIPI_CTRL_REGS_S* pstCtrlReg = get_mipi_ctrl_regs(devno);
    MIPI_CHECK_NULL_PTR(pstCtrlReg);

    user_def_dt.u32 = pstCtrlReg->MIPI_USERDEF_DT.u32;
    user_def.u32    = pstCtrlReg->MIPI_USER_DEF.u32;

    user_def_dt.bits.user_def0_dt = raw_data_type;
    user_def_dt.bits.user_def1_dt = raw_data_type;
    user_def_dt.bits.user_def2_dt = raw_data_type;
    user_def_dt.bits.user_def3_dt = raw_data_type;

    user_def.bits.user_def0 = data_type[0];
    user_def.bits.user_def1 = data_type[1];
    user_def.bits.user_def2 = data_type[2];
    user_def.bits.user_def3 = data_type[3];

    pstCtrlReg->MIPI_USERDEF_DT.u32 = user_def_dt.u32;
    pstCtrlReg->MIPI_USER_DEF.u32   = user_def.u32;

    RTL_Verify_RegWrite("set_mipi_user_dt", (unsigned int *)&(pstCtrlReg->MIPI_USERDEF_DT.u32),
                        "MIPI_USERDEF_DT",pstCtrlReg->MIPI_USERDEF_DT.u32);
    RTL_Verify_RegWrite("set_mipi_user_dt", (unsigned int *)&(pstCtrlReg->MIPI_USER_DEF.u32),
                        "MIPI_USER_DEF",pstCtrlReg->MIPI_USER_DEF.u32);
}

//******************************************************************************
//  Function    : set_mipi_dol_id
//  Description : Set MIPI DOL_ID, MIPI DOL mode support maximum 3WDR1
//  Input       : devno, dol_id
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
void mipi_drv_set_mipi_dol_id(COMBO_DEV devno, raw_data_type_e raw_data_type, short dol_id[])
{
    U_MIPI_DOL_ID_CODE0 dol_id0;
    U_MIPI_DOL_ID_CODE1 dol_id1;
    U_MIPI_DOL_ID_CODE2 dol_id2;
    short lef, sef1, sef2;
    short nxt_lef, nxt_sef1, nxt_sef2;

    MIPI_CTRL_REGS_S* pstCtrlReg = get_mipi_ctrl_regs(devno);
    MIPI_CHECK_NULL_PTR(pstCtrlReg);

    dol_id0.u32 = pstCtrlReg->MIPI_DOL_ID_CODE0.u32;
    dol_id1.u32 = pstCtrlReg->MIPI_DOL_ID_CODE1.u32;
    dol_id2.u32 = pstCtrlReg->MIPI_DOL_ID_CODE2.u32;

    lef  = 0x241;
    sef1 = 0x242;
    sef2 = 0x244;

    nxt_lef  = lef  + (1 << 4);
    nxt_sef1 = sef1 + (1 << 4);
    nxt_sef2 = sef2 + (1 << 4);

    dol_id0.bits.id_code_reg0 = lef;        // N Frame LEF
    dol_id0.bits.id_code_reg1 = sef1;       // N Frame SEF1
    dol_id1.bits.id_code_reg2 = sef2;       // N Frame SEF2
    
    dol_id1.bits.id_code_reg3 = nxt_lef;    // N+1 Frame LEF
    dol_id2.bits.id_code_reg4 = nxt_sef1;   // N+1 Frame SEF1
    dol_id2.bits.id_code_reg5 = nxt_sef2;   // N+1 frame SEF2
    
    pstCtrlReg->MIPI_DOL_ID_CODE0.u32 = dol_id0.u32;
    pstCtrlReg->MIPI_DOL_ID_CODE1.u32 = dol_id1.u32;
    pstCtrlReg->MIPI_DOL_ID_CODE2.u32 = dol_id2.u32;

    RTL_Verify_RegWrite("set_mipi_dol_id", (unsigned int *)&(pstCtrlReg->MIPI_DOL_ID_CODE0.u32),
                        "MIPI_DOL_ID_CODE0",pstCtrlReg->MIPI_DOL_ID_CODE0.u32);
    RTL_Verify_RegWrite("set_mipi_dol_id", (unsigned int *)&(pstCtrlReg->MIPI_DOL_ID_CODE1.u32),
                        "MIPI_DOL_ID_CODE1",pstCtrlReg->MIPI_DOL_ID_CODE1.u32);
    RTL_Verify_RegWrite("set_mipi_dol_id", (unsigned int *)&(pstCtrlReg->MIPI_DOL_ID_CODE2.u32),
                        "MIPI_DOL_ID_CODE2",pstCtrlReg->MIPI_DOL_ID_CODE2.u32);
}

//******************************************************************************
//  Function    : set_mipi_wdr_mode
//  Description : Set MIPI WDR mode en
//  Input       : devno, wdr_mode
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
void mipi_drv_set_mipi_wdr_mode(COMBO_DEV devno, mipi_wdr_mode_e wdr_mode)
{
    U_MIPI_CTRL_MODE_HS mode_hs;
    U_MIPI_CTRL_MODE_PIXEL mode_pixel;
    
    MIPI_CTRL_REGS_S* pstCtrlReg = get_mipi_ctrl_regs(devno);
    MIPI_CHECK_NULL_PTR(pstCtrlReg);

    mode_hs.u32 = pstCtrlReg->MIPI_CTRL_MODE_HS.u32;
    mode_pixel.u32 = pstCtrlReg->MIPI_CTRL_MODE_PIXEL.u32;

    if (HI_MIPI_WDR_MODE_NONE == wdr_mode)
    {
        mode_pixel.bits.mipi_dol_mode = 0;   
        mode_hs.bits.user_def_en = 0;
    }
    if (HI_MIPI_WDR_MODE_VC == wdr_mode)
    {
        
    }
    else if (HI_MIPI_WDR_MODE_DT == wdr_mode)
    {
        mode_hs.bits.user_def_en = 1;    
    }
    else if (HI_MIPI_WDR_MODE_DOL == wdr_mode)
    {
        mode_pixel.bits.mipi_dol_mode = 1;
    }

    pstCtrlReg->MIPI_CTRL_MODE_HS.u32 = mode_hs.u32;
    pstCtrlReg->MIPI_CTRL_MODE_PIXEL.u32 = mode_pixel.u32;

    RTL_Verify_RegWrite("set_mipi_wdr_mode", (unsigned int *)&(pstCtrlReg->MIPI_CTRL_MODE_HS.u32),
                        "MIPI_CTRL_MODE_HS",pstCtrlReg->MIPI_CTRL_MODE_HS.u32);
    RTL_Verify_RegWrite("set_mipi_wdr_mode", (unsigned int *)&(pstCtrlReg->MIPI_CTRL_MODE_PIXEL.u32),
                        "MIPI_CTRL_MODE_PIXEL",pstCtrlReg->MIPI_CTRL_MODE_PIXEL.u32);
    
}


//******************************************************************************
//  Function    : set_cil_int_mask
//  Description : set the MIPI cil(clock and interface logic) int mask
//  Input       : link_id, mask
//  Return      : none
//******************************************************************************
void mipi_drv_set_cil_int_mask(COMBO_LINK link_id, unsigned int mask)
{
	MIPI_LINK_REGS_S* pstLinkReg;

	pstLinkReg = get_mipi_link_regs(link_id);
    MIPI_CHECK_NULL_PTR(pstLinkReg);

    pstLinkReg->MIPI_CIL_INT_MSK_LINK.u32 = mask;
    RTL_Verify_RegWrite("set_cil_int_mask", (unsigned int *)&(pstLinkReg->MIPI_CIL_INT_MSK_LINK.u32),
            "MIPI_CIL_INT_MSK_LINK",pstLinkReg->MIPI_CIL_INT_MSK_LINK.u32);
}

//******************************************************************************
//  Function    : set_chn_int_mask
//  Description : set the MIPI channel interrupt mask
//  Input       : devno, mask
//  Return      : none
//******************************************************************************
void mipi_drv_set_chn_int_mask(COMBO_DEV devno, unsigned int mask)
{
	if (0 == devno)
	{
		gpMipiAllReg->MIPI_CHN0_INT_MSK.u32 = mask;
		RTL_Verify_RegWrite("set_chn_int_mask", (unsigned int *)&(gpMipiAllReg->MIPI_CHN0_INT_MSK.u32),
                        "MIPI_CHN0_INT_MSK",gpMipiAllReg->MIPI_CHN0_INT_MSK.u32);
	}
	else if (1 == devno)
	{
		gpMipiAllReg->MIPI_CHN1_INT_MSK.u32 = mask;
		RTL_Verify_RegWrite("set_chn_int_mask", (unsigned int *)&(gpMipiAllReg->MIPI_CHN1_INT_MSK.u32),
                        "MIPI_CHN1_INT_MSK",gpMipiAllReg->MIPI_CHN1_INT_MSK.u32);
	}
}

//******************************************************************************
//  Function    : set_mipi_ctrl_int_mask
//  Description : set the MIPI_CTRL interrupt mask
//  Input       : devno, mask
//  Return      : none
//******************************************************************************
void mipi_drv_set_mipi_ctrl_int_mask(COMBO_DEV devno, unsigned int mask)
{
    MIPI_CTRL_REGS_S* pstCtrlReg;

    pstCtrlReg = get_mipi_ctrl_regs(devno);
    MIPI_CHECK_NULL_PTR(pstCtrlReg);

    pstCtrlReg->MIPI_CTRL_INT_MSK.u32 = mask;
    RTL_Verify_RegWrite("set_mipi_ctrl_int_mask", (unsigned int *)&(pstCtrlReg->MIPI_CTRL_INT_MSK.u32),
            "MIPI_CTRL_INT_MSK",pstCtrlReg->MIPI_CTRL_INT_MSK.u32);
}

//******************************************************************************
//  Function    : set_lvds_ctrl_int_mask
//  Description : set the LVDS_CTRL interrupt mask
//  Input       : devno, mask
//  Return      : none
//******************************************************************************
void mipi_drv_set_lvds_ctrl_int_mask(COMBO_DEV devno, unsigned int mask)
{
    LVDS_CTRL_REGS_S* pstCtrlReg;

    pstCtrlReg = get_lvds_ctrl_regs(devno);
    MIPI_CHECK_NULL_PTR(pstCtrlReg);

    pstCtrlReg->LVDS_CTRL_INT_MSK.u32 = mask;
    RTL_Verify_RegWrite("set_lvds_ctrl_int_mask", (unsigned int *)&(pstCtrlReg->LVDS_CTRL_INT_MSK.u32),
            "LVDS_CTRL_INT_MSK",pstCtrlReg->LVDS_CTRL_INT_MSK.u32);
}

//******************************************************************************
//  Function    : set_align_int_mask
//  Description : set the MIPI ALIGN interrupt mask
//  Input       : devno, mask
//  Return      : none
//******************************************************************************
void mipi_drv_set_align_int_mask(COMBO_DEV devno, unsigned int mask)
{
	if (devno < COMBO_MAX_DEV_NUM)
	{
		gpMipiAllReg->stComboDevRegs[devno].ALIGN_INT_MSK.u32 = mask;
		RTL_Verify_RegWrite("set_align_int_mask", (unsigned int *)&(gpMipiAllReg->stComboDevRegs[devno].ALIGN_INT_MSK.u32),
                        "LVDS_CTRL_INT_MSK", gpMipiAllReg->stComboDevRegs[devno].ALIGN_INT_MSK.u32);
	}
}

//******************************************************************************
//  Function    : set_mipi_pkt_int1_mask
//  Description : set the MIPI packet interrupt mask
//  Input       : devno, mask
//  Return      : none
//******************************************************************************
void mipi_drv_set_mipi_pkt_int1_mask(COMBO_DEV devno, unsigned int mask)
{
    MIPI_CTRL_REGS_S* pstCtrlReg;

    pstCtrlReg = get_mipi_ctrl_regs(devno);
    MIPI_CHECK_NULL_PTR(pstCtrlReg);

    pstCtrlReg->MIPI_PKT_INTR_MSK.u32= mask;
    RTL_Verify_RegWrite("set_mipi_frame_int_mask", (unsigned int *)&(pstCtrlReg->MIPI_PKT_INTR_MSK.u32),
            "MIPI_PKT_INTR_MSK",pstCtrlReg->MIPI_PKT_INTR_MSK.u32);
}

//******************************************************************************
//  Function    : set_mipi_frame_int_mask
//  Description : set the MIPI Frame interrupt mask
//  Input       : devno, mask
//  Return      : none
//******************************************************************************
void mipi_drv_set_mipi_frame_int_mask(COMBO_DEV devno, unsigned int mask)
{
    MIPI_CTRL_REGS_S* pstCtrlReg;

    pstCtrlReg = get_mipi_ctrl_regs(devno);
    MIPI_CHECK_NULL_PTR(pstCtrlReg);

    pstCtrlReg->MIPI_FRAME_INTR_MSK.u32 = mask;
    RTL_Verify_RegWrite("set_mipi_frame_int_mask", (unsigned int *)&(pstCtrlReg->MIPI_FRAME_INTR_MSK.u32),
            "MIPI_FRAME_INTR_MSK",pstCtrlReg->MIPI_FRAME_INTR_MSK.u32);
}

//******************************************************************************
//  Function    : clear_cil_int
//  Description : clear the phy cil int status
//  Input       : link_id, intclr
//  Return      : int status
//******************************************************************************
void mipi_drv_clear_cil_int(COMBO_LINK link_id, unsigned int intclr)
{
    MIPI_LINK_REGS_S* pstLinkReg;

    pstLinkReg = get_mipi_link_regs(link_id);
    MIPI_CHECK_NULL_PTR(pstLinkReg);

    pstLinkReg->MIPI_CIL_INT_LINK.u32;        // read INT status

    pstLinkReg->MIPI_CIL_INT_RAW_LINK.u32 = intclr;  // Write Clear

}
void mipi_drv_clear_mipi_chn_int(COMBO_DEV devno)
{
	if(0 == devno)
    {
		gpMipiAllReg->MIPI_CHN0_INT_RAW.u32= 0xffff;
    }
	else if(1 == devno)
    {
		gpMipiAllReg->MIPI_CHN1_INT_RAW.u32= 0xffff;
    }
	else
    {
		return;
    }
}

//******************************************************************************
//  Function    : get_cil_raw_int
//  Description : set the phy cil int status
//  Input       : link_id
//  Return      : int status
//******************************************************************************
unsigned int mipi_drv_get_cil_raw_int(COMBO_LINK link_id)
{
	unsigned int status = 0;
	MIPI_LINK_REGS_S* pstLinkReg;

	pstLinkReg = get_mipi_link_regs(link_id);

	if (NULL != pstLinkReg)
	{
		status =  pstLinkReg->MIPI_CIL_INT_RAW_LINK.u32;
	}
	
	return status;
}

//******************************************************************************
//  Function    : get_cil_int
//  Description : set the phy cil int status
//  Input       : link_id
//  Return      : int status
//******************************************************************************
unsigned int mipi_drv_get_cil_int(COMBO_LINK link_id)
{
	unsigned int status = 0;
	MIPI_LINK_REGS_S* pstLinkReg;

	pstLinkReg = get_mipi_link_regs(link_id);

	if (NULL != pstLinkReg)
	{
		status =  pstLinkReg->MIPI_CIL_INT_LINK.u32;
	}
	
	return status;
}

//******************************************************************************
//  Function    : clear_mipi_csi_int
//  Description : clear the mipi 
//  Input       : devno
//  Return      : int status
//******************************************************************************
void mipi_drv_clear_mipi_csi_int(COMBO_DEV devno, unsigned int intclr)
{
    #if 1
    MIPI_CTRL_REGS_S* pstCtrlReg;
	unsigned int status = 0;
    
    pstCtrlReg = get_mipi_ctrl_regs(devno);
    MIPI_CHECK_NULL_PTR(pstCtrlReg);
    
    status = pstCtrlReg->MIPI_PKT_INTR_ST.u32;  // read clear
    status = pstCtrlReg->MIPI_PKT_INTR2_ST.u32;
    status = pstCtrlReg->MIPI_FRAME_INTR_ST.u32;
    status = pstCtrlReg->MIPI_LINE_INTR_ST.u32;
    
	status =  pstCtrlReg->MIPI_MAIN_INT_ST.u32;  // read clear  
    #endif

}

//******************************************************************************
//  Function    : get_mipi_pkt_int1
//  Description : set the mipi packet interrupt status 1
//  Input       : devno
//  Return      : int status
//******************************************************************************
unsigned int mipi_drv_get_mipi_pkt_int1(COMBO_DEV devno)
{
    MIPI_CTRL_REGS_S* pstCtrlReg;
    unsigned int status = 0;

    pstCtrlReg = get_mipi_ctrl_regs(devno);

	if (NULL != pstCtrlReg)
	{
		status = pstCtrlReg->MIPI_PKT_INTR_ST.u32;
	}

	return status;
}

//******************************************************************************
//  Function    : get_mipi_pkt_int2_status
//  Description : set the mipi packet interrupt status 2
//  Input       : devno
//  Return      : int status
//******************************************************************************
unsigned int mipi_drv_get_mipi_pkt_int2(COMBO_DEV devno)
{
    MIPI_CTRL_REGS_S* pstCtrlReg;
    unsigned int status = 0;

    pstCtrlReg = get_mipi_ctrl_regs(devno);

	if (NULL != pstCtrlReg)
	{
		status =  pstCtrlReg->MIPI_PKT_INTR2_ST.u32;
	}

	return status;
}

//******************************************************************************
//  Function    : mipi_drv_get_mipi_frame_int
//  Description : set the mipi packet interrupt status 2
//  Input       : devno
//  Return      : int status
//******************************************************************************
unsigned int mipi_drv_get_mipi_frame_int(COMBO_DEV devno)
{
    MIPI_CTRL_REGS_S* pstCtrlReg;
    unsigned int status = 0;

    pstCtrlReg = get_mipi_ctrl_regs(devno);

	if (NULL != pstCtrlReg)
	{
		status = pstCtrlReg->MIPI_FRAME_INTR_ST.u32;
	}

	return status;
}

//******************************************************************************
//  Function    : clear_mipi_ctrl_int
//  Description : clear the lvds_ctrl interrupt status, (write clear)
//  Input       : devno
//  Return      : int status
//******************************************************************************
void mipi_drv_clear_mipi_ctrl_int(COMBO_DEV devno, unsigned int clrint)
{
	MIPI_CTRL_REGS_S* pstCtrlReg;

	pstCtrlReg = get_mipi_ctrl_regs(devno);
    MIPI_CHECK_NULL_PTR(pstCtrlReg);

    pstCtrlReg->MIPI_CTRL_INT.u32;

    pstCtrlReg->MIPI_CTRL_INT_RAW.u32 = clrint;  // write clear

}

//******************************************************************************
//  Function    : get_mipi_ctrl_raw_int
//  Description : get the MIPI_CTRL_INT raw interrupt status
//  Input       : devno
//  Return      : int status
//******************************************************************************
unsigned int mipi_drv_get_mipi_ctrl_raw_int(COMBO_DEV devno)
{
	unsigned int status = 0;
	MIPI_CTRL_REGS_S* pstCtrlReg;

	pstCtrlReg = get_mipi_ctrl_regs(devno);

    if (NULL != pstCtrlReg)
    {
        status = pstCtrlReg->MIPI_CTRL_INT_RAW.u32;
    }

	return status;
}

//******************************************************************************
//  Function    : get_mipi_ctrl_int
//  Description : get the MIPI_CTRL_INT interrupt status
//  Input       : devno
//  Return      : int status
//******************************************************************************
unsigned int mipi_drv_get_mipi_ctrl_int(COMBO_DEV devno)
{
	unsigned int status = 0;
	MIPI_CTRL_REGS_S* pstCtrlReg;

	pstCtrlReg = get_mipi_ctrl_regs(devno);

    if (NULL != pstCtrlReg)
    {
        status = pstCtrlReg->MIPI_CTRL_INT.u32;
    }

	return status;
}

//******************************************************************************
//  Function    : clear_lvds_ctrl_int
//  Description : clear the lvds_ctrl interrupt status, (write clear)
//  Input       : devno, clrint
//  Return      : int status
//******************************************************************************
void mipi_drv_clear_lvds_ctrl_int(COMBO_DEV devno, unsigned int clrint)
{
	LVDS_CTRL_REGS_S* pstCtrlReg;

	pstCtrlReg = get_lvds_ctrl_regs(devno);
    MIPI_CHECK_NULL_PTR(pstCtrlReg);

    pstCtrlReg->LVDS_CTRL_INT.u32;

    pstCtrlReg->LVDS_CTRL_INT_RAW.u32 = clrint;    // write clear

}

//******************************************************************************
//  Function    : get_lvds_ctrl_raw_int
//  Description : get the LVDS_CTRL_INT raw interrupt status
//  Input       : devno
//  Return      : int status
//******************************************************************************
unsigned int mipi_drv_get_lvds_ctrl_raw_int(COMBO_DEV devno)
{
    unsigned int status = 0;
	LVDS_CTRL_REGS_S* pstCtrlReg;

	pstCtrlReg = get_lvds_ctrl_regs(devno);

    if (NULL != pstCtrlReg)
    {
        status =  pstCtrlReg->LVDS_CTRL_INT_RAW.u32;
	}

	return status;
}

//******************************************************************************
//  Function    : get_lvds_ctrl_int
//  Description : get the LVDS_CTRL_INT interrupt status
//  Input       : devno
//  Return      : int status
//******************************************************************************
unsigned int mipi_drv_get_lvds_ctrl_int(COMBO_DEV devno)
{
    unsigned int status = 0;
	LVDS_CTRL_REGS_S* pstCtrlReg;

	pstCtrlReg = get_lvds_ctrl_regs(devno);

    if (NULL != pstCtrlReg)
    {
        status =  pstCtrlReg->LVDS_CTRL_INT.u32;
    }

	return status;
}

//******************************************************************************
//  Function    : clear_align_int
//  Description : clear the ALIGN_INT raw interrupt status (write clear)
//  Input       : devno
//  Return      : int status
//******************************************************************************
void mipi_drv_clear_align_int(COMBO_DEV devno, unsigned int clrint)
{
    gpMipiAllReg->stComboDevRegs[devno].ALIGN_INT.u32;
    
    gpMipiAllReg->stComboDevRegs[devno].ALIGN_INT_RAW.u32 = clrint;  // write clear

}

//******************************************************************************
//  Function    : get_align_raw_int
//  Description : get the ALIGN_INT raw interrupt status
//  Input       : devno
//  Return      : int status
//******************************************************************************
unsigned int mipi_drv_get_align_raw_int(COMBO_DEV devno)
{
    unsigned int status = 0;

    if (devno < COMBO_MAX_DEV_NUM)
    {
        status =  gpMipiAllReg->stComboDevRegs[devno].ALIGN_INT_RAW.u32;
	}

	return status;
}

//******************************************************************************
//  Function    : get_align_int
//  Description : get the ALIGN_INT interrupt status
//  Input       : devno
//  Return      : int status
//******************************************************************************
unsigned int mipi_drv_get_align_int(COMBO_DEV devno)
{
    unsigned int status = 0;

    if (devno < COMBO_MAX_DEV_NUM)
    {
        status =  gpMipiAllReg->stComboDevRegs[devno].ALIGN_INT.u32;
	}

	return status;
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
    MIPI_CHECK_NULL_PTR(pstLinkReg);

    phycil_mode.u32 = pstLinkReg->PHY_MODE_LINK.u32;
    phycil_mode.bits.phy_rg_faclk_en = clkedge;
    pstLinkReg->PHY_MODE_LINK.u32 = phycil_mode.u32;
    RTL_Verify_RegWrite("set_output_clk_edge", (unsigned int *)&(pstLinkReg->PHY_MODE_LINK.u32),
                        "U_PHY_MODE_LINK",pstLinkReg->PHY_MODE_LINK.u32);
}


//******************************************************************************
//  Function    : set_input_data_width
//  Description : Set the value of the member PHY_MODE_LINK.frsck
//  Input       : unsigned int ufrsck: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
void mipi_drv_set_input_data_width(COMBO_LINK link_id, unsigned datawidth)
{
    U_PHY_SYNC_DCT_LINK phy_sync_dct;
    MIPI_LINK_REGS_S* pstLinkReg;

    pstLinkReg = get_mipi_link_regs(link_id); 
    MIPI_CHECK_NULL_PTR(pstLinkReg);

    phy_sync_dct.u32 = pstLinkReg->PHY_SYNC_DCT_LINK.u32;
    phy_sync_dct.bits.cil_raw_type = datawidth;
    pstLinkReg->PHY_MODE_LINK.u32 = phy_sync_dct.u32;
    RTL_Verify_RegWrite("set_input_data_width", (unsigned int *)&(pstLinkReg->PHY_SYNC_DCT_LINK.u32),
                        "PHY_SYNC_DCT_LINK",pstLinkReg->PHY_SYNC_DCT_LINK.u32);
}

//******************************************************************************
//  Function    : set_input_data_type
//  Description : Set the value of the member PHY_MODE_LINK.frsck
//  Input       : unsigned int ufrsck: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
void mipi_drv_set_output_msb(COMBO_LINK link_id, output_msb data)
{
    U_PHY_SYNC_DCT_LINK phy_sync_dct;
    MIPI_LINK_REGS_S* pstLinkReg;

    pstLinkReg = get_mipi_link_regs(link_id); 
    MIPI_CHECK_NULL_PTR(pstLinkReg);

    phy_sync_dct.u32 = pstLinkReg->PHY_SYNC_DCT_LINK.u32;
    phy_sync_dct.bits.cil_code_big_endian = data;
    pstLinkReg->PHY_MODE_LINK.u32 = phy_sync_dct.u32;
    RTL_Verify_RegWrite("set_output_msb", (unsigned int *)&(pstLinkReg->PHY_SYNC_DCT_LINK.u32),
                        "PHY_SYNC_DCT_LINK",pstLinkReg->PHY_MODE_LINK.u32);
}

//******************************************************************************
// hi3519 do not have this register
//******************************************************************************
void mipi_drv_set_phy_lp_ck_en(COMBO_LINK link_id, work_mode_t e_mode, 
                               short lane_id[LANE_NUM_PER_LINK])
{
#if 0
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

    RTL_Verify_RegWrite("mipi_drv_set_phy_lp_ck_en", (unsigned int *)&(pstLinkReg->PHY_EN_LINK.u32),
                        "U_PHYCIL_LP_EN", pstLinkReg->PHY_EN_LINK.u32);

    RTL_Verify_RegWrite("mipi_drv_set_phy_lp_ck_en", (unsigned int *)&(pstLinkReg->MIPI_FSM_CKEN.u32),
                        "MIPI_FSM_CKEN", pstLinkReg->MIPI_FSM_CKEN.u32);
#endif
}

//******************************************************************************
//  Function    : mipi_drv_set_phy_te_hs_en
//  Description : enable inner terminal resistance enable and high speed mode enable
//  Input       : link_id and lane's priority
//  Return      : none
//******************************************************************************
void mipi_drv_set_phy_te_hs_en(COMBO_LINK link_id, short lane_id[LANE_NUM_PER_LINK])
{
    U_PHY_EN_LINK     phy_en_link;
    MIPI_LINK_REGS_S* pstLinkReg = get_mipi_link_regs(link_id);
	HI_BOOL clock_en;

    MIPI_CHECK_NULL_PTR(pstLinkReg);

    phy_en_link.u32 = pstLinkReg->PHY_EN_LINK.u32;

	clock_en = IS_VALID_ID(lane_id[0]) || IS_VALID_ID(lane_id[1]) || 
		       IS_VALID_ID(lane_id[2]) || IS_VALID_ID(lane_id[3]);
    phy_en_link.bits.phy_clk_term_en = clock_en;
    phy_en_link.bits.phy_d0_term_en = IS_VALID_ID(lane_id[0]);
    phy_en_link.bits.phy_d1_term_en = IS_VALID_ID(lane_id[1]);
    phy_en_link.bits.phy_d2_term_en = IS_VALID_ID(lane_id[2]);
    phy_en_link.bits.phy_d3_term_en = IS_VALID_ID(lane_id[3]);    
    
    phy_en_link.bits.phy_da_d0_valid = IS_VALID_ID(lane_id[0]);
    phy_en_link.bits.phy_da_d1_valid = IS_VALID_ID(lane_id[1]);
    phy_en_link.bits.phy_da_d2_valid = IS_VALID_ID(lane_id[2]);
    phy_en_link.bits.phy_da_d3_valid = IS_VALID_ID(lane_id[3]);
    pstLinkReg->PHY_EN_LINK.u32 = phy_en_link.u32;
    
    RTL_Verify_RegWrite("set_phy_te_hs_en", (unsigned int *)&(pstLinkReg->PHY_EN_LINK.u32),
                        "U_PHY_EN_LINK", pstLinkReg->PHY_EN_LINK.u32);
}

void phy_chn1_ctrl(COMBO_DEV devno)
{
	U_PHY_ID		phy_id;
	phy_id.u32 =  gpMipiAllReg->stSysRegs.PHY_ID.u32;	
	phy_id.bits.phy2_id = 1; 
	gpMipiAllReg->stSysRegs.PHY_ID.u32 = phy_id.u32;
}
//******************************************************************************
//  Function    : phy_chn_ctrl
//  Description : phy channel control
//  Input       : devno, p_link_info, flag
//                if flag is HI_TRUE, select the phy channel, else clear the chn_ctrl
//  Return      : none
//******************************************************************************
static void phy_chn_ctrl(COMBO_DEV devno, link_info_s* p_link_info, HI_BOOL flag)
{
	U_CHN0_PHY_EN	phy_chn_ctrl_0;
	U_CHN1_PHY_EN	phy_chn_ctrl_1;
	U_PHY_ID		phy_id;

    phy_chn_ctrl_0.u32 = gpMipiAllReg->stSysRegs.CHN0_PHY_EN.u32;	
    phy_chn_ctrl_1.u32 = gpMipiAllReg->stSysRegs.CHN1_PHY_EN.u32;
	phy_id.u32 =  gpMipiAllReg->stSysRegs.PHY_ID.u32;

    if (p_link_info->total_lane_num <= 0)
    {
        return;
    }
    
    if (0 == devno)
    {
        if (1 == p_link_info->link_num)
        {
            if (flag)
            {
            	if(0 == p_link_info->link_arr[0].link_idx)
				{
                phy_chn_ctrl_0.bits.phy0_en = 1;
					phy_chn_ctrl_0.bits.phy1_en = 0;
					phy_chn_ctrl_0.bits.phy2_en = 0;
				}
				else if(1 == p_link_info->link_arr[0].link_idx)
				{
					phy_chn_ctrl_0.bits.phy0_en = 0;
					phy_chn_ctrl_0.bits.phy1_en = 1;
					phy_chn_ctrl_0.bits.phy2_en = 0;
				}
				else if(2 == p_link_info->link_arr[0].link_idx)
				{
					phy_chn_ctrl_0.bits.phy0_en = 0;
					phy_chn_ctrl_0.bits.phy1_en = 0;
					phy_chn_ctrl_0.bits.phy2_en = 1;
				}
            }
            else
            {
                phy_chn_ctrl_0.bits.phy0_en = 0;
            }
        }
        else if (2 == p_link_info->link_num)
        {
            if (flag)
            {
	            if((0 == p_link_info->link_arr[0].link_idx)
					&&(1 == p_link_info->link_arr[1].link_idx))
				{
                phy_chn_ctrl_0.bits.phy0_en = 1;
                phy_chn_ctrl_0.bits.phy1_en = 1;
					phy_chn_ctrl_0.bits.phy2_en = 0;
				}
				else  if((1 == p_link_info->link_arr[0].link_idx)
					&&(2 == p_link_info->link_arr[1].link_idx))
				{
					phy_chn_ctrl_0.bits.phy0_en = 0;
					phy_chn_ctrl_0.bits.phy1_en = 1;
					phy_chn_ctrl_0.bits.phy2_en = 1;
				}
				else  if((0 == p_link_info->link_arr[0].link_idx)
					&&(2 == p_link_info->link_arr[1].link_idx))
				{
					phy_chn_ctrl_0.bits.phy0_en = 1;
					phy_chn_ctrl_0.bits.phy1_en = 0;
					phy_chn_ctrl_0.bits.phy2_en = 1;
				}               
            }
            else
            {
                phy_chn_ctrl_0.bits.phy0_en = 0;
                phy_chn_ctrl_0.bits.phy1_en = 0;
            }
        }
        else if (3 == p_link_info->link_num)
        {
            if (flag)
            {
                phy_chn_ctrl_0.bits.phy0_en = 1;
                phy_chn_ctrl_0.bits.phy1_en = 1;
                phy_chn_ctrl_0.bits.phy2_en = 1;                
				phy_id.bits.phy2_id = 0;
            }
            else
            {
                phy_chn_ctrl_0.bits.phy0_en = 0;
                phy_chn_ctrl_0.bits.phy1_en = 0;
                phy_chn_ctrl_0.bits.phy2_en = 0;
				phy_id.bits.phy2_id = 0;
            }
        }
		gpMipiAllReg->stSysRegs.CHN0_PHY_EN.u32 = phy_chn_ctrl_0.u32 ;		
		gpMipiAllReg->stSysRegs.PHY_ID.u32 = phy_id.u32;
    }
    else if (1 == devno)
    { 
        /* sensor 1 can only use phy 2 */
        if (flag)
        {
			phy_chn_ctrl_1.bits.chn1_phy2_en = 1;
			phy_id.bits.phy2_id = 1;
        }
        else
        {
			phy_chn_ctrl_1.bits.chn1_phy2_en = 0;
			phy_id.bits.phy2_id = 0;
        }
		gpMipiAllReg->stSysRegs.CHN1_PHY_EN.u32 = phy_chn_ctrl_1.u32;		
		gpMipiAllReg->stSysRegs.PHY_ID.u32 = phy_id.u32;
    }

    RTL_Verify_RegWrite("set_phy_chn_ctrl", (unsigned int *)&(gpMipiAllReg->stSysRegs.CHN0_PHY_EN.u32),
            "PHY_CHN0_CTRL", gpMipiAllReg->stSysRegs.CHN0_PHY_EN.u32);
	RTL_Verify_RegWrite("set_phy_chn_ctrl", (unsigned int *)&(gpMipiAllReg->stSysRegs.CHN1_PHY_EN.u32),
            "PHY_CHN1_CTRL", gpMipiAllReg->stSysRegs.CHN1_PHY_EN.u32);
	RTL_Verify_RegWrite("PHY_ID", (unsigned int *)&(gpMipiAllReg->stSysRegs.PHY_ID.u32),
            "PHY_ID", gpMipiAllReg->stSysRegs.PHY_ID.u32);
}

//******************************************************************************
//  Function    : set_phy_chn_ctrl
//  Description : enable the PHY, and select the phy2 output to chn0 or chn1
//  Input       : devno, p_link_info
//  Return      : none
//******************************************************************************
void mipi_drv_set_phy_chn_ctrl(COMBO_DEV devno, link_info_s* p_link_info)
{
    phy_chn_ctrl(devno, p_link_info, HI_TRUE);
}

//******************************************************************************
//  Function    : clear_phy_chn_ctrl
//  Description : disalbe the PHY channel
//  Input       : devno, p_link_info
//  Return      : none
//******************************************************************************
void mipi_drv_clear_phy_chn_ctrl(COMBO_DEV devno, link_info_s* p_link_info)
{
    phy_chn_ctrl(devno, p_link_info, HI_FALSE);
}

//******************************************************************************
//  Function    : set_mipi_mem_cken
//  Description : enable or disable the MIPI memory clock gate
//  Input       : unsigned int umipi_mem_cken: 1 bits
//  Return      : none
//******************************************************************************
void mipi_drv_set_mem_cken(COMBO_DEV devno, HI_BOOL en)
{
	if(0 == devno)
	{
		U_CHN0_MEM_CTRL        CHN0_MEM_CTRL;	
		CHN0_MEM_CTRL.u32 = gpMipiAllReg->stSysRegs.CHN0_MEM_CTRL.u32;
		CHN0_MEM_CTRL.bits.mem_ck_gt = en;
		gpMipiAllReg->stSysRegs.CHN0_MEM_CTRL.u32 = CHN0_MEM_CTRL.u32;
	}
	else if(1 == devno)
	{
		U_CHN1_MEM_CTRL        CHN1_MEM_CTRL;	
		CHN1_MEM_CTRL.u32 = gpMipiAllReg->stSysRegs.CHN1_MEM_CTRL.u32;
		CHN1_MEM_CTRL.bits.chn1_mem_ck_gt = en;
		gpMipiAllReg->stSysRegs.CHN1_MEM_CTRL.u32 = CHN1_MEM_CTRL.u32;
	}
    RTL_Verify_RegWrite("set_mipi0_mem_cken", (unsigned int *)&(gpMipiAllReg->stSysRegs.CHN0_MEM_CTRL.u32),"U_MIPI_MEM_CTRL", gpMipiAllReg->stSysRegs.CHN0_MEM_CTRL.u32);
	RTL_Verify_RegWrite("set_mipi1_mem_cken", (unsigned int *)&(gpMipiAllReg->stSysRegs.CHN1_MEM_CTRL.u32),"U_MIPI_MEM_CTRL", gpMipiAllReg->stSysRegs.CHN1_MEM_CTRL.u32);
}

//******************************************************************************
//  Function    : set_mipi_rect
//  Description : set the mipi rect
//  Input       : @devno       MIPI Rx devno
//                @p_img_rect  MIPI image rect
//  Return      : none
//******************************************************************************
void mipi_drv_set_mipi_image_rect(COMBO_DEV devno, img_rect_t *p_img_rect)
{
    U_MIPI_IMGSIZE mipi_imgsize;
    U_MIPI_CROP_START_CHN0 crop_start_chn0;
    U_MIPI_CROP_START_CHN1 crop_start_chn1;
    U_MIPI_CROP_START_CHN2 crop_start_chn2;
    U_MIPI_CROP_START_CHN3 crop_start_chn3;
    MIPI_CTRL_REGS_S* pstCtrlReg = get_mipi_ctrl_regs(devno);

	if(HI_NULL == pstCtrlReg)
	{
		return;
	}

    mipi_imgsize.u32    = pstCtrlReg->MIPI_IMGSIZE.u32;
    crop_start_chn0.u32 = pstCtrlReg->MIPI_CROP_START_CHN0.u32;
    crop_start_chn1.u32 = pstCtrlReg->MIPI_CROP_START_CHN1.u32;
    crop_start_chn2.u32 = pstCtrlReg->MIPI_CROP_START_CHN2.u32;
    crop_start_chn3.u32 = pstCtrlReg->MIPI_CROP_START_CHN3.u32;

    mipi_imgsize.bits.mipi_imgwidth  = p_img_rect->width  - 1;
    mipi_imgsize.bits.mipi_imgheight = p_img_rect->height - 1;

    crop_start_chn0.bits.mipi_start_x_chn0 = p_img_rect->x;
    crop_start_chn0.bits.mipi_start_y_chn0 = p_img_rect->y;
    
    crop_start_chn1.bits.mipi_start_x_chn1 = p_img_rect->x;
    crop_start_chn1.bits.mipi_start_y_chn1 = p_img_rect->y;
    
    crop_start_chn2.bits.mipi_start_x_chn2 = p_img_rect->x;
    crop_start_chn2.bits.mipi_start_y_chn2 = p_img_rect->y;
    
    crop_start_chn3.bits.mipi_start_x_chn3 = p_img_rect->x;
    crop_start_chn3.bits.mipi_start_y_chn3 = p_img_rect->y;

    pstCtrlReg->MIPI_IMGSIZE.u32 = mipi_imgsize.u32;
    pstCtrlReg->MIPI_CROP_START_CHN0.u32 = crop_start_chn0.u32;
    pstCtrlReg->MIPI_CROP_START_CHN1.u32 = crop_start_chn1.u32;
    pstCtrlReg->MIPI_CROP_START_CHN2.u32 = crop_start_chn2.u32;
    pstCtrlReg->MIPI_CROP_START_CHN3.u32 = crop_start_chn3.u32;
    
    RTL_Verify_RegWrite("set_mipi_image_rect", (unsigned int *)&(pstCtrlReg->MIPI_IMGSIZE.u32),
            "MIPI_IMGSIZE", pstCtrlReg->MIPI_IMGSIZE.u32);
    RTL_Verify_RegWrite("set_mipi_image_rect", (unsigned int *)&(pstCtrlReg->MIPI_CROP_START_CHN0.u32),
            "MIPI_CROP_START_CHN0", pstCtrlReg->MIPI_CROP_START_CHN0.u32);
    RTL_Verify_RegWrite("set_mipi_image_rect", (unsigned int *)&(pstCtrlReg->MIPI_CROP_START_CHN1.u32),
            "MIPI_CROP_START_CHN1", pstCtrlReg->MIPI_CROP_START_CHN1.u32);
    RTL_Verify_RegWrite("set_mipi_image_rect", (unsigned int *)&(pstCtrlReg->MIPI_CROP_START_CHN2.u32),
            "MIPI_CROP_START_CHN2", pstCtrlReg->MIPI_CROP_START_CHN2.u32);
    RTL_Verify_RegWrite("set_mipi_image_rect", (unsigned int *)&(pstCtrlReg->MIPI_CROP_START_CHN3.u32),
            "MIPI_CROP_START_CHN3", pstCtrlReg->MIPI_CROP_START_CHN3.u32);
}

//******************************************************************************
//  Function    : set_mipi_crop_en
//  Description : enable/ disable MIPI crop
//  Input       : @devno    MIPI Rx devno
//                @en       crop en
//  Return      : none
//******************************************************************************
void mipi_drv_set_mipi_crop_en(COMBO_DEV devno, HI_BOOL en)
{
    U_MIPI_CTRL_MODE_PIXEL  ctrl_mode_pixel;
    MIPI_CTRL_REGS_S* pstCtrlReg = get_mipi_ctrl_regs(devno);

	if(HI_NULL == pstCtrlReg)
	{
		return;
	}

    ctrl_mode_pixel.u32 = pstCtrlReg->MIPI_CTRL_MODE_PIXEL.u32;

    ctrl_mode_pixel.bits.crop_en = en;

    pstCtrlReg->MIPI_CTRL_MODE_PIXEL.u32 = ctrl_mode_pixel.u32;

    RTL_Verify_RegWrite("set_mipi_crop_en", (unsigned int *)&(pstCtrlReg->MIPI_CTRL_MODE_PIXEL.u32),
            "MIPI_CTRL_MODE_PIXEL", pstCtrlReg->MIPI_CTRL_MODE_PIXEL.u32);
}

/*interrupts and get mipi attr*/

unsigned int mipi_drv_get_lanenum(COMBO_DEV devno)
{
    unsigned int value = 0;

    MIPI_CTRL_REGS_S* pstCtrlReg = get_mipi_ctrl_regs(devno);
    if (NULL != pstCtrlReg)
    {
        value = pstCtrlReg->MIPI_LANES_NUM.bits.lane_num;
    }

    return value + 1;
}

unsigned int mipi_drv_get_vc(COMBO_DEV devno, unsigned int data_id)
{
    MIPI_CTRL_REGS_S* pstCtrlReg;

    pstCtrlReg = get_mipi_ctrl_regs(devno);

    if (NULL != pstCtrlReg)
    {
        switch (data_id)
        {
        case 0:
            return pstCtrlReg->MIPI_DI_1.bits.di0_vc;
        case 1:
            return pstCtrlReg->MIPI_DI_1.bits.di1_vc;
        case 2:
            return pstCtrlReg->MIPI_DI_1.bits.di2_vc;
        case 3:
            return pstCtrlReg->MIPI_DI_1.bits.di3_vc;
        case 4:
            return pstCtrlReg->MIPI_DI_2.bits.di4_vc;
        case 5:
            return pstCtrlReg->MIPI_DI_2.bits.di5_vc;
        case 6:
            return pstCtrlReg->MIPI_DI_2.bits.di6_vc;
        case 7:
            return pstCtrlReg->MIPI_DI_2.bits.di7_vc;
        }
    }

    return 0;
}

raw_data_type_e mipi_drv_get_dt(COMBO_DEV devno, unsigned int data_id)
{
    unsigned int value = 0;
    MIPI_CTRL_REGS_S* pstCtrlReg;
    raw_data_type_e type = RAW_DATA_BUTT;

    pstCtrlReg = get_mipi_ctrl_regs(devno);
    if (NULL == pstCtrlReg)
    {
        return type;
    }

    switch (data_id)
    {
    case 0:
        value = pstCtrlReg->MIPI_DI_1.bits.di0_dt;
        break;
    case 1:
        value = pstCtrlReg->MIPI_DI_1.bits.di1_dt;
        break;
    case 2:
        value = pstCtrlReg->MIPI_DI_1.bits.di2_dt;
        break;
    case 3:
        value = pstCtrlReg->MIPI_DI_1.bits.di3_dt;
        break;
    case 4:
        value = pstCtrlReg->MIPI_DI_2.bits.di4_dt;
        break;
    case 5:
        value = pstCtrlReg->MIPI_DI_2.bits.di5_dt;
        break;
    case 6:
        value = pstCtrlReg->MIPI_DI_2.bits.di6_dt;
        break;
    case 7:
        value = pstCtrlReg->MIPI_DI_2.bits.di7_dt;
        break;
    default:
        break;
    }

    if (0x2A == value)
    {
        type = RAW_DATA_8BIT;
    }
    if(0x2b == value)
    {
        type = RAW_DATA_10BIT;
    }
    else if(0x2c == value)
    {
        type = RAW_DATA_12BIT;
    }
    else if(0x2d == value)
    {
        type = RAW_DATA_14BIT;
    }

    return type;
}

//******************************************************************************
//  Function    : set_lvds_image_rect
//  Description : set the lvds image rect
//  Input       : @devno            MIPI Rx devno
//                @p_img_rect       LVDS image rect
//                @total_lane_num   lane number for this MIPI Rx dev
//  Return      : none
//******************************************************************************
int mipi_drv_set_lvds_image_rect(COMBO_DEV devno, img_rect_t* p_img_rect, short total_lane_num)
{
    LVDS_CTRL_REGS_S* pstCtrlReg;
    U_LVDS_IMGSIZE lvds_img_size;
    U_LVDS_CROP_START0 crop_start0;
    U_LVDS_CROP_START1 crop_start1;
    U_LVDS_CROP_START2 crop_start2;
    U_LVDS_CROP_START3 crop_start3;
    unsigned int width_per_lane, x_per_lane;

    pstCtrlReg = get_lvds_ctrl_regs(devno);
    if (NULL == pstCtrlReg)
    {
        HI_ERR("get lvds_ctrl reg failed\n");
        return HI_FAILURE;
    }

    width_per_lane = (p_img_rect->width / total_lane_num);
    x_per_lane = (p_img_rect->x / total_lane_num);

    lvds_img_size.u32 = pstCtrlReg->LVDS_IMGSIZE.u32;
    crop_start0.u32 = pstCtrlReg->LVDS_CROP_START0.u32;
    crop_start1.u32 = pstCtrlReg->LVDS_CROP_START1.u32;
    crop_start2.u32 = pstCtrlReg->LVDS_CROP_START2.u32;
    crop_start3.u32 = pstCtrlReg->LVDS_CROP_START3.u32;

    lvds_img_size.bits.lvds_imgwidth_lane = width_per_lane - 1;
    lvds_img_size.bits.lvds_imgheight = p_img_rect->height - 1;

    crop_start0.bits.lvds_start_x0_lane = x_per_lane;
    crop_start0.bits.lvds_start_y0 =p_img_rect->y;
    
    crop_start1.bits.lvds_start_x1_lane = x_per_lane;
    crop_start1.bits.lvds_start_y1 =p_img_rect->y;
    
    crop_start2.bits.lvds_start_x2_lane = x_per_lane;
    crop_start2.bits.lvds_start_y2 =p_img_rect->y;
    
    crop_start3.bits.lvds_start_x3_lane = x_per_lane;
    crop_start3.bits.lvds_start_y3 =p_img_rect->y;

    pstCtrlReg->LVDS_IMGSIZE.u32 = lvds_img_size.u32;
    pstCtrlReg->LVDS_CROP_START0.u32 = crop_start0.u32;
    pstCtrlReg->LVDS_CROP_START1.u32 = crop_start1.u32;
    pstCtrlReg->LVDS_CROP_START2.u32 = crop_start2.u32;
    pstCtrlReg->LVDS_CROP_START3.u32 = crop_start3.u32;

    RTL_Verify_RegWrite((char*)__FUNCTION__, (unsigned int *)&(pstCtrlReg->LVDS_IMGSIZE.u32), 
            "LVDS_IMGSIZE", pstCtrlReg->LVDS_IMGSIZE.u32);
    RTL_Verify_RegWrite((char*)__FUNCTION__, (unsigned int *)&(pstCtrlReg->LVDS_CROP_START0.u32), 
            "LVDS_CROP_START0", pstCtrlReg->LVDS_CROP_START0.u32);
    RTL_Verify_RegWrite((char*)__FUNCTION__, (unsigned int *)&(pstCtrlReg->LVDS_CROP_START1.u32), 
            "LVDS_CROP_START1", pstCtrlReg->LVDS_CROP_START1.u32);
    RTL_Verify_RegWrite((char*)__FUNCTION__, (unsigned int *)&(pstCtrlReg->LVDS_CROP_START2.u32), 
            "LVDS_CROP_START2", pstCtrlReg->LVDS_CROP_START2.u32);
    RTL_Verify_RegWrite((char*)__FUNCTION__, (unsigned int *)&(pstCtrlReg->LVDS_CROP_START3.u32), 
            "LVDS_CROP_START3", pstCtrlReg->LVDS_CROP_START3.u32);
    return 0;
}

//******************************************************************************
//  Function    : set_lvds_crop_en
//  Description : enable/disable LVDS crop
//  Input       : @devno    MIPI Rx devno
//                @en       crop en
//  Return      : none
//******************************************************************************
void mipi_drv_set_lvds_crop_en(COMBO_DEV devno, HI_BOOL en)
{
    LVDS_CTRL_REGS_S* pstCtrlReg;
    U_LVDS_CTRL lvds_ctrl;

    pstCtrlReg = get_lvds_ctrl_regs(devno);

	if(HI_NULL == pstCtrlReg)
	{
		return;
	}

    lvds_ctrl.u32 = pstCtrlReg->LVDS_CTRL.u32;

    lvds_ctrl.bits.lvds_crop_en = en;

    pstCtrlReg->LVDS_CTRL.u32 = lvds_ctrl.u32;

    RTL_Verify_RegWrite("set_lvds_crop_en", (unsigned int *)&(pstCtrlReg->LVDS_CTRL.u32), 
            "LVDS_CTRL", pstCtrlReg->LVDS_CTRL.u32);
}


int mipi_drv_set_lvds_wdr_mode(COMBO_DEV devno, wdr_mode_e wdr_mode, lvds_vsync_type_t* vsync_type, lvds_fid_type_t* fid_type)
{
    LVDS_CTRL_REGS_S* pstCtrlReg;
    U_LVDS_WDR lvds_wdr;
    U_LVDS_DOLSCD_HBLK scd_hblk;

    pstCtrlReg = get_lvds_ctrl_regs(devno);
    if (NULL == pstCtrlReg)
    {
        HI_ERR("get lvds_ctrl reg failed\n");
        return HI_FAILURE;
    }

    lvds_wdr.u32 = pstCtrlReg->LVDS_WDR.u32;
    scd_hblk.u32 = pstCtrlReg->LVDS_DOLSCD_HBLK.u32;

    if (HI_WDR_MODE_NONE == wdr_mode)
    {
        lvds_wdr.bits.lvds_wdr_en = 0;
        lvds_wdr.bits.lvds_wdr_num = 0;
    }
    else
    {
        lvds_wdr.bits.lvds_wdr_en = 1;

        // set the wdr frame number
        switch (wdr_mode)
        {
        case HI_WDR_MODE_2F:
        case HI_WDR_MODE_DOL_2F:
            lvds_wdr.bits.lvds_wdr_num = 1;
            break;
        case HI_WDR_MODE_3F:
        case HI_WDR_MODE_DOL_3F:
            lvds_wdr.bits.lvds_wdr_num = 2;
            break;
        case HI_WDR_MODE_4F:
        case HI_WDR_MODE_DOL_4F:
            lvds_wdr.bits.lvds_wdr_num = 3;
            break;
        default:
            HI_ERR("not support WDR_MODE: %d\n", wdr_mode);
            return HI_FAILURE;
        }

        // set wdr mode
        if (wdr_mode >= HI_WDR_MODE_2F && wdr_mode <= HI_WDR_MODE_4F)
        {
            if (LVDS_VSYNC_NORMAL == vsync_type->sync_type)
            {
				// SOF-EOF WDR, long exposure frame and short exposure frame has independent sync code
                lvds_wdr.bits.lvds_wdr_mode = 0x0;
            }
            else if (LVDS_VSYNC_SHARE== vsync_type->sync_type)
            {
				// SOF-EOF WDR, long exposure frame and short exposure frame share the SOF and EOF
                lvds_wdr.bits.lvds_wdr_mode = 0x2;
            }
            else
            {
                HI_ERR("not support vsync type: %d\n", vsync_type->sync_type);
                return HI_FAILURE;
            }
        }
        else if (wdr_mode >= HI_WDR_MODE_DOL_2F && wdr_mode <= HI_WDR_MODE_DOL_4F)
        {
            // Sony DOL WDR
            if (LVDS_VSYNC_NORMAL == vsync_type->sync_type)
            {
                // SAV-EAV WDR, 4 sync code, fid embedded in 4th sync code
                // long exposure fame and short exposure frame has independent sync code
                if (LVDS_FID_IN_SAV == fid_type->fid)
                {
                    lvds_wdr.bits.lvds_wdr_mode = 0x4;
                }
                else if (LVDS_FID_IN_DATA== fid_type->fid)
                {
					// SAV-EAV WDR, 5 sync code(Line Information), fid in the fist DATA, 
                    /*  fid in data, line information */
                    if(fid_type->output_fil)
                    {
						// Frame Information Line is included in the image data
                        lvds_wdr.bits.lvds_wdr_mode = 0xd;
                    }
                    else
                    {
						// Frame Information Line is not included in the image data
                        lvds_wdr.bits.lvds_wdr_mode = 0x6; 
                    }
                }
                else
                {
                    HI_ERR("not support fid type: %d\n", fid_type->fid);
                    return HI_FAILURE;
                }
            }
            else if (LVDS_VSYNC_HCONNECT== vsync_type->sync_type)
            {
                // SAV-EAV H-Connection DOL, long exposure frame and short exposure frame 
                // share the same SAV EAV, the H-Blank is assigned by the dol_hblank1 and dol_hblank2
                if (LVDS_FID_NONE == fid_type->fid)
                {
                    lvds_wdr.bits.lvds_wdr_mode = 0x5;
                }
				#if 0
                else if (LVDS_FID_IN_DATA== fid_type->fid)
                {
                    if(fid_type->output_fil)
                    {
                        /* with frame information line output to VI */
                        lvds_wdr.bits.lvds_wdr_mode = 0b1110;
                    }
                    else
                    {
                        lvds_wdr.bits.lvds_wdr_mode = 0b110; 
                    }
                }
				#endif
                else
                {
                    HI_ERR("not support fid type: %d\n", fid_type->fid);
                    return HI_FAILURE;
                }
                scd_hblk.bits.dol_hblank1 = vsync_type->hblank1;
                scd_hblk.bits.dol_hblank2 = vsync_type->hblank2;
            }
            else
            {
                HI_ERR("not support vsync type: %d\n", vsync_type->sync_type);
                return HI_FAILURE;
            }
        }
    }

    pstCtrlReg->LVDS_WDR.u32 = lvds_wdr.u32;
    pstCtrlReg->LVDS_DOLSCD_HBLK.u32 = scd_hblk.u32;

    RTL_Verify_RegWrite(__FUNCTION__, (unsigned int *)&(pstCtrlReg->LVDS_WDR.u32), 
            "LVDS_WDR_MODE", pstCtrlReg->LVDS_WDR.u32);
    RTL_Verify_RegWrite(__FUNCTION__, (unsigned int *)&(pstCtrlReg->LVDS_DOLSCD_HBLK.u32), 
            "LVDS_DOLSCD_HBLK", pstCtrlReg->LVDS_DOLSCD_HBLK.u32);
    return HI_SUCCESS;
}

int mipi_drv_set_lvds_ctrl_mode(COMBO_DEV devno, lvds_sync_mode_e sync_mode, 
                                raw_data_type_e raw_data_type, 
                                lvds_bit_endian  data_endian, 
                                lvds_bit_endian sync_code_endian,
                                HI_BOOL b_disorder)
{
    LVDS_CTRL_REGS_S* pstCtrlReg;
    U_LVDS_CTRL lvds_ctrl;
    unsigned short raw_type;

    pstCtrlReg = get_lvds_ctrl_regs(devno);
    if (NULL == pstCtrlReg)
    {
        HI_ERR("get lvds_ctrl reg failed\n");
        return HI_FAILURE;
    }

    lvds_ctrl.u32 = pstCtrlReg->LVDS_CTRL.u32;

    switch (raw_data_type)
    {
    case RAW_DATA_8BIT:
        raw_type = 0x1;
        break;
    case RAW_DATA_10BIT:
        raw_type = 0x2;
        break;
    case RAW_DATA_12BIT:
        raw_type = 0x3;
        break;
    case RAW_DATA_14BIT:
        raw_type = 0x4;
        break;
    case RAW_DATA_16BIT:
        raw_type = 0x5;
        break;
    default:
        return HI_FAILURE;
    }

    lvds_ctrl.bits.lvds_sync_mode = sync_mode;
    lvds_ctrl.bits.lvds_raw_type = raw_type;
    lvds_ctrl.bits.lvds_pix_big_endian = data_endian;
    lvds_ctrl.bits.lvds_code_big_endian = sync_code_endian;
    //lvds_ctrl.bits.lvds_link_seq = b_disorder;

    pstCtrlReg->LVDS_CTRL.u32 = lvds_ctrl.u32;

    RTL_Verify_RegWrite(__FUNCTION__, (unsigned int *)&(pstCtrlReg->LVDS_CTRL.u32), 
            "LVDS_CTRL", pstCtrlReg->LVDS_CTRL.u32);
    return 0;
}

static void inline set_lane_en(COMBO_DEV devno, U_CHN0_LANE_EN* p_lane_en, 
								COMBO_LINK link_id, int lane_idx, HI_BOOL en)
{
	if(0 == devno)
{
    if (0 == link_id)
    {
        switch (lane_idx)
        {
        case 0:
            p_lane_en->bits.lane0_en = en;
            break;
        case 1:
            p_lane_en->bits.lane1_en = en;
            break;
        case 2:
            p_lane_en->bits.lane2_en = en;
            break;
        case 3:
            p_lane_en->bits.lane3_en = en;
            break;
        }
    }
    else if (1 == link_id)
    {
        switch (lane_idx)
        {
        case 0:
            p_lane_en->bits.lane4_en = en;
            break;
        case 1:
            p_lane_en->bits.lane5_en = en;
            break;
        case 2:
            p_lane_en->bits.lane6_en = en;
            break;
        case 3:
            p_lane_en->bits.lane7_en = en;
            break;
        }
    }
    else if (2 == link_id)
    {
        switch (lane_idx)
        {
        case 0:
            p_lane_en->bits.lane8_en = en;
            break;
        case 1:
            p_lane_en->bits.lane9_en = en;
            break;
        case 2:
            p_lane_en->bits.lane10_en = en;
            break;
        case 3:
            p_lane_en->bits.lane11_en = en;
            break;
        }
    }
}
	else if(1 == devno)
	{
		if (2 == link_id)
	    {
	        switch (lane_idx)
	        {
	        case 0:
	            p_lane_en->bits.lane0_en = en;
	            break;
	        case 1:
	            p_lane_en->bits.lane1_en = en;
	            break;
	        case 2:
	            p_lane_en->bits.lane2_en = en;
	            break;
	        case 3:
	            p_lane_en->bits.lane3_en = en;
	            break;
	        }
		}	
	}
	else
	{
	}
}

//******************************************************************************
//  Function    : set_link_lane_id
//  Description : set the lane id and lane en
//  Input       : unsigned int ufrsck: 1 bits
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int mipi_drv_set_link_lane_id(COMBO_DEV devno, COMBO_LINK link_id, short lane_id[LANE_NUM_PER_LINK])
{
    int i;
    U_LANE_ID_CHN lane_id_link_chn;
    volatile U_LANE_ID_CHN *p_lane_id_link_chn = NULL;
    U_CHN0_LANE_EN      chn0_lane_en, chn1_lane_en;	


    if (devno >= COMBO_MAX_DEV_NUM)
    {
        HI_ERR("invalid devno: %d\n", devno);
        return -1;
    }
    if (link_id >= COMBO_MAX_LINK_NUM)
    {
        HI_ERR("invalid link_id: %d\n", link_id);
        return -1;
    }

    /* set lane_en */
    if (0 == devno)
    {    
		chn0_lane_en.u32 = gpMipiAllReg->stSysRegs.CHN0_LANE_EN.u32;
    for (i = 0; i < LANE_NUM_PER_LINK; i++)
    {
			set_lane_en(devno, &chn0_lane_en, link_id, i, IS_VALID_ID(lane_id[i]));
    }

		gpMipiAllReg->stSysRegs.CHN0_LANE_EN.u32 = chn0_lane_en.u32;
    
        p_lane_id_link_chn = &gpMipiAllReg->stComboDevRegs[devno].LANE_ID_CHN[link_id];
    }
    else if (1 == devno)
    {
		chn1_lane_en.u32 = gpMipiAllReg->stSysRegs.CHN1_LANE_EN.u32;
		for (i = 0; i < LANE_NUM_PER_LINK; i++)
		{
			set_lane_en(devno, &chn1_lane_en, link_id, i, IS_VALID_ID(lane_id[i]));
		}
		gpMipiAllReg->stSysRegs.CHN1_LANE_EN.u32 = chn1_lane_en.u32;
        // chn 1 only have one link
        p_lane_id_link_chn = &gpMipiAllReg->stComboDevRegs[devno].LANE_ID_CHN[0];
    }
    else
    {
        HI_ERR("invalid devno: %d\n", devno);
        return -1;
    }

    lane_id_link_chn.u32 = (*p_lane_id_link_chn).u32;
    if (lane_id[0] >= 0)
    {
        lane_id_link_chn.bits.lane0_id = lane_id[0];
    }
    if (lane_id[1] >= 0)
    {
        lane_id_link_chn.bits.lane1_id = lane_id[1];
    }
    if (lane_id[2] >= 0)
    {
        lane_id_link_chn.bits.lane2_id = lane_id[2];
    }
    if (lane_id[3] >= 0)
    {
        lane_id_link_chn.bits.lane3_id = lane_id[3];
    }

    p_lane_id_link_chn->u32 = lane_id_link_chn.u32;

	RTL_Verify_RegWrite(__FUNCTION__, (unsigned int *)&(gpMipiAllReg->stSysRegs.CHN0_LANE_EN.u32), 
	    "CHN0_LANE_EN", gpMipiAllReg->stSysRegs.CHN0_LANE_EN.u32);
	RTL_Verify_RegWrite(__FUNCTION__, (unsigned int *)&(gpMipiAllReg->stSysRegs.CHN1_LANE_EN.u32), 
	    "CHN1_LANE_EN", gpMipiAllReg->stSysRegs.CHN1_LANE_EN.u32);
    RTL_Verify_RegWrite(__FUNCTION__, (unsigned int *)&(p_lane_id_link_chn->u32), 
            "LANE_ID_CHN", p_lane_id_link_chn->u32);
    return 0;
}

//******************************************************************************
//  Function    : set_lvds_sync_code
//  Description : Set LVDS N Frame or N+1 Frame sync code, 
//                from LVDSx_Lane0_SOF_01 to LVDSx_Lanen_EOL23, total lane_cnt lanes
//  Input       : devno, n_frame, lane_cnt, sync_code
//                sync_code's length is lane_cnt
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
static int set_lvds_sync_code(COMBO_DEV devno, HI_BOOL n_frame, unsigned int lane_cnt,
                    unsigned short sync_code[][WDR_VC_NUM][SYNC_CODE_NUM])
{
    int i;
    LVDS_CTRL_REGS_S* pstCtrlReg;
    LVDS_SYNC_CODE_S lane_sync_code, *p_sync_code;

    pstCtrlReg = get_lvds_ctrl_regs(devno);
    if (NULL == pstCtrlReg)
    {
        HI_ERR("get LVDS_CTRL registers failed, devno: %d\n", devno);
        return HI_FAILURE;
    }

    //osal_printk("%s() devno:%d, lane_cnt:%d\n", __FUNCTION__, devno, lane_cnt);
    for(i = 0; i < lane_cnt; i++)
    {
        if (1 == devno && i > 3)
        {
            // sensor 1 only have 4 lane
            break;
        }

        if (HI_TRUE == n_frame)
        {
            p_sync_code = &pstCtrlReg->N_SYNC_CODE[i];
        }
        else
        {
            p_sync_code = &pstCtrlReg->NXT_SYNC_CODE[i];
        }

        lane_sync_code = *p_sync_code;

        {
            U_LVDS_LANE_SOF_01 lvds_sof_01;
            lvds_sof_01.u32 = lane_sync_code.LANE_SOF_01.u32;
            lvds_sof_01.bits.lane_sof_0 = sync_code[i][0][0];
            lvds_sof_01.bits.lane_sof_1 = sync_code[i][1][0];
            p_sync_code->LANE_SOF_01.u32 = lvds_sof_01.u32;
        }
        {
            U_LVDS_LANE_SOF_23 lvds_sof_23;
            lvds_sof_23.u32 = lane_sync_code.LANE_SOF_23.u32;
            lvds_sof_23.bits.lane_sof_2 = sync_code[i][2][0];
            lvds_sof_23.bits.lane_sof_3 = sync_code[i][3][0];
            p_sync_code->LANE_SOF_23.u32 = lvds_sof_23.u32;
        }
        {
            U_LVDS_LANE_EOF_01 lvds_eof_01;
            lvds_eof_01.u32 = lane_sync_code.LANE_EOF_01.u32;
            lvds_eof_01.bits.lane_eof_0 = sync_code[i][0][1];
            lvds_eof_01.bits.lane_eof_1 = sync_code[i][1][1];
            p_sync_code->LANE_EOF_01.u32 = lvds_eof_01.u32;
        }
        {
            U_LVDS_LANE_EOF_23 lvds_eof_23;
            lvds_eof_23.u32 = lane_sync_code.LANE_EOF_23.u32;
            lvds_eof_23.bits.lane_eof_2 = sync_code[i][2][1];
            lvds_eof_23.bits.lane_eof_3 = sync_code[i][3][1];
            p_sync_code->LANE_EOF_23.u32 = lvds_eof_23.u32;
        }
        {
            U_LVDS_LANE_SOL_01 lvds_sol_01;
            lvds_sol_01.u32 = lane_sync_code.LANE_SOL_01.u32;
            lvds_sol_01.bits.lane_sol_0 = sync_code[i][0][2];
            lvds_sol_01.bits.lane_sol_1 = sync_code[i][1][2];
            p_sync_code->LANE_SOL_01.u32 = lvds_sol_01.u32;
        }
        {
            U_LVDS_LANE_SOL_23 lvds_sol_23;
            lvds_sol_23.u32 = lane_sync_code.LANE_SOL_23.u32;
            lvds_sol_23.bits.lane_sol_2 = sync_code[i][2][2];
            lvds_sol_23.bits.lane_sol_3 = sync_code[i][3][2];
            p_sync_code->LANE_SOL_23.u32 = lvds_sol_23.u32;
        }
        {
            U_LVDS_LANE_EOL_01 lvds_eol_01;
            lvds_eol_01.u32 = lane_sync_code.LANE_EOL_01.u32;
            lvds_eol_01.bits.lane_eol_0 = sync_code[i][0][3];
            lvds_eol_01.bits.lane_eol_1 = sync_code[i][1][3];
            p_sync_code->LANE_EOL_01.u32 = lvds_eol_01.u32;
        }
        {
            U_LVDS_LANE_EOL_23 lvds_eol_23;
            lvds_eol_23.u32 = lane_sync_code.LANE_EOL_23.u32;
            lvds_eol_23.bits.lane_eol_2 = sync_code[i][2][3];
            lvds_eol_23.bits.lane_eol_3 = sync_code[i][3][3];
            p_sync_code->LANE_EOL_23.u32 = lvds_eol_23.u32;
        }

        RTL_Verify_RegWrite(__FUNCTION__, (unsigned int *)&(p_sync_code->LANE_SOF_01.u32), "LINK_LANE_SOF_01", p_sync_code->LANE_SOF_01.u32);
        RTL_Verify_RegWrite(__FUNCTION__, (unsigned int *)&(p_sync_code->LANE_SOF_23.u32), "LINK_LANE_SOF_23", p_sync_code->LANE_SOF_23.u32);
        RTL_Verify_RegWrite(__FUNCTION__, (unsigned int *)&(p_sync_code->LANE_EOF_01.u32), "LINK_LANE_EOF_01", p_sync_code->LANE_EOF_01.u32);
        RTL_Verify_RegWrite(__FUNCTION__, (unsigned int *)&(p_sync_code->LANE_EOF_23.u32), "LINK_LANE_EOF_23", p_sync_code->LANE_EOF_23.u32);
        RTL_Verify_RegWrite(__FUNCTION__, (unsigned int *)&(p_sync_code->LANE_SOL_01.u32), "LINK_LANE_SOL_01", p_sync_code->LANE_SOL_01.u32);
        RTL_Verify_RegWrite(__FUNCTION__, (unsigned int *)&(p_sync_code->LANE_SOL_23.u32), "LINK_LANE_SOL_23", p_sync_code->LANE_SOL_23.u32);
        RTL_Verify_RegWrite(__FUNCTION__, (unsigned int *)&(p_sync_code->LANE_EOL_01.u32), "LINK_LANE_EOL_01", p_sync_code->LANE_EOL_01.u32);
        RTL_Verify_RegWrite(__FUNCTION__, (unsigned int *)&(p_sync_code->LANE_EOL_23.u32), "LINK_LANE_EOL_23", p_sync_code->LANE_EOL_23.u32);
    }
    return 0;
}

//******************************************************************************
//  Function    : set_lvds_nxt_sync_code
//  Description : Set LVDS N frame sync code
//  Input       : devno, lane_cnt, sync_code
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int mipi_drv_set_lvds_sync_code(COMBO_DEV devno, unsigned int lane_cnt,
                    unsigned short sync_code[][WDR_VC_NUM][SYNC_CODE_NUM])
{
	return set_lvds_sync_code(devno, HI_TRUE, lane_cnt, sync_code);
}

//******************************************************************************
//  Function    : set_lvds_nxt_sync_code
//  Description : Set LVDS N+1 frame sync code
//  Input       : devno, lane_cnt, sync_code
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
int mipi_drv_set_lvds_nxt_sync_code(COMBO_DEV devno, unsigned int lane_cnt, 
                    unsigned short sync_code[][WDR_VC_NUM][SYNC_CODE_NUM])
{
    return set_lvds_sync_code(devno, HI_FALSE, lane_cnt, sync_code);
}

//******************************************************************************
//  Function    : set_dol_line_information
//  Description : Set Sony DOL line information
//  Input       : devno, raw_type
//  Return      : void
//******************************************************************************
void mipi_drv_set_dol_line_information(COMBO_DEV devno, wdr_mode_e wdr_mode, raw_data_type_e raw_type)
{
    LVDS_CTRL_REGS_S* pstCtrlReg;

    pstCtrlReg = get_lvds_ctrl_regs(devno);
    MIPI_CHECK_NULL_PTR(pstCtrlReg);

    if (wdr_mode >= HI_WDR_MODE_DOL_2F)
    {
        pstCtrlReg->LVDS_LI_WORD0.bits.li_word0_0 = 0x0201;    // LEF n frame
        pstCtrlReg->LVDS_LI_WORD0.bits.li_word0_1 = 0x0211;    // LEF n + 1 frame

        pstCtrlReg->LVDS_LI_WORD1.bits.li_word1_0 = 0x0202;    // SEF1 n frame
        pstCtrlReg->LVDS_LI_WORD1.bits.li_word1_1 = 0x0212;    // SEF1 n + 1 frame
    }

    if (wdr_mode >= HI_WDR_MODE_DOL_3F)
    {
        pstCtrlReg->LVDS_LI_WORD2.bits.li_word2_0 = 0x0204;    // SEF2 n frame
        pstCtrlReg->LVDS_LI_WORD2.bits.li_word2_1 = 0x0214;    // SEF2 n + 1 frame
    }

    if (wdr_mode >= HI_WDR_MODE_DOL_4F)
    {
        pstCtrlReg->LVDS_LI_WORD3.bits.li_word3_0 = 0x0208;    // SEF3 n frame
        pstCtrlReg->LVDS_LI_WORD3.bits.li_word3_1 = 0x0218;    // SEF3 n + 1 frame
    }
}

//******************************************************************************
//  Function    : set phy_sync_code
//  Description : set LVDS sync code head(sof) for n frame and n+1 frame 
//  Input       : link_id, 
//                lane_idx, lane's priority, array size must less equal LANE_NUM_PER_LINK
//                sync_code, N frame's sync code
//                nxt_sync_code, N+1 frame's sync code
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
void mipi_drv_set_lvds_phy_sync_code(COMBO_LINK link_id, short lane_idx[], 
                    unsigned short   n_sync_code[][WDR_VC_NUM][SYNC_CODE_NUM], 
                    unsigned short nxt_sync_code[][WDR_VC_NUM][SYNC_CODE_NUM]) 
{
    int i;
    U_PHY_SYNC_CODE_LINK phy_sync_code;
    MIPI_LINK_REGS_S* pstLinkReg;
    short lane_id;

    pstLinkReg = get_mipi_link_regs(link_id);
    MIPI_CHECK_NULL_PTR(pstLinkReg);

    for (i = 0; i < LANE_NUM_PER_LINK; i++)
    {
        lane_id = lane_idx[i];
        if (-1 != lane_id)
        {
            phy_sync_code.u32 = pstLinkReg->PHY_SYNC_CODE_LINK[i].u32;
            phy_sync_code.bits.cil_sof0_word4 =   n_sync_code[lane_id][0][0];
            phy_sync_code.bits.cil_sof1_word4 = nxt_sync_code[lane_id][0][0];
            pstLinkReg->PHY_SYNC_CODE_LINK[i].u32 = phy_sync_code.u32;
            RTL_Verify_RegWrite("set_lvds_phy_sync_code", 
                    (unsigned int *)&(pstLinkReg->PHY_SYNC_CODE_LINK[i].u32), 
                    "PHY_SYNC_CODE_LINK", pstLinkReg->PHY_SYNC_CODE_LINK[i].u32);
        }
    }
}

//******************************************************************************
//  Function    : set set_phy_sync_dct
//  Description : set the lvds sync head Detect and ConTrol register  
//  Input       : link_id, raw_data_type, data_endian
//                lane_idx: an array, size is LANE_NUM_PER_LINK 
//  Return      : int: 0-Error, 1-Success
//******************************************************************************
void mipi_drv_set_phy_sync_dct(COMBO_LINK link_id, raw_data_type_e raw_data_type, lvds_bit_endian code_endian)
{
    U_PHY_SYNC_DCT_LINK sync_code_dct;
    MIPI_LINK_REGS_S* pstLinkReg;

    pstLinkReg = get_mipi_link_regs(link_id);
    MIPI_CHECK_NULL_PTR(pstLinkReg);

    sync_code_dct.u32 = pstLinkReg->PHY_SYNC_DCT_LINK.u32;
    switch (raw_data_type)
    {
    case RAW_DATA_8BIT:
        sync_code_dct.bits.cil_raw_type = 0x1;
        break;
    case RAW_DATA_10BIT:
        sync_code_dct.bits.cil_raw_type = 0x2;
        break;
    case RAW_DATA_12BIT:
        sync_code_dct.bits.cil_raw_type = 0x3;
        break;
    case RAW_DATA_14BIT:
        sync_code_dct.bits.cil_raw_type = 0x4;
        break;
    case RAW_DATA_16BIT:
        sync_code_dct.bits.cil_raw_type = 0x5;
        break;
    default:
        break;
    }
    sync_code_dct.bits.cil_code_big_endian = code_endian;

    pstLinkReg->PHY_SYNC_DCT_LINK.u32 = sync_code_dct.u32;

    RTL_Verify_RegWrite("set_phy_sync_dct", (unsigned int *)&(pstLinkReg->PHY_SYNC_DCT_LINK.u32), 
            "PHY_SYNC_DCT_LINK", pstLinkReg->PHY_SYNC_DCT_LINK.u32);
}

//****************************************************************************** 
// Hi3519 no need to use the start flag
//******************************************************************************
int mipi_drv_set_phy_reg_start(HI_BOOL en)
{
#if 0
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

    RTL_Verify_RegWrite(__FUNCTION__, (unsigned int *)&(gpMipiAllReg->MIPI_START.u32), 
                        "PHYCIL_REG_START", gpMipiAllReg->MIPI_START.u32);
#endif
    return 0;
}

//****************************************************************************** 
// This Register in hi3519 is PHY_CFG_LINK, it is reserved and not visible
//******************************************************************************
void mipi_drv_init_phy_cfg(COMBO_LINK link_id)
{
#if 0
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
#endif
}

//******************************************************************************
//  Function    : set_phy_mode
//  Description : set the phy work mode
//  Input       : link_id, work_mode, phy_clk_share, lane_id array
//  Return      : void
//******************************************************************************
void mipi_drv_set_phy_mode(COMBO_LINK link_id, work_mode_t work_mode, 
        phy_clk_share_e phy_clk_share, short lane_id[LANE_NUM_PER_LINK])
{
    U_PHY_MODE_LINK phycil_mode;
    MIPI_LINK_REGS_S* pstLinkReg;
    unsigned int phy_data_en = 0;
    int i;

    pstLinkReg = get_mipi_link_regs(link_id);
    MIPI_CHECK_NULL_PTR(pstLinkReg);

#if 1
    for (i = 0; i < LANE_NUM_PER_LINK; i++)
    {
        phy_data_en |= ( (IS_VALID_ID(lane_id[i]) ? 1 : 0) << i);
    }
#else
    phy_data_en = (IS_VALID_ID(lane_id[0]) | IS_VALID_ID(lane_id[1]) << 1
            | IS_VALID_ID(lane_id[2]) << 2 | IS_VALID_ID(lane_id[3]) << 3);
#endif

    if (0 != phy_data_en)
    {
        phycil_mode.u32 = pstLinkReg->PHY_MODE_LINK.u32;

        // set data lane and clock clane enable, if some lanes is enable, then enable clk
        phycil_mode.bits.phy_rg_en_d    = phy_data_en;
        phycil_mode.bits.phy_rg_en_clk  = 1;

			
        phycil_mode.bits.phy_rg_en_lp   = (WORK_MODE_MIPI == work_mode) ? 1 : 0;

        // MIPI mode set the common voltage less than 900 mv
        phycil_mode.bits.phy_rg_mipi_mode = 1;

        // set the clock edge down or up
        phycil_mode.bits.phy_rg_faclk_en = 1;

        // set the phy clock_share mode, link0 can not choose clock share
        // in FPGA version, the phy2 is not in same the chip with PHY0 and PHY1, so can not share clock with phy0
#ifndef HI_FPGA
        if (0 != link_id && PHY_CLK_SHARE_PHY0 == phy_clk_share)
#else
        if (1 == link_id && PHY_CLK_SHARE_PHY0 == phy_clk_share)
#endif
        {
            phycil_mode.bits.phy_rg_ext_clk_en = 1;
        }
        else
        {
            phycil_mode.bits.phy_rg_ext_clk_en = 0;
        }
    }
    else
    {
        // if phy_data_en is 0, then set the register to default value
        phycil_mode.u32 = 0;
    }
    if (WORK_MODE_CMOS   == work_mode ||
        WORK_MODE_BT1120 == work_mode)
    {
        phycil_mode.bits.phy_rg_en_cmos = 1;
	}
    else
    {
        phycil_mode.bits.phy_rg_en_cmos = 0;
    }

    pstLinkReg->PHY_MODE_LINK.u32 = phycil_mode.u32;

    RTL_Verify_RegWrite("mipi_drv_set_phy_mode", (unsigned int *)&(pstLinkReg->PHY_MODE_LINK.u32),
            "U_PHY_MODE_LINK", pstLinkReg->PHY_MODE_LINK.u32);
}

//******************************************************************************
//  Function    : mipi_drv_set_phy_cmv
//  Description : set the phy common voltage mode
//  Input       : link_id, cmv_mode
//  Return      : void
//******************************************************************************
void mipi_drv_set_phy_cmv(COMBO_LINK link_id, phy_cmv_e cmv_mode)
{
    U_PHY_MODE_LINK phycil_mode;
    MIPI_LINK_REGS_S* pstLinkReg;

    pstLinkReg = get_mipi_link_regs(link_id);
    MIPI_CHECK_NULL_PTR(pstLinkReg);

        phycil_mode.u32 = pstLinkReg->PHY_MODE_LINK.u32;
        if (PHY_CMV_GE900MV == cmv_mode)
        {
            phycil_mode.bits.phy_rg_mipi_mode = 0;
        }
        else if (PHY_CMV_LT900MV == cmv_mode)
        {
            phycil_mode.bits.phy_rg_mipi_mode = 1;
        }

        pstLinkReg->PHY_MODE_LINK.u32 = phycil_mode.u32;

        RTL_Verify_RegWrite("mipi_drv_set_phy_cmv", (unsigned int *)&(pstLinkReg->PHY_MODE_LINK.u32),
                "U_PHY_MODE_LINK", pstLinkReg->PHY_MODE_LINK.u32);
}

//******************************************************************************
//  Function    : set_phy_cfg_en
//  Description : only if it is called the configurations take effect
//                chn0_phycfg_en is a single pusle, will be cleared after write
//  Input       : devno, en
//  Return      : int: none
//******************************************************************************
void mipi_drv_set_phy_cfg_en(COMBO_DEV devno, HI_BOOL en)
{
    unsigned short phycfg_mode;
    work_mode_t work_mode;

    work_mode = mipi_drv_get_sys_work_mode(devno);

    if (WORK_MODE_MIPI == work_mode)
    {
        phycfg_mode = 0;
    }
    else if (WORK_MODE_CMOS == work_mode)
    {
        phycfg_mode = 2;
    }
    else
    {
        phycfg_mode = 1;
    }

    if (0 == devno)
    {
		U_CHN0_PHYCFG_MODE mipi0_phycfg_mode;
		U_CHN0_PHYCFG_EN mipi0_phycfg_en;
		mipi0_phycfg_mode.u32 = gpMipiAllReg->stSysRegs.CHN0_PHYCFG_MODE.u32;
		mipi0_phycfg_en.u32 = gpMipiAllReg->stSysRegs.CHN0_PHYCFG_EN.u32;
		mipi0_phycfg_mode.bits.chn0_phycfg_mode = phycfg_mode;
		mipi0_phycfg_en.bits.chn0_phycfg_en = en;
		gpMipiAllReg->stSysRegs.CHN0_PHYCFG_MODE.u32 = mipi0_phycfg_mode.u32;
		gpMipiAllReg->stSysRegs.CHN0_PHYCFG_EN.u32 = mipi0_phycfg_en.u32;
    }
    else if (1 == devno)
    {
		U_CHN1_PHYCFG_MODE mipi1_phycfg_mode;
		U_CHN1_PHYCFG_EN mipi1_phycfg_en;
		mipi1_phycfg_mode.u32 = gpMipiAllReg->stSysRegs.CHN1_PHYCFG_MODE.u32;
		mipi1_phycfg_en.u32 = gpMipiAllReg->stSysRegs.CHN1_PHYCFG_EN.u32;
		mipi1_phycfg_mode.bits.chn1_phycfg_mode = phycfg_mode;
		mipi1_phycfg_en.bits.chn1_phycfg_en = en;
		gpMipiAllReg->stSysRegs.CHN1_PHYCFG_MODE.u32 = mipi1_phycfg_mode.u32;
		gpMipiAllReg->stSysRegs.CHN1_PHYCFG_EN.u32 = mipi1_phycfg_en.u32;	
    }

    RTL_Verify_RegWrite("CHN0_PHYCFG_MODE", (unsigned int *)&(gpMipiAllReg->stSysRegs.CHN0_PHYCFG_MODE.u32),
            "CHN0_PHYCFG_MODE", gpMipiAllReg->stSysRegs.CHN0_PHYCFG_MODE.u32);
    RTL_Verify_RegWrite("CHN0_PHYCFG_EN", (unsigned int *)&(gpMipiAllReg->stSysRegs.CHN0_PHYCFG_EN.u32),
            "CHN0_PHYCFG_EN", gpMipiAllReg->stSysRegs.CHN0_PHYCFG_EN.u32);

	RTL_Verify_RegWrite("CHN1_PHYCFG_MODE", (unsigned int *)&(gpMipiAllReg->stSysRegs.CHN1_PHYCFG_MODE.u32),
	    "CHN1_PHYCFG_MODE", gpMipiAllReg->stSysRegs.CHN1_PHYCFG_MODE.u32);
	RTL_Verify_RegWrite("CHN1_PHYCFG_EN", (unsigned int *)&(gpMipiAllReg->stSysRegs.CHN1_PHYCFG_EN.u32),
	    "CHN1_PHYCFG_EN", gpMipiAllReg->stSysRegs.CHN1_PHYCFG_EN.u32);
}


void mipi_exception_handling(int devno)
{
    if (0 == devno)
    {
		U_CHN0_PHYCFG_MODE mipi0_phycfg_mode;
		U_CHN0_PHYCFG_EN mipi0_phycfg_en;
		mipi0_phycfg_mode.u32 = gpMipiAllReg->stSysRegs.CHN0_PHYCFG_MODE.u32;
		mipi0_phycfg_en.u32 = gpMipiAllReg->stSysRegs.CHN0_PHYCFG_EN.u32;
		mipi0_phycfg_mode.bits.chn0_phycfg_mode = 0x4;
		mipi0_phycfg_en.bits.chn0_phycfg_en = 0x1;
		gpMipiAllReg->stSysRegs.CHN0_PHYCFG_MODE.u32 = mipi0_phycfg_mode.u32;
		gpMipiAllReg->stSysRegs.CHN0_PHYCFG_EN.u32 = mipi0_phycfg_en.u32;
    }
    else if (1 == devno)
    {
		U_CHN1_PHYCFG_MODE mipi1_phycfg_mode;
		U_CHN1_PHYCFG_EN mipi1_phycfg_en;
		mipi1_phycfg_mode.u32 = gpMipiAllReg->stSysRegs.CHN1_PHYCFG_MODE.u32;
		mipi1_phycfg_en.u32 = gpMipiAllReg->stSysRegs.CHN1_PHYCFG_EN.u32;
		mipi1_phycfg_mode.bits.chn1_phycfg_mode = 0x4;
		mipi1_phycfg_en.bits.chn1_phycfg_en = 0x1;
		gpMipiAllReg->stSysRegs.CHN1_PHYCFG_MODE.u32 = mipi1_phycfg_mode.u32;
		gpMipiAllReg->stSysRegs.CHN1_PHYCFG_EN.u32 = mipi1_phycfg_en.u32;	
    }
}


//******************************************************************************
//  Function    : set_lvds_presync_clr_en
//  Description : LVDS auto correction, if LVDS_CTRL and ALIGN detect the error, 
//                then the LVDS_PRE_SYNC module will be set by force
//  Input       : devno, en
//  Return      : int: none
//******************************************************************************
void mipi_drv_set_lvds_presync_clr_en(COMBO_DEV devno, HI_BOOL en)
{
	if(0 == devno)
	{
		U_CHN0_CLR_EN	CHN0_CLR_EN;
		CHN0_CLR_EN.u32 =  gpMipiAllReg->stSysRegs.CHN0_CLR_EN.u32;

		CHN0_CLR_EN.bits.clr_en_lvds0 = en;
		CHN0_CLR_EN.bits.clr_en_align0 = en;		
		gpMipiAllReg->stSysRegs.CHN0_CLR_EN.u32 = CHN0_CLR_EN.u32;
	}
	else if(1 == devno)
	{
		U_CHN1_CLR_EN	CHN1_CLR_EN;
		CHN1_CLR_EN.u32 =  gpMipiAllReg->stSysRegs.CHN1_CLR_EN.u32;
		CHN1_CLR_EN.bits.clr_en_lvds1 = en;
		CHN1_CLR_EN.bits.clr_en_align1 = en;		
		gpMipiAllReg->stSysRegs.CHN1_CLR_EN.u32 = CHN1_CLR_EN.u32;
	}
	else
	{}   

    RTL_Verify_RegWrite("set_lvds_presync_clr_en", (unsigned int *)&(gpMipiAllReg->stSysRegs.CHN0_CLR_EN.u32),
            "CHN0_CLR_EN", gpMipiAllReg->stSysRegs.CHN0_CLR_EN.u32);
	RTL_Verify_RegWrite("set_lvds_presync_clr_en", (unsigned int *)&(gpMipiAllReg->stSysRegs.CHN1_CLR_EN.u32),
            "CHN1_CLR_EN", gpMipiAllReg->stSysRegs.CHN1_CLR_EN.u32);
}

//******************************************************************************
//  Function    : set_phy_ckgt_en
//  Description : enable the phy clock gate control 
//  Input       : link_id, en
//  Return      : int: none
//******************************************************************************
void mipi_drv_set_phy_ckgt_en(COMBO_DEV devno, COMBO_LINK link_id, HI_BOOL en)
{

    if(0 == devno)
    {
        U_CHN0_CIL_CTRL		CHN0_CIL_CTRL;
        CHN0_CIL_CTRL.u32 = gpMipiAllReg->stSysRegs.CHN0_CIL_CTRL.u32;

        if (0 == link_id)
        {
            CHN0_CIL_CTRL.bits.phycil0_cken = en;
        }
        else if (1 == link_id)
        {
            CHN0_CIL_CTRL.bits.phycil1_cken = en;
        }
        else if (2 == link_id)
        {
            CHN0_CIL_CTRL.bits.phycil2_cken = en;
        }
        gpMipiAllReg->stSysRegs.CHN0_CIL_CTRL.u32 = CHN0_CIL_CTRL.u32;
    }
	else if(1 == devno)
    {
        U_CHN1_CIL_CTRL		CHN1_CIL_CTRL;
        CHN1_CIL_CTRL.u32 = gpMipiAllReg->stSysRegs.CHN1_CIL_CTRL.u32;

        CHN1_CIL_CTRL.bits.chn1_phycil2_cken = en;		
        gpMipiAllReg->stSysRegs.CHN1_CIL_CTRL.u32 = CHN1_CIL_CTRL.u32;
    }
	else
	{
	}
    RTL_Verify_RegWrite("set_phy_ckgt_en", (unsigned int *)&(gpMipiAllReg->stSysRegs.CHN0_CIL_CTRL.u32),
            "MIPI_CIL_CTRL", gpMipiAllReg->stSysRegs.CHN0_CIL_CTRL.u32);
	 RTL_Verify_RegWrite("set_phy_ckgt_en", (unsigned int *)&(gpMipiAllReg->stSysRegs.CHN1_CIL_CTRL.u32),
            "MIPI_CIL_CTRL", gpMipiAllReg->stSysRegs.CHN1_CIL_CTRL.u32);
}

//******************************************************************************
//  Function    : get_mipi_imgsize_statis
//  Description : get the VC's image size statistics 
//  Input       : devno, vc
//  Output      : p_size
//  Return      : int: none
//******************************************************************************
void mipi_drv_get_mipi_imgsize_statis(COMBO_DEV devno, short vc, img_size_t *p_size)
{
    MIPI_CTRL_REGS_S* pstCtrlReg;
    U_MIPI_IMGSIZE_STATIS mipi_vc_imgsize;

    pstCtrlReg = get_mipi_ctrl_regs(devno);
    MIPI_CHECK_NULL_PTR(pstCtrlReg);

    if (vc < WDR_VC_NUM)
    {
        mipi_vc_imgsize.u32 = pstCtrlReg->MIPI_IMGSIZE_STATIS[vc].u32;
        p_size->width  = mipi_vc_imgsize.bits.imgwidth_statis_vc;
        p_size->height = mipi_vc_imgsize.bits.imgheight_statis_vc;
    }
}

//******************************************************************************
//  Function    : get_lvds_imgsize_statis
//  Description : get the lvds image statistics
//  Input       : devno, ef.  ef:0 LEF, 1 SEF1, 2 SEF2, 3 SEF3
//  Output      : p_size
//  Return      : int: none
//******************************************************************************
void mipi_drv_get_lvds_imgsize_statis(COMBO_DEV devno, short ef, img_size_t *p_size)
{
    LVDS_CTRL_REGS_S* pstCtrlReg;
    U_LVDS_IMGSIZE_STATIS lvds_imgsize;

    pstCtrlReg = get_lvds_ctrl_regs(devno);
    MIPI_CHECK_NULL_PTR(pstCtrlReg);

    if (ef < WDR_VC_NUM)
    {
        lvds_imgsize.u32 = pstCtrlReg->LVDS_IMGSIZE_STATIS[ef].u32;

        p_size->width  = lvds_imgsize.bits.lvds_imgwidth;
        p_size->height = lvds_imgsize.bits.lvds_imgheight;
    }
}

//******************************************************************************
//  Function    : get_lvds_lane_imgsize_statis
//  Description : get the lvds lane_image statistics
//  Input       : devno, lane
//  Output      : p_size
//  Return      : none
//******************************************************************************
void mipi_drv_get_lvds_lane_imgsize_statis(COMBO_DEV devno, short lane, img_size_t *p_size)
{
    LVDS_CTRL_REGS_S* pstCtrlReg;
    U_LVDS_LANE_IMGSIZE_STATIS lvds_lane_imgsize;

    pstCtrlReg = get_lvds_ctrl_regs(devno);
    MIPI_CHECK_NULL_PTR(pstCtrlReg);

    if (lane < COMBO_MAX_LANE_NUM)
    {
        lvds_lane_imgsize.u32 = pstCtrlReg->LVDS_LANE_IMGSIZE_STATIS[lane].u32;

        p_size->width  = lvds_lane_imgsize.bits.lane_imgwidth;
        p_size->height = lvds_lane_imgsize.bits.lane_imgheight;
        if (0 < p_size->width)
        {
            p_size->width += 1;
        }
    }
}

raw_data_type_e mipi_drv_get_lvds_raw_data_type(COMBO_DEV devno)
{
    LVDS_CTRL_REGS_S* pstCtrlReg;
    raw_data_type_e type = RAW_DATA_BUTT;

    pstCtrlReg = get_lvds_ctrl_regs(devno);
    if (NULL == pstCtrlReg)
    {
        HI_ERR("get lvds_ctrl reg failed\n");
        return type;
    }

    switch (pstCtrlReg->LVDS_CTRL.bits.lvds_raw_type)
    {
    case 1:
        type = RAW_DATA_8BIT;
        break;
    case 2:
        type = RAW_DATA_10BIT;
        break;
    case 3:
        type = RAW_DATA_12BIT;
        break;
    case 4:
        type = RAW_DATA_14BIT;
        break;
    case 5:
        type = RAW_DATA_16BIT;
        break;
    }

    return type;
}

unsigned int mipi_drv_get_phy_data(COMBO_LINK link_id)
{
    MIPI_LINK_REGS_S* pstLinkReg;
    unsigned int data = 0;

    pstLinkReg = get_mipi_link_regs(link_id);
    if (NULL != pstLinkReg)
    {
        data = pstLinkReg->PHY_DATA_LINK.u32;
    }
    else
    {
        HI_ERR("get mipi_link reg failed\n");
    }

    return data;
}

/*
 *  data after phy sync detect and data is aligned
 */
unsigned int mipi_drv_get_phy_aligned_data(COMBO_LINK link_id)
{
    MIPI_LINK_REGS_S* pstLinkReg;
    unsigned int data = 0;

    pstLinkReg = get_mipi_link_regs(link_id);
    if (NULL != pstLinkReg)
    {
        data = pstLinkReg->PHY_DATA_LVDS_LINK.u32;
    }
    else
    {
        HI_ERR("get mipi_link reg failed\n");
    }

    return data;
}

/*
 * get which lane output is valid after sync detect
 */
unsigned int mipi_drv_get_phy_valid_lane(COMBO_LINK link_id, work_mode_t work_mode)
{
    MIPI_LINK_REGS_S* pstLinkReg;
    unsigned int valid_lane = 0;

    pstLinkReg = get_mipi_link_regs(link_id);
    if (NULL != pstLinkReg)
    {
        valid_lane = pstLinkReg->PHY_OUT_VALID_LINK.u32;
        if (WORK_MODE_LVDS == work_mode)
        {
            valid_lane &= 0xf;
        }
        else
        {
            valid_lane = (valid_lane & 0xf0) >> 4;
        }
    }
    else
    {
        HI_ERR("get mipi_link reg failed\n");
    }

    return valid_lane;
}

