/******************************************************************************
 
  Copyright (C), 2001-2011, Huawei Tech. Co., Ltd.
 
 ******************************************************************************
  File Name     : hi_unf_cipher.c
  Version       : Initial Draft
  Author        : Q46153
  Created       : 2010/3/16
  Last Modified :
  Description   : unf of cipher 
  Function List :
  History       :
  1.Date        : 2010/3/16
    Author      : Q46153
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


#define CHECK_CIPHER_OPEN()\
do{\
    HI_CIPHER_LOCK();\
    if (g_CipherDevFd < 0)\
    {\
        printf("CIPHER is not open.\n");\
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

    if (g_CipherDevFd > 0)
    {
        HI_CIPHER_UNLOCK();
        return HI_SUCCESS;
    }

    //g_CipherDevFd = open("/dev/"UMAP_DEVNAME_CIPHER, O_RDWR, 0);
    g_CipherDevFd = open("/dev/cipher", O_RDWR, 0);
    if (g_CipherDevFd < 0)
    {
        printf("Open CIPHER err.\n");
        HI_CIPHER_UNLOCK();
        return HI_ERR_CIPHER_FAILED_INIT;
    }

    HI_CIPHER_UNLOCK();

    return HI_SUCCESS;
}



HI_S32 HI_UNF_CIPHER_Close(HI_VOID)
{
    HI_S32 Ret;

    HI_CIPHER_LOCK();

    if (g_CipherDevFd < 0)
    {
        HI_CIPHER_UNLOCK();
        return HI_SUCCESS;
    }

    Ret = close(g_CipherDevFd);

    if(HI_SUCCESS != Ret)
    {
        printf("Close CIPHER err.\n");
        HI_CIPHER_UNLOCK();
        return HI_ERR_CIPHER_NOT_INIT;
    }

    g_CipherDevFd = -1;

    HI_CIPHER_UNLOCK();

    return HI_SUCCESS;
}



HI_S32 HI_UNF_CIPHER_CreateHandle(HI_HANDLE* phCipher)
{
    HI_U32 Ret;

    if (NULL == phCipher)
    {
        printf("para phCipher is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    CHECK_CIPHER_OPEN();

    Ret=ioctl(g_CipherDevFd,HI_CIPHER_CreateHandle,phCipher);

    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    return HI_SUCCESS;

}



HI_S32 HI_UNF_CIPHER_DestroyHandle(HI_HANDLE hCipher)
{
    HI_U32 Ret;

    CHECK_CIPHER_OPEN();

    Ret=ioctl(g_CipherDevFd,HI_CIPHER_DestroyHandle,hCipher);

    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    return HI_SUCCESS;

}

HI_S32 HI_UNF_CIPHER_ConfigHandle(HI_HANDLE hCipher, HI_UNF_CIPHER_CTRL_S* pstCtrl)
{
    HI_U32 Ret;
    CIPHER_Config_CTRL  configdata;

    if (NULL == pstCtrl)
    {
        printf("para pstCtrl is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    memcpy(&configdata.CIpstCtrl, pstCtrl, sizeof(HI_UNF_CIPHER_CTRL_S));
    configdata.CIHandle=hCipher;

    if(configdata.CIpstCtrl.enWorkMode>=HI_UNF_CIPHER_WORK_MODE_BUTT)
    {
        printf("para setCIPHER wokemode is invalid.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    CHECK_CIPHER_OPEN();

    Ret=ioctl(g_CipherDevFd,HI_CIPHER_ConfigHandle,&configdata);

    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    return HI_SUCCESS;
}




HI_S32 HI_UNF_CIPHER_Encrypt(HI_HANDLE hCipher, HI_U32 u32SrcPhyAddr, HI_U32 u32DestPhyAddr, HI_U32 u32ByteLength)
{
    HI_U32 Ret;
    HI_U32 chnid;
    CIPHER_DATA_S CIdata;

    chnid=hCipher&0x00ff;
    if (0 == chnid || u32ByteLength < 16 || u32ByteLength >= CIPHER_MAX_ENC_DEC_LEN)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    CIdata.ScrPhyAddr=u32SrcPhyAddr;
    CIdata.DestPhyAddr=u32DestPhyAddr;
    CIdata.ByteLength=u32ByteLength;
    CIdata.CIHandle=hCipher;

    CHECK_CIPHER_OPEN();

    Ret=ioctl(g_CipherDevFd,HI_CIPHER_Encrypt,&CIdata);

    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    return HI_SUCCESS;

}

HI_S32 HI_UNF_CIPHER_Decrypt(HI_HANDLE hCipher, HI_U32 u32SrcPhyAddr, HI_U32 u32DestPhyAddr, HI_U32 u32ByteLength)
{
    HI_U32 Ret;
    HI_U32 chnid;
    CIPHER_DATA_S CIdata;

    chnid=hCipher&0x00ff;
    if (0 == chnid || u32ByteLength < 16 || u32ByteLength >= CIPHER_MAX_ENC_DEC_LEN)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    CIdata.ScrPhyAddr=u32SrcPhyAddr;
    CIdata.DestPhyAddr=u32DestPhyAddr;
    CIdata.ByteLength=u32ByteLength;
    CIdata.CIHandle=hCipher;

    CHECK_CIPHER_OPEN();

    Ret=ioctl(g_CipherDevFd,HI_CIPHER_Decrypt,&CIdata);

    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    return HI_SUCCESS;
}


