/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef XMEDIA_VB_H
#define XMEDIA_VB_H

#include "xmedia_type.h"
#include "xmedia_video_common.h"
#include "defines.h"

#ifdef __cplusplus
extern "C" {
#endif

#define VB_INVALID_BLKID            (-1U)
#define VB_INVALID_POOLID           (-1U)
#define VB_INVALID_HANDLE           (-1U)
#define XMEDIA_VB_MAX_COMM_POOLS    16

#define XMEDIA_VB_SUPPLEMENT_ISP_INFO_ENABLE     1
#define XMEDIA_VB_SUPPLEMENT_VDEC_INFO_ENABLE    (1 << 1)

typedef enum {
    XMEDIA_VB_MAP_MODE_NONE = 0,    /* no map */
    XMEDIA_VB_MAP_MODE_NOCACHE = 1, /* no cache map */
    XMEDIA_VB_MAP_MODE_CACHED = 2,  /* cached map, if you use this mode, you should flush cache by yourself */
    XMEDIA_VB_MAP_MODE_MAX
} xmedia_vb_map_mode;

typedef struct {
    xmedia_u32 block_size;
    xmedia_u32 block_cnt;
    xmedia_vb_map_mode map_mode;
    xmedia_char mmz_name[MAX_MMZ_NAME_LEN];
} xmedia_vb_pool_config;

typedef struct {
/*
 *    supplement_config：bit[0] 和bit[1]有效，其余为非法值。
 *    bit[0]: 1表示打开ISP的内存申请开关，0表示关闭；
 *    bit[1]: 1表示打开VDEC的内存申请开关，0表示关闭。
 */
    xmedia_u32 supplement_config;
    xmedia_u32 max_pool_cnt;
    xmedia_vb_pool_config common_pool[XMEDIA_VB_MAX_COMM_POOLS];
} xmedia_vb_config;

/*
 *  函数功能：VB模块初始化以及公共内存池初始化
 *  函数参数：
 *      config   - 输入参数，补充内存池、公共内存池的配置信息
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_OPEN_FAILED       - 打开设备节点失败
 *      XMEDIA_ERRCODE_NULL_PTR          - 空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 非法参数
 *      XMEDIA_ERRCODE_BUSY              - 资源忙，已执行初始化
 *      XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 创建公共内存池失败
 *  注意：
 *      config->max_pool_cnt可以不配置
 */
xmedia_s32 xmedia_vb_init(const xmedia_vb_config *config);

/*
 *  函数功能：VB模块去初始化以及公共内存池去初始化
 *  函数参数：
 *      无
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_OPEN_FAILED       - 打开设备节点失败
 *      XMEDIA_ERRCODE_BUSY              - 还有vb未被释放，不能去初始化
 */
xmedia_s32 xmedia_vb_exit(xmedia_void);

/*
 *  函数功能：打开/关闭强制销毁vb功能
 *  函数参数：
 *      enable   - 输入参数，打开或关闭强制销毁vb功能
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_OPEN_FAILED       - 打开设备节点失败
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 参数非法
 */
xmedia_s32 xmedia_vb_enable_force_destroy(xmedia_bool enable);

/*
 *  函数功能：获取公共内存池、补充内存池属性
 *  函数参数：
 *      config   - 输出参数，公共内存池配置信息
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_NULL_PTR          - 空指针
 *      XMEDIA_ERRCODE_OPEN_FAILED       - 打开设备节点失败
 *      XMEDIA_ERRCODE_NOT_READY         - 没有配置公共内存池，未初始化
 */
xmedia_s32 xmedia_vb_get_config(xmedia_vb_config *config);

/*
 *  函数功能：创建模块私有内存池
 *  函数参数：
 *      pool_config   - 输入参数，模块私有配置信息
 *  返回值：
 *      (-1U)  - 无效pool_Id，函数执行失败
 *      others - 新建的内存池ID
 *  注意：
 *      需要先完成初始化，xmedia_vb_init，才能创建私有内存池
 */
xmedia_u32 xmedia_vb_create_pool(xmedia_vb_pool_config *pool_config);

/*
 *  函数功能：销毁指定pool_id的内存池资源
 *  函数参数：
 *      pool_id   - 输入参数，需要销毁的内存池ID
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_OPEN_FAILED       - 打开设备节点失败
 *      XMEDIA_ERRCODE_NOT_READY         - VB没有初始化
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 参数非法
 *      XMEDIA_ERRCODE_NOT_EXIST         - 尝试销毁不存在的内存池
 *      XMEDIA_ERRCODE_NOT_PERMITTED     - 非法操作，尝试销毁内核态申请的私有内存池或者VB还有模块在使用
 */
xmedia_s32 xmedia_vb_destroy_pool(xmedia_u32 pool_id);

/*
 *  函数功能：根据内存池ID或size获取内存块handle
 *  函数参数：
 *      pool_id   - 输入参数，需要申请的内存池ID
 *      blk_size  - 输入参数，需要申请的内存块大小
 *      mmz_name  - 输入参数，内存块所属的mmz zone名称，在根据size申请内存块时需要设置
 *  返回值：
 *      (-1U)  - 无效handle，函数执行失败
 *      others - 内存块handle
 *  注意：
 *      如果pool_id为非法值(-1U)，则从公共内存池根据blk_size获取内存块
 */
xmedia_handle xmedia_vb_get_block(xmedia_u32 pool_id, xmedia_u32 blk_size, xmedia_char *mmz_name);

/*
 *  函数功能：根据handle释放内存块到free链表
 *  函数参数：
 *      handle   - 输入参数，需要释放的内存块handle
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_OPEN_FAILED       - 打开设备节点失败
 *      XMEDIA_ERRCODE_NOT_READY         - VB没有初始化
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 参数非法
 *      XMEDIA_ERRCODE_NOT_EXIST         - 尝试释放不存在的内存块
 */
xmedia_s32 xmedia_vb_release_block(xmedia_handle handle);

/*
 *  函数功能：根据物理地址获取对应内存块handle
 *  函数参数：
 *      phy_addr  - 输入参数，目标内存的物理地址
 *  返回值：
 *      (-1U)  - 无效handle，函数执行失败
 *      others - 内存块handle
 *  注意：
 *      找到的内存块的地址范围包含目标物理地址
 */
xmedia_handle xmedia_vb_phy_addr_to_handle(xmedia_u64 phy_addr);

/*
 *  函数功能：根据内存块handle获取对应的起始物理地址
 *  函数参数：
 *      handle   - 输入参数，目标内存块handle
 *  返回值：
 *      非0 - 目标内存块对应的起始物理地址
 *        0 - 函数执行失败
 */
xmedia_u64 xmedia_vb_handle_to_phy_addr(xmedia_handle handle);

/*
 *  函数功能：根据内存块handle获取对应内存池ID
 *  函数参数：
 *      handle   - 输入参数，目标内存块handle
 *  返回值：
 *      (-1U)  - 函数执行失败
 *      others - 目标内存池ID
 */
xmedia_u32 xmedia_vb_handle_to_pool_id(xmedia_handle handle);

/*
 *  函数功能：根据内存块handle查询当前使用者个数
 *  函数参数：
 *      handle   - 输入参数，目标内存块handle
 *  返回值：
 *      (-1U)  - 函数执行失败
 *      others - 目标内存块当前使用者个数
 */
xmedia_s32 xmedia_vb_inquire_user_cnt(xmedia_handle handle);

/*
 *  函数功能：根据句柄获取补充内存中内存块的信息
 *  函数参数：
 *      handle - 输入参数，内存块句柄
 *      info   - 输出参数，内存块描述信息
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_NULL_PTR          - 空指针
 *      XMEDIA_ERRCODE_OPEN_FAILED       - 打开设备节点失败
 *      XMEDIA_ERRCODE_NOT_READY         - VB没有初始化
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 参数非法
 *      XMEDIA_ERRCODE_COPY_DATA_ERROR   - 拷贝补充内存池信息到用户态失败
 */
xmedia_s32 xmedia_vb_get_supplement_info(xmedia_handle handle, xmedia_video_private_info *info);

typedef enum {
    XMEDIA_VB_AINR_DIRECTION_INPUT, // npu输入
    XMEDIA_VB_AINR_DIRECTION_OUTPUT, // npu输出
    XMEDIA_VB_AINR_DIRECTION_MAX
} xmedia_vb_ainr_direction;

typedef struct {
    xmedia_bool ainr_en;
    xmedia_vb_ainr_direction ainr_direction;
} xmedia_vb_ainr_attr;

typedef struct {
    xmedia_video_format video_fmt;
    xmedia_video_pixel_format pixel_fmt;
    xmedia_video_compress_mode cmp_mode;
    xmedia_video_data_width bit_width;

    xmedia_u32 width;
    xmedia_u32 height;
    xmedia_u32 align; // 0:auto

    xmedia_vb_ainr_attr ainr_attr;
} xmedia_vb_base_info;

typedef struct {
    xmedia_u32 vb_size;

    xmedia_u32 head_size;  //压缩头大小
    xmedia_u32 head_y_size;
    xmedia_u32 head_y_stride;
    xmedia_u32 head_c_stride;

    xmedia_u32 main_size;//内容数据大小
    xmedia_u32 main_y_size;
    xmedia_u32 main_stride; // y stride
    xmedia_u32 c_stride;
} xmedia_vb_cal_cfg;

/*
 *  函数功能：计算图像数据所需内存信息，包括内存大小，间距等
 *  函数参数：
 *      vb_base_info - 输入参数，图像数据描述信息
 *      vb_cal_cfg   - 输出参数，计算得到的内存大小、间距等信息
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_NULL_PTR          - 空指针
 *      XMEDIA_ERRCODE_OPEN_FAILED       - 打开设备节点失败
 *      XMEDIA_ERRCODE_NOT_SUPPORT       - 输入图像描述信息不支持
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 参数非法
 */
xmedia_s32 xmedia_vb_get_buffer_config(xmedia_vb_base_info* vb_base_info, xmedia_vb_cal_cfg* vb_cal_cfg);

/*
 *  函数功能：计算yuv格式图像数据卷绕模式下所需内存大小
 *  函数参数：
 *      vb_base_info - 输入参数，图像数据描述信息
 *      buffer_line  - 输入参数，卷绕buffer行数
 *  返回值：
 *      0        - 函数执行失败，返回0
 *      其他值      - 函数执行成功，返回内存大小
 */
xmedia_u32 xmedia_vb_get_wrap_buffer_size(xmedia_vb_base_info* vb_base_info, xmedia_u32 buffer_line);

#ifdef __cplusplus
}
#endif

#endif

