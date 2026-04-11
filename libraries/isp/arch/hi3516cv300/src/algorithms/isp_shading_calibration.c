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


#define WINDOWSIZEX (20)
#define WINDOWSIZEY (20)
#define GRIDNUMX    (17)
#define GRIDNUMY    (17)
#define LSC_MAX_PIXEL_VALUE (8191)
#define PROCESSBIT  (20)
#define ELIMINATEPCTLOW  (10)

#define LSCCLIP3(min,max,x)    ((x)< (min) ? (min) : ((x)>(max)?(max):(x)))
#define MAX(a, b) (((a) < (b)) ?  (b) : (a))
#define MIN(a,b)  (((a)>(b))?(b):(a))


typedef struct hi_LSC_Grid_Step_S
{
    HI_U32 *pu32GridWidthStep;
    HI_U32 *pu32GridHeightStep;

}ISP_LSC_Grid_Step_S;


    
typedef struct hi_Mesh_Shading_BayerChannel_Grid_Data_S
{
   HI_U32 *pu32R_data;
   HI_U32 *pu32Gr_data;
   HI_U32 *pu32Gb_data;
   HI_U32 *pu32B_data;
}ISP_Mesh_Shading_BayerChannel_Grid_Data_S;

HI_U32 getMaxData(HI_U32* pu32data, int length)
{
	int i;

	HI_U32 u32MaxData = 0;	
	for (i=0; i<length; i++)
	{
		if (pu32data[i] > u32MaxData)
	    {
			u32MaxData = pu32data[i];
	    }
	}
	return u32MaxData;
}

HI_U32 dataEnlarge(HI_U32 u32Data, HI_U32 u32MaxData, HI_U32 u32LSCMeasure)
{
	
    HI_U32 u32Ratio; //Using unsigned 32 bit to present the ratio;

	u32Ratio = (HI_U32)(((HI_FLOAT)u32MaxData / (HI_FLOAT)DIV_0_TO_1(u32Data))*u32LSCMeasure);  //Precise: 3.10bit

	return u32Ratio;

}

HI_VOID sort(HI_U32 *pu32Array,HI_U32 u32Num)
{
	HI_U32 i,j;
	HI_U32 temp;
	for (i = 0; i < u32Num ; i++)
	{
		for (j = 0; j < (u32Num-1) - i; j++)
		{
			if (pu32Array[j] > pu32Array[j + 1])
			{
				temp = pu32Array[j];
				pu32Array[j] = pu32Array[j + 1];
				pu32Array[j + 1] = temp;
			}
		}
	}
	
}


