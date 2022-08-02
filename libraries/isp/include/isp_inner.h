/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#ifndef __ISP_INNER_H__
#define __ISP_INNER_H__

#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include "mkp_isp.h"
#include "isp_math_utils.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define MPI_ISP_PARAM_CHECK_RETURN(x, min, max, fmt, ...)                  \
    do {                                                                   \
        if ((x) < (min) || (x) > (max)) {                                  \
            printf("[Func]:%s [Line]:%d [Info]:", __FUNCTION__, __LINE__); \
            printf("%#x " fmt, x, ##__VA_ARGS__);                          \
            return ERR_CODE_ISP_ILLEGAL_PARAM;                               \
        }                                                                  \
    } while (0)

#define MPI_ISP_MAX_PARAM_CHECK_RETURN(x, max, fmt, ...)                   \
    do {                                                                   \
        if ((x) > (max)) {                                                 \
            printf("[Func]:%s [Line]:%d [Info]:", __FUNCTION__, __LINE__); \
            printf("%#x " fmt, x, ##__VA_ARGS__);                          \
            return ERR_CODE_ISP_ILLEGAL_PARAM;                               \
        }                                                                  \
    } while (0)

#define MPI_ISP_ARRAY_PARAM_CHECK_RETURN(x, type, size, min, max, fmt, ...)     \
    do {                                                                        \
        int loops = size;                                                       \
        type *p = (type *)x;                                                    \
        while (loops--) {                                                       \
            if ((p[loops]) < (min) || (p[loops]) > (max)) {                     \
                printf("[Func]:%s [Line]:%d [Info]:", __FUNCTION__, __LINE__);  \
                printf("array[%d] = %#x " fmt, loops, p[loops], ##__VA_ARGS__); \
                return ERR_CODE_ISP_ILLEGAL_PARAM;                                \
            }                                                                   \
        }                                                                       \
    } while (0)

#define MPI_ISP_ARRAY_MAX_PARAM_CHECK_RETURN(x, type, size, max, fmt, ...)      \
    do {                                                                        \
        int loops = size;                                                       \
        type *p = (type *)x;                                                    \
        while (loops--) {                                                       \
            if ((p[loops]) > (max)) {                                           \
                printf("[Func]:%s [Line]:%d [Info]:", __FUNCTION__, __LINE__);  \
                printf("array[%d] = %#x " fmt, loops, p[loops], ##__VA_ARGS__); \
                return ERR_CODE_ISP_ILLEGAL_PARAM;                                \
            }                                                                   \
        }                                                                       \
    } while (0)

#ifdef _MSC_VER
#define MUTEX_INIT_LOCK(mutex) InitializeCriticalSection(&mutex)
#define MUTEX_LOCK(mutex) EnterCriticalSection(&mutex)
#define MUTEX_UNLOCK(mutex) LeaveCriticalSection(&mutex)
#define MUTEX_DESTROY(mutex) DeleteCriticalSection(&mutex)
#else
#define MUTEX_INIT_LOCK(mutex)                  \
    do {                                        \
        (void)pthread_mutex_init(&mutex, NULL); \
    } while (0)
#define MUTEX_LOCK(mutex)                 \
    do {                                  \
        (void)pthread_mutex_lock(&mutex); \
    } while (0)
#define MUTEX_UNLOCK(mutex)                 \
    do {                                    \
        (void)pthread_mutex_unlock(&mutex); \
    } while (0)
#define MUTEX_DESTROY(mutex)                 \
    do {                                     \
        (void)pthread_mutex_destroy(&mutex); \
    } while (0)
#endif

GK_S32 ISP_SetCalibrateAttr(VI_PIPE ViPipe, ISP_FPN_CALIBRATE_ATTR_S *pstCalibrate);
GK_S32 ISP_SetCorrectionAttr(VI_PIPE ViPipe, const ISP_FPN_ATTR_S *pstCorrection);
GK_S32 ISP_GetCorrectionAttr(VI_PIPE ViPipe, ISP_FPN_ATTR_S *pstCorrection);
GK_S32 ISP_GetFeFocusStatistics(VI_PIPE ViPipe, ISP_FE_FOCUS_STATISTICS_S *pstFEAFStat, ISP_STAT_S *pstIspActStat,
                                GK_U8 u8WdrChn);
GK_S32 ISP_GetAEStitchStatistics(VI_PIPE ViPipe, ISP_AE_STITCH_STATISTICS_S *pstAeStitchStat);
GK_S32 ISP_GetWBStitchStatistics(VI_PIPE ViPipe, ISP_WB_STITCH_STATISTICS_S *pstStitchWBStat);
GK_S32 ISP_MeshShadingCalibration(GK_U16 *pu16SrcRaw, ISP_MLSC_CALIBRATION_CFG_S *pstMLSCCaliCfg,
                                  ISP_MESH_SHADING_TABLE_S *pstMLSCTable);
GK_S32 ISP_GetLightboxGain(VI_PIPE ViPipe, ISP_AWB_Calibration_Gain_S *stAWBCalibrationGain);
GK_S32 ISP_SetRadialShadingAttr(VI_PIPE ViPipe, const ISP_RADIAL_SHADING_ATTR_S *pstRaShadingAttr);
GK_S32 ISP_GetRadialShadingAttr(VI_PIPE ViPipe, ISP_RADIAL_SHADING_ATTR_S *pstRaShadingAttr);
GK_S32 ISP_SetRadialShadingLUT(VI_PIPE ViPipe, const ISP_RADIAL_SHADING_LUT_ATTR_S *pstRaShadingLutAttr);
GK_S32 ISP_GetRadialShadingLUT(VI_PIPE ViPipe, ISP_RADIAL_SHADING_LUT_ATTR_S *pstRaShadingLutAttr);
GK_S32 ISP_SetPipeDifferAttr(VI_PIPE ViPipe, const ISP_PIPE_DIFF_ATTR_S *pstPipeDiffer);
GK_S32 ISP_GetPipeDifferAttr(VI_PIPE ViPipe, ISP_PIPE_DIFF_ATTR_S *pstPipeDiffer);
GK_S32 ISP_SetRcAttr(VI_PIPE ViPipe, const ISP_RC_ATTR_S *pstRcAttr);
GK_S32 ISP_GetRcAttr(VI_PIPE ViPipe, ISP_RC_ATTR_S *pstRcAttr);
GK_S32 ISP_SetRGBIRAttr(VI_PIPE ViPipe, const ISP_RGBIR_ATTR_S *pstRGBIRAttr);
GK_S32 ISP_GetRGBIRAttr(VI_PIPE ViPipe, ISP_RGBIR_ATTR_S *pstRGBIRAttr);
GK_S32 ISP_SetPreLogLUTAttr(VI_PIPE ViPipe, const ISP_PRELOGLUT_ATTR_S *pstPreLogLUTAttr);
GK_S32 ISP_GetPreLogLUTAttr(VI_PIPE ViPipe, ISP_PRELOGLUT_ATTR_S *pstPreLogLUTAttr);
GK_S32 ISP_SetLogLUTAttr(VI_PIPE ViPipe, const ISP_LOGLUT_ATTR_S *pstLogLUTAttr);
GK_S32 ISP_GetLogLUTAttr(VI_PIPE ViPipe, ISP_LOGLUT_ATTR_S *pstLogLUTAttr);
GK_S32 ISP_GetAEGridInfo(VI_PIPE ViPipe, ISP_AE_GRID_INFO_S *pstFEGridInfo, ISP_AE_GRID_INFO_S *pstBEGridInfo);
GK_S32 ISP_GetAFGridInfo(VI_PIPE ViPipe, ISP_FOCUS_GRID_INFO_S *pstFEGridInfo, ISP_FOCUS_GRID_INFO_S *pstBEGridInfo);
GK_S32 ISP_GetWBGridInfo(VI_PIPE ViPipe, ISP_AWB_GRID_INFO_S *pstBEGridInfo);
GK_S32 ISP_GetMGGridInfo(VI_PIPE ViPipe, ISP_MG_GRID_INFO_S *pstGridInfo);

GK_S32 ISP_SetClutCoeff(VI_PIPE ViPipe, const ISP_CLUT_LUT_S *pstClutLUT);
GK_S32 ISP_GetClutCoeff(VI_PIPE ViPipe, ISP_CLUT_LUT_S *pstClutLUT);
GK_S32 ISP_SetClutAttr(VI_PIPE ViPipe, const ISP_CLUT_ATTR_S *pstClutAttr);
GK_S32 ISP_GetClutAttr(VI_PIPE ViPipe, ISP_CLUT_ATTR_S *pstClutAttr);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
