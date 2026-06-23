/*
  Copyright (c), 2001-2024, Shenshu Tech. Co., Ltd.
 */

#ifndef IOCTL_CMD_OTP_H
#define IOCTL_CMD_OTP_H

#include "crypto_ioctl.h"
#include "ot_type.h"

#define IOC_TYPE_OTP    'm'

typedef enum {
    /* OTP. */
    OTP_IOC_NR_READ_WORD,
    OTP_IOC_NR_READ_BYTE,
    OTP_IOC_NR_WRITE_BYTE,
    OTP_IOC_NR_BUTT,
} otp_ioc_nr;

typedef struct {
    td_u32 addr;
    td_u32 word;
} otp_word_ctl_t;

typedef struct {
    td_u32 addr;
    td_u8 byte;
} otp_byte_ctl_t;

#define OTP_IOWR(nr, struct_name)           CRYPTO_IOC(CRYPTO_IOC_RW, IOC_TYPE_OTP, (nr), sizeof(struct_name))

#define CMD_OTP_READ_WORD           OTP_IOWR(OTP_IOC_NR_READ_WORD, otp_word_ctl_t)
#define CMD_OTP_READ_BYTE           OTP_IOWR(OTP_IOC_NR_READ_BYTE, otp_byte_ctl_t)
#define CMD_OTP_WRITE_BYTE          OTP_IOWR(OTP_IOC_NR_WRITE_BYTE, otp_byte_ctl_t)

#define CMD_OTP_MAX_NUM              OTP_IOC_NR_BUTT

#endif