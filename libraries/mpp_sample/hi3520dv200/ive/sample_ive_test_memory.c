#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>


#include "hi_common.h"
#include "hi_comm_video.h"
#include "hi_comm_sys.h"
#include "hi_comm_ive.h"

#include "mpi_vb.h"
#include "mpi_sys.h"
#include "mpi_ive.h"

#define SAMPLE_IVE_DATA_WIDTH  448
#define SAMPLE_IVE_DATA_HEIGHT 256
#define SAMPLE_IVE_DATA_STRIDE 448
#define SAMPLE_IVE_HIST_STRIDE 1024


HI_S32   SAMPLE_IVE_INIT()
{
    HI_S32 sRet = HI_FAILURE;
    VB_CONF_S       stVbConf;
    MPP_SYS_CONF_S   stSysConf;
	HI_BOOL bMpiInit=HI_FALSE;
    if(HI_TRUE == bMpiInit)
    {
        printf("MPI has been inited \n ");
        return sRet;
    }
    /*初始化之前先确定系统已退出*/
    HI_MPI_SYS_Exit();
    HI_MPI_VB_Exit();

    /*VB初始化之前先配置VB*/
    memset(&stVbConf,0,sizeof(VB_CONF_S));

    /*1080p*/
    stVbConf.u32MaxPoolCnt = 64;
    stVbConf.astCommPool[0].u32BlkSize   = 1920*1088*2;
    stVbConf.astCommPool[0].u32BlkCnt    = 0;

     /*720p*/

    stVbConf.astCommPool[1].u32BlkSize   = 1280*720*2;
    stVbConf.astCommPool[1].u32BlkCnt    = 0;


     /*D1*/

    stVbConf.astCommPool[2].u32BlkSize   = 768*576*2;
    stVbConf.astCommPool[2].u32BlkCnt    = 0;


     /*Cif*/

    stVbConf.astCommPool[3].u32BlkSize   = 384*288*2;
    stVbConf.astCommPool[3].u32BlkCnt    = 0;


  #if 1
    stVbConf.astCommPool[4].u32BlkSize   = 176*144*2;
    stVbConf.astCommPool[4].u32BlkCnt    = 0;

   #endif
    sRet = HI_MPI_VB_SetConf(&stVbConf);
    if(HI_SUCCESS != sRet)
    {
        printf("Config VB fail!\n");
        return sRet;
    }


    sRet = HI_MPI_VB_Init();
    if(HI_SUCCESS != sRet)
    {
        printf("Init VB fail!\n");
        return sRet;
    }


    memset(&stSysConf,0,sizeof(MPP_SYS_CONF_S));
    stSysConf.u32AlignWidth = 16;

    sRet = HI_MPI_SYS_SetConf(&stSysConf);
    if(HI_SUCCESS != sRet)
    {
        printf("Config sys fail!\n");
        HI_MPI_VB_Exit();
        return sRet;
    }


    sRet = HI_MPI_SYS_Init();
     if(HI_SUCCESS != sRet)
    {
        printf("Init sys fail!\n");
        HI_MPI_VB_Exit();
        return sRet;
    }

    bMpiInit = HI_TRUE;

    return sRet;
}

void SAMPLE_IVE_HandleSig(HI_S32 signo)
{
    if (SIGINT == signo || SIGTSTP == signo)
    {
        printf("\033[0;31mprogram exit abnormally!\033[0;39m\n");
        HI_MPI_SYS_Exit();
	    HI_MPI_VB_Exit();
    }

    exit(0);
}


