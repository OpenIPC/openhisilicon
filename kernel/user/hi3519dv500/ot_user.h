/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef OT_USER_H
#define OT_USER_H

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/version.h>
#include <linux/pagemap.h>
#include <linux/mm.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <uapi/linux/sched/types.h>

#include "ot_vdec_export.h"
#include "ot_vo_export.h"
#include "ot_vo_dev_export.h"
#include "ot_ao_export.h"
#include "ot_osal.h"

#ifdef __cplusplus
extern "C" {
#endif

#define VDEC_NOTIFY   0
#define AO_NOTIFY     0
#define VO_NOTIFY     0

#ifdef __cplusplus
}
#endif

#endif
