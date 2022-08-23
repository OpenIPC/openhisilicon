/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2016-2019. All rights reserved.
 * Description: mipi_rx_hal.c
 * Author:
 * Create: 2016-10-07
 */

#include "osal.h"
#include "type.h"
#include "hi_mipi.h"
#include "mipi_rx_hal.h"
#include "mipi_rx_reg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

static void *mipi_rx_core_reset_addr;

typedef unsigned int mipi_rx_phy_t;

typedef struct {
	unsigned int phy_rg_en_2121;
	unsigned int phy_rg_clk0_en;
	unsigned int phy_rg_clk1_en;
	unsigned int phy_rg_lp0_mode_en;
	unsigned int phy_rg_lp1_mode_en;
} phy_mode_link_t;

typedef enum {
	MIPI_ESC_D0 = 0x1 << 0,
	MIPI_ESC_D1 = 0x1 << 1,
	MIPI_ESC_D2 = 0x1 << 2,
	MIPI_ESC_D3 = 0x1 << 3,
	MIPI_ESC_CLK0 = 0x1 << 4,
	MIPI_ESC_CLK1 = 0x1 << 5,

	MIPI_TIMEOUT_D0 = 0x1 << 8,
	MIPI_TIMEOUT_D1 = 0x1 << 9,
	MIPI_TIMEOUT_D2 = 0x1 << 10,
	MIPI_TIMEOUT_D3 = 0x1 << 11,
	MIPI_TIMEOUT_CLK0 = 0x1 << 12,
	MIPI_TIMEOUT_CLK1 = 0x1 << 13,
} phy_err_int_state;

typedef enum {
	MIPI_VC0_PKT_DATA_CRC = 0x1 << 0, /* VC0'S data CRC error */
	MIPI_VC1_PKT_DATA_CRC = 0x1 << 1,
	MIPI_VC2_PKT_DATA_CRC = 0x1 << 2,
	MIPI_VC3_PKT_DATA_CRC = 0x1 << 3,

	MIPI_PKT_HEADER_ERR =
		0x1
		<< 16, /* Header has two error at least ,and ECC error correction is invalid */
} mipi_pkt_int_state;

typedef enum {
	MIPI_VC0_PKT_INVALID_DT = 0x1 << 0, /* do not support VC0'S data type */
	MIPI_VC1_PKT_INVALID_DT = 0x1 << 1, /* do not support VC1'S data type */
	MIPI_VC2_PKT_INVALID_DT = 0x1 << 2, /* do not support VC2'S data type */
	MIPI_VC3_PKT_INVALID_DT = 0x1 << 3, /* do not support VC3'S data type */

	MIPI_VC0_PKT_HEADER_ECC =
		0x1
		<< 16, /* VC0'S header has errors,and ECC error correction is ok */
	MIPI_VC1_PKT_HEADER_ECC = 0x1 << 17,
	MIPI_VC2_PKT_HEADER_ECC = 0x1 << 18,
	MIPI_VC3_PKT_HEADER_ECC = 0x1 << 19,
} mipi_pkt2_int_state;

typedef enum {
	MIPI_VC0_NO_MATCH =
		0x1 << 0, /* VC0,frame's start and frame's end do not match  */
	MIPI_VC1_NO_MATCH =
		0x1 << 1, /* VC1,frame's start and frame's end do not match  */
	MIPI_VC2_NO_MATCH =
		0x1 << 2, /* VC2,frame's start and frame's end do not match  */
	MIPI_VC3_NO_MATCH =
		0x1 << 3, /* VC3,frame's start and frame's end do not match  */

	MIPI_VC0_ORDER_ERR = 0x1 << 8, /* VC0'S frame order error */
	MIPI_VC1_ORDER_ERR = 0x1 << 9, /* VC1'S frame order error */
	MIPI_VC2_ORDER_ERR = 0x1 << 10, /* VC2'S frame order error */
	MIPI_VC3_ORDER_ERR = 0x1 << 11, /* VC3'S frame order error */

	MIPI_VC0_FRAME_CRC =
		0x1
		<< 16, /* in the last frame, VC0'S data has a CRC ERROR at least  */
	MIPI_VC1_FRAME_CRC =
		0x1
		<< 17, /* in the last frame, VC1'S data has a CRC ERROR at least  */
	MIPI_VC2_FRAME_CRC =
		0x1
		<< 18, /* in the last frame, VC2'S data has a CRC ERROR at least  */
	MIPI_VC3_FRAME_CRC =
		0x1
		<< 19, /* in the last frame, VC3'S data has a CRC ERROR at least  */
} mipi_frame_int_state;

typedef enum {
	CMD_FIFO_WRITE_ERR = 0x1 << 0, /* MIPI_CTRL write command FIFO error */
	DATA_FIFO_WRITE_ERR = 0x1 << 1,
	CMD_FIFO_READ_ERR = 0x1 << 16,
	DATA_FIFO_READ_ERR = 0x1 << 17,
} mipi_ctrl_int_state;

typedef enum {
	LINK0_WRITE_ERR = 0x1 << 16,
	LINK0_READ_ERR = 0x1 << 20,
	LVDS_STAT_ERR = 0x1 << 24,
	LVDS_POP_ERR = 0x1 << 25,
	CMD_WR_ERR = 0x1 << 26,
	CMD_RD_ERR = 0x1 << 27,
} lvds_int_state;

typedef enum {
	ALIGN_FIFO_FULL_ERR = 0x1 << 0,
	ALIGN_LANE0_ERR = 0x1 << 1,
	ALIGN_LANE1_ERR = 0x1 << 2,
	ALIGN_LANE2_ERR = 0x1 << 3,
	ALIGN_LANE3_ERR = 0x1 << 4,
} align_int_state;
/****************************************************************************
 * macro definition                                                         *
 ****************************************************************************/
#define MIPI_RX_REGS_ADDR 0x11240000
#define MIPI_RX_REGS_SIZE 0x10000

#define SNS_CRG_ADDR 0x120100F0
#define MIPI_RX_CRG_ADDR 0x120100F8

#define MIPI_RX_WORK_MODE_ADDR 0x12028018 /* offset 8018 */

#define MIPI_RX_IRQ 77

unsigned int regMapFlag = 0;

#define IS_VALID_ID(id) ((id) != -1)

#define SKEW_LINK (0x0)
#define MIPI_FSMO_VALUE (0x000f1d0c)

/*
 * global variables definition
*/
mipi_rx_regs_type_t *mipi_rx_regs_va = NULL;

unsigned int mipi_rx_irq_num = MIPI_RX_IRQ;

const static phy_mode_link_t phy_mode[][MIPI_RX_MAX_PHY_NUM] = {
	{ { 0, 1, 1, 1, 1 } },
};
phy_err_int_cnt_t phy_err_int_cnt[MIPI_RX_MAX_PHY_NUM];
mipi_err_int_cnt_t mipi_err_int_cnt[MIPI_RX_MAX_DEV_NUM];
lvds_err_int_cnt_t lvds_err_int_cnt[MIPI_RX_MAX_DEV_NUM];
align_err_int_cnt_t align_err_int_cnt[MIPI_RX_MAX_DEV_NUM];

/****************************************************************************
 * function definition                                                      *
 ****************************************************************************/

static void set_bit(unsigned long value, unsigned long offset, const void *addr)
{
	unsigned long t, mask;

	mask = 1 << offset;
	t = osal_readl((const volatile void *)addr);
	t &= ~mask;
	t |= (value << offset) & mask;
	osal_writel(t, (volatile void *)addr);
}

static void write_reg32(const void *addr, unsigned int value, unsigned int mask)
{
	unsigned int t;

	t = osal_readl((void *)addr);
	t &= ~mask;
	t |= value & mask;

	osal_writel(t, (void *)addr);
}

static mipi_rx_phy_cfg_t *get_mipi_rx_phy_regs(mipi_rx_phy_t phy_id)
{
	return &mipi_rx_regs_va->mipi_rx_phy_cfg[phy_id];
}

static mipi_rx_sys_regs_t *get_mipi_rx_sys_regs(void)
{
	return &mipi_rx_regs_va->mipi_rx_sys_regs;
}

static mipi_ctrl_regs_t *get_mipi_ctrl_regs(combo_dev_t devno)
{
	return &mipi_rx_regs_va->mipi_rx_ctrl_regs[devno].mipi_ctrl_regs;
}

static lvds_ctrl_regs_t *get_lvds_ctrl_regs(combo_dev_t devno)
{
	return &mipi_rx_regs_va->mipi_rx_ctrl_regs[devno].lvds_ctrl_regs;
}

void mipi_rx_set_cil_int_mask(mipi_rx_phy_t phy_id, unsigned int mask)
{
	U_MIPI_INT_MSK mipi_int_msk;
	volatile mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;
	volatile mipi_rx_sys_regs_t *mipi_rx_sys_regs = get_mipi_rx_sys_regs();

	mipi_int_msk.u32 = mipi_rx_sys_regs->MIPI_INT_MSK.u32;

	if (phy_id == 0) {
		mipi_int_msk.bits.int_phycil0_mask = 0x0;
	}

	mipi_rx_sys_regs->MIPI_INT_MSK.u32 = mipi_int_msk.u32;

	mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
	mipi_rx_phy_cfg->MIPI_CIL_INT_MSK_LINK.u32 = mask;
}

static void mipi_rx_set_phy_skew_link(mipi_rx_phy_t phy_id, unsigned int value)
{
	volatile U_PHY_SKEW_LINK phy_skew_link;
	volatile mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

	mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
	phy_skew_link.u32 = value;
	mipi_rx_phy_cfg->PHY_SKEW_LINK.u32 = phy_skew_link.u32;
}

void mipi_rx_set_phy_fsmo_link(mipi_rx_phy_t phy_id, unsigned int value)
{
	volatile mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

	mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
	mipi_rx_phy_cfg->CIL_FSM0_LINK.u32 = value;
}

void mipi_rx_set_phy_rg_2121_en(mipi_rx_phy_t phy_id, int enable)
{
	volatile U_PHY_MODE_LINK phy_mode_link;
	mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

	mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
	phy_mode_link.u32 = mipi_rx_phy_cfg->PHY_MODE_LINK.u32;
	phy_mode_link.bits.phy0_rg_en_2l2l = enable;
	mipi_rx_phy_cfg->PHY_MODE_LINK.u32 = phy_mode_link.u32;
}

void mipi_rx_set_phy_rg_clk0_en(unsigned int phy_id, int enable)
{
	volatile U_PHY_MODE_LINK phy_mode_link;
	mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

	mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
	phy_mode_link.u32 = mipi_rx_phy_cfg->PHY_MODE_LINK.u32;
	phy_mode_link.bits.phy0_rg_faclk0_en = enable;
	mipi_rx_phy_cfg->PHY_MODE_LINK.u32 = phy_mode_link.u32;
}

