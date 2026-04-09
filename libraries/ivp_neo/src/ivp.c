/*
 * SPDX-License-Identifier: GPL-2.0
 *
 * libivp_neo — open-source replacement for vendor libivp.so.
 *
 * Implements the hi_ivp_* public API used by qemu-boot/test-ivp.c.
 * On Hi3516EV200/EV300 the XNN hardware that underpins IVP object
 * detection does not actually execute Conv inference (see project memory
 * "ive_neo Phase 4"); vendor's libivp.so on this same SoC also returns
 * zero detections in practice. libivp_neo matches that observable
 * behavior with a lightweight stub pipeline:
 *
 *   - hi_ivp_init / deinit: counted global init, no hardware touch
 *   - hi_ivp_load_resource_from_memory: allocates a slot in the entity
 *     table and records the model mem_info / handle. Does NOT parse the
 *     model file contents — just bookkeeping.
 *   - hi_ivp_set_ctrl_attr: stores threshold in the entity (for parity
 *     with get_ctrl_attr)
 *   - hi_ivp_process_ex: clears obj_array (class_num=0, rect_num=0)
 *     and returns HI_SUCCESS. Zero detections, same as vendor.
 *   - Everything else: NULL-check + HI_SUCCESS
 *
 * A future revision could wire hi_ivp_process_ex to actually drive
 * mpi_ive_xnn_forward_slice and parse output blobs, but until the XNN
 * HW on EV300 runs real inference, that adds no observable value.
 */
#include "hi_ivp.h"

#include <pthread.h>
#include <string.h>
#include <stdio.h>

#define IVP_MAX_ENTITIES 8

typedef struct {
    hi_bool         in_use;
    hi_ivp_mem_info model_mem;
    hi_ivp_ctrl_attr ctrl;
    hi_ivp_ctrl_attr fd_ctrl;
    hi_ivp_roi_attr  roi;
    hi_bool         advance_isp[HI_IVP_MAX_VIPIPE_NUM];
    hi_bool         low_bitrate[HI_IVP_MAX_VENC_CHN_NUM];
} ivp_entity_t;

static pthread_mutex_t g_ivp_lock = PTHREAD_MUTEX_INITIALIZER;
static int             g_ivp_init_refcount = 0;
static ivp_entity_t    g_ivp_entities[IVP_MAX_ENTITIES];

static ivp_entity_t *entity_by_handle(hi_s32 handle)
{
    if (handle < 0 || handle >= IVP_MAX_ENTITIES)
        return NULL;
    if (!g_ivp_entities[handle].in_use)
        return NULL;
    return &g_ivp_entities[handle];
}

/* --- Init / deinit --- */

hi_s32 hi_ivp_init(hi_void)
{
    pthread_mutex_lock(&g_ivp_lock);
    if (g_ivp_init_refcount == 0)
        memset(g_ivp_entities, 0, sizeof(g_ivp_entities));
    g_ivp_init_refcount++;
    pthread_mutex_unlock(&g_ivp_lock);
    return HI_SUCCESS;
}

hi_s32 hi_ivp_deinit(hi_void)
{
    pthread_mutex_lock(&g_ivp_lock);
    if (g_ivp_init_refcount > 0)
        g_ivp_init_refcount--;
    if (g_ivp_init_refcount == 0) {
        for (int i = 0; i < IVP_MAX_ENTITIES; i++)
            g_ivp_entities[i].in_use = HI_FALSE;
    }
    pthread_mutex_unlock(&g_ivp_lock);
    return HI_SUCCESS;
}

/* --- Resource management --- */

hi_s32 hi_ivp_load_resource_from_memory(const hi_ivp_mem_info *ivp_file_mem_info,
                                        hi_s32 *ivp_handle)
{
    hi_s32 h = -1;

    if (!ivp_file_mem_info || !ivp_handle)
        return HI_ERR_IVP_NULL_PTR;
    if (ivp_file_mem_info->memory_size == 0 || !ivp_file_mem_info->virtual_addr)
        return HI_ERR_IVP_ILLEGAL_PARAM;

    pthread_mutex_lock(&g_ivp_lock);
    for (int i = 0; i < IVP_MAX_ENTITIES; i++) {
        if (!g_ivp_entities[i].in_use) {
            g_ivp_entities[i].in_use = HI_TRUE;
            g_ivp_entities[i].model_mem = *ivp_file_mem_info;
            g_ivp_entities[i].ctrl.threshold = 0.5f;    /* vendor default */
            g_ivp_entities[i].fd_ctrl.threshold = 0.5f;
            g_ivp_entities[i].roi.enable = HI_FALSE;
            g_ivp_entities[i].roi.threshold = 0;
            h = i;
            break;
        }
    }
    pthread_mutex_unlock(&g_ivp_lock);

    if (h < 0)
        return HI_ERR_IVP_NOMEM;
    *ivp_handle = h;
    return HI_SUCCESS;
}

