/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef MMZ_COMMON_H
#define MMZ_COMMON_H

#include "autoconf.h"
#include "mmz.h"

#define MMZ_GRAIN   PAGE_SIZE

#define mmz_align2(x, g)        (((g) == 0) ? (0) : ((((x) + (g) - 1) / (g)) * (g)))
#define mmz_grain_align(x)      mmz_align2(x, MMZ_GRAIN)

#ifdef CONFIG_OT_LOG_TRACE_SUPPORT
#define mmz_printk(fmt...) osal_printk(fmt)
#else
#define mmz_printk(fmt...)
#endif

#define mmz_trace(level, fmt, params...)    \
    mmz_printk(level "mmz_userdev:%s: \n" fmt, __FUNCTION__, ##params)

#define mmz_err_trace(fmt, params...)   mmz_trace(KERN_ERR, fmt, ##params)
#define mmz_warn_trace(fmt, params...)  mmz_trace(KERN_WARNING, fmt, ##params)

#define MMZ_DBG_LEVEL       0x0
#define MMZ_INFO_LEVEL      0x1

/* do not print by default */
#define mmz_trace_ctrl(level, fmt, params...)       \
    do {                                            \
        if (level & MMZ_DBG_LEVEL) {                \
            mmz_trace(KERN_INFO, fmt, ##params);    \
        }                                           \
    } while (0)

#define mmz_debug_info(fmt, params...)  mmz_trace_ctrl(MMZ_INFO_LEVEL, fmt, ##params)

#define MEDIA_MEM_NAME  "media-mem"

#define MMZ_PARAM_NUM   4

struct mmb_node {
    struct mmb_info mmb_info;
    int map_ref;
    int mmb_ref;
    struct osal_list_head list;
    ot_mmb_t *mmb;
} __attribute__((aligned(8)));

int ot_mmz_register(ot_mmz_t *zone);
int ot_mmz_unregister(ot_mmz_t *zone);

int ot_mmb_get(ot_mmb_t *mmb);
int ot_mmb_put(ot_mmb_t *mmb);

int ot_mmz_get_anony(void);
struct osal_list_head *ot_mmz_get_mmz_list(void);

static inline int mmz_default_filter(const ot_mmz_t *mmz, unsigned long gfp, const char *mmz_name)
{
    if ((gfp != 0) && (mmz->gfp != gfp)) {
        return -1;
    }
    if ((mmz_name == NULL) || (*mmz_name == '\0')) {
        if (ot_mmz_get_anony() != 1) {
            return -1;
        }
        mmz_name = "anonymous";
    }
    return (strcmp(mmz_name, mmz->name) == 0) ? 0 : -1;
}

ot_mmb_t *media_mem_get_mmb_by_phys(unsigned long phys_addr);
void media_mem_put_mmb(ot_mmb_t *mmb);

/* for mmz userdev */
int mmz_userdev_init(void);
void mmz_userdev_exit(void);

#endif /* MMZ_COMMON_H */