void mipi_rx_set_phy_rg_clk1_en(unsigned int phy_id, int enable)
{
	volatile U_PHY_MODE_LINK phy_mode_link;
	mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

	mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
	phy_mode_link.u32 = mipi_rx_phy_cfg->PHY_MODE_LINK.u32;
	phy_mode_link.bits.phy0_rg_faclk1_en = enable;
	mipi_rx_phy_cfg->PHY_MODE_LINK.u32 = phy_mode_link.u32;
}

void mipi_rx_set_phy_rg_lp0_mode_en(unsigned int phy_id, int enable)
{
	volatile U_PHY_MODE_LINK phy_mode_link;
	mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

	mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
	phy_mode_link.u32 = mipi_rx_phy_cfg->PHY_MODE_LINK.u32;
	phy_mode_link.bits.phy0_rg_en_lp0 = enable;
	mipi_rx_phy_cfg->PHY_MODE_LINK.u32 = phy_mode_link.u32;
}

void mipi_rx_set_phy_rg_lp1_mode_en(unsigned int phy_id, int enable)
{
	volatile U_PHY_MODE_LINK phy_mode_link;
	mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

	mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
	phy_mode_link.u32 = mipi_rx_phy_cfg->PHY_MODE_LINK.u32;
	phy_mode_link.bits.phy0_rg_en_lp1 = enable;
	mipi_rx_phy_cfg->PHY_MODE_LINK.u32 = phy_mode_link.u32;
}

void mipi_rx_drv_set_work_mode(combo_dev_t devno, input_mode_t input_mode)
{
	void *mipi_rx_work_mode_addr;

	mipi_rx_work_mode_addr =
		osal_ioremap(MIPI_RX_WORK_MODE_ADDR, (unsigned long)0x4);
	if (mipi_rx_work_mode_addr == NULL) {
		HI_ERR("mipi_rx work mode reg ioremap failed!\n");
		return;
	}

	if (input_mode == INPUT_MODE_MIPI) {
		write_reg32(mipi_rx_work_mode_addr, 0x0 << (2 * devno),
			    0x1 << (2 * devno));

	} else if ((input_mode == INPUT_MODE_SUBLVDS) ||
		   (input_mode == INPUT_MODE_LVDS) ||
		   (input_mode == INPUT_MODE_HISPI)) {
		write_reg32(mipi_rx_work_mode_addr, 0x1 << (2 * devno),
			    0x1 << (2 * devno));
	} else {
	}

	osal_iounmap((void *)mipi_rx_work_mode_addr);
}

void mipi_rx_drv_set_mipi_image_rect(combo_dev_t devno, img_rect_t *p_img_rect)
{
	U_MIPI_CROP_START_CHN0 crop_start_chn0;
	U_MIPI_CROP_START_CHN1 crop_start_chn1;
	U_MIPI_IMGSIZE mipi_imgsize;

	volatile mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

	crop_start_chn0.u32 = mipi_ctrl_regs->MIPI_CROP_START_CHN0.u32;
	crop_start_chn1.u32 = mipi_ctrl_regs->MIPI_CROP_START_CHN1.u32;

	mipi_imgsize.u32 = mipi_ctrl_regs->MIPI_IMGSIZE.u32;

	mipi_imgsize.bits.mipi_imgwidth = p_img_rect->width - 1;
	mipi_imgsize.bits.mipi_imgheight = p_img_rect->height - 1;

	crop_start_chn0.bits.mipi_start_x_chn0 = p_img_rect->x;
	crop_start_chn0.bits.mipi_start_y_chn0 = p_img_rect->y;

	crop_start_chn1.bits.mipi_start_x_chn1 = p_img_rect->x;
	crop_start_chn1.bits.mipi_start_y_chn1 = p_img_rect->y;

	mipi_ctrl_regs->MIPI_CROP_START_CHN0.u32 = crop_start_chn0.u32;
	mipi_ctrl_regs->MIPI_CROP_START_CHN1.u32 = crop_start_chn1.u32;

	mipi_ctrl_regs->MIPI_IMGSIZE.u32 = mipi_imgsize.u32;
}

void mipi_rx_drv_set_mipi_crop_en(combo_dev_t devno, int enable)
{
	U_MIPI_CTRL_MODE_PIXEL mipi_ctrl_mode_pixel;
	volatile mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

	mipi_ctrl_mode_pixel.u32 = mipi_ctrl_regs->MIPI_CTRL_MODE_PIXEL.u32;
	mipi_ctrl_mode_pixel.bits.crop_en = enable;
	mipi_ctrl_regs->MIPI_CTRL_MODE_PIXEL.u32 = mipi_ctrl_mode_pixel.u32;
}

void mipi_rx_drv_set_di_dt(combo_dev_t devno, data_type_t input_data_type)
{
	U_MIPI_DI_1 mipi_di_1;
	U_MIPI_DI_2 mipi_di_2;
	unsigned int temp_data_type = 0;
	volatile mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

	if (input_data_type == DATA_TYPE_RAW_8BIT) {
		temp_data_type = 0x2a;
	}
	if (input_data_type == DATA_TYPE_RAW_10BIT) {
		temp_data_type = 0x2b;
	} else if (input_data_type == DATA_TYPE_RAW_12BIT) {
		temp_data_type = 0x2c;
	} else if (input_data_type == DATA_TYPE_RAW_14BIT) {
		temp_data_type = 0x2d;
	} else if (input_data_type == DATA_TYPE_YUV420_8BIT_NORMAL) {
		temp_data_type = 0x18;
	} else if (input_data_type == DATA_TYPE_YUV420_8BIT_LEGACY) {
		temp_data_type = 0x1a;
	} else if (input_data_type == DATA_TYPE_YUV422_8BIT) {
		temp_data_type = 0x1e;
	}

	mipi_di_1.u32 = mipi_ctrl_regs->MIPI_DI_1.u32;
	mipi_di_1.bits.di0_dt = temp_data_type;
	mipi_di_1.bits.di1_dt = temp_data_type;
	mipi_di_1.bits.di2_dt = temp_data_type;
	mipi_di_1.bits.di3_dt = temp_data_type;
	mipi_ctrl_regs->MIPI_DI_1.u32 = mipi_di_1.u32;

	mipi_di_2.u32 = mipi_ctrl_regs->MIPI_DI_2.u32;
	mipi_di_2.bits.di4_dt = temp_data_type;
	mipi_di_2.bits.di5_dt = temp_data_type;
	mipi_di_2.bits.di6_dt = temp_data_type;
	mipi_di_2.bits.di7_dt = temp_data_type;
	mipi_ctrl_regs->MIPI_DI_2.u32 = mipi_di_2.u32;
}

void mipi_rx_drv_set_mipi_yuv_dt(combo_dev_t devno, data_type_t input_data_type)
{
	U_MIPI_USERDEF_DT user_def_dt;
	U_MIPI_USER_DEF user_def;
	U_MIPI_CTRL_MODE_HS mipi_ctrl_mode_hs;
	U_MIPI_CTRL_MODE_PIXEL mipi_ctrl_mode_pixel;
	volatile mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);
	unsigned char bit_width;
	unsigned char user_def_en;
	unsigned int temp_data_type;

	if (input_data_type == DATA_TYPE_YUV420_8BIT_NORMAL) {
		bit_width = 0;
		temp_data_type = 0x18;
		user_def_en = 1;
	} else if (input_data_type == DATA_TYPE_YUV420_8BIT_LEGACY) {
		bit_width = 0;
		temp_data_type = 0x1a;
		user_def_en = 1;
	} else if (input_data_type == DATA_TYPE_YUV422_8BIT) {
		bit_width = 0;
		temp_data_type = 0x1e;
		user_def_en = 1;
	} else {
		mipi_ctrl_mode_hs.u32 = mipi_ctrl_regs->MIPI_CTRL_MODE_HS.u32;
		mipi_ctrl_mode_hs.bits.user_def_en = 0;
		mipi_ctrl_regs->MIPI_CTRL_MODE_HS.u32 = mipi_ctrl_mode_hs.u32;
		return;
	}

	user_def_dt.u32 = mipi_ctrl_regs->MIPI_USERDEF_DT.u32;
	user_def.u32 = mipi_ctrl_regs->MIPI_USER_DEF.u32;
	mipi_ctrl_mode_hs.u32 = mipi_ctrl_regs->MIPI_CTRL_MODE_HS.u32;
	mipi_ctrl_mode_pixel.u32 = mipi_ctrl_regs->MIPI_CTRL_MODE_PIXEL.u32;

	user_def_dt.bits.user_def0_dt = bit_width;
	user_def_dt.bits.user_def1_dt = bit_width;
	user_def_dt.bits.user_def2_dt = bit_width;
	user_def_dt.bits.user_def3_dt = bit_width;

	user_def.bits.user_def0 = temp_data_type;
	user_def.bits.user_def1 = temp_data_type;
	user_def.bits.user_def2 = temp_data_type;
	user_def.bits.user_def3 = temp_data_type;

	mipi_ctrl_mode_hs.bits.user_def_en = user_def_en;

	if (input_data_type == DATA_TYPE_YUV420_8BIT_NORMAL) {
		mipi_ctrl_mode_pixel.bits.mipi_yuv420_even_detect = 1;
		mipi_ctrl_mode_pixel.bits.mipi_yuv_420_nolegacy_en = 1;
		mipi_ctrl_mode_pixel.bits.mipi_yuv_420_legacy_en = 0;
		mipi_ctrl_mode_pixel.bits.mipi_yuv_422_en = 0;
	} else if (input_data_type == DATA_TYPE_YUV420_8BIT_LEGACY) {
		mipi_ctrl_mode_pixel.bits.mipi_yuv_420_nolegacy_en = 0;
		mipi_ctrl_mode_pixel.bits.mipi_yuv_420_legacy_en = 1;
		mipi_ctrl_mode_pixel.bits.mipi_yuv_422_en = 0;
	} else if (input_data_type == DATA_TYPE_YUV422_8BIT) {
		mipi_ctrl_mode_pixel.bits.mipi_yuv_420_nolegacy_en = 0;
		mipi_ctrl_mode_pixel.bits.mipi_yuv_420_legacy_en = 0;
		mipi_ctrl_mode_pixel.bits.mipi_yuv_422_en = 1;
	}

	mipi_ctrl_regs->MIPI_USERDEF_DT.u32 = user_def_dt.u32;
	mipi_ctrl_regs->MIPI_USER_DEF.u32 = user_def.u32;
	mipi_ctrl_regs->MIPI_CTRL_MODE_HS.u32 = mipi_ctrl_mode_hs.u32;
	mipi_ctrl_regs->MIPI_CTRL_MODE_PIXEL.u32 = mipi_ctrl_mode_pixel.u32;
}

void mipi_rx_drv_set_mipi_user_dt(combo_dev_t devno,
				  data_type_t input_data_type,
				  const short data_type[WDR_VC_NUM])
{
	U_MIPI_USERDEF_DT user_def_dt;
	U_MIPI_USER_DEF user_def;
	volatile mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

	user_def_dt.u32 = mipi_ctrl_regs->MIPI_USERDEF_DT.u32;
	user_def.u32 = mipi_ctrl_regs->MIPI_USER_DEF.u32;

	user_def_dt.bits.user_def0_dt = input_data_type;
	user_def_dt.bits.user_def1_dt = input_data_type;

	user_def.bits.user_def0 = data_type[0];
	user_def.bits.user_def1 = data_type[1];

	mipi_ctrl_regs->MIPI_USERDEF_DT.u32 = user_def_dt.u32;
	mipi_ctrl_regs->MIPI_USER_DEF.u32 = user_def.u32;
}

