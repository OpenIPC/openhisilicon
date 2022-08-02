/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_ext_config.h"
#include "isp_config.h"
#include "isp_proc.h"
#include "isp_math_utils.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define LSC_GRID_ROWS       17
#define LSC_GRID_COLS       17
#define LSC_GRID_POINTS     (LSC_GRID_ROWS*LSC_GRID_COLS)
#define LSC_MEASURE_Q       1024
#define WINDOWSIZEX         20
#define WINDOWSIZEY         20

#define LSC_MAX_PIXEL_VALUE ((1<<20) - 1)

typedef struct _LSC_Grid_Step_S {
    GK_U32 au32GridWidthStep[LSC_GRID_COLS]; // Former expression: gridWidthStep[];
    GK_U32 au32GridHeightStep[LSC_GRID_ROWS]; // Former expression: gridHeightStep[];

} ISP_LSC_Grid_Step_S;


typedef struct LS_DATA {
    GK_U32 au32R_data[LSC_GRID_POINTS];
    GK_U32 au32Gr_data[LSC_GRID_POINTS];
    GK_U32 au32Gb_data[LSC_GRID_POINTS];
    GK_U32 au32B_data[LSC_GRID_POINTS];
} LS_DATA;

static const GK_FLOAT g_afMaxGainArray[8] = {
    (GK_FLOAT)1023 / (GK_FLOAT)512,     // 1.9
    (GK_FLOAT)1023 / (GK_FLOAT)256,     // 2.8
    (GK_FLOAT)1023 / (GK_FLOAT)128,     // 3.7
    (GK_FLOAT)1023 / (GK_FLOAT)64,      // 4.6
    (GK_FLOAT)1023 / (GK_FLOAT)1024 + 1, // 0.10
    (GK_FLOAT)1023 / (GK_FLOAT)512 + 1, // 1.9
    (GK_FLOAT)1023 / (GK_FLOAT)256 + 1, // 2.8
    (GK_FLOAT)1023 / (GK_FLOAT)128 + 1, // 3.7
};

/*
 * [getMaxData description]
 * @param pu32data   [Input] Input array
 * @param length     [Input] array length
 * Get the maxmum data from the input pu32data array
 */
static GK_U32 getMaxData(GK_U32 *pu32data, int length)
{
    int i;

    GK_U32 u32MaxData = 0;
    for (i = 0; i < length; i++) {
        if (pu32data[i] > u32MaxData) {
            u32MaxData = pu32data[i];
        }
    }
    return u32MaxData;
}

/*
 * [getMinData description]
 * @param pu32data   [Input] Input array
 * @param length     [Input] array length
 * Get the minmum data from the input pu32data array
 */
static GK_U32 getMinData(GK_U32 *pu32data, int length)
{
    int i;

    GK_U32 u32MinData = LSC_MAX_PIXEL_VALUE;
    for (i = 0; i < length; i++) {
        if (pu32data[i] < u32MinData) {
            u32MinData = pu32data[i];
        }
    }
    return u32MinData;
}

/*
 * [dataEnlarge description]
 * @param u32Data        [Input] Input Data
 * @param u32MaxData     [Input] Input Target value
 * @param u32MeshScale   [Input] Input scale value
 * Generate gain value at each grid point, uniformed by input u32MeshScale
 */
static GK_U32 dataEnlarge(GK_U32 u32Data, GK_U32 u32MaxData, GK_U32 u32MeshScale)
{

    GK_U32 ratio; // Using unsigned 32 bit to present the ratio;
    GK_U32 u32LscQValue;
    if (u32MeshScale < 4) {
        u32LscQValue = 1U << (9 - u32MeshScale);
        ratio = (GK_U32)(((GK_FLOAT)u32MaxData / (GK_FLOAT)DIV_0_TO_1(u32Data)) * u32LscQValue);
    } else {
        u32LscQValue = 1U << (14 - u32MeshScale);
        ratio = (GK_U32)(((GK_FLOAT)u32MaxData / (GK_FLOAT)DIV_0_TO_1(u32Data) - 1) * u32LscQValue);
    }

    return MIN2(ratio, 1023);

}

/*
 * [sort description]
 * @param pu32Array  [Input] Input array
 * @param u32Num     [Input] array length
 * Sort the input array from min to max
 */
