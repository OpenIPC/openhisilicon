#ifndef __SVP_CORE_H__
#define __SVP_CORE_H__

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "xmedia_cl.h"
#include "xmedia_svp_std.h"
#include "xmedia_sys.h"
#include "xmedia_tde.h"
#include "xmedia_ive.h"

#ifdef __cplusplus
extern "C" {
#endif

#define INPUT_MODEL_NUM_RGB888  1
#define INPUT_MODEL_NUM_YUV420  2
#define SVP_ALIGN_BYTE 16
#define SVP_ALIGN_UP(A, ALIGN) \
    ((((A) % (ALIGN)) == 0) ? (A) : ((A) + (ALIGN) - ((A) % (ALIGN))))

#define ALIGN_BYTE 8
#define DEFAULT_INOUT_NUM 16
#define ALIGN_FUNC(A, ALIGN) \
    ((((A) % (ALIGN)) == 0) ? (A) : ((A) + (ALIGN) - ((A) % (ALIGN))))

typedef struct _xmedia_npu_model {
    xmedia_cl_graph graph;
    xmedia_cl_device_id *devices;
    xmedia_cl_context context;
    xmedia_s32 *output_size;
    xmedia_cl_tensor_info_inout output;       // 模型输出相关数据信息
    xmedia_cl_tensor_info_inout input;        // 模型输入相关数据信息
    xmedia_u64 weight_u64phyaddr;
    xmedia_void* weight_viraddr;
    xmedia_s32 *user_count;
    xmedia_void *rgb_viraddr;
    xmedia_u64 rgb_phyaddr;
    xmedia_u64 output_u64phyaddr;
    xmedia_void* output_viraddr;
} xmedia_npu_model;

typedef struct {
    xmedia_u32 size;
    xmedia_bool reuse_flag;
    xmedia_u64 phyaddr;
    xmedia_void *viraddr;
} svp_mem_config;

typedef struct {
    xmedia_svp_mem_reuse_type reuse_type;
    svp_mem_config workbuf_cfg;
    svp_mem_config inputbuf_cfg;
    svp_mem_config outputbuf_cfg;
} svp_config;

xmedia_s32 npu_init_context();
xmedia_s32 svp_set_cfg(xmedia_svp_cfg *cfg);
xmedia_s32 svp_get_cfg(xmedia_svp_cfg *cfg);
xmedia_s32 npu_load_model(const xmedia_svp_modules *model, xmedia_npu_model *xmd_npu_model);
xmedia_s32 npu_run_model(xmedia_npu_model *xmd_npu_model, xmedia_u64 img_src_phy);
xmedia_s32 npu_unload_model(xmedia_npu_model *xmd_npu_model);
xmedia_s32 npu_release_context();
extern xmedia_s32 check_chip_otp();

#ifdef __cplusplus
}
#endif

#endif
