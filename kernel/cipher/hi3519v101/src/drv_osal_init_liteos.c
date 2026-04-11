/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_cipher_intf.c
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       :
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#include "hi_osal.h"
#include "hi_type.h"
#include "hal_cipher.h"
#include "drv_cipher.h"
#include "drv_cipher_ioctl.h"
#include "drv_hash.h"
#include "drv_cipher_log.h"
#include "hi_drv_cipher.h"
#include "config.h"
#include <asm/io.h>

extern HI_VOID DRV_CIPHER_UserCommCallBack(HI_U32 arg);

HI_U32 g_u32CipherBase = 0;
HI_U32 g_u32HashBase = 0;
HI_U32 g_u32RsaBase = 0;
HI_U32 g_u32RngBase = 0;
HI_U32 g_u32KladBase = 0;
HI_U32 g_u32EfuseBase = 0;


HI_VOID HAL_CIPHER_ReadReg(HI_U32 addr, HI_U32 *pu32Val)
{
    *pu32Val = CIPHER_READL((void*)addr);
    return;
}

HI_VOID HAL_CIPHER_WriteReg(HI_U32 addr, HI_U32 u32Val)
{
    CIPHER_WRITEL(u32Val, (void*)addr);
    return;
}

static HI_S32 CIPHER_ProcGetStatus(CIPHER_CHN_STATUS_S *pstCipherStatus)
{
#ifdef CIPHER_MULTICIPHER_SUPPORT
	return DRV_CIPHER_ProcGetStatus(pstCipherStatus);
#else
    memset(pstCipherStatus, 0, sizeof(CIPHER_CHN_STATUS_S));
    return HI_SUCCESS;
#endif
}