static GK_VOID sort(GK_U32 *pu32Array, GK_U32 u32Num)
{
    GK_U32 i, j;
    GK_U32 temp;
    for (i = 0; i < u32Num; i++) {
        for (j = 0; j < (u32Num - 1) - i; j++) {
            if (pu32Array[j] > pu32Array[j + 1]) {
                temp = pu32Array[j];
                pu32Array[j] = pu32Array[j + 1];
                pu32Array[j + 1] = temp;
            }
        }
    }

}

/*
 * [Get_Lsc_Data_Channel description]
 * @param pBayerImg        [Input]  Input image data
 * @param pu32GridData     [Output] MeshLsc Grid data generated
 * @param stLSCCaliCfg     [Input]  Mesh LSC Calibration configure
 * @param pstLscGridStepXY [Input]  Input Grid width&height information
 * @param u16BlcOffset     [Input]  Input BLC value
 * @param u8Value          [Input]  Indicate channel number: 0-R, 1-Gr, 2-Gb, 3-B
 */
static GK_S32 Get_Lsc_Data_Channel(GK_U16 *pBayerImg, GK_U32 *pu32GridData, ISP_MLSC_CALIBRATION_CFG_S *stLSCCaliCfg, ISP_LSC_Grid_Step_S *pstLscGridStepXY, GK_U16 u16BlcOffset, GK_U8 u8Value)
{
    GK_S32 s32Height   = (GK_S32)stLSCCaliCfg->u16DstImgHeight / 2;
    GK_S32 s32Width    = (GK_S32)stLSCCaliCfg->u16DstImgWidth / 2;
    GK_U32 size_x      = WINDOWSIZEX;
    GK_U32 size_y      = WINDOWSIZEY;
    GK_U32 grid_x_size = LSC_GRID_COLS;
    GK_U32 grid_y_size = LSC_GRID_ROWS;
    GK_S32 s32TrueBlcOffset;
    GK_U32 u32EliminatePctLow = 10;
    GK_U32 u32EliminatePctHigh = 90;
    GK_U32 fullWidth = ((GK_U32) s32Width) << 1;
    GK_U32 u32Stride = fullWidth;
    GK_U32 i, j;

    GK_U32 location_y = 0; // Added
    GK_U32 location_x;

    GK_U32 num, count, numTemp;
    GK_S32 x_start, x_end, y_start, y_end;

    GK_S32 x, y;

    // test
    GK_U32 pu32Data[400] = {0};

    GK_U32 u32Sum;
    GK_U32 u32CoorIndex = 0;
    GK_U32 u32ChnIndex  = 0;
    GK_U32 h, w;

    if (GK_NULL == pBayerImg || GK_NULL == pu32GridData) {
        ISP_TRACE(MODULE_DBG_ERR, "%s: LINE: %d pBayerImg/pls_data is NULL point !\n", __FUNCTION__, __LINE__);
        return GK_FAILURE;
    }

    u32ChnIndex = (stLSCCaliCfg->enBayer & 0x3) ^ u8Value;

    w = u32ChnIndex & 0x1;
    h = (u32ChnIndex >> 1) & 0x1;

    s32TrueBlcOffset  = (GK_S32)(((GK_U32)u16BlcOffset) << 8); // Input BLC will always be 12 bits

    for (i = 0; i < grid_y_size; i++) {
        location_y = location_y + pstLscGridStepXY->au32GridHeightStep[i];
        location_x = 0;
        for (j = 0; j < grid_x_size; j++) {
            location_x = location_x + pstLscGridStepXY->au32GridWidthStep[j];
            num = 0;

            x_start = location_x - size_x / 2;
            x_end   = location_x + size_x / 2;
            y_start = location_y - size_y / 2;
            y_end   = location_y + size_y / 2;

            if (x_start < 0) {
                x_start = 0;
            }
            if (x_end >= s32Width) {
                x_end = s32Width - 1;
            }
            if (y_start < 0) {
                y_start = 0;
            }
            if (y_end >= s32Height) {
                y_end = s32Height - 1;
            }

            for (y = y_start; y < y_end; y++) {
                for (x = x_start; x < x_end; x++) {
                    u32CoorIndex = 2 * x + 2 * y * u32Stride;
                    pu32Data[num] = (GK_U32)MAX2(0, ((GK_S32)((GK_U32)(pBayerImg[u32CoorIndex + w + h * u32Stride]) << (20 - stLSCCaliCfg->enRawBit)) - s32TrueBlcOffset));
                    num++;
                }
            }

            sort(pu32Data, num);

            numTemp = 0;
            u32Sum  = 0;

            for (count = num * u32EliminatePctLow / 100; count < num * u32EliminatePctHigh / 100; count++) {
                u32Sum  += pu32Data[count];

                numTemp++;
            }

            if (numTemp) {
                pu32GridData[ i * grid_x_size + j]    = u32Sum / numTemp;
            } else {
                ISP_TRACE(MODULE_DBG_ERR, "Something goes wrong in getLSData()!");
                return GK_FAILURE;
            }
        }
    }

    return GK_SUCCESS;
}


