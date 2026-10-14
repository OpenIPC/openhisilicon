/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __XMEDIA_AUDIO_VQE_ENHANCE_V2_H__
#define __XMEDIA_AUDIO_VQE_ENHANCE_V2_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define VQE_V2_MASK_HPF         0x00100
#define VQE_V2_MASK_AEC         0x00200
#define VQE_V2_MASK_AGC         0x00400
#define VQE_V2_MASK_ANR         0x00800
#define VQE_V2_MASK_BF          0x01000
#define VQE_V2_MASK_VAD         0x02000
#define VQE_V2_MASK_WNS         0x04000
#define VQE_V2_MASK_EQ          0x08000
#define VQE_V2_MASK_DEREVERB    0x10000
#define VQE_V2_MASK_CUSTOMER    0x20000
#define VQE_V2_MASK_3A     (VQE_V2_MASK_AEC | VQE_V2_MASK_AGC | VQE_V2_MASK_ANR)
#define VQE_V2_MASK_MAX    (VQE_V2_MASK_HPF | VQE_V2_MASK_AEC | VQE_V2_MASK_AGC | VQE_V2_MASK_ANR | VQE_V2_MASK_BF | VQE_V2_MASK_VAD | VQE_V2_MASK_WNS | VQE_V2_MASK_EQ | VQE_V2_MASK_DEREVERB | VQE_V2_MASK_CUSTOMER)
#define VQE_V2_LIB_MASK    0x80003

#define EQ_V2_BAND_MAX      30 /* EQ band num */

typedef enum {
    VQE_V2_USR_MODE_AUTO = 0,
    VQE_V2_USR_MODE_USER = 1,
    VQE_V2_USR_MODE_ADVANCED = 2,
    VQE_V2_USR_MODE_MAX,
} vqe_v2_usr_mode;

typedef enum {
    VQE_V2_ANR_SCENE_NORMAL = 0,
    VQE_V2_ANR_SCENE_MUSIC = 1,
    VQE_V2_ANR_SCENE_MAX,
} vqe_v2_anr_scene;

typedef enum {
    VQE_V2_NR_MODE_SPEECH = 0,
    VQE_V2_NR_MODE_NOISE = 1,
    VQE_V2_NR_MODE_MAX,
} vqe_v2_nr_mode;

typedef enum {
    VQE_V2_HPF_FREQ_80 = 80,
    VQE_V2_HPF_FREQ_120 = 120,
    VQE_V2_HPF_FREQ_150 = 150,
    VQE_V2_HPF_FREQ_MAX,
} vqe_v2_hpf_freq;

typedef struct {
    vqe_v2_usr_mode mode;
    vqe_v2_hpf_freq freq;
} vqe_hpf_attr_v2;

typedef struct {
    vqe_v2_usr_mode mode;
    vqe_v2_anr_scene usr_scene;
    vqe_v2_nr_mode nr_mode;       /* noise suppress mode */
    xmedia_u32 max_suppress_gain; /* maximum of noise suppress gain , range : [5,40] */
    xmedia_u32 suppress_level;    /* noise Suppress level,  range : [0,9] */
    xmedia_u32 nonstationary_suppress_level; /* non-stationary noise suppress level, range : [0,9] */
    xmedia_void* reserved;
} vqe_anr_attr_v2;

typedef struct {
    vqe_v2_usr_mode mode;
    xmedia_s32 target_level;       /* expected reacheable level, support -20 ~ -1 dB */
    xmedia_u32 max_boost_gain;     /* maximum magnification of input signal, support 0 ~ 30 dB */
    xmedia_s32 noise_floor;        /* signal and noise level boundary, support -120 ~ -25 dB */
    xmedia_u32 ratio;              /* small signal suppression, surpport 0 ~ 100 */
    xmedia_u32 attack_time;        /* velocity of input signal up to target level, support 1 ~ 100 */
    xmedia_u32 release_time;       /* velocity of input signal down to target level, support 1 ~ 100 */
    xmedia_void* reserved;
} vqe_agc_attr_v2;