HI_S32 getLSData(HI_U16* pu16BayerImg, ISP_Mesh_Shading_BayerChannel_Grid_Data_S* pstLSCGridData,ISP_LSC_Grid_Step_S *pstLscGridStepXY, ISP_LSC_CALIBRATION_CFG_S* pstLSCCaliCfg)
{
   
	HI_S32 s32Height   = (HI_S32)pstLSCCaliCfg->u16ImageHeight;
	HI_S32 s32Width    = (HI_S32)pstLSCCaliCfg->u16ImageWidth;
	HI_U32 size_x      = WINDOWSIZEX;//grid size
	HI_U32 size_y      = WINDOWSIZEY;
	HI_U32 grid_x_size = GRIDNUMX;//grid num
	HI_U32 grid_y_size = GRIDNUMY;
	
	//black level
	HI_S32 BLCOffsetR  = (HI_S32)(pstLSCCaliCfg->u16BLCOffsetR<<8);//20bit process bit
	HI_S32 BLCOffsetGr = (HI_S32)(pstLSCCaliCfg->u16BLCOffsetGr<<8);
	HI_S32 BLCOffsetGb = (HI_S32)(pstLSCCaliCfg->u16BLCOffsetGb<<8);
	HI_S32 BLCOffsetB  = (HI_S32)(pstLSCCaliCfg->u16BLCOffsetB<<8);
	
	//HI_U32 u32EliminatePctLow = ELIMINATEPCTLOW;
	HI_U32 u32EliminatePctLow = 10;
	HI_U32 u32EliminatePctHigh = 100 - u32EliminatePctLow;
    HI_U32 fullWidth = s32Width<<1;

    HI_U32 u32Shift = PROCESSBIT - pstLSCCaliCfg->enRawBit;
    
	HI_U32 i,j;
    
	HI_U32 location_y = 0; 
	HI_U32 location_x;

	HI_U32 num,count,numTemp;

	HI_S32 x_start, x_end, y_start, y_end;

	HI_S32 x, y;

	HI_U32 u32SumR,u32SumGr,u32SumGb,u32SumB;

    //malloc memery for four channels of a block
    HI_U32 *pu32DataR   = (HI_U32 *)malloc(sizeof(HI_U32)*WINDOWSIZEX*WINDOWSIZEY);
	HI_U32 *pu32DataGr	= (HI_U32 *)malloc(sizeof(HI_U32)*WINDOWSIZEX*WINDOWSIZEY);
	HI_U32 *pu32DataGb	= (HI_U32 *)malloc(sizeof(HI_U32)*WINDOWSIZEX*WINDOWSIZEY);
	HI_U32 *pu32DataB	= (HI_U32 *)malloc(sizeof(HI_U32)*WINDOWSIZEX*WINDOWSIZEY);
	if((pu32DataR==HI_NULL)||(pu32DataGr==HI_NULL)||(pu32DataGr==HI_NULL)||(pu32DataB==HI_NULL))
	{
	    printf("malloc memery fails in getLSData()! \n");
	    if(pu32DataR)
	    {
	        free(pu32DataR);
	        pu32DataR = HI_NULL;
	    }
	    if(pu32DataGr)
	    {
	        free(pu32DataGr);
	        pu32DataGr = HI_NULL;
	    }
	    if(pu32DataGb)
	    {
	        free(pu32DataGb);
	        pu32DataGb = HI_NULL;
	    }
	    if(pu32DataB)
	    {
	        free(pu32DataB);
	        pu32DataB = HI_NULL;
	    }
	    return HI_FAILURE;
	}

    //printf("%d,%d,%d",);
	for (i=0; i<grid_y_size; i++)
	{   
		location_y = location_y + pstLscGridStepXY->pu32GridHeightStep[i]; 
	    location_x = 0; 
		for (j=0; j<grid_x_size; j++)
		{
			location_x = location_x + pstLscGridStepXY->pu32GridWidthStep[j];
			num = 0;
			
			memset(pu32DataR, 0,size_x*size_y*sizeof(HI_U32));
            memset(pu32DataGr,0,size_x*size_y*sizeof(HI_U32));
            memset(pu32DataGb,0,size_x*size_y*sizeof(HI_U32));
            memset(pu32DataB, 0,size_x*size_y*sizeof(HI_U32));
            
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
			for (y=y_start; y<y_end; y++)
			{
				for (x=x_start; x<x_end; x++)
				{
					switch (pstLSCCaliCfg->enBayer)
					{
					case BAYER_RGGB://RGGB
						pu32DataR[num]  = (HI_U32)MAX(0,((HI_S32)(pu16BayerImg[2*x+2*y*fullWidth]<<u32Shift)            - BLCOffsetR ));
						pu32DataGr[num] = (HI_U32)MAX(0,((HI_S32)(pu16BayerImg[2*x+2*y*fullWidth+1]<<u32Shift)          - BLCOffsetGr));
						pu32DataGb[num] = (HI_U32)MAX(0,((HI_S32)(pu16BayerImg[2*x+2*y*fullWidth+fullWidth]<<u32Shift)  - BLCOffsetGb));
						pu32DataB[num]  = (HI_U32)MAX(0,((HI_S32)(pu16BayerImg[2*x+2*y*fullWidth+fullWidth+1]<<u32Shift)- BLCOffsetB ));
						break;
					case BAYER_GRBG://GRBG 
						pu32DataGr[num]	= (HI_U32)MAX(0,((HI_S32)(pu16BayerImg[2*x+2*y*fullWidth]<<u32Shift)			- BLCOffsetGr));
						pu32DataR[num]	= (HI_U32)MAX(0,((HI_S32)(pu16BayerImg[2*x+2*y*fullWidth+1]<<u32Shift)			- BLCOffsetR ));
						pu32DataB[num]	= (HI_U32)MAX(0,((HI_S32)(pu16BayerImg[2*x+2*y*fullWidth+fullWidth]<<u32Shift)	- BLCOffsetB ));
						pu32DataGb[num]	= (HI_U32)MAX(0,((HI_S32)(pu16BayerImg[2*x+2*y*fullWidth+fullWidth+1]<<u32Shift)- BLCOffsetGb));
						break;
					case BAYER_GBRG://GBRG
						pu32DataGb[num]	= (HI_U32)MAX(0,((HI_S32)(pu16BayerImg[2*x+2*y*fullWidth]<<u32Shift)			- BLCOffsetGb));
						pu32DataB[num]	= (HI_U32)MAX(0,((HI_S32)(pu16BayerImg[2*x+2*y*fullWidth+1]<<u32Shift)			- BLCOffsetB ));
						pu32DataR[num]	= (HI_U32)MAX(0,((HI_S32)(pu16BayerImg[2*x+2*y*fullWidth+fullWidth]<<u32Shift)	- BLCOffsetR ));
						pu32DataGr[num]	= (HI_U32)MAX(0,((HI_S32)(pu16BayerImg[2*x+2*y*fullWidth+fullWidth+1]<<u32Shift)- BLCOffsetGr));
						break;
					case BAYER_BGGR://BGGR
						pu32DataB[num]	= (HI_U32)MAX(0,((HI_S32)(pu16BayerImg[2*x+2*y*fullWidth]<<u32Shift)			- BLCOffsetB ));
						pu32DataGb[num]	= (HI_U32)MAX(0,((HI_S32)(pu16BayerImg[2*x+2*y*fullWidth+1]<<u32Shift)		    - BLCOffsetGb));
						pu32DataGr[num]	= (HI_U32)MAX(0,((HI_S32)(pu16BayerImg[2*x+2*y*fullWidth+fullWidth]<<u32Shift)	- BLCOffsetGr));
						pu32DataR[num]	= (HI_U32)MAX(0,((HI_S32)(pu16BayerImg[2*x+2*y*fullWidth+fullWidth+1]<<u32Shift)- BLCOffsetR ));
						break;
					default:
						break;
					}
					num++;
				}
			}
			//sort data 
            sort(pu32DataR,num);
            sort(pu32DataGr,num);
            sort(pu32DataGb,num);
            sort(pu32DataB,num);

			numTemp	= 0;
			u32SumR	= 0;
			u32SumGr= 0;
			u32SumGb= 0;
			u32SumB	= 0;
			
			for (count = num *u32EliminatePctLow/100;count < num*u32EliminatePctHigh/100;count++)
			{
				u32SumR	+= pu32DataR[count];
				u32SumGr+= pu32DataGr[count];
				u32SumGb+= pu32DataGb[count];
				u32SumB	+= pu32DataB[count];
				numTemp++;
			}
            
			if (numTemp)
			{
			    //average
				pstLSCGridData->pu32B_data[ i*grid_x_size + j]	= u32SumB/numTemp;
				pstLSCGridData->pu32Gb_data[i*grid_x_size + j]	= u32SumGb/numTemp;
				pstLSCGridData->pu32Gr_data[i*grid_x_size + j]	= u32SumGr/numTemp;
				pstLSCGridData->pu32R_data[ i*grid_x_size + j]	= u32SumR/numTemp;
				//printf("%d,",pstLSCGridData->pu32B_data[ i*grid_x_size + j]);
			}
			else
			{
				printf("Something goes wrong in getLSData()!");
				free(pu32DataR);
	            free(pu32DataGr);
	            free(pu32DataGb);
	            free(pu32DataB);
	            
	            pu32DataR = HI_NULL;
                pu32DataGr = HI_NULL;
                pu32DataGb = HI_NULL;
                pu32DataB = HI_NULL;
				return HI_FAILURE;
			}
		}
	}
	free(pu32DataR);
	free(pu32DataGr);
	free(pu32DataGb);
	free(pu32DataB);
	pu32DataR = HI_NULL;
    pu32DataGr = HI_NULL;
    pu32DataGb = HI_NULL;
    pu32DataB = HI_NULL;
	return HI_SUCCESS;
}




