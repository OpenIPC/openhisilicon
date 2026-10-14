#ifndef __XMEDIA_SVP_STD_H__
#define __XMEDIA_SVP_STD_H__

#include <sys/stat.h>
#include "mkp_npu.h"
#include "xmedia_svp.h"
#include "xmedia_mmz.h"

#define UNSIGNED_8_BIT_NUM 256           // U8类型最大限制
#define SVP_MIN_ATTR_STILLNESS_THRES 0.6 // 静止状态阈值最小限制

#ifdef __cplusplus
extern "C" {
#endif

#define CHECK_SVP_COND_GOTO_ERROR(cond, label, ...)         \
    do {                                                    \
        if (cond) {                                         \
            SVP_TRACE(MODULE_DBG_ERR, __VA_ARGS__);         \
            goto label;                                     \
        }                                                   \
    } while(0);                                             \

#define CHECK_SVP_COND_RETURN_ERROR(cond, ...)              \
    do {                                                    \
        if (cond) {                                         \
            SVP_TRACE(MODULE_DBG_ERR, __VA_ARGS__);         \
            return XMEDIA_FAILURE;                          \
        }                                                   \
    } while(0);                                             \


#define CHECK_CALLOC_RETURN_ERROR(a)                        \
    if (NULL == a) {                                        \
        SVP_TRACE(MODULE_DBG_ERR, "out of memory\n");       \
        return XMEDIA_FAILURE;                              \
    }

#define CHECK_CALLOC_RETURN_NULL(a)                         \
    if (NULL == a) {                                        \
        SVP_TRACE(MODULE_DBG_ERR, "out of memory\n");       \
        return NULL;                                        \
    }

#define CHECK_CALLOC_RETURN_VOID(a)                         \
    if (NULL == a) {                                        \
        SVP_TRACE(MODULE_DBG_ERR, "out of memory\n");       \
        return;                                             \
    }

#define CHECK_CALLOC_GOTO_ERROR(a, label)                   \
    do {                                                    \
        if (a == NULL) {                                    \
            SVP_TRACE(MODULE_DBG_ERR, "out of memory\n");   \
            goto label;                                     \
        }                                                   \
    } while(0)

#define CFREE(a)  \
    if (a) {      \
        free(a);  \
        a = NULL; \
    }

#define STD_MAX(a, b) ((a) > (b) ? (a) : (b))
#define STD_MIN(a, b) ((a) > (b) ? (b) : (a))

#define SVP_CHECK_U8(src_data, ...)                              \
    do {                                                         \
        if ((src_data) < 0 || (src_data) > UNSIGNED_8_BIT_NUM) { \
            SVP_TRACE(MODULE_DBG_ERR, __VA_ARGS__);              \
            return (XMEDIA_FAILURE);                             \
        }                                                        \
    } while (0);

#define SVP_CHECK_ATTR_STILLNESS_THRES(src_data, ...)                        \
    do {                                                                     \
        if ((src_data) <= SVP_MIN_ATTR_STILLNESS_THRES || (src_data) >= 1) { \
            SVP_TRACE(MODULE_DBG_ERR, __VA_ARGS__);                          \
            return (XMEDIA_FAILURE);                                         \
        }                                                                    \
    } while (0);

#define SVP_CHECK_ATTR_THRESHOLD(src_data, ...)     \
    do {                                            \
        if ((src_data) <= 0 || (src_data) >= 1) {   \
            SVP_TRACE(MODULE_DBG_ERR, __VA_ARGS__); \
            return (XMEDIA_FAILURE);                \
        }                                           \
    } while (0);

#define SVP_CHECK_ATTR_NUM(src_data, ...)                                \
    do {                                                                 \
        if ((src_data) <= 0 || (src_data) > XMEDIA_SVP_MAX_TARGET_NUM) { \
            SVP_TRACE(MODULE_DBG_ERR, __VA_ARGS__);                      \
            return (XMEDIA_FAILURE);                                     \
        }                                                                \
    } while (0);

#define SVP_CHECK_ATTR_ENABLE(src_data, ...)                           \
    do {                                                               \
        if ((src_data) != XMEDIA_FALSE && (src_data) != XMEDIA_TRUE) { \
            SVP_TRACE(MODULE_DBG_ERR, __VA_ARGS__);                    \
            return (XMEDIA_FAILURE);                                   \
        }                                                              \
    } while (0);

static __inline__ xmedia_s32 check_file_exist(xmedia_char *filepath)
{
    struct stat file_stat;

    if (stat(filepath, &file_stat) != 0) {
        SVP_TRACE(MODULE_DBG_ERR, "error, model file path err or not exist\n");
            return XMEDIA_FAILURE;
    }
    return XMEDIA_SUCCESS;
}


static __inline__ xmedia_s32 svp_mmz_alloc_and_map(const xmedia_char *mmz_name, xmedia_char *buf_name,
                                                   xmedia_u64 *phy_addr, xmedia_void **virt_addr, xmedia_u32 size)
{
    *phy_addr = xmedia_mmz_alloc(mmz_name, buf_name, size);
    if (*phy_addr != XMEDIA_NULL) {
        *virt_addr = xmedia_mmz_map(*phy_addr, size, XMEDIA_FALSE);
        if (*virt_addr == XMEDIA_NULL) {
            SVP_TRACE(MODULE_DBG_ERR,"xmedia_mmz_map filed, size %d \n", size);
                xmedia_mmz_free(*phy_addr);
                return XMEDIA_FAILURE;
        }
    } else {
        SVP_TRACE(MODULE_DBG_ERR,"xmedia_mmz_alloc filed, size %d \n", size);
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}

static __inline__ xmedia_s32 svp_mmz_alloc_and_map_cache(const xmedia_char *mmz_name, xmedia_char *buf_name,
                                                   xmedia_u64 *phy_addr, xmedia_void **virt_addr, xmedia_u32 size)
{
    *phy_addr = xmedia_mmz_alloc(mmz_name, buf_name, size);
    if (*phy_addr != XMEDIA_NULL) {
        *virt_addr = xmedia_mmz_map(*phy_addr, size, XMEDIA_TRUE);
        if (*virt_addr == XMEDIA_NULL) {
            SVP_TRACE(MODULE_DBG_ERR,"xmedia_mmz_map filed, size %d \n", size);
                xmedia_mmz_free(*phy_addr);
                return XMEDIA_FAILURE;
        }
    } else {
        SVP_TRACE(MODULE_DBG_ERR,"xmedia_mmz_alloc filed, size %d \n", size);
        return XMEDIA_FAILURE;
    }

    return XMEDIA_SUCCESS;
}

static __inline__ xmedia_void svp_mmz_unmap_and_free(xmedia_u64 phy_addr, xmedia_void *virt_addr)
{

    if (virt_addr != XMEDIA_NULL) {
        xmedia_mmz_unmap(virt_addr);
    }
    xmedia_mmz_free(phy_addr);
}

#ifdef __cplusplus
}
#endif

#endif
