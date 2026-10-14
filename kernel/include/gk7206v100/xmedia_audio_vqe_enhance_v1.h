/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __XMEDIA_AUDIO_VQE_ENHANCE_V1_H__
#define __XMEDIA_AUDIO_VQE_ENHANCE_V1_H__
#include "xmedia_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define VQE_V1_MASK_HPF         0x01
#define VQE_V1_MASK_AEC         0x02
#define VQE_V1_MASK_AGC         0x04
#define VQE_V1_MASK_ANR         0x08
#define VQE_V1_MASK_VAD         0x10
#define VQE_V1_MASK_WNS         0x20
#define VQE_V1_MASK_EQ          0x40
#define VQE_V1_MASK_DEREVERB    0x80
#define VQE_V1_MASK_CUSTOMER    0x100
#define VQE_V1_MASK_3A     (VQE_V1_MASK_AEC | VQE_V1_MASK_AGC | VQE_V1_MASK_ANR)
#define VQE_V1_MASK_MAX    (VQE_V1_MASK_HPF | VQE_V1_MASK_AEC | VQE_V1_MASK_AGC | VQE_V1_MASK_ANR | VQE_V1_MASK_VAD | VQE_V1_MASK_WNS | VQE_V1_MASK_EQ | VQE_V1_MASK_DEREVERB | VQE_V1_MASK_CUSTOMER)
#define VQE_V1_LIB_MASK    0x80001

#define VQE_V1_EQ_BAND_MAX      30 /* EQ band num */

typedef enum {
    VQE_V1_HPF_FREQ_80 = 80,
    VQE_V1_HPF_FREQ_120 = 120,
    VQE_V1_HPF_FREQ_150 = 150,
    VQE_V1_HPF_FREQ_MAX,
} vqe_v1_hpf_freq;

typedef enum {
    VQE_V1_USR_MODE_AUTO = 0,
    VQE_V1_USR_MODE_USER = 1,
    VQE_V1_USR_MODE_ADVANCED = 2,
    VQE_V1_USR_MODE_MAX,
} vqe_v1_usr_mode;

typedef enum {
    VQE_V1_ANR_SCENE_NORMAL = 0,
    VQE_V1_ANR_SCENE_MUSIC = 1,
    VQE_V1_ANR_SCENE_MAX,
} vqe_v1_anr_scene;

typedef enum {
    VQE_V1_NR_MODE_SPEECH = 0,
    VQE_V1_NR_MODE_NOISE = 1,
    VQE_V1_NR_MODE_MAX,
} vqe_v1_nr_mode;

typedef struct {
    vqe_v1_usr_mode mode;
    vqe_v1_hpf_freq freq;
} vqe_hpf_attr_v1;

typedef struct {
    vqe_v1_usr_mode mode;
    vqe_v1_anr_scene usr_scene;
    vqe_v1_nr_mode nr_mode;        /* noise suppress mode */
    xmedia_u32 max_suppress_gain; /* maximum of noise suppress gain , range : [5,40] */
    xmedia_u32 suppress_level;    /* noise Suppress level,  range : [0,9] */
    xmedia_u32 nonstationary_suppress_level; /* non-stationary noise suppress level, range : [0,9] */
    xmedia_void* reserved;
} vqe_anr_attr_v1;

typedef struct {
    vqe_v1_usr_mode mode;
    xmedia_s32 target_level;      /* support -20 ~ -1 dB */
    xmedia_u32 max_boost_gain;    /* support 0 ~ 30 Db*/
    xmedia_s32 noise_floor;       /* support -120 ~ -25 Db*/
    xmedia_u32 ratio;             /* small signal suppression, surpport 0 ~ 100 */
    xmedia_u32 attack_time;       /* support 1 ~ 100 */
    xmedia_u32 release_time;      /* support 1 ~ 100 */
    xmedia_void* reserved;
} vqe_agc_attr_v1;

