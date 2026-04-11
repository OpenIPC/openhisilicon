/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_isp_debug.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/08/30
  Description   : 
  History       :
  1.Date        : 2010/08/30
    Author      : x00100808
    Modification: Created file

******************************************************************************/

#ifndef __HI_ISP_DEBUG_H__
#define __HI_ISP_DEBUG_H__

#include <stdio.h>
#include "hi_debug.h"

#if 0
#define PRINT_DEBUG_INFO
#endif

#if 0
#define PRINT_INFO_2FILE
#endif

#define HI_ID_ISP	10

#define DEBUG_POS	printf("file:%s,line:%d\r\n",__FILE__,__LINE__);

#define ISP_TRACE(level, fmt...)\
    do{ \
            HI_TRACE(level,HI_ID_ISP,"[Func]:%s [Line]:%d [Info]:",__FUNCTION__, __LINE__);\
            HI_TRACE(level,HI_ID_ISP,##fmt);\
    }while(0)


#define HI_PRINT	printf

/* To avoid divide-0 exception in code. */
#define DIV_0_TO_1(a)   ( (0 == a) ? 1 : a )

#ifndef MAX
#define MAX(a, b) (((a) < (b)) ?  (b) : (a))
#endif

#ifndef MIN
#define MIN(a, b) (((a) > (b)) ?  (b) : (a))
#endif

#ifndef ABS
#define ABS(x) (((x) < 0) ? -(x) : (x))
#endif

#ifndef POW2
#define POW2(a) ((a) * (a))
#endif


/* WR_TXT_FILE
 * pStream: start address of write. 
 * size: how long to write, unit may be 8/16/32 according to sizeof(pStream).
 * file_name: must be string.
 * */
#define WR_TXT_FILE(pStream,size,file_name) \
    do\
    {\
        FILE* pFile;\
        pFile = fopen(file_name, "wt");\
        if (HI_NULL == pFile)\
        {\
            printf("open file err\n");\
        }\
        else\
        {\
            int i;\
            for(i = 0; i < size; i++)\
            {\
                fprintf(pFile, "%d\n", pStream[i]);\
            }\
            fclose(pFile);\
        }\
    }while(0)

#endif 	/* __HI_ISP_DEBUG_H__ */    		