typedef struct {
    vqe_v2_usr_mode mode;
    xmedia_bool cng_enable;               /* comfort noise generator enable, false : disable true : enable */
    xmedia_u32 freq_boundary;             /* high and low frequency boundary, range : [1500,3800] */
    xmedia_u32 low_suppress_level;        /* low freq echo cancellation level, range : [0,9] */
    xmedia_u32 high_suppress_level;       /* high freq echo cancellation level, range : [0,9] */
    xmedia_u32 low_enhance_voice_protect_level;     /* low freq voice protect level, range : [0,9] */
    xmedia_u32 high_enhance_voice_protect_level;    /* high freq voice protect level, range : [0,9] */
    xmedia_void* reserved;
} vqe_aec_attr_v2;

typedef struct {
    xmedia_void* reserved;
} vqe_bf_attr_v2;

typedef struct {
    vqe_v2_usr_mode mode;
    xmedia_u32 sensitivity_level;       /* vad sensitivity level, range : [0,9] */
    xmedia_void* reserved;
} vqe_vad_attr_v2;

typedef struct {
    vqe_v2_usr_mode mode;
    xmedia_u32 suppress_level;        /*support 0 ~ 4 */
    xmedia_void* reserved;
} vqe_wns_attr_v2;

typedef struct {
    xmedia_s8 gain[EQ_V2_BAND_MAX];    /**< gain level,range:[-50,20]dB */
    xmedia_void* reserved;
} vqe_eq_attr_v2;

typedef struct {
    vqe_v2_usr_mode mode;
    xmedia_s32 delay_offset;    /*support 5 ~ 30, unit:10ms*/
    xmedia_s32 delay_count;     /*support 3 ~ 10, unit:10ms*/
    xmedia_void* reserved;
} vqe_dereverb_attr_v2;

typedef struct {
    xmedia_u32 mask;
    vqe_hpf_attr_v2 hpf_attr;
    vqe_anr_attr_v2 anr_attr;
    vqe_aec_attr_v2 aec_attr;
    vqe_agc_attr_v2 agc_attr;
    vqe_bf_attr_v2 bf_attr;
    vqe_vad_attr_v2 vad_attr;
    vqe_wns_attr_v2 wns_attr;
    vqe_eq_attr_v2 eq_attr;
    vqe_dereverb_attr_v2 dereverb_attr;
    xmedia_void* customer_attr;
} ai_vqe_attr_v2;

typedef struct {
    xmedia_u32 mask;
    vqe_hpf_attr_v2 hpf_attr;
    vqe_anr_attr_v2 anr_attr;
    vqe_agc_attr_v2 agc_attr;
    vqe_eq_attr_v2 eq_attr;
    xmedia_void* customer_attr;
} ao_vqe_attr_v2;

typedef struct {
    xmedia_void* hpf_handle;
    xmedia_void* anr_handle;
    xmedia_void* aec_handle;
    xmedia_void* agc_handle;
    xmedia_void* bf_handle;
    xmedia_void* vad_handle;
    xmedia_void* wns_handle;
    xmedia_void* eq_handle;
    xmedia_void* resampler_handle;
    xmedia_void* dereverb_handle;
    xmedia_void* customer_handle;
} vqe_handle_v2;

/*v2 handle*/
xmedia_void *xmedia_vqe_v2_get_aec_handle(xmedia_void);
xmedia_void *xmedia_vqe_v2_get_agc_handle(xmedia_void);
xmedia_void *xmedia_vqe_v2_get_anr_handle(xmedia_void);
xmedia_void *xmedia_vqe_v2_get_vad_handle(xmedia_void);
xmedia_void *xmedia_vqe_v2_get_bf_handle(xmedia_void);
xmedia_void *xmedia_vqe_v2_get_eq_handle(xmedia_void);
xmedia_void *xmedia_vqe_v2_get_hpf_handle(xmedia_void);
xmedia_void *xmedia_vqe_v2_get_wns_handle(xmedia_void);
xmedia_void *xmedia_vqe_v2_get_resampler_handle(xmedia_void);
xmedia_void *xmedia_vqe_v2_get_dereverb_handle(xmedia_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
