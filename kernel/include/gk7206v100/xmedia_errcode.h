/*
 * Copyright (c) XMEDIA. All rights reserved.
 */
#ifndef XMEDIA_ERRCODE_H
#define XMEDIA_ERRCODE_H

#include "xmedia_type.h"

#define XMEDIA_ERRCODE_INVALID_PARAM            (xmedia_s32)(0x80010001)
#define XMEDIA_ERRCODE_NULL_PTR                 (xmedia_s32)(0x80010002)
#define XMEDIA_ERRCODE_NOT_ENOUGH_MEMORY        (xmedia_s32)(0x80010003)    // memory allocation fails
#define XMEDIA_ERRCODE_NOT_INIT                 (xmedia_s32)(0x80010004)
#define XMEDIA_ERRCODE_NOT_SUPPORT              (xmedia_s32)(0x80010005)
#define XMEDIA_ERRCODE_TIMEOUT                  (xmedia_s32)(0x80010006)
#define XMEDIA_ERRCODE_BAD_ADDRESS              (xmedia_s32)(0x80010007)
#define XMEDIA_ERRCODE_OPEN_FAILED              (xmedia_s32)(0x80010008)
#define XMEDIA_ERRCODE_CLOSE_FAILED             (xmedia_s32)(0x80010009)
#define XMEDIA_ERRCODE_CREATE_FAILED            (xmedia_s32)(0x8001000A)
#define XMEDIA_ERRCODE_DESTROY_FAILED           (xmedia_s32)(0x8001000B)
#define XMEDIA_ERRCODE_BUFFER_EMPTY             (xmedia_s32)(0x8001000C)
#define XMEDIA_ERRCODE_BUFFER_FULL              (xmedia_s32)(0x8001000D)
#define XMEDIA_ERRCODE_NO_BUFFER_FREE           (xmedia_s32)(0x8001000E)
#define XMEDIA_ERRCODE_NOT_PERMITTED            (xmedia_s32)(0x8001000F)    // Operation not permitted
#define XMEDIA_ERRCODE_EXIST                    (xmedia_s32)(0x80010010)    // Target exists
#define XMEDIA_ERRCODE_NOT_EXIST                (xmedia_s32)(0x80010011)    // Target does not exist
#define XMEDIA_ERRCODE_NOT_READY                (xmedia_s32)(0x80010012)    // Target not ready
#define XMEDIA_ERRCODE_BUSY                     (xmedia_s32)(0x80010013)    // Device or resource busy
#define XMEDIA_ERRCODE_COPY_DATA_ERROR          (xmedia_s32)(0x80010014)
#define XMEDIA_ERRCODE_TYPE_ERROR               (xmedia_s32)(0x80010015)
#define XMEDIA_ERRCODE_CHN_FULL                 (xmedia_s32)(0x80010016)
#define XMEDIA_ERRCODE_INVALID_CHN_ID           (xmedia_s32)(0x80010017)
#define XMEDIA_ERRCODE_INVALID_DEV_ID           (xmedia_s32)(0x80010018)
#define XMEDIA_ERRCODE_INVALID_PIPE_ID          (xmedia_s32)(0x80010019)
#define XMEDIA_ERRCODE_INVALID_GRP_ID           (xmedia_s32)(0x8001001A)
#define XMEDIA_ERRCODE_NOT_CONFIG               (xmedia_s32)(0x8001001B)
#define XMEDIA_ERRCODE_NOT_ENABLE               (xmedia_s32)(0x8001001C)
#define XMEDIA_ERRCODE_NOT_DISABLE              (xmedia_s32)(0x8001001D)
#define XMEDIA_ERRCODE_NOT_BIND                 (xmedia_s32)(0x8001001E)
#define XMEDIA_ERRCODE_BINDED                   (xmedia_s32)(0x8001001F)

#endif /* XMEDIA_ERRCODE_H */
