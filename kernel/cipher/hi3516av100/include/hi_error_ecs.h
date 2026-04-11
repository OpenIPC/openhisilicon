/**************************************
Author        : Hisilicon multime
Created       : 2006/3/6
Last Modified :
Description   : error code
Function List :
History       :
 *********************************/
#ifndef __HI_ERROR_ECS_H__
#define __HI_ERROR_ECS_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/***************************** Macro Definition ******************************/
/** \addtogroup      H_3_5 */


#define  HI_ERR_CIPHER_NOT_INIT                     (HI_S32)(0x804D0001)
#define  HI_ERR_CIPHER_INVALID_HANDLE               (HI_S32)(0x804D0002)
#define  HI_ERR_CIPHER_INVALID_POINT                (HI_S32)(0x804D0003)
#define  HI_ERR_CIPHER_INVALID_PARA                 (HI_S32)(0x804D0004)
#define  HI_ERR_CIPHER_FAILED_INIT                  (HI_S32)(0x804D0005)
#define  HI_ERR_CIPHER_FAILED_GETHANDLE             (HI_S32)(0x804D0006)
#define  HI_ERR_CIPHER_FAILED_RELEASEHANDLE         (HI_S32)(0x804D0007)
#define  HI_ERR_CIPHER_FAILED_CONFIGAES             (HI_S32)(0x804D0008)
#define  HI_ERR_CIPHER_FAILED_CONFIGDES             (HI_S32)(0x804D0009)
#define  HI_ERR_CIPHER_FAILED_ENCRYPT               (HI_S32)(0x804D000A)
#define  HI_ERR_CIPHER_FAILED_DECRYPT               (HI_S32)(0x804D000B)


/** @} */  /*! <!-- Macro Definition end */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __ECS_ERRCODE_H__ */
