/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef MKP_MEM_H
#define MKP_MEM_H

#include "ot_common.h"
#include "ot_debug.h"

#define mem_emerg_trace(fmt, ...)   \
    OT_EMERG_TRACE(OT_ID_MEM, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define mem_alert_trace(fmt, ...)   \
    OT_ALERT_TRACE(OT_ID_MEM, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define mem_crit_trace(fmt, ...)    \
    OT_CRIT_TRACE(OT_ID_MEM, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define mem_err_trace(fmt, ...)     \
    OT_ERR_TRACE(OT_ID_MEM, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define mem_warn_trace(fmt, ...)    \
    OT_WARN_TRACE(OT_ID_MEM, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define mem_notice_trace(fmt, ...)  \
    OT_NOTICE_TRACE(OT_ID_MEM, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define mem_info_trace(fmt, ...)    \
    OT_INFO_TRACE(OT_ID_MEM, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define mem_debug_trace(fmt, ...)   \
    OT_DEBUG_TRACE(OT_ID_MEM, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#endif /* MKP_MEM_H */
