/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_inner.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "isp_inner.h"
#include "mpi_sys.h"
#include "hi_comm_vi.h"
#include "hi_comm_isp.h"
#include "hi_comm_3a.h"
#include "hi_ae_comm.h"
#include "hi_awb_comm.h"
#include "isp_main.h"
#include "isp_ext_config.h"
#include "isp_math_utils.h"

hi_void isp_calc_grid_info(hi_u16 wdith, hi_u16 start_pox, hi_u16 block_num, hi_u16 *grid_info)
{
    hi_u16 i;
    hi_u16 integer;
    hi_u16 remainder;

    integer   = wdith / div_0_to_1(block_num);
    remainder = wdith % div_0_to_1(block_num);
    grid_info[0] = start_pox;
    for (i = 1; i < block_num; i++) {
        if (remainder > 0) {
            grid_info[i] = grid_info[i - 1] + integer + 1;
            remainder = remainder - 1;
        } else {
            grid_info[i] = grid_info[i - 1] + integer;
        }
    }

    return;
}
hi_u32 isp_get_striping_active_img_start(hi_u8 block_index, isp_working_mode *isp_work_mode)
{
    hi_u32 over_lap;
    hi_u32 block_start;

    over_lap = isp_work_mode->over_lap;
    if (block_index == 0) {
        block_start = isp_work_mode->block_rect[block_index].x;
    } else {
        block_start = isp_work_mode->block_rect[block_index].x + over_lap;
    }

    return block_start;
}

hi_u32 isp_get_striping_active_img_width(hi_u8 block_index, isp_working_mode *isp_work_mode)
{
    hi_u32 block_width;
    hi_u32 over_lap;
    hi_u8  block_num;

    over_lap    = isp_work_mode->over_lap;
    block_width = isp_work_mode->block_rect[block_index].width;
    block_num   = isp_work_mode->block_num;

    if ((block_index == 0) || (block_index == (block_num - 1))) { /* first block and last block */
        block_width = block_width - over_lap;
    } else {
        block_width = block_width - over_lap * 2; /*  overlap * 2 */
    }
    return block_width;
}

hi_u32 isp_get_striping_grid_x_info(hi_u16 *grid_pos, hi_u16 grid_num, isp_working_mode *isp_work_mode)
{
    hi_u8  i;
    hi_u16 start;
    hi_u16 width;
    hi_u16 div_num;
    hi_u16 index = 0;

    for (i = 0; i < isp_work_mode->block_num; i++) {
        start = isp_get_striping_active_img_start(i, isp_work_mode);
        width = isp_get_striping_active_img_width(i, isp_work_mode);

        if (i < grid_num % div_0_to_1(isp_work_mode->block_num)) {
            div_num = grid_num / div_0_to_1(isp_work_mode->block_num) + 1;
        } else {
            div_num = grid_num / div_0_to_1(isp_work_mode->block_num);
        }

        isp_calc_grid_info(width, start, div_num, &(grid_pos[index]));
        index = index + div_num;
    }
    return HI_SUCCESS;
}

hi_void isp_calc_ae_fe_grid_info(hi_vi_pipe vi_pipe, hi_isp_ae_grid_info *fe_grid_info)
{
    hi_bool crop_en;
    hi_u16  img_total_width, img_total_height;
    hi_u16  img_start_x, img_start_y;

    crop_en = hi_ext_system_ae_crop_en_read(vi_pipe);
    if (crop_en == HI_TRUE) {
        img_start_x      = hi_ext_system_ae_crop_x_read(vi_pipe);
        img_start_y      = hi_ext_system_ae_crop_y_read(vi_pipe);
        img_total_width  = hi_ext_system_ae_crop_width_read(vi_pipe);
        img_total_height = hi_ext_system_ae_crop_height_read(vi_pipe);
    } else {
        img_start_x      = 0;
        img_start_y      = 0;
        img_total_width  = hi_ext_sync_total_width_read(vi_pipe);
        img_total_height = hi_ext_sync_total_height_read(vi_pipe);
    }

    isp_calc_grid_info(img_total_width,  img_start_x, AE_ZONE_COLUMN, fe_grid_info->grid_x_pos);
    isp_calc_grid_info(img_total_height, img_start_y, AE_ZONE_ROW, fe_grid_info->grid_y_pos);

    fe_grid_info->grid_x_pos[AE_ZONE_COLUMN] = img_start_x + img_total_width - 1;
    fe_grid_info->grid_y_pos[AE_ZONE_ROW]    = img_start_y + img_total_height - 1;
    fe_grid_info->status = 1;
}

