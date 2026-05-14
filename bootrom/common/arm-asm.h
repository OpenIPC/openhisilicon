#ifndef BOOTROM_COMMON_ARM_ASM_H
#define BOOTROM_COMMON_ARM_ASM_H

.macro FUNC_BEGIN name
    .align 2
    .global \name
    .syntax unified
    .arm
    .type \name, %function
\name:
.endm

.macro FUNC_END name
    .size \name, .-\name
.endm

#endif
