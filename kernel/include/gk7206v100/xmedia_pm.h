/*
 * Copyright (c) XMEDIA. All rights reserved.
 */

#ifndef __XMEDIA_PM_H__
#define __XMEDIA_PM_H__

#include <stdbool.h>
#include "xmedia_type.h"
#include "xmedia_errcode.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*
 * xmedia_pm_init接口的输入参数，初始化类型
 * PM_INIT_TYPE_NORMAL:初始化pm模块，不使能str待机功能.
 * PM_INIT_TYPE_STR: 初始化pm模块，并使能str待机功能.
 *
 * 注意: str 待机功能依赖rtc功能.
 *      如果不支持rtc功能，str待机功能也不支持，不能使用PM_INIT_TYPE_STR参数 !!!
 */
typedef enum {
    PM_INIT_TYPE_NORMAL = 0,
    PM_INIT_TYPE_STR,
    PM_INIT_TYPE_MAX,
} xmedia_pm_init_type;

typedef struct {
    xmedia_pm_init_type type;
} xmedia_pm_init_param;

typedef enum {
    PM_WKMODE_WAKEUP = 0,
    PM_WKMODE_STARTUP,
    PM_WKMODE_BUTTON,
    PM_WKMODE_RTC,
    PM_WKMODE_MAX,
} xmedia_pm_wakeup_source;

typedef enum {
    PMC_PWR_RISING_EDGE = 0,
    PMC_PWR_FALLING_EDGE,
    PMC_PWR_HIGH_LEVEL,
    PMC_PWR_LOW_LEVEL,
    PMC_PWR_MAX,
} xmedia_pm_wakeup_polarity;

typedef struct {
    xmedia_bool  rtc_enable;  //    1：表示使能
    xmedia_u32   rtc_timeout; //     单位为10ms
} xmedia_pm_rtc_attr;

typedef struct {
    xmedia_u64   rtc_delta_time; //     单位为1ms
    xmedia_u64   rtc_current_time;
    xmedia_u64   rtc_last_suspend_time;
} xmedia_pm_rtc_info;

typedef struct {
    xmedia_bool  wakeup_enable; //     1：表示使能
    xmedia_pm_wakeup_polarity wakeup_polarity;  //上升沿、下降沿、高电平和低电平触发
} xmedia_pm_wakeup_attr;


typedef struct {
    xmedia_bool button_adj_enable;	//1:表示使能
    xmedia_u32  button_adj_time;	//button触发时间，精度100ms
}xmedia_pm_button_attr;

typedef struct {
    xmedia_pm_wakeup_attr wakeup_attr;
    xmedia_bool startup_enable;
    xmedia_bool button_enable;
    xmedia_pm_button_attr button_attr;
    xmedia_pm_rtc_attr rtc_attr;
} xmedia_pm_attr;

xmedia_s32 xmedia_pm_init(xmedia_void);
xmedia_s32 xmedia_pm_init_ext(xmedia_pm_init_param *param);
xmedia_s32 xmedia_pm_deinit(xmedia_void);
xmedia_s32 xmedia_pm_get_wakeup_source(xmedia_pm_wakeup_source *source);
xmedia_s32 xmedia_pm_get_attr(xmedia_pm_attr *attr);
xmedia_s32 xmedia_pm_set_attr(xmedia_pm_attr *attr);
xmedia_s32 xmedia_pm_get_rtc_info(xmedia_pm_rtc_info *rtc_info);
xmedia_s32 xmedia_pm_suspend(xmedia_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