#ifndef DISABLE_DEBUG_INFO
static int CIPHER_ProcRead(struct osal_proc_dir_entry *p)
{
	CIPHER_CHN_STATUS_S stCipherStatus[8];
	int i = 0;
	HI_S32 ret = HI_FAILURE;

    osal_seq_printf(p, "\n------------------------------------------CIPHER STATUS-----------------------------------------------------------------------------------------------\n");
    osal_seq_printf(p, "Chnid   Status   Decrypt   Alg   Mode   KeyLen    Phy-Addr in/out      KeyFrom  INT-RAW in/out  INT-EN in/out INT_OCNTCFG\n");

	crypto_memset(&stCipherStatus, 0, sizeof(stCipherStatus));
	for(i = 0; i < 8; i++)
	{
		stCipherStatus[i].u32ChnId = i;
	}

	ret = CIPHER_ProcGetStatus((CIPHER_CHN_STATUS_S *)&stCipherStatus);
	if(HI_SUCCESS != ret)
	{
    	osal_seq_printf(p, "CIPHER_ProcGetStatus failed!\n");
        return ret;
	}

	for(i = 0; i < 8; i++)
	{
	    osal_seq_printf(p, " %d       %s      %d      %s  %s    %03d      %08x/%08x    %s           %d/%d            %d/%d        %08x\n",
						i,
						stCipherStatus[i].ps8Openstatus,
						stCipherStatus[i].bIsDecrypt,
						stCipherStatus[i].ps8Alg,
						stCipherStatus[i].ps8Mode,
						stCipherStatus[i].u32KeyLen,
						stCipherStatus[i].u32DataInAddr,
						stCipherStatus[i].u32DataOutAddr,
						stCipherStatus[i].ps8KeyFrom,
						stCipherStatus[i].bInINTRaw,
						stCipherStatus[i].bOutINTRaw,
						stCipherStatus[i].bInINTEn,
						stCipherStatus[i].bOutINTEn,
						stCipherStatus[i].u32OutINTCount);
	}

    return HI_SUCCESS;
}
#endif
HI_S32 CIPHER_Ioctl(HI_U32 cmd, HI_U32 argp, HI_VOID *private_data)
{
    HI_S32 ret = HI_SUCCESS;

    HI_INFO_CIPHER("start cmd, MOD_ID=0x%02X, NR=0x%02x, SIZE=0x%02x!\n",
                _IOC_TYPE (cmd), _IOC_NR (cmd), _IOC_SIZE(cmd));

    if (argp == 0)
    {
        return HI_FAILURE;
    }

    switch(cmd)
    {
#ifdef CIPHER_MULTICIPHER_SUPPORT
	    case CMD_CIPHER_CREATEHANDLE:
		{
	        CIPHER_HANDLE_S *pstCIHandle = (CIPHER_HANDLE_S *)argp;

			ret = HI_DRV_CIPHER_CreateHandle(pstCIHandle, (HI_U32)private_data);
	        break;
	    }

	    case CMD_CIPHER_DESTROYHANDLE:
	    {
	        HI_HANDLE hCipherChn = *(HI_HANDLE *)argp;

			ret = HI_DRV_CIPHER_DestroyHandle(hCipherChn);
	        break;
	    }
	    case CMD_CIPHER_CONFIGHANDLE:
	    {
	        CIPHER_Config_CTRL stCIConfig = *(CIPHER_Config_CTRL *)argp;

	        ret = HI_DRV_CIPHER_ConfigChn(stCIConfig.CIHandle, &stCIConfig.CIpstCtrl);
	        break;
	    }
	    case CMD_CIPHER_ENCRYPT:
	    {
	        CIPHER_DATA_S *pstCIData = (CIPHER_DATA_S *)argp;

			ret = HI_DRV_CIPHER_Encrypt(pstCIData);
			break;
	    }
	    case CMD_CIPHER_DECRYPT:
	    {
	        CIPHER_DATA_S *pstCIData = (CIPHER_DATA_S *)argp;

			ret = HI_DRV_CIPHER_Decrypt(pstCIData);
			break;
	    }
	    case CMD_CIPHER_ENCRYPTMULTI:
	    {
	        CIPHER_DATA_S *pstCIData = (CIPHER_DATA_S *)argp;

			ret = HI_DRV_CIPHER_EncryptMulti(pstCIData);
			break;
	    }
	    case CMD_CIPHER_DECRYPTMULTI:
	    {
	        CIPHER_DATA_S *pstCIData = (CIPHER_DATA_S *)argp;

			ret = HI_DRV_CIPHER_DecryptMulti(pstCIData);
	        break;
	    }
        case HI_CIPHER_ENCRYPTMULTI_EX:
	    {
	        CIPHER_MUTIL_EX_DATA_S *pstCIDataEx = (CIPHER_MUTIL_EX_DATA_S *)argp;

            ret = HI_DRV_CIPHER_EncryptMultiEx(pstCIDataEx);
			break;
	    }
	    case HI_CIPHER_DECRYPTMULTI_EX:
	    {
	        CIPHER_MUTIL_EX_DATA_S *pstCIDataEx = (CIPHER_MUTIL_EX_DATA_S *)argp;

            ret = HI_DRV_CIPHER_DecryptMultiEx(pstCIDataEx);
            break;
        }
#ifdef CIPHER_CCM_GCM_SUPPORT
        case CMD_CIPHER_GETTAG:
        {
	        CIPHER_TAG *pstCITag = (CIPHER_TAG *)argp;

			ret = HI_DRV_CIPHER_GetTag(pstCITag);
	        break;
	    }
#endif
	    case CMD_CIPHER_GETHANDLECONFIG:
	    {
	        CIPHER_Config_CTRL *pstCIData = (CIPHER_Config_CTRL *)argp;

	        ret = HI_DRV_CIPHER_GetHandleConfig(pstCIData);
	        break;
	    }
#ifdef CIPHER_KLAD_SUPPORT
		case CMD_CIPHER_KLAD_KEY:
        {
	        CIPHER_KLAD_KEY_S *pstKladData = (CIPHER_KLAD_KEY_S *)argp;

			ret = HI_DRV_CIPHER_KladEncryptKey(pstKladData);
	        break;
	    }
#endif
#endif
#ifdef CIPHER_RNG_SUPPORT
	    case CMD_CIPHER_GETRANDOMNUMBER:
	    {
	        CIPHER_RNG_S *pstRNG  = (CIPHER_RNG_S *)argp;
			ret = HI_DRV_CIPHER_GetRandomNumber(pstRNG);
	        break;
	    }
#endif
#ifdef CIPHER_HASH_SUPPORT
	    case CMD_CIPHER_CALCHASHINIT:
        {
            CIPHER_HASH_INIT_S *pstHashInit = (CIPHER_HASH_INIT_S*)argp;

            ret = CIPHER_HashInit(&pstHashInit->stHashAttr, &pstHashInit->HashHandle);
            break;
        }
	    case CMD_CIPHER_CALCHASHUPDATE:
	    {
            CIPHER_HASH_UPDATE_S *pstHashUpdate = (CIPHER_HASH_UPDATE_S*)argp;

            ret = CIPHER_HashUpdate(pstHashUpdate->HashHandle, pstHashUpdate->pu8InputData, pstHashUpdate->u32InputDataLen);
            break;
	    }
	    case CMD_CIPHER_CALCHASHFINAL:
	    {
            CIPHER_HASH_FINISH_S *pstHashFinish = (CIPHER_HASH_FINISH_S*)argp;

            ret = CIPHER_HashFinal(pstHashFinish->HashHandle, pstHashFinish->pu8OutputHash);
	        break;
	    }
        case CMD_CIPHER_CALCHASH:
	    {
	        CIPHER_HASH_DATA_S *pstCipherHashData = (CIPHER_HASH_DATA_S*)argp;

	        ret = HI_DRV_CIPHER_CalcHash(pstCipherHashData);
	        break;
	    }
#endif
#ifdef CIPHER_RSA_SUPPORT
        case CMD_CIPHER_CALCRSA:
        {
	        CIPHER_RSA_DATA_S *pCipherRsaData = (CIPHER_RSA_DATA_S*)argp;
	        ret = HI_DRV_CIPHER_CalcRsa(pCipherRsaData);
	        break;
	    }
#endif
	    default:
	    {
	        HI_ERR_CIPHER("Unknow cmd, MOD_ID=0x%02X, NR=0x%02x, SIZE=0x%02x!\n",
                _IOC_TYPE (cmd), _IOC_NR (cmd), _IOC_SIZE(cmd));
	        ret = HI_FAILURE;
	        break;
	    }
    }

    HI_INFO_CIPHER("end cmd, MOD_ID=0x%02X, NR=0x%02x, SIZE=0x%02x!\n",
                _IOC_TYPE (cmd), _IOC_NR (cmd), _IOC_SIZE(cmd));

    return ret;
}

