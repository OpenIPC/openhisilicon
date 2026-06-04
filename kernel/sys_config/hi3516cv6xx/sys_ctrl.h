/*
  Copyright (c), 2001-2025, Shenshu Tech. Co., Ltd.
 */

#ifndef SYS_CTRL_H
#define SYS_CTRL_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef enum {
    VI_ONLINE_VIDEO_NORM_VPSS_ONLINE = 0,   /* OT_VI_ONLINE_VPSS_ONLINE */
    VI_ONLINE_VIDEO_NORM_VPSS_OFFLINE,      /* OT_VI_ONLINE_VPSS_OFFLINE */
    VI_OFFLINE_VIDEO_NORM_VPSS_ONLINE,      /* OT_VI_OFFLINE_VPSS_ONLINE */
    VI_OFFLINE_VIDEO_NORM_VPSS_OFFLINE,     /* OT_VI_OFFLINE_VPSS_OFFLINE */
    VI_VPSS_ONLINE_BUTT
} vi_vpss_online_mode;

typedef enum {
    SYSKOL_SLEEP_MODE_NONE,    /* next sleep with nothing */
    SYSKOL_SLEEP_MODE_LIGHT,   /* next sleep light, only cpu run with slow freq */
    SYSKOL_SLEEP_MODE_DEEP,    /* next sleep deep, all ip need stop working, sensor run with standby */
    SYSKOL_SLEEP_MODE_USER,    /* next sleep with user config, some ip could work commpared with deep mode */
    SYSKOL_SLEEP_MODE_BUTT
} syskol_sleep_mode;

typedef union {
    struct {
        unsigned int audio_cken : 1;
    } bits;
    unsigned int cken_mask;
} syskol_sleep_user_param;

#define VI_VPSS_ONLINE_MODE_DEF VI_OFFLINE_VIDEO_NORM_VPSS_OFFLINE

void sys_ctl(int online_flag);
void update_ctl_by_aiisp(int aiisp_neable);
void syskol_sleep_suspend(syskol_sleep_mode sleep_mode, const syskol_sleep_user_param *user_param);
void syskol_sleep_resume(syskol_sleep_mode sleep_mode, const syskol_sleep_user_param *user_param);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* SYS_CTRL_H */