void mipi_rx_drv_set_mipi_dol_id(combo_dev_t devno, data_type_t input_data_type,
				 short dol_id[])
{
	U_MIPI_DOL_ID_CODE0 dol_id0;
	U_MIPI_DOL_ID_CODE1 dol_id1;
	U_MIPI_DOL_ID_CODE2 dol_id2;
	short lef, sef1, sef2;
	short nxt_lef, nxt_sef1, nxt_sef2;
	volatile mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

	dol_id0.u32 = mipi_ctrl_regs->MIPI_DOL_ID_CODE0.u32;
	dol_id1.u32 = mipi_ctrl_regs->MIPI_DOL_ID_CODE1.u32;
	dol_id2.u32 = mipi_ctrl_regs->MIPI_DOL_ID_CODE2.u32;

	lef = 0x241;
	sef1 = 0x242;
	sef2 = 0x244;

	nxt_lef = lef + (1 << 4);
	nxt_sef1 = sef1 + (1 << 4);
	nxt_sef2 = sef2 + (1 << 4);

	dol_id0.bits.id_code_reg0 = lef;
	dol_id0.bits.id_code_reg1 = sef1;
	dol_id1.bits.id_code_reg2 = sef2;

	dol_id1.bits.id_code_reg3 = nxt_lef;
	dol_id2.bits.id_code_reg4 = nxt_sef1;
	dol_id2.bits.id_code_reg5 = nxt_sef2;

	mipi_ctrl_regs->MIPI_DOL_ID_CODE0.u32 = dol_id0.u32;
	mipi_ctrl_regs->MIPI_DOL_ID_CODE1.u32 = dol_id1.u32;
	mipi_ctrl_regs->MIPI_DOL_ID_CODE2.u32 = dol_id2.u32;
}

void mipi_rx_drv_set_mipi_wdr_mode(combo_dev_t devno, mipi_wdr_mode_t wdr_mode)
{
	U_MIPI_CTRL_MODE_HS mode_hs;
	U_MIPI_CTRL_MODE_PIXEL mode_pixel;
	volatile mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

	mode_hs.u32 = mipi_ctrl_regs->MIPI_CTRL_MODE_HS.u32;
	mode_pixel.u32 = mipi_ctrl_regs->MIPI_CTRL_MODE_PIXEL.u32;

	if (wdr_mode == HI_MIPI_WDR_MODE_NONE) {
		mode_pixel.bits.mipi_dol_mode = 0;
	}
	if (wdr_mode == HI_MIPI_WDR_MODE_VC) {
		mode_pixel.bits.mipi_dol_mode = 0;
	} else if (wdr_mode == HI_MIPI_WDR_MODE_DT) {
		mode_hs.bits.user_def_en = 1;
	} else if (wdr_mode == HI_MIPI_WDR_MODE_DOL) {
		mode_pixel.bits.mipi_dol_mode = 1;
	}

	mipi_ctrl_regs->MIPI_CTRL_MODE_HS.u32 = mode_hs.u32;
	mipi_ctrl_regs->MIPI_CTRL_MODE_PIXEL.u32 = mode_pixel.u32;
}

unsigned int mipi_rx_drv_get_phy_data(int phy_id, int lane_id)
{
	volatile U_PHY_DATA_LINK phy_data_link;
	mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;
	unsigned int lane_data = 0x0;

	mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
	phy_data_link.u32 = mipi_rx_phy_cfg->PHY_DATA_LINK.u32;

	if (lane_id == 0) {
		lane_data = phy_data_link.bits.phy_data0_mipi;
	} else if (lane_id == 1) {
		lane_data = phy_data_link.bits.phy_data1_mipi;
	} else if (lane_id == 2) {
		lane_data = phy_data_link.bits.phy_data2_mipi;
	} else if (lane_id == 3) {
		lane_data = phy_data_link.bits.phy_data3_mipi;
	}

	return lane_data;
}

unsigned int mipi_rx_drv_get_phy_mipi_link_data(int phy_id, int lane_id)
{
	volatile U_PHY_DATA_MIPI_LINK phy_data_mipi_link;
	mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;
	unsigned int lane_data = 0x0;

	mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
	phy_data_mipi_link.u32 = mipi_rx_phy_cfg->PHY_DATA_MIPI_LINK.u32;

	if (lane_id == 0) {
		lane_data = phy_data_mipi_link.bits.phy_data0_mipi_hs;
	} else if (lane_id == 1) {
		lane_data = phy_data_mipi_link.bits.phy_data1_mipi_hs;
	} else if (lane_id == 2) {
		lane_data = phy_data_mipi_link.bits.phy_data2_mipi_hs;
	} else if (lane_id == 3) {
		lane_data = phy_data_mipi_link.bits.phy_data3_mipi_hs;
	}

	return lane_data;
}

unsigned int mipi_rx_drv_get_phy_lvds_link_data(int phy_id, int lane_id)
{
	volatile U_PHY_DATA_LVDS_LINK phy_data_lvds_link;
	mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;
	unsigned int lane_data = 0x0;

	mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
	phy_data_lvds_link.u32 = mipi_rx_phy_cfg->PHY_DATA_LVDS_LINK.u32;

	if (lane_id == 0) {
		lane_data = phy_data_lvds_link.bits.phy_data0_lvds_hs;
	} else if (lane_id == 1) {
		lane_data = phy_data_lvds_link.bits.phy_data1_lvds_hs;
	} else if (lane_id == 2) {
		lane_data = phy_data_lvds_link.bits.phy_data2_lvds_hs;
	} else if (lane_id == 3) {
		lane_data = phy_data_lvds_link.bits.phy_data3_lvds_hs;
	}

	return lane_data;
}
void mipi_rx_drv_set_data_rate(combo_dev_t devno, mipi_data_rate_t data_rate)
{
	U_MIPI_CTRL_MODE_PIXEL mipi_ctrl_mode_pixel;
	unsigned int mipi_double_pix_en = 0;
	volatile mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

	if (data_rate == MIPI_DATA_RATE_X1) {
		mipi_double_pix_en = 0;
	} else if (data_rate == MIPI_DATA_RATE_X2) {
		mipi_double_pix_en = 1;
	} else {
		HI_ERR("unsupported  data_rate:%d  devno %d\n", data_rate,
		       devno);
		return;
	}

	mipi_ctrl_mode_pixel.u32 = mipi_ctrl_regs->MIPI_CTRL_MODE_PIXEL.u32;
	mipi_ctrl_mode_pixel.bits.mipi_double_pix_en = mipi_double_pix_en;
	mipi_ctrl_mode_pixel.bits.sync_clear_en = 0x1;
	mipi_ctrl_regs->MIPI_CTRL_MODE_PIXEL.u32 = mipi_ctrl_mode_pixel.u32;
}

void mipi_rx_set_lane_id(combo_dev_t devno, int lane_idx, short lane_id)
{
	U_LANE_ID0_CHN LANE_ID0_CHN0;

	volatile lvds_ctrl_regs_t *lvds_ctrl_regs = get_lvds_ctrl_regs(devno);

	LANE_ID0_CHN0.u32 = lvds_ctrl_regs->LANE_ID0_CHN0.u32;

	switch (lane_id) {
	case 0:
		LANE_ID0_CHN0.bits.lane0_id = lane_idx;
		break;

	case 1:
		LANE_ID0_CHN0.bits.lane1_id = lane_idx;
		break;

	case 2:
		LANE_ID0_CHN0.bits.lane2_id = lane_idx;
		break;

	case 3:
		LANE_ID0_CHN0.bits.lane3_id = lane_idx;
		break;

	default:
		break;
	}

	lvds_ctrl_regs->LANE_ID0_CHN0.u32 = LANE_ID0_CHN0.u32;
}

void mipi_rx_drv_set_link_lane_id(combo_dev_t devno, input_mode_t input_mode,
				  const short *p_lane_id)
{
	int i;
	int lane_num;

	if (input_mode == INPUT_MODE_MIPI) {
		lane_num = MIPI_LANE_NUM;
	} else {
		lane_num = LVDS_LANE_NUM;
	}

	for (i = 0; i < lane_num; i++) {
		if (IS_VALID_ID(p_lane_id[i])) {
			mipi_rx_set_lane_id(devno, i, p_lane_id[i]);
		}
	}
}

void mipi_rx_drv_set_mem_cken(combo_dev_t devno, int enable)
{
	U_CHN0_MEM_CTRL chn0_mem_ctrl;
	// U_CHN1_MEM_CTRL chn1_mem_ctrl;
	mipi_rx_sys_regs_t *mipi_rx_sys_regs;

	mipi_rx_sys_regs = get_mipi_rx_sys_regs();

	switch (devno) {
	case 0:
		chn0_mem_ctrl.u32 = mipi_rx_sys_regs->CHN0_MEM_CTRL.u32;
		chn0_mem_ctrl.bits.chn0_mem_ck_gt = enable;
		mipi_rx_sys_regs->CHN0_MEM_CTRL.u32 = chn0_mem_ctrl.u32;
		break;
	default:
		break;
	}
}

void mipi_rx_drv_set_clr_cken(combo_dev_t devno, int enable)
{
	U_CHN0_CLR_EN chn0_clr_en;
	// U_CHN1_CLR_EN chn1_clr_en;
	mipi_rx_sys_regs_t *mipi_rx_sys_regs;

	mipi_rx_sys_regs = get_mipi_rx_sys_regs();

	switch (devno) {
	case 0:
		chn0_clr_en.u32 = mipi_rx_sys_regs->CHN0_CLR_EN.u32;
		chn0_clr_en.bits.chn0_clr_en_lvds = enable;
		chn0_clr_en.bits.chn0_clr_en_align = enable;
		mipi_rx_sys_regs->CHN0_CLR_EN.u32 = chn0_clr_en.u32;
		break;

	default:
		break;
	}
}

void mipi_rx_drv_set_lane_num(combo_dev_t devno, unsigned int lane_num)
{
	U_MIPI_LANES_NUM mipi_lanes_num;
	mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

	mipi_lanes_num.u32 = mipi_ctrl_regs->MIPI_LANES_NUM.u32;
	mipi_lanes_num.bits.lane_num = lane_num - 1;
	mipi_ctrl_regs->MIPI_LANES_NUM.u32 = mipi_lanes_num.u32;
}