/*
 * [getLSData description]
 * @param BayerImg         [Input]  Input image data
 * @param pstLSCGridData   [Output] MeshLsc Grid data generated
 * @param pstLscGridStepXY [Input]  Input Grid width&height information
 * @param pstLSCCaliCfg    [Input]  MeshLsc Calibration results
 */
static GK_S32 getLSData(GK_U16 *BayerImg, LS_DATA *pstLSCGridData, ISP_LSC_Grid_Step_S *pstLscGridStepXY, ISP_MLSC_CALIBRATION_CFG_S *pstLSCCaliCfg)
{
    GK_S32 s32Ret = GK_FAILURE;

    s32Ret = Get_Lsc_Data_Channel(BayerImg, pstLSCGridData->au32R_data, pstLSCCaliCfg, pstLscGridStepXY, pstLSCCaliCfg->u16BLCOffsetR, 0);

    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "%s: LINE: %d Get_Lsc_Data of R Channel failure !\n", __FUNCTION__, __LINE__);
        return GK_FAILURE;
    }

    s32Ret = Get_Lsc_Data_Channel(BayerImg, pstLSCGridData->au32Gr_data, pstLSCCaliCfg, pstLscGridStepXY, pstLSCCaliCfg->u16BLCOffsetGr, 1);

    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "%s: LINE: %d Get_Lsc_Data of R Channel failure !\n", __FUNCTION__, __LINE__);
        return GK_FAILURE;
    }

    s32Ret = Get_Lsc_Data_Channel(BayerImg, pstLSCGridData->au32Gb_data, pstLSCCaliCfg, pstLscGridStepXY, pstLSCCaliCfg->u16BLCOffsetGb, 2);

    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "%s: LINE: %d Get_Lsc_Data of R Channel failure !\n", __FUNCTION__, __LINE__);
        return GK_FAILURE;
    }

    s32Ret = Get_Lsc_Data_Channel(BayerImg, pstLSCGridData->au32B_data, pstLSCCaliCfg, pstLscGridStepXY, pstLSCCaliCfg->u16BLCOffsetB, 3);

    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "%s: LINE: %d Get_Lsc_Data of R Channel failure !\n", __FUNCTION__, __LINE__);
        return GK_FAILURE;
    }

    return GK_SUCCESS;
}

/*
 * [lc_normalization description]
 * @param ls_data            [Input]  Input Grid Data
 * @param u32LSCGridPoints   [Input] MeshLsc Calibration results
 * @param u32MeshScale       [Input]  Input meshLsc calibration config
 * This function is used for testing if the input mesh scale is suitable for the current condition
 */
