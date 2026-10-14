#include "svp_core.h"

//#define SVP_DEBUG_SAVE_FILE 1
//#define SVP_TIME_DEBUG 1

#ifdef SVP_TIME_DEBUG
#include <sys/time.h>
static struct timeval g_stTimeCostStart;
static struct timeval g_stTimeCostEnd;

#define TIME_COST_START()   do {    \
    gettimeofday(&g_stTimeCostStart,NULL); \
} while(0)

#define TIME_COST_END()   do {    \
        gettimeofday(&g_stTimeCostEnd,NULL); \
} while(0)

#define TIME_COST_PRINT(info)   do { \
    xmedia_u64 u64TimeUseMs = (g_stTimeCostEnd.tv_sec - g_stTimeCostStart.tv_sec) * 1000000 \
                      + (g_stTimeCostEnd.tv_usec - g_stTimeCostStart.tv_usec);   \
    printf("%s cost time:%llu us\n",info,u64TimeUseMs);  \
} while(0)
#endif

xmedia_cl_device_id* g_devices = NULL;
xmedia_u32 g_num_devices = 0;
static xmedia_cl_context g_context;
static svp_config g_svp_cfg = {0};

// 内存复用相关参数
static xmedia_u32 max_worksize = 0;
static xmedia_u64 work_phyaddr;
static xmedia_void *work_viraddr;
static xmedia_u32 model_num = 0;
static xmedia_bool npu_work_local_flag = XMEDIA_FALSE;
//static xmedia_u32 max_rgbsize = 0;
//static xmedia_u64 rgb_phyaddr;
//static xmedia_void *rgb_viraddr; // 待cl接口支持查询input、output


static unsigned int common_get_type_get_bytes(xmedia_cl_data_type dtype)
{
    switch (dtype) {
        case XMEDIA_CL_INT8:
        case XMEDIA_CL_UINT8:
            return 1;
        case XMEDIA_CL_INT16:
        case XMEDIA_CL_UINT16:
        case XMEDIA_CL_FP16:
            return 2;
        case XMEDIA_CL_INT32:
        case XMEDIA_CL_FP32:
            return 4;
        default:
            return 0;
    }
}

static xmedia_s32 malloc_inout_tensor_mem(xmedia_cl_tensor_info_inout *inout, xmedia_u32 num)
{
    inout->tensor = (xmedia_cl_tensor *)malloc(sizeof(xmedia_cl_tensor) * num);
    if (inout->tensor == NULL) {
        SVP_TRACE(MODULE_DBG_NOTICE, " malloc_inout_tensor_mem err,malloc failed!\n");
        return XMEDIA_FAILURE;
    }

    inout->tensor_batch = (xmedia_cl_tensor_batch *)malloc(sizeof(xmedia_cl_tensor_batch) * num);
    if (inout->tensor_batch == NULL) {
        CFREE(inout->tensor);
        SVP_TRACE(MODULE_DBG_NOTICE, " malloc_inout_tensor_batch_mem err,malloc failed!\n");
        return XMEDIA_FAILURE;
    }

    inout->current_batch = (xmedia_cl_u32 *)malloc(sizeof(xmedia_cl_u32) * num);
    if (inout->current_batch == NULL) {
        CFREE(inout->tensor);
        CFREE(inout->tensor_batch);
        SVP_TRACE(MODULE_DBG_NOTICE, " malloc_inout_current_batch_mem err,malloc failed!\n");
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}

static void free_inout_tensor_mem(xmedia_cl_tensor_info_inout *inout)
{
    if (inout != NULL) {
            CFREE(inout->tensor);
            CFREE(inout->tensor_batch);
            CFREE(inout->current_batch);
    }
}

xmedia_s32 npu_init_context()
{
    xmedia_s32 ret;
    xmedia_s32 err_code;
    xmedia_u32 num_devices = 0;

    ret = xmedia_cl_init();
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "xmedia_cl_init err, errno %d\n", ret);
        return XMEDIA_FAILURE;
    }
    ret = xmedia_cl_get_device_ids(XMEDIA_CL_DEVICE_ALL, NULL, &num_devices);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "xmedia_cl_get_device_ids err, errno %d\n", ret);
        return XMEDIA_FAILURE;
    }

    if (num_devices <= 0) {
        SVP_TRACE(MODULE_DBG_ERR,"get devices failed !\n");
        return XMEDIA_FAILURE;
    }

    g_devices = (xmedia_cl_device_id*)calloc(num_devices, sizeof(xmedia_cl_device_id));
    if (g_devices == NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "g_device calloc error \n");
        return XMEDIA_FAILURE;
    }

    ret = xmedia_cl_get_device_ids(XMEDIA_CL_DEVICE_ALL, g_devices, &num_devices);
    if (ret != XMEDIA_SUCCESS) {
        CFREE(g_devices);
        SVP_TRACE(MODULE_DBG_ERR, "xmedia_cl_get_device_id err, errno %d\n", ret);
        return XMEDIA_FAILURE;
    }
    g_num_devices = num_devices;

    g_context = xmedia_cl_create_context(g_num_devices, g_devices, &err_code);
    if ((err_code != XMEDIA_SUCCESS) || (g_context == NULL)) {
        xmedia_cl_release_device_ids(g_devices, &g_num_devices);
        g_num_devices = 0;
        CFREE(g_devices);
        SVP_TRACE(MODULE_DBG_ERR,"create context failed !\n");
        return XMEDIA_FAILURE;
    }
    ret = xmedia_cl_get_workspace_addr(g_context, (xmedia_void **)&work_viraddr, &max_worksize);
    if (ret != XMEDIA_SUCCESS) {
        work_viraddr = XMEDIA_NULL;
        max_worksize = 0;
    }
    max_worksize = 0;
    g_svp_cfg.reuse_type = XMEDIA_SVP_MEM_TYPE_MAX;
    g_svp_cfg.workbuf_cfg.reuse_flag = XMEDIA_FALSE;
    g_svp_cfg.inputbuf_cfg.reuse_flag = XMEDIA_FALSE;
    g_svp_cfg.outputbuf_cfg.reuse_flag = XMEDIA_FALSE;

    return XMEDIA_SUCCESS;
}
/*
desc: release context and device
*/
xmedia_s32 npu_release_context()
{
    if (g_devices == NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "ERR:npu_release_context npu devices is NULL !\n");
        return XMEDIA_FAILURE;
    }
    xmedia_cl_release_context(g_context);
    xmedia_cl_release_device_ids(g_devices, &g_num_devices);
    g_num_devices = 0;
    CFREE(g_devices);
    xmedia_cl_uninit();

    return XMEDIA_SUCCESS;
}