HI_S32 lc_normalization(ISP_Mesh_Shading_BayerChannel_Grid_Data_S* pstLSCGridData, ISP_MESH_SHADING_TABLE_S* pstLSCCaliResult, ISP_LSC_CALIBRATION_CFG_S* stLSCCaliCfg)
{

	HI_U32 b_max_data, gb_max_data, gr_max_data, r_max_data;
    HI_U32 u32LSCGridPoints;
    HI_U32 i;


    u32LSCGridPoints = GRIDNUMX*GRIDNUMY;//17*17

    //find the Max data of each channel
	// for b channel
	b_max_data  = getMaxData(pstLSCGridData->pu32B_data, u32LSCGridPoints);
	// for gb channel
	gb_max_data = getMaxData(pstLSCGridData->pu32Gb_data, u32LSCGridPoints);
	// for gr channel
	gr_max_data = getMaxData(pstLSCGridData->pu32Gr_data, u32LSCGridPoints);
	// for r channel
	r_max_data  = getMaxData(pstLSCGridData->pu32R_data, u32LSCGridPoints);
    //printf("\nb_max_data=%d,gb_max_data=%d,gr_max_data=%d,r_max_data=%d\n",b_max_data,gb_max_data,gr_max_data,r_max_data);
    for ( i = 0 ; i < u32LSCGridPoints ; i++ )
    {
        //normalization process
        pstLSCCaliResult->au32BGain[i]  = MIN(dataEnlarge(pstLSCGridData->pu32B_data[i], b_max_data, 1024),LSC_MAX_PIXEL_VALUE);
        pstLSCCaliResult->au32GbGain[i] = MIN(dataEnlarge(pstLSCGridData->pu32Gb_data[i], gb_max_data, 1024),LSC_MAX_PIXEL_VALUE);
        pstLSCCaliResult->au32GrGain[i] = MIN(dataEnlarge(pstLSCGridData->pu32Gr_data[i], gr_max_data, 1024),LSC_MAX_PIXEL_VALUE);
        pstLSCCaliResult->au32RGain[i]  = MIN(dataEnlarge(pstLSCGridData->pu32R_data[i], r_max_data, 1024),LSC_MAX_PIXEL_VALUE);

    }

    return HI_SUCCESS;

}


