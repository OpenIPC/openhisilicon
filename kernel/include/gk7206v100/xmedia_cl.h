#ifndef XMEDIA_CL_H
#define XMEDIA_CL_H

#include "xmedia_cl_common.h"

#ifdef __cplusplus
extern "C" {
#endif

xmedia_cl_s32 xmedia_cl_init();
xmedia_cl_s32 xmedia_cl_uninit();

// get the device list
xmedia_cl_s32 xmedia_cl_get_device_ids(xmedia_cl_device_type_e device_type,
                                       xmedia_cl_device_id *devices,
                                       xmedia_cl_u32 *num_devices);

xmedia_cl_s32 xmedia_cl_release_device_ids(xmedia_cl_device_id *devices,
                                           xmedia_cl_u32 *num_devices);

xmedia_cl_context xmedia_cl_create_context(xmedia_cl_u32 num_devices,
                                           xmedia_cl_device_id *devices,
                                           xmedia_cl_s32 *err_code);

xmedia_cl_s32 xmedia_cl_release_context(xmedia_cl_context context);

xmedia_cl_s32 xmedia_cl_wait_for_events(xmedia_cl_u32 num_events,
                                        xmedia_cl_event *events);

xmedia_cl_s32 xmedia_cl_release_event(xmedia_cl_event event);

xmedia_cl_s32 xmedia_cl_query_event_status(xmedia_cl_event event,
                                           xmedia_cl_s32 *status);

xmedia_cl_s32 xmedia_cl_graph_querysize_from_file(const char *model,
                                                  xmedia_cl_u32 *worksize,
                                                  xmedia_cl_u32 *weightsize);
xmedia_cl_s32 xmedia_cl_graph_querysize_from_buff(const char *model,
                                                  xmedia_cl_u32 *worksize,
                                                  xmedia_cl_u32 *weightsize);
xmedia_cl_s32 xmedia_cl_graph_loadmodel_from_file(xmedia_cl_context *context,
                                                  const char *model,
                                                  xmedia_cl_graph *graph);
xmedia_cl_s32 xmedia_cl_graph_loadmodel_from_buff(xmedia_cl_context *context,
                                                  const char *model,
                                                  xmedia_cl_graph *graph);
xmedia_cl_s32 xmedia_cl_set_workspace_addr(xmedia_cl_context context,
                                           void *workspace_addr,
                                           xmedia_cl_u32 worksize);
xmedia_cl_s32 xmedia_cl_get_workspace_addr(xmedia_cl_context context,
                                           void **workspace_addr,
                                           xmedia_cl_u32 *worksize);
xmedia_cl_s32 xmedia_cl_graph_loadmodel_from_file_withmem(
    xmedia_cl_context *context, const char *model, void *workspace,
    xmedia_cl_u32 worksize, void *weight, xmedia_cl_u32 weightsize,
    xmedia_cl_graph *graph);
xmedia_cl_s32 xmedia_cl_graph_loadmodel_from_buff_withmem(
    xmedia_cl_context *context, const char *model, void *workspace,
    xmedia_cl_u32 worksize, void *weight, xmedia_cl_u32 weightsize,
    xmedia_cl_graph *graph);
xmedia_cl_s32 xmedia_cl_graph_get_input(xmedia_cl_graph graph,
                                        xmedia_cl_u32 input_num,
                                        xmedia_cl_tensor_info_inout *input);
xmedia_cl_s32 xmedia_cl_graph_get_output(xmedia_cl_graph graph,
                                         xmedia_cl_u32 output_num,
                                         xmedia_cl_tensor_info_inout *output);
xmedia_cl_s32
xmedia_cl_graph_set_inout(xmedia_cl_graph graph,
                          const xmedia_cl_tensor_info_inout *input,
                          const xmedia_cl_tensor_info_inout *output);
xmedia_cl_s32 xmedia_cl_graph_process(xmedia_cl_graph graph);
xmedia_cl_s32 xmedia_cl_graph_submit(xmedia_cl_graph graph,
                                     xmedia_cl_event *event);
xmedia_cl_s32 xmedia_cl_graph_unload(xmedia_cl_graph graph);

xmedia_cl_s32 xmedia_cl_graph_set_compress_config(xmedia_cl_graph graph,
                                                  void *device_head_addr,
                                                  xmedia_cl_u32 head_size,
                                                  xmedia_cl_u32 break_line);

xmedia_cl_s32 xmedia_cl_graph_communicate(xmedia_cl_graph graph,
                                          xmedia_cl_u32 *model_id);

xmedia_cl_s32 xmedia_cl_graph_decommunicate(xmedia_cl_graph graph,
                                            xmedia_cl_u32 model_id);
/*
   目前任务优先级定义四个，从0~3，优先级依次升高，具体见宏定义:
        XMEDIA_CL_JOB_SCHEDULE_PRIO_MIN
        XMEDIA_CL_JOB_SCHEDULE_PRIO_MEDIUM
        XMEDIA_CL_JOB_SCHEDULE_PRIO_HIGH
        XMEDIA_CL_JOB_SCHEDULE_PRIO_MAX
*/
xmedia_cl_s32 xmedia_cl_graph_set_schedule_prio(xmedia_cl_graph graph,
                                                xmedia_cl_u32 prio);

xmedia_cl_s32 xmedia_cl_set_event_callback(
    xmedia_cl_event event, xmedia_cl_s32 status,
    void (*notify)(xmedia_cl_event, xmedia_cl_s32, void *), void *data);

xmedia_cl_s32
xmedia_cl_set_profiling(xmedia_cl_graph graph,
                        xmedia_cl_profiling_params profiling_params);

xmedia_cl_s32 xmedia_cl_graph_set_dynamic_batch_size(
    xmedia_cl_graph graph, xmedia_cl_tensor_info_inout *input,
    xmedia_cl_u32 index, xmedia_cl_u32 batch_size);

xmedia_cl_s32 xmedia_cl_graph_get_private_data(xmedia_cl_graph graph,
                                               void **private_data,
                                               xmedia_cl_u32 *data_size);
xmedia_cl_s32
xmedia_cl_graph_get_memory_reuse_type(xmedia_cl_graph graph,
                                      xmedia_cl_memory_reuse_type *memory_type);

xmedia_cl_s32 xmedia_cl_graph_get_private_data_from_file(
    const char *model, void *private_data, xmedia_cl_u32 data_size);
xmedia_cl_s32 xmedia_cl_graph_get_private_data_from_buff(
    const char *model, void *private_data, xmedia_cl_u32 data_size);
xmedia_cl_s32
xmedia_cl_graph_query_model_info_from_file(const char *model, void *info,
                                           xmedia_cl_model_info_type info_type);
xmedia_cl_s32
xmedia_cl_graph_query_model_info_from_buff(const char *model, void *info,
                                           xmedia_cl_model_info_type info_type);
#ifdef __cplusplus
}
#endif

#endif