xmedia_s32 svp_get_cfg(xmedia_svp_cfg* svp_cfg)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    if (svp_cfg == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "ERR: svp cfg param is NULL !\n");
        return XMEDIA_FAILURE;
    }

    /* get svp mem reuse type */
    svp_cfg->reuse_type = g_svp_cfg.reuse_type;

    /* get svp workspace mem cfg */
    svp_cfg->workbuf_reuse_mem.size = g_svp_cfg.workbuf_cfg.size;
    svp_cfg->workbuf_reuse_mem.phyaddr = g_svp_cfg.workbuf_cfg.phyaddr;
    svp_cfg->workbuf_reuse_mem.viraddr = g_svp_cfg.workbuf_cfg.viraddr;

    /* get svp mdoel input mem cfg */
    svp_cfg->input_reuse_mem.size = g_svp_cfg.inputbuf_cfg.size;
    svp_cfg->input_reuse_mem.phyaddr = g_svp_cfg.inputbuf_cfg.phyaddr;
    svp_cfg->input_reuse_mem.viraddr = g_svp_cfg.inputbuf_cfg.viraddr;

    /* get svp mdoel output mem cfg */
    svp_cfg->output_reuse_mem.size = g_svp_cfg.outputbuf_cfg.size;
    svp_cfg->output_reuse_mem.phyaddr = g_svp_cfg.outputbuf_cfg.phyaddr;
    svp_cfg->output_reuse_mem.viraddr = g_svp_cfg.outputbuf_cfg.viraddr;

    return ret;
}

xmedia_s32 svp_set_cfg(xmedia_svp_cfg* svp_cfg)
{
    xmedia_s32 ret = XMEDIA_SUCCESS;

    if (svp_cfg == XMEDIA_NULL) {
        SVP_TRACE(MODULE_DBG_ERR, "ERR: svp cfg param is NULL !\n");
        return XMEDIA_FAILURE;
    }

    /* set svp mem reuse type */
    g_svp_cfg.reuse_type = svp_cfg->reuse_type;
    if (g_svp_cfg.reuse_type == XMEDIA_SVP_MEM_TYPE_COMPLETE) {
        SVP_TRACE(MODULE_DBG_ERR, "ERR: XMEDIA_SVP_MEM_TYPE_COMPLETE not support !\n");
        return XMEDIA_FAILURE;
    } else if (g_svp_cfg.reuse_type == XMEDIA_SVP_MEM_TYPE_MAX) {
        if (svp_cfg->workbuf_reuse_mem.viraddr != XMEDIA_NULL ||
            svp_cfg->input_reuse_mem.viraddr != XMEDIA_NULL ||
            svp_cfg->output_reuse_mem.viraddr != XMEDIA_NULL) {
            SVP_TRACE(MODULE_DBG_ERR, "ERR: XMEDIA_SVP_MEM_TYPE_MAX svp_cfg can not be set !\n");
            return XMEDIA_FAILURE;
        }
    } else {
        /* set svp workspace mem cfg */
        if ((svp_cfg->workbuf_reuse_mem.viraddr != XMEDIA_NULL)
            && (g_svp_cfg.workbuf_cfg.reuse_flag != XMEDIA_TRUE)) {
            g_svp_cfg.workbuf_cfg.reuse_flag = XMEDIA_TRUE;
            g_svp_cfg.workbuf_cfg.size = svp_cfg->workbuf_reuse_mem.size;
            g_svp_cfg.workbuf_cfg.phyaddr = svp_cfg->workbuf_reuse_mem.phyaddr;
            g_svp_cfg.workbuf_cfg.viraddr = svp_cfg->workbuf_reuse_mem.viraddr;
            max_worksize = g_svp_cfg.workbuf_cfg.size;
            work_phyaddr = g_svp_cfg.workbuf_cfg.phyaddr;
            work_viraddr = g_svp_cfg.workbuf_cfg.viraddr;
        } else {
            SVP_TRACE(MODULE_DBG_ERR, "ERR: set workbuf_cfg failed !\n");
            return XMEDIA_FAILURE;
        }

        /* set svp mdoel input mem cfg */
        if ((svp_cfg->input_reuse_mem.viraddr != XMEDIA_NULL)
            && (g_svp_cfg.inputbuf_cfg.reuse_flag != XMEDIA_TRUE)) {
            g_svp_cfg.inputbuf_cfg.reuse_flag = XMEDIA_TRUE;
            g_svp_cfg.inputbuf_cfg.size = svp_cfg->input_reuse_mem.size;
            g_svp_cfg.inputbuf_cfg.phyaddr = svp_cfg->input_reuse_mem.phyaddr;
            g_svp_cfg.inputbuf_cfg.viraddr = svp_cfg->input_reuse_mem.viraddr;
        } else {
            SVP_TRACE(MODULE_DBG_ERR, "ERR: set inputbuf_cfg failed !\n");
            return XMEDIA_FAILURE;
        }

        /* set svp mdoel output mem cfg */
        if ((svp_cfg->output_reuse_mem.viraddr != XMEDIA_NULL)
            && (g_svp_cfg.outputbuf_cfg.reuse_flag != XMEDIA_TRUE)) {
            g_svp_cfg.outputbuf_cfg.reuse_flag = XMEDIA_TRUE;
            g_svp_cfg.outputbuf_cfg.size = svp_cfg->output_reuse_mem.size;
            g_svp_cfg.outputbuf_cfg.phyaddr = svp_cfg->output_reuse_mem.phyaddr;
            g_svp_cfg.outputbuf_cfg.viraddr = svp_cfg->output_reuse_mem.viraddr;
        } else {
            SVP_TRACE(MODULE_DBG_ERR, "ERR: set outputbuf_cfg failed !\n");
            return XMEDIA_FAILURE;
        }
    }

    return ret;
}