static long DRV_CIPHER_Ioctl(HI_U32 cmd, unsigned long arg, HI_VOID *private_data)
{
    return CIPHER_Ioctl(cmd, arg, private_data);
}

static osal_fileops_t DRV_CIPHER_Fops;
static osal_dev_t *s_pstCipherDev;

HI_S32 CIPHER_DRV_ModInit(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
#ifndef DISABLE_DEBUG_INFO
	osal_proc_entry_t *pstProcEntry = HI_NULL;
#endif
    DRV_CIPHER_Fops.open = DRV_CIPHER_Open;
    DRV_CIPHER_Fops.unlocked_ioctl = DRV_CIPHER_Ioctl;
    DRV_CIPHER_Fops.release = DRV_CIPHER_Release;

    s_pstCipherDev = osal_createdev(UMAP_DEVNAME_CIPHER);
    s_pstCipherDev->fops  = &DRV_CIPHER_Fops;
    s_pstCipherDev->minor = UMAP_CIPHER_MINOR_BASE;

    ret = osal_registerdevice(s_pstCipherDev);
    if (HI_SUCCESS != ret )
    {
       HI_ERR_CIPHER("ERROR: could not register cipher devices\n");
       ret = HI_FAILURE;
       goto OUT0;
    }

#ifdef CIPHER_MULTICIPHER_SUPPORT
   g_u32CipherBase = (HI_U32)crypto_ioremap_nocache(REG_BASE_PHY_ADDR_CIPHER, REG_CI_BASE_SIZE);
   if (g_u32CipherBase == 0)
   {
       HI_ERR_CIPHER("ERROR: crypto_ioremap_nocache for cipher failed!!\n");
       ret = HI_FAILURE;
       goto OUT1;
   }
#endif

#ifdef CIPHER_HASH_SUPPORT
   g_u32HashBase = (HI_U32)crypto_ioremap_nocache(REG_BASE_PHY_ADDR_SHA, 0x1000);
   if (g_u32HashBase == 0)
   {
       HI_ERR_CIPHER("ERROR: crypto_ioremap_nocache for Hash failed!!\n");
       ret = HI_FAILURE;
       goto OUT2;
   }
#endif

#ifdef CIPHER_RSA_SUPPORT
   g_u32RsaBase = (HI_U32)crypto_ioremap_nocache(REG_BASE_PHY_ADDR_RSA, 0x1000);
   if (g_u32RsaBase == 0)
   {
       HI_ERR_CIPHER("ERROR: crypto_ioremap_nocache for RSA failed!!\n");
       ret = HI_FAILURE;
       goto OUT2;
   }
#endif

#ifdef CIPHER_RNG_SUPPORT
   g_u32RngBase = (HI_U32)crypto_ioremap_nocache(REG_BASE_PHY_ADDR_RNG, 0x1000);
   if (g_u32RngBase == 0)
   {
       HI_ERR_CIPHER("ERROR: crypto_ioremap_nocache for RNG failed!!\n");
       ret = HI_FAILURE;
       goto OUT2;
   }
#endif

#ifdef CIPHER_KLAD_SUPPORT
	g_u32KladBase = (HI_U32)crypto_ioremap_nocache(REG_BASE_PHY_ADDR_KLAD, 0x100);
   if (g_u32KladBase == 0)
   {
       HI_ERR_CIPHER("ERROR: crypto_ioremap_nocache for KLAD failed!!\n");
       ret = HI_FAILURE;
       goto OUT2;
   }
#endif

#ifdef CIPHER_EFUSE_SUPPORT
    g_u32EfuseBase = (HI_U32)crypto_ioremap_nocache(REG_BASE_PHY_ADDR_EFUSE, 0x100);
    if (g_u32EfuseBase == 0)
    {
       HI_ERR_CIPHER("ERROR: crypto_ioremap_nocache for EFUSE failed!!\n");
       ret = HI_FAILURE;
       goto OUT2;
    }
#endif

#ifdef CIPHER_MULTICIPHER_SUPPORT
    ret = DRV_CIPHER_Init();
    if (HI_SUCCESS != ret)
    {
        goto OUT2;
    }
#endif

#ifdef CIPHER_HASH_SUPPORT
    (HI_VOID)HASH_DRV_ModInit();
    (HI_VOID)HAL_Cipher_HashSoftReset();
#endif

#ifndef DISABLE_DEBUG_INFO
    pstProcEntry = osal_create_proc_entry(UMAP_DEVNAME_CIPHER, HI_NULL);
    if (NULL == pstProcEntry)
    {
        HI_ERR_CIPHER("cipher: can't create %s.\n", CIPHER_PROC_NAME);
        ret = HI_FAILURE;
        goto OUT3;
    }
    pstProcEntry->read = CIPHER_ProcRead;
#endif
    HI_PRINT("Load hi_cipher.ko success. \n");

    return HI_SUCCESS;

#ifndef DISABLE_DEBUG_INFO
OUT3:
    #ifdef CIPHER_MULTICIPHER_SUPPORT
    DRV_CIPHER_DeInit();
    #endif
#endif

OUT2:
    if (g_u32CipherBase != 0)
    {
        crypto_iounmap((void*)g_u32CipherBase);
        g_u32CipherBase = 0;
    }
    if (g_u32HashBase != 0)
    {
        crypto_iounmap((void*)g_u32HashBase);
        g_u32HashBase = 0;
    }
    if (g_u32RsaBase != 0)
    {
        crypto_iounmap((void*)g_u32RsaBase);
        g_u32RsaBase = 0;
    }
    if (g_u32RngBase != 0)
    {
        crypto_iounmap((void*)g_u32RngBase);
        g_u32RngBase = 0;
    }
	if (g_u32KladBase != 0)
    {
        crypto_iounmap((void*)g_u32KladBase);
        g_u32KladBase = 0;
    }
    if (g_u32EfuseBase != 0)
    {
        crypto_iounmap((void*)g_u32EfuseBase);
        g_u32EfuseBase = 0;
    }
OUT1:
    osal_deregisterdevice(s_pstCipherDev);

OUT0:
    osal_destroydev(s_pstCipherDev);
    return ret;
}

