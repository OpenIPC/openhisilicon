#include "xmedia_svp.h"
#include "task_common.h"
#include "svp_proc.h"

// 此处每次发布版本要手动更新
static xmedia_svp_context *g_context = XMEDIA_NULL;
static const xmedia_char *XMEDIA_SVP_VERSION_INFO = "svp_250626v03";

xmedia_s32 xmedia_svp_init()
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    if (g_context == XMEDIA_NULL) {
        g_context = (xmedia_svp_context*)calloc(1, sizeof(xmedia_svp_context));
        if (g_context == XMEDIA_NULL) {
            SVP_TRACE(MODULE_DBG_ERR, "g_context calloc failed !!\n");
            return XMEDIA_FAILURE;
        }
        pthread_mutex_init(&g_context->g_svp_mutex, XMEDIA_NULL);
        g_context->g_svp_init_flag = 0;
        g_context->proc = XMEDIA_NULL;
    }

    if (g_context->g_svp_init_flag == 0) {
        ret = npu_init_context();
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_ERR, "svp init failed \n");
            return XMEDIA_FAILURE;
        }

#ifdef DEBUG_PROC_INFO
        ret = svp_init_proc(g_context);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_ERR, "svp init proc failed \n");
            return XMEDIA_FAILURE;
        }
#endif

        ret = xmedia_vgs_init();
        ret = xmedia_tde_init();
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_ERR, "vgs tde init failed \n");
            return XMEDIA_FAILURE;
        }
        g_context->g_svp_init_flag = 1;
    } else {
        g_context->g_svp_init_flag++;
        SVP_TRACE(MODULE_DBG_ERR, "svp init err multiple times! \n");
    }
    return ret;
}

xmedia_s32 xmedia_svp_uninit()
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    xmedia_u32 index;

    if (g_context->g_svp_init_flag <= 0) {
        SVP_TRACE(MODULE_DBG_ERR, "uninit failed, not initialized or uninit mult!\n");
        return XMEDIA_FAILURE;
    }

    g_context->g_svp_init_flag--;
    if (g_context->g_svp_init_flag == 0) {
        for (index = 0; index < XMEDIA_SVP_MAX_TASK_NUM; index++) {
            if (g_context->g_task_handle[index].used != XMEDIA_FALSE ||
                g_context->g_task_handle[index].context != XMEDIA_NULL) {
                SVP_TRACE(MODULE_DBG_ERR, "there are models undestroy !\n");
                return XMEDIA_FAILURE;
            }
        }

        xmedia_vgs_exit();
        xmedia_tde_exit();

#ifdef DEBUG_PROC_INFO
        ret = svp_deinit_proc(g_context);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_ERR, "svp deinit proc failed \n");
            return XMEDIA_FAILURE;
        }
#endif

        ret = npu_release_context();
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_ERR, "svp uninit failed ! \n");
            return XMEDIA_FAILURE;
        }
        memset(g_context->g_task_handle, 0, sizeof(svp_task_handle) * XMEDIA_SVP_MAX_TASK_NUM);
    }

    return ret;
}

xmedia_s32 xmedia_svp_set_config(xmedia_svp_cfg* svp_cfg)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    if (g_context->g_svp_init_flag <= 0) {
        SVP_TRACE(MODULE_DBG_ERR, "get ptr is null! \n");
        return XMEDIA_FAILURE;
    }

    if (svp_cfg == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "svp_cfg is null! \n");
        return XMEDIA_FAILURE;
    }

    pthread_mutex_lock(&g_context->g_svp_mutex);
    ret = svp_set_cfg(svp_cfg);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "set svp_cfg failed!!!\n");
        pthread_mutex_unlock(&g_context->g_svp_mutex);
        return ret;
    }

    pthread_mutex_unlock(&g_context->g_svp_mutex);

    return ret;
}

xmedia_s32 xmedia_svp_get_config(xmedia_svp_cfg* svp_cfg)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    if (g_context->g_svp_init_flag <= 0) {
        SVP_TRACE(MODULE_DBG_ERR, "get ptr is null! \n");
        return XMEDIA_FAILURE;
    }

    if (svp_cfg == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "svp_cfg is null! \n");
        return XMEDIA_FAILURE;
    }

    pthread_mutex_lock(&g_context->g_svp_mutex);
    ret = svp_get_cfg(svp_cfg);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "get svp_cfg failed!!!\n");
        pthread_mutex_unlock(&g_context->g_svp_mutex);
        return ret;
    }

    pthread_mutex_unlock(&g_context->g_svp_mutex);

    return ret;
}