/*
@model_path : model file path
@xdm_npu_model: save model's graph ,input ,output information.
*/
xmedia_s32 npu_load_model(const xmedia_svp_modules *model, xmedia_npu_model* xmd_npu_model)
{
    xmedia_s32 ret, i;
    xmedia_u32 worksize = 0, weightsize = 0;
    xmedia_u32 size;

    if ((model == NULL) || (xmd_npu_model == NULL)) {
        SVP_TRACE(MODULE_DBG_ERR, "ERR: load model param is NULL !\n");
        return XMEDIA_FAILURE;
    }
    if (model->load_mode == XMEDIA_SVP_MODEL_FILE) {
        ret = xmedia_cl_graph_querysize_from_file(model->pathname, &worksize, &weightsize);
        if (XMEDIA_SUCCESS != ret) {
            SVP_TRACE(MODULE_DBG_ERR, "xmedia_cl_graph_querysize_from_file [%s], errno %d\n",
                                     model->pathname, ret);
            return XMEDIA_FAILURE;
        }
    } else if (model->load_mode == XMEDIA_SVP_MODEL_MEM) {
        if (model->buf == XMEDIA_NULL) {
            SVP_TRACE(MODULE_DBG_ERR, "mem type model input buffer is null !\n");
            return XMEDIA_FAILURE;
        }
        ret = xmedia_cl_graph_querysize_from_buff((xmedia_char *)model->buf, &worksize, &weightsize);
        if (XMEDIA_SUCCESS != ret) {
            SVP_TRACE(MODULE_DBG_ERR, "xmedia_cl_graph_querysize_from_buff, errno %d\n", ret);
            return XMEDIA_FAILURE;
        }
    }
    if (g_svp_cfg.reuse_type == XMEDIA_SVP_MEM_TYPE_AINR_SHARE) {
        if (max_worksize == 0) {
            ret = xmedia_cl_get_workspace_addr(g_context, (xmedia_void **)&work_viraddr, &max_worksize);
            if ((ret != XMEDIA_SUCCESS) || (max_worksize < worksize)) {
                // 外部设置，用户态init sys后设置的需考虑ainr和svp要用的所有模型
                SVP_TRACE(MODULE_DBG_ERR, "get cl worksize mem not enough, max %d  cur %d\n", max_worksize, worksize);
            }
        } else {
            if (worksize > max_worksize) {
                SVP_TRACE(MODULE_DBG_ERR, "worksize mem not enough, max %d  cur %d\n", max_worksize, worksize);
                return XMEDIA_FAILURE;
            }
        }
    } else if (g_svp_cfg.reuse_type == XMEDIA_SVP_MEM_TYPE_BLOCK) {
        if (g_svp_cfg.workbuf_cfg.reuse_flag == XMEDIA_TRUE) {
            if (worksize > max_worksize) {
                SVP_TRACE(MODULE_DBG_ERR, "worksize mem not enough, max %d  cur %d\n", max_worksize, worksize);
                return XMEDIA_FAILURE;
            }
        } else {
            SVP_TRACE(MODULE_DBG_ERR, "set svp workbuf_cfg reuse_flag abnormally!\n");
            return XMEDIA_FAILURE;
        }
    } else if (g_svp_cfg.reuse_type == XMEDIA_SVP_MEM_TYPE_MAX) {
        // npu work内存确认，由外部申请时size根据系统使用最大的模型决定由外部释放，内部申请时model_num标志位等于0时释放
        if (max_worksize == 0) {
            ret = xmedia_cl_get_workspace_addr(g_context, (xmedia_void **)&work_viraddr, &max_worksize);
            if ((ret == XMEDIA_SUCCESS) && (max_worksize < worksize)) {
                // 外部设置，用户态init sys后设置的需考虑ainr和svp要用的所有模型
                SVP_TRACE(MODULE_DBG_ERR,"get cl worksize mem not enough, max %d  cur %d\n", max_worksize, worksize);
            } else if (ret != XMEDIA_SUCCESS) {
                max_worksize = worksize;
                ret = svp_mmz_alloc_and_map_cache(XMEDIA_NULL, "npu_work_mem", &work_phyaddr, (xmedia_void **)&work_viraddr, max_worksize);
                if (ret != XMEDIA_SUCCESS) {
                    SVP_TRACE(MODULE_DBG_ERR,"svp_mmz_alloc_and_map_cache, errno %d\n", ret);
                    return XMEDIA_FAILURE;
                }
                npu_work_local_flag = XMEDIA_TRUE;
#if 0
                // 暂不考虑ainr使用的workbuf 小于svp， 且ainr在svp之后启动的情况
                ret = xmedia_cl_set_workspace_addr(g_context, work_viraddr, max_worksize);
                if (ret != XMEDIA_SUCCESS) {
                    SVP_TRACE(MODULE_DBG_ERR,"cl_set_workspace, errno %d\n", ret);
                    return XMEDIA_FAILURE;
                }
#endif
            }
        } else if (worksize > max_worksize) {
            SVP_TRACE(MODULE_DBG_ERR,"worksize mem not enough, max %d  cur %d\n", max_worksize, worksize);
            return XMEDIA_FAILURE;
        }
    }

    // npu weight内存申请，每个模型独立
    ret = svp_mmz_alloc_and_map_cache(XMEDIA_NULL, "svp_weight_mem", &(xmd_npu_model->weight_u64phyaddr),
                                (xmedia_void **)(&(xmd_npu_model->weight_viraddr)), weightsize);
    if (XMEDIA_SUCCESS != ret) {
        SVP_TRACE(MODULE_DBG_ERR,"svp_mmz_alloc_and_map_cache size %d errno %d\n", ret, weightsize);
        goto LOAD_MODEL_ERR0;
    }

    // 模型加载
    if (model->load_mode == XMEDIA_SVP_MODEL_FILE) {
        SVP_TRACE(MODULE_DBG_NOTICE,"[svp info]:load model pathname:%s \n", model->pathname);
        ret = xmedia_cl_graph_loadmodel_from_file_withmem(&g_context, model->pathname, work_viraddr,
                                                          worksize, xmd_npu_model->weight_viraddr,
                                                          weightsize, &xmd_npu_model->graph);
        if ((ret != XMEDIA_SUCCESS) || (xmd_npu_model->graph == NULL)) {
            SVP_TRACE(MODULE_DBG_ERR, "xmedia_cl_graph_loadmodel file err, errno %d \n", ret);
            goto LOAD_MODEL_ERR1;
        }
    } else if (model->load_mode == XMEDIA_SVP_MODEL_MEM) {
        ret = xmedia_cl_graph_loadmodel_from_buff_withmem(&g_context, (xmedia_char *)model->buf, work_viraddr,
                                                          worksize, xmd_npu_model->weight_viraddr,
                                                          weightsize, &xmd_npu_model->graph);
        if (ret != XMEDIA_SUCCESS) {
            SVP_TRACE(MODULE_DBG_ERR, "xmedia_cl_graph_loadmodel mem err, errno %d \n", ret);
            goto LOAD_MODEL_ERR1;
        }
    } else {
        SVP_TRACE(MODULE_DBG_ERR, "model load type, load errno %d\n", model->load_mode);
        goto LOAD_MODEL_ERR1;
    }

    // 获取模型input信息
    xmd_npu_model->input.num = 0; // xmedia_cl_graph_get_input 第二个参数为0时，会在第三个参数中将其赋值
    ret = xmedia_cl_graph_get_input(xmd_npu_model->graph, xmd_npu_model->input.num, &xmd_npu_model->input);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "xmedia_cl_graph_get_input err, errno %d\n", ret);
        goto LOAD_MODEL_ERR1;
    }
    if (xmd_npu_model->input.num == INPUT_MODEL_NUM_RGB888) {
        if (model->format != XMEDIA_SVP_INPUTDATA_FORMAT_RGB888) {
            SVP_TRACE(MODULE_DBG_ERR, "ERR: model input chnl:%d does not match format:%d \n",
                      xmd_npu_model->input.num,model->format);
            goto LOAD_MODEL_ERR1;
        }
    } else if (xmd_npu_model->input.num == INPUT_MODEL_NUM_YUV420) {
        if (model->format != XMEDIA_SVP_INPUTDATA_FORMAT_YUV420SP) {
            SVP_TRACE(MODULE_DBG_ERR, "ERR: model input chnl:%d does not match format:%d \n",
                      xmd_npu_model->input.num, model->format);
            goto LOAD_MODEL_ERR1;
        }
    } else {
        SVP_TRACE(MODULE_DBG_ERR, "ERR: model input abnormal:%d \n", xmd_npu_model->input.num);
        goto LOAD_MODEL_ERR1;
    }
    ret = malloc_inout_tensor_mem(&xmd_npu_model->input, xmd_npu_model->input.num);
    if (ret != XMEDIA_SUCCESS) {
        goto LOAD_MODEL_ERR1;
    }
    ret = xmedia_cl_graph_get_input(xmd_npu_model->graph, xmd_npu_model->input.num, &xmd_npu_model->input);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "xmedia_cl_graph_get_input err, errno %d\n", ret);
        goto LOAD_MODEL_ERR2;
    }

    // 若复用input内存，则直接赋地址；若不复用输入内存，则分别设置各模型的输入
    if (g_svp_cfg.reuse_type == XMEDIA_SVP_MEM_TYPE_BLOCK || g_svp_cfg.reuse_type == XMEDIA_SVP_MEM_TYPE_AINR_SHARE) {
        if (xmd_npu_model->input.tensor[0].size > g_svp_cfg.inputbuf_cfg.size) {
            SVP_TRACE(MODULE_DBG_ERR, "input_mem size is not enough, max %d  cur %d\n", g_svp_cfg.inputbuf_cfg.size,
                    xmd_npu_model->input.tensor[0].size);
            goto LOAD_MODEL_ERR2;
        }
        xmd_npu_model->rgb_phyaddr = g_svp_cfg.inputbuf_cfg.phyaddr;
        xmd_npu_model->rgb_viraddr = g_svp_cfg.inputbuf_cfg.viraddr;
    } else if (g_svp_cfg.reuse_type == XMEDIA_SVP_MEM_TYPE_MAX) {
        ret = svp_mmz_alloc_and_map(XMEDIA_NULL, "svp_rgb_mem", &(xmd_npu_model->rgb_phyaddr),
                            (xmedia_void **)(&(xmd_npu_model->rgb_viraddr)), xmd_npu_model->input.tensor[0].size);
        if (XMEDIA_SUCCESS != ret) {
            SVP_TRACE(MODULE_DBG_ERR,"svp_mmz_alloc_and_map size %d errno\n", ret);
            goto LOAD_MODEL_ERR2;
        }
    }

    xmd_npu_model->output.num = 0;  // xmedia_cl_graph_get_output 第二个参数为0时，会在第三个参数中将其赋值
    ret = xmedia_cl_graph_get_output(xmd_npu_model->graph, xmd_npu_model->output.num, &xmd_npu_model->output);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "xmedia_cl_graph_get_output err, errno %d\n", ret);
        goto LOAD_MODEL_ERR3;
    }
    xmd_npu_model->output_size  = (xmedia_cl_s32 *)malloc(sizeof(xmedia_cl_s32) * xmd_npu_model->output.num);
    if (xmd_npu_model->output_size == NULL) {
        goto LOAD_MODEL_ERR3;
    }
    ret = malloc_inout_tensor_mem(&xmd_npu_model->output, xmd_npu_model->output.num);
    if (ret != XMEDIA_SUCCESS) {
        goto LOAD_MODEL_ERR4;
    }

    ret = xmedia_cl_graph_get_output(xmd_npu_model->graph, xmd_npu_model->output.num, &xmd_npu_model->output);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "xmedia_cl_graph_get_output err, errno %d\n", ret);
        goto LOAD_MODEL_ERR5;
    }
    xmedia_u32 outputsizesum = 0;
    for (i = 0; i < xmd_npu_model->output.num; i++) {
        size = common_get_type_get_bytes(xmd_npu_model->output.tensor[i].shape.type) *
                                         xmd_npu_model->output.tensor[i].shape.dims[0] *
                                         xmd_npu_model->output.tensor[i].shape.dims[1] *
                                         xmd_npu_model->output.tensor[i].shape.pch[1];
        if (size <= 0) {
            SVP_TRACE(MODULE_DBG_ERR, "model input size err !\n");
            ret = XMEDIA_FAILURE;
            goto LOAD_MODEL_ERR5;

        }
        xmd_npu_model->output_size[i] = size;
        outputsizesum += size;
    }

    // 若不复用输出内存，则分别设置各模型的输出
    if (g_svp_cfg.reuse_type == XMEDIA_SVP_MEM_TYPE_BLOCK || g_svp_cfg.reuse_type == XMEDIA_SVP_MEM_TYPE_AINR_SHARE) {
        if (outputsizesum > g_svp_cfg.outputbuf_cfg.size) {
            SVP_TRACE(MODULE_DBG_ERR, "output_mem size is not enough, max %d  cur %d\n",
                      g_svp_cfg.outputbuf_cfg.size, outputsizesum);
            goto LOAD_MODEL_ERR5;
        }
        xmd_npu_model->output_u64phyaddr = g_svp_cfg.outputbuf_cfg.phyaddr;
        xmd_npu_model->output_viraddr = g_svp_cfg.outputbuf_cfg.viraddr;
    } else if (g_svp_cfg.reuse_type == XMEDIA_SVP_MEM_TYPE_MAX) {
        ret = svp_mmz_alloc_and_map_cache(XMEDIA_NULL, "npu_output", &(xmd_npu_model->output_u64phyaddr),
                                (xmedia_void **)(&xmd_npu_model->output_viraddr), outputsizesum);
        if (XMEDIA_CL_SUCCESS != ret) {
            SVP_TRACE(MODULE_DBG_ERR, "npu out svp_mmz_alloc_and_map_cache failed !\n");
            goto LOAD_MODEL_ERR5;
        }
    }

    for (i = 0; i < xmd_npu_model->output.num; i++) {
        if (i > 0) {
            xmd_npu_model->output.tensor[i].addr = xmd_npu_model->output.tensor[i - 1].addr +
                                    ALIGN_FUNC(xmd_npu_model->output_size[i - 1], ALIGN_BYTE); 
                                    // 对齐方式不同芯片有差异，影响输出数据，如aov output tensor 大小为3时
        } else {
            xmd_npu_model->output.tensor[i].addr = xmd_npu_model->output_viraddr;
        }
    }

    xmd_npu_model->user_count  = (xmedia_s32 *)malloc(sizeof(xmedia_s32));
    *xmd_npu_model->user_count = 0;
    model_num++; // 累计模型加载数量用于判断是否释放workbuf、rgbbuf

    return XMEDIA_SUCCESS;

