/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __XMEDIA_AUDIO_VQE_DETECT_H__
#define __XMEDIA_AUDIO_VQE_DETECT_H__

#include "xmedia_type.h"
#include "xmedia_audio_vqe_bcd.h"

#ifdef __cplusplus
extern "C" {
#endif

#define VQE_DETECT_MASK_HPF     0x01000000
#define VQE_DETECT_MASK_BCD     0x02000000
#define VQE_DETECT_MASK_VED     0x04000000
#define VQE_DETECT_MASK_SSL     0x08000000
#define VQE_DETECT_MASK_GBD     0x10000000

#define VQE_DETECT_MASK_MAX    (VQE_DETECT_MASK_HPF | VQE_DETECT_MASK_BCD | VQE_DETECT_MASK_VED | VQE_DETECT_MASK_SSL | VQE_DETECT_MASK_GBD)
#define VQE_DETECT_LIB_MASK    0x80004

#define xmedia_vqe_detect_get_hpf_handle    xmedia_vqe_v2_get_hpf_handle
typedef xmedia_s32 (*fn_detect_ssl_callback)(xmedia_void* private_data, xmedia_s32 out_angle); /* Sound Source Localization be noticed Function Handle type */
typedef xmedia_s32 (*fn_detect_ved_callback)(xmedia_void*, xmedia_bool, xmedia_bool, xmedia_s32); /* Volume Mutation Detection be noticed Function Handle type */
typedef xmedia_s32 (*fn_detect_gbd_callback)(xmedia_void* private_data); /* glass break detect be noticed Function Handle type */

typedef enum {
    VQE_DET_USR_MODE_AUTO = 0,
    VQE_DET_USR_MODE_USER = 1,
    VQE_DET_USR_MODE_ADVANCED = 2,
    VQE_DET_USR_MODE_MAX,
} vqe_detect_usr_mode;

typedef struct {
    xmedia_u32 mic_distance;          /* 30mm ~ 150mm */
    xmedia_u32 sensitivity_level;     /* range [0 ~ 9] */
    xmedia_u32 shape;                 /* 0:linear array, 1:circular array */
    fn_detect_ssl_callback callback;  /* the callback function pointer */
    xmedia_void* private_data;        /* the callback private data */
    xmedia_void* reserved;
} vqe_ssl_config;

typedef struct {
    vqe_detect_usr_mode mode;
    xmedia_s32 volume_threshold;       // current volume threshold for event detection, support -80 ~ 0 db
    xmedia_bool mutation_flag;         // flag for mutation detection enable
    xmedia_s32 fast_change_threshold;  // fast changing threshold for mutation detection, support 10 ~ 80 db
    xmedia_s32 slow_change_threshold;  // slow changing threshold for mutation detection, support 1 ~ 20 db
    fn_detect_ved_callback callback;   // the callback function pointer
    xmedia_void* private_data;         // the callback private data
} vqe_ved_config;

typedef struct {
    xmedia_bool usr_mode;                  /* mode 0: auto mode 1: mannual. */
    xmedia_bool bypass;                    /* value 0: enable function; value 1: bypass function. */
    xmedia_u32 alarm_threshold;            /* alarm threshold. range[0, 100] */
    xmedia_u32 time_limit;                 /* alarm time limit(s), range [0, 120]. */
    xmedia_u32 time_limit_threshold_count; /* alarm time limit threshold count, range [1, 5]. */
    xmedia_u32 interval_time;              /* alarm interval time to unfreeze(s),  [0, 120]*/
    fn_detect_gbd_callback callback;       /* the callback function pointer */
    audio_load_model gbd_model;            /*for load npu xmm,arm not use*/
    xmedia_void* private_data;             /* the callback private data */
} vqe_gbd_config;

typedef enum {
    VQE_DETECT_HPF_FREQ_80 = 80,
    VQE_DETECT_HPF_FREQ_120 = 120,
    VQE_DETECT_HPF_FREQ_150 = 150,
    VQE_DETECT_HPF_FREQ_MAX,
} vqe_detect_hpf_freq;

typedef struct {
    vqe_detect_usr_mode mode;
    vqe_detect_hpf_freq freq;
} vqe_hpf_attr_detect;

typedef struct {
    xmedia_u32 mask;
    vqe_hpf_attr_detect hpf_attr;
    audio_bcd_config bcd_attr;
    vqe_ssl_config ssl_attr;
    vqe_ved_config ved_attr;
    vqe_gbd_config gbd_attr;
} ai_vqe_attr_detect;

typedef struct {
    xmedia_void*  bcd_handle;
    xmedia_void*  hpf_handle;
    xmedia_void*  ved_handle;
    xmedia_void*  ssl_handle;
    xmedia_void*  gbd_handle;
} vqe_handle_detect;

xmedia_void *xmedia_vqe_detect_get_hpf_handle(xmedia_void);
xmedia_void *xmedia_vqe_detect_get_bcd_handle(xmedia_void);
xmedia_void *xmedia_vqe_detect_get_ved_handle(xmedia_void);
xmedia_void *xmedia_vqe_detect_get_ssl_handle(xmedia_void);
xmedia_void *xmedia_vqe_detect_get_gbd_handle(xmedia_void);
#ifdef __cplusplus
}
#endif
#endif