hi_s32  isp_calc_ae_be_grid_info(hi_vi_pipe vi_pipe, hi_isp_ae_grid_info *be_grid_info)
{
    hi_bool crop_en;
    hi_u16 be_width, be_height;
    hi_u16 be_start_x = 0;
    hi_u16 be_start_y = 0;
    isp_working_mode isp_work_mode;

    if (ioctl(isp_get_fd(vi_pipe), ISP_WORK_MODE_GET, &isp_work_mode) != HI_SUCCESS) {
        isp_err_trace("get work mode error!\n");
        return HI_FAILURE;
    }

    crop_en = hi_ext_system_ae_crop_en_read(vi_pipe);
    if ((is_striping_mode(isp_work_mode.running_mode)) ||
        (is_sidebyside_mode(isp_work_mode.running_mode))) {
        isp_get_striping_grid_x_info(be_grid_info->grid_x_pos, AE_ZONE_COLUMN, &isp_work_mode);
        be_start_y = isp_work_mode.block_rect[0].y;
        be_height = isp_work_mode.frame_rect.height;
        isp_calc_grid_info(be_height, be_start_y, AE_ZONE_ROW, be_grid_info->grid_y_pos);
        be_width = isp_work_mode.frame_rect.width;
    } else {
        if (crop_en == HI_TRUE) {
            be_start_x = hi_ext_system_ae_crop_x_read(vi_pipe);
            be_start_y = hi_ext_system_ae_crop_y_read(vi_pipe);
            be_width = hi_ext_system_ae_crop_width_read(vi_pipe);
            be_height = hi_ext_system_ae_crop_height_read(vi_pipe);
        } else {
            be_start_x = 0;
            be_start_y = 0;
            be_width = isp_work_mode.frame_rect.width;
            be_height = isp_work_mode.frame_rect.height;
        }

        isp_calc_grid_info(be_width, be_start_x, AE_ZONE_COLUMN, be_grid_info->grid_x_pos);
        isp_calc_grid_info(be_height, be_start_y, AE_ZONE_ROW, be_grid_info->grid_y_pos);
    }

    be_grid_info->grid_x_pos[AE_ZONE_COLUMN] = be_start_x + be_width - 1;  /* last position */
    be_grid_info->grid_y_pos[AE_ZONE_ROW] = be_start_y + be_height - 1;  /* last position */

    be_grid_info->status = 1;

    return HI_SUCCESS;
}

hi_s32 isp_get_ae_grid_info(hi_vi_pipe vi_pipe, hi_isp_ae_grid_info *fe_grid_info, hi_isp_ae_grid_info *be_grid_info)
{
    hi_s32 ret;
    memset_s(fe_grid_info, sizeof(hi_isp_ae_grid_info), 0, sizeof(hi_isp_ae_grid_info));
    memset_s(be_grid_info, sizeof(hi_isp_ae_grid_info), 0, sizeof(hi_isp_ae_grid_info));

    isp_calc_ae_fe_grid_info(vi_pipe, fe_grid_info);

    ret = isp_calc_ae_be_grid_info(vi_pipe, be_grid_info);

    return ret;
}

