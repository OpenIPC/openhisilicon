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

#include "isp_config.h"

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
static HI_S16 s16AcmLUT[3][9][13][29] = {{{{0}}}};

//Temp ACM Lut for logic format
static HI_S16 s_s16AcmLUTDeflt0_Luma[525]={0};
static HI_S16 s_s16AcmLUTDeflt0_Sat[525] ={0};
static HI_S16 s_s16AcmLUTDeflt0_Hue[525] ={0};

static HI_S16 s_s16AcmLUTDeflt1_Luma[420]={0};
static HI_S16 s_s16AcmLUTDeflt1_Sat[420] ={0};
static HI_S16 s_s16AcmLUTDeflt1_Hue[420] ={0};

static HI_S16 s_s16AcmLUTDeflt2_Luma[450]={0};
static HI_S16 s_s16AcmLUTDeflt2_Sat[450] ={0};
static HI_S16 s_s16AcmLUTDeflt2_Hue[450] ={0};

static HI_S16 s_s16AcmLUTDeflt3_Luma[360]={0};
static HI_S16 s_s16AcmLUTDeflt3_Sat[360] ={0};
static HI_S16 s_s16AcmLUTDeflt3_Hue[360] ={0};

static HI_S16 s_s16AcmLUTDeflt4_Luma[490]={0};
static HI_S16 s_s16AcmLUTDeflt4_Sat[490] ={0};
static HI_S16 s_s16AcmLUTDeflt4_Hue[490] ={0};

static HI_S16 s_s16AcmLUTDeflt5_Luma[392]={0};
static HI_S16 s_s16AcmLUTDeflt5_Sat[392] ={0};
static HI_S16 s_s16AcmLUTDeflt5_Hue[392] ={0};

static HI_S16 s_s16AcmLUTDeflt6_Luma[420]={0};
static HI_S16 s_s16AcmLUTDeflt6_Sat[420] ={0};
static HI_S16 s_s16AcmLUTDeflt6_Hue[420] ={0};

static HI_S16 s_s16AcmLUTDeflt7_Luma[336]={0};
static HI_S16 s_s16AcmLUTDeflt7_Sat[336] ={0};
static HI_S16 s_s16AcmLUTDeflt7_Hue[336] ={0};

static ACM_COEF_BITARRAY_S  stArray;

osal_spinlock_t g_stAcmLock;

/* */
static ISP_ACM_CTX_S               g_stAcmCtx;
static HI_BOOL                     g_bInited = HI_FALSE;

#define ACM_CHECK_NULL(p) \
{\
    if(HI_NULL == (p)) { return HI_FAILURE;}\
}\


HI_S32 ISP_ACM_DRV_SetCoeff(HI_VOID);
HI_S32 ISP_ACM_DRV_GetAttr(ISP_ACM_ATTR_S *pstAcmAttr);
HI_S32 ISP_ACM_DRV_SetAttr(ISP_ACM_ATTR_S *pstAcmAttr);


