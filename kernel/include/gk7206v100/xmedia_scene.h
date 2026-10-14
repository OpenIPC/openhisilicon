/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef XMEDIA_SCENE_H
#define XMEDIA_SCENE_H

#include "xmedia_type.h"

typedef enum {
    XMEDIA_MOD_ID_VICAP      = 0,
    XMEDIA_MOD_ID_VIPROC,
    XMEDIA_MOD_ID_VPSS,
    XMEDIA_MOD_ID_GDC,
    XMEDIA_MOD_ID_VGS,
    XMEDIA_MOD_ID_NPU,
    XMEDIA_MOD_ID_VPU,
    XMEDIA_MOD_ID_JPGE,
    XMEDIA_MOD_ID_CPU,
    XMEDIA_MOD_ID_VO,
    XMEDIA_MOD_ID_TDE,
    XMEDIA_MOD_ID_AIAO,
    XMEDIA_MOD_ID_SCD,
    XMEDIA_MOD_ID_MAX,
} xmedia_ddr_ost_qos_mod_id;

typedef struct {
    xmedia_u32 rd_ost;
    xmedia_u32 wr_ost;
    xmedia_u32 qos;
} xmedia_ddr_ost_qos_config;

/* npu load level */
typedef enum {
    XMEDIA_NPU_LOAD_LIGHT = 0, // [0-1T)
    XMEDIA_NPU_LOAD_MIDDLE,    // [1T-1.5T)
    XMEDIA_NPU_LOAD_HEAVY,     // [1.5T-2T]
    XMEDIA_NPU_LOAD_SUPPER,    // AINR
    XMEDIA_NPU_LOAD_MAX,
} xmedia_media_npu_load;

typedef struct {
    xmedia_u32 width;
    xmedia_u32 height;
    xmedia_u32 framerate;
} xmedia_multimedia_capability;

/* VPU chn width, height, fps info */
typedef struct {
    xmedia_u32 vpu_chn_id;
    xmedia_multimedia_capability capability;
} xmedia_vpu_chn_capability;

typedef struct {
    xmedia_u32 width;
    xmedia_u32 height;
    xmedia_bool wdr_enable;
} xmedia_vi_max_capability;

#define XMEDIA_MAX_VPU_CHNS_NUM    16

typedef struct {
    xmedia_vpu_chn_capability vpu_chn[XMEDIA_MAX_VPU_CHNS_NUM];
    xmedia_vi_max_capability vi_chn;
} xmedia_scene_capability;

/*
 *  函数功能：场景配置初始化
 *  函数参数：
 *      无
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_OPEN_FAILED       - 打开设备节点失败
 */
xmedia_s32 xmedia_scene_init(xmedia_void);

/*
 *  函数功能：场景配置去初始化
 *  函数参数：
 *      无
 *  返回值：
 *      无
 */
xmedia_void xmedia_scene_exit(xmedia_void);

/*
 *  函数功能：设置场景配置参数
 *  函数参数：
 *      npu_load         - 输入参数，npu负载级别，分为LIGHT/MIDDLE/HEAVY/SUPPER
 *      scene_capability - 输入参数，vpu大小码流以及vi通路的最大分辨率等配置
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT          - 没有调用init函数进行初始化
 *      XMEDIA_ERRCODE_NULL_PTR          - 空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 传入参数不合法
 *      XMEDIA_ERRCODE_NOT_PERMITTED     - 没有先调用xmedia_sys_init设置pipe工作模式
 */
xmedia_s32 xmedia_scene_config(xmedia_media_npu_load npu_load, xmedia_scene_capability *scene_capability);

/*
 *  函数功能：锁定/解锁指定模块的ost/qos配置
 *  函数参数：
 *      mod_id    - 输入参数，指定模块的id
 *      config    - 输入参数，需要锁定的目标ost/qos配置
 *      lock_flag - 输入参数，锁定/解锁标志，1表示锁定，0表示解锁
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT          - 没有调用init函数进行初始化
 *      XMEDIA_ERRCODE_NULL_PTR          - 空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 传入参数不合法
 */
xmedia_s32 xmedia_ddr_ost_qos_lock(xmedia_ddr_ost_qos_mod_id mod_id,
    xmedia_ddr_ost_qos_config *config, xmedia_u32 lock_flag);

#endif