HI_S32 main(int argc, char *argv[])
{
    HI_S32 s32Ret = 0;
    HI_U32 u32stride, u32height, u32width;
    HI_U32 Srcdata = 0, Dstdata = 0,Subdata = 0,u32testmode = 0;
    
    IVE_HANDLE IveHandleDMA;
	IVE_HANDLE IveHandleSUB;
	IVE_HANDLE IveHandleHIST;
	IVE_HANDLE IveHandleCheck;
    
	IVE_SRC_INFO_S stSrc;
    IVE_MEM_INFO_S stDst;
	IVE_MEM_INFO_S stSub;
	IVE_MEM_INFO_S stHist;
    IVE_MEM_INFO_S stCheck;

    IVE_SRC_INFO_S stDma;
    
    HI_VOID *pVirtSrc;
    HI_VOID *pVirtDst;
	HI_VOID *pVirSub;
	HI_VOID *pVirHist;    
	volatile HI_U8 *pVirtCheck;
    
    IVE_SUB_OUT_FMT_E eOutFmt;;
    HI_BOOL bInstant,bFinished;    
	HI_S32 s32RunCircle;

	int histval[256];
    int i ,j=0;;		
	HI_U32 u32Counting=0;

    
	signal(SIGINT, SAMPLE_IVE_HandleSig);
    signal(SIGTERM, SAMPLE_IVE_HandleSig);
    SAMPLE_IVE_INIT();
        
	memset(histval, 0, sizeof(histval));
    eOutFmt = IVE_SUB_OUT_FMT_ABS;        

    u32stride = SAMPLE_IVE_DATA_STRIDE;
    u32height = SAMPLE_IVE_DATA_HEIGHT;
    u32width = SAMPLE_IVE_DATA_WIDTH;

    stSrc.stSrcMem.u32Stride  = u32stride;
    stSrc.u32Height           = u32height;
    stSrc.u32Width            = u32width;
    stSrc.enSrcFmt            = IVE_SRC_FMT_SINGLE;    
	bInstant = HI_FALSE;
    
	printf("in ivetest %d\n",argc);
	stCheck.u32Stride = SAMPLE_IVE_DATA_STRIDE;


    s32Ret = HI_MPI_SYS_MmzAlloc(&stSrc.stSrcMem.u32PhyAddr, &pVirtSrc,     //alloc source memory
        "User", HI_NULL, stSrc.u32Height * stSrc.stSrcMem.u32Stride);
    if (s32Ret != HI_SUCCESS)
    {
        printf("alloc src mem error\n");
        return s32Ret;
    }
    memset(pVirtSrc, 4, stSrc.u32Height * stSrc.stSrcMem.u32Stride);
    
    s32Ret = HI_MPI_SYS_MmzAlloc(&stHist.u32PhyAddr, &pVirHist,            //alloc memory for histogram
        "User", HI_NULL, SAMPLE_IVE_HIST_STRIDE);
    if (s32Ret != HI_SUCCESS)
    {
        printf("alloc hist mem error\n");
        goto fail0;
    }
    memset(pVirHist, 0x5a, SAMPLE_IVE_HIST_STRIDE);   
    
    s32Ret = HI_MPI_SYS_MmzAlloc(&stCheck.u32PhyAddr, (void**)&pVirtCheck,          //alloc memory for check  
    "User", HI_NULL, stSrc.u32Height * stSrc.stSrcMem.u32Stride);
    if (s32Ret != HI_SUCCESS)
    {
        printf("alloc check mem error\n");
        goto fail1;
    }   
    
	if(argc < 2)
	{
	    s32RunCircle=1;        		
        
        s32Ret = HI_MPI_SYS_MmzAlloc(&stDst.u32PhyAddr, &pVirtDst,              //alloc destination memory
            "User", HI_NULL, stSrc.u32Height * stSrc.stSrcMem.u32Stride);
        if (s32Ret != HI_SUCCESS)
        {
            printf("alloc dst mem error\n");
            goto fail2;
        }
        memset(pVirtDst,0x5a,stSrc.u32Height * stSrc.stSrcMem.u32Stride);
        
	    s32Ret = HI_MPI_SYS_MmzAlloc(&stSub.u32PhyAddr, &pVirSub,              //alloc memory for result of sub
	        "User", HI_NULL, stSrc.u32Height * stSrc.stSrcMem.u32Stride);
	    if (s32Ret != HI_SUCCESS)
	    {
	        printf("alloc sub mem error\n");
            HI_MPI_SYS_MmzFree(stDst.u32PhyAddr,pVirtDst);
			goto fail2;
	    }
	    memset(pVirSub,0x5a,stSrc.u32Height * stSrc.stSrcMem.u32Stride);	

	}
	else
	{ 		
		HI_U32 u32endaddr;
		FILE *fp;
        
		stDma.stSrcMem.u32PhyAddr = (HI_U32)(strtol(argv[1],NULL,16))+0x80000000;
		u32endaddr = strtol(argv[2],NULL,16) + 0x80000000;
		printf("ivetest [begin_addr] [end_addr] [filename] [stride] [height]\n");
		if(argc>4)
			u32testmode = strtol(argv[4],NULL,10);
		if(argc>5)
			u32height = strtol(argv[5],NULL,10);

        stDma.stSrcMem.u32Stride  = u32stride;
        stDma.u32Height           = u32height;
        stDma.u32Width            = u32width;
        stDma.enSrcFmt            = IVE_SRC_FMT_SINGLE;
    
		printf("begin_addr: %x end_addr:%x filename %s stride:%d  height:%d\n",
				stDma.stSrcMem.u32PhyAddr, u32endaddr, argv[3], u32stride, u32height);
	        
		s32RunCircle = (HI_U32)(u32endaddr - stDma.stSrcMem.u32PhyAddr)/(stDma.u32Height * stDma.stSrcMem.u32Stride);
		s32RunCircle -= 1;
	
		fp = fopen(argv[3], "rb");
		if(fp == NULL)
		{
			int testval = strtol(argv[3], NULL, 16);		
			printf("open file :%s error\n", argv[3]);
			printf("use val: %x instead file\n", testval);
			memset(pVirtSrc, testval, stSrc.u32Height * stSrc.stSrcMem.u32Stride);
		}
		else
		{
			fread(pVirtSrc, 1, stSrc.u32Height * stSrc.stSrcMem.u32Stride, fp);
			fclose(fp);
		}	

	}

    stDst.u32Stride = stSrc.stSrcMem.u32Stride;
	stSub.u32Stride = stSrc.stSrcMem.u32Stride;
    stCheck.u32Stride = stSrc.stSrcMem.u32Stride;
	stHist.u32Stride = SAMPLE_IVE_HIST_STRIDE;	

    while(1)
	{
		j++;
		if(argc >= 2)
		{		
            stDst.u32PhyAddr = stDma.stSrcMem.u32PhyAddr; //destination address of copy operation 
			stSub.u32PhyAddr = stDst.u32PhyAddr + stSrc.u32Height * stSrc.stSrcMem.u32Stride;//destionation address of sub operation with source data & copy data
		}
        
		for(i=0; i<s32RunCircle; i++)
		{
			IVE_SRC_INFO_S stSubInfo, stHistInfo;
                       
                                 
    		s32Ret = HI_MPI_IVE_DMA(&IveHandleDMA, &stSrc, &stDst, bInstant);	
            
			if(s32Ret)
			{
				printf("stSrc: %d %x %d %d %d\n",stSrc.enSrcFmt, stSrc.stSrcMem.u32PhyAddr,
								stSrc.stSrcMem.u32Stride, stSrc.u32Height, stSrc.u32Width);
				printf("stDst: %x %d\n",stDst.u32PhyAddr, stDst.u32Stride);
		        printf("DMA error %x\n",s32Ret);
                goto ive_end;	
			}
            
			memcpy(&stSubInfo, &stSrc, sizeof(IVE_SRC_INFO_S));
			stSubInfo.stSrcMem.u32PhyAddr = stDst.u32PhyAddr;
			s32Ret=HI_MPI_IVE_SUB(&IveHandleSUB, &stSrc, &stSubInfo, &stSub, eOutFmt, bInstant);			
			if(s32Ret)
			{
		        printf("SUB error %x\n",s32Ret);
				goto ive_end;			
			}	

            
			memcpy(&stHistInfo, &stSrc, sizeof(IVE_SRC_INFO_S));
			stHistInfo.stSrcMem.u32PhyAddr = stSub.u32PhyAddr;		
			s32Ret=HI_MPI_IVE_HIST(&IveHandleHIST, &stHistInfo, &stHist, HI_TRUE);
			if(s32Ret)
			{
				
				printf("HistSrc: %d %x %d %d %d\n",stHistInfo.enSrcFmt,stHistInfo.stSrcMem.u32PhyAddr,
								stHistInfo.stSrcMem.u32Stride,stHistInfo.u32Height,stHistInfo.u32Width);
				printf("Hist: %x %d\n",stHist.u32PhyAddr,stHist.u32Stride);			
		        printf("HIST error %x\n",s32Ret);		
				goto ive_end;			
			}
            
			s32Ret=HI_MPI_IVE_Query(IveHandleHIST, &bFinished, HI_TRUE);
			if(s32Ret)
			{
		        printf("QUERY error %x\n",s32Ret);
				goto ive_end;		
			}
            
			if(bFinished)
			{
				volatile HI_S32 *ps32Hist;
				ps32Hist = (volatile HI_S32 *)pVirHist;
				if(ps32Hist[0] != stSrc.u32Height * stSrc.u32Width)//if hist[0] is not equal to image size,then there is error happen when ive runing. 
				{
					int k;
					printf("error at %x %x val:%d\n", stSubInfo.stSrcMem.u32PhyAddr, stSub.u32PhyAddr, ps32Hist[0]);
                    if(1 == u32testmode)
                    {
                       goto ive_end;
                    }
					for(k=1;k<255;k++)
					{
						if(ps32Hist[k]!=0)                               //print out difference of error data and source data
                        {                  
                            printf("Hist error value:%d Hist error number:%d \n", k, ps32Hist[k]);
							histval[k] += ps32Hist[k];
                        }
					}
                    
					s32Ret=HI_MPI_IVE_DMA(&IveHandleCheck, &stHistInfo, &stCheck, HI_TRUE);
					if(s32Ret)
					{
						printf("check dma %x\n",s32Ret);
						goto ive_end;		
					}
                    
					s32Ret=HI_MPI_IVE_Query(IveHandleCheck,&bFinished,1);
					if(s32Ret && !bFinished)
					{
						printf("check query error %x %d\n",s32Ret,bFinished);
						goto ive_end;		
					}
                    
					u32Counting++;
                    
					for(k=0; k<stSrc.u32Height * stSrc.stSrcMem.u32Stride; k++)
					{						
							if(pVirtCheck[k]!=0)                        //print out error data and source data
                                {                     

                                s32Ret = HI_MPI_SYS_GetReg(stSrc.stSrcMem.u32PhyAddr + k, &Srcdata);

                                s32Ret = HI_MPI_SYS_GetReg(stDst.u32PhyAddr + k, &Dstdata);

                                s32Ret = HI_MPI_SYS_GetReg(stSub.u32PhyAddr + k, &Subdata);
                            
								printf("check error at %x %x %x %x %x %x\n",stSrc.stSrcMem.u32PhyAddr + k, stDst.u32PhyAddr + k,
															stSub.u32PhyAddr + k, Srcdata, Dstdata, Subdata);
                                }

					}
					sleep(10);
                    
				}
			}
			else
			{
				printf("no block\n");
				goto ive_end;	
			}
            
            if(argc>=2)
            {
                stDst.u32PhyAddr = stSub.u32PhyAddr;
                stSub.u32PhyAddr = stDst.u32PhyAddr + stSrc.u32Height * stSrc.stSrcMem.u32Stride;
            }
            
		}
        if((j&0xf)==0)
        {
            printf("ive pass one\n");
        }
        
        if((j&0xf)==0)
        {
            printf("ive pass one\n");
        }
        if(j/0xf == 5)
        {
            printf("----End----\n");
            break;
        }

	}

    ive_end:
        if(argc<2)
        {
            HI_MPI_SYS_MmzFree(stSub.u32PhyAddr, pVirSub);
            HI_MPI_SYS_MmzFree(stDst.u32PhyAddr, pVirtDst);
        }
        
    fail2:
        HI_MPI_SYS_MmzFree(stCheck.u32PhyAddr, (void*)pVirtCheck);
    fail1:
        HI_MPI_SYS_MmzFree(stHist.u32PhyAddr, pVirHist); 
    fail0:
        HI_MPI_SYS_MmzFree(stSrc.stSrcMem.u32PhyAddr, pVirtSrc);            
            
    return s32Ret;
}

