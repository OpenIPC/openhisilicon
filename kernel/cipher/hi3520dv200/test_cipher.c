/**
\file
\brief cipher test file
\copyright Shenzhen Hisilicon Co., Ltd.
\date 2008-2018
\version draft
\author QuYaxin 46153
\date 2009-11-9
*/

#include "drv_cipher.h"
#include "cipher_mmz.h"
#include "cipher_log.h"


typedef HI_S32 (*test_func)(void);

/***************************** Macro Definition ******************************/
#define CI_TEST_RUN(function, ret) \
    do{ \
        ret = function; \
        if (ret != HI_SUCCESS) \
        { \
            printk("error when do %s, ERRNO:%#x. func:%s,line:%d\n", #function, ret, __FUNCTION__, __LINE__); \
        } \
    }while(0)

#define CI_TEST_CHECK_RET(ret) do { \
        if (ret != HI_SUCCESS)\
        {\
            printk("Case Failed, ERR:%#x, CaseName:%s\n", ret, __FUNCTION__);\
            return ret;        \
        }\
}while(0)

#define CI_TEST_CASE_END(ret) do { \
        if (ret != HI_SUCCESS)\
        {\
            printk("\n**Case Failed, ERR:%#x, CaseName:%s\n\n", ret, __FUNCTION__);\
            return ret;        \
        }\
        else\
        {\
            printk("\n**Case Success, CaseName:%s\n\n", __FUNCTION__);\
            return ret;        \
        }\
}while(0)



/*************************** Structure Definition ****************************/
#define CIPHER_TEST_BUF_SIZE      (10240)
#define CHAN_BUF_CNT              (100)

/*************************** Key Ladder Register Definition ****************************/
HI_U32  REG_KL_BASE_ADDR_VIRT   ;  

/* cipher reg read and write macro define */
#ifndef KL_READ_REG
#define KL_READ_REG(reg, result) ((result) = *(volatile unsigned int *)(reg))
#endif

#ifndef KL_WRITE_REG
#define KL_WRITE_REG(reg, data) (*((volatile unsigned int *)(reg)) = (data))
#endif

//#define  REG_KL_BASE_ADDR_PHY      0x00020000
#define  REG_KL_BASE_ADDR_PHY      0x90000000

#define  REG_KL_BASE_ADDR (REG_KL_BASE_ADDR_VIRT)


#define  REG_KL_SECURE_BOOT_CFG        (REG_KL_BASE_ADDR + 0x0000 )              
#define  REG_KL_SECURE_BOOT_STAT       (REG_KL_BASE_ADDR + 0x0004 )
#define  REG_KL_MARKET_SID_REF         (REG_KL_BASE_ADDR + 0x0008 )
#define  REG_KL_CHIP_ID                (REG_KL_BASE_ADDR + 0x000C )
#define  REG_KL_CA_DIN(i)              (REG_KL_BASE_ADDR + 0x0020 + (i)*0x4 )
#define  REG_KL_CA_CTRL                (REG_KL_BASE_ADDR + 0x0030 )
#define  REG_KL_CA_SET                 (REG_KL_BASE_ADDR + 0x0034 )
#define  REG_KL_CA_STAT                (REG_KL_BASE_ADDR + 0x0038 )
#define  REG_KL_CA_INT_STAT            (REG_KL_BASE_ADDR + 0x0040 )
#define  REG_KL_CA_INT_EN              (REG_KL_BASE_ADDR + 0x0044 )
#define  REG_KL_CA_INT_RAW             (REG_KL_BASE_ADDR + 0x0048 )
#define  REG_KL_CA_LOAD_OTP            (REG_KL_BASE_ADDR + 0x004C )
#define  REG_KL_CA_RST_ST              (REG_KL_BASE_ADDR + 0x0050 )
#define  REG_KL_KEY_DOUT(i)            (REG_KL_BASE_ADDR + 0x0060 + (i)*0x4 )
/********************** Global Variable declaration **************************/
static HI_U32 g_u32CiTestChnID[CIPHER_SOFT_CHAN_NUM];


static CI_MMZ_BUF_S g_stInputBuf[CIPHER_SOFT_CHAN_NUM][CHAN_BUF_CNT];
static CI_MMZ_BUF_S g_stOutputBuf[CIPHER_SOFT_CHAN_NUM][CHAN_BUF_CNT]; 
static CI_MMZ_BUF_S g_stResultBuf[CIPHER_SOFT_CHAN_NUM][CHAN_BUF_CNT]; /* the result we want */

volatile static int g_bCipherTestDone = 0;

/******************************* API declaration *****************************/

HI_S32 CIPHER_Test_Init(HI_VOID)
{
    HI_S32 ret;
    HI_U32 i;
    HI_U32 j;
    
	REG_KL_BASE_ADDR_VIRT = (HI_U32)ioremap_nocache(REG_KL_BASE_ADDR_PHY, 8*1024);
	
    for (i = 0; i < CIPHER_SOFT_CHAN_NUM; i++)
    {
        g_u32CiTestChnID[i] = i;
//		printk("open soft chn:%d.\n", i);
        CI_TEST_RUN(DRV_Cipher_OpenChn(g_u32CiTestChnID[i]),ret);
        CI_TEST_CHECK_RET(ret);
    }

	for (i = 0; i < CIPHER_SOFT_CHAN_NUM; i++)
	{
		for(j = 0; j < CHAN_BUF_CNT; j++)
		{
		    CI_TEST_RUN(CI_MmzAlloc("CI_in", NULL, CIPHER_TEST_BUF_SIZE, 0, &g_stInputBuf[i][j]), ret);
		    CI_TEST_RUN(CI_MmzAlloc("CI_out", NULL, CIPHER_TEST_BUF_SIZE, 0, &g_stOutputBuf[i][j]), ret);
		    CI_TEST_RUN(CI_MmzAlloc("CI_rst", NULL, CIPHER_TEST_BUF_SIZE, 0, &g_stResultBuf[i][j]), ret);
		}
	}

    for(i = 1; i < CIPHER_SOFT_CHAN_NUM; i++)
   	{
   		for(j = 0; j < CHAN_BUF_CNT; j++)
   		{
		    memset((void*)(g_stInputBuf[i][j].u32StartVirAddr),  0, 1);    
		    memset((void*)(g_stOutputBuf[i][j].u32StartVirAddr), 1, 1);    
		    memset((void*)(g_stResultBuf[i][j].u32StartVirAddr), 2, 1);    
		}
	}
	
	HI_INFO_CIPHER("Test init OK.\n");
    return HI_SUCCESS;
}

HI_S32 CIPHER_Test_DeInit(HI_VOID)
{
    HI_U32 i,j;
    HI_S32 ret, retTotal = HI_SUCCESS;
    
    for (i = 0; i < CIPHER_SOFT_CHAN_NUM; i++)
    {
        CI_TEST_RUN(DRV_Cipher_CloseChn(g_u32CiTestChnID[i]), ret);
        retTotal |= ret;
    }

    for (i = 0; i < CIPHER_SOFT_CHAN_NUM; i++)
	{
		for(j = 0; j < CHAN_BUF_CNT; j++)
		{
		    CI_MmzFree(&g_stInputBuf[i][j]);
		    CI_MmzFree(&g_stOutputBuf[i][j]);
		    CI_MmzFree(&g_stResultBuf[i][j]);
		}
	}
    
    CI_TEST_CHECK_RET(ret);

    return ret;
}

static inline void CIPHER_TestSetAttr(HI_UNF_CIPHER_CTRL_S *pAlgCtrl, HI_BOOL bKeyByCA,
    HI_UNF_CIPHER_ALG_E alg, HI_UNF_CIPHER_BIT_WIDTH_E bitWidth, HI_UNF_CIPHER_KEY_LENGTH_E keyLen, 
    HI_UNF_CIPHER_WORK_MODE_E workMode)
{
    pAlgCtrl->bKeyByCA = bKeyByCA;
    pAlgCtrl->enAlg = alg;
    pAlgCtrl->enBitWidth = bitWidth;
    pAlgCtrl->enKeyLen = keyLen;
    pAlgCtrl->enWorkMode = workMode;
}

static inline void CIPHER_TestSetIV(HI_UNF_CIPHER_CTRL_S *pAlgCtrl, HI_U32 iv0, HI_U32 iv1,HI_U32 iv2,HI_U32 iv3)
{
    pAlgCtrl->u32IV[0] = iv0;
    pAlgCtrl->u32IV[1] = iv1;
    pAlgCtrl->u32IV[2] = iv2;
    pAlgCtrl->u32IV[3] = iv3;
}

static inline void CIPHER_TestSetKey(HI_UNF_CIPHER_CTRL_S *pAlgCtrl, HI_U32 key0,
    HI_U32 key1,HI_U32 key2,HI_U32 key3,
    HI_U32 key4, HI_U32 key5,HI_U32 key6,HI_U32 key7)
{
    pAlgCtrl->u32Key[0] = key0;
    pAlgCtrl->u32Key[1] = key1;
    pAlgCtrl->u32Key[2] = key2;
    pAlgCtrl->u32Key[3] = key3;
    pAlgCtrl->u32Key[4] = key4;
    pAlgCtrl->u32Key[5] = key5;
    pAlgCtrl->u32Key[6] = key6;
    pAlgCtrl->u32Key[7] = key7;
}

HI_VOID CIPHER_Test_CallBack(HI_U32 args)
{
    g_bCipherTestDone--;
}

HI_S32 CIPHER_Test_WaitDone(HI_U32 timeOutMs)
{
	HI_U32 i = 0;
	
	do
	{
		msleep(1);
		i++;

		if ((i+1)%timeOutMs == 0)
		{
			HI_INFO_CIPHER("wait to done:%d, g_bCipherTestDone:%d\n", i,g_bCipherTestDone);
		}
	}while(i < timeOutMs && g_bCipherTestDone > 0);
//	}while(g_bCipherTestDone > 0);

	if (i >= timeOutMs)
	{
		printk("task NOT done int %d ms!, g_bCipherTestDone=%d\n", timeOutMs, g_bCipherTestDone);
		return HI_FAILURE;
	}
	
	return HI_SUCCESS;
}