hi_s32 isp_get_mg_grid_info(hi_vi_pipe vi_pipe, hi_isp_mg_grid_info *grid_info)
{
    hi_bool crop_en;
    hi_u16  be_width, be_height;
    hi_u16  be_start_x = 0;
    hi_u16  be_start_y = 0;
    isp_working_mode isp_work_mode;

    memset_s(grid_info, sizeof(hi_isp_mg_grid_info), 0, sizeof(hi_isp_mg_grid_info));

    if (ioctl(isp_get_fd(vi_pipe), ISP_WORK_MODE_GET, &isp_work_mode) != HI_SUCCESS) {
        isp_err_trace("get work mode error!\n");
        return HI_FAILURE;
    }

    crop_en = hi_ext_system_ae_crop_en_read(vi_pipe);

    if ((is_striping_mode(isp_work_mode.running_mode)) ||
        (is_sidebyside_mode(isp_work_mode.running_mode))) {
        isp_get_striping_grid_x_info(grid_info->grid_x_pos, MG_ZONE_COLUMN, &isp_work_mode);
        be_start_y   = isp_work_mode.block_rect[0].y;
        be_height   = isp_work_mode.frame_rect.height;
        isp_calc_grid_info(be_height, be_start_y, MG_ZONE_ROW, grid_info->grid_y_pos);
        be_width    = isp_work_mode.frame_rect.width;
    } else {
        if (crop_en == HI_TRUE) {
            be_start_x = hi_ext_system_ae_crop_x_read(vi_pipe);
            be_start_y = hi_ext_system_ae_crop_y_read(vi_pipe);
            be_width  = hi_ext_system_ae_crop_width_read(vi_pipe);
            be_height = hi_ext_system_ae_crop_height_read(vi_pipe);
        } else {
            be_start_x = 0;
            be_start_y = 0;
            be_width  = isp_work_mode.frame_rect.width;
            be_height = isp_work_mode.frame_rect.height;
        }

        isp_calc_grid_info(be_width,  be_start_x, MG_ZONE_COLUMN, grid_info->grid_x_pos);
        isp_calc_grid_info(be_height, be_start_y, MG_ZONE_ROW,    grid_info->grid_y_pos);
    }

    grid_info->grid_x_pos[MG_ZONE_COLUMN] = be_start_x + be_width  - 1; /* last position */
    grid_info->grid_y_pos[MG_ZONE_ROW]    = be_start_y + be_height - 1; /* last position */
    grid_info->status                     = 1;

    return HI_SUCCESS;
}

hi_s32 isp_get_af_grid_info(hi_vi_pipe vi_pipe, hi_isp_focus_grid_info *fe_grid_info,
                            hi_isp_focus_grid_info *be_grid_info)
{
    hi_bool crop_en;
    hi_u16  be_width, be_height;
    hi_u16  be_start_x = 0;
    hi_u16  be_start_y = 0;
    hi_u16  af_x_grid_num, af_y_grid_num;
    isp_working_mode isp_work_mode;

    memset_s(fe_grid_info, sizeof(hi_isp_focus_grid_info), 0, sizeof(hi_isp_focus_grid_info));
    memset_s(be_grid_info, sizeof(hi_isp_focus_grid_info), 0, sizeof(hi_isp_focus_grid_info));

    if (ioctl(isp_get_fd(vi_pipe), ISP_WORK_MODE_GET, &isp_work_mode) != HI_SUCCESS) {
        isp_err_trace("get work mode error!\n");
        return HI_FAILURE;
    }

    crop_en = hi_ext_af_crop_enable_read(vi_pipe);
    af_y_grid_num = hi_ext_af_window_vnum_read(vi_pipe);
    af_x_grid_num = hi_ext_af_window_hnum_read(vi_pipe);

    if ((is_striping_mode(isp_work_mode.running_mode)) ||
        (is_sidebyside_mode(isp_work_mode.running_mode))) {
        isp_get_striping_grid_x_info(be_grid_info->grid_x_pos, af_x_grid_num, &isp_work_mode);
        be_start_y = isp_work_mode.block_rect[0].y;
        be_height = isp_work_mode.frame_rect.height;
        isp_calc_grid_info(be_height, be_start_y, af_y_grid_num, be_grid_info->grid_y_pos);
        be_width  = isp_work_mode.frame_rect.width;
    } else {
        if (crop_en == HI_TRUE) {
            be_start_x = hi_ext_af_crop_pos_x_read(vi_pipe);
            be_start_y = hi_ext_af_crop_pos_y_read(vi_pipe);
            be_width  = hi_ext_af_crop_h_size_read(vi_pipe);
            be_height = hi_ext_af_crop_v_size_read(vi_pipe);
        } else {
            be_start_x = 0;
            be_start_y = 0;
            be_width  = isp_work_mode.frame_rect.width;
            be_height = isp_work_mode.frame_rect.height;
        }

        isp_calc_grid_info(be_width,  be_start_x, af_x_grid_num, be_grid_info->grid_x_pos);
        isp_calc_grid_info(be_height, be_start_y, af_y_grid_num,    be_grid_info->grid_y_pos);
    }

    be_grid_info->grid_x_pos[af_x_grid_num] = be_start_x + be_width  - 1; /* last position */
    be_grid_info->grid_y_pos[af_y_grid_num] = be_start_y + be_height - 1; /* last position */
    be_grid_info->status = 1;

    return HI_SUCCESS;
}

