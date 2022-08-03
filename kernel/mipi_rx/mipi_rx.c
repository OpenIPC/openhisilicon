/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2016-2019. All rights reserved.
 * Description: mipi_rx.c
 * Author:
 * Create: 2016-10-07
 */

#include <linux/types.h>
#include "osal.h"
#include "type.h"
#include "hi_mipi.h"
#include "mipi_rx_hal.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/*
 * macro definition
 */
#define MIPI_RX_DEV_NAME "mipi"
#define MIPI_RX_PROC_NAME "mipi_rx"

#define HIMEDIA_DYNAMIC_MINOR 255

#define COMBO_DEV_MAX_NUM 1
#define COMBO_MAX_LANE_NUM 4

#define COMBO_MIN_WIDTH 32
#define COMBO_MIN_HEIGHT 32
#define ENABLE_INT_MASK

/*
 * global variables definition
 */
typedef struct {
	lane_divide_mode_t lane_divide_mode;
	unsigned char hs_mode_cfged;
	combo_dev_attr_t combo_dev_attr[COMBO_DEV_MAX_NUM];
	unsigned char dev_valid[COMBO_DEV_MAX_NUM];
	unsigned char dev_cfged[COMBO_DEV_MAX_NUM];
	unsigned int lane_bitmap[COMBO_DEV_MAX_NUM];
} mipi_dev_ctx_t;

static osal_dev_t *mipi_rx_dev = NULL;

static osal_mutex_t mipi_mutex;

static mipi_dev_ctx_t mipi_dev_ctx;
static osal_spinlock_t mipi_ctx_spinlock;

extern phy_err_int_cnt_t phy_err_int_cnt[MIPI_RX_MAX_PHY_NUM];
extern mipi_err_int_cnt_t mipi_err_int_cnt[MIPI_RX_MAX_DEV_NUM];
extern lvds_err_int_cnt_t lvds_err_int_cnt[MIPI_RX_MAX_DEV_NUM];
extern align_err_int_cnt_t align_err_int_cnt[MIPI_RX_MAX_DEV_NUM];

/*
 * function definition
 */
unsigned char mipi_is_hs_mode_cfged(void)
{
	unsigned char hs_mode_cfged;

	osal_spin_lock(&mipi_ctx_spinlock);
	hs_mode_cfged = mipi_dev_ctx.hs_mode_cfged;
	osal_spin_unlock(&mipi_ctx_spinlock);

	return hs_mode_cfged;
}

static unsigned char mipi_is_dev_valid(combo_dev_t devno)
{
	unsigned char dev_valid;

	osal_spin_lock(&mipi_ctx_spinlock);
	dev_valid = mipi_dev_ctx.dev_valid[devno];
	osal_spin_unlock(&mipi_ctx_spinlock);

	return dev_valid;
}

static unsigned char mipi_is_dev_cfged(combo_dev_t devno)
{
	unsigned char dev_cfged;

	osal_spin_lock(&mipi_ctx_spinlock);
	dev_cfged = mipi_dev_ctx.dev_cfged[devno];
	osal_spin_unlock(&mipi_ctx_spinlock);

	return dev_cfged;
}

static int check_lane_id(combo_dev_t devno, input_mode_t input_mode,
			 const short p_lane_id[])
{
	int lane_num;
	int all_lane_id_invalid_flag = 1;
	int i, j;
	lane_divide_mode_t cur_lane_divide_mode;

	if (input_mode == INPUT_MODE_MIPI) {
		lane_num = MIPI_LANE_NUM;
	} else if (input_mode == INPUT_MODE_LVDS) {
		lane_num = LVDS_LANE_NUM;
	} else {
		return 0;
	}

	osal_spin_lock(&mipi_ctx_spinlock);
	cur_lane_divide_mode = mipi_dev_ctx.lane_divide_mode;
	osal_spin_unlock(&mipi_ctx_spinlock);

	for (i = 0; i < lane_num; i++) {
		int temp_id = p_lane_id[i];
		int lane_valid;

		if (temp_id < -1 || temp_id >= COMBO_MAX_LANE_NUM) {
			HI_ERR("lane_id[%d] is invalid value %d.\n", i,
			       temp_id);
			return -1;
		}

		if (-1 == temp_id) {
			continue;
		}

		all_lane_id_invalid_flag = 0;

		for (j = i + 1; j < lane_num; j++) {
			if (temp_id == p_lane_id[j]) {
				HI_ERR("lane_id[%d] can't be same value %d as lane_id[%d]\n",
				       i, temp_id, j);
				return -1;
			}
		}

		lane_valid = mipi_rx_drv_is_lane_valid(devno, temp_id,
						       cur_lane_divide_mode);

		if (!lane_valid) {
			HI_ERR("lane_id[%d] %d is invalid in hs_mode %d\n", i,
			       temp_id, cur_lane_divide_mode);
			return -1;
		}
	}

	if (all_lane_id_invalid_flag) {
		HI_ERR("all lane_id is invalid!\n");
		return -1;
	}

	return 0;
}

static unsigned int mipi_get_lane_bitmap(input_mode_t input_mode,
					 const short *p_lane_id,
					 unsigned int *p_total_lane_num)
{
	unsigned int lane_bitmap = 0;
	int lane_num;
	int total_lane_num;
	int i;

	if (input_mode == INPUT_MODE_MIPI) {
		lane_num = MIPI_LANE_NUM;
	} else {
		lane_num = LVDS_LANE_NUM;
	}

	total_lane_num = 0;

	for (i = 0; i < lane_num; i++) {
		short tmp_lane_id;
		tmp_lane_id = p_lane_id[i];

		if (-1 != tmp_lane_id) {
			lane_bitmap = lane_bitmap |
				      1 << (unsigned short)(tmp_lane_id);
			total_lane_num++;
		}
	}

	*p_total_lane_num = total_lane_num;

	return lane_bitmap;
}

static int mipi_check_comb_dev_attr(combo_dev_attr_t *p_attr)
{
	if (p_attr->devno >= COMBO_DEV_MAX_NUM) {
		HI_ERR("invalid combo_dev number(%d).\n", p_attr->devno);
		return -1;
	}

	if (p_attr->input_mode < INPUT_MODE_MIPI ||
	    p_attr->input_mode >= INPUT_MODE_BUTT) {
		HI_ERR("invalid input_mode(%d).\n", p_attr->input_mode);
		return -1;
	}

	if (p_attr->data_rate != MIPI_DATA_RATE_X1) {
		HI_ERR("invalid data_rate(%d).\n", p_attr->data_rate);
		return -1;
	}

	if (p_attr->img_rect.x < 0 || p_attr->img_rect.y < 0) {
		HI_ERR("crop x and y (%d, %d) must be great than 0\n",
		       p_attr->img_rect.x, p_attr->img_rect.y);
		return -1;
	}

	if (p_attr->img_rect.width < COMBO_MIN_WIDTH ||
	    p_attr->img_rect.height < COMBO_MIN_HEIGHT) {
		HI_ERR("invalid img_size(%d, %d), can't be smaller than (%d, %d)\n",
		       p_attr->img_rect.width, p_attr->img_rect.height,
		       COMBO_MIN_WIDTH, COMBO_MIN_HEIGHT);
		return -1;
	}

	if (p_attr->img_rect.width % 2 != 0 ||
	    p_attr->img_rect.height % 2 != 0) {
		HI_ERR("img_size(%d, %d) must be even value\n",
		       p_attr->img_rect.width, p_attr->img_rect.height);
		return -1;
	}

	return 0;
}

static int check_lvds_wdr_mode(lvds_dev_attr_t *p_attr)
{
	int ret = 0;

	switch (p_attr->wdr_mode) {
	case HI_WDR_MODE_2F:
	case HI_WDR_MODE_3F:
	case HI_WDR_MODE_4F: {
		if (p_attr->vsync_attr.sync_type != LVDS_VSYNC_NORMAL &&
		    p_attr->vsync_attr.sync_type != LVDS_VSYNC_SHARE) {
			HI_ERR("invalid sync_type, must be LVDS_VSYNC_NORMAL or LVDS_VSYNC_SHARE\n");
			ret = -1;
		}
		break;
	}

	case HI_WDR_MODE_DOL_2F:
	case HI_WDR_MODE_DOL_3F:
	case HI_WDR_MODE_DOL_4F: {
		if (p_attr->vsync_attr.sync_type == LVDS_VSYNC_NORMAL) {
			if (p_attr->fid_attr.fid_type != LVDS_FID_IN_SAV &&
			    p_attr->fid_attr.fid_type != LVDS_FID_IN_DATA) {
				HI_ERR("invalid fid_type, must be LVDS_FID_IN_SAV or LVDS_FID_IN_DATA\n");
				ret = -1;
			}
		} else if (p_attr->vsync_attr.sync_type ==
			   LVDS_VSYNC_HCONNECT) {
			if (p_attr->fid_attr.fid_type != LVDS_FID_NONE &&
			    p_attr->fid_attr.fid_type != LVDS_FID_IN_DATA) {
				HI_ERR("invalid fid_type, must be LVDS_FID_NONE or LVDS_FID_IN_DATA\n");
				ret = -1;
			}
		} else {
			HI_ERR("invalid sync_type, must be LVDS_VSYNC_NORMAL or LVDS_VSYNC_HCONNECT\n");
			ret = -1;
		}
		break;
	}

	default:
		break;
	}

	return ret;
}