void mipi_rx_drv_set_phy_en_link(mipi_rx_phy_t phy_id, unsigned int lane_bitmap)
{
	U_PHY_EN_LINK phy_en_link;
	mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

	mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
	phy_en_link.u32 = mipi_rx_phy_cfg->PHY_EN_LINK.u32;

	if (lane_bitmap & 0x5) {
		phy_en_link.bits.phy_da_d0_valid = lane_bitmap & 0x1;
		phy_en_link.bits.phy_da_d2_valid = (lane_bitmap & 0x4) >> 2;

		phy_en_link.bits.phy_d0_term_en = lane_bitmap & 0x1;
		phy_en_link.bits.phy_d2_term_en = (lane_bitmap & 0x4) >> 2;

		phy_en_link.bits.phy_clk0_term_en = 1;
	}

	if (lane_bitmap & 0xa) {
		phy_en_link.bits.phy_da_d1_valid = (lane_bitmap & 0x2) >> 1;
		phy_en_link.bits.phy_da_d3_valid = (lane_bitmap & 0x8) >> 3;
		phy_en_link.bits.phy_d1_term_en = (lane_bitmap & 0x2) >> 1;
		phy_en_link.bits.phy_d3_term_en = (lane_bitmap & 0x8) >> 3;
		phy_en_link.bits.phy_clk1_term_en = 1;
	}

	mipi_rx_phy_cfg->PHY_EN_LINK.u32 = phy_en_link.u32;
}

void mipi_rx_drv_set_phy_mode(mipi_rx_phy_t phy_id, input_mode_t input_mode,
			      unsigned int lane_bitmap)
{
	U_PHY_MODE_LINK phy_mode_link;
	mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;
	int cmos_en = 0;

	mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
	phy_mode_link.u32 = mipi_rx_phy_cfg->PHY_MODE_LINK.u32;

	if (input_mode == INPUT_MODE_CMOS || input_mode == INPUT_MODE_BT1120 ||
	    input_mode == INPUT_MODE_BYPASS) {
		cmos_en = 1;
	}

	phy_mode_link.bits.phy0_rg_en_d = phy_mode_link.bits.phy0_rg_en_d |
					  (lane_bitmap & 0xf);
	phy_mode_link.bits.phy0_rg_en_cmos = cmos_en;
	phy_mode_link.bits.phy0_rg_en_clk0 = 1;
	phy_mode_link.bits.phy0_rg_mipi_mode0 = 1;

	if (lane_bitmap & 0xa) {
		phy_mode_link.bits.phy0_rg_en_clk1 =
			1; // -- 2lane+2lane maybe need test
		phy_mode_link.bits.phy0_rg_mipi_mode1 = 1;
	}

	mipi_rx_phy_cfg->PHY_MODE_LINK.u32 = phy_mode_link.u32;
}

void mipi_rx_drv_set_cmos_en(mipi_rx_phy_t phy_id, int enable)
{
	volatile U_PHY_MODE_LINK phy_mode_link;
	mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

	mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
	phy_mode_link.u32 = mipi_rx_phy_cfg->PHY_MODE_LINK.u32;
	phy_mode_link.bits.phy0_rg_en_cmos = enable;
	mipi_rx_phy_cfg->PHY_MODE_LINK.u32 = phy_mode_link.u32;
}
void mipi_rx_drv_set_phy_en(unsigned int lane_bitmap)
{
	U_PHY_EN phy_en;
	mipi_rx_sys_regs_t *mipi_rx_sys_regs;

	mipi_rx_sys_regs = get_mipi_rx_sys_regs();
	phy_en.u32 = mipi_rx_sys_regs->PHY_EN.u32;

	if (lane_bitmap & 0xf) {
		phy_en.bits.phy0_en = 1;
	}

	mipi_rx_sys_regs->PHY_EN.u32 = phy_en.u32;
}

void mipi_rx_drv_set_lane_en(unsigned int lane_bitmap)
{
	U_LANE_EN lane_en;
	mipi_rx_sys_regs_t *mipi_rx_sys_regs;

	mipi_rx_sys_regs = get_mipi_rx_sys_regs();
	lane_en.u32 = mipi_rx_sys_regs->LANE_EN.u32;
	lane_en.u32 = (lane_bitmap & 0xf);
	mipi_rx_sys_regs->LANE_EN.u32 = lane_en.u32;
}

void mipi_rx_drv_set_phy_cil_en(unsigned int lane_bitmap, int enable)
{
	U_PHY_CIL_CTRL phy_cil_ctrl;
	mipi_rx_sys_regs_t *mipi_rx_sys_regs;

	mipi_rx_sys_regs = get_mipi_rx_sys_regs();
	phy_cil_ctrl.u32 = mipi_rx_sys_regs->PHY_CIL_CTRL.u32;

	if (lane_bitmap & 0xf) {
		phy_cil_ctrl.bits.phycil0_cken = enable;
	}

	mipi_rx_sys_regs->PHY_CIL_CTRL.u32 = phy_cil_ctrl.u32;
}

void mipi_rx_drv_set_phy_cfg_mode(input_mode_t input_mode,
				  unsigned int lane_bitmap)
{
	U_PHYCFG_MODE phycfg_mode;
	mipi_rx_sys_regs_t *mipi_rx_sys_regs;
	unsigned int cfg_mode;
	unsigned int cfg_mode_sel;

	mipi_rx_sys_regs = get_mipi_rx_sys_regs();
	phycfg_mode.u32 = mipi_rx_sys_regs->PHYCFG_MODE.u32;

	if (input_mode == INPUT_MODE_MIPI) {
		cfg_mode = 0;
		cfg_mode_sel = 0;
	} else if (input_mode == INPUT_MODE_SUBLVDS ||
		   input_mode == INPUT_MODE_LVDS ||
		   input_mode == INPUT_MODE_HISPI) {
		cfg_mode = 1;
		cfg_mode_sel = 0; /* RAW */
	} else {
		cfg_mode = 2;
		cfg_mode_sel = 1; /* CMOS */
	}

	if (lane_bitmap & 0xf) {
		phycfg_mode.bits.phycil0_0_cfg_mode = cfg_mode;
		phycfg_mode.bits.phycil0_cfg_mode_sel = cfg_mode_sel;
	} else if (lane_bitmap & 0x5) {
		phycfg_mode.bits.phycil0_0_cfg_mode = cfg_mode;
		phycfg_mode.bits.phycil0_cfg_mode_sel = cfg_mode_sel;
	}

	mipi_rx_sys_regs->PHYCFG_MODE.u32 = phycfg_mode.u32;
}

void mipi_rx_drv_set_phy_cfg_en(unsigned int lane_bitmap, int enable)
{
	U_PHYCFG_EN phycfg_en;
	mipi_rx_sys_regs_t *mipi_rx_sys_regs;

	mipi_rx_sys_regs = get_mipi_rx_sys_regs();
	phycfg_en.u32 = mipi_rx_sys_regs->PHYCFG_EN.u32;

	if (lane_bitmap & 0xf) {
		phycfg_en.bits.phycil0_cfg_en = enable;
	}

	mipi_rx_sys_regs->PHYCFG_EN.u32 = phycfg_en.u32;
}

void mipi_rx_drv_set_phy_config(input_mode_t input_mode,
				unsigned int lane_bitmap)
{
	unsigned int i;
	unsigned int mask;
	unsigned int phy_lane_bitmap;

	for (i = 0; i < MIPI_RX_MAX_PHY_NUM; i++) {
		mask = 0xf << (4 * i);
		if (lane_bitmap & mask) {
			phy_lane_bitmap = (lane_bitmap & mask) >> (4 * i);
			mipi_rx_drv_set_phy_en_link(i, phy_lane_bitmap);
			mipi_rx_drv_set_phy_mode(i, input_mode,
						 phy_lane_bitmap);
		}
	}

	mipi_rx_drv_set_phy_en(lane_bitmap);
	mipi_rx_drv_set_lane_en(lane_bitmap);
	mipi_rx_drv_set_phy_cil_en(lane_bitmap, 1);
	mipi_rx_drv_set_phy_cfg_mode(input_mode, lane_bitmap);
	mipi_rx_drv_set_phy_cfg_en(lane_bitmap, 1);
}

static void mipi_rx_drv_set_phy_cmv(mipi_rx_phy_t phy_id,
				    phy_cmv_mode_t cmv_mode,
				    unsigned int lane_bitmap)
{
	int mipi_cmv_mode = 0;
	U_PHY_MODE_LINK phy_mode_link;
	mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

	if (cmv_mode == PHY_CMV_GE1200MV) {
		mipi_cmv_mode = 0;
	} else if (cmv_mode == PHY_CMV_LT1200MV) {
		mipi_cmv_mode = 1;
	}

	mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
	phy_mode_link.u32 = mipi_rx_phy_cfg->PHY_MODE_LINK.u32;

	if (lane_bitmap & 0xa) {
		phy_mode_link.bits.phy0_rg_mipi_mode1 = mipi_cmv_mode;
	}

	if (lane_bitmap & 0x5) {
		phy_mode_link.bits.phy0_rg_mipi_mode0 = mipi_cmv_mode;
	}

	mipi_rx_phy_cfg->PHY_MODE_LINK.u32 = phy_mode_link.u32;
}

void mipi_rx_drv_set_phy_cmvmode(input_mode_t input_mode,
				 phy_cmv_mode_t cmv_mode,
				 unsigned int lane_bitmap)
{
	unsigned int i;
	unsigned int mask;
	unsigned int phy_lane_bitmap;

	for (i = 0; i < MIPI_RX_MAX_PHY_NUM; i++) {
		mask = 0xf << (4 * i);
		if (lane_bitmap & mask) {
			phy_lane_bitmap = (lane_bitmap & mask) >> (4 * i);
			mipi_rx_drv_set_phy_cmv(i, cmv_mode, phy_lane_bitmap);
		}
	}

	mipi_rx_drv_set_phy_cfg_mode(input_mode, lane_bitmap);
	mipi_rx_drv_set_phy_cfg_en(lane_bitmap, 1);
}
void mipi_rx_drv_set_lvds_image_rect(combo_dev_t devno, img_rect_t *p_img_rect,
				     short total_lane_num)
{
	volatile lvds_ctrl_regs_t *pstCtrlReg;
	U_LVDS_IMGSIZE lvds_img_size;
	U_LVDS_CROP_START0 crop_start0;
	U_LVDS_CROP_START1 crop_start1;
	unsigned int width_per_lane, x_per_lane;

	pstCtrlReg = get_lvds_ctrl_regs(devno);

	width_per_lane = (p_img_rect->width / total_lane_num);
	x_per_lane = (p_img_rect->x / total_lane_num);

	lvds_img_size.u32 = pstCtrlReg->LVDS_IMGSIZE.u32;
	crop_start0.u32 = pstCtrlReg->LVDS_CROP_START0.u32;
	crop_start1.u32 = pstCtrlReg->LVDS_CROP_START1.u32;

	lvds_img_size.bits.lvds_imgwidth_lane = width_per_lane - 1;
	lvds_img_size.bits.lvds_imgheight = p_img_rect->height - 1;

	crop_start0.bits.lvds_start_x0_lane = x_per_lane;
	crop_start0.bits.lvds_start_y0 = p_img_rect->y;

	crop_start1.bits.lvds_start_x1_lane = x_per_lane;
	crop_start1.bits.lvds_start_y1 = p_img_rect->y;

	pstCtrlReg->LVDS_IMGSIZE.u32 = lvds_img_size.u32;
	pstCtrlReg->LVDS_CROP_START0.u32 = crop_start0.u32;
	pstCtrlReg->LVDS_CROP_START1.u32 = crop_start1.u32;
}

