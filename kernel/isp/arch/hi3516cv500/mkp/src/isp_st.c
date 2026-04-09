/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of isp_st.c
 * Author: ISP SW
 * Create: 2012/06/28
 */

#include "isp_st.h"

hi_s32 hi_isp_sync_task_register(hi_vi_pipe vi_pipe, ISP_SYNC_TASK_NODE_S *new_node)
{
    HI_ASSERT(sizeof(isp_sync_task_node) == sizeof(ISP_SYNC_TASK_NODE_S));
    return isp_sync_task_register(vi_pipe, (isp_sync_task_node *)new_node);
}
hi_s32 hi_isp_sync_task_unregister(hi_vi_pipe vi_pipe, ISP_SYNC_TASK_NODE_S *del_node)
{
    HI_ASSERT(sizeof(isp_sync_task_node) == sizeof(ISP_SYNC_TASK_NODE_S));
    return isp_sync_task_unregister(vi_pipe, (isp_sync_task_node *)del_node);
}