static int check_lvds_dev_attr(combo_dev_t devno, lvds_dev_attr_t *p_attr)
{
	int ret;

	if ((p_attr->input_data_type < DATA_TYPE_RAW_8BIT) ||
	    (p_attr->input_data_type > DATA_TYPE_RAW_16BIT)) {
		HI_ERR("invalid data_type, must be in [%d, %d]\n",
		       DATA_TYPE_RAW_8BIT, DATA_TYPE_RAW_16BIT);
		return -1;
	}

	if ((p_attr->wdr_mode < HI_WDR_MODE_NONE) ||
	    (p_attr->wdr_mode >= HI_WDR_MODE_BUTT)) {
		HI_ERR("invalid wdr_mode, must be in [%d, %d)\n",
		       HI_WDR_MODE_NONE, HI_WDR_MODE_BUTT);
		return -1;
	}

	if ((p_attr->sync_mode < LVDS_SYNC_MODE_SOF) ||
	    (p_attr->sync_mode >= LVDS_SYNC_MODE_BUTT)) {
		HI_ERR("invalid sync_mode, must be in [%d, %d)\n",
		       LVDS_SYNC_MODE_SOF, LVDS_SYNC_MODE_BUTT);
		return -1;
	}

	if (p_attr->vsync_attr.sync_type < LVDS_VSYNC_NORMAL ||
	    p_attr->vsync_attr.sync_type >= LVDS_VSYNC_BUTT) {
		HI_ERR("invalid vsync_code, must be in [%d, %d)\n",
		       LVDS_VSYNC_NORMAL, LVDS_VSYNC_BUTT);
		return -1;
	}

	if (p_attr->fid_attr.fid_type < LVDS_FID_NONE ||
	    p_attr->fid_attr.fid_type >= LVDS_FID_BUTT) {
		HI_ERR("invalid fid_type, must be in [%d, %d)\n", LVDS_FID_NONE,
		       LVDS_FID_BUTT);
		return -1;
	}

	if (p_attr->fid_attr.output_fil != TRUE &&
	    p_attr->fid_attr.output_fil != FALSE) {
		HI_ERR("invalid output_fil, must be HI_TURE or FALSE\n");
		return -1;
	}

	if ((p_attr->data_endian < LVDS_ENDIAN_LITTLE) ||
	    (p_attr->data_endian >= LVDS_ENDIAN_BUTT)) {
		HI_ERR("invalid lvds_bit_endian, must be in [%d, %d)\n",
		       LVDS_ENDIAN_LITTLE, LVDS_ENDIAN_BUTT);
		return -1;
	}

	if ((p_attr->sync_code_endian < LVDS_ENDIAN_LITTLE) ||
	    (p_attr->sync_code_endian >= LVDS_ENDIAN_BUTT)) {
		HI_ERR("invalid lvds_bit_endian, must be in [%d, %d)\n",
		       LVDS_ENDIAN_LITTLE, LVDS_ENDIAN_BUTT);
		return -1;
	}

	ret = check_lvds_wdr_mode(p_attr);
	if (ret < 0) {
		HI_ERR("check_lvds_wdr_mode failed!\n");
		return -1;
	}

	ret = check_lane_id(devno, INPUT_MODE_LVDS, p_attr->lane_id);
	if (ret < 0) {
		HI_ERR("check_lane_id failed!\n");
		return -1;
	}

	return 0;
}

static void mipi_set_lvds_sync_codes(combo_dev_t devno, lvds_dev_attr_t *p_attr,
				     unsigned int total_lane_num,
				     unsigned int lane_bitmap)
{
	int i, j, k;
	unsigned short(*p_nxt_sync_code)[WDR_VC_NUM][SYNC_CODE_NUM] = NULL;
	int alloc_flag = FALSE;

	if (p_attr->wdr_mode >= HI_WDR_MODE_DOL_2F &&
	    p_attr->wdr_mode <= HI_WDR_MODE_DOL_4F) {
		/* Sony DOL Mode */
		p_nxt_sync_code =
			osal_vmalloc(sizeof(unsigned short) * total_lane_num *
				     WDR_VC_NUM * SYNC_CODE_NUM);
		if (p_nxt_sync_code == NULL) {
			HI_ERR("alloc memory for nxt_sync_code failed\n");
			return;
		}

		alloc_flag = TRUE;

		/* SONY DOL N frame and N+1 Frame has the diffrent sync_code
        * N+1 Frame FSET is 1, FSET is the 10th bit
        */
		for (i = 0; i < LVDS_LANE_NUM; i++) {
			for (j = 0; j < WDR_VC_NUM; j++) {
				for (k = 0; k < SYNC_CODE_NUM; k++) {
					if (p_attr->input_data_type ==
					    DATA_TYPE_RAW_10BIT) {
						p_nxt_sync_code[i][j][k] =
							p_attr->sync_code[i][j]
									 [k] +
							(1 << 8);
					} else {
						p_nxt_sync_code[i][j][k] =
							p_attr->sync_code[i][j]
									 [k] +
							(1 << 10);
					}
				}
			}
		}

		/* Set Dony DOL Line Information */
		if (p_attr->fid_attr.fid_type == LVDS_FID_IN_DATA) {
			mipi_rx_drv_set_dol_line_information(devno,
							     p_attr->wdr_mode);
		}
	} else {
		p_nxt_sync_code = p_attr->sync_code;
	}

	/* LVDS_CTRL Sync code */
	mipi_rx_drv_set_lvds_sync_code(devno, total_lane_num, p_attr->lane_id,
				       p_attr->sync_code);
	mipi_rx_drv_set_lvds_nxt_sync_code(devno, total_lane_num,
					   p_attr->lane_id, p_nxt_sync_code);

	/* PHY Sync code detect setting */
	mipi_rx_drv_set_phy_sync_config(p_attr, lane_bitmap, p_nxt_sync_code);

	if (alloc_flag == TRUE) {
		osal_vfree(p_nxt_sync_code);
	}
}

static int mipi_set_lvds_dev_attr(combo_dev_attr_t *p_combo_dev_attr)
{
	combo_dev_t devno;
	lvds_dev_attr_t *p_lvds_attr;
	int ret;
	unsigned int lane_bitmap, lane_num;

	devno = p_combo_dev_attr->devno;
	p_lvds_attr = &p_combo_dev_attr->lvds_attr;

	if (mipi_is_hs_mode_cfged() != TRUE) {
		HI_ERR("mipi must set hs mode before set lvds attr!\n");
		return -1;
	}

	if (!mipi_is_dev_valid(devno)) {
		HI_ERR("invalid combo dev num after set hs mode!\n");
		return -1;
	}

	ret = check_lvds_dev_attr(devno, p_lvds_attr);
	if (ret < 0) {
		HI_ERR("check_lvds_dev_attr failed!\n");
		return -1;
	}

	lane_bitmap = mipi_get_lane_bitmap(INPUT_MODE_LVDS,
					   p_lvds_attr->lane_id, &lane_num);

	/* work mode */
	mipi_rx_drv_set_work_mode(devno, INPUT_MODE_LVDS);

	/* image crop */
	mipi_rx_drv_set_lvds_image_rect(devno, &p_combo_dev_attr->img_rect,
					lane_num);
	mipi_rx_drv_set_lvds_crop_en(devno, TRUE);

	/* data type & mode */
	ret = mipi_rx_drv_set_lvds_wdr_mode(devno, p_lvds_attr->wdr_mode,
					    &p_lvds_attr->vsync_attr,
					    &p_lvds_attr->fid_attr);
	if (ret < 0) {
		HI_ERR("set lvds wdr mode failed!\n");
		return -1;
	}

	mipi_rx_drv_set_lvds_ctrl_mode(devno, p_lvds_attr->sync_mode,
				       p_lvds_attr->input_data_type,
				       p_lvds_attr->data_endian,
				       p_lvds_attr->sync_code_endian);

	/* data rate */
	mipi_rx_drv_set_lvds_data_rate(devno, p_combo_dev_attr->data_rate);

	/* phy lane config */
	mipi_rx_drv_set_link_lane_id(devno, INPUT_MODE_LVDS,
				     p_lvds_attr->lane_id);
	mipi_rx_drv_set_mem_cken(devno, TRUE);
	mipi_rx_drv_set_clr_cken(devno, TRUE);

	mipi_rx_drv_set_phy_config(INPUT_MODE_LVDS, lane_bitmap);

	/* sync codes */
	mipi_set_lvds_sync_codes(devno, p_lvds_attr, lane_num, lane_bitmap);

	mipi_rx_set_phy_rg_lp0_mode_en(devno, 0);
	mipi_rx_set_phy_rg_lp1_mode_en(devno, 0);
	osal_memset(&lvds_err_int_cnt[devno], 0, sizeof(lvds_err_int_cnt_t));
	osal_memset(&align_err_int_cnt[devno], 0, sizeof(align_err_int_cnt_t));
#ifdef ENABLE_INT_MASK
	mipi_rx_drv_set_mipi_int_mask(devno);
	mipi_rx_drv_set_lvds_ctrl_int_mask(devno, LVDS_CTRL_INT_MASK);
	mipi_rx_drv_set_align_int_mask(devno, ALIGN0_INT_MASK);
#endif
	return 0;
}

