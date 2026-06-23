/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef IMX347_SLAVE_CMOS_H
#define IMX347_SLAVE_CMOS_H

#include "ot_common_isp.h"
#include "ot_sns_ctrl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifndef min
#define min(a, b) (((a) > (b)) ?  (b) : (a))
#endif

#ifndef max
#define max(a, b) (((a) > (b)) ?  (a) : (b))
#endif

#define check_ret(express) \
    do { \
        td_s32 ret; \
        ret = express; \
        if (ret != TD_SUCCESS) { \
            printf("Failed at %s: LINE: %d with %#x!\n", __FUNCTION__, __LINE__, ret); \
        } \
    } while (0)

#define IMX347_SLAVE_I2C_ADDR    0x34
#define IMX347_SLAVE_ADDR_BYTE   2
#define IMX347_SLAVE_DATA_BYTE   1
#define imx347_slave_sensor_get_ctx(pipe, ctx)   ((ctx) = imx347_slave_get_ctx(pipe))

#define IMX347_SLAVE_FULL_LINES_MAX 0xFFFFF
#define IMX347_SLAVE_VS_TIME_MAX   0xFFFFFFFF
#define IMX347_SLAVE_FULL_LINES_MAX_2TO1_WDR 0x8AA  /* considering the YOUT_SIZE and bad frame */

#define IMX347_SLAVE_INCREASE_LINES 1 /* make real fps less than stand fps because NVR require */
#define IMX347_SLAVE_VMAX_4M30_LINEAR                    (1650 + IMX347_SLAVE_INCREASE_LINES)

typedef enum {
    IMX347_SLAVE_SENSOR_4M_30FPS_LINEAR_MODE = 0,
    IMX347_SLAVE_MODE_BUTT
} imx347_slave_res_mode;

typedef struct {
    td_u32      inck;
    td_u32      inck_per_hs;
    td_u32      inck_per_vs;
    td_u32      ver_lines;
    td_u32      max_ver_lines;
    td_float    max_fps;
    td_float    min_fps;
    td_u32      width;
    td_u32      height;
    td_u8       sns_mode;
    ot_wdr_mode wdr_mode;
    const char *mode_name;
} imx347_slave_video_mode_tbl;

ot_isp_sns_state *imx347_slave_get_ctx(ot_vi_pipe vi_pipe);
ot_isp_sns_commbus *imx347_slave_get_bus_info(ot_vi_pipe vi_pipe);

td_void imx347_slave_init(ot_vi_pipe vi_pipe);
td_void imx347_slave_exit(ot_vi_pipe vi_pipe);
td_void imx347_slave_standby(ot_vi_pipe vi_pipe);
td_void imx347_slave_restart(ot_vi_pipe vi_pipe);
td_void imx347_mirror_flip(ot_vi_pipe vi_pipe, ot_isp_sns_mirrorflip_type sns_mirror_flip);
td_void imx347_slave_set_blc_clamp_value(ot_vi_pipe vi_pipe, td_bool clamp_en);
td_void imx347_blc_clamp(ot_vi_pipe vi_pipe, ot_isp_sns_blc_clamp sns_blc_clamp);

td_s32  imx347_slave_write_register(ot_vi_pipe vi_pipe, td_u32 addr, td_u32 data);
td_s32  imx347_slave_read_register(ot_vi_pipe vi_pipe, td_u32 addr);
imx347_slave_video_mode_tbl *imx347_get_slave_mode_tbl(td_u8 img_mode);
ot_isp_slave_sns_sync *imx347_get_slave_sync(ot_vi_pipe vi_pipe);
td_s32 imx347_get_slave_bind_dev(ot_vi_pipe vi_pipe);
td_u32 imx347_get_slave_sensor_mode_time(ot_vi_pipe vi_pipe);
ot_isp_sns_state *imx347_get_slave_sns_state(ot_vi_pipe vi_pipe);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif /* IMX347_SLAVE_CMOS_H */