LOAD_MODEL_ERR5:
    free_inout_tensor_mem(&xmd_npu_model->output);
LOAD_MODEL_ERR4:
    CFREE(xmd_npu_model->output_size);
LOAD_MODEL_ERR3:
    if (g_svp_cfg.inputbuf_cfg.reuse_flag != XMEDIA_TRUE) {
        svp_mmz_unmap_and_free(xmd_npu_model->rgb_phyaddr, xmd_npu_model->rgb_viraddr);
    } else if (model_num == 0) {
        g_svp_cfg.inputbuf_cfg.size = 0;
        g_svp_cfg.inputbuf_cfg.reuse_flag = XMEDIA_FALSE;
    }
LOAD_MODEL_ERR2:
    free_inout_tensor_mem(&xmd_npu_model->input);
LOAD_MODEL_ERR1:
    svp_mmz_unmap_and_free(xmd_npu_model->weight_u64phyaddr, (xmedia_void *)xmd_npu_model->weight_viraddr);
LOAD_MODEL_ERR0:
    if (npu_work_local_flag == XMEDIA_TRUE) {
        svp_mmz_unmap_and_free(work_phyaddr, work_viraddr);
        max_worksize = 0;
        npu_work_local_flag = XMEDIA_FALSE;
    } else if (g_svp_cfg.workbuf_cfg.reuse_flag == XMEDIA_TRUE) {
        g_svp_cfg.workbuf_cfg.size = 0;
        g_svp_cfg.workbuf_cfg.reuse_flag = XMEDIA_FALSE;
    }
    xmedia_cl_graph_unload(xmd_npu_model->graph);
    xmd_npu_model->graph = NULL;

    return XMEDIA_FAILURE;
}

