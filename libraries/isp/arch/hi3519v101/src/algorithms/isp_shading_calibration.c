/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_shading.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/04/24
  Description   : 
  History       :
  1.Date        : 2013/04/24
    Author      : n00168968
    Modification: Created file

******************************************************************************/

#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_ext_config.h"
#include "isp_config.h"
#include "isp_proc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#ifndef MAX2
#define MAX2(a, b)          ((a) > (b) ? (a) : (b))
#endif

#ifndef CLIP3
#define CLIP3(x,min,max)    ((x)< (min) ? (min) : ((x)>(max)?(max):(x)))
#endif

#define MAX(a, b) (((a) < (b)) ?  (b) : (a))
#define MIN(a, b) (((a) > (b)) ?  (b) : (a))

#ifndef MAX3
#define MAX3(x, y, z) (MAX((x), MAX((y), (z))))
#endif

#define GRIDSIZEX_MIN               (5)
#define GRIDSIZEX_MAX               (65)
#define GRIDSIZEY_MIN               (3)
#define GRIDSIZEY_MAX               (65)
#define WINDOWSIZE_X                (20)
#define WINDOWSIZE_Y                (20)
#define MESH_SCALE_MAX              (7)
#define LSC_MAX_PIXEL_VALUE         ((1<<20) - 1)
#define LSC_GRAD_Q_VALUE            (32768)
#define MESH_SHADING_PLANE_NUM      (3)
#define MESH_SHADING_TABLE_NODE_NUMBER_MAX (64*64)
#define HIST_ARRAY_TOTAL  (256)
#define MESH_SHADING_ENLARGE_BIT (20)
#define ALLOCATE_ARRAY_MIN_MAX   (2)
#define ELIMINATE_TOTAL_PERCENT  (100)

/*******************  global variables *************************/
HI_U32 *g_u32GridWidthStep = HI_NULL;
HI_U32 *g_u32GridHeightStep = HI_NULL;
static const HI_FLOAT g_afMaxGainArray[8] = 
{
	(HI_FLOAT)255/(HI_FLOAT)128,	//1.99
	(HI_FLOAT)255/(HI_FLOAT)64,		//2.8
	(HI_FLOAT)255/(HI_FLOAT)32,		//3.7
	(HI_FLOAT)255/(HI_FLOAT)16,		//4.6
	(HI_FLOAT)255/(HI_FLOAT)256 + 1,  //0.10
	(HI_FLOAT)255/(HI_FLOAT)128 + 1,	//1.9
	(HI_FLOAT)255/(HI_FLOAT)64 + 1,	//2.8
	(HI_FLOAT)255/(HI_FLOAT)32 + 1,	//3.7
};

typedef struct hi_Mesh_Shading_Grid_Table_S
{
 /* 
    HI_U16 pu16Rgain[GridSizeX*GridSizeY]
    HI_U16 pu16Ggain[GridSizeX*GridSizeY]
    HI_U16 pu16Bgain[GridSizeX*GridSizeY]
 */
	HI_U16 *pu16Rgain;    
	HI_U16 *pu16Ggain;
	HI_U16 *pu16Bgain;
}ISP_Mesh_Shading_Grid_Table_S;

typedef struct hi_Mesh_Shading_BayerChannel_Grid_Table_S
{
 /*
    HI_U32 pu32Rgain[GridSizeX*GridSizeY];//13bit	3.10     
	HI_U32 pu32Grgain[GridSizeX*GridSizeY];//13bit	3.10
	HI_U32 pu32Gbgain[GridSizeX*GridSizeY];//13bit	3.10
	HI_U32 pu32Bgain[GridSizeX*GridSizeY];//13bit	3.10
*/
    HI_U32 *pu32Rgain;
    HI_U32 *pu32Grgain;
    HI_U32 *pu32Gbgain;
    HI_U32 *pu32Bgain;
}ISP_Mesh_Shading_BayerChannel_Table_S;

typedef struct hi_Mesh_Shading_BayerChannel_Grid_Data_S
{
 /* 
   HI_U32 pu32R_data[GridSizeX*GridSizeY];
   HI_U32 pu32Gr_data[GridSizeX*GridSizeY];
   HI_U32 pu32Gb_data[GridSizeX*GridSizeY];
   HI_U32 pu32B_data[GridSizeX*GridSizeY];      
*/
   HI_U32 *pu32R_data;
   HI_U32 *pu32Gr_data;
   HI_U32 *pu32Gb_data;
   HI_U32 *pu32B_data;
}ISP_Mesh_Shading_BayerChannel_Grid_Data_S;

HI_U32 Get_Max_Data(HI_U32* pu32data, HI_U32 u32length)
{
	HI_U32 i = 0;
	HI_U32 maxData = 0;	
	for (i = 0; i < u32length; i++)
	{
		if (pu32data[i] > maxData)
	    {
			maxData = pu32data[i];
	    }
	}
	return maxData;
}

HI_U32 Get_Min_Data(HI_U32* pu32data, HI_U32 u32length)
{
    HI_U32 i  = 0;
	HI_U32 minData = LSC_MAX_PIXEL_VALUE;	
	for (i = 0; i < u32length; i++)
	{
		if (pu32data[i] < minData)
		{
			minData = pu32data[i];
		}
	}
	return minData;
}

HI_S32 Get_Statistics_Array(HI_U32 *pu32data,HI_U32 *pu32Hist,HI_U32 au32EliMaxMin[2],HI_U32 u32length,HI_U8 u8EliPer,HI_U8 u8Bitdiff)
{
    if(pu32data == HI_NULL || pu32Hist == HI_NULL)
    {
       printf("%s: LINE: %d pu32data/pu32Hist is NULL point!\n",__FUNCTION__,__LINE__);
       return HI_FAILURE;   
    }
    
    HI_S32 i = 0;
    HI_U32 u32sumMin = 0;
    HI_U32 u32sumMax = 0;
    HI_U32 u32EliTotal = 0;

    u32EliTotal = u32length*u8EliPer/ELIMINATE_TOTAL_PERCENT;
    memset(pu32Hist ,0,sizeof(HI_U32)*HIST_ARRAY_TOTAL);

    //Get 256 bin the statistic of pu32data
    for(i = 0;i < u32length;i++)
    {
        pu32Hist[*(pu32data + i) >> u8Bitdiff]++;
    }

    //Allocate the Min location of Eliminate dark point in statistics
    u32sumMin = pu32Hist[0];
    for(i = 1;i < HIST_ARRAY_TOTAL;i++)
    {
        u32sumMin += pu32Hist[i];
        if(u32sumMin > u32EliTotal)
        {
            au32EliMaxMin[0] = (i-1) << u8Bitdiff;
            break;
        }
    }

    //Allocate the Max location of Eliminate white point in statistics
    u32sumMax = pu32Hist[HIST_ARRAY_TOTAL -1];
    for(i = HIST_ARRAY_TOTAL -2;i >= 0;i--)
    {
        u32sumMax += pu32Hist[i];
        if(u32sumMax > u32EliTotal)
        {
            au32EliMaxMin[1] = (i+1) << u8Bitdiff;
            break;
        }
    }
    return HI_SUCCESS;
}

HI_S32 Mesh_Shading_Scale_Test(ISP_Mesh_Shading_BayerChannel_Grid_Data_S *pls_data,HI_U32 u32LSCGridPoints,HI_U8 u8MeshScale)
{
    if(pls_data == HI_NULL)
    {
       printf("%s: LINE: %d pls_data is NULL point!\n",__FUNCTION__,__LINE__);
       return HI_FAILURE; 
    }

    if(pls_data->pu32R_data== HI_NULL || pls_data->pu32Gb_data == HI_NULL || pls_data->pu32Gr_data == HI_NULL
        || pls_data->pu32B_data == HI_NULL)
    {
        printf("%s: LINE: %d pls_data of each channel with NULL point\n",__FUNCTION__,__LINE__);
        return HI_FAILURE; 
    }
    
	HI_U32 b_max_data  = 0;
    HI_U32 gb_max_data = 0;
    HI_U32 gr_max_data = 0;
    HI_U32 r_max_data  = 0;
	HI_U32 b_min_data  = 0;
    HI_U32 gb_min_data = 0;
    HI_U32 gr_min_data = 0;
    HI_U32 r_min_data  = 0;
	HI_FLOAT b_max_gain  = 0.0;
    HI_FLOAT gb_max_gain = 0.0;
    HI_FLOAT gr_max_gain = 0.0;
    HI_FLOAT r_max_gain  = 0.0;
	HI_FLOAT max_gain = 0.0;

	//Get Max value of B/Gb/Gr/R channel
	b_max_data  = Get_Max_Data(pls_data->pu32B_data, u32LSCGridPoints);
	gb_max_data = Get_Max_Data(pls_data->pu32Gb_data, u32LSCGridPoints);
	gr_max_data = Get_Max_Data(pls_data->pu32Gr_data, u32LSCGridPoints);
	r_max_data  = Get_Max_Data(pls_data->pu32R_data, u32LSCGridPoints);

	//Get Min value of B/Gb/Gr/R channel
	b_min_data  = Get_Min_Data(pls_data->pu32B_data, u32LSCGridPoints);
	gb_min_data = Get_Min_Data(pls_data->pu32Gb_data, u32LSCGridPoints);
	gr_min_data = Get_Min_Data(pls_data->pu32Gr_data, u32LSCGridPoints);
	r_min_data  = Get_Min_Data(pls_data->pu32R_data, u32LSCGridPoints);

	b_max_gain  = (HI_FLOAT)b_max_data  / (HI_FLOAT)DIV_0_TO_1(b_min_data);
	gb_max_gain = (HI_FLOAT)gb_max_data / (HI_FLOAT)DIV_0_TO_1(gb_min_data);
	gr_max_gain = (HI_FLOAT)gr_max_data / (HI_FLOAT)DIV_0_TO_1(gr_min_data);
	r_max_gain  = (HI_FLOAT)r_max_data  / (HI_FLOAT)DIV_0_TO_1(r_min_data);

	max_gain = MAX(MAX3(b_max_gain,gb_max_gain,gr_max_gain),r_max_gain);

	printf("b_max_gain = %6f gb_max_gain =%6f gr_max_gain=%6f r_max_gain=%6f\n",b_max_gain,
        gb_max_gain,gr_max_gain,r_max_gain);

    printf("max_gain=%6f g_afMaxGainArray[%d] = %6f \n",max_gain, u8MeshScale ,g_afMaxGainArray[u8MeshScale] );
	if (max_gain > g_afMaxGainArray[u8MeshScale])
	{
		printf("WARNING:\n");
		printf("Max gain = %f\n",max_gain);

		if (u8MeshScale < 4)  //min gain's value is 0
		{
			if((max_gain > g_afMaxGainArray[0]) && (max_gain <= g_afMaxGainArray[1]))  //(2,4]
			{
				printf("Please set Mesh scale to %d\n",1);
			}
			else if((max_gain > g_afMaxGainArray[1]) && (max_gain <= g_afMaxGainArray[2]))//(4,8]
			{
				printf("Please set Mesh scale to %d\n",2);
			}
			else if((max_gain > g_afMaxGainArray[2]) && (max_gain <= g_afMaxGainArray[3]))//(8,16]
			{
				printf("Please set Mesh scale to %d\n",3);
			}
			else//  >16
			{
				printf("Max gain is too large to find accurate mesh scale,please set Mesh Scale to %d to reduce loss\n",3);
			}
		}
		else   //max gain's value is 1
		{
			if((max_gain > g_afMaxGainArray[4]) && (max_gain <= g_afMaxGainArray[5]))  //(2,3]
			{
				printf("Please set Mesh scale to %d\n",5);
			}
			else if((max_gain > g_afMaxGainArray[5]) && (max_gain <= g_afMaxGainArray[6]))//(3,5]
			{
				printf("Please set Mesh scale to %d\n",6);
			}
			else if((max_gain > g_afMaxGainArray[6]) && (max_gain <= g_afMaxGainArray[7]))//(5,9]
			{
				printf("Please set Mesh scale to %d\n",7);
			}
			else if((max_gain > g_afMaxGainArray[7]) && (max_gain <= g_afMaxGainArray[3]))//(9,16]
			{
				printf("Please set Mesh scale to %d\n",3);
			}
			else  //>16
			{
				printf("Max gain is too large to find accurate mesh scale,please set Mesh Scale to %d to reduce loss\n",3);
			}
		}
	}
	return HI_SUCCESS;
}