static GK_VOID MeshScaleTest(LS_DATA *ls_data, GK_U32 u32LSCGridPoints, GK_U32 u32MeshScale)
{
    GK_U32 b_max_data, gb_max_data, gr_max_data, r_max_data;
    GK_U32 b_min_data, gb_min_data, gr_min_data, r_min_data;
    GK_FLOAT b_max_gain, gb_max_gain, gr_max_gain, r_max_gain;
    GK_FLOAT max_gain;

    // for b channel
    b_max_data  = getMaxData(ls_data->au32B_data, u32LSCGridPoints);
    // for gb channel
    gb_max_data = getMaxData(ls_data->au32Gb_data, u32LSCGridPoints);
    // for gr channel
    gr_max_data = getMaxData(ls_data->au32Gr_data, u32LSCGridPoints);
    // for r channel
    r_max_data  = getMaxData(ls_data->au32R_data, u32LSCGridPoints);


    // for b channel
    b_min_data  = getMinData(ls_data->au32B_data, u32LSCGridPoints);
    // for gb channel
    gb_min_data = getMinData(ls_data->au32Gb_data, u32LSCGridPoints);
    // for gr channel
    gr_min_data = getMinData(ls_data->au32Gr_data, u32LSCGridPoints);
    // for r channel
    r_min_data  = getMinData(ls_data->au32R_data, u32LSCGridPoints);

    b_max_gain  = (GK_FLOAT)b_max_data  / (GK_FLOAT)b_min_data;
    gb_max_gain = (GK_FLOAT)gb_max_data / (GK_FLOAT)gb_min_data;
    gr_max_gain = (GK_FLOAT)gr_max_data / (GK_FLOAT)gr_min_data;
    r_max_gain  = (GK_FLOAT)r_max_data  / (GK_FLOAT)r_min_data;

    max_gain = MAX2(MAX3(b_max_gain, gb_max_gain, gr_max_gain), r_max_gain);


    if (max_gain > g_afMaxGainArray[u32MeshScale]) {
        printf("WARNING:\n");
        printf("Max gain = %f\n", max_gain);

        if (u32MeshScale < 4) { // min gain's value is 0
            if ((max_gain > g_afMaxGainArray[0]) && (max_gain <= g_afMaxGainArray[1])) { // (2,4]
                printf("Please set Mesh scale to %d\n", 1);
            } else if ((max_gain > g_afMaxGainArray[1]) && (max_gain <= g_afMaxGainArray[2])) { // (4,8]
                printf("Please set Mesh scale to %d\n", 2);
            } else if ((max_gain > g_afMaxGainArray[2]) && (max_gain <= g_afMaxGainArray[3])) { // (8,16]
                printf("Please set Mesh scale to %d\n", 3);
            } else { //  >16
                printf("Max gain is too large to find accurate mesh scale,please set Mesh Scale to %d to reduce loss\n", 3);
            }
        } else { // max gain's value is 1
            if ((max_gain > g_afMaxGainArray[4]) && (max_gain <= g_afMaxGainArray[5])) { // (2,3]
                printf("Please set Mesh scale to %d\n", 5);
            } else if ((max_gain > g_afMaxGainArray[5]) && (max_gain <= g_afMaxGainArray[6])) { // (3,5]
                printf("Please set Mesh scale to %d\n", 6);
            } else if ((max_gain > g_afMaxGainArray[6]) && (max_gain <= g_afMaxGainArray[7])) { // (5,9]
                printf("Please set Mesh scale to %d\n", 7);
            } else if ((max_gain > g_afMaxGainArray[7]) && (max_gain <= g_afMaxGainArray[3])) { // (9,16]
                printf("Please set Mesh scale to %d\n", 3);
            } else { // >16
                printf("Max gain is too large to find accurate mesh scale,please set Mesh Scale to %d to reduce loss\n", 3);
            }
        }
    }

    return;
}


/*
 * [lc_normalization description]
 * @param pstLSCGridData     [Input]  Input Grid Data
 * @param pstLSCCaliResult   [Output] MeshLsc Calibration results
 * @param pstLSCCaliCfg      [Input]  Input meshLsc calibration config
 */