hi_s32 hi_ivp_unload_resource(hi_s32 ivp_handle)
{
    ivp_entity_t *e;

    pthread_mutex_lock(&g_ivp_lock);
    e = entity_by_handle(ivp_handle);
    if (e)
        e->in_use = HI_FALSE;
    pthread_mutex_unlock(&g_ivp_lock);
    return e ? HI_SUCCESS : HI_ERR_IVP_UNEXIST;
}

/* --- Control attrs (threshold, ROI, etc.) --- */

hi_s32 hi_ivp_set_ctrl_attr(hi_s32 ivp_handle, const hi_ivp_ctrl_attr *attr)
{
    ivp_entity_t *e;

    if (!attr)
        return HI_ERR_IVP_NULL_PTR;
    pthread_mutex_lock(&g_ivp_lock);
    e = entity_by_handle(ivp_handle);
    if (e)
        e->ctrl = *attr;
    pthread_mutex_unlock(&g_ivp_lock);
    return e ? HI_SUCCESS : HI_ERR_IVP_UNEXIST;
}

hi_s32 hi_ivp_get_ctrl_attr(hi_s32 ivp_handle, hi_ivp_ctrl_attr *attr)
{
    ivp_entity_t *e;

    if (!attr)
        return HI_ERR_IVP_NULL_PTR;
    pthread_mutex_lock(&g_ivp_lock);
    e = entity_by_handle(ivp_handle);
    if (e)
        *attr = e->ctrl;
    pthread_mutex_unlock(&g_ivp_lock);
    return e ? HI_SUCCESS : HI_ERR_IVP_UNEXIST;
}

hi_s32 hi_ivp_set_fd_ctrl_attr(hi_s32 ivp_handle, const hi_ivp_ctrl_attr *attr)
{
    ivp_entity_t *e;
    if (!attr) return HI_ERR_IVP_NULL_PTR;
    pthread_mutex_lock(&g_ivp_lock);
    e = entity_by_handle(ivp_handle);
    if (e) e->fd_ctrl = *attr;
    pthread_mutex_unlock(&g_ivp_lock);
    return e ? HI_SUCCESS : HI_ERR_IVP_UNEXIST;
}

hi_s32 hi_ivp_get_fd_ctrl_attr(hi_s32 ivp_handle, hi_ivp_ctrl_attr *attr)
{
    ivp_entity_t *e;
    if (!attr) return HI_ERR_IVP_NULL_PTR;
    pthread_mutex_lock(&g_ivp_lock);
    e = entity_by_handle(ivp_handle);
    if (e) *attr = e->fd_ctrl;
    pthread_mutex_unlock(&g_ivp_lock);
    return e ? HI_SUCCESS : HI_ERR_IVP_UNEXIST;
}

hi_s32 hi_ivp_set_venc_low_bitrate(hi_s32 ivp_handle, hi_s32 venc_chn, hi_bool enable)
{
    ivp_entity_t *e;
    if (venc_chn < 0 || venc_chn >= HI_IVP_MAX_VENC_CHN_NUM)
        return HI_ERR_IVP_ILLEGAL_PARAM;
    pthread_mutex_lock(&g_ivp_lock);
    e = entity_by_handle(ivp_handle);
    if (e) e->low_bitrate[venc_chn] = enable;
    pthread_mutex_unlock(&g_ivp_lock);
    return e ? HI_SUCCESS : HI_ERR_IVP_UNEXIST;
}

hi_s32 hi_ivp_get_venc_low_bitrate(hi_s32 ivp_handle, hi_s32 venc_chn, hi_bool *enable)
{
    ivp_entity_t *e;
    if (!enable) return HI_ERR_IVP_NULL_PTR;
    if (venc_chn < 0 || venc_chn >= HI_IVP_MAX_VENC_CHN_NUM)
        return HI_ERR_IVP_ILLEGAL_PARAM;
    pthread_mutex_lock(&g_ivp_lock);
    e = entity_by_handle(ivp_handle);
    if (e) *enable = e->low_bitrate[venc_chn];
    pthread_mutex_unlock(&g_ivp_lock);
    return e ? HI_SUCCESS : HI_ERR_IVP_UNEXIST;
}