void mipi_rx_drv_set_lvds_crop_en(combo_dev_t devno, int enable)
{
	volatile lvds_ctrl_regs_t *pstCtrlReg;
	U_LVDS_CTRL lvds_ctrl;

	pstCtrlReg = get_lvds_ctrl_regs(devno);

	if (pstCtrlReg == NULL) {
		return;
	}

	lvds_ctrl.u32 = pstCtrlReg->LVDS_CTRL.u32;

	lvds_ctrl.bits.lvds_crop_en = enable;

	pstCtrlReg->LVDS_CTRL.u32 = lvds_ctrl.u32;
}

int mipi_rx_drv_set_lvds_wdr_mode(combo_dev_t devno, wdr_mode_t wdr_mode,
				  lvds_vsync_attr_t *vsync_attr,
				  lvds_fid_attr_t *fid_attr)
{
	volatile lvds_ctrl_regs_t *pstCtrlReg;
	U_LVDS_WDR lvds_wdr;
	U_LVDS_DOLSCD_HBLK scd_hblk;

	pstCtrlReg = get_lvds_ctrl_regs(devno);

	lvds_wdr.u32 = pstCtrlReg->LVDS_WDR.u32;
	scd_hblk.u32 = pstCtrlReg->LVDS_DOLSCD_HBLK.u32;

	if (wdr_mode == HI_WDR_MODE_NONE) {
		lvds_wdr.bits.lvds_wdr_en = 0;
		lvds_wdr.bits.lvds_wdr_num = 0;
	} else {
		lvds_wdr.bits.lvds_wdr_en = 1;

		// set the wdr frame number
		switch (wdr_mode) {
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
			return -1;
		}

		// set wdr mode
		if (wdr_mode >= HI_WDR_MODE_2F && wdr_mode <= HI_WDR_MODE_4F) {
			if (vsync_attr->sync_type == LVDS_VSYNC_NORMAL) {
				// SOF-EOF WDR, long exposure frame and short exposure frame has independent sync code
				lvds_wdr.bits.lvds_wdr_mode = 0x0;
			} else if (vsync_attr->sync_type == LVDS_VSYNC_SHARE) {
				// SOF-EOF WDR, long exposure frame and short exposure frame share the SOF and EOF
				lvds_wdr.bits.lvds_wdr_mode = 0x2;
			} else {
				HI_ERR("not support vsync type: %d\n",
				       vsync_attr->sync_type);
				return -1;
			}
		} else if (wdr_mode >= HI_WDR_MODE_DOL_2F &&
			   wdr_mode <= HI_WDR_MODE_DOL_4F) {
			// Sony DOL WDR
			if (vsync_attr->sync_type == LVDS_VSYNC_NORMAL) {
				// SAV-EAV WDR, 4 sync code, fid embedded in 4th sync code
				// long exposure fame and short exposure frame has independent sync code
				if (fid_attr->fid_type == LVDS_FID_IN_SAV) {
					lvds_wdr.bits.lvds_wdr_mode = 0x4;
				} else if (fid_attr->fid_type ==
					   LVDS_FID_IN_DATA) {
					// SAV-EAV WDR, 5 sync code(Line Information), fid in the fist DATA,
					/*  fid in data, line information */
					if (fid_attr->output_fil) {
						// Frame Information Line is included in the image data
						lvds_wdr.bits.lvds_wdr_mode =
							0xd;
					} else {
						// Frame Information Line is not included in the image data
						lvds_wdr.bits.lvds_wdr_mode =
							0x6;
					}
				} else {
					HI_ERR("not support fid type: %d\n",
					       fid_attr->fid_type);
					return -1;
				}
			} else if (vsync_attr->sync_type ==
				   LVDS_VSYNC_HCONNECT) {
				// SAV-EAV H-Connection DOL, long exposure frame and short exposure frame
				// share the same SAV EAV, the H-Blank is assigned by the dol_hblank1 and dol_hblank2
				if (fid_attr->fid_type == LVDS_FID_NONE) {
					lvds_wdr.bits.lvds_wdr_mode = 0x5;
				} else {
					HI_ERR("not support fid type: %d\n",
					       fid_attr->fid_type);
					return -1;
				}
				scd_hblk.bits.dol_hblank1 = vsync_attr->hblank1;
				scd_hblk.bits.dol_hblank2 = vsync_attr->hblank2;
			} else {
				HI_ERR("not support vsync type: %d\n",
				       vsync_attr->sync_type);
				return -1;
			}
		}
	}

	pstCtrlReg->LVDS_WDR.u32 = lvds_wdr.u32;
	pstCtrlReg->LVDS_DOLSCD_HBLK.u32 = scd_hblk.u32;

	return 0;
}

void mipi_rx_drv_set_lvds_ctrl_mode(combo_dev_t devno,
				    lvds_sync_mode_t sync_mode,
				    data_type_t input_data_type,
				    lvds_bit_endian_t data_endian,
				    lvds_bit_endian_t sync_code_endian)
{
	volatile lvds_ctrl_regs_t *pstCtrlReg;
	U_LVDS_CTRL lvds_ctrl;
	unsigned short raw_type;

	pstCtrlReg = get_lvds_ctrl_regs(devno);

	lvds_ctrl.u32 = pstCtrlReg->LVDS_CTRL.u32;

	switch (input_data_type) {
	case DATA_TYPE_RAW_8BIT:
		raw_type = 0x1;
		break;

	case DATA_TYPE_RAW_10BIT:
		raw_type = 0x2;
		break;

	case DATA_TYPE_RAW_12BIT:
		raw_type = 0x3;
		break;

	case DATA_TYPE_RAW_14BIT:
		raw_type = 0x4;
		break;

	case DATA_TYPE_RAW_16BIT:
		raw_type = 0x5;
		break;

	default:
		return;
	}

	lvds_ctrl.bits.lvds_sync_mode = sync_mode;
	lvds_ctrl.bits.lvds_raw_type = raw_type;
	lvds_ctrl.bits.lvds_pix_big_endian = data_endian;
	lvds_ctrl.bits.lvds_code_big_endian = sync_code_endian;

	pstCtrlReg->LVDS_CTRL.u32 = lvds_ctrl.u32;
}

void mipi_rx_drv_set_lvds_data_rate(combo_dev_t devno,
				    mipi_data_rate_t data_rate)
{
}

void mipi_rx_drv_set_dol_line_information(combo_dev_t devno,
					  wdr_mode_t wdr_mode)
{
	volatile lvds_ctrl_regs_t *pstCtrlReg;

	pstCtrlReg = get_lvds_ctrl_regs(devno);

	if (wdr_mode >= HI_WDR_MODE_DOL_2F) {
		pstCtrlReg->LVDS_LI_WORD0.bits.li_word0_0 =
			0x0201; // LEF n frame
		pstCtrlReg->LVDS_LI_WORD0.bits.li_word0_1 =
			0x0211; // LEF n + 1 frame

		pstCtrlReg->LVDS_LI_WORD1.bits.li_word1_0 =
			0x0202; // SEF1 n frame
		pstCtrlReg->LVDS_LI_WORD1.bits.li_word1_1 =
			0x0212; // SEF1 n + 1 frame
	}
}

void set_lvds_sync_code(combo_dev_t devno, int n_frame, unsigned int lane_cnt,
			const short lane_id[LVDS_LANE_NUM],
			unsigned short sync_code[][WDR_VC_NUM][SYNC_CODE_NUM])
{
	int i;
	short lane_idx;
	volatile lvds_ctrl_regs_t *pstCtrlReg;
	volatile lvds_sync_code_cfg_t lane_sync_code, *p_sync_code;

	pstCtrlReg = get_lvds_ctrl_regs(devno);

	for (i = 0; i < LVDS_LANE_NUM; i++) {
		lane_idx = lane_id[i];
		if (-1 != lane_idx) {
			if (n_frame == TRUE) {
				p_sync_code =
					&pstCtrlReg
						 ->lvds_this_frame_sync_code[i];
			} else {
				p_sync_code =
					&pstCtrlReg
						 ->lvds_next_frame_sync_code[i];
			}

			lane_sync_code = *p_sync_code;

			{
				U_LVDS_LANE_SOF_01 lvds_sof_01;
				lvds_sof_01.u32 =
					lane_sync_code.LVDS_LANE_SOF_01.u32;
				lvds_sof_01.bits.lane_sof_0 =
					sync_code[i][0][0];
				lvds_sof_01.bits.lane_sof_1 =
					sync_code[i][1][0];
				p_sync_code->LVDS_LANE_SOF_01.u32 =
					lvds_sof_01.u32;
			}
			{
				U_LVDS_LANE_EOF_01 lvds_eof_01;
				lvds_eof_01.u32 =
					lane_sync_code.LVDS_LANE_EOF_01.u32;
				lvds_eof_01.bits.lane_eof_0 =
					sync_code[i][0][1];
				lvds_eof_01.bits.lane_eof_1 =
					sync_code[i][1][1];
				p_sync_code->LVDS_LANE_EOF_01.u32 =
					lvds_eof_01.u32;
			}
			{
				U_LVDS_LANE_SOL_01 lvds_sol_01;
				lvds_sol_01.u32 =
					lane_sync_code.LVDS_LANE_SOL_01.u32;
				lvds_sol_01.bits.lane_sol_0 =
					sync_code[i][0][2];
				lvds_sol_01.bits.lane_sol_1 =
					sync_code[i][1][2];
				p_sync_code->LVDS_LANE_SOL_01.u32 =
					lvds_sol_01.u32;
			}
			{
				U_LVDS_LANE_EOL_01 lvds_eol_01;
				lvds_eol_01.u32 =
					lane_sync_code.LVDS_LANE_EOL_01.u32;
				lvds_eol_01.bits.lane_eol_0 =
					sync_code[i][0][3];
				lvds_eol_01.bits.lane_eol_1 =
					sync_code[i][1][3];
				p_sync_code->LVDS_LANE_EOL_01.u32 =
					lvds_eol_01.u32;
			}
		}
	}
}

void mipi_rx_drv_set_lvds_sync_code(
	combo_dev_t devno, unsigned int lane_cnt,
	const short lane_id[LVDS_LANE_NUM],
	unsigned short sync_code[][WDR_VC_NUM][SYNC_CODE_NUM])
{
	set_lvds_sync_code(devno, TRUE, lane_cnt, lane_id, sync_code);
}

void mipi_rx_drv_set_lvds_nxt_sync_code(
	combo_dev_t devno, unsigned int lane_cnt,
	const short lane_id[LVDS_LANE_NUM],
	unsigned short sync_code[][WDR_VC_NUM][SYNC_CODE_NUM])
{
	set_lvds_sync_code(devno, FALSE, lane_cnt, lane_id, sync_code);
}