static GK_VOID lc_normalization(LS_DATA *pstLSCGridData, ISP_MESH_SHADING_TABLE_S *pstLSCCaliResult, ISP_MLSC_CALIBRATION_CFG_S *pstLSCCaliCfg)
{

    GK_U32 b_max_data, gb_max_data, gr_max_data, r_max_data;
    GK_U32 u32LSCGridPoints;
    GK_U32 i;

    u32LSCGridPoints = LSC_GRID_POINTS;

    MeshScaleTest(pstLSCGridData, LSC_GRID_POINTS, pstLSCCaliCfg->u32MeshScale);

    // find the Max data of each channel
    // for b channel
    b_max_data  = getMaxData(pstLSCGridData->au32B_data, u32LSCGridPoints);
    // for gb channel
    gb_max_data = getMaxData(pstLSCGridData->au32Gb_data, u32LSCGridPoints);
    // for gr channel
    gr_max_data = getMaxData(pstLSCGridData->au32Gr_data, u32LSCGridPoints);
    // for r channel
    r_max_data  = getMaxData(pstLSCGridData->au32R_data, u32LSCGridPoints);
    for (i = 0; i < u32LSCGridPoints; i++) {
        // normalization process
        pstLSCCaliResult->stLscGainLut.au16BGain[i]  = (GK_U16)(MIN2(dataEnlarge(pstLSCGridData->au32B_data[i], b_max_data, pstLSCCaliCfg->u32MeshScale), LSC_MAX_PIXEL_VALUE));
        pstLSCCaliResult->stLscGainLut.au16GbGain[i] = (GK_U16)(MIN2(dataEnlarge(pstLSCGridData->au32Gb_data[i], gb_max_data, pstLSCCaliCfg->u32MeshScale), LSC_MAX_PIXEL_VALUE));
        pstLSCCaliResult->stLscGainLut.au16GrGain[i] = (GK_U16)(MIN2(dataEnlarge(pstLSCGridData->au32Gr_data[i], gr_max_data, pstLSCCaliCfg->u32MeshScale), LSC_MAX_PIXEL_VALUE));
        pstLSCCaliResult->stLscGainLut.au16RGain[i]  = (GK_U16)(MIN2(dataEnlarge(pstLSCGridData->au32R_data[i], r_max_data, pstLSCCaliCfg->u32MeshScale), LSC_MAX_PIXEL_VALUE));

    }
    pstLSCCaliResult->u8MeshScale = pstLSCCaliCfg->u32MeshScale;

    return;

}


/*
 * [GeometricGridSizeY description]
 * @param pstLSCCaliCfg      [Input]  Input meshLsc calibration config
 * @param pstLscGridStepXY   [Output]  Grid width&height info
 * @param pstLSCCaliResult   [Output] MeshLsc Calibration results
 */
static GK_VOID GeometricGridSizeX(ISP_MLSC_CALIBRATION_CFG_S *pstLSCCaliCfg, ISP_LSC_Grid_Step_S *pstLscGridStepXY, ISP_MESH_SHADING_TABLE_S *pstLSCCaliResult)
{
    // Parameters Definition
    GK_U32   u32TmpStepX[(LSC_GRID_COLS - 1) / 2]; // Former expression: tmpStepX
    GK_FLOAT f16Rx[(LSC_GRID_COLS - 1) / 2]; // Former expression: rx
    GK_FLOAT f16SumR; // Former expression: sum_r

    GK_U32 i, sum;
    GK_U32 u32HalfGridSizeX = (LSC_GRID_COLS - 1) / 2; // Former expression: u32HalfGridSizeX;
    GK_U32 u32Width, diff; // Former expression: u32Width, diff

    GK_FLOAT f16R1 = 1.0f;

    // Read Image info
    u32Width = pstLSCCaliCfg->u16DstImgWidth / 2;

    // Function start
    // Horizental direction
    f16Rx[0] = 1.0f;
    for (i = 1; i < u32HalfGridSizeX; i++) {
        f16Rx[i] = f16Rx[i - 1] * f16R1;
    }
    f16SumR = 0;
    for (i = 0; i < u32HalfGridSizeX; i++) {
        f16SumR = f16SumR + f16Rx[i];
    }
    for (i = 0; i < u32HalfGridSizeX; i++) {
        u32TmpStepX[i] = (int)(((u32Width / 2 * 1024 / DIV_0_TO_1(f16SumR)) * f16Rx[i] + (1024 >> 1)) / 1024);
    }

    sum = 0;
    for (i = 0; i < u32HalfGridSizeX; i++) {
        sum = sum + u32TmpStepX[i];
    }
    if (sum != (u32Width / 2)) {
        if (sum > (u32Width / 2)) {
            diff = sum - u32Width / 2;
            for (i = 1; i <= diff; i++) {
                u32TmpStepX[u32HalfGridSizeX - i] = u32TmpStepX[u32HalfGridSizeX - i] - 1;
            }
        } else {
            diff = u32Width / 2 - sum;
            for (i = 1; i <= diff; i++) {
                u32TmpStepX[i - 1] = u32TmpStepX[i - 1] + 1;
            }
        }
    }

    // Return the step length value to gridStepWidth and gridStepHeight
    pstLscGridStepXY->au32GridWidthStep[0] = 0;
    for (i = 1; i <= u32HalfGridSizeX; i++) {
        pstLscGridStepXY->au32GridWidthStep[i] = u32TmpStepX[i - 1];
        pstLscGridStepXY->au32GridWidthStep[LSC_GRID_COLS - i] = u32TmpStepX[i - 1];
        pstLSCCaliResult->au16XGridWidth[i - 1] = pstLscGridStepXY->au32GridWidthStep[i];
    }

    return;
}