HI_S32 Get_Lens_Shading_Data(HI_U16* pBayerImg,ISP_Mesh_Shading_BayerChannel_Grid_Data_S * pls_data, ISP_LSC_CALIBRATION_CFG_S * pstLSCCaliCfg)
{
    if(pBayerImg == HI_NULL || pls_data == HI_NULL || pstLSCCaliCfg == HI_NULL)
    {
        printf("%s: LINE: %d pBayerImg/pls_data/pstLSCCaliCfg is NULL point !\n",__FUNCTION__,__LINE__);
        return HI_FAILURE;   
    }

    if(pls_data->pu32R_data == HI_NULL || pls_data->pu32Gb_data == HI_NULL || pls_data->pu32Gr_data == HI_NULL
        || pls_data->pu32B_data == HI_NULL)
    {
        printf("%s: LINE: %d pls_data of each channel is NULL point !\n",__FUNCTION__,__LINE__);
        return HI_FAILURE;   
    }
    
    //Get parameters from stLSCCaliCfg
 	HI_S32 s32Height = (HI_S32)(pstLSCCaliCfg->u16ImageHeight >>1); 
	HI_S32 s32Width  = (HI_S32)(pstLSCCaliCfg->u16ImageWidth >>1);
    HI_U32 size_x    = WINDOWSIZE_X;
	HI_U32 size_y    = WINDOWSIZE_Y;
    HI_U32 grid_x_size  = (HI_U32)pstLSCCaliCfg->u8GridSizeX;
	HI_U32 grid_y_size  = (HI_U32)pstLSCCaliCfg->u8GridSizeY;
    HI_U32 u32EliminatePctLow = (HI_U32)pstLSCCaliCfg->u8EliminatePer;

    //Black Level
    HI_S32 BLCOffsetR  = 0;
	HI_S32 BLCOffsetGr = 0;
	HI_S32 BLCOffsetGb = 0;
	HI_S32 BLCOffsetB  = 0;
    
	HI_U32 i = 0;
    HI_U32 j = 0;

    HI_U32 location_x = 0;
    HI_U32 location_y = 0; 
    HI_S32 x = 0;
    HI_S32 y = 0;
    HI_S32 x_start = 0;
    HI_S32 x_end   = 0;
    HI_S32 y_start = 0;
    HI_S32 y_end   = 0;

    HI_U32 num = 0;
    HI_U32 count = 0;
    HI_U32 u32Index = 0;
    HI_U32 u32GridIndex = 0;
    HI_S32 s32Ret = HI_FAILURE;

    HI_U32 au32MaxMinR [ALLOCATE_ARRAY_MIN_MAX] = {0};
    HI_U32 au32MaxMinGr[ALLOCATE_ARRAY_MIN_MAX] = {0};
    HI_U32 au32MaxMinGb[ALLOCATE_ARRAY_MIN_MAX] = {0};
    HI_U32 au32MaxMinB [ALLOCATE_ARRAY_MIN_MAX] = {0};

    HI_U32 u32CountR  = 0;
    HI_U32 u32CountGr = 0;
    HI_U32 u32CountGb = 0;
    HI_U32 u32CountB  = 0;
	HI_U32 u32SumR  = 0;
    HI_U32 u32SumGr = 0;
    HI_U32 u32SumGb = 0;
    HI_U32 u32SumB  = 0;
 
    ISP_BAYER_RAW_E enBit = MESH_SHADING_ENLARGE_BIT - pstLSCCaliCfg->enRawBit;
    HI_U32 fullWidth = s32Width << 1;
    HI_S32 s32Bitdiff = pstLSCCaliCfg->enRawBit - 12;
    HI_U8  u8HistBitdif = pstLSCCaliCfg->enRawBit - 8;

    //Malloc memory of pu32DataR/Gr/Gb/B
	HI_U32 *pu32DataR	= (HI_U32 *)malloc(sizeof(HI_U32)*size_x*size_y);
    if(pu32DataR == HI_NULL)
    {
        printf("%s: LINE: %d pu32DataR malloc failure !\n",__FUNCTION__,__LINE__);
        return HI_FAILURE;
    }
    
	HI_U32 *pu32DataGr	= (HI_U32 *)malloc(sizeof(HI_U32)*size_x*size_y);
    if(pu32DataGr == HI_NULL)
    {
        free(pu32DataR);
        pu32DataR = HI_NULL;
        printf("%s: LINE: %d pu32DataGr malloc failure !\n",__FUNCTION__,__LINE__);
        return HI_FAILURE;
    }
    
	HI_U32 *pu32DataGb	= (HI_U32 *)malloc(sizeof(HI_U32)*size_x*size_y);
    if(pu32DataGb == HI_NULL)
    {
        free(pu32DataR);
        free(pu32DataGr);
        
        pu32DataR = HI_NULL;
        pu32DataGr = HI_NULL;
        printf("%s: LINE: %d pu32DataGb malloc failure !\n",__FUNCTION__,__LINE__);
        return HI_FAILURE;
    }
    
	HI_U32 *pu32DataB	= (HI_U32 *)malloc(sizeof(HI_U32)*size_x*size_y);
    if(pu32DataB == HI_NULL)
    {
        free(pu32DataR);
        free(pu32DataGr);
        free(pu32DataGb);
        
        pu32DataR = HI_NULL;
        pu32DataGr = HI_NULL;
        pu32DataGb = HI_NULL;
        printf("%s: LINE: %d pu32DataB malloc failure !\n",__FUNCTION__,__LINE__);
        return HI_FAILURE;
    }

    memset(pu32DataR, 0,sizeof(HI_U32)*size_x*size_y);
    memset(pu32DataGr,0,sizeof(HI_U32)*size_x*size_y);
    memset(pu32DataGb,0,sizeof(HI_U32)*size_x*size_y);
    memset(pu32DataB, 0,sizeof(HI_U32)*size_x*size_y);

    //Malloc memory of pu32HistArrayR/Gr/Gb/B
    HI_U32 *pu32HistArrayR  = (HI_U32 *)malloc(sizeof(HI_U32)*HIST_ARRAY_TOTAL);
    if(pu32HistArrayR == HI_NULL)
    {
        //free memory of pu32Data
        free(pu32DataR);
        free(pu32DataGr);
        free(pu32DataGb);
        free(pu32DataB);

        pu32DataR = HI_NULL;
        pu32DataGr = HI_NULL;
        pu32DataGb = HI_NULL;
        pu32DataB = HI_NULL;
        printf("%s: LINE: %d pu32HistArrayR malloc failure !\n",__FUNCTION__,__LINE__);
        return HI_FAILURE;
    }
    
    HI_U32 *pu32HistArrayGr = (HI_U32 *)malloc(sizeof(HI_U32)*HIST_ARRAY_TOTAL);
    if(pu32HistArrayGr == HI_NULL)
    {
        //free memory of pu32Data
        free(pu32DataR);
        free(pu32DataGr);
        free(pu32DataGb);
        free(pu32DataB);

        free(pu32HistArrayR);

        pu32DataR = HI_NULL;
        pu32DataGr = HI_NULL;
        pu32DataGb = HI_NULL;
        pu32DataB = HI_NULL;
        
        pu32HistArrayR = HI_NULL;
        printf("%s: LINE: %d pu32HistArrayGr malloc failure !\n",__FUNCTION__,__LINE__);
        return HI_FAILURE;
    }
    
    HI_U32 *pu32HistArrayGb = (HI_U32 *)malloc(sizeof(HI_U32)*HIST_ARRAY_TOTAL);
    if(pu32HistArrayGb == HI_NULL)
    {
        //free memory of pu32Data
        free(pu32DataR);
        free(pu32DataGr);
        free(pu32DataGb);
        free(pu32DataB);

        free(pu32HistArrayR);
        free(pu32HistArrayGr);

        pu32DataR = HI_NULL;
        pu32DataGr = HI_NULL;
        pu32DataGb = HI_NULL;
        pu32DataB = HI_NULL;
        
        pu32HistArrayR = HI_NULL;
        pu32HistArrayGr = HI_NULL;
        printf("%s: LINE: %d pu32HistArrayGb malloc failure !\n",__FUNCTION__,__LINE__);
        return HI_FAILURE;
    }
    
    HI_U32 *pu32HistArrayB  = (HI_U32 *)malloc(sizeof(HI_U32)*HIST_ARRAY_TOTAL);
    if(pu32HistArrayB == HI_NULL)
    {
        //free memory of pu32Data
        free(pu32DataR);
        free(pu32DataGr);
        free(pu32DataGb);
        free(pu32DataB);

        free(pu32HistArrayR);
        free(pu32HistArrayGr);
        free(pu32HistArrayGb);
        pu32DataR = HI_NULL;
        pu32DataGr = HI_NULL;
        pu32DataGb = HI_NULL;
        pu32DataB = HI_NULL;
        
        pu32HistArrayR = HI_NULL;
        pu32HistArrayGr = HI_NULL;
        pu32HistArrayGb = HI_NULL;
        printf("%s: LINE: %d pu32HistArrayB malloc failure !\n",__FUNCTION__,__LINE__);
        return HI_FAILURE;
    }

    location_x = 0;
    location_y = 0;

	if (pstLSCCaliCfg->bOffsetInEn)
	{
        if(s32Bitdiff >= 0)
        {
            BLCOffsetR  = (HI_S32)((HI_U32)(pstLSCCaliCfg->u16BLCOffsetR)  << s32Bitdiff);
    		BLCOffsetGr = (HI_S32)((HI_U32)(pstLSCCaliCfg->u16BLCOffsetGr) << s32Bitdiff);
    		BLCOffsetGb = (HI_S32)((HI_U32)(pstLSCCaliCfg->u16BLCOffsetGb) << s32Bitdiff);
    		BLCOffsetB  = (HI_S32)((HI_U32)(pstLSCCaliCfg->u16BLCOffsetB)  << s32Bitdiff);
        }
        else
        {
            BLCOffsetR  = (HI_S32)((HI_U32)(pstLSCCaliCfg->u16BLCOffsetR)  >> (-s32Bitdiff));
    		BLCOffsetGr = (HI_S32)((HI_U32)(pstLSCCaliCfg->u16BLCOffsetGr) >> (-s32Bitdiff));
    		BLCOffsetGb = (HI_S32)((HI_U32)(pstLSCCaliCfg->u16BLCOffsetGb) >> (-s32Bitdiff));
    		BLCOffsetB  = (HI_S32)((HI_U32)(pstLSCCaliCfg->u16BLCOffsetB)  >> (-s32Bitdiff));
        }
		
	}
	else
	{
		BLCOffsetR  = 0;
		BLCOffsetGr = 0;
		BLCOffsetGb = 0;
		BLCOffsetB  = 0;
	}

	for (i = 0; i < grid_y_size; i++)
	{   
		location_y = location_y + g_u32GridHeightStep[i]; 
	    location_x = 0;  
		for (j = 0; j < grid_x_size; j++)
		{
		    location_x = location_x + g_u32GridWidthStep[j];
			num = 0;

            x_start = location_x - size_x/2;	
			x_end   = location_x + size_x/2;
			y_start = location_y - size_y/2;
			y_end   = location_y + size_y/2;

			if (x_start < 0)
			{
				x_start = 0;
			}
			if (x_end > s32Width)
			{
				x_end = s32Width - 1;
			}
			if (y_start < 0)
			{
				y_start = 0;
			}
			if (y_end > s32Height)
			{
				y_end = s32Height - 1;
			}

			for (y = y_start; y < y_end; y++)
			{
				for (x = x_start; x < x_end; x++)
				{
                    u32Index = 2*x + 2*y*fullWidth;
                    switch (pstLSCCaliCfg->enBayer)
					{
                      case BAYER_RGGB:
  						pu32DataR [num] = (HI_U32)MAX(((HI_S32)(pBayerImg[u32Index])-BLCOffsetR),0);
  						pu32DataGr[num] = (HI_U32)MAX(((HI_S32)(pBayerImg[u32Index+1])-BLCOffsetGr),0);
  						pu32DataGb[num] = (HI_U32)MAX(((HI_S32)(pBayerImg[u32Index+fullWidth])-BLCOffsetGb),0);
  						pu32DataB [num] = (HI_U32)MAX(((HI_S32)(pBayerImg[u32Index+fullWidth+1])-BLCOffsetB),0);
  						break;
                        
					  case BAYER_GRBG:
						pu32DataGr[num]	= (HI_U32)MAX(((HI_S32)(pBayerImg[u32Index])-BLCOffsetGr),0);
						pu32DataR [num]	= (HI_U32)MAX(((HI_S32)(pBayerImg[u32Index+1])- BLCOffsetR ),0);
						pu32DataB [num]	= (HI_U32)MAX(((HI_S32)(pBayerImg[u32Index+fullWidth])	- BLCOffsetB),0);
						pu32DataGb[num]	= (HI_U32)MAX(((HI_S32)(pBayerImg[u32Index+fullWidth+1])	- BLCOffsetGb),0);
						break;
                        
					  case BAYER_GBRG:
						pu32DataGb[num]	= (HI_U32)MAX(((HI_S32)(pBayerImg[u32Index])- BLCOffsetGb),0);
						pu32DataB [num]	= (HI_U32)MAX(((HI_S32)(pBayerImg[u32Index+1])	- BLCOffsetB),0);
						pu32DataR [num]	= (HI_U32)MAX(((HI_S32)(pBayerImg[u32Index+fullWidth])	- BLCOffsetR),0);
						pu32DataGr[num]	= (HI_U32)MAX(((HI_S32)(pBayerImg[u32Index+fullWidth+1])- BLCOffsetGr),0);
						break;
                        
					  case BAYER_BGGR:
						pu32DataB [num]	= (HI_U32)MAX(((HI_S32)(pBayerImg[u32Index])- BLCOffsetB),0);
						pu32DataGb[num]	= (HI_U32)MAX(((HI_S32)(pBayerImg[u32Index+1])- BLCOffsetGb),0);
						pu32DataGr[num]	= (HI_U32)MAX(((HI_S32)(pBayerImg[u32Index+fullWidth])	- BLCOffsetGr),0);
						pu32DataR [num]	= (HI_U32)MAX(((HI_S32)(pBayerImg[u32Index+fullWidth+1])- BLCOffsetR),0);
						break;
                        
					 default:
						break;
                    }
                    num++;
				}
			}

            //allocate the white point and dark point in the HistArray of Each Channel
            s32Ret = Get_Statistics_Array(pu32DataR,pu32HistArrayR,au32MaxMinR,num,u32EliminatePctLow,u8HistBitdif);
            if(s32Ret != HI_SUCCESS)
            {
               //free memory of pu32Data
               free(pu32DataR);
               free(pu32DataGr);
               free(pu32DataGb);
               free(pu32DataB);

               //free memory of pu32HistArray
               free(pu32HistArrayR);
               free(pu32HistArrayGr);
               free(pu32HistArrayGb); 
               free(pu32HistArrayB); 
               
               pu32DataR = HI_NULL;
               pu32DataGr = HI_NULL;
               pu32DataGb = HI_NULL;
               pu32DataB = HI_NULL;
               
               pu32HistArrayR = HI_NULL;
               pu32HistArrayGr = HI_NULL;
               pu32HistArrayGb = HI_NULL;
               pu32HistArrayB = HI_NULL;
               printf("%s: LINE: %d Get_Statistics_Array of R Channel failure !\n",__FUNCTION__,__LINE__);
               return HI_FAILURE; 
            }
            
            s32Ret = Get_Statistics_Array(pu32DataGr,pu32HistArrayGr,au32MaxMinGr,num,u32EliminatePctLow,u8HistBitdif);
            if(s32Ret != HI_SUCCESS)
            {
               //free memory of pu32Data
               free(pu32DataR);
               free(pu32DataGr);
               free(pu32DataGb);
               free(pu32DataB);

               //free memory of pu32HistArray
               free(pu32HistArrayR);
               free(pu32HistArrayGr);
               free(pu32HistArrayGb); 
               free(pu32HistArrayB); 

               pu32DataR = HI_NULL;
               pu32DataGr = HI_NULL;
               pu32DataGb = HI_NULL;
               pu32DataB = HI_NULL;
               
               pu32HistArrayR = HI_NULL;
               pu32HistArrayGr = HI_NULL;
               pu32HistArrayGb = HI_NULL;
               pu32HistArrayB = HI_NULL;
               
               printf("%s: LINE: %d Get_Statistics_Array of Gr Channel failure !\n",__FUNCTION__,__LINE__);
               return HI_FAILURE; 
            }
            
            s32Ret = Get_Statistics_Array(pu32DataGb,pu32HistArrayGb,au32MaxMinGb,num,u32EliminatePctLow,u8HistBitdif);
            if(s32Ret != HI_SUCCESS)
            {
               //free memory of pu32Data
               free(pu32DataR);
               free(pu32DataGr);
               free(pu32DataGb);
               free(pu32DataB);

               //free memory of pu32HistArray
               free(pu32HistArrayR);
               free(pu32HistArrayGr);
               free(pu32HistArrayGb); 
               free(pu32HistArrayB);  

               pu32DataR = HI_NULL;
               pu32DataGr = HI_NULL;
               pu32DataGb = HI_NULL;
               pu32DataB = HI_NULL;
               
               pu32HistArrayR = HI_NULL;
               pu32HistArrayGr = HI_NULL;
               pu32HistArrayGb = HI_NULL;
               pu32HistArrayB = HI_NULL;
               
               printf("%s: LINE: %d Get_Statistics_Array of Gb Channel failure !\n",__FUNCTION__,__LINE__);
               return HI_FAILURE; 
            }
            
            s32Ret = Get_Statistics_Array(pu32DataB , pu32HistArrayB,au32MaxMinB,num,u32EliminatePctLow,u8HistBitdif);
            if(s32Ret != HI_SUCCESS)
            {
               //free memory of pu32Data
               free(pu32DataR);
               free(pu32DataGr);
               free(pu32DataGb);
               free(pu32DataB);

               //free memory of pu32HistArray
               free(pu32HistArrayR);
               free(pu32HistArrayGr);
               free(pu32HistArrayGb); 
               free(pu32HistArrayB); 

               pu32DataR = HI_NULL;
               pu32DataGr = HI_NULL;
               pu32DataGb = HI_NULL;
               pu32DataB = HI_NULL;
               
               pu32HistArrayR = HI_NULL;
               pu32HistArrayGr = HI_NULL;
               pu32HistArrayGb = HI_NULL;
               pu32HistArrayB = HI_NULL;
               
               printf("%s: LINE: %d Get_Statistics_Array of B Channel failure !\n",__FUNCTION__,__LINE__);
               return HI_FAILURE; 
            }

			u32CountR = 0;
            u32CountGr= 0;
            u32CountGb= 0;
            u32CountB = 0;
			u32SumR	= 0;
			u32SumGr= 0;
			u32SumGb= 0;
			u32SumB	= 0;
			for (count = 0; count < num ; count++)
			{
                if(pu32DataR[count] >= au32MaxMinR[0] && pu32DataR[count] <= au32MaxMinR[1])
                {
                   u32SumR += pu32DataR[count];
                   u32CountR++;
                }

                if(pu32DataGr[count] >= au32MaxMinGr[0] && pu32DataGr[count] <= au32MaxMinGr[1])
                {
                   u32SumGr += pu32DataGr[count];
                   u32CountGr++;
                }

                if(pu32DataGb[count] >= au32MaxMinGb[0] && pu32DataGb[count] <= au32MaxMinGb[1])
                {
                   u32SumGb += pu32DataGb[count];
                   u32CountGb++;
                }

                if(pu32DataB[count] >= au32MaxMinB[0] && pu32DataB[count] <= au32MaxMinB[1])
                {
                   u32SumB += pu32DataB[count];
                   u32CountB++;
                }
			}

			if (u32CountR != 0 && u32CountGr != 0 && u32CountGb != 0 && u32CountB != 0)
			{
                u32GridIndex = i*grid_x_size + j;
                u32SumR = u32SumR / DIV_0_TO_1(u32CountR);
                u32SumGr = u32SumGr / DIV_0_TO_1(u32CountGr);
                u32SumGb = u32SumGb / DIV_0_TO_1(u32CountGb);
                u32SumB = u32SumB / DIV_0_TO_1(u32CountB);
                
				pls_data->pu32B_data[u32GridIndex]  = u32SumB  << enBit;
				pls_data->pu32Gb_data[u32GridIndex] = u32SumGb << enBit;
				pls_data->pu32Gr_data[u32GridIndex] = u32SumGr << enBit;
				pls_data->pu32R_data[u32GridIndex]  = u32SumR  << enBit;
			}
			else
			{
                //free memory of pu32Data
                free(pu32DataR);
                free(pu32DataGr);
                free(pu32DataGb);
                free(pu32DataB);
 
                //free memory of pu32HistArray
                free(pu32HistArrayR);
                free(pu32HistArrayGr);
                free(pu32HistArrayGb); 
                free(pu32HistArrayB); 

                pu32DataR = HI_NULL;
                pu32DataGr = HI_NULL;
                pu32DataGb = HI_NULL;
                pu32DataB = HI_NULL;
                
                pu32HistArrayR = HI_NULL;
                pu32HistArrayGr = HI_NULL;
                pu32HistArrayGb = HI_NULL;
                pu32HistArrayB = HI_NULL;
               
				printf("u32CountR = %d u32CountGr = %d u32CountGb = %d u32CountB = %d\n!",u32CountR,
                    u32CountGr,u32CountGb,u32CountB);
				return HI_FAILURE;
			}
		}
	}

    //free pu32Data memory
    free(pu32DataR);
	free(pu32DataGr);
	free(pu32DataGb);
	free(pu32DataB);

    //free pu32HistArray memory
    free(pu32HistArrayR);
    free(pu32HistArrayGr);
    free(pu32HistArrayGb);
    free(pu32HistArrayB);

    pu32DataR = HI_NULL;
    pu32DataGr = HI_NULL;
    pu32DataGb = HI_NULL;
    pu32DataB = HI_NULL;
    
    pu32HistArrayR = HI_NULL;
    pu32HistArrayGr = HI_NULL;
    pu32HistArrayGb = HI_NULL;
    pu32HistArrayB = HI_NULL;
	return HI_SUCCESS;
}

