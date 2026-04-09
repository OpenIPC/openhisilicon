/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Function of imx307_2l_cmos.h
 * Author: ISP SW
 * Create: 2012/06/28
 */

#ifndef __IMX307_2L_CMOS_H_
#define __IMX307_2L_CMOS_H_

#include "hi_comm_isp.h"
#include "hi_sns_ctrl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define IMX307_2L_I2C_ADDR    0x34
#define IMX307_2L_ADDR_BYTE   2
#define IMX307_2L_DATA_BYTE   1
#define IMX307_2L_SENSOR_GET_CTX(dev, pstCtx)   ((pstCtx) = imx307_2l_get_ctx(dev))

ISP_SNS_STATE_S *imx307_2l_get_ctx(VI_PIPE vi_pipe);
ISP_SNS_COMMBUS_U *imx307_2l_get_bus_Info(VI_PIPE vi_pipe);
unsigned char imx307_2l_get_serdes_i2c_addr(VI_PIPE vi_pipe);

typedef struct {
    HI_U32      verti_lines;
    HI_U32      max_verti_lines;
    HI_FLOAT    max_fps;
    HI_FLOAT    min_fps;
    HI_U32      width;
    HI_U32      height;
    HI_U8       sns_mode;
    WDR_MODE_E  wdr_mode;
    const char *mode_name;
} imx307_2l_video_mode_tbl;

void imx307_2l_init(VI_PIPE vi_pipe);
void imx307_2l_exit(VI_PIPE vi_pipe);
void imx307_2l_standby(VI_PIPE vi_pipe);
void imx307_2l_restart(VI_PIPE vi_pipe);
int  imx307_2l_write_register(VI_PIPE vi_pipe, HI_U32 addr, HI_U32 data);
int  imx307_2l_read_register(VI_PIPE vi_pipe, HI_U32 addr);
int  imx307_2l_set_bus_ex_info(VI_PIPE vi_pipe, ISP_SNS_BUS_EX_S *serdes_info);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif /* __IMX307_2l_CMOS_H_ */