// Using geometric sequence to automatically generate grid size
HI_S32 GeometricGridSizeX(ISP_LSC_CALIBRATION_CFG_S* pstLSCCaliCfg,ISP_LSC_Grid_Step_S *pstLscGridStepXY,ISP_MESH_SHADING_TABLE_S* pstLSCCaliResult)   
{
   //Parameters Definition
   HI_FLOAT* f16Rx; //Former expression: rx
   HI_FLOAT f16SumR; //Former expression: sum_r
   HI_U32* u32TmpStepX;//Former expression: tmpStepX

   HI_U32 i,sum;
   HI_U32 u32HalfGridSizeX; //Former expression: u32HalfGridSizeX;
   HI_U32 u32Width,diff;//Former expression: u32Width, diff
   
   HI_FLOAT f16R1 = pstLSCCaliCfg->fCommRatioX;
   //Read Image info
   u32HalfGridSizeX = (GRIDNUMX-1)/2;

   u32Width = pstLSCCaliCfg->u16ImageWidth;
   
   //Memory allocate
   f16Rx       = (HI_FLOAT*)malloc(sizeof(float)*u32HalfGridSizeX); 
   u32TmpStepX = (HI_U32*)malloc(sizeof(HI_U32)*u32HalfGridSizeX);

   if (f16Rx==NULL || u32TmpStepX==NULL || u32HalfGridSizeX == 0)
   { 
       //Something goes error!
	   free(f16Rx);
	   free(u32TmpStepX);
	   return(HI_FAILURE);
   }

   //Function start 
   //Horizental direction
   f16Rx[0] = 1.0f;
   for ( i = 1 ; i < u32HalfGridSizeX; i++ )
   {
       f16Rx[i] = f16Rx[i-1]*f16R1;
	   //printf("f16Rx[%d] = %f \n", i, f16Rx[i]);
   }
   f16SumR = 0;
   for ( i = 0 ; i <u32HalfGridSizeX; i++ )
   {
       f16SumR = f16SumR + f16Rx[i];
   }
   for ( i = 0 ; i <u32HalfGridSizeX; i++ )
   {
       u32TmpStepX[i] = (int)(((u32Width/2*1024/DIV_0_TO_1(f16SumR))*f16Rx[i]+(1024>>1))/1024);
	   //printf("u32TmpStepX[%d] = %d \n", i, u32TmpStepX[i]);
   }

   free(f16Rx);
  

   sum = 0;
   for (i=0;i<u32HalfGridSizeX;i++)
   {
      sum=sum+u32TmpStepX[i];
   } 
    if (sum!=(u32Width/2))
    {
		if (sum>(u32Width/2))
		{
			diff=sum-u32Width/2; 
			for (i=1;i<=diff;i++)
				u32TmpStepX[u32HalfGridSizeX-i]= u32TmpStepX[u32HalfGridSizeX-i]-1;
		}
		else
		{
			diff=u32Width/2-sum;
			for (i=1;i<=diff;i++)
				u32TmpStepX[i-1] = u32TmpStepX[i-1]+1;
		}
    }

    //Return the step length value to gridStepWidth and gridStepHeight
    pstLscGridStepXY->pu32GridWidthStep[0] = 0;
    for ( i = 1 ; i <= u32HalfGridSizeX; i++ )
    {
         pstLscGridStepXY->pu32GridWidthStep[i] = u32TmpStepX[i-1];
         pstLscGridStepXY->pu32GridWidthStep[GRIDNUMX - i] = u32TmpStepX[i-1];
         pstLSCCaliResult->au32XGridWidth[i-1] = pstLscGridStepXY->pu32GridWidthStep[i];
         //printf("pstLscGridStepXY->pu32GridWidthStep[%d] = %d\n", i, pstLscGridStepXY->pu32GridWidthStep[i]);
    }


   free(u32TmpStepX);
   return(HI_SUCCESS);

}

