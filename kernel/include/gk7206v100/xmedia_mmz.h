/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef XMEDIA_MMZ_H
#define XMEDIA_MMZ_H

#include "xmedia_type.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  函数功能：申请 MMZ 内存
 *  函数参数：
 *      mmz_name - 输入参数，申请的内存内存池名
 *      buf_name - 输入参数，申请的内存命名
 *      size - 输入参数，申请的内存大小
 *  返回值：
 *      0      - 函数执行失败，返回空
 *      others - 函数执行成功，返回申请内存的物理地址
 */
xmedia_u64 xmedia_mmz_alloc(const xmedia_char *mmz_name, const xmedia_char *buf_name, xmedia_u32 size);

/*
 *  函数功能：释放 MMZ 内存
 *  函数参数：
 *      phy_addr - 输入参数，待释放的MMZ内存物理地址
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      others - 函数执行失败，返回错误码
 */
xmedia_s32 xmedia_mmz_free(xmedia_u64 phy_addr);

/*
 *  函数功能：映射内存
 *  函数参数：
 *      phy_addr - 输入参数，待映射内存对应的物理地址
 *      size - 输入参数，待映射内存的大小
 *      cached - 输入参数，映射时是否带cache
 *  返回值：
 *      XMEDIA_NULL - 函数执行失败，返回空指针
 *      others - 函数执行成功过，返回映射后的虚拟地址
 */
xmedia_void* xmedia_mmz_map(xmedia_u64 phy_addr, xmedia_u32 size, xmedia_bool cached);

/*
 *  函数功能：解除映射
 *  函数参数：
 *      virt_addr - 输入参数，待解除映射内存对应的虚拟地址
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      others - 函数执行失败，返回错误码
 */
xmedia_s32 xmedia_mmz_unmap(void *virt_addr);

/*
 *  函数功能：刷新 cache 里的内容到MMZ内存并且使 cache 里的内容无效
 *  函数参数：
 *      phy_addr - 输入参数，待刷新cache对应的内存物理地址
 *      virt_addr - 输入参数，待刷新cache对应的内存虚拟地址
 *      size - 输入参数，待刷新cache区域大小
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      others - 函数执行失败，返回错误码
 */
xmedia_s32 xmedia_mmz_flush_cache(xmedia_u64 phy_addr, xmedia_void *virt_addr, xmedia_u32 size);

/*
 *  函数功能：将用户空间mmz的虚拟地址转换成对应的物理地址
 *  函数参数：
 *      pphy_addr - 输入参数，输出的mmz内存物理地址保存在此指针中
 *      virt_addr - 输入参数，mmz的内存虚拟地址
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      others - 函数执行失败，返回错误码
 */
xmedia_s32 xmedia_mmz_virt_to_phy(xmedia_void *virt_addr, xmedia_u64 *pphy_addr);

/*
 *  函数功能：获取用户空间mmz map时是否cached
 *  函数参数：
 *      pcached - 输入参数，输出的mmz内存是否cached保存在此指针中
 *      virt_addr - 输入参数，mmz的内存虚拟地址
 *  返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      others - 函数执行失败，返回错误码
 */
xmedia_s32 xmedia_mmz_is_cached(xmedia_void *virt_addr, xmedia_bool *pcached);

xmedia_void xmedia_mmz_init(xmedia_void);

xmedia_void xmedia_mmz_exit(xmedia_void);

#ifdef __cplusplus
}
#endif

#endif
