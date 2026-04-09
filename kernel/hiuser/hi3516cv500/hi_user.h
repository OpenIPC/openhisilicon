/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2016-2019. All rights reserved.
 * Description: user driver.
 * Author: Hisilicon multimedia software group
 * Create: 2016-11-11
 */

#ifndef __HI_USER_H__
#define __HI_USER_H__

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/version.h>
#include <linux/pagemap.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/mm.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <linux/sched.h>

#include "hi_osal.h"
#include "vdec_exp.h"
#include "vou_exp.h"

#ifdef __cplusplus
extern "C" {
#endif

#define VDEC_NOTIFY 0
#ifdef __HuaweiLite__
#define VDEC_SET_SCHEDULER 1
#else
#define VDEC_SET_SCHEDULER 0
#endif
#define VO_NOTIFY 0

extern VDEC_EXPORT_SYMBOL_S  g_stVdecExpSymbol;
extern VOU_EXPORT_SYMBOL_S g_vou_exp_symbol;

#ifdef __cplusplus
}
#endif
#endif