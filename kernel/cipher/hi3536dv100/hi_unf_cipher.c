/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_unf_cipher.c
  Version       : Initial Draft
  Created       : 2010/3/16
  Last Modified :
  Description   : unf of cipher
  Function List :
  History       :
  1.Date        : 2010/3/16
    Modification: Created file

******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#include "hi_type.h"
#include "hi_unf_ecs.h"

static HI_S32 g_CipherDevFd = -1;

static pthread_mutex_t   g_CipherMutex = PTHREAD_MUTEX_INITIALIZER;

#define HI_CIPHER_LOCK()  	 (void)pthread_mutex_lock(&g_CipherMutex);
#define HI_CIPHER_UNLOCK()  	 (void)pthread_mutex_unlock(&g_CipherMutex);

#define HI_ERR_CIPHER(fmt...) \
do{\
    printf("CIPHER ERROR:");\
    printf(fmt);\
}while(0)


#define CHECK_CIPHER_OPEN()\
do{\
    HI_CIPHER_LOCK();\
    if (0 > g_CipherDevFd)\
    {\
        HI_ERR_CIPHER("CIPHER is not open.\n");\
        HI_CIPHER_UNLOCK();\
        return HI_ERR_CIPHER_NOT_INIT;\
    }\
    HI_CIPHER_UNLOCK();\
}while(0)

#define CIPHER_MAX_ENC_DEC_LEN 1024*1024


/*----------------------------------------------*
 * external variables                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * external routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * internal routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * project-wide global variables                *
 *----------------------------------------------*/

/*----------------------------------------------*
 * module-wide global variables                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * constants                                    *
 *----------------------------------------------*/

/*----------------------------------------------*
 * macros                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * routines' implementations                    *
 *----------------------------------------------*/
HI_S32 HI_UNF_CIPHER_Open(HI_VOID)
{
    HI_CIPHER_LOCK();

    if (0 < g_CipherDevFd)
    {
        HI_CIPHER_UNLOCK();
        return HI_SUCCESS;
    }

    g_CipherDevFd = open("/dev/cipher", O_RDWR, 0);
    if (0 > g_CipherDevFd)
    {
        HI_ERR_CIPHER("Open CIPHER err.\n");
        HI_CIPHER_UNLOCK();
        return HI_ERR_CIPHER_FAILED_INIT;
    }

    HI_CIPHER_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 HI_UNF_CIPHER_Close(HI_VOID)
{
    HI_S32 Ret = HI_FAILURE;

    HI_CIPHER_LOCK();

    if (0 > g_CipherDevFd)
    {
        HI_CIPHER_UNLOCK();
        return HI_SUCCESS;
    }

    Ret = close(g_CipherDevFd);

    if(HI_SUCCESS != Ret)
    {
        HI_ERR_CIPHER("Close CIPHER err.\n");
        HI_CIPHER_UNLOCK();
        return HI_ERR_CIPHER_NOT_INIT;
    }

    g_CipherDevFd = -1;

    HI_CIPHER_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 HI_UNF_CIPHER_CreateHandle(HI_HANDLE* phCipher)
{
    HI_U32 Ret = HI_FAILURE;

    if (NULL == phCipher)
    {
        HI_ERR_CIPHER("para phCipher is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    CHECK_CIPHER_OPEN();

    Ret=ioctl(g_CipherDevFd,HI_CIPHER_CreateHandle,phCipher);

    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_CIPHER_DestroyHandle(HI_HANDLE hCipher)
{
    HI_U32 Ret = HI_FAILURE;

    CHECK_CIPHER_OPEN();

    Ret=ioctl(g_CipherDevFd,HI_CIPHER_DestroyHandle,hCipher);

    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    return HI_SUCCESS;

}

HI_S32 HI_UNF_CIPHER_ConfigHandle(HI_HANDLE hCipher, HI_UNF_CIPHER_CTRL_S* pstCtrl)
{
    HI_U32 Ret = HI_FAILURE;
    CIPHER_Config_CTRL  configdata;

    if (NULL == pstCtrl)
    {
        HI_ERR_CIPHER("para pstCtrl is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    memset(&configdata, 0, sizeof(configdata));
    memcpy(&configdata.CIpstCtrl, pstCtrl, sizeof(HI_UNF_CIPHER_CTRL_S));
    configdata.CIHandle = hCipher;

    if (configdata.CIpstCtrl.enWorkMode>=HI_UNF_CIPHER_WORK_MODE_BUTT)
    {
        HI_ERR_CIPHER("para setCIPHER wokemode is invalid.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    CHECK_CIPHER_OPEN();

    Ret = ioctl(g_CipherDevFd,HI_CIPHER_ConfigHandle,&configdata);

    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_CIPHER_Encrypt(HI_HANDLE hCipher, HI_U32 u32SrcPhyAddr, HI_U32 u32DestPhyAddr, HI_U32 u32ByteLength)
{
    HI_U32 Ret = HI_FAILURE;
    HI_U32 chnid = 0;
    CIPHER_DATA_S CIdata;

    chnid=hCipher&0x00ff;
    if (0 == chnid || u32ByteLength < 1 || u32ByteLength >= CIPHER_MAX_ENC_DEC_LEN)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    memset(&CIdata, 0, sizeof(CIdata));

    CIdata.ScrPhyAddr=u32SrcPhyAddr;
    CIdata.DestPhyAddr=u32DestPhyAddr;
    CIdata.ByteLength=u32ByteLength;
    CIdata.CIHandle=hCipher;

    CHECK_CIPHER_OPEN();

    Ret=ioctl(g_CipherDevFd,HI_CIPHER_Encrypt,&CIdata);

    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_CIPHER_Decrypt(HI_HANDLE hCipher, HI_U32 u32SrcPhyAddr, HI_U32 u32DestPhyAddr, HI_U32 u32ByteLength)
{
    HI_U32 Ret = HI_FAILURE;
    HI_U32 chnid = 0;
    CIPHER_DATA_S CIdata;

    chnid=hCipher&0x00ff;
    if (0 == chnid || u32ByteLength < 1 || u32ByteLength >= CIPHER_MAX_ENC_DEC_LEN)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    memset(&CIdata, 0, sizeof(CIdata));

    CIdata.ScrPhyAddr=u32SrcPhyAddr;
    CIdata.DestPhyAddr=u32DestPhyAddr;
    CIdata.ByteLength=u32ByteLength;
    CIdata.CIHandle=hCipher;

    CHECK_CIPHER_OPEN();

    Ret=ioctl(g_CipherDevFd,HI_CIPHER_Decrypt,&CIdata);

    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    return HI_SUCCESS;
}