static int check_mipi_dev_attr(combo_dev_t devno, mipi_dev_attr_t *p_attr)
{
	int ret;
	int i;

	if ((p_attr->input_data_type < DATA_TYPE_RAW_8BIT) ||
	    (p_attr->input_data_type >= DATA_TYPE_BUTT)) {
		HI_ERR("invalid input_data_type, must be in [%d, %d)\n",
		       DATA_TYPE_RAW_8BIT, DATA_TYPE_BUTT);
		return -1;
	}

	if ((p_attr->wdr_mode < HI_MIPI_WDR_MODE_NONE) ||
	    (p_attr->wdr_mode >= HI_MIPI_WDR_MODE_BUTT)) {
		HI_ERR("invalid wdr_mode, must be in [%d, %d)\n",
		       HI_MIPI_WDR_MODE_NONE, HI_MIPI_WDR_MODE_BUTT);
		return -1;
	}

	if ((p_attr->wdr_mode != HI_MIPI_WDR_MODE_NONE) &&
	    (p_attr->input_data_type >= DATA_TYPE_YUV420_8BIT_NORMAL)) {
		HI_ERR("It do not support wdr mode when input_data_type is yuv format!\n");
		return -1;
	}

	if (p_attr->wdr_mode == HI_MIPI_WDR_MODE_DT) {
		for (i = 0; i < WDR_VC_NUM; i++) {
			/* data_type must be the CSI-2 reserve Type [0x38, 0x3f] */
			if (p_attr->data_type[i] < 0x38 ||
			    p_attr->data_type[i] > 0x3f) {
				HI_ERR("invalid data_type[%d]: %d, must be in [0x38, 0x3f]\n",
				       i, p_attr->data_type[i]);
				return -1;
			}
		}
	}

	ret = check_lane_id(devno, INPUT_MODE_MIPI, p_attr->lane_id);
	if (ret < 0) {
		HI_ERR("check_lane_id failed!\n");
		return -1;
	}

	return 0;
}

static void mipi_set_dt_and_mode(combo_dev_t devno, mipi_dev_attr_t *p_attr)
{
	data_type_t input_data_type;

	input_data_type = p_attr->input_data_type;

	mipi_rx_drv_set_di_dt(devno, input_data_type);

	mipi_rx_drv_set_mipi_yuv_dt(devno, input_data_type);

	if (p_attr->wdr_mode == HI_MIPI_WDR_MODE_DT) {
		mipi_rx_drv_set_mipi_user_dt(devno, input_data_type,
					     p_attr->data_type);
	} else if (p_attr->wdr_mode == HI_MIPI_WDR_MODE_DOL) {
		mipi_rx_drv_set_mipi_dol_id(devno, input_data_type, NULL);
	}

	mipi_rx_drv_set_mipi_wdr_mode(devno, p_attr->wdr_mode);
}

extern void vi_mipi_rx_mux(void);

static int mipi_set_mipi_dev_attr(combo_dev_attr_t *p_combo_dev_attr)
{
	combo_dev_t devno;
	mipi_dev_attr_t *p_mipi_attr;
	unsigned int lane_bitmap;
	unsigned int lane_num;
	int ret;

	devno = p_combo_dev_attr->devno;
	p_mipi_attr = &p_combo_dev_attr->mipi_attr;

	if (mipi_is_hs_mode_cfged() != TRUE) {
		HI_ERR("mipi must set hs mode before set mipi attr!\n");
		return -1;
	}

	if (!mipi_is_dev_valid(devno)) {
		HI_ERR("invalid combo dev num after set hs mode!\n");
		return -1;
	}

	ret = check_mipi_dev_attr(devno, p_mipi_attr);
	if (ret < 0) {
		HI_ERR("check_mipi_dev_attr failed!\n");
		return -1;
	}

	/* work mode */
	mipi_rx_drv_set_work_mode(devno, INPUT_MODE_MIPI);

	/* image crop */
	mipi_rx_drv_set_mipi_image_rect(devno, &p_combo_dev_attr->img_rect);
	mipi_rx_drv_set_mipi_crop_en(devno, TRUE);

	/* data type & mode */
	mipi_set_dt_and_mode(devno, p_mipi_attr);

	/* data rate */
	mipi_rx_drv_set_data_rate(devno, p_combo_dev_attr->data_rate);

	/* phy lane config */
	mipi_rx_drv_set_link_lane_id(devno, INPUT_MODE_MIPI,
				     p_mipi_attr->lane_id);

	lane_bitmap = mipi_get_lane_bitmap(INPUT_MODE_MIPI,
					   p_mipi_attr->lane_id, &lane_num);
	mipi_dev_ctx.lane_bitmap[devno] = lane_bitmap;
	mipi_rx_drv_set_mem_cken(devno, TRUE);
	mipi_rx_drv_set_lane_num(devno, lane_num);

	mipi_rx_drv_set_phy_config(INPUT_MODE_MIPI, lane_bitmap);

	/* interrupt mask */
	osal_memset(&mipi_err_int_cnt[devno], 0, sizeof(mipi_err_int_cnt_t));
	osal_memset(&align_err_int_cnt[devno], 0, sizeof(align_err_int_cnt_t));

#ifdef ENABLE_INT_MASK
	mipi_rx_drv_set_mipi_int_mask(devno);
	mipi_rx_drv_set_mipi_ctrl_int_mask(devno,
					   MIPI_CTRL_INT_MASK); /* 0x12f8 */
	mipi_rx_drv_set_mipi_pkt1_int_mask(devno,
					   MIPI_PKT_INT1_MASK); /* 0x1064 */
	mipi_rx_drv_set_mipi_pkt2_int_mask(devno,
					   MIPI_PKT_INT2_MASK); /* 0x1074 */
	mipi_rx_drv_set_mipi_frame_int_mask(devno,
					    MIPI_FRAME_INT_MASK); /* 0x1084 */
	mipi_rx_drv_set_align_int_mask(devno, ALIGN0_INT_MASK); /* 0x18f8 */
#endif

	return 0;
}

static int mipi_set_cmos_dev_attr(combo_dev_attr_t *p_combo_dev_attr)
{
	combo_dev_t devno;
	unsigned int lane_bitmap = 0xf;
	unsigned int phy_id = 0;

	devno = p_combo_dev_attr->devno;

	if (devno > CMOS_MAX_DEV_NUM) {
		HI_ERR("invalid cmos devno(%d)!\n", devno);
		return -1;
	}

	if (p_combo_dev_attr->input_mode != INPUT_MODE_BT656) {
		mipi_rx_drv_set_cmos_en(phy_id, 1);
		mipi_rx_drv_set_phy_en(lane_bitmap);
		mipi_rx_drv_set_phy_cfg_en(lane_bitmap, 1);
		mipi_rx_drv_set_lane_en(lane_bitmap);
		mipi_rx_drv_set_phy_cil_en(lane_bitmap, 1);
		mipi_rx_drv_set_phy_cfg_mode(
			INPUT_MODE_CMOS,
			lane_bitmap); /* BT1120 may not need 698~700 */

		mipi_rx_set_phy_rg_lp0_mode_en(phy_id, 0);
		mipi_rx_set_phy_rg_lp1_mode_en(phy_id, 0);
	}

	return 0;
}