hi_s32 isp_get_wb_grid_info(hi_vi_pipe vi_pipe, hi_isp_awb_grid_info *grid_info)
{
    hi_bool crop_en;
    hi_u16  be_width, be_height;
    hi_u16  be_start_x = 0;
    hi_u16  be_start_y = 0;
    hi_u16  u16awb_x_grid_num, u16awb_y_grid_num;
    isp_working_mode isp_work_mode;

    memset_s(grid_info, sizeof(hi_isp_awb_grid_info), 0, sizeof(hi_isp_awb_grid_info));

    if (ioctl(isp_get_fd(vi_pipe), ISP_WORK_MODE_GET, &isp_work_mode) != HI_SUCCESS) {
        isp_err_trace("get work mode error!\n");
        return HI_FAILURE;
    }

    u16awb_y_grid_num = hi_ext_system_awb_vnum_read(vi_pipe);
    u16awb_x_grid_num = hi_ext_system_awb_hnum_read(vi_pipe);
    crop_en           = hi_ext_system_awb_crop_en_read(vi_pipe);

    if ((is_striping_mode(isp_work_mode.running_mode)) ||
        (is_sidebyside_mode(isp_work_mode.running_mode))) {
        isp_get_striping_grid_x_info(grid_info->grid_x_pos, u16awb_x_grid_num, &isp_work_mode);
        be_start_y = isp_work_mode.block_rect[0].y;
        be_height = isp_work_mode.frame_rect.height;
        isp_calc_grid_info(be_height, be_start_y, u16awb_y_grid_num, grid_info->grid_y_pos);
        be_width    = isp_work_mode.frame_rect.width;
    } else {
        if (crop_en == HI_TRUE) {
            be_start_x = hi_ext_system_awb_crop_x_read(vi_pipe);
            be_start_y = hi_ext_system_awb_crop_y_read(vi_pipe);
            be_width   = hi_ext_system_awb_crop_width_read(vi_pipe);
            be_height  = hi_ext_system_awb_crop_height_read(vi_pipe);
        } else {
            be_start_x = 0;
            be_start_y = 0;
            be_width   = isp_work_mode.frame_rect.width;
            be_height  = isp_work_mode.frame_rect.height;
        }

        isp_calc_grid_info(be_width, be_start_x, u16awb_x_grid_num, grid_info->grid_x_pos);
        isp_calc_grid_info(be_height, be_start_y, u16awb_y_grid_num, grid_info->grid_y_pos);
    }

    grid_info->grid_x_pos[u16awb_x_grid_num] = be_start_x + be_width  - 1; /* last position */
    grid_info->grid_y_pos[u16awb_y_grid_num] = be_start_y + be_height - 1; /* last position */
    grid_info->status                     = 1;

    return HI_SUCCESS;
}

hi_s32 isp_get_ae_stitch_statistics(hi_vi_pipe vi_pipe, hi_isp_ae_stitch_statistics *ae_stitch_stat)
{
    return HI_ERR_ISP_NOT_SUPPORT;
}

