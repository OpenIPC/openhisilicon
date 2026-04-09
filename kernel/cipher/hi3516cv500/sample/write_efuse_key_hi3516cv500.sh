#!/bin/sh
# Build cipher drivers in kernel.
# Copyright (C) HiSilicon Technologies Co., Ltd. 2018-2019. All rights reserved.

set -e

#Set Key Index 1
himm 0x100B0008 0x01

#set key0~key7 and CRC16
himm 0x100B000C 0x11111111
himm 0x100B0010 0x11111111
himm 0x100B0014 0x11111111
himm 0x100B0018 0x11111111
himm 0x100B001C 0x11111111
himm 0x100B0020 0x11111111
himm 0x100B0024 0x11111111
himm 0x100B0028 0x11111111
#himm 0x100B002C 0x11111111

#Set mode, write key
himm 0x100B0000 0x02

#start write key to OTP 1
himm 0x100B0004 0x1acce551

sleep 1
himd.l 0x100B004C
#should be 0x00001202, Bit2 should be 1.


