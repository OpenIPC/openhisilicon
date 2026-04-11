#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <assert.h>


#include "hi_type.h"
#include "hi_unf_ecs.h"
#include "hi_error_ecs.h"

#include "mmz-userdev.h"

#define HIAPI_RUN_RETURN(ret) \
do {\
    if ( ret != HI_SUCCESS)\
    {\
        printf("\033[0;31m<%s>not pass at line:%d err:%x\033[0;39m\n",\
            __FUNCTION__,__LINE__,ret);\
        return ret;\
     }\
}while(0)

/* must load MMZ first */
static HI_S32 g_s32MmzFd = -1;
    
#define CI_CHECK_MMZ_OPEN() do{\
if(g_s32MmzFd < 0) \
{\
    g_s32MmzFd = open("/dev/mmz_userdev", O_RDWR);\
}\
}while(0)


/* user can call HI_MPI_SYS_MmzAlloc to replace*/
HI_S32 SAMPLE_CI_MmzAlloc(HI_U32 *pu32PhyAddr, void **ppVitAddr, const HI_CHAR *pstrMmb, const HI_CHAR *pstrZone, HI_U32 u32Len)
{
    struct mmb_info mmi = {0};

    CI_CHECK_MMZ_OPEN();

    mmi.size = u32Len;
    mmi.prot = PROT_READ | PROT_WRITE;
    mmi.flags = MAP_SHARED;
#if 1
    if (NULL != pstrMmb)
    {
        strncpy(mmi.mmb_name, pstrMmb, sizeof(mmi.mmb_name));
    }
    if (NULL != pstrZone)
    {
        strncpy(mmi.mmz_name, pstrZone, sizeof(mmi.mmz_name));
    }
#endif
    if (ioctl(g_s32MmzFd, IOC_MMB_ALLOC, &mmi) != HI_SUCCESS)
    {
    	printf("System alloc mmz memory failed!\n");
        return HI_FAILURE;
    }
    
    if (ioctl(g_s32MmzFd, IOC_MMB_USER_REMAP, &mmi) != HI_SUCCESS)
    {
    	printf("System remap mmz memory failed!\n");
        return HI_FAILURE;
    }
    
    *pu32PhyAddr = mmi.phys_addr;
    *ppVitAddr = mmi.mapped;
    
    return HI_SUCCESS;
}    

/* user can call HI_MPI_SYS_MmzFree to replace*/
HI_S32 SAMPLE_CI_MmzFree(HI_U32 u32PhyAddr)
{
    struct mmb_info mmi = {0};

    CI_CHECK_MMZ_OPEN();

    mmi.phys_addr = u32PhyAddr;
    
    if (ioctl(g_s32MmzFd, IOC_MMB_USER_UNMAP, &mmi) != HI_SUCCESS)
    {
    	printf("System unmap mmz memory failed!\n");
        return HI_FAILURE;
    }
    
    if (ioctl(g_s32MmzFd, IOC_MMB_FREE, &mmi) != HI_SUCCESS)
    {
    	printf("System free mmz memory failed!\n");
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}


HI_S32 Setconfiginfo(HI_HANDLE chnHandle)
{
    HI_U32 i;
    HI_U32 ckey = 'a';
    HI_UNF_CIPHER_CTRL_S CipherCtrl;

    CipherCtrl.bKeyByCA = HI_FALSE;
    CipherCtrl.enAlg = HI_UNF_CIPHER_ALG_AES;
    CipherCtrl.enWorkMode = HI_UNF_CIPHER_WORK_MODE_ECB;
    CipherCtrl.enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_8BIT;
    CipherCtrl.enKeyLen = HI_UNF_CIPHER_KEY_AES_128BIT;

    for (i = 0; i < 8; i++)
    {
        CipherCtrl.u32Key[i] = ckey++;
    }

    for (i = 0; i < 4; i++)
    {
        CipherCtrl.u32IV[i] = ckey++;
    }

    HIAPI_RUN_RETURN(HI_UNF_CIPHER_ConfigHandle(chnHandle, &CipherCtrl));

    return HI_SUCCESS;
}

int main(int argc, char* argv[])
{
    HI_U32 Testlen = 1024;

    HI_U32 inputAddrPhy;
    HI_U32 outPutAddrPhy;
    HI_U32 resultAddrPhy;

    //HI_U32 Testcached = 0;

    HI_U8* inputAddrVir;
    HI_U8* outputAddrVir;
    HI_U8* resultAddrVir;

    HI_HANDLE TestchnidEnc = 0;
    HI_HANDLE TestchnidDec = 0;

    HIAPI_RUN_RETURN(HI_UNF_CIPHER_Open());

    printf("open cipher success!\n");

    HIAPI_RUN_RETURN(HI_UNF_CIPHER_CreateHandle(&TestchnidEnc));

    HIAPI_RUN_RETURN(HI_UNF_CIPHER_CreateHandle(&TestchnidDec));

    printf("creathandle success! the chnid is %#x\n", TestchnidEnc);

    /*************************************************************/
    HIAPI_RUN_RETURN(SAMPLE_CI_MmzAlloc(&inputAddrPhy,(HI_VOID **)&inputAddrVir,
        "cipherIn",NULL,Testlen));

    HIAPI_RUN_RETURN(SAMPLE_CI_MmzAlloc(&outPutAddrPhy,(HI_VOID **)&outputAddrVir,
        "cipherOut",NULL,Testlen));

    HIAPI_RUN_RETURN(SAMPLE_CI_MmzAlloc(&resultAddrPhy,(HI_VOID **)&resultAddrVir,
        "cipherResult",NULL,Testlen));

    /*************************************************************/
    memset(inputAddrVir, 'I', Testlen);
    memset(outputAddrVir, 'O', Testlen);
    memset(resultAddrVir, 'R', Testlen);

    HIAPI_RUN_RETURN(Setconfiginfo(TestchnidEnc));
    HIAPI_RUN_RETURN(HI_UNF_CIPHER_Encrypt(TestchnidEnc, inputAddrPhy, outPutAddrPhy, Testlen));

    HIAPI_RUN_RETURN(Setconfiginfo(TestchnidDec));
    HIAPI_RUN_RETURN(HI_UNF_CIPHER_Decrypt(TestchnidDec, outPutAddrPhy, resultAddrPhy, Testlen));

    if (0 != memcmp(resultAddrVir, inputAddrVir, Testlen))
    {
        printf("encrypt and decrypt test fail!");
    }

    printf("test is over ,encrypt and decrypt OK!");
    HIAPI_RUN_RETURN(SAMPLE_CI_MmzFree(inputAddrPhy));
    HIAPI_RUN_RETURN(SAMPLE_CI_MmzFree(outPutAddrPhy));
    HIAPI_RUN_RETURN(SAMPLE_CI_MmzFree(resultAddrPhy));

    HIAPI_RUN_RETURN(HI_UNF_CIPHER_DestroyHandle(TestchnidDec));
    HIAPI_RUN_RETURN(HI_UNF_CIPHER_DestroyHandle(TestchnidEnc));
    HIAPI_RUN_RETURN(HI_UNF_CIPHER_Close());

    printf("close cipher success!\n");
    
    return HI_SUCCESS;
}