hi_s32 isp_get_wb_stitch_statistics(hi_vi_pipe vi_pipe, hi_isp_wb_stitch_statistics *stitch_wb_stat)
{
    return HI_ERR_ISP_NOT_SUPPORT;
}

hi_s32 isp_get_fe_focus_statistics(hi_vi_pipe vi_pipe, hi_isp_fe_focus_statistics *fe_af_stat,
                                   isp_stat *isp_act_stat, hi_u8 wdr_chn)
{
    memset_s(fe_af_stat, sizeof(hi_isp_fe_focus_statistics), 0, sizeof(hi_isp_fe_focus_statistics));

    return HI_SUCCESS;
}

hi_s32 isp_set_radial_shading_attr(hi_vi_pipe vi_pipe, const hi_isp_radial_shading_attr *radial_shading_attr)
{
    isp_err_trace("Not support this interface!\n");
    return HI_ERR_ISP_NOT_SUPPORT;
}

hi_s32 isp_get_radial_shading_attr(hi_vi_pipe vi_pipe, hi_isp_radial_shading_attr *radial_shading_attr)
{
    isp_err_trace("Not support this interface!\n");
    return HI_ERR_ISP_NOT_SUPPORT;
}

hi_s32 isp_set_radial_shading_lut(hi_vi_pipe vi_pipe, const hi_isp_radial_shading_lut_attr *radial_shading_lut_attr)
{
    isp_err_trace("Not support this interface!\n");
    return HI_ERR_ISP_NOT_SUPPORT;
}

hi_s32 isp_get_radial_shading_lut(hi_vi_pipe vi_pipe, hi_isp_radial_shading_lut_attr *radial_shading_lut_attr)
{
    isp_err_trace("Not support this interface!\n");
    return HI_ERR_ISP_NOT_SUPPORT;
}

hi_s32 isp_set_pipe_differ_attr(hi_vi_pipe vi_pipe, const hi_isp_pipe_diff_attr *pipe_differ)
{
    isp_err_trace("Not support this interface!\n");
    return HI_ERR_ISP_NOT_SUPPORT;
}

hi_s32 isp_get_pipe_differ_attr(hi_vi_pipe vi_pipe, hi_isp_pipe_diff_attr *pipe_differ)
{
    isp_err_trace("Not support this interface!\n");
    return HI_ERR_ISP_NOT_SUPPORT;
}

hi_s32 isp_set_rc_attr(hi_vi_pipe vi_pipe, const hi_isp_rc_attr *rc_attr)
{
    isp_err_trace("Not support this interface!\n");
    return HI_ERR_ISP_NOT_SUPPORT;
}

hi_s32 isp_get_rc_attr(hi_vi_pipe vi_pipe, hi_isp_rc_attr *rc_attr)
{
    isp_err_trace("Not support this interface!\n");
    return HI_ERR_ISP_NOT_SUPPORT;
}

hi_s32 isp_set_rgbir_attr(hi_vi_pipe vi_pipe, const hi_isp_rgbir_attr *rgbir_attr)
{
    isp_err_trace("Not support this interface!\n");
    return HI_ERR_ISP_NOT_SUPPORT;
}

hi_s32 isp_get_rgbir_attr(hi_vi_pipe vi_pipe, hi_isp_rgbir_attr *rgbir_attr)
{
    isp_err_trace("Not support this interface!\n");
    return HI_ERR_ISP_NOT_SUPPORT;
}

hi_s32 isp_set_pre_log_lut_attr(hi_vi_pipe vi_pipe, const hi_isp_preloglut_attr *pre_log_lut_attr)
{
    isp_err_trace("Not support this interface!\n");
    return HI_ERR_ISP_NOT_SUPPORT;
}

hi_s32 isp_get_pre_log_lut_attr(hi_vi_pipe vi_pipe, hi_isp_preloglut_attr *pre_log_lut_attr)
{
    isp_err_trace("Not support this interface!\n");
    return HI_ERR_ISP_NOT_SUPPORT;
}