static int mipi_set_combo_dev_attr(combo_dev_attr_t *p_attr)
{
	int ret;

	ret = mipi_check_comb_dev_attr(p_attr);
	if (ret < 0) {
		HI_ERR("mipi check combo_dev attr failed!\n");
		return -1;
	}

	if (osal_mutex_lock_interruptible(&mipi_mutex)) {
		return -ERESTARTSYS;
	}

	switch (p_attr->input_mode) {
	case INPUT_MODE_LVDS:
	case INPUT_MODE_SUBLVDS:
	case INPUT_MODE_HISPI: {
		ret = mipi_set_lvds_dev_attr(p_attr);
		if (ret < 0) {
			HI_ERR("mipi set lvds attr failed!\n");
			ret = -1;
		}
		/* mipi pinmux  ,here only for hi3516ev200,hi3516ev300,hi3518ev300,hi3516dv200       */
		vi_mipi_rx_mux();
		break;
	}

	case INPUT_MODE_MIPI: {
		ret = mipi_set_mipi_dev_attr(p_attr);
		if (ret < 0) {
			HI_ERR("mipi set mipi attr failed!\n");
			ret = -1;
		}
		/* mipi pinmux  ,here only for hi3516ev200,hi3516ev300,hi3518ev300,hi3516dv200 */
		vi_mipi_rx_mux();
		break;
	}

	case INPUT_MODE_CMOS:
	case INPUT_MODE_BT601:
	case INPUT_MODE_BT656:
	case INPUT_MODE_BT1120: {
		ret = mipi_set_cmos_dev_attr(p_attr);
		if (ret < 0) {
			HI_ERR("mipi set cmos attr failed!\n");
			ret = -1;
		}
		break;
	}
	default: {
		HI_ERR("invaild input mode\n");
		ret = -1;
		break;
	}
	}

	osal_mutex_unlock(&mipi_mutex);

	return ret;
}

static int mipi_set_phy_cmvmode(combo_dev_t devno, phy_cmv_mode_t cmv_mode)
{
	input_mode_t input_mode;
	unsigned int lane_bit_map;
	if (devno >= COMBO_DEV_MAX_NUM) {
		HI_ERR("invalid mipi dev number(%d).\n", devno);
		return -1;
	}

	if (cmv_mode < PHY_CMV_GE1200MV || cmv_mode >= PHY_CMV_BUTT) {
		HI_ERR("invalid common mode voltage mode: %d, must be int [%d, %d)\n",
		       cmv_mode, PHY_CMV_GE1200MV, PHY_CMV_BUTT);
		return -1;
	}

	if (osal_mutex_lock_interruptible(&mipi_mutex)) {
		return -ERESTARTSYS;
	}

	if (!mipi_is_dev_cfged(devno)) {
		osal_mutex_unlock(&mipi_mutex);
		HI_ERR("MIPI device %d has not beed configured\n", devno);
		return -1;
	}
	osal_spin_lock(&mipi_ctx_spinlock);
	input_mode = mipi_dev_ctx.combo_dev_attr[devno].input_mode;
	lane_bit_map = mipi_dev_ctx.lane_bitmap[devno];
	osal_spin_unlock(&mipi_ctx_spinlock);
	if (input_mode != INPUT_MODE_MIPI && input_mode != INPUT_MODE_SUBLVDS &&
	    input_mode != INPUT_MODE_LVDS && input_mode != INPUT_MODE_HISPI) {
		osal_mutex_unlock(&mipi_mutex);
		HI_ERR("devno: %d, input mode: %d, not support set common voltage mode\n",
		       devno, input_mode);
		return -1;
	}
	mipi_rx_drv_set_phy_cmvmode(input_mode, cmv_mode, lane_bit_map);

	osal_mutex_unlock(&mipi_mutex);

	return 0;
}

static int mipi_reset_sensor(sns_rst_source_t sns_reset_source)
{
	if (sns_reset_source >= SNS_MAX_RST_SOURCE_NUM) {
		HI_ERR("invalid sns_reset_source(%d).\n", sns_reset_source);
		return -1;
	}

	sensor_drv_reset(sns_reset_source);

	return 0;
}

static int mipi_unreset_sensor(sns_rst_source_t sns_reset_source)
{
	if (sns_reset_source >= SNS_MAX_RST_SOURCE_NUM) {
		HI_ERR("invalid sns_reset_source(%d).\n", sns_reset_source);
		return -1;
	}

	sensor_drv_unreset(sns_reset_source);

	return 0;
}

static int mipi_reset_mipi_rx(combo_dev_t combo_dev)
{
	if (combo_dev >= MIPI_RX_MAX_DEV_NUM) {
		HI_ERR("invalid combo_dev num(%d).\n", combo_dev);
		return -1;
	}

	mipi_rx_drv_core_reset(combo_dev);

	return 0;
}

static int mipi_unreset_mipi_rx(combo_dev_t combo_dev)
{
	if (combo_dev >= MIPI_RX_MAX_DEV_NUM) {
		HI_ERR("invalid combo_dev num(%d).\n", combo_dev);
		return -1;
	}

	mipi_rx_drv_core_unreset(combo_dev);

	return 0;
}

static void mipi_set_dev_valid(lane_divide_mode_t mode)
{
	switch (mode) {
	case LANE_DIVIDE_MODE_0:
		mipi_dev_ctx.dev_valid[0] = 1;
		break;
	default:
		break;
	}
}

static int mipi_set_hs_mode(lane_divide_mode_t lane_divide_mode)
{
	if ((lane_divide_mode < LANE_DIVIDE_MODE_0) ||
	    (lane_divide_mode >= LANE_DIVIDE_MODE_BUTT)) {
		HI_ERR("invalid lane_divide_mode(%d), must be in [%d, %d)\n",
		       lane_divide_mode, LANE_DIVIDE_MODE_0,
		       LANE_DIVIDE_MODE_BUTT);
		return -1;
	}

	mipi_rx_drv_set_hs_mode(lane_divide_mode);

	osal_spin_lock(&mipi_ctx_spinlock);
	mipi_dev_ctx.lane_divide_mode = lane_divide_mode;
	mipi_dev_ctx.hs_mode_cfged = TRUE;
	osal_memset(mipi_dev_ctx.dev_valid, 0, sizeof(mipi_dev_ctx.dev_valid));
	mipi_set_dev_valid(lane_divide_mode);
	osal_spin_unlock(&mipi_ctx_spinlock);

	return 0;
}

static int mipi_enable_mipi_rx_clock(combo_dev_t combo_dev)
{
	if (combo_dev >= MIPI_RX_MAX_DEV_NUM) {
		HI_ERR("invalid combo_dev num(%d).\n", combo_dev);
		return -1;
	}

	mipi_rx_drv_enable_clock(combo_dev);

	return 0;
}

static int mipi_disable_mipi_rx_clock(combo_dev_t combo_dev)
{
	if (combo_dev >= MIPI_RX_MAX_DEV_NUM) {
		HI_ERR("invalid combo_dev num(%d).\n", combo_dev);
		return -1;
	}

	mipi_rx_drv_disable_clock(combo_dev);

	return 0;
}

static int mipi_enable_sensor_clock(sns_clk_source_t sns_clk_source)
{
	if (sns_clk_source >= SNS_MAX_CLK_SOURCE_NUM) {
		HI_ERR("invalid sns_clk_source(%d).\n", sns_clk_source);
		return -1;
	}

	sensor_drv_enable_clock(sns_clk_source);

	return 0;
}

static int mipi_disable_sensor_clock(sns_clk_source_t sns_clk_source)
{
	if (sns_clk_source >= SNS_MAX_CLK_SOURCE_NUM) {
		HI_ERR("invalid sns_clk_source(%d).\n", sns_clk_source);
		return -1;
	}

	sensor_drv_disable_clock(sns_clk_source);

	return 0;
}

