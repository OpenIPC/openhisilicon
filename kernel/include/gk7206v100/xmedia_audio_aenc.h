/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __XMEDIA_AUDIO_AENC_H__
#define __XMEDIA_AUDIO_AENC_H__

#include "xmedia_type.h"
#include "xmedia_audio_codec.h"
#include "xmedia_audio_ai.h"
#include "xmedia_audio_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef struct {
    audio_codec_type  type;             // 编码类型
    encoder_param*    param;            // 编码参数
    xmedia_u32        in_buffer_size;   // 输入缓存大小，最大支持64 * 1024 字节
    xmedia_u32        out_frame_num;    // 输出缓存帧总数量，最大支持30
} aenc_attr;

/*
 *  函数功能：初始化音频编码模块
 *  函数参数：
 *     无
 *  返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_OPEN_FAILED- 打开设备节点失败
 *     XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 内存不足
 */
xmedia_s32 xmedia_aenc_init(xmedia_void);

/*
 *  函数功能：去初始化音频编码模块
 *  函数参数：
 *     无
 *  返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_BUSY - 设备忙无法直接退出
 */
xmedia_s32 xmedia_aenc_exit(xmedia_void);

/*
 *  函数功能：注册音频编码器
 *  函数参数：
 *     mask - 输入参数，编码器掩码
 *     handle - 输入参数，编码器句柄
 *  返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_INIT - aenc模块没有初始化
 *     XMEDIA_ERRCODE_INVALID_PARAM - 编码器掩码不合法
 *     XMEDIA_ERRCODE_EXIST - 编码器已经注册
 *     XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 内存不足
 */
xmedia_s32 xmedia_aenc_register(codec_mask_type mask, const audio_encoder* handle);

/*
 *  函数功能：反注册音频编码器
 *  函数参数：
 *     mask - 输入参数，编码器掩码
 *  返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_NOT_INIT - aenc模块没有初始化
 *     XMEDIA_ERRCODE_INVALID_PARAM - 编码器掩码不合法
 *     XMEDIA_ERRCODE_BUSY - 设备忙，无法反注册编码器
 */
xmedia_s32 xmedia_aenc_unregister(codec_mask_type mask);

/*
 * 函数功能: 创建一路音频编码
 * 函数参数：
 *     chn - 输入参数，通道号
 *     attr - 输入参数，通道属性参数
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_NOT_INIT - aenc模块没有初始化
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 通道号超出范围
 *     XMEDIA_ERRCODE_EXIST - 通道已经存在
 *     XMEDIA_ERRCODE_NOT_READY - 解码器没有注册
 *     XMEDIA_ERRCODE_INVALID_PARAM - 非法解码通道属性参数
 *     XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 内存不足
 */
xmedia_s32 xmedia_aenc_create_chn(aenc_chn chn, const aenc_attr* attr);

/*
 * 函数功能: 销毁一路音频编码
 * 函数参数：
 *     chn - 输入参数，通道号
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 非法通道号
 *     XMEDIA_ERRCODE_NOT_INIT - aenc模块没有初始化
 *     XMEDIA_ERRCODE_NOT_EXIST - 通道不存在
 *     XMEDIA_ERRCODE_BUSY - 设备忙，需要先解除绑定
 */
xmedia_s32 xmedia_aenc_destroy_chn(aenc_chn chn);

/*
 * 函数功能: 向音频编码通道发送码流数据
 * 函数参数：
 *     chn - 输入参数，通道号
 *     frame - 输入参数，码流帧信息
 *     time_out - 输入参数，超时时间
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 通道号超出范围
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_EXIST - 通道不存在
 *     XMEDIA_ERRCODE_INVALID_PARAM - 参数非法
 *     XMEDIA_ERRCODE_BUFFER_FULL - 码流buffer满
 */
xmedia_s32 xmedia_aenc_send_frame(aenc_chn chn, const audio_frame* frame, xmedia_s32 time_out);

/*
 * 函数功能: 获取音频编码通道的编码帧数据
 * 函数参数：
 *     chn - 输入参数, 通道号
 *     stream - 输出参数，编码帧信息
 *     time_out - 输入参数，超时时间
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 通道号超出范围
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_EXIST - 通道不存在
 *     XMEDIA_ERRCODE_BUFFER_EMPTY - 解码缓冲区为空
 */
xmedia_s32 xmedia_aenc_get_stream(aenc_chn chn, audio_stream* stream, xmedia_s32 time_out);

/*
 * 函数功能: 释放音频编码通道的编码帧
 * 函数参数：
 *     chn - 输入参数，通道号
 *     stream - 输入参数，编码帧信息
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 通道号超出范围
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_EXIST - 通道不存在
 *     XMEDIA_ERRCODE_INVALID_PARAM - 参数非法
 */
xmedia_s32 xmedia_aenc_release_stream(aenc_chn chn, audio_stream* stream);

/*
 * 函数功能: 绑定音频编码通道和音频输入通道
 * 函数参数：
 *     chn - 输入参数，通道号
 *     src_dev - 输入参数，音频输入设备号
 *     src_chn - 输入参数，音频输入通道号
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 无效通道号
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效设备号
 *     XMEDIA_ERRCODE_NOT_EXIST - 通道不存在
 *     XMEDIA_ERRCODE_NOT_PERMITTED - 通道已经在使用，绑定不被允许
 */
xmedia_s32 xmedia_aenc_bind(aenc_chn chn, ai_dev src_dev, ai_chn src_chn);

/*
 * 函数功能: 解除音频编码通道和音频输入通道
 * 函数参数：
 *     chn - 输入参数, 通道号
 *     src_dev - 输入参数，音频输入设备号
 *     src_chn - 输入参数，音频输入通道号
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_FAILURE - 函数执行失败
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 无效通道号
 *     XMEDIA_ERRCODE_INVALID_DEV_ID - 无效设备号
 *     XMEDIA_ERRCODE_NOT_EXIST - 通道不存在
 *     XMEDIA_ERRCODE_NOT_BIND - aenc没有绑定ai
 */
xmedia_s32 xmedia_aenc_unbind(aenc_chn chn, ai_dev src_dev, ai_chn src_chn);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