HI_VOID CIPHER_DRV_ModExit(HI_VOID)
{
#ifndef DISABLE_DEBUG_INFO
    osal_remove_proc_entry(UMAP_DEVNAME_CIPHER, NULL);
#endif
#ifdef CIPHER_MULTICIPHER_SUPPORT
    (HI_VOID)DRV_CIPHER_DeInit();
#endif

#ifdef CIPHER_HASH_SUPPORT
    (HI_VOID)HASH_DRV_ModDeInit();
#endif

    osal_deregisterdevice(s_pstCipherDev);
    osal_destroydev(s_pstCipherDev);

    if (g_u32CipherBase != 0)
    {
        crypto_iounmap((void*)g_u32CipherBase);
        g_u32CipherBase = 0;
    }
    if (g_u32HashBase != 0)
    {
        crypto_iounmap((void*)g_u32HashBase);
        g_u32HashBase = 0;
    }
    if (g_u32RsaBase != 0)
    {
        crypto_iounmap((void*)g_u32RsaBase);
        g_u32RsaBase = 0;
    }
    if (g_u32RngBase != 0)
    {
        crypto_iounmap((void*)g_u32RngBase);
        g_u32RngBase = 0;
    }
	if (g_u32KladBase != 0)
    {
        crypto_iounmap((void*)g_u32KladBase);
        g_u32KladBase = 0;
    }
    if (g_u32EfuseBase != 0)
    {
        crypto_iounmap((void*)g_u32EfuseBase);
        g_u32EfuseBase = 0;
    }

    HI_PRINT("Unload hi_cipher.ko success.\n");
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