void mipi_rx_drv_set_phy_sync_dct(mipi_rx_phy_t phy_id, int raw_type,
				  lvds_bit_endian_t code_endian,
				  unsigned int phy_lane_bitmap)
{
	U_PHY_SYNC_DCT_LINK phy_sync_dct_link;
	mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

	mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
	phy_sync_dct_link.u32 = mipi_rx_phy_cfg->PHY_SYNC_DCT_LINK.u32;

	if (phy_lane_bitmap & 0x5) {
		phy_sync_dct_link.bits.cil_raw_type0 = raw_type;
		phy_sync_dct_link.bits.cil_code_big_endian0 = code_endian;
	}

	if (phy_lane_bitmap & 0xa) {
		phy_sync_dct_link.bits.cil_raw_type1 = raw_type;
		phy_sync_dct_link.bits.cil_code_big_endian1 = code_endian;
	}

	mipi_rx_phy_cfg->PHY_SYNC_DCT_LINK.u32 = phy_sync_dct_link.u32;
}

short get_sensor_lane_index(short lane, const short lane_id[LVDS_LANE_NUM])
{
	int i = 0;

	for (i = 0; i < LVDS_LANE_NUM; i++) {
		if (lane_id[i] == lane) {
			break;
		}
	}

	return i;
}

void mipi_rx_drv_set_lvds_phy_sync_code(
	mipi_rx_phy_t phy_id, const short lane_id[LVDS_LANE_NUM],
	unsigned short n_sync_code[][WDR_VC_NUM][SYNC_CODE_NUM],
	unsigned short nxt_sync_code[][WDR_VC_NUM][SYNC_CODE_NUM],
	unsigned int phy_lane_bitmap)
{
	U_PHY_SYNC_SOF0_LINK phy_sync_sof0_link;
	U_PHY_SYNC_SOF1_LINK phy_sync_sof1_link;
	U_PHY_SYNC_SOF2_LINK phy_sync_sof2_link;
	U_PHY_SYNC_SOF3_LINK phy_sync_sof3_link;
	mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;
	short sensor_lane_idx;
	short lane;

	mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
	phy_sync_sof0_link.u32 = mipi_rx_phy_cfg->PHY_SYNC_SOF0_LINK.u32;
	phy_sync_sof1_link.u32 = mipi_rx_phy_cfg->PHY_SYNC_SOF1_LINK.u32;
	phy_sync_sof2_link.u32 = mipi_rx_phy_cfg->PHY_SYNC_SOF2_LINK.u32;
	phy_sync_sof3_link.u32 = mipi_rx_phy_cfg->PHY_SYNC_SOF3_LINK.u32;

	if (phy_lane_bitmap & 0x1) {
		lane = 0 + 4 * phy_id;
		sensor_lane_idx = get_sensor_lane_index(lane, lane_id);
		phy_sync_sof0_link.bits.cil_sof0_word4_0 =
			n_sync_code[sensor_lane_idx][0][0];
		phy_sync_sof0_link.bits.cil_sof1_word4_0 =
			nxt_sync_code[sensor_lane_idx][0][0];
	}

	if (phy_lane_bitmap & 0x2) {
		lane = 1 + 4 * phy_id;
		sensor_lane_idx = get_sensor_lane_index(lane, lane_id);
		phy_sync_sof1_link.bits.cil_sof0_word4_1 =
			n_sync_code[sensor_lane_idx][0][0];
		phy_sync_sof1_link.bits.cil_sof1_word4_1 =
			nxt_sync_code[sensor_lane_idx][0][0];
	}

	if (phy_lane_bitmap & 0x4) {
		lane = 2 + 4 * phy_id;
		sensor_lane_idx = get_sensor_lane_index(lane, lane_id);
		phy_sync_sof2_link.bits.cil_sof0_word4_2 =
			n_sync_code[sensor_lane_idx][0][0];
		phy_sync_sof2_link.bits.cil_sof1_word4_2 =
			nxt_sync_code[sensor_lane_idx][0][0];
	}

	if (phy_lane_bitmap & 0x8) {
		lane = 3 + 4 * phy_id;
		sensor_lane_idx = get_sensor_lane_index(lane, lane_id);
		phy_sync_sof3_link.bits.cil_sof0_word4_3 =
			n_sync_code[sensor_lane_idx][0][0];
		phy_sync_sof3_link.bits.cil_sof1_word4_3 =
			nxt_sync_code[sensor_lane_idx][0][0];
	}

	mipi_rx_phy_cfg->PHY_SYNC_SOF0_LINK.u32 = phy_sync_sof0_link.u32;
	mipi_rx_phy_cfg->PHY_SYNC_SOF1_LINK.u32 = phy_sync_sof1_link.u32;
	mipi_rx_phy_cfg->PHY_SYNC_SOF2_LINK.u32 = phy_sync_sof2_link.u32;
	mipi_rx_phy_cfg->PHY_SYNC_SOF3_LINK.u32 = phy_sync_sof3_link.u32;
}

void mipi_rx_drv_set_phy_sync_config(
	lvds_dev_attr_t *p_attr, unsigned int lane_bitmap,
	unsigned short nxt_sync_code[][WDR_VC_NUM][SYNC_CODE_NUM])
{
	int raw_type;
	unsigned int i;
	unsigned int mask;
	unsigned int phy_lane_bitmap;

	switch (p_attr->input_data_type) {
	case DATA_TYPE_RAW_8BIT:
		raw_type = 0x1;
		break;

	case DATA_TYPE_RAW_10BIT:
		raw_type = 0x2;
		break;

	case DATA_TYPE_RAW_12BIT:
		raw_type = 0x3;
		break;

	case DATA_TYPE_RAW_14BIT:
		raw_type = 0x4;
		break;

	case DATA_TYPE_RAW_16BIT:
		raw_type = 0x5;
		break;

	default:
		return;
	}

	for (i = 0; i < MIPI_RX_MAX_PHY_NUM; i++) {
		mask = 0xf << (4 * i);
		if (lane_bitmap & mask) {
			phy_lane_bitmap = (lane_bitmap & mask) >> (4 * i);
			mipi_rx_drv_set_phy_sync_dct(i, raw_type,
						     p_attr->sync_code_endian,
						     phy_lane_bitmap);
			mipi_rx_drv_set_lvds_phy_sync_code(i, p_attr->lane_id,
							   p_attr->sync_code,
							   nxt_sync_code,
							   phy_lane_bitmap);
		}
	}
}

int mipi_rx_drv_is_lane_valid(combo_dev_t devno, short lane_id,
			      lane_divide_mode_t mode)
{
	int lane_valid = 0;

	switch (mode) {
	case LANE_DIVIDE_MODE_0:
		if (devno == 0) {
			if (0 <= lane_id && lane_id <= 3) {
				lane_valid = 1;
			}
		}
		break;
	default:
		break;
	}

	return lane_valid;
}

static void mipi_rx_drv_hw_init(void);
void mipi_rx_drv_set_hs_mode(lane_divide_mode_t mode)
{
	int i;

	mipi_rx_drv_hw_init();

	for (i = 0; i < MIPI_RX_MAX_PHY_NUM; i++) {
		mipi_rx_set_phy_rg_2121_en(i, phy_mode[mode][i].phy_rg_en_2121);
		mipi_rx_set_phy_rg_clk0_en(i, phy_mode[mode][i].phy_rg_clk0_en);
		mipi_rx_set_phy_rg_clk1_en(i, phy_mode[mode][i].phy_rg_clk1_en);
		mipi_rx_set_phy_rg_lp0_mode_en(
			i, phy_mode[mode][i].phy_rg_lp0_mode_en);
		mipi_rx_set_phy_rg_lp1_mode_en(
			i, phy_mode[mode][i].phy_rg_lp1_mode_en);
	}
}

void mipi_rx_drv_set_mipi_int_mask(combo_dev_t devno)
{
	U_MIPI_INT_MSK mipi_int_msk;
	volatile mipi_rx_sys_regs_t *mipi_rx_sys_regs = get_mipi_rx_sys_regs();

	mipi_int_msk.u32 = mipi_rx_sys_regs->MIPI_INT_MSK.u32;

	if (devno == 0) {
		mipi_int_msk.bits.int_chn0_mask = 0x1;
	}
	mipi_rx_sys_regs->MIPI_INT_MSK.u32 = mipi_int_msk.u32;
}

void mipi_rx_drv_set_chn_int_mask(combo_dev_t devno, unsigned int mask)
{
	volatile lvds_ctrl_regs_t *lvds_ctrl_regs = get_lvds_ctrl_regs(devno);

	lvds_ctrl_regs->CHN_INT_MASK.u32 = mask;
}

void mipi_rx_drv_set_lvds_ctrl_int_mask(combo_dev_t devno, unsigned int mask)
{
	volatile lvds_ctrl_regs_t *lvds_ctrl_regs = get_lvds_ctrl_regs(devno);

	lvds_ctrl_regs->LVDS_CTRL_INT_MSK.u32 = mask;
}

void mipi_rx_drv_set_mipi_ctrl_int_mask(combo_dev_t devno, unsigned int mask)
{
	volatile mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

	mipi_ctrl_regs->MIPI_CTRL_INT_MSK.u32 = mask;
}

void mipi_rx_drv_set_mipi_pkt1_int_mask(combo_dev_t devno, unsigned int mask)
{
	volatile mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

	mipi_ctrl_regs->MIPI_PKT_INTR_MSK.u32 = mask;
}

void mipi_rx_drv_set_mipi_pkt2_int_mask(combo_dev_t devno, unsigned int mask)
{
	volatile mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

	mipi_ctrl_regs->MIPI_PKT_INTR2_MSK.u32 = mask;
}

void mipi_rx_drv_set_mipi_frame_int_mask(combo_dev_t devno, unsigned int mask)
{
	volatile mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

	mipi_ctrl_regs->MIPI_FRAME_INTR_MSK.u32 = mask;
}

void mipi_rx_drv_set_align_int_mask(combo_dev_t devno, unsigned int mask)
{
	volatile lvds_ctrl_regs_t *lvds_ctrl_regs = get_lvds_ctrl_regs(devno);

	lvds_ctrl_regs->ALIGN_INT_MSK.u32 = mask;
	lvds_ctrl_regs->CHN_INT_MASK.u32 = 0xf;
}
void mipi_rx_enable_disable_clock(combo_dev_t combo_dev, int enable)
{
	void *mipi_rx_clock_addr;

	mipi_rx_clock_addr = osal_ioremap(MIPI_RX_CRG_ADDR, (unsigned long)0x4);
	if (mipi_rx_clock_addr == NULL) {
		HI_ERR("mipi_rx clock ioremap failed!\n");
		return;
	}
	set_bit(enable, 2, mipi_rx_clock_addr);

	osal_iounmap((void *)mipi_rx_clock_addr);
}

void mipi_rx_drv_enable_clock(combo_dev_t combo_dev)
{
	mipi_rx_enable_disable_clock(combo_dev, 1);
}

void mipi_rx_drv_disable_clock(combo_dev_t combo_dev)
{
	mipi_rx_enable_disable_clock(combo_dev, 0);
}

