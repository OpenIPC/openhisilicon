/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_debug.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "hi_isp_debug.h"
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include <stdio.h>
#include "isp_main.h"
#include "isp_debug.h"
#include "isp_ext_config.h"
#include "mpi_sys.h"

hi_s32 isp_dbg_set(hi_vi_pipe vi_pipe, const hi_isp_debug_info *dbg_info)
{
    hi_u32 size = 0;

    if (hi_ext_system_sys_debug_enable_read(vi_pipe) && dbg_info->debug_en) {
        isp_err_trace("Hisi isp has enabled debug info!\n");
        return HI_FAILURE;
    }

    if (dbg_info->debug_en) {
        if (dbg_info->phy_addr == 0) {
            isp_err_trace("Hisi isp lib's debug phyaddr is 0!\n");
            return HI_FAILURE;
        }

        if (dbg_info->depth == 0) {
            isp_err_trace("Hisi ae lib's debug depth is 0!\n");
            return HI_FAILURE;
        }
        size = sizeof(hi_isp_dbg_attr) +
               sizeof(hi_isp_dbg_attr) * dbg_info->depth;
    }

    /* don't clear phyaddr and size when disable dbg info. */
    if (dbg_info->debug_en) {
        hi_ext_system_sys_debug_enable_write(vi_pipe, dbg_info->debug_en);
        hi_ext_system_sys_debug_high_addr_write(vi_pipe, ((dbg_info->phy_addr >> 32) & 0xFFFFFFFF)); /* shift 32 */
        hi_ext_system_sys_debug_low_addr_write(vi_pipe, (dbg_info->phy_addr & 0xFFFFFFFF));
        hi_ext_system_sys_debug_depth_write(vi_pipe, dbg_info->depth);
        hi_ext_system_sys_debug_size_write(vi_pipe, size);
    } else {
        hi_ext_system_sys_debug_enable_write(vi_pipe, dbg_info->debug_en);
    }

    return HI_SUCCESS;
}

hi_s32 isp_dbg_get(hi_vi_pipe vi_pipe, hi_isp_debug_info *dbg_info)
{
    hi_u64 phy_addr_high;
    hi_u64 phy_addr_temp;

    phy_addr_high  = (hi_u64)hi_ext_system_sys_debug_high_addr_read(vi_pipe);
    phy_addr_temp  = (hi_u64)hi_ext_system_sys_debug_low_addr_read(vi_pipe);
    phy_addr_temp |= (phy_addr_high << 32); /* left shift 32bits */

    dbg_info->phy_addr = phy_addr_temp;
    dbg_info->debug_en = hi_ext_system_sys_debug_enable_read(vi_pipe);
    dbg_info->depth    = hi_ext_system_sys_debug_depth_read(vi_pipe);

    return HI_SUCCESS;
}

hi_s32 isp_dbg_run_bgn(isp_dbg_ctrl *dbg, hi_u32 frm_cnt)
{
    hi_isp_dbg_status  *dbg_status = HI_NULL;

    if (!dbg->debug_en) {
        if (dbg->dbg_attr != HI_NULL) {
            HI_MPI_SYS_Munmap(dbg->dbg_attr, dbg->size);
            dbg->dbg_attr = HI_NULL;
            dbg->dbg_status = HI_NULL;
        }
        return HI_SUCCESS;
    }

    if ((dbg->debug_en) && (dbg->dbg_attr == HI_NULL)) {
        dbg->dbg_attr = (hi_isp_dbg_attr *)HI_MPI_SYS_Mmap(dbg->phy_addr, dbg->size);
        if (dbg->dbg_attr == HI_NULL) {
            isp_err_trace("isp map debug buf failed!\n");
            return HI_FAILURE;
        }
        dbg->dbg_status = (hi_isp_dbg_status *)(dbg->dbg_attr + 1);
    }

    dbg_status = dbg->dbg_status + (frm_cnt % div_0_to_1(dbg->depth));

    dbg_status->frm_num_bgn = frm_cnt;

    return HI_SUCCESS;
}

hi_s32 isp_dbg_run_end(isp_dbg_ctrl *dbg, hi_u32 frm_cnt)
{
    hi_isp_dbg_status *dbg_status = HI_NULL;

    if ((!dbg->debug_en) || (dbg->dbg_status == HI_NULL)) {
        return HI_SUCCESS;
    }
    dbg_status = dbg->dbg_status + (frm_cnt % div_0_to_1(dbg->depth));
    /* record status */
    dbg_status->frm_num_end = frm_cnt;

    return HI_SUCCESS;
}