void GenerateGridByPercentX(ISP_LSC_CALIBRATION_CFG_S* pstLSCCaliCfg,ISP_LSC_Grid_Step_S *pstLscGridStepXY,ISP_MESH_SHADING_TABLE_S* pstLSCCaliResult)
{
	HI_U32 nHalfSize = (GRIDNUMX-1) / 2;
	
    int i;
	pstLscGridStepXY->pu32GridWidthStep[0] = 0;
	for( i = 0; i < nHalfSize; i++)
	{
		float fPerc = (float)pstLSCCaliCfg->au8GridStepX[i] / 100.0f;
		pstLscGridStepXY->pu32GridWidthStep[i + 1] = (HI_U32)((float)pstLSCCaliCfg->u16ImageWidth * fPerc + 0.5f);
	}
	for( i = nHalfSize; i > 0; i--)
	{
		pstLscGridStepXY->pu32GridWidthStep[i] -= pstLscGridStepXY->pu32GridWidthStep[i - 1];
		pstLSCCaliResult->au32XGridWidth[i-1] = pstLscGridStepXY->pu32GridWidthStep[i];
	}
	for( i = nHalfSize + 1; i < GRIDNUMX; i++)
	{
		pstLscGridStepXY->pu32GridWidthStep[i] = pstLscGridStepXY->pu32GridWidthStep[GRIDNUMX - i];
	}
}