//CIPHER_Test_Case0:  just test TDES r2r or cw key ladder;
//AES r2r 3 key ladder is test in CIPHER_Test_Case8
HI_S32 CIPHER_Test_Case0(HI_VOID)
{
	HI_S32 ret;
	HI_S32 regAddr;
	HI_S32 regData;
	HI_U32 i;
	HI_U32 r2r_st0[4] = {0x20776f4e,0x74207369,0x33221100,0x77665544};
	HI_U32 r2r_gd0[4] = {0x8c535758,0x04fead10,0x74276bea,0xce2a94b7};
	HI_U32 r2r_st1[4] = {0xff12893e,0x74b1bca6,0x73afdd00,0x90cd5586};
	HI_U32 r2r_gd1[4] = {0xa5f652fb,0xe5e6c1a5,0xba848d3c,0x178b758e};
	HI_U32 cw_st0[4]  = {0x20776f4e,0x74207369,0x33221100,0x77665544};
	HI_U32 cw_gd0[4]  = {0x6e24b629,0x7ed77fbf,0xb2fd269f,0x5aa34aa9};
	HI_U32 cw_st1[4]  = {0xff12893e,0x74b1bca6,0,0};
	HI_U32 cw_gd1[4]  = {0x21e0e0bc,0x97530999,0,0};
	HI_U32 r2r_rd[4]  = {0,0,0,0};


	regAddr = REG_KL_SECURE_BOOT_CFG;  
    KL_READ_REG(regAddr, regData);
	if (regData != 0x56)
	{
		HI_ERR_CIPHER("REG_KL_SECURE_BOOT_CFG is Error, read=%x\n", regData);
		ret = HI_FAILURE;
		return ret;
	}

	regAddr = REG_KL_MARKET_SID_REF;  
    KL_READ_REG(regAddr, regData);
	if (regData != 0xe3e2e1e0)
	{
		HI_ERR_CIPHER("REG_KL_MARKET_SID_REF is Error, read=%x\n", regData);
		ret = HI_FAILURE;
		return ret;
	}

	regAddr = REG_KL_CHIP_ID;  
    KL_READ_REG(regAddr, regData);
	if (regData != 0xff030201)
	{
		HI_ERR_CIPHER("REG_KL_CHIP_ID is Error, read=%x\n", regData);
		ret = HI_FAILURE;
		return ret;
	}

	//start ram2ram key ladder test
	regData = 0xa;
	regAddr = REG_KL_CA_SET;  
    KL_WRITE_REG(regAddr, regData);

// ---------------------test ram2ram klad0------------------------------
	//set din
	for(i=0; i<4; i++)
	{
		regData = r2r_st0[i];
		regAddr = REG_KL_CA_DIN(i); 
		KL_WRITE_REG(regAddr, regData);
	}

	regData = 0xa8;
	regAddr = REG_KL_CA_CTRL; 
	KL_WRITE_REG(regAddr, regData);

	//wait interrupt
    regAddr = REG_KL_CA_INT_RAW;  
    KL_READ_REG(regAddr, regData);
    while((regData & 0x1) == 0x0)
    {
    	KL_READ_REG(regAddr, regData);
    }
    //clear interrupt
    regData = 0x1;
    KL_WRITE_REG(regAddr, regData);

	for(i=0; i<4; i++)
	{
		regAddr = REG_KL_KEY_DOUT(i);
		KL_READ_REG(regAddr, regData);
		r2r_rd[i] = regData;
	}

	if (memcmp(r2r_rd, r2r_gd0, 4))
    {
    	printk("R2R test 0 error Data NOT match!\n");
        ret = HI_FAILURE;
        return ret;
    }
    else
    {
        ret = HI_SUCCESS;
    }

// ---------------------test ram2ram klad 1------------------------------
    //set din
	for(i=0; i<4; i++)
	{
		regData = r2r_st1[i];
		regAddr = REG_KL_CA_DIN(i); 
		KL_WRITE_REG(regAddr, regData);
	}

	regData = 0x1aa;
	regAddr = REG_KL_CA_CTRL; 
	KL_WRITE_REG(regAddr, regData);

	//wait interrupt
    regAddr = REG_KL_CA_INT_RAW;  
    KL_READ_REG(regAddr, regData);
    while((regData & 0x1) == 0x0)
    {
    	KL_READ_REG(regAddr, regData);
    }
    //clear interrupt
    regData = 0x1;
    KL_WRITE_REG(regAddr, regData);

	for(i=0; i<4; i++)
	{
		regAddr = REG_KL_KEY_DOUT(i);
		KL_READ_REG(regAddr, regData);
		r2r_rd[i] = regData;
	}

	if (memcmp(r2r_rd, r2r_gd1, 4))
    {
    	printk("R2R test 1 error Data NOT match!\n");
        ret = HI_FAILURE;
        return ret;
    }
    else
    {
        ret = HI_SUCCESS;
    }

    //----------------start CW key klad0-----------------
    //set din
	for(i=0; i<4; i++)
	{
		regData = cw_st0[i];
		regAddr = REG_KL_CA_DIN(i); 
		KL_WRITE_REG(regAddr, regData);
	}

	regData = 0xa0;
	regAddr = REG_KL_CA_CTRL; 
	KL_WRITE_REG(regAddr, regData);

	//wait interrupt
    regAddr = REG_KL_CA_INT_RAW;  
    KL_READ_REG(regAddr, regData);
    while((regData & 0x1) == 0x0)
    {
    	KL_READ_REG(regAddr, regData);
    }
    //clear interrupt
    regData = 0x1;
    KL_WRITE_REG(regAddr, regData);

	for(i=0; i<4; i++)
	{
		regAddr = REG_KL_KEY_DOUT(i);
		KL_READ_REG(regAddr, regData);
		r2r_rd[i] = regData;
	}

	if (memcmp(r2r_rd, cw_gd0, 4))
    {
    	printk("R2R test 1 error Data NOT match!\n");
        ret = HI_FAILURE;
        return ret;
    }
    else
    {
        ret = HI_SUCCESS;
    }
    
    //----------------start CW klad1-----------------
   	    //set din
	for(i=0; i<2; i++)
	{
		regData = cw_st1[i];
		regAddr = REG_KL_CA_DIN(i); 
		KL_WRITE_REG(regAddr, regData);
	}

	regData = 0x2a2;
	regAddr = REG_KL_CA_CTRL; 
	KL_WRITE_REG(regAddr, regData);

	//wait interrupt
    regAddr = REG_KL_CA_INT_RAW;  
    KL_READ_REG(regAddr, regData);
    while((regData & 0x1) == 0x0)
    {
    	KL_READ_REG(regAddr, regData);
    }
    //clear interrupt
    regData = 0x1;
    KL_WRITE_REG(regAddr, regData);

	for(i=0; i<2; i++)
	{
		regAddr = REG_KL_KEY_DOUT(i);
		KL_READ_REG(regAddr, regData);
		r2r_rd[i] = regData;
	}

	if (memcmp(r2r_rd, cw_gd1, 2))
    {
    	printk("R2R test 1 error Data NOT match!\n");
        ret = HI_FAILURE;
        return ret;
    }
    else
    {
        ret = HI_SUCCESS;
    }

    // ---------------------test ram2ram klad 1------------------------------
    //set din
	for(i=0; i<4; i++)
	{
		regData = r2r_st1[i];
		regAddr = REG_KL_CA_DIN(i); 
		KL_WRITE_REG(regAddr, regData);
	}

	regData = 0x1aa;
	regAddr = REG_KL_CA_CTRL; 
	KL_WRITE_REG(regAddr, regData);

	//wait interrupt
    regAddr = REG_KL_CA_INT_RAW;  
    KL_READ_REG(regAddr, regData);
    while((regData & 0x1) == 0x0)
    {
    	KL_READ_REG(regAddr, regData);
    }
    //clear interrupt
    regData = 0x1;
    KL_WRITE_REG(regAddr, regData);

	for(i=0; i<4; i++)
	{
		regAddr = REG_KL_KEY_DOUT(i);
		KL_READ_REG(regAddr, regData);
		r2r_rd[i] = regData;
	}

	if (memcmp(r2r_rd, r2r_gd1, 4))
    {
    	printk("R2R test 1 error Data NOT match!\n");
        ret = HI_FAILURE;
        return ret;
    }
    else
    {
        ret = HI_SUCCESS;
    }

    return HI_SUCCESS;
}