xmedia_s32 xmedia_svp_task_create(xmedia_s32 *handle, const xmedia_svp_task_cfg cfg)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;
    xmedia_u32 index;
    svp_proc_context* proc_context = (svp_proc_context*)g_context->proc;

    if (proc_context == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "proc_context is null! \n");
        return XMEDIA_FAILURE;
    }

    if ((handle ==  XMEDIA_NULL) || (g_context->g_svp_init_flag <= 0) || (cfg.modules == XMEDIA_NULL)) {
        SVP_TRACE(MODULE_DBG_ERR, "get ptr is null, handle=%p \n", handle);
        return XMEDIA_FAILURE;
    }
    *handle = -1;

    pthread_mutex_lock(&g_context->g_svp_mutex);
    for (index = 0; index < XMEDIA_SVP_MAX_TASK_NUM; index++) {
        if (g_context->g_task_handle[index].used == XMEDIA_FALSE) {
            break;
        }
    }
    if (index >= XMEDIA_SVP_MAX_TASK_NUM) {
        SVP_TRACE(MODULE_DBG_ERR, "index=%u, max num  support (16) \n", index);
        pthread_mutex_unlock(&g_context->g_svp_mutex);
        return XMEDIA_FAILURE;
    }

    ret = svp_task_common_create(&g_context->g_task_handle[index], cfg.task_type, cfg);
    if (ret != XMEDIA_SUCCESS) {
        g_context->g_task_handle[index].used = XMEDIA_FALSE;
        SVP_TRACE(MODULE_DBG_ERR, "task_type[%u] init failed!!!\n", cfg.task_type);
        pthread_mutex_unlock(&g_context->g_svp_mutex);
        return ret;
    }

    g_context->g_task_handle[index].task_type = cfg.task_type;
    g_context->g_task_handle[index].used = XMEDIA_TRUE;
    proc_context->handle[index] = &g_context->g_task_handle[index];

#ifdef DEBUG_PROC_INFO
    svp_update_proc_info(g_context->proc, index);
#endif

    *handle = index;

    pthread_mutex_unlock(&g_context->g_svp_mutex);
    return ret;
}

xmedia_s32 xmedia_svp_task_set_attr(xmedia_s32 handle, const xmedia_void *task_attr)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    if ((handle < 0) || (handle >= XMEDIA_SVP_MAX_TASK_NUM)) {
        SVP_TRACE(MODULE_DBG_ERR, "handle=%d, not support\n", handle);
        return XMEDIA_FAILURE;
    }
    if (task_attr == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "set task_attr is null\n");
        return XMEDIA_FAILURE;
    }

    pthread_mutex_lock(&g_context->g_svp_mutex);
    if (g_context->g_task_handle[handle].used == XMEDIA_FALSE) {
        SVP_TRACE(MODULE_DBG_ERR, "g_task_handle[%d] is not used\n", handle);
        pthread_mutex_unlock(&g_context->g_svp_mutex);
        return XMEDIA_FAILURE;
    }

    ret = svp_task_common_set(&g_context->g_task_handle[handle], g_context->g_task_handle[handle].task_type, task_attr);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "task_type[%u] svp set_attr failed!!!\n", g_context->g_task_handle[handle].task_type);
        pthread_mutex_unlock(&g_context->g_svp_mutex);
        return ret;
    }

#ifdef DEBUG_PROC_INFO
    svp_update_proc_info(g_context->proc, handle);
#endif

    pthread_mutex_unlock(&g_context->g_svp_mutex);

    return ret;
}

