/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __XMEDIA_CIPHER_H__
#define __XMEDIA_CIPHER_H__

#include "xmedia_type.h"
#include "xmedia_errcode.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CIPHER_IV_CHANGE_ONE_PKG        (1)
#define CIPHER_IV_CHANGE_ALL_PKG        (2)

typedef enum {
    XMEDIA_CIPHER_WORK_MODE_ECB,
    XMEDIA_CIPHER_WORK_MODE_CBC,
    XMEDIA_CIPHER_WORK_MODE_CFB,
    XMEDIA_CIPHER_WORK_MODE_OFB,
    XMEDIA_CIPHER_WORK_MODE_CTR,
    XMEDIA_CIPHER_WORK_MODE_MAX,
} xmedia_cipher_work_mode;

typedef enum {
    XMEDIA_CIPHER_ALG_AES           = 0x0,
    XMEDIA_CIPHER_ALG_DMA           = 0x1,
    XMEDIA_CIPHER_ALG_MAX           = 0x2,
} xmedia_cipher_alg;

typedef enum {
    XMEDIA_CIPHER_KEY_AES_128BIT    = 0x0,
    XMEDIA_CIPHER_KEY_AES_192BIT    = 0x1,
    XMEDIA_CIPHER_KEY_AES_256BIT    = 0x2,
    XMEDIA_CIPHER_KEY_AES_MAX       = 0x3,
} xmedia_cipher_key_length;

typedef enum {
    XMEDIA_CIPHER_BIT_WIDTH_8BIT    = 0x0,
    XMEDIA_CIPHER_BIT_WIDTH_1BIT    = 0x1,
    XMEDIA_CIPHER_BIT_WIDTH_128BIT  = 0x2,
    XMEDIA_CIPHER_BIT_WIDTH_MAX     = 0X3,
} xmedia_cipher_bit_width;

typedef enum {
    XMEDIA_CIPHER_KEY_SRC_USER       = 0x0,
    XMEDIA_CIPHER_KEY_SRC_KLAD_1,
    XMEDIA_CIPHER_KEY_SRC_KLAD_2,
    XMEDIA_CIPHER_KEY_SRC_KLAD_3,
    XMEDIA_CIPHER_KEY_SRC_MAX,
} xmedia_cipher_ca_type;

typedef enum {
    XMEDIA_CIPHER_KLAD_TARGET_AES       = 0x0,
    XMEDIA_CIPHER_KLAD_TARGET_RSA,
    XMEDIA_CIPHER_KLAD_TARGET_MAX,
} xmedia_cipher_klad_target;

typedef struct {
    xmedia_u32 key[8];
    xmedia_u32 iv[4];
    xmedia_u32 iv_usage;
    xmedia_cipher_bit_width bit_width;
    xmedia_cipher_key_length key_len;
} xmedia_cipher_ctrl_aes;

typedef struct {
    xmedia_cipher_alg alg;
    xmedia_cipher_work_mode mode;
    xmedia_bool key_by_ca;
    xmedia_cipher_ca_type ca_type;
    xmedia_cipher_ctrl_aes aes_ctrl;
} xmedia_cipher_ctrl;

typedef struct {
    xmedia_ulong src_phy_addr;
    xmedia_ulong dest_phy_addr;
    xmedia_u32 byte_length;
} xmedia_cipher_package;

typedef struct {
    xmedia_u8 *data;
    xmedia_u32 data_len;
} xmedia_cipher_data;

typedef enum {
    XMEDIA_CIPHER_HASH_TYPE_SHA1,
    XMEDIA_CIPHER_HASH_TYPE_SHA224,
    XMEDIA_CIPHER_HASH_TYPE_SHA256,
    XMEDIA_CIPHER_HASH_TYPE_HMAC_SHA1,
    XMEDIA_CIPHER_HASH_TYPE_HMAC_SHA224,
    XMEDIA_CIPHER_HASH_TYPE_HMAC_SHA256,
    XMEDIA_CIPHER_HASH_TYPE_MAX,
} xmedia_cipher_hash_type;