HI_S32 CIPHER_Test_Case1(HI_VOID)
{
    HI_S32 ret;
    HI_U32 testSize = 16;
    HI_U32 i;

    HI_UNF_CIPHER_CTRL_S AlgCtrl;
    CIPHER_TASK_S task;

    for(i=0; i<10; i++)
    {
	    memset((void*)(g_stInputBuf[0][0].u32StartVirAddr),  (i), testSize);    
	    memset((void*)(g_stOutputBuf[0][0].u32StartVirAddr), (i + 3), testSize);    
	    memset((void*)(g_stResultBuf[0][0].u32StartVirAddr), (i + 5), testSize);    

	    CIPHER_TestSetAttr(&AlgCtrl, HI_FALSE, HI_UNF_CIPHER_ALG_AES,
	        HI_UNF_CIPHER_BIT_WIDTH_128BIT, HI_UNF_CIPHER_KEY_AES_128BIT, HI_UNF_CIPHER_WORK_MODE_ECB);
	    CIPHER_TestSetIV(&AlgCtrl, 0, 0, 0, 0);
	    CIPHER_TestSetKey(&AlgCtrl, 0x16157e2b, 0xa6d2ae28, 0x8815f7ab, 0x3c4fcf09, 0, 0, 0, 0);

	    task.stData2Process.bDecrypt = HI_FALSE;
	    task.stData2Process.u32src = g_stInputBuf[0][0].u32StartPhyAddr;
	    task.stData2Process.u32dest = g_stOutputBuf[0][0].u32StartPhyAddr;    
	    task.stData2Process.u32length = testSize;
	    task.u32CallBackArg = task.stData2Process.u32src;
	    
	    memcpy(task.stData2Process.u32DataPkg, (HI_VOID *)g_stInputBuf[0][0].u32StartVirAddr, 16);
	    CI_TEST_RUN(DRV_Cipher_ConfigChn(g_u32CiTestChnID[0], &AlgCtrl, CIPHER_Test_CallBack), ret);
	    g_bCipherTestDone = 1;
	    CI_TEST_RUN(DRV_Cipher_CreatTask(g_u32CiTestChnID[0], &task, AlgCtrl.u32Key, AlgCtrl.u32IV), ret);
		ret = CIPHER_Test_WaitDone(10);
		if (HI_SUCCESS == ret)
		{
			HI_INFO_CIPHER("wait 1 ok\n");
		} else {
			return ret;
		}
		memcpy((HI_VOID *)g_stOutputBuf[0][0].u32StartVirAddr, (HI_VOID *)task.stData2Process.u32DataPkg, 16);
	    task.stData2Process.bDecrypt = HI_TRUE;
	    task.stData2Process.u32src = g_stOutputBuf[0][0].u32StartPhyAddr;
	    task.stData2Process.u32dest = g_stResultBuf[0][0].u32StartPhyAddr;    
	    task.stData2Process.u32length = testSize;
	    task.u32CallBackArg = task.stData2Process.u32src;   
	    
	    CI_TEST_RUN(DRV_Cipher_ConfigChn(g_u32CiTestChnID[0], &AlgCtrl, CIPHER_Test_CallBack), ret);
		g_bCipherTestDone++;
	    CI_TEST_RUN(DRV_Cipher_CreatTask(g_u32CiTestChnID[0], &task, AlgCtrl.u32Key, AlgCtrl.u32IV), ret);
		ret = CIPHER_Test_WaitDone(10);
		if (HI_SUCCESS == ret)
		{	
			memcpy((HI_VOID *)g_stResultBuf[0][0].u32StartVirAddr, task.stData2Process.u32DataPkg, 16);
		    if (memcmp((void*)(g_stInputBuf[0][0].u32StartVirAddr), (void*)(g_stResultBuf[0][0].u32StartVirAddr), testSize))
		    {
		    	printk("Data NOT match!  i = %d\n", i);
		        ret = HI_FAILURE;
				return ret;
		    }
		    else
		    {
		        ret = HI_SUCCESS;
		    }
	    }
	}
    
    CI_TEST_CASE_END(ret);
    
    return HI_SUCCESS; 
}



HI_S32 CIPHER_Test_Case2(HI_VOID)
{
    HI_S32 ret;
    HI_U32 testSize = CIPHER_TEST_BUF_SIZE;

    HI_UNF_CIPHER_CTRL_S AlgCtrl;
    CIPHER_TASK_S task;
/*
    memset((void*)(g_stInputBuf[1][0].u32StartVirAddr),  0, testSize);    
    memset((void*)(g_stOutputBuf[1][0].u32StartVirAddr), 1, testSize);    
    memset((void*)(g_stResultBuf[1][0].u32StartVirAddr), 2, testSize);    ///???
*/
    CIPHER_TestSetAttr(&AlgCtrl, HI_FALSE, HI_UNF_CIPHER_ALG_AES,
        HI_UNF_CIPHER_BIT_WIDTH_128BIT, HI_UNF_CIPHER_KEY_AES_128BIT, HI_UNF_CIPHER_WORK_MODE_ECB);
    CIPHER_TestSetIV(&AlgCtrl, 0, 0, 0, 0);
    CIPHER_TestSetKey(&AlgCtrl, 0, 0, 0, 0, 0, 0, 0, 0);

    task.stData2Process.bDecrypt = HI_FALSE;
    task.stData2Process.u32src = g_stInputBuf[1][0].u32StartPhyAddr;
    task.stData2Process.u32dest = g_stOutputBuf[1][0].u32StartPhyAddr;    
    task.stData2Process.u32length = testSize;
    task.u32CallBackArg = task.stData2Process.u32src;
    
    CI_TEST_RUN(DRV_Cipher_ConfigChn(g_u32CiTestChnID[1], &AlgCtrl, CIPHER_Test_CallBack), ret);
    g_bCipherTestDone++;
    CI_TEST_RUN(DRV_Cipher_CreatTask(g_u32CiTestChnID[1], &task, NULL, NULL), ret);
	ret = CIPHER_Test_WaitDone(20);
	if (HI_SUCCESS == ret)
	{
		HI_INFO_CIPHER("wait 1 ok\n");
	}
    task.stData2Process.bDecrypt = HI_TRUE;
    task.stData2Process.u32src = g_stOutputBuf[1][0].u32StartPhyAddr;
    task.stData2Process.u32dest = g_stResultBuf[1][0].u32StartPhyAddr;    
    task.stData2Process.u32length = testSize;
    task.u32CallBackArg = task.stData2Process.u32src;   
    
    CI_TEST_RUN(DRV_Cipher_ConfigChn(g_u32CiTestChnID[1], &AlgCtrl, CIPHER_Test_CallBack), ret);
	g_bCipherTestDone++;
    CI_TEST_RUN(DRV_Cipher_CreatTask(g_u32CiTestChnID[1], &task, NULL, NULL), ret);

	ret = CIPHER_Test_WaitDone(20);
	if (HI_SUCCESS == ret)
	{
	    if (memcmp((void*)(g_stInputBuf[1][0].u32StartVirAddr), (void*)(g_stResultBuf[1][0].u32StartVirAddr), testSize))
	    {
	    	printk("Data NOT match!\n");
	        ret = HI_FAILURE;
	    }
	    else
	    {
	        ret = HI_SUCCESS;
	    }
    }
    
    CI_TEST_CASE_END(ret);
    
    return HI_SUCCESS; 
}

HI_S32 CIPHER_Test_Case3(HI_VOID)
{
    HI_S32 ret;
    HI_U32 testSize = CIPHER_TEST_BUF_SIZE;
    HI_U32 i,j;

    HI_UNF_CIPHER_CTRL_S AlgCtrl[CIPHER_SOFT_CHAN_NUM];
    CIPHER_TASK_S task;
/*
    for(i = 1; i < CIPHER_SOFT_CHAN_NUM; i++)
   	{
   		for(j = 0; j < CHAN_BUF_CNT; j++)
   		{
		    memset((void*)(g_stInputBuf[i][j].u32StartVirAddr),  0, 16);    
		    memset((void*)(g_stOutputBuf[i][j].u32StartVirAddr), 1, 16);    
		    memset((void*)(g_stResultBuf[i][j].u32StartVirAddr), 2, 16);    
		}
	}
*/	
    CIPHER_TestSetAttr(&AlgCtrl[1], HI_FALSE, HI_UNF_CIPHER_ALG_AES,
        HI_UNF_CIPHER_BIT_WIDTH_128BIT, HI_UNF_CIPHER_KEY_AES_128BIT, HI_UNF_CIPHER_WORK_MODE_ECB);
    CIPHER_TestSetIV(&AlgCtrl[1], 0x00112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[1], 0x11, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7);

    CIPHER_TestSetAttr(&AlgCtrl[2], HI_FALSE, HI_UNF_CIPHER_ALG_AES,
        HI_UNF_CIPHER_BIT_WIDTH_128BIT, HI_UNF_CIPHER_KEY_AES_128BIT, HI_UNF_CIPHER_WORK_MODE_CBC);
    CIPHER_TestSetIV(&AlgCtrl[2], 0x22112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[2], 0x22, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7);

    CIPHER_TestSetAttr(&AlgCtrl[3], HI_FALSE, HI_UNF_CIPHER_ALG_3DES,
        HI_UNF_CIPHER_BIT_WIDTH_64BIT, HI_UNF_CIPHER_KEY_DES_2KEY, HI_UNF_CIPHER_WORK_MODE_ECB);
    CIPHER_TestSetIV(&AlgCtrl[3], 0x33112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[3], 0x33, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7);

    CIPHER_TestSetAttr(&AlgCtrl[4], HI_FALSE, HI_UNF_CIPHER_ALG_3DES,
        HI_UNF_CIPHER_BIT_WIDTH_64BIT, HI_UNF_CIPHER_KEY_DES_3KEY, HI_UNF_CIPHER_WORK_MODE_ECB);
    CIPHER_TestSetIV(&AlgCtrl[4], 0x44112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[4], 0x44, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7);

    CIPHER_TestSetAttr(&AlgCtrl[5], HI_FALSE, HI_UNF_CIPHER_ALG_DES,
        HI_UNF_CIPHER_BIT_WIDTH_64BIT, HI_UNF_CIPHER_KEY_AES_128BIT, HI_UNF_CIPHER_WORK_MODE_ECB);
    CIPHER_TestSetIV(&AlgCtrl[5], 0x00112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[5], 0x55, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7);

    CIPHER_TestSetAttr(&AlgCtrl[6], HI_FALSE, HI_UNF_CIPHER_ALG_DES,
        HI_UNF_CIPHER_BIT_WIDTH_64BIT, HI_UNF_CIPHER_KEY_AES_128BIT, HI_UNF_CIPHER_WORK_MODE_CBC);
    CIPHER_TestSetIV(&AlgCtrl[6], 0x66112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[6], 0x66, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7);

    CIPHER_TestSetAttr(&AlgCtrl[7], HI_FALSE, HI_UNF_CIPHER_ALG_AES,
        HI_UNF_CIPHER_BIT_WIDTH_128BIT, HI_UNF_CIPHER_KEY_AES_256BIT, HI_UNF_CIPHER_WORK_MODE_CTR);
    CIPHER_TestSetIV(&AlgCtrl[7], 0x77112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[7], 0x77, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7);    

	for(i = 1; i < CIPHER_SOFT_CHAN_NUM; i++)
	{
		 CI_TEST_RUN(DRV_Cipher_ConfigChn(g_u32CiTestChnID[i], &AlgCtrl[i], CIPHER_Test_CallBack), ret);
	}

	g_bCipherTestDone = CHAN_BUF_CNT * (CIPHER_SOFT_CHAN_NUM - 1);
	for(j = 0; j < CHAN_BUF_CNT; j++)
	{
		for(i = 1; i < CIPHER_SOFT_CHAN_NUM; i++)
		{
		    task.stData2Process.bDecrypt = HI_FALSE;
		    task.stData2Process.u32src = g_stInputBuf[i][j].u32StartPhyAddr;
		    task.stData2Process.u32dest = g_stOutputBuf[i][j].u32StartPhyAddr;    
		    task.stData2Process.u32length = testSize;
		    task.u32CallBackArg = task.stData2Process.u32src;

//    		g_bCipherTestDone++;	
		    CI_TEST_RUN(DRV_Cipher_CreatTask(g_u32CiTestChnID[i], &task, AlgCtrl[i].u32Key, AlgCtrl[i].u32IV), ret);
		}
    }
	ret = CIPHER_Test_WaitDone(200);
	if (HI_SUCCESS == ret)
	{
		HI_INFO_CIPHER("wait 1 ok\n");
	}

	g_bCipherTestDone = CHAN_BUF_CNT * (CIPHER_SOFT_CHAN_NUM - 1);
	for(j = 0; j < CHAN_BUF_CNT; j++)
	{	
		for(i = 1; i < CIPHER_SOFT_CHAN_NUM; i++)
		{
		    task.stData2Process.bDecrypt = HI_TRUE;
		    task.stData2Process.u32src = g_stOutputBuf[i][j].u32StartPhyAddr;
		    task.stData2Process.u32dest = g_stResultBuf[i][j].u32StartPhyAddr;    
		    task.stData2Process.u32length = testSize;
		    task.u32CallBackArg = task.stData2Process.u32src;   
    
//	    	g_bCipherTestDone++;
		    CI_TEST_RUN(DRV_Cipher_CreatTask(g_u32CiTestChnID[i], &task, AlgCtrl[i].u32Key, AlgCtrl[i].u32IV), ret);
		}
	}
	ret = CIPHER_Test_WaitDone(200);
	if (HI_SUCCESS == ret)
	{
		for(i = 1; i < CIPHER_SOFT_CHAN_NUM; i++)
		{
			for(j = 0; j < CHAN_BUF_CNT; j++)
			{	
			    if (memcmp((void*)(g_stInputBuf[i][j].u32StartVirAddr), (void*)(g_stResultBuf[i][j].u32StartVirAddr), testSize))
			    {
			    	printk("Data NOT match! chan_id = %d, buf_cnt = %d\n", i, j);
			        ret = HI_FAILURE;
			        break;
			    }
			    else
			    {
			        ret = HI_SUCCESS;
			    }
			}
			if(ret == HI_FAILURE)
			{
				break;
			}				
		}
    }
    
    CI_TEST_CASE_END(ret);
    
    return HI_SUCCESS; 
}