void sensor_enable_disable_clock(sns_clk_source_t sns_clk_source, int enable)
{
	void *sensor_clock_addr;
	unsigned offset;

	if (sns_clk_source == 0) {
		offset = 0;
	} else if (sns_clk_source == 1) {
		offset = 6;
	} else {
		HI_ERR("invalid sensor clock source!\n");
		return;
	}

	sensor_clock_addr = osal_ioremap(SNS_CRG_ADDR, (unsigned long)0x4);
	if (sensor_clock_addr == NULL) {
		HI_ERR("sensor clock ioremap failed!\n");
		return;
	}
	set_bit(enable, offset, sensor_clock_addr);
	osal_iounmap((void *)sensor_clock_addr);
}

void sensor_drv_enable_clock(sns_clk_source_t sns_clk_source)
{
	sensor_enable_disable_clock(sns_clk_source, 1);
}

void sensor_drv_disable_clock(sns_clk_source_t sns_clk_source)
{
	sensor_enable_disable_clock(sns_clk_source, 0);
}

void mipi_rx_core_reset_unreset(combo_dev_t combo_dev, int reset)
{
	set_bit(reset, 4, mipi_rx_core_reset_addr);
}

void mipi_rx_drv_core_reset(combo_dev_t combo_dev)
{
	mipi_rx_core_reset_unreset(combo_dev, 1);
}

void mipi_rx_drv_core_unreset(combo_dev_t combo_dev)
{
	mipi_rx_core_reset_unreset(combo_dev, 0);
}

void sensor_reset_unreset(sns_rst_source_t sns_reset_source, int reset)
{
	void *sensor_reset_addr;
	unsigned offset;

	if (sns_reset_source == 0) {
		offset = 1;
	} else {
		HI_ERR("invalid sensor reset source!\n");
		return;
	}

	sensor_reset_addr = osal_ioremap(SNS_CRG_ADDR, (unsigned long)0x4);
	if (sensor_reset_addr == NULL) {
		HI_ERR("sensor reset ioremap failed!\n");
		return;
	}
	set_bit(reset, offset, sensor_reset_addr);
	osal_iounmap((void *)sensor_reset_addr);
}

void sensor_drv_reset(sns_rst_source_t sns_reset_source)
{
	sensor_reset_unreset(sns_reset_source, 1);
}

void sensor_drv_unreset(sns_rst_source_t sns_reset_source)
{
	sensor_reset_unreset(sns_reset_source, 0);
}

void mipi_rx_drv_get_mipi_imgsize_statis(combo_dev_t devno, short vc,
					 img_size_t *p_size)
{
	U_MIPI_IMGSIZE0_STATIS MIPI_IMGSIZE0_STATIS;
	U_MIPI_IMGSIZE1_STATIS MIPI_IMGSIZE1_STATIS;

	volatile mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

	if (vc == 0) {
		MIPI_IMGSIZE0_STATIS.u32 =
			mipi_ctrl_regs->MIPI_IMGSIZE0_STATIS.u32;
		p_size->width = MIPI_IMGSIZE0_STATIS.bits.imgwidth_statis_vc0;
		p_size->height = MIPI_IMGSIZE0_STATIS.bits.imgheight_statis_vc0;
	} else if (vc == 1) {
		MIPI_IMGSIZE1_STATIS.u32 =
			mipi_ctrl_regs->MIPI_IMGSIZE1_STATIS.u32;
		p_size->width = MIPI_IMGSIZE1_STATIS.bits.imgwidth_statis_vc1;
		p_size->height = MIPI_IMGSIZE1_STATIS.bits.imgheight_statis_vc1;
	} else {
		p_size->width = 0;
		p_size->height = 0;
	}
}

void mipi_rx_drv_get_lvds_imgsize_statis(combo_dev_t devno, short vc,
					 img_size_t *p_size)
{
	U_LVDS_IMGSIZE0_STATIS LVDS_IMGSIZE0_STATIS;
	U_LVDS_IMGSIZE1_STATIS LVDS_IMGSIZE1_STATIS;

	volatile lvds_ctrl_regs_t *lvds_ctrl_regs = get_lvds_ctrl_regs(devno);

	if (vc == 0) {
		LVDS_IMGSIZE0_STATIS.u32 =
			lvds_ctrl_regs->LVDS_IMGSIZE0_STATIS.u32;
		p_size->width = LVDS_IMGSIZE0_STATIS.bits.lvds_imgwidth0;
		p_size->height = LVDS_IMGSIZE0_STATIS.bits.lvds_imgheight0;
	} else if (vc == 1) {
		LVDS_IMGSIZE1_STATIS.u32 =
			lvds_ctrl_regs->LVDS_IMGSIZE1_STATIS.u32;
		p_size->width = LVDS_IMGSIZE1_STATIS.bits.lvds_imgwidth1;
		p_size->height = LVDS_IMGSIZE1_STATIS.bits.lvds_imgheight1;
	}
}

void mipi_rx_drv_get_lvds_lane_imgsize_statis(combo_dev_t devno, short lane,
					      img_size_t *p_size)
{
	U_LVDS_LANE_IMGSIZE_STATIS LVDS_LANE_IMGSIZE_STATIS;

	volatile lvds_ctrl_regs_t *lvds_ctrl_regs = get_lvds_ctrl_regs(devno);

	LVDS_LANE_IMGSIZE_STATIS.u32 =
		lvds_ctrl_regs->LVDS_LANE_IMGSIZE_STATIS[lane].u32;
	p_size->width = LVDS_LANE_IMGSIZE_STATIS.bits.lane_imgwidth + 1;
	p_size->height = LVDS_LANE_IMGSIZE_STATIS.bits.lane_imgheight;
}

static void mipi_rx_phy_cil_int_statis(int phy_id)
{
	unsigned int phy_int_status;

	mipi_rx_phy_cfg_t *mipi_rx_phy_cfg;

	mipi_rx_phy_cfg = get_mipi_rx_phy_regs(phy_id);
	phy_int_status = mipi_rx_phy_cfg->MIPI_CIL_INT_LINK.u32;

	if (phy_int_status) {
		mipi_rx_phy_cfg->MIPI_CIL_INT_RAW_LINK.u32 = 0xffffffff;

		if (phy_int_status & MIPI_ESC_CLK1) {
			phy_err_int_cnt[phy_id].clk1_fsm_escape_err_cnt++;
		}

		if (phy_int_status & MIPI_ESC_CLK0) {
			phy_err_int_cnt[phy_id].clk0_fsm_escape_err_cnt++;
		}

		if (phy_int_status & MIPI_ESC_D0) {
			phy_err_int_cnt[phy_id].d0_fsm_escape_err_cnt++;
		}

		if (phy_int_status & MIPI_ESC_D1) {
			phy_err_int_cnt[phy_id].d1_fsm_escape_err_cnt++;
		}

		if (phy_int_status & MIPI_ESC_D2) {
			phy_err_int_cnt[phy_id].d2_fsm_escape_err_cnt++;
		}

		if (phy_int_status & MIPI_ESC_D3) {
			phy_err_int_cnt[phy_id].d3_fsm_escape_err_cnt++;
		}

		if (phy_int_status & MIPI_TIMEOUT_CLK1) {
			phy_err_int_cnt[phy_id].clk1_fsm_timeout_err_cnt++;
		}

		if (phy_int_status & MIPI_TIMEOUT_CLK0) {
			phy_err_int_cnt[phy_id].clk0_fsm_timeout_err_cnt++;
		}

		if (phy_int_status & MIPI_TIMEOUT_D0) {
			phy_err_int_cnt[phy_id].d0_fsm_timeout_err_cnt++;
		}

		if (phy_int_status & MIPI_TIMEOUT_D1) {
			phy_err_int_cnt[phy_id].d1_fsm_timeout_err_cnt++;
		}

		if (phy_int_status & MIPI_TIMEOUT_D2) {
			phy_err_int_cnt[phy_id].d2_fsm_timeout_err_cnt++;
		}

		if (phy_int_status & MIPI_TIMEOUT_D3) {
			phy_err_int_cnt[phy_id].d3_fsm_timeout_err_cnt++;
		}
	}
}