#define ACM_CHECK_INIT() \
{\
    if(HI_TRUE != g_bInited) \
    {\
        ISP_TRACE(HI_DBG_ERR, "Error acm not initialized\n");\
        return HI_DEF_ERR(HI_ID_ISP,EN_ERR_LEVEL_ERROR,EN_ERR_SYS_NOTREADY);\
    }\
}\

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
static HI_U32 ACM_DRV_SplitArray(const HI_S16 * pstAcmCoefArray)
{
    HI_S32 i, j, k;
    HI_S16 *pLUT0, *pLUT1, *pLUT2, *pLUT3, *pLUT4, *pLUT5, *pLUT6, *pLUT7;

    osal_memcpy(&s16AcmLUT[0][0][0][0],pstAcmCoefArray,3*29*13*9*sizeof(HI_S16));

    pLUT0 = s_s16AcmLUTDeflt0_Luma;
    pLUT1 = s_s16AcmLUTDeflt1_Luma;
    pLUT2 = s_s16AcmLUTDeflt2_Luma;
    pLUT3 = s_s16AcmLUTDeflt3_Luma;
    pLUT4 = s_s16AcmLUTDeflt4_Luma;
    pLUT5 = s_s16AcmLUTDeflt5_Luma;
    pLUT6 = s_s16AcmLUTDeflt6_Luma;
    pLUT7 = s_s16AcmLUTDeflt7_Luma;

    for(k = 0; k < 29; k++)
    {
        for(j = 0; j < 13; j++)
        {
            for(i = 0; i < 9; i++)
            {
                switch((i&1) + ((j&1)<<1) + ((k&1)<<2))//Reorder
                {
                case 0:
                    *pLUT0 = s16AcmLUT[0][i][j][k];
                    pLUT0++;
                    break;
                case 1:
                    *pLUT1 = s16AcmLUT[0][i][j][k];
                    pLUT1++;
                    break;
                case 2:
                    *pLUT2 = s16AcmLUT[0][i][j][k];
                    pLUT2++;
                    break;
                case 3:
                    *pLUT3 = s16AcmLUT[0][i][j][k];
                    pLUT3++;
                    break;
                case 4:
                    *pLUT4 = s16AcmLUT[0][i][j][k];
                    pLUT4++;
                    break;
                case 5:
                    *pLUT5 = s16AcmLUT[0][i][j][k];
                    pLUT5++;
                    break;
                case 6:
                    *pLUT6 = s16AcmLUT[0][i][j][k];
                    pLUT6++;
                    break;
                case 7:
                    *pLUT7 = s16AcmLUT[0][i][j][k];
                    pLUT7++;
                    break;
                }
            }
        }
    }

    pLUT0 = s_s16AcmLUTDeflt0_Hue;
    pLUT1 = s_s16AcmLUTDeflt1_Hue;
    pLUT2 = s_s16AcmLUTDeflt2_Hue;
    pLUT3 = s_s16AcmLUTDeflt3_Hue;
    pLUT4 = s_s16AcmLUTDeflt4_Hue;
    pLUT5 = s_s16AcmLUTDeflt5_Hue;
    pLUT6 = s_s16AcmLUTDeflt6_Hue;
    pLUT7 = s_s16AcmLUTDeflt7_Hue;

    for(k = 0; k < 29; k++)
    {
        for(j = 0; j < 13; j++)
        {
            for(i = 0; i < 9; i++)
            {
                switch((i&1) + ((j&1)<<1) + ((k&1)<<2))
                {
                case 0:
                    *pLUT0 = s16AcmLUT[1][i][j][k];
                    pLUT0++;
                    break;
                case 1:
                    *pLUT1 = s16AcmLUT[1][i][j][k];
                    pLUT1++;
                    break;
                case 2:
                    *pLUT2 = s16AcmLUT[1][i][j][k];
                    pLUT2++;
                    break;
                case 3:
                    *pLUT3 = s16AcmLUT[1][i][j][k];
                    pLUT3++;
                    break;
                case 4:
                    *pLUT4 = s16AcmLUT[1][i][j][k];
                    pLUT4++;
                    break;
                case 5:
                    *pLUT5 = s16AcmLUT[1][i][j][k];
                    pLUT5++;
                    break;
                case 6:
                    *pLUT6 = s16AcmLUT[1][i][j][k];
                    pLUT6++;
                    break;
                case 7:
                    *pLUT7 = s16AcmLUT[1][i][j][k];
                    pLUT7++;
                    break;
                }
            }
        }
    }

    pLUT0 = s_s16AcmLUTDeflt0_Sat;
    pLUT1 = s_s16AcmLUTDeflt1_Sat;
    pLUT2 = s_s16AcmLUTDeflt2_Sat;
    pLUT3 = s_s16AcmLUTDeflt3_Sat;
    pLUT4 = s_s16AcmLUTDeflt4_Sat;
    pLUT5 = s_s16AcmLUTDeflt5_Sat;
    pLUT6 = s_s16AcmLUTDeflt6_Sat;
    pLUT7 = s_s16AcmLUTDeflt7_Sat;

    for(k = 0; k < 29; k++)
    {
        for(j = 0; j < 13; j++)
        {
            for(i = 0; i < 9; i++)
            {
                switch((i&1) + ((j&1)<<1) + ((k&1)<<2))
                {
                case 0:
                    *pLUT0 = s16AcmLUT[2][i][j][k];
                    pLUT0++;
                    break;
                case 1:
                    *pLUT1 = s16AcmLUT[2][i][j][k];
                    pLUT1++;
                    break;
                case 2:
                    *pLUT2 = s16AcmLUT[2][i][j][k];
                    pLUT2++;
                    break;
                case 3:
                    *pLUT3 = s16AcmLUT[2][i][j][k];
                    pLUT3++;
                    break;
                case 4:
                    *pLUT4 = s16AcmLUT[2][i][j][k];
                    pLUT4++;
                    break;
                case 5:
                    *pLUT5 = s16AcmLUT[2][i][j][k];
                    pLUT5++;
                    break;
                case 6:
                    *pLUT6 = s16AcmLUT[2][i][j][k];
                    pLUT6++;
                    break;
                case 7:
                    *pLUT7 = s16AcmLUT[2][i][j][k];
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
static HI_U32 ACM_DRV_TransCoefAlign(const HI_S16 * pstAcmCoefArray, ACM_COEF_BITARRAY_S *pBitCoef)
{
    HI_S16 i;
    ACM_PSCoef stACM_PSCoef;
    ACM_PSCoef *pACM_PSCoef = &stACM_PSCoef;

	ACM_PSCoef stACM_PSCoef1;
    ACM_PSCoef *pACM_PSCoef1 = &stACM_PSCoef1;

    ACM_DRV_SplitArray(pstAcmCoefArray);

    pACM_PSCoef->ps16Luma = &s_s16AcmLUTDeflt0_Luma[0];
    pACM_PSCoef->ps16Sat  = &s_s16AcmLUTDeflt0_Sat[0];
    pACM_PSCoef->ps16Hue  = &s_s16AcmLUTDeflt0_Hue[0];

	pACM_PSCoef1->ps16Luma = &s_s16AcmLUTDeflt4_Luma[0];
	pACM_PSCoef1->ps16Sat  = &s_s16AcmLUTDeflt4_Sat[0];
	pACM_PSCoef1->ps16Hue  = &s_s16AcmLUTDeflt4_Hue[0];

    for (i = 0; i < 263; i++) // 525/2=262.5
    {
		if(i==262)
        {
			pBitCoef->stBit[i].bits_0 = VouBitValue(*pACM_PSCoef->ps16Luma++);
			pBitCoef->stBit[i].bits_1 = 0;
			pBitCoef->stBit[i].bits_2 = VouBitValue(*pACM_PSCoef->ps16Sat++);
			pBitCoef->stBit[i].bits_3 = 0;
			pBitCoef->stBit[i].bits_4 = VouBitValue(*pACM_PSCoef->ps16Hue++);
			pBitCoef->stBit[i].bits_5 = 0;

			pBitCoef->stBit[i].bits_6 = 0;
			pBitCoef->stBit[i].bits_7 = 0;
			pBitCoef->stBit[i].bits_8 = 0;
			pBitCoef->stBit[i].bits_9 = 0;
			pBitCoef->stBit[i].bits_10 = 0;
			pBitCoef->stBit[i].bits_11 = 0;

			pBitCoef->stBit[i].bits_12 = 0;
			pBitCoef->stBit[i].bits_13 = 0;
			pBitCoef->stBit[i].bits_14 = 0;
			pBitCoef->stBit[i].bits_15 = 0;
			pBitCoef->stBit[i].bits_16 = 0;
			pBitCoef->stBit[i].bits_17 = 0;

			pBitCoef->stBit[i].bits_18 = 0;
			pBitCoef->stBit[i].bits_19 = 0;
			pBitCoef->stBit[i].bits_20 = 0;
			pBitCoef->stBit[i].bits_21 = 0;
			pBitCoef->stBit[i].bits_22 = 0;
			pBitCoef->stBit[i].bits_23 = 0;
        }
		else if(i>=245)  //490/2=245
        {
			pBitCoef->stBit[i].bits_0 = VouBitValue(*pACM_PSCoef->ps16Luma++);
			pBitCoef->stBit[i].bits_1 = 0;
			pBitCoef->stBit[i].bits_2 = VouBitValue(*pACM_PSCoef->ps16Sat++);
			pBitCoef->stBit[i].bits_3 = 0;
			pBitCoef->stBit[i].bits_4 = VouBitValue(*pACM_PSCoef->ps16Hue++);
			pBitCoef->stBit[i].bits_5 = 0;

			pBitCoef->stBit[i].bits_6 = VouBitValue(*pACM_PSCoef->ps16Luma++);
			pBitCoef->stBit[i].bits_7 = 0;
			pBitCoef->stBit[i].bits_8 = VouBitValue(*pACM_PSCoef->ps16Sat++);
			pBitCoef->stBit[i].bits_9 = 0;
			pBitCoef->stBit[i].bits_10 = VouBitValue(*pACM_PSCoef->ps16Hue++);
			pBitCoef->stBit[i].bits_11 = 0;

			pBitCoef->stBit[i].bits_12 = 0;
			pBitCoef->stBit[i].bits_13 = 0;
			pBitCoef->stBit[i].bits_14 = 0;
			pBitCoef->stBit[i].bits_15 = 0;
			pBitCoef->stBit[i].bits_16 = 0;
			pBitCoef->stBit[i].bits_17 = 0;

			pBitCoef->stBit[i].bits_18 = 0;
			pBitCoef->stBit[i].bits_19 = 0;
			pBitCoef->stBit[i].bits_20 = 0;
			pBitCoef->stBit[i].bits_21 = 0;
			pBitCoef->stBit[i].bits_22 = 0;
			pBitCoef->stBit[i].bits_23 = 0;
        }
        else
        {
			pBitCoef->stBit[i].bits_0 = VouBitValue(*pACM_PSCoef->ps16Luma++);
			pBitCoef->stBit[i].bits_1 = 0;
			pBitCoef->stBit[i].bits_2 = VouBitValue(*pACM_PSCoef->ps16Sat++);
			pBitCoef->stBit[i].bits_3 = 0;
			pBitCoef->stBit[i].bits_4 = VouBitValue(*pACM_PSCoef->ps16Hue++);
			pBitCoef->stBit[i].bits_5 = 0;

			pBitCoef->stBit[i].bits_6 = VouBitValue(*pACM_PSCoef->ps16Luma++);
			pBitCoef->stBit[i].bits_7 = 0;
			pBitCoef->stBit[i].bits_8 = VouBitValue(*pACM_PSCoef->ps16Sat++);
			pBitCoef->stBit[i].bits_9 = 0;
			pBitCoef->stBit[i].bits_10 = VouBitValue(*pACM_PSCoef->ps16Hue++);
			pBitCoef->stBit[i].bits_11 = 0;

			pBitCoef->stBit[i].bits_12 = VouBitValue(*pACM_PSCoef1->ps16Luma++);
			pBitCoef->stBit[i].bits_13 = 0;
			pBitCoef->stBit[i].bits_14 = VouBitValue(*pACM_PSCoef1->ps16Sat++);
			pBitCoef->stBit[i].bits_15 = 0;
			pBitCoef->stBit[i].bits_16 = VouBitValue(*pACM_PSCoef1->ps16Hue++);
			pBitCoef->stBit[i].bits_17 = 0;

			pBitCoef->stBit[i].bits_18 = VouBitValue(*pACM_PSCoef1->ps16Luma++);
			pBitCoef->stBit[i].bits_19 = 0;
			pBitCoef->stBit[i].bits_20 = VouBitValue(*pACM_PSCoef1->ps16Sat++);
			pBitCoef->stBit[i].bits_21 = 0;
			pBitCoef->stBit[i].bits_22 = VouBitValue(*pACM_PSCoef1->ps16Hue++);
			pBitCoef->stBit[i].bits_23 = 0;
		}
    }

    pACM_PSCoef->ps16Luma = &s_s16AcmLUTDeflt1_Luma[0];
    pACM_PSCoef->ps16Sat  = &s_s16AcmLUTDeflt1_Sat[0];
    pACM_PSCoef->ps16Hue  = &s_s16AcmLUTDeflt1_Hue[0];

	pACM_PSCoef1->ps16Luma = &s_s16AcmLUTDeflt5_Luma[0];
	pACM_PSCoef1->ps16Sat  = &s_s16AcmLUTDeflt5_Sat[0];
	pACM_PSCoef1->ps16Hue  = &s_s16AcmLUTDeflt5_Hue[0];

    for (i = 0; i < 210; i++)  // 420/2=210
    {
		if(i>=196)  //392/2=196
        {
			pBitCoef->stBit[i+264].bits_0 = VouBitValue(*pACM_PSCoef->ps16Luma++);
			pBitCoef->stBit[i+264].bits_1 = 0;
			pBitCoef->stBit[i+264].bits_2 = VouBitValue(*pACM_PSCoef->ps16Sat++);
			pBitCoef->stBit[i+264].bits_3 = 0;
			pBitCoef->stBit[i+264].bits_4 = VouBitValue(*pACM_PSCoef->ps16Hue++);
			pBitCoef->stBit[i+264].bits_5 = 0;

			pBitCoef->stBit[i+264].bits_6 = VouBitValue(*pACM_PSCoef->ps16Luma++);
			pBitCoef->stBit[i+264].bits_7 = 0;
			pBitCoef->stBit[i+264].bits_8 = VouBitValue(*pACM_PSCoef->ps16Sat++);
			pBitCoef->stBit[i+264].bits_9 = 0;
			pBitCoef->stBit[i+264].bits_10 = VouBitValue(*pACM_PSCoef->ps16Hue++);
			pBitCoef->stBit[i+264].bits_11 = 0;

			pBitCoef->stBit[i+264].bits_12 = 0;
			pBitCoef->stBit[i+264].bits_13 = 0;
			pBitCoef->stBit[i+264].bits_14 = 0;
			pBitCoef->stBit[i+264].bits_15 = 0;
			pBitCoef->stBit[i+264].bits_16 = 0;
			pBitCoef->stBit[i+264].bits_17 = 0;

			pBitCoef->stBit[i+264].bits_18 = 0;
			pBitCoef->stBit[i+264].bits_19 = 0;
			pBitCoef->stBit[i+264].bits_20 = 0;
			pBitCoef->stBit[i+264].bits_21 = 0;
			pBitCoef->stBit[i+264].bits_22 = 0;
			pBitCoef->stBit[i+264].bits_23 = 0;
        }
        else
        {
			pBitCoef->stBit[i+264].bits_0 = VouBitValue(*pACM_PSCoef->ps16Luma++);
			pBitCoef->stBit[i+264].bits_1 = 0;
			pBitCoef->stBit[i+264].bits_2 = VouBitValue(*pACM_PSCoef->ps16Sat++);
			pBitCoef->stBit[i+264].bits_3 = 0;
			pBitCoef->stBit[i+264].bits_4 = VouBitValue(*pACM_PSCoef->ps16Hue++);
			pBitCoef->stBit[i+264].bits_5 = 0;

			pBitCoef->stBit[i+264].bits_6 = VouBitValue(*pACM_PSCoef->ps16Luma++);
			pBitCoef->stBit[i+264].bits_7 = 0;
			pBitCoef->stBit[i+264].bits_8 = VouBitValue(*pACM_PSCoef->ps16Sat++);
			pBitCoef->stBit[i+264].bits_9 = 0;
			pBitCoef->stBit[i+264].bits_10 = VouBitValue(*pACM_PSCoef->ps16Hue++);
			pBitCoef->stBit[i+264].bits_11 = 0;

			pBitCoef->stBit[i+264].bits_12 = VouBitValue(*pACM_PSCoef1->ps16Luma++);
			pBitCoef->stBit[i+264].bits_13 = 0;
			pBitCoef->stBit[i+264].bits_14 = VouBitValue(*pACM_PSCoef1->ps16Sat++);
			pBitCoef->stBit[i+264].bits_15 = 0;
			pBitCoef->stBit[i+264].bits_16 = VouBitValue(*pACM_PSCoef1->ps16Hue++);
			pBitCoef->stBit[i+264].bits_17 = 0;

			pBitCoef->stBit[i+264].bits_18 = VouBitValue(*pACM_PSCoef1->ps16Luma++);
			pBitCoef->stBit[i+264].bits_19 = 0;
			pBitCoef->stBit[i+264].bits_20 = VouBitValue(*pACM_PSCoef1->ps16Sat++);
			pBitCoef->stBit[i+264].bits_21 = 0;
			pBitCoef->stBit[i+264].bits_22 = VouBitValue(*pACM_PSCoef1->ps16Hue++);
			pBitCoef->stBit[i+264].bits_23 = 0;
		}
    }

    pACM_PSCoef->ps16Luma = &s_s16AcmLUTDeflt2_Luma[0];
    pACM_PSCoef->ps16Sat  = &s_s16AcmLUTDeflt2_Sat[0];
    pACM_PSCoef->ps16Hue  = &s_s16AcmLUTDeflt2_Hue[0];

	pACM_PSCoef1->ps16Luma = &s_s16AcmLUTDeflt6_Luma[0];
	pACM_PSCoef1->ps16Sat  = &s_s16AcmLUTDeflt6_Sat[0];
	pACM_PSCoef1->ps16Hue  = &s_s16AcmLUTDeflt6_Hue[0];

    for (i = 0; i < 225; i++) // 450/2=225
    {
		if(i>=210)  //420/2=210
        {
			pBitCoef->stBit[i+476].bits_0 = VouBitValue(*pACM_PSCoef->ps16Luma++);
			pBitCoef->stBit[i+476].bits_1 = 0;
			pBitCoef->stBit[i+476].bits_2 = VouBitValue(*pACM_PSCoef->ps16Sat++);
			pBitCoef->stBit[i+476].bits_3 = 0;
			pBitCoef->stBit[i+476].bits_4 = VouBitValue(*pACM_PSCoef->ps16Hue++);
			pBitCoef->stBit[i+476].bits_5 = 0;

			pBitCoef->stBit[i+476].bits_6 = VouBitValue(*pACM_PSCoef->ps16Luma++);
			pBitCoef->stBit[i+476].bits_7 = 0;
			pBitCoef->stBit[i+476].bits_8 = VouBitValue(*pACM_PSCoef->ps16Sat++);
			pBitCoef->stBit[i+476].bits_9 = 0;
			pBitCoef->stBit[i+476].bits_10 = VouBitValue(*pACM_PSCoef->ps16Hue++);
			pBitCoef->stBit[i+476].bits_11 = 0;

			pBitCoef->stBit[i+476].bits_12 = 0;
			pBitCoef->stBit[i+476].bits_13 = 0;
			pBitCoef->stBit[i+476].bits_14 = 0;
			pBitCoef->stBit[i+476].bits_15 = 0;
			pBitCoef->stBit[i+476].bits_16 = 0;
			pBitCoef->stBit[i+476].bits_17 = 0;

			pBitCoef->stBit[i+476].bits_18 = 0;
			pBitCoef->stBit[i+476].bits_19 = 0;
			pBitCoef->stBit[i+476].bits_20 = 0;
			pBitCoef->stBit[i+476].bits_21 = 0;
			pBitCoef->stBit[i+476].bits_22 = 0;
			pBitCoef->stBit[i+476].bits_23 = 0;
        }
		else
        {
			pBitCoef->stBit[i+476].bits_0 = VouBitValue(*pACM_PSCoef->ps16Luma++);
			pBitCoef->stBit[i+476].bits_1 = 0;
			pBitCoef->stBit[i+476].bits_2 = VouBitValue(*pACM_PSCoef->ps16Sat++);
			pBitCoef->stBit[i+476].bits_3 = 0;
			pBitCoef->stBit[i+476].bits_4 = VouBitValue(*pACM_PSCoef->ps16Hue++);
			pBitCoef->stBit[i+476].bits_5 = 0;

			pBitCoef->stBit[i+476].bits_6 = VouBitValue(*pACM_PSCoef->ps16Luma++);
			pBitCoef->stBit[i+476].bits_7 = 0;
			pBitCoef->stBit[i+476].bits_8 = VouBitValue(*pACM_PSCoef->ps16Sat++);
			pBitCoef->stBit[i+476].bits_9 = 0;
			pBitCoef->stBit[i+476].bits_10 = VouBitValue(*pACM_PSCoef->ps16Hue++);
			pBitCoef->stBit[i+476].bits_11 = 0;

			pBitCoef->stBit[i+476].bits_12 = VouBitValue(*pACM_PSCoef1->ps16Luma++);
			pBitCoef->stBit[i+476].bits_13 = 0;
			pBitCoef->stBit[i+476].bits_14 = VouBitValue(*pACM_PSCoef1->ps16Sat++);
			pBitCoef->stBit[i+476].bits_15 = 0;
			pBitCoef->stBit[i+476].bits_16 = VouBitValue(*pACM_PSCoef1->ps16Hue++);
			pBitCoef->stBit[i+476].bits_17 = 0;

			pBitCoef->stBit[i+476].bits_18 = VouBitValue(*pACM_PSCoef1->ps16Luma++);
			pBitCoef->stBit[i+476].bits_19 = 0;
			pBitCoef->stBit[i+476].bits_20 = VouBitValue(*pACM_PSCoef1->ps16Sat++);
			pBitCoef->stBit[i+476].bits_21 = 0;
			pBitCoef->stBit[i+476].bits_22 = VouBitValue(*pACM_PSCoef1->ps16Hue++);
			pBitCoef->stBit[i+476].bits_23 = 0;
		}
    }

    pACM_PSCoef->ps16Luma = &s_s16AcmLUTDeflt3_Luma[0];
    pACM_PSCoef->ps16Sat = &s_s16AcmLUTDeflt3_Sat[0];
    pACM_PSCoef->ps16Hue = &s_s16AcmLUTDeflt3_Hue[0];

	pACM_PSCoef1->ps16Luma = &s_s16AcmLUTDeflt7_Luma[0];
	pACM_PSCoef1->ps16Sat  = &s_s16AcmLUTDeflt7_Sat[0];
	pACM_PSCoef1->ps16Hue  = &s_s16AcmLUTDeflt7_Hue[0];

    for (i = 0; i < 180; i++)  //360/2=180
    {
		if(i>=168)  //336/2=168
        {
			pBitCoef->stBit[i+704].bits_0 = VouBitValue(*pACM_PSCoef->ps16Luma++);
			pBitCoef->stBit[i+704].bits_1 = 0;
			pBitCoef->stBit[i+704].bits_2 = VouBitValue(*pACM_PSCoef->ps16Sat++);
			pBitCoef->stBit[i+704].bits_3 = 0;
			pBitCoef->stBit[i+704].bits_4 = VouBitValue(*pACM_PSCoef->ps16Hue++);
			pBitCoef->stBit[i+704].bits_5 = 0;

			pBitCoef->stBit[i+704].bits_6 = VouBitValue(*pACM_PSCoef->ps16Luma++);
			pBitCoef->stBit[i+704].bits_7 = 0;
			pBitCoef->stBit[i+704].bits_8 = VouBitValue(*pACM_PSCoef->ps16Sat++);
			pBitCoef->stBit[i+704].bits_9 = 0;
			pBitCoef->stBit[i+704].bits_10 = VouBitValue(*pACM_PSCoef->ps16Hue++);
			pBitCoef->stBit[i+704].bits_11 = 0;

			pBitCoef->stBit[i+704].bits_12 = 0;
			pBitCoef->stBit[i+704].bits_13 = 0;
			pBitCoef->stBit[i+704].bits_14 = 0;
			pBitCoef->stBit[i+704].bits_15 = 0;
			pBitCoef->stBit[i+704].bits_16 = 0;
			pBitCoef->stBit[i+704].bits_17 = 0;

			pBitCoef->stBit[i+704].bits_18 = 0;
			pBitCoef->stBit[i+704].bits_19 = 0;
			pBitCoef->stBit[i+704].bits_20 = 0;
			pBitCoef->stBit[i+704].bits_21 = 0;
			pBitCoef->stBit[i+704].bits_22 = 0;
			pBitCoef->stBit[i+704].bits_23 = 0;
        }
		else
        {
			pBitCoef->stBit[i+704].bits_0 = VouBitValue(*pACM_PSCoef->ps16Luma++);
			pBitCoef->stBit[i+704].bits_1 = 0;
			pBitCoef->stBit[i+704].bits_2 = VouBitValue(*pACM_PSCoef->ps16Sat++);
			pBitCoef->stBit[i+704].bits_3 = 0;
			pBitCoef->stBit[i+704].bits_4 = VouBitValue(*pACM_PSCoef->ps16Hue++);
			pBitCoef->stBit[i+704].bits_5 = 0;

			pBitCoef->stBit[i+704].bits_6 = VouBitValue(*pACM_PSCoef->ps16Luma++);
			pBitCoef->stBit[i+704].bits_7 = 0;
			pBitCoef->stBit[i+704].bits_8 = VouBitValue(*pACM_PSCoef->ps16Sat++);
			pBitCoef->stBit[i+704].bits_9 = 0;
			pBitCoef->stBit[i+704].bits_10 = VouBitValue(*pACM_PSCoef->ps16Hue++);
			pBitCoef->stBit[i+704].bits_11 = 0;

			pBitCoef->stBit[i+704].bits_12 = VouBitValue(*pACM_PSCoef1->ps16Luma++);
			pBitCoef->stBit[i+704].bits_13 = 0;
			pBitCoef->stBit[i+704].bits_14 = VouBitValue(*pACM_PSCoef1->ps16Sat++);
			pBitCoef->stBit[i+704].bits_15 = 0;
			pBitCoef->stBit[i+704].bits_16 = VouBitValue(*pACM_PSCoef1->ps16Hue++);
			pBitCoef->stBit[i+704].bits_17 = 0;

			pBitCoef->stBit[i+704].bits_18 = VouBitValue(*pACM_PSCoef1->ps16Luma++);
			pBitCoef->stBit[i+704].bits_19 = 0;
			pBitCoef->stBit[i+704].bits_20 = VouBitValue(*pACM_PSCoef1->ps16Sat++);
			pBitCoef->stBit[i+704].bits_21 = 0;
			pBitCoef->stBit[i+704].bits_22 = VouBitValue(*pACM_PSCoef1->ps16Hue++);
			pBitCoef->stBit[i+704].bits_23 = 0;
		}
    }

    pBitCoef->u32Size = COEFACMCNT * sizeof(ACM_COEF_BIT_S); //884
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
HI_VOID ACM_DRV_TransCoef(HI_VOID *pCoefMem, ISP_ACM_LUT_S *pstAcmLut)
{
    HI_S16* ps16AcmCoef = HI_NULL;
    osal_memset(&stArray,0,sizeof(stArray));
    stArray.u32Size = 884*16;

    ps16AcmCoef = (HI_S16 *)pstAcmLut;

    HI_ASSERT(ps16AcmCoef != HI_NULL);
    HI_ASSERT(pCoefMem != HI_NULL);

    ACM_DRV_TransCoefAlign(ps16AcmCoef, &stArray); 				//transform the table to struct
    osal_memcpy((HI_U8*)pCoefMem, stArray.stBit, stArray.u32Size); 	//osal_memcpy copy to suppositional address
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
HI_S32 ACM_DRV_UpdateAttr(ISP_DEV IspDev, HI_U32 u32AcmCoefAddr)
{
    ISP_ACM_CTX_S* pstAcmCtx = &g_stAcmCtx;
    //unsigned long u32Flags;

    HI_ASSERT(u32AcmCoefAddr != 0);

    // update ACM image size

    if(osal_down(&pstAcmCtx->stAcmSem))
    {
        return -ERESTARTSYS;
    }

	hi_isp_acm_enable_write(IspDev, (HI_BOOL)pstAcmCtx->stAcmCtrl.bEnable);
    hi_isp_acm_dbg_enable_write(IspDev, (HI_BOOL)pstAcmCtx->stAcmCtrl.bDemoEnable);
    hi_isp_acm_luma_gain_write(IspDev, pstAcmCtx->stAcmCtrl.u32GainLuma);
    hi_isp_acm_sat_gain_write(IspDev, pstAcmCtx->stAcmCtrl.u32GainSat);
    hi_isp_acm_hue_gain_write(IspDev, pstAcmCtx->stAcmCtrl.u32GainHue);
    hi_isp_acm_cbcrthr_write(IspDev, pstAcmCtx->stAcmCtrl.u32CbcrThr);
    hi_isp_acm_cliprange_write(IspDev, pstAcmCtx->stAcmCtrl.u32ClipRange);
    hi_isp_acm_stretch_write(IspDev, pstAcmCtx->stAcmCtrl.u32Stretch);
    hi_isp_acm_cliporwrap_write(IspDev, pstAcmCtx->stAcmCtrl.u32AcmClipOrWrap);

    //hi_isp_acm_coeff_addr_write(IspDev, u32AcmCoefAddr);
    /* In chip power up, there maybe a wrong interrput!!! */
    //hi_isp_acm_coeff_up_write(IspDev, 1);

    osal_up(&pstAcmCtx->stAcmSem);
    return HI_SUCCESS;
}
HI_S32 ACM_DRV_UpdateRegister(ISP_DEV IspDev, HI_U32 u32AcmCoefAddr)
{
    ISP_ACM_CTX_S* pstAcmCtx = &g_stAcmCtx;
    //unsigned long u32Flags;

    HI_ASSERT(u32AcmCoefAddr != 0);

    // update ACM image size
    if(osal_down(&pstAcmCtx->stAcmSem))
    {
       return -ERESTARTSYS;
    }

	hi_isp_acm_enable_write(IspDev, (HI_BOOL)pstAcmCtx->stAcmCtrl.bEnable);
    hi_isp_acm_dbg_enable_write(IspDev, (HI_BOOL)pstAcmCtx->stAcmCtrl.bDemoEnable);
    hi_isp_acm_luma_gain_write(IspDev, pstAcmCtx->stAcmCtrl.u32GainLuma);
    hi_isp_acm_sat_gain_write(IspDev, pstAcmCtx->stAcmCtrl.u32GainSat);
    hi_isp_acm_hue_gain_write(IspDev, pstAcmCtx->stAcmCtrl.u32GainHue);
    hi_isp_acm_cbcrthr_write(IspDev, pstAcmCtx->stAcmCtrl.u32CbcrThr);
    hi_isp_acm_cliprange_write(IspDev, pstAcmCtx->stAcmCtrl.u32ClipRange);
    hi_isp_acm_stretch_write(IspDev, pstAcmCtx->stAcmCtrl.u32Stretch);
    hi_isp_acm_cliporwrap_write(IspDev, pstAcmCtx->stAcmCtrl.u32AcmClipOrWrap);

    hi_isp_acm_coeff_addr_write(IspDev, u32AcmCoefAddr);
    /* In chip power up, there maybe a wrong interrput!!! */
    hi_isp_acm_coeff_up_write(IspDev, 1);

    osal_up(&pstAcmCtx->stAcmSem);
    return HI_SUCCESS;
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
HI_S32 ACM_DRV_InitDefault(ISP_ACM_CTX_S* pstAcmCtx)
{
	ISP_DEV IspDev = 0;
    HI_S32 s32Ret = HI_SUCCESS;
	HI_S32 s32AcmCoefSize;

    if(osal_down(&pstAcmCtx->stAcmSem))
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

    ACM_DRV_TransCoef((HI_VOID*)pstAcmCtx->stCoefBuff.pVirAddr, pstAcmCtx->pstCoefLut);

    osal_up(&pstAcmCtx->stAcmSem);

    s32Ret = ACM_DRV_UpdateRegister(IspDev, pstAcmCtx->stCoefBuff.u32PhyAddr);
    if(s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

    return s32Ret;
}



extern HI_S32 ISP_GetFrameEdge(ISP_DEV IspDev, HI_U32 *pu32Status);
static HI_S32 ISP_ACM_DRV_FrameEdgePend(HI_U32 u32FrameCnt)
{
	HI_U32 u32Status;

	if (!u32FrameCnt)
	{
		return HI_SUCCESS;
	}

	while (u32FrameCnt--)
	{
		ISP_GetFrameEdge(0, &u32Status);
	}

	return HI_SUCCESS;
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



HI_S32 ISP_ACM_DRV_SetAttr(ISP_ACM_ATTR_S *pstAcmAttr)
{
	ISP_DEV IspDev = 0;
    ISP_ACM_CTX_S* pstAcmCtx = &g_stAcmCtx;
    HI_S32 s32Ret;

    ACM_CHECK_NULL(pstAcmAttr);
    ACM_CHECK_INIT();

    if(osal_down(&pstAcmCtx->stAcmSem))
    {
       return -ERESTARTSYS;
    }
    osal_memcpy((HI_VOID*)&pstAcmCtx->stAcmCtrl, (HI_VOID*)pstAcmAttr, sizeof(ISP_ACM_ATTR_S));

    osal_up(&pstAcmCtx->stAcmSem);

	//bAcmEnable = pstAcmCtx->stAcmCtrl.bEnable;
	//pstAcmCtx->stAcmCtrl.bEnable = HI_FALSE;
	s32Ret = ACM_DRV_UpdateAttr(IspDev, pstAcmCtx->stCoefBuff.u32PhyAddr);
	if(s32Ret != HI_SUCCESS)
	{
	    return s32Ret;
	}
	//ISP_ACM_DRV_FrameEdgePend(2);
	//pstAcmCtx->stAcmCtrl.bEnable = bAcmEnable;
    //ACM_DRV_UpdateRegister(IspDev, pstAcmCtx->stCoefBuff.u32PhyAddr);

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
HI_S32 ISP_ACM_DRV_GetAttr(ISP_ACM_ATTR_S *pstAcmAttr)
{
    ISP_ACM_CTX_S* pstAcmCtx = &g_stAcmCtx;

    ACM_CHECK_NULL(pstAcmAttr);
    ACM_CHECK_INIT();

    if(osal_down(&pstAcmCtx->stAcmSem))
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
HI_S32 ISP_ACM_DRV_SetCoeff(HI_VOID)
{
	ISP_DEV IspDev = 0;
    ISP_ACM_CTX_S* pstAcmCtx = &g_stAcmCtx;
	HI_BOOL bAcmEnable;
	HI_S32 s32Ret;

    ACM_CHECK_INIT();

    if(osal_down(&pstAcmCtx->stAcmSem))
    {
       return -ERESTARTSYS;
    }

	ACM_DRV_TransCoef((HI_VOID*)pstAcmCtx->stCoefBuff.pVirAddr, pstAcmCtx->pstCoefLut);
    osal_up(&pstAcmCtx->stAcmSem);

	bAcmEnable = pstAcmCtx->stAcmCtrl.bEnable;
	pstAcmCtx->stAcmCtrl.bEnable = HI_FALSE;
	s32Ret = ACM_DRV_UpdateAttr(IspDev, pstAcmCtx->stCoefBuff.u32PhyAddr);
	if(s32Ret != HI_SUCCESS)
	{
	    return s32Ret;
	}
	ISP_ACM_DRV_FrameEdgePend(1);
	pstAcmCtx->stAcmCtrl.bEnable = bAcmEnable;
    s32Ret = ACM_DRV_UpdateRegister(IspDev, pstAcmCtx->stCoefBuff.u32PhyAddr);
    if(s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }
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
    unsigned int   *argp = (unsigned int   *)arg;
    //SP_DEV IspDev;
    HI_S32 ret;

    ACM_CHECK_INIT();
    //IspDev = ISP_GET_DEV(private_data);

    switch (cmd)
    {
        case ISP_ACM_ATTR_GET:
        {
            ISP_ACM_ATTR_S       stAcmAttr;

            ISP_ACM_DRV_GetAttr(&stAcmAttr);

            osal_memcpy(argp, &stAcmAttr, sizeof(ISP_ACM_ATTR_S));
            return 0;
        }
        case ISP_ACM_ATTR_SET:
        {
            ISP_ACM_DRV_SetAttr((ISP_ACM_ATTR_S *)argp);

            return 0;
        }
        case ISP_ACM_LUT_PHY_ADDR_GET:
        {

            //osal_memcpy(argp, &g_stAcmCtx.stMmzBuff, sizeof(ISP_ACM_BUFFER_S));

            ret = osal_copy_to_user(argp, &g_stAcmCtx.stMmzBuff, sizeof(ISP_ACM_BUFFER_S));
            if (ret)
    		{
    	        return HI_FAILURE;
    		}

            return 0;
        }
        case ISP_ACM_COEF_SET:
        {
            ISP_ACM_DRV_SetCoeff();
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
HI_S32 ISP_ACM_DRV_Init(HI_VOID)
{
    HI_U32 u32PhyAddr;
    HI_U8  *pu8VirAddr;
    HI_U32 u32CoefPhyAddr;
    HI_U8  *pu8CoefVirAddr;
    HI_U32 u32Size;
    ISP_ACM_CTX_S* pstAcmCtx = &g_stAcmCtx;
    HI_S32 s32Ret = HI_SUCCESS;

    if(HI_TRUE == g_bInited)
    {
        ISP_TRACE(HI_DBG_ERR, "err acm reinitialized\n");
        return HI_FAILURE;
    }

    osal_spin_lock_init(&g_stAcmLock);
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

    pstAcmCtx->stMmzBuff.pVirAddr   = pu8VirAddr;
    pstAcmCtx->stMmzBuff.u32PhyAddr = u32PhyAddr;
    pstAcmCtx->stMmzBuff.u32AcmSize = ADDROFFSET;

    pstAcmCtx->stCoefBuff.u32PhyAddr= u32CoefPhyAddr;
    pstAcmCtx->stCoefBuff.pVirAddr  = pu8CoefVirAddr;
    pstAcmCtx->stCoefBuff.u32AcmSize    = ACMADDROFFSET;

    pstAcmCtx->pstCoefLut = (ISP_ACM_LUT_S*)pu8VirAddr;

    ACM_DRV_InitDefault(pstAcmCtx);

    g_bInited = HI_TRUE;

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
HI_S32 ISP_ACM_DRV_Exit(HI_VOID)
{
	ISP_DEV IspDev = 0;
    ISP_ACM_CTX_S* pstAcmCtx = &g_stAcmCtx;
    //unsigned long u32Flags;

    HI_U32 u32PhyAddr;
    HI_U8* pu8VirAddr;
    HI_U32 u32CoefPhyAddr;
    HI_U8* pu8CoefVirAddr;

    ACM_CHECK_INIT();
    g_bInited = HI_FALSE;

    u32PhyAddr = pstAcmCtx->stMmzBuff.u32PhyAddr;
    pu8VirAddr = (HI_U8 *)pstAcmCtx->stMmzBuff.pVirAddr;
    u32CoefPhyAddr = pstAcmCtx->stCoefBuff.u32PhyAddr;
    pu8CoefVirAddr = (HI_U8 *)pstAcmCtx->stCoefBuff.pVirAddr;

    if(osal_down(&pstAcmCtx->stAcmSem))
    {
       return -ERESTARTSYS;
    }

    hi_isp_acm_coeff_addr_write(IspDev, 0);

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
    osal_spin_lock_destory(&g_stAcmLock);

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