HI_S32 CIPHER_Test_Case4(HI_VOID)
{
    HI_S32 ret;
    HI_U32 testSize = CIPHER_TEST_BUF_SIZE;
    HI_U32 i,j;

    HI_UNF_CIPHER_CTRL_S AlgCtrl[CIPHER_SOFT_CHAN_NUM];
    CIPHER_TASK_S task;
/*
    for(i = 1; i < CIPHER_SOFT_CHAN_NUM; i++)
   	{
   		for(j = 0; j < CHAN_BUF_CNT; j++)
   		{
		    memset((void*)(g_stInputBuf[i][j].u32StartVirAddr),  0, 16);    
		    memset((void*)(g_stOutputBuf[i][j].u32StartVirAddr), 1, 16);    
		    memset((void*)(g_stResultBuf[i][j].u32StartVirAddr), 2, 16);    
		}
	}
*/
    CIPHER_TestSetAttr(&AlgCtrl[1], HI_FALSE, HI_UNF_CIPHER_ALG_AES,
        HI_UNF_CIPHER_BIT_WIDTH_1BIT, HI_UNF_CIPHER_KEY_AES_128BIT, HI_UNF_CIPHER_WORK_MODE_CFB);
    CIPHER_TestSetIV(&AlgCtrl[1], 0x00112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[1], 0x11, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7);

    CIPHER_TestSetAttr(&AlgCtrl[2], HI_FALSE, HI_UNF_CIPHER_ALG_AES,
        HI_UNF_CIPHER_BIT_WIDTH_1BIT, HI_UNF_CIPHER_KEY_AES_128BIT, HI_UNF_CIPHER_WORK_MODE_OFB);
    CIPHER_TestSetIV(&AlgCtrl[2], 0x22112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[2], 0x22, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7);

    CIPHER_TestSetAttr(&AlgCtrl[3], HI_FALSE, HI_UNF_CIPHER_ALG_3DES,
        HI_UNF_CIPHER_BIT_WIDTH_64BIT, HI_UNF_CIPHER_KEY_DES_2KEY, HI_UNF_CIPHER_WORK_MODE_CBC);
    CIPHER_TestSetIV(&AlgCtrl[3], 0x33112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[3], 0x33, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7);

    CIPHER_TestSetAttr(&AlgCtrl[4], HI_FALSE, HI_UNF_CIPHER_ALG_3DES,
        HI_UNF_CIPHER_BIT_WIDTH_64BIT, HI_UNF_CIPHER_KEY_DES_3KEY, HI_UNF_CIPHER_WORK_MODE_CBC);
    CIPHER_TestSetIV(&AlgCtrl[4], 0x44112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[4], 0x44, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7);

    CIPHER_TestSetAttr(&AlgCtrl[5], HI_FALSE, HI_UNF_CIPHER_ALG_DES,
        HI_UNF_CIPHER_BIT_WIDTH_8BIT, HI_UNF_CIPHER_KEY_AES_128BIT, HI_UNF_CIPHER_WORK_MODE_CFB);
    CIPHER_TestSetIV(&AlgCtrl[5], 0x00112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[5], 0x55, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7);

    CIPHER_TestSetAttr(&AlgCtrl[6], HI_FALSE, HI_UNF_CIPHER_ALG_DES,
        HI_UNF_CIPHER_BIT_WIDTH_64BIT, HI_UNF_CIPHER_KEY_AES_128BIT, HI_UNF_CIPHER_WORK_MODE_CFB);
    CIPHER_TestSetIV(&AlgCtrl[6], 0x66112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[6], 0x66, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7);

    CIPHER_TestSetAttr(&AlgCtrl[7], HI_FALSE, HI_UNF_CIPHER_ALG_AES,
        HI_UNF_CIPHER_BIT_WIDTH_128BIT, HI_UNF_CIPHER_KEY_AES_192BIT, HI_UNF_CIPHER_WORK_MODE_CTR);
    CIPHER_TestSetIV(&AlgCtrl[7], 0x77112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[7], 0x77, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7);    

	for(i = 1; i < CIPHER_SOFT_CHAN_NUM; i++)
	{
		 CI_TEST_RUN(DRV_Cipher_ConfigChn(g_u32CiTestChnID[i], &AlgCtrl[i], CIPHER_Test_CallBack), ret);
	}

	g_bCipherTestDone = CHAN_BUF_CNT * (CIPHER_SOFT_CHAN_NUM - 1);
	for(j = 0; j < CHAN_BUF_CNT; j++)
	{
		for(i = 1; i < CIPHER_SOFT_CHAN_NUM; i++)
		{
		    task.stData2Process.bDecrypt = HI_FALSE;
		    task.stData2Process.u32src = g_stInputBuf[i][j].u32StartPhyAddr;
		    task.stData2Process.u32dest = g_stOutputBuf[i][j].u32StartPhyAddr;    
		    task.stData2Process.u32length = testSize;
		    task.u32CallBackArg = task.stData2Process.u32src;

//    		g_bCipherTestDone++;	
		    CI_TEST_RUN(DRV_Cipher_CreatTask(g_u32CiTestChnID[i], &task, AlgCtrl[i].u32Key, AlgCtrl[i].u32IV), ret);
		}
    }
	ret = CIPHER_Test_WaitDone(200);
	if (HI_SUCCESS == ret)
	{
		HI_INFO_CIPHER("wait 1 ok\n");
	}

	g_bCipherTestDone = CHAN_BUF_CNT * (CIPHER_SOFT_CHAN_NUM - 1);
	for(j = 0; j < CHAN_BUF_CNT; j++)
	{	
		for(i = 1; i < CIPHER_SOFT_CHAN_NUM; i++)
		{
		    task.stData2Process.bDecrypt = HI_TRUE;
		    task.stData2Process.u32src = g_stOutputBuf[i][j].u32StartPhyAddr;
		    task.stData2Process.u32dest = g_stResultBuf[i][j].u32StartPhyAddr;    
		    task.stData2Process.u32length = testSize;
		    task.u32CallBackArg = task.stData2Process.u32src;   
    
//	    	g_bCipherTestDone++;
		    CI_TEST_RUN(DRV_Cipher_CreatTask(g_u32CiTestChnID[i], &task, AlgCtrl[i].u32Key, AlgCtrl[i].u32IV), ret);
		}
	}
	ret = CIPHER_Test_WaitDone(200);
	if (HI_SUCCESS == ret)
	{
		for(i = 1; i < CIPHER_SOFT_CHAN_NUM; i++)
		{
			for(j = 0; j < CHAN_BUF_CNT; j++)
			{	
			    if (memcmp((void*)(g_stInputBuf[i][j].u32StartVirAddr), (void*)(g_stResultBuf[i][j].u32StartVirAddr), testSize))
			    {
			    	printk("Data NOT match! chan_id = %d, buf_cnt = %d\n", i, j);
			        ret = HI_FAILURE;
			        break;
			    }
			    else
			    {
			        ret = HI_SUCCESS;
			    }
			}
			if(ret == HI_FAILURE)
			{
				break;
			}				
		}
    }
    
    CI_TEST_CASE_END(ret);
    
    return HI_SUCCESS; 
}

