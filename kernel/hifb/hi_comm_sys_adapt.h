/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2007-2019. All rights reserved.
 * Description: chip specific configure data structure of sys
 * Author: Hisilicon multimedia software group
 * Create: 2007-01-30
 */

#ifndef __HI_COMM_SYS_ADAPT_H__
#define __HI_COMM_SYS_ADAPT_H__

#include "hi_comm_video_adapt.h"
#include "hi_comm_sys.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */


typedef struct {
    hi_bool all_online;

    hi_u32 frame_rate; /* input frame rate of VIPROC(linear/frame mode WDR is the sensor frame rate,
                          * and the line mode WDR is the frame rate after synthesis) */
    hi_u32 full_lines_std; /* total height of sensor, include of v_blank */

    hi_size large_stream_size; /* VENC large stream size */
    hi_size small_stream_size; /* VENC small stream size */

} hi_vpss_venc_wrap_param;

typedef struct {
    hi_u32 align;

} hi_mpp_sys_config;

typedef struct {
    hi_u64 phy_addr;
    hi_bool cached;
} hi_sys_vir_mem_info;

typedef struct {
    hi_u32 num;
    hi_mpp_chn mpp_chn[BIND_DEST_MAXNUM];
} hi_mpp_bind_dest;

typedef SCALE_RANGE_E hi_scale_range_type;

typedef COEFF_LEVEL_E hi_coeff_level;

typedef struct {
    hi_coeff_level hor_lum; /* horizontal luminance   coefficient level */
    hi_coeff_level hor_chr; /* horizontal chrominance coefficient level */
    hi_coeff_level ver_lum; /* vertical   luminance   coefficient level */
    hi_coeff_level ver_chr; /* vertical   chrominance coefficient level */
} hi_scale_coeff_level;

typedef struct {
    hi_scale_range_type horizontal;
    hi_scale_range_type vertical;
} hi_scale_range;

typedef struct {
    hi_scale_range scale_range;
    hi_scale_coeff_level scale_coeff_level;
} hi_scale_coeff_info;

typedef struct {
    hi_char gps_latitude_ref; /* GPS latitude_ref indicates whether the latitude is north or south latitude,
                               * 'N'/'S', default 'N' */
    hi_u32 gps_latitude[3][2]; /* GPS latitude is expressed as degrees, minutes and seconds, a typical format
                                   * like "dd/1, mm/1, ss/1", default 0/0, 0/0, 0/0 */
    hi_char gps_longitude_ref; /* GPS longitude_ref indicates whether the longitude is east or west longitude,
                                * 'E'/'W', default 'E' */
    hi_u32 gps_longitude[3][2]; /* GPS longitude is expressed as degrees, minutes and seconds, a typical format
                                    * like "dd/1, mm/1, ss/1", default 0/0, 0/0, 0/0 */
    hi_u8 gps_altitude_ref; /* GPS altitude_ref indicates the reference altitude used, 0 - above sea level,
                             * 1 - below sea level default 0 */
    hi_u32 gps_altitude[2]; /* GPS altitude_ref indicates the altitude based on the reference gps_altitude_ref,
                                * the reference unit is meters, default 0/0 */
} hi_gps_info;

typedef VI_VPSS_MODE_E hi_vi_vpss_mode_type;

typedef struct {
    hi_vi_vpss_mode_type mode[VI_MAX_PIPE_NUM];
} hi_vi_vpss_mode;

typedef struct {
    hi_u32 comp_ratio_10_bit; /* compression ratio of 10 bit */
    hi_u32 comp_ratio_12_bit; /* compression ratio of 12 bit */
    hi_u32 comp_ratio_14_bit; /* compression ratio of 14 bit */
} hi_raw_frame_compress_param;



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_COMM_SYS_ADAPT_H__ */

