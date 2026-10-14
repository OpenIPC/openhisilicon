/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __XMEDIA_AUDIO_ADEC_H__
#define __XMEDIA_AUDIO_ADEC_H__

#include "xmedia_type.h"
#include "xmedia_audio_common.h"
#include "xmedia_audio_codec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef struct {
    audio_codec_type type;        // 解码类型
    decoder_param* param;         // 解码参数
    xmedia_bool package_mode;     // 包模式和流程模式标志
    xmedia_u32 in_buffer_size;    // 输入缓存大小，最大支持64 * 1024 字节
    xmedia_u32 out_frame_num;     // 输出缓存帧总数量，最大支持30
} adec_attr;

typedef struct  {
    xmedia_u32 in_buffer_size;    // 输入缓存总字节数
    xmedia_u32 in_avail_size;     // 输入缓存可用字节数
    xmedia_u32 out_frame_num;     // 输出缓存帧总数量
    xmedia_u32 out_avail_frames;  // 输出缓存可用缓存帧数量
    xmedia_bool eos;              // 实际的解码结束标志
} adec_status;

/*
 *  函数功能：初始化音频解码模块
 *  函数参数：
 *     无
 *  返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_OPEN_FAILED - 打开设备节点失败
 *     XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 内存不足
 */
xmedia_s32 xmedia_adec_init(xmedia_void);

/*
 *  函数功能：去初始化音频解码模块
 *  函数参数：
 *     无
 *  返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_BUSY - 设备忙无法直接退出
 */
xmedia_s32 xmedia_adec_exit(xmedia_void);

/*
 *  函数功能：注册音频解码器
 *  函数参数：
 *     mask - 输入参数，解码器掩码
 *     handle - 输入参数，解码器句柄
 *  返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_INIT - adec模块没有初始化
 *     XMEDIA_ERRCODE_INVALID_PARAM - 解码器掩码不合法
 *     XMEDIA_ERRCODE_EXIST - 解码器已经注册
 *     XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 内存不足
 */
xmedia_s32 xmedia_adec_register(codec_mask_type mask, const audio_decoder* handle);

/*
 *  函数功能：反注册音频解码器
 *  函数参数：
 *     mask - 输入参数，解码器掩码
 *  返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_NOT_INIT - adec模块没有初始化
 *     XMEDIA_ERRCODE_INVALID_PARAM - 解码器掩码不合法
 *     XMEDIA_ERRCODE_BUSY - 设备忙, 无法反注册解码器
 */
xmedia_s32 xmedia_adec_unregister(codec_mask_type mask);

/*
 * 函数功能: 创建一路音频解码
 * 函数参数：
 *     chn - 输入参数，通道号
 *     attr - 输入参数，通道属性参数
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_NOT_INIT - adec模块没有初始化
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 通道号超出范围
 *     XMEDIA_ERRCODE_EXIST - 通道已经存在
 *     XMEDIA_ERRCODE_NOT_READY - 解码器没有注册
 *     XMEDIA_ERRCODE_INVALID_PARAM - 非法解码通道属性参数
 *     XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 内存不足
 */
xmedia_s32 xmedia_adec_create_chn(adec_chn chn, const adec_attr* attr);

/*
 * 函数功能: 销毁一路音频解码
 * 函数参数：
 *     chn - 输入参数，通道号
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 非法通道号
 *     XMEDIA_ERRCODE_NOT_INIT - adec模块没有初始化
 *     XMEDIA_ERRCODE_NOT_EXIST - 通道不存在
 *     XMEDIA_ERRCODE_BUSY - 设备忙，需要先解除绑定
 */
xmedia_s32 xmedia_adec_destroy_chn(adec_chn chn);

/*
 * 函数功能: 向音频解码通道发送码流数据
 * 函数参数：
 *     chn - 输入参数，通道号
 *     stream - 输入参数，码流信息
 *     time_out - 输入参数，超时时间
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 通道号超出范围
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_EXIST - 通道不存在
 *     XMEDIA_ERRCODE_INVALID_PARAM - 参数非法
 *     XMEDIA_ERRCODE_BUFFER_FULL - 码流buffer满
 */
xmedia_s32 xmedia_adec_send_stream(adec_chn chn, const audio_stream* stream, xmedia_s32 time_out);

/*
 * 函数功能: 获取音频解码通道的解码帧
 * 函数参数：
 *     chn - 输入参数，通道号
 *     frame - 输出参数，获取的解码帧信息
 *     milli_sec - 输入参数，超时时间
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 通道号超出范围
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_EXIST - 通道不存在
 *     XMEDIA_ERRCODE_BUFFER_EMPTY - 解码缓冲区为空
 */
xmedia_s32 xmedia_adec_get_frame(adec_chn chn, audio_frame* frame, xmedia_s32 time_out);

/*
 * 函数功能: 释放音频解码通道的解码帧
 * 函数参数：
 *     chn - 输入参数，adec通道号
 *     frame - 输入参数，释放的解码帧信息
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 通道号超出范围
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_EXIST - 通道不存在
 *     XMEDIA_ERRCODE_INVALID_PARAM - 参数非法
 */
xmedia_s32 xmedia_adec_release_frame(adec_chn chn, audio_frame* frame);

/*
 * 函数功能: 设置音频解码通道解码结束标志
 * 函数参数：
 *     chn - 输入参数，通道号
 *     eos - 输入参数，码流结束的eos标志
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 通道号超出范围
 *     XMEDIA_ERRCODE_NOT_EXIST - 通道不存在
 */
xmedia_s32 xmedia_adec_set_eos(adec_chn chn, xmedia_bool eos);

/*
 * 函数功能: 复位音频解码通道
 * 函数参数：
 *     chn - 输入参数，通道号
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 通道号超出范围
 *     XMEDIA_ERRCODE_NOT_EXIST - 通道不存在
 */
xmedia_s32 xmedia_adec_reset(adec_chn chn);

/*
 * 函数功能: 查询音频解码通道状态
 * 函数参数：
 *     chn - 输入参数，通道号
 *     status - 输出参数，通道状态
 * 返回值：
 *     XMEDIA_SUCCESS - 函数执行成功
 *     XMEDIA_ERRCODE_INVALID_CHN_ID - 通道号超出范围
 *     XMEDIA_ERRCODE_NULL_PTR - 空指针
 *     XMEDIA_ERRCODE_NOT_EXIST - 通道不存在
 */
xmedia_s32 xmedia_adec_get_status(adec_chn chn, adec_status* status);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