static long mipi_rx_ioctl(unsigned int cmd, unsigned long arg,
			  void *private_data)
{
	unsigned int *argp = (unsigned int *)(uintptr_t)arg;
	int ret;

	MIPIRX_CHECK_NULL_PTR(argp);

	switch (cmd) {
	case HI_MIPI_SET_DEV_ATTR: {
		combo_dev_attr_t *pstcombo_dev_attr = NULL;
		combo_dev_t devno;

		pstcombo_dev_attr = (combo_dev_attr_t *)argp;

		ret = mipi_set_combo_dev_attr(pstcombo_dev_attr);
		if (ret < 0) {
			HI_ERR("mipi set combo_dev attr failed!\n");
			ret = -1;
		} else {
			devno = pstcombo_dev_attr->devno;
			osal_spin_lock(&mipi_ctx_spinlock);
			mipi_dev_ctx.dev_cfged[devno] = TRUE;
			osal_memcpy(&mipi_dev_ctx.combo_dev_attr[devno],
				    pstcombo_dev_attr,
				    sizeof(combo_dev_attr_t));
			osal_spin_unlock(&mipi_ctx_spinlock);
		}
		break;
	}

	case HI_MIPI_SET_PHY_CMVMODE: {
		phy_cmv_t *p_phy_cmv;

		p_phy_cmv = (phy_cmv_t *)argp;

		ret = mipi_set_phy_cmvmode(p_phy_cmv->devno,
					   p_phy_cmv->cmv_mode);
		if (ret < 0) {
			HI_ERR("mipi set phy cmv mode failed!\n");
			ret = -1;
		}
		break;
	}

	case HI_MIPI_RESET_SENSOR: {
		sns_rst_source_t sns_reset_source;

		osal_memcpy(&sns_reset_source, argp, sizeof(sns_rst_source_t));
		ret = mipi_reset_sensor(sns_reset_source);
		if (ret < 0) {
			HI_ERR("mipi reset sensor failed!\n");
			ret = -1;
		}
		break;
	}

	case HI_MIPI_UNRESET_SENSOR: {
		sns_rst_source_t sns_reset_source;

		osal_memcpy(&sns_reset_source, argp, sizeof(sns_rst_source_t));
		ret = mipi_unreset_sensor(sns_reset_source);
		if (ret < 0) {
			HI_ERR("mipi unreset sensor failed!\n");
			ret = -1;
		}
		break;
	}

	case HI_MIPI_RESET_MIPI: {
		combo_dev_t combo_dev;

		osal_memcpy(&combo_dev, argp, sizeof(combo_dev_t));
		ret = mipi_reset_mipi_rx(combo_dev);
		if (ret < 0) {
			HI_ERR("mipi reset mipi_rx failed!\n");
			ret = -1;
		}
		break;
	}

	case HI_MIPI_UNRESET_MIPI: {
		combo_dev_t combo_dev;

		osal_memcpy(&combo_dev, argp, sizeof(combo_dev_t));
		ret = mipi_unreset_mipi_rx(combo_dev);
		if (ret < 0) {
			HI_ERR("mipi unreset mipi_rx failed!\n");
			ret = -1;
		}
		break;
	}

	case HI_MIPI_RESET_SLVS: {
		HI_ERR("Do not support SLVS, please use mipi/lvds mode.\n");
		ret = -1;
		break;
	}

	case HI_MIPI_UNRESET_SLVS: {
		HI_ERR("Do not support SLVS, please use mipi/lvds mode.\n");
		ret = -1;
		break;
	}

	case HI_MIPI_SET_HS_MODE: {
		lane_divide_mode_t lane_divide_mode;

		osal_memcpy(&lane_divide_mode, argp,
			    sizeof(lane_divide_mode_t));
		ret = mipi_set_hs_mode(lane_divide_mode);
		if (ret < 0) {
			HI_ERR("mipi set hs mode failed!\n");
			ret = -1;
		}
		break;
	}

	case HI_MIPI_ENABLE_MIPI_CLOCK: {
		combo_dev_t combo_dev;

		osal_memcpy(&combo_dev, argp, sizeof(combo_dev_t));
		ret = mipi_enable_mipi_rx_clock(combo_dev);
		if (ret < 0) {
			HI_ERR("mipi enable mipi_rx clock failed!\n");
			ret = -1;
		}
		break;
	}

	case HI_MIPI_DISABLE_MIPI_CLOCK: {
		combo_dev_t combo_dev;

		osal_memcpy(&combo_dev, argp, sizeof(combo_dev_t));
		ret = mipi_disable_mipi_rx_clock(combo_dev);
		if (ret < 0) {
			HI_ERR("mipi disable mipi_rx clock failed!\n");
			ret = -1;
		}
		break;
	}

	case HI_MIPI_ENABLE_SLVS_CLOCK: {
		HI_ERR("Do not support SLVS, please use mipi/lvds mode.\n");
		ret = -1;
		break;
	}

	case HI_MIPI_DISABLE_SLVS_CLOCK: {
		HI_ERR("Do not support SLVS, please use mipi/lvds mode.\n");
		ret = -1;
		break;
	}

	case HI_MIPI_ENABLE_SENSOR_CLOCK: {
		sns_clk_source_t sns_clk_source;

		osal_memcpy(&sns_clk_source, argp, sizeof(sns_clk_source_t));
		ret = mipi_enable_sensor_clock(sns_clk_source);
		if (ret < 0) {
			HI_ERR("mipi enable sensor clock failed!\n");
			ret = -1;
		}
		break;
	}

	case HI_MIPI_DISABLE_SENSOR_CLOCK: {
		sns_clk_source_t sns_clk_source;

		osal_memcpy(&sns_clk_source, argp, sizeof(sns_clk_source_t));
		ret = mipi_disable_sensor_clock(sns_clk_source);
		if (ret < 0) {
			HI_ERR("mipi disable sensor clock failed!\n");
			ret = -1;
		}
		break;
	}

	default: {
		HI_ERR("invalid himipi ioctl cmd\n");
		ret = -1;
		break;
	}
	}

	return ret;
}

#ifdef CONFIG_COMPAT
static long mipi_rx_compat_ioctl(unsigned int cmd, unsigned long arg,
				 void *private_data)
{
	switch (cmd) {
	default: {
		break;
	}
	}

	return mipi_rx_ioctl(cmd, arg, private_data);
}
#endif

static const char *mipi_get_intput_mode_name(input_mode_t input_mode)
{
	switch (input_mode) {
	case INPUT_MODE_SUBLVDS:
	case INPUT_MODE_LVDS:
	case INPUT_MODE_HISPI:
		return "LVDS";

	case INPUT_MODE_MIPI:
		return "MIPI";

	case INPUT_MODE_CMOS:
	case INPUT_MODE_BT1120:
	case INPUT_MODE_BT656:
	case INPUT_MODE_BYPASS:
		return "CMOS";

	default:
		break;
	}

	return "N/A";
}

static const char *mipi_get_raw_data_type_name(data_type_t type)
{
	switch (type) {
	case DATA_TYPE_RAW_8BIT:
		return "RAW8";

	case DATA_TYPE_RAW_10BIT:
		return "RAW10";

	case DATA_TYPE_RAW_12BIT:
		return "RAW12";

	case DATA_TYPE_RAW_14BIT:
		return "RAW14";

	case DATA_TYPE_RAW_16BIT:
		return "RAW16";

	case DATA_TYPE_YUV420_8BIT_NORMAL:
		return "yuv420_8bit_normal";

	case DATA_TYPE_YUV420_8BIT_LEGACY:
		return "yuv420_8bit_legacy";

	case DATA_TYPE_YUV422_8BIT:
		return "yuv422_8bit";

	default:
		break;
	}

	return "N/A";
}

static const char *mipi_get_data_rate_name(mipi_data_rate_t data_rate)
{
	switch (data_rate) {
	case MIPI_DATA_RATE_X1:
		return "X1";

	case MIPI_DATA_RATE_X2:
		return "X2";

	default:
		break;
	}

	return "N/A";
}
static const char *mipi_print_mipi_wdr_mode(mipi_wdr_mode_t wdr_mode)
{
	switch (wdr_mode) {
	case HI_MIPI_WDR_MODE_NONE:
		return "None";

	case HI_MIPI_WDR_MODE_VC:
		return "VC";

	case HI_MIPI_WDR_MODE_DT:
		return "DT";

	case HI_MIPI_WDR_MODE_DOL:
		return "DOL";

	default:
		break;
	}

	return "N/A";
}

static const char *mipi_print_lvds_wdr_mode(wdr_mode_t wdr_mode)
{
	switch (wdr_mode) {
	case HI_WDR_MODE_NONE:
		return "None";

	case HI_WDR_MODE_2F:
		return "2F";

	case HI_WDR_MODE_3F:
		return "3F";

	case HI_WDR_MODE_4F:
		return "4F";

	case HI_WDR_MODE_DOL_2F:
		return "DOL_2F";

	case HI_WDR_MODE_DOL_3F:
		return "DOL_3F";

	case HI_WDR_MODE_DOL_4F:
		return "DOL_4F";

	default:
		break;
	}

	return "N/A";
}