xmedia_s32 xmedia_svp_task_get_attr(xmedia_s32 handle, xmedia_void *task_attr)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    if ((handle < 0) || (handle >= XMEDIA_SVP_MAX_TASK_NUM)) {
        SVP_TRACE(MODULE_DBG_ERR, "handle=%d, not support\n", handle);
        return XMEDIA_FAILURE;
    }
    if (task_attr == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "task_attr ptr is null\n");
        return XMEDIA_FAILURE;
    }

    pthread_mutex_lock(&g_context->g_svp_mutex);
    if (g_context->g_task_handle[handle].used == XMEDIA_FALSE) {
        SVP_TRACE(MODULE_DBG_ERR, "g_task_handle[%d] is not used\n", handle);
        pthread_mutex_unlock(&g_context->g_svp_mutex);
        return XMEDIA_FAILURE;
    }

    ret = svp_task_common_get(&g_context->g_task_handle[handle], g_context->g_task_handle[handle].task_type, task_attr);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "task_type[%u] svp get_attr failed!!!\n", g_context->g_task_handle[handle].task_type);
        pthread_mutex_unlock(&g_context->g_svp_mutex);
        return ret;
    }
    pthread_mutex_unlock(&g_context->g_svp_mutex);

    return ret;
}

xmedia_s32 xmedia_svp_task_process(xmedia_s32 handle, const xmedia_svp_task_input *input, xmedia_void *output)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    if ((handle < 0) || (handle >= XMEDIA_SVP_MAX_TASK_NUM)) {
        SVP_TRACE(MODULE_DBG_ERR, "handle=%d, not support\n", handle);
        return XMEDIA_FAILURE;
    }

    if ((input == XMEDIA_NULL) || (output == XMEDIA_NULL) || input->frame == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "ptr is null\n");
        return XMEDIA_FAILURE;
    }

    pthread_mutex_lock(&g_context->g_svp_mutex);
    if (g_context->g_task_handle[handle].used == XMEDIA_FALSE) {
        SVP_TRACE(MODULE_DBG_ERR, "g_task_handle[%d] is not used\n", handle);
        pthread_mutex_unlock(&g_context->g_svp_mutex);
        return XMEDIA_FAILURE;
    }

#ifdef DEBUG_PROC_INFO
    xmedia_u64 cost_time;
    struct timeval begin;
    struct timeval end;
    gettimeofday(&begin, XMEDIA_NULL);
#endif

    ret = svp_task_common_process(&g_context->g_task_handle[handle],
                                   g_context->g_task_handle[handle].task_type, input, output);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "task_type[%u] svp process failed!!!\n", g_context->g_task_handle[handle].task_type);
        pthread_mutex_unlock(&g_context->g_svp_mutex);
        return ret;
    }

#ifdef DEBUG_PROC_INFO
    gettimeofday(&end, XMEDIA_NULL);
    cost_time = (end.tv_sec - begin.tv_sec) * 1000 \
              + (end.tv_usec - begin.tv_usec) / 1000;
    svp_update_proc_time(g_context->proc, handle, cost_time);
    svp_update_proc_framerate(g_context->proc, handle);
    svp_update_proc_info(g_context->proc, handle);
#endif

    pthread_mutex_unlock(&g_context->g_svp_mutex);
    return ret;
}

xmedia_s32 xmedia_svp_task_destroy(xmedia_s32 handle)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    if ((handle < 0) || (handle >= XMEDIA_SVP_MAX_TASK_NUM)) {
        SVP_TRACE(MODULE_DBG_ERR, "handle=%d, not support\n", handle);
        return XMEDIA_FAILURE;
    }

    pthread_mutex_lock(&g_context->g_svp_mutex);

    if (g_context->g_task_handle[handle].used == XMEDIA_FALSE) {
        SVP_TRACE(MODULE_DBG_ERR, "g_task_handle[%d] is not used\n", handle);
        pthread_mutex_unlock(&g_context->g_svp_mutex);
        return XMEDIA_FAILURE;
    }

    ret = svp_task_common_destroy(&g_context->g_task_handle[handle], g_context->g_task_handle[handle].task_type);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "task_type[%u] task_destroy failed!!!\n", g_context->g_task_handle[handle].task_type);
        pthread_mutex_unlock(&g_context->g_svp_mutex);
        return ret;
    }
    g_context->g_task_handle[handle].used = XMEDIA_FALSE;
    pthread_mutex_unlock(&g_context->g_svp_mutex);
    return ret;
}

xmedia_s32 xmedia_svp_get_version(xmedia_char *version, const xmedia_s32 len)
{
    if (version == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "version is null\n");
        return XMEDIA_FAILURE;
    }
    if (snprintf(version, len, "%s", XMEDIA_SVP_VERSION_INFO) < 0) {
        SVP_TRACE(MODULE_DBG_ERR, "get version error\n");
        return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}