HI_S32 CIPHER_Test_Case5(HI_VOID)
{
    HI_S32 ret;
    HI_U32 testSize = CIPHER_TEST_BUF_SIZE;
    HI_U32 i,j;

    HI_UNF_CIPHER_CTRL_S AlgCtrl[CIPHER_SOFT_CHAN_NUM];
    CIPHER_TASK_S task;
/*
    for(i = 1; i < CIPHER_SOFT_CHAN_NUM; i++)
   	{
   		for(j = 0; j < CHAN_BUF_CNT; j++)
   		{
		    memset((void*)(g_stInputBuf[i][j].u32StartVirAddr),  0, 16);    
		    memset((void*)(g_stOutputBuf[i][j].u32StartVirAddr), 1, 16);    
		    memset((void*)(g_stResultBuf[i][j].u32StartVirAddr), 2, 16);    
		}
	}
*/
    CIPHER_TestSetAttr(&AlgCtrl[1], HI_FALSE, HI_UNF_CIPHER_ALG_AES,
        HI_UNF_CIPHER_BIT_WIDTH_8BIT, HI_UNF_CIPHER_KEY_AES_128BIT, HI_UNF_CIPHER_WORK_MODE_CFB);
    CIPHER_TestSetIV(&AlgCtrl[1], 0x00112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[1], 0x11, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7);

    CIPHER_TestSetAttr(&AlgCtrl[2], HI_FALSE, HI_UNF_CIPHER_ALG_AES,
        HI_UNF_CIPHER_BIT_WIDTH_128BIT, HI_UNF_CIPHER_KEY_AES_128BIT, HI_UNF_CIPHER_WORK_MODE_OFB);
    CIPHER_TestSetIV(&AlgCtrl[2], 0x22112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[2], 0x22, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7);

    CIPHER_TestSetAttr(&AlgCtrl[3], HI_FALSE, HI_UNF_CIPHER_ALG_3DES,
        HI_UNF_CIPHER_BIT_WIDTH_64BIT, HI_UNF_CIPHER_KEY_DES_2KEY, HI_UNF_CIPHER_WORK_MODE_CFB);
    CIPHER_TestSetIV(&AlgCtrl[3], 0x33112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[3], 0x33, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7);

    CIPHER_TestSetAttr(&AlgCtrl[4], HI_FALSE, HI_UNF_CIPHER_ALG_3DES,
        HI_UNF_CIPHER_BIT_WIDTH_64BIT, HI_UNF_CIPHER_KEY_DES_3KEY, HI_UNF_CIPHER_WORK_MODE_OFB);
    CIPHER_TestSetIV(&AlgCtrl[4], 0x44112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[4], 0x44, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7);

    CIPHER_TestSetAttr(&AlgCtrl[5], HI_FALSE, HI_UNF_CIPHER_ALG_DES,
        HI_UNF_CIPHER_BIT_WIDTH_1BIT, HI_UNF_CIPHER_KEY_AES_128BIT, HI_UNF_CIPHER_WORK_MODE_CFB);
    CIPHER_TestSetIV(&AlgCtrl[5], 0x00112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[5], 0x55, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7);

    CIPHER_TestSetAttr(&AlgCtrl[6], HI_FALSE, HI_UNF_CIPHER_ALG_DES,
        HI_UNF_CIPHER_BIT_WIDTH_64BIT, HI_UNF_CIPHER_KEY_AES_128BIT, HI_UNF_CIPHER_WORK_MODE_OFB);
    CIPHER_TestSetIV(&AlgCtrl[6], 0x66112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[6], 0x66, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7);

    CIPHER_TestSetAttr(&AlgCtrl[7], HI_FALSE, HI_UNF_CIPHER_ALG_AES,
        HI_UNF_CIPHER_BIT_WIDTH_8BIT, HI_UNF_CIPHER_KEY_AES_192BIT, HI_UNF_CIPHER_WORK_MODE_CFB);
    CIPHER_TestSetIV(&AlgCtrl[7], 0x77112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[7], 0x77, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7);    

	for(i = 1; i < CIPHER_SOFT_CHAN_NUM; i++)
	{
		 CI_TEST_RUN(DRV_Cipher_ConfigChn(g_u32CiTestChnID[i], &AlgCtrl[i], CIPHER_Test_CallBack), ret);
	}

	g_bCipherTestDone = CHAN_BUF_CNT * (CIPHER_SOFT_CHAN_NUM - 1);
	for(j = 0; j < CHAN_BUF_CNT; j++)
	{
		for(i = 1; i < CIPHER_SOFT_CHAN_NUM; i++)
		{
		    task.stData2Process.bDecrypt = HI_FALSE;
		    task.stData2Process.u32src = g_stInputBuf[i][j].u32StartPhyAddr;
		    task.stData2Process.u32dest = g_stOutputBuf[i][j].u32StartPhyAddr;    
		    task.stData2Process.u32length = testSize;
		    task.u32CallBackArg = task.stData2Process.u32src;

//    		g_bCipherTestDone++;	
		    CI_TEST_RUN(DRV_Cipher_CreatTask(g_u32CiTestChnID[i], &task, AlgCtrl[i].u32Key, AlgCtrl[i].u32IV), ret);
		}
    }
	ret = CIPHER_Test_WaitDone(200);
	if (HI_SUCCESS == ret)
	{
		HI_INFO_CIPHER("wait 1 ok\n");
	}

	g_bCipherTestDone = CHAN_BUF_CNT * (CIPHER_SOFT_CHAN_NUM - 1);
	for(j = 0; j < CHAN_BUF_CNT; j++)
	{	
		for(i = 1; i < CIPHER_SOFT_CHAN_NUM; i++)
		{
		    task.stData2Process.bDecrypt = HI_TRUE;
		    task.stData2Process.u32src = g_stOutputBuf[i][j].u32StartPhyAddr;
		    task.stData2Process.u32dest = g_stResultBuf[i][j].u32StartPhyAddr;    
		    task.stData2Process.u32length = testSize;
		    task.u32CallBackArg = task.stData2Process.u32src;   
    
//	    	g_bCipherTestDone++;
		    CI_TEST_RUN(DRV_Cipher_CreatTask(g_u32CiTestChnID[i], &task, AlgCtrl[i].u32Key, AlgCtrl[i].u32IV), ret);
		}
	}
	ret = CIPHER_Test_WaitDone(200);
	if (HI_SUCCESS == ret)
	{
		for(i = 1; i < CIPHER_SOFT_CHAN_NUM; i++)
		{
			for(j = 0; j < CHAN_BUF_CNT; j++)
			{	
			    if (memcmp((void*)(g_stInputBuf[i][j].u32StartVirAddr), (void*)(g_stResultBuf[i][j].u32StartVirAddr), testSize))
			    {
			    	printk("Data NOT match! chan_id = %d, buf_cnt = %d\n", i, j);
			        ret = HI_FAILURE;
			        break;
			    }
			    else
			    {
			        ret = HI_SUCCESS;
			    }
			}
			if(ret == HI_FAILURE)
			{
				break;
			}			
		}
    }
    
    CI_TEST_CASE_END(ret);
    
    return HI_SUCCESS; 
}