xmedia_s32 svp_ive_yuv2rgb(xmedia_npu_model* xmd_npu_model,xmedia_u64 img_src_phy,xmedia_u64 img_src_rgb_phy)
{
    xmedia_s32 ret;
    xmedia_s32 handle = -1;
    xmedia_bool b_finish = XMEDIA_FALSE;
    xmedia_ive_csc_ctrl_s csc_ctrl;
    xmedia_ive_src_image_s src_image = {0};
    xmedia_ive_dst_image_s dst_image = {0};
    
    // fill img
    csc_ctrl.en_mode = XMEDIA_IVE_CSC_MODE_PIC_BT601_YUV2RGB;
    src_image.au64phyaddr[0] = img_src_phy;
    src_image.au64phyaddr[1] = img_src_phy + xmd_npu_model->input.tensor[0].shape.dims[3] * xmd_npu_model->input.tensor[0].shape.dims[2];
    src_image.au64phyaddr[2] = dst_image.au64phyaddr[1] + xmd_npu_model->input.tensor[0].shape.dims[3] * xmd_npu_model->input.tensor[0].shape.dims[2];
    src_image.au64viraddr[0] = 0;
    src_image.au32stride[0] = xmd_npu_model->input.tensor[0].shape.dims[3];
    src_image.au32stride[1] = xmd_npu_model->input.tensor[0].shape.dims[3];
    src_image.au32stride[2] = xmd_npu_model->input.tensor[0].shape.dims[3];
    src_image.u32width = xmd_npu_model->input.tensor[0].shape.dims[3];
    src_image.u32height = xmd_npu_model->input.tensor[0].shape.dims[2];
    src_image.en_type = XMEDIA_IVE_IMAGE_TYPE_YUV420SP;

    dst_image.au64phyaddr[2] = img_src_rgb_phy;
    dst_image.au64phyaddr[1] = img_src_rgb_phy + xmd_npu_model->input.tensor[0].shape.dims[3] * xmd_npu_model->input.tensor[0].shape.dims[2];
    dst_image.au64phyaddr[0] = dst_image.au64phyaddr[1] + xmd_npu_model->input.tensor[0].shape.dims[3] * xmd_npu_model->input.tensor[0].shape.dims[2];
    dst_image.au64viraddr[0] = 0;
    dst_image.au32stride[0] = xmd_npu_model->input.tensor[0].shape.dims[3];
    dst_image.au32stride[1] = xmd_npu_model->input.tensor[0].shape.dims[3];
    dst_image.au32stride[2] = xmd_npu_model->input.tensor[0].shape.dims[3];
    dst_image.u32width = xmd_npu_model->input.tensor[0].shape.dims[3];
    dst_image.u32height = xmd_npu_model->input.tensor[0].shape.dims[2];
    dst_image.en_type = XMEDIA_IVE_IMAGE_TYPE_U8C3_PLANAR;

    ret = xmedia_ive_csc(&handle, &src_image, &dst_image, &csc_ctrl, XMEDIA_FALSE);
    if (XMEDIA_SUCCESS != ret) {
        SVP_TRACE(MODULE_DBG_ERR,"xmedia_ive_csc failed %#x!\n", ret);
        return XMEDIA_FAILURE;
    }
    ret = xmedia_ive_query(handle, &b_finish, XMEDIA_TRUE);
    while (ERR_CODE_IVE_QUERY_TIMEOUT == ret) {
        usleep(100);
        ret = xmedia_ive_query(handle, &b_finish, XMEDIA_TRUE);
    }

    return XMEDIA_SUCCESS;
}