static const char *mipi_print_lane_divide_mode(lane_divide_mode_t mode)
{
	switch (mode) {
	case LANE_DIVIDE_MODE_0:
		return "4";
	default:
		break;
	}

	return "N/A";
}

static void proc_show_mipi_device(osal_proc_entry_t *s)
{
	const char *wdr_mode;
	combo_dev_t devno;
	input_mode_t input_mode;
	data_type_t data_type = DATA_TYPE_BUTT;

	osal_seq_printf(
		s,
		"\n-----MIPI DEV ATTR---------------------"
		"--------------------------------------------------------------------------------\n");

	osal_seq_printf(s,
			"%8s"
			"%10s"
			"%10s"
			"%20s"
			"%10s"
			"%8s"
			"%8s"
			"%8s"
			"%8s"
			"\n",
			"Devno", "WorkMode", "DataRate", "DataType", "WDRMode",
			"ImgX", "ImgY", "ImgW", "ImgH");

	for (devno = 0; devno < MIPI_RX_MAX_DEV_NUM; devno++) {
		combo_dev_attr_t *pdev_attr =
			&mipi_dev_ctx.combo_dev_attr[devno];

		input_mode = pdev_attr->input_mode;

		if (!mipi_is_dev_cfged(devno)) {
			continue;
		}
		if (input_mode == INPUT_MODE_MIPI) {
			data_type = pdev_attr->mipi_attr.input_data_type;
			wdr_mode = mipi_print_mipi_wdr_mode(
				pdev_attr->mipi_attr.wdr_mode);
		} else {
			data_type = pdev_attr->lvds_attr.input_data_type;
			wdr_mode = mipi_print_lvds_wdr_mode(
				pdev_attr->lvds_attr.wdr_mode);
		}

		osal_seq_printf(s,
				"%8d"
				"%10s"
				"%10s"
				"%20s"
				"%10s"
				"%8d"
				"%8d"
				"%8d"
				"%8d"
				"\n",
				devno, mipi_get_intput_mode_name(input_mode),
				mipi_get_data_rate_name(pdev_attr->data_rate),
				mipi_get_raw_data_type_name(data_type),
				wdr_mode, pdev_attr->img_rect.x,
				pdev_attr->img_rect.y,
				pdev_attr->img_rect.width,
				pdev_attr->img_rect.height);
	}
}

static void proc_show_mipi_lane(osal_proc_entry_t *s)
{
	combo_dev_t devno;
	input_mode_t input_mode;

	osal_seq_printf(
		s,
		"\n-----MIPI LANE INFO---------------------"
		"--------------------------------------------------------------------------------\n");

	osal_seq_printf(s,
			"%8s"
			"%24s"
			"\n",
			"Devno", "LaneID");

	for (devno = 0; devno < MIPI_RX_MAX_DEV_NUM; devno++) {
		combo_dev_attr_t *pdev_attr =
			&mipi_dev_ctx.combo_dev_attr[devno];

		input_mode = pdev_attr->input_mode;

		if (!mipi_is_dev_cfged(devno)) {
			continue;
		}

		if (input_mode == INPUT_MODE_MIPI) {
			osal_seq_printf(s,
					"%8d"
					"%10d,%3d,%3d,%3d"
					"\n",
					devno, pdev_attr->mipi_attr.lane_id[0],
					pdev_attr->mipi_attr.lane_id[1],
					pdev_attr->mipi_attr.lane_id[2],
					pdev_attr->mipi_attr.lane_id[3]);
		} else if (input_mode == INPUT_MODE_LVDS ||
			   input_mode == INPUT_MODE_SUBLVDS ||
			   input_mode == INPUT_MODE_HISPI) {
			osal_seq_printf(s,
					"%8d"
					"%10d,%3d,%3d,%3d"
					"\n",
					devno, pdev_attr->lvds_attr.lane_id[0],
					pdev_attr->lvds_attr.lane_id[1],
					pdev_attr->lvds_attr.lane_id[2],
					pdev_attr->lvds_attr.lane_id[3]);
		}
	}
}

static void proc_show_mipi_phy_data(osal_proc_entry_t *s)
{
	int i;

	osal_seq_printf(
		s,
		"\n-----MIPI PHY DATA INFO------------------------------------------------------\n");
	osal_seq_printf(s,
			"%8s"
			"%15s"
			"%19s"
			"%24s"
			"%22s"
			"\n",
			"PhyId", "LaneId", "PhyData", "MipiData", "LvdsData");

	for (i = 0; i < MIPI_RX_MAX_PHY_NUM; i++) {
		osal_seq_printf(
			s,
			"%8d%8d,%2d,%2d,%2d    0x%02x,0x%02x,0x%02x,0x%02x    "
			"0x%02x,0x%02x,0x%02x,0x%02x    0x%02x,0x%02x,0x%02x,0x%02x\n",
			i, 4 * i, 4 * i + 1, 4 * i + 2, 4 * i + 3,
			mipi_rx_drv_get_phy_data(i, 0),
			mipi_rx_drv_get_phy_data(i, 1),
			mipi_rx_drv_get_phy_data(i, 2),
			mipi_rx_drv_get_phy_data(i, 3),
			mipi_rx_drv_get_phy_mipi_link_data(i, 0),
			mipi_rx_drv_get_phy_mipi_link_data(i, 1),
			mipi_rx_drv_get_phy_mipi_link_data(i, 2),
			mipi_rx_drv_get_phy_mipi_link_data(i, 3),
			mipi_rx_drv_get_phy_lvds_link_data(i, 0),
			mipi_rx_drv_get_phy_lvds_link_data(i, 1),
			mipi_rx_drv_get_phy_lvds_link_data(i, 2),
			mipi_rx_drv_get_phy_lvds_link_data(i, 3));
	}
}
static void proc_show_mipi_detect_info(osal_proc_entry_t *s,
				       combo_dev_t devno_array[], int mipi_cnt)
{
	img_size_t image_size;
	short vc_num;
	int devno_idx;

	osal_seq_printf(
		s,
		"\n-----MIPI DETECT INFO----------------------------------------------------\n");
	osal_seq_printf(s, "%6s%3s%8s%8s\n", "Devno", "VC", "width", "height");

	for (devno_idx = 0; devno_idx < mipi_cnt; devno_idx++) {
		for (vc_num = 0; vc_num < WDR_VC_NUM; vc_num++) {
			mipi_rx_drv_get_mipi_imgsize_statis(
				devno_array[devno_idx], vc_num, &image_size);
			osal_seq_printf(s, "%6d%3d%8d%8d\n",
					devno_array[devno_idx], vc_num,
					image_size.width, image_size.height);
		}
	}
}

static void proc_show_lvds_detect_info(osal_proc_entry_t *s,
				       combo_dev_t devno_array[], int mipi_cnt)
{
	img_size_t image_size;
	short vc_num;
	int devno_idx;

	osal_seq_printf(
		s,
		"\n-----LVDS DETECT INFO----------------------------------------------------\n");
	osal_seq_printf(s, "%6s%3s%8s%8s\n", "Devno", "VC", "width", "height");

	for (devno_idx = 0; devno_idx < mipi_cnt; devno_idx++) {
		for (vc_num = 0; vc_num < WDR_VC_NUM; vc_num++) {
			mipi_rx_drv_get_lvds_imgsize_statis(
				devno_array[devno_idx], vc_num, &image_size);
			osal_seq_printf(s, "%6d%3d%8d%8d\n",
					devno_array[devno_idx], vc_num,
					image_size.width, image_size.height);
		}
	}
}

static void proc_show_lvds_lane_detect_info(osal_proc_entry_t *s,
					    combo_dev_t devno_array[],
					    int mipi_cnt)
{
	img_size_t image_size;
	short lane;
	int devno_idx;
	combo_dev_attr_t *pstcombo_dev_attr = NULL;

	osal_seq_printf(
		s,
		"\n-----LVDS LANE DETECT INFO----------------------------------------------------\n");
	osal_seq_printf(s, "%6s%6s%8s%8s\n", "Devno", "Lane", "width",
			"height");

	for (devno_idx = 0; devno_idx < mipi_cnt; devno_idx++) {
		pstcombo_dev_attr =
			&mipi_dev_ctx.combo_dev_attr[devno_array[devno_idx]];

		for (lane = 0; lane < LVDS_LANE_NUM; lane++) {
			if (-1 != pstcombo_dev_attr->lvds_attr.lane_id[lane]) {
				mipi_rx_drv_get_lvds_lane_imgsize_statis(
					devno_array[devno_idx], lane,
					&image_size);
				osal_seq_printf(s, "%6d%6d%8d%8d\n",
						devno_array[devno_idx],
						pstcombo_dev_attr->lvds_attr
							.lane_id[lane],
						image_size.width,
						image_size.height);
			}
		}
	}
}