HI_S32 Interp_Bilinear_Inter(HI_U16 *pu16grid, HI_S32 s32row, HI_S32 s32col, HI_S32 s32w, HI_S32 s32h, HI_S32 s32x, HI_S32 s32y)
{
	HI_S32  s32v00, s32v01, s32v10, s32v11;
	HI_S32  s32iv1, s32iv2, s32re;

	s32v00 = (HI_S32)pu16grid[s32row*s32w + s32col];
	s32v01 = (HI_S32)pu16grid[s32row*s32w + s32col + 1];
	s32v10 = (HI_S32)pu16grid[(s32row+1)*s32w + s32col];
	s32v11 = (HI_S32)pu16grid[(s32row+1)*s32w + s32col + 1];

	s32iv1 = s32v00 + ((s32v10-s32v00)*s32y + 16384)/LSC_GRAD_Q_VALUE;
	s32iv2 = s32v01 + ((s32v11-s32v01)*s32y + 16384)/LSC_GRAD_Q_VALUE;
	s32re  = s32iv1 + ((s32iv2-s32iv1)*s32x + 16384)/LSC_GRAD_Q_VALUE;
	return s32re;
}

HI_S32 Interp_for_GridSizeX_GridSizeY(ISP_Mesh_Shading_Grid_Table_S lsc_gain_profile
    ,ISP_MESH_SHADING_TABLE_S stMeshShadingResult,HI_U32 u32GridSizeX,HI_U32 u32GridSizeY)
{
    if(lsc_gain_profile.pu16Rgain == HI_NULL || lsc_gain_profile.pu16Ggain == HI_NULL ||
        lsc_gain_profile.pu16Bgain == HI_NULL)
    {
        printf("%s: LINE: %d lsc_gain_profile of each Channel gain with NULL point !\n",__FUNCTION__,__LINE__);
        return HI_FAILURE;  
    }

    if(stMeshShadingResult.pu8Bgain == HI_NULL || stMeshShadingResult.pu8Ggain == HI_NULL ||
        stMeshShadingResult.pu8Rgain == HI_NULL)
    {
        printf("%s: LINE: %d stMeshShadingResult of each Channel gain with NULL point !\n",__FUNCTION__,__LINE__);
        return HI_FAILURE;   
    }
    
	HI_S32 i = 0;
    HI_S32 j = 0;
	HI_S32 s32Pos = 0;
	HI_S32 s32Row = 0;
    HI_S32 s32Col = 0;
    HI_S32 x = 0;
    HI_S32 y = 0;
    HI_U32 u32Rgain = 0;
    HI_U32 u32Ggain = 0;
    HI_U32 u32Bgain = 0;

	for (j = 0; j < (u32GridSizeY-1); j++)
	{
		s32Row = (HI_S32)(1.0/DIV_0_TO_1(u32GridSizeY-1) * (j + 0.5) * (u32GridSizeY - 1));
		y = (HI_S32)((1.0/DIV_0_TO_1(u32GridSizeY-1) * (j + 0.5) * (u32GridSizeY - 1) - s32Row) * LSC_GRAD_Q_VALUE);

		for (i = 0; i < (u32GridSizeX-1); i++)
		{
     		s32Pos = j * (u32GridSizeX-1) + i;
     		s32Col = (HI_S32)(1.0/DIV_0_TO_1(u32GridSizeX-1) * (i + 0.5) * (u32GridSizeX - 1));
     		x = (HI_S32)((1.0/DIV_0_TO_1(u32GridSizeX-1) * (i + 0.5) * (u32GridSizeX - 1) - s32Col) * LSC_GRAD_Q_VALUE);

            u32Rgain = (HI_U32)(Interp_Bilinear_Inter(&(lsc_gain_profile.pu16Rgain[0]),s32Row,s32Col,u32GridSizeX,u32GridSizeY,x,y));
            u32Ggain = (HI_U32)(Interp_Bilinear_Inter(&(lsc_gain_profile.pu16Ggain[0]),s32Row,s32Col,u32GridSizeX,u32GridSizeY,x,y));
            u32Bgain = (HI_U32)(Interp_Bilinear_Inter(&(lsc_gain_profile.pu16Bgain[0]),s32Row,s32Col,u32GridSizeX,u32GridSizeY,x,y));
            
     		stMeshShadingResult.pu8Rgain[s32Pos] = MIN(u32Rgain , 255);
     		stMeshShadingResult.pu8Ggain[s32Pos] = MIN(u32Ggain , 255);
     		stMeshShadingResult.pu8Bgain[s32Pos] = MIN(u32Bgain , 255);
        }
	}
    return HI_SUCCESS;
}

