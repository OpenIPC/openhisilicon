#ifndef MPI_CL_H
#define MPI_CL_H

#include "xmedia_cl.h"

#ifdef __cplusplus
extern "C" {
#endif

xmedia_cl_s32 mpi_cl_init();
xmedia_cl_s32 mpi_cl_uninit();

// get the device list
xmedia_cl_s32 mpi_cl_get_device_ids(xmedia_cl_device_type_e device_type,
                                    xmedia_cl_device_id *devices,
                                    xmedia_cl_u32 *num_devices);

xmedia_cl_s32 mpi_cl_release_device_ids(xmedia_cl_device_id *devices,
                                        xmedia_cl_u32 *num_devices);

xmedia_cl_context mpi_cl_create_context(xmedia_cl_u32 num_devices,
                                        xmedia_cl_device_id *devices,
                                        xmedia_cl_s32 *err_code);

xmedia_cl_s32 mpi_cl_release_context(xmedia_cl_context context);

xmedia_cl_s32 mpi_cl_graph_querysize_from_file(const char *model,
                                               xmedia_cl_u32 *worksize,
                                               xmedia_cl_u32 *weightsize);
xmedia_cl_s32 mpi_cl_graph_querysize_from_buff(const char *model,
                                               xmedia_cl_u32 *worksize,
                                               xmedia_cl_u32 *weightsize);
xmedia_cl_s32 mpi_cl_graph_loadmodel_from_file(xmedia_cl_context *context,
                                               const char *model,
                                               xmedia_cl_graph *graph);
xmedia_cl_s32 mpi_cl_graph_loadmodel_from_buff(xmedia_cl_context *context,
                                               const char *model,
                                               xmedia_cl_graph *graph);
xmedia_cl_s32 mpi_cl_graph_loadmodel_from_file_withmem(
    xmedia_cl_context *context, const char *model, void *workspace,
    xmedia_cl_u32 worksize, void *weight, xmedia_cl_u32 weightsize,
    xmedia_cl_graph *graph);

xmedia_cl_s32 mpi_cl_graph_loadmodel_from_buff_withmem(
    xmedia_cl_context *context, const char *model, void *workspace,
    xmedia_cl_u32 worksize, void *weight, xmedia_cl_u32 weightsize,
    xmedia_cl_graph *graph);

xmedia_cl_s32 mpi_cl_graph_unload(xmedia_cl_graph graph);

xmedia_cl_s32 mpi_cl_graph_communicate(xmedia_cl_graph graph,
                                       xmedia_cl_u32 *model_id);

xmedia_cl_s32 mpi_cl_graph_decommunicate(xmedia_cl_graph graph,
                                         xmedia_cl_u32 model_id);

xmedia_cl_s32 mpi_cl_set_workspace_addr(xmedia_cl_context context,
                                        void *workspace_addr,
                                        xmedia_cl_u32 worksize);

xmedia_cl_s32 mpi_cl_get_workspace_addr(xmedia_cl_context context,
                                        void **workspace_addr,
                                        xmedia_cl_u32 *worksize);

xmedia_cl_s32 mpi_cl_graph_get_private_data(xmedia_cl_graph graph,
                                            void **private_data,
                                            xmedia_cl_u32 *data_size);

xmedia_cl_s32 mpi_cl_graph_get_header_crc_from_buff(const char *model,
                                                    xmedia_cl_u32 *crc);

xmedia_cl_s32 mpi_cl_graph_get_header_crc_from_file(const char *model,
                                                    xmedia_cl_u32 *crc);

xmedia_cl_s32 mpi_cl_graph_get_private_data_from_file(const char *model,
                                                      void *private_data,
                                                      xmedia_cl_u32 data_size);
xmedia_cl_s32 mpi_cl_graph_get_private_data_from_buff(const char *model,
                                                      void *private_data,
                                                      xmedia_cl_u32 data_size);
xmedia_cl_s32
mpi_cl_graph_query_model_info_from_file(const char *model, void *model_info,
                                        xmedia_cl_model_info_type info_type);

xmedia_cl_s32
mpi_cl_graph_query_model_info_from_buff(const char *model, void *model_info,
                                        xmedia_cl_model_info_type info_type);
#ifdef __cplusplus
}
#endif

#endif
