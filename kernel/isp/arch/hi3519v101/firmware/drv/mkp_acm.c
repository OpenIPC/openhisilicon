/******************************************************************************

  Copyright (C), 2001-2014, Huawei Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mkp_acm.c
  Version       : Initial Draft
  Author        : y00246723
  Created       : 2014/04/24
  Last Modified :
  Description   : acm alg
  Function List :
              
  History       :
  1.Date        : 2014/04/24
    Author      : y00246723
    Modification: Created file

******************************************************************************/

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

#include "hi_osal.h"

#include "hi_common.h"
#include "hi_comm_isp.h"
#include "mkp_isp.h"
#include "mm_ext.h"
#include "acm_ext.h"
#include "isp_acm_config.h"

//#include "isp_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define ACM_DBG_LVL HI_DGB_ERR

#define VouBitValue(a)    (a) 

//Default ACM Look Up Table for Y S H
extern HI_S16 s16AcmLUTDeflt[3][9][13][29] ;

//Temp ACM Look Up Table
static HI_S16 s16AcmLUT[ISP_MAX_DEV_NUM][3][9][13][29] = {{{{{0}}}}};

//Temp ACM Lut for logic format
static HI_S16 s_s16AcmLUTDeflt0_Luma[ISP_MAX_DEV_NUM][525]={{0}};
static HI_S16 s_s16AcmLUTDeflt0_Sat[ISP_MAX_DEV_NUM][525] ={{0}};
static HI_S16 s_s16AcmLUTDeflt0_Hue[ISP_MAX_DEV_NUM][525] ={{0}};

static HI_S16 s_s16AcmLUTDeflt1_Luma[ISP_MAX_DEV_NUM][420]={{0}};
static HI_S16 s_s16AcmLUTDeflt1_Sat[ISP_MAX_DEV_NUM][420] ={{0}};
static HI_S16 s_s16AcmLUTDeflt1_Hue[ISP_MAX_DEV_NUM][420] ={{0}};

static HI_S16 s_s16AcmLUTDeflt2_Luma[ISP_MAX_DEV_NUM][450]={{0}};
static HI_S16 s_s16AcmLUTDeflt2_Sat[ISP_MAX_DEV_NUM][450] ={{0}};
static HI_S16 s_s16AcmLUTDeflt2_Hue[ISP_MAX_DEV_NUM][450] ={{0}};

static HI_S16 s_s16AcmLUTDeflt3_Luma[ISP_MAX_DEV_NUM][360]={{0}};
static HI_S16 s_s16AcmLUTDeflt3_Sat[ISP_MAX_DEV_NUM][360] ={{0}};
static HI_S16 s_s16AcmLUTDeflt3_Hue[ISP_MAX_DEV_NUM][360] ={{0}};

static HI_S16 s_s16AcmLUTDeflt4_Luma[ISP_MAX_DEV_NUM][490]={{0}};
static HI_S16 s_s16AcmLUTDeflt4_Sat[ISP_MAX_DEV_NUM][490] ={{0}};
static HI_S16 s_s16AcmLUTDeflt4_Hue[ISP_MAX_DEV_NUM][490] ={{0}};

static HI_S16 s_s16AcmLUTDeflt5_Luma[ISP_MAX_DEV_NUM][392]={{0}};
static HI_S16 s_s16AcmLUTDeflt5_Sat[ISP_MAX_DEV_NUM][392] ={{0}};
static HI_S16 s_s16AcmLUTDeflt5_Hue[ISP_MAX_DEV_NUM][392] ={{0}};

static HI_S16 s_s16AcmLUTDeflt6_Luma[ISP_MAX_DEV_NUM][420]={{0}};
static HI_S16 s_s16AcmLUTDeflt6_Sat[ISP_MAX_DEV_NUM][420] ={{0}};
static HI_S16 s_s16AcmLUTDeflt6_Hue[ISP_MAX_DEV_NUM][420] ={{0}};
    
static HI_S16 s_s16AcmLUTDeflt7_Luma[ISP_MAX_DEV_NUM][336]={{0}};
static HI_S16 s_s16AcmLUTDeflt7_Sat[ISP_MAX_DEV_NUM][336] ={{0}};
static HI_S16 s_s16AcmLUTDeflt7_Hue[ISP_MAX_DEV_NUM][336] ={{0}};

static ACM_COEF_BITARRAY_S  stArray[ISP_MAX_DEV_NUM];

osal_spinlock_t g_stAcmLock[ISP_MAX_DEV_NUM];

/* */
static ISP_ACM_CTX_S               g_stAcmCtx[ISP_MAX_DEV_NUM];
static HI_BOOL                     g_bInited[ISP_MAX_DEV_NUM] = {HI_FALSE};

#define ACM_CHECK_NULL(p) \
{\
    if(HI_NULL == (p)) { return HI_ERR_ISP_NULL_PTR;}\
}\


HI_S32 ISP_ACM_DRV_SetCoeff(ISP_DEV IspDev);
HI_S32 ISP_ACM_DRV_GetAttr(ISP_DEV IspDev, ISP_ACM_ATTR_S *pstAcmAttr);
HI_S32 ISP_ACM_DRV_SetAttr(ISP_DEV IspDev, ISP_ACM_ATTR_S *pstAcmAttr);



#define ACM_CHECK_INIT(ISP_DEV) \
{\
    if(HI_TRUE != g_bInited[ISP_DEV]) \
    {\
        ISP_TRACE(HI_DBG_ERR, "Error acm not initialized\n");\
        return HI_DEF_ERR(HI_ID_ISP,EN_ERR_LEVEL_ERROR,EN_ERR_SYS_NOTREADY);\
    }\
}\

#if 0
static inline HI_VOID ACM_PrintCoeff(HI_VOID* pCoef)
{
    HI_S32 i;

    printf("addr:%x\n",pCoef);
    for(i = 0;i< 1024;i++)
    {
        if((i%16) == 0)
        {
            printf("\n");
        }    
        printf("%08x ",*(HI_U32*)pCoef++);
    }
    printf("\n");
}
#endif