typedef struct {
    xmedia_u8 *hmac_key;
    xmedia_u32 hmac_key_len;
    xmedia_cipher_hash_type type;
} xmedia_cipher_hash_attr;

typedef enum {
    XMEDIA_CIPHER_RSA_ENC_SCHEME_NO_PADDING,
    XMEDIA_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_0,
    XMEDIA_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_1,
    XMEDIA_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_2,
    XMEDIA_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA1,
    XMEDIA_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA224,
    XMEDIA_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA256,
    XMEDIA_CIPHER_RSA_ENC_SCHEME_RSAES_PKCS1_V1_5,
    XMEDIA_CIPHER_RSA_ENC_SCHEME_MAX,
} xmedia_cipher_rsa_enc_scheme;

typedef enum {
    XMEDIA_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA1 = 0x100,
    XMEDIA_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA224,
    XMEDIA_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA256,
    XMEDIA_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA1,
    XMEDIA_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA224,
    XMEDIA_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA256,
    XMEDIA_CIPHER_RSA_SIGN_SCHEME_MAX,
} xmedia_cipher_rsa_sign_scheme;

typedef struct {
    xmedia_u8  *n;
    xmedia_u8  *e;
    xmedia_u16 n_len;
    xmedia_u16 e_len;
} xmedia_cipher_rsa_pub_key;

typedef struct {
    xmedia_u8 *n;
    xmedia_u8 *e;
    xmedia_u8 *d;
    xmedia_u8 *p;
    xmedia_u8 *q;
    xmedia_u8 *dp;
    xmedia_u8 *dq;
    xmedia_u8 *qp;
    xmedia_u16 n_len;
    xmedia_u16 e_len;
    xmedia_u16 d_len;
    xmedia_u16 p_len;
    xmedia_u16 q_len;
    xmedia_u16 dp_len;
    xmedia_u16 dq_len;
    xmedia_u16 qp_len;
} xmedia_cipher_rsa_pri_key;

typedef struct {
    xmedia_cipher_rsa_enc_scheme scheme;
    xmedia_cipher_rsa_pub_key pub_key;
} xmedia_cipher_rsa_pub_enc;

typedef struct {
    xmedia_cipher_rsa_enc_scheme scheme;
    xmedia_cipher_rsa_pri_key pri_key;
    xmedia_cipher_ca_type ca_type;
} xmedia_cipher_rsa_pri_enc;

typedef struct {
    xmedia_cipher_rsa_sign_scheme scheme;
    xmedia_cipher_rsa_pri_key pri_key;
    xmedia_cipher_ca_type ca_type;
} xmedia_cipher_rsa_sign_attr;

typedef struct {
    xmedia_cipher_rsa_sign_scheme scheme;
    xmedia_cipher_rsa_pub_key pub_key;
} xmedia_cipher_rsa_verify_attr;


/*
 *  函数功能：cipher模块初始化
 *  函数参数：
 *      无
 *  返回值：
 *      XMEDIA_SUCCESS                  - 函数执行成功
 *      XMEDIA_ERRCODE_NOT_PERMITTED    - 超过接口允许的调用次数
 *      XMEDIA_ERRCODE_OPEN_FAILED      - 打开设备节点文件失败
 */
xmedia_s32 xmedia_cipher_init(xmedia_void);

/*
 *  函数功能：cipher模块去初始化
 *  函数参数：
 *      无
 *  返回值：
 *      XMEDIA_SUCCESS                  - 函数执行成功
 *      XMEDIA_ERRCODE_CLOSE_FAILED     - 关闭设备节点文件失败
 */
xmedia_s32 xmedia_cipher_exit(xmedia_void);

