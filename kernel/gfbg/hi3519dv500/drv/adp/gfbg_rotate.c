/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#include "gfbg_rotate.h"

static drv_tde_func_callback g_tde_rotate_call_back;

gfbg_rotate_ops g_gfbg_rotate_ops = {0};

td_void gfbg_rotation_register(td_void)
{
    g_gfbg_rotate_ops.gfbg_drv_rotate = gfbg_drv_rotate_process;
    g_gfbg_rotate_ops.is_support = TD_TRUE;
    return;
}

td_bool gfbg_get_rotation_support(td_void)
{
    return g_gfbg_rotate_ops.is_support;
}

gfbg_rotate gfbg_get_rotation(td_void)
{
    return g_gfbg_rotate_ops.gfbg_drv_rotate;
}

td_s32 gfbg_drv_set_tde_rotate_callback(drv_tde_func_callback tde_rot_callback)
{
    g_tde_rotate_call_back = tde_rot_callback;
    return TD_SUCCESS;
}

drv_tde_func_callback gfbg_drv_get_tde_rotate_callback(td_void)
{
    return g_tde_rotate_call_back;
}

