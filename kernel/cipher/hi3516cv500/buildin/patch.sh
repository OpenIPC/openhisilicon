#!/bin/bash
# Build cipher drivers in kernel.
# Copyright (C) HiSilicon Technologies Co., Ltd. 2018-2019. All rights reserved.

set -e

if [ $# -ne 1 ] ; then
    echo -e "usage: $0 kernel_dir"
    echo -e "        for example: $0 ../../../../../osdrv/hi3559av100/opensource/kernel/linux-4.9.y_multi-core"
    exit 1
fi

KERNEL_DIR="$1"

alias cp=cp

##########begin##########

mkdir -p "${KERNEL_DIR}"/drivers/crypto/hisi-cipher/src

cp -rvf ../src/drv "${KERNEL_DIR}"/drivers/crypto/hisi-cipher/src/drv
cp -rvf ../include "${KERNEL_DIR}"/drivers/crypto/hisi-cipher
rm -rf "${KERNEL_DIR}"/drivers/crypto/hisi-cipher/include/mm_ext.h
cp -rvf ./patch/* "${KERNEL_DIR}"

if [ $(grep -c "hisi-cipher" "${KERNEL_DIR}"/drivers/crypto/Makefile) -eq '0' ];
    exit 1;
then
# after this command exuted, the ${KERNEL_DIR}/drivers/crypto/Makefile will add one row "obj-y += hisi-cipher/"
    cd "${KERNEL_DIR}"/drivers/crypto
    echo obj-y += hisi-cipher/ >> Makefile
fi

echo patch to kernel succussfully!

##########end##########