/*
 *  函数功能：创建一路Cipher句柄
 *  函数参数：
 *      handle          -  输出参数，cipher句柄指针
 *  返回值：
 *      XMEDIA_SUCCESS                  - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PARAM    - 参数错误
 *      XMEDIA_ERRCODE_BUSY             - 通道已申请完，无可用通道
 *      XMEDIA_ERRCODE_NOT_READY        - 通道输入节点地址不可配置
 *  注意：
 *      1. handle不能为空
 *      2. cipher句柄将用于数据AES加解密时的输入
 *      3. 最大支持2路cipher
 *      4. 使用完通道后，应销毁对应的通道
 */
xmedia_s32 xmedia_cipher_create_handle(xmedia_handle *handle);

/*
 *  函数功能：销毁一路cipher
 *  函数参数：
 *      handle          -  输入参数，cipher句柄
 *  返回值：
 *      XMEDIA_SUCCESS                  - 函数执行成功
 *      XMEDIA_INVALID_HANDLE           - 输入句柄无效
 *  注意：
 *      创建与销毁通道成对使用
 */
xmedia_s32 xmedia_cipher_destroy_handle(xmedia_handle handle);

/*
 *  函数功能：配置cipher控制信息。详细配置请参见结构体xmedia_cipher_ctrl
 *  函数参数：
 *      handle          -  输入参数，cipher句柄
 *      ctrl            -  输入参数，cipher控制信息指针
 *  返回值：
 *      XMEDIA_SUCCESS                  - 函数执行成功
 *      XMEDIA_INVALID_HANDLE           - 输入句柄无效
 *      XMEDIA_ERRCODE_INVALID_PARAM    - 参数错误
 *      XMEDIA_ERRCODE_BUSY             - OTP控制器忙/KLAD忙
 *      XMEDIA_FAILURE                  - OTP控制器操作未完成
 *  注意：
 *      cipher控制信息指针不能为空
 */
xmedia_s32 xmedia_cipher_config_handle(xmedia_handle handle, const xmedia_cipher_ctrl *ctrl);

/*
 *  函数功能：获取cipher通道对应的配置信息
 *  函数参数：
 *      handle          -  输入参数，cipher句柄
 *      ctrl            -  输出参数，cipher通道的配置信息
 *  返回值：
 *      XMEDIA_SUCCESS                 - 函数执行成功
 *      XMEDIA_INVALID_HANDLE          - 输入句柄无效
 *      XMEDIA_ERRCODE_INVALID_PARAM   - 参数错误
 */
xmedia_s32 xmedia_cipher_get_handle_config(xmedia_handle handle, xmedia_cipher_ctrl *ctrl);

/*
 *  函数功能：对数据进行加密
 *  函数参数：
 *      handle          -  输入参数，cipher句柄
 *      src_phy_addr    -  输入参数，待加密的数据的物理地址
 *      dest_phy_addr   -  输入参数，存放加密结果的物理地址
 *      length          -  输入参数，数据的长度（单位：字节）
 *  返回值：
 *      XMEDIA_SUCCESS                 - 函数执行成功
 *      XMEDIA_INVALID_HANDLE          - 输入句柄无效
 *      XMEDIA_ERRCODE_INVALID_PARAM   - 参数错误
 *  注意：
 *      1. cipher句柄必须已创建
 *      2. 可多次调用
 *      3. 模式为CTR时数据的长度为任意长度，其他模式要求与block size对齐
 */
xmedia_s32 xmedia_cipher_encrypt(xmedia_handle handle, xmedia_ulong src_phy_addr,
                                xmedia_ulong dest_phy_addr, xmedia_u32 length);

/*
 *  函数功能：对数据进行解密
 *  函数参数：
 *      handle          -  输入参数，cipher句柄
 *      src_phy_addr    -  输入参数，待解密的数据的物理地址
 *      dest_phy_addr   -  输入参数，存放解密结果的物理地址
 *      length          -  输入参数，数据的长度（单位：字节）
 *  返回值：
 *      XMEDIA_SUCCESS                 - 函数执行成功
 *      XMEDIA_INVALID_HANDLE          - 输入句柄无效
 *      XMEDIA_ERRCODE_INVALID_PARAM   - 参数错误
 *  注意：
 *      1. cipher句柄必须已创建
 *      2. 可多次调用
 *      3. 模式为CTR时数据的长度为任意长度，其他模式要求与block size对齐
 */