hi_s32 isp_set_log_lut_attr(hi_vi_pipe vi_pipe, const hi_isp_loglut_attr *log_lut_attr)
{
    isp_err_trace("Not support this interface!\n");
    return HI_ERR_ISP_NOT_SUPPORT;
}

hi_s32 isp_get_log_lut_attr(hi_vi_pipe vi_pipe, hi_isp_loglut_attr *log_lut_attr)
{
    isp_err_trace("Not support this interface!\n");
    return HI_ERR_ISP_NOT_SUPPORT;
}

hi_s32 isp_set_clut_coeff(hi_vi_pipe vi_pipe, const hi_isp_clut_lut *clut_lut)
{
    hi_s32 ret;
    hi_u32 *vir_addr = HI_NULL;
    isp_mmz_buf_ex clut_buf;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(clut_lut);
    isp_check_open_return(vi_pipe);
    isp_check_mem_init_return(vi_pipe);

    if (ioctl(isp_get_fd(vi_pipe), ISP_CLUT_BUF_GET, &clut_buf.phy_addr) != HI_SUCCESS) {
        isp_err_trace("get clut buffer err\n");
        return HI_ERR_ISP_NOMEM;
    }

    clut_buf.vir_addr = HI_MPI_SYS_Mmap(clut_buf.phy_addr, HI_ISP_CLUT_LUT_LENGTH * sizeof(hi_u32));

    if (clut_buf.vir_addr == HI_NULL) {
        return HI_ERR_ISP_NULL_PTR;
    }

    vir_addr = (hi_u32 *)clut_buf.vir_addr;

    ret = memcpy_s(vir_addr, HI_ISP_CLUT_LUT_LENGTH * sizeof(hi_u32),
                   clut_lut->lut, HI_ISP_CLUT_LUT_LENGTH * sizeof(hi_u32));
    if (ret != 0) {
        isp_err_trace("ISP[%d] set clut lut failed!\n", vi_pipe);
    }

    hi_ext_system_clut_lut_update_en_write(vi_pipe, HI_TRUE);

    HI_MPI_SYS_Munmap(clut_buf.vir_addr, HI_ISP_CLUT_LUT_LENGTH * sizeof(hi_u32));

    return HI_SUCCESS;
}

hi_s32 isp_get_clut_coeff(hi_vi_pipe vi_pipe, hi_isp_clut_lut *clut_lut)
{
    hi_s32 ret;
    hi_u32 *vir_addr = HI_NULL;
    isp_mmz_buf_ex clut_buf;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(clut_lut);
    isp_check_open_return(vi_pipe);
    isp_check_mem_init_return(vi_pipe);

    if (ioctl(isp_get_fd(vi_pipe), ISP_CLUT_BUF_GET, &clut_buf.phy_addr) != HI_SUCCESS) {
        isp_err_trace("get clut buffer err\n");
        return HI_ERR_ISP_NOMEM;
    }

    clut_buf.vir_addr = HI_MPI_SYS_Mmap(clut_buf.phy_addr, HI_ISP_CLUT_LUT_LENGTH * sizeof(hi_u32));

    if (clut_buf.vir_addr == HI_NULL) {
        return HI_ERR_ISP_NULL_PTR;
    }

    vir_addr = (hi_u32 *)clut_buf.vir_addr;

    ret = memcpy_s(clut_lut->lut, HI_ISP_CLUT_LUT_LENGTH * sizeof(hi_u32),
                   vir_addr, HI_ISP_CLUT_LUT_LENGTH * sizeof(hi_u32));
    if (ret != 0) {
        isp_err_trace("ISP[%d] get clut lut failed!\n", vi_pipe);
    }

    HI_MPI_SYS_Munmap(clut_buf.vir_addr, HI_ISP_CLUT_LUT_LENGTH * sizeof(hi_u32));

    return HI_SUCCESS;
}

