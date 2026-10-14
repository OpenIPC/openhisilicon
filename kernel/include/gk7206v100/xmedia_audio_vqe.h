#ifndef _XMEDIA_API_VQE__H_
#define _XMEDIA_API_VQE__H_

#include "xmedia_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/*
 * 函数功能：注册音频处理算法
 * 函数参数：
 *      dev - 输入参数，算法掩码
 *      chn - 输入参数，算法句柄
 * 返回值：
 *      XMEDIA_SUCCESS - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PARAM - 无效参数
 *      XMEDIA_ERRCODE_NULL_PTR - 空指针
 */
xmedia_s32 xmedia_vqe_register_module(xmedia_u32 masks, xmedia_void* handle);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