/*
 * [GeometricGridSizeY description]
 * @param pstLSCCaliCfg      [Input]  Input meshLsc calibration config
 * @param pstLscGridStepXY   [Output]  Grid width&height info
 * @param pstLSCCaliResult   [Output] MeshLsc Calibration results
 */
static GK_VOID GeometricGridSizeY(ISP_MLSC_CALIBRATION_CFG_S *pstLSCCaliCfg, ISP_LSC_Grid_Step_S *pstLscGridStepXY, ISP_MESH_SHADING_TABLE_S *pstLSCCaliResult)
{
    // param definition
    GK_FLOAT f16Ry[(LSC_GRID_ROWS - 1) / 2]; // Former expression: ry
    GK_FLOAT f16SumR; // Former expression: sum_r
    GK_U32   u32TmpStepY[(LSC_GRID_ROWS - 1) / 2]; // Former expression: u32TmpStepY

    GK_U32 j, sum;
    GK_U32 u32HalfGridSizeY = (LSC_GRID_ROWS - 1) / 2; // Former expression: u32HalfGridSizeY
    GK_U32 u32Height, diff; // Former expression: u32Height, diff

    // Read Image info
    u32Height = pstLSCCaliCfg->u16DstImgHeight / 2;

    GK_FLOAT f16R1 = 1.0f;

    // Function start: Horizental direction
    f16Ry[0] = 1.0f;

    // Vertical direction
    for (j = 1; j < u32HalfGridSizeY; j++) {
        f16Ry[j] = f16Ry[j - 1] * f16R1;
    }
    f16SumR = 0;
    for (j = 0; j < u32HalfGridSizeY; j++) {
        f16SumR = f16SumR + f16Ry[j];
    }
    for (j = 0; j < u32HalfGridSizeY; j++) {
        u32TmpStepY[j] = (int)(((u32Height / 2 * 1024 / DIV_0_TO_1(f16SumR)) * f16Ry[j] + (1024 >> 1)) / 1024);
    }

    sum = 0;
    for (j = 0; j < u32HalfGridSizeY; j++) {
        sum = sum + u32TmpStepY[j];
    }
    if (sum != (u32Height / 2)) {
        if (sum > (u32Height / 2)) {
            diff = sum - u32Height / 2;
            for (j = 1; j <= diff; j++) {
                u32TmpStepY[u32HalfGridSizeY - j] = u32TmpStepY[u32HalfGridSizeY - j] - 1;
            }
        } else {
            diff = u32Height / 2 - sum;
            for (j = 1; j <= diff; j++) {
                u32TmpStepY[j - 1] = u32TmpStepY[j - 1] + 1;
            }
        }
    }

    // Return the step length value to gridStepWidth and gridStepHeight
    pstLscGridStepXY->au32GridHeightStep[0] = 0;
    for (j = 1; j <= u32HalfGridSizeY; j++) {
        pstLscGridStepXY->au32GridHeightStep[j] = u32TmpStepY[j - 1];
        pstLscGridStepXY->au32GridHeightStep[LSC_GRID_ROWS - j] = u32TmpStepY[j - 1];
        pstLSCCaliResult->au16YGridWidth[j - 1] = pstLscGridStepXY->au32GridHeightStep[j];
    }

    return;
}

/*
 * [LSC_GenerateGridInfo description]
 * @param pu16Data           [Input]  Input Raw Data that used for calibration
 * @param pstLSCCaliCfg      [Input]  Input meshLsc calibration config
 * @param pstLscGridStepXY   [Input]  Grid width&height info
 * @param pstLSCCaliResult   [Output] MeshLsc Calibration results
 */
