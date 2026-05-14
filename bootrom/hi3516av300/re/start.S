#include "../../common/arm-asm.h"
#include "hi3516av300-regmap.h"

@ Vector table at BOOTROM_START. Reset branches to _start; the seven ARM
@ exception vectors all redirect to 0x00000100 — which, after SC_CTRL.bit8
@ clears the boot remap, is wherever SDRAM ends up taking over from
@ entry-0. Until then, while remap is in effect, the same address routes
@ back into bootrom + 0x100 = _dead_loop. Either way the SoC ends up in
@ a defined trap on any unhandled exception.

    .text
    .globl _entry
_entry:
    B       _start
    LDR     PC, =0x00000100
    LDR     PC, =0x00000100
    LDR     PC, =0x00000100
    LDR     PC, =0x00000100
    LDR     PC, =0x00000100
    LDR     PC, =0x00000100
    LDR     PC, =0x00000100

    .align 2
    .globl _magic
_magic:
    .word 0x12345678

    .align 2
clear_remap_rom_ptr:
    .word clear_remap_start
clear_remap_ptr:
    .word CLEAR_REMAP_THUNK_DST

FUNC_BEGIN _start
    MRS     R0, CPSR
    BIC     R0, R0, #0x1F                @ clear mode bits
    ORR     R0, R0, #0xD3                @ supervisor mode, IRQ + FIQ masked
    MSR     CPSR_fc, R0
    BL      disable_ctrl_features
    LDR     R0, =STACK_START
    MOV     SP, R0
    BL      configure_soc_clocks         @ defined in bootloader.c (stub)
    LDR     R0, =SDRAM_START
    MOV     R1, #0
    LDR     R2, =(SDRAM_START + 0x268)
    SUBS    R2, R2, R0                   @ R2 = 0x268
    BL      memset                       @ zero first 0x268 bytes of SDRAM
    BL      main                         @ load image (defined in bootloader.c)
    LDR     R0, =clear_remap_start       @ src in bootrom
    LDR     R1, =CLEAR_REMAP_THUNK_DST   @ dst in SDRAM
    LDR     R2, =clear_remap_start
    LDR     R3, =clear_remap_end
    SUB     R2, R3, R2
    BL      memcpy32
    LDR     PC, =CLEAR_REMAP_THUNK_DST   @ jump to thunk now living in SDRAM
FUNC_END _start

@ This block is copied to SDRAM and executed there. It must be position-
@ independent only with respect to the literal pool that follows; the
@ LDRs use a fixed +PC offset, so the layout from clear_remap_start
@ through clear_remap_end is preserved by memcpy32.
clear_remap_start:
    LDR     R4, =SYSCTRL_START
    LDR     R0, [R4]                     @ SC_CTRL
    ORR     R0, R0, #0x100               @ clear boot remap; ENTRY_START → SDRAM
    STR     R0, [R4]
    LDR     R1, =SDRAM_START
    LDR     R0, [R1]                     @ first word of SDRAM = main image entry
    MOV     PC, R0
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
clear_remap_end:

FUNC_BEGIN _dead_loop
    LDR     R1, =SYSCTRL_START
    MOV     R0, #1
    STR     R0, [R1, #SC_SYSRES]         @ soft reset
1:  B       1b
FUNC_END _dead_loop

FUNC_BEGIN memcpy32
    ADD     R2, R0, R2
1:  LDM     R0!, {R3-R10}
    STM     R1!, {R3-R10}
    CMP     R0, R2
    BLE     1b
    BX      LR
FUNC_END memcpy32

FUNC_BEGIN disable_ctrl_features
    MRC     p15, 0, R1, c1, c0, 0
    BIC     R1, R1, #0x1800              @ disable I-cache, branch prediction
    BIC     R1, R1, #0x7                 @ disable D-cache, align check, MMU
    MCR     p15, 0, R1, c1, c0, 0
    BX      LR
FUNC_END disable_ctrl_features

FUNC_BEGIN enable_instruction_cache
    PUSH    {LR}
    MRC     p15, 0, R0, c1, c0, 1
    ORR     R0, R0, #0x40                @ enable L1 parity
    MCR     p15, 0, R0, c1, c0, 1
    BL      invalidate_instruction_pipeline
    MRC     p15, 0, R1, c1, c0, 0
    ORR     R1, R1, #0x1000              @ enable L1 I-cache
    MCR     p15, 0, R1, c1, c0, 0
    ISB     SY
    POP     {PC}
FUNC_END enable_instruction_cache

FUNC_BEGIN disable_instruction_cache
    MRC     p15, 0, R1, c1, c0, 0
    BIC     R1, R1, #0x1000
    MCR     p15, 0, R1, c1, c0, 0
    B       invalidate_instruction_pipeline
FUNC_END disable_instruction_cache

FUNC_BEGIN invalidate_instruction_pipeline
    MOV     R0, #0
    MCR     p15, 0, R0, c8, c7, 0        @ invalidate TLB
    MCR     p15, 0, R0, c7, c5, 0        @ invalidate I-cache
    BX      LR
FUNC_END invalidate_instruction_pipeline