typedef struct {
    vqe_v1_usr_mode mode;
    xmedia_bool cng_enable;                         /* comfort noise generator enable, false : disable true : enable */
    xmedia_u32 freq_boundary;                       /* high and low frequency boundary, range : [1500,3800] */
    xmedia_u32 low_suppress_level;                  /* low freq echo cancellation level,  range : [0,9] */
    xmedia_u32 high_suppress_level;                 /* high freq echo cancellation level,  range : [0,9] */
    xmedia_u32 low_enhance_voice_protect_level;     /* low freq voice protect level, range : [0,9] */
    xmedia_u32 high_enhance_voice_protect_level;    /* high freq voice protect level, range : [0,9] */
    xmedia_void* reserved;
} vqe_aec_attr_v1;

typedef struct {
    xmedia_s8 gain[VQE_V1_EQ_BAND_MAX]; /**< gain level,range:[-50,20]dB */
    xmedia_void* reserved;
} vqe_eq_attr_v1;

typedef struct {
    vqe_v1_usr_mode mode;
    xmedia_u32 sensitivity_level;     /* support 0 ~ 9 */
    xmedia_void* reserved;
} vqe_vad_attr_v1;

typedef struct {
    vqe_v1_usr_mode mode;
    xmedia_u32 suppress_level;    /*support 0 ~ 4 */
    xmedia_void* reserved;
} vqe_wns_attr_v1;

typedef struct {
    vqe_v1_usr_mode mode;
    xmedia_s32 delay_offset;    /*support 5 ~ 30, unit:10ms*/
    xmedia_s32 delay_count;     /*support 3 ~ 10, unit:10ms*/
    xmedia_void* reserved;
} vqe_dereverb_attr_v1;

typedef struct {
    xmedia_u32 mask;
    vqe_hpf_attr_v1 hpf_attr;
    vqe_anr_attr_v1 anr_attr;
    vqe_aec_attr_v1 aec_attr;
    vqe_agc_attr_v1 agc_attr;
    vqe_vad_attr_v1 vad_attr;
    vqe_wns_attr_v1 wns_attr;
    vqe_eq_attr_v1 eq_attr;
    vqe_dereverb_attr_v1 dereverb_attr;
    xmedia_void* customer_attr;
} ai_vqe_attr_v1;

typedef struct {
    xmedia_u32 mask;
    vqe_hpf_attr_v1 hpf_attr;
    vqe_anr_attr_v1 anr_attr;
    vqe_agc_attr_v1 agc_attr;
    vqe_eq_attr_v1 eq_attr;
    xmedia_void* customer_attr;
} ao_vqe_attr_v1;

typedef struct {
    xmedia_void* hpf_handle;
    xmedia_void* anr_handle;
    xmedia_void* aec_handle;
    xmedia_void* agc_handle;
    xmedia_void* vad_handle;
    xmedia_void* wns_handle;
    xmedia_void* eq_handle;
    xmedia_void* resampler_handle;
    xmedia_void* dereverb_handle;
    xmedia_void* customer_handle;
} vqe_handle_v1;

xmedia_void *xmedia_vqe_v1_get_aec_handle(xmedia_void);
xmedia_void *xmedia_vqe_v1_get_agc_handle(xmedia_void);
xmedia_void *xmedia_vqe_v1_get_anr_handle(xmedia_void);
xmedia_void* xmedia_vqe_v1_get_vad_handle(xmedia_void);
xmedia_void *xmedia_vqe_v1_get_eq_handle(xmedia_void);
xmedia_void *xmedia_vqe_v1_get_hpf_handle(xmedia_void);
xmedia_void *xmedia_vqe_v1_get_wns_handle(xmedia_void);
xmedia_void *xmedia_vqe_v1_get_resampler_handle(xmedia_void);
xmedia_void *xmedia_vqe_v1_get_dereverb_handle(xmedia_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