HI_S32 Check_LSCCaliCfg(ISP_LSC_CALIBRATION_CFG_S * pstLSCCaliCfg)
{
    if(pstLSCCaliCfg->bOffsetInEn != HI_TRUE && pstLSCCaliCfg->bOffsetInEn != HI_FALSE)
    {
       printf("%s: LINE: %d bOffsetInEn is illegal parameter\n",__FUNCTION__,__LINE__);
       return HI_FAILURE;
    }

    if(pstLSCCaliCfg->enBayer >= BAYER_BUTT)
    {
       printf("%s: LINE: %d Invalid value of enBayer\n",__FUNCTION__,__LINE__); 
       return HI_FAILURE;
    }

    if(pstLSCCaliCfg->enRawBit != BAYER_RAW_8BIT && pstLSCCaliCfg->enRawBit != BAYER_RAW_10BIT &&
        pstLSCCaliCfg->enRawBit != BAYER_RAW_12BIT && pstLSCCaliCfg->enRawBit != BAYER_RAW_14BIT &&
        pstLSCCaliCfg->enRawBit != BAYER_RAW_16BIT)
    {
       printf("%s: LINE: %d Invalid value of enRawBit\n",__FUNCTION__,__LINE__); 
       return HI_FAILURE;
    }

    if(pstLSCCaliCfg->u16ImageHeight < RES_HEIGHT_MIN || pstLSCCaliCfg->u16ImageHeight > RES_HEIGHT_MAX)
    {
       printf("%s: LINE: %d Invalid value of ImageHeight\n",__FUNCTION__,__LINE__); 
       return HI_FAILURE;
    }

    if(pstLSCCaliCfg->u16ImageWidth< RES_WIDTH_MIN || pstLSCCaliCfg->u16ImageWidth > RES_WIDTH_MAX)
    {
       printf("%s: LINE: %d Invalid value of ImageWidth\n",__FUNCTION__,__LINE__); 
       return HI_FAILURE;
    }

    if(pstLSCCaliCfg->u16BLCOffsetR > 0xFFF || pstLSCCaliCfg->u16BLCOffsetGb > 0xFFF || pstLSCCaliCfg->u16BLCOffsetGr > 0xFFF
        || pstLSCCaliCfg->u16BLCOffsetB > 0xFFF)
    {
       printf("%s: LINE: %d Invalid value of Black Level\n",__FUNCTION__,__LINE__); 
       return HI_FAILURE;
    }

    if(pstLSCCaliCfg->u8EliminatePer >= (ELIMINATE_TOTAL_PERCENT>>1))
    {
       printf("%s: LINE: %d Invalid value of EliminatePer\n",__FUNCTION__,__LINE__); 
       return HI_FAILURE;
    }

    if(pstLSCCaliCfg->u8GridSizeX < GRIDSIZEX_MIN || pstLSCCaliCfg->u8GridSizeX > GRIDSIZEX_MAX)
    {
       printf("%s: LINE: %d Invalid value of GridSizeX\n",__FUNCTION__,__LINE__); 
       return HI_FAILURE; 
    }

    if(pstLSCCaliCfg->u8GridSizeY < GRIDSIZEY_MIN || pstLSCCaliCfg->u8GridSizeY > GRIDSIZEY_MAX)
    {
       printf("%s: LINE: %d Invalid value of GridSizeY\n",__FUNCTION__,__LINE__); 
       return HI_FAILURE; 
    }
    
    if(pstLSCCaliCfg->u8MeshScale > MESH_SCALE_MAX)
    {
       printf("%s: LINE: %d Invalid value of u8MeshScale\n",__FUNCTION__,__LINE__); 
       return HI_FAILURE;  
    }
    return HI_SUCCESS;   
}

HI_S32 Init_Memory(ISP_LSC_CALIBRATION_CFG_S *pstLSCCaliCfg)
{
   if(pstLSCCaliCfg == HI_NULL)
   {
      printf("%s: LINE: %d stLSCCaliCfg is NULL point \n",__FUNCTION__,__LINE__);
      return HI_FAILURE;
   }
   
   HI_U32 u32GridSizeX = 0;
   HI_U32 u32GridSizeY = 0;

   u32GridSizeX = (HI_U32)pstLSCCaliCfg->u8GridSizeX;
   u32GridSizeY = (HI_U32)pstLSCCaliCfg->u8GridSizeY;
   
   if(u32GridSizeX == 0)
   {
      printf("%s: LINE: %d u32GridSizeX is zero!\n",__FUNCTION__,__LINE__);
      return HI_FAILURE;
   }
   
   g_u32GridWidthStep  = (HI_U32 *)malloc(sizeof(HI_U32)*u32GridSizeX);
   if(g_u32GridWidthStep == HI_NULL)
   {
      printf("%s: LINE: %d malloc memory of GridWidthStep failure!\n",__FUNCTION__,__LINE__);
      return HI_FAILURE;
   }

   if(u32GridSizeY == 0)
   {
      printf("%s: LINE: %d u32GridSizeY is zero!\n",__FUNCTION__,__LINE__);
      return HI_FAILURE;
   }
      
   g_u32GridHeightStep = (HI_U32 *)malloc(sizeof(HI_U32)*u32GridSizeY);
   if(g_u32GridHeightStep == HI_NULL)
   {
      free(g_u32GridWidthStep);
      g_u32GridWidthStep = HI_NULL;
      printf("%s: LINE: %d malloc memory of GridHeightStep failure!\n",__FUNCTION__,__LINE__);
      return HI_FAILURE;
   }
   return HI_SUCCESS;
}

