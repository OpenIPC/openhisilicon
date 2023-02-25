/*
 * OpenIPC.org
 */

#if !defined(__BT656_CMOS_H_)
#define __BT656_CMOS_H_

#include "comm_sns.h"
#include "comm_video.h"
#include "gk_api_ae.h"
#include "gk_api_awb.h"
#include "gk_api_isp.h"
#include "sns_ctrl.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/****************************************************************************
 * global variables                                                            *
 ****************************************************************************/

ISP_SNS_STATE_S *g_pastBT656[ISP_MAX_PIPE_NUM] = {GK_NULL};

ISP_SNS_COMMBUS_U g_aunBT656BusInfo[ISP_MAX_PIPE_NUM] = {
    [0] = {.s8I2cDev = 0}, [1 ... ISP_MAX_PIPE_NUM - 1] = {.s8I2cDev = -1}};

#define BT656_SENSOR_GET_CTX(dev, pstCtx) (pstCtx = g_pastBT656[dev])
#define BT656_SENSOR_SET_CTX(dev, pstCtx) (g_pastBT656[dev] = pstCtx)
#define BT656_SENSOR_RESET_CTX(dev) (g_pastBT656[dev] = GK_NULL)

//
/****************************************************************************
 * extern                                                                   *
 ****************************************************************************/
extern const unsigned int bt656_i2c_addr;
extern unsigned int bt656_addr_byte;
extern unsigned int bt656_data_byte;

extern void bt656_init(VI_PIPE ViPipe);
extern void bt656_exit(VI_PIPE ViPipe);
extern void bt656_standby(VI_PIPE ViPipe);
extern void bt656_restart(VI_PIPE ViPipe);
extern int sensor_write_register(VI_PIPE ViPipe, int addr, int data);
extern int sensor_read_register(VI_PIPE ViPipe, int addr);

/****************************************************************************
 * callback structure                                                       *
 ****************************************************************************/
static GK_S32 bt656_set_bus_info(VI_PIPE ViPipe,
                                 ISP_SNS_COMMBUS_U unSNSBusInfo) {
  g_aunBT656BusInfo[ViPipe].s8I2cDev = unSNSBusInfo.s8I2cDev;

  return GK_SUCCESS;
}

static GK_S32 sensor_ctx_init(VI_PIPE ViPipe) {
  ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

  BT656_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);

  if (pastSnsStateCtx == GK_NULL) {
    pastSnsStateCtx = (ISP_SNS_STATE_S *)malloc(sizeof(ISP_SNS_STATE_S));
    if (pastSnsStateCtx == GK_NULL) {
      ISP_TRACE(MODULE_DBG_ERR, "Isp[%d] SnsCtx malloc memory failed!\n",
                ViPipe);
      return ERR_CODE_ISP_NOMEM;
    }
  }

  memset(pastSnsStateCtx, 0, sizeof(ISP_SNS_STATE_S));

  BT656_SENSOR_SET_CTX(ViPipe, pastSnsStateCtx);

  return GK_SUCCESS;
}

static GK_VOID sensor_ctx_exit(VI_PIPE ViPipe) {
  ISP_SNS_STATE_S *pastSnsStateCtx = GK_NULL;

  BT656_SENSOR_GET_CTX(ViPipe, pastSnsStateCtx);
  SENSOR_FREE(pastSnsStateCtx);
  BT656_SENSOR_RESET_CTX(ViPipe);
}

static GK_S32 sensor_register_callback(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib,
                                       ALG_LIB_S *pstAwbLib) {
  GK_S32 s32Ret;

  s32Ret = sensor_ctx_init(ViPipe);
  if (s32Ret != GK_SUCCESS) {
    return GK_FAILURE;
  }

  bt656_init(ViPipe);

  return GK_SUCCESS;
}

static GK_S32 sensor_unregister_callback(VI_PIPE ViPipe, ALG_LIB_S *pstAeLib,
                                         ALG_LIB_S *pstAwbLib) {
  sensor_ctx_exit(ViPipe);

  return GK_SUCCESS;
}

static GK_S32 sensor_set_init(VI_PIPE ViPipe, ISP_INIT_ATTR_S *pstInitAttr) {
  return GK_SUCCESS;
}

ISP_SNS_OBJ_S stBt656_Obj = {.pfnRegisterCallback = sensor_register_callback,
                             .pfnUnRegisterCallback =
                                 sensor_unregister_callback,
                             .pfnStandby = bt656_standby,
                             .pfnRestart = bt656_restart,
                             .pfnMirrorFlip = GK_NULL,
                             .pfnWriteReg = sensor_write_register,
                             .pfnReadReg = sensor_read_register,
                             .pfnSetBusInfo = bt656_set_bus_info,
                             .pfnSetInit = sensor_set_init};

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __BT656_CMOS_H_ */