static void proc_show_mipi_hs_mode(osal_proc_entry_t *s)
{
	lane_divide_mode_t lane_divide_mode;

	osal_spin_lock(&mipi_ctx_spinlock);
	lane_divide_mode = mipi_dev_ctx.lane_divide_mode;
	osal_spin_unlock(&mipi_ctx_spinlock);

	osal_seq_printf(
		s,
		"\n-----MIPI LANE DIVIDE MODE------------------------"
		"---------------------------------------------------------------------\n");
	osal_seq_printf(s,
			"%6s"
			"%20s"
			"\n",
			"MODE", "LANE DIVIDE");
	osal_seq_printf(s,
			"%6d"
			"%20s"
			"\n",
			lane_divide_mode,
			mipi_print_lane_divide_mode(lane_divide_mode));
}

static void proc_show_phy_cil_int_err_cnt(osal_proc_entry_t *s)
{
	int phy_id;

	osal_seq_printf(
		s,
		"\n-----PHY CIL ERR INT INFO---------------------------------------------\n");
	osal_seq_printf(s,
			"%8s%11s%10s%12s%12s%12s%12s%9s%8s%10s%10s%10s%10s\n",
			"PhyId", "Clk2TmOut", "ClkTmOut", "Lane0TmOut",
			"Lane1TmOut", "Lane2TmOut", "Lane3TmOut", "Clk2Esc",
			"ClkEsc", "Lane0Esc", "Lane1Esc", "Lane2Esc",
			"Lane3Esc");

	for (phy_id = 0; phy_id < MIPI_RX_MAX_PHY_NUM; phy_id++) {
		osal_seq_printf(
			s,
			"%8d%11d%10d%12d%12d%12d%12d%9d%8d%10d%10d%10d%10d\n",
			phy_id,
			phy_err_int_cnt[phy_id].clk1_fsm_timeout_err_cnt,
			phy_err_int_cnt[phy_id].clk0_fsm_timeout_err_cnt,
			phy_err_int_cnt[phy_id].d0_fsm_timeout_err_cnt,
			phy_err_int_cnt[phy_id].d1_fsm_timeout_err_cnt,
			phy_err_int_cnt[phy_id].d2_fsm_timeout_err_cnt,
			phy_err_int_cnt[phy_id].d3_fsm_timeout_err_cnt,

			phy_err_int_cnt[phy_id].clk1_fsm_escape_err_cnt,
			phy_err_int_cnt[phy_id].clk0_fsm_escape_err_cnt,
			phy_err_int_cnt[phy_id].d0_fsm_escape_err_cnt,
			phy_err_int_cnt[phy_id].d1_fsm_escape_err_cnt,
			phy_err_int_cnt[phy_id].d2_fsm_escape_err_cnt,
			phy_err_int_cnt[phy_id].d3_fsm_escape_err_cnt);
	}
}

static void proc_show_mipi_int_err(osal_proc_entry_t *s,
				   combo_dev_t devno_array[], int mipi_cnt)
{
	int devno_idx;
	combo_dev_t devno;

	osal_seq_printf(
		s,
		"\n-----MIPI ERROR INT INFO 1-----------------------------------------------------------\n");
	osal_seq_printf(s, "%8s%6s%8s%8s%8s%8s%14s%14s%14s%14s\n", "Devno",
			"Ecc2", "Vc0CRC", "Vc1CRC", "Vc2CRC", "Vc3CRC",
			"Vc0EccCorrct", "Vc1EccCorrct", "Vc2EccCorrct",
			"Vc3EccCorrct");

	for (devno_idx = 0; devno_idx < mipi_cnt; devno_idx++) {
		devno = devno_array[devno_idx];
		osal_seq_printf(
			s, "%8d%6d%8d%8d%8d%8d%14d%14d%14d%14d\n", devno,
			mipi_err_int_cnt[devno].err_ecc_double_cnt,
			mipi_err_int_cnt[devno].vc0_err_crc_cnt,
			mipi_err_int_cnt[devno].vc1_err_crc_cnt,
			mipi_err_int_cnt[devno].vc2_err_crc_cnt,
			mipi_err_int_cnt[devno].vc3_err_crc_cnt,
			mipi_err_int_cnt[devno].vc0_err_ecc_corrected_cnt,
			mipi_err_int_cnt[devno].vc1_err_ecc_corrected_cnt,
			mipi_err_int_cnt[devno].vc2_err_ecc_corrected_cnt,
			mipi_err_int_cnt[devno].vc3_err_ecc_corrected_cnt);
	}

	osal_seq_printf(
		s,
		"\n-----MIPI ERROR INT INFO 2-----------------------------------------------------------\n");
	osal_seq_printf(s, "%8s%7s%7s%7s%7s%11s%11s%11s%11s\n", "Devno",
			"Vc0Dt", "Vc1Dt", "Vc2Dt", "Vc3Dt", "Vc0FrmCrc",
			"Vc1FrmCrc", "Vc2FrmCrc", "Vc3FrmCrc");

	for (devno_idx = 0; devno_idx < mipi_cnt; devno_idx++) {
		devno = devno_array[devno_idx];
		osal_seq_printf(s, "%8d%7d%7d%7d%7d%11d%11d%11d%11d\n", devno,
				mipi_err_int_cnt[devno].err_id_vc0_cnt,
				mipi_err_int_cnt[devno].err_id_vc1_cnt,
				mipi_err_int_cnt[devno].err_id_vc2_cnt,
				mipi_err_int_cnt[devno].err_id_vc3_cnt,
				mipi_err_int_cnt[devno].err_frame_data_vc0_cnt,
				mipi_err_int_cnt[devno].err_frame_data_vc1_cnt,
				mipi_err_int_cnt[devno].err_frame_data_vc2_cnt,
				mipi_err_int_cnt[devno].err_frame_data_vc3_cnt);
	}

	osal_seq_printf(
		s,
		"\n-----MIPI ERROR INT INFO 3-----------------------------------------------------------\n");
	osal_seq_printf(s, "%8s%11s%11s%11s%11s%12s%12s%12s%12s\n", "Devno",
			"Vc0FrmSeq", "Vc1FrmSeq", "Vc2FrmSeq", "Vc3FrmSeq",
			"Vc0BndryMt", "Vc1BndryMt", "Vc2BndryMt", "Vc3BndryMt");

	for (devno_idx = 0; devno_idx < mipi_cnt; devno_idx++) {
		devno = devno_array[devno_idx];
		osal_seq_printf(
			s, "%8d%11d%11d%11d%11d%12d%12d%12d%12d\n", devno,
			mipi_err_int_cnt[devno].err_f_seq_vc0_cnt,
			mipi_err_int_cnt[devno].err_f_seq_vc1_cnt,
			mipi_err_int_cnt[devno].err_f_seq_vc2_cnt,
			mipi_err_int_cnt[devno].err_f_seq_vc3_cnt,
			mipi_err_int_cnt[devno].err_f_bndry_match_vc0_cnt,
			mipi_err_int_cnt[devno].err_f_bndry_match_vc1_cnt,
			mipi_err_int_cnt[devno].err_f_bndry_match_vc2_cnt,
			mipi_err_int_cnt[devno].err_f_bndry_match_vc3_cnt);
	}

	osal_seq_printf(
		s,
		"\n-----MIPI ERROR INT INFO 4-----------------------------------------------------------\n");
	osal_seq_printf(s, "%8s%15s%14s%14s%15s\n", "Devno", "DataFifoRdErr",
			"CmdFifoRdErr", "CmdFifoWrErr", "DataFifoWrErr");

	for (devno_idx = 0; devno_idx < mipi_cnt; devno_idx++) {
		devno = devno_array[devno_idx];
		osal_seq_printf(s, "%8d%15d%14d%14d%15d\n", devno,
				mipi_err_int_cnt[devno].data_fifo_rderr_cnt,
				mipi_err_int_cnt[devno].cmd_fifo_rderr_cnt,
				mipi_err_int_cnt[devno].data_fifo_wrerr_cnt,
				mipi_err_int_cnt[devno].cmd_fifo_wrerr_cnt);
	}
}