// Using geometric sequence to automatically generate grid size
HI_S32 Geometric_Grid_Size_X(HI_U32 u32CaliGridSizeX,HI_U32 u32CaliWidth)   
{
   //Parameters Definition
   HI_FLOAT *pf16Rx = HI_NULL; 
   HI_U32   *pu32TmpStepX = HI_NULL;
   HI_FLOAT f16SumR = 0.0; 

   HI_U32 i = 0;
   HI_U32 u32sum = 0;
   HI_U32 u32HalfGridSizeX = 0; 
   HI_U32 u32HalfWidth = 0;
   HI_U32 u32Width = 0;
   HI_U32 u32diff = 0;

   //calibration raw info
   u32HalfGridSizeX = (u32CaliGridSizeX - 1)>>1;
   u32Width = u32CaliWidth;
   u32HalfWidth = u32Width >>1;
     
   if(u32HalfGridSizeX < 2 || u32HalfGridSizeX > 33)
   {
       printf("%s: LINE: %d u32HalfGridSizeX should be [2,32]!\n",__FUNCTION__,__LINE__);
       return HI_FAILURE ;
   }
   
   //Memory allocate
   pf16Rx = (HI_FLOAT*)malloc(sizeof(HI_FLOAT)*u32HalfGridSizeX); 
   if (pf16Rx == HI_NULL)
   {   
       printf("%s: LINE: %d pf16Rx malloc memory failure!\n",__FUNCTION__,__LINE__);
	   return HI_FAILURE;
   }
   
   pu32TmpStepX = (HI_U32*)malloc(sizeof(HI_U32)*u32HalfGridSizeX);
   if (pu32TmpStepX == HI_NULL)
   {   
       free(pf16Rx);
       pf16Rx = HI_NULL;
       printf("%s: LINE: %d pu32TmpStepX  malloc memory failure!\n",__FUNCTION__,__LINE__);
	   return HI_FAILURE;
   }

   //Function start ;Horizental direction
   pf16Rx[0] = 1.0f;
   for ( i = 1 ; i < u32HalfGridSizeX; i++ )
   {
       pf16Rx[i] = pf16Rx[i-1];
   }

   f16SumR = 0;
   for ( i = 0 ; i < u32HalfGridSizeX; i++ )
   {
      f16SumR = f16SumR + pf16Rx[i];
   }
   
   for ( i = 0 ; i < u32HalfGridSizeX; i++ )
   {
      pu32TmpStepX[i] = (HI_U32)(((u32Width/2*1024/DIV_0_TO_1(f16SumR))*pf16Rx[i]+(1024>>1))/1024);
   }

   u32sum = 0;
   for (i = 0;i < u32HalfGridSizeX;i++)
   {
      u32sum = u32sum + pu32TmpStepX[i];
   } 
   
   if (u32sum != u32HalfWidth)
   {
	  if (u32sum > u32HalfWidth)
	  {
		 u32diff = u32sum - u32HalfWidth; 
		 for (i = 1;i <= u32diff;i++)
		 {
            pu32TmpStepX[u32HalfGridSizeX-i] = pu32TmpStepX[u32HalfGridSizeX-i] - 1;
		 }
	  }
      
      else
	  {
		 u32diff = u32Width/2 - u32sum;
		 for (i = 1;i <= u32diff ; i++)
		 {
            pu32TmpStepX[i-1] = pu32TmpStepX[i-1]+1;
		 }
	  }
   }

   //Return the step length value to GridStepWidth 
   g_u32GridWidthStep[0] = 0;
   for ( i = 1 ; i <= u32HalfGridSizeX; i++ )
   {
        g_u32GridWidthStep[i] = pu32TmpStepX[i-1];
        g_u32GridWidthStep[u32CaliGridSizeX - i] = pu32TmpStepX[i-1];
		
   }

   //free memory
   free(pf16Rx);
   free(pu32TmpStepX);
   pf16Rx = HI_NULL;
   pu32TmpStepX = HI_NULL;
   return HI_SUCCESS;
}

HI_S32 Geometric_Grid_Size_Y(HI_U32 u32CaliGridSizeY,HI_U32 u32CaliHeight)   
{
   //param definition
   HI_FLOAT *pf16Ry = HI_NULL;       
   HI_U32   *pu32TmpStepY = HI_NULL;  
   HI_FLOAT  f16SumR = 0.0; 

   HI_U32 j = 0;
   HI_U32 u32sum = 0;
   HI_U32 u32HalfGridSizeY = 0; 
   HI_U32 u32HalfHeight = 0;
   HI_U32 u32Height = 0;
   HI_U32 u32diff = 0;
   
   //calibration raw info
   u32HalfGridSizeY = (u32CaliGridSizeY - 1)>>1;
   u32Height = u32CaliHeight;
   u32HalfHeight = u32Height >>1;
  
   if(u32HalfGridSizeY < 2 || u32HalfGridSizeY > 33)
   {
       printf("%s: LINE: %d u32HalfGridSizeY should be [2,32]!\n",__FUNCTION__,__LINE__);
       return HI_FAILURE ;
   }
   
   //Memory allocate
   pf16Ry = (HI_FLOAT*)malloc(sizeof(HI_FLOAT)*u32HalfGridSizeY);
   if(pf16Ry == HI_NULL)
   {
       printf("%s: LINE: %d pf16Ry malloc memory failure!\n",__FUNCTION__,__LINE__);
	   return HI_FAILURE;
   }
   
   pu32TmpStepY = (HI_U32*)malloc(sizeof(HI_U32)*u32HalfGridSizeY);
   
   if (pu32TmpStepY == HI_NULL)
   {
       free(pf16Ry);
       pf16Ry = HI_NULL;
       printf("%s: LINE: %d pu32TmpStepY  malloc memory failure!\n",__FUNCTION__,__LINE__);
	   return HI_FAILURE;
   }

   //Function start: vertical direction
   pf16Ry[0] = 1.0f;

   //Vertical direction
   for (j = 1; j < u32HalfGridSizeY; j++ )
   {
       pf16Ry[j] = pf16Ry[j-1];
   }

   f16SumR = 0;
   for (j = 0; j < u32HalfGridSizeY; j++ )
   {
       f16SumR = f16SumR + pf16Ry[j];
   }

   for (j = 0; j < u32HalfGridSizeY; j++)
   {
        pu32TmpStepY[j] = (HI_U32)(((u32Height/2*1024/DIV_0_TO_1(f16SumR)) * pf16Ry[j]+ (1024>>1))/1024);
   }

   u32sum = 0;
   for (j = 0; j < u32HalfGridSizeY; j++)
   {     
      u32sum = u32sum + pu32TmpStepY[j];
   }

   if (u32sum != u32HalfHeight)
   {
      if (u32sum > u32HalfHeight)
      {
         u32diff = u32sum - u32HalfHeight; 
         for (j = 1; j <= u32diff; j++)
         {
            pu32TmpStepY[u32HalfGridSizeY-j]= pu32TmpStepY[u32HalfGridSizeY-j]-1;
         }
      }
      else
      {
         u32diff = u32HalfHeight - u32sum;
         for (j = 1; j <= u32diff; j++)
         {
            pu32TmpStepY[j-1] = pu32TmpStepY[j-1]+1;
         }
      }
    }

    //Return the step length value to GridStepHeight
    g_u32GridHeightStep[0] = 0;
    for ( j = 1 ; j <= u32HalfGridSizeY; j++ )
    {
        g_u32GridHeightStep[j] = pu32TmpStepY[j-1];
        g_u32GridHeightStep[u32CaliGridSizeY - j] = pu32TmpStepY[j-1];
    }

    //free memory
    free(pf16Ry);
	free(pu32TmpStepY);
    pf16Ry = HI_NULL;
    pu32TmpStepY = HI_NULL;
    return(HI_SUCCESS);
}

HI_U32 Mesh_Shading_Data_Enlarge(HI_U32 u32data, HI_U32 u32maxData, HI_U8 u8MeshScale)
{
    HI_U32 ratio = 0;
	HI_U32 u32LscQValue = 0;
	if (u8MeshScale < 4)
	{
		u32LscQValue = 1<<(7 - u8MeshScale);
		ratio = (HI_U32)(((HI_FLOAT)u32maxData / (HI_FLOAT)DIV_0_TO_1(u32data))*u32LscQValue);
	}
	else
	{
		u32LscQValue = 1<<(12 - u8MeshScale);
		ratio = (HI_U32)(((HI_FLOAT)u32maxData / (HI_FLOAT)DIV_0_TO_1(u32data) - 1)*u32LscQValue);
	}
	return ratio;
}

HI_S32 Lsc_Normalization(ISP_Mesh_Shading_BayerChannel_Grid_Data_S *pls_data, ISP_Mesh_Shading_BayerChannel_Table_S *plsc_grid_info, 
    ISP_LSC_CALIBRATION_CFG_S * pstLSCCaliCfg)
{
    if(pls_data == HI_NULL || plsc_grid_info == HI_NULL || pstLSCCaliCfg == HI_NULL)
    {
        printf("%s: LINE: %d Input paramters of Lsc_Normalization with NULL point\n",__FUNCTION__,__LINE__);
        return HI_FAILURE;
    }

    if(pls_data->pu32R_data == HI_NULL || pls_data->pu32Gb_data == HI_NULL || pls_data->pu32Gr_data == HI_NULL
        || pls_data->pu32B_data == HI_NULL)
    {
        printf("%s: LINE: %d pls_data of each channel with NULL point\n",__FUNCTION__,__LINE__);
        return HI_FAILURE; 
    }

    if(plsc_grid_info->pu32Rgain == HI_NULL || plsc_grid_info->pu32Gbgain == HI_NULL || 
        plsc_grid_info->pu32Grgain == HI_NULL || plsc_grid_info->pu32Bgain == HI_NULL)
    {
        printf("%s: LINE: %d plsc_grid_info Gain of each channel with NULL point\n",__FUNCTION__,__LINE__);
        return HI_FAILURE; 
    }
    
	HI_U32 b_max_data  = 0;
    HI_U32 gb_max_data = 0;
    HI_U32 gr_max_data = 0;
    HI_U32 r_max_data  = 0;
    HI_U32 u32LSCGridPoints = 0;
	HI_U8  u8MeshScale = pstLSCCaliCfg->u8MeshScale;
	HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i = 0;

    u32LSCGridPoints = (HI_U32)pstLSCCaliCfg->u8GridSizeX * (HI_U32)pstLSCCaliCfg->u8GridSizeY;

	//Mesh Shading scale's Max gain test
	s32Ret = Mesh_Shading_Scale_Test(pls_data, u32LSCGridPoints, u8MeshScale);
    if(s32Ret != HI_SUCCESS)
    {
        printf("%s: LINE: %d Mesh_Shading_Scale_Test return failure\n",__FUNCTION__,__LINE__);
        return HI_FAILURE;
    }

	//Get Max value of B/Gb/Gr/R channel
	b_max_data  = Get_Max_Data(pls_data->pu32B_data, u32LSCGridPoints);
	gb_max_data = Get_Max_Data(pls_data->pu32Gb_data, u32LSCGridPoints);
	gr_max_data = Get_Max_Data(pls_data->pu32Gr_data, u32LSCGridPoints);
	r_max_data  = Get_Max_Data(pls_data->pu32R_data, u32LSCGridPoints);

    for ( i = 0 ; i < u32LSCGridPoints ; i++ )
    {
        plsc_grid_info->pu32Bgain[i]  = Mesh_Shading_Data_Enlarge(pls_data->pu32B_data[i],  b_max_data,  u8MeshScale);
        plsc_grid_info->pu32Gbgain[i] = Mesh_Shading_Data_Enlarge(pls_data->pu32Gb_data[i], gb_max_data, u8MeshScale);
        plsc_grid_info->pu32Grgain[i] = Mesh_Shading_Data_Enlarge(pls_data->pu32Gr_data[i], gr_max_data, u8MeshScale);
        plsc_grid_info->pu32Rgain[i]  = Mesh_Shading_Data_Enlarge(pls_data->pu32R_data[i],  r_max_data,  u8MeshScale);
    }
    return HI_SUCCESS;
}


