#ifndef HICOMPAT_H
#define HICOMPAT_H

/* Everything the prototypes below name. This header used to be included only
 * from the *_cmos.c files, which pull in gk_api_{isp,ae,awb}.h first and so
 * happened to have all of it already; a *_sensor_ctl.c includes only
 * gk_api_isp.h, and there the AE and AWB prototypes would not compile. All of
 * these are guarded, so naming them here costs a translation unit nothing. */
#include "ae_comm.h"
#include "awb_comm.h"
#include "comm_3a.h"
#include "comm_isp.h"
#include "comm_sns.h"
#include "common.h"

#if defined(SDK_CODE) && SDK_CODE+0 != 0
#if SDK_CODE == 0x3516E200
#define GK_API_ISP_SensorRegCallBack HI_MPI_ISP_SensorRegCallBack
#define GK_API_ISP_SensorUnRegCallBack HI_MPI_ISP_SensorUnRegCallBack
#define GK_API_AE_SensorRegCallBack HI_MPI_AE_SensorRegCallBack
#define GK_API_AE_SensorUnRegCallBack HI_MPI_AE_SensorUnRegCallBack
#define GK_API_AWB_SensorRegCallBack HI_MPI_AWB_SensorRegCallBack
#define GK_API_AWB_SensorUnRegCallBack HI_MPI_AWB_SensorUnRegCallBack
#define GK_API_ISP_GetModParam HI_MPI_ISP_GetModParam
#endif

GK_S32 GK_API_ISP_SensorRegCallBack(VI_PIPE ViPipe,
				    ISP_SNS_ATTR_INFO_S *pstSnsAttrInfo,
				    ISP_SENSOR_REGISTER_S *pstRegister);
GK_S32 GK_API_ISP_SensorUnRegCallBack(VI_PIPE ViPipe, SENSOR_ID SensorId);

GK_S32 GK_API_AE_SensorRegCallBack(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib,
				   ISP_SNS_ATTR_INFO_S *pstSnsAttrInfo,
				   AE_SENSOR_REGISTER_S *pstRegister);
GK_S32 GK_API_AE_SensorUnRegCallBack(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib,
				     SENSOR_ID SensorId);

GK_S32 GK_API_AWB_SensorRegCallBack(VI_PIPE ViPipe, ALG_LIB_S *pstAwbLib,
				    ISP_SNS_ATTR_INFO_S *pstSnsAttrInfo,
				    AWB_SENSOR_REGISTER_S *pstRegister);
GK_S32 GK_API_AWB_SensorUnRegCallBack(VI_PIPE ViPipe, ALG_LIB_S *pstAwbLib,
				      SENSOR_ID SensorId);

/* Declared here for the same reason as the six above: the #define rewrites the
 * call, so a prototype has to exist under whichever name it rewrites to. Used
 * by the drivers that read the ISP's quick-start flag. */
GK_S32 GK_API_ISP_GetModParam(ISP_MOD_PARAM_S *pstModParam);
#endif /* SDK_CODE != 0 */

#endif /* HICOMPAT_H */