HI_S32 CIPHER_Test_Case6(HI_VOID)
{
    HI_S32 ret;
    HI_U32 testSize = CIPHER_TEST_BUF_SIZE;
    HI_U32 i,j;

    HI_UNF_CIPHER_CTRL_S AlgCtrl[CIPHER_SOFT_CHAN_NUM];
    CIPHER_TASK_S task;
/*
    for(i = 1; i < CIPHER_SOFT_CHAN_NUM; i++)
   	{
   		for(j = 0; j < CHAN_BUF_CNT; j++)
   		{
		    memset((void*)(g_stInputBuf[i][j].u32StartVirAddr),  0, 16);    
		    memset((void*)(g_stOutputBuf[i][j].u32StartVirAddr), 1, 16);    
		    memset((void*)(g_stResultBuf[i][j].u32StartVirAddr), 2, 16);    
		}
	}
*/
    CIPHER_TestSetAttr(&AlgCtrl[1], HI_FALSE, HI_UNF_CIPHER_ALG_AES,
        HI_UNF_CIPHER_BIT_WIDTH_128BIT, HI_UNF_CIPHER_KEY_AES_128BIT, HI_UNF_CIPHER_WORK_MODE_CFB);
    CIPHER_TestSetIV(&AlgCtrl[1], 0x00112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[1], 0x11, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7);

    CIPHER_TestSetAttr(&AlgCtrl[2], HI_FALSE, HI_UNF_CIPHER_ALG_AES,
        HI_UNF_CIPHER_BIT_WIDTH_8BIT, HI_UNF_CIPHER_KEY_AES_128BIT, HI_UNF_CIPHER_WORK_MODE_OFB);
    CIPHER_TestSetIV(&AlgCtrl[2], 0x22112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[2], 0x22, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7);

    CIPHER_TestSetAttr(&AlgCtrl[3], HI_FALSE, HI_UNF_CIPHER_ALG_3DES,
        HI_UNF_CIPHER_BIT_WIDTH_8BIT, HI_UNF_CIPHER_KEY_DES_2KEY, HI_UNF_CIPHER_WORK_MODE_CFB);
    CIPHER_TestSetIV(&AlgCtrl[3], 0x33112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[3], 0x33, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7);

    CIPHER_TestSetAttr(&AlgCtrl[4], HI_FALSE, HI_UNF_CIPHER_ALG_3DES,
        HI_UNF_CIPHER_BIT_WIDTH_8BIT, HI_UNF_CIPHER_KEY_DES_3KEY, HI_UNF_CIPHER_WORK_MODE_OFB);
    CIPHER_TestSetIV(&AlgCtrl[4], 0x44112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[4], 0x44, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7);

    CIPHER_TestSetAttr(&AlgCtrl[5], HI_FALSE, HI_UNF_CIPHER_ALG_DES,
        HI_UNF_CIPHER_BIT_WIDTH_1BIT, HI_UNF_CIPHER_KEY_AES_128BIT, HI_UNF_CIPHER_WORK_MODE_OFB);
    CIPHER_TestSetIV(&AlgCtrl[5], 0x00112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[5], 0x55, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7);

    CIPHER_TestSetAttr(&AlgCtrl[6], HI_FALSE, HI_UNF_CIPHER_ALG_DES,
        HI_UNF_CIPHER_BIT_WIDTH_8BIT, HI_UNF_CIPHER_KEY_AES_128BIT, HI_UNF_CIPHER_WORK_MODE_OFB);
    CIPHER_TestSetIV(&AlgCtrl[6], 0x66112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[6], 0x66, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7);

    CIPHER_TestSetAttr(&AlgCtrl[7], HI_FALSE, HI_UNF_CIPHER_ALG_AES,
        HI_UNF_CIPHER_BIT_WIDTH_128BIT, HI_UNF_CIPHER_KEY_AES_256BIT, HI_UNF_CIPHER_WORK_MODE_CFB);
    CIPHER_TestSetIV(&AlgCtrl[7], 0x77112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[7], 0x77, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7);    

	for(i = 1; i < CIPHER_SOFT_CHAN_NUM; i++)
	{
		 CI_TEST_RUN(DRV_Cipher_ConfigChn(g_u32CiTestChnID[i], &AlgCtrl[i], CIPHER_Test_CallBack), ret);
	}
	g_bCipherTestDone = CHAN_BUF_CNT * (CIPHER_SOFT_CHAN_NUM - 1);
	for(j = 0; j < CHAN_BUF_CNT; j++)
	{
		for(i = 1; i < CIPHER_SOFT_CHAN_NUM; i++)
		{
		    task.stData2Process.bDecrypt = HI_FALSE;
		    task.stData2Process.u32src = g_stInputBuf[i][j].u32StartPhyAddr;
		    task.stData2Process.u32dest = g_stOutputBuf[i][j].u32StartPhyAddr;    
		    task.stData2Process.u32length = testSize;
		    task.u32CallBackArg = task.stData2Process.u32src;

//    		g_bCipherTestDone++;	
		    CI_TEST_RUN(DRV_Cipher_CreatTask(g_u32CiTestChnID[i], &task, AlgCtrl[i].u32Key, AlgCtrl[i].u32IV), ret);
		}
    }
	ret = CIPHER_Test_WaitDone(200);
    //ret = CIPHER_Test_WaitDone(500);
	if (HI_SUCCESS == ret)
	{
		HI_INFO_CIPHER("wait 1 ok\n");
	}
	g_bCipherTestDone = CHAN_BUF_CNT * (CIPHER_SOFT_CHAN_NUM - 1);
	for(j = 0; j < CHAN_BUF_CNT; j++)
	{	
		for(i = 1; i < CIPHER_SOFT_CHAN_NUM; i++)
		{
		    task.stData2Process.bDecrypt = HI_TRUE;
		    task.stData2Process.u32src = g_stOutputBuf[i][j].u32StartPhyAddr;
		    task.stData2Process.u32dest = g_stResultBuf[i][j].u32StartPhyAddr;    
		    task.stData2Process.u32length = testSize;
		    task.u32CallBackArg = task.stData2Process.u32src;   
    
//	    	g_bCipherTestDone++;
		    CI_TEST_RUN(DRV_Cipher_CreatTask(g_u32CiTestChnID[i], &task, AlgCtrl[i].u32Key, AlgCtrl[i].u32IV), ret);
		}
	}
	ret = CIPHER_Test_WaitDone(200);
    //ret = CIPHER_Test_WaitDone(500);
	if (HI_SUCCESS == ret)
	{
		for(i = 1; i < CIPHER_SOFT_CHAN_NUM; i++)
		{
			for(j = 0; j < CHAN_BUF_CNT; j++)
			{	
			    if (memcmp((void*)(g_stInputBuf[i][j].u32StartVirAddr), (void*)(g_stResultBuf[i][j].u32StartVirAddr), testSize))
			    {
			    	printk("Data NOT match! chan_id = %d, buf_cnt = %d\n", i, j);
			        ret = HI_FAILURE;
			        break;
			    }
			    else
			    {
			        ret = HI_SUCCESS;
			    }
			}
			if(ret == HI_FAILURE)
			{
				break;
			}			
		}
    }
    
    CI_TEST_CASE_END(ret);
    
    return HI_SUCCESS; 
}

HI_S32 CIPHER_Test_Case7(HI_VOID)
{
    HI_S32 ret;
    HI_U32 testSize = CIPHER_TEST_BUF_SIZE;
    HI_U32 i,j;

    HI_UNF_CIPHER_CTRL_S AlgCtrl[CIPHER_SOFT_CHAN_NUM];
    CIPHER_TASK_S task;
/*
    for(i = 1; i < CIPHER_SOFT_CHAN_NUM; i++)
   	{
   		for(j = 0; j < CHAN_BUF_CNT; j++)
   		{
		    memset((void*)(g_stInputBuf[i][j].u32StartVirAddr),  0, 16);    
		    memset((void*)(g_stOutputBuf[i][j].u32StartVirAddr), 1, 16);    
		    memset((void*)(g_stResultBuf[i][j].u32StartVirAddr), 2, 16);    
		}
	}
*/
    CIPHER_TestSetAttr(&AlgCtrl[1], HI_FALSE, HI_UNF_CIPHER_ALG_AES,
        HI_UNF_CIPHER_BIT_WIDTH_128BIT, HI_UNF_CIPHER_KEY_AES_192BIT, HI_UNF_CIPHER_WORK_MODE_ECB);
    CIPHER_TestSetIV(&AlgCtrl[1], 0x00112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[1], 0x11, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7);

    CIPHER_TestSetAttr(&AlgCtrl[2], HI_FALSE, HI_UNF_CIPHER_ALG_AES,
        HI_UNF_CIPHER_BIT_WIDTH_128BIT, HI_UNF_CIPHER_KEY_AES_192BIT, HI_UNF_CIPHER_WORK_MODE_CBC);
    CIPHER_TestSetIV(&AlgCtrl[2], 0x22112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[2], 0x22, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7);

    CIPHER_TestSetAttr(&AlgCtrl[3], HI_FALSE, HI_UNF_CIPHER_ALG_AES,
        HI_UNF_CIPHER_BIT_WIDTH_128BIT, HI_UNF_CIPHER_KEY_AES_256BIT, HI_UNF_CIPHER_WORK_MODE_ECB);
    CIPHER_TestSetIV(&AlgCtrl[3], 0x33112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[3], 0x33, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7);

    CIPHER_TestSetAttr(&AlgCtrl[4], HI_FALSE, HI_UNF_CIPHER_ALG_AES,
        HI_UNF_CIPHER_BIT_WIDTH_128BIT, HI_UNF_CIPHER_KEY_AES_256BIT, HI_UNF_CIPHER_WORK_MODE_CBC);
    CIPHER_TestSetIV(&AlgCtrl[4], 0x44112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[4], 0x44, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7);

    CIPHER_TestSetAttr(&AlgCtrl[5], HI_FALSE, HI_UNF_CIPHER_ALG_3DES,
        HI_UNF_CIPHER_BIT_WIDTH_1BIT, HI_UNF_CIPHER_KEY_DES_2KEY, HI_UNF_CIPHER_WORK_MODE_CFB);
    CIPHER_TestSetIV(&AlgCtrl[5], 0x00112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[5], 0x55, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7);

    CIPHER_TestSetAttr(&AlgCtrl[6], HI_FALSE, HI_UNF_CIPHER_ALG_3DES,
        HI_UNF_CIPHER_BIT_WIDTH_1BIT, HI_UNF_CIPHER_KEY_DES_3KEY, HI_UNF_CIPHER_WORK_MODE_OFB);
    CIPHER_TestSetIV(&AlgCtrl[6], 0x66112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[6], 0x66, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7);

    CIPHER_TestSetAttr(&AlgCtrl[7], HI_FALSE, HI_UNF_CIPHER_ALG_AES,
        HI_UNF_CIPHER_BIT_WIDTH_128BIT, HI_UNF_CIPHER_KEY_AES_192BIT, HI_UNF_CIPHER_WORK_MODE_OFB);
    CIPHER_TestSetIV(&AlgCtrl[7], 0x77112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[7], 0x77, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7);    

	for(i = 1; i < CIPHER_SOFT_CHAN_NUM; i++)
	{
		 CI_TEST_RUN(DRV_Cipher_ConfigChn(g_u32CiTestChnID[i], &AlgCtrl[i], CIPHER_Test_CallBack), ret);
	}
	g_bCipherTestDone = CHAN_BUF_CNT * (CIPHER_SOFT_CHAN_NUM - 1);
	for(j = 0; j < CHAN_BUF_CNT; j++)
	{
		for(i = 1; i < CIPHER_SOFT_CHAN_NUM; i++)
		{
		    task.stData2Process.bDecrypt = HI_FALSE;
		    task.stData2Process.u32src = g_stInputBuf[i][j].u32StartPhyAddr;
		    task.stData2Process.u32dest = g_stOutputBuf[i][j].u32StartPhyAddr;    
		    task.stData2Process.u32length = testSize;
		    task.u32CallBackArg = task.stData2Process.u32src;

//    		g_bCipherTestDone++;	
		    CI_TEST_RUN(DRV_Cipher_CreatTask(g_u32CiTestChnID[i], &task, AlgCtrl[i].u32Key, AlgCtrl[i].u32IV), ret);
		}
    }
    
    ret = CIPHER_Test_WaitDone(300);
	//ret = CIPHER_Test_WaitDone(800);
	if (HI_SUCCESS == ret)
	{
		HI_INFO_CIPHER("wait 1 ok\n");
	}
	g_bCipherTestDone = CHAN_BUF_CNT * (CIPHER_SOFT_CHAN_NUM - 1);
	for(j = 0; j < CHAN_BUF_CNT; j++)
	{	
		for(i = 1; i < CIPHER_SOFT_CHAN_NUM; i++)
		{
		    task.stData2Process.bDecrypt = HI_TRUE;
		    task.stData2Process.u32src = g_stOutputBuf[i][j].u32StartPhyAddr;
		    task.stData2Process.u32dest = g_stResultBuf[i][j].u32StartPhyAddr;    
		    task.stData2Process.u32length = testSize;
		    task.u32CallBackArg = task.stData2Process.u32src;   
    
//	    	g_bCipherTestDone++;
		    CI_TEST_RUN(DRV_Cipher_CreatTask(g_u32CiTestChnID[i], &task, AlgCtrl[i].u32Key, AlgCtrl[i].u32IV), ret);
		}
	}
    ret = CIPHER_Test_WaitDone(300);
    //ret = CIPHER_Test_WaitDone(800);
	if (HI_SUCCESS == ret)
	{
		for(i = 1; i < CIPHER_SOFT_CHAN_NUM; i++)
		{
			for(j = 0; j < CHAN_BUF_CNT; j++)
			{	
			    if (memcmp((void*)(g_stInputBuf[i][j].u32StartVirAddr), (void*)(g_stResultBuf[i][j].u32StartVirAddr), testSize))
			    {
			    	printk("Data NOT match! chan_id = %d, buf_cnt = %d\n", i, j);
			        ret = HI_FAILURE;
			        break;
			    }
			    else
			    {
			        ret = HI_SUCCESS;
			    }
			}
			if(ret == HI_FAILURE)
			{
				break;
			}
		}
    }
    
    CI_TEST_CASE_END(ret);
    
    return HI_SUCCESS; 
}

