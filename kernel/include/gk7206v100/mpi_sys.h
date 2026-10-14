/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef MPI_SYS_H
#define MPI_SYS_H

#include "xmedia_type.h"

typedef enum {
    MPI_SYS_SENSOR_CLK_EN = 0,
    MPI_SYS_SENSOR_RST_SEL,
    MPI_SYS_SENSOR_CLK_SET,
    MPI_SYS_SENSOR_CLK_GET,
    MPI_SYS_MAX,
} mpi_sys_func_id;

/*
 *  函数功能：配置模块时钟或复位
 *  函数参数：
 *      chn_id  - 输入参数，需配置模块的目标通道，不支持多通道则配置为0
 *      func_id - 输入参数，操作类型
 *      param   - 输入参数
 *                时钟开关/复位场景：0表示时钟关闭/复位撤销；1表示时钟打开/复位使能
 *                时钟配置场景：需要配置的时钟频率，单位为Hz
 *                func_id为MPI_SYS_SENSOR_CLK_GET时，param为输出参数，获取当前时钟频率，单位为Hz
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_OPEN_FAILED       - 打开设备节点失败
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 参数非法
 */
xmedia_s32 mpi_sys_mod_ctrl(xmedia_u32 chn_id, mpi_sys_func_id func_id, xmedia_u32 *param);

/*
 *  函数功能：创建内核态proc节点，以及调试信息缓存buffer，用于保存用户态调试信息
 *  函数参数：
 *      name       - 输入参数，创建的目标节点名称，用于区分不同节点，需要以‘\0’结束
 *      buffer_len - 输入参数，需申请的调试信息缓存buffer的长度
 *      handle     - 输出参数，创建的proc节点操作句柄
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT          - 没有初始化sys模块
 *      XMEDIA_ERRCODE_NULL_PTR          - 空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 参数非法
 *      XMEDIA_ERRCODE_CREATE_FAILED     - 创建proc节点失败
 *      XMEDIA_ERRCODE_BUFFER_FULL       - 已创建的节点达到最大值
 */
xmedia_s32 mpi_sys_create_proc_entry(xmedia_char *name, xmedia_u32 buffer_len, xmedia_handle *handle);

/*
 *  函数功能：销毁内核态proc节点，以及释放调试信息缓存buffer
 *  函数参数：
 *      handle     - 输入参数，目标节点操作句柄
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT          - 没有初始化sys模块
 *      XMEDIA_ERRCODE_DESTROY_FAILED    - 销毁proc节点失败
 *  注意：只能销毁用户态mpi_sys_create_proc_entry接口创建的proc节点
 */
xmedia_s32 mpi_sys_destory_proc(xmedia_handle handle);

/*
 *  函数功能：往proc节点buffer写入调试信息
 *  函数参数：
 *      handle         - 输入参数，目标节点操作句柄
 *      buffer_addr_va - 输入参数，用户态保存调试信息的缓存地址（虚拟地址）
 *      buffer_len     - 输入参数，需要写入的数据长度
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT          - 没有初始化sys模块
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 参数非法
 *      XMEDIA_ERRCODE_NULL_PTR          - buffer地址为空指针
 *      XMEDIA_ERRCODE_BUFFER_FULL       - 待写入数据长度大于内核缓存空间的最大长度
 *      XMEDIA_ERRCODE_BAD_ADDRESS       - buffer地址异常，不支持拷贝到内核态
 *      XMEDIA_ERRCODE_COPY_DATA_ERROR   - 从用户态拷贝数据到内核态出错
 *  注意：只支持一次性将调试信息写入内核态的buffer中，不支持分段写入
 */
xmedia_s32 mpi_sys_write_proc(xmedia_handle handle, xmedia_u64 buffer_addr_va, xmedia_u32 buffer_len);

#endif