static void proc_show_lvds_int_err(osal_proc_entry_t *s,
				   combo_dev_t devno_array[], int lvds_cnt)
{
	combo_dev_t devno;
	int devno_idx;

	osal_seq_printf(
		s,
		"\n-----LVDS ERROR INT INFO -----------------------------------------------------------\n");
	osal_seq_printf(s, "%8s%10s%10s%8s%9s%12s%12s\n", "Devno", "CmdRdErr",
			"CmdWrErr", "PopErr", "StatErr", "Link0WrErr",
			"Link0RdErr");

	for (devno_idx = 0; devno_idx < lvds_cnt; devno_idx++) {
		devno = devno_array[devno_idx];
		osal_seq_printf(s, "%8d%10d%10d%8d%9d%12d%12d\n", devno,
				lvds_err_int_cnt[devno].cmd_rd_err_cnt,
				lvds_err_int_cnt[devno].cmd_wr_err_cnt,
				lvds_err_int_cnt[devno].pop_err_cnt,
				lvds_err_int_cnt[devno].lvds_state_err_cnt,

				lvds_err_int_cnt[devno].link0_rd_err_cnt,
				lvds_err_int_cnt[devno].link0_wr_err_cnt);
	}
}

static void proc_show_align_int_err(osal_proc_entry_t *s)
{
	combo_dev_t devno;

	osal_seq_printf(
		s,
		"\n-----ALIGN ERROR INT INFO--------------------------------------\n");

	for (devno = 0; devno < MIPI_RX_MAX_DEV_NUM; devno++) {
		if (!mipi_is_dev_cfged(devno)) {
			continue;
		}

		osal_seq_printf(s, "%8s%14s%10s%10s%10s%10s\n", "Devno",
				"FIFO_FullErr", "Lane0Err", "Lane1Err",
				"Lane2Err", "Lane3Err");

		osal_seq_printf(s, "%8d%14d%10d%10d%10d%10d\n", devno,
				align_err_int_cnt[devno].fifo_full_err_cnt,
				align_err_int_cnt[devno].lane0_align_err_cnt,
				align_err_int_cnt[devno].lane1_align_err_cnt,
				align_err_int_cnt[devno].lane2_align_err_cnt,
				align_err_int_cnt[devno].lane3_align_err_cnt);
	}
}
static int mipi_proc_show(osal_proc_entry_t *s)
{
	combo_dev_t devno;
	int mipi_cnt = 0;
	int lvds_cnt = 0;
	input_mode_t input_mode;
	combo_dev_attr_t *pdev_attr;
	combo_dev_t devno_mipi[MIPI_RX_MAX_DEV_NUM] = { 0 };
	combo_dev_t devno_lvds[MIPI_RX_MAX_DEV_NUM] = { 0 };

	for (devno = 0; devno < MIPI_RX_MAX_DEV_NUM; devno++) {
		if (!mipi_is_dev_cfged(devno)) {
			continue;
		}

		osal_spin_lock(&mipi_ctx_spinlock);
		pdev_attr = &mipi_dev_ctx.combo_dev_attr[devno];
		input_mode = pdev_attr->input_mode;
		osal_spin_unlock(&mipi_ctx_spinlock);

		if (input_mode == INPUT_MODE_MIPI) {
			devno_mipi[mipi_cnt] = devno;
			mipi_cnt++;
		} else if ((input_mode == INPUT_MODE_LVDS) ||
			   (input_mode == INPUT_MODE_SUBLVDS) ||
			   (input_mode == INPUT_MODE_HISPI)) {
			devno_lvds[lvds_cnt] = devno;
			lvds_cnt++;
		}
	}

	if (mipi_cnt > 0 || lvds_cnt > 0) {
		proc_show_mipi_hs_mode(s);
		proc_show_mipi_device(s);

		proc_show_mipi_lane(s);
		proc_show_mipi_phy_data(s);
	}

	// MIPI Attr:
	if (mipi_cnt > 0) {
		proc_show_mipi_detect_info(s, devno_mipi, mipi_cnt);
	}

	if (lvds_cnt > 0) {
		proc_show_lvds_detect_info(s, devno_lvds, lvds_cnt);
		proc_show_lvds_lane_detect_info(s, devno_lvds, lvds_cnt);
	}

	if (mipi_cnt > 0 || lvds_cnt > 0) {
		proc_show_phy_cil_int_err_cnt(s);
	}
	if (mipi_cnt > 0) {
		proc_show_mipi_int_err(s, devno_mipi, mipi_cnt);
	}
	if (lvds_cnt > 0) {
		proc_show_lvds_int_err(s, devno_lvds, lvds_cnt);
	}
	if (mipi_cnt > 0 || lvds_cnt > 0) {
		proc_show_align_int_err(s);
	}

	return 0;
}

static int mipi_rx_proc_show(osal_proc_entry_t *s)
{
	osal_seq_printf(s, "\nModule: [MIPI_RX], Build Time[" __DATE__
			   ", " __TIME__ "]\n");

	mipi_proc_show(s);

	return 0;
}

static int mipi_rx_init(void)
{
	int ret;

	osal_mutex_init(&mipi_mutex);
	osal_spin_lock_init(&mipi_ctx_spinlock);

	ret = mipi_rx_drv_init();

	if (ret < 0) {
		HI_ERR("mipi_rx_drv_init fail!\n");
		goto fail0;
	}

	return 0;
fail0:
	return -1;
}

static void mipi_rx_exit(void)
{
	mipi_rx_drv_exit();

	osal_spin_lock_destory(&mipi_ctx_spinlock);
	osal_mutex_destory(&mipi_mutex);
}

static int mipi_rx_open(void *private_data)
{
	return 0;
}

static int mipi_rx_release(void *private_data)
{
	return 0;
}

static osal_fileops_t mipi_rx_fops = {
	.open = mipi_rx_open,
	.release = mipi_rx_release,
	.unlocked_ioctl = mipi_rx_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl = mipi_rx_compat_ioctl,
#endif
};

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
static int mipi_rx_freeze(osal_dev_t *pdev)
{
	return 0;
}

static int mipi_rx_restore(osal_dev_t *pdev)
{
	return 0;
}
#else
static int mipi_rx_freeze(osal_dev_t *pdev)
{
	return 0;
}

static int mipi_rx_restore(osal_dev_t *pdev)
{
	return 0;
}
#endif

static osal_pmops_t mipi_rx_pmops = {
	.pm_freeze = mipi_rx_freeze,
	.pm_restore = mipi_rx_restore,
};

int mipi_rx_mod_init(void)
{
	int ret;
	osal_proc_entry_t *mipi_rx_proc_entry;

	mipi_rx_dev = osal_createdev(MIPI_RX_DEV_NAME);
	if (mipi_rx_dev == NULL) {
		HI_ERR("create mipi_rx device failed!\n");
		goto fail0;
	}

	mipi_rx_dev->fops = &mipi_rx_fops;
	mipi_rx_dev->minor = HIMEDIA_DYNAMIC_MINOR;
	mipi_rx_dev->osal_pmops = &mipi_rx_pmops;

	ret = osal_registerdevice(mipi_rx_dev);
	if (ret < 0) {
		HI_ERR("register mipi_rx device failed!\n");
		goto fail1;
	}

	mipi_rx_proc_entry = osal_create_proc_entry(MIPI_RX_PROC_NAME, NULL);
	if (mipi_rx_proc_entry == NULL) {
		HI_ERR("create mipi_rx proc(%s) failed!\n", MIPI_RX_PROC_NAME);
		goto fail2;
	}

	mipi_rx_proc_entry->read = mipi_rx_proc_show;
	mipi_rx_proc_entry->write = NULL;

	ret = mipi_rx_init();
	if (ret < 0) {
		HI_ERR("mipi_rx_init failed!\n");
		goto fail3;
	}

	osal_printk("load mipi_rx driver successful!\n");
	return 0;

fail3:
	osal_remove_proc_entry(MIPI_RX_PROC_NAME, NULL);
fail2:
	osal_deregisterdevice(mipi_rx_dev);
fail1:
	osal_destroydev(mipi_rx_dev);
	mipi_rx_dev = NULL;
fail0:
	HI_ERR("load mipi_rx driver failed!\n");
	return -1;
}

void mipi_rx_mod_exit(void)
{
	mipi_rx_exit();

	osal_remove_proc_entry(MIPI_RX_PROC_NAME, NULL);

	osal_deregisterdevice(mipi_rx_dev);

	osal_destroydev(mipi_rx_dev);
	mipi_rx_dev = NULL;

	osal_printk("unload mipi_rx driver ok!\n");
}

#ifdef __cplusplus
#if __cplusplus
}

#endif
#endif /* End of #ifdef __cplusplus */