static void mipi_int_statics(combo_dev_t devno)
{
	unsigned int pkt_int1, pkt_int2;
	unsigned int frame_int;
	unsigned int line_int;
	unsigned int mipi_main_int;
	unsigned int mipi_ctrl_int;
	mipi_ctrl_regs_t *mipi_ctrl_regs = get_mipi_ctrl_regs(devno);

	pkt_int1 = mipi_ctrl_regs->MIPI_PKT_INTR_ST.u32;
	pkt_int2 = mipi_ctrl_regs->MIPI_PKT_INTR2_ST.u32;
	frame_int = mipi_ctrl_regs->MIPI_FRAME_INTR_ST.u32;
	mipi_ctrl_int = mipi_ctrl_regs->MIPI_CTRL_INT.u32;

	mipi_main_int = mipi_ctrl_regs->MIPI_MAIN_INT_ST.u32;
	line_int = mipi_ctrl_regs->MIPI_LINE_INTR_ST.u32;

	if (mipi_main_int) {
		mipi_ctrl_regs->MIPI_MAIN_INT_ST.u32 = 0xffffffff;
	}
	if (line_int) {
		mipi_ctrl_regs->MIPI_LINE_INTR_ST.u32 = 0xffffffff;
	}
	if (mipi_ctrl_int) {
		mipi_ctrl_regs->MIPI_CTRL_INT_RAW.u32 = 0xffffffff;
	}

	if (pkt_int1) {
		if (pkt_int1 & MIPI_PKT_HEADER_ERR) {
			mipi_err_int_cnt[devno].err_ecc_double_cnt++;
		}

		if (pkt_int1 & MIPI_VC0_PKT_DATA_CRC) {
			mipi_err_int_cnt[devno].vc0_err_crc_cnt++;
		}

		if (pkt_int1 & MIPI_VC1_PKT_DATA_CRC) {
			mipi_err_int_cnt[devno].vc1_err_crc_cnt++;
		}

		if (pkt_int1 & MIPI_VC2_PKT_DATA_CRC) {
			mipi_err_int_cnt[devno].vc2_err_crc_cnt++;
		}

		if (pkt_int1 & MIPI_VC3_PKT_DATA_CRC) {
			mipi_err_int_cnt[devno].vc3_err_crc_cnt++;
		}
	}

	if (pkt_int2) {
		if (pkt_int2 & MIPI_VC0_PKT_INVALID_DT) {
			mipi_err_int_cnt[devno].err_id_vc0_cnt++;
		}

		if (pkt_int2 & MIPI_VC1_PKT_INVALID_DT) {
			mipi_err_int_cnt[devno].err_id_vc1_cnt++;
		}

		if (pkt_int2 & MIPI_VC2_PKT_INVALID_DT) {
			mipi_err_int_cnt[devno].err_id_vc2_cnt++;
		}

		if (pkt_int2 & MIPI_VC3_PKT_INVALID_DT) {
			mipi_err_int_cnt[devno].err_id_vc3_cnt++;
		}

		if (pkt_int2 & MIPI_VC0_PKT_HEADER_ECC) {
			mipi_err_int_cnt[devno].vc0_err_ecc_corrected_cnt++;
		}

		if (pkt_int2 & MIPI_VC1_PKT_HEADER_ECC) {
			mipi_err_int_cnt[devno].vc1_err_ecc_corrected_cnt++;
		}

		if (pkt_int2 & MIPI_VC2_PKT_HEADER_ECC) {
			mipi_err_int_cnt[devno].vc2_err_ecc_corrected_cnt++;
		}

		if (pkt_int2 & MIPI_VC3_PKT_HEADER_ECC) {
			mipi_err_int_cnt[devno].vc3_err_ecc_corrected_cnt++;
		}
	}

	if (frame_int) {
		if (frame_int & MIPI_VC0_FRAME_CRC) {
			mipi_err_int_cnt[devno].err_frame_data_vc0_cnt++;
		}

		if (frame_int & MIPI_VC1_FRAME_CRC) {
			mipi_err_int_cnt[devno].err_frame_data_vc1_cnt++;
		}

		if (frame_int & MIPI_VC2_FRAME_CRC) {
			mipi_err_int_cnt[devno].err_frame_data_vc2_cnt++;
		}

		if (frame_int & MIPI_VC3_FRAME_CRC) {
			mipi_err_int_cnt[devno].err_frame_data_vc3_cnt++;
		}

		if (frame_int & MIPI_VC0_ORDER_ERR) {
			mipi_err_int_cnt[devno].err_f_seq_vc0_cnt++;
		}

		if (frame_int & MIPI_VC1_ORDER_ERR) {
			mipi_err_int_cnt[devno].err_f_seq_vc1_cnt++;
		}

		if (frame_int & MIPI_VC2_ORDER_ERR) {
			mipi_err_int_cnt[devno].err_f_seq_vc2_cnt++;
		}

		if (frame_int & MIPI_VC3_ORDER_ERR) {
			mipi_err_int_cnt[devno].err_f_seq_vc3_cnt++;
		}

		if (frame_int & MIPI_VC0_NO_MATCH) {
			mipi_err_int_cnt[devno].err_f_bndry_match_vc0_cnt++;
		}

		if (frame_int & MIPI_VC1_NO_MATCH) {
			mipi_err_int_cnt[devno].err_f_bndry_match_vc1_cnt++;
		}

		if (frame_int & MIPI_VC2_NO_MATCH) {
			mipi_err_int_cnt[devno].err_f_bndry_match_vc2_cnt++;
		}

		if (frame_int & MIPI_VC3_NO_MATCH) {
			mipi_err_int_cnt[devno].err_f_bndry_match_vc3_cnt++;
		}
	}

	if (mipi_ctrl_int) {
		if (mipi_ctrl_int & CMD_FIFO_READ_ERR) {
			mipi_err_int_cnt[devno].cmd_fifo_rderr_cnt++;
		}

		if (mipi_ctrl_int & DATA_FIFO_READ_ERR) {
			mipi_err_int_cnt[devno].data_fifo_rderr_cnt++;
		}

		if (mipi_ctrl_int & CMD_FIFO_WRITE_ERR) {
			mipi_err_int_cnt[devno].cmd_fifo_wrerr_cnt++;
		}

		if (mipi_ctrl_int & DATA_FIFO_WRITE_ERR) {
			mipi_err_int_cnt[devno].data_fifo_wrerr_cnt++;
		}
	}
}

static void lvds_int_statics(combo_dev_t devno)
{
	unsigned int lvds_ctrl_int;
	volatile lvds_ctrl_regs_t *lvds_ctrl_regs = get_lvds_ctrl_regs(devno);

	lvds_ctrl_int = lvds_ctrl_regs->LVDS_CTRL_INT.u32;

	if (lvds_ctrl_int) {
		lvds_ctrl_regs->LVDS_CTRL_INT_RAW.u32 = 0xffffffff;
	}

	if (lvds_ctrl_int & CMD_RD_ERR) {
		lvds_err_int_cnt[devno].cmd_rd_err_cnt++;
	}

	if (lvds_ctrl_int & CMD_WR_ERR) {
		lvds_err_int_cnt[devno].cmd_wr_err_cnt++;
	}

	if (lvds_ctrl_int & LVDS_POP_ERR) {
		lvds_err_int_cnt[devno].pop_err_cnt++;
	}

	if (lvds_ctrl_int & LVDS_STAT_ERR) {
		lvds_err_int_cnt[devno].lvds_state_err_cnt++;
	}

	if (lvds_ctrl_int & LINK0_READ_ERR) {
		lvds_err_int_cnt[devno].link0_rd_err_cnt++;
	}

	if (lvds_ctrl_int & LINK0_WRITE_ERR) {
		lvds_err_int_cnt[devno].link0_wr_err_cnt++;
	}
}

static void align_int_statis(combo_dev_t devno)
{
	unsigned int align_int;
	volatile lvds_ctrl_regs_t *lvds_ctrl_regs = get_lvds_ctrl_regs(devno);

	align_int = lvds_ctrl_regs->ALIGN_INT.u32;

	if (align_int) {
		lvds_ctrl_regs->ALIGN_INT_RAW.u32 = 0xffffffff;
	}

	if (align_int & ALIGN_FIFO_FULL_ERR) {
		align_err_int_cnt[devno].fifo_full_err_cnt++;
	}

	if (align_int & ALIGN_LANE0_ERR) {
		align_err_int_cnt[devno].lane0_align_err_cnt++;
	}

	if (align_int & ALIGN_LANE1_ERR) {
		align_err_int_cnt[devno].lane1_align_err_cnt++;
	}

	if (align_int & ALIGN_LANE2_ERR) {
		align_err_int_cnt[devno].lane2_align_err_cnt++;
	}

	if (align_int & ALIGN_LANE3_ERR) {
		align_err_int_cnt[devno].lane3_align_err_cnt++;
	}
}
static int mipi_rx_interrupt_route(int irq, void *dev_id)
{
	volatile mipi_rx_sys_regs_t *mipi_rx_sys_regs = get_mipi_rx_sys_regs();
	volatile lvds_ctrl_regs_t *lvds_ctrl_regs;
	int i = 0;

	for (i = 0; i < MIPI_RX_MAX_PHY_NUM; i++) {
		mipi_rx_phy_cil_int_statis(i);
	}

	for (i = 0; i < MIPI_RX_MAX_DEV_NUM; i++) {
		lvds_ctrl_regs = get_lvds_ctrl_regs(i);
		if (lvds_ctrl_regs->CHN_INT_RAW.u32) {
			mipi_rx_drv_core_reset(i);
			mipi_rx_drv_core_unreset(i);
		} else {
			continue;
		}
		mipi_int_statics(i);
		lvds_int_statics(i);
		align_int_statis(i);
		lvds_ctrl_regs->CHN_INT_RAW.u32 = 0xf;
	}
	mipi_rx_sys_regs->MIPI_INT_RAW.u32 = 0xff;
	return OSAL_IRQ_HANDLED;
}

static int mipi_rx_drv_reg_init(void)
{
	if (!mipi_rx_regs_va) {
		mipi_rx_regs_va = (mipi_rx_regs_type_t *)osal_ioremap(
			MIPI_RX_REGS_ADDR, (unsigned int)MIPI_RX_REGS_SIZE);
		if (mipi_rx_regs_va == NULL) {
			HI_ERR("remap mipi_rx reg addr fail\n");
			return -1;
		}
		regMapFlag = 1;
	}

	return 0;
}

static void mipi_rx_drv_reg_exit(void)
{
	if (regMapFlag == 1) {
		osal_iounmap((void *)mipi_rx_regs_va);
		regMapFlag = 0;
	}
}

static int mipi_rx_register_irq(void)
{
	int ret;

	ret = osal_request_irq(mipi_rx_irq_num, mipi_rx_interrupt_route, NULL,
			       "MIPI_RX", mipi_rx_interrupt_route);
	if (ret < 0) {
		HI_ERR("mipi_rx: failed to register irq.\n");
		return -1;
	}

	return 0;
}

static void mipi_rx_unregister_irq(void)
{
	osal_free_irq(mipi_rx_irq_num, mipi_rx_interrupt_route);
}

static void mipi_rx_drv_hw_init(void)
{
	void *mipi_rx_crg_addr;
	int i;

	mipi_rx_crg_addr = osal_ioremap(MIPI_RX_CRG_ADDR, (unsigned long)0x4);

	write_reg32(mipi_rx_crg_addr, 1 << 0, 0x1 << 0);
	write_reg32(mipi_rx_crg_addr, 1 << 3, 0x1 << 3);
	/* reset */
	write_reg32(mipi_rx_crg_addr, 1 << 6, 0x1 << 6); /* mipi_bus_srst_req */
	osal_udelay(10);
	write_reg32(mipi_rx_crg_addr, 0, 0x1 << 6);
	osal_iounmap((void *)mipi_rx_crg_addr);

	for (i = 0; i < MIPI_RX_MAX_PHY_NUM; ++i) {
		mipi_rx_set_cil_int_mask(i, MIPI_CIL_INT_MASK);
		mipi_rx_set_phy_skew_link(i, SKEW_LINK);
		mipi_rx_set_phy_fsmo_link(i, MIPI_FSMO_VALUE);
	}
}
static void mipi_rx_drv_hw_exit(void)
{
	void *mipi_rx_crg_addr;
	mipi_rx_crg_addr = osal_ioremap(MIPI_RX_CRG_ADDR, (unsigned long)0x4);
	write_reg32(mipi_rx_crg_addr, 1 << 6, 0x1 << 6);
	write_reg32(mipi_rx_crg_addr, 0, 0x1 << 0);
	write_reg32(mipi_rx_crg_addr, 0, 0x1 << 3);

	osal_iounmap((void *)mipi_rx_crg_addr);
}

int mipi_rx_drv_init(void)
{
	int ret;

	ret = mipi_rx_drv_reg_init();
	if (ret < 0) {
		HI_ERR("mipi_rx_drv_reg_init fail!\n");
		goto fail0;
	}

	ret = mipi_rx_register_irq();
	if (ret < 0) {
		HI_ERR("mipi_rx_register_irq fail!\n");
		goto fail1;
	}

	mipi_rx_core_reset_addr =
		osal_ioremap(MIPI_RX_CRG_ADDR, (unsigned long)0x4);
	if (mipi_rx_core_reset_addr == NULL) {
		HI_ERR("mipi_rx reset ioremap failed!\n");
		goto fail2;
	}
	mipi_rx_drv_hw_init();

	return 0;

fail2:
	mipi_rx_unregister_irq();
fail1:
	mipi_rx_drv_reg_exit();
fail0:
	return -1;
}

void mipi_rx_drv_exit(void)
{
	mipi_rx_unregister_irq();
	mipi_rx_drv_reg_exit();
	mipi_rx_drv_hw_exit();
	osal_iounmap(mipi_rx_core_reset_addr);
}

#ifdef __cplusplus
#if __cplusplus
}

#endif
#endif /* End of #ifdef __cplusplus */