HI_S32 GeometricGridSizeY(ISP_LSC_CALIBRATION_CFG_S* pstLSCCaliCfg,ISP_LSC_Grid_Step_S *pstLscGridStepXY,ISP_MESH_SHADING_TABLE_S* pstLSCCaliResult)   
{
    //param definition
    HI_FLOAT* f16Ry; //Former expression: ry
    HI_FLOAT f16SumR; //Former expression: sum_r
    HI_U32* u32TmpStepY; //Former expression: u32TmpStepY

    HI_U32 j, sum;
    HI_U32 u32HalfGridSizeY; //Former expression: u32HalfGridSizeY
    HI_U32 u32Height, diff;//Former expression: u32Height, diff
   
    //Read Image info
    u32HalfGridSizeY = (GRIDNUMY-1)/2;
    u32Height = pstLSCCaliCfg->u16ImageHeight;
  
    HI_FLOAT f16R1 = pstLSCCaliCfg->fCommRatioY;
   
    //Memory allocate
    f16Ry       = (HI_FLOAT*)malloc(sizeof(HI_FLOAT)*u32HalfGridSizeY);
    u32TmpStepY = (HI_U32*)malloc(sizeof(HI_U32)*u32HalfGridSizeY);
   
    if (f16Ry == NULL || u32TmpStepY == NULL || u32HalfGridSizeY == 0)
    {
	   free(f16Ry);
	   free(u32TmpStepY);
	   return(HI_FAILURE);
    }

    //Function start: Horizental direction
    f16Ry[0] = 1.0f;

    //Vertical direction
    for ( j = 1 ; j < u32HalfGridSizeY; j++ )
    {
        f16Ry[j] = f16Ry[j-1]*f16R1;
    }
    f16SumR = 0;
    for ( j = 0 ; j <u32HalfGridSizeY; j++ )
    {
        f16SumR = f16SumR + f16Ry[j];
    }
    for ( j = 0 ; j <u32HalfGridSizeY; j++ )
    {
        u32TmpStepY[j] = (int)(((u32Height/2*1024/DIV_0_TO_1(f16SumR))*f16Ry[j]+(1024>>1))/1024);
    }
    
	free(f16Ry);

    sum = 0;
    for ( j = 0 ; j <u32HalfGridSizeY; j++ )
    {     
      sum=sum+u32TmpStepY[j];
    }
    if (sum!=(u32Height/2))
    {
         if (sum>(u32Height/2))
         {
             diff=sum-u32Height/2; 
             for (j=1;j<=diff;j++)
                 u32TmpStepY[u32HalfGridSizeY-j]= u32TmpStepY[u32HalfGridSizeY-j]-1;
         }
         else
         {
             diff=u32Height/2-sum;
             for (j=1;j<=diff;j++)
                 u32TmpStepY[j-1] = u32TmpStepY[j-1]+1;
         }
    }

    //Return the step length value to gridStepWidth and gridStepHeight
    pstLscGridStepXY->pu32GridHeightStep[0] = 0;
    for ( j = 1 ; j <= u32HalfGridSizeY; j++ )
    {
        pstLscGridStepXY->pu32GridHeightStep[j] = u32TmpStepY[j-1];
        pstLscGridStepXY->pu32GridHeightStep[GRIDNUMY - j] = u32TmpStepY[j-1];
        pstLSCCaliResult->au32YGridWidth[j-1] = pstLscGridStepXY->pu32GridHeightStep[j];
        //printf("au32GridHeightStep[%d] = %d\n", j, pstLscGridStepXY->pu32GridHeightStep[j]);
    }

	free(u32TmpStepY);
    return(HI_SUCCESS);
}


void GenerateGridByPercentY(ISP_LSC_CALIBRATION_CFG_S* pstLSCCaliCfg,ISP_LSC_Grid_Step_S *pstLscGridStepXY,ISP_MESH_SHADING_TABLE_S* pstLSCCaliResult)
{
	HI_U32 nHalfSize = (HI_U32)GRIDNUMY / 2;
    int i;
	pstLscGridStepXY->pu32GridHeightStep[0] = 0;
	for( i = 0; i < nHalfSize; i++)
	{
		float fPerc = (float)pstLSCCaliCfg->au8GridStepY[i] / 100.0f;
		pstLscGridStepXY->pu32GridHeightStep[i + 1] = (HI_U32)((float)pstLSCCaliCfg->u16ImageHeight* fPerc + 0.5f);
	}
	for( i = nHalfSize; i > 0; i--)
	{
		pstLscGridStepXY->pu32GridHeightStep[i] -= pstLscGridStepXY->pu32GridHeightStep[i - 1];
		pstLSCCaliResult->au32YGridWidth[i-1] = pstLscGridStepXY->pu32GridHeightStep[i];
	}
	for( i = nHalfSize + 1; i < GRIDNUMY; i++)
	{
		pstLscGridStepXY->pu32GridHeightStep[i] = pstLscGridStepXY->pu32GridHeightStep[GRIDNUMY - i];
	}
}