HI_S32 CIPHER_Test_Case8(HI_VOID)
{
    HI_S32 ret;
    HI_U32 testSize = CIPHER_TEST_BUF_SIZE;
    HI_U32 i,j;
    HI_UNF_CIPHER_CTRL_S AlgCtrl[CIPHER_SOFT_CHAN_NUM];
    CIPHER_TASK_S task;
    
	HI_S32 regAddr;
	HI_S32 regData;
	HI_U32 r2r_st[2][4] = {{0x0a46c3a4,0x031ff5de,0x00000059,0x0a46b078},
						   {0x0a130acc,0xf71ff5dc,0x09fef234,0x03973bd2}
						  };
	HI_U32 r2r_gd[2][4] = {{0xc7ece7f6,0x79fb58e7,0xb7c541b9,0xfcf371db},
						   {0x90b4a0fb,0x60c4b060,0xcec33c25,0x3b539895}
						  };
	HI_U32 r2r_key_chn_st[8][4] = {{0x096c1474,0xf71ff5dc,0x09fef274,0x03973b36},
	 							   {0x0a46cd3c,0x0375c6b9,0x00000051,0x0975cdc8},
	 							   {0x0a465d24,0xf71ff5dc,0x09fef334,0x03000036},
	 							   {0x0975b80c,0x03665f87,0x00000051,0x0975d178},
	 							   {0x0975c528,0x03f1c643,0x00000051,0x0975f550},
	 							   {0x0975a9e4,0x03665fef,0x00000051,0x09760370},
	 							   {0x09760e7c,0x0300002e,0x00000051,0x0975e968},
	 							   {0x00000008,0xf738b295,0x00000009,0x00000058}
	 							  };
	HI_U32 r2r_key_chn_gd[8][4] = {{0x0a45f388,0xbcc2df39,0xba99b2e5,0x9165269e},
	 							   {0xfc344522,0x7ab1b593,0x1f8729c6,0x45a1639d},
	 							   {0xce2473f6,0xe83a333f,0x8046f696,0x24be1b58},
	 							   {0x85cd6ad8,0xc908e832,0xeb100bef,0x417aca70},
	 							   {0xeb497655,0xb98514d1,0x7d926f50,0x4bc0739d},
	 							   {0x2110ef89,0xf0c9bbf3,0x8a5ba8cd,0x472a5f80},
	 							   {0xeef92f04,0x00d99bb4,0x0cbbe0d4,0x53250ea6},
	 							   {0xeaf45900,0x99c2c957,0xfacf2c4f,0xea227057}
	 							  };
	HI_U32 r2r_rd[4]  = {0,0,0,0};

	//set ram2ram key
	regData = 0xc; //ram2ram 3 KLE
	regAddr = REG_KL_CA_SET;  
    KL_WRITE_REG(regAddr, regData);

// ---------------------test ram2ram klad0------------------------------
	for(j=0; j<2; j++)
	{
		//set din
		for(i=0; i<4; i++)
		{
			regData = r2r_st[j][i];
			regAddr = REG_KL_CA_DIN(i); 
			KL_WRITE_REG(regAddr, regData);
		}

		regData = 0xa9 + ((j<<1)&0x6);
		regAddr = REG_KL_CA_CTRL; 
		KL_WRITE_REG(regAddr, regData);

		//wait interrupt
	    regAddr = REG_KL_CA_INT_RAW;  
	    KL_READ_REG(regAddr, regData);
	    while((regData & 0x1) == 0x0)
	    {
	    	KL_READ_REG(regAddr, regData);
	    }
	    //clear interrupt
	    regData = 0x1;
	    KL_WRITE_REG(regAddr, regData);

		for(i=0; i<4; i++)
		{
			regAddr = REG_KL_KEY_DOUT(i);
			KL_READ_REG(regAddr, regData);
			r2r_rd[i] = regData;
		}

		if (memcmp(r2r_rd, r2r_gd[j], 4))
	    {
	    	printk("R2R KL0 or KL1 Data NOT match!\n");
	        ret = HI_FAILURE;
	        return ret;
	    }
	    else
	    {
	    }
	}

// ---------------------set cipher klad key------------------------------
	for(j=0; j<8; j++)
	{
		for(i=0; i<4; i++)
		{
			regData = r2r_key_chn_st[j][i];
			regAddr = REG_KL_CA_DIN(i); 
			KL_WRITE_REG(regAddr, regData);
		}

		regData = 0xad + ((j << 8) & 0x1f00);
		regAddr = REG_KL_CA_CTRL; 
		KL_WRITE_REG(regAddr, regData);

		//wait interrupt
	    regAddr = REG_KL_CA_INT_RAW;  
	    KL_READ_REG(regAddr, regData);
	    while((regData & 0x1) == 0x0)
	    {
	    	KL_READ_REG(regAddr, regData);
	    }
	    //clear interrupt
	    regData = 0x1;
	    KL_WRITE_REG(regAddr, regData);

		for(i=0; i<4; i++)
		{
			regAddr = REG_KL_KEY_DOUT(i);
			KL_READ_REG(regAddr, regData);
			r2r_rd[i] = regData;
		}

		if (memcmp(r2r_rd, r2r_key_chn_gd[j], 4))
	    {
	    	printk("R2R KEY Data NOT match! j=%d\n",j);
	        ret = HI_FAILURE;
	        return ret;
	    }
	    else
	    {
	    }
	}
	
    CIPHER_TestSetAttr(&AlgCtrl[1], HI_FALSE, HI_UNF_CIPHER_ALG_AES,
        HI_UNF_CIPHER_BIT_WIDTH_128BIT, HI_UNF_CIPHER_KEY_AES_128BIT, HI_UNF_CIPHER_WORK_MODE_CTR);
    CIPHER_TestSetIV(&AlgCtrl[1], 0x00112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[1], r2r_key_chn_gd[1][0], r2r_key_chn_gd[1][1], r2r_key_chn_gd[1][2], r2r_key_chn_gd[1][3], 0x4, 0x5, 0x6, 0x7);

    CIPHER_TestSetAttr(&AlgCtrl[2], HI_FALSE, HI_UNF_CIPHER_ALG_3DES,
        HI_UNF_CIPHER_BIT_WIDTH_128BIT, HI_UNF_CIPHER_KEY_DES_2KEY, HI_UNF_CIPHER_WORK_MODE_CBC);
    CIPHER_TestSetIV(&AlgCtrl[2], 0x22112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[2], r2r_key_chn_gd[2][0], r2r_key_chn_gd[2][1], r2r_key_chn_gd[2][2], r2r_key_chn_gd[2][3],0x4, 0x5, 0x6, 0x7);

    CIPHER_TestSetAttr(&AlgCtrl[3], HI_FALSE, HI_UNF_CIPHER_ALG_AES,
        HI_UNF_CIPHER_BIT_WIDTH_128BIT, HI_UNF_CIPHER_KEY_AES_128BIT, HI_UNF_CIPHER_WORK_MODE_CBC);
    CIPHER_TestSetIV(&AlgCtrl[3], 0x33112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[3], r2r_key_chn_gd[3][0], r2r_key_chn_gd[3][1], r2r_key_chn_gd[3][2], r2r_key_chn_gd[3][3],0x4, 0x5, 0x6, 0x7);

    CIPHER_TestSetAttr(&AlgCtrl[4], HI_FALSE, HI_UNF_CIPHER_ALG_AES,
        HI_UNF_CIPHER_BIT_WIDTH_128BIT, HI_UNF_CIPHER_KEY_AES_128BIT, HI_UNF_CIPHER_WORK_MODE_OFB);
    CIPHER_TestSetIV(&AlgCtrl[4], 0x44112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[4], r2r_key_chn_gd[4][0], r2r_key_chn_gd[4][1], r2r_key_chn_gd[4][2], r2r_key_chn_gd[4][3],0x4, 0x5, 0x6, 0x7);

    CIPHER_TestSetAttr(&AlgCtrl[5], HI_FALSE, HI_UNF_CIPHER_ALG_3DES,
        HI_UNF_CIPHER_BIT_WIDTH_64BIT, HI_UNF_CIPHER_KEY_DES_2KEY, HI_UNF_CIPHER_WORK_MODE_CFB);
    CIPHER_TestSetIV(&AlgCtrl[5], 0x00112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[5], r2r_key_chn_gd[5][0], r2r_key_chn_gd[5][1], r2r_key_chn_gd[5][2], r2r_key_chn_gd[5][3],0x4, 0x5, 0x6, 0x7);

    CIPHER_TestSetAttr(&AlgCtrl[6], HI_FALSE, HI_UNF_CIPHER_ALG_3DES,
        HI_UNF_CIPHER_BIT_WIDTH_64BIT, HI_UNF_CIPHER_KEY_DES_2KEY, HI_UNF_CIPHER_WORK_MODE_OFB);
    CIPHER_TestSetIV(&AlgCtrl[6], 0x66112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[6], r2r_key_chn_gd[6][0], r2r_key_chn_gd[6][1], r2r_key_chn_gd[6][2], r2r_key_chn_gd[6][3],0x4, 0x5, 0x6, 0x7);

    CIPHER_TestSetAttr(&AlgCtrl[7], HI_FALSE, HI_UNF_CIPHER_ALG_AES,
        HI_UNF_CIPHER_BIT_WIDTH_128BIT, HI_UNF_CIPHER_KEY_AES_128BIT, HI_UNF_CIPHER_WORK_MODE_CFB);
    CIPHER_TestSetIV(&AlgCtrl[7], 0x77112233, 0x44556677, 0x8899aabb, 0xccddeeff);
    CIPHER_TestSetKey(&AlgCtrl[7], r2r_key_chn_gd[7][0], r2r_key_chn_gd[7][1], r2r_key_chn_gd[7][2], r2r_key_chn_gd[7][3],0x4, 0x5, 0x6, 0x7);    

	for(i = 1; i < CIPHER_SOFT_CHAN_NUM; i++)
	{
		 CI_TEST_RUN(DRV_Cipher_ConfigChn(g_u32CiTestChnID[i], &AlgCtrl[i], CIPHER_Test_CallBack), ret);
	}
	g_bCipherTestDone = CHAN_BUF_CNT * (CIPHER_SOFT_CHAN_NUM - 1);
	for(j = 0; j < CHAN_BUF_CNT; j++)
	{
		for(i = 1; i < CIPHER_SOFT_CHAN_NUM; i++)
		{
		    task.stData2Process.bDecrypt = HI_FALSE;
		    task.stData2Process.u32src = g_stInputBuf[i][j].u32StartPhyAddr;
		    task.stData2Process.u32dest = g_stOutputBuf[i][j].u32StartPhyAddr;    
		    task.stData2Process.u32length = testSize;
		    task.u32CallBackArg = task.stData2Process.u32src;

//    		g_bCipherTestDone++;	
		    CI_TEST_RUN(DRV_Cipher_CreatTask(g_u32CiTestChnID[i], &task, AlgCtrl[i].u32Key, AlgCtrl[i].u32IV), ret);
		}
    }
	ret = CIPHER_Test_WaitDone(300);
	if (HI_SUCCESS == ret)
	{
		HI_INFO_CIPHER("wait 1 ok\n");
	}

    CIPHER_TestSetAttr(&AlgCtrl[1], HI_TRUE, HI_UNF_CIPHER_ALG_AES,
        HI_UNF_CIPHER_BIT_WIDTH_128BIT, HI_UNF_CIPHER_KEY_AES_128BIT, HI_UNF_CIPHER_WORK_MODE_CTR);

    CIPHER_TestSetAttr(&AlgCtrl[2], HI_TRUE, HI_UNF_CIPHER_ALG_3DES,
        HI_UNF_CIPHER_BIT_WIDTH_128BIT, HI_UNF_CIPHER_KEY_DES_2KEY, HI_UNF_CIPHER_WORK_MODE_CBC);

    CIPHER_TestSetAttr(&AlgCtrl[3], HI_TRUE, HI_UNF_CIPHER_ALG_AES,
        HI_UNF_CIPHER_BIT_WIDTH_128BIT, HI_UNF_CIPHER_KEY_AES_128BIT, HI_UNF_CIPHER_WORK_MODE_CBC);

    CIPHER_TestSetAttr(&AlgCtrl[4], HI_TRUE, HI_UNF_CIPHER_ALG_AES,
        HI_UNF_CIPHER_BIT_WIDTH_128BIT, HI_UNF_CIPHER_KEY_AES_128BIT, HI_UNF_CIPHER_WORK_MODE_OFB);

    CIPHER_TestSetAttr(&AlgCtrl[5], HI_TRUE, HI_UNF_CIPHER_ALG_3DES,
        HI_UNF_CIPHER_BIT_WIDTH_64BIT, HI_UNF_CIPHER_KEY_DES_2KEY, HI_UNF_CIPHER_WORK_MODE_CFB);

    CIPHER_TestSetAttr(&AlgCtrl[6], HI_TRUE, HI_UNF_CIPHER_ALG_3DES,
        HI_UNF_CIPHER_BIT_WIDTH_64BIT, HI_UNF_CIPHER_KEY_DES_2KEY, HI_UNF_CIPHER_WORK_MODE_OFB);

    CIPHER_TestSetAttr(&AlgCtrl[7], HI_TRUE, HI_UNF_CIPHER_ALG_AES,
        HI_UNF_CIPHER_BIT_WIDTH_128BIT, HI_UNF_CIPHER_KEY_AES_128BIT, HI_UNF_CIPHER_WORK_MODE_CFB);

	for(i = 1; i < CIPHER_SOFT_CHAN_NUM; i++)
	{
		 CI_TEST_RUN(DRV_Cipher_ConfigChn(g_u32CiTestChnID[i], &AlgCtrl[i], CIPHER_Test_CallBack), ret);
	}

	g_bCipherTestDone = CHAN_BUF_CNT * (CIPHER_SOFT_CHAN_NUM - 1);
	for(j = 0; j < CHAN_BUF_CNT; j++)
	{	
		for(i = 1; i < CIPHER_SOFT_CHAN_NUM; i++)
		{
		    task.stData2Process.bDecrypt = HI_TRUE;
		    task.stData2Process.u32src = g_stOutputBuf[i][j].u32StartPhyAddr;
		    task.stData2Process.u32dest = g_stResultBuf[i][j].u32StartPhyAddr;    
		    task.stData2Process.u32length = testSize;
		    task.u32CallBackArg = task.stData2Process.u32src;   
    
//	    	g_bCipherTestDone++;
		    CI_TEST_RUN(DRV_Cipher_CreatTask(g_u32CiTestChnID[i], &task, AlgCtrl[i].u32Key, AlgCtrl[i].u32IV), ret);
		}
	}
	ret = CIPHER_Test_WaitDone(300);
	if (HI_SUCCESS == ret)
	{
		for(i = 1; i < CIPHER_SOFT_CHAN_NUM; i++)
		{
			for(j = 0; j < CHAN_BUF_CNT; j++)
			{	
			    if (memcmp((void*)(g_stInputBuf[i][j].u32StartVirAddr), (void*)(g_stResultBuf[i][j].u32StartVirAddr), testSize))
			    {
			    	printk("Data NOT match! chan_id = %d, buf_cnt = %d\n", i, j);
			        ret = HI_FAILURE;
			        break;
			    }
			    else
			    {
			        ret = HI_SUCCESS;
			    }
			}
			if(ret == HI_FAILURE)
			{
				break;
			}
		}
    }
    
    CI_TEST_CASE_END(ret);
    
    return HI_SUCCESS; 
}