HI_S32 ISP_MeshShadingCalibration(HI_U16 *pu16SrcRaw, 
    ISP_LSC_CALIBRATION_CFG_S *pstLSCCaliCfg , ISP_MESH_SHADING_TABLE_S stMeshShadingResult)
{
    if(pu16SrcRaw == HI_NULL || pstLSCCaliCfg == HI_NULL)
    {
        printf("%s: LINE: %d pu16SrcRaw/pstLSCCaliCfg is NULL Point!\n",__FUNCTION__,__LINE__);
        return HI_FAILURE; 
    }

    if(stMeshShadingResult.pu8Bgain == HI_NULL || stMeshShadingResult.pu8Ggain == HI_NULL ||
        stMeshShadingResult.pu8Rgain == HI_NULL)
    {
        printf("%s: LINE: %d stMeshShadingResult of each channel gain is NULL Point!\n",__FUNCTION__,__LINE__);
        return HI_FAILURE;  
    }

    HI_S32 S32Ret = HI_FAILURE;

    //Step1: Check the valid param of Mesh Shading cfg
    S32Ret = Check_LSCCaliCfg(pstLSCCaliCfg);
    if(S32Ret != HI_SUCCESS)
    {
       printf("%s: LINE: %d Check_LSCCaliCfg failure!\n",__FUNCTION__,__LINE__);
       return HI_FAILURE;
    }
        
    //local varaible declaration;
    HI_U32 u32Height = 0;
    HI_U32 u32Wdith  = 0;
    HI_U32 u32GridX  = 0;
    HI_U32 u32GridY  = 0;
    HI_U32 u32LSCGridPoints = 0;
    HI_U32 i = 0;

#if 0
    // Debug for write shading calibration result to txt
    HI_CHAR FileNameR[255]="TMPRgain.txt";
	HI_CHAR FileNameG[255]="TMPGgain.txt";
	HI_CHAR FileNameB[255]="TMPBgain.txt";
    
	FILE *pFileR = fopen(FileNameR, "wb");
	FILE *pFileG = fopen(FileNameG, "wb");
	FILE *pFileB = fopen(FileNameB, "wb");
#endif 

    ISP_Mesh_Shading_BayerChannel_Grid_Data_S stMeshShadingBayerChannelGridData;
    ISP_Mesh_Shading_BayerChannel_Table_S     stMeshShadingBayerChannelGain;
    ISP_Mesh_Shading_Grid_Table_S             stMeshShadingGridTab;

    u32Height = (HI_U32)pstLSCCaliCfg->u16ImageHeight;
    u32Wdith  = (HI_U32)pstLSCCaliCfg->u16ImageWidth;
    u32GridX  = (HI_U32)pstLSCCaliCfg->u8GridSizeX;
    u32GridY  = (HI_U32)pstLSCCaliCfg->u8GridSizeY;
    u32LSCGridPoints = u32GridX * u32GridY;

    if(u32LSCGridPoints == 0)
    {
        printf("%s: LINE: %d u32LSCGridPoints is Zero !\n",__FUNCTION__,__LINE__);
        return HI_FAILURE;
    }

    //stMeshShadingBayerChannelGridData memory malloc
    stMeshShadingBayerChannelGridData.pu32R_data  = (HI_U32*)malloc(sizeof(HI_U32)*u32LSCGridPoints);
    if(stMeshShadingBayerChannelGridData.pu32R_data == HI_NULL)
    {
        printf("%s: LINE: %d stMeshShadingBayerChannelGridData.pu32R_data  malloc failure !\n",__FUNCTION__,__LINE__);
        return HI_FAILURE;
    }
    
    stMeshShadingBayerChannelGridData.pu32Gr_data = (HI_U32*)malloc(sizeof(HI_U32)*u32LSCGridPoints);
    if(stMeshShadingBayerChannelGridData.pu32Gr_data == HI_NULL)
    {
        free(stMeshShadingBayerChannelGridData.pu32R_data);
        stMeshShadingBayerChannelGridData.pu32R_data = HI_NULL;
        printf("%s: LINE: %d stMeshShadingBayerChannelGridData.pu32Gr_data  malloc failure !\n",__FUNCTION__,__LINE__);
        return HI_FAILURE;
    }
    
    stMeshShadingBayerChannelGridData.pu32Gb_data = (HI_U32*)malloc(sizeof(HI_U32)*u32LSCGridPoints);
    if(stMeshShadingBayerChannelGridData.pu32Gb_data == HI_NULL)
    {
        free(stMeshShadingBayerChannelGridData.pu32R_data);
        free(stMeshShadingBayerChannelGridData.pu32Gr_data);
        
        stMeshShadingBayerChannelGridData.pu32R_data = HI_NULL;
        stMeshShadingBayerChannelGridData.pu32Gr_data = HI_NULL;
        printf("%s: LINE: %d stMeshShadingBayerChannelGridData.pu32Gb_data  malloc failure !\n",__FUNCTION__,__LINE__);
        return HI_FAILURE;
    }
    
    stMeshShadingBayerChannelGridData.pu32B_data  = (HI_U32*)malloc(sizeof(HI_U32)*u32LSCGridPoints);
    if(stMeshShadingBayerChannelGridData.pu32B_data == HI_NULL)
    {
        free(stMeshShadingBayerChannelGridData.pu32R_data);
        free(stMeshShadingBayerChannelGridData.pu32Gr_data);
        free(stMeshShadingBayerChannelGridData.pu32Gb_data);
        
        stMeshShadingBayerChannelGridData.pu32R_data = HI_NULL;
        stMeshShadingBayerChannelGridData.pu32Gr_data = HI_NULL;
        stMeshShadingBayerChannelGridData.pu32Gb_data = HI_NULL;
        printf("%s: LINE: %d stMeshShadingBayerChannelGridData.pu32B_data malloc failure !\n",__FUNCTION__,__LINE__);
        return HI_FAILURE;
    }

    //stMeshShadingBayerChannelGain memory malloc
    stMeshShadingBayerChannelGain.pu32Rgain  = (HI_U32*)malloc(sizeof(HI_U32)*u32LSCGridPoints);
    if(stMeshShadingBayerChannelGain.pu32Rgain == HI_NULL)
    {
        //free memory of stMeshShadingBayerChannelGridData
        free(stMeshShadingBayerChannelGridData.pu32R_data);
        free(stMeshShadingBayerChannelGridData.pu32Gr_data);
        free(stMeshShadingBayerChannelGridData.pu32Gb_data);
        free(stMeshShadingBayerChannelGridData.pu32B_data);
        
        stMeshShadingBayerChannelGridData.pu32R_data = HI_NULL;
        stMeshShadingBayerChannelGridData.pu32Gr_data = HI_NULL;
        stMeshShadingBayerChannelGridData.pu32Gb_data = HI_NULL;
        stMeshShadingBayerChannelGridData.pu32B_data = HI_NULL;
        printf("%s: LINE: %d stMeshShadingBayerChannelGain.pu32Rgain malloc failure !\n",__FUNCTION__,__LINE__);
        return HI_FAILURE;
    }
    
    stMeshShadingBayerChannelGain.pu32Grgain = (HI_U32*)malloc(sizeof(HI_U32)*u32LSCGridPoints);
    if(stMeshShadingBayerChannelGain.pu32Grgain == HI_NULL)
    {
        //free memory of stMeshShadingBayerChannelGridData
        free(stMeshShadingBayerChannelGridData.pu32R_data);
        free(stMeshShadingBayerChannelGridData.pu32Gr_data);
        free(stMeshShadingBayerChannelGridData.pu32Gb_data);
        free(stMeshShadingBayerChannelGridData.pu32B_data);

        free(stMeshShadingBayerChannelGain.pu32Rgain);

        stMeshShadingBayerChannelGridData.pu32R_data = HI_NULL;
        stMeshShadingBayerChannelGridData.pu32Gr_data = HI_NULL;
        stMeshShadingBayerChannelGridData.pu32Gb_data = HI_NULL;
        stMeshShadingBayerChannelGridData.pu32B_data = HI_NULL;
        
        stMeshShadingBayerChannelGain.pu32Rgain = HI_NULL;
        
        printf("%s: LINE: %d stMeshShadingBayerChannelGain.pu32Grgain malloc failure !\n",__FUNCTION__,__LINE__);
        return HI_FAILURE;
    }
    
    stMeshShadingBayerChannelGain.pu32Gbgain = (HI_U32*)malloc(sizeof(HI_U32)*u32LSCGridPoints);
    if(stMeshShadingBayerChannelGain.pu32Gbgain == HI_NULL)
    {
        //free memory of stMeshShadingBayerChannelGridData
        free(stMeshShadingBayerChannelGridData.pu32R_data);
        free(stMeshShadingBayerChannelGridData.pu32Gr_data);
        free(stMeshShadingBayerChannelGridData.pu32Gb_data);
        free(stMeshShadingBayerChannelGridData.pu32B_data);

        free(stMeshShadingBayerChannelGain.pu32Rgain);
        free(stMeshShadingBayerChannelGain.pu32Grgain);

        stMeshShadingBayerChannelGridData.pu32R_data = HI_NULL;
        stMeshShadingBayerChannelGridData.pu32Gr_data = HI_NULL;
        stMeshShadingBayerChannelGridData.pu32Gb_data = HI_NULL;
        stMeshShadingBayerChannelGridData.pu32B_data = HI_NULL;
        
        stMeshShadingBayerChannelGain.pu32Rgain = HI_NULL;
        stMeshShadingBayerChannelGain.pu32Grgain = HI_NULL;
        
        printf("%s: LINE: %d stMeshShadingBayerChannelGain.pu32Gbgain malloc failure !\n",__FUNCTION__,__LINE__);
        return HI_FAILURE;
    }
    
    stMeshShadingBayerChannelGain.pu32Bgain  = (HI_U32*)malloc(sizeof(HI_U32)*u32LSCGridPoints);
    if(stMeshShadingBayerChannelGain.pu32Bgain == HI_NULL)
    {
        //free memory of stMeshShadingBayerChannelGridData
        free(stMeshShadingBayerChannelGridData.pu32R_data);
        free(stMeshShadingBayerChannelGridData.pu32Gr_data);
        free(stMeshShadingBayerChannelGridData.pu32Gb_data);
        free(stMeshShadingBayerChannelGridData.pu32B_data);

        free(stMeshShadingBayerChannelGain.pu32Rgain);
        free(stMeshShadingBayerChannelGain.pu32Grgain);
        free(stMeshShadingBayerChannelGain.pu32Gbgain);

        stMeshShadingBayerChannelGridData.pu32R_data = HI_NULL;
        stMeshShadingBayerChannelGridData.pu32Gr_data = HI_NULL;
        stMeshShadingBayerChannelGridData.pu32Gb_data = HI_NULL;
        stMeshShadingBayerChannelGridData.pu32B_data = HI_NULL;
        
        stMeshShadingBayerChannelGain.pu32Rgain = HI_NULL;
        stMeshShadingBayerChannelGain.pu32Grgain = HI_NULL;
        stMeshShadingBayerChannelGain.pu32Gbgain = HI_NULL;
        
        printf("%s: LINE: %d stMeshShadingBayerChannelGain.pu32Bgain malloc failure !\n",__FUNCTION__,__LINE__);
        return HI_FAILURE;
    }

    //stMeshShadingGridTab memory malloc
    stMeshShadingGridTab.pu16Rgain = (HI_U16*)malloc(sizeof(HI_U16)*u32LSCGridPoints);
    if(stMeshShadingGridTab.pu16Rgain == HI_NULL)
    {
        //free memory of of stMeshShadingBayerChannelGridData
        free(stMeshShadingBayerChannelGridData.pu32R_data);
        free(stMeshShadingBayerChannelGridData.pu32Gr_data);
        free(stMeshShadingBayerChannelGridData.pu32Gb_data);
        free(stMeshShadingBayerChannelGridData.pu32B_data);

        //free memory of stMeshShadingBayerChannelGain
        free(stMeshShadingBayerChannelGain.pu32Rgain );
        free(stMeshShadingBayerChannelGain.pu32Grgain);
        free(stMeshShadingBayerChannelGain.pu32Gbgain);
        free(stMeshShadingBayerChannelGain.pu32Bgain); 

        stMeshShadingBayerChannelGridData.pu32R_data = HI_NULL;
        stMeshShadingBayerChannelGridData.pu32Gr_data = HI_NULL;
        stMeshShadingBayerChannelGridData.pu32Gb_data = HI_NULL;
        stMeshShadingBayerChannelGridData.pu32B_data = HI_NULL;
        
        stMeshShadingBayerChannelGain.pu32Rgain = HI_NULL;
        stMeshShadingBayerChannelGain.pu32Grgain = HI_NULL;
        stMeshShadingBayerChannelGain.pu32Gbgain = HI_NULL;
        stMeshShadingBayerChannelGain.pu32Bgain = HI_NULL;
        
        printf("%s: LINE: %d stMeshShadingGridTab.pu16Rgain malloc failure !\n",__FUNCTION__,__LINE__);
        return HI_FAILURE;
    }
    
    stMeshShadingGridTab.pu16Ggain = (HI_U16*)malloc(sizeof(HI_U16)*u32LSCGridPoints);
    if(stMeshShadingGridTab.pu16Ggain == HI_NULL)
    {
        //free memory of of stMeshShadingBayerChannelGridData
        free(stMeshShadingBayerChannelGridData.pu32R_data);
        free(stMeshShadingBayerChannelGridData.pu32Gr_data);
        free(stMeshShadingBayerChannelGridData.pu32Gb_data);
        free(stMeshShadingBayerChannelGridData.pu32B_data);

        //free memory of stMeshShadingBayerChannelGain
        free(stMeshShadingBayerChannelGain.pu32Rgain );
        free(stMeshShadingBayerChannelGain.pu32Grgain);
        free(stMeshShadingBayerChannelGain.pu32Gbgain);
        free(stMeshShadingBayerChannelGain.pu32Bgain); 

        free(stMeshShadingGridTab.pu16Rgain);

        stMeshShadingBayerChannelGridData.pu32R_data = HI_NULL;
        stMeshShadingBayerChannelGridData.pu32Gr_data = HI_NULL;
        stMeshShadingBayerChannelGridData.pu32Gb_data = HI_NULL;
        stMeshShadingBayerChannelGridData.pu32B_data = HI_NULL;
        
        stMeshShadingBayerChannelGain.pu32Rgain = HI_NULL;
        stMeshShadingBayerChannelGain.pu32Grgain = HI_NULL;
        stMeshShadingBayerChannelGain.pu32Gbgain = HI_NULL;
        stMeshShadingBayerChannelGain.pu32Bgain = HI_NULL;
        stMeshShadingGridTab.pu16Rgain = HI_NULL;
        
        printf("%s: LINE: %d stMeshShadingGridTab.pu16Ggain malloc failure !\n",__FUNCTION__,__LINE__);
        return HI_FAILURE;
    }
    
    stMeshShadingGridTab.pu16Bgain = (HI_U16*)malloc(sizeof(HI_U16)*u32LSCGridPoints);
    if(stMeshShadingGridTab.pu16Bgain == HI_NULL)
    {
        //free memory of of stMeshShadingBayerChannelGridData
        free(stMeshShadingBayerChannelGridData.pu32R_data);
        free(stMeshShadingBayerChannelGridData.pu32Gr_data);
        free(stMeshShadingBayerChannelGridData.pu32Gb_data);
        free(stMeshShadingBayerChannelGridData.pu32B_data);

        //free memory of stMeshShadingBayerChannelGain
        free(stMeshShadingBayerChannelGain.pu32Rgain );
        free(stMeshShadingBayerChannelGain.pu32Grgain);
        free(stMeshShadingBayerChannelGain.pu32Gbgain);
        free(stMeshShadingBayerChannelGain.pu32Bgain); 

        free(stMeshShadingGridTab.pu16Rgain);
        free(stMeshShadingGridTab.pu16Ggain);

        stMeshShadingBayerChannelGridData.pu32R_data = HI_NULL;
        stMeshShadingBayerChannelGridData.pu32Gr_data = HI_NULL;
        stMeshShadingBayerChannelGridData.pu32Gb_data = HI_NULL;
        stMeshShadingBayerChannelGridData.pu32B_data = HI_NULL;
        
        stMeshShadingBayerChannelGain.pu32Rgain = HI_NULL;
        stMeshShadingBayerChannelGain.pu32Grgain = HI_NULL;
        stMeshShadingBayerChannelGain.pu32Gbgain = HI_NULL;
        stMeshShadingBayerChannelGain.pu32Bgain = HI_NULL;
        
        stMeshShadingGridTab.pu16Rgain = HI_NULL;
        stMeshShadingGridTab.pu16Ggain = HI_NULL;
        
        printf("%s: LINE: %d stMeshShadingGridTab.pu16Bgain malloc failure !\n",__FUNCTION__,__LINE__);
        return HI_FAILURE;
    }

    //Step2: Init Global Memory
    S32Ret = Init_Memory(pstLSCCaliCfg);
    if(S32Ret != HI_SUCCESS)
    {
       //free global memory
       free(g_u32GridWidthStep);
       free(g_u32GridHeightStep);
       
       //free memory of stMeshShadingBayerChannelGridData
       free(stMeshShadingBayerChannelGridData.pu32R_data);
       free(stMeshShadingBayerChannelGridData.pu32Gr_data);
       free(stMeshShadingBayerChannelGridData.pu32Gb_data);
       free(stMeshShadingBayerChannelGridData.pu32B_data);
   
       //free memory of stMeshShadingBayerChannelGain
       free(stMeshShadingBayerChannelGain.pu32Rgain );
       free(stMeshShadingBayerChannelGain.pu32Grgain);
       free(stMeshShadingBayerChannelGain.pu32Gbgain);
       free(stMeshShadingBayerChannelGain.pu32Bgain); 

       //free memory of stMeshShadingGridTab
       free(stMeshShadingGridTab.pu16Rgain);
       free(stMeshShadingGridTab.pu16Ggain);
       free(stMeshShadingGridTab.pu16Bgain);

       g_u32GridWidthStep = HI_NULL;
       g_u32GridHeightStep = HI_NULL;

       stMeshShadingBayerChannelGridData.pu32R_data = HI_NULL;
       stMeshShadingBayerChannelGridData.pu32Gr_data = HI_NULL;
       stMeshShadingBayerChannelGridData.pu32Gb_data = HI_NULL;
       stMeshShadingBayerChannelGridData.pu32B_data = HI_NULL;
       
       stMeshShadingBayerChannelGain.pu32Rgain = HI_NULL;
       stMeshShadingBayerChannelGain.pu32Grgain = HI_NULL;
       stMeshShadingBayerChannelGain.pu32Gbgain = HI_NULL;
       stMeshShadingBayerChannelGain.pu32Bgain = HI_NULL;
       
       stMeshShadingGridTab.pu16Rgain = HI_NULL;
       stMeshShadingGridTab.pu16Ggain = HI_NULL;
       stMeshShadingGridTab.pu16Bgain = HI_NULL;
       printf("%s: LINE: %d Init_Memory failure!\n",__FUNCTION__,__LINE__);
       return HI_FAILURE;
    }

    //Step3: Grid horizontal direction and vertical direction(X/Y)
    S32Ret = Geometric_Grid_Size_X(u32GridX,u32Wdith >> 1);
    if(S32Ret != HI_SUCCESS)
    {
       //free global memory
       free(g_u32GridWidthStep);
       free(g_u32GridHeightStep);
       
       //free memory of stMeshShadingBayerChannelGridData
       free(stMeshShadingBayerChannelGridData.pu32R_data);
       free(stMeshShadingBayerChannelGridData.pu32Gr_data);
       free(stMeshShadingBayerChannelGridData.pu32Gb_data);
       free(stMeshShadingBayerChannelGridData.pu32B_data);
   
       //free memory of stMeshShadingBayerChannelGain
       free(stMeshShadingBayerChannelGain.pu32Rgain);
       free(stMeshShadingBayerChannelGain.pu32Grgain);
       free(stMeshShadingBayerChannelGain.pu32Gbgain);
       free(stMeshShadingBayerChannelGain.pu32Bgain); 

       //free memory of stMeshShadingGridTab
       free(stMeshShadingGridTab.pu16Rgain);
       free(stMeshShadingGridTab.pu16Ggain);
       free(stMeshShadingGridTab.pu16Bgain);

       g_u32GridWidthStep = HI_NULL;
       g_u32GridHeightStep = HI_NULL;

       stMeshShadingBayerChannelGridData.pu32R_data = HI_NULL;
       stMeshShadingBayerChannelGridData.pu32Gr_data = HI_NULL;
       stMeshShadingBayerChannelGridData.pu32Gb_data = HI_NULL;
       stMeshShadingBayerChannelGridData.pu32B_data = HI_NULL;
       
       stMeshShadingBayerChannelGain.pu32Rgain = HI_NULL;
       stMeshShadingBayerChannelGain.pu32Grgain = HI_NULL;
       stMeshShadingBayerChannelGain.pu32Gbgain = HI_NULL;
       stMeshShadingBayerChannelGain.pu32Bgain = HI_NULL;
       
       stMeshShadingGridTab.pu16Rgain = HI_NULL;
       stMeshShadingGridTab.pu16Ggain = HI_NULL;
       stMeshShadingGridTab.pu16Bgain = HI_NULL;
       
       printf("%s: LINE: %d Geometric_Grid_Size_X failure!\n",__FUNCTION__,__LINE__);
       return HI_FAILURE;
    }

    S32Ret = Geometric_Grid_Size_Y(u32GridY,u32Height >> 1);
    if(S32Ret != HI_SUCCESS)
    {
       //free global memory
       free(g_u32GridWidthStep);
       free(g_u32GridHeightStep);
       
       //free memory stMeshShadingBayerChannelGridData
       free(stMeshShadingBayerChannelGridData.pu32R_data);
       free(stMeshShadingBayerChannelGridData.pu32Gr_data);
       free(stMeshShadingBayerChannelGridData.pu32Gb_data);
       free(stMeshShadingBayerChannelGridData.pu32B_data);
   
       //free memory stMeshShadingBayerChannelGain
       free(stMeshShadingBayerChannelGain.pu32Rgain);
       free(stMeshShadingBayerChannelGain.pu32Grgain);
       free(stMeshShadingBayerChannelGain.pu32Gbgain);
       free(stMeshShadingBayerChannelGain.pu32Bgain); 

       //free memory stMeshShadingGridTab
       free(stMeshShadingGridTab.pu16Rgain);
       free(stMeshShadingGridTab.pu16Ggain);
       free(stMeshShadingGridTab.pu16Bgain);

       g_u32GridWidthStep = HI_NULL;
       g_u32GridHeightStep = HI_NULL;

       stMeshShadingBayerChannelGridData.pu32R_data = HI_NULL;
       stMeshShadingBayerChannelGridData.pu32Gr_data = HI_NULL;
       stMeshShadingBayerChannelGridData.pu32Gb_data = HI_NULL;
       stMeshShadingBayerChannelGridData.pu32B_data = HI_NULL;
       
       stMeshShadingBayerChannelGain.pu32Rgain = HI_NULL;
       stMeshShadingBayerChannelGain.pu32Grgain = HI_NULL;
       stMeshShadingBayerChannelGain.pu32Gbgain = HI_NULL;
       stMeshShadingBayerChannelGain.pu32Bgain = HI_NULL;
       
       stMeshShadingGridTab.pu16Rgain = HI_NULL;
       stMeshShadingGridTab.pu16Ggain = HI_NULL;
       stMeshShadingGridTab.pu16Bgain = HI_NULL;
       
       printf("%s: LINE: %d Geometric_Grid_Size_Y failure!\n",__FUNCTION__,__LINE__);
       return HI_FAILURE;
    }

    //Step4: Get Lens Correction Coefficients
    S32Ret = Get_Lens_Shading_Data(pu16SrcRaw, &stMeshShadingBayerChannelGridData, pstLSCCaliCfg);
    if(S32Ret != HI_SUCCESS)
    {
       //free global memory
       free(g_u32GridWidthStep);
       free(g_u32GridHeightStep);
       
       //free memory stMeshShadingBayerChannelGridData
       free(stMeshShadingBayerChannelGridData.pu32R_data);
       free(stMeshShadingBayerChannelGridData.pu32Gr_data);
       free(stMeshShadingBayerChannelGridData.pu32Gb_data);
       free(stMeshShadingBayerChannelGridData.pu32B_data);
   
       //free memory stMeshShadingBayerChannelGain
       free(stMeshShadingBayerChannelGain.pu32Rgain );
       free(stMeshShadingBayerChannelGain.pu32Grgain);
       free(stMeshShadingBayerChannelGain.pu32Gbgain);
       free(stMeshShadingBayerChannelGain.pu32Bgain); 

       //free memory stMeshShadingGridTab
       free(stMeshShadingGridTab.pu16Rgain);
       free(stMeshShadingGridTab.pu16Ggain);
       free(stMeshShadingGridTab.pu16Bgain);

       g_u32GridWidthStep = HI_NULL;
       g_u32GridHeightStep = HI_NULL;

       stMeshShadingBayerChannelGridData.pu32R_data = HI_NULL;
       stMeshShadingBayerChannelGridData.pu32Gr_data = HI_NULL;
       stMeshShadingBayerChannelGridData.pu32Gb_data = HI_NULL;
       stMeshShadingBayerChannelGridData.pu32B_data = HI_NULL;
       
       stMeshShadingBayerChannelGain.pu32Rgain = HI_NULL;
       stMeshShadingBayerChannelGain.pu32Grgain = HI_NULL;
       stMeshShadingBayerChannelGain.pu32Gbgain = HI_NULL;
       stMeshShadingBayerChannelGain.pu32Bgain = HI_NULL;
       
       stMeshShadingGridTab.pu16Rgain = HI_NULL;
       stMeshShadingGridTab.pu16Ggain = HI_NULL;
       stMeshShadingGridTab.pu16Bgain = HI_NULL;
       
       printf("%s: LINE: %d Get Lens Shading data failure!\n",__FUNCTION__,__LINE__);
       return HI_FAILURE;
    }

    //Step5:Normalization the Shading table
    S32Ret = Lsc_Normalization(&stMeshShadingBayerChannelGridData, &stMeshShadingBayerChannelGain, pstLSCCaliCfg);
    if(S32Ret != HI_SUCCESS)
    {
       //free global memory
       free(g_u32GridWidthStep);
       free(g_u32GridHeightStep);
       
       //free memory stMeshShadingBayerChannelGridData
       free(stMeshShadingBayerChannelGridData.pu32R_data);
       free(stMeshShadingBayerChannelGridData.pu32Gr_data);
       free(stMeshShadingBayerChannelGridData.pu32Gb_data);
       free(stMeshShadingBayerChannelGridData.pu32B_data);
   
       //free memory stMeshShadingBayerChannelGain
       free(stMeshShadingBayerChannelGain.pu32Rgain );
       free(stMeshShadingBayerChannelGain.pu32Grgain);
       free(stMeshShadingBayerChannelGain.pu32Gbgain);
       free(stMeshShadingBayerChannelGain.pu32Bgain); 

       //free memory stMeshShadingGridTab
       free(stMeshShadingGridTab.pu16Rgain);
       free(stMeshShadingGridTab.pu16Ggain);
       free(stMeshShadingGridTab.pu16Bgain);

       g_u32GridWidthStep = HI_NULL;
       g_u32GridHeightStep = HI_NULL;

       stMeshShadingBayerChannelGridData.pu32R_data = HI_NULL;
       stMeshShadingBayerChannelGridData.pu32Gr_data = HI_NULL;
       stMeshShadingBayerChannelGridData.pu32Gb_data = HI_NULL;
       stMeshShadingBayerChannelGridData.pu32B_data = HI_NULL;
       
       stMeshShadingBayerChannelGain.pu32Rgain = HI_NULL;
       stMeshShadingBayerChannelGain.pu32Grgain = HI_NULL;
       stMeshShadingBayerChannelGain.pu32Gbgain = HI_NULL;
       stMeshShadingBayerChannelGain.pu32Bgain = HI_NULL;
       
       stMeshShadingGridTab.pu16Rgain = HI_NULL;
       stMeshShadingGridTab.pu16Ggain = HI_NULL;
       stMeshShadingGridTab.pu16Bgain = HI_NULL;
       printf("%s: LINE: %d Lsc_Normalization failure!\n",__FUNCTION__,__LINE__);
       return HI_FAILURE;
    }

    #if 0
    HI_U32 j;
	for (j = 0;j < u32GridY;j++)
	{
		for (i = 0;i < u32GridX;i++)
		{
            #if 0
			fprintf(pFileR , "0x%x,\n",stMeshShadingResult.pu8Rgain[j*(stLSCCaliCfg.u8GridSizeX-1)+i]);
			fprintf(pFileG , "0x%x,\n",stMeshShadingResult.pu8Ggain[j*(stLSCCaliCfg.u8GridSizeX-1)+i]);
			fprintf(pFileB , "0x%x,\n",stMeshShadingResult.pu8Bgain[j*(stLSCCaliCfg.u8GridSizeX-1)+i]);
            #endif

            fprintf(pFileR , "%d, ",stMeshShadingBayerChannelGain.pu32Rgain [j*u32GridX + i]);
			fprintf(pFileG , "%d, ",stMeshShadingBayerChannelGain.pu32Grgain[j*u32GridX + i]);
			fprintf(pFileB , "%d, ",stMeshShadingBayerChannelGain.pu32Bgain [j*u32GridX + i]);
		}

        fprintf(pFileR,"\n");
        fprintf(pFileG,"\n");
        fprintf(pFileB,"\n");
	}
   #endif 
 
    //Convert Grgain and Gbgain to Ggain
	for(i = 0; i < u32LSCGridPoints; i++)
	{
		stMeshShadingGridTab.pu16Rgain[i] = (HI_U32)(stMeshShadingBayerChannelGain.pu32Rgain [i]);
		stMeshShadingGridTab.pu16Ggain[i] = (HI_U32)((stMeshShadingBayerChannelGain.pu32Grgain[i] 
            + stMeshShadingBayerChannelGain.pu32Gbgain[i])>>1);
		stMeshShadingGridTab.pu16Bgain[i] = (HI_U32)(stMeshShadingBayerChannelGain.pu32Bgain[i]);
	}

    //Step6:Blinear Interp Shading table[65*65] to shading table[64*64]
	S32Ret = Interp_for_GridSizeX_GridSizeY(stMeshShadingGridTab ,stMeshShadingResult,u32GridX,u32GridY);
    if(S32Ret != HI_SUCCESS)
    {
       //free global memory
       free(g_u32GridWidthStep);
       free(g_u32GridHeightStep);
       
       //free memory stMeshShadingBayerChannelGridData
       free(stMeshShadingBayerChannelGridData.pu32R_data);
       free(stMeshShadingBayerChannelGridData.pu32Gr_data);
       free(stMeshShadingBayerChannelGridData.pu32Gb_data);
       free(stMeshShadingBayerChannelGridData.pu32B_data);
   
       //free memory stMeshShadingBayerChannelGain
       free(stMeshShadingBayerChannelGain.pu32Rgain );
       free(stMeshShadingBayerChannelGain.pu32Grgain);
       free(stMeshShadingBayerChannelGain.pu32Gbgain);
       free(stMeshShadingBayerChannelGain.pu32Bgain); 

       //free memory stMeshShadingGridTab
       free(stMeshShadingGridTab.pu16Rgain);
       free(stMeshShadingGridTab.pu16Ggain);
       free(stMeshShadingGridTab.pu16Bgain);

       g_u32GridWidthStep = HI_NULL;
       g_u32GridHeightStep = HI_NULL;

       stMeshShadingBayerChannelGridData.pu32R_data = HI_NULL;
       stMeshShadingBayerChannelGridData.pu32Gr_data = HI_NULL;
       stMeshShadingBayerChannelGridData.pu32Gb_data = HI_NULL;
       stMeshShadingBayerChannelGridData.pu32B_data = HI_NULL;
       
       stMeshShadingBayerChannelGain.pu32Rgain = HI_NULL;
       stMeshShadingBayerChannelGain.pu32Grgain = HI_NULL;
       stMeshShadingBayerChannelGain.pu32Gbgain = HI_NULL;
       stMeshShadingBayerChannelGain.pu32Bgain = HI_NULL;
       
       stMeshShadingGridTab.pu16Rgain = HI_NULL;
       stMeshShadingGridTab.pu16Ggain = HI_NULL;
       stMeshShadingGridTab.pu16Bgain = HI_NULL;
       
       printf("%s: LINE: %d Interp_for_GridSizeX_GridSizeY failure!\n",__FUNCTION__,__LINE__);
       return HI_FAILURE;
    }

    printf("Calibration end!\n");

    //free global memory
    free(g_u32GridWidthStep);
    free(g_u32GridHeightStep);
    
    //free memory stMeshShadingBayerChannelGridData
    free(stMeshShadingBayerChannelGridData.pu32R_data);
    free(stMeshShadingBayerChannelGridData.pu32Gr_data);
    free(stMeshShadingBayerChannelGridData.pu32Gb_data);
    free(stMeshShadingBayerChannelGridData.pu32B_data);
 
    //free memory stMeshShadingBayerChannelGain
    free(stMeshShadingBayerChannelGain.pu32Rgain );
    free(stMeshShadingBayerChannelGain.pu32Grgain);
    free(stMeshShadingBayerChannelGain.pu32Gbgain);
    free(stMeshShadingBayerChannelGain.pu32Bgain); 
 
    //free memory stMeshShadingGridTab
    free(stMeshShadingGridTab.pu16Rgain);
    free(stMeshShadingGridTab.pu16Ggain);
    free(stMeshShadingGridTab.pu16Bgain);

    g_u32GridWidthStep = HI_NULL;
    g_u32GridHeightStep = HI_NULL;

    stMeshShadingBayerChannelGridData.pu32R_data = HI_NULL;
    stMeshShadingBayerChannelGridData.pu32Gr_data = HI_NULL;
    stMeshShadingBayerChannelGridData.pu32Gb_data = HI_NULL;
    stMeshShadingBayerChannelGridData.pu32B_data = HI_NULL;
    
    stMeshShadingBayerChannelGain.pu32Rgain = HI_NULL;
    stMeshShadingBayerChannelGain.pu32Grgain = HI_NULL;
    stMeshShadingBayerChannelGain.pu32Gbgain = HI_NULL;
    stMeshShadingBayerChannelGain.pu32Bgain = HI_NULL;
    
    stMeshShadingGridTab.pu16Rgain = HI_NULL;
    stMeshShadingGridTab.pu16Ggain = HI_NULL;
    stMeshShadingGridTab.pu16Bgain = HI_NULL;
	return HI_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