xmedia_s32 svp_tde_y2r(xmedia_npu_model* xmd_npu_model,xmedia_u64 img_src_phy,xmedia_u64 img_src_rgb_phy)
{
    xmedia_s32 handle = -1;

    xmedia_s32 ret;
    xmedia_tde_surface_info src = {0};
    xmedia_tde_surface_info dst = {0};

    ret = xmedia_tde_create_job(&handle);
    if (XMEDIA_SUCCESS != ret)
    {
        SVP_TRACE(MODULE_DBG_ERR,"xmedia_tde_create_job failed %#x!\n",ret);
        handle = -1;
        return XMEDIA_FAILURE;
    }

    src.surface.phys_addr[0] = img_src_phy;
    src.surface.phys_addr[1] = img_src_phy + xmd_npu_model->input.tensor[0].shape.dims[2] * xmd_npu_model->input.tensor[0].shape.dims[1];
    src.surface.stride[0] = xmd_npu_model->input.tensor[0].shape.dims[2];   
    src.surface.stride[1] = xmd_npu_model->input.tensor[0].shape.dims[2];
    src.surface.width = xmd_npu_model->input.tensor[0].shape.dims[2];
    src.surface.height = xmd_npu_model->input.tensor[0].shape.dims[1];
    //src.surface.pixel_format = XMEDIA_VIDEO_PIXEL_FMT_YVU_SEMIPLANAR_420;
    src.surface.pixel_format = XMEDIA_VIDEO_PIXEL_FMT_YUV_SEMIPLANAR_420;
    src.surface.alpha0 = 0;
    src.surface.alpha1 = 255;
    src.surface.is_alpha_ext_1555 = 0;
    src.surface.clut_reload = XMEDIA_FALSE;

    src.rect.x = 0;
    src.rect.y = 0;
    src.rect.width = xmd_npu_model->input.tensor[0].shape.dims[2];
    src.rect.height = xmd_npu_model->input.tensor[0].shape.dims[1];

    dst.surface.phys_addr[0] = img_src_rgb_phy;
    dst.surface.width = xmd_npu_model->input.tensor[0].shape.dims[2];
    dst.surface.height = xmd_npu_model->input.tensor[0].shape.dims[1];
    dst.surface.stride[0] = xmd_npu_model->input.tensor[0].shape.dims[2] * 3; // tde 驱动要求,XMEDIA_VIDEO_PIXEL_FMT_RGB_888 的stride至少是surface->width * 2
//    dst.surface.stride[1] = output->frame.width; // add for lcb,rgb 对此没有要求
    dst.surface.pixel_format = XMEDIA_VIDEO_PIXEL_FMT_RGB_888;
    dst.surface.is_alpha_ext_1555 = XMEDIA_FALSE; // 是否使能1555的Alpha扩展。当位图格式为ARGB1555时，该项有效
    dst.surface.alpha0 = 0;
    dst.surface.alpha1 = 255;
//    dst.surface.clut_phys_addr = ; // 颜色扩展或颜色矫正功能需要配置这个
    dst.surface.clut_reload = XMEDIA_FALSE; // 颜色表是否重载。

    dst.rect.x = 0;
    dst.rect.y = 0;
    dst.rect.width = xmd_npu_model->input.tensor[0].shape.dims[2];
    dst.rect.height = xmd_npu_model->input.tensor[0].shape.dims[1];

    ret = xmedia_tde_add_task_single_blit(handle, &src, &dst, 0, XMEDIA_NULL);
    if (XMEDIA_SUCCESS != ret)
    {
        SVP_TRACE(MODULE_DBG_ERR,"xmedia_tde_add_task_scale failed ret: %#x !\n", ret);
        xmedia_tde_cancel_job(handle);
        return XMEDIA_FAILURE;
    }

    ret = xmedia_tde_submit_job(handle);
    if (XMEDIA_SUCCESS != ret)
    {
        SVP_TRACE(MODULE_DBG_ERR,"xmedia_tde_submit_job failed ,ret: %#x!\n", ret);
        xmedia_tde_cancel_job(handle);
        return XMEDIA_FAILURE;
    }

    ret = xmedia_tde_wait_job(handle, 2000);
    if (XMEDIA_SUCCESS != ret)
    {
        SVP_TRACE(MODULE_DBG_ERR,"tde overtime %#x!\n",ret);
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}

void nv21_to_rgb888(unsigned char* yuv, unsigned char* rgb, int width, int height) {
    int frameSize = width * height;
    int yIndex = 0;
    int uvIndex = frameSize;
    int r, g, b, y, u, v;
    int index = 0;
    
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            y = yuv[yIndex];
            u = yuv[uvIndex + 1];
            v = yuv[uvIndex];
            
            // YUV to RGB conversion
            r = y + (1.370705 * (v - 128));
            g = y - (0.698001 * (v - 128)) - (0.337633 * (u - 128));
            b = y + (1.732446 * (u - 128));
            
            // Clamping to 0-255
            r = r < 0 ? 0 : (r > 255 ? 255 : r);
            g = g < 0 ? 0 : (g > 255 ? 255 : g);
            b = b < 0 ? 0 : (b > 255 ? 255 : b);
            
            rgb[index++] = (unsigned char)r;
            rgb[index++] = (unsigned char)g;
            rgb[index++] = (unsigned char)b;
            
            yIndex++;
            
            if (j % 2 == 0 && i % 2 == 0) {
                uvIndex += 2;
            }
        }
    }
}