static test_func g_pfnTestCases[] =
{
    CIPHER_Test_Case0,  /* 测试CA卡的，目前我们单板上没有这个条件测试 */
    CIPHER_Test_Case1, 
    CIPHER_Test_Case2,
    CIPHER_Test_Case3,
    CIPHER_Test_Case4,
    CIPHER_Test_Case5,
    CIPHER_Test_Case6,
    CIPHER_Test_Case7,
    CIPHER_Test_Case8,  /* 测试CA卡的，目前我们单板上没有这个条件测试 */
    NULL
};

HI_U32 g_u32CipherStartCase = 1;
HI_U32 g_u32CipherEndCase = 7;

HI_S32 CIPHER_Test_Run(HI_U32 beginNo, HI_U32 endNo)
{
    HI_S32 ret;
    HI_U32 i;
    
    CIPHER_Test_Init();


    for (i = beginNo; 
    	((i < sizeof(g_pfnTestCases)/sizeof(g_pfnTestCases[0]))
    	&& (i <= endNo)); i++)
    {
        printk("Runing CIPHER_Test_Case%d...\n", i);
        if (g_pfnTestCases[i] != NULL)
        {
            ret = g_pfnTestCases[i]();
            if (HI_SUCCESS == ret)
            {
                printk("CIPHER_Test_Case%d SUCCESS!\n\n", i);
            }
            else
            {
                printk("CIPHER_Test_Case%d ERROR, ret=%#x\n\n", i, ret);
            }
        }
    }
    
    CIPHER_Test_DeInit();

    return HI_SUCCESS;
}