xmedia_s32 xmedia_cipher_decrypt(xmedia_handle handle, xmedia_ulong src_phy_addr,
                                xmedia_ulong dest_phy_addr, xmedia_u32 length);

/*
 *  函数功能：对数据进行加密
 *  函数参数：
 *      handle          -  输入参数，cipher句柄
 *      src_data        -  输入参数，待加密的数据的虚拟地址
 *      dest_data       -  输出参数，存放加密结果的虚拟地址
 *      length          -  输入参数，数据的长度（单位：字节）
 *  返回值：
 *      XMEDIA_SUCCESS                 - 函数执行成功
 *      XMEDIA_INVALID_HANDLE          - 输入句柄无效
 *      XMEDIA_ERRCODE_INVALID_PARAM   - 参数错误
 *  注意：
 *      1. cipher句柄必须已创建
 *      2. 可多次调用
 *      3. 模式为CTR时数据的长度为任意长度，其他模式要求与block size对齐
 */
xmedia_s32 xmedia_cipher_encrypt_vir(xmedia_handle handle, const xmedia_u8 *src_data,
                                xmedia_u8 *dest_data, xmedia_u32 length);

/*
 *  函数功能：对数据进行解密
 *  函数参数：
 *      handle         -  输入参数，cipher句柄
 *      src_data       -  输入参数，待解密的数据的虚拟地址
 *      dest_data      -  输出参数，存放解密结果的虚拟地址
 *      length         -  输入参数，数据的长度（单位：字节）
 *  返回值：
 *      XMEDIA_SUCCESS                 - 函数执行成功
 *      XMEDIA_INVALID_HANDLE          - 输入句柄无效
 *      XMEDIA_ERRCODE_INVALID_PARAM   - 参数错误
 *  注意：
 *      1. cipher句柄必须已创建
 *      2. 可多次调用
 *      3. 模式为CTR时数据的长度为任意长度，其他模式要求与block size对齐
 */
xmedia_s32 xmedia_cipher_decrypt_vir(xmedia_handle handle, const xmedia_u8 *src_data,
                                xmedia_u8 *dest_data, xmedia_u32 length);

/*
 *  函数功能：进行多个包数据的加密
 *  函数参数：
 *      handle          -  输入参数，cipher句柄
 *      pkg             -  输入参数，待加密的数据包
 *      num             -  输入参数，待加密的数据包个数
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_INVALID_HANDLE            - 输入句柄无效
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 参数错误
 *      XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 申请内存失败
 *  注意：
 *      1. cipher句柄必须已创建
 *      2. 可多次调用
 *      3. 每次加密的数据包个数最多不超过128个
 *      4. 对于多个包的操作，每个包都使用xmedia_cipher_config_handle配置的IV进行运算。
 *         IV作用域是可配置的，前一个包的向量运算结果可以作为下一个包的IV，
 *         或者每个包IV都是独立运算的(前一次函数调用的结果不会影响后一次函数调用的运算结果)
 */
xmedia_s32 xmedia_cipher_encrypt_multi(xmedia_handle handle, const xmedia_cipher_package *pkg,
                                xmedia_u32 num);

/*
 *  函数功能：进行多个包数据的解密
 *  函数参数：
 *      handle          -  输入参数，cipher句柄
 *      pkg             -  输入参数，待解密的数据包
 *      num             -  输入参数，待解密的数据包个数
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_INVALID_HANDLE            - 输入句柄无效
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 参数错误
 *      XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 申请内存失败
 *  注意：
 *      1. cipher句柄必须已创建
 *      2. 可多次调用
 *      3. 每次加密的数据包个数最多不超过128个
 *      4. 对于多个包的操作，每个包都使用xmedia_cipher_config_handle配置的IV进行运算。
 *         IV作用域是可配置的，前一个包的向量运算结果可以作为下一个包的IV，
 *         或者每个包IV都是独立运算的(前一次函数调用的结果不会影响后一次函数调用的运算结果)
 */