/*****************************************************************************
 Prototype    : HI_U32 ACM_DRV_SplitArray
 Description  : 
 Input        :   
 Output       : none
 Return Value : static 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/04/29
    Author       : y00246723
    Modification : Created function

*****************************************************************************/
static HI_U32 ACM_DRV_SplitArray(ISP_DEV IspDev, const HI_S16 * pstAcmCoefArray)
{
    HI_S32 i, j, k;
    HI_S16 *pLUT0, *pLUT1, *pLUT2, *pLUT3, *pLUT4, *pLUT5, *pLUT6, *pLUT7;

    osal_memcpy(&s16AcmLUT[IspDev][0][0][0][0],pstAcmCoefArray,3*29*13*9*sizeof(HI_S16));
    
    pLUT0 = s_s16AcmLUTDeflt0_Luma[IspDev]; 
    pLUT1 = s_s16AcmLUTDeflt1_Luma[IspDev];
    pLUT2 = s_s16AcmLUTDeflt2_Luma[IspDev];
    pLUT3 = s_s16AcmLUTDeflt3_Luma[IspDev];
    pLUT4 = s_s16AcmLUTDeflt4_Luma[IspDev];
    pLUT5 = s_s16AcmLUTDeflt5_Luma[IspDev];
    pLUT6 = s_s16AcmLUTDeflt6_Luma[IspDev];
    pLUT7 = s_s16AcmLUTDeflt7_Luma[IspDev];

    for(k = 0; k < 29; k++)
    {
        for(j = 0; j < 13; j++)
        {
            for(i = 0; i < 9; i++)
            {
                switch((i&1) + ((j&1)<<1) + ((k&1)<<2))//Reorder
                {
                case 0:
                    *pLUT0 = s16AcmLUT[IspDev][0][i][j][k]; 
                    pLUT0++;
                    break;                        
                case 1:
                    *pLUT1 = s16AcmLUT[IspDev][0][i][j][k];
                    pLUT1++;
                    break;
                case 2:
                    *pLUT2 = s16AcmLUT[IspDev][0][i][j][k];
                    pLUT2++;
                    break;
                case 3:
                    *pLUT3 = s16AcmLUT[IspDev][0][i][j][k];
                    pLUT3++;
                    break;
                case 4:
                    *pLUT4 = s16AcmLUT[IspDev][0][i][j][k];
                    pLUT4++;
                    break;
                case 5:
                    *pLUT5 = s16AcmLUT[IspDev][0][i][j][k];
                    pLUT5++;
                    break;
                case 6:
                    *pLUT6 = s16AcmLUT[IspDev][0][i][j][k];
                    pLUT6++;
                    break;
                case 7:
                    *pLUT7 = s16AcmLUT[IspDev][0][i][j][k];
                    pLUT7++;
                    break;
                }
            }
        }
    }

    pLUT0 = s_s16AcmLUTDeflt0_Hue[IspDev];
    pLUT1 = s_s16AcmLUTDeflt1_Hue[IspDev];
    pLUT2 = s_s16AcmLUTDeflt2_Hue[IspDev];
    pLUT3 = s_s16AcmLUTDeflt3_Hue[IspDev];
    pLUT4 = s_s16AcmLUTDeflt4_Hue[IspDev];
    pLUT5 = s_s16AcmLUTDeflt5_Hue[IspDev];
    pLUT6 = s_s16AcmLUTDeflt6_Hue[IspDev];
    pLUT7 = s_s16AcmLUTDeflt7_Hue[IspDev];

    for(k = 0; k < 29; k++)
    {
        for(j = 0; j < 13; j++)
        {
            for(i = 0; i < 9; i++)
            {
                switch((i&1) + ((j&1)<<1) + ((k&1)<<2))
                {
                case 0:
                    *pLUT0 = s16AcmLUT[IspDev][1][i][j][k];
                    pLUT0++;
                    break;                        
                case 1:
                    *pLUT1 = s16AcmLUT[IspDev][1][i][j][k];
                    pLUT1++;
                    break;
                case 2:
                    *pLUT2 = s16AcmLUT[IspDev][1][i][j][k];
                    pLUT2++;
                    break;
                case 3:
                    *pLUT3 = s16AcmLUT[IspDev][1][i][j][k];
                    pLUT3++;
                    break;
                case 4:
                    *pLUT4 = s16AcmLUT[IspDev][1][i][j][k];
                    pLUT4++;
                    break;
                case 5:
                    *pLUT5 = s16AcmLUT[IspDev][1][i][j][k];
                    pLUT5++;
                    break;
                case 6:
                    *pLUT6 = s16AcmLUT[IspDev][1][i][j][k];
                    pLUT6++;
                    break;
                case 7:
                    *pLUT7 = s16AcmLUT[IspDev][1][i][j][k];
                    pLUT7++;
                    break;
                }
            }
        }
    }

    pLUT0 = s_s16AcmLUTDeflt0_Sat[IspDev];
    pLUT1 = s_s16AcmLUTDeflt1_Sat[IspDev];
    pLUT2 = s_s16AcmLUTDeflt2_Sat[IspDev];
    pLUT3 = s_s16AcmLUTDeflt3_Sat[IspDev];
    pLUT4 = s_s16AcmLUTDeflt4_Sat[IspDev];
    pLUT5 = s_s16AcmLUTDeflt5_Sat[IspDev];
    pLUT6 = s_s16AcmLUTDeflt6_Sat[IspDev];
    pLUT7 = s_s16AcmLUTDeflt7_Sat[IspDev];

    for(k = 0; k < 29; k++)
    {
        for(j = 0; j < 13; j++)
        {
            for(i = 0; i < 9; i++)
            {
                switch((i&1) + ((j&1)<<1) + ((k&1)<<2))
                {
                case 0:
                    *pLUT0 = s16AcmLUT[IspDev][2][i][j][k];
                    pLUT0++;
                    break;                        
                case 1:
                    *pLUT1 = s16AcmLUT[IspDev][2][i][j][k];
                    pLUT1++;
                    break;
                case 2:
                    *pLUT2 = s16AcmLUT[IspDev][2][i][j][k];
                    pLUT2++;
                    break;
                case 3:
                    *pLUT3 = s16AcmLUT[IspDev][2][i][j][k];
                    pLUT3++;
                    break;
                case 4:
                    *pLUT4 = s16AcmLUT[IspDev][2][i][j][k];
                    pLUT4++;
                    break;
                case 5:
                    *pLUT5 = s16AcmLUT[IspDev][2][i][j][k];
                    pLUT5++;
                    break;
                case 6:
                    *pLUT6 = s16AcmLUT[IspDev][2][i][j][k];
                    pLUT6++;
                    break;
                case 7:
                    *pLUT7 = s16AcmLUT[IspDev][2][i][j][k];
                    pLUT7++;
                    break;
                }
            }
        }
    }
    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype    : HI_U32 ACM_DRV_TransCoefAlign
 Description  : 
 Input        :   
 Output       : none
 Return Value : static 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/04/29
    Author       : y00246723
    Modification : Created function

*****************************************************************************/
static HI_U32 ACM_DRV_TransCoefAlign(ISP_DEV IspDev, const HI_S16 * pstAcmCoefArray, ACM_COEF_BITARRAY_S *pBitCoef)
{
    HI_S16 i, s16Tmp;
    ACM_PSCoef stACM_PSCoef;
    ACM_PSCoef *pACM_PSCoef = &stACM_PSCoef;
    
    ACM_DRV_SplitArray(IspDev, pstAcmCoefArray);

    pACM_PSCoef->ps16Luma = &s_s16AcmLUTDeflt0_Luma[IspDev][0];
    pACM_PSCoef->ps16Sat  = &s_s16AcmLUTDeflt0_Sat[IspDev][0];
    pACM_PSCoef->ps16Hue  = &s_s16AcmLUTDeflt0_Hue[IspDev][0];


    for (i = 0; i < 132; i++) // 525/4=131.25
    {
        if(i==131)  
        {
            pBitCoef->stBit[i].bits_0 = VouBitValue(*pACM_PSCoef->ps16Luma++); 
            pBitCoef->stBit[i].bits_1 = 0;    
            pBitCoef->stBit[i].bits_2 = VouBitValue(*pACM_PSCoef->ps16Sat++);

            s16Tmp = 0;
            pBitCoef->stBit[i].bits_35 = s16Tmp;
            pBitCoef->stBit[i].bits_34 = (s16Tmp >> 5); 
            pBitCoef->stBit[i].bits_4 = VouBitValue(*pACM_PSCoef->ps16Hue++);
            pBitCoef->stBit[i].bits_5 = 0;
			
            pBitCoef->stBit[i].bits_6 = 0; 

            s16Tmp = 0;
            pBitCoef->stBit[i].bits_75 = s16Tmp;
            pBitCoef->stBit[i].bits_74 = s16Tmp >> 5;

            pBitCoef->stBit[i].bits_8 = 0;
            pBitCoef->stBit[i].bits_9 = 0;
            pBitCoef->stBit[i].bits_10= 0;

            s16Tmp = 0;
            pBitCoef->stBit[i].bits_113 = s16Tmp;
            pBitCoef->stBit[i].bits_114 = s16Tmp >> 3;
        }
        else
        {
            pBitCoef->stBit[i].bits_0 = VouBitValue(*pACM_PSCoef->ps16Luma++);
            pBitCoef->stBit[i].bits_1 = VouBitValue(*pACM_PSCoef->ps16Luma++); //repeat luma   
            pBitCoef->stBit[i].bits_2 = VouBitValue(*pACM_PSCoef->ps16Sat++);
            
            s16Tmp = VouBitValue(*pACM_PSCoef->ps16Sat++); //repeat saturation
            pBitCoef->stBit[i].bits_35 = s16Tmp;
            pBitCoef->stBit[i].bits_34 = (s16Tmp >> 5); //handle it as bit move to the right 5 bit
            
            pBitCoef->stBit[i].bits_4 = VouBitValue(*pACM_PSCoef->ps16Hue++);
            pBitCoef->stBit[i].bits_5 = VouBitValue(*pACM_PSCoef->ps16Hue++);
            pBitCoef->stBit[i].bits_6 = VouBitValue(*pACM_PSCoef->ps16Luma++); //repeat
            
            s16Tmp = VouBitValue(*pACM_PSCoef->ps16Luma++);
            pBitCoef->stBit[i].bits_75 = s16Tmp;
            pBitCoef->stBit[i].bits_74 = s16Tmp >> 5;
            
            pBitCoef->stBit[i].bits_8 = VouBitValue(*pACM_PSCoef->ps16Sat++);
            pBitCoef->stBit[i].bits_9 = VouBitValue(*pACM_PSCoef->ps16Sat++);
            pBitCoef->stBit[i].bits_10= VouBitValue(*pACM_PSCoef->ps16Hue++);
            
            s16Tmp = VouBitValue(*pACM_PSCoef->ps16Hue++);
            pBitCoef->stBit[i].bits_113 = s16Tmp;
            pBitCoef->stBit[i].bits_114 = s16Tmp >> 3;
       }
        pBitCoef->stBit[i].bits_12 = 0; //apply last 28bit to 0
    }
    
    pACM_PSCoef->ps16Luma = &s_s16AcmLUTDeflt1_Luma[IspDev][0];
    pACM_PSCoef->ps16Sat  = &s_s16AcmLUTDeflt1_Sat[IspDev][0];
    pACM_PSCoef->ps16Hue  = &s_s16AcmLUTDeflt1_Hue[IspDev][0];

    for (i = 0; i < 105; i++)  // 420/4=105
    {
        pBitCoef->stBit[i+132].bits_0 = VouBitValue(*pACM_PSCoef->ps16Luma++); 
        pBitCoef->stBit[i+132].bits_1 = VouBitValue(*pACM_PSCoef->ps16Luma++);    
        pBitCoef->stBit[i+132].bits_2 = VouBitValue(*pACM_PSCoef->ps16Sat++);

        s16Tmp = VouBitValue(*pACM_PSCoef->ps16Sat++);
        pBitCoef->stBit[i+132].bits_35 = s16Tmp;
        pBitCoef->stBit[i+132].bits_34 = (s16Tmp >> 5); 

        pBitCoef->stBit[i+132].bits_4 = VouBitValue(*pACM_PSCoef->ps16Hue++);
        pBitCoef->stBit[i+132].bits_5 = VouBitValue(*pACM_PSCoef->ps16Hue++);
        pBitCoef->stBit[i+132].bits_6 = VouBitValue(*pACM_PSCoef->ps16Luma++); 

        s16Tmp = VouBitValue(*pACM_PSCoef->ps16Luma++);
        pBitCoef->stBit[i+132].bits_75 = s16Tmp;
        pBitCoef->stBit[i+132].bits_74 = s16Tmp >> 5;

        pBitCoef->stBit[i+132].bits_8 = VouBitValue(*pACM_PSCoef->ps16Sat++);
        pBitCoef->stBit[i+132].bits_9 = VouBitValue(*pACM_PSCoef->ps16Sat++);
        pBitCoef->stBit[i+132].bits_10= VouBitValue(*pACM_PSCoef->ps16Hue++);

        s16Tmp = VouBitValue(*pACM_PSCoef->ps16Hue++);
        pBitCoef->stBit[i+132].bits_113 = s16Tmp;
        pBitCoef->stBit[i+132].bits_114 = s16Tmp >> 3;

        pBitCoef->stBit[i+132].bits_12 = 0; 
    }
    
    pACM_PSCoef->ps16Luma = &s_s16AcmLUTDeflt2_Luma[IspDev][0];
    pACM_PSCoef->ps16Sat  = &s_s16AcmLUTDeflt2_Sat[IspDev][0];
    pACM_PSCoef->ps16Hue  = &s_s16AcmLUTDeflt2_Hue[IspDev][0];

    for (i = 0; i < 113; i++) // 450/4=112.5
    {
        if(i==112)  //0.5
        {
            pBitCoef->stBit[i+237].bits_0 = VouBitValue(*pACM_PSCoef->ps16Luma++);
            pBitCoef->stBit[i+237].bits_1 = VouBitValue(*pACM_PSCoef->ps16Luma++);    
            pBitCoef->stBit[i+237].bits_2 = VouBitValue(*pACM_PSCoef->ps16Sat++);

            s16Tmp = VouBitValue(*pACM_PSCoef->ps16Sat++);
            pBitCoef->stBit[i+237].bits_35 = s16Tmp;
            pBitCoef->stBit[i+237].bits_34 = (s16Tmp >> 5);
            pBitCoef->stBit[i+237].bits_4 = VouBitValue(*pACM_PSCoef->ps16Hue++);
            pBitCoef->stBit[i+237].bits_5 = VouBitValue(*pACM_PSCoef->ps16Hue++);
            
			
            pBitCoef->stBit[i+237].bits_6 = 0;

            s16Tmp = 0;
            pBitCoef->stBit[i+237].bits_75 = s16Tmp;
            pBitCoef->stBit[i+237].bits_74 = s16Tmp >> 5;

            pBitCoef->stBit[i+237].bits_8 = 0;
            pBitCoef->stBit[i+237].bits_9 = 0;
            pBitCoef->stBit[i+237].bits_10= 0;

            s16Tmp = 0;
            pBitCoef->stBit[i+237].bits_113 = s16Tmp;
            pBitCoef->stBit[i+237].bits_114 = s16Tmp >> 3;
            
        }
        else
        {
            pBitCoef->stBit[i+237].bits_0 = VouBitValue(*pACM_PSCoef->ps16Luma++); 
            pBitCoef->stBit[i+237].bits_1 = VouBitValue(*pACM_PSCoef->ps16Luma++);    
            pBitCoef->stBit[i+237].bits_2 = VouBitValue(*pACM_PSCoef->ps16Sat++);

            s16Tmp = VouBitValue(*pACM_PSCoef->ps16Sat++);
            pBitCoef->stBit[i+237].bits_35 = s16Tmp;
            pBitCoef->stBit[i+237].bits_34 = (s16Tmp >> 5); 

            pBitCoef->stBit[i+237].bits_4 = VouBitValue(*pACM_PSCoef->ps16Hue++);
            pBitCoef->stBit[i+237].bits_5 = VouBitValue(*pACM_PSCoef->ps16Hue++);
            pBitCoef->stBit[i+237].bits_6 = VouBitValue(*pACM_PSCoef->ps16Luma++); 

            s16Tmp = VouBitValue(*pACM_PSCoef->ps16Luma++);
            pBitCoef->stBit[i+237].bits_75 = s16Tmp;
            pBitCoef->stBit[i+237].bits_74 = s16Tmp >> 5;

            pBitCoef->stBit[i+237].bits_8 = VouBitValue(*pACM_PSCoef->ps16Sat++);
            pBitCoef->stBit[i+237].bits_9 = VouBitValue(*pACM_PSCoef->ps16Sat++);
            pBitCoef->stBit[i+237].bits_10= VouBitValue(*pACM_PSCoef->ps16Hue++);

            s16Tmp = VouBitValue(*pACM_PSCoef->ps16Hue++);
            pBitCoef->stBit[i+237].bits_113 = s16Tmp;
            pBitCoef->stBit[i+237].bits_114 = s16Tmp >> 3;
        
        }
        pBitCoef->stBit[i+237].bits_12 = 0; 
    }


    pACM_PSCoef->ps16Luma = &s_s16AcmLUTDeflt3_Luma[IspDev][0];
    pACM_PSCoef->ps16Sat = &s_s16AcmLUTDeflt3_Sat[IspDev][0];
    pACM_PSCoef->ps16Hue = &s_s16AcmLUTDeflt3_Hue[IspDev][0];

    for (i = 0; i < 90; i++)  //360/4=90
    {
            pBitCoef->stBit[i+350].bits_0 = VouBitValue(*pACM_PSCoef->ps16Luma++); 
            pBitCoef->stBit[i+350].bits_1 = VouBitValue(*pACM_PSCoef->ps16Luma++);    
            pBitCoef->stBit[i+350].bits_2 = VouBitValue(*pACM_PSCoef->ps16Sat++);

            s16Tmp = VouBitValue(*pACM_PSCoef->ps16Sat++);
            pBitCoef->stBit[i+350].bits_35 = s16Tmp;
            pBitCoef->stBit[i+350].bits_34 = (s16Tmp >> 5); 

            pBitCoef->stBit[i+350].bits_4 = VouBitValue(*pACM_PSCoef->ps16Hue++);
            pBitCoef->stBit[i+350].bits_5 = VouBitValue(*pACM_PSCoef->ps16Hue++);
            pBitCoef->stBit[i+350].bits_6 = VouBitValue(*pACM_PSCoef->ps16Luma++);

            s16Tmp = VouBitValue(*pACM_PSCoef->ps16Luma++);
            pBitCoef->stBit[i+350].bits_75 = s16Tmp;
            pBitCoef->stBit[i+350].bits_74 = s16Tmp >> 5;

            pBitCoef->stBit[i+350].bits_8 = VouBitValue(*pACM_PSCoef->ps16Sat++);
            pBitCoef->stBit[i+350].bits_9 = VouBitValue(*pACM_PSCoef->ps16Sat++);
            pBitCoef->stBit[i+350].bits_10= VouBitValue(*pACM_PSCoef->ps16Hue++);

            s16Tmp = VouBitValue(*pACM_PSCoef->ps16Hue++);
            pBitCoef->stBit[i+350].bits_113 = s16Tmp;
            pBitCoef->stBit[i+350].bits_114 = s16Tmp >> 3;
			
        pBitCoef->stBit[i+350].bits_12 = 0; 
    }

    pACM_PSCoef->ps16Luma = &s_s16AcmLUTDeflt4_Luma[IspDev][0];
    pACM_PSCoef->ps16Sat = &s_s16AcmLUTDeflt4_Sat[IspDev][0];
    pACM_PSCoef->ps16Hue = &s_s16AcmLUTDeflt4_Hue[IspDev][0];

    for (i = 0; i < 123; i++)   //490/4=122.5
    {
		if(i==122)  //0.5
        {
            pBitCoef->stBit[i+440].bits_0 = VouBitValue(*pACM_PSCoef->ps16Luma++);
            pBitCoef->stBit[i+440].bits_1 = VouBitValue(*pACM_PSCoef->ps16Luma++);    
            pBitCoef->stBit[i+440].bits_2 = VouBitValue(*pACM_PSCoef->ps16Sat++);

            s16Tmp = VouBitValue(*pACM_PSCoef->ps16Sat++);
            pBitCoef->stBit[i+440].bits_35 = s16Tmp;
            pBitCoef->stBit[i+440].bits_34 = (s16Tmp >> 5);
            pBitCoef->stBit[i+440].bits_4 = VouBitValue(*pACM_PSCoef->ps16Hue++);
            pBitCoef->stBit[i+440].bits_5 = VouBitValue(*pACM_PSCoef->ps16Hue++);
            
			
            pBitCoef->stBit[i+440].bits_6 = 0;

            s16Tmp = 0;
            pBitCoef->stBit[i+440].bits_75 = s16Tmp;
            pBitCoef->stBit[i+440].bits_74 = s16Tmp >> 5;

            pBitCoef->stBit[i+440].bits_8 = 0;
            pBitCoef->stBit[i+440].bits_9 = 0;
            pBitCoef->stBit[i+440].bits_10= 0;

            s16Tmp = 0;
            pBitCoef->stBit[i+440].bits_113 = s16Tmp;
            pBitCoef->stBit[i+440].bits_114 = s16Tmp >> 3;

        }
        else
        {
            pBitCoef->stBit[i+440].bits_0 = VouBitValue(*pACM_PSCoef->ps16Luma++); 
            pBitCoef->stBit[i+440].bits_1 = VouBitValue(*pACM_PSCoef->ps16Luma++);    
            pBitCoef->stBit[i+440].bits_2 = VouBitValue(*pACM_PSCoef->ps16Sat++);
            
            s16Tmp = VouBitValue(*pACM_PSCoef->ps16Sat++);
            pBitCoef->stBit[i+440].bits_35 = s16Tmp;
            pBitCoef->stBit[i+440].bits_34 = (s16Tmp >> 5); 
            
            pBitCoef->stBit[i+440].bits_4 = VouBitValue(*pACM_PSCoef->ps16Hue++);
            pBitCoef->stBit[i+440].bits_5 = VouBitValue(*pACM_PSCoef->ps16Hue++);
            pBitCoef->stBit[i+440].bits_6 = VouBitValue(*pACM_PSCoef->ps16Luma++); 
            
            s16Tmp = VouBitValue(*pACM_PSCoef->ps16Luma++);
            pBitCoef->stBit[i+440].bits_75 = s16Tmp;
            pBitCoef->stBit[i+440].bits_74 = s16Tmp >> 5;
            
            pBitCoef->stBit[i+440].bits_8 = VouBitValue(*pACM_PSCoef->ps16Sat++);
            pBitCoef->stBit[i+440].bits_9 = VouBitValue(*pACM_PSCoef->ps16Sat++);
            pBitCoef->stBit[i+440].bits_10= VouBitValue(*pACM_PSCoef->ps16Hue++);
            
            s16Tmp = VouBitValue(*pACM_PSCoef->ps16Hue++);
            pBitCoef->stBit[i+440].bits_113 = s16Tmp;
            pBitCoef->stBit[i+440].bits_114 = s16Tmp >> 3;
        }
        pBitCoef->stBit[i+440].bits_12 = 0; 
    }

    pACM_PSCoef->ps16Luma = &s_s16AcmLUTDeflt5_Luma[IspDev][0];
    pACM_PSCoef->ps16Sat = &s_s16AcmLUTDeflt5_Sat[IspDev][0];
    pACM_PSCoef->ps16Hue = &s_s16AcmLUTDeflt5_Hue[IspDev][0];

    for (i = 0; i < 98; i++) //392/4=98
    {
        pBitCoef->stBit[i+563].bits_0 = VouBitValue(*pACM_PSCoef->ps16Luma++); 
        pBitCoef->stBit[i+563].bits_1 = VouBitValue(*pACM_PSCoef->ps16Luma++);    
        pBitCoef->stBit[i+563].bits_2 = VouBitValue(*pACM_PSCoef->ps16Sat++);

        s16Tmp = VouBitValue(*pACM_PSCoef->ps16Sat++);
        pBitCoef->stBit[i+563].bits_35 = s16Tmp;
        pBitCoef->stBit[i+563].bits_34 = (s16Tmp >> 5);

        pBitCoef->stBit[i+563].bits_4 = VouBitValue(*pACM_PSCoef->ps16Hue++);
        pBitCoef->stBit[i+563].bits_5 = VouBitValue(*pACM_PSCoef->ps16Hue++);
        pBitCoef->stBit[i+563].bits_6 = VouBitValue(*pACM_PSCoef->ps16Luma++);

        s16Tmp = VouBitValue(*pACM_PSCoef->ps16Luma++);
        pBitCoef->stBit[i+563].bits_75 = s16Tmp;
        pBitCoef->stBit[i+563].bits_74 = s16Tmp >> 5;

        pBitCoef->stBit[i+563].bits_8 = VouBitValue(*pACM_PSCoef->ps16Sat++);
        pBitCoef->stBit[i+563].bits_9 = VouBitValue(*pACM_PSCoef->ps16Sat++);
        pBitCoef->stBit[i+563].bits_10= VouBitValue(*pACM_PSCoef->ps16Hue++);

        s16Tmp = VouBitValue(*pACM_PSCoef->ps16Hue++);
        pBitCoef->stBit[i+563].bits_113 = s16Tmp;
        pBitCoef->stBit[i+563].bits_114 = s16Tmp >> 3;

        pBitCoef->stBit[i+563].bits_12 = 0;
    }

    pACM_PSCoef->ps16Luma = &s_s16AcmLUTDeflt6_Luma[IspDev][0];
    pACM_PSCoef->ps16Sat = &s_s16AcmLUTDeflt6_Sat[IspDev][0];
    pACM_PSCoef->ps16Hue = &s_s16AcmLUTDeflt6_Hue[IspDev][0];

    for (i = 0; i < 105; i++)   //420/4=105
    {
        pBitCoef->stBit[i+661].bits_0 = VouBitValue(*pACM_PSCoef->ps16Luma++);
        pBitCoef->stBit[i+661].bits_1 = VouBitValue(*pACM_PSCoef->ps16Luma++);    
        pBitCoef->stBit[i+661].bits_2 = VouBitValue(*pACM_PSCoef->ps16Sat++);
        
        s16Tmp = VouBitValue(*pACM_PSCoef->ps16Sat++);
        pBitCoef->stBit[i+661].bits_35 = s16Tmp;
        pBitCoef->stBit[i+661].bits_34 = (s16Tmp >> 5);
        
        pBitCoef->stBit[i+661].bits_4 = VouBitValue(*pACM_PSCoef->ps16Hue++);
        pBitCoef->stBit[i+661].bits_5 = VouBitValue(*pACM_PSCoef->ps16Hue++);
        pBitCoef->stBit[i+661].bits_6 = VouBitValue(*pACM_PSCoef->ps16Luma++);
        
        s16Tmp = VouBitValue(*pACM_PSCoef->ps16Luma++);
        pBitCoef->stBit[i+661].bits_75 = s16Tmp;
        pBitCoef->stBit[i+661].bits_74 = s16Tmp >> 5;
        
        pBitCoef->stBit[i+661].bits_8 = VouBitValue(*pACM_PSCoef->ps16Sat++);
        pBitCoef->stBit[i+661].bits_9 = VouBitValue(*pACM_PSCoef->ps16Sat++);
        pBitCoef->stBit[i+661].bits_10= VouBitValue(*pACM_PSCoef->ps16Hue++);
        
        s16Tmp = VouBitValue(*pACM_PSCoef->ps16Hue++);
        pBitCoef->stBit[i+661].bits_113 = s16Tmp;
        pBitCoef->stBit[i+661].bits_114 = s16Tmp >> 3;
        
        pBitCoef->stBit[i+661].bits_12 = 0;
    }

    pACM_PSCoef->ps16Luma = &s_s16AcmLUTDeflt7_Luma[IspDev][0];
    pACM_PSCoef->ps16Sat =  &s_s16AcmLUTDeflt7_Sat[IspDev][0];
    pACM_PSCoef->ps16Hue =  &s_s16AcmLUTDeflt7_Hue[IspDev][0];

    for (i = 0; i < 84; i++)   //336/4=84
    {
        pBitCoef->stBit[i+766].bits_0 = VouBitValue(*pACM_PSCoef->ps16Luma++);
        pBitCoef->stBit[i+766].bits_1 = VouBitValue(*pACM_PSCoef->ps16Luma++);    
        pBitCoef->stBit[i+766].bits_2 = VouBitValue(*pACM_PSCoef->ps16Sat++);

        s16Tmp = VouBitValue(*pACM_PSCoef->ps16Sat++);
        pBitCoef->stBit[i+766].bits_35 = s16Tmp;
        pBitCoef->stBit[i+766].bits_34 = (s16Tmp >> 5);

        pBitCoef->stBit[i+766].bits_4 = VouBitValue(*pACM_PSCoef->ps16Hue++);
        pBitCoef->stBit[i+766].bits_5 = VouBitValue(*pACM_PSCoef->ps16Hue++);
        pBitCoef->stBit[i+766].bits_6 = VouBitValue(*pACM_PSCoef->ps16Luma++);

        s16Tmp = VouBitValue(*pACM_PSCoef->ps16Luma++);
        pBitCoef->stBit[i+766].bits_75 = s16Tmp;
        pBitCoef->stBit[i+766].bits_74 = s16Tmp >> 5;

        pBitCoef->stBit[i+766].bits_8 = VouBitValue(*pACM_PSCoef->ps16Sat++);
        pBitCoef->stBit[i+766].bits_9 = VouBitValue(*pACM_PSCoef->ps16Sat++);
        pBitCoef->stBit[i+766].bits_10= VouBitValue(*pACM_PSCoef->ps16Hue++);

        s16Tmp = VouBitValue(*pACM_PSCoef->ps16Hue++);
        pBitCoef->stBit[i+766].bits_113 = s16Tmp;
        pBitCoef->stBit[i+766].bits_114 = s16Tmp >> 3;

        pBitCoef->stBit[i+766].bits_12 = 0;
    }

    pBitCoef->u32Size = COEFACMCNT * sizeof(ACM_COEF_BIT_S); 

    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype    : ACM_DRV_TransCoef
 Description  : Write ACM Lut to Ram
 Input        :   
 Output       : none
 Return Value : HI_VOID 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/04/29
    Author       : y00246723
    Modification : Created function

*****************************************************************************/

HI_VOID ACM_DRV_TransCoef(ISP_DEV IspDev, HI_VOID *pCoefMem, ISP_ACM_LUT_S *pstAcmLut)
{
    HI_S16* ps16AcmCoef = HI_NULL;
    osal_memset(&stArray[IspDev], 0, sizeof(ACM_COEF_BITARRAY_S));
    stArray[IspDev].u32Size = 850*16;

    ps16AcmCoef = (HI_S16 *)pstAcmLut;
	
    HI_ASSERT(ps16AcmCoef != HI_NULL);
    HI_ASSERT(pCoefMem != HI_NULL);
    
    ACM_DRV_TransCoefAlign(IspDev, ps16AcmCoef, &stArray[IspDev]); 				//transform the table to struct
    osal_memcpy((HI_U8*)pCoefMem, stArray[IspDev].stBit, stArray[IspDev].u32Size); 	//memcpy copy to suppositional address
    
}

/*****************************************************************************
 Prototype    : ACM_DRV_UpdateRegister
 Description  : 
 Input        :   
 Output       : none
 Return Value : HI_VOID 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/04/29
    Author       : y00246723
    Modification : Created function

*****************************************************************************/
HI_VOID ACM_DRV_UpdateRegister(ISP_DEV IspDev, HI_U32 u32AcmCoefAddr)
{
    ISP_ACM_CTX_S* pstAcmCtx = &g_stAcmCtx[IspDev];
    unsigned long u32Flags;

    HI_ASSERT(u32AcmCoefAddr != 0);

    osal_spin_lock_irqsave(&g_stAcmLock[IspDev], &u32Flags);
    
    hi_vi_acm_write_luma_gain(IspDev, pstAcmCtx->stAcmCtrl.u32GainLuma);
    hi_vi_acm_write_sat_gain(IspDev, pstAcmCtx->stAcmCtrl.u32GainSat);
    hi_vi_acm_write_hue_gain(IspDev, pstAcmCtx->stAcmCtrl.u32GainHue);
    hi_vi_acm_write_cbcrth(IspDev, pstAcmCtx->stAcmCtrl.u32CbcrThr);
    hi_vi_acm_write_cliprange(IspDev, pstAcmCtx->stAcmCtrl.u32ClipRange);
    hi_vi_acm_write_stretch(IspDev, pstAcmCtx->stAcmCtrl.u32Stretch);
    hi_vi_acm_write_cliporwrap(IspDev, pstAcmCtx->stAcmCtrl.u32AcmClipOrWrap);
    
    hi_vi_acm_write_coeff_addr(IspDev, u32AcmCoefAddr);
    /* In chip power up, there maybe a wrong interrput!!! */
    hi_vi_acm_write_para_up(IspDev, 1);

    hi_vi_acm_write_enable(IspDev, (HI_BOOL)pstAcmCtx->stAcmCtrl.bEnable);
    hi_vi_acm_write_dbg_enable(IspDev, (HI_BOOL)pstAcmCtx->stAcmCtrl.bDemoEnable);

    osal_spin_unlock_irqrestore(&g_stAcmLock[IspDev], &u32Flags);
}

/*****************************************************************************
 Prototype    : ACM_DRV_InitDefault
 Description  : 
 Input        :   
 Output       : none
 Return Value : HI_VOID 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/04/29
    Author       : y00246723
    Modification : Created function

*****************************************************************************/
HI_S32 ACM_DRV_InitDefault(ISP_DEV IspDev, ISP_ACM_CTX_S* pstAcmCtx)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_S32 s32AcmCoefSize; 
    
    if(osal_down_interruptible(&pstAcmCtx->stAcmSem))
    {
       return -ERESTARTSYS;
    }

    pstAcmCtx->stAcmCtrl.bEnable = HI_FALSE;
    pstAcmCtx->stAcmCtrl.bDemoEnable = HI_FALSE;
    pstAcmCtx->stAcmCtrl.u32GainLuma = 64;
    pstAcmCtx->stAcmCtrl.u32GainSat = 64;
    pstAcmCtx->stAcmCtrl.u32GainHue = 64;
    
    pstAcmCtx->stAcmCtrl.u32CbcrThr = 0;
    pstAcmCtx->stAcmCtrl.u32ClipRange = 1;
    pstAcmCtx->stAcmCtrl.u32Stretch = 1;
    pstAcmCtx->stAcmCtrl.u32AcmClipOrWrap = 0;

    s32AcmCoefSize = sizeof(HI_S16)*LUTDEFNUM;

    osal_memcpy(&(pstAcmCtx->pstCoefLut->as16Y) ,&s16AcmLUTDeflt[0][0][0][0],s32AcmCoefSize);
    osal_memcpy(&(pstAcmCtx->pstCoefLut->as16H) ,&s16AcmLUTDeflt[1][0][0][0],s32AcmCoefSize);
    osal_memcpy(&(pstAcmCtx->pstCoefLut->as16S) ,&s16AcmLUTDeflt[2][0][0][0],s32AcmCoefSize);

    ACM_DRV_TransCoef(IspDev, (HI_VOID*)pstAcmCtx->stCoefBuff.pVirAddr, pstAcmCtx->pstCoefLut); 

    osal_up(&pstAcmCtx->stAcmSem);

    ACM_DRV_UpdateRegister(IspDev, pstAcmCtx->stCoefBuff.u32PhyAddr);

    return s32Ret;
}

/*****************************************************************************
 Prototype    : ISP_ACM_SetAttr
 Description  : Set ACM attribute
 Input        :   
 Output       : none
 Return Value : HI_S32 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/04/29
    Author       : y00246723
    Modification : Created function

*****************************************************************************/
HI_S32 ISP_ACM_DRV_SetAttr(ISP_DEV IspDev, ISP_ACM_ATTR_S *pstAcmAttr)
{
    ISP_ACM_CTX_S* pstAcmCtx = &g_stAcmCtx[IspDev];

    ACM_CHECK_NULL(pstAcmAttr);
    ACM_CHECK_INIT(IspDev);

    if(osal_down_interruptible(&pstAcmCtx->stAcmSem))
    {
       return -ERESTARTSYS;
    }
    osal_memcpy((HI_VOID*)&pstAcmCtx->stAcmCtrl, (HI_VOID*)pstAcmAttr, sizeof(ISP_ACM_ATTR_S)); 
   
    osal_up(&pstAcmCtx->stAcmSem);

    ACM_DRV_UpdateRegister(IspDev, pstAcmCtx->stCoefBuff.u32PhyAddr);


    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype    : ISP_ACM_GetAttr
 Description  : Get ACM attribute
 Input        :   
 Output       : none
 Return Value : HI_S32 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/04/29
    Author       : y00246723
    Modification : Created function

*****************************************************************************/
HI_S32 ISP_ACM_DRV_GetAttr(ISP_DEV IspDev, ISP_ACM_ATTR_S *pstAcmAttr)
{
    ISP_ACM_CTX_S* pstAcmCtx = &g_stAcmCtx[IspDev];

    ACM_CHECK_NULL(pstAcmAttr);
    ACM_CHECK_INIT(IspDev);

    if(osal_down_interruptible(&pstAcmCtx->stAcmSem))
    {
       return -ERESTARTSYS;
    }
    osal_memcpy((HI_VOID*)pstAcmAttr, (HI_VOID*)&pstAcmCtx->stAcmCtrl, sizeof(ISP_ACM_ATTR_S));
    osal_up(&pstAcmCtx->stAcmSem);
    
    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype    : ISP_ACM_SetCoeff
 Description  : Set ACM coeff
 Input        :   
 Output       : none
 Return Value : HI_S32 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/04/29
    Author       : y00246723
    Modification : Created function

*****************************************************************************/
HI_S32 ISP_ACM_DRV_SetCoeff(ISP_DEV IspDev)
{
    ISP_ACM_CTX_S* pstAcmCtx = &g_stAcmCtx[IspDev];
	
    ACM_CHECK_INIT(IspDev);

    if(osal_down_interruptible(&pstAcmCtx->stAcmSem))
    {
       return -ERESTARTSYS;
    }
   
    ACM_DRV_TransCoef(IspDev, (HI_VOID*)pstAcmCtx->stCoefBuff.pVirAddr, pstAcmCtx->pstCoefLut); 
    osal_up(&pstAcmCtx->stAcmSem);
	
    ACM_DRV_UpdateRegister(IspDev, pstAcmCtx->stCoefBuff.u32PhyAddr);

    return HI_SUCCESS;
}


/********************* INTERFACE****************************/

/*****************************************************************************
 Prototype    : ISP_AcmCtrl
 Description  : ACM interface Ctrl
 Input        :   
 Output       : none
 Return Value : HI_S32 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/04/29
    Author       : y00246723
    Modification : Created function

*****************************************************************************/
HI_S32 ISP_ACM_DRV_Ioctrl(unsigned int cmd, unsigned long arg, void *private_data)
{
    unsigned int *argp = (unsigned int *)arg;
    ISP_DEV IspDev;

    IspDev = ISP_GET_DEV(private_data);

    ACM_CHECK_INIT(IspDev);

    switch (cmd)
    {
        case ISP_ACM_ATTR_GET:
        {
            ISP_ACM_ATTR_S       stAcmAttr = {0};

            ISP_ACM_DRV_GetAttr(IspDev, &stAcmAttr);

            osal_memcpy(argp, &stAcmAttr, sizeof(ISP_ACM_ATTR_S));
            return 0;
        }
        case ISP_ACM_ATTR_SET:
        {
            ISP_ACM_DRV_SetAttr(IspDev, (ISP_ACM_ATTR_S *)argp);

            return 0;
        }
        case ISP_ACM_LUT_PHY_ADDR_GET:
        {

			osal_memcpy(argp, &g_stAcmCtx[IspDev].stMmzBuff, sizeof(ISP_ACM_BUFFER_S));
            return 0;
        }
        case ISP_ACM_COEF_SET:
        {
            ISP_ACM_DRV_SetCoeff(IspDev);
            return 0;
        }
        default :
            break;
    }
    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype    : ISP_ACM_Init
 Description  : ACM ALG initialize
 Input        :   
 Output       : none
 Return Value : HI_S32 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/04/29
    Author       : y00246723
    Modification : Created function

*****************************************************************************/
HI_S32 ISP_ACM_DRV_Init(ISP_DEV IspDev)
{   
    HI_U32 u32PhyAddr;
    HI_U8  *pu8VirAddr;
    HI_U32 u32CoefPhyAddr;
    HI_U8  *pu8CoefVirAddr;
    HI_U32 u32Size;
    ISP_ACM_CTX_S* pstAcmCtx = &g_stAcmCtx[IspDev];
    HI_S32 s32Ret = HI_SUCCESS;

    if(HI_TRUE == g_bInited[IspDev])
    {
        ISP_TRACE(HI_DBG_ERR, "err acm reinitialized\n");
        return HI_FAILURE;
    }

    osal_spin_lock_init(&g_stAcmLock[IspDev]);
    osal_sema_init(&pstAcmCtx->stAcmSem,1);
    u32Size = ADDROFFSET;
    s32Ret = CMPI_MmzMallocNocache(HI_NULL, "ACM LUTS", &u32PhyAddr, (HI_VOID**)&pu8VirAddr, u32Size);
    if (HI_SUCCESS != s32Ret)
    {
        ISP_TRACE(HI_DBG_ERR, "alloc acm luts buf err\n");
        return HI_ERR_ISP_NOMEM;
    }

    u32Size = ACMADDROFFSET;
    s32Ret = CMPI_MmzMallocCached(HI_NULL, "ACM Coef", &u32CoefPhyAddr, (HI_VOID**)&pu8CoefVirAddr, u32Size);
    if (HI_SUCCESS != s32Ret)
    {
        ISP_TRACE(HI_DBG_ERR, "alloc acm coef buf err\n");
        return HI_ERR_ISP_NOMEM;
    }

    pstAcmCtx->stMmzBuff.pVirAddr       = pu8VirAddr;
    pstAcmCtx->stMmzBuff.u32PhyAddr     = u32PhyAddr;
    pstAcmCtx->stMmzBuff.u32AcmSize     = ADDROFFSET;

    pstAcmCtx->stCoefBuff.u32PhyAddr    = u32CoefPhyAddr;
    pstAcmCtx->stCoefBuff.pVirAddr      = pu8CoefVirAddr;
    pstAcmCtx->stCoefBuff.u32AcmSize    = ACMADDROFFSET;

    pstAcmCtx->pstCoefLut = (ISP_ACM_LUT_S*)pu8VirAddr;
    
    ACM_DRV_InitDefault(IspDev, pstAcmCtx);

    g_bInited[IspDev] = HI_TRUE;
    
    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype    : ISP_ACM_Exit
 Description  : ACM interface Exit
 Input        :   
 Output       : none
 Return Value : HI_S32 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/04/29
    Author       : y00246723
    Modification : Created function

*****************************************************************************/
HI_S32 ISP_ACM_DRV_Exit(ISP_DEV IspDev)
{
    ISP_ACM_CTX_S* pstAcmCtx = &g_stAcmCtx[IspDev];
    unsigned long u32Flags;

    HI_U32 u32PhyAddr; 
    HI_U8* pu8VirAddr; 
    HI_U32 u32CoefPhyAddr; 
    HI_U8* pu8CoefVirAddr; 

    ACM_CHECK_INIT(IspDev);
    g_bInited[IspDev] = HI_FALSE;

    u32PhyAddr = pstAcmCtx->stMmzBuff.u32PhyAddr;
    pu8VirAddr = (HI_U8 *)pstAcmCtx->stMmzBuff.pVirAddr;
    u32CoefPhyAddr = pstAcmCtx->stCoefBuff.u32PhyAddr;
    pu8CoefVirAddr = (HI_U8 *)pstAcmCtx->stCoefBuff.pVirAddr;


    osal_spin_lock_irqsave(&g_stAcmLock[IspDev], &u32Flags);
    hi_vi_acm_write_coeff_addr(IspDev, 0);
    osal_spin_unlock_irqrestore(&g_stAcmLock[IspDev], &u32Flags);

    if(osal_down_interruptible(&pstAcmCtx->stAcmSem))
    {
       return -ERESTARTSYS;
    }
    pstAcmCtx->stMmzBuff.u32PhyAddr = 0;
    pstAcmCtx->stMmzBuff.pVirAddr   = HI_NULL;
    pstAcmCtx->stCoefBuff.u32PhyAddr = 0;
    pstAcmCtx->stCoefBuff.pVirAddr   = HI_NULL;
    osal_up(&pstAcmCtx->stAcmSem);

    if (0 != u32PhyAddr)
    {
        CMPI_MmzFree(u32PhyAddr, pu8VirAddr);
    }

    if (0 != u32CoefPhyAddr)
    {
        hil_mmb_invalid_cache_byaddr(pu8CoefVirAddr, u32CoefPhyAddr, pstAcmCtx->stCoefBuff.u32AcmSize);
        CMPI_MmzFree(u32CoefPhyAddr, pu8CoefVirAddr);
    }
    osal_sema_destory(&pstAcmCtx->stAcmSem);
    osal_spin_lock_destory(&g_stAcmLock[IspDev]);
    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

