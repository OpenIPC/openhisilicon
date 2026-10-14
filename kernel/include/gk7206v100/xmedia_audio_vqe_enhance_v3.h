/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __XMEDIA_AUDIO_VQE_ENHANCE_V3_H__
#define __XMEDIA_AUDIO_VQE_ENHANCE_V3_H__
#include "xmedia_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define VQE_V3_MASK_HPF         0x01
#define VQE_V3_MASK_AGC         0x02
#define VQE_V3_MASK_ANR         0x04
#define VQE_V3_MASK_EQ          0x08
#define VQE_V3_MASK_CUSTOMER    0x10
#define VQE_V3_MASK_3A     (VQE_V3_MASK_AGC | VQE_V3_MASK_ANR)
#define VQE_V3_MASK_MAX    (VQE_V3_MASK_HPF | VQE_V3_MASK_AGC | VQE_V3_MASK_ANR |  VQE_V3_MASK_EQ | VQE_V3_MASK_CUSTOMER)
#define VQE_V3_LIB_MASK    0x80007

#define  xmedia_vqe_v3_get_eq_handle    xmedia_vqe_v1_get_eq_handle
#define  xmedia_vqe_v3_get_hpf_handle   xmedia_vqe_v1_get_hpf_handle

#define VQE_V3_EQ_BAND_MAX      30 /* EQ band num */

typedef enum {
    VQE_V3_HPF_FREQ_80 = 80,
    VQE_V3_HPF_FREQ_120 = 120,
    VQE_V3_HPF_FREQ_150 = 150,
    VQE_V3_HPF_FREQ_MAX,
} vqe_v3_hpf_freq;

typedef enum {
    VQE_V3_USR_MODE_AUTO = 0,
    VQE_V3_USR_MODE_USER = 1,
    VQE_V3_USR_MODE_ADVANCED = 2,
    VQE_V3_USR_MODE_MAX,
} vqe_v3_usr_mode;

typedef enum {
    VQE_V3_ANR_SCENE_NORMAL = 0,
    VQE_V3_ANR_SCENE_MUSIC = 1,
    VQE_V3_ANR_SCENE_MAX,
} vqe_v3_anr_scene;

typedef enum {
    VQE_V3_NR_MODE_SPEECH = 0,
    VQE_V3_NR_MODE_NOISE = 1,
    VQE_V3_NR_MODE_MAX,
} vqe_v3_nr_mode;

typedef struct {
    vqe_v3_usr_mode mode;
    vqe_v3_hpf_freq freq;
} vqe_hpf_attr_v3;

typedef struct {
    vqe_v3_usr_mode mode;
    xmedia_u32 level;             /* noise Suppress level,  range : [0,3] */
    xmedia_void* reserved;
} vqe_anr_attr_v3;

typedef struct {
    vqe_v3_usr_mode mode;
    xmedia_s32 target_level;      /* support -20 ~ -1 dB */
    xmedia_u32 max_boost_gain;    /* support 0 ~ 30 Db*/
    xmedia_s32 noise_floor;       /* support -120 ~ -25 Db*/
    xmedia_u32 ratio;             /* small signal suppression, surpport 0 ~ 100 */
    xmedia_u32 attack_time;       /* support 1 ~ 100 */
    xmedia_u32 release_time;      /* support 1 ~ 100 */
    xmedia_void* reserved;
} vqe_agc_attr_v3;

typedef struct {
    xmedia_s8 gain[VQE_V3_EQ_BAND_MAX]; /**< gain level,range:[-50,20]dB */
    xmedia_void* reserved;
} vqe_eq_attr_v3;

typedef struct {
    xmedia_u32 mask;
    vqe_hpf_attr_v3 hpf_attr;
    vqe_anr_attr_v3 anr_attr;
    vqe_agc_attr_v3 agc_attr;
    vqe_eq_attr_v3 eq_attr;
    xmedia_void* customer_attr;
} ai_vqe_attr_v3;

typedef struct {
    xmedia_void* hpf_handle;
    xmedia_void* anr_handle;
    xmedia_void* agc_handle;
    xmedia_void* eq_handle;
    xmedia_void* resampler_handle;
    xmedia_void* customer_handle;
} vqe_handle_v3;

xmedia_void *xmedia_vqe_v3_get_agc_handle(xmedia_void);
xmedia_void *xmedia_vqe_v3_get_anr_handle(xmedia_void);
xmedia_void *xmedia_vqe_v3_get_eq_handle(xmedia_void);
xmedia_void *xmedia_vqe_v3_get_hpf_handle(xmedia_void);
xmedia_void *xmedia_vqe_v3_get_resampler_handle(xmedia_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