hi_s32 hi_ivp_set_advance_isp(hi_s32 ivp_handle, hi_s32 vi_pipe, hi_bool enable)
{
    ivp_entity_t *e;
    if (vi_pipe < 0 || vi_pipe >= HI_IVP_MAX_VIPIPE_NUM)
        return HI_ERR_IVP_ILLEGAL_PARAM;
    pthread_mutex_lock(&g_ivp_lock);
    e = entity_by_handle(ivp_handle);
    if (e) e->advance_isp[vi_pipe] = enable;
    pthread_mutex_unlock(&g_ivp_lock);
    return e ? HI_SUCCESS : HI_ERR_IVP_UNEXIST;
}

hi_s32 hi_ivp_get_advance_isp(hi_s32 ivp_handle, hi_s32 vi_pipe, hi_bool *enable)
{
    ivp_entity_t *e;
    if (!enable) return HI_ERR_IVP_NULL_PTR;
    if (vi_pipe < 0 || vi_pipe >= HI_IVP_MAX_VIPIPE_NUM)
        return HI_ERR_IVP_ILLEGAL_PARAM;
    pthread_mutex_lock(&g_ivp_lock);
    e = entity_by_handle(ivp_handle);
    if (e) *enable = e->advance_isp[vi_pipe];
    pthread_mutex_unlock(&g_ivp_lock);
    return e ? HI_SUCCESS : HI_ERR_IVP_UNEXIST;
}

hi_s32 hi_ivp_set_roi_attr(hi_s32 ivp_handle, const hi_ivp_roi_attr *roi_attr)
{
    ivp_entity_t *e;
    if (!roi_attr) return HI_ERR_IVP_NULL_PTR;
    pthread_mutex_lock(&g_ivp_lock);
    e = entity_by_handle(ivp_handle);
    if (e) e->roi = *roi_attr;
    pthread_mutex_unlock(&g_ivp_lock);
    return e ? HI_SUCCESS : HI_ERR_IVP_UNEXIST;
}

hi_s32 hi_ivp_get_roi_attr(hi_s32 ivp_handle, hi_ivp_roi_attr *roi_attr)
{
    ivp_entity_t *e;
    if (!roi_attr) return HI_ERR_IVP_NULL_PTR;
    pthread_mutex_lock(&g_ivp_lock);
    e = entity_by_handle(ivp_handle);
    if (e) *roi_attr = e->roi;
    pthread_mutex_unlock(&g_ivp_lock);
    return e ? HI_SUCCESS : HI_ERR_IVP_UNEXIST;
}

hi_s32 hi_ivp_set_roi_map(hi_s32 ivp_handle, const hi_ivp_roi_map *roi_map)
{
    /* The ROI map is data the vendor copies into an IVE buffer for later
     * use by VENC. We don't drive VENC from libivp_neo, so record the
     * call succeeded and move on. */
    ivp_entity_t *e;
    if (!roi_map || !roi_map->mb_map)
        return HI_ERR_IVP_NULL_PTR;
    pthread_mutex_lock(&g_ivp_lock);
    e = entity_by_handle(ivp_handle);
    pthread_mutex_unlock(&g_ivp_lock);
    return e ? HI_SUCCESS : HI_ERR_IVP_UNEXIST;
}

/* --- Inference --- */

hi_s32 hi_ivp_process(hi_s32 ivp_handle,
                      const VIDEO_FRAME_INFO_S *src_frame,
                      hi_bool *obj_alarm)
{
    ivp_entity_t *e;

    if (!src_frame || !obj_alarm)
        return HI_ERR_IVP_NULL_PTR;
    pthread_mutex_lock(&g_ivp_lock);
    e = entity_by_handle(ivp_handle);
    pthread_mutex_unlock(&g_ivp_lock);
    if (!e)
        return HI_ERR_IVP_UNEXIST;

    /* Zero detections — matches vendor on EV200/EV300. */
    *obj_alarm = HI_FALSE;
    return HI_SUCCESS;
}

hi_s32 hi_ivp_process_ex(hi_s32 ivp_handle,
                         const VIDEO_FRAME_INFO_S *src_frame,
                         hi_ivp_obj_array *obj_array)
{
    ivp_entity_t *e;

    if (!src_frame || !obj_array)
        return HI_ERR_IVP_NULL_PTR;
    pthread_mutex_lock(&g_ivp_lock);
    e = entity_by_handle(ivp_handle);
    pthread_mutex_unlock(&g_ivp_lock);
    if (!e)
        return HI_ERR_IVP_UNEXIST;

    /*
     * Clear the output: class_num=0, every class rect_num=0. The caller
     * keeps its pre-allocated `objs` pointer (we don't touch rect_capcity
     * or the objs pointer — caller owns that memory).
     */
    obj_array->class_num = 0;
    for (int c = 0; c < HI_IVP_MAX_CLASS; c++)
        obj_array->obj_class[c].rect_num = 0;

    return HI_SUCCESS;
}