//软件csc
void sw_svp_yuv_to_rgb(xmedia_npu_model* xmd_npu_model,xmedia_u64 img_src_phy, xmedia_u64 img_src_rgb_phy)
{
    xmedia_void *virt_addr_src;
    xmedia_void *virt_addr_dst;
    int w = xmd_npu_model->input.tensor[0].shape.dims[2];
    int h = xmd_npu_model->input.tensor[0].shape.dims[1];
    virt_addr_src = xmedia_mmz_map(img_src_phy, w * h * 3 / 2, XMEDIA_FALSE);
    virt_addr_dst = xmedia_mmz_map(img_src_rgb_phy, w * h * 3, XMEDIA_FALSE);

   nv21_to_rgb888(virt_addr_src, virt_addr_dst, w ,h);
   xmedia_mmz_unmap(virt_addr_src);
   xmedia_mmz_unmap(virt_addr_dst);
}

/*
@xmd_npu_model
@img_src:img data address,format:Y,UV;Y,UV;Y,UV.....
*/
xmedia_s32 npu_run_model(xmedia_npu_model* xmd_npu_model, xmedia_u64 img_src_phy)
{
    xmedia_s32 i, size;
    xmedia_s32 ret;

    if (img_src_phy == 0) {
         SVP_TRACE(MODULE_DBG_ERR,"input imgdata phy is zero !\n");
         return XMEDIA_FAILURE;
    }

#ifdef SVP_DEBUG_SAVE_FILE
    FILE *fp_000 = fopen("src_360p.bin", "wb");
    // fwrite(img_src, 640*360*3/2, 1, fp_000);
    fclose(fp_000);
    printf("---tensor[0].dims[0,1,2,3]:%d,%d,%d,%d \n",
                                    xmd_npu_model->input.tensor[0].shape.dims[0],    // n h w c
                                    xmd_npu_model->input.tensor[0].shape.dims[1],
                                    xmd_npu_model->input.tensor[0].shape.dims[2],
                                    xmd_npu_model->input.tensor[0].shape.dims[3]);
#endif

#if 0
    // IVE CSC
    svp_ive_yuv2rgb(xmd_npu_model, img_src_phy, xmd_npu_model->rgb_u64phyaddr);
#else
#ifdef SVP_TIME_DEBUG
TIME_COST_START();
#endif
    //IVE CSC 未完成，暂时使用TDE转换
    //sw_svp_yuv_to_rgb(xmd_npu_model, img_src_phy, rgb_phyaddr);
    svp_tde_y2r(xmd_npu_model, img_src_phy, xmd_npu_model->rgb_phyaddr);
#ifdef SVP_TIME_DEBUG
TIME_COST_END();
TIME_COST_PRINT("svp y2r");
#endif
#endif

#ifdef SVP_DEBUG_SAVE_FILE
    FILE *fp_002 = fopen("rgb_pkg_360p.bin", "wb");
    fwrite(xmd_npu_model->rgb_work_viraddr, 640*360*3, 1, fp_002);
    fclose(fp_002);
#endif

    if (xmd_npu_model->input.num == INPUT_MODEL_NUM_RGB888) {
        for (i = 0; i < xmd_npu_model->input.num; i++) {
            size = common_get_type_get_bytes(xmd_npu_model->input.tensor[i].shape.type) *
                                             xmd_npu_model->input.tensor[i].shape.dims[0] *
                                             xmd_npu_model->input.tensor[i].shape.dims[1] *
                                             xmd_npu_model->input.tensor[i].shape.pch[1];
#ifdef SVP_DEBUG_SAVE_FILE
        printf(" tensor[0].dims[0,1,2,3]:%d,%d,%d,%d \n",
                                    xmd_npu_model->input.tensor[0].shape.dims[0],
                                    xmd_npu_model->input.tensor[0].shape.dims[1],
                                    xmd_npu_model->input.tensor[0].shape.dims[2],
                                    xmd_npu_model->input.tensor[0].shape.dims[3]);
        printf("tensor[0].pch[0]:%d,tensor[0].pch[1]:%d \n",
                                  xmd_npu_model->input.tensor[0].shape.pch[0],
                                  xmd_npu_model->input.tensor[0].shape.pch[1]);
            SVP_TRACE(MODULE_DBG_ERR, "input num %d\n", xmd_npu_model->input.num);
            SVP_TRACE(MODULE_DBG_ERR, "size:%d ,i:%d\n",size ,i);

            FILE *fp0 = fopen("inputdata_1.bin","rb");
            if (fp0 == NULL) {
                SVP_TRACE(MODULE_DBG_ERR, "open file fail! \n");
                return XMEDIA_FAILURE;
            }
            fread((void*)xmd_npu_model->rgb_work_viraddr, 1, size, fp0);

#endif
            if (size <= 0) {
                SVP_TRACE(MODULE_DBG_ERR,"model input size errr !\n");
                return XMEDIA_FAILURE;
            }
            xmd_npu_model->input.tensor[i].addr = xmd_npu_model->rgb_viraddr;
        }
    } else {
        SVP_TRACE(MODULE_DBG_ERR, "ERR: run model input abnormal:%d \n", xmd_npu_model->input.num);
        return XMEDIA_FAILURE;
    }
#ifdef SVP_TIME_DEBUG
    TIME_COST_START();
#endif
    ret = xmedia_cl_graph_set_inout(xmd_npu_model->graph, &xmd_npu_model->input, &xmd_npu_model->output);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "xmedia_cl_graph_set_inout err, errno: %d\n", ret);
        return XMEDIA_FAILURE;
    }
    ret = xmedia_cl_graph_process(xmd_npu_model->graph);
    if (ret != XMEDIA_SUCCESS) {
        SVP_TRACE(MODULE_DBG_ERR, "xmedia_cl_graph_process err, errno: %d\n", ret);
        return XMEDIA_FAILURE;
    }