xmedia_s32 xmedia_cipher_decrypt_multi(xmedia_handle handle, const xmedia_cipher_package *pkg,
                                xmedia_u32 num);

/*
 *  函数功能：使用KLAD对透明密钥进行加密
 *  函数参数：
 *      type            -  输入参数，KLAD根密钥选择，只能选择EFUSE Key
 *      target          -  输入参数，使用该密钥的模块
 *      clean_key       -  输入参数，透明密钥
 *      ecnrypt_key     -  输出参数，加密密钥
 *      key_len         -  输入参数，密钥的长度，必须是16整数倍
 *  返回值：
 *      XMEDIA_SUCCESS                 - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PARAM   - 参数错误
 *      XMEDIA_ERRCODE_BUSY            - OTP控制器忙/KLAD忙
 *      XMEDIA_FAILURE                 - OTP控制器操作未完成
 *  注意：
 *      1. 默认使用AES256 ECB
 *      2. target为XMEDIA_CIPHER_KLAD_TARGET_RSA时，会对输入数据每16B进行大小端转换，然后对转换后的数据进行加密
 */
xmedia_s32 xmedia_cipher_klad_encrypt_key(xmedia_cipher_ca_type type,
                                xmedia_cipher_klad_target target, const xmedia_u8 *clean_key,
                                xmedia_u8 *encrypt_key, xmedia_u32 key_len);

/*
 *  函数功能：生成随机数
 *  函数参数：
 *      number          -  输出参数，输出的随机数
 *  返回值：
 *      XMEDIA_SUCCESS                 - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PARAM   - 参数错误
 *      XMEDIA_ERRCODE_TIMEOUT         - 获取随机数超时
 *      XMEDIA_FAILURE                 - 随机数无效
 */
xmedia_s32 xmedia_cipher_get_random_number(xmedia_u32 *number);

/*
 *  函数功能：初始化HASH模块
 *  函数参数：
 *      attr            -  输入参数，用于计算hash的结构体参数
 *      handle          -  输出参数，输出的hash句柄
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 参数错误
 *      XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 申请内存失败
 *      XMEDIA_ERRCODE_BUSY              - 所有通道忙，无法申请通道
 *  注意：
 *      如果有其他程序正在使用HASH模块，返回失败状态
 */
xmedia_s32 xmedia_cipher_hash_init(const xmedia_cipher_hash_attr *attr, xmedia_handle *handle);

/*
 *  函数功能：计算hash值
 *  函数参数：
 *      handle          -  输入参数，hash句柄
 *      input           -  输入参数，输入数据缓冲
 *      in_len          -  输入参数，输入数据的长度，单位：byte
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 参数错误
 *      XMEDIA_INVALID_HANDLE            - hash句柄无效
 *  注意：
 *      1. Hash句柄必须已经创建
 *      2. 可以分多次调用，每次计算若干字节数据
 */
xmedia_s32 xmedia_cipher_hash_update(xmedia_handle handle, const xmedia_u8 *input, xmedia_u32 in_len);

/*
 *  函数功能：获取hash值
 *  函数参数：
 *      handle          -  输入参数，hash句柄
 *      output_hash     -  输出参数，输出的hash值
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 参数错误
 *      XMEDIA_INVALID_HANDLE            - hash句柄无效
 */
xmedia_s32 xmedia_cipher_hash_final(xmedia_handle handle, xmedia_u8 *output);

/*
 *  函数功能：使用RSA公钥加密一段明文
 *  函数参数：
 *      rsa_enc         -  输入参数，公钥加密属性结构体
 *      input           -  输入参数，待加密的数据及其长度
 *      output          -  输出参数，加密结果数据及其长度
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 参数错误
 *      XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 申请内存失败
 *      XMEDIA_FAILURE                   - 加密失败
 */