HI_S32 LSC_GenerateGridInfo(HI_U16* pu16Data, ISP_LSC_CALIBRATION_CFG_S* pstLSCCaliCfg, ISP_LSC_Grid_Step_S *pstLscGridStepXY,ISP_MESH_SHADING_TABLE_S* pstLSCCaliResult)
{
    //Pass all the input params to the function
    ISP_Mesh_Shading_BayerChannel_Grid_Data_S stLSData;

	HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32LSCGridPoints;


    u32LSCGridPoints = SHADING_MESH_NUM;
    
    //memory initial
    stLSData.pu32B_data= (HI_U32*)malloc(sizeof(HI_U32)*u32LSCGridPoints);
    stLSData.pu32Gb_data= (HI_U32*)malloc(sizeof(HI_U32)*u32LSCGridPoints);
    stLSData.pu32Gr_data= (HI_U32*)malloc(sizeof(HI_U32)*u32LSCGridPoints);
    stLSData.pu32R_data= (HI_U32*)malloc(sizeof(HI_U32)*u32LSCGridPoints);

    if((HI_NULL == stLSData.pu32B_data)||(HI_NULL == stLSData.pu32Gb_data)||(HI_NULL == stLSData.pu32Gr_data)||(HI_NULL == stLSData.pu32R_data))
    {
        printf("malloc memory for LSData fails in LSC_GenerateGridInfo()!\n");
        if(stLSData.pu32B_data)
        {
            free(stLSData.pu32B_data);
            stLSData.pu32B_data = HI_NULL;
        }
        if(stLSData.pu32Gb_data)
        {
            free(stLSData.pu32Gb_data);
            stLSData.pu32Gb_data = HI_NULL;
        }
        if(stLSData.pu32Gr_data)
        {
            free(stLSData.pu32Gr_data);
            stLSData.pu32Gr_data = HI_NULL;
        }
        if(stLSData.pu32R_data)
        {
            free(stLSData.pu32R_data);
            stLSData.pu32R_data = HI_NULL;
        }
        return HI_FAILURE;
    }


    //get Lens Correction Coefficients
    s32Ret = getLSData(pu16Data, &stLSData,pstLscGridStepXY, pstLSCCaliCfg); 
    if(s32Ret != HI_SUCCESS)
    {
        printf("There are some errors in getLSData()!\n");
        if(stLSData.pu32B_data)
        {
            free(stLSData.pu32B_data);
            stLSData.pu32B_data = HI_NULL;
        }
        if(stLSData.pu32Gb_data)
        {
            free(stLSData.pu32Gb_data);
            stLSData.pu32Gb_data = HI_NULL;
        }
        if(stLSData.pu32Gr_data)
        {
            free(stLSData.pu32Gr_data);
            stLSData.pu32Gr_data = HI_NULL;
        }
        if(stLSData.pu32R_data)
        {
            free(stLSData.pu32R_data);
            stLSData.pu32R_data = HI_NULL;
        }
        return HI_FAILURE;
    }

	s32Ret = lc_normalization(&stLSData, pstLSCCaliResult, pstLSCCaliCfg);
	

    //Free
    free(stLSData.pu32B_data);
	free(stLSData.pu32Gb_data);
	free(stLSData.pu32Gr_data);
	free(stLSData.pu32R_data);
	stLSData.pu32R_data = HI_NULL;
	stLSData.pu32Gb_data = HI_NULL;
	stLSData.pu32Gr_data = HI_NULL;
	stLSData.pu32B_data = HI_NULL;
	return s32Ret;
   
}