hi_s32 isp_set_clut_attr(hi_vi_pipe vi_pipe, const hi_isp_clut_attr *clut_attr)
{
    hi_u32 max_gain = 4095;

    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(clut_attr);
    isp_check_bool_return(clut_attr->enable);
    isp_check_open_return(vi_pipe);
    isp_check_mem_init_return(vi_pipe);

    if (clut_attr->gain_r > max_gain) {
        isp_err_trace("Invalid gain_r!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (clut_attr->gain_g > max_gain) {
        isp_err_trace("Invalid gain_g!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if (clut_attr->gain_b > max_gain) {
        isp_err_trace("Invalid gain_b!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    hi_ext_system_clut_en_write(vi_pipe, clut_attr->enable);
    hi_ext_system_clut_gain_r_write(vi_pipe, clut_attr->gain_r);
    hi_ext_system_clut_gain_g_write(vi_pipe, clut_attr->gain_g);
    hi_ext_system_clut_gain_b_write(vi_pipe, clut_attr->gain_b);
    hi_ext_system_clut_ctrl_update_en_write(vi_pipe, HI_TRUE);

    return HI_SUCCESS;
}
hi_s32 isp_get_clut_attr(hi_vi_pipe vi_pipe, hi_isp_clut_attr *clut_attr)
{
    isp_check_pipe_return(vi_pipe);
    isp_check_pointer_return(clut_attr);
    isp_check_open_return(vi_pipe);
    isp_check_mem_init_return(vi_pipe);

    clut_attr->enable = hi_ext_system_clut_en_read(vi_pipe);
    clut_attr->gain_r = hi_ext_system_clut_gain_r_read(vi_pipe);
    clut_attr->gain_g = hi_ext_system_clut_gain_g_read(vi_pipe);
    clut_attr->gain_b = hi_ext_system_clut_gain_b_read(vi_pipe);

    return HI_SUCCESS;
}

hi_s32 isp_set_raw_pos(hi_vi_pipe vi_pipe, const hi_isp_raw_pos_attr *raw_pos_attr)
{
    return HI_ERR_ISP_NOT_SUPPORT;
}

hi_s32 isp_get_raw_pos(hi_vi_pipe vi_pipe, hi_isp_raw_pos_attr *raw_pos_attr)
{
    return HI_ERR_ISP_NOT_SUPPORT;
}

hi_s32 isp_calc_flicker_type(hi_vi_pipe vi_pipe, hi_isp_calc_flicker_input *input_param,
                             hi_isp_calc_flicker_output *output_param, hi_video_frame_info frame[],
                             hi_u32 array_size)
{
    hi_u32 i = 0;

    if (array_size != 3) { /* frame number cannot be 3 */
        isp_err_trace("Frame Number is not 3 Frame\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    isp_check_pipe_return(vi_pipe);
    isp_check_open_return(vi_pipe);
    isp_check_mem_init_return(vi_pipe);

    isp_check_pointer_return(input_param);
    isp_check_pointer_return(output_param);

    if (input_param->lines_per_second < 500) { /* Lines per second range [500, inf) */
        isp_err_trace("LinePerSecond is out of range\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    for (i = 0; i < array_size; i++) {
        isp_check_pointer_return(frame + i);

        if (frame[i].v_frame.phy_addr[0] == 0) {
            isp_err_trace("The Phy Address Error!!!\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if ((frame[i].v_frame.width < RES_WIDTH_MIN) ||
            (frame[i].v_frame.width > res_width_max(vi_pipe))) {
            isp_err_trace("The Image width is out of range!!!\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if ((frame[i].v_frame.height < RES_HEIGHT_MIN) ||
            (frame[i].v_frame.height > res_height_max(vi_pipe))) {
            isp_err_trace("The Image height is out of range!!!\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (i != 0) {
            if ((frame[i].v_frame.time_ref - frame[i - 1].v_frame.time_ref) != 2) { /* discontinue is 2 frames */
                isp_err_trace("The Frames is not continuity!!!\n");
                return HI_ERR_ISP_ILLEGAL_PARAM;
            }
        }
    }

    return calc_flicker_type(input_param, output_param, frame, array_size);
}