xmedia_s32 xmedia_cipher_rsa_public_encrypt(const xmedia_cipher_rsa_pub_enc *rsa_enc,
                                xmedia_cipher_data *input,
                                xmedia_cipher_data *output);

/*
 *  函数功能：使用RSA私钥解密一段密文
 *  函数参数：
 *      rsa_dec         -  输入参数，私钥解密属性结构体
 *      input           -  输入参数，待解密的数据及其长度
 *      output          -  输出参数，解密结果数据及其长度
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 参数错误
 *      XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 申请内存失败
 *      XMEDIA_FAILURE                   - 解密失败
 */
xmedia_s32 xmedia_cipher_rsa_private_decrypt(const xmedia_cipher_rsa_pri_enc *rsa_dec,
                                xmedia_cipher_data *input,
                                xmedia_cipher_data *output);

/*
 *  函数功能：使用RSA私钥加密一段明文
 *  函数参数：
 *      rsa_enc         -  输入参数，私钥加密属性结构体
 *      input           -  输入参数，待加密的数据及其长度
 *      output          -  输出参数，加密结果数据及其长度
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 参数错误
 *      XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 申请内存失败
 *      XMEDIA_FAILURE                   - 加密失败
 */
xmedia_s32 xmedia_cipher_rsa_private_encrypt(const xmedia_cipher_rsa_pri_enc *rsa_enc,
                                xmedia_cipher_data *input,
                                xmedia_cipher_data *output);

/*
 *  函数功能：使用RSA公钥解密一段密文
 *  函数参数：
 *      rsa_dec         -  输入参数，公钥解密属性结构体
 *      input           -  输入参数，待解密的数据及其长度
 *      output          -  输出参数，解密结果数据及其长度
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 参数错误
 *      XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 申请内存失败
 *      XMEDIA_FAILURE                   - 解密失败
 */
xmedia_s32 xmedia_cipher_rsa_public_decrypt(const xmedia_cipher_rsa_pub_enc *rsa_dec,
                                xmedia_cipher_data *input,
                                xmedia_cipher_data *output);

/*
 *  函数功能：使用RSA私钥签名一段文本
 *  函数参数：
 *      rsa_sign_attr   -  输入参数，签名属性结构体
 *      data            -  输入参数，包括待签名的数据及其长度, 如果hash_data不为空，
 *                         则使用hash_data进行签名，该参数将被忽略
 *      hash_data       -  输入参数，待签名文本的HASH摘要，如果为空，则自动计算输入数据的HASH摘要进行签名
 *      sign            -  输出参数，包括签名结果数据及其长度
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 参数错误
 *      XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 申请内存失败
 *      XMEDIA_FAILURE                   - 签名失败
 */
xmedia_s32 xmedia_cipher_rsa_sign(const xmedia_cipher_rsa_sign_attr *rsa_sign_attr,
                                xmedia_cipher_data *data,
                                const xmedia_u8 *hash_data, xmedia_cipher_data *sign);

/*
 *  函数功能：使用RSA公钥验证签名
 *  函数参数：
 *      rsa_verify_attr -  输入参数，签名验证属性结构体
 *      data            -  输入参数，包括待验证的数据及其长度, 如果hash_data不为空，
 *                         则使用hash_data进行验证，该参数将被忽略
 *      hash_data       -  输入参数，待验证文本的HASH摘要，如果为空，则自动计算数据的HASH摘要进行验证
 *      sign            -  输入参数，包括待验证的签名数据及其长度
 *  返回值：
 *      XMEDIA_SUCCESS                   - 函数执行成功
 *      XMEDIA_ERRCODE_INVALID_PARAM     - 参数错误
 *      XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY - 申请内存失败
 *      XMEDIA_FAILURE                   - 验签失败
 */
xmedia_s32 xmedia_cipher_rsa_verify(const xmedia_cipher_rsa_verify_attr *rsa_verify_attr,
                                xmedia_cipher_data *data,
                                const xmedia_u8 *hash_data, xmedia_cipher_data *sign);


#ifdef __cplusplus
}
#endif

#endif