HI_S32 ISP_MeshShadingCalibration(HI_U16 *pu16SrcRaw, 
    ISP_LSC_CALIBRATION_CFG_S *pstLSCCaliCfg , ISP_MESH_SHADING_TABLE_S *pstMeshShadingResult)
{


    HI_S32 S32Ret = HI_SUCCESS;

    ISP_LSC_Grid_Step_S stLscGridStepXY;
    pstLSCCaliCfg->u16ImageWidth /= 2;
    pstLSCCaliCfg->u16ImageHeight/= 2;
    //malloc memory 
    stLscGridStepXY.pu32GridWidthStep = (HI_U32*)malloc(sizeof(HI_U32)*GRIDNUMX);
    stLscGridStepXY.pu32GridHeightStep= (HI_U32*)malloc(sizeof(HI_U32)*GRIDNUMY);
    if((stLscGridStepXY.pu32GridWidthStep==HI_NULL)||(stLscGridStepXY.pu32GridHeightStep==HI_NULL))
    {
        printf("malloc memory for stLscGridStepXY fails in ISP_MeshShadingCalibration()!\n");
        if(stLscGridStepXY.pu32GridWidthStep)
        {
            free(stLscGridStepXY.pu32GridWidthStep);
            stLscGridStepXY.pu32GridWidthStep = HI_NULL;
        }
        if(stLscGridStepXY.pu32GridHeightStep)
        {
            free(stLscGridStepXY.pu32GridHeightStep);
            stLscGridStepXY.pu32GridHeightStep = HI_NULL;
        }
        return HI_FAILURE; 
    }
    
    if (LSC_GS_GEOMETRIC == pstLSCCaliCfg->enGSModeX)
	{
	   // pstLSCCaliCfg->fCommRatioX = (int)(pstLSCCaliCfg->fCommRatioX * 10 +0.5) / 10.0;
		S32Ret = GeometricGridSizeX(pstLSCCaliCfg,&stLscGridStepXY, pstMeshShadingResult);
		if(S32Ret != HI_SUCCESS)
		{
		    printf("There are some errors in GeometricGridSizeX()!\n");
            if(stLscGridStepXY.pu32GridWidthStep)
            {
                free(stLscGridStepXY.pu32GridWidthStep);
                stLscGridStepXY.pu32GridWidthStep = HI_NULL;
            }
            if(stLscGridStepXY.pu32GridHeightStep)
            {
                free(stLscGridStepXY.pu32GridHeightStep);
                stLscGridStepXY.pu32GridHeightStep = HI_NULL;
            }
            return HI_FAILURE; 
		}
	}
	else // LSC_GS_MANUAL X
	{
		GenerateGridByPercentX(pstLSCCaliCfg, &stLscGridStepXY, pstMeshShadingResult);
	}
	if (LSC_GS_GEOMETRIC == pstLSCCaliCfg->enGSModeY)
	{
		S32Ret = GeometricGridSizeY(pstLSCCaliCfg, &stLscGridStepXY, pstMeshShadingResult);
		if(S32Ret != HI_SUCCESS)
		{
		    printf("There are some errors in GeometricGridSizeY()!\n");
            if(stLscGridStepXY.pu32GridWidthStep)
            {
                free(stLscGridStepXY.pu32GridWidthStep);
                stLscGridStepXY.pu32GridWidthStep = HI_NULL;
            }
            if(stLscGridStepXY.pu32GridHeightStep)
            {
                free(stLscGridStepXY.pu32GridHeightStep);
                stLscGridStepXY.pu32GridHeightStep = HI_NULL;
            }
            return HI_FAILURE; 
		}
	}
	else // LSC_GS_MANUAL Y
	{
		GenerateGridByPercentY(pstLSCCaliCfg, &stLscGridStepXY, pstMeshShadingResult);
	}


    S32Ret = LSC_GenerateGridInfo(pu16SrcRaw,pstLSCCaliCfg,&stLscGridStepXY,pstMeshShadingResult);
    if(S32Ret == HI_FAILURE)
    {
        printf("There are some errors in LSC_GenerateGridInfo()!\n");
        if(stLscGridStepXY.pu32GridWidthStep)
        {
            free(stLscGridStepXY.pu32GridWidthStep);
            stLscGridStepXY.pu32GridWidthStep = HI_NULL;
        }
        if(stLscGridStepXY.pu32GridHeightStep)
        {
            free(stLscGridStepXY.pu32GridHeightStep);
            stLscGridStepXY.pu32GridHeightStep = HI_NULL;
        }
        return HI_FAILURE;
    }
    
    if(stLscGridStepXY.pu32GridWidthStep)
    {
        free(stLscGridStepXY.pu32GridWidthStep);
        stLscGridStepXY.pu32GridWidthStep = HI_NULL;
    }
    if(stLscGridStepXY.pu32GridHeightStep)
    {
        free(stLscGridStepXY.pu32GridHeightStep);
        stLscGridStepXY.pu32GridHeightStep = HI_NULL;
    }

	return S32Ret;
}




#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


