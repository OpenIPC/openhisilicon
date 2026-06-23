/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef OT_KM_IOCTL_CMD_H
#define OT_KM_IOCTL_CMD_H
#include "crypto_ioctl.h"
#include "crypto_common_struct.h"
#include "crypto_km_struct.h"

#define IOC_TYPE_KM    'n'

typedef enum {
    /* Keyslot. */
    KM_KEYSLOT_IOC_NR_CREATE_HANDLE,
    KM_KEYSLOT_IOC_NR_DESTROY_HANDLE,
    /* Klad. */
    KM_KLAD_IOC_NR_CREATE_HANDLE,
    KM_KLAD_IOC_NR_DESTROY_HANDLE,
    KM_KLAD_IOC_NR_ATTACH,
    KM_KLAD_IOC_NR_DETACH,
    KM_KLAD_IOC_NR_SET_ATTR,
    KM_KLAD_IOC_NR_GET_ATTR,
    KM_KLAD_IOC_NR_SET_SESSION_KEY,
    KM_KLAD_IOC_NR_SET_CONTENT_KEY,
    KM_KLAD_IOC_NR_SET_CLEAR_KEY,
    /* kdf. */
    KM_KDF_IOC_NR_PBKDF2,
    KM_IOC_NR_BUTT,
} km_ioc_nr;

/* Keyslot. */
typedef struct {
    crypto_handle kapi_keyslot_handle;
    km_keyslot_type keyslot_type;
} keyslot_create_ctl_t;

typedef struct {
    crypto_handle kapi_keyslot_handle;
} keyslot_destroy_ctl_t;

/* Klad. */
typedef struct {
    crypto_handle kapi_klad_handle;
} klad_handle_ctl_t;

typedef struct {
    crypto_handle kapi_klad_handle;
    km_klad_dest_type klad_type;
    crypto_handle kapi_keyslot_handle;
} klad_attach_ctl_t;

typedef struct klad_attr_ctl {
    crypto_handle kapi_klad_handle;
    km_klad_attr attr;
} klad_attr_ctl_t;

typedef struct klad_set_session_key_ctl {
    crypto_handle kapi_klad_handle;
    km_klad_level level;
    km_klad_alg_type alg;
    td_u32 key_size;
    crypto_compat_addr key;
} klad_set_session_key_ctl_t;

typedef struct klad_set_content_key_ctl {
    crypto_handle kapi_klad_handle;
    km_klad_alg_type alg;
    km_klad_key_parity key_parity;
    td_u32 key_size;
    crypto_compat_addr key;
} klad_set_content_key_ctl_t;

typedef struct klad_set_clear_key_ctl {
    crypto_handle kapi_klad_handle;
    km_klad_hmac_type hmac_type;
    km_klad_key_parity key_parity;
    td_u32 key_size;
    crypto_compat_addr key;
} klad_set_clear_key_ctl_t;

#define KM_IOWR(nr, struct_name)           CRYPTO_IOC(CRYPTO_IOC_RW, IOC_TYPE_KM, (nr), sizeof(struct_name))

/* Keyslot. */
#define CMD_KEYSLOT_CREATE_HANDLE       KM_IOWR(KM_KEYSLOT_IOC_NR_CREATE_HANDLE, keyslot_create_ctl_t)
#define CMD_KEYSLOT_DESTROY_HANDLE      KM_IOWR(KM_KEYSLOT_IOC_NR_DESTROY_HANDLE, keyslot_destroy_ctl_t)

/* Klad. */
#define CMD_KLAD_CREATE_HANDLE          KM_IOWR(KM_KLAD_IOC_NR_CREATE_HANDLE, klad_handle_ctl_t)
#define CMD_KLAD_DESTROY_HANDLE         KM_IOWR(KM_KLAD_IOC_NR_DESTROY_HANDLE, klad_handle_ctl_t)
#define CMD_KLAD_ATTACH                 KM_IOWR(KM_KLAD_IOC_NR_ATTACH, klad_attach_ctl_t)
#define CMD_KLAD_DETACH                 KM_IOWR(KM_KLAD_IOC_NR_DETACH, klad_attach_ctl_t)
#define CMD_KLAD_SET_ATTR               KM_IOWR(KM_KLAD_IOC_NR_SET_ATTR, klad_attr_ctl_t)
#define CMD_KLAD_GET_ATTR               KM_IOWR(KM_KLAD_IOC_NR_GET_ATTR, klad_attr_ctl_t)
#define CMD_KLAD_SET_SESSION_KEY        KM_IOWR(KM_KLAD_IOC_NR_SET_SESSION_KEY, klad_set_session_key_ctl_t)
#define CMD_KLAD_SET_CONTENT_KEY        KM_IOWR(KM_KLAD_IOC_NR_SET_CONTENT_KEY, klad_set_content_key_ctl_t)
#define CMD_KLAD_SET_CLEAR_KEY          KM_IOWR(KM_KLAD_IOC_NR_SET_CLEAR_KEY, klad_set_clear_key_ctl_t)

#define CMD_KM_MAX_NUM                  KM_IOC_NR_BUTT

#endif