#ifdef SVP_TIME_DEBUG
    TIME_COST_END();
    TIME_COST_PRINT("npu cl process hw");
#endif
    return XMEDIA_SUCCESS;
}

xmedia_s32 npu_unload_model(xmedia_npu_model* xmd_npu_model)
{
    if (xmd_npu_model->graph != NULL) {
        if (g_svp_cfg.reuse_type == XMEDIA_SVP_MEM_TYPE_MAX) {
            svp_mmz_unmap_and_free(xmd_npu_model->rgb_phyaddr, xmd_npu_model->rgb_viraddr);
            svp_mmz_unmap_and_free(xmd_npu_model->output_u64phyaddr, xmd_npu_model->output_viraddr);
        }
        svp_mmz_unmap_and_free(xmd_npu_model->weight_u64phyaddr, xmd_npu_model->weight_viraddr);
        CFREE(xmd_npu_model->output_size);
        CFREE(xmd_npu_model->user_count);
        free_inout_tensor_mem(&xmd_npu_model->output);
        free_inout_tensor_mem(&xmd_npu_model->input);
        xmedia_cl_graph_unload(xmd_npu_model->graph);
        xmd_npu_model->graph = NULL;
    }
    model_num--;
    if (model_num == 0) {
        if ((npu_work_local_flag == XMEDIA_TRUE) && (work_viraddr != NULL)) {
            svp_mmz_unmap_and_free(work_phyaddr, work_viraddr);
            max_worksize = 0;
            npu_work_local_flag = XMEDIA_FALSE;
        }
        if (g_svp_cfg.workbuf_cfg.reuse_flag == XMEDIA_TRUE) {
            g_svp_cfg.workbuf_cfg.size = 0;
            g_svp_cfg.workbuf_cfg.reuse_flag = XMEDIA_FALSE;
        }
        if (g_svp_cfg.inputbuf_cfg.reuse_flag == XMEDIA_TRUE) {
            g_svp_cfg.inputbuf_cfg.size = 0;
            g_svp_cfg.inputbuf_cfg.reuse_flag = XMEDIA_FALSE;
        }
        if (g_svp_cfg.outputbuf_cfg.reuse_flag == XMEDIA_TRUE) {
            g_svp_cfg.outputbuf_cfg.size = 0;
            g_svp_cfg.outputbuf_cfg.reuse_flag = XMEDIA_FALSE;
        }
    }

    return XMEDIA_SUCCESS;
}