static GK_S32 LSC_GenerateGridInfo(GK_U16 *pu16Data, ISP_MLSC_CALIBRATION_CFG_S *pstLSCCaliCfg, ISP_LSC_Grid_Step_S *pstLscGridStepXY, ISP_MESH_SHADING_TABLE_S *pstLSCCaliResult)
{
    // Pass all the input params to the function
    LS_DATA *pstLSData;
    GK_S32 s32Ret = GK_SUCCESS;

    // memory allocation
    pstLSData = (LS_DATA *)ISP_MALLOC(sizeof(LS_DATA));
    if (pstLSData == GK_NULL) {
        ISP_TRACE(MODULE_DBG_ERR, "pstLSData allocation fail!\n");
        return GK_FAILURE;
    }

    memset(pstLSData, 0, sizeof(LS_DATA));

    // get Lens Correction Coefficients
    s32Ret = getLSData(pu16Data, pstLSData, pstLscGridStepXY, pstLSCCaliCfg);
    if (s32Ret != GK_SUCCESS) {
        ISP_TRACE(MODULE_DBG_ERR, "There are some errors in getLSData()!\n");
        ISP_FREE(pstLSData);
        return GK_FAILURE;
    }

    lc_normalization(pstLSData, pstLSCCaliResult, pstLSCCaliCfg);

    ISP_FREE(pstLSData);
    return s32Ret;

}

/*
 * [ISP_MeshShadingCalibration description]
 * @param pu16SrcRaw     [Input]  Input Raw Data that used for calibration
 * @param pstMLSCCaliCfg [Input]  Input meshLsc calibration config
 * @param pstMLSCTable   [Output] MeshLsc Calibration results
 */
GK_S32 ISP_MeshShadingCalibration(GK_U16 *pu16SrcRaw,
                                  ISP_MLSC_CALIBRATION_CFG_S *pstMLSCCaliCfg,
                                  ISP_MESH_SHADING_TABLE_S *pstMLSCTable)
{
    GK_S32 s32Ret;
    GK_S32 i, j;
    GK_U16 *pu16CropRaw = NULL;

    ISP_LSC_Grid_Step_S stLscGridStepXY;

    pu16CropRaw = (GK_U16 *)ISP_MALLOC(sizeof(GK_U16) * pstMLSCCaliCfg->u16DstImgWidth * pstMLSCCaliCfg->u16DstImgHeight);
    if (pu16CropRaw == NULL) {
        ISP_TRACE(MODULE_DBG_ERR, "malloc memory failed!\n");
        return GK_FAILURE;
    }
    memset(pu16CropRaw, 0, sizeof(GK_U16) * pstMLSCCaliCfg->u16DstImgWidth * pstMLSCCaliCfg->u16DstImgHeight);

    for (j = 0; j < pstMLSCCaliCfg->u16DstImgHeight; j++) {
        for (i = 0; i < pstMLSCCaliCfg->u16DstImgWidth; i++) {
            *(pu16CropRaw + j * pstMLSCCaliCfg->u16DstImgWidth + i) = *(pu16SrcRaw + (j + pstMLSCCaliCfg->u16OffsetY) * pstMLSCCaliCfg->u16ImgWidth + i + pstMLSCCaliCfg->u16OffsetX);
        }
    }

    /* Get Grid info */
    GeometricGridSizeX(pstMLSCCaliCfg, &stLscGridStepXY, pstMLSCTable);

    GeometricGridSizeY(pstMLSCCaliCfg, &stLscGridStepXY, pstMLSCTable);

    // malloc memory
    s32Ret = LSC_GenerateGridInfo(pu16CropRaw, pstMLSCCaliCfg, &stLscGridStepXY, pstMLSCTable);
    if (s32Ret == GK_FAILURE) {
        if (pu16CropRaw != NULL) {
            free(pu16CropRaw);
        }
        ISP_TRACE(MODULE_DBG_ERR, "There are some errors in LSC_GenerateGridInfo()!\n");
        return GK_FAILURE;
    }
    memset(&pstMLSCTable->stBNRLscGainLut, 0, sizeof(ISP_BNR_LSC_GAIN_LUT_S));

    if (pu16CropRaw != NULL) {
        free(pu16CropRaw);
    }
    return GK_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
