/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __XMEDIA_OTP_H__
#define __XMEDIA_OTP_H__

#include "xmedia_type.h"
#include "xmedia_errcode.h"

#ifdef __cplusplus
extern "C" {
#endif

#define     SHA256_BYTE_LEN             32
#define     AES256_KEY_BYTE_LEN         32
#define     OTP_USER_DATA_LEN_MAX       32         // 单位：word，用户自定义区共1Kbit

typedef enum {
    XMEDIA_OTP_AES256_KEY0 = 0x0,
    XMEDIA_OTP_AES256_KEY1,
    XMEDIA_OTP_AES256_KEY2,
    XMEDIA_OTP_AES256_KEY3,
    XMEDIA_OTP_AES256_KEY_MAX
} xmedia_otp_aes_key_index;

typedef enum {
    XMEDIA_OTP_UART_BURN_CLOSE = 0x0,
    XMEDIA_OTP_UART_BURN_REOPEN,
    XMEDIA_OTP_UART_BURN_MODE_MAX
} xmedia_otp_uart_burn_mode;

typedef enum {
    XMEDIA_OTP_FMC_FREQ_50MHZ = 0x0,
    XMEDIA_OTP_FMC_FREQ_75MHZ,
    XMEDIA_OTP_FMC_FREQ_12MHZ,
    XMEDIA_OTP_FMC_FREQ_MAX
} xmedia_otp_fmc_freq;

/*
 *  函数功能：OTP模块初始化
 *  函数参数：
 *      无
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_OPEN_FAILED       - 获取文件描述符失败
 */
xmedia_s32 xmedia_otp_init(xmedia_void);

/*
 *  函数功能：OTP模块去初始化
 *  函数参数：
 *      无
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_OPEN_FAILED       - 获取文件描述符失败
 *      XMEDIA_ERRCODE_CLOSE_FAILED      - 关闭文件描述符失败
 */
xmedia_s32 xmedia_otp_exit(xmedia_void);

/*
 *  函数功能：使能otp烧写功能，使能后，其余接口会操作真实的otp区域；未使能时，其余接口操作的是模拟otp区域
 *  函数参数：
 *      无
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT          - 未调用xmedia_otp_init接口进行初始化
 */
xmedia_s32 xmedia_otp_program_enable(xmedia_void);

/*
 *  函数功能：使能安全启动
 *  函数参数：
 *      无
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT          - 未调用xmedia_otp_init接口进行初始化
 *      XMEDIA_ERRCODE_NOT_PERMITTED     - otp位已配置过并锁定，无法更改
 *
 *  注：
 *      使能安全启动前，必须先调用xmedia_otp_root_key_sha256_set接口！
 */
xmedia_s32 xmedia_otp_secure_boot_enable(xmedia_void);

/*
 *  函数功能：使能DDR加扰
 *  函数参数：
 *      无
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT          - 未调用xmedia_otp_init接口进行初始化
 *      XMEDIA_ERRCODE_NOT_PERMITTED     - otp位已配置过并锁定，无法更改
 */
xmedia_s32 xmedia_otp_ddr_ca_enable(xmedia_void);

/*
 *  函数功能：关闭jtag接口
 *  函数参数：
 *      无
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT          - 未调用xmedia_otp_init接口进行初始化
 *      XMEDIA_ERRCODE_NOT_PERMITTED     - otp位已配置过并锁定，无法更改
 */
xmedia_s32 xmedia_otp_jtag_disable(xmedia_void);

/*
 *  函数功能：设置spi nand flash启动模式为4线
 *  函数参数：
 *      无
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT          - 未调用xmedia_otp_init接口进行初始化
 *      XMEDIA_ERRCODE_NOT_PERMITTED     - otp位已配置过并锁定，无法更改
 *
 *  注：
 *      默认为1线
 */
xmedia_s32 xmedia_otp_spi_nand_mode_set(xmedia_void);

/*
 *  函数功能：关闭、打开UART镜像烧写
 *  函数参数：
 *      burn_mode         - 输入参数，关闭或打开UART镜像烧写
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT          - 未调用xmedia_otp_init接口进行初始化
 *      XMEDIA_ERRCODE_NOT_PERMITTED     - otp位已配置过并锁定，无法更改
 *
 *  注：
 *      默认UART可以进行镜像的烧写，关闭UART烧写后可以再次打开UART烧写，再次打开UART烧写后不能再更改
 */
xmedia_s32 xmedia_otp_uart_burn_mode_sel(xmedia_otp_uart_burn_mode burn_mode);

/*
 *  函数功能：设置bootrom阶段spi nor、spi nand的时钟频率
 *  函数参数：
 *      frequency         - 输入参数
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT          - 未调用xmedia_otp_init接口进行初始化
 *      XMEDIA_ERRCODE_NOT_PERMITTED     - otp位已配置过并锁定，无法更改
 *
 *  注：
 *      默认时钟频率为12MHz，更改顺序只支持：默认频率->50MHz->75MHz->12MHz,更改顺序可以跳过某频率但是不可逆
 */
xmedia_s32 xmedia_otp_fmc_freq_sel(xmedia_otp_fmc_freq frequency);

/*
 *  函数功能：关闭bootrom串口打印
 *  函数参数：
 *      无
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT          - 未调用xmedia_otp_init接口进行初始化
 *      XMEDIA_ERRCODE_NOT_PERMITTED     - otp位已配置过并锁定，无法更改
 */
xmedia_s32 xmedia_otp_bootrom_debug_disable(xmedia_void);

/*
 *  函数功能：烧写安全启动使用的RSA4096 PSS验签算法公钥的SHA256值
 *  函数参数：
 *      sha256_data    - 输入参数，SHA256数据起始地址
 *      data_len       - 输入参数，SHA256数据长度，单位Byte
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT          - 未调用xmedia_otp_init接口进行初始化
 *      XMEDIA_ERRCODE_NULL_PTR          - 空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 传入参数不合法
 *      XMEDIA_ERRCODE_NOT_PERMITTED     - otp位已配置过并锁定，无法更改
 */
xmedia_s32 xmedia_otp_root_key_sha256_set(xmedia_u8 *sha256_data, xmedia_u32 data_len);

/*
 *  函数功能：烧写AES使用的硬件key
 *  函数参数：
 *      key           - 输入参数，key数据起始地址
 *      key_len       - 输入参数，key数据长度，单位Byte
 *      key_index     - 输入参数，key标志
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT          - 未调用xmedia_otp_init接口进行初始化
 *      XMEDIA_ERRCODE_NULL_PTR          - 空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 传入参数不合法
 *      XMEDIA_ERRCODE_NOT_PERMITTED     - otp位已配置过并锁定，无法更改
 */
xmedia_s32 xmedia_otp_aes256_key_set(xmedia_u8 *key, xmedia_u32 key_len, xmedia_otp_aes_key_index key_index);

/*
 *  函数功能：烧写自定义区OTP指定偏移的区域
 *  函数参数：
 *      data           - 输入参数，要写入otp中的值
 *      offset         - 输入参数，自定义otp区中的偏移，以32bit为单位
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT          - 未调用xmedia_otp_init接口进行初始化
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 传入参数不合法
 *      XMEDIA_ERRCODE_NOT_PERMITTED     - otp位已配置过并锁定，无法更改
 */
xmedia_s32 xmedia_otp_user_data_set(xmedia_u32 data, xmedia_u32 offset);

/*
 *  函数功能：读取自定义区OTP指定偏移的区域的值
 *  函数参数：
 *      data           - 输出参数，otp指定偏移的值
 *      offset         - 输入参数，自定义otp区中的偏移，以32bit为单位
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_INIT          - 未调用xmedia_otp_init接口进行初始化
 *      XMEDIA_ERRCODE_NULL_PTR          - 空指针
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 传入参数不合法
 */
xmedia_s32 xmedia_otp_user_data_get(xmedia_u32 *data, xmedia_u32 offset);

#ifdef __cplusplus
}
#endif

#endif
